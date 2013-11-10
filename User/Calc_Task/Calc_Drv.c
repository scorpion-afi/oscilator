// signal measuring driver

#define CalcDrv	
#include "Calc_Drv.h"

#include "stdio.h"

// lcd_write is defined in Menu_Task.c
// string by pStr might will be changed, so it can not be in read-only memory
extern void lcd_write( char *pStr, char x, char y );

// shows result of measuring:
// power, current and voltage for two channels
// i_ch1, i_ch2 - value of currents in mA
// u_ch1, u_ch2 - value of voltages in V
// powers will be printed in mW 
//==============================================================================
void show_result( float i_ch1, float u_ch1, float i_ch2, float u_ch2 )
{ 
  char temp_str[21]; 
  
  // prints power of channel 1
  sprintf( temp_str, "%5.2f", i_ch1*u_ch1 );
  lcd_write( temp_str, 2, 1 );
  
  // prints power of channel 2
  sprintf( temp_str, "%5.2f", i_ch2*u_ch2 );
  lcd_write( temp_str, 13, 1 );
  
  // prints current of channel 1
  sprintf( temp_str, "%5.1f", i_ch1 );
  lcd_write( temp_str, 2, 2 );
  
  // prints current of channel 2
  sprintf( temp_str, "%5.1f", i_ch2 );
  lcd_write( temp_str, 13, 2 );
  
  // prints voltage of channel 1
  sprintf( temp_str, "% 5.3f", u_ch1 );
  lcd_write( temp_str, 2, 3 );
  
  // prints voltage of channel 2
  sprintf( temp_str, "% 5.3f", u_ch2 );
  lcd_write( temp_str, 13, 3 ); 
}