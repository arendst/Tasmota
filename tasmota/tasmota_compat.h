#pragma once

#ifdef ESP32
#include <esp8266toEsp32.h>
#define PACKED __attribute((__packed__))
// Modul
#undef MODULE
#define MODULE WEMOS // [Module] Select default model
#endif  // ESP32

#ifdef ESP8266
// ESP8266
#define PACKED
#define ESP_getResetReason() ESP.getResetReason()
#define ESP_reset() ESP.reset()
#define ESP_getBootVersion() ESP.getBootVersion()
#define ESP_getFlashChipId() ESP.getFlashChipId()
#define ESP_getSketchSize() ESP.getSketchSize()
#define ESP_getChipId() ESP.getChipId()
//
// UDP
#define PortUdp_write(p,n) PortUdp.write(p, n)
//
// Serial minimal type to hold the config
#define SerConfu8 uint8_t
#endif  // ESP8266
