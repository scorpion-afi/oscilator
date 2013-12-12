// MeasTask 

#define MeasTask
#include "Meas_Task.h"

#include "Meas_Drv.h"         // for using measuring driver functions

#include "CommonDefines.h"    //для взаимодействия с FreeRTOS
 
// function-thread (task), that serves measuring of channel parameters requests
//==============================================================================
void vMeasTask( void* pvParameters )
{ 
  // variable for messages from other threads
  char message = 0; // 0 - off, 1 - on
  
  // current state of measuring chain (TIM-ADC-DMA)
  char cur_state = 0; // 0 - off, 1 - on
  
  while( 1 )
  {
    // waiting for message: on/off measuring 
    xQueueReceive( queu_to_meas, ( void* )&message, portMAX_DELAY );
    
    if( message )       // if measuring must be switch 'on'
    {
      if( !cur_state )  // if measuring is not 'on'
      {
        cur_state = 1;
        start_meas();  // start measuring
      }    
    }
    else                // if meauing must be switch 'off'
    {
      if( cur_state )   // if measuring is not 'off'
      {
        cur_state = 0;
        stop_meas();   // stop measuring
      }    
    }    
  } // while( 1 )
  
  // Уничтожить задачу, если произошел выход из бесконечного цикла  
  vTaskDelete( NULL );
 }


