
// LSDTask 

#define LSDTask	
#include "LCD_Task.h"

#include "LCD_Drv.h"         //��� ������ ������� "��������" LCD

#include "CommonDefines.h"  //��� �������������� � FreeRTOS 

#include "InterDefines.h"   //�����������  sLCDParam
  
// �������, ���������� ����� LCDTask
//==============================================================================
void vLCDTask(void *pvParameters)
{  
  sLCDParam LCDParam;
  
  InitLCD();
  
  while(1)
  { 
    //���� ������� ��������� �� MTask
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
  
  // ���������� ������, ���� ��������� ����� �� ������������ �����  
  vTaskDelete(NULL);
}
