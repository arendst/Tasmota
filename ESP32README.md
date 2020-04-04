![Tasmota logo](/tools/logo/TASMOTA_FullLogo_Vector.svg)

## esp32 port with minimal changes
This is my esp32 port, i try to make only minimal changes to the original
source code.
Here are the main things i have done 

- in platformio.ini [common] and [env] i 
  need "lib_extra_dirs" and "lib_ignore" options
- all code that is not for ESP32 i put in "#ifdef ESP8266" the define is from 
  framework
- all code for ESP32 is in "#ifdef ESP32"
- changed SerialConfig variable  from uint8_t to SerialConfig
- changed "HTTP_HEADER" to "HTTP_HEADER1", in ESP32 its an enums
- for ip adress 0 i used IPAddress(0,0,0,0)
- Special ESP.xxx call i change to ESP_xxx (ESP_rtcUserMemoryWrite, ...)
- because ESP32 has no SONOFF module, i have to put all 
  "if (SONOFF_xxx == Settings.module)" in "#ifdef ESP8266"
- variable "sleep" i changed to "ssleep" because of standard library sleep(..) function
- all esp32 stuff is in support_esp32.ino
- in tasmota.ino i include "tasmota_compat.h"
- in tasmota_template.h i use ifdef and tasmota_templESP32.h


- Only Flash Mode **DOUT** is supported. Do not use Flash Mode DIO / QIO / QOUT as it might seem to brick your device.
 It assures keeping your custom settings when you download and compile a new version. You have to make a copy from the provided `user_config_override_sample.h` file and add your setting overrides. To enable the override you have to set a compile define as documented in the `user_config_override_sample.h` file.

## Configuration Information
this is my


