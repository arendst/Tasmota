/*
  support_button.ino - button support for Tasmota

  Copyright (C) 2021  Federico Leoni and Theo Arends

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

#define BUTTON_V2
#ifdef BUTTON_V2
/*********************************************************************************************\
 * Button support
\*********************************************************************************************/

#define MAX_RELAY_BUTTON1       5  // Max number of relay controlled by BUTTON1
#ifdef ESP32
#define TOUCH_PIN_THRESHOLD     12 // Smaller value will treated as button press
#define TOUCH_HIT_THRESHOLD     3  // successful hits to filter out noise
#endif  // ESP32

const char kMultiPress[] PROGMEM =
  "|SINGLE|DOUBLE|TRIPLE|QUAD|PENTA|";

struct BUTTON {
  uint32_t debounce = 0;                     // Button debounce timer
  uint32_t no_pullup_mask = 0;               // key no pullup flag (1 = no pullup)
  uint32_t inverted_mask = 0;                // Key inverted flag (1 = inverted)
#ifdef ESP32
  uint32_t touch_mask = 0;                   // Touch flag (1 = inverted)
#endif  // ESP32
  uint16_t hold_timer[MAX_KEYS] = { 0 };     // Timer for button hold
  uint16_t dual_code = 0;                    // Sonoff dual received code

  uint8_t last_state[MAX_KEYS];              // Last button states
  uint8_t window_timer[MAX_KEYS] = { 0 };    // Max time between button presses to record press count
  uint8_t press_counter[MAX_KEYS] = { 0 };   // Number of button presses within Button.window_timer

  uint8_t dual_receive_count = 0;            // Sonoff dual input flag
#ifdef ESP32
  uint8_t touch_hits[MAX_KEYS] = { 0 };      // Hits in a row to filter out noise
#endif  // ESP32
  uint8_t present = 0;                       // Number of buttons found flag
} Button;

#ifdef ESP32
struct TOUCH_BUTTON {
  uint8_t pin_threshold = TOUCH_PIN_THRESHOLD;
  uint8_t hit_threshold = TOUCH_HIT_THRESHOLD;
  uint32_t calibration = 0; // Bitfield
} TOUCH_BUTTON;
#endif  // ESP32

/********************************************************************************************/

void ButtonPullupFlag(uint32_t button_bit) {
  bitSet(Button.no_pullup_mask, button_bit);
}

void ButtonInvertFlag(uint32_t button_bit) {
  bitSet(Button.inverted_mask, button_bit);
}

#ifdef ESP32
void ButtonTouchFlag(uint32_t button_bit) {
  bitSet(Button.touch_mask, button_bit);
}
#endif  // ESP32

void ButtonInit(void) {
  Button.present = 0;
#ifdef ESP8266
  if ((SONOFF_DUAL == TasmotaGlobal.module_type) || (CH4 == TasmotaGlobal.module_type)) {
    Button.present++;
  }
#endif  // ESP8266
  for (uint32_t i = 0; i < MAX_KEYS; i++) {
    Button.last_state[i] = NOT_PRESSED;
    if (PinUsed(GPIO_KEY1, i)) {
      Button.present++;
#ifdef ESP8266
      pinMode(Pin(GPIO_KEY1, i), bitRead(Button.no_pullup_mask, i) ? INPUT : ((16 == Pin(GPIO_KEY1, i)) ? INPUT_PULLDOWN_16 : INPUT_PULLUP));
#endif  // ESP8266
#ifdef ESP32
      pinMode(Pin(GPIO_KEY1, i), bitRead(Button.no_pullup_mask, i) ? INPUT : INPUT_PULLUP);
#endif  // ESP32
    }
#ifdef USE_ADC
    else if (PinUsed(GPIO_ADC_BUTTON, i) || PinUsed(GPIO_ADC_BUTTON_INV, i)) {
      Button.present++;
    }
#endif  // USE_ADC
  }
}

uint8_t ButtonSerial(uint8_t serial_in_byte) {
  if (Button.dual_receive_count) {
    Button.dual_receive_count--;
    if (Button.dual_receive_count) {
      Button.dual_code = (Button.dual_code << 8) | serial_in_byte;
      serial_in_byte = 0;
    } else {
      if (serial_in_byte != 0xA1) {
        Button.dual_code = 0;                // 0xA1 - End of Sonoff dual button code
      }
    }
  }
  if (0xA0 == serial_in_byte) {              // 0xA0 - Start of Sonoff dual button code
    serial_in_byte = 0;
    Button.dual_code = 0;
    Button.dual_receive_count = 3;
  }

  return serial_in_byte;
}

