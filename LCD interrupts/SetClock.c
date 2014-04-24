#include "LCD.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Interrupts.h"
#include "clock.h"

#define wait_interval 5

static uint8_t setup = false;
static uint8_t wait_timer = 0;
static uint8_t s;
static uint8_t m;
static uint8_t h;
static uint8_t i = 0;
static uint32_t number;

static char num_str[7]; 
static char save[8]; 

void SetClock(char key)
{  
   // timer to avoid multiple key-presses:
   if (wait_timer > 0)
   {
      wait_timer--;
   }

   // enter/exit setup:
   if (key == '*' && wait_timer == 0)
   {
      setup = !setup;
      wait_timer = wait_interval;
      i = 0;
      num_str[0] = '\0';

   }

   switch (setup)
   {
      case true:
      clock_master = false;// clock stopped

      // fill string with zeros:
      if (strlen(num_str) == 0)
      {
         strcpy(num_str, "000000");
      }

      if (key != 0 && key != '*' && wait_timer == 0) // check for valid input
      {
         wait_timer = wait_interval; // reset timer
         num_str[i] = key; // save key input to string
         i++;
      }

      number = atol(num_str); // convert string to long
         
         // split into separate numbers:
         h = (number / 10000);
         m = ((number % 10000)/100);
         s = (number % 100);

      clockUpdate(h, m, s); // update 

      if (i > 0)
      {
         LCD_goto(1,1); 
         sprintf(save, "%02d:%02d:%02d", h, m, s);
         LCD_str(save);
      }

      else
      { 
         LCD_goto(1,1);
         LCD_str("--:--:--");
      }

      // check if hour, minutes and seconds have been entered:
      if (i == 6)
      {   
         // check if numbers are out of bounds:
         if (h > 23)
         {
            h = 0;
            m = 0;
            s = 0;
            clockUpdate(h, m, s);                 
         }

         if (m > 59)
         {
            h = 0;
            m = 0;
            s = 0;
            clockUpdate(h, m, s);
         }

         if (s > 59)
         {
            h = 0;
            m = 0;
            s = 0;
            clockUpdate(h, m, s);        
         }
         
         setup = false; //exit setup
      } 
      break;
         
      case false: // clock running      
      clock_master = true;
      break;
   }
}
