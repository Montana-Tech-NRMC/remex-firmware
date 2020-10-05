/*
 * over_current.c
 *
 *  Created on: Aug 22, 2020
 *      Author: Justin Bak
 *      Code Owner: Justin Bak
 */

#include "over_current.h"

void setup_ADC() {
    ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
    ADC10CTL1 = INCH_0;                         // input channel A1
    ADC10AE0 |= 0x01;                           // PA.1 ADC option select pin 1.0

}

void set_threshHold(int limit) {
    current_limit = limit;
}

void set_overcurrent_cb(void (*cb)(void)) {
    oc_callback = cb;
}

void read_current() {
    ADC10CTL0 |= ENC + ADC10SC; // Start sampling
}


// ADC10 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC10_VECTOR))) ADC10_ISR (void)
#else
#error Compiler not supported!
#endif
{
    if (ADC10MEM > current_limit) {
        oc_callback();
    }

    __bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
}
