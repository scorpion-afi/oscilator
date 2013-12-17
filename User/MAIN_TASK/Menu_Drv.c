
// "драйвер" меню 

#define MDrv	
#include "Menu_Drv.h"

  #include "stdio.h"
  #include "string.h"
  #include "stdlib.h"

  char SigName_0[] = "SF:sin";
  char SigName_1[] = "SF:pulse";
  char SigName_2[] = "SF:exp";
  char SigName_3[] = "SF:sawtoo";
  char SigName_4[] = "SF:norm";
  char SigName_5[] = "SF:unifrm";
  
  char Acronym_A[] = "A = ";
  char Acronym_O[] = "O = ";
  char Acronym_F[] = "F=";
  char Acronym_D[] = "D = ";
  char Acronym_M[] = "m = ";
  char Acronym_var[] = "ь = ";  //ско
  
  char Unit_B[] = "B";
  char Unit_Hz[] = "Hz";
  char Unit_percent[] = "%";
  
  char V_format[] =    "%2.1f";
  char Hz_format[] =   "%5.0f";       //some problems
  char Perc_format[] = "%3.0f";

  //аппаратно зависимые callback-функции, обьявлены  в файле Menu_Task.c     
  extern void lcd_write(char *pStr, char x, char y);
  extern void shift_blink(char x, char y);
  extern void blink_control(int isOn);
  extern void preInitDAC(const sChannel *ptr, char curCh);
  extern void Off_DAC_Channel(char ChNum);
  extern void meas_control( char is_on );
  extern void sd_control( void );
  
  // from "CommonDefines.h"
  extern int lock_send_message_to_calc_thread;
  
