/*
 * AsyncHttpClientLight.cpp  - Created on: 13.02.2026 
 *
 * VER: 1.0
 * 
 * Copyright (C) 2026 by Martin Macák (HexaMaster) <hexamaster@icloud.com>
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



#include "AsyncHttpClientLight.h"
#include <base64.h>
#include "WiFiHelper.h"
#include <ctype.h>
#include <cstdlib>
#include <new>


// --------- Templates / Helpers  ------------------------------------

template <class T, class... Args>
static T* ahcl_new_obj(Args... args) noexcept {
  void* mem = std::malloc(sizeof(T));
  if (!mem) return nullptr;
  return new (mem) T{ args... };   // placement new
}

template <class T>
static void ahcl_delete_obj(T*& p) noexcept {
  if (!p) return;
  p->~T();
  std::free(p);
  p = nullptr;
}

struct AhclFreeDeleter {
  void operator()(uint8_t* p) const noexcept { std::free(p); }
};

// --------- Utils (SHA1 pin parsing) ------------------------------------
static inline int hexval_local(char c) {
  if (c >= '0' && c <= '9') return c - '0';
  c = toupper((unsigned char)c);
  if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
  return -1;
}
static bool parse_sha1_pin_local(const char* in, uint8_t out[20]) {
  if (!in) return false;
  char buf[41]; int n = 0;
  for (const char* p = in; *p && n < 40; ++p) {
    if (*p == ':' || *p == ' ') continue;
    int hv = hexval_local(*p);
    if (hv < 0) return false;
    buf[n++] = (char)toupper((unsigned char)*p);
  }
  if (n != 40) return false;
  for (int i=0;i<20;i++){
    int hi = hexval_local(buf[2*i]);
    int lo = hexval_local(buf[2*i+1]);
    if (hi < 0 || lo < 0) return false;
    out[i] = (uint8_t)((hi<<4)|lo);
  }
  return true;
}

// ======================== AsyncTcpAdapter (HTTP) =======================
bool AsyncHttpClientLight::AsyncTcpAdapter::connect_fd(const struct sockaddr_in& sa, int32_t timeout_ms) {
  stop();

  int fd = ::socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) return false;

  // nonblocking connect + select with timeout
  int flags = fcntl(fd, F_GETFL, 0);
  fcntl(fd, F_SETFL, flags | O_NONBLOCK);

  int res = ::connect(fd, (const struct sockaddr*)&sa, sizeof(sa));
  if (res < 0 && errno != EINPROGRESS) { ::close(fd); return false; }

  fd_set w; FD_ZERO(&w); FD_SET(fd, &w);
  struct timeval tv;
  tv.tv_sec  = timeout_ms / 1000;
  tv.tv_usec = (timeout_ms % 1000) * 1000;
  int sel = ::select(fd+1, nullptr, &w, nullptr, &tv);
  if (sel <= 0) { ::close(fd); return false; }

  int soerr = 0; socklen_t slen = sizeof(soerr);
  if (getsockopt(fd, SOL_SOCKET, SO_ERROR, &soerr, &slen) != 0 || soerr != 0) {
    ::close(fd); return false;
  }

#ifdef TCP_NODELAY
  int one = 1;
  setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
#endif

  _fd = fd;
  _isConnected = true;
  return true;
}

bool AsyncHttpClientLight::AsyncTcpAdapter::connectIP(IPAddress ip, uint16_t port, int32_t timeout_ms) {
  struct sockaddr_in sa; memset(&sa, 0, sizeof(sa));
  sa.sin_family = AF_INET;
  sa.sin_port   = htons(port);
  sa.sin_addr.s_addr = (in_addr_t)ip;
  return connect_fd(sa, timeout_ms);
}

bool AsyncHttpClientLight::AsyncTcpAdapter::connectHost(const char* host, uint16_t port, int32_t timeout_ms) {
  IPAddress ip;
  if (!WiFiHelper::hostByName(host, ip)) return false;
  return connectIP(ip, port, timeout_ms);
}

size_t AsyncHttpClientLight::AsyncTcpAdapter::write(const uint8_t* data, size_t len) {
  if (_fd < 0 || !_isConnected) return 0;
  int s = ::send(_fd, data, len, MSG_DONTWAIT);
  if (s < 0) {
    if (errno != EAGAIN && errno != EWOULDBLOCK) { stop(); }
    return 0;
  }
  return (size_t)s;
}

int AsyncHttpClientLight::AsyncTcpAdapter::read(uint8_t* buf, size_t len) {
  if (_fd < 0 || !_isConnected) return -1;
  int r = ::recv(_fd, buf, len, MSG_DONTWAIT);
  if (r == 0) { stop(); return 0; }   // peer close
  if (r < 0) {
    if (errno != EAGAIN && errno != EWOULDBLOCK) { stop(); }
    return 0;
  }
  return r;
}

int AsyncHttpClientLight::AsyncTcpAdapter::available() {
  if (_fd < 0 || !_isConnected) return 0;
  int bytes = 0;
  if (ioctl(_fd, FIONREAD, &bytes) < 0) return 0;
  return bytes;
}

void AsyncHttpClientLight::AsyncTcpAdapter::stop() {
  if (_fd >= 0) {
#if ASYNCHTTP_HTTP_LINGER_RST
    struct linger sl; sl.l_onoff = 1; sl.l_linger = 0;
    (void)setsockopt(_fd, SOL_SOCKET, SO_LINGER, &sl, sizeof(sl));
#endif
    ::close(_fd);
  }
  _fd = -1;
  _isConnected = false;
}

// ======================== BearSslAdapter (HTTPS) =======================
#ifdef USE_WEBCLIENT_HTTPS
AsyncHttpClientLight::BearSslAdapter::BearSslAdapter(
    const uint8_t* pin1, bool hasPin1,
    const uint8_t* pin2, bool hasPin2,
    bool rsaOnly) {

  _cli.reset(new BearSSL::WiFiClientSecure_light(16384, 0));
  static const uint8_t any[20] = {0};

  if (hasPin1 || hasPin2) {
    const uint8_t* p1 = hasPin1 ? pin1 : any;
    const uint8_t* p2 = hasPin2 ? pin2 : any;
    _cli->setInsecure(); 
    _cli->setPubKeyFingerprint(p1, p2, /*allow_all=*/false);
  } else {
    _cli->setPubKeyFingerprint(any, any, /*allow_all=*/true);
  }
  _cli->setRSAOnly(rsaOnly);
}

bool AsyncHttpClientLight::BearSslAdapter::connectHost(const char* host, uint16_t port, int32_t timeout_ms) {
  return _cli->connect(host, port, timeout_ms);
}
bool AsyncHttpClientLight::BearSslAdapter::connectIP(IPAddress ip, uint16_t port, int32_t timeout_ms) {
  return _cli->connect(ip, port, timeout_ms);
}
size_t AsyncHttpClientLight::BearSslAdapter::write(const uint8_t* data, size_t len) { return _cli->write(data, len); }
int    AsyncHttpClientLight::BearSslAdapter::read(uint8_t* buf, size_t len)        { return _cli->read(buf, len); }
int    AsyncHttpClientLight::BearSslAdapter::available()                            { return _cli->available(); }
bool   AsyncHttpClientLight::BearSslAdapter::connected()                            { return _cli->connected(); }
void   AsyncHttpClientLight::BearSslAdapter::stop()                                 { _cli->stop(); }
void   AsyncHttpClientLight::BearSslAdapter::setTimeout(uint16_t ms)                { _cli->setTimeout(ms); }
#endif

