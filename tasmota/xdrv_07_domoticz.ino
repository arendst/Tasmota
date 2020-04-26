/*
  xdrv_07_domoticz.ino - domoticz support for Tasmota

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

#ifdef USE_DOMOTICZ

#define XDRV_07             7

#define D_PRFX_DOMOTICZ "Domoticz"
#define D_CMND_IDX "Idx"
#define D_CMND_KEYIDX "KeyIdx"
#define D_CMND_SWITCHIDX "SwitchIdx"
#define D_CMND_SENSORIDX "SensorIdx"
#define D_CMND_UPDATETIMER "UpdateTimer"

const char kDomoticzCommands[] PROGMEM = D_PRFX_DOMOTICZ "|"  // Prefix
  D_CMND_IDX "|" D_CMND_KEYIDX "|" D_CMND_SWITCHIDX "|" D_CMND_SENSORIDX "|" D_CMND_UPDATETIMER ;

void (* const DomoticzCommand[])(void) PROGMEM = {
  &CmndDomoticzIdx, &CmndDomoticzKeyIdx, &CmndDomoticzSwitchIdx, &CmndDomoticzSensorIdx, &CmndDomoticzUpdateTimer };

const char DOMOTICZ_MESSAGE[] PROGMEM = "{\"idx\":%d,\"nvalue\":%d,\"svalue\":\"%s\",\"Battery\":%d,\"RSSI\":%d}";

#if MAX_DOMOTICZ_SNS_IDX < DZ_MAX_SENSORS
  #error "Domoticz: Too many sensors or change settings.h layout"
#endif

const char kDomoticzSensors[] PROGMEM =
  D_DOMOTICZ_TEMP "|" D_DOMOTICZ_TEMP_HUM "|" D_DOMOTICZ_TEMP_HUM_BARO "|" D_DOMOTICZ_POWER_ENERGY "|" D_DOMOTICZ_ILLUMINANCE "|"
  D_DOMOTICZ_COUNT "|" D_DOMOTICZ_VOLTAGE "|" D_DOMOTICZ_CURRENT "|" D_DOMOTICZ_AIRQUALITY "|" D_DOMOTICZ_P1_SMART_METER "|" D_DOMOTICZ_SHUTTER ;

char domoticz_in_topic[] = DOMOTICZ_IN_TOPIC;

int domoticz_update_timer = 0;
uint32_t domoticz_fan_debounce = 0;             // iFan02 state debounce timer
bool domoticz_subscribe = false;
bool domoticz_update_flag = true;

#ifdef USE_SHUTTER
bool domoticz_is_shutter = false;
#endif // USE_SHUTTER

int DomoticzBatteryQuality(void)
{
  // Battery 0%: ESP 2.6V (minimum operating voltage is 2.5)
  // Battery 100%: ESP 3.6V (maximum operating voltage is 3.6)
  // Battery 101% to 200%: ESP over 3.6V (means over maximum operating voltage)

  int quality = 100;	// Voltage range from 2,6V > 0%  to 3,6V > 100%

#ifdef USE_ADC_VCC
  uint16_t voltage = ESP.getVcc();
  if (voltage <= 2600) {
    quality = 0;
  } else if (voltage >= 4600) {
    quality = 200;
  } else {
    quality = (voltage - 2600) / 10;
  }
#endif
  return quality;
}

int DomoticzRssiQuality(void)
{
  // RSSI range: 0% to 10% (12 means disable RSSI in Domoticz)

  return WifiGetRssiAsQuality(WiFi.RSSI()) / 10;
}

#ifdef USE_SONOFF_IFAN
void MqttPublishDomoticzFanState(void)
{
  if (Settings.flag.mqtt_enabled && Settings.domoticz_relay_idx[1]) {  // SetOption3 - Enable MQTT
    char svalue[8];  // Fanspeed value

    int fan_speed = GetFanspeed();
    snprintf_P(svalue, sizeof(svalue), PSTR("%d"), fan_speed * 10);
    Response_P(DOMOTICZ_MESSAGE, (int)Settings.domoticz_relay_idx[1], (0 == fan_speed) ? 0 : 2, svalue, DomoticzBatteryQuality(), DomoticzRssiQuality());
    MqttPublish(domoticz_in_topic);

    domoticz_fan_debounce = millis();
  }
}

void DomoticzUpdateFanState(void)
{
  if (domoticz_update_flag) {
    MqttPublishDomoticzFanState();
  }
  domoticz_update_flag = true;
}
#endif  // USE_SONOFF_IFAN

void MqttPublishDomoticzPowerState(uint8_t device)
{
  if (Settings.flag.mqtt_enabled) {  // SetOption3 - Enable MQTT
    if ((device < 1) || (device > devices_present)) { device = 1; }
    if (Settings.domoticz_relay_idx[device -1]) {
#ifdef USE_SHUTTER
      if (domoticz_is_shutter) {
        // Shutter is updated by sensor update - power state should not be sent
      } else {
#endif  // USE_SHUTTER
#ifdef USE_SONOFF_IFAN
      if (IsModuleIfan() && (device > 1)) {
        // Fan handled by MqttPublishDomoticzFanState
      } else {
#endif  // USE_SONOFF_IFAN
        char svalue[8];  // Dimmer value

        snprintf_P(svalue, sizeof(svalue), PSTR("%d"), Settings.light_dimmer);
        Response_P(DOMOTICZ_MESSAGE, (int)Settings.domoticz_relay_idx[device -1], (power & (1 << (device -1))) ? 1 : 0, (light_type) ? svalue : "", DomoticzBatteryQuality(), DomoticzRssiQuality());
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

void DomoticzUpdatePowerState(uint8_t device)
{
  if (domoticz_update_flag) {
    MqttPublishDomoticzPowerState(device);
  }
  domoticz_update_flag = true;
}

void DomoticzMqttUpdate(void)
{
  if (domoticz_subscribe && (Settings.domoticz_update_timer || domoticz_update_timer)) {
    domoticz_update_timer--;
    if (domoticz_update_timer <= 0) {
      domoticz_update_timer = Settings.domoticz_update_timer;
      for (uint32_t i = 1; i <= devices_present; i++) {
#ifdef USE_SHUTTER
        if (domoticz_is_shutter)
        {
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

void DomoticzMqttSubscribe(void)
{
  uint8_t maxdev = (devices_present > MAX_DOMOTICZ_IDX) ? MAX_DOMOTICZ_IDX : devices_present;
  for (uint32_t i = 0; i < maxdev; i++) {
    if (Settings.domoticz_relay_idx[i]) {
      domoticz_subscribe = true;
    }
  }

  if (domoticz_subscribe) {
    char stopic[TOPSZ];
    snprintf_P(stopic, sizeof(stopic), PSTR(DOMOTICZ_OUT_TOPIC "/#")); // domoticz topic
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

bool DomoticzMqttData(void)
{
  domoticz_update_flag = true;

  if (strncasecmp_P(XdrvMailbox.topic, PSTR(DOMOTICZ_OUT_TOPIC), strlen(DOMOTICZ_OUT_TOPIC)) != 0) {
    return false;  // Process unchanged data
  }

  // topic is domoticz/out so try to analyse
  if (XdrvMailbox.data_len < 20) {
    return true;  // No valid data
  }
  StaticJsonBuffer<400> jsonBuf;
  JsonObject& domoticz = jsonBuf.parseObject(XdrvMailbox.data);
  if (!domoticz.success()) {
    return true;  // To much or invalid data
  }
//    if (strcmp_P(domoticz["dtype"],PSTR("Light/Switch"))) {
//      return true;
//    }
  uint32_t idx = domoticz["idx"];
  int16_t nvalue = -1;
  if (domoticz.containsKey("nvalue")) {
    nvalue = domoticz["nvalue"];
  }

  AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_DOMOTICZ "idx %d, nvalue %d"), idx, nvalue);

  bool found = false;
  if ((idx > 0) && (nvalue >= 0) && (nvalue <= 15)) {
    uint8_t maxdev = (devices_present > MAX_DOMOTICZ_IDX) ? MAX_DOMOTICZ_IDX : devices_present;
    for (uint32_t i = 0; i < maxdev; i++) {
      if (idx == Settings.domoticz_relay_idx[i]) {
        bool iscolordimmer = strcmp_P(domoticz["dtype"],PSTR("Color Switch")) == 0;
        bool isShutter = strcmp_P(domoticz["dtype"],PSTR("Light/Switch")) == 0 & strncmp_P(domoticz["switchType"],PSTR("Blinds"), 6) == 0;

        char stemp1[10];
        snprintf_P(stemp1, sizeof(stemp1), PSTR("%d"), i +1);
#ifdef USE_SONOFF_IFAN
        if (IsModuleIfan() && (1 == i)) {  // Idx 2 is fanspeed
          uint8_t svalue = 0;
          if (domoticz.containsKey("svalue1")) {
            svalue = domoticz["svalue1"];
          } else {
            return true;  // Invalid data
          }
          svalue = (nvalue == 2) ? svalue / 10 : 0;
          if (GetFanspeed() == svalue) {
            return true;  // Stop loop as already set
          }
          if (TimePassedSince(domoticz_fan_debounce) < 1000) {
            return true;  // Stop loop if device in limbo
          }
          snprintf_P(XdrvMailbox.topic, XdrvMailbox.index, PSTR("/" D_CMND_FANSPEED));
          snprintf_P(XdrvMailbox.data, XdrvMailbox.data_len, PSTR("%d"), svalue);
          found = true;
        } else
#endif  // USE_SONOFF_IFAN
#ifdef USE_SHUTTER
        if (isShutter)
        {
          if (domoticz.containsKey("nvalue")) {
            nvalue = domoticz["nvalue"];
          }

          uint8_t position = 0;
          if (domoticz.containsKey("svalue1")) {
            position = domoticz["svalue1"];
          }
          if (nvalue != 2) {
            position = nvalue == 0 ? 0 : 100;
          }

          snprintf_P(XdrvMailbox.topic, TOPSZ, PSTR("/" D_PRFX_SHUTTER D_CMND_SHUTTER_POSITION));
          snprintf_P(XdrvMailbox.data, XdrvMailbox.data_len, PSTR("%d"), position);
          XdrvMailbox.data_len = position > 99 ? 3 : (position > 9 ? 2 : 1);

          found = true;
        } else
#endif // USE_SHUTTER
#ifdef USE_LIGHT
        if (iscolordimmer && 10 == nvalue) {  // Color_SetColor
          // https://www.domoticz.com/wiki/Domoticz_API/JSON_URL%27s#Set_a_light_to_a_certain_color_or_color_temperature
          JsonObject& color = domoticz["Color"];
          uint16_t level = nvalue = domoticz["svalue1"];
          uint16_t r = color["r"]; r = r * level / 100;
          uint16_t g = color["g"]; g = g * level / 100;
          uint16_t b = color["b"]; b = b * level / 100;
          uint16_t cw = color["cw"]; cw = cw * level / 100;
          uint16_t ww = color["ww"]; ww = ww * level / 100;
          uint16_t m = 0;
          uint16_t t = 0;
          if (color.containsKey("m")) {
            m = color["m"];
            t = color["t"];
          }
          if (2 == m) {  // White with color temperature. Valid fields: t
            snprintf_P(XdrvMailbox.topic, XdrvMailbox.index, PSTR("/" D_CMND_BACKLOG));
            snprintf_P(XdrvMailbox.data, XdrvMailbox.data_len, PSTR(D_CMND_COLORTEMPERATURE " %d;" D_CMND_DIMMER " %d"), changeUIntScale(t, 0, 255, CT_MIN, CT_MAX), level);
          } else {
            snprintf_P(XdrvMailbox.topic, XdrvMailbox.index, PSTR("/" D_CMND_COLOR));
            snprintf_P(XdrvMailbox.data, XdrvMailbox.data_len, PSTR("%02x%02x%02x%02x%02x"), r, g, b, cw, ww);
          }
          found = true;
        }
        else if ((!iscolordimmer && 2 == nvalue) ||  // gswitch_sSetLevel
                  (iscolordimmer && 15 == nvalue)) {  // Color_SetBrightnessLevel
          if (domoticz.containsKey("svalue1")) {
            nvalue = domoticz["svalue1"];
          } else {
            return true;  // Invalid data
          }
          if (light_type && (Settings.light_dimmer == nvalue) && ((power >> i) &1)) {
            return true;  // State already set
          }
          snprintf_P(XdrvMailbox.topic, XdrvMailbox.index, PSTR("/" D_CMND_DIMMER));
          snprintf_P(XdrvMailbox.data, XdrvMailbox.data_len, PSTR("%d"), nvalue);
          found = true;
        } else
#endif  // USE_LIGHT
        if (1 == nvalue || 0 == nvalue) {
          if (((power >> i) &1) == (power_t)nvalue) {
            return true;  // Stop loop
          }
          snprintf_P(XdrvMailbox.topic, XdrvMailbox.index, PSTR("/" D_CMND_POWER "%s"), (devices_present > 1) ? stemp1 : "");
          snprintf_P(XdrvMailbox.data, XdrvMailbox.data_len, PSTR("%d"), nvalue);
          found = true;
        }
        break;
      }
    }
  }
  if (!found) { return true; }  // No command received

  AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_DOMOTICZ D_RECEIVED_TOPIC " %s, " D_DATA " %s"), XdrvMailbox.topic, XdrvMailbox.data);

  domoticz_update_flag = false;
  return false;  // Process new data
}

/*********************************************************************************************/

