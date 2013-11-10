
// SYSCLK = HCLK =  72 ���.
// PCLK1  =         36 ���.
// PCLK2  =         72 ���.

#define MAIN
#include "main.h"
/*
#include "PolBut_Task.h"    //����������� �������-������ PBTask
#include "Menu_Task.h"      //����������� �������-������ MTask
#include "LCD_Task.h"       //����������� �������-������ LCDTask
#include "Osc_Task.h"       //����������� �������-������ BeepTask
#include "Meas_Task.h"
#include "Calc_Task.h"
#include "SD_Task.h"

#include "InterDefines.h"   //����������� sLCDParam � sOscParam

#include "stm32f10x.h"            
//extern uint32_t SystemCoreClock;  //SystemCoreClock - ������� SYSCLK

// only for printing Loading...
#include "LCD_Drv.h"
*/
/*
#include "stm32f10x_rcc.h"  
RCC_ClocksTypeDef kyky;
RCC_GetClocksFreq( &kyky );
*/

//#include "SD_Drv.h"


#include "diskio.h"
#include "fattime.h"
#include "ff.h"

void init_TIM5(void);
FRESULT create_file(const char *FileName);

//����� �����
//=======================================================================================
int main()
{  
  init_TIM5();
  
  for( int i = 0; i < 10000; i++);
  
  DSTATUS card_status = disk_initialize(0);
  
  FATFS   fs;                   //���������, � ������� ����� ��������� ���� 
 
  FRESULT res = f_mount(0, &fs);              //��������� ���������� ����� �� ���������� fs
  create_file ("0:afi.txt");  //������� ������ ����
  //create_file ("0:proba2.txt"); //������� ������ ����
  f_mount(0, NULL);             //�������� ���������� ����� �� ���������� fs
  
  while(1)
  {
    ;
  }
  //-+-+-+-+-+-+-+-+-+-+-+ 
}

//
void init_TIM5(void)
{
  NVIC_InitTypeDef          NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef   TIM_TimeBaseInitStruct;
  
  // Enable TIM5 clocks
  RCC->APB1ENR |=  0x08;
  
  // necessary time delay - 10ms
  // 10 ms <-> 100 ips (interrupts per second)
  // PCLK1 = 36 ���
  // CK_PSC = 2*PCLK1 = 72 MGz
  // number of ips = CK_PSC / (PSC*CNT)
  // ==>
  // 100 ips = 72 * 10^6 / (1000*720)
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;            
  TIM_TimeBaseInitStruct.TIM_Period = 720;   // ���������� 100 ��� � �������
  TIM_TimeBaseInitStruct.TIM_Prescaler = 999; // 1000 - 1
  TIM_TimeBaseInit( TIM5, &TIM_TimeBaseInitStruct );
  
  // necessary time delay - 1000ms
  // 1000 ms <-> 1 ips (interrupts per second)
  // PCLK1 = 36 ���
  // CK_PSC = 2*PCLK1 = 72 MGz
  // number of ips = CK_PSC / (PSC*CNT)
  // ==>
  // 1 ips = 72 * 10^6 / (1000*72000)
  //TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;            
  //TIM_TimeBaseInitStruct.TIM_Period = 36000;   // ���������� 2 ���� � �������
  //TIM_TimeBaseInitStruct.TIM_Prescaler = 1999; // 1000 - 1
  //TIM_TimeBaseInit( TIM5, &TIM_TimeBaseInitStruct );
  
  // Enable the TIM5 Interrupt
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 13; // 13 or 1101 1111 > configMAX_SYSCALL_INTERRUPT_PRIORITY
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 

  // TIM5 enable counter
  TIM_Cmd(TIM5, ENABLE);
  
  // Enable TIM5 Update interrupt
  TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
}

//*********************************************************************************************
//function ������� ��������� ���� � ���������� � ���� ������ "Hello, Word"                   //
//argument ��� �����                                                                         //
//*********************************************************************************************
FRESULT create_file(const char *FileName)
{
  FRESULT res;               //��� ������������� ��������� ����������
  FIL file;                  //�������� ������
  UINT len;                  //��� �������� ���������� ������� ���������� ����
  char str[] = "Hello, Word";  //������������ ������
 
  //������� ���� � ������ FileName � ��������� ��� ��� ��� ������
  res = f_open(&file, FileName, FA_WRITE | FA_CREATE_ALWAYS); 
  if(res)
  {
    return res;    //���� ��������� ������
  }
  
  //���������� ������ � ����
  res = f_write(&file, str, sizeof(str), &len);
  if(res)
  {
    f_close(&file);
    return res;    //���� ��������� ������
  }
  
  //��������� ����
  f_close(&file);
  
  return res;
}
/*
// ������� ������� ��
//������������ ���������: 1 - ��� ������ ������� �������
//                        0 - �� �������
//===================================================================================
int Create_OS_Objects(void)
{ 
  portBASE_TYPE rez;
 
  // queues creation
  
  //������� ������� ��� �������������� ����� PBTask � MTask
  qPB_to_M = xQueueCreate(5, sizeof(char));
  if(qPB_to_M == NULL) return 0;
  
  //������� ������� ��� �������������� ����� MTask � LCDTask
  qM_to_LCD = xQueueCreate(5, sizeof(sLCDParam));
  if(qM_to_LCD == NULL) return 0;
  
  //������� ������� ��� �������������� � OscTask
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
  
  //������� ������(�����) PBTask � �����������, ������ 2
  rez = xTaskCreate(vPBTask, "PBTask", 256, NULL, 1, NULL);
  if(rez != pdPASS) return 0;
  
  //������� ������(�����) MTask � �����������, ������ 3
  rez = xTaskCreate(vMTask, "MTask", 800, NULL, 2, NULL);
  if(rez != pdPASS) return 0;
   
  //������� ������(�����) LCDTask � �����������, ������ 4
  rez = xTaskCreate(vLCDTask, "LCDTask", 256, NULL, 4, NULL);
  if(rez != pdPASS) return 0;
  
  //������� ������(�����) OscTask � �����������, ������ 5
  rez = xTaskCreate(vOscTask, "OscTask", 512, NULL, 5, NULL);
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