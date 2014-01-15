//драйвер генерации сигналов

#define OscDrv	
#include "Osc_Drv.h"

#include "string.h"  //для memcpy
#include "math.h"    //для sin

#define pi           3.14159265358979323846
#define Nph_reg      4294967296        // 2^32-1 Разрядность регистра, хранящего шаг фазы.
#define Fsamples     100000
//#define Lev_per_Volt 409.6   // 4096.0 / 10.0
//#define Lev_per_Volt 819   // 4096.0 / 10.0
#define Lev_per_Volt 407   // 4096.0/5.0

#define Duty_step  42949673        // 2^32 / 100 + 0.5
#define Freq_koef  Nph_reg/Fsamples
#define Phase_koef Nph_reg/(2*pi)

//extern void sound(int T, int num);

// Начальная настройка аппаратуры и переменных ответственных за генерацию 
//==============================================================================
void InitDAC_TIM_DMA(void)
{
  pDSPFunction[0] = CalcSin;
  pDSPFunction[1] = CalcPulse;
  pDSPFunction[2] = CalcExp;
  pDSPFunction[3] = CalcSawtooth;
  pDSPFunction[4] = CalcGaus;
  pDSPFunction[5] = CalcUniform;
  pDSPFunction[6] = CalcZero;

//-----------------------------------------------------------------------------
  // тип сигналов - ноль ("эффект" выключенного генератора)
  
  //эмулируем приход сообщения от M_Thread
  
  //заполняем первую половину DAC_Buff (DAC Channel 1)
  CurOscParam[0].Ch_num = 0;
  CurOscParam[0].Sig_Type = 6;
  pDAC_Buff = DAC_Buff;
  ReCalc(&(CurOscParam[0]));
  
  //заполняем вторую половину DAC_Buff (DAC Channel 1)
  pDAC_Buff = DAC_Buff + size_of_DAC_Buff/2;
  ReCalc(&(CurOscParam[0]));
  
  //заполняем первую половину DAC_Buff (DAC Channel 2)
  CurOscParam[1].Ch_num = 1;
  CurOscParam[1].Sig_Type = 6;
  pDAC_Buff = DAC_Buff;
  ReCalc(&(CurOscParam[1]));
  
  //заполняем вторую половину DAC_Buff (DAC Channel 2)
  pDAC_Buff = DAC_Buff + size_of_DAC_Buff/2;
  ReCalc(&(CurOscParam[1]));
//-----------------------------------------------------------------------------
  
  //старт можно расматривать как переход из второй половины на первую 
  CurFillArea = 1;
  pDAC_Buff = DAC_Buff + size_of_DAC_Buff/2;
    
//Настройка аппаратуры
  
 //DAC init------------------------------
  
  //DAC interface clock enable
  RCC->APB1ENR |= 1<<29;
  
  //DAC channel2 trigger selection - Timer 4 TRGO event, DAC channel2 trigger enable, DAC channel2 enable
  //DAC channel1 DMA mode enabled, DAC channel1 trigger selection - Timer 4 TRGO event, DAC channel1 trigger enable, DAC channel1 enable
  DAC->CR = 0x2D102D;

 //DMA init------------------------------
  
  //DMA2 clock enable
  RCC->AHBENR |= 2;
  
  //сбрасываем флаг прерывания global interrupt 3 канала DMA 2
  DMA2->IFCR = (1<<8);//DMA2_IT_GL3;
 
  //Memory size - 32 bits, Peripheral size - 32 bits, Memory increment mode enabled, 
  //Circular mode enabled, Read from memory, All interrupts enable
  //  Channel disabled
  DMA2_Channel3->CCR = 0xABE;
  DMA2_Channel3->CNDTR = size_of_DAC_Buff;
  DMA2_Channel3->CPAR = 0x40007420;           // DAC_DHR12RD
  DMA2_Channel3->CMAR = (uint32_t)DAC_Buff; // вот это да
  
  //Настройка NVIC для DMA IRQ 
  NVIC_InitStruct.NVIC_IRQChannel = DMA2_Channel3_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 13;  // 13 or 1100 1111 > configMAX_SYSCALL_INTERRUPT_PRIORITY
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct); 
  
  //  Channel enabled 
  DMA2_Channel3->CCR |= 1;

 //TIM 4 init------------------------------
  
  // PCLK1 = 36 МГц
  // F_interrupt = PCLK1*2 / (TIM_Prescaler + 1) / TIM_Period;

  // Enable TIM4 clocks
  RCC->APB1ENR |= 4;
  
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;            
  TIM_TimeBaseInitStruct.TIM_Period = 720-1;//1440;     // прерывания 100 000 раз в секунду(720)
  TIM_TimeBaseInitStruct.TIM_Prescaler = 0; 
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
  
  //выбираем в качестве источника внешнего тригера(TRGO) update event
  TIM_SelectOutputTrigger(TIM4, TIM_TRGOSource_Update); 
  
  // TIM4 enable counter
  TIM_Cmd(TIM4, ENABLE);
  
  // Disable TIM4 Update interrupt
  TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE);
 
  //после этого связка TIM4-DAC-DMA2 работает непрерывно, даже когда выбран тип сигнала "switch off"
}

