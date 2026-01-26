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

#ifndef MBFS_CLASS_H
#define MBFS_CLASS_H

#include <Arduino.h>
#include <FS.h>
#include "MB_FS_Interfaces.h"
#include MB_STRING_INCLUDE_CLASS


#define MB_FS_ERROR_FILE_IO_ERROR -300
#define MB_FS_ERROR_FILE_NOT_FOUND -301
#define MB_FS_ERROR_FLASH_STORAGE_IS_NOT_READY -302
#define MB_FS_ERROR_SD_STORAGE_IS_NOT_READY -303
#define MB_FS_ERROR_FILE_STILL_OPENED -304


#define mbfs_file_type mb_fs_mem_storage_type
#define mbfs_flash mb_fs_mem_storage_type_flash
#define mbfs_sd mb_fs_mem_storage_type_sd
#define mbfs_undefined mb_fs_mem_storage_type_undefined

#define mbfs_type (mbfs_file_type)

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

extern FS *ufsp;

class MB_FS {

public:
    MB_FS() {}
    ~MB_FS() {}


    bool sdBegin(int ss = -1, int sck = -1, int miso = -1, int mosi = -1, uint32_t frequency = 4000000) {
        mfsp = ufsp;
        return true;
    }

    bool sdSPIBegin(int ss, SPIClass *spiConfig, uint32_t frequency) {
        mfsp = ufsp;
        return true;
    }

    // Check the mounting status of Flash storage.
    bool flashReady() {
        return true;
    }

    // Check the mounting status of SD storage.
    bool sdReady() {
        return true;
    }

    // Check the mounting status of Flash or SD storage with mb_fs_mem_storage_type.
    bool checkStorageReady(mbfs_file_type type) {
        return true;
    }

    int open(const MB_String &filename, mbfs_file_type type, mb_fs_open_mode mode) {
        char fmode[8];
        switch (mode) {
            case mb_fs_open_mode_read:
                strcpy(fmode, "r");
                break;
            case mb_fs_open_mode_write:
                strcpy(fmode, "w");
                break;
            case mb_fs_open_mode_append:
                strcpy(fmode, "a");
                break;
        }
        flash_file = filename;
        mfsp = ufsp;
        mb_File = mfsp->open(filename.c_str(), fmode);
        //Serial.printf(">>> 1 %s, %s, %d ", filename.c_str(), fmode, mb_File);
        return mb_File;
    }

    bool ready(mbfs_file_type type) {
        return true;
    }

    int size(mbfs_file_type type) {
        return mb_File.size();
    }

    // Check if file is ready to read/write.
    int available(mbfs_file_type type) {
        return mb_File.available();
    }

    // Read byte array. Return the number of bytes that completed read or negative value for error.
    int read(mbfs_file_type type, uint8_t *buf, size_t len) {
        return mb_File.read(buf, len);
    }

    // Print char array. Return the number of bytes that completed write or negative value for error.
    int print(mbfs_file_type type, const char *str) {
        return mb_File.print(str);
    }

    // Print char array with new line. Return the number of bytes that completed write or negative value for error.
    int println(mbfs_file_type type, const char *str) {
        return mb_File.print(str);
    }

    // Print integer. Return the number of bytes that completed write or negative value for error.
    int print(mbfs_file_type type, int v) {
        return mb_File.print(v);
    }

    // Print integer with newline. Return the number of bytes that completed write or negative value for error.
    int println(mbfs_file_type type, int v) {
        return mb_File.print(v);
    }

    int print(mbfs_file_type type, unsigned int v) {
        return mb_File.print(v);
    }

    // Print integer with newline. Return the number of bytes that completed write or negative value for error.
    int println(mbfs_file_type type, unsigned int v) {
        return mb_File.print(v);
    }

    // Write byte array. Return the number of bytes that completed write or negative value for error.
    int write(mbfs_file_type type, uint8_t *buf, size_t len) {
        return mb_File.write(buf, len);
    }

    // Close file.
    void close(mbfs_file_type type) {
        mb_File.close();
    }

    // Check file existence.
    bool existed(const MB_String &filename, mbfs_file_type type) {
        return mfsp->exists(filename.c_str());
    }

    // Seek to position in file.
    bool seek(mbfs_file_type type, int pos) {
        return mb_File.seek(pos);
    }

    // Read byte. Return the 1 for completed read or negative value for error.
    int read(mbfs_file_type type) {
        return mb_File.read();
    }

    // Write byte. Return the 1 for completed write or negative value for error.
    int write(mbfs_file_type type, uint8_t v) {
        return mb_File.write(v);
    }

    bool remove(const MB_String &filename, mbfs_file_type type) {
        return mfsp->remove(filename.c_str());
    }

    fs::File &getFlashFile() {
        return mb_File;
    }

    // Get name of opened file.
    const char *name(mbfs_file_type type) {
        return flash_file.c_str();
    }

    // Calculate CRC16 of byte array.
    uint16_t calCRC(const char *buf) {
        uint8_t x;
        uint16_t crc = 0xFFFF;

        int length = (int)strlen(buf);

        while (length--) {
            x = crc >> 8 ^ *buf++;
            x ^= x >> 4;
            crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x << 5)) ^ ((uint16_t)x);
        }
        return crc;
    }

    // Free reserved memory at pointer.
    void delP(void *ptr) {
        void **p = (void **)ptr;
        if (*p) {
            free(*p);
            *p = 0;
        }
    }

    // Allocate memory
    void *newP(size_t len, bool clear = true) {
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

    size_t getReservedLen(size_t len) {
        int blen = len + 1;

        int newlen = (blen / 4) * 4;

        if (newlen < blen)
            newlen += 4;

        return (size_t)newlen;
    }

    void createDirs(MB_String dirs, mbfs_file_type type) {
        if (!longNameSupported())
            return;

        MB_String dir;
        int count = 0;
        int lastPos = 0;
        for (size_t i = 0; i < dirs.length(); i++) {
            dir.append(1, dirs[i]);
            count++;
            if (dirs[i] == '/' && i > 0) {
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

    bool longNameSupported() {

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
    MB_String flash_file, sd_file;
    FS *mfsp;
    File mb_File;

};

#endif
