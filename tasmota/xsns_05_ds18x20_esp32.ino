/*
  xsns_05_ds18x20_esp32.ino - DS18x20 temperature sensor support for Tasmota

  Copyright (C) 2021  Heiko Krupp and Theo Arends

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

#ifdef ESP32
#ifdef USE_DS18x20
/*********************************************************************************************\
 * DS18B20 - Temperature - Multiple sensors
\*********************************************************************************************/

#define XSNS_05              5

//#define DS18x20_USE_ID_AS_NAME      // Use last 3 bytes for naming of sensors

#define DS18S20_CHIPID       0x10  // +/-0.5C 9-bit
#define DS1822_CHIPID        0x22  // +/-2C 12-bit
#define DS18B20_CHIPID       0x28  // +/-0.5C 12-bit
#define MAX31850_CHIPID      0x3B  // +/-0.25C 14-bit

#define W1_SKIP_ROM          0xCC
#define W1_CONVERT_TEMP      0x44
#define W1_READ_SCRATCHPAD   0xBE

#define DS18X20_MAX_SENSORS  8

const char kDs18x20Types[] PROGMEM = "DS18x20|DS18S20|DS1822|DS18B20|MAX31850";

uint8_t ds18x20_chipids[] = { 0, DS18S20_CHIPID, DS1822_CHIPID, DS18B20_CHIPID, MAX31850_CHIPID };

uint8_t ds18x20_address[DS18X20_MAX_SENSORS][8];
uint8_t ds18x20_index[DS18X20_MAX_SENSORS];
uint8_t ds18x20_valid[DS18X20_MAX_SENSORS];
uint8_t ds18x20_sensors = 0;
char ds18x20_types[17];

/********************************************************************************************/

#include <OneWire.h>

OneWire *ds = nullptr;

void Ds18x20Init(void) {
  ds = new OneWire(Pin(GPIO_DSB));

  Ds18x20Search();
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DSB D_SENSORS_FOUND " %d"), ds18x20_sensors);
}

void Ds18x20Search(void) {
  uint8_t num_sensors=0;
  uint8_t sensor = 0;

  ds->reset_search();
  for (num_sensors = 0; num_sensors < DS18X20_MAX_SENSORS; num_sensors) {
    if (!ds->search(ds18x20_address[num_sensors])) {
      ds->reset_search();
      break;
    }
    // If CRC Ok and Type DS18S20, DS1822, DS18B20 or MAX31850
    if ((OneWire::crc8(ds18x20_address[num_sensors], 7) == ds18x20_address[num_sensors][7]) &&
       ((ds18x20_address[num_sensors][0]==DS18S20_CHIPID) ||
        (ds18x20_address[num_sensors][0]==DS1822_CHIPID) ||
        (ds18x20_address[num_sensors][0]==DS18B20_CHIPID) ||
        (ds18x20_address[num_sensors][0]==MAX31850_CHIPID))) {
      num_sensors++;
    }
  }
  for (uint32_t i = 0; i < num_sensors; i++) {
    ds18x20_index[i] = i;
  }
  for (uint32_t i = 0; i < num_sensors; i++) {
    for (uint32_t j = i + 1; j < num_sensors; j++) {
      if (uint32_t(ds18x20_address[ds18x20_index[i]]) > uint32_t(ds18x20_address[ds18x20_index[j]])) {
        std::swap(ds18x20_index[i], ds18x20_index[j]);
      }
    }
  }
  ds18x20_sensors = num_sensors;
}

void Ds18x20Convert(void) {
  ds->reset();
  ds->write(W1_SKIP_ROM);        // Address all Sensors on Bus
  ds->write(W1_CONVERT_TEMP);    // start conversion, no parasite power on at the end
//  delay(750);                   // 750ms should be enough for 12bit conv
}

