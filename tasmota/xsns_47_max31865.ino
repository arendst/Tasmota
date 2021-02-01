/*
  xsns_39_MAX31865.ino - MAX31865 thermocouple sensor support for Tasmota

  Copyright (C) 2021  Alberto Lopez Siemens

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

#ifdef USE_MAX31865
/*********************************************************************************************\
 * MAX31865 - Thermocouple
\*********************************************************************************************/

#define XSNS_47              47

#include "Adafruit_MAX31865.h"

// set defaults if not defined
#ifndef MAX31865_PTD_WIRES
  #define MAX31865_PTD_WIRES  2
#endif
#ifndef MAX31865_PTD_RES
  #define MAX31865_PTD_RES    100
#endif
#ifndef MAX31865_REF_RES
  #define MAX31865_REF_RES    430
#endif
#ifndef MAX31865_PTD_BIAS
  #define MAX31865_PTD_BIAS   0
#endif
#ifndef MAX31865_PTD_LOW_RES
  #define MAX31865_PTD_LOW_RES 0
#endif
#ifndef MAX31865_PTD_HIGH_RES
  #define MAX31865_PTD_HIGH_RES 0xffff
#endif

#if MAX31865_PTD_WIRES == 4
  #define MAX31865_PTD_WIRESCFG MAX31865_4WIRE
#elif MAX31865_PTD_WIRES == 3
  #define MAX31865_PTD_WIRESCFG MAX31865_3WIRE
#else
  #define MAX31865_PTD_WIRESCFG MAX31865_2WIRE
#endif  // MAX31865_PTD_WIRES
#ifdef MAX31865_PTD_WIRES1
  #if MAX31865_PTD_WIRES1 == 4
    #define MAX31865_PTD_WIRES1CFG MAX31865_4WIRE
  #elif MAX31865_PTD_WIRES == 3
    #define MAX31865_PTD_WIRES1CFG MAX31865_3WIRE
  #else
    #define MAX31865_PTD_WIRES1CFG MAX31865_2WIRE
  #endif
#endif  // MAX31865_PTD_WIRES1
#ifdef MAX31865_PTD_WIRES2
  #if MAX31865_PTD_WIRES2 == 4
    #define MAX31865_PTD_WIRES2CFG MAX31865_4WIRE
  #elif MAX31865_PTD_WIRES == 3
    #define MAX31865_PTD_WIRES2CFG MAX31865_3WIRE
  #else
    #define MAX31865_PTD_WIRES2CFG MAX31865_2WIRE
  #endif
#endif  // MAX31865_PTD_WIRES2
#ifdef MAX31865_PTD_WIRES3
  #if MAX31865_PTD_WIRES3 == 4
    #define MAX31865_PTD_WIRES3CFG MAX31865_4WIRE
  #elif MAX31865_PTD_WIRES == 3
    #define MAX31865_PTD_WIRES3CFG MAX31865_3WIRE
  #else
    #define MAX31865_PTD_WIRES3CFG MAX31865_2WIRE
  #endif
#endif  // MAX31865_PTD_WIRE3
#ifdef MAX31865_PTD_WIRES4
  #if MAX31865_PTD_WIRES4 == 4
    #define MAX31865_PTD_WIRES4CFG MAX31865_4WIRE
  #elif MAX31865_PTD_WIRES == 3
    #define MAX31865_PTD_WIRES4CFG MAX31865_3WIRE
  #else
    #define MAX31865_PTD_WIRES4CFG MAX31865_2WIRE
  #endif
#endif  // MAX31865_PTD_WIRES4
#ifdef MAX31865_PTD_WIRES5
  #if MAX31865_PTD_WIRES5 == 4
    #define MAX31865_PTD_WIRES5CFG MAX31865_4WIRE
  #elif MAX31865_PTD_WIRES == 3
    #define MAX31865_PTD_WIRES5CFG MAX31865_3WIRE
  #else
    #define MAX31865_PTD_WIRES5CFG MAX31865_2WIRE
  #endif
#endif  // MAX31865_PTD_WIRES5
#ifdef MAX31865_PTD_WIRES6
  #if MAX31865_PTD_WIRES6 == 4
    #define MAX31865_PTD_WIRES6CFG MAX31865_4WIRE
  #elif MAX31865_PTD_WIRES == 3
    #define MAX31865_PTD_WIRES6CFG MAX31865_3WIRE
  #else
    #define MAX31865_PTD_WIRES6CFG MAX31865_2WIRE
  #endif
#endif  // MAX31865_PTD_WIRES6

