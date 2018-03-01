/*
  xdrv_00_mqtt.ino - mqtt support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends

  This program is free software: you can redistribute it and/or modify
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

/*********************************************************************************************\
 * Select only ONE of the defines below - Not supported yet
\*********************************************************************************************/

// Default MQTT driver for both non-TLS and TLS connections. Blocks network if MQTT server is unavailable.
#define USE_MQTT_CLIENT        1              // Use PubSubClient library

// Replaces PubSubClient solving network hangs when MQTT server is unavailable. No TLS support
//#define USE_MQTT_CLIENT        2              // Use (patched) esp-mqtt-arduino library (+4k8 code) - non-TLS only

// Only TLS support. Unstable due to lack of memory
//#define USE_MQTT_CLIENT        3              // Use ESP8266MQTTClient library (+52k code, +15k mem) - TLS only and unstable

/*********************************************************************************************/

enum MqttCommands {
  CMND_MQTTHOST, CMND_MQTTPORT, CMND_MQTTRETRY, CMND_STATETEXT, CMND_MQTTFINGERPRINT, CMND_MQTTCLIENT,
  CMND_MQTTUSER, CMND_MQTTPASSWORD, CMND_FULLTOPIC, CMND_PREFIX, CMND_GROUPTOPIC, CMND_TOPIC,
  CMND_BUTTONTOPIC, CMND_SWITCHTOPIC, CMND_BUTTONRETAIN, CMND_SWITCHRETAIN, CMND_POWERRETAIN, CMND_SENSORRETAIN };
const char kMqttCommands[] PROGMEM =
  D_CMND_MQTTHOST "|" D_CMND_MQTTPORT "|" D_CMND_MQTTRETRY "|" D_CMND_STATETEXT "|" D_CMND_MQTTFINGERPRINT "|" D_CMND_MQTTCLIENT "|"
  D_CMND_MQTTUSER "|" D_CMND_MQTTPASSWORD "|" D_CMND_FULLTOPIC "|" D_CMND_PREFIX "|" D_CMND_GROUPTOPIC "|" D_CMND_TOPIC "|"
  D_CMND_BUTTONTOPIC "|" D_CMND_SWITCHTOPIC "|" D_CMND_BUTTONRETAIN "|" D_CMND_SWITCHRETAIN "|" D_CMND_POWERRETAIN "|" D_CMND_SENSORRETAIN ;

uint8_t mqtt_retry_counter = 0;             // MQTT connection retry counter
uint8_t mqtt_connection_flag = 2;           // MQTT connection messages flag

/*********************************************************************************************\
 * MQTT driver specific code need to provide the following functions:
 *
 * bool MqttIsConnected()
 * void MqttDisconnect()
 * void MqttSubscribeLib(char *topic)
 * bool MqttPublishLib(const char* topic, boolean retained)
 * void MqttCheckConnection()
 * void MqttLoop()
\*********************************************************************************************/

#if (1 == USE_MQTT_CLIENT)  /*****************************************************************/

#include <PubSubClient.h>

// Max message size calculated by PubSubClient is (MQTT_MAX_PACKET_SIZE < 5 + 2 + strlen(topic) + plength)
#if (MQTT_MAX_PACKET_SIZE -TOPSZ -7) < MIN_MESSZ  // If the max message size is too small, throw an error at compile time. See PubSubClient.cpp line 359
  #error "MQTT_MAX_PACKET_SIZE is too small in libraries/PubSubClient/src/PubSubClient.h, increase it to at least 1000"
#endif

PubSubClient MqttClient(EspClient);

bool MqttIsConnected()
{
  return MqttClient.connected();
}

void MqttDisconnect()
{
  MqttClient.disconnect();
}

void MqttSubscribeLib(char *topic)
{
  MqttClient.subscribe(topic);
  MqttClient.loop();  // Solve LmacRxBlk:1 messages
}

bool MqttPublishLib(const char* topic, boolean retained)
{
  return MqttClient.publish(topic, mqtt_data, retained);
}

void MqttCheckConnection()
{
  if (Settings.flag.mqtt_enabled) {
    if (!MqttIsConnected()) {
      if (!mqtt_retry_counter) {
        MqttReconnect();
      } else {
        mqtt_retry_counter--;
      }
    }
  } else {
    if (!mqtt_retry_counter) {
      MqttReconnect();
    }
  }
}

