/*
 * Author: Justin Bak
 * Code Owner: Justin Bak
 */
#include <msp430.h>
#include "remex.h"
#include "i2c.h"

/**
 * remex.c
 */

// Must have definition. regmap is set in the init method
unsigned char regmap[REGMAP_SIZE] = { 0x00 };

//////////// Global Vars //////////////////
unsigned char reg = UNKNOWN_REG;
enum i2c_states state = start;
///////////////////////////////////////////

// init is called once at the beginning of operation.
void init(void)
{
    __bis_SR_register(GIE); // Enable global interrupts
    clear_registers();
    i2c_slave_init(start_condition_cb, stop_condition_cb, receive_cb, transmit_cb, SLAVE_ADDR);

    PM5CTL0 &= ~LOCKLPM5;
}

// code within loop repeats continually.
void loop(void)
{
}

void clear_registers(void)
{
    unsigned int i;
    for (i = REGMAP_SIZE - 1; i > 0; i--) {
        regmap[i] = 0x00;
    }
}

// This function is called in an interrupt. Do not stall
void receive_cb(const unsigned char in)
{
    switch(state) {
    case start:
        reg = in;
        if (reg == command_reg) {
            state = cmd_byte;
        } else {
            state = reg_set;
        }
        break;
    case reg_set:
        regmap[reg] = in;
        reg++;
        break;
    case cmd_byte:
        process_cmd(in);
        break;
    }
    if (reg >= REGMAP_SIZE) {
        reg = 0;
    }
}

// This function is called in an interrupt. Do not stall
void transmit_cb(unsigned volatile int *out)
{
    switch(state) {
    case start:
        if (reg < REGMAP_SIZE) {
            *out = regmap[reg];
        } else {
            *out = 0xff;
        }

        reg++;
        if (reg >= REGMAP_SIZE) {
            reg = 0;
        }
        break;
    }
}

// This function is called in an interrupt. Do not stall.
void start_condition_cb(void)
{
    state = start;
}

// This function is called in an interrupt. Do not stall.
void stop_condition_cb(void)
{
    state = stop;
}

// This function is called in an interrupt. Do not stall.
void process_cmd(unsigned char cmd)
{
    //find the desired speed and clicks in the register map.
    /*
    int speedA = (int) (regmap[des_speed_a_L] + (regmap[des_speed_a_H] << 8));
    int speedB = (int) (regmap[des_speed_b_L] + (regmap[des_speed_b_H] << 8));
    int destA = (int) (regmap[des_pos_a_L] + (regmap[des_pos_a_H] << 8));
    int destB = (int) (regmap[des_pos_b_L] + (regmap[des_pos_b_H] << 8));
    */
    // start pid control to move motors to desired positions.
    //pid_control(speedA, speedB, destA, destB);

}


