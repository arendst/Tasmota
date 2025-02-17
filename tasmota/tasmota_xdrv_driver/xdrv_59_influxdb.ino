/*
  xdrv_59_influxdb.ino - Influxdb support for Tasmota

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
 * Ifx         - Set Influxdb state off (0) or on (1) and show current state
 * IfxHost     - Set Influxdb host name or IP address
 * IfxPort     - Set Influxdb port
 * IfxDatabase - Set Influxdb v1 and database name
 * IfxUser     - Set Influxdb v1 and userid
 * IfxPassword - Set Influxdb v1 and password
 * IfxBucket   - Set Influxdb v2 and bucket name
 * IfxOrg      - Set Influxdb v2 and organization
 * IfxToken    - Set Influxdb v2 and token
 * IfxPeriod   - Set Influxdb period. If not set (or 0), use Teleperiod
 * IfxSensor   - Set Influxdb sensor logging off (0) or on (1)
 * IfxRP       - Set Influxdb retention policy
 * IfxLog      - Set Influxdb logging level (4 = default)
 * IfxFeed     - Feed Influxdb with JSON data
 *
 * The following triggers result in automatic influxdb numeric feeds without appended time:
 * - this driver initiated state message
 * - this driver initiated teleperiod data
 * - power commands
\*********************************************************************************************/

#define XDRV_59            59

#ifndef INFLUXDB_INITIAL
#define INFLUXDB_INITIAL   7             // Initial number of seconds after wifi connect keeping in mind sensor initialization
#endif
#ifndef INFLUXDB_STATE
#define INFLUXDB_STATE     0             // [Ifx] Influxdb initially Off (0) or On (1)
#endif
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
#ifndef INFLUXDB_RP
#define INFLUXDB_RP        ""            // [IfxRP] Influxdb v1 retention policy (blank is default, usually autogen infinite)
#endif

static const char UninitializedMessage[] PROGMEM = "Unconfigured instance";
// This cannot be put to PROGMEM due to the way how it is used
static const char RetryAfter[] = "Retry-After";
static const char TransferEncoding[] = "Transfer-Encoding";

#if defined(ESP32) && defined(USE_WEBCLIENT_HTTPS)
  HTTPClientLight *IFDBhttpClient = nullptr;
#else
  WiFiClient *IFDBwifiClient = nullptr;
  HTTPClient *IFDBhttpClient = nullptr;
#endif

struct {
  String _serverUrl;                     // Connection info
  String _writeUrl;                      // Cached full write url
  String _lastErrorResponse;             // Server reponse or library error message for last failed request
  uint32_t _lastRequestTime = 0;         // Last time in ms we made a request to server
  int interval = 0;
  int _lastStatusCode = 0;               // HTTP status code of last request to server
  int _lastRetryAfter = 0;               // Store retry timeout suggested by server after last request
  uint8_t log_level = LOG_LEVEL_DEBUG_MORE;
  bool _connectionReuse;                 // true if HTTP connection should be kept open. Usable for frequent writes. Default false
  bool init = false;
} IFDB;

/*********************************************************************************************\
 * Influxdb library function
\*********************************************************************************************/

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

bool InfluxDbHostByName(void) {
  String host = SettingsText(SET_INFLUXDB_HOST);
  IFDB._serverUrl = "";
  if (strncmp(host.c_str(),"http",4))
    IFDB._serverUrl += "http://";
  IFDB._serverUrl += host;
  if (Settings->influxdb_port) {
    IFDB._serverUrl += ":";
    IFDB._serverUrl += Settings->influxdb_port;
  }
  return true;
}

bool InfluxDbParameterInit(void) {
  if (strlen(SettingsText(SET_INFLUXDB_BUCKET)) == 0 ||
      (2 == Settings->influxdb_version && (strlen(SettingsText(SET_INFLUXDB_ORG)) == 0 ||
                                           strlen(SettingsText(SET_INFLUXDB_TOKEN)) == 0))) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("IFX: Invalid parameters"));
    return false;
  }
  if (!InfluxDbHostByName()) { return false; }

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
    if (strlen(SettingsText(SET_INFLUXDB_RP)) != 0) {
      IFDB._writeUrl += "&rp=";
      IFDB._writeUrl += UrlEncode(SettingsText(SET_INFLUXDB_RP));
    }
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR("IFX: Url %s"), IFDB._writeUrl.c_str());

  return true;
}

