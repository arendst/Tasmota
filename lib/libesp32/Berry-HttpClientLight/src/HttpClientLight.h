/**
 * HTTPClientLight.h
 *
 * Created on: 02.11.2015
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

#ifndef HTTPClient_Light_H_
#define HTTPClient_Light_H_

#define HTTPCLIENT_1_1_COMPATIBLE

#include <memory>
#include <Arduino.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>

#include <HTTPClient.h>     // import definitions from the original code

#include "tasmota_options.h"

#define HTTPCLIENT_DEFAULT_TCP_TIMEOUT (5000)

/// HTTP client errors
#define HTTPC_ERROR_CONNECTION_REFUSED  (-1)
#define HTTPC_ERROR_SEND_HEADER_FAILED  (-2)
#define HTTPC_ERROR_SEND_PAYLOAD_FAILED (-3)
#define HTTPC_ERROR_NOT_CONNECTED       (-4)
#define HTTPC_ERROR_CONNECTION_LOST     (-5)
#define HTTPC_ERROR_NO_STREAM           (-6)
#define HTTPC_ERROR_NO_HTTP_SERVER      (-7)
#define HTTPC_ERROR_TOO_LESS_RAM        (-8)
#define HTTPC_ERROR_ENCODING            (-9)
#define HTTPC_ERROR_STREAM_WRITE        (-10)
#define HTTPC_ERROR_READ_TIMEOUT        (-11)

/// size for the stream handling
#define HTTP_TCP_BUFFER_SIZE (1460)

/// HTTP codes see RFC7231
// typedef enum {
//     HTTP_CODE_CONTINUE = 100,
//     HTTP_CODE_SWITCHING_PROTOCOLS = 101,
//     HTTP_CODE_PROCESSING = 102,
//     HTTP_CODE_OK = 200,
//     HTTP_CODE_CREATED = 201,
//     HTTP_CODE_ACCEPTED = 202,
//     HTTP_CODE_NON_AUTHORITATIVE_INFORMATION = 203,
//     HTTP_CODE_NO_CONTENT = 204,
//     HTTP_CODE_RESET_CONTENT = 205,
//     HTTP_CODE_PARTIAL_CONTENT = 206,
//     HTTP_CODE_MULTI_STATUS = 207,
//     HTTP_CODE_ALREADY_REPORTED = 208,
//     HTTP_CODE_IM_USED = 226,
//     HTTP_CODE_MULTIPLE_CHOICES = 300,
//     HTTP_CODE_MOVED_PERMANENTLY = 301,
//     HTTP_CODE_FOUND = 302,
//     HTTP_CODE_SEE_OTHER = 303,
//     HTTP_CODE_NOT_MODIFIED = 304,
//     HTTP_CODE_USE_PROXY = 305,
//     HTTP_CODE_TEMPORARY_REDIRECT = 307,
//     HTTP_CODE_PERMANENT_REDIRECT = 308,
//     HTTP_CODE_BAD_REQUEST = 400,
//     HTTP_CODE_UNAUTHORIZED = 401,
//     HTTP_CODE_PAYMENT_REQUIRED = 402,
//     HTTP_CODE_FORBIDDEN = 403,
//     HTTP_CODE_NOT_FOUND = 404,
//     HTTP_CODE_METHOD_NOT_ALLOWED = 405,
//     HTTP_CODE_NOT_ACCEPTABLE = 406,
//     HTTP_CODE_PROXY_AUTHENTICATION_REQUIRED = 407,
//     HTTP_CODE_REQUEST_TIMEOUT = 408,
//     HTTP_CODE_CONFLICT = 409,
//     HTTP_CODE_GONE = 410,
//     HTTP_CODE_LENGTH_REQUIRED = 411,
//     HTTP_CODE_PRECONDITION_FAILED = 412,
//     HTTP_CODE_PAYLOAD_TOO_LARGE = 413,
//     HTTP_CODE_URI_TOO_LONG = 414,
//     HTTP_CODE_UNSUPPORTED_MEDIA_TYPE = 415,
//     HTTP_CODE_RANGE_NOT_SATISFIABLE = 416,
//     HTTP_CODE_EXPECTATION_FAILED = 417,
//     HTTP_CODE_MISDIRECTED_REQUEST = 421,
//     HTTP_CODE_UNPROCESSABLE_ENTITY = 422,
//     HTTP_CODE_LOCKED = 423,
//     HTTP_CODE_FAILED_DEPENDENCY = 424,
//     HTTP_CODE_UPGRADE_REQUIRED = 426,
//     HTTP_CODE_PRECONDITION_REQUIRED = 428,
//     HTTP_CODE_TOO_MANY_REQUESTS = 429,
//     HTTP_CODE_REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
//     HTTP_CODE_INTERNAL_SERVER_ERROR = 500,
//     HTTP_CODE_NOT_IMPLEMENTED = 501,
//     HTTP_CODE_BAD_GATEWAY = 502,
//     HTTP_CODE_SERVICE_UNAVAILABLE = 503,
//     HTTP_CODE_GATEWAY_TIMEOUT = 504,
//     HTTP_CODE_HTTP_VERSION_NOT_SUPPORTED = 505,
//     HTTP_CODE_VARIANT_ALSO_NEGOTIATES = 506,
//     HTTP_CODE_INSUFFICIENT_STORAGE = 507,
//     HTTP_CODE_LOOP_DETECTED = 508,
//     HTTP_CODE_NOT_EXTENDED = 510,
//     HTTP_CODE_NETWORK_AUTHENTICATION_REQUIRED = 511
// } t_http_codes;

// typedef enum {
//     HTTPC_TE_IDENTITY,
//     HTTPC_TE_CHUNKED
// } transferEncoding_t;

/**
 * redirection follow mode.
 * + `HTTPC_DISABLE_FOLLOW_REDIRECTS` - no redirection will be followed.
 * + `HTTPC_STRICT_FOLLOW_REDIRECTS` - strict RFC2616, only requests using
 *      GET or HEAD methods will be redirected (using the same method),
 *      since the RFC requires end-user confirmation in other cases.
 * + `HTTPC_FORCE_FOLLOW_REDIRECTS` - all redirections will be followed,
 *      regardless of a used method. New request will use the same method,
 *      and they will include the same body data and the same headers.
 *      In the sense of the RFC, it's just like every redirection is confirmed.
 */
