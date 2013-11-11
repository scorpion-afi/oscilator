
// SYSCLK = HCLK =  72 ���.
// PCLK1  =         36 ���.
// PCLK2  =         72 ���.

#define MAIN
#include "main.h"

#include "stm32f10x_rcc.h"  

//#include "SD_Drv.h"

#include "diskio.h"
#include "fattime.h"
#include "ff.h"

void init_TIM5(void);
//FRESULT create_file(const char *FileName);

//����� �����
//=======================================================================================
int main()
{ 
  RCC_ClocksTypeDef kyky;
  RCC_GetClocksFreq( &kyky );
 
  //init_sd();
 
  init_TIM5();
  
  for( int i = 0; i < 10000; i++);
  
  DSTATUS card_status = disk_initialize(0);
  
  FATFS   fs;                       //���������, � ������� ����� ��������� ���� 
 
  FRESULT res = f_mount(0, &fs);    //��������� ���������� ����� �� ���������� fs
  //create_file ("0:afi.txt");        //������� ������ ����
  //f_mount(0, NULL);                 //�������� ���������� ����� �� ���������� fs

  
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
/*
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
*/