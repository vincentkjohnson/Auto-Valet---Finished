#include <stdint.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"
#include <inc/tm4c123gh6pm.h>

/**
 * Servo.h file
 * @Author Vincent Johnson && Joshua French
 */

/*
 * variable to keep track of current degree of servo
 */
double currentDegree;

/*
 * Method that initializes the timer1B to control
 * servo with sensors
 */
void servo_init(void);

/*
 * Method used to move the servo, in takes a variable
 * of degree and moves the servo with sensors accordingly
 */
void servo_move(double degrees);

/*
 * Method used to move the servo based off which button
 * was pushed. 0 and 180 degree restraints in place.
 */
void button_servo_move();