bool DomoticzSendKey(uint8_t key, uint8_t device, uint8_t state, uint8_t svalflg)
{
  bool result = false;

  if (device <= MAX_DOMOTICZ_IDX) {
    if ((Settings.domoticz_key_idx[device -1] || Settings.domoticz_switch_idx[device -1]) && (svalflg)) {
      Response_P(PSTR("{\"command\":\"switchlight\",\"idx\":%d,\"switchcmd\":\"%s\"}"),
        (key) ? Settings.domoticz_switch_idx[device -1] : Settings.domoticz_key_idx[device -1], (state) ? (POWER_TOGGLE == state) ? "Toggle" : "On" : "Off");
      MqttPublish(domoticz_in_topic);
      result = true;
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

uint8_t DomoticzHumidityState(float h)
{
  return (!h) ? 0 : (h < 40) ? 2 : (h > 70) ? 3 : 1;
}

void DomoticzSensor(uint8_t idx, char *data)
{
  if (Settings.domoticz_sensor_idx[idx]) {
    char dmess[128];  // {"idx":26700,"nvalue":0,"svalue":"22330.1;10234.4;22000.5;10243.4;1006;3000","Battery":100,"RSSI":10}

    memcpy(dmess, mqtt_data, sizeof(dmess));
    if (DZ_AIRQUALITY == idx) {
      Response_P(PSTR("{\"idx\":%d,\"nvalue\":%s,\"Battery\":%d,\"RSSI\":%d}"),
        Settings.domoticz_sensor_idx[idx], data, DomoticzBatteryQuality(), DomoticzRssiQuality());
    } else {
      uint8_t nvalue = 0;
#ifdef USE_SHUTTER
      if (DZ_SHUTTER == idx) {
        uint8_t position = atoi(data);
        nvalue = position < 2 ? 0 : (position == 100 ? 1 : 2);
      }
#endif  // USE_SHUTTER
      Response_P(DOMOTICZ_MESSAGE,
        Settings.domoticz_sensor_idx[idx], nvalue, data, DomoticzBatteryQuality(), DomoticzRssiQuality());
    }
    MqttPublish(domoticz_in_topic);
    memcpy(mqtt_data, dmess, sizeof(dmess));
  }
}

void DomoticzSensor(uint8_t idx, uint32_t value)
{
  char data[16];
  snprintf_P(data, sizeof(data), PSTR("%d"), value);
  DomoticzSensor(idx, data);
}

//void DomoticzTempHumPressureSensor(float temp, float hum, float baro = -1);
void DomoticzTempHumPressureSensor(float temp, float hum, float baro)
{
  char temperature[FLOATSZ];
  dtostrfd(temp, 2, temperature);
  char humidity[FLOATSZ];
  dtostrfd(hum, 2, humidity);

  char data[32];
  if (baro > -1) {
    char pressure[FLOATSZ];
    dtostrfd(baro, 2, pressure);

    snprintf_P(data, sizeof(data), PSTR("%s;%s;%d;%s;5"), temperature, humidity, DomoticzHumidityState(hum), pressure);
    DomoticzSensor(DZ_TEMP_HUM_BARO, data);
  } else {
    snprintf_P(data, sizeof(data), PSTR("%s;%s;%d"), temperature, humidity, DomoticzHumidityState(hum));
    DomoticzSensor(DZ_TEMP_HUM, data);
  }
}

void DomoticzSensorPowerEnergy(int power, char *energy)
{
  char data[16];
  snprintf_P(data, sizeof(data), PSTR("%d;%s"), power, energy);
  DomoticzSensor(DZ_POWER_ENERGY, data);
}

void DomoticzSensorP1SmartMeter(char *usage1, char *usage2, char *return1, char *return2, int power)
{
  //usage1   = energy usage meter tariff 1, This is an incrementing counter
  //usage2   = energy usage meter tariff 2, This is an incrementing counter
  //return1  = energy return meter tariff 1, This is an incrementing counter
  //return2  = energy return meter tariff 2, This is an incrementing counter
  //power    = if >= 0 actual usage power. if < 0 actual return power (Watt)
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

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndDomoticzIdx(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_DOMOTICZ_IDX)) {
    if (XdrvMailbox.payload >= 0) {
      Settings.domoticz_relay_idx[XdrvMailbox.index -1] = XdrvMailbox.payload;
      restart_flag = 2;
    }
    ResponseCmndIdxNumber(Settings.domoticz_relay_idx[XdrvMailbox.index -1]);
  }
}

void CmndDomoticzKeyIdx(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_DOMOTICZ_IDX)) {
    if (XdrvMailbox.payload >= 0) {
      Settings.domoticz_key_idx[XdrvMailbox.index -1] = XdrvMailbox.payload;
    }
    ResponseCmndIdxNumber(Settings.domoticz_key_idx[XdrvMailbox.index -1]);
  }
}

