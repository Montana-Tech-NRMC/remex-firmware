/*
 * over_current.h
 *
 *  Created on: Aug 22, 2020
 *      Author: Justin Bak
 *      Code Owner: Justin Bak
 */

#ifndef ADC_H_
#define ADC_H_

#define CHANNEL_A ADCINCH_0
#define CHANNEL_B ADCINCH_1
#define CHANNEL_C ADCINCH_11

#include <msp430.h>

/// adc call back functions that will be executed when when an adc is read
void (*adc_callbackA)(int);
void (*adc_callbackB)(int);
void (*adc_callbackC)(int);

/// read_adc will read the input channel A and call adc_callbackA with the result of the call
void read_adc(int channel);

/// setup_ADC will map the call back functions and initilize GPIO for reading ADC on 3 channels.
void setup_ADC(void (*cba)(int), void(*cbb)(int), void(*cbc)(int));

#endif /* ADC_H_ */
