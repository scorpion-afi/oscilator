
// PBTask 

#define PBTask
#include "PolBut_Task.h"

#include "PolBut_Drv.h"      //��� ������ ������� �������� ���������

#include "CommonDefines.h"  //��� �������������� � FreeRTOS

//sKeyMesg KPadGL;
// �������, ���������� ����� PBTask
//==============================================================================
void vPBTask(void *pvParameters)
{ 
  sKeyMesg KPad;
  portTickType xLastWakeTime;
  
  //������������� ����������, ��������� � ������� ���������
  InitPad_Task();
  
  //���������� ������ ���� ���
  xLastWakeTime = xTaskGetTickCount();
  
  while(1)
  { 
    // �������� �� 10 �����������
    vTaskDelayUntil( &xLastWakeTime, ( 10 / portTICK_RATE_MS ) );
        
    //����� ��������� ������ KeyPad ���������� 
    //KPad.num: 0 - 7
    KPad = GetKeyPadState();
    
    //KPadGL = KPad;
    if(KPad.typeEvent != -1)
      Send_PolBut_Message(KPad.typeEvent, 1, KPad.num);  
   } 

  // ���������� ������, ���� ��������� ����� �� ������������ �����  
  vTaskDelete(NULL);
}

// ������� ���������  � ������� ������ MTask
//typeEvent - ��� �������
//typePad - ��� ���������� (1 - KeyPad, 0 - MatrixPad)
//num - ����� ������(0-7 ��� 0-11)

// ������� � ������ Main_Task ������� �� 5 ���������
// ������ �������� - 1 ����
// 7:5 ����: ��� �������(1 - ButtonOn, 2 - ButtonPress)
// 4:1 ����: ����� ������ (0-7 ��� 0-11)
// 0 ���: ��� ���������� (1 - KeyPad, 0 - MatrixPad)
//===================================================================================
void Send_PolBut_Message(int typeEvent, int typePad, int num)
{
  char value;
    
  value =  (char)(num<<1);
  value |= (char)typePad;
  value |= (char)(typeEvent<<5);
  
  //"��������" ���� � ������� PB_to_M �� �������� ��������� �����
  xQueueSend(qPB_to_M, (void *)&value, portMAX_DELAY);
}
