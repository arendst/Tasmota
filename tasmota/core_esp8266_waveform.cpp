/*
  esp8266_waveform - General purpose waveform generation and control,
                     supporting outputs on all pins in parallel.

  Copyright (c) 2018 Earle F. Philhower, III.  All rights reserved.

  The core idea is to have a programmable waveform generator with a unique
  high and low period (defined in microseconds).  TIMER1 is set to 1-shot
  mode and is always loaded with the time until the next edge of any live
  waveforms.

  Up to one waveform generator per pin supported.

  Each waveform generator is synchronized to the ESP cycle counter, not the
  timer.  This allows for removing interrupt jitter and delay as the counter
  always increments once per 80MHz clock.  Changes to a waveform are
  contiguous and only take effect on the next waveform transition,
  allowing for smooth transitions.

  This replaces older tone(), analogWrite(), and the Servo classes.

  Everywhere in the code where "cycles" is used, it means ESP.getCycleTime()
  cycles, not TIMER1 cycles (which may be 2 CPU clocks @ 160MHz).

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

#include <core_version.h>
#if defined(ARDUINO_ESP8266_RELEASE_2_6_1) || defined(ARDUINO_ESP8266_RELEASE_2_6_2) || defined(ARDUINO_ESP8266_RELEASE_2_6_3) || !defined(ARDUINO_ESP8266_RELEASE)
#warning **** Tasmota is using a patched PWM Arduino version as planned ****


#include <Arduino.h>
#include "ets_sys.h"
#include "core_esp8266_waveform.h"

extern "C" {

// Maximum delay between IRQs
#define MAXIRQUS (10000)

// Set/clear GPIO 0-15 by bitmask
#define SetGPIO(a) do { GPOS = a; } while (0)
#define ClearGPIO(a) do { GPOC = a; } while (0)

// Waveform generator can create tones, PWM, and servos
typedef struct {
  uint32_t nextServiceCycle;   // ESP cycle timer when a transition required
  uint32_t expiryCycle;        // For time-limited waveform, the cycle when this waveform must stop
  uint32_t nextTimeHighCycles; // Copy over low->high to keep smooth waveform
  uint32_t nextTimeLowCycles;  // Copy over high->low to keep smooth waveform
} Waveform;

static Waveform waveform[17];        // State of all possible pins
static volatile uint32_t waveformState = 0;   // Is the pin high or low, updated in NMI so no access outside the NMI code
static volatile uint32_t waveformEnabled = 0; // Is it actively running, updated in NMI so no access outside the NMI code

// Enable lock-free by only allowing updates to waveformState and waveformEnabled from IRQ service routine
static volatile uint32_t waveformToEnable = 0;  // Message to the NMI handler to start a waveform on a inactive pin
static volatile uint32_t waveformToDisable = 0; // Message to the NMI handler to disable a pin from waveform generation

static uint32_t (*timer1CB)() = NULL;


// Non-speed critical bits
#pragma GCC optimize ("Os")

static inline ICACHE_RAM_ATTR uint32_t GetCycleCount() {
  uint32_t ccount;
  __asm__ __volatile__("esync; rsr %0,ccount":"=a"(ccount));
  return ccount;
}

// Interrupt on/off control
static ICACHE_RAM_ATTR void timer1Interrupt();
static bool timerRunning = false;

static void initTimer() {
  timer1_disable();
  ETS_FRC_TIMER1_INTR_ATTACH(NULL, NULL);
  ETS_FRC_TIMER1_NMI_INTR_ATTACH(timer1Interrupt);
  timer1_enable(TIM_DIV1, TIM_EDGE, TIM_SINGLE);
  timerRunning = true;
}

static void ICACHE_RAM_ATTR deinitTimer() {
  ETS_FRC_TIMER1_NMI_INTR_ATTACH(NULL);
  timer1_disable();
  timer1_isr_init();
  timerRunning = false;
}

// Set a callback.  Pass in NULL to stop it
void setTimer1Callback(uint32_t (*fn)()) {
  timer1CB = fn;
  if (!timerRunning && fn) {
    initTimer();
    timer1_write(microsecondsToClockCycles(1)); // Cause an interrupt post-haste
  } else if (timerRunning && !fn && !waveformEnabled) {
    deinitTimer();
  }
}

// Start up a waveform on a pin, or change the current one.  Will change to the new
// waveform smoothly on next low->high transition.  For immediate change, stopWaveform()
// first, then it will immediately begin.
int startWaveform(uint8_t pin, uint32_t timeHighUS, uint32_t timeLowUS, uint32_t runTimeUS) {
  if ((pin > 16) || isFlashInterfacePin(pin)) {
    return false;
  }
  Waveform *wave = &waveform[pin];
  // Adjust to shave off some of the IRQ time, approximately
  wave->nextTimeHighCycles = microsecondsToClockCycles(timeHighUS);
  wave->nextTimeLowCycles = microsecondsToClockCycles(timeLowUS);
  wave->expiryCycle = runTimeUS ? GetCycleCount() + microsecondsToClockCycles(runTimeUS) : 0;
  if (runTimeUS && !wave->expiryCycle) {
    wave->expiryCycle = 1; // expiryCycle==0 means no timeout, so avoid setting it
  }

  uint32_t mask = 1<<pin;
  if (!(waveformEnabled & mask)) {
    // Actually set the pin high or low in the IRQ service to guarantee times
    wave->nextServiceCycle = GetCycleCount() + microsecondsToClockCycles(1);
    waveformToEnable |= mask;
    if (!timerRunning) {
      initTimer();
      timer1_write(microsecondsToClockCycles(10));
    } else {
      // Ensure timely service....
      if (T1L > microsecondsToClockCycles(10)) {
        timer1_write(microsecondsToClockCycles(10));
      }
    }
    while (waveformToEnable) {
      delay(0); // Wait for waveform to update
    }
  }

  return true;
}

// Speed critical bits
#pragma GCC optimize ("O2")
// Normally would not want two copies like this, but due to different
// optimization levels the inline attribute gets lost if we try the
// other version.

static inline ICACHE_RAM_ATTR uint32_t GetCycleCountIRQ() {
  uint32_t ccount;
  __asm__ __volatile__("rsr %0,ccount":"=a"(ccount));
  return ccount;
}

static inline ICACHE_RAM_ATTR uint32_t min_u32(uint32_t a, uint32_t b) {
  if (a < b) {
    return a;
  }
  return b;
}

static inline ICACHE_RAM_ATTR int32_t max_32(int32_t a, int32_t b) {
    if (a < b) {
        return b;
    }
    return a;
}

// Stops a waveform on a pin
int ICACHE_RAM_ATTR stopWaveform(uint8_t pin) {
  // Can't possibly need to stop anything if there is no timer active
  if (!timerRunning) {
    return false;
  }
  // If user sends in a pin >16 but <32, this will always point to a 0 bit
  // If they send >=32, then the shift will result in 0 and it will also return false
  uint32_t mask = 1<<pin;
  if (!(waveformEnabled & mask)) {
    return false; // It's not running, nothing to do here
  }
  waveformToDisable |= mask;
  // Ensure timely service....
  if (T1L > microsecondsToClockCycles(10)) {
    timer1_write(microsecondsToClockCycles(10));
  }
  while (waveformToDisable) {
    /* no-op */ // Can't delay() since stopWaveform may be called from an IRQ
  }
  if (!waveformEnabled && !timer1CB) {
    deinitTimer();
  }
  return true;
}

