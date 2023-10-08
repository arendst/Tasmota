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
  WiFiClientSecure.h - Base class that provides Client SSL to ESP32
  Copyright (c) 2011 Adrian McEwen.  All right reserved.
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

#ifndef BSSL_TCP_CLIENT_H
#define BSSL_TCP_CLIENT_H

#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wvla"

#include <Arduino.h>
#include "../ESP_SSLClient_FS.h"
#include "../ESP_SSLClient_Const.h"
#if defined(USE_LIB_SSL_ENGINE) || defined(USE_EMBED_SSL_ENGINE)

#include "BSSL_SSL_Client.h"

#include <string>

class BSSL_TCP_Client : public Client
{

protected:
    bool _use_insecure;

public:
    BSSL_TCP_Client *next;

    // The default class constructor
    BSSL_TCP_Client();

    // The class deconstructor
    ~BSSL_TCP_Client();

    /**
     * Set the client.
     * @param client The pointer to Client interface.
     * @param enableSSL The ssl option; true for enable, false for disable.
     * 
     * Due to the client pointer is assigned, to avoid dangling pointer, 
     * client should be existed as long as it was used for transportation.
     */
    void setClient(Client *client, bool enableSSL = true);

    /**
     * Set debug level.
     * @param level The debug level or esp_ssl_client_debug_level.
     * esp_ssl_debug_none = 0
     * esp_ssl_debug_error = 1
     * esp_ssl_debug_warn = 2
     * esp_ssl_debug_info = 3
     * esp_ssl_debug_dump = 4
     */
    void setDebugLevel(int level);

    /**
     * Connect to server.
     * @param ip The server IP to connect.
     * @param port The server port to connecte.
     * @return 1 for success or 0 for error.
     */
    int connect(IPAddress ip, uint16_t port) override;

    /**
     * Connect to server.
     * @param ip The server IP to connect.
     * @param port The server port to connect.
     * @param timeout The connection time out in miiliseconds.
     * @return 1 for success or 0 for error.
     */
    int connect(IPAddress ip, uint16_t port, int32_t timeout);

    /**
     * Connect to server.
     * @param host The server host name.
     * @param port The server port to connect.
     * @return 1 for success or 0 for error.
     */
    int connect(const char *host, uint16_t port) override;

    /**
     * Connect to server.
     * @param host The server host name.
     * @param port The server port to connect.
     * @param timeout The connection time out in miiliseconds.
     * @return 1 for success or 0 for error.
     */
    int connect(const char *host, uint16_t port, int32_t timeout);

    /**
     * Get TCP connection status.
     * @return 1 for connected or 0 for not connected.
     */
    uint8_t connected() override;

    /**
     * Validate the last Client connection with these host and port.
     * @param host The server host name.
     * @param port The server port to connect.
     * The Client connection will be closed when the provided host or port is not match with that of last connection.
     */
    void validate(const char *host, uint16_t port);

    /**
     * Validate the last Client connection with these IP and port.
     * @param ip The server IP to connect.
     * @param port The server port to connect.
     * The Client connection will be closed when the provided IP or port is not match with that of last connection.
     */
    void validate(IPAddress ip, uint16_t port);

    /**
     * Get available data size to read.
     * @return The avaiable data size.
     * @note Get available data directly via lwIP for non-secure mode or via mbedTLS for secure mode.
     */
    int available() override;

    /**
     * The TCP data read function.
     * @return A byte data that was successfully read or -1 for error.
     * @note Get data directly via lwIP for non-secure mode or via mbedTLS to deccrypt data for secure mode.
     */
    int read() override;

    /**
     * The TCP data read function.
     * @param buf The data buffer.
     * @param size The length of data that read.
     * @return The size of data that was successfully read or 0 for error.
     * @note Get data directly via lwIP for non-secure mode or via mbedTLS to deccrypt data for secure mode.
     */
    int read(uint8_t *buf, size_t size) override;

    /**
     * The TCP data send function.
     * @param data The data to send.
     * @return The size of data that was successfully sent or 0 for error.
     */
    int send(const char *data);

    /**
     * The TCP data print function.
     * @param data The data to print.
     * @return The size of data that was successfully print or 0 for error.
     */
    int print(const char *data);

    /**
     * The TCP data print function.
     * @param data The data to print.
     * @return The size of data that was successfully print or 0 for error.
     */
    int print(const String &data);

    /**
     * The TCP data print function.
     * @param data The data to print.
     * @return The size of data that was successfully print or 0 for error.
     */
    int print(int data);

    /**
     * The TCP data print with new line function.
     * @param data The data to print.
     * @return The size of data that was successfully print or 0 for error.
     */
    int println(const char *data);

    /**
     * The TCP data print with new line function.
     * @param data The data to print.
     * @return The size of data that was successfully print or 0 for error.
     */
    int println(const String &data);

    /**
     * The TCP data print with new line function.
     * @param data The data to print.
     * @return The size of data that was successfully print or 0 for error.
     */
    int println(int data);

    /**
     * The TCP data write function.
     * @param buf The data to write.
     * @param size The length of data to write.
     * @return The size of data that was successfully written or 0 for error.
     * @note Send data directly via lwIP for non-secure mode or via mbedTLS to encrypt for secure mode.
     */
    size_t write(const uint8_t *buf, size_t size) override;

    /**
     * The TCP data write function.
     * @param data The byte of data to write.
     * @return The size of data that was successfully written (1) or 0 for error.
     * @note Send data directly via lwIP for non-secure mode or via mbedTLS to encrypt for secure mode.
     */
    size_t write(uint8_t data) override;

