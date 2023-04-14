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

#define FRAM_REGMAP_START 0x8500

unsigned long *FRAM_write_ptr;

// Must have definition. regmap is set in the init method
uint8_t regmap[REGMAP_SIZE] = { 0x00 };

enum remex_states remex;

// init is called once at the beginning of operation.
void init(void) {

    P2DIR |= BIT0 | BIT1 | BIT2;
    P2OUT &=~ (BIT0 | BIT1 | BIT2);
    
    P6DIR |= BIT6; // dev board Test LED light
    P6OUT &=~ BIT6; // turn off the light

    initialize_register_map();
    init_adc((uint8_t*)&regmap);
    init_pwm_A();
    init_pwm_B();
    init_encoders((uint8_t*)&regmap);
    i2c_slave_init(SLAVE_ADDR, (uint8_t*)&regmap);

    __bis_SR_register(GIE); // Enable global interrupts

    // Turn on proof of life LED.
    P3DIR |= BIT5;
    P3OUT |= BIT5;
    // Disable GPIO High impedance.
    PM5CTL0 &=~ LOCKLPM5;

}

// code within loop repeats continually.
void loop(void) {
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
    case SAVE_STATE:
        FRAM_write();
        break;
    }
}

void start_motors(void) {
    int pwm_speed_a = BYTES_TO_SHORT(regmap, DES_SPEED_A);
    set_pwm_A(pwm_speed_a);

    int pwm_speed_b = BYTES_TO_SHORT(regmap, DES_SPEED_B);
    set_pwm_B(pwm_speed_b);
}

void FRAM_write(void) {
    FRAM_write_ptr = (unsigned long *) FRAM_REGMAP_START;
    unsigned int i = 0;
    SYSCFG0 = FRWPPW | PFWP;

    for(i = 0; i < REGMAP_SIZE; i++) {
        *FRAM_write_ptr++ = regmap[i];
    }

    SYSCFG0 = FRWPPW | DFWP | PFWP;
}

void initialize_register_map(void) {
    FRAM_write_ptr = (unsigned long *) FRAM_REGMAP_START;
    unsigned int i = 0;
    SYSCFG0 = FRWPPW | PFWP;

    for(i = 0; i < REGMAP_SIZE; i++) {
        regmap[i] =  *FRAM_write_ptr++;
    }

    SYSCFG0 = FRWPPW | DFWP | PFWP;
}

