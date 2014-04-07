


#define F_CPU 16000000UL // 16 MHz clock speed
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include "LCD.h"

/***********************/
// Sensors
/***********************/

int ADC_init (int channel)
{ switch (channel)
     { 
         case 0:
         ADMUX &= ~(0b00000001);    // Initialize ADMUX, to select input and reference voltage
         break;
         
         case 1:
         ADMUX |= 0b00000001;    // Initialize ADMUX, to select input and reference voltage
         break;
      }
    
      ADCSRA = 0b10010111;    // Initialize the control and status register
      ADCSRA |=0b01000000;    // Start the conversion   
        
      while ((ADCSRA & 0x10) == 0);
      return (ADC);
}

/***********************/
// Temperature Sensor
// Todo: Add average function!!!
/***********************/
void temp (void)
{  char s[10];
   int TempArray[5];

   int temp = 0, temp2;
           
      TempArray[5]=((ADC * 0.0048875 )- 0.400) / 0.0195 * 100;

      for(int i = 0; i < 5; i++){
      temp +=TempArray[i];
      }
      temp = temp / 5;

      temp2 = temp % 100;
      temp = temp / 100;
     
      sprintf(s, "%02d.%02d", temp, temp2);    //convert integer into a string
      LCD_goto(13,1);
      LCD_str(s);
}
      

/***********************/
// Light Sensor
//!!! If <99, third digit remains on screen

/***********************/
void light (void)
{ 
   unsigned int adc_light;
   char int_buffer[10];  
   char lead_0[2] = "0";
   char lead_00[3] = "00";

   ADC_init(1);
   adc_light = ADC;
   itoa(adc_light, int_buffer, 10); //convert integer into a string

   LCD_goto (5,2);

   if (strlen(int_buffer) == 1)
   {
      
      LCD_str(strcat(lead_00, int_buffer));
   }

   if (strlen(int_buffer) == 2)
   {
      LCD_str(strcat(lead_0, int_buffer));
   }

   else
   {  
      LCD_str(int_buffer);
   }

}  
