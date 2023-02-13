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
 * SEN5X - Gas (VOC - Volatile Organic Compounds / NOx - Nitrous Oxides) and Particulates (PPM)
 *
 * Source: Sensirion SEN5X Driver + Example, and Tasmota Driver 98 by Jean-Pierre Deschamps
 * Adaption for TASMOTA: Tyeth Gundry
 *
 * I2C Address: 0x59
\*********************************************************************************************/

#define XSNS_103 103
#define XI2C_76 76 // See I2CDEVICES.md

#define SEN5X_ADDRESS 0x69

#include <SensirionI2CSen5x.h>
#include <Wire.h>
SensirionI2CSen5x *sen5x = nullptr;

struct SEN5XDATA_s {
  bool sen5x_ready;
  float abshum;
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

void sen5x_Init(void)
{
  if(!TasmotaGlobal.i2c_enabled){
    DEBUG_SENSOR_LOG(PSTR("I2C Not enabled, so not loading SEN5X driver."));
    return;
  }
  int usingI2cBus = 0;
#ifdef ESP32
  if (!I2cSetDevice(SEN5X_ADDRESS, 0))
  {
    DEBUG_SENSOR_LOG(PSTR("Sensirion SEN5X not found, i2c bus 0"));
    if (TasmotaGlobal.i2c_enabled_2 ){

      if(!I2cSetDevice(SEN5X_ADDRESS, 1)){
        DEBUG_SENSOR_LOG(PSTR("Sensirion SEN5X not found, i2c bus 1"));
        return;
      }
      usingI2cBus = 1;
    }
    else {
      return;
    }
  }
#else
  if (!I2cSetDevice(SEN5X_ADDRESS))
  {
    DEBUG_SENSOR_LOG(PSTR("Sensirion SEN5X not found, i2c bus 0"));
    return;
  }
#endif
  if (SEN5XDATA == nullptr) 
    SEN5XDATA = (SEN5XDATA_s *)calloc(1, sizeof(struct SEN5XDATA_s));
  SEN5XDATA->sen5x_ready = false;
  if(sen5x == nullptr) sen5x = new SensirionI2CSen5x();
  if(usingI2cBus==1){
#ifdef ESP32
    sen5x->begin(Wire1);
#else
    sen5x->begin(Wire);
#endif
  } 
  else {
    sen5x->begin(Wire);
  }  
  int error_stop = sen5x->deviceReset();
  if (error_stop != 0)
  {
    DEBUG_SENSOR_LOG(PSTR("Sensirion SEN5X failed to reset device (I2C Bus %d)"), usingI2cBus);
    return;
  }
  // Wait 1 second for sensors to start recording + 100ms for reset command
  delay(1100);
  int error_start = sen5x->startMeasurement();
  if (error_start != 0)
  {
    DEBUG_SENSOR_LOG(PSTR("Sensirion SEN5X failed to start measurement (I2C Bus %d)"), usingI2cBus);
    return;
  }
  SEN5XDATA->sen5x_ready = true;
  I2cSetActiveFound(SEN5X_ADDRESS, "SEN5X", usingI2cBus);
  DEBUG_SENSOR_LOG(PSTR("Sensirion SEN5X found, i2c bus %d"), usingI2cBus);
}

// #define POW_FUNC pow
#define POW_FUNC FastPrecisePow

float sen5x_AbsoluteHumidity(float temperature, float humidity)
{
  // taken from https://carnotcycle.wordpress.com/2012/08/04/how-to-convert-relative-humidity-to-absolute-humidity/
  // precision is about 0.1°C in range -30 to 35°C
  // August-Roche-Magnus 	6.1094 exp(17.625 x T)/(T + 243.04)
  // Buck (1981) 		6.1121 exp(17.502 x T)/(T + 240.97)
  // reference https://www.eas.ualberta.ca/jdwilson/EAS372_13/Vomel_CIRES_satvpformulae.html
  float temp = NAN;
  const float mw = 18.01534f;  // molar mass of water g/mol
  const float r = 8.31447215f; // Universal gas constant J/mol/K

  if (isnan(temperature) || isnan(humidity))
  {
    return NAN;
  }

  temp = POW_FUNC(2.718281828f, (17.67f * temperature) / (temperature + 243.5f));

  // return (6.112 * temp * humidity * 2.1674) / (273.15 + temperature); 	//simplified version
  return (6.112f * temp * humidity * mw) / ((273.15f + temperature) * r); // long version
}

#define SAVE_PERIOD 30

void SEN5XUpdate(void) // Perform every second to ensure proper operation of the baseline compensation algorithm
{
  uint16_t error;
  char errorMessage[256];
  DEBUG_SENSOR_LOG(PSTR("Running readMeasuredValues for SEN5X..."));

  error = sen5x->readMeasuredValues(
      SEN5XDATA->massConcentrationPm1p0, SEN5XDATA->massConcentrationPm2p5, SEN5XDATA->massConcentrationPm4p0,
      SEN5XDATA->massConcentrationPm10p0, SEN5XDATA->ambientHumidity, SEN5XDATA->ambientTemperature, SEN5XDATA->vocIndex,
      SEN5XDATA->noxIndex);

  if (error)
  {
    AddLog(LOG_LEVEL_DEBUG, PSTR("Failed to retrieve SEN5X readings."));
    #ifdef DEBUG_TASMOTA_SENSOR
    DEBUG_SENSOR_LOG(PSTR("Error trying to execute readMeasuredValues(): \n"));
    errorToString(error, errorMessage, 256);
    DEBUG_SENSOR_LOG(errorMessage);
    #endif
  }
  else
  {
#ifdef DEBUG_TASMOTA_SENSOR
    DEBUG_SENSOR_LOG(PSTR("SEN5x readings:-"));
    DEBUG_SENSOR_LOG(PSTR("MassConcentrationPm1p0: %f\n"), SEN5XDATA->massConcentrationPm1p0);
    DEBUG_SENSOR_LOG(PSTR("MassConcentrationPm2p5: %f\n"), SEN5XDATA->massConcentrationPm2p5);
    DEBUG_SENSOR_LOG(PSTR("MassConcentrationPm4p0: %f\n"), SEN5XDATA->massConcentrationPm4p0);
    DEBUG_SENSOR_LOG(PSTR("MassConcentrationPm10p0: %f\n"), SEN5XDATA->massConcentrationPm10p0);
    if (isnan(SEN5XDATA->ambientHumidity))
    {
      DEBUG_SENSOR_LOG(PSTR("AmbientHumidity: n/a\n"));
    }
    else
    {
      DEBUG_SENSOR_LOG(PSTR("AmbientHumidity: %f\n"), SEN5XDATA->ambientHumidity);
    }

    if (isnan(SEN5XDATA->ambientTemperature))
    {
    DEBUG_SENSOR_LOG(PSTR("AmbientTemperature: n/a\n"));
    }
    else
    {
    DEBUG_SENSOR_LOG(PSTR("AmbientTemperature: %f\n"), SEN5XDATA->ambientTemperature);
    }
    
    if (isnan(SEN5XDATA->vocIndex))
    {
      DEBUG_SENSOR_LOG(PSTR("VocIndex: n/a\n"));
    }
    else
    {
    DEBUG_SENSOR_LOG(PSTR("VocIndex: %f\n"), SEN5XDATA->vocIndex);
    }
    
    if (isnan(SEN5XDATA->noxIndex))
    {
      DEBUG_SENSOR_LOG(PSTR("NoxIndex: n/a\n"));
    }
    else
    {
      DEBUG_SENSOR_LOG(PSTR("NoxIndex: %f\n"), SEN5XDATA->noxIndex);
    }
#endif
  }
  if (!isnan(SEN5XDATA->ambientTemperature) && SEN5XDATA->ambientHumidity > 0) {
    SEN5XDATA->abshum = sen5x_AbsoluteHumidity(SEN5XDATA->ambientTemperature, SEN5XDATA->ambientHumidity);
    DEBUG_SENSOR_LOG(PSTR("AbsoluteHumidity: %f\n"), SEN5XDATA->abshum);
  }
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_SEN5X_UNITS[] PROGMEM = "{s}SEN5X %s{m}%.*f %s{e}";
const char HTTP_SNS_SEN5X_UNITLESS[] PROGMEM = "{s}SEN5X %s{m}%.*f{e}";
// {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
const char HTTP_SNS_AHUMSEN5X[] PROGMEM = "{s}SEN5X Abs Humidity{m}%s g/m³{e}";
#endif

#define D_JSON_AHUM "aHumidity"

void SEN5XShow(bool json)
{
  if (SEN5XDATA->sen5x_ready)
  {
    char sen5x_abs_hum[33];
    bool ahum_available = !isnan(SEN5XDATA->ambientTemperature) && (SEN5XDATA->ambientHumidity > 0);
    if (ahum_available)
    {
      // has humidity + temperature
      dtostrfd(SEN5XDATA->abshum, 4, sen5x_abs_hum);
    }
    if (json)
    {
      ResponseAppend_P(PSTR(",\"SEN5X\":{"));
      ResponseAppend_P(PSTR("\"PM1\":%.1f,"), SEN5XDATA->massConcentrationPm1p0);
      ResponseAppend_P(PSTR("\"PM2.5\":%.1f,"), SEN5XDATA->massConcentrationPm2p5);
      ResponseAppend_P(PSTR("\"PM4\":%.1f,"), SEN5XDATA->massConcentrationPm4p0);
      ResponseAppend_P(PSTR("\"PM10\":%.1f,"), SEN5XDATA->massConcentrationPm10p0);
      if (!isnan(SEN5XDATA->noxIndex))
        ResponseAppend_P(PSTR("\"NOx\":%.0f,"), SEN5XDATA->noxIndex);
      if (!isnan(SEN5XDATA->vocIndex))
        ResponseAppend_P(PSTR("\"VOC\":%.0f,"), SEN5XDATA->vocIndex);
      if (!isnan(SEN5XDATA->ambientTemperature))
        ResponseAppendTHD(SEN5XDATA->ambientTemperature, SEN5XDATA->ambientHumidity);
      if (ahum_available)
        ResponseAppend_P(PSTR(",\"" D_JSON_AHUM "\":%s"), sen5x_abs_hum);
      ResponseJsonEnd();
    }

#ifdef USE_WEBSERVER

    WSContentSend_PD(HTTP_SNS_SEN5X_UNITS, "PM1", 1, SEN5XDATA->massConcentrationPm1p0, "μg/m³");
    WSContentSend_PD(HTTP_SNS_SEN5X_UNITS, "PM2.5", 1, SEN5XDATA->massConcentrationPm2p5, "μg/m³");
    WSContentSend_PD(HTTP_SNS_SEN5X_UNITS, "PM4", 1, SEN5XDATA->massConcentrationPm4p0, "μg/m³");
    WSContentSend_PD(HTTP_SNS_SEN5X_UNITS, "PM10", 1, SEN5XDATA->massConcentrationPm10p0, "μg/m³");
    if (!isnan(SEN5XDATA->noxIndex))
      WSContentSend_PD(HTTP_SNS_SEN5X_UNITLESS, "NOx", 0, SEN5XDATA->noxIndex);
    if (!isnan(SEN5XDATA->vocIndex))
      WSContentSend_PD(HTTP_SNS_SEN5X_UNITLESS, "VOC", 0, SEN5XDATA->vocIndex);
    if (!isnan(SEN5XDATA->ambientTemperature))
      WSContentSend_PD(HTTP_SNS_SEN5X_UNITS, "Temperature", 2, SEN5XDATA->ambientTemperature, "°C");
    if (!isnan(SEN5XDATA->ambientHumidity))
      WSContentSend_PD(HTTP_SNS_SEN5X_UNITS, "Humidity", 2, SEN5XDATA->ambientHumidity, "%RH");
    if (ahum_available)
      WSContentSend_PD(HTTP_SNS_AHUMSEN5X, sen5x_abs_hum);

#endif
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns103(uint32_t function)
{
  if (!I2cEnabled(XI2C_76))
  {
    return false;
  }

  bool result = false;

  if (FUNC_INIT == function)
  {
    sen5x_Init();
  }
  else if (SEN5XDATA != nullptr)
  {
    switch (function)
    {
    case FUNC_EVERY_SECOND:
      SEN5XUpdate();
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