//начальная картина на LCD
//==============================================================================
void InitView(void)
{    
  //0 - sin, 1 - pulse, 2 - exp, 3 - sawtooth, 4 - norm noise , 5 - uniform noise
  // 0 - A(D), 1 - O(M), 2 - F, 3 - D  
  CurChannel = 0;
  EditMode = 0;
  
  is_meas_mode = 1;       // measurement mode
  is_sd_write_mode = 0;   // write to sd is disallowed

//Channel 1 
  Channel[0].pos_x = 0;
  Channel[0].CurSigForm  = 1;
  Channel[0].isOn = 1;
 
  //SignalParam 0  sin
  Channel[0].SignalParam[0].pname = SigName_0; 
  Channel[0].SignalParam[0].name_pos_y = 1;
  
  //Amplitude
  Channel[0].SignalParam[0].Param[0].min = 0.0;   
  Channel[0].SignalParam[0].Param[0].max = 5.0;   
  Channel[0].SignalParam[0].Param[0].cur = 2.0;
  Channel[0].SignalParam[0].Param[0].step = 0.1;
  Channel[0].SignalParam[0].Param[0].pacronym = Acronym_A;
  Channel[0].SignalParam[0].Param[0].punit = Unit_B;
  Channel[0].SignalParam[0].Param[0].bg_pos_x  = 4;
  Channel[0].SignalParam[0].Param[0].bg_pos_y  = 2;
  Channel[0].SignalParam[0].Param[0].isVisible = 1;
  Channel[0].SignalParam[0].Param[0].format = V_format;
  Channel[0].SignalParam[0].Param[0].isAvailable = 1;
  Channel[0].SignalParam[0].Param[0].index_together = 1;
  Channel[0].SignalParam[0].Param[0].isSign = 0;
  Channel[0].SignalParam[0].Param[0].numDig = 2;
  Channel[0].SignalParam[0].Param[0].isFract = 1;
    
  //Offset
  Channel[0].SignalParam[0].Param[1].min = 0.0;   
  Channel[0].SignalParam[0].Param[1].max = 5.0;   
  Channel[0].SignalParam[0].Param[1].cur = 2.0;   
  Channel[0].SignalParam[0].Param[1].step = 0.1;
  Channel[0].SignalParam[0].Param[1].pacronym = Acronym_O;
  Channel[0].SignalParam[0].Param[1].punit = Unit_B;
  Channel[0].SignalParam[0].Param[1].bg_pos_x  = 4;
  Channel[0].SignalParam[0].Param[1].bg_pos_y  = 2;
  Channel[0].SignalParam[0].Param[1].isVisible = 0;
  Channel[0].SignalParam[0].Param[1].format = V_format;
  Channel[0].SignalParam[0].Param[1].isAvailable = 1;
  Channel[0].SignalParam[0].Param[1].index_together = 0;
  Channel[0].SignalParam[0].Param[1].isSign = 1;
  Channel[0].SignalParam[0].Param[1].numDig = 2;
  Channel[0].SignalParam[0].Param[1].isFract = 1;
  
  //Frequency
  Channel[0].SignalParam[0].Param[2].min = 0.1;   
  Channel[0].SignalParam[0].Param[2].max = 10000.0;   
  Channel[0].SignalParam[0].Param[2].cur = 0.0;   
  Channel[0].SignalParam[0].Param[2].step = 1.0;
  Channel[0].SignalParam[0].Param[2].pacronym = Acronym_F;
  Channel[0].SignalParam[0].Param[2].punit = Unit_Hz;
  Channel[0].SignalParam[0].Param[2].bg_pos_x  = 2;
  Channel[0].SignalParam[0].Param[2].bg_pos_y  = 3;
  Channel[0].SignalParam[0].Param[2].isVisible = 1;
  Channel[0].SignalParam[0].Param[2].format = Hz_format;
  Channel[0].SignalParam[0].Param[2].isAvailable = 1;
  Channel[0].SignalParam[0].Param[2].index_together = 0;
  Channel[0].SignalParam[0].Param[2].isSign = 0;
  Channel[0].SignalParam[0].Param[2].numDig = 6;
  Channel[0].SignalParam[0].Param[2].isFract = 0;

  //Duty
  Channel[0].SignalParam[0].Param[3].isAvailable = 0;
  Channel[0].SignalParam[0].Param[3].isVisible = 0;

  //SignalParam 1  pulse  
  Channel[0].SignalParam[1].pname = SigName_1;
  Channel[0].SignalParam[1].name_pos_y = 1;
  
  //Amplitude
  Channel[0].SignalParam[1].Param[0].min = 0.0;
  Channel[0].SignalParam[1].Param[0].max = 5.0;
  Channel[0].SignalParam[1].Param[0].cur = 0.0;   
  Channel[0].SignalParam[1].Param[0].step = 0.1;
  Channel[0].SignalParam[1].Param[0].pacronym = Acronym_A;
  Channel[0].SignalParam[1].Param[0].punit = Unit_B;
  Channel[0].SignalParam[1].Param[0].bg_pos_x  = 4;
  Channel[0].SignalParam[1].Param[0].bg_pos_y  = 2;
  Channel[0].SignalParam[1].Param[0].isVisible = 0;
  Channel[0].SignalParam[1].Param[0].format = V_format;
  Channel[0].SignalParam[1].Param[0].isAvailable = 1;
  Channel[0].SignalParam[1].Param[0].index_together = 1;
  Channel[0].SignalParam[1].Param[0].isSign = 0;
  Channel[0].SignalParam[1].Param[0].numDig = 2;
  Channel[0].SignalParam[1].Param[0].isFract = 1;
   
  //Offset
  Channel[0].SignalParam[1].Param[1].min = 0.0;
  Channel[0].SignalParam[1].Param[1].max = 5.0;
  Channel[0].SignalParam[1].Param[1].cur = 1.0;   
  Channel[0].SignalParam[1].Param[1].step = 0.1;
  Channel[0].SignalParam[1].Param[1].pacronym = Acronym_O;
  Channel[0].SignalParam[1].Param[1].punit = Unit_B;
  Channel[0].SignalParam[1].Param[1].bg_pos_x  = 4;
  Channel[0].SignalParam[1].Param[1].bg_pos_y  = 2;
  Channel[0].SignalParam[1].Param[1].isVisible = 1;
  Channel[0].SignalParam[1].Param[1].format = V_format;
  Channel[0].SignalParam[1].Param[1].isAvailable = 1;
  Channel[0].SignalParam[1].Param[1].index_together = 0;
  Channel[0].SignalParam[1].Param[1].isSign = 1;
  Channel[0].SignalParam[1].Param[1].numDig = 2;
  Channel[0].SignalParam[1].Param[1].isFract = 1;
   
  //Frequency
  Channel[0].SignalParam[1].Param[2].min = 0.1;
  Channel[0].SignalParam[1].Param[2].max = 10000.0;   
  Channel[0].SignalParam[1].Param[2].cur = 0.0;   
  Channel[0].SignalParam[1].Param[2].step = 1.0;
  Channel[0].SignalParam[1].Param[2].pacronym = Acronym_F;
  Channel[0].SignalParam[1].Param[2].punit = Unit_Hz;
  Channel[0].SignalParam[1].Param[2].bg_pos_x  = 2;
  Channel[0].SignalParam[1].Param[2].bg_pos_y  = 3;
  Channel[0].SignalParam[1].Param[2].isVisible = 0;
  Channel[0].SignalParam[1].Param[2].format = Hz_format;
  Channel[0].SignalParam[1].Param[2].isAvailable = 1;
  Channel[0].SignalParam[1].Param[2].index_together = 3;
  Channel[0].SignalParam[1].Param[2].isSign = 0;
  Channel[0].SignalParam[1].Param[2].numDig = 6;
  Channel[0].SignalParam[1].Param[2].isFract = 0;

  //Duty
  Channel[0].SignalParam[1].Param[3].min = 20;
  Channel[0].SignalParam[1].Param[3].max = 100.0; 
  Channel[0].SignalParam[1].Param[3].cur = 30;   
  Channel[0].SignalParam[1].Param[3].step = 1; 
  Channel[0].SignalParam[1].Param[3].pacronym = Acronym_D;
  Channel[0].SignalParam[1].Param[3].punit = Unit_percent;
  Channel[0].SignalParam[1].Param[3].bg_pos_x  = 5;
  Channel[0].SignalParam[1].Param[3].bg_pos_y  = 3;
  Channel[0].SignalParam[1].Param[3].isVisible = 1;
  Channel[0].SignalParam[1].Param[3].format = Perc_format;
  Channel[0].SignalParam[1].Param[3].isAvailable = 1;
  Channel[0].SignalParam[1].Param[3].index_together = 2;
  Channel[0].SignalParam[1].Param[3].isSign = 0;
  Channel[0].SignalParam[1].Param[3].numDig = 3;
  Channel[0].SignalParam[1].Param[3].isFract = 0;
   
  //SignalParam 2  exp
  Channel[0].SignalParam[2].pname = SigName_2;
  Channel[0].SignalParam[2].name_pos_y = 1;
  
  //Amplitude
  Channel[0].SignalParam[2].Param[0].min = 0.1;
  Channel[0].SignalParam[2].Param[0].max = 5.0;
  Channel[0].SignalParam[2].Param[0].cur = 1.0;   
  Channel[0].SignalParam[2].Param[0].step = 0.1;
  Channel[0].SignalParam[2].Param[0].pacronym = Acronym_A;
  Channel[0].SignalParam[2].Param[0].punit = Unit_B;
  Channel[0].SignalParam[2].Param[0].bg_pos_x  = 4;
  Channel[0].SignalParam[2].Param[0].bg_pos_y  = 2;
  Channel[0].SignalParam[2].Param[0].isVisible = 0;
  Channel[0].SignalParam[2].Param[0].format = V_format;
  Channel[0].SignalParam[2].Param[0].isAvailable = 1;
  Channel[0].SignalParam[2].Param[0].index_together = 1;
  Channel[0].SignalParam[2].Param[0].isSign = 0;
  Channel[0].SignalParam[2].Param[0].numDig = 2;
  Channel[0].SignalParam[2].Param[0].isFract = 1;
   
  //Offset
  Channel[0].SignalParam[2].Param[1].min = 0.0;
  Channel[0].SignalParam[2].Param[1].max = 4.9;
  Channel[0].SignalParam[2].Param[1].cur = 1.0;   
  Channel[0].SignalParam[2].Param[1].step = 0.1;
  Channel[0].SignalParam[2].Param[1].pacronym = Acronym_O;
  Channel[0].SignalParam[2].Param[1].punit = Unit_B;
  Channel[0].SignalParam[2].Param[1].bg_pos_x  = 4;
  Channel[0].SignalParam[2].Param[1].bg_pos_y  = 2;
  Channel[0].SignalParam[2].Param[1].isVisible = 1;
  Channel[0].SignalParam[2].Param[1].format = V_format;
  Channel[0].SignalParam[2].Param[1].isAvailable = 1;
  Channel[0].SignalParam[2].Param[1].index_together = 0;
  Channel[0].SignalParam[2].Param[1].isSign = 1;
  Channel[0].SignalParam[2].Param[1].numDig = 2;
  Channel[0].SignalParam[2].Param[1].isFract = 1;
   
  //Frequency
  Channel[0].SignalParam[2].Param[2].min = 0.1;
  Channel[0].SignalParam[2].Param[2].max = 10000.0;   
  Channel[0].SignalParam[2].Param[2].cur = 1.0;   
  Channel[0].SignalParam[2].Param[2].step = 1.0;
  Channel[0].SignalParam[2].Param[2].pacronym = Acronym_F;
  Channel[0].SignalParam[2].Param[2].punit = Unit_Hz;
  Channel[0].SignalParam[2].Param[2].bg_pos_x  = 2;
  Channel[0].SignalParam[2].Param[2].bg_pos_y  = 3;
  Channel[0].SignalParam[2].Param[2].isVisible = 0;
  Channel[0].SignalParam[2].Param[2].format = Hz_format;
  Channel[0].SignalParam[2].Param[2].isAvailable = 1;
  Channel[0].SignalParam[2].Param[2].index_together = 3;
  Channel[0].SignalParam[2].Param[2].isSign = 0;
  Channel[0].SignalParam[2].Param[2].numDig = 6;
  Channel[0].SignalParam[2].Param[2].isFract = 0;

  //Duty
  Channel[0].SignalParam[2].Param[3].min = 0.0;
  Channel[0].SignalParam[2].Param[3].max = 100.0; 
  Channel[0].SignalParam[2].Param[3].cur = 45;   
  Channel[0].SignalParam[2].Param[3].step = 1; 
  Channel[0].SignalParam[2].Param[3].pacronym = Acronym_D;
  Channel[0].SignalParam[2].Param[3].punit = Unit_percent;
  Channel[0].SignalParam[2].Param[3].bg_pos_x  = 5;
  Channel[0].SignalParam[2].Param[3].bg_pos_y  = 3;
  Channel[0].SignalParam[2].Param[3].isVisible = 1;
  Channel[0].SignalParam[2].Param[3].format = Perc_format;
  Channel[0].SignalParam[2].Param[3].isAvailable = 1;
  Channel[0].SignalParam[2].Param[3].index_together = 2;
  Channel[0].SignalParam[2].Param[3].isSign = 0;
  Channel[0].SignalParam[2].Param[3].numDig = 3;
  Channel[0].SignalParam[2].Param[3].isFract = 0;
   
  //SignalParam 3  sawtooth
  Channel[0].SignalParam[3].pname = SigName_3;
  Channel[0].SignalParam[3].name_pos_y = 1;
  
  //Amplitude
  Channel[0].SignalParam[3].Param[0].min = 0.0;
  Channel[0].SignalParam[3].Param[0].max = 5.0;
  Channel[0].SignalParam[3].Param[0].cur = 1.5;   
  Channel[0].SignalParam[3].Param[0].step = 0.1;
  Channel[0].SignalParam[3].Param[0].pacronym = Acronym_A;
  Channel[0].SignalParam[3].Param[0].punit = Unit_B;
  Channel[0].SignalParam[3].Param[0].bg_pos_x  = 4;
  Channel[0].SignalParam[3].Param[0].bg_pos_y  = 2;
  Channel[0].SignalParam[3].Param[0].isVisible = 0;
  Channel[0].SignalParam[3].Param[0].format = V_format;
  Channel[0].SignalParam[3].Param[0].isAvailable = 1;
  Channel[0].SignalParam[3].Param[0].index_together = 1;
  Channel[0].SignalParam[3].Param[0].isSign = 0;
  Channel[0].SignalParam[3].Param[0].numDig = 2;
  Channel[0].SignalParam[3].Param[0].isFract = 1;
   
  //Offset
  Channel[0].SignalParam[3].Param[1].min = 0.0;
  Channel[0].SignalParam[3].Param[1].max = 5.0;
  Channel[0].SignalParam[3].Param[1].cur = 2.0;   
  Channel[0].SignalParam[3].Param[1].step = 0.1;
  Channel[0].SignalParam[3].Param[1].pacronym = Acronym_O;
  Channel[0].SignalParam[3].Param[1].punit = Unit_B;
  Channel[0].SignalParam[3].Param[1].bg_pos_x  = 4;
  Channel[0].SignalParam[3].Param[1].bg_pos_y  = 2;
  Channel[0].SignalParam[3].Param[1].isVisible = 1;
  Channel[0].SignalParam[3].Param[1].format = V_format;
  Channel[0].SignalParam[3].Param[1].isAvailable = 1;
  Channel[0].SignalParam[3].Param[1].index_together = 0;
  Channel[0].SignalParam[3].Param[1].isSign = 1;
  Channel[0].SignalParam[3].Param[1].numDig = 2;
  Channel[0].SignalParam[3].Param[1].isFract = 1;
   
  //Frequency
  Channel[0].SignalParam[3].Param[2].min = 0.1;
  Channel[0].SignalParam[3].Param[2].max = 10000.0;   
  Channel[0].SignalParam[3].Param[2].cur = 0;   
  Channel[0].SignalParam[3].Param[2].step = 1.0;
  Channel[0].SignalParam[3].Param[2].pacronym = Acronym_F;
  Channel[0].SignalParam[3].Param[2].punit = Unit_Hz;
  Channel[0].SignalParam[3].Param[2].bg_pos_x  = 2;
  Channel[0].SignalParam[3].Param[2].bg_pos_y  = 3;
  Channel[0].SignalParam[3].Param[2].isVisible = 1;
  Channel[0].SignalParam[3].Param[2].format = Hz_format;
  Channel[0].SignalParam[3].Param[2].isAvailable = 1;
  Channel[0].SignalParam[3].Param[2].index_together = 0;
  Channel[0].SignalParam[3].Param[2].isSign = 0;
  Channel[0].SignalParam[3].Param[2].numDig = 6;
  Channel[0].SignalParam[3].Param[2].isFract = 0;
 
  //Duty
  Channel[0].SignalParam[3].Param[3].isAvailable = 0;
  Channel[0].SignalParam[3].Param[3].isVisible = 0;
   
  //SignalParam 4  gaussian noise
  Channel[0].SignalParam[4].pname = SigName_4;
  Channel[0].SignalParam[4].name_pos_y = 1;

  //Mean
  Channel[0].SignalParam[4].Param[4].min = 0.0;
  Channel[0].SignalParam[4].Param[4].max = 5.0;
  Channel[0].SignalParam[4].Param[4].cur = 1.5;   
  Channel[0].SignalParam[4].Param[4].step = 0.1;
  Channel[0].SignalParam[4].Param[4].pacronym = Acronym_M;
  Channel[0].SignalParam[4].Param[4].punit = Unit_B;
  Channel[0].SignalParam[4].Param[4].bg_pos_x  = 4;
  Channel[0].SignalParam[4].Param[4].bg_pos_y  = 2;
  Channel[0].SignalParam[4].Param[4].isVisible = 1;
  Channel[0].SignalParam[4].Param[4].format = V_format;
  Channel[0].SignalParam[4].Param[4].isAvailable = 1;
  Channel[0].SignalParam[4].Param[4].index_together = 0;
  Channel[0].SignalParam[4].Param[4].isSign = 1;
  Channel[0].SignalParam[4].Param[4].numDig = 2;
  Channel[0].SignalParam[4].Param[4].isFract = 1;

  //Variance (Standard Deviation)
  Channel[0].SignalParam[4].Param[5].min = 0.0;
  Channel[0].SignalParam[4].Param[5].max = 5.0;
  Channel[0].SignalParam[4].Param[5].cur = 1.5;   
  Channel[0].SignalParam[4].Param[5].step = 0.1;
  Channel[0].SignalParam[4].Param[5].pacronym = Acronym_var;
  Channel[0].SignalParam[4].Param[5].punit = Unit_B;
  Channel[0].SignalParam[4].Param[5].bg_pos_x  = 4;
  Channel[0].SignalParam[4].Param[5].bg_pos_y  = 3;
  Channel[0].SignalParam[4].Param[5].isVisible = 1;
  Channel[0].SignalParam[4].Param[5].format = V_format;
  Channel[0].SignalParam[4].Param[5].isAvailable = 1;
  Channel[0].SignalParam[4].Param[5].index_together = 0;
  Channel[0].SignalParam[4].Param[5].isSign = 0;
  Channel[0].SignalParam[4].Param[5].numDig = 2;
  Channel[0].SignalParam[4].Param[5].isFract = 1;
  
  //SignalParam 5  uniform noise
  Channel[0].SignalParam[5].pname = SigName_5;
  Channel[0].SignalParam[5].name_pos_y = 1;
  
  //Mean
  Channel[0].SignalParam[5].Param[4].min = 0.0;
  Channel[0].SignalParam[5].Param[4].max = 5.0;
  Channel[0].SignalParam[5].Param[4].cur = 1.5;   
  Channel[0].SignalParam[5].Param[4].step = 0.1;
  Channel[0].SignalParam[5].Param[4].pacronym = Acronym_M;
  Channel[0].SignalParam[5].Param[4].punit = Unit_B;
  Channel[0].SignalParam[5].Param[4].bg_pos_x  = 4;
  Channel[0].SignalParam[5].Param[4].bg_pos_y  = 2;
  Channel[0].SignalParam[5].Param[4].isVisible = 1;
  Channel[0].SignalParam[5].Param[4].format = V_format;
  Channel[0].SignalParam[5].Param[4].isAvailable = 1;
  Channel[0].SignalParam[5].Param[4].index_together = 0;
  Channel[0].SignalParam[5].Param[4].isSign = 1;
  Channel[0].SignalParam[5].Param[4].numDig = 2;
  Channel[0].SignalParam[5].Param[4].isFract = 1;

  //Variance (Standard Deviation)
  Channel[0].SignalParam[5].Param[5].min = 0.0;
  Channel[0].SignalParam[5].Param[5].max = 5.0;
  Channel[0].SignalParam[5].Param[5].cur = 1.5;   
  Channel[0].SignalParam[5].Param[5].step = 0.1;
  Channel[0].SignalParam[5].Param[5].pacronym = Acronym_var;
  Channel[0].SignalParam[5].Param[5].punit = Unit_B;
  Channel[0].SignalParam[5].Param[5].bg_pos_x  = 4;
  Channel[0].SignalParam[5].Param[5].bg_pos_y  = 3;
  Channel[0].SignalParam[5].Param[5].isVisible = 1;
  Channel[0].SignalParam[5].Param[5].format = V_format;
  Channel[0].SignalParam[5].Param[5].isAvailable = 1;
  Channel[0].SignalParam[5].Param[5].index_together = 0;
  Channel[0].SignalParam[5].Param[5].isSign = 0;
  Channel[0].SignalParam[5].Param[5].numDig = 2;
  Channel[0].SignalParam[5].Param[5].isFract = 1;
   
//Channel 2 
  
  Channel[1].pos_x = 11;
  Channel[1].CurSigForm  = 1;
  Channel[1].isOn = 1;
 
  //SignalParam 0  sin
  Channel[1].SignalParam[0].pname = SigName_0; 
  Channel[1].SignalParam[0].name_pos_y = 1;
  
  //Amplitude
  Channel[1].SignalParam[0].Param[0].min = 0.0;   
  Channel[1].SignalParam[0].Param[0].max = 5.0;   
  Channel[1].SignalParam[0].Param[0].cur = 2.0;
  Channel[1].SignalParam[0].Param[0].step = 0.1;
  Channel[1].SignalParam[0].Param[0].pacronym = Acronym_A;
  Channel[1].SignalParam[0].Param[0].punit = Unit_B;
  Channel[1].SignalParam[0].Param[0].bg_pos_x  = 15;
  Channel[1].SignalParam[0].Param[0].bg_pos_y  = 2;
  Channel[1].SignalParam[0].Param[0].isVisible = 1;
  Channel[1].SignalParam[0].Param[0].format = V_format;
  Channel[1].SignalParam[0].Param[0].isAvailable = 1;
  Channel[1].SignalParam[0].Param[0].index_together = 1;
  Channel[1].SignalParam[0].Param[0].isSign = 0;
  Channel[1].SignalParam[0].Param[0].numDig = 2;
  Channel[1].SignalParam[0].Param[0].isFract = 1;
    
  //Offset
  Channel[1].SignalParam[0].Param[1].min = 0.0;   
  Channel[1].SignalParam[0].Param[1].max = 5.0;   
 Channel[1].SignalParam[0].Param[1].cur = 3.0;   
  Channel[1].SignalParam[0].Param[1].step = 0.1;
  Channel[1].SignalParam[0].Param[1].pacronym = Acronym_O;
  Channel[1].SignalParam[0].Param[1].punit = Unit_B;
  Channel[1].SignalParam[0].Param[1].bg_pos_x  = 15;
  Channel[1].SignalParam[0].Param[1].bg_pos_y  = 2;
  Channel[1].SignalParam[0].Param[1].isVisible = 0;
  Channel[1].SignalParam[0].Param[1].format = V_format;
  Channel[1].SignalParam[0].Param[1].isAvailable = 1;
  Channel[1].SignalParam[0].Param[1].index_together = 0;
  Channel[1].SignalParam[0].Param[1].isSign = 1;
  Channel[1].SignalParam[0].Param[1].numDig = 2;
  Channel[1].SignalParam[0].Param[1].isFract = 1;
  
  //Frequency
  Channel[1].SignalParam[0].Param[2].min = 0.1;   
  Channel[1].SignalParam[0].Param[2].max = 10000.0;   
  Channel[1].SignalParam[0].Param[2].cur = 100.0;   
  Channel[1].SignalParam[0].Param[2].step = 1.0;
  Channel[1].SignalParam[0].Param[2].pacronym = Acronym_F;
  Channel[1].SignalParam[0].Param[2].punit = Unit_Hz;
  Channel[1].SignalParam[0].Param[2].bg_pos_x  = 13;
  Channel[1].SignalParam[0].Param[2].bg_pos_y  = 3;
  Channel[1].SignalParam[0].Param[2].isVisible = 1;
  Channel[1].SignalParam[0].Param[2].format = Hz_format;
  Channel[1].SignalParam[0].Param[2].isAvailable = 1;
  Channel[1].SignalParam[0].Param[2].index_together = 0;
  Channel[1].SignalParam[0].Param[2].isSign = 0;
  Channel[1].SignalParam[0].Param[2].numDig = 6;
  Channel[1].SignalParam[0].Param[2].isFract = 0;

  //Duty
  Channel[1].SignalParam[0].Param[3].isAvailable = 0;
  Channel[1].SignalParam[0].Param[3].isVisible = 0;

  //SignalParam 1  pulse  
  Channel[1].SignalParam[1].pname = SigName_1;
  Channel[1].SignalParam[1].name_pos_y = 1;
  
  //Amplitude
  Channel[1].SignalParam[1].Param[0].min = 0.0;
  Channel[1].SignalParam[1].Param[0].max = 5.0;
  Channel[1].SignalParam[1].Param[0].cur = 0.0;   
  Channel[1].SignalParam[1].Param[0].step = 0.1;
  Channel[1].SignalParam[1].Param[0].pacronym = Acronym_A;
  Channel[1].SignalParam[1].Param[0].punit = Unit_B;
  Channel[1].SignalParam[1].Param[0].bg_pos_x  = 15;
  Channel[1].SignalParam[1].Param[0].bg_pos_y  = 2;
  Channel[1].SignalParam[1].Param[0].isVisible = 0;
  Channel[1].SignalParam[1].Param[0].format = V_format;
  Channel[1].SignalParam[1].Param[0].isAvailable = 1;
  Channel[1].SignalParam[1].Param[0].index_together = 1;
  Channel[1].SignalParam[1].Param[0].isSign = 0;
  Channel[1].SignalParam[1].Param[0].numDig = 2;
  Channel[1].SignalParam[1].Param[0].isFract = 1;
   
  //Offset
  Channel[1].SignalParam[1].Param[1].min = 0.0;
  Channel[1].SignalParam[1].Param[1].max = 5.0;
  Channel[1].SignalParam[1].Param[1].cur = 1.0;   
  Channel[1].SignalParam[1].Param[1].step = 0.1;
  Channel[1].SignalParam[1].Param[1].pacronym = Acronym_O;
  Channel[1].SignalParam[1].Param[1].punit = Unit_B;
  Channel[1].SignalParam[1].Param[1].bg_pos_x  = 15;
  Channel[1].SignalParam[1].Param[1].bg_pos_y  = 2;
  Channel[1].SignalParam[1].Param[1].isVisible = 1;
  Channel[1].SignalParam[1].Param[1].format = V_format;
  Channel[1].SignalParam[1].Param[1].isAvailable = 1;
  Channel[1].SignalParam[1].Param[1].index_together = 0;
  Channel[1].SignalParam[1].Param[1].isSign = 1;
  Channel[1].SignalParam[1].Param[1].numDig = 2;
  Channel[1].SignalParam[1].Param[1].isFract = 1;
   
  //Frequency
  Channel[1].SignalParam[1].Param[2].min = 0.1;
  Channel[1].SignalParam[1].Param[2].max = 10000.0;   
  Channel[1].SignalParam[1].Param[2].cur = 0.0;   
  Channel[1].SignalParam[1].Param[2].step = 1.0;
  Channel[1].SignalParam[1].Param[2].pacronym = Acronym_F;
  Channel[1].SignalParam[1].Param[2].punit = Unit_Hz;
  Channel[1].SignalParam[1].Param[2].bg_pos_x  = 13;
  Channel[1].SignalParam[1].Param[2].bg_pos_y  = 3;
  Channel[1].SignalParam[1].Param[2].isVisible = 0;
  Channel[1].SignalParam[1].Param[2].format = Hz_format;
  Channel[1].SignalParam[1].Param[2].isAvailable = 1;
  Channel[1].SignalParam[1].Param[2].index_together = 3;
  Channel[1].SignalParam[1].Param[2].isSign = 0;
  Channel[1].SignalParam[1].Param[2].numDig = 6;
  Channel[1].SignalParam[1].Param[2].isFract = 0;

  //Duty
  Channel[1].SignalParam[1].Param[3].min = 20;
  Channel[1].SignalParam[1].Param[3].max = 100.0; 
  Channel[1].SignalParam[1].Param[3].cur = 50;   
  Channel[1].SignalParam[1].Param[3].step = 1; 
  Channel[1].SignalParam[1].Param[3].pacronym = Acronym_D;
  Channel[1].SignalParam[1].Param[3].punit = Unit_percent;
  Channel[1].SignalParam[1].Param[3].bg_pos_x  = 16;
  Channel[1].SignalParam[1].Param[3].bg_pos_y  = 3;
  Channel[1].SignalParam[1].Param[3].isVisible = 1;
  Channel[1].SignalParam[1].Param[3].format = Perc_format;
  Channel[1].SignalParam[1].Param[3].isAvailable = 1;
  Channel[1].SignalParam[1].Param[3].index_together = 2;
  Channel[1].SignalParam[1].Param[3].isSign = 0;
  Channel[1].SignalParam[1].Param[3].numDig = 3;
  Channel[1].SignalParam[1].Param[3].isFract = 0;
   
  //SignalParam 2  exp
  Channel[1].SignalParam[2].pname = SigName_2;
  Channel[1].SignalParam[2].name_pos_y = 1;
  
  //Amplitude
  Channel[1].SignalParam[2].Param[0].min = 0.1;
  Channel[1].SignalParam[2].Param[0].max = 5.0;
  Channel[1].SignalParam[2].Param[0].cur = 1.0;   
  Channel[1].SignalParam[2].Param[0].step = 0.1;
  Channel[1].SignalParam[2].Param[0].pacronym = Acronym_A;
  Channel[1].SignalParam[2].Param[0].punit = Unit_B;
  Channel[1].SignalParam[2].Param[0].bg_pos_x  = 15;
  Channel[1].SignalParam[2].Param[0].bg_pos_y  = 2;
  Channel[1].SignalParam[2].Param[0].isVisible = 0;
  Channel[1].SignalParam[2].Param[0].format = V_format;
  Channel[1].SignalParam[2].Param[0].isAvailable = 1;
  Channel[1].SignalParam[2].Param[0].index_together = 1;
  Channel[1].SignalParam[2].Param[0].isSign = 0;
  Channel[1].SignalParam[2].Param[0].numDig = 2;
  Channel[1].SignalParam[2].Param[0].isFract = 1;
   
  //Offset
  Channel[1].SignalParam[2].Param[1].min = 0.0;
  Channel[1].SignalParam[2].Param[1].max = 4.9;
  Channel[1].SignalParam[2].Param[1].cur = 1.0;   
  Channel[1].SignalParam[2].Param[1].step = 0.1;
  Channel[1].SignalParam[2].Param[1].pacronym = Acronym_O;
  Channel[1].SignalParam[2].Param[1].punit = Unit_B;
  Channel[1].SignalParam[2].Param[1].bg_pos_x  = 15;
  Channel[1].SignalParam[2].Param[1].bg_pos_y  = 2;
  Channel[1].SignalParam[2].Param[1].isVisible = 1;
  Channel[1].SignalParam[2].Param[1].format = V_format;
  Channel[1].SignalParam[2].Param[1].isAvailable = 1;
  Channel[1].SignalParam[2].Param[1].index_together = 0;
  Channel[1].SignalParam[2].Param[1].isSign = 1;
  Channel[1].SignalParam[2].Param[1].numDig = 2;
  Channel[1].SignalParam[2].Param[1].isFract = 1;
   
  //Frequency
  Channel[1].SignalParam[2].Param[2].min = 0.1;
  Channel[1].SignalParam[2].Param[2].max = 10000.0;   
  Channel[1].SignalParam[2].Param[2].cur = 1.0;   
  Channel[1].SignalParam[2].Param[2].step = 1.0;
  Channel[1].SignalParam[2].Param[2].pacronym = Acronym_F;
  Channel[1].SignalParam[2].Param[2].punit = Unit_Hz;
  Channel[1].SignalParam[2].Param[2].bg_pos_x  = 13;
  Channel[1].SignalParam[2].Param[2].bg_pos_y  = 3;
  Channel[1].SignalParam[2].Param[2].isVisible = 0;
  Channel[1].SignalParam[2].Param[2].format = Hz_format;
  Channel[1].SignalParam[2].Param[2].isAvailable = 1;
  Channel[1].SignalParam[2].Param[2].index_together = 3;
  Channel[1].SignalParam[2].Param[2].isSign = 0;
  Channel[1].SignalParam[2].Param[2].numDig = 6;
  Channel[1].SignalParam[2].Param[2].isFract = 0;

  //Duty
  Channel[1].SignalParam[2].Param[3].min = 0.0;
  Channel[1].SignalParam[2].Param[3].max = 100.0; 
  Channel[1].SignalParam[2].Param[3].cur = 45;   
  Channel[1].SignalParam[2].Param[3].step = 1; 
  Channel[1].SignalParam[2].Param[3].pacronym = Acronym_D;
  Channel[1].SignalParam[2].Param[3].punit = Unit_percent;
  Channel[1].SignalParam[2].Param[3].bg_pos_x  = 16;
  Channel[1].SignalParam[2].Param[3].bg_pos_y  = 3;
  Channel[1].SignalParam[2].Param[3].isVisible = 1;
  Channel[1].SignalParam[2].Param[3].format = Perc_format;
  Channel[1].SignalParam[2].Param[3].isAvailable = 1;
  Channel[1].SignalParam[2].Param[3].index_together = 2;
  Channel[1].SignalParam[2].Param[3].isSign = 0;
  Channel[1].SignalParam[2].Param[3].numDig = 3;
  Channel[1].SignalParam[2].Param[3].isFract = 0;
   
  //SignalParam 3  sawtooth
  Channel[1].SignalParam[3].pname = SigName_3;
  Channel[1].SignalParam[3].name_pos_y = 1;
  
  //Amplitude
  Channel[1].SignalParam[3].Param[0].min = 0.0;
  Channel[1].SignalParam[3].Param[0].max = 5.0;
  Channel[1].SignalParam[3].Param[0].cur = 1.5;   
  Channel[1].SignalParam[3].Param[0].step = 0.1;
  Channel[1].SignalParam[3].Param[0].pacronym = Acronym_A;
  Channel[1].SignalParam[3].Param[0].punit = Unit_B;
  Channel[1].SignalParam[3].Param[0].bg_pos_x  = 15;
  Channel[1].SignalParam[3].Param[0].bg_pos_y  = 2;
  Channel[1].SignalParam[3].Param[0].isVisible = 0;
  Channel[1].SignalParam[3].Param[0].format = V_format;
  Channel[1].SignalParam[3].Param[0].isAvailable = 1;
  Channel[1].SignalParam[3].Param[0].index_together = 1;
  Channel[1].SignalParam[3].Param[0].isSign = 0;
  Channel[1].SignalParam[3].Param[0].numDig = 2;
  Channel[1].SignalParam[3].Param[0].isFract = 1;
   
  //Offset
  Channel[1].SignalParam[3].Param[1].min = 0.0;
  Channel[1].SignalParam[3].Param[1].max = 5.0;
  Channel[1].SignalParam[3].Param[1].cur = 2.0;   
  Channel[1].SignalParam[3].Param[1].step = 0.1;
  Channel[1].SignalParam[3].Param[1].pacronym = Acronym_O;
  Channel[1].SignalParam[3].Param[1].punit = Unit_B;
  Channel[1].SignalParam[3].Param[1].bg_pos_x  = 15;
  Channel[1].SignalParam[3].Param[1].bg_pos_y  = 2;
  Channel[1].SignalParam[3].Param[1].isVisible = 1;
  Channel[1].SignalParam[3].Param[1].format = V_format;
  Channel[1].SignalParam[3].Param[1].isAvailable = 1;
  Channel[1].SignalParam[3].Param[1].index_together = 0;
  Channel[1].SignalParam[3].Param[1].isSign = 1;
  Channel[1].SignalParam[3].Param[1].numDig = 2;
  Channel[1].SignalParam[3].Param[1].isFract = 1;
   
  //Frequency
  Channel[1].SignalParam[3].Param[2].min = 0.1;
  Channel[1].SignalParam[3].Param[2].max = 10000.0;   
  Channel[1].SignalParam[3].Param[2].cur = 0.1;   
  Channel[1].SignalParam[3].Param[2].step = 1.0;
  Channel[1].SignalParam[3].Param[2].pacronym = Acronym_F;
  Channel[1].SignalParam[3].Param[2].punit = Unit_Hz;
  Channel[1].SignalParam[3].Param[2].bg_pos_x  = 13;
  Channel[1].SignalParam[3].Param[2].bg_pos_y  = 3;
  Channel[1].SignalParam[3].Param[2].isVisible = 1;
  Channel[1].SignalParam[3].Param[2].format = Hz_format;
  Channel[1].SignalParam[3].Param[2].isAvailable = 1;
  Channel[1].SignalParam[3].Param[2].index_together = 0;
  Channel[1].SignalParam[3].Param[2].isSign = 0;
  Channel[1].SignalParam[3].Param[2].numDig = 5;
  Channel[1].SignalParam[3].Param[2].isFract = 0;
 
  //Duty
  Channel[1].SignalParam[3].Param[3].isAvailable = 0;
  Channel[1].SignalParam[3].Param[3].isVisible = 0;
   
  //SignalParam 4  gaussian noise
  Channel[1].SignalParam[4].pname = SigName_4;
  Channel[1].SignalParam[4].name_pos_y = 1;

  //Mean
  Channel[1].SignalParam[4].Param[4].min = 0.0;
  Channel[1].SignalParam[4].Param[4].max = 5.0;
  Channel[1].SignalParam[4].Param[4].cur = 1.5;   
  Channel[1].SignalParam[4].Param[4].step = 0.1;
  Channel[1].SignalParam[4].Param[4].pacronym = Acronym_M;
  Channel[1].SignalParam[4].Param[4].punit = Unit_B;
  Channel[1].SignalParam[4].Param[4].bg_pos_x  = 15;
  Channel[1].SignalParam[4].Param[4].bg_pos_y  = 2;
  Channel[1].SignalParam[4].Param[4].isVisible = 1;
  Channel[1].SignalParam[4].Param[4].format = V_format;
  Channel[1].SignalParam[4].Param[4].isAvailable = 1;
  Channel[1].SignalParam[4].Param[4].index_together = 0;
  Channel[1].SignalParam[4].Param[4].isSign = 1;
  Channel[1].SignalParam[4].Param[4].numDig = 2;
  Channel[1].SignalParam[4].Param[4].isFract = 1;

  //Variance (Standard Deviation)
  Channel[1].SignalParam[4].Param[5].min = 0.0;
  Channel[1].SignalParam[4].Param[5].max = 5.0;
  Channel[1].SignalParam[4].Param[5].cur = 1.5;   
  Channel[1].SignalParam[4].Param[5].step = 0.1;
  Channel[1].SignalParam[4].Param[5].pacronym = Acronym_var;
  Channel[1].SignalParam[4].Param[5].punit = Unit_B;
  Channel[1].SignalParam[4].Param[5].bg_pos_x  = 15;
  Channel[1].SignalParam[4].Param[5].bg_pos_y  = 3;
  Channel[1].SignalParam[4].Param[5].isVisible = 1;
  Channel[1].SignalParam[4].Param[5].format = V_format;
  Channel[1].SignalParam[4].Param[5].isAvailable = 1;
  Channel[1].SignalParam[4].Param[5].index_together = 0;
  Channel[1].SignalParam[4].Param[5].isSign = 0;
  Channel[1].SignalParam[4].Param[5].numDig = 2;
  Channel[1].SignalParam[4].Param[5].isFract = 1;
  
  //SignalParam 5  uniform noise
  Channel[1].SignalParam[5].pname = SigName_5;
  Channel[1].SignalParam[5].name_pos_y = 1;
  
  //Mean
  Channel[1].SignalParam[5].Param[4].min = 0.0;
  Channel[1].SignalParam[5].Param[4].max = 5.0;
  Channel[1].SignalParam[5].Param[4].cur = 1.5;   
  Channel[1].SignalParam[5].Param[4].step = 0.1;
  Channel[1].SignalParam[5].Param[4].pacronym = Acronym_M;
  Channel[1].SignalParam[5].Param[4].punit = Unit_B;
  Channel[1].SignalParam[5].Param[4].bg_pos_x  = 15;
  Channel[1].SignalParam[5].Param[4].bg_pos_y  = 2;
  Channel[1].SignalParam[5].Param[4].isVisible = 1;
  Channel[1].SignalParam[5].Param[4].format = V_format;
  Channel[1].SignalParam[5].Param[4].isAvailable = 1;
  Channel[1].SignalParam[5].Param[4].index_together = 0;
  Channel[1].SignalParam[5].Param[4].isSign = 1;
  Channel[1].SignalParam[5].Param[4].numDig = 2;
  Channel[1].SignalParam[5].Param[4].isFract = 1;

  //Variance (Standard Deviation)
  Channel[1].SignalParam[5].Param[5].min = 0.0;
  Channel[1].SignalParam[5].Param[5].max = 5.0;
  Channel[1].SignalParam[5].Param[5].cur = 1.5;   
  Channel[1].SignalParam[5].Param[5].step = 0.1;
  Channel[1].SignalParam[5].Param[5].pacronym = Acronym_var;
  Channel[1].SignalParam[5].Param[5].punit = Unit_B;
  Channel[1].SignalParam[5].Param[5].bg_pos_x  = 15;
  Channel[1].SignalParam[5].Param[5].bg_pos_y  = 3;
  Channel[1].SignalParam[5].Param[5].isVisible = 1;
  Channel[1].SignalParam[5].Param[5].format = V_format;
  Channel[1].SignalParam[5].Param[5].isAvailable = 1;
  Channel[1].SignalParam[5].Param[5].index_together = 0;
  Channel[1].SignalParam[5].Param[5].isSign = 0;
  Channel[1].SignalParam[5].Param[5].numDig = 2;
  Channel[1].SignalParam[5].Param[5].isFract = 1;
 
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //+++++++++++++++++++ Отображаем начальную картину на LCD ++++++++++++++++++++
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
  ReDrawZeroRow();
  re_draw_body();

  //выключаем курсор
  blink_control(0);
  
  //начальная картина на выходах ЦАПа
  preInitDAC(&(Channel[0]), 0);
  preInitDAC(&(Channel[1]), 1);	

  // switch 'on' measuring
  meas_control( 1 );
}

