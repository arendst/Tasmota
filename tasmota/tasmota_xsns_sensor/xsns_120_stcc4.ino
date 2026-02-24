/*
  xsns_120_stcc4.ino - Sensirion STCC4 CO₂ sensor support for Tasmota

  SPDX-FileCopyrightText: 2026 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_I2C
#ifdef USE_STCC4
/*********************************************************************************************\
 * Sensirion STCC4 CO₂ (and optional embedded SHT4x RH&T sensor)
 * 
 * Commands:
 * Stcc        - State information
 * StccPres    - Set the pressure compensation in hPa (default 1013)
 * StccPres <400..1100>
 * StccRHT     - Write temperature and humidity values (default 25°C and 50 %RH) when SHT4x is not present
 * StccRHT <temperature>,<humidity>
 * StccRHT <0..200>,<0..100>
 * StccCal     - Re-calibrate CO₂ sensor (FRC)
 * StccCal <351..32000>
 * StccReset 1 - Reset the Forced recalibration (FRC) and ASC algorithm history
 *
 * I2C Address: 0x64 or 0x65
\*********************************************************************************************/

#define XSNS_120                         120
#define XI2C_99                          99    // See I2CDEVICES.md

//#define SENSIRION_DEBUG                        // Adds 1k2 to code size

#include <SensirionI2cStcc4.h>

#define STCC4_STATE_READ_MEASUREMENT     0
#define STCC4_STATE_START_MEASUREMENT    2     // Wait at least 2 seconds as otherwise see sporadic error 527 - Not enough data received

SensirionI2cStcc4 stcc4;

struct StccData_s {
  int16_t co2;
  uint16_t humidity;
  uint16_t temperature;
  uint16_t sensor_status;
  uint8_t state;
} *StccData = nullptr;

/********************************************************************************************/

void StccStopStartMeasurement(void) {
  // Stop measurement and restart after 2 seconds
  stcc4.stopContinuousMeasurement();                  // Performs internal delay(1200)
  StccData->state = STCC4_STATE_START_MEASUREMENT;
}

bool StccError(const char* func, int error) {
  bool result = (error != 0);
  if (result) {
#ifdef SENSIRION_DEBUG
    char error_msg[64];
    errorToString(error, error_msg, sizeof(error_msg));
    AddLog(LOG_LEVEL_DEBUG, PSTR("STC: %s error %d %s"), func, error, error_msg);
#else
    AddLog(LOG_LEVEL_DEBUG, PSTR("STC: %s error %d"), func, error);
#endif
  }
  return result;
}

bool CmndStccError(int error) {
  bool result = (error != 0);
  if (result) {
    ResponseCmnd();
#ifdef SENSIRION_DEBUG
    char error_msg[64];
    errorToString(error, error_msg, sizeof(error_msg));
    ResponseAppend_P(PSTR("{\"Error\":\"%d %s\"}"), error, error_msg);
#else
    ResponseAppend_P(PSTR("{\"Error\":%d}"), error);
#endif
  }
  return result;
}

/********************************************************************************************/

void StccInit(void) {
  PowerOnDelay(20);  // Sensor startup time (Time after power-on until I2C communication can be started)
  for (uint32_t address = STCC4_I2C_ADDR_64; address < STCC4_I2C_ADDR_64 +2; address++) {
    for (uint32_t bus = 0; bus < 2; bus++) {
      if (!I2cSetDevice(address, bus)) { 
//        StccError("Scan", address + bus +1);
        continue;
      }
      stcc4.begin(I2cGetWire(bus), address);
/*
      if (StccError("ExitSleep", stcc4.exitSleepMode())) {  // Performs delay(5)
        continue;
      }
*/
      if (StccError("ForceStop", stcc4.stopContinuousMeasurement())) {  // Performs delay(1200) if no error
        continue;
      }
      uint32_t product_id;
      uint64_t serial_number;
      if (StccError("Serialnumber", stcc4.getProductId(product_id, serial_number))) {
        continue;
      }

      StccData = (StccData_s *)calloc(1, sizeof(struct StccData_s));
      StccData->state = STCC4_STATE_START_MEASUREMENT;
      I2cSetActiveFound(address, "STCC4", bus);
      AddLog(LOG_LEVEL_DEBUG, PSTR("STC: STCC4 serialnumber %_U"), &serial_number);
      return;
    }
  }
}