/*********************************************************************************************\
 * Button handler with single press only or multi-press and hold on all buttons
 *
 * ButtonDebounce (50) - Debounce time in mSec
 * SetOption1  (0)     - If set do not execute commands WifiConfig and Reset
 * SetOption11 (0)     - If set perform single press action on double press and reverse (on two relay devices only)
 * SetOption13 (0)     - If set act on single press only
 * SetOption73 (0)     - Decouple button from relay and send just mqtt topic
\*********************************************************************************************/

void ButtonHandler(void) {
  if (TasmotaGlobal.uptime < 4) { return; }                     // Block GPIO for 4 seconds after poweron to workaround Wemos D1 / Obi RTS circuit

  uint8_t hold_time_extent = IMMINENT_RESET_FACTOR;             // Extent hold time factor in case of iminnent Reset command
  uint16_t loops_per_second = 1000 / Settings.button_debounce;  // ButtonDebounce (50)
  char scmnd[20];

  for (uint32_t button_index = 0; button_index < MAX_KEYS; button_index++) {
    uint8_t button = NOT_PRESSED;
    uint8_t button_present = 0;

#ifdef ESP8266
    if (!button_index && ((SONOFF_DUAL == TasmotaGlobal.module_type) || (CH4 == TasmotaGlobal.module_type))) {
      button_present = 1;
      if (Button.dual_code) {
        AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_BUTTON " " D_CODE " %04X"), Button.dual_code);
        button = PRESSED;
        if (0xF500 == Button.dual_code) {                      // Button hold
          Button.hold_timer[button_index] = (loops_per_second * Settings.param[P_HOLD_TIME] / 10) -1;  // SetOption32 (40)
          hold_time_extent = 1;
        }
        Button.dual_code = 0;
      }
    } else
#endif  // ESP8266
    if (PinUsed(GPIO_KEY1, button_index)) {
      button_present = 1;
#ifdef ESP32
      if (bitRead(Button.touch_mask, button_index)) {          // Touch
        uint32_t _value = touchRead(Pin(GPIO_KEY1, button_index));
        button = NOT_PRESSED;
        if (_value != 0) {                                     // Probably read-error
          if (_value < TOUCH_BUTTON.pin_threshold) {
            if (++Button.touch_hits[button_index] > TOUCH_BUTTON.hit_threshold) {
              if (!bitRead(TOUCH_BUTTON.calibration, button_index+1)) {
                button = PRESSED;
              }
            }
          } else {
            Button.touch_hits[button_index] = 0;
          }
        } else {
          Button.touch_hits[button_index] = 0;
        }
        if (bitRead(TOUCH_BUTTON.calibration, button_index+1)) {
          AddLog(LOG_LEVEL_INFO, PSTR("PLOT: %u, %u, %u,"), button_index+1, _value, Button.touch_hits[button_index]);  // Button number (1..4), value, continuous hits under threshold
        }
      } else
#endif  // ESP32
      {                                                 // Normal button
        button = (digitalRead(Pin(GPIO_KEY1, button_index)) != bitRead(Button.inverted_mask, button_index));
      }
    }
#ifdef USE_ADC
    else if (PinUsed(GPIO_ADC_BUTTON, button_index)) {
      button_present = 1;
      button = AdcGetButton(Pin(GPIO_ADC_BUTTON, button_index));
    }
    else if (PinUsed(GPIO_ADC_BUTTON_INV, button_index)) {
      button_present = 1;
      button = AdcGetButton(Pin(GPIO_ADC_BUTTON_INV, button_index));
    }
#endif  // USE_ADC
    if (button_present) {
      XdrvMailbox.index = button_index;
      XdrvMailbox.payload = button;
      if (XdrvCall(FUNC_BUTTON_PRESSED)) {
        // Serviced
      }
#ifdef ESP8266
      else if (SONOFF_4CHPRO == TasmotaGlobal.module_type) {
        if (Button.hold_timer[button_index]) { Button.hold_timer[button_index]--; }

        bool button_pressed = false;
        if ((PRESSED == button) && (NOT_PRESSED == Button.last_state[button_index])) {
          AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_BUTTON "%d " D_LEVEL_10), button_index +1);
          Button.hold_timer[button_index] = loops_per_second;
          button_pressed = true;
        }
        if ((NOT_PRESSED == button) && (PRESSED == Button.last_state[button_index])) {
          AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_BUTTON "%d " D_LEVEL_01), button_index +1);
          if (!Button.hold_timer[button_index]) { button_pressed = true; }  // Do not allow within 1 second
        }
        if (button_pressed) {
          if (!Settings.flag3.mqtt_buttons) {          // SetOption73 (0) - Decouple button from relay and send just mqtt topic
            if (!SendKey(KEY_BUTTON, button_index +1, POWER_TOGGLE)) {  // Execute Toggle command via MQTT if ButtonTopic is set
              ExecuteCommandPower(button_index +1, POWER_TOGGLE, SRC_BUTTON);  // Execute Toggle command internally
            }
          } else {
            MqttButtonTopic(button_index +1, 1, 0);    // SetOption73 (0) - Decouple button from relay and send just mqtt topic
          }
        }
      }
