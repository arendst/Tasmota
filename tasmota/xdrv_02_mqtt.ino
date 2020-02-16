/*
  xdrv_02_mqtt.ino - mqtt support for Tasmota

  Copyright (C) 2020  Theo Arends

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

// #define DEBUG_DUMP_TLS    // allow dumping of TLS Flash keys

#ifdef USE_MQTT_TLS
  #include "WiFiClientSecureLightBearSSL.h"
  BearSSL::WiFiClientSecure_light *tlsClient;
#else
  WiFiClient EspClient;                     // Wifi Client
#endif

const char kMqttCommands[] PROGMEM = "|"  // No prefix
#if defined(USE_MQTT_TLS) && !defined(USE_MQTT_TLS_CA_CERT)
  D_CMND_MQTTFINGERPRINT "|"
#endif
  D_CMND_MQTTUSER "|" D_CMND_MQTTPASSWORD "|"
#if defined(USE_MQTT_TLS) && defined(USE_MQTT_AWS_IOT)
  D_CMND_TLSKEY "|"
#endif
  D_CMND_MQTTHOST "|" D_CMND_MQTTPORT "|" D_CMND_MQTTRETRY "|" D_CMND_STATETEXT "|" D_CMND_MQTTCLIENT "|"
  D_CMND_FULLTOPIC "|" D_CMND_PREFIX "|" D_CMND_GROUPTOPIC "|" D_CMND_TOPIC "|" D_CMND_PUBLISH "|" D_CMND_MQTTLOG "|"
  D_CMND_BUTTONTOPIC "|" D_CMND_SWITCHTOPIC "|" D_CMND_BUTTONRETAIN "|" D_CMND_SWITCHRETAIN "|" D_CMND_POWERRETAIN "|" D_CMND_SENSORRETAIN ;

void (* const MqttCommand[])(void) PROGMEM = {
#if defined(USE_MQTT_TLS) && !defined(USE_MQTT_TLS_CA_CERT)
  &CmndMqttFingerprint,
#endif
  &CmndMqttUser, &CmndMqttPassword,
#if defined(USE_MQTT_TLS) && defined(USE_MQTT_AWS_IOT)
  &CmndTlsKey,
#endif
  &CmndMqttHost, &CmndMqttPort, &CmndMqttRetry, &CmndStateText, &CmndMqttClient,
  &CmndFullTopic, &CmndPrefix, &CmndGroupTopic, &CmndTopic, &CmndPublish, &CmndMqttlog,
  &CmndButtonTopic, &CmndSwitchTopic, &CmndButtonRetain, &CmndSwitchRetain, &CmndPowerRetain, &CmndSensorRetain };

struct MQTT {
  uint16_t connect_count = 0;            // MQTT re-connect count
  uint16_t retry_counter = 1;            // MQTT connection retry counter
  uint8_t initial_connection_state = 2;  // MQTT connection messages state
  bool connected = false;                // MQTT virtual connection status
  bool allowed = false;                  // MQTT enabled and parameters valid
} Mqtt;

#ifdef USE_MQTT_TLS

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

void MakeValidMqtt(uint32_t option, char* str)
{
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

#ifdef USE_DISCOVERY
#ifdef MQTT_HOST_DISCOVERY
void MqttDiscoverServer(void)
{
  if (!Wifi.mdns_begun) { return; }

  int n = MDNS.queryService("mqtt", "tcp");  // Search for mqtt service

  AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_MDNS D_QUERY_DONE " %d"), n);

  if (n > 0) {
    uint32_t i = 0;            // If the hostname isn't set, use the first record found.
#ifdef MDNS_HOSTNAME
    for (i = n; i > 0; i--) {  // Search from last to first and use first if not found
      if (!strcmp(MDNS.hostname(i).c_str(), MDNS_HOSTNAME)) {
        break;                 // Stop at matching record
      }
    }
#endif  // MDNS_HOSTNAME
    SettingsUpdateText(SET_MQTT_HOST, MDNS.IP(i).toString().c_str());
    Settings.mqtt_port = MDNS.port(i);

    AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_MDNS D_MQTT_SERVICE_FOUND " %s, " D_IP_ADDRESS " %s, " D_PORT " %d"), MDNS.hostname(i).c_str(), SettingsText(SET_MQTT_HOST), Settings.mqtt_port);
  }
}
#endif  // MQTT_HOST_DISCOVERY
#endif  // USE_DISCOVERY

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

#ifdef USE_MQTT_TLS
PubSubClient MqttClient;
#else
PubSubClient MqttClient(EspClient);
#endif

void MqttInit(void)
{
#ifdef USE_MQTT_TLS
  tlsClient = new BearSSL::WiFiClientSecure_light(1024,1024);

#ifdef USE_MQTT_AWS_IOT
  loadTlsDir();   // load key and certificate data from Flash
  tlsClient->setClientECCert(AWS_IoT_Client_Certificate,
                             AWS_IoT_Private_Key,
                             0xFFFF /* all usages, don't care */, 0);
#endif

#ifdef USE_MQTT_TLS_CA_CERT
#ifdef USE_MQTT_AWS_IOT
  tlsClient->setTrustAnchor(&AmazonRootCA1_TA);
#else
  tlsClient->setTrustAnchor(&LetsEncryptX3CrossSigned_TA);
#endif // USE_MQTT_AWS_IOT
#endif // USE_MQTT_TLS_CA_CERT

  MqttClient.setClient(*tlsClient);
#endif // USE_MQTT_TLS
}

bool MqttIsConnected(void)
{
  return MqttClient.connected();
}

void MqttDisconnect(void)
{
  MqttClient.disconnect();
}

