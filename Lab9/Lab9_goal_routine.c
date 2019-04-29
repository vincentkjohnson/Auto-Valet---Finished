//#include "lcd.h"
//#include "open_interface.h"
//#include "lab5_scan_data.h"
//#include "uart.h"
//#include "adc.h"
//#include "ping.h"
//#include "servo.h"
//#include "button.h"
//#include <math.h>
//#include "scan.h"
//#include "movement.h"
///*
// * Routine to track Small tall pillars
// */
//double objects[20][3];
//
//void goal_routine(int* event, int object_event, int object_Number_One, int object_Number_Two, double angle_One, double angle_Two, double distance_One, double distance_Two) {
//    int done = 0;
//    int *moveEvent = event;
//    int objEvent = object_event;
//    int objOne = object_Number_One;
//    int objTwo = object_Number_Two;
//    double angleOne = angle_One;
//    double angleTwo = angle_Two;
//    double distOne = distance_One;
//    double distTwo = distance_Two;
//
//    /* CAN'T SEE FAR OBJECT
//     *  X    -60cm-    X
//     *
//     * [ ]
//     */
//
//
//    /* CAN'T SEE FAR OBJECT
//     * X    -60cm-    X
//     *
//     *               [ ]
//     */
//
//    //1 small object with 5.5 cm
//    if (objEvent == 400 && done == 0) {
//        if (angleOne < 90) {
//            turn_left(sensor_data, angleOne);
//        } else if (angle > 90) {
//            turn_right(sensor_data, angleOne);
//        }
//
//        //Line bot 20cm from small object
//        if (distanceOne >= 20) {
//            double distance = distOne - 20;
//            move_foward(sensor_data, (distOne - distance) * 100);
//        } else {
//            double distnace = 20 - distOne;
//            move_backward(sensor_data, (distOne + distance) * 100);
//        }
//    }
//
//    //2 small object with 5.5 cm but objects in between
//    if (objEvent == 401 && done = 0) {
//
//        /*      00000
//         * X    -60cm-    X
//         *
//         *       [ ]
//         */
//
//        double angle = (90 - angleOne) * 3.14159265 / 180;
//        double distance = distOne / sin(angle);
//        turn_left(sensor_data, 90 + angleOne);
//        move_forward(sensor_data, (distance + 60) * 100);
//        if (*moveEvent == 7) {
//            turn_right(sensor_data, 90 + angleOne);
//        }
//        else
//        {
//            move_backward(sensor_data , 50);
//            turn_right(sensor_data, 90 + angleOne);
//        }
//
//        move_foward(sensor_data, 600);
//
//
//    }
//
//    //2 small objects nothing in between
//    if (objEvent == 402 && done = 0) {
//
//        /*
//         * X    -60cm-    X
//         *
//         *       [ ]
//         */
//
//        if ((40 < angleOne < 90) && (130 > angleTwo > 90)) {
//            double distance = distTwo / cos(angleTwo * 3.14159265 / 180);
//            move_forward(sensors_data, (distance + 30) * 100);
//        }
//        done = 1
//    }
//
//    if (done = 1) {
//        scan_Goal();
//    }
//}
