# Overview

This is a C++ BLE library for Espressif ESP32 and Nordic nRF51/nRF52 devices that uses the NimBLE BLE stack.  
The aim is to maintain, as much as reasonable, the original ESP32 Arduino BLE API by while adding new features and making improvements in performance, resource use, and stability.  
<br/>

# What is NimBLE?
NimBLE is a completely open source Bluetooth Low Energy stack produced by [Apache](https://github.com/apache/mynewt-nimble).  
<br/>

# Arduino installation
**NOTE:** Nordic devices require using [n-able arduino core](https://github.com/h2zero/n-able-Arduino)

**Arduino Library manager:** Go to `sketch` -> `Include Library` -> `Manage Libraries` and search for NimBLE and install.

**Alternatively:** Download as .zip and extract to Arduino/libraries folder, or in Arduino IDE from Sketch menu -> Include library -> Add .Zip library.

`#include "NimBLEDevice.h"` at the beginning of your sketch.

Call `NimBLEDevice::init` in `setup`.  
<br/>

# Platformio installation
* Open platformio.ini, a project configuration file located in the root of PlatformIO project.  
* Add the following line to the lib_deps option of [env:] section:
```
h2zero/NimBLE-Arduino@^1.4.0
```
* Build a project, PlatformIO will automatically install dependencies.  
<br/>

# Using
This library is intended to be compatible with the original ESP32 BLE library functions and types with minor changes.

If you have not used the original BLE library please refer to the [New user guide](New_user_guide.md).

If you are familiar with the original library, see: [The migration guide](Migration_guide.md) for details.

Also see [Improvements and updates](Improvements_and_updates.md) for information about non-breaking changes.

For more advanced usage see [Usage tips](Usage_tips.md) for more performance and optimization.  
<br/>

## Examples
See the Refactored_original_examples in the examples folder for highlights of the differences with the original library.

More advanced examples highlighting many available features are in examples/NimBLE_Server, NimBLE_Client.

Beacon examples provided by [beegee-tokyo](https://github.com/beegee-tokyo) are in examples/BLE_Beacon_Scanner, BLE_EddystoneTLM_Beacon, BLE_EddystoneURL_Beacon.

Change the settings in the nimconfig.h file to customize NimBLE to your project, such as increasing max connections (default is 3).  
<br/>

## Arduino command line and platformio
As an alternative to changing the configuration in nimconfig.h, Arduino command line and platformio.ini options are available.

See the command line configuration options available in [Command line config](Command_line_config.md).  
<br/>

# Need help? Have a question or suggestion?
Come chat on [gitter](https://gitter.im/NimBLE-Arduino/community?utm_source=share-link&utm_medium=link&utm_campaign=share-link) or open an issue at [NimBLE-Arduino](https://github.com/h2zero/NimBLE-Arduino/issues) or [esp-nimble-cpp](https://github.com/h2zero/esp-nimble-cpp/issues)  
<br/>

# Acknowledgments

* [nkolban](https://github.com/nkolban) and [chegewara](https://github.com/chegewara) for the [original esp32 BLE library](https://github.com/nkolban/esp32-snippets/tree/master/cpp_utils) this project was derived from.
* [beegee-tokyo](https://github.com/beegee-tokyo) for contributing your time to test/debug and contributing the beacon examples.
* [Jeroen88](https://github.com/Jeroen88) for the amazing help debugging and improving the client code.  
<br/>

