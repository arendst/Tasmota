/*
  Asynchronous WebServer library for Espressif MCUs

  Copyright (c) 2016 Hristo Gochkov. All rights reserved.
  This file is part of the esp8266 core for Arduino environment.

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
#ifndef _ESPAsyncWebServer_H_
#define _ESPAsyncWebServer_H_

#include "Arduino.h"

#include <functional>
#include "FS.h"

#include "StringArray.h"

#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#else
#error Platform not supported
#endif

#ifdef ASYNCWEBSERVER_REGEX
#define ASYNCWEBSERVER_REGEX_ATTRIBUTE
#else
#define ASYNCWEBSERVER_REGEX_ATTRIBUTE __attribute__((warning("ASYNCWEBSERVER_REGEX not defined")))
#endif

#define DEBUGF(...) //Serial.printf(__VA_ARGS__)

class AsyncWebServer;
class AsyncWebServerRequest;
class AsyncWebServerResponse;
class AsyncWebHeader;
class AsyncWebParameter;
class AsyncWebRewrite;
class AsyncWebHandler;
class AsyncStaticWebHandler;
class AsyncCallbackWebHandler;
class AsyncResponseStream;

#ifndef WEBSERVER_H
typedef enum {
  HTTP_GET     = 0b00000001,
  HTTP_POST    = 0b00000010,
  HTTP_DELETE  = 0b00000100,
  HTTP_PUT     = 0b00001000,
  HTTP_PATCH   = 0b00010000,
  HTTP_HEAD    = 0b00100000,
  HTTP_OPTIONS = 0b01000000,
  HTTP_ANY     = 0b01111111,
} WebRequestMethod;
#endif

//if this value is returned when asked for data, packet will not be sent and you will be asked for data again
#define RESPONSE_TRY_AGAIN 0xFFFFFFFF

typedef uint8_t WebRequestMethodComposite;
typedef std::function<void(void)> ArDisconnectHandler;

/*
 * PARAMETER :: Chainable object to hold GET/POST and FILE parameters
 * */

class AsyncWebParameter {
  private:
    String _name;
    String _value;
    size_t _size;
    bool _isForm;
    bool _isFile;

  public:

    AsyncWebParameter(const String& name, const String& value, bool form=false, bool file=false, size_t size=0): _name(name), _value(value), _size(size), _isForm(form), _isFile(file){}
    const String& name() const { return _name; }
    const String& value() const { return _value; }
    size_t size() const { return _size; }
    bool isPost() const { return _isForm; }
    bool isFile() const { return _isFile; }
};

/*
 * HEADER :: Chainable object to hold the headers
 * */

class AsyncWebHeader {
  private:
    String _name;
    String _value;

  public:
    AsyncWebHeader(const String& name, const String& value): _name(name), _value(value){}
    AsyncWebHeader(const String& data): _name(), _value(){
      if(!data) return;
      int index = data.indexOf(':');
      if (index < 0) return;
      _name = data.substring(0, index);
      _value = data.substring(index + 2);
    }
    ~AsyncWebHeader(){}
    const String& name() const { return _name; }
    const String& value() const { return _value; }
    String toString() const { return String(_name+": "+_value+"\r\n"); }
};

/*
 * REQUEST :: Each incoming Client is wrapped inside a Request and both live together until disconnect
 * */

typedef enum { RCT_NOT_USED = -1, RCT_DEFAULT = 0, RCT_HTTP, RCT_WS, RCT_EVENT, RCT_MAX } RequestedConnectionType;

typedef std::function<size_t(uint8_t*, size_t, size_t)> AwsResponseFiller;
typedef std::function<String(const String&)> AwsTemplateProcessor;

class AsyncWebServerRequest {
  using File = fs::File;
  using FS = fs::FS;
  friend class AsyncWebServer;
  friend class AsyncCallbackWebHandler;
  private:
    AsyncClient* _client;
    AsyncWebServer* _server;
    AsyncWebHandler* _handler;
    AsyncWebServerResponse* _response;
    StringArray _interestingHeaders;
    ArDisconnectHandler _onDisconnectfn;

    String _temp;
    uint8_t _parseState;

    uint8_t _version;
    WebRequestMethodComposite _method;
    String _url;
    String _host;
    String _contentType;
    String _boundary;
    String _authorization;
    RequestedConnectionType _reqconntype;
    void _removeNotInterestingHeaders();
    bool _isDigest;
    bool _isMultipart;
    bool _isPlainPost;
    bool _expectingContinue;
    size_t _contentLength;
    size_t _parsedLength;

    AsyncLinkedList<AsyncWebHeader *> _headers;
    AsyncLinkedList<AsyncWebParameter *> _params;
    AsyncLinkedList<String *> _pathParams;

