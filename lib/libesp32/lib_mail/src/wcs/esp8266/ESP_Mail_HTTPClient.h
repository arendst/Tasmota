/*
 * HTTP Client for ESP8266 wrapper v1.0.3
 * 
 * The MIT License (MIT)
 * Copyright (c) 2021 K. Suwatchai (Mobizt)
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

#ifndef ESP_Mail_HTTPClient_H
#define ESP_Mail_HTTPClient_H

#ifdef ESP8266

//ARDUINO_ESP8266_GIT_VER
//2.6.2 0xbc204a9b
//2.6.1 0x482516e3
//2.6.0 0x643ec203
//2.5.2 0x8b899c12
//2.5.1 0xac02aff5
//2.5.0 0x951aeffa
//2.5.0-beta3 0x21db8fc9
//2.5.0-beta2 0x0fd86a07
//2.5.0-beta1 0x9c1e03a1
//2.4.2 0xbb28d4a3
//2.4.1 0x614f7c32
//2.4.0 0x4ceabea9
//2.4.0-rc2 0x0c897c37
//2.4.0-rc1 0xf6d232f1

#include <Arduino.h>
#include <core_version.h>
#include <time.h>
#include <string>

#include "SDK_Version_Common.h"

#ifndef ARDUINO_ESP8266_GIT_VER
#error Your ESP8266 Arduino Core SDK is outdated, please update. From Arduino IDE go to Boards Manager and search 'esp8266' then select the latest version.
#endif

#include <WiFiClient.h>

#if ARDUINO_ESP8266_GIT_VER != 0xf6d232f1 && ARDUINO_ESP8266_GIT_VER != 0x0c897c37 && ARDUINO_ESP8266_GIT_VER != 0x4ceabea9 && ARDUINO_ESP8266_GIT_VER != 0x614f7c32 && ARDUINO_ESP8266_GIT_VER != 0xbb28d4a3
#include "ESP_Mail_WCS.h"
#include "ESP_Mail_BearSSLHelpers.h"
#define ESP_MAIL_SSL_CLIENT ESP_Mail::ESP_Mail_WCS
#else
#error Please update the ESP8266 Arduino Core SDK to latest version.
#endif

#define FS_NO_GLOBALS
#include <FS.h>
#include <SD.h>
#include "ESP_Mail_FS.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#define ESP_MAIL_FLASH_FS ESP_Mail_DEFAULT_FLASH_FS
#define ESP_MAIL_SD_FS ESP_Mail_DEFAULT_SD_FS

#if __has_include(<WiFiEspAT.h>) || __has_include(<espduino.h>)
#error WiFi UART bridge was not supported.
#endif

#define ESP_MAIL_ERROR_HTTPC_ERROR_CONNECTION_REFUSED (-1)
#define ESP_MAIL_ERROR_HTTPC_ERROR_SEND_HEADER_FAILED (-2)
#define ESP_MAIL_ERROR_HTTPC_ERROR_SEND_PAYLOAD_FAILED (-3)
#define ESP_MAIL_DEFAULT_TCP_TIMEOUT_SEC 30

enum esp_mail_file_storage_type
{
  esp_mail_file_storage_type_none,
  esp_mail_file_storage_type_flash,
  esp_mail_file_storage_type_sd,
  esp_mail_file_storage_type_univ
};

class ESP_Mail_HTTPClient
{

public:
  ESP_Mail_HTTPClient();
  ~ESP_Mail_HTTPClient();

  bool begin(const char *host, uint16_t port);

  bool connected(void);

  int send(const char *header, const char *payload);

  bool send(const char *header);

  WiFiClient *stream(void);

  ESP_Mail::ESP_Mail_WCS *_stream(void);

  size_t _ns_print(const char *buf);

  size_t _ns_println(const char *buf);

  void setCACert(const char *caCert);
  void setCertFile(const char *caCertFile, esp_mail_file_storage_type storageType, uint8_t sdPin);
  bool connect(bool secured);

  int _certType = -1;
  std::string _caCertFile = "";
  esp_mail_file_storage_type _caCertFileStoreageType = esp_mail_file_storage_type::esp_mail_file_storage_type_none;
  uint16_t tcpTimeout = 40000;

  uint8_t _sdPin = 15;
  bool _clockReady = false;
  uint16_t _bsslRxSize = 1024;
  uint16_t _bsslTxSize = 1024;
  bool fragmentable = false;
  int chunkSize = 1024;
  int maxRXBufSize = 16384; //SSL full supported 16 kB
  int maxTXBufSize = 16384;
  bool mflnChecked = false;
  int rxBufDivider = maxRXBufSize / chunkSize;
  int txBufDivider = maxRXBufSize / chunkSize;
  ;

private:
  std::unique_ptr<ESP_MAIL_SSL_CLIENT> _wcs = std::unique_ptr<ESP_MAIL_SSL_CLIENT>(new ESP_MAIL_SSL_CLIENT());
  std::unique_ptr<char> _cacert;
  std::string _host = "";
  uint16_t _port = 0;
  bool _secured = true;
};

#endif /* ESP8266 */

#endif /* ESP_Mail_HTTPClient_H */