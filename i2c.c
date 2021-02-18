/*
 * i2c.c
 *
 *  Created on: Oct 21, 2020
 *      Author: Justin Bak
 */

#include <msp430.h>
#include "i2c.h"

char * memmap_local;
int MAP_RW_END;
int MAP_RW_START;

void (*receive_func)(const unsigned char in);
void (*transmit_func)(unsigned volatile char *out);
void (*start_func)(void);
void (*stop_func)(void);

// Must have definition. regmap is set in the init method
extern unsigned char regmap[REGMAP_SIZE] = { 0x00 };

void i2c_slave_init(
        void (*stt_cb)(void),
        void (*stp_cb)(void),
        void (*rx_cb)(unsigned char in),
        void (*tx_cb)(unsigned volatile char *out),
        unsigned char slave_addr,
        char * mem_ptr)
{

    P1SEL0 |= SDA_PIN + SCL_PIN;             // Set i2c on pins 1.6 and 1.7 for USCIB0
    UCB0CTLW0 |= UCSWRST;                    // Enable software reset
    UCB0CTLW0 |= UCMODE_3 | UCSYNC;          // I2CSlave, synchronous mode
    UCB0I2COA0 = slave_addr | UCOAEN;        // Set slave address
    UCB0CTLW0 &= ~UCSWRST;                   // Clear software reset, return to normal operation
    UCB0IE |= (UCTXIE0 | UCSTPIE | UCRXIE0); // transmit, stop, rx interrupts enable
    IE2 |= UCB0TXIE + UCB0RXIE;              // Enable TX, RX interrupt
    UCB0I2CIE |= UCSTTIE | UCSTPIE;          // Enable STT, STP interrupt

    // Link callback function pointers.
    receive_func = rx_cb;
    transmit_func = tx_cb;
    start_func = stt_cb;
    stop_func = stp_cb;

    memmap_local = mem_ptr;
    MAP_RW_END = mem_rw_end;
    MAP_RW_START; = mem_rw_start;
}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void)
{
    if (UCB0STAT & UCSTTIFG) {
        UCB0STAT &=~ UCSTTIFG; // clear start condition flag
        start_func();
    } else if (UCB0STAT & UCSTPIFG) {
        UCB0STAT &=~ UCSTPIFG;
        stop_func();
    }
 }

#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
    if (IFG2 & UCB0TXIFG) {
        transmit_func(&UCB0TXBUF);
    } else {
        receive_func(UCB0RXBUF);
    }
}


