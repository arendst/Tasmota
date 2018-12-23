/*
  xdrv_02_mqtt.ino - mqtt support for Sonoff-Tasmota

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

#define XDRV_02                2

/*********************************************************************************************\
 * Select ONE of possible MQTT library types below
\*********************************************************************************************/
// Default MQTT driver for both non-TLS and TLS connections. Blocks network if MQTT server is unavailable.
//#define MQTT_LIBRARY_TYPE      MQTT_PUBSUBCLIENT   // Use PubSubClient library
// Alternative MQTT driver does not block network when MQTT server is unavailable. No TLS support
//#define MQTT_LIBRARY_TYPE      MQTT_TASMOTAMQTT    // Use TasmotaMqtt library (+4k4 (core 2.3.0), +14k4 (core 2.4.2 lwip2) code, +4k mem) - non-TLS only
// Alternative MQTT driver does not block network when MQTT server is unavailable. TLS should work but needs to be tested.
//#define MQTT_LIBRARY_TYPE      MQTT_ARDUINOMQTT    // Use arduino-mqtt (lwmqtt) library (+3k3 code, +2k mem)

#if (MQTT_LIBRARY_TYPE == MQTT_ESPMQTTARDUINO)     // Obsolete as of v6.2.1.11
#undef MQTT_LIBRARY_TYPE
#define MQTT_LIBRARY_TYPE      MQTT_ARDUINOMQTT
#endif

/*
#if (MQTT_LIBRARY_TYPE == MQTT_TASMOTAMQTT)
#undef MQTT_LIBRARY_TYPE
#define MQTT_LIBRARY_TYPE      MQTT_ARDUINOMQTT    // Obsolete in near future
#endif
*/

#ifdef USE_MQTT_TLS

#if (MQTT_LIBRARY_TYPE == MQTT_TASMOTAMQTT)
#undef MQTT_LIBRARY_TYPE
#endif

#ifndef MQTT_LIBRARY_TYPE
#define MQTT_LIBRARY_TYPE      MQTT_PUBSUBCLIENT   // Use PubSubClient library as it only supports TLS
#endif

#endif  //  USE_MQTT_TLS

/*********************************************************************************************/

#ifdef USE_MQTT_TLS
#ifdef USE_MQTT_TLS_CA_CERT
  #include "sonoff_letsencrypt.h"           // LetsEncrypt certificate
#endif
  WiFiClientSecure EspClient;               // Wifi Secure Client
#else
  WiFiClient EspClient;                     // Wifi Client
#endif

enum MqttCommands {
  CMND_MQTTHOST, CMND_MQTTPORT, CMND_MQTTRETRY, CMND_STATETEXT, CMND_MQTTFINGERPRINT, CMND_MQTTCLIENT,
  CMND_MQTTUSER, CMND_MQTTPASSWORD, CMND_FULLTOPIC, CMND_PREFIX, CMND_GROUPTOPIC, CMND_TOPIC, CMND_PUBLISH,
  CMND_BUTTONTOPIC, CMND_SWITCHTOPIC, CMND_BUTTONRETAIN, CMND_SWITCHRETAIN, CMND_POWERRETAIN, CMND_SENSORRETAIN };
const char kMqttCommands[] PROGMEM =
  D_CMND_MQTTHOST "|" D_CMND_MQTTPORT "|" D_CMND_MQTTRETRY "|" D_CMND_STATETEXT "|" D_CMND_MQTTFINGERPRINT "|" D_CMND_MQTTCLIENT "|"
  D_CMND_MQTTUSER "|" D_CMND_MQTTPASSWORD "|" D_CMND_FULLTOPIC "|" D_CMND_PREFIX "|" D_CMND_GROUPTOPIC "|" D_CMND_TOPIC "|" D_CMND_PUBLISH "|"
  D_CMND_BUTTONTOPIC "|" D_CMND_SWITCHTOPIC "|" D_CMND_BUTTONRETAIN "|" D_CMND_SWITCHRETAIN "|" D_CMND_POWERRETAIN "|" D_CMND_SENSORRETAIN ;

uint16_t mqtt_retry_counter = 1;            // MQTT connection retry counter
uint8_t mqtt_initial_connection_state = 2;  // MQTT connection messages state
bool mqtt_connected = false;                // MQTT virtual connection status

/*********************************************************************************************\
 * MQTT driver specific code need to provide the following functions:
 *
 * bool MqttIsConnected()
 * void MqttDisconnect()
 * void MqttSubscribeLib(char *topic)
 * bool MqttPublishLib(const char* topic, boolean retained)
 * void MqttLoop()
\*********************************************************************************************/