// функция меняет текущюю заполняемую область циклического буфера DAC_Buff (CurFillArea)
// изменяет указатель pDAC_Buff и заполняет текущюю заполняемую область
// циклического буфера DAC_Buff отсчетами текущих сигналов на обоих каналах
//==============================================================================
void ReFill(void)
{ 
  //делаем циклический буфер
  if(CurFillArea == 0)
  {
    CurFillArea = 1;
    pDAC_Buff = DAC_Buff + size_of_DAC_Buff/2;
  }
  else
  {
    CurFillArea = 0;
    pDAC_Buff = DAC_Buff;   
  }
  
  CurDAC_Ch = 0;
  
  //если нужно пересчитывать
  if(CurOscParam[CurDAC_Ch].Sig_Type != 6)  
    pDSPFunction[CurOscParam[CurDAC_Ch].Sig_Type](); //вызов функции, ответственной за вычисление отсчетов текущего сигнала текущего канала

  CurDAC_Ch = 1;
  
  //если нужно пересчитывать
  if(CurOscParam[CurDAC_Ch].Sig_Type != 6)  
    pDSPFunction[CurOscParam[CurDAC_Ch].Sig_Type](); //вызов функции, ответственной за вычисление отсчетов текущего сигнала текущего канала
}

// функция меняет текущий сигнал на канале x и/или его параметры 
// и заполнет текущую область циклического буфера DAC_Buff отсчетами
// текущего типа сигнала x-го канала (CurOscParam[x])
// только для одного канала !!!
// x - извлекается по pOscParam
//==============================================================================
void ReCalc(const sOscParam *pOscParam)
{
  //меняем, если нужно, канал ЦАПа, с которым будем работать 
  // "приказ" от меню 
  CurDAC_Ch = pOscParam->Ch_num;
  
  //изменяем текущий сигнал на канале[CurDAC_Ch] и/или его параметры 
  memcpy(&(CurOscParam[CurDAC_Ch]), pOscParam, sizeof(sOscParam));
  
  //сбрасываем флаг первого входа для текущего канала для текущего типа сигнала
  //будем пересчитывать "модельку" сигнала
  first_entry[CurDAC_Ch] = 0;
   
  //вызов функции, ответственной за вычисление отсчетов сигнала текущего канала
  pDSPFunction[CurOscParam[CurDAC_Ch].Sig_Type]();    
}


