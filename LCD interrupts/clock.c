#include "LCD.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "SetClock.h"
#include "Interrupts.h"

static uint8_t seconds;
static uint8_t minutes;
static uint8_t hours;
static char save[2];

int clock_master;

void clock(void)
{           
   switch (clock_master)
   {
      case 0:

      LCD_goto(1,1);
      LCD_str("--:--:--"); 
      break;

      case 1:

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

void clockUpdate(uint8_t h, uint8_t m, uint8_t s)
{
   hours = h;
   minutes = m;
   seconds = s;
}


