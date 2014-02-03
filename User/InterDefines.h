
//���������� ����� ���������

// struct for communication between Polling button thread and Menu thread
typedef struct
{
  char EventType;
  char isKeyPad;
  char is_double_click; // is double click occured ?  1 - yes
  char num;			    // code of pressed button
  char prev_but_num;	// code of previosly pressed button
}s_pol_button;

//��������� ��������� � ������ LCD_Task
typedef struct 
{
  //ID �������
  // 0 - SendString(pData, param_1, param_2)
  // 1 - ShiftBlink(param_1, param_2)
  // 2 - ControlBlink(param_1, param_2, param_3)
  char ID_cmd;  
  
  //��������� ��, ����������� ���������� ������,
  //� ������� ����� ������ � �����������
  //����� ����������� �� ����� ��������� ������ ������ ���� �����������
  char *pData;
  
  char param_1;
  char param_2;
  char param_3;
}sLCDParam;

typedef enum Sig_Type_ { SIN = 0, PULSE, EXP, SAWTOO, GAUS, UNIFORM, ZERO, DMA, SWEEP_CONTROL } Sig_Type_t;

//��������� ��������� � ������ Osc_Task
typedef struct 
{
  char Ch_num;     // 0 - Channel 1, 1 - Channel 2
  char Sig_Type;   // 0 - sin, 1 - pulse, 2 - inv_pulse, 3 - triangl, 4 - gaus noise,
                   // 5 - uniform noise, 6 - zero("switch off" generator), 7 - DMA_Interrupt, 8 - for sweeping control
  float amp;       //���  Sig_Type = 4 � 5 - ��� 
  float offset;    //���  Sig_Type = 4 � 5 - �������������� �������� 
  float duty;
  float freq; 
  char is_freq_sweep_on; // is frequency sweeping allowed ?  1 - allowed, 0 - not allowed
}sOscParam;


typedef enum sd_type { SD_STOP = 0, SD_START, SD_WRITE, SD_EVENT } sd_type_t;

//��������� ��������� � ������ SD_Task
typedef struct S_Sd_Param 
{
  sd_type_t type;       // operation type
  void* data;           // data to write
  unsigned int num;     // size in bytes !!!
  unsigned long time;   // time to write
}S_Sd_Param_t;