// ======================== AsyncHttpClientLight core =====================

AsyncHttpClientLight::AsyncHttpClientLight() {
#ifdef ESP32
  _busy = xSemaphoreCreateMutex();
#endif
}

AsyncHttpClientLight::~AsyncHttpClientLight() {
  end();
  if (_currentHeaders) { delete[] _currentHeaders; _currentHeaders = nullptr; }
#ifdef ESP32
  if (_asyncDone) { vSemaphoreDelete(_asyncDone); _asyncDone = nullptr; }
  if (_busy) { vSemaphoreDelete(_busy); _busy = nullptr; }
#endif
}

bool AsyncHttpClientLight::setPubKeyPins(const char* pin1, const char* pin2) {
  _hasPin1 = _hasPin2 = false;
  if (pin1 && *pin1) _hasPin1 = parse_sha1_pin_local(pin1, _pin1);
  if (pin2 && *pin2) _hasPin2 = parse_sha1_pin_local(pin2, _pin2);
  if (!_hasPin1) memset(_pin1, 0, sizeof(_pin1));
  if (!_hasPin2) memset(_pin2, 0, sizeof(_pin2));
  return _hasPin1 || _hasPin2;
}
void AsyncHttpClientLight::clearPubKeyPins() { _hasPin1 = _hasPin2 = false; memset(_pin1,0,sizeof(_pin1)); memset(_pin2,0,sizeof(_pin2)); }

void AsyncHttpClientLight::setAuthorization(const char * user, const char * password) {
  if (user && password) {
    String auth = user; auth += ":"; auth += password;
    _base64Authorization = base64::encode(auth);
  }
}
void AsyncHttpClientLight::setAuthorization(const char * auth) { if (auth) _base64Authorization = auth; }

void AsyncHttpClientLight::setTimeout(uint16_t timeout) {
  _tcpTimeout = timeout;
  if (_transport) _transport->setTimeout(timeout);
}

bool AsyncHttpClientLight::begin(String url) {
  _port = 0; _secure = false; _headers = ""; _returnCode = 0; _size = -1; _location = "";
  _bodyPending = false; // BODY_PENDING reset
  return beginInternal(url, url.startsWith("https") ? "https" : "http");
}

bool AsyncHttpClientLight::beginInternal(String url, const char* expectedProto) {
  int index = url.indexOf(':');
  if (index < 0) return false;
  _protocol = url.substring(0, index);
  if (_protocol != expectedProto) return false;

  url.remove(0, (index + 3)); // remove http(s)://

  index = url.indexOf('/');
  String host = (index >= 0) ? url.substring(0, index) : url;
  url.remove(0, (index >= 0) ? index : url.length());

  // auth
  index = host.indexOf('@');
  if (index >= 0) {
    String auth = host.substring(0, index);
    host.remove(0, index + 1);
    _base64Authorization = base64::encode(auth);
  }

  // port
  index = host.indexOf(':');
  if (index >= 0) {
    _host = host.substring(0, index);
    host.remove(0, (index + 1));
    _port = host.toInt();
  } else {
    _host = host;
  }

  if (_port == 0) _port = (_protocol == "https") ? 443 : 80;
  _secure = (_protocol == "https");
  _uri = url.length() ? url : "/";

  _transport.reset();
  if (_secure) {
#ifdef USE_WEBCLIENT_HTTPS
    _transport.reset(new BearSslAdapter(_pin1, _hasPin1, _pin2, _hasPin2, _rsaOnly));
#else
    return false;
#endif
  } else {
    _transport.reset(new AsyncTcpAdapter());
  }
  if (_transport) _transport->setTimeout(_tcpTimeout);
  return true;
}

void AsyncHttpClientLight::end() {
  if (_transport) _transport->stop();
  _transport.reset();
  _headers = ""; _base64Authorization = ""; _size = -1; _returnCode = 0; _location = "";
  _bodyPending = false; // BODY_PENDING reset
}

bool AsyncHttpClientLight::connected() {
  return _transport && _transport->connected();
}

bool AsyncHttpClientLight::connectTransport() {
  if (!_transport) return false;
  return _transport->connectHost(_host.c_str(), _port, _connectTimeout);
}

bool AsyncHttpClientLight::ensureConnected() {
  if (!_transport) return false;
  if (_transport->connected()) return true;
  return connectTransport();
}

// ---------- headers add/collect ---------------------------------------

void AsyncHttpClientLight::addHeader(const String& name, const String& value, bool first, bool replace) {
  if (!name.equalsIgnoreCase(F("Connection")) &&
      !name.equalsIgnoreCase(F("User-Agent")) &&
      !name.equalsIgnoreCase(F("Host")) &&
      !(name.equalsIgnoreCase(F("Authorization")) && _base64Authorization.length())) {

    String headerLine = name; headerLine += ": ";

    if (replace) {
      int headerStart = _headers.indexOf(headerLine);
      if (headerStart != -1 && (headerStart == 0 || _headers[headerStart - 1] == '\n')) {
        int headerEnd = _headers.indexOf('\n', headerStart);
        _headers = _headers.substring(0, headerStart) + _headers.substring(headerEnd + 1);
      }
    }

    headerLine += value; headerLine += "\r\n";
    if (first) _headers = headerLine + _headers;
    else       _headers += headerLine;
  }
}

void AsyncHttpClientLight::collectHeaders(const char* headerKeys[], const size_t headerKeysCount) {
  if (_currentHeaders) { delete[] _currentHeaders; _currentHeaders = nullptr; }
  _headerKeysCount = headerKeysCount;
  if (_headerKeysCount) {
    _currentHeaders = new RequestArgument[_headerKeysCount];
    for (size_t i=0;i<_headerKeysCount;i++) _currentHeaders[i].key = headerKeys[i];
  }
}

String AsyncHttpClientLight::header(const char* name) const {
  for (size_t i=0;i<_headerKeysCount;i++) {
    if (_currentHeaders[i].key.equalsIgnoreCase(name)) return _currentHeaders[i].value;
  }
  return String();
}
String AsyncHttpClientLight::header(size_t i) const {
  if (i < _headerKeysCount) return _currentHeaders[i].value;
  return String();
}
String AsyncHttpClientLight::headerName(size_t i) const {
  if (i < _headerKeysCount) return _currentHeaders[i].key;
  return String();
}
bool AsyncHttpClientLight::hasHeader(const char* name) const {
  for (size_t i=0;i<_headerKeysCount;i++) {
    if (_currentHeaders[i].key.equalsIgnoreCase(name) && _currentHeaders[i].value.length() > 0) return true;
  }
  return false;
}

// ---------- robust write (1× reconnect 0-sent headers) ------------

bool AsyncHttpClientLight::writeAllWithOneReconnect(const uint8_t* data, size_t len, bool allow_retry_if_zero_sent) {
  if (!_transport) return false;

  auto try_send = [&](const uint8_t* p, size_t l, size_t& sent_total)->bool {
    uint32_t start = millis();
    sent_total = 0;
    const uint8_t* cur = p;
    size_t left = l;
    while (left) {
      if (!_transport->connected()) return false;
      size_t s = _transport->write(cur, left);
      if (s > 0) {
        cur += s; left -= s; sent_total += s;
        start = millis();
      } else {
        delay(1);
        if ((millis()-start) > (uint32_t)_tcpTimeout) return false;
      }
    }
    return true;
  };

  size_t first_sent = 0;
  bool ok = try_send(data, len, first_sent);
  if (ok) return true;

  if (allow_retry_if_zero_sent && first_sent == 0 && _reuse) {
    _transport->stop();
    if (!connectTransport()) return false;
    size_t second_sent = 0;
    return try_send(data, len, second_sent);
  }
  return false;
}

