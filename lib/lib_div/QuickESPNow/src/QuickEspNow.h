#ifndef _QUICK_ESPNOW_h
#define _QUICK_ESPNOW_h

#if defined ESP32
#include "QuickEspNow_esp32.h"
#elif defined ESP8266
#include "QuickEspNow_esp8266.h"
#else
#error "Unsupported platform"
#endif //ESP32

#endif //_QUICK_ESPNOW_h