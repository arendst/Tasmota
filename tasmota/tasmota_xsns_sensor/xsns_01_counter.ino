/*
  xsns_01_counter.ino - Counter sensors (water meters, electricity meters etc.) sensor support for Tasmota

  Copyright (C) 2021  Maarten Damen and Theo Arends
                      Stefan Bode (Zero-Cross Dimmer)

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

#ifdef USE_COUNTER
/*********************************************************************************************\
 * Counter sensors (water meters, electricity meters etc.)
\*********************************************************************************************/

#define XSNS_01             1

#define D_PRFX_COUNTER "Counter"
#define D_CMND_COUNTERTYPE "Type"
#define D_CMND_COUNTERDEBOUNCE "Debounce"
#define D_CMND_COUNTERDEBOUNCELOW "DebounceLow"
#define D_CMND_COUNTERDEBOUNCEHIGH "DebounceHigh"

const char kCounterCommands[] PROGMEM = D_PRFX_COUNTER "|"  // Prefix
  "|" D_CMND_COUNTERTYPE "|" D_CMND_COUNTERDEBOUNCE  "|" D_CMND_COUNTERDEBOUNCELOW "|" D_CMND_COUNTERDEBOUNCEHIGH ;

void (* const CounterCommand[])(void) PROGMEM = {
  &CmndCounter, &CmndCounterType, &CmndCounterDebounce, &CmndCounterDebounceLow, &CmndCounterDebounceHigh };

uint8_t ctr_index[MAX_COUNTERS] =  { 0, 1, 2, 3 };

struct COUNTER {
  uint32_t timer[MAX_COUNTERS];  // Last counter time in micro seconds
  uint32_t timer_low_high[MAX_COUNTERS];  // Last low/high counter time in micro seconds
  uint8_t no_pullup = 0;         // Counter input pullup flag (1 = No pullup)
  uint8_t pin_state = 0;         // LSB0..3 Last state of counter pin; LSB7==0 IRQ is FALLING, LSB7==1 IRQ is CHANGE
  bool any_counter = false;

} Counter;

#ifdef USE_AC_ZERO_CROSS_DIMMER
struct AC_ZERO_CROSS_DIMMER {
  bool startReSync = false;                      // set to TRUE if zero-cross event occurs
  bool startMeasurePhase[MAX_COUNTERS] ;         // set to TRUE if channel is ON and zero-cross occurs to initiate phase measure on channel
  bool pwm_defined[MAX_COUNTERS];                // check if all GPIO are set and zerocross enabled. Then ADD dimmer.
  bool PWM_ON[MAX_COUNTERS] ;                    // internal ON/OFF of the channel
  uint32_t current_cycle_ClockCycles = 0;        // amount of clock cycles between two zero-cross events.
  uint32_t currentPWMCycleCount[MAX_COUNTERS] ;  // clock cycle time of PWM channel, required to measure actual phase. [3] is phase of zero-cross
  int16_t  currentShiftClockCycle[MAX_COUNTERS]; // dynamic phase correction per channel in clock cycles
  uint32_t tobe_cycle_timeClockCycles = 0;       // clock cycles between zero-cross events. Depend on main frequency and CPU speed
  uint32_t lastCycleCount = 0;                   // Last value of GetCycleCount during zero-cross sychronisation
  uint32_t currentSteps = 100;                   // dynamic value of zero-crosses between two sychronisation intervalls (default=20 == 200ms at 100Hz)
  uint32_t high;                                 // cycle counts for PWM high vaule. needs long enough (4µs) to secure fire TRIAC
} ac_zero_cross_dimmer;
#endif //USE_AC_ZERO_CROSS_DIMMER

