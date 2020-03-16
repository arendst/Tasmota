/*
  support_switch.ino - switch support for Tasmota

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

#define SWITCH_V3
#ifdef SWITCH_V3
/*********************************************************************************************\
 * Switch support with input filter
 *
 * Inspired by (https://github.com/OLIMEX/olimex-iot-firmware-esp8266/blob/master/olimex/user/user_switch2.c)
\*********************************************************************************************/

const uint8_t SWITCH_PROBE_INTERVAL = 10;   // Time in milliseconds between switch input probe

#include <Ticker.h>

Ticker TickerSwitch;

struct SWITCH {
  unsigned long debounce = 0;                // Switch debounce timer
  uint16_t no_pullup_mask = 0;               // Switch pull-up bitmask flags
  uint8_t state[MAX_SWITCHES] = { 0 };
  uint8_t last_state[MAX_SWITCHES];          // Last wall switch states
  uint8_t hold_timer[MAX_SWITCHES] = { 0 };  // Timer for wallswitch push button hold
  uint8_t virtual_state[MAX_SWITCHES];       // Virtual switch states
  uint8_t present = 0;
} Switch;

/********************************************************************************************/

void SwitchPullupFlag(uint16 switch_bit)
{
  bitSet(Switch.no_pullup_mask, switch_bit);
}

void SwitchSetVirtual(uint32_t index, uint8_t state)
{
  Switch.virtual_state[index] = state;
}

uint8_t SwitchGetVirtual(uint32_t index)
{
  return Switch.virtual_state[index];
}

uint8_t SwitchLastState(uint32_t index)
{
  return Switch.last_state[index];
}

bool SwitchState(uint32_t index)
{
  uint32_t switchmode = Settings.switchmode[index];
  return ((FOLLOW_INV == switchmode) ||
          (PUSHBUTTON_INV == switchmode) ||
          (PUSHBUTTONHOLD_INV == switchmode) ||
          (FOLLOWMULTI_INV == switchmode) ||
          (PUSHHOLDMULTI_INV == switchmode) ||
          (PUSHON_INV == switchmode)
         ) ^ Switch.last_state[index];
}

/*********************************************************************************************/

void SwitchProbe(void)
{
  if (uptime < 4) { return; }                           // Block GPIO for 4 seconds after poweron to workaround Wemos D1 / Obi RTS circuit

  uint8_t state_filter = Settings.switch_debounce / SWITCH_PROBE_INTERVAL;   // 5, 10, 15
  uint8_t force_high = (Settings.switch_debounce % 50) &1;                   // 51, 101, 151 etc
  uint8_t force_low = (Settings.switch_debounce % 50) &2;                    // 52, 102, 152 etc

  for (uint32_t i = 0; i < MAX_SWITCHES; i++) {
    if (pin[GPIO_SWT1 +i] < 99) {
      // Olimex user_switch2.c code to fix 50Hz induced pulses
      if (1 == digitalRead(pin[GPIO_SWT1 +i])) {

        if (force_high) {                               // Enabled with SwitchDebounce x1
          if (1 == Switch.virtual_state[i]) {
            Switch.state[i] = state_filter;         // With noisy input keep current state 1 unless constant 0
          }
        }

        if (Switch.state[i] < state_filter) {
          Switch.state[i]++;
          if (state_filter == Switch.state[i]) {
            Switch.virtual_state[i] = 1;
          }
        }
      } else {

        if (force_low) {                                // Enabled with SwitchDebounce x2
          if (0 == Switch.virtual_state[i]) {
            Switch.state[i] = 0;                    // With noisy input keep current state 0 unless constant 1
          }
        }

        if (Switch.state[i] > 0) {
          Switch.state[i]--;
          if (0 == Switch.state[i]) {
            Switch.virtual_state[i] = 0;
          }
        }
      }
    }
  }
  TickerSwitch.attach_ms(SWITCH_PROBE_INTERVAL, SwitchProbe);  // Re-arm as core 2.3.0 does only support ONCE mode
}

void SwitchInit(void)
{
  Switch.present = 0;
  for (uint32_t i = 0; i < MAX_SWITCHES; i++) {
    Switch.last_state[i] = 1;  // Init global to virtual switch state;
    if (pin[GPIO_SWT1 +i] < 99) {
      Switch.present++;
      pinMode(pin[GPIO_SWT1 +i], bitRead(Switch.no_pullup_mask, i) ? INPUT : ((16 == pin[GPIO_SWT1 +i]) ? INPUT_PULLDOWN_16 : INPUT_PULLUP));
      Switch.last_state[i] = digitalRead(pin[GPIO_SWT1 +i]);  // Set global now so doesn't change the saved power state on first switch check
    }
    Switch.virtual_state[i] = Switch.last_state[i];
  }
  if (Switch.present) { TickerSwitch.attach_ms(SWITCH_PROBE_INTERVAL, SwitchProbe); }
}

