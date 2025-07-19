/**
 *
 * @file HTTPUpdate.cpp based om ESP8266HTTPUpdate.cpp
 * @date 16.10.2018
 * @author Markus Sattler
 *
 * Copyright (c) 2015 Markus Sattler. All rights reserved.
 * This file is part of the ESP32 Http Updater.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "HTTPUpdateLight.h"
#include <StreamString.h>

#include <esp_partition.h>
#include <esp_ota_ops.h>                // get running partition

// Tasmota Logging
extern void AddLog(uint32_t loglevel, PGM_P formatP, ...);
enum LoggingLevels {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE};

// To do extern "C" uint32_t _SPIFFS_start;
// To do extern "C" uint32_t _SPIFFS_end;

HTTPUpdateLight::HTTPUpdateLight(void)
        : _httpClientTimeout(8000), _ledPin(-1)
{
    _followRedirects = HTTPC_STRICT_FOLLOW_REDIRECTS;
}

HTTPUpdateLight::HTTPUpdateLight(int httpClientTimeout)
        : _httpClientTimeout(httpClientTimeout), _ledPin(-1)
{
    _followRedirects = HTTPC_STRICT_FOLLOW_REDIRECTS;
}

HTTPUpdateLight::~HTTPUpdateLight(void)
{
}

// HTTPUpdateResult HTTPUpdateLight::update(WiFiClient& client, const String& url, const String& currentVersion)
// {
//     HTTPClient http;
//     if(!http.begin(client, url))
//     {
//         return HTTP_UPDATE_FAILED;
//     }
//     return handleUpdate(http, currentVersion, false);
// }

// HTTPUpdateResult HTTPUpdateLight::updateSpiffs(HTTPClient& httpClient, const String& currentVersion)
// {
//     return handleUpdate(httpClient, currentVersion, true);
// }

// HTTPUpdateResult HTTPUpdateLight::updateSpiffs(WiFiClient& client, const String& url, const String& currentVersion)
// {
//     HTTPClient http;
//     if(!http.begin(client, url))
//     {
//         return HTTP_UPDATE_FAILED;
//     }
//     return handleUpdate(http, currentVersion, true);
// }

HTTPUpdateResult HTTPUpdateLight::update(HTTPClientLight& httpClient,
        const String& currentVersion)
{
    return handleUpdate(httpClient, currentVersion, false);
}

// HTTPUpdateResult HTTPUpdateLight::update(WiFiClient& client, const String& host, uint16_t port, const String& uri,
//         const String& currentVersion)
// {
//     HTTPClient http;
//     if(!http.begin(client, host, port, uri))
//     {
//         return HTTP_UPDATE_FAILED;
//     }
//     return handleUpdate(http, currentVersion, false);
// }

/**
 * return error code as int
 * @return int error code
 */
int HTTPUpdateLight::getLastError(void)
{
    return _lastError;
}

/**
 * return error code as String
 * @return String error
 */
String HTTPUpdateLight::getLastErrorString(void)
{

    if(_lastError == 0) {
        return String(); // no error
    }

    // error from Update class
    if(_lastError > 0) {
        StreamString error;
        TasUpdate.printError(error);
        error.trim(); // remove line ending
        return String("Update error: ") + error;
    }

    // error from http client
    if(_lastError > -100) {
        return String("HTTP error: ") + HTTPClientLight::errorToString(_lastError);
    }

    switch(_lastError) {
    case HTTP_UE_TOO_LESS_SPACE:
        return "Not Enough space";
    case HTTP_UE_SERVER_NOT_REPORT_SIZE:
        return "Server Did Not Report Size";
    case HTTP_UE_SERVER_FILE_NOT_FOUND:
        return "File Not Found (404)";
    case HTTP_UE_SERVER_FORBIDDEN:
        return "Forbidden (403)";
    case HTTP_UE_SERVER_WRONG_HTTP_CODE:
        return "Wrong HTTP Code";
    case HTTP_UE_SERVER_FAULTY_MD5:
        return "Wrong MD5";
    case HTTP_UE_BIN_VERIFY_HEADER_FAILED:
        return "Verify Bin Header Failed";
    case HTTP_UE_BIN_FOR_WRONG_FLASH:
        return "New Binary Does Not Fit Flash Size";
    case HTTP_UE_NO_PARTITION:
        return "Partition Could Not be Found";
    }

    return String();
}

