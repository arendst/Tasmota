/**
 * BSSL_SSL_Client library v1.0.11 for Arduino devices.
 *
 * Created August 27, 2003
 *
 * This work contains codes based on WiFiClientSecure from Earle F. Philhower and SSLClient from OSU OPEnS Lab.
 *
 * Copyright (c) 2018 Earle F. Philhower, III
 *
 * Copyright 2019 OSU OPEnS Lab
 *
 * The MIT License (MIT)
 * Copyright (c) 2023 K. Suwatchai (Mobizt)
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

#ifndef BSSL_SSL_CLIENT_CPP
#define BSSL_SSL_CLIENT_CPP

#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wvla"

#include <Arduino.h>
#include "../ESP_SSLClient_FS.h"
#include "../ESP_SSLClient_Const.h"
#if defined(USE_LIB_SSL_ENGINE) || defined(USE_EMBED_SSL_ENGINE)

#include "BSSL_Helper.h"
#include "BSSL_SSL_Client.h"

#if defined(ESP8266) && defined(MMU_EXTERNAL_HEAP) && defined(ESP_SSLCLIENT_USE_PSRAM)
#include <umm_malloc/umm_malloc.h>
#include <umm_malloc/umm_heap_select.h>
#define ESP_SSLCLIENT_ESP8266_USE_EXTERNAL_HEAP
#endif

#if defined(USE_EMBED_SSL_ENGINE)
#include <list>
#include <errno.h>
#include <algorithm>
#include "StackThunk.h"
#include "lwip/opt.h"
#include "lwip/ip.h"
#include "lwip/tcp.h"
#include "lwip/inet.h"
#include "lwip/netif.h"

#if 1
#if !CORE_MOCK

// The BearSSL thunks in use for now
#define br_ssl_engine_recvapp_ack thunk_br_ssl_engine_recvapp_ack
#define br_ssl_engine_recvapp_buf thunk_br_ssl_engine_recvapp_buf
#define br_ssl_engine_recvrec_ack thunk_br_ssl_engine_recvrec_ack
#define br_ssl_engine_recvrec_buf thunk_br_ssl_engine_recvrec_buf
#define br_ssl_engine_sendapp_ack thunk_br_ssl_engine_sendapp_ack
#define br_ssl_engine_sendapp_buf thunk_br_ssl_engine_sendapp_buf
#define br_ssl_engine_sendrec_ack thunk_br_ssl_engine_sendrec_ack
#define br_ssl_engine_sendrec_buf thunk_br_ssl_engine_sendrec_buf

#endif
#endif

#endif

BSSL_SSL_Client::BSSL_SSL_Client(Client *client)
{
    setClient(client);
    mClear();
    mClearAuthenticationSettings();
#if defined(ESP_SSL_FS_SUPPORTED)
    _certStore = nullptr; // Don't want to remove cert store on a clear, should be long lived
#endif
    _sk = nullptr;
#if defined(USE_EMBED_SSL_ENGINE)
    stack_thunk_add_ref();
#endif
}

BSSL_SSL_Client::~BSSL_SSL_Client()
{
    if (_basic_client)
    {
        if (_basic_client->connected())
            _basic_client->stop();
        _basic_client = nullptr;
    }
    freeImpl(&_cipher_list);
    mFreeSSL();
#if defined(USE_EMBED_SSL_ENGINE)
    stack_thunk_del_ref();
#endif
}

void BSSL_SSL_Client::setClient(Client *client, bool ssl)
{
    _basic_client = client;
    _isSSLEnabled = ssl;
}

void BSSL_SSL_Client::setDebugLevel(int level)
{
    if (level < esp_ssl_debug_none || level > esp_ssl_debug_dump)
        _debug_level = esp_ssl_debug_none;
    else
        _debug_level = level;
}

int BSSL_SSL_Client::connect(IPAddress ip, uint16_t port)
{
    if (_isSSLEnabled && mIsSecurePort(port)) // SSL connect
        return connectSSL(ip, port);

    if (!mConnectBasicClient(nullptr, ip, port))
        return 0;

    return 1;
}

int BSSL_SSL_Client::connect(const char *host, uint16_t port)
{
    if (_isSSLEnabled && mIsSecurePort(port))
        return connectSSL(host, port);

    if (!mConnectBasicClient(host, IPAddress(), port))
        return 0;

    return 1;
}

uint8_t BSSL_SSL_Client::connected()
{
    if (!mIsClientInitialized(false))
        return 0;

    if (!_secure)
        return _basic_client->connected();

    // check all of the error cases
    const auto c_con = _basic_client->connected();
    const auto br_con = br_ssl_engine_current_state(_eng) != BR_SSL_CLOSED && _is_connected;
    const auto wr_ok = getWriteError() == 0;
    // if we're in an error state, close the connection and set a write error
    if (br_con && !c_con)
    {
        // If we've got a write error, the client probably failed for some reason
        if (_basic_client->getWriteError())
        {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
            String s = PSTR("Socket was unexpectedly interrupted. m_client error: ");
            s += _basic_client->getWriteError();
            esp_ssl_debug_print(s.c_str(), _debug_level, esp_ssl_debug_info, __func__);
#endif
            setWriteError(esp_ssl_write_error);
        }
        // Else tell the user the endpoint closed the socket on us (ouch)
        else
        {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
            // esp_ssl_debug_print(PSTR("Socket was dropped unexpectedly (this can be an alternative to closing the connection)."), _debug_level, esp_ssl_debug_warn, func_name);
#endif
        }

        // set the write error so the engine doesn't try to close the connection
        _is_connected = false;
        stop();
    }
    else if (!wr_ok)
    {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
        esp_ssl_debug_print(PSTR("Not connected because write error is set."), _debug_level, esp_ssl_debug_error, __func__);
        mPrintClientError(getWriteError(), esp_ssl_debug_error, __func__);
#endif
    }

    return c_con && br_con;
}

void BSSL_SSL_Client::validate(const char *host, uint16_t port)
{
    mConnectionValidate(host, IPAddress(), port);
}

void BSSL_SSL_Client::validate(IPAddress ip, uint16_t port)
{
    mConnectionValidate(nullptr, ip, port);
}

int BSSL_SSL_Client::available()
{
    if (!mIsClientInitialized(false))
        return 0;

    if (!_secure)
        return _basic_client->available();

    // connection check
    if (!mSoftConnected(__func__))
        return 0;

    // run the SSL engine until we are waiting for either user input or a server response
    unsigned state = mUpdateEngine();
    if (state == 0)
    {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
        esp_ssl_debug_print(PSTR("SSL engine failed to update."), _debug_level, esp_ssl_debug_error, __func__);
#endif
    }
    else if (state & BR_SSL_RECVAPP)
    {
        // return how many received bytes we have
        _recvapp_buf = br_ssl_engine_recvapp_buf(_eng, &_recvapp_len);
        return (int)(_recvapp_len);
    }
    else if (state == BR_SSL_CLOSED)
    {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
        esp_ssl_debug_print(PSTR("SSL Engine closed after update."), _debug_level, esp_ssl_debug_info, __func__);
#endif
    }
    // flush the buffer if it's stuck in the SENDAPP state
    else if (state & BR_SSL_SENDAPP)
        br_ssl_engine_flush(_eng, 0);
    // other state, or client is closed
    return 0;
}

int BSSL_SSL_Client::read()
{
    uint8_t read_val;
    return read(&read_val, 1) > 0 ? read_val : -1;
};

int BSSL_SSL_Client::read(uint8_t *buf, size_t size)
{
    if (!mIsClientInitialized(false))
        return 0;

    if (!_secure)
        return _basic_client->read(buf, size);

    // check that the engine is ready to read
    if (available() <= 0 || !size)
        return -1;
    // read the buffer, send the ack, and return the bytes read
    _recvapp_buf = br_ssl_engine_recvapp_buf(_eng, &_recvapp_len);
    const size_t read_amount = size > _recvapp_len ? _recvapp_len : size;
    if (buf)
        memcpy(buf, _recvapp_buf, read_amount);
    // tell engine we read that many bytes
    br_ssl_engine_recvapp_ack(_eng, read_amount);
    // tell the user we read that many bytes
    return read_amount;
}

size_t BSSL_SSL_Client::write(const uint8_t *buf, size_t size)
{
    if (!mIsClientInitialized(false))
        return 0;

    if (!_secure)
        return _basic_client->write(buf, size);

    const char *func_name = __func__;
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
    // super debug
    if (_debug_level >= esp_ssl_debug_dump)
        ESP_SSLCLIENT_DEBUG_PORT.write(buf, size);
#endif
    // check if the socket is still open and such
    if (!mSoftConnected(func_name) || !buf || !size)
        return 0;
    // wait until bearssl is ready to send
    if (mRunUntil(BR_SSL_SENDAPP) < 0)
    {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
        esp_ssl_debug_print(PSTR("Failed while waiting for the engine to enter BR_SSL_SENDAPP."), _debug_level, esp_ssl_debug_error, func_name);
#endif
        return 0;
    }
    // add to the bearssl io buffer, simply appending whatever we want to write
    size_t alen;
    unsigned char *br_buf = br_ssl_engine_sendapp_buf(_eng, &alen);
    size_t cur_idx = 0;
    if (alen == 0)
    {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
        esp_ssl_debug_print(PSTR("BearSSL returned zero length buffer for sending, did an internal error occur?"), _debug_level, esp_ssl_debug_error, func_name);
#endif
        return 0;
    }
    // while there are still elements to write
    while (cur_idx < size)
    {
        // if we're about to fill the buffer, we need to send the data and then wait
        // for another oppurtinity to send
        // so we only send the smallest of the buffer size or our data size - how much we've already sent
        const size_t cpamount = size - cur_idx >= alen - _write_idx ? alen - _write_idx : size - cur_idx;
        memcpy(br_buf + _write_idx, buf + cur_idx, cpamount);
        // increment write idx
        _write_idx += cpamount;
        // increment the buffer pointer
        cur_idx += cpamount;
        // if we filled the buffer, reset _write_idx, and mark the data for sending
        if (_write_idx == alen)
        {
            // indicate to bearssl that we are done writing
            br_ssl_engine_sendapp_ack(_eng, _write_idx);
            // reset the write index
            _write_idx = 0;
            // write to the socket immediatly
            if (mRunUntil(BR_SSL_SENDAPP) < 0)
            {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
                esp_ssl_debug_print(PSTR("Failed while waiting for the engine to enter BR_SSL_SENDAPP."), _debug_level, esp_ssl_debug_error, func_name);
#endif
                return 0;
            }
            // reset the buffer pointer
            br_buf = br_ssl_engine_sendapp_buf(_eng, &alen);
        }
    }
    // works oky
    return size;
}

size_t BSSL_SSL_Client::write(uint8_t b)
{
    return write(&b, 1);
}

size_t BSSL_SSL_Client::write_P(PGM_P buf, size_t size)
{
    char dest[size];
    memcpy_P((void *)dest, buf, size);
    return write((const uint8_t *)dest, size);
}

size_t BSSL_SSL_Client::write(Stream &stream)
{
    if (!mIsClientInitialized(false))
        return 0;

    if (!connected())
    {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
        esp_ssl_debug_print(PSTR("Connect not completed yet."), _debug_level, esp_ssl_debug_error, __func__);
#endif
        return 0;
    }

    size_t dl = stream.available();
    uint8_t buf[dl];
    stream.readBytes(buf, dl);
    return write(buf, dl);
}

int BSSL_SSL_Client::peek()
{

    if (!_sc || !available())
    {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
        esp_ssl_debug_print(PSTR("Not connected, none left available."), _debug_level, esp_ssl_debug_error, __func__);
#endif
        return -1;
    }

    if (!_secure)
        return _basic_client->peek();

    _recvapp_buf = br_ssl_engine_recvapp_buf(_eng, &_recvapp_len);
    if (_recvapp_buf && _recvapp_len)
        return _recvapp_buf[0];

#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
    esp_ssl_debug_print(PSTR("No data left."), _debug_level, esp_ssl_debug_error, __func__);
#endif
    return -1;
}

size_t BSSL_SSL_Client::peekBytes(uint8_t *buffer, size_t length)
{
    if (!mIsClientInitialized(false) || !_secure)
        return 0;

    size_t to_copy = 0;
    if (!_sc)
        return 0;

    unsigned long _startMillis = millis();
    while ((available() < (int)length) && ((millis() - _startMillis) < 5000))
    {
        yield();
    }

    to_copy = _recvapp_len < length ? _recvapp_len : length;
    memcpy(buffer, _recvapp_buf, to_copy);
    return to_copy;
}

// Don't validate the chain, just accept whatever is given.  VERY INSECURE!
void BSSL_SSL_Client::setInsecure()
{
    mClearAuthenticationSettings();
    _use_insecure = true;
}

void BSSL_SSL_Client::enableSSL(bool enable)
{
    _isSSLEnabled = enable;
}

int BSSL_SSL_Client::connectSSL(IPAddress ip, uint16_t port)
{

    if (!mIsClientInitialized(true))
        return 0;

    if (!_basic_client->connected() && !mConnectBasicClient(nullptr, ip, port))
        return 0;

    _ip = ip;
    _port = port;

    return mConnectSSL(nullptr);
}

int BSSL_SSL_Client::connectSSL(const char *host, uint16_t port)
{

    if (!mIsClientInitialized(true))
        return 0;

    if (!_basic_client->connected() && !mConnectBasicClient(host, IPAddress(), port))
        return 0;

    _host = host;
    _port = port;

    return mConnectSSL(host);
}

void BSSL_SSL_Client::stop()
{
    if (!_secure)
        return;

    // Only if we've already connected, store session params and clear the connection options
    if (_session)
        br_ssl_engine_get_session_parameters(_eng, _session->getSession());

    // tell the SSL connection to gracefully close
    // Disabled to prevent close_notify from hanging BSSL_SSL_Client
    // br_ssl_engine_close(_eng);
    // if the engine isn't closed, and the socket is still open
    auto state = br_ssl_engine_current_state(_eng);
    if (state != BR_SSL_CLOSED && state != 0 && connected())
    {
        // Discard any incoming application data.
        _recvapp_buf = br_ssl_engine_recvapp_buf(_eng, &_recvapp_len);
        if (_recvapp_buf != nullptr)
            br_ssl_engine_recvapp_ack(_eng, _recvapp_len);
        // run SSL to finish any existing transactions
        flush();
    }

    // close the socket
    if (_basic_client)
    {
        _basic_client->flush();
        _basic_client->stop();
    }

    mFreeSSL();
}

void BSSL_SSL_Client::setTimeout(unsigned int timeoutMs) { _timeout = timeoutMs; }

void BSSL_SSL_Client::setHandshakeTimeout(unsigned int timeoutMs) { _handshake_timeout = timeoutMs; }

void BSSL_SSL_Client::flush()
{
    if (!_secure && _basic_client)
    {
        _basic_client->flush();
        return;
    }

    if (_write_idx > 0)
    {
        if (mRunUntil(BR_SSL_RECVAPP) < 0)
        {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
            esp_ssl_debug_print(PSTR("Could not flush write buffer!"), _debug_level, esp_ssl_debug_error, __func__);
            int error = br_ssl_engine_last_error(_eng);
            if (error != BR_ERR_OK)
                mPrintSSLError(error, esp_ssl_debug_error, __func__);
            if (getWriteError())
                mPrintClientError(getWriteError(), esp_ssl_debug_error, __func__);
#endif
        }
    }
}

void BSSL_SSL_Client::setBufferSizes(int recv, int xmit)
{
    // Following constants taken from bearssl/src/ssl/ssl_engine.c (not exported unfortunately)
    const int MAX_OUT_OVERHEAD = 85;
    const int MAX_IN_OVERHEAD = 325;

    // The data buffers must be between 512B and 16KB
    recv = std::max(512, std::min(16384, recv));
    xmit = std::max(512, std::min(16384, xmit));

    // Add in overhead for SSL protocol
    recv += MAX_IN_OVERHEAD;
    xmit += MAX_OUT_OVERHEAD;
    _iobuf_in_size = recv;
    _iobuf_out_size = xmit;
}

int BSSL_SSL_Client::availableForWrite()
{
    if (!mIsClientInitialized(false) || !_secure)
        return 0;

    // code taken from ::write()
    if (!connected() || !_handshake_done)
    {
        return 0;
    }
    // Get BearSSL to a state where we can send
    if (mRunUntil(BR_SSL_SENDAPP) < 0)
    {
        return 0;
    }
    if (br_ssl_engine_current_state(_eng) & BR_SSL_SENDAPP)
    {
        size_t sendapp_len;
        (void)br_ssl_engine_sendapp_buf(_eng, &sendapp_len);
        // We want to call br_ssl_engine_sendapp_ack(0) but 0 is forbidden (bssl doc).
        // After checking br_ssl_engine_sendapp_buf() src code,
        // it seems that it is OK to not call ack when the buffer is left untouched.
        // forbidden: br_ssl_engine_sendapp_ack(_eng, 0);
        return (int)sendapp_len;
    }
    return 0;
}

void BSSL_SSL_Client::setSession(BearSSL_Session *session) { _session = session; };

// Assume a given public key, don't validate or use cert info at all
void BSSL_SSL_Client::setKnownKey(const PublicKey *pk, unsigned usages)
{
    mClearAuthenticationSettings();
    _knownkey = pk;
    _knownkey_usages = usages;
}

// Only check SHA1 fingerprint of certificate
bool BSSL_SSL_Client::setFingerprint(const uint8_t fingerprint[20])
{
    mClearAuthenticationSettings();
    _use_fingerprint = true;
    memcpy_P(_fingerprint, fingerprint, 20);
    return true;
}

// Set a fingerprint by parsing an ASCII string
bool BSSL_SSL_Client::setFingerprint(const char *fpStr)
{
    int idx = 0;
    uint8_t c, d;
    uint8_t fp[20];

    while (idx < 20)
    {
        c = pgm_read_byte(fpStr++);
        if (!c)
        {
            break; // String ended, done processing
        }
        d = pgm_read_byte(fpStr++);
        if (!d)
        {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
            esp_ssl_debug_print(PSTR("FP too short"), _debug_level, esp_ssl_debug_error, __func__);
#endif
            return false; // Only half of the last hex digit, error
        }
        c = htoi(c);
        d = htoi(d);
        if ((c > 15) || (d > 15))
        {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
            esp_ssl_debug_print(PSTR("Invalid char"), _debug_level, esp_ssl_debug_error, __func__);
#endif
            return false; // Error in one of the hex characters
        }
        fp[idx++] = (c << 4) | d;

        // Skip 0 or more spaces or colons
        while (pgm_read_byte(fpStr) && (pgm_read_byte(fpStr) == ' ' || pgm_read_byte(fpStr) == ':'))
        {
            fpStr++;
        }
    }
    if ((idx != 20) || pgm_read_byte(fpStr))
    {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
        esp_ssl_debug_print(PSTR("Garbage at end of fp"), _debug_level, esp_ssl_debug_error, __func__);
#endif
        return false; // Garbage at EOL or we didn't have enough hex digits
    }
    return setFingerprint(fp);
}

// Accept any certificate that's self-signed
void BSSL_SSL_Client::allowSelfSignedCerts()
{
    mClearAuthenticationSettings();
    _use_self_signed = true;
}

// Install certificates of trusted CAs or specific site
void BSSL_SSL_Client::setTrustAnchors(const X509List *ta)
{
    mClearAuthenticationSettings();
    _ta = ta;
}

// In cases when NTP is not used, app must set a time manually to check cert validity
void BSSL_SSL_Client::setX509Time(time_t now)
{
    _now = now;
}

void BSSL_SSL_Client::setClientRSACert(const X509List *chain, const PrivateKey *sk)
{
    if (_esp32_chain)
    {
        delete _esp32_chain;
        _esp32_chain = nullptr;
    }
    if (_esp32_sk)
    {
        delete _esp32_sk;
        _esp32_sk = nullptr;
    }
    _chain = chain;
    _sk = sk;
}

void BSSL_SSL_Client::setClientECCert(const X509List *chain, const PrivateKey *sk, unsigned allowed_usages, unsigned cert_issuer_key_type)
{
    if (_esp32_chain)
    {
        delete _esp32_chain;
        _esp32_chain = nullptr;
    }
    if (_esp32_sk)
    {
        delete _esp32_sk;
        _esp32_sk = nullptr;
    }
    _chain = chain;
    _sk = sk;
    _allowed_usages = allowed_usages;
    _cert_issuer_key_type = cert_issuer_key_type;
}

// Returns whether MFLN negotiation for the above buffer sizes succeeded (after connection)
int BSSL_SSL_Client::getMFLNStatus()
{
    return connected() && br_ssl_engine_get_mfln_negotiated(_eng);
}

// Returns an error ID and possibly a string (if dest != null) of the last
// BearSSL reported error.
int BSSL_SSL_Client::getLastSSLError(char *dest, size_t len)
{
    int err = 0;
    const char *t = "";
    const char *recv_fatal = "";
    const char *send_fatal = "";
    if (_sc)
        err = br_ssl_engine_last_error(_eng);

    if (_oom_err)
        err = -1000;
    else
    {
        if (err & BR_ERR_RECV_FATAL_ALERT)
        {
#if defined(ESP_SSLCLIENT_ENABLE_SSL_ERROR_STRING)
            recv_fatal = PSTR("SSL received fatal alert - ");
#endif
            err &= ~BR_ERR_RECV_FATAL_ALERT;
        }
        if (err & BR_ERR_SEND_FATAL_ALERT)
        {
#if defined(ESP_SSLCLIENT_ENABLE_SSL_ERROR_STRING)
            send_fatal = PSTR("SSL sent fatal alert - ");
#endif
            err &= ~BR_ERR_SEND_FATAL_ALERT;
        }
    }
#if defined(ESP_SSLCLIENT_ENABLE_SSL_ERROR_STRING)
    switch (err)
    {
    case -1000:
        t = PSTR("Unable to allocate memory for SSL structures and buffers.");
        break;
    case BR_ERR_BAD_PARAM:
        t = PSTR("Caller-provided parameter is incorrect.");
        break;
    case BR_ERR_BAD_STATE:
        t = PSTR("Operation requested by the caller cannot be applied with the current context state (e.g. reading data while outgoing data is waiting to be sent).");
        break;
    case BR_ERR_UNSUPPORTED_VERSION:
        t = PSTR("Incoming protocol or record version is unsupported.");
        break;
    case BR_ERR_BAD_VERSION:
        t = PSTR("Incoming record version does not match the expected version.");
        break;
    case BR_ERR_BAD_LENGTH:
        t = PSTR("Incoming record length is invalid.");
        break;
    case BR_ERR_TOO_LARGE:
        t = PSTR("Incoming record is too large to be processed, or buffer is too small for the handshake message to send.");
        break;
    case BR_ERR_BAD_MAC:
        t = PSTR("Decryption found an invalid padding, or the record MAC is not correct.");
        break;
    case BR_ERR_NO_RANDOM:
        t = PSTR("No initial entropy was provided, and none can be obtained from the OS.");
        break;
    case BR_ERR_UNKNOWN_TYPE:
        t = PSTR("Incoming record type is unknown.");
        break;
    case BR_ERR_UNEXPECTED:
        t = PSTR("Incoming record or message has wrong type with regards to the current engine state.");
        break;
    case BR_ERR_BAD_CCS:
        t = PSTR("ChangeCipherSpec message from the peer has invalid contents.");
        break;
    case BR_ERR_BAD_ALERT:
        t = PSTR("Alert message from the peer has invalid contents (odd length).");
        break;
    case BR_ERR_BAD_HANDSHAKE:
        t = PSTR("Incoming handshake message decoding failed.");
        break;
    case BR_ERR_OVERSIZED_ID:
        t = PSTR("ServerHello contains a session ID which is larger than 32 bytes.");
        break;
    case BR_ERR_BAD_CIPHER_SUITE:
        t = PSTR("Server wants to use a cipher suite that we did not claim to support. This is also reported if we tried to advertise a cipher suite that we do not support.");
        break;
    case BR_ERR_BAD_COMPRESSION:
        t = PSTR("Server wants to use a compression that we did not claim to support.");
        break;
    case BR_ERR_BAD_FRAGLEN:
        t = PSTR("Server's max fragment length does not match client's.");
        break;
    case BR_ERR_BAD_SECRENEG:
        t = PSTR("Secure renegotiation failed.");
        break;
    case BR_ERR_EXTRA_EXTENSION:
        t = PSTR("Server sent an extension type that we did not announce, or used the same extension type several times in a single ServerHello.");
        break;
    case BR_ERR_BAD_SNI:
        t = PSTR("Invalid Server Name Indication contents (when used by the server, this extension shall be empty).");
        break;
    case BR_ERR_BAD_HELLO_DONE:
        t = PSTR("Invalid ServerHelloDone from the server (length is not 0).");
        break;
    case BR_ERR_LIMIT_EXCEEDED:
        t = PSTR("Internal limit exceeded (e.g. server's public key is too large).");
        break;
    case BR_ERR_BAD_FINISHED:
        t = PSTR("Finished message from peer does not match the expected value.");
        break;
    case BR_ERR_RESUME_MISMATCH:
        t = PSTR("Session resumption attempt with distinct version or cipher suite.");
        break;
    case BR_ERR_INVALID_ALGORITHM:
        t = PSTR("Unsupported or invalid algorithm (ECDHE curve, signature algorithm, hash function).");
        break;
    case BR_ERR_BAD_SIGNATURE:
        t = PSTR("Invalid signature in ServerKeyExchange or CertificateVerify message.");
        break;
    case BR_ERR_WRONG_KEY_USAGE:
        t = PSTR("Peer's public key does not have the proper type or is not allowed for the requested operation.");
        break;
    case BR_ERR_NO_CLIENT_AUTH:
        t = PSTR("Client did not send a certificate upon request, or the client certificate could not be validated.");
        break;
    case BR_ERR_IO:
        t = PSTR("I/O error or premature close on transport stream.");
        break;
    case BR_ERR_X509_INVALID_VALUE:
        t = PSTR("Invalid value in an ASN.1 structure.");
        break;
    case BR_ERR_X509_TRUNCATED:
        t = PSTR("Truncated certificate or other ASN.1 object.");
        break;
    case BR_ERR_X509_EMPTY_CHAIN:
        t = PSTR("Empty certificate chain (no certificate at all).");
        break;
    case BR_ERR_X509_INNER_TRUNC:
        t = PSTR("Decoding error: inner element extends beyond outer element size.");
        break;
    case BR_ERR_X509_BAD_TAG_CLASS:
        t = PSTR("Decoding error: unsupported tag class (application or private).");
        break;
    case BR_ERR_X509_BAD_TAG_VALUE:
        t = PSTR("Decoding error: unsupported tag value.");
        break;
    case BR_ERR_X509_INDEFINITE_LENGTH:
        t = PSTR("Decoding error: indefinite length.");
        break;
    case BR_ERR_X509_EXTRA_ELEMENT:
        t = PSTR("Decoding error: extraneous element.");
        break;
    case BR_ERR_X509_UNEXPECTED:
        t = PSTR("Decoding error: unexpected element.");
        break;
    case BR_ERR_X509_NOT_CONSTRUCTED:
        t = PSTR("Decoding error: expected constructed element, but is primitive.");
        break;
    case BR_ERR_X509_NOT_PRIMITIVE:
        t = PSTR("Decoding error: expected primitive element, but is constructed.");
        break;
    case BR_ERR_X509_PARTIAL_BYTE:
        t = PSTR("Decoding error: BIT STRING length is not multiple of 8.");
        break;
    case BR_ERR_X509_BAD_BOOLEAN:
        t = PSTR("Decoding error: BOOLEAN value has invalid length.");
        break;
    case BR_ERR_X509_OVERFLOW:
        t = PSTR("Decoding error: value is off-limits.");
        break;
    case BR_ERR_X509_BAD_DN:
        t = PSTR("Invalid distinguished name.");
        break;
    case BR_ERR_X509_BAD_TIME:
        t = PSTR("Invalid date/time representation.");
        break;
    case BR_ERR_X509_UNSUPPORTED:
        t = PSTR("Certificate contains unsupported features that cannot be ignored.");
        break;
    case BR_ERR_X509_LIMIT_EXCEEDED:
        t = PSTR("Key or signature size exceeds internal limits.");
        break;
    case BR_ERR_X509_WRONG_KEY_TYPE:
        t = PSTR("Key type does not match that which was expected.");
        break;
    case BR_ERR_X509_BAD_SIGNATURE:
        t = PSTR("Signature is invalid.");
        break;
    case BR_ERR_X509_TIME_UNKNOWN:
        t = PSTR("Validation time is unknown.");
        break;
    case BR_ERR_X509_EXPIRED:
        t = PSTR("Certificate is expired or not yet valid.");
        break;
    case BR_ERR_X509_DN_MISMATCH:
        t = PSTR("Issuer/Subject DN mismatch in the chain.");
        break;
    case BR_ERR_X509_BAD_SERVER_NAME:
        t = PSTR("Expected server name was not found in the chain.");
        break;
    case BR_ERR_X509_CRITICAL_EXTENSION:
        t = PSTR("Unknown critical extension in certificate.");
        break;
    case BR_ERR_X509_NOT_CA:
        t = PSTR("Not a CA, or path length constraint violation.");
        break;
    case BR_ERR_X509_FORBIDDEN_KEY_USAGE:
        t = PSTR("Key Usage extension prohibits intended usage.");
        break;
    case BR_ERR_X509_WEAK_PUBLIC_KEY:
        t = PSTR("Public key found in certificate is too small.");
        break;
    case BR_ERR_X509_NOT_TRUSTED:
        t = PSTR("Chain could not be linked to a trust anchor.");
        break;
    default:
        t = PSTR("Unknown error code.");
        break;
    }

    if (dest)
    {
        // snprintf is PSTR safe and guaranteed to 0-terminate
        snprintf(dest, len, "%s%s%s", recv_fatal, send_fatal, t);
    }

#endif

    return err;
}

// Attach a preconfigured certificate store
#if defined(ESP_SSL_FS_SUPPORTED)
void BSSL_SSL_Client::setCertStore(CertStoreBase *certStore)
{
    _certStore = certStore;
}
#endif

// Set custom list of ciphers
bool BSSL_SSL_Client::setCiphers(const uint16_t *cipherAry, int cipherCount)
{
    _cipher_list = (uint16_t *)mallocImpl(cipherCount);
    if (!_cipher_list)
    {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
        esp_ssl_debug_print(PSTR("list empty"), _debug_level, esp_ssl_debug_error, __func__);
#endif
        return false;
    }
    memcpy_P(_cipher_list, cipherAry, cipherCount * sizeof(uint16_t));
    _cipher_cnt = cipherCount;
    return true;
}

bool BSSL_SSL_Client::setCiphers(const std::vector<uint16_t> &list)
{
    return setCiphers(&list[0], list.size());
}

bool BSSL_SSL_Client::setCiphersLessSecure()
{
    return setCiphers(faster_suites_P, sizeof(faster_suites_P) / sizeof(faster_suites_P[0]));
}

bool BSSL_SSL_Client::setSSLVersion(uint32_t min, uint32_t max)
{
    if (((min != BR_TLS10) && (min != BR_TLS11) && (min != BR_TLS12)) ||
        ((max != BR_TLS10) && (max != BR_TLS11) && (max != BR_TLS12)) ||
        (max < min))
    {
        return false; // Invalid options
    }
    _tls_min = min;
    _tls_max = max;
    return true;
}

// Checks for support of Maximum Frame Length Negotiation at the given
// blocksize.  Note that, per spec, only 512, 1024, 2048, and 4096 are
// supported.  Many servers today do not support this negotiation.

// TODO - Allow for fragmentation...but not very critical as the ServerHello
//      we use comes to < 80 bytes which has no reason to ever be fragmented.
// TODO - Check the type of returned extensions and that the MFL is the exact
//      same one we sent.  Not critical as only horribly broken servers would
//      return changed or add their own extensions.
bool BSSL_SSL_Client::probeMaxFragmentLength(IPAddress ip, uint16_t port, uint16_t len)
{
    return mProbeMaxFragmentLength(nullptr, ip, port, len);
}

bool BSSL_SSL_Client::probeMaxFragmentLength(const char *name, uint16_t port, uint16_t len)
{
    return mProbeMaxFragmentLength(name, IPAddress(), port, len);
}

bool BSSL_SSL_Client::probeMaxFragmentLength(const String &host, uint16_t port, uint16_t len)
{
    return BSSL_SSL_Client::probeMaxFragmentLength(host.c_str(), port, len);
}

size_t BSSL_SSL_Client::peekAvailable()
{
    return available();
}

// return a pointer to available data buffer (size = peekAvailable())
// semantic forbids any kind of read() before calling peekConsume()
const char *BSSL_SSL_Client::peekBuffer()
{
    return (const char *)_recvapp_buf;
}

// consume bytes after use (see peekBuffer)
void BSSL_SSL_Client::peekConsume(size_t consume)
{
    // according to BSSL_SSL_Client::read:
    br_ssl_engine_recvapp_ack(_eng, consume);
    _recvapp_buf = nullptr;
    _recvapp_len = 0;
}

void BSSL_SSL_Client::setCACert(const char *rootCA)
{
    if (_esp32_ta)
        delete _esp32_ta;
    _esp32_ta = new X509List(rootCA);
}

void BSSL_SSL_Client::setCertificate(const char *client_ca)
{
    if (_esp32_chain)
        delete _esp32_chain;
    _esp32_chain = new X509List(client_ca);
}

void BSSL_SSL_Client::setPrivateKey(const char *private_key)
{
    if (_esp32_sk)
        delete _esp32_sk;
    _esp32_sk = new PrivateKey(private_key);
}

bool BSSL_SSL_Client::loadCACert(Stream &stream, size_t size)
{
    bool ret = false;
    auto buff = (char *)mallocImpl(size);
    if (size == stream.readBytes(buff, size))
    {
        setCACert(buff);
        ret = true;
    }
    freeImpl(&buff);
    return ret;
}

bool BSSL_SSL_Client::loadCertificate(Stream &stream, size_t size)
{
    bool ret = false;
    auto buff = (char *)mallocImpl(size);
    if (size == stream.readBytes(buff, size))
    {
        setCertificate(buff);
        ret = true;
    }
    freeImpl(&buff);
    return ret;
}

bool BSSL_SSL_Client::loadPrivateKey(Stream &stream, size_t size)
{
    bool ret = false;
    auto buff = (char *)mallocImpl(size);
    if (size == stream.readBytes(buff, size))
    {
        setPrivateKey(buff);
        ret = true;
    }
    freeImpl(&buff);
    return ret;
}

int BSSL_SSL_Client::connect(IPAddress ip, uint16_t port, const char *rootCABuff, const char *cli_cert, const char *cli_key)
{
    setSecure(rootCABuff, cli_cert, cli_key);
    return connect(ip, port);
}

int BSSL_SSL_Client::connect(const char *host, uint16_t port, const char *rootCABuff, const char *cli_cert, const char *cli_key)
{
    setSecure(rootCABuff, cli_cert, cli_key);
    return connect(host, port);
}

BSSL_SSL_Client &BSSL_SSL_Client::operator=(const BSSL_SSL_Client &other)
{
    stop();
    setClient(other._basic_client);
    _use_insecure = other._use_insecure;
    _timeout = other._timeout;
    _handshake_timeout = other._handshake_timeout;
    return *this;
}

bool BSSL_SSL_Client::operator==(const BSSL_SSL_Client &rhs)
{
    return _basic_client == rhs._basic_client;
}

unsigned int BSSL_SSL_Client::getTimeout() const { return _timeout; }

void BSSL_SSL_Client::setSecure(const char *rootCABuff, const char *cli_cert, const char *cli_key)
{
    if (_esp32_ta)
    {
        delete _esp32_ta;
        _esp32_ta = nullptr;
    }
    if (_esp32_chain)
    {
        delete _esp32_chain;
        _esp32_chain = nullptr;
    }
    if (_esp32_sk)
    {
        delete _esp32_sk;
        _esp32_sk = nullptr;
    }
    if (rootCABuff)
    {
        setCertificate(rootCABuff);
    }
    if (cli_cert && cli_key)
    {
        setCertificate(cli_cert);
        setPrivateKey(cli_key);
    }
}

//////////////////////////////////////////////////////
// Private access
//////////////////////////////////////////////////////

bool BSSL_SSL_Client::mProbeMaxFragmentLength(Client *probe, uint16_t len)
{

    // Hardcoded TLS 1.2 packets used throughout
    static const uint8_t clientHelloHead_P[] PROGMEM = {
        0x16, 0x03, 0x03, 0x00, 0,                      // TLS header, change last 2 bytes to len
        0x01, 0x00, 0x00, 0,                            // Last 3 bytes == length
        0x03, 0x03,                                     // Proto version TLS 1.2
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, // Random (gmtime + rand[28])
        0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
        0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
        0x00, // Session ID
    };
    // Followed by our cipher-suite, generated on-the-fly
    //    0x00, 0x02, // cipher suite len
    //      0xc0, 0x13, // BR_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA
    static const uint8_t clientHelloTail_P[] PROGMEM = {
        0x01, 0x00,            // No compression
        0x00, 26 + 14 + 6 + 5, // Extension length
        0x00, 0x0d, 0x00, 0x16, 0x00, 0x14, 0x04, 0x03, 0x03, 0x03, 0x05, 0x03,
        0x06, 0x03, 0x02, 0x03, 0x04, 0x01, 0x03, 0x01, 0x05, 0x01, 0x06,
        0x01, 0x02, 0x01, // Supported signature algorithms
        0x00, 0x0a, 0x00, 0x0a, 0x00, 0x08, 0x00, 0x17, 0x00, 0x18, 0x00, 0x19,
        0x00, 0x1d,                         // Supported groups
        0x00, 0x0b, 0x00, 0x02, 0x01, 0x00, // Supported EC formats
        0x00, 0x01,                         // Max Frag Len
        0x00, 0x01,                         // len of MaxFragLen
    };
    // Followed by a 1-byte MFLN size requesst
    //          0x04 // 2^12 = 4K
    uint8_t mfl;

    switch (len)
    {
    case 512:
        mfl = 1;
        break;
    case 1024:
        mfl = 2;
        break;
    case 2048:
        mfl = 3;
        break;
    case 4096:
        mfl = 4;
        break;
    default:
        return false; // Invalid size
    }
    int ttlLen = sizeof(clientHelloHead_P) + (2 + sizeof(suites_P)) + (sizeof(clientHelloTail_P) + 1);
    uint8_t *clientHello = (uint8_t *)mallocImpl(ttlLen);
    if (!clientHello)
    {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
        esp_ssl_debug_print(PSTR("OOM error."), _debug_level, esp_ssl_debug_error, __func__);
#endif
        return false;
    }
    memcpy_P(clientHello, clientHelloHead_P, sizeof(clientHelloHead_P));
    clientHello[sizeof(clientHelloHead_P) + 0] = sizeof(suites_P) >> 8;   // MSB byte len
    clientHello[sizeof(clientHelloHead_P) + 1] = sizeof(suites_P) & 0xff; // LSB byte len
    for (size_t i = 0; i < sizeof(suites_P) / sizeof(suites_P[0]); i++)
    {
        uint16_t flip = pgm_read_word(&suites_P[i]);
        // Swap to network byte order
        flip = ((flip >> 8) & 0xff) | ((flip & 0xff) << 8);
        memcpy(clientHello + sizeof(clientHelloHead_P) + 2 + 2 * i, &flip, 2);
    }
    memcpy_P(clientHello + sizeof(clientHelloHead_P) + 2 + sizeof(suites_P), clientHelloTail_P, sizeof(clientHelloTail_P));
    clientHello[sizeof(clientHelloHead_P) + 2 + sizeof(suites_P) + sizeof(clientHelloTail_P)] = mfl;

    // Fix up TLS fragment length
    clientHello[3] = (ttlLen - 5) >> 8;
    clientHello[4] = (ttlLen - 5) & 0xff;
    // Fix up ClientHello message length
    clientHello[7] = (ttlLen - 5 - 4) >> 8;
    clientHello[8] = (ttlLen - 5 - 4) & 0xff;

    int ret = probe->write(clientHello, ttlLen);
    freeImpl(&clientHello); // We're done w/the hello message
    if (!probe->connected() || (ret != ttlLen))
    {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
        esp_ssl_debug_print(PSTR("Protocol error."), _debug_level, esp_ssl_debug_error, __func__);
#endif
        return false;
    }

    bool supportsLen = false;
    uint8_t fragResp[5];
    int fragLen;
    uint8_t hand[4];
    int handLen;
    uint8_t protoVer[2];
    uint8_t rand[32];
    uint8_t sessionLen;
    uint8_t cipher[2];
    uint8_t comp;
    uint8_t extBytes[2];
    uint16_t extLen;

    ret = probe->readBytes(fragResp, 5);
    if (!probe->connected() || (ret != 5) || (fragResp[0] != 0x16) || (fragResp[1] != 0x03) || (fragResp[2] != 0x03))
    {
        // Short read, not a HANDSHAKE or not TLS 1.2, so it's not supported
        return send_abort(probe, supportsLen);
    }
    fragLen = (fragResp[3] << 8) | fragResp[4];
    if (fragLen < 4 + 2 + 32 + 1 + 2 + 1)
    {
        // Too short to have an extension
        return send_abort(probe, supportsLen);
    }

    ret = probe->readBytes(hand, 4);
    fragLen -= ret;
    if ((ret != 4) || (hand[0] != 2))
    {
        // Short read or not server_hello
        return send_abort(probe, supportsLen);
    }
    handLen = (hand[1] << 16) | (hand[2] << 8) | hand[3];
    if (handLen != fragLen)
    {
        // Got some weird mismatch, this is invalid
        return send_abort(probe, supportsLen);
    }

    ret = probe->readBytes(protoVer, 2);
    handLen -= ret;
    if ((ret != 2) || (protoVer[0] != 0x03) || (protoVer[1] != 0x03))
    {
        // Short read or not tls 1.2, so can't do MFLN
        return send_abort(probe, supportsLen);
    }

    ret = probe->readBytes(rand, 32);
    handLen -= ret;
    if (ret != 32)
    {
        // short read of random data
        return send_abort(probe, supportsLen);
    }

    ret = probe->readBytes(&sessionLen, 1);
    handLen -= ret;
    if ((ret != 1) || (sessionLen > 32))
    {
        // short read of session len or invalid size
        return send_abort(probe, supportsLen);
    }
    if (sessionLen)
    {
        ret = probe->readBytes(rand, sessionLen);
        handLen -= ret;
        if (ret != sessionLen)
        {
            // short session id read
            return send_abort(probe, supportsLen);
        }
    }

    ret = probe->readBytes(cipher, 2);
    handLen -= ret;
    if (ret != 2)
    {
        // Short read...we don't check the cipher here
        return send_abort(probe, supportsLen);
    }

    ret = probe->readBytes(&comp, 1);
    handLen -= ret;
    if ((ret != 1) || comp != 0)
    {
        // short read or invalid compression
        return send_abort(probe, supportsLen);
    }

    ret = probe->readBytes(extBytes, 2);
    handLen -= ret;
    extLen = extBytes[1] | (extBytes[0] << 8);
    if ((extLen == 0) || (ret != 2))
    {
        return send_abort(probe, supportsLen);
    }

    while (handLen > 0)
    {
        // Parse each extension and look for MFLN
        uint8_t typeBytes[2];
        ret = probe->readBytes(typeBytes, 2);
        handLen -= 2;
        if ((ret != 2) || (handLen <= 0))
        {
            return send_abort(probe, supportsLen);
        }
        uint8_t lenBytes[2];
        ret = probe->readBytes(lenBytes, 2);
        handLen -= 2;
        uint16_t extLen = lenBytes[1] | (lenBytes[0] << 8);
        if ((ret != 2) || (handLen <= 0) || (extLen > 32) || (extLen > handLen))
        {
            return send_abort(probe, supportsLen);
        }
        if ((typeBytes[0] == 0x00) && (typeBytes[1] == 0x01))
        { // MFLN extension!
            // If present and 1-byte in length, it's supported
            return send_abort(probe, extLen == 1 ? true : false);
        }
        // Skip the extension, move to next one
        uint8_t junk[32];
        ret = probe->readBytes(junk, extLen);
        handLen -= extLen;
        if (ret != extLen)
        {
            return send_abort(probe, supportsLen);
        }
    }
    return send_abort(probe, supportsLen);
}

bool BSSL_SSL_Client::mProbeMaxFragmentLength(const char *name, IPAddress ip, uint16_t port, uint16_t len)
{
    if (!mIsClientInitialized(false))
        return false;

    _basic_client->stop();

    if (!(name ? _basic_client->connect(name, port) : _basic_client->connect(ip, port)))
    {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
        esp_ssl_debug_print(PSTR("Can't connect."), _debug_level, esp_ssl_debug_error, __func__);
#endif
        return false;
    }

    bool ret = mProbeMaxFragmentLength(_basic_client, len);
    _basic_client->stop();
    return ret;
}

int BSSL_SSL_Client::mIsClientInitialized(bool notify)
{
    if (!_basic_client)
    {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
        if (notify)
            esp_ssl_debug_print(PSTR("Basic client is not yet initialized."), _debug_level, esp_ssl_debug_error, __func__);
#endif
        return 0;
    }
    return 1;
}

int BSSL_SSL_Client::mConnectBasicClient(const char *host, IPAddress ip, uint16_t port)
{

    if (!mConnectionValidate(host, ip, port))
        return 0;

    if (!(host ? _basic_client->connect(host, port) : _basic_client->connect(ip, port)))
    {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
        esp_ssl_debug_print(PSTR("Failed to connect to server using basic client."), _debug_level, esp_ssl_debug_error, __func__);
#endif
        setWriteError(esp_ssl_connection_fail);
        mFreeSSL();
        return 0;
    }

    _secure = false;
    _write_idx = 0;
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
    esp_ssl_debug_print(PSTR("Basic client connected!"), _debug_level, esp_ssl_debug_info, __func__);
#endif
    return 1;
}

bool BSSL_SSL_Client::mSoftConnected(const char *func_name)
{
    // check if the socket is still open and such
    if (getWriteError())
    {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
        esp_ssl_debug_print(PSTR("Cannot operate if the write error is not reset."), _debug_level, esp_ssl_debug_error, func_name);
        mPrintClientError(getWriteError(), esp_ssl_debug_error, func_name);
#endif
        return false;
    }
    // check if the ssl engine is still open
    if (!_is_connected || br_ssl_engine_current_state(_eng) == BR_SSL_CLOSED)
    {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
        esp_ssl_debug_print(PSTR("Cannot operate on a closed SSL connection."), _debug_level, esp_ssl_debug_error, func_name);
        int error = br_ssl_engine_last_error(_eng);
        if (error != BR_ERR_OK)
            mPrintSSLError(error, esp_ssl_debug_error, func_name);
#endif
        return false;
    }
    return true;
}

int BSSL_SSL_Client::mConnectSSL(const char *host)
{

#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
    esp_ssl_debug_print(PSTR("Start connection."), _debug_level, esp_ssl_debug_info, __func__);
#endif
    mFreeSSL();
    _oom_err = false;

#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
    // BearSSL will reject all connections unless an authentication option is set, warn in DEBUG builds
#if defined(ESP_SSL_FS_SUPPORTED)
#define CRTSTORECOND &&!_certStore
#else
#define CRTSTORECOND
#endif
    if (!_use_insecure && !_use_fingerprint && !_use_self_signed && !_knownkey CRTSTORECOND && !_ta)
    {
        esp_ssl_debug_print(PSTR("Connection *will* fail, no authentication method is setup."), _debug_level, esp_ssl_debug_warn, __func__);
    }
#endif

    _sc = std::make_shared<br_ssl_client_context>();
    _eng = &_sc->eng; // Allocation/deallocation taken care of by the _sc shared_ptr

    _iobuf_in = (unsigned char *)mallocImpl(_iobuf_in_size);
    _iobuf_out = (unsigned char *)mallocImpl(_iobuf_out_size);

    if (!_sc || !_iobuf_in || !_iobuf_out)
    {
        mFreeSSL(); // Frees _sc, _iobuf*
        _oom_err = true;
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
        esp_ssl_debug_print(PSTR("OOM error."), _debug_level, esp_ssl_debug_error, __func__);
#endif
        return 0;
    }

    // If no cipher list yet set, use defaults
    if (!_cipher_list)
        bssl::br_ssl_client_base_init(_sc.get(), suites_P, sizeof(suites_P) / sizeof(suites_P[0]));
    else
        bssl::br_ssl_client_base_init(_sc.get(), _cipher_list, _cipher_cnt);

    // Only failure possible in the installation is OOM
    if (!mInstallClientX509Validator())
    {
        mFreeSSL();
        _oom_err = true;
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
        esp_ssl_debug_print(PSTR("Can't install x509 validator."), _debug_level, esp_ssl_debug_error, __func__);
#endif
        return 0;
    }

    br_ssl_engine_set_buffers_bidi(_eng, _iobuf_in, _iobuf_in_size, _iobuf_out, _iobuf_out_size);
    br_ssl_engine_set_versions(_eng, _tls_min, _tls_max);

    // Apply any client certificates, if supplied.
    if (_sk && _sk->isRSA())
    {
        br_ssl_client_set_single_rsa(_sc.get(), _chain ? _chain->getX509Certs() : nullptr, _chain ? _chain->getCount() : 0,
                                     _sk->getRSA(), br_rsa_pkcs1_sign_get_default());
    }
    else if (_sk && _sk->isEC())
    {
#ifndef BEARSSL_SSL_BASIC
        br_ssl_client_set_single_ec(_sc.get(), _chain ? _chain->getX509Certs() : nullptr, _chain ? _chain->getCount() : 0,
                                    _sk->getEC(), _allowed_usages,
                                    _cert_issuer_key_type, br_ec_get_default(), br_ecdsa_sign_asn1_get_default());
#else
        mFreeSSL();
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
        esp_ssl_debug_print(PSTR("Attempting to use EC cert in minimal cipher mode (no EC)."), _debug_level, esp_ssl_debug_error, __func__);
#endif
        return 0;
#endif
    }
    else if (_esp32_sk && _esp32_chain)
    {
        br_ssl_client_set_single_rsa(_sc.get(), _esp32_chain->getX509Certs(), _esp32_chain->getCount(),
                                     _esp32_sk->getRSA(), br_rsa_pkcs1_sign_get_default());
    }

    // clear the write error
    setWriteError(esp_ssl_ok);

    // we want 128 bits to be safe, as recommended by the bearssl docs
    uint8_t rng_seeds[16];

    // prng
    for (uint8_t i = 0; i < sizeof rng_seeds; i++)
        rng_seeds[i] = static_cast<uint8_t>(random(256));

    br_ssl_engine_inject_entropy(_eng, rng_seeds, sizeof rng_seeds);

    // Restore session from the storage spot, if present
    if (_session)
    {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
        esp_ssl_debug_print(PSTR("Set SSL session!"), _debug_level, esp_ssl_debug_info, __func__);
#endif
        br_ssl_engine_set_session_parameters(_eng, _session->getSession());
    }

    if (!br_ssl_client_reset(_sc.get(), host, _session ? 1 : 0))
    {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
        esp_ssl_debug_print(PSTR("Can't reset client."), _debug_level, esp_ssl_debug_error, __func__);
        mPrintSSLError(br_ssl_engine_last_error(_eng), esp_ssl_debug_error, __func__);
#endif
        setWriteError(esp_ssl_connection_fail);
        mFreeSSL();
        return 0;
    }

// SSL/TLS handshake
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
    esp_ssl_debug_print(PSTR("Wait for SSL handshake."), _debug_level, esp_ssl_debug_info, __func__);
#endif

    if (mRunUntil(BR_SSL_SENDAPP, _handshake_timeout) < 0)
    {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
        esp_ssl_debug_print(PSTR("Failed to initlalize the SSL layer."), _debug_level, esp_ssl_debug_error, __func__);
        mPrintSSLError(br_ssl_engine_last_error(_eng), esp_ssl_debug_error, __func__);
#endif
        mFreeSSL();
        return 0;
    }

#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
    esp_ssl_debug_print(PSTR("Connection successful!"), _debug_level, esp_ssl_debug_info, __func__);
#endif
    _handshake_done = true;
    _is_connected = true;
    _secure = true;

    // Save session
    if (_session)
        br_ssl_engine_get_session_parameters(_eng, _session->getSession());

    // Session is already validated here, there is no need to keep following
    _x509_minimal = nullptr;
    _x509_insecure = nullptr;
    _x509_knownkey = nullptr;

    return 1;
}

bool BSSL_SSL_Client::mConnectionValidate(const char *host, IPAddress ip, uint16_t port)
{
    if (!mIsClientInitialized(true))
        return false;

    if (_basic_client && _basic_client->connected() &&
                host
            ? (strcasecmp(host, _host.c_str()) != 0 || port != _port)
            : (ip != _ip || port != _port))
    {
        _basic_client->stop();
    }

    return true;
}

int BSSL_SSL_Client::mRunUntil(const unsigned target, unsigned long timeout)
{
    unsigned lastState = 0;
    size_t lastLen = 0;
    const unsigned long start = millis();
    for (;;)
    {
        unsigned state = mUpdateEngine();
        // error check
        if (state == BR_SSL_CLOSED || getWriteError() != esp_ssl_ok)
        {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
            if (state == BR_SSL_CLOSED)
                esp_ssl_debug_print(PSTR("Terminating because the ssl engine closed."), _debug_level, esp_ssl_debug_warn, __func__);
            else
                esp_ssl_debug_print(PSTR("Terminating with write error."), _debug_level, esp_ssl_debug_warn, __func__);
#endif
            return -1;
        }
        // timeout check
        if (millis() - start > ((timeout > 0) ? timeout : getTimeout()))
        {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
            esp_ssl_debug_print(PSTR("SSL internals timed out!"), _debug_level, esp_ssl_debug_error, __func__);
#endif
            setWriteError(esp_ssl_write_error);
            stop();
            return -1;
        }
        // debug
        if (state != lastState || lastState == 0)
        {
            lastState = state;
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
            esp_ssl_debug_print(PSTR("SSL state changed."), _debug_level, esp_ssl_debug_info, __func__);
            mPrintSSLState(state, esp_ssl_debug_info, __func__);
#endif
        }
        if (state & BR_SSL_RECVREC)
        {
            size_t len;
            br_ssl_engine_recvrec_buf(_eng, &len);
            if (lastLen != len)
            {
                lastLen = len;
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
                String s = PSTR("Expected bytes count: ");
                s += len;
                esp_ssl_debug_print(s.c_str(), _debug_level, esp_ssl_debug_info, __func__);
#endif
            }
        }
        /*
         * If we reached our target, then we are finished.
         */
        if (state & target || (target == 0 && state == 0))
            return 0;

        /*
         * If some application data must be read, and we did not
         * exit, then this means that we are trying to write data,
         * and that's not possible until the application data is
         * read. This may happen if using a shared in/out buffer,
         * and the underlying protocol is not strictly half-duplex.
         * Normally this would be unrecoverable, however we can attempt
         * to remedy the problem by telling the engine to discard
         * the data.
         */
        if (state & BR_SSL_RECVAPP)
        {
            _recvapp_buf = br_ssl_engine_recvapp_buf(_eng, &_recvapp_len);
            if (_recvapp_buf != nullptr)
            {
                // if application data is ready in buffer, don't ignore, just return.

                // _write_idx = 0;
                // esp_ssl_debug_print(PSTR("Discarded unread data to favor a write operation."), _debug_level, esp_ssl_debug_warn, __func__);
                // br_ssl_engine_recvapp_ack(_eng, len);
                // continue;
                return 0;
            }
            else
            {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
                esp_ssl_debug_print(PSTR("SSL engine state is RECVAPP, however the buffer was null! (This is a problem with BearSSL internals)."), _debug_level, esp_ssl_debug_error, __func__);
#endif
                setWriteError(esp_ssl_write_error);
                stop();
                return -1;
            }
        }

        if (target & BR_SSL_SENDAPP)
        {
            // reset the write index
            _write_idx = 0;
            continue;
        }

        /*
         * We can reach that point if the target RECVAPP, and
         * the state contains SENDAPP only. This may happen with
         * a shared in/out buffer. In that case, we must flush
         * the buffered data to "make room" for a new incoming
         * record.
         */
        if (state & BR_SSL_SENDAPP && target & BR_SSL_RECVAPP)
            br_ssl_engine_flush(_eng, 0);
    }
}

