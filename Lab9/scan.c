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
//void scan_area() {
//    double objects[20][3]; //[Width][Angle]
//    double ir_boundary = 60.0; //cm
//    double ping_boundary = 70.0; //cm
//    int voltage;
//    double ping_distance;
//    double ir_distance;
//    double calibration = 0;
//    char sendStr[60];
//    double avg_distance = 0;
//    int start_angle;
//    int end_angle;
//    int detected = 0;
//    int num_objects = 0;
//    double distance[180];
//
//    servo_move(0);
//    timer_waitMillis(1000);
//    int i = 0;
//    for (i = 0; i <= 180; i++) {
//        servo_move(i);
////        timer_waitMillis(50);
//        ping_distance = ping_getDistance() * 0.0010625;  //.0010300
//        distance[i] = ping_distance;
//        voltage =  adc_read();
//        ir_distance = adc_conversion(voltage, calibration);
//
//        if (ping_distance < ping_boundary && ir_distance < ir_boundary) {
//            lcd_printf("Object Detected");
//            if (detected == 0) {
//                detected = 1;
//                start_angle = i;
//            }
//        }
//        else {
//            lcd_printf("No Object");
//            end_angle = i;
//            if (detected == 1) {
//                double width = 0;
//                if (end_angle - start_angle > 10)
//                {
//                    width = (end_angle - start_angle) - 8;
//                    start_angle -= 7;
//                    end_angle -= 15;
//                }
//                else
//                {
//                    width = end_angle - start_angle - 1;
//                    start_angle -= 7;
//                    end_angle -= 7;
//                }
//                if (width > 0) {
//                    num_objects++;
//
//                    int j;
//
//                    if (start_angle < 0) {
//                        end_angle += abs(start_angle);
//                        start_angle = 0;
//                    }
//
//                    avg_distance = 0;
//                    for (j = start_angle; j <= end_angle; j++) {
//                        avg_distance += distance[j];
//                    }
//                    avg_distance =  avg_distance / width;
//                    double w = 2 * avg_distance / (tan((90 - (width / 2)) * (3.14159265 / 180)) - 1);
//                    objects[num_objects][1] = w;
//                    objects[num_objects][0] = start_angle + (width / 2) + 5;
//                    objects[num_objects][2] = avg_distance;
//                    sprintf(sendStr , "Object %d\r\nWidth(deg): %lf\r\nDistance(cm): %lf\r\nLinear Width(cm): %lf\r\n\n" , num_objects , width , avg_distance , w);
//                    uart_sendStr(sendStr);
//
//                }
//            }
//            detected = 0;
//        }
//
//    }
//
//
//    //analysis of data
//    if (num_objects == 0)
//    {
//        *events = 7;
//    }
//    int smallObject = 0;
//    for (i = 1; i <= num_objects; i++)
//    {
//        if (objects[i][1] < 7.5)
//        {
//            smallObject += 1;
//        }
//    }
////    printf("%d\n" , smallObject);
//    //if small object detected
//    if (smallObject >= 1)
//    {
//        //if it sees one small object
//        if (num_objects == 1)
//        {
//            //if the one small object is over 20 cm away
//            if (objects[1][2] > 20.0)
//            {
//                if (objects[1][0] < 22.5)
//                {
//                    *events = 12;
//                }
//                else if (objects[1][0] < 45 && objects[1][0] >= 22.5)
//                {
//                    *events = 13;
//                }
//                else if (objects[1][0] < 67.5 && objects[1][0] >= 45)
//                {
//                    *events = 14;
//                }
//                else if (objects[1][0] < 90 && objects[1][0] >= 67.5)
//                {
//                    *events = 15;
//                }
//                else if (objects[1][0] < 112.5 && objects[1][0] >= 90)
//                {
//                    *events = 16;
//                }
//                else if (objects[1][0] < 135 && objects[1][0] >= 112.5)
//                {
//                    *events = 17;
//                }
//                else if (objects[1][0] < 157.5 && objects[1][0] >= 135)
//                {
//                    *events = 18;
//                }
//                else
//                {
//                    *events = 19;
//                }
//            }
//            //if the single small object is within 20 cm away
//            else
//            {
//                *events = 7;//TODO
//            }
//        }
//        if (smallObject >= 2)
//        {
//            //if it sees small objects and that is all it sees
//            if (smallObject == num_objects)
//            {
//                int angle = (objects[1][0] + objects[2][0]) / 2;
//                if (angle < 90)
//                {
//                    *events = 100 + (90 - angle);
//                }
//                else
//                {
//                    *events = 200 + (angle - 90);
//                }
//            }
//            //if it sees small objects and tall objects
//            else
//            {
//                *events = 7;//TODO
//
//            }
//        }
//    }
//    //if no small object detected
//    if (smallObject == 0 )
//    {
//        int move_forward = 0;
//        for (i = 1; i <= num_objects; i++)
//        {
//            if (objects[i][0] > 40 && objects[i][0] < 130)
//            {
//                move_forward = 1;
//            }
//        }
//        //if it cannot move directly forward
//        if (move_forward == 1)
//        {
//            //figure out which way it needs to turn
//            for (i = 1; i <= num_objects; i++)
//            {
//                if (objects[i][0] > 40 && objects[i][0] <= 90)
//                {
//                    *events = 9;
//                    break;
//                }
//                if (objects[i][0] >= 130 && objects[i][0] < 90)
//                {
//                    *events = 8;
//                    break;
//                }
//            }
//
//        }
//        //if it can move directly forward
//        if (move_forward != 1)
//        {
//            *events = 7;
//        }
//    }
//}

