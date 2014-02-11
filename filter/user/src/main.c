
/*
// for checking frequencies set
#include "stm32f10x_rcc.h"  
RCC_ClocksTypeDef kyky;
RCC_GetClocksFreq( &kyky );
*/

#include "stm32f10x.h"

//#include "fir.h"
#include "iir.h"

#define slice (1.0f/72000000.0f)

int system_interrupt_cnt;
void float_plus_float( void );

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
  uint32_t tic = 0;
  uint32_t toc = 0;
  float calc_time = 0;
  
  SysTick_Config( SysTick_LOAD_RELOAD_Msk );
  
  tic = SysTick->VAL;
  float_plus_float();
  toc = tic - SysTick->VAL;
  
  calc_time = (float)( toc + system_interrupt_cnt*16777216 ) * slice;
  
  iir_filter( round_robin_in + 127, round_robin_out + 127, false );

  //set_sync_level();
  while( 1 )
  {
    iir_filter( round_robin_in + 127, round_robin_out + 127, true );
    iir_filter( round_robin_in + 127 + IN_SIZE, round_robin_out + 127 + IN_SIZE, false );
  }
}

void float_plus_float( void )
{
  float a = 1.1f;
  float b = 2.5f;
 
  for( int i = 0; i < 72000000; i++ )
    a += b;
}
