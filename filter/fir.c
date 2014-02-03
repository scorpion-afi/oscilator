
// filter order (number of coefficient is N + 1 (with coefficient with index 0) )
#define N 8
#define IN_SIZE 1024 // size of input array with data to filtering (in items)

extern void fir_filter( const unsigned int* p_in, unsigned int* p_out, const unsigned int* p_coef );


// FIR filter implementation

// number of " 'приложений' фильтра к отсчетам входного сигнала" 
#define NUMBER_OF_PASS ( (IN_SIZE - 1) - N + 1 )
#define N_PLUS_1 (N + 1)

// p_in - pointer to data to filtering (size is IN_SIZE)
// p_out - 'filtered' data
// p_koef - pointer to coefficients (size is N + 1)

// size of p_out must be equal or greater then NUMBER_OF_PASS !!!
// to increase performance, no any verifications are doing    !!!

// implementation of FIR filter
//===========================================================================================
void fir_filter( const unsigned int* p_in, unsigned int* p_out, const unsigned int* p_coef )
{

  if( first_half )
  {
	  p_in += IN_SIZE - N
  }
  for( unsigned int i = 0; i < NUMBER_OF_PASS; i++ )
  {	
    for( unsigned int j = 0; j < N_PLUS_1; j++ )
	  *p_out += *p_in++ * *p_coef++; 
	  
	// return p_coef to initial state (подготовка к следующему 'приложению' фильтра)
	p_coef -= N_PLUS_1;
	  
	// preparing to next filter operation (подготовка к следующему 'приложению' фильтра)
	p_in -= N;
	    
	p_out++;
  }
}
