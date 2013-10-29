
//драйвер LCD

#define LCDDrv	
#include "LCD_Drv.h"
	
//===================================================================================
// Начальная инициализация LCD
void InitLCD(void)
{
  InitGPIO_LCD();               // Настройка портов в/в для LCD.
  
  Preinit_LCD();
  
  SendFirstCommand();		//переходим в 4-битный режим
  Delay_asm(526);		//задержка на 37 мкс
  
  //используем 4-битную шину данных/адресов : DB7 .. DB4 
  //используем обе строки                0x28
  //шрифт размером 5x8 пикселей          0x28
  //используем одну строку               0x24
  //шрифт размером 5x10 пикселей         0x24
  SendCommand(0x28);		//Functon Set
  Delay_asm(526);		//задержка на 37 мкс
  
  //включить дисплей
  //отключить курсор (символ подчеркивания)
  //включить курсор (черный квадрат)
  SendCommand(0x0D);		//Dispay ON/OFF Control
  Delay_asm(526);		//задержка на 37 мкс
  
  //слева на право (инкремент)
  //дезактивируем сдвиг
  SendCommand(0x06);		//Entry Mode Set
  Delay_asm(526);		//задержка на 37 мкс
  
  // очистка дисплэя
  SendCommand(0x01);		//Clear Display
  Delay_asm(28569);		//задержка на 2000 мкс
    
  // Создание символа сигма (СКО)
  CreateSigmaPattern();
}

//===================================================================================
// Посылка строки данных в ж/к контроллер
//data - указатель на строку символов с нулем в конце
//pos - позиция в строке (0 -19)
//nRow - номер строки экрана(0 - 3)
void SendString(char *data, char pos, char nRow)
{
  uint8_t ch;
  
  switch(nRow)	//вычисление адресса  строки в памяти ж/к контроллера
  {
    case 0:
     pos += 0x0;
    break;

    case 1:
     pos += 0x40;
    break;

    case 2:
     pos += 0x14;
    break;

    case 3:
     pos += 0x54;
    break;

    default:
    break;
  };

  pos |= 0x80;		// получатель DDRAM 
  SendCommand((uint8_t)pos);	// установка начального адресса 
  
  Delay_asm(526);		//задержка на 37 мкс

  while( (ch = (uint8_t)(*data++)) != 0 )
  {
    SendData(ch);
    Delay_asm(569);		//задержка на 40 мкс
  }
}

//===================================================================================
//управление экраном и курсором
// d - 1: экран включен
// c - 1: курсор включен
// b - 1: курсор мигает  
void ControlBlink(char d, char c, char b)
{
  uint8_t temp = 0;
    
  //формируем команду
  temp  = (uint8_t)d<<2;
  temp |= (uint8_t)c<<1;
  temp |= (uint8_t)b;
  temp |= 0x8;
  
  SendCommand(temp);	// Display on/off control
  Delay_asm(526);	//задержка на 37 мкс
}

//===================================================================================
// очистка всего дисплэя
void ClearLCD(void)
{
  SendCommand(0x01);		//Clear Display
  Delay_asm(28569);		//задержка на 2000 мкс
}

//===================================================================================
// сдвиг курсора
//pos - позиция в строке (0 -19)
//nRow - номер строки экрана(0 - 3)
void ShiftBlink(char pos, char nRow)
{
  switch(nRow)	//вычисление адресса  строки в памяти ж/к контроллера
  {
    case 0:
     pos += 0x0;
    break;

    case 1:
     pos += 0x40;
    break;

    case 2:
     pos += 0x14;
    break;

    case 3:
     pos += 0x54;
    break;

    default:
    break;
  };

  pos |= 0x80;		        // получатель DDRAM 
  SendCommand((uint8_t)pos);	// установка адресса (тут будет мигать кусор)
  
  Delay_asm(526);		//задержка на 37 мкс
}



//===================================================================================
//===================================================================================
//===================================================================================



//===================================================================================
// Настройка портов в/в для LCD.
void InitGPIO_LCD(void)
{ 
  GPIO_StructInit(&GPIO_InitStruct);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB, ENABLE);
  
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 

  GPIO_Init(GPIOB, &GPIO_InitStruct); 
  
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOC, &GPIO_InitStruct); 
  
  GPIO_WriteBit(GPIOC, RW_Pin, Bit_RESET);  //RW = 0 - будем записывать данные
}

