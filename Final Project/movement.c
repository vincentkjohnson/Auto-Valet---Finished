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


void move_foward(oi_t *sensor_data, int millimeters) {
    int sum = 0;
    short boundary = 2700;
    short hole = 1500;

    //Right | Left 270, 258
     while (sum < millimeters ) {
         oi_update(sensor_data);
         if (sensor_data -> bumpLeft == 1 || sensor_data -> cliffFrontLeftSignal >= boundary || sensor_data -> cliffFrontLeftSignal <= hole && sensor_data -> bumpRight == 1 || sensor_data -> cliffFrontRightSignal >= boundary || sensor_data -> cliffFrontRightSignal <= hole) {
             oi_setWheels(0,0);
         } else {
             oi_setWheels(250,250);
         }
         sum += sensor_data -> distance;

         lcd_printf("Distance: %d", sum);

    }

    oi_setWheels(0,0);
}

int move_backward(oi_t *sensor_data, int millimeters) {
    int sum = 0;

    //Right | Left 270, 258
    oi_setWheels(-250,-250);
    while (abs(sum) < millimeters) {
        oi_update(sensor_data);
        sum += sensor_data -> distance;

        lcd_printf("Distance: %d", sum);
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
        lcd_printf("Angle: %f", sum);
    }
    oi_setWheels(0,0);
}

void turn_left(oi_t *sensor_data, double degrees) {
    double sum = 0;
    oi_setWheels(150, -150);
    while (abs(sum) < degrees) {
        oi_update(sensor_data);
        sum += sensor_data -> angle;
        lcd_printf("Angle: %f", sum);
    }
    oi_setWheels(0,0);
}

void moving_in_square(oi_t *sensor_data) {
    int loops = 0;
    while (loops < 4) {
        move_foward(sensor_data, 500);
        turn_right(sensor_data , 90.0);
        loops++;
    }
}

void collision_detection(oi_t *sensor_data, int distance) {
    int sum = 0;
    short boundary = 2700;
    short hole = 1500;

    while (sum < distance) {
        oi_update(sensor_data);
        if (sensor_data -> bumpLeft == 1 || sensor_data -> cliffFrontLeftSignal >= boundary || sensor_data -> cliffFrontLeftSignal <= hole ) {
            oi_setWheels(0, 0);
            sum -= move_backward(sensor_data, 150);
            //sum += sensor_data -> distance;
            turn_right(sensor_data, 90.0);
            move_foward(sensor_data, 250);
            turn_left(sensor_data, 90.0);
//            sum -= 150;
        }
        if (sensor_data -> bumpRight == 1 || sensor_data -> cliffFrontRightSignal >= boundary || sensor_data -> cliffFrontRightSignal <= hole) {
            oi_setWheels(0, 0);
            sum -= move_backward(sensor_data, 150);
            //sum += sensor_data -> distance;
            turn_left(sensor_data, 90.0);
            move_foward(sensor_data, 250);
            turn_right(sensor_data, 90.0);
//            sum -= 150;
        }
        oi_setWheels(125,125);
        sum += sensor_data -> distance;
        lcd_printf("Distance: %d", sum);
    }
    oi_setWheels(0,0);
}