void IRAM_ATTR CounterIsrArg(void *arg) {
  uint32_t index = *static_cast<uint8_t*>(arg);

  uint32_t time = micros();
  uint32_t debounce_time;

  if (Counter.pin_state) {
    // handle low and high debounce times when configured
    if (digitalRead(Pin(GPIO_CNTR1, index)) == bitRead(Counter.pin_state, index)) {
      // new pin state to be ignored because debounce time was not met during last IRQ
      return;
    }
    debounce_time = time - Counter.timer_low_high[index];
    if bitRead(Counter.pin_state, index) {
      // last valid pin state was high, current pin state is low
      if (debounce_time <= Settings->pulse_counter_debounce_high * 1000) return;
    } else {
      // last valid pin state was low, current pin state is high
      if (debounce_time <= Settings->pulse_counter_debounce_low * 1000) return;
    }
    // passed debounce check, save pin state and timing
    Counter.timer_low_high[index] = time;
    Counter.pin_state ^= (1<<index);
    // do not count on rising edge
    if bitRead(Counter.pin_state, index) {
      // PWMfrequency 100
      // restart PWM each second (german 50Hz has to up to 0.01% deviation)
      // restart initiated by setting Counter.startReSync = true;
#ifdef USE_AC_ZERO_CROSS_DIMMER
      // if zero-cross events occur ond channel is on. phase on PWM must be measured
      if ( ac_zero_cross_dimmer.startMeasurePhase[index] == true ) {
        ac_zero_cross_dimmer.currentPWMCycleCount[index] = ESP.getCycleCount();
        ac_zero_cross_dimmer.startMeasurePhase[index] = false;
      }
      // if zero-cross event occurs (200ms window, 5-times a second) and device is online for >10sec
      if (index == 3 && RtcSettings.pulse_counter[index]%(Settings->pwm_frequency / 5) == 0 && ac_zero_cross_dimmer.pwm_defined[index] && millis() > 10000) {
        ac_zero_cross_dimmer.currentPWMCycleCount[index] = ESP.getCycleCount();

        if (ac_zero_cross_dimmer.lastCycleCount > 0) {
          // start phase measure on PWM channels and initiate phase sync with zero-cross.
          ac_zero_cross_dimmer.startReSync = true;
          for (uint8_t k=0; k < MAX_COUNTERS-1; k++ ) {
            if (ac_zero_cross_dimmer.PWM_ON[k] == true) ac_zero_cross_dimmer.startMeasurePhase[k] = true;
          }
          ac_zero_cross_dimmer.currentSteps = (ac_zero_cross_dimmer.currentPWMCycleCount[index]-ac_zero_cross_dimmer.lastCycleCount+(ac_zero_cross_dimmer.tobe_cycle_timeClockCycles/2))/(ac_zero_cross_dimmer.tobe_cycle_timeClockCycles);
          ac_zero_cross_dimmer.current_cycle_ClockCycles = (ac_zero_cross_dimmer.currentPWMCycleCount[index]-ac_zero_cross_dimmer.lastCycleCount)/ac_zero_cross_dimmer.currentSteps;
        }
        ac_zero_cross_dimmer.lastCycleCount = ac_zero_cross_dimmer.currentPWMCycleCount[index];
      }

#endif //USE_AC_ZERO_CROSS_DIMMER
      return;
    }
  }

  debounce_time = time - Counter.timer[index];
  if (debounce_time > Settings->pulse_counter_debounce * 1000) {
    Counter.timer[index] = time;
    if (bitRead(Settings->pulse_counter_type, index)) {
      RtcSettings.pulse_counter[index] = debounce_time;
    } else {
      RtcSettings.pulse_counter[index]++;
    }
  }
}

/********************************************************************************************/

void CounterInterruptDisable(bool state)
{
  if (state) {   // Disable interrupts
    if (Counter.any_counter) {
      for (uint32_t i = 0; i < MAX_COUNTERS; i++) {
        if (PinUsed(GPIO_CNTR1, i)) {
          detachInterrupt(Pin(GPIO_CNTR1, i));
        }
      }
      Counter.any_counter = false;
    }
  } else {       // Enable interrupts
    if (!Counter.any_counter) {
      CounterInit();
    }
  }
}

bool CounterPinState(void)
{
  if ((XdrvMailbox.index >= AGPIO(GPIO_CNTR1_NP)) && (XdrvMailbox.index < (AGPIO(GPIO_CNTR1_NP) + MAX_COUNTERS))) {
    bitSet(Counter.no_pullup, XdrvMailbox.index - AGPIO(GPIO_CNTR1_NP));
    XdrvMailbox.index -= (AGPIO(GPIO_CNTR1_NP) - AGPIO(GPIO_CNTR1));
    return true;
  }
  return false;
}

