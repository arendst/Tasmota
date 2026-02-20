/*
 * AsyncHttpClientLight.h  - Created on: 26.08.2025
 *
 * VER: 1.0
 *
 * Copyright (C) 2025 by Martin Macák (HexaMaster) <hexamaster@icloud.com>
 *
 * Async HTTP/HTTPS client for ESP32 (WDT-friendly)
 *
 * Features
 * - HTTP: lwIP socket transport (IClientLite + AsyncTcpAdapter) with SO_LINGER (RST-close) to reduce TIME_WAIT pressure
 * - HTTPS: BearSSL::WiFiClientSecure_light
 * - API and error codes compatible with Arduino HTTPClient/HTTPClientLight semantics
 *
 * Async extensions
 * - asyncGETStart(), asyncPOSTStart(): launch a request in the background (FreeRTOS worker), non-blocking for the caller
 * - asyncState(): query status; after ASYNC_DONE you can use getString()/getSize()/writeToStream() without further I/O
 * - asyncAbort(): best-effort cancellation of an in-flight async request
 * Note: in async mode the response body is stored in an internal RAM buffer (hard cap kAsyncBodyLimit).
 *
 * Acknowledgments: API/error-code compatibility inspired by Arduino HTTPClient / Tasmota HTTPClientLight.
 * This file contains an independent implementation; no source code was copied.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#pragma once

#include <Arduino.h>
#include <WiFiClient.h>
#include <Stream.h>
#include <StreamString.h>

#include "tasmota_options.h"
#include <HTTPClient.h>

#ifdef ESP32
  #include <freertos/FreeRTOS.h>
  #include <freertos/task.h>
  #include <freertos/semphr.h>
  #include <lwip/sockets.h>
  #include <lwip/netdb.h>
  #include <fcntl.h>
  #include <errno.h>
  #include <sys/ioctl.h>
#endif

#include <memory>

#ifndef HTTPC_ERROR_CONNECTION_REFUSED
#define HTTPC_ERROR_CONNECTION_REFUSED   (-1)
#define HTTPC_ERROR_SEND_HEADER_FAILED   (-2)
#define HTTPC_ERROR_SEND_PAYLOAD_FAILED  (-3)
#define HTTPC_ERROR_NOT_CONNECTED        (-4)
#define HTTPC_ERROR_CONNECTION_LOST      (-5)
#define HTTPC_ERROR_NO_STREAM            (-6)
#define HTTPC_ERROR_NO_HTTP_SERVER       (-7)
#define HTTPC_ERROR_TOO_LESS_RAM         (-8)
#define HTTPC_ERROR_ENCODING             (-9)
#define HTTPC_ERROR_STREAM_WRITE         (-10)
#define HTTPC_ERROR_READ_TIMEOUT         (-11)
#endif

#ifndef HTTP_TCP_BUFFER_SIZE
#define HTTP_TCP_BUFFER_SIZE 1460
#endif

#ifndef ASYNCHTTP_WKR_STACK
  #define ASYNCHTTP_WKR_STACK 12288
#endif
#ifndef ASYNCHTTP_WAIT_SLICE_MS
  #define ASYNCHTTP_WAIT_SLICE_MS 25
#endif

// Enable RST-close (SO_LINGER) on the raw HTTP socket (reduces TIME_WAIT pressure)
#ifndef ASYNCHTTP_HTTP_LINGER_RST
  #define ASYNCHTTP_HTTP_LINGER_RST 1
#endif

#ifdef USE_WEBCLIENT_HTTPS
  #include "WiFiClientSecureLightBearSSL.h"
#endif

class AsyncHttpClientLight {
public:
  AsyncHttpClientLight();
  ~AsyncHttpClientLight();

  bool begin(String url);
  bool begin(String url, const char* CAcert) { (void)CAcert; return begin(url); }
  void end();

  bool connected();

  void setReuse(bool reuse) { _reuse = reuse; }
  void setUserAgent(const String& userAgent) { _userAgent = userAgent; }
  void setAuthorization(const char * user, const char * password);
  void setAuthorization(const char * auth);
  void setConnectTimeout(int32_t connectTimeout) { _connectTimeout = connectTimeout; }
  void setTimeout(uint16_t timeout);
  void useHTTP10(bool useHTTP10) { _useHTTP10 = useHTTP10; _reuse = !useHTTP10; }
  void setFollowRedirects(followRedirects_t follow) { _followRedirects = follow; }
  void setRedirectLimit(uint16_t limit) { _redirectLimit = limit; }
  void setWaitSliceMs(uint32_t ms) { _waitSliceMs = ms ? ms : 1; }

  bool setPubKeyPins(const char* pin1, const char* pin2 = nullptr);
  void clearPubKeyPins();
  void setRSAOnly(bool rsaOnly) { _rsaOnly = rsaOnly; }

  // sync calls
  int GET();
  int POST(uint8_t * payload, size_t size);
  int POST(String payload);
  int PUT(uint8_t * payload, size_t size);
  int PUT(String payload);
  int PATCH(uint8_t * payload, size_t size);
  int PATCH(String payload);
  int DELETE(uint8_t * payload, size_t size);
  int DELETE(String payload);
  int sendRequest(const char * type, String payload);
  int sendRequest(const char * type, uint8_t * payload = nullptr, size_t size = 0);
  int sendRequest(const char * type, Stream * stream, size_t size = 0);

  int    getSize(void) const { return _size; }
  String getString(void);                 
  int    writeToStream(Stream * stream);

  void   addHeader(const String& name, const String& value, bool first=false, bool replace=true);
  void   collectHeaders(const char* headerKeys[], const size_t headerKeysCount);
  String header(const char* name) const;
  String header(const String& name) const { return header(name.c_str()); }
  String header(size_t i) const;
  String headerName(size_t i) const;
  int    headers() const { return (int)_headerKeysCount; }
  bool   hasHeader(const char* name) const;

  bool   setURL(const String &url);
  const String &getLocation(void) const { return _location; }

  static String errorToString(int error);

  // async calls

  enum AsyncState_t : uint8_t {
    ASYNC_IDLE     = 0,
    ASYNC_RUNNING  = 1,
    ASYNC_DONE     = 2,
    ASYNC_ERROR    = 3,
    ASYNC_ABORTED  = 4
  };


  int  asyncGETStart();
  int  asyncPOSTStart(uint8_t* payload, size_t size);
  int  asyncPOSTStart(const String& payload);

  AsyncState_t asyncState(int* httpCodeOut = nullptr, int* bytesOut = nullptr) const;

  bool asyncBusy() const { return _asyncState == ASYNC_RUNNING; }
  void asyncAbort();

private:
  int readBodyToStreamString(StreamString& s, int size_limit);

  struct IClientLite {
    virtual ~IClientLite() {}
    virtual bool connectHost(const char* host, uint16_t port, int32_t timeout_ms) = 0;
    virtual bool connectIP(IPAddress ip, uint16_t port, int32_t timeout_ms) = 0;
    virtual size_t write(const uint8_t* data, size_t len) = 0;
    virtual int    read(uint8_t* buf, size_t len) = 0;
    virtual int    available() = 0;
    virtual bool   connected() = 0;
    virtual void   stop() = 0;
    virtual void   setTimeout(uint16_t ms) = 0;
  };

  class AsyncTcpAdapter : public IClientLite {
  public:
    AsyncTcpAdapter() {}
    ~AsyncTcpAdapter() override { stop(); }

    bool connectHost(const char* host, uint16_t port, int32_t timeout_ms) override;
    bool connectIP(IPAddress ip, uint16_t port, int32_t timeout_ms) override;
    size_t write(const uint8_t* data, size_t len) override;
    int    read(uint8_t* buf, size_t len) override;
    int    available() override;
    bool   connected() override { return _isConnected && _fd >= 0; }
    void   stop() override;
    void   setTimeout(uint16_t ms) override { _rwTimeout = ms; }

  private:
    int      _fd = -1;
    bool     _isConnected = false;
    uint16_t _rwTimeout = 5000;

    bool connect_fd(const struct sockaddr_in& sa, int32_t timeout_ms);
  };

#ifdef USE_WEBCLIENT_HTTPS
  class BearSslAdapter : public IClientLite {
  public:
    BearSslAdapter(const uint8_t* pin1, bool hasPin1,
                   const uint8_t* pin2, bool hasPin2,
                   bool rsaOnly);
    ~BearSslAdapter() override { stop(); }

    bool connectHost(const char* host, uint16_t port, int32_t timeout_ms) override;
    bool connectIP(IPAddress ip, uint16_t port, int32_t timeout_ms) override;
    size_t write(const uint8_t* data, size_t len) override;
    int    read(uint8_t* buf, size_t len) override;
    int    available() override;
    bool   connected() override;
    void   stop() override;
    void   setTimeout(uint16_t ms) override;

  private:
    std::unique_ptr<BearSSL::WiFiClientSecure_light> _cli;
  };
#endif

  bool beginInternal(String url, const char* expectedProto);
  bool ensureConnected();
  bool connectTransport();
  bool sendHeader(const char * method);
  int  handleHeaderResponse();
  int  writeToStreamDataBlock(Stream * stream, int size);
  bool writeAllWithOneReconnect(const uint8_t* data, size_t len, bool allow_retry_if_zero_sent);

#ifdef ESP32
  struct Job {
    typedef int (*Fn)(void* arg);
    Fn fn = nullptr;
    void* arg = nullptr;
    int result = HTTPC_ERROR_NOT_CONNECTED;
    SemaphoreHandle_t done = nullptr;
  };
  static void WorkerTask(void* pv);

  // Blocking execution of a job (existing sync calls)
  int  runJob(Job::Fn fn, void* arg);

  // Non-blocking job start (for async mode): 1=queued, 0=busy, <0=error.
  int  startJob(Job::Fn fn, void* arg);
#endif

  struct SendBufArg { AsyncHttpClientLight* self; const char* method; uint8_t* payload; size_t size; };
  struct SendStrArg { AsyncHttpClientLight* self; const char* method; String payload; };
  struct SendStrmArg{ AsyncHttpClientLight* self; const char* method; Stream* stream; size_t size; };
  struct IntOpArg   { AsyncHttpClientLight* self; };

  static int Job_GET(void* arg);
  static int Job_SendBuf(void* arg);
  static int Job_SendStr(void* arg);
  static int Job_SendStrm(void* arg);
  static int Job_WriteToStream(void* arg);
  static int Job_GetString(void* arg);

  // --- async jobs---
  struct AsyncGetArg     { AsyncHttpClientLight* self; };
  struct AsyncPostBufArg { AsyncHttpClientLight* self; uint8_t* payload; size_t size; };
  struct AsyncPostStrArg { AsyncHttpClientLight* self; String payload; };

  static int Job_AsyncGET(void* arg);
  static int Job_AsyncPOST_Buf(void* arg);
  static int Job_AsyncPOST_Str(void* arg);

  // =======================
  // URL / HTTP state
  // =======================
  String _protocol, _host, _uri;
  uint16_t _port = 0;
  bool _secure = false;

  String _headers;
  String _userAgent = F("AsyncHttpClientLight");
  String _base64Authorization;
  uint16_t _tcpTimeout = 5000;
  int32_t  _connectTimeout = 10000;
  bool _reuse = true;
  bool _canReuse = false;
  bool _useHTTP10 = false;
  int  _returnCode = 0;
  int  _size = -1;
  transferEncoding_t _transferEncoding = HTTPC_TE_IDENTITY;
  String _location;
  followRedirects_t _followRedirects = HTTPC_DISABLE_FOLLOW_REDIRECTS;
  uint16_t _redirectLimit = 5;

  bool _bodyPending = false;

  struct RequestArgument { String key; String value; };
  RequestArgument* _currentHeaders = nullptr;
  size_t           _headerKeysCount = 0;

  bool _hasPin1=false, _hasPin2=false, _rsaOnly=false;
  uint8_t _pin1[20] = {0}, _pin2[20] = {0};

  std::unique_ptr<IClientLite> _transport;

#ifdef ESP32
  SemaphoreHandle_t _busy = nullptr;       // Parallel operations watchdog
#endif
  uint32_t _waitSliceMs = ASYNCHTTP_WAIT_SLICE_MS;

  Stream* _lastStream = nullptr;
  String  _lastString;

  // -------------------------
  // Async internal state
  // -------------------------
  enum AsyncOp_t : uint8_t { AOP_NONE=0, AOP_GET=1, AOP_POST=2 };
  AsyncOp_t     _asyncOp = AOP_NONE;
  AsyncState_t  _asyncState = ASYNC_IDLE;
  volatile bool _asyncAbortReq = false;

  // HTTP result and metadata after completion (or on error)
  int           _asyncHttpCode = HTTPC_ERROR_NOT_CONNECTED;
  int           _asyncRxBytes  = -1;

  // RAM buffer for the response body in async mode (hard cap to protect the heap)
  static constexpr size_t kAsyncBodyLimit = 16384;
  StreamString  _asyncBody;

#ifdef ESP32
  // RTOS/polling internals
  Job*             _asyncJob  = nullptr;
  SemaphoreHandle_t _asyncDone = nullptr;
  TaskHandle_t     _asyncTask = nullptr;
#endif

// ---------------------------------------------------------
// FRIEND: the free-function helper in .cpp has access to private members
// ---------------------------------------------------------
  friend int ahcl_readBodyToStreamString(AsyncHttpClientLight* self, StreamString& s, int size_limit);
};
