/*
  xsns_interface.ino - Sensor interface support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends inspired by ESPEasy

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

boolean (* const xsns_func_ptr[])(byte) PROGMEM = {  // Sensor Function Pointers for simple implementation of sensors
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
 *
 * FUNC_INIT
 * FUNC_PREP_BEFORE_TELEPERIOD
 * FUNC_SAVE_BEFORE_RESTART
 * FUNC_JSON_APPEND
 * FUNC_WEB_APPEND
 * return FUNC_COMMAND
 * FUNC_EVERY_SECOND
 * FUNC_EVERY_50_MSECOND
\*********************************************************************************************/

boolean XsnsCall(byte Function)
{
  boolean result = false;

  for (byte x = 0; x < xsns_present; x++) {
    result = xsns_func_ptr[x](Function);
    if (result) {
      break;
    }
  }

  return result;
}
