/*
  xsns_90_influxdb.ino - Influxdb support for Tasmota

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

#ifdef USE_INFLUXDB
/*********************************************************************************************\
 * Influxdb support
 *
 * To save over 80k not supporting https this driver uses a subset of library
 * https://github.com/tobiasschuerg/InfluxDB-Client-for-Arduino
 *
 * The text format for metrics, labels and values is documented at
 * https://docs.influxdata.com/influxdb/v1.8/write_protocols/line_protocol_reference/
 *
 * Supported commands:
 * Ifx         - Show current state
 * IfxHost     - Set Influxdb host name or IP address
 * IfxPort     - Set Influxdb port
 * IfxDatabase - Set Influxdb v1 and database name
 * IfxUser     - Set Influxdb v1 and userid
 * IfxPassword - Set Influxdb v1 and password
 * IfxBucket   - Set Influxdb v2 and bucket name
 * IfxOrg      - Set Influxdb v2 and organization
 * IfxToken    - Set Influxdb v2 and token
 *
 * Set influxdb update interval with command teleperiod
\*********************************************************************************************/

#define XSNS_90            90

#define INFLUXDB_INITIAL   7             // Initial number of seconds after wifi connect keeping in mind sensor initialization

#ifndef INFLUXDB_VERSION
#define INFLUXDB_VERSION   1             // Version of Influxdb 1 or 2
#endif
#ifndef INFLUXDB_HOST
#define INFLUXDB_HOST      "influxdb"    // [IfxHost] Influxdb hostname or IP address
#endif
#ifndef INFLUXDB_PORT
#define INFLUXDB_PORT      8086          // [IfxPort] Influxdb port number
#endif
#ifndef INFLUXDB_ORG
#define INFLUXDB_ORG       ""            // [IfxUser, IfxOrg] Influxdb v1 username or v2 organisation
#endif
#ifndef INFLUXDB_TOKEN
#define INFLUXDB_TOKEN     ""            // [IfxPassword, IfxToken] Influxdb v1 password or v2 token
#endif
#ifndef INFLUXDB_BUCKET
#define INFLUXDB_BUCKET    "db"          // [IfxDatabase, IfxBucket] Influxdb v1 database or v2 bucket
#endif

static const char UninitializedMessage[] PROGMEM = "Unconfigured instance";
// This cannot be put to PROGMEM due to the way how it is used
static const char RetryAfter[] = "Retry-After";
static const char TransferEncoding[] = "Transfer-Encoding";

WiFiClient *IFDBwifiClient = nullptr;
HTTPClient *IFDBhttpClient = nullptr;

struct {
  String _serverUrl;                     // Connection info
  String _writeUrl;                      // Cached full write url
  String _lastErrorResponse;             // Server reponse or library error message for last failed request
  uint32_t _lastRequestTime = 0;         // Last time in ms we made are a request to server
  int interval = 0;
  int _lastStatusCode = 0;               // HTTP status code of last request to server
  int _lastRetryAfter = 0;               // Store retry timeout suggested by server after last request
  bool _connectionReuse;                 // true if HTTP connection should be kept open. Usable for frequent writes. Default false
  bool init = false;
} IFDB;

String InfluxDbAuth(void) {
  String auth = "";
  if (strlen(SettingsText(SET_INFLUXDB_ORG)) > 0 && strlen(SettingsText(SET_INFLUXDB_TOKEN)) > 0) {
    auth = "&u=";
    auth += UrlEncode(SettingsText(SET_INFLUXDB_ORG));
    auth += "&p=";
    auth += UrlEncode(SettingsText(SET_INFLUXDB_TOKEN));
  }
  return auth;
}

bool InfluxDbParameterInit(void) {
  if (strlen(SettingsText(SET_INFLUXDB_BUCKET)) == 0 ||
      (2 == Settings->influxdb_version && (strlen(SettingsText(SET_INFLUXDB_ORG)) == 0 ||
                                           strlen(SettingsText(SET_INFLUXDB_TOKEN)) == 0))) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("IFX: Invalid parameters"));
    return false;
  }
  IFDB._serverUrl = "http://";
  IFDB._serverUrl += SettingsText(SET_INFLUXDB_HOST);
  IFDB._serverUrl += ":";
  IFDB._serverUrl += Settings->influxdb_port;

  IFDB._writeUrl = IFDB._serverUrl;
  if (2 == Settings->influxdb_version) {
    IFDB._writeUrl += "/api/v2/write?org=";
    IFDB._writeUrl += UrlEncode(SettingsText(SET_INFLUXDB_ORG));
    IFDB._writeUrl += "&bucket=";
    IFDB._writeUrl += UrlEncode(SettingsText(SET_INFLUXDB_BUCKET));
  } else {
    IFDB._writeUrl += "/write?db=";
    IFDB._writeUrl += UrlEncode(SettingsText(SET_INFLUXDB_BUCKET));
    IFDB._writeUrl += InfluxDbAuth();
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR("IFX: Url %s"), IFDB._writeUrl.c_str());

  return true;
}

