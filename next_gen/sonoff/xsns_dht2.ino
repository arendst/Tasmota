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

#ifdef USE_DHT2
/*********************************************************************************************\
 * DHT11, DHT21 (AM2301), DHT22 (AM2302, AM2321) - Temperature and Humidy
 *
 * Reading temperature or humidity takes about 250 milliseconds!
 * Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
\*********************************************************************************************/

// WARNING: To use this DHT library you'll need to delete files DHT_U.cpp and DHT_U.h if present

#include "DHT.h"

DHT dht2(pin[GPIO_DHT11], dht_type);

float dht2_t, dht2_h = 0;

boolean dht_readTempHum(bool S, float &t, float &h)
{
  h = dht2.readHumidity();
  t = dht2.readTemperature(S);
  if (!isnan(t)) dht2_t = t; else if (dht2_h) t = dht2_t;
  if (!isnan(h)) dht2_h = h; else if (dht2_h) h = dht2_h;
  return (!isnan(t) && !isnan(h));
}

void dht_init()
{
  dht2.begin();
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void dht_mqttPresent(char* svalue, uint16_t ssvalue, uint8_t* djson)
{
  char stemp1[10], stemp2[10];
  float t, h;

  if (dht_readTempHum(TEMP_CONVERSION, t, h)) {     // Read temperature
    dtostrf(t, 1, TEMP_RESOLUTION &3, stemp1);
    dtostrf(h, 1, HUMIDITY_RESOLUTION &3, stemp2);
    snprintf_P(svalue, ssvalue, PSTR("%s, \"DHT\":{\"Temperature\":\"%s\", \"Humidity\":\"%s\"}"), svalue, stemp1, stemp2);
    *djson = 1;
  }
}

#ifdef USE_WEBSERVER
String dht_webPresent()
{
  char stemp[10], sconv[10];
  float t, h;
  String page = "";
  
  if (dht_readTempHum(TEMP_CONVERSION, t, h)) {     // Read temperature as Celsius (the default)
    snprintf_P(sconv, sizeof(sconv), PSTR("&deg;%c"), (TEMP_CONVERSION) ? 'F' : 'C');
    dtostrf(t, 1, TEMP_RESOLUTION &3, stemp);
    page += F("<tr><td>DHT Temperature: </td><td>"); page += stemp; page += sconv; page += F("</td></tr>");
    dtostrf(h, 1, HUMIDITY_RESOLUTION &3, stemp);
    page += F("<tr><td>DHT Humidity: </td><td>"); page += stemp; page += F("%</td></tr>");
  }
  return page;
}
#endif  // USE_WEBSERVER
#endif  // USE_DHT2