//=================================================================================
//=================================================================================
//=================================================================================

  
// вычисление отсчетов синуса
//==============================================================================
void CalcSin(void)
{ 
  float dPhase, value;  
  uint32_t SUM, temp, *ptemp;  
  uint16_t Ampl;
  static uint16_t Offset;
  
  ptemp = pDAC_Buff; 
   
  // Создаем "модельку" четверти периода sin-уса для текущего канала,
  // с учетом амплитуды и смещения
  if(!first_entry[CurDAC_Ch])
  {  
    first_entry[CurDAC_Ch] = 1;
    
    dPhase = 2*pi/(8192);  // а не (8192 - 1)
    
    Ampl = (uint16_t)(CurOscParam[CurDAC_Ch].amp*Lev_per_Volt + 0.5);
    Offset = (uint16_t)(CurOscParam[CurDAC_Ch].offset*Lev_per_Volt + 0.5); 
    
    //программный "дисторшн" (от 0 до 4095 (уровни ЦАПа))
    for(int i = 0; i < 2049 /*sizeof(RAMBuff[CurDAC_Ch])*/; i++)
    {
      value = Ampl*sin(i*dPhase) + Offset;
      
      if(value >= 4095.0)
       RAMBuff[CurDAC_Ch][i] = 4095;
      else if(value <= 0.0)
       RAMBuff[CurDAC_Ch][i] = 0;
      else
       RAMBuff[CurDAC_Ch][i] = (uint16_t)(value + 0.5);         
    }
    
    AcmPhase[CurDAC_Ch] = (uint32_t)(0/*phase*/*Phase_koef + 0.5);   // phase: 0 - 2pi
   
    FREQ_REG[CurDAC_Ch] = (uint32_t)(CurOscParam[CurDAC_Ch].freq*Freq_koef + 0.5); 
  }
  
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 
  //вычисление отсчетов синуса(будут положены в DAC_Buff)
  for(int i = 0; i < size_of_DAC_Buff/2; i++)
  {
    //аккумулятор фазы 13 битный 
    SUM = AcmPhase[CurDAC_Ch] >> 19;
    
    //первая четверть
    if(SUM < 2049)
    {
      if(CurDAC_Ch == 0)
      {
        *ptemp &= 0xFFFF0000;
        *ptemp++ |= (uint32_t)(RAMBuff[CurDAC_Ch][SUM]);
      }
      else
      {
        *ptemp &= 0x0000FFFF;       
        *ptemp++ |= (uint32_t)(RAMBuff[CurDAC_Ch][SUM]<<16); 
      }
    }
    
    //вторая четверть
    else if( (SUM > 2048) && (SUM < 4097) )
    {
      SUM = 4096 - SUM;  //2048 - (SUM - 2048);
     
      if(CurDAC_Ch == 0)
      {
        *ptemp &= 0xFFFF0000;        
        *ptemp++ |= (uint32_t)(RAMBuff[CurDAC_Ch][SUM]);
      }
      else 
      {
        *ptemp &= 0x0000FFFF;               
        *ptemp++ |= (uint32_t)(RAMBuff[CurDAC_Ch][SUM]<<16);
      }
    }
    
    //третья четверть    
    else if( (SUM > 4096) && (SUM < 6145) )
    {
      SUM -= 4096;
      
      temp = 2*Offset - RAMBuff[CurDAC_Ch][SUM]; // 2048 - (RAMBuff[CurDAC_Ch][SUM] - 2047);
      //temp = 2048 - (RAMBuff[CurDAC_Ch][SUM] - 2047);
        
      if(CurDAC_Ch == 0)
      {
        *ptemp &= 0xFFFF0000;          
        *ptemp++ |= temp;
      }
      else
      {
        *ptemp &= 0x0000FFFF;                       
        *ptemp++ |= (temp<<16);
      }
    }
    
    //четвертая четверть    
    else if(SUM > 6144)
    {
      SUM = 8192 - SUM; //2048 - (SUM - 6144);
      
      temp = 2*Offset - RAMBuff[CurDAC_Ch][SUM]; // 2048 - (RAMBuff[CurDAC_Ch][SUM] - 2047);
      //temp = 2048 - (RAMBuff[CurDAC_Ch][SUM] - 2047);
     
      if(CurDAC_Ch == 0)
      {
        *ptemp &= 0xFFFF0000;                  
        *ptemp++ |= temp;
      }
      else
      {
        *ptemp &= 0x0000FFFF;                               
        *ptemp++ |= (temp<<16);
      }
    }
 /*   	
    //switch по квадрантам(четвертям) синуса
    switch(AcmPhase[CurDAC_Ch]>>30)
    {
      case 1: // первая четверть
      {
        if(CurDAC_Ch == 0)
          *pDAC_Buff++ |= RAMBuff[CurDAC_Ch][SUM];
        else  
          *pDAC_Buff++ |= RAMBuff[CurDAC_Ch][SUM]<<16;        
      }
      break;
      
      case 2: // вторая четверть
      {
        SUM = 4096 - SUM;  //2048 - (SUM - 2048);
       
        if(CurDAC_Ch == 0)
          *pDAC_Buff++ |= RAMBuff[CurDAC_Ch][SUM];
        else  
          *pDAC_Buff++ |= RAMBuff[CurDAC_Ch][SUM]<<16;               
      }
      break;
      
      case 3: // третья четверть
      {
        SUM -= 4096;
        
        temp = 2048 - (RAMBuff[CurDAC_Ch][SUM] - 2047);
          
        if(CurDAC_Ch == 0)
          *pDAC_Buff++ |= temp;
        else  
          *pDAC_Buff++ |= temp<<16;                
      }
      break;
      
      case 4: // четвертая четверть
      {
        SUM = 8192 - SUM; //2048 - (SUM - 6144);
        
        temp = 2048 - (RAMBuff[CurDAC_Ch][SUM] - 2047);
       
        if(CurDAC_Ch == 0)
          *pDAC_Buff++ |= temp;
        else  
          *pDAC_Buff++ |= temp<<16;                
      }
      break;     
    };
  */  
    // увеличиваем значение мгновенной фазы на величину, соответствующюю частоте,
    // то есть скорости изменения мгновенной фазы
    AcmPhase[CurDAC_Ch] = (uint32_t)(AcmPhase[CurDAC_Ch] + FREQ_REG[CurDAC_Ch]);
  }//вычисление отсчетов синуса(будут положены в DAC_Buff)
}

