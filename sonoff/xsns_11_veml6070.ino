/*
  xsns_11_veml6070.ino - VEML6070 ultra violet light sensor support for Sonoff-Tasmota

  Copyright (C) 2017  Theo Arends

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
#ifdef USE_VEML6070
/*********************************************************************************************\
 * VEML6070 - Ultra Violet Light Intensity
 *
 * I2C Address: 0x38 and 0x39
\*********************************************************************************************/

#define VEML6070_ADDR_H      0x39
#define VEML6070_ADDR_L      0x38

#define VEML6070_INTEGRATION_TIME 3  // 500msec integration time

uint8_t veml6070_address;
uint8_t veml6070_type = 0;

uint16_t Veml6070ReadUv()
{
  if (Wire.requestFrom(VEML6070_ADDR_H, 1) != 1) {
    return -1;
  }
  uint16_t uvi = Wire.read();
  uvi <<= 8;
  if (Wire.requestFrom(VEML6070_ADDR_L, 1) != 1) {
    return -1;
  }
  uvi |= Wire.read();

  return uvi;
}

/********************************************************************************************/

void Veml6070Detect()
{
  if (veml6070_type) {
    return;
  }

  uint8_t itime = VEML6070_INTEGRATION_TIME;

  veml6070_address = VEML6070_ADDR_L;
  Wire.beginTransmission(veml6070_address);
  Wire.write((itime << 2) | 0x02);
  uint8_t status = Wire.endTransmission();
  if (!status) {
    veml6070_type = 1;
    snprintf_P(log_data, sizeof(log_data), S_LOG_I2C_FOUND_AT, "VEML6070", veml6070_address);
    AddLog(LOG_LEVEL_DEBUG);
  }
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_ULTRAVIOLET[] PROGMEM =
  "%s{s}VEML6070 " D_UV_LEVEL "{m}%d{e}";  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

void Veml6070Show(boolean json)
{
  if (veml6070_type) {
    uint16_t uvlevel = Veml6070ReadUv();

    if (json) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"VEML6070\":{\"" D_UV_LEVEL "\":%d}"), mqtt_data, uvlevel);
#ifdef USE_DOMOTICZ
      DomoticzSensor(DZ_ILLUMINANCE, uvlevel);
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_ULTRAVIOLET, mqtt_data, uvlevel);
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_11

boolean Xsns11(byte function)
{
  boolean result = false;

  if (i2c_flg) {
    switch (function) {
//      case FUNC_XSNS_INIT:
//        break;
      case FUNC_XSNS_PREP:
        Veml6070Detect();
        break;
      case FUNC_XSNS_JSON_APPEND:
        Veml6070Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_XSNS_WEB:
        Veml6070Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_VEML6070
#endif  // USE_I2C

