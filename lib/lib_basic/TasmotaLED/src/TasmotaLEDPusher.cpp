/*
  TasmotaLEDPusher.cpp - Implementation to push Leds via hardware acceleration

  Copyright (C) 2024  Stephan Hadinger

  This library is free software: you can redistribute it and/or modify
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

#ifdef ESP32

#include "TasmotaLEDPusher.h"
#include "TasmotaLED.h"

//**************************************************************************************************************
// enable AddLog support within a C++ library
extern void AddLog(uint32_t loglevel, PGM_P formatP, ...);
enum LoggingLevels {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE};
//**************************************************************************************************************


// convert to the appropriate hardware acceleration based on capacities of the SOC
uint32_t TasmotaLEDPusher::ResolveHardware(uint32_t hw) {
uint32_t hw_orig = hw;
  // Step 1. discard any unsupported hardware, and replace with TasmotaLed_HW_Default
  uint32_t hw_type = hw & 0xFF0000;     // discard bits 0..15
#if !TASMOTALED_HARDWARE_RMT
  if (hw_type == TasmotaLed_RMT) {
    hw = TasmotaLed_HW_None;
  }
#endif // TASMOTALED_HARDWARE_RMT
#if !TASMOTALED_HARDWARE_SPI
  if (hw_type == TasmotaLed_SPI) {
    hw = TasmotaLed_HW_None;
  }
#endif // TASMOTALED_HARDWARE_SPI
#if !TASMOTALED_HARDWARE_I2S
  if (hw_type == TasmotaLed_I2S) {
    hw = TasmotaLed_HW_None;
  }
#endif // TASMOTALED_HARDWARE_I2S

  // Step 2. If TasmotaLed_HW_Default, find a suitable scheme, RMT preferred
#if TASMOTALED_HARDWARE_RMT
  if ((hw & 0xFF0000) == TasmotaLed_HW_Default) {
    hw = TasmotaLed_RMT;
  }
#endif // TASMOTALED_HARDWARE_RMT
#if TASMOTALED_HARDWARE_I2S
  if ((hw & 0xFF0000) == TasmotaLed_HW_Default) {
    hw = TasmotaLed_I2S;
  }
#endif // TASMOTALED_HARDWARE_I2S
#if TASMOTALED_HARDWARE_SPI
  if ((hw & 0xFF0000) == TasmotaLed_HW_Default) {
    hw = TasmotaLed_SPI;
  }
#endif // TASMOTALED_HARDWARE_SPI
  return hw;
}


TasmotaLEDPusher * TasmotaLEDPusher::Create(uint32_t hw, int8_t gpio) {
  TasmotaLEDPusher * pusher = nullptr;

  hw = TasmotaLEDPusher::ResolveHardware(hw);

  switch (hw & 0XFF0000) {
#if TASMOTALED_HARDWARE_RMT
    case TasmotaLed_RMT:
      pusher = new TasmotaLEDPusherRMT(gpio);
      AddLog(LOG_LEVEL_DEBUG, "LED: RMT gpio %i", gpio);
      break;
#endif // TASMOTALED_HARDWARE_RMT
#if TASMOTALED_HARDWARE_SPI
    case TasmotaLed_SPI:
      pusher = new TasmotaLEDPusherSPI(gpio);
      AddLog(LOG_LEVEL_DEBUG, "LED: SPI gpio %i", gpio);
      break;
#endif // TASMOTALED_HARDWARE_SPI
    default:
      break;
  }
  return pusher;
}

#endif // ESP32
