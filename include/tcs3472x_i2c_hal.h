/**
 * @file tcs3472x_i2c_hal.h
 * @brief Hardware abstraction layer (HAL) for I2C communication with the TCS3472x series sensors.
 *
 * This header provides the function prototypes for initializing, writing to, reading from, and closing
 * the I2C communication with the TCS3472x color sensors. It abstracts the details of I2C communication
 * to simplify sensor interactions in higher-level application code.
 */

#ifndef TCS3472X_I2C_HAL_H
#define TCS3472X_I2C_HAL_H

#include <stdint.h>

/**
 * @brief Initializes the I2C communication interface for TCS3472x sensors.
 *
 * Sets up the I2C bus on the specified device address for communication with the TCS3472x sensors.
 * This function should be called before any other operations are performed on the I2C bus.
 *
 * @param device_address The I2C device address of the sensor.
 * @return Returns 0 on successful initialization, or -1 if an error occurs.
 */
int8_t tcs3472x_i2c_hal_init(int device_address);

/**
 * @brief Writes data to the I2C bus connected to the TCS3472x sensor.
 *
 * This function sends a sequence of bytes stored in a buffer to the TCS3472x sensor via the I2C bus.
 *
 * @param buffer Pointer to the data buffer containing bytes to be written.
 * @param length Number of bytes in the buffer to write to the I2C bus.
 * @return Returns 0 on successful write, or -1 if an error occurs.
 */
int8_t tcs3472x_i2c_hal_write(uint8_t *buffer, uint16_t length);

/**
 * @brief Reads data from the I2C bus connected to the TCS3472x sensor.
 *
 * This function reads a sequence of bytes from the TCS3472x sensor into a buffer. The number of bytes
 * read is specified by the length parameter.
 *
 * @param buffer Pointer to the buffer where the read data will be stored.
 * @param length Number of bytes to read from the I2C bus.
 * @return Returns 0 on successful read, or -1 if an error occurs.
 */
int8_t tcs3472x_i2c_hal_read(uint8_t *buffer, uint16_t length);

/**
 * @brief Closes the I2C communication interface with the TCS3472x sensor.
 *
 * This function deinitializes the I2C interface that has been used for communicating with the TCS3472x sensor.
 * It should be called to cleanly shut down the I2C interface when no longer needed.
 *
 * @return Returns 0 on successful close, or -1 if an error occurs.
 */
int8_t tcs3472x_i2c_hal_close(void);

#endif // TCS3472X_I2C_HAL_H
