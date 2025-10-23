/*
  xdrv_07_ufs_domoticz.ino - domoticz support for Tasmota

  Copyright (C) 2025  Theo Arends

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

#ifdef USE_DOMOTICZ
#ifdef USE_UFILESYS
/*********************************************************************************************\
 * Domoticz support with all relays/buttons/switches using more RAM and Settings from filesystem
 *
 * Adds commands:
 * DzIdx<number> <idx>      - Set power number to Domoticz Idx allowing Domoticz to control Tasmota power
 * DzKeyIdx<number> <idx>   - Set button number to Domoticz Idx allowing Tasmota button as input to Domoticz
 * DzSwitchId<number> <idx> - Set switch number to Domoticz Idx allowing Tasmota switch as input to Domoticz
 * DzSensorIdx<type> <idx>  - Set sensor type to Domoticz Idx
 * DzUpdateTimer 0          - Send power state at teleperiod to Domoticz (default)
 * DzUpdateTimer <seconds>  - Send power state at <seconds> interval to Domoticz
 * DzSend1 <idx>,<values>   - {\"idx\":<idx>,\"nvalue\":0,\"svalue\":\"<values>\",\"Battery\":xx,\"RSSI\":yy}
 *   Example: rule1 on power1#state do dzsend1 9001,%value% endon
 * DzSend1 418,%var1%;%var2% or DzSend1 418,%var1%:%var2% - Notice colon as substitute to semi-colon
 * DzSend2 <idx>,<values>   - USE_SHUTTER only - {\"idx\":<idx>,\"nvalue\":<position>,\"svalue\":\"<values>\",\"Battery\":xx,\"RSSI\":yy}
 * DzSend3 <idx>,<values>   - {\"idx\":<idx>,\"nvalue\":<values>,\"Battery\":xx,\"RSSI\":yy}
 * DzSend4 <idx>,<state>    - {\"command\":\"switchlight\",\"idx\":<idx>,\"switchcmd\":\"<state>\"}
 * DzSend5 <idx>,<state>    - {\"command\":\"switchscene\",\"idx\":<idx>,\"switchcmd\":\"<state>\"}
\*********************************************************************************************/

#define XDRV_07             7

//#define DOMOTICZ_IDX_MAX           // Support for highest Domoticz Idx number over 65535 (uses uint32_t which uses more RAM)

//#define USE_DOMOTICZ_DEBUG         // Enable additional debug logging

#ifdef ESP32
#define DOMOTICZ_IDX_MAX           // Support for highest Domoticz Idx number (uses uint32_t)
#endif
#ifdef DOMOTICZ_IDX_MAX
typedef uint32_t uintdz_t;         // Max Domoticz Idx = 0xFFFFFFFF = 4294967295
#else
typedef uint16_t uintdz_t;         // Max Domoticz Idx = 0xFFFF = 65535
#endif

#define D_PRFX_DOMOTICZ "Dz"
#define D_CMND_IDX "Idx"
#define D_CMND_KEYIDX "KeyIdx"
#define D_CMND_SWITCHIDX "SwitchIdx"
#define D_CMND_SENSORIDX "SensorIdx"
#define D_CMND_UPDATETIMER "UpdateTimer"
#define D_CMND_DZSEND "Send"

const char kDomoticzCommands[] PROGMEM = D_PRFX_DOMOTICZ "|"  // Prefix
  D_CMND_IDX "|" D_CMND_KEYIDX "|" D_CMND_SWITCHIDX "|" D_CMND_SENSORIDX "|" D_CMND_UPDATETIMER "|" D_CMND_DZSEND ;

void (* const DomoticzCommand[])(void) PROGMEM = {
  &CmndDomoticzIdx, &CmndDomoticzKeyIdx, &CmndDomoticzSwitchIdx, &CmndDomoticzSensorIdx, &CmndDomoticzUpdateTimer, &CmndDomoticzSend };

const char DOMOTICZ_MESSAGE[] PROGMEM = "{\"idx\":%d,\"nvalue\":%d,\"svalue\":\"%s\",\"Battery\":%d,\"RSSI\":%d}";

const char kDomoticzSensors[] PROGMEM =  // Relates to enum DomoticzSensors
  D_DOMOTICZ_TEMP "|" D_DOMOTICZ_TEMP_HUM "|" D_DOMOTICZ_TEMP_HUM_BARO "|" D_DOMOTICZ_POWER_ENERGY "|" D_DOMOTICZ_ILLUMINANCE "|"
  D_DOMOTICZ_COUNT "|" D_DOMOTICZ_VOLTAGE "|" D_DOMOTICZ_CURRENT "|" D_DOMOTICZ_AIRQUALITY "|" D_DOMOTICZ_P1_SMART_METER "|" D_DOMOTICZ_SHUTTER ;

const char kDomoticzCommand[] PROGMEM = "switchlight|switchscene";

char domoticz_in_topic[] = DOMOTICZ_IN_TOPIC;

typedef struct DzSettings_t {
  uint32_t crc32;                    // To detect file changes
  uintdz_t* relay_idx;
  uintdz_t* key_idx;
  uintdz_t* switch_idx;
  uintdz_t sensor_idx[DZ_MAX_SENSORS];
  uintdz_t update_timer;
} DzSettings_t;

typedef struct Domoticz_t {
  DzSettings_t Settings;             // Persistent settings
#ifdef USE_SONOFF_IFAN
  uint32_t fan_debounce;             // iFan02 state debounce timer
#endif  // USE_SONOFF_IFAN
  int update_timer;
  uint8_t devices;
  uint8_t keys;
  uint8_t switches;
  bool subscribe;
  bool update_flag;
#ifdef USE_SHUTTER
  bool is_shutter;
#endif // USE_SHUTTER
} Domoticz_t;
Domoticz_t* Domoticz;

/*********************************************************************************************\
 * Driver Settings load and save
\*********************************************************************************************/

#define XDRV_07_KEY           "drvset03"

