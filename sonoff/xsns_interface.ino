/*
  xsns_interface.ino - Sensor interface support for Sonoff-Tasmota

  Copyright (C) 2019  Theo Arends inspired by ESPEasy

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

#ifdef XFUNC_PTR_IN_ROM
bool (* const xsns_func_ptr[])(uint8_t) PROGMEM = {  // Sensor Function Pointers for simple implementation of sensors
#else
bool (* const xsns_func_ptr[])(uint8_t) = {  // Sensor Function Pointers for simple implementation of sensors
#endif

#ifdef XSNS_01
  &Xsns01,
#endif

#ifdef XSNS_02
  &Xsns02,
#endif

#ifdef XSNS_03
  &Xsns03,
#endif

#ifdef XSNS_04
  &Xsns04,
#endif

#ifdef XSNS_05
  &Xsns05,
#endif

#ifdef XSNS_06
  &Xsns06,
#endif

#ifdef XSNS_07
  &Xsns07,
#endif

#ifdef XSNS_08
  &Xsns08,
#endif

#ifdef XSNS_09
  &Xsns09,
#endif

#ifdef XSNS_10
  &Xsns10,
#endif

#ifdef XSNS_11
  &Xsns11,
#endif

#ifdef XSNS_12
  &Xsns12,
#endif

#ifdef XSNS_13
  &Xsns13,
#endif

#ifdef XSNS_14
  &Xsns14,
#endif

#ifdef XSNS_15
  &Xsns15,
#endif

#ifdef XSNS_16
  &Xsns16,
#endif

#ifdef XSNS_17
  &Xsns17,
#endif

#ifdef XSNS_18
  &Xsns18,
#endif

#ifdef XSNS_19
  &Xsns19,
#endif

#ifdef XSNS_20
  &Xsns20,
#endif

#ifdef XSNS_21
  &Xsns21,
#endif

#ifdef XSNS_22
  &Xsns22,
#endif

#ifdef XSNS_23
  &Xsns23,
#endif

#ifdef XSNS_24
  &Xsns24,
#endif

#ifdef XSNS_25
  &Xsns25,
#endif

#ifdef XSNS_26
  &Xsns26,
#endif

#ifdef XSNS_27
  &Xsns27,
#endif

#ifdef XSNS_28
  &Xsns28,
#endif

#ifdef XSNS_29
  &Xsns29,
#endif

#ifdef XSNS_30
  &Xsns30,
#endif

#ifdef XSNS_31
  &Xsns31,
#endif

#ifdef XSNS_32
  &Xsns32,
#endif

#ifdef XSNS_33
  &Xsns33,
#endif

#ifdef XSNS_34
  &Xsns34,
#endif

#ifdef XSNS_35
  &Xsns35,
#endif

#ifdef XSNS_36
  &Xsns36,
#endif

#ifdef XSNS_37
  &Xsns37,
#endif

#ifdef XSNS_38
  &Xsns38,
#endif

#ifdef XSNS_39
  &Xsns39,
#endif

#ifdef XSNS_40
  &Xsns40,
#endif

#ifdef XSNS_41
  &Xsns41,
#endif

#ifdef XSNS_42
  &Xsns42,
#endif

#ifdef XSNS_43
  &Xsns43,
#endif

#ifdef XSNS_44
  &Xsns44,
#endif

#ifdef XSNS_45
  &Xsns45,
#endif

#ifdef XSNS_46
  &Xsns46,
#endif

#ifdef XSNS_47
  &Xsns47,
#endif

#ifdef XSNS_48
  &Xsns48,
#endif

#ifdef XSNS_49
  &Xsns49,
#endif

#ifdef XSNS_50
  &Xsns50,
#endif

// Optional user defined sensors in range 91 - 99

#ifdef XSNS_91
  &Xsns91,
#endif

#ifdef XSNS_92
  &Xsns92,
#endif

#ifdef XSNS_93
  &Xsns93,
#endif

#ifdef XSNS_94
  &Xsns94,
#endif

#ifdef XSNS_95
  &Xsns95,
#endif

#ifdef XSNS_96
  &Xsns96,
#endif

#ifdef XSNS_97
  &Xsns97,
#endif

#ifdef XSNS_98
  &Xsns98,
#endif

#ifdef XSNS_99
  &Xsns99
#endif
};

const uint8_t xsns_present = sizeof(xsns_func_ptr) / sizeof(xsns_func_ptr[0]);  // Number of External Sensors found

/*********************************************************************************************\
 * Function call to all xsns
\*********************************************************************************************/

bool XsnsNextCall(uint8_t Function, uint8_t &xsns_index)
{
  xsns_index++;
  if (xsns_index == xsns_present) { xsns_index = 0; }
#ifdef USE_DEBUG_DRIVER
  while (!XsnsEnabled(xsns_index) && !xsns_index) {  // Perform at least first sensor (counter)
    xsns_index++;
    if (xsns_index == xsns_present) { xsns_index = 0; }
  }
#endif
//  WifiAddDelayWhenDisconnected();
  return xsns_func_ptr[xsns_index](Function);
}

bool XsnsCall(uint8_t Function)
{
  bool result = false;

#ifdef PROFILE_XSNS_EVERY_SECOND
  uint32_t profile_start_millis = millis();
#endif  // PROFILE_XSNS_EVERY_SECOND

  for (uint8_t x = 0; x < xsns_present; x++) {
#ifdef USE_DEBUG_DRIVER
    if (XsnsEnabled(x)) {
#endif

#ifdef PROFILE_XSNS_SENSOR_EVERY_SECOND
      uint32_t profile_start_millis = millis();
#endif  // PROFILE_XSNS_SENSOR_EVERY_SECOND
//      WifiAddDelayWhenDisconnected();
      result = xsns_func_ptr[x](Function);

#ifdef PROFILE_XSNS_SENSOR_EVERY_SECOND
      uint32_t profile_millis = millis() - profile_start_millis;
      if (profile_millis) {
        if (FUNC_EVERY_SECOND == Function) {
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PRF: At %08u XsnsCall %d to Sensor %d took %u mS"), uptime, Function, x, profile_millis);
        }
      }
#endif  // PROFILE_XSNS_SENSOR_EVERY_SECOND

      if (result && ((FUNC_COMMAND == Function) ||
                     (FUNC_COMMAND_SENSOR == Function)
                    )) {
        break;
      }
#ifdef USE_DEBUG_DRIVER
    }
#endif
  }

#ifdef PROFILE_XSNS_EVERY_SECOND
  uint32_t profile_millis = millis() - profile_start_millis;
  if (profile_millis) {
    if (FUNC_EVERY_SECOND == Function) {
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("PRF: At %08u XsnsCall %d took %u mS"), uptime, Function, profile_millis);
    }
  }
#endif  // PROFILE_XSNS_EVERY_SECOND

  return result;
}