void MqttSubscribeLib(const char *topic)
{
  MqttClient.subscribe(topic);
  MqttClient.loop();  // Solve LmacRxBlk:1 messages
}

void MqttUnsubscribeLib(const char *topic)
{
  MqttClient.unsubscribe(topic);
  MqttClient.loop();  // Solve LmacRxBlk:1 messages
}

bool MqttPublishLib(const char* topic, bool retained)
{
  // If Prefix1 equals Prefix2 disable next MQTT subscription to prevent loop
  if (!strcmp(SettingsText(SET_MQTTPREFIX1), SettingsText(SET_MQTTPREFIX2))) {
    char *str = strstr(topic, SettingsText(SET_MQTTPREFIX1));
    if (str == topic) {
      mqtt_cmnd_blocked_reset = 4;  // Allow up to four seconds before resetting residual cmnd blocks
      mqtt_cmnd_blocked++;
    }
  }

  bool result = MqttClient.publish(topic, mqtt_data, retained);
  yield();  // #3313
  return result;
}

void MqttDataHandler(char* mqtt_topic, uint8_t* mqtt_data, unsigned int data_len)
{
#ifdef USE_DEBUG_DRIVER
  ShowFreeMem(PSTR("MqttDataHandler"));
#endif

  // Do not allow more data than would be feasable within stack space
  if (data_len >= MQTT_MAX_PACKET_SIZE) { return; }

  // Do not execute multiple times if Prefix1 equals Prefix2
  if (!strcmp(SettingsText(SET_MQTTPREFIX1), SettingsText(SET_MQTTPREFIX2))) {
    char *str = strstr(mqtt_topic, SettingsText(SET_MQTTPREFIX1));
    if ((str == mqtt_topic) && mqtt_cmnd_blocked) {
      mqtt_cmnd_blocked--;
      return;
    }
  }

  // Save MQTT data ASAP as it's data is discarded by PubSubClient with next publish as used in MQTTlog
  char topic[TOPSZ];
  strlcpy(topic, mqtt_topic, sizeof(topic));
  mqtt_data[data_len] = 0;
  char data[data_len +1];
  memcpy(data, mqtt_data, sizeof(data));

  AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_MQTT D_RECEIVED_TOPIC " \"%s\", " D_DATA_SIZE " %d, " D_DATA " \"%s\""), topic, data_len, data);
//  if (LOG_LEVEL_DEBUG_MORE <= seriallog_level) { Serial.println(data); }

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

void MqttRetryCounter(uint8_t value)
{
  Mqtt.retry_counter = value;
}

void MqttSubscribe(const char *topic)
{
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT D_SUBSCRIBE_TO " %s"), topic);
  MqttSubscribeLib(topic);
}

void MqttUnsubscribe(const char *topic)
{
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT D_UNSUBSCRIBE_FROM " %s"), topic);
  MqttUnsubscribeLib(topic);
}

void MqttPublishLogging(const char *mxtime)
{
  char saved_mqtt_data[strlen(mqtt_data) +1];
  memcpy(saved_mqtt_data, mqtt_data, sizeof(saved_mqtt_data));

//    ResponseTime_P(PSTR(",\"Log\":{\"%s\"}}"), log_data);  // Will fail as some messages contain JSON
  Response_P(PSTR("%s%s"), mxtime, log_data);            // No JSON and ugly!!
  char stopic[TOPSZ];
  GetTopic_P(stopic, STAT, mqtt_topic, PSTR("LOGGING"));
  MqttPublishLib(stopic, false);

  memcpy(mqtt_data, saved_mqtt_data, sizeof(saved_mqtt_data));
}

void MqttPublish(const char* topic, bool retained)
{
#ifdef USE_DEBUG_DRIVER
  ShowFreeMem(PSTR("MqttPublish"));
#endif

#if defined(USE_MQTT_TLS) && defined(USE_MQTT_AWS_IOT) || defined(MQTT_NO_RETAIN)
//  if (retained) {
//    AddLog_P(LOG_LEVEL_INFO, S_LOG_MQTT, PSTR("Retained are not supported by AWS IoT, using retained = false."));
//  }
  retained = false;   // AWS IoT does not support retained, it will disconnect if received
#endif

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

  snprintf_P(log_data, sizeof(log_data), PSTR("%s%s = %s"), slog_type, (Settings.flag.mqtt_enabled) ? topic : strrchr(topic,'/')+1, mqtt_data);  // SetOption3 - Enable MQTT
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

void MqttPublish(const char* topic)
{
  MqttPublish(topic, false);
}

void MqttPublishPrefixTopic_P(uint32_t prefix, const char* subtopic, bool retained)
{
/* prefix 0 = cmnd using subtopic
 * prefix 1 = stat using subtopic
 * prefix 2 = tele using subtopic
 * prefix 4 = cmnd using subtopic or RESULT
 * prefix 5 = stat using subtopic or RESULT
 * prefix 6 = tele using subtopic or RESULT
 */
  char romram[64];
  char stopic[TOPSZ];

  snprintf_P(romram, sizeof(romram), ((prefix > 3) && !Settings.flag.mqtt_response) ? S_RSLT_RESULT : subtopic);  // SetOption4 - Switch between MQTT RESULT or COMMAND
  for (uint32_t i = 0; i < strlen(romram); i++) {
    romram[i] = toupper(romram[i]);
  }
  prefix &= 3;
  GetTopic_P(stopic, prefix, mqtt_topic, romram);
  MqttPublish(stopic, retained);

#ifdef USE_MQTT_AWS_IOT
  if ((prefix > 0) && (Settings.flag4.awsiot_shadow)) {    // placeholder for SetOptionXX
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
    char *mqtt_save = (char*) malloc(strlen(mqtt_data)+1);
    if (!mqtt_save) { return; }    // abort
    strcpy(mqtt_save, mqtt_data);
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"state\":{\"reported\":%s}}"), mqtt_save);
    free(mqtt_save);

    bool result = MqttClient.publish(romram, mqtt_data, false);
    AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT "Updated shadow: %s"), romram);
    yield();  // #3313
  }
