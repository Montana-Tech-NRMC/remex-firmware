/*
 * remex.h
 *
 *  Created on: Oct 28, 2020
 *      Author: Justin Bak
 */

#ifndef REMEX_H_
#define REMEX_H_

#define SLAVE_ADDR  0x48
#define UNKNOWN_REG 0xFF
#define UNKNOWN_PAR 0xFF

// state machine states
enum i2c_states {
   start,
   reg_set,
   stop,
   cmd_byte,
};

/**
 * stop_pwm is a callback function that is called when an motor over current condition is detected.
 */
void stop_pwm(void);

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
void receive_cb(const unsigned char in);

/**
 * transmit_cb is a callback function that is called when the master wants to read a byte from the slave.
 *
 * @param[out] the pointer to the transmit buffer to send a byte.
 */
void transmit_cb(unsigned volatile char *out);

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

#endif /* REMEX_H_ */
