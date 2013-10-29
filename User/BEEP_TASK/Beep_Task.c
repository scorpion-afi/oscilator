
// BeepTask 

#define BeepTask
#include "Beep_Task.h"

//внутри подключаетс€ "InterDefines.h"
#include "Beep_Drv.h"         //дл€ вызова функций "драйвера" меню

#include "CommonDefines.h"  //дл€ взаимодействи€ с FreeRTOS

// функци€, релизующ€€ поток BeepTask
//==============================================================================
void vBeepTask(void *pvParameters)
{
  /*
  //сообщение к BeepTask
  sBeepParam BeepParam;
  
  InitBeep();
  
  while(1)
  { 
    //ждем прихода сообщени€ к BeepTask
    //сообщение Ќ≈ извлекаем из очереди
    xQueuePeek(qTo_Beep, (void *)&BeepParam, portMAX_DELAY);
    
    //захватываем семофор, если возможно или "зависаем" на ожидание
    xSemaphoreTake(semBeepTask, portMAX_DELAY);
      
    //извлекаем сообщение к BeepTask
    xQueueReceive(qTo_Beep, (void *)&BeepParam, portMAX_DELAY);
    
    //ќбрабатываем сообщение - запускаем аппаратный таймер
    //в ISR этого таймера освобождаем семафор
    Beep(BeepParam);       
  } 
  
  // ”ничтожить задачу, если произошел выход из бесконечного цикла  
  vTaskDelete(NULL);
*/  
}
