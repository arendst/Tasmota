/*
  xsns_118_ags02ma.ino - AGS02MA TVOC sensor support for Tasmota

  Copyright (C) 2025  Akshaylal S

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
#ifdef USE_AGS02MA
/*********************************************************************************************\
 * AGS02MA - TVOC (Total Volatile Organic Compounds) Sensor
 *
 * Source: RobTillaart/AGS02MA library
 * Adaption for TASMOTA: Akshaylal S
 *
 * I2C Address: 0x1A
\*********************************************************************************************/

#define XSNS_118            118
#define XI2C_95             95  // See I2CDEVICES.md

#define AGS02MA_ADDRESS     0x1A

#include "AGS02MA.h"

enum AGS02MA_State {
  STATE_AGS02MA_START,
  STATE_AGS02MA_INIT,
  STATE_AGS02MA_HEATING,
  STATE_AGS02MA_NORMAL,
  STATE_AGS02MA_FAIL,
};

AGS02MA *ags02ma = nullptr;
AGS02MA_State ags02ma_state = STATE_AGS02MA_START;

bool ags02ma_init = false;
bool ags02ma_read_pend = false;

uint32_t ags02ma_ppb_value = 0;
uint16_t heating_counter = 0;  // Counter for heating period (120 seconds / 24 checks = 5 seconds per check)

/********************************************************************************************/

void Ags02maInit(void)
{
  if (!I2cSetDevice(AGS02MA_ADDRESS)) { return; }

  ags02ma = new AGS02MA(AGS02MA_ADDRESS, &I2cGetWire());

  bool b = ags02ma->begin();
  if (!b) {
    AddLog(LOG_LEVEL_INFO, PSTR("AGS02MA: Sensor not found or initialization failed"));
    ags02ma_state = STATE_AGS02MA_FAIL;
    return;
  }

  uint8_t version = ags02ma->getSensorVersion();
  AddLog(LOG_LEVEL_INFO, PSTR("AGS02MA: Sensor version 0x%02X"), version);

  // Set PPB mode
  b = ags02ma->setPPBMode();
  if (!b) {
    AddLog(LOG_LEVEL_INFO, PSTR("AGS02MA: Failed to set PPB mode"));
    ags02ma_state = STATE_AGS02MA_FAIL;
    return;
  }

  uint8_t mode = ags02ma->getMode();
  AddLog(LOG_LEVEL_INFO, PSTR("AGS02MA: Mode set to %d"), mode);

  I2cSetActiveFound(AGS02MA_ADDRESS, "AGS02MA", XI2C_95);

  ags02ma_init = true;
  ags02ma_state = STATE_AGS02MA_HEATING;
  heating_counter = 0;

  AddLog(LOG_LEVEL_INFO, PSTR("AGS02MA: Starting warm-up period (120 seconds)"));
}

void Ags02maUpdate(void)
{
  if (ags02ma_state == STATE_AGS02MA_FAIL) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("AGS02MA: In FAIL state"));
    return;
  }

  // Handle heating period
  if (ags02ma_state == STATE_AGS02MA_HEATING) {
    // Check every 5 seconds (called from FUNC_EVERY_SECOND, so count to 5)
    if (heating_counter % 5 == 0) {
      if (ags02ma->isHeated()) {
        AddLog(LOG_LEVEL_INFO, PSTR("AGS02MA: Warm-up complete, sensor ready"));
        ags02ma_state = STATE_AGS02MA_NORMAL;
        heating_counter = 0;
        return;
      }
    }
    heating_counter++;

    // Log progress every 24 seconds (approximately)
    if (heating_counter % 24 == 0) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("AGS02MA: Warming up... %d seconds elapsed"), heating_counter);
    }
    return;
  }

  // Normal operation - read sensor value
  if (ags02ma_state == STATE_AGS02MA_NORMAL) {
    ags02ma_ppb_value = ags02ma->readPPB();
    
    uint8_t status = ags02ma->lastStatus();
    uint8_t error = ags02ma->lastError();
    
    if (error != 0) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("AGS02MA: Read error - Status: 0x%02X, Error: 0x%02X"), status, error);
    } else {
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("AGS02MA: PPB value: %d"), ags02ma_ppb_value);
    }
  }
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_AGS02MA[] PROGMEM =
  "{s}AGS02MA " D_TVOC "{m}%d " D_UNIT_PARTS_PER_BILLION "{e}";  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif

void Ags02maShow(bool json)
{
  if (ags02ma_state == STATE_AGS02MA_NORMAL) {
    if (json) {
      ResponseAppend_P(PSTR(",\"AGS02MA\":{\"" D_JSON_TVOC "\":%d}"), 
                       ags02ma_ppb_value);
#ifdef USE_DOMOTICZ
      if (0 == TasmotaGlobal.tele_period) {
        DomoticzSensor(DZ_AIRQUALITY, ags02ma_ppb_value);
      }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_AGS02MA, ags02ma_ppb_value);
#endif
    }
  } else if (ags02ma_state == STATE_AGS02MA_HEATING) {
    if (json) {
      ResponseAppend_P(PSTR(",\"AGS02MA\":{\"Status\":\"Heating\"}"));
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(PSTR("{s}AGS02MA Status{m}Warming up...{e}"));
#endif
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns118(uint32_t function)
{
  if (!I2cEnabled(XI2C_95)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    Ags02maInit();
  }
  else if (ags02ma_init) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        Ags02maUpdate();
        break;
      case FUNC_JSON_APPEND:
        Ags02maShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Ags02maShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_AGS02MA
#endif  // USE_I2C