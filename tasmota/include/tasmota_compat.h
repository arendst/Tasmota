/*
  tasmota_compat.h - ESP32 support for Tasmota

  Copyright (C) 2021  Jörg Schüler-Maroldt and Theo Arends

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

#pragma once

#ifdef ESP32
#include <esp8266toEsp32.h>
// Modul
#undef MODULE
#define MODULE WEMOS // [Module] Select default model

// ESP32-C2 has no RTC slow memory; RTC_NOINIT_ATTR is explicitly broken there.
// The IDF header itself defines RTC_NOINIT_ATTR as static_assert(0,...) on C2,
// so we must redefine it *after* that header has been pulled in via esp8266toEsp32.h.
#ifdef CONFIG_IDF_TARGET_ESP32C2
  #ifdef RTC_NOINIT_ATTR
    #undef RTC_NOINIT_ATTR
  #endif
  #define RTC_NOINIT_ATTR __NOINIT_ATTR
#endif  // CONFIG_IDF_TARGET_ESP32C2

#endif  // ESP32

#ifdef ESP8266
// ESP8266
//
// UDP
#define PortUdp_write(p,n) PortUdp.write(p, n)
//
// Serial minimal type to hold the config
#define SerConfu8 uint8_t
#endif  // ESP8266
