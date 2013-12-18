// SD driver 

#define SDDrv	
#include "SD_Drv.h"

// for FAT_FS
#include "diskio.h"  // for DSTATUS
#include "ff.h"

#define FILE_NAME "0:adc.txt"

FATFS fs;          // main FAT_FS struct
FIL file;          // file object
FILINFO fil_info;  // file info (for debug only)
FRESULT res;       // (for debug only)

// inits tim5 and hardwares linked with sd card
// return 0, if initilization has been finished successfully
//==============================================================================
unsigned char init_sd( void )  
{ 
  DSTATUS card_status; 
  
  init_TIM5();
  
  card_status = disk_initialize( 0 ); 
    
  res = f_mount( 0, &fs );       // выполн€ем св€зывание диска со структурой fs
    
  return card_status;
}

// opens file FILE_NAME for appending (file pointer will be shifted to end of file)
// return 0, if initilization has been finished successfully
//==============================================================================
unsigned int open_file( void )
{
  DWORD size;
  
  // opens/creates file with name path
  // FA_OPEN_ALWAYS - Opens the file if it is existing. If not, a new file is created.
  // FA_WRITE - Data can be written to the file.
  res = f_open( &file, FILE_NAME, FA_WRITE | FA_OPEN_ALWAYS ); 
  if( res )
  {
    return res;    //если произошла ошибка
  }
  
  res = f_stat( FILE_NAME, &fil_info );
    
  size = f_size( &file );
  
  // move to end of file to append data 
  return f_lseek( &file, size );
}

// closes file FILE_NAME
// return 0, if initilization has been finished successfully
//==============================================================================
unsigned int close_file( void )
{
  return f_close( &file );
}

// data - pointer to data to be written on sd-card
// num - size of data in bytes !!!
// return 0, if all is ok
//==============================================================================
unsigned int write_file( const void* data, unsigned int num )
{
  UINT len;                  // len will storage number of real written bytes

  // writes num bytes of data to file
  res = f_write( &file, data, num, &len );
  
  if( ( res ) || ( num != len ) )  //if some error was occured
  {
    return res;    
  } 
  
  return f_sync( &file ); // flushes the cached information of a writing file
}

// TIM5 -- APB1 (TIMXCLK = 72 MHz, not PCLK1 !!!)
// initialization of tim 5 to generate interrupts (10 ms)
//==============================================================================
void init_TIM5( void )
{
  NVIC_InitTypeDef          NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef   TIM_TimeBaseInitStruct;
  
  // Enable TIM5 clocks
  //RCC->APB1ENR |=  0x08;
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM5, ENABLE );
  
  // Deinitializes the TIM5 peripheral registers to their default reset values 
  TIM_DeInit( TIM5 );
  
  // Fills each TIM_TimeBaseInitStruct member with its default value
  TIM_TimeBaseStructInit( &TIM_TimeBaseInitStruct );
  
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
  
  // Enable TIM5 Update interrupt
  TIM_ITConfig( TIM5, TIM_IT_Update, ENABLE );  
  
  // TIM5 enable counter
  TIM_Cmd( TIM5, ENABLE );  
  
  // Enable the TIM5 Interrupt
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 13; // 13 or 1101 1111 > configMAX_SYSCALL_INTERRUPT_PRIORITY
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init( &NVIC_InitStructure ); 
}

// deinitialize TIM5 for FAT_FS purpose
//==============================================================================
void de_init_TIM5( void )
{  
  // Disable TIM5 Update interrupt
  TIM_ITConfig( TIM5, TIM_IT_Update, DISABLE );
    
  // TIM5 disable counter
  TIM_Cmd( TIM5, DISABLE );
  
  // Disable TIM5 clocks
  //RCC->APB1ENR &= ~0x08;
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM5, DISABLE );
}