/*
 * hardwareIO.c
 *
 * Created on: Mar 11, 2021
 *     Author: Justin Bak
 */

#include <msp430.h>
#include "hardwareIO.h"

unsigned int *__count_a = 0, *__count_b = 0;
void (*switch_a_cb)(int) = 0L;
void (*switch_b_cb)(int) = 0L;

void init_switches(void (*switch_a)(int), void (*switch_b)(int))
{
    if (switch_a) {
        P2DIR &=~(BIT6);  // Set PIN 2.4 as input
        P2OUT |= (BIT6);  // Set PIN 2.4 pull up resistor (active low)
        P2REN |= (BIT6);  // Enable PIN 2.4 pulling resistor
        P2IES |= (BIT6);  // PIN 2.4 hi/low edge
        P2IE  |= (BIT6);  // PIN 2.4 enable interrupt

        switch_a_cb = switch_a;
        __count_b = 0;
    }

    if (switch_b) {
        P2DIR &=~(BIT5);  // Set PIN 2.4 as input
        P2OUT |= (BIT5);  // Set PIN 2.4 pull up resistor (active low)
        P2REN |= (BIT5);  // Enable PIN 2.4 pulling resistor
        P2IES |= (BIT5);  // PIN 2.4 hi/low edge
        P2IE  |= (BIT5);  // PIN 2.4 enable interrupt

        switch_b_cb = switch_b;
        __count_b = 0;
    }
}

void init_encoders(unsigned int* count_a, unsigned int* count_b)
{
    if (count_a) {
        P2DIR &=~(BIT2 | BIT3); // Set PIN 2.2 and 2.3 as input direction
        P2IES |= (BIT3); // 2.2 on Hi/low edge
        P2IE  |= (BIT3); // Enable Interrupt on 2.2

        __count_a = count_a; // store the pointer to the counter
    }

    if (count_b) {
        P2DIR &=~(BIT5 | BIT6); // Set PIN 2.5 and 2.6 as input direction
        P2SEL0 &=~ BIT6;
        P2SEL1 &=~ BIT6;
        P2IFG &=~ (BIT5);
        P2IE  |= (BIT5); // Enable Interrupt on 2.4
        P2IES |= (BIT5); // 2.5

        __count_b = count_b; // store the pointer to the counter
        switch_a_cb = 0L;
        switch_b_cb = 0L;
    }
}

#pragma vector = PORT2_VECTOR
__interrupt void Port_2(void)
{
    if (BIT3 & P2IFG){
        if (__count_a) {
            if(!(P2IN & BIT2)) {
                (*__count_a)++;
            } else if (P2IN & BIT2) {
                (*__count_a)--;
            }
        }
        P2IFG &=~ BIT3;
    }

    if (BIT5 & P2IFG) {
        if (__count_b) {
            if((P2IN & BIT6)) {
                (*__count_b)++;
            } else if (P2IN & BIT6) {
                (*__count_b)--;
            }
        }

        if (switch_a_cb) {
            switch_a_cb((P2IN & BIT5));
        }
        P2IFG &=~ BIT5;
    }

    if (BIT6 & P2IFG) {
        if (switch_b_cb) {
            switch_b_cb((P2IN & BIT6));
        }
        P2IFG &=~ BIT6;
    }
}
