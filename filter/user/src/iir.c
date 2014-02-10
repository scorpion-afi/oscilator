
// IIR filter implementation

#define IIR
#include "iir.h"

// number of " 'приложений' фильтра к отсчетам входного сигнала" 
#define NUMBER_OF_PASS IN_SIZE
#define M_PLUS_1 (M + 1)
#define N_MINUS_1 (N - 1)

// p_in - pointer to data to filtering (size is IN_SIZE)
// p_out - 'filtered' data (first N samples will be considered as previosly samples)
// p_a - pointer to coefficients (size is N)  (must be in back order)
// p_b - pointer to coefficients (size is M + 1)

// size of p_out must be equal or greater then NUMBER_OF_PASS !!!
// to increase performance, no any verifications are doing    !!!

// implementation of IIR filter
//==============================================================================
void iir_filter( unsigned int* p_in, unsigned int* p_out, bool first_half )
{
  unsigned int j;
  unsigned int* p_out_prev;
  
  // to filter samples in 'tail'
  p_in -= M;
  p_out -= N;

  // moving 'tail', only if round-robin buffer is used
  if( first_half )
  {
	 memcpy( p_in, p_in + IN_SIZE*2, M );
	 memcpy( p_out, p_out + IN_SIZE*2, N );
  }

  //
  p_out_prev = p_out;

  // shifting to last + 1 output samples
  p_out += N;
  
  for( unsigned int i = 0; i < NUMBER_OF_PASS; i++ )
  {	   
	*p_out = 0;

	// non-recursive part
    for( j = 0; j < M_PLUS_1; j++ )
      *p_out += *p_in++ * *p_coef_b++;
    
    // recursive part
    for( j = 0; j < N; j++ )
      *p_out += *p_out_prev++ * *p_coef_a++;

    // get only 12 MSBs
    *p_out >>= 20;

    p_out++;
  
    // preparing to next filter itaration (подготовка к следующему 'приложению' фильтра)
    // for non-recursive part
    p_in -= M;
    p_coef_b -= M_PLUS_1;
  
    // preparing to next filter itaration (подготовка к следующему 'приложению' фильтра)
    // for recursive part
    p_out_prev -= N_MINUS_1;
    p_coef_a -= N;
  }
}