// вычисление отсчетов pulse
//==============================================================================
void CalcPulse(void)
{ 
  uint32_t SUM, *ptemp;
  uint16_t Ampl, Offset, value;
  int16_t sign_value;
  
  //номера точек pulse сигналов(двух каналов), которые УЖЕ равны 0(или 0 + Offset)
  static uint32_t duty_koef[2];
    
  ptemp = pDAC_Buff; 
   
  // Создаем "модельку"  pulse для текущего канала, с учетом амплитуды и смещения
  if(!first_entry[CurDAC_Ch])
  {  
    first_entry[CurDAC_Ch] = 1;
       
    Ampl = (uint16_t)(CurOscParam[CurDAC_Ch].amp*Lev_per_Volt + 0.5);
    Offset = (uint16_t)(CurOscParam[CurDAC_Ch].offset*Lev_per_Volt + 0.5);
    duty_koef[CurDAC_Ch] = (uint32_t)(CurOscParam[CurDAC_Ch].duty * Duty_step + 0.5);
   
    //RAMBuff[CurDAC_Ch][0] хранит значение "пика" pulse сигнала
    value = Ampl + Offset;
    if(value > 4095)
     RAMBuff[CurDAC_Ch][0] = 4095;  
    else
     RAMBuff[CurDAC_Ch][0] = value;
    
    //RAMBuff[CurDAC_Ch][1] хранит значение "впадины" pulse сигнала    
    sign_value = Offset - Ampl;
    if(sign_value < 0)
     RAMBuff[CurDAC_Ch][1] = 0;
    else
     RAMBuff[CurDAC_Ch][1] = sign_value;
               
    AcmPhase[CurDAC_Ch] = (uint32_t)(0/*phase*/*Phase_koef + 0.5);   // phase: 0 - 2pi
    
    FREQ_REG[CurDAC_Ch] = (uint32_t)(CurOscParam[CurDAC_Ch].freq*Freq_koef + 0.5);     
  }
  
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 
  //вычисление отсчетов pulse (будут положены в DAC_Buff)
  for(int i = 0; i < size_of_DAC_Buff/2; i++)
  {
    //аккумулятор фазы 32 битный
    SUM = AcmPhase[CurDAC_Ch];
       
    if(SUM < duty_koef[CurDAC_Ch])  
    {
      if(CurDAC_Ch == 0)
      {
        *ptemp &= 0xFFFF0000;                  
        *ptemp++ |= (uint32_t)(RAMBuff[CurDAC_Ch][0]);
      }
      else
      {
        *ptemp &= 0x0000FFFF;                               
        *ptemp++ |= (uint32_t)(RAMBuff[CurDAC_Ch][0]<<16);
      }
    }
    else
    {
      if(CurDAC_Ch == 0)
      {
        *ptemp &= 0xFFFF0000;                  
        *ptemp++ |= (uint32_t)(RAMBuff[CurDAC_Ch][1]);
      }
      else
      {
        *ptemp &= 0x0000FFFF;                               
        *ptemp++ |= (uint32_t)(RAMBuff[CurDAC_Ch][1]<<16);
      }     
    }
         
    // увеличиваем значение мгновенной фазы на величину, соответствующюю частоте,
    // то есть скорости изменения мгновенной фазы
    AcmPhase[CurDAC_Ch] = (uint32_t)(AcmPhase[CurDAC_Ch] + FREQ_REG[CurDAC_Ch]);    
  }//вычисление отсчетов pulse (будут положены в DAC_Buff) 
}

