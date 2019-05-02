#include "ping.h"
#include "timer.h"
#include "lcd.h"

/*
 * Authors: Vincent Johnson & Joshua French
 */
char edge = 0; //rising edge

void ping_init() {

    //Config GPIO
    //i. Turn on Clock for Port B and Timer 3
    //

    SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R | 0x02;
    SYSCTL_RCGCTIMER_R |= 0x8;
    timer_waitMillis(100);

    //ii. Enable alternate function and set peripheral functionality
    GPIO_PORTB_AFSEL_R |= 0x8;
    GPIO_PORTB_PCTL_R |= 0x00007000;

    //iii. Set digital or analog mode, and pin direction
    GPIO_PORTB_DEN_R |= 0x8; //Set Pin 3 Digital mode
    GPIO_PORTB_DIR_R |= 0x8; //set up pin 3 as output

    //2. Set up Timer 3
    //i configure Timer 3 mode
    //Disable Timer while we set it up
    TIMER3_CTL_R &= ~0x100;

    //Configure Timer 3 functionality
    TIMER3_CFG_R = 0x4; //16bit mode
    TIMER3_TBMR_R = 0b10111; //input-edge-time mode
    TIMER3_CTL_R = 0xC00; //Detect Positive and Negative Edges

    //TIMER3_TAPR_R = ; //Use prescaler extension
    //TIMER3_TAILR_R = ; //Load max 24bit value

    //ii. Set up Timer 3 interrupts
    TIMER3_ICR_R |= 0x400; //Clear capture event interrupt status
    TIMER3_IMR_R |= 0x400;  //Enable capture events

    //3. NVIC Setup
    //i. Configure NVIC to allow Timer 3 interrupts
    NVIC_EN1_R |= 0x10;

    //ii. Bind timer 3 interrupt
    IntRegister(INT_TIMER3B, TIMER3_Handler);

    //Re-enalbe Timer 3
    //TIMER3_CTL_R |= 0x100;

    IntMasterEnable();

}

void TIMER3_Handler() { //check to see if interrupt bit is tripped
    if (edge == 1) {
        TIMER3_ICR_R |= 0x400; // clear interrupt flag
        risingEdge = TIMER3_TBR_R; //capture time of rising edge event
        edge = 0; //
    } else if (edge == 0) {
        TIMER3_ICR_R |= 0x400; // clear interrupt flag

        fallingEdge = TIMER3_TBR_R;
        edge = 2; // done checking
        TIMER3_CTL_R &= ~0x100;
    }
}

//void ping_send() {
//    GPIO_PORTB_DIR_R |= 0x8;
//    GPIO_PORTB_AFSEL_R &= ~0x8;
//    GPIO_PORTB_DATA_R &= ~0x8;
//    GPIO_PORTB_DATA_R |= 0x8;
//    timer_waitMicros(5);
//}

void ping_send() {
    GPIO_PORTB_AFSEL_R &= ~0x8; //Alt Function Off
	//GPIO_PORTB_PCTL_R &= ~0x0000F000; //Port Control Off
	
	GPIO_PORTB_DIR_R |= 0x08; //set PB3 as output
	GPIO_PORTB_DATA_R |= 0x08; //set PB3 to high

    timer_waitMicros(5);
}

void ping_read(void) {
	GPIO_PORTB_DATA_R &= ~0x8;  //set PB3 to low
	edge = 1;
    GPIO_PORTB_DIR_R &= ~0x8; //set PB3 to input-edge-time
	GPIO_PORTB_AFSEL_R |= 0x8; //Turn alt function on
	
	TIMER3_CTL_R |= 0x100; //Enable Timer3 to start counting
}

float ping_getDistance() {
	ping_send();
	ping_read();
	while(edge != 2);
	
	pulseLength = fallingEdge - risingEdge;
	return pulseLength; // 923.4f;
}
