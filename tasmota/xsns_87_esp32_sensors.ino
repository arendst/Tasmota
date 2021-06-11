/*
  xsns_87_esp32_sensors.ino - ESP32 Temperature and Hall Effect sensor for Tasmota

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
// Below test to solve ESP32-C3 compilations (20210420)
#if CONFIG_IDF_TARGET_ESP32
/*********************************************************************************************\
 * ESP32 CPU Temperature and optional Hall Effect sensor
 *
 * ESP32 internal Hall Effect sensor connected to both GPIO36 and GPIO39
 * To enable set
 * GPIO36 as HallEffect 1
 * GPIO39 as HallEffect 2
\*********************************************************************************************/

#define XSNS_87                  87

#if CONFIG_IDF_TARGET_ESP32

#define HALLEFFECT_SAMPLE_COUNT  32   // 32 takes about 12 mS at 80MHz CPU frequency

struct {
  bool present = false;
} HEData;

void Esp32SensorInit(void) {
  if (PinUsed(GPIO_HALLEFFECT) && PinUsed(GPIO_HALLEFFECT, 1)) {
    if (((36 == Pin(GPIO_HALLEFFECT)) && (39 == Pin(GPIO_HALLEFFECT, 1))) ||
        ((39 == Pin(GPIO_HALLEFFECT)) && (36 == Pin(GPIO_HALLEFFECT, 1)))) {
      HEData.present = true;
      hallRead();
    }
  }
}

#endif  // CONFIG_IDF_TARGET_ESP32

void Esp32SensorShow(bool json) {
  float t = CpuTemperature();

#if CONFIG_IDF_TARGET_ESP32
  int value = 0;
  if (HEData.present) {
    for (uint32_t i = 0; i < HALLEFFECT_SAMPLE_COUNT; i++) {
      value += hallRead();
    }
    value /= HALLEFFECT_SAMPLE_COUNT;
  }
#endif  // CONFIG_IDF_TARGET_ESP32

  if (json) {
    bool temperature_present = (ResponseContains_P(PSTR(D_JSON_TEMPERATURE)));
    ResponseAppend_P(PSTR(",\"ESP32\":{\"" D_JSON_TEMPERATURE "\":%*_f"), Settings->flag2.temperature_resolution, &t);

#if CONFIG_IDF_TARGET_ESP32
    if (HEData.present) {
      ResponseAppend_P(PSTR(",\"" D_JSON_HALLEFFECT "\":%d"), value);
    }
#endif  // CONFIG_IDF_TARGET_ESP32

    ResponseJsonEnd();
#ifdef USE_DOMOTICZ
    if (0 == TasmotaGlobal.tele_period) {
      if (!temperature_present) {  // Only send if no other sensor already did
        DomoticzFloatSensor(DZ_TEMP, t);
      }

#if CONFIG_IDF_TARGET_ESP32
      if (HEData.present) {
        DomoticzSensor(DZ_COUNT, value);
      }
#endif  // CONFIG_IDF_TARGET_ESP32

    }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_Temp("ESP32", t);

#if CONFIG_IDF_TARGET_ESP32
    if (HEData.present) {
      WSContentSend_P(HTTP_SNS_HALL_EFFECT, "ESP32", value);
    }
#endif  // CONFIG_IDF_TARGET_ESP32

#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns87(uint8_t function) {
  bool result = false;

  switch (function) {
    case FUNC_JSON_APPEND:
      Esp32SensorShow(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      Esp32SensorShow(0);
      break;
#endif  // USE_WEBSERVER
    case FUNC_INIT:
      Esp32SensorInit();
      break;
  }
  return result;
}

#endif  // CONFIG_IDF_TARGET_ESP32
#endif  // ESP32
