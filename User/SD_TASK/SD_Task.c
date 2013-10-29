
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
  
  if( init_sd() )
  {
     vTaskDelete( NULL );
     return;  // ?
  }
  
  while( 1 )
  {
    // waiting for messages from ISR or another sources
    xQueueReceive( queu_to_sd, ( void* )&message, portMAX_DELAY ); 
    
    // semaphore take
    write( message.data, message.num );  
   
    // DMA ISR must give semaphore
    
    // semaphore take
    //write( (char*)message.time, sizeof( message.time ) ); 
    
    // DMA ISR must give semaphore
  } 

  // destroy task, if exit from while was occured
  vTaskDelete( NULL );
 }
