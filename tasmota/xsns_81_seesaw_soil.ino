/*
  xsns_81_seesaw_soil - I2C Capacitance & temperature sensor support for Tasmota

  Copyright (C) 2021  Wayne Ross, Theo Arends, Peter Franck

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
#ifdef USE_SEESAW_SOIL

/*********************************************************************************************\
 * SEESAW_SOIL - Capacitance & Temperature Sensor
 *
 * I2C Address: 0x36, 0x37, 0x38, 0x39
 *
 * Memory footprint: 1296 bytes flash, 64 bytes RAM
 *
 * NOTE:  #define SEESAW_SOIL_PUBLISH enables immediate MQTT on soil moisture change
 *        otherwise the moisture value will only be emitted every TelePeriod
 *        #define SEESAW_SOIL_RAW enables displaying analog capacitance input in the
 *        web page for calibration purposes
\*********************************************************************************************/

#define XSNS_81              81
#define XI2C_56              56                 // See I2CDEVICES.md

#include "Adafruit_seesaw.h"                    // we only use definitions, no code

//#define SEESAW_SOIL_RAW                       // enable raw readings
//#define SEESAW_SOIL_PUBLISH                   // enable immediate publish
//#define SEESAW_SOIL_PERSISTENT_NAMING         // enable naming sensors by i2c address
//#define DEBUG_SEESAW_SOIL                     // enable debugging

#define SEESAW_SOIL_MAX_SENSORS     4
#define SEESAW_SOIL_START_ADDRESS   0x36

const char SeeSoilName[]  = "SeeSoil";          // spaces not allowed for Homeassistant integration/mqtt topics
uint8_t    SeeSoilCount   = 0;                  // global sensor count

struct SEESAW_SOIL {
  uint8_t   address;                            // i2c address
  float     moisture;
  float     temperature;
#ifdef SEESAW_SOIL_RAW
  uint16_t  capacitance;                        // raw analog reading
#endif // SEESAW_SOIL_RAW
} SeeSoil[SEESAW_SOIL_MAX_SENSORS];

// Used to convert capacitance into a moisture.
// From observation, a free air reading is at 320
// Immersed in tap water, reading is 1014
// Appears to be a 10-bit device, readings close to 1020
// So let's make a scale that converts those (apparent) facts into a percentage
#define MAX_CAPACITANCE 1020.0f   // subject to calibration
#define MIN_CAPACITANCE 320       // subject to calibration
#define CAP_TO_MOIST(c) ((max((int)(c),MIN_CAPACITANCE)-MIN_CAPACITANCE)/(MAX_CAPACITANCE-MIN_CAPACITANCE)*100)

/*********************************************************************************************\
 * i2c routines
\*********************************************************************************************/

void SEESAW_SOILDetect(void) {
  uint8_t buf;
  uint32_t i, addr;

  for (i = 0; i < SEESAW_SOIL_MAX_SENSORS; i++) {
    addr = SEESAW_SOIL_START_ADDRESS + i;
    if ( ! I2cSetDevice(addr)) { continue; }
    delay(1);
    SEESAW_Reset(addr);                         // reset all seesaw MCUs at once
  }
  delay(500);                                   // give MCUs time to boot
  for (i = 0; i < SEESAW_SOIL_MAX_SENSORS; i++) {
    addr = SEESAW_SOIL_START_ADDRESS + i;
    if ( ! I2cSetDevice(addr)) { continue; }
    if ( ! SEESAW_ValidRead(addr, SEESAW_STATUS_BASE, SEESAW_STATUS_HW_ID, &buf, 1, 0)) {
      continue;
    }
    if (buf != SEESAW_HW_ID_CODE) {
#ifdef DEBUG_SEESAW_SOIL
      AddLog_P(LOG_LEVEL_DEBUG, PSTR("SEE: HWID mismatch ADDR=%X, ID=%X"), addr, buf);
#endif // DEBUG_SEESAW_SOIL
      continue;
    }
    SeeSoil[SeeSoilCount].address = addr;
    SeeSoil[SeeSoilCount].temperature = NAN;
    SeeSoil[SeeSoilCount].moisture = NAN;
 #ifdef SEESAW_SOIL_RAW
    SeeSoil[SeeSoilCount].capacitance = 0;      // raw analog reading
#endif // SEESAW_SOIL_RAW
    I2cSetActiveFound(SeeSoil[SeeSoilCount].address, SeeSoilName);
    SeeSoilCount++;
  }
}

float SEESAW_Temp(uint8_t addr) {               // get temperature from seesaw at addr
  uint8_t buf[4];

  if (SEESAW_ValidRead(addr, SEESAW_STATUS_BASE, SEESAW_STATUS_TEMP, buf, 4, 1000)) {
    int32_t ret = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) |
                  ((uint32_t)buf[2] << 8) | (uint32_t)buf[3];
    return ConvertTemp((1.0 / (1UL << 16)) * ret);
  }
  return NAN;
}

float SEESAW_Moist(uint8_t addr) {              // get moisture from seesaw at addr
  uint8_t buf[2];
  uint16_t ret;
  int32_t tries = 2;

  while (tries--) {
    delay(1);
    if (SEESAW_ValidRead(addr, SEESAW_TOUCH_BASE, SEESAW_TOUCH_CHANNEL_OFFSET, buf, 2, 3000)) {
      ret = ((uint16_t)buf[0] << 8) | buf[1];
#ifdef SEESAW_SOIL_RAW
      for (int i=0; i < SeeSoilCount; i++) {
        if (SeeSoil[i].address == addr) {
          SeeSoil[i].capacitance = ret;
          break;
        }
      }
#endif // SEESAW_SOIL_RAW
      if (ret != 0xFFFF) { return (float) CAP_TO_MOIST(ret); }
    }
  }
  return NAN;
}

