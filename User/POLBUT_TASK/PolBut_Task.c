
// PBTask 

#define PBTask
#include "PolBut_Task.h"

#include "PolBut_Drv.h"      //��� ������ ������� �������� ���������

#include "CommonDefines.h"  // for communication with FreeRTOS

#include "InterDefines.h"   // declaration of s_pol_button

void send_polbut_message( const sKeyMesg* temp );

//sKeyMesg KPadGL;

// �������, ���������� ����� PBTask
//==============================================================================
void vPBTask( void* pvParameters )
{ 
  sKeyMesg KPad;
  portTickType xLastWakeTime;
  
  //������������� ����������, ��������� � ������� ���������
  InitPad_Task();
  
  //���������� ������ ���� ���
  xLastWakeTime = xTaskGetTickCount();
  
  while( 1 )
  { 
    // �������� �� 10 �����������
    vTaskDelayUntil( &xLastWakeTime, ( 10 / portTICK_RATE_MS ) );
        
    //����� ��������� ������ KeyPad ���������� 
    //KPad.num: 0 - 7

    // if we have some event on keyboard
    if( get_key_pad_state( &KPad ) )
      send_polbut_message( &KPad );
   } 

  // ���������� ������, ���� ��������� ����� �� ������������ �����  
  vTaskDelete( NULL );
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
void send_polbut_message( const sKeyMesg* temp )
{
  s_pol_button message;

  if( temp->typeEvent == TWO_BUTTON_PRESSED )
  {
	message.is_double_click = 1;
	message.num = temp->num;
	message.prev_but_num = temp->prev_but_num;
  }
  else
  {
    message.is_double_click = 0;
	message.num = temp->num;
	message.isKeyPad  = 1;  // only KeyPad
	message.EventType = temp->typeEvent;
  }

  //"��������" ���� � ������� PB_to_M �� �������� ��������� �����
  // send message to qPB_to_M queue
  xQueueSend( qPB_to_M, (void *)&message, portMAX_DELAY );
}