unsigned BSSL_SSL_Client::mUpdateEngine()
{
    for (;;)
    {
        // get the state
        unsigned state = br_ssl_engine_current_state(_eng);
        // debug
        if (_bssl_last_state == 0 || state != _bssl_last_state)
        {
            _bssl_last_state = state;
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
            mPrintSSLState(state, esp_ssl_debug_info, __func__);
#endif
        }
        if (state & BR_SSL_CLOSED)
            return state;
        /*
         * If there is some record data to send, do it. This takes
         * precedence over everything else.
         */
        if (state & BR_SSL_SENDREC)
        {
            unsigned char *buf;
            size_t len;
            int wlen;

            buf = br_ssl_engine_sendrec_buf(_eng, &len);
            wlen = _basic_client->write(buf, len);
            _basic_client->flush();
            if (wlen <= 0)
            {
                // if the arduino client encountered an error
                if (_basic_client->getWriteError() || !_basic_client->connected())
                {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
                    esp_ssl_debug_print(PSTR("Error writing to basic client."), _debug_level, esp_ssl_debug_error, __func__);
#endif
                    setWriteError(esp_ssl_write_error);
                }
                // else presumably the socket just closed itself, so just stop the engine
                stop();
                return 0;
            }
            if (wlen > 0)
            {
                br_ssl_engine_sendrec_ack(_eng, wlen);
            }
            continue;
        }

        /*
         * If the client has specified there is client data to send, and
         * the engine is ready to handle it, send it along.
         */
        if (_write_idx > 0)
        {
            // if we've reached the point where BR_SSL_SENDAPP is off but
            // data has been written to the io buffer, something is wrong
            if (!(state & BR_SSL_SENDAPP))
            {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
                esp_ssl_debug_print(PSTR("Error _write_idx > 0 but the ssl engine is not ready for data."), _debug_level, esp_ssl_debug_error, __func__);
#endif
                setWriteError(esp_ssl_write_error);
                stop();
                return 0;
            }
            // else time to send the application data
            else if (state & BR_SSL_SENDAPP)
            {
                size_t alen;
                unsigned char *buf = br_ssl_engine_sendapp_buf(_eng, &alen);
                // engine check
                if (alen == 0 || buf == nullptr)
                {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
                    esp_ssl_debug_print(PSTR("Engine set write flag but returned null buffer."), _debug_level, esp_ssl_debug_error, __func__);
#endif
                    setWriteError(esp_ssl_write_error);
                    stop();
                    return 0;
                }
                // sanity check
                if (alen < _write_idx)
                {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
                    esp_ssl_debug_print(PSTR("Alen is less than _write_idx."), _debug_level, esp_ssl_debug_error, __func__);
#endif
                    setWriteError(esp_ssl_internal_error);
                    stop();
                    return 0;
                }
                // all good? lets send the data
                // presumably the BSSL_SSL_Client::write function has already added
                // data to *buf, so now we tell bearssl it's time for the
                // encryption step.
                // this will encrypt the data and presumably spit it out
                // for BR_SSL_SENDREC to send over ethernet.
                br_ssl_engine_sendapp_ack(_eng, _write_idx);
                // reset the iobuffer index
                _write_idx = 0;
                // loop again!
                continue;
            }
        }

        /*
         * If there is some record data to recieve, check if we've
         * recieved it so far. If we have, then we can update the state.
         * else we can return that we're still waiting for the server.
         */
        if (state & BR_SSL_RECVREC)
        {
            size_t len;
            unsigned char *buf = br_ssl_engine_recvrec_buf(_eng, &len);
            // do we have the record you're looking for?
            const auto avail = _basic_client->available();
            if (avail > 0)
            {
                // I suppose so!
                int rlen = _basic_client->read(buf, avail < (int)len ? avail : (int)len);
                if (rlen <= 0)
                {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
                    String s = PSTR("Error reading bytes from basic client. Write Error: ");
                    s += _basic_client->getWriteError();
                    esp_ssl_debug_print(s.c_str(), _debug_level, esp_ssl_debug_error, __func__);
#endif
                    setWriteError(esp_ssl_write_error);
                    stop();
                    return 0;
                }
                if (rlen > 0)
                {
                    br_ssl_engine_recvrec_ack(_eng, rlen);
                }
                continue;
            }
            // guess not, tell the state we're waiting still
            else
            {

#if defined __has_include
#if __has_include(<Ethernet.h>)
                // Add a delay since spamming _basic_client->availible breaks the poor wiz chip
                delay(10);
#endif
#endif
                return state;
            }
        }
        // if it's not any of the above states, then it must be waiting to send or recieve app data
        // in which case we return
        return state;
    }
}