// ---------- send header line -------------------------------------------

bool AsyncHttpClientLight::sendHeader(const char * method) {
  // BODY_PENDING
  if (_bodyPending) {
    if (_transport) _transport->stop();
    _bodyPending = false;
  }

  if (!ensureConnected()) return false;

  String header = String(method) + " " + _uri + F(" HTTP/1.");
  header += _useHTTP10 ? "0" : "1";

  header += String(F("\r\nHost: ")) + _host;
  if (_port != 80 && _port != 443) { header += ':'; header += String(_port); }
  header += String(F("\r\nUser-Agent: ")) + _userAgent +
            F("\r\nConnection: ");
  header += (_reuse ? F("keep-alive") : F("close"));
  header += "\r\n";

  // identities only
  if (!_useHTTP10) {
    header += F("Accept-Encoding: identity\r\n");
  }

  if (_base64Authorization.length()) {
    String auth = _base64Authorization; auth.replace("\n", "");
    header += F("Authorization: Basic "); header += auth; header += "\r\n";
  }

  header += _headers + "\r\n";

  return writeAllWithOneReconnect((const uint8_t*)header.c_str(), header.length(), /*allow_retry_if_zero_sent=*/true);
}

// ---------- handle response headers ------------------------------------

int AsyncHttpClientLight::handleHeaderResponse() {
  if (!_transport || !_transport->connected()) return HTTPC_ERROR_NOT_CONNECTED;

  String te;
  _canReuse = _reuse;
  _transferEncoding = HTTPC_TE_IDENTITY;

  auto readLine = [&](String& out)->bool{
    out = "";
    uint32_t start = millis();
    char c;
    while (true) {
      int avail = _transport->available();
      if (avail <= 0) {
        delay(1);
        if ((millis()-start) > (uint32_t)_tcpTimeout) return false;
        continue;
      }
      int r = _transport->read((uint8_t*)&c, 1);
      if (r == 1) {
        if (c == '\r') continue;
        if (c == '\n') return true;
        out += c;
      }
    }
  };

  bool firstLine = true;
  _size = -1;
  _returnCode = 0;
  _location = "";

  while (_transport->connected()) {
    String line;
    if (!readLine(line)) return HTTPC_ERROR_READ_TIMEOUT;

    if (line.length() == 0) {
      if (te.length()) {
        if (te.equalsIgnoreCase("chunked"))      _transferEncoding = HTTPC_TE_CHUNKED;
        else if (te.equalsIgnoreCase("identity")) _transferEncoding = HTTPC_TE_IDENTITY;
        else return HTTPC_ERROR_ENCODING;
      } else _transferEncoding = HTTPC_TE_IDENTITY;

      // BODY_PENDING: by headers
      _bodyPending = (_transferEncoding == HTTPC_TE_CHUNKED) || (_size > 0);

      return _returnCode ? _returnCode : HTTPC_ERROR_NO_HTTP_SERVER;
    }

    if (firstLine) {
      firstLine = false;
      if (_canReuse && line.startsWith("HTTP/1.")) _canReuse = (line[sizeof "HTTP/1." - 1] != '0');
      int cp = line.indexOf(' ');
      if (cp > 0) {
        int next = line.indexOf(' ', cp+1);
        _returnCode = line.substring(cp+1, next > 0 ? next : line.length()).toInt();
      }
      continue;
    }

    int colon = line.indexOf(':');
    if (colon > 0) {
      String name = line.substring(0, colon);
      String value = line.substring(colon+1); value.trim();

      if (name.equalsIgnoreCase("Content-Length")) _size = value.toInt();
      if (_canReuse && name.equalsIgnoreCase("Connection")) {
        if (value.indexOf("close") >= 0 && value.indexOf("keep-alive") < 0) _canReuse = false;
      }
      if (name.equalsIgnoreCase("Transfer-Encoding")) te = value;
      if (name.equalsIgnoreCase("Location")) _location = value;

      for (size_t i=0;i<_headerKeysCount;i++) {
        if (_currentHeaders[i].key.equalsIgnoreCase(name)) {
          _currentHeaders[i].value = value;
          break;
        }
      }
    }
  }
  return HTTPC_ERROR_CONNECTION_LOST;
}

// ---------- body read (sync) -------------------------------------------

int AsyncHttpClientLight::writeToStreamDataBlock(Stream * stream, int size) {
  int len = size;
  int bytesWritten = 0;
  uint8_t buf[HTTP_TCP_BUFFER_SIZE];

  while (_transport && _transport->connected() && (len > 0 || len == -1)) {
    int avail = _transport->available();
    if (avail <= 0) { delay(1); continue; }
    int wish = avail;
    if (len > 0 && wish > len) wish = len;
    if (wish > (int)sizeof(buf)) wish = sizeof(buf);
    int r = _transport->read(buf, wish);
    if (r > 0) {
      int w = stream->write(buf, r);
      if (w != r) return HTTPC_ERROR_STREAM_WRITE;
      bytesWritten += w;
      if (len > 0) len -= r;
    }
  }

  if ((size > 0) && (size != bytesWritten)) return HTTPC_ERROR_STREAM_WRITE;
  return bytesWritten;
}

int AsyncHttpClientLight::writeToStream(Stream * stream) {
  // FAST-PATH: async DONE → from internal buffer
  if (_asyncState == ASYNC_DONE) {
    const String& s = _asyncBody;
    size_t n = s.length();
    if (n == 0) return 0;
    size_t w = stream->write((const uint8_t*)s.c_str(), n);
    return (w == n) ? (int)n : HTTPC_ERROR_STREAM_WRITE;
  }

  _lastStream = stream;
  IntOpArg a{ this };
  return runJob(&Job_WriteToStream, &a);
}

String AsyncHttpClientLight::getString(void) {
  // FAST-PATH: async DONE → return from internal buffer
  if (_asyncState == ASYNC_DONE) {
    return String(_asyncBody);
  }

  _lastString = String();
  IntOpArg a{ this };
  int rc = runJob(&Job_GetString, &a);
  if (rc < 0) return String();
  String out = _lastString;
  _lastString = String();
  return out;
}

// ---------- requests (sync) --------------------------------------------

int AsyncHttpClientLight::POST(uint8_t * payload, size_t size) {
  SendBufArg a{ this, "POST", payload, size };
  return runJob(&Job_SendBuf, &a);
}

int AsyncHttpClientLight::POST(String payload) {
  SendStrArg a{ this, "POST", payload };
  return runJob(&Job_SendStr, &a);
}

int AsyncHttpClientLight::PUT(uint8_t * payload, size_t size) {
  SendBufArg a{ this, "PUT", payload, size };
  return runJob(&Job_SendBuf, &a);
}

int AsyncHttpClientLight::PUT(String payload) {
  SendStrArg a{ this, "PUT", payload };
  return runJob(&Job_SendStr, &a);
}

int AsyncHttpClientLight::PATCH(uint8_t * payload, size_t size) {
  SendBufArg a{ this, "PATCH", payload, size };
  return runJob(&Job_SendBuf, &a);
}

int AsyncHttpClientLight::PATCH(String payload) {
  SendStrArg a{ this, "PATCH", payload };
  return runJob(&Job_SendStr, &a);
}

int AsyncHttpClientLight::DELETE(uint8_t * payload, size_t size) {
  SendBufArg a{ this, "DELETE", payload, size };
  return runJob(&Job_SendBuf, &a);
}

