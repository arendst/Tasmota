/**
 * The MB_FS, filesystems wrapper class v1.0.16
 *
 * This wrapper class is for SD and Flash filesystems interface which supports SdFat (//https://github.com/greiman/SdFat)
 *
 *  Created June 14, 2023
 *
 * The MIT License (MIT)
 * Copyright (c) 2023 K. Suwatchai (Mobizt)
 *
 *
 * Permission is hereby granted, free of charge, to any person returning a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#if 0
#ifndef MBFS_CLASS_H
#define MBFS_CLASS_H

#include <Arduino.h>
#include "MB_MCU.h"

#define FS_NO_GLOBALS
#if defined(ESP32) || defined(ESP8266) || defined(MB_ARDUINO_PICO)
#if defined(MBFS_FLASH_FS) || defined(MBFS_SD_FS)
#include <FS.h>
#endif
#endif
#include "MB_FS_Interfaces.h"
#include MB_STRING_INCLUDE_CLASS

#if defined(MBFS_FLASH_FS) || defined(MBFS_SD_FS)
#include "SPI.h"
#endif

#if defined(ESP32) && __has_include(<sys/stat.h>)
#ifdef _LITTLEFS_H_
#define MB_FS_USE_POSIX_STAT
#include <sys/stat.h>
namespace mb_fs_ns
{
    inline bool exists(const char *mountPoint, const char *filename)
    {
        MB_String path = mountPoint;
        path += filename;
        struct stat st;
        return stat(path.c_str(), &st) == 0;
    }
};
#endif
#endif

using namespace mb_string;

#if defined(BOARD_HAS_PSRAM) && defined(MB_STRING_USE_PSRAM)
#include <esp32-hal-psram.h>
#endif

#define MB_FS_ERROR_FILE_IO_ERROR -300
#define MB_FS_ERROR_FILE_NOT_FOUND -301
#define MB_FS_ERROR_FLASH_STORAGE_IS_NOT_READY -302
#define MB_FS_ERROR_SD_STORAGE_IS_NOT_READY -303
#define MB_FS_ERROR_FILE_STILL_OPENED -304

typedef enum
{
    mb_fs_mem_storage_type_undefined,
    mb_fs_mem_storage_type_flash,
    mb_fs_mem_storage_type_sd
} mb_fs_mem_storage_type;

typedef enum
{
    mb_fs_open_mode_undefined = -1,
    mb_fs_open_mode_read = 0,
    mb_fs_open_mode_write,
    mb_fs_open_mode_append
} mb_fs_open_mode;

#define mbfs_file_type mb_fs_mem_storage_type
#define mbfs_flash mb_fs_mem_storage_type_flash
#define mbfs_sd mb_fs_mem_storage_type_sd
#define mbfs_undefined mb_fs_mem_storage_type_undefined

#define mbfs_type (mbfs_file_type)

#if defined(ARDUINO_ARCH_SAMD) || defined(__AVR_ATmega4809__) || defined(ARDUINO_NANO_RP2040_CONNECT)
#if !defined(MBFS_SDFAT_ENABLED)
struct mbfs_sd_config_info_t
{
    int ss = -1;
};
#endif
#elif defined(ESP32) || defined(MBFS_SDFAT_ENABLED)

#if defined(ESP32)
struct mbfs_sd_mmc_config_info_t
{
    const char *mountpoint = "";
    bool mode1bit = false;
    bool format_if_mount_failed = false;
};
#endif

struct mbfs_sd_config_info_t
{
    int ss = -1;
    int sck = -1;
    int miso = -1;
    int mosi = -1;
    uint32_t frequency = 4000000;

#if defined(MBFS_ESP32_SDFAT_ENABLED) || defined(MBFS_SDFAT_ENABLED)

    SdSpiConfig *sdFatSPIConfig = nullptr;
    SdioConfig *sdFatSDIOConfig = nullptr;

#endif

#if defined(ESP32)

#if defined(MBFS_SD_FS)
    SPIClass *spiConfig = nullptr;
#endif
    mbfs_sd_mmc_config_info_t sdMMCConfig;
#endif

#if defined(MBFS_SDFAT_ENABLED)
    SPIClass *spiConfig = nullptr;
#endif
};

#elif defined(ESP8266) || defined(MB_ARDUINO_PICO)
struct mbfs_sd_config_info_t
{
    int ss = -1;
#if defined(MBFS_SD_FS)
    SDFSConfig *sdFSConfig = nullptr;
#endif
};
#else
struct mbfs_sd_config_info_t
{
    int ss = -1;
};
#endif

class MB_FS
{

public:
    MB_FS() {}
    ~MB_FS() {}

    struct mbfs_sd_config_info_t sd_config;

    // Assign the SD card interfaces with GPIO pins.
    bool sdBegin(int ss = -1, int sck = -1, int miso = -1, int mosi = -1, uint32_t frequency = 4000000)
    {
        if (sd_rdy)
            return true;

#if defined(MBFS_SD_FS) && defined(MBFS_CARD_TYPE_SD)
        sd_config.ss = ss;
#if defined(ESP32)
        sd_config.sck = sck;
        sd_config.miso = miso;
        sd_config.mosi = mosi;
        SPI.begin(sck, miso, mosi, ss);
        sd_config.frequency = frequency;
        return sdSPIBegin(ss, &SPI, frequency);
#elif defined(ESP8266) || defined(ARDUINO_ARCH_SAMD) || defined(__AVR_ATmega4809__) || defined(ARDUINO_NANO_RP2040_CONNECT)
        sd_rdy = MBFS_SD_FS.begin(ss);
        return sd_rdy;
#elif defined(MB_ARDUINO_PICO)
        SDFSConfig c;
        c.setCSPin(ss);
        c.setSPISpeed(frequency);
        MBFS_SD_FS.setConfig(c);
        sd_rdy = MBFS_SD_FS.begin();
        return sd_rdy;
#endif

#endif
        return false;
    }

#if defined(ESP32) && defined(MBFS_SD_FS) && defined(MBFS_CARD_TYPE_SD)

    // Assign the SD card interfaces with SPIClass object pointer (ESP32 only).
    bool sdSPIBegin(int ss, SPIClass *spiConfig, uint32_t frequency)
    {

        if (sd_rdy)
            return true;

        sd_config.frequency = frequency;

#if defined(ESP32)

        sd_config.ss = ss;

        if (spiConfig)
            sd_config.spiConfig = spiConfig;
        else
            sd_config.spiConfig = &SPI;

#if !defined(MBFS_ESP32_SDFAT_ENABLED) || defined(MBFS_SDFAT_ENABLED)
        if (ss > -1)
            sd_rdy = MBFS_SD_FS.begin(ss, *sd_config.spiConfig, frequency);
        else
            sd_rdy = MBFS_SD_FS.begin();
#endif

#elif defined(ESP8266) || defined(MB_ARDUINO_PICO)

        cfg->_int.sd_config.sck = sck;

        if (ss > -1)
            sd_rdy = MBFS_SD_FS.begin(ss);
        else
            sd_rdy = MBFS_SD_FS.begin(SD_CS_PIN);
#endif

        return sd_rdy;
    }

#endif

#if defined(MBFS_ESP32_SDFAT_ENABLED) || defined(MBFS_SDFAT_ENABLED)

    // Assign the SD card interfaces with SdSpiConfig object pointer and SPI pins assignment.
    bool sdFatBegin(SdSpiConfig *sdFatSPIConfig, int ss, int sck, int miso, int mosi)
    {

        if (sd_rdy)
            return true;

        if (sdFatSPIConfig)
        {
            sd_config.sdFatSPIConfig = sdFatSPIConfig;
            sd_config.spiConfig = &SPI;
            sd_config.ss = ss;

#if defined(ESP32)
            if (ss > -1)
                sd_config.spiConfig->begin(sck, miso, mosi, ss);
#endif

            sd_rdy = MBFS_SD_FS.begin(*sd_config.sdFatSPIConfig);
            return sd_rdy;
        }

        return false;
    }

    // Assign the SD card interfaces with SdioConfig object pointer.
    bool sdFatBegin(SdioConfig *sdFatSDIOConfig)
    {

        if (sd_rdy)
            return true;

#if defined(HAS_SDIO_CLASS) // Default is 0 (no SDIO) in SdFatConfig.h

#if HAS_SDIO_CLASS

        if (sdFatSDIOConfig)
        {
            sd_config.sdFatSDIOConfig = sdFatSDIOConfig;

            sd_rdy = MBFS_SD_FS.begin(*sd_config.sdFatSDIOConfig);
            return sd_rdy;
        }
#endif

#endif

        return false;
    }
#endif

#if (defined(ESP8266) || defined(MB_ARDUINO_PICO)) && defined(MBFS_SD_FS)
    // Assign the SD card interfaces with SDFSConfig object pointer (ESP8266 and Pico only).
    bool sdFatBegin(SDFSConfig *sdFSConfig)
    {

        if (sd_rdy)
            return true;

        if (sdFSConfig)
        {
            sd_config.sdFSConfig = sdFSConfig;
            SDFS.setConfig(*sd_config.sdFSConfig);
            sd_rdy = SDFS.begin();
            return sd_rdy;
        }

        return false;
    }
#endif

    // Assign the SD_MMC card interfaces (ESP32 only).
    bool sdMMCBegin(const char *mountpoint, bool mode1bit, bool format_if_mount_failed)
    {

        if (sd_rdy)
            return true;

#if defined(ESP32)
#if defined(MBFS_CARD_TYPE_SD_MMC)

        sd_config.sdMMCConfig.mountpoint = mountpoint;
        sd_config.sdMMCConfig.mode1bit = mode1bit;
        sd_config.sdMMCConfig.format_if_mount_failed = format_if_mount_failed;

        sd_rdy = MBFS_SD_FS.begin(mountpoint, mode1bit, format_if_mount_failed);
        return sd_rdy;
#endif
#endif
        return false;
    }

    // Check the mounting status of Flash storage.
    bool flashReady()
    {
#if defined MBFS_FLASH_FS

        if (flash_rdy)
            return true;

#if defined(ESP32)

#if defined(MBFS_FORMAT_FLASH)
        flash_rdy = MBFS_FLASH_FS.begin(true);
#else
        flash_rdy = MBFS_FLASH_FS.begin();
#endif

#elif defined(ESP8266) || defined(MB_ARDUINO_PICO)
        flash_rdy = MBFS_FLASH_FS.begin();
#endif

#endif

        return flash_rdy;
    }

    // Check the mounting status of SD storage.
    bool sdReady()
    {

#if defined(MBFS_SD_FS)

        if (sd_rdy)
            return true;

#if defined(ESP32)

#if defined(MBFS_CARD_TYPE_SD)

        if (!sd_config.spiConfig)
        {
            if (sd_config.ss > -1)
                SPI.begin(sd_config.sck, sd_config.miso, sd_config.mosi, sd_config.ss);
            sd_config.spiConfig = &SPI;
        }

#if defined(MBFS_ESP32_SDFAT_ENABLED) || defined(MBFS_SDFAT_ENABLED)

        if (!sd_rdy)
        {
            if (sd_config.sdFatSPIConfig)
                sd_rdy = MBFS_SD_FS.begin(*sd_config.sdFatSPIConfig);
            else if (sd_config.sdFatSDIOConfig)
                sd_rdy = MBFS_SD_FS.begin(*sd_config.sdFatSDIOConfig);
        }

#else
        if (!sd_rdy)
            sd_rdy = sdSPIBegin(sd_config.ss, sd_config.spiConfig, sd_config.frequency);

#endif

#elif defined(MBFS_CARD_TYPE_SD_MMC)
        if (!sd_rdy)
            sd_rdy = sdMMCBegin(sd_config.sdMMCConfig.mountpoint, sd_config.sdMMCConfig.mode1bit, sd_config.sdMMCConfig.format_if_mount_failed);
#endif

#elif defined(ESP8266) || defined(MB_ARDUINO_PICO)
        if (!sd_rdy)
        {
            if (sd_config.sdFSConfig)
                sd_rdy = sdFatBegin(sd_config.sdFSConfig);
            else
                sd_rdy = sdBegin(sd_config.ss);
        }

#elif defined(ARDUINO_ARCH_SAMD) || defined(__AVR_ATmega4809__) || defined(ARDUINO_NANO_RP2040_CONNECT)
        if (!sd_rdy)
            sd_rdy = sdBegin(sd_config.ss);
#endif

#endif

        return sd_rdy;
    }

    // Check the mounting status of Flash or SD storage with mb_fs_mem_storage_type.
    bool checkStorageReady(mbfs_file_type type)
    {

#if defined(MBFS_USE_FILE_STORAGE)
        if (type == mbfs_flash)
        {
            if (!flash_rdy)
                flashReady();
            return flash_rdy;
        }
        else if (type == mbfs_sd)
        {
            if (!sd_rdy)
                sdReady();
            return sd_rdy;
        }
#endif

        return false;
    }

    // Open file for read or write with file name, mb_fs_mem_storage_type and mb_fs_open_mode.
    // return size of file (read) or 0 (write) or negative value for error
    int open(const MB_String &filename, mbfs_file_type type, mb_fs_open_mode mode)
    {

#if defined(MBFS_USE_FILE_STORAGE)

        if (!checkStorageReady(type))
        {
            if (type == mbfs_flash)
                return MB_FS_ERROR_FLASH_STORAGE_IS_NOT_READY;
            else if (type == mbfs_sd)
                return MB_FS_ERROR_SD_STORAGE_IS_NOT_READY;
            else
                return MB_FS_ERROR_FILE_IO_ERROR;
        }

        if (mode == mb_fs_open_mode_read)
        {
            if (!existed(filename.c_str(), type))
                return MB_FS_ERROR_FILE_NOT_FOUND;
        }

        int ret = openFile(filename, type, mode);

        if (ret < 0)
            return ret;

        if (ready(type))
            return ret;

#endif
        return MB_FS_ERROR_FILE_IO_ERROR;
    }

    // Check if file is already open.
    bool ready(mbfs_file_type type)
    {
#if defined(MBFS_FLASH_FS)
        if (type == mbfs_flash && mb_flashFs)
            return true;
#endif
#if defined(MBFS_SD_FS)
        if (type == mbfs_sd && mb_sdFs)
            return true;
#endif
        return false;
    }

    // Get file for read/write with file name, mb_fs_mem_storage_type and mb_fs_open_mode.
    int size(mbfs_file_type type)
    {
        int size = 0;

#if defined(MBFS_FLASH_FS)
        if (type == mbfs_flash && mb_flashFs)
            size = mb_flashFs.size();
#endif
#if defined(MBFS_SD_FS)
        if (type == mbfs_sd && mb_sdFs)
            size = mb_sdFs.size();
#endif
        return size;
    }

    // Check if file is ready to read/write.
    int available(mbfs_file_type type)
    {
        int available = 0;

#if defined(MBFS_FLASH_FS)
        if (type == mbfs_flash && mb_flashFs)
            available = mb_flashFs.available();
#endif
#if defined(MBFS_SD_FS)
        if (type == mbfs_sd && mb_sdFs)
            available = mb_sdFs.available();
#endif
        return available;
    }

    // Read byte array. Return the number of bytes that completed read or negative value for error.
    int read(mbfs_file_type type, uint8_t *buf, size_t len)
    {
        int read = 0;
#if defined(MBFS_FLASH_FS)
        if (type == mbfs_flash && mb_flashFs)
            read = mb_flashFs.read(buf, len);
#endif
#if defined(MBFS_SD_FS)
        if (type == mbfs_sd && mb_sdFs)
            read = mb_sdFs.read(buf, len);
#endif
        return read;
    }

    // Print char array. Return the number of bytes that completed write or negative value for error.
    int print(mbfs_file_type type, const char *str)
    {
        int write = 0;
#if defined(MBFS_FLASH_FS)
        if (type == mbfs_flash && mb_flashFs)
            write = mb_flashFs.print(str);
#endif
#if defined(MBFS_SD_FS)

        if (type == mbfs_sd && mb_sdFs)
            write = mb_sdFs.print(str);
#endif
        return write;
    }

    // Print char array with new line. Return the number of bytes that completed write or negative value for error.
    int println(mbfs_file_type type, const char *str)
    {
        int write = print(type, str);
        if (write == (int)strlen(str))
            write += print(type, (const char *)MBSTRING_FLASH_MCR("\n"));
        return write;
    }

    // Print integer. Return the number of bytes that completed write or negative value for error.
    int print(mbfs_file_type type, int v)
    {
        int write = 0;
#if defined(MBFS_FLASH_FS)
        if (type == mbfs_flash && mb_flashFs)
            write = mb_flashFs.print(v);
#endif
#if defined(MBFS_SD_FS)

        if (type == mbfs_sd && mb_sdFs)
            write = mb_sdFs.print(v);
#endif
        return write;
    }

    // Print integer with newline. Return the number of bytes that completed write or negative value for error.
    int println(mbfs_file_type type, int v)
    {
        int write = print(type, v);
        if (write > 0)
            write += print(type, (const char *)MBSTRING_FLASH_MCR("\n"));
        return write;
    }

    int print(mbfs_file_type type, unsigned int v)
    {
        int write = 0;
#if defined(MBFS_FLASH_FS)
        if (type == mbfs_flash && mb_flashFs)
            write = mb_flashFs.print(v);
#endif
#if defined(MBFS_SD_FS)

        if (type == mbfs_sd && mb_sdFs)
            write = mb_sdFs.print(v);
#endif
        return write;
    }

    // Print integer with newline. Return the number of bytes that completed write or negative value for error.
    int println(mbfs_file_type type, unsigned int v)
    {
        int write = print(type, v);
        if (write > 0)
            write += print(type, (const char *)MBSTRING_FLASH_MCR("\n"));
        return write;
    }

    // Write byte array. Return the number of bytes that completed write or negative value for error.
    int write(mbfs_file_type type, uint8_t *buf, size_t len)
    {
        int write = 0;
#if defined(MBFS_FLASH_FS)
        if (type == mbfs_flash && mb_flashFs)
            write = mb_flashFs.write(buf, len);
#endif
#if defined(MBFS_SD_FS)

        if (type == mbfs_sd && mb_sdFs)
            write = mb_sdFs.write(buf, len);
#endif
        return write;
    }

    // Close file.
    void close(mbfs_file_type type)
    {

#if defined(MBFS_FLASH_FS)
        if (type == mbfs_flash && mb_flashFs && flash_opened)
        {
            mb_flashFs.close();
            flash_filename_crc = 0;
            flash_opened = false;
            flash_open_mode = mb_fs_open_mode_undefined;
        }
#endif

#if defined(MBFS_SD_FS)
        if (type == mbfs_sd && mb_sdFs && sd_opened)
        {
            mb_sdFs.close();
            sd_filename_crc = 0;
            sd_opened = false;
            sd_open_mode = mb_fs_open_mode_undefined;
        }
#endif
    }

    // Check file existence.
    bool existed(const MB_String &filename, mbfs_file_type type)
    {

        if (!checkStorageReady(type))
            return false;

#if defined(MBFS_FLASH_FS)
        if (type == mbfs_flash)
        {

// The workaround for ESP32 LittleFS when calling vfs_api.cpp open() issue.
// See https://github.com/espressif/arduino-esp32/issues/7615
#if defined(MB_FS_USE_POSIX_STAT)
            return mb_fs_ns::exists("/littlefs", filename.c_str());
#else
            return MBFS_FLASH_FS.exists(filename.c_str());
#endif
        }

#endif

#if defined(MBFS_SD_FS)
        if (type == mbfs_sd)
        {
#if defined(MBFS_ESP32_SDFAT_ENABLED) || defined(MBFS_SDFAT_ENABLED)
            MBFS_SD_FILE file;
            bool ret = file.open(filename.c_str(), O_RDONLY);
            file.close();
            return ret;
#else
            return MBFS_SD_FS.exists(filename.c_str());
#endif
        }
#endif

        return false;
    }

    // Seek to position in file.
    bool seek(mbfs_file_type type, int pos)
    {

#if defined(MBFS_FLASH_FS)
        if (type == mbfs_flash && mb_flashFs)
            return mb_flashFs.seek(pos);
#endif
#if defined(MBFS_SD_FS)
        if (type == mbfs_sd && mb_sdFs)
            return mb_sdFs.seek(pos);
#endif

        return false;
    }

    // Read byte. Return the 1 for completed read or negative value for error.
    int read(mbfs_file_type type)
    {
#if defined(MBFS_FLASH_FS)
        if (type == mbfs_flash && mb_flashFs)
            return mb_flashFs.read();
#endif
#if defined(MBFS_SD_FS)
        if (type == mbfs_sd && mb_sdFs)
            return mb_sdFs.read();
#endif
        return -1;
    }

    // Write byte. Return the 1 for completed write or negative value for error.
    int write(mbfs_file_type type, uint8_t v)
    {
#if defined(MBFS_FLASH_FS)
        if (type == mbfs_flash && mb_flashFs)
            return mb_flashFs.write(v);
#endif
#if defined(MBFS_SD_FS)
        if (type == mbfs_sd && mb_sdFs)
            return mb_sdFs.write(v);
#endif
        return -1;
    }

    bool remove(const MB_String &filename, mbfs_file_type type)
    {
        if (!checkStorageReady(type))
            return false;

#if defined(MBFS_FLASH_FS)
        if (type == mbfs_flash && !flashReady())
            return false;
#endif

#if defined(MBFS_SD_FS)
        if (type == mbfs_sd && !sdReady())
            return false;
#endif

        if (!existed(filename, type))
            return true;

#if defined(MBFS_FLASH_FS)
        if (type == mbfs_flash)
            return MBFS_FLASH_FS.remove(filename.c_str());
#endif
#if defined(MBFS_SD_FS)
        if (type == mbfs_sd)
        {
#if defined(MBFS_ESP32_SDFAT_ENABLED) || defined(MBFS_SDFAT_ENABLED)
            if (mb_sdFs.open(filename.c_str(), O_RDWR | O_CREAT | O_APPEND))
            {
                mb_sdFs.remove();
                mb_sdFs.close();
                return true;
            }
#else
            return MBFS_SD_FS.remove(filename.c_str());
#endif
        }

#endif
        return false;
    }

// Get the Flash file instance.
#if defined(MBFS_FLASH_FS)
    fs::File &getFlashFile()
    {
        return mb_flashFs;
    }
#endif

// Get the SD file instance.
#if defined(MBFS_SD_FS)
    MBFS_SD_FILE &getSDFile()
    {
        return mb_sdFs;
    }
#endif

    // Get name of opened file.
    const char *name(mbfs_file_type type)
    {
#if defined(MBFS_FLASH_FS)
        if (type == mbfs_flash && mb_flashFs)
            return flash_file.c_str();
#endif
#if defined(MBFS_SD_FS)
        if (type == mbfs_sd && mb_sdFs)
            return sd_file.c_str();
#endif

        return "";
    }

    // Calculate CRC16 of byte array.
    uint16_t calCRC(const char *buf)
    {
        uint8_t x;
        uint16_t crc = 0xFFFF;

        int length = (int)strlen(buf);

        while (length--)
        {
            x = crc >> 8 ^ *buf++;
            x ^= x >> 4;
            crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x << 5)) ^ ((uint16_t)x);
        }
        return crc;
    }

    // Free reserved memory at pointer.
    void delP(void *ptr)
    {
        void **p = (void **)ptr;
        if (*p)
        {
            free(*p);
            *p = 0;
        }
    }

    // Allocate memory
    void *newP(size_t len, bool clear = true)
    {
        void *p;
        size_t newLen = getReservedLen(len);
#if defined(BOARD_HAS_PSRAM) && defined(MB_STRING_USE_PSRAM)

        if (ESP.getPsramSize() > 0)
            p = (void *)ps_malloc(newLen);
        else
            p = (void *)malloc(newLen);

        if (!p)
            return NULL;

#else

#if defined(ESP8266_USE_EXTERNAL_HEAP)
        ESP.setExternalHeap();
#endif

        p = (void *)malloc(newLen);
        bool nn = p ? true : false;

#if defined(ESP8266_USE_EXTERNAL_HEAP)
        ESP.resetHeap();
#endif

        if (!nn)
            return NULL;

#endif
        if (clear)
            memset(p, 0, newLen);
        return p;
    }

    size_t getReservedLen(size_t len)
    {
        int blen = len + 1;

        int newlen = (blen / 4) * 4;

        if (newlen < blen)
            newlen += 4;

        return (size_t)newlen;
    }

    void createDirs(MB_String dirs, mbfs_file_type type)
    {
        if (!longNameSupported())
            return;

        MB_String dir;
        int count = 0;
        int lastPos = 0;
        for (size_t i = 0; i < dirs.length(); i++)
        {
            dir.append(1, dirs[i]);
            count++;
            if (dirs[i] == '/' && i > 0)
            {
                if (dir.length() > 0)
                {

                    lastPos = dir.length() - 1;

#if defined(MBFS_FLASH_FS)
                    if (type == mbfs_flash)
                        MBFS_FLASH_FS.mkdir(dir.substr(0, dir.length() - 1).c_str());
#endif

#if defined(MBFS_SD_FS)
                    if (type == mbfs_sd)
                        MBFS_SD_FS.mkdir(dir.substr(0, dir.length() - 1).c_str());
#endif
                }
                count = 0;
            }
        }

        if (count > 0)
        {
            if (dir.find('.', lastPos) == MB_String::npos)
            {
#if defined(MBFS_FLASH_FS)
                if (type == mbfs_flash)
                    MBFS_FLASH_FS.mkdir(dir.c_str());
#endif

#if defined(MBFS_SD_FS)
                if (type == mbfs_sd)
                    MBFS_SD_FS.mkdir(dir.c_str());
#endif
            }
        }

        dir.clear();
    }

    bool longNameSupported()
    {

#if defined(MBFS_SDFAT_ENABLED) || defined(MBFS_FLASH_FS)
        return true;
#endif

#if defined(MBFS_SD_FS) && (defined(ESP32) || defined(ESP8266) || defined(MB_ARDUINO_PICO))
        return true;
#endif

        return false;
    }

private:
    uint16_t flash_filename_crc = 0;
    uint16_t sd_filename_crc = 0;
    MB_String flash_file, sd_file;
    mb_fs_open_mode flash_open_mode = mb_fs_open_mode_undefined;
    mb_fs_open_mode sd_open_mode = mb_fs_open_mode_undefined;
    bool flash_opened = false;
    bool sd_opened = false;
    bool sd_rdy = false;
    bool flash_rdy = false;
    uint16_t loopCount = 0;

#if defined(MBFS_FLASH_FS)
    fs::File mb_flashFs;
#endif
#if defined(MBFS_SD_FS)
    MBFS_SD_FILE mb_sdFs;
#endif

    int openFile(const MB_String &filename, mb_fs_mem_storage_type type, mb_fs_open_mode mode)
    {

#if defined(MBFS_FLASH_FS)
        if (type == mbfs_flash)
            return openFlashFile(filename, mode);
#endif
#if defined(MBFS_SD_FS)
        if (type == mbfs_sd)
            return openSDFile(filename, mode);
#endif
        return MB_FS_ERROR_FILE_IO_ERROR;
    }

    int openSDFile(const MB_String &filename, mb_fs_open_mode mode)
    {
        int ret = MB_FS_ERROR_FILE_IO_ERROR;

#if defined(MBFS_SD_FS)

        if (mode == mb_fs_open_mode_read || mode == mb_fs_open_mode_write || mode == mb_fs_open_mode_append)
        {
            uint16_t crc = calCRC(filename.c_str());

            if (mode == sd_open_mode && flash_filename_crc == crc && sd_opened) // same sd file opened, leave it
                return MB_FS_ERROR_FILE_STILL_OPENED;

            if (sd_opened)
                close(mbfs_sd); // sd file opened, close it

            flash_filename_crc = crc;
        }

#if defined(MBFS_ESP32_SDFAT_ENABLED) || defined(MBFS_SDFAT_ENABLED)

        if (mode == mb_fs_open_mode_read)
        {
            if (mb_sdFs.open(filename.c_str(), O_RDONLY))
            {
                sd_file = filename;
                sd_opened = true;
                sd_open_mode = mode;
                ret = mb_sdFs.size();
            }
        }
        else if (mode == mb_fs_open_mode_write || mode == mb_fs_open_mode_append)
        {
            if (mode == mb_fs_open_mode_write)
                remove(filename, mb_fs_mem_storage_type_sd);

            createDirs(filename, mb_fs_mem_storage_type_sd);
            if (mb_sdFs.open(filename.c_str(), O_RDWR | O_CREAT | O_APPEND))
            {
                sd_file = filename;
                sd_opened = true;
                sd_open_mode = mode;
                ret = 0;
            }
        }

#else

        if (mode == mb_fs_open_mode_read)
        {
#if defined(ESP32) || defined(ESP8266)
            mb_sdFs = MBFS_SD_FS.open(filename.c_str(), FILE_READ);
#else
            mb_sdFs = MBFS_SD_FS.open(filename.c_str(), "r");
#endif
            if (mb_sdFs)
            {
                sd_file = filename;
                sd_opened = true;
                sd_open_mode = mode;
                ret = mb_sdFs.size();
            }
        }
        else if (mode == mb_fs_open_mode_write || mode == mb_fs_open_mode_append)
        {
            if (mode == mb_fs_open_mode_write)
                remove(filename, mb_fs_mem_storage_type_sd);

            createDirs(filename, mb_fs_mem_storage_type_sd);
#if defined(ESP32)
            if (mode == mb_fs_open_mode_write)
                mb_sdFs = MBFS_SD_FS.open(filename.c_str(), FILE_WRITE);
            else
                mb_sdFs = MBFS_SD_FS.open(filename.c_str(), FILE_APPEND);
#elif defined(ESP8266)
            mb_sdFs = MBFS_SD_FS.open(filename.c_str(), FILE_WRITE);
#else
            if (mode == mb_fs_open_mode_write)
                mb_sdFs = MBFS_SD_FS.open(filename.c_str(), "w");
            else
                mb_sdFs = MBFS_SD_FS.open(filename.c_str(), "a");
#endif

            if (mb_sdFs)
            {
                sd_file = filename;
                sd_opened = true;
                sd_open_mode = mode;
                ret = 0;
            }
        }
#endif

#endif
        return ret;
    }

    int openFlashFile(const MB_String &filename, mb_fs_open_mode mode)
    {
        int ret = MB_FS_ERROR_FILE_IO_ERROR;

#if defined(MBFS_FLASH_FS)

        if (mode == mb_fs_open_mode_read || mode == mb_fs_open_mode_write || mode == mb_fs_open_mode_append)
        {
            uint16_t crc = calCRC(filename.c_str());
            if (mode == flash_open_mode && sd_filename_crc == crc && flash_opened) // same flash file opened, leave it
                return MB_FS_ERROR_FILE_STILL_OPENED;

            if (flash_opened)
                close(mbfs_flash); // flash file opened, close it

            sd_filename_crc = crc;
        }

        if (mode == mb_fs_open_mode_read)
        {
            mb_flashFs = MBFS_FLASH_FS.open(filename.c_str(), "r");
            if (mb_flashFs)
            {
                flash_file = filename;
                flash_opened = true;
                flash_open_mode = mode;
                ret = mb_flashFs.size();
            }
        }
        else if (mode == mb_fs_open_mode_write || mode == mb_fs_open_mode_append)
        {
            if (mode == mb_fs_open_mode_write)
                remove(filename, mb_fs_mem_storage_type_flash);

            createDirs(filename, mb_fs_mem_storage_type_flash);
            if (mode == mb_fs_open_mode_write)
                mb_flashFs = MBFS_FLASH_FS.open(filename.c_str(), "w");
            else
                mb_flashFs = MBFS_FLASH_FS.open(filename.c_str(), "a");

            if (mb_flashFs)
            {
                flash_file = filename;
                flash_opened = true;
                flash_open_mode = mode;
                ret = 0;
            }
        }

#endif
        return ret;
    }
};

#endif
#endif