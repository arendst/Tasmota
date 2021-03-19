/*
  xdrv_02_mqtt.ino - mqtt support for Tasmota

  Copyright (C) 2021  Theo Arends

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

#define XDRV_02                    2

#ifndef MQTT_WIFI_CLIENT_TIMEOUT
#define MQTT_WIFI_CLIENT_TIMEOUT   200    // Wifi TCP connection timeout (default is 5000 mSec)
#endif

#define USE_MQTT_NEW_PUBSUBCLIENT

// #define DEBUG_DUMP_TLS    // allow dumping of TLS Flash keys

#ifdef USE_MQTT_TLS
  #include "WiFiClientSecureLightBearSSL.h"
  BearSSL::WiFiClientSecure_light *tlsClient;
#endif
WiFiClient EspClient;                     // Wifi Client - non-TLS

const char kMqttCommands[] PROGMEM = "|"  // No prefix
  // SetOption synonyms
  D_SO_MQTTJSONONLY "|"
#ifdef USE_MQTT_TLS
  D_SO_MQTTTLS "|"
#endif
  D_SO_MQTTNORETAIN "|" D_SO_MQTTDETACHRELAY "|"
  // regular commands
#if defined(USE_MQTT_TLS) && !defined(USE_MQTT_TLS_CA_CERT)
  D_CMND_MQTTFINGERPRINT "|"
#endif
  D_CMND_MQTTUSER "|" D_CMND_MQTTPASSWORD "|" D_CMND_MQTTKEEPALIVE "|" D_CMND_MQTTTIMEOUT "|"
#if defined(USE_MQTT_TLS) && defined(USE_MQTT_AWS_IOT)
  D_CMND_TLSKEY "|"
#endif
  D_CMND_MQTTHOST "|" D_CMND_MQTTPORT "|" D_CMND_MQTTRETRY "|" D_CMND_STATETEXT "|" D_CMND_MQTTCLIENT "|"
  D_CMND_FULLTOPIC "|" D_CMND_PREFIX "|" D_CMND_GROUPTOPIC "|" D_CMND_TOPIC "|" D_CMND_PUBLISH "|" D_CMND_MQTTLOG "|"
  D_CMND_BUTTONTOPIC "|" D_CMND_SWITCHTOPIC "|" D_CMND_BUTTONRETAIN "|" D_CMND_SWITCHRETAIN "|" D_CMND_POWERRETAIN "|"
  D_CMND_SENSORRETAIN "|" D_CMND_INFORETAIN "|" D_CMND_STATERETAIN ;

SO_SYNONYMS(kMqttSynonyms,
  90,
#ifdef USE_MQTT_TLS
  103,
#endif
  104, 114
);

// const uint8_t kMqttSynonyms[] PROGMEM = {
//   4,  // number of synonyms
//   90, 103, 104, 114,
// };

void (* const MqttCommand[])(void) PROGMEM = {
#if defined(USE_MQTT_TLS) && !defined(USE_MQTT_TLS_CA_CERT)
  &CmndMqttFingerprint,
#endif
  &CmndMqttUser, &CmndMqttPassword, &CmndMqttKeepAlive, &CmndMqttTimeout,
#if defined(USE_MQTT_TLS) && defined(USE_MQTT_AWS_IOT)
  &CmndTlsKey,
#endif
  &CmndMqttHost, &CmndMqttPort, &CmndMqttRetry, &CmndStateText, &CmndMqttClient,
  &CmndFullTopic, &CmndPrefix, &CmndGroupTopic, &CmndTopic, &CmndPublish, &CmndMqttlog,
  &CmndButtonTopic, &CmndSwitchTopic, &CmndButtonRetain, &CmndSwitchRetain, &CmndPowerRetain, &CmndSensorRetain,
  &CmndInfoRetain, &CmndStateRetain };

struct MQTT {
  uint16_t connect_count = 0;            // MQTT re-connect count
  uint16_t retry_counter = 1;            // MQTT connection retry counter
  uint16_t retry_counter_delay = 1;      // MQTT retry counter multiplier
  uint8_t initial_connection_state = 2;  // MQTT connection messages state
  bool connected = false;                // MQTT virtual connection status
  bool allowed = false;                  // MQTT enabled and parameters valid
  bool mqtt_tls = false;                 // MQTT TLS is enabled
} Mqtt;

#ifdef USE_MQTT_TLS

// This part of code is necessary to store Private Key and Cert in Flash
#ifdef USE_MQTT_AWS_IOT
#include <base64.hpp>

const br_ec_private_key *AWS_IoT_Private_Key = nullptr;
const br_x509_certificate *AWS_IoT_Client_Certificate = nullptr;

class tls_entry_t {
public:
  uint32_t name;    // simple 4 letters name. Currently 'skey', 'crt ', 'crt1', 'crt2'
  uint16_t start;   // start offset
  uint16_t len;     // len of object
};                  // 8 bytes

const static uint32_t TLS_NAME_SKEY = 0x2079656B; // 'key ' little endian
const static uint32_t TLS_NAME_CRT  = 0x20747263; // 'crt ' little endian

class tls_dir_t {
public:
  tls_entry_t entry[4];     // 4 entries max, only 4 used today, for future use
};                          // 4*8 = 64 bytes

tls_dir_t tls_dir;          // memory copy of tls_dir from flash

#endif  // USE_MQTT_AWS_IOT

// check whether the fingerprint is filled with a single value
// Filled with 0x00 = accept any fingerprint and learn it for next time
// Filled with 0xFF = accept any fingerpring forever
bool is_fingerprint_mono_value(uint8_t finger[20], uint8_t value) {
	for (uint32_t i = 0; i<20; i++) {
		if (finger[i] != value) {
			return false;
		}
	}
	return true;
}
#endif  // USE_MQTT_TLS

void MakeValidMqtt(uint32_t option, char* str) {
// option 0 = replace by underscore
// option 1 = delete character
  uint32_t i = 0;
  while (str[i] > 0) {
//        if ((str[i] == '/') || (str[i] == '+') || (str[i] == '#') || (str[i] == ' ')) {
    if ((str[i] == '+') || (str[i] == '#') || (str[i] == ' ')) {
      if (option) {
        uint32_t j = i;
        while (str[j] > 0) {
          str[j] = str[j +1];
          j++;
        }
        i--;
      } else {
        str[i] = '_';
      }
    }
    i++;
  }
}

/*********************************************************************************************\
 * MQTT driver specific code need to provide the following functions:
 *
 * bool MqttIsConnected()
 * void MqttDisconnect()
 * void MqttSubscribeLib(char *topic)
 * bool MqttPublishLib(const char* topic, bool retained)
\*********************************************************************************************/

#include <PubSubClient.h>

// Max message size calculated by PubSubClient is (MQTT_MAX_PACKET_SIZE < 5 + 2 + strlen(topic) + plength)
#if (MQTT_MAX_PACKET_SIZE -TOPSZ -7) < MIN_MESSZ  // If the max message size is too small, throw an error at compile time. See PubSubClient.cpp line 359
  #error "MQTT_MAX_PACKET_SIZE is too small in libraries/PubSubClient/src/PubSubClient.h, increase it to at least 1200"
#endif

PubSubClient MqttClient;

void MqttInit(void) {
#ifdef USE_MQTT_TLS
  if ((8883 == Settings.mqtt_port) || (8884 == Settings.mqtt_port)) {
    // Turn on TLS for port 8883 (TLS) and 8884 (TLS, client certificate)
    Settings.flag4.mqtt_tls = true;
  }
  Mqtt.mqtt_tls = Settings.flag4.mqtt_tls;   // this flag should not change even if we change the SetOption (until reboot)

  // Detect AWS IoT and set default parameters
  String host = String(SettingsText(SET_MQTT_HOST));
  if (host.indexOf(F(".iot.")) && host.endsWith(F(".amazonaws.com"))) {  // look for ".iot." and ".amazonaws.com" in the domain name
    Settings.flag4.mqtt_no_retain = true;
  }

  if (Mqtt.mqtt_tls) {
#ifdef ESP32
    tlsClient = new BearSSL::WiFiClientSecure_light(2048,2048);
#else // ESP32 - ESP8266
    tlsClient = new BearSSL::WiFiClientSecure_light(1024,1024);
#endif

#ifdef USE_MQTT_AWS_IOT
    loadTlsDir();   // load key and certificate data from Flash
    if ((nullptr != AWS_IoT_Private_Key) && (nullptr != AWS_IoT_Client_Certificate)) {
      tlsClient->setClientECCert(AWS_IoT_Client_Certificate,
                                AWS_IoT_Private_Key,
                                0xFFFF /* all usages, don't care */, 0);
    }
#endif

#ifdef USE_MQTT_TLS_CA_CERT
    tlsClient->setTrustAnchor(Tasmota_TA, nitems(Tasmota_TA));
#endif // USE_MQTT_TLS_CA_CERT

    MqttClient.setClient(*tlsClient);
  } else {
    MqttClient.setClient(EspClient);    // non-TLS
  }
#else // USE_MQTT_TLS
  MqttClient.setClient(EspClient);
#endif // USE_MQTT_TLS

  MqttClient.setKeepAlive(Settings.mqtt_keepalive);
  MqttClient.setSocketTimeout(Settings.mqtt_socket_timeout);
}

