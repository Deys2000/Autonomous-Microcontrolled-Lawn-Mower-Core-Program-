/* 
 * File:   keypadIn.c
 * Author: aquadri
 *
 * Created on November 26, 2020, 3:22 AM
 */
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
volatile int32_t amount = 500000;
volatile int32_t small_amount = 50000;


unsigned char stuff_to_print[] = "Finally got this stuff to work";
unsigned char array[] = "Some string";
volatile int a = 0;

int main(){
    //unsigned char stuff_to_print[] = "Finally got this stuff to work"; // put the string you want displayed here 
    // I set the baud rate for the atmega to 9600 bps so that number corresponds to 103 for us
    	UBRR0H = (103>>8); // putting high bits of 103 into UBRR0H
    	UBRR0L = (103);    // putting low bits of 103 into UBRR0L
    	UCSR0C = 0x06;     // the atmega that you want to sent 8 bits at a time and the 1 stop bit
    	    
    while(1){
        a = getKeyInput();
        int counter = 0;
        delay(small_amount);
//        while(stuff_to_print[counter] != 0) {
//			while (!( UCSR0A & (1<<UDRE0))); //Wait for empty transmit buffer								 
//			UDR0 = stuff_to_print[counter];		//Put data into UDR0 and send to computer
//			counter++;    
//            //UDR0 = a + '0';
        
        UCSR0B |= 0b00001000; // turn on the transmitter
        while (!( UCSR0A & (1<<UDRE0)));   //Wait for empty transmit buffer again    
        UDR0 = a + '0';
        // this stuff below is just for formatting
		while (!( UCSR0A & (1<<UDRE0)));   //Wait for empty transmit buffer again      
        UDR0 = '\n';					    /* Put data into buffer, sends the data */
		while (!( UCSR0A & (1<<UDRE0)));   /* Wait for empty transmit buffer       */
		UDR0 = '\r';		    
        while (!( UCSR0A & (1<<UDRE0)));
        UCSR0B &= 0b11110111;; // turn off the transmitter
		delay(small_amount); 	
		}
        
    return 0;
}
int delay( volatile int32_t x){
    while(x >0){
        x = x -1;
    }
    return 0;
}

int getKeyInput(){
    DDRD = 0b10101011;
    PORTD = 0b11010100; // turn on all inputs to pull up resistors
    DDRB = 0b00100000;
    while(1){
        //Turn on Row 1, Pin 5
        PORTD = 0b11110100;
        delay(small_amount);
        //check key typed
        if( PIND == 0b10110100){ // Pin 6, Column 2
            PORTB ^= (1<<5);
            while(PIND == 0b10110100);
            return 2;
        }
        if( PIND == 0b11100100){ // Pin 4, Column 1
            PORTB ^= (1<<5);
            while(PIND == 0b11100100);
            return 1;
        }
        if( PIND == 0b11110000){ // Pin 2, Column 3
            PORTB ^= (1<<5);
            while(PIND == 0b11110000);
            return 3;
        }       
        //Turn on Row 2, Pin 0
        PORTD = 0b11010101;
        delay(small_amount);        
        if( PIND == 0b10010101){ // Pin 6, Column 2
            PORTB ^= (1<<5);
            while(PIND == 0b10010101);
            return 5;
        }
        if( PIND == 0b11000101){ // Pin 4, Column 1
            PORTB ^= (1<<5);
            while(PIND == 0b11000101);
            return 4;
        }
        if( PIND== 0b11010001){ // Pin 2, Column 3
            PORTB ^= (1<<5);
            while(PIND == 0b11010001);
            return 6;
        }
        //Turn on Row 3, Pin 1
        PORTD = 0b11010110;
        delay(small_amount);        
        if( PIND == 0b10010110){ // Pin 6, Column 2
            PORTB ^= (1<<5);
            while(PIND == 0b10010110);
            return 8;
        }
        if( PIND == 0b11000110){ // Pin 4, Column 1
            PORTB ^= (1<<5);
            while(PIND == 0b11000110);
            return 7;
        }
        if( PIND == 0b11010010){ // Pin 2, Column 3
            PORTB ^= (1<<5);
            while(PIND == 0b11010010);
            return 9;
        }        
        //Turn on Row 4, Pin 3
        PORTD = 0b11011100;
        delay(small_amount);        
        if( PIND == 0b10011100){ // Pin 6, Column 2
            PORTB ^= (1<<5);
            while(PIND == 0b10011100);
            return 0;
        }
        if( PIND == 0b11001100){ // Pin 4, Column 1
            PORTB ^= (1<<5);
            while(PIND == 0b11001100);
            return 111;
        }
        if( PIND == 0b11011000){ // Pin 2, Column 3
            PORTB ^= (1<<5);
            while(PIND == 0b11011000);
            return 222;
        }
    }
}
