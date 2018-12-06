/*
  xdrv_07_domoticz.ino - domoticz support for Sonoff-Tasmota

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

#ifdef USE_DOMOTICZ

#define XDRV_07             7

const char DOMOTICZ_MESSAGE[] PROGMEM = "{\"idx\":%d,\"nvalue\":%d,\"svalue\":\"%s\",\"Battery\":%d,\"RSSI\":%d}";

enum DomoticzCommands { CMND_IDX, CMND_KEYIDX, CMND_SWITCHIDX, CMND_SENSORIDX, CMND_UPDATETIMER };
const char kDomoticzCommands[] PROGMEM = D_CMND_IDX "|" D_CMND_KEYIDX "|" D_CMND_SWITCHIDX "|" D_CMND_SENSORIDX "|" D_CMND_UPDATETIMER ;

//enum DomoticzSensors {DZ_TEMP, DZ_TEMP_HUM, DZ_TEMP_HUM_BARO, DZ_POWER_ENERGY, DZ_ILLUMINANCE, DZ_COUNT, DZ_VOLTAGE, DZ_CURRENT, DZ_AIRQUALITY, DZ_MAX_SENSORS};

#if MAX_DOMOTICZ_SNS_IDX < DZ_MAX_SENSORS
  #error "Domoticz: Too many sensors or change settings.h layout"
#endif

const char kDomoticzSensors[] PROGMEM =
  D_DOMOTICZ_TEMP "|" D_DOMOTICZ_TEMP_HUM "|" D_DOMOTICZ_TEMP_HUM_BARO "|" D_DOMOTICZ_POWER_ENERGY "|" D_DOMOTICZ_ILLUMINANCE "|" D_DOMOTICZ_COUNT "|" D_DOMOTICZ_VOLTAGE "|" D_DOMOTICZ_CURRENT "|" D_DOMOTICZ_AIRQUALITY ;

const char S_JSON_DOMOTICZ_COMMAND_INDEX_NVALUE[] PROGMEM = "{\"" D_CMND_DOMOTICZ "%s%d\":%d}";
const char S_JSON_DOMOTICZ_COMMAND_INDEX_LVALUE[] PROGMEM = "{\"" D_CMND_DOMOTICZ "%s%d\":%lu}";

char domoticz_in_topic[] = DOMOTICZ_IN_TOPIC;
char domoticz_out_topic[] = DOMOTICZ_OUT_TOPIC;

boolean domoticz_subscribe = false;
byte domoticz_update_flag = 1;
int domoticz_update_timer = 0;
unsigned long fan_debounce = 0;             // iFan02 state debounce timer

int DomoticzBatteryQuality(void)
{
  // Battery 0%: ESP 2.6V (minimum operating voltage is 2.5)
  // Battery 100%: ESP 3.6V (maximum operating voltage is 3.6)
  // Battery 101% to 200%: ESP over 3.6V (means over maximum operating voltage)

  int quality = 0;	// Voltage range from 2,6V > 0%  to 3,6V > 100%

  uint16_t voltage = ESP.getVcc();
  if (voltage <= 2600) {
    quality = 0;
  } else if (voltage >= 4600) {
    quality = 200;
  } else {
    quality = (voltage - 2600) / 10;
  }
  return quality;
}

int DomoticzRssiQuality(void)
{
  // RSSI range: 0% to 10% (12 means disable RSSI in Domoticz)

  return WifiGetRssiAsQuality(WiFi.RSSI()) / 10;
}

void MqttPublishDomoticzFanState()
{
  if (Settings.flag.mqtt_enabled && Settings.domoticz_relay_idx[1]) {
    char svalue[8];  // Fanspeed value

    int fan_speed = GetFanspeed();
    snprintf_P(svalue, sizeof(svalue), PSTR("%d"), fan_speed * 10);
    snprintf_P(mqtt_data, sizeof(mqtt_data), DOMOTICZ_MESSAGE,
      Settings.domoticz_relay_idx[1], (0 == fan_speed) ? 0 : 2, svalue, DomoticzBatteryQuality(), DomoticzRssiQuality());
    MqttPublish(domoticz_in_topic);

    fan_debounce = millis();
  }
}

void DomoticzUpdateFanState()
{
  if (domoticz_update_flag) {
    MqttPublishDomoticzFanState();
  }
  domoticz_update_flag = 1;
}

void MqttPublishDomoticzPowerState(byte device)
{
  if (Settings.flag.mqtt_enabled) {
    if ((device < 1) || (device > devices_present)) { device = 1; }
    if (Settings.domoticz_relay_idx[device -1]) {
      if ((SONOFF_IFAN02 == Settings.module) && (device > 1)) {
        // Fan handled by MqttPublishDomoticzFanState
      } else {
        char svalue[8];  // Dimmer value

        snprintf_P(svalue, sizeof(svalue), PSTR("%d"), Settings.light_dimmer);
        snprintf_P(mqtt_data, sizeof(mqtt_data), DOMOTICZ_MESSAGE,
          Settings.domoticz_relay_idx[device -1], (power & (1 << (device -1))) ? 1 : 0, (light_type) ? svalue : "", DomoticzBatteryQuality(), DomoticzRssiQuality());
        MqttPublish(domoticz_in_topic);
      }
    }
  }
}

void DomoticzUpdatePowerState(byte device)
{
  if (domoticz_update_flag) {
    MqttPublishDomoticzPowerState(device);
  }
  domoticz_update_flag = 1;
}

void DomoticzMqttUpdate(void)
{
  if (domoticz_subscribe && (Settings.domoticz_update_timer || domoticz_update_timer)) {
    domoticz_update_timer--;
    if (domoticz_update_timer <= 0) {
      domoticz_update_timer = Settings.domoticz_update_timer;
      for (byte i = 1; i <= devices_present; i++) {
        if ((SONOFF_IFAN02 == Settings.module) && (i > 1)) {
          MqttPublishDomoticzFanState();
          break;
        } else {
          MqttPublishDomoticzPowerState(i);
        }
      }
    }
  }
}

void DomoticzMqttSubscribe(void)
{
  uint8_t maxdev = (devices_present > MAX_DOMOTICZ_IDX) ? MAX_DOMOTICZ_IDX : devices_present;
  for (byte i = 0; i < maxdev; i++) {
    if (Settings.domoticz_relay_idx[i]) {
      domoticz_subscribe = true;
    }
  }
  if (domoticz_subscribe) {
    char stopic[TOPSZ];
    snprintf_P(stopic, sizeof(stopic), PSTR("%s/#"), domoticz_out_topic); // domoticz topic
    MqttSubscribe(stopic);
  }
}

/*
 * ArduinoJSON Domoticz Switch entry used to calculate jsonBuf: JSON_OBJECT_SIZE(11) + 129 = 313
{
   "Battery" : 255,
   "RSSI" : 12,
   "dtype" : "Light/Switch",
   "id" : "000140E7",
   "idx" : 159,
   "name" : "Sonoff1",
   "nvalue" : 1,
   "stype" : "Switch",
   "svalue1" : "0",
   "switchType" : "Dimmer",
   "unit" : 1
}
 * Fail on this one
{
   "LastUpdate" : "2018-10-02 20:39:45",
   "Name" : "Sfeerverlichting",
   "Status" : "Off",
   "Timers" : "true",
   "Type" : "Group",
   "idx" : "2"
}
*/

