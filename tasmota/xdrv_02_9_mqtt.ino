/*
  xdrv_02_9_mqtt.ino - mqtt support for Tasmota

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

#define USE_MQTT_NEW_PUBSUBCLIENT

// #define DEBUG_DUMP_TLS    // allow dumping of TLS Flash keys

#ifdef USE_MQTT_TLS
  #include "WiFiClientSecureLightBearSSL.h"
  BearSSL::WiFiClientSecure_light *tlsClient;
#endif
WiFiClient EspClient;                     // Wifi Client - non-TLS

#ifdef USE_MQTT_AZURE_IOT
#undef  MQTT_PORT
#define MQTT_PORT         8883
#if defined(USE_MQTT_AZURE_DPS_SCOPEID) && defined(USE_MQTT_AZURE_DPS_PRESHAREDKEY)
  #include <ESP8266HTTPClient.h>
  // dedicated tlsHttpsClient for DPS as the 'tlsClient' above causes error '-1' in httpsClient after it is associated with PubSub.  It cost ~5K of heap
  BearSSL::WiFiClientSecure_light *tlsHttpsClient = new BearSSL::WiFiClientSecure_light(1024,1024);
  HTTPClient httpsClient;
  int httpsClientReturn;
#endif // USE_MQTT_AZURE_DPS_SCOPEID
  #include <base64.hpp>
  #include <t_bearssl.h>
  #include <JsonParser.h>
#endif  // USE_MQTT_AZURE_IOT

const char kMqttCommands[] PROGMEM = "|"  // No prefix
  // SetOption synonyms
  D_SO_MQTTJSONONLY "|"
#ifdef USE_MQTT_TLS
  D_SO_MQTTTLS "|" D_SO_MQTTTLS_FINGERPRINT "|"
#endif
  D_SO_MQTTNORETAIN "|" D_SO_MQTTDETACHRELAY "|"
  // regular commands
#if defined(USE_MQTT_TLS)
  D_CMND_MQTTFINGERPRINT "|"
#endif
  D_CMND_MQTTUSER "|" D_CMND_MQTTPASSWORD "|" D_CMND_MQTTKEEPALIVE "|" D_CMND_MQTTTIMEOUT "|" D_CMND_MQTTWIFITIMEOUT "|"
#if defined(USE_MQTT_TLS) && defined(USE_MQTT_AWS_IOT)
  D_CMND_TLSKEY "|"
#endif
#ifdef USE_MQTT_FILE
  D_CMND_FILEUPLOAD "|" D_CMND_FILEDOWNLOAD "|"
#endif  // USE_MQTT_FILE
  D_CMND_MQTTHOST "|" D_CMND_MQTTPORT "|" D_CMND_MQTTRETRY "|" D_CMND_STATETEXT "|" D_CMND_MQTTCLIENT "|"
  D_CMND_FULLTOPIC "|" D_CMND_PREFIX "|" D_CMND_GROUPTOPIC "|" D_CMND_TOPIC "|" D_CMND_PUBLISH "|" D_CMND_MQTTLOG "|"
  D_CMND_BUTTONTOPIC "|" D_CMND_SWITCHTOPIC "|" D_CMND_BUTTONRETAIN "|" D_CMND_SWITCHRETAIN "|" D_CMND_POWERRETAIN "|"
  D_CMND_SENSORRETAIN "|" D_CMND_INFORETAIN "|" D_CMND_STATERETAIN ;

SO_SYNONYMS(kMqttSynonyms,
  90,
#ifdef USE_MQTT_TLS
  103, 132,
#endif
  104, 114
);

void (* const MqttCommand[])(void) PROGMEM = {
#if defined(USE_MQTT_TLS)
  &CmndMqttFingerprint,
#endif
  &CmndMqttUser, &CmndMqttPassword, &CmndMqttKeepAlive, &CmndMqttTimeout, &CmndMqttWifiTimeout,
#if defined(USE_MQTT_TLS) && defined(USE_MQTT_AWS_IOT)
  &CmndTlsKey,
#endif
#ifdef USE_MQTT_FILE
  &CmndFileUpload, &CmndFileDownload,
#endif  // USE_MQTT_FILE
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
  bool disable_logging = false;          // Temporarly disable logging on some commands
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

void MqttDisableLogging(bool state) {
  // Disable logging only on repeating MQTT messages
  Mqtt.disable_logging = state;
  TasmotaGlobal.masterlog_level = (Mqtt.disable_logging) ? LOG_LEVEL_DEBUG_MORE : LOG_LEVEL_NONE;
}

/*********************************************************************************************\
 * MQTT driver specific code need to provide the following functions:
 *
 * bool MqttIsConnected()
 * void MqttDisconnect()
 * void MqttSubscribeLib(char *topic)
 * bool MqttPublishLib(const char* topic, const uint8_t* payload, unsigned int plength, bool retained)
 *
 * Change/Verify PubSubClient.h defines:
 * #define MQTT_MAX_PACKET_SIZE 1200     // Tasmota v8.1.0.8
\*********************************************************************************************/

#include <PubSubClient.h>

PubSubClient MqttClient;

void MqttInit(void) {
#ifdef USE_MQTT_AZURE_IOT
  Settings->mqtt_port = 8883;
#endif //USE_MQTT_AZURE_IOT
#ifdef USE_MQTT_TLS
  bool aws_iot_host = false;
  if ((8883 == Settings->mqtt_port) || (8884 == Settings->mqtt_port) || (443 == Settings->mqtt_port)) {
    // Turn on TLS for port 8883 (TLS), 8884 (TLS, client certificate), 443 (TLS, user/password)
    Settings->flag4.mqtt_tls = true;
  }
  Mqtt.mqtt_tls = Settings->flag4.mqtt_tls;   // this flag should not change even if we change the SetOption (until reboot)

  // Detect AWS IoT and set default parameters
  String host = String(SettingsText(SET_MQTT_HOST));
  if (host.indexOf(F(".iot.")) && host.endsWith(F(".amazonaws.com"))) {  // look for ".iot." and ".amazonaws.com" in the domain name
    Settings->flag4.mqtt_no_retain = true;
    aws_iot_host = true;
  }

  if (Mqtt.mqtt_tls) {
#ifdef ESP32
    tlsClient = new BearSSL::WiFiClientSecure_light(2048,2048);
#else // ESP32 - ESP8266
    tlsClient = new BearSSL::WiFiClientSecure_light(1024,1024);
#endif

    if (443 == Settings->mqtt_port && aws_iot_host) {
      static const char * alpn_mqtt = "mqtt";   // needs to be static
      tlsClient->setALPN(&alpn_mqtt, 1);         // need to set alpn to 'mqtt' for AWS IoT
    }
#ifdef USE_MQTT_AWS_IOT
    loadTlsDir();   // load key and certificate data from Flash
    if ((nullptr != AWS_IoT_Private_Key) && (nullptr != AWS_IoT_Client_Certificate)) {
      tlsClient->setClientECCert(AWS_IoT_Client_Certificate,
                                AWS_IoT_Private_Key,
                                0xFFFF /* all usages, don't care */, 0);
    }
#endif

    if (!Settings->flag5.tls_use_fingerprint) {
      tlsClient->setTrustAnchor(Tasmota_TA, nitems(Tasmota_TA));
    }

    MqttClient.setClient(*tlsClient);
  } else {
    MqttClient.setClient(EspClient);    // non-TLS
  }
#else // USE_MQTT_TLS
  MqttClient.setClient(EspClient);
#endif // USE_MQTT_TLS

  MqttClient.setKeepAlive(Settings->mqtt_keepalive);
  MqttClient.setSocketTimeout(Settings->mqtt_socket_timeout);
}