void CmndDomoticzSwitchIdx(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_DOMOTICZ_IDX)) {
    if (XdrvMailbox.payload >= 0) {
      Settings.domoticz_switch_idx[XdrvMailbox.index -1] = XdrvMailbox.payload;
    }
    ResponseCmndIdxNumber(Settings.domoticz_switch_idx[XdrvMailbox.index -1]);
  }
}

void CmndDomoticzSensorIdx(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= DZ_MAX_SENSORS)) {
    if (XdrvMailbox.payload >= 0) {
      Settings.domoticz_sensor_idx[XdrvMailbox.index -1] = XdrvMailbox.payload;
    }
    ResponseCmndIdxNumber(Settings.domoticz_sensor_idx[XdrvMailbox.index -1]);
  }
}

void CmndDomoticzUpdateTimer(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
    Settings.domoticz_update_timer = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings.domoticz_update_timer);
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

#ifdef USE_WEBSERVER

#define WEB_HANDLE_DOMOTICZ "dm"

const char S_CONFIGURE_DOMOTICZ[] PROGMEM = D_CONFIGURE_DOMOTICZ;

const char HTTP_BTN_MENU_DOMOTICZ[] PROGMEM =
  "<p><form action='" WEB_HANDLE_DOMOTICZ "' method='get'><button>" D_CONFIGURE_DOMOTICZ "</button></form></p>";

