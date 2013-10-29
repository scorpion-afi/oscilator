
//������� ���������

#ifdef PBDrv
  #define GLB_PBDrv 		
#else	
  #define GLB_PBDrv extern
#endif

  #define ButtonOn 1
  #define ButtonPress 2

  typedef struct 
  {
    int typeEvent;  //��� �������(������(ButtonOn), ������(ButtonPress))
    int num;        //����� ������(0-7 ��� 0-11, � ����������� �� ���� ����������)    
  }sKeyMesg;
      
  // ������������� ������ ������ ������
  GLB_PBDrv void InitPad_Task(void);
  
  //����� KeyPad ����������
  // ���������� ���������� � ���� ������� � ������ ������
  // ���� ������� ����, �� typeEvent = -1
  GLB_PBDrv sKeyMesg GetKeyPadState(void);
    
#ifdef PBDrv
  
    #include "stm32f10x.h"
  
    // ���������, �����������  ������
    typedef struct 
    {
      int Sst;                // ������(�� ������) /�� ������(������) (1/0)
      int cnt;	              // ������� 
      int presscnt;           // ��� ���� �������
      int state;	      // ��������� ������
      uint16_t ID;            // ��� ������   GPIO_Pin0...15
      GPIO_TypeDef* PortName; // ��� ����� GPIOA...C
    }sButtonDescr;
    
    //��������� ������ �/� ��� ���������
    void initGPIO_Pads(void);
    
    // ����� �������/������� ������ (return: 1 - ������ ������ ���� ������)
    int GetSst_Key(void);
    
    // State Machine ���  ������
    int Key_State_machine(sButtonDescr *ButtonDescr);
  
    // ������ �������� sButtonDescr ��� 8 ������ (KeyPad)
    sButtonDescr ButtonDescr_K[8];
    
    GPIO_InitTypeDef          GPIO_InitStruc_Pad;
#endif