bool DomoticzLoadData(void) {
  char key[] = XDRV_07_KEY;
  String json = UfsJsonSettingsRead(key);
  if (json.length() == 0) { return false; }

  // {"Crc":1882268982,"Update":0,"Relay":[1,2,3,4],"Key":[5,6,7,8],"Switch":[9,10,11,12],"Sensor":[13,14]}
  JsonParser parser((char*)json.c_str());
  JsonParserObject root = parser.getRootObject();
  if (!root) { return false; }

  Domoticz->Settings.crc32 = root.getUInt(PSTR("Crc"), Domoticz->Settings.crc32);
  Domoticz->Settings.update_timer = root.getUInt(PSTR("Update"), Domoticz->Settings.update_timer);
  JsonParserArray arr = root[PSTR("Relay")];
  if (arr) {
    for (uint32_t i = 0; i < Domoticz->devices; i++) {
      if (arr[i]) { Domoticz->Settings.relay_idx[i] = arr[i].getUInt(); }
    }
  }
  arr = root[PSTR("Key")];
  if (arr) {
    for (uint32_t i = 0; i < Domoticz->keys; i++) {
      if (arr[i]) { Domoticz->Settings.key_idx[i] = arr[i].getUInt(); }
    }
  }
  arr = root[PSTR("Switch")];
  if (arr) {
    for (uint32_t i = 0; i < Domoticz->switches; i++) {
      if (arr[i]) { Domoticz->Settings.switch_idx[i] = arr[i].getUInt(); }
    }
  }
  arr = root[PSTR("Sensor")];
  if (arr) {
    for (uint32_t i = 0; i < DZ_MAX_SENSORS; i++) {
      if (arr[i]) { Domoticz->Settings.sensor_idx[i] = arr[i].getUInt(); }
    }
  }
  return true;
}

bool DomoticzSaveData(void) {
  Response_P(PSTR("{\"" XDRV_07_KEY "\":{"
                   "\"Crc\":%u,"
                   "\"Update\":%u"),
                   Domoticz->Settings.crc32,
                   Domoticz->Settings.update_timer);
  if (Domoticz->devices) {
    ResponseAppend_P(PSTR(",\"Relay\":"));
    for (uint32_t i = 0; i < Domoticz->devices; i++) {
      ResponseAppend_P(PSTR("%c%d"), (0==i)?'[':',', Domoticz->Settings.relay_idx[i]);
    }
    ResponseAppend_P(PSTR("]"));
  }
  if (Domoticz->keys) {
    ResponseAppend_P(PSTR(",\"Key\":"));
    for (uint32_t i = 0; i < Domoticz->keys; i++) {
      ResponseAppend_P(PSTR("%c%d"), (0==i)?'[':',', Domoticz->Settings.key_idx[i]);
    }
    ResponseAppend_P(PSTR("]"));
  }
  if (Domoticz->switches) {
    ResponseAppend_P(PSTR(",\"Switch\":"));
    for (uint32_t i = 0; i < Domoticz->switches; i++) {
      ResponseAppend_P(PSTR("%c%d"), (0==i)?'[':',', Domoticz->Settings.switch_idx[i]);
    }
    ResponseAppend_P(PSTR("]"));
  }
  ResponseAppend_P(PSTR(",\"Sensor\":"));
  for (uint32_t i = 0; i < DZ_MAX_SENSORS; i++) {
    ResponseAppend_P(PSTR("%c%d"), (0==i)?'[':',', Domoticz->Settings.sensor_idx[i]);
  }
  ResponseAppend_P(PSTR("]}}"));

  if (!UfsJsonSettingsWrite(ResponseData())) {
    return false;
  }
  return true;
}

void DomoticzDeleteData(void) {
  char key[] = XDRV_07_KEY;
  UfsJsonSettingsDelete(key);  // Use defaults
}

/*********************************************************************************************/

void DomoticzSettingsLoad(bool erase) {
  // Called from FUNC_PRE_INIT (erase = 0) once at restart
  // Called from FUNC_RESET_SETTINGS (erase = 1) after command reset 4, 5, or 6

//  memset(&Domoticz->Settings, 0x00, sizeof(DzSettings_t));  // Won't work as we need to keep our pointers
  Domoticz->Settings.update_timer = 0;
  for (uint32_t i = 0; i < Domoticz->devices; i++) {
    Domoticz->Settings.relay_idx[i] = 0;
    if (i < Domoticz->keys) {
      Domoticz->Settings.key_idx[i] = 0;
    }
    if (i < Domoticz->switches) {
      Domoticz->Settings.switch_idx[i] = 0;
    }
  }
  for (uint32_t i = 0; i < DZ_MAX_SENSORS; i++) {
    Domoticz->Settings.sensor_idx[i] = 0;
  }

#ifndef CONFIG_IDF_TARGET_ESP32P4
  // Init any other parameter in struct DzSettings
  Domoticz->Settings.update_timer = Settings->domoticz_update_timer;
  for (uint32_t i = 0; i < MAX_DOMOTICZ_IDX; i++) {
    if (i < Domoticz->devices) {
      Domoticz->Settings.relay_idx[i] = Settings->domoticz_relay_idx[i];
    }
    if (i < Domoticz->keys) {
      Domoticz->Settings.key_idx[i] = Settings->domoticz_key_idx[i];
    }
    if (i < Domoticz->switches) {
      Domoticz->Settings.switch_idx[i] = Settings->domoticz_switch_idx[i];
    }
  }
  for (uint32_t i = 0; i < MAX_DOMOTICZ_SNS_IDX; i++) {
    if (i < DZ_MAX_SENSORS) {
      Domoticz->Settings.sensor_idx[i] = Settings->domoticz_sensor_idx[i];
    }
  }
  // *** End Init default values ***
#endif  // CONFIG_IDF_TARGET_ESP32P4

  // Try to load key
  if (erase) {
    DomoticzDeleteData();
  }
  else if (DomoticzLoadData()) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CFG: Domoticz loaded from file"));
  }
  else {
    // File system not ready: No flash space reserved for file system
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("CFG: Domoticz use defaults as file system not ready or key not found"));
  }
}

