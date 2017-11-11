/*
  xdrv_domoticz.ino - domoticz support for Sonoff-Tasmota

  Copyright (C) 2017  Theo Arends

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

#ifdef USE_WEBSERVER
const char HTTP_FORM_DOMOTICZ[] PROGMEM =
  "<fieldset><legend><b>&nbsp;" D_DOMOTICZ_PARAMETERS "&nbsp;</b></legend><form method='post' action='sv'>"
  "<input id='w' name='w' value='4' hidden><input id='r' name='r' value='1' hidden>"
  "<br/><table>";
const char HTTP_FORM_DOMOTICZ_RELAY[] PROGMEM =
  "<tr><td width='260'><b>" D_DOMOTICZ_IDX " {1</b></td><td width='70'><input id='r{1' name='r{1' placeholder='0' value='{2'></td></tr>"
  "<tr><td width='260'><b>" D_DOMOTICZ_KEY_IDX " {1</b></td><td width='70'><input id='k{1' name='k{1' placeholder='0' value='{3'></td></tr>";
  const char HTTP_FORM_DOMOTICZ_SWITCH[] PROGMEM =
  "<tr><td width='260'><b>" D_DOMOTICZ_SWITCH_IDX " {1</b></td><td width='70'><input id='s{1' name='s{1' placeholder='0' value='{4'></td></tr>";
const char HTTP_FORM_DOMOTICZ_SENSOR[] PROGMEM =
  "<tr><td width='260'><b>" D_DOMOTICZ_SENSOR_IDX " {1</b> {2</td><td width='70'><input id='l{1' name='l{1' placeholder='0' value='{5'></td></tr>";
const char HTTP_FORM_DOMOTICZ_TIMER[] PROGMEM =
  "<tr><td width='260'><b>" D_DOMOTICZ_UPDATE_TIMER "</b> (" STR(DOMOTICZ_UPDATE_TIMER) ")</td><td width='70'><input id='ut' name='ut' placeholder='" STR(DOMOTICZ_UPDATE_TIMER) "' value='{6'</td></tr>";
#endif  // USE_WEBSERVER

enum DomoticzCommands {
    CMND_IDX,      CMND_KEYIDX,      CMND_SWITCHIDX,      CMND_SENSORIDX,      CMND_UPDATETIMER };
const char kDomoticzCommands[] PROGMEM =
  D_CMND_IDX "|" D_CMND_KEYIDX "|" D_CMND_SWITCHIDX "|" D_CMND_SENSORIDX "|" D_CMND_UPDATETIMER ;

enum DomoticzSensors {DZ_TEMP, DZ_TEMP_HUM, DZ_TEMP_HUM_BARO, DZ_POWER_ENERGY, DZ_ILLUMINANCE, DZ_COUNT, DZ_VOLTAGE, DZ_CURRENT, DZ_MAX_SENSORS};

const char kDomoticzSensors[] PROGMEM =
  D_DOMOTICZ_TEMP "|" D_DOMOTICZ_TEMP_HUM "|" D_DOMOTICZ_TEMP_HUM_BARO "|" D_DOMOTICZ_POWER_ENERGY "|" D_DOMOTICZ_ILLUMINANCE "|" D_DOMOTICZ_COUNT "|" D_DOMOTICZ_VOLTAGE "|" D_DOMOTICZ_CURRENT ;

const char S_JSON_DOMOTICZ_COMMAND_INDEX_NVALUE[] PROGMEM = "{\"" D_CMND_DOMOTICZ "%s%d\":%d}";

char domoticz_in_topic[] = DOMOTICZ_IN_TOPIC;
char domoticz_out_topic[] = DOMOTICZ_OUT_TOPIC;

boolean domoticz_subscribe = false;
int domoticz_update_timer = 0;
byte domoticz_update_flag = 1;

void MqttPublishDomoticzPowerState(byte device)
{
  char sdimmer[8];

  if ((device < 1) || (device > devices_present)) {
    device = 1;
  }
  if (Settings.flag.mqtt_enabled && Settings.domoticz_relay_idx[device -1]) {
    snprintf_P(sdimmer, sizeof(sdimmer), PSTR("%d"), Settings.light_dimmer);
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"idx\":%d,\"nvalue\":%d,\"svalue\":\"%s\"}"),
      Settings.domoticz_relay_idx[device -1], (power & (1 << (device -1))) ? 1 : 0, (light_type) ? sdimmer : "");
    MqttPublish(domoticz_in_topic);
  }
}

void DomoticzUpdatePowerState(byte device)
{
  if (domoticz_update_flag) {
    MqttPublishDomoticzPowerState(device);
  }
  domoticz_update_flag = 1;
}

void DomoticzMqttUpdate()
{
  if (domoticz_subscribe && (Settings.domoticz_update_timer || domoticz_update_timer)) {
    domoticz_update_timer--;
    if (domoticz_update_timer <= 0) {
      domoticz_update_timer = Settings.domoticz_update_timer;
      for (byte i = 1; i <= devices_present; i++) {
        MqttPublishDomoticzPowerState(i);
      }
    }
  }
}

void DomoticzSetUpdateTimer(uint16_t value)
{
  domoticz_update_timer = value;
}

void DomoticzMqttSubscribe()
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
*/

