/*
 * remex.h
 *
 *  Created on: Oct 28, 2020
 *      Author: Justin Bak
 */

#ifndef REMEX_H_
#define REMEX_H_

enum remex_states {
    run,
    halt,
    error,
    finished,
};

/**
 * adc_channel_a is the call back function when the adc has finished it's conversion and has a
 * value.
 */
void adc_channel_a(int current);

/**
 * adc_channel_b is the call back function when the adc has finished it's conversion and has a
 * value.
 */
void adc_channel_b(int current);

/**
 * adc_channel_c is the call back function when the adc has finished it's conversion and has a
 * value.
 */
void adc_channel_c(int current);

/**
 * process_cmd is a function that is called when a command byte is received from the master.
 *
 * @param[in] cmd is the byte received from the master.
 */
void onI2CCommand(unsigned const char cmd);

/**
 * init is called once, and initializes the registers and libraries for the board.
 */
void init(void);

/**
 * loop is called repeatedly as long as the board is powered.
 */
void loop(void);

/**
 * clear_registers is a helper function that sets all the registers in the register map to zero.
 */
void clear_registers(void);

/**
 * int2str will convert an integer inval and store into string str_out
 *
 * @param[in] inval is the integer value to be converted to a string
 * @param[out] str_out is the output string where the string value of the integer will be written.
 */
void int2str(int inval, char * str_out);

#endif /* REMEX_H_ */