float RefRes [] = {
#ifdef MAX31865_REF_RES1
    MAX31865_REF_RES1,
#else
    MAX31865_REF_RES,
#endif
#ifdef MAX31865_REF_RES2
    MAX31865_REF_RES2,
#else
    MAX31865_REF_RES,
#endif
#ifdef MAX31865_REF_RES3
    MAX31865_REF_RES3,
#else
    MAX31865_REF_RES,
#endif
#ifdef MAX31865_REF_RES4
    MAX31865_REF_RES4,
#else
    MAX31865_REF_RES,
#endif
#ifdef MAX31865_REF_RES5
    MAX31865_REF_RES5,
#else
    MAX31865_REF_RES,
#endif
#ifdef MAX31865_REF_RES6
    MAX31865_REF_RES6
#else
    MAX31865_REF_RES
#endif
  };

int8_t max31865_init_status = 0;
uint8_t max31865_pins_used = 0; //used as a bit array

Adafruit_MAX31865 max31865[MAX_MAX31865S];

struct MAX31865_Result_Struct {
  uint8_t   ErrorCode;
  uint16_t  Rtd;
  float     PtdResistance;
  float     PtdTemp;
} MAX31865_Result[MAX_MAX31865S];

void MAX31865_Init(void) {
  if (max31865_init_status) { return; }

  max31865_numwires_t PtdWires [] = {
#ifdef MAX31865_PTD_WIRES1
    MAX31865_PTD_WIRES1CFG,
#else
    MAX31865_PTD_WIRESCFG,
#endif
#ifdef MAX31865_PTD_WIRES2
    MAX31865_PTD_WIRES2CFG,
#else
    MAX31865_PTD_WIRESCFG,
#endif
#ifdef MAX31865_PTD_WIRES3
    MAX31865_PTD_WIRES3CFG,
#else
    MAX31865_PTD_WIRESCFG,
#endif
#ifdef MAX31865_PTD_WIRES4
    MAX31865_PTD_WIRES4CFG,
#else
    MAX31865_PTD_WIRESCFG,
#endif
#ifdef MAX31865_PTD_WIRES5
    MAX31865_PTD_WIRES5CFG,
#else
    MAX31865_PTD_WIRESCFG,
#endif
#ifdef MAX31865_PTD_WIRES6
    MAX31865_PTD_WIRES6CFG
#else
    MAX31865_PTD_WIRESCFG
#endif
  };
  float PtdLowRes [] = {
#ifdef MAX31865_PTD_LOW_RES1
    MAX31865_PTD_LOW_RES1,
#else
    MAX31865_PTD_LOW_RES,
#endif
#ifdef MAX31865_PTD_LOW_RES2
    MAX31865_PTD_LOW_RES2,
#else
    MAX31865_PTD_LOW_RES,
#endif
#ifdef MAX31865_PTD_LOW_RES3
    MAX31865_PTD_LOW_RES3,
#else
    MAX31865_PTD_LOW_RES,
#endif
#ifdef MAX31865_PTD_LOW_RES4
    MAX31865_PTD_LOW_RES4,
#else
    MAX31865_PTD_LOW_RES,
#endif
#ifdef MAX31865_PTD_LOW_RES5
    MAX31865_PTD_LOW_RES5,
#else
    MAX31865_PTD_LOW_RES,
#endif
#ifdef MAX31865_PTD_LOW_RES6
    MAX31865_PTD_LOW_RES6
#else
    MAX31865_PTD_LOW_RES
#endif
  };
  float PtdHighRes [] = {
#ifdef MAX31865_PTD_HIGH_RES1
    MAX31865_PTD_HIGH_RES1,
#else
    MAX31865_PTD_HIGH_RES,
#endif
#ifdef MAX31865_PTD_HIGH_RES2
    MAX31865_PTD_HIGH_RES2,
#else
    MAX31865_PTD_HIGH_RES,
#endif
#ifdef MAX31865_PTD_HIGH_RES3
    MAX31865_PTD_HIGH_RES3,
#else
    MAX31865_PTD_HIGH_RES,
#endif
#ifdef MAX31865_PTD_HIGH_RES4
    MAX31865_PTD_HIGH_RES4,
#else
    MAX31865_PTD_HIGH_RES,
#endif
#ifdef MAX31865_PTD_HIGH_RES5
    MAX31865_PTD_HIGH_RES5,
#else
    MAX31865_PTD_HIGH_RES,
#endif
#ifdef MAX31865_PTD_HIGH_RES6
    MAX31865_PTD_HIGH_RES6
#else
    MAX31865_PTD_HIGH_RES
#endif
  };

  max31865_init_status = 1;
  for (uint32_t i = 0; i < MAX_MAX31865S; i++) {
    if (PinUsed(GPIO_SSPI_MAX31865_CS1, i)) {
      max31865_pins_used |= 1 << i;  //set lowest bit
      max31865[i].setPins(
        Pin(GPIO_SSPI_MAX31865_CS1, i),
        Pin(GPIO_SSPI_MOSI),
        Pin(GPIO_SSPI_MISO),
        Pin(GPIO_SSPI_SCLK)
      );

      if (!max31865[i].begin(PtdWires[i])) {
        max31865_init_status = -1;
      }
    }
  }
}


