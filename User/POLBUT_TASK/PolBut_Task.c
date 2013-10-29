
// PBTask 

#define PBTask
#include "PolBut_Task.h"

#include "PolBut_Drv.h"      //для вызова функций драйвера клавиатур

#include "CommonDefines.h"  //для взаимодействия с FreeRTOS

//sKeyMesg KPadGL;
// функция, релизующяя поток PBTask
//==============================================================================
void vPBTask(void *pvParameters)
{ 
  sKeyMesg KPad;
  portTickType xLastWakeTime;
  
  //инициализация аппаратуры, связанной с опросом клавиатур
  InitPad_Task();
  
  //вызывается только один раз
  xLastWakeTime = xTaskGetTickCount();
  
  while(1)
  { 
    // Задержка на 10 миллисекунд
    vTaskDelayUntil( &xLastWakeTime, ( 10 / portTICK_RATE_MS ) );
        
    //опрос состояний кнопок KeyPad клавиатуры 
    //KPad.num: 0 - 7
    KPad = GetKeyPadState();
    
    //KPadGL = KPad;
    if(KPad.typeEvent != -1)
      Send_PolBut_Message(KPad.typeEvent, 1, KPad.num);  
   } 

  // Уничтожить задачу, если произошел выход из бесконечного цикла  
  vTaskDelete(NULL);
}

// Посылка сообщения  в очередь потоку MTask
//typeEvent - тип события
//typePad - тип клавиатуры (1 - KeyPad, 0 - MatrixPad)
//num - номер кнопки(0-7 или 0-11)

// очередь к потоку Main_Task состоит из 5 элементов
// размер элемента - 1 байт
// 7:5 биты: тип события(1 - ButtonOn, 2 - ButtonPress)
// 4:1 биты: номер кнопки (0-7 или 0-11)
// 0 бит: тип клавиатуры (1 - KeyPad, 0 - MatrixPad)
//===================================================================================
void Send_PolBut_Message(int typeEvent, int typePad, int num)
{
  char value;
    
  value =  (char)(num<<1);
  value |= (char)typePad;
  value |= (char)(typeEvent<<5);
  
  //"зависнем" пока в очереди PB_to_M не появится свободное место
  xQueueSend(qPB_to_M, (void *)&value, portMAX_DELAY);
}
