#pragma once

#ifdef ESP32
#include <esp8266toEsp32.h>
// Modul
#undef MODULE
#define MODULE WEMOS // [Module] Select default model
#endif  // ESP32

#ifdef ESP8266
// ESP8266
//
// UDP
#define PortUdp_write(p,n) PortUdp.write(p, n)
//
// Serial minimal type to hold the config
#define SerConfu8 uint8_t
#endif  // ESP8266
