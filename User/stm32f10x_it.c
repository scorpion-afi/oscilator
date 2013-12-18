/**
  ******************************************************************************
  * @file    ADC/3ADCs_DMA/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.1.0 
  * @date    06/19/2009
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

#include "InterDefines.h"   // определение sOscParam and S_Sd_Param_t
#include "CommonDefines.h"  // for using FreeRTOS functions,types and common (global) variables (flags)

// for use pointer p_beg_adc_buff
#include "Meas_Drv.h" 

// for disk_timerproc() calling
#include "diskio.h"

#include "stm32f10x.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup ADC_3ADCs_DMA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
    ;
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */


//ќбработчик прерываний от таймера TIM2. 
//==============================================================================
void TIM2_IRQHandler(void)
{ 
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);	//__Clear TIM2 update interrupt__
}

//√енераци€ сигналов (Osc_xxx.c)

//ќбработчик прерываний от DMA2_Channel3 
// will be called with x Hz frequence
//==============================================================================
void DMA2_Channel3_IRQHandler(void)     //DAC_Channel1
{  
  sOscParam temp;
  portBASE_TYPE rez;
         
  //if(DMA_GetITStatus(DMA2_IT_HT3) || DMA_GetITStatus(DMA2_IT_TC3)) 
  if( ( DMA2->ISR & 0x400 ) || // ( 1 << 10 )
      ( DMA2->ISR & 0x200 )    // ( 1 << 9 )
    )
  {       
    //информируем, что сообщение пришло от DMA ISR  
    temp.Sig_Type = 7; 
    
    //об€з€тельно !!!
    rez = pdFALSE;
    
    //посылаем в "голову" очереди
    xQueueSendToFrontFromISR( qTo_Osc, (void *)&temp, &rez );
  
    //если в результате посылки сообщени€ была разблокирована задача, более приоритетна€, чем та,
    //которую прервало данное прерывание, то переключаем контекст не дожида€сь окончани€ кванта времени 
    if( rez == pdTRUE )
    {
      portEND_SWITCHING_ISR( rez );
    }
  }
  
  //сбрасываем флаг прерывани€ global interrupt 3 канала DMA 2
  //DMA2->IFCR = (1<<8);//DMA2_IT_GL3; 
  DMA2->IFCR |= 0x100; //DMA2_IT_GL3;  may led to unnormaly work
}

// measuring

// this constant defines measuring info refresh rate
// now it is 3 Hz
#define FPS 3  // 3 = 10/3

//ќбработчик прерываний от DMA1_Channel1 
// will be called with  10 Hz frequence    195
//==============================================================================
void DMA1_Channel1_IRQHandler( void )     
{
  static unsigned int counter;
  S_Sd_Param_t sd_param;
  
  portBASE_TYPE rez;
  portBASE_TYPE rez_1;
     
  if( DMA1->ISR & 0x04 )    //( 1 << 2 )
  {
    sd_param.data = (void*)(p_beg_adc_buff);
  }
  else if( DMA1->ISR & 0x02)  //( 1 << 1 )
  {
    sd_param.data = (void*)(p_beg_adc_buff + ADC_NUM_DIV_2); 
  }
  
  // message to Calc Task will be sent three times per seconds
  if( ( ++counter > FPS ) && ( lock_send_message_to_calc_thread  == 0 ) )
  {    
    counter = 0; 
    
    rez = pdFALSE;  // об€з€тельно !!!
    
    // sends a message to display results of measuring
    xQueueSendFromISR( queu_to_calc, (void *)&(sd_param.data), &rez );
  }
  
  // message to SD thread, to write on sd card, will be send only when allowed
  if( !lock_send_message_to_sd_thread )
  {
    rez_1 = pdFALSE;  // об€з€тельно !!!
    
    sd_param.type = SD_WRITE;
    sd_param.num = ADC_NUM;   // size in bytes !!!
    sd_param.time = 0x1109;   // any, while   
    
    // sends a message to write data to sd card
    xQueueSendFromISR( queu_to_sd, (void *)&sd_param, &rez_1 );
   
    //если в результате посылки сообщени€ была разблокирована задача, более приоритетна€, чем та,
    //которую прервало данное прерывание, то переключаем контекст не дожида€сь окончани€ кванта времени 
    if( rez_1 == pdTRUE )
    {
      portEND_SWITCHING_ISR( rez_1 ); 
    }   
  }
  
  //сбрасываем флаг прерывани€ global interrupt 1 канала DMA 1
  DMA1->IFCR |= 1; //DMA1_IT_GL1;
}

//ќбработчик прерываний от таймера TIM5. (10 ms)     ___ for FatFS purpose ___
//==============================================================================
void TIM5_IRQHandler(void)
{ 
  disk_timerproc();
  
  TIM_ClearITPendingBit(TIM5, TIM_IT_Update);	//__Clear TIM5 update interrupt__
}

// extil line 4 isr (PC4)
//==============================================================================
void EXTI4_IRQHandler( void )
{ 
  S_Sd_Param_t sd_param;  
  portBASE_TYPE rez_1 = pdFALSE;  // об€з€тельно !!!
    
  sd_param.type = SD_EVENT;
  
  // sends a message SD_EVENT to write data to sd card
  xQueueSendFromISR( queu_to_sd, (void *)&sd_param, &rez_1 );
 
  //если в результате посылки сообщени€ была разблокирована задача, более приоритетна€, чем та,
  //которую прервало данное прерывание, то переключаем контекст не дожида€сь окончани€ кванта времени 
  if( rez_1 == pdTRUE )
  {
    portEND_SWITCHING_ISR( rez_1 ); 
  }   

  // Clear the EXTI line 4 pending bit 
  EXTI_ClearITPendingBit( EXTI_Line4 );
}

// extil line 5 isr (PC5)
// are some questions !!!
//==============================================================================
void EXTI9_5_IRQHandler( void )
{ 
  /*
  S_Sd_Param_t sd_param;  
  portBASE_TYPE rez_1 = pdFALSE;  // об€з€тельно !!!
    
  sd_param.type = SD_EVENT;
  
  // sends a message SD_EVENT to write data to sd card
  xQueueSendFromISR( queu_to_sd, (void *)&sd_param, &rez_1 );
 
  //если в результате посылки сообщени€ была разблокирована задача, более приоритетна€, чем та,
  //которую прервало данное прерывание, то переключаем контекст не дожида€сь окончани€ кванта времени 
  if( rez_1 == pdTRUE )
  {
    portEND_SWITCHING_ISR( rez_1 ); 
  }   
  */
  
  // Clear the EXTI line 5 pending bit 
  EXTI_ClearITPendingBit( EXTI_Line5 );
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
