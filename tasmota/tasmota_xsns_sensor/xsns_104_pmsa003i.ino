/*
  xsns_104_pmsa003i.ino - PMSA003I air quality sensor support for Tasmota

  Copyright (C) 2023  Jean-Pierre Deschamps

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
#ifdef USE_PMSA003I
/*********************************************************************************************\
 * PMSA003I - PM2.5 Air Quality Sensor with I2C Interface
 *
 * Source: Adafruit Industries
 * Adaption for TASMOTA: Jean-Pierre Deschamps
 *
 * I2C Address: 0x12
\*********************************************************************************************/

#define XSNS_104            104
#define XI2C_78             78  // See I2CDEVICES.md

#define PMSA003I_ADDRESS    0x12

#ifndef PMSA003I_WARMUP_DELAY
#define PMSA003I_WARMUP_DELAY 30 // Ignore PMSA003I readings for XX seconds after start
#endif

#include "Adafruit_PM25AQI.h"

struct PMSA003I {
  bool type = false;
  bool ready = false;
  uint8_t warmup_counter;  // count for warmup
  PM25_AQI_Data data;
  Adafruit_PM25AQI aqi = Adafruit_PM25AQI();
} Pmsa003i;


/********************************************************************************************/

void pmsa003i_Init(void)
{
  if (!I2cSetDevice(PMSA003I_ADDRESS)) {
    // AddLog(LOG_LEVEL_DEBUG, PSTR("PMS: " D_JSON_I2CSCAN_NO_DEVICES_FOUND));
    return;
  }

  if (Pmsa003i.aqi.begin_I2C()) {
    Pmsa003i.type = true;
    Pmsa003i.warmup_counter = PMSA003I_WARMUP_DELAY;
    I2cSetActiveFound(PMSA003I_ADDRESS, "PMSA003I");
//  } else {
//    AddLog(LOG_LEVEL_DEBUG, PSTR("PMS: " "Begin_I2C failed"));
  }
}


void Pmsa003iUpdate(void)
{
  if (Pmsa003i.warmup_counter > 0) {
    Pmsa003i.warmup_counter--;
    return;
  }

  Pmsa003i.ready = false;
  PM25_AQI_Data data;
  if (! Pmsa003i.aqi.read(&data)) { // Could not read from AQI
    return;
  }
  Pmsa003i.data = data;
  Pmsa003i.ready = true;
}

void Pmsa003iShow(bool json) {
  if (Pmsa003i.ready) {
    char types[10];
    strcpy_P(types, PSTR("PMSA003I"));

    if (json) {
      ResponseAppend_P(PSTR(",\"%s\":{\"CF1\":%d,\"CF2.5\":%d,\"CF10\":%d,\"PM1\":%d,\"PM2.5\":%d,\"PM10\":%d,\"PB0.3\":%d,\"PB0.5\":%d,\"PB1\":%d,\"PB2.5\":%d,\"PB5\":%d,\"PB10\":%d}"),
        types,
        Pmsa003i.data.pm10_standard, Pmsa003i.data.pm25_standard, Pmsa003i.data.pm100_standard,
        Pmsa003i.data.pm10_env, Pmsa003i.data.pm25_env, Pmsa003i.data.pm100_env,
        Pmsa003i.data.particles_03um, Pmsa003i.data.particles_05um, Pmsa003i.data.particles_10um, Pmsa003i.data.particles_25um, Pmsa003i.data.particles_50um, Pmsa003i.data.particles_100um);
#ifdef USE_DOMOTICZ
      if (0 == TasmotaGlobal.tele_period) {
        DomoticzSensor(DZ_COUNT, Pmsa003i.data.pm10_env);     // PM1
        DomoticzSensor(DZ_VOLTAGE, Pmsa003i.data.pm25_env);   // PM2.5
        DomoticzSensor(DZ_CURRENT, Pmsa003i.data.pm100_env);  // PM10
      }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
//      WSContentSend_PD(HTTP_SNS_STANDARD_CONCENTRATION, types, "1", Pmsa003i.data.pm10_standard);
//      WSContentSend_PD(HTTP_SNS_STANDARD_CONCENTRATION, types, "2.5", Pmsa003i.data.pm25_standard);
//      WSContentSend_PD(HTTP_SNS_STANDARD_CONCENTRATION, types, "10", Pmsa003i.data.pm100_standard);
      WSContentSend_PD(HTTP_SNS_ENVIRONMENTAL_CONCENTRATION, types, "1", Pmsa003i.data.pm10_env);
      WSContentSend_PD(HTTP_SNS_ENVIRONMENTAL_CONCENTRATION, types, "2.5", Pmsa003i.data.pm25_env);
      WSContentSend_PD(HTTP_SNS_ENVIRONMENTAL_CONCENTRATION, types, "10", Pmsa003i.data.pm100_env);
      WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, types, "0.3", Pmsa003i.data.particles_03um);
      WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, types, "0.5", Pmsa003i.data.particles_05um);
      WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, types, "1", Pmsa003i.data.particles_10um);
      WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, types, "2.5", Pmsa003i.data.particles_25um);
      WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, types, "5", Pmsa003i.data.particles_50um);
      WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, types, "10", Pmsa003i.data.particles_100um);
#endif
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns104(uint32_t function)
{
  if (!I2cEnabled(XI2C_78)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    pmsa003i_Init();
  }
  else if (Pmsa003i.type) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        Pmsa003iUpdate();
        break;
      case FUNC_JSON_APPEND:
        Pmsa003iShow(1);
        break;
  #ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Pmsa003iShow(0);
        break;
  #endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_PMSA003I
#endif  // USE_I2C
