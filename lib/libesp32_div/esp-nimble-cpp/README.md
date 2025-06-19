[![Release Version](https://img.shields.io/github/release/h2zero/esp-nimble-cpp.svg?style=plastic)
![Release Date](https://img.shields.io/github/release-date/h2zero/esp-nimble-cpp.svg?style=plastic)](https://github.com/h2zero/esp-nimble-cpp/releases/latest/)  

> [!IMPORTANT]
> Version 2 is now released!
> Check out the [1.x to 2.x Migration Guide](docs/1.x_to2.x_migration_guide.md) and [Release Notes](https://github.com/h2zero/esp-nimble-cpp/releases/latest/)

# esp-nimble-cpp

NimBLE CPP library for use with ESP32 that attempts to maintain compatibility with the [nkolban cpp_utils BLE API](https://github.com/nkolban/esp32-snippets/tree/master/cpp_utils).

**An Arduino version of this library, including NimBLE, can be [found here.](https://github.com/h2zero/NimBLE-Arduino)**

This library **significantly** reduces resource usage and improves performance for ESP32 BLE applications as compared    
with the bluedroid based library. The goal is to maintain, as much as reasonable, compatibility with the original   
library but using the NimBLE stack. In addition, this library will be more actively developed and maintained   
to provide improved capabilities and stability over the original.

**Testing shows a nearly 50% reduction in flash use and approx. 100kB less ram consumed vs the original!**  
*Your results may vary*  
<br/>

# What is NimBLE?
NimBLE is a completely open source Bluetooth Low Energy stack produced by [Apache](https://github.com/apache/mynewt-nimble).  
It is more suited to resource constrained devices than bluedroid and has now been ported to the ESP32 by Espressif.  
<br/>

# Installation

### ESP-IDF v4.0+
Download as .zip and extract or clone into the components folder in your esp-idf project.

Run menuconfig, go to `Component config->Bluetooth` enable Bluetooth and in `Bluetooth host` NimBLE.  
Configure settings in `NimBLE Options`.  
`#include "NimBLEDevice.h"` in main.cpp.  
Call `NimBLEDevice::init("");` in `app_main`.  
<br/>

# Using 
This library is intended to be compatible with the original ESP32 BLE functions and types with minor changes.  

If you have not used the original Bluedroid library please refer to the [New user guide](docs/New_user_guide.md).  

If you are familiar with the original library, see: [The migration guide](docs/Migration_guide.md) for details about breaking changes and migration.  

Also see [Improvements_and_updates](docs/Improvements_and_updates.md) for information about non-breaking changes.  

[Full API documentation and class list can be found here.](https://h2zero.github.io/esp-nimble-cpp/)  
<br/>  

## Using with Arduino as an IDF component and CMake
When using this library along with Arduino and compiling with *CMake* you must add `add_compile_definitions(ARDUINO_ARCH_ESP32=1)`  
in your project/CMakeLists.txt after the line `include($ENV{IDF_PATH}/tools/cmake/project.cmake)` to prevent Arduino from releasing BLE memory.
<br>

# Sponsors
Thank you to all the sponsors who support this project!

<!-- sponsors --><!-- sponsors -->

If you use this library for a commercial product please consider [sponsoring the development](https://github.com/sponsors/h2zero) to ensure the continued updates and maintenance.  
<br/>

# Acknowledgments
* [nkolban](https://github.com/nkolban) and [chegewara](https://github.com/chegewara) for the [original esp32 BLE library](https://github.com/nkolban/esp32-snippets/tree/master/cpp_utils) this project was derived from.
* [beegee-tokyo](https://github.com/beegee-tokyo) for contributing your time to test/debug and contributing the beacon examples.
* [Jeroen88](https://github.com/Jeroen88) for the amazing help debugging and improving the client code.  
<br/>  

