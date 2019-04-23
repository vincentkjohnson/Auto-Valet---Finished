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
#include "open_interface.h"
#include "movement.h"
#include "timer.h"


/**
 * main.c
 *Author: Vincent Johnson && Joshua French && Xinlei Yu
 *
 * @date 04-1-19
 */
volatile int event = 0;
volatile char s_data[21];
volatile int i = 0;
void main(void)
{
    //Initialize the LCD. This also clears the screen
    lcd_init();
    servo_init();
    button_init();
    init_ADC();
    ping_init();
    serial_init(&event , &s_data[0] , &i);
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    move_init(&event);
    scan_init(&event);
//    putty_display();

    while (1)
    {
        if (event == 0)
        {
            lcd_printf("scanning");
            scan_area();
        }
        //bump right
        if (event == 1)
        {
            lcd_printf("right bump");
            move_backward(sensor_data , 50);
            turn_left(sensor_data , 47);
            event = 0;
//            move_forward(sensor_data , 250);
        }
        //bump left
        if (event == 2)
        {
            lcd_printf("left bump");
            move_backward(sensor_data , 50);
            turn_right(sensor_data , 43);
            event = 0;
//            move_forward(sensor_data , 250);
        }
        //right border
        if (event == 3)
        {
            lcd_printf("right border");
            move_backward(sensor_data , 50);
            turn_left(sensor_data , 17);
            event = 0;
//            move_forward(sensor_data , 250);
        }
        //left border
        if (event == 4)
        {
            lcd_printf("left border");
            move_backward(sensor_data , 50);
            turn_right(sensor_data , 23);
            event = 0;
//            move_forward(sensor_data , 250);
        }
        //right cliff
        if (event == 5)
        {
            lcd_printf("right cliff");
            move_backward(sensor_data , 50);
            turn_left(sensor_data , 15);
            event = 0;
//            move_forward(sensor_data , 250);
        }
        //left cliff
        if (event == 6)
        {
            lcd_printf("left cliff");
            move_backward(sensor_data , 50);
            turn_right(sensor_data , 15);
            event = 0;
//            move_forward(sensor_data , 250);
        }
        //the path is clear, can keep moving forward
        if (event == 7)
        {
            lcd_printf("all clear");
            event = 0;
            move_forward(sensor_data , 250);
        }
        //obstructions in the path on the left side, turn right and scan again
        if (event == 8)
        {
            lcd_printf("not clear");
            event = 0;
            turn_right(sensor_data , 30);
        }
        //obstructions in the path on the right side, turn left and scan again
        if (event == 9)
        {
            lcd_printf("not clear");
            event = 0;
            turn_left(sensor_data , 30);
        }

    }
    oi_free(sensor_data);
//    lcd_printf("done");
    //scan_area();
}


