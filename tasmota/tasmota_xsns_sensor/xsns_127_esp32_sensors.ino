/*
  xsns_127_esp32_sensors.ino - ESP32 Temperature sensor for Tasmota

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
#ifdef USE_ESP32_SENSORS
/*********************************************************************************************\
 * ESP32 CPU Temperature sensor
 *
 * To allow for not updating the global temperature by the ESP32 temperature sensor this
 *   driver needs to be the highest numbered driver (currently 127)
\*********************************************************************************************/

#define XSNS_127                 127

void Esp32SensorShow(bool json) {
  bool json_end = false;

  if (Settings->flag6.use_esp32_temperature) {  // SetOption146 - (ESP32) Show ESP32 internal temperature sensor
    float c = CpuTemperature();  // in Celsius
    float t = ConvertTempToFahrenheit(c);

    if (json) {
      if (!ResponseContains_P(PSTR(D_JSON_TEMPERATURE))) {
        UpdateGlobalTemperature(c);
      }
      ResponseAppend_P(PSTR(",\"ESP32\":{\"" D_JSON_TEMPERATURE "\":%*_f"), Settings->flag2.temperature_resolution, &t);
      json_end = true;

#ifdef USE_DOMOTICZ
//    Instead of below code use a rule like 'on tele-esp32#temperature do dzsend1 9988,%value% endon'
//      where 9988 is the domoticz sensor Idx
//    if (0 == TasmotaGlobal.tele_period) {
//      if (!ResponseContains_P(PSTR(D_JSON_TEMPERATURE))) {  // Only send if no other sensor already did
//        DomoticzFloatSensor(DZ_TEMP, t);
//      }
//    }
#endif  // USE_DOMOTICZ

#ifdef USE_WEBSERVER
    } else {
      WSContentSend_Temp("ESP32", t);
#endif  // USE_WEBSERVER
    }
  }

  if (json_end) {
    ResponseJsonEnd();
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns127(uint32_t function) {
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
  }
  return result;
}
#endif  // USE_ESP32_SENSORS
#endif  // ESP32
