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
#include "i2c_memory_map.h"

#define RING_BUF_SIZE 10

/**
 * remex.c
 */
/////// Global Vars /////

static const int deadZone = 5;
static const int gainxMult = 1;
static const int gainxDiv = 100;
static const int intxMult = 1;
static const int intxDiv = 10;

int integrator_ring_buf[RING_BUF_SIZE];
unsigned int integrator_ptr = 0;
/////////////////////////

void pushIntegratorVal(int val) {
    integrator_ring_buf[integrator_ptr] = (val / RING_BUF_SIZE);
    integrator_ptr++;
    if (integrator_ptr >= RING_BUF_SIZE) {
        integrator_ptr = 0;
    }
}

int integratorRollingAverage() {
    int sum = 0;
    unsigned int i;
    for (i = RING_BUF_SIZE; i != 0; i--) {
        sum += integrator_ring_buf[i-1];
    }
    return sum;
}



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

void switch_cb_b(int state) {
    remex = halt;
    P4OUT ^= BIT7;
    set_PWM_A(0);
}

void switch_cb_lower(int state) {
    remex = halt;
    set_PWM_A(0);
    regmap[POSITION_A_L] = 0;
    regmap[POSITION_A_H] = 0;
}

int positionA;

// init is called once at the beginning of operation.
void init(void)
{
    clear_registers();
    i2c_slave_init(onI2CStartBit, onI2CStopBit, onI2CByteReceived, onI2CByteTransmit, SLAVE_ADDR);
    init_i2c_memory_map(&regmap, onI2CCommand);
    init_PWM_A();
    init_encoders(&positionA, 0);
    init_switches(switch_cb_lower, switch_cb_b);
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
    int pval, ival, dval;
    int out;
    int diff;

    /*
    if (positionA > 1000 && positionA < 1500) {
        set_PWM_A(0);
    }
    */
    if (remex == goTo) {
        regmap[POSITION_A_L] = (char) (positionA & 0xFF);
        regmap[POSITION_A_H] = (char) (positionA >> 8);
        int currentPosition = (regmap[POSITION_A_H] << 8) + regmap[POSITION_A_L];
        int desiredPosition = (regmap[DES_POS_A_H] << 8) + regmap[DES_POS_A_L];

        diff = (desiredPosition - currentPosition);
        pushIntegratorVal(diff);
        ival = integratorRollingAverage();
        if (abs(diff) <= abs(deadZone)) {
            //TODO: Set a state machine loop that will acknowledge this
            remex = halt;
            set_PWM_A(0);
        } else {
            pval = diff;
            pval *= gainxMult;
            pval /= gainxDiv;
            ival *= intxMult;
            ival /= intxDiv;
            out = pval + ival;
            set_PWM_A(out);
        }
    }
    if (remex == zero) {
        set_PWM_A(-50);
    }
}

int direction() {
    int desiredPos = (int) (regmap[DES_POS_A_L] + (regmap[DES_POS_A_H] << 8));
    int currentPos = (int) (regmap[POSITION_A_L] + (regmap[POSITION_A_H] << 8));

    if (desiredPos > currentPos) {
        return 1;
    } else if (desiredPos < currentPos) {
        return -1;
    }
    return 0;
}

// This function is called in an interrupt. Do not stall.
void onI2CCommand(unsigned const char cmd)
{
    if (cmd == 0xa5) {
        int dir = direction();
        if (P2IN & BIT4 && dir > 0) {
            remex = halt;
        } else if ( P2IN & BIT5 && dir < 0) {
            remex = halt;
        } else {
            remex = goTo;
        }
    }

    if (cmd == 0x2E) {
        if (P2IN & BIT4) {
            remex = halt;
        } else {
            remex = zero;
        }
    }
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