bool InfluxDbInit(void) {
#if defined(ESP32) && defined(USE_WEBCLIENT_HTTPS)
  if (!IFDBhttpClient) {
    IFDBhttpClient = new HTTPClientLight;
  }
#else
  IFDBwifiClient = new WiFiClient;
  if (!IFDBhttpClient) {
    IFDBhttpClient = new HTTPClient;
  }
#endif
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
    if (IFDB._lastStatusCode >= 429) { // Retryable server errors
      if (IFDBhttpClient->hasHeader(RetryAfter)) {
        IFDB._lastRetryAfter = IFDBhttpClient->header(RetryAfter).toInt();
        AddLog(LOG_LEVEL_DEBUG, PSTR("IFX: Reply after %d"), IFDB._lastRetryAfter);
      }
    }
  }
  IFDB._lastErrorResponse = "";
  if (IFDB._lastStatusCode != expectedStatusCode) {
    if (IFDB._lastStatusCode > 0) {
      IFDB._lastErrorResponse = IFDBhttpClient->getString();  // {"error":"database not found: \"db\""}\n
    } else {
      IFDB._lastErrorResponse = IFDBhttpClient->errorToString(IFDB._lastStatusCode);
    }
    IFDB._lastErrorResponse.trim();  // Remove trailing \n
    AddLog(LOG_LEVEL_INFO, PSTR("IFX: Error '%s'"), IFDB._lastErrorResponse.c_str());
  } else {
    AddLog(IFDB.log_level, PSTR("IFX: Done"));
  }
}

