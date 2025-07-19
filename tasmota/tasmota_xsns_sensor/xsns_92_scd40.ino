/*
  xsns_92_scd40.ino - SCD40/SCD41 I2C CO2(+temp+RH) sensor support for Tasmota,
                      based on frogmore42's xsns_42_scd30.ino

  Copyright (C) 2021  Frogmore42, Arnold-n

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

// define USE_SCD40 to use SCD40 or SCD41 device; use SCD41-functions only if you use SCD41 sensor
// define USE_SCD40_LOWPOWER to use low-power periodic measurement mode (both SCD40 and SCD41)
// define SCD40_DEBUG to debug

// Console instructions supported: (errorvalue=-1 in case of error, errorvalue=0 otherwise)
//                                 (data=-1 in case of error, value otherwise)
//                                 (third colum: time in ms needed for execution)
//                                 (DPM: may be executed during periodic measurements)
//
// Instruction    Returns   Exec(ms) DPM Function
// ------------------------------------------------------------------------------------
// SCD40Alt       data            1  no  get Sensor Altitude (in m)
// SCD40Alt x     errorvalue      1  no  set Sensor Altitude (in m)
// SCD40Auto      data            1  no  get CalibrationEnabled status (bool)
// SCD40Auto x    errorvalue      1  no  set CalibrationEnabled status (bool)
// SCD40Toff      data            1  no  get Temperature offset (centigrades)
// SCD40Toff x    errorvalue      1  no  set Temperature offset (centigrades) (some rounding may occur)
// SCD40Pres x    errorvalue      1  yes set Ambient Pressure (mbar) (overrides Sensor Altitude setting)
// SCD40Cal x     errorvalue    400  no  perform forced recalibration (ppm CO2)
// SCD40Test      errorvalue  10000  no  perform selftest
// SCD40StLp      errorvalue      0  no  start periodic measurement in low-power mode (1/30s)
// SCD40Strt      errorvalue      0  no  start periodic measurement (1/5s)
// SCD40Stop      errorvalue    500  yes stop periodic measurement
// SCD40Pers      errorvalue    800  no  persist settings in EEPROM (2000 write cycles guaranteed)
// SCD40Rein      errorvalue     20  no  reinit sensor
// SCD40Fact      errorvalue   1200  no  factory reset sensor
//
// SCD40Sing      errorvalue   5000  no  (SCD41 only) measure single shot
// SCD40SRHT      errorvalue     50  no  (SCD41 only) measure single shot, RHT only

//#define SCD40_DEBUG

#ifdef USE_I2C

#ifdef USE_SCD40

#define XSNS_92        92
#define XI2C_62        62  // See I2CDEVICES.md

// #define SCD40_ADDRESS  0x62 // already defined in lib

#define SCD40_MAX_MISSED_READS 30 // in seconds (at 1 read/second)
#define SCD40_STATE_NO_ERROR 0
#define SCD40_STATE_ERROR_DATA_CRC 1
#define SCD40_STATE_ERROR_READ_MEAS 2
#define SCD40_STATE_ERROR_SOFT_RESET 3
#define SCD40_STATE_ERROR_I2C_RESET 4

#include "Arduino.h"
#include <FrogmoreScd40.h>

#define D_CMND_SCD40 "SCD40"

const char S_JSON_SCD40_COMMAND_NVALUE[] PROGMEM = "{\"" D_CMND_SCD40 "%s\":%d}";
//const char S_JSON_SCD40_COMMAND[] PROGMEM        = "{\"" D_CMND_SCD40 "%s\"}";
const char kSCD40_Commands[] PROGMEM             = "Alt|Auto|Toff|Pres|Cal|Test|StLP|Strt|Stop|Pers|Rein|Fact|Sing|SRHT";

/*********************************************************************************************\
 * enumerations
\*********************************************************************************************/

enum SCD40_Commands {         // commands useable in console or rules
  CMND_SCD40_ALTITUDE,
  CMND_SCD40_AUTOMODE,
  CMND_SCD40_TEMPOFFSET,
  CMND_SCD40_PRESSURE,
  CMND_SCD40_FORCEDRECALIBRATION,
  CMND_SCD40_SELFTEST,
  CMND_SCD40_START_MEASUREMENT_LOW_POWER,
  CMND_SCD40_START_MEASUREMENT,
  CMND_SCD40_STOP_MEASUREMENT,
  CMND_SCD40_PERSIST,
  CMND_SCD40_REINIT,
  CMND_SCD40_FACTORYRESET,
  CMND_SCD40_SINGLESHOT,
  CMND_SCD40_SINGLESHOT_RHT_ONLY
};