boolean DomoticzMqttData(void)
{
  char stemp1[10];
  unsigned long idx = 0;
  int16_t nvalue = -1;
  int16_t found = 0;

  domoticz_update_flag = 1;
  if (!strncmp(XdrvMailbox.topic, domoticz_out_topic, strlen(domoticz_out_topic))) {
    if (XdrvMailbox.data_len < 20) {
      return 1;
    }
    StaticJsonBuffer<400> jsonBuf;
    JsonObject& domoticz = jsonBuf.parseObject(XdrvMailbox.data);
    if (!domoticz.success()) {
      return 1;
    }
//    if (strcmp_P(domoticz["dtype"],PSTR("Light/Switch"))) {
//      return 1;
//    }
    idx = domoticz["idx"];
    if (domoticz.containsKey("nvalue")) {
      nvalue = domoticz["nvalue"];
    }

    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DOMOTICZ "idx %d, nvalue %d"), idx, nvalue);
    AddLog(LOG_LEVEL_DEBUG_MORE);

    if ((idx > 0) && (nvalue >= 0) && (nvalue <= 15)) {
      uint8_t maxdev = (devices_present > MAX_DOMOTICZ_IDX) ? MAX_DOMOTICZ_IDX : devices_present;
      for (byte i = 0; i < maxdev; i++) {
        if (idx == Settings.domoticz_relay_idx[i]) {
          bool iscolordimmer = strcmp_P(domoticz["dtype"],PSTR("Color Switch")) == 0;
          snprintf_P(stemp1, sizeof(stemp1), PSTR("%d"), i +1);
          if ((SONOFF_IFAN02 == Settings.module) && (1 == i)) {  // Idx 2 is fanspeed
            uint8_t svalue = 0;
            if (domoticz.containsKey("svalue1")) {
              svalue = domoticz["svalue1"];
            } else {
              return 1;
            }
            svalue = (nvalue == 2) ? svalue / 10 : 0;
            if (GetFanspeed() == svalue) {
              return 1;  // Stop loop as already set
            }
            if (TimePassedSince(fan_debounce) < 1000) {
              return 1;  // Stop loop if device in limbo
            }
            snprintf_P(XdrvMailbox.topic, XdrvMailbox.index, PSTR("/" D_CMND_FANSPEED));
            snprintf_P(XdrvMailbox.data, XdrvMailbox.data_len, PSTR("%d"), svalue);
            found = 1;
          }
          else if (iscolordimmer && 10 == nvalue) { // Color_SetColor
            JsonObject& color = domoticz["Color"];
            uint16_t level = nvalue = domoticz["svalue1"];
            uint16_t r = color["r"]; r = r * level / 100;
            uint16_t g = color["g"]; g = g * level / 100;
            uint16_t b = color["b"]; b = b * level / 100;
            uint16_t cw = color["cw"]; cw = cw * level / 100;
            uint16_t ww = color["ww"]; ww = ww * level / 100;
            snprintf_P(XdrvMailbox.topic, XdrvMailbox.index, PSTR("/" D_CMND_COLOR));
            snprintf_P(XdrvMailbox.data, XdrvMailbox.data_len, PSTR("%02x%02x%02x%02x%02x"), r, g, b, cw, ww);
            found = 1;
          }
          else if ((!iscolordimmer && 2 == nvalue) || // gswitch_sSetLevel
                   (iscolordimmer && 15 == nvalue)) { // Color_SetBrightnessLevel
            if (domoticz.containsKey("svalue1")) {
              nvalue = domoticz["svalue1"];
            } else {
              return 1;
            }
            if (light_type && (Settings.light_dimmer == nvalue) && ((power >> i) &1)) {
              return 1;
            }
            snprintf_P(XdrvMailbox.topic, XdrvMailbox.index, PSTR("/" D_CMND_DIMMER));
            snprintf_P(XdrvMailbox.data, XdrvMailbox.data_len, PSTR("%d"), nvalue);
            found = 1;
          }
          else if (1 == nvalue || 0 == nvalue) {
            if (((power >> i) &1) == (power_t)nvalue) {
              return 1;  // Stop loop
            }
            snprintf_P(XdrvMailbox.topic, XdrvMailbox.index, PSTR("/" D_CMND_POWER "%s"), (devices_present > 1) ? stemp1 : "");
            snprintf_P(XdrvMailbox.data, XdrvMailbox.data_len, PSTR("%d"), nvalue);
            found = 1;
          }
          break;
        }
      }
    }
    if (!found) {
      return 1;
    }

    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DOMOTICZ D_RECEIVED_TOPIC " %s, " D_DATA " %s"), XdrvMailbox.topic, XdrvMailbox.data);
    AddLog(LOG_LEVEL_DEBUG_MORE);

    domoticz_update_flag = 0;
  }
  return 0;
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