// вычисление отсчетов y = exp(x) 
//==============================================================================
void CalcExp(void)
{
  float dPhase = 0.0f;
  float value = 0.0f; 
  float rc_time = 0.0f;
  
  uint32_t SUM = 0;
  uint32_t* ptemp = NULL;
  uint16_t Ampl = 0;
  uint16_t Offset = 0; 
           
  // Создаем "модельку" одного периода y = exp(x) для текущего канала,
  // с учетом амплитуды, смещения и постоянной rc
  if( !first_entry[CurDAC_Ch] )
  {  
    first_entry[CurDAC_Ch] = 1;
    
    dPhase = 2*pi/(2048);  // NOT 2048 - 1
    
    Ampl = (uint16_t)(CurOscParam[CurDAC_Ch].amp*Lev_per_Volt + 0.5);
    //Offset = (uint16_t)( (CurOscParam[CurDAC_Ch].offset + 5.0)*409.5 + 0.5);
    Offset = (uint16_t)( CurOscParam[CurDAC_Ch].offset*Lev_per_Volt + 0.5);
 
    rc_time = 2.0f;     
    //rc_time = ( ( CurOscParam[CurDAC_Ch].rc_time + ? ) * ? + ?);

    //программный "дисторшн" (от 0 до 4095 (уровни ЦАПа))
    for(int i = 0; i < 2048; i++)
    {
      value = Ampl*exp(-i*dPhase/rc_time) + Offset;
      
      if( value >= 4095.0 )
      {
        RAMBuff[CurDAC_Ch][i] = 4095;
      }
      else if( value <= 0.0 )
      {
        RAMBuff[CurDAC_Ch][i] = 0;
      }
      else
      {
        RAMBuff[CurDAC_Ch][i] = (uint16_t)( value + 0.5 );
      }
    }
    
    AcmPhase[CurDAC_Ch] = (uint32_t)(0/*phase*/*Phase_koef + 0.5);   // phase: 0 - 2pi
   
    FREQ_REG[CurDAC_Ch] = (uint32_t)(CurOscParam[CurDAC_Ch].freq*Freq_koef + 0.5);    
  }
  
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 
  ptemp = pDAC_Buff; 
  
  // RAMBuff --> DAC_Buff
  //вычисление отсчетов exp (будут положены в DAC_Buff)
  for( int i = 0; i < size_of_DAC_Buff/2; i++ )
  {
    //аккумулятор фазы 11 битный
    SUM = AcmPhase[CurDAC_Ch] >> 21;
       
    if( CurDAC_Ch == 0 )
    {
      *ptemp &= 0xFFFF0000;                  
      *ptemp++ |= (uint32_t)(RAMBuff[CurDAC_Ch][SUM]);
    }
    else
    {
      *ptemp &= 0x0000FFFF;                               
      *ptemp++ |= (uint32_t)(RAMBuff[CurDAC_Ch][SUM] << 16);       
    }
        
    // увеличиваем значение мгновенной фазы на величину, соответствующюю частоте,
    // то есть скорости изменения мгновенной фазы
    AcmPhase[CurDAC_Ch] = (uint32_t)( AcmPhase[CurDAC_Ch] + FREQ_REG[CurDAC_Ch] );    
    
  }//вычисление отсчетов exp (будут положены в DAC_Buff)   
}

