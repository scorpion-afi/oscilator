
// SDTask 

#ifdef SDTask
  #define GLB_SDTask 	
#else	
  #define GLB_SDTask extern
#endif

  // thread-function vSDTask
  GLB_SDTask void vSDTask( void* pvParameters );  

#ifdef SDTask

#endif