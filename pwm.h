/*
 * pwm.h
 *
 *  Created on: Oct 26, 2020
 *      Author: Andrew McLinden
 */
#include <msp430.h>
#ifndef PWM_H_
#define PWM_H_
#define PWMPIN BIT2

/*
 *
 * init_PWM Initializes PWM code (pin and timer settings)
 *
 */
void init_PWM();

/*
 * set_PWM_out Sets PWM to certain speed ranging from
 * full speed backwards(1000) to full speed forwards(2000)
 *
 * @param[in] in *value >=1000 and <=2000
 */

void set_PWM_out(const int in);

#endif /* PWM_H_ */
