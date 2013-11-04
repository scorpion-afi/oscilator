//драйвер генерации сигналов

#ifdef OscDrv
  #define GLB_OscDrv 		
#else	
  #define GLB_OscDrv extern
#endif

  #include "InterDefines.h"   //определение sOscParam

  GLB_OscDrv void InitDAC_TIM_DMA(void);
  GLB_OscDrv void ReFill(void);
  GLB_OscDrv void ReCalc(const sOscParam *pOscParam);
  
#ifdef OscDrv
  
  #include "stm32f10x.h"
  
  //размер буфера канала DAC  (в item-ах DMA)
  #define size_of_DAC_Buff 1024
  
  // циклический буфер DAC_Buff, хранящий отсчеты для двух каналов
  // в старших 16 битах - отсчеты для DAC Channel 2, в младших - DAC Channel 1 
  uint32_t DAC_Buff[size_of_DAC_Buff]; //4 Кбайт
  
  //указатель на текущюю заполняемую область циклического буфера DAC_Buff
  uint32_t *pDAC_Buff; 
  
  //текущий канал ЦАП
  // 0 - Channel 1, 1 - Channel 2
  char CurDAC_Ch;
  
  // характеристика сигналов на каналах DAC
  // 0 - Channel 1, 1 - Channel 2 
  sOscParam CurOscParam[2]; 
  
  //индекс текущей заполняемой(НЕ "выбрасываемой" в DAC) области циклического буфера DAC_Buff
  // 0 - первая половина, 1 - вторая половина циклического буфера DAC_Buff
  char CurFillArea; 
  
  //массив указателей на функции вычисления отсчетов 7 типов сигналов
  // 0 - sin, 1 - pulse, 2 - inv_pulse, 3 - triangl, 4 - gaus noise , 5 - uniform noise, 6 - zero("switch off" generator)
  void (*pDSPFunction[7])(void);
  
  //++++++++++++++++++++++++++ Для DDS +++++++++++++++++++++++++++++++++++++
  
  //массив, показывающий, первый ли это вызов соответсствующей функции pDSPFunction
  // 0 - значит первый
  // 0 - Channel 1, 1 - Channel 2     
  char first_entry[2];
  
  //массивы, хранящие отсчеты различных типов сигналов для двух каналов
  uint16_t RAMBuff[2][2049];  //!!!! 2049

  //массив хранящий текущее значение мгновенной фазы для обоих каналов
  // 0 - Channel 1, 1 - Channel 2   
  uint32_t AcmPhase[2];
  
  //массив хранящий шаг по фазе для двух каналов(то есть частоту)
  // 0 - Channel 1, 1 - Channel 2     
  uint32_t FREQ_REG[2];


    
  TIM_TimeBaseInitTypeDef   TIM_TimeBaseInitStruct;
  NVIC_InitTypeDef          NVIC_InitStruct;
  
  void CalcSin(void);  
  void CalcPulse(void);
  void CalcExp(void);
  void CalcSawtooth(void);
  void CalcGaus(void);
  void CalcUniform(void);
  void CalcZero(void);
   
#endif