// The SDK and hardware take some time to actually get to our NMI code, so
// decrement the next IRQ's timer value by a bit so we can actually catch the
// real CPU cycle counter we want for the waveforms.
#if F_CPU == 80000000
  #define DELTAIRQ (microsecondsToClockCycles(3))
#else
  #define DELTAIRQ (microsecondsToClockCycles(2))
#endif


static ICACHE_RAM_ATTR void timer1Interrupt() {
  // Optimize the NMI inner loop by keeping track of the min and max GPIO that we
  // are generating.  In the common case (1 PWM) these may be the same pin and
  // we can avoid looking at the other pins.
  static int startPin = 0;
  static int endPin = 0;

  uint32_t nextEventCycles = microsecondsToClockCycles(MAXIRQUS);
  uint32_t timeoutCycle = GetCycleCountIRQ() + microsecondsToClockCycles(14);

  if (waveformToEnable || waveformToDisable) {
    // Handle enable/disable requests from main app.
    waveformEnabled = (waveformEnabled & ~waveformToDisable) | waveformToEnable; // Set the requested waveforms on/off
    waveformState &= ~waveformToEnable;  // And clear the state of any just started
    waveformToEnable = 0;
    waveformToDisable = 0;
    // Find the first GPIO being generated by checking GCC's find-first-set (returns 1 + the bit of the first 1 in an int32_t)
    startPin = __builtin_ffs(waveformEnabled) - 1;
    // Find the last bit by subtracting off GCC's count-leading-zeros (no offset in this one)
    endPin = 32 - __builtin_clz(waveformEnabled);
  }

  bool done = false;
  if (waveformEnabled) {
    do {
      nextEventCycles = microsecondsToClockCycles(MAXIRQUS);
      for (int i = startPin; i <= endPin; i++) {
        uint32_t mask = 1<<i;

        // If it's not on, ignore!
        if (!(waveformEnabled & mask)) {
          continue;
        }

        Waveform *wave = &waveform[i];
        uint32_t now = GetCycleCountIRQ();

        // Disable any waveforms that are done
        if (wave->expiryCycle) {
          int32_t expiryToGo = wave->expiryCycle - now;
          if (expiryToGo < 0) {
              // Done, remove!
              waveformEnabled &= ~mask;
              if (i == 16) {
                GP16O &= ~1;
              } else {
                ClearGPIO(mask);
              }
              continue;
            }
        }

        // Check for toggles
        int32_t cyclesToGo = wave->nextServiceCycle - now;
        if (cyclesToGo < 0) {
          // See #7057
          // The following is a no-op unless we have overshot by an entire waveform cycle.
          // As modulus is an expensive operation, this code is removed for now:
          // cyclesToGo = -((-cyclesToGo) % (wave->nextTimeHighCycles + wave->nextTimeLowCycles));
          //
          // Alternative version with lower CPU impact:
          // while (-cyclesToGo > wave->nextTimeHighCycles + wave->nextTimeLowCycles) { cyclesToGo += wave->nextTimeHighCycles + wave->nextTimeLowCycles); }
          //
          // detect interrupt storm, for example during wifi connection.
          // if we overshoot the cycle by more than 25%, we forget phase and keep PWM duration
          int32_t overshoot = (-cyclesToGo) > ((wave->nextTimeHighCycles + wave->nextTimeLowCycles) >> 2);
          waveformState ^= mask;
          if (waveformState & mask) {
            if (i == 16) {
              GP16O |= 1; // GPIO16 write slow as it's RMW
            } else {
              SetGPIO(mask);
            }
            if (overshoot) {
              wave->nextServiceCycle = now + wave->nextTimeHighCycles;
              nextEventCycles = min_u32(nextEventCycles, wave->nextTimeHighCycles);
            } else {
              wave->nextServiceCycle += wave->nextTimeHighCycles;
              nextEventCycles = min_u32(nextEventCycles, max_32(wave->nextTimeHighCycles + cyclesToGo, microsecondsToClockCycles(1)));
            }
          } else {
            if (i == 16) {
              GP16O &= ~1; // GPIO16 write slow as it's RMW
            } else {
              ClearGPIO(mask);
            }
            if (overshoot) {
              wave->nextServiceCycle = now + wave->nextTimeLowCycles;
              nextEventCycles = min_u32(nextEventCycles, wave->nextTimeLowCycles);
            } else {
              wave->nextServiceCycle += wave->nextTimeLowCycles;
              nextEventCycles = min_u32(nextEventCycles, max_32(wave->nextTimeLowCycles + cyclesToGo, microsecondsToClockCycles(1)));
            }
          }
        } else {
          uint32_t deltaCycles = wave->nextServiceCycle - now;
          nextEventCycles = min_u32(nextEventCycles, deltaCycles);
        }
      }

      // Exit the loop if we've hit the fixed runtime limit or the next event is known to be after that timeout would occur
      uint32_t now = GetCycleCountIRQ();
      int32_t cycleDeltaNextEvent = timeoutCycle - (now + nextEventCycles);
      int32_t cyclesLeftTimeout = timeoutCycle - now;
      done = (cycleDeltaNextEvent < 0) || (cyclesLeftTimeout < 0);
    } while (!done);
  } // if (waveformEnabled)

  if (timer1CB) {
    nextEventCycles = min_u32(nextEventCycles, timer1CB());
  }

  if (nextEventCycles < microsecondsToClockCycles(10)) {
    nextEventCycles = microsecondsToClockCycles(10);
  }
  nextEventCycles -= DELTAIRQ;

  // Do it here instead of global function to save time and because we know it's edge-IRQ
#if F_CPU == 160000000
  T1L = nextEventCycles >> 1; // Already know we're in range by MAXIRQUS
#else
  T1L = nextEventCycles; // Already know we're in range by MAXIRQUS
#endif
  TEIE |= TEIE1; // Edge int enable
}

};

#endif  // ARDUINO_ESP8266_RELEASE