bool MqttIsConnected(void) {
  return MqttClient.connected();
}

void MqttDisconnect(void) {
  MqttClient.disconnect();
}

void MqttSubscribeLib(const char *topic) {
  MqttClient.subscribe(topic);
  MqttClient.loop();  // Solve LmacRxBlk:1 messages
}

void MqttUnsubscribeLib(const char *topic) {
  MqttClient.unsubscribe(topic);
  MqttClient.loop();  // Solve LmacRxBlk:1 messages
}

bool MqttPublishLib(const char* topic, bool retained) {
  // If Prefix1 equals Prefix2 disable next MQTT subscription to prevent loop
  if (!strcmp(SettingsText(SET_MQTTPREFIX1), SettingsText(SET_MQTTPREFIX2))) {
    char *str = strstr(topic, SettingsText(SET_MQTTPREFIX1));
    if (str == topic) {
      TasmotaGlobal.mqtt_cmnd_blocked_reset = 4;  // Allow up to four seconds before resetting residual cmnd blocks
      TasmotaGlobal.mqtt_cmnd_blocked++;
    }
  }

  bool result = MqttClient.publish(topic, TasmotaGlobal.mqtt_data, retained);
  yield();  // #3313
  return result;
}

#ifdef DEBUG_TASMOTA_CORE
void MqttDumpData(char* topic, char* data, uint32_t data_len) {
  char dump_data[data_len +1];
  memcpy(dump_data, data, sizeof(dump_data));  // Make another copy for removing optional control characters
  DEBUG_CORE_LOG(PSTR(D_LOG_MQTT "Size %d, \"%s %s\""), data_len, topic, RemoveControlCharacter(dump_data));
}
#endif

void MqttDataHandler(char* mqtt_topic, uint8_t* mqtt_data, unsigned int data_len) {
#ifdef USE_DEBUG_DRIVER
  ShowFreeMem(PSTR("MqttDataHandler"));
#endif

  // Do not allow more data than would be feasable within stack space
  if (data_len >= MQTT_MAX_PACKET_SIZE) { return; }

  // Do not execute multiple times if Prefix1 equals Prefix2
  if (!strcmp(SettingsText(SET_MQTTPREFIX1), SettingsText(SET_MQTTPREFIX2))) {
    char *str = strstr(mqtt_topic, SettingsText(SET_MQTTPREFIX1));
    if ((str == mqtt_topic) && TasmotaGlobal.mqtt_cmnd_blocked) {
      TasmotaGlobal.mqtt_cmnd_blocked--;
      return;
    }
  }

  // Save MQTT data ASAP as it's data is discarded by PubSubClient with next publish as used in MQTTlog
  char topic[TOPSZ];
  strlcpy(topic, mqtt_topic, sizeof(topic));
  mqtt_data[data_len] = 0;
  char data[data_len +1];
  memcpy(data, mqtt_data, sizeof(data));

#ifdef DEBUG_TASMOTA_CORE
  MqttDumpData(topic, data, data_len);  // Use a function to save stack space used by dump_data
#endif

  // MQTT pre-processing
  XdrvMailbox.index = strlen(topic);
  XdrvMailbox.data_len = data_len;
  XdrvMailbox.topic = topic;
  XdrvMailbox.data = (char*)data;
  if (XdrvCall(FUNC_MQTT_DATA)) { return; }

  ShowSource(SRC_MQTT);

  CommandHandler(topic, data, data_len);
}

/*********************************************************************************************/

void MqttRetryCounter(uint8_t value) {
  Mqtt.retry_counter = value;
}

void MqttSubscribe(const char *topic) {
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT D_SUBSCRIBE_TO " %s"), topic);
  MqttSubscribeLib(topic);
}

void MqttUnsubscribe(const char *topic) {
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT D_UNSUBSCRIBE_FROM " %s"), topic);
  MqttUnsubscribeLib(topic);
}

void MqttPublishLoggingAsync(bool refresh) {
  static uint32_t index = 1;

  if (!Settings.mqttlog_level || !Settings.flag.mqtt_enabled || !Mqtt.connected) { return; }  // SetOption3 - Enable MQTT
  if (refresh && !NeedLogRefresh(Settings.mqttlog_level, index)) { return; }

  char* line;
  size_t len;
  while (GetLog(Settings.mqttlog_level, &index, &line, &len)) {
    strlcpy(TasmotaGlobal.mqtt_data, line, len);  // No JSON and ugly!!
    char stopic[TOPSZ];
    GetTopic_P(stopic, STAT, TasmotaGlobal.mqtt_topic, PSTR("LOGGING"));
    MqttPublishLib(stopic, false);
  }
}

void MqttPublish(const char* topic, bool retained) {
#ifdef USE_DEBUG_DRIVER
  ShowFreeMem(PSTR("MqttPublish"));
#endif

  if (Settings.flag4.mqtt_no_retain) {
    retained = false;   // Some brokers don't support retained, they will disconnect if received
  }

  char sretained[CMDSZ];
  sretained[0] = '\0';
  char slog_type[20];
  snprintf_P(slog_type, sizeof(slog_type), PSTR(D_LOG_RESULT));

  if (Settings.flag.mqtt_enabled) {  // SetOption3 - Enable MQTT
    if (MqttPublishLib(topic, retained)) {
      snprintf_P(slog_type, sizeof(slog_type), PSTR(D_LOG_MQTT));
      if (retained) {
        snprintf_P(sretained, sizeof(sretained), PSTR(" (" D_RETAINED ")"));
      }
    }
  }

  char log_data[MAX_LOGSZ];
  snprintf_P(log_data, sizeof(log_data), PSTR("%s%s = %s"), slog_type, (Settings.flag.mqtt_enabled) ? topic : strrchr(topic,'/')+1, TasmotaGlobal.mqtt_data);  // SetOption3 - Enable MQTT
  if (strlen(log_data) >= (sizeof(log_data) - strlen(sretained) -1)) {
    log_data[sizeof(log_data) - strlen(sretained) -5] = '\0';
    snprintf_P(log_data, sizeof(log_data), PSTR("%s ..."), log_data);
  }
  snprintf_P(log_data, sizeof(log_data), PSTR("%s%s"), log_data, sretained);
  AddLogData(LOG_LEVEL_INFO, log_data);

  if (Settings.ledstate &0x04) {
    TasmotaGlobal.blinks++;
  }
}

void MqttPublish(const char* topic) {
  MqttPublish(topic, false);
}

void MqttPublishPrefixTopic_P(uint32_t prefix, const char* subtopic, bool retained) {
/* prefix 0 = cmnd using subtopic
 * prefix 1 = stat using subtopic
 * prefix 2 = tele using subtopic
 * prefix 4 = cmnd using subtopic or RESULT
 * prefix 5 = stat using subtopic or RESULT
 * prefix 6 = tele using subtopic or RESULT
 */
  char romram[64];
  snprintf_P(romram, sizeof(romram), ((prefix > 3) && !Settings.flag.mqtt_response) ? S_RSLT_RESULT : subtopic);  // SetOption4 - Switch between MQTT RESULT or COMMAND
  UpperCase(romram, romram);

  prefix &= 3;
  char stopic[TOPSZ];
  GetTopic_P(stopic, prefix, TasmotaGlobal.mqtt_topic, romram);
  MqttPublish(stopic, retained);

#if defined(USE_MQTT_AWS_IOT) || defined(USE_MQTT_AWS_IOT_LIGHT)
  if ((prefix > 0) && (Settings.flag4.awsiot_shadow) && (Mqtt.connected)) {    // placeholder for SetOptionXX
    // compute the target topic
    char *topic = SettingsText(SET_MQTT_TOPIC);
    char topic2[strlen(topic)+1];       // save buffer onto stack
    strcpy(topic2, topic);
    // replace any '/' with '_'
    char *s = topic2;
    while (*s) {
      if ('/' == *s) {
        *s = '_';
      }
      s++;
    }
    // update topic is "$aws/things/<topic>/shadow/update"
    snprintf_P(romram, sizeof(romram), PSTR("$aws/things/%s/shadow/update"), topic2);

    // copy buffer
    char *mqtt_save = (char*) malloc(strlen(TasmotaGlobal.mqtt_data)+1);
    if (!mqtt_save) { return; }    // abort
    strcpy(mqtt_save, TasmotaGlobal.mqtt_data);
    snprintf_P(TasmotaGlobal.mqtt_data, sizeof(TasmotaGlobal.mqtt_data), PSTR("{\"state\":{\"reported\":%s}}"), mqtt_save);
    free(mqtt_save);

    bool result = MqttClient.publish(romram, TasmotaGlobal.mqtt_data, false);
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT "Updated shadow: %s"), romram);
    yield();  // #3313
  }