const char HTTP_FORM_DOMOTICZ[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_DOMOTICZ_PARAMETERS "&nbsp;</b></legend>"
  "<form method='post' action='" WEB_HANDLE_DOMOTICZ "'>"
  "<table>";
const char HTTP_FORM_DOMOTICZ_RELAY[] PROGMEM =
  "<tr><td style='width:260px'><b>" D_DOMOTICZ_IDX " %d</b></td><td style='width:70px'><input id='r%d' placeholder='0' value='%d'></td></tr>"
  "<tr><td style='width:260px'><b>" D_DOMOTICZ_KEY_IDX " %d</b></td><td style='width:70px'><input id='k%d' placeholder='0' value='%d'></td></tr>";
const char HTTP_FORM_DOMOTICZ_SWITCH[] PROGMEM =
  "<tr><td style='width:260px'><b>" D_DOMOTICZ_SWITCH_IDX " %d</b></td><td style='width:70px'><input id='s%d' placeholder='0' value='%d'></td></tr>";
const char HTTP_FORM_DOMOTICZ_SENSOR[] PROGMEM =
  "<tr><td style='width:260px'><b>" D_DOMOTICZ_SENSOR_IDX " %d</b> %s</td><td style='width:70px'><input id='l%d' placeholder='0' value='%d'></td></tr>";
const char HTTP_FORM_DOMOTICZ_TIMER[] PROGMEM =
  "<tr><td style='width:260px'><b>" D_DOMOTICZ_UPDATE_TIMER "</b> (" STR(DOMOTICZ_UPDATE_TIMER) ")</td><td style='width:70px'><input id='ut' placeholder='" STR(DOMOTICZ_UPDATE_TIMER) "' value='%d'></td></tr>";

void HandleDomoticzConfiguration(void)
{
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURE_DOMOTICZ);

  if (Webserver->hasArg("save")) {
    DomoticzSaveSettings();
    WebRestart(1);
    return;
  }

  char stemp[40];

  WSContentStart_P(S_CONFIGURE_DOMOTICZ);
  WSContentSendStyle();
  WSContentSend_P(HTTP_FORM_DOMOTICZ);
  for (uint32_t i = 0; i < MAX_DOMOTICZ_IDX; i++) {
    if (i < devices_present) {
      WSContentSend_P(HTTP_FORM_DOMOTICZ_RELAY,
        i +1, i, Settings.domoticz_relay_idx[i],
        i +1, i, Settings.domoticz_key_idx[i]);
    }
    if (Pin(GPIO_SWT1, i) < 99) {
      WSContentSend_P(HTTP_FORM_DOMOTICZ_SWITCH,
        i +1, i, Settings.domoticz_switch_idx[i]);
    }
#ifdef USE_SONOFF_IFAN
    if (IsModuleIfan() && (1 == i)) { break; }
#endif  // USE_SONOFF_IFAN
  }
  for (uint32_t i = 0; i < DZ_MAX_SENSORS; i++) {
    WSContentSend_P(HTTP_FORM_DOMOTICZ_SENSOR,
      i +1, GetTextIndexed(stemp, sizeof(stemp), i, kDomoticzSensors), i, Settings.domoticz_sensor_idx[i]);
  }
  WSContentSend_P(HTTP_FORM_DOMOTICZ_TIMER, Settings.domoticz_update_timer);
  WSContentSend_P(PSTR("</table>"));
  WSContentSend_P(HTTP_FORM_END);
  WSContentSpaceButton(BUTTON_CONFIGURATION);
  WSContentStop();
}

