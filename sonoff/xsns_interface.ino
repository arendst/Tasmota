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

#ifdef XFUNC_PTR_IN_ROM
boolean (* const xsns_func_ptr[])(byte) PROGMEM = {  // Sensor Function Pointers for simple implementation of sensors
#else
boolean (* const xsns_func_ptr[])(byte) = {  // Sensor Function Pointers for simple implementation of sensors
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
uint8_t xsns_index = 0;

#ifdef XFUNC_PTR_IN_ROM
const uint8_t kXsnsList[] PROGMEM = {
#else
const uint8_t kXsnsList[] = {
#endif

#ifdef XSNS_01
  XSNS_01,
#endif

#ifdef XSNS_02
  XSNS_02,
#endif

#ifdef XSNS_03
  XSNS_03,
#endif

#ifdef XSNS_04
  XSNS_04,
#endif

#ifdef XSNS_05
  XSNS_05,
#endif

#ifdef XSNS_06
  XSNS_06,
#endif

#ifdef XSNS_07
  XSNS_07,
#endif

#ifdef XSNS_08
  XSNS_08,
#endif

#ifdef XSNS_09
  XSNS_09,
#endif

#ifdef XSNS_10
  XSNS_10,
#endif

#ifdef XSNS_11
  XSNS_11,
#endif

#ifdef XSNS_12
  XSNS_12,
#endif

#ifdef XSNS_13
  XSNS_13,
#endif

#ifdef XSNS_14
  XSNS_14,
#endif

#ifdef XSNS_15
  XSNS_15,
#endif

#ifdef XSNS_16
  XSNS_16,
#endif

#ifdef XSNS_17
  XSNS_17,
#endif

#ifdef XSNS_18
  XSNS_18,
#endif

#ifdef XSNS_19
  XSNS_19,
#endif

#ifdef XSNS_20
  XSNS_20,
#endif

#ifdef XSNS_21
  XSNS_21,
#endif

#ifdef XSNS_22
  XSNS_22,
#endif

#ifdef XSNS_23
  XSNS_23,
#endif

#ifdef XSNS_24
  XSNS_24,
#endif

#ifdef XSNS_25
  XSNS_25,
#endif

#ifdef XSNS_26
  XSNS_26,
#endif

#ifdef XSNS_27
  XSNS_27,
#endif

#ifdef XSNS_28
  XSNS_28,
#endif

#ifdef XSNS_29
  XSNS_29,
#endif

#ifdef XSNS_30
  XSNS_30,
#endif

#ifdef XSNS_31
  XSNS_31,
#endif

#ifdef XSNS_32
  XSNS_32,
#endif

#ifdef XSNS_33
  XSNS_33,
#endif

#ifdef XSNS_34
  XSNS_34,
#endif

#ifdef XSNS_35
  XSNS_35,
#endif

#ifdef XSNS_36
  XSNS_36,
#endif

#ifdef XSNS_37
  XSNS_37,
#endif

#ifdef XSNS_38
  XSNS_38,
#endif

#ifdef XSNS_39
  XSNS_39,
#endif

#ifdef XSNS_40
  XSNS_40,
#endif

#ifdef XSNS_41
  XSNS_41,
#endif

#ifdef XSNS_42
  XSNS_42,
#endif

#ifdef XSNS_43
  XSNS_43,
#endif

#ifdef XSNS_44
  XSNS_44,
#endif

#ifdef XSNS_45
  XSNS_45,
#endif

#ifdef XSNS_46
  XSNS_46,
#endif

#ifdef XSNS_47
  XSNS_47,
#endif

#ifdef XSNS_48
  XSNS_48,
#endif

#ifdef XSNS_49
  XSNS_49,
#endif

#ifdef XSNS_50
  XSNS_50,
#endif

// Optional user defined sensors in range 91 - 99

#ifdef XSNS_91
  XSNS_91,
#endif

#ifdef XSNS_92
  XSNS_92,
#endif

#ifdef XSNS_93
  XSNS_93,
#endif

#ifdef XSNS_94
  XSNS_94,
#endif

#ifdef XSNS_95
  XSNS_95,
#endif

#ifdef XSNS_96
  XSNS_96,
#endif

#ifdef XSNS_97
  XSNS_97,
#endif

#ifdef XSNS_98
  XSNS_98,
#endif

#ifdef XSNS_99
  XSNS_99
#endif
};


/*********************************************************************************************\
 * Function call to all xsns
\*********************************************************************************************/

boolean XsnsEnabled(byte sns_index)
{
  if (sns_index < sizeof(kXsnsList)) {
#ifdef XFUNC_PTR_IN_ROM
    uint8_t index = pgm_read_byte(kXsnsList + sns_index);
#else
    uint8_t index = kXsnsList[sns_index];
#endif
    return bitRead(Settings.sensors[index / 32], index % 32);
  }
  return 1;
}

boolean XsnsPresent(byte sns_index)
{
  uint8_t index = 0;
  for (byte i = 0; i < sizeof(kXsnsList); i++) {
#ifdef XFUNC_PTR_IN_ROM
    index = pgm_read_byte(kXsnsList + i);
#else
    index = kXsnsList[i];
#endif
    if (index == sns_index) { return true; }
  }
  return false;
}

String XsnsGetSensors()
{
  char state[2] = { 0 };

  String data = F("[");
  for (byte i = 0; i < MAX_XSNS_DRIVERS; i++) {
    if (i && (!(i % 16))) { data += F(","); }
    if (!(i % 16)) { data += F("\""); }
    state[0] = '-';
    if (XsnsPresent(i)) { state[0] = bitRead(Settings.sensors[i / 32], i % 32) ? '1' : '0'; }
    data += String(state);
    if (i && (!((i +1) % 16))) { data += F("\""); }
  }
  data += F("]");

  return data;
}

boolean XsnsNextCall(byte Function)
{
  xsns_index++;
  if (xsns_index == xsns_present) { xsns_index = 0; }
  while (!XsnsEnabled(xsns_index) && !xsns_index) {  // Perform at least first sensor (counter)
    xsns_index++;
    if (xsns_index == xsns_present) { xsns_index = 0; }
  }
  if (global_state.wifi_down) { delay(DRIVER_BOOT_DELAY); }
  return xsns_func_ptr[xsns_index](Function);
}

boolean XsnsCall(byte Function)
{
  boolean result = false;

#ifdef PROFILE_XSNS_EVERY_SECOND
  uint32_t profile_start_millis = millis();
#endif  // PROFILE_XSNS_EVERY_SECOND

  for (byte x = 0; x < xsns_present; x++) {
    if (XsnsEnabled(x)) {

#ifdef PROFILE_XSNS_SENSOR_EVERY_SECOND
      uint32_t profile_start_millis = millis();
#endif  // PROFILE_XSNS_SENSOR_EVERY_SECOND
      if (global_state.wifi_down) { delay(DRIVER_BOOT_DELAY); }
      result = xsns_func_ptr[x](Function);

#ifdef PROFILE_XSNS_SENSOR_EVERY_SECOND
      uint32_t profile_millis = millis() - profile_start_millis;
      if (profile_millis) {
        if (FUNC_EVERY_SECOND == Function) {
          snprintf_P(log_data, sizeof(log_data), PSTR("PRF: At %08u XsnsCall %d to Sensor %d took %u mS"), uptime, Function, x, profile_millis);
          AddLog(LOG_LEVEL_DEBUG);
        }
      }
#endif  // PROFILE_XSNS_SENSOR_EVERY_SECOND

      if (result) break;
    }
  }

#ifdef PROFILE_XSNS_EVERY_SECOND
  uint32_t profile_millis = millis() - profile_start_millis;
  if (profile_millis) {
    if (FUNC_EVERY_SECOND == Function) {
      snprintf_P(log_data, sizeof(log_data), PSTR("PRF: At %08u XsnsCall %d took %u mS"), uptime, Function, profile_millis);
      AddLog(LOG_LEVEL_DEBUG);
    }
  }
#endif  // PROFILE_XSNS_EVERY_SECOND

  return result;
}
