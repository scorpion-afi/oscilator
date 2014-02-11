
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
  //#define N 36 // max order is 255

  // coefficients of fir filter

#ifdef FS_24

  #define N 36
  //coefficients for fir filter with A pass = 1dB, A stop = 120dB, F pass = 9.6 kHz, F stop = 12 kHz and Fs = 24kHz
  static unsigned int coef[N + 1] = {   
                                        0,    6,      0,     37,      0,    125,      0,    315,      0,
                                      640,    0,   1091,      0,   1602,      0,   2054,      0,   2322,
                                    30411, 2322,      0,   2054,      0,   1602,      0,   1091,      0,
                                      640,    0,    315,      0,    125,      0,     37,      0,      6,
                                        0 
                                    };
#endif 
  
#ifdef FS_48
  
  #define N 73  
  //coefficients for fir filter with A pass = 1dB, A stop = 120dB, F pass = 9.6 kHz, F stop = 12 kHz and Fs = 48kHz
  static unsigned int coef[N + 1] = {   
                                        0,      1,     21,     88,    216,    352,    370,    185,      0,
                                        0,      0,    174,    194,      0,      0,      0,    263,    225,
                                        0,      0,     25,    477,    230,      0,      0,    291,    843,
                                       86,      0,      0,   1043,   1576,      0,      0,      0,   6302,
                                    12897,  12897,   6302,      0,      0,      0,   1576,   1043,      0,
                                        0,     86,    843,    291,      0,      0,    230,    477,     25,
                                        0,      0,    225,    263,      0,      0,      0,    194,    174,
                                        0,      0,      0,    185,    370,    352,    216,     88,     21,
                                        1,      0
                                    }; 
#endif 
  
#ifdef FS_96
  
  #define N 146    
  //coefficients for fir filter with A pass = 1dB, A stop = 120dB, F pass = 9.6 kHz, F stop = 12 kHz and Fs = 96kHz
  static unsigned int coef[N + 1] = {   
                                         0,      1,      2,      6,     14,     28,     51,     81,    117,
                                      153,    182,    196,    185,    148,     87,     12,      0,      0,
                                        0,      0,      0,     31,     91,    116,     97,     39,      0,
                                        0,      0,      0,      0,     60,    133,    155,    112,     15,
                                        0,      0,      0,      0,     14,    153,    239,    228,    113,
                                        0,      0,      0,      0,      0,    147,    351,    421,    309,
                                       41,      0,      0,      0,      0,     68,    523,    818,    787,
                                      374,      0,      0,      0,      0,      0,   1187,   3152,   5062,
                                     6447,   6953,   6447,   5062,   3152,   1187,      0,      0,      0,
                                        0,      0,    374,    787,    818,    523,     68,      0,      0,
                                        0,      0,     41,    309,    421,    351,    147,      0,      0,
                                        0,      0,      0,    113,    228,    239,    153,     14,      0,
                                        0,      0,      0,     15,    112,    155,    133,     60,      0,
                                        0,      0,      0,      0,     39,     97,    116,     91,     31,
                                        0,      0,      0,      0,      0,     12,     87,    148,    185,
                                      196,    182,    153,    117,     81,     51,     28,     14,      6,
                                        2,      1,      0
                                    }; 
#endif 
   
  static unsigned int* p_coef = coef;

#endif