/*
  xsns_72_mcp9808 - MCP9808 I2C temperature sensor support for Tasmota

  Copyright (C) 2020  Martin Wagner and Theo Arends

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
#ifdef USE_MCP9808
/*********************************************************************************************\
 * MCP9808 - Temperature Sensor
 *
 * I2C Address: 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
 *
\*********************************************************************************************/

#define XSNS_72              72
#define XI2C_51              51  // See I2CDEVICES.md

#include "Adafruit_MCP9808.h"
Adafruit_MCP9808 mcp9808 = Adafruit_MCP9808(); // create object copy

#define MCP9808_MAX_SENSORS    8
#define MCP9808_START_ADDRESS  0x18

struct {
char types[9] = "MCP9808";
uint8_t count  = 0;
} mcp9808_cfg;

struct {
  float   temperature = NAN;
  uint8_t address;
} mcp9808_sensors[MCP9808_MAX_SENSORS];

/********************************************************************************************/

float MCP9808Read(uint8_t addr) {
  float t = mcp9808.readTempC(addr);
  return t;
}

void MCP9808Detect(void) {
  for (uint8_t i = 0; i < MCP9808_MAX_SENSORS; i++) {
    if (!I2cSetDevice(MCP9808_START_ADDRESS + i)) { continue; }    

    if (mcp9808.begin(MCP9808_START_ADDRESS + i)) {    
      mcp9808_sensors[mcp9808_cfg.count].address = MCP9808_START_ADDRESS + i;
      I2cSetActiveFound(mcp9808_sensors[mcp9808_cfg.count].address, mcp9808_cfg.types);
      mcp9808.setResolution (mcp9808_sensors[mcp9808_cfg.count].address, 2); // Set Resolution to 0.125°C
      mcp9808_cfg.count++;
    }
  }
}

void MCP9808EverySecond(void) {
  for (uint32_t i = 0; i < mcp9808_cfg.count; i++) {
    float t = MCP9808Read(mcp9808_sensors[i].address);
    mcp9808_sensors[i].temperature = ConvertTemp(t);
  }
}

void MCP9808Show(bool json) {
  for (uint32_t i = 0; i < mcp9808_cfg.count; i++) {
    char temperature[33];
    dtostrfd(mcp9808_sensors[i].temperature, Settings.flag2.temperature_resolution, temperature);
    
    char sensor_name[10];
    strlcpy(sensor_name, mcp9808_cfg.types, sizeof(sensor_name));
    if (mcp9808_cfg.count > 1) {
      snprintf_P(sensor_name, sizeof(sensor_name), PSTR("%s%c%d"), sensor_name, IndexSeparator(), i +1); // MCP9808-1
    }

    if (json) {
      ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_TEMPERATURE "\":%s}"), sensor_name, temperature);
      if (0 == tele_period) {
#ifdef USE_DOMOTICZ
        DomoticzSensor(DZ_TEMP, temperature);
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
        KnxSensor(KNX_TEMPERATURE, mcp9808_sensors[i].temperature);
#endif // USE_KNX
      }
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_TEMP, sensor_name, temperature, TempUnit());
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns72(uint8_t function)
{
  if (!I2cEnabled(XI2C_51)) { return false; }
  bool result = false;

  if (FUNC_INIT == function) {
    MCP9808Detect();
  }
  else if (mcp9808_cfg.count){
    switch (function) {
      case FUNC_EVERY_SECOND:
        MCP9808EverySecond();
        break;
      case FUNC_JSON_APPEND:
        MCP9808Show(1);
        break;
  #ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        MCP9808Show(0);
        break;
  #endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_MCP9808
#endif  // USE_I2C