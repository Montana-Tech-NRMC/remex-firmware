/*
 * pwm.c
 *
 *  Created on: Aug 22, 2020
 *      Author: Justin Bak
 */

#include "pwm.h"


void setup_timerA1_1(int period, uint_8 output_mode, int duty_cycle, int control ) {
    TA1CCR0 = period; //Set the period in the Timer 1 Capture Compare 0 register in micro seconds.
    TA1CCTL1 = output_mode; // Set the output mode
    TA1CCR1 = duty_cycle; // The period in microseconds that the Power is ON.
    TA1CTL = control;
}

void set_dutycycle(int dutycycle) {
    TA1CCR1 = dutycycle; // set the dutycycle
}


