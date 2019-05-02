#include <stdint.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"
#include <inc/tm4c123gh6pm.h>

/*
 * Authors: Vincent Johnson & Joshua French
 */

long risingEdge;
long fallingEdge;
long pulseLength;

void ping_init(void);

void TIMER3_Handler();

void ping_read(void);

void ping_send(void);

float ping_getDistance(void);
