#ifndef HARDWAREIO_H_
#define HARDWAREIO_H_

/**
 * init_encoders will initialize hardware to read QEI encoders.
 *
 * @param[in] count_a integer pointer that should be updated from hardware interrupts for first encoder
 * if pointer is 0 then do not initialize encoder.
 * @param[in] count_b integer pointer that should be updated from hardware interrupts for secondary encoder
 * if pointer is 0 then do not initialize encoder.
 */
void init_encoders(int* count_a, int* count_b);

/**
 * init_switches will initialize hardware interrupts to read limit switches or buttons.
 *
 * @param[in] switch_a is the callback function that will be invoked when the limit switch connected to pin
 * 2.4 on the micro-controller is triggered.
 *
 * @param[in] switch_b is the callback function that will be invoked when the limit switch connected to pin
 * 2.5 on the micro-controller is triggered.
 *
 * if either parameter is null do not initialize hardware for the associated pin.
 */
void init_switches(void(*switch_a)(int), void(*switch_b)(int));

#endif /* HARDWARE_H_ */