int AsyncHttpClientLight::DELETE(String payload) {
  SendStrArg a{ this, "DELETE", payload };
  return runJob(&Job_SendStr, &a);
}

int AsyncHttpClientLight::sendRequest(const char * method, String payload) {
  SendStrArg a{ this, method, payload };
  return runJob(&Job_SendStr, &a);
}
int AsyncHttpClientLight::sendRequest(const char * method, uint8_t * payload, size_t size) {
  SendBufArg a{ this, method, payload, size };
  return runJob(&Job_SendBuf, &a);
}
int AsyncHttpClientLight::sendRequest(const char * method, Stream * stream, size_t size) {
  SendStrmArg a{ this, method, stream, size };
  return runJob(&Job_SendStrm, &a);
}
int AsyncHttpClientLight::GET() { IntOpArg a{ this }; return runJob(&Job_GET, &a); }

// ---------- error text --------------------------------------------------

String AsyncHttpClientLight::errorToString(int error) {
  switch (error) {
    case HTTPC_ERROR_CONNECTION_REFUSED: return F("connection refused");
    case HTTPC_ERROR_SEND_HEADER_FAILED: return F("send header failed");
    case HTTPC_ERROR_SEND_PAYLOAD_FAILED:return F("send payload failed");
    case HTTPC_ERROR_NOT_CONNECTED:      return F("not connected");
    case HTTPC_ERROR_CONNECTION_LOST:    return F("connection lost");
    case HTTPC_ERROR_NO_STREAM:          return F("no stream");
    case HTTPC_ERROR_NO_HTTP_SERVER:     return F("no HTTP server");
    case HTTPC_ERROR_TOO_LESS_RAM:       return F("too less ram");
    case HTTPC_ERROR_ENCODING:           return F("Transfer-Encoding not supported");
    case HTTPC_ERROR_STREAM_WRITE:       return F("Stream write error");
    case HTTPC_ERROR_READ_TIMEOUT:       return F("read Timeout");
    default: return String();
  }
}

// ---------- redirects / URL --------------------------------------------

bool AsyncHttpClientLight::setURL(const String& url) {
  if (url.length() && url[0] == '/') {
    _uri = url;
    _headers = ""; _size = -1; _returnCode = 0; _location = "";
    return true;
  }
  if (!url.startsWith(_protocol + ':')) return false;
  return beginInternal(url, _protocol.c_str());
}

// ======================== RTOS worker infra ============================
#ifdef ESP32

void AsyncHttpClientLight::WorkerTask(void* pv) {
  Job* job = reinterpret_cast<Job*>(pv);
  int res = HTTPC_ERROR_NOT_CONNECTED;
  if (job && job->fn) res = job->fn(job->arg);
  if (job) {
    job->result = res;
    if (job->done) xSemaphoreGive(job->done);
  }
  vTaskDelete(nullptr);
}

int AsyncHttpClientLight::runJob(Job::Fn fn, void* arg) {
  if (_busy) xSemaphoreTake(_busy, portMAX_DELAY);

  Job job;
  job.fn = fn;
  job.arg = arg;
  job.result = HTTPC_ERROR_NOT_CONNECTED;
  job.done = xSemaphoreCreateBinary();
  if (!job.done) {
    if (_busy) xSemaphoreGive(_busy);
    return HTTPC_ERROR_TOO_LESS_RAM;
  }

  TaskHandle_t th = nullptr;
  BaseType_t ok = xTaskCreate(
      &WorkerTask, "AHttpWkr",
      ASYNCHTTP_WKR_STACK / sizeof(StackType_t),
      &job, tskIDLE_PRIORITY + 1, &th);

  if (ok != pdPASS || th == nullptr) {
    vSemaphoreDelete(job.done);
    if (_busy) xSemaphoreGive(_busy);
    return HTTPC_ERROR_TOO_LESS_RAM;
  }

  while (true) {
    if (xSemaphoreTake(job.done, pdMS_TO_TICKS(_waitSliceMs)) == pdTRUE) break;
    delay(0); // feed WDT
  }

  vSemaphoreDelete(job.done);
  if (_busy) xSemaphoreGive(_busy);

  return job.result;
}

// Non-blocking job – 1 queued, 0 busy, <0 error
int AsyncHttpClientLight::startJob(Job::Fn fn, void* arg) {
  if (!_busy) return HTTPC_ERROR_NOT_CONNECTED;

  if (_asyncState == ASYNC_RUNNING) return 0;

  xSemaphoreTake(_busy, portMAX_DELAY);

  // long live Job on heap – released in asyncState() after done
  _asyncJob = ahcl_new_obj<Job>();
  if (!_asyncJob) {
    xSemaphoreGive(_busy);
    return HTTPC_ERROR_TOO_LESS_RAM;
  }
  _asyncJob->fn = fn;
  _asyncJob->arg = arg;
  _asyncJob->result = HTTPC_ERROR_NOT_CONNECTED;
  _asyncDone = xSemaphoreCreateBinary();
  if (!_asyncDone) {
    ahcl_delete_obj(_asyncJob);
    xSemaphoreGive(_busy);
    return HTTPC_ERROR_TOO_LESS_RAM;
  }
  _asyncJob->done = _asyncDone;

  _asyncTask = nullptr;
  BaseType_t ok = xTaskCreate(
      &WorkerTask, "AHttpWkrA",
      ASYNCHTTP_WKR_STACK / sizeof(StackType_t),
      _asyncJob, tskIDLE_PRIORITY + 1, &_asyncTask);

  if (ok != pdPASS || _asyncTask == nullptr) {
    vSemaphoreDelete(_asyncDone); _asyncDone = nullptr;
    ahcl_delete_obj(_asyncJob);
    xSemaphoreGive(_busy);
    return HTTPC_ERROR_TOO_LESS_RAM;
  }

  // we run async
  return 1;
}
#endif // ESP32

// ======================== Job implements (sync) =====================

int AsyncHttpClientLight::Job_GET(void* arg) {
  auto* a = reinterpret_cast<IntOpArg*>(arg);
  if (!a || !a->self) return HTTPC_ERROR_NOT_CONNECTED;

  for (size_t i=0;i<a->self->_headerKeysCount;i++) {
    if (a->self->_currentHeaders[i].value.length()) a->self->_currentHeaders[i].value.clear();
  }

  if (!a->self->sendHeader("GET")) return HTTPC_ERROR_SEND_HEADER_FAILED;
  return a->self->handleHeaderResponse();
}

