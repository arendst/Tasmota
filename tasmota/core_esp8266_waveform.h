/*
  esp8266_waveform - General purpose waveform generation and control,
                     supporting outputs on all pins in parallel.

  Copyright (c) 2018 Earle F. Philhower, III.  All rights reserved.
  Copyright (c) 2020 Dirk O. Kaar.

  The core idea is to have a programmable waveform generator with a unique
  high and low period (defined in microseconds or CPU clock cycles).  TIMER1 is
  set to 1-shot mode and is always loaded with the time until the next edge
  of any live waveforms.

  Up to one waveform generator per pin supported.

  Each waveform generator is synchronized to the ESP clock cycle counter, not the
  timer.  This allows for removing interrupt jitter and delay as the counter
  always increments once per 80MHz clock.  Changes to a waveform are
  contiguous and only take effect on the next waveform transition,
  allowing for smooth transitions.

  This replaces older tone(), analogWrite(), and the Servo classes.

  Everywhere in the code where "ccy" or "ccys" is used, it means ESP.getCycleCount()
  clock cycle count, or an interval measured in CPU clock cycles, but not TIMER1
  cycles (which may be 2 CPU clock cycles @ 160MHz).

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include <Arduino.h>

#ifndef __ESP8266_WAVEFORM_H
#define __ESP8266_WAVEFORM_H

#ifdef __cplusplus
extern "C" {
#endif

// Start or change a waveform of the specified high and low times on specific pin.
// If runtimeUS > 0 then automatically stop it after that many usecs, relative to the next
// full period.
// If waveform is not yet started on pin, and on pin == alignPhase a waveform is running,
// the new waveform is started at phaseOffsetUS phase offset, in microseconds, to that.
// Setting autoPwm to true allows the wave generator to maintain PWM duty to idle cycle ratio
// under load, for applications where frequency or duty cycle must not change, leave false.
// Returns true or false on success or failure.
int startWaveform(uint8_t pin, uint32_t timeHighUS, uint32_t timeLowUS,
  uint32_t runTimeUS = 0, int8_t alignPhase = -1, uint32_t phaseOffsetUS = 0, bool autoPwm = false);
// Start or change a waveform of the specified high and low CPU clock cycles on specific pin.
// If runtimeCycles > 0 then automatically stop it after that many CPU clock cycles, relative to the next
// full period.
// If waveform is not yet started on pin, and on pin == alignPhase a waveform is running,
// the new waveform is started at phaseOffsetCcys phase offset, in CPU clock cycles, to that.
// Setting autoPwm to true allows the wave generator to maintain PWM duty to idle cycle ratio
// under load, for applications where frequency or duty cycle must not change, leave false.
// Returns true or false on success or failure.
int startWaveformClockCycles(uint8_t pin, uint32_t timeHighCcys, uint32_t timeLowCcys,
  uint32_t runTimeCcys = 0, int8_t alignPhase = -1, uint32_t phaseOffsetCcys = 0, bool autoPwm = false);
// Stop a waveform, if any, on the specified pin.
// Returns true or false on success or failure.
int stopWaveform(uint8_t pin);

// Add a callback function to be called on *EVERY* timer1 trigger.  The
// callback returns the number of microseconds until the next desired call.
// However, since it is called every timer1 interrupt, it may be called
// again before this period.  It should therefore use the ESP Cycle Counter
// to determine whether or not to perform an operation.
// Pass in NULL to disable the callback and, if no other waveforms being
// generated, stop the timer as well.
// Make sure the CB function has the ICACHE_RAM_ATTR decorator.
void setTimer1Callback(uint32_t (*fn)());

#ifdef __cplusplus
}
#endif

#endif