#endif  // ESP8266
      else {
        if ((PRESSED == button) && (NOT_PRESSED == Button.last_state[button_index])) {

          if (Settings.flag.button_single) {           // SetOption13 (0) - Allow only single button press for immediate action,
            if (!Settings.flag3.mqtt_buttons) {        // SetOption73 (0) - Decouple button from relay and send just mqtt topic
              AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_BUTTON "%d " D_IMMEDIATE), button_index +1);
              if (!SendKey(KEY_BUTTON, button_index +1, POWER_TOGGLE)) {  // Execute Toggle command via MQTT if ButtonTopic is set
                ExecuteCommandPower(button_index +1, POWER_TOGGLE, SRC_BUTTON);  // Execute Toggle command internally
              }
            } else {
              MqttButtonTopic(button_index +1, 1, 0);  // SetOption73 1 - Decouple button from relay and send just mqtt topic
            }
          } else {
            Button.press_counter[button_index] = (Button.window_timer[button_index]) ? Button.press_counter[button_index] +1 : 1;
            AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_BUTTON "%d " D_MULTI_PRESS " %d"), button_index +1, Button.press_counter[button_index]);
            Button.window_timer[button_index] = loops_per_second / 2;  // 0.5 second multi press window
          }
          TasmotaGlobal.blinks = 201;
        }

        if (NOT_PRESSED == button) {
          Button.hold_timer[button_index] = 0;
        } else {
          Button.hold_timer[button_index]++;
          if (Settings.flag.button_single) {           // SetOption13 (0) - Allow only single button press for immediate action
            if (Button.hold_timer[button_index] == loops_per_second * hold_time_extent * Settings.param[P_HOLD_TIME] / 10) {  // SetOption32 (40) - Button held for factor times longer
              snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_SETOPTION "13 0"));  // Disable single press only
              ExecuteCommand(scmnd, SRC_BUTTON);
            }
          } else {
            if (Button.hold_timer[button_index] == loops_per_second * Settings.param[P_HOLD_TIME] / 10) {  // SetOption32 (40) - Button hold
              Button.press_counter[button_index] = 0;
              if (Settings.flag3.mqtt_buttons) {       // SetOption73 (0) - Decouple button from relay and send just mqtt topic
                MqttButtonTopic(button_index +1, 3, 1);
              } else {
                SendKey(KEY_BUTTON, button_index +1, POWER_HOLD);  // Execute Hold command via MQTT if ButtonTopic is set
              }
            } else {
              if (Settings.flag.button_restrict) {     // SetOption1 (0) - Control button multipress
                if (Settings.param[P_HOLD_IGNORE] > 0) {     // SetOption40 (0) - Do not ignore button hold
                  if (Button.hold_timer[button_index] > loops_per_second * Settings.param[P_HOLD_IGNORE] / 10) {
                    Button.hold_timer[button_index] = 0;     // Reset button hold counter to stay below hold trigger
                    Button.press_counter[button_index] = 0;  // Discard button press to disable functionality
                  }
                }
              } else {
                if ((Button.hold_timer[button_index] == loops_per_second * hold_time_extent * Settings.param[P_HOLD_TIME] / 10)) {  // SetOption32 (40) - Button held for factor times longer
                  Button.press_counter[button_index] = 0;
                  snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_RESET " 1"));
                  ExecuteCommand(scmnd, SRC_BUTTON);
                }
              }
            }
          }
        }

        if (!Settings.flag.button_single) {            // SetOption13 (0) - Allow multi-press
          if (Button.window_timer[button_index]) {
            Button.window_timer[button_index]--;
          } else {
            if (!TasmotaGlobal.restart_flag && !Button.hold_timer[button_index] && (Button.press_counter[button_index] > 0) && (Button.press_counter[button_index] < 7)) {

              bool single_press = false;
              if (Button.press_counter[button_index] < 3) {  // Single or Double press
#ifdef ESP8266
                if ((SONOFF_DUAL_R2 == TasmotaGlobal.module_type) || (SONOFF_DUAL == TasmotaGlobal.module_type) || (CH4 == TasmotaGlobal.module_type)) {
                  single_press = true;
                } else
#endif  // ESP8266
                {
                  single_press = (Settings.flag.button_swap +1 == Button.press_counter[button_index]);  // SetOption11 (0)
                  if ((1 == Button.present) && (2 == TasmotaGlobal.devices_present)) {  // Single Button with two devices only
                    if (Settings.flag.button_swap) {           // SetOption11 (0)
                      Button.press_counter[button_index] = (single_press) ? 1 : 2;
                    }
                  }
                }
              }
#ifdef ROTARY_V1
              if (!RotaryButtonPressed(button_index)) {
#endif
                if (!Settings.flag3.mqtt_buttons && single_press && SendKey(KEY_BUTTON, button_index + Button.press_counter[button_index], POWER_TOGGLE)) {  // Execute Toggle command via MQTT if ButtonTopic is set
                  // Success
                } else {
                  if (Button.press_counter[button_index] < 6) { // Single to Penta press
                    if (WifiState() > WIFI_RESTART) {           // Wifimanager active
                      TasmotaGlobal.restart_flag = 1;
                    }
                    if (!Settings.flag3.mqtt_buttons) {         // SetOption73 - Detach buttons from relays and enable MQTT action state for multipress
                      if (Button.press_counter[button_index] == 1) {  // By default first press always send a TOGGLE (2)
                        ExecuteCommandPower(button_index + Button.press_counter[button_index], POWER_TOGGLE, SRC_BUTTON);
                      } else {
                        SendKey(KEY_BUTTON, button_index +1, Button.press_counter[button_index] +9);    // 2,3,4 and 5 press send just the key value (11,12,13 and 14) for rules
                        if (0 == button_index) {               // BUTTON1 can toggle up to 5 relays if present. If a relay is not present will send out the key value (2,11,12,13 and 14) for rules
                          bool valid_relay = PinUsed(GPIO_REL1, Button.press_counter[button_index]-1);
#ifdef ESP8266
                          if ((SONOFF_DUAL == TasmotaGlobal.module_type) || (CH4 == TasmotaGlobal.module_type)) {
                            valid_relay = (Button.press_counter[button_index] <= TasmotaGlobal.devices_present);
                          }
#endif  // ESP8266
                          if ((Button.press_counter[button_index] > 1) && valid_relay && (Button.press_counter[button_index] <= MAX_RELAY_BUTTON1)) {
                            ExecuteCommandPower(button_index + Button.press_counter[button_index], POWER_TOGGLE, SRC_BUTTON);   // Execute Toggle command internally
//                            AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: Relay%d found on GPIO%d"), Button.press_counter[button_index], Pin(GPIO_REL1, Button.press_counter[button_index]-1));
                          }
                        }
                      }
                    }

                  } else {    // 6 press start wificonfig 2
                    if (!Settings.flag.button_restrict) {  // SetOption1  - Control button multipress
                      snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_WIFICONFIG " 2"));
                      ExecuteCommand(scmnd, SRC_BUTTON);
                    }
                  }
                  if (Settings.flag3.mqtt_buttons) {   // SetOption73 (0) - Decouple button from relay and send just mqtt topic
                    if (Button.press_counter[button_index] >= 1 && Button.press_counter[button_index] <= 5) {
                      MqttButtonTopic(button_index +1, Button.press_counter[button_index], 0);
                    }
                  }
                }
#ifdef ROTARY_V1
              }
#endif
              Button.press_counter[button_index] = 0;
            }
          }
        }

      }
    }
    Button.last_state[button_index] = button;
  }
}

