/*
 * Customized version of ESP32 HTTPClient Library. 
 * Allow custom header and payload with STARTTLS support
 * 
 * v 1.0.0
 * 
 * The MIT License (MIT)
 * Copyright (c) 2019 K. Suwatchai (Mobizt)
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

#ifndef ESP32MailHTTPClient_CPP
#define ESP32MailHTTPClient_CPP

#ifdef ESP32

#include "ESP32MailHTTPClient.h"

class TransportTraits
{
public:
    virtual ~TransportTraits() {}

    virtual std::unique_ptr<WiFiClient> create()
    {
        return std::unique_ptr<WiFiClient>(new WiFiClient());
    }

    virtual bool
    verify(WiFiClient &client, const char *host, bool starttls, DebugMsgCallback cb)
    {
        return true;
    }
};

class TLSTraits : public TransportTraits
{
public:
    TLSTraits(const char *CAcert, const char *clicert = nullptr, const char *clikey = nullptr) : _cacert(CAcert), _clicert(clicert), _clikey(clikey) {}

    std::unique_ptr<WiFiClient> create() override
    {
        return std::unique_ptr<WiFiClient>(new WiFiClientSecureESP32());
    }

    bool verify(WiFiClient &client, const char *host, bool starttls, DebugMsgCallback cb) override
    {
        WiFiClientSecureESP32 &wcs = static_cast<WiFiClientSecureESP32 &>(client);
        wcs.setCACert(_cacert);
        wcs.setCertificate(_clicert);
        wcs.setPrivateKey(_clikey);
        wcs.setSTARTTLS(starttls);
        wcs.setDebugCB(cb);
        return true;
    }

protected:
    const char *_cacert;
    const char *_clicert;
    const char *_clikey;
};

ESP32MailHTTPClient::ESP32MailHTTPClient() {}

ESP32MailHTTPClient::~ESP32MailHTTPClient()
{
    if (_client)
        _client->stop();
}

bool ESP32MailHTTPClient::begin(const char *host, uint16_t port, const char *uri, const char *CAcert)
{
    transportTraits.reset(nullptr);

    _host = host;
    _port = port;
    _uri = uri;
    transportTraits = TransportTraitsPtr(new TLSTraits(CAcert));
    return true;
}

bool ESP32MailHTTPClient::connected()
{
    if (_client)
        return ((_client->available() > 0) || _client->connected());
    return false;
}

bool ESP32MailHTTPClient::sendHeader(const char *header)
{
    if (!connected())
        return false;
    return (_client->write(header, strlen(header)) == strlen(header));
}

int ESP32MailHTTPClient::sendRequest(const char *header, const char *payload)
{
    size_t size = strlen(payload);
    if (strlen(header) > 0)
    {
        if (!connect())
            return HTTPC_ERROR_CONNECTION_REFUSED;
        if (!sendHeader(header))
            return HTTPC_ERROR_SEND_HEADER_FAILED;
    }
    if (size > 0)
        if (_client->write(&payload[0], size) != size)
            return HTTPC_ERROR_SEND_PAYLOAD_FAILED;

    return 0;
}

WiFiClient *ESP32MailHTTPClient::getStreamPtr(void)
{
    if (connected())
        return _client.get();
    return nullptr;
}

bool ESP32MailHTTPClient::connect(void)
{
    if (connected())
    {
        while (_client->available() > 0)
            _client->read();
        return true;
    }

    if (!transportTraits)
        return false;

    _client = transportTraits->create();

    if (!transportTraits->verify(*_client, _host.c_str(), false, _debugCallback))
    {
        _client->stop();
        return false;
    }

    if (!_client->connect(_host.c_str(), _port))
        return false;

    return connected();
}

bool ESP32MailHTTPClient::connect(bool starttls)
{
    if (connected())
    {
        while (_client->available() > 0)
            _client->read();
        return true;
    }

    if (!transportTraits)
        return false;

    _client = transportTraits->create();

    if (!transportTraits->verify(*_client, _host.c_str(), starttls, _debugCallback))
    {
        _client->stop();
        return false;
    }

    if (!_client->connect(_host.c_str(), _port))
        return false;

    return connected();
}

void ESP32MailHTTPClient::setDebugCallback(DebugMsgCallback cb)
{
    _debugCallback = std::move(cb);
}

#endif //ESP32

#endif //ESP32MailHTTPClient_CPP
