#include <xc.h>
#include "LCD.h"
#include <stdio.h>

/************************************
 * Function to toggle LCD enable bit on then off
 * when this function is called the LCD screen reads the data lines
************************************/
void LCD_E_TOG(void)
{
	//turn the LCD enable bit on
    LCD_E = 1; // E stands for chip enable signal
	__delay_us(2); //wait a short delay
	//turn the LCD enable bit off again
    LCD_E = 0;
}

/************************************
 * Function to set the 4-bit data line levels for the LCD
************************************/
void LCD_sendnibble(unsigned char number)
{

	//set the data lines here (think back to LED array output)
    if (number & 0b0001) {LCD_DB4 = 1;} else {LCD_DB4 = 0;}
    if (number & 0b0010) {LCD_DB5 = 1;} else {LCD_DB5 = 0;}
    if (number & 0b0100) {LCD_DB6 = 1;} else {LCD_DB6 = 0;}
    if (number & 0b1000) {LCD_DB7 = 1;} else {LCD_DB7 = 0;}

    LCD_E_TOG();			//toggle the enable bit to instruct the LCD to read the data lines
    __delay_us(5);          //Delay 5uS
}


/************************************
 * Function to send full 8-bit commands/data over the 4-bit interface
 * high nibble (4 most significant bits) are sent first, then low nibble sent
************************************/
void LCD_sendbyte(unsigned char Byte, char type)
{
    // set RS pin whether it is a Command (0) or Data/Char (1) using type argument
    LCD_RS = type;
    // send high bits of Byte using LCDout function
    // send low bits of Byte using LCDout function
    
    // Most significant bits
    unsigned char msb = Byte >> 4;  // Right shift 4 times to get the most significant bits from the 8 bits
    LCD_sendnibble(msb);
    // Least significant bits
    unsigned char lsb = Byte & 0b00001111; // Use AND condition to extract only the last 4 sig bits
    LCD_sendnibble(lsb); //
    
    __delay_us(50);               //delay 50uS (minimum for command to execute)
}

/************************************
 * Function to initialise the LCD after power on
************************************/
void LCD_Init(void)
{

    //Define LCD Pins as Outputs and (Checked Clicker2 manual)
    //set all pins low (might be random values on start up, fixes lots of issues)
    TRISEbits.TRISE1 = 0;
    TRISEbits.TRISE3 = 0;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0;
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC6 = 0;
    
    LCD_DB4 = 0;
    LCD_DB5 = 0;
    LCD_DB6 = 0;
    LCD_DB7 = 0;

    //Initialisation sequence code
	// follow the sequence in the GitHub Readme picture for 4-bit interface.
	// first Function set should be sent with LCD_sendnibble (the LCD is in 8 bit mode at start up)
	// after this use LCD_sendbyte to operate in 4 bit mode
    __delay_ms(50);
    LCD_sendnibble(0b0011); // Function set 1
    __delay_ms(50);
    LCD_sendbyte(0b00101000,0); // Function set 2 (from the graph, 1st row is 0010, 2nd row is 1000, N = 1, F = 0)
    __delay_ms(50);
    LCD_sendbyte(0b00101000,0); // Function set 3 (from the graph, 1st row is 0010, 2nd row is 1000, N = 1, F = 0)
    __delay_ms(50);
    LCD_sendbyte(0b00001000,0); // Display ON/OFF control (from the graph, 1st row is 0000, 2nd row is 1000, D = 0, C = 0, B = 0)
    __delay_ms(50);
    LCD_sendbyte(0b00000001,0); // Display clear
    __delay_ms(5);
    LCD_sendbyte(0b00000110,0); // Entry Mode Set
    
	//remember to turn the LCD display back on at the end of the initialisation (not in the data sheet)
    LCD_sendbyte(0b00001100,0); // Turn on the display, turn on display (D), cursor (C) and blinking (B)
}

/************************************
 * Function to set the cursor to beginning of line 1 or 2
************************************/
void LCD_setline (char line)
{
    //Send 0x80 to set line to 1 (0x00 ddram address)
    //Send 0xC0 to set line to 2 (0x40 ddram address)
     if (line==1){
        LCD_sendbyte(0x80,0);//Send 0x80 to set line to 1 (0x00 ddram address), 0x80 is 128
    }
    if (line==2){
        LCD_sendbyte(0xC0,0);//Send 0xC0 to set line to 2 (0x40 ddram address), 0xC0 is 192
    }
    
}

