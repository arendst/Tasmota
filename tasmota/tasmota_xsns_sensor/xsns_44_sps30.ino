/*
  xsns_44_sps30.ino - Sensirion SPS30 support for Tasmota

  Copyright (C) 2021  Gerhard Mutz and Theo Arends

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
#ifdef USE_SPS30
/*********************************************************************************************\
 * SPS30 - Particulate Matter (PM)
 *
 * I2C Address: 0x69
\*********************************************************************************************/

#define XSNS_44                    44
#define XI2C_30                    30  // See I2CDEVICES.md

//#define SPS30_ENABLE_SLEEP             // SPS30 v2.0: Adds support for sleep/wakeup to reduce power when not measuring (+0k2 code)
//#define SPS30_PARTS_PER_DECILITER      // Report NCPM as parts per deciliter instead of parts per cm3 (+0k1 code)

//#define SENSIRION_DEBUG                // Adds 1k2 to code size

#include <SensirionI2cSps30.h>

SensirionI2cSps30 sps30;

struct SPS30DATA_s {
  float PM1_0;
  float PM2_5;
  float PM4_0;
  float PM10;
  float NCPM0_5;
  float NCPM1_0;
  float NCPM2_5;
  float NCPM4_0;
  float NCPM10;
  float TYPSIZ;
  uint8_t running;
} *SPS30DATA = nullptr;

/********************************************************************************************/

bool Sps30Error(const char* func, int error) {
  bool result = (error != 0);
  if (result) {
#ifdef SENSIRION_DEBUG
    char error_msg[64];
    errorToString(error, error_msg, sizeof(error_msg));
    AddLog(LOG_LEVEL_DEBUG, PSTR("SPS: %s error %d %s"), func, error, error_msg);
#else
    AddLog(LOG_LEVEL_DEBUG, PSTR("SPS: %s error %d"), func, error);
#endif
  }
  return result;
}

#ifdef SPS30_ENABLE_SLEEP
int16_t Sps30Wakeup(void) {
  // SPS30 v2.0: Switch from sleep to idle mode. Performs delay(5) if no error
  // If the software implementation does not allow to send a I2C-Start-Condition followed by a Stop-Condition,
  // the Wake-up command can be sent twice in succession. In this case the first Wake-up command is ignored, but causes
  // the interface to be activated.
  int16_t localError = 0;
  sps30.wakeUp();
/*
  // Note: Lib v1.0.1 always returns 0 on call to wakeUp()
  if (Sps30Error("WakeUp", sps30.wakeUp())) {
    localError = 1;
  }
*/  
  sps30.wakeUp();
  return localError;
}
#endif  // SPS30_ENABLE_SLEEP

/********************************************************************************************/

void Sps30Init(void) {
  PowerOnDelay(60);  // Sensor startup time (Time after power-on until I2C communication can be started)
  for (uint32_t bus = 0; bus < MAX_I2C; bus++) {
    if (!I2cSetDevice(SPS30_I2C_ADDR_69, bus)) { 
//      Sps30Error("Scan", bus +1);
      continue;
    }
    sps30.begin(I2cGetWire(bus), SPS30_I2C_ADDR_69);

    if (sps30.deviceReset()) {                        // Switch to power reset state (idle mode). Performs delay(100) if no error
      if (Sps30Error("Reset", sps30.deviceReset())) { // See https://github.com/arendst/Tasmota/discussions/24452
        continue;
      }
    }

    uint8_t major;
    uint8_t minor;
//    if (Sps30Error("Version", sps30.readFirmwareVersion(major, minor))) {
    if (sps30.readFirmwareVersion(major, minor)) {
      continue;
    }

    int8_t serial_number[32] = { 0 };
//    if (Sps30Error("Serialnumber", sps30.readSerialNumber(serial_number, sizeof(serial_number)))) {
    if (sps30.readSerialNumber(serial_number, sizeof(serial_number))) {
      continue;
    }
/*
    // (always “00080000” without terminating null-character, recommended to use as product identifier)
    int8_t product_type[8] = { 0 };
    if (Sps30Error("Producttype", sps30.readProductType(product_type, sizeof(product_type)))) {
      continue;
    }
*/
    if (Sps30Error("StartMeasurement", sps30.startMeasurement(SPS30_OUTPUT_FORMAT_OUTPUT_FORMAT_FLOAT))) {
      continue;
    }

    SPS30DATA = (SPS30DATA_s *)calloc(1, sizeof(struct SPS30DATA_s));
    if (nullptr == SPS30DATA) { return; }
    SPS30DATA->running = 1;

    I2cSetActiveFound(SPS30_I2C_ADDR_69, "SPS30", bus);
    AddLog(LOG_LEVEL_DEBUG, PSTR("SPS: SPS30 serialnumber %s v%d.%d"), serial_number, major, minor);
    return;
  }
}

