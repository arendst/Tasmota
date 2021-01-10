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
 * NOTE:  #define SEESAW_SOIL_PUBLISH enables immediate MQTT on soil moisture change
 *        otherwise the moisture value will only be emitted every TelePeriod
 *        #define SEESAW_SOIL_RAW enables displaying analog capacitance input in the
 *        web page for calibration purposes
\*********************************************************************************************/

#define XSNS_81              81
#define XI2C_56              56                 // See I2CDEVICES.md

#include "Adafruit_seesaw.h"

#define SEESAW_SOIL_MAX_SENSORS    4
#define SEESAW_SOIL_START_ADDRESS  0x36

const char SeeSoilName[]  = "SeeSoil";          // spaces not allowed for Homeassistant integration/mqtt topics
uint8_t    SeeSoilCount   = 0;                  // global sensor count

struct SEESAW_SOIL {
  Adafruit_seesaw *ss;                          // instance pointer
  uint16_t capacitance;
  float    temperature;
  uint8_t  address;
} SeeSoil[SEESAW_SOIL_MAX_SENSORS];

// Used to convert capacitance into a moisture.
// From observation, a free air reading is at 320
// Immersed in tap water, reading is 1014
// Appears to be a 10-bit device, readings close to 1020
// So let's make a scale that converts those (apparent) facts into a percentage
#define MAX_CAPACITANCE 1020.0f   // subject to calibration
#define MIN_CAPACITANCE 320       // subject to calibration
#define CAP_TO_MOIST(c) ((max((int)(c),MIN_CAPACITANCE)-MIN_CAPACITANCE)/(MAX_CAPACITANCE-MIN_CAPACITANCE))

/********************************************************************************************/

void SEESAW_SOILDetect(void) {
  Adafruit_seesaw *SSptr=0;

  for (uint32_t i = 0; i < SEESAW_SOIL_MAX_SENSORS; i++) {
    int addr = SEESAW_SOIL_START_ADDRESS + i;
    if (!I2cSetDevice(addr)) { continue; }

    if (!SSptr) {                               // don't have an object,
      SSptr = new Adafruit_seesaw();            // allocate one
    }
    if (SSptr->begin(addr)) {
      SeeSoil[SeeSoilCount].ss = SSptr;         // save copy of pointer
      SSptr = 0;                                // mark that we took it
      SeeSoil[SeeSoilCount].address = addr;
      SeeSoil[SeeSoilCount].temperature = NAN;
      SeeSoil[SeeSoilCount].capacitance = 0;
      I2cSetActiveFound(SeeSoil[SeeSoilCount].address, SeeSoilName);
      SeeSoilCount++;
    }
  }
  if (SSptr) {
    delete SSptr; // used object for detection, didn't find anything so we don't need this object
  }
}

void SEESAW_SOILEverySecond(void) {             // update sensor values and publish if changed
#ifdef SEESAW_SOIL_PUBLISH
  uint32_t old_moist;
#endif // SEESAW_SOIL_PUBLISH

  for (uint32_t i = 0; i < SeeSoilCount; i++) {
    SeeSoil[i].temperature = ConvertTemp(SeeSoil[i].ss->getTemp());
#ifdef SEESAW_SOIL_PUBLISH
    old_moist = uint32_t (CAP_TO_MOIST(SeeSoil[i].capacitance)*100);
#endif // SEESAW_SOIL_PUBLISH
    SeeSoil[i].capacitance = SeeSoil[i].ss->touchRead(0);
#ifdef SEESAW_SOIL_PUBLISH
    if (uint32_t (CAP_TO_MOIST(SeeSoil[i].capacitance)*100) != old_moist) {
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
        DomoticzTempHumPressureSensor(SeeSoil[i].temperature, CAP_TO_MOIST(SeeSoil[i].capacitance)*100, -42.0f);
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
        KnxSensor(KNX_TEMPERATURE, SeeSoil[i].temperature);
        KnxSensor(KNX_HUMIDITY, CAP_TO_MOIST(SeeSoil[i].capacitance) * 100);
#endif // USE_KNX
      }
#ifdef USE_WEBSERVER
    } else {
#ifdef SEESAW_SOIL_RAW
      WSContentSend_PD(HTTP_SNS_ANALOG, sensor_name, 0, SeeSoil[i].capacitance);  // dump raw value
#endif // SEESAW_SOIL_RAW
      WSContentSend_PD(HTTP_SNS_MOISTURE, sensor_name,
                       uint32_t (CAP_TO_MOIST(SeeSoil[i].capacitance)*100));      // web page formats as integer (%d) percent
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
                   sensor_name, SeeSoil[no].address, temperature, uint32_t (CAP_TO_MOIST(SeeSoil[no].capacitance)*100));
}

void SEESAW_SOILName(int no, char *name, int len)    // generates a sensor name
{
  if (SeeSoilCount > 1) {
    snprintf_P(name, len, PSTR("%s%c%u"), SeeSoilName, IndexSeparator(), no + 1);
  }
  else {
    strlcpy(name, SeeSoilName, len);
  }
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
