/*
 * over_current.c
 *
 *  Created on: Aug 22, 2020
 *      Author: Justin Bak
 *      Code Owner: Justin Bak
 *
 * ADC on pins:
 * PIN     Channel
 * ---------------
 * 1.0        A
 * 1.1        B
 * 5.3        C
 */

#include <adc.h>

void setup_ADC(void (*cba)(int), void (*cbb)(int), void (*cbc)(int)) {
    // Configure ADC A1 and A0 pin
    P1SEL0 |= BIT1 + BIT0; // P1.0 & 1.1
    P1SEL1 |= BIT1 + BIT0;
    P5SEL0 |= BIT3;        // P5.3
    P5SEL1 |= BIT3;

    ADCCTL0 |= ADCSHT_2 | ADCON;        // ADCON, S&H=16 ADC clks
    ADCCTL1 |= ADCSHP;                  // ADCCLK = MODOSC; sampling timer
    ADCCTL2 &=~ ADCRES;                 // clear ADCRES in ADCCTL
    ADCCTL2 |= ADCRES_2;                // 12-bit conversion results
    ADCIE |= ADCIE0;                    // Enable conversion complete interrupt
    ADCMCTL0 |= ADCINCH_1 | ADCSREF_1;  // A1 ADC input select;

    adc_callbackA = cba;
    adc_callbackB = cbb;
    adc_callbackC = cbc;
}

void read_adc(int channel) {
    ADCMCTL0 = channel;
    ADCCTL0 |= ADCENC | ADCSC; // Sampling conversion start
}


// ADC10 interrupt service routine
#pragma vector=ADC_VECTOR
__interrupt void ADC10_ISR(void)
{
    int ADC_result = ADCMEM0;
    switch(ADCMCTL0) {
    case ADCINCH_0:
        if (adc_callbackA) {
            adc_callbackA(ADC_result);
        }
        break;
    case ADCINCH_1:
        if (adc_callbackB) {
            adc_callbackB(ADC_result);
        }
        break;
    case ADCINCH_11:
        if (adc_callbackC) {
            adc_callbackC(ADC_result);
        }
        break;
    default:
        break;
    }
}