void StccUpdate(void) {
  switch (StccData->state) { 
    case STCC4_STATE_READ_MEASUREMENT:
      {
        // Use temp vars in case the read fails; Do not store invalid values.
        int16_t co2;
        uint16_t humidity;
        uint16_t temperature;
        uint16_t sensor_status;
        bool error = false;
        if (stcc4.readMeasurementRaw(co2, temperature, humidity, sensor_status)) {
          delay(150);  // A failed read can be caused by clock shifting. We advise to retry after a delay of 150ms.
          error = StccError("Measurement", stcc4.readMeasurementRaw(co2, temperature, humidity, sensor_status));
        }
        if (!error) {
          StccData->co2 = co2;
          StccData->temperature = temperature;
          StccData->humidity = humidity;
          StccData->sensor_status = sensor_status;
#ifdef USE_LIGHT
          LightSetSignal(CO2_LOW, CO2_HIGH, StccData->co2);  // SetOption18 - Pair light signal with CO₂ sensor
#endif  // USE_LIGHT
          StccData->state = STCC4_STATE_START_MEASUREMENT;   // Take at least 2 seconds between read as otherwise see sporadic error 527 - Not enough data received
        }
      }
      break;
    case STCC4_STATE_START_MEASUREMENT:
      if (StccError("StartContinuous", stcc4.startContinuousMeasurement())) {
        StccData->state = STCC4_STATE_START_MEASUREMENT +2;
      }
      break;
  }
  if (StccData->state) {
    StccData->state--;
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

const char kStccCommands[] PROGMEM = "Stcc|"  // Prefix
  "|Pres|RHT|Cal|Reset";

void (* const StccCommand[])(void) PROGMEM = {
  &CmndStccState, &CmndStccPressure, &CmndStccRHT, &CmndStccReset };

void CmndStccState(void) {
  StccStopStartMeasurement();
  uint32_t product_id;
  uint64_t serial_number;
  stcc4.getProductId(product_id, serial_number);
  Response_P(PSTR("{\"STCC4\":{\"Product\":\"%08X\",\"Serial\":%_U,\"Status\":\"%04X\"}"),
                   product_id, &serial_number, StccData->sensor_status);
}

void CmndStccPressure(void) {
  // StccPres  - Set the pressure compensation in hPa (default 1013)
  // StccPres <400..1100>
  if ((XdrvMailbox.payload >= 400) && (XdrvMailbox.payload <= 1100)) {
    if (!CmndStccError(stcc4.setPressureCompensationRaw(XdrvMailbox.payload * 50))) {
      ResponseCmndDone();
    }
  }
}

void CmndStccRHT(void) {
  // StccRHT   - Write temperature and humidity values (default 25°C and 50 %RH) when SHT4x is not present
  // StccRHT <temperature>,<humidity>
  // StccRHT <0..200>,<0..100>
  uint32_t value[2] = { 0 };
  if (2 == ParseParameters(2, value)) {
    float raw_temperature = (((float)value[0] + 45.0) * 65535.0) / 175.0;
    float raw_humidity = (((float)value[1] + 6.0) * 65535.0) / 125.0;
    if (!CmndStccError(stcc4.setRhtCompensation((uint16_t)raw_temperature, (uint16_t)raw_humidity))) {
      ResponseCmndDone();
    }
  }
}

void CmndStccCalibration(void) {
  // StccCal   - Re-calibrate CO₂ sensor with target_value
  // StccCal <351..32000>
  if (XdrvMailbox.payload > 350) {
    StccStopStartMeasurement();
    int16_t reference = XdrvMailbox.payload;
    int16_t correction;
    if (!CmndStccError(stcc4.performForcedRecalibration(reference, correction))) {
      ResponseCmndNumber(correction);  // FRC = return_value - 0x8000. If the recalibration has failed this returned value is 0xFFFF.
    }
  }
}

void CmndStccReset(void) {
  // StccReset 1 - Reset the Forced recalibration (FRC) and ASC algorithm history
  if (1 == XdrvMailbox.payload) {
    StccStopStartMeasurement();
    uint16_t result;
    if (!CmndStccError(stcc4.performFactoryReset(result))) {
      ResponseCmndNumber(result);  // If the factory reset has failed the returned value is not 0
    }
  }
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

#define STCC4_INT_INVALID 0x7FFF

float StccInt16(int16_t value) {
 return (value == STCC4_INT_INVALID) ? NAN : value;
}

void StccShow(bool json) {
  float co2 = StccInt16(StccData->co2);  // Change invalid to null
  float humidity = ConvertHumidity(stcc4.signalRelativeHumidity(StccData->humidity));
  float temperature = ConvertTemp(stcc4.signalTemperature(StccData->temperature));

  if (json) {
    ResponseAppend_P(PSTR(",\"STCC4\":{\"" D_JSON_CO2 "\":%0_f,"), &co2);
    ResponseAppendTHD(temperature, humidity);
    ResponseAppend_P(PSTR("}"));
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_F_CO2, "STCC4", &co2);
    WSContentSend_THD("STCC4", temperature, humidity);
#endif
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns120(uint32_t function) {
  if (!I2cEnabled(XI2C_99)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    StccInit();
  }
  else if (StccData) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        StccUpdate();
        break;
      case FUNC_JSON_APPEND:
        StccShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        StccShow(0);
        break;
#endif // USE_WEBSERVER
      case FUNC_COMMAND:
        result = DecodeCommand(kStccCommands, StccCommand);
        break;
    }
  }
  return result;
}

#endif  // USE_STCC4
#endif  // USE_I2C
