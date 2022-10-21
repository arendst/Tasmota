/**
 * HTTPClientLight.cpp
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
 * Adapted in October 2018
 */

#ifdef ESP32
#include "tasmota_options.h"

#include <Arduino.h>

#ifdef HTTPCLIENT_1_1_COMPATIBLE
#include <WiFi.h>
#include <WiFiClientSecure.h>
#endif

#include <StreamString.h>
#include <base64.h>

#include "HttpClientLight.h"

#ifdef USE_WEBCLIENT_HTTPS
#include "WiFiClientSecureLightBearSSL.h"
#endif // USE_WEBCLIENT_HTTPS

// #ifdef HTTPCLIENT_1_1_COMPATIBLE
class TransportTraitsLight
{
public:
    virtual ~TransportTraitsLight()
    {
    }

    virtual std::unique_ptr<WiFiClient> create()
    {
        return std::unique_ptr<WiFiClient>(new WiFiClient());
    }

    virtual bool verify(WiFiClient& client, const char* host)
    {
        return true;
    }

    virtual int32_t getLastError(WiFiClient& client)
    {
        return 0;
    }

};

#ifdef USE_WEBCLIENT_HTTPS
static const uint8_t _fingerprint_any[] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
class BearSSLTraits : public TransportTraitsLight
{
public:
    BearSSLTraits(int recv, int xmit) :
        _recv(recv), _xmit(xmit)
    {
    }
    
    std::unique_ptr<WiFiClient> create() override
    {
        std::unique_ptr<WiFiClient> p = std::unique_ptr<WiFiClient>(new BearSSL::WiFiClientSecure_light(_recv, _xmit));
        return p;
    }

    bool verify(WiFiClient& client, const char* host) override
    {
        BearSSL::WiFiClientSecure_light& wcs = static_cast<BearSSL::WiFiClientSecure_light&>(client);
        wcs.setPubKeyFingerprint(_fingerprint_any, _fingerprint_any, true); // allow all fingerprints
        return true;
    }

    int32_t getLastError(WiFiClient& client)
    {
        BearSSL::WiFiClientSecure_light& wcs = static_cast<BearSSL::WiFiClientSecure_light&>(client);
        return wcs.getLastError();
    }


protected:
    int _recv;
    int _xmit;
};
#endif // USE_WEBCLIENT_HTTPS

/**
 * constructor
 */
HTTPClientLight::HTTPClientLight()
{
}

/**
 * destructor
 */
HTTPClientLight::~HTTPClientLight()
{
    if(_client) {
        _client->stop();
    }
    if(_currentHeaders) {
        delete[] _currentHeaders;
    }
    if(_tcpDeprecated) {
        _tcpDeprecated.reset(nullptr);
    }
    if(_transportTraits) {
        _transportTraits.reset(nullptr);
    }
}

void HTTPClientLight::clear()
{
    _returnCode = 0;
    _size = -1;
    _headers = "";
}


/**
 * parsing the url for all needed parameters
 * @param client Client&
 * @param url String
 * @param https bool
 * @return success bool
 */
// bool HTTPClientLight::begin(WiFiClient &client, String url) {
// #ifdef HTTPCLIENT_1_1_COMPATIBLE
//     if(_tcpDeprecated) {
//         log_d("mix up of new and deprecated api");
//         _canReuse = false;
//         end();
//     }
// #endif

//     _client = &client;

//     // check for : (http: or https:)
//     int index = url.indexOf(':');
//     if(index < 0) {
//         log_d("failed to parse protocol");
//         return false;
//     }

//     String protocol = url.substring(0, index);
//     if(protocol != "http" && protocol != "https") {
//         log_d("unknown protocol '%s'", protocol.c_str());
//         return false;
//     }

//     _port = (protocol == "https" ? 443 : 80);
//     return beginInternal(url, protocol.c_str());
// }


/**
 * directly supply all needed parameters
 * @param client Client&
 * @param host String
 * @param port uint16_t
 * @param uri String
 * @param https bool
 * @return success bool
 */
// bool HTTPClientLight::begin(WiFiClient &client, String host, uint16_t port, String uri, bool https)
// {
// #ifdef HTTPCLIENT_1_1_COMPATIBLE
//     if(_tcpDeprecated) {
//         log_d("mix up of new and deprecated api");
//         _canReuse = false;
//         end();
//     }
// #endif

//     _client = &client;

//      clear();
//     _host = host;
//     _port = port;
//     _uri = uri;
//     _protocol = (https ? "https" : "http");
//     return true;
// }


#ifdef HTTPCLIENT_1_1_COMPATIBLE
bool HTTPClientLight::begin(String url, const char* CAcert)
{
    if(_client && !_tcpDeprecated) {
        log_d("mix up of new and deprecated api");
        _canReuse = false;
        end();
    }

    _port = 443;
    if (!beginInternal(url, "https")) {
        return false;
    }
    _secure = true;
#ifdef USE_WEBCLIENT_HTTPS
    _transportTraits = TransportTraitsLightPtr(new BearSSLTraits(16384, 0));
    // set buffer to 16KB half duplex, so we won't lose responses bigger than 16KB
    // half duplex is well suited for HTTPS: one request followed by responses
#else
    _transportTraits = nullptr;
#endif
    if(!_transportTraits) {
        log_e("could not create transport traits");
        return false;
    }

    return true;
}

