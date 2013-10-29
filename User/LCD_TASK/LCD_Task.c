
// LSDTask 

#define LSDTask	
#include "LCD_Task.h"

#include "LCD_Drv.h"         //дл€ вызова функций "драйвера" LCD

#include "CommonDefines.h"  //дл€ взаимодействи€ с FreeRTOS 

#include "InterDefines.h"   //определение  sLCDParam
  
// функци€, релизующ€€ поток LCDTask
//==============================================================================
void vLCDTask(void *pvParameters)
{  
  sLCDParam LCDParam;
  
  InitLCD();
  
  while(1)
  { 
    //ждем прихода сообщени€ от MTask
    xQueueReceive(qM_to_LCD, (void *)&LCDParam, portMAX_DELAY);
        
    switch(LCDParam.ID_cmd)
    {
      case 0:
      {
        SendString(LCDParam.pData, LCDParam.param_1, LCDParam.param_2);
      }
      break;
      
      case 1:
      {
        ShiftBlink(LCDParam.param_1, LCDParam.param_2);
      }
      break;
      
      case 2:
      {
        ControlBlink(LCDParam.param_1, LCDParam.param_2, LCDParam.param_3);
      }
      break;
      
      default:
       break;     
    };
  } 
  
  // ”ничтожить задачу, если произошел выход из бесконечного цикла  
  vTaskDelete(NULL);
}