//menu state machine
// ButNum - номер кнопки    (0-7, 0-11)
// EventType - тип события  (1 - ButtonOn, 2 - ButtonPress)
// isKeyPad - тип клавиатуры(1 - KeyPad, 0 - MatrixPad)
//==============================================================================
void MenuStateMach(char ButNum, char EventType, char isKeyPad)
{
  // blocking buttons, except '+' ans '-', in measuring mode
  if( ( is_meas_mode ) && ( ButNum != 6 ) && ( ButNum != 7 ) )
  {
    return;
  }
   
  sSignalParam tmpSig;

  // обработка нажатий KeyPad
  switch( ButNum )
  {
    case 0:          //кнопка "Channel select"
    {
      //выходим из режима редактирования
      EditMode = 0; 
      
      //перерисовываем текущий параметр редактирования, для возврата к значению параметра до редактирования,
      //если не было потверждено его изменение или отображение изменения, если  потверждение было
      ReDrawParam( &Channel[CurChannel].SignalParam[Channel[CurChannel].CurSigForm].Param[CurEditParam] );
    
      //сбрасываем состояние кнопок
      for( int i = 0; i < 6; i++ )
      {
        StateButton[i] = 0;
      }
    
      //выключаем курсор
      blink_control(0);
      
      //отображаем, на LCD, текущий канал
      if( CurChannel == 0 )     //Channel 1
      {
        CurChannel = 1;
      }
      else if( CurChannel == 1 )//Channel 2
      {
        CurChannel = 0;
      }    
      ReDrawZeroRow();
    }
    break;
          
    case 1:          //кнопка "Signal Form"
    {
      //выходим из режима редактирования
      EditMode = 0;
    
      //сбрасываем состояние кнопок
      for( int i = 0; i < 6; i++ )
      {
        StateButton[i] = 0;
      }
    
      //выключаем курсор
      blink_control( 0 );
    
      //"закругляем" перебор типов сигналов
      Channel[CurChannel].CurSigForm++;
      if( Channel[CurChannel].CurSigForm >= 6 )
      {
        Channel[CurChannel].CurSigForm = 0;
      }
    
      tmpSig = Channel[CurChannel].SignalParam[Channel[CurChannel].CurSigForm];
    
      //отображаем, на LCD, имя текущего сигнала текущего канала
      lcd_write( "         ",  Channel[CurChannel].pos_x, tmpSig.name_pos_y );
      lcd_write( tmpSig.pname, Channel[CurChannel].pos_x, tmpSig.name_pos_y );
    
      //отображаем, на LCD, видимые, в данный момент, параметры текущего сигнала текущего канала
      for( int i = 0; i < 6; i++ )
      {
        ReDrawParam( &(tmpSig.Param[i]) );
      }
      
      preInitDAC( &(Channel[CurChannel]), CurChannel );
    }
    break;
          
    case 2:          //кнопка "A/sigma" 
    {
      //выходим из режима редактирования
      EditMode = 0;
      
      //переходим в режим редактирования и переводим курсор
      Switch_to_EditMode( 0 );  
    }
    break;

    case 3:          //кнопка "O/m"
    {
      //выходим из режима редактирования
      EditMode = 0;
      
      //переходим в режим редактирования и переводим курсор
      Switch_to_EditMode( 1 );  
    }
    break;

    case 4:          //кнопка "F"
    {
      //выходим из режима редактирования
      EditMode = 0;
      
      //переходим в режим редактирования и переводим курсор
      Switch_to_EditMode( 2 );  
    }
    break;

    case 5:          //кнопка "D"
    {
      //выходим из режима редактирования
      EditMode = 0;
      
      //переходим в режим редактирования и переводим курсор
      Switch_to_EditMode( 3 );  
    }
    break;

    case 6:          //кнопка "+"
    {
      if( EditMode == 1 )
      {
        EditParam( 12 );
      }
      else
      {  
        if( is_meas_mode == 0 )
        {        
          // switch to measuring mode
          is_meas_mode = 1;
          
          // Unlocking sending message from DMA1 Channel 1 ISR to Calc thread
          lock_send_message_to_calc_thread = 0;    
        }
        else
        {
          // switch to generation mode
          is_meas_mode = 0;
                   
          // locking sending message from DMA1 Channel 1 ISR to Calc thread
          lock_send_message_to_calc_thread = 1;
               
        }//else
        
        ReDrawZeroRow();
        re_draw_body(); 
      }// else 
    }
    break;

    case 7:          //кнопка "-"
    {
      if( EditMode == 1 )
      {
        EditParam( 13 );
      }
      else
      {                        
        // sends a message SD_EVENT to sd_thread
        sd_control();
      }
    }   
    break;
    
    default:
    break;   
  }//switch(ButNum) 
}

