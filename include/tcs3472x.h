#ifndef TCS3472X_H
#define TCS3472X_H

#include <stdint.h>

/* Register Map */
#define ENABLE_REGISTER     0x00    ///< Enables states and interrupts (R/W)
#define ATIME_REGISTER      0x01    ///< RGBC time (R/W)
#define WTIME_REGISTER      0x03    ///< Wait time (R/W)
#define AILTL_REGISTER      0x04    ///< Clear interrupt low threshold low byte (R/W)
#define AILTH_REGISTER      0x05    ///< Clear interrupt low threshold high byte (R/W)
#define AIHTL_REGISTER      0x06    ///< Clear interrupt high threshold low byte (R/W)
#define AIHTH_REGISTER      0x07    ///< Clear interrupt high threshold high byte (R/W)
#define PERS_REGISTER       0x0C    ///< Interrupt persistence filter (R/W)
#define CONFIG_REGISTER     0x0D    ///< Configuration (R/W)
#define CONTROL_REGISTER    0x0F    ///< Control (R/W)
#define ID_REGISTER         0x12    ///< Device ID (Read only)
#define STATUS_REGISTER     0x13    ///< Device status (Read only)
#define CDATAL_REGISTER     0x14    ///< Clear data low byte (Read only)
#define CDATAH_REGISTER     0x15    ///< Clear data high byte (Read only)
#define RDATAL_REGISTER     0x16    ///< Red data low byte (Read only)
#define RDATAH_REGISTER     0x17    ///< Red data high byte (Read only)
#define GDATAL_REGISTER     0x18    ///< Green data low byte (Read only)
#define GDATAH_REGISTER     0x19    ///< Green data high byte (Read only)
#define BDATAL_REGISTER     0x1A    ///< Blue data low byte (Read only)
#define BDATAH_REGISTER     0x1B    ///< Blue data high byte (Read only)



/**
 * @brief Enable register structure to control various sensor states.
 */
typedef union {
    struct {
        uint8_t pon: 1;      ///< Power on bit.
        uint8_t aen: 1;      ///< RGBC enable bit.
        uint8_t reserved: 1; ///< Reserved bit.
        uint8_t wen: 1;      ///< Wait enable bit.
        uint8_t aien: 1;     ///< RGBC interrupt enable bit.
        uint8_t reserved2: 3;///< Reserved bits.
    } bits;
    uint8_t byte;           ///< Combined byte representation of the enable register.
} enable_register_t;


void tcs3472x_init(void); ///< Initializes the TCS3472x sensor.
uint8_t tcs3472x_get_enable(void); ///< Retrieves the current state of the enable register.

// uint8_t tcs3472x_get_device_id(void); TODO

/**
 * @brief Retrieves all color data from the sensor.
 * @param buff Pointer to a buffer where the color data will be stored. Buffer must be large enough to hold 4 uint16_t values.
 */
void tcs3472x_get_all_colors_data(uint16_t *buff);

uint16_t tcs3472x_get_clear_data(void);
uint16_t tcs3472x_get_red_data(void);
uint16_t tcs3472x_get_green_data(void);
uint16_t tcs3472x_get_blue_data(void);

#endif // TCS3472X_H