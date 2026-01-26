#ifndef ESP_SSLClient_FS_H
#define ESP_SSLClient_FS_H

// #if (defined(ESP8266) || defined(ARDUINO_ARCH_RP2040)) && !defined(ARDUINO_NANO_RP2040_CONNECT)
// // for ESP8266 and Raspberry Pi Pico (RP2040) only except for Arduino Nano RP2040 Connect
// #define USE_EMBED_SSL_ENGINE
// #else
#define USE_LIB_SSL_ENGINE
// #endif

#pragma once

// for enable debugging
#define ESP_SSLCLIENT_ENABLE_DEBUG

/** Call ssl_client.setDebugLevel(x) to set the debug
 * esp_ssl_debug_none = 0
 * esp_ssl_debug_error = 1
 * esp_ssl_debug_warn = 2
 * esp_ssl_debug_info = 3
 * esp_ssl_debug_dump = 4
 */

// for debug port
#define ESP_SSLCLIENT_DEBUG_PORT Serial

// for SSL Error String
#define ESP_SSLCLIENT_ENABLE_SSL_ERROR_STRING

// for Filesystem support that required for CertStore
#define ESP_SSLCLIENT_USE_FILESYSTEM

// For external SRAM (PSRAM) support
#define ESP_SSLCLIENT_USE_PSRAM

#if defined __has_include
#if __has_include(<Custom_ESP_SSLClient_FS.h>)
#include "Custom_ESP_SSLClient_FS.h"
#endif
#endif

#endif