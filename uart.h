/*
 * uart.c
 *
 *  Created on: Apr 2, 2021
 *      Author: juicetin
 */

#ifndef UART_H_
#define UART_H_

void init_UART();

void putc(char in);

void puts(char* in);

void putsn(char* in, int len);

#endif /* UART_H_ */
