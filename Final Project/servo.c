#include "servo.h"
#include "lcd.h"
#include "button.h"

/**
 * Authors: Vincent Johnson && Joshua French
 * Program to initialize servo and move servo
 */

/*
 * variable used to account for increasing or decreasing degree
 */
int multiplier = 1;
int calibration = 1500;


/**
 * method to initialize timer1B with servo
 */
void servo_init(void) {
    /*
     * Step 1: Initialize Timers
     * Timer1 && GPIO_PORTB (PB5)
     */
    //i. Turn on Clock for Port B and Timer 1
    SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R | 0x02;
    SYSCTL_RCGCTIMER_R |= 0x2;

    //ii. Enable alternate function and set peripheral functionality
    GPIO_PORTB_AFSEL_R |= 0x20;
    GPIO_PORTB_PCTL_R |= 0x700000;

    //iii. Set digital or analog mode, and pin direction
    GPIO_PORTB_DEN_R |= 0x20; //Set Pin 5 Digital mode
    GPIO_PORTB_DIR_R |= 0x20; //set up pin 5 as output

    //2. Set up Timer 1
    //i configure Timer 1 mode
    //Disable Timer while we set it up
    TIMER1_CTL_R &= 0x0000;

    //Configure Timer 1 functionality
    TIMER1_CFG_R = 0x4; //16bit mode
    TIMER1_TBMR_R = 0b01010; //Set PWM Mode with Periodic Count
    TIMER1_TBPR_R = 0x4; // >> 16; //Use prescaler extension
    TIMER1_TBILR_R = 0xE200; // & 0xFFFF; //Load into GPTMTBILR as start value (period)
    TIMER1_TBMATCHR_R = 0x0; //Set the low pulse value
    TIMER1_TBPMR_R = 0x4; // >> 16; //Prescale Match

    //ii. Set up Timer 1 interrupts
    //TIMER1_ICR_R |= ; //Clear capture event interrupt status
    //TIMER1_IM_R |= ;  //Enable capture events

    //3. NVIC Setup
    //i. Configure NVIC to allow Timer 3 interrupts

    //ii. Bind timer 1 interrupt

    //Re-enable Timer 1
    TIMER1_CTL_R |= 0x100;

    currentDegree = 90;
}

/**
 * Method to move the servo
 * @degrees
 */
void servo_move(double degrees) {
	//TIMER1_TBMATCHR_R = 320000 - (16000 + (16000 * (degrees / 180)));
    TIMER1_TBMATCHR_R = (312000 - ((1400 * degrees) / 9)) + calibration;
}

/**
 * method to move the servo with buttons
 *
 */
void button_servo_move() {
    double temp = currentDegree;

    if (button_getButton() == 1) {
        currentDegree = currentDegree + 1 * multiplier;
        if (currentDegree > 180 || currentDegree < 0) {
            currentDegree = temp;
        }
    } else if (button_getButton() == 2) {
        currentDegree = currentDegree + 2.5 * multiplier;
        if (currentDegree > 180 || currentDegree < 0) {
            currentDegree = temp;
        }
    } else if (button_getButton() == 3) {
        currentDegree = currentDegree + 5 * multiplier;
        if (currentDegree > 180 || currentDegree < 0) {
            currentDegree = temp;
        }
    } else if (button_getButton() == 4) {
        //switch between clockwise and counterclockwise movement of the servo
        multiplier = 0 - multiplier;
    } else if (button_getButton() == 5) {
        currentDegree = 0.0;
    } else if (button_getButton() == 6) {
        currentDegree = 180.0;
    }
    servo_move(currentDegree);
}
