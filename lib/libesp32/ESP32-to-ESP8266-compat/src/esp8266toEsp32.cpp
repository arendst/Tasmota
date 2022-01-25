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
//
#include "Arduino.h"
//#include "lwip/apps/sntp.h"
#include <nvs.h>

// See libraries\ESP32\examples\ResetReason.ino
#if ESP_IDF_VERSION_MAJOR > 3      // IDF 4+
  #if CONFIG_IDF_TARGET_ESP32      // ESP32/PICO-D4
    #include "esp32/rom/rtc.h"
  #elif CONFIG_IDF_TARGET_ESP32S2  // ESP32-S2
    #include "esp32s2/rom/rtc.h"
  #elif CONFIG_IDF_TARGET_ESP32C3  // ESP32-C3
    #include "esp32c3/rom/rtc.h"
  #else
    #error Target CONFIG_IDF_TARGET is not supported
  #endif
#else // ESP32 Before IDF 4.0
  #include "rom/rtc.h"
#endif

#include <ESP8266WiFi.h>
#include "esp8266toEsp32.h"

// ESP Stuff

/*********************************************************************************************\
 * ESP32 analogWrite emulation support
\*********************************************************************************************/

#if CONFIG_IDF_TARGET_ESP32C3
  uint8_t _pwm_channel[PWM_SUPPORTED_CHANNELS] = { 99, 99, 99, 99, 99, 99 };
  uint32_t _pwm_frequency = 977;     // Default 977Hz
  uint8_t _pwm_bit_num = 10;         // Default 1023
#else // other ESP32
  uint8_t _pwm_channel[PWM_SUPPORTED_CHANNELS] = { 99, 99, 99, 99, 99, 99, 99, 99 };
  uint32_t _pwm_frequency = 977;     // Default 977Hz
  uint8_t _pwm_bit_num = 10;         // Default 1023
#endif // CONFIG_IDF_TARGET_ESP32C3 vs ESP32

uint32_t _analog_pin2chan(uint32_t pin) {
  for (uint32_t channel = 0; channel < PWM_SUPPORTED_CHANNELS; channel++) {
    if ((_pwm_channel[channel] < 99) && (_pwm_channel[channel] == pin)) {
      return channel;
    }
  }
  return 0;
}

void _analogWriteFreqRange(void) {
  for (uint32_t channel = 0; channel < PWM_SUPPORTED_CHANNELS; channel++) {
    if (_pwm_channel[channel] < 99) {
      ledcSetup(channel + PWM_CHANNEL_OFFSET, _pwm_frequency, _pwm_bit_num);
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
  _pwm_bit_num = _analogGetResolution(range);
  _analogWriteFreqRange();
}

void analogWriteFreq(uint32_t freq) {
  _pwm_frequency = freq;
  _analogWriteFreqRange();
}

bool analogAttach(uint32_t pin) {
  // Find if pin is already attached
  uint32_t channel;
  for (channel = 0; channel < PWM_SUPPORTED_CHANNELS; channel++) {
    if (_pwm_channel[channel] == pin) {
      // Already attached
      // Serial.printf("PWM: Already attached pin %d to channel %d\n", pin, channel);
      return true;
    }
  }
  // Find an empty channel
  for (channel = 0; channel < PWM_SUPPORTED_CHANNELS; channel++) {
    if (99 == _pwm_channel[channel]) {
      _pwm_channel[channel] = pin;
      ledcAttachPin(pin, channel + PWM_CHANNEL_OFFSET);
      ledcSetup(channel + PWM_CHANNEL_OFFSET, _pwm_frequency, _pwm_bit_num);
      // Serial.printf("PWM: New attach pin %d to channel %d\n", pin, channel);
      return true;
    }
  }
  // No more channels available
  return false;
}

void analogWrite(uint8_t pin, int val)
{
  uint32_t channel = _analog_pin2chan(pin);
  if ( val >> (_pwm_bit_num-1) ) ++val;
  ledcWrite(channel + PWM_CHANNEL_OFFSET, val);
  // Serial.printf("write %d - %d\n",channel,val);
}

/*
  The primary goal of this library is to add phase control to PWM ledc
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

#include "driver/ledc.h"

#ifdef SOC_LEDC_SUPPORT_HS_MODE
#define LEDC_CHANNELS           (SOC_LEDC_CHANNEL_NUM<<1)
#else
#define LEDC_CHANNELS           (SOC_LEDC_CHANNEL_NUM)
#endif

// exported from Arduno Core
extern uint8_t channels_resolution[LEDC_CHANNELS];

void analogWritePhase(uint8_t pin, uint32_t duty, uint32_t phase)
{
  uint32_t chan = _analog_pin2chan(pin) + PWM_CHANNEL_OFFSET;
  if (duty >> (_pwm_bit_num-1) ) ++duty;

  if(chan >= LEDC_CHANNELS){
    return;
  }
  uint8_t group=(chan/8), channel=(chan%8);

  //Fixing if all bits in resolution is set = LEDC FULL ON
  uint32_t max_duty = (1 << channels_resolution[chan]) - 1;
  phase = phase % max_duty;

  if(duty == max_duty){     // no sure whether this is needed anymore TODO
    duty = max_duty + 1;
  }

  ledc_set_duty_with_hpoint((ledc_mode_t)group, (ledc_channel_t)channel, duty, phase);
  ledc_update_duty((ledc_mode_t)group, (ledc_channel_t)channel);
}
