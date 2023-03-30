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
#include "i2c_memory_map.h"

/**
 * remex.c
 */

// Must have definition. regmap is set in the init method
unsigned char regmap[REGMAP_SIZE] = { 0x00 };

enum remex_states remex;

void on_threshold_broken(int channel, int value) {
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
    
    clear_registers();
    init_adc(3000, on_threshold_broken);
    //i2c_slave_init(onI2CStartBit, onI2CStopBit, onI2CByteReceived, onI2CByteTransmit, SLAVE_ADDR);
    //init_i2c_memory_map((unsigned char *)&regmap, onI2CCommand);

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
    switch(remex) {
    case goTo:
        break;
    case zeroing:
        break;
    case zeroed:
        break;
    case halt:
        break;
    }
}

// This function is called in an interrupt. Do not stall.
void onI2CCommand(unsigned const char cmd)
{
    // goto/begin command
    if (cmd == 0xa5) {
        int dir = direction();
        if (P2IN & BIT4 && dir < 0) {
            remex = halt;
        } else if ( P2IN & BIT5 && dir > 0) {
            remex = halt;
        } else {
            remex = goTo;
        }
    }

    // zero command
    if (cmd == 0x2E) {
        if (P2IN & BIT4) {
            remex = halt;
        } else {
            remex = zeroing;
        }
    }
}

void clear_registers(void)
{
    unsigned int i;
    for (i = REGMAP_SIZE - 1; i > 0; i--) {
        regmap[i] = 0x00;
    }
}