#endif // USE_MQTT_AWS_IOT
}

void MqttPublishPrefixTopic_P(uint32_t prefix, const char* subtopic)
{
  MqttPublishPrefixTopic_P(prefix, subtopic, false);
}

void MqttPublishTeleSensor(void)
{
  MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);  // CMND_SENSORRETAIN
  XdrvRulesProcess();
}

void MqttPublishPowerState(uint32_t device)
{
  char stopic[TOPSZ];
  char scommand[33];

  if ((device < 1) || (device > devices_present)) { device = 1; }

#ifdef USE_SONOFF_IFAN
  if (IsModuleIfan() && (device > 1)) {
    if (GetFanspeed() < MaxFanspeed()) {  // 4 occurs when fanspeed is 3 and RC button 2 is pressed
#ifdef USE_DOMOTICZ
      DomoticzUpdateFanState();  // RC Button feedback
#endif  // USE_DOMOTICZ
      snprintf_P(scommand, sizeof(scommand), PSTR(D_CMND_FANSPEED));
      GetTopic_P(stopic, STAT, mqtt_topic, (Settings.flag.mqtt_response) ? scommand : S_RSLT_RESULT);  // SetOption4 - Switch between MQTT RESULT or COMMAND
      Response_P(S_JSON_COMMAND_NVALUE, scommand, GetFanspeed());
      MqttPublish(stopic);
    }
  } else {
#endif  // USE_SONOFF_IFAN
    GetPowerDevice(scommand, device, sizeof(scommand), Settings.flag.device_index_enable);           // SetOption26 - Switch between POWER or POWER1
    GetTopic_P(stopic, STAT, mqtt_topic, (Settings.flag.mqtt_response) ? scommand : S_RSLT_RESULT);  // SetOption4 - Switch between MQTT RESULT or COMMAND
    Response_P(S_JSON_COMMAND_SVALUE, scommand, GetStateText(bitRead(power, device -1)));
    MqttPublish(stopic);

    GetTopic_P(stopic, STAT, mqtt_topic, scommand);
    Response_P(GetStateText(bitRead(power, device -1)));
    MqttPublish(stopic, Settings.flag.mqtt_power_retain);  // CMND_POWERRETAIN
#ifdef USE_SONOFF_IFAN
  }
#endif  // USE_SONOFF_IFAN
}

void MqttPublishAllPowerState(void)
{
  for (uint32_t i = 1; i <= devices_present; i++) {
    MqttPublishPowerState(i);
#ifdef USE_SONOFF_IFAN
    if (IsModuleIfan()) { break; }  // Report status of light relay only
#endif  // USE_SONOFF_IFAN
  }
}

void MqttPublishPowerBlinkState(uint32_t device)
{
  char scommand[33];

  if ((device < 1) || (device > devices_present)) {
    device = 1;
  }
  Response_P(PSTR("{\"%s\":\"" D_JSON_BLINK " %s\"}"),
    GetPowerDevice(scommand, device, sizeof(scommand), Settings.flag.device_index_enable), GetStateText(bitRead(blink_mask, device -1)));  // SetOption26 - Switch between POWER or POWER1

  MqttPublishPrefixTopic_P(RESULT_OR_STAT, S_RSLT_POWER);
}

/*********************************************************************************************/

uint16_t MqttConnectCount(void)
{
  return Mqtt.connect_count;
}

void MqttDisconnected(int state)
{
  Mqtt.connected = false;
  Mqtt.retry_counter = Settings.mqtt_retry;

  MqttClient.disconnect();

  AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_CONNECT_FAILED_TO " %s:%d, rc %d. " D_RETRY_IN " %d " D_UNIT_SECOND), SettingsText(SET_MQTT_HOST), Settings.mqtt_port, state, Mqtt.retry_counter);
  rules_flag.mqtt_disconnected = 1;
}

void MqttConnected(void)
{
  char stopic[TOPSZ];

  if (Mqtt.allowed) {
    AddLog_P(LOG_LEVEL_INFO, S_LOG_MQTT, PSTR(D_CONNECTED));
    Mqtt.connected = true;
    Mqtt.retry_counter = 0;
    Mqtt.connect_count++;

    GetTopic_P(stopic, TELE, mqtt_topic, S_LWT);
    Response_P(PSTR(D_ONLINE));
    MqttPublish(stopic, true);

    // Satisfy iobroker (#299)
    mqtt_data[0] = '\0';
    MqttPublishPrefixTopic_P(CMND, S_RSLT_POWER);

    GetTopic_P(stopic, CMND, mqtt_topic, PSTR("#"));
    MqttSubscribe(stopic);
    if (strstr_P(SettingsText(SET_MQTT_FULLTOPIC), MQTT_TOKEN_TOPIC) != nullptr) {
      GetGroupTopic_P(stopic, PSTR("#"));  // SetOption75 0: %prefix%/nothing/%topic% = cmnd/nothing/<grouptopic>/# or SetOption75 1: cmnd/<grouptopic>
      MqttSubscribe(stopic);
      GetFallbackTopic_P(stopic, PSTR("#"));
      MqttSubscribe(stopic);
    }

    XdrvCall(FUNC_MQTT_SUBSCRIBE);
  }

  if (Mqtt.initial_connection_state) {
    char stopic2[TOPSZ];
    Response_P(PSTR("{\"" D_CMND_MODULE "\":\"%s\",\"" D_JSON_VERSION "\":\"%s%s\",\"" D_JSON_FALLBACKTOPIC "\":\"%s\",\"" D_CMND_GROUPTOPIC "\":\"%s\"}"),
      ModuleName().c_str(), my_version, my_image, GetFallbackTopic_P(stopic, ""), GetGroupTopic_P(stopic2, ""));
    MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_INFO "1"));
