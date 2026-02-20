/**
 *
 * The ESP SSL Client Class, ESP_SSLClient.h v2.1.6
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

#ifndef ESP_SSLCLIENT_H
#define ESP_SSLCLIENT_H

#include "ESP_SSLClient_FS.h"

#ifndef SSLCLIENT_CONNECTION_UPGRADABLE
#define SSLCLIENT_CONNECTION_UPGRADABLE
#endif
#include "ESP_SSLClient_Const.h"
#if defined(USE_EMBED_SSL_ENGINE) || defined(USE_LIB_SSL_ENGINE)
#include "client/BSSL_TCP_Client.h"
class ESP_SSLClient : public BSSL_TCP_Client
{
public:
    ESP_SSLClient(){};
    ~ESP_SSLClient(){};
};

class ESP_SSLClient2 : public BSSL_TCP_Client
{
public:
    ESP_SSLClient2(Client &client, bool enableSSL = true) : _base_client(client)
    {
        setClient(&_base_client, enableSSL);
    };
    ~ESP_SSLClient2(){};

private:
    Client &_base_client;
};

#else
class ESP_SSLClient
{
public:
    ESP_SSLClient(){};
    ~ESP_SSLClient(){};
};

class ESP_SSLClient2
{
public:
    ESP_SSLClient2(Client &client, bool enableSSL = true) : _base_client(client){};
    ~ESP_SSLClient2(){};

private:
    Client &_base_client;
};
#endif

#endif
