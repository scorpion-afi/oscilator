
//драйвер LCD 

#ifdef	LCDDrv
  #define GLB_LCDDrv 			
#else	
  #define GLB_LCDDrv extern
#endif

  // Начальная инициализация LCD 
  GLB_LCDDrv void InitLCD(void);
  
  // Посылка строки данных в ж/к контроллер
  //data - указатель на строку символов с нулем в конце
  //pos - позиция в строке (0 -19)
  //nRow - номер строки экрана(0 - 3)
  GLB_LCDDrv void SendString(char *data, char pos, char nRow);
  
  // сдвиг курсора
  //pos - позиция в строке (0 -19)
  //nRow - номер строки экрана(0 - 3)
  GLB_LCDDrv void ShiftBlink(char pos, char nRow);
    
  //управление экраном и курсором
  // d - 1: экран включен
  // c - 1: курсор включен
  // b - 1: курсор мигает  
  GLB_LCDDrv void ControlBlink(char d, char c, char b);
  
  // очистка всего дисплэя
  GLB_LCDDrv void ClearLCD(void);

#ifdef	LCDDrv
  
  #include "stm32f10x.h"
  
  #define RS_Pin             GPIO_Pin_12
  #define RW_Pin             GPIO_Pin_5
  
  #define RS_Clear__E_Set    GPIO_Pin_6
  #define RS_Set__E_Set      GPIO_Pin_12 | GPIO_Pin_6
  
  #define RS_Clear__E_Clear  0
  #define RS_Set__E_Clear    GPIO_Pin_12
  
  #define Function_set_4bit  0x2000   //для перевода из 8 в 4-битный режим
  #define Function_StartUp   0x3000   //для преинициализации LCD
  
  // Настройка портов в/в.
  void InitGPIO_LCD(void);

  // необходимо для корректной работы(сброс командами, а не внутренней цепью сброса)
  void Preinit_LCD(void);
  
  //перевод режима работы из 8-битного(после подачи питания)в 4-битный
  void SendFirstCommand(void);
  
  // Посылка команд в ж/к контроллер
  void SendCommand(uint8_t command);
  
  // Посылка  данных(1 байт) в ж/к контроллер
  void SendData(uint8_t data);
  
  // Создание символа сигма (СКО)
  void CreateSigmaPattern(void);
  
  // задержка в нано секундах
  extern void Delay_asm(int period);
  
  GPIO_InitTypeDef          GPIO_InitStruct;
 
#endif