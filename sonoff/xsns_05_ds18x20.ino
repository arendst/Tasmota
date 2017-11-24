/*
  xsns_05_ds18x20.ino - DS18x20 temperature sensor support for Sonoff-Tasmota

  Copyright (C) 2017  Heiko Krupp and Theo Arends

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

#ifdef USE_DS18x20
/*********************************************************************************************\
 * DS18B20 - Temperature
\*********************************************************************************************/

#define DS18S20_CHIPID       0x10  // +/-0.5C 9-bit
#define DS1822_CHIPID        0x22  // +/-2C 12-bit
#define DS18B20_CHIPID       0x28  // +/-0.5C 12-bit
#define MAX31850_CHIPID      0x3B  // +/-0.25C 14-bit

#define W1_SKIP_ROM          0xCC
#define W1_CONVERT_TEMP      0x44
#define W1_WRITE_EEPROM      0x48
#define W1_WRITE_SCRATCHPAD  0x4E
#define W1_READ_SCRATCHPAD   0xBE

#define DS18X20_MAX_SENSORS  8

const char kDs18x20Types[] PROGMEM = "DS18x20|DS18S20|DS1822|DS18B20|MAX31850";

#include <OneWire.h>

OneWire *ds = NULL;

uint8_t ds18x20_chipids[] = { 0, DS18S20_CHIPID, DS1822_CHIPID, DS18B20_CHIPID, MAX31850_CHIPID };
uint8_t ds18x20_address[DS18X20_MAX_SENSORS][8];
uint8_t ds18x20_index[DS18X20_MAX_SENSORS] = { 0 };
uint8_t ds18x20_sensors = 0;
char ds18x20_types[9];

void Ds18x20Init()
{
  ds = new OneWire(pin[GPIO_DSB]);
  ds->reset_search();
  for (ds18x20_sensors = 0; ds18x20_sensors < DS18X20_MAX_SENSORS; ds18x20_sensors) {
    if (!ds->search(ds18x20_address[ds18x20_sensors])) {
      ds->reset_search();
      break;
    }
    if ((OneWire::crc8(ds18x20_address[ds18x20_sensors], 7) == ds18x20_address[ds18x20_sensors][7]) &&
       ((ds18x20_address[ds18x20_sensors][0] == DS18S20_CHIPID) ||
        (ds18x20_address[ds18x20_sensors][0] == DS1822_CHIPID) ||
        (ds18x20_address[ds18x20_sensors][0] == DS18B20_CHIPID) ||
        (ds18x20_address[ds18x20_sensors][0] == MAX31850_CHIPID))) {
      ds18x20_index[ds18x20_sensors] = ds18x20_sensors;
      ds18x20_sensors++;
    }
  }
  for (byte i = 0; i < ds18x20_sensors; i++) {
    for (byte j = i + 1; j < ds18x20_sensors; j++) {
      if (uint32_t(ds18x20_address[ds18x20_index[i]]) > uint32_t(ds18x20_address[ds18x20_index[j]])) {
        std::swap(ds18x20_index[i], ds18x20_index[j]);
      }
    }
  }
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DSB D_SENSORS_FOUND " %d"), ds18x20_sensors);
  AddLog(LOG_LEVEL_DEBUG);
}

void Ds18x20Convert()
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
    case DS1822_CHIPID:
    case DS18B20_CHIPID:
      if (data[4] != 0x7F) {
        data[4] = 0x7F;                 // Set resolution to 12-bit
        ds->reset();
        ds->select(ds18x20_address[ds18x20_index[sensor]]);
        ds->write(W1_WRITE_SCRATCHPAD); // Write Scratchpad
        ds->write(data[2]);             // Th Register
        ds->write(data[3]);             // Tl Register
        ds->write(data[4]);             // Configuration Register
        ds->select(ds18x20_address[ds18x20_index[sensor]]);
        ds->write(W1_WRITE_EEPROM);     // Save scratchpad to EEPROM
      }
    case MAX31850_CHIPID:
      uint16_t temp12 = (data[1] << 8) + data[0];
      if (temp12 > 2047) {
        temp12 = (~temp12) +1;
        sign = -1;
      }
      t = ConvertTemp(sign * temp12 * 0.0625);
      break;
    }
  } else {
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DSB D_SENSOR_CRC_ERROR));
  }
  return (!isnan(t));
}

/********************************************************************************************/

void Ds18x20Show(boolean json)
{
  char temperature[10];
  char stemp[12];
  char separator[2] = { '\0' };
  float t;
  bool domoticz_flag = true;

  for (byte i = 0; i < ds18x20_sensors; i++) {
    if (Ds18x20Read(i, t)) {           // Check if read failed
      dtostrfd(t, Settings.flag2.temperature_resolution, temperature);

      uint8_t index = sizeof(ds18x20_chipids);
      while (index) {
        if (ds18x20_address[ds18x20_index[i]][0] == ds18x20_chipids[index]) {
          break;
        }
        index--;
      }
      GetTextIndexed(ds18x20_types, sizeof(ds18x20_types), index, kDs18x20Types);
      if (json) {
        if (1 == ds18x20_sensors) {
          snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"%s\":{\"" D_TEMPERATURE "\":%s}"), mqtt_data, ds18x20_types, temperature);
        } else {
          char address[17];
//          for (byte j = 0; j < 8; j++) {
//            sprintf(address+2*j, "%02X", ds18x20_address[ds18x20_index[i]][j]);
//          }
          for (byte j = 0; j < 6; j++) {
            sprintf(address+2*j, "%02X", ds18x20_address[ds18x20_index[i]][6-j]);  // Skip sensor type and crc
          }
          if (!separator[0]) {
            snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"DS18x20\":{"), mqtt_data);
          }
          snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"DS%d\":{\"" D_TYPE "\":\"%s\",\"" D_ADDRESS "\":\"%s\",\"" D_TEMPERATURE "\":%s}"),
            mqtt_data, separator, i +1, ds18x20_types, address, temperature);
          separator[0] = ',';
        }
#ifdef USE_DOMOTICZ
        if (domoticz_flag) {
          DomoticzSensor(DZ_TEMP, temperature);
          domoticz_flag = false;
        }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
      } else {
        snprintf_P(stemp, sizeof(stemp), PSTR("%s-%d"), ds18x20_types, i +1);
        snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_TEMP, mqtt_data, (1 == ds18x20_sensors) ? ds18x20_types : stemp, temperature, TempUnit());
#endif  // USE_WEBSERVER
      }
    }
  }
  if (json) {
    if (separator[0]) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_05

boolean Xsns05(byte function)
{
  boolean result = false;

  if (pin[GPIO_DSB] < 99) {
    switch (function) {
      case FUNC_XSNS_INIT:
        Ds18x20Init();
        break;
      case FUNC_XSNS_PREP:
        Ds18x20Convert();   // Check for changes in sensor number and start conversion, takes up to one second
        break;
      case FUNC_XSNS_JSON_APPEND:
        Ds18x20Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_XSNS_WEB:
        Ds18x20Show(0);
        Ds18x20Convert();   // Check for changes in sensor number and start conversion, takes up to one second
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_DS18x20