void DomoticzSettingsSave(void) {
  // Called from FUNC_SAVE_SETTINGS every SaveData second and at restart
  uint32_t crc32 = GetCfgCrc32((uint8_t*)&Domoticz->Settings +4, sizeof(DzSettings_t) -4);  // Skip crc32
  if (Domoticz->devices) {
    crc32 += GetCfgCrc32((uint8_t*)Domoticz->Settings.relay_idx, Domoticz->devices * sizeof(uintdz_t));
  }
  if (Domoticz->keys) {
    crc32 += GetCfgCrc32((uint8_t*)Domoticz->Settings.key_idx, Domoticz->keys * sizeof(uintdz_t));
  }
  if (Domoticz->switches) {
    crc32 += GetCfgCrc32((uint8_t*)Domoticz->Settings.switch_idx, Domoticz->switches * sizeof(uintdz_t));
  }
  if (crc32 != Domoticz->Settings.crc32) {
    Domoticz->Settings.crc32 = crc32;
    if (DomoticzSaveData()) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("CFG: Domoticz saved to file"));
    } else {
      // File system not ready: No flash space reserved for file system
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("CFG: Domoticz ERROR File system not ready or unable to save file"));
    }
  }
}

/*********************************************************************************************/

int DomoticzBatteryQuality(void) {
  // Battery 0%: ESP 2.6V (minimum operating voltage is 2.5)
  // Battery 100%: ESP 3.6V (maximum operating voltage is 3.6)
  // Battery 101% to 200%: ESP over 3.6V (means over maximum operating voltage)

  int quality = 100;	// Voltage range from 2,6V > 0%  to 3,6V > 100%

#ifdef ESP8266
#ifdef USE_ADC_VCC
  uint16_t voltage = ESP.getVcc();
  if (voltage <= 2600) {
    quality = 0;
  } else if (voltage >= 4600) {
    quality = 200;
  } else {
    quality = (voltage - 2600) / 10;
  }
#endif  // USE_ADC_VCC
#endif  // ESP8266
  return quality;
}

int DomoticzRssiQuality(void) {
  // RSSI range: 0% to 10% (12 means disable RSSI in Domoticz)

  return WifiGetRssiAsQuality(WiFi.RSSI()) / 10;
}

uint32_t DomoticzRelayIdx(uint32_t relay) {
  if (relay >= Domoticz->devices) { return 0; }
  return Domoticz->Settings.relay_idx[relay];
}

void DomoticzSetRelayIdx(uint32_t relay, uint32_t idx) {
  if (relay >= Domoticz->devices) { return; }
  Domoticz->Settings.relay_idx[relay] = idx;
}

#ifdef USE_SONOFF_IFAN
void MqttPublishDomoticzFanState(void) {
  if (Settings->flag.mqtt_enabled && DomoticzRelayIdx(1)) {  // SetOption3 - Enable MQTT
    char svalue[8];  // Fanspeed value

    int fan_speed = GetFanspeed();
    snprintf_P(svalue, sizeof(svalue), PSTR("%d"), fan_speed * 10);
    Response_P(DOMOTICZ_MESSAGE, (int)DomoticzRelayIdx(1),
                                 (0 == fan_speed) ? 0 : 2,
                                 svalue,
                                 DomoticzBatteryQuality(), DomoticzRssiQuality());
    MqttPublish(domoticz_in_topic);

    Domoticz->fan_debounce = millis() + 1000;  // 1 second
  }
}

void DomoticzUpdateFanState(void) {
  if (!Domoticz) { return; }  // No MQTT enabled or unable to allocate memory

  if (Domoticz->update_flag) {
    MqttPublishDomoticzFanState();
  }
  Domoticz->update_flag = true;
}
#endif  // USE_SONOFF_IFAN

/*********************************************************************************************/

void MqttPublishDomoticzPowerState(uint8_t device) {
  if (Settings->flag.mqtt_enabled) {  // SetOption3 - Enable MQTT
    if (device < 1) { device = 1; }
    if (device > Domoticz->devices) { return; }
    if (DomoticzRelayIdx(device -1)) {
#ifdef USE_SHUTTER
      if (Domoticz->is_shutter) {
        // Shutter is updated by sensor update - power state should not be sent
      } else {
#endif  // USE_SHUTTER
#ifdef USE_SONOFF_IFAN
      if (IsModuleIfan() && (device > 1)) {
        // Fan handled by MqttPublishDomoticzFanState
      } else {
#endif  // USE_SONOFF_IFAN
        char svalue[8];  // Dimmer value

        snprintf_P(svalue, sizeof(svalue), PSTR("%d"), Settings->light_dimmer);
        Response_P(DOMOTICZ_MESSAGE, (int)DomoticzRelayIdx(device -1),
                                     (TasmotaGlobal.power & (1 << (device -1))) ? 1 : 0,
                                     (TasmotaGlobal.light_type) ? svalue : "",
                                     DomoticzBatteryQuality(), DomoticzRssiQuality());
        MqttPublish(domoticz_in_topic);
#ifdef USE_SONOFF_IFAN
      }
#endif // USE_SONOFF_IFAN
#ifdef USE_SHUTTER
      }
#endif //USE_SHUTTER
    }
  }
}

void DomoticzUpdatePowerState(uint8_t device) {
  if (!Domoticz) { return; }  // No MQTT enabled or unable to allocate memory

  if (Domoticz->update_flag) {
    MqttPublishDomoticzPowerState(device);
  }
  Domoticz->update_flag = true;
}

/*********************************************************************************************/

void DomoticzMqttUpdate(void) {
  if (Domoticz->subscribe && (Domoticz->Settings.update_timer || Domoticz->update_timer)) {
    Domoticz->update_timer--;
    if (Domoticz->update_timer <= 0) {
      Domoticz->update_timer = Domoticz->Settings.update_timer;
      for (uint32_t i = 1; i <= Domoticz->devices; i++) {
#ifdef USE_SHUTTER
        if (Domoticz->is_shutter) {
          // no power state updates for shutters
          break;
        }
#endif // USE_SHUTTER
#ifdef USE_SONOFF_IFAN
        if (IsModuleIfan() && (i > 1)) {
          MqttPublishDomoticzFanState();
          break;
        } else {
#endif  // USE_SONOFF_IFAN
          MqttPublishDomoticzPowerState(i);
#ifdef USE_SONOFF_IFAN
        }
#endif  // USE_SONOFF_IFAN
      }
    }
  }
}

