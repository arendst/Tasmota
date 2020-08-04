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
#pragma once
#ifdef ESP32
// my debug Stuff
#define Serial_Debug1(p) Serial.printf p
#define Serial_DebugX(p)

//
// basics
//
// dummy defines
#define SPIFFS_END (SPI_FLASH_SEC_SIZE * 200)
#define SETTINGS_LOCATION SPIFFS_END

#include <Esp.h>

/*********************************************************************************************\
 * ESP32 analogWrite emulation support
\*********************************************************************************************/

#define PWM_SUPPORTED_CHANNELS 8
#define PWM_CHANNEL_OFFSET     2   // Webcam uses channel 0, so we offset standard PWM

uint8_t _pwm_channel[PWM_SUPPORTED_CHANNELS] = { 99, 99, 99, 99, 99, 99, 99, 99 };
uint32_t _pwm_frequency = 977;     // Default 977Hz
uint8_t _pwm_bit_num = 10;         // Default 1023

inline uint32_t _analog_pin2chan(uint32_t pin) {
  for (uint32_t channel = 0; channel < PWM_SUPPORTED_CHANNELS; channel++) {
    if ((_pwm_channel[channel] < 99) && (_pwm_channel[channel] == pin)) {
      return channel;
    }
  }
  return 0;
}

inline void _analogWriteFreqRange(void) {
  for (uint32_t channel = 0; channel < PWM_SUPPORTED_CHANNELS; channel++) {
    if (_pwm_channel[channel] < 99) {
//      uint32_t duty = ledcRead(channel + PWM_CHANNEL_OFFSET);
      ledcSetup(channel + PWM_CHANNEL_OFFSET, _pwm_frequency, _pwm_bit_num);
//      ledcWrite(channel + PWM_CHANNEL_OFFSET, duty);
    }
  }
//  Serial.printf("freq - range %d - %d\n",freq,range);
}

// input range is in full range, ledc needs bits
inline uint32_t _analogGetResolution(uint32_t x) {
  uint32_t bits = 0;
  while (x) {
    bits++;
    x >>= 1;
  }
  return bits;
}

inline void analogWriteRange(uint32_t range) {
  _pwm_bit_num = _analogGetResolution(range);
  _analogWriteFreqRange();
}

inline void analogWriteFreq(uint32_t freq) {
  _pwm_frequency = freq;
  _analogWriteFreqRange();
}

inline void analogAttach(uint32_t pin, uint32_t channel) {
  _pwm_channel[channel &7] = pin;
  ledcAttachPin(pin, channel + PWM_CHANNEL_OFFSET);
  ledcSetup(channel + PWM_CHANNEL_OFFSET, _pwm_frequency, _pwm_bit_num);
//  Serial.printf("attach %d - %d\n", channel, pin);
}

inline void analogWrite(uint8_t pin, int val)
{
  uint32_t channel = _analog_pin2chan(pin);
  ledcWrite(channel + PWM_CHANNEL_OFFSET, val);
//  Serial.printf("write %d - %d\n",channel,val);
}

/*********************************************************************************************/

#define INPUT_PULLDOWN_16 INPUT_PULLUP

typedef double real64_t;

//
// Time and Timer
//
#define ETS_UART_INTR_DISABLE()
#define ETS_UART_INTR_ENABLE()

#define ESPhttpUpdate httpUpdate
#define getFlashChipRealSize() getFlashChipSize()

#define os_delay_us ets_delay_us
// Serial minimal type to hold the config
typedef int SerConfu8;
typedef int SerialConfig;
//#define analogWrite(a, b)

//
// WS2812
//
#define NeoEsp8266BitBang800KbpsMethod NeoEsp32BitBang800KbpsMethod
//
// UDP
//
//#define PortUdp_writestr(log_data) PortUdp.write((const uint8_t *)(log_data), strlen(log_data))
#define PortUdp_write(log_data, n) PortUdp.write((const uint8_t *)(log_data), n)

//
#define wifi_forceSleepBegin()

#undef LWIP_IPV6

#define REASON_DEFAULT_RST      0  // "Power on"                normal startup by power on
#define REASON_WDT_RST          1  // "Hardware Watchdog"       hardware watch dog reset
#define REASON_EXCEPTION_RST    2  // "Exception"               exception reset, GPIO status won’t change
#define REASON_SOFT_WDT_RST     3  // "Software Watchdog"       software watch dog reset, GPIO status won’t change
#define REASON_SOFT_RESTART     4  // "Software/System restart" software restart ,system_restart , GPIO status won’t change
#define REASON_DEEP_SLEEP_AWAKE 5  // "Deep-Sleep Wake"         wake up from deep-sleep
#define REASON_EXT_SYS_RST      6  // "External System"         external system reset

// memmove ...
#define memcpy_P memcpy
#define memmove_P memmove
#define strncpy_P strncpy
#define strcmp_P strcmp
#define memccpy_P memccpy
#define snprintf_P snprintf
#define sprintf_P sprintf
#define strncmp_P strncmp

// LWIP STuff

#define STATION_IF 0

#endif
