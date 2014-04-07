#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include "LCD.h"
#include "Keyboard.h"
#include "ADC.h"
#include "Interrupts.h"
#include "clock.h"
#include <stdbool.h>

// RTOS timer intervals:

#define clock_interval 1000;         
#define LDR_interval 200; 
#define temp_interval 500;
#define key_interval 20;
#define backlight_interval 20;

// RTOS timer variables: 

int clock_run;
int clock_count;

int LDR_run;
int LDR_count;

int temp_run;
int temp_count;

int key_run;
int key_count;

int backlight_run;
int backlight_count;

// Initialize timer, interrupt and variable
void timer1_init()
{   
   // set up timer with prescaler = 64 and CTC mode
   TCCR1B |= (1 << CS11) | (1 << CS10) | (1 << WGM12);

   // initialize counter
   TCNT1 = 0;

   // initialize compare value
   OCR1A = 250;    //1ms = 16.000Hz / 64 = 250 cycles

   // enable compare interrupt
   TIMSK |= (1 << OCIE1A);

   // enable global interrupts
   sei();
}

// this ISR is fired whenever a match occurs
ISR (TIMER1_COMPA_vect)
{  
   if(clock_count-- == 0)
   { 
      clock_count = clock_interval;
      clock_run = true;
   } 

   if(LDR_count-- == 0)
   { 
      LDR_count = LDR_interval;
      LDR_run = true;
   } 

   if(temp_count-- == 0)
   { 
      temp_count = temp_interval;
      temp_run = true;
   } 

   if(key_count-- == 0)
   { 
      key_count = key_interval;
      key_run = true;
   } 

   if(backlight_count-- == 0)
   { 
      backlight_count = backlight_interval;
      backlight_run = true;
   }          
}