#endif // USE_MQTT_AWS_IOT
}

void MqttPublishPrefixTopic_P(uint32_t prefix, const char* subtopic) {
  MqttPublishPrefixTopic_P(prefix, subtopic, false);
}

void MqttPublishPrefixTopicRulesProcess_P(uint32_t prefix, const char* subtopic, bool retained) {
  MqttPublishPrefixTopic_P(prefix, subtopic, retained);
  XdrvRulesProcess();
}

void MqttPublishPrefixTopicRulesProcess_P(uint32_t prefix, const char* subtopic) {
  MqttPublishPrefixTopicRulesProcess_P(prefix, subtopic, false);
}

void MqttPublishTeleSensor(void) {
  MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);  // CMND_SENSORRETAIN
}

void MqttPublishPowerState(uint32_t device) {
  char stopic[TOPSZ];
  char scommand[33];

  if ((device < 1) || (device > TasmotaGlobal.devices_present)) { device = 1; }

#ifdef USE_SONOFF_IFAN
  if (IsModuleIfan() && (device > 1)) {
    if (GetFanspeed() < MaxFanspeed()) {  // 4 occurs when fanspeed is 3 and RC button 2 is pressed
#ifdef USE_DOMOTICZ
      DomoticzUpdateFanState();  // RC Button feedback
#endif  // USE_DOMOTICZ
      snprintf_P(scommand, sizeof(scommand), PSTR(D_CMND_FANSPEED));
      GetTopic_P(stopic, STAT, TasmotaGlobal.mqtt_topic, (Settings.flag.mqtt_response) ? scommand : S_RSLT_RESULT);  // SetOption4 - Switch between MQTT RESULT or COMMAND
      Response_P(S_JSON_COMMAND_NVALUE, scommand, GetFanspeed());
      MqttPublish(stopic);
    }
  } else {
#endif  // USE_SONOFF_IFAN
    GetPowerDevice(scommand, device, sizeof(scommand), Settings.flag.device_index_enable);           // SetOption26 - Switch between POWER or POWER1
    GetTopic_P(stopic, STAT, TasmotaGlobal.mqtt_topic, (Settings.flag.mqtt_response) ? scommand : S_RSLT_RESULT);  // SetOption4 - Switch between MQTT RESULT or COMMAND
    Response_P(S_JSON_COMMAND_SVALUE, scommand, GetStateText(bitRead(TasmotaGlobal.power, device -1)));
    MqttPublish(stopic);

    if (!Settings.flag4.only_json_message) {  // SetOption90 - Disable non-json MQTT response
      GetTopic_P(stopic, STAT, TasmotaGlobal.mqtt_topic, scommand);
      Response_P(GetStateText(bitRead(TasmotaGlobal.power, device -1)));
      MqttPublish(stopic, Settings.flag.mqtt_power_retain);  // CMND_POWERRETAIN
    }
#ifdef USE_SONOFF_IFAN
  }
#endif  // USE_SONOFF_IFAN
}

void MqttPublishAllPowerState(void) {
  for (uint32_t i = 1; i <= TasmotaGlobal.devices_present; i++) {
    MqttPublishPowerState(i);
#ifdef USE_SONOFF_IFAN
    if (IsModuleIfan()) { break; }  // Report status of light relay only
#endif  // USE_SONOFF_IFAN
  }
}

void MqttPublishPowerBlinkState(uint32_t device) {
  char scommand[33];

  if ((device < 1) || (device > TasmotaGlobal.devices_present)) {
    device = 1;
  }
  Response_P(PSTR("{\"%s\":\"" D_JSON_BLINK " %s\"}"),
    GetPowerDevice(scommand, device, sizeof(scommand), Settings.flag.device_index_enable), GetStateText(bitRead(TasmotaGlobal.blink_mask, device -1)));  // SetOption26 - Switch between POWER or POWER1

  MqttPublishPrefixTopic_P(RESULT_OR_STAT, S_RSLT_POWER);
}

/*********************************************************************************************/

uint16_t MqttConnectCount(void) {
  return Mqtt.connect_count;
}

void MqttDisconnected(int state) {
  Mqtt.connected = false;

  Mqtt.retry_counter = Settings.mqtt_retry * Mqtt.retry_counter_delay;
  if ((Settings.mqtt_retry * Mqtt.retry_counter_delay) < 120) {
    Mqtt.retry_counter_delay++;
  }

  MqttClient.disconnect();

  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_CONNECT_FAILED_TO " %s:%d, rc %d. " D_RETRY_IN " %d " D_UNIT_SECOND), SettingsText(SET_MQTT_HOST), Settings.mqtt_port, state, Mqtt.retry_counter);
  TasmotaGlobal.rules_flag.mqtt_disconnected = 1;
}

void MqttConnected(void) {
  char stopic[TOPSZ];

  if (Mqtt.allowed) {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_CONNECTED));
    Mqtt.connected = true;
    Mqtt.retry_counter = 0;
    Mqtt.retry_counter_delay = 1;
    Mqtt.connect_count++;

    GetTopic_P(stopic, TELE, TasmotaGlobal.mqtt_topic, S_LWT);
    Response_P(PSTR(MQTT_LWT_ONLINE));
    MqttPublish(stopic, true);

    if (!Settings.flag4.only_json_message) {  // SetOption90 - Disable non-json MQTT response
      // Satisfy iobroker (#299)
      ResponseClear();
      MqttPublishPrefixTopic_P(CMND, S_RSLT_POWER);
    }

    GetTopic_P(stopic, CMND, TasmotaGlobal.mqtt_topic, PSTR("#"));
    MqttSubscribe(stopic);
    if (strstr_P(SettingsText(SET_MQTT_FULLTOPIC), MQTT_TOKEN_TOPIC) != nullptr) {
      uint32_t real_index = SET_MQTT_GRP_TOPIC;
      for (uint32_t i = 0; i < MAX_GROUP_TOPICS; i++) {
        if (1 == i) { real_index = SET_MQTT_GRP_TOPIC2 -1; }
        if (strlen(SettingsText(real_index +i))) {
          GetGroupTopic_P(stopic, PSTR("#"), real_index +i);  // SetOption75 0: %prefix%/nothing/%topic% = cmnd/nothing/<grouptopic>/# or SetOption75 1: cmnd/<grouptopic>
          MqttSubscribe(stopic);
        }
      }
      GetFallbackTopic_P(stopic, PSTR("#"));
      MqttSubscribe(stopic);
    }

    XdrvCall(FUNC_MQTT_SUBSCRIBE);
  }

  if (Mqtt.initial_connection_state) {
    if (ResetReason() != REASON_DEEP_SLEEP_AWAKE) {
      char stopic2[TOPSZ];
      Response_P(PSTR("{\"" D_CMND_MODULE "\":\"%s\",\"" D_JSON_VERSION "\":\"%s%s\",\"" D_JSON_FALLBACKTOPIC "\":\"%s\",\"" D_CMND_GROUPTOPIC "\":\"%s\"}"),
        ModuleName().c_str(), TasmotaGlobal.version, TasmotaGlobal.image_name, GetFallbackTopic_P(stopic, ""), GetGroupTopic_P(stopic2, "", SET_MQTT_GRP_TOPIC));
      MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_INFO "1"), Settings.flag5.mqtt_info_retain);
#ifdef USE_WEBSERVER
      if (Settings.webserver) {
#if LWIP_IPV6
        Response_P(PSTR("{\"" D_JSON_WEBSERVER_MODE "\":\"%s\",\"" D_CMND_HOSTNAME "\":\"%s\",\"" D_CMND_IPADDRESS "\":\"%s\",\"IPv6Address\":\"%s\"}"),
          (2 == Settings.webserver) ? PSTR(D_ADMIN) : PSTR(D_USER), NetworkHostname(), NetworkAddress().toString().c_str(), WifiGetIPv6().c_str(), Settings.flag5.mqtt_info_retain);
#else
        Response_P(PSTR("{\"" D_JSON_WEBSERVER_MODE "\":\"%s\",\"" D_CMND_HOSTNAME "\":\"%s\",\"" D_CMND_IPADDRESS "\":\"%s\"}"),
          (2 == Settings.webserver) ? PSTR(D_ADMIN) : PSTR(D_USER), NetworkHostname(), NetworkAddress().toString().c_str(), Settings.flag5.mqtt_info_retain);
#endif // LWIP_IPV6 = 1
        MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_INFO "2"), Settings.flag5.mqtt_info_retain);
      }
