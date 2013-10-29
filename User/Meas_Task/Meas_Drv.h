//драйвер генерации сигналов

#ifdef MeasDrv
  #define GLB_MeasDrv 		
#else	
  #define GLB_MeasDrv extern
#endif

  #define ADC_NUM         4096                  // unsigned short
  #define ADC_NUM_DIV_2   (ADC_NUM/2)

  GLB_MeasDrv unsigned short* p_beg_adc_buff;

  GLB_MeasDrv void start( void );
  GLB_MeasDrv void stop( void );

#ifdef MeasDrv
  
  void init( void ); 
  
#endif