/*
 This library is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef ESP32

#include "Arduino.h"
#include "esp_idf_version.h"
#include "esp8266toEsp32.h"
#include "driver/ledc.h"

// Tasmota Logging
extern void AddLog(uint32_t loglevel, PGM_P formatP, ...);
enum LoggingLevels {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE};

// ESP Stuff

// This is from Arduino code -- not sure why it is necessary
//Use XTAL clock if possible to avoid timer frequency error when setting APB clock < 80 Mhz
//Need to be fixed in ESP-IDF
#ifdef SOC_LEDC_SUPPORT_XTAL_CLOCK
#define LEDC_DEFAULT_CLK        LEDC_USE_XTAL_CLK
#else
#define LEDC_DEFAULT_CLK        LEDC_AUTO_CLK
#endif
#if (ESP_IDF_VERSION_MAJOR >= 5)
  #define LEDC_MAX_BIT_WIDTH      SOC_LEDC_TIMER_BIT_WIDTH
#else
  #define LEDC_MAX_BIT_WIDTH      SOC_LEDC_TIMER_BIT_WIDE_NUM
#endif

// define our limits to ease any change from esp-idf
#define MAX_TIMERS              LEDC_TIMER_MAX            // 4 timers for all ESP32 variants
#ifdef SOC_LEDC_SUPPORT_HS_MODE
  #define PWM_HAS_HIGHSPEED  SOC_LEDC_SUPPORT_HS_MODE     // are there 2 banks of timers/ledc
#endif


// replicated from `tasmota.h`
#if CONFIG_IDF_TARGET_ESP32
const uint8_t MAX_PWMS = 16;              // ESP32: 16 ledc PWM channels in total - TODO for now
#elif CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
const uint8_t MAX_PWMS = 8;               // ESP32S2/S3: 8 ledc PWM channels in total
#elif CONFIG_IDF_TARGET_ESP32C2 || CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C5 || CONFIG_IDF_TARGET_ESP32C6
const uint8_t MAX_PWMS = 6;               // ESP32C2/C3/C6: 6 ledc PWM channels in total
#else
const uint8_t MAX_PWMS = 5;               // Unknown - revert to 5 PWM max
#endif

// current configuration of timers: frequency and resolution

static uint32_t  timer_freq_hz[MAX_TIMERS] = {0};
static uint8_t   timer_duty_resolution[MAX_TIMERS] = {0};

// channel mapping
static int8_t pin_to_channel[SOC_GPIO_PIN_COUNT] = { 0 }; // contains the channel assigned to each pin, 0 means unassigned, substract 1
static uint8_t  pwm_timer[MAX_PWMS] = {0};          // contains the timer assigned to each channel

static const uint32_t pwm_def_frequency = 977;      // Default 977Hz
static const ledc_timer_bit_t  pwm_def_bit_num = LEDC_TIMER_10_BIT;         // Default 1023
static bool     pwm_impl_inited = false;  // trigger initialization

/*********************************************************************************************\
 * ESP32 analogWrite emulation support
\*********************************************************************************************/

// apply the configuration of timer number `timer` to the actual timer
// it should be called whenever you change the configuration of a Timer
void _analog_applyTimerConfig(int32_t timer) {
  esp_err_t ret;
  if (timer < 0 || timer >= MAX_TIMERS) { return; }   // avoid overflow or underflow

  // AddLog(LOG_LEVEL_INFO, "PWM: ledc_timer_config(res=%i timer=%i freq=%i)", timer_duty_resolution[timer], timer, timer_freq_hz[timer]);
  // we apply configuration to timer
  ledc_timer_config_t cfg = {
    (ledc_mode_t) 0,                                        // speed mode - first bank
    (ledc_timer_bit_t) timer_duty_resolution[timer],   // duty_resolution
    (ledc_timer_t) timer,                                   // timer_num
    timer_freq_hz[timer],                              // freq_hz
    LEDC_DEFAULT_CLK                                        // clk_cfg
  };
  ret = ledc_timer_config(&cfg);
  if (ret != ESP_OK) {
    AddLog(LOG_LEVEL_ERROR, "PWM: ledc_timer_config %i failed ret=%i", timer, ret);
  }
#ifdef PWM_HAS_HIGHSPEED
  // apply the same parameter to the low-speed timer as well
  cfg.speed_mode = (ledc_mode_t) 1;         // first bank
  ret = ledc_timer_config(&cfg);
  if (ret != ESP_OK) {
    AddLog(LOG_LEVEL_ERROR, "PWM: ledc_timer_config %i failed ret=%i", timer + MAX_TIMERS, ret);
  }
#endif
}

