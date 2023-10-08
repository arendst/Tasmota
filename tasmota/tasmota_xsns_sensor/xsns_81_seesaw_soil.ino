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
 * SEESAW_SOIL - Capacitice Soil Moisture & Temperature Sensor
 *
 * I2C Address: 0x36, 0x37, 0x38, 0x39
 *
 * This version of the driver replaces all delay loops by a state machine. So the number
 * of instruction cycles consumed has been reduced dramatically. The sensors are reset,
 * detected, commanded and read all at once. So the reading times won't increase with the
 * number of sensors attached. The detection of sensors does not happen in FUNC_INIT any
 * more. All i2c handling happens in the 50ms state machine.
 * The memory footprint has suffered a little bit from this redesign, naturally.
 *
 * Memory footprint: 1444 bytes flash / 68 bytes RAM
 *
 * NOTE:  #define SEESAW_SOIL_PUBLISH enables immediate MQTT on soil moisture change
 *        otherwise the moisture value will only be emitted every TelePeriod
 *        #define SEESAW_SOIL_RAW enables displaying analog capacitance input in the
 *        web page for calibration purposes
 *        #define SEESAW_SOIL_PERSISTENT_NAMING to get sensor names indexed by i2c address
\*********************************************************************************************/

#define XSNS_81              81
#define XI2C_56              56                 // See I2CDEVICES.md

#include "Adafruit_seesaw.h"                    // we only use definitions, no code

#define SEESAW_SOIL_MAX_SENSORS     4
#define SEESAW_SOIL_START_ADDRESS   0x36
                                                // I2C state machine
#define STATE_IDLE                  0x00
#define STATE_RESET                 0x01
#define STATE_INIT                  0x02
#define STATE_DETECT                0x04
#define STATE_COMMAND_TEMP          0x08
#define STATE_READ_TEMP             0x10
#define STATE_COMMAND_MOIST         0x20
#define STATE_READ_MOIST            0x40
                                                // I2C commands
#define COMMAND_RESET               0x01
#define COMMAND_ID                  0x02
#define COMMAND_TEMP                0x04
#define COMMAND_MOIST               0x08
                                                // I2C delays
#define DELAY_DETECT                1           // ms delay before reading ID
#define DELAY_TEMP                  1           // ms delay between command and reading
#define DELAY_MOIST                 5           // ms delay between command and reading
#define DELAY_RESET                 500         // ms delay after slave reset

// Convert capacitance into a moisture.
// From observation, a free air reading is at 320, immersed in tap water, reading is 1014
// So let's make a scale that converts those (apparent) facts into a percentage
#define MAX_CAPACITANCE 1020.0f                 // subject to calibration
#define MIN_CAPACITANCE 320                     // subject to calibration
#define CAP_TO_MOIST(c) ((max((int)(c),MIN_CAPACITANCE)-MIN_CAPACITANCE)/(MAX_CAPACITANCE-MIN_CAPACITANCE)*100)

struct SEESAW_SOIL {
  const char  name[8]   = "SeeSoil";            // spaces not allowed for Homeassistant integration/mqtt topics
  uint8_t     count     = 0;                    // global sensor count (0xFF = not initialized)
  uint8_t     state     = STATE_IDLE;           // current state
  bool        present   = false;                // driver active
} SeeSoil;

struct SEESAW_SOIL_SNS {
  uint8_t     address;                          // i2c address
  float       moisture;
  float       temperature;
#ifdef SEESAW_SOIL_RAW
  uint16_t    capacitance;                      // raw analog reading
#endif // SEESAW_SOIL_RAW
} SeeSoilSNS[SEESAW_SOIL_MAX_SENSORS];

/*********************************************************************************************\
 * i2c routines
\*********************************************************************************************/

void seeSoilInit(void) {
  for (int i = 0; i < SEESAW_SOIL_MAX_SENSORS; i++) {
    int addr = SEESAW_SOIL_START_ADDRESS + i;
    if ( ! I2cSetDevice(addr) ) { continue; }
    seeSoilCommand(COMMAND_RESET);
  }
  SeeSoil.state = STATE_RESET;
  SeeSoil.present = true;
}

void seeSoilEvery50ms(void){                    // i2c state machine
  static uint32_t state_time;

  uint32_t time_diff = millis() - state_time;

  switch (SeeSoil.state) {
    case STATE_RESET:                           // reset was just issued
      SeeSoil.state = STATE_INIT;
      break;
    case STATE_INIT:                            // wait for sensors to settle
      if (time_diff < DELAY_RESET) { return; }
      seeSoilCommand(COMMAND_ID);               // send hardware id commands
      SeeSoil.state = STATE_DETECT;
      break;
    case STATE_DETECT:                          // detect sensors
      if (time_diff < DELAY_DETECT) { return; }
      seeSoilDetect();
      SeeSoil.state=STATE_COMMAND_TEMP;
      break;
    case STATE_COMMAND_TEMP:                    // send temperature commands
      seeSoilCommand(COMMAND_TEMP);
      SeeSoil.state = STATE_READ_TEMP;
      break;
    case STATE_READ_TEMP:
      if (time_diff < DELAY_TEMP) { return; }
      seeSoilRead(COMMAND_TEMP);                // read temperature values
      SeeSoil.state = STATE_COMMAND_MOIST;
      break;
    case STATE_COMMAND_MOIST:                   // send moisture commands
      seeSoilCommand(COMMAND_MOIST);
      SeeSoil.state = STATE_READ_MOIST;
      break;
    case STATE_READ_MOIST:
      if (time_diff < DELAY_MOIST) { return; }
      seeSoilRead(COMMAND_MOIST);               // read moisture values
      SeeSoil.state = STATE_COMMAND_TEMP;
      break;
  }
  state_time = millis();
}

