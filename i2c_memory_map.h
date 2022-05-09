/*
 * i2c_memory_map.h
 *
 *  Created on: Jan 28, 2022
 *      Author: Justin Bak
 */

#ifndef I2C_MEMORY_MAP_H_
#define I2C_MEMORY_MAP_H_

#define SLAVE_ADDR  0x44
#define UNKNOWN_REG 0xFF
#define UNKNOWN_PAR 0xFF

/************* Register addresses **********************/

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
/****************************************************/

/// state machine states
enum i2c_states {
    start,
    reg_set,
    stop,
    cmd_byte,
};

/**
 * init_i2c_memory_map sets up the memory map handler given a pointer to the register array.
 *
 * @param[in] is the pointer to the start of the i2c register map.
 */
void init_i2c_memory_map(unsigned char * map_start, void (*process_cmd)(unsigned const char));

/**
 * onI2CByteRecieved is a call back function that is called when the master writes a byte.
 *
 * @param[in] in is the byte received from the master.
 */
void onI2CByteReceived(const unsigned char in);

/**
 * onI2CTransmitByte is a callback function that is called when the master wants to read a byte from the slave.
 *
 * @param[out] the pointer to the transmit buffer to send a byte.
 */
void onI2CByteTransmit(unsigned volatile int *out);

/**
 * onI2CStartBit is a callback function for when a i2c start condition is detected from the master.
 */
void onI2CStartBit(void);

/**
 * onI2cStopBit is a callback function for when a i2c stop condition is detected from the master.
 */
void onI2CStopBit(void);

/**
 * combineInt will take in a register address and combine it with the adjacent address to create an int value.
 *
 * @param[in] registerStart register value to start on.
 * @return    the integer resulting from the combination of the two register locations.
 */
int combineInt(unsigned int registerStart);


/**
 * splitInt will take an integer value and a memory location and will split the integer to store it in adjacent
 * memory locations.
 *
 * @param[in] registerStart the register to start on.
 * @param[in] value         the value to store.
 */
void splitInt(unsigned int registerStart, int value);

#endif /* I2C_MEMORY_MAP_H_ */
