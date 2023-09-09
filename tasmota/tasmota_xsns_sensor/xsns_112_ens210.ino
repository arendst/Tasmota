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
#define ENS210_MAX_COUNT		  2


#include "ScioSense_ENS210.h"

typedef struct ENS210DATA_s {
  ScioSense_ENS210     *ens210;

  float  temperature;
  float  humidity;

  uint8_t ready;
  uint8_t type;
  uint8_t tcnt;
  uint8_t ecnt;
} ENS210DATA_t;

ENS210DATA_t *ENS210data[ENS210_MAX_COUNT] = {nullptr, };
uint8_t ENS210Count = 0;

/********************************************************************************************/

void ens210Detect(void)
{
  ENS210Count = 0;
  ENS210DATA_t *pENS210;
  uint8_t i2caddr = ENS210_I2CADDR;
  for (uint8_t i = 0; i < ENS210_MAX_COUNT; i++, i2caddr++)
  {
    if (!I2cActive(i2caddr))
    {
      pENS210 = (ENS210DATA_t *)calloc(1, sizeof(ENS210DATA_t));
      if (!pENS210)
      {
        AddLog(LOG_LEVEL_ERROR, PSTR(ENS210_LOG ":@0x02%X Memory error!"), i2caddr);
        return;
      }
      pENS210->ens210 = new ScioSense_ENS210(i2caddr);
      if (!pENS210->ens210)
      {
        AddLog(LOG_LEVEL_ERROR, PSTR(ENS210_LOG ":@0x02%X Memory error 2!"), i2caddr);
        free(pENS210);
        return;
      }
      ENS210data[i] = pENS210;
      if (pENS210->ens210->begin())
      {
        if (pENS210->ens210->available())
        {
          pENS210->ens210->setSingleMode(false);
          pENS210->type = 1;
          I2cSetActiveFound(i2caddr, ENS210_DEVICE_NAME);
          ENS210Count++;
        }
      }
    }
  }
}

void ens210Update(void)  // Perform every n second
{
  for (uint8_t i = 0 ; i < ENS210Count; i++) {
    ENS210DATA_t *pENS210 = ENS210data[i];
    if (pENS210->type) {
      pENS210->tcnt++;
      if (pENS210->tcnt >= ENS210_EVERYNSECONDS) {
        pENS210->tcnt = 0;
        pENS210->ready = 0;
        if (pENS210->ens210->available()) {
          pENS210->ens210->measure();
              
          pENS210->temperature = pENS210->ens210->getTempCelsius();
          pENS210->humidity = pENS210->ens210->getHumidityPercent();
          pENS210->ready = 1;
          pENS210->ecnt = 0;
        } else {
          // failed, count up
          pENS210->ecnt++;
          if (pENS210->ecnt > 6) {
            // after 30 seconds, restart
            pENS210->ens210->begin();
            if (pENS210->ens210->available()) {
              pENS210->ens210->setSingleMode(false);
            }
          }
        }  
      }
    }
  }
}

void ens210Show(bool json)
{
  char name[20];
  for (uint8_t i = 0 ; i < ENS210Count; i++) {
    ENS210DATA_t *pENS210 = ENS210data[i];
    if (pENS210->type) {
      if (pENS210->ready) {
        snprintf_P(name, sizeof(name), (ENS210Count > 1) ? PSTR("%s%c%d") : PSTR("%s"), ENS210_DEVICE_NAME, IndexSeparator(), i +1);
        TempHumDewShow(json, (0 == TasmotaGlobal.tele_period), name, pENS210->temperature, pENS210->humidity);
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
