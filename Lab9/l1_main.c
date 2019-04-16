#include "lcd.h"
#include "open_interface.h"
#include "lab5_scan_data.h"
#include "uart.h"
#include "adc.h"
#include "ping.h"
#include "servo.h"
#include "button.h"
#include "scan.h"
#include <math.h>

/**
 * main.c
 *Author: Vincent Johnson && Joshua French
 *
 * @date 04-1-19
 */
volatile int char_event = 0;
volatile char s_data[21];
volatile int i;
void main(void)
{
    //Initialize the LCD. This also clears the screen
    lcd_init();
    servo_init();
    button_init();
    init_ADC();
    ping_init();
    serial_init(&char_event , &s_data[0] , &i);

    scan_area();
}


