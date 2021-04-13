/*
 * uart.c
 *
 *  Created on: Apr 2, 2021
 *      Author: Justin Bak
 */

#include "uart.h"
#include <msp430.h>

void init_UART() {

    P4SEL0 |= BIT2 | BIT3; // set 2-UART pins as second function
    P4SEL1 &=~ (BIT2 | BIT3);

    UCA1CTLW0 |= UCSWRST;
    UCA1CTLW0 |= UCSSEL__SMCLK;

    // Baud rate calculation
    // 8,000,000/9,600/16 = 52.083
    // Fractional portion = 0.083
    // UG table 22-4: UCBRSx = 0x49
    // UCBRFx = int( (52.083 - 52) * 16) = 1
    UCA1BR0 = 52;       // 8,000,000/9,600/16
    UCA1BR1 = 0x00;
    UCA1MCTLW = 0x4900 | UCOS16 | UCBRF_1;

    UCA1CTLW0 &= ~UCSWRST; // Init eUSCI
    UCA1IE |= UCRXIE;      // Enable USCI_A1 RX interrupt
}

void putc(char in) {
    while(!(UCA1IFG & UCTXIFG));
    UCA1TXBUF = in;
}

void puts(char* in) {
    char* s = in;
    while(*s != 0) {
        putc(*s++);
    }
}

void putsn(char* in, int n) {
    char* s = in;
    int i = 0;
    for (i = n; i > 0; i--) {
        putc(*s++);
    }
}


#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void) {
    switch(__even_in_range(UCA1IV, USCI_UART_UCTXCPTIFG))
    {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG: break;
    case USCI_UART_UCTXIFG: break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
    default: break;
    }
}