// Переключение в режим редактирования, изменение видимости, перерисовка и перевод курсора на место редактирования,
//если текущий тип сигнала поддерживает данный параметр редактирования
// param - параметр редактирования ( 0 - A, 1 - O, 2 - F, 3 - D),
//если текущий тип сигнала - шум, то 0 - m, 1 - var(sigma)
//==============================================================================
void Switch_to_EditMode( int param )
{  
  sParam *tempParam;
  sSignalParam *tmpSig; 
  char spacestr[9];

  memset( spacestr, 0x20, sizeof( spacestr ) );
  
  tmpSig = &(Channel[CurChannel].SignalParam[Channel[CurChannel].CurSigForm]);

  //если текущий тип сигнала - шум
  if( ( Channel[CurChannel].CurSigForm == 4 ) ||
      ( Channel[CurChannel].CurSigForm == 5 )
    )
  {
    // у шума только два доступных параметра к редактированию
    if( param == 0 )
    {
      param = 4;
    }
    else if( param == 1 )
    {
      param = 5;
    }
    else // данный тип параметра недоступен текущему типу сигнала 
    {
      //перерисовываем текущий параметр редактирования, для возврата к значению параметра до редактирования,
      //если не было потверждено его изменение или отображение изменения, если  потверждение было
      ReDrawParam( &Channel[CurChannel].SignalParam[Channel[CurChannel].CurSigForm].Param[CurEditParam] );

      //сбрасываем состояние кнопок
      for( int i = 0; i < 6; i++ )
      {
        StateButton[i] = 0;
      }

      //выключаем курсор
      blink_control( 0 ); 
      
      return;
    }
    
    //имеем дело с параметром редактирования, соответственно нажатой кнопки
    tempParam = &(tmpSig->Param[param]); 
  }
  else
  {    
    //имеем дело с параметром редактирования, соответственно нажатой кнопки
    tempParam = &(tmpSig->Param[param]); 

    // данный тип параметра недоступен текущему типу сигнала
    if( tempParam->isAvailable != 1 )
    {
      //перерисовываем текущий параметр редактирования, для возврата к значению параметра до редактирования,
      //если не было потверждено его изменение или отображение изменения, если  потверждение было
      ReDrawParam( &Channel[CurChannel].SignalParam[Channel[CurChannel].CurSigForm].Param[CurEditParam] );

      //сбрасываем состояние кнопок
      for(int i = 0; i < 6; i++)
      {
        StateButton[i] = 0;
      }

      //выключаем курсор
      blink_control(0);

      return; 
    } 
  }
  
  // если дошли досюда, то текущий параметр доступен к редактированию, нужно его сделать видимым,
  // если он невидим, отобразить его и перевести курсор на знакоместо, с которого начнется редактирование
  // также нужно перейти в режим редактирования
  
  //переходим в режим редактирования
  EditMode = 1;

  //если параметр редактирования не видим, то нужно сделать его видимым, отобразить его на LCD
  //и сделать параметр, с которым делится строчка LCD, невидимым
  if( tempParam->isVisible != 1 )
  {
    tempParam->isVisible = 1;
    
    //делаем невидимым параметр, с которым текущий параметр делит строчку на LCD
    tmpSig->Param[tempParam->index_together].isVisible = 0;
	
    //отображаем редактируемый параметр 
    ReDrawParam( tempParam );            
  }

  //сбрасываем состояние кнопок, кроме текущей
  for( int i = 0; i < 6; i++ )
  {
    if(i != param)
    {
      StateButton[i] = 0;
    }
  }

  //состояние инкремент/декремент
  if( StateButton[param] == 0 )
  {
    // рисуем измененнный параметр на LCD
    ReDrawParam( &(tmpSig->Param[CurEditParam]) );	  
    
    StateButton[param] = 1;

    // инициализируем переменные для state machine редактирования параметров, 
    // подрежим инкремент/декремент
    CurEditParam = param;
    PrevValue = tempParam->cur;
  }
  else if( StateButton[param] == 1 ) //состояние ввода цифр (now it apply-state)
  {
    StateButton[param] = 0;
    
    // applying changes
    EditParam( 9 );
    return;
  }
  else if(StateButton[param] == 2);

  //включаем курсор
  blink_control( 1 );

  //переводим курсор на позицию редактирования
  shift_blink( tempParam->bg_pos_x, tempParam->bg_pos_y ); 
}