void seeSoilDetect(void) {                      // detect sensors
  uint8_t buf;

  SeeSoil.count = 0;
  SeeSoil.present = false;
  for (int i = 0; i < SEESAW_SOIL_MAX_SENSORS; i++) {
    uint32_t addr = SEESAW_SOIL_START_ADDRESS + i;
    if ( ! I2cSetDevice(addr)) { continue; }
    if (1 != Wire.requestFrom((uint8_t) addr, (uint8_t) 1)) { continue; }
    buf = (uint8_t) Wire.read();
    if (buf != SEESAW_HW_ID_CODE) {             // check hardware id
#ifdef DEBUG_SEESAW_SOIL
      AddLog(LOG_LEVEL_DEBUG, PSTR("SEE: HWID mismatch ADDR=%X, ID=%X"), addr, buf);
#endif // DEBUG_SEESAW_SOIL
      continue;
    }
    SeeSoilSNS[SeeSoil.count].address = addr;
    SeeSoilSNS[SeeSoil.count].temperature = NAN;
    SeeSoilSNS[SeeSoil.count].moisture = NAN;
#ifdef SEESAW_SOIL_RAW
    SeeSoilSNS[SeeSoil.count].capacitance = 0;  // raw analog reading
#endif // SEESAW_SOIL_RAW
    I2cSetActiveFound(SeeSoilSNS[SeeSoil.count].address, SeeSoil.name);
    SeeSoil.count++;
    SeeSoil.present = true;
#ifdef DEBUG_SEESAW_SOIL
    AddLog(LOG_LEVEL_DEBUG, PSTR("SEE: FOUND sensor %u at %02X"), i, addr);
#endif // DEBUG_SEESAW_SOIL
  }
}

void seeSoilCommand(uint32_t command) {         // issue commands to sensors
  uint8_t  regLow;
  uint8_t  regHigh = SEESAW_STATUS_BASE;
  uint32_t count   = SeeSoil.count;

  switch (command) {
    case COMMAND_RESET:
      count   = SEESAW_SOIL_MAX_SENSORS;
      regLow  = SEESAW_STATUS_SWRST;
      break;
    case COMMAND_ID:
      count   = SEESAW_SOIL_MAX_SENSORS;
      regLow  = SEESAW_STATUS_HW_ID;
      break;
    case COMMAND_TEMP:
      regLow  = SEESAW_STATUS_TEMP;
      break;
    case COMMAND_MOIST:
      regHigh = SEESAW_TOUCH_BASE;
      regLow  = SEESAW_TOUCH_CHANNEL_OFFSET;
      break;
    default:
#ifdef DEBUG_SEESAW_SOIL
      AddLog(LOG_LEVEL_DEBUG, PSTR("SEE: ILL CMD:%02X"), command);
#endif // DEBUG_SEESAW_SOIL
      return;
  }
  for (int i = 0; i < count; i++) {
    uint32_t addr = (command & (COMMAND_RESET|COMMAND_ID)) ? SEESAW_SOIL_START_ADDRESS + i : SeeSoilSNS[i].address;
    Wire.beginTransmission((uint8_t) addr);
    Wire.write((uint8_t) regHigh);
    Wire.write((uint8_t) regLow);
    uint32_t err = Wire.endTransmission();
#ifdef DEBUG_SEESAW_SOIL
    AddLog(LOG_LEVEL_DEBUG, PSTR("SEE: SNS=%u ADDR=%02X CMD=%02X ERR=%u"), i, addr, command, err);
#endif // DEBUG_SEESAW_SOIL
    }
}

void seeSoilRead(uint32_t command) {            // read values from sensors
  uint8_t   buf[4];
  uint32_t  num;
  int32_t   ret;

  num = (command == COMMAND_TEMP) ? 4 : 2;      // response size in bytes

  for (int i = 0; i < SeeSoil.count; i++) {     // for all sensors
    if (num != Wire.requestFrom((uint8_t) SeeSoilSNS[i].address, (uint8_t) num)) { continue; }
    bzero(buf, sizeof(buf));
    for (int b = 0; b < num; b++) {
      buf[b] = (uint8_t) Wire.read();
    }
    if (command == COMMAND_TEMP) {
      ret = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) |
            ((uint32_t)buf[2] << 8) | (uint32_t)buf[3];
      SeeSoilSNS[i].temperature = ConvertTemp((1.0 / (1UL << 16)) * ret);
    } else {                                    // COMMAND_MOIST
      ret = (uint32_t)buf[0] << 8 | (uint32_t)buf[1];
      SeeSoilSNS[i].moisture = CAP_TO_MOIST(ret);
#ifdef SEESAW_SOIL_RAW
      SeeSoilSNS[i].capacitance = ret;
#endif // SEESAW_SOIL_RAW
    }
#ifdef DEBUG_SEESAW_SOIL
    AddLog(LOG_LEVEL_DEBUG, PSTR("SEE: READ #%u ADDR=%02X NUM=%u RET=%X"), i, SeeSoilSNS[i].address, num, ret);
#endif // DEBUG_SEESAW_SOIL
  }
}

