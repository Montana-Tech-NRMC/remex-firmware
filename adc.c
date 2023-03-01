/**
 * over_current.c
 *
 *  Created on: Aug 22, 2020
 *      Author: Justin Bak
 *      Code Owner: Justin Bak
 *
 * ADC on pins:
 * PIN     Channel
 * ---------------
 * 1.0        0
 * 1.1        1
 * 1.2        2
 * 1.3        3
 */

#include <adc.h>

const int high_threshold = 3200;
int channel = 0;

void init_adc(void (*cba)(int), void (*cbb)(int), void (*cbc)(int)) {
    // Configure ADC A1 and A0 pin
    P1SEL0 |= BIT1 + BIT0 + BIT2 + BIT3; // P1.0 & 1.1 & 1.2 & 1.3
    P1SEL1 |= BIT1 + BIT0 + BIT2 + BIT3;

    ADCCTL0 |= ADCON | ADCSHT_2;                            // ADCON, sample-and-hold 16 ADCCLK cycles
    ADCCTL1 |= ADCSHP | ADCSHS_2 | ADCSSEL_1 | ADCCONSEQ_3; // ADC Pulse sample mode; TB1.1 Trigger; ADCCLK = ACLK; repeat sequence of channels
    ADCCTL2 &= ~ADCRES;
    ADCCTL2 |= ADCRES_2;                                    // 12-bit conversion resolution
    ADCMCTL0 |= ADCINCH_3;                                  // A0~3(EoS); Use internal 1.5v Ref
    ADCHI = high_threshold;                                 // Set the high threshold for interrupt
    ADCIE |= ADCIE0 | ADCHIIE;                              // Enable conversion complete interrupt

    ADCCTL0 |= ADCENC;                                      // ADC enable encoding

    TB1CCR0 = 1024-1;                                       // PWM Period
    TB1CCR1 = 512-1;                                        // CCR1 Duty cycle 50%
    TB1CCTL1 = OUTMOD_7;                                    // CCR1 reset/set
    TB1CTL = TBSSEL_1 | MC_1 | TBCLR;                       // ACLK, up mode

    P6DIR |= BIT6;
}

// ADC interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC_VECTOR))) ADC_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(ADCIV,ADCIV_ADCIFG))
    {
        case ADCIV_NONE:
            break;
        case ADCIV_ADCOVIFG:
            break;
        case ADCIV_ADCTOVIFG:
            break;
        case ADCIV_ADCHIIFG:
            ADCIFG = 0;
            __no_operation();
            break;
        case ADCIV_ADCLOIFG:
            break;
        case ADCIV_ADCINIFG:
            break;
        case ADCIV_ADCIFG:
            if (ADCMEM0 < 0x200) {                            // ADCMEM = A0 < 0.5V?
                __no_operation();
                P6OUT &=~ BIT6;
            } else {
                P6OUT |= BIT6;
            }
            ADCIFG = 0;
            break;                                           // Clear CPUOFF bit from 0(SR)
        default:
            break;
    }
    channel++;
    if (channel > 3) {
        channel = 0;
    }
}