bool SEESAW_ValidRead(uint8_t addr, uint8_t regHigh, uint8_t regLow,        // read from seesaw sensor
                 uint8_t *buf, uint8_t num, uint16_t delay) {

  Wire.beginTransmission((uint8_t) addr);
  Wire.write((uint8_t) regHigh);
  Wire.write((uint8_t) regLow);
  int err = Wire.endTransmission();
  if (err) { return false; }
  delayMicroseconds(delay);
  if (num != Wire.requestFrom((uint8_t) addr, (uint8_t) num)) {
    return false;
  }
  for (int i = 0; i < num; i++) {
    buf[i] = (uint8_t) Wire.read();
  }
  return true;
}

bool SEESAW_Reset(uint8_t addr) {               // init sensor MCU
  Wire.beginTransmission((uint8_t) addr);
  Wire.write((uint8_t) SEESAW_STATUS_BASE);
  Wire.write((uint8_t) SEESAW_STATUS_SWRST);
  return (Wire.endTransmission() == 0);
}

/*********************************************************************************************\
 * JSON routines
\*********************************************************************************************/

void SEESAW_SOILEverySecond(void) {             // update sensor values and publish if changed
#ifdef SEESAW_SOIL_PUBLISH
  uint32_t old_moist;
#endif // SEESAW_SOIL_PUBLISH

  for (int i = 0; i < SeeSoilCount; i++) {
    SeeSoil[i].temperature = SEESAW_Temp(SeeSoil[i].address);
#ifdef SEESAW_SOIL_PUBLISH
    old_moist = (uint32_t) SeeSoil[i].moisture;
#endif // SEESAW_SOIL_PUBLISH
    SeeSoil[i].moisture = SEESAW_Moist(SeeSoil[i].address);
#ifdef SEESAW_SOIL_PUBLISH
    if ((uint32_t) SeeSoil[i].moisture != old_moist) {
      Response_P(PSTR("{"));                    // send values to MQTT & rules
      SEESAW_SOILJson(i);
      ResponseJsonEnd();
      MqttPublishTeleSensor();
    }
#endif // SEESAW_SOIL_PUBLISH
  }
}

void SEESAW_SOILShow(bool json) {
  char temperature[FLOATSZ];
  char sensor_name[sizeof(SeeSoilName) + 3];

  for (uint32_t i = 0; i < SeeSoilCount; i++) {
    dtostrfd(SeeSoil[i].temperature, Settings.flag2.temperature_resolution, temperature);
    SEESAW_SOILName(i, sensor_name, sizeof(sensor_name));
    if (json) {
      ResponseAppend_P(PSTR(","));                                          // compose tele json
      SEESAW_SOILJson(i);
      if (0 == TasmotaGlobal.tele_period) {
#ifdef USE_DOMOTICZ
        DomoticzTempHumPressureSensor(SeeSoil[i].temperature, SeeSoil[i].moisture, -42.0f);
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
        KnxSensor(KNX_TEMPERATURE, SeeSoil[i].temperature);
        KnxSensor(KNX_HUMIDITY, SeeSoil[i].moisture);
#endif // USE_KNX
      }
#ifdef USE_WEBSERVER
    } else {
#ifdef SEESAW_SOIL_RAW
      WSContentSend_PD(HTTP_SNS_ANALOG, sensor_name, 0, SeeSoil[i].capacitance);
#endif // SEESAW_SOIL_RAW
      WSContentSend_PD(HTTP_SNS_MOISTURE, sensor_name, (uint32_t) SeeSoil[i].moisture);
      WSContentSend_PD(HTTP_SNS_TEMP, sensor_name, temperature, TempUnit());
#endif // USE_WEBSERVER
    }
  } // for each sensor connected
}

void SEESAW_SOILJson(int no) {                        // common json
  char temperature[FLOATSZ];
  char sensor_name[sizeof(SeeSoilName) + 3];

  SEESAW_SOILName(no, sensor_name, sizeof(sensor_name));
  dtostrfd(SeeSoil[no].temperature, Settings.flag2.temperature_resolution, temperature);
  ResponseAppend_P(PSTR ("\"%s\":{\"" D_JSON_ID "\":\"%02X\",\"" D_JSON_TEMPERATURE "\":%s,\"" D_JSON_MOISTURE "\":%u}"),
                   sensor_name, SeeSoil[no].address, temperature, (uint32_t) SeeSoil[no].moisture);
}

void SEESAW_SOILName(int no, char *name, int len)    // generates a sensor name
{
#ifdef SEESAW_SOIL_PERSISTENT_NAMING
  snprintf_P(name, len, PSTR("%s%c%02X"), SeeSoilName, IndexSeparator(), SeeSoil[no].address);
#else
  if (SeeSoilCount > 1) {
    snprintf_P(name, len, PSTR("%s%c%u"), SeeSoilName, IndexSeparator(), no + 1);
  }
  else {
    strlcpy(name, SeeSoilName, len);
  }
#endif // SEESAW_SOIL_PERSISTENT_NAMING
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns81(uint8_t function)
{
  if (!I2cEnabled(XI2C_56)) { return false; }
  bool result = false;

  if (FUNC_INIT == function) {
    SEESAW_SOILDetect();
  }
  else if (SeeSoilCount){
    switch (function) {
      case FUNC_EVERY_SECOND:
        SEESAW_SOILEverySecond();
        break;
      case FUNC_JSON_APPEND:
        SEESAW_SOILShow(1);
        break;
  #ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        SEESAW_SOILShow(0);
        break;
  #endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_SEESAW_SOIL
#endif  // USE_I2C
