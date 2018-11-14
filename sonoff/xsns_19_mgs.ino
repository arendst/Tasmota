/*
  xsns_19_mgs.ino - Xadow and Grove Mutichannel Gas sensor support for Sonoff-Tasmota

  Copyright (C) 2018  Palich2000 and Theo Arends

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
#ifdef USE_MGS
/*********************************************************************************************\
 * Grove - Multichannel Gas Sensor
 * http://wiki.seeed.cc/Grove-Multichannel_Gas_Sensor/
 *
 * https://github.com/Seeed-Studio/Mutichannel_Gas_Sensor.git
\*********************************************************************************************/

#define XSNS_19            19

#ifndef MGS_SENSOR_ADDR
#define MGS_SENSOR_ADDR    0x04             // Default Mutichannel Gas sensor i2c address
#endif

#include "MutichannelGasSensor.h"

void MGSInit(void) {
  gas.begin(MGS_SENSOR_ADDR);
}

boolean MGSPrepare(void)
{
  gas.begin(MGS_SENSOR_ADDR);
  if (!gas.isError()) {
    snprintf_P(log_data, sizeof(log_data), S_LOG_I2C_FOUND_AT, "MultiGasSensor", MGS_SENSOR_ADDR);
    AddLog(LOG_LEVEL_DEBUG);
    return true;
  } else {
    return false;
  }
}

char* measure_gas(int gas_type, char* buffer)
{
  float f = gas.calcGas(gas_type);
  dtostrfd(f, 2, buffer);
  return buffer;
}

#ifdef USE_WEBSERVER
const char HTTP_MGS_GAS[] PROGMEM = "%s{s}MGS %s{m}%s " D_UNIT_PARTS_PER_MILLION "{e}";  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif // USE_WEBSERVER

void MGSShow(boolean json)
{
  char buffer[25];
  if (json) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"MGS\":{\"NH3\":%s"), mqtt_data, measure_gas(NH3, buffer));
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"CO\":%s"), mqtt_data, measure_gas(CO, buffer));
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"NO2\":%s"), mqtt_data, measure_gas(NO2, buffer));
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"C3H8\":%s"), mqtt_data, measure_gas(C3H8, buffer));
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"C4H10\":%s"), mqtt_data, measure_gas(C4H10, buffer));
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"CH4\":%s"), mqtt_data, measure_gas(GAS_CH4, buffer));
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"H2\":%s"), mqtt_data, measure_gas(H2, buffer));
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"C2H5OH\":%s}"), mqtt_data, measure_gas(C2H5OH, buffer));
#ifdef USE_WEBSERVER
  } else {
    snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_MGS_GAS, mqtt_data, "NH3", measure_gas(NH3, buffer));
    snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_MGS_GAS, mqtt_data, "CO", measure_gas(CO, buffer));
    snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_MGS_GAS, mqtt_data, "NO2", measure_gas(NO2, buffer));
    snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_MGS_GAS, mqtt_data, "C3H8", measure_gas(C3H8, buffer));
    snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_MGS_GAS, mqtt_data, "C4H10", measure_gas(C4H10, buffer));
    snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_MGS_GAS, mqtt_data, "CH4", measure_gas(GAS_CH4, buffer));
    snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_MGS_GAS, mqtt_data, "H2", measure_gas(H2, buffer));
    snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_MGS_GAS, mqtt_data, "C2H5OH", measure_gas(C2H5OH, buffer));
#endif // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

boolean Xsns19(byte function)
{
  boolean result = false;
  static int detected = false;

  if (i2c_flg) {
    switch (function) {
      case FUNC_INIT:
//        MGSInit();
        break;
      case FUNC_PREP_BEFORE_TELEPERIOD:
        detected = MGSPrepare();
        break;
      case FUNC_JSON_APPEND:
        if (detected) MGSShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        if (detected) MGSShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_MGS
#endif  // USE_I2C