void BSSL_SSL_Client::mPrintClientError(const int ssl_error, int level, const char *func_name)
{
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
    const char *msg = nullptr;

    switch (ssl_error)
    {
    case esp_ssl_ok:
        msg = PSTR("SSL OK");
        break;
    case esp_ssl_connection_fail:
        msg = PSTR("SSL Connection fail");
        break;
    case esp_ssl_write_error:
        msg = PSTR("SSL Write error");
        break;
    case esp_ssl_read_error:
        msg = PSTR("SSL Read error");
        break;
    case esp_ssl_out_of_memory:
        msg = PSTR("SSL OOM error");
        break;
    case esp_ssl_internal_error:
        msg = PSTR("SSL Internal error");
        break;
    }

    esp_ssl_debug_print(msg, _debug_level, level, func_name);
#endif
}

void BSSL_SSL_Client::mPrintSSLError(const unsigned br_error_code, int level, const char *func_name)
{
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
    char dest[300];
    getLastSSLError(dest, 300);
    esp_ssl_debug_print(dest, _debug_level, level, func_name);
#endif
}

void BSSL_SSL_Client::mPrintSSLState(const unsigned state, int level, const char *func_name)
{
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
    const char *msg = nullptr;

    if (state == 0)
        msg = PSTR("State Invalid");
    else if (state & BR_SSL_CLOSED)
        msg = PSTR("State Connection close");
    else
    {
        if (state & BR_SSL_SENDREC)
            msg = PSTR("State SENDREC");
        if (state & BR_SSL_RECVREC)
            msg = PSTR("State RECVREC");
        if (state & BR_SSL_SENDAPP)
            msg = PSTR("State SENDAPP");
        if (state & BR_SSL_RECVAPP)
            msg = PSTR("State RECVAPP");
    }

    esp_ssl_debug_print(msg, _debug_level, level, func_name);
#endif
}