FrogmoreScd40 scd40;

bool scd40Found = false;
bool scd40IsDataValid = false;
int scd40ErrorState = SCD40_STATE_NO_ERROR;
int scd40Loop_count = 0;
int scd40DataNotAvailable_count = 0;
int scd40GoodMeas_count = 0;
int scd40Reset_count = 0;
int scd40CrcError_count = 0;
int scd40i2cReset_count = 0;
uint16_t scd40_CO2 = 0;
uint16_t scd40_CO2EAvg = 0;
float scd40_Humid = 0.0;
float scd40_Temp = 0.0;

void Scd40Detect(void)
{
  if (!I2cSetDevice(SCD40_ADDRESS)) { return; }

  scd40.begin(&I2cGetWire());

  // don't stop in case of error, try to continue
  delay(10); // not sure whether this is needed
  int error = scd40.forceStopPeriodicMeasurement();  // after reboot, stop (if any) periodic measurement, or reinit may not work
#ifdef SCD40_DEBUG
  AddLog(LOG_LEVEL_DEBUG, PSTR("SCD40 force-stop error: %d"), error);
#endif
  delay(550); // wait >500ms after stopPeriodicMeasurement before SCD40 allows any other command

  error = scd40.reinit(); // just in case
#ifdef SCD40_DEBUG
  AddLog(LOG_LEVEL_DEBUG, PSTR("SCD40 reinit error: %d"), error);
#endif
  delay(20); // not sure whether this is needed

  uint16_t sn[3];
  error = scd40.getSerialNumber(sn);
  AddLog(LOG_LEVEL_INFO, PSTR("SCD40 serial nr 0x%X 0x%X 0x%X") ,sn[0], sn[1], sn[2]);

  //  by default, start measurements, only register device if this succeeds
#ifdef USE_SCD40_LOWPOWER
  if (scd40.startLowPowerPeriodicMeasurement()) { return; }
#else
  if (scd40.startPeriodicMeasurement()) { return; }
#endif
  I2cSetActiveFound(SCD40_ADDRESS, "SCD40");
  scd40Found = true;
#ifdef SCD40_DEBUG
  AddLog(LOG_LEVEL_DEBUG, PSTR("SCD40 found, measurements started."));
#endif
}

