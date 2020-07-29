# *** UPDATES ***
**Breaking changes:**   
**NEW** on June 21, 2020
> ```
> NimBLEClient::getServices(bool refresh = false)   
> NimBLERemoteService::getCharacteristics(bool refresh = false)   
> NimBLERemoteCharacteristic::getDecriptors(bool refresh = false)
>```
These methods now take an optional (bool) parameter.   
If true it will clear the respective vector and retrieve all the respective attributes from the peripheral.   
If false(default) it will return the respective vector empty or otherwise with the currently stored attributes. 

**NEW** on May 23, 2020   
Client and scan now use `std::vector` instead of `std::map` for storing the remote attribute database.   
   
This change will affect your application code if you use `NimBLEClient::getServices()` or `NimBLERemoteService::getCharacteristics()`   
in your application as they now return a pointer to `std::vector` of the respective attributes.   

In addition `NimBLERemoteService::getCharacteristicsByHandle()` has been removed as it is no longer maintained in the library.

These changes were necessary due to the amount of resources required to use `std::map`, it was not justifed by any benfit it provided.   
   
It is expected that there will be minimal impact on most applications, if you need help adjusting your code please create an issue.   

# NimBLE-Arduino
A fork of the NimBLE stack restructured for compilation in the Ardruino IDE with a CPP library for use with ESP32.

This library **significantly** reduces resource usage and improves performance for ESP32 BLE applications as compared    
with the bluedroid based library. The goal is to maintain, as much as reasonable, compatibility with the original   
library but refactored to use the NimBLE stack. In addition, this library will be more actively developed and maintained   
to provide improved capabilites and stability over the original.

## Resource use improvement:

### (Original) BLE_client example comparison (Debug):
#### Arduino BLE Library   
Sketch uses **1216377** bytes (58%) of program storage space.   
Memory after connection: Free Heap: **171548**  

#### NimBLE-Arduino library
Sketch uses **617256** bytes (29%) of program storage space.   
Memory after connection: Free Heap: **270336**   
***
### (Original) BLE_notify example comparison (Debug):   
#### Arduino BLE Library
Sketch uses **1208409** bytes (57%) of program storage space.   
Memory after connection: Free Heap: **173300**   

#### NimBLE-Arduino library   
Sketch uses **603432** bytes (28%) of program storage space.   
Memory after connection: Free Heap: **269792**  

**As shown: there is nearly a 50% reduction in flash use and approx. 100kB less ram consumed!**
 
  
# Installation:

Download as .zip and extract to Arduino/libraries folder, or in Arduino IDE from Sketch menu -> Include library -> Add .Zip library.

`#include "NimBLEDevice.h"` at the beginning of your sketch.

Tested and working with esp32-arduino v1.0.2 and 1.0.4 in Arduino IDE v1.8.12 and platform IO.


# Usage: 

This library is intended to be compatible with the original ESP32 BLE functions and types with minor changes.

Check the Refactored_original_examples in the examples folder for highlights of the differences with the original library.

More advanced examples highlighting many available features are in examples/ NimBLE_Server, NimBLE_Client.

Beacon examples provided by @beegee-tokyo are in examples/ BLE_Beacon_Scanner, BLE_EddystoneTLM_Beacon, BLE_EddystoneURL_Beacon.   

Change the settings in the `nimconfig.h` file to customize NimBLE to your project, such as increasing max connections, default is 3.


# Continuing development:

This Library is tracking the esp-nimble repo, nimble-1.2.0-idf master branch, currently [@46c1d9f.](https://github.com/espressif/esp-nimble)

Also tracking the NimBLE related changes in esp-idf, master branch, currently [@2ef4890.](https://github.com/espressif/esp-idf/tree/master/components/bt/host/nimble)

# Acknowledgments:

* @nkolban and @chegewara for the [original esp32 BLE library](https://github.com/nkolban/esp32-snippets) this project was derived from.
* @beegee-tokyo for contributing your time to test/debug and contributing the beacon examples.
* @Jeroen88 for the amazing help debugging and improving the client code.


# Todo:

1. Create documentation.
2. Add BLE Mesh code.
3. Expose more NimBLE features.


