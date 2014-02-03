
//������� ���������

#ifdef PBDrv
  #define GLB_PBDrv 		
#else	
  #define GLB_PBDrv extern
#endif

  #define ButtonOn 1
  #define ButtonPress 2
  #define TWO_BUTTON_PRESSED 3

  typedef struct 
  {
    int typeEvent;  //��� �������(������(ButtonOn), ������(ButtonPress), TWO_BUTTON_PRESSED)
    int num;        //����� ������(0-7 ��� 0-11, � ����������� �� ���� ����������) 
    int prev_but_num; // number of previosly pushed buttons
  }sKeyMesg;
      
  // ������������� ������ ������ ������
  GLB_PBDrv void InitPad_Task(void);
  
  //����� KeyPad ����������
  // ���������� ���������� � ���� ������� � ������ ������
  // ���� ������� ����, �� typeEvent = -1

  // polls all buttons and fills temp
  // return 0, if no new events were occured, otherwise: 1
  GLB_PBDrv int get_key_pad_state( sKeyMesg* temp );
    
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

    // monitors i/o lines, with connected buttons, and fills ButtonDescr_K[].Sst values
    void get_sst_key( void );
    
    // State Machine ���  ������
    int Key_State_machine(sButtonDescr *ButtonDescr);
  
    // ������ �������� sButtonDescr ��� 8 ������ (KeyPad)
    sButtonDescr ButtonDescr_K[8];
    
    GPIO_InitTypeDef          GPIO_InitStruc_Pad;
#endif
