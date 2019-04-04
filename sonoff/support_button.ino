/*
  support_button.ino - button support for Sonoff-Tasmota

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

#define BUTTON_V1
#ifdef BUTTON_V1
/*********************************************************************************************\
 * Button support
\*********************************************************************************************/

unsigned long button_debounce = 0;          // Button debounce timer
uint16_t holdbutton[MAX_KEYS] = { 0 };      // Timer for button hold
uint16_t dual_button_code = 0;              // Sonoff dual received code

uint8_t lastbutton[MAX_KEYS] = { NOT_PRESSED, NOT_PRESSED, NOT_PRESSED, NOT_PRESSED };  // Last button states
uint8_t multiwindow[MAX_KEYS] = { 0 };      // Max time between button presses to record press count
uint8_t multipress[MAX_KEYS] = { 0 };       // Number of button presses within multiwindow

uint8_t dual_hex_code = 0;                  // Sonoff dual input flag
uint8_t key_no_pullup = 0;                  // key no pullup flag (1 = no pullup)
uint8_t key_inverted = 0;                   // Key inverted flag (1 = inverted)
uint8_t buttons_found = 0;                  // Number of buttons found flag

/********************************************************************************************/

void ButtonPullupFlag(uint8 button_bit)
{
  bitSet(key_no_pullup, button_bit);
}

void ButtonInvertFlag(uint8 button_bit)
{
  bitSet(key_inverted, button_bit);
}

void ButtonInit(void)
{
  buttons_found = 0;
  for (uint8_t i = 0; i < MAX_KEYS; i++) {
    if (pin[GPIO_KEY1 +i] < 99) {
      buttons_found++;
      pinMode(pin[GPIO_KEY1 +i], bitRead(key_no_pullup, i) ? INPUT : ((16 == pin[GPIO_KEY1 +i]) ? INPUT_PULLDOWN_16 : INPUT_PULLUP));
    }
  }
}

uint8_t ButtonSerial(uint8_t serial_in_byte)
{
  if (dual_hex_code) {
    dual_hex_code--;
    if (dual_hex_code) {
      dual_button_code = (dual_button_code << 8) | serial_in_byte;
      serial_in_byte = 0;
    } else {
      if (serial_in_byte != 0xA1) {
        dual_button_code = 0;                // 0xA1 - End of Sonoff dual button code
      }
    }
  }
  if (0xA0 == serial_in_byte) {              // 0xA0 - Start of Sonoff dual button code
    serial_in_byte = 0;
    dual_button_code = 0;
    dual_hex_code = 3;
  }

  return serial_in_byte;
}

/*********************************************************************************************\
 * Button handler with single press only or multi-press and hold on all buttons
\*********************************************************************************************/