void scan_area() {
    double objects[20][3]; //[Width][Angle]
    double ir_boundary = 60.0; //cm
    double ping_boundary = 70.0; //cm
    int voltage;
    double ping_distance;
    double ir_distance;
    double calibration = 0;
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
                double width = end_angle - start_angle;
                if (width > 1) {
                    num_objects++;
                    int j;
                    avg_distance = 0;
                    for (j = start_angle; j < end_angle; j++) {
                        avg_distance += distance[j];
                    }
                    avg_distance =  avg_distance / width;
                    double w = 2 * avg_distance / (tan((90 - (width / 2)) * (3.14159265 / 180)) - 1);
                    objects[num_objects][1] = w;
                    objects[num_objects][0] = start_angle + (width / 2);
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
            smallObject += 1;
        }
    }
//    printf("%d\n" , smallObject);
    //if small object detected
    if (smallObject >= 1)
    {
        //if it sees one small object
        if (num_objects == 1)
        {
            //if the one small object is over 20 cm away
            if (objects[1][2] > 20.0)
            {
                if (objects[1][0] < 22.5)
                {
                    *events = 12;
                }
                else if (objects[1][0] < 45 && objects[1][0] >= 22.5)
                {
                    *events = 13;
                }
                else if (objects[1][0] < 67.5 && objects[1][0] >= 45)
                {
                    *events = 14;
                }
                else if (objects[1][0] < 90 && objects[1][0] >= 67.5)
                {
                    *events = 15;
                }
                else if (objects[1][0] < 112.5 && objects[1][0] >= 90)
                {
                    *events = 16;
                }
                else if (objects[1][0] < 135 && objects[1][0] >= 112.5)
                {
                    *events = 17;
                }
                else if (objects[1][0] < 157.5 && objects[1][0] >= 135)
                {
                    *events = 18;
                }
                else
                {
                    *events = 19;
                }
            }
            //if the single small object is within 20 cm away
            else
            {
                *events = 7;//TODO
            }
        }
        if (smallObject >= 2)
        {
            //if it sees small objects and that is all it sees
            if (smallObject == num_objects)
            {
                int angle = (objects[1][0] + objects[2][0]) / 2;
                if (angle < 90)
                {
                    *events = 100 + (90 - angle);
                }
                else
                {
                    *events = 200 + (angle - 90);
                }
            }
            //if it sees small objects and tall objects
            else
            {
                *events = 7;//TODO

            }
        }
    }
    //if no small object detected
    if (smallObject == 0)
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
            for (i = 1; i <= num_objects; i++)
            {
                if (objects[i][0] > 40 && objects[i][0] <= 90)
                {
                    *events = 9;
                    break;
                }
                if (objects[i][0] >= 130 && objects[i][0] < 90)
                {
                    *events = 8;
                    break;
                }
            }

        }
        //if it can move directly forward
        if (move_forward != 1)
        {
            *events = 7;
        }
    }
}


