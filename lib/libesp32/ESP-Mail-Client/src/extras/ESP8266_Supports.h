#ifndef ESP8266_SUPPORTS_H
#define ESP8266_SUPPORTS_H
#include <Arduino.h>
#include "MB_MCU.h"
#if defined(ESP8266) || defined(MB_ARDUINO_PICO)

#include <string>

//__GNUC__
//__GNUC_MINOR__
//__GNUC_PATCHLEVEL__

#ifdef __GNUC__
#if __GNUC__ > 4 || __GNUC__ == 10
#if defined(ARDUINO_ESP8266_GIT_VER)
#if ARDUINO_ESP8266_GIT_VER > 0
#define ESP8266_CORE_SDK_V3_X_X
#endif
#endif
#endif
#endif

#if defined __has_include

#if __has_include(<LwipIntfDev.h>)
#include <LwipIntfDev.h>
#endif

#if __has_include(<ENC28J60lwIP.h>)&& defined(ENABLE_ESP8266_ENC28J60_ETH)
#define INC_ENC28J60_LWIP
#include <ENC28J60lwIP.h>
#endif

#if __has_include(<W5100lwIP.h>) && defined(ENABLE_ESP8266_W5100_ETH)
#define INC_W5100_LWIP
#include <W5100lwIP.h>
#endif

#if __has_include(<W5500lwIP.h>)&& defined(ENABLE_ESP8266_W5500_ETH)
#define INC_W5500_LWIP
#include <W5500lwIP.h>
#endif

#if defined(MB_ARDUINO_PICO)

#endif

#endif

#endif

#endif