#if (MQTT_LIBRARY_TYPE == MQTT_PUBSUBCLIENT)  /***********************************************/

#include <PubSubClient.h>

// Max message size calculated by PubSubClient is (MQTT_MAX_PACKET_SIZE < 5 + 2 + strlen(topic) + plength)
#if (MQTT_MAX_PACKET_SIZE -TOPSZ -7) < MIN_MESSZ  // If the max message size is too small, throw an error at compile time. See PubSubClient.cpp line 359
  #error "MQTT_MAX_PACKET_SIZE is too small in libraries/PubSubClient/src/PubSubClient.h, increase it to at least 1000"
#endif

PubSubClient MqttClient(EspClient);

bool MqttIsConnected(void)
{
  return MqttClient.connected();
}

void MqttDisconnect(void)
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
  bool result = MqttClient.publish(topic, mqtt_data, retained);
  yield();  // #3313
  return result;
}

void MqttLoop(void)
{
  MqttClient.loop();
}

#elif (MQTT_LIBRARY_TYPE == MQTT_TASMOTAMQTT)  /**********************************************/

#include <TasmotaMqtt.h>
TasmotaMqtt MqttClient;

bool MqttIsConnected(void)
{
  return MqttClient.Connected();
}

void MqttDisconnect(void)
{
  MqttClient.Disconnect();
}

void MqttDisconnectedCb(void)
{
  MqttDisconnected(MqttClient.State());  // status codes are documented in file mqtt.h as tConnState
}

void MqttSubscribeLib(char *topic)
{
  MqttClient.Subscribe(topic, 0);
}

bool MqttPublishLib(const char* topic, boolean retained)
{
  return MqttClient.Publish(topic, mqtt_data, strlen(mqtt_data), 0, retained);
}

void MqttLoop(void)
{
}

#elif (MQTT_LIBRARY_TYPE == MQTT_ARDUINOMQTT)  /**********************************************/

#include <MQTTClient.h>
MQTTClient MqttClient(MQTT_MAX_PACKET_SIZE);

bool MqttIsConnected(void)
{
  return MqttClient.connected();
}

void MqttDisconnect(void)
{
  MqttClient.disconnect();
}

/*
void MqttMyDataCb(MQTTClient* client, char* topic, char* data, int data_len)
//void MqttMyDataCb(MQTTClient *client, char topic[], char data[], int data_len)
{
//  MqttDataHandler((char*)topic, (byte*)data, data_len);
}
*/

void MqttMyDataCb(String &topic, String &data)
{
  MqttDataHandler((char*)topic.c_str(), (byte*)data.c_str(), data.length());
}

void MqttSubscribeLib(char *topic)
{
  MqttClient.subscribe(topic, 0);
}

bool MqttPublishLib(const char* topic, boolean retained)
{
  return MqttClient.publish(topic, mqtt_data, strlen(mqtt_data), retained, 0);
}

void MqttLoop(void)
{
  MqttClient.loop();
//  delay(10);
}

#endif  // MQTT_LIBRARY_TYPE

/*********************************************************************************************/

#ifdef USE_DISCOVERY
#ifdef MQTT_HOST_DISCOVERY
boolean MqttDiscoverServer(void)
{
  if (!mdns_begun) { return false; }

  int n = MDNS.queryService("mqtt", "tcp");  // Search for mqtt service

  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_MDNS D_QUERY_DONE " %d"), n);
  AddLog(LOG_LEVEL_INFO);

  if (n > 0) {
    // Note: current strategy is to get the first MQTT service (even when many are found)
    snprintf_P(Settings.mqtt_host, sizeof(Settings.mqtt_host), MDNS.IP(0).toString().c_str());
    Settings.mqtt_port = MDNS.port(0);

    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_MDNS D_MQTT_SERVICE_FOUND " %s, " D_IP_ADDRESS " %s, " D_PORT " %d"),
      MDNS.hostname(0).c_str(), Settings.mqtt_host, Settings.mqtt_port);
    AddLog(LOG_LEVEL_INFO);
  }

  return n > 0;
}
#endif  // MQTT_HOST_DISCOVERY
#endif  // USE_DISCOVERY

