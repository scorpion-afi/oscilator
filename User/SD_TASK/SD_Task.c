
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
  int state = 0; // sd card is off
   
  while( 1 )
  {
    // waiting for messages from ISR or another sources
    xQueueReceive( queu_to_sd, ( void* )&message, portMAX_DELAY );
    
    switch( message.type ) 
    {
      case SD_EVENT:
      {
        if( state )  // if sd card is on
        {
          state = 0;
          
          close_file();     
          de_init_TIM5();    
          //write_file();  //write close time
        
          lock_send_message_to_sd_thread = 1; // disallow to send message from DMA 1 ch1 ISR to sd_thread         
        }
        else         // if sd card is off
        {                
          if( init_sd() ) // if init of sd card is failed
          {
            de_init_TIM5();
            break;
          }
          
          state = 1;
          
          open_file();
          //write_file();  //write open time
          
          lock_send_message_to_sd_thread = 0; // allow to send message from DMA 1 ch1 ISR to sd_thread 
        }
      }
      break;      
      
      case SD_WRITE:
      {
        write_file( message.data, message.num );
      }
      break;
      
      default: break;
   }
  } // while( 1 )

  // destroy task, if exit from while was occured
  vTaskDelete( NULL );
 }
