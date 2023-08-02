/*
  xsns_42_scd30.ino - SCD30 CO2 sensor support for Tasmota

  Copyright (C) 2021  Frogmore42

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
#ifdef USE_SCD30
/*********************************************************************************************\
 * SCD30 NDIR CO2 Temperature and Humidity sensor
\*********************************************************************************************/

#define XSNS_42                       42
#define XI2C_29                       29  // See I2CDEVICES.md

//#define SCD30_DEBUG

#define SCD30_ADDRESS                 0x61

#define SCD30_MAX_MISSED_READS        3
#define SCD30_STATE_NO_ERROR          0
#define SCD30_STATE_ERROR_DATA_CRC    1
#define SCD30_STATE_ERROR_READ_MEAS   2
#define SCD30_STATE_ERROR_SOFT_RESET  3
#define SCD30_STATE_ERROR_I2C_RESET   4
#define SCD30_STATE_ERROR_UNKNOWN     5

const char kScd30Commands[] PROGMEM = "Scd30|"  // Prefix
  "Alt|Auto|Cal|FW|Int|Pres|TOff";
void (*const kScd30Command[])(void) PROGMEM = {
  &CmndScd30Altitude, &CmndScd30AutoMode, &CmndScd30Calibrate, &CmndScd30Firmware, &CmndScd30Interval, &CmndScd30Pressure, &CmndScd30TempOffset };

/********************************************************************************************/

#include <FrogmoreScd30.h>

FrogmoreScd30 scd30;

struct {
  float humidity = 0.0f;
  float temperature = 0.0f;
  int error_state = SCD30_STATE_NO_ERROR;
  int loop_count = 0;
  int data_not_available_count = 0;
  int good_measure_count = 0;
  int reset_count = 0;
  int error_count = 0;
  int co2_zero_count = 0;
  int i2c_reset_count = 0;
  uint16_t interval;
  uint16_t co2 = 0;
  uint16_t co2e_avg = 0;
  bool init_once;
  bool found = false;
  bool data_valid = false;
} Scd30;

void Scd30Detect(void) {
  if (!I2cSetDevice(SCD30_ADDRESS)) { return; }

  scd30.begin();

  uint8_t major = 0;
  uint8_t minor = 0;
  if (scd30.getFirmwareVersion(&major, &minor)) { return; }
  if (scd30.getMeasurementInterval(&Scd30.interval)) { return; }
  if (scd30.beginMeasuring()) { return; }

  I2cSetActiveFound(SCD30_ADDRESS, "SCD30");
  Scd30.found = true;

//  AddLog(LOG_LEVEL_DEBUG, PSTR("SCD: FW v%d.%d"), major, minor);
}

