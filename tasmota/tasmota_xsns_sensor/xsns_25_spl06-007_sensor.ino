/*
  xsns_25_spl06-007_sensor.ino - SPL06-007 Temperature and Pressure sensor support for Tasmota

  Copyright (C) 2024 rai68

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
#ifdef USE_SPL06_007

/*********************************************************************************************\
 * SPL006/7 Temperature and Pressure sensor support for Tasmota
 *
 * Source: rv701
 *
 * I2C Address: 0x76
\*********************************************************************************************/

#define XSNS_25             25
#define XI2C_87             87  // See I2CDEVICES.md

#include "SPL06-007.h"

#define SPL007_ADDRESS     0x76

struct SPL007 {
  float temperature = NAN;
  float pressure = NAN;
  uint8_t address;
  bool valid = false;
} spl007_s;

/********************************************************************************************/

void spl007Detect(void)
{
  if (!I2cActive(SPL007_ADDRESS))
  {
    SPL_init();
    if (get_spl_id() != 16) {
//      AddLog(LOG_LEVEL_ERROR, PSTR("SPL: @%02X not found error!"), SPL007_ADDRESS);
      return;
    }
    spl007_s.valid = true;
    I2cSetActiveFound(SPL007_ADDRESS, "SPL06-007");
  }
}

void spl007Update(void)  // Perform every n second
{
    spl007_s.temperature = ConvertTemp(get_temp_c());
    spl007_s.pressure = ConvertPressure(get_pressure());
}

void spl007Show(bool json)
{

    float sealevel = ConvertPressureForSeaLevel(spl007_s.pressure);
    char sea_pressure[33];
    dtostrfd(sealevel, Settings->flag2.pressure_resolution, sea_pressure);

    char str_pressure[33];
    dtostrfd(spl007_s.pressure, Settings->flag2.pressure_resolution, str_pressure);

    if (json) {
    ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_TEMPERATURE "\":%*_f,\"" D_JSON_PRESSURE "\":%s"),
        "SPL06-007", Settings->flag2.temperature_resolution, &spl007_s.temperature,  str_pressure);
    if (Settings->altitude != 0) {
        ResponseAppend_P(PSTR(",\"" D_JSON_PRESSUREATSEALEVEL "\":%s"), sea_pressure);
    }
    ResponseJsonEnd();
#ifdef USE_DOMOTICZ
        // Domoticz and knx only support one temp sensor
        if ((0 == TasmotaGlobal.tele_period)) {
          DomoticzFloatSensor(DZ_TEMP, spl007_s.temperature);
        }
#endif // USE_DOMOTICZ
#ifdef USE_WEBSERVER
      } else {

        WSContentSend_Temp("SPL06-007", spl007_s.temperature);
        WSContentSend_PD(HTTP_SNS_PRESSURE, "SPL06-007", str_pressure, PressureUnit().c_str());
        if (Settings->altitude != 0) {
          WSContentSend_PD(HTTP_SNS_SEAPRESSURE, "SPL06-007", sea_pressure, PressureUnit().c_str());
        }
#endif // USE_WEBSERVER
      }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns25(uint32_t function) 
{
  if (!I2cEnabled(XI2C_87)) { return false; }
  bool result = false;

  if (FUNC_INIT == function) {
    spl007Detect();
  }
  else if (spl007_s.valid) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        spl007Update();
        break;
      case FUNC_JSON_APPEND:
        spl007Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        spl007Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_SPL06_007
#endif  // USE_I2C
