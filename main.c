/*
 * main.c
 *
 *  Created on: Oct 30, 2020
 *      Author: Justin Bak
 */

#include <msp430.h>
#include "remex.h"

#define ever (;;)

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	init();
	for ever {
	    loop();
	}
}


