#include <string.h>
#define F_CPU 16000000UL // 16 MHz clock speed
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>
#include "ADC.h"
#include "DAC.h"
#include "BitManip.h"
#include "Interrupts.h"


/***********************/
// Pin Definitions 
/***********************/	
   #define CS 0x10   // CS  Used to start the transmission for potentiometer 
	#define RS 0x04 	// LCD Register Select on PA2
	#define RW 0x08  	// LCD RW on PA3
	#define D0 0x10 	// LCD Data Pin 0 on PA4 
	#define D1 0x20 	// LCD Data Pin 1 on PA5
	#define D2 0x40 	// LCD Data Pin 2 on PA6
	#define D3 0x80  	// LCD Data Pin 3 on PA7
	#define E  0x40 	// LCD E on PC6
	#define BL 0x80	// LCD Backlight on PD7 (via transistor switch)

/***********************/
// Variable Definitions
/***********************/

int LCD_busy;
int useBusy;
int count;

/***********************/
// LCD Command TX
/***********************/
void LCD_cmd (char x)
{
 	int dummy = 0; // Used for small delay

	// Split 8-bit command into 2 bytes with 4 bits each
	char MSB = x & 0xF0; // mask out LSB;
	char LSB = x << 4; // move LSB up to MSB

	// Set RS = 0 to send a command to LCD RS = 1 to send an ASCII value
	bit_clr(PORTA, RS);
	
	// Set RW = 0 to write to LCD, RW = 1 to read from LCD
	bit_clr(PORTA, RW);

	// Set E = 1
	bit_set(PORTC, 0x40);

	// Send MSB on DB4-7
	// PORTA = (PORTA & 0x0F) | MSB
	PORTA &= 0x0F; // Clear MSB bits
	PORTA |= MSB;
	

	// Set E = 0
	bit_clr(PORTC, E);

	dummy++; // Small delay

	// Set E = 1
	bit_set(PORTC, E);
	
	// Send LSB on DB4-7
	PORTA &= 0x0F; // Clear MSB bits
	PORTA |= LSB;
	
	// Set E = 0
	bit_clr(PORTC, E);
	
	_delay_ms(2);
}

/***********************/
// LCD Print TX
/***********************/
void LCD_prt (char x)
{
 	int dummy = 0; // Used for small delay

	// Split 8-bit command into 2 bytes with 4 bits each
	char MSB = x & 0xF0; // mask out LSB;
	char LSB = x << 4; // move LSB up to MSB

	// Set RS = 0 to send a command to LCD RS = 1 to send an ASCII value
	bit_set(PORTA, RS);
	
	// Set RW = 0 to write to LCD, RW = 1 to read from LCD
	bit_clr(PORTA, RW);

	// Set E = 1
	bit_set(PORTC, 0x40);

	// Send MSB on DB4-7
	// PORTA = (PORTA & 0x0F) | MSB
	PORTA &= 0x0F; // Clear MSB bits
	PORTA |= MSB;
	

	// Set E = 0
	bit_clr(PORTC, E);

	dummy++; // Small delay

	// Set E = 1
	bit_set(PORTC, E);
	
	// Send LSB on DB4-7
	PORTA &= 0x0F; // Clear MSB bits
	PORTA |= LSB;
	
	// Set E = 0
	bit_clr(PORTC, E);
	
	_delay_ms(2);
}

/***********************/
// LCD Initialization
/***********************/

void LCD_init (void)
{  
   /* uP Pin definition */
   DDRA = 0xFC; // Pin 2-7: LCD RS, RW, D0-D3
	DDRC = 0x40; // Pin 7: LCD E
	DDRD = 0xFF; //0x80; // Pint 8: LCD Backlight
   
   /* LCD hardware initialization */
	_delay_ms(100);
	LCD_cmd(0x28);
	_delay_ms(10);	
	LCD_cmd(0x28);
	LCD_cmd(0x28);

	LCD_cmd(0x06);
	LCD_cmd(0x0C);
	LCD_cmd(0x01);	
}
/***********************/
// LCD Backlight On/Off
// Input: 0/1
// Output: None
/***********************/

void LCD_BL(int x)
{
	switch(x)
	{
	case 0: // Turn off backlight
	bit_clr(PORTD, BL);
	break;

	case 1: // Turn on backlight
	bit_set(PORTD, BL);
	break;
	}
}

/**************************/
// LCD Print String
// Input: String
// Output: Character
/**************************/

void LCD_str(char *str)
{
	// print char and increment counter until end of string
  	 while(*str > 0)
 	{	
   		LCD_prt(*str++); 
   	}
}

/***********************/
// LCD Move left or right
// Input: 0 or 1
// Output: None
/***********************/

void LCD_move(int m)
{
	switch(m)	
	{
	case 0:
	LCD_cmd(0x10); // Move cursor to the left
	break;
	
	case 1:
	LCD_cmd(0x14); // Move cursor to the right
	break;
	}
}
/**********************************/
// LCD Goto XY Position
// Input: X pos.: 1-20, Y pos.: 1-2
// Output: None
/**********************************/

void LCD_goto(int x, int y)
{
	switch(y)
	{
	case 1: // Goto x value on first line
	LCD_cmd(0x80| (x-1));
	break;

	case 2: // Goto x value on second line
	x += 40;
	LCD_cmd(0x80| (x-1));
	break;
	}
}

/***********************/
// LCD Display Clear
// Input: None
// Output: None
/***********************/

void LCD_clr()
{
	LCD_cmd(0x01); // Clears LCD and returns cursor to home
}


/***********************/
// LCD Backlight Dimmer
// Input: 
// Output: None
/***********************/
void BL_dimmer (char key)
{
   static uint8_t BL_dimValue = 255;
   static uint8_t toggle = true;
   static uint8_t wait_timer = 0;
   
   #define wait_interval 10
   #define fade_speed 15
   #define test_max 240
   #define test_min 15
   
   // set up TIMER2 with no prescaler and fast PWM mode
   TCCR2 |= (1 << WGM20)|(1 << WGM21)|(1 << CS20)|(1 << COM21);

   if (wait_timer > 0)
   {
      wait_timer--;
   }

   // toggles backlight on/off: 
   if (key == '#' && wait_timer == 0)
   {
      toggle = !toggle;
      wait_timer = wait_interval; // reset timer
   }

   switch (toggle)
   {
      case 1:
      LCD_goto(10,2);
      LCD_str("ON ");
      if (toggle == true && BL_dimValue < 255)
         {
           
            if (BL_dimValue >= test_max)
               {
                  BL_dimValue = 255;
               }
            else 
            {
               BL_dimValue += fade_speed;
            }
         }  
      break;

      case 0:
      LCD_goto(10,2);
      LCD_str("OFF");
      if (toggle == false && BL_dimValue > 0)
         {
            if (BL_dimValue <= test_min)
               {
                  BL_dimValue = 0;
               }
            else 
            {
               BL_dimValue -= fade_speed;
            }
         }
   }
   
   OCR2 = BL_dimValue; // PWM width for fade
    
}


