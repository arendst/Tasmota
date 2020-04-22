/*
  esp8266_waveform - General purpose waveform generation and control,
                     supporting outputs on all pins in parallel.

  Copyright (c) 2018 Earle F. Philhower, III.  All rights reserved.

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

  Everywhere in the code where "cycles" is used, it means ESP.getCycleCount()
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
  uint32_t gotoTimeHighCycles; // Copied over on the next period to preserve phase
  uint32_t gotoTimeLowCycles;  //
} Waveform;

static Waveform waveform[17];        // State of all possible pins
static volatile uint32_t waveformState = 0;   // Is the pin high or low, updated in NMI so no access outside the NMI code
static volatile uint32_t waveformEnabled = 0; // Is it actively running, updated in NMI so no access outside the NMI code

// Enable lock-free by only allowing updates to waveformState and waveformEnabled from IRQ service routine
static volatile uint32_t waveformToEnable = 0;  // Message to the NMI handler to start a waveform on a inactive pin
static volatile uint32_t waveformToDisable = 0; // Message to the NMI handler to disable a pin from waveform generation

volatile int32_t waveformToChange = -1;
volatile uint32_t waveformNewHigh = 0;
volatile uint32_t waveformNewLow = 0;

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

// PWM edge definition
typedef struct {
  unsigned int pin   : 8;
  unsigned int delta : 24;
} PWMEntry;

// PWM machine state
typedef struct {
  uint32_t mask; // Bitmask of active pins
  uint8_t cnt;   // How many entries
  uint8_t idx;   // Where the state machine is along the list
  PWMEntry edge[maxPWMs + 1]; // Include space for terminal element
  uint32_t nextServiceCycle;  // Clock cycle for next step
} PWMState;

static PWMState pwmState;
static volatile PWMState *pwmUpdate = nullptr; // Set by main code, cleared by ISR
static uint32_t pwmPeriod = (1000000L * system_get_cpu_freq()) / 1000;

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
    for (auto i = 0; i < p.cnt; i++) {
      uint64_t val64p16 = ((uint64_t)p.edge[i].delta) << 16;
      uint64_t newVal64p32 = val64p16 * ratio64p16;
      p.edge[i].delta = newVal64p32 >> 32;
      ttl += p.edge[i].delta;
    }
    p.edge[p.cnt].delta = cc - ttl; // Final cleanup exactly cc total cycles
    // Update and wait for mailbox to be emptied
    pwmUpdate = &p;
    while (pwmUpdate) {
      delay(0);
    }
  }
  pwmPeriod = cc;
}

// Helper routine to remove an entry from the state machine
static void _removePWMEntry(int pin, PWMState *p) {
  if (!((1<<pin) & p->mask)) {
    return;
  }

  int delta = 0;
  int i;
  for (i=0; i < p->cnt; i++) {
    if (p->edge[i].pin == pin) {
      delta = p->edge[i].delta;
      break;
    }
  }
  // Add the removed previous pin delta to preserve absolute position
  p->edge[i+1].delta += delta;
  // Move everything back one and clean up
  for (i++; i <= p->cnt; i++) {
    p->edge[i-1] = p->edge[i];
  }
  p->mask &= ~(1<<pin);
  p->cnt--;
}

// Called by analogWrite(0/100%) to disable PWM on a specific pin
bool _stopPWM(int pin) {
  if (!((1<<pin) & pwmState.mask)) {
    return false; // Pin not actually active
  }
  
  PWMState p;  // The working copy since we can't edit the one in use
  p = pwmState;
  _removePWMEntry(pin, &p);
  // Update and wait for mailbox to be emptied
  pwmUpdate = &p;
  while (pwmUpdate) {
    delay(0);
  }
  // Possibly shut doen the timer completely if we're done
  if (!waveformEnabled && !pwmState.cnt && !timer1CB) {
    deinitTimer();
  }
  return true;
}

// Called by analogWrite(1...99%) to set the PWM duty in clock cycles
bool _setPWM(int pin, uint32_t cc) {
  PWMState p;  // Working copy
  p = pwmState;
  // Get rid of any entries for this pin
  _removePWMEntry(pin, &p);
  // And add it to the list, in order
  if (p.cnt >= maxPWMs) {
    return false; // No space left
  } else if (p.cnt == 0) {
    // Starting up from scratch, special case 1st element and PWM period
    p.edge[0].pin = pin;
    p.edge[0].delta = cc;
    p.edge[1].pin = 0xff;
    p.edge[1].delta = pwmPeriod - cc;
    p.cnt = 1;
    p.mask = 1<<pin;
  } else {
    uint32_t ttl=0;
    uint32_t i;
    // Skip along until we're at the spot to insert
    for (i=0; (i <= p.cnt) && (ttl + p.edge[i].delta < cc); i++) {
      ttl += p.edge[i].delta;
    }
    // Shift everything out by one to make space for new edge
    memmove(&p.edge[i + 1], &p.edge[i], (1 + p.cnt - i) * sizeof(p.edge[0]));
    int off = cc - ttl; // The delta from the last edge to the one we're inserting
    p.edge[i].pin = pin;
    p.edge[i].delta = off; // Add the delta to this new pin
    p.edge[i + 1].delta -= off; // And subtract it from the follower to keep sum(deltas) constant
    p.cnt++;
    p.mask |= 1<<pin;
  }
  // Set mailbox and wait for ISR to copy it over
  pwmUpdate = &p;
  if (!timerRunning) {
    initTimer();
    timer1_write(microsecondsToClockCycles(10));
  }
  while (pwmUpdate) { delay(0); }
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
  Waveform *wave = &waveform[pin];
  wave->expiryCycle = runTimeCycles ? GetCycleCount() + runTimeCycles : 0;
  if (runTimeCycles && !wave->expiryCycle) {
    wave->expiryCycle = 1; // expiryCycle==0 means no timeout, so avoid setting it
  }

  uint32_t mask = 1<<pin;
  if (waveformEnabled & mask) {
    waveformNewHigh = timeHighCycles;
    waveformNewLow = timeLowCycles;
    waveformToChange = pin;
    while (waveformToChange >= 0) {
      delay(0); // Wait for waveform to update
    }
  } else { //  if (!(waveformEnabled & mask)) {
    wave->timeHighCycles = timeHighCycles;
    wave->timeLowCycles = timeLowCycles;
    wave->gotoTimeHighCycles = wave->timeHighCycles;
    wave->gotoTimeLowCycles = wave->timeLowCycles;    // Actually set the pin high or low in the IRQ service to guarantee times
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
  if (!waveformEnabled && !pwmState.cnt && !timer1CB) {
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
    // Handle enable/disable requests from main app
    waveformEnabled = (waveformEnabled & ~waveformToDisable) | waveformToEnable; // Set the requested waveforms on/off
    waveformState &= ~waveformToEnable;  // And clear the state of any just started
    waveformToEnable = 0;
    waveformToDisable = 0;
    // Find the first GPIO being generated by checking GCC's find-first-set (returns 1 + the bit of the first 1 in an int32_t)
    startPin = __builtin_ffs(waveformEnabled) - 1;
    // Find the last bit by subtracting off GCC's count-leading-zeros (no offset in this one)
    endPin = 32 - __builtin_clz(waveformEnabled);
  } else if (!pwmState.cnt && pwmUpdate) {
    // Start up the PWM generator by copying from the mailbox
    pwmState = *(PWMState*)pwmUpdate;
    pwmUpdate = nullptr;
    pwmState.nextServiceCycle = GetCycleCountIRQ(); // Do it this loop!
    pwmState.idx = pwmState.cnt; // Cause it to start at t=0
  } else if (waveformToChange >=0) {
    waveform[waveformToChange].gotoTimeHighCycles = waveformNewHigh;
    waveform[waveformToChange].gotoTimeLowCycles = waveformNewLow;
    waveformToChange = -1;
  }

  bool done = false;
  if (waveformEnabled || pwmState.cnt) {
    do {
      nextEventCycles = microsecondsToClockCycles(MAXIRQUS);
      
      // PWM state machine implementation
      if (pwmState.cnt) {
        uint32_t now =  GetCycleCountIRQ();
        int32_t cyclesToGo = pwmState.nextServiceCycle - now;
        if (cyclesToGo <= 10) {
            if (pwmState.idx == pwmState.cnt) { // Start of pulses, possibly copy new
                if (pwmUpdate) {
                    // Do the memory copy from temp to global and clear mailbox
                    pwmState = *(PWMState*)pwmUpdate;
                    pwmUpdate = nullptr;
                }
                GPOS = pwmState.mask; // Set all active pins high
                // GPIO16 isn't the same as the others
                if (pwmState.mask & 0x100) {
                  GP16O |= 1;
                }
                pwmState.idx = 0;
            } else {
                do {
                    // Drop the pin at this edge
                    GPOC = 1<<pwmState.edge[pwmState.idx].pin;
                    // GPIO16 still needs manual work
                    if (pwmState.edge[pwmState.idx].pin == 16) {
                      GP16O &= ~1;
                    }
                    pwmState.idx++;
                    // Any other pins at this same PWM value will have delta==0, drop them too.
                } while (pwmState.edge[pwmState.idx].delta == 0);
            }
            // Preserve duty cycle over PWM period by using now+xxx instead of += delta
            pwmState.nextServiceCycle = now + pwmState.edge[pwmState.idx].delta;
            cyclesToGo = pwmState.nextServiceCycle - now;
            if (cyclesToGo<0) cyclesToGo=0;
        }
        nextEventCycles = min_u32(nextEventCycles, cyclesToGo);
      }

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
          waveformState ^= mask;
          if (waveformState & mask) {
            if (i == 16) {
              GP16O |= 1; // GPIO16 write slow as it's RMW
            } else {
              SetGPIO(mask);
            }
            wave->nextServiceCycle = now + wave->timeHighCycles;
            nextEventCycles = min_u32(nextEventCycles, wave->timeHighCycles);
          } else {
            if (i == 16) {
              GP16O &= ~1; // GPIO16 write slow as it's RMW
            } else {
              ClearGPIO(mask);
            }
            wave->nextServiceCycle = now + wave->timeLowCycles;
            nextEventCycles = min_u32(nextEventCycles, wave->timeLowCycles);
            // Copy over next full-cycle timings
            wave->timeHighCycles = wave->gotoTimeHighCycles;
            wave->timeLowCycles = wave->gotoTimeLowCycles;
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

  if (nextEventCycles < microsecondsToClockCycles(5)) {
    nextEventCycles = microsecondsToClockCycles(5);
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

#endif  // ESP8266