int AsyncHttpClientLight::Job_SendBuf(void* arg) {
  auto* a = reinterpret_cast<SendBufArg*>(arg);
  if (!a || !a->self) return HTTPC_ERROR_NOT_CONNECTED;

  int code = HTTPC_ERROR_NOT_CONNECTED;
  bool redirect = false;
  uint16_t redirectCount = 0;
  const char* method = a->method;
  uint8_t* payload = a->payload;
  size_t size = a->size;

  do {
    for (size_t i = 0; i < a->self->_headerKeysCount; i++) {
      if (a->self->_currentHeaders[i].value.length()) a->self->_currentHeaders[i].value.clear();
    }
    a->self->_returnCode = 0;
    a->self->_size = -1;
    a->self->_location = "";
    a->self->_transferEncoding = HTTPC_TE_IDENTITY;

    if (payload && size > 0) {
      a->self->addHeader(F("Content-Length"), String(size), false, true);
    }

    if (!a->self->sendHeader(method)) {
      return HTTPC_ERROR_SEND_HEADER_FAILED;
    }

    // payload
    if (payload && size > 0) {
      uint8_t* p = payload;
      size_t left = size;
      uint32_t start = millis();
      while (left) {
        if (!a->self->connected()) return HTTPC_ERROR_CONNECTION_LOST;
        size_t w = a->self->_transport->write(p, left);
        if (w > 0) { p += w; left -= w; start = millis(); }
        else {
          delay(1);
          if ((millis() - start) > (uint32_t)a->self->_tcpTimeout) {
            return HTTPC_ERROR_SEND_PAYLOAD_FAILED;
          }
        }
      }
    }

    code = a->self->handleHeaderResponse();

    redirect = false;
    if (a->self->_followRedirects != HTTPC_DISABLE_FOLLOW_REDIRECTS &&
        redirectCount < a->self->_redirectLimit &&
        a->self->_location.length() > 0) {
      switch (code) {
        case 301: case 307:
          if (a->self->_followRedirects == HTTPC_FORCE_FOLLOW_REDIRECTS ||
              !strcmp(method, "GET") || !strcmp(method, "HEAD")) {
            redirectCount++;
            if (!a->self->setURL(a->self->_location)) break;
            redirect = true;
          }
          break;
        case 302: case 303:
          redirectCount++;
          if (!a->self->setURL(a->self->_location)) break;
          method = "GET";
          payload = nullptr;
          size = 0;
          redirect = true;
          break;
        default:
          break;
      }
    }
  } while (redirect);

  return code;
}

int AsyncHttpClientLight::Job_SendStr(void* arg) {
  auto* a = reinterpret_cast<SendStrArg*>(arg);
  if (!a || !a->self) return HTTPC_ERROR_NOT_CONNECTED;
  int code;
  bool redirect=false; uint16_t redirectCount=0;
  const char* method = a->method;
  String payload = a->payload;
  do {
    for (size_t i=0;i<a->self->_headerKeysCount;i++) if (a->self->_currentHeaders[i].value.length()) a->self->_currentHeaders[i].value.clear();
    a->self->_returnCode = 0; a->self->_size = -1; a->self->_location = ""; a->self->_transferEncoding = HTTPC_TE_IDENTITY;

    if (payload.length() > 0) a->self->addHeader(F("Content-Length"), String(payload.length()), false, true);

    if (!a->self->sendHeader(method)) return HTTPC_ERROR_SEND_HEADER_FAILED;

    // telo
    if (payload.length() > 0) {
      const uint8_t* p = (const uint8_t*)payload.c_str(); size_t left = payload.length();
      uint32_t start = millis();
      while (left) {
        if (!a->self->connected()) return HTTPC_ERROR_CONNECTION_LOST;
        size_t w = a->self->_transport->write(p, left);
        if (w > 0) { p += w; left -= w; start = millis(); }
        else {
          delay(1);
          if ((millis()-start) > (uint32_t)a->self->_tcpTimeout) return HTTPC_ERROR_SEND_PAYLOAD_FAILED;
        }
      }
    }

    code = a->self->handleHeaderResponse();

    redirect = false;
    if (a->self->_followRedirects != HTTPC_DISABLE_FOLLOW_REDIRECTS &&
        redirectCount < a->self->_redirectLimit &&
        a->self->_location.length() > 0) {
      switch (code) {
        case 301: case 307:
          if (a->self->_followRedirects == HTTPC_FORCE_FOLLOW_REDIRECTS ||
              !strcmp(method, "GET") || !strcmp(method, "HEAD")) {
            redirectCount++;
            if (!a->self->setURL(a->self->_location)) break;
            redirect = true;
          }
          break;
        case 302: case 303:
          redirectCount++;
          if (!a->self->setURL(a->self->_location)) break;
          method = "GET";
          payload = String();
          redirect = true;
          break;
        default: break;
      }
    }
  } while (redirect);
  return code;
}

int AsyncHttpClientLight::Job_SendStrm(void* arg) {
  auto* a = reinterpret_cast<SendStrmArg*>(arg);
  if (!a || !a->self || !a->stream) return HTTPC_ERROR_NO_STREAM;

  if(!a->self->ensureConnected()) return HTTPC_ERROR_CONNECTION_REFUSED;
  if(a->size > 0) a->self->addHeader("Content-Length", String(a->size), false, true);
  if(!a->self->sendHeader(a->method)) return HTTPC_ERROR_SEND_HEADER_FAILED;

  int buff_size = HTTP_TCP_BUFFER_SIZE;
  int len = a->size;
  if(len == 0) len = -1;
  if((len > 0) && (len < HTTP_TCP_BUFFER_SIZE)) buff_size = len;

  std::unique_ptr<uint8_t, AhclFreeDeleter> buff(
    (uint8_t*)std::malloc(buff_size)
  );
  if (!buff) return HTTPC_ERROR_TOO_LESS_RAM;

  int bytesWritten = 0;
  while(a->self->connected() && (a->stream->available() > -1) && (len > 0 || len == -1)) {
    int sizeAvailable = a->stream->available();
    if(sizeAvailable) {
      int readBytes = sizeAvailable;
      if(len > 0 && readBytes > len) readBytes = len;
      if(readBytes > buff_size) readBytes = buff_size;

      int bytesRead = a->stream->readBytes(buff.get(), readBytes);

      uint32_t start = millis();
      int left = bytesRead;
      const uint8_t* p = buff.get();
      while (left > 0) {
        if (!a->self->connected()) return HTTPC_ERROR_CONNECTION_LOST;
        int w = (int)a->self->_transport->write(p, left);
        if (w > 0) { p += w; left -= w; bytesWritten += w; start = millis(); }
        else {
          delay(1);
          if ((millis()-start) > (uint32_t)a->self->_tcpTimeout) return HTTPC_ERROR_SEND_PAYLOAD_FAILED;
        }
      }
    } else {
      delay(1);
    }
  }

  int rc = a->self->handleHeaderResponse();
  return rc;
}

