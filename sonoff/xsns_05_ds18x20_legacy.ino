/*
  xsns_05_ds18x20_legacy.ino - DS18x20 temperature sensor support for Sonoff-Tasmota

  Copyright (C) 2018  Heiko Krupp and Theo Arends

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

#ifdef USE_DS18x20_LEGACY
/*********************************************************************************************\
 * DS18B20 - Temperature
\*********************************************************************************************/

#define XSNS_05              5

#define DS18S20_CHIPID       0x10
#define DS18B20_CHIPID       0x28
#define MAX31850_CHIPID      0x3B

#define W1_SKIP_ROM          0xCC
#define W1_CONVERT_TEMP      0x44
#define W1_READ_SCRATCHPAD   0xBE

#define DS18X20_MAX_SENSORS  8

#include <OneWire.h>

OneWire *ds = NULL;

uint8_t ds18x20_address[DS18X20_MAX_SENSORS][8];
uint8_t ds18x20_index[DS18X20_MAX_SENSORS];
uint8_t ds18x20_sensors = 0;
char ds18x20_types[9];

void Ds18x20Init(void)
{
  ds = new OneWire(pin[GPIO_DSB]);
}

void Ds18x20Search(void)
{
  uint8_t num_sensors=0;
  uint8_t sensor = 0;

  ds->reset_search();
  for (num_sensors = 0; num_sensors < DS18X20_MAX_SENSORS; num_sensors) {
    if (!ds->search(ds18x20_address[num_sensors])) {
      ds->reset_search();
      break;
    }
    // If CRC Ok and Type DS18S20, DS18B20 or MAX31850
    if ((OneWire::crc8(ds18x20_address[num_sensors], 7) == ds18x20_address[num_sensors][7]) &&
       ((ds18x20_address[num_sensors][0]==DS18S20_CHIPID) || (ds18x20_address[num_sensors][0]==DS18B20_CHIPID) || (ds18x20_address[num_sensors][0]==MAX31850_CHIPID))) {
      num_sensors++;
    }
  }
  for (byte i = 0; i < num_sensors; i++) {
    ds18x20_index[i] = i;
  }
  for (byte i = 0; i < num_sensors; i++) {
    for (byte j = i + 1; j < num_sensors; j++) {
      if (uint32_t(ds18x20_address[ds18x20_index[i]]) > uint32_t(ds18x20_address[ds18x20_index[j]])) {
        std::swap(ds18x20_index[i], ds18x20_index[j]);
      }
    }
  }
  ds18x20_sensors = num_sensors;
}

uint8_t Ds18x20Sensors(void)
{
  return ds18x20_sensors;
}

String Ds18x20Addresses(uint8_t sensor)
{
  char address[20];

  for (byte i = 0; i < 8; i++) {
    sprintf(address+2*i, "%02X", ds18x20_address[ds18x20_index[sensor]][i]);
  }
  return String(address);
}

void Ds18x20Convert(void)
{
  ds->reset();
  ds->write(W1_SKIP_ROM);        // Address all Sensors on Bus
  ds->write(W1_CONVERT_TEMP);    // start conversion, no parasite power on at the end
//  delay(750);                   // 750ms should be enough for 12bit conv
}