// gets data from the sensor
void Scd40Update(void)
{
  bool isAvailable;

  scd40Loop_count++;

  uint32_t error = 0;
  switch (scd40ErrorState) {
    case SCD40_STATE_NO_ERROR: {
      error = scd40.readMeasurement(&scd40_CO2, &scd40_CO2EAvg, &scd40_Temp, &scd40_Humid);
      switch (error) {
        case ERROR_SCD40_NO_ERROR:
          scd40Loop_count = 0;
          scd40IsDataValid = true;
          scd40GoodMeas_count++;
#ifdef USE_LIGHT
          LightSetSignal(CO2_LOW, CO2_HIGH, scd40_CO2);
#endif  // USE_LIGHT
          break;

        case ERROR_SCD40_NO_DATA:
          scd40DataNotAvailable_count++;
          AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SCD40: no data available."));
          break;

        case ERROR_SCD40_CRC_ERROR:
          scd40CrcError_count++;
#ifdef SCD40_DEBUG
          AddLog(LOG_LEVEL_INFO, PSTR("SCD40: CRC error, CRC error: %ld, good: %ld, no data: %ld, sc30_reset: %ld, i2c_reset: %ld"),
            scd40CrcError_count, scd40GoodMeas_count, scd40DataNotAvailable_count, scd40Reset_count, scd40i2cReset_count);
#endif
          break;

        default: {
          scd40ErrorState = SCD40_STATE_ERROR_READ_MEAS;
#ifdef SCD40_DEBUG
          AddLog(LOG_LEVEL_INFO, PSTR("SCD40: Update: ReadMeasurement error: 0x%lX, counter: %ld"), error, scd40Loop_count);
#endif
          return;
        }
        break;
      }
    }
    break;

    case SCD40_STATE_ERROR_READ_MEAS: {
#ifdef SCD40_DEBUG
      AddLog(LOG_LEVEL_INFO, PSTR("SCD40: (rd) in error state: %d, good: %ld, no data: %ld, sc30 reset: %ld, i2c reset: %ld"),
        scd40ErrorState, scd40GoodMeas_count, scd40DataNotAvailable_count, scd40Reset_count, scd40i2cReset_count);
      AddLog(LOG_LEVEL_INFO, PSTR("SCD40: not answering, sending soft reset, counter: %ld"), scd40Loop_count);
#endif
      scd40Reset_count++;
      error = scd40.stopPeriodicMeasurement();
      if (error) {
        scd40ErrorState = SCD40_STATE_ERROR_SOFT_RESET;
#ifdef SCD40_DEBUG
        AddLog(LOG_LEVEL_INFO, PSTR("SCD40: stopPeriodicMeasurement got error: 0x%lX"), error);
#endif
      } else {
        error = scd40.reinit();
        if (error) {
#ifdef SCD40_DEBUG
          AddLog(LOG_LEVEL_INFO, PSTR("SCD40: resetting got error: 0x%lX"), error);
#endif
          scd40ErrorState = SCD40_STATE_ERROR_SOFT_RESET;
        } else {
          scd40ErrorState = ERROR_SCD40_NO_ERROR;
        }
      }
    }
    break;

    case SCD40_STATE_ERROR_SOFT_RESET: {
#ifdef SCD40_DEBUG
      AddLog(LOG_LEVEL_INFO, PSTR("SCD40: (rst) in error state: %d, good: %ld, no data: %ld, sc30 reset: %ld, i2c reset: %ld"),
        scd40ErrorState, scd40GoodMeas_count, scd40DataNotAvailable_count, scd40Reset_count, scd40i2cReset_count);
      AddLog(LOG_LEVEL_INFO, PSTR("SCD40: clearing i2c bus"));
#endif
      scd40i2cReset_count++;
      error = scd40.clearI2CBus();
      if (error) {
        scd40ErrorState = SCD40_STATE_ERROR_I2C_RESET;
#ifdef SCD40_DEBUG
        AddLog(LOG_LEVEL_INFO, PSTR("SCD40: error clearing i2c bus: 0x%lX"), error);
#endif
      } else {
        scd40ErrorState = ERROR_SCD40_NO_ERROR;
      }
    }
    break;

    case SCD40_STATE_ERROR_I2C_RESET: {
      // Give up
    }
    break;
  }

  if (scd40Loop_count > SCD40_MAX_MISSED_READS) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("SCD40: max-missed-reads."));
    scd40IsDataValid = false;
  }
}

int Scd40GetCommand(int command_code, uint16_t *pvalue)
{
  switch (command_code)
  {
    case CMND_SCD40_ALTITUDE:
      return scd40.getSensorAltitude(pvalue);
      break;

    case CMND_SCD40_AUTOMODE:
      return scd40.getAutomaticSelfCalibrationEnabled(pvalue);
      break;

    case CMND_SCD40_TEMPOFFSET:
      return scd40.getTemperatureOffset(pvalue);
      break;

    case CMND_SCD40_SELFTEST:
      return scd40.performSelfTest(pvalue);
      break;

    default:
      // else for Unknown command
    break;
  }
  return 0;  // Fix GCC 10.1 warning
}

int Scd40SetCommand(int command_code, uint16_t *pvalue)
{
  switch (command_code)
  {
    case CMND_SCD40_ALTITUDE:
      return scd40.setSensorAltitude(*pvalue);
      break;

    case CMND_SCD40_AUTOMODE:
      return scd40.setAutomaticSelfCalibrationEnabled((bool) (*pvalue));
      break;

    case CMND_SCD40_TEMPOFFSET:
      return scd40.setTemperatureOffset(*pvalue);
      break;

    case CMND_SCD40_PRESSURE:
      return scd40.setAmbientPressure(*pvalue);
      break;

    case CMND_SCD40_FORCEDRECALIBRATION:
      return scd40.performForcedRecalibration(*pvalue);
      break;

    default:
      // else for Unknown command
    break;
  }
  return 0;  // Fix GCC 10.1 warning
}

/*********************************************************************************************\
 * Command Sensor42
\*********************************************************************************************/