extern String getSketchSHA256();
// String getSketchSHA256() {
//   const size_t HASH_LEN = 32; // SHA-256 digest length

//   uint8_t sha_256[HASH_LEN] = { 0 };

// // get sha256 digest for running partition
//   if(esp_partition_get_sha256(esp_ota_get_running_partition(), sha_256) == 0) {
//     char buffer[2 * HASH_LEN + 1];

//     for(size_t index = 0; index < HASH_LEN; index++) {
//       uint8_t nibble = (sha_256[index] & 0xf0) >> 4;
//       buffer[2 * index] = nibble < 10 ? char(nibble + '0') : char(nibble - 10 + 'A');

//       nibble = sha_256[index] & 0x0f;
//       buffer[2 * index + 1] = nibble < 10 ? char(nibble + '0') : char(nibble - 10 + 'A');
//     }

//     buffer[2 * HASH_LEN] = '\0';

//     return String(buffer);
//   } else {

//     return String();
//   }
// }

/**
 *
 * @param http HTTPClientLight *
 * @param currentVersion const char *
 * @return HTTPUpdateResult
 */
HTTPUpdateResult HTTPUpdateLight::handleUpdate(HTTPClientLight& http, const String& currentVersion, bool spiffs)
{

    HTTPUpdateResult ret = HTTP_UPDATE_FAILED;

    // use HTTP/1.0 for update since the update handler not support any transfer Encoding
    http.useHTTP10(true);
    http.setTimeout(_httpClientTimeout);
    http.setFollowRedirects(_followRedirects);
    http.setUserAgent("ESP32-http-Update");
    http.addHeader("Cache-Control", "no-cache");
    http.addHeader("x-ESP32-STA-MAC", WiFiHelper::macAddress());
    http.addHeader("x-ESP32-AP-MAC", WiFi.softAPmacAddress());
    http.addHeader("x-ESP32-free-space", String(ESP.getFreeSketchSpace()));
    http.addHeader("x-ESP32-sketch-size", String(ESP.getSketchSize()));
    String sketchMD5 = ESP.getSketchMD5();
    if(sketchMD5.length() != 0) {
        http.addHeader("x-ESP32-sketch-md5", sketchMD5);
    }
    // Add also a SHA256
    String sketchSHA256 = getSketchSHA256();
    if(sketchSHA256.length() != 0) {
      http.addHeader("x-ESP32-sketch-sha256", sketchSHA256);
    }
    http.addHeader("x-ESP32-chip-size", String(ESP.getFlashChipSize()));
    http.addHeader("x-ESP32-sdk-version", ESP.getSdkVersion());

    if(spiffs) {
        http.addHeader("x-ESP32-mode", "spiffs");
    } else {
        http.addHeader("x-ESP32-mode", "sketch");
    }

    if(currentVersion && currentVersion[0] != 0x00) {
        http.addHeader("x-ESP32-version", currentVersion);
    }

    const char * headerkeys[] = { "x-MD5" };
    size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);

    // track these headers
    http.collectHeaders(headerkeys, headerkeyssize);

    uint32_t http_connect_time = millis();

    int code = http.GET();      // 0 if ok or < 0 if error
    int len = http.getSize();   // -1 if no info or > 0 when Content-Length is set by server

    // Add specific logging for Tasmota
    if (len < 0) {              // -1 if no info or > 0 when Content-Length is set by server
      if (code <= -1000) {      // BearSSL error 46 transformed to -1046
        AddLog(LOG_LEVEL_INFO, "OTA: TLS connection error %d after %d ms", -code - 1000, millis() - http_connect_time);
      } else if (code == -1) {  // HTTPC_ERROR_CONNECTION_REFUSED
        AddLog(LOG_LEVEL_INFO, "OTA: Connection timeout after %d ms", millis() - http_connect_time);
      } else {
        AddLog(LOG_LEVEL_INFO, "OTA: Connection error %d after %d ms", code, millis() - http_connect_time);
      }
    } else {
      AddLog(LOG_LEVEL_DEBUG, PSTR("OTA: Connected in %d ms, stack low mark %d"),
        millis() - http_connect_time, uxTaskGetStackHighWaterMark(nullptr));
    }

    if(code <= 0) {
        // log_e("HTTP error: %s\n", http.errorToString(code).c_str());
        _lastError = code;
        http.end();
        return HTTP_UPDATE_FAILED;
    }


    log_d("Header read fin.\n");
    log_d("Server header:\n");
    log_d(" - code: %d\n", code);
    log_d(" - len: %d\n", len);

    if(http.hasHeader("x-MD5")) {
        log_d(" - MD5: %s\n", http.header("x-MD5").c_str());
    }

    log_d("ESP32 info:\n");
    log_d(" - free Space: %d\n", ESP.getFreeSketchSpace());
    log_d(" - current Sketch Size: %d\n", ESP.getSketchSize());

    if(currentVersion && currentVersion[0] != 0x00) {
        log_d(" - current version: %s\n", currentVersion.c_str() );
    }

    switch(code) {
    case HTTP_CODE_OK:  ///< OK (Start Update)
        if(len > 0) {
            bool startUpdate = true;
            if(spiffs) {
                const esp_partition_t* _partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_SPIFFS, NULL);
                if(!_partition){
                    _lastError = HTTP_UE_NO_PARTITION;
                    return HTTP_UPDATE_FAILED;
                }

                if(len > _partition->size) {
                    log_e("spiffsSize to low (%d) needed: %d\n", _partition->size, len);
                    startUpdate = false;
                }
            } else {
                int sketchFreeSpace = ESP.getFreeSketchSpace();
                if(!sketchFreeSpace){
                    _lastError = HTTP_UE_NO_PARTITION;
                    return HTTP_UPDATE_FAILED;
                }

                if(len > sketchFreeSpace) {
                    log_e("FreeSketchSpace to low (%d) needed: %d\n", sketchFreeSpace, len);
                    startUpdate = false;
                }
            }

            if(!startUpdate) {
                _lastError = HTTP_UE_TOO_LESS_SPACE;
                ret = HTTP_UPDATE_FAILED;
            } else {
                // Warn main app we're starting up...
                if (_cbStart) {
                    _cbStart();
                }

                WiFiClient * tcp = http.getStreamPtr();

// To do?                WiFiUDP::stopAll();
// To do?                WiFiClient::stopAllExcept(tcp);

                delay(100);

                int command;

                if(spiffs) {
                    command = U_SPIFFS;
                    log_d("runUpdate spiffs...\n");
                } else {
                    command = U_FLASH;
                    log_d("runUpdate flash...\n");
                }

                if(!spiffs) {
/* To do
                    uint8_t buf[4];
                    if(tcp->peekBytes(&buf[0], 4) != 4) {
                        log_e("peekBytes magic header failed\n");
                        _lastError = HTTP_UE_BIN_VERIFY_HEADER_FAILED;
                        http.end();
                        return HTTP_UPDATE_FAILED;
                    }
*/

                    // check for valid first magic byte
//                    if(buf[0] != 0xE9) {
                    if(tcp->peek() != 0xE9) {
                        log_e("Magic header does not start with 0xE9\n");
                        _lastError = HTTP_UE_BIN_VERIFY_HEADER_FAILED;
                        http.end();
                        return HTTP_UPDATE_FAILED;

                    }
/* To do
                    uint32_t bin_flash_size = ESP.magicFlashChipSize((buf[3] & 0xf0) >> 4);

                    // check if new bin fits to SPI flash
                    if(bin_flash_size > ESP.getFlashChipRealSize()) {
                        log_e("New binary does not fit SPI Flash size\n");
                        _lastError = HTTP_UE_BIN_FOR_WRONG_FLASH;
                        http.end();
                        return HTTP_UPDATE_FAILED;
                    }
*/
                }
                if(runUpdate(*tcp, len, http.header("x-MD5"), command)) {
                    ret = HTTP_UPDATE_OK;
                    log_d("Update ok\n");
                    http.end();
                    // Warn main app we're all done
                    if (_cbEnd) {
                        _cbEnd();
                    }

                    if(_rebootOnUpdate && !spiffs) {
                        ESP.restart();
                    }

                } else {
                    ret = HTTP_UPDATE_FAILED;
                    log_e("Update failed\n");
                }
            }
        } else {
            _lastError = HTTP_UE_SERVER_NOT_REPORT_SIZE;
            ret = HTTP_UPDATE_FAILED;
            log_e("Content-Length was 0 or wasn't set by Server?!\n");
        }
        break;
    case HTTP_CODE_NOT_MODIFIED:
        ///< Not Modified (No updates)
        ret = HTTP_UPDATE_NO_UPDATES;
        break;
    case HTTP_CODE_NOT_FOUND:
        _lastError = HTTP_UE_SERVER_FILE_NOT_FOUND;
        ret = HTTP_UPDATE_FAILED;
        break;
    case HTTP_CODE_FORBIDDEN:
        _lastError = HTTP_UE_SERVER_FORBIDDEN;
        ret = HTTP_UPDATE_FAILED;
        break;
    default:
        _lastError = HTTP_UE_SERVER_WRONG_HTTP_CODE;
        ret = HTTP_UPDATE_FAILED;
        AddLog(LOG_LEVEL_INFO, "OTA: unsupported HTTP return code %i", code);
        // log_e("HTTP Code is (%d)\n", code);
        break;
    }

    http.end();
    return ret;
}