#ifdef USE_WEBSERVER
    if (Settings.webserver) {
#if LWIP_IPV6
      Response_P(PSTR("{\"" D_JSON_WEBSERVER_MODE "\":\"%s\",\"" D_CMND_HOSTNAME "\":\"%s\",\"" D_CMND_IPADDRESS "\":\"%s\",\"IPv6Address\":\"%s\"}"),
        (2 == Settings.webserver) ? D_ADMIN : D_USER, my_hostname, WiFi.localIP().toString().c_str(),WifiGetIPv6().c_str());
#else
      Response_P(PSTR("{\"" D_JSON_WEBSERVER_MODE "\":\"%s\",\"" D_CMND_HOSTNAME "\":\"%s\",\"" D_CMND_IPADDRESS "\":\"%s\"}"),
        (2 == Settings.webserver) ? D_ADMIN : D_USER, my_hostname, WiFi.localIP().toString().c_str());
#endif // LWIP_IPV6 = 1
      MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_INFO "2"));
    }
#endif  // USE_WEBSERVER
    Response_P(PSTR("{\"" D_JSON_RESTARTREASON "\":"));
    if (CrashFlag()) {
      CrashDump();
    } else {
      ResponseAppend_P(PSTR("\"%s\""), GetResetReason().c_str());
    }
    ResponseJsonEnd();
    MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_INFO "3"));
    MqttPublishAllPowerState();
    if (Settings.tele_period) {
      tele_period = Settings.tele_period -5;  // Enable TelePeriod in 5 seconds
    }
    rules_flag.system_boot = 1;
    XdrvCall(FUNC_MQTT_INIT);
  }
  Mqtt.initial_connection_state = 0;

  global_state.mqtt_down = 0;
  if (Settings.flag.mqtt_enabled) {  // SetOption3 - Enable MQTT
    rules_flag.mqtt_connected = 1;
  }
}

void MqttReconnect(void)
{
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
    if (!AWS_IoT_Private_Key || !AWS_IoT_Client_Certificate) {
      Mqtt.allowed = false;
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

  AddLog_P(LOG_LEVEL_INFO, S_LOG_MQTT, PSTR(D_ATTEMPTING_CONNECTION));

  Mqtt.connected = false;
  Mqtt.retry_counter = Settings.mqtt_retry;
  global_state.mqtt_down = 1;

  char *mqtt_user = nullptr;
  char *mqtt_pwd = nullptr;
  if (strlen(SettingsText(SET_MQTT_USER))) {
    mqtt_user = SettingsText(SET_MQTT_USER);
  }
  if (strlen(SettingsText(SET_MQTT_PWD))) {
    mqtt_pwd = SettingsText(SET_MQTT_PWD);
  }

  GetTopic_P(stopic, TELE, mqtt_topic, S_LWT);
  Response_P(S_OFFLINE);

  if (MqttClient.connected()) { MqttClient.disconnect(); }
#ifdef USE_MQTT_TLS
  tlsClient->stop();
#else
  EspClient = WiFiClient();               // Wifi Client reconnect issue 4497 (https://github.com/esp8266/Arduino/issues/4497)
  MqttClient.setClient(EspClient);
#endif

  if (2 == Mqtt.initial_connection_state) {  // Executed once just after power on and wifi is connected
    Mqtt.initial_connection_state = 1;
  }

  MqttClient.setCallback(MqttDataHandler);
#if defined(USE_MQTT_TLS) && defined(USE_MQTT_AWS_IOT)
  // re-assign private keys in case it was updated in between
  tlsClient->setClientECCert(AWS_IoT_Client_Certificate,
                             AWS_IoT_Private_Key,
                             0xFFFF /* all usages, don't care */, 0);
#endif
  MqttClient.setServer(SettingsText(SET_MQTT_HOST), Settings.mqtt_port);

  uint32_t mqtt_connect_time = millis();
#if defined(USE_MQTT_TLS) && !defined(USE_MQTT_TLS_CA_CERT)
  bool allow_all_fingerprints = false;
  bool learn_fingerprint1 = is_fingerprint_mono_value(Settings.mqtt_fingerprint[0], 0x00);
  bool learn_fingerprint2 = is_fingerprint_mono_value(Settings.mqtt_fingerprint[1], 0x00);
  allow_all_fingerprints |= is_fingerprint_mono_value(Settings.mqtt_fingerprint[0], 0xff);
  allow_all_fingerprints |= is_fingerprint_mono_value(Settings.mqtt_fingerprint[1], 0xff);
  allow_all_fingerprints |= learn_fingerprint1;
  allow_all_fingerprints |= learn_fingerprint2;
  tlsClient->setPubKeyFingerprint(Settings.mqtt_fingerprint[0], Settings.mqtt_fingerprint[1], allow_all_fingerprints);
#endif
#if defined(USE_MQTT_TLS) && defined(USE_MQTT_AWS_IOT)
  AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT "AWS IoT endpoint: %s"), SettingsText(SET_MQTT_HOST));
  if (MqttClient.connect(mqtt_client, nullptr, nullptr, stopic, 1, false, mqtt_data, MQTT_CLEAN_SESSION)) {
#else
  if (MqttClient.connect(mqtt_client, mqtt_user, mqtt_pwd, stopic, 1, true, mqtt_data, MQTT_CLEAN_SESSION)) {
#endif
#ifdef USE_MQTT_TLS
    AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT "TLS connected in %d ms, max ThunkStack used %d"),
      millis() - mqtt_connect_time, tlsClient->getMaxThunkStackUse());
    if (!tlsClient->getMFLNStatus()) {
      AddLog_P(LOG_LEVEL_INFO, S_LOG_MQTT, PSTR("MFLN not supported by TLS server"));
    }
