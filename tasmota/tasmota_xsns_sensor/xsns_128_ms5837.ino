/*
  xsns_14_sht3x.ino - SHT3X, SHT4X and SHTCX temperature and humidity sensor support for Tasmota

  Copyright (C) 2022  Theo Arends, Stefan Tibus

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
#ifdef USE_MS5837

#define MS5837_ADDR         0x76

#define XSNS_128            128
#define XI2C_98             98         // See I2CDEVICES.md
/*********************************************************************************************\
 * BlueRobotics Pressure Sensor
 *
 * This driver supports the following sensors:
 * - BlueRobotics MS5837
\*********************************************************************************************/

#include <Wire.h>
#include <MS5837.h>

MS5837 sensor_ms5837;

uint8_t ms5837_start = 0;

/********************************************************************************************/

void MS5837init(void) {

  if (I2cSetDevice(0x76)) {
    TwoWire& myWire = I2cGetWire();

    if(sensor_ms5837.init(myWire)) {
      sensor_ms5837.setModel(sensor_ms5837.MS5837_02BA);
      sensor_ms5837.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)
      ms5837_start = 1;
      I2cSetActiveFound(MS5837_ADDR, "MS5837");
    }
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR("BME: Gas variant %d"), bme_dev[bmp_idx].variant_id);
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_MS5837[] PROGMEM =
  "{s}MS5837 Pressure {m}%s hPa{e}{s}MS5837 Temperature {m}%s C{e}";  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

void MS5837Show(bool json) {
  sensor_ms5837.read();

  char temperature_str[8];
  dtostrf(sensor_ms5837.temperature(), sizeof(temperature_str)-1, 3, temperature_str);
  char pressure_str[8];
  dtostrf(sensor_ms5837.pressure(), sizeof(pressure_str)-1, 3, pressure_str);
  if (json) {
    ResponseAppend_P(PSTR(",\"MS5837\":{\"Temperature\":%s,"), temperature_str);
    ResponseAppend_P(PSTR("\"Pressure\":%s}"), pressure_str);
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_MS5837, pressure_str, temperature_str);
#endif  // USE_WEBSERVER
    }
  if (I2cEnabled(XI2C_10)) {
    pressure_delta = sensor_ms5837.pressure() - bmp_sensors[0].bmp_pressure;
    inches_water = pressure_delta*0.401463078662;
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns128(uint32_t function) {
  //if (!I2cEnabled(XI2C_98)) { return false; }

  bool result = false;
  //I2cScan();

  if (FUNC_INIT == function) {
    MS5837init();
  }
  else if (ms5837_start) {
    switch (function) {
      case FUNC_JSON_APPEND:
        MS5837Show(1);
        break;
  #ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        MS5837Show(0);
        break;
  #endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_SHT3X
#endif  // USE_I2C
