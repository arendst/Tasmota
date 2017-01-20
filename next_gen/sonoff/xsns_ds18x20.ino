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

#ifdef USE_DS18x20
/*********************************************************************************************\
 * DS18B20 - Temperature
\*********************************************************************************************/

#define W1_SKIP_ROM 0xCC
#define W1_CONVERT_TEMP 0x44
#define W1_READ_SCRATCHPAD 0xBE

#define DS18X20_MAX_SENSORS 8

#include <OneWire.h>

OneWire ds(pin[GPIO_DSB]);

uint8_t ds18x20_addr[DS18X20_MAX_SENSORS][8];
uint8_t ds18x20_idx[DS18X20_MAX_SENSORS];
uint8_t ds18x20_snsrs = 0;

void ds18x20_search()
{
  uint8_t num_sensors=0;
  uint8_t sensor = 0;
  uint8_t i;

  ds.reset_search();
  for (num_sensors = 0; num_sensors < DS18X20_MAX_SENSORS; num_sensors)
  {
    if (!ds.search(ds18x20_addr[num_sensors])) {
      ds.reset_search();
      break;
    }
    // If CRC Ok and Type DS18S20 or DS18B20
    if ((OneWire::crc8(ds18x20_addr[num_sensors], 7) == ds18x20_addr[num_sensors][7]) &&
       ((ds18x20_addr[num_sensors][0]==0x10) || (ds18x20_addr[num_sensors][0]==0x28)))
       num_sensors++;
  }
  for (int i = 0; i < num_sensors; i++) ds18x20_idx[i] = i;
  for (int i = 0; i < num_sensors; i++) {
    for (int j = i + 1; j < num_sensors; j++) {
      if (uint32_t(ds18x20_addr[ds18x20_idx[i]]) > uint32_t(ds18x20_addr[ds18x20_idx[j]])) {
        std::swap(ds18x20_idx[i], ds18x20_idx[j]);
      }
    }
  }
  ds18x20_snsrs = num_sensors;
}

uint8_t ds18x20_sensors()
{
  return ds18x20_snsrs;
}

String ds18x20_address(uint8_t sensor)
{
  char addrStr[20];
  uint8_t i;

  for (i = 0; i < 8; i++) sprintf(addrStr+2*i, "%02X", ds18x20_addr[ds18x20_idx[sensor]][i]);
  return String(addrStr);
}

String ds18x20_type(uint8_t sensor)
{
  char typeStr[10];

  switch(ds18x20_addr[ds18x20_idx[sensor]][0]) {
  case 0x10:
    strcpy(typeStr, "DS18S20");
    break;
  case 0x28:
    strcpy(typeStr, "DS18B20");
    break;
  default:
    strcpy(typeStr, "DS18x20");
  }
  return String(typeStr);
}

void ds18x20_convert()
{
  ds.reset();
  ds.write(W1_SKIP_ROM);        // Address all Sensors on Bus
  ds.write(W1_CONVERT_TEMP);    // start conversion, no parasite power on at the end
//  delay(750);                   // 750ms should be enough for 12bit conv
}

float ds18x20_convertCtoF(float c)
{
  return c * 1.8 + 32;
}

boolean ds18x20_read(uint8_t sensor, bool S, float &t)
{
  byte data[12];
  uint8_t sign = 1;
  uint8_t i = 0;
  float temp9 = 0.0;
  uint8_t present = 0;

  t = NAN;

  ds.reset();
  ds.select(ds18x20_addr[ds18x20_idx[sensor]]);
  ds.write(W1_READ_SCRATCHPAD); // Read Scratchpad

  for (i = 0; i < 9; i++) data[i] = ds.read();
  if (OneWire::crc8(data, 8) == data[8]) {
    switch(ds18x20_addr[ds18x20_idx[sensor]][0]) {
    case 0x10:  // DS18S20
      if (data[1] > 0x80) sign = -1; // App-Note fix possible sign error
      if (data[0] & 1) {
        temp9 = ((data[0] >> 1) + 0.5) * sign;
      } else {
        temp9 = (data[0] >> 1) * sign;
      }
      t = (temp9 - 0.25) + ((16.0 - data[6]) / 16.0);
      if(S) t = ds18x20_convertCtoF(t);
      break;
    case 0x28:  // DS18B20
      t = ((data[1] << 8) + data[0]) * 0.0625;
      if(S) t = ds18x20_convertCtoF(t);
      break;
    }
  }
  return (!isnan(t));
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void ds18x20_mqttPresent(char* svalue, uint16_t ssvalue, uint8_t* djson)
{
  char stemp1[10], stemp2[10];
  float t;

  byte dsxflg = 0;
  for (byte i = 0; i < ds18x20_sensors(); i++) {
    if (ds18x20_read(i, TEMP_CONVERSION, t)) {           // Check if read failed
      dtostrf(t, 1, TEMP_RESOLUTION &3, stemp2);
      if (!dsxflg) {
        snprintf_P(svalue, ssvalue, PSTR("%s, \"DS18x20\":{"), svalue);
        *djson = 1;
        stemp1[0] = '\0';
        dsxflg = 1;
      }
      snprintf_P(svalue, ssvalue, PSTR("%s%s\"DS%d\":{\"Type\":\"%s\", \"Address\":\"%s\", \"Temperature\":\"%s\"}"),
        svalue, stemp1, i +1, ds18x20_type(i).c_str(), ds18x20_address(i).c_str(), stemp2);
      strcpy(stemp1, ", ");
    }
  }
  if (dsxflg) snprintf_P(svalue, ssvalue, PSTR("%s}"), svalue);
}

#ifdef USE_WEBSERVER
String ds18x20_webPresent()
{
  char stemp[10], sconv[10];
  float t;
  String page = "";

  snprintf_P(sconv, sizeof(sconv), PSTR("&deg;%c"), (TEMP_CONVERSION) ? 'F' : 'C');
  for (byte i = 0; i < ds18x20_sensors(); i++) {
    if (ds18x20_read(i, TEMP_CONVERSION, t)) {   // Check if read failed
      dtostrf(t, 1, TEMP_RESOLUTION &3, stemp);
      page += F("<tr><td>DS"); page += String(i +1); page += F(" Temperature: </td><td>"); page += stemp; page += sconv; page += F("</td></tr>");
    }
  }
  return page;
}
#endif  // USE_WEBSERVER
#endif  // USE_DS18x20
