/*
  xsns_interface.ino - Sensor interface support for Tasmota

  Copyright (C) 2021  Theo Arends inspired by ESPEasy

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

#ifdef XSNS_51
  &Xsns51,
#endif

#ifdef XSNS_52
  &Xsns52,
#endif

#ifdef XSNS_53
  &Xsns53,
#endif

#ifdef XSNS_54
  &Xsns54,
#endif

#ifdef XSNS_55
  &Xsns55,
#endif

#ifdef XSNS_56
  &Xsns56,
#endif

#ifdef XSNS_57
  &Xsns57,
#endif

#ifdef XSNS_58
  &Xsns58,
#endif

#ifdef XSNS_59
  &Xsns59,
#endif

#ifdef XSNS_60
  &Xsns60,
#endif

#ifdef XSNS_61
  &Xsns61,
#endif

#ifdef XSNS_62
  &Xsns62,
#endif

#ifdef XSNS_63
  &Xsns63,
#endif

#ifdef XSNS_64
  &Xsns64,
#endif

#ifdef XSNS_65
  &Xsns65,
#endif

#ifdef XSNS_66
  &Xsns66,
#endif

#ifdef XSNS_67
  &Xsns67,
#endif

#ifdef XSNS_68
  &Xsns68,
#endif

#ifdef XSNS_69
  &Xsns69,
#endif

#ifdef XSNS_70
  &Xsns70,
#endif

#ifdef XSNS_71
  &Xsns71,
#endif

#ifdef XSNS_72
  &Xsns72,
#endif

#ifdef XSNS_73
  &Xsns73,
#endif

#ifdef XSNS_74
  &Xsns74,
#endif

#ifdef XSNS_75
  &Xsns75,
#endif

#ifdef XSNS_76
  &Xsns76,
#endif

#ifdef XSNS_77
  &Xsns77,
#endif

#ifdef XSNS_78
  &Xsns78,
#endif

#ifdef XSNS_79
  &Xsns79,
#endif

#ifdef XSNS_80
  &Xsns80,
#endif

#ifdef XSNS_81
  &Xsns81,
#endif

#ifdef XSNS_82
  &Xsns82,
#endif

#ifdef XSNS_83
  &Xsns83,
#endif

#ifdef XSNS_84
  &Xsns84,
#endif

#ifdef XSNS_85
  &Xsns85,
#endif

#ifdef XSNS_86
  &Xsns86,
#endif

#ifdef XSNS_87
  &Xsns87,
#endif

#ifdef XSNS_88
  &Xsns88,
#endif

#ifdef XSNS_89
  &Xsns89,
#endif

#ifdef XSNS_90
  &Xsns90,
#endif

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
  &Xsns99,
#endif

#ifdef XSNS_100
  &Xsns100,
#endif

#ifdef XSNS_101
  &Xsns101,
#endif

#ifdef XSNS_102
  &Xsns102,
#endif

#ifdef XSNS_103
  &Xsns103,
#endif

#ifdef XSNS_104
  &Xsns104,
#endif

#ifdef XSNS_105
  &Xsns105,
#endif

#ifdef XSNS_106
  &Xsns106,
#endif

#ifdef XSNS_107
  &Xsns107,
#endif

#ifdef XSNS_108
  &Xsns108,
#endif

#ifdef XSNS_109
  &Xsns109
#endif

#ifdef XSNS_110
  &Xsns110,
#endif

#ifdef XSNS_111
  &Xsns111,
#endif

#ifdef XSNS_112
  &Xsns112,
#endif

#ifdef XSNS_113
  &Xsns113,
#endif

#ifdef XSNS_114
  &Xsns114,
#endif

#ifdef XSNS_115
  &Xsns115,
#endif

#ifdef XSNS_116
  &Xsns116,
#endif

#ifdef XSNS_117
  &Xsns117,
#endif

#ifdef XSNS_118
  &Xsns118,
#endif

#ifdef XSNS_119
  &Xsns119,
#endif

#ifdef XSNS_120
  &Xsns120,
#endif

#ifdef XSNS_121
  &Xsns121,
#endif

#ifdef XSNS_122
  &Xsns122,
#endif

#ifdef XSNS_123
  &Xsns123,
#endif

#ifdef XSNS_124
  &Xsns124,
#endif

#ifdef XSNS_125
  &Xsns125,
#endif

#ifdef XSNS_126
  &Xsns126,
#endif

#ifdef XSNS_127
  &Xsns127
#endif
};

const uint8_t xsns_present = sizeof(xsns_func_ptr) / sizeof(xsns_func_ptr[0]);  // Number of External Sensors found

/*********************************************************************************************\
 * Xsns available list
\*********************************************************************************************/

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