#endif  // USE_WEBSERVER
      Response_P(PSTR("{\"" D_JSON_RESTARTREASON "\":"));
      if (CrashFlag()) {
        CrashDump();
      } else {
        ResponseAppend_P(PSTR("\"%s\""), GetResetReason().c_str());
      }
      ResponseJsonEnd();
      MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_INFO "3"), Settings.flag5.mqtt_info_retain);
    }

    MqttPublishAllPowerState();
    if (Settings.tele_period) {
      TasmotaGlobal.tele_period = Settings.tele_period -5;  // Enable TelePeriod in 5 seconds
    }
    TasmotaGlobal.rules_flag.system_boot = 1;
    XdrvCall(FUNC_MQTT_INIT);
  }
  Mqtt.initial_connection_state = 0;

  TasmotaGlobal.global_state.mqtt_down = 0;
  if (Settings.flag.mqtt_enabled) {  // SetOption3 - Enable MQTT
    TasmotaGlobal.rules_flag.mqtt_connected = 1;
  }
}

void MqttReconnect(void) {
  char stopic[TOPSZ];

  Mqtt.allowed = Settings.flag.mqtt_enabled;  // SetOption3 - Enable MQTT
  if (Mqtt.allowed) {
#ifdef USE_DISCOVERY
#ifdef MQTT_HOST_DISCOVERY
    MqttDiscoverServer();
#endif  // MQTT_HOST_DISCOVERY
#endif  // USE_DISCOVERY
    if (!strlen(SettingsText(SET_MQTT_HOST)) || !Settings.mqtt_port) {
      Mqtt.allowed = false;
    }
#if defined(USE_MQTT_TLS) && defined(USE_MQTT_AWS_IOT)
    // don't enable MQTT for AWS IoT if Private Key or Certificate are not set
    if (Mqtt.mqtt_tls) {
      if (0 == strlen(SettingsText(SET_MQTT_PWD))) {     // we anticipate that an empty password does not make sense with TLS. This avoids failed connections
        Mqtt.allowed = false;
      }
    }
#endif
  }
  if (!Mqtt.allowed) {
    MqttConnected();
    return;
  }

#ifdef USE_EMULATION
  UdpDisconnect();
#endif  // USE_EMULATION

  Mqtt.connected = false;
  Mqtt.retry_counter = Settings.mqtt_retry * Mqtt.retry_counter_delay;
  TasmotaGlobal.global_state.mqtt_down = 1;

#ifdef FIRMWARE_MINIMAL
#ifndef USE_MQTT_TLS
  // Don't try to connect if MQTT requires TLS but TLS is not supported
  if (Settings.flag4.mqtt_tls) {
    return;
  }
#endif
#endif

  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_ATTEMPTING_CONNECTION));

  char *mqtt_user = nullptr;
  char *mqtt_pwd = nullptr;
  if (strlen(SettingsText(SET_MQTT_USER))) {
    mqtt_user = SettingsText(SET_MQTT_USER);
  }
  if (strlen(SettingsText(SET_MQTT_PWD))) {
    mqtt_pwd = SettingsText(SET_MQTT_PWD);
  }

  GetTopic_P(stopic, TELE, TasmotaGlobal.mqtt_topic, S_LWT);
  Response_P(S_LWT_OFFLINE);

  if (MqttClient.connected()) { MqttClient.disconnect(); }
  EspClient.setTimeout(MQTT_WIFI_CLIENT_TIMEOUT);
#ifdef USE_MQTT_TLS
  if (Mqtt.mqtt_tls) {
    tlsClient->stop();
  } else {
//    EspClient = WiFiClient();               // Wifi Client reconnect issue 4497 (https://github.com/esp8266/Arduino/issues/4497)
    MqttClient.setClient(EspClient);
  }
#else
//  EspClient = WiFiClient();               // Wifi Client reconnect issue 4497 (https://github.com/esp8266/Arduino/issues/4497)
  MqttClient.setClient(EspClient);
#endif

  if (2 == Mqtt.initial_connection_state) {  // Executed once just after power on and wifi is connected
    Mqtt.initial_connection_state = 1;
  }

  MqttClient.setCallback(MqttDataHandler);
#if defined(USE_MQTT_TLS) && defined(USE_MQTT_AWS_IOT)
  // re-assign private keys in case it was updated in between
  if (Mqtt.mqtt_tls) {
    if ((nullptr != AWS_IoT_Private_Key) && (nullptr != AWS_IoT_Client_Certificate)) {
      tlsClient->setClientECCert(AWS_IoT_Client_Certificate,
                                AWS_IoT_Private_Key,
                                0xFFFF /* all usages, don't care */, 0);
    }
  }
#endif
  MqttClient.setServer(SettingsText(SET_MQTT_HOST), Settings.mqtt_port);

  uint32_t mqtt_connect_time = millis();
#if defined(USE_MQTT_TLS) && !defined(USE_MQTT_TLS_CA_CERT)
  bool allow_all_fingerprints;
  bool learn_fingerprint1;
  bool learn_fingerprint2;
  if (Mqtt.mqtt_tls) {
    allow_all_fingerprints = false;
    learn_fingerprint1 = is_fingerprint_mono_value(Settings.mqtt_fingerprint[0], 0x00);
    learn_fingerprint2 = is_fingerprint_mono_value(Settings.mqtt_fingerprint[1], 0x00);
    allow_all_fingerprints |= is_fingerprint_mono_value(Settings.mqtt_fingerprint[0], 0xff);
    allow_all_fingerprints |= is_fingerprint_mono_value(Settings.mqtt_fingerprint[1], 0xff);
    allow_all_fingerprints |= learn_fingerprint1;
    allow_all_fingerprints |= learn_fingerprint2;
    tlsClient->setPubKeyFingerprint(Settings.mqtt_fingerprint[0], Settings.mqtt_fingerprint[1], allow_all_fingerprints);
  }
#endif
  bool lwt_retain = Settings.flag4.mqtt_no_retain ? false : true;   // no retained last will if "no_retain"
#if defined(USE_MQTT_TLS) && defined(USE_MQTT_AWS_IOT)
  if (Mqtt.mqtt_tls) {
    if ((nullptr != AWS_IoT_Private_Key) && (nullptr != AWS_IoT_Client_Certificate)) {
      // if private key is there, we remove user/pwd
      mqtt_user = nullptr;
      mqtt_pwd  = nullptr;
    }
  }
#endif

  if (MqttClient.connect(TasmotaGlobal.mqtt_client, mqtt_user, mqtt_pwd, stopic, 1, lwt_retain, TasmotaGlobal.mqtt_data, MQTT_CLEAN_SESSION)) {
#ifdef USE_MQTT_TLS
    if (Mqtt.mqtt_tls) {
#ifdef ESP8266
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT "TLS connected in %d ms, max ThunkStack used %d"),
        millis() - mqtt_connect_time, tlsClient->getMaxThunkStackUse());
#elif defined(ESP32)
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT "TLS connected in %d ms, stack low mark %d"),
        millis() - mqtt_connect_time, uxTaskGetStackHighWaterMark(nullptr));
#endif
      if (!tlsClient->getMFLNStatus()) {
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT "MFLN not supported by TLS server"));
      }
#ifndef USE_MQTT_TLS_CA_CERT  // don't bother with fingerprints if using CA validation
      const uint8_t *recv_fingerprint = tlsClient->getRecvPubKeyFingerprint();
      // create a printable version of the fingerprint received
      char buf_fingerprint[64];
      ToHex_P(recv_fingerprint, 20, buf_fingerprint, sizeof(buf_fingerprint), ' ');
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT "Server fingerprint: %s"), buf_fingerprint);

      bool learned = false;

      // If the fingerprint slot is marked for update, we'll do so.
      // Otherwise, if the fingerprint slot had the magic trust-on-first-use
      // value, we will save the current fingerprint there, but only if the other fingerprint slot
      // *didn't* match it.
      if (recv_fingerprint[20] & 0x1 || (learn_fingerprint1 && 0 != memcmp(recv_fingerprint, Settings.mqtt_fingerprint[1], 20))) {
        memcpy(Settings.mqtt_fingerprint[0], recv_fingerprint, 20);
        learned = true;
      }
      // As above, but for the other slot.
      if (recv_fingerprint[20] & 0x2 || (learn_fingerprint2 && 0 != memcmp(recv_fingerprint, Settings.mqtt_fingerprint[0], 20))) {
        memcpy(Settings.mqtt_fingerprint[1], recv_fingerprint, 20);
        learned = true;
      }

      if (learned) {
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT "Fingerprint learned: %s"), buf_fingerprint);

        SettingsSaveAll();  // save settings
      }
