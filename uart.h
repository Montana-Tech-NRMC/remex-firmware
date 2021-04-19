/*
 * uart.c
 *
 *  Created on: Apr 2, 2021
 *      Author: Justin Bak
 */

#ifndef UART_H_
#define UART_H_

/**
 * Initialize UART with baud rate of 9600 on pins 4.2 and 4.3
 */
void init_UART();

/**
 * putc will output a single character in to the UART bus
 *
 * @param[in] in is the character to send to the UART bus.
 */
void putc(char in);

/**
 * puts will output a string of characters to the UART bus
 *
 * @param[in] in is a null terminated string of characters to send to the UART bus.
 */
void puts(char* in);

/**
 * putsn will output a string of characters to the UART bus
 *
 * @param[in] in is a string of characters to send to the UART bus.
 * @param[len] is the length of the string that will be sent over the UART bus.
 */
void putsn(char* in, int len);

#endif /* UART_H_ */
