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

#define DOMOTICZ_MAX_SENSORS  6

#ifdef USE_WEBSERVER
const char HTTP_FORM_DOMOTICZ[] PROGMEM =
  "<fieldset><legend><b>&nbsp;Domoticz parameters&nbsp;</b></legend><form method='post' action='sv'>"
  "<input id='w' name='w' value='4' hidden><input id='r' name='r' value='1' hidden>"
  "<br/><table style='width:97%'>";
const char HTTP_FORM_DOMOTICZ_RELAY[] PROGMEM =
  "<tr><td><b>Idx {1</b></td></td><td width='20%'><input id='r{1' name='r{1' length=8 placeholder='0' value='{2'></td></tr>"
  "<tr><td><b>Key idx {1</b></td><td><input id='k{1' name='k{1' length=8 placeholder='0' value='{3'></td></tr>";
const char HTTP_FORM_DOMOTICZ_SWITCH[] PROGMEM =
  "<tr><td><b>Switch idx {1</b></td><td width='20%'><input id='s{1' name='s{1' length=8 placeholder='0' value='{4'></td></tr>";
const char HTTP_FORM_DOMOTICZ_SENSOR[] PROGMEM =
  "<tr><td><b>Sensor idx {1</b> - {2</td><td width='20%'><input id='l{1' name='l{1' length=8 placeholder='0' value='{5'></td></tr>";
const char HTTP_FORM_DOMOTICZ_TIMER[] PROGMEM =
  "<tr><td><b>Update timer</b> (" STR(DOMOTICZ_UPDATE_TIMER) ")</td><td><input id='ut' name='ut' length=32 placeholder='" STR(DOMOTICZ_UPDATE_TIMER) "' value='{6'</td></tr>";
#endif  // USE_WEBSERVER

const char domoticz_sensors[DOMOTICZ_MAX_SENSORS][14] PROGMEM =
  { "Temp", "Temp,Hum", "Temp,Hum,Baro", "Power,Energy", "Illuminance", "Count" };

char domoticz_in_topic[] = DOMOTICZ_IN_TOPIC;
char domoticz_out_topic[] = DOMOTICZ_OUT_TOPIC;

boolean domoticz_subscribe = false;
int domoticz_update_timer = 0;
byte domoticz_update_flag = 1;

void mqtt_publishDomoticzPowerState(byte device)
{
  char svalue[64];  // was MESSZ

  if (sysCfg.flag.mqtt_enabled && sysCfg.domoticz_relay_idx[device -1]) {
    if ((device < 1) || (device > Maxdevice)) {
      device = 1;
    }
    if (sfl_flg) {
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"idx\":%d,\"nvalue\":2,\"svalue\":\"%d\"}"),
        sysCfg.domoticz_relay_idx[device -1], sysCfg.led_dimmer[device -1]);
      mqtt_publish(domoticz_in_topic, svalue);
    }
    else if ((Maxdevice == device) && (pin[GPIO_WS2812] < 99)) {
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"idx\":%d,\"nvalue\":2,\"svalue\":\"%d\"}"),
        sysCfg.domoticz_relay_idx[device -1], sysCfg.ws_dimmer);
      mqtt_publish(domoticz_in_topic, svalue);
    }
    snprintf_P(svalue, sizeof(svalue), PSTR("{\"idx\":%d,\"nvalue\":%d,\"svalue\":\"\"}"),
      sysCfg.domoticz_relay_idx[device -1], (power & (0x01 << (device -1))) ? 1 : 0);
    mqtt_publish(domoticz_in_topic, svalue);
  }
}

void domoticz_updatePowerState(byte device)
{
  if (domoticz_update_flag) {
    mqtt_publishDomoticzPowerState(device);
  }
  domoticz_update_flag = 1;
}

void domoticz_mqttUpdate()
{
  if (domoticz_subscribe && (sysCfg.domoticz_update_timer || domoticz_update_timer)) {
    domoticz_update_timer--;
    if (domoticz_update_timer <= 0) {
      domoticz_update_timer = sysCfg.domoticz_update_timer;
      for (byte i = 1; i <= Maxdevice; i++) {
        mqtt_publishDomoticzPowerState(i);
      }
    }
  }
}