#endif // !USE_MQTT_TLS_CA_CERT
    }
#endif // USE_MQTT_TLS
    MqttConnected();
  } else {
#ifdef USE_MQTT_TLS
    if (Mqtt.mqtt_tls) {
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT "TLS connection error: %d"), tlsClient->getLastError());
    }
#endif
    MqttDisconnected(MqttClient.state());  // status codes are documented here http://pubsubclient.knolleary.net/api.html#state
  }
}

void MqttCheck(void) {
  if (Settings.flag.mqtt_enabled) {  // SetOption3 - Enable MQTT
    if (!MqttIsConnected()) {
      TasmotaGlobal.global_state.mqtt_down = 1;
      if (!Mqtt.retry_counter) {
        MqttReconnect();
      } else {
        Mqtt.retry_counter--;
      }
    } else {
      TasmotaGlobal.global_state.mqtt_down = 0;
    }
  } else {
    TasmotaGlobal.global_state.mqtt_down = 0;
    if (Mqtt.initial_connection_state) {
      MqttReconnect();
    }
  }
}

bool KeyTopicActive(uint32_t key) {
  // key = 0 - Button topic
  // key = 1 - Switch topic
  key &= 1;
  char key_topic[TOPSZ];
  Format(key_topic, SettingsText(SET_MQTT_BUTTON_TOPIC + key), sizeof(key_topic));
  return ((strlen(key_topic) != 0) && strcmp(key_topic, "0"));
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

#if defined(USE_MQTT_TLS) && !defined(USE_MQTT_TLS_CA_CERT)
void CmndMqttFingerprint(void) {
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 2)) {
    char fingerprint[60];
    if ((XdrvMailbox.data_len > 0) && (XdrvMailbox.data_len < sizeof(fingerprint))) {
      if (SC_DEFAULT == Shortcut()) {
        memcpy_P(Settings.mqtt_fingerprint[XdrvMailbox.index -1], (1 == XdrvMailbox.index) ? default_fingerprint1 : default_fingerprint2, sizeof(default_fingerprint1));
      } else {
        strlcpy(fingerprint, (SC_CLEAR == Shortcut()) ? "" : XdrvMailbox.data, sizeof(fingerprint));
        char *p = fingerprint;
        for (uint32_t i = 0; i < 20; i++) {
          Settings.mqtt_fingerprint[XdrvMailbox.index -1][i] = strtol(p, &p, 16);
        }
      }
      TasmotaGlobal.restart_flag = 2;
    }
    ResponseCmndIdxChar(ToHex_P((unsigned char *)Settings.mqtt_fingerprint[XdrvMailbox.index -1], 20, fingerprint, sizeof(fingerprint), ' '));
  }
}
#endif

void CmndMqttUser(void) {
  if (XdrvMailbox.data_len > 0) {
    SettingsUpdateText(SET_MQTT_USER, (SC_CLEAR == Shortcut()) ? "" : (SC_DEFAULT == Shortcut()) ? PSTR(MQTT_USER) : XdrvMailbox.data);
    TasmotaGlobal.restart_flag = 2;
  }
  ResponseCmndChar(SettingsText(SET_MQTT_USER));
}

void CmndMqttPassword(void) {
  if (XdrvMailbox.data_len > 0) {
    SettingsUpdateText(SET_MQTT_PWD, (SC_CLEAR == Shortcut()) ? "" : (SC_DEFAULT == Shortcut()) ? PSTR(MQTT_PASS) : XdrvMailbox.data);
    ResponseCmndChar(SettingsText(SET_MQTT_PWD));
    TasmotaGlobal.restart_flag = 2;
  } else {
    Response_P(S_JSON_COMMAND_ASTERISK, XdrvMailbox.command);
  }
}

void CmndMqttKeepAlive(void) {
  if ((XdrvMailbox.payload >= 1) && (XdrvMailbox.payload <= 100)) {
    Settings.mqtt_keepalive = XdrvMailbox.payload;
#ifdef USE_MQTT_NEW_PUBSUBCLIENT
    MqttClient.setKeepAlive(Settings.mqtt_keepalive);
#endif
  }
  ResponseCmndNumber(Settings.mqtt_keepalive);
}

void CmndMqttTimeout(void) {
  if ((XdrvMailbox.payload >= 1) && (XdrvMailbox.payload <= 100)) {
    Settings.mqtt_socket_timeout = XdrvMailbox.payload;
#ifdef USE_MQTT_NEW_PUBSUBCLIENT
    MqttClient.setSocketTimeout(Settings.mqtt_socket_timeout);
#endif
  }
  ResponseCmndNumber(Settings.mqtt_socket_timeout);
}

void CmndMqttlog(void) {
  if ((XdrvMailbox.payload >= LOG_LEVEL_NONE) && (XdrvMailbox.payload <= LOG_LEVEL_DEBUG_MORE)) {
    Settings.mqttlog_level = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings.mqttlog_level);
}

void CmndMqttHost(void) {
  if (XdrvMailbox.data_len > 0) {
    SettingsUpdateText(SET_MQTT_HOST, (SC_CLEAR == Shortcut()) ? "" : (SC_DEFAULT == Shortcut()) ? MQTT_HOST : XdrvMailbox.data);
    TasmotaGlobal.restart_flag = 2;
  }
  ResponseCmndChar(SettingsText(SET_MQTT_HOST));
}

void CmndMqttPort(void) {
  if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload < 65536)) {
    Settings.mqtt_port = (1 == XdrvMailbox.payload) ? MQTT_PORT : XdrvMailbox.payload;
    TasmotaGlobal.restart_flag = 2;
  }
  ResponseCmndNumber(Settings.mqtt_port);
}

void CmndMqttRetry(void) {
  if ((XdrvMailbox.payload >= MQTT_RETRY_SECS) && (XdrvMailbox.payload < 32001)) {
    Settings.mqtt_retry = XdrvMailbox.payload;
    Mqtt.retry_counter = Settings.mqtt_retry;
  }
  ResponseCmndNumber(Settings.mqtt_retry);
}

void CmndStateText(void) {
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_STATE_TEXT)) {
    if (!XdrvMailbox.usridx) {
      ResponseCmndAll(SET_STATE_TXT1, MAX_STATE_TEXT);
    } else {
      if (XdrvMailbox.data_len > 0) {
        for (uint32_t i = 0; i <= XdrvMailbox.data_len; i++) {
          if (XdrvMailbox.data[i] == ' ') XdrvMailbox.data[i] = '_';
        }
        SettingsUpdateText(SET_STATE_TXT1 + XdrvMailbox.index -1, XdrvMailbox.data);
      }
      ResponseCmndIdxChar(GetStateText(XdrvMailbox.index -1));
    }
  }
}

void CmndMqttClient(void) {
  if (!XdrvMailbox.grpflg && (XdrvMailbox.data_len > 0)) {
    SettingsUpdateText(SET_MQTT_CLIENT, (SC_DEFAULT == Shortcut()) ? PSTR(MQTT_CLIENT_ID) : XdrvMailbox.data);
    TasmotaGlobal.restart_flag = 2;
  }
  ResponseCmndChar(SettingsText(SET_MQTT_CLIENT));
}

void CmndFullTopic(void) {
  if (XdrvMailbox.data_len > 0) {
    MakeValidMqtt(1, XdrvMailbox.data);
    if (!strcmp(XdrvMailbox.data, TasmotaGlobal.mqtt_client)) { SetShortcutDefault(); }
    char stemp1[TOPSZ];
    strlcpy(stemp1, (SC_DEFAULT == Shortcut()) ? MQTT_FULLTOPIC : XdrvMailbox.data, sizeof(stemp1));
    if (strcmp(stemp1, SettingsText(SET_MQTT_FULLTOPIC))) {
      Response_P((Settings.flag.mqtt_offline) ? S_LWT_OFFLINE : "");  // SetOption10 - Control MQTT LWT message format
      MqttPublishPrefixTopic_P(TELE, S_LWT, true);          // Offline or remove previous retained topic
      SettingsUpdateText(SET_MQTT_FULLTOPIC, stemp1);
      TasmotaGlobal.restart_flag = 2;
    }
  }
  ResponseCmndChar(SettingsText(SET_MQTT_FULLTOPIC));
}

void CmndPrefix(void) {
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_MQTT_PREFIXES)) {
    if (!XdrvMailbox.usridx) {
      ResponseCmndAll(SET_MQTTPREFIX1, MAX_MQTT_PREFIXES);
    } else {
      if (XdrvMailbox.data_len > 0) {
        MakeValidMqtt(0, XdrvMailbox.data);
        SettingsUpdateText(SET_MQTTPREFIX1 + XdrvMailbox.index -1,
          (SC_DEFAULT == Shortcut()) ? (1==XdrvMailbox.index) ? PSTR(SUB_PREFIX) : (2==XdrvMailbox.index) ? PSTR(PUB_PREFIX) : PSTR(PUB_PREFIX2) : XdrvMailbox.data);
        TasmotaGlobal.restart_flag = 2;
      }
      ResponseCmndIdxChar(SettingsText(SET_MQTTPREFIX1 + XdrvMailbox.index -1));
    }
  }
}

