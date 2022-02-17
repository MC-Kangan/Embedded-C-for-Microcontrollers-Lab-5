#include <xc.h>
#include "interrupts.h"
#include "serial.h"

/************************************
 * Function to turn on interrupts and set if priority is used
 * Note you also need to enable peripheral interrupts in the INTCON register to use CM1IE.
************************************/
void Interrupts_init(void)
{   PIE4bits.RC4IE = 1;	// receive interrupt
    PIE4bits.TX4IE = 0; // transmit interrupt (only turn on when you have more than one byte to send)
    INTCONbits.GIEL=1;  // turn on peripheral interrupts
    INTCONbits.GIE=1; 	// turn on interrupts globally
	// turn on global interrupts, peripheral interrupts and the interrupt source 
	// It's a good idea to turn on global interrupts last, once all other interrupt configuration is done.
}

/************************************
 * High priority interrupt service routine
 * Make sure all enabled interrupts are checked and flags cleared
************************************/
void __interrupt(high_priority) HighISR()
{   if(PIR4bits.RC4IF){    //check the interrupt source some code you want to execute here; 
                           // After reading RC4IF, the flag will be cleared to 0
        putCharToRxBuf(RC4REG); // add the character to RX buffer
	}    
    if (PIR4bits.TX4IF && (PIE4bits.TX4IE == 1)) { // If transmit buffer TX4REG is empty and transmit interrupt is enabled
        TX4REG = getCharFromTxBuf(); // Get characters from transmit buffer. After writing TX4REG, flag is cleared to 0
        //PIR4bits.TX4IF = 0; // Setting the transmit buffer to be full
        if (isDataInTxBuf() == 0) {PIE4bits.TX4IE = 0;} // If TX buffer is empty/ All information is read (if write count = read count), disabled the transmit interrupt
    }
}

