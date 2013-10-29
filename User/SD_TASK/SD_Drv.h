
// SD driver

#ifdef SDDrv
  #define GLB_SDDrv 		
#else	
  #define GLB_SDDrv extern
#endif

  GLB_SDDrv int init_sd( void );
  GLB_SDDrv unsigned int write( const void* data, unsigned int num );

#ifdef SDDrv   

  void init_TIM5( void );
    
#endif