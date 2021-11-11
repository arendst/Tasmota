/*
  xsns_96_at42qt1070.ino - AT42QT1070 touch sensor support for Tasmota

  Copyright (C) 2021  Luc Boudreau

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
#ifdef USE_AT42QT1070

/*********************************************************************************************\
 * AT42QT1070 - Touch Controller
 *
 * Source:        Luc Boudreau
 *
 * I2C Address: 0x1B
\*********************************************************************************************/

#define XSNS_96             96
#define XI2C_66             66      // See I2CDEVICES.md

#define AT42QT1070_ADDR              0x1B
#define AT42QT1070_CHIPID            0x2E
#define AT42QT1070_REG_CHIPID        0x00
#define AT42QT1070_REG_KEYSTATUS     0x03
#define AT42QT1070_REG_RESET         0x39
#define AT42QT1070_REG_CALIBRATE     0x3801

const char* AT42QT1070_NAME PROGMEM = "AT42QT1070";
const char at42qt1070_json[] PROGMEM = "\"AT42QT1070\":{\"KEYS\":\"";

struct AT42QT1070 {
  // Bits 0 to 6 are buttons 0 to 6
  uint8_t buttonStatusCurrent;
  uint8_t buttonStatusPrevious;
};

struct AT42QT1070 *AT42QT1070data = nullptr;

/**
 * The various initialization steps for this sensor.
 */
void AT42QT1070Init(void)  {
  AT42QT1070data = (AT42QT1070*)calloc(1,sizeof(AT42QT1070));
  if (!AT42QT1070data) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("AT42QT1070: out of memory"));
    return;
  }
  // reset
  Wire.beginTransmission(AT42QT1070_ADDR);
  Wire.write(AT42QT1070_REG_RESET);
  Wire.endTransmission();
  delay(50);
  //calibrate
  AT42QT1070Calibrate();
}

void AT42QT1070Calibrate(void) {
  Wire.beginTransmission(AT42QT1070_ADDR);
  Wire.write(AT42QT1070_REG_CALIBRATE);
  Wire.endTransmission();
}

/**
 * Performs a read of all buttons
 */
void AT42QT1070Read(void) {
  AT42QT1070data->buttonStatusCurrent = I2cRead8(AT42QT1070_ADDR, AT42QT1070_REG_KEYSTATUS);
  for (uint8_t i = 0; i < 7; i++) {
    bool currentBit = 0 != (AT42QT1070data->buttonStatusCurrent & (1 << i));
    bool previousBit = 0 != (AT42QT1070data->buttonStatusPrevious & (1 << i));
    if (currentBit != previousBit) {
      Response_P(PSTR("{\"%s\":{\"Button%i\":%i}}"), AT42QT1070_NAME, i, currentBit);
      MqttPublishTeleSensor();
    }
  }
  AT42QT1070data->buttonStatusPrevious = AT42QT1070data->buttonStatusCurrent;
}

/**
 * Detects the sensor
 */
bool AT42QT1070Detect(void) {
  if (!I2cSetDevice(AT42QT1070_ADDR)) {
    return false;
  }
  uint8_t chipid = I2cRead8(AT42QT1070_ADDR, AT42QT1070_REG_CHIPID);
  //AddLog(LOG_LEVEL_DEBUG, PSTR("AT42QT1070Detect: device at address = 0x%02x; chip id 0x%02x"), AT42QT1070_ADDR, chipid);
  if (chipid = AT42QT1070_CHIPID) {
    AT42QT1070Init();
    I2cSetActiveFound(AT42QT1070_ADDR, AT42QT1070_NAME);
    return true;
  }
  return false;
}

/**
 * Shows the sensor in the ui
 */
void AT42QT1070Show(bool json) {
  if (json) {
    ResponseAppend_P(PSTR(",\"AT42QT1070\":{"));

    for (uint8_t i = 0; i < 7; i++) {
      bool currentBit = 0 != (AT42QT1070data->buttonStatusCurrent & (1 << i));
      if (i > 0) {
        ResponseAppend_P(PSTR(","));
      }
      ResponseAppend_P(PSTR("\"Button%i\":%i"), i, currentBit);
    }
    ResponseAppend_P(PSTR("}"));
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns96(uint8_t function)
{
  bool result = false;
  if (!I2cEnabled(XI2C_66)) {
    return result;
  }

  if (FUNC_INIT == function) {
    result = AT42QT1070Detect();
  }
  else {
    if (AT42QT1070data) {
      switch (function) {
        //case FUNC_EVERY_SECOND:
          //AddLog(LOG_LEVEL_DEBUG, PSTR("AT42QT1070 button status: %d"), AT42QT1070data->buttonStatusCurrent);
          //break;
        case FUNC_EVERY_100_MSECOND:
          // Update button status 10/s. Works nice as a baseline.
          AT42QT1070Read();
          result = true;
          break;
        case FUNC_JSON_APPEND:
          AT42QT1070Show(1);
          result = true;
          break;
        case FUNC_SAVE_AT_MIDNIGHT:
          // Recalibrate daily at midnight
          AT42QT1070Calibrate();
          break;
      }
    }
  }
  return result;
}

#endif  // USE_AT42QT1070
#endif  // USE_I2C