boolean DomoticzMqttData(char *topicBuf, uint16_t stopicBuf, char *dataBuf, uint16_t sdataBuf)
{
  char stemp1[10];
  char scommand[10];
  unsigned long idx = 0;
  int16_t nvalue;
  int16_t found = 0;

  domoticz_update_flag = 1;
  if (!strncmp(topicBuf, domoticz_out_topic, strlen(domoticz_out_topic))) {
    if (sdataBuf < 20) {
      return 1;
    }
    StaticJsonBuffer<400> jsonBuf;
    JsonObject& domoticz = jsonBuf.parseObject(dataBuf);
    if (!domoticz.success()) {
      return 1;
    }
//    if (strcmp_P(domoticz["dtype"],PSTR("Light/Switch"))) {
//      return 1;
//    }
    idx = domoticz["idx"];
    nvalue = domoticz["nvalue"];

    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DOMOTICZ "idx %d, nvalue %d"), idx, nvalue);
    AddLog(LOG_LEVEL_DEBUG_MORE);

    if ((idx > 0) && (nvalue >= 0) && (nvalue <= 2)) {
      uint8_t maxdev = (devices_present > MAX_DOMOTICZ_IDX) ? MAX_DOMOTICZ_IDX : devices_present;
      for (byte i = 0; i < maxdev; i++) {
        if (idx == Settings.domoticz_relay_idx[i]) {
          snprintf_P(stemp1, sizeof(stemp1), PSTR("%d"), i +1);
          if (2 == nvalue) {
            nvalue = domoticz["svalue1"];
            if (light_type && (Settings.light_dimmer == nvalue) && ((power >> i) &1)) {
              return 1;
            }
            snprintf_P(topicBuf, stopicBuf, PSTR("/" D_CMND_DIMMER));
            snprintf_P(dataBuf, sdataBuf, PSTR("%d"), nvalue);
            found = 1;
          } else {
            if (((power >> i) &1) == nvalue) {
              return 1;
            }
            snprintf_P(topicBuf, stopicBuf, PSTR("/" D_CMND_POWER "%s"), (devices_present > 1) ? stemp1 : "");
            snprintf_P(dataBuf, sdataBuf, PSTR("%d"), nvalue);
            found = 1;
          }
          break;
        }
      }
    }
    if (!found) {
      return 1;
    }

    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DOMOTICZ D_RECEIVED_TOPIC " %s, " D_DATA " %s"), topicBuf, dataBuf);
    AddLog(LOG_LEVEL_DEBUG_MORE);

    domoticz_update_flag = 0;
  }
  return 0;
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

boolean DomoticzCommand(const char *type, uint16_t index, char *dataBuf, uint16_t data_len, int16_t payload)
{
  char command [CMDSZ];
  boolean serviced = true;
  uint8_t dmtcz_len = strlen(D_CMND_DOMOTICZ);  // Prep for string length change

  if (!strncasecmp_P(type, PSTR(D_CMND_DOMOTICZ), dmtcz_len)) {  // Prefix
    int command_code = GetCommandCode(command, sizeof(command), type +dmtcz_len, kDomoticzCommands);
    if ((CMND_IDX == command_code) && (index > 0) && (index <= MAX_DOMOTICZ_IDX)) {
      if (payload >= 0) {
        Settings.domoticz_relay_idx[index -1] = payload;
        restart_flag = 2;
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_DOMOTICZ_COMMAND_INDEX_NVALUE, command, index, Settings.domoticz_relay_idx[index -1]);
    }
    else if ((CMND_KEYIDX == command_code) && (index > 0) && (index <= MAX_DOMOTICZ_IDX)) {
      if (payload >= 0) {
        Settings.domoticz_key_idx[index -1] = payload;
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_DOMOTICZ_COMMAND_INDEX_NVALUE, command, index, Settings.domoticz_key_idx[index -1]);
    }
    else if ((CMND_SWITCHIDX == command_code) && (index > 0) && (index <= MAX_DOMOTICZ_IDX)) {
      if (payload >= 0) {
        Settings.domoticz_switch_idx[index -1] = payload;
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_DOMOTICZ_COMMAND_INDEX_NVALUE, command, index, Settings.domoticz_key_idx[index -1]);
    }
    else if ((CMND_SENSORIDX == command_code) && (index > 0) && (index <= DZ_MAX_SENSORS)) {
      if (payload >= 0) {
        Settings.domoticz_sensor_idx[index -1] = payload;
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_DOMOTICZ_COMMAND_INDEX_NVALUE, command, index, Settings.domoticz_sensor_idx[index -1]);
    }
    else if (CMND_UPDATETIMER == command_code) {
      if ((payload >= 0) && (payload < 3601)) {
        Settings.domoticz_update_timer = payload;
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_DOMOTICZ "%s\":%d}"), command, Settings.domoticz_update_timer);
    }
    else serviced = false;
  }
  else serviced = false;
  return serviced;
}

