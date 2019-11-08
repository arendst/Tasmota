/*
  xsns_57_tsl2591.ino - TSL2591 light sensor support for Sonoff-Tasmota

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
#ifdef USE_TSL2591
/*********************************************************************************************\
 * TSL2591 - Light Intensity
 *
 * I2C Addresses: 0x29 (low), 0x39 (float) or 0x49 (high)
\*********************************************************************************************/

#define XSNS_57             57

#include <Adafruit_TSL2591.h>

Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);

uint8_t tsl2591_type = 0;
uint8_t tsl2591_valid = 0;
float tsl2591_lux = 0;

void Tsl2591Init(void)
{
  if (tsl2591_type) {
    return;
  }
  if (tsl.begin()) {
    tsl.setGain(TSL2591_GAIN_MED);
    tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
    tsl2591_type = 1;
  }
}

bool Tsl2591Read(void)
{
  uint32_t lum = tsl.getFullLuminosity();
  uint16_t ir, full;
  ir = lum >> 16;
  full = lum & 0xFFFF;
  tsl2591_lux = tsl.calculateLux(full, ir);
  tsl2591_valid = 1;
}

void Tsl2591EverySecond(void)
{
  Tsl2591Read();
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_TSL2591[] PROGMEM =
  "{s}TSL2591 " D_ILLUMINANCE "{m}%s " D_UNIT_LUX "{e}";  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

void Tsl2591Show(bool json)
{
  if (tsl2591_valid) {
    char lux_str[10];
    dtostrf(tsl2591_lux, sizeof(lux_str)-1, 3, lux_str);
    if (json) {
      ResponseAppend_P(PSTR(",\"TSL2591\":{\"" D_JSON_ILLUMINANCE "\":%s}"), lux_str);
#ifdef USE_DOMOTICZ
      if (0 == tele_period) { DomoticzSensor(DZ_ILLUMINANCE, tsl2591_lux); }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_TSL2591, lux_str);
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns57(uint8_t function)
{
  bool result = false;

  if (i2c_flg) {
    switch (function) {
      case FUNC_INIT:
        Tsl2591Init();
        break;
      case FUNC_EVERY_SECOND:
        Tsl2591EverySecond();
        break;
      case FUNC_JSON_APPEND:
        Tsl2591Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Tsl2591Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_TSL2591
#endif  // USE_I2C
