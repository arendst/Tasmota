/*
  TasmotaMqtt.h - Wrapper for mqtt for esp8266 by Tuan PM for Tasmota

  Copyright (C) 2018 Theo Arends and Ingo Randolf

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

#ifndef TasmotaMqtt_h
#define TasmotaMqtt_h
/*********************************************************************************************\
 * TasmotaMqtt supports currently only non-TLS MQTT
 *
 * Adapted from esp-mqtt-arduino by Ingo Randolf (https://github.com/i-n-g-o/esp-mqtt-arduino)
\*********************************************************************************************/

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

// MQTT_KEEPALIVE : keepAlive interval in Seconds
#ifndef MQTT_KEEPALIVE
#define MQTT_KEEPALIVE  15
#endif

class TasmotaMqtt {
public:
  TasmotaMqtt();
  ~TasmotaMqtt();

  void InitConnection(const char* host, uint32_t port, uint8_t security = 0);
  void InitClient(const char* client_id, const char* client_user, const char* client_pass, uint32_t keep_alive_time = MQTT_KEEPALIVE, uint8_t clean_session = 1);
  void DeleteClient();
  void InitLWT(const char* will_topic, const char* will_msg, uint8_t will_qos = 0, bool will_retain = false);

	void OnConnected( void (*)(void) );
	void OnDisconnected( void (*)(void) );
	void OnPublished( void (*)(void) );
	void OnTimeout( void (*)(void) );
  void OnData( void (*)(char*, uint8_t*, unsigned int) );

  bool Subscribe(const char* topic, uint8_t qos = 0);
  bool Unsubscribe(const char* topic);

  void Connect();
  void Connect(const char* client_id, const char* client_user, const char* client_pass, const char* will_topic, const char* will_msg, uint8_t will_qos = 0, bool will_retain = false);
  void Disconnect();

  bool Publish(const char* topic, const char* data, int data_length, int qos = 0, bool retain = false);

  bool Connected();

	int State() { return mqttClient.connState; };

	void (*onMqttConnectedCb)(void);
	void (*onMqttDisconnectedCb)(void);
 	void (*onMqttPublishedCb)(void);
 	void (*onMqttTimeoutCb)(void);
  void (*onMqttDataCb) (char*, uint8_t*, unsigned int);

  // internal callback
  void _onMqttDataCb(const char*, uint32_t, const char*, uint32_t);

private:
  MQTT_Client mqttClient;
};

#endif  // TasmotaMqtt_h