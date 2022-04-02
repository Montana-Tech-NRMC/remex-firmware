/*
 * i2c_memory_map.c
 *
 * Created on: Jan 28, 2022
 *     Author: Justin Bak
 */

#include <msp430.h>
#include "i2c_memory_map.h"

char* regmap_local;

//////////// Global Vars //////////////////
unsigned char reg;
enum i2c_states state;
///////////////////////////////////////////

void (*process_cmd) (unsigned const char);

void init_i2c_memory_map(unsigned char* map_start, void (*cmd_callback)(unsigned const char)) {
    regmap_local = map_start;
    state = start;
    reg = UNKNOWN_REG;
    process_cmd = cmd_callback;
}

// This function is called in an interrupt. Do not stall
void onI2CByteReceived(const unsigned char in)
{
    switch(state) {
    case stop:
        reg = in;
        if (reg == COMMAND_REG) {
            state = cmd_byte;
        } else {
            state = reg_set;
        }
        break;
    case reg_set:
        if (reg < READONLY) {
            regmap_local[reg] = in;
            reg++;
        }
        break;
    case cmd_byte:
        process_cmd(in);
        break;
    default:
        __no_operation();
    }
}

// This function is called in an interrupt. Do not stall
void onI2CByteTransmit(unsigned volatile int *out)
{
    if (reg < REGMAP_SIZE) {
        *out = regmap_local[reg];
    } else {
        *out = 0xff;
    }

    reg++;
    if (reg >= REGMAP_SIZE) {
        reg = 0;
    }
}

// This function is called in an interrupt. Do not stall.
void onI2CStartBit(void)
{
    state = start;
}

// This function is called in an interrupt. Do not stall.
void onI2CStopBit(void)
{
    state = stop;
}
