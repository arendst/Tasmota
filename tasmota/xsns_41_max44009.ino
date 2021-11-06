/*
  xsns_41_max44009.ino - MAX44009 ambient light sensor support for Tasmota

  Copyright (C) 2021  Theo Arends

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
#ifdef USE_MAX44009
/*********************************************************************************************\
 * MAX44009 - Ambient Light Intensity
 *
 * I2C Address: 0x4a or 0x4b
\*********************************************************************************************/

#define XSNS_41			           41
#define XI2C_28                28  // See I2CDEVICES.md

#define MAX44009_ADDR1         0x4A
#define MAX44009_ADDR2         0x4B
#define MAX44009_NO_REGISTERS  8
#define REG_CONFIG             0x02
#define REG_LUMINANCE          0x03
#define REG_LOWER_THRESHOLD    0x06
#define REG_THRESHOLD_TIMER    0x07

#define MAX44009_CONTINUOUS_AUTO_MODE 0x80 // Start measurement in automatic, continous mode

uint8_t max44009_address;
uint8_t max44009_addresses[] = { MAX44009_ADDR1, MAX44009_ADDR2, 0 }; //0 terminated list
uint8_t max44009_found = 0;
uint8_t max44009_valid = 0;
float max44009_illuminance = 0;
char max44009_types[] = "MAX44009";

bool Max4409Read_lum(void)
{
  max44009_valid = 0;
  uint8_t regdata[2];

  /* Read 2 bytes luminance */
  if (I2cValidRead16((uint16_t *)&regdata, max44009_address, REG_LUMINANCE)) {
    int exponent = (regdata[0] & 0xF0) >> 4;
    int mantissa = ((regdata[0] & 0x0F) << 4) | (regdata[1] & 0x0F);
    max44009_illuminance = (float)(((0x00000001 << exponent) * (float)mantissa) * 0.045);
    max44009_valid = 1;
    return true;
  } else {
    return false;
  }
}

/********************************************************************************************/

void Max4409Detect(void)
{
  uint8_t buffer1;
  uint8_t buffer2;
  for (uint32_t i = 0; 0 != max44009_addresses[i]; i++) {

    max44009_address = max44009_addresses[i];
    if (!I2cSetDevice(max44009_address)) { continue; }

    if ((I2cValidRead8(&buffer1, max44009_address, REG_LOWER_THRESHOLD)) &&
        (I2cValidRead8(&buffer2, max44009_address, REG_THRESHOLD_TIMER))) {
      //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("MAX44009 %x: %x, %x"), max44009_address, (int)buffer1, (int)buffer2);
      if ((0x00 == buffer1) &&
          (0xFF == buffer2)) {

        // looks like a MAX44009, try to initialize

        Wire.beginTransmission(max44009_address);

        // select configuration register and set mode
        Wire.write(REG_CONFIG);
        Wire.write(MAX44009_CONTINUOUS_AUTO_MODE);
        if (0 == Wire.endTransmission()) {
          I2cSetActiveFound(max44009_address, max44009_types);
          max44009_found = 1;
          break;
        }
      }
    }
  }
}

void Max4409EverySecond(void)
{
  Max4409Read_lum();
}

void Max4409Show(bool json)
{
  char illum_str[8];

  if (max44009_valid) {

    /* convert illuminance to string with suitable accuracy */

    uint8_t prec = 0;
    if (10 > max44009_illuminance ) {
      prec = 3;
    } else if (100 > max44009_illuminance) {
      prec = 2;
    } else if (1000 > max44009_illuminance) {
      prec = 1;
    }
    dtostrf(max44009_illuminance, sizeof(illum_str) -1, prec, illum_str);

    if (json) {
      ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_ILLUMINANCE "\":%s}"), max44009_types, illum_str);
#ifdef USE_DOMOTICZ
      if (0 == TasmotaGlobal.tele_period) {
        DomoticzSensor(DZ_ILLUMINANCE, illum_str);
      }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      // show integer value for lx on web-server
      WSContentSend_PD(HTTP_SNS_ILLUMINANCE, max44009_types, (int)max44009_illuminance);
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns41(uint8_t function)
{
  if (!I2cEnabled(XI2C_28)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    Max4409Detect();
  }
  else if (max44009_found) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        Max4409EverySecond();
        break;
      case FUNC_JSON_APPEND:
        Max4409Show(1);
        break;
  #ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Max4409Show(0);
        break;
  #endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_MAX44009
#endif  // USE_I2C
