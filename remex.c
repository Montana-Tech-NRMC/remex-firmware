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
#include "globals.h"

/**
 * remex.c
 */

// Must have definition. regmap is set in the init method
uint8_t regmap[REGMAP_SIZE] = { 0x00 };

enum remex_states remex;

// init is called once at the beginning of operation.
void init(void)
{

    P2DIR |= BIT0 | BIT1 | BIT2;
    P2OUT &=~ (BIT0 | BIT1 | BIT2);
    
    P6DIR |= BIT6; // dev board Test LED light
    P6OUT &=~ BIT6; // turn off the light

    clear_registers();
    init_adc((uint8_t*)&regmap);
    init_pwm_A();
    init_pwm_B();
    i2c_slave_init(SLAVE_ADDR, (uint8_t*)&regmap);

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
    __bis_SR_register(LPM0_bits);
    __no_operation();

    switch(i2c_command) {
    case TURN_ON_LIGHT:
        P6OUT |= BIT6;
        break;
    case TURN_OFF_LIGHT:
        P6OUT &=~ BIT6;
        break;
    case START_PWM:
        start_motors();
        break;
    }
}

void start_motors() {
    int pwm_speed_a = BYTES_TO_SHORT(regmap, DES_SPEED_A);
    set_pwm_A(pwm_speed_a);

    int pwm_speed_b = BYTES_TO_SHORT(regmap, DES_SPEED_B);
    set_pwm_B(pwm_speed_b);
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