void DomoticzSaveSettings(void)
{
  char stemp[20];
  char ssensor_indices[6 * MAX_DOMOTICZ_SNS_IDX];
  char tmp[100];

  for (uint32_t i = 0; i < MAX_DOMOTICZ_IDX; i++) {
    snprintf_P(stemp, sizeof(stemp), PSTR("r%d"), i);
    WebGetArg(stemp, tmp, sizeof(tmp));
    Settings.domoticz_relay_idx[i] = (!strlen(tmp)) ? 0 : atoi(tmp);
    snprintf_P(stemp, sizeof(stemp), PSTR("k%d"), i);
    WebGetArg(stemp, tmp, sizeof(tmp));
    Settings.domoticz_key_idx[i] = (!strlen(tmp)) ? 0 : atoi(tmp);
    snprintf_P(stemp, sizeof(stemp), PSTR("s%d"), i);
    WebGetArg(stemp, tmp, sizeof(tmp));
    Settings.domoticz_switch_idx[i] = (!strlen(tmp)) ? 0 : atoi(tmp);
  }
  ssensor_indices[0] = '\0';
  for (uint32_t i = 0; i < DZ_MAX_SENSORS; i++) {
    snprintf_P(stemp, sizeof(stemp), PSTR("l%d"), i);
    WebGetArg(stemp, tmp, sizeof(tmp));
    Settings.domoticz_sensor_idx[i] = (!strlen(tmp)) ? 0 : atoi(tmp);
    snprintf_P(ssensor_indices, sizeof(ssensor_indices), PSTR("%s%s%d"), ssensor_indices, (strlen(ssensor_indices)) ? "," : "",  Settings.domoticz_sensor_idx[i]);
  }
  WebGetArg("ut", tmp, sizeof(tmp));
  Settings.domoticz_update_timer = (!strlen(tmp)) ? DOMOTICZ_UPDATE_TIMER : atoi(tmp);

  AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_DOMOTICZ D_CMND_IDX " %d,%d,%d,%d, " D_CMND_KEYIDX " %d,%d,%d,%d, " D_CMND_SWITCHIDX " %d,%d,%d,%d, " D_CMND_SENSORIDX " %s, " D_CMND_UPDATETIMER " %d"),
    Settings.domoticz_relay_idx[0], Settings.domoticz_relay_idx[1], Settings.domoticz_relay_idx[2], Settings.domoticz_relay_idx[3],
    Settings.domoticz_key_idx[0], Settings.domoticz_key_idx[1], Settings.domoticz_key_idx[2], Settings.domoticz_key_idx[3],
    Settings.domoticz_switch_idx[0], Settings.domoticz_switch_idx[1], Settings.domoticz_switch_idx[2], Settings.domoticz_switch_idx[3],
    ssensor_indices, Settings.domoticz_update_timer);
}
#endif  // USE_WEBSERVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv07(uint8_t function)
{
  bool result = false;

  if (Settings.flag.mqtt_enabled) {  // SetOption3 - Enable MQTT
    switch (function) {
      case FUNC_EVERY_SECOND:
        DomoticzMqttUpdate();
        break;
      case FUNC_MQTT_DATA:
        result = DomoticzMqttData();
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_ADD_BUTTON:
        WSContentSend_P(HTTP_BTN_MENU_DOMOTICZ);
        break;
      case FUNC_WEB_ADD_HANDLER:
        Webserver->on("/" WEB_HANDLE_DOMOTICZ, HandleDomoticzConfiguration);
        break;
#endif  // USE_WEBSERVER
      case FUNC_MQTT_SUBSCRIBE:
        DomoticzMqttSubscribe();
#ifdef USE_SHUTTER
        if (Settings.domoticz_sensor_idx[DZ_SHUTTER]) { domoticz_is_shutter = true; }
#endif // USE_SHUTTER
        break;
      case FUNC_MQTT_INIT:
        domoticz_update_timer = 2;
        break;
      case FUNC_SHOW_SENSOR:
//        DomoticzSendSensor();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kDomoticzCommands, DomoticzCommand);
        break;
    }
  }
  return result;
}

#endif  // USE_DOMOTICZ
