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

class TasmotaWebServer : public ESP8266WebServer
{
public:
	TasmotaWebServer(int port) : ESP8266WebServer(port)
	{
	}

  bool isChunked(void) const { return _chunked; }
};
#endif // ESP8266

#ifdef ESP32
#include <WebServer.h>

class TasmotaWebServer : public WebServer
{
public:
	TasmotaWebServer(int port) :WebServer(port)
	{
	}

  bool isChunked(void) const { return _chunked; }
};
#endif // ESP32

#endif // __TASMOTA_WEBSERVER__