boolean DomoticzCommand(void)
{
  char command [CMDSZ];
  boolean serviced = true;
  uint8_t dmtcz_len = strlen(D_CMND_DOMOTICZ);  // Prep for string length change

  if (!strncasecmp_P(XdrvMailbox.topic, PSTR(D_CMND_DOMOTICZ), dmtcz_len)) {  // Prefix
    int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic +dmtcz_len, kDomoticzCommands);
    if (-1 == command_code) {
      serviced = false;  // Unknown command
    }
    else if ((CMND_IDX == command_code) && (XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_DOMOTICZ_IDX)) {
      if (XdrvMailbox.payload >= 0) {
        Settings.domoticz_relay_idx[XdrvMailbox.index -1] = XdrvMailbox.payload;
        restart_flag = 2;
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_DOMOTICZ_COMMAND_INDEX_LVALUE, command, XdrvMailbox.index, Settings.domoticz_relay_idx[XdrvMailbox.index -1]);
    }
    else if ((CMND_KEYIDX == command_code) && (XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_DOMOTICZ_IDX)) {
      if (XdrvMailbox.payload >= 0) {
        Settings.domoticz_key_idx[XdrvMailbox.index -1] = XdrvMailbox.payload;
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_DOMOTICZ_COMMAND_INDEX_LVALUE, command, XdrvMailbox.index, Settings.domoticz_key_idx[XdrvMailbox.index -1]);
    }
    else if ((CMND_SWITCHIDX == command_code) && (XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_DOMOTICZ_IDX)) {
      if (XdrvMailbox.payload >= 0) {
        Settings.domoticz_switch_idx[XdrvMailbox.index -1] = XdrvMailbox.payload;
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_DOMOTICZ_COMMAND_INDEX_NVALUE, command, XdrvMailbox.index, Settings.domoticz_switch_idx[XdrvMailbox.index -1]);
    }
    else if ((CMND_SENSORIDX == command_code) && (XdrvMailbox.index > 0) && (XdrvMailbox.index <= DZ_MAX_SENSORS)) {
      if (XdrvMailbox.payload >= 0) {
        Settings.domoticz_sensor_idx[XdrvMailbox.index -1] = XdrvMailbox.payload;
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_DOMOTICZ_COMMAND_INDEX_NVALUE, command, XdrvMailbox.index, Settings.domoticz_sensor_idx[XdrvMailbox.index -1]);
    }
    else if (CMND_UPDATETIMER == command_code) {
      if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
        Settings.domoticz_update_timer = XdrvMailbox.payload;
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_DOMOTICZ "%s\":%d}"), command, Settings.domoticz_update_timer);
    }
    else serviced = false;  // Unknown command
  }
  else serviced = false;  // Unknown command

  return serviced;
}

