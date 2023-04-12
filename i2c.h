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
#define MAX_CURRENT        0x00
#define CURRENT_DURATION   0x02
#define DES_SPEED_A        0x04
#define DES_SPEED_B        0x06
#define DES_POSITION_A     0x08
#define DES_POSITION_B     0x0A
#define PID_GAIN_MULT      0x0C
#define PID_GAIN_DIV       0x0D
#define PID_INT_MULT       0x0E
#define PID_INT_DIV        0x0F
#define PID_DIF_MULT       0x10
#define PID_DIF_DIV        0x11

#define MODE_REG           0x15

#define READONLY           0x20
/// Read only Registers
#define POSITION_A         0x20
#define POSITION_B         0x22
#define ADC_A              0x24
#define ADC_B              0x26
#define ADC_C              0x28
#define ADC_D              0x2A
#define SWITCH_STATES      0x2C
#define REMEX_STATE        0x2F

#define REGMAP_SIZE        0x2F
#define UNKNOWN_REG        0xFF
#define UNKNOWN_PAR        0xFF

/********** I2C Commands ****************************/

#define START_PWM          0x01
#define STOP_PWM           0x02
#define ZERO_MOTOR         0x04
#define SAVE_STATE         0x08

//For testing i2c map
#define TURN_ON_LIGHT      0xa5
#define TURN_OFF_LIGHT     0x2e

/****************************************************/

#define BYTES_TO_SHORT(array, index) \
    (((index) + 1 < sizeof(array)) && ((index) >= 0) ? \
    ((unsigned short int)((array)[(index+1)] << 8) | (array)[(index)]) : \
    0)

#define SHORT_TO_BYTES(array, size, index, value) \
    do { \
        if ((index) + sizeof(value) > (size)) { \
            __no_operation(); \
        } else { \
            unsigned int tmp = (unsigned int)(value); \
            (array)[index + 1] = (unsigned char)(tmp >> 8); \
            (array)[index] = (unsigned char)(tmp & 0xff); \
        } \
    } while (0)

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
        uint8_t* memory_start);

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

unsigned int get_int_from_memory(const uint8_t index);

void load_int_to_memory(const uint8_t index, const int value);

#endif /* I2C_H_ */
