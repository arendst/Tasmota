/*
  esp8266_waveform - General purpose waveform generation and control,
                     supporting outputs on all pins in parallel.

  Copyright (c) 2018 Earle F. Philhower, III.  All rights reserved.

  The core idea is to have a programmable waveform generator with a unique
  high and low period (defined in microseconds or CPU clock cycles).  TIMER1
  is set to 1-shot mode and is always loaded with the time until the next
  edge of any live waveforms.

  Up to one waveform generator per pin supported.

  Each waveform generator is synchronized to the ESP clock cycle counter, not
  the timer.  This allows for removing interrupt jitter and delay as the
  counter always increments once per 80MHz clock.  Changes to a waveform are
  contiguous and only take effect on the next waveform transition,
  allowing for smooth transitions.

  This replaces older tone(), analogWrite(), and the Servo classes.

  Everywhere in the code where "cycles" is used, it means ESP.getCycleCount()
  clock cycle count, or an interval measured in CPU clock cycles, but not
  TIMER1 cycles (which may be 2 CPU clock cycles @ 160MHz).

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

#ifdef ESP8266

#include <Arduino.h>
#include "ets_sys.h"
#include "core_esp8266_waveform.h"
#include "user_interface.h"
extern "C" {

// Internal-only calls, not for applications
extern void _setPWMPeriodCC(uint32_t cc);
extern bool _stopPWM(int pin);
extern bool _setPWM(int pin, uint32_t cc);
extern int startWaveformClockCycles(uint8_t pin, uint32_t timeHighCycles, uint32_t timeLowCycles, uint32_t runTimeCycles);

// Maximum delay between IRQs
#define MAXIRQUS (10000)

// Set/clear GPIO 0-15 by bitmask
#define SetGPIO(a) do { GPOS = a; } while (0)
#define ClearGPIO(a) do { GPOC = a; } while (0)

// Waveform generator can create tones, PWM, and servos
typedef struct {
  uint32_t nextServiceCycle;   // ESP cycle timer when a transition required
  uint32_t expiryCycle;        // For time-limited waveform, the cycle when this waveform must stop
  uint32_t timeHighCycles;     // Currently running waveform period
  uint32_t timeLowCycles;      //
  uint32_t desiredHighCycles;     // Currently running waveform period
  uint32_t desiredLowCycles;      //
  uint32_t gotoTimeHighCycles; // Copied over on the next period to preserve phase
  uint32_t gotoTimeLowCycles;  //
  uint32_t lastEdge; //
} Waveform;

class WVFState {
public:
  Waveform waveform[17];        // State of all possible pins
  uint32_t waveformState = 0;   // Is the pin high or low, updated in NMI so no access outside the NMI code
  uint32_t waveformEnabled = 0; // Is it actively running, updated in NMI so no access outside the NMI code

  // Enable lock-free by only allowing updates to waveformState and waveformEnabled from IRQ service routine
  uint32_t waveformToEnable = 0;  // Message to the NMI handler to start a waveform on a inactive pin
  uint32_t waveformToDisable = 0; // Message to the NMI handler to disable a pin from waveform generation

  int32_t waveformToChange = -1;
  uint32_t waveformNewHigh = 0;
  uint32_t waveformNewLow = 0;

  uint32_t (*timer1CB)() = NULL;

  // Optimize the NMI inner loop by keeping track of the min and max GPIO that we
  // are generating.  In the common case (1 PWM) these may be the same pin and
  // we can avoid looking at the other pins.
  int startPin = 0;
  int endPin = 0;
};
static WVFState wvfState;


// Ensure everything is read/written to RAM
#define MEMBARRIER() { __asm__ volatile("" ::: "memory"); }

// Non-speed critical bits
#pragma GCC optimize ("Os")

// Interrupt on/off control
static ICACHE_RAM_ATTR void timer1Interrupt();
static bool timerRunning = false;

static void initTimer() {
  if (!timerRunning) {
    timer1_disable();
    ETS_FRC_TIMER1_INTR_ATTACH(NULL, NULL);
    ETS_FRC_TIMER1_NMI_INTR_ATTACH(timer1Interrupt);
    timer1_enable(TIM_DIV1, TIM_EDGE, TIM_SINGLE);
    timerRunning = true;
    timer1_write(microsecondsToClockCycles(10));
  }
}

static ICACHE_RAM_ATTR void forceTimerInterrupt() {
  if (T1L > microsecondsToClockCycles(10)) {
    T1L = microsecondsToClockCycles(10);
  }
}

// PWM implementation using special purpose state machine
//
// Keep an ordered list of pins with the delta in cycles between each
// element, with a terminal entry making up the remainder of the PWM
// period.  With this method sum(all deltas) == PWM period clock cycles.
//
// At t=0 set all pins high and set the timeout for the 1st edge.
// On interrupt, if we're at the last element reset to t=0 state
// Otherwise, clear that pin down and set delay for next element
// and so forth.

constexpr int maxPWMs = 8;

// PWM machine state
typedef struct {
  uint32_t mask; // Bitmask of active pins
  uint32_t cnt;  // How many entries
  uint32_t idx;  // Where the state machine is along the list
  uint8_t  pin[maxPWMs + 1];
  uint32_t delta[maxPWMs + 1];
  uint32_t nextServiceCycle;  // Clock cycle for next step
} PWMState;

static PWMState pwmState;
static PWMState *pwmUpdate = nullptr; // Set by main code, cleared by ISR
static uint32_t pwmPeriod = microsecondsToClockCycles(1000000UL) / 1000;



static ICACHE_RAM_ATTR void disableIdleTimer() {
 if (timerRunning && !wvfState.waveformEnabled && !pwmState.cnt && !wvfState.timer1CB) {
    ETS_FRC_TIMER1_NMI_INTR_ATTACH(NULL);
    timer1_disable();
    timer1_isr_init();
    timerRunning = false;
  }
}


// Called when analogWriteFreq() changed to update the PWM total period
void _setPWMPeriodCC(uint32_t cc) {
  if (cc == pwmPeriod) {
    return;
  }
  if (pwmState.cnt) {
    // Adjust any running ones to the best of our abilities by scaling them
    // Used FP math for speed and code size
    uint64_t oldCC64p0 = ((uint64_t)pwmPeriod);
    uint64_t newCC64p16 = ((uint64_t)cc) << 16;
    uint64_t ratio64p16 = (newCC64p16 / oldCC64p0);
    PWMState p;  // The working copy since we can't edit the one in use
    p = pwmState;
    uint32_t ttl = 0;
    for (uint32_t i = 0; i < p.cnt; i++) {
      uint64_t val64p16 = ((uint64_t)p.delta[i]) << 16;
      uint64_t newVal64p32 = val64p16 * ratio64p16;
      p.delta[i] = newVal64p32 >> 32;
      ttl += p.delta[i];
    }
    p.delta[p.cnt] = cc - ttl; // Final cleanup exactly cc total cycles
    // Update and wait for mailbox to be emptied
    pwmUpdate = &p;
    MEMBARRIER();
    forceTimerInterrupt();
    while (pwmUpdate) {
      delay(0);
      // No mem barrier.  The external function call guarantees it's re-read
    }
  }
  pwmPeriod = cc;
}

// Helper routine to remove an entry from the state machine
static ICACHE_RAM_ATTR void _removePWMEntry(int pin, PWMState *p) {
  uint32_t i;

  // Find the pin to pull out...
  for (i = 0; p->pin[i] != pin; i++) { /* no-op */ }
  auto delta = p->delta[i];

  // Add the removed previous pin delta to preserve absolute position
  p->delta[i+1] += delta;

  // Move everything back one
  for (i++; i <= p->cnt; i++) {
    p->pin[i-1] = p->pin[i];
    p->delta[i-1] = p->delta[i];
  }
  // Remove the pin from the active list
  p->mask &= ~(1<<pin);
  p->cnt--;
}

