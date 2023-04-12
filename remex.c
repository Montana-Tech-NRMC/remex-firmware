/*
 * Author: Justin Bak
 * Code Owner: Justin Bak
 */
#include <msp430.h>
#include <math.h>
#include "remex.h"
#include "i2c.h"
#include "pwm.h"
#include "adc.h"
#include "uart.h"
#include "hardwareIO.h"

/**
 * remex.c
 */

// Must have definition. regmap is set in the init method
uint8_t regmap[REGMAP_SIZE] = { 0x00 };

enum remex_states remex;

void adc_threshold_broken(int channel, int value) {
    switch(channel) {
        case 0:
            P2OUT |= BIT0;
            P2OUT &=~ (BIT1 | BIT2);
            break;
        case 1:
            P2OUT |= BIT1;
            P2OUT &=~ (BIT0 | BIT2);
            break;
        case 2:
            P2OUT |= BIT0 | BIT1;
            P2OUT &=~ BIT2;
            break;
        case 3:
            P2OUT |= BIT2;
            P2OUT &=~ (BIT1 | BIT0);
            break;
    }
}

// init is called once at the beginning of operation.
void init(void)
{

    P2DIR |= BIT0 | BIT1 | BIT2;
    P2OUT &=~ (BIT0 | BIT1 | BIT2);
    
    P6DIR |= BIT6; // dev board Test LED light
    P6OUT &=~ BIT6; // turn off the light

    clear_registers();
    init_adc((uint8_t*)&regmap);
    i2c_slave_init(SLAVE_ADDR, (uint8_t*)&regmap, process_i2c_command);

    __bis_SR_register(GIE); // Enable global interrupts

    // Turn on proof of life LED.
    P3DIR |= BIT5;
    P3OUT |= BIT5;
    // Disable GPIO High impedance.
    PM5CTL0 &=~ LOCKLPM5;

}

// code within loop repeats continually.
void loop(void)
{
    WDTCTL = WDT_ADLY_16;
}

// This function is called in an interrupt. Do not stall.
void process_i2c_command(const uint8_t command)
{
    // goto/begin command
    switch(command) {
    case 0xa5:
        P6OUT |= BIT6;
        break;
    case 0x2E:
        P6OUT &=~ BIT6;
        break;
    }
}

void clear_registers(void)
{
    unsigned int i;
    for (i = REGMAP_SIZE - 1; i > 0; i--) {
        regmap[i] = 0x00;
    }
}

