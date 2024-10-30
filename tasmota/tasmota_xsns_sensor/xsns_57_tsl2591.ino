/*
  xsns_57_tsl2591.ino - TSL2591 light sensor support for Tasmota

  Copyright (C) 2021  Markus Bösling and Theo Arends

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
 * (Used library supports address 0x29 only)
\*********************************************************************************************/

#define XSNS_57             57
#define XI2C_40             40    // See I2CDEVICES.md

#define TSL2591_ADDRESS     0x29  // Used library only supports this address only

#include <Adafruit_TSL2591.h>

Adafruit_TSL2591 tsl = Adafruit_TSL2591();

uint8_t tsl2591_type = 0;
uint8_t tsl2591_valid = 0;
float tsl2591_lux = 0;

tsl2591Gain_t gain_enum_array[4] = {TSL2591_GAIN_LOW,TSL2591_GAIN_MED,TSL2591_GAIN_HIGH,TSL2591_GAIN_MAX};
tsl2591IntegrationTime_t int_enum_array[6] = {TSL2591_INTEGRATIONTIME_100MS,TSL2591_INTEGRATIONTIME_200MS,TSL2591_INTEGRATIONTIME_300MS,TSL2591_INTEGRATIONTIME_400MS,TSL2591_INTEGRATIONTIME_500MS,TSL2591_INTEGRATIONTIME_600MS};

void Tsl2591Init(void)
{
//  if (I2cSetDevice(0x29) || I2cSetDevice(0x39) || I2cSetDevice(0x49)) {
  if (I2cSetDevice(0x29)) {
    if (tsl.begin()) {
      tsl.setGain(TSL2591_GAIN_MED);
      tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
      tsl2591_type = 1;
      I2cSetActiveFound(TSL2591_ADDRESS, "TSL2591");
    }
  }
}

void Tsl2591Read(void)
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
      if (0 == TasmotaGlobal.tele_period) { DomoticzSensor(DZ_ILLUMINANCE, tsl2591_lux); }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_TSL2591, lux_str);
#endif  // USE_WEBSERVER
    }
  }
}

bool tsl2591CommandSensor() {
  bool serviced = true;
  char argument[XdrvMailbox.data_len];
  long value = 0;

  for (uint32_t ca = 0; ca < XdrvMailbox.data_len; ca++) {
    if ((' ' == XdrvMailbox.data[ca]) || ('=' == XdrvMailbox.data[ca])) { XdrvMailbox.data[ca] = ','; }
  }

  bool any_value = (strchr(XdrvMailbox.data, ',') != nullptr);
  if (any_value) { value = strtol(ArgV(argument, 2), nullptr, 10); }

  tsl.setGain(gain_enum_array[XdrvMailbox.payload-1]);
  tsl.setTiming(int_enum_array[value-1]);

  Response_P(PSTR("{\"Gain input\":%d,\"Gain hex\":0x%x,"),XdrvMailbox.payload,tsl.getGain());
  ResponseAppend_P(PSTR("\"Timing input\":%d,\"Timing hex\":0x0%x}"),value,tsl.getTiming());
  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns57(uint32_t function)
{
  if (!I2cEnabled(XI2C_40)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    Tsl2591Init();
  }
  else if (tsl2591_type) {
    switch (function) {
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
      case FUNC_COMMAND_SENSOR:
        if (XSNS_57 == XdrvMailbox.index) {
          result = tsl2591CommandSensor();
        }
        break;
    }
  }
  return result;
}

#endif  // USE_TSL2591
#endif  // USE_I2C
