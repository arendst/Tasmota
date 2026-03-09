#ifndef ESP_MAIL_NETWORKS_PROVIDER_H
#define ESP_MAIL_NETWORKS_PROVIDER_H

#include "../ESP_Mail_FS.h"

#include "../ESP_Mail_Client_Version.h"
#if !VALID_VERSION_CHECK(30409)
#error "Mixed versions compilation."
#endif

#if __has_include(<esp_idf_version.h>)
#include <esp_idf_version.h>
#endif

#include "ESP8266_Supports.h"

// Renesas devices
#if defined(ARDUINO_UNOWIFIR4) || defined(ARDUINO_MINIMA) || defined(ARDUINO_PORTENTA_C33)
#define ESP_MAIL_STRSEP strsepImpl
#define ESP_MAIL_USE_STRSEP_IMPL
#else
#define ESP_MAIL_STRSEP strsep
#endif

#if defined(ESP32) || defined(ESP8266) || defined(ARDUINO_RASPBERRY_PI_PICO_W) || \
    defined(ARDUINO_UNOWIFIR4) || defined(ARDUINO_PORTENTA_C33) ||                \
    defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_H7_M4) ||         \
    __has_include(<WiFiNINA.h>) ||__has_include(<WiFi101.h>)

#if !defined(ESP_MAIL_DISABLE_ONBOARD_WIFI)

#define ESP_MAIL_WIFI_IS_AVAILABLE

#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#elif __has_include(<WiFiNINA.h>)
#include <WiFiNINA.h>
#elif __has_include(<WiFi101.h>)
#include <WiFi101.h>
#elif __has_include(<WiFiS3.h>)
#include <WiFiS3.h>
#elif __has_include(<WiFi.h>)
#include <WiFi.h>
#endif

#if !defined(ARDUINO_RASPBERRY_PI_PICO_W) && \
    !defined(MB_ARDUINO_ARCH_SAMD) &&        \
    !defined(MB_ARDUINO_MBED_PORTENTA) &&    \
    !defined(ARDUINO_UNOWIFIR4) &&           \
    !defined(ARDUINO_PORTENTA_C33) &&        \
    !defined(ARDUINO_NANO_RP2040_CONNECT)
#define ESP_MAIL_HAS_WIFI_DISCONNECT
#endif

#if defined(ARDUINO_PORTENTA_C33)
#define CONST_STRING_CAST char * // C33 WiFi library SSID is char array
#else
#define CONST_STRING_CAST const char *
#endif

// WiFiS3 getTime is currently return 0 (not implemented)
#if __has_include(<WiFiNINA.h>) || __has_include(<WiFi101.h>)
#define ESP_MAIL_HAS_WIFI_TIME
#endif

#if defined(MB_ARDUINO_PICO) && __has_include(<WiFiMulti.h>)
#define ESP_MAIL_HAS_WIFIMULTI
#endif

#endif

#endif

#if !defined(ESP_MAIL_DISABLE_NATIVE_ETHERNET)

#if defined(ESP32) && defined(CONFIG_ETH_ENABLED)
#include <ETH.h>
#define ESP_MAIL_ETH_IS_AVAILABLE
#elif defined(ESP8266) && defined(ESP8266_CORE_SDK_V3_X_X)
#if defined(INC_ENC28J60_LWIP) || defined(INC_W5100_LWIP) || defined(INC_W5500_LWIP)
#define ESP_MAIL_ETH_IS_AVAILABLE
#endif
#endif

#endif

#if defined(TINY_GSM_MODEM_SIM800) ||     \
    defined(TINY_GSM_MODEM_SIM808) ||     \
    defined(TINY_GSM_MODEM_SIM868) ||     \
    defined(TINY_GSM_MODEM_SIM900) ||     \
    defined(TINY_GSM_MODEM_SIM7000) ||    \
    defined(TINY_GSM_MODEM_SIM7000) ||    \
    defined(TINY_GSM_MODEM_SIM7000SSL) || \
    defined(TINY_GSM_MODEM_SIM7070) ||    \
    defined(TINY_GSM_MODEM_SIM7080) ||    \
    defined(TINY_GSM_MODEM_SIM7090) ||    \
    defined(TINY_GSM_MODEM_SIM5320) ||    \
    defined(TINY_GSM_MODEM_SIM5360) ||    \
    defined(TINY_GSM_MODEM_SIM5300) ||    \
    defined(TINY_GSM_MODEM_SIM7100) ||    \
    defined(TINY_GSM_MODEM_SIM7600) ||    \
    defined(TINY_GSM_MODEM_SIM7800) ||    \
    defined(TINY_GSM_MODEM_SIM7500) ||    \
    defined(TINY_GSM_MODEM_UBLOX) ||      \
    defined(TINY_GSM_MODEM_SARAR4) ||     \
    defined(TINY_GSM_MODEM_M95) ||        \
    defined(TINY_GSM_MODEM_BG96) ||       \
    defined(TINY_GSM_MODEM_A6) ||         \
    defined(TINY_GSM_MODEM_A7) ||         \
    defined(TINY_GSM_MODEM_M590) ||       \
    defined(TINY_GSM_MODEM_MC60) ||       \
    defined(TINY_GSM_MODEM_MC60E) ||      \
    defined(TINY_GSM_MODEM_XBEE) ||       \
    defined(TINY_GSM_MODEM_SEQUANS_MONARCH)
#define ESP_MAIL_TINYGSM_IS_AVAILABLE
#endif

#if defined(ESP_MAIL_TINYGSM_IS_AVAILABLE) && __has_include(<TinyGsmClient.h>)
#include <TinyGsmClient.h>
#define ESP_MAIL_GSM_MODEM_IS_AVAILABLE
#endif

#if defined(ESP_MAIL_WIFI_IS_AVAILABLE)
#define WiFI_CONNECTED (WiFi.status() == WL_CONNECTED)
#else
#define WiFI_CONNECTED false
#endif

#endif