#ifdef USE_MQTT_AZURE_IOT
  String Sha256Sign(String dataToSign, String preSharedKey){
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT "sha256 dataToSign is '%s'"), String(dataToSign).c_str());
    char dataToSignChar[dataToSign.length() + 1];
    dataToSign.toCharArray(dataToSignChar, dataToSign.length() + 1);

    unsigned char decodedPSK[32];
    unsigned char encryptedSignature[100];
    unsigned char encodedSignature[100];
    br_sha256_context sha256_context;
    br_hmac_key_context hmac_key_context;
    br_hmac_context hmac_context;

    // need to base64 decode the Preshared key and the length
    int base64_decoded_device_length = decode_base64((unsigned char*)preSharedKey.c_str(), decodedPSK);

    // create the sha256 hmac and hash the data
    br_sha256_init(&sha256_context);
    br_hmac_key_init(&hmac_key_context, sha256_context.vtable, decodedPSK, base64_decoded_device_length);
    br_hmac_init(&hmac_context, &hmac_key_context, 32);
    br_hmac_update(&hmac_context, dataToSignChar, sizeof(dataToSignChar)-1);
    br_hmac_out(&hmac_context, encryptedSignature);

    // base64 decode the HMAC to a char
    encode_base64(encryptedSignature, br_hmac_size(&hmac_context), encodedSignature);

    // creating the real SAS Token
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT "sha256 signature is '%s'"), String((char*)encodedSignature).c_str());

    return String((char*)encodedSignature);
  }

  String urlEncodeBase64(String stringToEncode){
    // correctly URL encoding the 64 characters of Base64 and the '=' sign
    stringToEncode.replace("+", "%2B");
    stringToEncode.replace("=", "%3D");
    stringToEncode.replace("/", "%2F");
    return stringToEncode;
  }

  String AzurePSKtoToken(char *iotHubFQDN, const char *deviceId, const char *preSharedKey, int sasTTL = 86400){
    int ttl = time(NULL) + sasTTL;
    String dataToSignString = urlEncodeBase64(String(iotHubFQDN) + "/devices/" + String(deviceId)) + "\n" + String(ttl);
    String signedData = Sha256Sign(dataToSignString, String(preSharedKey));

    // creating the real SAS Token
    String realSASToken = "SharedAccessSignature sr="   + urlEncodeBase64(String(iotHubFQDN) + "/devices/" + String(deviceId));
    realSASToken += "&sig=" + urlEncodeBase64(signedData) + "&se="  + String(ttl);

    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT "Azure IoT Hub SAS Token is '%s'"), realSASToken.c_str());

    return realSASToken;
  }

#if defined(USE_MQTT_AZURE_DPS_SCOPEID) && defined(USE_MQTT_AZURE_DPS_PRESHAREDKEY)
  String AzureDSPPSKtoToken(String scopeId, String deviceId, const char *preSharedKey, int sasTTL = 3600){
    int ttl = time(NULL) + sasTTL;
    String dataToSignString = urlEncodeBase64(scopeId + "/registrations/" + deviceId) + "\n" + String(ttl);
    String signedData = Sha256Sign(dataToSignString, String(preSharedKey));

    // creating the real SAS Token
    String realSASToken = "SharedAccessSignature sr=" + urlEncodeBase64(scopeId + "/registrations/" + deviceId);
    realSASToken += "&sig=" + urlEncodeBase64(signedData) + "&skn=registration" + "&se="  + String(ttl);

    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT "Azure DPS SAS Token is '%s'"), realSASToken.c_str());

    return realSASToken;
  }

  void ProvisionAzureDPS(){
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT "Starting Azure DPS registration..."));
    // Scope and Key are derived from user_config_override.h, USE_MQTT_AZURE_DPS_SCOPE_ENDPOINT is optional
    String dPSScopeId = USE_MQTT_AZURE_DPS_SCOPEID;
    String dPSPreSharedKey = USE_MQTT_AZURE_DPS_PRESHAREDKEY;
    #if defined(USE_MQTT_AZURE_DPS_SCOPE_ENDPOINT)
      String endpoint=USE_MQTT_AZURE_DPS_SCOPE_ENDPOINT;
    #else
      String endpoint="https://global.azure-devices-provisioning.net/";
    #endif //USE_MQTT_AZURE_DPS_SCOPE_ENDPOINT

    String MACAddress = WiFi.macAddress();
    MACAddress.replace(":", "");

    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT "DPS register for %s, scope %s to %s."), MACAddress.c_str(), dPSScopeId.c_str(), endpoint.c_str());

    // derive our PSK from the DPS and set the device ID
    String devicePresharedKey = Sha256Sign(MACAddress, dPSPreSharedKey);
    char devicePresharedKeyChar[devicePresharedKey.length() + 1];
    devicePresharedKey.toCharArray(devicePresharedKeyChar, devicePresharedKey.length() + 1);

    // generate a SAS Token with this new derived key
    String dPSSASToken = AzureDSPPSKtoToken(dPSScopeId, MACAddress, devicePresharedKey.c_str());

    // REST to DPS to start the assigning process
    String dPSURL = endpoint + dPSScopeId + "/registrations/" + MACAddress + "/register?api-version=2019-03-31";
    String dPSPutContent = "{\"registrationId\": \"" + MACAddress + "\"}";

    httpsClient.setReuse(true);

    httpsClient.begin(*tlsHttpsClient, dPSURL);
    httpsClient.addHeader("User-Agent", "Tasmota");
    httpsClient.addHeader("Content-Type", "application/json");
    httpsClient.addHeader("Content-Encoding", "utf-8");
    httpsClient.addHeader("Authorization", dPSSASToken);
    httpsClientReturn = httpsClient.PUT(dPSPutContent);
    String dPSAssigningResponseJSON;

    if (httpsClientReturn == HTTP_CODE_ACCEPTED){
      dPSAssigningResponseJSON = httpsClient.getString();
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT "DPS Assigning response '%s'"), dPSAssigningResponseJSON.c_str());
    } else {
      dPSAssigningResponseJSON = httpsClient.getString();
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT "DPS Assigning response '%s'"), dPSAssigningResponseJSON.c_str());
      AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_MQTT "Azure DPS REST assignment connection failed with code '%d'.  Restarting."), httpsClientReturn);
      WebRestart(1);
    }

    if (dPSAssigningResponseJSON.indexOf("\"assigning\"") == -1){
      AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_MQTT "Azure DPS assignment failed with response '%s'.  Restarting."), dPSAssigningResponseJSON.c_str());
      WebRestart(1);
    } else {
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT "Azure DPS assignment response '%s'."), dPSAssigningResponseJSON.c_str());
    }

    httpsClient.end();

    JsonParser dPSAssigningResponseParser((char*) dPSAssigningResponseJSON.c_str());
    JsonParserObject dPSAssigningResponseRoot = dPSAssigningResponseParser.getRootObject();
    String dPSAssigningOperationId = dPSAssigningResponseRoot.getStr("operationId");

    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT "DPS operationId is '%s'."), dPSAssigningOperationId.c_str());

    bool assigned = false;
    int assignedCounter = 1;
    String dPSAssignedResponseJSON;
    dPSURL = endpoint + dPSScopeId + "/registrations/" + MACAddress + "/operations/" + dPSAssigningOperationId + "?api-version=2019-03-31";

    while (!assigned && assignedCounter < 5){
      httpsClient.begin(*tlsHttpsClient, dPSURL);
      httpsClient.addHeader("User-Agent", "Tasmota");
      httpsClient.addHeader("Content-Type", "application/json");
      httpsClient.addHeader("Content-Encoding", "utf-8");
      httpsClient.addHeader("Authorization", dPSSASToken);
      httpsClientReturn = httpsClient.GET();

      if (httpsClientReturn == HTTP_CODE_OK){
        dPSAssignedResponseJSON = httpsClient.getString();
      } else if (httpsClientReturn !=  HTTP_CODE_ACCEPTED){
        AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_MQTT "Azure DPS REST check connection failed with code '%d'."), httpsClientReturn);
      }

      if (dPSAssignedResponseJSON.indexOf("\"status\":\"assigned\"") > 0){
        assigned = true;
      } else if (httpsClientReturn !=  HTTP_CODE_ACCEPTED) {
        AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT "DPS try %d, response '%s'."), assignedCounter, dPSAssignedResponseJSON.c_str());
      }

      delay(1000 * assignedCounter);
      assignedCounter+=1;
    }

    httpsClient.end();

    if (assigned){
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT "Azure DPS registration response '%s'."), dPSAssignedResponseJSON.c_str());

      JsonParser parser((char*) dPSAssignedResponseJSON.c_str());
      JsonParserObject stateObject = parser.getRootObject()[PSTR("registrationState")].getObject();
      String deviceId = stateObject["deviceId"].getStr();
      String iotHub = stateObject["assignedHub"].getStr();

      bool newProvision = false;
      if (String(SettingsText(SET_MQTT_PWD)) != devicePresharedKey ||
          String(SettingsText(SET_MQTT_HOST)) != iotHub ||
          String(SettingsText(SET_MQTT_CLIENT)) != deviceId ||
          String(SettingsText(SET_MQTT_USER)) != deviceId) {

        newProvision = true;
        SettingsUpdateText(SET_MQTT_PWD, devicePresharedKey.c_str());
        SettingsUpdateText(SET_MQTT_HOST, iotHub.c_str());
        SettingsUpdateText(SET_MQTT_CLIENT, deviceId.c_str());
        SettingsUpdateText(SET_MQTT_USER, deviceId.c_str());
      }

      if (newProvision){  // because this is the first time we have been provisioned must reboot
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT "Azure DPS registration success, changed in DPS registration, restarting."));
        WebRestart(1);
      } else {
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT "Azure DPS registration success, no changes."));
      }

    } else {
      AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_MQTT "Azure DPS registration response failed with response '%s'."), dPSAssignedResponseJSON.c_str());
    }
  }