    /**
     * The TCP data write function.
     * @param buf The PGM data to write.
     * @param size The length of data to write.
     * @return The size of data that was successfully written or 0 for error.
     */
    size_t write_P(PGM_P buf, size_t size);

    /**
     * The TCP data write function.
     * @param buf The string data to write.
     * @return The size of data that was successfully written or 0 for error.
     */
    size_t write(const char *buf);

    /**
     * The TCP data write function.
     * @param stream The stream data to write.
     * @return The size of data that was successfully written or 0 for error.
     */
    size_t write(Stream &stream);

    /**
     * Read one byte from Stream with time out.
     * @return The byte of data that was successfully read or -1 for timed out.
     */
    int peek() override;

    /**
     * Disable certificate verification and ignore the authentication.
     */
    void setInsecure(); // Don't validate the chain, just accept whatever is given.  VERY INSECURE!

    /**
     * Enable/disable the SSL layer transport.
     * @param enable The enable option; true for enable, false to disable.
     */
    void enableSSL(bool enable);

    /**
     * Upgrade the current connection by setting up the SSL and perform the SSL handshake.
     *
     * @return operating result.
     */
    bool connectSSL();

    /**
     * Upgrade the current connection by setting up the SSL and perform the SSL handshake.
     * @param host The host to connect (unused).
     * @param port The port to connect (unused).
     * @return operating result.
     */
    bool connectSSL(const String host, uint16_t port);

    /**
     * Stop the TCP connection and release resources.
     */
    void stop() override;

    /**
     * Set the TCP timeout in seconds.
     * @param seconds The TCP timeout in seconds.
     */
    int setTimeout(uint32_t seconds);

    /**
     * Get the TCP timeout in seconds.
     * @return The TCP timeout in seconds.
     */
    int getTimeout();

    /**
     * Set the SSL handshake timeout in seconds.
     * @param handshake_timeout The SSL handshake timeout in seconds.
     */
    void setHandshakeTimeout(unsigned long handshake_timeout);

    /**
     * Wait for all receive buffer data read.
     */
    void flush() override;

    /**
     *  Sets the requested buffer size for transmit and receive
     *  @param recv The receive buffer size.
     *  @param xmit The transmit buffer size.
     */
    void setBufferSizes(int recv, int xmit);

    operator bool() { return connected(); }

    int availableForWrite();

    void setSession(BearSSL_Session *session);

    void setKnownKey(const PublicKey *pk, unsigned usages = BR_KEYTYPE_KEYX | BR_KEYTYPE_SIGN);

    /**
     * Verify certificate's SHA256 fingerprint.
     *
     * @param fingerprint The certificate's SHA256 fingerprint data to compare with server certificate's SHA256 fingerprint.
     * @return verification result.
     */
    bool setFingerprint(const uint8_t fingerprint[20]);

    bool setFingerprint(const char *fpStr);

    void allowSelfSignedCerts();

    void setTrustAnchors(const X509List *ta);

    void setX509Time(time_t now);

    void setClientRSACert(const X509List *cert, const PrivateKey *sk);

    void setClientECCert(const X509List *cert, const PrivateKey *sk, unsigned allowed_usages, unsigned cert_issuer_key_type);

    int getMFLNStatus();

    int getLastSSLError(char *dest = NULL, size_t len = 0);
#if defined(ESP_SSL_FS_SUPPORTED)
    void setCertStore(CertStoreBase *certStore);
#endif
    bool setCiphers(const uint16_t *cipherAry, int cipherCount);

    bool setCiphers(const std::vector<uint16_t> &list);

    bool setCiphersLessSecure();

    bool setSSLVersion(uint32_t min = BR_TLS10, uint32_t max = BR_TLS12);

    bool probeMaxFragmentLength(IPAddress ip, uint16_t port, uint16_t len);

    bool probeMaxFragmentLength(const char *hostname, uint16_t port, uint16_t len);

    bool probeMaxFragmentLength(const String &host, uint16_t port, uint16_t len);

    bool hasPeekBufferAPI() const;

    size_t peekAvailable();

    const char *peekBuffer();

    void peekConsume(size_t consume);

    /**
     * Set the Root CA or CA certificate.
     * @param rootCA The Root CA or CA certificate.
     */
    void setCACert(const char *rootCA);

    void setCertificate(const char *client_ca);

    void setPrivateKey(const char *private_key);

    /**
     * Read and set CA cert from file (Stream).
     * @param stream The Stream interface.
     * @param size The size of data to read.
     * @return The operating result.
     */
    bool loadCACert(Stream &stream, size_t size);

    bool loadCertificate(Stream &stream, size_t size);

    bool loadPrivateKey(Stream &stream, size_t size);

    int connect(IPAddress ip, uint16_t port, const char *rootCABuff, const char *cli_cert, const char *cli_key);

    int connect(const char *host, uint16_t port, const char *rootCABuff, const char *cli_cert, const char *cli_key);

    BSSL_TCP_Client &operator=(const BSSL_TCP_Client &other);

    bool operator==(const bool value) { return bool() == value; }

    bool operator!=(const bool value) { return bool() != value; }

    bool operator==(const BSSL_TCP_Client &);

    bool operator!=(const BSSL_TCP_Client &rhs) { return !this->operator==(rhs); };

private:
    String _host;
    uint16_t _port;
    BSSL_SSL_Client _ssl_client;
    Client *_basic_client = nullptr;
    unsigned long _timeout = 15000;
    unsigned long _handshake_timeout = 60000;

    char *mStreamLoad(Stream &stream, size_t size);
};

#endif

#endif