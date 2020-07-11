![Tasmota logo](/tools/logo/TASMOTA_FullLogo_Vector.svg)

## ESP32 port with minimal changes
## Description:
This is my esp32 port, i try to make only minimal changes to the original source code 
from Theo Arends, now again for development branch.

## Checklist:
  - [x] The pull request is done against the latest dev branch
  - [x] Only relevant files were touched
  - [x] Only one feature/fix was added per PR.
  - [x] The code change is tested and works on core Tasmota_core_stage
  - [ ] The code change pass travis tests. **Your PR cannot be merged unless tests pass**
  - [x] I accept the [CLA](https://github.com/arendst/Tasmota/blob/development/CONTRIBUTING.md#contributor-license-agreement-cla).
  - [x] i checked binary "tasmota.bin" to be the same in development and development_esp32 branch

Here are the main things i have done

-   in "lib_extra_dirs" i have libesp32 directory for things missing in ESP32 framework
    my "ESP32-to-ESP8266-compat" has all files that are not available in ESP32
    so you dont have to change the source code and i write code to get the informations from ESP32
-   all librarys that are not compatibel i add to lib_ignore
-   all code that is not for ESP32 i put in "#ifdef ESP8266" the define is from espessif platform
-   all code for ESP32 is in "#ifdef ESP32"
-   SerConfu8 type uint8_t for SerialConfig list
-   changed "HTTP_HEADER" to "HTTP_HEADER1", in ESP32 its an enum
-   for ip adress 0 i used IPAddress(0,0,0,0)
-   Special ESP.xxx call i change to ESP_xxx (ESP_rtcUserMemoryWrite, ...) and write macros for ESP8266
-   because ESP32 has only WEMOS 32 modul, i exclude all code like this:
    "if (SONOFF_xxx == Settings.module)" in "#ifdef ESP8266"
-   variable "sleep" i changed to "ssleep" because of standard library sleep(..) function
-   all esp32 stuff is in support_esp32.ino
-   in tasmota.ino i include "tasmota_compat.h"
-   in tasmota_template.h i use ifdef and tasmota_templESP32.h
-   defines for sensors that currently don't work, i undef in tasmota_templESP32.h
-   comment fo "no warnig" in "xdrv_20_hue.ino" thats the only warning i had

Build info

Copy platformio_override_esp32.ini to platformio_override.ini an select your imagetype. 
You can build tasmota and tasmota32 Version with one build. 
If you need other versions change platformio_override.ini

stay at home, have fun and keep good

Jörg