    uint8_t _multiParseState;
    uint8_t _boundaryPosition;
    size_t _itemStartIndex;
    size_t _itemSize;
    String _itemName;
    String _itemFilename;
    String _itemType;
    String _itemValue;
    uint8_t *_itemBuffer;
    size_t _itemBufferIndex;
    bool _itemIsFile;

    void _onPoll();
    void _onAck(size_t len, uint32_t time);
    void _onError(int8_t error);
    void _onTimeout(uint32_t time);
    void _onDisconnect();
    void _onData(void *buf, size_t len);

    void _addParam(AsyncWebParameter*);
    void _addPathParam(const char *param);

    bool _parseReqHead();
    bool _parseReqHeader();
    void _parseLine();
    void _parsePlainPostChar(uint8_t data);
    void _parseMultipartPostByte(uint8_t data, bool last);
    void _addGetParams(const String& params);

    void _handleUploadStart();
    void _handleUploadByte(uint8_t data, bool last);
    void _handleUploadEnd();

  public:
    File _tempFile;
    void *_tempObject;

    AsyncWebServerRequest(AsyncWebServer*, AsyncClient*);
    ~AsyncWebServerRequest();

    AsyncClient* client(){ return _client; }
    uint8_t version() const { return _version; }
    WebRequestMethodComposite method() const { return _method; }
    const String& url() const { return _url; }
    const String& host() const { return _host; }
    const String& contentType() const { return _contentType; }
    size_t contentLength() const { return _contentLength; }
    bool multipart() const { return _isMultipart; }
    const char * methodToString() const;
    const char * requestedConnTypeToString() const;
    RequestedConnectionType requestedConnType() const { return _reqconntype; }
    bool isExpectedRequestedConnType(RequestedConnectionType erct1, RequestedConnectionType erct2 = RCT_NOT_USED, RequestedConnectionType erct3 = RCT_NOT_USED);
    void onDisconnect (ArDisconnectHandler fn);

    //hash is the string representation of:
    // base64(user:pass) for basic or
    // user:realm:md5(user:realm:pass) for digest
    bool authenticate(const char * hash);
    bool authenticate(const char * username, const char * password, const char * realm = NULL, bool passwordIsHash = false);
    void requestAuthentication(const char * realm = NULL, bool isDigest = true);

    void setHandler(AsyncWebHandler *handler){ _handler = handler; }
    void addInterestingHeader(const String& name);

    void redirect(const String& url);

    void send(AsyncWebServerResponse *response);
    void send(int code, const String& contentType=String(), const String& content=String());
    void send(FS &fs, const String& path, const String& contentType=String(), bool download=false, AwsTemplateProcessor callback=nullptr);
    void send(File content, const String& path, const String& contentType=String(), bool download=false, AwsTemplateProcessor callback=nullptr);
    void send(Stream &stream, const String& contentType, size_t len, AwsTemplateProcessor callback=nullptr);
    void send(const String& contentType, size_t len, AwsResponseFiller callback, AwsTemplateProcessor templateCallback=nullptr);
    void sendChunked(const String& contentType, AwsResponseFiller callback, AwsTemplateProcessor templateCallback=nullptr);
    void send_P(int code, const String& contentType, const uint8_t * content, size_t len, AwsTemplateProcessor callback=nullptr);
    void send_P(int code, const String& contentType, PGM_P content, AwsTemplateProcessor callback=nullptr);

    AsyncWebServerResponse *beginResponse(int code, const String& contentType=String(), const String& content=String());
    AsyncWebServerResponse *beginResponse(FS &fs, const String& path, const String& contentType=String(), bool download=false, AwsTemplateProcessor callback=nullptr);
    AsyncWebServerResponse *beginResponse(File content, const String& path, const String& contentType=String(), bool download=false, AwsTemplateProcessor callback=nullptr);
    AsyncWebServerResponse *beginResponse(Stream &stream, const String& contentType, size_t len, AwsTemplateProcessor callback=nullptr);
    AsyncWebServerResponse *beginResponse(const String& contentType, size_t len, AwsResponseFiller callback, AwsTemplateProcessor templateCallback=nullptr);
    AsyncWebServerResponse *beginChunkedResponse(const String& contentType, AwsResponseFiller callback, AwsTemplateProcessor templateCallback=nullptr);
    AsyncResponseStream *beginResponseStream(const String& contentType, size_t bufferSize=1460);
    AsyncWebServerResponse *beginResponse_P(int code, const String& contentType, const uint8_t * content, size_t len, AwsTemplateProcessor callback=nullptr);
    AsyncWebServerResponse *beginResponse_P(int code, const String& contentType, PGM_P content, AwsTemplateProcessor callback=nullptr);

