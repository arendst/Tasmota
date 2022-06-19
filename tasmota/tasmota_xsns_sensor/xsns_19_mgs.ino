/*
  xsns_19_mgs.ino - Xadow and Grove Mutichannel Gas sensor support for Tasmota

  Copyright (C) 2021  Palich2000 and Theo Arends

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
#define XI2C_17            17  // See I2CDEVICES.md

#ifndef MGS_SENSOR_ADDR
#define MGS_SENSOR_ADDR    0x04             // Default Mutichannel Gas sensor i2c address
#endif

#include "MutichannelGasSensor.h"

bool mgs_detected = false;

void MGSPrepare(void)
{
  if (!I2cSetDevice(MGS_SENSOR_ADDR)) { return; }

  gas.begin(MGS_SENSOR_ADDR);
  if (!gas.isError()) {
    I2cSetActiveFound(MGS_SENSOR_ADDR, "MultiGas");
    mgs_detected = true;
  }
}

char* measure_gas(int gas_type, char* buffer)
{
  float f = gas.calcGas(gas_type);
  dtostrfd(f, 2, buffer);
  return buffer;
}

#ifdef USE_WEBSERVER
const char HTTP_MGS_GAS[] PROGMEM = "{s}MGS %s{m}%s " D_UNIT_PARTS_PER_MILLION "{e}";  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif // USE_WEBSERVER

void MGSShow(bool json)
{
  char buffer[33];
  if (json) {
    ResponseAppend_P(PSTR(",\"MGS\":{\"NH3\":%s"), measure_gas(NH3, buffer));
    ResponseAppend_P(PSTR(",\"CO\":%s"), measure_gas(CO, buffer));
    ResponseAppend_P(PSTR(",\"NO2\":%s"), measure_gas(NO2, buffer));
    ResponseAppend_P(PSTR(",\"C3H8\":%s"), measure_gas(C3H8, buffer));
    ResponseAppend_P(PSTR(",\"C4H10\":%s"), measure_gas(C4H10, buffer));
    ResponseAppend_P(PSTR(",\"CH4\":%s"), measure_gas(GAS_CH4, buffer));
    ResponseAppend_P(PSTR(",\"H2\":%s"), measure_gas(H2, buffer));
    ResponseAppend_P(PSTR(",\"C2H5OH\":%s}"), measure_gas(C2H5OH, buffer));
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_MGS_GAS, "NH3", measure_gas(NH3, buffer));
    WSContentSend_PD(HTTP_MGS_GAS, "CO", measure_gas(CO, buffer));
    WSContentSend_PD(HTTP_MGS_GAS, "NO2", measure_gas(NO2, buffer));
    WSContentSend_PD(HTTP_MGS_GAS, "C3H8", measure_gas(C3H8, buffer));
    WSContentSend_PD(HTTP_MGS_GAS, "C4H10", measure_gas(C4H10, buffer));
    WSContentSend_PD(HTTP_MGS_GAS, "CH4", measure_gas(GAS_CH4, buffer));
    WSContentSend_PD(HTTP_MGS_GAS, "H2", measure_gas(H2, buffer));
    WSContentSend_PD(HTTP_MGS_GAS, "C2H5OH", measure_gas(C2H5OH, buffer));
#endif // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns19(uint8_t function)
{
  if (!I2cEnabled(XI2C_17)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    MGSPrepare();
  }
  else if (mgs_detected) {
    switch (function) {
      case FUNC_JSON_APPEND:
        MGSShow(1);
        break;
  #ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        MGSShow(0);
        break;
  #endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_MGS
#endif  // USE_I2C
