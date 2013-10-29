
//������� ���������

#define PBDrv	
#include "PolBut_Drv.h"

// ����� state machine  � �������� 10 ��

int repet_count = 5;          //��� ������������  (50 ��)
int press_count = 100;        //��� �������� � ��������� ������ (1 �)
int T_press = 20;             //��� �������������� ��������� � ������� ������ (200 ��)


// ������������� ������ ������ ������
//===================================================================================
void InitPad_Task(void)
{
  //��������� ������ i/o
  initGPIO_Pads();
  
  // KeyPad --------------------------------------------------------------------
  
  // SignalForm  PC8   ?
  ButtonDescr_K[0].cnt = 0;
  ButtonDescr_K[0].state = 0;
  ButtonDescr_K[0].ID = GPIO_Pin_8;
  ButtonDescr_K[0].PortName = GPIOC;
  
  // Channel  PC7   ?   
  ButtonDescr_K[1].ID = GPIO_Pin_7;
  ButtonDescr_K[1].PortName = GPIOC;
  
  // Offset  PA8   ?   
  ButtonDescr_K[2].ID = GPIO_Pin_8;
  ButtonDescr_K[2].PortName = GPIOA;
  
  // Amplitude  PC9    ?  
  ButtonDescr_K[3].ID = GPIO_Pin_9;
  ButtonDescr_K[3].PortName = GPIOC;
  
  // Frequency  PA9   ?   
  ButtonDescr_K[4].ID = GPIO_Pin_9;
  ButtonDescr_K[4].PortName = GPIOA;
  
  // Duty  PA10     ? 
  ButtonDescr_K[5].ID = GPIO_Pin_10;
  ButtonDescr_K[5].PortName = GPIOA;
  
  // Up  PA11    ?  
  ButtonDescr_K[6].ID = GPIO_Pin_11;
  ButtonDescr_K[6].PortName = GPIOA;
  
  // Down  PA12   ?   
  ButtonDescr_K[7].ID = GPIO_Pin_12;
  ButtonDescr_K[7].PortName = GPIOA;
}

//����� KeyPad ����������
// ��� ������� ������ ���������� ������������
// GetSst_Key ��� ������ ������ ������ ������� ��������� �� ������ ��
// Key_State_machine ������ �� ���������� ������ ������ KeyPad ���������� 
//���������� ��������� sKeyMesg, � ������� ����������� ��� ������� � ����� ������(0 - 7)
// ���� ���� typeEvent ����� -1, �� ������� �� ���������
//===================================================================================
sKeyMesg GetKeyPadState(void)
{
  sKeyMesg temp;
    
  temp.typeEvent = -1;
 
  if(GetSst_Key())    //���� ������ ������ ���� ������, ����� ����������� ��������� ButtonDescr_K
  {
    for(int i = 0; i < 8; i++)
    {
      temp.typeEvent = Key_State_machine(&ButtonDescr_K[i]);
      if(temp.typeEvent != -1)
      {
        temp.num = i;
        break;
      }
    }
  } 
  return temp; 
}


//===================================================================================
//===================================================================================
//===================================================================================


//===================================================================================
// ����� �������� ��������� ������ ��� KeyPad ����������
// ��������� ���� Sst �������� ButtonDescr_K.
//���������� 1 - ������ ������ ���� ������,
//           0 - � ���� ������ ������� ������ ��������� ������.
int GetSst_Key(void)
{
  int temp;
  
  temp = 0;
  
  for(int i = 0; i < 8; i++)
  {      
    //������ ��  ������ (������ �������� ����� ��������� �� �����)
    if( (ButtonDescr_K[i].ID & GPIO_ReadInputData(ButtonDescr_K[i].PortName)) == ButtonDescr_K[i].ID )
      ButtonDescr_K[i].Sst = 0;  // �� ������ / ������ 
    else
    {
      if(++temp > 1) return 0;  // ���� � ���� ������ ������� ������ ��������� ������ 
      ButtonDescr_K[i].Sst = 1;  // ������ / �� ������
    }
  }
  return 1; 
}

//===================================================================================
// State Machine ���  ������
//ButtonDescr - ��������� �� ��������� sButtonDescr, ����������� ������
//���������� ��� �������(ButtonOn ��� ButtonPress ��� -1, ���� ���� �������)
//��� ������� ������, ������� ���������������� ButtonDescr
int Key_State_machine(sButtonDescr *ButtonDescr)
{ 
  int temp;
  
  temp = -1;
  
  switch(ButtonDescr->state)
  {
    case 0:   // ��������� ����.  OFF 
    {
        if(ButtonDescr->Sst) // ������
        {
            ButtonDescr->cnt++;
            if(ButtonDescr->cnt > repet_count)
            {
                ButtonDescr->state = 1;
       
                ButtonDescr->cnt = 0;
                ButtonDescr->presscnt = 0;              
                //EventOn                                        // 1
                temp = ButtonOn;
            }
        }
        else                  // �� ������
            ButtonDescr->cnt = 0;
    }
    break;
    
    case 1:   // ��������� ���.  ON 
    {
        if(!ButtonDescr->Sst) // ������
        {
            ButtonDescr->cnt++;
            if(ButtonDescr->cnt > repet_count)
            {
                ButtonDescr->state = 0;
            
                ButtonDescr->cnt = 0;                         
                //EventOff;                                      // 2
            }
        }
        else                  // �� ������
        {
            ButtonDescr->cnt = 0;
            ButtonDescr->presscnt++;
            if(ButtonDescr->presscnt > press_count) // > 2 �
            {
                ButtonDescr->state = 2;
                
                ButtonDescr->presscnt = 0;                      
                //EventToPress                                   // 3
            }
        }
    }
    break;  //case 1
    
    case 2:   // ��������� ������. Press
    {
        if(!ButtonDescr->Sst) // ������
        {
            ButtonDescr->cnt++;
            if(ButtonDescr->cnt > repet_count)
            {
                ButtonDescr->state = 0;
                
                ButtonDescr->cnt = 0;                           
                //ButtonDescr->EventOff = 1;                     // 4
            }
        }
        else                  // �� ������
        {
            ButtonDescr->cnt = 0;
            ButtonDescr->presscnt++;
            if(ButtonDescr->presscnt > T_press) // > ? �
            {
                ButtonDescr->presscnt = 0;
                //EventPress                                     // 5 
                temp = ButtonPress;
            }
        }
    }
    break;  // case 2
    
    default:
      break;   
  };  // switch(ButtonDescr->state)
  
  return temp;
}

//��������� ������ i/o ��� ���������
//===================================================================================
void initGPIO_Pads(void)
{
  // KeyPad --------------------------------------------------------------------
 
  GPIO_StructInit(&GPIO_InitStruc_Pad);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  
  GPIO_InitStruc_Pad.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStruc_Pad.GPIO_Mode = GPIO_Mode_IPU; // input pull-up

  GPIO_Init(GPIOC, &GPIO_InitStruc_Pad);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
  GPIO_InitStruc_Pad.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_InitStruc_Pad.GPIO_Mode = GPIO_Mode_IPU; // input pull-up

  GPIO_Init(GPIOA, &GPIO_InitStruc_Pad);
}