// Called by analogWrite(0/100%) to disable PWM on a specific pin
ICACHE_RAM_ATTR bool _stopPWM(int pin) {
  if (!((1<<pin) & pwmState.mask)) {
    return false; // Pin not actually active
  }

  PWMState p;  // The working copy since we can't edit the one in use
  p = pwmState;
  _removePWMEntry(pin, &p);

  // Update and wait for mailbox to be emptied
  pwmUpdate = &p;
  MEMBARRIER();
  forceTimerInterrupt();
  while (pwmUpdate) {
    MEMBARRIER();
    /* Busy wait, could be in ISR */
  }
  // Possibly shut down the timer completely if we're done
  disableIdleTimer();
  return true;
}

// Called by analogWrite(1...99%) to set the PWM duty in clock cycles
bool _setPWM(int pin, uint32_t cc) {
  stopWaveform(pin);
  PWMState p;  // Working copy
  p = pwmState;
  // Get rid of any entries for this pin
  if ((1<<pin) & p.mask) {
    _removePWMEntry(pin, &p);
  }
  // And add it to the list, in order
  if (p.cnt >= maxPWMs) {
    return false; // No space left
  } else if (p.cnt == 0) {
    // Starting up from scratch, special case 1st element and PWM period
    p.pin[0] = pin;
    p.delta[0] = cc;
    p.pin[1] = 0xff;
    p.delta[1] = pwmPeriod - cc;
    p.cnt = 1;
    p.mask = 1<<pin;
  } else {
    uint32_t ttl=0;
    uint32_t i;
    // Skip along until we're at the spot to insert
    for (i=0; (i <= p.cnt) && (ttl + p.delta[i] < cc); i++) {
      ttl += p.delta[i];
    }
    // Shift everything out by one to make space for new edge
    memmove(&p.pin[i + 1], &p.pin[i], (1 + p.cnt - i) * sizeof(p.pin[0]));
    memmove(&p.delta[i + 1], &p.delta[i], (1 + p.cnt - i) * sizeof(p.delta[0]));
    int off = cc - ttl; // The delta from the last edge to the one we're inserting
    p.pin[i] = pin;
    p.delta[i] = off; // Add the delta to this new pin
    p.delta[i + 1] -= off; // And subtract it from the follower to keep sum(deltas) constant
    p.cnt++;
    p.mask |= 1<<pin;
  }

  // Set mailbox and wait for ISR to copy it over
  pwmUpdate = &p;
  MEMBARRIER();
  initTimer();
  forceTimerInterrupt();
  while (pwmUpdate) {
    delay(0);
  }
  return true;
}