/*********************************************************************************************\
 * JSON routines
\*********************************************************************************************/

#ifdef SEESAW_SOIL_PUBLISH
void seeSoilEverySecond(void) {                 // update sensor values and publish if changed
  static uint16_t old_moist[SEESAW_SOIL_MAX_SENSORS];
  static bool firstcall = true;

  for (int i = 0; i < SeeSoil.count; i++) {
    if (firstcall) { firstcall = false; }
    else {
      if ((uint32_t) SeeSoilSNS[i].moisture != old_moist[i]) {
        Response_P(PSTR("{"));                  // send values to MQTT & rules
        seeSoilJson(i);
        ResponseJsonEnd();
        MqttPublishTeleSensor();
      }
    }
    old_moist[i] = (uint32_t) SeeSoilSNS[i].moisture;
  }
}
#endif // SEESAW_SOIL_PUBLISH

void seeSoilShow(bool json) {
  char sensor_name[sizeof(SeeSoil.name) + 3];

  for (uint32_t i = 0; i < SeeSoil.count; i++) {
    seeSoilName(i, sensor_name, sizeof(sensor_name));
    if (json) {
      ResponseAppend_P(PSTR(","));              // compose tele json
      seeSoilJson(i);
      if (0 == TasmotaGlobal.tele_period) {
#ifdef USE_DOMOTICZ
        DomoticzTempHumPressureSensor(SeeSoilSNS[i].temperature, SeeSoilSNS[i].moisture, -42.0f);
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
        KnxSensor(KNX_TEMPERATURE, SeeSoilSNS[i].temperature);
        KnxSensor(KNX_HUMIDITY, SeeSoilSNS[i].moisture);
#endif // USE_KNX
      }
#ifdef USE_WEBSERVER
    } else {
#ifdef SEESAW_SOIL_RAW
      WSContentSend_PD(HTTP_SNS_ANALOG, sensor_name, 0, SeeSoilSNS[i].capacitance);
#endif // SEESAW_SOIL_RAW
      WSContentSend_PD(HTTP_SNS_MOISTURE, sensor_name, (uint32_t) SeeSoilSNS[i].moisture);
      WSContentSend_Temp(sensor_name, SeeSoilSNS[i].temperature);
#endif // USE_WEBSERVER
    }
  } // for each sensor connected
}

void seeSoilJson(int no) {                      // common json
  char sensor_name[sizeof(SeeSoil.name) + 3];
  seeSoilName(no, sensor_name, sizeof(sensor_name));

  ResponseAppend_P(PSTR ("\"%s\":{\"" D_JSON_ID "\":\"%02X\",\"" D_JSON_TEMPERATURE "\":%*_f,\"" D_JSON_MOISTURE "\":%u}"),
    sensor_name, SeeSoilSNS[no].address,
    Settings->flag2.temperature_resolution, &SeeSoilSNS[no].temperature,
    (uint32_t) SeeSoilSNS[no].moisture);
}

void seeSoilName(int no, char *name, int len)   // generates a sensor name
{
#ifdef SEESAW_SOIL_PERSISTENT_NAMING
  snprintf_P(name, len, PSTR("%s%c%02X"), SeeSoil.name, IndexSeparator(), SeeSoilSNS[no].address);
#else
  if (SeeSoil.count > 1) {
    snprintf_P(name, len, PSTR("%s%c%u"), SeeSoil.name, IndexSeparator(), no + 1);
  }
  else {
    strlcpy(name, SeeSoil.name, len);
  }
#endif // SEESAW_SOIL_PERSISTENT_NAMING
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns81(uint32_t function)
{
  if (!I2cEnabled(XI2C_56)) { return false; }
  bool result = false;

  if (FUNC_INIT == function) {
    seeSoilInit();
  }
  else if (SeeSoil.present){
    switch (function) {
      case FUNC_EVERY_50_MSECOND:
        seeSoilEvery50ms();
        break;
  #ifdef SEESAW_SOIL_PUBLISH
      case FUNC_EVERY_SECOND:
        seeSoilEverySecond();
        break;
  #endif   // SEESAW_SOIL_PUBLISH
      case FUNC_JSON_APPEND:
        seeSoilShow(1);
        break;
  #ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        seeSoilShow(0);
        break;
  #endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_SEESAW_SOIL
#endif  // USE_I2C