bool BSSL_SSL_Client::mIsSecurePort(uint16_t port)
{
    int size = *(&_secure_ports + 1) - _secure_ports;
    for (int i = 0; i < size; i++)
    {
        if (port == _secure_ports[i])
            return true;
    }

    return false;
}

void BSSL_SSL_Client::mBSSLX509InsecureInit(bssl::br_x509_insecure_context *ctx, int _use_fingerprint, const uint8_t _fingerprint[20], int _allow_self_signed)
{
    static const br_x509_class br_x509_insecure_vtable PROGMEM = {
        sizeof(bssl::br_x509_insecure_context),
        bssl::insecure_start_chain,
        bssl::insecure_start_cert,
        bssl::insecure_append,
        bssl::insecure_end_cert,
        bssl::insecure_end_chain,
        bssl::insecure_get_pkey};

    memset(ctx, 0, sizeof *ctx);
    ctx->vtable = &br_x509_insecure_vtable;
    ctx->done_cert = false;
    ctx->match_fingerprint = _use_fingerprint ? _fingerprint : nullptr;
    ctx->allow_self_signed = _allow_self_signed ? 1 : 0;
}

void BSSL_SSL_Client::mClearAuthenticationSettings()
{
    _use_insecure = false;
    _use_fingerprint = false;
    _use_self_signed = false;
    _knownkey = nullptr;
    _ta = nullptr;
    if (_esp32_ta)
    {
        delete _esp32_ta;
        _esp32_ta = nullptr;
    }
}