bool Scd40CommandSensor()
{
  char command[CMDSZ];
  bool serviced = true;
  uint8_t prefix_len = strlen(D_CMND_SCD40);
  int error;

  if (!strncasecmp_P(XdrvMailbox.topic, PSTR(D_CMND_SCD40), prefix_len)) {  // prefix
    int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic + prefix_len, kSCD40_Commands);

// not supported here: readMeasurement, getDataReadyStatus, getSerialNumber, measure_single_shot, measure_single_shot_rht_only

    switch (command_code) {
      case CMND_SCD40_ALTITUDE:
      case CMND_SCD40_AUTOMODE:
      case CMND_SCD40_TEMPOFFSET:
      case CMND_SCD40_PRESSURE:            // write-only
      case CMND_SCD40_FORCEDRECALIBRATION: // write-only
      case CMND_SCD40_SELFTEST:            // read-only
      {
        uint16_t value = 0;
        if (XdrvMailbox.data_len > 0)
        {
          if (command_code != CMND_SCD40_SELFTEST) {
            value = XdrvMailbox.payload;
            error = Scd40SetCommand(command_code, &value);
            Response_P(S_JSON_SCD40_COMMAND_NVALUE, command, error?-1:0);
          } else {
            serviced = false;
            break;
          }
        }
        else
        {
          if ((command_code != CMND_SCD40_PRESSURE) && (command_code != CMND_SCD40_FORCEDRECALIBRATION)) {
            error = Scd40GetCommand(command_code, &value);
            Response_P(S_JSON_SCD40_COMMAND_NVALUE, command, error?-1:value);
          } else {
            serviced = false;
            break;
          }
        }
      }
      break;

      case CMND_SCD40_START_MEASUREMENT_LOW_POWER:
      {
        error = scd40.startLowPowerPeriodicMeasurement();
        Response_P(S_JSON_SCD40_COMMAND_NVALUE, command, error?-1:0);
      }
      break;

      case CMND_SCD40_START_MEASUREMENT:
      {
        error = scd40.startPeriodicMeasurement();
        Response_P(S_JSON_SCD40_COMMAND_NVALUE, command, error?-1:0);
      }
      break;

      case CMND_SCD40_STOP_MEASUREMENT:
      {
        error = scd40.stopPeriodicMeasurement();
        Response_P(S_JSON_SCD40_COMMAND_NVALUE, command, error?-1:0);
      }
      break;

      case CMND_SCD40_PERSIST:
      {
        error = scd40.persistSettings();
        Response_P(S_JSON_SCD40_COMMAND_NVALUE, command, error?-1:0);
      }
      break;

      case CMND_SCD40_REINIT:
      {
        error = scd40.reinit();
        Response_P(S_JSON_SCD40_COMMAND_NVALUE, command, error?-1:0);
      }
      break;

      case CMND_SCD40_FACTORYRESET:
      {
        error = scd40.performFactoryReset();
        Response_P(S_JSON_SCD40_COMMAND_NVALUE, command, error?-1:0);
      }
      break;

      case CMND_SCD40_SINGLESHOT:
      {
        error = scd40.measureSingleShot();
        Response_P(S_JSON_SCD40_COMMAND_NVALUE, command, error?-1:0);
      }
      break;

      case CMND_SCD40_SINGLESHOT_RHT_ONLY:
      {
        error = scd40.measureSingleShotRhtOnly();
        Response_P(S_JSON_SCD40_COMMAND_NVALUE, command, error?-1:0);
      }
      break;

      default:
        // else for Unknown command
        serviced = false;
      break;
    }
  } else {
    serviced = false;
  }
  return serviced;
}

void Scd40Show(bool json)
{
  if (scd40IsDataValid)
  {
    float t = ConvertTemp(scd40_Temp);
    float h = ConvertHumidity(scd40_Humid);

    if (json) {
      ResponseAppend_P(PSTR(",\"SCD40\":{\"" D_JSON_CO2 "\":%d,\"" D_JSON_ECO2 "\":%d,"), scd40_CO2, scd40_CO2EAvg);
      ResponseAppendTHD(t, h);
      ResponseJsonEnd();
#ifdef USE_DOMOTICZ
      if (0 == TasmotaGlobal.tele_period) {
        DomoticzSensor(DZ_AIRQUALITY, scd40_CO2);
        DomoticzTempHumPressureSensor(t, h);
      }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_CO2EAVG, "SCD40", scd40_CO2EAvg);
      WSContentSend_PD(HTTP_SNS_CO2, "SCD40", scd40_CO2);
      WSContentSend_THD("SCD40", t, h);
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns92(uint32_t function)
{
  if (!I2cEnabled(XI2C_62)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    Scd40Detect();
  }
  else if (scd40Found) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        Scd40Update();
        break;
      case FUNC_COMMAND:
        result = Scd40CommandSensor();
        break;
      case FUNC_JSON_APPEND:
        Scd40Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Scd40Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_SCD40
#endif  // USE_I2C
