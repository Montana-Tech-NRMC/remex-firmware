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
#include <stdint.h>

/**
 *  setup_ADC will start the adc module reading from 4 consecutive channels. An interrupt will be triggered if a channel's voltage exceeds high_threshold 
 *
 *  @param[in] high_threshold is the upper voltage limit where an interrupt will be triggered.
 */
void init_adc(uint8_t* memory_map);

#endif /* ADC_H_ */
