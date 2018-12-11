/*
  xsns_04_snfsc.ino - sonoff SC support for Sonoff-Tasmota

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

/*********************************************************************************************\
  Sonoff Sc

  sc_value[0] DHT11 Humidity
  sc_value[1] DHT11 Temperature in Celsius
  sc_value[2] Light level from 1 (Dark) to 10 (Bright) - inverted from original
  sc_value[3] Noise level from 1 (Quiet) to 10 (Loud)
  sc_value[4] Air Quality level from 1 (Bad) to 10 (Good) - inverted from original

  To ATMEGA328P:
    AT+DEVCONFIG="uploadFreq":1800,"humiThreshold":2,"tempThreshold":1[1B]
    AT+NOTIFY="uploadFreq":1800,"humiThreshold":2,"tempThreshold":1[1B]
      response: AT+NOTIFY=ok[1B]
    AT+SEND=fail[1B]
    AT+SEND=ok[1B]
    AT+STATUS=4[1B]
    AT+STATUS[1B]
    AT+START[1B]

  From ATMEGA328P:
    AT+UPDATE="humidity":42,"temperature":20,"light":7,"noise":3,"dusty":1[1B]
      response: AT+SEND=ok[1B] or AT+SEND=fail[1B]
    AT+STATUS?[1B]
      response: AT+STATUS=4[1B]

  Sequence:
   SC sends:        ATMEGA328P sends:
   AT+START[1B]
                    AT+UPDATE="humidity":42,"temperature":20,"light":7,"noise":3,"dusty":1[1B]
   AT+SEND=ok[1B]

                    AT+STATUS?[1B]
   AT+STATUS=4[1B]

\*********************************************************************************************/

#define XSNS_04             4

uint16_t sc_value[5] = { 0 };

void SonoffScSend(const char *data)
{
  Serial.write(data);
  Serial.write('\x1B');
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_SERIAL D_TRANSMIT " %s"), data);
  AddLog(LOG_LEVEL_DEBUG);
}

void SonoffScInit(void)
{
//  SonoffScSend("AT+DEVCONFIG=\"uploadFreq\":1800");
  SonoffScSend("AT+START");
//  SonoffScSend("AT+STATUS");
}

void SonoffScSerialInput(char *rcvstat)
{
  char *p;
  char *str;
  uint16_t value[5] = { 0 };

  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_SERIAL D_RECEIVED " %s"), rcvstat);
  AddLog(LOG_LEVEL_DEBUG);

  if (!strncasecmp_P(rcvstat, PSTR("AT+UPDATE="), 10)) {
    int8_t i = -1;
    for (str = strtok_r(rcvstat, ":", &p); str && i < 5; str = strtok_r(NULL, ":", &p)) {
      value[i++] = atoi(str);
    }
    if (value[0] > 0) {
      for (byte i = 0; i < 5; i++) {
        sc_value[i] = value[i];
      }
      sc_value[2] = (11 - sc_value[2]) * 10;  // Invert light level
      sc_value[3] *= 10;
      sc_value[4] = (11 - sc_value[4]) * 10;  // Invert dust level
      SonoffScSend("AT+SEND=ok");
    } else {
      SonoffScSend("AT+SEND=fail");
    }
  }
  else if (!strcasecmp_P(rcvstat, PSTR("AT+STATUS?"))) {
    SonoffScSend("AT+STATUS=4");
  }
}

/********************************************************************************************/

#ifdef USE_WEBSERVER
const char HTTP_SNS_SCPLUS[] PROGMEM =
  "%s{s}" D_LIGHT "{m}%d%%{e}{s}" D_NOISE "{m}%d%%{e}{s}" D_AIR_QUALITY "{m}%d%%{e}";  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

void SonoffScShow(boolean json)
{
  if (sc_value[0] > 0) {
    char temperature[10];
    char humidity[10];

    float t = ConvertTemp(sc_value[1]);
    float h = sc_value[0];
    dtostrfd(t, Settings.flag2.temperature_resolution, temperature);
    dtostrfd(h, Settings.flag2.humidity_resolution, humidity);

    if (json) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"SonoffSC\":{\"" D_JSON_TEMPERATURE "\":%s,\"" D_JSON_HUMIDITY "\":%s,\"" D_JSON_LIGHT "\":%d,\"" D_JSON_NOISE "\":%d,\"" D_JSON_AIRQUALITY "\":%d}"),
        mqtt_data, temperature, humidity, sc_value[2], sc_value[3], sc_value[4]);
#ifdef USE_DOMOTICZ
      if (0 == tele_period) {
        DomoticzTempHumSensor(temperature, humidity);
        DomoticzSensor(DZ_ILLUMINANCE, sc_value[2]);
        DomoticzSensor(DZ_COUNT, sc_value[3]);
        DomoticzSensor(DZ_AIRQUALITY, 500 + ((100 - sc_value[4]) * 20));
      }
#endif  // USE_DOMOTICZ

#ifdef USE_KNX
      if (0 == tele_period) {
        KnxSensor(KNX_TEMPERATURE, t);
        KnxSensor(KNX_HUMIDITY, h);
      }
#endif  // USE_KNX

#ifdef USE_WEBSERVER
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_TEMP, mqtt_data, "", temperature, TempUnit());
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_HUM, mqtt_data, "", humidity);
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_SCPLUS, mqtt_data, sc_value[2], sc_value[3], sc_value[4]);
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

boolean Xsns04(byte function)
{
  boolean result = false;

  if (SONOFF_SC == Settings.module) {
    switch (function) {
      case FUNC_INIT:
        SonoffScInit();
        break;
      case FUNC_JSON_APPEND:
        SonoffScShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        SonoffScShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}