void ButtonHandler(void)
{
  if (uptime < 4) { return; }                                  // Block GPIO for 4 seconds after poweron to workaround Wemos D1 / Obi RTS circuit

  uint8_t button = NOT_PRESSED;
  uint8_t button_present = 0;
  uint8_t hold_time_extent = IMMINENT_RESET_FACTOR;            // Extent hold time factor in case of iminnent Reset command
  uint16_t loops_per_second = 1000 / Settings.button_debounce;
  char scmnd[20];

//  uint8_t maxdev = (devices_present > MAX_KEYS) ? MAX_KEYS : devices_present;
//  for (uint8_t button_index = 0; button_index < maxdev; button_index++) {
  for (uint8_t button_index = 0; button_index < MAX_KEYS; button_index++) {
    button = NOT_PRESSED;
    button_present = 0;

    if (!button_index && ((SONOFF_DUAL == my_module_type) || (CH4 == my_module_type))) {
      button_present = 1;
      if (dual_button_code) {
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_BUTTON " " D_CODE " %04X"), dual_button_code);
        button = PRESSED;
        if (0xF500 == dual_button_code) {                      // Button hold
          holdbutton[button_index] = (loops_per_second * Settings.param[P_HOLD_TIME] / 10) -1;
          hold_time_extent = 1;
        }
        dual_button_code = 0;
      }
    } else {
      if (pin[GPIO_KEY1 +button_index] < 99) {
        button_present = 1;
        button = (digitalRead(pin[GPIO_KEY1 +button_index]) != bitRead(key_inverted, button_index));
      }
    }

    if (button_present) {
      XdrvMailbox.index = button_index;
      XdrvMailbox.payload = button;
      if (XdrvCall(FUNC_BUTTON_PRESSED)) {
        // Serviced
      }
      else if (SONOFF_4CHPRO == my_module_type) {
        if (holdbutton[button_index]) { holdbutton[button_index]--; }

        bool button_pressed = false;
        if ((PRESSED == button) && (NOT_PRESSED == lastbutton[button_index])) {
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_BUTTON "%d " D_LEVEL_10), button_index +1);
          holdbutton[button_index] = loops_per_second;
          button_pressed = true;
        }
        if ((NOT_PRESSED == button) && (PRESSED == lastbutton[button_index])) {
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_BUTTON "%d " D_LEVEL_01), button_index +1);
          if (!holdbutton[button_index]) { button_pressed = true; }  // Do not allow within 1 second
        }
        if (button_pressed) {
          if (!SendKey(0, button_index +1, POWER_TOGGLE)) {    // Execute Toggle command via MQTT if ButtonTopic is set
            ExecuteCommandPower(button_index +1, POWER_TOGGLE, SRC_BUTTON);  // Execute Toggle command internally
          }
        }
      }
      else {
        if ((PRESSED == button) && (NOT_PRESSED == lastbutton[button_index])) {
          if (Settings.flag.button_single) {                   // Allow only single button press for immediate action
            AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_BUTTON "%d " D_IMMEDIATE), button_index +1);
            if (!SendKey(0, button_index +1, POWER_TOGGLE)) {  // Execute Toggle command via MQTT if ButtonTopic is set
              ExecuteCommandPower(button_index +1, POWER_TOGGLE, SRC_BUTTON);  // Execute Toggle command internally
            }
          } else {
            multipress[button_index] = (multiwindow[button_index]) ? multipress[button_index] +1 : 1;
            AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_BUTTON "%d " D_MULTI_PRESS " %d"), button_index +1, multipress[button_index]);
            multiwindow[button_index] = loops_per_second / 2;  // 0.5 second multi press window
          }
          blinks = 201;
        }

        if (NOT_PRESSED == button) {
          holdbutton[button_index] = 0;
        } else {
          holdbutton[button_index]++;
          if (Settings.flag.button_single) {                   // Allow only single button press for immediate action
            if (holdbutton[button_index] == loops_per_second * hold_time_extent * Settings.param[P_HOLD_TIME] / 10) {  // Button held for factor times longer
//              Settings.flag.button_single = 0;
              snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_SETOPTION "13 0"));  // Disable single press only
              ExecuteCommand(scmnd, SRC_BUTTON);
            }
          } else {
            if (Settings.flag.button_restrict) {               // Button restriction
              if (holdbutton[button_index] == loops_per_second * Settings.param[P_HOLD_TIME] / 10) {  // Button hold
                multipress[button_index] = 0;
                SendKey(0, button_index +1, 3);                // Execute Hold command via MQTT if ButtonTopic is set
              }
            } else {
              if (holdbutton[button_index] == loops_per_second * hold_time_extent * Settings.param[P_HOLD_TIME] / 10) {  // Button held for factor times longer
                multipress[button_index] = 0;
                snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_RESET " 1"));
                ExecuteCommand(scmnd, SRC_BUTTON);
              }
            }
          }
        }

        if (!Settings.flag.button_single) {                    // Allow multi-press
          if (multiwindow[button_index]) {
            multiwindow[button_index]--;
          } else {
            if (!restart_flag && !holdbutton[button_index] && (multipress[button_index] > 0) && (multipress[button_index] < MAX_BUTTON_COMMANDS +3)) {
              bool single_press = false;
              if (multipress[button_index] < 3) {              // Single or Double press
                if ((SONOFF_DUAL_R2 == my_module_type) || (SONOFF_DUAL == my_module_type) || (CH4 == my_module_type)) {
                  single_press = true;
                } else  {
                  single_press = (Settings.flag.button_swap +1 == multipress[button_index]);
                  multipress[button_index] = 1;
                }
              }
              if ((MI_DESK_LAMP == my_module_type) && (button_index == 0) && (rotary_changed) && (light_power)) {
                rotary_changed = 0;                            // Color temp changed, no need to turn of the light
              } else {
                if (single_press && SendKey(0, button_index + multipress[button_index], POWER_TOGGLE)) {  // Execute Toggle command via MQTT if ButtonTopic is set
                  // Success
                } else {
                  if (multipress[button_index] < 3) {          // Single or Double press
                    if (WifiState() > WIFI_RESTART) {          // WPSconfig, Smartconfig or Wifimanager active
                      restart_flag = 1;
                    } else {
                      ExecuteCommandPower(button_index + multipress[button_index], POWER_TOGGLE, SRC_BUTTON);  // Execute Toggle command internally
                    }
                  } else {                                     // 3 - 7 press
                    if (!Settings.flag.button_restrict) {
                      snprintf_P(scmnd, sizeof(scmnd), kCommands[multipress[button_index] -3]);
                      ExecuteCommand(scmnd, SRC_BUTTON);
                    }
                  }
                }
              }
              multipress[button_index] = 0;
            }
          }
        }
      }
    }
    lastbutton[button_index] = button;
  }
}

void ButtonLoop(void)
{
  if (buttons_found) {
    if (TimeReached(button_debounce)) {
      SetNextTimeInterval(button_debounce, Settings.button_debounce);
      ButtonHandler();
    }
  }
}

#endif  // BUTTON_V1
