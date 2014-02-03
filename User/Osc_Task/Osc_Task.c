// OscTask 

#define OscTask
#include "Osc_Task.h"

// ������ ������������ InterDefines.h ��� ����������� sOscParam
#include "Osc_Drv.h"         //��� ������ ������� �������� ���������

#include "CommonDefines.h"  //��� �������������� � FreeRTOS
  
// �������, ���������� ����� OscTask
//==============================================================================
void vOscTask( void *pvParameters )
{ 
  sOscParam OscParam; 
  
  InitDAC_TIM_DMA();
  
  while( 1 )
  { 
    //���� ������� ��������� �� PBTask ��� DMA ISR
    xQueueReceive( qTo_Osc, (void *)&OscParam, portMAX_DELAY );
    
    // ���������, �� ����������� ���������� DMA, ��������������� � �������������
    // ����� ������� ����������� ������� ������������ ������ DAC_Buff, ��������� ���������
    // pDAC_Buff � ���������� ������� ������ ������������ ������ DAC_Buff ��������� �������
    // �������� ����� �������
    if( OscParam.Sig_Type == 7 )
      ReFill();
    else if( OscParam.Sig_Type == SWEEP_CONTROL )
      sweep_on_off( OscParam.Ch_num );

    // ���������, �� ������ M_Thread, ��������������� � ������������� ����� ���� �������
    // ���/� ��� ����������
    // � ���������� ������� ������� ������������ ������ DAC_Buff ��������� �������
    // �������� ����� �������
    else
      ReCalc( &OscParam );
  } 
  
  // ���������� ������, ���� ��������� ����� �� ������������ �����  
  vTaskDelete( NULL );
}