void Sps30EverySecond(void) {
  if (!SPS30DATA->running) { return; }

  if (TasmotaGlobal.uptime % 10 == 0) {
    float PM1_0;
    float PM2_5;
    float PM4_0;
    float PM10;
    float NCPM0_5;
    float NCPM1_0;
    float NCPM2_5;
    float NCPM4_0;
    float NCPM10;
    float TYPSIZ;
    if (!Sps30Error("Measurement", sps30.readMeasurementValuesFloat(
      PM1_0, PM2_5, PM4_0, PM10, NCPM0_5, NCPM1_0, NCPM2_5, NCPM4_0, NCPM10, TYPSIZ))) {
      SPS30DATA->PM1_0 = PM1_0;
      SPS30DATA->PM2_5 = PM2_5;
      SPS30DATA->PM4_0 = PM4_0;
      SPS30DATA->PM10 = PM10;
#ifdef SPS30_PARTS_PER_DECILITER
      SPS30DATA->NCPM0_5 = NCPM0_5 * 100;  // ppd = 100 cm3
      SPS30DATA->NCPM1_0 = NCPM1_0 * 100;
      SPS30DATA->NCPM2_5 = NCPM2_5 * 100;
      SPS30DATA->NCPM4_0 = NCPM4_0 * 100;
      SPS30DATA->NCPM10 = NCPM10 * 100;
#else   // Number per cm3
      SPS30DATA->NCPM0_5 = NCPM0_5;
      SPS30DATA->NCPM1_0 = NCPM1_0;
      SPS30DATA->NCPM2_5 = NCPM2_5;
      SPS30DATA->NCPM4_0 = NCPM4_0;
      SPS30DATA->NCPM10 = NCPM10;
#endif  // SPS30_PARTS_PER_DECILITER
      SPS30DATA->TYPSIZ = TYPSIZ;
    }
  }

  if (TasmotaGlobal.uptime % 3600 == 0 && TasmotaGlobal.uptime > 60) {
    // should auto clean once per week runtime
    // so count hours, should be in Settings
    Settings->sps30_inuse_hours++;
    if (Settings->sps30_inuse_hours > (7*24)) {
      Settings->sps30_inuse_hours = 0;
      sps30.startFanCleaning();
      ResponseTime_P(PSTR(",\"SPS30\":\"FanCleaning\"}"));
      MqttPublishTeleSensor();
    }
  }

}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

