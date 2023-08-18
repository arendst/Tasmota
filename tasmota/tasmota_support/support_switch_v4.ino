/*
  support_switch.ino - switch support for Tasmota

  Copyright (C) 2021  Theo Arends

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

#define SWITCH_V4
#ifdef SWITCH_V4
/*********************************************************************************************\
 * Switch support with input filter
 *
 * Inspired by (https://github.com/OLIMEX/olimex-iot-firmware-esp8266/blob/master/olimex/user/user_switch2.c)
\*********************************************************************************************/

const uint8_t SWITCH_PROBE_INTERVAL = 10;      // Time in milliseconds between switch input probe
const uint8_t SWITCH_FAST_PROBE_INTERVAL = 2;  // Time in milliseconds between switch input probe for AC detection
const uint8_t SWITCH_AC_PERIOD = (20 + SWITCH_FAST_PROBE_INTERVAL - 1) / SWITCH_FAST_PROBE_INTERVAL;   // Duration of an AC wave in probe intervals

// Switch Mode definietions
#define SM_TIMER_MASK         0x3F
#define SM_NO_TIMER_MASK      0xFF
#define SM_FIRST_PRESS        0x40
#define SM_SECOND_PRESS       0x80
#define POWER_NONE            99

const char kSwitchPressStates[] PROGMEM =
  "||||POWER_INCREMENT|POWER_INV|POWER_CLEAR|POWER_RELEASE|POWER_100||POWER_DELAYED";

#include <Ticker.h>

Ticker TickerSwitch;

struct SWITCH {
  uint32_t debounce = 0;                         // Switch debounce timer
  uint32_t no_pullup_mask = 0;                   // Switch pull-up bitmask flags
  uint32_t pulldown_mask = 0;                    // Switch pull-down bitmask flags
  uint32_t used = 0;                             // Switch used bitmask
  uint32_t virtual_pin = 0;                      // Switch state bitmask
  uint8_t state[MAX_SWITCHES_SET] = { 0 };
  uint8_t last_state[MAX_SWITCHES_SET];          // Last wall switch states
  uint8_t hold_timer[MAX_SWITCHES_SET] = { 0 };  // Timer for wallswitch push button hold
  uint8_t debounced_state[MAX_SWITCHES_SET];     // Switch debounced states
  uint8_t first_change = 0;
  bool probe_mutex;
} Switch;

/********************************************************************************************/

void SwitchPullupFlag(uint32 switch_bit) {
  bitSet(Switch.no_pullup_mask, switch_bit);
}

void SwitchPulldownFlag(uint32 switch_bit) {
  bitSet(Switch.pulldown_mask, switch_bit);
}

/*------------------------------------------------------------------------------------------*/

void SwitchSetVirtualPinState(uint32_t index, uint32_t state) {
  // Set virtual pin state to be debounced as used by early detected switches
  bitWrite(Switch.virtual_pin, index, state);
}

uint8_t SwitchLastState(uint32_t index) {
  // Get last state
  return Switch.last_state[index];
}

uint8_t SwitchGetState(uint32_t index) {
  // Get current state
  return Switch.debounced_state[index];
}

void SwitchSetState(uint32_t index, uint32_t state) {
  // Set debounced pin state to be used by late detected switches
  if (!bitRead(Switch.used, index)) {
    for (uint32_t i = 0; i <= index; i++) {
      if (!bitRead(Switch.used, i)) {
        bitSet(Switch.used, i);
        AddLog(LOG_LEVEL_DEBUG, PSTR("SWT: Add vSwitch%d, State %d"), i +1, Switch.debounced_state[i]);
      }
    }
  }
  Switch.debounced_state[index] = state;
}

/*------------------------------------------------------------------------------------------*/

bool SwitchUsed(uint32_t index) {
  return bitRead(Switch.used, index);
}

