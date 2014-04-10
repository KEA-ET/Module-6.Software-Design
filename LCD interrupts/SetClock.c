#include "LCD.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Interrupts.h"
#include "clock.h"

#define wait_interval 10

static uint8_t setup = false;
static uint8_t wait_timer = 0;
static uint8_t s;
static uint8_t m;
static uint8_t h;
static uint8_t i = 0;
static char num_str[6]; 
static uint32_t number;

void SetClock(char key)
{  
   // timer to avoid multiple key-presses:
   if (wait_timer > 0)
   {
      wait_timer--;
   }

   // enter setup:
   if (key == '*' && wait_timer == 0)
   {
      setup = !setup;
      wait_timer = wait_interval;
   }
   
   switch (setup)
   {
      case 1: // clock stopped
      clock_master = false;

      if (key != 0 && wait_timer == 0) // check for valid input
      {
         wait_timer = wait_interval; // reset timer

         num_str[i] = key; // save key input to string
         i++;
         
      // check if hour, min, and seconds have been entered:
      if (i > 5)
      {
         number = atol(num_str); // convert string to long
         
         // split into separate numbers:
         h = (number / 10000);
         m = ((number % 10000)/100);
         s = (number % 100);

         clockUpdate(h, m, s); // update clock
         setup = false; //exit setup
         i = 0; // reset counter
      }
     }
      break;
         
      case 0: // clock running      
      clock_master = true;
      
      break;
   }
}
