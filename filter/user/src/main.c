
#include "stm32f10x.h"

#include "fir.h"

unsigned int round_robin_in[IN_SIZE*2 + 255];
unsigned int round_robin_out[IN_SIZE*2];

void main( void )
{ 
  fir_filter( round_robin_in + 255, round_robin_out, false );
  
  while( 1 )
  {  
    fir_filter( round_robin_in + 255, round_robin_out, true );
    fir_filter( round_robin_in + 255 + IN_SIZE, round_robin_out + IN_SIZE, false );
  }
}