    size_t headers() const;                     // get header count
    bool hasHeader(const String& name) const;   // check if header exists
    bool hasHeader(const __FlashStringHelper * data) const;   // check if header exists

    AsyncWebHeader* getHeader(const String& name) const;
    AsyncWebHeader* getHeader(const __FlashStringHelper * data) const;
    AsyncWebHeader* getHeader(size_t num) const;

    size_t params() const;                      // get arguments count
    bool hasParam(const String& name, bool post=false, bool file=false) const;
    bool hasParam(const __FlashStringHelper * data, bool post=false, bool file=false) const;

    AsyncWebParameter* getParam(const String& name, bool post=false, bool file=false) const;
    AsyncWebParameter* getParam(const __FlashStringHelper * data, bool post, bool file) const; 
    AsyncWebParameter* getParam(size_t num) const;

    size_t args() const { return params(); }     // get arguments count
    const String& arg(const String& name) const; // get request argument value by name
    const String& arg(const __FlashStringHelper * data) const; // get request argument value by F(name)    
    const String& arg(size_t i) const;           // get request argument value by number
    const String& argName(size_t i) const;       // get request argument name by number
    bool hasArg(const char* name) const;         // check if argument exists
    bool hasArg(const __FlashStringHelper * data) const;         // check if F(argument) exists

    const String& ASYNCWEBSERVER_REGEX_ATTRIBUTE pathArg(size_t i) const;

    const String& header(const char* name) const;// get request header value by name
    const String& header(const __FlashStringHelper * data) const;// get request header value by F(name)    
    const String& header(size_t i) const;        // get request header value by number
    const String& headerName(size_t i) const;    // get request header name by number
    String urlDecode(const String& text) const;
};

/*
 * FILTER :: Callback to filter AsyncWebRewrite and AsyncWebHandler (done by the Server)
 * */

typedef std::function<bool(AsyncWebServerRequest *request)> ArRequestFilterFunction;

bool ON_STA_FILTER(AsyncWebServerRequest *request);

bool ON_AP_FILTER(AsyncWebServerRequest *request);

/*
 * REWRITE :: One instance can be handle any Request (done by the Server)
 * */

class AsyncWebRewrite {
  protected:
    String _from;
    String _toUrl;
    String _params;
    ArRequestFilterFunction _filter;
  public:
    AsyncWebRewrite(const char* from, const char* to): _from(from), _toUrl(to), _params(String()), _filter(NULL){
      int index = _toUrl.indexOf('?');
      if (index > 0) {
        _params = _toUrl.substring(index +1);
        _toUrl = _toUrl.substring(0, index);
      }
    }
    virtual ~AsyncWebRewrite(){}
    AsyncWebRewrite& setFilter(ArRequestFilterFunction fn) { _filter = fn; return *this; }
    bool filter(AsyncWebServerRequest *request) const { return _filter == NULL || _filter(request); }
    const String& from(void) const { return _from; }
    const String& toUrl(void) const { return _toUrl; }
    const String& params(void) const { return _params; }
    virtual bool match(AsyncWebServerRequest *request) { return from() == request->url() && filter(request); }
};

/*
 * HANDLER :: One instance can be attached to any Request (done by the Server)
 * */

class AsyncWebHandler {
  protected:
    ArRequestFilterFunction _filter;
    String _username;
    String _password;
  public:
    AsyncWebHandler():_username(""), _password(""){}
    AsyncWebHandler& setFilter(ArRequestFilterFunction fn) { _filter = fn; return *this; }
    AsyncWebHandler& setAuthentication(const char *username, const char *password){  _username = String(username);_password = String(password); return *this; };
    bool filter(AsyncWebServerRequest *request){ return _filter == NULL || _filter(request); }
    virtual ~AsyncWebHandler(){}
    virtual bool canHandle(AsyncWebServerRequest *request __attribute__((unused))){
      return false;
    }
    virtual void handleRequest(AsyncWebServerRequest *request __attribute__((unused))){}
    virtual void handleUpload(AsyncWebServerRequest *request  __attribute__((unused)), const String& filename __attribute__((unused)), size_t index __attribute__((unused)), uint8_t *data __attribute__((unused)), size_t len __attribute__((unused)), bool final  __attribute__((unused))){}
    virtual void handleBody(AsyncWebServerRequest *request __attribute__((unused)), uint8_t *data __attribute__((unused)), size_t len __attribute__((unused)), size_t index __attribute__((unused)), size_t total __attribute__((unused))){}
    virtual bool isRequestHandlerTrivial(){return true;}
};

/*
 * RESPONSE :: One instance is created for each Request (attached by the Handler)
 * */

