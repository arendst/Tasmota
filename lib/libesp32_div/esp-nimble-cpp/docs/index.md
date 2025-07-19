# Overview

This is a C++ BLE library for the ESP32 that uses the NimBLE host stack instead of bluedroid.  
The aim is to maintain, as much as reasonable, the original bluedroid C++ & Arduino BLE API by while adding new features  
and making improvements in performance, resource use, and stability.  

**Testing shows a nearly 50% reduction in flash use and approx. 100kB less ram consumed vs the original!**  
*Your results may vary*
<br/>  

# What is NimBLE?
NimBLE is a completely open source Bluetooth Low Energy stack produced by [Apache](https://github.com/apache/mynewt-nimble).  
It is more suited to resource constrained devices than bluedroid and has now been ported to the ESP32 by Espressif.  
<br/>

# ESP-IDF Installation
### v4.0+
Download as .zip and extract or clone into the components folder in your esp-idf project.

Run menuconfig, go to `Component config->Bluetooth` enable Bluetooth and in `Bluetooth host` NimBLE.  
Configure settings in `NimBLE Options`.  
`#include "NimBLEDevice.h"` in main.cpp.  
Call `NimBLEDevice::init` in `app_main`.  
<br/>

### v3.2 & v3.3
The NimBLE component does not come with these versions of IDF (now included in 3.3.2 and above).   
A backport that works in these versions has been created and is [available here](https://github.com/h2zero/esp-nimble-component).  
Download or clone that repo into your project/components folder and run menuconfig.
Configure settings in `main menu -> NimBLE Options`.  

`#include "NimBLEDevice.h"` in main.cpp.  
Call `NimBLEDevice::init` in `app_main`.  
<br/>  

# Using
This library is intended to be compatible with the original ESP32 BLE functions and types with minor changes.  

If you have not used the original Bluedroid library please refer to the [New user guide](New_user_guide.md).

If you are familiar with the original library, see: [The migration guide](Migration_guide.md) for details.  

Also see [Improvements and updates](Improvements_and_updates.md) for information about non-breaking changes.  

For more advanced usage see [Usage tips](Usage_tips.md) for more performance and optimization.  
<br/>

# Need help? Have a question or suggestion?
Come chat on [gitter](https://gitter.im/NimBLE-Arduino/community?utm_source=share-link&utm_medium=link&utm_campaign=share-link) or open an issue at [NimBLE-Arduino](https://github.com/h2zero/NimBLE-Arduino/issues) or [esp-nimble-cpp](https://github.com/h2zero/esp-nimble-cpp/issues)  
<br/>

# Acknowledgments

* [nkolban](https://github.com/nkolban) and [chegewara](https://github.com/chegewara) for the [original esp32 BLE library](https://github.com/nkolban/esp32-snippets/tree/master/cpp_utils) this project was derived from.
* [beegee-tokyo](https://github.com/beegee-tokyo) for contributing your time to test/debug and contributing the beacon examples.
* [Jeroen88](https://github.com/Jeroen88) for the amazing help debugging and improving the client code.
<br/>  

