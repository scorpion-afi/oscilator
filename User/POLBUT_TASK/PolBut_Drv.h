
//драйвер клавиатур

#ifdef PBDrv
  #define GLB_PBDrv 		
#else	
  #define GLB_PBDrv extern
#endif

  #define ButtonOn 1
  #define ButtonPress 2

  typedef struct 
  {
    int typeEvent;  //тип событи€(нажата(ButtonOn), зажата(ButtonPress))
    int num;        //номер кнопки(0-7 или 0-11, в зависимости от типа клавиатуры)    
  }sKeyMesg;
      
  // »нициализаци€ задачи опроса кнопок
  GLB_PBDrv void InitPad_Task(void);
  
  //опрос KeyPad клавиатуры
  // возвращ€ет информацию о типе событи€ и номере кнопки
  // если событи€ нету, то typeEvent = -1
  GLB_PBDrv sKeyMesg GetKeyPadState(void);
    
#ifdef PBDrv
  
    #include "stm32f10x.h"
  
    // структура, описывающ€€  кнопку
    typedef struct 
    {
      int Sst;                // нажата(не отжата) /не нажата(отжата) (1/0)
      int cnt;	              // счетчик 
      int presscnt;           // еще один счетчик
      int state;	      // состо€ние кнопки
      uint16_t ID;            // код кнопки   GPIO_Pin0...15
      GPIO_TypeDef* PortName; // им€ порта GPIOA...C
    }sButtonDescr;
    
    //настройка портов в/в дл€ клавиатур
    void initGPIO_Pads(void);
    
    // ќпрос нажати€/отжати€ кнопок (return: 1 - нажата только ќƒЌј кнопка)
    int GetSst_Key(void);
    
    // State Machine дл€  кнопок
    int Key_State_machine(sButtonDescr *ButtonDescr);
  
    // массив структур sButtonDescr дл€ 8 кнопок (KeyPad)
    sButtonDescr ButtonDescr_K[8];
    
    GPIO_InitTypeDef          GPIO_InitStruc_Pad;
#endif