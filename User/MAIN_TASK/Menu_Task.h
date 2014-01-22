
// MTask 

#ifdef	MTask
  #define GLB_MTask 			
#else	
  #define GLB_MTask extern
#endif

  GLB_MTask void vMTask(void *pvParameters);  


#ifdef MTask

  //��������� ��������� �������, ����� ��������� ��� extern � �������� ���� 
  void lcd_write( char *pStr, char x, char y );
  void shift_blink( char x, char y );
  void blink_control( int isOn );
  void Off_DAC_Channel( char ChNum );
  void meas_control( char is_on );
  void sd_control( void );
  void sweep_freq_control( int cur_channel );
  
  void ChangeChar( char *pStr );

#endif
