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

#define ENS210_EVERYNSECONDS 	5
#define ENS210_DEVICE_NAME		"EN210"
#define ENS210_LOG				    "E21"

#include "ScioSense_ENS210.h"

typedef struct ENS210DATA_s {
  ScioSense_ENS210     *ens210;

  float  temperature;
  float  humidity;

  uint8_t ready;
  uint8_t tcnt;
  uint8_t ecnt;
} ENS210DATA_t;

ENS210DATA_t *ENS210data = nullptr;

/********************************************************************************************/

void ens210Detect(void)
{
  uint8_t i2caddr = ENS210_I2CADDR;
  if (!I2cActive(i2caddr))
  {
    ScioSense_ENS210 *ens210 = new ScioSense_ENS210(i2caddr);
    if (!ens210) {
      AddLog(LOG_LEVEL_DEBUG, PSTR(ENS210_LOG ":@%02X create error!"), i2caddr);
      return;
    }
    if (!ens210->begin()) {
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(ENS210_LOG ":@%02X begin error!"), i2caddr);
      return;
    }
    if (!ens210->available()) {
      AddLog(LOG_LEVEL_ERROR, PSTR(ENS210_LOG ":@%02X available error!"), i2caddr);
      return;
    }
    ens210->setSingleMode(false);
    ENS210data = (ENS210DATA_t *)calloc(1, sizeof(ENS210DATA_t));
    if (!ENS210data) {
      AddLog(LOG_LEVEL_ERROR, PSTR(ENS210_LOG ":@%02X Memory error!"), i2caddr);
      return;
    }
    ENS210data->ens210 = ens210;
    I2cSetActiveFound(i2caddr, ENS210_DEVICE_NAME);
  }
}

void ens210Update(void)  // Perform every n second
{
  ENS210data->tcnt++;
  if (ENS210data->tcnt >= ENS210_EVERYNSECONDS) {
    ENS210data->tcnt = 0;
    ENS210data->ready = 0;
    if (ENS210data->ens210->available()) {
      ENS210data->ens210->measure();
          
      ENS210data->temperature = ENS210data->ens210->getTempCelsius();
      ENS210data->humidity = ENS210data->ens210->getHumidityPercent();
      ENS210data->ready = 1;
      ENS210data->ecnt = 0;
    } else {
      // failed, count up
      ENS210data->ecnt++;
      if (ENS210data->ecnt > 6) {
        // after 30 seconds, restart
        ENS210data->ens210->begin();
        if (ENS210data->ens210->available()) {
          ENS210data->ens210->setSingleMode(false);
        }
      }
    }  
  }
}

void ens210Show(bool json)
{
  if (ENS210data->ready) {
    TempHumDewShow(json, (0 == TasmotaGlobal.tele_period), ENS210_DEVICE_NAME, ENS210data->temperature, ENS210data->humidity);
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
  else if (ENS210data) {
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