void MqttLoop()
{
  MqttClient.loop();
}

#elif (2 == USE_MQTT_CLIENT)  /***************************************************************/

#include <MQTT.h>
MQTT *MqttClient = NULL;

bool MqttIsConnected()
{
  bool result = false;
  if (MqttClient) result = (MqttClient->isConnected() && !mqtt_retry_counter);
  return result;
}

void MqttDisconnect()
{
  if (MqttClient) MqttClient->disconnect();
}

void MqttMyDataCb(const char* topic, uint32_t topic_len, const char *data, uint32_t data_len)
{
	char topicCpy[topic_len];
	memcpy(topicCpy, topic, topic_len);
	topicCpy[topic_len] = 0;
  MqttDataHandler((char*)topicCpy, (byte*)data, data_len);
}

void MqttMyNoneCb()
{
  // Do nothing
}

void MqttSubscribeLib(char *topic)
{
  MqttClient->subscribe(topic);
}

bool MqttPublishLib(const char* topic, boolean retained)
{
  return MqttClient->publish(topic, mqtt_data, strlen(mqtt_data), 0, retained);
}

void MqttCheckConnection()
{
  if (Settings.flag.mqtt_enabled) {
    if (!MqttIsConnected()) {
      if (!mqtt_retry_counter) {
        MqttReconnect();
      } else {
        mqtt_retry_counter--;
      }
    }
    MqttReconnect();
  } else {
    if (!mqtt_retry_counter) {
      MqttReconnect();
    }
  }
}

void MqttLoop()
{
  // No action
}

#elif (3 == USE_MQTT_CLIENT)  /***************************************************************/

#include <ESP8266MQTTClient.h>
MQTTClient MqttClient;

bool MqttIsConnected()
{
  return true;
}

void MqttDisconnect()
{
  // No action
}

void MqttSubscribeLib(char *topic)
{
  MqttClient.subscribe(topic, 0);
}

bool MqttPublishLib(const char* topic, boolean retained)
{
  return MqttClient.publish(topic, mqtt_data, 0, retained);
}

void MqttCheckConnection()
{
  if (Settings.flag.mqtt_enabled) {
    MqttReconnect();
  } else {
    if (!mqtt_retry_counter) {
      MqttReconnect();
    }
  }
}

void MqttLoop()
{
  MqttClient.handle();
}

#endif  // USE_MQTT_CLIENT

/*********************************************************************************************/

void MqttRetryCounter(uint8_t value)
{
  mqtt_retry_counter = value;
}

void MqttSubscribe(char *topic)
{
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_MQTT D_SUBSCRIBE_TO " %s"), topic);
  AddLog(LOG_LEVEL_DEBUG);
  MqttSubscribeLib(topic);
}

void MqttPublishDirect(const char* topic, boolean retained)
{
  char sretained[CMDSZ];
  char slog_type[10];

  sretained[0] = '\0';
  snprintf_P(slog_type, sizeof(slog_type), PSTR(D_LOG_RESULT));

  if (Settings.flag.mqtt_enabled) {
    if (MqttPublishLib(topic, retained)) {
      snprintf_P(slog_type, sizeof(slog_type), PSTR(D_LOG_MQTT));
      if (retained) {
        snprintf_P(sretained, sizeof(sretained), PSTR(" (" D_RETAINED ")"));
      }
    }
  }

  snprintf_P(log_data, sizeof(log_data), PSTR("%s%s = %s"), slog_type, (Settings.flag.mqtt_enabled) ? topic : strrchr(topic,'/')+1, mqtt_data);
  if (strlen(log_data) >= (sizeof(log_data) - strlen(sretained) -1)) {
    log_data[sizeof(log_data) - strlen(sretained) -5] = '\0';
    snprintf_P(log_data, sizeof(log_data), PSTR("%s ..."), log_data);
  }
  snprintf_P(log_data, sizeof(log_data), PSTR("%s%s"), log_data, sretained);

  AddLog(LOG_LEVEL_INFO);
  if (Settings.ledstate &0x04) {
    blinks++;
  }
}

void MqttPublish(const char* topic, boolean retained)
{
  char *me;

  if (!strcmp(Settings.mqtt_prefix[0],Settings.mqtt_prefix[1])) {
    me = strstr(topic,Settings.mqtt_prefix[0]);
    if (me == topic) {
      mqtt_cmnd_publish += 8;
    }
  }
  MqttPublishDirect(topic, retained);
}