bool InfluxDbValidateConnection(void) {
#if defined(ESP32) && defined(USE_WEBCLIENT_HTTPS)
  if (!InfluxDbInit()) {
#else
  if (!IFDBwifiClient && !InfluxDbInit()) {
#endif
    IFDB._lastStatusCode = 0;
    IFDB._lastErrorResponse = FPSTR(UninitializedMessage);
    return false;
  }
  // on version 1.x /ping will by default return status code 204, without verbose
  if (!InfluxDbHostByName()) { return false; }

  String url = IFDB._serverUrl + (2 == Settings->influxdb_version ? "/health" : "/ping?verbose=true");
  if (1 == Settings->influxdb_version) {
    url += InfluxDbAuth();
  }
  AddLog(LOG_LEVEL_INFO, PSTR("IFX: Validating connection to %s"), url.c_str());

#if defined(ESP32) && defined(USE_WEBCLIENT_HTTPS)
  if (!IFDBhttpClient->begin(url)) {
#else // HTTP only
  if (!IFDBhttpClient->begin(*IFDBwifiClient, url)) {
#endif
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
#if defined(ESP32) && defined(USE_WEBCLIENT_HTTPS)
  if (!InfluxDbInit()) {
#else
  if (!IFDBwifiClient && !InfluxDbInit()) {
#endif
    IFDB._lastStatusCode = 0;
    IFDB._lastErrorResponse = FPSTR(UninitializedMessage);
    return 0;
  }
  if (data) {
#if defined(ESP32) && defined(USE_WEBCLIENT_HTTPS)
    if (!IFDBhttpClient->begin(IFDB._writeUrl)) {
#else
    if (!IFDBhttpClient->begin(*IFDBwifiClient, IFDB._writeUrl)) {
#endif
      AddLog(LOG_LEVEL_DEBUG, PSTR("IFX: Begin failed"));
      return false;
    }

    Trim((char*)data);  // Remove trailing \n
    AddLog(IFDB.log_level, PSTR("IFX: Sending\n%s"), data);
    IFDBhttpClient->addHeader(F("Content-Type"), F("text/plain"));
    InfluxDbBeforeRequest();
    IFDB._lastStatusCode = IFDBhttpClient->POST((uint8_t*)data, strlen(data));
    AddLog(IFDB.log_level, PSTR("IFX: POST statusCode %d"), IFDB._lastStatusCode);
    InfluxDbAfterRequest(204, true);
    IFDBhttpClient->end();
  }
  return IFDB._lastStatusCode;
}

/*********************************************************************************************\
 * Data preparation
\*********************************************************************************************/

char* InfluxDbNumber(char* alternative, JsonParserToken value) {
  if (value.isValid()) {
    char* source = (char*)value.getStr();
    // Test for valid numeric data ('-.0123456789') or ON, OFF etc. as defined in kOptions
    if (source != nullptr) {
      char* out = source;
      // Convert special text as found in kOptions to a number
      // Like "OFF" -> 0, "ON" -> 1, "TOGGLE" -> 2
      int number = GetStateNumber(source);
      if (number >= 0) {
        itoa(number, alternative, 10);
        out = alternative;
      }
      if (IsNumeric(out)) {
        return out;
      }
    }
  }
  return nullptr;
}

void InfluxDbProcessJson(bool use_copy = false) {
  if (!IFDB.init) { return; }

  char *json_data = ResponseData();
  if (use_copy) {
    json_data = (char*)malloc(ResponseSize()+2);
    if (!json_data) { return; }
    strlcpy(json_data, ResponseData(), ResponseSize());
  }

  AddLog(IFDB.log_level, PSTR("IFX: Process %s"), json_data);

  JsonParser parser(json_data);  // Destroys json_data
  JsonParserObject root = parser.getRootObject();
  if (root) {
    char number[12];     // '1' to '255'
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
              char* value = InfluxDbNumber(number, key3.getValue());
              if ((value != nullptr) && key2.isValid() && key3.isValid()) {
                // Level 3
                LowerCase(sensor, key2.getStr());
                LowerCase(type, key3.getStr());
                // temperature,device=tasmota1,sensor=DS18B20 value=24.44
                snprintf_P(linebuf, sizeof(linebuf), PSTR("%s,device=%s,sensor=%s value=%s\n"),
                  type, TasmotaGlobal.mqtt_topic, sensor, value);
                data += linebuf;
              }
            }
          } else {
            // Level 2
            // { ... "ANALOG":{"Temperature":184.72},"DS18B20":{"Id":"01144A0CB2AA","Temperature":24.88},"HTU21":{"Temperature":25.32,"Humidity":49.2,"DewPoint":13.88},"Global":{"Temperature":24.88,"Humidity":49.2,"DewPoint":13.47}, ... }
            if (!key1.isValid() || !value2.isValid()) { continue; }
            LowerCase(type, key2.getStr());
            bool is_id = (!strcmp_P(type, PSTR("id")));  // Index for DS18B20
            bool is_array = value2.isArray();
            char* value = nullptr;
            if (is_id && !is_array) {
              snprintf_P(sensor_id, sizeof(sensor_id), PSTR(",id=%s"), value2.getStr());
            } else {
              value = InfluxDbNumber(number, (is_array) ? (value2.getArray())[0] : value2);
            }
            if ((value != nullptr) && key2.isValid()) {
              LowerCase(sensor, key1.getStr());

//              AddLog(LOG_LEVEL_DEBUG, PSTR("IFX2: sensor %s (%s), type %s (%s)"), key1.getStr(), sensor, key2.getStr(), type);

              if (is_array) {
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
      } else {
        // Level 1
        // {"Time":"2021-08-13T14:15:56","Switch1":"ON","Switch2":"OFF", ... "TempUnit":"C"}
        char* value = InfluxDbNumber(number, value1);
        if ((value != nullptr) && key1.isValid()) {
          LowerCase(type, key1.getStr());
          // switch1,device=demo,sensor=device value=0
          // power1,device=demo,sensor=device value=1
          snprintf_P(linebuf, sizeof(linebuf), PSTR("%s,device=%s,sensor=device value=%s\n"),
            type, TasmotaGlobal.mqtt_topic, value);
          data += linebuf;
        }
      }
    }
    if (data.length() > 0 ) {
//      AddLog(LOG_LEVEL_DEBUG, PSTR("IFX: Sensor data:\n%s"), data.c_str());
      InfluxDbPostData(data.c_str());
    }
  }

  if (use_copy) {
    free(json_data);
  }
}

void InfluxDbProcess(bool use_copy) {
  if (Settings->sbflag1.influxdb_sensor) {  // IfxSensor
    InfluxDbProcessJson(use_copy);
  }
}

void InfluxDbPublishPowerState(uint32_t device) {
  Response_P(PSTR("{\"power%d\":\"%d\"}"), device, bitRead(TasmotaGlobal.power, device -1));
  InfluxDbProcessJson();
}

void InfluxDbLoop(void) {
  if (!TasmotaGlobal.global_state.network_down) {
    IFDB.interval--;
    uint16_t period = Settings->influxdb_period ? Settings->influxdb_period : Settings->tele_period;
    if (IFDB.interval <= 0 || IFDB.interval > period) {
      IFDB.interval = period;
      if (!IFDB.init) {
        if (InfluxDbParameterInit()) {
          IFDB.init = InfluxDbValidateConnection();
          if (IFDB.init) {
            IFDB.interval = INFLUXDB_INITIAL;
          }
        }
      } else {

        // {"Time":"2021-08-14T17:19:33","Uptime":"0T00:24:09","UptimeSec":1449,"Heap":29,"SleepMode":"Dynamic","Sleep":50,"LoadAvg":19,"MqttCount":1,"POWER1":"ON","Wifi":{"AP":1,"SSId":"indebuurt_IoT","BSSId":"98:38:C9:CA:17:C1","Channel":11,"Mode":"11n","RSSI":100,"Signal":-44,"LinkCount":1,"Downtime":"0T00:00:03"}}
        uint32_t backup = Settings->flag.device_index_enable;
        Settings->flag.device_index_enable = 1;  // Force indexes to power
        ResponseClear();
        MqttShowState();          // Pull state data
        Settings->flag.device_index_enable = backup;
        InfluxDbProcessJson();

        // {"Time":"2021-08-14T17:19:33","Switch1":"ON","Switch2":"OFF","ANALOG":{"Temperature":184.72},"DS18B20":{"Id":"01144A0CB2AA","Temperature":27.50},"HTU21":{"Temperature":28.23,"Humidity":39.7,"DewPoint":13.20},"Global":{"Temperature":27.50,"Humidity":39.7,"DewPoint":12.55},"TempUnit":"C"}
        ResponseClear();
        if (MqttShowSensor(true)) {   // Pull sensor data
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
#define D_CMND_INFLUXDBLOG      "Log"
#define D_CMND_INFLUXDBHOST     "Host"
#define D_CMND_INFLUXDBPORT     "Port"
#define D_CMND_INFLUXDBUSER     "User"
#define D_CMND_INFLUXDBORG      "Org"
#define D_CMND_INFLUXDBPASSWORD "Password"
#define D_CMND_INFLUXDBTOKEN    "Token"
#define D_CMND_INFLUXDBDATABASE "Database"
#define D_CMND_INFLUXDBBUCKET   "Bucket"
#define D_CMND_INFLUXDBPERIOD   "Period"
#define D_CMND_INFLUXDBSENSOR   "Sensor"
#define D_CMND_INFLUXDBRP       "RP"
#define D_CMND_INFLUXDBFEED     "Feed"

const char kInfluxDbCommands[] PROGMEM = D_PRFX_INFLUXDB "|"  // Prefix
  "|" D_CMND_INFLUXDBLOG "|"
  D_CMND_INFLUXDBHOST "|" D_CMND_INFLUXDBPORT "|"
  D_CMND_INFLUXDBUSER "|" D_CMND_INFLUXDBORG "|"
  D_CMND_INFLUXDBPASSWORD "|" D_CMND_INFLUXDBTOKEN "|"
  D_CMND_INFLUXDBDATABASE "|" D_CMND_INFLUXDBBUCKET "|"
  D_CMND_INFLUXDBPERIOD "|" D_CMND_INFLUXDBSENSOR "|"
  D_CMND_INFLUXDBRP "|" D_CMND_INFLUXDBFEED;

void (* const InfluxCommand[])(void) PROGMEM = {
  &CmndInfluxDbState, &CmndInfluxDbLog,
  &CmndInfluxDbHost, &CmndInfluxDbPort,
  &CmndInfluxDbUser, &CmndInfluxDbUser,
  &CmndInfluxDbPassword, &CmndInfluxDbPassword,
  &CmndInfluxDbDatabase, &CmndInfluxDbDatabase,
  &CmndInfluxDbPeriod, &CmndInfluxDbSensor,
  &CmndInfluxDbRP, &CmndInfluxDbFeed };

void InfluxDbReinit(void) {
  IFDB.init = false;
  IFDB.interval = 2;
}

void CmndInfluxDbState(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    if (Settings->sbflag1.influxdb_state != XdrvMailbox.payload) {
      Settings->sbflag1.influxdb_state = XdrvMailbox.payload;
      InfluxDbReinit();
    }
  }
  Response_P(PSTR("{\"" D_PRFX_INFLUXDB "\":{\"State\":\"%s\",\"" D_CMND_INFLUXDBHOST "\":\"%s\",\"" D_CMND_INFLUXDBPORT "\":%d,\"Version\":%d"),
    GetStateText(Settings->sbflag1.influxdb_state), SettingsText(SET_INFLUXDB_HOST), Settings->influxdb_port, Settings->influxdb_version);
  if (1 == Settings->influxdb_version) {
    ResponseAppend_P(PSTR(",\"" D_CMND_INFLUXDBDATABASE "\":\"%s\",\"" D_CMND_INFLUXDBUSER "\":\"%s\"}}"),
      SettingsText(SET_INFLUXDB_BUCKET), SettingsText(SET_INFLUXDB_ORG));
  } else {
    ResponseAppend_P(PSTR(",\"" D_CMND_INFLUXDBBUCKET "\":\"%s\",\"" D_CMND_INFLUXDBORG "\":\"%s\"}}"),
      SettingsText(SET_INFLUXDB_BUCKET), SettingsText(SET_INFLUXDB_ORG));
  }
}

void CmndInfluxDbSensor(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->sbflag1.influxdb_sensor = XdrvMailbox.payload;  // IfxSensor
  }
  ResponseCmndStateText(Settings->sbflag1.influxdb_sensor);  // IfxSensor
}

void CmndInfluxDbLog(void) {
  if ((XdrvMailbox.payload >= LOG_LEVEL_NONE) && (XdrvMailbox.payload <= LOG_LEVEL_DEBUG_MORE)) {
    IFDB.log_level = XdrvMailbox.payload;
  }
  ResponseCmndNumber(IFDB.log_level);
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

void CmndInfluxDbRP(void) {
  if (XdrvMailbox.data_len > 0) {
    SettingsUpdateText(SET_INFLUXDB_RP, (SC_CLEAR == Shortcut()) ? "" : (SC_DEFAULT == Shortcut()) ? PSTR(INFLUXDB_RP) : XdrvMailbox.data);
    InfluxDbReinit();
  }
  ResponseCmndChar(SettingsText(SET_INFLUXDB_RP));
}

void CmndInfluxDbPeriod(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
    Settings->influxdb_period = XdrvMailbox.payload;
    if(Settings->influxdb_period > 0 && Settings->influxdb_period < 10) {
      Settings->influxdb_period = 10;
    }
  }
  ResponseCmndNumber(Settings->influxdb_period);
}

void CmndInfluxDbFeed(void) {
  // IfxFeed {"Data":10}
  if ((XdrvMailbox.data_len > 0) && ('{' == XdrvMailbox.data[0])) {
    Response_P(XdrvMailbox.data);
    InfluxDbProcessJson();
    ResponseCmndDone();
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv59(uint32_t function) {
  bool result = false;

  if (FUNC_PRE_INIT == function) {
    // Initial persistent settings executed only once
    if (!Settings->sbflag1.influxdb_default) {
      Settings->sbflag1.influxdb_state = INFLUXDB_STATE;
      Settings->influxdb_version = INFLUXDB_VERSION;
      Settings->influxdb_port = INFLUXDB_PORT;
      SettingsUpdateText(SET_INFLUXDB_HOST, PSTR(INFLUXDB_HOST));
      SettingsUpdateText(SET_INFLUXDB_ORG, PSTR(INFLUXDB_ORG));
      SettingsUpdateText(SET_INFLUXDB_TOKEN, PSTR(INFLUXDB_TOKEN));
      SettingsUpdateText(SET_INFLUXDB_BUCKET, PSTR(INFLUXDB_BUCKET));
      SettingsUpdateText(SET_INFLUXDB_RP, PSTR(INFLUXDB_RP));
      Settings->sbflag1.influxdb_default = 1;
    }
  } else if (FUNC_COMMAND == function) {
    result = DecodeCommand(kInfluxDbCommands, InfluxCommand);
  } else if (Settings->sbflag1.influxdb_state) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        InfluxDbLoop();
        break;
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif  // USE_INFLUXDB
