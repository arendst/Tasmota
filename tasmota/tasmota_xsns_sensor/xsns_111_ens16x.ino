/*
  xsns_98_ENS16x.ino - ENS16x gas and air quality sensor support for Tasmota

  Copyright (C) 2021  Christoph Friese and Theo Arends

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
#ifdef USE_ENS16X

/*********************************************************************************************\
 * ENS16x - Gas (TVOC - Total Volatile Organic Compounds) and Air Quality (CO2)
 *
 * Source: ScioSense
 *
 * I2C Address: 0x52 
\*********************************************************************************************/

#define XSNS_111             111
#define XI2C_111             111  // See I2CDEVICES.md d

#define EVERYNSECONDS 5

#include "ScioSense_ENS16x.h"

ScioSense_ENS16x      ens16xa(ENS16x_I2CADDR_0);
struct  {
  uint16_t  TVOC;
  uint16_t  eCO2;
  uint16_t  AQIS;

  uint8_t ready = 0;
  uint8_t type = 0;;
  uint8_t tcnt = 0;
  uint8_t ecnt = 0;
} ENS16xdataA;

ScioSense_ENS16x      ens16xb(ENS16x_I2CADDR_1);
struct  {
  uint16_t  TVOC;
  uint16_t  eCO2;
  uint16_t  AQIS;

  uint8_t ready = 0;
  uint8_t type = 0;;
  uint8_t tcnt = 0;
  uint8_t ecnt = 0;
} ENS16xdataB;

uint8_t ENS16xCount = 0;

/********************************************************************************************/

void ens16xDetect(void)
{
	ENS16xCount = 0;
	if (!I2cActive(ENS16x_I2CADDR_0)) { 
		if (ens16xa.begin()) {
			if (ens16xa.available()) {
				if (ens16xa.setMode(ENS16x_OPMODE_STD) ) {
					ENS16xdataA.type = 1;
					I2cSetActiveFound(ENS16x_I2CADDR_0, "ENS16xa");
					ENS16xCount++;
				}
			}
		}
	}
	if (!I2cActive(ENS16x_I2CADDR_1)) { 
		if (ens16xb.begin()) {
			if (ens16xb.available()) {
				if (ens16xb.setMode(ENS16x_OPMODE_STD) ) {
					ENS16xdataB.type = 1;
					I2cSetActiveFound(ENS16x_I2CADDR_1, "ENS16xb");
					ENS16xCount++;
				}
			}
		}
	}
}

void ens16xUpdate(void)  // Perform every n second
{
//First sensor
	if (ENS16xdataA.type) {
		ENS16xdataA.tcnt++;
		if (ENS16xdataA.tcnt >= EVERYNSECONDS) {
			ENS16xdataA.tcnt = 0;
			ENS16xdataA.ready = 0;
			if (ens16xa.available()) {
				ens16xa.measure();
				ENS16xdataA.TVOC = ens16xa.getTVOC();
				ENS16xdataA.eCO2 = ens16xa.geteCO2();
				if (ens16xa.revENS16x() > 0) ENS16xdataA.AQIS = ens16xa.getAQIS();
				else ENS16xdataA.AQIS = ens16xa.getAQI();

				ENS16xdataA.ready = 1;
				ENS16xdataA.ecnt = 0;
			}
		} else {
			// failed, count up
			ENS16xdataA.ecnt++;
			if (ENS16xdataA.ecnt > 6) {
				// after 30 seconds, restart
				ens16xa.begin();
				if (ens16xa.available()) {
					ens16xa.setMode(ENS16x_OPMODE_STD);
				}
			}
		}
	}
//Second sensor
	if (ENS16xdataB.type) {
		ENS16xdataB.tcnt++;
		if (ENS16xdataB.tcnt >= EVERYNSECONDS) {
			ENS16xdataB.tcnt = 0;
			ENS16xdataB.ready = 0;
			if (ENS16xb.available()) {
				ens16xb.measure();
				ENS16xdataB.TVOC = ens16xb.getTVOC();
				ENS16xdataB.eCO2 = ens16xb.geteCO2();
				if (ens16xb.revENS16x() > 0) ENS16xdataB.AQIS = ens16xb.getAQIS();
				else ENS16xdataB.AQIS = ens16xb.getAQI();

				ENS16xdataB.ready = 1;
				ENS16xdataB.ecnt = 0;
			}
		} else {
			// failed, count up
			ENS16xdataB.ecnt++;
			if (ENS16xdataB.ecnt > 6) {
				// after 30 seconds, restart
				ens16xb.begin();
				if (ens16xb.available()) {
					ens16xb.setMode(ENS16x_OPMODE_STD);
				}
			}
		}
	}
}

const char HTTP_SNS_ENS16x[] PROGMEM =
  "{s}ENS16x " D_AQSI "{m}%d {e}"                // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
  "{s}ENS16x " D_ECO2 "{m}%d " D_UNIT_PARTS_PER_MILLION "{e}"
  "{s}ENS16x " D_TVOC "{m}%d " D_UNIT_PARTS_PER_BILLION "{e}";

void ens16xShow(bool json)
{
  if (ENS16xdataA.ready) {
    if (json) {
		if (ENS16xCount == 1)
			ResponseAppend_P(PSTR(",\"ENS16x\":{\"AQIS\":%d,\"" D_JSON_ECO2 "\":%d,\"" D_JSON_TVOC "\":%d}"), ENS16xdataA.AQIS, ENS16xdataA.eCO2, ENS16xdataA.TVOC);
		else {
			ResponseAppend_P(PSTR(",\"ENS16xa\":{\"AQIS\":%d,\"" D_JSON_ECO2 "\":%d,\"" D_JSON_TVOC "\":%d}"), ENS16xdataA.AQIS, ENS16xdataA.eCO2, ENS16xdataA.TVOC);
		}
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_ENS16x, ENS16xdataA.AQIS, ENS16xdataA.eCO2, ENS16xdataA.TVOC);
#endif
    }
  }
  if (ENS16xdataB.ready) {
    if (json) {
		if (ENS16xCount == 1)
			ResponseAppend_P(PSTR(",\"ENS16x\":{\"AQIS\":%d,\"" D_JSON_ECO2 "\":%d,\"" D_JSON_TVOC "\":%d}"), ENS16xdataB.AQIS, ENS16xdataB.eCO2, ENS16xdataB.TVOC);
		else {
			ResponseAppend_P(PSTR(",\"ENS16xb\":{\"AQIS\":%d,\"" D_JSON_ECO2 "\":%d,\"" D_JSON_TVOC "\":%d}"), ENS16xdataB.AQIS, ENS16xdataB.eCO2, ENS16xdataB.TVOC);
		}
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_ENS16x, ENS16xdataB.AQIS, ENS16xdataB.eCO2, ENS16xdataB.TVOC);
#endif
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns111(uint32_t function)
{
  if (!I2cEnabled(XI2C_111)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    ENS16xDetect();
  }
  else {
    switch (function) {
      case FUNC_EVERY_SECOND:
        ENS16xUpdate();
        break;
      case FUNC_JSON_APPEND:
        ENS16xShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        ENS16xShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_ENS16x
#endif  // USE_I2C
