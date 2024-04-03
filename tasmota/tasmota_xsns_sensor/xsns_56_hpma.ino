/*
  xsns_56_hpma.ino - Honeywell HPMA115S0 particle concentration sensor support for Tasmota

  Copyright (C) 2021  David Hunt

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

#ifdef USE_HPMA
/*********************************************************************************************\
 * Honeywell HPMA115S0 particle concentration sensor
 * For background information see
 *   https://sensing.honeywell.com/hpma115s0-xxx-particulate-matter-sensors
 * For protocol specification see
 *   https://sensing.honeywell.com/honeywell-sensing-hpm-series-particle-sensors-datasheet-32322550-e-en.pdf
 *
\*********************************************************************************************/

#define XSNS_56             56

#include <hpma115S0.h>
#include <TasmotaSerial.h>

TasmotaSerial *HpmaSerial;
HPMA115S0 *hpma115S0;

uint8_t hpma_type = 1;
uint8_t hpma_valid = 0;

struct hpmadata {
  unsigned int pm10;
  unsigned int pm2_5;
} hpma_data;

/*********************************************************************************************/

void HpmaSecond(void)                 // Every second
{
  unsigned int pm2_5, pm10;

  /*
   * Sensor sends data every second, so we just read it and update the structure.
   */
  if (hpma115S0->ReadParticleMeasurement(&pm2_5, &pm10)) {
    hpma_data.pm2_5 = pm2_5;
    hpma_data.pm10 = pm10;
    hpma_valid = 1;
  }

}

void HpmaInit(void)
{
  hpma_type = 0;
  if (PinUsed(GPIO_HPMA_RX) && PinUsed(GPIO_HPMA_TX)) {
    HpmaSerial = new TasmotaSerial(Pin(GPIO_HPMA_RX), Pin(GPIO_HPMA_TX), 1);
    hpma115S0 = new HPMA115S0(*HpmaSerial);

    if (HpmaSerial->begin(9600)) {
      if (HpmaSerial->hardwareSerial()) {
        ClaimSerial();
      }
#ifdef ESP32
      AddLog(LOG_LEVEL_DEBUG, PSTR("HPM: Serial UART%d"), HpmaSerial->getUart());
#endif
      hpma_type = 1;
      hpma115S0->Init();
      hpma115S0->StartParticleMeasurement();
    }
  }
}

void HpmaShow(bool json) {
  if (hpma_valid) {
    char types[10];
    strcpy_P(types, PSTR("HPMA"));

    if (json) {
      ResponseAppend_P(PSTR(",\"%s\":{\"PM2.5\":%d,\"PM10\":%d}"), types, hpma_data.pm2_5, hpma_data.pm10);
#ifdef USE_DOMOTICZ
      if (0 == TasmotaGlobal.tele_period) {
        DomoticzSensor(DZ_VOLTAGE, hpma_data.pm2_5);  // PM2.5
        DomoticzSensor(DZ_CURRENT, hpma_data.pm10);   // PM10
      }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_ENVIRONMENTAL_CONCENTRATION, types, "2.5", hpma_data.pm2_5);
      WSContentSend_PD(HTTP_SNS_ENVIRONMENTAL_CONCENTRATION, types, "10", hpma_data.pm10);
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns56(uint32_t function)
{
  bool result = false;

  if (hpma_type) {
    switch (function) {
      case FUNC_INIT:
        HpmaInit();
        break;
      case FUNC_EVERY_SECOND:
        HpmaSecond();
        break;
      case FUNC_COMMAND_SENSOR:
        if (XSNS_56 == XdrvMailbox.index) {
          return true;
        }
        break;
      case FUNC_JSON_APPEND:
        HpmaShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        HpmaShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_HPMA
