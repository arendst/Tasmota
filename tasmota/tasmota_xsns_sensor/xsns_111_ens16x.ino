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
#ifdef USE_ENS16x

/*********************************************************************************************\
 * ENS16x - Gas (TVOC - Total Volatile Organic Compounds) and Air Quality (CO2)
 *
 * Source: ScioSense
 *
 * I2C Address: 0x52 
\*********************************************************************************************/

#define XSNS_111             111
#define XI2C_84             84  // See I2CDEVICES.md

#define ENS16x_EVERYNSECONDS	5
#define ENS16x_DEVICE_NAME		"EN16X"
#define ENS16x_LOG						"E16"
#define ENS16x_MAX_COUNT			2

#include "ScioSense_ENS16x.h"

typedef struct ENS16xData_s {
  ScioSense_ENS16x      *ens16x;
  uint16_t  TVOC;
  uint16_t  eCO2;
  uint16_t  AQIS;

  uint8_t ready;
  uint8_t tcnt;
  uint8_t ecnt;
} ENS16xDATA_t;

ENS16xDATA_t *ENS16xData[ENS16x_MAX_COUNT] = {nullptr, };
uint8_t ENS16xCount = 0;

/********************************************************************************************/

void ens16xDetect(void)
{
  ENS16xCount = 0;
  ENS16xDATA_t *pENS16X;
  ScioSense_ENS16x *ens16x;
  uint8_t i2caddr = ENS16x_I2CADDR_0;
  for (uint8_t i = 0 ; i < ENS16x_MAX_COUNT; i++, i2caddr++) {
    if (!I2cActive(i2caddr)) {
      ens16x = new ScioSense_ENS16x(i2caddr);
      if (!ens16x) {
        AddLog(LOG_LEVEL_DEBUG, PSTR(ENS16x_LOG":@%02X create error!"), i2caddr);
        continue;
      }
      if (!(ens16x->begin())) {
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(ENS16x_LOG":@%02X begin error!"), i2caddr);
        continue;
      }
      if (!(ens16x->setMode(ENS16x_OPMODE_STD))) {
        AddLog(LOG_LEVEL_ERROR, PSTR(ENS16x_LOG":@%02X setmode error!"), i2caddr);
        continue;
      }
      pENS16X = (ENS16xDATA_t*)calloc(1, sizeof(ENS16xDATA_t));
      if (!pENS16X) {
        AddLog(LOG_LEVEL_ERROR, PSTR(ENS16x_LOG":@%02X Memory error!"), i2caddr);
        continue;
      }
      pENS16X->ens16x = ens16x;
      ENS16xData[ENS16xCount] = pENS16X;
      ENS16xCount++;
      I2cSetActiveFound(i2caddr, ENS16x_DEVICE_NAME);
    }
  }
}

void ens16xUpdate(void)  // Perform every n second
{
  for (uint8_t i = 0 ; i < ENS16xCount; i++) {
    ENS16xDATA_t *pENS16X = ENS16xData[i];
    pENS16X->tcnt++;
    if (pENS16X->tcnt >= ENS16x_EVERYNSECONDS) {
      pENS16X->tcnt = 0;
      pENS16X->ready = 0;
      if (pENS16X->ens16x->available()) {
        pENS16X->ens16x->measure();
        pENS16X->TVOC = pENS16X->ens16x->getTVOC();
        pENS16X->eCO2 = pENS16X->ens16x->geteCO2();
        if (pENS16X->ens16x->revENS16x() > 0) pENS16X->AQIS = pENS16X->ens16x->getAQIS();
        else pENS16X->AQIS = pENS16X->ens16x->getAQI();

        pENS16X->ready = 1;
        pENS16X->ecnt = 0;
      }
    } else {
      // failed, count up
      pENS16X->ecnt++;
      if (pENS16X->ecnt > 6) {
        // after 30 seconds, restart
        pENS16X->ens16x->begin();
        if (pENS16X->ens16x->available()) {
          pENS16X->ens16x->setMode(ENS16x_OPMODE_STD);
        }
      }
    }
  }
}

const char HTTP_SNS_ENS16x[] PROGMEM =
  "{s}%s AQI{m}%d {e}"                // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
  "{s}%s " D_ECO2 "{m}%d " D_UNIT_PARTS_PER_MILLION "{e}"
  "{s}%s " D_TVOC "{m}%d " D_UNIT_PARTS_PER_BILLION "{e}";

void ens16xShow(bool json)
{
  char name[20];
  for (uint8_t i = 0 ; i < ENS16xCount; i++) {
    ENS16xDATA_t *pENS16X = ENS16xData[i];
    if (pENS16X->ready) {
      snprintf_P(name, sizeof(name), (ENS16xCount > 1) ? PSTR("%s%c%d") : PSTR("%s"), ENS16x_DEVICE_NAME, IndexSeparator(), i +1);
      if (json) {
        ResponseAppend_P(PSTR(",\"%s\":{\"AQIS\":%d,\"" D_JSON_ECO2 "\":%d,\"" D_JSON_TVOC "\":%d}"), name, pENS16X->AQIS, pENS16X->eCO2, pENS16X->TVOC);
  #ifdef USE_WEBSERVER
      } else {
        WSContentSend_PD(HTTP_SNS_ENS16x, name, pENS16X->AQIS, name, pENS16X->eCO2, name, pENS16X->TVOC);
  #endif
      }
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns111(uint32_t function)
{
  if (!I2cEnabled(XI2C_84)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    ens16xDetect();
  }
  else if (ENS16xCount) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        ens16xUpdate();
        break;
      case FUNC_JSON_APPEND:
        ens16xShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        ens16xShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_ENS16x
#endif  // USE_I2C
