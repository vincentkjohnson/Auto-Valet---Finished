/*
 * @adc.c
 * Set up file for ADC as well as functions used in averaging
 * incoming data and reading ADC data
 *
 * @Authors: Vincent Johnson & Joshua French
 * @dateL 3/11/19
 */
#include "adc.h"
#include "timer.h"
#include "math.h"

void init_ADC() {
    /*
     * Configure GPIO module associated with ADC:
     * 1. Turn on clock for GPIO Port B
     * 2. Enable alternate function
     * 3. Configure pins for input and analog mode
     * 4. Use default simple trigger source
     */

    //Turn on Clock for GPIO Port
    SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R | 0x02;
    SYSCTL_RCGCADC_R |= 0x1; //Enable ADC0 clock

    //Enable alternate function
    GPIO_PORTB_AFSEL_R |=  0x10;

    //Configure pints for input and analog mode
    GPIO_PORTB_DIR_R &= ~0x10;
    GPIO_PORTB_DEN_R &= ~0x10;
    GPIO_PORTB_AMSEL_R |= 0x10;

    //use default simple trigger source
    GPIO_PORTB_ADCCTL_R = 0x0;

    /*
     * Setup ADC
     * 1. Configure ADC
     * 2. Setup ADC Interrupts
     */

    //Configure ADC
    GPIO_PORTB_ADCCTL_R = 0x0; //Use sysClk as ADC clk.

    //Choose sample sequencer (SSO, SS1, SS2, or SS3)
    //Disable ADC sample sequence SSO while configuring
    //OK to use SS1 or SS2 (cannot use SS3 for multiple samples)
    ADC0_ACTSS_R &= 0xFFFE;

    //Set ADC SS0 Functionality
    ADC0_EMUX_R &= 0xFFF0;
    ADC0_SSMUX0_R |= 0x0000000A;

    //Configure which sample is end of sequence
    ADC0_SSCTL0_R |= 0x00000006;

    //ADC Sample Averaging Control (ADCSAC)
    ADC0_SAC_R |= 0x6;

    //Setup ADC Interrupts

    /*
     * Initialize NVIC
     * 1. Configure NVIC to allow ADC interrupts
     * 2. Bind ADC interrupt requests to user's interrupt handler
     * 3. Renable ADC SS0
     */

    //Configure NVIC to allow ADC interrupts
    //Bind ADC interrupt requests to user's interrupt handler

    //Renable ADC SS0
    ADC0_ACTSS_R |= 0x1; //Re-enable ADC0 SS0 by setting bit 0
}

int adc_read() {

    int pMeter;
    //Initiate SS0 conversion sequence
    ADC0_PSSI_R |= 0x01;

    //wait for SS0 ADC conversion to complete
    while ((ADC0_RIS_R & 0x1) == 0);

    //Read the 1st Sample from AIN0
    pMeter = ADC0_SSFIFO0_R;


    //Clear raw interrupt status flag for SS0
    ADC0_ISC_R |= 0x01;

    //Get converted results form SS0 to FIFO
    timer_waitMillis(100);

    return pMeter;

}

double adc_conversion(int voltage, double calibration) {
    return 50000*(pow(voltage , -1.113)) - calibration;
}