/**
 * parsing the url for all needed parameters
 * @param url String
 */
bool HTTPClientLight::begin(String url)
{
    if(_client && !_tcpDeprecated) {
        log_d("mix up of new and deprecated api");
        _canReuse = false;
        end();
    }

    _port = 80;
    if (!beginInternal(url, "http")) {
        return begin(url, (const char*)NULL);
    }
    _transportTraits = TransportTraitsLightPtr(new TransportTraitsLight());
    if(!_transportTraits) {
        log_e("could not create transport traits");
        return false;
    }

    return true;
}
#endif // HTTPCLIENT_1_1_COMPATIBLE

bool HTTPClientLight::beginInternal(String url, const char* expectedProtocol)
{
    log_v("url: %s", url.c_str());
    clear();

    // check for : (http: or https:
    int index = url.indexOf(':');
    if(index < 0) {
        log_e("failed to parse protocol");
        return false;
    }

    _protocol = url.substring(0, index);
    if (_protocol != expectedProtocol) {
        log_w("unexpected protocol: %s, expected %s", _protocol.c_str(), expectedProtocol);
        return false;
    }

    url.remove(0, (index + 3)); // remove http:// or https://

    index = url.indexOf('/');
    String host = url.substring(0, index);
    url.remove(0, index); // remove host part

    // get Authorization
    index = host.indexOf('@');
    if(index >= 0) {
        // auth info
        String auth = host.substring(0, index);
        host.remove(0, index + 1); // remove auth part including @
        _base64Authorization = base64::encode(auth);
    }

    // get port
    index = host.indexOf(':');
    String the_host;
    if(index >= 0) {
        the_host = host.substring(0, index); // hostname
        host.remove(0, (index + 1)); // remove hostname + :
        _port = host.toInt(); // get port
    } else {
        the_host = host;
    }
    if(_host != the_host && connected()){
        log_d("switching host from '%s' to '%s'. disconnecting first", _host.c_str(), the_host.c_str());
        _canReuse = false;
        disconnect(true);
    }
    _host = the_host;
    _uri = url;
    log_d("protocol: %s, host: %s port: %d url: %s", _protocol.c_str(), _host.c_str(), _port, _uri.c_str());
    return true;
}

#ifdef HTTPCLIENT_1_1_COMPATIBLE
// bool HTTPClientLight::begin(String host, uint16_t port, String uri)
// {
//     if(_client && !_tcpDeprecated) {
//         log_d("mix up of new and deprecated api");
//         _canReuse = false;
//         end();
//     }

//     clear();
//     _host = host;
//     _port = port;
//     _uri = uri;
//     // _transportTraits = TransportTraitsLightPtr(new TransportTraitsLight());  TODO
//     _transportTraits = nullptr;
//     log_d("host: %s port: %d uri: %s", host.c_str(), port, uri.c_str());
//     return true;
// }

// bool HTTPClientLight::begin(String host, uint16_t port, String uri, const char* CAcert)
// {
//     if(_client && !_tcpDeprecated) {
//         log_d("mix up of new and deprecated api");
//         _canReuse = false;
//         end();
//     }

//     clear();
//     _host = host;
//     _port = port;
//     _uri = uri;

//     if (strlen(CAcert) == 0) {
//         return false;
//     }
//     _secure = true;
//     _transportTraits = TransportTraitsLightPtr(new TLSTraits(CAcert));
//     return true;
// }

// bool HTTPClientLight::begin(String host, uint16_t port, String uri, const char* CAcert, const char* cli_cert, const char* cli_key)
// {
//     if(_client && !_tcpDeprecated) {
//         log_d("mix up of new and deprecated api");
//         _canReuse = false;
//         end();
//     }

//     clear();
//     _host = host;
//     _port = port;
//     _uri = uri;

//     if (strlen(CAcert) == 0) {
//         return false;
//     }
//     _secure = true;
//     _transportTraits = TransportTraitsLightPtr(new TLSTraits(CAcert, cli_cert, cli_key));
//     return true;
// }
#endif // HTTPCLIENT_1_1_COMPATIBLE

/**
 * end
 * called after the payload is handled
 */
void HTTPClientLight::end(void)
{
    disconnect(false);
    _tcpDeprecated.reset(nullptr);  // release the TLS implementation
    _client = _tcpDeprecated.get();  // update client
    clear();
}



/**
 * disconnect
 * close the TCP socket
 */
