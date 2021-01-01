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

#if MAX31865_PTD_WIRES == 4
  #define PTD_WIRES MAX31865_4WIRE
#elif MAX31865_PTD_WIRES == 3
  #define PTD_WIRES MAX31865_3WIRE
#else
  #define PTD_WIRES MAX31865_2WIRE
#endif

#include "Adafruit_MAX31865.h"

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

      if (!max31865[i].begin(PTD_WIRES)) {
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
  for (uint32_t i = 0; i < MAX_MAX31865S; i++) {
    if (max31865_pins_used & (1 << i)) {
      uint16_t rtd;

      rtd = max31865[i].readRTD();
      MAX31865_Result[i].Rtd = rtd;
      MAX31865_Result[i].PtdResistance = max31865[i].rtd_to_resistance(rtd, MAX31865_REF_RES);
      MAX31865_Result[i].PtdTemp = max31865[i].rtd_to_temperature(rtd, MAX31865_PTD_RES, MAX31865_REF_RES) + MAX31865_PTD_BIAS;
    }
  }
}

void MAX31865_Show(bool Json) {
  uint8_t report_once = 0;
  for (uint32_t i = 0; i < MAX_MAX31865S; i++) {
    if (max31865_pins_used & (1 << i)) {
      char temperature[33];
      char resistance[33];

      dtostrfd(MAX31865_Result[i].PtdResistance, Settings.flag2.temperature_resolution, resistance);
      dtostrfd(MAX31865_Result[i].PtdTemp, Settings.flag2.temperature_resolution, temperature);

      if (Json) {
        ResponseAppend_P(PSTR(",\"MAX31865%c%d\":{\"" D_JSON_TEMPERATURE "\":%s,\"" D_JSON_RESISTANCE "\":%s,\"" D_JSON_ERROR "\":%d}"), \
          IndexSeparator(), i, temperature, resistance, MAX31865_Result[i].ErrorCode);
        if ((0 == TasmotaGlobal.tele_period) && (!report_once)) {
#ifdef USE_DOMOTICZ
          DomoticzSensor(DZ_TEMP, temperature);
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
        WSContentSend_PD(HTTP_SNS_TEMP, sensorname, temperature, TempUnit());
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