boolean DomoticzButton(byte key, byte device, byte state, byte svalflg)
{
  if ((Settings.domoticz_key_idx[device -1] || Settings.domoticz_switch_idx[device -1]) && (svalflg)) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"command\":\"switchlight\",\"idx\":%d,\"switchcmd\":\"%s\"}"),
      (key) ? Settings.domoticz_switch_idx[device -1] : Settings.domoticz_key_idx[device -1], (state) ? (2 == state) ? "Toggle" : "On" : "Off");
    MqttPublish(domoticz_in_topic);
    return 1;
  } else {
    return 0;
  }
}

/*********************************************************************************************\
 * Sensors
\*********************************************************************************************/

uint8_t DomoticzHumidityState(char *hum)
{
  uint8_t h = atoi(hum);
  return (!h) ? 0 : (h < 40) ? 2 : (h > 70) ? 3 : 1;
}

void DomoticzSensor(byte idx, char *data)
{
  if (Settings.domoticz_sensor_idx[idx]) {
    char dmess[64];

    memcpy(dmess, mqtt_data, sizeof(dmess));
    snprintf_P(mqtt_data, sizeof(dmess), PSTR("{\"idx\":%d,\"nvalue\":0,\"svalue\":\"%s\"}"),
      Settings.domoticz_sensor_idx[idx], data);
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

void DomoticzSensorPowerEnergy(uint16_t power, char *energy)
{
  char data[16];
  snprintf_P(data, sizeof(data), PSTR("%d;%s"), power, energy);
  DomoticzSensor(DZ_POWER_ENERGY, data);
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

#ifdef USE_WEBSERVER
const char S_CONFIGURE_DOMOTICZ[] PROGMEM = D_CONFIGURE_DOMOTICZ;

void HandleDomoticzConfiguration()
{
  if (HTTP_USER == webserver_state) {
    HandleRoot();
    return;
  }
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURE_DOMOTICZ);

  char stemp[32];
  char *sensortype;

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_CONFIGURE_DOMOTICZ));
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

void DomoticzSaveSettings()
{
  char stemp[20];

  for (byte i = 0; i < MAX_DOMOTICZ_IDX; i++) {
    snprintf_P(stemp, sizeof(stemp), PSTR("r%d"), i +1);
    Settings.domoticz_relay_idx[i] = (!strlen(WebServer->arg(stemp).c_str())) ? 0 : atoi(WebServer->arg(stemp).c_str());
    snprintf_P(stemp, sizeof(stemp), PSTR("k%d"), i +1);
    Settings.domoticz_key_idx[i] = (!strlen(WebServer->arg(stemp).c_str())) ? 0 : atoi(WebServer->arg(stemp).c_str());
    snprintf_P(stemp, sizeof(stemp), PSTR("s%d"), i +1);
    Settings.domoticz_switch_idx[i] = (!strlen(WebServer->arg(stemp).c_str())) ? 0 : atoi(WebServer->arg(stemp).c_str());
  }
  for (byte i = 0; i < DZ_MAX_SENSORS; i++) {
    snprintf_P(stemp, sizeof(stemp), PSTR("l%d"), i +1);
    Settings.domoticz_sensor_idx[i] = (!strlen(WebServer->arg(stemp).c_str())) ? 0 : atoi(WebServer->arg(stemp).c_str());
  }
  Settings.domoticz_update_timer = (!strlen(WebServer->arg("ut").c_str())) ? DOMOTICZ_UPDATE_TIMER : atoi(WebServer->arg("ut").c_str());
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DOMOTICZ D_CMND_IDX " %d, %d, %d, %d, " D_CMND_UPDATETIMER " %d"),
    Settings.domoticz_relay_idx[0], Settings.domoticz_relay_idx[1], Settings.domoticz_relay_idx[2], Settings.domoticz_relay_idx[3],
    Settings.domoticz_update_timer);
  AddLog(LOG_LEVEL_INFO);
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DOMOTICZ D_CMND_KEYIDX " %d, %d, %d, %d, " D_CMND_SWITCHIDX " %d, %d, %d, %d, " D_CMND_SENSORIDX " %d, %d, %d, %d, %d, %d, %d, %d"),
    Settings.domoticz_key_idx[0], Settings.domoticz_key_idx[1], Settings.domoticz_key_idx[2], Settings.domoticz_key_idx[3],
    Settings.domoticz_switch_idx[0], Settings.domoticz_switch_idx[1], Settings.domoticz_switch_idx[2], Settings.domoticz_switch_idx[3],
    Settings.domoticz_sensor_idx[0], Settings.domoticz_sensor_idx[1], Settings.domoticz_sensor_idx[2], Settings.domoticz_sensor_idx[3],
    Settings.domoticz_sensor_idx[4], Settings.domoticz_sensor_idx[5], Settings.domoticz_sensor_idx[6], Settings.domoticz_sensor_idx[7]);
  AddLog(LOG_LEVEL_INFO);
}
#endif  // USE_WEBSERVER
#endif  // USE_DOMOTICZ