void MqttPublish(const char* topic)
{
  MqttPublish(topic, false);
}

void MqttPublishPrefixTopic_P(uint8_t prefix, const char* subtopic, boolean retained)
{
/* prefix 0 = cmnd using subtopic
 * prefix 1 = stat using subtopic
 * prefix 2 = tele using subtopic
 * prefix 4 = cmnd using subtopic or RESULT
 * prefix 5 = stat using subtopic or RESULT
 * prefix 6 = tele using subtopic or RESULT
 */
  char romram[33];
  char stopic[TOPSZ];

  snprintf_P(romram, sizeof(romram), ((prefix > 3) && !Settings.flag.mqtt_response) ? S_RSLT_RESULT : subtopic);
  for (byte i = 0; i < strlen(romram); i++) {
    romram[i] = toupper(romram[i]);
  }
  prefix &= 3;
  GetTopic_P(stopic, prefix, mqtt_topic, romram);
  MqttPublish(stopic, retained);
}

void MqttPublishPrefixTopic_P(uint8_t prefix, const char* subtopic)
{
  MqttPublishPrefixTopic_P(prefix, subtopic, false);
}

void MqttPublishPowerState(byte device)
{
  char stopic[TOPSZ];
  char scommand[33];

  if ((device < 1) || (device > devices_present)) {
    device = 1;
  }
  GetPowerDevice(scommand, device, sizeof(scommand));
  GetTopic_P(stopic, STAT, mqtt_topic, (Settings.flag.mqtt_response) ? scommand : S_RSLT_RESULT);
  snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, scommand, GetStateText(bitRead(power, device -1)));
  MqttPublish(stopic);

  GetTopic_P(stopic, STAT, mqtt_topic, scommand);
  snprintf_P(mqtt_data, sizeof(mqtt_data), GetStateText(bitRead(power, device -1)));
  MqttPublish(stopic, Settings.flag.mqtt_power_retain);
}

void MqttPublishPowerBlinkState(byte device)
{
  char scommand[33];

  if ((device < 1) || (device > devices_present)) {
    device = 1;
  }
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"%s\":\"" D_JSON_BLINK " %s\"}"),
    GetPowerDevice(scommand, device, sizeof(scommand)), GetStateText(bitRead(blink_mask, device -1)));

  MqttPublishPrefixTopic_P(RESULT_OR_STAT, S_RSLT_POWER);
}

/*********************************************************************************************/

void MqttConnected()
{
  char stopic[TOPSZ];

  if (Settings.flag.mqtt_enabled) {

    // Satisfy iobroker (#299)
    mqtt_data[0] = '\0';
    MqttPublishPrefixTopic_P(CMND, S_RSLT_POWER);

    GetTopic_P(stopic, CMND, mqtt_topic, PSTR("#"));
    MqttSubscribe(stopic);
    if (strstr(Settings.mqtt_fulltopic, MQTT_TOKEN_TOPIC) != NULL) {
      GetTopic_P(stopic, CMND, Settings.mqtt_grptopic, PSTR("#"));
      MqttSubscribe(stopic);
      fallback_topic_flag = 1;
      GetTopic_P(stopic, CMND, mqtt_client, PSTR("#"));
      fallback_topic_flag = 0;
      MqttSubscribe(stopic);
    }

    XdrvCall(FUNC_MQTT_SUBSCRIBE);
  }

  if (mqtt_connection_flag) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_MODULE "\":\"%s\",\"" D_JSON_VERSION "\":\"%s\",\"" D_JSON_FALLBACKTOPIC "\":\"%s\",\"" D_CMND_GROUPTOPIC "\":\"%s\"}"),
      my_module.name, my_version, mqtt_client, Settings.mqtt_grptopic);
    MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_INFO "1"));
#ifdef USE_WEBSERVER
    if (Settings.webserver) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_WEBSERVER_MODE "\":\"%s\",\"" D_CMND_HOSTNAME "\":\"%s\",\"" D_CMND_IPADDRESS "\":\"%s\"}"),
        (2 == Settings.webserver) ? D_ADMIN : D_USER, my_hostname, WiFi.localIP().toString().c_str());
      MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_INFO "2"));
    }