//редактирование параметра
//ButNum - цифры(для "1" - 0, для "9" - 8, для delete - 9, для "0" - 10, для "." - 11, для "+" - 12, для "-" - 13)
//==============================================================================
void EditParam( char ButNum )
{
  float temp;
  sParam *ptempParam, tempParam;

  temp = PrevValue;
  
  //информация о текущем редактируемом параметре
  ptempParam = &(Channel[CurChannel].SignalParam[Channel[CurChannel].CurSigForm].Param[CurEditParam]);
  tempParam = (Channel[CurChannel].SignalParam[Channel[CurChannel].CurSigForm].Param[CurEditParam]);
 
  if( ButNum == 9 )	              //если нажата кнопки enter(*)
  {
    //вносим изменения в глобальную структуру
    ptempParam->cur = PrevValue;

    // рисуем измененнный параметр на LCD
    ReDrawParam( ptempParam );	  
    
    //сбрасываем состояние кнопок
    for( int i = 0; i < 6; i++ )
    {
      StateButton[i] = 0;
    }

    //выходим из режима редактирования
    EditMode = 0;

    //выключаем курсор
    blink_control(0);
    
    preInitDAC( &(Channel[CurChannel]), CurChannel );
    
    return;
  }
  
  if( ButNum == 12 )	              //если нажата кнопки "+"
  {
    temp += tempParam.step;
    if( ( temp <= tempParam.max ) && ( temp >= tempParam.min ) )
    {
      PrevValue = temp;             // для следующих заходов в EditParam
      tempParam.cur = temp;         // меняем локальную копию, только для рисования 
      ReDrawParam( &tempParam );	  // рисуем измененнный параметр на LCD

      //переводим курсор на позицию редактирования
      shift_blink( tempParam.bg_pos_x, tempParam.bg_pos_y ); 
    }
  }
  else if( ButNum == 13 )	          //если нажата кнопки "-"
  {
    temp -= tempParam.step;
    if( ( temp <= tempParam.max ) && ( temp >= tempParam.min ) )
    {
      PrevValue = temp;             // для следующих заходов в EditParam
      tempParam.cur = temp;         // меняем локальную копию, только для рисования 
      ReDrawParam(&tempParam);	  // рисуем измененнный параметр на LCD
  
      //переводим курсор на позицию редактирования
      shift_blink(tempParam.bg_pos_x, tempParam.bg_pos_y); 
    }
  }
}

