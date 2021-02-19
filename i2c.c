/*
 * i2c.c
 *
 *  Created on: Oct 21, 2020
 *      Author: Justin Bak
 */

#include <msp430.h>
#include "i2c.h"

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
        unsigned char slave_addr)
{
    P1SEL0 |= SDA_PIN + SCL_PIN;             // Set i2c on pins 1.6 and 1.7 for USCIB0
    UCB0CTLW0 |= UCSWRST;                    // Enable software reset
    UCB0CTLW0 |= UCMODE_3 | UCSYNC;          // I2CSlave, synchronous mode
    UCB0I2COA0 = slave_addr | UCOAEN;        // Set slave address
    UCB0CTLW0 &= ~UCSWRST;                   // Clear software reset, return to normal operation
    UCB0IE |= (UCTXIE0 | UCSTPIE | UCRXIE0); // transmit, stop, rx interrupts enable

    // Link callback function pointers.
    receive_func = rx_cb;
    transmit_func = tx_cb;
    start_func = stt_cb;
    stop_func = stp_cb;
}

#pragma vector = USCI_B0_VECTOR
__interrupt void USCIB0_ISR(void)
{
    switch(__even_in_range(UCB0IV, USCI_I2C_UCBIT9IFG))
    {
    case USCI_I2C_UCSTTIFG: // Start condition
        start_func();
        break;
    case USCI_I2C_UCSTPIFG: // Stop Condition
        stop_func();
        break;
    case USCI_I2C_UCRXIFG0: // Receive Condition
        receive_func(UCB0RXBUF);
        break;
    case USCI_I2C_UCTXIFG0: // Transmit Condition
        transmit_func(&UCB0TXBUF);
        break;
    }
}
