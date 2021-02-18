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
#include <string.h>
volatile int32_t amount = 500000;
volatile int32_t small_amount = 5000;


int main(){
    //unsigned char stuff_to_print[] = "Finally got this stuff to work"; // put the string you want displayed here 
    // I set the baud rate for the atmega to 9600 bps so that number corresponds to 103 for us
    //UBRR0H = (103>>8); // putting high bits of 103 into UBRR0H, dont really need this as 103 is done within 8 bits
    UBRR0L = (103);    // putting low bits of 103 into UBRR0L    
    UCSR0C = 0x06;     // the atmega that you want to sent 8 bits at a time and the 1 stop bit
    //to start the program on the chip
    int start = getKeyInput();

    //
    //passwordSequence(); // complete, still do have small issue with keys 1 and 2...
    //int setting = settingSequence();
    int setting = 2;
    if( setting == 1){  // AutoDrive
        //autoDrive();
        ;
    }   
    else if (setting == 2){ // Manual
        //manual();
        int i = 0;
        unsigned char toScen[] = "The lawn mower has been set to Manual Mode. \n\r "
        "Use the following buttons to move around: \n\r"
        "Switch Button - Toggle Blade On/Off \n\r"
        "2 - Move Forward \n\r"
        "4 - Turn Left \n\r"
        "6 - Turn Right \n\r"
        "5 - Pause Movement \n\r"
        "0 - End Manual Mode \n\r";
        UCSR0B |= 0b00001000; // turn on the transmitter     
        while(toScen[i] != 0){
            while(!(UCSR0A & (1<<UDRE0)));
            UDR0 = toScen[i];
            i++;
        }
        while (!( UCSR0A & (1<<UDRE0)));   //Wait for empty transmit buffer again      
        UDR0 = '\n';					   /* Put data into buffer, sends the data */
        while (!( UCSR0A & (1<<UDRE0)));   /* Wait for empty transmit buffer       */
        UDR0 = '\r';		    
        while (!( UCSR0A & (1<<UDRE0)));
        UCSR0B &= 0b11110111;; // turn off the transmitter 
        
        //Start the Driving sequence
        DDRC |= 0b00000011;
        PORTC |= 0b00000000;
        int direction = 5;
        while( direction != 0){
            direction = getKeyInput();
            if ( direction == 2){ // move forward
                PORTC = 0b00000011;               
            }else if ( direction == 4){ // turn left
                PORTC = 0b00000001;             
            }else if ( direction == 6){ // turn right
                PORTC = 0b00000010;             
            }else if( direction == 5){ // pause movement
                PORTC = 0b00000000;                
            }    
        }
        
        i = 0;
        unsigned char toScn[] = "The lawn mower is no longer in Manual Mode";
        UCSR0B |= 0b00001000; // turn on the transmitter     
        while(toScn[i] != 0){
            while(!(UCSR0A & (1<<UDRE0)));
            UDR0 = toScn[i];
            i++;
        }
        while (!( UCSR0A & (1<<UDRE0)));   //Wait for empty transmit buffer again      
        UDR0 = '\n';					   /* Put data into buffer, sends the data */
        while (!( UCSR0A & (1<<UDRE0)));   /* Wait for empty transmit buffer       */
        UDR0 = '\r';		    
        while (!( UCSR0A & (1<<UDRE0)));
        UCSR0B &= 0b11110111; // turn off the transmitter 
        
    }

    delay(amount); 
    
        
    return 0;
}
int settingSequence(){
    nextLine();   
        int i = 0;
        unsigned char toScen[] = "What setting would you like to use today? \n\rAutoDrive (enter 1) \n\rManual (enter 2)";
        UCSR0B |= 0b00001000; // turn on the transmitter     
        while(toScen[i] != 0){
            while(!(UCSR0A & (1<<UDRE0)));
            UDR0 = toScen[i];
            i++;
        }
        while (!( UCSR0A & (1<<UDRE0)));   //Wait for empty transmit buffer again      
        UDR0 = '\n';					   /* Put data into buffer, sends the data */
        while (!( UCSR0A & (1<<UDRE0)));   /* Wait for empty transmit buffer       */
        UDR0 = '\r';		    
        while (!( UCSR0A & (1<<UDRE0)));
        UCSR0B &= 0b11110111;; // turn off the transmitter 
        
    int loop = 1;
    while(loop){
        int x = getKeyInput();
        displayKey(x);
        if( x == 1 || x == 2){
            return x;
        }
        else{
        nextLine();   
        int i = 0;
        unsigned char toScen[] = "Invalid Number, please select among the choices below. \n\rAutoDrive (enter 1) \n\rManual (enter 2)";
        UCSR0B |= 0b00001000; // turn on the transmitter     
        while(toScen[i] != 0){
            while(!(UCSR0A & (1<<UDRE0)));
            UDR0 = toScen[i];
            i++;
        }
        while (!( UCSR0A & (1<<UDRE0)));   //Wait for empty transmit buffer again      
        UDR0 = '\n';					   /* Put data into buffer, sends the data */
        while (!( UCSR0A & (1<<UDRE0)));   /* Wait for empty transmit buffer       */
        UDR0 = '\r';		    
        while (!( UCSR0A & (1<<UDRE0)));
        UCSR0B &= 0b11110111;; // turn off the transmitter 
            
        }
    }
}

