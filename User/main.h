
// File:    main.h 

#ifdef	MAIN
  #define GLOBAL 				
#else	
  #define GLOBAL extern
#endif


 //���������� ����������           
  
#ifdef MAIN

 #define MAIN
 #include "CommonDefines.h"  //��� �������������� � FreeRTOS

 // �������� ��������� ������� -----------------------------------------------
  
 //
 void init_common( void );

 // ������� ������� ��
 int Create_OS_Objects(void);
 
 //void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed portCHAR *pcTaskName );

 // �������� ���������� -------------------------------------------------------
 
#endif  




  
