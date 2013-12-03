
// SYSCLK = HCLK =  72 ���.
// PCLK1  =         36 ���.
// PCLK2  =         72 ���.

#define MAIN
#include "main.h"

#include "SD_Drv.h"

#include "stm32f10x.h"

char adc_array[4096];
int write_fail;

//����� �����
//=======================================================================================
int main()
{  
  init_TIM5();
  init_sd();
  
  for( int i = 0; i < sizeof( adc_array ); i++ ) 
    adc_array[i] = i%256;
      
  for( int j = 0; j < 1000; j++ )               // 4Mb
  {
    if( write( adc_array, sizeof( adc_array ) ) )
      write_fail++;
  }
  
  
  while( 1 )
  {
    ;
  }
}