void DomoticzMqttSubscribe(void) {
  bool any_relay = false;
  for (uint32_t i = 0; i < Domoticz->devices; i++) {
    if (DomoticzRelayIdx(i)) {
      any_relay = true;
      break;
    }
  }
  char stopic[TOPSZ];
  snprintf_P(stopic, sizeof(stopic), PSTR(DOMOTICZ_OUT_TOPIC "/#"));  // domoticz topic
  if (Domoticz->subscribe && !any_relay) {
    Domoticz->subscribe = false;
    MqttUnsubscribe(stopic);
  }
//  if (!Domoticz->subscribe && any_relay) {  // Fails on MQTT server reconnect
  if (any_relay) {
    Domoticz->subscribe = true;
    MqttSubscribe(stopic);
  }
}

int DomoticzIdx2Relay(uint32_t idx) {
  if (idx > 0) {
    for (uint32_t i = 0; i < Domoticz->devices; i++) {
      if (idx == DomoticzRelayIdx(i)) {
        return i;
      }
    }
  }
  return -1;  // Idx not mine
}

bool DomoticzMqttData(void) {
/*
  XdrvMailbox.topic = topic;
  XdrvMailbox.index = strlen(topic);
  XdrvMailbox.data = (char*)data;
  XdrvMailbox.data_len = data_len;
*/
  Domoticz->update_flag = true;

  if (!Domoticz->subscribe) { 
    return false;  // No Domoticz driver subscription so try user subscribes
  }

  // Default subscibed to domoticz/out/#
  if (strncasecmp_P(XdrvMailbox.topic, PSTR(DOMOTICZ_OUT_TOPIC), strlen(DOMOTICZ_OUT_TOPIC)) != 0) {
    return false;  // Process unchanged data
  }

  // topic is domoticz/out so check if valid data could be available
  if (XdrvMailbox.data_len < 20) {
    return true;  // No valid data
  }

#ifdef USE_DOMOTICZ_DEBUG
  char dom_data[XdrvMailbox.data_len +1];
  strcpy(dom_data, XdrvMailbox.data);
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_DOMOTICZ "%s = %s"), XdrvMailbox.topic, RemoveControlCharacter(dom_data));
#endif  // USE_DOMOTICZ_DEBUG

  // Quick check if this is mine using topic domoticz/out/{$idx}
  if (strlen(XdrvMailbox.topic) > strlen(DOMOTICZ_OUT_TOPIC)) {
    char* topic_index = &XdrvMailbox.topic[strlen(DOMOTICZ_OUT_TOPIC) +1];
    if (strchr(topic_index, '/') == nullptr) {         // Skip if topic ...floor/room
      if (DomoticzIdx2Relay(atoi(topic_index)) < 0) {
        return true;  // Idx not mine
      }
    }
  }

  String domoticz_data = XdrvMailbox.data;  // Copy the string into a new buffer that will be modified
  JsonParser parser((char*)domoticz_data.c_str());
  JsonParserObject domoticz = parser.getRootObject();
  if (!domoticz) {
    return true;  // To much or invalid data
  }
  int32_t relay_index = DomoticzIdx2Relay(domoticz.getUInt(PSTR("idx"), 0));
  if (relay_index < 0) {
    return true;  // Idx not mine
  }
  int32_t nvalue = domoticz.getInt(PSTR("nvalue"), -1);
  if ((nvalue < 0) || (nvalue > 16)) {
    return true;  // Nvalue out of boundaries
  }

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_DOMOTICZ "%s, idx %d, nvalue %d"), XdrvMailbox.topic, DomoticzRelayIdx(relay_index), nvalue);

  bool iscolordimmer = (strcmp_P(domoticz.getStr(PSTR("dtype")), PSTR("Color Switch")) == 0);
  bool isShutter = (strcmp_P(domoticz.getStr(PSTR("dtype")), PSTR("Light/Switch")) == 0) &&
                   (strncmp_P(domoticz.getStr(PSTR("switchType")), PSTR("Blinds"), 6) == 0);

#ifdef USE_SONOFF_IFAN
  if (IsModuleIfan() && (1 == relay_index)) {  // Idx 2 is fanspeed
    JsonParserToken svalue_tok = domoticz[PSTR("svalue1")];
    if (!svalue_tok) {
      return true;
    }
    uint32_t svalue = svalue_tok.getUInt();
    svalue = (2 == nvalue) ? svalue / 10 : 0;
    if (GetFanspeed() == svalue) {
      return true;  // Stop as already set
    }
    if (!TimeReached(Domoticz->fan_debounce)) {
      return true;  // Stop if device in limbo
    }
    snprintf_P(XdrvMailbox.topic, XdrvMailbox.index, PSTR("/" D_CMND_FANSPEED));
    snprintf_P(XdrvMailbox.data, XdrvMailbox.data_len, PSTR("%d"), svalue);
  } else
#endif  // USE_SONOFF_IFAN
#ifdef USE_SHUTTER
  if (isShutter) {
    uint32_t position = domoticz.getUInt(PSTR("svalue1"), 0);
    if (nvalue != 2) {
      position = (0 == nvalue) ? 0 : 100;
    }
    snprintf_P(XdrvMailbox.topic, TOPSZ, PSTR("/" D_PRFX_SHUTTER D_CMND_SHUTTER_POSITION));
    snprintf_P(XdrvMailbox.data, XdrvMailbox.data_len, PSTR("%d"), position);
    XdrvMailbox.data_len = position > 99 ? 3 : (position > 9 ? 2 : 1);
  } else