#endif  // USE_WEBSERVER
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_RESTARTREASON "\":\"%s\"}"),
      (GetResetReason() == "Exception") ? ESP.getResetInfo().c_str() : GetResetReason().c_str());
    MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_INFO "3"));
    if (Settings.tele_period) {
      tele_period = Settings.tele_period -9;
    }
    status_update_timer = 2;

    XdrvCall(FUNC_MQTT_INIT);
  }
  mqtt_connection_flag = 0;
}

void MqttReconnect()
{
  char stopic[TOPSZ];

  mqtt_retry_counter = Settings.mqtt_retry;

  if (!Settings.flag.mqtt_enabled) {
    MqttConnected();
    return;
  }
#ifdef USE_EMULATION
  UdpDisconnect();
#endif  // USE_EMULATION
  if (mqtt_connection_flag > 1) {  // Executed once just after power on and wifi is connected - Initial TLS check
#ifdef USE_MQTT_TLS
    AddLog_P(LOG_LEVEL_INFO, S_LOG_MQTT, PSTR(D_FINGERPRINT));
    if (!EspClient.connect(Settings.mqtt_host, Settings.mqtt_port)) {
      snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_MQTT D_TLS_CONNECT_FAILED_TO " %s:%d. " D_RETRY_IN " %d " D_UNIT_SECOND),
        Settings.mqtt_host, Settings.mqtt_port, mqtt_retry_counter);
      AddLog(LOG_LEVEL_DEBUG);
      EspClient.stop();
      return;
    }
    if (EspClient.verify(Settings.mqtt_fingerprint, Settings.mqtt_host)) {
      AddLog_P(LOG_LEVEL_INFO, S_LOG_MQTT, PSTR(D_VERIFIED));
    } else {
      AddLog_P(LOG_LEVEL_INFO, S_LOG_MQTT, PSTR(D_INSECURE));
      EspClient.stop();
      return;
    }
    EspClient.stop();
    yield();
#endif  // USE_MQTT_TLS
    mqtt_connection_flag = 1;
//    mqtt_retry_counter = 1;
//    return;
  }

  AddLog_P(LOG_LEVEL_INFO, S_LOG_MQTT, PSTR(D_ATTEMPTING_CONNECTION));
#ifndef USE_MQTT_TLS
#ifdef USE_DISCOVERY
#ifdef MQTT_HOST_DISCOVERY
  if (!strlen(Settings.mqtt_host)) {
    MdnsDiscoverMqttServer();
  }
#endif  // MQTT_HOST_DISCOVERY
#endif  // USE_DISCOVERY
#endif  // USE_MQTT_TLS

  char *mqtt_user = NULL;
  char *mqtt_pwd = NULL;
  if (strlen(Settings.mqtt_user) > 0) mqtt_user = Settings.mqtt_user;
  if (strlen(Settings.mqtt_pwd) > 0) mqtt_pwd = Settings.mqtt_pwd;

  GetTopic_P(stopic, TELE, mqtt_topic, S_LWT);
  snprintf_P(mqtt_data, sizeof(mqtt_data), S_OFFLINE);

#if (1 == USE_MQTT_CLIENT)
  MqttClient.setCallback(MqttDataHandler);
  MqttClient.setServer(Settings.mqtt_host, Settings.mqtt_port);
  if (MqttClient.connect(mqtt_client, mqtt_user, mqtt_pwd, stopic, 1, true, mqtt_data)) {
    AddLog_P(LOG_LEVEL_INFO, S_LOG_MQTT, PSTR(D_CONNECTED));
    mqtt_retry_counter = 0;
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR(D_ONLINE));
    MqttPublish(stopic, true);
    MqttConnected();
  } else {
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_MQTT D_CONNECT_FAILED_TO " %s:%d, rc %d. " D_RETRY_IN " %d " D_UNIT_SECOND),
      Settings.mqtt_host, Settings.mqtt_port, MqttClient.state(), mqtt_retry_counter);  //status codes are documented here http://pubsubclient.knolleary.net/api.html#state
    AddLog(LOG_LEVEL_INFO);
  }
