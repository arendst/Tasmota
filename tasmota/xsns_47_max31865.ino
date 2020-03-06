/*
  xsns_39_MAX31865.ino - MAX31865 thermocouple sensor support for Tasmota

  Copyright (C) 2020 Alberto Lopez Siemens

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

#ifndef USE_SPI
#error "MAX31865 requires USE_SPI enabled"
#endif

#include "Adafruit_MAX31865.h"

#define XSNS_47              47

#if MAX31865_PTD_WIRES == 4
  #define PTD_WIRES MAX31865_4WIRE
#elif MAX31865_PTD_WIRES == 3
  #define PTD_WIRES MAX31865_3WIRE
#else
  #define PTD_WIRES MAX31865_2WIRE
#endif

int8_t init_status = 0;

Adafruit_MAX31865 max31865;

struct MAX31865_Result_Struct {
    uint8_t   ErrorCode;
    uint16_t  Rtd;
    float     PtdResistance;
    float     PtdTemp;
} MAX31865_Result;

void MAX31865_Init(void){
    if(init_status)
        return;

  max31865.setPins(
    pin[GPIO_SSPI_CS],
    pin[GPIO_SSPI_MOSI],
    pin[GPIO_SSPI_MISO],
    pin[GPIO_SSPI_SCLK]
  );

  if(max31865.begin(PTD_WIRES))
    init_status = 1;
  else
    init_status = -1;
}

/*
*   MAX31865_GetResult(void)
*   Acquires the raw data via SPI, checks for MAX31865 errors and fills result structure
*/
void MAX31865_GetResult(void){
    uint16_t rtd;

    rtd = max31865.readRTD();
    MAX31865_Result.Rtd = rtd;
    MAX31865_Result.PtdResistance = max31865.rtd_to_resistance(rtd, MAX31865_REF_RES);
    MAX31865_Result.PtdTemp = max31865.rtd_to_temperature(rtd, MAX31865_PTD_RES, MAX31865_REF_RES) + MAX31865_PTD_BIAS;
}

void MAX31865_Show(bool Json){
    char temperature[33];
    char resistance[33];

    dtostrfd(MAX31865_Result.PtdResistance, Settings.flag2.temperature_resolution, resistance);
    dtostrfd(MAX31865_Result.PtdTemp, Settings.flag2.temperature_resolution, temperature);

    if(Json){
        ResponseAppend_P(PSTR(",\"MAX31865\":{\"" D_JSON_TEMPERATURE "\":%s,\"" D_JSON_RESISTANCE "\":%s,\"" D_JSON_ERROR "\":%d}"), \
          temperature, resistance, MAX31865_Result.ErrorCode);
#ifdef USE_DOMOTICZ
        if (0 == tele_period) {
          DomoticzSensor(DZ_TEMP, temperature);
        }
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
        if (0 == tele_period) {
          KnxSensor(KNX_TEMPERATURE, MAX31865_Result.PtdTemp);
        }
#endif  // USE_KNX
    } else {
#ifdef USE_WEBSERVER
        WSContentSend_PD(HTTP_SNS_TEMP, "MAX31865", temperature, TempUnit());
#endif  // USE_WEBSERVER
    }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns47(uint8_t function)
{
  bool result = false;
  if((pin[GPIO_SSPI_MISO] < 99) && (pin[GPIO_SSPI_MOSI] < 99) &&
     (pin[GPIO_SSPI_SCLK] < 99) && (pin[GPIO_SSPI_CS] < 99)) {

    switch (function) {
      case FUNC_INIT:
        MAX31865_Init();
        break;

      case FUNC_EVERY_SECOND:
        MAX31865_GetResult();
        break;

      case FUNC_JSON_APPEND:
        MAX31865_Show(true);
        break;

#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        MAX31865_Show(false);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_MAX31865
