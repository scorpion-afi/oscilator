// OscTask 

#define OscTask
#include "Osc_Task.h"

// внутри подключаетс€ InterDefines.h дл€ определение sOscParam
#include "Osc_Drv.h"         //дл€ вызова функций драйвера генерации

#include "CommonDefines.h"  //дл€ взаимодействи€ с FreeRTOS
  
// функци€, релизующ€€ поток OscTask
//==============================================================================
void vOscTask(void *pvParameters)
{ 
  sOscParam OscParam; 
  
  InitDAC_TIM_DMA();
  
  while(1)
  { 
    //ждем прихода сообщени€ от PBTask или DMA ISR
    xQueueReceive(qTo_Osc, (void *)&OscParam, portMAX_DELAY);
    
    // сообщение, из обработчика прерывани€ DMA, сигнализирующее о необходимости
    // смены текущей заполн€емой области циклического буфера DAC_Buff, изменении указател€
    // pDAC_Buff и заполнение текущей оласти циклического буфера DAC_Buff отсчетами текущих
    // сигналов обоих каналов
    if(OscParam.Sig_Type == 7)
      ReFill();

    // сообщение, от потока M_Thread, сигнализирующее о необходимости смене типа сигнала
    // или/и его параметров
    // и заполнении текущей области циклического буфера DAC_Buff отсчетами текущих
    // сигналов обоих каналов
    else
      ReCalc(&OscParam);        
  } 
  
  // ”ничтожить задачу, если произошел выход из бесконечного цикла  
  vTaskDelete(NULL);
}


