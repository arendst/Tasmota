/*
  xdrv_interface.ino - Driver interface support for Tasmota

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
bool (* const xdrv_func_ptr[])(uint8_t) PROGMEM = {   // Driver Function Pointers
#else
bool (* const xdrv_func_ptr[])(uint8_t) = {   // Driver Function Pointers
#endif

#ifdef XDRV_01
  &Xdrv01,
#endif

#ifdef XDRV_02
  &Xdrv02,
#endif

#ifdef XDRV_03
  &Xdrv03,
#endif

#ifdef XDRV_04
  &Xdrv04,
#endif

#ifdef XDRV_05
  &Xdrv05,
#endif

#ifdef XDRV_06
  &Xdrv06,
#endif

#ifdef XDRV_07
  &Xdrv07,
#endif

#ifdef XDRV_08
  &Xdrv08,
#endif

#ifdef XDRV_09
  &Xdrv09,
#endif

#ifdef XDRV_10
  &Xdrv10,
#endif

#ifdef XDRV_11
  &Xdrv11,
#endif

#ifdef XDRV_12
  &Xdrv12,
#endif

#ifdef XDRV_13
  &Xdrv13,
#endif

#ifdef XDRV_14
  &Xdrv14,
#endif

#ifdef XDRV_15
  &Xdrv15,
#endif

#ifdef XDRV_16
  &Xdrv16,
#endif

#ifdef XDRV_17
  &Xdrv17,
#endif

#ifdef XDRV_18
  &Xdrv18,
#endif

#ifdef XDRV_19
  &Xdrv19,
#endif

#ifdef XDRV_20
  &Xdrv20,
#endif

#ifdef XDRV_21
  &Xdrv21,
#endif

#ifdef XDRV_22
  &Xdrv22,
#endif

#ifdef XDRV_23
  &Xdrv23,
#endif

#ifdef XDRV_24
  &Xdrv24,
#endif

#ifdef XDRV_25
  &Xdrv25,
#endif

#ifdef XDRV_26
  &Xdrv26,
#endif

#ifdef XDRV_27
  &Xdrv27,
#endif

#ifdef XDRV_28
  &Xdrv28,
#endif

#ifdef XDRV_29
  &Xdrv29,
#endif

#ifdef XDRV_30
  &Xdrv30,
#endif

#ifdef XDRV_31
  &Xdrv31,
#endif

#ifdef XDRV_32
  &Xdrv32,
#endif

#ifdef XDRV_33
  &Xdrv33,
#endif

#ifdef XDRV_34
  &Xdrv34,
#endif

#ifdef XDRV_35
  &Xdrv35,
#endif

#ifdef XDRV_36
  &Xdrv36,
#endif

#ifdef XDRV_37
  &Xdrv37,
#endif

#ifdef XDRV_38
  &Xdrv38,
#endif

#ifdef XDRV_39
  &Xdrv39,
#endif

#ifdef XDRV_40
  &Xdrv40,
#endif

#ifdef XDRV_41
  &Xdrv41,
#endif

#ifdef XDRV_42
  &Xdrv42,
#endif

#ifdef XDRV_43
  &Xdrv43,
#endif

#ifdef XDRV_44
  &Xdrv44,
#endif

#ifdef XDRV_45
  &Xdrv45,
#endif

#ifdef XDRV_46
  &Xdrv46,
#endif

#ifdef XDRV_47
  &Xdrv47,
#endif

#ifdef XDRV_48
  &Xdrv48,
#endif

#ifdef XDRV_49
  &Xdrv49,
#endif

#ifdef XDRV_50
  &Xdrv50,
#endif

#ifdef XDRV_51
  &Xdrv51,
#endif

#ifdef XDRV_52
  &Xdrv52,
#endif

#ifdef XDRV_53
  &Xdrv53,
#endif

#ifdef XDRV_54
  &Xdrv54,
#endif

#ifdef XDRV_55
  &Xdrv55,
#endif

#ifdef XDRV_56
  &Xdrv56,
#endif

#ifdef XDRV_57
  &Xdrv57,
#endif

#ifdef XDRV_58
  &Xdrv58,
#endif

#ifdef XDRV_59
  &Xdrv59,
#endif

#ifdef XDRV_60
  &Xdrv60,
#endif

#ifdef XDRV_61
  &Xdrv61,
#endif

#ifdef XDRV_62
  &Xdrv62,
#endif

#ifdef XDRV_63
  &Xdrv63,
#endif

#ifdef XDRV_64
  &Xdrv64,
#endif

#ifdef XDRV_65
  &Xdrv65,
#endif

#ifdef XDRV_66
  &Xdrv66,
#endif

#ifdef XDRV_67
  &Xdrv67,
#endif

#ifdef XDRV_68
  &Xdrv68,
#endif

#ifdef XDRV_69
  &Xdrv69,
#endif

#ifdef XDRV_70
  &Xdrv70,
#endif

#ifdef XDRV_71
  &Xdrv71,
#endif

#ifdef XDRV_72
  &Xdrv72,
#endif

#ifdef XDRV_73
  &Xdrv73,
#endif

#ifdef XDRV_74
  &Xdrv74,
#endif

#ifdef XDRV_75
  &Xdrv75,
#endif

#ifdef XDRV_76
  &Xdrv76,
#endif

#ifdef XDRV_77
  &Xdrv77,
#endif

#ifdef XDRV_78
  &Xdrv78,
#endif

#ifdef XDRV_79
  &Xdrv79,
#endif

#ifdef XDRV_80
  &Xdrv80,
#endif

#ifdef XDRV_81
  &Xdrv81,
#endif

#ifdef XDRV_82
  &Xdrv82,
#endif

#ifdef XDRV_83
  &Xdrv83,
#endif

#ifdef XDRV_84
  &Xdrv84,
#endif

#ifdef XDRV_85
  &Xdrv85,
#endif

#ifdef XDRV_86
  &Xdrv86,
#endif

#ifdef XDRV_87
  &Xdrv87,
#endif

#ifdef XDRV_88
  &Xdrv88,
#endif

#ifdef XDRV_89
  &Xdrv89,
#endif

#ifdef XDRV_90
  &Xdrv90,
#endif

#ifdef XDRV_91
  &Xdrv91,
#endif

#ifdef XDRV_92
  &Xdrv92,
#endif

#ifdef XDRV_93
  &Xdrv93,
#endif

#ifdef XDRV_94
  &Xdrv94,
#endif

#ifdef XDRV_95
  &Xdrv95,
#endif

#ifdef XDRV_96
  &Xdrv96,
#endif

#ifdef XDRV_97
  &Xdrv97,
#endif

#ifdef XDRV_98
  &Xdrv98,
#endif

#ifdef XDRV_99
  &Xdrv99,
#endif

#ifdef XDRV_100
  &Xdrv100,
#endif

#ifdef XDRV_101
  &Xdrv101,
#endif

#ifdef XDRV_102
  &Xdrv102,
#endif

#ifdef XDRV_103
  &Xdrv103,
#endif

#ifdef XDRV_104
  &Xdrv104,
#endif

#ifdef XDRV_105
  &Xdrv105,
#endif

#ifdef XDRV_106
  &Xdrv106,
#endif

#ifdef XDRV_107
  &Xdrv107,
#endif

#ifdef XDRV_108
  &Xdrv108,
#endif

#ifdef XDRV_109
  &Xdrv109,
#endif

#ifdef XDRV_110
  &Xdrv110,
#endif

#ifdef XDRV_111
  &Xdrv111,
#endif

#ifdef XDRV_112
  &Xdrv112,
#endif

#ifdef XDRV_113
  &Xdrv113,
#endif

#ifdef XDRV_114
  &Xdrv114,
#endif

#ifdef XDRV_115
  &Xdrv115,
#endif

#ifdef XDRV_116
  &Xdrv116,
#endif

#ifdef XDRV_117
  &Xdrv117,
#endif

#ifdef XDRV_118
  &Xdrv118,
#endif

#ifdef XDRV_119
  &Xdrv119,
#endif

#ifdef XDRV_120
  &Xdrv120,
#endif

#ifdef XDRV_121
  &Xdrv121,
#endif

#ifdef XDRV_122
  &Xdrv122,
#endif

#ifdef XDRV_123
  &Xdrv123,
#endif

#ifdef XDRV_124
  &Xdrv124,
#endif

#ifdef XDRV_125
  &Xdrv125,
#endif

#ifdef XDRV_126
  &Xdrv126,
#endif

#ifdef XDRV_127
  &Xdrv127,
#endif

#ifdef XDRV_128
  &Xdrv128
#endif
};

const uint8_t xdrv_present = sizeof(xdrv_func_ptr) / sizeof(xdrv_func_ptr[0]);  // Number of drivers found

/*********************************************************************************************\
 * Xdrv available list
\*********************************************************************************************/

