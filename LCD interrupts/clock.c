#include "LCD.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

unsigned char seconds;
unsigned char minutes;
unsigned char hours;

void clock(char key)
{  



   static uint16_t counter = 0;
   static uint8_t toggle = true;
   static uint8_t wait_timer = 0;
   const static uint8_t wait_interval = 5;
   char save[2];

   if (wait_timer > 0)  
   {
      wait_timer--;
   }

   // debug:
   
//   LCD_goto (18,1);
//   LCD_prt(key);

   //toggles clock on/off
   if (key == '*' && wait_timer == 0)
   {
      toggle = !toggle;
      wait_timer = wait_interval; // reset timer
   }
   
   switch (toggle)
   {

      case 0: // clock stopped
      LCD_goto(1,1);
      LCD_str("--:--:--");
      break;
         
      case 1: // clock running

      LCD_goto(1,1); 
      sprintf(save, "%02d", hours);
      LCD_str(save);
      LCD_goto(4,1);
   
      sprintf(save, "%02d", minutes);
      LCD_str(save);
      LCD_goto(7,1);
      sprintf(save, "%02d", seconds);
      LCD_str(save);


      counter++; // 10 counts at 0.1 seconds intervals = 1 seconds
      if (counter%10 == 0)
      {
      seconds = (counter/10);
      }

      if(seconds == 60)
      {
         counter = 0;
         seconds = 0;
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
      break;
   }
}



