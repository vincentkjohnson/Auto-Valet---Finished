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
void event_10(void);
void event_11(oi_t **sensor_data);
void event_12(oi_t **sensor_data);
void event_13(oi_t **sensor_data);
void event_14(oi_t **sensor_data);
void event_15(oi_t **sensor_data);
void event_16(oi_t **sensor_data);
void event_17(oi_t **sensor_data);
void event_18(oi_t **sensor_data);
void event_19(oi_t **sensor_data);
void event_100(oi_t **sensor_data);
void event_200(oi_t **sensor_data);
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

    int start = 0;
    while (start == 0) {
        //logic to receive from app
        start = 1;
    }

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

        if (event == 10) { event_10(); }

        if (event == 11) { event_11(&sensor_data); }

        if (event == 12) { event_12(&sensor_data); }

        if (event == 13) { event_13(&sensor_data); }

        if (event == 14) { event_14(&sensor_data); }

        if (event == 15) { event_15(&sensor_data); }

        if (event == 16) { event_16(&sensor_data); }

        if (event == 17) { event_17(&sensor_data); }

        if (event == 18) { event_18(&sensor_data); }

        if (event == 19) { event_19(&sensor_data); }
//        //Check GOAL
        if (event >= 100 && event <= 190) { event_100(&sensor_data); }

        if (event >= 200 && event <= 290) { event_200(&sensor_data); }
    }
    unsigned char MarioD_songTone[11] = {71,77,77,77,77,76,74,72,76,76,72};
    unsigned char MarioD_songDuration[11] = {15,15,15,15,20,20,20,15,15,15,20};
    oi_loadSong(3, 11, MarioD_songTone,MarioD_songDuration);
    oi_play_song(3);

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
    lcd_printf("not clear");
    event = 0;
    turn_left(*sensor_data , 30);
}

void event_10(void) {
    lcd_printf("full scan");
    scan_area();
}

void event_11(oi_t **sensor_data) {
    lcd_printf("goal scan");
    int i = 0;
    i += scan_goal();
    turn_right(*sensor_data , 90);
    i += scan_goal();
    turn_right(*sensor_data , 90);
    i += scan_goal();
    turn_right(*sensor_data , 90);
    i += scan_goal();
    turn_right(*sensor_data , 90);
    if (i >= 3)
    {
        lcd_printf("Objective Complete");
        DONE = 1;
    }
    else
    {
        event == 0;
    }
}

void event_12(oi_t **sensor_data) {
    lcd_printf("1 small object");
    turn_right(*sensor_data , 78);
    event = 10;
    move_forward(*sensor_data , 100);

}

void event_13(oi_t **sensor_data) {
    lcd_printf("1 small object");
    turn_right(*sensor_data , 56);
    event = 10;
    move_forward(*sensor_data , 100);
}

void event_14(oi_t **sensor_data) {
    lcd_printf("1 small object");
    turn_left(*sensor_data , 34);
    event = 10;
    move_forward(*sensor_data , 100);

}

void event_15(oi_t **sensor_data) {
    lcd_printf("1 small object");
    turn_left(*sensor_data , 11);
    event = 10;
    move_forward(*sensor_data , 100);
}

void event_16(oi_t **sensor_data) {
    lcd_printf("1 small object");
    turn_left(*sensor_data , 11);
    event = 10;
    move_forward(*sensor_data , 100);
}

void event_17(oi_t **sensor_data) {
    lcd_printf("1 small object");
    turn_left(*sensor_data , 34);
    event = 10;
    move_forward(*sensor_data , 100);
}

void event_18(oi_t **sensor_data) {
    lcd_printf("1 small object");
    turn_left(*sensor_data , 56);
    event = 10;
    move_forward(*sensor_data , 100);
}

void event_19(oi_t **sensor_data) {
    lcd_printf("1 small object");
    turn_left(*sensor_data , 78);
    event = 10;
    move_forward(*sensor_data , 100);
}

void event_100(oi_t **sensor_data) {
    event -= 100;
    turn_right(*sensor_data , event);
    event = 11;
    plow_forward(*sensor_data , 400);
    if (event == 1 || event == 2)
    {
        DONE = 1;
    }
}

void event_200(oi_t **sensor_data) {
    event -= 200;
    turn_left(*sensor_data , event);
    event = 11;
    plow_forward(*sensor_data , 400);
    if (event == 1 || event == 2)
    {
        DONE = 1;
    }
}
