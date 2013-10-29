
//������� LCD 

#ifdef	LCDDrv
  #define GLB_LCDDrv 			
#else	
  #define GLB_LCDDrv extern
#endif

  // ��������� ������������� LCD 
  GLB_LCDDrv void InitLCD(void);
  
  // ������� ������ ������ � �/� ����������
  //data - ��������� �� ������ �������� � ����� � �����
  //pos - ������� � ������ (0 -19)
  //nRow - ����� ������ ������(0 - 3)
  GLB_LCDDrv void SendString(char *data, char pos, char nRow);
  
  // ����� �������
  //pos - ������� � ������ (0 -19)
  //nRow - ����� ������ ������(0 - 3)
  GLB_LCDDrv void ShiftBlink(char pos, char nRow);
    
  //���������� ������� � ��������
  // d - 1: ����� �������
  // c - 1: ������ �������
  // b - 1: ������ ������  
  GLB_LCDDrv void ControlBlink(char d, char c, char b);
  
  // ������� ����� �������
  GLB_LCDDrv void ClearLCD(void);

#ifdef	LCDDrv
  
  #include "stm32f10x.h"
  
  #define RS_Pin             GPIO_Pin_12
  #define RW_Pin             GPIO_Pin_5
  
  #define RS_Clear__E_Set    GPIO_Pin_6
  #define RS_Set__E_Set      GPIO_Pin_12 | GPIO_Pin_6
  
  #define RS_Clear__E_Clear  0
  #define RS_Set__E_Clear    GPIO_Pin_12
  
  #define Function_set_4bit  0x2000   //��� �������� �� 8 � 4-������ �����
  #define Function_StartUp   0x3000   //��� ���������������� LCD
  
  // ��������� ������ �/�.
  void InitGPIO_LCD(void);

  // ���������� ��� ���������� ������(����� ���������, � �� ���������� ����� ������)
  void Preinit_LCD(void);
  
  //������� ������ ������ �� 8-�������(����� ������ �������)� 4-������
  void SendFirstCommand(void);
  
  // ������� ������ � �/� ����������
  void SendCommand(uint8_t command);
  
  // �������  ������(1 ����) � �/� ����������
  void SendData(uint8_t data);
  
  // �������� ������� ����� (���)
  void CreateSigmaPattern(void);
  
  // �������� � ���� ��������
  extern void Delay_asm(int period);
  
  GPIO_InitTypeDef          GPIO_InitStruct;
 
#endif