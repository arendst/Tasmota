/*
  xsns_01_counter.ino - Counter sensors (water meters, electricity meters etc.) sensor support for Tasmota

  Copyright (C) 2021  Maarten Damen and Theo Arends

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

void IRAM_ATTR CounterIsrArg(void *arg);
void CounterIsrArg(void *arg) {
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
      if (index == 3) { ACDimmerZeroCross(time); }
      return;
#else
      if (!Settings->flag6.counter_both_edges) {  // SetOption159 - (Counter) Enable counting on both rising and falling edge (1)
        return;
      }
#endif //USE_AC_ZERO_CROSS_DIMMER
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
    if (Settings->flag4.zerocross_dimmer) {
        return;
    }
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

// is this GPIO configured as a counter
// this encapsulates the logic and avoids exposing internals to Berry
bool CounterPinConfigured(int32_t counter) {
  if ((counter > 0) && (counter <= MAX_COUNTERS) && (PinUsed(GPIO_CNTR1, counter - 1))) {
    return true;
  } else {
    return false;
  }
}

// return counter value for this GPIO
// this encapsulates the logic and avoids exposing internals to Berry
uint32_t CounterPinRead(int32_t counter) {
  if (CounterPinConfigured(counter)) {
    return RtcSettings.pulse_counter[counter - 1];
  }
  return 0;
}

// set the value, add offset if `add` is true, return value
// this encapsulates the logic and avoids exposing internals to Berry
uint32_t CounterPinSet(int32_t counter, int32_t value, bool add) {
  if (CounterPinConfigured(counter)) {
    if (add) {
      RtcSettings.pulse_counter[counter - 1] += value;
    } else {
      RtcSettings.pulse_counter[counter - 1] = value;
    }
    return RtcSettings.pulse_counter[counter - 1];
  }
  return 0;
}


void CounterInit(void)
{

  for (uint32_t i = 0; i < MAX_COUNTERS; i++) {
    if (PinUsed(GPIO_CNTR1, i)) {
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
      case FUNC_INTERRUPT_STOP:
        CounterInterruptDisable(true);
        break;
      case FUNC_INTERRUPT_START:
        CounterInterruptDisable(false);
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
