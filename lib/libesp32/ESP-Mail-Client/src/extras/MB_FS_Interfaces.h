#pragma once

/* Convert specific definitions to MB_FS definitions */
#ifndef MB_FS_INTERFACES_H
#define MB_FS_INTERFACES_H

#include <Arduino.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

// include definitions file
#include "./ESP_Mail_FS.h"

// 1. ESP_MAIL_DEFAULT_FLASH_FS -> MBFS_FLASH_FS
#if defined ESP_MAIL_DEFAULT_FLASH_FS
#define MBFS_FLASH_FS ESP_MAIL_DEFAULT_FLASH_FS
#endif

// 2. ESP_MAIL_DEFAULT_SD_FS -> MBFS_SD_FS
#if defined ESP_MAIL_DEFAULT_SD_FS
#define MBFS_SD_FS ESP_MAIL_DEFAULT_SD_FS
#endif

// 3. ESP_MAIL_CARD_TYPE_SD -> MBFS_CARD_TYPE_SD
#if defined(ESP_MAIL_CARD_TYPE_SD)
#define MBFS_CARD_TYPE_SD /*  */ ESP_MAIL_CARD_TYPE_SD
#endif

// 4. ESP_MAIL_CARD_TYPE_SD_MMC -> MBFS_CARD_TYPE_SD_MMC
#if defined(ESP_MAIL_CARD_TYPE_SD_MMC)
#define MBFS_CARD_TYPE_SD_MMC /*  */ ESP_MAIL_CARD_TYPE_SD_MMC
#endif

// 5. ESP_MAIL_FORMAT_FLASH_IF_MOUNT_FAILED -> MBFS_FORMAT_FLASH
#if defined(ESP_MAIL_FORMAT_FLASH_IF_MOUNT_FAILED)
#define MBFS_FORMAT_FLASH /*  */ ESP_MAIL_FORMAT_FLASH_IF_MOUNT_FAILED
#endif

#if defined(MBFS_SD_FS) || defined(MBFS_FLASH_FS)
#define MBFS_USE_FILE_STORAGE
#endif

// Only SdFat library from Bill Greiman
#if defined(ESP32) && defined(SD_FAT_VERSION) && defined(SD_FAT_VERSION_STR) && defined(MBFS_SD_FS) && defined(MBFS_CARD_TYPE_SD)
#define MBFS_ESP32_SDFAT_ENABLED
#ifndef ESP_MAIL_USE_SDFAT
#define ESP_MAIL_USE_SDFAT
#endif
#endif

// Only SdFat library from Bill Greiman
#if !defined(ESP32) && !defined(ESP8266) && !defined(ARDUINO_ARCH_RP2040) && defined(SD_FAT_VERSION) && defined(SD_FAT_VERSION_STR) && defined(MBFS_SD_FS) && defined(MBFS_CARD_TYPE_SD)
#define MBFS_SDFAT_ENABLED
#ifndef ESP_MAIL_USE_SDFAT
#define ESP_MAIL_USE_SDFAT
#endif
#endif

// For MB_String
#if defined(ESP_MAIL_USE_PSRAM)
#define MB_STRING_USE_PSRAM
#endif

#if defined(MBFS_SD_FS)

#if !defined(ESP_MAIL_SD_FILE)

#if defined(MBFS_ESP32_SDFAT_ENABLED)
#define MBFS_SD_FILE SdFile
#else
#if defined(ESP32) || defined(ESP8266) || defined(ARDUINO_ARCH_RP2040)
#define MBFS_SD_FILE fs::File
#else
#define MBFS_SD_FILE File
#endif

#endif

#else

#define MBFS_SD_FILE ESP_MAIL_SD_FILE

#endif

#endif

#ifndef MB_STRING_INCLUDE_CLASS
#define MB_STRING_INCLUDE_CLASS "MB_String.h"
#endif



#endif /* MB_FS_INTERFACES_H */