#ifndef USE_MQTT_TLS_CA_CERT  // don't bother with fingerprints if using CA validation
    // create a printable version of the fingerprint received
    char buf_fingerprint[64];
    ToHex_P((unsigned char *)tlsClient->getRecvPubKeyFingerprint(), 20, buf_fingerprint, sizeof(buf_fingerprint), ' ');
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT "Server fingerprint: %s"), buf_fingerprint);

    if (learn_fingerprint1 || learn_fingerprint2) {
      // we potentially need to learn the fingerprint just seen
      bool fingerprint_matched = false;
      const uint8_t *recv_fingerprint = tlsClient->getRecvPubKeyFingerprint();
      if (0 == memcmp(recv_fingerprint, Settings.mqtt_fingerprint[0], 20)) {
        fingerprint_matched = true;
      }
      if (0 == memcmp(recv_fingerprint, Settings.mqtt_fingerprint[1], 20)) {
        fingerprint_matched = true;
      }
      if (!fingerprint_matched) {
        // we had no match, so we need to change all fingerprints ready to learn
        if (learn_fingerprint1) {
          memcpy(Settings.mqtt_fingerprint[0], recv_fingerprint, 20);
        }
        if (learn_fingerprint2) {
          memcpy(Settings.mqtt_fingerprint[1], recv_fingerprint, 20);
        }
        AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT "Fingerprint learned: %s"), buf_fingerprint);

        SettingsSaveAll();  // save settings
      }
    }
#endif // !USE_MQTT_TLS_CA_CERT
#endif // USE_MQTT_TLS
    MqttConnected();
  } else {
#ifdef USE_MQTT_TLS
    AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT "TLS connection error: %d"), tlsClient->getLastError());
#endif
    MqttDisconnected(MqttClient.state());  // status codes are documented here http://pubsubclient.knolleary.net/api.html#state
  }
}

void MqttCheck(void)
{
  if (Settings.flag.mqtt_enabled) {  // SetOption3 - Enable MQTT
    if (!MqttIsConnected()) {
      global_state.mqtt_down = 1;
      if (!Mqtt.retry_counter) {
        MqttReconnect();
      } else {
        Mqtt.retry_counter--;
      }
    } else {
      global_state.mqtt_down = 0;
    }
  } else {
    global_state.mqtt_down = 0;
    if (Mqtt.initial_connection_state) {
      MqttReconnect();
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

#if defined(USE_MQTT_TLS) && !defined(USE_MQTT_TLS_CA_CERT)
void CmndMqttFingerprint(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 2)) {
    char fingerprint[60];
    if ((XdrvMailbox.data_len > 0) && (XdrvMailbox.data_len < sizeof(fingerprint))) {
      strlcpy(fingerprint, (SC_CLEAR == Shortcut()) ? "" : (SC_DEFAULT == Shortcut()) ? (1 == XdrvMailbox.index) ? MQTT_FINGERPRINT1 : MQTT_FINGERPRINT2 : XdrvMailbox.data, sizeof(fingerprint));
      char *p = fingerprint;
      for (uint32_t i = 0; i < 20; i++) {
        Settings.mqtt_fingerprint[XdrvMailbox.index -1][i] = strtol(p, &p, 16);
      }
      restart_flag = 2;
    }
    ResponseCmndIdxChar(ToHex_P((unsigned char *)Settings.mqtt_fingerprint[XdrvMailbox.index -1], 20, fingerprint, sizeof(fingerprint), ' '));
  }
}
#endif

void CmndMqttUser(void)
{
  if (XdrvMailbox.data_len > 0) {
    SettingsUpdateText(SET_MQTT_USER, (SC_CLEAR == Shortcut()) ? "" : (SC_DEFAULT == Shortcut()) ? MQTT_USER : XdrvMailbox.data);
    restart_flag = 2;
  }
  ResponseCmndChar(SettingsText(SET_MQTT_USER));
}

void CmndMqttPassword(void)
{
  if (XdrvMailbox.data_len > 0) {
    SettingsUpdateText(SET_MQTT_PWD, (SC_CLEAR == Shortcut()) ? "" : (SC_DEFAULT == Shortcut()) ? MQTT_PASS : XdrvMailbox.data);
    ResponseCmndChar(SettingsText(SET_MQTT_PWD));
    restart_flag = 2;
  } else {
    Response_P(S_JSON_COMMAND_ASTERISK, XdrvMailbox.command);
  }
}

void CmndMqttlog(void)
{
  if ((XdrvMailbox.payload >= LOG_LEVEL_NONE) && (XdrvMailbox.payload <= LOG_LEVEL_DEBUG_MORE)) {
    Settings.mqttlog_level = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings.mqttlog_level);
}

void CmndMqttHost(void)
{
  if (XdrvMailbox.data_len > 0) {
    SettingsUpdateText(SET_MQTT_HOST, (SC_CLEAR == Shortcut()) ? "" : (SC_DEFAULT == Shortcut()) ? MQTT_HOST : XdrvMailbox.data);
    restart_flag = 2;
  }
  ResponseCmndChar(SettingsText(SET_MQTT_HOST));
}

void CmndMqttPort(void)
{
  if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload < 65536)) {
    Settings.mqtt_port = (1 == XdrvMailbox.payload) ? MQTT_PORT : XdrvMailbox.payload;
    restart_flag = 2;
  }
  ResponseCmndNumber(Settings.mqtt_port);
}

