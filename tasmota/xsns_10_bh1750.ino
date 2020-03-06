/*
  xsns_10_bh1750.ino - BH1750 ambient light sensor support for Tasmota

  Copyright (C) 2020  Theo Arends

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
#define XI2C_11              11  // See I2CDEVICES.md

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
  for (uint32_t i = 0; i < sizeof(bh1750_addresses); i++) {
    bh1750_address = bh1750_addresses[i];
    if (I2cActive(bh1750_address)) { continue; }
    Wire.beginTransmission(bh1750_address);
    Wire.write(BH1750_CONTINUOUS_HIGH_RES_MODE);
    if (!Wire.endTransmission()) {
      I2cSetActiveFound(bh1750_address, bh1750_types);
      bh1750_type = 1;
      break;
    }
  }
}

void Bh1750EverySecond(void)
{
  // 1mS
  if (!Bh1750Read()) {
    AddLogMissed(bh1750_types, bh1750_valid);
  }
}

void Bh1750Show(bool json)
{
  if (bh1750_valid) {
    if (json) {
      ResponseAppend_P(JSON_SNS_ILLUMINANCE, bh1750_types, bh1750_illuminance);
#ifdef USE_DOMOTICZ
      if (0 == tele_period) {
        DomoticzSensor(DZ_ILLUMINANCE, bh1750_illuminance);
      }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_ILLUMINANCE, bh1750_types, bh1750_illuminance);
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns10(uint8_t function)
{
  if (!I2cEnabled(XI2C_11)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    Bh1750Detect();
  }
  else if (bh1750_type) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        Bh1750EverySecond();
        break;
      case FUNC_JSON_APPEND:
        Bh1750Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Bh1750Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_BH1750
#endif  // USE_I2C
