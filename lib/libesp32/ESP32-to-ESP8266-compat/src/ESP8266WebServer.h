//
// Compat with ESP32
//
#pragma once
#include <WebServer.h>

//#define ESP8266WebServer WebServer


class ESP8266WebServer : public WebServer
{
public:
	ESP8266WebServer(int port)
	:WebServer(port)
	{
	}
};

//#define ENC_TYPE_AUTO 0
