/*
  xsns_113_hc8.ino - HC8 CO2 sensor support for Tasmota

  Copyright (C) 2023 Daniel Maier

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

#ifdef USE_HC8
/*********************************************************************************************\
 * HC8 - CO2 sensor
 *
 * Hardware Serial will be selected if GPIO1 = [HC8 Rx]
 */

#define XSNS_113                      113


/*********************************************************************************************\
 * Source: https://spezifisch.codeberg.page/posts/2022-08-23/co2-sensor-reverse-engineering/
 *
\*********************************************************************************************/

#include <TasmotaSerial.h>

TasmotaSerial *Hc8Serial;

uint16_t hc8_last_ppm = 0;
bool Hc8Initialized = false;

/*********************************************************************************************/


uint8_t Hc8CalculateChecksum(uint8_t *array) {
  uint8_t checksum = 0;
  for(int i = 0; i < 15; i++) {
    checksum += array[i];
  }
  return checksum;
}

void Hc8EverySecond(void)
{
  if(!Hc8Initialized)
    return;

  uint8_t hc8_response[16];
  if(Hc8Serial->available() >= 16) {
    while(Hc8Serial->available()) {
      hc8_response[0] = Hc8Serial->read();
      if(hc8_response[0] == 0x42)
        break;
    }

    for(int i = 1; i < 16; i++) {
      hc8_response[i] = Hc8Serial->read();
    }

    if(*(uint16_t*)hc8_response != 0x4d42) {
      return;
    }

    uint8_t checksum = Hc8CalculateChecksum(hc8_response);
    if(hc8_response[15] != checksum) {
      return;
    }

    hc8_last_ppm = (hc8_response[6]<<8) | hc8_response[7];
  }
}

void Hc8Init(void)
{
  if (PinUsed(GPIO_HC8_RXD)) {
    Hc8Serial = new TasmotaSerial(Pin(GPIO_HC8_RXD), -1, 1);
    if (Hc8Serial->begin(9600)) {
      if (Hc8Serial->hardwareSerial()) { ClaimSerial(); }
      Hc8Initialized = true;
    }
  }
}

void Hc8Show(bool json)
{
  char types[4] = "HC8";
  char model[4] = "HC8";

  if (json) {
    ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_MODEL "\":\"%s\",\"" D_JSON_CO2 "\":\"%d\"}"),
        types, model, hc8_last_ppm);
#ifdef USE_DOMOTICZ
    if (0 == TasmotaGlobal.tele_period) {
      DomoticzSensor(DZ_AIRQUALITY, hc8_last_ppm);
    }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_CO2, types, hc8_last_ppm);
#endif  // USE_WEBSERVER
  }
}


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns113(uint32_t function)
{
  bool result = false;
  switch (function) {
    case FUNC_INIT:
      Hc8Init();
      break;
    case FUNC_EVERY_SECOND:
      Hc8EverySecond();
      break;
    case FUNC_COMMAND_SENSOR:
      if (XSNS_113 == XdrvMailbox.index) {
        result = true;
      }
      break;
    case FUNC_JSON_APPEND:
      Hc8Show(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      Hc8Show(0);
      break;
#endif  // USE_WEBSERVER
  }
  return result;
}

#endif  // USE_HC8
