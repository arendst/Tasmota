/*
  xsns_98_sgp40.ino - SGP40 gas and air quality sensor support for Tasmota

  Copyright (C) 2022  Jean-Pierre Deschamps

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
#ifdef USE_SGP40
/*********************************************************************************************\
 * SGP40 - Gas (TVOC - Total Volatile Organic Compounds) and Air Quality (CO2)
 *
 * Source: Gerhard Mutz and Adafruit Industries
 * Adaption for TASMOTA: Jean-Pierre Deschamps
 *
 * I2C Address: 0x59
\*********************************************************************************************/

#define XSNS_98             98
#define XI2C_69             69  // See I2CDEVICES.md

#define SGP40_ADDRESS       0x59

#include "Adafruit_SGP40.h"
Adafruit_SGP40 sgp40;

bool sgp40_type = false;
bool sgp40_ready = false;
float sgp40_abshum;
uint16_t raw_base;
int32_t voc_index;

/********************************************************************************************/

void sgp40_Init(void)
{
  if (!I2cSetDevice(SGP40_ADDRESS)) { return; }

  if (sgp40.begin()) {
    sgp40_type = true;
//    AddLog(LOG_LEVEL_DEBUG, PSTR("SGP: Serialnumber 0x%04X-0x%04X-0x%04X"), sgp40.serialnumber[0], sgp40.serialnumber[1], sgp40.serialnumber[2]);
    I2cSetActiveFound(SGP40_ADDRESS, "SGP40");
  }
}

//#define POW_FUNC pow
#define POW_FUNC FastPrecisePow

float sgp40_AbsoluteHumidity(float temperature, float humidity) {
  //taken from https://carnotcycle.wordpress.com/2012/08/04/how-to-convert-relative-humidity-to-absolute-humidity/
  //precision is about 0.1°C in range -30 to 35°C
  //August-Roche-Magnus 	6.1094 exp(17.625 x T)/(T + 243.04)
  //Buck (1981) 		6.1121 exp(17.502 x T)/(T + 240.97)
  //reference https://www.eas.ualberta.ca/jdwilson/EAS372_13/Vomel_CIRES_satvpformulae.html
  float temp = NAN;
  const float mw = 18.01534f; 	// molar mass of water g/mol
  const float r = 8.31447215f; 	// Universal gas constant J/mol/K

  if (isnan(temperature) || isnan(humidity) ) {
    return NAN;
  }

  temp = POW_FUNC(2.718281828f, (17.67f * temperature) / (temperature + 243.5f));

  //return (6.112 * temp * humidity * 2.1674) / (273.15 + temperature); 	//simplified version
  return (6.112f * temp * humidity * mw) / ((273.15f + temperature) * r); 	//long version
}

#define SAVE_PERIOD 30

void Sgp40Update(void)  // Perform every second to ensure proper operation of the baseline compensation algorithm
{
  sgp40_ready = false;
  if (TasmotaGlobal.global_update && (TasmotaGlobal.humidity > 0) && !isnan(TasmotaGlobal.temperature_celsius)) {
    // abs hum in mg/m3
    sgp40_abshum = sgp40_AbsoluteHumidity(TasmotaGlobal.temperature_celsius, TasmotaGlobal.humidity);
  }
  sgp40_ready = true;

  // these should normally be stored permanently and used for fast restart
  if (!(TasmotaGlobal.uptime%SAVE_PERIOD)) {
    // store settings every N seconds
    raw_base = sgp40.measureRaw(TasmotaGlobal.temperature_celsius, TasmotaGlobal.humidity);
    voc_index = sgp40.measureVocIndex(TasmotaGlobal.temperature_celsius, TasmotaGlobal.humidity);
  }
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_SGP40[] PROGMEM =
  "{s}SGP40 " D_JSON_RAW "{m}%d " "{e}"                // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
  "{s}SGP40 " D_AIR_QUALITY "{m}%d " "{e}";
const char HTTP_SNS_AHUM40[] PROGMEM = "{s}SGP40 Abs Humidity{m}%s g/m3{e}";
#endif

void Sgp40Show(bool json)
{
  if (sgp40_ready) {
    char abs_hum[33];
    bool ahum_available = TasmotaGlobal.global_update && (TasmotaGlobal.humidity > 0) && !isnan(TasmotaGlobal.temperature_celsius);
    if (ahum_available) {
        // has humidity + temperature
        dtostrfd(sgp40_abshum,4,abs_hum);
    }
    if (json) {
      ResponseAppend_P(PSTR(",\"SGP40\":{\"" D_JSON_RAW "\":%d,\"" D_JSON_AIRQUALITY "\":%d"), raw_base, voc_index);
      if (ahum_available) {
        ResponseAppend_P(PSTR(",\"" D_JSON_AHUM "\":%s"),abs_hum);
      }
      ResponseJsonEnd();
#ifdef USE_DOMOTICZ
      if (0 == TasmotaGlobal.tele_period) {
        DomoticzSensor(DZ_AIRQUALITY, voc_index);
      }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_SGP40, raw_base, voc_index);
      if (ahum_available) {
        WSContentSend_PD(HTTP_SNS_AHUM40, abs_hum);
      }
#endif
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns98(uint32_t function)
{
  if (!I2cEnabled(XI2C_69)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    sgp40_Init();
  }
  else if (sgp40_type) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        Sgp40Update();
        break;
      case FUNC_JSON_APPEND:
        Sgp40Show(1);
        break;
  #ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Sgp40Show(0);
        break;
  #endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_SGP40
#endif  // USE_I2C