//перерисовывает строчку-параметр текущего канала, если он[параметр] доступен и видим
//pParam - указатель на строчку-параметр, которую нужно перерисовать
//==============================================================================
void ReDrawParam(const sParam *pParam)
{
  char temp[21];

  if( ( pParam->isVisible == 1 )&& ( pParam->isAvailable == 1 ) )
  {
	*temp = 0;
	strncat( temp, pParam->pacronym, strlen(pParam->pacronym) );
	sprintf( temp + strlen(pParam->pacronym), pParam->format, pParam->cur );
	
    //ограничение для частоты
	if( pParam->cur <= 99999.0 )
    {
	  strncat( temp, pParam->punit, strlen(pParam->punit) );
    }
	else
    {
	  strncat( temp, pParam->punit, strlen(pParam->punit) - 1 );
    }
        
	lcd_write( "         ", Channel[CurChannel].pos_x,  pParam->bg_pos_y );
	lcd_write( temp, Channel[CurChannel].pos_x, pParam->bg_pos_y );                 
  }
}

//перерисовывает нулевую строку LCD(имена каналов)
//с учетом текущего выбранного канала,так же учитывается включен или выключен канал
//==============================================================================
void ReDrawZeroRow(void)
{ 
  if( 1 == is_meas_mode )
  {
    lcd_write("   Ch1   ", Channel[0].pos_x, 0);
    lcd_write("   Ch2   ", Channel[1].pos_x, 0);
    return;
  }

  //отображаем, на LCD, текущий канал
  if(CurChannel == 0)
  {
    lcd_write("+++Ch1+++", Channel[0].pos_x, 0);
    lcd_write("   Ch2   ", Channel[1].pos_x, 0);
  }
  else if(CurChannel == 1)
  {
    lcd_write("   Ch1   ", Channel[0].pos_x, 0);
    lcd_write("+++Ch2+++", Channel[1].pos_x, 0);
  }
}
//==============================================================================
void re_draw_body( void )
{
  char temp[21] = {0, };
  sParam tempParam;
  sSignalParam tmpSig;

  // if we must draw body for measuring power
  if( 1 == is_meas_mode )
  {
    lcd_write( "P=     mW", 0, 1 );          
    lcd_write( "P=     mW", 11, 1 );
            
    lcd_write( "I=     mA", 0, 2 );
    lcd_write( "I=     mA", 11, 2 );
            
    lcd_write( "U=      V", 0, 3 );
    lcd_write( "U=      V", 11, 3 );
  }
  else if( 0 == is_meas_mode ) // if must draw body for generation
  {
    for(int k = 0; k < 2; k++)
    {
      tmpSig  = Channel[k].SignalParam[Channel[k].CurSigForm];
    	 
	  //отображаем, на LCD, имя текущего сигнала k-канала
      lcd_write("         ",  Channel[k].pos_x, tmpSig.name_pos_y);
      lcd_write(tmpSig.pname, Channel[k].pos_x, tmpSig.name_pos_y);
    	  
	  //отображаем, на LCD, видимые, в данный момент, параметры текущего сигнала текущего канала     
      for(int i = 0; i < 6; i++)
      {
        tempParam = tmpSig.Param[i];
        if(tempParam.isVisible == 1)
        {
          *temp = 0;
          strncat(temp, tempParam.pacronym, strlen(tempParam.pacronym));
          sprintf(temp + strlen(tempParam.pacronym), tempParam.format, tempParam.cur);
	      
          //ограничение для частоты
		  if(tempParam.cur < 99999.0)
		    strncat(temp, tempParam.punit, strlen(tempParam.punit));
		  else
		    strncat(temp, tempParam.punit, strlen(tempParam.punit) - 1);

          lcd_write("         ", Channel[k].pos_x,  tempParam.bg_pos_y);
          lcd_write(temp, Channel[k].pos_x, tempParam.bg_pos_y);                 
        }
      }
    }//for(int k = 0; k < 2; k++)
  }// else if ( 0 == is_meas_mode )
}