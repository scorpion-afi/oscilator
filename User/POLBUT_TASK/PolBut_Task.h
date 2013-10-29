  
// PBTask 

#ifdef PBTask
  #define GLB_PBTask 	
#else	
  #define GLB_PBTask extern
#endif

// функци€, релизующ€€ поток PBTask
GLB_PBTask void vPBTask(void *pvParameters);

#ifdef PBTask

  // ѕодготовка сообщени€ дл€ отправки в очередь потоку MTask
  void Send_PolBut_Message(int typeEvent, int typePad, int num);
  
#endif