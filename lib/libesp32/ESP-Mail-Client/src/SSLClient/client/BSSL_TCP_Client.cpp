/**
 * BSSL_TCP_Client v2.0.12 for Arduino devices.
 *
 * Created August 27, 2023
 *
 * The MIT License (MIT)
 * Copyright (c) 2023 K. Suwatchai (Mobizt)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
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

/*
  WiFiClientSecure.cpp - Client Secure class for ESP32
  Copyright (c) 2016 Hristo Gochkov  All right reserved.
  Additions Copyright (C) 2017 Evandro Luis Copercini.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef BSSL_TCP_CLIENT_CPP
#define BSSL_TCP_CLIENT_CPP

#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wvla"

#include <Arduino.h>
#include "../ESP_SSLClient_FS.h"
#include "../ESP_SSLClient_Const.h"
#if defined(USE_LIB_SSL_ENGINE) || defined(USE_EMBED_SSL_ENGINE)

#include "BSSL_TCP_Client.h"
// #include <lwip/sockets.h>
// #include <lwip/netdb.h>
// #include <errno.h>

#undef connect
#undef write
#undef read

BSSL_TCP_Client::BSSL_TCP_Client()
{
    setClient(nullptr);
    _ssl_client.setTimeout(120000);
    _use_insecure = false;
}

BSSL_TCP_Client::~BSSL_TCP_Client()
{
    stop();
    setClient(nullptr);
}

void BSSL_TCP_Client::setClient(Client *client, bool enableSSL)
{
    _basic_client = client;
    _ssl_client.setClient(_basic_client, enableSSL);
}

void BSSL_TCP_Client::setDebugLevel(int level)
{
    _ssl_client.setDebugLevel(level);
}

int BSSL_TCP_Client::connect(IPAddress ip, uint16_t port)
{
    return connect(ip, port, 0);
}

int BSSL_TCP_Client::connect(IPAddress ip, uint16_t port, int32_t timeout)
{
    _port = port;

    if (timeout > 0)
    {
        _timeout = timeout;
        if (_basic_client)
            _basic_client->setTimeout(_timeout);
        _ssl_client.setTimeout(_timeout);
    }

    return _ssl_client.connect(ip, port);
}

int BSSL_TCP_Client::connect(const char *host, uint16_t port)
{
    return connect(host, port, 0);
}

int BSSL_TCP_Client::connect(const char *host, uint16_t port, int32_t timeout)
{

    _host = host;
    _port = port;

    if (timeout > 0)
    {
        _timeout = timeout;
        if (_basic_client)
            _basic_client->setTimeout(_timeout);
        _ssl_client.setTimeout(_timeout);
    }

    return _ssl_client.connect(host, port);
}

uint8_t BSSL_TCP_Client::connected()
{
    return _ssl_client.connected();
}

void BSSL_TCP_Client::validate(const char *host, uint16_t port)
{
    _ssl_client.validate(host, port);
}

void BSSL_TCP_Client::validate(IPAddress ip, uint16_t port)
{
    _ssl_client.validate(ip, port);
}

int BSSL_TCP_Client::available()
{
    return _ssl_client.available();
}

int BSSL_TCP_Client::read()
{
    uint8_t data = -1;
    int res = read(&data, 1);
    if (res < 0)
        return res;
    return data;
}

int BSSL_TCP_Client::read(uint8_t *buf, size_t size)
{
    if (!_ssl_client.connected())
        return 0;
    return _ssl_client.read(buf, size);
}

int BSSL_TCP_Client::send(const char *data)
{
    return write((uint8_t *)data, strlen(data));
}

int BSSL_TCP_Client::print(const char *data)
{
    return send(data);
}

int BSSL_TCP_Client::print(const String &data)
{
    return print(data.c_str());
}

int BSSL_TCP_Client::print(int data)
{
    char buf[64];
    memset(buf, 0, 64);
    sprintf(buf, (const char *)FPSTR("%d"), data);
    int ret = send(buf);
    return ret;
}

int BSSL_TCP_Client::println(const char *data)
{
    int len = send(data);
    if (len < 0)
        return len;
    int sz = send((const char *)FPSTR("\r\n"));
    if (sz < 0)
        return sz;
    return len + sz;
}

int BSSL_TCP_Client::println(const String &data)
{
    return println(data.c_str());
}

int BSSL_TCP_Client::println(int data)
{
    char buf[64];
    memset(buf, 0, 64);
    sprintf(buf, (const char *)FPSTR("%d\r\n"), data);
    int ret = send(buf);
    return ret;
}

size_t BSSL_TCP_Client::write(const uint8_t *buf, size_t size)
{
    if (!_ssl_client.connected())
        return 0;
    return _ssl_client.write(buf, size);
}

size_t BSSL_TCP_Client::write(uint8_t data)
{
    return write(&data, 1);
}

size_t BSSL_TCP_Client::write_P(PGM_P buf, size_t size) { return _ssl_client.write_P(buf, size); }

size_t BSSL_TCP_Client::write(const char *buf) { return write((const uint8_t *)buf, strlen(buf)); }

size_t BSSL_TCP_Client::write(Stream &stream) { return _ssl_client.write(stream); }

int BSSL_TCP_Client::peek()
{
    return _ssl_client.peek();
}

void BSSL_TCP_Client::setInsecure()
{
    _ssl_client.setInsecure();
    _use_insecure = true;
}

void BSSL_TCP_Client::enableSSL(bool enable)
{
    _ssl_client.enableSSL(enable);
}

bool BSSL_TCP_Client::connectSSL()
{
    if (!_ssl_client.connectSSL(_host.c_str(), _port))
    {
        stop();
        return 0;
    }
    return 1;
}

bool BSSL_TCP_Client::connectSSL(const String host, uint16_t port) { return connectSSL(); }

void BSSL_TCP_Client::stop()
{
    _ssl_client.stop();
}

int BSSL_TCP_Client::setTimeout(uint32_t seconds)
{
    _timeout = seconds * 1000;
    _ssl_client.setTimeout(_timeout);
    return 1;
}

int BSSL_TCP_Client::getTimeout() { return _ssl_client.getTimeout() / 1000; }

void BSSL_TCP_Client::setHandshakeTimeout(unsigned long handshake_timeout)
{
    _handshake_timeout = handshake_timeout * 1000;
    _ssl_client.setHandshakeTimeout(_handshake_timeout);
}

void BSSL_TCP_Client::flush()
{
    if (!_basic_client)
        return;

    while (available() > 0)
        read();
}

void BSSL_TCP_Client::setBufferSizes(int recv, int xmit)
{
    _ssl_client.setBufferSizes(recv, xmit);
}

int BSSL_TCP_Client::availableForWrite() { return _ssl_client.availableForWrite(); };

void BSSL_TCP_Client::setSession(BearSSL_Session *session) { _ssl_client.setSession(session); };

void BSSL_TCP_Client::setKnownKey(const PublicKey *pk, unsigned usages)
{
    _ssl_client.setKnownKey(pk, usages);
}

bool BSSL_TCP_Client::setFingerprint(const uint8_t fingerprint[20])
{
    return _ssl_client.setFingerprint(fingerprint);
}

bool BSSL_TCP_Client::setFingerprint(const char *fpStr)
{
    return _ssl_client.setFingerprint(fpStr);
}

void BSSL_TCP_Client::allowSelfSignedCerts()
{
    _ssl_client.allowSelfSignedCerts();
}

void BSSL_TCP_Client::setTrustAnchors(const X509List *ta)
{
    _ssl_client.setTrustAnchors(ta);
}

void BSSL_TCP_Client::setX509Time(time_t now)
{
    _ssl_client.setX509Time(now);
}

void BSSL_TCP_Client::setClientRSACert(const X509List *cert, const PrivateKey *sk)
{
    _ssl_client.setClientRSACert(cert, sk);
}

void BSSL_TCP_Client::setClientECCert(const X509List *cert, const PrivateKey *sk, unsigned allowed_usages, unsigned cert_issuer_key_type)
{
    _ssl_client.setClientECCert(cert, sk, allowed_usages, cert_issuer_key_type);
}

int BSSL_TCP_Client::getMFLNStatus() { return _ssl_client.getMFLNStatus(); };

int BSSL_TCP_Client::getLastSSLError(char *dest, size_t len)
{
    return _ssl_client.getLastSSLError(dest, len);
}

#if defined(ESP_SSL_FS_SUPPORTED)
void BSSL_TCP_Client::setCertStore(CertStoreBase *certStore)
{
    _ssl_client.setCertStore(certStore);
}
#endif

bool BSSL_TCP_Client::setCiphers(const uint16_t *cipherAry, int cipherCount)
{
    return _ssl_client.setCiphers(cipherAry, cipherCount);
}

bool BSSL_TCP_Client::setCiphers(const std::vector<uint16_t> &list)
{
    return _ssl_client.setCiphers(list);
}

bool BSSL_TCP_Client::setCiphersLessSecure()
{
    return _ssl_client.setCiphersLessSecure();
}

bool BSSL_TCP_Client::setSSLVersion(uint32_t min, uint32_t max)
{
    return _ssl_client.setSSLVersion(min, max);
}

bool BSSL_TCP_Client::probeMaxFragmentLength(IPAddress ip, uint16_t port, uint16_t len) { return _ssl_client.probeMaxFragmentLength(ip, port, len); };

bool BSSL_TCP_Client::probeMaxFragmentLength(const char *hostname, uint16_t port, uint16_t len) { return _ssl_client.probeMaxFragmentLength(hostname, port, len); };

bool BSSL_TCP_Client::probeMaxFragmentLength(const String &host, uint16_t port, uint16_t len) { return _ssl_client.probeMaxFragmentLength(host, port, len); };

// peek buffer API is present
bool BSSL_TCP_Client::hasPeekBufferAPI() const { return true; }

// return number of byte accessible by peekBuffer()
size_t BSSL_TCP_Client::peekAvailable() { return _ssl_client.peekAvailable(); }

// return a pointer to available data buffer (size = peekAvailable())
// semantic forbids any kind of read() before calling peekConsume()
const char *BSSL_TCP_Client::peekBuffer() { return _ssl_client.peekBuffer(); }

// consume bytes after use (see peekBuffer)
void BSSL_TCP_Client::peekConsume(size_t consume) { return _ssl_client.peekConsume(consume); }

void BSSL_TCP_Client::setCACert(const char *rootCA)
{
    _ssl_client.setCACert(rootCA);
}

void BSSL_TCP_Client::setCertificate(const char *client_ca) { return _ssl_client.setCertificate(client_ca); }

void BSSL_TCP_Client::setPrivateKey(const char *private_key) { return _ssl_client.setPrivateKey(private_key); }

bool BSSL_TCP_Client::loadCACert(Stream &stream, size_t size)
{
    char *dest = mStreamLoad(stream, size);
    bool ret = false;
    if (dest)
    {
        setCACert(dest);
        ret = true;
    }
    return ret;
}

bool BSSL_TCP_Client::loadCertificate(Stream &stream, size_t size) { return _ssl_client.loadCertificate(stream, size); }

bool BSSL_TCP_Client::loadPrivateKey(Stream &stream, size_t size) { return _ssl_client.loadPrivateKey(stream, size); }

int BSSL_TCP_Client::connect(IPAddress ip, uint16_t port, const char *rootCABuff, const char *cli_cert, const char *cli_key) { return _ssl_client.connect(ip, port, rootCABuff, cli_cert, cli_key); }

int BSSL_TCP_Client::connect(const char *host, uint16_t port, const char *rootCABuff, const char *cli_cert, const char *cli_key) { return _ssl_client.connect(host, port, rootCABuff, cli_cert, cli_key); }

BSSL_TCP_Client &BSSL_TCP_Client::operator=(const BSSL_TCP_Client &other)
{
    stop();
    setClient(other._basic_client);
    _use_insecure = other._use_insecure;
    _timeout = other._timeout;
    _handshake_timeout = other._handshake_timeout;
    _ssl_client.setTimeout(_timeout);
    _ssl_client.setHandshakeTimeout(_handshake_timeout);
    if (_use_insecure)
        _ssl_client.setInsecure();
    return *this;
}

char *BSSL_TCP_Client::mStreamLoad(Stream &stream, size_t size)
{
    char *dest = (char *)malloc(size + 1);
    if (!dest)
    {
        return nullptr;
    }
    if (size != stream.readBytes(dest, size))
    {
        free(dest);
        dest = nullptr;
        return nullptr;
    }
    dest[size] = '\0';
    return dest;
}

bool BSSL_TCP_Client::operator==(const BSSL_TCP_Client &rhs)
{
    return _basic_client == rhs._basic_client && _port == rhs._port && _host == rhs._host;
}

#endif

#endif