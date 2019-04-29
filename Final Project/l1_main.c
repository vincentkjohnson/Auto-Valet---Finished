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

//Main Functions to be used:
void event_0(void);
void event_1(oi_t **sensor_data);
void event_2(oi_t **sensor_data);
void event_3(oi_t **sensor_data);
void event_4(oi_t **sensor_data);
void event_5(oi_t **sensor_data);
void event_6(oi_t **sensor_data);
void event_7(oi_t **sensor_data);
void event_8(oi_t **sensor_data);
void event_9(oi_t **sensor_data);
void event_CHECK_GOAL(oi_t **sensor_data);

//Pointer variables from other programs
// --------------------------
volatile int event = 0;
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
    serial_init(&event , &s_data[0] , &i);
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    move_init(&event);
    scan_init(&event);

    while (DONE == 0) {
        //Scanning
        if (event == 0) { event_0(); }
        //Bump right
        if (event == 1) { event_1(&sensor_data); }
//        //Bump left
        if (event == 2) { event_2(&sensor_data); }
//        //Right Border
        if (event == 3) { event_3(&sensor_data); }
//        //Left Border
        if (event == 4) { event_4(&sensor_data); }
//        //Right Cliff
        if (event == 5) { event_5(&sensor_data); }
//        //Left Cliff
        if (event == 6) { event_6(&sensor_data); }
//        //Path is clear, keep moving forward
        if (event == 7) { event_7(&sensor_data); }
//        //Obstructions in path
        if (event == 8) { event_8(&sensor_data); }
//        //Found small object that is approximately 7.5cm
        if (event == 9) { event_9(&sensor_data); }
//        //Check GOAL
        if (event == 200) { event_CHECK_GOAL(&sensor_data); }
    }

    unsigned char V_songTone[16] = {70,76,69,60,78,72,69,60,71,70,76,57,72,78,72,69};
    unsigned char V_songDuration[16] = { 16, 16, 16, 32, 16, 16, 16, 8, 8, 16, 16, 16, 32, 16, 16, 64};

    oi_loadSong(1, 16, V_songTone, V_songDuration);
    oi_play_song(1);

    oi_free(sensor_data);
    return 0;
}

/*
 * Scan
 */
void event_0(void) {
    lcd_printf("scanning");
    scan_area();
}

/*
 * Bump Right
 */
void event_1(oi_t **sensor_data) {
    lcd_printf("right bump");
    move_backward(*sensor_data , 50);
    turn_left(*sensor_data , 16);
    event = 0;
}

/*
 * Bump Left
 */
void event_2(oi_t **sensor_data) {
    lcd_printf("left bump");
    move_backward(*sensor_data , 50);
    turn_right(*sensor_data , 14);
    event = 0;
}

/*
 * Right Border
 */
void event_3(oi_t **sensor_data) {
    lcd_printf("right border");
    move_backward(*sensor_data , 50);
    turn_left(*sensor_data , 15);
    event = 0;
}

/*
 * Left Border
 */
void event_4(oi_t **sensor_data) {
    lcd_printf("left border");
    move_backward(*sensor_data , 50);
    turn_right(*sensor_data , 15);
    event = 0;
}

/*
 * Right Cliff
 */
void event_5(oi_t **sensor_data) {
    lcd_printf("right cliff");
    move_backward(*sensor_data , 50);
    turn_left(*sensor_data , 15);
    event = 0;
}

/*
 * Left Cliff
 */
void event_6(oi_t **sensor_data) {
    lcd_printf("left cliff");
    move_backward(*sensor_data , 50);
    turn_right(*sensor_data , 15);
    event = 0;
}

/*
 * Path is clear,
 * Keep moving
 */
void event_7(oi_t **sensor_data) {
    lcd_printf("all clear");
    move_forward(*sensor_data , 250);
    event = 0;
}

/*
 * Obstructions in path,
 * Turn and scan again
 */
void event_8(oi_t **sensor_data) {
    lcd_printf("not clear");
    event = 0;
    turn_right(*sensor_data , 30);
}

/*
 * Detected
 */
void event_9(oi_t **sensor_data) {
    lcd_printf("Found small object");
    event = 0;

    angle_small = 45.0;
    distance_small = 50.0;

    if (angle_small < 90) {
        turn_left(*sensor_data, angle_small);
    } else if (angle_small > 90) {
        turn_right(*sensor_data, angle_small);
    }

    //Line bot 20cm from small object
    if (distance_small >= 20) {
        double temp_distance = distance_small - 20;
        move_forward(*sensor_data, (distance_small - temp_distance) * 10);
    } else {
        double temp_distance = 20 - distance_small;
        move_backward(*sensor_data, (distance_small + temp_distance) * 10);
    }

    DONE = 1;
}

void event_CHECK_GOAL(oi_t **sensor_data) {
    lcd_printf("Checking Goal Posts");
    int total_objects = 0;
    int count = 1;
    while (count <= 2) {
        scan_area();
        int small_objects = 0;
        int j = 0;
        for (; j < num_objects; j++) {
            if (objects[j][1] < 8) {
                small_objects++;
            }
        }

        if (small_objects > 1) {
            total_objects += small_objects;
        }
        turn_left(*sensor_data, 180);
        count ++;
    }

    if (total_objects == 4) {
        DONE = 1;
        event = 0;
    } else {
        DONE = 0;
    }
}
