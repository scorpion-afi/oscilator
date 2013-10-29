  
  #include "FreeRTOS.h"
  #include "task.h"         //переключение задач
  #include "list.h"
  #include "queue.h"

  #ifdef MAIN
    #define GLB_Com					
  #else	    
    #define GLB_Com extern
  #endif

  //переменные для работы с очередями
  GLB_Com xQueueHandle qPB_to_M, qM_to_LCD, qTo_Osc, queu_to_meas, queu_to_calc,
                       queu_to_sd;

  // for lock sending message from DMA1 Channel 1 ISR to Calc thread
  GLB_Com int lock_send_message_to_calc_thread;