bool InfluxDbInit(void) {
  IFDBwifiClient = new WiFiClient;
  if (!IFDBhttpClient) {
    IFDBhttpClient = new HTTPClient;
  }
  IFDBhttpClient->setReuse(IFDB._connectionReuse);
  char server[32];
  snprintf_P(server, sizeof(server), PSTR("Tasmota/%s (%s)"), TasmotaGlobal.version, GetDeviceHardware().c_str());
  IFDBhttpClient->setUserAgent(server);
  return true;
}

void InfluxDbBeforeRequest() {
  if (strlen(SettingsText(SET_INFLUXDB_TOKEN)) > 0) {
    String auth_token = SettingsText(SET_INFLUXDB_TOKEN);
    IFDBhttpClient->addHeader(F("Authorization"), "Token " + auth_token);
  }
  const char *headerKeys[] = { RetryAfter, TransferEncoding };
  IFDBhttpClient->collectHeaders(headerKeys, 2);
}

void InfluxDbAfterRequest(int expectedStatusCode, bool modifyLastConnStatus) {
  if (modifyLastConnStatus) {
    IFDB._lastRequestTime = millis();
//    AddLog(LOG_LEVEL_DEBUG, PSTR("IFX: HTTP status code %d"), IFDB._lastStatusCode);
    IFDB._lastRetryAfter = 0;
    if (IFDB._lastStatusCode >= 429) { //retryable server errors
      if (IFDBhttpClient->hasHeader(RetryAfter)) {
        IFDB._lastRetryAfter = IFDBhttpClient->header(RetryAfter).toInt();
        AddLog(LOG_LEVEL_DEBUG, PSTR("IFX: Reply after %d"), IFDB._lastRetryAfter);
      }
    }
  }
  IFDB._lastErrorResponse = "";
  if (IFDB._lastStatusCode != expectedStatusCode) {
    if (IFDB._lastStatusCode > 0) {
      IFDB._lastErrorResponse = IFDBhttpClient->getString();
      AddLog(LOG_LEVEL_INFO, PSTR("IFX: %s"), IFDB._lastErrorResponse.c_str());  // {"error":"database not found: \"db\""}
    } else {
      IFDB._lastErrorResponse = IFDBhttpClient->errorToString(IFDB._lastStatusCode);
      AddLog(LOG_LEVEL_INFO, PSTR("IFX: Error %s"), IFDB._lastErrorResponse.c_str());
    }
  }
}

bool InfluxDbValidateConnection(void) {
  if (!IFDBwifiClient && !InfluxDbInit()) {
    IFDB._lastStatusCode = 0;
    IFDB._lastErrorResponse = FPSTR(UninitializedMessage);
    return false;
  }
  // on version 1.x /ping will by default return status code 204, without verbose
  String url = IFDB._serverUrl + (2 == Settings->influxdb_version ? "/health" : "/ping?verbose=true");
  if (1 == Settings->influxdb_version) {
    url += InfluxDbAuth();
  }
  // on version 1.8.9 /health works fine
//  String url = IFDB._serverUrl + "/health";
  AddLog(LOG_LEVEL_INFO, PSTR("IFX: Validating connection to %s"), url.c_str());

  if (!IFDBhttpClient->begin(*IFDBwifiClient, url)) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("IFX: Begin failed"));
    return false;
  }
  IFDBhttpClient->addHeader(F("Accept"), F("application/json"));

  IFDB._lastStatusCode = IFDBhttpClient->GET();
  IFDB._lastErrorResponse = "";
  InfluxDbAfterRequest(200, false);
  IFDBhttpClient->end();

  return IFDB._lastStatusCode == 200;
}

int InfluxDbPostData(const char *data) {
  if (!IFDBwifiClient && !InfluxDbInit()) {
    IFDB._lastStatusCode = 0;
    IFDB._lastErrorResponse = FPSTR(UninitializedMessage);
    return 0;
  }
  if (data) {
    if (!IFDBhttpClient->begin(*IFDBwifiClient, IFDB._writeUrl)) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("IFX: Begin failed"));
      return false;
    }

    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("IFX: Sending\n%s"), data);
    IFDBhttpClient->addHeader(F("Content-Type"), F("text/plain"));
    InfluxDbBeforeRequest();
    IFDB._lastStatusCode = IFDBhttpClient->POST((uint8_t*)data, strlen(data));
    InfluxDbAfterRequest(204, true);
    IFDBhttpClient->end();
  }
  return IFDB._lastStatusCode;
}

