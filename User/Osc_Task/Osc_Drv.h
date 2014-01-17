//������� ��������� ��������

#ifdef OscDrv
  #define GLB_OscDrv 		
#else	
  #define GLB_OscDrv extern
#endif

  #include "InterDefines.h"   //����������� sOscParam

  GLB_OscDrv void InitDAC_TIM_DMA(void);
  GLB_OscDrv void ReFill(void);
  GLB_OscDrv void ReCalc(const sOscParam *pOscParam);
  
#ifdef OscDrv
  
  #include "stm32f10x.h"
  
  //������ ������ ������ DAC  (� item-�� DMA)
  #define size_of_DAC_Buff 1024
  
  // ����������� ����� DAC_Buff, �������� ������� ��� ���� �������
  // � ������� 16 ����� - ������� ��� DAC Channel 2, � ������� - DAC Channel 1 
  uint32_t DAC_Buff[size_of_DAC_Buff]; //4 �����
  
  //��������� �� ������� ����������� ������� ������������ ������ DAC_Buff
  uint32_t *pDAC_Buff; 
  
  //������� ����� ���
  // 0 - Channel 1, 1 - Channel 2
  char CurDAC_Ch;
  
  // �������������� �������� �� ������� DAC
  // 0 - Channel 1, 1 - Channel 2 
  sOscParam CurOscParam[2]; 
  
  //������ ������� �����������(�� "�������������" � DAC) ������� ������������ ������ DAC_Buff
  // 0 - ������ ��������, 1 - ������ �������� ������������ ������ DAC_Buff
  char CurFillArea; 
  
  //������ ���������� �� ������� ���������� �������� 7 ����� ��������
  // 0 - sin, 1 - pulse, 2 - inv_pulse, 3 - triangl, 4 - gaus noise , 5 - uniform noise, 6 - zero("switch off" generator)
  void (*pDSPFunction[7])(void);
  
  //++++++++++++++++++++++++++ ��� DDS +++++++++++++++++++++++++++++++++++++
  
  //������, ������������, ������ �� ��� ����� ���������������� ������� pDSPFunction
  // 0 - ������ ������
  // 0 - Channel 1, 1 - Channel 2     
  char first_entry[2];
  
  //�������, �������� ������� ��������� ����� �������� ��� ���� �������
  uint16_t RAMBuff[2][2049];  //!!!! 2049

  //������ �������� ������� �������� ���������� ���� ��� ����� �������
  // 0 - Channel 1, 1 - Channel 2   
  uint32_t AcmPhase[2];
  
  //������ �������� ��� �� ���� ��� ���� �������(�� ���� �������)
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

  void sweep_control( void );
   
#endif