#ifdef XFUNC_PTR_IN_ROM
const uint8_t kXdrvList[] PROGMEM = {
#else
const uint8_t kXdrvList[] = {
#endif

#ifdef XDRV_01
  XDRV_01,
#endif

#ifdef XDRV_02
  XDRV_02,
#endif

#ifdef XDRV_03
  XDRV_03,
#endif

#ifdef XDRV_04
  XDRV_04,
#endif

#ifdef XDRV_05
  XDRV_05,
#endif

#ifdef XDRV_06
  XDRV_06,
#endif

#ifdef XDRV_07
  XDRV_07,
#endif

#ifdef XDRV_08
  XDRV_08,
#endif

#ifdef XDRV_09
  XDRV_09,
#endif

#ifdef XDRV_10
  XDRV_10,
#endif

#ifdef XDRV_11
  XDRV_11,
#endif

#ifdef XDRV_12
  XDRV_12,
#endif

#ifdef XDRV_13
  XDRV_13,
#endif

#ifdef XDRV_14
  XDRV_14,
#endif

#ifdef XDRV_15
  XDRV_15,
#endif

#ifdef XDRV_16
  XDRV_16,
#endif

#ifdef XDRV_17
  XDRV_17,
#endif

#ifdef XDRV_18
  XDRV_18,
#endif

#ifdef XDRV_19
  XDRV_19,
#endif

#ifdef XDRV_20
  XDRV_20,
#endif

#ifdef XDRV_21
  XDRV_21,
#endif

#ifdef XDRV_22
  XDRV_22,
#endif

#ifdef XDRV_23
  XDRV_23,
#endif

#ifdef XDRV_24
  XDRV_24,
#endif

#ifdef XDRV_25
  XDRV_25,
#endif

#ifdef XDRV_26
  XDRV_26,
#endif

#ifdef XDRV_27
  XDRV_27,
#endif

#ifdef XDRV_28
  XDRV_28,
#endif

#ifdef XDRV_29
  XDRV_29,
#endif

#ifdef XDRV_30
  XDRV_30,
#endif

#ifdef XDRV_31
  XDRV_31,
#endif

#ifdef XDRV_32
  XDRV_32,
#endif

#ifdef XDRV_33
  XDRV_33,
#endif

#ifdef XDRV_34
  XDRV_34,
#endif

#ifdef XDRV_35
  XDRV_35,
#endif

#ifdef XDRV_36
  XDRV_36,
#endif

#ifdef XDRV_37
  XDRV_37,
#endif

#ifdef XDRV_38
  XDRV_38,
#endif

#ifdef XDRV_39
  XDRV_39,
#endif

#ifdef XDRV_40
  XDRV_40,
#endif

#ifdef XDRV_41
  XDRV_41,
#endif

#ifdef XDRV_42
  XDRV_42,
#endif

#ifdef XDRV_43
  XDRV_43,
#endif

#ifdef XDRV_44
  XDRV_44,
#endif

#ifdef XDRV_45
  XDRV_45,
#endif

#ifdef XDRV_46
  XDRV_46,
#endif

#ifdef XDRV_47
  XDRV_47,
#endif

#ifdef XDRV_48
  XDRV_48,
#endif

#ifdef XDRV_49
  XDRV_49,
#endif

#ifdef XDRV_50
  XDRV_50,
#endif

#ifdef XDRV_51
  XDRV_51,
#endif

#ifdef XDRV_52
  XDRV_52,
#endif

#ifdef XDRV_53
  XDRV_53,
#endif

#ifdef XDRV_54
  XDRV_54,
#endif

#ifdef XDRV_55
  XDRV_55,
#endif

#ifdef XDRV_56
  XDRV_56,
#endif

#ifdef XDRV_57
  XDRV_57,
#endif

#ifdef XDRV_58
  XDRV_58,
#endif

#ifdef XDRV_59
  XDRV_59,
#endif

#ifdef XDRV_60
  XDRV_60,
#endif

#ifdef XDRV_61
  XDRV_61,
#endif

#ifdef XDRV_62
  XDRV_62,
#endif

#ifdef XDRV_63
  XDRV_63,
#endif

#ifdef XDRV_64
  XDRV_64,
#endif

#ifdef XDRV_65
  XDRV_65,
#endif

#ifdef XDRV_66
  XDRV_66,
#endif

#ifdef XDRV_67
  XDRV_67,
#endif

#ifdef XDRV_68
  XDRV_68,
#endif

#ifdef XDRV_69
  XDRV_69,
#endif

#ifdef XDRV_70
  XDRV_70,
#endif

#ifdef XDRV_71
  XDRV_71,
#endif

#ifdef XDRV_72
  XDRV_72,
#endif

#ifdef XDRV_73
  XDRV_73,
#endif

#ifdef XDRV_74
  XDRV_74,
#endif

#ifdef XDRV_75
  XDRV_75,
#endif

#ifdef XDRV_76
  XDRV_76,
#endif

#ifdef XDRV_77
  XDRV_77,
#endif

#ifdef XDRV_78
  XDRV_78,
#endif

#ifdef XDRV_79
  XDRV_79,
#endif

#ifdef XDRV_80
  XDRV_80,
#endif

#ifdef XDRV_81
  XDRV_81,
#endif

#ifdef XDRV_82
  XDRV_82,
#endif

#ifdef XDRV_83
  XDRV_83,
#endif

#ifdef XDRV_84
  XDRV_84,
#endif

#ifdef XDRV_85
  XDRV_85,
#endif

#ifdef XDRV_86
  XDRV_86,
#endif

#ifdef XDRV_87
  XDRV_87,
#endif

#ifdef XDRV_88
  XDRV_88,
#endif

#ifdef XDRV_89
  XDRV_89,
#endif

#ifdef XDRV_90
  XDRV_90,
#endif

#ifdef XDRV_91
  XDRV_91,
#endif

#ifdef XDRV_92
  XDRV_92,
#endif

#ifdef XDRV_93
  XDRV_93,
#endif

#ifdef XDRV_94
  XDRV_94,
#endif

#ifdef XDRV_95
  XDRV_95,
#endif

#ifdef XDRV_96
  XDRV_96,
#endif

#ifdef XDRV_97
  XDRV_97,
#endif

#ifdef XDRV_98
  XDRV_98,
#endif

#ifdef XDRV_99
  XDRV_99,
#endif

#ifdef XDRV_100
  XDRV_100,
#endif

#ifdef XDRV_101
  XDRV_101,
#endif

#ifdef XDRV_102
  XDRV_102,
#endif

#ifdef XDRV_103
  XDRV_103,
#endif

#ifdef XDRV_104
  XDRV_104,
#endif

#ifdef XDRV_105
  XDRV_105,
#endif

#ifdef XDRV_106
  XDRV_106,
#endif

#ifdef XDRV_107
  XDRV_107,
#endif

#ifdef XDRV_108
  XDRV_108,
#endif

#ifdef XDRV_109
  XDRV_109,
#endif

#ifdef XDRV_110
  XDRV_110,
#endif

#ifdef XDRV_111
  XDRV_111,
#endif

#ifdef XDRV_112
  XDRV_112,
#endif

#ifdef XDRV_113
  XDRV_113,
#endif

#ifdef XDRV_114
  XDRV_114,
#endif

#ifdef XDRV_115
  XDRV_115,
#endif

#ifdef XDRV_116
  XDRV_116,
#endif

#ifdef XDRV_117
  XDRV_117,
#endif

#ifdef XDRV_118
  XDRV_118,
#endif

#ifdef XDRV_119
  XDRV_119,
#endif

#ifdef XDRV_120
  XDRV_120,
#endif

#ifdef XDRV_121
  XDRV_121,
#endif

#ifdef XDRV_122
  XDRV_122,
#endif

#ifdef XDRV_123
  XDRV_123,
#endif

#ifdef XDRV_124
  XDRV_124,
#endif

#ifdef XDRV_125
  XDRV_125,
#endif

#ifdef XDRV_126
  XDRV_126,
#endif

#ifdef XDRV_127
  XDRV_127,
#endif

#ifdef XDRV_128
  XDRV_128
#endif
};

/*********************************************************************************************/

void XsnsDriverState(void)
{
  ResponseAppend_P(PSTR(",\"Drivers\":\""));  // Use string for future enable/disable signal
  for (uint32_t i = 0; i < sizeof(kXdrvList); i++) {
#ifdef XFUNC_PTR_IN_ROM
    uint32_t driverid = pgm_read_byte(kXdrvList + i);
#else
    uint32_t driverid = kXdrvList[i];
#endif
    ResponseAppend_P(PSTR("%s%d"), (i) ? "," : "", driverid);
  }
  ResponseAppend_P(PSTR("\""));
}

/*********************************************************************************************/

bool XdrvRulesProcess(bool teleperiod, const char* event) {
  char* data_save = XdrvMailbox.data;
  XdrvMailbox.data = (char*)event;
  bool rule_handled = XdrvCallDriver(10, (teleperiod) ? FUNC_TELEPERIOD_RULES_PROCESS : FUNC_RULES_PROCESS);
#if defined(USE_BERRY) && !defined(USE_RULES)
  // events are sent to Berry in Rules driver, or here if USE_RULES is not defined (only on a subset)
  bool berry_handled = XdrvCallDriver(52, FUNC_RULES_PROCESS);
  rule_handled |= berry_handled;
#endif
  XdrvMailbox.data = data_save;
  return rule_handled;
}

bool XdrvRulesProcess(bool teleperiod) {
  bool result = XdrvRulesProcess(teleperiod, ResponseData());
  ResponseClear();  // Free heap space
  return result;
}

#ifdef USE_DEBUG_DRIVER
void ShowFreeMem(const char *where)
{
  char stemp[30];
  snprintf_P(stemp, sizeof(stemp), where);
  XdrvMailbox.data = stemp;
  XdrvCall(FUNC_FREE_MEM);
}
#endif

/*********************************************************************************************\
 * Function call to single xdrv
\*********************************************************************************************/

bool XdrvCallDriver(uint32_t driver, uint8_t Function)
{
  for (uint32_t x = 0; x < xdrv_present; x++) {
#ifdef XFUNC_PTR_IN_ROM
    uint32_t listed = pgm_read_byte(kXdrvList + x);
#else
    uint32_t listed = kXdrvList[x];
#endif
    if (driver == listed) {
      return xdrv_func_ptr[x](Function);
    }
  }
  return false;
}

/*********************************************************************************************\
 * Function call to all xdrv
\*********************************************************************************************/

bool XdrvCall(uint8_t Function)
{
  bool result = false;

//  DEBUG_TRACE_LOG(PSTR("DRV: %d"), Function);

  uint32_t profile_driver_start = millis();

  for (uint32_t x = 0; x < xdrv_present; x++) {

    uint32_t profile_function_start = millis();

    result = xdrv_func_ptr[x](Function);

#ifdef USE_PROFILE_FUNCTION
#ifdef XFUNC_PTR_IN_ROM
      uint32_t index = pgm_read_byte(kXdrvList + x);
#else
      uint32_t index = kXdrvList[x];
#endif
    PROFILE_FUNCTION("drv", index, Function, profile_function_start);
#endif  // USE_PROFILE_FUNCTION

    if (result && ((FUNC_COMMAND == Function) ||
                   (FUNC_COMMAND_DRIVER == Function) ||
                   (FUNC_MQTT_DATA == Function) ||
                   (FUNC_RULES_PROCESS == Function) ||
                   (FUNC_BUTTON_PRESSED == Function) ||
                   (FUNC_SERIAL == Function) ||
                   (FUNC_MODULE_INIT == Function) ||
                   (FUNC_SET_CHANNELS == Function) ||
                   (FUNC_PIN_STATE == Function) ||
                   (FUNC_SET_DEVICE_POWER == Function)
                  )) {
      break;
    }
  }

  PROFILE_DRIVER("drv", Function, profile_driver_start);

  return result;
}
