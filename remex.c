/*
 * Author: Justin Bak
 * Code Owner: Justin Bak
 */
#include <msp430.h> 
#include "remex.h"
#include "adc.h"

/**
 * remex.c
 */

//////////// Global Vars //////////////////
///////////////////////////////////////////

void adc_channel_a(int current) {
    volatile int foo = current;
    return;
}

void adc_channel_b(int current) {
    volatile int foo = current;
    return;
}

void adc_channel_c(int current) {
    volatile int foo = current;
    return;
}

// init is called once at the beginning of operation.
void init(void)
{
    PM5CTL0 &=~ LOCKLPM5; // Disable GPIO High impedance.
    __bis_SR_register(GIE); // Enable global interrupts
	setup_ADC(adc_channel_a, adc_channel_b, adc_channel_c);
}

// code within loop repeats continually.
void loop(void)
{
    read_adc(CHANNEL_A);
}
