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
#include "i2c_memory_map.h"

/**
 * remex.c
 */
/////// Global Vars /////
int pval, ival, dval, deadZone;
/////////////////////////

// Must have definition. regmap is set in the init method
unsigned char regmap[REGMAP_SIZE] = { 0x00 };

enum remex_states remex;
int speedA = 0;

void adc_channel_a(int current) {
    regmap[ADC_A_L] = current & 0xFF; // low byte
    regmap[ADC_A_H] = current >> 8;   // high byte
    return;
}

void init_timer() {
    TB1CCTL0 |= CCIE;                // TBCCR0 Interrupt Enabled
    TB1CCR0 = 50000;
    TB1CTL = TBSSEL__SMCLK | MC__UP | ID_3; // Use the SMCLCK in Up Mode
}

void switch_cb(int state) {
    set_PWM_A(0);
}

unsigned int positionA;

// init is called once at the beginning of operation.
void init(void)
{
    clear_registers();
    i2c_slave_init(onI2CStartBit, onI2CStopBit, onI2CByteReceived, onI2CByteTransmit, SLAVE_ADDR);
    init_i2c_memory_map(&regmap, onI2CCommand);
    init_PWM_A();
    init_encoders(&positionA, 0);
    init_switches(switch_cb, 0);
//  init_encoders(((unsigned int *)&regmap[POSITION_A_L]), 0);
    __bis_SR_register(GIE); // Enable global interrupts

	// Turn on proof of life LED.
    P4DIR |= BIT7;
    P4OUT |= BIT7;
    // Disable GPIO High impedance.
    PM5CTL0 &=~ LOCKLPM5;
}

// code within loop repeats continually.
void loop(void)
{
<<<<<<< HEAD
    int diff;
    int gainx;
    if (positionA > 1000 && positionA < 1500) {
        set_PWM_A(3000);
    }
    regmap[POSITION_A_L] = (char) (positionA & 0xFF);
    regmap[POSITION_A_H] = (char) (positionA >> 8);
    int currentPosition = (regmap[POSITION_A_H] << 8) + regmap[POSITION_A_L];
    int desiredPosition = (regmap[DES_POS_A_H] << 8) + regmap[DES_POS_A_L];

    diff = (currentPosition - desiredPosition);
    if (diff >= deadZone || diff <= deadZone) {
        remex = halt;
    } else {
        pval = diff;
        gainx = pval;
        set_PWM_A(gainx);
    }
=======
    //regmap[POSITION_A_H] = positionA >> 8;
    //regmap[POSITION_A_L] = positionA & 0xff;
>>>>>>> 2744f101b7c5836f4677196c7d52f10cceaed50c
}

// This function is called in an interrupt. Do not stall.
void onI2CCommand(unsigned const char cmd)
{
    if (cmd == 0xa5) {
        remex = goTo;
        //find the desired speed and clicks in the register map.
        int speedA = (int) (regmap[DES_SPEED_A_L] + (regmap[DES_SPEED_A_H] << 8));

        /*
        int speedB = (int) (regmap[des_speed_b_L] + (regmap[des_speed_b_H] << 8));
        int destA = (int) (regmap[des_pos_a_L] + (regmap[des_pos_a_H] << 8));
        int destB = (int) (regmap[des_pos_b_L] + (regmap[des_pos_b_H] << 8));
        */
        set_PWM_A(speedA);
        // start pid control to move motors to desired positions.
        //pid_control(speedA, speedB, destA, destB);
    }

    if (cmd == 0xb0) {}
}

void clear_registers(void)
{
    unsigned int i;
    for (i = REGMAP_SIZE - 1; i > 0; i--) {
        regmap[i] = 0x00;
    }
}

// Timer B0 interrupt service routine
#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer_B_1 (void)
{
    read_adc(CHANNEL_A);
}