void CmndPublish(void) {
  // Allow wildcard character "#" as space replacement in topic (#10258)
  // publish cmnd/theo#arends/power 2 ==> publish cmnd/theo arends/power 2
  if (XdrvMailbox.data_len > 0) {
    char *payload_part;
    char *mqtt_part = strtok_r(XdrvMailbox.data, " ", &payload_part);
    if (mqtt_part) {
      char stemp1[TOPSZ];
      strlcpy(stemp1, mqtt_part, sizeof(stemp1));
      ReplaceChar(stemp1, '#', ' ');
      if ((payload_part != nullptr) && strlen(payload_part)) {
        strlcpy(TasmotaGlobal.mqtt_data, payload_part, sizeof(TasmotaGlobal.mqtt_data));
      } else {
        ResponseClear();
      }
      MqttPublish(stemp1, (XdrvMailbox.index == 2));
//      ResponseCmndDone();
      ResponseClear();
    }
  }
}

void CmndGroupTopic(void) {
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_GROUP_TOPICS)) {
    if (XdrvMailbox.data_len > 0) {
      uint32_t settings_text_index = (1 == XdrvMailbox.index) ? SET_MQTT_GRP_TOPIC : SET_MQTT_GRP_TOPIC2 + XdrvMailbox.index - 2;
      MakeValidMqtt(0, XdrvMailbox.data);
      if (!strcmp(XdrvMailbox.data, TasmotaGlobal.mqtt_client)) { SetShortcutDefault(); }
      SettingsUpdateText(settings_text_index, (SC_CLEAR == Shortcut()) ? "" : (SC_DEFAULT == Shortcut()) ? PSTR(MQTT_GRPTOPIC) : XdrvMailbox.data);

      // Eliminate duplicates, have at least one and fill from index 1
      char stemp[MAX_GROUP_TOPICS][TOPSZ];
      uint32_t read_index = 0;
      uint32_t real_index = SET_MQTT_GRP_TOPIC;
      for (uint32_t i = 0; i < MAX_GROUP_TOPICS; i++) {
        if (1 == i) { real_index = SET_MQTT_GRP_TOPIC2 -1; }
        if (strlen(SettingsText(real_index +i))) {
          bool not_equal = true;
          for (uint32_t j = 0; j < read_index; j++) {
            if (!strcmp(SettingsText(real_index +i), stemp[j])) {  // Topics are case-sensitive
              not_equal = false;
            }
          }
          if (not_equal) {
            strncpy(stemp[read_index], SettingsText(real_index +i), sizeof(stemp[read_index]));
            read_index++;
          }
        }
      }
      if (0 == read_index) {
        SettingsUpdateText(SET_MQTT_GRP_TOPIC, PSTR(MQTT_GRPTOPIC));
      } else {
        uint32_t write_index = 0;
        uint32_t real_index = SET_MQTT_GRP_TOPIC;
        for (uint32_t i = 0; i < MAX_GROUP_TOPICS; i++) {
          if (1 == i) { real_index = SET_MQTT_GRP_TOPIC2 -1; }
          if (write_index < read_index) {
            SettingsUpdateText(real_index +i, stemp[write_index]);
            write_index++;
          } else {
            SettingsUpdateText(real_index +i, "");
          }
        }
      }

      TasmotaGlobal.restart_flag = 2;
    }
    ResponseCmndAll(SET_MQTT_GRP_TOPIC, MAX_GROUP_TOPICS);
  }
}

void CmndTopic(void) {
  if (!XdrvMailbox.grpflg && (XdrvMailbox.data_len > 0)) {
    MakeValidMqtt(0, XdrvMailbox.data);
    if (!strcmp(XdrvMailbox.data, TasmotaGlobal.mqtt_client)) { SetShortcutDefault(); }
    char stemp1[TOPSZ];
    strlcpy(stemp1, (SC_DEFAULT == Shortcut()) ? MQTT_TOPIC : XdrvMailbox.data, sizeof(stemp1));
    if (strcmp(stemp1, SettingsText(SET_MQTT_TOPIC))) {
      Response_P((Settings.flag.mqtt_offline) ? S_LWT_OFFLINE : "");  // SetOption10 - Control MQTT LWT message format
      MqttPublishPrefixTopic_P(TELE, S_LWT, true);          // Offline or remove previous retained topic
      SettingsUpdateText(SET_MQTT_TOPIC, stemp1);
      TasmotaGlobal.restart_flag = 2;
    }
  }
  ResponseCmndChar(SettingsText(SET_MQTT_TOPIC));
}

void CmndButtonTopic(void) {
  if (!XdrvMailbox.grpflg && (XdrvMailbox.data_len > 0)) {
    MakeValidMqtt(0, XdrvMailbox.data);
    if (!strcmp(XdrvMailbox.data, TasmotaGlobal.mqtt_client)) { SetShortcutDefault(); }
    switch (Shortcut()) {
      case SC_CLEAR: SettingsUpdateText(SET_MQTT_BUTTON_TOPIC, ""); break;
      case SC_DEFAULT: SettingsUpdateText(SET_MQTT_BUTTON_TOPIC, TasmotaGlobal.mqtt_topic); break;
      case SC_USER: SettingsUpdateText(SET_MQTT_BUTTON_TOPIC, MQTT_BUTTON_TOPIC); break;
      default: SettingsUpdateText(SET_MQTT_BUTTON_TOPIC, XdrvMailbox.data);
    }
  }
  ResponseCmndChar(SettingsText(SET_MQTT_BUTTON_TOPIC));
}

void CmndSwitchTopic(void) {
  if (!XdrvMailbox.grpflg && (XdrvMailbox.data_len > 0)) {
    MakeValidMqtt(0, XdrvMailbox.data);
    if (!strcmp(XdrvMailbox.data, TasmotaGlobal.mqtt_client)) { SetShortcutDefault(); }
    switch (Shortcut()) {
      case SC_CLEAR: SettingsUpdateText(SET_MQTT_SWITCH_TOPIC, ""); break;
      case SC_DEFAULT: SettingsUpdateText(SET_MQTT_SWITCH_TOPIC, TasmotaGlobal.mqtt_topic); break;
      case SC_USER: SettingsUpdateText(SET_MQTT_SWITCH_TOPIC, PSTR(MQTT_SWITCH_TOPIC)); break;
      default: SettingsUpdateText(SET_MQTT_SWITCH_TOPIC, XdrvMailbox.data);
    }
  }
  ResponseCmndChar(SettingsText(SET_MQTT_SWITCH_TOPIC));
}

void CmndButtonRetain(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    if (!XdrvMailbox.payload) {
      for (uint32_t i = 1; i <= MAX_KEYS; i++) {
        SendKey(KEY_BUTTON, i, CLEAR_RETAIN);  // Clear MQTT retain in broker
      }
    }
    Settings.flag.mqtt_button_retain = XdrvMailbox.payload;  // CMND_BUTTONRETAIN
  }
  ResponseCmndStateText(Settings.flag.mqtt_button_retain);   // CMND_BUTTONRETAIN
}

void CmndSwitchRetain(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    if (!XdrvMailbox.payload) {
      for (uint32_t i = 1; i <= MAX_SWITCHES; i++) {
        SendKey(KEY_SWITCH, i, CLEAR_RETAIN);  // Clear MQTT retain in broker
      }
    }
    Settings.flag.mqtt_switch_retain = XdrvMailbox.payload;  // CMND_SWITCHRETAIN
  }
  ResponseCmndStateText(Settings.flag.mqtt_switch_retain);   // CMND_SWITCHRETAIN
}

void CmndPowerRetain(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    if (!XdrvMailbox.payload) {
      char stemp1[TOPSZ];
      char scommand[CMDSZ];
      for (uint32_t i = 1; i <= TasmotaGlobal.devices_present; i++) {  // Clear MQTT retain in broker
        GetTopic_P(stemp1, STAT, TasmotaGlobal.mqtt_topic, GetPowerDevice(scommand, i, sizeof(scommand), Settings.flag.device_index_enable));  // SetOption26 - Switch between POWER or POWER1
        ResponseClear();
        MqttPublish(stemp1, Settings.flag.mqtt_power_retain);  // CMND_POWERRETAIN
      }
    }
    Settings.flag.mqtt_power_retain = XdrvMailbox.payload;     // CMND_POWERRETAIN
    if (Settings.flag.mqtt_power_retain) {
      Settings.flag4.only_json_message = 0;                    // SetOption90 - Disable non-json MQTT response
    }
  }
  ResponseCmndStateText(Settings.flag.mqtt_power_retain);      // CMND_POWERRETAIN
}

