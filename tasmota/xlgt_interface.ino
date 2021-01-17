/*
  xlgt_interface.ino - Light driver interface support for Tasmota

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

#ifdef USE_LIGHT

#ifdef XFUNC_PTR_IN_ROM
bool (* const xlgt_func_ptr[])(uint8_t) PROGMEM = {   // Light driver Function Pointers
#else
bool (* const xlgt_func_ptr[])(uint8_t) = {   // Light driver Function Pointers
#endif

#ifdef XLGT_01
  &Xlgt01,
#endif

#ifdef XLGT_02
  &Xlgt02,
#endif

#ifdef XLGT_03
  &Xlgt03,
#endif

#ifdef XLGT_04
  &Xlgt04,
#endif

#ifdef XLGT_05
  &Xlgt05,
#endif

#ifdef XLGT_06
  &Xlgt06,
#endif

#ifdef XLGT_07
  &Xlgt07,
#endif

#ifdef XLGT_08
  &Xlgt08,
#endif

#ifdef XLGT_09
  &Xlgt09,
#endif

#ifdef XLGT_10
  &Xlgt10,
#endif

#ifdef XLGT_11
  &Xlgt11,
#endif

#ifdef XLGT_12
  &Xlgt12,
#endif

#ifdef XLGT_13
  &Xlgt13,
#endif

#ifdef XLGT_14
  &Xlgt14,
#endif

#ifdef XLGT_15
  &Xlgt15,
#endif

#ifdef XLGT_16
  &Xlgt16
#endif
};

const uint8_t xlgt_present = sizeof(xlgt_func_ptr) / sizeof(xlgt_func_ptr[0]);  // Number of drivers found

uint8_t xlgt_active = 0;

bool XlgtCall(uint8_t function)
{
  DEBUG_TRACE_LOG(PSTR("LGT: %d"), function);

  if (FUNC_MODULE_INIT == function) {
    for (uint32_t x = 0; x < xlgt_present; x++) {
      xlgt_func_ptr[x](function);
      if (TasmotaGlobal.light_driver) {
        xlgt_active = x;
        return true;  // Stop further driver investigation
      }
    }
  }
  else if (TasmotaGlobal.light_driver) {
    return xlgt_func_ptr[xlgt_active](function);
  }
  return false;
}

#endif  // USE_LIGHT
