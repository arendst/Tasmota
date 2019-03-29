/*
  support_switch.ino - switch support for Sonoff-Tasmota

  Copyright (C) 2019  Theo Arends

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

#define SWITCH_V2
#ifdef SWITCH_V2
/*********************************************************************************************\
 * Switch support with input filter
 *
 * Inspired by (https://github.com/OLIMEX/olimex-iot-firmware-esp8266/blob/master/olimex/user/user_switch2.c)
\*********************************************************************************************/

#define SWITCH_PROBE_INTERVAL    10         // Time in milliseconds between switch input probe

#include <Ticker.h>

Ticker TickerSwitch;

unsigned long switch_debounce = 0;          // Switch debounce timer
uint16_t switch_no_pullup = 0;              // Switch pull-up bitmask flags
uint8_t switch_state_buf[MAX_SWITCHES] = { 0 };
uint8_t lastwallswitch[MAX_SWITCHES];       // Last wall switch states
uint8_t holdwallswitch[MAX_SWITCHES] = { 0 };  // Timer for wallswitch push button hold
uint8_t switch_virtual[MAX_SWITCHES];       // Virtual switch states
uint8_t switches_found = 0;

/********************************************************************************************/

void SwitchPullupFlag(uint16 switch_bit)
{
  bitSet(switch_no_pullup, switch_bit);
}

uint8_t SwitchLastState(uint8_t index)
{
  return lastwallswitch[index];
}

void SwitchSetVirtual(uint8_t index, uint8_t state)
{
  switch_virtual[index] = state;
}

uint8_t SwitchGetVirtual(uint8_t index)
{
  return switch_virtual[index];
}

/*********************************************************************************************/

void SwitchProbe(void)
{
  if (uptime < 4) { return; }                           // Block GPIO for 4 seconds after poweron to workaround Wemos D1 / Obi RTS circuit

  uint8_t state_filter = Settings.switch_debounce / SWITCH_PROBE_INTERVAL;   // 5, 10, 15
  uint8_t force_high = (Settings.switch_debounce % 50) &1;                   // 51, 101, 151 etc
  uint8_t force_low = (Settings.switch_debounce % 50) &2;                    // 52, 102, 152 etc

  for (uint8_t i = 0; i < MAX_SWITCHES; i++) {
    if (pin[GPIO_SWT1 +i] < 99) {
      // Olimex user_switch2.c code to fix 50Hz induced pulses
      if (1 == digitalRead(pin[GPIO_SWT1 +i])) {

        if (force_high) {                               // Enabled with SwitchDebounce x1
          if (1 == switch_virtual[i]) {
            switch_state_buf[i] = state_filter;         // With noisy input keep current state 1 unless constant 0
          }
        }

        if (switch_state_buf[i] < state_filter) {
          switch_state_buf[i]++;
          if (state_filter == switch_state_buf[i]) {
            switch_virtual[i] = 1;
          }
        }
      } else {

        if (force_low) {                                // Enabled with SwitchDebounce x2
          if (0 == switch_virtual[i]) {
            switch_state_buf[i] = 0;                    // With noisy input keep current state 0 unless constant 1
          }
        }

        if (switch_state_buf[i] > 0) {
          switch_state_buf[i]--;
          if (0 == switch_state_buf[i]) {
            switch_virtual[i] = 0;
          }
        }
      }
    }
  }
  TickerSwitch.attach_ms(SWITCH_PROBE_INTERVAL, SwitchProbe);  // Re-arm as core 2.3.0 does only support ONCE mode
}

void SwitchInit(void)
{
  switches_found = 0;
  for (uint8_t i = 0; i < MAX_SWITCHES; i++) {
    lastwallswitch[i] = 1;  // Init global to virtual switch state;
    if (pin[GPIO_SWT1 +i] < 99) {
      switches_found++;
      pinMode(pin[GPIO_SWT1 +i], bitRead(switch_no_pullup, i) ? INPUT : ((16 == pin[GPIO_SWT1 +i]) ? INPUT_PULLDOWN_16 : INPUT_PULLUP));
      lastwallswitch[i] = digitalRead(pin[GPIO_SWT1 +i]);  // Set global now so doesn't change the saved power state on first switch check
    }
    switch_virtual[i] = lastwallswitch[i];
  }
  if (switches_found) { TickerSwitch.attach_ms(SWITCH_PROBE_INTERVAL, SwitchProbe); }
}

