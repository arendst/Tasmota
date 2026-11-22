/*
  TasmotaWebServer.h - lib/default/TasmotaWebServer/library.json

  Copyright (C) 2021  Theo Arends & Stephan Hadinger

  This library is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __TASMOTA_WEBSERVER__
#define __TASMOTA_WEBSERVER__

#ifdef ESP8266
#include <ESP8266WebServer.h>

// ms to wait for the request on an idle connection when another client
// already sent data or the pending queue is full (same as esp8266/Arduino#8216)
#ifndef HTTP_MAX_DATA_AVAILABLE_WAIT
#define HTTP_MAX_DATA_AVAILABLE_WAIT 30
#endif

class TasmotaWebServer : public ESP8266WebServer
{
public:
	TasmotaWebServer(int port) : ESP8266WebServer(port)
	{
	}

  bool isChunked(void) const { return _chunked; }

  // Hides ESP8266WebServer::handleClient() (not virtual, callers use TasmotaWebServer*).
  // The core serves one client at a time and waits up to HTTP_MAX_DATA_WAIT (5 s)
  // for the request on the current connection. Browsers open idle speculative
  // connections, so a request ready on another connection can wait 5 s.
  // Backport of esp8266/Arduino#8216: drop the idle connection after
  // HTTP_MAX_DATA_AVAILABLE_WAIT when another client is waiting with data.
  void handleClient(void) {
    if ((HC_WAIT_READ == _currentStatus) &&
        !_currentClient.available() &&
        (millis() - _statusChange > HTTP_MAX_DATA_AVAILABLE_WAIT) &&
        otherClientReady()) {
      _currentClient = ClientType();
      _currentStatus = HC_NONE;
      _currentUpload.reset();
    }
    ESP8266WebServer::handleClient();
  }

protected:
  // true if a queued client already sent data, or the pending queue is full
  bool otherClientReady(void);
};
#endif // ESP8266

#ifdef ESP32
#include <WebServer.h>
#include "libb64/cencode.h"

class TasmotaWebServer : public WebServer
{
public:
	TasmotaWebServer(int port) :WebServer(port)
	{
	}

  bool isChunked(void) const { return _chunked; }

  // Basic authentication only. These hide WebServer::authenticate() and
  // WebServer::requestAuthentication(), which also implement Digest auth.
  // Since Arduino 3.3.12-1 Digest auth links HMAC-SHA256 and SHA-512 code (~7 KB flash)
  // even when only Basic auth is used.
  bool authenticate(const char *username, const char *password) {
    String auth = header(F("Authorization"));
    if (!auth.startsWith(F("Basic "))) { return false; }
    auth.remove(0, 6);
    auth.trim();

    String credentials = username;
    credentials += ':';
    credentials += password;
    char *encoded = (char*)malloc(base64_encode_expected_len(credentials.length()) + 1);
    if (encoded == nullptr) { return false; }
    base64_encode_chars(credentials.c_str(), credentials.length(), encoded);
    bool ok = auth.equalsConstantTime(encoded);
    free(encoded);
    return ok;
  }

  void requestAuthentication(void) {
    sendHeader(F("WWW-Authenticate"), F("Basic realm=\"Login Required\""));
    send(401, "text/html", "");
  }
};
#endif // ESP32

#endif // __TASMOTA_WEBSERVER__
