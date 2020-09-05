/*
 * Author: Justin Bak
 * Code Owner: Justin Bak
 */
#include <msp430.h> 
#include "pwm.h"
#include "over_current.h"


/**
 * main.c
 */
#define PWM 0x02
#define ever (;;)

int dutycycle = 1550;

void stop_pwm() {
    set_dutycycle(halt_dc);
}

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    // Defines the pins
    P2DIR |= BIT2; // Set Pin 1.2 as an output
    P2SEL |= BIT2; // Select Pin 1.2 as PWM output

	setup_ADC();

	set_overcurrent_cb(&stop_pwm); // set the over current call back function

	setup_timerA1_1(
	        20000, // period of pwm signal 20ms
	        OUTMOD_7, // reset/set
	        10000, // 50% duty cycle
	        TASSEL_2 + MC_1 // TASSEL_2 selects the SMCLK as the source, MC1  tells it to count up to the value in TA1CCR0
	);
	
    set_dutycycle(dutycycle);

    __bis_SR_register(GIE); // Enable global interupts

	for ever {
	    read_current();
	}

	return 0;
}