#ifdef XSNS_51
  XSNS_51,
#endif

#ifdef XSNS_52
  XSNS_52,
#endif

#ifdef XSNS_53
  XSNS_53,
#endif

#ifdef XSNS_54
  XSNS_54,
#endif

#ifdef XSNS_55
  XSNS_55,
#endif

#ifdef XSNS_56
  XSNS_56,
#endif

#ifdef XSNS_57
  XSNS_57,
#endif

#ifdef XSNS_58
  XSNS_58,
#endif

#ifdef XSNS_59
  XSNS_59,
#endif

#ifdef XSNS_60
  XSNS_60,
#endif

#ifdef XSNS_61
  XSNS_61,
#endif

#ifdef XSNS_62
  XSNS_62,
#endif

#ifdef XSNS_63
  XSNS_63,
#endif

#ifdef XSNS_64
  XSNS_64,
#endif

#ifdef XSNS_65
  XSNS_65,
#endif

#ifdef XSNS_66
  XSNS_66,
#endif

#ifdef XSNS_67
  XSNS_67,
#endif

#ifdef XSNS_68
  XSNS_68,
#endif

#ifdef XSNS_69
  XSNS_69,
#endif

#ifdef XSNS_70
  XSNS_70,
#endif

#ifdef XSNS_71
  XSNS_71,
#endif

#ifdef XSNS_72
  XSNS_72,
#endif

#ifdef XSNS_73
  XSNS_73,
#endif

#ifdef XSNS_74
  XSNS_74,
#endif

#ifdef XSNS_75
  XSNS_75,
#endif

#ifdef XSNS_76
  XSNS_76,
#endif

#ifdef XSNS_77
  XSNS_77,
#endif

#ifdef XSNS_78
  XSNS_78,
#endif

#ifdef XSNS_79
  XSNS_79,
#endif

#ifdef XSNS_80
  XSNS_80,
#endif

#ifdef XSNS_81
  XSNS_81,
#endif

#ifdef XSNS_82
  XSNS_82,
#endif

#ifdef XSNS_83
  XSNS_83,
#endif

#ifdef XSNS_84
  XSNS_84,
#endif

#ifdef XSNS_85
  XSNS_85,
#endif

#ifdef XSNS_86
  XSNS_86,
#endif

#ifdef XSNS_87
  XSNS_87,
#endif

#ifdef XSNS_88
  XSNS_88,
#endif

#ifdef XSNS_89
  XSNS_89,
#endif

#ifdef XSNS_90
  XSNS_90,
#endif

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
  XSNS_99,
#endif

#ifdef XSNS_100
  XSNS_100,
#endif

#ifdef XSNS_101
  XSNS_101,
#endif

#ifdef XSNS_102
  XSNS_102,
#endif

#ifdef XSNS_103
  XSNS_103,
#endif

#ifdef XSNS_104
  XSNS_104,
#endif

#ifdef XSNS_105
  XSNS_105,
#endif

#ifdef XSNS_106
  XSNS_106,
#endif

#ifdef XSNS_107
  XSNS_107,
#endif

#ifdef XSNS_108
  XSNS_108,
#endif

#ifdef XSNS_109
  XSNS_109,
#endif

#ifdef XSNS_110
  XSNS_110,
#endif

#ifdef XSNS_111
  XSNS_111,
#endif

#ifdef XSNS_112
  XSNS_112,
#endif

#ifdef XSNS_113
  XSNS_113,
#endif

#ifdef XSNS_114
  XSNS_114,
#endif

#ifdef XSNS_115
  XSNS_115,
#endif

#ifdef XSNS_116
  XSNS_116,
#endif

#ifdef XSNS_117
  XSNS_117,