boolean Ds18x20Read(uint8_t sensor, float &t)
{
  byte data[12];
  int8_t sign = 1;
  uint16_t temp12 = 0;
  int16_t temp14 = 0;
  float temp9 = 0.0;
  uint8_t present = 0;

  t = NAN;

  ds->reset();
  ds->select(ds18x20_address[ds18x20_index[sensor]]);
  ds->write(W1_READ_SCRATCHPAD); // Read Scratchpad

  for (byte i = 0; i < 9; i++) {
    data[i] = ds->read();
  }
  if (OneWire::crc8(data, 8) == data[8]) {
    switch(ds18x20_address[ds18x20_index[sensor]][0]) {
    case DS18S20_CHIPID:
      if (data[1] > 0x80) {
        data[0] = (~data[0]) +1;
        sign = -1;  // App-Note fix possible sign error
      }
      if (data[0] & 1) {
        temp9 = ((data[0] >> 1) + 0.5) * sign;
      } else {
        temp9 = (data[0] >> 1) * sign;
      }
      t = ConvertTemp((temp9 - 0.25) + ((16.0 - data[6]) / 16.0));
      break;
    case DS18B20_CHIPID:
      temp12 = (data[1] << 8) + data[0];
      if (temp12 > 2047) {
        temp12 = (~temp12) +1;
        sign = -1;
      }
      t = ConvertTemp(sign * temp12 * 0.0625);  // Divide by 16
      break;
    case MAX31850_CHIPID:
        temp14 = (data[1] << 8) + (data[0] & 0xFC);
        t = ConvertTemp(temp14 * 0.0625);  // Divide by 16
      break;
    }
  }
  return (!isnan(t));
}

/********************************************************************************************/

void Ds18x20Type(uint8_t sensor)
{
  strcpy_P(ds18x20_types, PSTR("DS18x20"));
  switch(ds18x20_address[ds18x20_index[sensor]][0]) {
    case DS18S20_CHIPID:
      strcpy_P(ds18x20_types, PSTR("DS18S20"));
      break;
    case DS18B20_CHIPID:
      strcpy_P(ds18x20_types, PSTR("DS18B20"));
      break;
    case MAX31850_CHIPID:
      strcpy_P(ds18x20_types, PSTR("MAX31850"));
      break;
  }
}

void Ds18x20Show(boolean json)
{
  char stemp[10];
  float t;

  byte dsxflg = 0;
  for (byte i = 0; i < Ds18x20Sensors(); i++) {
    if (Ds18x20Read(i, t)) {           // Check if read failed
      Ds18x20Type(i);
      char temperature[33];
      dtostrfd(t, Settings.flag2.temperature_resolution, temperature);

      if (json) {
        if (!dsxflg) {
          snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"DS18x20\":{"), mqtt_data);
          stemp[0] = '\0';
        }
        dsxflg++;
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"DS%d\":{\"" D_JSON_TYPE "\":\"%s\",\"" D_JSON_ADDRESS "\":\"%s\",\"" D_JSON_TEMPERATURE "\":%s}"),
          mqtt_data, stemp, i +1, ds18x20_types, Ds18x20Addresses(i).c_str(), temperature);
        strlcpy(stemp, ",", sizeof(stemp));
#ifdef USE_DOMOTICZ
        if ((0 == tele_period) && (1 == dsxflg)) {
          DomoticzSensor(DZ_TEMP, temperature);
        }
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
        if ((0 == tele_period) && (1 == dsxflg)) {
          KnxSensor(KNX_TEMPERATURE, t);
        }
#endif  // USE_KNX
#ifdef USE_WEBSERVER
      } else {
        snprintf_P(stemp, sizeof(stemp), PSTR("%s-%d"), ds18x20_types, i +1);
        snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_TEMP, mqtt_data, stemp, temperature, TempUnit());
#endif  // USE_WEBSERVER
      }
    }
  }
  if (json) {
    if (dsxflg) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
    }
  }
  Ds18x20Search();      // Check for changes in sensors number
  Ds18x20Convert();     // Start Conversion, takes up to one second
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

boolean Xsns05(byte function)
{
  boolean result = false;

  if (pin[GPIO_DSB] < 99) {
    switch (function) {
      case FUNC_INIT:
        Ds18x20Init();
        break;
      case FUNC_PREP_BEFORE_TELEPERIOD:
        Ds18x20Search();      // Check for changes in sensors number
        Ds18x20Convert();     // Start Conversion, takes up to one second
        break;
      case FUNC_JSON_APPEND:
        Ds18x20Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        Ds18x20Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_DS18x20_LEGACY
