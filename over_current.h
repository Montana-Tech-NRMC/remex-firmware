/*
 * over_current.h
 *
 *  Created on: Aug 22, 2020
 *      Author: Justin Bak
 *      Code Owner: Justin Bak
 */

#ifndef OVER_CURRENT_H_
#define OVER_CURRENT_H_

#include <msp430.h>


int current_limit;

void (*oc_callback)(void);

void set_overcurrent_cb(void (*oc_callback)(void));

void set_threshHold(int limit);

void read_current(void);

void setup_ADC();

#endif /* OVER_CURRENT_H_ */