// вычисление отсчетов Sawtooth
//==============================================================================
void CalcSawtooth(void)
{
  uint32_t SUM, *ptemp;
  uint16_t Ampl, Offset, value;
  
  // y = x*k + b
  // y - уровень ЦАПа
  // x - 0...2047
  // k  - Ampl/(4095) 
  // b - Offset
  float k;
  //static float k;
  //static uint16_t Offset;
    
  ptemp = pDAC_Buff; 
   
  // Создаем "модельку"  triangl для текущего канала, с учетом амплитуды и смещения
  if(!first_entry[CurDAC_Ch])
  {  
    first_entry[CurDAC_Ch] = 1;
       
    Ampl = (uint16_t)(CurOscParam[CurDAC_Ch].amp*Lev_per_Volt + 0.5);
    Offset = (uint16_t)(CurOscParam[CurDAC_Ch].offset*Lev_per_Volt + 0.5);
    k = Ampl/2047.0;
    
    //программный "дисторшн" + заполнение таблицы triangl сигнала
    for(int i = 0; i < 2048; i++)
    {
      value = (uint16_t)(i*k + Offset + 0.5);
      if(value > 4095)       
       RAMBuff[CurDAC_Ch][i] = 4095;
      else
       RAMBuff[CurDAC_Ch][i] = value;       
    }
                   
    AcmPhase[CurDAC_Ch] = (uint32_t)(0/*phase*/*Phase_koef + 0.5);   // phase: 0 - 2pi
    
    FREQ_REG[CurDAC_Ch] = (uint32_t)(CurOscParam[CurDAC_Ch].freq*Freq_koef + 0.5);     
  }
  
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 
  //вычисление отсчетов triangl (будут положены в DAC_Buff)
  for(int i = 0; i < size_of_DAC_Buff/2; i++)
  {
    //аккумулятор фазы 11 битный
    SUM = AcmPhase[CurDAC_Ch] >> 21;
       
    if(CurDAC_Ch == 0)
    {
      *ptemp &= 0xFFFF0000;                  
      //*ptemp++ |= (uint32_t)(SUM * k + Offset);
      *ptemp++ |= (uint32_t)(RAMBuff[CurDAC_Ch][SUM]);
    }
    else
    {
      *ptemp &= 0x0000FFFF;                               
      //*ptemp++ |= ((uint32_t)(SUM * k + Offset)) << 16;
      *ptemp++ |= (uint32_t)(RAMBuff[CurDAC_Ch][SUM] << 16);     
    }
        
    // увеличиваем значение мгновенной фазы на величину, соответствующюю частоте,
    // то есть скорости изменения мгновенной фазы
    AcmPhase[CurDAC_Ch] = (uint32_t)(AcmPhase[CurDAC_Ch] + FREQ_REG[CurDAC_Ch]);    
  }//вычисление отсчетов triangl (будут положены в DAC_Buff)    
}

// 16 * 4095 < 2 ^ 16
#define UNIFORM_SET 16  // do not increase this value

