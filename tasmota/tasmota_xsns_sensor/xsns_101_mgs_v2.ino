/*
  xsns_101_mgs_v2.ino - Xadow and Grove Mutichannel Gas sensor support for Tasmota

  Copyright (C) 2021  Jeroen Vermeulen and Theo Arends

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
#ifdef USE_MGS_V2
/*********************************************************************************************\
 * Grove - Multichannel Gas Sensor
 * https://wiki.seeedstudio.com/Grove-Multichannel-Gas-Sensor-V2
 *
 * https://github.com/Seeed-Studio/Seeed_Arduino_MultiGas
\*********************************************************************************************/

#define XSNS_101           101
#define XI2C_73            73  // See I2CDEVICES.md

#ifndef MGS_V2_SENSOR_ADDR
#define MGS_V2_SENSOR_ADDR    0x08             // Default Mutichannel Gas sensor i2c address
#endif

#include "Multichannel_Gas_GMXXX.h"
GAS_GMXXX<TwoWire> gasGMXXX;
bool mgs_v2_detected = false;

void MGSv2Prepare(void)
{
  if (!I2cSetDevice(MGS_V2_SENSOR_ADDR)) { return; }

  gasGMXXX.begin(Wire, MGS_V2_SENSOR_ADDR);
  I2cSetActiveFound(MGS_V2_SENSOR_ADDR, "MultiGas");
  mgs_v2_detected = true;
}

#ifdef USE_WEBSERVER
const char HTTP_MGS_V2_GAS[] PROGMEM = "{s}MGSv2 %s{m}%d " D_UNIT_PARTS_PER_MILLION "{e}";  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif // USE_WEBSERVER

void MGSv2Show(bool json)
{
  char buffer[33];
  if (json) {
    ResponseAppend_P(PSTR(",\"NO2\":%d"), gasGMXXX.measure_NO2());
    ResponseAppend_P(PSTR(",\"C2H5OH\":%d}"), gasGMXXX.measure_C2H5OH());
    ResponseAppend_P(PSTR(",\"VOC\":%d"), gasGMXXX.measure_VOC());
    ResponseAppend_P(PSTR(",\"CO\":%d"), gasGMXXX.measure_CO());
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_MGS_V2_GAS, "NO2", gasGMXXX.measure_NO2());
    WSContentSend_PD(HTTP_MGS_V2_GAS, "C2H5OH", gasGMXXX.measure_C2H5OH());
    WSContentSend_PD(HTTP_MGS_V2_GAS, "VOC", gasGMXXX.measure_VOC());
    WSContentSend_PD(HTTP_MGS_V2_GAS, "CO", gasGMXXX.measure_CO());
#endif // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns101(uint8_t function)
{
  if (!I2cEnabled(XI2C_73)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    MGSv2Prepare();
  }
  else if (mgs_v2_detected) {
    switch (function) {
      case FUNC_JSON_APPEND:
        MGSv2Show(1);
        break;
  #ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        MGSv2Show(0);
        break;
  #endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_MGS_V2
#endif  // USE_I2C
