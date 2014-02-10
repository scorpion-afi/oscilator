
#include "stm32f10x.h"

//#include "fir.h"
#include "iir.h"

/*
unsigned int round_robin_in[IN_SIZE*2 + 255];
unsigned int round_robin_out[IN_SIZE*2];

// fir filter
//===========================================================================================
void main( void )
{ 
  fir_filter( round_robin_in + 255, round_robin_out, false );
  
  set_sync_level();
  while( 1 )
  {  
    fir_filter( round_robin_in + 255, round_robin_out, true );
    fir_filter( round_robin_in + 255 + IN_SIZE, round_robin_out + IN_SIZE, false );
  }
}
*/

unsigned int round_robin_in[IN_SIZE*2 + 127];
unsigned int round_robin_out[IN_SIZE*2 + 127];

// iir filter
//===========================================================================================
void main( void )
{
  iir_filter( round_robin_in + 127, round_robin_out + 127, false );

  set_sync_level();
  while( 1 )
  {
    iir_filter( round_robin_in + 127, round_robin_out + 127, true );
    iir_filter( round_robin_in + 127 + IN_SIZE, round_robin_out + 127 + IN_SIZE, false );
  }
}