typedef enum {
  RESPONSE_SETUP, RESPONSE_HEADERS, RESPONSE_CONTENT, RESPONSE_WAIT_ACK, RESPONSE_END, RESPONSE_FAILED
} WebResponseState;

class AsyncWebServerResponse {
  protected:
    int _code;
    AsyncLinkedList<AsyncWebHeader *> _headers;
    String _contentType;
    size_t _contentLength;
    bool _sendContentLength;
    bool _chunked;
    size_t _headLength;
    size_t _sentLength;
    size_t _ackedLength;
    size_t _writtenLength;
    WebResponseState _state;
    const char* _responseCodeToString(int code);

  public:
    AsyncWebServerResponse();
    virtual ~AsyncWebServerResponse();
    virtual void setCode(int code);
    virtual void setContentLength(size_t len);
    virtual void setContentType(const String& type);
    virtual void addHeader(const String& name, const String& value);
    virtual String _assembleHead(uint8_t version);
    virtual bool _started() const;
    virtual bool _finished() const;
    virtual bool _failed() const;
    virtual bool _sourceValid() const;
    virtual void _respond(AsyncWebServerRequest *request);
    virtual size_t _ack(AsyncWebServerRequest *request, size_t len, uint32_t time);
};

/*
 * SERVER :: One instance
 * */

typedef std::function<void(AsyncWebServerRequest *request)> ArRequestHandlerFunction;
typedef std::function<void(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final)> ArUploadHandlerFunction;
typedef std::function<void(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)> ArBodyHandlerFunction;

class AsyncWebServer {
  protected:
    AsyncServer _server;
    AsyncLinkedList<AsyncWebRewrite*> _rewrites;
    AsyncLinkedList<AsyncWebHandler*> _handlers;
    AsyncCallbackWebHandler* _catchAllHandler;

  public:
    AsyncWebServer(uint16_t port);
    ~AsyncWebServer();

    void begin();
    void end();

#if ASYNC_TCP_SSL_ENABLED
    void onSslFileRequest(AcSSlFileHandler cb, void* arg);
    void beginSecure(const char *cert, const char *private_key_file, const char *password);
#endif

    AsyncWebRewrite& addRewrite(AsyncWebRewrite* rewrite);
    bool removeRewrite(AsyncWebRewrite* rewrite);
    AsyncWebRewrite& rewrite(const char* from, const char* to);

    AsyncWebHandler& addHandler(AsyncWebHandler* handler);
    bool removeHandler(AsyncWebHandler* handler);
  
    AsyncCallbackWebHandler& on(const char* uri, ArRequestHandlerFunction onRequest);
    AsyncCallbackWebHandler& on(const char* uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest);
    AsyncCallbackWebHandler& on(const char* uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest, ArUploadHandlerFunction onUpload);
    AsyncCallbackWebHandler& on(const char* uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest, ArUploadHandlerFunction onUpload, ArBodyHandlerFunction onBody);

    AsyncStaticWebHandler& serveStatic(const char* uri, fs::FS& fs, const char* path, const char* cache_control = NULL);

    void onNotFound(ArRequestHandlerFunction fn);  //called when handler is not assigned
    void onFileUpload(ArUploadHandlerFunction fn); //handle file uploads
    void onRequestBody(ArBodyHandlerFunction fn); //handle posts with plain body content (JSON often transmitted this way as a request)

    void reset(); //remove all writers and handlers, with onNotFound/onFileUpload/onRequestBody 
  
    void _handleDisconnect(AsyncWebServerRequest *request);
    void _attachHandler(AsyncWebServerRequest *request);
    void _rewriteRequest(AsyncWebServerRequest *request);
};

class DefaultHeaders {
  using headers_t = AsyncLinkedList<AsyncWebHeader *>;
  headers_t _headers;
  
  DefaultHeaders()
  :_headers(headers_t([](AsyncWebHeader *h){ delete h; }))
  {}
public:
  using ConstIterator = headers_t::ConstIterator;

  void addHeader(const String& name, const String& value){
    _headers.add(new AsyncWebHeader(name, value));
  }  
  
  ConstIterator begin() const { return _headers.begin(); }
  ConstIterator end() const { return _headers.end(); }

  DefaultHeaders(DefaultHeaders const &) = delete;
  DefaultHeaders &operator=(DefaultHeaders const &) = delete;
  static DefaultHeaders &Instance() {
    static DefaultHeaders instance;
    return instance;
  }
};

#include "WebResponseImpl.h"
#include "WebHandlerImpl.h"
#include "AsyncWebSocket.h"
#include "AsyncEventSource.h"

#endif /* _AsyncWebServer_H_ */
