
// "�������" ���� 

#ifdef MDrv
  #define GLB_MDrv				
#else	
  #define GLB_MDrv extern
#endif

  #include "InterDefines.h" // only for declaration of s_pol_button struct
  
  //���������, ����������� �������� ������� (A, O, F, D, m, var)
  // � ��������� � �������� ���������
  //���������, ������, ���� ���������� ����� ��������� ������������ ������ ���� "O = -0.3B"
  //������ ������� ��������� � ��������� ������� ������� ��� ��������������
  typedef struct  
  {
    float min;           //������������
    float max;           //������������
    float cur;
    float step;          // ��� ��������� (��� ������ "+" � "-")
    char *pacronym;      // ��������� (A = )
    char *punit;         // ������� ��������� (B)
    char bg_pos_x;       // ��������� ������� ������� ��� ��������������
    char bg_pos_y;       // ��������� ������� ������� ��� �������������� 
    char isVisible;      // ����� �� ���� �������� ���������� �� LCD
    char *format;        // ������ ��� sprintf
    char isAvailable;    // ������������  ��������� � �������
    char index_together; // ������ ���������, � ������� ������� ������� �� LCD
    char isSign;         // 1 - �������� ��������
    char numDig;         // max ���-�� ����(���������, ��� ���������� �����)
    char isFract;		   // ������� �� ��������
  }sParam;

  //���������, ����������� ������, ���, ������� ����� � ��� ���������
  typedef struct  
  {
    char *pname;      // ��� ������� � ��������� (SF:pulse)
    char name_pos_y;  // ������� ��� ��������� �����   
    sParam Param[6];  // 0 - A, 1 - O, 2 - F, 3 - D, 4 - m, 5 - var
    char is_freq_sweep_on; // is frequency sweeping allowed ?  1 - allowed, 0 - not allowed
  }sSignalParam;
  
  //���������, ����������� ����� � ��� ���������
  typedef struct  
  {	
    //0 - sin, 1 - pulse, 2 - inv_pulse, 3 - triangl, 4 - gaus noise , 5 - uniform noise
    sSignalParam SignalParam[6];
    char pos_x;         // ������� ��� ���������      
    char CurSigForm;    // 0 - sin, 1 - pulse, 2 - inv_pulse, 3 - triangl, 4 - gaus noise , 5 - uniform noise
    char isOn;          // ������� �� ����� (1 - �������)
  }sChannel;  

  //��������� ������� �� LCD
  GLB_MDrv void InitView( void );

  //menu state machine
  GLB_MDrv void MenuStateMach( const s_pol_button* message );
    
 #ifdef	MDrv 
  
    // 0 - Channel 1, 1 - Channel 2
    sChannel Channel[2];

    // 0 - Channel 1, 1 - Channel 2
    char CurChannel;

    // ����� �������������� (1 - �������)
    char EditMode;    //bool
    
    // controls switching between measuring/generating display modes
    // 0 - gen_mode
    // 1 - meas_mode
    char is_meas_mode; //bool
     
//-------------------------------------------------------------------------------------------------------------
    //������ �������� ������ � ������ ��������������(EditMode = 1)
    //��� ���������� ���������� ��� ������ ������������ � ����� ��������������
    //��� ���������� ������ Switch_to_EditMode

    //��� �� ������ ����(+/-), ��� �������������� ��������� 
    char didSignSet;
    
    //���� �� ������ ���������� �����, ��� �������������� ��������� 
    char didDotSet;
    
    //������������ �� �����(������� � ������������� state machine �������������� ���������)
    char isMin;
    
    //���-�� �������� ����
    char numPutsDig;
    
    //���������� �������� � ��������� ��� state machine �������������� ��������� 
    float PrevValue, multiplier;
    
    // ������� ������������� ��������(������), ����� ����� ������ ��� EditMode = 1
    // 0 - A, 1 - O, 2 - F, 3 - D, 4 - m, 5 - var
    char CurEditParam; 
    
    //������ ����������� �������� ����
    char strParam[10];
    
    //������ ��������� ������, ��� ������������ ������� ��������������
    char StateButton[6];
    
//-------------------------------------------------------------------------------------------------------------
 
    void Switch_to_EditMode( int param );
    void EditParam( char ButNum );
    void ReDrawParam( const sParam *pParam );
    void ReDrawZeroRow( void );
    void re_draw_body( void );
    
#endif
