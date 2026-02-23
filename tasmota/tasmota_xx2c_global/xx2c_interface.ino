/*
  xx2c_interface.ino - I2c interface support for Tasmota

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

#ifdef USE_I2C

#ifdef XFUNC_PTR_IN_ROM
const uint8_t kI2cList[] PROGMEM = {
#else
const uint8_t kI2cList[] = {
#endif

#ifdef XI2C_01
  XI2C_01,
#endif

#ifdef XI2C_02
  XI2C_02,
#endif

#ifdef XI2C_03
  XI2C_03,
#endif

#ifdef XI2C_04
  XI2C_04,
#endif

#ifdef XI2C_05
  XI2C_05,
#endif

#ifdef XI2C_06
  XI2C_06,
#endif

#ifdef XI2C_07
  XI2C_07,
#endif

#ifdef XI2C_08
  XI2C_08,
#endif

#ifdef XI2C_09
  XI2C_09,
#endif

#ifdef XI2C_10
  XI2C_10,
#endif

#ifdef XI2C_11
  XI2C_11,
#endif

#ifdef XI2C_12
  XI2C_12,
#endif

#ifdef XI2C_13
  XI2C_13,
#endif

#ifdef XI2C_14
  XI2C_14,
#endif

#ifdef XI2C_15
  XI2C_15,
#endif

#ifdef XI2C_16
  XI2C_16,
#endif

#ifdef XI2C_17
  XI2C_17,
#endif

#ifdef XI2C_18
  XI2C_18,
#endif

#ifdef XI2C_19
  XI2C_19,
#endif

#ifdef XI2C_20
  XI2C_20,
#endif

#ifdef XI2C_21
  XI2C_21,
#endif

#ifdef XI2C_22
  XI2C_22,
#endif

#ifdef XI2C_23
  XI2C_23,
#endif

#ifdef XI2C_24
  XI2C_24,
#endif

#ifdef XI2C_25
  XI2C_25,
#endif

#ifdef XI2C_26
  XI2C_26,
#endif

#ifdef XI2C_27
  XI2C_27,
#endif

#ifdef XI2C_28
  XI2C_28,
#endif

#ifdef XI2C_29
  XI2C_29,
#endif

#ifdef XI2C_30
  XI2C_30,
#endif

#ifdef XI2C_31
  XI2C_31,
#endif

#ifdef XI2C_32
  XI2C_32,
#endif

#ifdef XI2C_33
  XI2C_33,
#endif

#ifdef XI2C_34
  XI2C_34,
#endif

#ifdef XI2C_35
  XI2C_35,
#endif

#ifdef XI2C_36
  XI2C_36,
#endif

#ifdef XI2C_37
  XI2C_37,
#endif

#ifdef XI2C_38
  XI2C_38,
#endif

#ifdef XI2C_39
  XI2C_39,
#endif

#ifdef XI2C_40
  XI2C_40,
#endif

#ifdef XI2C_41
  XI2C_41,
#endif

#ifdef XI2C_42
  XI2C_42,
#endif

#ifdef XI2C_43
  XI2C_43,
#endif

#ifdef XI2C_44
  XI2C_44,
#endif

#ifdef XI2C_45
  XI2C_45,
#endif

#ifdef XI2C_46
  XI2C_46,
#endif

#ifdef XI2C_47
  XI2C_47,
#endif

#ifdef XI2C_48
  XI2C_48,
#endif

#ifdef XI2C_49
  XI2C_49,
#endif

#ifdef XI2C_50
  XI2C_50,
#endif

#ifdef XI2C_51
  XI2C_51,
#endif

#ifdef XI2C_52
  XI2C_52,
#endif

#ifdef XI2C_53
  XI2C_53,
#endif

#ifdef XI2C_54
  XI2C_54,
#endif

#ifdef XI2C_55
  XI2C_55,
#endif

#ifdef XI2C_56
  XI2C_56,
#endif

#ifdef XI2C_57
  XI2C_57,
#endif

#ifdef XI2C_58
  XI2C_58,
#endif

#ifdef XI2C_59
  XI2C_59,
#endif

#ifdef XI2C_60
  XI2C_60,
#endif

#ifdef XI2C_61
  XI2C_61,
#endif

#ifdef XI2C_62
  XI2C_62,
#endif

#ifdef XI2C_63
  XI2C_63,
#endif

#ifdef XI2C_64
  XI2C_64,
#endif

#ifdef XI2C_65
  XI2C_65,
#endif

#ifdef XI2C_66
  XI2C_66,
#endif

#ifdef XI2C_67
  XI2C_67,
#endif

#ifdef XI2C_68
  XI2C_68,
#endif

#ifdef XI2C_69
  XI2C_69,
#endif

#ifdef XI2C_70
  XI2C_70,
#endif

#ifdef XI2C_71
  XI2C_71,
#endif

#ifdef XI2C_72
  XI2C_72,
#endif

#ifdef XI2C_73
  XI2C_73,
#endif

#ifdef XI2C_74
  XI2C_74,
#endif

#ifdef XI2C_75
  XI2C_75,
#endif

#ifdef XI2C_76
  XI2C_76,
#endif

#ifdef XI2C_77
  XI2C_77,
#endif

#ifdef XI2C_78
  XI2C_78,
#endif

#ifdef XI2C_79
  XI2C_79,
#endif

#ifdef XI2C_80
  XI2C_80,
#endif

#ifdef XI2C_81
  XI2C_81,
#endif

#ifdef XI2C_82
  XI2C_82,
#endif

#ifdef XI2C_83
  XI2C_83,
#endif

#ifdef XI2C_84
  XI2C_84,
#endif

#ifdef XI2C_85
  XI2C_85,
#endif

#ifdef XI2C_86
  XI2C_86,
#endif

#ifdef XI2C_87
  XI2C_87,
#endif

#ifdef XI2C_88
  XI2C_88,
#endif

#ifdef XI2C_89
  XI2C_89,
#endif

#ifdef XI2C_90
  XI2C_90,
#endif

#ifdef XI2C_91
  XI2C_91,
#endif

#ifdef XI2C_92
  XI2C_92,
#endif

#ifdef XI2C_93
  XI2C_93,
#endif

#ifdef XI2C_94
  XI2C_94,
#endif

#ifdef XI2C_95
  XI2C_95,
#endif

#ifdef XI2C_96
  XI2C_96,
#endif

#ifdef XI2C_97
  XI2C_97,
#endif

#ifdef XI2C_98
  XI2C_98,
#endif

#ifdef XI2C_99
  XI2C_99,
#endif

#ifdef XI2C_100
  XI2C_100,
#endif

#ifdef XI2C_101
  XI2C_101,
#endif

#ifdef XI2C_102
  XI2C_102,
#endif

#ifdef XI2C_103
  XI2C_103,
#endif

#ifdef XI2C_104
  XI2C_104,
#endif

#ifdef XI2C_105
  XI2C_105,
#endif

#ifdef XI2C_106
  XI2C_106,
#endif

#ifdef XI2C_107
  XI2C_107,
#endif

#ifdef XI2C_108
  XI2C_108,
#endif

#ifdef XI2C_109
  XI2C_109,
#endif

#ifdef XI2C_110
  XI2C_110,
#endif

#ifdef XI2C_111
  XI2C_111,
#endif

#ifdef XI2C_112
  XI2C_112,
#endif

#ifdef XI2C_113
  XI2C_113,
#endif

#ifdef XI2C_114
  XI2C_114,
#endif

#ifdef XI2C_115
  XI2C_115,
#endif

#ifdef XI2C_116
  XI2C_116,
#endif

#ifdef XI2C_117
  XI2C_117,
#endif

#ifdef XI2C_118
  XI2C_118,
#endif

#ifdef XI2C_119
  XI2C_119,
#endif

#ifdef XI2C_120
  XI2C_120,
#endif

#ifdef XI2C_121
  XI2C_121,
#endif

#ifdef XI2C_122
  XI2C_122,
#endif

#ifdef XI2C_123
  XI2C_123,
#endif

#ifdef XI2C_124
  XI2C_124,
#endif

#ifdef XI2C_125
  XI2C_125,
#endif

#ifdef XI2C_126
  XI2C_126,
#endif

#ifdef XI2C_127
  XI2C_127,
#endif

#ifdef XI2C_128
  XI2C_128,
#endif

#ifdef XI2C_129
  XI2C_129,
#endif

#ifdef XI2C_130
  XI2C_130,
#endif

#ifdef XI2C_131
  XI2C_131,
#endif

#ifdef XI2C_132
  XI2C_132,
#endif

#ifdef XI2C_133
  XI2C_133,
#endif

#ifdef XI2C_134
  XI2C_134,
#endif

#ifdef XI2C_135
  XI2C_135,
#endif

#ifdef XI2C_136
  XI2C_136,
#endif

#ifdef XI2C_137
  XI2C_137,
#endif

#ifdef XI2C_138
  XI2C_138,
#endif

#ifdef XI2C_139
  XI2C_139,
#endif

#ifdef XI2C_140
  XI2C_140,
#endif

#ifdef XI2C_141
  XI2C_141,
#endif

#ifdef XI2C_142
  XI2C_142,
#endif

#ifdef XI2C_143
  XI2C_143,
#endif

#ifdef XI2C_144
  XI2C_144,
#endif

#ifdef XI2C_145
  XI2C_145,
#endif

#ifdef XI2C_146
  XI2C_146,
#endif

#ifdef XI2C_147
  XI2C_147,
#endif

#ifdef XI2C_148
  XI2C_148,
#endif

#ifdef XI2C_149
  XI2C_149,
#endif

#ifdef XI2C_150
  XI2C_150,
#endif

#ifdef XI2C_151
  XI2C_151,
#endif

#ifdef XI2C_152
  XI2C_152,
#endif

#ifdef XI2C_153
  XI2C_153,
#endif

#ifdef XI2C_154
  XI2C_154,
#endif

#ifdef XI2C_155
  XI2C_155,
#endif

#ifdef XI2C_156
  XI2C_156,
#endif

#ifdef XI2C_157
  XI2C_157,
#endif

#ifdef XI2C_158
  XI2C_158,
#endif

#ifdef XI2C_159
  XI2C_159
#endif

};

/*********************************************************************************************/