void CmndSensorRetain(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    if (!XdrvMailbox.payload) {
      ResponseClear();
      MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);  // CMND_SENSORRETAIN
      MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_ENERGY), Settings.flag.mqtt_sensor_retain);  // CMND_SENSORRETAIN
    }
    Settings.flag.mqtt_sensor_retain = XdrvMailbox.payload;                                   // CMND_SENSORRETAIN
  }
  ResponseCmndStateText(Settings.flag.mqtt_sensor_retain);                                    // CMND_SENSORRETAIN
}

void CmndInfoRetain(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    if (!XdrvMailbox.payload) {
      ResponseClear();
      MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_INFO), Settings.flag5.mqtt_info_retain);  // CMND_INFORETAIN
    }
    Settings.flag5.mqtt_info_retain = XdrvMailbox.payload;                                   // CMND_INFORETAIN
  }
  ResponseCmndStateText(Settings.flag5.mqtt_info_retain);                                    // CMND_INFORETAIN
}

void CmndStateRetain(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    if (!XdrvMailbox.payload) {
      ResponseClear();
      MqttPublishPrefixTopic_P(STAT, PSTR(D_RSLT_STATE), Settings.flag5.mqtt_state_retain);  // CMND_STATERETAIN
    }
    Settings.flag5.mqtt_state_retain = XdrvMailbox.payload;                                   // CMND_STATERETAIN
  }
  ResponseCmndStateText(Settings.flag5.mqtt_state_retain);                                    // CMND_STATERETAIN
}

/*********************************************************************************************\
 * TLS private key and certificate - store into Flash
\*********************************************************************************************/
#if defined(USE_MQTT_TLS) && defined(USE_MQTT_AWS_IOT)

// const static uint16_t tls_spi_start_sector = EEPROM_LOCATION + 4;  // 0xXXFF
// const static uint8_t* tls_spi_start    = (uint8_t*) ((tls_spi_start_sector * SPI_FLASH_SEC_SIZE) + 0x40200000);  // 0x40XFF000
const static uint16_t tls_spi_start_sector = 0xFF;  // Force last bank of first MB
const static uint8_t* tls_spi_start    = (uint8_t*) 0x402FF000;  // 0x402FF000
const static size_t   tls_spi_len      = 0x1000;  // 4kb blocs
const static size_t   tls_block_offset = 0x0400;
const static size_t   tls_block_len    = 0x0400;   // 1kb
const static size_t   tls_obj_store_offset = tls_block_offset + sizeof(tls_dir_t);


inline void TlsEraseBuffer(uint8_t *buffer) {
  memset(buffer + tls_block_offset, 0xFF, tls_block_len);
}

// static data structures for Private Key and Certificate, only the pointer
// to binary data will change to a region in SPI Flash
static br_ec_private_key EC = {
	23,
	nullptr, 0
};

static br_x509_certificate CHAIN[] = {
	{ nullptr, 0 }
};

// load a copy of the tls_dir from flash into ram
// and calculate the appropriate data structures for AWS_IoT_Private_Key and AWS_IoT_Client_Certificate
void loadTlsDir(void) {
  memcpy_P(&tls_dir, tls_spi_start + tls_block_offset, sizeof(tls_dir));

  // calculate the addresses for Key and Cert in Flash
  if ((TLS_NAME_SKEY == tls_dir.entry[0].name) && (tls_dir.entry[0].len > 0)) {
    EC.x = (unsigned char *)(tls_spi_start + tls_obj_store_offset + tls_dir.entry[0].start);
    EC.xlen = tls_dir.entry[0].len;
    AWS_IoT_Private_Key = &EC;
  } else {
    AWS_IoT_Private_Key = nullptr;
  }
  if ((TLS_NAME_CRT == tls_dir.entry[1].name) && (tls_dir.entry[1].len > 0)) {
    CHAIN[0].data = (unsigned char *) (tls_spi_start + tls_obj_store_offset + tls_dir.entry[1].start);
    CHAIN[0].data_len = tls_dir.entry[1].len;
    AWS_IoT_Client_Certificate = CHAIN;
  } else {
    AWS_IoT_Client_Certificate = nullptr;
  }
//Serial.printf("AWS_IoT_Private_Key = %x, AWS_IoT_Client_Certificate = %x\n", AWS_IoT_Private_Key, AWS_IoT_Client_Certificate);
}

const char ALLOCATE_ERROR[] PROGMEM = "TLSKey " D_JSON_ERROR ": cannot allocate buffer.";

void CmndTlsKey(void) {
#ifdef DEBUG_DUMP_TLS
  if (0 == XdrvMailbox.index){
    CmndTlsDump();
  }
#endif  // DEBUG_DUMP_TLS
  if ((XdrvMailbox.index >= 1) && (XdrvMailbox.index <= 2)) {
    tls_dir_t *tls_dir_write;

    if (XdrvMailbox.data_len > 0) {   // write new value
      // first copy SPI buffer into ram
      uint8_t *spi_buffer = (uint8_t*) malloc(tls_spi_len);
      if (!spi_buffer) {
        AddLog(LOG_LEVEL_ERROR, ALLOCATE_ERROR);
        return;
      }
      memcpy_P(spi_buffer, tls_spi_start, tls_spi_len);

      // remove any white space from the base64
      RemoveSpace(XdrvMailbox.data);

      // allocate buffer for decoded base64
      uint32_t bin_len = decode_base64_length((unsigned char*)XdrvMailbox.data);
      uint8_t  *bin_buf = nullptr;
      if (bin_len > 0) {
        bin_buf = (uint8_t*) malloc(bin_len + 4);
        if (!bin_buf) {
          AddLog(LOG_LEVEL_ERROR, ALLOCATE_ERROR);
          free(spi_buffer);
          return;
        }
      }

      // decode base64
      if (bin_len > 0) {
        decode_base64((unsigned char*)XdrvMailbox.data, bin_buf);
      }

      // address of writable tls_dir in buffer
      tls_dir_write = (tls_dir_t*) (spi_buffer + tls_block_offset);

      if (1 == XdrvMailbox.index) {
        // Try to write Private key
        // Start by erasing all
        TlsEraseBuffer(spi_buffer);   // Erase any previously stored data
        if (bin_len > 0) {
          if (bin_len != 32) {
            // no private key was previously stored, abort
            AddLog(LOG_LEVEL_INFO, PSTR("TLSKey: Certificate must be 32 bytes: %d."), bin_len);
            free(spi_buffer);
            free(bin_buf);
            return;
          }
          tls_entry_t *entry = &tls_dir_write->entry[0];
          entry->name = TLS_NAME_SKEY;
          entry->start = 0;
          entry->len = bin_len;
          memcpy(spi_buffer + tls_obj_store_offset + entry->start, bin_buf, entry->len);
        } else {
          // if lenght is zero, simply erase this SPI flash area
        }
      } else if (2 == XdrvMailbox.index) {
        // Try to write Certificate
        if (TLS_NAME_SKEY != tls_dir.entry[0].name) {
          // no private key was previously stored, abort
          AddLog(LOG_LEVEL_INFO, PSTR("TLSKey: cannot store Cert if no Key previously stored."));
          free(spi_buffer);
          free(bin_buf);
          return;
        }
        if (bin_len <= 256) {
          // Certificate lenght too short
          AddLog(LOG_LEVEL_INFO, PSTR("TLSKey: Certificate length too short: %d."), bin_len);
          free(spi_buffer);
          free(bin_buf);
          return;
        }
        tls_entry_t *entry = &tls_dir_write->entry[1];
        entry->name = TLS_NAME_CRT;
        entry->start = (tls_dir_write->entry[0].start + tls_dir_write->entry[0].len + 3) & ~0x03; // align to 4 bytes boundary
        entry->len = bin_len;
        memcpy(spi_buffer + tls_obj_store_offset + entry->start, bin_buf, entry->len);
      }

      if (ESP.flashEraseSector(tls_spi_start_sector)) {
        ESP.flashWrite(tls_spi_start_sector * SPI_FLASH_SEC_SIZE, (uint32_t*) spi_buffer, SPI_FLASH_SEC_SIZE);
      }
      free(spi_buffer);
      free(bin_buf);
    }

    loadTlsDir();   // reload into memory any potential change
    Response_P(PSTR("{\"%s1\":%d,\"%s2\":%d}"),
      XdrvMailbox.command, AWS_IoT_Private_Key ? tls_dir.entry[0].len : -1,
      XdrvMailbox.command, AWS_IoT_Client_Certificate ? tls_dir.entry[1].len : -1);
  }
}

