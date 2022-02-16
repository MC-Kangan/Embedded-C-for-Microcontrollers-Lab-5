// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include <stdio.h>
#include "LCD.h"
#include "serial.h"
#include "ADC.h"
#include "interrupts.h"
//#include "timers.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  


void main(void) {
    LCD_Init();  
    ADC_init();
    Interrupts_init();
    //Timer0_init();
    initUSART4();
    //LCD_sendstring("hello world"); // Send the string to LCD screen
   
    unsigned int volt = 0;
    char message[0]; // Initialise a string, which essentially is an array
    char info;
    
    while (1) {
//        //message = getCharSerial4(); //Accept characters from the pc
//        LCD_setline(1); // Set Line 1
//        volt = ADC_getval(); // Get ADC_value
//        ADC2String(message,volt); // Create the string to be displayed 
//        LCD_sendstring(message); // Send the string to LCD screen
//        sendStringSerial4(message); // Send the string to realterm
//        sendStringSerial4("      "); // This is to add the spacing to each string
//        
//        __delay_ms(1000);
        if (isDataInRxBuf()){ // if data is in RX buffer
            info = getCharFromRxBuf(); // get the information to from the RX buffer
            LCD_sendbyte(info,1); // send the information to be displayed on the LCD
            putCharToTxBuf(info); // store the information to the TX buffer
            sendTxBuf(); // turn on the interrupt
        }
    }
}