// typedef enum {
//     HTTPC_DISABLE_FOLLOW_REDIRECTS,
//     HTTPC_STRICT_FOLLOW_REDIRECTS,
//     HTTPC_FORCE_FOLLOW_REDIRECTS
// } followRedirects_t;



class HTTPClientLight
{
public:
    HTTPClientLight();
    ~HTTPClientLight();

/*
 * Since both begin() functions take a reference to client as a parameter, you need to 
 * ensure the client object lives the entire time of the HTTPClientLight
 */
    // bool begin(WiFiClient &client, String url);
    // bool begin(WiFiClient &client, String host, uint16_t port, String uri = "/", bool https = false);

#ifdef HTTPCLIENT_1_1_COMPATIBLE
    bool begin(String url);
    bool begin(String url, const char* CAcert);
    // bool begin(String host, uint16_t port, String uri = "/");
    // bool begin(String host, uint16_t port, String uri, const char* CAcert);
    // bool begin(String host, uint16_t port, String uri, const char* CAcert, const char* cli_cert, const char* cli_key);
#endif

    void end(void);

    bool connected(void);

    void setReuse(bool reuse); /// keep-alive
    void setUserAgent(const String& userAgent);
    void setAuthorization(const char * user, const char * password);
    void setAuthorization(const char * auth);
    void setConnectTimeout(int32_t connectTimeout);
    void setTimeout(uint16_t timeout);

    // Redirections
    void setFollowRedirects(followRedirects_t follow);
    void setRedirectLimit(uint16_t limit); // max redirects to follow for a single request

    bool setURL(const String &url);
    void useHTTP10(bool usehttp10 = true);

    /// request handling
    int GET();
    int PATCH(uint8_t * payload, size_t size);
    int PATCH(String payload);
    int POST(uint8_t * payload, size_t size);
    int POST(String payload);
    int PUT(uint8_t * payload, size_t size);
    int PUT(String payload);
    int sendRequest(const char * type, String payload);
    int sendRequest(const char * type, uint8_t * payload = NULL, size_t size = 0);
    int sendRequest(const char * type, Stream * stream, size_t size = 0);

    void addHeader(const String& name, const String& value, bool first = false, bool replace = true);

    /// Response handling
    void collectHeaders(const char* headerKeys[], const size_t headerKeysCount);
    String header(const char* name);   // get request header value by name
    String header(size_t i);              // get request header value by number
    String headerName(size_t i);          // get request header name by number
    int headers();                     // get header count
    bool hasHeader(const char* name);  // check if header exists


    int getSize(void);
    const String &getLocation(void);

    WiFiClient& getStream(void);
    WiFiClient* getStreamPtr(void);
    int writeToStream(Stream* stream);
    String getString(void);

    static String errorToString(int error);

protected:
    struct RequestArgument {
        String key;
        String value;
    };

    bool beginInternal(String url, const char* expectedProtocol);
    void disconnect(bool preserveClient = false);
    void clear();
    int returnError(int error);
    bool connect(void);
    bool sendHeader(const char * type);
    int handleHeaderResponse();
    int writeToStreamDataBlock(Stream * stream, int len);


#ifdef HTTPCLIENT_1_1_COMPATIBLE
    TransportTraitsPtr _transportTraits;
    std::unique_ptr<WiFiClient> _tcpDeprecated;
#endif

    WiFiClient* _client = nullptr;

    /// request handling
    String _host;
    uint16_t _port = 0;
    int32_t _connectTimeout = -1;
    bool _reuse = true;
    uint16_t _tcpTimeout = HTTPCLIENT_DEFAULT_TCP_TIMEOUT;
    bool _useHTTP10 = false;
    bool _secure = false;

    String _uri;
    String _protocol;
    String _headers;
    String _userAgent = "ESP32HTTPClient";
    String _base64Authorization;

    /// Response handling
    RequestArgument* _currentHeaders = nullptr;
    size_t           _headerKeysCount = 0;

    int _returnCode = 0;
    int _size = -1;
    bool _canReuse = false;
    followRedirects_t _followRedirects = HTTPC_DISABLE_FOLLOW_REDIRECTS;
    uint16_t _redirectLimit = 10;
    String _location;
    transferEncoding_t _transferEncoding = HTTPC_TE_IDENTITY;
};



#endif /* HTTPClient_Light_H_ */