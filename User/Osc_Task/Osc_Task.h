// OscTask 

#ifdef OscTask
  #define GLB_OscTask 	
#else	
  #define GLB_OscTask extern
#endif

  // функци€, релизующ€€ поток OscTask
  GLB_OscTask void vOscTask(void *pvParameters);  

#ifdef OscTask

#endif