// Start up a waveform on a pin, or change the current one.  Will change to the new
// waveform smoothly on next low->high transition.  For immediate change, stopWaveform()
// first, then it will immediately begin.
int startWaveform(uint8_t pin, uint32_t timeHighUS, uint32_t timeLowUS, uint32_t runTimeUS) {
  return startWaveformClockCycles(pin, microsecondsToClockCycles(timeHighUS), microsecondsToClockCycles(timeLowUS), microsecondsToClockCycles(runTimeUS));
}

int startWaveformClockCycles(uint8_t pin, uint32_t timeHighCycles, uint32_t timeLowCycles, uint32_t runTimeCycles) {
   if ((pin > 16) || isFlashInterfacePin(pin)) {
    return false;
  }
  Waveform *wave = &wvfState.waveform[pin];
  wave->expiryCycle = runTimeCycles ? ESP.getCycleCount() + runTimeCycles : 0;
  if (runTimeCycles && !wave->expiryCycle) {
    wave->expiryCycle = 1; // expiryCycle==0 means no timeout, so avoid setting it
  }

  _stopPWM(pin); // Make sure there's no PWM live here

  uint32_t mask = 1<<pin;
  MEMBARRIER();
  if (wvfState.waveformEnabled & mask) {
    wvfState.waveformNewHigh = timeHighCycles;
    wvfState.waveformNewLow = timeLowCycles;
    MEMBARRIER();
    wvfState.waveformToChange = pin;
    while (wvfState.waveformToChange >= 0) {
      delay(0); // Wait for waveform to update
      // No mem barrier here, the call to a global function implies global state updated
    }
  } else { //  if (!(wvfState.waveformEnabled & mask)) {
    wave->timeHighCycles = timeHighCycles;
    wave->timeLowCycles = timeLowCycles;
    wave->desiredHighCycles = wave->timeHighCycles;
    wave->desiredLowCycles = wave->timeLowCycles;
    wave->lastEdge = 0;
    wave->gotoTimeHighCycles = wave->timeHighCycles;
    wave->gotoTimeLowCycles = wave->timeLowCycles;    // Actually set the pin high or low in the IRQ service to guarantee times
    wave->nextServiceCycle = ESP.getCycleCount() + microsecondsToClockCycles(1);
    wvfState.waveformToEnable |= mask;
    MEMBARRIER();
    initTimer();
    forceTimerInterrupt();
    while (wvfState.waveformToEnable) {
      delay(0); // Wait for waveform to update
      // No mem barrier here, the call to a global function implies global state updated
    }
  }

  return true;
}