void HTTPClientLight::disconnect(bool preserveClient)
{
    if(connected()) {
        if(_client->available() > 0) {
            log_d("still data in buffer (%d), clean up.\n", _client->available());
            while(_client->available() > 0) {
                _client->read();
            }
        }

        if(_reuse && _canReuse) {
            log_d("tcp keep open for reuse");
        } else {
            log_d("tcp stop");
            _client->stop();
            if(!preserveClient) {
                _client = nullptr;
#ifdef HTTPCLIENT_1_1_COMPATIBLE
                if(_tcpDeprecated) {
                    _transportTraits.reset(nullptr);
                    _tcpDeprecated.reset(nullptr);
                    _client = _tcpDeprecated.get();  // update client
                }
#endif
            }
        }
    } else {
        log_d("tcp is closed\n");
    }
}


/**
 * connected
 * @return connected status
 */
bool HTTPClientLight::connected()
{
    if(_client) {
        return ((_client->available() > 0) || _client->connected());
    }
    return false;
}

/**
 * try to reuse the connection to the server
 * keep-alive
 * @param reuse bool
 */
void HTTPClientLight::setReuse(bool reuse)
{
    _reuse = reuse;
}

/**
 * set User Agent
 * @param userAgent const char *
 */
void HTTPClientLight::setUserAgent(const String& userAgent)
{
    _userAgent = userAgent;
}

/**
 * set the Authorizatio for the http request
 * @param user const char *
 * @param password const char *
 */
void HTTPClientLight::setAuthorization(const char * user, const char * password)
{
    if(user && password) {
        String auth = user;
        auth += ":";
        auth += password;
        _base64Authorization = base64::encode(auth);
    }
}

/**
 * set the Authorizatio for the http request
 * @param auth const char * base64
 */
void HTTPClientLight::setAuthorization(const char * auth)
{
    if(auth) {
        _base64Authorization = auth;
    }
}

/**
 * set the timeout (ms) for establishing a connection to the server
 * @param connectTimeout int32_t
 */
void HTTPClientLight::setConnectTimeout(int32_t connectTimeout)
{
    _connectTimeout = connectTimeout;
}

/**
 * set the timeout for the TCP connection
 * @param timeout unsigned int
 */
void HTTPClientLight::setTimeout(uint16_t timeout)
{
    _tcpTimeout = timeout;
    if(connected()) {
        _client->setTimeout((timeout + 500) / 1000);
    }
}

/**
 * use HTTP1.0
 * @param use
 */
void HTTPClientLight::useHTTP10(bool useHTTP10)
{
    _useHTTP10 = useHTTP10;
    _reuse = !useHTTP10;
}

/**
 * send a GET request
 * @return http code
 */
int HTTPClientLight::GET()
{
    return sendRequest("GET");
}

/**
 * sends a post request to the server
 * @param payload uint8_t *
 * @param size size_t
 * @return http code
 */
int HTTPClientLight::POST(uint8_t * payload, size_t size)
{
    return sendRequest("POST", payload, size);
}

int HTTPClientLight::POST(String payload)
{
    return POST((uint8_t *) payload.c_str(), payload.length());
}

/**
 * sends a patch request to the server
 * @param payload uint8_t *
 * @param size size_t
 * @return http code
 */
int HTTPClientLight::PATCH(uint8_t * payload, size_t size)
{
    return sendRequest("PATCH", payload, size);
}

int HTTPClientLight::PATCH(String payload)
{
    return PATCH((uint8_t *) payload.c_str(), payload.length());
}

/**
 * sends a put request to the server
 * @param payload uint8_t *
 * @param size size_t
 * @return http code
 */
int HTTPClientLight::PUT(uint8_t * payload, size_t size) {
    return sendRequest("PUT", payload, size);
}

int HTTPClientLight::PUT(String payload) {
    return PUT((uint8_t *) payload.c_str(), payload.length());
}

/**
 * sendRequest
 * @param type const char *     "GET", "POST", ....
 * @param payload String        data for the message body
 * @return
 */
int HTTPClientLight::sendRequest(const char * type, String payload)
{
    return sendRequest(type, (uint8_t *) payload.c_str(), payload.length());
}

/**
 * sendRequest
 * @param type const char *     "GET", "POST", ....
 * @param payload uint8_t *     data for the message body if null not send
 * @param size size_t           size for the message body if 0 not send
 * @return -1 if no info or > 0 when Content-Length is set by server
 */
