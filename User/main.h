
// File:    main.h 

#ifdef	MAIN
  #define GLOBAL 				
#else	
  #define GLOBAL extern
#endif


 //Глобальные переменные           
  
#ifdef MAIN

 #define MAIN
 #include "CommonDefines.h"  //для взаимодействия с FreeRTOS

 // Закрытые прототипы функций -----------------------------------------------
  
 void init_common( void );
 void init_exti( void );

 // Создаем обьекты ОС
 int Create_OS_Objects(void);
 
 //void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed portCHAR *pcTaskName );

 // Закрытые переменные -------------------------------------------------------
 
#endif  




  
