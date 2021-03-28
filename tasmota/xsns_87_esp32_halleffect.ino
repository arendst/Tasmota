/*
  xsns_87_esp32_halleffect.ino - ESP32 Hall Effect sensor for Tasmota

  Copyright (C) 2021  Theo Arends

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

#ifdef ESP32
#ifdef USE_HALLEFFECT
/*********************************************************************************************\
 * ESP32 internal Hall Effect sensor connected to both GPIO36 and GPIO39
 *
 * To enable set
 * GPIO36 as HallEffect 1
 * GPIO39 as HallEffect 2
\*********************************************************************************************/

#define XSNS_87                  87

#define HALLEFFECT_SAMPLE_COUNT  32   // 32 takes about 12 mS at 80MHz CPU frequency

struct {
  bool present = false;
} HEData;

void HallEffectInit(void) {
  if (PinUsed(GPIO_HALLEFFECT) && PinUsed(GPIO_HALLEFFECT, 1)) {
    if (((36 == Pin(GPIO_HALLEFFECT)) && (39 == Pin(GPIO_HALLEFFECT, 1))) ||
        ((39 == Pin(GPIO_HALLEFFECT)) && (36 == Pin(GPIO_HALLEFFECT, 1)))) {
      HEData.present = true;
      hallRead();
    }
  }
}

#ifdef USE_WEBSERVER
// {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
const char HTTP_SNS_HALL_EFFECT[] PROGMEM = "{s}" D_HALL_EFFECT "{m}%d{e}";
#endif  // USE_WEBSERVER

void HallEffectShow(bool json) {
  int value = 0;
  for (uint32_t i = 0; i < HALLEFFECT_SAMPLE_COUNT; i++) {
    value += hallRead();
  }
  value /= HALLEFFECT_SAMPLE_COUNT;
  if (json) {
    ResponseAppend_P(PSTR(",\"" D_JSON_HALLEFFECT "\":%d"), value);
#ifdef USE_DOMOTICZ
    if (0 == TasmotaGlobal.tele_period) {
      DomoticzSensor(DZ_COUNT, value);
    }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_P(HTTP_SNS_HALL_EFFECT, value);
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns87(uint8_t function) {
  bool result = false;

  if (FUNC_INIT == function) {
    HallEffectInit();
  }
  else if (HEData.present) {
    switch (function) {
      case FUNC_JSON_APPEND:
        HallEffectShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        HallEffectShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_HALLEFFECT
#endif  // ESP32
