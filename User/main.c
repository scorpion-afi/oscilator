
// SYSCLK = HCLK =  72 ���.
// PCLK1  =         36 ���.
// PCLK2  =         72 ���.

#define MAIN
#include "main.h"

#include "SD_Drv.h"
#include "Meas_Drv.h"         // for using measuring driver functions

unsigned int must_write;
unsigned int res;
int first_half;  
 
//����� �����
//=======================================================================================
int main( void )
{  
  first_half = 1;
  
  init_TIM5();
  res = init_sd();
  
  if( !res )
  { 
    start();
  }
  
  while( 1 )
  {
    if( must_write )
    {    
      if( first_half )
      {
        first_half = 0;
        write( p_beg_adc_buff, 4096 );
      }
      else
      {
        first_half = 1;
        write( p_beg_adc_buff + ADC_NUM_DIV_2, 4096 );
      }          
      
      must_write = 0;
    }  
  }
}
