#define F_CPU 16000000UL // 16 MHz clock speed

#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "clock.h"
#include "LCD.h"
#include "ADC.h"
#include "DAC.h"
#include "LCD_fixed_text.h"
#include "Interrupts.h"
#include "Keyboard.h"

static char key;

/***********************/
// Main Program
/***********************/

int main (void)
{  
	LCD_init(); // LCD init
	LCD_cmd(0x00); // Blink cursor

   fixed_text();
   timer1_init();

   while(1)
   {  
      // Function run flags:
      if(clock_run)
      {  
         clock();
         clock_run = false;
      }   

      if(LDR_run)
   
      {  
         light();
         LDR_run = false;
      }

      if(temp_run)
      {   
         temp();
         temp_run = false;
      }

      if(key_run)
      {  
         key = KEY_read();
         key_run = false;

      }

       if(backlight_run)
      {  
         BL_dimmer(key);
         backlight_run = false;
      }
     }
}
