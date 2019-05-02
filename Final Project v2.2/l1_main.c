#include "lcd.h"
#include "open_interface.h"
#include "movement.h"
#include "button.h"
#include "uart.h"
#include "adc.h"
#include "ping.h"
#include "servo.h"
#include "scan.h"
#include <math.h>
#include "timer.h"


/**
 * main.c
 *Author: Vincent Johnson && Joshua French && Xinlei Yu
 *
 * @date 04-1-19
 */

//Pointer variables from other programs
// --------------------------
volatile int event = 0;
volatile int s_event = 0;
volatile char s_data[21];
volatile int i = 0;
int DONE = 0;
// -------------------------
//char songTone[16] = { };

int main(void)
{
    //Initializations
    lcd_init();
    servo_init();
    button_init();
    init_ADC();
    ping_init();
    serial_init(&s_event , &s_data[0] , &i);
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    move_init(&event);
    scan_init(&event);
//    oi_free(sensor_data);

    int start = 0;
//    oi_update(sensor_data);
//    double charge = (sensor_data -> batteryCharge) / (sensor_data -> batteryCapacity);
//    lcd_printf("%lf" , charge);
//    char startArray[1];
    while (start == 0) {
//        startArray[0] = UART1_Receive();
//        if (startArray[0] == "#") {
//            start = 1;
//        }
        start = button_getButton();
    }

    while (DONE == 0) {

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
            move_forward(sensor_data , 150);
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
            event = 0;
            scan_area();
        }
        if (event == 11)
        {
            lcd_printf("goal scan");
            int i = 0;
            i += scan_goal();
            turn_right(sensor_data , 180);
            i += scan_goal();
            turn_right(sensor_data , 180);
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
            lcd_printf("1 small, move closer");
            turn_right(sensor_data , 78);
            event = 10;
            move_forward(sensor_data , 100);
        }
        //small object from 22.5-45 and outside of 20cm
        if (event == 13)
        {
            lcd_printf("1 small, move closer");
            turn_right(sensor_data , 56);
            event = 10;
            move_forward(sensor_data , 100);
        }
        //small object from 45-67.5 and outside of 20cm
        if (event == 14)
        {
            lcd_printf("1 small, move closer");
            turn_left(sensor_data , 34);
            event = 10;
            move_forward(sensor_data , 100);
        }
        //small object from 67.5-90 and outside of 20cm
        if (event == 15)
        {
            lcd_printf("1 small, move closer");
            turn_left(sensor_data , 11);
            event = 10;
            move_forward(sensor_data , 100);
        }
        //small object from 90-112.5 and outside of 20cm
        if (event == 16)
        {
            lcd_printf("1 small, move closer");
            turn_left(sensor_data , 11);
            event = 10;
            move_forward(sensor_data , 100);
        }
        //small object from 112.5-135 and outside of 20cm
        if (event == 17)
        {
            lcd_printf("1 small, move closer");
            turn_left(sensor_data , 34);
            event = 10;
            move_forward(sensor_data , 100);
        }
        //small object from 135-157.5 and outside of 20cm
        if (event == 18)
        {
            lcd_printf("1 small, move closer");
            turn_left(sensor_data , 56);
            event = 10;
            move_forward(sensor_data , 100);
        }
        //small object from 157.5-180 and outside of 20cm
        if (event == 19)
        {
            lcd_printf("1 small, move closer");
            turn_left(sensor_data , 78);
            event = 10;
            move_forward(sensor_data , 100);
        }
        //100 - 190 turn right 1xx degrees and go between pylons
        if (event >= 100 && event <= 190)
        {
            //event -= 100;
            lcd_printf("2 small, turn right");
            double distance_one = objects[1][2];
            double distance_two = objects[2][2];
            double angle_one = objects[1][0];
            double angle_two = objects[2][0];

            if (abs(distance_one - distance_two) <= 10) {
//                turn_right(sensor_data, (angle_one + angle_two) / 2);
                event = 11;
                plow_forward(sensor_data , 400);
                if (event == 1 || event == 2) {
                    break;
                }
            } else if (distance_one < distance_two) {
                turn_right(sensor_data , angle_two);
                move_forward(sensor_data, (distance_two / 2));
                event = 10;
            } else if (distance_two < distance_one) {
                turn_right(sensor_data , angle_one);
                move_forward(sensor_data, (distance_one / 2));
                event = 10;
            }
        }
        //200 - 290 turn right 1xx degrees and go between pillars
        if (event >= 200 && event <= 290)
        {
            //event -= 100;
            lcd_printf("2 small, turn left");
            double distance_one = objects[1][2];
            double distance_two = objects[2][2];
            double angle_one = objects[1][0];
            double angle_two = objects[2][0];

            if (abs(distance_one - distance_two) <= 10) {
//                turn_left(sensor_data, (angle_one + angle_two) / 2);
                event = 11;
                plow_forward(sensor_data , 400);
                if (event == 1 || event == 2) {
                    break;
                }
            } else if (distance_one < distance_two) {
                turn_left(sensor_data , angle_two);
                move_forward(sensor_data, (distance_two / 2));
                event = 10;
            } else if (distance_two < distance_one) {
                turn_left(sensor_data , angle_one);
                move_forward(sensor_data, (distance_one / 2));
                event = 10;
            }
        }
        //300 - 480 single small object found 300 + x degrees
        if (event >= 300 && event <= 480)
        {
            lcd_printf("scan for small");
            //let event = the degree at which the small object was seen
            event -= 300;
            //turn so the object is at 150 degrees
            if (event < 150)
            {
                turn_right(sensor_data , 130 - event);
            }
            else
            {
                turn_left(sensor_data , event - 130);
            }
            //check to see if it can see two small objects
            int pillars = scan_goal();
            //if it sees two pillars
            if (pillars >= 2)
            {
                event = 10;
            }
            else
            {
                //if it sees less than two pillars, turn left and check the other side
                //turn so the object is at 30 degrees
                turn_left(sensor_data , 120);
                //check to see if it can see two small objects
                pillars = scan_goal();
                //if it sees two pillars
                if (pillars >= 2)
                {
                    event = 10;
                }
                //if it sees less than two pillars
                event = 10;
                turn_right(sensor_data , 5);
                move_forward(sensor_data , 100);
            }
        }
    }
    unsigned char MarioD_songTone[11] = {71,77,77,77,77,76,74,72,76,76,72};
    unsigned char MarioD_songDuration[11] = {15,15,15,15,20,20,20,15,15,15,20};
    oi_loadSong(3, 11, MarioD_songTone,MarioD_songDuration);
    oi_play_song(3);

    oi_free(sensor_data);
    return 0;
}