int MqttLibraryType(void)
{
  return (int)MQTT_LIBRARY_TYPE;
}

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

  ShowFreeMem(PSTR("MqttPublishDirect"));

  sretained[0] = '\0';
  snprintf_P(slog_type, sizeof(slog_type), PSTR(D_LOG_RESULT));

  if (Settings.flag.mqtt_enabled) {
    if (MqttIsConnected()) {
      if (MqttPublishLib(topic, retained)) {
        snprintf_P(slog_type, sizeof(slog_type), PSTR(D_LOG_MQTT));
        if (retained) {
          snprintf_P(sretained, sizeof(sretained), PSTR(" (" D_RETAINED ")"));
        }
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
      mqtt_cmnd_publish += 3;
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

  if ((device < 1) || (device > devices_present)) { device = 1; }

  if ((SONOFF_IFAN02 == Settings.module) && (device > 1)) {
    if (GetFanspeed() < MAX_FAN_SPEED) {  // 4 occurs when fanspeed is 3 and RC button 2 is pressed
#ifdef USE_DOMOTICZ
      DomoticzUpdateFanState();  // RC Button feedback
#endif  // USE_DOMOTICZ
      snprintf_P(scommand, sizeof(scommand), PSTR(D_CMND_FANSPEED));
      GetTopic_P(stopic, STAT, mqtt_topic, (Settings.flag.mqtt_response) ? scommand : S_RSLT_RESULT);
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, scommand, GetFanspeed());
      MqttPublish(stopic);
    }
  } else {
    GetPowerDevice(scommand, device, sizeof(scommand), Settings.flag.device_index_enable);
    GetTopic_P(stopic, STAT, mqtt_topic, (Settings.flag.mqtt_response) ? scommand : S_RSLT_RESULT);
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, scommand, GetStateText(bitRead(power, device -1)));
    MqttPublish(stopic);

    GetTopic_P(stopic, STAT, mqtt_topic, scommand);
    snprintf_P(mqtt_data, sizeof(mqtt_data), GetStateText(bitRead(power, device -1)));
    MqttPublish(stopic, Settings.flag.mqtt_power_retain);
  }
}

void MqttPublishPowerBlinkState(byte device)
{
  char scommand[33];

  if ((device < 1) || (device > devices_present)) {
    device = 1;
  }
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"%s\":\"" D_JSON_BLINK " %s\"}"),
    GetPowerDevice(scommand, device, sizeof(scommand), Settings.flag.device_index_enable), GetStateText(bitRead(blink_mask, device -1)));

  MqttPublishPrefixTopic_P(RESULT_OR_STAT, S_RSLT_POWER);
}

/*********************************************************************************************/

void MqttDisconnected(int state)
{
  mqtt_connected = false;
  mqtt_retry_counter = Settings.mqtt_retry;

  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_MQTT D_CONNECT_FAILED_TO " %s:%d, rc %d. " D_RETRY_IN " %d " D_UNIT_SECOND),
    Settings.mqtt_host, Settings.mqtt_port, state, mqtt_retry_counter);
  AddLog(LOG_LEVEL_INFO);
  rules_flag.mqtt_disconnected = 1;
}

void MqttConnected(void)
{
  char stopic[TOPSZ];

  if (Settings.flag.mqtt_enabled) {
    AddLog_P(LOG_LEVEL_INFO, S_LOG_MQTT, PSTR(D_CONNECTED));
    mqtt_connected = true;
    mqtt_retry_counter = 0;

    GetTopic_P(stopic, TELE, mqtt_topic, S_LWT);
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR(D_ONLINE));
    MqttPublish(stopic, true);

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

  if (mqtt_initial_connection_state) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_MODULE "\":\"%s\",\"" D_JSON_VERSION "\":\"%s%s\",\"" D_JSON_FALLBACKTOPIC "\":\"%s\",\"" D_CMND_GROUPTOPIC "\":\"%s\"}"),
      my_module.name, my_version, my_image, mqtt_client, Settings.mqtt_grptopic);
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
    for (byte i = 1; i <= devices_present; i++) {
      MqttPublishPowerState(i);
      if (SONOFF_IFAN02 == Settings.module) { break; }  // Report status of light relay only
    }
    if (Settings.tele_period) { tele_period = Settings.tele_period -9; }  // Enable TelePeriod in 9 seconds
    rules_flag.system_boot = 1;
    XdrvCall(FUNC_MQTT_INIT);
  }
  mqtt_initial_connection_state = 0;

  global_state.mqtt_down = 0;
  if (Settings.flag.mqtt_enabled) {
    rules_flag.mqtt_connected = 1;
  }
}

