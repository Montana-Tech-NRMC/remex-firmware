/*
 * pwm.c
 *
 *  Created on: Oct 26, 2020
 *      Author: Andrew McLinden , Marcus Frisbee
 *
 *
 */
#include "pwm.h"
#include "i2c.h"

void init_pwm_A() {
    P6SEL0 |=  PWMPINA;
    P6DIR  |=  PWMPINA;           //set pin 6.0 to output direction
    P6SEL1 &=~ PWMPINA;           //select pin 6.0 as PWM output
    TB3CCTL1 = OUTMOD_7;         //ccr1 set/reset
    init_clk_src();
}

void init_pwm_B() {
    P6SEL0 |=  PWMPINB;
    P6DIR  |=  PWMPINB;            //set pin 6.1 to output direction
    P6SEL1 &=~ PWMPINB;           //select pin 6.1 as PWM output
    TB3CCTL2 = OUTMOD_7;         //ccr2 set/reset
    init_clk_src();
}

void set_pwm_A(const int in) {
    if (in > MAX) {
        TB3CCR1 = MAX;
    } else if (in < MIN) {
        TB3CCR1 = MIN;
    } else {
        TB3CCR1 = in;
    }
}

void set_pwm_B(const int in) {
    if (in > MAX) {
        TB3CCR2 = MAX;
    } else if (in < MIN) {
        TB3CCR2 = MIN;
    } else {
        TB3CCR2 = in;
    }
}

void init_clk_src(){
    TB3CCR0 = 40000;            //Set period of timer B0 Capture/compare 0 register to 2000 us.
    TB3CTL = TBSSEL__SMCLK + MC_1;   //TASSEL_SMCLK selects SMCLK as the clock source and MC_1 tells it to count up to the value in TB0CCr0.
}
