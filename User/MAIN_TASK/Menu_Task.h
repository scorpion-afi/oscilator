
// MTask 

#ifdef	MTask
  #define GLB_MTask 			
#else	
  #define GLB_MTask extern
#endif

  GLB_MTask void vMTask(void *pvParameters);  

#ifdef MTask

  //аппаратно зависимые функции, будут обь€влены как extern в драйвере меню 
  void lcd_write(char *pStr, char x, char y);
  void shift_blink(char x, char y);
  void blink_control(int isOn);
  void Off_DAC_Channel(char ChNum);
  void meas_control( char is_on );
  void sd_control( void );
  
  void ChangeChar(char *pStr);
  
  //извлечение информации из Mesg в ButNum, EventType и return
  // подстройка под menu state machine
  // ButNum - номер кнопки
  // EventType - тип событи€  (1 - ButtonOn, 2 - ButtonPress) 
  // возвращ€ет тип клавиатуры, от которой пришло сообщение
  // (1 - KeyPad, 0 - MatrixPad) 
  char PrepareMesg(char Mesg, char *ButNum, char *EventType);
  
#endif