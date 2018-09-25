/*
  xsns_16_tsl2561.ino - TSL2561 light sensor support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends and Joachim Banzhaf

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

#include <Tsl2561Util.h>

Tsl2561 Tsl(Wire);

void Tsl2561Detect()
{
  if (!Tsl.available()) {
    Tsl.begin();
    if (Tsl.available()) {
      snprintf_P(log_data, sizeof(log_data), S_LOG_I2C_FOUND_AT, "TSL2561", Tsl.address());
      AddLog(LOG_LEVEL_DEBUG);
    }
  }
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_TSL2561[] PROGMEM =
  "%s{s}TSL2561 " D_ILLUMINANCE "{m}%u.%03u " D_UNIT_LUX "{e}";  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

void Tsl2561Show(boolean json)
{
  uint8_t id;
  bool gain;
  Tsl2561::exposure_t exposure;
  uint16_t scaledFull, scaledIr;
  uint32_t full, ir;
  uint32_t milliLux;

  if (Tsl.available()) {
    if (Tsl.on()) {
      if( Tsl.id(id)
       && Tsl2561Util::autoGain(Tsl, gain, exposure, scaledFull, scaledIr)
       && Tsl2561Util::normalizedLuminosity(gain, exposure, full = scaledFull, ir = scaledIr)
       && Tsl2561Util::milliLux(full, ir, milliLux, Tsl2561::packageCS(id))) {

//        snprintf_P(log_data, sizeof(log_data), PSTR(D_ILLUMINANCE " g:%d, e:%d, f:%u, i:%u -> %u.%03u " D_UNIT_LUX),
//          gain, exposure, full, ir, milliLux/1000, milliLux%1000);
//        AddLog(LOG_LEVEL_DEBUG);

        if (json) {
          snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"TSL2561\":{\"" D_JSON_ILLUMINANCE "\":%u.%03u}"),
            mqtt_data, milliLux/1000, milliLux%1000);
#ifdef USE_DOMOTICZ
          if (0 == tele_period) DomoticzSensor(DZ_ILLUMINANCE, (milliLux+500)/1000);
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
        } else {
          snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_TSL2561, mqtt_data, milliLux/1000, milliLux%1000);
#endif  // USE_WEBSERVER
        }
      }
      Tsl.off();
    }
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
      case FUNC_PREP_BEFORE_TELEPERIOD:
        Tsl2561Detect();
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
