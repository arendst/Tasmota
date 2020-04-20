/*
  xdrv_35_pwm_dimmer.ino - PWM Dimmer Switch support for Tasmota

  Copyright (C) 2020  Paul C Diem

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

#ifdef USE_PWM_DIMMER

/*********************************************************************************************\
* Support for Martin Jerry/acenx/Tessan/NTONPOWER SD0x PWM dimmer switches. The brightness of
* the load for these dimmers is controlled by a PWM GPIO pin. Examples are:
*
* https://www.amazon.com/dp/B07FXYSVR1
* https://www.amazon.com/dp/B07V26Q3VD
* https://www.amazon.com/dp/B07K67D43J
* https://www.amazon.com/dp/B07TTGFWFM
*
\*********************************************************************************************/

#define XDRV_35             35

const char kPWMDimmerCommands[] PROGMEM = "|"  // No prefix
  D_CMND_BRI_PRESET;

void (* const PWMDimmerCommand[])(void) PROGMEM = {
  &CmndBriPreset };

#ifdef USE_PWM_DIMMER_REMOTE
struct remote_pwm_dimmer {
  bool power_on;
  uint8_t bri_power_on;
  uint8_t bri_preset_low;
  uint8_t bri_preset_high;
  uint8_t fixed_color_index;
  uint8_t bri;
  bool power_button_increases_bri;
};
#endif  // USE_PWM_DIMMER_REMOTE

uint32_t last_button_press_time;
uint32_t button_hold_time[3];
uint8_t led_timeout_seconds = 0;
uint8_t restore_powered_off_led_counter = 0;
uint8_t power_button_index = 0;
uint8_t down_button_index = 1;
uint8_t buttons_pressed = 0;
uint8_t tap_count = 0;
bool down_button_tapped = false;
bool power_button_increases_bri = true;
bool invert_power_button_bri_direction = false;
bool restore_brightness_leds = false;
bool button_pressed[3] = { false, false, false };
bool button_hold_sent[3];
bool button_hold_processed[3];
#ifdef USE_PWM_DIMMER_REMOTE
struct remote_pwm_dimmer * remote_pwm_dimmers;
struct remote_pwm_dimmer * active_remote_pwm_dimmer;
uint8_t remote_pwm_dimmer_count;
bool active_device_is_local;
#endif  // USE_PWM_DIMMER_REMOTE

void PWMModulePreInit(void)
{
  Settings.seriallog_level = 0;
  Settings.flag.mqtt_serial = 0;  // Disable serial logging
  Settings.ledstate = 0;          // Disable LED usage

  // If the module was just changed to PWM Dimmer, set the defaults.
  if (Settings.last_module != Settings.module) {
    Settings.flag.pwm_control = true;     // SetOption15 - Switch between commands PWM or COLOR/DIMMER/CT/CHANNEL
    Settings.param[P_HOLD_TIME] = 5;      // SetOption32 - Button held for factor times longer
    Settings.bri_power_on = Settings.bri_preset_low = Settings.bri_preset_high = 0;
    Settings.last_module = Settings.module;
  }

  // Make sure the brightness level settings are sensible.
  if (!Settings.bri_power_on) Settings.bri_power_on = 128;
  if (!Settings.bri_preset_low) Settings.bri_preset_low = 10;
  if (Settings.bri_preset_high < Settings.bri_preset_low) Settings.bri_preset_high = 255;

  PWMDimmerSetPoweredOffLed();

  // The relay initializes to on. If the power is supposed to be off, turn the relay off.
  if (!power && pin[GPIO_REL1] < 99) digitalWrite(pin[GPIO_REL1], bitRead(rel_inverted, 0) ? 1 : 0);

#ifdef USE_PWM_DIMMER_REMOTE
  // If remote device mode is enabled, set the device group count to the number of buttons
  // present.
  if (Settings.flag4.remote_device_mode) {
    Settings.flag4.device_groups_enabled = true;

    device_group_count = 0;
    for (uint32_t button_index = 0; button_index < MAX_KEYS; button_index++) {
      if (pin[GPIO_KEY1 + button_index] < 99) device_group_count++;
    }

    remote_pwm_dimmer_count = device_group_count - 1;
    if (remote_pwm_dimmer_count) {
      if ((remote_pwm_dimmers = (struct remote_pwm_dimmer *) calloc(remote_pwm_dimmer_count, sizeof(struct remote_pwm_dimmer))) == nullptr) {
        AddLog_P2(LOG_LEVEL_ERROR, PSTR("PWMDimmer: error allocating PWM dimmer array"));
        Settings.flag4.remote_device_mode = false;
      }
      else {
        for (uint8_t i = 0; i < remote_pwm_dimmer_count; i++) {
          active_remote_pwm_dimmer = &remote_pwm_dimmers[i];
          active_remote_pwm_dimmer->bri_power_on = 128;
          active_remote_pwm_dimmer->bri_preset_low = 10;
          active_remote_pwm_dimmer->bri_preset_high = 255;
        }
      }
    }
  }
  active_device_is_local = true;
#endif  // USE_PWM_DIMMER_REMOTE
}

