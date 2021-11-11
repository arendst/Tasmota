/*
  xsns_95_MAX11612.ino - MAX11612 ADC sensor support for Tasmota

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
#ifdef USE_MAX11612

/*********************************************************************************************\
 * MAX11612 - A/D converter
 *
 * Source:        Luc Boudreau
 * Other sources: Luis Teixeira from the HDC1080 code (GPL3+)
 *                sSense arduino library (Public Domain)
 *
 * I2C Address: 0x34
\*********************************************************************************************/

#define XSNS_95             95
#define XI2C_65             65      // See I2CDEVICES.md

#define MAX11612_ADDR        0x34

const char* MAX11612_NAME = "MAX11612";

struct MAX11612 {
  uint16_t ain0;
  uint16_t ain1;
  uint16_t ain2;
};

struct MAX11612 *MAX11612data = nullptr;

/**
 * The various initialization steps for this sensor.
 */
void MAX11612Init(void)  {
  MAX11612data = (MAX11612*)calloc(1,sizeof(MAX11612));
  if (!MAX11612data) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("MAX11612: out of memory"));
    return;
  }
}

/**
 * Performs a adc write read of all channels
 */
void MAX11612Read(void) {
  uint8_t byte[2];
	uint8_t setupRegister = 0x00;
  uint8_t configRegister = 0x00;

  // Setup byte bit
  setupRegister |= 0x80;
  // Use external reference
  setupRegister |= 0x20;
  // Use external clock
  setupRegister |= 0x08;
  // Don't reset
  setupRegister |= 0x02;

  // Start at channel 0
  // Enable scan
  // Single ended
  configRegister |= 0x21;
  // Channel 0
  MAX11612data->ain0 = MAX11612ReadValue(setupRegister, configRegister);

  // Channel 1
  configRegister |= 0x02;
  MAX11612data->ain1 = MAX11612ReadValue(setupRegister, configRegister);

  // Channel 2
  configRegister = 0x21;
  configRegister |= 0x02;
  MAX11612data->ain2 = MAX11612ReadValue(setupRegister, configRegister);
  
}

uint16_t MAX11612ReadValue(uint8_t setupRegister, uint8_t configRegister) {

  /*
  * This sensor works by writing the config registry and the setup registry,
  * followed by reading two bytes.
  */

  Wire.beginTransmission(MAX11612_ADDR);
  Wire.write(configRegister);
  Wire.write(setupRegister);
  if (Wire.endTransmission() != 0) { return 0; }
  
  delay(1);
  
  Wire.requestFrom(MAX11612_ADDR, 2);
  uint8_t time_out_count = 0;
  while (2 != Wire.available()) {
      time_out_count++;
      if (time_out_count > 10) {
          return 0;
      }
      delay(1);
  }

  uint8_t result[2] = {0,0};
  result[0] = Wire.read();
  result[1] = Wire.read();

  // first 4 bits of MSB are junk 1s
  uint16_t value = 0;
  value =  (((uint16_t)result[0] & 0x000f) << 8);
	value += ((uint16_t)result[1] & 0x00ff);
  
  return value;
}

/**
 * Detects the sensor
 */
bool MAX11612Detect(void) {
  if (!I2cSetDevice(MAX11612_ADDR)) {
    return false;
  }
  MAX11612Init();
  I2cSetActiveFound(MAX11612_ADDR, MAX11612_NAME);
  return true;
}

/**
 * Shows the sensor in the ui
 */
void MAX11612Show(bool json) {
  if (json) {
    ResponseAppend_P(PSTR(",\"%s\":{"), MAX11612_NAME);
    ResponseAppend_P(PSTR("%s\"A%d\":%d"), "", 0, MAX11612data->ain0);
    ResponseAppend_P(PSTR("%s\"A%d\":%d"), ",", 1, MAX11612data->ain1);
    ResponseAppend_P(PSTR("%s\"A%d\":%d"), ",", 2, MAX11612data->ain2);
    ResponseJsonEnd();
  }
#ifdef USE_WEBSERVER
  else {
    WSContentSend_PD(HTTP_SNS_ANALOG, MAX11612_NAME, 0, MAX11612data->ain0);
    WSContentSend_PD(HTTP_SNS_ANALOG, MAX11612_NAME, 1, MAX11612data->ain1);
    WSContentSend_PD(HTTP_SNS_ANALOG, MAX11612_NAME, 2, MAX11612data->ain2);
  }
#endif  // USE_WEBSERVER
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns95(uint8_t function)
{
  bool result = false;
  if (!I2cEnabled(XI2C_65)) {
    return result;
  }

  if (FUNC_INIT == function) {
    result = MAX11612Detect();
  }
  else {
    if (MAX11612data) {
      switch (function) {
        case FUNC_EVERY_SECOND:
          MAX11612Read();
          result = true;
          break;
        case FUNC_JSON_APPEND:
          MAX11612Show(1);
          result = true;
          break;
#ifdef USE_WEBSERVER
        case FUNC_WEB_SENSOR:
          MAX11612Show(0);
          result = true;
          break;
#endif  // USE_WEBSERVER
      }
    }
  }
  return result;
}

#endif  // USE_MAX11612
#endif  // USE_I2C
