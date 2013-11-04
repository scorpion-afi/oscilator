
// MTask 

#define MTask	
#include "Menu_Task.h"

#include "Menu_Drv.h"

#include "InterDefines.h"   //����������� sBeepParam, sLCDParam

#include "CommonDefines.h"  //��� �������������� � FreeRTOS

void preInitDAC(const sChannel *ptr, char curCh);

// �������, ���������� ����� MTask
//==============================================================================
void vMTask(void *pvParameters)
{ 
  //��������� �� PBTask
  char PBMessage;
  
  char ButNum;
  char isKeyPad;
  char EventType; 
  
  InitView(); 
     
  while(1)
  { 
    //���� ������� ��������� �� PBTask
    xQueueReceive(qPB_to_M, (void *)&PBMessage, portMAX_DELAY);
       
    isKeyPad = PrepareMesg( PBMessage, &ButNum, &EventType );
    MenuStateMach( ButNum, EventType, isKeyPad );
  } 
  
  // ���������� ������, ���� ��������� ����� �� ������������ �����  
  vTaskDelete(NULL);
}


//==============================================================================
//==============================================================================
//==============================================================================


// ��������� ��������� �� PBTask 
//==============================================================================
char PrepareMesg( char Mesg, char *ButNum, char *EventType )
{
  char temp;
  char dig;

  if((Mesg & 0xE0) == (0x01<<5))              // ButtonOn
    *EventType = 1;
  else if((Mesg & 0xE0) == (0x02<<5))         // ButtonPress
    *EventType = 2;

  //temp = Mesg & 0x01;             // �������� ��� ����������
  temp = 1;
  
  dig = (Mesg & 0x1E)>>1;         // �������� ����� ������
    
  *ButNum = dig;
      
  // Channel select    0  
  // Signal Form       1 
  // Amplitude/Sigma   2
  // Offset/Mean       3  
  // Frequency         4
  // Duty              5
  // +                 6
  // -                 7
  
  return temp;  
}

//���������� � ��� ������ pStr �� ������� (x, y)
//==============================================================================
void lcd_write(char *pStr, char x, char y)
{ 
  sLCDParam temp;
  
  temp.ID_cmd = 0; //SendString(pData, param_1, param_2)
  
  //�������� �������, ������� ������ ���������� � ��������� ����, �� ASCII ������
  ChangeChar(pStr);

  temp.pData  = pStr;
  temp.param_1 = x;
  temp.param_2 = y; 
  
  xQueueSend(qM_to_LCD, (void *)&temp, portMAX_DELAY);
}

//�������� ������ �� ������� (x, y)
//==============================================================================
void shift_blink(char x, char y)
{
  sLCDParam temp;
  
  temp.ID_cmd = 1; //ShiftBlink(param_1, param_2);

  temp.param_1 = x;
  temp.param_2 = y; 
  
  xQueueSend(qM_to_LCD, (void *)&temp, portMAX_DELAY); 
}

//��������� ������ 
// isOn - ���� 1, �� ������ �������
//==============================================================================
void blink_control(int isOn)
{
  sLCDParam temp;
  
  temp.ID_cmd = 2; //ControlBlink(param_1, param_2, param_3);

  temp.param_1 = 1;
  temp.param_2 = isOn; 
  temp.param_3 = isOn; 
  
  xQueueSend(qM_to_LCD, (void *)&temp, portMAX_DELAY); 
}

// ����������������� DAC
// ptr - ��������� �� ��������� ���� sChannel, ������ ����������� ����������
// ��� �������� ������ Osc_Thread
// curCh - ������� ����� ( 0 - Channel 1, 1 - Channel 2 )
//==============================================================================
void preInitDAC(const sChannel *ptr, char curCh)
{
  sOscParam temp;
  
  //���� ����� �� �������, �� � �� ���� �������� ���������
  if(ptr->isOn != 1) return;   
  
  temp.Ch_num = curCh;
  temp.Sig_Type = ptr->CurSigForm;
  
  //��� ��������-�����
  if((temp.Sig_Type == 4) || (temp.Sig_Type == 5))
  {
    temp.amp = ptr->SignalParam[ptr->CurSigForm].Param[5].cur;   
    temp.offset = ptr->SignalParam[ptr->CurSigForm].Param[4].cur;   
  }
  else
  {
    temp.amp = ptr->SignalParam[ptr->CurSigForm].Param[0].cur;   
    temp.offset = ptr->SignalParam[ptr->CurSigForm].Param[1].cur;
    
    temp.duty = ptr->SignalParam[ptr->CurSigForm].Param[3].cur;
    temp.freq = ptr->SignalParam[ptr->CurSigForm].Param[2].cur;
  }
  
  xQueueSend(qTo_Osc, (void *)&temp, portMAX_DELAY);
}

//���������� ������ ����
//ChNum - ����� ������, ������� ����� ���������
//==============================================================================
void Off_DAC_Channel(char ChNum)
{ 
  sOscParam temp;
  
  //zero("switch off" generator)
  temp.Sig_Type = 6;
  temp.Ch_num = ChNum;
   
  xQueueSend(qTo_Osc, (void *)&temp, portMAX_DELAY);
}

// controls measuing hardware
// if is_on == 1, then  on
// else -                off
//==============================================================================
void meas_control( char is_on )
{
  if ( ( is_on == 0 ) || ( is_on == 1 ) )
  {
    xQueueSend( queu_to_meas, ( void * )&is_on, portMAX_DELAY );   
  }  
}

//
// pStr - ��������� �� ����-��������������� ������
//
//!!!
//
// ����� ������������� ������, ���� ������ ����� �� ���� ������,
// �� ������� �� ����������� �������� � Hard fault exception
//
//!!!
//
//==============================================================================
void ChangeChar(char *pStr)
{
  while(*pStr != 0)
  {
    if(*pStr == '�') *pStr = 0xFF;   //������ ������ ���������
    else if(*pStr == '�') *pStr = 0x01;   //������ �����(���������, ����� � CGRAM)
      pStr++;
  }
}





