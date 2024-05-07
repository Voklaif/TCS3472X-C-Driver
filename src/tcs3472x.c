/**
 * @file tcs3472x.c
 * @brief Implementation of functions for interfacing with the TCS3472x color sensor over I2C.
 *
 * This file implements the functions declared in tcs3472x.h, providing concrete behaviors
 * for initializing the sensor, reading sensor data, and managing sensor settings. The implementation
 * abstracts the I2C protocol to enable easy interactions with the TCS3472x sensor.
 */

#include <stdio.h>
#include "tcs3472x.h"
#include "tcs3472x_i2c_hal.h"

#define LOG_ERROR(msg) fprintf(stderr, "Error: %s", msg)

/**
 * Enum for command types used with the TCS3472x sensor.
 */
typedef enum {
    REPEAT_BYTE = 0b00,        ///< Command to repeat reading/writing the same register.
    AUTO_INCREMENT = 0b01,     ///< Command to auto-increment register address on sequential reads/writes.
    SPECIAL_FUNCTION = 0b11,   ///< Command for special functions, not typically used in standard operations.
} command_type_t;

/**
 * Union to represent the command register structure, allowing manipulation of specific fields.
 */
typedef union {
    struct {
        uint8_t addr_sf: 5;    ///< Address or special function field.
        uint8_t type: 2;       ///< Type of subsequent transaction.
        uint8_t cmd: 1;        ///< Must be set to 1 to indicate the start of a command.
    } bits;
    uint8_t byte;             ///< Full byte representation of the command register for easy transmission.
} command_register_t;

static command_register_t command_register;

// Function prototypes
static uint16_t _get_color_data(uint8_t reg_address);
void _write_command_register(uint8_t reg_address, command_type_t cmd_type);

/**
 * Writes a command to the command register of the TCS3472x sensor.
 *
 * @param reg_address The register address or special function code to write.
 * @param cmd_type The type of command being issued (repeat, auto-increment, special function).
 */
void _write_command_register(uint8_t reg_address, command_type_t cmd_type) {
    command_register.bits.cmd = 1;
    command_register.bits.type = cmd_type;
    command_register.bits.addr_sf = reg_address;

    if (tcs3472x_i2c_hal_write(&command_register.byte, 1) < 0) {
        LOG_ERROR("Failed to write command register.\n");
    }
}

/**
 * Initializes the TCS3472x sensor by setting the power on and enabling RGB color detection.
 */
void tcs3472x_init(void) {
    enable_register_t enable_register;

    enable_register.bits.aen = 1;
    enable_register.bits.pon = 1;

    _write_command_register(ENABLE_REGISTER, REPEAT_BYTE);

    if (tcs3472x_i2c_hal_write(&enable_register.byte, 1) < 0) {
        LOG_ERROR("Failed to initialize TCS3472x sensor (set PON).\n");
    }
}

/**
 * Retrieves the current status of the enable register from the TCS3472x sensor.
 *
 * @return Current value of the enable register.
 */
uint8_t tcs3472x_get_enable(void) {
    uint8_t enable = 0;

    _write_command_register(ENABLE_REGISTER, REPEAT_BYTE);

    if (tcs3472x_i2c_hal_read(&enable, 1) < 0) {
        LOG_ERROR("Failed to read ENABLE register.\n");
        return 0;
    }
    return enable;
}

/**
 * Retrieves all color channel data from the TCS3472x sensor.
 *
 * @param buff Pointer to an array where the color data will be stored.
 */
void tcs3472x_get_all_colors_data(uint16_t *buff) {
    uint8_t data[8] = {0};  // 2 bytes for each color (clear, red, green, blue)
    uint16_t combined_data = 0;

    _write_command_register(CDATAL_REGISTER, AUTO_INCREMENT);
    tcs3472x_i2c_hal_read(data, sizeof(data));

    combined_data = (data[1] << 8) | data[0];
    buff[0] = combined_data;
    combined_data = (data[3] << 8) | data[2];
    buff[1] = combined_data;
    combined_data = (data[5] << 8) | data[4];
    buff[2] = combined_data;
    combined_data = (data[7] << 8) | data[6];
    buff[3] = combined_data;
}

/**
 * Reads and returns the clear channel data from the sensor.
 *
 * @return The 16-bit clear channel data.
 */
uint16_t tcs3472x_get_clear_data(void) {
    return _get_color_data(CDATAL_REGISTER);
}

/**
 * Reads and returns the red channel data from the sensor.
 *
 * @return The 16-bit red channel data.
 */
uint16_t tcs3472x_get_red_data(void) {
    return _get_color_data(RDATAL_REGISTER);
}

/**
 * Reads and returns the green channel data from the sensor.
 *
 * @return The 16-bit green channel data.
 */
uint16_t tcs3472x_get_green_data(void) {
    return _get_color_data(GDATAL_REGISTER);
}

/**
 * Reads and returns the blue channel data from the sensor.
 *
 * @return The 16-bit blue channel data.
 */
uint16_t tcs3472x_get_blue_data(void) {
    return _get_color_data(BDATAL_REGISTER);
}

/**
 * Helper function to read a specific color data register.
 *
 * @param reg_address The register address of the color data to read.
 * @return The 16-bit color data.
 */
static uint16_t _get_color_data(uint8_t reg_address) {
    uint8_t data[2] = {0};
    uint16_t combined_data = 0;

    _write_command_register(reg_address, AUTO_INCREMENT);
    tcs3472x_i2c_hal_read(data, sizeof(data));

    combined_data = (data[1] << 8) | data[0];
    return combined_data;
}