#endif // USE_SHUTTER
#ifdef USE_LIGHT
  if (iscolordimmer && 10 == nvalue) {  // Color_SetColor
    // https://www.domoticz.com/wiki/Domoticz_API/JSON_URL%27s#Set_a_light_to_a_certain_color_or_color_temperature
    JsonParserObject color = domoticz[PSTR("Color")].getObject();
    // JsonObject& color = domoticz["Color"];
    uint32_t level = nvalue = domoticz.getUInt(PSTR("svalue1"), 0);
    uint32_t r = color.getUInt(PSTR("r"), 0) * level / 100;
    uint32_t g = color.getUInt(PSTR("g"), 0) * level / 100;
    uint32_t b = color.getUInt(PSTR("b"), 0) * level / 100;
    uint32_t cw = color.getUInt(PSTR("cw"), 0) * level / 100;
    uint32_t ww = color.getUInt(PSTR("ww"), 0) * level / 100;
    uint32_t m = color.getUInt(PSTR("m"), 0);
    uint32_t t = color.getUInt(PSTR("t"), 0);
    if (2 == m) {  // White with color temperature. Valid fields: t
      snprintf_P(XdrvMailbox.topic, XdrvMailbox.index, PSTR("/" D_CMND_BACKLOG));
      snprintf_P(XdrvMailbox.data, XdrvMailbox.data_len, PSTR(D_CMND_COLORTEMPERATURE " %d;" D_CMND_DIMMER " %d"), changeUIntScale(t, 0, 255, CT_MIN, CT_MAX), level);
    } else {
      snprintf_P(XdrvMailbox.topic, XdrvMailbox.index, PSTR("/" D_CMND_COLOR));
      snprintf_P(XdrvMailbox.data, XdrvMailbox.data_len, PSTR("%02x%02x%02x%02x%02x"), r, g, b, cw, ww);
    }
  }
  else if ((!iscolordimmer && 2 == nvalue) ||  // gswitch_sSetLevel
            (iscolordimmer && 15 == nvalue)) {  // Color_SetBrightnessLevel
    if (domoticz[PSTR("svalue1")]) {
      nvalue = domoticz.getUInt(PSTR("svalue1"), 0);
    } else {
      return true;  // Invalid data
    }
    if (TasmotaGlobal.light_type && (Settings->light_dimmer == nvalue) && ((TasmotaGlobal.power >> relay_index) &1)) {
      return true;  // State already set
    }
    snprintf_P(XdrvMailbox.topic, XdrvMailbox.index, PSTR("/" D_CMND_DIMMER));
    snprintf_P(XdrvMailbox.data, XdrvMailbox.data_len, PSTR("%d"), nvalue);
  } else
#endif  // USE_LIGHT
  if (1 == nvalue || 0 == nvalue) {
    if (((TasmotaGlobal.power >> relay_index) &1) == (power_t)nvalue) {
      return true;  // Stop loop
    }
    char stemp1[10];
    snprintf_P(XdrvMailbox.topic, XdrvMailbox.index, PSTR("/" D_CMND_POWER "%s"), (Domoticz->devices > 1) ? itoa(relay_index +1, stemp1, 10) : "");
    snprintf_P(XdrvMailbox.data, XdrvMailbox.data_len, PSTR("%d"), nvalue);
  } else {
    return true;    // No command received
  }

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_DOMOTICZ D_RECEIVED_TOPIC " %s, " D_DATA " %s"), XdrvMailbox.topic, XdrvMailbox.data);

  Domoticz->update_flag = false;
  return false;    // Process new data
}

/*********************************************************************************************/

void DomoticzSendSwitch(uint32_t type, uint32_t index, uint32_t state) {
  char stemp[16];  // "switchlight" or "switchscene"
  Response_P(PSTR("{\"command\":\"%s\",\"idx\":%d,\"switchcmd\":\"%s\"}"),
    GetTextIndexed(stemp, sizeof(stemp), type, kDomoticzCommand),
    index,
    (state) ? (POWER_TOGGLE == state) ? "Toggle" : "On" : "Off");  // Domoticz case sensitive
  MqttPublish(domoticz_in_topic);
}

bool DomoticzSendKey(uint32_t key, uint32_t device, uint32_t state, uint32_t svalflg) {
  // If ButtonTopic or SwitchTopic is set perform DomoticzSendSwitch
  if (!Domoticz) { return false; }  // No MQTT enabled or unable to allocate memory

  if (svalflg) { 
    if (key) {  // Switch
      if ((device <= Domoticz->switches) && Domoticz->Settings.switch_idx[device -1]) {
        DomoticzSendSwitch(0, Domoticz->Settings.switch_idx[device -1], state);
        return true;
      }
    } else {    // Button
      if ((device <= Domoticz->keys) && Domoticz->Settings.key_idx[device -1]) {
        DomoticzSendSwitch(0, Domoticz->Settings.key_idx[device -1], state);
        return true;
      }
    }
  }
  return false;
}

/*********************************************************************************************\
 * Sensors
 *
 * Source : https://www.domoticz.com/wiki/Domoticz_API/JSON_URL%27s
 *          https://www.domoticz.com/wiki/MQTT
 *
 * Percentage, Barometric, Air Quality:
 * {\"idx\":%d,\"nvalue\":%s}, Idx, Value
 *
 * Humidity:
 * {\"idx\":%d,\"nvalue\":%s,\"svalue\":\"%s\"}, Idx, Humidity, HumidityStatus
 *
 * All other:
 * {\"idx\":%d,\"nvalue\":0,\"svalue\":\"%s\"}, Idx, Value(s)
 *
\*********************************************************************************************/

void DomoticzSendData(uint32_t sensor_idx, uint32_t idx, char *data) {
  char payload[128];  // {"idx":26700,"nvalue":0,"svalue":"22330.1;10234.4;22000.5;10243.4;1006;3000","Battery":100,"RSSI":10}
  if (DZ_AIRQUALITY == sensor_idx) {
    snprintf_P(payload, sizeof(payload), PSTR("{\"idx\":%d,\"nvalue\":%s,\"Battery\":%d,\"RSSI\":%d}"),
      idx, data, DomoticzBatteryQuality(), DomoticzRssiQuality());
  } else {
    uint8_t nvalue = 0;
#ifdef USE_SHUTTER
    if (DZ_SHUTTER == sensor_idx) {
      uint8_t position = atoi(data);
      nvalue = position < 2 ? 0 : (position == 100 ? 1 : 2);
    }
#endif  // USE_SHUTTER
    snprintf_P(payload, sizeof(payload), DOMOTICZ_MESSAGE,  // "{\"idx\":%d,\"nvalue\":%d,\"svalue\":\"%s\",\"Battery\":%d,\"RSSI\":%d}"
      idx, nvalue, data, DomoticzBatteryQuality(), DomoticzRssiQuality());
  }
  MqttPublishPayload(domoticz_in_topic, payload);
}