// initialize all timers and memory structures
void _analogInit(void) {
  if (pwm_impl_inited) { return; }
  // set all channels to unaffected (255)

  // On ESP32 there are 2 groups of timers. 0..LEDC_TIMER_MAX-1 and LEDC_TIMER_MAX..2*LEDC_TIMER_MAX-1
  for (uint32_t i = 0; i < MAX_TIMERS; i++) {
    timer_freq_hz[i] = pwm_def_frequency;
    timer_duty_resolution[i] = pwm_def_bit_num;
    _analog_applyTimerConfig(i);    // apply settings to Timer 0
  }
  pwm_impl_inited = true;
}

// set the timer number for a GPIO, ignore if the GPIO is not set or Timer number is invalid
// Timer range is 0..3
void ledcSetTimer(uint8_t chan, uint8_t timer) {
  if (timer >= MAX_TIMERS || chan > MAX_PWMS) { return; }
  uint8_t cur_timer = pwm_timer[chan];

  if (timer != cur_timer) {     // ignore if the timer number is the same
    pwm_timer[chan] = timer;    // change the timer value
    // apply to hardware
    uint8_t group=(chan/8);
    uint8_t channel=(chan%8);
    esp_err_t ret = ledc_bind_channel_timer((ledc_mode_t) group, (ledc_channel_t) channel, (ledc_timer_t) timer);
    if (ret != ESP_OK) {
      AddLog(LOG_LEVEL_ERROR, "PWM: ledc_bind_channel_timer %i failed ret=%i", timer, ret);
    }
  }
}

// return the channel number for a GPIO, -1 if none
int32_t analogGetChannel2(uint32_t pin) {    // returns -1 if uallocated
  if (pin >= SOC_GPIO_PIN_COUNT) { return -1; }
  return pin_to_channel[pin] - 1;
}

/* Convert a GPIO number to the pointer of the Timer number */
int32_t _analog_pin2timer(uint32_t pin) {    // returns -1 if uallocated
  _analogInit();      // make sure the mapping array is initialized
  int chan = analogGetChannel2(pin);
  if (chan < 0) { return -1; }
  int32_t timer = pwm_timer[chan];
  if (timer > MAX_TIMERS) { timer = 0; }
  return timer;
}

// get the next unused timer, returns -1 if no free timer is available
// Keep in mind that Timer 0 is reserved, which leaves only 3 timers available
//
// This function does not reserve the timer, it is reserved only when you assign a GPIO to it
static int32_t analogNextFreeTimer() {
  _analogInit();      // make sure the mapping array is initialized
  bool assigned[MAX_TIMERS] = {};
  assigned[0] = true;

  for (uint32_t chan = 0; chan < MAX_PWMS; chan++) {
    assigned[pwm_timer[chan]] = true;
  }

  // find first free
  for (uint32_t j = 0; j < MAX_TIMERS; j++) {
    if (!assigned[j]) {
      // AddLog(LOG_LEVEL_INFO, "PWM: analogNextFreeTimer next_timer=%i", j);
      return j;
      }
  }
  // AddLog(LOG_LEVEL_INFO, "PWM: analogNextFreeTimer no free timer");
  return -1;    // none available
}

// input range is in full range, ledc needs bits
uint32_t _analogGetResolution(uint32_t x) {
  uint32_t bits = 0;
  while (x) {
    bits++;
    x >>= 1;
  }
  return bits;
}

void analogWriteRange(uint32_t range, int32_t pin) {
  // AddLog(LOG_LEVEL_INFO, "PWM: analogWriteRange range=%i pin=%i", range, pin);
  _analogInit();      // make sure the mapping array is initialized
  int32_t timer = (pin < 0) ? 0 : _analog_pin2timer(pin);
  if (timer < 0) { return; }

  uint32_t pwm_bit_num = _analogGetResolution(range);
  if (pwm_bit_num > LEDC_MAX_BIT_WIDTH || pwm_bit_num == 0) {
    AddLog(LOG_LEVEL_ERROR, "PWM: range is invalid: %i", range);
    return;
  }
  timer_duty_resolution[timer] = (ledc_timer_bit_t) pwm_bit_num;
  _analog_applyTimerConfig(timer);
}

