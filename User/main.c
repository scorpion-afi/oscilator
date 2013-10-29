
// SYSCLK = HCLK =  72 ћ√ц.
// PCLK1  =         36 ћ√ц.
// PCLK2  =         72 ћ√ц.

#define MAIN
#include "main.h"

#include "PolBut_Task.h"    //определение функции-потока PBTask
#include "Menu_Task.h"      //определение функции-потока MTask
#include "LCD_Task.h"       //определение функции-потока LCDTask
#include "Osc_Task.h"       //определение функции-потока BeepTask
#include "Meas_Task.h"
#include "Calc_Task.h"
#include "SD_Task.h"

#include "InterDefines.h"   //определение sLCDParam и sOscParam

#include "stm32f10x.h"            
//extern uint32_t SystemCoreClock;  //SystemCoreClock - текущ€€ SYSCLK

// only for printing Loading...
#include "LCD_Drv.h"

/*
#include "stm32f10x_rcc.h"  
RCC_ClocksTypeDef kyky;
RCC_GetClocksFreq( &kyky );
*/

//точка входа
//=======================================================================================
int main()
{  
  InitLCD();
  SendString( "Loading...", 5, 1 );
  for( int i = 0; i < 3600000; i++ );
  
  //по требовани€м FreeRTOS
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  //создание задач, очередей и других обьектов ќ—
  if( Create_OS_Objects() != 0 )
  {
    // запускаем планировщик
    vTaskStartScheduler();
  }  
  
  return 0;
}

// —оздаем обьекты ќ—
//возвращ€емый результат: 1 - все обекты создать удалось
//                        0 - не удалось
//===================================================================================
int Create_OS_Objects(void)
{ 
  portBASE_TYPE rez;
 
  // queues creation
  
  //создаем очередь дл€ взаимодействи€ между PBTask и MTask
  qPB_to_M = xQueueCreate(5, sizeof(char));
  if(qPB_to_M == NULL) return 0;
  
  //создаем очередь дл€ взаимодействи€ между MTask и LCDTask
  qM_to_LCD = xQueueCreate(5, sizeof(sLCDParam));
  if(qM_to_LCD == NULL) return 0;
  
  //создаем очередь дл€ взаимодействи€ с OscTask
  qTo_Osc = xQueueCreate(5, sizeof(sOscParam));
  if(qTo_Osc == NULL) return 0;  
  
  // creating queue for Meas_Task
  queu_to_meas = xQueueCreate( 5, sizeof( char ) );
  if( queu_to_meas == NULL )
  {
    return 0;
  }
  
  // creating queue for Calc_Task
  queu_to_calc = xQueueCreate( 5, sizeof( unsigned short* ) );
  if( queu_to_calc == NULL )
  {
    return 0;
  }
  
  // creating queue for SD_Task
  queu_to_sd = xQueueCreate( 5, sizeof( S_Sd_Param_t ) );
  if( queu_to_sd == NULL )
  {
    return 0;
  }
  
  // threads creation
 
  //  Idle Task - priority 0
  
  //создаем задачу(поток) PBTask с приоритетом, равным 2
  rez = xTaskCreate(vPBTask, "PBTask", 256, NULL, 1, NULL);
  if(rez != pdPASS) return 0;
  
  //создаем задачу(поток) MTask с приоритетом, равным 3
  rez = xTaskCreate(vMTask, "MTask", 800, NULL, 2, NULL);
  if(rez != pdPASS) return 0;
   
  //создаем задачу(поток) LCDTask с приоритетом, равным 4
  rez = xTaskCreate(vLCDTask, "LCDTask", 256, NULL, 4, NULL);
  if(rez != pdPASS) return 0;
  
  //создаем задачу(поток) OscTask с приоритетом, равным 5
  rez = xTaskCreate(vOscTask, "OscTask", 512, NULL, 5, NULL);
  if( rez != pdPASS ) return 0;
  
  //создаем задачу (поток) CalcTask с приоритетом, равным 4
  rez = xTaskCreate( vCalcTask, "CalcTask", 256, NULL, 3, NULL );
  if( rez != pdPASS )
  {
    return 0;
  } 
  
  //создаем задачу (поток) SDTask с приоритетом, равным 4
  rez = xTaskCreate( vSDTask, "SDTask", 256, NULL, 4, NULL );
  if( rez != pdPASS )
  {
    return 0;
  }
  
  //создаем задачу (поток) MeasTask с приоритетом, равным 4
  rez = xTaskCreate( vMeasTask, "MeasTask", 256, NULL, 4, NULL );
  if( rez != pdPASS )
  {
    return 0;
  }

  return 1; 
}

//configCHECK_FOR_STACK_OVERFLOW = 2
/*void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed portCHAR *pcTaskName )
{
  int c;
  
  c = 0;
  c++;
}
*/