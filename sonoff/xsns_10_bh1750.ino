/*
  xsns_10_bh1750.ino - BH1750 ambient light sensor support for Sonoff-Tasmota

  Copyright (C) 2019  Theo Arends

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

#define XSNS_10              10

#define BH1750_ADDR1         0x23
#define BH1750_ADDR2         0x5C

#define BH1750_CONTINUOUS_HIGH_RES_MODE 0x10 // Start measurement at 1lx resolution. Measurement time is approx 120ms.

uint8_t bh1750_address;
uint8_t bh1750_addresses[] = { BH1750_ADDR1, BH1750_ADDR2 };
uint8_t bh1750_type = 0;
uint8_t bh1750_valid = 0;
uint16_t bh1750_illuminance = 0;
char bh1750_types[] = "BH1750";

bool Bh1750Read(void)
{
  if (bh1750_valid) { bh1750_valid--; }

  if (2 != Wire.requestFrom(bh1750_address, (uint8_t)2)) { return false; }
  uint8_t msb = Wire.read();
  uint8_t lsb = Wire.read();
  bh1750_illuminance = ((msb << 8) | lsb) / 1.2;
  bh1750_valid = SENSOR_MAX_MISS;
  return true;
}

/********************************************************************************************/

void Bh1750Detect(void)
{
  if (bh1750_type) {
    return;
  }

  for (uint8_t i = 0; i < sizeof(bh1750_addresses); i++) {
    bh1750_address = bh1750_addresses[i];
    Wire.beginTransmission(bh1750_address);
    Wire.write(BH1750_CONTINUOUS_HIGH_RES_MODE);
    if (!Wire.endTransmission()) {
      bh1750_type = 1;
      AddLog_P2(LOG_LEVEL_DEBUG, S_LOG_I2C_FOUND_AT, bh1750_types, bh1750_address);
      break;
    }
  }
}

void Bh1750EverySecond(void)
{
  if (90 == (uptime %100)) {
    // 1mS
    Bh1750Detect();
  }
  else {
    // 1mS
    if (bh1750_type) {
      if (!Bh1750Read()) {
        AddLogMissed(bh1750_types, bh1750_valid);
//        if (!bh1750_valid) { bh1750_type = 0; }
      }
    }
  }
}

void Bh1750Show(bool json)
{
  if (bh1750_valid) {
    if (json) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"%s\":{\"" D_JSON_ILLUMINANCE "\":%d}"), mqtt_data, bh1750_types, bh1750_illuminance);
#ifdef USE_DOMOTICZ
      if (0 == tele_period) {
        DomoticzSensor(DZ_ILLUMINANCE, bh1750_illuminance);
      }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_ILLUMINANCE, mqtt_data, bh1750_types, bh1750_illuminance);
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns10(uint8_t function)
{
  bool result = false;

  if (i2c_flg) {
    switch (function) {
      case FUNC_INIT:
        Bh1750Detect();
        break;
      case FUNC_EVERY_SECOND:
        Bh1750EverySecond();
        break;
      case FUNC_JSON_APPEND:
        Bh1750Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        Bh1750Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_BH1750
#endif  // USE_I2C
