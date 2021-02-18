/* 
 * File:   
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
#include <avr/interrupt.h>

#define ISR_TIMER1_RELOAD (61/2) // set to go off every half a second
    
volatile int global_percent = 0;
volatile int32_t amount = 500000;
volatile int32_t small_amount = 5000;
volatile int32_t turn_time = 1300000;
volatile int32_t line_time = 2000000;

int main(){
    // Set baud rate 9600 bps, that number corresponds to 103 for the ATmega
    UBRR0L = 103;     
    UCSR0C = 0x06;     // configuring 8 bits at a time and then 1 stop bit  
   
    // Program Starts
    //Wait for Power Button to be pressed
        int start = 1;
        while(start){
        int key = getKeyInput();
        if( key == 3){
            start = 0;
            //Turn on Power Light
            DDRD = (1<<7);
            PORTD = (1<<7);            
        }
    }
    // Take in password from user
    passwordSequence(); 
    
    // main functionality loop
    int loop = 1;
    while( loop ){
        int setting = settingSequence();
        // AutoDrive
        if( setting == 1){ autoDrive(); } 
    
        // Manual
        else if (setting == 2){ manualMode();  } 
    
        // Shutdown Machine
        else if(setting == 3){
            loop = 0;
            PORTD = 0x00; // turn off power light
            endMower(); // end of the program
        }
    }            
    return 0;
}
//AutoDrive Functionality 
//--------------------------------
void autoDrive(){
        nextLine();

        // setting up the timer1
        TCCR1A |= (1<<WGM11); //CTC Mode
        OCR1A = 0xFF;
        TIMSK1 |= (1<<OCIE1A);
        TCCR1B |= (1<<CS10)|(1<<CS12); // 1024 times using the 101 bit pattern
       
        // actual driving starts here
        DDRC = 0b00000011;
        
        sei();
        moveAhead();
        global_percent = global_percent + 20;
        turnLeft();
        moveAhead();
        global_percent = global_percent + 20;      
        turnRight();
        moveAhead();
        global_percent = global_percent + 20;
        turnLeft();
        moveAhead();
        global_percent = global_percent + 20;
        turnRight();
        moveAhead();
        global_percent = global_percent + 20;
        delay(amount*3  );
        cli();
        //Turn off Motor Functions
        PORTC = 0b00000000;
        
        
     
}
void moveAhead(){
    // drive ahead
    PORTC = 0b00000011;
    delay(line_time);
}
void turnRight(){
        //turn right 180
        PORTC = 0b00000001;
        delay(turn_time); 
}
void turnLeft(){
        //turn left 180
        PORTC = 0b00000010;
        delay(turn_time); 
}
ISR (TIMER1_COMPA_vect){
    static int32_t isr_count_timer1 = ISR_TIMER1_RELOAD;
    
    if(isr_count_timer1 > 0){
        isr_count_timer1--;
    }
    else{
        printPercentage();
        isr_count_timer1 = ISR_TIMER1_RELOAD;
    }
}
void printPercentage(){
    int i = 0;
    UCSR0B |= 0b00001000; // turn on the transmitter  
    if(global_percent == 0){
        unsigned char toScreen1[] = "percentage of lawn complete: 0%";
        while(toScreen1[i] != 0){
            while(!(UCSR0A & (1<<UDRE0)));
            UDR0 = toScreen1[i];
            i++;
        }
    }
    else if(global_percent == 20){
        unsigned char toScreen1[] = "percentage of lawn complete: 20%";
        while(toScreen1[i] != 0){
            while(!(UCSR0A & (1<<UDRE0)));
            UDR0 = toScreen1[i];
            i++;
        }
    }
    else if(global_percent == 40){
        unsigned char toScreen1[] = "percentage of lawn complete: 40%";
        while(toScreen1[i] != 0){
            while(!(UCSR0A & (1<<UDRE0)));
            UDR0 = toScreen1[i];
            i++;
        }
    }
    else if(global_percent == 60){
        unsigned char toScreen1[] = "percentage of lawn complete: 60%";
        while(toScreen1[i] != 0){
            while(!(UCSR0A & (1<<UDRE0)));
            UDR0 = toScreen1[i];
            i++;
        }
    }
    else if(global_percent == 80){
        unsigned char toScreen1[] = "percentage of lawn complete: 80%";
        while(toScreen1[i] != 0){
            while(!(UCSR0A & (1<<UDRE0)));
            UDR0 = toScreen1[i];
            i++;
        }
    }
    else{
        unsigned char toScreen1[] = "percentage of lawn complete: 100% \n\rHave a nice day :)\n\r\nThe Lawn Mower is no longer in Automatic Mode ";
        while(toScreen1[i] != 0){
            while(!(UCSR0A & (1<<UDRE0)));
            UDR0 = toScreen1[i];
            i++;
        }
        global_percent=0;
    }
       
        while (!( UCSR0A & (1<<UDRE0)));   //Wait for empty transmit buffer again      
        UDR0 = '\n';					   /* Put data into buffer, sends the data */
        while (!( UCSR0A & (1<<UDRE0)));   /* Wait for empty transmit buffer       */
        UDR0 = '\r';		    
        while (!( UCSR0A & (1<<UDRE0)));
        UCSR0B &= 0b11110111; // turn off the transmitter
        
}
//---------------------------------
//Manual Mode Functionality
//---------------------------------
void manualMode(){
    //manual();
        int i = 0;
        unsigned char toScen[] = "\n\rThe lawn mower has been set to Manual Mode. \n\r"
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
//---------------------------------
//Shutdown Mower Functionality
//----------------------------
void endMower(){
    int i = 0;
    unsigned char toScen[] = "\n\rLawn Mower is now shutting down, have a nice day!";
        UCSR0B |= 0b00001000; // turn on the transmitter     
        while(toScen[i] != 0){
            while(!(UCSR0A & (1<<UDRE0)));
            UDR0 = toScen[i];
            i++;
        }
        while (!( UCSR0A & (1<<UDRE0)));
        UCSR0B &= 0b11110111;; // turn off the transmitter
}
//----------------------------
// Main method setting function
int settingSequence(){
    nextLine();   
        int i = 0;
        unsigned char toScen[] = "What setting would you like to use? \n\rAutoDrive (enter 1) \n\rManual (enter 2) \n\rShutdown Lawn Mower (enter 3)";
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
        if( x == 1 || x == 2 || x == 3){
            return x;
        }
        else{
        nextLine();   
        int i = 0;
        unsigned char toScen[] = "Invalid Number, please select among the choices below. \n\rAutoDrive (enter 1) \n\rManual (enter 2) \n\rShutdown Lawn Mower (enter 3)";
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
// Password Input setup function
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
// Delay Function
int delay( volatile int32_t x){
    while(x >0){
        x = x -1;
    }
    return 0;
}
// Keypad Input Function
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
// UART Functions Start Here
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
// UART Functions End Here