boolean DomoticzSendKey(byte key, byte device, byte state, byte svalflg)
{
  boolean result = 0;

  if (device <= MAX_DOMOTICZ_IDX) {
    if ((Settings.domoticz_key_idx[device -1] || Settings.domoticz_switch_idx[device -1]) && (svalflg)) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"command\":\"switchlight\",\"idx\":%d,\"switchcmd\":\"%s\"}"),
        (key) ? Settings.domoticz_switch_idx[device -1] : Settings.domoticz_key_idx[device -1], (state) ? (2 == state) ? "Toggle" : "On" : "Off");
      MqttPublish(domoticz_in_topic);
      result = 1;
    }
  }
  return result;
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

uint8_t DomoticzHumidityState(char *hum)
{
  uint8_t h = atoi(hum);
  return (!h) ? 0 : (h < 40) ? 2 : (h > 70) ? 3 : 1;
}

void DomoticzSensor(byte idx, char *data)
{
  if (Settings.domoticz_sensor_idx[idx]) {
    char dmess[90];

    memcpy(dmess, mqtt_data, sizeof(dmess));
    if (DZ_AIRQUALITY == idx) {
      snprintf_P(mqtt_data, sizeof(dmess), PSTR("{\"idx\":%d,\"nvalue\":%s,\"Battery\":%d,\"RSSI\":%d}"),
        Settings.domoticz_sensor_idx[idx], data, DomoticzBatteryQuality(), DomoticzRssiQuality());
    } else {
      snprintf_P(mqtt_data, sizeof(dmess), DOMOTICZ_MESSAGE,
        Settings.domoticz_sensor_idx[idx], 0, data, DomoticzBatteryQuality(), DomoticzRssiQuality());
    }
    MqttPublish(domoticz_in_topic);
    memcpy(mqtt_data, dmess, sizeof(dmess));
  }
}

void DomoticzSensor(byte idx, uint32_t value)
{
  char data[16];
  snprintf_P(data, sizeof(data), PSTR("%d"), value);
  DomoticzSensor(idx, data);
}

void DomoticzTempHumSensor(char *temp, char *hum)
{
  char data[16];
  snprintf_P(data, sizeof(data), PSTR("%s;%s;%d"), temp, hum, DomoticzHumidityState(hum));
  DomoticzSensor(DZ_TEMP_HUM, data);
}

void DomoticzTempHumPressureSensor(char *temp, char *hum, char *baro)
{
  char data[32];
  snprintf_P(data, sizeof(data), PSTR("%s;%s;%d;%s;5"), temp, hum, DomoticzHumidityState(hum), baro);
  DomoticzSensor(DZ_TEMP_HUM_BARO, data);
}