void passwordSequence(){
    int x = getKeyInput();        
    int i = 0;
            unsigned char toScren[] = "Good Morning, Please enter the 4 digit password to activate your machine.";
            UCSR0B |= 0b00001000; // turn on the transmitter     
            while(toScren[i] != 0){
                while(!(UCSR0A & (1<<UDRE0)));
                UDR0 = toScren[i];
                i++;
            }
            while (!( UCSR0A & (1<<UDRE0)));   //Wait for empty transmit buffer again      
            UDR0 = '\n';					   /* Put data into buffer, sends the data */
            while (!( UCSR0A & (1<<UDRE0)));   /* Wait for empty transmit buffer       */
            UDR0 = '\r';		    
            while (!( UCSR0A & (1<<UDRE0)));
            UCSR0B &= 0b11110111;; // turn off the transmitter 
    int loop = 1;
    while(loop){
        
        int a = getKeyInput();        
        displayKey(a);
		delay(small_amount); 	
        int b = getKeyInput();
        displayKey(b);
        delay(small_amount);
        int c = getKeyInput();        
        displayKey(c);
        delay(small_amount);
        int d = getKeyInput();
        displayKey(d);
        delay(small_amount);
        nextLine();
        int comparison = 2*5*7*9;
        if( (a*b*c*d) == comparison){
            int i = 0;
            unsigned char toScren[] = "Password is correct, you are granted access.";
            UCSR0B |= 0b00001000; // turn on the transmitter     
            while(toScren[i] != 0){
                while(!(UCSR0A & (1<<UDRE0)));
                UDR0 = toScren[i];
                i++;
            }
            while (!( UCSR0A & (1<<UDRE0)));   //Wait for empty transmit buffer again      
            UDR0 = '\n';					   /* Put data into buffer, sends the data */
            while (!( UCSR0A & (1<<UDRE0)));   /* Wait for empty transmit buffer       */
            UDR0 = '\r';		    
            while (!( UCSR0A & (1<<UDRE0)));
            UCSR0B &= 0b11110111;; // turn off the transmitter   
            loop = 0;
        }
        else{
            int i = 0;
            unsigned char toScren[] = "That was not the correct password, please try again.";
            UCSR0B |= 0b00001000; // turn on the transmitter     
            while(toScren[i] != 0){
                while(!(UCSR0A & (1<<UDRE0)));
                UDR0 = toScren[i];
                i++;
            }
            while (!( UCSR0A & (1<<UDRE0)));   //Wait for empty transmit buffer again      
            UDR0 = '\n';					   /* Put data into buffer, sends the data */
            while (!( UCSR0A & (1<<UDRE0)));   /* Wait for empty transmit buffer       */
            UDR0 = '\r';		    
            while (!( UCSR0A & (1<<UDRE0)));
            UCSR0B &= 0b11110111;; // turn off the transmitter            
            
        }
		delay(small_amount); 	
    }
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
            return 0;
        }
        if( PIND == 0b11011000){ // Pin 2, Column 3
            PORTB ^= (1<<5);
            while(PIND == 0b11011000);
            return 0;
        }
    }
}

void displayUART( unsigned char toScren[]){
    int i = 0;
    UCSR0B |= 0b00001000; // turn on the transmitter     
    while(toScren[i] != 0){
        while(!(UCSR0A & (1<<UDRE0)));
        UDR0 = toScren[i];
        i++;
    }
    while (!( UCSR0A & (1<<UDRE0)));   //Wait for empty transmit buffer again      
    UDR0 = '\n';					   /* Put data into buffer, sends the data */
	while (!( UCSR0A & (1<<UDRE0)));   /* Wait for empty transmit buffer       */
	UDR0 = '\r';		    
    while (!( UCSR0A & (1<<UDRE0)));
    UCSR0B &= 0b11110111;; // turn off the transmitter
}

void displayKey( unsigned int key){
    UCSR0B |= 0b00001000; // turn on the transmitter
    while (!( UCSR0A & (1<<UDRE0)));   //Wait for empty transmit buffer again      
    UDR0 = key + '0';    
    while (!( UCSR0A & (1<<UDRE0)));
    UCSR0B &= 0b11110111;; // turn off the transmitter 
}

void nextLine(){
    UCSR0B |= 0b00001000; // turn on the transmitter
    while (!( UCSR0A & (1<<UDRE0)));   //Wait for empty transmit buffer again      
    UDR0 = '\n';					   /* Put data into buffer, sends the data */
	while (!( UCSR0A & (1<<UDRE0)));   /* Wait for empty transmit buffer       */
	UDR0 = '\r';
    UCSR0B &= 0b11110111;; // turn off the transmitter 
}