#ifdef DEBUG_DUMP_TLS
// Dump TLS Flash data - don't activate in production to protect your private keys
uint32_t bswap32(uint32_t x) {
	return	((x << 24) & 0xff000000 ) |
		((x <<  8) & 0x00ff0000 ) |
		((x >>  8) & 0x0000ff00 ) |
		((x >> 24) & 0x000000ff );
}
void CmndTlsDump(void) {
  uint32_t start = (uint32_t)tls_spi_start + tls_block_offset;
  uint32_t end   = start + tls_block_len -1;
  for (uint32_t pos = start; pos < end; pos += 0x10) {
    uint32_t* values = (uint32_t*)(pos);
    Serial.printf_P(PSTR("%08x:  %08x %08x %08x %08x\n"), pos, bswap32(values[0]), bswap32(values[1]), bswap32(values[2]), bswap32(values[3]));
  }
}
#endif  // DEBUG_DUMP_TLS
#endif

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

#ifdef USE_WEBSERVER

#define WEB_HANDLE_MQTT "mq"

const char S_CONFIGURE_MQTT[] PROGMEM = D_CONFIGURE_MQTT;

const char HTTP_BTN_MENU_MQTT[] PROGMEM =
  "<p><form action='" WEB_HANDLE_MQTT "' method='get'><button>" D_CONFIGURE_MQTT "</button></form></p>";

const char HTTP_FORM_MQTT1[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_MQTT_PARAMETERS "&nbsp;</b></legend>"
  "<form method='get' action='" WEB_HANDLE_MQTT "'>"
  "<p><b>" D_HOST "</b> (" MQTT_HOST ")<br><input id='mh' placeholder=\"" MQTT_HOST "\" value=\"%s\"></p>"
  "<p><b>" D_PORT "</b> (" STR(MQTT_PORT) ")<br><input id='ml' placeholder='" STR(MQTT_PORT) "' value='%d'></p>"
#ifdef USE_MQTT_TLS
  "<p><label><input id='b3' type='checkbox'%s><b>" D_MQTT_TLS_ENABLE "</b></label><br>"
#endif // USE_MQTT_TLS
  "<p><b>" D_CLIENT "</b> (%s)<br><input id='mc' placeholder=\"%s\" value=\"%s\"></p>";
const char HTTP_FORM_MQTT2[] PROGMEM =
  "<p><b>" D_USER "</b> (" MQTT_USER ")<br><input id='mu' placeholder=\"" MQTT_USER "\" value=\"%s\"></p>"
  "<p><label><b>" D_PASSWORD "</b><input type='checkbox' onclick='sp(\"mp\")'></label><br><input id='mp' type='password' placeholder=\"" D_PASSWORD "\" value=\"" D_ASTERISK_PWD "\"></p>"
  "<p><b>" D_TOPIC "</b> = %%topic%% (%s)<br><input id='mt' placeholder=\"%s\" value=\"%s\"></p>"
  "<p><b>" D_FULL_TOPIC "</b> (%s)<br><input id='mf' placeholder=\"%s\" value=\"%s\"></p>";

void HandleMqttConfiguration(void)
{
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_CONFIGURE_MQTT));

  if (Webserver->hasArg(F("save"))) {
    MqttSaveSettings();
    WebRestart(1);
    return;
  }

  char str[TOPSZ];

  WSContentStart_P(PSTR(D_CONFIGURE_MQTT));
  WSContentSendStyle();
  WSContentSend_P(HTTP_FORM_MQTT1,
    SettingsText(SET_MQTT_HOST),
    Settings.mqtt_port,
#ifdef USE_MQTT_TLS
    Mqtt.mqtt_tls ? PSTR(" checked") : "",      // SetOption102 - Enable MQTT TLS
#endif // USE_MQTT_TLS
    Format(str, PSTR(MQTT_CLIENT_ID), sizeof(str)), PSTR(MQTT_CLIENT_ID), SettingsText(SET_MQTT_CLIENT));
  WSContentSend_P(HTTP_FORM_MQTT2,
    (!strlen(SettingsText(SET_MQTT_USER))) ? "0" : SettingsText(SET_MQTT_USER),
    Format(str, PSTR(MQTT_TOPIC), sizeof(str)), PSTR(MQTT_TOPIC), SettingsText(SET_MQTT_TOPIC),
    PSTR(MQTT_FULLTOPIC), PSTR(MQTT_FULLTOPIC), SettingsText(SET_MQTT_FULLTOPIC));
  WSContentSend_P(HTTP_FORM_END);
  WSContentSpaceButton(BUTTON_CONFIGURATION);
  WSContentStop();
}

void MqttSaveSettings(void)
{
  char tmp[TOPSZ];
  char stemp[TOPSZ];
  char stemp2[TOPSZ];

  WebGetArg(PSTR("mt"), tmp, sizeof(tmp));
  strlcpy(stemp, (!strlen(tmp)) ? MQTT_TOPIC : tmp, sizeof(stemp));
  MakeValidMqtt(0, stemp);
  WebGetArg(PSTR("mf"), tmp, sizeof(tmp));
  strlcpy(stemp2, (!strlen(tmp)) ? MQTT_FULLTOPIC : tmp, sizeof(stemp2));
  MakeValidMqtt(1, stemp2);
  if ((strcmp(stemp, SettingsText(SET_MQTT_TOPIC))) || (strcmp(stemp2, SettingsText(SET_MQTT_FULLTOPIC)))) {
    Response_P((Settings.flag.mqtt_offline) ? S_LWT_OFFLINE : "");  // SetOption10 - Control MQTT LWT message format
    MqttPublishPrefixTopic_P(TELE, S_LWT, true);                // Offline or remove previous retained topic
  }
  SettingsUpdateText(SET_MQTT_TOPIC, stemp);
  SettingsUpdateText(SET_MQTT_FULLTOPIC, stemp2);
  WebGetArg(PSTR("mh"), tmp, sizeof(tmp));
  SettingsUpdateText(SET_MQTT_HOST, (!strlen(tmp)) ? PSTR(MQTT_HOST) : (!strcmp(tmp,"0")) ? "" : tmp);
  WebGetArg(PSTR("ml"), tmp, sizeof(tmp));
  Settings.mqtt_port = (!strlen(tmp)) ? MQTT_PORT : atoi(tmp);
#ifdef USE_MQTT_TLS
  Settings.flag4.mqtt_tls = Webserver->hasArg(F("b3"));  // SetOption102 - Enable MQTT TLS
#endif
  WebGetArg(PSTR("mc"), tmp, sizeof(tmp));
  SettingsUpdateText(SET_MQTT_CLIENT, (!strlen(tmp)) ? PSTR(MQTT_CLIENT_ID) : tmp);
  WebGetArg(PSTR("mu"), tmp, sizeof(tmp));
  SettingsUpdateText(SET_MQTT_USER, (!strlen(tmp)) ? PSTR(MQTT_USER) : (!strcmp(tmp,"0")) ? "" : tmp);
  WebGetArg(PSTR("mp"), tmp, sizeof(tmp));
  SettingsUpdateText(SET_MQTT_PWD, (!strlen(tmp)) ? "" : (!strcmp(tmp, D_ASTERISK_PWD)) ? SettingsText(SET_MQTT_PWD) : tmp);
  AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_CMND_MQTTHOST " %s, " D_CMND_MQTTPORT " %d, " D_CMND_MQTTCLIENT " %s, " D_CMND_MQTTUSER " %s, " D_CMND_TOPIC " %s, " D_CMND_FULLTOPIC " %s"),
    SettingsText(SET_MQTT_HOST), Settings.mqtt_port, SettingsText(SET_MQTT_CLIENT), SettingsText(SET_MQTT_USER), SettingsText(SET_MQTT_TOPIC), SettingsText(SET_MQTT_FULLTOPIC));
}
#endif  // USE_WEBSERVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv02(uint8_t function)
{
  bool result = false;

  if (Settings.flag.mqtt_enabled) {  // SetOption3 - Enable MQTT
    switch (function) {
      case FUNC_PRE_INIT:
        MqttInit();
        break;
      case FUNC_EVERY_50_MSECOND:  // https://github.com/knolleary/pubsubclient/issues/556
        MqttClient.loop();
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_ADD_BUTTON:
        WSContentSend_P(HTTP_BTN_MENU_MQTT);
        break;
      case FUNC_WEB_ADD_HANDLER:
        WebServer_on(PSTR("/" WEB_HANDLE_MQTT), HandleMqttConfiguration);
        break;
#endif  // USE_WEBSERVER
      case FUNC_COMMAND:
        result = DecodeCommand(kMqttCommands, MqttCommand, kMqttSynonyms);
        break;
    }
  }
  return result;
}
