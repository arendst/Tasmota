/*
  xsns_42_scd30.ino - SC30 CO2 sensor support for Tasmota

  Copyright (C) 2020 Frogmore42

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

#define XSNS_42        42
#define XI2C_29        29  // See I2CDEVICES.md

//#define SCD30_DEBUG

#define SCD30_ADDRESS  0x61

#define SCD30_MAX_MISSED_READS 3
#define SCD30_STATE_NO_ERROR 0
#define SCD30_STATE_ERROR_DATA_CRC 1
#define SCD30_STATE_ERROR_READ_MEAS 2
#define SCD30_STATE_ERROR_SOFT_RESET 3
#define SCD30_STATE_ERROR_I2C_RESET 4
#define SCD30_STATE_ERROR_UNKNOWN 5

#include "Arduino.h"
#include <FrogmoreScd30.h>

#define D_CMND_SCD30 "SCD30"

const char S_JSON_SCD30_COMMAND_NVALUE[] PROGMEM = "{\"" D_CMND_SCD30 "%s\":%d}";
const char S_JSON_SCD30_COMMAND_NFW_VALUE[] PROGMEM = "{\"" D_CMND_SCD30 "%s\":%d.%d}";
const char S_JSON_SCD30_COMMAND[] PROGMEM        = "{\"" D_CMND_SCD30 "%s\"}";
const char kSCD30_Commands[] PROGMEM             = "Alt|Auto|Cal|FW|Int|Pres|TOff";

/*********************************************************************************************\
 * enumerationsines
\*********************************************************************************************/

enum SCD30_Commands {         // commands useable in console or rules
  CMND_SCD30_ALTITUDE,
  CMND_SCD30_AUTOMODE,
  CMND_SCD30_CALIBRATE,
  CMND_SCD30_FW,
  CMND_SCD30_INTERVAL,
  CMND_SCD30_PRESSURE,
  CMND_SCD30_TEMPOFFSET
};

FrogmoreScd30 scd30;

bool scd30Found = false;
bool scd30IsDataValid = false;
int scd30ErrorState = SCD30_STATE_NO_ERROR;
uint16_t scd30Interval_sec;
int scd30Loop_count = 0;
int scd30DataNotAvailable_count = 0;
int scd30GoodMeas_count = 0;
int scd30Reset_count = 0;
int scd30CrcError_count = 0;
int scd30Co2Zero_count = 0;
int i2cReset_count = 0;
uint16_t scd30_CO2 = 0;
uint16_t scd30_CO2EAvg = 0;
float scd30_Humid = 0.0;
float scd30_Temp = 0.0;

void Scd30Detect(void)
{
  if (I2cActive(SCD30_ADDRESS)) { return; }

  scd30.begin();

  uint8_t major = 0;
  uint8_t minor = 0;
  if (scd30.getFirmwareVersion(&major, &minor)) { return; }
  uint16_t interval_sec;
  if (scd30.getMeasurementInterval(&scd30Interval_sec)) { return; }
  if (scd30.beginMeasuring()) { return; }

  I2cSetActiveFound(SCD30_ADDRESS, "SCD30");
  scd30Found = true;

  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SCD: FW v%d.%d"), major, minor);
}

