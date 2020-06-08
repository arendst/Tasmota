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

#define HP303B_ADDR1 0x77
#define HP303B_ADDR2 0x76

#include <LOLIN_HP303B.h>
// HP303B Opject
LOLIN_HP303B HP303BSensor = LOLIN_HP303B();

uint8_t address;
uint8_t addresses[2] = {HP303B_ADDR1, HP303B_ADDR2};
uint8_t type = 0;
uint8_t valid = 0;
float temperature;
float pressure;
int16_t oversampling = 7;
char types[7] = "HP303B";

/*********************************************************************************************/

bool HP303B_Read(float &temperature, float &pressure, uint8_t hp303b_address)
{
  float t;
  float p;
  int16_t ret;

  ret = HP303BSensor.measureTempOnce(t, oversampling);
  if (ret != 0)
    return false;

  ret = HP303BSensor.measurePressureOnce(p, oversampling);
  if (ret != 0)
    return false;

  temperature = (float)ConvertTemp(t);
  pressure = (float)ConvertPressure(p) / 100; //conversion to hPa

  return true;
}

/********************************************************************************************/

void HP303B_Detect(void)
{
  for (uint32_t i = 0; i < sizeof(addresses); i++)
  {
    if (!I2cSetDevice(addresses[i]))
    {
      continue;
    }

    HP303BSensor.begin(addresses[i]);

    float t;
    float p;
    if (HP303B_Read(t, p, addresses[i]))
    {
      I2cSetActiveFound(addresses[i], types);
      address = addresses[i];
      type = 1;
      break;
    }
  }
}

void HP303B_Show(bool json)
{

  if (HP303B_Read(temperature, pressure, address))
  {
    char str_temperature[33];
    dtostrfd(temperature, Settings.flag2.temperature_resolution, str_temperature);
    char str_pressure[33];
    dtostrfd(pressure, Settings.flag2.pressure_resolution, str_pressure);

    if (json)
    {
      ResponseAppend_P(PSTR(",\"HP303B\":{\"" D_JSON_TEMPERATURE "\":%s,\"" D_JSON_PRESSURE "\":%s"), str_temperature,  str_pressure);
      ResponseJsonEnd();
#ifdef USE_DOMOTICZ
      if (0 == tele_period)
      {
        DomoticzSensor(DZ_TEMP, temperature);
      }
#endif // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    }
    else
    {
      WSContentSend_PD(HTTP_SNS_TEMP, "HP303B", str_temperature, TempUnit());
      WSContentSend_PD(HTTP_SNS_PRESSURE, "HP303B", str_pressure, PressureUnit().c_str());
#endif // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns73(uint8_t function)
{
  if (!I2cEnabled(XI2C_52))
  {
    return false;
  }

  bool result = false;

  if (FUNC_INIT == function)
  {
    HP303B_Detect();
  }
  else if (type)
  {
    switch (function)
    {
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
