// MeasTask 

#ifdef MeasTask
  #define GLB_MeasTask 	
#else	
  #define GLB_MeasTask extern
#endif

  // �������, ���������� ����� OscTask
  GLB_MeasTask void vMeasTask( void* pvParameters );  

#ifdef MeasTask

#endif