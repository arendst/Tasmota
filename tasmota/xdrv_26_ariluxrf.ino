/*
  xdrv_26_ariluxrf.ino - Arilux Rf support for Tasmota

  Copyright (C) 2020  Theo Arends

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

#ifdef USE_LIGHT
#ifdef USE_ARILUX_RF
/*********************************************************************************************\
 * Arilux LC11 Rf support stripped from RCSwitch library
\*********************************************************************************************/

#define XDRV_26              26

const uint32_t ARILUX_RF_TIME_AVOID_DUPLICATE = 1000;  // Milliseconds

const uint8_t ARILUX_RF_MAX_CHANGES = 51;              // Pulses (sync + 2 x 24 bits)
const uint32_t ARILUX_RF_SEPARATION_LIMIT = 4300;      // Microseconds
const uint32_t ARILUX_RF_RECEIVE_TOLERANCE = 60;       // Percentage

struct ARILUX {
  unsigned int rf_timings[ARILUX_RF_MAX_CHANGES];

  unsigned long rf_received_value = 0;
  unsigned long rf_last_received_value = 0;
  unsigned long rf_last_time = 0;
  unsigned long rf_lasttime = 0;

  unsigned int rf_change_count = 0;
  unsigned int rf_repeat_count = 0;

  uint8_t rf_toggle = 0;
} Arilux;

#ifndef ARDUINO_ESP8266_RELEASE_2_3_0          // Fix core 2.5.x ISR not in IRAM Exception
#ifndef USE_WS2812_DMA                         // Collides with Neopixelbus but solves RF misses
void AriluxRfInterrupt(void) ICACHE_RAM_ATTR;  // As iram is tight and it works this way too
#endif  // USE_WS2812_DMA
#endif  // ARDUINO_ESP8266_RELEASE_2_3_0

void AriluxRfInterrupt(void)
{
  unsigned long time = micros();
  unsigned int duration = time - Arilux.rf_lasttime;

  if (duration > ARILUX_RF_SEPARATION_LIMIT) {
    if (abs(duration - Arilux.rf_timings[0]) < 200) {
      Arilux.rf_repeat_count++;
      if (Arilux.rf_repeat_count == 2) {
        unsigned long code = 0;
        const unsigned int delay = Arilux.rf_timings[0] / 31;
        const unsigned int delayTolerance = delay * ARILUX_RF_RECEIVE_TOLERANCE / 100;
        for (unsigned int i = 1; i < Arilux.rf_change_count -1; i += 2) {
          code <<= 1;
          if (abs(Arilux.rf_timings[i] - (delay *3)) < delayTolerance && abs(Arilux.rf_timings[i +1] - delay) < delayTolerance) {
            code |= 1;
          }
        }
        if (Arilux.rf_change_count > 49) {  // Need 1 sync bit and 24 data bits
          Arilux.rf_received_value = code;
        }
        Arilux.rf_repeat_count = 0;
      }
    }
    Arilux.rf_change_count = 0;
  }
  if (Arilux.rf_change_count >= ARILUX_RF_MAX_CHANGES) {
    Arilux.rf_change_count = 0;
    Arilux.rf_repeat_count = 0;
  }
  Arilux.rf_timings[Arilux.rf_change_count++] = duration;
  Arilux.rf_lasttime = time;
}

void AriluxRfHandler(void)
{
  unsigned long now = millis();
  if (Arilux.rf_received_value && !((Arilux.rf_received_value == Arilux.rf_last_received_value) && (now - Arilux.rf_last_time < ARILUX_RF_TIME_AVOID_DUPLICATE))) {
    Arilux.rf_last_received_value = Arilux.rf_received_value;
    Arilux.rf_last_time = now;

    uint16_t hostcode = Arilux.rf_received_value >> 8 & 0xFFFF;
    if (Settings.rf_code[1][6] == Settings.rf_code[1][7]) {
      Settings.rf_code[1][6] = hostcode >> 8 & 0xFF;
      Settings.rf_code[1][7] = hostcode & 0xFF;
    }
    uint16_t stored_hostcode = Settings.rf_code[1][6] << 8 | Settings.rf_code[1][7];

    DEBUG_DRIVER_LOG(PSTR(D_LOG_RFR D_HOST D_CODE " 0x%04X, " D_RECEIVED " 0x%06X"), stored_hostcode, Arilux.rf_received_value);

    if (hostcode == stored_hostcode) {
      char command[33];
      char value = '-';
      command[0] = '\0';
      uint8_t  keycode = Arilux.rf_received_value & 0xFF;
      switch (keycode) {
        case 1:  // Power On
        case 3:  // Power Off
          snprintf_P(command, sizeof(command), PSTR(D_CMND_POWER " %d"), (1 == keycode) ? 1 : 0);
          break;
        case 2:  // Toggle
          Arilux.rf_toggle++;
          Arilux.rf_toggle &= 0x3;
          snprintf_P(command, sizeof(command), PSTR(D_CMND_COLOR " %d"), 200 + Arilux.rf_toggle);
          break;
        case 4:  // Speed +
          value = '+';
        case 7:  // Speed -
          snprintf_P(command, sizeof(command), PSTR(D_CMND_SPEED " %c"), value);
          break;
        case 5:  // Scheme +
          value = '+';
        case 8:  // Scheme -
          snprintf_P(command, sizeof(command), PSTR(D_CMND_SCHEME " %c"), value);
          break;
        case 6:  // Dimmer +
          value = '+';
        case 9:  // Dimmer -
          snprintf_P(command, sizeof(command), PSTR(D_CMND_DIMMER " %c"), value);
          break;
        default: {
          if ((keycode >= 10) && (keycode <= 21)) {
            snprintf_P(command, sizeof(command), PSTR(D_CMND_COLOR " %d"), keycode -9);
          }
        }
      }
      if (strlen(command)) {
        ExecuteCommand(command, SRC_LIGHT);
      }
    }
  }
  Arilux.rf_received_value = 0;
}

void AriluxRfInit(void)
{
  if (PinUsed(GPIO_ARIRFRCV) && PinUsed(GPIO_ARIRFSEL)) {
    if (Settings.last_module != Settings.module) {
      Settings.rf_code[1][6] = 0;
      Settings.rf_code[1][7] = 0;
      Settings.last_module = Settings.module;
    }
    Arilux.rf_received_value = 0;

    digitalWrite(Pin(GPIO_ARIRFSEL), 0);  // Turn on RF
    attachInterrupt(Pin(GPIO_ARIRFRCV), AriluxRfInterrupt, CHANGE);
  }
}

void AriluxRfDisable(void)
{
  if (PinUsed(GPIO_ARIRFRCV) && PinUsed(GPIO_ARIRFSEL)) {
    detachInterrupt(Pin(GPIO_ARIRFRCV));
    digitalWrite(Pin(GPIO_ARIRFSEL), 1);  // Turn off RF
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv26(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_EVERY_50_MSECOND:
      if (PinUsed(GPIO_ARIRFRCV)) { AriluxRfHandler(); }
      break;
    case FUNC_EVERY_SECOND:
      if (10 == uptime) { AriluxRfInit(); }  // Needs rest before enabling RF interrupts
      break;
  }
  return result;
}

#endif  // USE_ARILUX_RF
#endif  // USE_LIGHT