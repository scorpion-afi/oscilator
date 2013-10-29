
#define SINE	
#include "Sine.h"

#include "Osc_Drv.h"

#include "math.h"    //��� sin

#define pi       3.14159265358979323846
#define Nph_reg  4294967296        // 2^32-1 ����������� ��������, ��������� ��� ����.
#define Fsamples 100000

// ���������� �������� ������
//==============================================================================
void CalcSin(void)
{ 
  float dPhase;
  unsigned int temp;
   
  unsigned int Ampl;
  int Offset;
  unsigned int SUM;
  
  uint32_t *ptemp;
  
  ptemp = pDAC_Buff; 
   
  // ������� "��������" �������� ������� sin-��� ��� �������� ������,
  // � ������ ��������� � ��������
  if(!first_entry[CurDAC_Ch])
  {  
    first_entry[CurDAC_Ch] = 1;
    
    dPhase = 2*pi/(8192);  // � �� (8192 - 1)
    
    Ampl =  (unsigned int)( ((CurOscParam[CurDAC_Ch].amp + 5.0)*4095.0) / 10.0 );
    Offset = (int)( ((CurOscParam[CurDAC_Ch].offset + 5.0)*4095.0) / 10.0 );
 
    Ampl = 2047;
    Offset = 2047; 
    
    for(int i = 0; i < 2049 /*sizeof(RAMBuff[CurDAC_Ch])*/; i++)
      RAMBuff[CurDAC_Ch][i] = (uint16_t)(Ampl*sin(i*dPhase) + Offset);
    
    AcmPhase[CurDAC_Ch] = (unsigned int)(0/*phase*/*Nph_reg/(2*pi));   // phase: 0 - 2pi
    
    FREQ_REG[CurDAC_Ch] = (unsigned int)(0.1/*CurOscParam[CurDAC_Ch].freq*/*Nph_reg/Fsamples);     
  }
  
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 
  for(int i = 0; i < size_of_DAC_Buff/2; i++)
  {
    //����������� ���� 13 ������ 
    SUM = AcmPhase[CurDAC_Ch] >> 19;
    
    if(SUM < 2049)
    {
      if(CurDAC_Ch == 0)
      {
        *ptemp &= 0xFFFF0000;
        *ptemp++ |= RAMBuff[CurDAC_Ch][SUM];
      }
      else
      {
        *ptemp &= 0x0000FFFF;       
        *ptemp++ |= (RAMBuff[CurDAC_Ch][SUM]<<16); 
      }
    }
    else if( (SUM > 2048) && (SUM < 4097) )
    {
      SUM = 4096 - SUM;  //2048 - (SUM - 2048);
     
      if(CurDAC_Ch == 0)
      {
        *ptemp &= 0xFFFF0000;        
        *ptemp++ |= RAMBuff[CurDAC_Ch][SUM];
      }
      else 
      {
        *ptemp &= 0x0000FFFF;               
        *ptemp++ |= (RAMBuff[CurDAC_Ch][SUM]<<16);
      }
    }
    else if( (SUM > 4096) && (SUM < 6145) )
    {
      SUM -= 4096;
      
      temp = 4095 - RAMBuff[CurDAC_Ch][SUM]; // 2048 - (RAMBuff[CurDAC_Ch][SUM] - 2047);
        
      if(CurDAC_Ch == 0)
      {
        *ptemp &= 0xFFFF0000;          
        *ptemp++ |= temp;
      }
      else
      {
        *ptemp &= 0x0000FFFF;                       
        *ptemp++ |= (temp<<16);
      }
    }
    else if(SUM > 6144)
    {
      SUM = 8192 - SUM; //2048 - (SUM - 6144);
      
      temp = 4095 - RAMBuff[CurDAC_Ch][SUM]; // 2048 - (RAMBuff[CurDAC_Ch][SUM] - 2047);
     
      if(CurDAC_Ch == 0)
      {
        *ptemp &= 0xFFFF0000;                  
        *ptemp++ |= temp;
      }
      else
      {
        *ptemp &= 0x0000FFFF;                               
        *ptemp++ |= (temp<<16);
      }
    }
 /*   	
    //switch �� ����������(���������) ������
    switch(AcmPhase[CurDAC_Ch]>>30)
    {
      case 1: // ������ ��������
      {
        if(CurDAC_Ch == 0)
          *pDAC_Buff++ |= RAMBuff[CurDAC_Ch][SUM];
        else  
          *pDAC_Buff++ |= RAMBuff[CurDAC_Ch][SUM]<<16;        
      }
      break;
      
      case 2: // ������ ��������
      {
        SUM = 4096 - SUM;  //2048 - (SUM - 2048);
       
        if(CurDAC_Ch == 0)
          *pDAC_Buff++ |= RAMBuff[CurDAC_Ch][SUM];
        else  
          *pDAC_Buff++ |= RAMBuff[CurDAC_Ch][SUM]<<16;               
      }
      break;
      
      case 3: // ������ ��������
      {
        SUM -= 4096;
        
        temp = 2048 - (RAMBuff[CurDAC_Ch][SUM] - 2047);
          
        if(CurDAC_Ch == 0)
          *pDAC_Buff++ |= temp;
        else  
          *pDAC_Buff++ |= temp<<16;                
      }
      break;
      
      case 4: // ��������� ��������
      {
        SUM = 8192 - SUM; //2048 - (SUM - 6144);
        
        temp = 2048 - (RAMBuff[CurDAC_Ch][SUM] - 2047);
       
        if(CurDAC_Ch == 0)
          *pDAC_Buff++ |= temp;
        else  
          *pDAC_Buff++ |= temp<<16;                
      }
      break;     
    };
  */  
    // ����������� �������� ���������� ���� �� ��������, ��������������� �������,
    // �� ���� �������� ��������� ���������� ����
    AcmPhase[CurDAC_Ch] = (uint32_t)(AcmPhase[CurDAC_Ch] + FREQ_REG[CurDAC_Ch]);
  }
}