  
// PBTask 

#ifdef PBTask
  #define GLB_PBTask 	
#else	
  #define GLB_PBTask extern
#endif

// �������, ���������� ����� PBTask
GLB_PBTask void vPBTask(void *pvParameters);

#ifdef PBTask

  // ���������� ��������� ��� �������� � ������� ������ MTask
  void Send_PolBut_Message(int typeEvent, int typePad, int num);
  
#endif