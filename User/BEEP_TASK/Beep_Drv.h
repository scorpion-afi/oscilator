// "�������" Beep 

#ifdef	BeepDrv
  #define GLB_BeepDrv 			
#else	
  #define GLB_BeepDrv extern
#endif

#include "InterDefines.h"   //����������� sBeepParam

GLB_BeepDrv void InitBeep(void);
GLB_BeepDrv void Beep(sBeepParam BeepParam);

//��� TIM2 ISR
GLB_BeepDrv int Num_T;

#ifdef	BeepDrv

  #include "stm32f10x.h"

  NVIC_InitTypeDef          NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef   TIM_TimeBaseInitStructure;
  GPIO_InitTypeDef          GPIO_InitStruc_Beep;

#endif