// Set a callback.  Pass in NULL to stop it
void setTimer1Callback(uint32_t (*fn)()) {
  wvfState.timer1CB = fn;
  if (fn) {
    initTimer();
    forceTimerInterrupt();
  }
  disableIdleTimer();
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

// Stops a waveform on a pin
int ICACHE_RAM_ATTR stopWaveform(uint8_t pin) {
  // Can't possibly need to stop anything if there is no timer active
  if (!timerRunning) {
    return false;
  }
  // If user sends in a pin >16 but <32, this will always point to a 0 bit
  // If they send >=32, then the shift will result in 0 and it will also return false
  if (wvfState.waveformEnabled & (1UL << pin)) {
    wvfState.waveformToDisable = 1UL << pin;
    forceTimerInterrupt();
    while (wvfState.waveformToDisable) {
      MEMBARRIER(); // If it wasn't written yet, it has to be by now
      /* no-op */ // Can't delay() since stopWaveform may be called from an IRQ
    }
  }
  disableIdleTimer();
  return true;
}

// The SDK and hardware take some time to actually get to our NMI code, so
// decrement the next IRQ's timer value by a bit so we can actually catch the
// real CPU cycle counter we want for the waveforms.

// The SDK also sometimes is running at a different speed the the Arduino core
// so the ESP cycle counter is actually running at a variable speed.
// adjust(x) takes care of adjusting a delta clock cycle amount accordingly.
#if F_CPU == 80000000
  #define DELTAIRQ (microsecondsToClockCycles(3))
  #define adjust(x) ((x) << (turbo ? 1 : 0))
#else
  #define DELTAIRQ (microsecondsToClockCycles(2))
  #define adjust(x) ((x) >> (turbo ? 0 : 1))
#endif

#define ENABLE_ADJUST   // Adjust takes 36 bytes
#define ENABLE_FEEDBACK // Feedback costs 68 bytes
#define ENABLE_PWM      // PWM takes 160 bytes

#ifndef ENABLE_ADJUST
  #undef adjust
  #define adjust(x) (x)
#endif


static ICACHE_RAM_ATTR void timer1Interrupt() {
  // Flag if the core is at 160 MHz, for use by adjust()
  bool turbo = (*(uint32_t*)0x3FF00014) & 1 ? true : false;

  uint32_t nextEventCycles = microsecondsToClockCycles(MAXIRQUS);
  uint32_t timeoutCycle = GetCycleCountIRQ() + microsecondsToClockCycles(14);

  if (wvfState.waveformToEnable || wvfState.waveformToDisable) {
    // Handle enable/disable requests from main app
    wvfState.waveformEnabled = (wvfState.waveformEnabled & ~wvfState.waveformToDisable) | wvfState.waveformToEnable; // Set the requested waveforms on/off
    wvfState.waveformState &= ~wvfState.waveformToEnable;  // And clear the state of any just started
    wvfState.waveformToEnable = 0;
    wvfState.waveformToDisable = 0;
    // No mem barrier.  Globals must be written to RAM on ISR exit.
    // Find the first GPIO being generated by checking GCC's find-first-set (returns 1 + the bit of the first 1 in an int32_t)
    wvfState.startPin = __builtin_ffs(wvfState.waveformEnabled) - 1;
    // Find the last bit by subtracting off GCC's count-leading-zeros (no offset in this one)
    wvfState.endPin = 32 - __builtin_clz(wvfState.waveformEnabled);
#ifdef ENABLE_PWM
  } else if (!pwmState.cnt && pwmUpdate) {
    // Start up the PWM generator by copying from the mailbox
    pwmState.cnt = 1;
    pwmState.idx = 1; // Ensure copy this cycle, cause it to start at t=0
    pwmState.nextServiceCycle = GetCycleCountIRQ(); // Do it this loop!
    // No need for mem barrier here.  Global must be written by IRQ exit
#endif
  } else if (wvfState.waveformToChange >= 0) {
    wvfState.waveform[wvfState.waveformToChange].gotoTimeHighCycles = wvfState.waveformNewHigh;
    wvfState.waveform[wvfState.waveformToChange].gotoTimeLowCycles = wvfState.waveformNewLow;
    wvfState.waveformToChange = -1;
    // No need for memory barrier here.  The global has to be written before exit the ISR.
  }

  bool done = false;
  if (wvfState.waveformEnabled || pwmState.cnt) {
    do {
      nextEventCycles = microsecondsToClockCycles(MAXIRQUS);

#ifdef ENABLE_PWM
      // PWM state machine implementation
      if (pwmState.cnt) {
        uint32_t now = GetCycleCountIRQ();
        int32_t cyclesToGo = pwmState.nextServiceCycle - now;
        if (cyclesToGo < 0) {
            if (pwmState.idx == pwmState.cnt) { // Start of pulses, possibly copy new
                if (pwmUpdate) {
                    // Do the memory copy from temp to global and clear mailbox
                    pwmState = *(PWMState*)pwmUpdate;
                    pwmUpdate = nullptr;
                }
                GPOS = pwmState.mask; // Set all active pins high
                // GPIO16 isn't the same as the others
                if (pwmState.mask & (1<<16)) {
                  GP16O = 1;
                }
                pwmState.idx = 0;
            } else {
                do {
                    // Drop the pin at this edge
                    GPOC = 1<<pwmState.pin[pwmState.idx];
                    // GPIO16 still needs manual work
                    if (pwmState.pin[pwmState.idx] == 16) {
                      GP16O = 0;
                    }
                    pwmState.idx++;
                    // Any other pins at this same PWM value will have delta==0, drop them too.
                } while (pwmState.delta[pwmState.idx] == 0);
            }
            // Preserve duty cycle over PWM period by using now+xxx instead of += delta
            cyclesToGo = adjust(pwmState.delta[pwmState.idx]);
            pwmState.nextServiceCycle = GetCycleCountIRQ() + cyclesToGo;
        }
        nextEventCycles = min_u32(nextEventCycles, cyclesToGo);
      }
#endif

      for (auto i = wvfState.startPin; i <= wvfState.endPin; i++) {
        uint32_t mask = 1<<i;

        // If it's not on, ignore!
        if (!(wvfState.waveformEnabled & mask)) {
          continue;
        }

        Waveform *wave = &wvfState.waveform[i];
        uint32_t now = GetCycleCountIRQ();

        // Disable any waveforms that are done
        if (wave->expiryCycle) {
          int32_t expiryToGo = wave->expiryCycle - now;
          if (expiryToGo < 0) {
              // Done, remove!
              wvfState.waveformEnabled &= ~mask;
              if (i == 16) {
                GP16O = 0;
              } else {
                ClearGPIO(mask);
              }
              continue;
            }
        }

        // Check for toggles
        int32_t cyclesToGo = wave->nextServiceCycle - now;
        if (cyclesToGo < 0) {
          uint32_t nextEdgeCycles;
          uint32_t desired = 0;
          uint32_t *timeToUpdate;
          wvfState.waveformState ^= mask;
          if (wvfState.waveformState & mask) {
            if (i == 16) {
              GP16O = 1; // GPIO16 write slow as it's RMW
            } else {
              SetGPIO(mask);
            }
            if (wave->gotoTimeHighCycles) {
              // Copy over next full-cycle timings
              wave->timeHighCycles = wave->gotoTimeHighCycles;
              wave->desiredHighCycles = wave->gotoTimeHighCycles;
              wave->timeLowCycles = wave->gotoTimeLowCycles;
              wave->desiredLowCycles = wave->gotoTimeLowCycles;
              wave->gotoTimeHighCycles = 0;
            } else {
#ifdef ENABLE_FEEDBACK
              if (wave->lastEdge) {
                desired = wave->desiredLowCycles;
                timeToUpdate = &wave->timeLowCycles;
              }
            }
#endif
            nextEdgeCycles = wave->timeHighCycles;
          } else {
            if (i == 16) {
              GP16O = 0; // GPIO16 write slow as it's RMW
            } else {
              ClearGPIO(mask);
            }
#ifdef ENABLE_FEEDBACK
            desired = wave->desiredHighCycles;
            timeToUpdate = &wave->timeHighCycles;
#endif
            nextEdgeCycles = wave->timeLowCycles;
          }
#ifdef ENABLE_FEEDBACK
          if (desired) {
            desired = adjust(desired);
            int32_t err = desired - (now - wave->lastEdge);
            if (abs(err) < desired) { // If we've lost > the entire phase, ignore this error signal
                err /= 2;
                *timeToUpdate += err;
            }
          }
#endif
          nextEdgeCycles = adjust(nextEdgeCycles);
          wave->nextServiceCycle = now + nextEdgeCycles;
          nextEventCycles = min_u32(nextEventCycles, nextEdgeCycles);
          wave->lastEdge = now;
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
  } // if (wvfState.waveformEnabled)

  if (wvfState.timer1CB) {
    nextEventCycles = min_u32(nextEventCycles, wvfState.timer1CB());
  }

  if (nextEventCycles < microsecondsToClockCycles(5)) {
    nextEventCycles = microsecondsToClockCycles(5);
  }
  nextEventCycles -= DELTAIRQ;

  // Do it here instead of global function to save time and because we know it's edge-IRQ
  T1L = nextEventCycles >> (turbo ? 1 : 0);
  TEIE |= TEIE1; // Edge int enable
}

};

#endif  // ESP8266