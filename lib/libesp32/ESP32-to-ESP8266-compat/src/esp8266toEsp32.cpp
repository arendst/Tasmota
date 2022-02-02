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
#include "esp8266toEsp32.h"
#include "driver/ledc.h"

// Tasmota Logging
extern void AddLog(uint32_t loglevel, PGM_P formatP, ...);
enum LoggingLevels {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE};

// ESP Stuff

// replicated from `tasmota.h`
#if defined(CONFIG_IDF_TARGET_ESP32)
  const uint8_t MAX_PWMS = 16;            // ESP32: 16 ledc PWM channels in total - TODO for now
#elif defined(CONFIG_IDF_TARGET_ESP32S2)
  const uint8_t MAX_PWMS = 8;             // ESP32S2: 8 ledc PWM channels in total
#elif defined(CONFIG_IDF_TARGET_ESP32S3)
  const uint8_t MAX_PWMS = 8;             // ESP32S2: 8 ledc PWM channels in total
#elif defined(CONFIG_IDF_TARGET_ESP32C3)
  const uint8_t MAX_PWMS = 6;             // ESP32C3: 6 ledc PWM channels in total
#else
  const uint8_t MAX_PWMS = 5;             // Unknown - revert to 5 PWM max
#endif

// channel mapping
static uint8_t  pwm_channel[MAX_PWMS];
static uint32_t pwm_frequency = 977;      // Default 977Hz
static uint8_t  pwm_bit_num = 10;         // Default 1023
static bool     pwm_impl_inited = false;  // trigger initialization

/*********************************************************************************************\
 * ESP32 analogWrite emulation support
\*********************************************************************************************/

void _analogInit(void) {
  if (pwm_impl_inited) return;
  // set all channels to unaffected (255)
  for (uint32_t i = 0; i < MAX_PWMS; i++) {
    pwm_channel[i] = 255;
  }
  pwm_impl_inited = true;
}

int32_t _analog_pin2chan(uint32_t pin) {    // returns -1 if uallocated
  _analogInit();      // make sure the mapping array is initialized
  for (uint32_t channel = 0; channel < MAX_PWMS; channel++) {
    if ((pwm_channel[channel] < 255) && (pwm_channel[channel] == pin)) {
      return channel;
    }
  }
  return -1;
}

void _analogWriteFreqRange(void) {
  _analogInit();      // make sure the mapping array is initialized
  for (uint32_t channel = 0; channel < MAX_PWMS; channel++) {
    if (pwm_channel[channel] < 255) {
      ledcSetup(channel, pwm_frequency, pwm_bit_num);
    }
  }
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

void analogWriteRange(uint32_t range) {
  pwm_bit_num = _analogGetResolution(range);
  _analogWriteFreqRange();
}

void analogWriteFreq(uint32_t freq) {
  pwm_frequency = freq;
  _analogWriteFreqRange();
}

int32_t analogAttach(uint32_t pin) {    // returns ledc channel used, or -1 if failed
  _analogInit();      // make sure the mapping array is initialized
  // Find if pin is already attached
  int32_t channel = _analog_pin2chan(pin);
  if (channel >= 0) { return channel; }
  // Find an empty channel
  for (channel = 0; channel < MAX_PWMS; channel++) {
    if (255 == pwm_channel[channel]) {
      pwm_channel[channel] = pin;
      ledcAttachPin(pin, channel);
      ledcSetup(channel, pwm_frequency, pwm_bit_num);
      // Serial.printf("PWM: New attach pin %d to channel %d\n", pin, channel);
      return channel;
    }
  }
  // No more channels available
  AddLog(LOG_LEVEL_INFO, "PWM: no more PWM (ledc) channel for GPIO %i", pin);
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

// exported from Arduno Core
extern uint8_t channels_resolution[MAX_PWMS];

void analogWritePhase(uint8_t pin, uint32_t duty, uint32_t phase)
{
  int32_t chan = _analog_pin2chan(pin);
  if (chan < 0) {    // not yet allocated, try to allocate
    chan = analogAttach(pin);
    if (chan < 0) { return; }   // failed
  }

  if (duty >> (pwm_bit_num-1) ) ++duty;   // input is 0..1023 but PWM takes 0..1024 - so we skip at mid-range. It creates a small non-linearity
  if (phase >> (pwm_bit_num-1) ) ++phase;

  uint8_t group=(chan/8), channel=(chan%8);

  //Fixing if all bits in resolution is set = LEDC FULL ON
  uint32_t max_duty = (1 << channels_resolution[chan]) - 1;
  phase = phase & max_duty;

  ledc_set_duty_with_hpoint((ledc_mode_t)group, (ledc_channel_t)channel, duty, phase);
  ledc_update_duty((ledc_mode_t)group, (ledc_channel_t)channel);
}

#endif // ESP32
