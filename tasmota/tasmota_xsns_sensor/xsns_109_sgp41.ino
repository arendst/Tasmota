/*
  xsns_109_sgp41.ino - SGP41 VOC and NOx sensor support for Tasmota

  Copyright (C) 2023  Andrew Klaus

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
#ifdef USE_SGP41
/*********************************************************************************************\
 * SGP41 - Gas (TVOC - Total Volatile Organic Compounds) and NOx
 *
 * Source: Gerhard Mutz and  Industries
 * Adaption for TASMOTA: Andrew Klaus
 *
 * I2C Address: 0x59
\*********************************************************************************************/

#define XSNS_109            109
#define XI2C_82             82  // See I2CDEVICES.md

#define SGP41_ADDRESS       0x59

#include "SensirionI2CSgp41.h"

extern "C" {
#include "sensirion_gas_index_algorithm.h"
};

SensirionI2CSgp41 sgp41;

bool sgp41_init = false;
bool sgp41_ready = false;
uint16_t srawVoc;
uint16_t srawNox;
int32_t voc_index_sgp41;
int32_t nox_index_sgp41;

uint16_t conditioning_s = 1;

GasIndexAlgorithmParams voc_algorithm_params;
GasIndexAlgorithmParams nox_algorithm_params;

/********************************************************************************************/
void sgp41_print_serial(uint16_t serialNumber[], uint8_t serialNumberSize) {
  Serial.print("SerialNumber:");
  Serial.print("0x");
  for (size_t i = 0; i < serialNumberSize; i++) {
      uint16_t value = serialNumber[i];
      Serial.print(value < 4096 ? "0" : "");
      Serial.print(value < 256 ? "0" : "");
      Serial.print(value < 16 ? "0" : "");
      Serial.print(value, HEX);
  }
  Serial.println();
}

void sgp41_Init(void)
{
  if (!I2cSetDevice(SGP41_ADDRESS)) { return; }

  uint16_t testResult;
  uint8_t serialNumberSize = 3;
  uint16_t serialNumber[serialNumberSize];
  uint16_t error;
  char errorMessage[256];

  sgp41.begin(Wire);

  error = sgp41.getSerialNumber(serialNumber, serialNumberSize);

  if (error) {
    Serial.println("Failed to get s/n from sgp41 driver");
    return;
  }

  sgp41_init = true;
  I2cSetActiveFound(SGP41_ADDRESS, "SGP41");
  sgp41_print_serial(serialNumber, serialNumberSize);

  error = sgp41.executeSelfTest(testResult);
  if (error) {
      Serial.print("Error trying to execute executeSelfTest(): ");
      errorToString(error, errorMessage, 256);
      Serial.println(errorMessage);
  } else if (testResult != 0xD400) {
      Serial.print("executeSelfTest failed with error: ");
      Serial.println(testResult);
  }

  GasIndexAlgorithm_init(&nox_algorithm_params, GasIndexAlgorithm_ALGORITHM_TYPE_NOX);
  GasIndexAlgorithm_init(&voc_algorithm_params, GasIndexAlgorithm_ALGORITHM_TYPE_VOC);
}

void Sgp41Update(void)
{
  sgp41_ready = true;

  char errorMessage[256];
  uint16_t defaultRh = 0x8000;
  uint16_t defaultT = 0x6666;
  uint16_t error;

  if (!(TasmotaGlobal.uptime%10)) {
    // store settings every 10 seconds
    // Conversion from https://sensirion.com/media/documents/5FE8673C/61E96F50/Sensirion_Gas_Sensors_Datasheet_SGP41.pdf
    uint16_t rhticks = (uint16_t)((TasmotaGlobal.humidity * 65535) / 100 + 0.5);
    uint16_t tempticks = (uint16_t)(((TasmotaGlobal.temperature_celsius + 45) * 65535) / 175);

    if (conditioning_s > 0) {
        // During NOx conditioning (10s) SRAW NOx will remain 0
        error = sgp41.executeConditioning(rhticks, tempticks, srawVoc);
        GasIndexAlgorithm_process(&voc_algorithm_params, srawVoc, &voc_index_sgp41);
        conditioning_s--;
    } else {
        // Read Measurement
      error = sgp41.measureRawSignals(rhticks, tempticks, srawVoc, srawNox);

      if (error) {
          Serial.print("Error trying to execute measureRawSignals(): ");
          errorToString(error, errorMessage, 256);
          Serial.println(errorMessage);
      }

      GasIndexAlgorithm_process(&voc_algorithm_params, srawVoc, &voc_index_sgp41);
      GasIndexAlgorithm_process(&nox_algorithm_params, srawNox, &nox_index_sgp41);
    }

  }
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_SGP41[] PROGMEM =
  "{s}SGP41 TVOC " D_JSON_RAW "{m}%d " "{e}"                // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
  "{s}SGP41 NOx " D_JSON_RAW "{m}%d " "{e}"
  "{s}SGP41 " D_TVOC "{m}%d " "{e}"
  "{s}SGP41 " D_NOX "{m}%d " "{e}";
#endif

void Sgp41Show(bool json)
{
  if (sgp41_ready) {
    if (json) {
      ResponseAppend_P(PSTR(",\"SGP41\":{\"" D_JSON_RAW "\":%d,\"" D_JSON_AIRQUALITY "\":%d"), srawVoc, voc_index_sgp41);
      ResponseJsonEnd();
#ifdef USE_DOMOTICZ
      if (0 == TasmotaGlobal.tele_period) DomoticzSensor(DZ_AIRQUALITY, srawVoc);
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_SGP41, srawVoc, srawNox, voc_index_sgp41, nox_index_sgp41);
#endif
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns109(uint32_t function)
{
  if (!I2cEnabled(XI2C_82)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    sgp41_Init();
  }
  else if (sgp41_init) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        Sgp41Update();
        break;
      case FUNC_JSON_APPEND:
        Sgp41Show(1);
        break;
  #ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Sgp41Show(0);
        break;
  #endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_sgp41
#endif  // USE_I2C
