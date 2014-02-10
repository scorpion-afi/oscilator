
// FIR declaration

#ifdef FIR
  #define GLB_FIR 		
#else	
  #define GLB_FIR extern
#endif

// to declaration of 'bool' type
#include <stdbool.h>

// size of input array with data to filtering (in items)
// if round-robin buffer is used, IN_SIZE is almost half-size of round-robin buffer
// total size of round-robin buffer is 255 + IN_SIZE * 2,
// where 255 is linked with max order of fir filter (255)
#define IN_SIZE 256 

GLB_FIR void fir_filter( unsigned int* p_in, unsigned int* p_out, bool first_half );

#ifdef FIR

  // filter order (number of coefficients is N + 1 (with coefficient with index 0) )
  #define N 8 // max order is 255

  // coefficients of fir filter
  static unsigned int coef[N + 1] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
  
  static unsigned int* p_coef = coef;

#endif