/*
  xsns_31_ccs811.ino - CCS811 gas and air quality sensor support for Tasmota

  Copyright (C) 2020  Gerhard Mutz and Theo Arends

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
#ifdef USE_CCS811
/*********************************************************************************************\
 * CCS811 - Gas (TVOC - Total Volatile Organic Compounds) and Air Quality (CO2)
 *
 * Source: Adafruit
 *
 * I2C Address: 0x5A assumes ADDR connected to Gnd, Wake also must be grounded
\*********************************************************************************************/

#define XSNS_31             31
#define XI2C_24             24  // See I2CDEVICES.md

#define EVERYNSECONDS 5

#include "Adafruit_CCS811.h"

Adafruit_CCS811 ccs;
uint8_t CCS811_ready = 0;
uint8_t CCS811_type = 0;;
uint16_t eCO2;
uint16_t TVOC;
uint8_t tcnt = 0;
uint8_t ecnt = 0;

/********************************************************************************************/

void CCS811Detect(void)
{
  if (I2cActive(CCS811_ADDRESS)) { return; }

  if (!ccs.begin(CCS811_ADDRESS)) {
    CCS811_type = 1;
    I2cSetActiveFound(CCS811_ADDRESS, "CCS811");
  }
}

void CCS811Update(void)  // Perform every n second
{
  tcnt++;
  if (tcnt >= EVERYNSECONDS) {
    tcnt = 0;
    CCS811_ready = 0;
    if (ccs.available()) {
      if (!ccs.readData()){
        TVOC = ccs.getTVOC();
        eCO2 = ccs.geteCO2();
        CCS811_ready = 1;
        if (global_update && (global_humidity > 0) && !isnan(global_temperature_celsius)) {
          ccs.setEnvironmentalData((uint8_t)global_humidity, global_temperature_celsius);
        }
        ecnt = 0;
      }
    } else {
      // failed, count up
      ecnt++;
      if (ecnt > 6) {
        // after 30 seconds, restart
        ccs.begin(CCS811_ADDRESS);
      }
    }
  }
}

const char HTTP_SNS_CCS811[] PROGMEM =
  "{s}CCS811 " D_ECO2 "{m}%d " D_UNIT_PARTS_PER_MILLION "{e}"                // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
  "{s}CCS811 " D_TVOC "{m}%d " D_UNIT_PARTS_PER_BILLION "{e}";

void CCS811Show(bool json)
{
  if (CCS811_ready) {
    if (json) {
      ResponseAppend_P(PSTR(",\"CCS811\":{\"" D_JSON_ECO2 "\":%d,\"" D_JSON_TVOC "\":%d}"), eCO2,TVOC);
#ifdef USE_DOMOTICZ
      if (0 == tele_period) DomoticzSensor(DZ_AIRQUALITY, eCO2);
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_CCS811, eCO2, TVOC);
#endif
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns31(uint8_t function)
{
  if (!I2cEnabled(XI2C_24)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    CCS811Detect();
  }
  else if (CCS811_type) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        CCS811Update();
        break;
      case FUNC_JSON_APPEND:
        CCS811Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        CCS811Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_CCS811
#endif  // USE_I2C
