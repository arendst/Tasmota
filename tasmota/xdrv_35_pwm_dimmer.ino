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
  D_CMND_BRI_PRESET
#ifdef USE_DEVICE_GROUPS
  "|" D_CMND_PWM_DIMMER_PWMS
#endif  // USE_DEVICE_GROUPS
  ;

void (* const PWMDimmerCommand[])(void) PROGMEM = {
  &CmndBriPreset,
#ifdef USE_DEVICE_GROUPS
  &CmndPWMDimmerPWMs,
#endif  // USE_DEVICE_GROUPS
  };

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

uint32_t button_hold_time[3];
uint8_t led_timeout_seconds = 0;
uint8_t restore_powered_off_led_counter = 0;
uint8_t power_button_index = 0;
uint8_t buttons_pressed = 0;
uint8_t local_fixed_color_index = 128;
bool ignore_power_button = false;
bool button_tapped = false;
bool down_button_tapped = false;
bool tap_handled = false;
bool power_button_increases_bri = true;
bool invert_power_button_bri_direction = false;
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
  if (!power && PinUsed(GPIO_REL1)) digitalWrite(Pin(GPIO_REL1), bitRead(rel_inverted, 0) ? 1 : 0);

#ifdef USE_PWM_DIMMER_REMOTE
  // If remote device mode is enabled, set the device group count to the number of buttons
  // present.
  if (Settings.flag4.multiple_device_groups) {
    Settings.flag4.device_groups_enabled = true;

    device_group_count = 0;
    for (uint32_t button_index = 0; button_index < MAX_KEYS; button_index++) {
      if (PinUsed(GPIO_KEY1, button_index)) device_group_count++;
    }

    remote_pwm_dimmer_count = device_group_count - 1;
    if (remote_pwm_dimmer_count) {
      if ((remote_pwm_dimmers = (struct remote_pwm_dimmer *) calloc(remote_pwm_dimmer_count, sizeof(struct remote_pwm_dimmer))) == nullptr) {
        AddLog_P2(LOG_LEVEL_ERROR, PSTR("PWMDimmer: error allocating PWM dimmer array"));
        Settings.flag4.multiple_device_groups = false;
      }
      else {
        for (uint8_t i = 0; i < remote_pwm_dimmer_count; i++) {
          active_remote_pwm_dimmer = &remote_pwm_dimmers[i];
          active_remote_pwm_dimmer->bri_power_on = 128;
          active_remote_pwm_dimmer->bri_preset_low = 10;
          active_remote_pwm_dimmer->bri_preset_high = 255;
          active_remote_pwm_dimmer->fixed_color_index = 128;
        }
      }
    }
  }
  active_device_is_local = true;
#endif  // USE_PWM_DIMMER_REMOTE
}

// bri: -1 = set to current light bri, -2 = timeout, 0-255 = set to bri
void PWMDimmerSetBrightnessLeds(int32_t bri)
{
  if (leds_present) {
    led_timeout_seconds = 5;
    if (bri < 0) {
      bri = ((bri == -2 && Settings.flag4.led_timeout) || !Light.power ? 0 : light_state.getBri());
      if (!bri || !Settings.flag4.led_timeout) led_timeout_seconds = 0;
    }
    uint32_t step = 256 / (leds_present + 1);
    uint32_t level = step;
    SetLedPowerIdx(0, bri >= level);
    if (leds_present > 1) {
      level += step;
      SetLedPowerIdx(1, bri >= level);
      if (leds_present > 2) {
        level += step;
        SetLedPowerIdx(2, bri >= level);
        if (leds_present > 3) {
          level += step;
          SetLedPowerIdx(3, bri >= level);
        }
      }
    }
  }
}

void PWMDimmerSetPoweredOffLed(void)
{
  // Set the powered-off LED state.
  if (PinUsed(GPIO_LEDLNK)) {
    bool power_off_led_on = !power && Settings.flag4.powered_off_led;
    if (ledlnk_inverted) power_off_led_on ^= 1;
    digitalWrite(Pin(GPIO_LEDLNK), power_off_led_on);
  }
}

