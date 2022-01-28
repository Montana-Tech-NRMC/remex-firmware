/*
 * Author: Justin Bak
 * Code Owner: Justin Bak
 */
#include <msp430.h>
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
unsigned char regmap[REGMAP_SIZE] = { 0x00 };

int encoder = 0;
char enc_str[4];

//////////// Global Vars //////////////////
unsigned char reg = UNKNOWN_REG;
enum i2c_states state = start;
///////////////////////////////////////////

void adc_channel_a(int current) {
    volatile int foo = current;
    return;
}

// init is called once at the beginning of operation.
void init(void)
{
    clear_registers();
    i2c_slave_init(start_condition_cb, stop_condition_cb, onI2CByteReceived, transmit_cb, SLAVE_ADDR);
    init_PWM_A();
    init_PWM_B();
    //init_ADC();
    //init_switches();
    init_UART();
    __bis_SR_register(GIE); // Enable global interrupts
	init_encoders(0, &encoder);

	// Turn on proof of life LED.
    P4DIR |= BIT7;
    P4OUT |= BIT7;

    // Disable GPIO High impedance.
    PM5CTL0 &=~ LOCKLPM5;
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
void onI2CByteReceived(const unsigned char in)
{
    switch(state) {
    case stop:
        reg = in;
        if (reg == command_reg) {
            state = cmd_byte;
        } else {
            state = reg_set;
        }
        break;
    case reg_set:
        if (reg < READONLY) {
            regmap[reg] = in;
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
void transmit_cb(unsigned volatile int *out)
{
    if (reg < REGMAP_SIZE) {
        *out = regmap[reg];
    } else {
        *out = 0xff;
    }

    reg++;
    if (reg >= REGMAP_SIZE) {
        reg = 0;
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
    if (cmd == 0xa5) {
        //find the desired speed and clicks in the register map.
        int speedA = (int) (regmap[des_speed_a_L] + (regmap[des_speed_a_H] << 8));
        /*
        int speedB = (int) (regmap[des_speed_b_L] + (regmap[des_speed_b_H] << 8));
        int destA = (int) (regmap[des_pos_a_L] + (regmap[des_pos_a_H] << 8));
        int destB = (int) (regmap[des_pos_b_L] + (regmap[des_pos_b_H] << 8));
        */

        set_PWM_A(speedA);
        // start pid control to move motors to desired positions.
        //pid_control(speedA, speedB, destA, destB);
    }
}

void int2str(int inval, char * str_out){
    char nval;
    int c;
    for (c=0;c<4;c++){
        nval=inval&0x0F;
        inval>>=4;
        if (nval<=0x09)
            str_out[3-c]=0x30+nval;
        else
            str_out[3-c]='A'+(nval-0x0A);
    }
}