void CounterInit(void)
{
  for (uint32_t i = 0; i < MAX_COUNTERS; i++) {
    if (PinUsed(GPIO_CNTR1, i)) {
#ifdef USE_AC_ZERO_CROSS_DIMMER
      if (Settings->flag4.zerocross_dimmer) {
        ac_zero_cross_dimmer.current_cycle_ClockCycles = ac_zero_cross_dimmer.tobe_cycle_timeClockCycles = microsecondsToClockCycles(1000000 / Settings->pwm_frequency);
        // short fire on PWM to ensure not to hit next sinus curve but trigger the TRIAC. 0.78% of duty cycle (10ms) ~4µs
        ac_zero_cross_dimmer.high = ac_zero_cross_dimmer.current_cycle_ClockCycles / 256;

        // Support for dimmer 1-3. Counter4 reseverd for zero-cross signal
        if ((i < MAX_COUNTERS-1 && PinUsed(GPIO_PWM1, i)) || ( i == MAX_COUNTERS-1) ) {
          ac_zero_cross_dimmer.pwm_defined[i] = true;
          if (i == 3) {
            AddLog(LOG_LEVEL_INFO, PSTR("ZeroCross initialized"));
          } else {
            AddLog(LOG_LEVEL_INFO, PSTR("Dimmer: [%d] initialized. READY. Dimmer %d"), i+1, Light.fade_running ? Light.fade_cur_10[i] : Light.fade_start_10[i]);
          }

        }
      }
#endif //USE_AC_ZERO_CROSS_DIMMER
      Counter.any_counter = true;
      pinMode(Pin(GPIO_CNTR1, i), bitRead(Counter.no_pullup, i) ? INPUT : INPUT_PULLUP);
      if ((0 == Settings->pulse_counter_debounce_low) && (0 == Settings->pulse_counter_debounce_high) && !Settings->flag4.zerocross_dimmer) {
        Counter.pin_state = 0;
        attachInterruptArg(Pin(GPIO_CNTR1, i), CounterIsrArg, &ctr_index[i], FALLING);
      } else {
        Counter.pin_state = 0x8f;
        attachInterruptArg(Pin(GPIO_CNTR1, i), CounterIsrArg, &ctr_index[i], CHANGE);
      }
    }
  }
}

void CounterEverySecond(void)
{
  for (uint32_t i = 0; i < MAX_COUNTERS; i++) {
    if (PinUsed(GPIO_CNTR1, i)) {
      if (bitRead(Settings->pulse_counter_type, i)) {
        uint32_t time = micros() - Counter.timer[i];
        if (time > 4200000000) {  // 70 minutes
          RtcSettings.pulse_counter[i] = 4200000000;  // Set Timer to max in case of no more interrupts due to stall of measured device
        }
      }
    }
  }
}

void CounterSaveState(void)
{
  for (uint32_t i = 0; i < MAX_COUNTERS; i++) {
    if (PinUsed(GPIO_CNTR1, i)) {
      Settings->pulse_counter[i] = RtcSettings.pulse_counter[i];
    }
  }
}

void CounterShow(bool json)
{
  bool header = false;
  uint8_t dsxflg = 0;
  for (uint32_t i = 0; i < MAX_COUNTERS; i++) {
    if (PinUsed(GPIO_CNTR1, i)) {
      char counter[33];
      if (bitRead(Settings->pulse_counter_type, i)) {
        dtostrfd((double)RtcSettings.pulse_counter[i] / 1000000, 6, counter);
      } else {
        dsxflg++;
        snprintf_P(counter, sizeof(counter), PSTR("%lu"), RtcSettings.pulse_counter[i]);
      }

      if (json) {
        if (!header) {
          ResponseAppend_P(PSTR(",\"COUNTER\":{"));
        }
        ResponseAppend_P(PSTR("%s\"C%d\":%s"), (header)?",":"", i +1, counter);
        header = true;
#ifdef USE_DOMOTICZ
        if ((0 == TasmotaGlobal.tele_period) && (1 == dsxflg)) {
          DomoticzSensor(DZ_COUNT, RtcSettings.pulse_counter[i]);
          dsxflg++;
        }
#endif  // USE_DOMOTICZ
        if ((0 == TasmotaGlobal.tele_period ) && (Settings->flag3.counter_reset_on_tele)) {
          RtcSettings.pulse_counter[i] = 0;
        }
#ifdef USE_WEBSERVER
      } else {
        WSContentSend_PD(PSTR("{s}" D_COUNTER "%d{m}%s%s{e}"),
          i +1, counter, (bitRead(Settings->pulse_counter_type, i)) ? " " D_UNIT_SECOND : "");
#endif  // USE_WEBSERVER
      }
    }
  }
  if (header) {
    ResponseJsonEnd();
  }
}

