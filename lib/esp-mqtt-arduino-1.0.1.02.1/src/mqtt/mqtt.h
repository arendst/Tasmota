/* mqtt.h
*
* Copyright (c) 2014-2015, Tuan PM <tuanpm at live dot com>
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
* * Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
* * Neither the name of Redis nor the names of its contributors may be used
* to endorse or promote products derived from this software without
* specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef USER_AT_MQTT_H_
#define USER_AT_MQTT_H_
#include "mqtt_config.h"
#include "mqtt_msg.h"
#include "user_interface.h"

#include "queue.h"
typedef struct mqtt_event_data_t
{
  uint8_t type;
  const char* topic;
  const char* data;
  uint16_t topic_length;
  uint16_t data_length;
  uint16_t data_offset;
} mqtt_event_data_t;

typedef struct mqtt_state_t
{
  uint16_t port;
  int auto_reconnect;
  mqtt_connect_info_t* connect_info;
  uint8_t* in_buffer;
  uint8_t* out_buffer;
  int in_buffer_length;
  int out_buffer_length;
  uint16_t message_length;
  uint16_t message_length_read;
  mqtt_message_t* outbound_message;
  mqtt_connection_t mqtt_connection;
  uint16_t pending_msg_id;
  int pending_msg_type;
  int pending_publish_qos;
} mqtt_state_t;

typedef enum {
  WIFI_INIT,
  WIFI_CONNECTING,
  WIFI_CONNECTING_ERROR,
  WIFI_CONNECTED,
  DNS_RESOLVE,
  TCP_DISCONNECTING,
  TCP_DISCONNECTED,
  TCP_RECONNECT_DISCONNECTING,
  TCP_RECONNECT_REQ,
  TCP_RECONNECT,
  TCP_CONNECTING,
  TCP_CONNECTING_ERROR,
  TCP_CONNECTED,
  MQTT_CONNECT_SEND,
  MQTT_CONNECT_SENDING,
  MQTT_SUBSCIBE_SEND,
  MQTT_SUBSCIBE_SENDING,
  MQTT_DATA,
  MQTT_KEEPALIVE_SEND,
  MQTT_PUBLISH_RECV,
  MQTT_PUBLISHING,
  MQTT_DELETING,
  MQTT_DELETED,
} tConnState;

typedef void (*MqttCallback)(uint32_t *args);
typedef void (*MqttDataCallback)(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t lengh);

typedef struct  {
  struct espconn *pCon;
  uint8_t security;
  uint8_t* host;
  uint32_t port;
  ip_addr_t ip;
  mqtt_state_t  mqtt_state;
  mqtt_connect_info_t connect_info;
  MqttCallback connectedCb;
  MqttCallback disconnectedCb;
  MqttCallback publishedCb;
  MqttCallback timeoutCb;
  MqttDataCallback dataCb;
  ETSTimer mqttTimer;
  uint32_t keepAliveTick;
  uint32_t reconnectTick;
  uint32_t sendTimeout;
  tConnState connState;
  QUEUE msgQueue;
  void* user_data;
} MQTT_Client;

#define SEC_NONSSL 0
#define SEC_SSL 1

#define MQTT_FLAG_CONNECTED   1
#define MQTT_FLAG_READY     2
#define MQTT_FLAG_EXIT      4

#define MQTT_EVENT_TYPE_NONE      0
#define MQTT_EVENT_TYPE_CONNECTED     1
#define MQTT_EVENT_TYPE_DISCONNECTED  2
#define MQTT_EVENT_TYPE_SUBSCRIBED    3
#define MQTT_EVENT_TYPE_UNSUBSCRIBED  4
#define MQTT_EVENT_TYPE_PUBLISH     5
#define MQTT_EVENT_TYPE_PUBLISHED     6
#define MQTT_EVENT_TYPE_EXITED      7
#define MQTT_EVENT_TYPE_PUBLISH_CONTINUATION 8

void ICACHE_FLASH_ATTR MQTT_InitConnection(MQTT_Client *mqttClient, uint8_t* host, uint32_t port, uint8_t security);
bool ICACHE_FLASH_ATTR MQTT_InitClient(MQTT_Client *mqttClient, uint8_t* client_id, uint8_t* client_user, uint8_t* client_pass, uint32_t keepAliveTime, uint8_t cleanSession);
void ICACHE_FLASH_ATTR MQTT_DeleteClient(MQTT_Client *mqttClient);
void ICACHE_FLASH_ATTR MQTT_InitLWT(MQTT_Client *mqttClient, uint8_t* will_topic, uint8_t* will_msg, uint8_t will_qos, uint8_t will_retain);

void ICACHE_FLASH_ATTR MQTT_SetUserId(MQTT_Client *mqttClient, const char* client_id);
void ICACHE_FLASH_ATTR MQTT_SetUserPwd(MQTT_Client *mqttClient, const char* user_id, const char* pwd);

void ICACHE_FLASH_ATTR MQTT_OnConnected(MQTT_Client *mqttClient, MqttCallback connectedCb);
void ICACHE_FLASH_ATTR MQTT_OnDisconnected(MQTT_Client *mqttClient, MqttCallback disconnectedCb);
void ICACHE_FLASH_ATTR MQTT_OnPublished(MQTT_Client *mqttClient, MqttCallback publishedCb);
void ICACHE_FLASH_ATTR MQTT_OnTimeout(MQTT_Client *mqttClient, MqttCallback timeoutCb);
void ICACHE_FLASH_ATTR MQTT_OnData(MQTT_Client *mqttClient, MqttDataCallback dataCb);
bool ICACHE_FLASH_ATTR MQTT_Subscribe(MQTT_Client *client, char* topic, uint8_t qos);
bool ICACHE_FLASH_ATTR MQTT_UnSubscribe(MQTT_Client *client, char* topic);
void ICACHE_FLASH_ATTR MQTT_Connect(MQTT_Client *mqttClient);
void ICACHE_FLASH_ATTR MQTT_Disconnect(MQTT_Client *mqttClient);
bool ICACHE_FLASH_ATTR MQTT_Publish(MQTT_Client *client, const char* topic, const char* data, int data_length, int qos, int retain);

#endif /* USER_AT_MQTT_H_ */
