
// MTask 

#ifdef	MTask
  #define GLB_MTask 			
#else	
  #define GLB_MTask extern
#endif

  GLB_MTask void vMTask(void *pvParameters);  

#ifdef MTask

  //��������� ��������� �������, ����� ��������� ��� extern � �������� ���� 
  void lcd_write(char *pStr, char x, char y);
  void shift_blink(char x, char y);
  void blink_control(int isOn);
  void Off_DAC_Channel(char ChNum);
  void meas_control( char is_on );
  void sd_control( void );
  
  void ChangeChar(char *pStr);
  
  //���������� ���������� �� Mesg � ButNum, EventType � return
  // ���������� ��� menu state machine
  // ButNum - ����� ������
  // EventType - ��� �������  (1 - ButtonOn, 2 - ButtonPress) 
  // ���������� ��� ����������, �� ������� ������ ���������
  // (1 - KeyPad, 0 - MatrixPad) 
  char PrepareMesg(char Mesg, char *ButNum, char *EventType);
  
#endif