/*
  xsns_116_ms5837.ino - BlueRobotics MS5837 pressure sensor support for Tasmota

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

#define XSNS_116            116
#define XI2C_91             91         // See I2CDEVICES.md
/*********************************************************************************************\
 * BlueRobotics Pressure Sensor
 *
 * This driver supports the following sensors:
 * - BlueRobotics MS5837
\*********************************************************************************************/

#include <Wire.h>
#include <MS5837.h>

MS5837 sensor_ms5837;

uint8_t ms5837Start = 0;
float pressureOffset = 2.85;

/********************************************************************************************/

void MS5837init(void) {

  if (I2cSetDevice(0x76)) {
    TwoWire& myWire = I2cGetWire();

    if(sensor_ms5837.init(myWire)) {
      sensor_ms5837.setModel(sensor_ms5837.MS5837_02BA);
      sensor_ms5837.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)
      ms5837Start = 1;
      I2cSetActiveFound(MS5837_ADDR, "MS5837");
    }
  }
}

#ifdef USE_WEBSERVER
// const char HTTP_SNS_MS5837_DEFAULT[] PROGMEM =
//   "{s}MS5837 Temperature {m}%s " D_UNIT_DEGREE            "%c{e}{s}MS5837 Pressure {m}%s %s{e}";  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
// const char HTTP_SNS_MS5837_INCHES_WATER[] PROGMEM =
//   "{s}Inches Water {m}%s in{e}";  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
// char HTTP_SNS_MS5837_DUAL[118];
const char name_str[] PROGMEM = "MS5837";
#endif  // USE_WEBSERVER

void MS5837Show(bool json) {
  float ms5837Temp;
  float ms5837Pres;
  float pressure_delta;
  float inches_water;
  char temperature_str[8];
  char pressure_str[8];
  char inchesWater_str[8];

  if (I2cEnabled(XI2C_91)) {
    sensor_ms5837.read();
    ms5837Temp = ConvertTemp(sensor_ms5837.temperature());
    ms5837Pres = ConvertPressure(sensor_ms5837.pressure());
    ext_snprintf_P(temperature_str, sizeof(temperature_str), PSTR("%1_f"), &ms5837Temp);
    ext_snprintf_P(pressure_str, sizeof(pressure_str), PSTR("%1_f"), &ms5837Pres);
    if (json) {
      ResponseAppend_P(PSTR(",\"MS5837\":{\"Temperature\":%s,\"Pressure\":%s"), temperature_str, pressure_str);
    }
    if (I2cEnabled(XI2C_10)) {
      pressure_delta = sensor_ms5837.pressure() - bmp_sensors[0].bmp_pressure + pressureOffset;
      inches_water = pressure_delta*0.401463078662f;
      ext_snprintf_P(inchesWater_str, sizeof(inchesWater_str), PSTR("%1_f"), &inches_water);
      if (json) {
        ResponseAppend_P(PSTR(",\"Inches Water\":%s"),inchesWater_str);
      }
    }
    if (json) {
      ResponseAppend_P(PSTR("}"));

#ifdef USE_WEBSERVER
    } else {
        // if (I2cEnabled(XI2C_10)) {
        //   strncat(HTTP_SNS_MS5837_DUAL,HTTP_SNS_MS5837_DEFAULT,sizeof(HTTP_SNS_MS5837_DUAL));
        //   strncat(HTTP_SNS_MS5837_DUAL,HTTP_SNS_MS5837_INCHES_WATER,sizeof(HTTP_SNS_MS5837_DUAL));
        //   WSContentSend_PD(HTTP_SNS_MS5837_DUAL, temperature_str, TempUnit(), pressure_str, PressureUnit().c_str(), inchesWater_str);
        // }
        // else {
        //   WSContentSend_PD(HTTP_SNS_MS5837_DEFAULT, temperature_str, TempUnit(), pressure_str, PressureUnit().c_str());
        // }
        
        WSContentSend_PD(HTTP_SNS_F_TEMP, name_str, &ms5837Temp, TempUnit());
        WSContentSend_PD(HTTP_SNS_PRESSURE, name_str, pressure_str, PressureUnit().c_str());
        if (I2cEnabled(XI2C_10)) {
          WSContentSend_PD(HTTP_SNS_WATER_DEPTH, name_str, inchesWater_str);
        }
#endif  // USE_WEBSERVER
    }
  }
}
/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns116(uint32_t function) {
  if (!I2cEnabled(XI2C_91)) { return false; }

  bool result = false;
  //I2cScan();

  if (FUNC_INIT == function) {
    MS5837init();
  }
  else if (ms5837Start) {
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

#endif  // USE_MS5837
#endif  // USE_I2C
