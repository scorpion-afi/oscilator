// MeasTask 

#ifdef CalcTask
  #define GLB_CalcTask 	
#else	
  #define GLB_CalcTask extern
#endif

  // функци€, релизующ€€ поток OscTask
  GLB_CalcTask void vCalcTask( void* pvParameters );  

#ifdef CalcTask

#endif