bool SwitchState(uint32_t index) {
  uint32_t switchmode = Settings->switchmode[index];
  return ((FOLLOW_INV == switchmode) ||
          (PUSHBUTTON_INV == switchmode) ||
          (PUSHBUTTONHOLD_INV == switchmode) ||
          (FOLLOWMULTI_INV == switchmode) ||
          (PUSHHOLDMULTI_INV == switchmode) ||
          (PUSHON_INV == switchmode) ||
          (PUSH_IGNORE_INV == switchmode)
         ) ^ Switch.last_state[index];
}

/*********************************************************************************************/

void SwitchProbe(void) {
  if (Switch.probe_mutex || (TasmotaGlobal.uptime < 4)) { return; }  // Block GPIO for 4 seconds after poweron to workaround Wemos D1 / Obi RTS circuit
  Switch.probe_mutex = true;

  uint32_t state_filter;
  uint32_t first_change = Switch.first_change;
  uint32_t debounce_flags = Settings->switch_debounce % 10;
  bool force_high = (debounce_flags &1);                     // 51, 101, 151 etc
  bool force_low = (debounce_flags &2);                      // 52, 102, 152 etc
  bool ac_detect = (debounce_flags == 9);

  if (ac_detect) {
    if (Settings->switch_debounce < 2 * SWITCH_AC_PERIOD * SWITCH_FAST_PROBE_INTERVAL + 9) {
      state_filter = 2 * SWITCH_AC_PERIOD;
    } else if (Settings->switch_debounce > (0x7f - 2 * SWITCH_AC_PERIOD) * SWITCH_FAST_PROBE_INTERVAL) {
      state_filter = 0x7f;
    } else {
      state_filter = (Settings->switch_debounce - 9) / SWITCH_FAST_PROBE_INTERVAL;
    }
  } else {
    state_filter = Settings->switch_debounce / SWITCH_PROBE_INTERVAL;	// 5, 10, 15
  }

  uint32_t not_activated;
  for (uint32_t i = 0; i < MAX_SWITCHES_SET; i++) {
    if (!bitRead(Switch.used, i)) { continue; }

    if (PinUsed(GPIO_SWT1, i)) {
      not_activated = digitalRead(Pin(GPIO_SWT1, i));
    } else {
      not_activated = bitRead(Switch.virtual_pin, i);
    }

    // Olimex user_switch2.c code to fix 50Hz induced pulses
    if (not_activated) {

      if (ac_detect) {                                     // Enabled with SwitchDebounce x9
        Switch.state[i] |= 0x80;
        if (Switch.state[i] > 0x80) {
          Switch.state[i]--;
          if (0x80 == Switch.state[i]) {
            Switch.debounced_state[i] = 0;
            Switch.first_change = false;
          }
        }
      } else {

        if (force_high) {                                  // Enabled with SwitchDebounce x1
          if (1 == Switch.debounced_state[i]) {
            Switch.state[i] = state_filter;                // With noisy input keep current state 1 unless constant 0
          }
        }

        if (Switch.state[i] < state_filter) {
          Switch.state[i]++;
          if (state_filter == Switch.state[i]) {
            Switch.debounced_state[i] = 1;
          }
        }
      }
    } else {

      if (ac_detect) {                                     // Enabled with SwitchDebounce x9
        /*
          * Moes MS-104B and similar devices using an AC detection circuitry
          * on their switch inputs generating an ~4 ms long low pulse every
          * AC wave. We start the time measurement on the falling edge.
          *
          * state: bit7: previous state, bit6..0: counter
          */
        if (Switch.state[i] & 0x80) {
          Switch.state[i] &= 0x7f;
          if (Switch.state[i] < state_filter - 2 * SWITCH_AC_PERIOD) {
            Switch.state[i] += 2 * SWITCH_AC_PERIOD;
          } else {
            Switch.state[i] = state_filter;
            Switch.debounced_state[i] = 1;
            if (first_change) {
              Switch.last_state[i] = 1;
              Switch.first_change = false;
            }
          }
        } else {
          if (Switch.state[i] > 0x00) {
            Switch.state[i]--;
            if (0x00 == Switch.state[i]) {
              Switch.debounced_state[i] = 0;
              Switch.first_change = false;
            }
          }
        }
      } else {

        if (force_low) {                                   // Enabled with SwitchDebounce x2
          if (0 == Switch.debounced_state[i]) {
            Switch.state[i] = 0;                           // With noisy input keep current state 0 unless constant 1
          }
        }

        if (Switch.state[i] > 0) {
          Switch.state[i]--;
          if (0 == Switch.state[i]) {
            Switch.debounced_state[i] = 0;
          }
        }
      }
    }
  }
  Switch.probe_mutex = false;
}

