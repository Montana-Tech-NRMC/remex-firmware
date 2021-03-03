/*
 * main.c
 *
 *  Created on: Oct 30, 2020
 *      Author: Justin Bak
 */

#include <msp430.h>
//#include "remex.h"

#define ever (;;)

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	// Change DCO main frequency to 16 MHz from 1 MHz
	__bis_SR_register(SCG0); // disable FLL
	CSCTL3 |= SELREF__REFOCLK; // Set REFO as FLL reference source
	CSCTL0 = 0; // clear DCO and MOD registers
	CSCTL1 &= ~(DCORSEL_7); // Clear DCO frequency select bits first
	CSCTL1 |= DCORSEL_5; // Set DCO = 16MHz
	CSCTL2 = FLLD_0 + 487; // DCOCLKDIV = 16MHz
	__delay_cycles(3);
	__bic_SR_register(SCG0); // enable FLL
	while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1)); // FLL locked

	CSCTL4 = SELMS__DCOCLKDIV | SELA__REFOCLK; // set default REFO(~32768Hz) as ACLK source, ACLK = 32768Hz
	// default DCOCLKDIV as MCLK and SMCLK source
	CSCTL5 |= DIVM__8; // SMCLK = MCLK = DCO/2 = 16 MHz/2 = 8 MHz

	init_PWM_A();
	init_PWM_B();

	for ever {
	    set_PWM_A(2000);
	    set_PWM_B(4000);
	}
}
