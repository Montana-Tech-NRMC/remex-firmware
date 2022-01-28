/**
 * over_current.h
 *
 *  Created on: Aug 22, 2020
 *      Author: Justin Bak
 *      Code Owner: Justin Bak
 */

#ifndef ADC_H_
#define ADC_H_

/**
 * Constants to be used in read_adc to select channels.
 */
#define CHANNEL_A ADCINCH_0
#define CHANNEL_B ADCINCH_1
#define CHANNEL_C ADCINCH_11

#include <msp430.h>

/// adc call back functions that will be executed when when an adc is read
void (*adc_callbackA)(int);
void (*adc_callbackB)(int);
void (*adc_callbackC)(int);

/**
 *  read_adc will read the input channel A and call adc_callbackA with the result of the call
 *
 *  @param[in] channel is the channel that should be read. Constants for channels to read are defined as constants.
 */
void read_adc(int channel);

/**
 *  setup_ADC will map the call back functions and initilize GPIO for reading ADC on 3 channels.
 *
 *  @param[in] cba function pointer which will be called by the ADC ISR when a sample on channel A is ready
 *  @param[in] cbb function pointer which will be called by the ADC ISR when a sample on channel B is ready
 *  @param[in] cbc function pointer which will be called by the ADC ISR when a sample on channel C is ready
 */
void init_ADC(void (*cba)(int), void(*cbb)(int), void(*cbc)(int));

#endif /* ADC_H_ */