// change both freq and range
// `0`: set to global value
// `-1`: keep unchanged
// if pin < 0 then change global value for timer 0
void analogWriteFreqRange(int32_t freq, int32_t range, int32_t pin) {
  // AddLog(LOG_LEVEL_INFO, "PWM: analogWriteFreqRange freq=%i range=%i pin=%i", freq, range, pin);
  _analogInit();      // make sure the mapping array is initialized
  uint32_t timer0_freq = timer_freq_hz[0];          // global values
  uint8_t  timer0_res = timer_duty_resolution[0];

  int32_t timer = 0;
  int32_t res = timer0_res;
  if (pin < 0) {
    if (freq <= 0) { freq = timer0_freq; }
    if (range > 0) {
      res = _analogGetResolution(range);
      if (res >= LEDC_TIMER_BIT_MAX) { return; }
    }
  } else {
    int32_t chan = analogGetChannel2(pin);
    if (chan < 0) { return; }
    timer = pwm_timer[chan];
    if (freq < 0) { freq = timer_freq_hz[timer]; }
    if (freq == 0) { freq = timer0_freq; }

    res = timer0_res;
    if (range < 0) { res = timer_duty_resolution[timer]; }
    if (range != 0) { res = _analogGetResolution(range); }
    if (res >= LEDC_TIMER_BIT_MAX) { return; }

    if (freq == timer0_freq && res == timer0_res) {
      // settings match with the global value
      if (timer != 0) {
        ledcSetTimer(chan, 0);
        timer = 0;
      }
      // else nothing to change
    } else {
      // specific (non-global) values, require a specific timer
      if (timer == 0) {   // currently using the global timer, need to change
        // we need to allocate a new timer to this pin
        int32_t next_timer = analogNextFreeTimer();
        if (next_timer < 0) {
          AddLog(LOG_LEVEL_ERROR, "PWM: failed to assign a timer to GPIO %i", pin);
        } else {
          ledcSetTimer(chan, next_timer);
          timer = next_timer;
        }
      }
    }
    pwm_timer[chan] = timer;
  }

  // AddLog(LOG_LEVEL_INFO, "PWM: analogWriteFreq actual freq=%i res=%i pin=%i timer=%i", freq, res, pin, timer);
  if (timer_freq_hz[timer] != freq || timer_duty_resolution[timer] != res) {
    timer_freq_hz[timer] = freq;
    timer_duty_resolution[timer] = res;
    _analog_applyTimerConfig(timer);
  }
}

// set the frequency, in pin == -1 then change the global value of timer 0
void analogWriteFreq(uint32_t freq, int32_t pin) {
  analogWriteFreqRange(freq, 0, pin);
}

// find next unassigned channel, or -1 if none available
static int32_t findEmptyChannel() {
  bool chan_used[MAX_PWMS] = {0};
  for (uint32_t pin = 0; pin < SOC_GPIO_PIN_COUNT; pin++) {
    if (pin_to_channel[pin] > 0) {
      chan_used[pin_to_channel[pin] - 1] = true;
    }
  }

  // find empty slot
  for (uint32_t chan = 0; chan < MAX_PWMS; chan++) {
    if (!chan_used[chan]) {
      return chan;
    }
  }
  return -1;
}

int32_t analogAttach(uint32_t pin, bool output_invert) {    // returns ledc channel used, or -1 if failed
  _analogInit();      // make sure the mapping array is initialized
  // Find if pin is already attached
  int32_t chan = analogGetChannel2(pin);
  if (chan >= 0) { return chan; }
  // Find an empty channel
  chan = findEmptyChannel();
  if (chan < 0) {
    AddLog(LOG_LEVEL_INFO, "PWM: no more PWM (ledc) channel for GPIO %i", pin);
    return -1;
  }

  // new channel attached to pin
  pin_to_channel[pin] = chan + 1;

  // ledcAttachPin(pin, channel);  -- replicating here because we want the default duty
  // timer0 used by default
  uint8_t group=(chan/8);
  uint8_t channel=(chan%8);
  uint8_t timer=0;

  // AddLog(LOG_LEVEL_INFO, "PWM: ledc_channel pin=%i out_invert=%i", pin, output_invert);
  ledc_channel_config_t ledc_channel = {
      (int)pin,          // gpio
      (ledc_mode_t)group,        // speed-mode
      (ledc_channel_t)channel,      // channel
      (ledc_intr_type_t)LEDC_INTR_DISABLE,  // intr_type
      (ledc_timer_t)timer,        // timer_sel
      0,            // duty
      0,            // hpoint
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 4, 0)
      (ledc_sleep_mode_t) 2,
#endif
      { output_invert ? 1u : 0u },// output_invert
  };
  ledc_channel_config(&ledc_channel);

  // AddLog(LOG_LEVEL_INFO, "PWM: New attach pin %d to channel %d", pin, channel);
  return chan;
}

void analogDetach(uint32_t pin) {
  if (pin_to_channel[pin] > 0) {
#if ESP_IDF_VERSION_MAJOR < 5
    ledcDetachPin(pin);
#else
    ledcDetach(pin);
#endif
    pin_to_channel[pin] = 0;
  }
}

void analogDetachAll(void) {
  for (uint32_t pin = 0; pin < SOC_GPIO_PIN_COUNT; pin++) { 
    analogDetach(pin);
  }
}

extern "C" uint32_t ledcReadFreq2(uint8_t chan) {
// extern "C" uint32_t __wrap_ledcReadFreq(uint8_t chan) {
  if (chan > MAX_PWMS) {
    return 0;     // wrong channel
  }
  int32_t timer = pwm_timer[chan];
  int32_t freq = timer_freq_hz[timer];
  return freq;
}