void SwitchInit(void) {
  bool ac_detect = (Settings->switch_debounce % 10 == 9);
  Switch.used = 0;
  for (uint32_t i = 0; i < MAX_SWITCHES_SET; i++) {
    Switch.last_state[i] = NOT_PRESSED;
    if (PinUsed(GPIO_SWT1, i)) {
      bitSet(Switch.used, i);            // This pin is used
#ifdef ESP8266
      pinMode(Pin(GPIO_SWT1, i), bitRead(Switch.no_pullup_mask, i) ? INPUT : ((16 == Pin(GPIO_SWT1, i)) ? INPUT_PULLDOWN_16 : INPUT_PULLUP));
#endif  // ESP8266
#ifdef ESP32
      pinMode(Pin(GPIO_SWT1, i), bitRead(Switch.pulldown_mask, i) ? INPUT_PULLDOWN : bitRead(Switch.no_pullup_mask, i) ? INPUT : INPUT_PULLUP);
#endif  // ESP32
      Switch.last_state[i] = digitalRead(Pin(GPIO_SWT1, i));  // Set global now so doesn't change the saved power state on first switch check
      if (ac_detect) {
        Switch.state[i] = 0x80 + 2 * SWITCH_AC_PERIOD;
        Switch.last_state[i] = 0;				 // Will set later in the debouncing code
      }
    }
    else {
      XdrvMailbox.index = i;
      if (XdrvCall(FUNC_ADD_SWITCH)) {
        /*
           At entry:
           XdrvMailbox.index = switch index
           At exit:
           XdrvMailbox.index bit 0 = current state
        */
        bitSet(Switch.used, i);          // This pin is used
        bool state = (XdrvMailbox.index &1);
        SwitchSetVirtualPinState(i, state);  // Virtual hardware pin state
        Switch.last_state[i] = bitRead(Switch.virtual_pin, i);

        AddLog(LOG_LEVEL_DEBUG, PSTR("SWT: Add vSwitch%d, State %d"), i +1, Switch.last_state[i]);
      }
    }
    Switch.debounced_state[i] = Switch.last_state[i];
  }

//  AddLog(LOG_LEVEL_DEBUG, PSTR("BTN: vPinUsed %08X, State %08X"), Switch.used, Switch.virtual_pin);

  if (Switch.used) {                     // Any bit set
    Switch.first_change = true;
    TickerSwitch.attach_ms((ac_detect) ? SWITCH_FAST_PROBE_INTERVAL : SWITCH_PROBE_INTERVAL, SwitchProbe);
  }
}

/*********************************************************************************************\
 * Switch handler
\*********************************************************************************************/

