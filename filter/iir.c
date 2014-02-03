
#define N 6 // a
#define M 4 // b
#define IN_SIZE 1024 // size of input array with data to filtering (in items)

extern void iir_filter( const unsigned int* p_in, unsigned int* p_out, const unsigned int* p_a,
                        register const unsigned int* p_b );

unsigned int p_a[N] = {};
unsigned int p_b[M] = {};

// IIR filter implementation

// number of " 'приложений' фильтра к отсчетам входного сигнала" 
#define NUMBER_OF_PASS ( (IN_SIZE - 1) - M + 1 )
#define M_PLUS_1 (M + 1)

// p_in - pointer to data to filtering (size is IN_SIZE)
// p_out - 'filtered' data (first N samples will be considered as previosly samples)
// p_a - pointer to coefficients (size is N)
// p_b - pointer to coefficients (size is M + 1) (must be in back order)

// size of p_out must be equal or greater then NUMBER_OF_PASS !!!
// to increase performance, no any verifications are doing    !!!

// implementation of IIR filter
//==============================================================================
void iir_filter( const unsigned int* p_in, unsigned int* p_out, const unsigned int* p_a,
		         register const unsigned int* p_b )
{
  unsigned int j;
  unsigned int* p_out_prev;
  
  // shifting to last + 1 output samples
  p_out += N;
  p_out_prev = p_out;
  
  for( unsigned int i = 0; i < NUMBER_OF_PASS; i++ )
  {	   
	// non-recursive part
    for( j = 0; j < M_PLUS_1; j++ )
      *p_out += *p_in++ * *p_b++; 
    
    // recursive part
    for( j = 0; j < N; j++ )
      *p_out += *--p_out_prev * *p_a++; 

    p_out++;
  
    // preparing to next filter itaration (подготовка к следующему 'приложению' фильтра)
    // for non-recursive part
    p_in -= M;
    p_b -= M_PLUS_1;
  
    // preparing to next filter itaration (подготовка к следующему 'приложению' фильтра)
    // for recursive part
    p_out_prev = p_out;
    p_a -= N;
  }
}
