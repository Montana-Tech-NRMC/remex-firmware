/*
 * pwm.c
 *
 *  Created on: Oct 26, 2020
 *      Author: Andrew McLinden
 */
#include "pwm.h"
void init_PWM() {
    P2DIR |= PWMPIN;            //set pin 2.2 to output direction
    P2SEL |= PWMPIN;            //select pin 2.2 as PWM output
    TA1CCR0 = 20000;            //Set period of timer A0 Capture/compare 0 register to 1000 us.
    TA1CCTL1 = OUTMOD_7;
    TA1CTL = TASSEL_2 + MC_1;   //TASSEL_2 selects SMCLK as the clock source and MC_1 tells it to count up to the value in TA0CCr0.
}
void set_PWM_out(const int in) {
    TA1CCR1 = in;               //period in microseconds that the power is ON.
}
