// SD driver 

#define SDDrv	
#include "SD_Drv.h"

// for FAT_FS
#include "diskio.h"  // for DSTATUS
#include "ff.h"

#define FILE_NAME "0:scor.txt"

FATFS fs;       // main FAT_FS struct
FIL file;       // file object

// initialization of sd thread
// return 0, if all is ok
//==============================================================================
int init_sd( void )
{
  DSTATUS card_status = 0;
  FRESULT res = FR_OK; 
  
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
  
  // opens/creates file with name FILE_NAME
  // FA_OPEN_ALWAYS - Opens the file if it is existing. If not, a new file is created.
  // FA_WRITE | FA_READ - Data can be read/written from/to the file.
  res = f_open( &file, FILE_NAME, FA_WRITE | FA_READ | FA_OPEN_ALWAYS ); 
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
  UINT len;                  // len will storage number of real written bytes
  FRESULT res; 

  // writes num bytes of data to file
  res = f_write( &file, data, num, &len );
  
  if( ( res ) || ( num != len ) )  //if some error was occured
  {
    return 1;    
  } 
  
  return f_sync( &file ); // flushes the cached information of a writing file 
}


//==============================================================================
//==============================================================================
//==============================================================================


// initialize TIM5 for FAT_FS purpose
//==============================================================================
void init_TIM5( void )
{
  NVIC_InitTypeDef          NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef   TIM_TimeBaseInitStruct;
   
  //socket_cp_init(); 
    
  // Enable TIM5 clocks
  RCC->APB1ENR |=  0x08;
  
  // necessary time delay - 10ms
  // 10 ms <-> 100 ips (interrupts per second)
  // PCLK1 = 36 ���
  // CK_PSC = 2*PCLK1 = 72 MGz
  // number of ips = CK_PSC / (PSC*CNT)
  // ==>
  // 100 ips = 72 * 10^6 / (1000*720)
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;            
  TIM_TimeBaseInitStruct.TIM_Period = 720;   // ���������� 100 ��� � �������
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

// deinitialize TIM5 for FAT_FS purpose
//==============================================================================
void de_init_TIM5( void )
{  
  // Disable TIM5 Update interrupt
  TIM_ITConfig( TIM5, TIM_IT_Update, DISABLE );
    
  // TIM5 disable counter
  TIM_Cmd( TIM5, DISABLE );
  
  // Disable TIM5 clocks
  RCC->APB1ENR &= ~0x08;
}