void CmndMqttRetry(void)
{
  if ((XdrvMailbox.payload >= MQTT_RETRY_SECS) && (XdrvMailbox.payload < 32001)) {
    Settings.mqtt_retry = XdrvMailbox.payload;
    Mqtt.retry_counter = Settings.mqtt_retry;
  }
  ResponseCmndNumber(Settings.mqtt_retry);
}

void CmndStateText(void)
{
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

void CmndMqttClient(void)
{
  if (!XdrvMailbox.grpflg && (XdrvMailbox.data_len > 0)) {
    SettingsUpdateText(SET_MQTT_CLIENT, (SC_DEFAULT == Shortcut()) ? MQTT_CLIENT_ID : XdrvMailbox.data);
    restart_flag = 2;
  }
  ResponseCmndChar(SettingsText(SET_MQTT_CLIENT));
}

void CmndFullTopic(void)
{
  if (XdrvMailbox.data_len > 0) {
    MakeValidMqtt(1, XdrvMailbox.data);
    if (!strcmp(XdrvMailbox.data, mqtt_client)) { SetShortcutDefault(); }
    char stemp1[TOPSZ];
    strlcpy(stemp1, (SC_DEFAULT == Shortcut()) ? MQTT_FULLTOPIC : XdrvMailbox.data, sizeof(stemp1));
    if (strcmp(stemp1, SettingsText(SET_MQTT_FULLTOPIC))) {
      Response_P((Settings.flag.mqtt_offline) ? S_OFFLINE : "");  // SetOption10 - Control MQTT LWT message format
      MqttPublishPrefixTopic_P(TELE, PSTR(D_LWT), true);          // Offline or remove previous retained topic
      SettingsUpdateText(SET_MQTT_FULLTOPIC, stemp1);
      restart_flag = 2;
    }
  }
  ResponseCmndChar(SettingsText(SET_MQTT_FULLTOPIC));
}

void CmndPrefix(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_MQTT_PREFIXES)) {
    if (!XdrvMailbox.usridx) {
      ResponseCmndAll(SET_MQTTPREFIX1, MAX_MQTT_PREFIXES);
    } else {
      if (XdrvMailbox.data_len > 0) {
        MakeValidMqtt(0, XdrvMailbox.data);
        SettingsUpdateText(SET_MQTTPREFIX1 + XdrvMailbox.index -1,
          (SC_DEFAULT == Shortcut()) ? (1==XdrvMailbox.index) ? SUB_PREFIX : (2==XdrvMailbox.index) ? PUB_PREFIX : PUB_PREFIX2 : XdrvMailbox.data);
        restart_flag = 2;
      }
      ResponseCmndIdxChar(SettingsText(SET_MQTTPREFIX1 + XdrvMailbox.index -1));
    }
  }
}

void CmndPublish(void)
{
  if (XdrvMailbox.data_len > 0) {
    char *payload_part;
    char *mqtt_part = strtok_r(XdrvMailbox.data, " ", &payload_part);
    if (mqtt_part) {
      char stemp1[TOPSZ];
      strlcpy(stemp1, mqtt_part, sizeof(stemp1));
      if ((payload_part != nullptr) && strlen(payload_part)) {
        strlcpy(mqtt_data, payload_part, sizeof(mqtt_data));
      } else {
        mqtt_data[0] = '\0';
      }
      MqttPublish(stemp1, (XdrvMailbox.index == 2));
//      ResponseCmndDone();
      mqtt_data[0] = '\0';
    }
  }
}

void CmndGroupTopic(void)
{
  if (XdrvMailbox.data_len > 0) {
    MakeValidMqtt(0, XdrvMailbox.data);
    if (!strcmp(XdrvMailbox.data, mqtt_client)) { SetShortcutDefault(); }
    SettingsUpdateText(SET_MQTT_GRP_TOPIC, (SC_DEFAULT == Shortcut()) ? MQTT_GRPTOPIC : XdrvMailbox.data);
    restart_flag = 2;
  }
  ResponseCmndChar(SettingsText(SET_MQTT_GRP_TOPIC));
}

void CmndTopic(void)
{
  if (!XdrvMailbox.grpflg && (XdrvMailbox.data_len > 0)) {
    MakeValidMqtt(0, XdrvMailbox.data);
    if (!strcmp(XdrvMailbox.data, mqtt_client)) { SetShortcutDefault(); }
    char stemp1[TOPSZ];
    strlcpy(stemp1, (SC_DEFAULT == Shortcut()) ? MQTT_TOPIC : XdrvMailbox.data, sizeof(stemp1));
    if (strcmp(stemp1, SettingsText(SET_MQTT_TOPIC))) {
      Response_P((Settings.flag.mqtt_offline) ? S_OFFLINE : "");  // SetOption10 - Control MQTT LWT message format
      MqttPublishPrefixTopic_P(TELE, PSTR(D_LWT), true);          // Offline or remove previous retained topic
      SettingsUpdateText(SET_MQTT_TOPIC, stemp1);
      restart_flag = 2;
    }
  }
  ResponseCmndChar(SettingsText(SET_MQTT_TOPIC));
}

