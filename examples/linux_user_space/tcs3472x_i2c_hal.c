/**
 * @file tcs3472x_i2c_hal.c
 * @brief I2C communication implementation for TCS3472x sensor on Linux.
 *
 * This file implements the hardware abstraction layer (HAL) for communicating with the TCS3472x color sensor
 * over the I2C bus on a Linux platform. It provides initialization, read, write, and close functionalities
 * using Linux system calls.
 */

#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>          // For O_RDWR
#include <sys/ioctl.h>      // For ioctl()
#include <linux/i2c-dev.h>  // For I2C_SLAVE
#include <unistd.h>         // For close()

#define I2C_DEVICE_PATH "/dev/i2c-1"

#define I2C_WRITE_FAILED -1
#define I2C_READ_FAILED -2

static int8_t i2c_device = -1; ///< File descriptor for the I2C device.

/**
 * Initializes the I2C bus for communication with the sensor.
 * @param device_address The I2C address of the sensor.
 * @return 0 on success, -1 on error.
 */
int8_t tcs3472x_i2c_hal_init(int device_address) {
    i2c_device = open(I2C_DEVICE_PATH, O_RDWR);
    if (i2c_device < 0) {
        perror("Failed to open I2C bus");
        return -1;
    }
    
    if (ioctl(i2c_device, I2C_SLAVE, device_address) < 0) {
        perror("Failed to set I2C device address");
        close(i2c_device);
        return -1;
    }

    return 0;
}

/**
 * Writes data to the I2C bus.
 * @param buffer Pointer to the data buffer to write.
 * @param length Number of bytes to write.
 * @return 0 on success, I2C_WRITE_FAILED on error.
 */
int8_t tcs3472x_i2c_hal_write(const uint8_t *buffer, uint16_t length) {
    if (write(i2c_device, buffer, length) != length) {
        perror("I2C write error");
        return I2C_WRITE_FAILED;
    }

    return 0;
}

/**
 * Reads data from the I2C bus.
 * @param buffer Pointer to the buffer where data will be stored.
 * @param length Number of bytes to read.
 * @return 0 on success, I2C_READ_FAILED on error.
 */
int8_t tcs3472x_i2c_hal_read(uint8_t *buffer, uint16_t length) {
    if (read(i2c_device, buffer, length) != length) {
        perror("I2C read error");
        return I2C_READ_FAILED;
    }

    return 0;
}

/**
 * Closes the I2C communication.
 * @return 0 on success, -1 on error.
 */
int8_t tcs3472x_i2c_hal_close(void) {
    if (close(i2c_device) < 0) {
        perror("Failed to close I2C device");
        return -1;
    }
    return 0;
}
