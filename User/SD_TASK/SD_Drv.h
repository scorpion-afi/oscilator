
// SD driver

#ifdef SDDrv
  #define GLB_SDDrv 		
#else	
  #define GLB_SDDrv extern
#endif

  GLB_SDDrv unsigned char init_sd( void );
  GLB_SDDrv unsigned int open_file( void );
  GLB_SDDrv unsigned int close_file( void );
  GLB_SDDrv unsigned int write_file( const void* data, unsigned int num );  
  GLB_SDDrv void init_TIM5( void );
  GLB_SDDrv void de_init_TIM5( void );
  
#ifdef SDDrv   
   
#endif