int AsyncHttpClientLight::Job_WriteToStream(void* arg) {
  auto* a = reinterpret_cast<IntOpArg*>(arg);
  if (!a || !a->self || !a->self->_lastStream) return HTTPC_ERROR_NO_STREAM;

  int ret = 0;

  if (a->self->_transferEncoding == HTTPC_TE_IDENTITY) {
    int r = a->self->writeToStreamDataBlock(a->self->_lastStream, a->self->_size);
    if (r < 0) return r;
    ret = r;
  } else if (a->self->_transferEncoding == HTTPC_TE_CHUNKED) {
    auto readLine = [&](String& out)->bool {
      out = "";
      char c;
      uint32_t start = millis();
      while (true) {
        int avail = a->self->_transport ? a->self->_transport->available() : 0;
        if (avail <= 0) {
          delay(1);
          if ((millis() - start) > (uint32_t)a->self->_tcpTimeout) return false;
          continue;
        }
        int r = a->self->_transport->read((uint8_t*)&c, 1);
        if (r == 1) {
          if (c == '\r') continue;
          if (c == '\n') return true;
          out += c;
        }
      }
    };

    while (true) {
      // chunk size line
      String h;
      if (!readLine(h)) {
        if (a->self->_transport) a->self->_transport->stop();
        a->self->_bodyPending = false;
        return HTTPC_ERROR_READ_TIMEOUT;
      }
      h.trim();
      int chunk = (int) strtol(h.c_str(), nullptr, 16);

      if (chunk <= 0) {
        String tmp;
        if (!readLine(tmp)) {
          if (a->self->_transport) a->self->_transport->stop();
          a->self->_bodyPending = false;
          return HTTPC_ERROR_READ_TIMEOUT;
        }
        while (true) {
          String t;
          if (!readLine(t)) {
            if (a->self->_transport) a->self->_transport->stop();
            a->self->_bodyPending = false;
            return HTTPC_ERROR_READ_TIMEOUT;
          }
          if (t.length() == 0) break;
        }
        break; // end
      }

      int remaining = chunk;
      while (remaining > 0) {
        uint8_t buf[HTTP_TCP_BUFFER_SIZE];
        int wish = remaining;
        if (wish > (int)sizeof(buf)) wish = sizeof(buf);

        uint32_t start = millis();
        while (a->self->_transport->available() <= 0) {
          delay(1);
          if ((millis() - start) > (uint32_t)a->self->_tcpTimeout) {
            if (a->self->_transport) a->self->_transport->stop();
            a->self->_bodyPending = false;
            return HTTPC_ERROR_READ_TIMEOUT;
          }
        }

        int avail = a->self->_transport->available();
        if (wish > avail) wish = avail;

        int r = a->self->_transport->read(buf, wish);
        if (r > 0) {
          int w = a->self->_lastStream->write(buf, r);
          if (w != r) return HTTPC_ERROR_STREAM_WRITE;
          ret += r;
          remaining -= r;
        }
      }

      // CRLF after chunks
      char crlf[2]; int got = 0;
      uint32_t t2 = millis();
      while (got < 2) {
        if ((millis() - t2) > (uint32_t)a->self->_tcpTimeout) {
          if (a->self->_transport) a->self->_transport->stop();
          a->self->_bodyPending = false;
          return HTTPC_ERROR_READ_TIMEOUT;
        }
        if (a->self->_transport->available() <= 0) { delay(1); continue; }
        int r = a->self->_transport->read((uint8_t*)&crlf[got], 1);
        if (r == 1) got++;
      }
      if (crlf[0] != '\r' || crlf[1] != '\n') return HTTPC_ERROR_READ_TIMEOUT;
    }
  } else {
    return HTTPC_ERROR_ENCODING;
  }

  // BODY 
  a->self->_bodyPending = false;

  if (!a->self->_reuse || !a->self->_canReuse) {
    a->self->_transport->stop();
  }
  return ret;
}

int AsyncHttpClientLight::Job_GetString(void* arg) {
  auto* a = reinterpret_cast<IntOpArg*>(arg);
  if (!a || !a->self) return HTTPC_ERROR_NOT_CONNECTED;

  StreamString s;
  int expect = a->self->_size;
  if (expect < 0) expect = 1024;
  (void)s.reserve(expect + 1);

  int ret = 0;

  if (a->self->_transferEncoding == HTTPC_TE_IDENTITY) {
    int r = a->self->writeToStreamDataBlock(&s, a->self->_size);
    if (r < 0) return r;
    ret = r;
  } else if (a->self->_transferEncoding == HTTPC_TE_CHUNKED) {
    auto readLine = [&](String& out)->bool {
      out = "";
      char c;
      uint32_t start = millis();
      while (true) {
        int avail = a->self->_transport ? a->self->_transport->available() : 0;
        if (avail <= 0) {
          delay(1);
          if ((millis() - start) > (uint32_t)a->self->_tcpTimeout) return false;
          continue;
        }
        int r = a->self->_transport->read((uint8_t*)&c, 1);
        if (r == 1) {
          if (c == '\r') continue;
          if (c == '\n') return true;
          out += c;
        }
      }
    };

    while (true) {
      String h;
      if (!readLine(h)) {
        if (a->self->_transport) a->self->_transport->stop();
        a->self->_bodyPending = false;
        return HTTPC_ERROR_READ_TIMEOUT;
      }
      h.trim();
      int chunk = (int) strtol(h.c_str(), nullptr, 16);

      if (chunk <= 0) {
        String tmp;
        if (!readLine(tmp)) {
          if (a->self->_transport) a->self->_transport->stop();
          a->self->_bodyPending = false;
          return HTTPC_ERROR_READ_TIMEOUT;
        }
        while (true) {
          String t;
          if (!readLine(t)) {
            if (a->self->_transport) a->self->_transport->stop();
            a->self->_bodyPending = false;
            return HTTPC_ERROR_READ_TIMEOUT;
          }
          if (t.length() == 0) break;
        }
        break; // end
      }

      int remaining = chunk;
      while (remaining > 0) {
        uint8_t buf[HTTP_TCP_BUFFER_SIZE];
        int wish = remaining;
        if (wish > (int)sizeof(buf)) wish = sizeof(buf);

        uint32_t start = millis();
        while (a->self->_transport->available() <= 0) {
          delay(1);
          if ((millis() - start) > (uint32_t)a->self->_tcpTimeout) {
            if (a->self->_transport) a->self->_transport->stop();
            a->self->_bodyPending = false;
            return HTTPC_ERROR_READ_TIMEOUT;
          }
        }

        int avail = a->self->_transport->available();
        if (wish > avail) wish = avail;

        int r = a->self->_transport->read(buf, wish);
        if (r > 0) {
          int w = s.write(buf, r);
          if (w != r) return HTTPC_ERROR_STREAM_WRITE;
          ret += r;
          remaining -= r;
        }
      }

      // CRLF after chunk
      char crlf[2]; int got = 0;
      uint32_t t2 = millis();
      while (got < 2) {
        if ((millis() - t2) > (uint32_t)a->self->_tcpTimeout) {
          if (a->self->_transport) a->self->_transport->stop();
          a->self->_bodyPending = false;
          return HTTPC_ERROR_READ_TIMEOUT;
        }
        if (a->self->_transport->available() <= 0) { delay(1); continue; }
        int r = a->self->_transport->read((uint8_t*)&crlf[got], 1);
        if (r == 1) got++;
      }
      if (crlf[0] != '\r' || crlf[1] != '\n') return HTTPC_ERROR_READ_TIMEOUT;
    }
  } else {
    return HTTPC_ERROR_ENCODING;
  }

  a->self->_bodyPending = false;

  if (!a->self->_reuse || !a->self->_canReuse) {
    a->self->_transport->stop();
  }

  a->self->_lastString = (String)s;
  return 0;
}

// ======================== Async „lite“ implement ===================