/*********************************************************************************************\
 * Switch handler
\*********************************************************************************************/

void SwitchHandler(uint8_t mode)
{
  if (uptime < 4) { return; }                                  // Block GPIO for 4 seconds after poweron to workaround Wemos D1 / Obi RTS circuit

  uint8_t button = NOT_PRESSED;
  uint8_t switchflag;
  uint16_t loops_per_second = 1000 / Settings.switch_debounce;

  for (uint8_t i = 0; i < MAX_SWITCHES; i++) {
    if ((pin[GPIO_SWT1 +i] < 99) || (mode)) {

      if (holdwallswitch[i]) {
        holdwallswitch[i]--;
        if (0 == holdwallswitch[i]) {
          SendKey(1, i +1, 3);           // Execute command via MQTT
        }
      }

      button = switch_virtual[i];

// enum SwitchModeOptions {TOGGLE, FOLLOW, FOLLOW_INV, PUSHBUTTON, PUSHBUTTON_INV, PUSHBUTTONHOLD, PUSHBUTTONHOLD_INV, PUSHBUTTON_TOGGLE, MAX_SWITCH_OPTION};

      if (button != lastwallswitch[i]) {
        switchflag = 3;
        switch (Settings.switchmode[i]) {
        case TOGGLE:
          switchflag = 2;                // Toggle
          break;
        case FOLLOW:
          switchflag = button &1;        // Follow wall switch state
          break;
        case FOLLOW_INV:
          switchflag = ~button &1;       // Follow inverted wall switch state
          break;
        case PUSHBUTTON:
          if ((PRESSED == button) && (NOT_PRESSED == lastwallswitch[i])) {
            switchflag = 2;              // Toggle with pushbutton to Gnd
          }
          break;
        case PUSHBUTTON_INV:
          if ((NOT_PRESSED == button) && (PRESSED == lastwallswitch[i])) {
            switchflag = 2;              // Toggle with releasing pushbutton from Gnd
          }
          break;
        case PUSHBUTTON_TOGGLE:
          if (button != lastwallswitch[i]) {
            switchflag = 2;              // Toggle with any pushbutton change
          }
          break;
        case PUSHBUTTONHOLD:
          if ((PRESSED == button) && (NOT_PRESSED == lastwallswitch[i])) {
            holdwallswitch[i] = loops_per_second * Settings.param[P_HOLD_TIME] / 10;
          }
          if ((NOT_PRESSED == button) && (PRESSED == lastwallswitch[i]) && (holdwallswitch[i])) {
            holdwallswitch[i] = 0;
            switchflag = 2;              // Toggle with pushbutton to Gnd
          }
          break;
        case PUSHBUTTONHOLD_INV:
          if ((NOT_PRESSED == button) && (PRESSED == lastwallswitch[i])) {
            holdwallswitch[i] = loops_per_second * Settings.param[P_HOLD_TIME] / 10;
          }
          if ((PRESSED == button) && (NOT_PRESSED == lastwallswitch[i]) && (holdwallswitch[i])) {
            holdwallswitch[i] = 0;
            switchflag = 2;              // Toggle with pushbutton to Gnd
          }
          break;
        }

        if (switchflag < 3) {
          if (!SendKey(1, i +1, switchflag)) {  // Execute command via MQTT
            ExecuteCommandPower(i +1, switchflag, SRC_SWITCH);  // Execute command internally (if i < devices_present)
          }
        }

        lastwallswitch[i] = button;
      }
    }
  }
}

void SwitchLoop(void)
{
  if (switches_found) {
    if (TimeReached(switch_debounce)) {
      SetNextTimeInterval(switch_debounce, Settings.switch_debounce);
      SwitchHandler(0);
    }
  }
}

#endif  // SWITCH_V2
