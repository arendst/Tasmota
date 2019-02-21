/*
  xsns_21_sgp30.ino - SGP30 gas and air quality sensor support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends

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
#ifdef USE_SGP30
/*********************************************************************************************\
 * SGP30 - Gas (TVOC - Total Volatile Organic Compounds) and Air Quality (CO2)
 *
 * Source: Gerhard Mutz and Adafruit Industries
 *
 * I2C Address: 0x58
\*********************************************************************************************/

#define XSNS_21             21

#include "Adafruit_SGP30.h"
Adafruit_SGP30 sgp;

uint8_t sgp30_type = 0;
uint8_t sgp30_ready = 0;
uint8_t sgp30_counter = 0;

/********************************************************************************************/

void Sgp30Update(void)  // Perform every second to ensure proper operation of the baseline compensation algorithm
{
  sgp30_ready = 0;
  if (!sgp30_type) {
    if (sgp.begin()) {
      sgp30_type = 1;
//      snprintf_P(log_data, sizeof(log_data), PSTR("SGP: Serialnumber 0x%04X-0x%04X-0x%04X"), sgp.serialnumber[0], sgp.serialnumber[1], sgp.serialnumber[2]);
//      AddLog(LOG_LEVEL_DEBUG);
      snprintf_P(log_data, sizeof(log_data), S_LOG_I2C_FOUND_AT, "SGP30", 0x58);
      AddLog(LOG_LEVEL_DEBUG);
    }
  } else {
    if (!sgp.IAQmeasure()) return;  // Measurement failed
    sgp30_counter++;
    if (30 == sgp30_counter) {
      sgp30_counter = 0;

      uint16_t TVOC_base;
      uint16_t eCO2_base;

      if (!sgp.getIAQBaseline(&eCO2_base, &TVOC_base)) return;  // Failed to get baseline readings
//      snprintf_P(log_data, sizeof(log_data), PSTR("SGP: Baseline values eCO2 0x%04X, TVOC 0x%04X"), eCO2_base, TVOC_base);
//      AddLog(LOG_LEVEL_DEBUG);
    }
    sgp30_ready = 1;
  }
}

const char HTTP_SNS_SGP30[] PROGMEM = "%s"
  "{s}SGP30 " D_ECO2 "{m}%d " D_UNIT_PARTS_PER_MILLION "{e}"                // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
  "{s}SGP30 " D_TVOC "{m}%d " D_UNIT_PARTS_PER_BILLION "{e}";

void Sgp30Show(boolean json)
{
  if (sgp30_ready) {
    if (json) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"SGP30\":{\"" D_JSON_ECO2 "\":%d,\"" D_JSON_TVOC "\":%d}"), mqtt_data, sgp.eCO2, sgp.TVOC);
#ifdef USE_DOMOTICZ
      if (0 == tele_period) DomoticzSensor(DZ_AIRQUALITY, sgp.eCO2);
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_SGP30, mqtt_data, sgp.eCO2, sgp.TVOC);
#endif
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

boolean Xsns21(byte function)
{
  boolean result = false;

  if (i2c_flg) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        Sgp30Update();
        break;
      case FUNC_JSON_APPEND:
        Sgp30Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        Sgp30Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_SGP30
#endif  // USE_I2C