/*********************************************************************************************/

char* InfluxDbMakeNumber(char* dest, const char* source) {
  // Convert special text as found in kOptions to a number
  // Like "OFF" -> 0, "ON" -> 1, "TOGGLE" -> 2
  int number = GetStateNumber(source);
  if (number >= 0) {
    itoa(number, dest, 10);
    return dest;
  }
  return (char*)source;
}

void InfluxDbProcessJson(void) {
  if (!IFDB.init) { return; }

//  AddLog(LOG_LEVEL_DEBUG, PSTR("IFX: JSON %s"), TasmotaGlobal.mqtt_data.c_str());

  String jsonStr = TasmotaGlobal.mqtt_data;
  JsonParser parser((char *)jsonStr.c_str());
  JsonParserObject root = parser.getRootObject();
  if (root) {
    char number[32];
    char linebuf[128];   // 'temperature,device=demo,sensor=ds18b20,id=01144A0CB2AA value=26.44\n'
    char sensor[64];     // 'ds18b20'
    char type[64];       // 'temperature'
    char sensor_id[32];  // ',id=01144A0CB2AA'
    sensor_id[0] = '\0';

    String data = "";    // Multiple linebufs

    for (auto key1 : root) {
      JsonParserToken value1 = key1.getValue();
      if (value1.isObject()) {
        JsonParserObject Object2 = value1.getObject();
        for (auto key2 : Object2) {
          JsonParserToken value2 = key2.getValue();
          if (value2.isObject()) {
            JsonParserObject Object3 = value2.getObject();
            for (auto key3 : Object3) {
              const char *value = key3.getValue().getStr();
              if (value != nullptr) {
                value = InfluxDbMakeNumber(number, value);
                if (isdigit(value[0])) {
                  // Level 3
                  LowerCase(sensor, key2.getStr());
                  LowerCase(type, key3.getStr());
                  // temperature,device=tasmota1,sensor=DS18B20 value=24.44
                  snprintf_P(linebuf, sizeof(linebuf), PSTR("%s,device=%s,sensor=%s value=%s\n"),
                    type, TasmotaGlobal.mqtt_topic, sensor, value);
                  data += linebuf;
                }
              }
            }
          } else {
            // Level 2
            // { ... "ANALOG":{"Temperature":184.72},"DS18B20":{"Id":"01144A0CB2AA","Temperature":24.88},"HTU21":{"Temperature":25.32,"Humidity":49.2,"DewPoint":13.88},"Global":{"Temperature":24.88,"Humidity":49.2,"DewPoint":13.47}, ... }
            bool isarray = value2.isArray();
            const char *value = (isarray) ? (value2.getArray())[0].getStr() : value2.getStr();
            if (value != nullptr) {
              value = InfluxDbMakeNumber(number, value);
              if (isdigit(value[0])) {
                LowerCase(sensor, key1.getStr());
                LowerCase(type, key2.getStr());

  //              AddLog(LOG_LEVEL_DEBUG, PSTR("IFX2: sensor %s (%s), type %s (%s)"), key1.getStr(), sensor, key2.getStr(), type);

                if (strcmp(type, "totalstarttime") != 0) {  // Not needed/wanted
                  if (strcmp(type, "id") == 0) {            // Index for DS18B20
                    snprintf_P(sensor_id, sizeof(sensor_id), PSTR(",id=%s"), value);
                  } else {
                    if (isarray) {
                      JsonParserArray arr = value2.getArray();
                      uint32_t i = 0;
                      for (auto val : arr) {
                        i++;
                        // power1,device=shelly25,sensor=energy value=0.00
                        // power2,device=shelly25,sensor=energy value=4.12
                        snprintf_P(linebuf, sizeof(linebuf), PSTR("%s%d,device=%s,sensor=%s%s value=%s\n"),
                          type, i, TasmotaGlobal.mqtt_topic, sensor, sensor_id, val.getStr());
                        data += linebuf;
                      }
                    } else {
                      // temperature,device=demo,sensor=ds18b20,id=01144A0CB2AA value=22.63
                      snprintf_P(linebuf, sizeof(linebuf), PSTR("%s,device=%s,sensor=%s%s value=%s\n"),
                        type, TasmotaGlobal.mqtt_topic, sensor, sensor_id, value);
                      data += linebuf;
                    }
                    sensor_id[0] = '\0';
                  }
                }
              }
            }
          }
        }
      } else {
        // Level 1
        // {"Time":"2021-08-13T14:15:56","Switch1":"ON","Switch2":"OFF", ... "TempUnit":"C"}
        const char *value = value1.getStr();
        if (value != nullptr) {
          value = InfluxDbMakeNumber(number, value);
          if (isdigit(value[0])) {
            LowerCase(type, key1.getStr());

            if (!((strcasecmp_P(type, PSTR(D_JSON_TIME)) == 0) ||              // No time,device=demo value=2021-08-11T09:46:29
                  (strcasecmp_P(type, PSTR(D_JSON_TEMPERATURE_UNIT)) == 0) ||  // No tempunit,device=demo value=C
                  (strcasecmp_P(type, PSTR(D_JSON_PRESSURE_UNIT)) == 0) ||
                  (strcasecmp_P(type, PSTR(D_JSON_SPEED_UNIT)) == 0)
                  )) {
              // switch1,device=demo value=1
              snprintf_P(linebuf, sizeof(linebuf), PSTR("%s,device=%s,sensor=device value=%s\n"),
                type, TasmotaGlobal.mqtt_topic, value);
              data += linebuf;
            }
          }
        }
      }
    }
    if (data.length() > 0 ) {
//      AddLog(LOG_LEVEL_DEBUG, PSTR("IFX: Sensor data:\n%s"), data.c_str());
      InfluxDbPostData(data.c_str());
    }
  }
}

