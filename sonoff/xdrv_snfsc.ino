/*
Copyright (c) 2017 Theo Arends.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

/*********************************************************************************************\
 * Sonoff Sc
 *
 * sc_value[0] DHT11 Humidity
 * sc_value[1] DHT11 Temperature in Celsius
 * sc_value[2] Light level from 1 (Dark) to 10 (Bright) - inverted from original
 * sc_value[3] Noise level from 1 (Quiet) to 10 (Loud)
 * sc_value[4] Air Quality level from 1 (Bad) to 10 (Good) - inverted from original
 *
 * To ATMEGA328P:
 *   AT+DEVCONFIG="uploadFreq":1800,"humiThreshold":2,"tempThreshold":1[1B]
 *   AT+NOTIFY="uploadFreq":1800,"humiThreshold":2,"tempThreshold":1[1B]
 *     response: AT+NOTIFY=ok[1B]
 *   AT+SEND=fail[1B]
 *   AT+SEND=ok[1B]
 *   AT+STATUS=4[1B]
 *   AT+STATUS[1B]
 *   AT+START[1B]
 * 
 * From ATMEGA328P:
 *   AT+UPDATE="humidity":42,"temperature":20,"light":7,"noise":3,"dusty":1[1B]
 *     response: AT+SEND=ok[1B] or AT+SEND=fail[1B]
 *   AT+STATUS?[1B]
 *     response: AT+STATUS=4[1B]
 *
 * Sequence:
 *  SC sends:        ATMEGA328P sends:
 *  AT+START[1B]
 *                   AT+UPDATE="humidity":42,"temperature":20,"light":7,"noise":3,"dusty":1[1B]
 *  AT+SEND=ok[1B]
 *
 *                   AT+STATUS?[1B]
 *  AT+STATUS=4[1B]
 *
\*********************************************************************************************/

uint16_t sc_value[5] = { 0 };

void sc_init()
{
//  Serial.write("AT+DEVCONFIG=\"uploadFreq\":1800\e");
  Serial.write("AT+START\e");
//  Serial.write("AT+STATUS\e");
}

void sc_rcvstat(char *rcvstat)
{
  char *p, *str;
  uint16_t value[5] = { 0 };

  if (!strncmp(rcvstat,"AT+UPDATE=",10)) {
    int8_t i = -1;
    for (str = strtok_r(rcvstat, ":", &p); str && i < 5; str = strtok_r(NULL, ":", &p)) value[i++] = atoi(str);
    if (value[0] > 0) {
      for (byte i = 0; i < 5; i++) sc_value[i] = value[i];
      sc_value[2] = (11 - sc_value[2]) * 10;  // Invert light level
      sc_value[3] *= 10;
      sc_value[4] = (11 - sc_value[4]) * 10;  // Invert dust level
      Serial.write("AT+SEND=ok\e");
    } else {
      Serial.write("AT+SEND=fail\e");
    }
  }
  else if (!strcmp(rcvstat,"AT+STATUS?")) {
    Serial.write("AT+STATUS=4\e");
  }
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

float sc_convertCtoF(float c)
{
  return c * 1.8 + 32;
}

void sc_mqttPresent(char* svalue, uint16_t ssvalue, uint8_t* djson)
{
  if (sc_value[0] > 0) {
    char stemp1[10], stemp2[10];

    float t = sc_value[1];
    if (TEMP_CONVERSION) t = sc_convertCtoF(t);
    dtostrf(t, 1, TEMP_RESOLUTION &3, stemp1);
    float h = sc_value[0];
    dtostrf(h, 1, HUMIDITY_RESOLUTION &3, stemp2);
    snprintf_P(svalue, ssvalue, PSTR("%s, \"SC\":{\"Temperature\":%s, \"Humidity\":%s, \"Light\":%d, \"Noise\":%d, \"AirQuality\":%d}"),
      svalue, stemp1, stemp2, sc_value[2], sc_value[3], sc_value[4]);
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
    char stemp[10], sensor[80], scstype[] = "SC";

    float t = sc_value[1];
    if (TEMP_CONVERSION) t = sc_convertCtoF(t);
    dtostrf(t, 1, TEMP_RESOLUTION &3, stemp);
    snprintf_P(sensor, sizeof(sensor), HTTP_SNS_TEMP, scstype, stemp, (TEMP_CONVERSION) ? 'F' : 'C');
    page += sensor;
    float h = sc_value[0];
    dtostrf(h, 1, HUMIDITY_RESOLUTION &3, stemp);
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