#elif (2 == USE_MQTT_CLIENT)
  if (!MqttClient) {
    MqttClient = new MQTT(mqtt_client, Settings.mqtt_host, Settings.mqtt_port, stopic, 1, true, mqtt_data);
//    MqttClient->onConnected(MqttMyNoneCb);
//    MqttClient->onDisconnected(MqttMyNoneCb);
//    MqttClient->onPublished(MqttMyNoneCb);
    MqttClient->onData(MqttMyDataCb);
    MqttClient->setUserPwd(mqtt_user, mqtt_pwd);
  }
  MqttClient->connect();
  uint16_t mqtt_timeout = 50;
  while (!MqttClient->isConnected() && mqtt_timeout--) delay(1);
  if (MqttClient->isConnected()) {
    AddLog_P(LOG_LEVEL_INFO, S_LOG_MQTT, PSTR(D_CONNECTED));
    mqtt_retry_counter = 0;
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR(D_ONLINE));
    MqttPublish(stopic, true);
    MqttConnected();
  } else {
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_MQTT D_CONNECT_FAILED_TO " %s:%d. " D_RETRY_IN " %d " D_UNIT_SECOND),
      Settings.mqtt_host, Settings.mqtt_port, mqtt_retry_counter);
    AddLog(LOG_LEVEL_INFO);
  }
#elif (3 == USE_MQTT_CLIENT)
  //topic, data, data is continuing
  MqttClient.onData([](String topic, String data, bool cont) {
    MqttDataHandler((char*)topic.c_str(), (byte*)data.c_str(), strlen(data.c_str()));
  });
  char uri[200];
  snprintf_P(uri, sizeof(uri), PSTR("mqtt://%s:%s@%s:%d#%s"), mqtt_user, mqtt_pwd, Settings.mqtt_host, Settings.mqtt_port, mqtt_client);

  if (MqttClient.begin(String(uri), {.lwtTopic = stopic, .lwtMsg = mqtt_data, .lwtQos = 1, .lwtRetain = 1}, MQTT_KEEPALIVE, true)) {
    AddLog_P(LOG_LEVEL_INFO, S_LOG_MQTT, PSTR(D_CONNECTED));
    mqtt_retry_counter = 0;
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR(D_ONLINE));
    MqttPublish(stopic, true);
    MqttConnected();
  } else {
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_MQTT D_CONNECT_FAILED_TO " %s:%d. " D_RETRY_IN " %d " D_UNIT_SECOND),
      Settings.mqtt_host, Settings.mqtt_port, mqtt_retry_counter);  //status codes are documented here http://pubsubclient.knolleary.net/api.html#state
    AddLog(LOG_LEVEL_INFO);
  }
#endif  // USE_MQTT_CLIENT
}

/*********************************************************************************************/