int HTTPClientLight::sendRequest(const char * type, uint8_t * payload, size_t size)
{
    int code;
    bool redirect = false;
    uint16_t redirectCount = 0;
    do {
        // wipe out any existing headers from previous request
        for(size_t i = 0; i < _headerKeysCount; i++) {
            if (_currentHeaders[i].value.length() > 0) {
                _currentHeaders[i].value.clear();
            }
        }

        log_d("request type: '%s' redirCount: %d\n", type, redirectCount);
        
        // connect to server
        if(!connect()) {
            if (_secure) {
                int32_t ret = _transportTraits->getLastError(*_client);
                if (ret) {
                    return returnError(-1000 - ret); // BearSSL error 46 transformed to -1046
                }
            }
            return returnError(HTTPC_ERROR_CONNECTION_REFUSED);
        }

        if(payload && size > 0) {
            addHeader(F("Content-Length"), String(size));
        }

        // send Header
        if(!sendHeader(type)) {
            return returnError(HTTPC_ERROR_SEND_HEADER_FAILED);
        }

        // send Payload if needed
        if(payload && size > 0) {
            if(_client->write(&payload[0], size) != size) {
                return returnError(HTTPC_ERROR_SEND_PAYLOAD_FAILED);
            }
        }

        code = handleHeaderResponse();
        log_d("sendRequest code=%d\n", code);

        // Handle redirections as stated in RFC document:
        // https://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
        //
        // Implementing HTTP_CODE_FOUND as redirection with GET method,
        // to follow most of existing user agent implementations.
        //
        redirect = false;
        if (
            _followRedirects != HTTPC_DISABLE_FOLLOW_REDIRECTS && 
            redirectCount < _redirectLimit &&
            _location.length() > 0
        ) {
            switch (code) {
                // redirecting using the same method
                case HTTP_CODE_MOVED_PERMANENTLY:
                case HTTP_CODE_TEMPORARY_REDIRECT: {
                    if (
                        // allow to force redirections on other methods
                        // (the RFC require user to accept the redirection)
                        _followRedirects == HTTPC_FORCE_FOLLOW_REDIRECTS ||
                        // allow GET and HEAD methods without force
                        !strcmp(type, "GET") || 
                        !strcmp(type, "HEAD")
                    ) {
                        redirectCount += 1;
                        log_d("following redirect (the same method): '%s' redirCount: %d\n", _location.c_str(), redirectCount);
                        if (!setURL(_location)) {
                            log_d("failed setting URL for redirection\n");
                            // no redirection
                            break;
                        }
                        // redirect using the same request method and payload, diffrent URL
                        redirect = true;
                    }
                    break;
                }
                // redirecting with method dropped to GET or HEAD
                // note: it does not need `HTTPC_FORCE_FOLLOW_REDIRECTS` for any method
                case HTTP_CODE_FOUND:
                case HTTP_CODE_SEE_OTHER: {
                    redirectCount += 1;
                    log_d("following redirect (dropped to GET/HEAD): '%s' redirCount: %d\n", _location.c_str(), redirectCount);
                    if (!setURL(_location)) {
                        log_d("failed setting URL for redirection\n");
                        // no redirection
                        break;
                    }
                    // redirect after changing method to GET/HEAD and dropping payload
                    type = "GET";
                    payload = nullptr;
                    size = 0;
                    redirect = true;
                    break;
                }

                default:
                    break;
            }
        }

    } while (redirect);
    // handle Server Response (Header)
    return returnError(code);
}

/**
 * sendRequest
 * @param type const char *     "GET", "POST", ....
 * @param stream Stream *       data stream for the message body
 * @param size size_t           size for the message body if 0 not Content-Length is send
 * @return -1 if no info or > 0 when Content-Length is set by server
 */
int HTTPClientLight::sendRequest(const char * type, Stream * stream, size_t size)
{

    if(!stream) {
        return returnError(HTTPC_ERROR_NO_STREAM);
    }

    // connect to server
    if(!connect()) {
        return returnError(HTTPC_ERROR_CONNECTION_REFUSED);
    }

    if(size > 0) {
        addHeader("Content-Length", String(size));
    }

    // send Header
    if(!sendHeader(type)) {
        return returnError(HTTPC_ERROR_SEND_HEADER_FAILED);
    }

    int buff_size = HTTP_TCP_BUFFER_SIZE;

    int len = size;
    int bytesWritten = 0;

    if(len == 0) {
        len = -1;
    }

    // if possible create smaller buffer then HTTP_TCP_BUFFER_SIZE
    if((len > 0) && (len < HTTP_TCP_BUFFER_SIZE)) {
        buff_size = len;
    }

    // create buffer for read
    uint8_t * buff = (uint8_t *) malloc(buff_size);

    if(buff) {
        // read all data from stream and send it to server
        while(connected() && (stream->available() > -1) && (len > 0 || len == -1)) {

            // get available data size
            int sizeAvailable = stream->available();

            if(sizeAvailable) {

                int readBytes = sizeAvailable;

                // read only the asked bytes
                if(len > 0 && readBytes > len) {
                    readBytes = len;
                }

                // not read more the buffer can handle
                if(readBytes > buff_size) {
                    readBytes = buff_size;
                }

                // read data
                int bytesRead = stream->readBytes(buff, readBytes);

                // write it to Stream
                int bytesWrite = _client->write((const uint8_t *) buff, bytesRead);
                bytesWritten += bytesWrite;

                // are all Bytes a writen to stream ?
                if(bytesWrite != bytesRead) {
                    log_d("short write, asked for %d but got %d retry...", bytesRead, bytesWrite);

                    // check for write error
                    if(_client->getWriteError()) {
                        log_d("stream write error %d", _client->getWriteError());

                        //reset write error for retry
                        _client->clearWriteError();
                    }

                    // some time for the stream
                    delay(1);

                    int leftBytes = (readBytes - bytesWrite);

                    // retry to send the missed bytes
                    bytesWrite = _client->write((const uint8_t *) (buff + bytesWrite), leftBytes);
                    bytesWritten += bytesWrite;

                    if(bytesWrite != leftBytes) {
                        // failed again
                        log_d("short write, asked for %d but got %d failed.", leftBytes, bytesWrite);
                        free(buff);
                        return returnError(HTTPC_ERROR_SEND_PAYLOAD_FAILED);
                    }
                }

                // check for write error
                if(_client->getWriteError()) {
                    log_d("stream write error %d", _client->getWriteError());
                    free(buff);
                    return returnError(HTTPC_ERROR_SEND_PAYLOAD_FAILED);
                }

                // count bytes to read left
                if(len > 0) {
                    len -= readBytes;
                }

                delay(0);
            } else {
                delay(1);
            }
        }

        free(buff);

        if(size && (int) size != bytesWritten) {
            log_d("Stream payload bytesWritten %d and size %d mismatch!.", bytesWritten, size);
            log_d("ERROR SEND PAYLOAD FAILED!");
            return returnError(HTTPC_ERROR_SEND_PAYLOAD_FAILED);
        } else {
            log_d("Stream payload written: %d", bytesWritten);
        }

    } else {
        log_d("too less ram! need %d", HTTP_TCP_BUFFER_SIZE);
        return returnError(HTTPC_ERROR_TOO_LESS_RAM);
    }

    // handle Server Response (Header)
    return returnError(handleHeaderResponse());
}

