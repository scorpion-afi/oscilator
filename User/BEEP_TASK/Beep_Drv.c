
// "драйвер" Beep 

#define BeepDrv	
#include "Beep_Drv.h"

void InitBeep(void)
{
  // Enable TIM2 clocks
  RCC->APB1ENR |= 1;
  //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
  
  // Enable the TIM2 Interrupt
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 13; // 13 or 1101 1111 > configMAX_SYSCALL_INTERRUPT_PRIORITY
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
  
  //настройка ножки, на которой сидит бипер
  GPIO_StructInit(&GPIO_InitStruc_Beep);
  
  //IO port A clock enabled 
  RCC->APB2ENR |= 4;
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
  GPIO_InitStruc_Beep.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStruc_Beep.GPIO_Mode = GPIO_Mode_Out_PP; // output push-pull

  GPIO_Init(GPIOA, &GPIO_InitStruc_Beep); 
}

//T - период пачки пр€моугольных импульсов (ms)
//num - кол-во периодов
void Beep(sBeepParam BeepParam)
{   
  // PCLK1 = 36 ћ√ц
  
  //T_interrupt = T/2
  //F_interrupt = (2/T)*1000, где T в мс
  //F_interrupt = PCLK1*2 / (TIM_Prescaler + 1) / TIM_Period;
  
  // (2*1000)/T = ( (PCLK1*2) / (TIM_Prescaler + 1) ) / TIM_Period
  // TIM_Period = ( T*PCLK1 ) / ( 1000*(TIM_Prescaler + 1) )
  
  //TIM_Period = (T*36*10^6) / (1000 * (TIM_Prescaler + 1))
     
  TIM_TimeBaseInitStructure.TIM_Period = BeepParam.T*36;     // прерывани€ T*2*1000 раз в секунду
  TIM_TimeBaseInitStructure.TIM_Prescaler = 999;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
   
  Num_T = BeepParam.num*2; // без +1
  
  // TIM2 enable counter
  TIM_Cmd(TIM2, ENABLE);
  
  // Enable TIM2 Update interrupt
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}
