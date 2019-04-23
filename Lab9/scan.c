#include "lcd.h"
#include "open_interface.h"
#include "lab5_scan_data.h"
#include "uart.h"
#include "adc.h"
#include "ping.h"
#include "servo.h"
#include "button.h"
#include <math.h>

/*
 *
 * Authors: Vincent Johnson & Joshua French & Xinlei Yu
 *
 */


volatile int *events;

void scan_init(int *event1)
{
    events = event1;
}

/**
 * method scans a 180 degree arc in front of the robot,
 * and prints out any objects width and angle of location onto putty.
 *
 * Then finds the smallest object and prints the  angle and width to LCD.
 */
void scan_area() {
    double objects[20][3]; //[Width][Angle]
    double ir_boundary = 60.0; //cm
    double ping_boundary = 70.0; //cm
    int voltage;
    double ping_distance;
    double ir_distance;
    double calibration = 8.5;
    char sendStr[60];
    double avg_distance = 0;
    int start_angle;
    int end_angle;
    int detected = 0;
    int num_objects = 0;
    double distance[180];

    servo_move(0);
    timer_waitMillis(1000);
    int i = 0;
    for (i = 0; i <= 180; i++) {
        servo_move(i);
//        timer_waitMillis(50);
        ping_distance = ping_getDistance() * 0.0010625;  //.0010300
        distance[i] = ping_distance;
        voltage =  adc_read();
        ir_distance = adc_conversion(voltage, calibration);

        if (ping_distance < ping_boundary && ir_distance < ir_boundary) {
            lcd_printf("Object Detected");
            if (detected == 0) {
                detected = 1;
                start_angle = i;
            }
        }
        else {
            lcd_printf("No Object");
            end_angle = i;
            if (detected == 1) {
                double width = (end_angle - start_angle) - 8;
                if (width > 0.0) {
                    num_objects++;

                    int j;
                    start_angle -= 6;
                    if (start_angle < 0) {
                        start_angle = 0;
                    }
                    end_angle -= 13;
                    avg_distance = 0;
                    for (j = start_angle; j <= end_angle; j++) {
                        avg_distance += distance[j];
                    }
                    avg_distance =  avg_distance / width;
                    double w = 2 * avg_distance / (tan((90 - (width / 2)) * (3.14159265 / 180)) - 1);
                    objects[num_objects][1] = w;
                    objects[num_objects][0] = start_angle + (width / 2) + 5;
                    objects[num_objects][2] = avg_distance;
                    sprintf(sendStr , "Object %d\r\nWidth(deg): %lf\r\nDistance(cm): %lf\r\nLinear Width(cm): %lf\r\n\n" , num_objects , width , avg_distance , w);
                    uart_sendStr(sendStr);

                }
            }
            detected = 0;
        }

    }


    //analysis of data
    if (num_objects == 0)
    {
        *events = 7;
    }
    int smallObject = 0;
    for (i = 1; i <= num_objects; i++)
    {
        if (objects[i][1] < 7.5)
        {
            smallObject = 1;
        }
    }
    //printf("%d\n" , smallObject);
    //if small object detected
    if (smallObject == 1)
    {
        *events = 7;
    }
    //if no small object detected
    if (smallObject != 1)
    {
        int move_forward = 0;
        for (i = 1; i <= num_objects; i++)
        {
            if (objects[i][0] > 40 && objects[i][0] < 130)
            {
                move_forward = 1;
            }
        }
        //if it cannot move directly forward
        if (move_forward == 1)
        {
            //figure out which way it needs to turn



            *events = 8;
        }
        //if it can move directly forward
        if (move_forward != 1)
        {
            *events = 7;
        }
    }









//    int min_object;
//    double min_width = 10000;
//    double min_angle = 0.0;
//    for (i = 1; i <= num_objects; i++) {
//        if (objects[i][1] < min_width) {
//            min_width = objects[i][1];
//            min_angle = objects[i][0];
//            min_object = i;
//
//        }
//    }
//    lcd_printf("Smallest Object: %d\nWidth: %.2lf \nAngle: %.2lf" , min_object, min_width, min_angle);
//    servo_move(min_angle);
}

//will take in the data from the scan and throw an event according to what it sees
//objects input will have the center angle in 0, and the linear width in 1




