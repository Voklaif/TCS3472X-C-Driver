CC=gcc
CFLAGS=-I./include
BUILD_DIR=build

all: $(BUILD_DIR) tcs3472x_example

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

tcs3472x_example: src/tcs3472x.c examples/linux_user_space/tcs3472x_example.c examples/linux_user_space/tcs3472x_i2c_hal.c
	$(CC) $(CFLAGS) src/tcs3472x.c examples/linux_user_space/tcs3472x_example.c examples/linux_user_space/tcs3472x_i2c_hal.c -o $(BUILD_DIR)/tcs3472x_example

clean:
	rm -rf $(BUILD_DIR)
