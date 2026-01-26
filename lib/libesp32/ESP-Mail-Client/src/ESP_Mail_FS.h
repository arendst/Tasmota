

#pragma once

#ifndef ESP_MAIL_CONFIG_H
#define ESP_MAIL_CONFIG_H

#include "ESP_Mail_Client_Version.h"
#if !VALID_VERSION_CHECK(30409)
#error "Mixed versions compilation."
#endif

#include <Arduino.h>
#include "extras/MB_MCU.h"

/** 📌 Predefined Build Options
 * ⛔ Use following build flag to disable all predefined options.
 * -D DISABLE_ALL_OPTIONS
 */

/**📍 For enabling the device or library time setup from NTP server
 * ⛔ Use following build flag to disable.
 * -D DISABLE_NTP_TIME
 */
#define ENABLE_NTP_TIME

/**📍 For enabling the error string from error reason
 * ⛔ Use following build flag to disable.
 * -D DISABLE_ERROR_STRING
 */
#define ENABLE_ERROR_STRING

/**📍 For IMAP class compilation
 * ⛔ Use following build flag to disable.
 * -D DISABLE_IMAP
 */
#define ENABLE_IMAP

/**📍 For SMTP class compilation
 * ⛔ Use following build flag to disable.
 * -D DISABLE_SMTP
 */
#define ENABLE_SMTP

/**📍 For enabling PSRAM support
 * ⛔ Use following build flag to disable.
 * -D DISABLE_PSRAM
 */
#define ESP_MAIL_USE_PSRAM

/**📌 For enabling flash filesystem support
 *
 * 📍 For SPIFFS
 * #define ESP_MAIL_DEFAULT_FLASH_FS SPIFFS
 *
 *
 * 📍 For LittleFS Filesystem
 * #include <LittleFS.h>
 * #define ESP_MAIL_DEFAULT_FLASH_FS LittleFS
 *
 *
 * 📍 For SPIFFS Filesystem
 * #if defined(ESP32)
 * #include <SPIFFS.h>
 * #endif
 * #define ESP_MAIL_DEFAULT_FLASH_FS SPIFFS
 *
 *
 * 📍 For FAT Filesystem
 * #include <FFat.h>
 * #define ESP_MAIL_DEFAULT_FLASH_FS FFat  //For ESP32 FAT
 *
 * 🚫 Use following build flags to disable.
 * -D DISABLE_FLASH or -DDISABLE_FLASH in PlatformIO
 */

#if defined(ESP32) || defined(ESP8266) || defined(MB_ARDUINO_PICO)

#if defined(ESP8266) || defined(MB_ARDUINO_PICO)

#include <LittleFS.h>
#define ESP_MAIL_DEFAULT_FLASH_FS LittleFS

#elif defined(ESP_ARDUINO_VERSION) /* ESP32 core >= v2.0.x */ /* ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(2, 0, 0) */

#include <LittleFS.h>
#define ESP_MAIL_DEFAULT_FLASH_FS LittleFS

#else

#include <SPIFFS.h>
#define ESP_MAIL_DEFAULT_FLASH_FS SPIFFS

#endif

#endif

// For ESP32, format SPIFFS or FFat if mounting failed
#define ESP_MAIL_FORMAT_FLASH_IF_MOUNT_FAILED 1

/**📌 For enabling SD filesystem support
 *
 * 📍 For SD
 * #include <SD.h>
 * #define ESP_MAIL_DEFAULT_SD_FS SD
 * #define ESP_MAIL_CARD_TYPE_SD 1
 *
 * 📍 For SD MMC (ESP32)
 * #include <SD_MMC.h>
 * #define ESP_MAIL_DEFAULT_SD_FS SD_MMC //For ESP32 SDMMC
 * #define ESP_MAIL_CARD_TYPE_SD_MMC 1
 *
 * 📍 For SdFat on ESP32 and other devices except for ESP8266
 * #include <SdFat.h> //https://github.com/greiman/SdFat
 * static SdFat sd_fat_fs;   //should declare as static here
 * #define ESP_MAIL_DEFAULT_SD_FS sd_fat_fs
 * #define ESP_MAIL_CARD_TYPE_SD 1
 * #define ESP_MAIL_SD_FS_FILE SdFile
 *
 *
 * ⛔ Use following build flags to disable.
 * -D DISABLE_SD or -DDISABLE_SD in PlatformIO
 */
#if defined(ESP32) || defined(ESP8266)
#include <SD.h>
#define ESP_MAIL_DEFAULT_SD_FS SD
#define ESP_MAIL_CARD_TYPE_SD 1
#elif defined(MB_ARDUINO_PICO)
// Use SDFS (ESP8266SdFat) instead of SD
#include <SDFS.h>
#define ESP_MAIL_DEFAULT_SD_FS SDFS
#define ESP_MAIL_CARD_TYPE_SD 1
#endif

/** 🔖 Optional Build Options
 *
 * 🏷️ For silent operation (no debug printing and callback)
 * #define SILENT_MODE
 *
 * 🏷️ For ENC28J60 Ethernet module support in ESP8266
 * #define ENABLE_ESP8266_ENC28J60_ETH
 *
 * 🏷️ For W5500 Ethernet module support in ESP8266
 * #define ENABLE_ESP8266_W5500_ETH
 *
 * 🏷️ For W5100 Ethernet module support in ESP8266
 * #define ENABLE_ESP8266_W5100_ETH
 *
 * 🏷️ For disabling on-board WiFI functionality in case external Client usage
 * #define ESP_MAIL_DISABLE_ONBOARD_WIFI
 *
 * 🏷️ For disabling native (sdk) Ethernet functionality in case external Client usage
 * #define ESP_MAIL_DISABLE_NATIVE_ETHERNET
 *
 * 🏷️ For disabling SSL connection (also disabling TLS using STARTTLS) in MAP and SMTP application
 * #define ESP_MAIL_DISABLE_SSL
 *
 * 🏷️ For debug port assignment if SILENT_MODE option was not set
 * #define ESP_MAIL_DEBUG_PORT Serial
 */

#define ENABLE_ESP8266_ENC28J60_ETH

#if __has_include("Custom_ESP_Mail_FS.h")
#include "Custom_ESP_Mail_FS.h"
#endif

#include "extras/Build_Options.h"

#endif
