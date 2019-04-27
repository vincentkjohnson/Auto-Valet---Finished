#include "lcd.h"
#include "open_interface.h"
#include "lab5_scan_data.h"
#include "uart.h"
#include "adc.h"
#include "ping.h"
#include "servo.h"
#include "button.h"
#include "movement.h"

#define PI 3.14159265
/**
 * main.c
 *Author: Vincent Johnson && Joshua French && Xinlei Yu
 *
 * @date 04-1-19
 */

void scanOnce(void);

volatile int char_event = 0;
volatile char s_data[21];
volatile int i;
volatile float data[180][2];

int ser_total = 0;
int direction = 1;

//object detection variables
int obj_see = 0, degree_count = 0, total_obj_count = 0, smallest_degree = 180,
        smallest_obj_num = 0, starting_angle = 0, ending_angle = 0;
double mid_angle;
double smallest_dist_width = 0;

int scan_done = 0;

int obj_distance[5] = { 99, 99, 99, 99, 99 };
int obj_angle[5] = { 199, 199, 199, 199, 199 };
double obj_radius[5] = { 99, 99, 99, 99, 99 };
char display[80][161];

char str[80];
double temp;



int main(void)
 {
    //Initialize the LCD. This also clears the screen
    lcd_init();
    servo_init();
   button_init();
   init_ADC();
   ping_init();
    serial_init(&char_event , &s_data[0] , &i);

    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

  //  collision_detection(sensor_data, 5000);
        servo_move(0);
 timer_waitMillis(1000);
      //  scanOnce();

    oi_free(sensor_data);

    int voltage;
    double distance;
    double calibration = 8.5;
    char sendStr[60];

    sprintf(sendStr, "%-20s%-20s%-20s\r\n", "Degrees", "IR Distance (cm)", "Sonar Distance(cm)");
    uart_sendStr(sendStr);

    int i = 0;
    for (i = 0; i <= 180; i++) {
        servo_move(i);
        timer_waitMillis(100);
        distance = ping_getDistance() * .0010625;
        voltage =  adc_read();
        data[i][0] = adc_conversion(voltage, calibration);
        data[i][1] = distance;

        sprintf(sendStr ,"%d\t\t\t%.2f\t\t%.2f\r\n", i, data[i][0], data[i][1]);
        uart_sendStr(sendStr);
    }

    object_Distance(*data);

    return 0;

}

/*
 * function that scan high pillars, scan once and tell where are pillars located
 *
 *
 * hasn't finished
 *
 */

void scanOnce(void)
{
    //IR variables

    int voltage;
    double distance;
    double calibration = 8.5;
    char sendStr[60];



    int i = 0

    //IR
    for (i = 0; i <= 180; i++) {
            servo_move(i);
            timer_waitMillis(100);
            distance = ping_getDistance() * .0010625;
            voltage =  adc_read();
            data[i][0] = adc_conversion(voltage, calibration);
            data[i][1] = distance;

            sprintf(sendStr ,"%d\t\t\t%.2f\t\t%.2f\r\n", i, data[i][0], data[i][1]);
            uart_sendStr(sendStr);
        }

    timer_waitMillis(125);
}