void DomoticzSensor(uint8_t idx, char *data) {
  if (!Domoticz) { return; }  // No MQTT enabled or unable to allocate memory

  if (Domoticz->Settings.sensor_idx[idx]) {
    DomoticzSendData(idx, Domoticz->Settings.sensor_idx[idx], data);
  }
}

void DomoticzSensor(uint8_t idx, int value) {
  if (!Domoticz) { return; }  // No MQTT enabled or unable to allocate memory

  char data[16];
  snprintf_P(data, sizeof(data), PSTR("%d"), value);
  DomoticzSensor(idx, data);
}

void DomoticzFloatSensor(uint8_t idx, float value) {
  if (!Domoticz) { return; }  // No MQTT enabled or unable to allocate memory
  
  uint32_t resolution = 1;
/*
  switch (idx) {
    case DZ_TEMP: resolution = Settings->flag2.temperature_resolution; break;
    case DZ_POWER_ENERGY: resolution = Settings->flag2.wattage_resolution; break;
    case DZ_VOLTAGE: resolution = Settings->flag2.voltage_resolution; break;
    case DZ_CURRENT: resolution = Settings->flag2.current_resolution; break;
  }
*/
  if (DZ_TEMP == idx) { resolution = Settings->flag2.temperature_resolution; }
  else if (DZ_POWER_ENERGY == idx) { resolution = Settings->flag2.wattage_resolution; }
  else if (DZ_VOLTAGE == idx) { resolution = Settings->flag2.voltage_resolution; }
  else if (DZ_CURRENT == idx) { resolution = Settings->flag2.current_resolution; }
  char data[FLOATSZ];
  dtostrfd(value, resolution, data);
  DomoticzSensor(idx, data);
}

uint8_t DomoticzHumidityState(float h) {
  return (!h) ? 0 : (h < 40) ? 2 : (h > 70) ? 3 : 1;
}

//void DomoticzTempHumPressureSensor(float temp, float hum, float baro = -1);
void DomoticzTempHumPressureSensor(float temp, float hum, float baro) {
  if (!Domoticz) { return; }  // No MQTT enabled or unable to allocate memory

  char temperature[FLOATSZ];
  dtostrfd(temp, Settings->flag2.temperature_resolution, temperature);
  char humidity[FLOATSZ];
  dtostrfd(hum, Settings->flag2.humidity_resolution, humidity);

  char data[32];
  if (baro > -1) {
    char pressure[FLOATSZ];
    dtostrfd(baro, Settings->flag2.pressure_resolution, pressure);

    snprintf_P(data, sizeof(data), PSTR("%s;%s;%d;%s;5"), temperature, humidity, DomoticzHumidityState(hum), pressure);
    DomoticzSensor(DZ_TEMP_HUM_BARO, data);
  } else {
    snprintf_P(data, sizeof(data), PSTR("%s;%s;%d"), temperature, humidity, DomoticzHumidityState(hum));
    DomoticzSensor(DZ_TEMP_HUM, data);
  }
}

void DomoticzSensorPowerEnergy(int power, char *energy) {
  if (!Domoticz) { return; }  // No MQTT enabled or unable to allocate memory

  char data[16];
  snprintf_P(data, sizeof(data), PSTR("%d;%s"), power, energy);
  DomoticzSensor(DZ_POWER_ENERGY, data);
}

void DomoticzSensorP1SmartMeter(char *usage1, char *usage2, char *return1, char *return2, int power) {
  //usage1   = energy usage meter tariff 1, This is an incrementing counter
  //usage2   = energy usage meter tariff 2, This is an incrementing counter
  //return1  = energy return meter tariff 1, This is an incrementing counter
  //return2  = energy return meter tariff 2, This is an incrementing counter
  //power    = if >= 0 actual usage power. if < 0 actual return power (Watt)
  if (!Domoticz) { return; }  // No MQTT enabled or unable to allocate memory

  int consumed = power;
  int produced = 0;
  if (power < 0) {
    consumed = 0;
    produced = -power;
  }
  char data[64];
  snprintf_P(data, sizeof(data), PSTR("%s;%s;%s;%s;%d;%d"), usage1, usage2, return1, return2, consumed, produced);
  DomoticzSensor(DZ_P1_SMART_METER, data);
}

/*********************************************************************************************/

void DomoticzInit(void) {
  if (Settings->flag.mqtt_enabled) {  // SetOption3 - Enable MQTT
    Domoticz = (Domoticz_t*)calloc(1, sizeof(Domoticz_t));  // Need calloc to reset registers to 0/false
    if (nullptr == Domoticz) { return; }

    Domoticz->devices = TasmotaGlobal.devices_present;
    if (Domoticz->devices) {
      Domoticz->Settings.relay_idx = (uintdz_t*)calloc(Domoticz->devices, sizeof(uintdz_t));
      if (nullptr == Domoticz->Settings.relay_idx) { return; }
      for (uint32_t i = 0; i < Domoticz->devices; i++) {
        if (ButtonUsed(i)) { Domoticz->keys++; }
        if (SwitchUsed(i)) { Domoticz->switches++; }
      }
      if (Domoticz->keys) {
        Domoticz->Settings.key_idx = (uintdz_t*)calloc(Domoticz->keys, sizeof(uintdz_t));
        if (nullptr == Domoticz->Settings.key_idx) { return; }
      }
      if (Domoticz->switches) {
        Domoticz->Settings.switch_idx = (uintdz_t*)calloc(Domoticz->switches, sizeof(uintdz_t));
        if (nullptr == Domoticz->Settings.switch_idx) { return; }
      }
    }

    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DOMOTICZ "Support %d Device(s), %d Button(s), %d Switch(es) and %d Sensors"),
      Domoticz->devices, Domoticz->keys, Domoticz->switches, DZ_MAX_SENSORS);

    DomoticzSettingsLoad(0);
    Domoticz->update_flag = true;
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndDomoticzIdx(void) {
  // DzIdx0 0    - Reset all disabling subscription too
  // DzIdx1 403  - Relate relay1 (=Power1) to Domoticz Idx 403 persistent
  // DzIdx5 403  - Relate relay5 (=Power5) to Domoticz Idx 403 non-persistent (need a rule at boot to become persistent)
  if ((XdrvMailbox.index >= 0) && (XdrvMailbox.index <= Domoticz->devices)) {
    if (XdrvMailbox.payload >= 0) {
      if (0 == XdrvMailbox.index) {
        for (uint32_t i = 0; i < Domoticz->devices; i++) {
          DomoticzSetRelayIdx(i, 0);
        }
      } else {
        DomoticzSetRelayIdx(XdrvMailbox.index -1, XdrvMailbox.payload);
      }
      DomoticzMqttSubscribe();
    }
    ResponseCmndIdxNumber(DomoticzRelayIdx(XdrvMailbox.index -1));
  }
}

