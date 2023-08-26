/*
  xsns_103_sen5x.ino - SEN5X gas and air quality sensor support for Tasmota

  Copyright (C) 2022  Tyeth Gundry

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
#ifdef USE_SEN5X
/*********************************************************************************************\
 * SEN5X - Gas (VOC - Volatile Organic Compounds / NOx - Nitrous Oxides) and Particulates (PM)
 *
 * Source: Sensirion SEN5X Driver + Example, and Tasmota Driver 98 by Jean-Pierre Deschamps
 * Adaption for TASMOTA: Tyeth Gundry
 *
 * I2C Address: 0x69
\*********************************************************************************************/

#define XSNS_103 103
#define XI2C_76 76 // See I2CDEVICES.md

#define SEN5X_ADDRESS 0x69
#define SEN5X_PASSIVE_MODE_INTERVAL 10

#include <SensirionI2CSen5x.h>
#include <Wire.h>
SensirionI2CSen5x *sen5x = nullptr;

struct SEN5XDATA_s {
  float massConcentrationPm1p0;
  float massConcentrationPm2p5;
  float massConcentrationPm4p0;
  float massConcentrationPm10p0;
  float ambientHumidity;
  float ambientTemperature;
  float vocIndex;
  float noxIndex;
} *SEN5XDATA = nullptr;

/********************************************************************************************/

void sen5x_Init(void) {
  int usingI2cBus = 0;
#ifdef ESP32
  if (!I2cSetDevice(SEN5X_ADDRESS, 0)) {
    DEBUG_SENSOR_LOG(PSTR("Sensirion SEN5X not found, i2c bus 0"));
    if (TasmotaGlobal.i2c_enabled_2 ) {
      if(!I2cSetDevice(SEN5X_ADDRESS, 1)) {
        DEBUG_SENSOR_LOG(PSTR("Sensirion SEN5X not found, i2c bus 1"));
        return;
      }
      usingI2cBus = 1;
    } else {
      return;
    }
  }
#else
  if (!I2cSetDevice(SEN5X_ADDRESS)) {
    DEBUG_SENSOR_LOG(PSTR("Sensirion SEN5X not found, i2c bus 0"));
    return;
  }
#endif

  sen5x = new SensirionI2CSen5x();
  if (1 == usingI2cBus) {
#ifdef ESP32
    sen5x->begin(Wire1);
#else
    sen5x->begin(Wire);
#endif
  }
  else {
    sen5x->begin(Wire);
  }
  
  if (!Settings->flag6.sen5x_passive_mode) {
    int error_stop = sen5x->deviceReset();
    if (error_stop != 0) {
      DEBUG_SENSOR_LOG(PSTR("Sensirion SEN5X failed to reset device (I2C Bus %d)"), usingI2cBus);
      return;
    }
    // Wait 1 second for sensors to start recording + 100ms for reset command
    delay(1100);
    int error_start = sen5x->startMeasurement();
    if (error_start != 0) {
      DEBUG_SENSOR_LOG(PSTR("Sensirion SEN5X failed to start measurement (I2C Bus %d)"), usingI2cBus);
      return;
    }
  }

  SEN5XDATA = (SEN5XDATA_s *)calloc(1, sizeof(struct SEN5XDATA_s));
  I2cSetActiveFound(SEN5X_ADDRESS, "SEN5X", usingI2cBus);
}

void SEN5XUpdate(void) {  // Perform every second to ensure proper operation of the baseline compensation algorithm
  uint16_t error;
  char errorMessage[256];
  DEBUG_SENSOR_LOG(PSTR("Running readMeasuredValues for SEN5X..."));

  error = sen5x->readMeasuredValues(
      SEN5XDATA->massConcentrationPm1p0, SEN5XDATA->massConcentrationPm2p5, SEN5XDATA->massConcentrationPm4p0,
      SEN5XDATA->massConcentrationPm10p0, SEN5XDATA->ambientHumidity, SEN5XDATA->ambientTemperature, SEN5XDATA->vocIndex,
      SEN5XDATA->noxIndex);

  if (error) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("S5X: Failed to retrieve readings"));