void SwitchHandler(void) {
  if (TasmotaGlobal.uptime < 4) { return; }                  // Block GPIO for 4 seconds after poweron to workaround Wemos D1 / Obi RTS circuit

  uint32_t loops_per_second = 1000 / Settings->switch_debounce;

  for (uint32_t i = 0; i < MAX_SWITCHES_SET; i++) {
    if (!bitRead(Switch.used, i)) { continue; }

    uint32_t button = Switch.debounced_state[i];
    uint32_t switchflag = POWER_TOGGLE +1;
    uint32_t mqtt_action = POWER_NONE;
    uint32_t switchmode = Settings->switchmode[i];

    if (Switch.hold_timer[i] & (((switchmode == PUSHHOLDMULTI) | (switchmode == PUSHHOLDMULTI_INV)) ? SM_TIMER_MASK: SM_NO_TIMER_MASK)) {
      Switch.hold_timer[i]--;
      if ((Switch.hold_timer[i] & SM_TIMER_MASK) == loops_per_second * Settings->param[P_HOLD_TIME] / 25) {
        if ((switchmode == PUSHHOLDMULTI) | (switchmode == PUSHHOLDMULTI_INV)){
          if (((switchmode == PUSHHOLDMULTI) & (NOT_PRESSED == Switch.last_state[i])) | ((switchmode == PUSHHOLDMULTI_INV) & (PRESSED == Switch.last_state[i]))) {
            SendKey(KEY_SWITCH, i +1, POWER_INCREMENT);      // Execute command via MQTT
          }
          else if ((Switch.hold_timer[i] & ~SM_TIMER_MASK) == SM_FIRST_PRESS) {
              SendKey(KEY_SWITCH, i +1, POWER_DELAYED);      // Execute command via MQTT
              mqtt_action = POWER_DELAYED;
              Switch.hold_timer[i] = 0;
          }
        }
      }
      if (0 == (Switch.hold_timer[i] & (((switchmode == PUSHHOLDMULTI) | (switchmode == PUSHHOLDMULTI_INV)) ? SM_TIMER_MASK: SM_NO_TIMER_MASK))) {
        switch (switchmode) {
          case TOGGLEMULTI:
            switchflag = POWER_TOGGLE;                     // Toggle after hold
            break;
          case FOLLOWMULTI:
            switchflag = button &1;                        // Follow wall switch state after hold
            break;
          case FOLLOWMULTI_INV:
            switchflag = ~button &1;                       // Follow inverted wall switch state after hold
            break;
          case PUSHHOLDMULTI:
            if (NOT_PRESSED == button) {
              Switch.hold_timer[i] = loops_per_second * Settings->param[P_HOLD_TIME] / 25;
              SendKey(KEY_SWITCH, i +1, POWER_INCREMENT);  // Execute command via MQTT
              mqtt_action = POWER_INCREMENT;
            } else {
              Switch.hold_timer[i]= 0;
              SendKey(KEY_SWITCH, i +1, POWER_CLEAR);      // Execute command via MQTT
              mqtt_action = POWER_CLEAR;
            }
            break;
          case PUSHHOLDMULTI_INV:
            if (PRESSED == button) {
              Switch.hold_timer[i] = loops_per_second * Settings->param[P_HOLD_TIME] / 25;
              SendKey(KEY_SWITCH, i +1, POWER_INCREMENT);  // Execute command via MQTT
              mqtt_action = POWER_INCREMENT;
            } else {
              Switch.hold_timer[i]= 0;
              SendKey(KEY_SWITCH, i +1, POWER_CLEAR);      // Execute command via MQTT
              mqtt_action = POWER_CLEAR;
            }
            break;
        default:
          SendKey(KEY_SWITCH, i +1, POWER_HOLD);           // Execute command via MQTT
          mqtt_action = POWER_HOLD;
          break;
        }
      }
    }

    if (button != Switch.last_state[i]) {  // This implies if ((PRESSED == button) then (NOT_PRESSED == Switch.last_state[i]))
      switch (switchmode) {
      case TOGGLE:                         // SwitchMode 0
      case PUSHBUTTON_TOGGLE:              // SwitchMode 7
        switchflag = POWER_TOGGLE;         // Toggle
        break;
      case FOLLOW:                         // SwitchMode 1
        switchflag = button &1;            // Follow wall switch state
        break;
      case FOLLOW_INV:                     // SwitchMode 2
        switchflag = ~button &1;           // Follow inverted wall switch state
        break;
      case PUSHBUTTON:                     // SwitchMode 3
        if (PRESSED == button) {
          switchflag = POWER_TOGGLE;       // Toggle with pushbutton to Gnd
        }
        break;
      case PUSHBUTTON_INV:                 // SwitchMode 4
        if (NOT_PRESSED == button) {
          switchflag = POWER_TOGGLE;       // Toggle with releasing pushbutton from Gnd
        }
        break;
      case PUSHBUTTONHOLD:                 // SwitchMode 5
        if (PRESSED == button) {
          Switch.hold_timer[i] = loops_per_second * Settings->param[P_HOLD_TIME] / 10;  // Start timer on button press
        }
        if ((NOT_PRESSED == button) && (Switch.hold_timer[i])) {
          Switch.hold_timer[i] = 0;        // Button released and hold timer not expired : stop timer...
          switchflag = POWER_TOGGLE;       // ...and Toggle
        }
        break;
      case PUSHBUTTONHOLD_INV:             // SwitchMode 6
        if (NOT_PRESSED == button) {
          Switch.hold_timer[i] = loops_per_second * Settings->param[P_HOLD_TIME] / 10;  // Start timer on button press...
        }
        if ((PRESSED == button) && (Switch.hold_timer[i])) {
          Switch.hold_timer[i] = 0;        // Button released and hold timer not expired : stop timer.
          switchflag = POWER_TOGGLE;       // ...and Toggle
        }
        break;
      case TOGGLEMULTI:                    // SwitchMode 8
      case FOLLOWMULTI:                    // SwitchMode 9
      case FOLLOWMULTI_INV:                // SwitchMode 10
        if (Switch.hold_timer[i]) {
          Switch.hold_timer[i] = 0;
          SendKey(KEY_SWITCH, i +1, POWER_HOLD);              // Execute command via MQTT
          mqtt_action = POWER_HOLD;
        } else {
          Switch.hold_timer[i] = loops_per_second / 2;        // 0.5 second multi press window
        }
        break;
      case PUSHHOLDMULTI:                  // SwitchMode 11
        if (NOT_PRESSED == button) {
          if ((Switch.hold_timer[i] & SM_TIMER_MASK) != 0) {
            Switch.hold_timer[i] = ((Switch.hold_timer[i] & ~SM_TIMER_MASK) == SM_FIRST_PRESS) ? SM_SECOND_PRESS : 0;
            SendKey(KEY_SWITCH, i +1, POWER_INV);             // Execute command via MQTT
            mqtt_action = POWER_INV;
          }
        } else {
          if ((Switch.hold_timer[i] & SM_TIMER_MASK) > loops_per_second * Settings->param[P_HOLD_TIME] / 25) {
            if ((Switch.hold_timer[i] & ~SM_TIMER_MASK) != SM_SECOND_PRESS) {
              Switch.hold_timer[i]= SM_FIRST_PRESS;
              switchflag = POWER_TOGGLE;                      // Toggle with pushbutton
            }
            else{
              SendKey(KEY_SWITCH, i +1, POWER_100);           // Execute command via MQTT
              mqtt_action = POWER_100;
              Switch.hold_timer[i]= 0;
            }
          } else {
            Switch.hold_timer[i]= 0;
            SendKey(KEY_SWITCH, i +1, POWER_RELEASE);         // Execute command via MQTT
            mqtt_action = POWER_RELEASE;
          }
        }
        Switch.hold_timer[i] = (Switch.hold_timer[i] & ~SM_TIMER_MASK) | loops_per_second * Settings->param[P_HOLD_TIME] / 10;
        break;
      case PUSHHOLDMULTI_INV:              // SwitchMode 12
        if (PRESSED == button) {
          if ((Switch.hold_timer[i] & SM_TIMER_MASK) != 0) {
            Switch.hold_timer[i] = ((Switch.hold_timer[i] & ~SM_TIMER_MASK) == SM_FIRST_PRESS) ? SM_SECOND_PRESS : 0;
            SendKey(KEY_SWITCH, i +1, POWER_INV);             // Execute command via MQTT
            mqtt_action = POWER_INV;
          }
        } else {
          if ((Switch.hold_timer[i] & SM_TIMER_MASK)> loops_per_second * Settings->param[P_HOLD_TIME] / 25) {
            if ((Switch.hold_timer[i] & ~SM_TIMER_MASK) != SM_SECOND_PRESS) {
              Switch.hold_timer[i]= SM_FIRST_PRESS;
              switchflag = POWER_TOGGLE;                      // Toggle with pushbutton
            }
            else{
              SendKey(KEY_SWITCH, i +1, POWER_100);           // Execute command via MQTT
              mqtt_action = POWER_100;
              Switch.hold_timer[i]= 0;
            }
          } else {
            Switch.hold_timer[i]= 0;
            SendKey(KEY_SWITCH, i +1, POWER_RELEASE);         // Execute command via MQTT
            mqtt_action = POWER_RELEASE;
          }
        }
        Switch.hold_timer[i] = (Switch.hold_timer[i] & ~SM_TIMER_MASK) | loops_per_second * Settings->param[P_HOLD_TIME] / 10;
        break;
      case PUSHON:                         // SwitchMode 13
        if (PRESSED == button) {
          switchflag = POWER_ON;                              // Power ON with pushbutton to Gnd
        }
        break;
      case PUSHON_INV:                     // SwitchMode 14
        if (NOT_PRESSED == button) {
          switchflag = POWER_ON;                              // Power ON with releasing pushbutton from Gnd
        }
        break;
      case PUSH_IGNORE:                    // SwitchMode 15
      case PUSH_IGNORE_INV:                // SwitchMode 16
        Switch.last_state[i] = button;                        // Update switch state before publishing
        MqttPublishSensor();
        break;
      }
      Switch.last_state[i] = button;
    }
    if (switchflag <= POWER_TOGGLE) {
      if (!Settings->flag5.mqtt_switches) {                   // SetOption114 (0) - Detach Switches from relays and enable MQTT action state for all the SwitchModes
        if (!SendKey(KEY_SWITCH, i +1, switchflag)) {         // Execute command via MQTT
          ExecuteCommandPower(i +1, switchflag, SRC_SWITCH);  // Execute command internally (if i < TasmotaGlobal.devices_present)
        }
      } else { mqtt_action = switchflag; }
    }
    if ((mqtt_action != POWER_NONE) && Settings->flag5.mqtt_switches) {  // SetOption114 (0) - Detach Switches from relays and enable MQTT action state for all the SwitchModes
      if (!Settings->flag.hass_discovery) {                   // SetOption19 - Control Home Assistant automatic discovery (See SetOption59)
        char mqtt_state_str[16];
        char *mqtt_state = mqtt_state_str;
        if (mqtt_action <= 3) {
          if (mqtt_action != 3) { SendKey(KEY_SWITCH, i +1, mqtt_action); }
          mqtt_state = SettingsText(SET_STATE_TXT1 + mqtt_action);
        } else {
          GetTextIndexed(mqtt_state_str, sizeof(mqtt_state_str), mqtt_action, kSwitchPressStates);
        }
        Response_P(S_JSON_SVALUE_ACTION_SVALUE, GetSwitchText(i).c_str(), mqtt_state);
        char scommand[10];
        snprintf_P(scommand, sizeof(scommand), PSTR(D_JSON_SWITCH "%d"), i +1);
        MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, scommand);
      }
      mqtt_action = POWER_NONE;
    }
  }
}

void SwitchLoop(void) {
  if (Switch.used) {
    if (TimeReached(Switch.debounce)) {
      SetNextTimeInterval(Switch.debounce, Settings->switch_debounce);
      SwitchHandler();
    }
  }
}

#endif  // SWITCH_V4
