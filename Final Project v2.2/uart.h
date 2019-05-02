#include <stdint.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"
#include <inc/tm4c123gh6pm.h>


//Initialize UART1 and PORTB
void serial_init();

//Transmit data to UART1
void UART1_Transmit(char sdata);

//Receive data to UART1
char UART1_Receive(void);

void UART1_Handler(void);

void uart_sendStr(const char *data);

void uart_sendChar(char data);

//checkpoint 1
void putty_display(void);

void putty_display2(void);