void CmndDomoticzKeyIdx(void) {
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= Domoticz->keys)) {
    if (XdrvMailbox.payload >= 0) {
      Domoticz->Settings.key_idx[XdrvMailbox.index -1] = XdrvMailbox.payload;
    }
    ResponseCmndIdxNumber(Domoticz->Settings.key_idx[XdrvMailbox.index -1]);
  }
}

void CmndDomoticzSwitchIdx(void) {
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= Domoticz->switches)) {
    if (XdrvMailbox.payload >= 0) {
      Domoticz->Settings.switch_idx[XdrvMailbox.index -1] = XdrvMailbox.payload;
    }
    ResponseCmndIdxNumber(Domoticz->Settings.switch_idx[XdrvMailbox.index -1]);
  }
}

void CmndDomoticzSensorIdx(void) {
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= DZ_MAX_SENSORS)) {
    if (XdrvMailbox.payload >= 0) {
      Domoticz->Settings.sensor_idx[XdrvMailbox.index -1] = XdrvMailbox.payload;
    }
    ResponseCmndIdxNumber(Domoticz->Settings.sensor_idx[XdrvMailbox.index -1]);
  }
}

void CmndDomoticzUpdateTimer(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
    Domoticz->Settings.update_timer = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Domoticz->Settings.update_timer);
}

void CmndDomoticzSend(void) {
  /*
  DzSend1 <idx>,<values> - {\"idx\":<idx>,\"nvalue\":0,\"svalue\":\"<values>\",\"Battery\":xx,\"RSSI\":yy}
    rule1 on power1#state do dzsend1 9001,%value% endon
  DzSend1 418,%var1%;%var2% or DzSend1 418,%var1%:%var2% - Notice colon as substitute to semi-colon
  DzSend2 <idx>,<values> - USE_SHUTTER only - {\"idx\":<idx>,\"nvalue\":<position>,\"svalue\":\"<values>\",\"Battery\":xx,\"RSSI\":yy}
  DzSend3 <idx>,<values> - {\"idx\":<idx>,\"nvalue\":<values>,\"Battery\":xx,\"RSSI\":yy}
  DzSend4 <idx>,<state>  - {\"command\":\"switchlight\",\"idx\":<idx>,\"switchcmd\":\"<state>\"}
  DzSend5 <idx>,<state>  - {\"command\":\"switchscene\",\"idx\":<idx>,\"switchcmd\":\"<state>\"}
  */
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 5)) {
    if (XdrvMailbox.data_len > 0) {
      if (strchr(XdrvMailbox.data, ',') != nullptr) {  // Process parameter entry
        char *data;
        uint32_t index = strtoul(strtok_r(XdrvMailbox.data, ",", &data), nullptr, 10);
        if ((index > 0) && (data != nullptr)) {
          ReplaceChar(data,':',';');  // As a workaround for command backlog inter-command separator
          if (XdrvMailbox.index > 3) {
            uint32_t state = strtoul(data, nullptr, 10);  // 0, 1 or 2
            DomoticzSendSwitch(XdrvMailbox.index -4, index, state);
          } else {
            uint32_t type = DZ_TEMP;
            if (2 == XdrvMailbox.index) { type = DZ_SHUTTER; }
            else if (3 == XdrvMailbox.index) { type = DZ_AIRQUALITY; }
            DomoticzSendData(type, index, data);
          }
          ResponseCmndDone();
        }
      }
    }
  }
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

#ifdef USE_WEBSERVER

#define WEB_HANDLE_DOMOTICZ "dm"

const char HTTP_BTN_MENU_DOMOTICZ[] PROGMEM =
  "<p></p><form action='" WEB_HANDLE_DOMOTICZ "' method='get'><button>" D_CONFIGURE_DOMOTICZ "</button></form>";

