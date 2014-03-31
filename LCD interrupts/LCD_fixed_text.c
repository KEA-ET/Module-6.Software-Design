#include "LCD.h"

void fixed_text ()
{
   LCD_goto(3,1);
   LCD_str(":");

   LCD_goto(6,1);
   LCD_str(":");

   LCD_goto(11,1);
   LCD_str("T:");

   LCD_goto(15,1);
   LCD_str(".");

   LCD_goto(1,2);
   LCD_str("LDR:");
}