/**
 * size of message body / payload
 * @return -1 if no info or > 0 when Content-Length is set by server
 */
int HTTPClientLight::getSize(void)
{
    return _size;
}

/**
 * returns the stream of the tcp connection
 * @return WiFiClient
 */
WiFiClient& HTTPClientLight::getStream(void)
{
    if (connected()) {
        return *_client;
    }

    log_w("getStream: not connected");
    static WiFiClient empty;
    return empty;
}

/**
 * returns a pointer to the stream of the tcp connection
 * @return WiFiClient*
 */
WiFiClient* HTTPClientLight::getStreamPtr(void)
{
    if(connected()) {
        return _client;
    }

    log_w("getStreamPtr: not connected");
    return nullptr;
}

/**
 * write all  message body / payload to Stream
 * @param stream Stream *
 * @return bytes written ( negative values are error codes )
 */
int HTTPClientLight::writeToStream(Stream * stream)
{

    if(!stream) {
        return returnError(HTTPC_ERROR_NO_STREAM);
    }

    if(!connected()) {
        return returnError(HTTPC_ERROR_NOT_CONNECTED);
    }

    // get length of document (is -1 when Server sends no Content-Length header)
    int len = _size;
    int ret = 0;

    if(_transferEncoding == HTTPC_TE_IDENTITY) {
        ret = writeToStreamDataBlock(stream, len);

        // have we an error?
        if(ret < 0) {
            return returnError(ret);
        }
    } else if(_transferEncoding == HTTPC_TE_CHUNKED) {
        int size = 0;
        while(1) {
            if(!connected()) {
                return returnError(HTTPC_ERROR_CONNECTION_LOST);
            }
            String chunkHeader = _client->readStringUntil('\n');

            if(chunkHeader.length() <= 0) {
                return returnError(HTTPC_ERROR_READ_TIMEOUT);
            }

            chunkHeader.trim(); // remove \r

            // read size of chunk
            len = (uint32_t) strtol((const char *) chunkHeader.c_str(), NULL, 16);
            size += len;
            log_d(" read chunk len: %d", len);

            // data left?
            if(len > 0) {
                int r = writeToStreamDataBlock(stream, len);
                if(r < 0) {
                    // error in writeToStreamDataBlock
                    return returnError(r);
                }
                ret += r;
            } else {

                // if no length Header use global chunk size
                if(_size <= 0) {
                    _size = size;
                }

                // check if we have write all data out
                if(ret != _size) {
                    return returnError(HTTPC_ERROR_STREAM_WRITE);
                }
                break;
            }

            // read trailing \r\n at the end of the chunk
            char buf[2];
            auto trailing_seq_len = _client->readBytes((uint8_t*)buf, 2);
            if (trailing_seq_len != 2 || buf[0] != '\r' || buf[1] != '\n') {
                return returnError(HTTPC_ERROR_READ_TIMEOUT);
            }

            delay(0);
        }
    } else {
        return returnError(HTTPC_ERROR_ENCODING);
    }

//    end();
    disconnect(true);
    return ret;
}

/**
 * return all payload as String (may need lot of ram or trigger out of memory!)
 * @return String
 */
