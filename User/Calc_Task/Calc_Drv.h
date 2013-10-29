//драйвер генерации сигналов

#ifdef CalcDrv
  #define GLB_CalcDrv 		
#else	
  #define GLB_CalcDrv extern
#endif

  GLB_CalcDrv void init_calc( void );
  GLB_CalcDrv void show_result( float i_ch1, float u_ch1,
                                float i_ch2, float u_ch2 );

#ifdef CalcDrv   
  
#endif