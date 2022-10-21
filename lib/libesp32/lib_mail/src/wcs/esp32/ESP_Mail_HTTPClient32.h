/*
 * Customized version of ESP32 HTTPClient Library. 
 * 
 * v 1.1.5
 * 
 * The MIT License (MIT)
 * Copyright (c) 2021 K. Suwatchai (Mobizt)
 * 
 * HTTPClient Arduino library for ESP32
 *
 * Copyright (c) 2015 Markus Sattler. All rights reserved.
 * This file is part of the HTTPClient for Arduino.
 * Port to ESP32 by Evandro Luis Copercini (2017),
 * changed fingerprints to CA verification.
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

#ifndef ESP_Mail_HTTPClient32_H
#define ESP_Mail_HTTPClient32_H

#ifdef ESP32

#include <Arduino.h>
#include <WiFiClient.h>
#include <FS.h>
//#include <SPIFFS.h>
#include <SD.h>
#include "ESP_Mail_FS.h"
#include "ESP_Mail_WCS32.h"

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

class ESP_Mail_HTTPClient32
{
public:
  ESP_Mail_HTTPClient32();
  ~ESP_Mail_HTTPClient32();

  /**
    * Initialization of new http connection.
    * \param host - Host name without protocols.
    * \param port - Server's port.
    * \return True as default.
    * If no certificate string provided, use (const char*)NULL to CAcert param 
    */
  bool begin(const char *host, uint16_t port);

  /**
    * Check the http connection status.
    * \return True if connected.
    */
  bool connected();

  /**
    * Establish http connection if header provided and send it, send payload if provided.
    * \param header - The header string (constant chars array).
    * \param payload - The payload string (constant chars array), optional.
    * \return http status code, Return zero if new http connection and header and/or payload sent 
    * with no error or no header and payload provided. If obly payload provided, no new http connection was established.
    */
  int send(const char *header, const char *payload);

  /**
    * Send extra header without making new http connection (if send has been called)
    * \param header - The header string (constant chars array).
    * \return True if header sending success.
    * Need to call send with header first. 
    */
  bool send(const char *header);

  /**
    * Get the WiFi client pointer.
    * \return WiFi client pointer.
    */
  WiFiClient *stream(void);

  /**
   * Set insecure mode
  */
  void setInsecure();

  ESP_Mail_WCS32 *_stream(void);
  size_t _ns_print(const char *buf);
  size_t _ns_println(const char *buf);

  int tcpTimeout = 40000;
  bool connect(void);
  bool connect(bool secured);
  void setCACert(const char *caCert);
  void setCertFile(const char *caCertFile, esp_mail_file_storage_type storageType);
  void setDebugCallback(DebugMsgCallback cb);
  bool _secured = true;

  int _certType = -1;
  std::string _caCertFile = "";
  esp_mail_file_storage_type _caCertFileStoreageType = esp_mail_file_storage_type::esp_mail_file_storage_type_none;

protected:
  DebugMsgCallback _debugCallback = NULL;
  std::unique_ptr<ESP_Mail_WCS32> _wcs = std::unique_ptr<ESP_Mail_WCS32>(new ESP_Mail_WCS32());

  std::string _host = "";
  uint16_t _port = 0;
};

#endif //ESP32

#endif //ESP_Mail_HTTPClient32_H