void BSSL_SSL_Client::mClear()
{
    _timeout = 15000;
    _sc = nullptr;
    _eng = nullptr;
    _x509_minimal = nullptr;
    _x509_insecure = nullptr;
    _x509_knownkey = nullptr;

    freeImpl(&_iobuf_in);
    freeImpl(&_iobuf_out);
    _now = 0; // You can override or ensure time() is correct w/configTime
    _ta = nullptr;
    setBufferSizes(16384, 512); // Minimum safe
    _secure = false;
    _recvapp_buf = nullptr;
    _recvapp_len = 0;
    _oom_err = false;
    _session = nullptr;
    freeImpl(&_cipher_list);
    _cipher_cnt = 0;
    _tls_min = BR_TLS10;
    _tls_max = BR_TLS12;
    if (_esp32_ta)
    {
        delete _esp32_ta;
        _esp32_ta = nullptr;
    }
}

// X.509 validators differ from server to client
// Installs the appropriate X509 cert validation method for a client connection
bool BSSL_SSL_Client::mInstallClientX509Validator()
{

    if (_use_insecure || _use_fingerprint || _use_self_signed)
    {
        // Use common insecure x509 authenticator
        _x509_insecure = std::make_shared<struct bssl::br_x509_insecure_context>();
        if (!_x509_insecure)
        {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
            esp_ssl_debug_print(PSTR("OOM for _x509_insecure"), _debug_level, esp_ssl_debug_error, __func__);
#endif
            return false;
        }
        mBSSLX509InsecureInit(_x509_insecure.get(), _use_fingerprint, _fingerprint, _use_self_signed);
        br_ssl_engine_set_x509(_eng, &_x509_insecure->vtable);
    }
    else if (_knownkey)
    {
        // Simple, pre-known public key authenticator, ignores cert completely.
        _x509_knownkey = std::make_shared<br_x509_knownkey_context>();
        if (!_x509_knownkey)
        {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
            esp_ssl_debug_print(PSTR("OOM for _x509_knownkey"), _debug_level, esp_ssl_debug_error, __func__);
#endif
            return false;
        }
        if (_knownkey->isRSA())
        {
            br_x509_knownkey_init_rsa(_x509_knownkey.get(), _knownkey->getRSA(), _knownkey_usages);
        }
        else if (_knownkey->isEC())
        {
#ifndef BEARSSL_SSL_BASIC
            br_x509_knownkey_init_ec(_x509_knownkey.get(), _knownkey->getEC(), _knownkey_usages);
#else
            (void)_knownkey;
            (void)_knownkey_usages;
            esp_ssl_debug_print(PSTR("Attempting to use EC keys in minimal cipher mode (no EC)"), _debug_level, esp_ssl_debug_error, __func__);
            return false;
#endif
        }
        br_ssl_engine_set_x509(_eng, &_x509_knownkey->vtable);
    }
    else
    {
        // X509 minimal validator.  Checks dates, cert chain for trusted CA, etc.
        _x509_minimal = std::make_shared<br_x509_minimal_context>();
        if (!_x509_minimal)
        {
#if defined(ESP_SSLCLIENT_ENABLE_DEBUG)
            esp_ssl_debug_print(PSTR("OOM for _x509_minimal"), _debug_level, esp_ssl_debug_error, __func__);
#endif
            return false;
        }
        if (_esp32_ta)
        {
            br_x509_minimal_init(_x509_minimal.get(), &br_sha256_vtable, _esp32_ta->getTrustAnchors(), _esp32_ta->getCount());
        }
        else
        {
            br_x509_minimal_init(_x509_minimal.get(), &br_sha256_vtable, _ta ? _ta->getTrustAnchors() : nullptr, _ta ? _ta->getCount() : 0);
        }
        br_x509_minimal_set_rsa(_x509_minimal.get(), br_ssl_engine_get_rsavrfy(_eng));
#ifndef BEARSSL_SSL_BASIC
        br_x509_minimal_set_ecdsa(_x509_minimal.get(), br_ssl_engine_get_ec(_eng), br_ssl_engine_get_ecdsa(_eng));
#endif
        bssl::br_x509_minimal_install_hashes(_x509_minimal.get());

#if (defined(ESP32) || defined(ESP8266) || defined(ARDUINO_ARCH_RP2040)) && !defined(ARDUINO_NANO_RP2040_CONNECT)
        if (_now < ESP_SSLCLIENT_VALID_TIMESTAMP)
            _now = time(nullptr);
#endif
        if (_now)
        {
            // Magic constants convert to x509 times
            br_x509_minimal_set_time(_x509_minimal.get(), ((uint32_t)_now) / 86400 + 719528, ((uint32_t)_now) % 86400);
        }
#if defined(ESP_SSL_FS_SUPPORTED)
        if (_certStore)
        {
            _certStore->installCertStore(_x509_minimal.get());
        }
#endif
        br_ssl_engine_set_x509(_eng, &_x509_minimal->vtable);
    }
    return true;
}

