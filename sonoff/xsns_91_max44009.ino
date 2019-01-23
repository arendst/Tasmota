/*
  xsns_91_max44009.ino - MAX44009 ambient light sensor support for Sonoff-Tasmota

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
#ifdef USE_MAX44009
/*********************************************************************************************\
 * MAX44009 - Ambient Light Intensity
 *
 * I2C Address: 0x4a or 0x4b
\*********************************************************************************************/

#define XSNS_91			91

#define MAX44009_ADDR1         0x4A
#define MAX44009_ADDR2         0x4B
#define MAX44009_NO_REGISTERS  8
#define REG_CONFIG 0x02
#define REG_LUMINANCE 0x03
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
  /* Select luminance start register */
  Wire.beginTransmission(max44009_address);
  Wire.write(REG_LUMINANCE);
  Wire.endTransmission();

  if (2 != Wire.requestFrom(max44009_address, (uint8_t)2)) { 
    return false; 
  }
  byte msb = Wire.read();
  byte lsb = Wire.read();
  int exponent = (msb & 0xF0) >> 4;
  int mantissa = ((msb & 0x0F) << 4) | (lsb & 0x0F);
  max44009_illuminance = pow(2, exponent) * mantissa * 0.045;
  
  max44009_valid = 1;
  return true;
}

bool Max4409Read_register(uint8_t regno, byte *value) {
  Wire.beginTransmission(max44009_address);
  Wire.write(regno);
  if ((0 != Wire.endTransmission()) ||
      (1 != Wire.requestFrom(max44009_address, (uint8_t)1))) { 
    return false; 
  } 
  *value = (byte)Wire.read();
  return true;
}

/********************************************************************************************/

void Max4409Detect(void)
{
  byte reg[8];
  bool failed = false;

  if (max44009_found) {
    return;
  }

  for (byte i = 0; 0 != max44009_addresses[i]; i++) {
    max44009_address = max44009_addresses[i];

    // we need to read each register separately, as auto-increment is only 
    // supported for illiminance registers
    for (byte r = 0; r < MAX44009_NO_REGISTERS; r++) {
      if (false == Max4409Read_register(r, &reg[r])) {
        // snprintf_P(log_data, sizeof(log_data), "MAX44009 at %x: Failed to read register %d",
        //            max44009_address, (int)r);
        // AddLog(LOG_LEVEL_DEBUG_MORE);
        failed = true;
        break;
      }
    }
    if (failed) {
      continue;
    }
      
    // snprintf_P(log_data, sizeof(log_data), "MAX44009 at %x: Read %x %x %x %x %x %x %x %x",
    //           (int) max44009_address, (int)reg[0], (int)reg[1], (int)reg[2], (int)reg[3], 
    //         (int)reg[4], (int)reg[5], (int)reg[6], (int)reg[7]);
    // AddLog(LOG_LEVEL_DEBUG_MORE);

    if ( (0x00 == reg[0]) &&
         (0x00 == reg[1]) &&
         // reg[2] is written at configuration, so we cannot rely on its value after restart
         // reg[3] and reg[4] will always contain lux values
         // Datasheet says reg[5] is on power-up is  0xff, but we get 0xef?
         // This makes sense as 0xf for exponent means invalid, so we accept both
         ( (0xef == reg[5]) || (0xff == reg[5]) ) &&
         (0x00 == reg[6]) &&
         (0xff == reg[7])) {

      // looks like a MAX44009, try to initialize 

      Wire.beginTransmission(max44009_address);

      // select configuration register and set mode 
      Wire.write(REG_CONFIG);
      Wire.write(MAX44009_CONTINUOUS_AUTO_MODE);
      if (0 == Wire.endTransmission()) {
        max44009_found = 1;
        snprintf_P(log_data, sizeof(log_data), S_LOG_I2C_FOUND_AT, max44009_types, max44009_address);
        AddLog(LOG_LEVEL_DEBUG);
        break;
      }  else {
      //  snprintf_P(log_data, sizeof(log_data), "MAX44009 at %x: config failed!", max44009_address);
      //  AddLog(LOG_LEVEL_DEBUG_MORE);
      }
    } else {
      //snprintf_P(log_data, sizeof(log_data), "Reading initial data failed: No MAX44009 at %x", max44009_address);
      //AddLog(LOG_LEVEL_DEBUG_MORE);
    }
  }
}

void Max4409EverySecond(void)
{
  if (max44009_found) {
    Max4409Read_lum();
  }
}

void Max4409Show(boolean json)
{
  char illum_str[8];

  if (max44009_valid) {

    /* convert illuminance to string with suitable accuracy */
    if (max44009_illuminance < 10) {
      dtostrf(max44009_illuminance, sizeof(illum_str) -1, 3, illum_str);
    } else if (max44009_illuminance < 100) {
      dtostrf(max44009_illuminance, sizeof(illum_str) -1, 2, illum_str);
    } else if (max44009_illuminance < 1000) {
      dtostrf(max44009_illuminance, sizeof(illum_str) -1, 1, illum_str);
    } else {
      dtostrf(max44009_illuminance, sizeof(illum_str) -1, 0, illum_str);
    }

    if (json) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), 
                 PSTR("%s,\"%s\":{\"" D_JSON_ILLUMINANCE "\":%s}"), 
                 mqtt_data, max44009_types, illum_str);
#ifdef USE_DOMOTICZ
      if (0 == tele_period) {
        DomoticzSensor(DZ_ILLUMINANCE, illum_str);
      }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      // show integer value for lx on web-server
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_ILLUMINANCE, 
                 mqtt_data, max44009_types, (int)max44009_illuminance);
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

boolean Xsns91(byte function)
{
  boolean result = false;

  if (i2c_flg) {
    switch (function) {
      case FUNC_INIT:
        Max4409Detect();
        break;
      case FUNC_EVERY_SECOND:
        Max4409EverySecond();
        break;
      case FUNC_JSON_APPEND:
        Max4409Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        Max4409Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_MAX44009
#endif  // USE_I2C
