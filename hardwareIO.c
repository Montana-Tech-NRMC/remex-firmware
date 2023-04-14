/*
 * hardwareIO.c
 *
 * Created on: Mar 11, 2021
 *     Author: Justin Bak
 */

#include <msp430.h>
#include "hardwareIO.h"
#include "i2c.h"

unsigned int count_left = 0, count_right = 0;
uint8_t* local_memory_map;

void init_encoders(uint8_t* memory_map)
{
    local_memory_map = memory_map;

    P1DIR &=~(QEI_1_A | QEI_1_B | QEI_2_A | QEI_2_B);
    P1IES |= (QEI_1_A | QEI_2_A); // Hi/low edge
    P1IE  |= (QEI_1_A | QEI_2_A); // Enable Interrupt
    P1IFG &=~(QEI_1_A | QEI_2_A);
}

#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void)
{
    if (QEI_1_A & P1IFG){
        if(!(P1IN & QEI_1_B)) {
            count_left--;
        } else if (P1IN & QEI_1_B) {
            count_left++;
        }
        P1IFG &=~ QEI_1_A;

        SHORT_TO_BYTES(local_memory_map, REGMAP_SIZE, POSITION_A, count_left);
    }

    if (QEI_2_A & P1IFG) {
        if((P1IN & QEI_2_B)) {
            count_right++;
        } else if (P1IN & QEI_2_B) {
            count_right--;
        }
        P1IFG &=~ QEI_2_A;

        SHORT_TO_BYTES(local_memory_map, REGMAP_SIZE, POSITION_B, count_right);
    }
}
