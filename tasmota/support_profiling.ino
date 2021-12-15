/*
  support_profiling.ino - profiling support for Tasmota

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

/*********************************************************************************************\
 * Profiling features
\*********************************************************************************************/

// Moved to my_user_config.h (and user_config_override.h)
//#define USE_PROFILING                            // Enable profiling
//#define PROFILE_THRESHOLD            70          // Minimum duration in milliseconds to start logging
//#define USE_PROFILE_DRIVER                       // Enable driver profiling
//#define USE_PROFILE_FUNCTION                     // Enable driver function profiling

/*
// Moved to tasmota_globals.h fixing compile warnings
#ifndef USE_PROFILING
#undef USE_PROFILE_DRIVER
#undef USE_PROFILE_FUNCTION
#endif

#ifdef USE_PROFILE_DRIVER
#define PROFILE_DRIVER(DRIVER, FUNCTION, START) AddLogDriver(DRIVER, FUNCTION, START)
#else
#define PROFILE_DRIVER(DRIVER, FUNCTION, START)
#endif  // USE_PROFILE_DRIVER

#ifdef USE_PROFILE_FUNCTION
#define PROFILE_FUNCTION(DRIVER, INDEX, FUNCTION, START) AddLogFunction(DRIVER, INDEX, FUNCTION, START)
#else
#define PROFILE_FUNCTION(DRIVER, INDEX, FUNCTION, START)
#endif  // USE_PROFILE_DRIVER
*/
/*********************************************************************************************\
 * Profiling services
\*********************************************************************************************/

#ifdef USE_PROFILING

#ifndef PROFILE_THRESHOLD
#define PROFILE_THRESHOLD            70       // Minimum duration in milliseconds to start logging
#endif

// Below needs to be inline with enum XsnsFunctions
const char kXSnsFunctions[] PROGMEM = "SETTINGS_OVERRIDE|PIN_STATE|MODULE_INIT|PRE_INIT|INIT|"
                                      "LOOP|EVERY_50_MSECOND|EVERY_100_MSECOND|EVERY_200_MSECOND|EVERY_250_MSECOND|EVERY_SECOND|"
                                      "SAVE_SETTINGS|SAVE_AT_MIDNIGHT|SAVE_BEFORE_RESTART|"
                                      "AFTER_TELEPERIOD|JSON_APPEND|WEB_SENSOR|COMMAND|COMMAND_SENSOR|COMMAND_DRIVER|"
                                      "MQTT_SUBSCRIBE|MQTT_INIT|MQTT_DATA|"
                                      "SET_POWER|SET_DEVICE_POWER|SHOW_SENSOR|ANY_KEY|"
                                      "ENERGY_EVERY_SECOND|ENERGY_RESET|"
                                      "RULES_PROCESS|TELEPERIOD_RULES_PROCESS|SERIAL|FREE_MEM|BUTTON_PRESSED|"
                                      "WEB_ADD_BUTTON|WEB_ADD_CONSOLE_BUTTON|WEB_ADD_MANAGEMENT_BUTTON|WEB_ADD_MAIN_BUTTON|"
                                      "WEB_ADD_HANDLER|SET_CHANNELS|SET_SCHEME|HOTPLUG_SCAN|"
                                      "DEVICE_GROUP_ITEM";

#ifdef USE_PROFILE_DRIVER
void AddLogDriver(const char *driver, uint8_t function, uint32_t start) {
  uint32_t profile_millis = millis() - start;
  if (profile_millis > PROFILE_THRESHOLD) {
    char stemp1[20];
    AddLog(LOG_LEVEL_DEBUG, PSTR("PRF: *** x%s FUNC_%s (%d ms)"), driver, GetTextIndexed(stemp1, sizeof(stemp1), function, kXSnsFunctions), profile_millis);
  }
}
#endif  // USE_PROFILE_DRIVER

#ifdef USE_PROFILE_FUNCTION
void AddLogFunction(const char *driver, uint8_t index, uint8_t function, uint32_t start) {
  uint32_t profile_millis = millis() - start;
  if (profile_millis > PROFILE_THRESHOLD) {
    char stemp1[20];
    AddLog(LOG_LEVEL_DEBUG, PSTR("PRF: *** x%s_%02d FUNC_%s (%d ms)"), driver, index, GetTextIndexed(stemp1, sizeof(stemp1), function, kXSnsFunctions), profile_millis);
  }
}
#endif  // USE_PROFILE_DRIVER

#endif  // USE_PROFILING
