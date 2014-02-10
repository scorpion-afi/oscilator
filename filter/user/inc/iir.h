
#ifdef IIR
  #define GLB_IIR 			
#else	
  #define GLB_IIR extern
#endif

// to declaration of 'bool' type
#include <stdbool.h>

// size of input array with data to filtering (in items)
// if round-robin buffer is used, IN_SIZE is almost half-size of round-robin buffer
// total size of round-robin buffer is 127 + IN_SIZE * 2,
// where 127 is linked with max order of iir filter (127)
#define IN_SIZE 256

GLB_IIR void iir_filter( unsigned int* p_in, unsigned int* p_out, bool first_half );

#ifdef IIR

  // filter order (number of coefficients is N + 1 (with coefficient with index 0) )
  // max order is 127
  #define N 6 // a
  #define M 4 // b

  // coefficients of iir filter
  static unsigned int coef_a[N] = { 5, 4, 3, 2, 1, 0 };    // recursive part  (are in back order)
  static unsigned int coef_b[M + 1] = { 0, 1, 2, 3, 4 };

  static unsigned int* p_coef_a = coef_a;
  static unsigned int* p_coef_b = coef_b;

#endif