void CmndButtonTopic(void)
{
  if (!XdrvMailbox.grpflg && (XdrvMailbox.data_len > 0)) {
    MakeValidMqtt(0, XdrvMailbox.data);
    if (!strcmp(XdrvMailbox.data, mqtt_client)) { SetShortcutDefault(); }
    switch (Shortcut()) {
      case SC_CLEAR: SettingsUpdateText(SET_MQTT_BUTTON_TOPIC, ""); break;
      case SC_DEFAULT: SettingsUpdateText(SET_MQTT_BUTTON_TOPIC, mqtt_topic); break;
      case SC_USER: SettingsUpdateText(SET_MQTT_BUTTON_TOPIC, MQTT_BUTTON_TOPIC); break;
      default: SettingsUpdateText(SET_MQTT_BUTTON_TOPIC, XdrvMailbox.data);
    }
  }
  ResponseCmndChar(SettingsText(SET_MQTT_BUTTON_TOPIC));
}

void CmndSwitchTopic(void)
{
  if (!XdrvMailbox.grpflg && (XdrvMailbox.data_len > 0)) {
    MakeValidMqtt(0, XdrvMailbox.data);
    if (!strcmp(XdrvMailbox.data, mqtt_client)) { SetShortcutDefault(); }
    switch (Shortcut()) {
      case SC_CLEAR: SettingsUpdateText(SET_MQTT_SWITCH_TOPIC, ""); break;
      case SC_DEFAULT: SettingsUpdateText(SET_MQTT_SWITCH_TOPIC, mqtt_topic); break;
      case SC_USER: SettingsUpdateText(SET_MQTT_SWITCH_TOPIC, MQTT_SWITCH_TOPIC); break;
      default: SettingsUpdateText(SET_MQTT_SWITCH_TOPIC, XdrvMailbox.data);
    }
  }
  ResponseCmndChar(SettingsText(SET_MQTT_SWITCH_TOPIC));
}

void CmndButtonRetain(void)
{
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

void CmndSwitchRetain(void)
{
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

void CmndPowerRetain(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    if (!XdrvMailbox.payload) {
      char stemp1[TOPSZ];
      char scommand[CMDSZ];
      for (uint32_t i = 1; i <= devices_present; i++) {  // Clear MQTT retain in broker
        GetTopic_P(stemp1, STAT, mqtt_topic, GetPowerDevice(scommand, i, sizeof(scommand), Settings.flag.device_index_enable));  // SetOption26 - Switch between POWER or POWER1
        mqtt_data[0] = '\0';
        MqttPublish(stemp1, Settings.flag.mqtt_power_retain);  // CMND_POWERRETAIN
      }
    }
    Settings.flag.mqtt_power_retain = XdrvMailbox.payload;     // CMND_POWERRETAIN
  }
  ResponseCmndStateText(Settings.flag.mqtt_power_retain);      // CMND_POWERRETAIN
}

void CmndSensorRetain(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    if (!XdrvMailbox.payload) {
      mqtt_data[0] = '\0';
      MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);  // CMND_SENSORRETAIN
      MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_ENERGY), Settings.flag.mqtt_sensor_retain);  // CMND_SENSORRETAIN
    }
    Settings.flag.mqtt_sensor_retain = XdrvMailbox.payload;                                   // CMND_SENSORRETAIN
  }
  ResponseCmndStateText(Settings.flag.mqtt_sensor_retain);                                    // CMND_SENSORRETAIN
}

/*********************************************************************************************\
 * TLS private key and certificate - store into Flash
\*********************************************************************************************/
#if defined(USE_MQTT_TLS) && defined(USE_MQTT_AWS_IOT)

