#include <stdint.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"
#include <inc/tm4c123gh6pm.h>

/*
 *
 * Authors: Vincent Johnson & Joshua French
 *
 */



void init_ADC();

int adc_read();

double adc_conversion(int voltage, double calibration);