// Helper routine: read the entire BODY into a StreamString with a size limit and abort check.
// Returns the number of bytes read, or <0 on error.
int AsyncHttpClientLight::readBodyToStreamString(StreamString& s, int size_limit) {
  int read_total = 0;

  auto bail = [&](int err)->int{
    if (_transport) _transport->stop();
    _bodyPending = false;
    return err;
  };

  auto push = [&](const uint8_t* buf, int n)->bool{
    if (n <= 0) return true;
    int left = n;
    while (left) {
      size_t w = s.write(buf, left);
      if ((int)w <= 0) return false;
      buf += w; left -= (int)w;
    }
    return true;
  };

  // identity
  if (_transferEncoding == HTTPC_TE_IDENTITY) {
    int remaining = _size;
    if (remaining < 0) remaining = size_limit; // max to limit if we dont know content height

    while (remaining != 0) {
      if (_asyncAbortReq) return bail(HTTPC_ERROR_CONNECTION_LOST);

      int avail = _transport ? _transport->available() : 0;
      if (avail <= 0) { delay(1); continue; }

      if (avail > remaining && remaining > 0) avail = remaining;
      if (avail > HTTP_TCP_BUFFER_SIZE) avail = HTTP_TCP_BUFFER_SIZE;

      uint8_t buf[HTTP_TCP_BUFFER_SIZE];
      int r = _transport->read(buf, avail);
      if (r > 0) {
        // limit
        int room = size_limit - read_total;
        if (room <= 0) { // limit done - close
          return bail(0);
        }
        int take = (r > room) ? room : r;
        if (!push(buf, take)) return bail(HTTPC_ERROR_STREAM_WRITE);
        read_total += take;
        if (remaining > 0) remaining -= r;
      }
    }
  }
  // chunked
  else if (_transferEncoding == HTTPC_TE_CHUNKED) {
    auto readLine = [&](String& out)->bool {
      out = "";
      char c;
      uint32_t start = millis();
      while (true) {
        if (_asyncAbortReq) return false;
        int avail = _transport ? _transport->available() : 0;
        if (avail <= 0) {
          delay(1);
          if ((millis() - start) > (uint32_t)_tcpTimeout) return false;
          continue;
        }
        int r = _transport->read((uint8_t*)&c, 1);
        if (r == 1) { if (c == '\r') continue; if (c == '\n') return true; out += c; }
      }
    };

    while (true) {
      if (_asyncAbortReq) return bail(HTTPC_ERROR_CONNECTION_LOST);
      String h; if (!readLine(h)) return bail(HTTPC_ERROR_READ_TIMEOUT);
      h.trim();
      int chunk = (int) strtol(h.c_str(), nullptr, 16);
      if (chunk <= 0) {
        String tmp;
        if (!readLine(tmp)) return bail(HTTPC_ERROR_READ_TIMEOUT);
        while (true) { String t; if (!readLine(t)) return bail(HTTPC_ERROR_READ_TIMEOUT); if (t.length() == 0) break; }
        break; // end
      }

      int remaining = chunk;
      while (remaining > 0) {
        if (_asyncAbortReq) return bail(HTTPC_ERROR_CONNECTION_LOST);
        uint8_t buf[HTTP_TCP_BUFFER_SIZE];
        int wish = remaining;
        if (wish > (int)sizeof(buf)) wish = sizeof(buf);

        uint32_t start = millis();
        while (_transport->available() <= 0) {
          if (_asyncAbortReq) return bail(HTTPC_ERROR_CONNECTION_LOST);
          delay(1);
          if ((millis() - start) > (uint32_t)_tcpTimeout) return bail(HTTPC_ERROR_READ_TIMEOUT);
        }

        int avail = _transport->available();
        if (wish > avail) wish = avail;

        int r = _transport->read(buf, wish);
        if (r > 0) {
          int room = size_limit - read_total;
          if (room <= 0) return bail(0);
          int take = (r > room) ? room : r;
          if (!push(buf, take)) return bail(HTTPC_ERROR_STREAM_WRITE);
          read_total += take;
          remaining -= r;
        }
      }

      // CRLF za chunkom
      char crlf[2]; int got = 0;
      uint32_t t2 = millis();
      while (got < 2) {
        if (_asyncAbortReq) return bail(HTTPC_ERROR_CONNECTION_LOST);
        if ((millis() - t2) > (uint32_t)_tcpTimeout) return bail(HTTPC_ERROR_READ_TIMEOUT);
        if (_transport->available() <= 0) { delay(1); continue; }
        int r = _transport->read((uint8_t*)&crlf[got], 1);
        if (r == 1) got++;
      }
      if (crlf[0] != '\r' || crlf[1] != '\n') return bail(HTTPC_ERROR_READ_TIMEOUT);
    }
  } else {
    return HTTPC_ERROR_ENCODING;
  }

  // body read done
  _bodyPending = false;
  return read_total;
}


// ---------- Async jobs --------------------------------------------------

int AsyncHttpClientLight::Job_AsyncGET(void* arg) {
  auto* a = reinterpret_cast<AsyncGetArg*>(arg);
  if (!a || !a->self) return HTTPC_ERROR_NOT_CONNECTED;
  AsyncHttpClientLight* self = a->self;
  ahcl_delete_obj(a); // arg on heap – release

  // reset per-request
  for (size_t i=0;i<self->_headerKeysCount;i++) if (self->_currentHeaders[i].value.length()) self->_currentHeaders[i].value.clear();
  self->_returnCode = 0; self->_size = -1; self->_location = "";
  self->_transferEncoding = HTTPC_TE_IDENTITY;

  if (!self->sendHeader("GET")) { self->_asyncHttpCode = HTTPC_ERROR_SEND_HEADER_FAILED; return HTTPC_ERROR_SEND_HEADER_FAILED; }
  int code = self->handleHeaderResponse();
  self->_asyncHttpCode = code;
  if (code < 0) return code;

  // buffer limit
  self->_asyncBody = StreamString();
  int expected = (self->_size > 0) ? self->_size : 1024;
  if (expected > (int)AsyncHttpClientLight::kAsyncBodyLimit) expected = AsyncHttpClientLight::kAsyncBodyLimit;
  (void)self->_asyncBody.reserve(expected + 1);

  int r = self->readBodyToStreamString(self->_asyncBody, (int)AsyncHttpClientLight::kAsyncBodyLimit);
  if (r < 0) { self->_asyncRxBytes = r; return r; }
  self->_asyncRxBytes = r;

  if (!self->_reuse || !self->_canReuse) {
    if (self->_transport) self->_transport->stop();
  }
  return 0;
}

int AsyncHttpClientLight::Job_AsyncPOST_Buf(void* arg) {
  auto* a = reinterpret_cast<AsyncPostBufArg*>(arg);
  if (!a || !a->self) return HTTPC_ERROR_NOT_CONNECTED;
  AsyncHttpClientLight* self = a->self;

  // no payload copy, reading from pointer
  uint8_t* payload = a->payload;
  size_t   size    = a->size;
  ahcl_delete_obj(a); // arg on heap

  // HEADERS
  for (size_t i=0;i<self->_headerKeysCount;i++) if (self->_currentHeaders[i].value.length()) self->_currentHeaders[i].value.clear();
  self->_returnCode = 0; self->_size = -1; self->_location = "";
  self->_transferEncoding = HTTPC_TE_IDENTITY;

  if (payload && size > 0) self->addHeader(F("Content-Length"), String(size), false, true);

  if (!self->sendHeader("POST")) { self->_asyncHttpCode = HTTPC_ERROR_SEND_HEADER_FAILED; return HTTPC_ERROR_SEND_HEADER_FAILED; }

  // BODY
  if (payload && size > 0) {
    const uint8_t* p = payload; size_t left = size;
    uint32_t start = millis();
    while (left) {
      if (self->_asyncAbortReq) { if (self->_transport) self->_transport->stop(); self->_asyncHttpCode = HTTPC_ERROR_CONNECTION_LOST; return HTTPC_ERROR_CONNECTION_LOST; }
      if (!self->connected()) return HTTPC_ERROR_CONNECTION_LOST;
      size_t w = self->_transport->write(p, left);
      if (w > 0) { p += w; left -= w; start = millis(); }
      else {
        delay(1);
        if ((millis()-start) > (uint32_t)self->_tcpTimeout) { self->_asyncHttpCode = HTTPC_ERROR_SEND_PAYLOAD_FAILED; return HTTPC_ERROR_SEND_PAYLOAD_FAILED; }
      }
    }
  }

  int code = self->handleHeaderResponse();
  self->_asyncHttpCode = code;
  if (code < 0) return code;

  self->_asyncBody = StreamString();
  int expected = (self->_size > 0) ? self->_size : 1024;
  if (expected > (int)AsyncHttpClientLight::kAsyncBodyLimit) expected = AsyncHttpClientLight::kAsyncBodyLimit;
  (void)self->_asyncBody.reserve(expected + 1);

  int r = self->readBodyToStreamString(self->_asyncBody, (int)AsyncHttpClientLight::kAsyncBodyLimit);
  if (r < 0) { self->_asyncRxBytes = r; return r; }
  self->_asyncRxBytes = r;

  if (!self->_reuse || !self->_canReuse) {
    if (self->_transport) self->_transport->stop();
  }
  return 0;
}

