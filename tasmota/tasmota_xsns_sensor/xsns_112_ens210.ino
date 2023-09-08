/*
  xsns_99_ens210.ino - ENS210 gas and air quality sensor support for Tasmota

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
#ifdef USE_ENS210

/*********************************************************************************************\
 * ENS210 - Temperature & Humidity sensor
 *
 * Source: ScioSense
 *
 * I2C Address: 0x43 
\*********************************************************************************************/

#define XSNS_112             112
#define XI2C_85             85  // See I2CDEVICES.md

#define ENS210_I2CADDR_0      0x43
#define ENS210_I2CADDR_1      0x44
#define EVERYNSECONDS 5

#include "ScioSense_ENS210.h"

ScioSense_ENS210     ens210a(ENS210_I2CADDR_0);
struct  {
  float  temperature;
  float  humidity;

  uint8_t ready = 0;
  uint8_t type = 0;;
  uint8_t tcnt = 0;
  uint8_t ecnt = 0;
} ENS210dataA;

ScioSense_ENS210      ens210b(ENS210_I2CADDR_1);
struct  {
  float  temperature;
  float  humidity;

  uint8_t ready = 0;
  uint8_t type = 0;;
  uint8_t tcnt = 0;
  uint8_t ecnt = 0;
} ENS210dataB;

uint8_t ENS210Count = 0;

/********************************************************************************************/

void ens210Detect(void)
{
	ENS210Count = 0;
	if (!I2cActive(ENS210_I2CADDR_0)) {
		if (ens210a.begin()) {
			if (ens210a.available()) {
				ens210a.setSingleMode(false);
				ENS210dataA.type = 1;
				I2cSetActiveFound(ENS210_I2CADDR_0, "ENS210a");
				ENS210Count++;
			}
		}
	}
	
	if (!I2cActive(ENS210_I2CADDR_1)) {
		if (ens210b.begin()) {
			if (ens210b.available()) {
				ens210b.setSingleMode(false);
				ENS210dataB.type = 1;
				I2cSetActiveFound(ENS210_I2CADDR_1, "ENS210b");
				ENS210Count++;
			}
		}
	}
}

void ens210Update(void)  // Perform every n second
{
	if (ENS210dataA.type) {
		ENS210dataA.tcnt++;
		if (ENS210dataA.tcnt >= EVERYNSECONDS) {
			ENS210dataA.tcnt = 0;
			ENS210dataA.ready = 0;
			if (ens210a.available()) {
				ens210a.measure();
            
				ENS210dataA.temperature = ens210a.getTempCelsius();
				ENS210dataA.humidity = ens210a.getHumidityPercent();
				ENS210dataA.ready = 1;
				ENS210dataA.ecnt = 0;
			} else {
				// failed, count up
				ENS210dataA.ecnt++;
				if (ENS210dataA.ecnt > 6) {
					// after 30 seconds, restart
					ens210a.begin();
					if (ens210a.available()) {
						ens210a.setSingleMode(false);
					}
				}
			}  
		}
	}
	if (ENS210dataB.type) {
		ENS210dataB.tcnt++;
		if (ENS210dataB.tcnt >= EVERYNSECONDS) {
			ENS210dataB.tcnt = 0;
			ENS210dataB.ready = 0;
			if (ens210b.available()) {
				ens210b.measure();
            
				ENS210dataB.temperature = ens210b.getTempCelsius();
				ENS210dataB.humidity = ens210b.getHumidityPercent();
				ENS210dataB.ready = 1;
				ENS210dataB.ecnt = 0;
			} else {
				// failed, count up
				ENS210dataB.ecnt++;
				if (ENS210dataB.ecnt > 6) {
					// after 30 seconds, restart
					ens210b.begin();
					if (ens210b.available()) {
						ens210b.setSingleMode(false);
					}
				}
			}  
		}
	}
}

void ens210Show(bool json)
{
	if (ENS210dataA.type) {
		if (ENS210dataA.ready) {
			if (ENS210Count == 1) {
				TempHumDewShow(json, (0 == TasmotaGlobal.tele_period), "ENS210", ENS210dataA.temperature, ENS210dataA.humidity);
			} else {
				TempHumDewShow(json, (0 == TasmotaGlobal.tele_period), "ENS210a", ENS210dataA.temperature, ENS210dataA.humidity);
			}
		}
	}
	if (ENS210dataB.type) {
		if (ENS210dataB.ready) {
			if (ENS210Count == 1) {
				TempHumDewShow(json, (0 == TasmotaGlobal.tele_period), "ENS210", ENS210dataB.temperature, ENS210dataB.humidity);
			} else {
				TempHumDewShow(json, (0 == TasmotaGlobal.tele_period), "ENS210b", ENS210dataB.temperature, ENS210dataB.humidity);
			}
		}
	}
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns112(uint32_t function) 
{
  if (!I2cEnabled(XI2C_85)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    ens210Detect();
  }
  else {
    switch (function) {
      case FUNC_EVERY_SECOND:
        ens210Update();
        break;
      case FUNC_JSON_APPEND:
        ens210Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        ens210Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_ENS210
#endif  // USE_I2C
