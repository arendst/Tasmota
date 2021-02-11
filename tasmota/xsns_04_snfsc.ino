/*
  xsns_04_snfsc.ino - sonoff SC support for Tasmota

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

#ifdef USE_SONOFF_SC
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
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_SERIAL D_TRANSMIT " %s"), data);
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

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_SERIAL D_RECEIVED " %s"), rcvstat);

  if (!strncasecmp_P(rcvstat, PSTR("AT+UPDATE="), 10)) {
    int8_t i = -1;
    for (str = strtok_r(rcvstat, ":", &p); str && i < 5; str = strtok_r(nullptr, ":", &p)) {
      value[i++] = atoi(str);
    }
    if (value[0] > 0) {
      for (uint32_t i = 0; i < 5; i++) {
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
  "{s}" D_LIGHT "{m}%d%%{e}{s}" D_NOISE "{m}%d%%{e}{s}" D_AIR_QUALITY "{m}%d%%{e}";  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

void SonoffScShow(bool json)
{
  if (sc_value[0] > 0) {
    float t = ConvertTemp(sc_value[1]);
    float h = ConvertHumidity(sc_value[0]);

    if (json) {
      ResponseAppend_P(PSTR(",\"SonoffSC\":{"));
      ResponseAppendTHD(t, h);
      ResponseAppend_P(PSTR(",\"" D_JSON_LIGHT "\":%d,\"" D_JSON_NOISE "\":%d,\"" D_JSON_AIRQUALITY "\":%d}"), sc_value[2], sc_value[3], sc_value[4]);
#ifdef USE_DOMOTICZ
      if (0 == TasmotaGlobal.tele_period) {
        DomoticzTempHumPressureSensor(t, h);
        DomoticzSensor(DZ_ILLUMINANCE, sc_value[2]);
        DomoticzSensor(DZ_COUNT, sc_value[3]);
        DomoticzSensor(DZ_AIRQUALITY, 500 + ((100 - sc_value[4]) * 20));
      }
#endif  // USE_DOMOTICZ

#ifdef USE_KNX
      if (0 == TasmotaGlobal.tele_period) {
        KnxSensor(KNX_TEMPERATURE, t);
        KnxSensor(KNX_HUMIDITY, h);
      }
#endif  // USE_KNX

#ifdef USE_WEBSERVER
    } else {
      WSContentSend_THD("", t, h);
      WSContentSend_PD(HTTP_SNS_SCPLUS, sc_value[2], sc_value[3], sc_value[4]);
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns04(uint8_t function)
{
  bool result = false;

  if (SONOFF_SC == TasmotaGlobal.module_type) {
    switch (function) {
      case FUNC_JSON_APPEND:
        SonoffScShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        SonoffScShow(0);
        break;
#endif  // USE_WEBSERVER
      case FUNC_INIT:
        SonoffScInit();
        break;
    }
  }
  return result;
}

#endif  // USE_SONOFF_SC
