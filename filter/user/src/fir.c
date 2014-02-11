
// FIR filter implementation

#define FIR
#define FS_96
#include "fir.h"

#include <string.h> // to memcpy() using

// number of " 'приложений' фильтра к отсчетам входного сигнала" 
#define NUMBER_OF_PASS  IN_SIZE
#define N_PLUS_1        (N + 1)

// p_in - pointer to data to filtering, p_in must points to samples with index 255,
// so p_in is not points to begin of input array, this will happen inside function
// p_out - 'filtered' data
// first_half - this flag has meaningful only if round-robin buffer is used,
// in this case it must be switched in each round-robin buffer switching

// size of p_in must be equal IN_SIZE + 255                   !!!
// size of p_out must be equal NUMBER_OF_PASS                 !!!
// to increase performance, no any verifications are doing    !!!

// if round-robin buffer is used size of it must be 255 + IN_SIZE * 2 (and IN_SIZE * 2 for output)
// if round-robin buffer is not used first_half must be false,
// and first 255 samples may be set to zero

// result of filtering may take max 32 bits
// (12 bits input samples, 12 bits coefficients and max order of filter 255)
// so, for 12 bit DAC, it must be truncated

// implementation of FIR filter
//===========================================================================================
void fir_filter( unsigned int* p_in, unsigned int* p_out, bool first_half )
{ 
  // to filter samples in 'tail' 
  p_in -= N;
  
  // moving 'tail', only if round-robin buffer is used 
  if( first_half )
    memcpy( p_in, p_in + IN_SIZE*2, N );  

  for( unsigned int i = 0; i < NUMBER_OF_PASS; i++ )
  {	
    *p_out = 0;

    for( unsigned int j = 0; j < N_PLUS_1; j++ )
      *p_out += *p_in++ * *p_coef++; 
	  
    // return p_coef to initial state (подготовка к следующему 'приложению' фильтра)
    p_coef -= N_PLUS_1;
	  
    // preparing to next filter operation (подготовка к следующему 'приложению' фильтра)
    p_in -= N;
    
    // get only 12 MSBs
    *p_out >>= 20;
    
    p_out++;
  }
}