String HTTPClientLight::getString(void)
{
    // _size can be -1 when Server sends no Content-Length header
    if(_size > 0 || _size == -1) {
        StreamString sstring;
        // try to reserve needed memory (noop if _size == -1)
        if(sstring.reserve((_size + 1))) {
            writeToStream(&sstring);
            return sstring;            
        } else {
            log_d("not enough memory to reserve a string! need: %d", (_size + 1));
        }
    }

    return "";
}

/**
 * converts error code to String
 * @param error int
 * @return String
 */
String HTTPClientLight::errorToString(int error)
{
    switch(error) {
    case HTTPC_ERROR_CONNECTION_REFUSED:
        return F("connection refused");
    case HTTPC_ERROR_SEND_HEADER_FAILED:
        return F("send header failed");
    case HTTPC_ERROR_SEND_PAYLOAD_FAILED:
        return F("send payload failed");
    case HTTPC_ERROR_NOT_CONNECTED:
        return F("not connected");
    case HTTPC_ERROR_CONNECTION_LOST:
        return F("connection lost");
    case HTTPC_ERROR_NO_STREAM:
        return F("no stream");
    case HTTPC_ERROR_NO_HTTP_SERVER:
        return F("no HTTP server");
    case HTTPC_ERROR_TOO_LESS_RAM:
        return F("too less ram");
    case HTTPC_ERROR_ENCODING:
        return F("Transfer-Encoding not supported");
    case HTTPC_ERROR_STREAM_WRITE:
        return F("Stream write error");
    case HTTPC_ERROR_READ_TIMEOUT:
        return F("read Timeout");
    default:
        return String();
    }
}

/**
 * adds Header to the request
 * @param name
 * @param value
 * @param first
 */
void HTTPClientLight::addHeader(const String& name, const String& value, bool first, bool replace)
{
    // not allow set of Header handled by code
    if(!name.equalsIgnoreCase(F("Connection")) &&
       !name.equalsIgnoreCase(F("User-Agent")) &&
       !name.equalsIgnoreCase(F("Host")) &&
       !(name.equalsIgnoreCase(F("Authorization")) && _base64Authorization.length())){

        String headerLine = name;
        headerLine += ": ";

        if (replace) {
            int headerStart = _headers.indexOf(headerLine);
            if (headerStart != -1 && (headerStart == 0 || _headers[headerStart - 1] == '\n')) {
                int headerEnd = _headers.indexOf('\n', headerStart);
                _headers = _headers.substring(0, headerStart) + _headers.substring(headerEnd + 1);
            }
        }

        headerLine += value;
        headerLine += "\r\n";
        if(first) {
            _headers = headerLine + _headers;
        } else {
            _headers += headerLine;
        }
    }
}

void HTTPClientLight::collectHeaders(const char* headerKeys[], const size_t headerKeysCount)
{
    _headerKeysCount = headerKeysCount;
    if(_currentHeaders) {
        delete[] _currentHeaders;
    }
    _currentHeaders = new RequestArgument[_headerKeysCount];
    for(size_t i = 0; i < _headerKeysCount; i++) {
        _currentHeaders[i].key = headerKeys[i];
    }
}

String HTTPClientLight::header(const char* name)
{
    for(size_t i = 0; i < _headerKeysCount; ++i) {
        if(_currentHeaders[i].key == name) {
            return _currentHeaders[i].value;
        }
    }
    return String();
}

String HTTPClientLight::header(size_t i)
{
    if(i < _headerKeysCount) {
        return _currentHeaders[i].value;
    }
    return String();
}

String HTTPClientLight::headerName(size_t i)
{
    if(i < _headerKeysCount) {
        return _currentHeaders[i].key;
    }
    return String();
}

int HTTPClientLight::headers()
{
    return _headerKeysCount;
}

bool HTTPClientLight::hasHeader(const char* name)
{
    for(size_t i = 0; i < _headerKeysCount; ++i) {
        if((_currentHeaders[i].key == name) && (_currentHeaders[i].value.length() > 0)) {
            return true;
        }
    }
    return false;
}

/**
 * init TCP connection and handle ssl verify if needed
 * @return true if connection is ok
 */
bool HTTPClientLight::connect(void)
{
    if(connected()) {
        if(_reuse) {
            log_d("already connected, reusing connection");
        } else {
            log_d("already connected, try reuse!");
        }
        while(_client->available() > 0) {
            _client->read();
        }
        return true;
    }

#ifdef HTTPCLIENT_1_1_COMPATIBLE
     if(_transportTraits && !_client) {
        _tcpDeprecated = _transportTraits->create();
        if(!_tcpDeprecated) {
            log_e("failed to create client");
            return false;
        }
        _client = _tcpDeprecated.get();
     }
#endif

    if (!_client) {
        log_d("HTTPClientLight::begin was not called or returned error");
        return false;
    }
#ifdef HTTPCLIENT_1_1_COMPATIBLE
    if (_tcpDeprecated && !_transportTraits->verify(*_client, _host.c_str())) {
        log_d("transport level verify failed");
        _client->stop();
        return false;
    }	
#endif
    if(!_client->connect(_host.c_str(), _port, _connectTimeout)) {
        log_d("failed connect to %s:%u", _host.c_str(), _port);
        return false;
    }

    // set Timeout for WiFiClient and for Stream::readBytesUntil() and Stream::readStringUntil()
    _client->setTimeout((_tcpTimeout + 500) / 1000);	

    log_d(" connected to %s:%u", _host.c_str(), _port);


/*
#ifdef ESP8266
    _client->setNoDelay(true);
#endif
 */
 return connected();
}

