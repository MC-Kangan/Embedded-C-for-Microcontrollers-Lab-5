// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include <stdio.h>
#include "LCD.h"
#include "serial.h"
//#include "ADC.h"
//#include "interrupts.h"
//#include "timers.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  


void main(void) {
    LCD_Init();  
    //ADC_init();
    //Interrupts_init();
    //Timer0_init();
    initUSART4();
   
    char message;
    
    while (1) {
        message = getCharSerial4(); //Accept characters from the pc
        LCD_sendbyte(message,1); // Send data to be displayed on the LCD screen (one letter is sent at one time)
        
        
    }
}