#ifdef USE_MQTT_TLS
boolean MqttCheckTls(void)
{
  char fingerprint1[60];
  char fingerprint2[60];
  boolean result = false;

  fingerprint1[0] = '\0';
  fingerprint2[0] = '\0';
  for (byte i = 0; i < sizeof(Settings.mqtt_fingerprint[0]); i++) {
    snprintf_P(fingerprint1, sizeof(fingerprint1), PSTR("%s%s%02X"), fingerprint1, (i) ? " " : "", Settings.mqtt_fingerprint[0][i]);
    snprintf_P(fingerprint2, sizeof(fingerprint2), PSTR("%s%s%02X"), fingerprint2, (i) ? " " : "", Settings.mqtt_fingerprint[1][i]);
  }

  AddLog_P(LOG_LEVEL_INFO, S_LOG_MQTT, PSTR(D_FINGERPRINT));

//#ifdef ARDUINO_ESP8266_RELEASE_2_4_1
  EspClient = WiFiClientSecure();               // Wifi Secure Client reconnect issue 4497 (https://github.com/esp8266/Arduino/issues/4497)
//#endif

  if (!EspClient.connect(Settings.mqtt_host, Settings.mqtt_port)) {
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_MQTT D_TLS_CONNECT_FAILED_TO " %s:%d. " D_RETRY_IN " %d " D_UNIT_SECOND),
      Settings.mqtt_host, Settings.mqtt_port, mqtt_retry_counter);
    AddLog(LOG_LEVEL_DEBUG);
  } else {
#ifdef USE_MQTT_TLS_CA_CERT
    unsigned char tls_ca_cert[] = MQTT_TLS_CA_CERT;
    if (EspClient.setCACert(tls_ca_cert, MQTT_TLS_CA_CERT_LENGTH)) {
      if (EspClient.verifyCertChain(Settings.mqtt_host)) {
        AddLog_P(LOG_LEVEL_INFO, S_LOG_MQTT, PSTR(D_VERIFIED "CA"));
        result = true;
      }
    }
#else
    if (EspClient.verify(fingerprint1, Settings.mqtt_host)) {
      AddLog_P(LOG_LEVEL_INFO, S_LOG_MQTT, PSTR(D_VERIFIED "1"));
      result = true;
    }
    else if (EspClient.verify(fingerprint2, Settings.mqtt_host)) {
      AddLog_P(LOG_LEVEL_INFO, S_LOG_MQTT, PSTR(D_VERIFIED "2"));
      result = true;
    }
#endif
  }
  if (!result) AddLog_P(LOG_LEVEL_INFO, S_LOG_MQTT, PSTR(D_FAILED));
  EspClient.stop();
  yield();
  return result;
}
#endif  // USE_MQTT_TLS

void MqttReconnect(void)
{
  char stopic[TOPSZ];

  if (!Settings.flag.mqtt_enabled) {
    MqttConnected();
    return;
  }

#if defined(USE_WEBSERVER) && defined(USE_EMULATION)
  UdpDisconnect();
#endif  // USE_EMULATION

  AddLog_P(LOG_LEVEL_INFO, S_LOG_MQTT, PSTR(D_ATTEMPTING_CONNECTION));

  mqtt_connected = false;
  mqtt_retry_counter = Settings.mqtt_retry;
  global_state.mqtt_down = 1;

#ifndef USE_MQTT_TLS
#ifdef USE_DISCOVERY
#ifdef MQTT_HOST_DISCOVERY
  if (!strlen(Settings.mqtt_host) && !MqttDiscoverServer()) { return; }
#endif  // MQTT_HOST_DISCOVERY
#endif  // USE_DISCOVERY
#endif  // USE_MQTT_TLS

  char *mqtt_user = NULL;
  char *mqtt_pwd = NULL;
  if (strlen(Settings.mqtt_user) > 0) mqtt_user = Settings.mqtt_user;
  if (strlen(Settings.mqtt_pwd) > 0) mqtt_pwd = Settings.mqtt_pwd;

  GetTopic_P(stopic, TELE, mqtt_topic, S_LWT);
  snprintf_P(mqtt_data, sizeof(mqtt_data), S_OFFLINE);

//#ifdef ARDUINO_ESP8266_RELEASE_2_4_1
#ifdef USE_MQTT_TLS
  EspClient = WiFiClientSecure();         // Wifi Secure Client reconnect issue 4497 (https://github.com/esp8266/Arduino/issues/4497)
#else
  EspClient = WiFiClient();               // Wifi Client reconnect issue 4497 (https://github.com/esp8266/Arduino/issues/4497)
#endif
//#endif

  if (2 == mqtt_initial_connection_state) {  // Executed once just after power on and wifi is connected
#ifdef USE_MQTT_TLS
    if (!MqttCheckTls()) return;
#endif  // USE_MQTT_TLS

#if (MQTT_LIBRARY_TYPE == MQTT_TASMOTAMQTT)
    MqttClient.InitConnection(Settings.mqtt_host, Settings.mqtt_port);
    MqttClient.InitClient(mqtt_client, mqtt_user, mqtt_pwd, MQTT_KEEPALIVE);
    MqttClient.InitLWT(stopic, mqtt_data, 1, true);
    MqttClient.OnConnected(MqttConnected);
    MqttClient.OnDisconnected(MqttDisconnectedCb);
    MqttClient.OnData(MqttDataHandler);
#elif (MQTT_LIBRARY_TYPE == MQTT_ARDUINOMQTT)
    MqttClient.begin(Settings.mqtt_host, Settings.mqtt_port, EspClient);
    MqttClient.setWill(stopic, mqtt_data, true, 1);
    MqttClient.setOptions(MQTT_KEEPALIVE, true, MQTT_TIMEOUT);
//    MqttClient.onMessageAdvanced(MqttMyDataCb);
    MqttClient.onMessage(MqttMyDataCb);
#endif

    mqtt_initial_connection_state = 1;
  }

#if (MQTT_LIBRARY_TYPE == MQTT_PUBSUBCLIENT)
  MqttClient.setCallback(MqttDataHandler);
  MqttClient.setServer(Settings.mqtt_host, Settings.mqtt_port);
  if (MqttClient.connect(mqtt_client, mqtt_user, mqtt_pwd, stopic, 1, true, mqtt_data)) {
    MqttConnected();
  } else {
    MqttDisconnected(MqttClient.state());  // status codes are documented here http://pubsubclient.knolleary.net/api.html#state
  }
#elif (MQTT_LIBRARY_TYPE == MQTT_TASMOTAMQTT)
  MqttClient.Connect();
#elif (MQTT_LIBRARY_TYPE == MQTT_ARDUINOMQTT)
  if (MqttClient.connect(mqtt_client, mqtt_user, mqtt_pwd)) {
    MqttConnected();
  } else {
    MqttDisconnected(MqttClient.lastError());  // status codes are documented here https://github.com/256dpi/lwmqtt/blob/master/include/lwmqtt.h#L11
  }
#endif  // MQTT_LIBRARY_TYPE
}