#endif // USE_MQTT_AZURE_DPS_SCOPEID
#endif // USE_MQTT_AZURE_IOT

bool MqttIsConnected(void) {
  return MqttClient.connected();
}

void MqttDisconnect(void) {
  MqttClient.disconnect();
}

void MqttSubscribeLib(const char *topic) {
#ifdef USE_MQTT_AZURE_IOT
  // Azure IoT Hub currently does not support custom topics: https://docs.microsoft.com/en-us/azure/iot-hub/iot-hub-mqtt-support
  String realTopicString = "devices/" + String(SettingsText(SET_MQTT_CLIENT));
  realTopicString += "/messages/devicebound/#";
  MqttClient.subscribe(realTopicString.c_str());
  SettingsUpdateText(SET_MQTT_FULLTOPIC, SettingsText(SET_MQTT_CLIENT));
  SettingsUpdateText(SET_MQTT_TOPIC, SettingsText(SET_MQTT_CLIENT));
#else
  MqttClient.subscribe(topic);
#endif  // USE_MQTT_AZURE_IOT
  MqttClient.loop();  // Solve LmacRxBlk:1 messages
}

void MqttUnsubscribeLib(const char *topic) {
  MqttClient.unsubscribe(topic);
  MqttClient.loop();  // Solve LmacRxBlk:1 messages
}

bool MqttPublishLib(const char* topic, const uint8_t* payload, unsigned int plength, bool retained) {
  // If Prefix1 equals Prefix2 disable next MQTT subscription to prevent loop
  if (!strcmp(SettingsText(SET_MQTTPREFIX1), SettingsText(SET_MQTTPREFIX2))) {
    char *str = strstr(topic, SettingsText(SET_MQTTPREFIX1));
    if (str == topic) {
      TasmotaGlobal.mqtt_cmnd_blocked_reset = 4;  // Allow up to four seconds before resetting residual cmnd blocks
      TasmotaGlobal.mqtt_cmnd_blocked++;
    }
  }

#ifdef USE_TASMESH
 if (MESHrouteMQTTtoMESH(topic, (char*)payload, retained)) {  // If we are a node, send this via ESP-Now
   yield();
   return true;
 }
#endif  // USE_TASMESH

#ifdef USE_MQTT_AZURE_IOT
  String sourceTopicString = urlEncodeBase64(String(topic));
  String topicString = "devices/" + String(SettingsText(SET_MQTT_CLIENT));
  topicString += "/messages/events/topic=" + sourceTopicString;

  JsonParser mqtt_message((char*) String((const char*)payload).c_str());
  JsonParserObject message_object = mqtt_message.getRootObject();
  if (!message_object.isValid()) {   // only sending valid JSON, yet this is optional
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT "Invalid JSON for topic '%s', not sending to Azure IoT Hub"), topic);
    return true;
  }
  topic = topicString.c_str();
#endif  // USE_MQTT_AZURE_IOT

  if (!MqttClient.beginPublish(topic, plength, retained)) {
//    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT "Connection lost or message too large"));
    return false;
  }
  uint32_t written = MqttClient.write(payload, plength);
  if (written != plength) {
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT "Message too large"));
    return false;
  }
  MqttClient.endPublish();

  yield();  // #3313
  return true;
}

void MqttDataHandler(char* mqtt_topic, uint8_t* mqtt_data, unsigned int data_len) {
  SHOW_FREE_MEM(PSTR("MqttDataHandler"));

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

#ifdef USE_MQTT_FILE
  FMqtt.topic_size = strlen(mqtt_topic);
#endif  // USE_MQTT_FILE

//  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT "BufferSize %d, Topic |%s|, Length %d, data_len %d"), MqttClient.getBufferSize(), mqtt_topic, strlen(mqtt_topic), data_len);

  char topic[TOPSZ];
#ifdef USE_MQTT_AZURE_IOT
  // for Azure, we read the topic from the property of the message
  String fullTopicString = String(mqtt_topic);
  String toppicUpper = fullTopicString;
  toppicUpper.toUpperCase();
  int startOfTopic = toppicUpper.indexOf("TOPIC=");
  if (startOfTopic == -1){
    AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_MQTT "Azure IoT message without the property topic."));
    return;
  }
  String newTopic = fullTopicString.substring(startOfTopic + 6);
  newTopic.replace("%2F", "/");
  if (newTopic.indexOf("/") == -1){
    AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_MQTT "Invalid Topic %s"), newTopic.c_str());
    return;
  }
  strlcpy(topic, newTopic.c_str(), sizeof(topic));
#else
  strlcpy(topic, mqtt_topic, sizeof(topic));
#endif  // USE_MQTT_AZURE_IOT
  mqtt_data[data_len] = 0;

  if (Mqtt.disable_logging) {
    TasmotaGlobal.masterlog_level = LOG_LEVEL_DEBUG_MORE;  // Hide logging
  }

#ifdef USE_TASMESH
#ifdef ESP32
  if (MESHinterceptMQTTonBroker(topic, (uint8_t*)mqtt_data, data_len +1)) {
    return;  // Check if this is a message for a node
  }
#endif  // ESP32
#endif  // USE_TASMESH

  // MQTT pre-processing
  XdrvMailbox.index = strlen(topic);
  XdrvMailbox.data_len = data_len;
  XdrvMailbox.topic = topic;
  XdrvMailbox.data = (char*)mqtt_data;
  if (XdrvCall(FUNC_MQTT_DATA)) { return; }

  ShowSource(SRC_MQTT);

  CommandHandler(topic, (char*)mqtt_data, data_len);

  if (Mqtt.disable_logging) {
    TasmotaGlobal.masterlog_level = LOG_LEVEL_NONE;  // Enable logging
  }
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

  if (!Settings->mqttlog_level || !Settings->flag.mqtt_enabled || !Mqtt.connected) { return; }  // SetOption3 - Enable MQTT
  if (refresh && !NeedLogRefresh(Settings->mqttlog_level, index)) { return; }

  char* line;
  size_t len;
  while (GetLog(Settings->mqttlog_level, &index, &line, &len)) {
    char stopic[TOPSZ];
    GetTopic_P(stopic, STAT, TasmotaGlobal.mqtt_topic, PSTR("LOGGING"));
    MqttPublishLib(stopic, (const uint8_t*)line, len -1, false);
  }
}

