/*
 * i2c.c
 *
 *  Created on: Oct 21, 2020
 *      Author: Justin Bak
 */

#include <msp430.h>
#include "i2c.h"
#include "globals.h"

uint8_t* regmap_local;
uint8_t memory_map_index = 0;
enum i2c_states state;

volatile uint8_t exit_lpm = 0;

void i2c_slave_init(
        unsigned char slave_addr,
        uint8_t* memory_start
    )
{
    regmap_local = memory_start;

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
        i2c_command = in;
        exit_lpm = 1;
        break;
    default:
        __no_operation();
    }
}

unsigned int get_int_from_memory(const uint8_t index) {
    if (index + 1 < REGMAP_SIZE) {
        return (unsigned int) (regmap_local[(unsigned int) index+1] << 8) | regmap_local[index];
    }
    return 0;
}

void load_int_to_memory(const uint8_t index, const int value) {
    if (index + 1 > REGMAP_SIZE) { return; }

    regmap_local[(unsigned int) index+1] = (uint8_t)(value >> 8);
    regmap_local[index] = (uint8_t)(value & 0xff);
}

#pragma vector = USCI_B1_VECTOR
__interrupt void USCIB1_ISR(void)
{
    char readval;
    switch(__even_in_range(UCB1IV, USCI_I2C_UCBIT9IFG))
    {
    case USCI_I2C_UCSTTIFG: // Start condition
        state = start;
        break;
    case USCI_I2C_UCSTPIFG: // Stop Condition
        UCB1IFG &=~ UCSTPIFG;
        state = stop;
        break;
    case USCI_I2C_UCRXIFG0: // Receive Condition
         // must read rxbuf to clear interrupt
        readval = UCB1RXBUF;
        receive_byte(readval);
        if(exit_lpm) {
            __bic_SR_register_on_exit(LPM0_bits);
            exit_lpm = 0;
        }
        break;
    case USCI_I2C_UCTXIFG0: // Transmit Condition
        transmit_byte(&UCB1TXBUF);
        break;
    }
}