void domoticz_setUpdateTimer(uint16_t value)
{
  domoticz_update_timer = value;
}

void domoticz_mqttSubscribe()
{
  for (byte i = 0; i < Maxdevice; i++) {
    if (sysCfg.domoticz_relay_idx[i]) {
      domoticz_subscribe = true;
    }
  }
  if (domoticz_subscribe) {
    char stopic[TOPSZ];
    snprintf_P(stopic, sizeof(stopic), PSTR("%s/#"), domoticz_out_topic); // domoticz topic
    mqttClient.subscribe(stopic);
    mqttClient.loop();  // Solve LmacRxBlk:1 messages
  }
}

boolean domoticz_update()
{
  return domoticz_update_flag;
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

boolean domoticz_mqttData(char *topicBuf, uint16_t stopicBuf, char *dataBuf, uint16_t sdataBuf)
{
  char log[LOGSZ];
  char stemp1[10];
  char scommand[10];
  unsigned long idx = 0;
  int16_t nvalue;
  int16_t found = 0;
  
  domoticz_update_flag = 1;
  if (!strncmp(topicBuf, domoticz_out_topic, strlen(domoticz_out_topic)) != 0) {
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

    snprintf_P(log, sizeof(log), PSTR("DMTZ: idx %d, nvalue %d"), idx, nvalue);
    addLog(LOG_LEVEL_DEBUG_MORE, log);

    if (nvalue >= 0 && nvalue <= 2) {
      for (byte i = 0; i < Maxdevice; i++) {
        if ((idx > 0) && (idx == sysCfg.domoticz_relay_idx[i])) {
          snprintf_P(stemp1, sizeof(stemp1), PSTR("%d"), i +1);
          if (2 == nvalue) {
            nvalue = domoticz["svalue1"];
            if ((pin[GPIO_WS2812] < 99) && (sysCfg.ws_dimmer == nvalue)) {
              return 1;
            }
            if (sfl_flg && (sysCfg.led_dimmer[i] == nvalue)) {
              return 1;
            }
            snprintf_P(topicBuf, stopicBuf, PSTR("/DIMMER%s"), (Maxdevice > 1) ? stemp1 : "");
            snprintf_P(dataBuf, sdataBuf, PSTR("%d"), nvalue);
            found = 1;
          } else {
            if (((power >> i) &1) == nvalue) {
              return 1;
            }
            snprintf_P(topicBuf, stopicBuf, PSTR("/POWER%s"), (Maxdevice > 1) ? stemp1 : "");
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

    snprintf_P(log, sizeof(log), PSTR("DMTZ: Receive topic %s, data %s"), topicBuf, dataBuf);
    addLog(LOG_LEVEL_DEBUG_MORE, log);

    domoticz_update_flag = 0;
  }
  return 0;
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

boolean domoticz_command(const char *type, uint16_t index, char *dataBuf, uint16_t data_len, int16_t payload, char *svalue, uint16_t ssvalue)
{
  boolean serviced = true;
  
  if (!strncmp_P(type,PSTR("DOMOTICZ"),8)) {
    if (!strcmp_P(type +8,PSTR("IDX")) && (index > 0) && (index <= Maxdevice)) {
      if (payload >= 0) {
        sysCfg.domoticz_relay_idx[index -1] = payload;
        restartflag = 2;
      }
      snprintf_P(svalue, ssvalue, PSTR("{\"DomoticzIdx%d\":%d}"), index, sysCfg.domoticz_relay_idx[index -1]);
    }
    else if (!strcmp_P(type +8,PSTR("KEYIDX")) && (index > 0) && (index <= Maxdevice)) {
      if (payload >= 0) {
        sysCfg.domoticz_key_idx[index -1] = payload;
      }
      snprintf_P(svalue, ssvalue, PSTR("{\"DomoticzKeyIdx%d\":%d}"), index, sysCfg.domoticz_key_idx[index -1]);
    }
    else if (!strcmp_P(type +8,PSTR("SWITCHIDX")) && (index > 0) && (index <= Maxdevice)) {
      if (payload >= 0) {
        sysCfg.domoticz_switch_idx[index -1] = payload;
      }
      snprintf_P(svalue, ssvalue, PSTR("{\"DomoticzSwitchIdx%d\":%d}"), index, sysCfg.domoticz_key_idx[index -1]);
    }
    else if (!strcmp_P(type +8,PSTR("SENSORIDX")) && (index > 0) && (index <= DOMOTICZ_MAX_SENSORS)) {
      if (payload >= 0) {
        sysCfg.domoticz_sensor_idx[index -1] = payload;
      }
      snprintf_P(svalue, ssvalue, PSTR("{\"DomoticzSensorIdx%d\":%d}"), index, sysCfg.domoticz_sensor_idx[index -1]);
    }
    else if (!strcmp_P(type +8,PSTR("UPDATETIMER"))) {
      if ((payload >= 0) && (payload < 3601)) {
        sysCfg.domoticz_update_timer = payload;
      }
      snprintf_P(svalue, ssvalue, PSTR("{\"DomoticzUpdateTimer\":%d}"), sysCfg.domoticz_update_timer);
    }
    else serviced = false;
  }
  else serviced = false;
  return serviced;
}

boolean domoticz_button(byte key, byte device, byte state, byte svalflg)
{
  if ((sysCfg.domoticz_key_idx[device -1] || sysCfg.domoticz_switch_idx[device -1]) && (svalflg)) {
    char svalue[80];  // was MESSZ

    snprintf_P(svalue, sizeof(svalue), PSTR("{\"command\":\"switchlight\",\"idx\":%d,\"switchcmd\":\"%s\"}"),
      (key) ? sysCfg.domoticz_switch_idx[device -1] : sysCfg.domoticz_key_idx[device -1], (state) ? (2 == state) ? "Toggle" : "On" : "Off");
    mqtt_publish(domoticz_in_topic, svalue);
    return 1;
  } else {
    return 0;
  }
}

/*********************************************************************************************\
 * Sensors
\*********************************************************************************************/

uint8_t dom_hum_stat(char *hum)
{
  uint8_t h = atoi(hum);
  return (!h) ? 0 : (h < 40) ? 2 : (h > 70) ? 3 : 1;
}

void dom_sensor(byte idx, char *data)
{
  char dmess[64];

  if (sysCfg.domoticz_sensor_idx[idx]) {
    snprintf_P(dmess, sizeof(dmess), PSTR("{\"idx\":%d,\"nvalue\":0,\"svalue\":\"%s\"}"),
      sysCfg.domoticz_sensor_idx[idx], data);
    mqtt_publish(domoticz_in_topic, dmess);
  }
}

void domoticz_sensor1(char *temp)
{
  dom_sensor(0, temp);
}

void domoticz_sensor2(char *temp, char *hum)
{
  char data[16];
  snprintf_P(data, sizeof(data), PSTR("%s;%s;%d"), temp, hum, dom_hum_stat(hum));
  dom_sensor(1, data);
}

void domoticz_sensor3(char *temp, char *hum, char *baro)
{
  char data[32];
  snprintf_P(data, sizeof(data), PSTR("%s;%s;%d;%s;5"), temp, hum, dom_hum_stat(hum), baro);
  dom_sensor(2, data);
}

void domoticz_sensor4(uint16_t power, char *energy)
{
  char data[16];
  snprintf_P(data, sizeof(data), PSTR("%d;%s"), power, energy);
  dom_sensor(3, data);
}

void domoticz_sensor5(uint16_t lux)
{
  char data[8];
  snprintf_P(data, sizeof(data), PSTR("%d"), lux);
  dom_sensor(4, data);
}

void domoticz_sensor6(uint32_t count)
{
  char data[16];
  snprintf_P(data, sizeof(data), PSTR("%d"), count);
  dom_sensor(5, data);
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

#ifdef USE_WEBSERVER
void handleDomoticz()
{
  if (HTTP_USER == _httpflag) {
    handleRoot();
    return;
  }
  addLog_P(LOG_LEVEL_DEBUG, PSTR("HTTP: Handle Domoticz config"));

  char stemp[20];
  
  String page = FPSTR(HTTP_HEAD);
  page.replace("{v}", "Configure Domoticz");
  page += FPSTR(HTTP_FORM_DOMOTICZ);
  for (int i = 0; i < 4; i++) {
    if (i < Maxdevice) {
      page += FPSTR(HTTP_FORM_DOMOTICZ_RELAY);
      page.replace("{2", String((int)sysCfg.domoticz_relay_idx[i]));
      page.replace("{3", String((int)sysCfg.domoticz_key_idx[i]));
    }
    if (pin[GPIO_SWT1 +i] < 99) {
      page += FPSTR(HTTP_FORM_DOMOTICZ_SWITCH);
      page.replace("{4", String((int)sysCfg.domoticz_switch_idx[i]));
    }
    page.replace("{1", String(i +1));
  }
  for (int i = 0; i < DOMOTICZ_MAX_SENSORS; i++) {
    page += FPSTR(HTTP_FORM_DOMOTICZ_SENSOR);
    page.replace("{1", String(i +1));
    snprintf_P(stemp, sizeof(stemp), domoticz_sensors[i]);
    page.replace("{2", stemp);
    page.replace("{5", String((int)sysCfg.domoticz_sensor_idx[i]));
  }
  page += FPSTR(HTTP_FORM_DOMOTICZ_TIMER);
  page.replace("{6", String((int)sysCfg.domoticz_update_timer));
  page += F("</table>");
  page += FPSTR(HTTP_FORM_END);
  page += FPSTR(HTTP_BTN_CONF);
  showPage(page);
}

void domoticz_saveSettings()
{
  char log[LOGSZ], stemp[20];

  for (byte i = 0; i < 4; i++) {
    snprintf_P(stemp, sizeof(stemp), PSTR("r%d"), i +1);
    sysCfg.domoticz_relay_idx[i] = (!strlen(webServer->arg(stemp).c_str())) ? 0 : atoi(webServer->arg(stemp).c_str());
    snprintf_P(stemp, sizeof(stemp), PSTR("k%d"), i +1);
    sysCfg.domoticz_key_idx[i] = (!strlen(webServer->arg(stemp).c_str())) ? 0 : atoi(webServer->arg(stemp).c_str());
    snprintf_P(stemp, sizeof(stemp), PSTR("s%d"), i +1);
    sysCfg.domoticz_switch_idx[i] = (!strlen(webServer->arg(stemp).c_str())) ? 0 : atoi(webServer->arg(stemp).c_str());
  }
  for (byte i = 0; i < DOMOTICZ_MAX_SENSORS; i++) {
    snprintf_P(stemp, sizeof(stemp), PSTR("l%d"), i +1);
    sysCfg.domoticz_sensor_idx[i] = (!strlen(webServer->arg(stemp).c_str())) ? 0 : atoi(webServer->arg(stemp).c_str());
  }
  sysCfg.domoticz_update_timer = (!strlen(webServer->arg("ut").c_str())) ? DOMOTICZ_UPDATE_TIMER : atoi(webServer->arg("ut").c_str());
  snprintf_P(log, sizeof(log), PSTR("HTTP: Domoticz idx %d, %d, %d, %d, update timer %d"),
    sysCfg.domoticz_relay_idx[0], sysCfg.domoticz_relay_idx[1], sysCfg.domoticz_relay_idx[2], sysCfg.domoticz_relay_idx[3],
    sysCfg.domoticz_update_timer);
  addLog(LOG_LEVEL_INFO, log);
  snprintf_P(log, sizeof(log), PSTR("HTTP: key %d, %d, %d, %d, switch %d, %d, %d, %d, sensor %d, %d, %d, %d, %d, %d"),
    sysCfg.domoticz_key_idx[0], sysCfg.domoticz_key_idx[1], sysCfg.domoticz_key_idx[2], sysCfg.domoticz_key_idx[3],
    sysCfg.domoticz_switch_idx[0], sysCfg.domoticz_switch_idx[1], sysCfg.domoticz_switch_idx[2], sysCfg.domoticz_switch_idx[3],
    sysCfg.domoticz_sensor_idx[0], sysCfg.domoticz_sensor_idx[1], sysCfg.domoticz_sensor_idx[2], sysCfg.domoticz_sensor_idx[3],
    sysCfg.domoticz_sensor_idx[4], sysCfg.domoticz_sensor_idx[5]);
  addLog(LOG_LEVEL_INFO, log);
}
#endif  // USE_WEBSERVER
#endif  // USE_DOMOTICZ