// const static uint16_t tls_spi_start_sector = SPIFFS_END + 4;  // 0xXXFF
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
        AddLog_P(LOG_LEVEL_ERROR, ALLOCATE_ERROR);
        return;
      }
      memcpy_P(spi_buffer, tls_spi_start, tls_spi_len);

      // remove any white space from the base64
      RemoveAllSpaces(XdrvMailbox.data);

      // allocate buffer for decoded base64
      uint32_t bin_len = decode_base64_length((unsigned char*)XdrvMailbox.data);
      uint8_t  *bin_buf = nullptr;
      if (bin_len > 0) {
        bin_buf = (uint8_t*) malloc(bin_len + 4);
        if (!bin_buf) {
          AddLog_P(LOG_LEVEL_ERROR, ALLOCATE_ERROR);
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
            AddLog_P2(LOG_LEVEL_INFO, PSTR("TLSKey: Certificate must be 32 bytes: %d."), bin_len);
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
          AddLog_P(LOG_LEVEL_INFO, PSTR("TLSKey: cannot store Cert if no Key previously stored."));
          free(spi_buffer);
          free(bin_buf);
          return;
        }
        if (bin_len <= 256) {
          // Certificate lenght too short
          AddLog_P2(LOG_LEVEL_INFO, PSTR("TLSKey: Certificate length too short: %d."), bin_len);
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
#ifdef ARDUINO_ESP8266_RELEASE_2_3_0
    Serial.printf("%08x:  %08x %08x %08x %08x\n", pos, bswap32(values[0]), bswap32(values[1]), bswap32(values[2]), bswap32(values[3]));
#else
    Serial.printf_P(PSTR("%08x:  %08x %08x %08x %08x\n"), pos, bswap32(values[0]), bswap32(values[1]), bswap32(values[2]), bswap32(values[3]));
#endif
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
  "<p><b>" D_HOST "</b> (" MQTT_HOST ")<br><input id='mh' placeholder='" MQTT_HOST" ' value='%s'></p>"
  "<p><b>" D_PORT "</b> (" STR(MQTT_PORT) ")<br><input id='ml' placeholder='" STR(MQTT_PORT) "' value='%d'></p>"
  "<p><b>" D_CLIENT "</b> (%s)<br><input id='mc' placeholder='%s' value='%s'></p>";
const char HTTP_FORM_MQTT2[] PROGMEM =
  "<p><b>" D_USER "</b> (" MQTT_USER ")<br><input id='mu' placeholder='" MQTT_USER "' value='%s'></p>"
  "<p><b>" D_PASSWORD "</b><input type='checkbox' onclick='sp(\"mp\")'><br><input id='mp' type='password' placeholder='" D_PASSWORD "' value='" D_ASTERISK_PWD "'></p>"
  "<p><b>" D_TOPIC "</b> = %%topic%% (%s)<br><input id='mt' placeholder='%s' value='%s'></p>"
  "<p><b>" D_FULL_TOPIC "</b> (%s)<br><input id='mf' placeholder='%s' value='%s'></p>";

void HandleMqttConfiguration(void)
{
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURE_MQTT);

  if (WebServer->hasArg("save")) {
    MqttSaveSettings();
    WebRestart(1);
    return;
  }

  char str[TOPSZ];

  WSContentStart_P(S_CONFIGURE_MQTT);
  WSContentSendStyle();
  WSContentSend_P(HTTP_FORM_MQTT1,
    SettingsText(SET_MQTT_HOST),
    Settings.mqtt_port,
    Format(str, MQTT_CLIENT_ID, sizeof(str)), MQTT_CLIENT_ID, SettingsText(SET_MQTT_CLIENT));
  WSContentSend_P(HTTP_FORM_MQTT2,
    (!strlen(SettingsText(SET_MQTT_USER))) ? "0" : SettingsText(SET_MQTT_USER),
    Format(str, MQTT_TOPIC, sizeof(str)), MQTT_TOPIC, SettingsText(SET_MQTT_TOPIC),
    MQTT_FULLTOPIC, MQTT_FULLTOPIC, SettingsText(SET_MQTT_FULLTOPIC));
  WSContentSend_P(HTTP_FORM_END);
  WSContentSpaceButton(BUTTON_CONFIGURATION);
  WSContentStop();
}

void MqttSaveSettings(void)
{
  char tmp[TOPSZ];
  char stemp[TOPSZ];
  char stemp2[TOPSZ];

  WebGetArg("mt", tmp, sizeof(tmp));
  strlcpy(stemp, (!strlen(tmp)) ? MQTT_TOPIC : tmp, sizeof(stemp));
  MakeValidMqtt(0, stemp);
  WebGetArg("mf", tmp, sizeof(tmp));
  strlcpy(stemp2, (!strlen(tmp)) ? MQTT_FULLTOPIC : tmp, sizeof(stemp2));
  MakeValidMqtt(1, stemp2);
  if ((strcmp(stemp, SettingsText(SET_MQTT_TOPIC))) || (strcmp(stemp2, SettingsText(SET_MQTT_FULLTOPIC)))) {
    Response_P((Settings.flag.mqtt_offline) ? S_OFFLINE : "");  // SetOption10 - Control MQTT LWT message format
    MqttPublishPrefixTopic_P(TELE, S_LWT, true);                // Offline or remove previous retained topic
  }
  SettingsUpdateText(SET_MQTT_TOPIC, stemp);
  SettingsUpdateText(SET_MQTT_FULLTOPIC, stemp2);
  WebGetArg("mh", tmp, sizeof(tmp));
  SettingsUpdateText(SET_MQTT_HOST, (!strlen(tmp)) ? MQTT_HOST : (!strcmp(tmp,"0")) ? "" : tmp);
  WebGetArg("ml", tmp, sizeof(tmp));
  Settings.mqtt_port = (!strlen(tmp)) ? MQTT_PORT : atoi(tmp);
  WebGetArg("mc", tmp, sizeof(tmp));
  SettingsUpdateText(SET_MQTT_CLIENT, (!strlen(tmp)) ? MQTT_CLIENT_ID : tmp);
#if defined(USE_MQTT_TLS) && defined(USE_MQTT_AWS_IOT)
  AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_CMND_MQTTHOST " %s, " D_CMND_MQTTPORT " %d, " D_CMND_MQTTCLIENT " %s, " D_CMND_TOPIC " %s, " D_CMND_FULLTOPIC " %s"),
    SettingsText(SET_MQTT_HOST), Settings.mqtt_port, SettingsText(SET_MQTT_CLIENT), SettingsText(SET_MQTT_TOPIC), SettingsText(SET_MQTT_FULLTOPIC));
#else // USE_MQTT_AWS_IOT
  WebGetArg("mu", tmp, sizeof(tmp));
  SettingsUpdateText(SET_MQTT_USER, (!strlen(tmp)) ? MQTT_USER : (!strcmp(tmp,"0")) ? "" : tmp);
  WebGetArg("mp", tmp, sizeof(tmp));
  SettingsUpdateText(SET_MQTT_PWD, (!strlen(tmp)) ? "" : (!strcmp(tmp, D_ASTERISK_PWD)) ? SettingsText(SET_MQTT_PWD) : tmp);
  AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_CMND_MQTTHOST " %s, " D_CMND_MQTTPORT " %d, " D_CMND_MQTTCLIENT " %s, " D_CMND_MQTTUSER " %s, " D_CMND_TOPIC " %s, " D_CMND_FULLTOPIC " %s"),
    SettingsText(SET_MQTT_HOST), Settings.mqtt_port, SettingsText(SET_MQTT_CLIENT), SettingsText(SET_MQTT_USER), SettingsText(SET_MQTT_TOPIC), SettingsText(SET_MQTT_FULLTOPIC));
#endif
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
        WebServer->on("/" WEB_HANDLE_MQTT, HandleMqttConfiguration);
        break;
#endif  // USE_WEBSERVER
      case FUNC_COMMAND:
        result = DecodeCommand(kMqttCommands, MqttCommand);
        break;
    }
  }
  return result;
}
