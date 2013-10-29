
//������� LCD

#define LCDDrv	
#include "LCD_Drv.h"
	
//===================================================================================
// ��������� ������������� LCD
void InitLCD(void)
{
  InitGPIO_LCD();               // ��������� ������ �/� ��� LCD.
  
  Preinit_LCD();
  
  SendFirstCommand();		//��������� � 4-������ �����
  Delay_asm(526);		//�������� �� 37 ���
  
  //���������� 4-������ ���� ������/������� : DB7 .. DB4 
  //���������� ��� ������                0x28
  //����� �������� 5x8 ��������          0x28
  //���������� ���� ������               0x24
  //����� �������� 5x10 ��������         0x24
  SendCommand(0x28);		//Functon Set
  Delay_asm(526);		//�������� �� 37 ���
  
  //�������� �������
  //��������� ������ (������ �������������)
  //�������� ������ (������ �������)
  SendCommand(0x0D);		//Dispay ON/OFF Control
  Delay_asm(526);		//�������� �� 37 ���
  
  //����� �� ����� (���������)
  //������������� �����
  SendCommand(0x06);		//Entry Mode Set
  Delay_asm(526);		//�������� �� 37 ���
  
  // ������� �������
  SendCommand(0x01);		//Clear Display
  Delay_asm(28569);		//�������� �� 2000 ���
    
  // �������� ������� ����� (���)
  CreateSigmaPattern();
}

//===================================================================================
// ������� ������ ������ � �/� ����������
//data - ��������� �� ������ �������� � ����� � �����
//pos - ������� � ������ (0 -19)
//nRow - ����� ������ ������(0 - 3)
void SendString(char *data, char pos, char nRow)
{
  uint8_t ch;
  
  switch(nRow)	//���������� �������  ������ � ������ �/� �����������
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

  pos |= 0x80;		// ���������� DDRAM 
  SendCommand((uint8_t)pos);	// ��������� ���������� ������� 
  
  Delay_asm(526);		//�������� �� 37 ���

  while( (ch = (uint8_t)(*data++)) != 0 )
  {
    SendData(ch);
    Delay_asm(569);		//�������� �� 40 ���
  }
}

//===================================================================================
//���������� ������� � ��������
// d - 1: ����� �������
// c - 1: ������ �������
// b - 1: ������ ������  
void ControlBlink(char d, char c, char b)
{
  uint8_t temp = 0;
    
  //��������� �������
  temp  = (uint8_t)d<<2;
  temp |= (uint8_t)c<<1;
  temp |= (uint8_t)b;
  temp |= 0x8;
  
  SendCommand(temp);	// Display on/off control
  Delay_asm(526);	//�������� �� 37 ���
}

//===================================================================================
// ������� ����� �������
void ClearLCD(void)
{
  SendCommand(0x01);		//Clear Display
  Delay_asm(28569);		//�������� �� 2000 ���
}

//===================================================================================
// ����� �������
//pos - ������� � ������ (0 -19)
//nRow - ����� ������ ������(0 - 3)
void ShiftBlink(char pos, char nRow)
{
  switch(nRow)	//���������� �������  ������ � ������ �/� �����������
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

  pos |= 0x80;		        // ���������� DDRAM 
  SendCommand((uint8_t)pos);	// ��������� ������� (��� ����� ������ �����)
  
  Delay_asm(526);		//�������� �� 37 ���
}



//===================================================================================
//===================================================================================
//===================================================================================



//===================================================================================
// ��������� ������ �/� ��� LCD.
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
  
  GPIO_WriteBit(GPIOC, RW_Pin, Bit_RESET);  //RW = 0 - ����� ���������� ������
}

//===================================================================================
// ���������� ��� ���������� ������(����� ���������, � �� ���������� ����� ������)
void Preinit_LCD(void)
{
  Delay_asm(571426);		//40 �� ������
  
  GPIO_WriteBit(GPIOC, RS_Pin, Bit_RESET);  //RS = 0 - �������� �������
  Delay_asm(1);		//50 ���� ������
  
  //Delay_asm(571426);		//40 �� ������

  GPIO_Write(GPIOB, Function_StartUp | RS_Clear__E_Set);    // E = 1
  Delay_asm(2);	        //250 ���� ������
  GPIO_Write(GPIOB, Function_StartUp | RS_Clear__E_Clear);  // E = 0	

  Delay_asm(71426);		 // 5 �� ������
  
  GPIO_Write(GPIOB, Function_StartUp | RS_Clear__E_Set);    // E = 1
  Delay_asm(2);	        //250 ���� ������
  GPIO_Write(GPIOB, Function_StartUp | RS_Clear__E_Clear);  // E = 0	
  
  Delay_asm(1426);		 // 100 ��� ������

  GPIO_Write(GPIOB, Function_StartUp | RS_Clear__E_Set);    // E = 1
  Delay_asm(2);	        //250 ���� ������
  GPIO_Write(GPIOB, Function_StartUp | RS_Clear__E_Clear);  // E = 0	

  Delay_asm(1426);		 // 100 ��� ������
}