/**
 * sends HTTP request header
 * @param type (GET, POST, ...)
 * @return status
 */
bool HTTPClientLight::sendHeader(const char * type)
{
    if(!connected()) {
        return false;
    }

    String header = String(type) + " " + _uri + F(" HTTP/1.");

    if(_useHTTP10) {
        header += "0";
    } else {
        header += "1";
    }

    header += String(F("\r\nHost: ")) + _host;
    if (_port != 80 && _port != 443)
    {
        header += ':';
        header += String(_port);
    }
    header += String(F("\r\nUser-Agent: ")) + _userAgent +
              F("\r\nConnection: ");

    if(_reuse) {
        header += F("keep-alive");
    } else {
        header += F("close");
    }
    header += "\r\n";

    if(!_useHTTP10) {
        header += F("Accept-Encoding: identity;q=1,chunked;q=0.1,*;q=0\r\n");
    }

    if(_base64Authorization.length()) {
        _base64Authorization.replace("\n", "");
        header += F("Authorization: Basic ");
        header += _base64Authorization;
        header += "\r\n";
    }

    header += _headers + "\r\n";

    return (_client->write((const uint8_t *) header.c_str(), header.length()) == header.length());
}

/**
 * reads the response from the server
 * @return int http code
 */
int HTTPClientLight::handleHeaderResponse()
{

    if(!connected()) {
        return HTTPC_ERROR_NOT_CONNECTED;
    }

    clear();

    _canReuse = _reuse;

    String transferEncoding;

    _transferEncoding = HTTPC_TE_IDENTITY;
    unsigned long lastDataTime = millis();
    bool firstLine = true;

    while(connected()) {
        size_t len = _client->available();
        if(len > 0) {
            String headerLine = _client->readStringUntil('\n');
            headerLine.trim(); // remove \r

            lastDataTime = millis();

            log_v("RX: '%s'", headerLine.c_str());

            if(firstLine) {
		firstLine = false;
                if(_canReuse && headerLine.startsWith("HTTP/1.")) {
                    _canReuse = (headerLine[sizeof "HTTP/1." - 1] != '0');
                }
                int codePos = headerLine.indexOf(' ') + 1;
                _returnCode = headerLine.substring(codePos, headerLine.indexOf(' ', codePos)).toInt();
            } else if(headerLine.indexOf(':')) {
                String headerName = headerLine.substring(0, headerLine.indexOf(':'));
                String headerValue = headerLine.substring(headerLine.indexOf(':') + 1);
                headerValue.trim();

                if(headerName.equalsIgnoreCase("Content-Length")) {
                    _size = headerValue.toInt();
                }

                if(_canReuse && headerName.equalsIgnoreCase("Connection")) {
                    if(headerValue.indexOf("close") >= 0 && headerValue.indexOf("keep-alive") < 0) {
                        _canReuse = false;
                    }
                }

                if(headerName.equalsIgnoreCase("Transfer-Encoding")) {
                    transferEncoding = headerValue;
                }

                if (headerName.equalsIgnoreCase("Location")) {
                    _location = headerValue;
                }

                for(size_t i = 0; i < _headerKeysCount; i++) {
                    if(_currentHeaders[i].key.equalsIgnoreCase(headerName)) {
                        _currentHeaders[i].value = headerValue;
                        break;
                    }
                }
            }

            if(headerLine == "") {
                log_d("code: %d", _returnCode);

                if(_size > 0) {
                    log_d("size: %d", _size);
                }

                if(transferEncoding.length() > 0) {
                    log_d("Transfer-Encoding: %s", transferEncoding.c_str());
                    if(transferEncoding.equalsIgnoreCase("chunked")) {
                        _transferEncoding = HTTPC_TE_CHUNKED;
                    } else if(transferEncoding.equalsIgnoreCase("identity")) {
                        _transferEncoding = HTTPC_TE_IDENTITY;
                    } else {
                        return HTTPC_ERROR_ENCODING;
                    }
                } else {
                    _transferEncoding = HTTPC_TE_IDENTITY;
                }

                if(_returnCode) {
                    return _returnCode;
                } else {
                    log_d("Remote host is not an HTTP Server!");
                    return HTTPC_ERROR_NO_HTTP_SERVER;
                }
            }

        } else {
            if((millis() - lastDataTime) > _tcpTimeout) {
                return HTTPC_ERROR_READ_TIMEOUT;
            }
            delay(10);
        }
    }

    return HTTPC_ERROR_CONNECTION_LOST;
}

