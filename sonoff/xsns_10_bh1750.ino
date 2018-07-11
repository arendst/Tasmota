/*
  xsns_10_bh1750.ino - BH1750 ambient light sensor support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends

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
#ifdef USE_BH1750
/*********************************************************************************************\
 * BH1750 - Ambient Light Intensity
 *
 * I2C Address: 0x23 or 0x5C
\*********************************************************************************************/

#define BH1750_ADDR1         0x23
#define BH1750_ADDR2         0x5C

#define BH1750_CONTINUOUS_HIGH_RES_MODE 0x10 // Start measurement at 1lx resolution. Measurement time is approx 120ms.

uint8_t bh1750_address;
uint8_t bh1750_addresses[] = { BH1750_ADDR1, BH1750_ADDR2 };
uint8_t bh1750_type = 0;

uint16_t Bh1750ReadLux()
  if (2 != Wire.requestFrom(bh1750_address, (uint8_t)2)) { return false; }
  bh1750_illuminance = ((msb << 8) | lsb) / 1.2;
  return true;
      snprintf_P(log_data, sizeof(log_data), S_LOG_I2C_FOUND_AT, bh1750_types, bh1750_address);
  "%s{s}%s " D_ILLUMINANCE "{m}%d " D_UNIT_LUX "{e}";  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
  if (bh1750_valid) {
    uint16_t illuminance = Bh1750ReadLux();

      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"%s\":{\"" D_JSON_ILLUMINANCE "\":%d}"), mqtt_data, bh1750_types, bh1750_illuminance);
        DomoticzSensor(DZ_ILLUMINANCE, bh1750_illuminance);
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_ILLUMINANCE, mqtt_data, bh1750_types, bh1750_illuminance);
      case FUNC_INIT:
