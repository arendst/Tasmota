/*
 *Customized ssl_client.h to support STARTTLS protocol, version 1.0.5
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

/* Provide SSL/TLS functions to ESP32 with Arduino IDE
 * by Evandro Copercini - 2017 - Apache 2.0 License
 */

#ifndef ESP_MAIL_SSL_CLIENT32_H
#define ESP_MAIL_SSL_CLIENT32_H

#ifdef ESP32

#include <mbedtls/platform.h>
#include <mbedtls/net.h>
#include <mbedtls/debug.h>
#include <mbedtls/ssl.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/error.h>

static const char esp_ssl_client_str_1[] PROGMEM = "! E: ";
static const char esp_ssl_client_str_2[] PROGMEM = "> C: starting socket";
static const char esp_ssl_client_str_3[] PROGMEM = "! E: opening socket";
static const char esp_ssl_client_str_4[] PROGMEM = "! E: could not get ip from host";
static const char esp_ssl_client_str_5[] PROGMEM = "> C: connecting to Server";
static const char esp_ssl_client_str_6[] PROGMEM = "> C: server connected";
static const char esp_ssl_client_str_7[] PROGMEM = "! E: connect to Server failed!";
static const char esp_ssl_client_str_8[] PROGMEM = "< S: ";
static const char esp_ssl_client_str_9[] PROGMEM = "> C: seeding the random number generator";
static const char esp_ssl_client_str_10[] PROGMEM = "> C: setting up the SSL/TLS structure";
static const char esp_ssl_client_str_11[] PROGMEM = "> C: loading CA cert";
static const char esp_ssl_client_str_12[] PROGMEM = "> C: setting up PSK";
static const char esp_ssl_client_str_13[] PROGMEM = "! E: pre-shared key not valid hex or too long";
static const char esp_ssl_client_str_14[] PROGMEM = "> C: set mbedtls config";
static const char esp_ssl_client_str_15[] PROGMEM = "> C: loading CRT cert";
static const char esp_ssl_client_str_16[] PROGMEM = "> C: loading private key";
static const char esp_ssl_client_str_17[] PROGMEM = "> C: setting hostname for TLS session";
static const char esp_ssl_client_str_18[] PROGMEM = "> C: performing the SSL handshaking";
static const char esp_ssl_client_str_19[] PROGMEM = "> C: verifying peer X.509 certificate";
static const char esp_ssl_client_str_20[] PROGMEM = "! E: failed to verify peer certificate!";
static const char esp_ssl_client_str_21[] PROGMEM = "> C: certificate verified";
static const char esp_ssl_client_str_22[] PROGMEM = "> C: cleaning SSL connection";
static const char esp_ssl_client_str_23[] PROGMEM = "!E: fingerprint too short";
static const char esp_ssl_client_str_24[] PROGMEM = "!E: invalid hex sequence";
static const char esp_ssl_client_str_25[] PROGMEM = "!E: could not fetch peer certificate";
static const char esp_ssl_client_str_26[] PROGMEM = "!E: fingerprint doesn't match";

typedef void (*DebugMsgCallback)(const char *msg);

typedef struct esp_mail_ssl_ctx32
{
    int socket;
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
} esp_mail_ssl_ctx32;

void ssl_init(esp_mail_ssl_ctx32 *ssl_client);
int start_socket(esp_mail_ssl_ctx32 *ssl_client, const char *host, uint32_t port, int timeout);
int start_ssl_client(esp_mail_ssl_ctx32 *ssl_client, const char *host, uint32_t port, int timeout, const char *rootCABuff, const char *cli_cert, const char *cli_key, const char *pskIdent, const char *psKey);
void stop_ssl_socket(esp_mail_ssl_ctx32 *ssl_client, const char *rootCABuff, const char *cli_cert, const char *cli_key);
int data_to_read(esp_mail_ssl_ctx32 *ssl_client);
int send_ssl_data(esp_mail_ssl_ctx32 *ssl_client, const uint8_t *data, uint16_t len);
int get_ssl_receive(esp_mail_ssl_ctx32 *ssl_client, uint8_t *data, int length);
bool verify_ssl_fingerprint(esp_mail_ssl_ctx32 *ssl_client, const char *fp, const char *domain_name);
bool verify_ssl_dn(esp_mail_ssl_ctx32 *ssl_client, const char *domain_name);
int _ns_lwip_write(esp_mail_ssl_ctx32 *ssl_client, const char *buf, int bufLen);
int _ns_lwip_read(esp_mail_ssl_ctx32 *ssl_client, char *buf, int bufLen);
void ssl_client_debug_pgm_send_cb(esp_mail_ssl_ctx32 *ssl_client, PGM_P info);

#endif //ESP32

#endif //SSL_CLIENT32_H
