/*
  xsns_116_ms5837.ino - BlueRobotics MS5837 pressure and temperature sensor support for Tasmota

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
/*********************************************************************************************\
 * BlueRobotics Pressure Sensor
 *
 * This driver supports the following sensors:
 * - BlueRobotics MS5837
 * 
 * I2C Address: 0x76
\*********************************************************************************************/

#define XSNS_116            116
#define XI2C_91             91         // See I2CDEVICES.md

#define MS5837_ADDR         0x76

#include <Wire.h>
#include <MS5837.h>

MS5837 ms5837_sensor;

bool ms5837_start = false;
float ms5837_pressure_offset = 2.85f;

/********************************************************************************************/

void MS5837init(void) {

  if (I2cSetDevice(0x76)) {
    TwoWire& myWire = I2cGetWire();

    if(ms5837_sensor.init(myWire)) {
      ms5837_sensor.setModel(ms5837_sensor.MS5837_02BA);
      ms5837_sensor.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)
      ms5837_start = true;
      if(!isnan(Settings->ms5837_pressure_offset)) {
        ms5837_pressure_offset = Settings->ms5837_pressure_offset;
      }
      I2cSetActiveFound(MS5837_ADDR, "MS5837");
    }
  }
}

#ifdef USE_WEBSERVER
const char name_str[] PROGMEM = "MS5837";
#endif  // USE_WEBSERVER

void MS5837Show(bool json) {
  float ms5837Temp;
  float ms5837Pres;
  float pressure_delta;
  float cm_water;
  char temperature_str[8];
  char pressure_str[8];
  char cmWater_str[8];

  if (I2cEnabled(XI2C_91)) {
    ms5837_sensor.read();
    ms5837Temp = ConvertTemp(ms5837_sensor.temperature());
    ms5837Pres = ConvertPressure(ms5837_sensor.pressure() + ms5837_pressure_offset);
    ext_snprintf_P(temperature_str, sizeof(temperature_str), PSTR("%1_f"), &ms5837Temp);
    ext_snprintf_P(pressure_str, sizeof(pressure_str), PSTR("%1_f"), &ms5837Pres);
    if (json) {
      ResponseAppend_P(PSTR(",\"MS5837\":{\"" D_JSON_TEMPERATURE "\":%s,\"" D_JSON_PRESSURE "\":%s"), temperature_str, pressure_str);
    }
#ifdef USE_BMP
    if (I2cEnabled(XI2C_10)) {
      pressure_delta = (ms5837_sensor.pressure() + ms5837_pressure_offset) - bmp_sensors[0].bmp_pressure;
      cm_water = pressure_delta*0.401463078662f*2.54f; // changes from inches to cm after read using 2.54cm/in conversion
      ext_snprintf_P(cmWater_str, sizeof(cmWater_str), PSTR("%1_f"), &cm_water);
      if (json) {
        ResponseAppend_P(PSTR(",\"" D_JSON_WATER_DEPTH "\":%s"),cmWater_str);
      }
    }
#endif  // USE_BMP
    if (json) {
      ResponseAppend_P(PSTR("}"));

#ifdef USE_WEBSERVER
    } else {
        WSContentSend_PD(HTTP_SNS_F_TEMP, name_str, Settings->flag2.temperature_resolution, &ms5837Temp, TempUnit());
        WSContentSend_PD(HTTP_SNS_PRESSURE, name_str, pressure_str, PressureUnit().c_str());
#ifdef USE_BMP
        if (I2cEnabled(XI2C_10)) {
          WSContentSend_PD(HTTP_SNS_WATER_DEPTH, name_str, &cmWater_str);
        }
#endif  // USE_BMP
#endif  // USE_WEBSERVER
    }
  }
}

bool ms5837CommandSensor() {
  bool serviced = true;
  char argument[XdrvMailbox.data_len];
  long value = 0;

  for (uint32_t ca = 0; ca < XdrvMailbox.data_len; ca++) {
    if ((' ' == XdrvMailbox.data[ca]) || ('=' == XdrvMailbox.data[ca])) { XdrvMailbox.data[ca] = ','; }
  }

  bool any_value = (strchr(XdrvMailbox.data, ',') != nullptr);
  if (any_value) { value = strtol(ArgV(argument, 2), nullptr, 10); }

  switch (XdrvMailbox.payload) {
    case 0:
      MS5837Show(0);
#ifdef USE_BMP
      ms5837_pressure_offset = bmp_sensors[0].bmp_pressure - ms5837_sensor.pressure();
#endif  // USE_BMP
      Settings->ms5837_pressure_offset = ms5837_pressure_offset;
      Response_P(PSTR("Set MS5837 pressure offset to %f"),ms5837_pressure_offset);
      break;
    case 1:
      ms5837_pressure_offset = value;
      Settings->ms5837_pressure_offset = ms5837_pressure_offset;
      Response_P(PSTR("Set MS5837 pressure offset to %f"),ms5837_pressure_offset);
      break;
  }
  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns116(uint32_t function) {
  if (!I2cEnabled(XI2C_91)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    MS5837init();
  }
  else if (ms5837_start) {
    switch (function) {
      case FUNC_COMMAND_SENSOR:
        if (XSNS_116 == XdrvMailbox.index) {
          result = ms5837CommandSensor();
        }
        break;
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