// gets data from the sensor every 3 seconds or so to give the sensor time to gather new data
void Scd30Update(void)
{
  scd30Loop_count++;
  if (scd30Loop_count > (scd30Interval_sec - 1)) {
    int error = 0;
    switch (scd30ErrorState) {
      case SCD30_STATE_NO_ERROR: {
        error = scd30.readMeasurement(&scd30_CO2, &scd30_CO2EAvg, &scd30_Temp, &scd30_Humid);
        switch (error) {
          case ERROR_SCD30_NO_ERROR:
            scd30Loop_count = 0;
            scd30IsDataValid = true;
            scd30GoodMeas_count++;
            break;

          case ERROR_SCD30_NO_DATA:
            scd30DataNotAvailable_count++;
            break;

          case ERROR_SCD30_CRC_ERROR:
            scd30ErrorState = SCD30_STATE_ERROR_DATA_CRC;
            scd30CrcError_count++;
#ifdef SCD30_DEBUG
            AddLog_P2(LOG_LEVEL_ERROR, PSTR("SCD30: CRC error, CRC error: %ld, CO2 zero: %ld, good: %ld, no data: %ld, sc30_reset: %ld, i2c_reset: %ld"),
              scd30CrcError_count, scd30Co2Zero_count, scd30GoodMeas_count, scd30DataNotAvailable_count, scd30Reset_count, i2cReset_count);
#endif
            break;

          case ERROR_SCD30_CO2_ZERO:
            scd30Co2Zero_count++;
#ifdef SCD30_DEBUG
            AddLog_P2(LOG_LEVEL_ERROR, PSTR("SCD30: CO2 zero, CRC error: %ld, CO2 zero: %ld, good: %ld, no data: %ld, sc30_reset: %ld, i2c_reset: %ld"),
              scd30CrcError_count, scd30Co2Zero_count, scd30GoodMeas_count, scd30DataNotAvailable_count, scd30Reset_count, i2cReset_count);
#endif
            break;

          default: {
            scd30ErrorState = SCD30_STATE_ERROR_READ_MEAS;
#ifdef SCD30_DEBUG
            AddLog_P2(LOG_LEVEL_ERROR, PSTR("SCD30: Update: ReadMeasurement error: 0x%lX, counter: %ld"), error, scd30Loop_count);
#endif
             return;
          }
          break;
        }
      }
      break;

      case SCD30_STATE_ERROR_DATA_CRC: {
        //scd30IsDataValid = false;
#ifdef SCD30_DEBUG
        AddLog_P2(LOG_LEVEL_ERROR, PSTR("SCD30: in error state: %d, good: %ld, no data: %ld, sc30 reset: %ld, i2c reset: %ld"),
          scd30ErrorState, scd30GoodMeas_count, scd30DataNotAvailable_count, scd30Reset_count, i2cReset_count);
        AddLog_P2(LOG_LEVEL_ERROR, PSTR("SCD30: got CRC error, try again, counter: %ld"), scd30Loop_count);
#endif
        scd30ErrorState = ERROR_SCD30_NO_ERROR;
      }
      break;

      case SCD30_STATE_ERROR_READ_MEAS: {
        //scd30IsDataValid = false;
#ifdef SCD30_DEBUG
        AddLog_P2(LOG_LEVEL_ERROR, PSTR("SCD30: in error state: %d, good: %ld, no data: %ld, sc30 reset: %ld, i2c reset: %ld"),
          scd30ErrorState, scd30GoodMeas_count, scd30DataNotAvailable_count, scd30Reset_count, i2cReset_count);
        AddLog_P2(LOG_LEVEL_ERROR, PSTR("SCD30: not answering, sending soft reset, counter: %ld"), scd30Loop_count);
#endif
        scd30Reset_count++;
        error = scd30.softReset();
        if (error) {
#ifdef SCD30_DEBUG
          AddLog_P2(LOG_LEVEL_ERROR, PSTR("SCD30: resetting got error: 0x%lX"), error);
#endif
          error >>= 8;
          if (error == 4) {
            scd30ErrorState = SCD30_STATE_ERROR_SOFT_RESET;
          } else {
            scd30ErrorState = SCD30_STATE_ERROR_UNKNOWN;
          }
        } else {
          scd30ErrorState = ERROR_SCD30_NO_ERROR;
        }
      }
      break;

      case SCD30_STATE_ERROR_SOFT_RESET: {
        //scd30IsDataValid = false;
#ifdef SCD30_DEBUG
        AddLog_P2(LOG_LEVEL_ERROR, PSTR("SCD30: in error state: %d, good: %ld, no data: %ld, sc30 reset: %ld, i2c reset: %ld"),
          scd30ErrorState, scd30GoodMeas_count, scd30DataNotAvailable_count, scd30Reset_count, i2cReset_count);
        AddLog_P(LOG_LEVEL_ERROR, PSTR("SCD30: clearing i2c bus"));
#endif
        i2cReset_count++;
        error = scd30.clearI2CBus();
        if (error) {
          scd30ErrorState = SCD30_STATE_ERROR_I2C_RESET;
#ifdef SCD30_DEBUG
          AddLog_P2(LOG_LEVEL_ERROR, PSTR("SCD30: error clearing i2c bus: 0x%lX"), error);
#endif
        } else {
          scd30ErrorState = ERROR_SCD30_NO_ERROR;
        }
      }
      break;

      default: {
        //scd30IsDataValid = false;
#ifdef SCD30_DEBUG
        AddLog_P2(LOG_LEVEL_ERROR, PSTR("SCD30: unknown error state: 0x%lX"), scd30ErrorState);
#endif
        scd30ErrorState = SCD30_STATE_ERROR_SOFT_RESET; // try again
      }
    }

    if (scd30Loop_count > (SCD30_MAX_MISSED_READS * scd30Interval_sec)) {
      scd30IsDataValid = false;
    }
  }
}


int Scd30GetCommand(int command_code, uint16_t *pvalue)
{
  switch (command_code)
  {
    case CMND_SCD30_ALTITUDE:
      return scd30.getAltitudeCompensation(pvalue);
      break;

    case CMND_SCD30_AUTOMODE:
      return scd30.getCalibrationType(pvalue);
      break;

    case CMND_SCD30_CALIBRATE:
      return scd30.getForcedRecalibrationFactor(pvalue);
      break;

    case CMND_SCD30_INTERVAL:
      return scd30.getMeasurementInterval(pvalue);
      break;

    case CMND_SCD30_PRESSURE:
      return scd30.getAmbientPressure(pvalue);
      break;

    case CMND_SCD30_TEMPOFFSET:
      return scd30.getTemperatureOffset(pvalue);
      break;

      default:
        // else for Unknown command
      break;
  }
}

