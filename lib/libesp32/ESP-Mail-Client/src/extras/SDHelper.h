#pragma once

#ifndef SD_HELPER_H_
#define SD_HELPER_H_

#include <Arduino.h>
#include <ESP_Mail_Client.h>

// If SD Card used for storage, assign SD card type and FS used in src/ESP_Mail_FS.h and
// change the config for that card interfaces in this file (src/extras/SDHelper.h)

#if defined(MBFS_SD_FS) && defined(MBFS_CARD_TYPE_SD)

#if defined(ESP32)

#define SPI_CS_PIN 13
#define SPI_SCK_PIN 14
#define SPI_MISO_PIN 2
#define SPI_MOSI_PIN 15
#define SPI_CLOCK_IN_MHz 16

// #define SPI_CS_PIN 5
// #define SPI_SCK_PIN 18
// #define SPI_MISO_PIN 19
// #define SPI_MOSI_PIN 23
// #define SPI_CLOCK_IN_MHz 4 // may work on lower clock rate

#elif defined(ESP8266)
#define SPI_CS_PIN 15
#elif defined(MB_ARDUINO_PICO)
// Use SPI 1's SS (GPIO 13) port as CS for SPI
#define SPI_CS_PIN PIN_SPI1_SS
#elif defined(MB_ARDUINO_ARCH_SAMD) || defined(MB_ARDUINO_NANO_RP2040_CONNECT) || defined(MB_ARDUINO_TEENSY)

#define SPI_CS_PIN 4

#if defined(MBFS_SDFAT_ENABLED)
#define SPI_SCK_PIN -1
#define SPI_MISO_PIN -1
#define SPI_MOSI_PIN -1
#define SPI_CLOCK_IN_MHz 4 // works on lower clock rate
#endif

#endif

// if SdFat library installed and ESP_Mail_FS.h was set to use it (for ESP32 only)
#if defined(MBFS_ESP32_SDFAT_ENABLED) || defined(MBFS_SDFAT_ENABLED)

// https://github.com/greiman/SdFat
SdSpiConfig sdFatSPIConfig(SPI_CS_PIN, DEDICATED_SPI, SD_SCK_MHZ(SPI_CLOCK_IN_MHz));

#elif defined(ESP32) // if ESP32 and no SdFat library installed

SPIClass spi;

#elif defined(ESP8266)

SDFSConfig sdFSConfig(SPI_CS_PIN, SPI_HALF_SPEED);

#elif defined(MB_ARDUINO_PICO)

/** Use Pico SPI 1 for SPI
 * MISO  GPIO 12
 * MOSI  GPIO 15
 * SCK   GPIO 14
 * SS    GPIO 13
 */
SDFSConfig sdFSConfig(SPI_CS_PIN, SPI_HALF_SPEED, SPI1);

#endif

#endif

bool SD_Card_Mounting()
{

#if defined(MBFS_SD_FS) && defined(MBFS_CARD_TYPE_SD)

#if defined(MBFS_ESP32_SDFAT_ENABLED) || defined(MBFS_SDFAT_ENABLED)

    Serial.print("\nMounting SD Card... ");

    if (!MailClient.sdBegin(&sdFatSPIConfig, SPI_CS_PIN, SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN)) // pointer to SdSpiConfig, SS, SCK,MISO, MOSI
    {
        Serial.println("failed\n");
        return false;
    }
    else
    {
        Serial.println("success\n");
        return true;
    }

#elif defined(ESP32) // if ESP32 and no SdFat library installed

    Serial.print("\nMounting SD Card... ");

    spi.begin(SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN, SPI_CS_PIN); // SPI pins config -> SCK,MISO, MOSI, SS
    if (!MailClient.sdBegin(SPI_CS_PIN, &spi))                      // SS, pointer to SPIClass <- SPIClass object should defined as static or global
    {
        Serial.println("failed\n");
        return false;
    }
    else
    {
        Serial.println("success\n");
        return true;
    }
#elif defined(ESP8266)

    Serial.print("\nMounting SD Card... ");

    if (!MailClient.sdBegin(SPI_CS_PIN)) // or Firebase.sdBegin(&sdFSConfig)
    {
        Serial.println("failed\n");
        return false;
    }
    else
    {
        Serial.println("success\n");
        return true;
    }

#elif defined(MB_ARDUINO_PICO)

    Serial.print("\nMounting SD Card... ");

    if (!MailClient.sdBegin(&sdFSConfig)) // We begin with the SDFSConfig to use SPI 1 port
    {
        Serial.println("failed\n");
        return false;
    }
    else
    {
        Serial.println("success\n");
        return true;
    }

#endif

#endif

#if defined(MBFS_SD_FS) && defined(MBFS_CARD_TYPE_SD_MMC)

    Serial.print("\nMounting SD_MMC Card... ");

    if (!MailClient.sdMMCBegin("/sdcard", false, true))
    {
        Serial.println("failed\n");
        return false;
    }
    else
    {
        Serial.println("success\n");
        return true;
    }
#endif

    Serial.println("\nSD filesystem was not setup yet.");
    return false;
}

#endif
