/*********************************************************************************
 Name: Shota Tonari
 Cruz ID: 1828506
 ECE13
 *********************************************************************************/

// **** Include libraries here ****
// old bounce
// Standard libraries
#include <stdio.h>
					
//CMPE13 Support Library
#include "BOARD.h"
#include "Leds_Lab06.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// **** Declare any datatypes here ****


// **** Define global, module-level, or external variables here ****
struct Timer {
    uint8_t event;
    int16_t timeRemaining;
};

static struct Timer TimerA;


#define LEFT 0
#define RIGHT 1
static int state = LEFT;
uint8_t defaultLED = 0x80;
// **** Declare function prototypes ****


int main(void)
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    T1CON = 0; // everything should be off
    T1CONbits.TCKPS = 1; // 1:8 prescaler
    PR1 = 0xFFFF; // interrupt at max interval
    T1CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T1IF = 0; //clear the interrupt flag before configuring
    IPC1bits.T1IP = 4; // priority of  4
    IPC1bits.T1IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T1IE = 1; // turn the interrupt on

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    printf("Welcome to 1828506's lab6 part2 (bounce_switch).  Compiled on %s %s.\n",__TIME__,__DATE__);
    
    // Initialize LEDS
    LEDS_INIT();
    
    // Define Functions
    TimerA.event = FALSE;
    TimerA.timeRemaining = 1;
    
    // Set LED and state
    LEDS_SET(defaultLED);
    // char leftLED = 0x80;
    // char rightLED = 0x01;
							 
	while(1){
        if (TimerA.event) {
            // Set LED
            LEDS_SET(defaultLED);
            
            if (LATEbits.LATE7)
                state = RIGHT;
            else if (LATEbits.LATE0)
                state = LEFT;
            
            // Increment each LED by one shift depending on LEFT/RIGHT           
            if (state == LEFT) 
                defaultLED = defaultLED << 1;
            else if (state == RIGHT) 
                defaultLED = defaultLED >> 1;
            
            // Clear event flag
            TimerA.event = FALSE;
            
        }               
        //poll timer events and react if any occur
        
    }
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/
}

/**
 * This is the interrupt for the Timer1 peripheral. It will trigger at the frequency of the peripheral
 * clock, divided by the timer 1 prescaler and the interrupt interval.
 * 
 * It should not be called, and should communicate with main code only by using module-level variables.
 */
void __ISR(_TIMER_1_VECTOR, ipl4auto) Timer1Handler(void)
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    TimerA.timeRemaining--;
    if (TimerA.timeRemaining == FALSE) {
        TimerA.event = 1;
        TimerA.timeRemaining = SWITCH_STATES() + 1;
    }
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/									
	 
}