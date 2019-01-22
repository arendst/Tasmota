/*
  xnrg_interface.ino - Energy driver interface support for Sonoff-Tasmota

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
int (* const xnrg_func_ptr[])(byte) PROGMEM = {   // Energy driver Function Pointers
#else
int (* const xnrg_func_ptr[])(byte) = {   // Energy driver Function Pointers
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
  &Xnrg16
#endif
};

const uint8_t xnrg_present = sizeof(xnrg_func_ptr) / sizeof(xnrg_func_ptr[0]);  // Number of drivers found

int XnrgCall(byte Function)
{
  int result = 0;

  for (byte x = 0; x < xnrg_present; x++) {
    result = xnrg_func_ptr[x](Function);
    if (result) break;
  }
  return result;
}