void DomoticzSensorPowerEnergy(int power, char *energy)
{
  char data[16];
  snprintf_P(data, sizeof(data), PSTR("%d;%s"), power, energy);
  DomoticzSensor(DZ_POWER_ENERGY, data);
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

#ifdef USE_WEBSERVER

#define WEB_HANDLE_DOMOTICZ "dm"

const char S_CONFIGURE_DOMOTICZ[] PROGMEM = D_CONFIGURE_DOMOTICZ;

const char HTTP_BTN_MENU_DOMOTICZ[] PROGMEM =
  "<br/><form action='" WEB_HANDLE_DOMOTICZ "' method='get'><button>" D_CONFIGURE_DOMOTICZ "</button></form>";

const char HTTP_FORM_DOMOTICZ[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_DOMOTICZ_PARAMETERS "&nbsp;</b></legend><form method='post' action='" WEB_HANDLE_DOMOTICZ "'>"
  "<br/><table>";
const char HTTP_FORM_DOMOTICZ_RELAY[] PROGMEM =
  "<tr><td style='width:260px'><b>" D_DOMOTICZ_IDX " {1</b></td><td style='width:70px'><input id='r{1' name='r{1' placeholder='0' value='{2'></td></tr>"
  "<tr><td style='width:260px'><b>" D_DOMOTICZ_KEY_IDX " {1</b></td><td style='width:70px'><input id='k{1' name='k{1' placeholder='0' value='{3'></td></tr>";
  const char HTTP_FORM_DOMOTICZ_SWITCH[] PROGMEM =
  "<tr><td style='width:260px'><b>" D_DOMOTICZ_SWITCH_IDX " {1</b></td><td style='width:70px'><input id='s{1' name='s{1' placeholder='0' value='{4'></td></tr>";
const char HTTP_FORM_DOMOTICZ_SENSOR[] PROGMEM =
  "<tr><td style='width:260px'><b>" D_DOMOTICZ_SENSOR_IDX " {1</b> {2</td><td style='width:70px'><input id='l{1' name='l{1' placeholder='0' value='{5'></td></tr>";
const char HTTP_FORM_DOMOTICZ_TIMER[] PROGMEM =
  "<tr><td style='width:260px'><b>" D_DOMOTICZ_UPDATE_TIMER "</b> (" STR(DOMOTICZ_UPDATE_TIMER) ")</td><td style='width:70px'><input id='ut' name='ut' placeholder='" STR(DOMOTICZ_UPDATE_TIMER) "' value='{6'</td></tr>";

void HandleDomoticzConfiguration(void)
{
  if (HttpUser()) { return; }
  if (!WebAuthenticate()) { return WebServer->requestAuthentication(); }
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURE_DOMOTICZ);

  if (WebServer->hasArg("save")) {
    DomoticzSaveSettings();
    WebRestart(1);
    return;
  }

  char stemp[32];

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_CONFIGURE_DOMOTICZ));
  page += FPSTR(HTTP_HEAD_STYLE);
  page += FPSTR(HTTP_FORM_DOMOTICZ);
  for (int i = 0; i < MAX_DOMOTICZ_IDX; i++) {
    if (i < devices_present) {
      page += FPSTR(HTTP_FORM_DOMOTICZ_RELAY);
      page.replace("{2", String((int)Settings.domoticz_relay_idx[i]));
      page.replace("{3", String((int)Settings.domoticz_key_idx[i]));
    }
    if (pin[GPIO_SWT1 +i] < 99) {
      page += FPSTR(HTTP_FORM_DOMOTICZ_SWITCH);
      page.replace("{4", String((int)Settings.domoticz_switch_idx[i]));
    }
    page.replace("{1", String(i +1));
    if ((SONOFF_IFAN02 == Settings.module) && (1 == i)) { break; }
  }
  for (int i = 0; i < DZ_MAX_SENSORS; i++) {
    page += FPSTR(HTTP_FORM_DOMOTICZ_SENSOR);
    page.replace("{1", String(i +1));
    page.replace("{2", GetTextIndexed(stemp, sizeof(stemp), i, kDomoticzSensors));
    page.replace("{5", String((int)Settings.domoticz_sensor_idx[i]));
  }
  page += FPSTR(HTTP_FORM_DOMOTICZ_TIMER);
  page.replace("{6", String((int)Settings.domoticz_update_timer));
  page += F("</table>");
  page += FPSTR(HTTP_FORM_END);
  page += FPSTR(HTTP_BTN_CONF);
  ShowPage(page);
}

