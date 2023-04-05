/*
 * i2c.c
 *
 *  Created on: Oct 21, 2020
 *      Author: Justin Bak
 */

#include <msp430.h>
#include "i2c.h"

uint8_t* regmap_local;
uint8_t memory_map_index = 0;
enum i2c_states state;

void (*command_callback)(const uint8_t) = 0L;

void i2c_slave_init(
        unsigned char slave_addr,
        uint8_t* memory_start,
        void (*command_handler)(const uint8_t)
     )
{
    regmap_local = memory_start;
    command_callback = command_handler;

    P4SEL0 |= SDA_PIN + SCL_PIN;             // Set i2c on pins 4.6 and 4.7 for USCIB0
    UCB1CTLW0 |= UCSWRST;                    // Enable software reset
    UCB1CTLW0 |= UCMODE_3 | UCSYNC;          // I2CSlave, synchronous mode
    UCB1I2COA0 = slave_addr | UCOAEN;                  // Set slave address
    UCB1CTLW0 &= ~UCSWRST;                             // Clear software reset, return to normal operation
    UCB1IE |= (UCTXIE0 | UCSTPIE | UCRXIE0 | UCSTTIE); // transmit, stop, start, rx interrupts enable
}

void transmit_byte(unsigned volatile int *out) {
    if (memory_map_index < REGMAP_SIZE) {
        *out = regmap_local[memory_map_index];
    } else {
        *out = UNKNOWN_REG;
    }

    memory_map_index++;
    if (memory_map_index >= REGMAP_SIZE) {
        memory_map_index = 0;
    }
}

void receive_byte(const unsigned char in) {
    switch(state) {
    case start:
        memory_map_index = in;
        if (memory_map_index == COMMAND_REG) {
            state = command_byte;
        } else {
            state = register_set;
        }
        break;
    case register_set:
        if (memory_map_index < READONLY) {
            regmap_local[memory_map_index] = in;
            memory_map_index++;
        }
        break;
    case command_byte:
        if (command_callback != 0L) {
            command_callback(in);
        }
        break;
    default:
        __no_operation();
    }
}

void start_condition_found() {
    state = start;
}

void stop_condition_found() {
    state = stop;
}

int combineInt(unsigned int registerStart) {
    return (int) (regmap_local[registerStart+1] << 8) + regmap_local[registerStart];
}

void splitInt(unsigned int registerStart, int value) {
    regmap_local[registerStart] = (char) (value & 0xFF);
    regmap_local[registerStart + 1] = (char) (value >> 8);
}

#pragma vector = USCI_B1_VECTOR
__interrupt void USCIB1_ISR(void)
{
    char readval;
    switch(__even_in_range(UCB1IV, USCI_I2C_UCBIT9IFG))
    {
    case USCI_I2C_UCSTTIFG: // Start condition
        start_condition_found();
        break;
    case USCI_I2C_UCSTPIFG: // Stop Condition
        UCB1IFG &=~ UCSTPIFG;
        stop_condition_found();
        break;
    case USCI_I2C_UCRXIFG0: // Receive Condition
         // must read rxbuf to clear interrupt
        readval = UCB1RXBUF;
        receive_byte(readval);
        break;
    case USCI_I2C_UCTXIFG0: // Transmit Condition
        transmit_byte(&UCB1TXBUF);
        break;
    }
}
