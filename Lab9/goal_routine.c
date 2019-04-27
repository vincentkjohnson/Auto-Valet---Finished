#include "lcd.h"
#include "open_interface.h"
#include "lab5_scan_data.h"
#include "uart.h"
#include "adc.h"
#include "ping.h"
#include "servo.h"
#include "button.h"
#include <math.h>
#include "scan.h"
/*
 * Routine to track Small tall pillars
 */
double objects[20][3];

void goal_routine(int object_event, int object_Number_One, int object_Number_Two, double angle_One, double angle_Two, double distance_One, double distance_Two) {
    int event = object_event;
    int objOne = object_Number_One;
    int objTwo = object_Number_Two;
    double angleOne = angle_One;
    double angleTwo = angle_Two;
    double distOne = distance_One;
    double distTwo = distance_Two;


    //1 small object with 5.5 cm
    if (event == 400) {
        if (angleOne < 90) {
            turn_left(sensor_data, angleOne);
        } else if (angle > 90) {
            turn_right(sensor_data, angleOne);
        }

        //Line bot 20cm from small object
        if (distanceOne >= 20) {
            double distance = distOne - 20;
            move_foward(sensor_data, distOne - distance);
        } else {
            double distnace = 20 - distOne;
            move_backward(sensor_data, distOne + distance);
        }
    }

    //2 small object with 5.5 cm but objects in between
    if (event == 401) {
        double h;

        if (angleOne > 90 && angleOne > angleTwo) {
            h = Math.sin(angleOne) * distOne;
        } else if (angleTwo > 90 && angleTwo > angleOne) {
            h = Math.sin(angleTwo) * distTwo;
        } else {
            h = Math.sin(angleTwo) * distTwo;
        }
    }

    //2 small objects nothing in between
    if (event == 402) {
        //Law of Cosines

    }
}