void PWMDimmerSetPower(void)
{
  DigitalWrite(GPIO_REL1, 0, bitRead(rel_inverted, 0) ? !power : power);
  PWMDimmerSetBrightnessLeds(-1);
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
#endif  // USE_PWM_DIMMER_REMOTE
    case DGR_ITEM_LIGHT_FIXED_COLOR:
#ifdef USE_PWM_DIMMER_REMOTE
      if (!device_is_local)
        remote_pwm_dimmer->fixed_color_index = value;
      else
#endif  // USE_PWM_DIMMER_REMOTE
        local_fixed_color_index = value;
      break;
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

  // If the button is not pressed and was not just released (the most common case), just return.
  if (XdrvMailbox.payload && !button_pressed[XdrvMailbox.index]) return;

  bool handle_tap = false;
  bool state_updated = false;
  int32_t bri_offset = 0;
  uint8_t power_on_bri = 0;
  uint8_t dgr_item = 0;
  uint8_t dgr_value = 0;
  uint8_t dgr_more_to_come = true;
  uint8_t mqtt_trigger = 0;
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
  bool is_down_button = (button_index == (power_button_index ? 0 : 1));

  // If the button is pressed, ...
  if (!XdrvMailbox.payload) {

    // If the button was just pressed, flag the button as pressed, clear the hold sent flag and
    // increment the buttons pressed count.
    if (!button_pressed[button_index]) {
      button_pressed[button_index] = true;
      button_hold_time[button_index] = now + Settings.param[P_HOLD_TIME] * 100;
      buttons_pressed++;

#ifdef USE_PWM_DIMMER_REMOTE
      // If there are no other buttons pressed right now and remote mode is enabled, make the device
      // associated with this button the device we're going to control.
      if (buttons_pressed == 1 && Settings.flag4.multiple_device_groups) {
        power_button_index = button_index;
        active_device_is_local = device_groups[power_button_index].local;
        if (!active_device_is_local) active_remote_pwm_dimmer = &remote_pwm_dimmers[power_button_index - 1];
      }
#endif  // USE_PWM_DIMMER_REMOTE

      return;
    }

    // If the button is being held, ...
    if (button_hold_time[button_index] < now) {

      // If we're not in the middle of a power button plus up/down button sequence and the button
      // has been held for over 10 seconds, execute the WiFiConfig 2 command.
      if (!ignore_power_button && now - button_hold_time[button_index] > 10000) {
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
        button_hold_processed[button_index] = (!is_power_button && button_tapped ? false : SendKey(KEY_BUTTON, button_index + 1, POWER_HOLD));
      }
      if (!button_hold_processed[button_index]) {

        // If this is about the power button, ...
        if (is_power_button) {

          // If we're not ignoring the power button and no other buttons are pressed, ...
          if (!ignore_power_button && buttons_pressed == 1) {

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

          // If the power button is also pressed, ...
          if (button_pressed[power_button_index]) {

            // If the up or down button was tapped while holding the power button before this,
            // handle the operation below.
            if (button_tapped) {
              handle_tap = true;
              button_hold_time[button_index] = now + 500;
            }

            // Otherwise, publish MQTT Event Trigger#.
            else {
              mqtt_trigger = (is_down_button ? 1 : 2);
              button_hold_time[button_index] = now + 60000;
              ignore_power_button = true;
            }
          }

          // Otherwise, if the power is on, adjust the brightness. Set the direction based on which
          // button is pressed. The new brightness will be calculated below.
          else if (power_is_on && !button_tapped) {
            bri_offset = (is_down_button ? -1 : 1);
          }
        }
      }
    }
  }

  // If the button was just released, ...
  else {
    bool button_was_held = button_hold_sent[button_index];
    button_hold_sent[button_index] = false;

    // If the button was not held, send a button toggle. If the button was held but not processes by
    // support_button or support_buttondoesn't process the toggle (is not handled by a rule), ...
    if (!(button_was_held ? button_hold_processed[button_index] : SendKey(KEY_BUTTON, button_index + 1, POWER_TOGGLE))) {

      // If this is about the power button, ...
      if (is_power_button) {

        // If the up or down button was tapped while the power button was held and the up or
        // down buttons weren't tapped or held afterwards, handle the operation based on which
        // button was tapped.
        if (button_tapped) {
          if (!tap_handled) {
#ifdef USE_PWM_DIMMER_REMOTE
            if (active_device_is_local) {
#endif // USE_PWM_DIMMER_REMOTE

              // Toggle the powered-off LED option.
              if (down_button_tapped) {
                  Settings.flag4.led_timeout ^= 1;
                  if (Light.power) PWMDimmerSetBrightnessLeds(Settings.flag4.led_timeout ? 0 : -1);
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
          tap_handled = false;
        }

        // Otherwise, if the power button was held, ...
        else if (button_was_held) {

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
            dgr_item = DGR_ITEM_FLAGS;
            state_updated = true;
#endif  // USE_PWM_DIMMER_REMOTE
          }
        }

        // If the power button was not held and we're not ignoring the next power button release,
        // toggle the power.
        else if (!ignore_power_button) {
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

        // If the power button is also pressed, set the flag to ignore the power button until it is
        // released.
        if (button_pressed[power_button_index]) {
          ignore_power_button = true;

          // If the button was tapped, handle it below.
          if (button_tapped) {
            handle_tap = true;
          }

          // Otherwise, if the button was not held, flag the tap.
          else if (!button_was_held) {
            button_tapped = true;
            down_button_tapped = is_down_button;
          }
        }

        // If the power button is not also pressed, ...
        else {

          // If the power is on, ...
          if (power_is_on) {

            // If the button was not held, adjust the brightness. Set the direction based on which
            // button is pressed. The new brightness will be calculated below.
            if (!button_was_held) {
              bri_offset = (is_down_button ? -1 : 1);
              dgr_more_to_come = false;
              state_updated = true;
            }

            // If the button was held and the hold was not processed by a rule, we changed the
            // brightness and sent updates with the more-to-come message type while the button was
            // held. Send a final update.
            else if (!button_hold_processed[button_index]) {
              dgr_item = DGR_ITEM_FLAGS;
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
    if (is_power_button) {
      ignore_power_button = false;
      button_tapped = false;
    }
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
    int32_t new_bri = bri + bri_offset * ((dgr_item ? 16 : Settings.light_correction ? 4 : bri / 16 + 1));

    if (bri_offset > 0) {
      if (new_bri > 255) new_bri = 255;
    }
    else {
      if (new_bri < 1) new_bri = 1;
    }
    if (new_bri != bri) {
#ifdef USE_DEVICE_GROUPS
      SendDeviceGroupMessage(power_button_index, (dgr_more_to_come ? DGR_MSGTYP_UPDATE_MORE_TO_COME : DGR_MSGTYP_UPDATE), DGR_ITEM_LIGHT_BRI, new_bri);
#endif  // USE_DEVICE_GROUPS
#ifdef USE_PWM_DIMMER_REMOTE
      if (!active_device_is_local) {
        active_remote_pwm_dimmer->bri_power_on = active_remote_pwm_dimmer->bri = new_bri;
        PWMDimmerSetBrightnessLeds(new_bri);
      }
      else {
#endif  // USE_PWM_DIMMER_REMOTE
        skip_light_fade = true;
#ifdef USE_DEVICE_GROUPS
        ignore_dgr_sends = true;
#endif  // USE_DEVICE_GROUPS
        light_state.setBri(new_bri);
        LightAnimate();
        skip_light_fade = false;
#ifdef USE_DEVICE_GROUPS
        ignore_dgr_sends = false;
#endif  // USE_DEVICE_GROUPS
        Settings.bri_power_on = new_bri;
#ifdef USE_PWM_DIMMER_REMOTE
      }
#endif  // USE_PWM_DIMMER_REMOTE
    }
#ifdef USE_PWM_DIMMER_REMOTE
    else if (!active_device_is_local)
      PWMDimmerSetBrightnessLeds(new_bri);
#endif  // USE_PWM_DIMMER_REMOTE
    else
        PWMDimmerSetBrightnessLeds(-1);
  }

  // If we need to toggle the power on, do it.
  else if (power_on_bri) {
    power_t new_power;
#ifdef USE_DEVICE_GROUPS
#ifdef USE_PWM_DIMMER_REMOTE
    if (!active_device_is_local) {
      active_remote_pwm_dimmer->power_on ^= 1;
      new_power = active_remote_pwm_dimmer->power_on;
      PWMDimmerSetBrightnessLeds(new_power ? -power_on_bri : 0);
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
#ifdef USE_DEVICE_GROUPS
      Light.devgrp_no_channels_out = true;
#endif // USE_DEVICE_GROUPS
      ExecuteCommandPower(1, POWER_TOGGLE, SRC_RETRY);
#ifdef USE_DEVICE_GROUPS
      Light.devgrp_no_channels_out = false;
#endif // USE_DEVICE_GROUPS
#ifdef USE_PWM_DIMMER_REMOTE
    }
#endif  // USE_PWM_DIMMER_REMOTE
  }

  // If the up or down button was tapped while holding the power button and the up or down button
  // is being held or was just released after not being held, handle the operation.
  if (handle_tap) {
    ignore_power_button = tap_handled = true;

    // If the down button was tapped while holding the power button, send a device group update to
    // select the previous/next fixed color.
    if (down_button_tapped) {
#ifdef USE_DEVICE_GROUPS
      int8_t add_value = (is_down_button ? -1 : 1);
#ifdef USE_PWM_DIMMER_REMOTE
      if (!active_device_is_local) {
        active_remote_pwm_dimmer->fixed_color_index += add_value;
        dgr_value = active_remote_pwm_dimmer->fixed_color_index;
      }
      else {
#endif  // USE_PWM_DIMMER_REMOTE
        local_fixed_color_index += add_value;
        dgr_value = local_fixed_color_index;
#ifdef USE_PWM_DIMMER_REMOTE
      }
#endif  // USE_PWM_DIMMER_REMOTE
      dgr_item = DGR_ITEM_LIGHT_FIXED_COLOR;
#endif // USE_DEVICE_GROUPS
      ;
    }
    // If the down button was tapped while holding the power button, publish an MQTT Event Trigger#.
    else {
      mqtt_trigger = (is_down_button ? 3 : 4);
    }
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

  // If we're not changing the brightness or toggling the power and we made changes, send a group
  // update.
  if (dgr_item) {
#ifdef USE_DEVICE_GROUPS
    DevGroupMessageType message_type = DGR_MSGTYP_UPDATE_DIRECT;
    if (handle_tap && active_device_is_local) message_type = (DevGroupMessageType)(message_type + DGR_MSGTYPFLAG_WITH_LOCAL);
    SendDeviceGroupMessage(power_button_index, message_type, dgr_item, dgr_value);
#endif  // USE_DEVICE_GROUPS
#ifdef USE_PWM_DIMMER_REMOTE
    if (active_device_is_local)
#endif  // USE_PWM_DIMMER_REMOTE
      light_controller.saveSettings();
  }

  if (state_updated)
#ifdef USE_PWM_DIMMER_REMOTE
    if (active_device_is_local)
#endif  // USE_PWM_DIMMER_REMOTE
      if (Settings.flag3.hass_tele_on_power)  // SetOption59 - Send tele/%topic%/STATE in addition to stat/%topic%/RESULT
        MqttPublishTeleState();
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

#ifdef USE_DEVICE_GROUPS
void CmndPWMDimmerPWMs(void)
{
  if (XdrvMailbox.data_len > 0 && XdrvMailbox.payload <= 5) {
    Settings.pwm_dimmer_cfg.pwm_count = XdrvMailbox.payload - 1;
    restart_flag = 2;
  }
  Response_P(PSTR("{\"" D_CMND_PWM_DIMMER_PWMS "\":%u}"), Settings.pwm_dimmer_cfg.pwm_count + 1);
}
#endif  // USE_DEVICE_GROUPS

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv35(uint8_t function)
{
  bool result = false;

  if (PWM_DIMMER != my_module_type) return result;

  switch (function) {
    case FUNC_EVERY_SECOND:
      // Turn off/restore the brightness LED's if it's time.
      if (led_timeout_seconds && !--led_timeout_seconds) {
        PWMDimmerSetBrightnessLeds(-2);
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
