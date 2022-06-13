/**
 *
 * @file HTTPUpdate.h based on ESP8266HTTPUpdate.h
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

#ifndef ___HTTP_UPDATE_LIGHT_H___
#define ___HTTP_UPDATE_LIGHT_H___

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <HttpClientLight.h>
#include <TasUpdate.h>
#include <HTTPUpdate.h>

/// note we use HTTP client errors too so we start at 100
// #define HTTP_UE_TOO_LESS_SPACE              (-100)
// #define HTTP_UE_SERVER_NOT_REPORT_SIZE      (-101)
// #define HTTP_UE_SERVER_FILE_NOT_FOUND       (-102)
// #define HTTP_UE_SERVER_FORBIDDEN            (-103)
// #define HTTP_UE_SERVER_WRONG_HTTP_CODE      (-104)
// #define HTTP_UE_SERVER_FAULTY_MD5           (-105)
// #define HTTP_UE_BIN_VERIFY_HEADER_FAILED    (-106)
// #define HTTP_UE_BIN_FOR_WRONG_FLASH         (-107)
// #define HTTP_UE_NO_PARTITION                (-108)

// enum HTTPUpdateResult {
//     HTTP_UPDATE_FAILED,
//     HTTP_UPDATE_NO_UPDATES,
//     HTTP_UPDATE_OK
// };

// typedef HTTPUpdateResult t_httpUpdate_return; // backward compatibility

// using HTTPUpdateStartCB = std::function<void()>;
// using HTTPUpdateEndCB = std::function<void()>;
// using HTTPUpdateErrorCB = std::function<void(int)>;
// using HTTPUpdateProgressCB = std::function<void(int, int)>;

class HTTPUpdateLight
{
public:
    HTTPUpdateLight(void);
    HTTPUpdateLight(int httpClientTimeout);
    ~HTTPUpdateLight(void);

    void rebootOnUpdate(bool reboot)
    {
        _rebootOnUpdate = reboot;
    }

    /**
      * set redirect follow mode. See `followRedirects_t` enum for avaliable modes.
      * @param follow
      */
    void setFollowRedirects(followRedirects_t follow)
    {
        _followRedirects = follow;
    }

    void setLedPin(int ledPin = -1, uint8_t ledOn = HIGH)
    {
        _ledPin = ledPin;
        _ledOn = ledOn;
    }

// Start Tasmota Factory patch
    void setFactory(bool factory = false)
    {
        _factory = factory;
    }
// End Tasmota Factory patch

    // t_httpUpdate_return update(WiFiClient& client, const String& url, const String& currentVersion = "");

    // t_httpUpdate_return update(WiFiClient& client, const String& host, uint16_t port, const String& uri = "/",
    //                            const String& currentVersion = "");

    // t_httpUpdate_return updateSpiffs(WiFiClient& client, const String& url, const String& currentVersion = "");

    t_httpUpdate_return update(HTTPClientLight& httpClient,
                               const String& currentVersion = "");

    // t_httpUpdate_return updateSpiffs(HTTPClient &httpClient, const String &currentVersion = "");

    // Notification callbacks
    void onStart(HTTPUpdateStartCB cbOnStart)          { _cbStart = cbOnStart; }
    void onEnd(HTTPUpdateEndCB cbOnEnd)                { _cbEnd = cbOnEnd; }
    void onError(HTTPUpdateErrorCB cbOnError)          { _cbError = cbOnError; }
    void onProgress(HTTPUpdateProgressCB cbOnProgress) { _cbProgress = cbOnProgress; }

    int getLastError(void);
    String getLastErrorString(void);

protected:
    t_httpUpdate_return handleUpdate(HTTPClientLight& http, const String& currentVersion, bool spiffs = false);
    bool runUpdate(Stream& in, uint32_t size, String md5, int command = U_FLASH);

    // Set the error and potentially use a CB to notify the application
    void _setLastError(int err) {
        _lastError = err;
        if (_cbError) {
            _cbError(err);
        }
    }
    int _lastError;
    bool _rebootOnUpdate = true;
private:
    int _httpClientTimeout;
    followRedirects_t _followRedirects;

    // Callbacks
    HTTPUpdateStartCB    _cbStart;
    HTTPUpdateEndCB      _cbEnd;
    HTTPUpdateErrorCB    _cbError;
    HTTPUpdateProgressCB _cbProgress;

    int _ledPin;
    uint8_t _ledOn;
// Start Tasmota Factory patch
    bool _factory;
// End Tasmota Factory patch
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_HTTPUPDATE)
extern HTTPUpdateLight httpUpdateLight;
#endif

#endif /* ___HTTP_UPDATE_LIGHT_H___ */
