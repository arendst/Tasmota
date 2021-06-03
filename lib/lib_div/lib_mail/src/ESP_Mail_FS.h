#ifndef ESP_Mail_CONFIG_H
#define ESP_Mail_CONFIG_H
#include <Arduino.h>

/**
 * To use other flash file systems
 *
 * LittleFS File system
 *
 * #include <LittleFS.h>
 * #define ESP_Mail_DEFAULT_FLASH_FS LittleFS //For ESP8266 LitteFS
 *
 *
 * FFat File system
 *
 * #include <FFat.h>
 * #define ESP_Mail_DEFAULT_FLASH_FS FFat  //For ESP32 FFat
 *
*/
// #define ESP_Mail_DEFAULT_FLASH_FS SPIFFS

#ifdef ESP8266
 #include <LittleFS.h>
 #define ESP_Mail_DEFAULT_FLASH_FS LittleFS
#endif

#ifdef ESP32
 #include <LITTLEFS.h>
  #define ESP_Mail_DEFAULT_FLASH_FS LITTLEFS
#endif

/**
 * To use SD card file systems with different hardware interface
 * e.g. SDMMC hardware bus on the ESP32
 * https://github.com/espressif/arduino-esp32/tree/master/libraries/SD#faq
 *
 #include <SD_MMC.h>
 #define ESP_Mail_DEFAULT_SD_FS SD_MMC //For ESP32 SDMMC
 #define CARD_TYPE_SD_MMC 1
 *
*/
#define ESP_Mail_DEFAULT_SD_FS SD
#define CARD_TYPE_SD 1

//For ESP32, format SPIFFS or FFat if mounting failed
#define FORMAT_FLASH_IF_MOUNT_FAILED 1


#endif