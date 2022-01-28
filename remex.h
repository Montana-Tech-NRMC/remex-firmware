/*
 * remex.h
 *
 *  Created on: Oct 28, 2020
 *      Author: Justin Bak
 */

#ifndef REMEX_H_
#define REMEX_H_

#define SLAVE_ADDR  0x44
#define UNKNOWN_REG 0xFF
#define UNKNOWN_PAR 0xFF

/************* Register addresses **********************/

/// Read / Write Registers
#define command_reg        0xCC
#define mode_reg           0x00
#define max_current_L      0x01
#define max_current_H      0x02
#define current_duration_L 0x03
#define current_duration_H 0x04
#define des_speed_a_L      0x05
#define des_speed_a_H      0x06
#define des_speed_b_L      0x07
#define des_speed_b_H      0x08
#define des_pos_a_L        0x09
#define des_pos_a_H        0x0A
#define des_pos_b_L        0x0B
#define des_pos_b_H        0x0C

#define READONLY           0x20
/// Read only Registers
#define position_a_L       0x20
#define position_a_H       0x21
#define position_b_L       0x22
#define position_b_H       0x23
#define ADC_A_L            0x24
#define ADC_A_H            0x25
#define ADC_B_L            0x26
#define ADC_B_H            0x27
#define ADC_C_L            0x28
#define ADC_C_H            0x29
#define switch_states      0x2A
#define remex_state        0x2F

#define REGMAP_SIZE 0x2F
/****************************************************/

/// state machine states
enum i2c_states {
    start,
    reg_set,
    stop,
    cmd_byte,
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
 * clear_registers is a helper function that sets all the registers in the register map to zero.
 */
void clear_registers(void);

/**
 * process_cmd is a function that is called when a command byte is received from the master.
 *
 * @param[in] cmd is the byte received from the master.
 */
void process_cmd(unsigned char cmd);

/**
 * receive_cb is a call back function that is called when the master writes a byte.
 *
 * @param[in] in is the byte received from the master.
 */
void onI2CByteReceived(const unsigned char in);

/**
 * transmit_cb is a callback function that is called when the master wants to read a byte from the slave.
 *
 * @param[out] the pointer to the transmit buffer to send a byte.
 */
void transmit_cb(unsigned volatile int *out);

/**
 * start_condition_cb is a callback function for when a i2c start condition is detected from the master.
 */
void start_condition_cb(void);

/**
 * stop_condition_cb is a callback function for when a i2c stop condition is detected from the master.
 */
void stop_condition_cb(void);

/**
 * init is called once, and initializes the registers and libraries for the board.
 */
void init(void);

/**
 * loop is called repeatedly as long as the board is powered.
 */
void loop(void);

/**
 * int2str will convert an integer inval and store into string str_out
 *
 * @param[in] inval is the integer value to be converted to a string
 * @param[out] str_out is the output string where the string value of the integer will be written.
 */
void int2str(int inval, char * str_out);

#endif /* REMEX_H_ */
