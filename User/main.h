
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

 // ������� ������� ��
 int Create_OS_Objects(void);
 
 //void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed portCHAR *pcTaskName );

 // �������� ���������� -------------------------------------------------------
 
#endif  




  