/*********************************************************************************************\
 * Switch handler
\*********************************************************************************************/

void SwitchHandler(uint8_t mode)
{
  if (uptime < 4) { return; }                                  // Block GPIO for 4 seconds after poweron to workaround Wemos D1 / Obi RTS circuit

  uint16_t loops_per_second = 1000 / Settings.switch_debounce;

  for (uint32_t i = 0; i < MAX_SWITCHES; i++) {
    if ((pin[GPIO_SWT1 +i] < 99) || (mode)) {
      uint8_t button = Switch.virtual_state[i];
      uint8_t switchflag = POWER_TOGGLE +1;

      if (Switch.hold_timer[i]) {
        Switch.hold_timer[i]--;
        if (0 == Switch.hold_timer[i]) {

          switch (Settings.switchmode[i]) {
            case TOGGLEMULTI:
              switchflag = POWER_TOGGLE;     // Toggle after hold
              break;
            case FOLLOWMULTI:
              switchflag = button &1;        // Follow wall switch state after hold
              break;
            case FOLLOWMULTI_INV:
              switchflag = ~button &1;       // Follow inverted wall switch state after hold
              break;
            case PUSHHOLDMULTI:
              if (NOT_PRESSED == button) {
                Switch.hold_timer[i] = loops_per_second * Settings.param[P_HOLD_TIME] / 25;
                SendKey(KEY_SWITCH, i +1, POWER_INCREMENT);  // Execute command via MQTT
              } else {
                SendKey(KEY_SWITCH, i +1, POWER_CLEAR);  // Execute command via MQTT
              }
              break;
            case PUSHHOLDMULTI_INV:
              if (PRESSED == button) {
                Switch.hold_timer[i] = loops_per_second * Settings.param[P_HOLD_TIME] / 25;
                SendKey(KEY_SWITCH, i +1, POWER_INCREMENT);  // Execute command via MQTT
              } else {
                SendKey(KEY_SWITCH, i +1, POWER_CLEAR);  // Execute command via MQTT
              }
              break;
          default:
            SendKey(KEY_SWITCH, i +1, POWER_HOLD);  // Execute command via MQTT
            break;
          }
        }
      }

      if (button != Switch.last_state[i]) {  // This implies if ((PRESSED == button) then (NOT_PRESSED == Switch.last_state[i]))
        switch (Settings.switchmode[i]) {
        case TOGGLE:
        case PUSHBUTTON_TOGGLE:
          switchflag = POWER_TOGGLE;     // Toggle
          break;
        case FOLLOW:
          switchflag = button &1;        // Follow wall switch state
          break;
        case FOLLOW_INV:
          switchflag = ~button &1;       // Follow inverted wall switch state
          break;
        case PUSHBUTTON:
//          if ((PRESSED == button) && (NOT_PRESSED == Switch.last_state[i])) {
          if (PRESSED == button) {
            switchflag = POWER_TOGGLE;   // Toggle with pushbutton to Gnd
          }
          break;
        case PUSHBUTTON_INV:
//          if ((NOT_PRESSED == button) && (PRESSED == Switch.last_state[i])) {
          if (NOT_PRESSED == button) {
            switchflag = POWER_TOGGLE;   // Toggle with releasing pushbutton from Gnd
          }
          break;
        case PUSHBUTTONHOLD:
//          if ((PRESSED == button) && (NOT_PRESSED == Switch.last_state[i])) {
          if (PRESSED == button) {
            Switch.hold_timer[i] = loops_per_second * Settings.param[P_HOLD_TIME] / 10;  // Start timer on button press
          }
//          if ((NOT_PRESSED == button) && (PRESSED == Switch.last_state[i]) && (Switch.hold_timer[i])) {
          if ((NOT_PRESSED == button) && (Switch.hold_timer[i])) {
            Switch.hold_timer[i] = 0;    // Button released and hold timer not expired : stop timer...
            switchflag = POWER_TOGGLE;   // ...and Toggle
          }
          break;
        case PUSHBUTTONHOLD_INV:
//          if ((NOT_PRESSED == button) && (PRESSED == Switch.last_state[i])) {
          if (NOT_PRESSED == button) {
            Switch.hold_timer[i] = loops_per_second * Settings.param[P_HOLD_TIME] / 10;  // Start timer on button press...
          }
//          if ((PRESSED == button) && (NOT_PRESSED == Switch.last_state[i]) && (Switch.hold_timer[i])) {
          if ((PRESSED == button) && (Switch.hold_timer[i])) {
            Switch.hold_timer[i] = 0;    // Button released and hold timer not expired : stop timer.
            switchflag = POWER_TOGGLE;   // ...and Toggle
          }
          break;
/*
        // Reverted Fix switchmode 6 according to issue 7778 (#7831)
        case PUSHBUTTONHOLD_INV:
          if ((PRESSED == button) && (NOT_PRESSED == Switch.last_state[i])) {
            Switch.hold_timer[i] = loops_per_second * Settings.param[P_HOLD_TIME] / 10;  // Start timer on button press...
            switchflag = POWER_TOGGLE;   // ...and Toggle
          }
          if ((NOT_PRESSED == button) && (PRESSED == Switch.last_state[i])) {
            Switch.hold_timer[i] = 0;   // Button released : stop timer.
          }
          break;
*/
        case TOGGLEMULTI:
        case FOLLOWMULTI:
        case FOLLOWMULTI_INV:
          if (Switch.hold_timer[i]) {
            Switch.hold_timer[i] = 0;
            SendKey(KEY_SWITCH, i +1, POWER_HOLD);  // Execute command via MQTT
          } else {
            Switch.hold_timer[i] = loops_per_second / 2;  // 0.5 second multi press window
          }
          break;
        case PUSHHOLDMULTI:
//          if ((NOT_PRESSED == button) && (PRESSED == Switch.last_state[i])) {
          if (NOT_PRESSED == button) {
            if (Switch.hold_timer[i] != 0) {
              SendKey(KEY_SWITCH, i +1, POWER_INV);  // Execute command via MQTT
            }
            Switch.hold_timer[i] = loops_per_second * Settings.param[P_HOLD_TIME] / 10;
          }
//          if ((PRESSED == button) && (NOT_PRESSED == Switch.last_state[i])) {
          if (PRESSED == button) {
            if (Switch.hold_timer[i] > loops_per_second * Settings.param[P_HOLD_TIME] / 25) {
              switchflag = POWER_TOGGLE;   // Toggle with pushbutton
            }
            Switch.hold_timer[i] = loops_per_second * Settings.param[P_HOLD_TIME] / 10;
          }
          break;
        case PUSHHOLDMULTI_INV:
//          if ((PRESSED == button) && (NOT_PRESSED == Switch.last_state[i])) {
          if (PRESSED == button) {
            if (Switch.hold_timer[i] != 0) {
              SendKey(KEY_SWITCH, i +1, POWER_INV);  // Execute command via MQTT
            }
            Switch.hold_timer[i] = loops_per_second * Settings.param[P_HOLD_TIME] / 10;
          }
//          if ((NOT_PRESSED == button) && (PRESSED == Switch.last_state[i])) {
          if (NOT_PRESSED == button) {
            if (Switch.hold_timer[i] > loops_per_second * Settings.param[P_HOLD_TIME] / 25) {
              switchflag = POWER_TOGGLE;   // Toggle with pushbutton
            }
            Switch.hold_timer[i] = loops_per_second * Settings.param[P_HOLD_TIME] / 10;
          }
          break;
        case PUSHON:
          if (PRESSED == button) {
            switchflag = POWER_ON;       // Power ON with pushbutton to Gnd
          }
          break;
        case PUSHON_INV:
          if (NOT_PRESSED == button) {
            switchflag = POWER_ON;       // Power ON with releasing pushbutton from Gnd
          }
          break;
        }
        Switch.last_state[i] = button;
      }
      if (switchflag <= POWER_TOGGLE) {
        if (!SendKey(KEY_SWITCH, i +1, switchflag)) {  // Execute command via MQTT
          ExecuteCommandPower(i +1, switchflag, SRC_SWITCH);  // Execute command internally (if i < devices_present)
        }
      }
    }
  }
}

void SwitchLoop(void)
{
  if (Switch.present) {
    if (TimeReached(Switch.debounce)) {
      SetNextTimeInterval(Switch.debounce, Settings.switch_debounce);
      SwitchHandler(0);
    }
  }
}

#endif  // SWITCH_V3