#ifdef USE_AC_ZERO_CROSS_DIMMER
void SyncACDimmer(void)
{
  if (ac_zero_cross_dimmer.startReSync ) {
    // currently only support one AC Dimmer PWM. Plan to support up to 4 Dimmer on same Phase.
    for (uint32_t i = 0; i < MAX_COUNTERS-1; i++) {
      if (Light.fade_start_10[i] == 0 && Light.fade_cur_10[i] == 0 && ac_zero_cross_dimmer.PWM_ON[i]==false ) continue;
      if (ac_zero_cross_dimmer.pwm_defined[i] && (ac_zero_cross_dimmer.startMeasurePhase[i] == 0 || ac_zero_cross_dimmer.PWM_ON[i] == false ) )
      {
        uint32_t phaseStart_ActualClockCycles; // As-Is positon of PWM after Zero Cross
        uint32_t phaseStart_ToBeClockCycles;   // To be position after zero-cross to fire PWM start
        int16_t  phaseShift_ClockCycles;       //


        // reset trigger for PWM sync
        ac_zero_cross_dimmer.startReSync = false;
        // calculate timeoffset to fire PWM based on Dimmer
        phaseStart_ToBeClockCycles = (ac_zero_cross_dimmer.tobe_cycle_timeClockCycles * (1024 - (Light.fade_running ? Light.fade_cur_10[i] : Light.fade_start_10[i]))) / 1024;

        // Limit range to avoid overshoot and undershoot
        phaseStart_ToBeClockCycles = tmin(tmax(phaseStart_ToBeClockCycles, 160000), 0.95* ac_zero_cross_dimmer.tobe_cycle_timeClockCycles);

        // Switch OFF dimmer
        if (Light.fade_start_10[i] == 0 && !Light.fade_running) {
          ac_zero_cross_dimmer.PWM_ON[i]=false;
          Light.fade_cur_10[i] = 0;
          digitalWrite(Pin(GPIO_PWM1, i), LOW);
          //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("CNT2: [%d], curr: %d, final: %d, fading: %d, phase-shift: %d, ON/OFF: %d"),i, Light.fade_cur_10[i], Light.fade_start_10[i], Light.fade_running, phaseStart_ToBeClockCycles,ac_zero_cross_dimmer.PWM_ON[i]);
          continue;
        }
        // Calculyte clockcycles between zero-cross [3] and start of the current PWM signal [i]
        phaseStart_ActualClockCycles = ac_zero_cross_dimmer.currentPWMCycleCount[i]-ac_zero_cross_dimmer.currentPWMCycleCount[3];

        // Calulate additional or less clockcycles to move current phase position to should be position
        phaseShift_ClockCycles = (int32_t)((int32_t)phaseStart_ToBeClockCycles-(int32_t)phaseStart_ActualClockCycles)/100;

        if ( ac_zero_cross_dimmer.PWM_ON[i] == 0 ) {
          // because in LOOP calculate the timelag to fire PWM correctly with zero-cross
          uint32_t timelag_ClockCycles = (ESP.getCycleCount() - ac_zero_cross_dimmer.currentPWMCycleCount[3])%ac_zero_cross_dimmer.tobe_cycle_timeClockCycles;
          timelag_ClockCycles = ((phaseStart_ToBeClockCycles + ac_zero_cross_dimmer.tobe_cycle_timeClockCycles) - timelag_ClockCycles)%ac_zero_cross_dimmer.tobe_cycle_timeClockCycles;

          delayMicroseconds(clockCyclesToMicroseconds(timelag_ClockCycles));
          ac_zero_cross_dimmer.PWM_ON[i]=true;
          pinMode(Pin(GPIO_PWM1, i), OUTPUT);
        } else {
          // currentShiftClockCycle is an I-Controller (not PID) to realign the phase. grace time are 5 clock cycles
          ac_zero_cross_dimmer.currentShiftClockCycle[i] += phaseShift_ClockCycles > 5 ? 1 : (phaseShift_ClockCycles < -5 ? -1 : 0);
          ac_zero_cross_dimmer.current_cycle_ClockCycles += ac_zero_cross_dimmer.currentShiftClockCycle[i]+phaseShift_ClockCycles;
        }
#ifdef ESP8266
          // Find the first GPIO being generated by checking GCC's find-first-set (returns 1 + the bit of the first 1 in an int32_t
          startWaveformClockCycles(Pin(GPIO_PWM1, i), ac_zero_cross_dimmer.high, ac_zero_cross_dimmer.current_cycle_ClockCycles - ac_zero_cross_dimmer.high, 0, -1, 0, true);
#endif  // ESP8266
#ifdef ESP32
          // Under investigation. Still not working
          double esp32freq =  1000000.0 / clockCyclesToMicroseconds(ac_zero_cross_dimmer.current_cycle_ClockCycles);
          ledcSetup(i, esp32freq, 10);
          ledcAttachPin(Pin(GPIO_PWM1, i), i);
          ledcWrite(i, 5);

#endif  // ESP32

        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("CNT: [%d], shift: %d, dimm_time_CCs %d, phaseShift_CCs %d, currentPWMcylce: %lu, current_cycle_CC: %lu, lastcc %lu, currentSteps %lu, currDIM %lu, last delta:%lu"),
                                             i, ac_zero_cross_dimmer.currentShiftClockCycle[i], phaseStart_ToBeClockCycles,phaseShift_ClockCycles,ac_zero_cross_dimmer.currentPWMCycleCount[i],ac_zero_cross_dimmer.current_cycle_ClockCycles , ac_zero_cross_dimmer.lastCycleCount, ac_zero_cross_dimmer.currentSteps, Light.fade_cur_10[i],phaseStart_ActualClockCycles);
        // Light fading
        //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("CNT: [%d], curr: %d, final: %d, fading: %d, phase-shift: %d, ON/OFF: %d"),i, Light.fade_cur_10[i], Light.fade_start_10[i], Light.fade_running, phaseStart_ToBeClockCycles,ac_zero_cross_dimmer.PWM_ON[i]);

      } // do sync onchannel
    } // loop on counter
  } // zero cross detected
} // end SyncACDimmer
#endif //USE_AC_ZERO_CROSS_DIMMER

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndCounter(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_COUNTERS)) {
    if ((XdrvMailbox.data_len > 0) && PinUsed(GPIO_CNTR1, XdrvMailbox.index -1)) {
      if ((XdrvMailbox.data[0] == '-') || (XdrvMailbox.data[0] == '+')) {
        RtcSettings.pulse_counter[XdrvMailbox.index -1] += XdrvMailbox.payload;
        Settings->pulse_counter[XdrvMailbox.index -1] += XdrvMailbox.payload;
      } else {
        RtcSettings.pulse_counter[XdrvMailbox.index -1] = XdrvMailbox.payload;
        Settings->pulse_counter[XdrvMailbox.index -1] = XdrvMailbox.payload;
      }
    }
    ResponseCmndIdxNumber(RtcSettings.pulse_counter[XdrvMailbox.index -1]);
  }
}

