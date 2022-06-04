/*
  xnrg_interface.ino - Energy driver interface support for Tasmota

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

#ifdef USE_ENERGY_SENSOR

#ifdef XFUNC_PTR_IN_ROM
bool (* const xnrg_func_ptr[])(uint8_t) PROGMEM = {   // Energy driver Function Pointers
#else
bool (* const xnrg_func_ptr[])(uint8_t) = {   // Energy driver Function Pointers
#endif

#ifdef XNRG_01
  &Xnrg01,
#endif

#ifdef XNRG_02
  &Xnrg02,
#endif

#ifdef XNRG_03
  &Xnrg03,
#endif

#ifdef XNRG_04
  &Xnrg04,
#endif

#ifdef XNRG_05
  &Xnrg05,
#endif

#ifdef XNRG_06
  &Xnrg06,
#endif

#ifdef XNRG_07
  &Xnrg07,
#endif

#ifdef XNRG_08
  &Xnrg08,
#endif

#ifdef XNRG_09
  &Xnrg09,
#endif

#ifdef XNRG_10
  &Xnrg10,
#endif

#ifdef XNRG_11
  &Xnrg11,
#endif

#ifdef XNRG_12
  &Xnrg12,
#endif

#ifdef XNRG_13
  &Xnrg13,
#endif

#ifdef XNRG_14
  &Xnrg14,
#endif

#ifdef XNRG_15
  &Xnrg15,
#endif

#ifdef XNRG_16
  &Xnrg16,
#endif

#ifdef XNRG_17
  &Xnrg17,
#endif

#ifdef XNRG_18
  &Xnrg18,
#endif

#ifdef XNRG_19
  &Xnrg19,
#endif

#ifdef XNRG_20
  &Xnrg20,
#endif

#ifdef XNRG_21
  &Xnrg21,
#endif

#ifdef XNRG_22
  &Xnrg22,
#endif

#ifdef XNRG_23
  &Xnrg23,
#endif

#ifdef XNRG_24
  &Xnrg24,
#endif

#ifdef XNRG_25
  &Xnrg25,
#endif

#ifdef XNRG_26
  &Xnrg26,
#endif

#ifdef XNRG_27
  &Xnrg27,
#endif

#ifdef XNRG_28
  &Xnrg28,
#endif

#ifdef XNRG_29
  &Xnrg29,
#endif

#ifdef XNRG_30
  &Xnrg30,
#endif

#ifdef XNRG_31  // Reserved for use by xdrv_45_shelly_dimmer.ino
  &Xnrg31,
#endif

#ifdef XNRG_32  // Reserved for use by xdrv_16_tuyamcu.ino
  &Xnrg32
#endif
};

const uint8_t xnrg_present = sizeof(xnrg_func_ptr) / sizeof(xnrg_func_ptr[0]);  // Number of drivers found

uint8_t xnrg_active = 0;

bool XnrgCall(uint8_t function)
{
  DEBUG_TRACE_LOG(PSTR("NRG: %d"), function);

  if (FUNC_PRE_INIT == function) {
    for (uint32_t x = 0; x < xnrg_present; x++) {
      xnrg_func_ptr[x](function);
      if (TasmotaGlobal.energy_driver) {
        xnrg_active = x;
        return true;  // Stop further driver investigation
      }
    }
  }
  else if (TasmotaGlobal.energy_driver) {
    return xnrg_func_ptr[xnrg_active](function);
  }
  return false;
}

#endif  // USE_ENERGY_SENSOR