/************************************
 * Function to send string to LCD screen
************************************/
void LCD_sendstring(char *string)
{	  
	while(*string != 0){  // While the data pointed to isn?t a 0x00 do below (strings in C must end with a NULL byte) 
		LCD_sendbyte(*string++,1); 	//Send out the current byte pointed to and increment the pointer
	}
}

/************************************
 * Function to send string to LCD screen
************************************/
void LCD_scroll(void)
{
    __delay_ms(500);
    LCD_sendbyte(0b00011000,0); // shift display, shift left
    //code here to scroll the text on the LCD screen
}

/************************************
 * Function takes a ADC value and works out the voltage to 2 dp
 * the result is stored in buf as ascii text ready for display on LCD
 * Note result is stored in a buffer using pointers, it is not sent to the LCD
************************************/
void ADC2String(char *buf, unsigned int ADC_val){
	//code to calculate the inegeter and fractions part of a ADC value
	// and format as a string using sprintf (see GitHub readme)
    unsigned int int_part = 0, fract_part = 0;
    int_part = 33*ADC_val/2550; // Integer part 
    fract_part = 33*ADC_val*10/255 - int_part*100; // Fraction part
    sprintf(buf,"V = %d.%02dV",int_part,fract_part); // Store the string into buf
    
}

// Function to showresult (same logic as ADC2String)
void showresult(char *buf, unsigned int second){
	//code to calculate the inegeter and fractions part of a ADC value
	// and format as a string using sprintf (see GitHub readme)
    unsigned int time = second/2;
    sprintf(buf,"%03d seconds :)  ",time); // Store the string into buf
    
}


void LCD_update_screen(unsigned char player_pos, unsigned char enemy_pos, unsigned char enemy_pos2, unsigned char bullet1, unsigned char bullet2) {
    
    LCD_sendbyte(1,0);  // clear display
    __delay_ms(2);
    LCD_sendbyte(player_pos,0);          // set cursor position
    LCD_sendbyte(0,1);                  // display 'player' character data in CGRAM to LCD 
    LCD_sendbyte(enemy_pos,0);           // set cursor position
    LCD_sendbyte(0b10111011,1);          // display 'enemy1' character data '<<' in CGRAM to LCD
    LCD_sendbyte(enemy_pos2,0);           // set cursor position
    LCD_sendbyte(0b10111100,1);          // display 'enemy2' character data '>>' in CGRAM to LCD
    LCD_sendbyte(bullet1,0);           // set cursor position
    LCD_sendbyte(0b11001101,1);          // display bullet1 character data in CGRAM to LCD
    LCD_sendbyte(bullet2,0);           // set cursor position
    LCD_sendbyte(0b11001101,1);          // display bullet2 character data in CGRAM to LCD
    
}
unsigned char LCD_move(unsigned char player_pos,unsigned char enemy_pos, unsigned char enemy_pos2, unsigned char bullet1, unsigned char bullet2){
    
    if (player_pos == 0xC0 + 7) {                                 // player at line 2
        player_pos = 0x80 + 7;                                    // move player to line 1
        LCD_update_screen(player_pos, enemy_pos, enemy_pos2, bullet1, bullet2);   // display
    }
    else{
        player_pos = 0xC0 + 7;
        LCD_update_screen(player_pos, enemy_pos, enemy_pos2, bullet1, bullet2);   // display
    }
    return player_pos;
}

// This function is not used
void LCD_reset(unsigned char player_pos, unsigned char enemy_pos, unsigned char enemy_pos2, unsigned char bullet1, unsigned char bullet2) {
    LCD_sendbyte(1,0);  // clear display
    player_pos = 0xC0 + 7;        // position at the second row
    enemy_pos = 0XC0 + 20;  // position at the end of the second row
    enemy_pos2 = 0X80;  // position at the end of the first row
    bullet1 = 0;
    bullet2 = 0;
}

// Function to create characters
void LCD_create_character(unsigned char *character, unsigned char CGRAM_loc) {
    LCD_sendbyte(0b01000000 + CGRAM_loc,0);  // Set CGRAM address in address counter.
    int i; 
    for (i=0;i<8;i++) {  // sending character pattern to CGRAM
        LCD_sendbyte(character[i],1);
    }
}