void CmndCounterType(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_COUNTERS)) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1) && PinUsed(GPIO_CNTR1, XdrvMailbox.index -1)) {
      bitWrite(Settings->pulse_counter_type, XdrvMailbox.index -1, XdrvMailbox.payload &1);
      RtcSettings.pulse_counter[XdrvMailbox.index -1] = 0;
      Settings->pulse_counter[XdrvMailbox.index -1] = 0;
    }
    ResponseCmndIdxNumber(bitRead(Settings->pulse_counter_type, XdrvMailbox.index -1));
  }
}

void CmndCounterDebounce(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 32001)) {
    Settings->pulse_counter_debounce = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->pulse_counter_debounce);
}

void CmndCounterDebounceLow(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 32001)) {
    Settings->pulse_counter_debounce_low = XdrvMailbox.payload;
    CounterInit();
  }
  ResponseCmndNumber(Settings->pulse_counter_debounce_low);
}

void CmndCounterDebounceHigh(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 32001)) {
    Settings->pulse_counter_debounce_high = XdrvMailbox.payload;
    CounterInit();
  }
  ResponseCmndNumber(Settings->pulse_counter_debounce_high);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns01(uint32_t function)
{
  bool result = false;

  if (Counter.any_counter) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        CounterEverySecond();
        break;
      case FUNC_JSON_APPEND:
        CounterShow(1);
        break;
#ifdef USE_AC_ZERO_CROSS_DIMMER
      case FUNC_EVERY_50_MSECOND:
        SyncACDimmer();
      break;
#endif //USE_AC_ZERO_CROSS_DIMMER
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        CounterShow(0);
        break;
#endif  // USE_WEBSERVER
      case FUNC_SAVE_BEFORE_RESTART:
      case FUNC_SAVE_AT_MIDNIGHT:
        CounterSaveState();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kCounterCommands, CounterCommand);
        break;
    }
  } else {
    switch (function) {
      case FUNC_INIT:
        CounterInit();
        break;
      case FUNC_PIN_STATE:
        result = CounterPinState();
        break;
    }
  }
  return result;
}

#endif  // USE_COUNTER
