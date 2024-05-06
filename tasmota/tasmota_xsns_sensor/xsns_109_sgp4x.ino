/*
  xsns_109_sgp4x.ino - SGP4X VOC and NOx sensor support for Tasmota

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
#ifdef USE_SGP4X
/*********************************************************************************************\
 * SGP4x - Gas (TVOC - Total Volatile Organic Compounds) and NOx
 *
 * Source: Sensirion Driver, with mods by Andrew Klaus
 * Adaption for TASMOTA: Andrew Klaus
 *
 * I2C Address: 0x59
\*********************************************************************************************/

#define XSNS_109            109
#define XI2C_82             82  // See I2CDEVICES.md

#define SGP4X_ADDRESS       0x59

#include "SensirionI2CSgp4x.h"

extern "C" {
#include "sensirion_gas_index_algorithm.h"
};

enum SGP4X_Type {
  TYPE_SGP41,
  TYPE_SGP40,
};

enum SGP4X_State {
  STATE_SGP4X_START,
  STATE_SGP4X_SELFTEST_SENT,
  STATE_SGP4X_SELFTEST_DONE,
  STATE_SGP4X_COND_SENT,
  STATE_SGP4X_COND_DONE,
  STATE_SGP4X_NORMAL,
  STATE_SGP4X_FAIL,
};
SensirionI2CSgp4x sgp4x;
SGP4X_State sgp4x_state = STATE_SGP4X_START;
SGP4X_Type  sgp4x_type = TYPE_SGP41;

bool sgp4x_init = false;
bool sgp4x_read_pend = false;

uint16_t srawVoc;
uint16_t srawNox;
int32_t voc_index_sgp4x;
int32_t nox_index_sgp4x;

uint16_t selftest_wait = 2;    // 2x 250ms cycles (500ms) delay for self-testing
uint16_t conditioning_s = 10; // 10 second delay for startup

GasIndexAlgorithmParams voc_algorithm_params;
GasIndexAlgorithmParams nox_algorithm_params;

/********************************************************************************************/

void sgp4x_Init(void)
{
  if (!I2cSetDevice(SGP4X_ADDRESS)) { return; }

  uint8_t serialNumberSize = 3;
  uint16_t serialNumber[serialNumberSize];
  uint16_t error;

  sgp4x.begin(Wire);
  error = sgp4x.getSerialNumber(serialNumber, serialNumberSize);

  if (error) {
    AddLog(LOG_LEVEL_INFO, PSTR("SGP4X error: error during getserialnumber"));
    Sgp4xHandleError(error);
    return;
  } else {
    AddLog(LOG_LEVEL_INFO, PSTR("SGP4X serial nr 0x%X 0x%X 0x%X") ,serialNumber[0], serialNumber[1], serialNumber[2]);
  }

  // The command is still active for both SGP40 and SGP41 using 0x202f to distinguish them.
  uint16_t features;
  error = sgp4x.getFeaturesValue(features);
  if (error) {
    AddLog(LOG_LEVEL_INFO, PSTR("SGP4X error: error during getFeaturesValue"));
    Sgp4xHandleError(error);
    return;
  }
  AddLog(LOG_LEVEL_INFO, PSTR("SGP4X features: 0x%X") ,features);

  // Undocumented, but confirmed by Sensirion
  // SGP40 is 0x3240, SGP41 is 0x0240
  if (features == 0x3240) {
    sgp4x_type = TYPE_SGP40;
    // SGP40 doesn't do conditioning, so skip
    sgp4x_state = STATE_SGP4X_NORMAL;
  }

  I2cSetActiveFound(SGP4X_ADDRESS, "SGP4X");

  sgp4x_init = true;
  error = sgp4x.sendSelfTestCmd();

  if (error) {
    AddLog(LOG_LEVEL_INFO, PSTR("SGP4X error: error during selftest"));
    Sgp4xHandleError(error);
    sgp4x_state = STATE_SGP4X_FAIL;
    return;
  }

  sgp4x_state = STATE_SGP4X_SELFTEST_SENT;

  GasIndexAlgorithm_init(&nox_algorithm_params, GasIndexAlgorithm_ALGORITHM_TYPE_NOX);
  GasIndexAlgorithm_init(&voc_algorithm_params, GasIndexAlgorithm_ALGORITHM_TYPE_VOC);
}

void Sgp4xHandleError(uint16_t error) {
  char errorMessage[256];
  errorToString(error, errorMessage, 256);
  AddLog(LOG_LEVEL_ERROR, PSTR("SGP4X error: %s"), errorMessage);
}