// gets data from the sensor every 3 seconds or so to give the sensor time to gather new data
void Scd30Update(void) {
  Scd30.loop_count++;
  if (Scd30.loop_count > (Scd30.interval - 1)) {
    uint32_t error = 0;
    switch (Scd30.error_state) {
      case SCD30_STATE_NO_ERROR: {
        error = scd30.readMeasurement(&Scd30.co2, &Scd30.co2e_avg, &Scd30.temperature, &Scd30.humidity);
        switch (error) {
          case ERROR_SCD30_NO_ERROR:
            Scd30.loop_count = 0;
            Scd30.data_valid = true;
            Scd30.good_measure_count++;
#ifdef USE_LIGHT
            LightSetSignal(CO2_LOW, CO2_HIGH, Scd30.co2);
#endif  // USE_LIGHT
            break;

          case ERROR_SCD30_NO_DATA:
            Scd30.data_not_available_count++;
            break;

          case ERROR_SCD30_CRC_ERROR:
            Scd30.error_state = SCD30_STATE_ERROR_DATA_CRC;
            Scd30.error_count++;
#ifdef SCD30_DEBUG
            AddLog(LOG_LEVEL_ERROR, PSTR("SCD30: CRC error, CRC error: %ld, CO2 zero: %ld, good: %ld, no data: %ld, sc30_reset: %ld, i2c_reset: %ld"),
              Scd30.error_count, Scd30.co2_zero_count, Scd30.good_measure_count, Scd30.data_not_available_count, Scd30.reset_count, Scd30.i2c_reset_count);
#endif
            break;

          case ERROR_SCD30_CO2_ZERO:
            Scd30.co2_zero_count++;
#ifdef SCD30_DEBUG
            AddLog(LOG_LEVEL_ERROR, PSTR("SCD30: CO2 zero, CRC error: %ld, CO2 zero: %ld, good: %ld, no data: %ld, sc30_reset: %ld, i2c_reset: %ld"),
              Scd30.error_count, Scd30.co2_zero_count, Scd30.good_measure_count, Scd30.data_not_available_count, Scd30.reset_count, Scd30.i2c_reset_count);
#endif
            break;

          default: {
            Scd30.error_state = SCD30_STATE_ERROR_READ_MEAS;
#ifdef SCD30_DEBUG
            AddLog(LOG_LEVEL_ERROR, PSTR("SCD30: Update: ReadMeasurement error: 0x%lX, counter: %ld"), error, Scd30.loop_count);
#endif
             return;
          }
          break;
        }
      }
      break;

      case SCD30_STATE_ERROR_DATA_CRC: {
        //Scd30.data_valid = false;
#ifdef SCD30_DEBUG
        AddLog(LOG_LEVEL_ERROR, PSTR("SCD30: in error state: %d, good: %ld, no data: %ld, sc30 reset: %ld, i2c reset: %ld"),
          Scd30.error_state, Scd30.good_measure_count, Scd30.data_not_available_count, Scd30.reset_count, Scd30.i2c_reset_count);
        AddLog(LOG_LEVEL_ERROR, PSTR("SCD30: got CRC error, try again, counter: %ld"), Scd30.loop_count);
#endif
        Scd30.error_state = ERROR_SCD30_NO_ERROR;
      }
      break;

      case SCD30_STATE_ERROR_READ_MEAS: {
        //Scd30.data_valid = false;
#ifdef SCD30_DEBUG
        AddLog(LOG_LEVEL_ERROR, PSTR("SCD30: in error state: %d, good: %ld, no data: %ld, sc30 reset: %ld, i2c reset: %ld"),
          Scd30.error_state, Scd30.good_measure_count, Scd30.data_not_available_count, Scd30.reset_count, Scd30.i2c_reset_count);
        AddLog(LOG_LEVEL_ERROR, PSTR("SCD30: not answering, sending soft reset, counter: %ld"), Scd30.loop_count);
#endif
        Scd30.reset_count++;
        error = scd30.softReset();
        if (error) {
#ifdef SCD30_DEBUG
          AddLog(LOG_LEVEL_ERROR, PSTR("SCD30: resetting got error: 0x%lX"), error);
#endif
          error >>= 8;
          if (error == 4) {
            Scd30.error_state = SCD30_STATE_ERROR_SOFT_RESET;
          } else {
            Scd30.error_state = SCD30_STATE_ERROR_UNKNOWN;
          }
        } else {
          Scd30.error_state = ERROR_SCD30_NO_ERROR;
        }
      }
      break;

      case SCD30_STATE_ERROR_SOFT_RESET: {
        //Scd30.data_valid = false;
#ifdef SCD30_DEBUG
        AddLog(LOG_LEVEL_ERROR, PSTR("SCD30: in error state: %d, good: %ld, no data: %ld, sc30 reset: %ld, i2c reset: %ld"),
          Scd30.error_state, Scd30.good_measure_count, Scd30.data_not_available_count, Scd30.reset_count, Scd30.i2c_reset_count);
        AddLog(LOG_LEVEL_ERROR, PSTR("SCD30: clearing i2c bus"));
#endif
        Scd30.i2c_reset_count++;
        error = scd30.clearI2CBus();
        if (error) {
          Scd30.error_state = SCD30_STATE_ERROR_I2C_RESET;
#ifdef SCD30_DEBUG
          AddLog(LOG_LEVEL_ERROR, PSTR("SCD30: error clearing i2c bus: 0x%lX"), error);
#endif
        } else {
          Scd30.error_state = ERROR_SCD30_NO_ERROR;
        }
      }
      break;

      default: {
        //Scd30.data_valid = false;
#ifdef SCD30_DEBUG
        AddLog(LOG_LEVEL_ERROR, PSTR("SCD30: unknown error state: 0x%lX"), Scd30.error_state);
#endif
        Scd30.error_state = SCD30_STATE_ERROR_SOFT_RESET; // try again
      }
    }

    if (Scd30.loop_count > (SCD30_MAX_MISSED_READS * Scd30.interval)) {
      Scd30.data_valid = false;
    }
  }
}