/**
 * write Update to flash
 * @param in Stream&
 * @param size uint32_t
 * @param md5 String
 * @return true if Update ok
 */
bool HTTPUpdateLight::runUpdate(Stream& in, uint32_t size, String md5, int command)
{

    StreamString error;

    if (_cbProgress) {
        TasUpdate.onProgress(_cbProgress);
    }

// Start Tasmota Factory patch
//    if(!Update.begin(size, command, _ledPin, _ledOn)) {
    if(!TasUpdate.begin(size, command, _ledPin, _ledOn, NULL, _factory)) {
// End Tasmota Factory patch
        _lastError = TasUpdate.getError();
        TasUpdate.printError(error);
        error.trim(); // remove line ending
        log_e("TasUpdate.begin failed! (%s)\n", error.c_str());
        return false;
    }

    if (_cbProgress) {
        _cbProgress(0, size);
    }

    if(md5.length()) {
        if(!TasUpdate.setMD5(md5.c_str())) {
            _lastError = HTTP_UE_SERVER_FAULTY_MD5;
            log_e("TasUpdate.setMD5 failed! (%s)\n", md5.c_str());
            return false;
        }
    }

// To do: the SHA256 could be checked if the server sends it

    if(TasUpdate.writeStream(in) != size) {
        _lastError = TasUpdate.getError();
        TasUpdate.printError(error);
        error.trim(); // remove line ending
        log_e("TasUpdate.writeStream failed! (%s)\n", error.c_str());
        return false;
    }

    if (_cbProgress) {
        _cbProgress(size, size);
    }

    if(!TasUpdate.end()) {
        _lastError = TasUpdate.getError();
        TasUpdate.printError(error);
        error.trim(); // remove line ending
        log_e("TasUpdate.end failed! (%s)\n", error.c_str());
        return false;
    }

    return true;
}

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_HTTPUPDATE)
HTTPUpdateLight httpUpdateLight;
#endif