#ifdef DEBUG_TASMOTA_SENSOR
    DEBUG_SENSOR_LOG(PSTR("Error trying to execute readMeasuredValues():"));
    errorToString(error, errorMessage, 256);
    DEBUG_SENSOR_LOG(errorMessage);
  } else {
    DEBUG_SENSOR_LOG(PSTR("SEN5x readings:-"));
    DEBUG_SENSOR_LOG(PSTR("MassConcentrationPm1p0: %1_f"), &SEN5XDATA->massConcentrationPm1p0);
    DEBUG_SENSOR_LOG(PSTR("MassConcentrationPm2p5: %1_f"), &SEN5XDATA->massConcentrationPm2p5);
    DEBUG_SENSOR_LOG(PSTR("MassConcentrationPm4p0: %1_f"), &SEN5XDATA->massConcentrationPm4p0);
    DEBUG_SENSOR_LOG(PSTR("MassConcentrationPm10p0: %1_f"), &SEN5XDATA->massConcentrationPm10p0);
    if (isnan(SEN5XDATA->ambientHumidity)) {
      DEBUG_SENSOR_LOG(PSTR("AmbientHumidity: n/a"));
    } else {
      DEBUG_SENSOR_LOG(PSTR("AmbientHumidity: %*_f"), 2, &SEN5XDATA->ambientHumidity);
    }

    if (isnan(SEN5XDATA->ambientTemperature)) {
      DEBUG_SENSOR_LOG(PSTR("AmbientTemperature: n/a"));
    } else {
      DEBUG_SENSOR_LOG(PSTR("AmbientTemperature: %*_f"), 2, &SEN5XDATA->ambientTemperature);
    }

    if (isnan(SEN5XDATA->vocIndex)) {
      DEBUG_SENSOR_LOG(PSTR("VocIndex: n/a"));
    } else {
      DEBUG_SENSOR_LOG(PSTR("VocIndex: %*_f"), 0, &SEN5XDATA->vocIndex);
    }

    if (isnan(SEN5XDATA->noxIndex)) {
      DEBUG_SENSOR_LOG(PSTR("NoxIndex: n/a"));
    } else {
      DEBUG_SENSOR_LOG(PSTR("NoxIndex: %*_f"), 0, &SEN5XDATA->noxIndex);
    }
#endif
  }
}

void SEN5XShow(bool json) {
  char types[10];
  strcpy_P(types, PSTR("SEN5X"));

  float temperature = 0;
  float humidity = 0;
  float abs_humidity = 0;
  bool ahum_available = (!isnan(SEN5XDATA->ambientTemperature) && !isnan(SEN5XDATA->ambientHumidity) && (SEN5XDATA->ambientHumidity > 0));
  if (ahum_available) {
    temperature = ConvertTemp(SEN5XDATA->ambientTemperature);
    humidity = ConvertHumidity(SEN5XDATA->ambientHumidity);
    abs_humidity = CalcTempHumToAbsHum(SEN5XDATA->ambientTemperature, SEN5XDATA->ambientHumidity);
  }

  if (json) {
    ResponseAppend_P(PSTR(",\"%s\":{\"PM1\":%1_f,\"PM2.5\":%1_f,\"PM4\":%1_f,\"PM10\":%1_f,"),
      types,
      &SEN5XDATA->massConcentrationPm1p0, &SEN5XDATA->massConcentrationPm2p5, &SEN5XDATA->massConcentrationPm4p0, &SEN5XDATA->massConcentrationPm10p0);
    if (!isnan(SEN5XDATA->noxIndex)) {
      ResponseAppend_P(PSTR("\"NOx\":%0_f,"), &SEN5XDATA->noxIndex);
    }
    if (!isnan(SEN5XDATA->vocIndex)) {
      ResponseAppend_P(PSTR("\"VOC\":%0_f,"), &SEN5XDATA->vocIndex);
    }
    if (ahum_available) {
      ResponseAppendTHD(temperature, humidity);
      ResponseAppend_P(PSTR(",\"" D_JSON_AHUM "\":%4_f"), &abs_humidity);
    }
    ResponseJsonEnd();
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_F_ENVIRONMENTAL_CONCENTRATION, types, "1", &SEN5XDATA->massConcentrationPm1p0);
    WSContentSend_PD(HTTP_SNS_F_ENVIRONMENTAL_CONCENTRATION, types, "2.5", &SEN5XDATA->massConcentrationPm2p5);
    WSContentSend_PD(HTTP_SNS_F_ENVIRONMENTAL_CONCENTRATION, types, "4", &SEN5XDATA->massConcentrationPm4p0);
    WSContentSend_PD(HTTP_SNS_F_ENVIRONMENTAL_CONCENTRATION, types, "10", &SEN5XDATA->massConcentrationPm10p0);
    if (!isnan(SEN5XDATA->noxIndex)) {
      WSContentSend_PD(HTTP_SNS_F_NOX, types, 0, &SEN5XDATA->noxIndex);
    }
    if (!isnan(SEN5XDATA->vocIndex)) {
      WSContentSend_PD(HTTP_SNS_F_VOC, types, 0, &SEN5XDATA->vocIndex);
    }
    if (ahum_available) {
      WSContentSend_THD(types, temperature, humidity);
      WSContentSend_PD(HTTP_SNS_F_ABS_HUM, types, 4, &abs_humidity);
    }
#endif
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns103(uint32_t function) {
  if (!I2cEnabled(XI2C_76)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    sen5x_Init();
  }
  else if (SEN5XDATA != nullptr) {
    switch (function) {
    case FUNC_EVERY_SECOND:
      if (Settings->flag6.sen5x_passive_mode) {
        if (TasmotaGlobal.uptime % SEN5X_PASSIVE_MODE_INTERVAL == 0) {
          SEN5XUpdate();
        }
      }
      else {
        SEN5XUpdate();
      }
      break;
    case FUNC_JSON_APPEND:
      SEN5XShow(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      SEN5XShow(0);
      break;
#endif // USE_WEBSERVER
    }
  }
  return result;
}

#endif // USE_SEN5X
#endif // USE_I2C
