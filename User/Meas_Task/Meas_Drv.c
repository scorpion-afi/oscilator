// signal measuring driver

#define MeasDrv	
#include "Meas_Drv.h"

#include "stm32f10x.h"

#define ADC1_DR_Address ( (uint32_t) 0x4001244C )

// cycle-buffer  -- 8kB
unsigned short gl_adc_buff[ADC_NUM];

// 
//==============================================================================
void start( void )
{
  init();
}

// 
//==============================================================================
void stop( void )
{
  // TIM3 disable counter
  TIM_Cmd( TIM3, DISABLE );
  
  // TIM3 reset
  RCC->APB1RSTR |= 0x02;
    
  // disable TIM3 clocks
  RCC->APB1ENR &= ~0x02;
  
  
  // disable ADC1 
  ADC_Cmd( ADC1, DISABLE );  
  
  // ADC1 reset
  RCC->APB2RSTR |= 1 << 9;
  
  // disable ADC1 clocks
  RCC->APB2ENR &= ~(1 << 9);
  
  
  // disable DMA1 channel 1 
  DMA_Cmd( DMA1_Channel1, DISABLE );
}

// initialization of TIM3 and ADC1 for measuring 
//==============================================================================
void init( void )
{
  // takes pointer to global adc buffer
  p_beg_adc_buff = gl_adc_buff;
  
  TIM_TimeBaseInitTypeDef   TIM_TimeBaseInitStruct;
  ADC_InitTypeDef           ADC_InitStructure;
  DMA_InitTypeDef           DMA_InitStructure;
  GPIO_InitTypeDef          GPIO_InitStructure;
  NVIC_InitTypeDef          NVIC_InitStruct;
  
  // common settings ------------------------------------
  
  // setting ADCCLK to 12MHz
  RCC_ADCCLKConfig( RCC_PCLK2_Div6 ); 
  
  // Enable DMA1 clock
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_DMA1, ENABLE );

  // Enable GPIOC and ADC1 clock 
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC1, ENABLE );
  
  // Configure PC.00, PC.01, PC.02 and PC.03 (ADC Channel 10, 11, 12 and 13)
  // as analog inputs 
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init( GPIOC, &GPIO_InitStructure );
                          
  // ADC 1 init------------------------------
  
  // ADC_Channel_10 - IoutCh1 (first in regular group)
  // ADC_Channel_11 - VoutCh1
  // ADC_Channel_12 - IoutCh2 
  // ADC_Channel_13 - VoutCh2
  
  // ADCCLK = 12MHz
  // amount of convertion cycles
  // (for four channels, with sample time = 13.5 cycles) equal 
  // (13.5 + 12.5)*4 = 104 cycles
  // it takes 104 * (1/12)*10^-6 (s) ~ 8.6 mks
  // i.e ADC have ~1.4 mks rest (if TIM3 generates TRGO with 100kHz frequency)
  
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 4;
  ADC_Init( ADC1, &ADC_InitStructure );

  // ADC1 regular channels configuration  
  ADC_RegularChannelConfig( ADC1, ADC_Channel_10, 1, ADC_SampleTime_13Cycles5 );
  ADC_RegularChannelConfig( ADC1, ADC_Channel_11, 2, ADC_SampleTime_13Cycles5 );
  ADC_RegularChannelConfig( ADC1, ADC_Channel_12, 3, ADC_SampleTime_13Cycles5 );
  ADC_RegularChannelConfig( ADC1, ADC_Channel_13, 4, ADC_SampleTime_13Cycles5 );

  // Enable ADC1 external trigger conversion 
  ADC_ExternalTrigConvCmd( ADC1, ENABLE );

  // Enable ADC1 DMA 
  ADC_DMACmd( ADC1, ENABLE );
  
  // Enable ADC1 
  ADC_Cmd( ADC1, ENABLE );  

  // Enable ADC1 reset calibration register   
  ADC_ResetCalibration( ADC1 );
  // Check the end of ADC1 reset calibration register 
  while( ADC_GetResetCalibrationStatus(ADC1) );

  // Start ADC1 calibration 
  ADC_StartCalibration( ADC1 );
  // Check the end of ADC1 calibration 
  while( ADC_GetCalibrationStatus( ADC1 ) );  
  
  //DMA1 init -------------------------------------------------
  
  //сбрасываем флаг прерывания global interrupt 1 канала DMA 1
  DMA_ClearITPendingBit( DMA1_IT_GL1 );
    
  DMA_DeInit( DMA1_Channel1 );
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&gl_adc_buff;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = ADC_NUM;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init( DMA1_Channel1, &DMA_InitStructure );
  
  //Настройка NVIC для DMA IRQ 
  NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel1_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 12;  // 12 > configMAX_SYSCALL_INTERRUPT_PRIORITY
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init( &NVIC_InitStruct ); 
  
  DMA_ITConfig( DMA1_Channel1, DMA_IT_TC | DMA_IT_HT, ENABLE );
  DMA_ITConfig( DMA1_Channel1, DMA_IT_TE, DISABLE );
  
  // Enable DMA1 channel1 
  DMA_Cmd( DMA1_Channel1, ENABLE );
  
  // TIM 3 init------------------------------
  
  // PCLK1 = 36 МГц
  // F_interrupt = PCLK1*2 / (TIM_Prescaler + 1) / TIM_Period;

  // Enable TIM3 clocks
  RCC->APB1ENR |=  0x02;
  
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;            
  TIM_TimeBaseInitStruct.TIM_Period = 720-1;   // прерывания 10000 раз в секунду(720)
  TIM_TimeBaseInitStruct.TIM_Prescaler = 9; // 100 - 1
  TIM_TimeBaseInit( TIM3, &TIM_TimeBaseInitStruct );
  
  //выбираем в качестве источника внешнего тригера(TRGO) update event
  TIM_SelectOutputTrigger( TIM3, TIM_TRGOSource_Update ); 
  
  // TIM3 enable counter
  TIM_Cmd( TIM3, ENABLE );
  
  // Disable TIM3 Update interrupt
  TIM_ITConfig( TIM3, TIM_IT_Update, DISABLE );
  
  // after this line, TIM3-ADC1-DMA1 band will start work
}