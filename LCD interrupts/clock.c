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
static char save[8];

int clock_master;

void clock(void)
{           
   if (clock_master == true)
   {

      LCD_goto(1,1); 
      sprintf(save, "%02d:%02d:%02d", hours, minutes, seconds);
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
   }
}

void clockUpdate(uint8_t h, uint8_t m, uint8_t s)
{
   hours = h;
   minutes = m;
   seconds = s;
}


