/**
 *
 * The Network Upgradable Arduino Secure TCP Client Class, ESP_Mail_TCPClient.h v1.0.1
 *
 * Created August 27, 2023
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

#ifndef ESP_MAIL_TCPCLIENT_H
#define ESP_MAIL_TCPCLIENT_H

#include "ESP_Mail_Client_Version.h"
#if !VALID_VERSION_CHECK(30409)
#error "Mixed versions compilation."
#endif

#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wdelete-non-virtual-dtor"

#include "ESP_Mail_Const.h"

#if defined(ESP32) && (defined(ENABLE_SMTP) || defined(ENABLE_IMAP))
#include "IPAddress.h"
#include "lwip/sockets.h"
#endif

#if defined(ESP32)

#include "ESP8266WiFi.h"
#if defined(ESP_MAIL_WIFI_IS_AVAILABLE)
#define WIFI_HAS_HOST_BY_NAME
#endif
#include "extras/WiFiClientImpl.h"
#define BASE_WIFICLIENT WiFiClientImpl

#elif defined(ESP_MAIL_WIFI_IS_AVAILABLE)
#include "WiFiClient.h"
#define BASE_WIFICLIENT WiFiClient
#endif

#include "SSLClient/ESP_SSLClient.h"

class ESP_Mail_TCPClient
{
public:
    ESP_Mail_TCPClient()
    {
#if !defined(ESP_MAIL_DISABLE_SSL)
        _tcp_client = new ESP_SSLClient();
#endif
    }

    ~ESP_Mail_TCPClient()
    {
        clear();
#if !defined(ESP_MAIL_DISABLE_SSL)
        if (_tcp_client)
            delete _tcp_client;
        _tcp_client = nullptr;
#endif
    }

    /**
     * Set the client.
     * @param client The Client interface.
     */
    void setClient(Client *client)
    {
        clear();
        _basic_client = client;
        _client_type = esp_mail_client_type_external_basic_client;
    }

    /** Assign TinyGsm Clients.
     *
     * @param client The pointer to TinyGsmClient.
     * @param modem The pointer to TinyGsm modem object. Modem should be initialized and/or set mode before transfering data
     * @param pin The SIM pin.
     * @param apn The GPRS APN (Access Point Name).
     * @param user The GPRS user.
     * @param password The GPRS password.
     */
    void setGSMClient(Client *client, void *modem = nullptr, const char *pin = nullptr, const char *apn = nullptr, const char *user = nullptr, const char *password = nullptr)
    {
#if defined(ESP_MAIL_GSM_MODEM_IS_AVAILABLE)
        _pin = pin;
        _apn = apn;
        _user = user;
        _password = password;
        _modem = modem;
        _client_type = esp_mail_client_type_external_gsm_client;
        _basic_client = client;
#endif
    }

    /**
     * Set Root CA certificate to verify.
     * @param caCert The certificate.
     */
    void setCACert(const char *caCert)
    {
#if !defined(ESP_MAIL_DISABLE_SSL)
        if (caCert)
        {
            if (_x509)
                delete _x509;

            _x509 = new X509List(caCert);
            _tcp_client->setTrustAnchors(_x509);

            setCertType(esp_mail_cert_type_data);
            setTA(true);
        }
        else
        {
            setCertType(esp_mail_cert_type_none);
            setInSecure();
        }
#endif
    }

    /**
     * Set Root CA certificate to verify.
     * @param certFile The certificate file path.
     * @param storageType The storage type mb_fs_mem_storage_type_flash or mb_fs_mem_storage_type_sd.
     * @return true when certificate loaded successfully.
     */
    bool setCertFile(const char *certFile, mb_fs_mem_storage_type storageType)
    {
#if !defined(ESP_MAIL_DISABLE_SSL)
        if (!_mbfs)
            return false;

        if (_clock_ready && strlen(certFile) > 0)
        {
            MB_String filename = certFile;
            if (filename.length() > 0)
            {
                if (filename[0] != '/')
                    filename.prepend('/');
            }

            int len = _mbfs->open(filename, storageType, mb_fs_open_mode_read);
            if (len > -1)
            {
                uint8_t *der = (uint8_t *)_mbfs->newP(len);
                if (_mbfs->available(storageType))
                    _mbfs->read(storageType, der, len);
                _mbfs->close(storageType);

                if (_x509)
                    delete _x509;

                _x509 = new X509List(der, len);
                _tcp_client->setTrustAnchors(_x509);
                setTA(true);
                _mbfs->delP(&der);

                setCertType(esp_mail_cert_type_file);
            }
        }
#endif

        return getCertType() == esp_mail_cert_type_file;
    }

    /**
     * Set TCP connection time out in seconds.
     * @param timeoutSec The time out in seconds.
     */
    void setTimeout(uint32_t timeoutSec)
    {
        _tcp_client->setTimeout(timeoutSec * 1000);
    }

    /**  Set the BearSSL IO buffer size.
     *
     * @param rx The BearSSL receive buffer size in bytes.
     * @param tx The BearSSL trasmit buffer size in bytes.
     */
    void setIOBufferSize(int rx, int tx)
    {
        _rx_size = rx;
        _tx_size = tx;
    }

    /**
     * Get the ethernet link status.
     * @return true for link up or false for link down.
     */
    bool ethLinkUp()
    {
        bool ret = false;

#if !defined(ESP_MAIL_DISABLE_NATIVE_ETHERNET) && (defined(ENABLE_IMAP) || defined(ENABLE_SMTP))

#if defined(ESP_MAIL_ETH_IS_AVAILABLE)

#if defined(ESP32)
        if (validIP(ETH.localIP()))
        {
            ETH.linkUp();
            ret = true;
        }
#elif defined(ESP8266)

#if defined(ENABLE_SMTP) || defined(ENABLE_IMAP)
        if (!_session_config)
            return false;
#endif

#if defined(ESP8266) && defined(ESP8266_CORE_SDK_V3_X_X)

#if defined(INC_ENC28J60_LWIP)
        if (_session_config->spi_ethernet_module.enc28j60)
        {
            ret = _session_config->spi_ethernet_module.enc28j60->status() == WL_CONNECTED;
            goto ex;
        }
#endif
#if defined(INC_W5100_LWIP)
        if (_session_config->spi_ethernet_module.w5100)
        {
            ret = _session_config->spi_ethernet_module.w5100->status() == WL_CONNECTED;
            goto ex;
        }
#endif
#if defined(INC_W5500_LWIP)
        if (_session_config->spi_ethernet_module.w5500)
        {
            ret = _session_config->spi_ethernet_module.w5500->status() == WL_CONNECTED;
            goto ex;
        }
#endif
#elif defined(MB_ARDUINO_PICO)

#endif

        return ret;

#if defined(INC_ENC28J60_LWIP) || defined(INC_W5100_LWIP) || defined(INC_W5500_LWIP)
    ex:
#endif
        // workaround for ESP8266 Ethernet
        delayMicroseconds(0);
#endif

#endif

#endif

        return ret;
    }

    /**
     * Checking for valid IP.
     * @return true for valid.
     */
    bool validIP(IPAddress ip)
    {
        char buf[16];
        sprintf(buf, "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
        return strcmp(buf, "0.0.0.0") != 0;
    }

    /**
     * Ethernet DNS workaround.
     */
    void ethDNSWorkAround()
    {

#if !defined(ESP_MAIL_DISABLE_NATIVE_ETHERNET) && (defined(ENABLE_SMTP) || defined(ENABLE_IMAP))
        if (!_session_config)
            return;

#if defined(ESP8266_CORE_SDK_V3_X_X)

#if defined(INC_ENC28J60_LWIP)
        if (_session_config->spi_ethernet_module.enc28j60)
            goto ex;
#endif
#if defined(INC_W5100_LWIP)
        if (_session_config->spi_ethernet_module.w5100)
            goto ex;
#endif
#if defined(INC_W5500_LWIP)
        if (_session_config->spi_ethernet_module.w5500)
            goto ex;
#endif

#elif defined(MB_ARDUINO_PICO)

#endif

        return;

#if defined(INC_ENC28J60_LWIP) || defined(INC_W5100_LWIP) || defined(INC_W5500_LWIP)
    ex:
#if defined(ESP_MAIL_WIFI_IS_AVAILABLE)
        BASE_WIFICLIENT client;
        client.connect(_session_config->server.host_name.c_str(), _session_config->server.port);
        client.stop();
#endif
#endif

#endif
    }

    /**
     * Get the network status.
     * @return true for connected or false for not connected.
     */
    bool networkReady()
    {

        // We will not invoke the network status request when device has built-in WiFi or Ethernet and it is connected.
        if (_client_type == esp_mail_client_type_external_gsm_client)
        {
            _network_status = gprsConnected();
            if (!_network_status)
                gprsConnect();
        }
        else if (WiFI_CONNECTED || ethLinkUp())
            _network_status = true;
        else if (_client_type == esp_mail_client_type_external_basic_client)
        {
            if (!_network_status_cb)
                _last_error = 1;
            else
                _network_status_cb();
        }

        return _network_status;
    }

    /**
     * Reconnect the network.
     */
    void networkReconnect()
    {

        if (_client_type == esp_mail_client_type_external_basic_client)
        {
#if defined(ESP_MAIL_HAS_WIFI_DISCONNECT)
            // We can reconnect WiFi when device connected via built-in WiFi that supports reconnect
            if (WiFI_CONNECTED)
            {
                WiFi.reconnect();
                return;
            }

#endif

            if (_network_connection_cb)
                _network_connection_cb();
        }
        else if (_client_type == esp_mail_client_type_external_gsm_client)
        {
            gprsDisconnect();
            gprsConnect();
        }
        else if (_client_type == esp_mail_client_type_internal_basic_client)
        {

#if defined(ESP_MAIL_WIFI_IS_AVAILABLE)
#if defined(ESP32) || defined(ESP8266)
            WiFi.reconnect();
#else
            if (_wifi_multi && _wifi_multi->credentials.size())
                _wifi_multi->reconnect();
#endif
#endif
        }
    }

    /**
     * Disconnect the network.
     */
    void networkDisconnect() {}

    /**
     * Get the Client type.
     * @return The esp_mail_client_type enum value.
     */
    esp_mail_client_type type() { return _client_type; }

    /**
     * Get the Client initialization status.
     * @return The initialization status.
     */
    bool isInitialized()
    {
        bool rdy = true;
#if !defined(ESP_MAIL_WIFI_IS_AVAILABLE)
        if (_client_type == esp_mail_client_type_external_basic_client &&
            (!_network_connection_cb || !_network_status_cb))
            rdy = false;
        else if (_client_type != esp_mail_client_type_external_basic_client ||
                 _client_type != esp_mail_client_type_external_gsm_client)
            rdy = false;
#else
        // assume external client is WiFiClient and network status request callback is not required
        // when device was connected to network using on board WiFi
        if (_client_type == esp_mail_client_type_external_basic_client &&
            (!_network_connection_cb || (!_network_status_cb && !WiFI_CONNECTED && !ethLinkUp())))
        {
            rdy = false;
        }

#endif

        if (!rdy)
        {
#if !defined(SILENT_MODE) && (defined(ENABLE_SMTP) || defined(ENABLE_IMAP))
            if (_debug_level > 0)
            {
                if (!_network_connection_cb)
                    esp_mail_debug_print_tag(esp_mail_error_client_str_2 /* "network connection callback is required" */, esp_mail_debug_tag_type_error, true);
                if (!WiFI_CONNECTED && !ethLinkUp())
                {
                    if (!_network_status_cb)
                        esp_mail_debug_print_tag(esp_mail_error_client_str_3 /* "network connection status callback is required" */, esp_mail_debug_tag_type_error, true);
                }
            }
#endif
        }

        return rdy;
    }

    /**
     * Set Root CA certificate to verify.
     * @param name The host name.
     * @param ip The ip address result.
     * @return 1 for success or 0 for failed.
     */
    int hostByName(const char *name, IPAddress &ip)
    {
#if defined(ESP_MAIL_WIFI_IS_AVAILABLE)
        return WiFiHelper::hostByName(name, ip);
#else
        return 1;
#endif
    }

    /**
     * Store the host name and port.
     * @param host The host name to connect.
     * @param port The port to connect.
     * @return true.
     */
    bool begin(const char *host, uint16_t port)
    {
        _host = host;
        _port = port;
#if !defined(ESP_MAIL_DISABLE_SSL)
        _tcp_client->setBufferSizes(_rx_size >= _minRXTXBufSize && _rx_size <= _maxRXBufSize ? _rx_size : _maxRXBufSize / rxBufDivider,
                                    _tx_size >= _minRXTXBufSize && _tx_size <= _maxTXBufSize ? _tx_size : _maxTXBufSize / txBufDivider);
#endif
        _last_error = 0;
        return true;
    }

    /**
     * Start TCP connection using stored host name and port.
     * @param secure The secure mode option.
     * @param verify The Root CA certificate verification option.
     * @return true for success or false for error.
     */

    bool connect(bool secured, bool verify)
    {
        bool ret = false;

#if defined(ENABLE_SMTP) || defined(ENABLE_IMAP)
        _last_error = 0;

#if !defined(ESP_MAIL_DISABLE_SSL)
        _tcp_client->enableSSL(secured);
        setSecure(secured);
        setVerify(verify);
#endif

        if (connected())
        {
            flush();
            return true;
        }

        if (!_basic_client)
        {
            if (_client_type == esp_mail_client_type_external_basic_client)
            {
                _last_error = 1;
                return false;
            }
            else if (_client_type != esp_mail_client_type_external_gsm_client)
            {
// Device has no built-in WiFi, external client required.
#if defined(ESP_MAIL_WIFI_IS_AVAILABLE)
                _basic_client = new BASE_WIFICLIENT();
                _client_type = esp_mail_client_type_internal_basic_client;
#else
                _last_error = 1;
                return false;
#endif
            }
        }

#if defined(ESP_MAIL_DISABLE_SSL)
        _tcp_client = _basic_client;
#else
        _tcp_client->setClient(_basic_client);
#endif

        if (!_tcp_client->connect(_host.c_str(), _port))
            return false;

#if defined(ESP_MAIL_WIFI_IS_AVAILABLE) && (defined(ESP32) || defined(ESP8266) || defined(MB_ARDUINO_PICO))
        if (_client_type == esp_mail_client_type_internal_basic_client)
            reinterpret_cast<BASE_WIFICLIENT *>(_basic_client)->setNoDelay(true);
#endif

        // For TCP keepalive should work in ESP8266 core > 3.1.2.
        // https://github.com/esp8266/Arduino/pull/8940

        // Not currently supported by WiFiClientSecure in Arduino Pico core

        if (_client_type == esp_mail_client_type_internal_basic_client)
        {
            if (isKeepAliveSet())
            {
#if defined(ESP_MAIL_WIFI_IS_AVAILABLE)

#if defined(ESP8266)
                if (_tcpKeepIdleSeconds == 0 || _tcpKeepIntervalSeconds == 0 || _tcpKeepCount == 0)
                    reinterpret_cast<BASE_WIFICLIENT *>(_basic_client)->disableKeepAlive();
                else
                    reinterpret_cast<BASE_WIFICLIENT *>(_basic_client)->keepAlive(_tcpKeepIdleSeconds, _tcpKeepIntervalSeconds, _tcpKeepCount);

#elif defined(ESP32)

                if (_tcpKeepIdleSeconds == 0 || _tcpKeepIntervalSeconds == 0 || _tcpKeepCount == 0)
                {
                    _tcpKeepIdleSeconds = 0;
                    _tcpKeepIntervalSeconds = 0;
                    _tcpKeepCount = 0;
                }

                bool success = setOption(TCP_KEEPIDLE, &_tcpKeepIdleSeconds) > -1 &&
                               setOption(TCP_KEEPINTVL, &_tcpKeepIntervalSeconds) > -1 &&
                               setOption(TCP_KEEPCNT, &_tcpKeepCount) > -1;
                if (!success)
                    _isKeepAlive = false;
#endif

#endif
            }
        }

        ret = connected();
        if (!ret)
            stop();
#endif
        return ret;
    }

    /**
     * Upgrade the current connection by setting up the SSL and perform the SSL handshake.
     *
     * @param verify The Root CA certificate verification option
     * @return operating result.
     */

    bool connectSSL(bool verify)
    {
#if !defined(ESP_MAIL_DISABLE_SSL)
        _tcp_client->setDebugLevel(2);

        bool ret = _tcp_client->connected();

        if (ret)
        {
            setVerify(verify);
            ret = _tcp_client->connectSSL(_host.c_str(), _port);
            if (ret)
                _secured = true;
        }

        if (!ret)
            stop();

        return ret;
#endif
        return false;
    }

    /**
     * Stop TCP connection.
     */
    void stop()
    {
        if (_tcp_client)
            _tcp_client->stop();
    }

    /**
     * Get the TCP connection status.
     * @return true for connected or false for not connected.
     */
    bool connected() { return _tcp_client && _tcp_client->connected(); };

    /**
     * The TCP data write function.
     * @param data The data to write.
     * @param len The length of data to write.
     * @return The size of data that was successfully written or 0 for error.
     */
    int write(uint8_t *data, int len)
    {
        if (!_tcp_client)
            return TCP_CLIENT_ERROR_NOT_INITIALIZED;

        if (!data)
            return TCP_CLIENT_ERROR_SEND_DATA_FAILED;

        if (len == 0)
            return TCP_CLIENT_ERROR_SEND_DATA_FAILED;

        if (!networkReady())
            return TCP_CLIENT_ERROR_NOT_CONNECTED;

        if (!connect(isSecure(), isVerify()))
            return TCP_CLIENT_ERROR_CONNECTION_REFUSED;

        int toSend = _chunkSize;
        int sent = 0;
        while (sent < len)
        {
            if (sent + toSend > len)
                toSend = len - sent;

            if ((int)_tcp_client->write(data + sent, toSend) != toSend)
                return TCP_CLIENT_ERROR_SEND_DATA_FAILED;

            sent += toSend;
        }

        return len;
    }

    /**
     * The TCP data send function.
     * @param data The data to send.
     * @return The size of data that was successfully sent or 0 for error.
     */
    int send(const char *data) { return write((uint8_t *)data, strlen(data)); }

    /**
     * The TCP data print function.
     * @param data The data to print.
     * @return The size of data that was successfully print or 0 for error.
     */
    int print(const char *data) { return send(data); }

    /**
     * The TCP data print function.
     * @param data The data to print.
     * @return The size of data that was successfully print or 0 for error.
     */
    int print(int data)
    {
        char buf[64];
        memset(buf, 0, 64);
        sprintf(buf, (const char *)FPSTR("%d"), data);
        int ret = send(buf);
        return ret;
    }

    /**
     * The TCP data print with new line function.
     * @param data The data to print.
     * @return The size of data that was successfully print or 0 for error.
     */
    int println(const char *data)
    {
        int len = send(data);
        if (len < 0)
            return len;
        int sz = send((const char *)FPSTR("\r\n"));
        if (sz < 0)
            return sz;
        return len + sz;
    }

    /**
     * The TCP data print with new line function.
     * @param data The data to print.
     * @return The size of data that was successfully print or 0 for error.
     */
    int println(int data)
    {
        char buf[64];
        memset(buf, 0, 64);
        sprintf(buf, (const char *)FPSTR("%d\r\n"), data);
        int ret = send(buf);
        return ret;
    }

    /**
     * Get available data size to read.
     * @return The avaiable data size.
     */
    int available()
    {
        if (!_basic_client)
            return TCP_CLIENT_ERROR_NOT_INITIALIZED;

        return _tcp_client->available();
    }

    /**
     * The TCP data read function.
     * @return The read value or -1 for error.
     */
    int read()
    {
        if (!_basic_client)
            return TCP_CLIENT_ERROR_NOT_INITIALIZED;

        return _tcp_client->read();
    }

    /**
     * The TCP data read function.
     * @param buf The data buffer.
     * @param len The length of data that read.
     * @return The size of data that was successfully read or negative value for error.
     */
    int readBytes(uint8_t *buf, int len)
    {
        if (!_basic_client)
            return TCP_CLIENT_ERROR_NOT_INITIALIZED;

        return _tcp_client->read(buf, len);
    }

    /**
     * The TCP data read function.
     * @param buf The data buffer.
     * @param len The length of data that read.
     * @return The size of data that was successfully read or negative value for error.
     */
    int readBytes(char *buf, int len)
    {
        return readBytes((uint8_t *)buf, len);
    }

    /**
     * Wait for all receive buffer data read.
     */
    void flush()
    {
        if (_tcp_client)
            _tcp_client->flush();
    }

    /**
     * Set the network connection request callback.
     * @param networkConnectionCB The callback function that handles the network connection.
     */
    void networkConnectionRequestCallback(NetworkConnectionRequestCallback networkConnectionCB)
    {
        _network_connection_cb = networkConnectionCB;
    }

    /**
     * Set the network status request callback.
     * @param networkStatusCB The callback function that calls the setNetworkStatus function to set the network status.
     */
    void networkStatusRequestCallback(NetworkStatusRequestCallback networkStatusCB)
    {
        _network_status_cb = networkStatusCB;
    }

    /**
     * Set the network status which should call in side the networkStatusRequestCallback function.
     * @param status The status of network.
     */
    void setNetworkStatus(bool status)
    {
        _network_status = status;
    }

    void setMBFS(MB_FS *mbfs) { _mbfs = mbfs; }

#if defined(ENABLE_SMTP) || defined(ENABLE_IMAP)
    void setSession(Session_Config *session_config)
    {
        _session_config = session_config;
    }
#endif

    void setClockReady(bool rdy)
    {
        _clock_ready = rdy;
    }

    void setCertType(esp_mail_cert_type type) { _cert_type = type; }

    esp_mail_cert_type getCertType() { return _cert_type; }

    void setDebugLevel(int debug) { _debug_level = debug; }

    unsigned long tcpTimeout()
    {
        if (_tcp_client)
            return 1000 * _tcp_client->getTimeout();
        return 0;
    }

    void disconnect(){};

    void keepAlive(int tcpKeepIdleSeconds, int tcpKeepIntervalSeconds, int tcpKeepCount)
    {
        _tcpKeepIdleSeconds = tcpKeepIdleSeconds;
        _tcpKeepIntervalSeconds = tcpKeepIntervalSeconds;
        _tcpKeepCount = tcpKeepCount;
        _isKeepAlive = tcpKeepIdleSeconds > 0 && tcpKeepIntervalSeconds > 0 && tcpKeepCount > 0;
    }

    bool isKeepAliveSet() { return _tcpKeepIdleSeconds > -1 && _tcpKeepIntervalSeconds > -1 && _tcpKeepCount > -1; };

    bool isKeepAlive() { return _isKeepAlive; };

    void clear()
    {
#if !defined(ESP_MAIL_DISABLE_SSL)
        if (_basic_client && _client_type == esp_mail_client_type_internal_basic_client)
        {
#if defined(ESP_MAIL_WIFI_IS_AVAILABLE)
            delete (BASE_WIFICLIENT *)_basic_client;
#else
            delete _basic_client;
#endif
            _basic_client = nullptr;
        }
#else
        _basic_client = nullptr;
#endif
        _client_type = esp_mail_client_type_undefined;
    }

    void setWiFi(esp_mail_wifi_credentials_t *wifi) { _wifi_multi = wifi; }

    bool gprsConnect()
    {
#if defined(ESP_MAIL_GSM_MODEM_IS_AVAILABLE)
        TinyGsm *gsmModem = (TinyGsm *)_modem;
        if (gsmModem)
        {
            // Unlock your SIM card with a PIN if needed
            if (_pin.length() && gsmModem->getSimStatus() != 3)
                gsmModem->simUnlock(_pin.c_str());

#if defined(TINY_GSM_MODEM_XBEE)
            // The XBee must run the gprsConnect function BEFORE waiting for network!
            gsmModem->gprsConnect(_apn.c_str(), _user.c_str(), _password.c_str());
#endif

#if !defined(SILENT_MODE) && (defined(ENABLE_IMAP) || defined(ENABLE_SMTP))
            if (_debug_level > 0 && _last_error == 0)
                esp_mail_debug_print_tag((const char *)MBSTRING_FLASH_MCR("Waiting for network..."), esp_mail_debug_tag_type_info, false);
#endif
            if (!gsmModem->waitForNetwork())
            {
#if !defined(SILENT_MODE) && (defined(ENABLE_IMAP) || defined(ENABLE_SMTP))
                if (_debug_level > 0 && _last_error == 0)
                    esp_mail_debug_print_tag((const char *)MBSTRING_FLASH_MCR(" fail"), esp_mail_debug_tag_type_info, true, false);
#endif
                _last_error = 1;
                _network_status = false;
                return false;
            }

#if !defined(SILENT_MODE) && (defined(ENABLE_IMAP) || defined(ENABLE_SMTP))
            if (_debug_level > 0 && _last_error == 0)
                esp_mail_debug_print_tag((const char *)MBSTRING_FLASH_MCR(" success"), esp_mail_debug_tag_type_info, true, false);
#endif

            if (gsmModem->isNetworkConnected())
            {
#if !defined(SILENT_MODE) && (defined(ENABLE_IMAP) || defined(ENABLE_SMTP))
                if (_debug_level > 0 && _last_error == 0)
                {
                    esp_mail_debug_print_tag((const char *)MBSTRING_FLASH_MCR("Connecting to "), esp_mail_debug_tag_type_info, false);
                    esp_mail_debug_print_tag(_apn.c_str(), esp_mail_debug_tag_type_info, false, false);
                }
#endif
                _network_status = gsmModem->gprsConnect(_apn.c_str(), _user.c_str(), _password.c_str()) &&
                                  gsmModem->isGprsConnected();

#if !defined(SILENT_MODE) && (defined(ENABLE_IMAP) || defined(ENABLE_SMTP))
                if (_debug_level > 0 && _last_error == 0)
                {
                    if (_network_status)
                        esp_mail_debug_print_tag((const char *)MBSTRING_FLASH_MCR(" success"), esp_mail_debug_tag_type_info, true, false);
                    else
                        esp_mail_debug_print_tag((const char *)MBSTRING_FLASH_MCR(" fail"), esp_mail_debug_tag_type_info, true, false);
                }
#endif
            }

            if (!_network_status)
                _last_error = 1;

            return _network_status;
        }

#endif
        return false;
    }

    bool gprsConnected()
    {
#if defined(ESP_MAIL_GSM_MODEM_IS_AVAILABLE)
        TinyGsm *gsmModem = (TinyGsm *)_modem;
        _network_status = gsmModem && gsmModem->isGprsConnected();
#endif
        return _network_status;
    }

    bool gprsDisconnect()
    {
#if defined(ESP_MAIL_GSM_MODEM_IS_AVAILABLE)
        TinyGsm *gsmModem = (TinyGsm *)_modem;
        _network_status = gsmModem && gsmModem->gprsDisconnect();
#endif
        return !_network_status;
    }

    bool gprsGetTime(int &year, int &month, int &day, int &hour, int &min, int &sec, float &timezone)
    {
#if defined(ESP_MAIL_GSM_MODEM_IS_AVAILABLE) && defined(TINY_GSM_MODEM_HAS_TIME)

        if (!gprsConnected())
            return 0;

        TinyGsm *gsmModem = (TinyGsm *)_modem;
        year = 0;
        month = 0;
        day = 0;
        hour = 0;
        min = 0;
        sec = 0;
        timezone = 0;
        for (int8_t i = 5; i; i--)
        {
            if (gsmModem->getNetworkTime(&year, &month, &day, &hour, &min, &sec, &timezone))
            {
                return true;
            }
        }
#endif
        return false;
    }

    int setOption(int option, int *value)
    {
#if defined(ESP32) && defined(ESP_MAIL_WIFI_IS_AVAILABLE)
        // Actually we wish to use setSocketOption directly but it is ambiguous in old ESP32 core v1.0.x.;
        // Use setOption instead for old core support.
        return reinterpret_cast<BASE_WIFICLIENT *>(_basic_client)->setOption(option, value);
#endif
        return 0;
    }

    void setTA(bool hasTA)
    {
        _has_ta = hasTA;
    }

    void setSecure(bool secure)
    {
        _secured = secure;
    }

    void setInSecure()
    {
#if !defined(ESP_MAIL_DISABLE_SSL)
        _use_insecure = true;
        setTA(false);
#endif
    }

    void setVerify(bool verify)
    {
#if !defined(ESP_MAIL_DISABLE_SSL)
        if (_has_ta)
            _use_insecure = !verify;

        if (_use_insecure)
            _tcp_client->setInsecure();
#endif
    }

    bool isSecure()
    {
        return _secured;
    }

    bool isVerify()
    {
        return !_use_insecure;
    }

    int rxBufDivider = 16;
    int txBufDivider = 32;

private:
    // lwIP TCP Keepalive idle in seconds.
    int _tcpKeepIdleSeconds = -1;
    // lwIP TCP Keepalive interval in seconds.
    int _tcpKeepIntervalSeconds = -1;
    // lwIP TCP Keepalive count.
    int _tcpKeepCount = -1;
    bool _isKeepAlive = false;

    uint16_t _bsslRxSize = 1024;
    uint16_t _bsslTxSize = 1024;
    const int _maxRXBufSize = 16384; // SSL full supported 16 kB
    const int _maxTXBufSize = 16384;
    const int _minRXTXBufSize = 512;
#if defined(ESP_MAIL_DISABLE_SSL)
    Client *_tcp_client = nullptr;
#else
    ESP_SSLClient *_tcp_client = nullptr;
    X509List *_x509 = nullptr;
#endif

    MB_String _host;
    uint16_t _port = 443;

    MB_FS *_mbfs = nullptr;
    Client *_basic_client = nullptr;
    esp_mail_wifi_credentials_t *_wifi_multi = nullptr;
#if defined(ENABLE_SMTP) || defined(ENABLE_IMAP)
    Session_Config *_session_config = nullptr;
#endif
    NetworkConnectionRequestCallback _network_connection_cb = NULL;
    NetworkStatusRequestCallback _network_status_cb = NULL;
#if defined(ESP_MAIL_HAS_WIFIMULTI)
    WiFiMulti *_multi = nullptr;
#endif
#if defined(ESP_MAIL_GSM_MODEM_IS_AVAILABLE)
    MB_String _pin, _apn, _user, _password;
    void *_modem = nullptr;
#endif

    bool _has_ta = false;
    bool _secured = false;
    bool _use_insecure = false;
    int _debug_level = 0;
    int _chunkSize = 1024;
    bool _clock_ready = false;
    int _last_error = 0;
    volatile bool _network_status = false;
    int _rx_size = -1, _tx_size = -1;

    esp_mail_cert_type _cert_type = esp_mail_cert_type_undefined;
    esp_mail_client_type _client_type = esp_mail_client_type_undefined;
};

#endif