/*
void MqttButtonTopic(uint8_t button_id, uint8_t action, uint8_t hold) {
  char scommand[CMDSZ];
  char stopic[TOPSZ];
  char mqttstate[7];

  SendKey(KEY_BUTTON, button_id, (hold) ? 3 : action +9);

  if (!Settings.flag.hass_discovery) {
    GetTextIndexed(mqttstate, sizeof(mqttstate), action, kMultiPress);
    snprintf_P(scommand, sizeof(scommand), PSTR("BUTTON%d"), button_id);
    GetTopic_P(stopic, STAT, TasmotaGlobal.mqtt_topic, scommand);
    Response_P(S_JSON_COMMAND_SVALUE, "ACTION", (hold) ? SettingsText(SET_STATE_TXT4) : mqttstate);
    MqttPublish(stopic);
  }
}
*/

void MqttButtonTopic(uint32_t button_id, uint32_t action, uint32_t hold) {
  SendKey(KEY_BUTTON, button_id, (hold) ? 3 : action +9);

  if (!Settings.flag.hass_discovery) {                    // SetOption19 - Control Home Assistant automatic discovery (See SetOption59)
    char scommand[10];
    snprintf_P(scommand, sizeof(scommand), PSTR(D_JSON_BUTTON "%d"), button_id);
    char mqttstate[7];
    Response_P(S_JSON_SVALUE_ACTION_SVALUE, scommand, (hold) ? SettingsText(SET_STATE_TXT4) : GetTextIndexed(mqttstate, sizeof(mqttstate), action, kMultiPress));
    MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, scommand);
  }
}

void ButtonLoop(void) {
  if (Button.present) {
    if (TimeReached(Button.debounce)) {
      SetNextTimeInterval(Button.debounce, Settings.button_debounce);  // ButtonDebounce (50)
      ButtonHandler();
    }
  }
}

#endif  // BUTTON_V2