void DomoticzSaveSettings(void)
{
  char stemp[20];
  char ssensor_indices[6 * MAX_DOMOTICZ_SNS_IDX];
  char tmp[100];

  for (byte i = 0; i < MAX_DOMOTICZ_IDX; i++) {
    snprintf_P(stemp, sizeof(stemp), PSTR("r%d"), i +1);
    WebGetArg(stemp, tmp, sizeof(tmp));
    Settings.domoticz_relay_idx[i] = (!strlen(tmp)) ? 0 : atoi(tmp);
    snprintf_P(stemp, sizeof(stemp), PSTR("k%d"), i +1);
    WebGetArg(stemp, tmp, sizeof(tmp));
    Settings.domoticz_key_idx[i] = (!strlen(tmp)) ? 0 : atoi(tmp);
    snprintf_P(stemp, sizeof(stemp), PSTR("s%d"), i +1);
    WebGetArg(stemp, tmp, sizeof(tmp));
    Settings.domoticz_switch_idx[i] = (!strlen(tmp)) ? 0 : atoi(tmp);
  }
  ssensor_indices[0] = '\0';
  for (byte i = 0; i < DZ_MAX_SENSORS; i++) {
    snprintf_P(stemp, sizeof(stemp), PSTR("l%d"), i +1);
    WebGetArg(stemp, tmp, sizeof(tmp));
    Settings.domoticz_sensor_idx[i] = (!strlen(tmp)) ? 0 : atoi(tmp);
    snprintf_P(ssensor_indices, sizeof(ssensor_indices), PSTR("%s%s%d"), ssensor_indices, (strlen(ssensor_indices)) ? "," : "",  Settings.domoticz_sensor_idx[i]);
  }
  WebGetArg("ut", tmp, sizeof(tmp));
  Settings.domoticz_update_timer = (!strlen(tmp)) ? DOMOTICZ_UPDATE_TIMER : atoi(tmp);

  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DOMOTICZ D_CMND_IDX " %d,%d,%d,%d, " D_CMND_KEYIDX " %d,%d,%d,%d, " D_CMND_SWITCHIDX " %d,%d,%d,%d, " D_CMND_SENSORIDX " %s, " D_CMND_UPDATETIMER " %d"),
    Settings.domoticz_relay_idx[0], Settings.domoticz_relay_idx[1], Settings.domoticz_relay_idx[2], Settings.domoticz_relay_idx[3],
    Settings.domoticz_key_idx[0], Settings.domoticz_key_idx[1], Settings.domoticz_key_idx[2], Settings.domoticz_key_idx[3],
    Settings.domoticz_switch_idx[0], Settings.domoticz_switch_idx[1], Settings.domoticz_switch_idx[2], Settings.domoticz_switch_idx[3],
    ssensor_indices, Settings.domoticz_update_timer);
  AddLog(LOG_LEVEL_INFO);
}
#endif  // USE_WEBSERVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

boolean Xdrv07(byte function)
{
  boolean result = false;

  if (Settings.flag.mqtt_enabled) {
    switch (function) {
#ifdef USE_WEBSERVER
      case FUNC_WEB_ADD_BUTTON:
        strncat_P(mqtt_data, HTTP_BTN_MENU_DOMOTICZ, sizeof(mqtt_data) - strlen(mqtt_data) -1);
        break;
      case FUNC_WEB_ADD_HANDLER:
        WebServer->on("/" WEB_HANDLE_DOMOTICZ, HandleDomoticzConfiguration);
        break;
#endif  // USE_WEBSERVER
      case FUNC_COMMAND:
        result = DomoticzCommand();
        break;
      case FUNC_MQTT_SUBSCRIBE:
        DomoticzMqttSubscribe();
        break;
      case FUNC_MQTT_INIT:
        domoticz_update_timer = 2;
        break;
      case FUNC_MQTT_DATA:
        result = DomoticzMqttData();
        break;
      case FUNC_EVERY_SECOND:
        DomoticzMqttUpdate();
        break;
      case FUNC_SHOW_SENSOR:
//        DomoticzSendSensor();
        break;
    }
  }
  return result;
}

#endif  // USE_DOMOTICZ