void InfluxDbPublishPowerState(uint32_t device) {
  Response_P(PSTR("{\"power%d\":\"%d\"}"), device, bitRead(TasmotaGlobal.power, device -1));
  InfluxDbProcessJson();
}

void InfluxDbLoop(void) {
  if (!TasmotaGlobal.global_state.network_down) {
    IFDB.interval--;
    if (IFDB.interval <= 0 || IFDB.interval > Settings->tele_period) {
      IFDB.interval = Settings->tele_period;
      if (!IFDB.init) {
        if (InfluxDbParameterInit()) {
          IFDB.init = InfluxDbValidateConnection();
          if (IFDB.init) {
            IFDB.interval = INFLUXDB_INITIAL;
          }
        }
      } else {
        ResponseClear();
        if (MqttShowSensor()) {   // Pull sensor data
          InfluxDbProcessJson();
        };
      }
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

#define D_PRFX_INFLUXDB         "Ifx"
#define D_CMND_INFLUXDBHOST     "Host"
#define D_CMND_INFLUXDBPORT     "Port"
#define D_CMND_INFLUXDBUSER     "User"
#define D_CMND_INFLUXDBORG      "Org"
#define D_CMND_INFLUXDBPASSWORD "Password"
#define D_CMND_INFLUXDBTOKEN    "Token"
#define D_CMND_INFLUXDBDATABASE "Database"
#define D_CMND_INFLUXDBBUCKET   "Bucket"

const char kInfluxDbCommands[] PROGMEM = D_PRFX_INFLUXDB "|"  // Prefix
  "|"
  D_CMND_INFLUXDBHOST "|" D_CMND_INFLUXDBPORT "|"
  D_CMND_INFLUXDBUSER "|" D_CMND_INFLUXDBORG "|"
  D_CMND_INFLUXDBPASSWORD "|" D_CMND_INFLUXDBTOKEN "|"
  D_CMND_INFLUXDBDATABASE "|" D_CMND_INFLUXDBBUCKET;

void (* const InfluxCommand[])(void) PROGMEM = {
  &CmndInfluxDbState,
  &CmndInfluxDbHost, &CmndInfluxDbPort,
  &CmndInfluxDbUser, &CmndInfluxDbUser,
  &CmndInfluxDbPassword, &CmndInfluxDbPassword,
  &CmndInfluxDbDatabase, &CmndInfluxDbDatabase };

void InfluxDbReinit(void) {
  IFDB.init = false;
  IFDB.interval = 2;
}

void CmndInfluxDbState(void) {
  Response_P(PSTR("{\"" D_PRFX_INFLUXDB "\":{\"" D_CMND_INFLUXDBHOST "\":\"%s\",\"" D_CMND_INFLUXDBPORT "\":%d,\"Version\":%d"),
    SettingsText(SET_INFLUXDB_HOST), Settings->influxdb_port, Settings->influxdb_version);
  if (1 == Settings->influxdb_version) {
    ResponseAppend_P(PSTR(",\"" D_CMND_INFLUXDBDATABASE "\":\"%s\",\"" D_CMND_INFLUXDBUSER "\":\"%s\"}}"),
      SettingsText(SET_INFLUXDB_BUCKET), SettingsText(SET_INFLUXDB_ORG));
  } else {
    ResponseAppend_P(PSTR(",\"" D_CMND_INFLUXDBBUCKET "\":\"%s\",\"" D_CMND_INFLUXDBORG "\":\"%s\"}}"),
      SettingsText(SET_INFLUXDB_BUCKET), SettingsText(SET_INFLUXDB_ORG));
  }
}

void CmndInfluxDbHost(void) {
  if (XdrvMailbox.data_len > 0) {
    SettingsUpdateText(SET_INFLUXDB_HOST, (SC_CLEAR == Shortcut()) ? "" : (SC_DEFAULT == Shortcut()) ? PSTR(INFLUXDB_HOST) : XdrvMailbox.data);
    InfluxDbReinit();
  }
  ResponseCmndChar(SettingsText(SET_INFLUXDB_HOST));
}

void CmndInfluxDbPort(void) {
  if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload < 65536)) {
    Settings->influxdb_port = (1 == XdrvMailbox.payload) ? INFLUXDB_PORT : XdrvMailbox.payload;
    InfluxDbReinit();
  }
  ResponseCmndNumber(Settings->influxdb_port);
}

void CmndInfluxDbUser(void) {
  if (XdrvMailbox.data_len > 0) {
    Settings->influxdb_version = (XdrvMailbox.command[3] == 'U') ? 1 : 2;  // User or Org
    SettingsUpdateText(SET_INFLUXDB_ORG, (SC_CLEAR == Shortcut()) ? "" : (SC_DEFAULT == Shortcut()) ? PSTR(INFLUXDB_ORG) : XdrvMailbox.data);
    InfluxDbReinit();
  }
  ResponseCmndChar(SettingsText(SET_INFLUXDB_ORG));
}

void CmndInfluxDbPassword(void) {
  bool show_asterisk = (2 == XdrvMailbox.index);
  if (XdrvMailbox.data_len > 0) {
    Settings->influxdb_version = (XdrvMailbox.command[3] == 'P') ? 1 : 2;  // Password or Token
    SettingsUpdateText(SET_INFLUXDB_TOKEN, (SC_CLEAR == Shortcut()) ? "" : (SC_DEFAULT == Shortcut()) ? PSTR(INFLUXDB_TOKEN) : XdrvMailbox.data);
    if (!show_asterisk) {
      ResponseCmndChar(SettingsText(SET_INFLUXDB_TOKEN));
    }
    InfluxDbReinit();
  } else {
    show_asterisk = true;
  }
  if (show_asterisk) {
    Response_P(S_JSON_COMMAND_ASTERISK, XdrvMailbox.command);
  }
}

void CmndInfluxDbDatabase(void) {
  if (XdrvMailbox.data_len > 0) {
    Settings->influxdb_version = (XdrvMailbox.command[3] == 'D') ? 1 : 2;  // Database or Bucket
    SettingsUpdateText(SET_INFLUXDB_BUCKET, (SC_CLEAR == Shortcut()) ? "" : (SC_DEFAULT == Shortcut()) ? PSTR(INFLUXDB_BUCKET) : XdrvMailbox.data);
    InfluxDbReinit();
  }
  ResponseCmndChar(SettingsText(SET_INFLUXDB_BUCKET));
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns90(uint8_t function) {
  bool result = false;

  if (!Settings->sbflag1.influxdb_default) {
    Settings->influxdb_version = INFLUXDB_VERSION;
    Settings->influxdb_port = INFLUXDB_PORT;
    SettingsUpdateText(SET_INFLUXDB_HOST, PSTR(INFLUXDB_HOST));
    SettingsUpdateText(SET_INFLUXDB_ORG, PSTR(INFLUXDB_ORG));
    SettingsUpdateText(SET_INFLUXDB_TOKEN, PSTR(INFLUXDB_TOKEN));
    SettingsUpdateText(SET_INFLUXDB_BUCKET, PSTR(INFLUXDB_BUCKET));
    Settings->sbflag1.influxdb_default = 1;
  }

  switch (function) {
    case FUNC_EVERY_SECOND:
      InfluxDbLoop();
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kInfluxDbCommands, InfluxCommand);
      break;
  }
  return result;
}

#endif  // USE_INFLUXDB