/*********************************************************************************************\
 * Command Scd30
\*********************************************************************************************/

void CmndScd30Altitude(void) {
  uint16_t value = 0;
  if (XdrvMailbox.data_len > 0) {
    value = XdrvMailbox.payload;
    scd30.setAltitudeCompensation(value);
  } else {
    scd30.getAltitudeCompensation(&value);
  }
  ResponseCmndNumber(value);
};

void CmndScd30AutoMode(void) {
  uint16_t value = 0;
  if (XdrvMailbox.data_len > 0) {
    value = XdrvMailbox.payload;
    scd30.setCalibrationType(value);
  } else {
    scd30.getCalibrationType(&value);
  }
  ResponseCmndNumber(value);
};

void CmndScd30Calibrate(void) {
  uint16_t value = 0;
  if (XdrvMailbox.data_len > 0) {
    value = XdrvMailbox.payload;
    scd30.setForcedRecalibrationFactor(value);
  } else {
    scd30.getForcedRecalibrationFactor(&value);
  }
  ResponseCmndNumber(value);
};

void CmndScd30Firmware(void) {
  uint8_t major = 0;
  uint8_t minor = 0;
  int error = scd30.getFirmwareVersion(&major, &minor);
  if (!error) {
    float firmware = major + ((float)minor / 100);
    ResponseCmndFloat(firmware, 2);
  }
};

void CmndScd30Interval(void) {
  uint16_t value = 0;
  if (XdrvMailbox.data_len > 0) {
    value = XdrvMailbox.payload;
    int error = scd30.setMeasurementInterval(value);
    if (!error) {
      Scd30.interval = value;
    }
  }
  scd30.getMeasurementInterval(&value);
  ResponseCmndNumber(value);
};

void CmndScd30Pressure(void) {
  uint16_t value = 0;
  if (XdrvMailbox.data_len > 0) {
    value = XdrvMailbox.payload;
    scd30.setAmbientPressure(value);
  } else {
    scd30.getAmbientPressure(&value);
  }
  ResponseCmndNumber(value);
};

void CmndScd30TempOffset(void) {
  uint16_t value = 0;
  if (XdrvMailbox.data_len > 0) {
    value = XdrvMailbox.payload;
    scd30.setTemperatureOffset(value);
  } else {
    scd30.getTemperatureOffset(&value);
  }
  ResponseCmndNumber(value);
};

/********************************************************************************************/

void Scd30Show(bool json) {
  if (Scd30.data_valid) {
    float t = ConvertTemp(Scd30.temperature);
    float h = ConvertHumidity(Scd30.humidity);

    if (json) {
      ResponseAppend_P(PSTR(",\"SCD30\":{\"" D_JSON_CO2 "\":%d,\"" D_JSON_ECO2 "\":%d,"), Scd30.co2, Scd30.co2e_avg);
      ResponseAppendTHD(t, h);
      ResponseJsonEnd();
#ifdef USE_DOMOTICZ
      if (0 == TasmotaGlobal.tele_period) {
        DomoticzSensor(DZ_AIRQUALITY, Scd30.co2);
        DomoticzTempHumPressureSensor(t, h);
      }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_CO2EAVG, "SCD30", Scd30.co2e_avg);
      WSContentSend_PD(HTTP_SNS_CO2, "SCD30", Scd30.co2);
      WSContentSend_THD("SCD30", t, h);
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns42(uint32_t function) {
  if (!I2cEnabled(XI2C_29)) { return false; }

  bool result = false;

  // https://github.com/arendst/Tasmota/issues/15438 and datasheet (The boot-up time is < 2 s.)
/*
  if (FUNC_INIT == function) {
    Scd30Detect();
  }
*/
  if (!Scd30.init_once && (FUNC_EVERY_SECOND == function) && (TasmotaGlobal.uptime > 3)) {
    Scd30.init_once = true;
    Scd30Detect();
  }
  else if (Scd30.found) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        Scd30Update();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kScd30Commands, kScd30Command);
        break;
      case FUNC_JSON_APPEND:
        Scd30Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Scd30Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_SCD30
#endif  // USE_I2C