uint8_t ledcReadResolution(uint8_t chan) {
  if (chan > MAX_PWMS) {
    return 0;     // wrong channel
  }
  int32_t timer = pwm_timer[chan];
  int32_t res = timer_duty_resolution[timer];
  return res;
}

int32_t ledcReadDutyResolution(uint8_t pin) {
  int32_t chan = analogGetChannel2(pin);
  if (chan >= 0) {
    return (1 << ledcReadResolution(chan));
  }
  return -1;
}

// Version of ledcRead that works for both Core2 and Core3
// Return -1 if pin is not configured as PWM
int32_t ledcRead2(uint8_t pin) {
  int32_t chan = analogGetChannel2(pin);
  if (chan >= 0) {
    uint8_t group=(chan/8), channel=(chan%8);
    return ledc_get_duty((ledc_mode_t)group, (ledc_channel_t)channel);
  }
  return -1;
}

// void analogWrite(uint8_t pin, int val);
extern "C" void __wrap__Z11analogWritehi(uint8_t pin, int val) {
  analogWritePhase(pin, val, 0);      // if unspecified, use phase = 0
}

/*
  The primary goal of this function is to add phase control to PWM ledc
  functions.

  Phase control allows to stress less the power supply of LED lights.
  By default all phases are starting at the same moment. This means
  the the power supply always takes a power hit at the start of each
  new cycle, even if the average power is low.
  Phase control is also of major importance for H-bridge where
  both PWM lines should NEVER be active at the same time.

  Unfortunately Arduino Core does not allow any customization nor
  extendibility for the ledc/analogWrite functions. We have therefore
  no other choice than duplicating part of Arduino code.

  WARNING: this means it can easily break if ever Arduino internal
  implementation changes.
*/

void analogWritePhase(uint8_t pin, uint32_t duty, uint32_t phase)
{
  int32_t chan = analogGetChannel2(pin);
  if (chan < 0) {    // not yet allocated, try to allocate
    chan = analogAttach(pin);
    if (chan < 0) {
      AddLog(LOG_LEVEL_INFO, "PWM: analogWritePhase invalid chan=%i", chan);
      return;
    }   // failed
  }
  int32_t timer = _analog_pin2timer(pin);
  if (timer < 0) {
    AddLog(LOG_LEVEL_INFO, "PWM: analogWritePhase invalid timer=%i", timer);
    return;
  }

  int32_t pwm_bit_num = timer_duty_resolution[timer];
  // AddLog(LOG_LEVEL_INFO, "PWM: analogWritePhase pin=%i chan=%i duty=%03X phase=%03X pwm_bit_num=%i", pin, chan, duty, phase, pwm_bit_num);

  if (duty >> (pwm_bit_num-1) ) ++duty;   // input is 0..1023 but PWM takes 0..1024 - so we skip at mid-range. It creates a small non-linearity
  if (phase >> (pwm_bit_num-1) ) ++phase;

  uint8_t group=(chan/8), channel=(chan%8);

  //Fixing if all bits in resolution is set = LEDC FULL ON
  uint32_t max_duty = (1 << pwm_bit_num) - 1;
  phase = phase & max_duty;

  esp_err_t err1, err2;
  err1 = ledc_set_duty_with_hpoint((ledc_mode_t)group, (ledc_channel_t)channel, duty, phase);
  err2 = ledc_update_duty((ledc_mode_t)group, (ledc_channel_t)channel);
  // AddLog(LOG_LEVEL_INFO, "PWM: err1=%i err2=%i", err1, err2);
}

// get the timer number for a GPIO, -1 if not found
int32_t analogGetTimer(uint8_t pin) {
  return _analog_pin2timer(pin);
}

int32_t analogGetTimerForChannel(uint8_t chan) {
  _analogInit();      // make sure the mapping array is initialized
  if (chan > MAX_PWMS) { return -1; }
  int32_t timer = pwm_timer[chan];
  if (timer > MAX_TIMERS) { timer = 0; }
  return timer;
}


// get the next unused timer, returns -1 if no free timer is available
// Keep in mind that Timer 0 is reserved, which leaves only 3 timers available
//
// Get timer resolution (in bits) - default 10
uint8_t analogGetTimerResolution(uint8_t timer) {
  _analogInit();      // make sure the mapping array is initialized
  if (timer >= MAX_TIMERS) { timer = 0; }
  return timer_duty_resolution[timer];
}

// Get timer frequency (in Hz) - default 977
uint32_t analogGetTimerFrequency(uint8_t timer) {
  _analogInit();      // make sure the mapping array is initialized
  if (timer >= MAX_TIMERS) { timer = 0; }
  return timer_freq_hz[timer]; // TODO check validity of value
}


#endif // ESP32
