/**
 * @file tcs3472x_example.c
 * @brief Example application for demonstrating usage of the TCS3472x sensor with the I2C HAL.
 *
 * This program initializes the TCS3472x sensor, then continuously reads and prints all color
 * data from the sensor. It demonstrates the usage of the sensor API in a practical application.
 */

#include <stdio.h>
#include <unistd.h>

#include "tcs3472x_i2c_hal.h"
#include "tcs3472x.h"

#define DEVICE_ADDRESS  0x29

int main() {
    uint16_t clear_data, red_data, green_data, blue_data = 0;
    uint16_t all_colors[4] = {0};

    if (tcs3472x_i2c_hal_init(DEVICE_ADDRESS) < 0) {
        printf("I2C initialization failed.\n");
        return -1;
    }

    tcs3472x_init(); // Initialize the sensor settings

    while(1) {
        tcs3472x_get_all_colors_data(all_colors); // Read all colors
        clear_data = tcs3472x_get_clear_data();   // Individual clear data
        red_data = tcs3472x_get_red_data();       // Individual red data
        green_data = tcs3472x_get_green_data();   // Individual green data
        blue_data = tcs3472x_get_blue_data();     // Individual blue data

        printf("SINGLE |    C = %d    |    R = %d    |    G = %d    |    B = %d    |\n", clear_data, red_data, green_data, blue_data);
        printf("ALL    |    C = %d    |    R = %d    |    G = %d    |    B = %d    |\n", all_colors[0], all_colors[1], all_colors[2], all_colors[3]);

        sleep(1); // Delay for readability
    }

    tcs3472x_i2c_hal_close(); // Cleanup on exit, though this part of the code is never reached in this example
    return 0;
}
