#include "uart.h"
#include "lcd.h"
#include "stdio.h"

volatile int *char_event_ptr;
volatile char *s_data_ptr;
volatile int *i_ptr;


#define BIT0        0x01
#define BIT1        0x02
#define BIT2        0x04
#define BIT3        0x08
#define BIT4        0x10
#define BIT5        0x20
#define BIT6        0x40
#define BIT7        0x80

//Initialize UART1
void serial_init(int *char_event_addr , char *s_data_addr , int *i_addr) {
    //1.Set up GPIO
    //A. Configure GPIO module associated with UART1
    // i. Turn on Clock for PortB and UART1
    SYSCTL_RCGCGPIO_R |= 0x02 | 0x02;
    SYSCTL_RCGCUART_R = 0x02;

    // ii. Enable alternate function for Pb, Pb and set functionality
    GPIO_PORTB_AFSEL_R |= (BIT1 | BIT0); // UART1 TX and RX
    GPIO_PORTB_PCTL_R |= 0x11;  //Set port B pins to 0, 1 to RX, TX

    // iii. Set digital or analog mode and pin directions
    GPIO_PORTB_DEN_R |= (BIT1 | BIT0);
    GPIO_PORTB_DIR_R &= ~BIT0; //Set pin 0 (Rx) to input
    GPIO_PORTB_DIR_R |= BIT1; //Set pin 1 (Tx) to output

    //2. Set up UART device
    //A. Configure UART functionality, frame format and Baud speed

    UART1_IM_R  &= ~0b00010000;
    //Disable UART1 device while we set it up
    UART1_CTL_R &= 0xFFFFFFFE;

    //Set desired UART functionality
    UART1_CTL_R = 0b1100000000; //Receive enable, Transmit enable

    //Set up baud rate (9600 Baud)
    UART1_IBRD_R = 8; //checkpoint 2 = 104  | checkpoint 3 = 8
    UART1_FBRD_R = 44;  //checkpoint 2 = 11   | checkpoint 3 = 44
    UART1_CC_R = 0x00; //Use system clock as UART clock source

    //Set frame format: use frame format specified above
    UART1_LCRH_R = 0x60;

    //B. Setup UART1 interrupts
    UART1_ICR_R |= 0b00010000;
    UART1_IM_R  |= 0b00010000;

    //
    char_event_ptr = char_event_addr;
    s_data_ptr = s_data_addr;
    i_ptr = i_addr;

    //3. NVIC Setup
    //A. Configure NVIC to allow UART interrupts
    NVIC_EN0_R |= 0x00000040;

    //B. Bind UART1 interrupt requests to user's interrupt handler
    IntRegister(INT_UART1, UART1_Handler);

    //Re-enable UART10
    UART1_CTL_R = UART1_CTL_R | 0x01;
    IntMasterEnable(); //Globally allows CPU to service interrupts
}

void UART1_Handler() {
    UART1_ICR_R |= 0x30;
    *char_event_ptr = 1;
    volatile char *temp_ptr = s_data_ptr + *i_ptr;
    *temp_ptr = UART1_Receive();
    UART1_Transmit(*temp_ptr);
    *i_ptr += 1;
}

void UART1_Transmit(char sdata) {
    while (UART1_FR_R & 0x20);
    UART1_DR_R = sdata;
}

char UART1_Receive(void) {
    char rdata;
    while(UART1_FR_R & 0x10);
    rdata = (char) (UART1_DR_R & 0xFF);
    return rdata;
}

void uart_sendStr(const char *data) {
    //until we reach a null character
    while (*data != '\0')
    {
        //send the current character
        uart_sendChar(*data);
        // increment the pointer.
        data++;
    }
}

void uart_sendChar(char data) {
    UART1_Transmit(data);
}

void putty_display(void) {
    char s_data[20];
    int i = 0;
    while (1) {
        for (i = 0; i < 20; i++) {
            s_data[i] = UART1_Receive();
            if (s_data[i] == '\r') {
                int j;
                for (j = i; j < 20; j++) {
                    s_data[j] = 0x20;
                }
                break;
            }
            lcd_printf("%d %c" , i + 1 , s_data[i]);
        }
        lcd_printf("%s", s_data);
    }
}

void putty_display2(void) {
    char s_data[20];
    int i = 0;
    while (1) {
        for (i = 0; i < 20; i++) {
            s_data[i] = UART1_Receive();
            UART1_Transmit(s_data[i]);
            if (s_data[i] == '\r') {
                int j;
                for (j = i; j < 20; j++) {
                    s_data[j] = 0x20;
                }
                UART1_Transmit(0xA);
                break;
            }
            lcd_printf("%d %c" , i + 1 , s_data[i]);
        }

        lcd_printf("%s", s_data);
    }
}

