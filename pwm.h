/*
 * pwm.h
 *
 *  Created on: Aug 22, 2020
 *      Author: Justin Bak
 */

#ifndef PWM_H_
#define PWM_H_

#include <msp430.h>

#define uint_8 char

#define halt_dc 1550
// setup_timerA1_1 sets up the TimerA1.1 for use with PWM.
// period is the period of the PWM signal
// outmode is one of the 8 OUTMOD_X constants defined in msp430g2553.h
// duty_cycle defines the % uptime or the pwm signals duty cycle.
// control sets the control value of TA1CTL. ex ```TASSEL_2 + MC_1```
void setup_timerA1_1(int period, uint_8 outmode, int duty_cycle, int control);

/*
void setup_timerA1_0(void);

void setup_timerA0_1(void);

void setup_timerA0_0(void);
*/

// set_dutycycle changes the duty cycle value.
void set_dutycycle(int dutycycle);

#endif /* PWM_H_ */