void MqttCheck(void)
{
  if (Settings.flag.mqtt_enabled) {
    if (!MqttIsConnected()) {
      global_state.mqtt_down = 1;
      if (!mqtt_retry_counter) {
#ifndef USE_MQTT_TLS
#ifdef USE_DISCOVERY
#ifdef MQTT_HOST_DISCOVERY
        if (!strlen(Settings.mqtt_host) && !mdns_begun) { return; }
#endif  // MQTT_HOST_DISCOVERY
#endif  // USE_DISCOVERY
#endif  // USE_MQTT_TLS
        MqttReconnect();
      } else {
        mqtt_retry_counter--;
      }
    } else {
      global_state.mqtt_down = 0;
    }
  } else {
    global_state.mqtt_down = 0;
    if (mqtt_initial_connection_state) MqttReconnect();
  }
}

/*********************************************************************************************/

bool MqttCommand(void)
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
  if (-1 == command_code) {
    serviced = false;  // Unknown command
  }
  else if (CMND_MQTTHOST == command_code) {
    if ((data_len > 0) && (data_len < sizeof(Settings.mqtt_host))) {
      strlcpy(Settings.mqtt_host, (SC_CLEAR == Shortcut(dataBuf)) ? "" : (SC_DEFAULT == Shortcut(dataBuf)) ? MQTT_HOST : dataBuf, sizeof(Settings.mqtt_host));
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
        if (dataBuf[i] == ' ') dataBuf[i] = '_';
      }
      strlcpy(Settings.state_text[index -1], dataBuf, sizeof(Settings.state_text[0]));
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, command, index, GetStateText(index -1));
  }
#ifdef USE_MQTT_TLS
  else if ((CMND_MQTTFINGERPRINT == command_code) && (index > 0) && (index <= 2)) {
    char fingerprint[60];
    if ((data_len > 0) && (data_len < sizeof(fingerprint))) {
      strlcpy(fingerprint, (SC_CLEAR == Shortcut(dataBuf)) ? "" : (SC_DEFAULT == Shortcut(dataBuf)) ? (1 == index) ? MQTT_FINGERPRINT1 : MQTT_FINGERPRINT2 : dataBuf, sizeof(fingerprint));
      char *p = fingerprint;
      for (byte i = 0; i < 20; i++) {
        Settings.mqtt_fingerprint[index -1][i] = strtol(p, &p, 16);
      }
      restart_flag = 2;
    }
    fingerprint[0] = '\0';
    for (byte i = 0; i < sizeof(Settings.mqtt_fingerprint[index -1]); i++) {
      snprintf_P(fingerprint, sizeof(fingerprint), PSTR("%s%s%02X"), fingerprint, (i) ? " " : "", Settings.mqtt_fingerprint[index -1][i]);
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, command, index, fingerprint);
  }