void MqttPublishPayload(const char* topic, const char* payload, uint32_t binary_length, bool retained) {
  // Publish <topic> payload string or binary when binary_length set with optional retained

  SHOW_FREE_MEM(PSTR("MqttPublish"));

  bool binary_data = (binary_length > 0);
  if (!binary_data) {
    binary_length = strlen(payload);
  }

  if (Settings->flag4.mqtt_no_retain) {                   // SetOption104 - Disable all MQTT retained messages, some brokers don't support it: AWS IoT, Losant
    retained = false;                                    // Some brokers don't support retained, they will disconnect if received
  }

  // To lower heap usage the payload is not copied to the heap but used directly
  String log_data_topic;                                 // 20210420 Moved to heap to solve tight stack resulting in exception 2
  if (Settings->flag.mqtt_enabled && MqttPublishLib(topic, (const uint8_t*)payload, binary_length, retained)) {  // SetOption3 - Enable MQTT
#ifdef USE_TASMESH
    log_data_topic = (MESHroleNode()) ? F("MSH: ") : F(D_LOG_MQTT);  // MSH: or MQT:
#else
    log_data_topic = F(D_LOG_MQTT);                      // MQT:
#endif  // USE_TASMESH
    log_data_topic += topic;                             // stat/tasmota/STATUS2
  } else {
    log_data_topic = F(D_LOG_RESULT);                    // RSL:
    char *command = strrchr(topic, '/');                 // If last part of topic it is always the command
    log_data_topic += (command == nullptr) ? topic : command +1;  // STATUS2
    retained = false;                                    // Without MQTT enabled there is no retained message
  }
  log_data_topic += F(" = ");                            // =
  char* log_data_payload = (char*)payload;
  String log_data_payload_b;
  if (binary_data) {
    log_data_payload_b = HexToString((uint8_t*)payload, binary_length);
    log_data_payload = (char*)log_data_payload_b.c_str();
  }
  char* log_data_retained = nullptr;
  String log_data_retained_b;
  if (retained) {
    log_data_retained_b = F(" (" D_RETAINED ")");        // (retained)
    log_data_retained = (char*)log_data_retained_b.c_str();
  }
  AddLogData(LOG_LEVEL_INFO, log_data_topic.c_str(), log_data_payload, log_data_retained);  // MQT: stat/tasmota/STATUS2 = {"StatusFWR":{"Version":...

  if (Settings->ledstate &0x04) {
    TasmotaGlobal.blinks++;
  }
}

void MqttPublishPayload(const char* topic, const char* payload) {
  // Publish <topic> payload string no retained
  MqttPublishPayload(topic, payload, 0, false);
}

void MqttPublish(const char* topic, bool retained) {
  // Publish <topic> default ResponseData string with optional retained
  MqttPublishPayload(topic, ResponseData(), 0, retained);
}

void MqttPublish(const char* topic) {
  // Publish <topic> default ResponseData string no retained
  MqttPublish(topic, false);
}

void MqttPublishPayloadPrefixTopic_P(uint32_t prefix, const char* subtopic, const char* payload, uint32_t binary_length, bool retained) {
/*
  Publish <prefix>/<device>/<RESULT or <subtopic>> payload string or binary when binary_length set with optional retained

  prefix 0 = cmnd using subtopic
  prefix 1 = stat using subtopic
  prefix 2 = tele using subtopic
  prefix 4 = cmnd using subtopic or RESULT
  prefix 5 = stat using subtopic or RESULT
  prefix 6 = tele using subtopic or RESULT
*/
  char romram[64];
  snprintf_P(romram, sizeof(romram), ((prefix > 3) && !Settings->flag.mqtt_response) ? S_RSLT_RESULT : subtopic);  // SetOption4 - Switch between MQTT RESULT or COMMAND
  UpperCase(romram, romram);

  prefix &= 3;
  char stopic[TOPSZ];
  GetTopic_P(stopic, prefix, TasmotaGlobal.mqtt_topic, romram);
  MqttPublishPayload(stopic, payload, binary_length, retained);

#if defined(USE_MQTT_AWS_IOT) || defined(USE_MQTT_AWS_IOT_LIGHT)
  if ((prefix > 0) && (Settings->flag4.awsiot_shadow) && (Mqtt.connected)) {    // placeholder for SetOptionXX
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
    String aws_payload = F("{\"state\":{\"reported\":%s}}");
    aws_payload += payload;

    MqttClient.publish(romram, aws_payload.c_str(), false);

    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT "Updated shadow: %s"), romram);
    yield();  // #3313
  }
#endif // USE_MQTT_AWS_IOT
}

void MqttPublishPayloadPrefixTopic_P(uint32_t prefix, const char* subtopic, const char* payload, uint32_t binary_length) {
  // Publish <prefix>/<device>/<RESULT or <subtopic>> payload string or binary when binary_length set no retained
  MqttPublishPayloadPrefixTopic_P(prefix, subtopic, payload, binary_length, false);
}

void MqttPublishPayloadPrefixTopic_P(uint32_t prefix, const char* subtopic, const char* payload) {
  // Publish <prefix>/<device>/<RESULT or <subtopic>> payload string no retained
  MqttPublishPayloadPrefixTopic_P(prefix, subtopic, payload, 0, false);
}

void MqttPublishPayloadPrefixTopicRulesProcess_P(uint32_t prefix, const char* subtopic, const char* payload, bool retained) {
  // Publish <prefix>/<device>/<RESULT or <subtopic>> payload string with optional retained
  //   then process rules
  MqttPublishPayloadPrefixTopic_P(prefix, subtopic, payload, 0, retained);
  XdrvRulesProcess(0, payload);
}

void MqttPublishPayloadPrefixTopicRulesProcess_P(uint32_t prefix, const char* subtopic, const char* payload) {
  // Publish <prefix>/<device>/<RESULT or <subtopic>> default ResponseData string no retained
  //   then process rules
  MqttPublishPayloadPrefixTopicRulesProcess_P(prefix, subtopic, payload, false);
}

void MqttPublishPrefixTopic_P(uint32_t prefix, const char* subtopic, bool retained) {
  // Publish <prefix>/<device>/<RESULT or <subtopic>> default ResponseData string with optional retained
  MqttPublishPayloadPrefixTopic_P(prefix, subtopic, ResponseData(), 0, retained);
}

void MqttPublishPrefixTopic_P(uint32_t prefix, const char* subtopic) {
  // Publish <prefix>/<device>/<RESULT or <subtopic>> default ResponseData string no retained
  MqttPublishPrefixTopic_P(prefix, subtopic, false);
}

void MqttPublishPrefixTopicRulesProcess_P(uint32_t prefix, const char* subtopic, bool retained) {
  // Publish <prefix>/<device>/<RESULT or <subtopic>> default ResponseData string with optional retained
  //   then process rules
  MqttPublishPrefixTopic_P(prefix, subtopic, retained);
  XdrvRulesProcess(0);
}

void MqttPublishPrefixTopicRulesProcess_P(uint32_t prefix, const char* subtopic) {
  // Publish <prefix>/<device>/<RESULT or <subtopic>> default ResponseData string no retained
  //   then process rules
  MqttPublishPrefixTopicRulesProcess_P(prefix, subtopic, false);
}

void MqttPublishTeleSensor(void) {
  // Publish tele/<device>/SENSOR default ResponseData string with optional retained
  //   then process rules
  MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR(D_RSLT_SENSOR), Settings->flag.mqtt_sensor_retain);  // CMND_SENSORRETAIN
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
      GetTopic_P(stopic, STAT, TasmotaGlobal.mqtt_topic, (Settings->flag.mqtt_response) ? scommand : S_RSLT_RESULT);  // SetOption4 - Switch between MQTT RESULT or COMMAND
      Response_P(S_JSON_COMMAND_NVALUE, scommand, GetFanspeed());
      MqttPublish(stopic);
    }
  } else {
#endif  // USE_SONOFF_IFAN
    GetPowerDevice(scommand, device, sizeof(scommand), Settings->flag.device_index_enable);           // SetOption26 - Switch between POWER or POWER1
    GetTopic_P(stopic, STAT, TasmotaGlobal.mqtt_topic, (Settings->flag.mqtt_response) ? scommand : S_RSLT_RESULT);  // SetOption4 - Switch between MQTT RESULT or COMMAND
    Response_P(S_JSON_COMMAND_SVALUE, scommand, GetStateText(bitRead(TasmotaGlobal.power, device -1)));
    MqttPublish(stopic);

    if (!Settings->flag4.only_json_message) {  // SetOption90 - Disable non-json MQTT response
      GetTopic_P(stopic, STAT, TasmotaGlobal.mqtt_topic, scommand);
      Response_P(GetStateText(bitRead(TasmotaGlobal.power, device -1)));
      MqttPublish(stopic, Settings->flag.mqtt_power_retain);  // CMND_POWERRETAIN
    }

#ifdef USE_INFLUXDB
    InfluxDbPublishPowerState(device);
#endif

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
    GetPowerDevice(scommand, device, sizeof(scommand), Settings->flag.device_index_enable), GetStateText(bitRead(TasmotaGlobal.blink_mask, device -1)));  // SetOption26 - Switch between POWER or POWER1

  MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, S_RSLT_POWER);
}

