
#ifdef IIR
  #define GLB_IIR 			
#else	
  #define GLB_IIR extern
#endif

GLB_IIR void iir_filter( const unsigned int* p_in, unsigned int* p_out, const unsigned int* p_a,
                        register const unsigned int* p_b );

#ifdef IIR

  #define N 6 // a
  #define M 4 // b
  #define IN_SIZE 1024 // size of input array with data to filtering (in items)

#endif