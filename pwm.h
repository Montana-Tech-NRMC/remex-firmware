/**
 * pwm.h
 *
 *  Created on: Oct 26, 2020
 *      Author: Andrew McLinden, Marcus Frisbee
 */
#include <msp430.h>
#ifndef PWM_H_
#define PWM_H_
#define PWMPINA BIT0 //pin 6.0 pwm
#define PWMPINB BIT1 //pin 6.1 pwm
#define MAX 4000
#define MIN 2000

/**
 *
 * init_PWM_A Initializes PWM code for motor A(pin and timer settings)
 *
 */
void init_pwm_A();

/**
 *
 * init_PWM_B Initializes PWM code for motor B (pin and timer settings)
 *
 */
void init_pwm_B();

/**
 * set_PWM_out_A Sets PWM to certain speed ranging from
 * full speed backwards(2000) to full speed forwards(4000)
 * for motor A
 *
 * @param[in] in value >=2000 and <=4000
 */

void set_pwm_A(const int in);

/**
 * set_PWM_out_B Sets PWM to certain speed ranging from
 * full speed backwards(2000) to full speed forwards(4000)
 * for motor B
 *
 * @param[in] in value >=2000 and <=4000
 */

void set_pwm_B(const int in);

/**
 * init_clk_src Sets the clock source of the PWM, the period of the clock,
 * and the count mode of the clock
 *
 */
void init_clk_src();

#endif /* PWM_H_ */