bool I2cEnabled(uint32_t i2c_index) {
  bool state;
  if (i2c_index < 96) {
    state = bitRead(Settings->i2c_drivers[i2c_index / 32], i2c_index % 32);
  } else {
    state = bitRead(Settings->i2c_drivers2[(i2c_index / 32) -3], i2c_index % 32);
  }
  return ((TasmotaGlobal.i2c_enabled[0]
#ifdef USE_I2C_BUS2
   || TasmotaGlobal.i2c_enabled[1]
#endif
   ) && state);
}

void I2cDriverState(void) {
  ResponseAppend_P(PSTR("\""));  // Use string for enable/disable signal
  for (uint32_t i = 0; i < sizeof(kI2cList); i++) {
#ifdef XFUNC_PTR_IN_ROM
    uint32_t i2c_driver_id = pgm_read_byte(kI2cList + i);
#else
    uint32_t i2c_driver_id = kI2cList[i];
#endif
    bool disabled = false;
    if (i2c_driver_id < MAX_I2C_DRIVERS) {
      if (i2c_driver_id < 96) {
        disabled = !bitRead(Settings->i2c_drivers[i2c_driver_id / 32], i2c_driver_id % 32);
      } else {
        disabled = !bitRead(Settings->i2c_drivers2[(i2c_driver_id / 32) -3], i2c_driver_id % 32);
      }
    }
    ResponseAppend_P(PSTR("%s%s%d"), (i) ? "," : "", (disabled) ? "!" : "", i2c_driver_id);
  }
  ResponseAppend_P(PSTR("\""));
}

#endif  // USE_I2C