/*********************************************************************************************/

uint16_t MqttConnectCount(void) {
  return Mqtt.connect_count;
}

void MqttDisconnected(int state) {
  Mqtt.connected = false;

  Mqtt.retry_counter = Settings->mqtt_retry * Mqtt.retry_counter_delay;
  if ((Settings->mqtt_retry * Mqtt.retry_counter_delay) < 120) {
    Mqtt.retry_counter_delay++;
  }

  MqttClient.disconnect();
  // Check if this solves intermittent MQTT re-connection failures when broker is restarted
  EspClient.stop();

  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_CONNECT_FAILED_TO " %s:%d, rc %d. " D_RETRY_IN " %d " D_UNIT_SECOND),
    SettingsText(SET_MQTT_HOST), Settings->mqtt_port, state, Mqtt.retry_counter);
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

    if (!Settings->flag4.only_json_message) {  // SetOption90 - Disable non-json MQTT response
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
      Response_P(PSTR("{\"Info1\":{\"" D_CMND_MODULE "\":\"%s\",\"" D_JSON_VERSION "\":\"%s%s\",\"" D_JSON_FALLBACKTOPIC "\":\"%s\",\"" D_CMND_GROUPTOPIC "\":\"%s\"}}"),
        ModuleName().c_str(), TasmotaGlobal.version, TasmotaGlobal.image_name, GetFallbackTopic_P(stopic, ""), GetGroupTopic_P(stopic2, "", SET_MQTT_GRP_TOPIC));
      MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR(D_RSLT_INFO "1"), Settings->flag5.mqtt_info_retain);
#ifdef USE_WEBSERVER
      if (Settings->webserver) {
#if LWIP_IPV6
        Response_P(PSTR("{\"Info2\":{\"" D_JSON_WEBSERVER_MODE "\":\"%s\",\"" D_CMND_HOSTNAME "\":\"%s\",\"" D_CMND_IPADDRESS "\":\"%s\",\"IPv6Address\":\"%s\"}}"),
          (2 == Settings->webserver) ? PSTR(D_ADMIN) : PSTR(D_USER), NetworkHostname(), NetworkAddress().toString().c_str(), WifiGetIPv6().c_str(), Settings->flag5.mqtt_info_retain);
#else
        Response_P(PSTR("{\"Info2\":{\"" D_JSON_WEBSERVER_MODE "\":\"%s\",\"" D_CMND_HOSTNAME "\":\"%s\",\"" D_CMND_IPADDRESS "\":\"%s\"}}"),
          (2 == Settings->webserver) ? PSTR(D_ADMIN) : PSTR(D_USER), NetworkHostname(), NetworkAddress().toString().c_str(), Settings->flag5.mqtt_info_retain);
#endif // LWIP_IPV6 = 1
        MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR(D_RSLT_INFO "2"), Settings->flag5.mqtt_info_retain);
      }
#endif  // USE_WEBSERVER
      Response_P(PSTR("{\"Info3\":{\"" D_JSON_RESTARTREASON "\":"));
      if (CrashFlag()) {
        CrashDump();
      } else {
        ResponseAppend_P(PSTR("\"%s\""), GetResetReason().c_str());
      }
      ResponseJsonEndEnd();
      MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR(D_RSLT_INFO "3"), Settings->flag5.mqtt_info_retain);
    }

    MqttPublishAllPowerState();
    if (Settings->tele_period) {
      TasmotaGlobal.tele_period = Settings->tele_period -5;  // Enable TelePeriod in 5 seconds
    }
    TasmotaGlobal.rules_flag.system_boot = 1;
    XdrvCall(FUNC_MQTT_INIT);
  }
  Mqtt.initial_connection_state = 0;

  TasmotaGlobal.global_state.mqtt_down = 0;
  if (Settings->flag.mqtt_enabled) {  // SetOption3 - Enable MQTT
    TasmotaGlobal.rules_flag.mqtt_connected = 1;
  }
}

