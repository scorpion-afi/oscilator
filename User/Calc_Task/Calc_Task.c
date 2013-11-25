// CalcTask 

#define CalcTask
#include "Calc_Task.h"

#include "Calc_Drv.h"         // for using measuring driver functions

#include "CommonDefines.h"    //для взаимодействия с FreeRTOS

// !!! size of cycle-buffer is defined in Meas_Drv.h and equal 512 ushort !!!
// including of Meas_Drv.h is undesirable
  
// function-thread (task), that serves requests from DMA1 iterrupt
//==============================================================================
void vCalcTask( void* pvParameters )
{ 
  float i_ch1 = 0.0f;
  float u_ch1 = 0.0f;
  float i_ch2 = 0.0f;
  float u_ch2 = 0.0f;
  
  unsigned short* adc_ptr = 0;
  
  while( 1 )
  {
    // waiting for messages from ISR
    // pointer to sub-buffer of circular buffer, that can be procesed
    xQueueReceive( queu_to_calc, ( void* )&adc_ptr, portMAX_DELAY );
    
    if( !adc_ptr )
    {
      continue;
    }
    
    // ADC_Channel_10 - IoutCh1 (first in regular group)
    // ADC_Channel_11 - VoutCh1
    // ADC_Channel_12 - VoutCh2 
    // ADC_Channel_13 - IoutCh2
    
    // Vref = 3.3 V
    // R7 =  2 kOhm
    // R36 = 2 kOhm
    // not 2000.0f, because of show_result() MUST receive currents in mA
    i_ch1 = 0.1943f * adc_ptr[0] - 0.3238f; 
    u_ch1 = 0.0011848f * adc_ptr[1] - 0.0119f;           //( 3.3 * 1.446 * adc_ptr[1]/4095.0f );
        
    u_ch2 = 0.0009407f * adc_ptr[2] - 0.005f;           //( 3.3 * 1.16 * adc_ptr[3]/4095.0f );
    i_ch2 = 0.1943f * adc_ptr[3] - 0.4829f;
    
    /*i_ch1 = ( ( (DAC_Buff[0]&0x0000FFFF)/4095.0f )* 3.3f )/2.0f; 
    u_ch1 = ( ( (DAC_Buff[1]&0x0000FFFF)/4095.0f )* 3.3f );
 
    i_ch2 = ( ( (DAC_Buff[2]&0x0000FFFF)/4095.0f )* 3.3f )/2.0f;
    u_ch2 = ( ( (DAC_Buff[3]&0x0000FFFF)/4095.0f )* 3.3f );*/
   
    // this function will have shown result of measuring:
    // power, current and voltage for two channels
    //  i_ch1, i_ch2 - value of currents in mA
    //  u_ch1, u_ch2 - value of voltages in V 
    //  powers will be printed in mW   (i*u)
    show_result( i_ch1, u_ch1, i_ch2, u_ch2 );  
  } 
  
  // Уничтожить задачу, если произошел выход из бесконечного цикла  
  vTaskDelete( NULL );
 }