/*
*   MAX31865_GetResult(void)
*   Acquires the raw data via SPI, checks for MAX31865 errors and fills result structure
*/
void MAX31865_GetResult(void) {
  float PtdBias [] = {
#ifdef MAX31865_PTD_BIAS1
    MAX31865_PTD_BIAS1,
#else
    MAX31865_PTD_BIAS,
#endif
#ifdef MAX31865_PTD_BIAS2
    MAX31865_PTD_BIAS2,
#else
    MAX31865_PTD_BIAS,
#endif
#ifdef MAX31865_PTD_BIAS3
    MAX31865_PTD_BIAS3,
#else
    MAX31865_PTD_BIAS,
#endif
#ifdef MAX31865_PTD_BIAS4
    MAX31865_PTD_BIAS4,
#else
    MAX31865_PTD_BIAS,
#endif
#ifdef MAX31865_PTD_BIAS5
    MAX31865_PTD_BIAS5,
#else
    MAX31865_PTD_BIAS,
#endif
#ifdef MAX31865_PTD_BIAS6
    MAX31865_PTD_BIAS6
#else
    MAX31865_PTD_BIAS
#endif
  };
  float PtdRes [] = {
#ifdef MAX31865_PTD_RES1
    MAX31865_PTD_RES1,
#else
    MAX31865_PTD_RES,
#endif
#ifdef MAX31865_PTD_RES2
    MAX31865_PTD_RES2,
#else
    MAX31865_PTD_RES,
#endif
#ifdef MAX31865_PTD_RES3
    MAX31865_PTD_RES3,
#else
    MAX31865_PTD_RES,
#endif
#ifdef MAX31865_PTD_RES4
    MAX31865_PTD_RES4,
#else
    MAX31865_PTD_RES,
#endif
#ifdef MAX31865_PTD_RES5
    MAX31865_PTD_RES5,
#else
    MAX31865_PTD_RES,
#endif
#ifdef MAX31865_PTD_RES6
    MAX31865_PTD_RES6
#else
    MAX31865_PTD_RES
#endif
  };
  for (uint32_t i = 0; i < MAX_MAX31865S; i++) {
    if (max31865_pins_used & (1 << i)) {
      uint16_t rtd;

      rtd = max31865[i].readRTD();
      MAX31865_Result[i].Rtd = rtd;
      MAX31865_Result[i].PtdResistance = max31865[i].rtd_to_resistance(rtd, RefRes[i]);
      MAX31865_Result[i].PtdTemp = ConvertTemp(max31865[i].rtd_to_temperature(rtd, PtdRes[i], RefRes[i]) + PtdBias[i]);
    }
  }
}

void MAX31865_Show(bool Json) {
  uint8_t report_once = 0;
  for (uint32_t i = 0; i < MAX_MAX31865S; i++) {
    if (max31865_pins_used & (1 << i)) {
      if (Json) {
        ResponseAppend_P(PSTR(",\"MAX31865%c%d\":{\"" D_JSON_TEMPERATURE "\":%*_f,\"" D_JSON_RESISTANCE "\":%*_f,\"" D_JSON_ERROR "\":%d}"), \
          IndexSeparator(), i,
          Settings.flag2.temperature_resolution, &MAX31865_Result[i].PtdTemp,
          Settings.flag2.temperature_resolution, &MAX31865_Result[i].PtdResistance,
          MAX31865_Result[i].ErrorCode);
        if ((0 == TasmotaGlobal.tele_period) && (!report_once)) {
#ifdef USE_DOMOTICZ
          DomoticzFloatSensor(DZ_TEMP, MAX31865_Result[i].PtdTemp);
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
          KnxSensor(KNX_TEMPERATURE, MAX31865_Result[i].PtdTemp);
#endif  // USE_KNX
          report_once++;
        }
#ifdef USE_WEBSERVER
      } else {
        char sensorname[33];
        sprintf(sensorname, "MAX31865%c%d", IndexSeparator(), i);
        WSContentSend_Temp(sensorname, MAX31865_Result[i].PtdTemp);
#endif  // USE_WEBSERVER
      }
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns47(uint8_t function)
{
  bool result = false;

  if (PinUsed(GPIO_SSPI_MAX31865_CS1, GPIO_ANY) && PinUsed(GPIO_SSPI_MISO) && PinUsed(GPIO_SSPI_MOSI) && PinUsed(GPIO_SSPI_SCLK)) {
    switch (function) {
      case FUNC_INIT:
        MAX31865_Init();
        break;

      case FUNC_EVERY_SECOND:
        MAX31865_GetResult();
        break;

      case FUNC_JSON_APPEND:
        MAX31865_Show(1);
        break;

#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        MAX31865_Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_MAX31865