void MqttReconnect(void) {
  char stopic[TOPSZ];

  Mqtt.allowed = Settings->flag.mqtt_enabled && (TasmotaGlobal.restart_flag == 0);  // SetOption3 - Enable MQTT, and don't connect if restart in process
  if (Mqtt.allowed) {
#if defined(USE_MQTT_AZURE_DPS_SCOPEID) && defined(USE_MQTT_AZURE_DPS_PRESHAREDKEY)
  ProvisionAzureDPS();
#endif
#ifdef USE_DISCOVERY
#ifdef MQTT_HOST_DISCOVERY
    MqttDiscoverServer();
#endif  // MQTT_HOST_DISCOVERY
#endif  // USE_DISCOVERY
    if (!strlen(SettingsText(SET_MQTT_HOST)) || !Settings->mqtt_port) {
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
  Mqtt.retry_counter = Settings->mqtt_retry * Mqtt.retry_counter_delay;
  TasmotaGlobal.global_state.mqtt_down = 1;

#ifdef FIRMWARE_MINIMAL
#ifndef USE_MQTT_TLS
  // Don't try to connect if MQTT requires TLS but TLS is not supported
  if (Settings->flag4.mqtt_tls) {
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
  EspClient.setTimeout(Settings->mqtt_wifi_timeout * 100);
#ifdef USE_MQTT_TLS
  if (Mqtt.mqtt_tls) {
    tlsClient->stop();
  } else {
//    EspClient = WiFiClient();                // Wifi Client reconnect issue 4497 (https://github.com/esp8266/Arduino/issues/4497)
    MqttClient.setClient(EspClient);
  }
#else
//  EspClient = WiFiClient();                  // Wifi Client reconnect issue 4497 (https://github.com/esp8266/Arduino/issues/4497)
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
  MqttClient.setServer(SettingsText(SET_MQTT_HOST), Settings->mqtt_port);

  uint32_t mqtt_connect_time = millis();
#if defined(USE_MQTT_TLS)
  bool allow_all_fingerprints = false;
  bool learn_fingerprint1 = false;
  bool learn_fingerprint2 = false;
  if (Mqtt.mqtt_tls && Settings->flag5.tls_use_fingerprint) {
    allow_all_fingerprints = false;
    learn_fingerprint1 = is_fingerprint_mono_value(Settings->mqtt_fingerprint[0], 0x00);
    learn_fingerprint2 = is_fingerprint_mono_value(Settings->mqtt_fingerprint[1], 0x00);
    allow_all_fingerprints |= is_fingerprint_mono_value(Settings->mqtt_fingerprint[0], 0xff);
    allow_all_fingerprints |= is_fingerprint_mono_value(Settings->mqtt_fingerprint[1], 0xff);
    allow_all_fingerprints |= learn_fingerprint1;
    allow_all_fingerprints |= learn_fingerprint2;
    tlsClient->setPubKeyFingerprint(Settings->mqtt_fingerprint[0], Settings->mqtt_fingerprint[1], allow_all_fingerprints);
  }
#endif
  bool lwt_retain = Settings->flag4.mqtt_no_retain ? false : true;   // no retained last will if "no_retain"
#if defined(USE_MQTT_TLS) && defined(USE_MQTT_AWS_IOT)
  if (Mqtt.mqtt_tls) {
    if ((nullptr != AWS_IoT_Private_Key) && (nullptr != AWS_IoT_Client_Certificate)) {
      // if private key is there, we remove user/pwd
      mqtt_user = nullptr;
      mqtt_pwd  = nullptr;
    }
  }
#endif

#ifdef USE_MQTT_AZURE_IOT
  String azureMqtt_password = SettingsText(SET_MQTT_PWD);
  if (azureMqtt_password.indexOf("SharedAccessSignature") == -1) {
    // assuming a PreSharedKey was provided, calculating a SAS Token into azureMqtt_password
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT "Authenticating with an Azure IoT Hub Token"));
    azureMqtt_password = AzurePSKtoToken(SettingsText(SET_MQTT_HOST), SettingsText(SET_MQTT_CLIENT), SettingsText(SET_MQTT_PWD));
  }

  String azureMqtt_userString = String(SettingsText(SET_MQTT_HOST)) + "/" + String(SettingsText(SET_MQTT_CLIENT)); + "/?api-version=2018-06-30";
  if (MqttClient.connect(TasmotaGlobal.mqtt_client, azureMqtt_userString.c_str(), azureMqtt_password.c_str(), stopic, 1, lwt_retain, ResponseData(), MQTT_CLEAN_SESSION)) {
#else
  if (MqttClient.connect(TasmotaGlobal.mqtt_client, mqtt_user, mqtt_pwd, stopic, 1, lwt_retain, ResponseData(), MQTT_CLEAN_SESSION)) {
#endif  // USE_MQTT_AZURE_IOT
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

      if (Settings->flag5.tls_use_fingerprint) {    // CA validation
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
        if (recv_fingerprint[20] & 0x1 || (learn_fingerprint1 && 0 != memcmp(recv_fingerprint, Settings->mqtt_fingerprint[1], 20))) {
          memcpy(Settings->mqtt_fingerprint[0], recv_fingerprint, 20);
          learned = true;
        }
        // As above, but for the other slot.
        if (recv_fingerprint[20] & 0x2 || (learn_fingerprint2 && 0 != memcmp(recv_fingerprint, Settings->mqtt_fingerprint[0], 20))) {
          memcpy(Settings->mqtt_fingerprint[1], recv_fingerprint, 20);
          learned = true;
        }

        if (learned) {
          AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT "Fingerprint learned: %s"), buf_fingerprint);

          SettingsSaveAll();  // save settings
        }
      }

    }
#endif // USE_MQTT_TLS
    MqttConnected();
  } else {
#ifdef USE_MQTT_TLS
    if (Mqtt.mqtt_tls) {
/*
      getLastError codes as documented in lib\lib_ssl\bearssl-esp8266\src\t_bearssl_ssl.h
      SSL-level error codes
      |    Receive Fatal Alert
      |    |     Send Fatal Alert
      |    |     |
       0 : 256 : 512 : BR_ERR_OK
       1 : 257 : 513 : BR_ERR_BAD_PARAM           - caller-provided parameter is incorrect
       2 : 258 : 514 : BR_ERR_BAD_STATE           - operation requested by the caller cannot be applied with the current context state (e.g. reading data while outgoing data is waiting to be sent)
       3 : 259 : 515 : BR_ERR_UNSUPPORTED_VERSION - incoming protocol or record version is unsupported
       4 : 260 : 516 : BR_ERR_BAD_VERSION         - incoming record version does not match the expected version
       5 : 261 : 517 : BR_ERR_BAD_LENGTH          - incoming record length is invalid
       6 : 262 : 518 : BR_ERR_TOO_LARGE           - incoming record is too large to be processed, or buffer is too small for the handshake message to send
       7 : 263 : 519 : BR_ERR_BAD_MAC             - decryption found an invalid padding, or the record MAC is not correct
       8 : 264 : 520 : BR_ERR_NO_RANDOM           - no initial entropy was provided, and none can be obtained from the OS
       9 : 265 : 521 : BR_ERR_UNKNOWN_TYPE        - incoming record type is unknown
      10 : 266 : 522 : BR_ERR_UNEXPECTED          - incoming record or message has wrong type with regards to the current engine state
      12 : 268 : 524 : BR_ERR_BAD_CCS             - ChangeCipherSpec message from the peer has invalid contents
      13 : 269 : 525 : BR_ERR_BAD_ALERT           - alert message from the peer has invalid contents (odd length)
      14 : 270 : 526 : BR_ERR_BAD_HANDSHAKE       - incoming handshake message decoding failed
      15 : 271 : 527 : BR_ERR_OVERSIZED_ID        - ServerHello contains a session ID which is larger than 32 bytes
      16 : 272 : 528 : BR_ERR_BAD_CIPHER_SUITE    - server wants to use a cipher suite that we did not claim to support. This is also reported if we tried to advertise a cipher suite that we do not support
      17 : 273 : 529 : BR_ERR_BAD_COMPRESSION     - server wants to use a compression that we did not claim to support
      18 : 274 : 530 : BR_ERR_BAD_FRAGLEN         - server's max fragment length does not match client's
      19 : 275 : 531 : BR_ERR_BAD_SECRENEG        - secure renegotiation failed
      20 : 276 : 532 : BR_ERR_EXTRA_EXTENSION     - server sent an extension type that we did not announce, or used the same extension type several times in a single ServerHello
      21 : 277 : 533 : BR_ERR_BAD_SNI             - invalid Server Name Indication contents (when used by the server, this extension shall be empty)
      22 : 278 : 534 : BR_ERR_BAD_HELLO_DONE      - invalid ServerHelloDone from the server (length is not 0)
      23 : 279 : 535 : BR_ERR_LIMIT_EXCEEDED      - internal limit exceeded (e.g. server's public key is too large)
      24 : 280 : 536 : BR_ERR_BAD_FINISHED        - Finished message from peer does not match the expected value
      25 : 281 : 537 : BR_ERR_RESUME_MISMATCH     - session resumption attempt with distinct version or cipher suite
      26 : 282 : 538 : BR_ERR_INVALID_ALGORITHM   - unsupported or invalid algorithm (ECDHE curve, signature algorithm, hash function)
      27 : 283 : 539 : BR_ERR_BAD_SIGNATURE       - invalid signature (on ServerKeyExchange from server, or in CertificateVerify from client)
      28 : 284 : 540 : BR_ERR_WRONG_KEY_USAGE     - peer's public key does not have the proper type or is not allowed for requested operation
      29 : 285 : 541 : BR_ERR_NO_CLIENT_AUTH      - client did not send a certificate upon request, or the client certificate could not be validated
      31 : 287 : 543 : BR_ERR_IO                  - I/O error or premature close on underlying transport stream. This error code is set only by the simplified I/O API ("br_sslio_*")

      getLastError codes as documented in lib\lib_ssl\bearssl-esp8266\src\t_bearssl_x509.h
      32 : BR_ERR_X509_OK                  - validation was successful; this is not actually an error
      33 : BR_ERR_X509_INVALID_VALUE       - invalid value in an ASN.1 structure
      34 : BR_ERR_X509_TRUNCATED           - truncated certificate
      35 : BR_ERR_X509_EMPTY_CHAIN         - empty certificate chain (no certificate at all)
      36 : BR_ERR_X509_INNER_TRUNC         - decoding error: inner element extends beyond outer element size
      37 : BR_ERR_X509_BAD_TAG_CLASS       - decoding error: unsupported tag class (application or private)
      38 : BR_ERR_X509_BAD_TAG_VALUE       - decoding error: unsupported tag value
      39 : BR_ERR_X509_INDEFINITE_LENGTH   - decoding error: indefinite length
      40 : BR_ERR_X509_EXTRA_ELEMENT       - decoding error: extraneous element
      41 : BR_ERR_X509_UNEXPECTED          - decoding error: unexpected element
      42 : BR_ERR_X509_NOT_CONSTRUCTED     - decoding error: expected constructed element, but is primitive
      43 : BR_ERR_X509_NOT_PRIMITIVE       - decoding error: expected primitive element, but is constructed
      44 : BR_ERR_X509_PARTIAL_BYTE        - decoding error: BIT STRING length is not multiple of 8
      45 : BR_ERR_X509_BAD_BOOLEAN         - decoding error: BOOLEAN value has invalid length
      46 : BR_ERR_X509_OVERFLOW            - decoding error: value is off-limits
      47 : BR_ERR_X509_BAD_DN              - invalid distinguished name
      48 : BR_ERR_X509_BAD_TIME            - invalid date/time representation
      49 : BR_ERR_X509_UNSUPPORTED         - certificate contains unsupported features that cannot be ignored
      50 : BR_ERR_X509_LIMIT_EXCEEDED      - key or signature size exceeds internal limits
      51 : BR_ERR_X509_WRONG_KEY_TYPE      - key type does not match that which was expected
      52 : BR_ERR_X509_BAD_SIGNATURE       - signature is invalid
      53 : BR_ERR_X509_TIME_UNKNOWN        - validation time is unknown
      54 : BR_ERR_X509_EXPIRED             - certificate is expired or not yet valid
      55 : BR_ERR_X509_DN_MISMATCH         - issuer/subject DN mismatch in the chain
      56 : BR_ERR_X509_BAD_SERVER_NAME     - expected server name was not found in the chain
      57 : BR_ERR_X509_CRITICAL_EXTENSION  - unknown critical extension in certificate
      58 : BR_ERR_X509_NOT_CA              - not a CA, or path length constraint violation
      59 : BR_ERR_X509_FORBIDDEN_KEY_USAGE - Key Usage extension prohibits intended usage
      60 : BR_ERR_X509_WEAK_PUBLIC_KEY     - public key found in certificate is too small
      62 : BR_ERR_X509_NOT_TRUSTED         - chain could not be linked to a trust anchor

      getLastError codes as documented in lib\lib_ssl\bearssl-esp8266\src\t_bearssl_ssl.h
       10 : 266 : BR_ALERT_UNEXPECTED_MESSAGE
       20 : 276 : BR_ALERT_BAD_RECORD_MAC
       22 : 278 : BR_ALERT_RECORD_OVERFLOW
       30 : 286 : BR_ALERT_DECOMPRESSION_FAILURE
       40 : 296 : BR_ALERT_HANDSHAKE_FAILURE
       42 : 298 : BR_ALERT_BAD_CERTIFICATE
       43 : 299 : BR_ALERT_UNSUPPORTED_CERTIFICATE
       44 : 300 : BR_ALERT_CERTIFICATE_REVOKED
       45 : 301 : BR_ALERT_CERTIFICATE_EXPIRED
       46 : 302 : BR_ALERT_CERTIFICATE_UNKNOWN
       47 : 303 : BR_ALERT_ILLEGAL_PARAMETER
       48 : 304 : BR_ALERT_UNKNOWN_CA
       49 : 305 : BR_ALERT_ACCESS_DENIED
       50 : 306 : BR_ALERT_DECODE_ERROR
       51 : 307 : BR_ALERT_DECRYPT_ERROR
       70 : 326 : BR_ALERT_PROTOCOL_VERSION
       71 : 327 : BR_ALERT_INSUFFICIENT_SECURITY
       80 : 336 : BR_ALERT_INTERNAL_ERROR
       90 : 346 : BR_ALERT_USER_CANCELED
      100 : 356 : BR_ALERT_NO_RENEGOTIATION
      110 : 366 : BR_ALERT_UNSUPPORTED_EXTENSION
      120 : 376 : BR_ALERT_NO_APPLICATION_PROTOCOL
*/
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT "TLS connection error: %d"), tlsClient->getLastError());
    }
