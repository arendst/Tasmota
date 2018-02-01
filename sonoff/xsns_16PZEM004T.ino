/*
  xsns_16PZEM004T.ino - PZEM004T power monitor

  Copyright (C) 2018  Palich2000

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

#ifdef USE_SERIAL
#ifdef USE_PZEM_004T
/*********************************************************************************************\
 * Peacefair PZEM-004T Energy monitor
 * 
 *
 * https://github.com/olehs/PZEM004T
\*********************************************************************************************/

#include "PZEM004T.h"

//SHT3X sht30(SHT3X_SENSOR_ADDR);

bool PZEM004TInit() {
    
    return(serial_flg!=0);
}

boolean PZEM004TDetect()
{
    return(serial_flg!=0);
}


void PZEM004TShow(boolean json)
{
  if ((serial_flg!=0)) {
    if (json) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), JSON_SNS_TEMPHUM, mqtt_data, "PZEM004T", 0, 0);
    }
#ifdef USE_WEBSERVER
    else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_TEMP, mqtt_data, "", 0, TempUnit());
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_HUM, mqtt_data, "", 0);
    }
#endif  // USE_WEBSERVER
  }
}



/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_16

boolean Xsns16(byte function)
{
  boolean result = false;

  if (i2c_flg) {
    switch (function) {
      case FUNC_XSNS_INIT:
	result=PZEM004TInit();
        break;
      case FUNC_XSNS_PREP:
        result=PZEM004TDetect();
        break;
      case FUNC_XSNS_JSON_APPEND:
        PZEM004TShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_XSNS_WEB:
        PZEM004TShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_PZEM_004T
#endif  // USE_SERIAL
