
// SD driver

#ifdef SDDrv
  #define GLB_SDDrv 		
#else	
  #define GLB_SDDrv extern
#endif

  GLB_SDDrv int  init_sd( void );
  GLB_SDDrv void init_TIM5( void );
  GLB_SDDrv void de_init_TIM5( void );
  GLB_SDDrv unsigned int write( const void* data, unsigned int num );
  GLB_SDDrv unsigned int read( void* buf, unsigned int num );
  
#ifdef SDDrv   

   
#endif