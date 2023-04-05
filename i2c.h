/*
 * i2c.h
 *
 *  Created on: Oct 21, 2020
 *      Author: Justin Bak
 */

#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>

#define SLAVE_ADDR  0x44

#define SDA_PIN BIT6 // Pin 4.6
#define SCL_PIN BIT7 // Pin 4.7

/************* Memory Map addresses **********************/

/// Read / Write Registers
#define COMMAND_REG        0xCC
#define MODE_REG           0x00
#define MAX_CURRENT_L      0x01
#define MAX_CURRENT_H      0x02
#define CURRENT_DURATION_L 0x03
#define CURRENT_DURATION_H 0x04
#define DES_SPEED_A_L      0x05
#define DES_SPEED_A_H      0x06
#define DES_SPEED_B_L      0x07
#define DES_SPEED_B_H      0x08
#define DES_POS_A_L        0x09
#define DES_POS_A_H        0x0A
#define DES_POS_B_L        0x0B
#define DES_POS_B_H        0x0C
#define PID_GAIN_MULT      0x0D
#define PID_GAIN_DIV       0x0E
#define PID_INT_MULT       0x0F
#define PID_INT_DIV        0x10
#define PID_DIF_MULT       0x11
#define PID_DIF_DIV        0x12

#define READONLY           0x20
/// Read only Registers
#define POSITION_A_L       0x20
#define POSITION_A_H       0x21
#define POSITION_B_L       0x22
#define POSITION_B_H       0x23
#define ADC_A_L            0x24
#define ADC_A_H            0x25
#define ADC_B_L            0x26
#define ADC_B_H            0x27
#define ADC_C_L            0x28
#define ADC_C_H            0x29
#define SWITCH_STATES      0x2A
#define REMEX_STATE        0x2F

#define REGMAP_SIZE 0x2F
#define UNKNOWN_REG 0xFF
#define UNKNOWN_PAR 0xFF
/****************************************************/

/// state machine states
enum i2c_states {
    start,
    register_set,
    stop,
    command_byte,
};

/**
 * i2c_slave_init will initialize the i2c bus and set the device address.
 *
 * @param[in] slave_addr is the desired device address.
 */
void i2c_slave_init(
        unsigned char slave_addr,
        uint8_t* memory_start,
        void (*command_handler)(const uint8_t)
        );

/**
 * transmit_byte take the value pointed to by the memory_map_ptr and load it into the
 * 'out' parameter. Sending the byte over the i2c bus.
 */
void transmit_byte(unsigned volatile int *out);

/**
 * receive_byte controls the state machine for the smbus. And takes care of writing and storing
 * data or commands.
 */
void receive_byte(const unsigned char in);

void start_condition_found();

void stop_condition_found();

#endif /* I2C_H_ */