//quick scan
void scan_quick() {
    double ir_boundary = 60.0; //cm
    double ping_boundary = 70.0; //cm
    int voltage;
    double ping_distance;
    double ir_distance;
    double calibration = 0;
    int detected = 0;

    servo_move(0);
    timer_waitMillis(1000);
    int i = 0;
    for (i = 0; i <= 180; i += 4) {
        servo_move(i);
        timer_waitMillis(50);
        ping_distance = ping_getDistance() * 0.0010625;  //.0010300
        voltage =  adc_read();
        ir_distance = adc_conversion(voltage, calibration);

        if (ping_distance < ping_boundary && ir_distance < ir_boundary) {
            lcd_printf("Object Detected");
            if (detected == 0) {
                detected = 1;
            }
        }
        else {
            lcd_printf("No Object");
        }
    }
    if (detected == 0)
    {
        *events = 7;
    }
    else
    {
        *events = 10;
    }
}




////goal scan
//int scan_goal()
//{
//    double objects[20][3]; //[Width][Angle]
//    double ir_boundary = 60.0; //cm
//    double ping_boundary = 70.0; //cm
//    int voltage;
//    double ping_distance;
//    double ir_distance;
//    double calibration = 8.5;
//    char sendStr[60];
//    double avg_distance = 0;
//    int start_angle;
//    int end_angle;
//    int detected = 0;
//    int num_objects = 0;
//    double distance[180];
//
//    servo_move(0);
//    timer_waitMillis(1000);
//    int i = 0;
//    for (i = 0; i <= 180; i++) {
//        servo_move(i);
////        timer_waitMillis(50);
//        ping_distance = ping_getDistance() * 0.0010625;  //.0010300
//        distance[i] = ping_distance;
//        voltage =  adc_read();
//        ir_distance = adc_conversion(voltage, calibration);
//
//        if (ping_distance < ping_boundary && ir_distance < ir_boundary) {
//            lcd_printf("Object Detected");
//            if (detected == 0) {
//                detected = 1;
//                start_angle = i;
//            }
//        }
//        else {
//            lcd_printf("No Object");
//            end_angle = i;
//            if (detected == 1) {
//                double width = 0;
//                if (end_angle - start_angle > 10)
//                {
//                    width = (end_angle - start_angle) - 8;
//                    start_angle -= 7;
//                    end_angle -= 15;
//                }
//                else
//                {
//                    width = end_angle - start_angle - 1;
//                    start_angle -= 7;
//                    end_angle -= 8;
//                }
//                if (width > 0) {
//                    num_objects++;
//
//                    int j;
//
//                    if (start_angle < 0) {
//                        start_angle = 0;
//                    }
//
//                    avg_distance = 0;
//                    for (j = start_angle; j <= end_angle; j++) {
//                        avg_distance += distance[j];
//                    }
//                    avg_distance =  avg_distance / width;
//                    double w = 2 * avg_distance / (tan((90 - (width / 2)) * (3.14159265 / 180)) - 1);
//                    objects[num_objects][1] = w;
//                    objects[num_objects][0] = start_angle + (width / 2) + 5;
//                    objects[num_objects][2] = avg_distance;
//                    sprintf(sendStr , "Object %d\r\nWidth(deg): %lf\r\nDistance(cm): %lf\r\nLinear Width(cm): %lf\r\n\n" , num_objects , width , avg_distance , w);
//                    uart_sendStr(sendStr);
//
//                }
//            }
//            detected = 0;
//        }
//
//    }
//
//    //analysis of data
//    for (i = 1; i <= num_objects; i++)
//    {
//        if (objects[i][1] < 7.5)
//        {
//            return 1;
//        }
//    }
//    return 0;
//}


//goal scan
int scan_goal()
{
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
                double width = end_angle - start_angle;
                if (width > 1) {
                    num_objects++;
                    int j;
                    avg_distance = 0;
                    for (j = start_angle; j < end_angle; j++) {
                        avg_distance += distance[j];
                    }
                    avg_distance =  avg_distance / width;
                    double w = 2 * avg_distance / (tan((90 - (width / 2)) * (3.14159265 / 180)) - 1);
                    objects[num_objects][1] = w;
                    objects[num_objects][0] = start_angle + (width / 2);
                    objects[num_objects][2] = avg_distance;
                    sprintf(sendStr , "Object %d\r\nWidth(deg): %lf\r\nDistance(cm): %lf\r\nLinear Width(cm): %lf\r\n\n" , num_objects , width , avg_distance , w);
                    uart_sendStr(sendStr);

                }
            }
            detected = 0;
        }

    }

    //analysis of data
    for (i = 1; i <= num_objects; i++)
    {
        if (objects[i][1] < 7.5)
        {
            return 1;
        }
    }
    return 0;
}