//===================================================================================
//������� ������ ������ �� 8-�������(����� ������ �������)� 4-������
void SendFirstCommand(void)
{    
  GPIO_WriteBit(GPIOC, RS_Pin, Bit_RESET);  //RS = 0 - �������� �������
  Delay_asm(1);		//50 ���� ������

  GPIO_Write(GPIOB, Function_set_4bit | RS_Clear__E_Set);    // E = 1
  Delay_asm(1);	        //250 ���� ������
  GPIO_Write(GPIOB, Function_set_4bit | RS_Clear__E_Clear);  // E = 0	
}

//===================================================================================
// ������� ������ � �/� ����������
// command - ������� � ��������
// DB7...DB4 - 7...10 ���� ����� GPIOB !!!
void SendCommand(uint8_t command)
{
  uint16_t command_16;
  
  GPIO_WriteBit(GPIOC, RS_Pin, Bit_RESET);  //RS = 0 - �������� �������
  Delay_asm(1);		//50 ���� ������

  // ������ ������� ������� ����� command
  command_16 = (uint16_t)(command>>4);
  command_16 <<= 7;
  GPIO_Write(GPIOB, command_16 | RS_Clear__E_Set);    // E = 1
  Delay_asm(1);	        //250 ���� ������
  GPIO_Write(GPIOB, command_16 | RS_Clear__E_Clear);  // E = 0	


  Delay_asm(1);		//40 ���� ������     �� ������ ������

  // ������ ������� ������� ����� command
  command_16 = (uint16_t)(command & 0x0F);
  command_16 <<= 7;
  GPIO_Write(GPIOB, command_16 | RS_Clear__E_Set);    // E = 1
  Delay_asm(1);	        //250 ���� ������
  GPIO_Write(GPIOB, command_16 | RS_Clear__E_Clear);  // E = 0	
}

//===================================================================================
// �������  ������(1 ����) � �/� ����������
// data - ������, ������� ����� ���������
// DB7...DB4 - 7...10 ���� ����� GPIOB !!!
void SendData(uint8_t data)
{
  uint16_t data_16;

  GPIO_WriteBit(GPIOC, RS_Pin, Bit_SET);  //RS =  - �������� ������
  Delay_asm(1);		  //50 ���� ������

  // ������ ������� ������� ����� command
  data_16 = (uint16_t)(data>>4);
  data_16 <<= 7;
  GPIO_Write(GPIOB, data_16 | RS_Set__E_Set);    // E = 1
  Delay_asm(1);	          //250 ���� ������
  GPIO_Write(GPIOB, data_16 | RS_Set__E_Clear);  // E = 0	

  Delay_asm(1);		  //40 ���� ������     �� ������ ������

  // ������ ������� ������� ����� command
  data_16 = (uint16_t)(data & 0x0F);
  data_16 <<= 7;
  GPIO_Write(GPIOB, data_16 | RS_Set__E_Set);    // E = 1
  Delay_asm(1);	          //250 ���� ������
  GPIO_Write(GPIOB, data_16 | RS_Set__E_Clear);  // E = 0	
}

// �������� ������� ����� (���)
// ����� ������ �� ������ 0x01
void CreateSigmaPattern(void)
{
  //Set CGRAM address to 0x08 -> DDRAM address 0x01
  SendCommand(0x48);
  Delay_asm(526);		//�������� �� 37 ���

  SendData(1);
  Delay_asm(569);		//�������� �� 40 ���
  
  SendData(14);
  Delay_asm(569);		//�������� �� 40 ���
  
  SendData(16); 
  Delay_asm(569);		//�������� �� 40 ���
  
  SendData(22);
  Delay_asm(569);		//�������� �� 40 ���
  
  SendData(17);
  Delay_asm(569);		//�������� �� 40 ���
  
  SendData(17);
  Delay_asm(569);		//�������� �� 40 ���
  
  SendData(14);
  Delay_asm(569);		//�������� �� 40 ���
  
  SendData(0);  
  Delay_asm(569);		//�������� �� 40 ���
}