const char HTTP_FORM_DOMOTICZ[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_DOMOTICZ_PARAMETERS "&nbsp;</b></legend>"
  "<form method='post' action='" WEB_HANDLE_DOMOTICZ "'>"
  "<table>"
  "<tr><td style='width:116px'></td><td style='width:70px'><b>%s</b></td><td style='width:70px'><b>%s</b></td><td style='width:70px'></td></tr>";
const char HTTP_FORM_DOMOTICZ_INDEX[] PROGMEM =
  "<tr><td><b>" D_DOMOTICZ_IDX " %d</b></td><td>";
const char HTTP_FORM_DOMOTICZ_INPUT[] PROGMEM =
  "<input id='%c%d' placeholder='0' value='%d'>";
const char HTTP_FORM_DOMOTICZ_SENSOR[] PROGMEM =
  "<tr><td colspan='3'><b>" D_DOMOTICZ_SENSOR_IDX " %d</b> %s</td><td>";
const char HTTP_FORM_DOMOTICZ_TIMER[] PROGMEM =
  "<tr><td colspan='3'><b>" D_DOMOTICZ_UPDATE_TIMER "</b> (" STR(DOMOTICZ_UPDATE_TIMER) ")</td>"
  "<td><input id='ut' placeholder='" STR(DOMOTICZ_UPDATE_TIMER) "' value='%d'></td></tr>";

void HandleDomoticzConfiguration(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_CONFIGURE_DOMOTICZ));

  if (Webserver->hasArg(F("save"))) {
    DomoticzSaveSettings();
    HandleConfiguration();
    return;
  }

  WSContentStart_P(PSTR(D_CONFIGURE_DOMOTICZ));
  WSContentSendStyle();
  WSContentSend_P(HTTP_FORM_DOMOTICZ, (Domoticz->switches)? D_DOMOTICZ_SWITCH :"", (Domoticz->keys)? D_DOMOTICZ_KEY :"");
  for (uint32_t i = 0; i < Domoticz->devices; i++) {
    WSContentSend_P(HTTP_FORM_DOMOTICZ_INDEX, i +1);
    if (i < Domoticz->switches) {
      WSContentSend_P(HTTP_FORM_DOMOTICZ_INPUT, 's', i, Domoticz->Settings.switch_idx[i]);
    }
    WSContentSend_P(PSTR("</td><td>"));
    if (i < Domoticz->keys) {
      WSContentSend_P(HTTP_FORM_DOMOTICZ_INPUT, 'k', i, Domoticz->Settings.key_idx[i]);
    }
    WSContentSend_P(PSTR("</td><td>"));
    WSContentSend_P(HTTP_FORM_DOMOTICZ_INPUT, 'r', i, Domoticz->Settings.relay_idx[i]);
    WSContentSend_P(PSTR("</td></tr>"));
#ifdef USE_SONOFF_IFAN
    if (IsModuleIfan() && (1 == i)) { break; }
#endif  // USE_SONOFF_IFAN
  }
  char stemp[40];
  for (uint32_t i = 0; i < DZ_MAX_SENSORS; i++) {
    WSContentSend_P(HTTP_FORM_DOMOTICZ_SENSOR, i +1, GetTextIndexed(stemp, sizeof(stemp), i, kDomoticzSensors));
    WSContentSend_P(HTTP_FORM_DOMOTICZ_INPUT, 'l', i, Domoticz->Settings.sensor_idx[i]);
    WSContentSend_P(PSTR("</td></tr>"));
  }
  WSContentSend_P(HTTP_FORM_DOMOTICZ_TIMER, Domoticz->Settings.update_timer);
  WSContentSend_P(PSTR("</table>"));
  WSContentSend_P(HTTP_FORM_END);
  WSContentSpaceButton(BUTTON_CONFIGURATION);
  WSContentStop();
}

String DomoticzAddWebCommand(const char* command, const char* arg, uint32_t value) {
  char tmp[8];                       // WebGetArg numbers only
  WebGetArg(arg, tmp, sizeof(tmp));
  if (!strlen(tmp)) { strcpy(tmp, "0"); }
  if (atoi(tmp) == value) { return ""; }
  return AddWebCommand(command, arg, PSTR("0"));
}

void DomoticzSaveSettings(void) {
  String cmnd = F(D_CMND_BACKLOG "0 ");
  cmnd += AddWebCommand(PSTR(D_PRFX_DOMOTICZ D_CMND_UPDATETIMER), PSTR("ut"), STR(DOMOTICZ_UPDATE_TIMER));
  char arg_idx[5];
  char cmnd2[24];
  for (uint32_t i = 0; i < Domoticz->devices; i++) {
    snprintf_P(cmnd2, sizeof(cmnd2), PSTR(D_PRFX_DOMOTICZ D_CMND_IDX "%d"), i +1);
    snprintf_P(arg_idx, sizeof(arg_idx), PSTR("r%d"), i);
    cmnd += DomoticzAddWebCommand(cmnd2, arg_idx, Domoticz->Settings.relay_idx[i]);
    if (i < Domoticz->keys) {
      snprintf_P(cmnd2, sizeof(cmnd2), PSTR(D_PRFX_DOMOTICZ D_CMND_KEYIDX "%d"), i +1);
      arg_idx[0] = 'k';
      cmnd += DomoticzAddWebCommand(cmnd2, arg_idx, Domoticz->Settings.key_idx[i]);
    }
    if (i < Domoticz->switches) {
      snprintf_P(cmnd2, sizeof(cmnd2), PSTR(D_PRFX_DOMOTICZ D_CMND_SWITCHIDX "%d"), i +1);
      arg_idx[0] = 's';
      cmnd += DomoticzAddWebCommand(cmnd2, arg_idx, Domoticz->Settings.switch_idx[i]);
    }
  }
  for (uint32_t i = 0; i < DZ_MAX_SENSORS; i++) {
    snprintf_P(cmnd2, sizeof(cmnd2), PSTR(D_PRFX_DOMOTICZ D_CMND_SENSORIDX "%d"), i +1);
    snprintf_P(arg_idx, sizeof(arg_idx), PSTR("l%d"), i);
    cmnd += DomoticzAddWebCommand(cmnd2, arg_idx, Domoticz->Settings.sensor_idx[i]);
  }
  ExecuteWebCommand((char*)cmnd.c_str());
}
#endif  // USE_WEBSERVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv07(uint32_t function) {
  bool result = false;

  if (FUNC_INIT == function) {
    DomoticzInit();
  }
  else if (Domoticz) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        DomoticzMqttUpdate();
        break;
      case FUNC_MQTT_DATA:
        result = DomoticzMqttData();
        break;
      case FUNC_RESET_SETTINGS:
        DomoticzSettingsLoad(1);
        break;
      case FUNC_SAVE_SETTINGS:
        DomoticzSettingsSave();
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_ADD_BUTTON:
        WSContentSend_P(HTTP_BTN_MENU_DOMOTICZ);
        break;
      case FUNC_WEB_ADD_HANDLER:
        WebServer_on(PSTR("/" WEB_HANDLE_DOMOTICZ), HandleDomoticzConfiguration);
        break;
#endif  // USE_WEBSERVER
      case FUNC_MQTT_SUBSCRIBE:
        DomoticzMqttSubscribe();
#ifdef USE_SHUTTER
        if (Domoticz->Settings.sensor_idx[DZ_SHUTTER]) { 
          Domoticz->is_shutter = true;
        }
#endif // USE_SHUTTER
        break;
      case FUNC_MQTT_INIT:
        Domoticz->update_timer = 2;
        break;
      case FUNC_SHOW_SENSOR:
//        DomoticzSendSensor();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kDomoticzCommands, DomoticzCommand);
        break;
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif  // USE_UFILESYS
#endif  // USE_DOMOTICZ
