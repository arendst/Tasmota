/*
 *Customized WiFiClientSecure.cpp version 1.0.3
 * 
 * The MIT License (MIT)
 * Copyright (c) 2021 K. Suwatchai (Mobizt)
 * 
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

#ifndef ESP_Mail_WCS32_CPP
#define ESP_Mail_WCS32_CPP

#ifdef ESP32

#include "ESP_Mail_WCS32.h"
#include <lwip/sockets.h>
#include <lwip/netdb.h>
#include <errno.h>

#undef connect
#undef write
#undef read

ESP_Mail_WCS32::ESP_Mail_WCS32()
{
    _connected = false;

    sslclient = new esp_mail_ssl_ctx32;
    ssl_init(sslclient);
    sslclient->socket = -1;
    sslclient->handshake_timeout = 120000;
    _CA_cert = NULL;
    _cert = NULL;
    _private_key = NULL;
    _pskIdent = NULL;
    _psKey = NULL;
    next = NULL;
}

ESP_Mail_WCS32::ESP_Mail_WCS32(int sock)
{
    _connected = false;
    _timeout = 0;

    sslclient = new esp_mail_ssl_ctx32;
    ssl_init(sslclient);
    sslclient->socket = sock;
    sslclient->handshake_timeout = 120000;

    if (sock >= 0)
    {
        _connected = true;
    }

    _CA_cert = NULL;
    _cert = NULL;
    _private_key = NULL;
    _pskIdent = NULL;
    _psKey = NULL;
    next = NULL;
}

ESP_Mail_WCS32::ESP_Mail_WCS32(bool secured)
{
    _connected = false;
    sslclient = new esp_mail_ssl_ctx32;
    ssl_init(sslclient);
    sslclient->socket = -1;
    sslclient->handshake_timeout = 120000;
    _secured = secured;
    _CA_cert = NULL;
    _cert = NULL;
    _private_key = NULL;
    _pskIdent = NULL;
    _psKey = NULL;
    next = NULL;
}

ESP_Mail_WCS32::~ESP_Mail_WCS32()
{
    stop();
    delete sslclient;
}

ESP_Mail_WCS32 &ESP_Mail_WCS32::operator=(const ESP_Mail_WCS32 &other)
{
    stop();
    sslclient->socket = other.sslclient->socket;
    _connected = other._connected;
    return *this;
}

void ESP_Mail_WCS32::stop()
{
    if (sslclient->socket >= 0)
    {
        close(sslclient->socket);
        sslclient->socket = -1;
        _connected = false;
        _peek = -1;
    }
    stop_ssl_socket(sslclient, _CA_cert, _cert, _private_key);
}

int ESP_Mail_WCS32::connect(IPAddress ip, uint16_t port)
{
    if (_pskIdent && _psKey)
        return connect(ip, port, _pskIdent, _psKey);
    return connect(ip, port, _CA_cert, _cert, _private_key);
}

int ESP_Mail_WCS32::connect(IPAddress ip, uint16_t port, int32_t timeout)
{
    _timeout = timeout;
    return connect(ip, port);
}

int ESP_Mail_WCS32::connect(const char *host, uint16_t port)
{
    if (_pskIdent && _psKey)
        return connect(host, port, _pskIdent, _psKey);
    return connect(host, port, _CA_cert, _cert, _private_key);
}

int ESP_Mail_WCS32::connect(const char *host, uint16_t port, int32_t timeout)
{
    _timeout = timeout;
    return connect(host, port);
}

int ESP_Mail_WCS32::connect(IPAddress ip, uint16_t port, const char *_CA_cert, const char *_cert, const char *_private_key)
{
    return connect(ip.toString().c_str(), port, _CA_cert, _cert, _private_key);
}

int ESP_Mail_WCS32::connect(const char *host, uint16_t port, const char *_CA_cert, const char *_cert, const char *_private_key)
{
    _host = host;
    _port = port;
    _withCert = true;

    if (_timeout > 0)
    {
        sslclient->handshake_timeout = _timeout;
    }

    int ret = start_socket(sslclient, host, port, _timeout);
    _lastError = ret;
    if (ret < 0)
    {
        log_e("start_ssl_client: %d", ret);
        stop();
        return 0;
    }

    if (_secured)
    {
        ret = start_ssl_client(sslclient, host, port, _timeout, _CA_cert, _cert, _private_key, NULL, NULL);
        _lastError = ret;
        if (ret < 0)
        {
            log_e("start_ssl_client: %d", ret);
            stop();
            return 0;
        }
    }

    _connected = true;
    return 1;
}

int ESP_Mail_WCS32::connect(IPAddress ip, uint16_t port, const char *pskIdent, const char *psKey)
{
    return connect(ip.toString().c_str(), port, _pskIdent, _psKey);
}

int ESP_Mail_WCS32::connect(const char *host, uint16_t port, const char *pskIdent, const char *psKey)
{
    _host = host;
    _port = port;
    _withCert = true;

    log_v("start_ssl_client with PSK");
    if (_timeout > 0)
    {
        sslclient->handshake_timeout = _timeout;
    }
    int ret = start_socket(sslclient, host, port, _timeout);
    _lastError = ret;
    if (ret < 0)
    {
        log_e("start_ssl_client: %d", ret);
        stop();
        return 0;
    }

    if (_secured)
    {
        ret = start_ssl_client(sslclient, host, port, _timeout, NULL, NULL, NULL, _pskIdent, _psKey);
        _lastError = ret;
        if (ret < 0)
        {
            log_e("start_ssl_client: %d", ret);
            stop();
            return 0;
        }
    }
    _connected = true;
    return 1;
}

int ESP_Mail_WCS32::peek()
{
    if (_peek >= 0)
    {
        return _peek;
    }
    _peek = timedRead();
    return _peek;
}

size_t ESP_Mail_WCS32::write(uint8_t data)
{
    return write(&data, 1);
}

int ESP_Mail_WCS32::read()
{
    uint8_t data = -1;
    int res = read(&data, 1);
    if (res < 0)
    {
        return res;
    }
    return data;
}

size_t ESP_Mail_WCS32::write(const uint8_t *buf, size_t size)
{
    if (!_connected)
    {
        return 0;
    }
    int res = send_ssl_data(sslclient, buf, size);
    if (res < 0)
    {
        stop();
        res = 0;
    }
    return res;
}

int ESP_Mail_WCS32::read(uint8_t *buf, size_t size)
{
    int peeked = 0;
    int avail = available();
    if ((!buf && size) || avail <= 0)
    {
        return -1;
    }
    if (!size)
    {
        return 0;
    }
    if (_peek >= 0)
    {
        buf[0] = _peek;
        _peek = -1;
        size--;
        avail--;
        if (!size || !avail)
        {
            return 1;
        }
        buf++;
        peeked = 1;
    }

    int res = get_ssl_receive(sslclient, buf, size);
    if (res < 0)
    {
        stop();
        return peeked ? peeked : res;
    }
    return res + peeked;
}

int ESP_Mail_WCS32::available()
{
    int peeked = (_peek >= 0);
    if (!_connected)
    {
        return peeked;
    }
    int res = data_to_read(sslclient);
    if (res < 0)
    {
        stop();
        return peeked ? peeked : res;
    }
    return res + peeked;
}

uint8_t ESP_Mail_WCS32::connected()
{
    uint8_t dummy = 0;
    read(&dummy, 0);

    return _connected;
}

void ESP_Mail_WCS32::setCACert(const char *rootCA)
{
    _CA_cert = rootCA;
}

void ESP_Mail_WCS32::setCertificate(const char *client_ca)
{
    _cert = client_ca;
}

void ESP_Mail_WCS32::setPrivateKey(const char *private_key)
{
    _private_key = private_key;
}

void ESP_Mail_WCS32::setPreSharedKey(const char *pskIdent, const char *psKey)
{
    _pskIdent = pskIdent;
    _psKey = psKey;
}

bool ESP_Mail_WCS32::verify(const char *fp, const char *domain_name)
{
    if (!sslclient)
        return false;

    return verify_ssl_fingerprint(sslclient, fp, domain_name);
}

char *ESP_Mail_WCS32::_streamLoad(Stream &stream, size_t size)
{
    static char *dest = nullptr;
    if (dest)
    {
        free(dest);
    }
    dest = (char *)malloc(size);
    if (!dest)
    {
        return nullptr;
    }
    if (size != stream.readBytes(dest, size))
    {
        free(dest);
        dest = nullptr;
    }
    return dest;
}

bool ESP_Mail_WCS32::loadCACert(Stream &stream, size_t size)
{
    char *dest = _streamLoad(stream, size);
    bool ret = false;
    if (dest)
    {
        setCACert(dest);
        ret = true;
    }
    return ret;
}

bool ESP_Mail_WCS32::loadCertificate(Stream &stream, size_t size)
{
    char *dest = _streamLoad(stream, size);
    bool ret = false;
    if (dest)
    {
        setCertificate(dest);
        ret = true;
    }
    return ret;
}

bool ESP_Mail_WCS32::loadPrivateKey(Stream &stream, size_t size)
{
    char *dest = _streamLoad(stream, size);
    bool ret = false;
    if (dest)
    {
        setPrivateKey(dest);
        ret = true;
    }
    return ret;
}

int ESP_Mail_WCS32::lastError(char *buf, const size_t size)
{
    if (!_lastError)
    {
        return 0;
    }
    char error_buf[100];
    mbedtls_strerror(_lastError, error_buf, 100);
    snprintf(buf, size, "%s", error_buf);
    return _lastError;
}

void ESP_Mail_WCS32::setHandshakeTimeout(unsigned long handshake_timeout)
{
    sslclient->handshake_timeout = handshake_timeout * 1000;
}

void ESP_Mail_WCS32::setSTARTTLS(bool enable)
{
    _secured = !enable;
}

void ESP_Mail_WCS32::setDebugCB(DebugMsgCallback cb)
{
    sslclient->_debugCallback = std::move(cb);
}

int ESP_Mail_WCS32::_ns_available()
{
    if (sslclient->socket < 0)
        return false;

    if (_rxBuf.length() == 0)
    {
        int bufLen = 1024;
        char *tmp = new char[bufLen];
        memset(tmp, 0, bufLen);
        int ret = _ns_lwip_read(sslclient, tmp, bufLen);
        if (ret > 0)
            _rxBuf += tmp;
        delete[] tmp;
    }

    int result = _rxBuf.length();

    if (!result)
    {
        optimistic_yield(100);
    }
    return result;
}
size_t ESP_Mail_WCS32::_ns_write(const char *buf, size_t size)
{
    if (sslclient->socket < 0 || !size)
        return 0;
    return _ns_lwip_write(sslclient, buf, size);
}

size_t ESP_Mail_WCS32::_ns_read(char *buf, size_t size)
{
    if (_rxBuf.length() == 0)
        return _ns_lwip_read(sslclient, buf, size);
    else
    {
        size_t sz = size;
        if (sz > _rxBuf.length())
            sz = _rxBuf.length();
        strncpy(buf, _rxBuf.c_str(), sz);
        _rxBuf.erase(0, sz);
        return sz;
    }
}

int ESP_Mail_WCS32::_ns_read()
{
    int c = -1;
    if (_rxBuf.length() == 0)
    {
        char *buf = new char[2];
        memset(buf, 0, 2);
        int ret = _ns_lwip_read(sslclient, buf, 1);
        if (ret > 0)
            c = buf[0];
        delete[] buf;
    }
    else
    {
        c = _rxBuf.c_str()[0];
        _rxBuf.erase(0, 1);
    }

    return c;
}

uint8_t ESP_Mail_WCS32::_ns_connected()
{
    return sslclient->socket >= 0;
}

bool ESP_Mail_WCS32::_ns_connect_ssl()
{
    int ret = 0;
    if (_withKey)
        ret = start_ssl_client(sslclient, _host.c_str(), _port, _timeout, _CA_cert, _cert, _private_key, NULL, NULL);
    else if (_withCert)
        ret = start_ssl_client(sslclient, _host.c_str(), _port, _timeout, NULL, NULL, NULL, _pskIdent, _psKey);

    _lastError = ret;
    if (ret < 0)
    {
        log_e("start_ssl_client: %d", ret);
        stop();
        return 0;
    }

    return 1;
}

#endif //ESP32

#endif //WiFiClientSecureESP32_CPP