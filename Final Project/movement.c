/*
 * movement.c
 *
 *  Created on: Feb 4, 2019
 *      Author: vincentj && jkfrench
 */

#include "open_interface.h"
#include "movement.h"
#include "lcd.h"
#include "math.h"


volatile int *events;
int distance = 50;

void move_init(int *event1) {
    events = event1;
}

void move_forward(oi_t *sensor_data, int millimeters) {
    int sum = 0;

    //Right | Left 270, 258
    oi_setWheels(100,100);
    while (sum < millimeters) {
        oi_update(sensor_data);
        sum += sensor_data -> distance;

        //Front Left Signal
        int fl = sensor_data -> cliffFrontLeftSignal;
        //Front Right Signal
        int fr = sensor_data -> cliffFrontRightSignal;
        //Left Signal
        int l = sensor_data -> cliffLeftSignal;
        //Right Signal
        int r = sensor_data -> cliffRightSignal;

        //Right Bumper has been hit.
        if (sensor_data -> bumpRight == 1) {
            *events = 1;
            break;
        }

        //Left Bumper has been hit
        if (sensor_data -> bumpLeft == 1) {
            *events = 2;
            break;
        }

        //Threshold for Boundary on right
        if (fr >= 2700 || r >= 2700) {
            *events = 3;
            break;
        }

        //Threshold for Boundary on left
        if (fl >= 2700 || l >= 2700) {
            *events = 4;
            break;
        }

        //Threshold for Hole on right
        if (fr <= 1200 || r <= 1200) {
            *events = 5;
            break;
        }

        //Threshold for Hole on left
        if (fl <= 1200 || l <= 1200) {
            *events = 6;
            break;
        }
    }
    oi_setWheels(0,0);
}

int move_backward(oi_t *sensor_data, int millimeters) {
    int sum = 0;

    //Right | Left 270, 258
    oi_setWheels(-100,-100);
    while (abs(sum) < millimeters) {
        oi_update(sensor_data);
        sum += sensor_data -> distance;
    }

    oi_setWheels(0,0);
    return abs(sum);
}

void turn_right(oi_t *sensor_data, double degrees) {
    double sum = 0;
    oi_setWheels(-150, 150);
    while (abs(sum) < degrees) {
        oi_update(sensor_data);
        sum += sensor_data -> angle;
        //lcd_printf("Angle: %f", sum);
    }
    oi_setWheels(0,0);
}

void turn_left(oi_t *sensor_data, double degrees) {
    double sum = 0;
    oi_setWheels(150, -150);
    while (abs(sum) < degrees) {
        oi_update(sensor_data);
        sum += sensor_data -> angle;
       // lcd_printf("Angle: %f", sum);
    }
    oi_setWheels(0,0);
}

void collision_detection(oi_t *sensor_data, int event) {
    if (event == 0) {
        lcd_printf("scanning");
        timer_waitMillis(100);
        move_forward(sensor_data , 250);

    }
    //bump right
    if (event == 1) {
        lcd_printf("right bump");
        move_backward(sensor_data , 50);
        turn_left(sensor_data , 16);
        event = 0;
        move_forward(sensor_data , 250);
    }
    //bump left
    if (event == 2) {
        lcd_printf("left bump");
        move_backward(sensor_data , 50);
        turn_right(sensor_data , 14);
        event = 0;
        move_forward(sensor_data , 250);
    }
    //right border
    if (event == 3) {
        lcd_printf("right border");
        move_backward(sensor_data , 50);
        turn_left(sensor_data , 15);
        event = 0;
        move_forward(sensor_data , 250);
    }
    //left border
    if (event == 4) {
        lcd_printf("left border");
        move_backward(sensor_data , 50);
        turn_right(sensor_data , 15);
        event = 0;
        move_forward(sensor_data , 250);
    }
    //right cliff
    if (event == 5) {
        lcd_printf("right cliff");
        move_backward(sensor_data , 50);
        turn_left(sensor_data , 15);
        event = 0;
        move_forward(sensor_data , 250);
    }
    //left cliff
    if (event == 6) {
        lcd_printf("left cliff");
        move_backward(sensor_data , 50);
        turn_right(sensor_data , 15);
        event = 0;
        move_forward(sensor_data , 250);
    }
    //Obstacle Detected in front
    if (event == 7) {
        lcd_printf("Obstacle in front");
        move_backward(sensor_data, 50);
        turn_right(sensor_data, 90);
    }

}



