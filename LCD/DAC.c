#define F_CPU 16000000UL // 16 MHz clock speed
#include <avr/io.h>
#include <util/delay.h>
#include "BitManip.h"

/***********************/
// Using PWM
/***********************/
void pwm_init (void)
{    DDRB |=  0b00001000;        // Set pin 3 on PORTB as output
     TCCR0 = 0b01101001;
     int sinus [21] = {128,168,203,232,250,255,250,232,203,168,128,88,53,24,6,0,6,24,53,88,128};
     while(1)
       {   for(int i=0;i<20;i++)
             { OCR0 = sinus[i];
                  _delay_ms(5);
             }  
       }
}
/***********************/
// Potentiometer Initialization
/***********************/
void potent_init()
{ DDRB |=  0b11111000;           // Set pin 5(data out) and pin 7(clock) on PORTB as output
  SPCR =  0b01010011;             // the control register
  SPSR = SPSR & 0b11111110;        
  _delay_ms(10);
}

/***********************/
// External Digital to Analog Convertor
/***********************/
void potentiometer()
{ int sinus [21] = {128,168,203,232,250,255,250,232,203,168,128,88,53,24,6,0,6,24,53,88,128};
  while(1)
   {  for(int i=0;i<20;i++)         // send data to SPDR
            { bit_clr (PORTB,PB5);           // set CS low
              SPDR =  0b00010001;           // command byte     
              while ((SPSR & 0x80) == 0)
              {}
              SPDR = sinus[i];
              while ((SPSR & 0x80) == 0)
              {}
              bit_set(PORTB,PB5);           // set CS high PINB = 0b00010000;
              _delay_ms(5);
            }
    }
}
