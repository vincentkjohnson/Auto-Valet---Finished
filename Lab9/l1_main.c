#include "lcd.h"
#include "open_interface.h"
#include "lab5_scan_data.h"
#include "uart.h"
#include "adc.h"
#include "ping.h"
#include "servo.h"
#include "button.h"
#include <math.h>

/**
 * main.c
 *Author: Vincent Johnson && Joshua French
 *
 * @date 04-1-19
 */
volatile int char_event = 0;
volatile char s_data[21];
volatile int i;
volatile double objects[20];

void main(void)
{
    //Initialize the LCD. This also clears the screen
    lcd_init();
    servo_init();
    button_init();
    init_ADC();
    ping_init();
    serial_init(&char_event , &s_data[0] , &i);

//    servo_move(90);
//    while(1) {
//        timer_waitMillis(500);
//        int voltage = adc_read();
//        double calibration = 0.0;
//        lcd_printf("%d, %f", voltage, adc_conversion(voltage, calibration));
//    }



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

//    sprintf(sendStr, "%-20s%-20s%-20s\r\n", "Degrees", "IR Distance (cm)", "Sonar Distance(cm)");
//    uart_sendStr(sendStr);

    servo_move(0);
    timer_waitMillis(1000);
    int i = 0;
    for (i = 0; i <= 180; i++) {
        servo_move(i);
        timer_waitMillis(50);
        ping_distance = ping_getDistance() * 0.0010300;  //.0010300
        voltage =  adc_read();
        ir_distance = adc_conversion(voltage, calibration);

        if (ping_distance < ping_boundary && ir_distance < ir_boundary) {
            lcd_printf("Object Detected");
            if (detected == 0) {
                detected = 1;
                start_angle = i;
                //num_objects++;
            }
            avg_distance += ping_distance;
        }
        else {
            lcd_printf("No Object");
            end_angle = i;
            if (detected == 1) {
                double width = (end_angle - start_angle) / 2;
                if (width > 2.0) {
                    num_objects++;
                    avg_distance =  avg_distance / (2 * width);
                    double w = 2 * avg_distance * sin(width * 3.14159265 / 360); //linear width
                    //double w = 2 * avg_distance / (tan((90 - (width / 2)) * (3.14159265 / 180)) - 1);
                    sprintf(sendStr , "Object %d\r\nWidth(deg): %lf\r\nDistance(cm): %lf\r\nLinear Width(cm): %lf\r\n\n" , num_objects , width , avg_distance , w);
                    uart_sendStr(sendStr);
                    objects[num_objects] = w;
                }
            }
            detected = 0;
            avg_distance = 0;
        }
//        sprintf(sendStr ,"%d\t\t\t%.2f\t\t%.2f\r\n", i, ir_distance, ping_distance);
//        uart_sendStr(sendStr);


    }


    lcd_printf("Objects: %d" , num_objects);
}


