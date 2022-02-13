#include <xc.h>
#include "interrupts.h"
#include "serial.h"

/************************************
 * Function to turn on interrupts and set if priority is used
 * Note you also need to enable peripheral interrupts in the INTCON register to use CM1IE.
************************************/
void Interrupts_init(void)
{   PIE4bits.RC4IE=1;	//receive interrupt
    INTCONbits.GIEL=1;  // turn on peripheral interrupts
    INTCONbits.GIE=1; 	//turn on interrupts globally
	// turn on global interrupts, peripheral interrupts and the interrupt source 
	// It's a good idea to turn on global interrupts last, once all other interrupt configuration is done.
}

/************************************
 * High priority interrupt service routine
 * Make sure all enabled interrupts are checked and flags cleared
************************************/
void __interrupt(high_priority) HighISR()
{   if(PIR4bits.RC4IF){    //check the interrupt source some code you want to execute here; 
        putCharToRxBuf(RC4REG);
	}
}

