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

void (*receive_func)(const unsigned char in);
void (*transmit_func)(unsigned volatile int *out);
void (*start_func)(void);
void (*stop_func)(void);

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
        void (*tx_cb)(unsigned volatile int *out),
        unsigned char slave_addr);

#endif /* I2C_H_ */
