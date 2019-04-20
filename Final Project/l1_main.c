#include "lcd.h"
#include "open_interface.h"
#include "lab5_scan_data.h"
#include "uart.h"
#include "adc.h"
#include "ping.h"
#include "servo.h"
#include "button.h"
#include "movement.h"


/**
 * main.c
 *Author: Vincent Johnson && Joshua French
 *
 * @date 04-1-19
 */
volatile int char_event = 0;
volatile char s_data[21];
volatile int i;
volatile float data[180][2];

int main(void)
{
    //Initialize the LCD. This also clears the screen
    lcd_init();
//    servo_init();
//    button_init();
//    init_ADC();
//    ping_init();
//    serial_init(&char_event , &s_data[0] , &i);

    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

    collision_detection(sensor_data, 5000);


    oi_free(sensor_data);

//    int voltage;
//    double distance;
//    double calibration = 8.5;
//    char sendStr[60];
//
//    sprintf(sendStr, "%-20s%-20s%-20s\r\n", "Degrees", "IR Distance (cm)", "Sonar Distance(cm)");
//    uart_sendStr(sendStr);
//
//    int i = 0;
//    for (i = 0; i <= 180; i++) {
//        servo_move(i);
//        timer_waitMillis(100);
//        distance = ping_getDistance() * .0010625;
//        voltage =  adc_read();
//        data[i][0] = adc_conversion(voltage, calibration);
//        data[i][1] = distance;
//
//        sprintf(sendStr ,"%d\t\t\t%.2f\t\t%.2f\r\n", i, data[i][0], data[i][1]);
//        uart_sendStr(sendStr);
//    }
//
//    object_Distance(*data);

    return 0;

}