bool Sps30Command(void) {
  // Sensor44 0 - Stop measurment
  // Sensor44 1 - Start measurment
  // Sensor44 c - Clean
  bool serviced = true;
  if (XdrvMailbox.data_len > 0) {
    char *cp = XdrvMailbox.data;
    if (*cp == 'c') {
      // clean cmd
      sps30.startFanCleaning();
      Response_P(PSTR("{\"SPS30\":\"FanCleaning\"}"));
      return true;
    }
    else if (*cp == '0' || *cp == '1') {
      SPS30DATA->running = *cp &1;
      if (SPS30DATA->running) {
#ifdef SPS30_ENABLE_SLEEP
        Sps30Wakeup();
#endif  // SPS30_ENABLE_SLEEP
        sps30.startMeasurement(SPS30_OUTPUT_FORMAT_OUTPUT_FORMAT_FLOAT);
      } else {
        sps30.stopMeasurement();
#ifdef SPS30_ENABLE_SLEEP
        sps30.sleep();
#endif  // SPS30_ENABLE_SLEEP
      }
    } else {
      return false;;
    }
  }
  Response_P(PSTR("{\"SPS30\":\"%s\"}"), SPS30DATA->running ? "Running" : "Stopped");
  return serviced;
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

#define D_UNIT_NUMBER_PER_CUBIC_CM "#/cm³"
#define D_TYPICAL_PARTICLE_SIZE "Typical Size"
#define D_NUMBER_ENVIRONMENTAL_CONCENTRATION "NCPM"

#ifdef USE_WEBSERVER
#ifndef SPS30_PARTS_PER_DECILITER
const char HTTP_SNS_F_NUMBER_ENVIRONMENTAL_CONCENTRATION[] PROGMEM = "{s}%s " D_NUMBER_ENVIRONMENTAL_CONCENTRATION " %s " D_UNIT_MICROMETER "{m}%1_f " D_UNIT_NUMBER_PER_CUBIC_CM "{e}";
#endif  // No SPS30_PARTS_PER_DECILITER
const char HTTP_SNS_F_TYPICAL_PARTICLE_SIZE[] PROGMEM ="{s}%s " D_TYPICAL_PARTICLE_SIZE "{m}%2_f " D_UNIT_MICROMETER "{e}";
#endif  // USE_WEBSERVER

void Sps30Show(bool json) {
  if (!SPS30DATA->running) { return; }

  if (json) {
    ResponseAppend_P(PSTR(",\"SPS30\":{\"PM1_0\":%2_f,\"PM2_5\":%2_f,\"PM4_0\":%2_f,\"PM10\":%2_f"),
      &SPS30DATA->PM1_0, &SPS30DATA->PM2_5, &SPS30DATA->PM4_0, &SPS30DATA->PM10);
#ifdef SPS30_PARTS_PER_DECILITER
    ResponseAppend_P(PSTR(",\"NCPM0_5\":%d,\"NCPM1_0\":%d,\"NCPM2_5\":%d,\"NCPM4_0\":%d,\"NCPM10\":%d"),
      (uint32_t)SPS30DATA->NCPM0_5, (uint32_t)SPS30DATA->NCPM1_0, (uint32_t)SPS30DATA->NCPM2_5, (uint32_t)SPS30DATA->NCPM4_0, (uint32_t)SPS30DATA->NCPM10);
#else   // Number per cm3
    ResponseAppend_P(PSTR(",\"NCPM0_5\":%2_f,\"NCPM1_0\":%2_f,\"NCPM2_5\":%2_f,\"NCPM4_0\":%2_f,\"NCPM10\":%2_f"),
      &SPS30DATA->NCPM0_5, &SPS30DATA->NCPM1_0, &SPS30DATA->NCPM2_5, &SPS30DATA->NCPM4_0, &SPS30DATA->NCPM10);
#endif  // SPS30_PARTS_PER_DECILITER
    ResponseAppend_P(PSTR(",\"TYPSIZ\":%2_f}"), &SPS30DATA->TYPSIZ);
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_F_ENVIRONMENTAL_CONCENTRATION, "SPS30", "1", &SPS30DATA->PM1_0);
    WSContentSend_PD(HTTP_SNS_F_ENVIRONMENTAL_CONCENTRATION, "SPS30", "2.5", &SPS30DATA->PM2_5);
    WSContentSend_PD(HTTP_SNS_F_ENVIRONMENTAL_CONCENTRATION, "SPS30", "4", &SPS30DATA->PM4_0);
    WSContentSend_PD(HTTP_SNS_F_ENVIRONMENTAL_CONCENTRATION, "SPS30", "10", &SPS30DATA->PM10);
#ifdef SPS30_PARTS_PER_DECILITER
    WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, "SPS30", "0.5", (uint32_t)SPS30DATA->NCPM0_5);
    WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, "SPS30", "1", (uint32_t)SPS30DATA->NCPM1_0);
    WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, "SPS30", "2.5", (uint32_t)SPS30DATA->NCPM2_5);
    WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, "SPS30", "4", (uint32_t)SPS30DATA->NCPM4_0);
    WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, "SPS30", "10", (uint32_t)SPS30DATA->NCPM10);
#else   // Number per cm3
    WSContentSend_PD(HTTP_SNS_F_NUMBER_ENVIRONMENTAL_CONCENTRATION, "SPS30", "0.5", &SPS30DATA->NCPM0_5);
    WSContentSend_PD(HTTP_SNS_F_NUMBER_ENVIRONMENTAL_CONCENTRATION, "SPS30", "1", &SPS30DATA->NCPM1_0);
    WSContentSend_PD(HTTP_SNS_F_NUMBER_ENVIRONMENTAL_CONCENTRATION, "SPS30", "2.5", &SPS30DATA->NCPM2_5);
    WSContentSend_PD(HTTP_SNS_F_NUMBER_ENVIRONMENTAL_CONCENTRATION, "SPS30", "4", &SPS30DATA->NCPM4_0);
    WSContentSend_PD(HTTP_SNS_F_NUMBER_ENVIRONMENTAL_CONCENTRATION, "SPS30", "10", &SPS30DATA->NCPM10);
#endif  // SPS30_PARTS_PER_DECILITER
    WSContentSend_PD(HTTP_SNS_F_TYPICAL_PARTICLE_SIZE, "SPS30", &SPS30DATA->TYPSIZ);
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns44(uint32_t function) {
  if (!I2cEnabled(XI2C_30)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    Sps30Init();
  }
  else if (SPS30DATA) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        Sps30EverySecond();
        break;
      case FUNC_JSON_APPEND:
        Sps30Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Sps30Show(0);
        break;
#endif  // USE_WEBSERVER
#ifdef SPS30_ENABLE_SLEEP
      case FUNC_SAVE_BEFORE_RESTART:
        Sps30Wakeup();         // Need to wakeup before a restart as otherwise SPS30 is not recognised after restart
        break;
#endif  // SPS30_ENABLE_SLEEP
      case FUNC_COMMAND_SENSOR:
        if (XSNS_44 == XdrvMailbox.index) {
          result = Sps30Command();
        }
        break;
    }
  }
  return result;
}

#endif  // USE_SPS30
#endif  // USE_I2C
