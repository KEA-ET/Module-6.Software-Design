#include "LCD.h"
#include <stdio.h>
#include <stdlib.h>

unsigned char seconds;
unsigned char minutes;
unsigned char hours;

void clock()
{  char save[2];
   LCD_goto(1,1);
   sprintf(save, "%02d", hours);
   LCD_str(save);
   LCD_goto(4,1);
   
   sprintf(save, "%02d", minutes);
   LCD_str(save);
   LCD_goto(7,1);
   sprintf(save, "%02d", seconds);
   LCD_str(save);

    seconds++;

    if(seconds == 60)
      {
       seconds= 0;
       minutes++;
      }
    if(minutes == 60)
      {
       minutes = 0;
       hours++;
      }
    if(hours == 24)
      {
       hours=0;
      }
}