bool Ds18x20Read(uint8_t sensor, float &t)
{
  uint8_t data[12];
  int8_t sign = 1;

  t = NAN;

  uint8_t index = ds18x20_index[sensor];
  if (ds18x20_valid[index]) { ds18x20_valid[index]--; }

  ds->reset();
  ds->select(ds18x20_address[index]);
  ds->write(W1_READ_SCRATCHPAD); // Read Scratchpad

  for (uint32_t i = 0; i < 9; i++) {
    data[i] = ds->read();
  }
  if (OneWire::crc8(data, 8) == data[8]) {
    switch(ds18x20_address[index][0]) {
      case DS18S20_CHIPID: {
        int16_t tempS = (((data[1] << 8) | (data[0] & 0xFE)) << 3) | ((0x10 - data[6]) & 0x0F);
        t = ConvertTemp(tempS * 0.0625 - 0.250);
        ds18x20_valid[index] = SENSOR_MAX_MISS;
        return true;
      }
      case DS1822_CHIPID:
      case DS18B20_CHIPID: {
        uint16_t temp12 = (data[1] << 8) + data[0];
        if (temp12 > 2047) {
          temp12 = (~temp12) +1;
          sign = -1;
        }
        t = ConvertTemp(sign * temp12 * 0.0625);  // Divide by 16
        ds18x20_valid[index] = SENSOR_MAX_MISS;
        return true;
      }
      case MAX31850_CHIPID: {
        int16_t temp14 = (data[1] << 8) + (data[0] & 0xFC);
        t = ConvertTemp(temp14 * 0.0625);  // Divide by 16
        ds18x20_valid[index] = SENSOR_MAX_MISS;
        return true;
      }
    }
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DSB D_SENSOR_CRC_ERROR));
  return false;
}

void Ds18x20Name(uint8_t sensor)
{
  uint8_t index = sizeof(ds18x20_chipids);
  while (index) {
    if (ds18x20_address[ds18x20_index[sensor]][0] == ds18x20_chipids[index]) {
      break;
    }
    index--;
  }
  GetTextIndexed(ds18x20_types, sizeof(ds18x20_types), index, kDs18x20Types);
  if (ds18x20_sensors > 1) {
#ifdef DS18x20_USE_ID_AS_NAME
    char address[17];
    for (uint32_t j = 0; j < 3; j++) {
      sprintf(address+2*j, "%02X", ds18x20_sensor[ds18x20_sensor[sensor].index].address[3-j]);  // Only last 3 bytes
    }
    snprintf_P(ds18x20_types, sizeof(ds18x20_types), PSTR("%s%c%s"), ds18x20_types, IndexSeparator(), address);
#else
    snprintf_P(ds18x20_types, sizeof(ds18x20_types), PSTR("%s%c%d"), ds18x20_types, IndexSeparator(), sensor +1);
#endif
  }
}

/********************************************************************************************/

void Ds18x20EverySecond(void)
{
  if (!ds18x20_sensors) { return; }

  if (TasmotaGlobal.uptime & 1) {
    // 2mS
//    Ds18x20Search();      // Check for changes in sensors number
    Ds18x20Convert();     // Start Conversion, takes up to one second
  } else {
    float t;
    for (uint32_t i = 0; i < ds18x20_sensors; i++) {
      // 12mS per device
      if (!Ds18x20Read(i, t)) {   // Read temperature
        Ds18x20Name(i);
        AddLogMissed(ds18x20_types, ds18x20_valid[ds18x20_index[i]]);
      }
    }
  }
}

void Ds18x20Show(bool json)
{
  float t;

  uint8_t dsxflg = 0;
  for (uint32_t i = 0; i < ds18x20_sensors; i++) {
    if (Ds18x20Read(i, t)) {           // Check if read failed
      Ds18x20Name(i);

      if (json) {
        char address[17];
        for (uint32_t j = 0; j < 6; j++) {
          sprintf(address+2*j, "%02X", ds18x20_address[ds18x20_index[i]][6-j]);  // Skip sensor type and crc
        }
        ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_ID "\":\"%s\",\"" D_JSON_TEMPERATURE "\":%*_f}"),
          ds18x20_types, address, Settings.flag2.temperature_resolution, &t);
        dsxflg++;
#ifdef USE_DOMOTICZ
        if ((0 == TasmotaGlobal.tele_period) && (1 == dsxflg)) {
          DomoticzFloatSensor(DZ_TEMP, t);
        }
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
        if ((0 == TasmotaGlobal.tele_period) && (1 == dsxflg)) {
          KnxSensor(KNX_TEMPERATURE, t);
        }
#endif  // USE_KNX
#ifdef USE_WEBSERVER
      } else {
        WSContentSend_Temp(ds18x20_types, t);
#endif  // USE_WEBSERVER
      }
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns05(uint8_t function)
{
  bool result = false;

  if (PinUsed(GPIO_DSB)) {
    switch (function) {
      case FUNC_INIT:
        Ds18x20Init();
        break;
      case FUNC_EVERY_SECOND:
        Ds18x20EverySecond();
        break;
      case FUNC_JSON_APPEND:
        Ds18x20Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Ds18x20Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_DS18x20
#endif  // ESP32