// вычисление отсчетов шума с нормальным законом распределения
//==============================================================================
void CalcGaus(void)
{
  uint32_t *ptemp, temp;
  static uint16_t Offset[2];
  
  // R[] - stores value of LSFR for two channel
  static uint16_t R[2];
    
  ptemp = pDAC_Buff;
  
  // Вычисляем добавки за счет амплитуды и смещения ("создаем модельку Gaus signal")
  if(!first_entry[CurDAC_Ch])
  {  
    first_entry[CurDAC_Ch] = 1;
       
    Offset[CurDAC_Ch] = (uint16_t)(CurOscParam[CurDAC_Ch].offset*Lev_per_Volt + 0.5);
    
    R[CurDAC_Ch] = 1;   //anti-lock mechanism  
  }
  
  //вычисление отсчетов шума(будут положены в DAC_Buff)
  for(int i = 0; i < size_of_DAC_Buff/2; i++)
  {   
    temp = 0;
      
    // temp can not be greater then 2 ^ 16
    for( int i = 0; i < UNIFORM_SET; i++ )
    {
      // random number in range: 0 - 4095 (uniform distribution)
      R[CurDAC_Ch] = ( ( ( (R[CurDAC_Ch]>>6) ^ (R[CurDAC_Ch]>>4) ^ (R[CurDAC_Ch]>>1) ^ R[CurDAC_Ch] ) & 1 ) << 11 ) | (R[CurDAC_Ch] >> 1);
           
      temp += R[CurDAC_Ch]; 
      
      if( R[CurDAC_Ch] == 0 ) R[CurDAC_Ch] = 1; //anti-lock mechanism     
    }
    
    // take 12 most significant bits
    temp = ( temp >> 4 ) & 0xFFF;
    
    temp += Offset[CurDAC_Ch];
    
    if(temp > 4095) temp = 4095;
    
    if(CurDAC_Ch == 0)
    {
      *ptemp &= 0xFFFF0000;                  
      *ptemp++ |= (uint32_t)(temp);
    }
    else
    {
      *ptemp &= 0x0000FFFF;                               
      *ptemp++ |= (uint32_t)(temp << 16);     
    }     
  }      
}

// вычисление отсчетов шума с равномерным законом распределения
//==============================================================================
void CalcUniform(void)
{
  uint32_t *ptemp, temp;
  static uint16_t Offset[2];
  
  //R[] - хранит сдержимое регистров сдвига(LSFR) для двух каналов
  static uint16_t R[2];
    
  ptemp = pDAC_Buff;
  
  // Вычисляем добавки за счет амплитуды и смещения ("создаем модельку Gaus signal")
  if(!first_entry[CurDAC_Ch])
  {  
    first_entry[CurDAC_Ch] = 1;
   
    Offset[CurDAC_Ch] = (uint16_t)(CurOscParam[CurDAC_Ch].offset*Lev_per_Volt + 0.5);
                  
    R[CurDAC_Ch] = 1;   //anti-lock mechanism
  }
  
  //вычисление отсчетов шума(будут положены в DAC_Buff)
  for(int i = 0; i < size_of_DAC_Buff/2; i++)
  {
    // random number in range: 0 - 4095  (uniform distribution)
    R[CurDAC_Ch] = ( ( ( (R[CurDAC_Ch]>>6) ^ (R[CurDAC_Ch]>>4) ^ (R[CurDAC_Ch]>>1) ^ R[CurDAC_Ch] ) & 1 ) << 11 ) | (R[CurDAC_Ch] >> 1);
    
    temp = R[CurDAC_Ch] + Offset[CurDAC_Ch];
    if(temp > 4095) temp = 4095;
    
    if(CurDAC_Ch == 0)
    {
      *ptemp &= 0xFFFF0000;                  
      *ptemp++ |= (uint32_t)(temp);
    }
    else
    {
      *ptemp &= 0x0000FFFF;                               
      *ptemp++ |= (uint32_t)(temp << 16);     
    }
    
    if(R[CurDAC_Ch] == 0) R[CurDAC_Ch] = 1; //anti-lock mechanism    
  }    
}

// вычисление отсчетов сигнала zero 
//==============================================================================
void CalcZero(void)
{
  uint32_t *temp;
  
  temp = pDAC_Buff;
  
  if(temp == NULL) return;
  
  if(CurDAC_Ch == 0)//DAC Channel 1
  { 
    for(int i = 0; i < size_of_DAC_Buff/2; i++)
      *temp++ |= (uint32_t)0x0;   // 3,3 V   
  }
  else
  {
    for(int i = 0; i < size_of_DAC_Buff/2; i++)
      *temp++ |= (uint32_t)(0x0<<16);   // 3,3 V     
  }
}