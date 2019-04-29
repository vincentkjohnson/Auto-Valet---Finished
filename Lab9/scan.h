#include <stdint.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"
#include <inc/tm4c123gh6pm.h>

/*
 *
 * Authors: Vincent Johnson & Joshua French
 *
 */

/**
 * method scans a 180 degree arc in front of the robot,
 * and prints out any objects width and angle of location onto putty.
 *
 * Then finds the smallest object and prints the  angle and width to LCD.
 */

extern double objects[20][3];
extern double angle_small;
extern double distance_small;

void scan_area();
void scan_init(int *event1);
