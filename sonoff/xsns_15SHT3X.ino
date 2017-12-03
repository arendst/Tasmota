/*
  xsns_15SHT3X.ino - WeMos SHT30 Shield

  Copyright (C) 2017  Palich2000

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
#ifdef USE_SHT3X
/*********************************************************************************************\
 * WeMos SHT30 Shield
 * https://wiki.wemos.cc/products:d1_mini_shields:sht30_shield
 *
 * https://github.com/wemos/WEMOS_SHT3x_Arduino_Library
\*********************************************************************************************/

#include "WEMOS_SHT3X.h"

SHT3X sht30(SHT3X_SENSOR_ADDR);

boolean SHT3XDetect()
{
    if (sht30.get()==0) {
	snprintf_P(log_data, sizeof(log_data), S_LOG_I2C_FOUND_AT, "SHT3X", SHT3X_SENSOR_ADDR);
	AddLog(LOG_LEVEL_DEBUG);
	return true;
    }
    return false;
}


void SHT3XShow(boolean json)
{
  if (sht30.get()==0) {
    char temperature[10];
    char humidity[10];

    dtostrfd(sht30.cTemp, Settings.flag2.temperature_resolution, temperature);
    dtostrfd(sht30.humidity, Settings.flag2.humidity_resolution, humidity);

    if (json) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), JSON_SNS_TEMPHUM, mqtt_data, "SHT3X", temperature, humidity);
    }
#ifdef USE_WEBSERVER
    else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_TEMP, mqtt_data, "", temperature, TempUnit());
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_HUM, mqtt_data, "", humidity);
    }
#endif  // USE_WEBSERVER
  }
}



/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_15

boolean Xsns15(byte function)
{
  boolean result = false;

  if (i2c_flg) {
    switch (function) {
      case FUNC_XSNS_INIT:
	//SHT3XInit();
        break;
      case FUNC_XSNS_PREP:
        SHT3XDetect();
        break;
      case FUNC_XSNS_JSON_APPEND:
        SHT3XShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_XSNS_WEB:
        SHT3XShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_SHT3X
#endif  // USE_I2C
