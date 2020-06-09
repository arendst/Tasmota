/*
  xsns_72_hp303b.ino - HP303B digital barometric air pressure sensor support for Tasmota

  Copyright (C) 2020  Theo Arends

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
#ifdef USE_I2C
#ifdef USE_HP303B
/*********************************************************************************************\
 * HP303B - Gas (TVOC - Total Volatile Organic Compounds) and Air Quality (CO2)
 *
 * Source: Lolin LOLIN_HP303B_Library
 *
 * I2C Address: 0x77 or 0x76
\*********************************************************************************************/

#define XSNS_73 73
#define XI2C_52 52 // See I2CDEVICES.md

#include <LOLIN_HP303B.h>
// HP303B Object
LOLIN_HP303B HP303BSensor = LOLIN_HP303B();

#define HP303B_MAX_SENSORS    2
#define HP303B_START_ADDRESS  0x76

struct {
char types[7] = "HP303B";
uint8_t count  = 0;
int16_t oversampling = 7;
} hp303b_cfg;

struct BHP303B {
  uint8_t address;
  uint8_t valid = 0;
  float temperature = NAN;
  float pressure = NAN;
} hp303b_sensor[HP303B_MAX_SENSORS];
/*********************************************************************************************/

bool HP303B_Read(uint8_t hp303b_idx)
{
  if (hp303b_sensor[hp303b_idx].valid) { hp303b_sensor[hp303b_idx].valid--; }

  float t;
  float p;
  int16_t ret;

  ret = HP303BSensor.measureTempOnce(t, hp303b_cfg.oversampling);
  if (ret != 0)
    return false;

  ret = HP303BSensor.measurePressureOnce(p, hp303b_cfg.oversampling);
  if (ret != 0)
    return false;

  hp303b_sensor[hp303b_idx].temperature = (float)ConvertTemp(t);
  hp303b_sensor[hp303b_idx].pressure = (float)ConvertPressure(p) / 100; //conversion to hPa

  hp303b_sensor[hp303b_idx].valid = SENSOR_MAX_MISS;
  return true;
}

/********************************************************************************************/

void HP303B_Detect(void)
{
  for (uint32_t i = 0; i < HP303B_MAX_SENSORS; i++)
  {
    if (!I2cSetDevice(HP303B_START_ADDRESS + i )) { continue; }

    if (HP303BSensor.begin(HP303B_START_ADDRESS + i))
    {
      hp303b_sensor[hp303b_cfg.count].address = HP303B_START_ADDRESS + i;
      I2cSetActiveFound(hp303b_sensor[hp303b_cfg.count].address, hp303b_cfg.types);
      hp303b_cfg.count++;
    }
  }
}

void HP303B_EverySecond(void)
{
  for (uint32_t i = 0; i < hp303b_cfg.count; i++) {
    if (uptime &1) {
    if (!HP303B_Read(i)) {
      AddLogMissed(hp303b_cfg.types, hp303b_sensor[i].valid);
    }
  }
  }
}

void HP303B_Show(bool json)
{
  for (uint32_t i = 0; i < hp303b_cfg.count; i++) {
    char sensor_name[12];
    strlcpy(sensor_name, hp303b_cfg.types, sizeof(sensor_name));
    if (hp303b_cfg.count > 1) {
      snprintf_P(sensor_name, sizeof(sensor_name), PSTR("%s%c0x%02X"), sensor_name, IndexSeparator(), hp303b_sensor[i].address); // HP303B-0x76, HP303B-0x77
    }

    if (hp303b_sensor[i].valid)
    {
      char str_temperature[33];
      dtostrfd(hp303b_sensor[i].temperature, Settings.flag2.temperature_resolution, str_temperature);
      char str_pressure[33];
      dtostrfd(hp303b_sensor[i].pressure, Settings.flag2.pressure_resolution, str_pressure);

      if (json)
      {
        ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_TEMPERATURE "\":%s,\"" D_JSON_PRESSURE "\":%s"), sensor_name, str_temperature,  str_pressure);
        ResponseJsonEnd();
  #ifdef USE_DOMOTICZ
        // Domoticz and knx only support one temp sensor
        if ((0 == tele_period) && (0 == i)) {
          DomoticzSensor(DZ_TEMP, hp303b_sensor[i].temperature);
        }
  #endif // USE_DOMOTICZ
  #ifdef USE_WEBSERVER
      }
      else
      {
        WSContentSend_PD(HTTP_SNS_TEMP, sensor_name, str_temperature, TempUnit());
        WSContentSend_PD(HTTP_SNS_PRESSURE, sensor_name, str_pressure, PressureUnit().c_str());
  #endif // USE_WEBSERVER
      }
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns73(uint8_t function)
{
  if (!I2cEnabled(XI2C_52)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    HP303B_Detect();
  }
  else if (hp303b_cfg.count)
  {
    switch (function)
    {
      case FUNC_EVERY_SECOND:
      HP303B_EverySecond();
      break;
    case FUNC_JSON_APPEND:
      HP303B_Show(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      HP303B_Show(0);
      break;
#endif // USE_WEBSERVER
    }
  }
  return result;
}

#endif // USE_HP303B
#endif // USE_I2C
