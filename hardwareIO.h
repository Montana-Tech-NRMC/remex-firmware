#ifndef HARDWAREIO_H_
#define HARDWAREIO_H_

#include <stdint.h>

#define QEI_1_A BIT4
#define QEI_1_B BIT5
#define QEI_2_A BIT6
#define QEI_2_B BIT7

/**
 * init_encoders will initialize hardware to read QEI encoders.
 *
 * @param[in] count_a integer pointer that should be updated from hardware interrupts for first encoder
 * if pointer is 0 then do not initialize encoder.
 * @param[in] count_b integer pointer that should be updated from hardware interrupts for secondary encoder
 * if pointer is 0 then do not initialize encoder.
 */
void init_encoders(uint8_t* memory_map);

#endif /* HARDWARE_H_ */
