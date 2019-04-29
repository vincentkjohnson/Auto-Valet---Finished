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
        //quick scan
        if (event == 0)
        {
            lcd_printf("scanning");
            scan_quick();
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
            move_forward(sensor_data , 200);
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
        if (event == 10)
        {
            lcd_printf("full scan");
            scan_area();
        }
        if (event == 11)
        {
            lcd_printf("goal scan");
            int i = 0;
            i += scan_goal();
            turn_right(sensor_data , 90);
            i += scan_goal();
            turn_right(sensor_data , 90);
            i += scan_goal();
            turn_right(sensor_data , 90);
            i += scan_goal();
            turn_right(sensor_data , 90);
            if (i >= 3)
            {
                lcd_printf("Objective Complete");
                break;
            }
            else
            {
                event == 0;
            }
        }
        //small object from 0-22.5 and outside of 20cm
        if (event == 12)
        {
            lcd_printf("1 small object");
            turn_right(sensor_data , 78);
            event = 10;
            move_forward(sensor_data , 100);
        }
        //small object from 22.5-45 and outside of 20cm
        if (event == 13)
        {
            lcd_printf("1 small object");
            turn_right(sensor_data , 56);
            event = 10;
            move_forward(sensor_data , 100);
        }
        //small object from 45-67.5 and outside of 20cm
        if (event == 14)
        {
            lcd_printf("1 small object");
            turn_left(sensor_data , 34);
            event = 10;
            move_forward(sensor_data , 100);
        }
        //small object from 67.5-90 and outside of 20cm
        if (event == 15)
        {
            lcd_printf("1 small object");
            turn_left(sensor_data , 11);
            event = 10;
            move_forward(sensor_data , 100);
        }
        //small object from 90-112.5 and outside of 20cm
        if (event == 16)
        {
            lcd_printf("1 small object");
            turn_left(sensor_data , 11);
            event = 10;
            move_forward(sensor_data , 100);
        }
        //small object from 112.5-135 and outside of 20cm
        if (event == 17)
        {
            lcd_printf("1 small object");
            turn_left(sensor_data , 34);
            event = 10;
            move_forward(sensor_data , 100);
        }
        //small object from 135-157.5 and outside of 20cm
        if (event == 18)
        {
            lcd_printf("1 small object");
            turn_left(sensor_data , 56);
            event = 10;
            move_forward(sensor_data , 100);
        }
        //small object from 157.5-180 and outside of 20cm
        if (event == 19)
        {
            lcd_printf("1 small object");
            turn_left(sensor_data , 78);
            event = 10;
            move_forward(sensor_data , 100);
        }
        //100 - 190 turn right 1xx degrees and go between pylons
        if (event >= 100 && event <= 190)
        {
            event -= 100;
            turn_right(sensor_data , event);
            event = 11;
            plow_forward(sensor_data , 400);
            if (event == 1 || event == 2)
            {
                break;
            }
        }
        //200 - 290 turn right 1xx degrees and go between pylons
        if (event >= 200 && event <= 290)
        {
            event -= 200;
            turn_left(sensor_data , event);
            event = 11;
            plow_forward(sensor_data , 400);
            if (event == 1 || event == 2)
            {
                break;
            }
        }
    }
    oi_free(sensor_data);
//    lcd_printf("done");
    //scan_area();
}


