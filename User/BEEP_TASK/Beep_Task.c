
// BeepTask 

#define BeepTask
#include "Beep_Task.h"

//������ ������������ "InterDefines.h"
#include "Beep_Drv.h"         //��� ������ ������� "��������" ����

#include "CommonDefines.h"  //��� �������������� � FreeRTOS

// �������, ���������� ����� BeepTask
//==============================================================================
void vBeepTask(void *pvParameters)
{
  /*
  //��������� � BeepTask
  sBeepParam BeepParam;
  
  InitBeep();
  
  while(1)
  { 
    //���� ������� ��������� � BeepTask
    //��������� �� ��������� �� �������
    xQueuePeek(qTo_Beep, (void *)&BeepParam, portMAX_DELAY);
    
    //����������� �������, ���� �������� ��� "��������" �� ��������
    xSemaphoreTake(semBeepTask, portMAX_DELAY);
      
    //��������� ��������� � BeepTask
    xQueueReceive(qTo_Beep, (void *)&BeepParam, portMAX_DELAY);
    
    //������������ ��������� - ��������� ���������� ������
    //� ISR ����� ������� ����������� �������
    Beep(BeepParam);       
  } 
  
  // ���������� ������, ���� ��������� ����� �� ������������ �����  
  vTaskDelete(NULL);
*/  
}