// operation: 0 = normal, -1 = all off, 1 = all on
void PWMDimmerSetBrightnessLeds(int32_t operation)
{
  if (leds_present) {
    uint32_t step = (!operation ? 256 / (leds_present + 1) : operation < 0 ? 256 : 0);
    uint32_t current_bri = (Light.power ? light_state.getBri() : 0);
    uint32_t level = step;
    SetLedPowerIdx(0, current_bri >= level);
    if (leds_present > 1) {
      level += step;
      SetLedPowerIdx(1, current_bri >= level);
      if (leds_present > 2) {
        level += step;
        SetLedPowerIdx(2, current_bri >= level);
        if (leds_present > 3) {
          level += step;
          SetLedPowerIdx(3, current_bri >= level);
        }
      }
    }

    // If enabled, set the LED timeout.
    if (!operation) led_timeout_seconds = (current_bri && Settings.flag4.led_timeout ? 5 : 0);
  }
}

void PWMDimmerSetPoweredOffLed(void)
{
  // Set the powered-off LED state.
  if (pin[GPIO_LEDLNK] < 99) {
    bool power_off_led_on = !power && Settings.flag4.powered_off_led;
    if (ledlnk_inverted) power_off_led_on ^= 1;
    digitalWrite(pin[GPIO_LEDLNK], power_off_led_on);
  }
}

void PWMDimmerSetPower(void)
{
  DigitalWrite(GPIO_REL1, bitRead(rel_inverted, 0) ? !power : power);
  PWMDimmerSetBrightnessLeds(0);
  PWMDimmerSetPoweredOffLed();
}

#ifdef USE_DEVICE_GROUPS
void PWMDimmerHandleDevGroupItem(void)
{
  uint32_t value = XdrvMailbox.payload;
#ifdef USE_PWM_DIMMER_REMOTE
  uint8_t device_group_index = *(uint8_t *)XdrvMailbox.topic;
  if (device_group_index > remote_pwm_dimmer_count) return;
  bool device_is_local = device_groups[device_group_index].local;
  struct remote_pwm_dimmer * remote_pwm_dimmer = &remote_pwm_dimmers[device_group_index - 1];
#else  // USE_PWM_DIMMER_REMOTE
  if (*(uint8_t *)XdrvMailbox.topic) return;
#endif  // !USE_PWM_DIMMER_REMOTE

  switch (XdrvMailbox.command_code) {
#ifdef USE_PWM_DIMMER_REMOTE
    case DGR_ITEM_LIGHT_BRI:
      if (!device_is_local) remote_pwm_dimmer->bri = value;
      break;
    case DGR_ITEM_POWER:
      if (!device_is_local) {
        remote_pwm_dimmer->power_on = value & 1;
        remote_pwm_dimmer->power_button_increases_bri = (remote_pwm_dimmer->bri < 128);
      }
      break;
    case DGR_ITEM_LIGHT_FIXED_COLOR:
      if (!device_is_local) remote_pwm_dimmer->fixed_color_index = value;
      break;
#endif  // USE_PWM_DIMMER_REMOTE
    case DGR_ITEM_BRI_POWER_ON:
#ifdef USE_PWM_DIMMER_REMOTE
      if (!device_is_local)
        remote_pwm_dimmer->bri_power_on = value;
      else
#endif  // USE_PWM_DIMMER_REMOTE
        Settings.bri_power_on = value;
      break;
    case DGR_ITEM_BRI_PRESET_LOW:
#ifdef USE_PWM_DIMMER_REMOTE
      if (!device_is_local)
        remote_pwm_dimmer->bri_preset_low = value;
      else
#endif  // USE_PWM_DIMMER_REMOTE
        Settings.bri_preset_low = value;
      break;
    case DGR_ITEM_BRI_PRESET_HIGH:
#ifdef USE_PWM_DIMMER_REMOTE
      if (!device_is_local)
        remote_pwm_dimmer->bri_preset_high = value;
      else
#endif  // USE_PWM_DIMMER_REMOTE
        Settings.bri_preset_high = value;
      break;
    case DGR_ITEM_STATUS:
#ifdef USE_PWM_DIMMER_REMOTE
      if (device_is_local)
#endif  // USE_PWM_DIMMER_REMOTE
        SendLocalDeviceGroupMessage(DGR_MSGTYP_UPDATE, DGR_ITEM_BRI_POWER_ON, Settings.bri_power_on,
        DGR_ITEM_BRI_PRESET_LOW, Settings.bri_preset_low, DGR_ITEM_BRI_PRESET_HIGH, Settings.bri_preset_high);
      break;
  }
}
#endif  // USE_DEVICE_GROUPS