//===================================================================================
// необходимо для корректной работы(сброс командами, а не внутренней цепью сброса)
void Preinit_LCD(void)
{
  Delay_asm(571426);		//40 мс секунд
  
  GPIO_WriteBit(GPIOC, RS_Pin, Bit_RESET);  //RS = 0 - передаем команду
  Delay_asm(1);		//50 нано секунд
  
  //Delay_asm(571426);		//40 мс секунд

  GPIO_Write(GPIOB, Function_StartUp | RS_Clear__E_Set);    // E = 1
  Delay_asm(2);	        //250 нано секунд
  GPIO_Write(GPIOB, Function_StartUp | RS_Clear__E_Clear);  // E = 0	

  Delay_asm(71426);		 // 5 мс секунд
  
  GPIO_Write(GPIOB, Function_StartUp | RS_Clear__E_Set);    // E = 1
  Delay_asm(2);	        //250 нано секунд
  GPIO_Write(GPIOB, Function_StartUp | RS_Clear__E_Clear);  // E = 0	
  
  Delay_asm(1426);		 // 100 мкс секунд

  GPIO_Write(GPIOB, Function_StartUp | RS_Clear__E_Set);    // E = 1
  Delay_asm(2);	        //250 нано секунд
  GPIO_Write(GPIOB, Function_StartUp | RS_Clear__E_Clear);  // E = 0	

  Delay_asm(1426);		 // 100 мкс секунд
}

//===================================================================================
//перевод режима работы из 8-битного(после подачи питания)в 4-битный
void SendFirstCommand(void)
{    
  GPIO_WriteBit(GPIOC, RS_Pin, Bit_RESET);  //RS = 0 - передаем команду
  Delay_asm(1);		//50 нано секунд

  GPIO_Write(GPIOB, Function_set_4bit | RS_Clear__E_Set);    // E = 1
  Delay_asm(1);	        //250 нано секунд
  GPIO_Write(GPIOB, Function_set_4bit | RS_Clear__E_Clear);  // E = 0	
}

//===================================================================================
// Посылка команд в ж/к контроллер
// command - команда к отправке
// DB7...DB4 - 7...10 биты порта GPIOB !!!
void SendCommand(uint8_t command)
{
  uint16_t command_16;
  
  GPIO_WriteBit(GPIOC, RS_Pin, Bit_RESET);  //RS = 0 - передаем команду
  Delay_asm(1);		//50 нано секунд

  // запись старшей тетрады байта command
  command_16 = (uint16_t)(command>>4);
  command_16 <<= 7;
  GPIO_Write(GPIOB, command_16 | RS_Clear__E_Set);    // E = 1
  Delay_asm(1);	        //250 нано секунд
  GPIO_Write(GPIOB, command_16 | RS_Clear__E_Clear);  // E = 0	


  Delay_asm(1);		//40 нано секунд     на всякий случай

  // запись младшей тетрады байта command
  command_16 = (uint16_t)(command & 0x0F);
  command_16 <<= 7;
  GPIO_Write(GPIOB, command_16 | RS_Clear__E_Set);    // E = 1
  Delay_asm(1);	        //250 нано секунд
  GPIO_Write(GPIOB, command_16 | RS_Clear__E_Clear);  // E = 0	
}

//===================================================================================
// Посылка  данных(1 байт) в ж/к контроллер
// data - данные, которые нужно отправить
// DB7...DB4 - 7...10 биты порта GPIOB !!!
void SendData(uint8_t data)
{
  uint16_t data_16;

  GPIO_WriteBit(GPIOC, RS_Pin, Bit_SET);  //RS =  - передаем данные
  Delay_asm(1);		  //50 нано секунд

  // запись старшей тетрады байта command
  data_16 = (uint16_t)(data>>4);
  data_16 <<= 7;
  GPIO_Write(GPIOB, data_16 | RS_Set__E_Set);    // E = 1
  Delay_asm(1);	          //250 нано секунд
  GPIO_Write(GPIOB, data_16 | RS_Set__E_Clear);  // E = 0	

  Delay_asm(1);		  //40 нано секунд     на всякий случай

  // запись младшей тетрады байта command
  data_16 = (uint16_t)(data & 0x0F);
  data_16 <<= 7;
  GPIO_Write(GPIOB, data_16 | RS_Set__E_Set);    // E = 1
  Delay_asm(1);	          //250 нано секунд
  GPIO_Write(GPIOB, data_16 | RS_Set__E_Clear);  // E = 0	
}

// Создание символа сигма (СКО)
// будет лежать по адресу 0x01
void CreateSigmaPattern(void)
{
  //Set CGRAM address to 0x08 -> DDRAM address 0x01
  SendCommand(0x48);
  Delay_asm(526);		//задержка на 37 мкс

  SendData(1);
  Delay_asm(569);		//задержка на 40 мкс
  
  SendData(14);
  Delay_asm(569);		//задержка на 40 мкс
  
  SendData(16); 
  Delay_asm(569);		//задержка на 40 мкс
  
  SendData(22);
  Delay_asm(569);		//задержка на 40 мкс
  
  SendData(17);
  Delay_asm(569);		//задержка на 40 мкс
  
  SendData(17);
  Delay_asm(569);		//задержка на 40 мкс
  
  SendData(14);
  Delay_asm(569);		//задержка на 40 мкс
  
  SendData(0);  
  Delay_asm(569);		//задержка на 40 мкс
}