#endif
/*
    State codes as documented here http://pubsubclient.knolleary.net/api.html#state
    -4 : MQTT_CONNECTION_TIMEOUT      - the server didn't respond within the keepalive time
    -3 : MQTT_CONNECTION_LOST         - the network connection was broken
    -2 : MQTT_CONNECT_FAILED          - the network connection failed
    -1 : MQTT_DISCONNECTED            - the client is disconnected cleanly
     0 : MQTT_CONNECTED               - the client is connected
     1 : MQTT_CONNECT_BAD_PROTOCOL    - the server doesn't support the requested version of MQTT
     2 : MQTT_CONNECT_BAD_CLIENT_ID   - the server rejected the client identifier
     3 : MQTT_CONNECT_UNAVAILABLE     - the server was unable to accept the connection
     4 : MQTT_CONNECT_BAD_CREDENTIALS - the username/password were rejected
     5 : MQTT_CONNECT_UNAUTHORIZED    - the client was not authorized to connect
*/
    MqttDisconnected(MqttClient.state());
  }
}

void MqttCheck(void) {
  if (Settings->flag.mqtt_enabled) {  // SetOption3 - Enable MQTT
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

#if defined(USE_MQTT_TLS)
void CmndMqttFingerprint(void) {
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 2)) {
    char fingerprint[60];
    if ((XdrvMailbox.data_len > 0) && (XdrvMailbox.data_len < sizeof(fingerprint))) {
      if (SC_DEFAULT == Shortcut()) {
        memcpy_P(Settings->mqtt_fingerprint[XdrvMailbox.index -1], (1 == XdrvMailbox.index) ? default_fingerprint1 : default_fingerprint2, sizeof(default_fingerprint1));
      } else {
        strlcpy(fingerprint, (SC_CLEAR == Shortcut()) ? "" : XdrvMailbox.data, sizeof(fingerprint));
        char *p = fingerprint;
        for (uint32_t i = 0; i < 20; i++) {
          Settings->mqtt_fingerprint[XdrvMailbox.index -1][i] = strtol(p, &p, 16);
        }
      }
      TasmotaGlobal.restart_flag = 2;
    }
    ResponseCmndIdxChar(ToHex_P((unsigned char *)Settings->mqtt_fingerprint[XdrvMailbox.index -1], 20, fingerprint, sizeof(fingerprint), ' '));
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
  bool show_asterisk = (2 == XdrvMailbox.index);
  if (XdrvMailbox.data_len > 0) {
    SettingsUpdateText(SET_MQTT_PWD, (SC_CLEAR == Shortcut()) ? "" : (SC_DEFAULT == Shortcut()) ? PSTR(MQTT_PASS) : XdrvMailbox.data);
    if (!show_asterisk) {
      ResponseCmndChar(SettingsText(SET_MQTT_PWD));
    }
    TasmotaGlobal.restart_flag = 2;
  } else {
    show_asterisk = true;
  }
  if (show_asterisk) {
    Response_P(S_JSON_COMMAND_ASTERISK, XdrvMailbox.command);
  }
}

void CmndMqttKeepAlive(void) {
  if ((XdrvMailbox.payload >= 1) && (XdrvMailbox.payload <= 100)) {
    Settings->mqtt_keepalive = XdrvMailbox.payload;
#ifdef USE_MQTT_NEW_PUBSUBCLIENT
    MqttClient.setKeepAlive(Settings->mqtt_keepalive);
#endif
  }
  ResponseCmndNumber(Settings->mqtt_keepalive);
}

void CmndMqttTimeout(void) {
  // Set timeout between 1 and 100 seconds
  if ((XdrvMailbox.payload >= 1) && (XdrvMailbox.payload <= 100)) {
    Settings->mqtt_socket_timeout = XdrvMailbox.payload;
#ifdef USE_MQTT_NEW_PUBSUBCLIENT
    MqttClient.setSocketTimeout(Settings->mqtt_socket_timeout);
#endif
  }
  ResponseCmndNumber(Settings->mqtt_socket_timeout);
}

void CmndMqttWifiTimeout(void) {
  // Set timeout between 100 and 20000 mSec
  if ((XdrvMailbox.payload >= 100) && (XdrvMailbox.payload <= 20000)) {
    Settings->mqtt_wifi_timeout = XdrvMailbox.payload / 100;
    EspClient.setTimeout(Settings->mqtt_wifi_timeout * 100);
  }
  ResponseCmndNumber(Settings->mqtt_wifi_timeout * 100);
}

void CmndMqttlog(void) {
  if ((XdrvMailbox.payload >= LOG_LEVEL_NONE) && (XdrvMailbox.payload <= LOG_LEVEL_DEBUG_MORE)) {
    Settings->mqttlog_level = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->mqttlog_level);
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
    Settings->mqtt_port = (1 == XdrvMailbox.payload) ? MQTT_PORT : XdrvMailbox.payload;
    TasmotaGlobal.restart_flag = 2;
  }
  ResponseCmndNumber(Settings->mqtt_port);
}

void CmndMqttRetry(void) {
  if ((XdrvMailbox.payload >= MQTT_RETRY_SECS) && (XdrvMailbox.payload < 32001)) {
    Settings->mqtt_retry = XdrvMailbox.payload;
    Mqtt.retry_counter = Settings->mqtt_retry;
  }
  ResponseCmndNumber(Settings->mqtt_retry);
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
      Response_P((Settings->flag.mqtt_offline) ? S_LWT_OFFLINE : "");  // SetOption10 - Control MQTT LWT message format
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
        Response_P(payload_part);
      } else {
        ResponseClear();
      }
      MqttPublish(stemp1, (XdrvMailbox.index == 2));
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
      Response_P((Settings->flag.mqtt_offline) ? S_LWT_OFFLINE : "");  // SetOption10 - Control MQTT LWT message format
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
    Settings->flag.mqtt_button_retain = XdrvMailbox.payload;  // CMND_BUTTONRETAIN
  }
  ResponseCmndStateText(Settings->flag.mqtt_button_retain);   // CMND_BUTTONRETAIN
}

