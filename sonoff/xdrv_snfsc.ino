/*
  xdrv_snfsc.ino - sonoff SC support for Sonoff-Tasmota

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

uint16_t sc_value[5] = { 0 };

void sc_send(const char *data)
{
  Serial.write(data);
  Serial.write('\x1B');
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_SERIAL D_TRANSMIT " %s"), data);
  addLog(LOG_LEVEL_DEBUG);
}

void sc_init()
{
//  sc_send("AT+DEVCONFIG=\"uploadFreq\":1800");
  sc_send("AT+START");
//  sc_send("AT+STATUS");
}

void sc_rcvstat(char *rcvstat)
{
  char *p;
  char *str;
  uint16_t value[5] = { 0 };

  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_SERIAL D_RECEIVED " %s"), rcvstat);
  addLog(LOG_LEVEL_DEBUG);

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
      sc_send("AT+SEND=ok");
    } else {
      sc_send("AT+SEND=fail");
    }
  }
  else if (!strcasecmp_P(rcvstat, PSTR("AT+STATUS?"))) {
    sc_send("AT+STATUS=4");
  }
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void sc_mqttPresent(uint8_t* djson)
{
  if (sc_value[0] > 0) {
    char stemp1[10];
    char stemp2[10];

    float t = convertTemp(sc_value[1]);
    dtostrfd(t, sysCfg.flag.temperature_resolution, stemp1);
    float h = sc_value[0];
    dtostrfd(h, sysCfg.flag.humidity_resolution, stemp2);
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s, \"" D_TEMPERATURE "\":%s, \"" D_HUMIDITY "\":%s, \"" D_LIGHT "\":%d, \"" D_NOISE "\":%d, \"" D_AIRQUALITY "\":%d"),
      mqtt_data, stemp1, stemp2, sc_value[2], sc_value[3], sc_value[4]);
    *djson = 1;
#ifdef USE_DOMOTICZ
    domoticz_sensor2(stemp1, stemp2);
    domoticz_sensor5(sc_value[2]);
#endif  // USE_DOMOTICZ
  }
}

#ifdef USE_WEBSERVER
String sc_webPresent()
{
  String page = "";

  if (sc_value[0] > 0) {
    char stemp[10];
    char sensor[80];
    char scstype[] = "";

    float t = convertTemp(sc_value[1]);
    dtostrfi(t, sysCfg.flag.temperature_resolution, stemp);
    snprintf_P(sensor, sizeof(sensor), HTTP_SNS_TEMP, scstype, stemp, tempUnit());
    page += sensor;
    float h = sc_value[0];
    dtostrfi(h, sysCfg.flag.humidity_resolution, stemp);
    snprintf_P(sensor, sizeof(sensor), HTTP_SNS_HUM, scstype, stemp);
    page += sensor;
    snprintf_P(sensor, sizeof(sensor), HTTP_SNS_LIGHT, scstype, sc_value[2]);
    page += sensor;
    snprintf_P(sensor, sizeof(sensor), HTTP_SNS_NOISE, scstype, sc_value[3]);
    page += sensor;
    snprintf_P(sensor, sizeof(sensor), HTTP_SNS_DUST, scstype, sc_value[4]);
    page += sensor;
  }
  return page;
}
#endif  // USE_WEBSERVER