#endif

#ifdef XSNS_118
  XSNS_118,
#endif

#ifdef XSNS_119
  XSNS_119,
#endif

#ifdef XSNS_120
  XSNS_120,
#endif

#ifdef XSNS_121
  XSNS_121,
#endif

#ifdef XSNS_122
  XSNS_122,
#endif

#ifdef XSNS_123
  XSNS_123,
#endif

#ifdef XSNS_124
  XSNS_124,
#endif

#ifdef XSNS_125
  XSNS_125,
#endif

#ifdef XSNS_126
  XSNS_126,
#endif

#ifdef XSNS_127
  XSNS_127
#endif
};

/*********************************************************************************************/

bool XsnsEnabled(uint32_t sensor_list, uint32_t sns_index) {
  // sensor_list 0 = sensors
  // sensor_list 1 = web_sensors
  if (sns_index < sizeof(kXsnsList)) {
#ifdef XFUNC_PTR_IN_ROM
    uint32_t index = pgm_read_byte(kXsnsList + sns_index);
#else
    uint32_t index = kXsnsList[sns_index];
#endif
    if (index < MAX_XSNS_DRIVERS) {
      return bitRead(Settings->sensors[sensor_list][index / 32], index % 32);
    }
  }
  return true;
}

void XsnsSensorState(uint32_t sensor_list) {
  // sensor_list 0 = sensors
  // sensor_list 1 = web_sensors
  ResponseAppend_P(PSTR("\""));  // Use string for enable/disable signal
  for (uint32_t i = 0; i < sizeof(kXsnsList); i++) {
#ifdef XFUNC_PTR_IN_ROM
    uint32_t sensorid = pgm_read_byte(kXsnsList + i);
#else
    uint32_t sensorid = kXsnsList[i];
#endif
    bool disabled = false;
    if (sensorid < MAX_XSNS_DRIVERS) {
      disabled = !bitRead(Settings->sensors[sensor_list][sensorid / 32], sensorid % 32);
    }
    ResponseAppend_P(PSTR("%s%s%d"), (i) ? "," : "", (disabled) ? "!" : "", sensorid);
  }
  ResponseAppend_P(PSTR("\""));
}

/*********************************************************************************************\
 * Function call to all xsns
\*********************************************************************************************/

bool XsnsNextCall(uint8_t Function, uint8_t &xsns_index) {
  if (0 == xsns_present) {
    xsns_index = 0;
    return false;
  }

  xsns_index++;
  if (xsns_index == xsns_present) { xsns_index = 0; }
  uint32_t max_disabled = xsns_present;
  while ((!XsnsEnabled(0, xsns_index) || ((FUNC_WEB_SENSOR == Function) && !XsnsEnabled(1, xsns_index))) && max_disabled--) {  // Perform at least one sensor
    xsns_index++;
    if (xsns_index == xsns_present) { xsns_index = 0; }
  }

  return xsns_func_ptr[xsns_index](Function);
}

bool XsnsCall(uint8_t Function) {
  bool result = false;

//  DEBUG_TRACE_LOG(PSTR("SNS: %d"), Function);

  uint32_t profile_driver_start = millis();

  for (uint32_t x = 0; x < xsns_present; x++) {
    if (XsnsEnabled(0, x)) {  // Skip disabled sensor
      if ((FUNC_WEB_SENSOR == Function) && !XsnsEnabled(1, x)) { continue; }  // Skip web info for disabled sensors

      uint32_t profile_function_start = millis();

      result = xsns_func_ptr[x](Function);

#ifdef USE_PROFILE_FUNCTION
#ifdef XFUNC_PTR_IN_ROM
      uint32_t index = pgm_read_byte(kXsnsList + x);
#else
      uint32_t index = kXsnsList[x];
#endif
      PROFILE_FUNCTION("sns", index, Function, profile_function_start);
#endif  // USE_PROFILE_FUNCTION

      if (result && ((FUNC_COMMAND == Function) ||
                     (FUNC_PIN_STATE == Function) ||
                     (FUNC_COMMAND_SENSOR == Function)
                    )) {
        break;
      }
    }
  }

  PROFILE_DRIVER("sns", Function, profile_driver_start);

  return result;
}
