/*
  TasmotaMqtt.cpp - Wrapper for mqtt for esp8266 by Tuan PM for Tasmota

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

#include "TasmotaMqtt.h"

#include "user_interface.h"
#include "osapi.h"
#include "os_type.h"

/*********************************************************************************************\
 * Prerequisite
 *
 * Copy .c and .h files from https://github.com/tuanpmt/esp_mqtt folder mqtt to folder mqtt
 * - Replace BOOL with bool
 * - Remove variables certificate and private_key from file mqtt.c
 * - Add file user_config.h with default defines for
 *     MQTT_BUF_SIZE 256, MQTT_RECONNECT_TIMEOUT 5, QUEUE_BUFFER_SIZE 2048 and PROTOCOL_NAMEv311
\*********************************************************************************************/

/*********************************************************************************************\
 * Mqtt internal callbacks
\*********************************************************************************************/

static void mqttConnectedCb(uint32_t *args)
{
  MQTT_Client* client = (MQTT_Client*)args;
  TasmotaMqtt* _this = (TasmotaMqtt*)client->user_data;
  if (_this && _this->onMqttConnectedCb) _this->onMqttConnectedCb();
}

static void mqttDisconnectedCb(uint32_t *args)
{
  MQTT_Client* client = (MQTT_Client*)args;
  TasmotaMqtt* _this = (TasmotaMqtt*)client->user_data;
  if (_this && _this->onMqttDisconnectedCb) _this->onMqttDisconnectedCb();
}

static void mqttPublishedCb(uint32_t *args)
{
  MQTT_Client* client = (MQTT_Client*)args;
  TasmotaMqtt* _this = (TasmotaMqtt*)client->user_data;
  if (_this && _this->onMqttPublishedCb) _this->onMqttPublishedCb();
}

static void mqttTimeoutCb(uint32_t *args)
{
  MQTT_Client* client = (MQTT_Client*)args;
  TasmotaMqtt* _this = (TasmotaMqtt*)client->user_data;
  if (_this && _this->onMqttTimeoutCb) _this->onMqttTimeoutCb();
}

static void mqttDataCb(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t data_len)
{
  MQTT_Client* client = (MQTT_Client*)args;
  TasmotaMqtt* _this = (TasmotaMqtt*)client->user_data;
  if (_this) _this->_onMqttDataCb(topic, topic_len, data, data_len);
}

/*********************************************************************************************\
 * TasmotaMqtt class implementation
\*********************************************************************************************/

TasmotaMqtt::TasmotaMqtt() :
  onMqttConnectedCb(0),
  onMqttDisconnectedCb(0),
  onMqttPublishedCb(0),
  onMqttTimeoutCb(0),
  onMqttDataCb(0)
{
}

TasmotaMqtt::~TasmotaMqtt()
{
  MQTT_DeleteClient(&mqttClient);
}

void TasmotaMqtt::InitConnection(const char* host, uint32_t port, uint8_t security)
{
  MQTT_InitConnection(&mqttClient, (uint8_t*)host, port, security);

  // set user data
  mqttClient.user_data = (void*)this;

  MQTT_OnConnected(&mqttClient, mqttConnectedCb);
	MQTT_OnDisconnected(&mqttClient, mqttDisconnectedCb);
  MQTT_OnPublished(&mqttClient, mqttPublishedCb);
  MQTT_OnTimeout(&mqttClient, mqttTimeoutCb);
  MQTT_OnData(&mqttClient, mqttDataCb);
}

void TasmotaMqtt::InitClient(const char* client_id, const char* client_user, const char* client_pass, uint32_t keep_alive_time, uint8_t clean_session)
{
  MQTT_InitClient(&mqttClient, (uint8_t*)client_id, (uint8_t*)client_user, (uint8_t*)client_pass, keep_alive_time, clean_session);
}

void TasmotaMqtt::DeleteClient()
{
  MQTT_DeleteClient(&mqttClient);
}

void TasmotaMqtt::InitLWT(const char* will_topic, const char* will_msg, uint8_t will_qos, bool will_retain)
{
  MQTT_InitLWT(&mqttClient, (uint8_t*)will_topic, (uint8_t*)will_msg, will_qos, (uint8_t)will_retain);
}

void TasmotaMqtt::OnConnected( void (*function)(void) )
{
	onMqttConnectedCb = function;
}

void TasmotaMqtt::OnDisconnected( void (*function)(void) )
{
	onMqttDisconnectedCb = function;
}

void TasmotaMqtt::OnPublished( void (*function)(void) )
{
	onMqttPublishedCb = function;
}

void TasmotaMqtt::OnTimeout( void (*function)(void) )
{
	onMqttTimeoutCb = function;
}

void TasmotaMqtt::OnData( void (*function)(char*, uint8_t*, unsigned int) )
{
  onMqttDataCb = function;
}

bool TasmotaMqtt::Subscribe(const char* topic, uint8_t qos)
{
  return MQTT_Subscribe(&mqttClient, (char*)topic, qos);
}

bool TasmotaMqtt::Unsubscribe(const char* topic)
{
  return MQTT_UnSubscribe(&mqttClient, (char*)topic);
}

void TasmotaMqtt::Connect()
{
  MQTT_Connect(&mqttClient);
}

void TasmotaMqtt::Connect(const char* client_id, const char* client_user, const char* client_pass, const char* will_topic, const char* will_msg, uint8_t will_qos, bool will_retain)
{
  MQTT_InitClient(&mqttClient, (uint8_t*)client_id, (uint8_t*)client_user, (uint8_t*)client_pass, MQTT_KEEPALIVE, 1);
  MQTT_InitLWT(&mqttClient, (uint8_t*)will_topic, (uint8_t*)will_msg, will_qos, (uint8_t)will_retain);
  MQTT_Connect(&mqttClient);
}

void TasmotaMqtt::Disconnect()
{
  MQTT_Disconnect(&mqttClient);
}

bool TasmotaMqtt::Publish(const char* topic, const char* data, int data_length, int qos, bool retain)
{
  return MQTT_Publish(&mqttClient, topic, data, data_length, qos, (int)retain);
}

bool TasmotaMqtt::Connected()
{
  return (mqttClient.connState > TCP_CONNECTED);
}

/*********************************************************************************************/

void TasmotaMqtt::_onMqttDataCb(const char* topic, uint32_t topic_len, const char* data, uint32_t data_len)
{
	char topic_copy[topic_len +1];

	memcpy(topic_copy, topic, topic_len);
	topic_copy[topic_len] = 0;
  if (0 == data_len) data = (const char*)&topic_copy + topic_len;
  onMqttDataCb((char*)topic_copy, (byte*)data, data_len);
}
