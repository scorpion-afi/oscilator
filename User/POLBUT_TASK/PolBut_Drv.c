
//драйвер клавиатур

#define PBDrv	
#include "PolBut_Drv.h"

// вызов state machine  с периодом 10 мс

int repet_count = 5;          //для антидребезга  (50 мс)
int press_count = 100;        //для перехода в состояние ЗАЖАТО (1 с)
int T_press = 20;             //для периодического извещения о зажатии кнопки (200 мс)


// Инициализация задачи опроса кнопок
//===================================================================================
void InitPad_Task(void)
{
  //настройка портов i/o
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

//опрос KeyPad клавиатуры
// эта функция должна вызываться периодически
// GetSst_Key при каждом вызове узнает текущее состояние на ножках мк
// Key_State_machine следит за состоянием каждой кнопки KeyPad клавиатуры 
//возвращяет структуру sKeyMesg, в которой указывается тип события и номер кнопки(0 - 7)
// если поле typeEvent равно -1, то событие не произошло
//===================================================================================
sKeyMesg GetKeyPadState(void)
{
  sKeyMesg temp;
    
  temp.typeEvent = -1;
 
  if(GetSst_Key())    //если нажата только ОДНА кнопка, также заполняются структуры ButtonDescr_K
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
// Опрос текущего состояния кнопок для KeyPad клавиатуры
// заполняем поля Sst структур ButtonDescr_K.
//возвращяет 1 - нажата только ОДНА кнопка,
//           0 - в один момент времени нажато несколько кнопок.
int GetSst_Key(void)
{
  int temp;
  
  temp = 0;
  
  for(int i = 0; i < 8; i++)
  {      
    //нажата ли  кнопка (кнопка посажена одним контактом на землю)
    if( (ButtonDescr_K[i].ID & GPIO_ReadInputData(ButtonDescr_K[i].PortName)) == ButtonDescr_K[i].ID )
      ButtonDescr_K[i].Sst = 0;  // не нажата / отжата 
    else
    {
      if(++temp > 1) return 0;  // если в один момент времени нажато несколько кнопок 
      ButtonDescr_K[i].Sst = 1;  // нажата / не отжата
    }
  }
  return 1; 
}

//===================================================================================
// State Machine для  кнопок
//ButtonDescr - указатель на структуру sButtonDescr, описывающую кнопку
//возвращяет тип события(ButtonOn или ButtonPress или -1, если нету события)
//для текущей кнопки, которая идентифицируется ButtonDescr
int Key_State_machine(sButtonDescr *ButtonDescr)
{ 
  int temp;
  
  temp = -1;
  
  switch(ButtonDescr->state)
  {
    case 0:   // состояние ВЫКЛ.  OFF 
    {
        if(ButtonDescr->Sst) // нажата
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
        else                  // не нажата
            ButtonDescr->cnt = 0;
    }
    break;
    
    case 1:   // состояние ВКЛ.  ON 
    {
        if(!ButtonDescr->Sst) // отжата
        {
            ButtonDescr->cnt++;
            if(ButtonDescr->cnt > repet_count)
            {
                ButtonDescr->state = 0;
            
                ButtonDescr->cnt = 0;                         
                //EventOff;                                      // 2
            }
        }
        else                  // не отжата
        {
            ButtonDescr->cnt = 0;
            ButtonDescr->presscnt++;
            if(ButtonDescr->presscnt > press_count) // > 2 с
            {
                ButtonDescr->state = 2;
                
                ButtonDescr->presscnt = 0;                      
                //EventToPress                                   // 3
            }
        }
    }
    break;  //case 1
    
    case 2:   // состояние ЗАЖАТО. Press
    {
        if(!ButtonDescr->Sst) // отжата
        {
            ButtonDescr->cnt++;
            if(ButtonDescr->cnt > repet_count)
            {
                ButtonDescr->state = 0;
                
                ButtonDescr->cnt = 0;                           
                //ButtonDescr->EventOff = 1;                     // 4
            }
        }
        else                  // не отжата
        {
            ButtonDescr->cnt = 0;
            ButtonDescr->presscnt++;
            if(ButtonDescr->presscnt > T_press) // > ? с
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

//настройка портов i/o для клавиатур
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