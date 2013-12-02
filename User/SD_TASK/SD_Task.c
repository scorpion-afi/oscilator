
// SDTask 

#define SDTask
#include "SD_Task.h"

#include "SD_Drv.h"           // for using sd driver functions

#include "InterDefines.h"    // for S_Sd_Param_t
#include "CommonDefines.h"    //for interaction with FreeRTOS

// function-thread (task), that serves requests from DMA1 iterrupt
//==============================================================================
void vSDTask( void* pvParameters )
{ 
  S_Sd_Param_t message;
  
  init_TIM5();
  
  if( init_sd() ) // if init of sd card is failed
  {
     de_init_TIM5();
     vTaskDelete( NULL );
     return;  // ?
  }
  
  while( 1 )
  {
    // waiting for messages from ISR or another sources
    xQueueReceive( queu_to_sd, ( void* )&message, portMAX_DELAY ); 
    
    if( write( message.data, message.num ) )
      break;
  } 

  // destroy task, if exit from while was occured
  vTaskDelete( NULL );
 }