bool MqttCommand()
{
  char command [CMDSZ];
  bool serviced = true;
  char stemp1[TOPSZ];
  char scommand[CMDSZ];
  uint16_t i;

  uint16_t index = XdrvMailbox.index;
  uint16_t data_len = XdrvMailbox.data_len;
  uint16_t payload16 = XdrvMailbox.payload16;
  int16_t payload = XdrvMailbox.payload;
  uint8_t grpflg =  XdrvMailbox.grpflg;
  char *type = XdrvMailbox.topic;
  char *dataBuf = XdrvMailbox.data;

  int command_code = GetCommandCode(command, sizeof(command), type, kMqttCommands);
  if (CMND_MQTTHOST == command_code) {
    if ((data_len > 0) && (data_len < sizeof(Settings.mqtt_host))) {
      strlcpy(Settings.mqtt_host, (!strcmp(dataBuf,"0")) ? "" : (1 == payload) ? MQTT_HOST : dataBuf, sizeof(Settings.mqtt_host));
      restart_flag = 2;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, Settings.mqtt_host);
  }
  else if (CMND_MQTTPORT == command_code) {
    if (payload16 > 0) {
      Settings.mqtt_port = (1 == payload16) ? MQTT_PORT : payload16;
      restart_flag = 2;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command, Settings.mqtt_port);
  }
  else if (CMND_MQTTRETRY == command_code) {
    if ((payload >= MQTT_RETRY_SECS) && (payload < 32001)) {
      Settings.mqtt_retry = payload;
      mqtt_retry_counter = Settings.mqtt_retry;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command, Settings.mqtt_retry);
  }
  else if ((CMND_STATETEXT == command_code) && (index > 0) && (index <= 4)) {
    if ((data_len > 0) && (data_len < sizeof(Settings.state_text[0]))) {
      for(i = 0; i <= data_len; i++) {
        if (dataBuf[i] == ' ') {
          dataBuf[i] = '_';
        }
      }
      strlcpy(Settings.state_text[index -1], dataBuf, sizeof(Settings.state_text[0]));
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, command, index, GetStateText(index -1));
  }
#ifdef USE_MQTT_TLS
  else if (CMND_MQTTFINGERPRINT == command_code) {
    if ((data_len > 0) && (data_len < sizeof(Settings.mqtt_fingerprint))) {
      strlcpy(Settings.mqtt_fingerprint, (!strcmp(dataBuf,"0")) ? "" : (1 == payload) ? MQTT_FINGERPRINT : dataBuf, sizeof(Settings.mqtt_fingerprint));
      restart_flag = 2;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, Settings.mqtt_fingerprint);
  }
#endif
  else if ((CMND_MQTTCLIENT == command_code) && !grpflg) {
    if ((data_len > 0) && (data_len < sizeof(Settings.mqtt_client))) {
      strlcpy(Settings.mqtt_client, (1 == payload) ? MQTT_CLIENT_ID : dataBuf, sizeof(Settings.mqtt_client));
      restart_flag = 2;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, Settings.mqtt_client);
  }
  else if (CMND_MQTTUSER == command_code) {
    if ((data_len > 0) && (data_len < sizeof(Settings.mqtt_user))) {
      strlcpy(Settings.mqtt_user, (!strcmp(dataBuf,"0")) ? "" : (1 == payload) ? MQTT_USER : dataBuf, sizeof(Settings.mqtt_user));
      restart_flag = 2;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, Settings.mqtt_user);
  }
  else if (CMND_MQTTPASSWORD == command_code) {
    if ((data_len > 0) && (data_len < sizeof(Settings.mqtt_pwd))) {
      strlcpy(Settings.mqtt_pwd, (!strcmp(dataBuf,"0")) ? "" : (1 == payload) ? MQTT_PASS : dataBuf, sizeof(Settings.mqtt_pwd));
      restart_flag = 2;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, Settings.mqtt_pwd);
  }
  else if (CMND_FULLTOPIC == command_code) {
    if ((data_len > 0) && (data_len < sizeof(Settings.mqtt_fulltopic))) {
      MakeValidMqtt(1, dataBuf);
      if (!strcmp(dataBuf, mqtt_client)) {
        payload = 1;
      }
      strlcpy(stemp1, (1 == payload) ? MQTT_FULLTOPIC : dataBuf, sizeof(stemp1));
      if (strcmp(stemp1, Settings.mqtt_fulltopic)) {
        snprintf_P(mqtt_data, sizeof(mqtt_data), (Settings.flag.mqtt_offline) ? S_OFFLINE : "");
        MqttPublishPrefixTopic_P(TELE, PSTR(D_LWT), true);  // Offline or remove previous retained topic
        strlcpy(Settings.mqtt_fulltopic, stemp1, sizeof(Settings.mqtt_fulltopic));
        restart_flag = 2;
      }
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, Settings.mqtt_fulltopic);
  }
  else if ((CMND_PREFIX == command_code) && (index > 0) && (index <= 3)) {
    if ((data_len > 0) && (data_len < sizeof(Settings.mqtt_prefix[0]))) {
      MakeValidMqtt(0, dataBuf);
      strlcpy(Settings.mqtt_prefix[index -1], (1 == payload) ? (1==index)?SUB_PREFIX:(2==index)?PUB_PREFIX:PUB_PREFIX2 : dataBuf, sizeof(Settings.mqtt_prefix[0]));
//      if (Settings.mqtt_prefix[index -1][strlen(Settings.mqtt_prefix[index -1])] == '/') Settings.mqtt_prefix[index -1][strlen(Settings.mqtt_prefix[index -1])] = 0;
      restart_flag = 2;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, command, index, Settings.mqtt_prefix[index -1]);
  }
  else if (CMND_GROUPTOPIC == command_code) {
    if ((data_len > 0) && (data_len < sizeof(Settings.mqtt_grptopic))) {
      MakeValidMqtt(0, dataBuf);
      if (!strcmp(dataBuf, mqtt_client)) {
        payload = 1;
      }
      strlcpy(Settings.mqtt_grptopic, (1 == payload) ? MQTT_GRPTOPIC : dataBuf, sizeof(Settings.mqtt_grptopic));
      restart_flag = 2;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, Settings.mqtt_grptopic);
  }
  else if ((CMND_TOPIC == command_code) && !grpflg) {
    if ((data_len > 0) && (data_len < sizeof(Settings.mqtt_topic))) {
      MakeValidMqtt(0, dataBuf);
      if (!strcmp(dataBuf, mqtt_client)) {
        payload = 1;
      }
      strlcpy(stemp1, (1 == payload) ? MQTT_TOPIC : dataBuf, sizeof(stemp1));
      if (strcmp(stemp1, Settings.mqtt_topic)) {
        snprintf_P(mqtt_data, sizeof(mqtt_data), (Settings.flag.mqtt_offline) ? S_OFFLINE : "");
        MqttPublishPrefixTopic_P(TELE, PSTR(D_LWT), true);  // Offline or remove previous retained topic
        strlcpy(Settings.mqtt_topic, stemp1, sizeof(Settings.mqtt_topic));
        restart_flag = 2;
      }
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, Settings.mqtt_topic);
  }
  else if ((CMND_BUTTONTOPIC == command_code) && !grpflg) {
    if ((data_len > 0) && (data_len < sizeof(Settings.button_topic))) {
      MakeValidMqtt(0, dataBuf);
      if (!strcmp(dataBuf, mqtt_client)) {
        payload = 1;
      }
      strlcpy(Settings.button_topic, (!strcmp(dataBuf,"0")) ? "" : (1 == payload) ? mqtt_topic : dataBuf, sizeof(Settings.button_topic));
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, Settings.button_topic);
  }
  else if (CMND_SWITCHTOPIC == command_code) {
    if ((data_len > 0) && (data_len < sizeof(Settings.switch_topic))) {
      MakeValidMqtt(0, dataBuf);
      if (!strcmp(dataBuf, mqtt_client)) {
        payload = 1;
      }
      strlcpy(Settings.switch_topic, (!strcmp(dataBuf,"0")) ? "" : (1 == payload) ? mqtt_topic : dataBuf, sizeof(Settings.switch_topic));
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, Settings.switch_topic);
  }
  else if (CMND_BUTTONRETAIN == command_code) {
    if ((payload >= 0) && (payload <= 1)) {
      strlcpy(Settings.button_topic, mqtt_topic, sizeof(Settings.button_topic));
      if (!payload) {
        for(i = 1; i <= MAX_KEYS; i++) {
          send_button_power(0, i, 9);  // Clear MQTT retain in broker
        }
      }
      Settings.flag.mqtt_button_retain = payload;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, GetStateText(Settings.flag.mqtt_button_retain));
  }
  else if (CMND_SWITCHRETAIN == command_code) {
    if ((payload >= 0) && (payload <= 1)) {
      strlcpy(Settings.button_topic, mqtt_topic, sizeof(Settings.button_topic));
      if (!payload) {
        for(i = 1; i <= MAX_SWITCHES; i++) {
          send_button_power(1, i, 9);  // Clear MQTT retain in broker
        }
      }
      Settings.flag.mqtt_switch_retain = payload;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, GetStateText(Settings.flag.mqtt_switch_retain));
  }
  else if (CMND_POWERRETAIN == command_code) {
    if ((payload >= 0) && (payload <= 1)) {
      if (!payload) {
        for(i = 1; i <= devices_present; i++) {  // Clear MQTT retain in broker
          GetTopic_P(stemp1, STAT, mqtt_topic, GetPowerDevice(scommand, i, sizeof(scommand)));
          mqtt_data[0] = '\0';
          MqttPublish(stemp1, Settings.flag.mqtt_power_retain);
        }
      }
      Settings.flag.mqtt_power_retain = payload;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, GetStateText(Settings.flag.mqtt_power_retain));
  }
  else if (CMND_SENSORRETAIN == command_code) {
    if ((payload >= 0) && (payload <= 1)) {
      if (!payload) {
        mqtt_data[0] = '\0';
        MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);
        MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_ENERGY), Settings.flag.mqtt_sensor_retain);
      }
      Settings.flag.mqtt_sensor_retain = payload;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, GetStateText(Settings.flag.mqtt_sensor_retain));
  }
  else serviced = false;
  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_00

boolean Xdrv00(byte function)
{
  boolean result = false;

  switch (function) {
    case FUNC_LOOP:
      if (Settings.flag.mqtt_enabled) MqttLoop();
      break;
    case FUNC_EVERY_SECOND:
      if (WL_CONNECTED == WiFi.status()) MqttCheckConnection();
      break;
    case FUNC_COMMAND:
      if (Settings.flag.mqtt_enabled) result = MqttCommand();
      break;
  }
  return result;
}
