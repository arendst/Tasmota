/*
 *Customized ssl_client.h to support STARTTLS protocol, version 1.0.3
 * 
 * The MIT License (MIT)
 * Copyright (c) 2019 K. Suwatchai (Mobizt)
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

/* Provide SSL/TLS functions to ESP32 with Arduino IDE
 * by Evandro Copercini - 2017 - Apache 2.0 License
 */

#ifndef SSL_CLIENT32_H
#define SSL_CLIENT32_H

#ifdef ESP32

#include "mbedtls/platform.h"
#include "mbedtls/net.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"

static const char ESP32_SSL_CLIENT_STR_1[] PROGMEM = "ERROR: ";
static const char ESP32_SSL_CLIENT_STR_2[] PROGMEM = "INFO: starting socket";
static const char ESP32_SSL_CLIENT_STR_3[] PROGMEM = "ERROR: opening socket";
static const char ESP32_SSL_CLIENT_STR_4[] PROGMEM = "ERROR: could not get ip from host";
static const char ESP32_SSL_CLIENT_STR_5[] PROGMEM = "INFO: connecting to Server...";
static const char ESP32_SSL_CLIENT_STR_6[] PROGMEM = "INFO: server connected";
static const char ESP32_SSL_CLIENT_STR_7[] PROGMEM = "ERROR: connect to Server failed!";
static const char ESP32_SSL_CLIENT_STR_8[] PROGMEM = "INFO: begin STARTTLS handshake";
static const char ESP32_SSL_CLIENT_STR_9[] PROGMEM = "INFO: seeding the random number generator";
static const char ESP32_SSL_CLIENT_STR_10[] PROGMEM = "INFO: setting up the SSL/TLS structure...";
static const char ESP32_SSL_CLIENT_STR_11[] PROGMEM = "INFO: loading CA cert";
static const char ESP32_SSL_CLIENT_STR_12[] PROGMEM = "INFO: setting up PSK";
static const char ESP32_SSL_CLIENT_STR_13[] PROGMEM = "ERROR: pre-shared key not valid hex or too long";
static const char ESP32_SSL_CLIENT_STR_14[] PROGMEM = "INFO: set mbedtls config";
static const char ESP32_SSL_CLIENT_STR_15[] PROGMEM = "INFO: loading CRT cert";
static const char ESP32_SSL_CLIENT_STR_16[] PROGMEM = "INFO: loading private key";
static const char ESP32_SSL_CLIENT_STR_17[] PROGMEM = "INFO: setting hostname for TLS session...";
static const char ESP32_SSL_CLIENT_STR_18[] PROGMEM = "INFO: performing the SSL/TLS handshake...";
static const char ESP32_SSL_CLIENT_STR_19[] PROGMEM = "INFO: verifying peer X.509 certificate...";
static const char ESP32_SSL_CLIENT_STR_20[] PROGMEM = "ERROR: failed to verify peer certificate!";
static const char ESP32_SSL_CLIENT_STR_21[] PROGMEM = "INFO: certificate verified";
static const char ESP32_SSL_CLIENT_STR_22[] PROGMEM = "INFO: cleaning SSL connection";
static const char ESP32_SSL_CLIENT_STR_23[] PROGMEM = "ERROR: fingerprint too short";
static const char ESP32_SSL_CLIENT_STR_24[] PROGMEM = "ERROR: invalid hex sequence";
static const char ESP32_SSL_CLIENT_STR_25[] PROGMEM = "ERROR: could not fetch peer certificate";
static const char ESP32_SSL_CLIENT_STR_26[] PROGMEM = "ERROR: fingerprint doesn't match";
static const char ESP32_SSL_CLIENT_STR_27[] PROGMEM = "ERROR: waiting incoming data failed!";
static const char ESP32_SSL_CLIENT_STR_28[] PROGMEM = "ERROR: reading incoming data failed!";
static const char ESP32_SSL_CLIENT_STR_29[] PROGMEM = "EHLO DUDE\r\n";
static const char ESP32_SSL_CLIENT_STR_30[] PROGMEM = "INFO: send SMTP command extended HELO";
static const char ESP32_SSL_CLIENT_STR_31[] PROGMEM = "ERROR: send SMTP command failed!";
static const char ESP32_SSL_CLIENT_STR_32[] PROGMEM = "ERROR: waiting incoming data failed!";
static const char ESP32_SSL_CLIENT_STR_33[] PROGMEM = "ERROR: reading incoming data failed!";
static const char ESP32_SSL_CLIENT_STR_34[] PROGMEM = "STARTTLS\r\n";
static const char ESP32_SSL_CLIENT_STR_35[] PROGMEM = "INFO: send STARTTLS protocol command";
static const char ESP32_SSL_CLIENT_STR_36[] PROGMEM = "ERROR: send STARTTLS protocol command failed!";
static const char ESP32_SSL_CLIENT_STR_37[] PROGMEM = "ERROR: waiting incoming data failed!";
static const char ESP32_SSL_CLIENT_STR_38[] PROGMEM = "ERROR: reading incoming data failed!";

typedef void (*DebugMsgCallback)(const char *msg);

typedef struct sslclient_context32 {
    int socket;
    bool starttls;
    mbedtls_ssl_context ssl_ctx;
    mbedtls_ssl_config ssl_conf;
    mbedtls_net_context server_fd;

    mbedtls_ctr_drbg_context drbg_ctx;
    mbedtls_entropy_context entropy_ctx;

    mbedtls_x509_crt ca_cert;
    mbedtls_x509_crt client_cert;
    mbedtls_pk_context client_key;
    DebugMsgCallback _debugCallback;

    unsigned long handshake_timeout;
} sslclient_context32;


void ssl_init(sslclient_context32 *ssl_client);
int start_ssl_client(sslclient_context32 *ssl_client, const char *host, uint32_t port, int timeout, const char *rootCABuff, const char *cli_cert, const char *cli_key, const char *pskIdent, const char *psKey);
void stop_ssl_socket(sslclient_context32 *ssl_client, const char *rootCABuff, const char *cli_cert, const char *cli_key);
int data_to_read(sslclient_context32 *ssl_client);
int send_ssl_data(sslclient_context32 *ssl_client, const uint8_t *data, uint16_t len);
int get_ssl_receive(sslclient_context32 *ssl_client, uint8_t *data, int length);
bool verify_ssl_fingerprint(sslclient_context32 *ssl_client, const char* fp, const char* domain_name);
bool verify_ssl_dn(sslclient_context32 *ssl_client, const char* domain_name);
int starttlsHandshake(sslclient_context32 *ssl_client, int port);
void ESP32SSLClientDebugInfo(PGM_P info, sslclient_context32 *ssl_client);

#endif //ESP32

#endif //SSL_CLIENT32_H