int AsyncHttpClientLight::Job_AsyncPOST_Str(void* arg) {
  auto* a = reinterpret_cast<AsyncPostStrArg*>(arg);
  if (!a || !a->self) return HTTPC_ERROR_NOT_CONNECTED;
  AsyncHttpClientLight* self = a->self;
  String payload = a->payload;
  ahcl_delete_obj(a);

  for (size_t i=0;i<self->_headerKeysCount;i++) if (self->_currentHeaders[i].value.length()) self->_currentHeaders[i].value.clear();
  self->_returnCode = 0; self->_size = -1; self->_location = "";
  self->_transferEncoding = HTTPC_TE_IDENTITY;

  if (payload.length() > 0) self->addHeader(F("Content-Length"), String(payload.length()), false, true);

  if (!self->sendHeader("POST")) { self->_asyncHttpCode = HTTPC_ERROR_SEND_HEADER_FAILED; return HTTPC_ERROR_SEND_HEADER_FAILED; }

  if (payload.length() > 0) {
    const uint8_t* p = (const uint8_t*)payload.c_str(); size_t left = payload.length();
    uint32_t start = millis();
    while (left) {
      if (self->_asyncAbortReq) { if (self->_transport) self->_transport->stop(); self->_asyncHttpCode = HTTPC_ERROR_CONNECTION_LOST; return HTTPC_ERROR_CONNECTION_LOST; }
      if (!self->connected()) return HTTPC_ERROR_CONNECTION_LOST;
      size_t w = self->_transport->write(p, left);
      if (w > 0) { p += w; left -= w; start = millis(); }
      else {
        delay(1);
        if ((millis()-start) > (uint32_t)self->_tcpTimeout) { self->_asyncHttpCode = HTTPC_ERROR_SEND_PAYLOAD_FAILED; return HTTPC_ERROR_SEND_PAYLOAD_FAILED; }
      }
    }
  }

  int code = self->handleHeaderResponse();
  self->_asyncHttpCode = code;
  if (code < 0) return code;

  self->_asyncBody = StreamString();
  int expected = (self->_size > 0) ? self->_size : 1024;
  if (expected > (int)AsyncHttpClientLight::kAsyncBodyLimit) expected = AsyncHttpClientLight::kAsyncBodyLimit;
  (void)self->_asyncBody.reserve(expected + 1);

  int r = self->readBodyToStreamString(self->_asyncBody, (int)AsyncHttpClientLight::kAsyncBodyLimit);
  if (r < 0) { self->_asyncRxBytes = r; return r; }
  self->_asyncRxBytes = r;

  if (!self->_reuse || !self->_canReuse) {
    if (self->_transport) self->_transport->stop();
  }
  return 0;
}

// ---------- Async API ---------------------------------------------------

int AsyncHttpClientLight::asyncGETStart() {
#ifdef ESP32
  _asyncAbortReq = false;
  _asyncOp = AOP_GET;
  _asyncHttpCode = HTTPC_ERROR_NOT_CONNECTED;
  _asyncRxBytes = -1;
  // arg on heap – release in Job_AsyncGET
  auto* arg = ahcl_new_obj<AsyncGetArg>(this);
  if (!arg) { _asyncState = ASYNC_ERROR; return HTTPC_ERROR_TOO_LESS_RAM; }

  int q = startJob(&Job_AsyncGET, arg);
  if (q <= 0) {
    _asyncState = (q < 0 ? ASYNC_ERROR : ASYNC_IDLE);
    return q;
  }
  _asyncState = ASYNC_RUNNING;
  return 1;
#else
  return HTTPC_ERROR_NOT_CONNECTED;
#endif
}

int AsyncHttpClientLight::asyncPOSTStart(uint8_t* payload, size_t size) {
#ifdef ESP32
  _asyncAbortReq = false;
  _asyncOp = AOP_POST;
  _asyncHttpCode = HTTPC_ERROR_NOT_CONNECTED;
  _asyncRxBytes = -1;
  auto* arg = ahcl_new_obj<AsyncPostBufArg>(this, payload, size);
  if (!arg) { _asyncState = ASYNC_ERROR; return HTTPC_ERROR_TOO_LESS_RAM; }

  int q = startJob(&Job_AsyncPOST_Buf, arg);
  if (q <= 0) {
    _asyncState = (q < 0 ? ASYNC_ERROR : ASYNC_IDLE);
    return q;
  }
  _asyncState = ASYNC_RUNNING;
  return 1;
#else
  return HTTPC_ERROR_NOT_CONNECTED;
#endif
}

int AsyncHttpClientLight::asyncPOSTStart(const String& payload) {
#ifdef ESP32
  _asyncAbortReq = false;
  _asyncOp = AOP_POST;
  _asyncHttpCode = HTTPC_ERROR_NOT_CONNECTED;
  _asyncRxBytes = -1;
  auto* arg = ahcl_new_obj<AsyncPostStrArg>(this, payload);
  if (!arg) { _asyncState = ASYNC_ERROR; return HTTPC_ERROR_TOO_LESS_RAM; }

  int q = startJob(&Job_AsyncPOST_Str, arg);
  if (q <= 0) {
    _asyncState = (q < 0 ? ASYNC_ERROR : ASYNC_IDLE);
    return q;
  }
  _asyncState = ASYNC_RUNNING;
  return 1;
#else
  return HTTPC_ERROR_NOT_CONNECTED;
#endif
}

AsyncHttpClientLight::AsyncState_t
AsyncHttpClientLight::asyncState(int* httpCodeOut, int* bytesOut) const {
#ifdef ESP32
  // const → we still need to touch/update state (poll + cleanup)
  auto* self = const_cast<AsyncHttpClientLight*>(this);

  if (self->_asyncState == ASYNC_RUNNING && self->_asyncDone) {
    // non-blocking poll
    if (xSemaphoreTake(self->_asyncDone, 0) == pdTRUE) {
      // job done
      vSemaphoreDelete(self->_asyncDone); self->_asyncDone = nullptr;

      // result set in v _asyncHttpCode/_asyncRxBytes a _asyncBody
      if (self->_asyncAbortReq) self->_asyncState = ASYNC_ABORTED;
      else self->_asyncState = (self->_asyncHttpCode >= 0 ? ASYNC_DONE : ASYNC_ERROR);

      // release mutex
      if (self->_busy) xSemaphoreGive(self->_busy);

      // release heap jobs
      ahcl_delete_obj(self->_asyncJob);
      self->_asyncTask = nullptr;
    }
  }

  if (httpCodeOut) *httpCodeOut = self->_asyncHttpCode;
  if (bytesOut)    *bytesOut    = self->_asyncRxBytes;
  return self->_asyncState;
#else
  (void)httpCodeOut; (void)bytesOut;
  return ASYNC_ERROR;
#endif
}

void AsyncHttpClientLight::asyncAbort() {
  _asyncAbortReq = true;
}

// ---------- Friend wrapper (public access to private helper) -----------
int ahcl_readBodyToStreamString(AsyncHttpClientLight* self, StreamString& s, int size_limit) {
  if (!self) return HTTPC_ERROR_NOT_CONNECTED;
  return self->readBodyToStreamString(s, size_limit);
}