/**
 * write one Data Block to Stream
 * @param stream Stream *
 * @param size int
 * @return < 0 = error >= 0 = size written
 */
int HTTPClientLight::writeToStreamDataBlock(Stream * stream, int size)
{
    int buff_size = HTTP_TCP_BUFFER_SIZE;
    int len = size;
    int bytesWritten = 0;

    // if possible create smaller buffer then HTTP_TCP_BUFFER_SIZE
    if((len > 0) && (len < HTTP_TCP_BUFFER_SIZE)) {
        buff_size = len;
    }

    // create buffer for read
    uint8_t * buff = (uint8_t *) malloc(buff_size);

    if(buff) {
        // read all data from server
        while(connected() && (len > 0 || len == -1)) {

            // get available data size
            size_t sizeAvailable = _client->available();

            if(sizeAvailable) {

                int readBytes = sizeAvailable;

                // read only the asked bytes
                if(len > 0 && readBytes > len) {
                    readBytes = len;
                }

                // not read more the buffer can handle
                if(readBytes > buff_size) {
                    readBytes = buff_size;
                }
		    
        		// stop if no more reading    
        		if (readBytes == 0)
        			break;

                // read data
                int bytesRead = _client->readBytes(buff, readBytes);

                // write it to Stream
                int bytesWrite = stream->write(buff, bytesRead);
                bytesWritten += bytesWrite;

                // are all Bytes a writen to stream ?
                if(bytesWrite != bytesRead) {
                    log_d("short write asked for %d but got %d retry...", bytesRead, bytesWrite);

                    // check for write error
                    if(stream->getWriteError()) {
                        log_d("stream write error %d", stream->getWriteError());

                        //reset write error for retry
                        stream->clearWriteError();
                    }

                    // some time for the stream
                    delay(1);

                    int leftBytes = (readBytes - bytesWrite);

                    // retry to send the missed bytes
                    bytesWrite = stream->write((buff + bytesWrite), leftBytes);
                    bytesWritten += bytesWrite;

                    if(bytesWrite != leftBytes) {
                        // failed again
                        log_w("short write asked for %d but got %d failed.", leftBytes, bytesWrite);
                        free(buff);
                        return HTTPC_ERROR_STREAM_WRITE;
                    }
                }

                // check for write error
                if(stream->getWriteError()) {
                    log_w("stream write error %d", stream->getWriteError());
                    free(buff);
                    return HTTPC_ERROR_STREAM_WRITE;
                }

                // count bytes to read left
                if(len > 0) {
                    len -= readBytes;
                }

                delay(0);
            } else {
                delay(1);
            }
        }

        free(buff);

        log_d("connection closed or file end (written: %d).", bytesWritten);

        if((size > 0) && (size != bytesWritten)) {
            log_d("bytesWritten %d and size %d mismatch!.", bytesWritten, size);
            return HTTPC_ERROR_STREAM_WRITE;
        }

    } else {
        log_w("too less ram! need %d", HTTP_TCP_BUFFER_SIZE);
        return HTTPC_ERROR_TOO_LESS_RAM;
    }

    return bytesWritten;
}

/**
 * called to handle error return, may disconnect the connection if still exists
 * @param error
 * @return error
 */
int HTTPClientLight::returnError(int error)
{
    if(error < 0) {
        log_w("error(%d): %s", error, errorToString(error).c_str());
        if(connected()) {
            log_d("tcp stop");
            _client->stop();
        }
    }
    return error;
}

void HTTPClientLight::setFollowRedirects(followRedirects_t follow)
{
    _followRedirects = follow;
}

void HTTPClientLight::setRedirectLimit(uint16_t limit)
{
    _redirectLimit = limit;
}

/**
 * set the URL to a new value. Handy for following redirects.
 * @param url
 */
bool HTTPClientLight::setURL(const String& url)
{
    // if the new location is only a path then only update the URI
    if (url && url[0] == '/') {
        _uri = url;
        clear();
        return true;
    }

    if (!url.startsWith(_protocol + ':')) {
        log_d("new URL not the same protocol, expected '%s', URL: '%s'\n", _protocol.c_str(), url.c_str());
        return false;
    }

    // check if the port is specified
    int indexPort = url.indexOf(':', 6); // find the first ':' excluding the one from the protocol
    int indexURI = url.indexOf('/', 7); // find where the URI starts to make sure the ':' is not part of it
    if (indexPort == -1 || indexPort > indexURI) {
        // the port is not specified
        _port = (_protocol == "https" ? 443 : 80);
    }

    // disconnect but preserve _client. 
    // Also have to keep the connection otherwise it will free some of the memory used by _client 
    // and will blow up later when trying to do _client->available() or similar
    _canReuse = true;
    disconnect(true);
    return beginInternal(url, _protocol.c_str());
}

const String &HTTPClientLight::getLocation(void)
{
    return _location;
}
#endif // ESP32
