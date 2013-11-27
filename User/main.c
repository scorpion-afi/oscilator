
// SYSCLK = HCLK =  72 ћ√ц.
// PCLK1  =         36 ћ√ц.
// PCLK2  =         72 ћ√ц.

#define MAIN
#include "main.h"

#include "stm32f10x_rcc.h"  

#include "diskio.h"
#include "ff.h"

int init_sd( void );
void init_TIM5( void );
unsigned int write( const void* data, unsigned int num );

FATFS fs;       // main FAT_FS struct
FIL file;       // file object

//точка входа
//=======================================================================================
int main()
{ 
  RCC_ClocksTypeDef kyky;
  RCC_GetClocksFreq( &kyky );
 
  init_sd();
  
  write( "hello afi", 9 );
   
  while(1)
  {
    ;
  }
  //-+-+-+-+-+-+-+-+-+-+-+ 
}

// initialization of sd thread
// return 0, if all is ok
//==============================================================================
int init_sd( void )
{
  DSTATUS card_status = 0;
  FRESULT res = FR_OK; 
  
  init_TIM5();
  
  for( int i = 0; i < 100000; i++ );
  
  card_status = disk_initialize( 0 );
  if( card_status )
  {
    return 1;
  }
   
  res = f_mount( 0, &fs );   // mounts disk 0 with fs           
  if( res )
  {
    return 2;    
  }

  // creates file with name FILE_NAME and open it for write
  res = f_open( &file, "0:readmea.txt", FA_WRITE | FA_CREATE_ALWAYS ); 
  if( res )
  {
    return 3;    
  } 
  
  return 0;
}

// data - pointer to data to be written on sd-card
// num - size of data in bytes !!!
// return 0, if all is ok
//==============================================================================
unsigned int write( const void* data, unsigned int num )
{
  UINT len = 0;                  // len will storage number of real written bytes
  FRESULT res = FR_OK; 

  // writes num bytes of data to file
  res = f_write( &file, data, num, &len );
  if( res )
  {
    return 1;    //if some error was occured
  }
  
  if( num != len )
  {
    return 2;    //if some error was occured
  }
  
  return 0; 
}


//==============================================================================
//==============================================================================
//==============================================================================


// initialize TIM5 for FAT_FS purpose
//==============================================================================
void init_TIM5(void)
{
  NVIC_InitTypeDef          NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef   TIM_TimeBaseInitStruct;
  
  // Enable TIM5 clocks
  RCC->APB1ENR |=  0x08;
  
  // necessary time delay - 10ms
  // 10 ms <-> 100 ips (interrupts per second)
  // PCLK1 = 36 ћ√ц
  // CK_PSC = 2*PCLK1 = 72 MGz
  // number of ips = CK_PSC / (PSC*CNT)
  // ==>
  // 100 ips = 72 * 10^6 / (1000*720)
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;            
  TIM_TimeBaseInitStruct.TIM_Period = 720;   // прерывани€ 100 раз в секунду
  TIM_TimeBaseInitStruct.TIM_Prescaler = 999; // 1000 - 1
  TIM_TimeBaseInit( TIM5, &TIM_TimeBaseInitStruct );
   
  // Enable the TIM5 Interrupt
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 14; // 14 or 1101 1111 > configMAX_SYSCALL_INTERRUPT_PRIORITY
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init( &NVIC_InitStructure ); 

  // TIM5 enable counter
  TIM_Cmd( TIM5, ENABLE );
  
  // Enable TIM5 Update interrupt
  TIM_ITConfig( TIM5, TIM_IT_Update, ENABLE );
}