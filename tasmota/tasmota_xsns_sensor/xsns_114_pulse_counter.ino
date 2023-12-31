/*
  xsns_114_pulse_counter.ino - Pulse Counter Module sensor for Tasmota

  Copyright (C) 2024 David Roe

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

#ifdef ESP32
#ifdef USE_PULSE_COUNTER
/*********************************************************************************************\
 * Pulse Counter sensor
\*********************************************************************************************/

#define XSNS_114                 114

#include "driver/pcnt.h"

struct PulseCounter {
  uint32_t last_check = 0;
  uint16_t freq = 0;
};

PulseCounter PulseCounters[MAX_PULSE_COUNTER_MODULES];

void PulseCounterInit(void) {
  uint32_t now = micros();
  for (uint32_t i = 0; i < MAX_PULSE_COUNTER_MODULES; i++) {
    if (PinUsed(GPIO_PULSE_COUNTER, i)) {
      PulseCounters[i].last_check = now;

      pcnt_config_t pcnt_config = {
        .pulse_gpio_num = Pin(GPIO_PULSE_COUNTER, i),
        .pos_mode = PCNT_COUNT_INC,
        .unit = (pcnt_unit_t)i,
        .channel = PCNT_CHANNEL_0,
      };
      pcnt_unit_config(&pcnt_config);
      pcnt_set_filter_value((pcnt_unit_t)i, 1023);
      pcnt_filter_enable((pcnt_unit_t)i);
      pcnt_counter_clear((pcnt_unit_t)i);
      pinMode(Pin(GPIO_PULSE_COUNTER, i), INPUT); // No Pull Up
    }
  }
}

void PulseCounterShow(bool json) {
  int16_t count;
  uint32_t now = micros();
  for (uint32_t i = 0; i < MAX_PULSE_COUNTER_MODULES; i++) {
    if (PulseCounters[i].last_check > 0) {
      if (now > PulseCounters[i].last_check) {
        pcnt_get_counter_value((pcnt_unit_t)i, &count);
        PulseCounters[i].freq = (count * 1000000) / (now - PulseCounters[i].last_check);
      }
      pcnt_counter_clear((pcnt_unit_t)i);
      PulseCounters[i].last_check = now;

      if (json) {
        ResponseAppend_P(",\"PulseCounter%i\":{\"Frequency\":%i}", i + 1, PulseCounters[i].freq);

#ifdef USE_WEBSERVER
      } else {
        WSContentSend_P("{s}PulseCounter%i{m}%ihz", i + 1, PulseCounters[i].freq);
#endif  // USE_WEBSERVER
      }
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns114(uint32_t function) {
  bool result = false;

  switch (function) {
    case FUNC_JSON_APPEND:
      PulseCounterShow(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      PulseCounterShow(0);
      break;
#endif  // USE_WEBSERVER
    case FUNC_INIT:
      PulseCounterInit();
      break;
  }
  return result;
}
#endif  // USE_PULSE_COUNTER
#endif  // ESP32
