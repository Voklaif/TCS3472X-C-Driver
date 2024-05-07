# TCS3472x C Driver

## Overview

This project provides a C library for interfacing with the TCS3472x color sensor, which offers color and ambient light sensing capabilities. The library is designed for embedded systems and can be used in various applications such as color temperature measurement and ambient light sensing adjustments.

## Features

- Easy interfacing with the TCS3472x sensor via I2C.
- Reading color data (RGB and Clear).
- Example applications demonstrating the use of the library in a Linux environment.

## Prerequisites

Before you begin, ensure you have the following tools installed on your system:

- GCC (GNU Compiler Collection)
- Make
- Git (for cloning the repository)

## Getting Started

### Cloning the Repository

To get started with the TCS34725 C Driver, clone the repository to your local machine:

```bash
git clone https://github.com/voklaif/TCS34725-C-Driver.git
cd TCS34725-C-Driver
```

### Building the Project

To build the project, run the following command from the root of the repository:

```bash
make all
```

This will compile the source files and create the executable in the `build` directory.

### Running the Examples

Navigate to the `build` directory and execute the example program:

```bash
cd build
./tcs3472x_example
```

Documentation
For more detailed information about the API and functionalities, please refer to the code documentation in the include directory.

## Current Status and Future Plans

### Current Status

This project is in its initial stages, currently providing a basic framework for interfacing with the TCS3472x color sensor. It supports basic functionality such as initializing the sensor, reading raw color data, and basic sensor configuration.

### Future Plans

Going forward, I plan to expand the capabilities of this driver to include more advanced features such as:

- Control over integration time and gain settings.
- Support for sensors interrupt capabilieties.
- Improved calibration techniques and examples.
- Development of a comprehensive test suite for validating sensor functionalities under various conditions.
- Creation of higher-level abstractions for easier integration into user projects.

Contributions, ideas, and feedback to help drive the development of this project are welcomed.