void Sgp4xSendReadCmd(void)
{
  // Check if we're already waiting for a read
  // Or if we need to wait a cycle before initiating a reading
  if (sgp4x_read_pend){return;}

  uint16_t error;
  uint16_t rhticks = (uint16_t)((TasmotaGlobal.humidity * 65535) / 100 + 0.5);
  uint16_t tempticks = (uint16_t)(((TasmotaGlobal.temperature_celsius + 45) * 65535) / 175);

  // Handle self testing
  // Wait 2 cycles (at least 500ms to read selftest value)
  if (sgp4x_state == STATE_SGP4X_SELFTEST_SENT and selftest_wait > 0) {
    selftest_wait--;
    return;
  } else if (sgp4x_state == STATE_SGP4X_SELFTEST_SENT) {
    if (Sgp4xReadSelfTest()){
      sgp4x_state = STATE_SGP4X_FAIL;
      return;
    } else {
      sgp4x_state = STATE_SGP4X_SELFTEST_DONE;
    }
  }

  // Initiate conditioning
  if (sgp4x_state == STATE_SGP4X_SELFTEST_DONE) {
    error = sgp4x.sendConditioningCmd(0x8000, 0x6666);
    sgp4x_state = STATE_SGP4X_COND_SENT;

    if (error) {
      Sgp4xHandleError(error);
    }
    return;
  }

  if (sgp4x_state == STATE_SGP4X_COND_DONE) {
    sgp4x_state = STATE_SGP4X_NORMAL;
  }

  // Normal operation
  if (sgp4x_state == STATE_SGP4X_NORMAL) {
    if (sgp4x_type == TYPE_SGP41) {
      error = sgp4x.sendRawSignalsCmd(rhticks, tempticks);
    } else {
      error = sgp4x.sendRawSignalCmd(rhticks, tempticks);
    }
    if (error) {
      Sgp4xHandleError(error);
    } else {
      sgp4x_read_pend = true;
    }
    return;
  }

  return;
}

bool Sgp4xReadSelfTest() {
  uint16_t testResult;
  uint16_t error;

  error = sgp4x.readSelfTestValue(testResult);
  if (error) {
    Sgp4xHandleError(error);
    return true;
  } else if (testResult != 0xD400) {
    AddLog(LOG_LEVEL_ERROR, PSTR("SGP4X: executeSelfTest failed with error: %s"), testResult);
    return true;
  }

  return false;
}

void Sgp4xUpdate(void)
{
  uint16_t error;

  if (sgp4x_state == STATE_SGP4X_FAIL) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("SGP4X in FAIL state"));
    return;
  }

  // Conditioning - SGP41 NOx needs 10s to warmup
  if (sgp4x_state == STATE_SGP4X_COND_SENT) {
    if (conditioning_s > 0) {
      conditioning_s--;
      return;
    } else {
      sgp4x_state = STATE_SGP4X_COND_DONE;
    }
  }

  if (sgp4x_state == STATE_SGP4X_NORMAL && sgp4x_read_pend) {
    if (sgp4x_type == TYPE_SGP41) {
      error = sgp4x.readRawSignalsValue(srawVoc, srawNox);
    } else {
      error = sgp4x.readRawSignalValue(srawVoc);
    }
    sgp4x_read_pend = false;

    if (!error) {
      // SGP40 and SGP41 support VOC
      GasIndexAlgorithm_process(&voc_algorithm_params, srawVoc, &voc_index_sgp4x);

      // SGP41 supports NOx
      if (sgp4x_type == TYPE_SGP41) {
        GasIndexAlgorithm_process(&nox_algorithm_params, srawNox, &nox_index_sgp4x);
      }
    } else {
      Sgp4xHandleError(error);
    }
  }
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_SGP4X_SGP41[] PROGMEM =
  "{s}SGP41 TVOC " D_JSON_RAW "{m}%d " "{e}"                // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
  "{s}SGP41 NOX " D_JSON_RAW "{m}%d " "{e}"
  "{s}SGP41 " D_TVOC "{m}%d " "{e}"
  "{s}SGP41 " D_NOX "{m}%d " "{e}";

const char HTTP_SNS_SGP4X_SGP40[] PROGMEM =
  "{s}SGP40 TVOC " D_JSON_RAW "{m}%d " "{e}"                // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
  "{s}SGP40 " D_TVOC "{m}%d " "{e}";
#endif

void Sgp4xShow(bool json)
{
  if (sgp4x_state == STATE_SGP4X_NORMAL) {
    if (json) {
      if (sgp4x_type == TYPE_SGP41) {
        ResponseAppend_P(PSTR(",\"SGP41\":{\"VOC_" D_JSON_RAW "\":%d,\"NOX_" D_JSON_RAW "\":%d,\"" D_TVOC "\":%d,\"" D_NOX "\":%d"), srawVoc, srawNox, voc_index_sgp4x, nox_index_sgp4x);
      } else {
        ResponseAppend_P(PSTR(",\"SGP40\":{\"VOC_" D_JSON_RAW "\":%d,\"" D_TVOC "\":%d"), srawVoc, voc_index_sgp4x);
      }
      ResponseJsonEnd();
#ifdef USE_DOMOTICZ
      if (0 == TasmotaGlobal.tele_period) {
        DomoticzSensor(DZ_AIRQUALITY, voc_index_sgp4x);
      }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
	      if (sgp4x_type == TYPE_SGP41) {
        WSContentSend_PD(HTTP_SNS_SGP4X_SGP41, srawVoc, srawNox, voc_index_sgp4x, nox_index_sgp4x);
      } else {
        WSContentSend_PD(HTTP_SNS_SGP4X_SGP40, srawVoc, voc_index_sgp4x);
      }
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
    sgp4x_Init();
  }
  else if (sgp4x_init) {
    switch (function) {
      case FUNC_EVERY_250_MSECOND:
        Sgp4xSendReadCmd();
        break;
      case FUNC_EVERY_SECOND:
        Sgp4xUpdate();
        break;
      case FUNC_JSON_APPEND:
        Sgp4xShow(1);
        break;
  #ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Sgp4xShow(0);
        break;
  #endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_sgp4x
#endif  // USE_I2C
