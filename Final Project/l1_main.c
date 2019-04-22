#include "lcd.h"
#include "open_interface.h"
#include "lab5_scan_data.h"
#include "uart.h"
#include "adc.h"
#include "ping.h"
#include "servo.h"
#include "button.h"

/**
 * main.c
 *Author: Vincent Johnson && Joshua French
 *
 * @date 04-1-19
 */
volatile int char_event = 0;
volatile char s_data[21];
volatile int i;
volatile float data[180][2];

void main(void) {

    //Wait for Start Command

    //Scan Area

    //Send scan to mobile

    //Move depending on scan


}


