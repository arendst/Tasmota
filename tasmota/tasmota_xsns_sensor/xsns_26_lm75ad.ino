/*
  xsns_26_lm75ad.ino - Support for I2C LM75AD Temperature Sensor

  Copyright (C) 2021  Andre Thomas and Theo Arends

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
#ifdef USE_LM75AD

/*********************************************************************************************\
 * LM75AD - Temperature
 *
 * Docs at https://www.nxp.com/docs/en/data-sheet/LM75A.pdf
 *
 * I2C Address: 0x48 - 0x4F
\*********************************************************************************************/

#define XSNS_26                 26
#define XI2C_20                 20    // See I2CDEVICES.md

#ifndef LM75AD_MAX_SENSORS
#define LM75AD_MAX_SENSORS      8
#endif

#define LM75AD_ADDRESS          0x48  // Start address
#define LM75AD_COUNT            8     // Number of sequential addresses

#define LM75_TEMP_REGISTER      0x00
#define LM75_CONF_REGISTER      0x01
#define LM75_THYST_REGISTER     0x02
#define LM75_TOS_REGISTER       0x03

struct {
  uint8_t address[LM75AD_MAX_SENSORS];
  uint8_t bus[LM75AD_MAX_SENSORS];
  uint8_t count;
} Lm75;

void LM75ADDetect(void) {
  if ((LM75AD_MAX_SENSORS < 1) || (LM75AD_MAX_SENSORS > 16)) { return; }  // Safeguard user changed LM75AD_MAX_SENSORS out of bounds
  for (uint32_t bus = 0; bus < 2; bus++) {
    for (uint32_t address = LM75AD_ADDRESS; address < LM75AD_ADDRESS + LM75AD_COUNT; address++) {
      if (!I2cSetDevice(address, bus)) { continue; } // Do not make the next step without a confirmed device on the bus
      uint16_t buffer;
      if (I2cValidRead16(&buffer, address, LM75_THYST_REGISTER, bus)) {
        if (buffer == 0x4B00) {
          I2cSetActiveFound(address, "LM75AD", bus);
          Lm75.address[Lm75.count] = address;
          Lm75.bus[Lm75.count] = bus;
          Lm75.count++;
          if (LM75AD_MAX_SENSORS == Lm75.count) { return; }
        }
      }
    }
  }
}

float LM75ADGetTemp(uint32_t sensor) {
  uint16_t t;
  if (I2cValidRead16(&t, Lm75.address[sensor], LM75_TEMP_REGISTER, Lm75.bus[sensor])) {
    int16_t sign = 1;
    if (t & 0x8000) { // We are getting a negative temperature value
      t = (~t) +0x20;
      sign = -1;
    }
    t = t >> 5; // Shift value into place (5 LSB not used)
    return ConvertTemp(sign * t * 0.125f);
  }
  return NAN;  // Will be changed to "null" by ext_vsprintf_P()
}

void LM75ADShow(bool json) {
  for (uint32_t sensor = 0; sensor < Lm75.count; sensor++) {
    float t = LM75ADGetTemp(sensor);
//    if (!isnan(t)) {
      char name[16];
      // LM75AD
      strlcpy(name, "LM75AD", sizeof(name));
      if (Lm75.count > 1) {
        // LM75AD-49
        snprintf_P(name, sizeof(name), PSTR("%s%c%02X"), name, IndexSeparator(), Lm75.address[sensor]);
#ifdef USE_I2C_BUS2
        if (TasmotaGlobal.i2c_enabled[1]) {  // Second bus enabled
          uint8_t bus = Lm75.bus[0];
          for (uint32_t i = 1; i < Lm75.count; i++) {
            if (bus != Lm75.bus[i]) {        // Different busses
              // LM75AD-49-1
              snprintf_P(name, sizeof(name), PSTR("%s%c%d"), name, IndexSeparator(), Lm75.bus[sensor] +1);
              break;
            }
          }
        }
#endif  // USE_I2C_BUS2
      }

      if (json) {
        ResponseAppend_P(JSON_SNS_F_TEMP, name, Settings->flag2.temperature_resolution, &t);
#ifdef USE_DOMOTICZ
        if ((0 == TasmotaGlobal.tele_period) && (0 == sensor)) {
          DomoticzFloatSensor(DZ_TEMP, t);
        }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
      } else {
        WSContentSend_Temp(name, t);
#endif  // USE_WEBSERVER
      }
//    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns26(uint32_t function)
{
  if (!I2cEnabled(XI2C_20)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    LM75ADDetect();
  }
  else if (Lm75.count) {
    switch (function) {
      case FUNC_JSON_APPEND:
        LM75ADShow(1);
        break;
  #ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        LM75ADShow(0);
        break;
  #endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_LM75AD
#endif  // USE_I2C