int Scd30SetCommand(int command_code, uint16_t value)
{
  switch (command_code)
  {
    case CMND_SCD30_ALTITUDE:
      return scd30.setAltitudeCompensation(value);
      break;

    case CMND_SCD30_AUTOMODE:
      return scd30.setCalibrationType(value);
      break;

    case CMND_SCD30_CALIBRATE:
      return scd30.setForcedRecalibrationFactor(value);
      break;

    case CMND_SCD30_INTERVAL:
    {
      int error = scd30.setMeasurementInterval(value);
      if (!error)
      {
        scd30Interval_sec = value;
      }

      return error;
    }
      break;

    case CMND_SCD30_PRESSURE:
      return scd30.setAmbientPressure(value);
      break;

    case CMND_SCD30_TEMPOFFSET:
      return scd30.setTemperatureOffset(value);
      break;

      default:
        // else for Unknown command
      break;
  }
}

/*********************************************************************************************\
 * Command Sensor42
\*********************************************************************************************/

bool Scd30CommandSensor()
{
  char command[CMDSZ];
  bool serviced = true;
  uint8_t prefix_len = strlen(D_CMND_SCD30);

  if (!strncasecmp_P(XdrvMailbox.topic, PSTR(D_CMND_SCD30), prefix_len)) {  // prefix
    int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic + prefix_len, kSCD30_Commands);

    switch (command_code) {
      case CMND_SCD30_ALTITUDE:
      case CMND_SCD30_AUTOMODE:
      case CMND_SCD30_CALIBRATE:
      case CMND_SCD30_INTERVAL:
      case CMND_SCD30_PRESSURE:
      case CMND_SCD30_TEMPOFFSET:
      {
        uint16_t value = 0;
        if (XdrvMailbox.data_len > 0)
        {
          value = XdrvMailbox.payload;
          Scd30SetCommand(command_code, value);
        }
        else
        {
          Scd30GetCommand(command_code, &value);
        }

        Response_P(S_JSON_SCD30_COMMAND_NVALUE, command, value);
      }
        break;

      case CMND_SCD30_FW:
      {
        uint8_t major = 0;
        uint8_t minor = 0;
        int error;
        error = scd30.getFirmwareVersion(&major, &minor);
        if (error)
        {
#ifdef SCD30_DEBUG
          AddLog_P2(LOG_LEVEL_ERROR, PSTR("SCD30: error getting FW version: 0x%lX"), error);
#endif
          serviced = false;
        }
        else
        {
          Response_P(S_JSON_SCD30_COMMAND_NFW_VALUE, command, major, minor);
        }
      }
        break;

      default:
        // else for Unknown command
        serviced = false;
      break;
    }
  }
  return serviced;
}

void Scd30Show(bool json)
{
  if (scd30IsDataValid)
  {
    char humidity[10];
    dtostrfd(ConvertHumidity(scd30_Humid), Settings.flag2.humidity_resolution, humidity);
    char temperature[10];
    dtostrfd(ConvertTemp(scd30_Temp), Settings.flag2.temperature_resolution, temperature);

    if (json) {
      //ResponseAppend_P(PSTR(",\"SCD30\":{\"" D_JSON_CO2 "\":%d,\"" D_JSON_TEMPERATURE "\":%s,\"" D_JSON_HUMIDITY "\":%s}"), scd30_CO2, temperature, humidity);
      ResponseAppend_P(PSTR(",\"SCD30\":{\"" D_JSON_CO2 "\":%d,\"" D_JSON_ECO2 "\":%d,\"" D_JSON_TEMPERATURE "\":%s,\"" D_JSON_HUMIDITY "\":%s}"),
        scd30_CO2, scd30_CO2EAvg, temperature, humidity);
#ifdef USE_DOMOTICZ
      if (0 == tele_period)
      {
        DomoticzSensor(DZ_AIRQUALITY, scd30_CO2);
        DomoticzTempHumSensor(temperature, humidity);
      }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_CO2EAVG, "SCD30", scd30_CO2EAvg);
      WSContentSend_PD(HTTP_SNS_CO2, "SCD30", scd30_CO2);
      WSContentSend_PD(HTTP_SNS_TEMP, "SCD30", temperature, TempUnit());
      WSContentSend_PD(HTTP_SNS_HUM, "SCD30", humidity);
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns42(byte function)
{
  if (!I2cEnabled(XI2C_29)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    Scd30Detect();
  }
  else if (scd30Found) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        Scd30Update();
        break;
      case FUNC_COMMAND:
        result = Scd30CommandSensor();
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
