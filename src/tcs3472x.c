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

// Magic numbers from datasheet
const float INTEGRATION_TIME_MULTIPLIER = 2.4f;
const uint16_t INTEGRATION_TIME_CONST = 256;
const uint16_t INTEGRATION_TIME_SPECIAL_CASE = 700;

static command_register_t command_register;

// Function prototypes
static uint16_t _get_color_data(uint8_t reg_address);
float _calc_atime_in_milliseconds (uint8_t atime);
void _write_command_register(uint8_t reg_address, command_type_t cmd_type);
uint8_t _build_command_register(uint8_t reg_address, command_type_t cmd_type);

/**
 * Writes a command to the command register of the TCS3472x sensor.
 *
 * @param reg_address The register address or special function code to write.
 * @param cmd_type The type of command being issued (repeat, auto-increment, special function).
 */
void _write_command_register(uint8_t reg_address, command_type_t cmd_type) {

    command_register.byte = _build_command_register(reg_address, cmd_type);

    if (tcs3472x_i2c_hal_write(&command_register.byte, 1) < 0) {
        LOG_ERROR("Failed to write command register.\r\n");
    }
    LOG_DEBUG("reg_address = 0x%X\r\n", reg_address);
}


void tcs3472x_init(void) {
    enable_register_t enable_register;

    enable_register.bits.aien = 1;
    enable_register.bits.wen = 1;
    enable_register.bits.aen = 1;
    enable_register.bits.pon = 1;

    _write_command_register(ENABLE_REGISTER, REPEAT_BYTE);

    if (tcs3472x_i2c_hal_write(&enable_register.byte, 1) < 0) {
        LOG_ERROR("Failed to initialize TCS3472x sensor (set PON).\r\n");
    }
}

uint8_t tcs3472x_get_enable(void) {
    uint8_t enable = 0;

    _write_command_register(ENABLE_REGISTER, REPEAT_BYTE);

    if (tcs3472x_i2c_hal_read(&enable, 1) < 0) {
        LOG_ERROR("Failed to read ENABLE register.\r\n");
        return 0;
    }
    return enable;
}

uint8_t tcs3472x_get_id(void) {
	uint8_t id = 0;

	_write_command_register(ID_REGISTER, REPEAT_BYTE);

    if (tcs3472x_i2c_hal_read(&id, 1) < 0) {
        LOG_ERROR("Failed to read ID register.\r\n");
        return 0;
    }
    return id;
}

float tcs3472x_set_atime(float integration_time) {
	const float MAX_INTEGRATION_TIME_ALLOWED = INTEGRATION_TIME_CONST * INTEGRATION_TIME_MULTIPLIER; // 614.4 milliseconds
	uint8_t atime_reg = 0;
	uint8_t send_data[2] = {0};
	float actual_integration_time = 0;

	if(integration_time >= MAX_INTEGRATION_TIME_ALLOWED) {
		atime_reg = 0x00;
		actual_integration_time = INTEGRATION_TIME_SPECIAL_CASE;
	}
	else {
		atime_reg = INTEGRATION_TIME_CONST - (integration_time / INTEGRATION_TIME_MULTIPLIER);
		actual_integration_time = _calc_atime_in_milliseconds(atime_reg);
	}

    command_register.byte = _build_command_register(ATIME_REGISTER, REPEAT_BYTE);

    send_data[0] = command_register.byte;
    send_data[1] = atime_reg;

    if (tcs3472x_i2c_hal_write(send_data, 2) < 0) {
        LOG_ERROR("Failed to set ATIME register.\r\n");
        return -1;
    }

	// Returns actual value of atime in milliseconds
	return actual_integration_time;
}

float tcs3472x_get_atime(void) {

	uint8_t atime_reg = 0;
	float atime_ms = 0;

	_write_command_register(ATIME_REGISTER, REPEAT_BYTE);

    if (tcs3472x_i2c_hal_read(&atime_reg, 1) < 0) {
        LOG_ERROR("Failed to read ATIME register.\r\n");
        return -1;
    }

    // Special case according to datasheet
    if (atime_reg == 0) {
    	atime_ms = INTEGRATION_TIME_SPECIAL_CASE;
    }
    // Calculate the integration time in milliseconds
    else {
    	atime_ms = _calc_atime_in_milliseconds(atime_reg);
    }
    return atime_ms;
}

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

uint16_t tcs3472x_get_clear_data(void) {
    return _get_color_data(CDATAL_REGISTER);
}

uint16_t tcs3472x_get_red_data(void) {
    return _get_color_data(RDATAL_REGISTER);
}

uint16_t tcs3472x_get_green_data(void) {
    return _get_color_data(GDATAL_REGISTER);
}

uint16_t tcs3472x_get_blue_data(void) {
    return _get_color_data(BDATAL_REGISTER);
}

uint8_t _build_command_register(uint8_t reg_address, command_type_t cmd_type) {
    command_register.bits.cmd = 1;
    command_register.bits.type = cmd_type;
    command_register.bits.addr_sf = reg_address;

    return command_register.byte;
}

float _calc_atime_in_milliseconds(uint8_t atime) {
	return (INTEGRATION_TIME_CONST-atime) * INTEGRATION_TIME_MULTIPLIER;
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