void PWMDimmerHandleButton(void)
{
  /*
  * Power Button          Up/Down Buttons         State   Remote Mode   Action
  * --------------------  ----------------------  -----   ------------  ----------------------------
  * Press & release       Released                Any     Any           Toggle power
  * Hold for hold time    Released                On      Any           Brighter/dimmer while held
  *                                                                     then reverse direction
  * Hold for hold time    Released                Off     Any           Power on at bri preset low
  * Hold while            Press & release up      Any     Any           Toggle/change options
  * Hold while            Press & release down    Any     Any           Toggle/change options
  * Hold while            Press up                On      Yes           Brighter
  * Hold while            Press down              On      Yes           Dimmer
  * Released              Hold up                 On      No            Brigther
  * Released              Hold down               On      No            Dimmer
  * Released              Press & release up      Off     No            Power on at bri preset low
  * Released              Press & release down    Off     No            Power on at bri preset high
  * 
  * Holding any button for over 10 seconds executes the WiFiConfig 2 command.
  * 
  * In remote mode, whichever button is pressed first becomes the power button and any buttons
  * pressed while it is held affect the device associated with it. The up and down buttons change
  * depeneding on which button is the current power button:
  * 
  * Power Down  Up
  * ----- ----  --
  * 1     2     3
  * 2     1     3
  * 3     1     2
  */

  // If the button is not pressed and was not just released (the most common case), ...
  if (XdrvMailbox.payload && !button_pressed[XdrvMailbox.index]) {

    // If no buttons have been pressed for 400ms, reset the button press counts.
    if (last_button_press_time && !buttons_pressed && millis() - last_button_press_time > 400) {
      last_button_press_time = 0;
      tap_count = 0;
    }
    return;
  }

  bool state_updated = false;
  int8_t bri_offset = 0;
  uint8_t power_on_bri = 0;
  uint8_t dgr_item = 0;
  uint8_t dgr_value;
  uint8_t dgr_more_to_come = false;
  uint32_t button_index = XdrvMailbox.index;
  uint32_t now = millis();
  
  // Initialize some variables.
#ifdef USE_PWM_DIMMER_REMOTE
  bool power_is_on = (!active_device_is_local ? active_remote_pwm_dimmer->power_on : power);
  bool is_power_button = (button_index == power_button_index);
#else // USE_PWM_DIMMER_REMOTE
  bool power_is_on = power;
  bool is_power_button = !button_index;
#endif  // USE_PWM_DIMMER_REMOTE
  bool is_down_button = (button_index == down_button_index);

  // If the button is pressed, ...
  if (!XdrvMailbox.payload) {

    // If the button was just pressed, flag the button as pressed, clear the hold sent flag and
    // increment the buttons pressed count.
    if (!button_pressed[button_index]) {
      last_button_press_time = now;
      button_pressed[button_index] = true;
      button_hold_time[button_index] = now + Settings.param[P_HOLD_TIME] * 100;
      button_hold_sent[button_index] = false;
      buttons_pressed++;

#ifdef USE_PWM_DIMMER_REMOTE
      // If there are no other buttons pressed right now and remote mode is enabled, make the device
      // associated with this button the device we're going to control.
      if (buttons_pressed == 1 && Settings.flag4.remote_device_mode) {
        power_button_index = button_index;
        down_button_index = (button_index ? 0 : 1);
        active_device_is_local = device_groups[power_button_index].local;
        if (!active_device_is_local) active_remote_pwm_dimmer = &remote_pwm_dimmers[power_button_index - 1];
      }
#endif  // USE_PWM_DIMMER_REMOTE

      return;
    }

    // If the button is being held, ...
    if (button_hold_time[button_index] < now) {

      // If the power button is not also pressed and the button has been held for over 10 seconds,
      // execute the WiFiConfig 2 command.
      if (!button_pressed[power_button_index] && now - button_hold_time[button_index] > 10000) {
        button_hold_time[button_index] = now + 90000;
        char scmnd[20];
        snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_WIFICONFIG " 2"));
        ExecuteCommand(scmnd, SRC_BUTTON);
        return;
      }

      // Send a button hold if we haven't already. If it is handled (by the button topic or by a
      // rule), ignore the this button until it's released.
      if (!button_hold_sent[button_index]) {
        button_hold_sent[button_index] = true;
        button_hold_processed[button_index] = (!is_power_button && tap_count ? false : SendKey(KEY_BUTTON, button_index + 1, POWER_HOLD));
      }
      if (!button_hold_processed[button_index]) {

        // If this is about the power button, ...
        if (is_power_button) {

          // If no other buttons are pressed and the up or down button was tapped while holding the
          // power button before this, ...
          if (buttons_pressed == 1 && !tap_count) {

            // If the power is on, adjust the brightness. Set the direction based on the current
            // direction for the device and then invert the direction when the power button is
            // released. The new brightness will be calculated below.
            if (power_is_on) {
#ifdef USE_PWM_DIMMER_REMOTE
              bri_offset = (!active_device_is_local ? (active_remote_pwm_dimmer->power_button_increases_bri ? 1 : -1) : (power_button_increases_bri ? 1 : -1));
#else // USE_PWM_DIMMER_REMOTE
              bri_offset = (power_button_increases_bri ? 1 : -1);
#endif  // USE_PWM_DIMMER_REMOTE
              invert_power_button_bri_direction = true;
            }

            // If the power is not on, turn it on using an initial brightness of bri_preset_low and
            // set the power button hold time to delay before we start increasing the brightness.
            else {
#ifdef USE_PWM_DIMMER_REMOTE
              if (!active_device_is_local)
                power_on_bri = active_remote_pwm_dimmer->bri = active_remote_pwm_dimmer->bri_preset_low;
              else
#endif  // USE_PWM_DIMMER_REMOTE
                power_on_bri = Settings.bri_preset_low;
              button_hold_time[button_index] = now + 500;
            }
          }
        }

        // If this is about the down or up buttons, ...
        else {

          // If the power is on and the up or down button was not tapped while holding the power
          // button before this, adjust the brightness. Set the direction based on which button is
          // pressed. The new brightness will be calculated below.
          if (power_is_on && !tap_count) {
            bri_offset = (is_down_button ? -1 : 1);
          }

          else {
            uint8_t mqtt_trigger = 0;

            // If the up or down button was tapped while holding the power button before this,
            // handle the operation.
            if (tap_count) {

              // Send a device group update to select the previous/next fixed color.
              if (down_button_tapped) {
#ifdef USE_DEVICE_GROUPS
                uint8_t uint8_value;
#ifdef USE_PWM_DIMMER_REMOTE
                if (!active_device_is_local)
                  uint8_value = active_remote_pwm_dimmer->fixed_color_index;
                else
#endif  // USE_PWM_DIMMER_REMOTE
                  uint8_value = Light.fixed_color_index;
                if (is_down_button)
                  uint8_value--;
                else
                  uint8_value++;
#ifdef USE_PWM_DIMMER_REMOTE
                if (!active_device_is_local)
                  active_remote_pwm_dimmer->fixed_color_index = uint8_value;
                else
#endif  // USE_PWM_DIMMER_REMOTE
                  Light.fixed_color_index = uint8_value;
                dgr_item = DGR_ITEM_LIGHT_FIXED_COLOR;
                dgr_value = uint8_value;
                dgr_more_to_come = true;
#endif // USE_DEVICE_GROUPS
                ;
              }

              // Publish MQTT Event Trigger#.
              else {
                mqtt_trigger = (is_down_button ? 3 : 4);
              }
            }

            // If the power is not on, publish MQTT Event Trigger#.
            else if (!power_is_on) {
              mqtt_trigger = (is_down_button ? 1 : 2);
            }

            // If we need to publish an MQTT trigger, do it.
            if (mqtt_trigger) {
              char topic[TOPSZ];
              sprintf_P(mqtt_data, PSTR("Trigger%u"), mqtt_trigger);
#ifdef USE_PWM_DIMMER_REMOTE
              if (!active_device_is_local) {
                snprintf_P(topic, sizeof(topic), PSTR("cmnd/%s/Event"), device_groups[power_button_index].group_name);
                MqttPublish(topic);
              }
              else
#endif  // USE_PWM_DIMMER_REMOTE
                MqttPublishPrefixTopic_P(CMND, PSTR("Event"));
            }

            button_hold_time[button_index] = now + 500;
          }
        }
      }
    }
  }

  // If the button was just released, ...
  else {
    bool button_was_held = button_hold_sent[button_index];

    // If the button was not held, send a button toggle. If the button was held but not processes by
    // support_button or support_buttondoesn't process the toggle (is not handled by a rule), ...
    if (!(button_hold_sent[button_index] ? button_hold_processed[button_index] : SendKey(KEY_BUTTON, button_index + 1, POWER_TOGGLE))) {

      // If this is about the power button, ...
      if (is_power_button) {

        // If the power button was held, ...
        if (button_was_held) {

          // If the power button was held with no other buttons pressed, we changed the brightness
          // so invert the bri direction for the next time and send a final update.
          if (invert_power_button_bri_direction) {
            invert_power_button_bri_direction = false;
#ifdef USE_PWM_DIMMER_REMOTE
            if (!active_device_is_local)
              active_remote_pwm_dimmer->power_button_increases_bri ^= 1;
            else
#endif  // USE_PWM_DIMMER_REMOTE
              power_button_increases_bri ^= 1;
#ifdef USE_PWM_DIMMER_REMOTE
            dgr_item = 255;
            state_updated = true;
#endif  // USE_PWM_DIMMER_REMOTE
          }

          // If the up or down button was tapped while the power button was held, ...
          else if (tap_count) {
          
            // If the button was tapped but not held, handle the operation based on which button was
            // tapped.
            if (!button_was_held) {

#ifdef USE_PWM_DIMMER_REMOTE
              if (active_device_is_local) {
#endif // USE_PWM_DIMMER_REMOTE

                // Toggle the powered-off LED option.
                if (down_button_tapped) {
                    Settings.flag4.led_timeout ^= 1;
                    if (Light.power) PWMDimmerSetBrightnessLeds(Settings.flag4.led_timeout ? -1 : 0);
                }

                // Toggle the LED timeout.
                else {
                    Settings.flag4.powered_off_led ^= 1;
                    PWMDimmerSetPoweredOffLed();
                }
#ifdef USE_PWM_DIMMER_REMOTE
              }
#endif // USE_PWM_DIMMER_REMOTE
            }

            // If the down button was tapped and held, we changed the fixed color. Send a final
            // update.
            else if (down_button_tapped) {
              dgr_item = 255;
            }
          }
        }

        // If the power button was not held, toggle the power.
        else {
#ifdef USE_PWM_DIMMER_REMOTE
          if (!active_device_is_local)
            power_on_bri = active_remote_pwm_dimmer->bri_power_on;
          else
#endif  // USE_PWM_DIMMER_REMOTE
            power_on_bri = Settings.bri_power_on;
        }
      }

      // If this is about the up or down buttons, ...
      else {

        if (restore_brightness_leds) {
          restore_brightness_leds = false;
          PWMDimmerSetBrightnessLeds(Settings.flag4.led_timeout ? -1 : 0);
        }

        // If the button was not held and the power button is also pressed, increment the count of
        // how many times the button has been tapped.
        if (!button_was_held && button_pressed[power_button_index]) {
          down_button_tapped = is_down_button;
          tap_count++;
        }

        // If the button wasn't tapped while the power button was pressed, ...
        if (!tap_count) {

          // If the power is on, ...
          if (power_is_on) {
          
            // If the button was not held, adjust the brightness. Set the direction based on which
            // button is pressed. The new brightness will be calculated below.
            if (button_hold_time[button_index] >= now) {
              bri_offset = (is_down_button ? -10 : 10);
              dgr_item = 255;
            }

            // If the button was held and the hold was not processed by a rule, we changed the
            // brightness and sent updates with the more-to-come message type while the button was
            // held. Send a final update.
            else if (!button_hold_processed[button_index]) {
              dgr_item = 255;
              state_updated = true;
            }
          }

          // If the power is off, turn it on using a temporary brightness of bri_preset_low if the
          // down button is pressed or bri_preset_low if the up button is pressed.
          else {
#ifdef USE_PWM_DIMMER_REMOTE
            if (!active_device_is_local)
              power_on_bri = active_remote_pwm_dimmer->bri = (is_down_button ? active_remote_pwm_dimmer->bri_preset_low : active_remote_pwm_dimmer->bri_preset_high);
            else
#endif  // USE_PWM_DIMMER_REMOTE
              power_on_bri = (is_down_button ? Settings.bri_preset_low : Settings.bri_preset_high);
          }
        }
      }
    }

    // Flag the button as released.
    button_pressed[button_index] = false;
    buttons_pressed--;
  }

  // If we need to adjust the brightness, do it.
  if (bri_offset) {
    int32_t bri;
#ifdef USE_PWM_DIMMER_REMOTE
    if (!active_device_is_local)
      bri = active_remote_pwm_dimmer->bri;
    else
#endif  // USE_PWM_DIMMER_REMOTE
      bri = light_state.getBri();
    int32_t new_bri;
    bri_offset *= (Settings.light_correction ? 4 : bri / 16 + 1);
    new_bri = bri + bri_offset;
    if (bri_offset > 0) {
      if (new_bri > 255) new_bri = 255;
    }
    else {
      if (new_bri < 1) new_bri = 1;
    }
    if (new_bri != bri) {
#ifdef USE_DEVICE_GROUPS
      SendDeviceGroupMessage(power_button_index, (dgr_item ? DGR_MSGTYP_UPDATE : DGR_MSGTYP_UPDATE_MORE_TO_COME), DGR_ITEM_LIGHT_BRI, new_bri);
#endif  // USE_DEVICE_GROUPS
#ifdef USE_PWM_DIMMER_REMOTE
      if (!active_device_is_local)
        active_remote_pwm_dimmer->bri_power_on = active_remote_pwm_dimmer->bri = new_bri;
      else {
#endif  // USE_PWM_DIMMER_REMOTE
        skip_light_fade = true;
        light_state.setBri(new_bri);
        LightAnimate();
        skip_light_fade = false;
        Settings.bri_power_on = new_bri;
#ifdef USE_PWM_DIMMER_REMOTE
      }
#endif  // USE_PWM_DIMMER_REMOTE
    }
    else {
      PWMDimmerSetBrightnessLeds(0);
    }
  }

  // If we're supposed to toggle the power on, do it.
  else if (power_on_bri) {
    power_t new_power;
#ifdef USE_DEVICE_GROUPS
#ifdef USE_PWM_DIMMER_REMOTE
    if (!active_device_is_local) {
      active_remote_pwm_dimmer->power_on ^= 1;
      new_power = active_remote_pwm_dimmer->power_on;
    }
    else {
#endif  // USE_PWM_DIMMER_REMOTE
      new_power = power ^ 1;
#ifdef USE_PWM_DIMMER_REMOTE
    }
#endif  // USE_PWM_DIMMER_REMOTE
    if (new_power)
      SendDeviceGroupMessage(power_button_index, DGR_MSGTYP_UPDATE, DGR_ITEM_LIGHT_BRI, power_on_bri, DGR_ITEM_POWER, new_power);
    else
      SendDeviceGroupMessage(power_button_index, DGR_MSGTYP_UPDATE, DGR_ITEM_POWER, new_power);
#endif  // USE_DEVICE_GROUPS

#ifdef USE_PWM_DIMMER_REMOTE
    if (!active_device_is_local)
      active_remote_pwm_dimmer->power_button_increases_bri = (power_on_bri < 128);
    else {
#endif  // USE_PWM_DIMMER_REMOTE
      light_state.setBri(power_on_bri);
      ExecuteCommandPower(1, POWER_TOGGLE, SRC_RETRY);
#ifdef USE_PWM_DIMMER_REMOTE
    }
#endif  // USE_PWM_DIMMER_REMOTE
  }

  // If we're not changing the brightness or toggling the power and we made changes, send a group
  // update.
  else if (dgr_item) {
#ifdef USE_DEVICE_GROUPS
    if (dgr_item == 255) dgr_item = 0;
    SendDeviceGroupMessage(power_button_index, (dgr_more_to_come ? DGR_MSGTYP_UPDATE_MORE_TO_COME : DGR_MSGTYP_UPDATE_DIRECT), dgr_item, dgr_value);
#endif  // USE_DEVICE_GROUPS
#ifdef USE_PWM_DIMMER_REMOTE
    if (active_device_is_local) {
#endif  // USE_PWM_DIMMER_REMOTE
      light_controller.saveSettings();
      if (state_updated && Settings.flag3.hass_tele_on_power) {  // SetOption59 - Send tele/%topic%/STATE in addition to stat/%topic%/RESULT
        MqttPublishTeleState();
      }
#ifdef USE_PWM_DIMMER_REMOTE
    }
#endif  // USE_PWM_DIMMER_REMOTE
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndBriPreset(void)
{
  if (XdrvMailbox.data_len > 0) {
    bool valid = true;
    uint32_t value;
    uint8_t parm[2];
    parm[0] = Settings.bri_preset_low;
    parm[1] = Settings.bri_preset_high;
    char * ptr = XdrvMailbox.data;
    for (uint32_t i = 0; i < 2; i++) {
      while (*ptr == ' ') ptr++;
      if (*ptr == '+') {
        if (parm[i] < 255) parm[i]++;
      }
      else if (*ptr == '-') {
        if (parm[i] > 1) parm[i]--;
      }
      else {
        value = strtoul(ptr, &ptr, 0);
        if (value < 1 || value > 255) {
          valid = false;
          break;
        }
        parm[i] = value;
        if (*ptr != ',') break;
      }
      ptr++;
    }
    if (valid && !*ptr) {
      if (parm[0] < parm[1]) {
        Settings.bri_preset_low = parm[0];
        Settings.bri_preset_high = parm[1];
      } else
      {
        Settings.bri_preset_low = parm[1];
        Settings.bri_preset_high = parm[0];
      }
#ifdef USE_DEVICE_GROUPS
      SendLocalDeviceGroupMessage(DGR_MSGTYP_UPDATE, DGR_ITEM_BRI_PRESET_LOW, Settings.bri_preset_low, DGR_ITEM_BRI_PRESET_HIGH, Settings.bri_preset_high);
#endif  // USE_DEVICE_GROUPS
    }
  }
  Response_P(PSTR("{\"" D_CMND_BRI_PRESET "\":{\"Low\":%d,\"High\":%d}}"), Settings.bri_preset_low, Settings.bri_preset_high);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv35(uint8_t function)
{
  bool result = false;

  if (PWM_DIMMER != my_module_type) return result;

  switch (function) {
    case FUNC_EVERY_SECOND:
      // Turn off the brightness LED's if it's time.
      if (led_timeout_seconds && !led_timeout_seconds--) {
        PWMDimmerSetBrightnessLeds(-1);
      }

      // The powered-off LED is also the LedLink LED. If we lose the WiFi or MQTT server connection,
      // the LED will be set to a blinking state and will be turned off when the connection is
      // restored. If the state is blinking now, set a flag so we know that we need to restore it
      // when it stops blinking.
      if (global_state.data)
        restore_powered_off_led_counter = 5;
      else if (restore_powered_off_led_counter) {
        PWMDimmerSetPoweredOffLed();
        restore_powered_off_led_counter--;
      }
      break;

    case FUNC_BUTTON_PRESSED:
      PWMDimmerHandleButton();
      result = true;
      break;

#ifdef USE_DEVICE_GROUPS
    case FUNC_DEVICE_GROUP_ITEM:
      PWMDimmerHandleDevGroupItem();
      break;
#endif  // USE_DEVICE_GROUPS

    case FUNC_COMMAND:
      result = DecodeCommand(kPWMDimmerCommands, PWMDimmerCommand);
      break;

    case FUNC_SET_DEVICE_POWER:
      // If we're turning the power on, turn the relay and the brightness LEDs on and turn the
      // powered-off LED off.
      if (XdrvMailbox.index) {
        PWMDimmerSetPower();

        // Set the power button hold dimmer direction based on the current brightness.
        power_button_increases_bri = (light_state.getBri() < 128);
      }

      // If we're turning the power off, return true so SetDevicePower doesn't turn the relay off.
      // It will be turned off in LightApplyFade when the fade is done.
      else
        result = true;
      break;

    case FUNC_PRE_INIT:
      PWMModulePreInit();
      break;
  }
  return result;
}

#endif  // USE_PWM_DIMMER