#endif
  else if ((CMND_MQTTCLIENT == command_code) && !grpflg) {
    if ((data_len > 0) && (data_len < sizeof(Settings.mqtt_client))) {
      strlcpy(Settings.mqtt_client, (SC_DEFAULT == Shortcut(dataBuf)) ? MQTT_CLIENT_ID : dataBuf, sizeof(Settings.mqtt_client));
      restart_flag = 2;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, Settings.mqtt_client);
  }
  else if (CMND_MQTTUSER == command_code) {
    if ((data_len > 0) && (data_len < sizeof(Settings.mqtt_user))) {
      strlcpy(Settings.mqtt_user, (SC_CLEAR == Shortcut(dataBuf)) ? "" : (SC_DEFAULT == Shortcut(dataBuf)) ? MQTT_USER : dataBuf, sizeof(Settings.mqtt_user));
      restart_flag = 2;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, Settings.mqtt_user);
  }
  else if (CMND_MQTTPASSWORD == command_code) {
    if ((data_len > 0) && (data_len < sizeof(Settings.mqtt_pwd))) {
      strlcpy(Settings.mqtt_pwd, (SC_CLEAR == Shortcut(dataBuf)) ? "" : (SC_DEFAULT == Shortcut(dataBuf)) ? MQTT_PASS : dataBuf, sizeof(Settings.mqtt_pwd));
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, Settings.mqtt_pwd);
      restart_flag = 2;
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_ASTERIX, command);
    }
  }
  else if (CMND_FULLTOPIC == command_code) {
    if ((data_len > 0) && (data_len < sizeof(Settings.mqtt_fulltopic))) {
      MakeValidMqtt(1, dataBuf);
      if (!strcmp(dataBuf, mqtt_client)) SetShortcut(dataBuf, SC_DEFAULT);
      strlcpy(stemp1, (SC_DEFAULT == Shortcut(dataBuf)) ? MQTT_FULLTOPIC : dataBuf, sizeof(stemp1));
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
      strlcpy(Settings.mqtt_prefix[index -1], (SC_DEFAULT == Shortcut(dataBuf)) ? (1==index)?SUB_PREFIX:(2==index)?PUB_PREFIX:PUB_PREFIX2 : dataBuf, sizeof(Settings.mqtt_prefix[0]));
//      if (Settings.mqtt_prefix[index -1][strlen(Settings.mqtt_prefix[index -1])] == '/') Settings.mqtt_prefix[index -1][strlen(Settings.mqtt_prefix[index -1])] = 0;
      restart_flag = 2;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_SVALUE, command, index, Settings.mqtt_prefix[index -1]);
  }
  else if (CMND_PUBLISH == command_code) {
    if (data_len > 0) {
      char *mqtt_part = strtok(dataBuf, " ");
      if (mqtt_part) {
        snprintf(stemp1, sizeof(stemp1), mqtt_part);
        mqtt_part = strtok(NULL, " ");
        if (mqtt_part) {
          snprintf(mqtt_data, sizeof(mqtt_data), mqtt_part);
        } else {
          mqtt_data[0] = '\0';
        }
        MqttPublishDirect(stemp1, (index == 2));
//        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, D_JSON_DONE);
        mqtt_data[0] = '\0';
      }
    }
  }
  else if (CMND_GROUPTOPIC == command_code) {
    if ((data_len > 0) && (data_len < sizeof(Settings.mqtt_grptopic))) {
      MakeValidMqtt(0, dataBuf);
      if (!strcmp(dataBuf, mqtt_client)) SetShortcut(dataBuf, SC_DEFAULT);
      strlcpy(Settings.mqtt_grptopic, (SC_DEFAULT == Shortcut(dataBuf)) ? MQTT_GRPTOPIC : dataBuf, sizeof(Settings.mqtt_grptopic));
      restart_flag = 2;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, Settings.mqtt_grptopic);
  }
  else if ((CMND_TOPIC == command_code) && !grpflg) {
    if ((data_len > 0) && (data_len < sizeof(Settings.mqtt_topic))) {
      MakeValidMqtt(0, dataBuf);
      if (!strcmp(dataBuf, mqtt_client)) SetShortcut(dataBuf, SC_DEFAULT);
      strlcpy(stemp1, (SC_DEFAULT == Shortcut(dataBuf)) ? MQTT_TOPIC : dataBuf, sizeof(stemp1));
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
      if (!strcmp(dataBuf, mqtt_client)) SetShortcut(dataBuf, SC_DEFAULT);
      switch (Shortcut(dataBuf)) {
        case SC_CLEAR: strlcpy(Settings.button_topic, "", sizeof(Settings.button_topic)); break;
        case SC_DEFAULT: strlcpy(Settings.button_topic, mqtt_topic, sizeof(Settings.button_topic)); break;
        case SC_USER: strlcpy(Settings.button_topic, MQTT_BUTTON_TOPIC, sizeof(Settings.button_topic)); break;
        default: strlcpy(Settings.button_topic, dataBuf, sizeof(Settings.button_topic));
      }
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, Settings.button_topic);
  }
  else if (CMND_SWITCHTOPIC == command_code) {
    if ((data_len > 0) && (data_len < sizeof(Settings.switch_topic))) {
      MakeValidMqtt(0, dataBuf);
      if (!strcmp(dataBuf, mqtt_client)) SetShortcut(dataBuf, SC_DEFAULT);
      switch (Shortcut(dataBuf)) {
        case SC_CLEAR: strlcpy(Settings.switch_topic, "", sizeof(Settings.switch_topic)); break;
        case SC_DEFAULT: strlcpy(Settings.switch_topic, mqtt_topic, sizeof(Settings.switch_topic)); break;
        case SC_USER: strlcpy(Settings.switch_topic, MQTT_SWITCH_TOPIC, sizeof(Settings.switch_topic)); break;
        default: strlcpy(Settings.switch_topic, dataBuf, sizeof(Settings.switch_topic));
      }
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, Settings.switch_topic);
  }
  else if (CMND_BUTTONRETAIN == command_code) {
    if ((payload >= 0) && (payload <= 1)) {
      if (!payload) {
        for(i = 1; i <= MAX_KEYS; i++) {
          SendKey(0, i, 9);  // Clear MQTT retain in broker
        }
      }
      Settings.flag.mqtt_button_retain = payload;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, GetStateText(Settings.flag.mqtt_button_retain));
  }
  else if (CMND_SWITCHRETAIN == command_code) {
    if ((payload >= 0) && (payload <= 1)) {
      if (!payload) {
        for(i = 1; i <= MAX_SWITCHES; i++) {
          SendKey(1, i, 9);  // Clear MQTT retain in broker
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
          GetTopic_P(stemp1, STAT, mqtt_topic, GetPowerDevice(scommand, i, sizeof(scommand), Settings.flag.device_index_enable));
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
  else serviced = false;  // Unknown command

  return serviced;
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

#ifdef USE_WEBSERVER

#define WEB_HANDLE_MQTT "mq"

const char S_CONFIGURE_MQTT[] PROGMEM = D_CONFIGURE_MQTT;

const char HTTP_BTN_MENU_MQTT[] PROGMEM =
  "<br/><form action='" WEB_HANDLE_MQTT "' method='get'><button>" D_CONFIGURE_MQTT "</button></form>";

const char HTTP_FORM_MQTT[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_MQTT_PARAMETERS "&nbsp;</b></legend><form method='get' action='" WEB_HANDLE_MQTT "'>"
  "<br/><b>" D_HOST "</b> (" MQTT_HOST ")<br/><input id='mh' name='mh' placeholder='" MQTT_HOST" ' value='{m1'><br/>"
  "<br/><b>" D_PORT "</b> (" STR(MQTT_PORT) ")<br/><input id='ml' name='ml' placeholder='" STR(MQTT_PORT) "' value='{m2'><br/>"
  "<br/><b>" D_CLIENT "</b> ({m0)<br/><input id='mc' name='mc' placeholder='" MQTT_CLIENT_ID "' value='{m3'><br/>"
  "<br/><b>" D_USER "</b> (" MQTT_USER ")<br/><input id='mu' name='mu' placeholder='" MQTT_USER "' value='{m4'><br/>"
  "<br/><b>" D_PASSWORD "</b><br/><input id='mp' name='mp' type='password' placeholder='" MQTT_PASS "' value='{m5'><br/>"
  "<br/><b>" D_TOPIC "</b> = %topic% (" MQTT_TOPIC ")<br/><input id='mt' name='mt' placeholder='" MQTT_TOPIC" ' value='{m6'><br/>"
  "<br/><b>" D_FULL_TOPIC "</b> (" MQTT_FULLTOPIC ")<br/><input id='mf' name='mf' placeholder='" MQTT_FULLTOPIC" ' value='{m7'><br/>";

void HandleMqttConfiguration(void)
{
  if (HttpUser()) { return; }
  if (!WebAuthenticate()) { return WebServer->requestAuthentication(); }
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURE_MQTT);

  if (WebServer->hasArg("save")) {
    MqttSaveSettings();
    WebRestart(1);
    return;
  }

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_CONFIGURE_MQTT));
  page += FPSTR(HTTP_HEAD_STYLE);

  page += FPSTR(HTTP_FORM_MQTT);
  char str[sizeof(Settings.mqtt_client)];
  page.replace(F("{m0"), Format(str, MQTT_CLIENT_ID, sizeof(Settings.mqtt_client)));
  page.replace(F("{m1"), Settings.mqtt_host);
  page.replace(F("{m2"), String(Settings.mqtt_port));
  page.replace(F("{m3"), Settings.mqtt_client);
  page.replace(F("{m4"), (Settings.mqtt_user[0] == '\0')?"0":Settings.mqtt_user);
  page.replace(F("{m5"), (Settings.mqtt_pwd[0] == '\0')?"0":Settings.mqtt_pwd);
  page.replace(F("{m6"), Settings.mqtt_topic);
  page.replace(F("{m7"), Settings.mqtt_fulltopic);

  page += FPSTR(HTTP_FORM_END);
  page += FPSTR(HTTP_BTN_CONF);
  ShowPage(page);
}

void MqttSaveSettings(void)
{
  char tmp[100];
  char stemp[TOPSZ];
  char stemp2[TOPSZ];

  WebGetArg("mt", tmp, sizeof(tmp));
  strlcpy(stemp, (!strlen(tmp)) ? MQTT_TOPIC : tmp, sizeof(stemp));
  MakeValidMqtt(0, stemp);
  WebGetArg("mf", tmp, sizeof(tmp));
  strlcpy(stemp2, (!strlen(tmp)) ? MQTT_FULLTOPIC : tmp, sizeof(stemp2));
  MakeValidMqtt(1,stemp2);
  if ((strcmp(stemp, Settings.mqtt_topic)) || (strcmp(stemp2, Settings.mqtt_fulltopic))) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), (Settings.flag.mqtt_offline) ? S_OFFLINE : "");
    MqttPublishPrefixTopic_P(TELE, S_LWT, true);  // Offline or remove previous retained topic
  }
  strlcpy(Settings.mqtt_topic, stemp, sizeof(Settings.mqtt_topic));
  strlcpy(Settings.mqtt_fulltopic, stemp2, sizeof(Settings.mqtt_fulltopic));
  WebGetArg("mh", tmp, sizeof(tmp));
  strlcpy(Settings.mqtt_host, (!strlen(tmp)) ? MQTT_HOST : (!strcmp(tmp,"0")) ? "" : tmp, sizeof(Settings.mqtt_host));
  WebGetArg("ml", tmp, sizeof(tmp));
  Settings.mqtt_port = (!strlen(tmp)) ? MQTT_PORT : atoi(tmp);
  WebGetArg("mc", tmp, sizeof(tmp));
  strlcpy(Settings.mqtt_client, (!strlen(tmp)) ? MQTT_CLIENT_ID : tmp, sizeof(Settings.mqtt_client));
  WebGetArg("mu", tmp, sizeof(tmp));
  strlcpy(Settings.mqtt_user, (!strlen(tmp)) ? MQTT_USER : (!strcmp(tmp,"0")) ? "" : tmp, sizeof(Settings.mqtt_user));
  WebGetArg("mp", tmp, sizeof(tmp));
  strlcpy(Settings.mqtt_pwd, (!strlen(tmp)) ? MQTT_PASS : (!strcmp(tmp,"0")) ? "" : tmp, sizeof(Settings.mqtt_pwd));
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_MQTT D_CMND_MQTTHOST " %s, " D_CMND_MQTTPORT " %d, " D_CMND_MQTTCLIENT " %s, " D_CMND_MQTTUSER " %s, " D_CMND_MQTTPASSWORD " %s, " D_CMND_TOPIC " %s, " D_CMND_FULLTOPIC " %s"),
    Settings.mqtt_host, Settings.mqtt_port, Settings.mqtt_client, Settings.mqtt_user, Settings.mqtt_pwd, Settings.mqtt_topic, Settings.mqtt_fulltopic);
  AddLog(LOG_LEVEL_INFO);
}
#endif  // USE_WEBSERVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

boolean Xdrv02(byte function)
{
  boolean result = false;

  if (Settings.flag.mqtt_enabled) {
    switch (function) {
#ifdef USE_WEBSERVER
      case FUNC_WEB_ADD_BUTTON:
        strncat_P(mqtt_data, HTTP_BTN_MENU_MQTT, sizeof(mqtt_data) - strlen(mqtt_data) -1);
        break;
      case FUNC_WEB_ADD_HANDLER:
        WebServer->on("/" WEB_HANDLE_MQTT, HandleMqttConfiguration);
        break;
#endif  // USE_WEBSERVER
      case FUNC_LOOP:
        if (!global_state.mqtt_down) { MqttLoop(); }
        break;
      case FUNC_COMMAND:
        result = MqttCommand();
        break;
    }
  }
  return result;
}
