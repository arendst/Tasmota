/**
 * WiFiClientImpl v1.0.1
 *
 * This library provides the base client in replacement of ESP32 WiFiClient.
 *
 * The WiFiClient in ESP32 cannot be used in multithreading environment as in FreeRTOS task
 * which can (always) lead to the assetion error "pbuf_free: p->ref > 0".
 *
 * Created August 20, 2023
 *
 * The MIT License (MIT)
 * Copyright (c) 2022 K. Suwatchai (Mobizt)
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

#if !defined(WIFICLIENT_IMPL_H) && defined(ESP32)
#define WIFICLIENT_IMPL_H

#include <lwip/sockets.h>
class WiFiClientImpl : public Client
{
public:
    WiFiClientImpl(){};
    virtual ~WiFiClientImpl() { tcpClose(); };
    int connect(IPAddress ip, uint16_t port) { return tcpConnect(ip, port, _timeout); }
    int connect(IPAddress ip, uint16_t port, int32_t timeout_ms) { return tcpConnect(ip, port, timeout_ms); }
    int connect(const char *host, uint16_t port)
    {
        IPAddress address((uint32_t)0);
#if defined(WIFI_HAS_HOST_BY_NAME)
        if (!WiFiGenericClass::hostByName(host, address))
            return -1;
#endif
        return tcpConnect(address, port, _timeout);
    }
    int connect(const char *host, uint16_t port, int32_t timeout_ms)
    {
        _timeout = timeout_ms;
        return connect(host, port);
    }
    size_t write(uint8_t data) { return write(&data, 1); }
    size_t write(const uint8_t *buf, size_t size) { return tcpWrite(buf, size); }
    size_t write_P(PGM_P buf, size_t size) { return write(buf, size); }
    size_t write(Stream &stream)
    {
        uint8_t *buf = (uint8_t *)malloc(1360);
        if (!buf)
        {
            return 0;
        }
        size_t toRead = 0, toWrite = 0, written = 0;
        size_t available = stream.available();
        while (available)
        {
            toRead = (available > 1360) ? 1360 : available;
            toWrite = stream.readBytes(buf, toRead);
            written += write(buf, toWrite);
            available = stream.available();
        }
        free(buf);
        buf = nullptr;
        return written;
    }
    int available() { return tcpAavailable(); }
    int read()
    {
        uint8_t data = 0;
        int res = read(&data, 1);
        if (res < 0)
        {
            return res;
        }
        if (res == 0)
        { //  No data available.
            return -1;
        }
        return data;
    }
    int read(uint8_t *buf, size_t size) { return tcpRead(buf, size); }
    int peek() { return tcpPeek(); }
    void flush()
    {
        if (r_available())
            fillRxBuffer();
        _fillPos = _fillSize;
    }

    void stop() { tcpClose(); }
    uint8_t connected() { return tcpConnected(); }

    operator bool()
    {
        return connected();
    }
    WiFiClientImpl &operator=(const WiFiClientImpl &other);
    bool operator==(const bool value)
    {
        return bool() == value;
    }
    bool operator!=(const bool value)
    {
        return bool() != value;
    }
    bool operator==(const WiFiClientImpl &);
    bool operator!=(const WiFiClientImpl &rhs)
    {
        return !this->operator==(rhs);
    };

    virtual int fd() const { return _socket; }

    int setSocketOption(int option, char *value, size_t len)
    {
        return setSocketOption(SOL_SOCKET, option, (const void *)value, len);
    }
    int setSocketOption(int level, int option, const void *value, size_t len)
    {
        int res = setsockopt(_socket, level, option, value, len);
        if (res < 0)
        {
            log_e("fail on %d, errno: %d, \"%s\"", _socket, errno, strerror(errno));
        }
        return res;
    }
    int setOption(int option, int *value)
    {
        return setSocketOption(IPPROTO_TCP, option, (const void *)value, sizeof(int));
    }
    int getOption(int option, int *value)
    {
        socklen_t size = sizeof(int);
        int res = getsockopt(_socket, IPPROTO_TCP, option, (char *)value, &size);
        if (res < 0)
        {
            log_e("fail on fd %d, errno: %d, \"%s\"", _socket, errno, strerror(errno));
        }
        return res;
    }

    int setTimeout(uint32_t seconds)
    {
        Client::setTimeout(seconds * 1000); // This should be here?
        _timeout = seconds * 1000;
        if (_socket >= 0)
        {
            struct timeval tv;
            tv.tv_sec = seconds;
            tv.tv_usec = 0;
            if (setSocketOption(SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval)) < 0)
            {
                return -1;
            }
            return setSocketOption(SO_SNDTIMEO, (char *)&tv, sizeof(struct timeval));
        }
        else
        {
            return 0;
        }
    }

    int setNoDelay(bool nodelay)
    {
        int flag = nodelay;
        return setOption(TCP_NODELAY, &flag);
    }

    bool getNoDelay()
    {
        int flag = 0;
        getOption(TCP_NODELAY, &flag);
        return flag;
    }

    IPAddress remoteIP() const
    {
        return remoteIP(_socket);
    }

    IPAddress remoteIP(int fd) const
    {
        struct sockaddr_storage addr;
        socklen_t len = sizeof addr;
        getpeername(fd, (struct sockaddr *)&addr, &len);
        struct sockaddr_in *s = (struct sockaddr_in *)&addr;
        return IPAddress((uint32_t)(s->sin_addr.s_addr));
    }

    uint16_t remotePort() const
    {
        return remotePort(_socket);
    }

    uint16_t remotePort(int fd) const
    {
        struct sockaddr_storage addr;
        socklen_t len = sizeof addr;
        getpeername(fd, (struct sockaddr *)&addr, &len);
        struct sockaddr_in *s = (struct sockaddr_in *)&addr;
        return ntohs(s->sin_port);
    }

    IPAddress localIP() const
    {
        return localIP(_socket);
    }

    IPAddress localIP(int fd) const
    {
        struct sockaddr_storage addr;
        socklen_t len = sizeof addr;
        getsockname(fd, (struct sockaddr *)&addr, &len);
        struct sockaddr_in *s = (struct sockaddr_in *)&addr;
        return IPAddress((uint32_t)(s->sin_addr.s_addr));
    }

    uint16_t localPort() const
    {
        return localPort(_socket);
    }

    uint16_t localPort(int fd) const
    {
        struct sockaddr_storage addr;
        socklen_t len = sizeof addr;
        getsockname(fd, (struct sockaddr *)&addr, &len);
        struct sockaddr_in *s = (struct sockaddr_in *)&addr;
        return ntohs(s->sin_port);
    }

    // friend class WiFiServer;
    using Print::write;

private:
    int _socket = -1;
    int _timeout = 30000;
    size_t _rxBuffSize = 2048;
    uint8_t *_rxBuff = nullptr;
    size_t _fillPos = 0;
    size_t _fillSize = 0;
    bool _failed = false;
    bool _connected = false;

    size_t r_available()
    {
        if (_socket < 0)
        {
            return 0;
        }
        int count;
#ifdef ESP_IDF_VERSION_MAJOR
        int res = lwip_ioctl(_socket, FIONREAD, &count);
#else
        int res = lwip_ioctl_r(_socket, FIONREAD, &count);
#endif
        if (res < 0)
        {
            _failed = true;
            return 0;
        }
        return count;
    }

    size_t fillRxBuffer()
    {
        if (!_rxBuff && !allocRxBuffer(_rxBuffSize))
            return 0;

        if (_fillSize && _fillPos == _fillSize)
        {
            _fillSize = 0;
            _fillPos = 0;
        }

        if (!_rxBuff || _rxBuffSize <= _fillSize || !r_available())
        {
            return 0;
        }
        int res = recv(_socket, _rxBuff + _fillSize, _rxBuffSize - _fillSize, MSG_DONTWAIT);
        if (res < 0)
        {
            if (errno != EWOULDBLOCK)
            {
                _failed = true;
            }
            return 0;
        }
        _fillSize += res;
        return res;
    }

    bool failed()
    {
        return _failed;
    }

    int tcpConnect(const IPAddress &ip, uint32_t port, int timeout)
    {
        int enable = 1;

        log_v("Starting socket");

        _socket = -1;

        _socket = lwip_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (_socket < 0)
        {
            return _socket;
        }

        struct sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = ip;
        serv_addr.sin_port = htons(port);

        if (timeout <= 0)
            timeout = 30000; // Milli seconds.

        fd_set fdset;
        struct timeval tv;
        FD_ZERO(&fdset);
        FD_SET(_socket, &fdset);
        tv.tv_sec = timeout / 1000;
        tv.tv_usec = (timeout % 1000) * 1000;

        int res = lwip_connect(_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

        if (res < 0 && errno != EINPROGRESS)
        {
            log_e("connect on fd %d, errno: %d, \"%s\"", _socket, errno, strerror(errno));
            tcpClose();
            return -1;
        }

        res = select(_socket + 1, nullptr, &fdset, nullptr, timeout < 0 ? nullptr : &tv);
        if (res < 0)
        {
            log_e("select on fd %d, errno: %d, \"%s\"", _socket, errno, strerror(errno));
            tcpClose();
            return -1;
        }
        else if (res == 0)
        {
            log_i("select returned due to timeout %d ms for fd %d", timeout, _socket);
            tcpClose();
            return -1;
        }
        else
        {
            int sockerr;
            socklen_t len = (socklen_t)sizeof(int);
            res = getsockopt(_socket, SOL_SOCKET, SO_ERROR, &sockerr, &len);

            if (res < 0)
            {
                log_e("getsockopt on fd %d, errno: %d, \"%s\"", _socket, errno, strerror(errno));
                tcpClose();
                return -1;
            }

            if (sockerr != 0)
            {
                log_e("socket error on fd %d, errno: %d, \"%s\"", _socket, sockerr, strerror(sockerr));
                tcpClose();
                return -1;
            }
        }

        lwip_setsockopt(_socket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
        lwip_setsockopt(_socket, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));

        lwip_setsockopt(_socket, IPPROTO_TCP, TCP_NODELAY, &enable, sizeof(enable));
        lwip_setsockopt(_socket, SOL_SOCKET, SO_KEEPALIVE, &enable, sizeof(enable));

        fcntl(_socket, F_SETFL, fcntl(_socket, F_GETFL, 0) | O_NONBLOCK);

        _connected = true;

        return 1;
    }

    void tcpClose()
    {
        lwip_close(_socket);
        _socket = -1;
        _connected = false;
        freeRxBuffer();
    }

    int tcpAavailable()
    {
        return _fillSize - _fillPos + r_available();
    }

    int tcpPeek()
    {
        if (!_rxBuff || _fillPos == _fillSize && !fillRxBuffer())
        {
            return -1;
        }
        return _rxBuff[_fillPos];
    }

    size_t tcpWrite(const uint8_t *buf, size_t size)
    {
        if (!tcpConnected() || !size)
            return 0;

        int res = 0;
        int retry = 10;
        int socketFileDescriptor = _socket;
        size_t totalBytesSent = 0;
        size_t bytesRemaining = size;

        while (retry)
        {
            // use select to make sure the socket is ready for writing
            fd_set set;
            struct timeval tv;
            FD_ZERO(&set);                      // empties the set
            FD_SET(socketFileDescriptor, &set); // adds FD to the set
            tv.tv_sec = 0;
            tv.tv_usec = 1000000;
            retry--;

            if (select(socketFileDescriptor + 1, NULL, &set, NULL, &tv) < 0)
            {
                return 0;
            }

            if (FD_ISSET(socketFileDescriptor, &set))
            {
                res = send(socketFileDescriptor, (void *)buf, bytesRemaining, MSG_DONTWAIT);
                if (res > 0)
                {
                    totalBytesSent += res;
                    if (totalBytesSent >= size)
                    {
                        // completed successfully
                        retry = 0;
                    }
                    else
                    {
                        buf += res;
                        bytesRemaining -= res;
                        retry = 10;
                    }
                }
                else if (res < 0)
                {
                    log_e("fail on fd %d, errno: %d, \"%s\"", _socket, errno, strerror(errno));
                    if (errno != EAGAIN)
                    {
                        // if resource was busy, can try again, otherwise give up
                        res = 0;
                        retry = 0;
                    }
                }
                else
                {
                    // Try again
                }
            }
        }
        return totalBytesSent;

        // return ssize_t or signed size_t for error
        return lwip_write(_socket, buf, size);
    }

    size_t tcpRead(uint8_t *dst, size_t len)
    {

        if (!dst || !len || (_fillPos == _fillSize && !fillRxBuffer()))
        {
            return _failed ? -1 : 0;
        }

        int remain = _fillSize - _fillPos;
        if (len <= remain || ((len - remain) <= (_rxBuffSize - _fillSize) && fillRxBuffer() >= (len - remain)))
        {
            if (len == 1)
            {
                *dst = _rxBuff[_fillPos];
            }
            else
            {
                memcpy(dst, _rxBuff + _fillPos, len);
            }
            _fillPos += len;
            return len;
        }

        size_t left = len;
        size_t toRead = remain;
        uint8_t *buf = dst;
        memcpy(buf, _rxBuff + _fillPos, toRead);
        _fillPos += toRead;
        left -= toRead;
        buf += toRead;
        while (left)
        {
            if (!fillRxBuffer())
            {
                return len - left;
            }
            remain = _fillSize - _fillPos;
            toRead = (remain > left) ? left : remain;
            memcpy(buf, _rxBuff + _fillPos, toRead);
            _fillPos += toRead;
            left -= toRead;
            buf += toRead;
        }
        return len;
    }

    int tcpConnected()
    {
        return _socket >= 0;
    }

    bool allocRxBuffer(size_t size)
    {
        if (_rxBuff)
            freeRxBuffer();

        _rxBuff = (uint8_t *)malloc(size);
        if (!_rxBuff)
        {

            log_e("Not enough memory to allocate buffer");
            _failed = true;
            return false;
        }

        return true;
    }

    void freeRxBuffer()
    {
        if (_rxBuff)
            free(_rxBuff);

        _rxBuff = nullptr;
    }
};

#endif /* WIFICLIENT_IMPL_H */
