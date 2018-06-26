/*//-------------------------------------------------------------------------------
 *  MQTT.h
 *
 *  Header file for MQTT Wrapper
 *
 *  Wrapper for Arduino written by Ingo Randolf during
 *	eTextiles Summercamp 2015.
 *
 *	This library is intended to be used with esp8266 modules.
 *
 *
 *  This class wraps a slightly modified version of mqtt
 *	for esp8266 written by Tuan PM.
 *	Original code: https://github.com/tuanpmt/esp_mqtt
 *
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 //-------------------------------------------------------------------------------*/
#ifndef MQTT_WRAPPER_H
#define MQTT_WRAPPER_H

#include <Arduino.h>

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

extern "C" {
	#include <stddef.h>
	#include "mqtt/mqtt.h"
}


class MQTT
{
public:
	MQTT(const char* client_id, const char* host, uint32_t port);
	~MQTT();
	
	void setClientId(const char* client_id);
	void setUserPwd(const char* user, const char* pwd);
	
	void connect();
	void disconnect();
	bool isConnected();
	
	bool publish(const char* topic, const char* buf, uint32_t buf_len, int qos = 0, int retain = 0);
	bool publish(String& topic, String& data, int qos = 0, int retain = 0);
	bool publish(String& topic, const char* buf, uint32_t buf_len, int qos = 0, int retain = 0);
	bool publish(const char* topic, String& data, int qos = 0, int retain = 0);
	
	bool subscribe(const char* topic, uint8_t qos = 0);
	bool subscribe(const String& topic, uint8_t qos = 0);
	
	int getState() { return mqttClient.connState; };

	// set callbacks
	void onConnected( void (*)(void) );
	void onDisconnected( void (*)(void) );
	void onPublished( void (*)(void) );
	void onData( void (*)(String&, String&) );
	void onData( void (*)(const char*, uint32_t, const char*, uint32_t) );
	
	// user callbacks
	void (*onMqttConnectedCb)(void);
	void (*onMqttDisconnectedCb)(void);
	void (*onMqttPublishedCb)(void);
	void (*onMqttDataCb) (String&, String&);
	void (*onMqttDataRawCb) (const char*, uint32_t, const char*, uint32_t);
	
	// internal callback
	void _onMqttDataCb(const char*, uint32_t, const char*, uint32_t);
	
private:
	MQTT_Client mqttClient;
	
};




#endif