void CmndSwitchRetain(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    if (!XdrvMailbox.payload) {
      for (uint32_t i = 1; i <= MAX_SWITCHES; i++) {
        SendKey(KEY_SWITCH, i, CLEAR_RETAIN);  // Clear MQTT retain in broker
      }
    }
    Settings->flag.mqtt_switch_retain = XdrvMailbox.payload;  // CMND_SWITCHRETAIN
  }
  ResponseCmndStateText(Settings->flag.mqtt_switch_retain);   // CMND_SWITCHRETAIN
}

void CmndPowerRetain(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    if (!XdrvMailbox.payload) {
      char stemp1[TOPSZ];
      char scommand[CMDSZ];
      for (uint32_t i = 1; i <= TasmotaGlobal.devices_present; i++) {  // Clear MQTT retain in broker
        GetTopic_P(stemp1, STAT, TasmotaGlobal.mqtt_topic, GetPowerDevice(scommand, i, sizeof(scommand), Settings->flag.device_index_enable));  // SetOption26 - Switch between POWER or POWER1
        ResponseClear();
        MqttPublish(stemp1, Settings->flag.mqtt_power_retain);  // CMND_POWERRETAIN
      }
    }
    Settings->flag.mqtt_power_retain = XdrvMailbox.payload;     // CMND_POWERRETAIN
    if (Settings->flag.mqtt_power_retain) {
      Settings->flag4.only_json_message = 0;                    // SetOption90 - Disable non-json MQTT response
    }
  }
  ResponseCmndStateText(Settings->flag.mqtt_power_retain);      // CMND_POWERRETAIN
}

void CmndSensorRetain(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    if (!XdrvMailbox.payload) {
      ResponseClear();
      MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings->flag.mqtt_sensor_retain);  // CMND_SENSORRETAIN
      MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_ENERGY), Settings->flag.mqtt_sensor_retain);  // CMND_SENSORRETAIN
    }
    Settings->flag.mqtt_sensor_retain = XdrvMailbox.payload;                                   // CMND_SENSORRETAIN
  }
  ResponseCmndStateText(Settings->flag.mqtt_sensor_retain);                                    // CMND_SENSORRETAIN
}

void CmndInfoRetain(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    if (!XdrvMailbox.payload) {
      ResponseClear();
      MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_INFO), Settings->flag5.mqtt_info_retain);  // CMND_INFORETAIN
    }
    Settings->flag5.mqtt_info_retain = XdrvMailbox.payload;                                   // CMND_INFORETAIN
  }
  ResponseCmndStateText(Settings->flag5.mqtt_info_retain);                                    // CMND_INFORETAIN
}

void CmndStateRetain(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    if (!XdrvMailbox.payload) {
      ResponseClear();
      MqttPublishPrefixTopic_P(STAT, PSTR(D_RSLT_STATE), Settings->flag5.mqtt_state_retain);  // CMND_STATERETAIN
    }
    Settings->flag5.mqtt_state_retain = XdrvMailbox.payload;                                   // CMND_STATERETAIN
  }
  ResponseCmndStateText(Settings->flag5.mqtt_state_retain);                                    // CMND_STATERETAIN
}

/*********************************************************************************************\
 * TLS private key and certificate - store into Flash
\*********************************************************************************************/
#if defined(USE_MQTT_TLS) && defined(USE_MQTT_AWS_IOT)

#ifdef ESP32
static uint8_t * tls_spi_start = nullptr;
const static size_t   tls_spi_len      = 0x0400;  // 1kb blocs
const static size_t   tls_block_offset = 0x0000;  // don't need offset in FS
#else
// const static uint16_t tls_spi_start_sector = EEPROM_LOCATION + 4;  // 0xXXFF
// const static uint8_t* tls_spi_start    = (uint8_t*) ((tls_spi_start_sector * SPI_FLASH_SEC_SIZE) + 0x40200000);  // 0x40XFF000
const static uint16_t tls_spi_start_sector = 0xFF;  // Force last bank of first MB
const static uint8_t* tls_spi_start    = (uint8_t*) 0x402FF000;  // 0x402FF000
const static size_t   tls_spi_len      = 0x1000;  // 4kb blocs
const static size_t   tls_block_offset = 0x0400;
#endif
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
#ifdef ESP32
  // We load the file in RAM and use it as if it was in Flash. The buffer is never deallocated once we loaded TLS keys
  AWS_IoT_Private_Key = nullptr;
  AWS_IoT_Client_Certificate = nullptr;
  if (TfsFileExists(TASM_FILE_TLSKEY)) {
    if (tls_spi_start == nullptr){
      tls_spi_start = (uint8_t*) malloc(tls_block_len);
      if (tls_spi_start == nullptr) {
        return;
      }
    }
    TfsLoadFile(TASM_FILE_TLSKEY, tls_spi_start, tls_block_len);
  } else {
    return;   // file does not exist, do nothing
  }
#endif
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
      if (tls_spi_start != nullptr) {  // safeguard for ESP32
        memcpy_P(spi_buffer, tls_spi_start, tls_spi_len);
      } else {
        memset(spi_buffer, 0, tls_spi_len);   // safeguard for ESP32, removed by compiler for ESP8266
      }

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

      bool save_file = false;   // for ESP32, do we need to write file
      if (1 == XdrvMailbox.index) {
        // Try to write Private key
        // Start by erasing all
#ifdef ESP32
        if (TfsFileExists(TASM_FILE_TLSKEY)) {
          TfsDeleteFile(TASM_FILE_TLSKEY);  // delete file
        }
#else
        TlsEraseBuffer(spi_buffer);   // Erase any previously stored data
#endif
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
          save_file = true;
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
        save_file = true;
      }

#ifdef ESP32
      if (save_file) {
        TfsSaveFile(TASM_FILE_TLSKEY, spi_buffer, tls_spi_len);
      }
#else
      if (ESP.flashEraseSector(tls_spi_start_sector)) {
        ESP.flashWrite(tls_spi_start_sector * SPI_FLASH_SEC_SIZE, (uint32_t*) spi_buffer, SPI_FLASH_SEC_SIZE);
      }
#endif
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
  if (tls_spi_start == nullptr) { return; }   // safeguard for ESP32, removed by compiler for ESP8266
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
  "<p><label><b>" D_PASSWORD "</b><input type='checkbox' onclick='sp(\"mp\")'></label><br><input id='mp' type='password' minlength='5' placeholder=\"" D_PASSWORD "\" value=\"" D_ASTERISK_PWD "\"></p>"
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
    Settings->mqtt_port,
#ifdef USE_MQTT_TLS
    Mqtt.mqtt_tls ? PSTR(" checked") : "",      // SetOption103 - Enable MQTT TLS
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

void MqttSaveSettings(void) {
  String cmnd = F(D_CMND_BACKLOG "0 ");
  cmnd += AddWebCommand(PSTR(D_CMND_MQTTHOST), PSTR("mh"), PSTR("1"));
  cmnd += AddWebCommand(PSTR(D_CMND_MQTTPORT), PSTR("ml"), PSTR("1"));
  cmnd += AddWebCommand(PSTR(D_CMND_MQTTCLIENT), PSTR("mc"), PSTR("1"));
  cmnd += AddWebCommand(PSTR(D_CMND_MQTTUSER), PSTR("mu"), PSTR("1"));
  cmnd += AddWebCommand(PSTR(D_CMND_MQTTPASSWORD "2"), PSTR("mp"), PSTR("\""));
  cmnd += AddWebCommand(PSTR(D_CMND_TOPIC), PSTR("mt"), PSTR("1"));
  cmnd += AddWebCommand(PSTR(D_CMND_FULLTOPIC), PSTR("mf"), PSTR("1"));
#ifdef USE_MQTT_TLS
  cmnd += F(";" D_CMND_SO "103 ");
  cmnd += Webserver->hasArg(F("b3"));  // SetOption103 - Enable MQTT TLS
#endif
  ExecuteWebCommand((char*)cmnd.c_str());
}
#endif  // USE_WEBSERVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv02(uint8_t function)
{
  bool result = false;

  if (Settings->flag.mqtt_enabled) {  // SetOption3 - Enable MQTT
    switch (function) {
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
      case FUNC_PRE_INIT:
        MqttInit();
        break;
    }
  }
  return result;
}
