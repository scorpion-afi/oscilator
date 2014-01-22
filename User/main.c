
// SYSCLK = HCLK =  72 ���.
// PCLK1  =         36 ���.
// PCLK2  =         72 ���.

#define MAIN
#include "main.h"

#include "PolBut_Task.h"    //����������� �������-������ PBTask
#include "Menu_Task.h"      //����������� �������-������ MTask
#include "LCD_Task.h"       //����������� �������-������ LCDTask
#include "Osc_Task.h"       //����������� �������-������ BeepTask
#include "Meas_Task.h"      // vMeasTask function declaration
#include "Calc_Task.h"      // vCalcTask function declaration
#include "SD_Task.h"        // vSDTask function declaration

#include "InterDefines.h"   //����������� sLCDParam � sOscParam

#include "stm32f10x.h"            
//extern uint32_t SystemCoreClock;  //SystemCoreClock - ������� SYSCLK

// only for printing Loading...
#include "LCD_Drv.h"

/*
// for checking frequencies set
#include "stm32f10x_rcc.h"  
RCC_ClocksTypeDef kyky;
RCC_GetClocksFreq( &kyky );
*/

//����� �����
//=======================================================================================
int main( void )
{  
  InitLCD();
  SendString( "Loading...", 5, 1 );
  for( int i = 0; i < 3600000; i++ );
  
  // some initialization, which must be applied before FreeRTOS will be started
  init_common();
  
  // initialization of external interrupt lines
  init_exti();
  
  //EXTI_GenerateSWInterrupt( EXTI_Line4 ); 
  //while( 1 );
  
  //�� ����������� FreeRTOS
  NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

  //�������� �����, �������� � ������ �������� ��
  if( Create_OS_Objects() != 0 )
  {
    // ��������� �����������
    vTaskStartScheduler();
  }  
  
  return 0;
}

// some initialization, which must be applied before FreeRTOS will be started                      
//===================================================================================
void init_common( void )
{
  lock_send_message_to_calc_thread = 0;
  lock_send_message_to_sd_thread = 1;
}

// initialization of external interrupt lines
// SyncInCh1 - PC4 
// SyncInCh2 - PC5 
//===================================================================================
void init_exti( void )
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  // Deinitializes the EXTI peripheral registers to their default reset values 
  EXTI_DeInit();
  
  // Fills each GPIO_InitStructure member with its default value
  GPIO_StructInit( &GPIO_InitStructure );
  
  // Fills each EXTI_InitStruct member with its reset value
  EXTI_StructInit( &EXTI_InitStructure );
    
  // Enable GPIOC and AFIO clock 
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE );
  
  // Configure PC4 and PC5 as input floating 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init( GPIOC, &GPIO_InitStructure );
  
  // Connect EXTI line 4 to PC4 
  GPIO_EXTILineConfig( GPIO_PortSourceGPIOC, GPIO_PinSource4 );  
  
  // Configure EXTI line 4 
  EXTI_InitStructure.EXTI_Line = EXTI_Line4;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init( &EXTI_InitStructure ); 
  
  // Connect EXTI line 5 to PC5 
  GPIO_EXTILineConfig( GPIO_PortSourceGPIOC, GPIO_PinSource5 );  
  
  // Configure EXTI line 5 
  EXTI_InitStructure.EXTI_Line = EXTI_Line5;
  EXTI_Init( &EXTI_InitStructure ); 
  
  // Enable EXTI line 4 Interrupt
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 12;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init( &NVIC_InitStructure ); 
  
  // Enable EXTI line 5 Interrupt
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_Init( &NVIC_InitStructure ); 
}

// ������� ������� ��
//������������ ���������: 1 - ��� ������ ������� �������
//                        0 - �� �������
//===================================================================================
int Create_OS_Objects( void )
{ 
  portBASE_TYPE rez;
 
  // queues creation
  
  //������� ������� ��� �������������� ����� PBTask � MTask
  qPB_to_M = xQueueCreate( 5, sizeof( s_pol_button ) );
  if( qPB_to_M == NULL ) return 0;
  
  //������� ������� ��� �������������� ����� MTask � LCDTask
  qM_to_LCD = xQueueCreate( 5, sizeof( sLCDParam ) );
  if( qM_to_LCD == NULL ) return 0;
  
  //������� ������� ��� �������������� � OscTask
  qTo_Osc = xQueueCreate(5 , sizeof( sOscParam ) );
  if( qTo_Osc == NULL ) return 0;  
  
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
  
  //������� ������(�����) PBTask � �����������, ������ 2
  rez = xTaskCreate( vPBTask, "PBTask", 256, NULL, 1, NULL );
  if( rez != pdPASS ) return 0;
  
  //������� ������(�����) MTask � �����������, ������ 3
  rez = xTaskCreate( vMTask, "MTask", 800, NULL, 2, NULL );
  if( rez != pdPASS ) return 0;
   
  //������� ������(�����) LCDTask � �����������, ������ 4
  rez = xTaskCreate( vLCDTask, "LCDTask", 256, NULL, 4, NULL );
  if( rez != pdPASS ) return 0;
  
  //������� ������(�����) OscTask � �����������, ������ 5
  rez = xTaskCreate( vOscTask, "OscTask", 512, NULL, 5, NULL );
  if( rez != pdPASS ) return 0;
  
  //������� ������ (�����) CalcTask � �����������, ������ 4
  rez = xTaskCreate( vCalcTask, "CalcTask", 256, NULL, 3, NULL );
  if( rez != pdPASS )
  {
    return 0;
  } 
  
  //������� ������ (�����) SDTask � �����������, ������ 4
  rez = xTaskCreate( vSDTask, "SDTask", 256, NULL, 4, NULL );
  if( rez != pdPASS )
  {
    return 0;
  }
  
  //������� ������ (�����) MeasTask � �����������, ������ 4
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
