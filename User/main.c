
// SYSCLK = HCLK =  72 ћ√ц.
// PCLK1  =         36 ћ√ц.
// PCLK2  =         72 ћ√ц.

#define MAIN
#include "main.h"

#include "SD_Drv.h"
#include "Meas_Drv.h"         // for using measuring driver functions

char adc_array[4096];
char dac_array[4096];
int write_fail_cnt;

unsigned int res = 0;
  
//точка входа
//=======================================================================================
int main()
{  
  init_TIM5();
  for(int i = 0; i < 10000; i++ );
  res = init_sd();
  
  if( !res )
  { 
    //read( dac_array, sizeof( dac_array ) );
    
    for( int i = 0; i < sizeof( adc_array ); i++ )
      adc_array[i] = i%256;
        
    for( int j = 0; j < 1000; j++ )               // 4Mb
    {
      if( write( adc_array, sizeof( adc_array ) ) )
        write_fail_cnt++;
    }
  }
  
  while( 1 )
  {
    ;
  }
}