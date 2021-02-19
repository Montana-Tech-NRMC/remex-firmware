/*
 * i2c.h
 *
 *  Created on: Oct 21, 2020
 *      Author: Justin Bak
 */

#ifndef I2C_H_
#define I2C_H_

#define SDA_PIN BIT2 // Pin 1.2
#define SCL_PIN BIT3 // Pin 1.3

/************* Register addresses **********************/

// Read / Write Registers
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
// Read only Registers
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

/****************************************************/

#define REGMAP_SIZE 0x2F

// The register map
extern unsigned char regmap[REGMAP_SIZE];
/**
 * i2c_slave_init will initialize the i2c bus and set the device address.
 *
 * @param[in] stt_cb function pointer to be called when a i2c start condition is detected
 * @param[in] stp_cb function pointer to be called when a i2c stop condition is detected
 * @param[in] rx_cb function pointer to be called when i2c byte is received from master
 * @param[in] tx_cb function pointer to be called to send i2c byte to master
 * @param[in] slave_addr is the desired device address.
 */
void i2c_slave_init(
        void (*stt_cb)(void),
        void (*stp_cb)(void),
        void (*rx_cb)(unsigned char in),
        void (*tx_cb)(unsigned volatile char *send),
        unsigned char slave_addr);

#endif /* I2C_H_ */
