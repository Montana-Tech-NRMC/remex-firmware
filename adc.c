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
#include <i2c.h>

int channel = 3;
uint8_t* local_memory_map;

void init_adc(uint8_t* memory_map) {
    local_memory_map = memory_map;

    // Configure ADC A1 and A0 pin
    P1SEL0 |= BIT0 + BIT1 + BIT2 + BIT3; // P1.0 & 1.1 & 1.2 & 1.3
    P1SEL1 |= BIT0 + BIT1 + BIT2 + BIT3;

    ADCCTL0 |= ADCON | ADCSHT_2;                            // ADCON, sample-and-hold 16 ADCCLK cycles
    ADCCTL1 |= ADCSHP | ADCSHS_2 | ADCSSEL_1 | ADCCONSEQ_3; // ADC Pulse sample mode; TB1.1 Trigger; ADCCLK = ACLK; repeat sequence of channels
    ADCCTL2 &= ~ADCRES;
    ADCCTL2 |= ADCRES_2;                                    // 12-bit conversion resolution
    ADCMCTL0 |= ADCINCH_3;                                  // A0~3(EoS); Use internal 1.5v Ref
    ADCIE |= ADCIE0;                                        // Enable conversion complete interrupt

    ADCCTL0 |= ADCENC;                                      // ADC enable encoding

    TB1CCR0 = 1024-1;                                       // PWM Period
    TB1CCR1 = 512-1;                                        // CCR1 Duty cycle 50%
    TB1CCTL1 = OUTMOD_7;                                    // CCR1 reset/set
    TB1CTL = TBSSEL_1 | MC_1 | TBCLR;                       // ACLK, up mode
}

volatile int adcval = 0;

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
            break;
        case ADCIV_ADCLOIFG:
            break;
        case ADCIV_ADCINIFG:
            break;
        case ADCIV_ADCIFG:
            adcval = ADCMEM0;

            switch(channel) {
                case 0:
                    SHORT_TO_BYTES(local_memory_map, REGMAP_SIZE, ADC_A, adcval);
                    break;
                case 1:
                    SHORT_TO_BYTES(local_memory_map, REGMAP_SIZE, ADC_B, adcval);
                    break;
                case 2:
                    SHORT_TO_BYTES(local_memory_map, REGMAP_SIZE, ADC_C, adcval);
                    break;
                case 3:
                    SHORT_TO_BYTES(local_memory_map, REGMAP_SIZE, ADC_D, adcval);
                    break;
                default:
                    break;
            }

            if (--channel < 0) {
                channel = 3;
            }

            ADCIFG = 0;
            break;                                           // Clear CPUOFF bit from 0(SR)
        default:
            break;
    }
}
