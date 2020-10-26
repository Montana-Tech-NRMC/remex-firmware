/*
 * Author: Justin Bak
 * Code Owner: Justin Bak
 */
#include <msp430.h> 
#include "over_current.h"
#include "pwm.h"

/**
 * main.c
 */
#define ever (;;)

void stop_pwm() {
}

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	init_PWM();
	set_PWM_out(1500);

	setup_ADC();

	set_overcurrent_cb(&stop_pwm); // set the over current call back function

    __bis_SR_register(GIE); // Enable global interupts

	for ever {
	    read_current();
	}

	return 0;
}


