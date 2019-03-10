/*
  xsns_16_tsl2561.ino - TSL2561 light sensor support for Sonoff-Tasmota

  Copyright (C) 2019  Theo Arends and Joachim Banzhaf

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
#ifdef USE_TSL2561
/*********************************************************************************************\
 * TSL2561 - Light Intensity
 *
 * Using library https://github.com/joba-1/Joba_Tsl2561
 *
 * I2C Addresses: 0x29 (low), 0x39 (float) or 0x49 (high)
\*********************************************************************************************/

#define XSNS_16             16

#include <Tsl2561Util.h>

Tsl2561 Tsl(Wire);

uint8_t tsl2561_type = 0;
uint8_t tsl2561_valid = 0;
uint32_t tsl2561_milliLux = 0;
char tsl2561_types[] = "TSL2561";

bool Tsl2561Read(void)
{
  if (tsl2561_valid) { tsl2561_valid--; }

  uint8_t id;
  bool gain;
  Tsl2561::exposure_t exposure;
  uint16_t scaledFull, scaledIr;
  uint32_t full, ir;

    if (Tsl.on()) {
      if (Tsl.id(id)
        && Tsl2561Util::autoGain(Tsl, gain, exposure, scaledFull, scaledIr)
        && Tsl2561Util::normalizedLuminosity(gain, exposure, full = scaledFull, ir = scaledIr)
        && Tsl2561Util::milliLux(full, ir, tsl2561_milliLux, Tsl2561::packageCS(id))) {
      } else{
        tsl2561_milliLux = 0;
      }
    }
  tsl2561_valid = SENSOR_MAX_MISS;
  return true;
}

void Tsl2561Detect(void)
{
  if (tsl2561_type) { return; }
  uint8_t id;

  if (I2cDevice(0x29) || I2cDevice(0x39) || I2cDevice(0x49)) {
    Tsl.begin();
    if (!Tsl.id(id)) return;
    if (Tsl.on()) {
      tsl2561_type = 1;
      AddLog_P2(LOG_LEVEL_DEBUG, S_LOG_I2C_FOUND_AT, tsl2561_types, Tsl.address(), id);
    }
  }
}

void Tsl2561EverySecond(void)
{
  if (90 == (uptime %100)) {
    // 1mS
    Tsl2561Detect();
  }
  else if (!(uptime %2)) {  // Update every 2 seconds
    // ?mS - 4Sec
    if (tsl2561_type) {
      if (!Tsl2561Read()) {
        AddLogMissed(tsl2561_types, tsl2561_valid);
               if (!tsl2561_valid) { tsl2561_type = 0; }
      }
    }
  }
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_TSL2561[] PROGMEM =
  "%s{s}TSL2561 " D_ILLUMINANCE "{m}%u.%03u " D_UNIT_LUX "{e}";  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

void Tsl2561Show(bool json)
{
  if (tsl2561_valid) {
    if (json) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"TSL2561\":{\"" D_JSON_ILLUMINANCE "\":%u.%03u}"),
        mqtt_data, tsl2561_milliLux / 1000, tsl2561_milliLux % 1000);
#ifdef USE_DOMOTICZ
      if (0 == tele_period) { DomoticzSensor(DZ_ILLUMINANCE, (tsl2561_milliLux + 500) / 1000); }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_TSL2561, mqtt_data, tsl2561_milliLux / 1000, tsl2561_milliLux % 1000);
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns16(uint8_t function)
{
  bool result = false;

  if (i2c_flg) {
    switch (function) {
      case FUNC_INIT:
        Tsl2561Detect();
        break;
      case FUNC_EVERY_SECOND:
        Tsl2561EverySecond();
        break;
      case FUNC_JSON_APPEND:
        Tsl2561Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        Tsl2561Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_TSL2561
#endif  // USE_I2C
