
//обьявление типов сообщений

//структура сообщения к потоку LCD_Task
typedef struct 
{
  //ID команды
  // 0 - SendString(pData, param_1, param_2)
  // 1 - ShiftBlink(param_1, param_2)
  // 2 - ControlBlink(param_1, param_2, param_3)
  char ID_cmd;  
  
  //указатель на, динамически выделенную память,
  //в которой лежат данные к отображению
  //после отображения по этому указателю память должна быть освобождена
  char *pData;
  
  char param_1;
  char param_2;
  char param_3;
}sLCDParam;

//структура сообщения к потоку Osc_Task
typedef struct 
{
  char Ch_num;     // 0 - Channel 1, 1 - Channel 2
  char Sig_Type;   // 0 - sin, 1 - pulse, 2 - inv_pulse, 3 - triangl, 4 - gaus noise,
                   // 5 - uniform noise, 6 - zero("switch off" generator), 7 - DMA_Interrupt 
  float amp;       //для  Sig_Type = 4 и 5 - СКО 
  float offset;    //для  Sig_Type = 4 и 5 - математическое ожидание 
  float duty;
  float freq; 
}sOscParam;

typedef enum sd_type { SD_STOP = 0, SD_START, SD_WRITE } sd_type_t;

//структура сообщения к потоку SD_Task
typedef struct S_Sd_Param 
{
  sd_type_t type;       // operation type
  void* data;           // data to write
  unsigned int num;     // size in bytes !!!
  unsigned long time;   // time to write
}S_Sd_Param_t;