void BSSL_SSL_Client::mFreeSSL()
{
    // These are smart pointers and will free if refcnt==0
    _sc = nullptr;
    _x509_minimal = nullptr;
    _x509_insecure = nullptr;
    _x509_knownkey = nullptr;
    freeImpl(&_iobuf_in);
    freeImpl(&_iobuf_out);
    // Reset non-allocated ptrs (pointing to bits potentially free'd above)
    _recvapp_buf = nullptr;
    _recvapp_len = 0;
    // This connection is toast
    _handshake_done = false;
    _timeout = 15000;
    _secure = false;
    _is_connected = false;
}

uint8_t *BSSL_SSL_Client::mStreamLoad(Stream &stream, size_t size)
{
    uint8_t *dest = (uint8_t *)malloc(size + 1);
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

// Allocate memory
void *BSSL_SSL_Client::mallocImpl(size_t len, bool clear)
{
    void *p;
    size_t newLen = getReservedLen(len);
#if defined(BOARD_HAS_PSRAM) && defined(ESP_SSLCLIENT_USE_PSRAM)

    if (ESP.getPsramSize() > 0)
        p = (void *)ps_malloc(newLen);
    else
        p = (void *)malloc(newLen);

    if (!p)
        return NULL;

#else

#if defined(ESP_SSLCLIENT_ESP8266_USE_EXTERNAL_HEAP)
    ESP.setExternalHeap();
#endif

    p = (void *)malloc(newLen);
    bool nn = p ? true : false;

#if defined(ESP_SSLCLIENT_ESP8266_USE_EXTERNAL_HEAP)
    ESP.resetHeap();
#endif

    if (!nn)
        return NULL;

#endif
    if (clear)
        memset(p, 0, newLen);
    return p;
}

// Free reserved memory at pointer.
void BSSL_SSL_Client::freeImpl(void *ptr)
{
    void **p = (void **)ptr;
    if (*p)
    {
        free(*p);
        *p = 0;
    }
}

size_t BSSL_SSL_Client::getReservedLen(size_t len)
{
    int blen = len + 1;

    int newlen = (blen / 4) * 4;

    if (newlen < blen)
        newlen += 4;

    return (size_t)newlen;
}

#endif

#endif