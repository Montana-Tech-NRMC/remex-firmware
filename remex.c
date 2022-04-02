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

// init is called once at the beginning of operation.
void init(void)
{
    clear_registers();
    i2c_slave_init(onI2CStartBit, onI2CStopBit, onI2CByteReceived, onI2CByteTransmit, SLAVE_ADDR);
    init_i2c_memory_map(&regmap, onI2CCommand);
    init_PWM_A();
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
    switch(remex) {
    case run:
        break;
    case halt:
        break;
    case error:
        break;
    case finished:
        break;
    }
}

// This function is called in an interrupt. Do not stall.
void onI2CCommand(unsigned const char cmd)
{
    if (cmd == 0xa5) {
        remex = start;
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
}

void clear_registers(void)
{
    unsigned int i;
    for (i = REGMAP_SIZE - 1; i > 0; i--) {
        regmap[i] = 0x00;
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

// Timer B0 interrupt service routine
#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer_B_1 (void)
{
    read_adc(CHANNEL_A);
}


