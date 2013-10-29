
// "драйвер" меню 

#ifdef MDrv
  #define GLB_MDrv				
#else	
  #define GLB_MDrv extern
#endif

  //Начальная картина на LCD
  GLB_MDrv void InitView( void );

  //menu state machine
  // ButNum - номер кнопки
  // EventType - тип события  (1 - ButtonOn, 2 - ButtonPress)
  // isKeyPad - тип клавиатуры(1 - KeyPad, 0 - MatrixPad)
  GLB_MDrv void MenuStateMach( char ButNum, char EventType, char isKeyPad );
         
  
  //структура, описывающяя параметр сигнала (A, O, F, D, m, var)
  // и приставку с еденицей измерения
  //пользуясь, только, этой структурой можно полностью сформировать строку типа "O = -0.3B"
  //хранит границы параметра и начальную позицию курсора для редактирования
  typedef struct  
  {
    float min;           //включительно
    float max;           //включительно
    float cur;
    float step;          // шаг изменения (для кнопок "+" и "-")
    char *pacronym;      // приставка (A = )
    char *punit;         // еденицы измерения (B)
    char bg_pos_x;       // начальная позиция курсора для редактирования
    char bg_pos_y;       // начальная позиция курсора для редактирования 
    char isVisible;      // нужно ли этот параметр отображать на LCD
    char *format;        // формат для sprintf
    char isAvailable;    // доступоность  параметра у сигнала
    char index_together; // индекс параметра, с которым делится строчка на LCD
    char isSign;         // 1 - параметр знаковый
    char numDig;         // max кол-во цифр(знакомест, без десятичной точки)
    char isFract;		   // дробный ли параметр
  }sParam;

  //структура, описывающяя сигнал, имя, позицию имени и его параметры
  typedef struct  
  {
    char *pname;      // имя сигнала с префиксом (SF:pulse)
    char name_pos_y;  // позиция для рисования имени   
    sParam Param[6];  // 0 - A, 1 - O, 2 - F, 3 - D, 4 - m, 5 - var
  }sSignalParam;
  
  //структура, описывающяя канал с его сигналами
  typedef struct  
  {	
    //0 - sin, 1 - pulse, 2 - inv_pulse, 3 - triangl, 4 - gaus noise , 5 - uniform noise
    sSignalParam SignalParam[6];
    char pos_x;         // позиция для рисования      
    char CurSigForm;    // 0 - sin, 1 - pulse, 2 - inv_pulse, 3 - triangl, 4 - gaus noise , 5 - uniform noise
    char isOn;          // включен ли канал (1 - включен)
  }sChannel;  
    
 #ifdef	MDrv 
  
    // 0 - Channel 1, 1 - Channel 2
    sChannel Channel[2];

    // 0 - Channel 1, 1 - Channel 2
    char CurChannel;

    // режим редактирования (1 - включен)
    char EditMode;    //bool
    
    // controls switching between measuring/generating display modes
    // 0 - gen_mode
    // 1 - meas_mode
    char is_meas_mode; //bool

//-------------------------------------------------------------------------------------------------------------
    //имееют значение только в режиме редактирования(EditMode = 1)
    //эти переменные изменяются при каждом переключении в режим редактирования
    //это происходит внутри Switch_to_EditMode

    //был ли введен знак(+/-), для редактируемого параметра 
    char didSignSet;
    
    //была ли введен десятичная точка, для редактируемого параметра 
    char didDotSet;
    
    //отрицательно ли число(связано с особенностями state machine редактирования параметра)
    char isMin;
    
    //кол-во введеных цифр
    char numPutsDig;
    
    //предыдущее значение и множитель для state machine редактирования параметра 
    float PrevValue, multiplier;
    
    // текущий редактируемый параметр(индекс), имеет смысл только при EditMode = 1
    // 0 - A, 1 - O, 2 - F, 3 - D, 4 - m, 5 - var
    char CurEditParam; 
    
    //строка отображения вводимых цифр
    char strParam[10];
    
    //массив состояний кнопок, для переключения режимов редактирования
    char StateButton[6];
    
//-------------------------------------------------------------------------------------------------------------
 
    void Switch_to_EditMode( int param );
    void EditParam( char ButNum );
    void ReDrawParam( const sParam *pParam );
    void ReDrawZeroRow( void );
    void re_draw_body( void );
    
#endif