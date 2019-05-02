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

void move_init(int *event1)
{
    events = event1;
}

void move_forward(oi_t *sensor_data, int millimeters) {
    int sum = 0;

    //Right | Left 270, 258
    oi_setWheels(100,100);
    while (sum < millimeters) {
        oi_update(sensor_data);
        sum += sensor_data -> distance;
        int fl = sensor_data -> cliffFrontLeftSignal;
        int fr = sensor_data -> cliffFrontRightSignal;
        int l = sensor_data -> cliffLeftSignal;
        int r = sensor_data -> cliffRightSignal;

        //Hit something on the right
        if (sensor_data -> bumpRight == 1) {
            *events = 1;
            break;
        }

        //Hit something on the left
        if (sensor_data -> bumpLeft == 1) {
            *events = 2;
            break;
        }

        //Right Border
        if (fr >= 2700 || r >= 2700) {
            *events = 3;
            break;
        }

        //Left Border
        if (fl >= 2700 || l >= 2700) {
            *events = 4;
            break;
        }

        //Right Cliff
        if (fr <= 900 || r <= 900) {
            *events = 5;
            break;
        }

        //Left Cliff
        if (fl <= 900 || l <= 900) {
            *events = 6;
            break;
        }
    }

    oi_setWheels(0,0);
}

void plow_forward(oi_t *sensor_data, int millimeters)
{
    int sum = 0;

    //Right | Left 270, 258
    oi_setWheels(100,100);
    while (sum < millimeters) {
        oi_update(sensor_data);
        sum += sensor_data -> distance;
        int fl = sensor_data -> cliffFrontLeftSignal;
        int fr = sensor_data -> cliffFrontRightSignal;
        int l = sensor_data -> cliffLeftSignal;
        int r = sensor_data -> cliffRightSignal;
        if (sensor_data -> bumpRight == 1)
        {
            *events = 1;
        }
        if (sensor_data -> bumpLeft == 1)
        {
            *events = 2;
        }
        //Right Cliff
        if (fr <= 900 || r <= 900) {
            *events = 1;
            break;
        }

        //Left Cliff
        if (fl <= 900 || l <= 900) {
            *events = 2;
            break;
        }
        //Right Border
        if (fr >= 2700 || r >= 2700) {
            *events = 1;
            break;
        }

        //Left Border
        if (fl >= 2700 || l >= 2700) {
            *events = 2;
            break;
        }
    }
    oi_setWheels(0,0);
}

int move_backward(oi_t *sensor_data, int millimeters) {

    unsigned char Back_songTone[16] = {96,96,96,96,96,96,96,96,96};
    unsigned char Back_songDuration[16] = {16,16,16,16,16,16,16,16,16};
    oi_loadSong(2, 16, Back_songTone, Back_songDuration);
    oi_play_song(2);

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
    }
    oi_setWheels(0,0);
}

void turn_left(oi_t *sensor_data, double degrees) {
    double sum = 0;
    oi_setWheels(150, -150);
    while (abs(sum) < degrees) {
        oi_update(sensor_data);
        sum += sensor_data -> angle;
    }
    oi_setWheels(0,0);
}

