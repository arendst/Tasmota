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

#ifndef ESP_Mail_HTTPClient32_CPP
#define ESP_Mail_HTTPClient32_CPP

#ifdef ESP32

#include "ESP_Mail_HTTPClient32.h"

ESP_Mail_HTTPClient32::ESP_Mail_HTTPClient32()
{
}

ESP_Mail_HTTPClient32::~ESP_Mail_HTTPClient32()
{
    if (_wcs)
    {
        _wcs->stop();
        _wcs.reset(nullptr);
        _wcs.release();
    }
    std::string().swap(_host);
    std::string().swap(_caCertFile);
}

bool ESP_Mail_HTTPClient32::begin(const char *host, uint16_t port)
{
    _host = host;
    _port = port;
    return true;
}

bool ESP_Mail_HTTPClient32::connected()
{
    if (_wcs)
    {
        if (_secured)
            return _wcs->connected();
        else
            return _wcs->_ns_connected();
    }
    return false;
}

bool ESP_Mail_HTTPClient32::send(const char *header)
{
    if (!connected())
        return false;
    if (_secured)
        return (_wcs->write(header, strlen(header)) == strlen(header));
    else
        return (_wcs->_ns_write(header, strlen(header)) == strlen(header));
}

int ESP_Mail_HTTPClient32::send(const char *header, const char *payload)
{
    size_t size = strlen(payload);
    if (strlen(header) > 0)
    {
        if (!connect(_secured))
        {
            return ESP_MAIL_ERROR_HTTPC_ERROR_CONNECTION_REFUSED;
        }

        if (!send(header))
        {
            return ESP_MAIL_ERROR_HTTPC_ERROR_SEND_HEADER_FAILED;
        }
    }

    if (size > 0)
    {
        if (_secured)
        {
            if (_wcs->write(&payload[0], size) != size)
                return ESP_MAIL_ERROR_HTTPC_ERROR_SEND_PAYLOAD_FAILED;
        }
        else
        {
            if (_wcs->_ns_write(&payload[0], size) != size)
                return ESP_MAIL_ERROR_HTTPC_ERROR_SEND_PAYLOAD_FAILED;
        }
    }

    return 0;
}

WiFiClient *ESP_Mail_HTTPClient32::stream(void)
{
    if (connected())
        return _wcs.get();
    return nullptr;
}
ESP_Mail_WCS32 *ESP_Mail_HTTPClient32::_stream(void)
{
    if (connected())
        return _wcs.get();
    return nullptr;
}

size_t ESP_Mail_HTTPClient32::_ns_print(const char *buf)
{
    size_t size = strlen(buf);
    return _wcs->_ns_write(&buf[0], size);
}

size_t ESP_Mail_HTTPClient32::_ns_println(const char *buf)
{
    size_t size = strlen(buf);
    size_t wr = _wcs->_ns_write((const char *)&buf[0], size);
    std::string s = "\r\n";
    wr += _wcs->_ns_write(s.c_str(), s.length());
    return wr;
}

bool ESP_Mail_HTTPClient32::connect(void)
{
    return connect(false);
}

bool ESP_Mail_HTTPClient32::connect(bool secured)
{
    _secured = secured;

    if (connected())
    {
        if (_secured)
        {
            while (_wcs->available() > 0)
                _wcs->read();
        }
        else
        {
            while (_wcs->_ns_available() > 0)
                _wcs->_ns_read();
        }
        return true;
    }

    if (_debugCallback)
        _wcs->setDebugCB(&_debugCallback);
    _wcs->setSTARTTLS(!secured);

    if (!_wcs->connect(_host.c_str(), _port))
        return false;
    return connected();
}

void ESP_Mail_HTTPClient32::setDebugCallback(DebugMsgCallback cb)
{
    _debugCallback = std::move(cb);
}

void ESP_Mail_HTTPClient32::setCACert(const char *caCert)
{
    _wcs->setCACert(caCert);
    if (caCert)
        _certType = 1;
    else
    {
        setInsecure();
        _certType = 0;
    }
    //_wcs->setNoDelay(true);
}

void ESP_Mail_HTTPClient32::setCertFile(const char *caCertFile, esp_mail_file_storage_type storageType)
{

    if (strlen(caCertFile) > 0)
    {
        File f;
        if (storageType == esp_mail_file_storage_type_flash)
        {
            ESP_MAIL_FLASH_FS.begin();
            if (ESP_MAIL_FLASH_FS.exists(caCertFile))
                f = ESP_MAIL_FLASH_FS.open(caCertFile, FILE_READ);
        }
        else if (storageType == esp_mail_file_storage_type_sd)
        {
            ESP_MAIL_SD_FS.begin();
            if (ESP_MAIL_SD_FS.exists(caCertFile))
                f = ESP_MAIL_SD_FS.open(caCertFile, FILE_READ);
        }

        if (f)
        {
            size_t len = f.size();
            _wcs->loadCACert(f, len);
            f.close();
        }
        _certType = 2;
    }
    //_wcs->setNoDelay(true);
}

void ESP_Mail_HTTPClient32::setInsecure()
{
    _wcs->setInsecure();
}

#endif //ESP32

#endif //ESP_Mail_HTTPClient32_CPP
