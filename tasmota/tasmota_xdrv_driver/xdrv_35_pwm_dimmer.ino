/*
  xdrv_35_pwm_dimmer.ino - PWM Dimmer Switch support for Tasmota

  Copyright (C) 2021  Paul C Diem

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
#ifdef USE_LIGHT

/*********************************************************************************************\
* Support for Martin Jerry/acenx/Tessan/NTONPOWER SD0x PWM dimmer switches. The brightness of
* the load for these dimmers is controlled by a PWM GPIO pin. Examples are:
*
* https://www.amazon.com/dp/B07FXYSVR1
* https://www.amazon.com/dp/B07V26Q3VD
* https://www.amazon.com/dp/B07K67D43J
* https://www.amazon.com/dp/B07TTGFWFM
*
* Template for Linkind device
* {"NAME":"ESP32-Linkind","GPIO":[6213,8448,0,0,640,0,0,0,0,288,0,0,0,0,0,0,0,608,0,0,0,544,0,0,0,0,0,0,33,32,0,0,0,0,0,0],"FLAG":0,"BASE":1}
*
\*********************************************************************************************/

#define XDRV_35             35
#define MAX_PWM_DIMMER_KEYS 3

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

uint32_t ignore_any_key_time = 0;
uint32_t button_hold_time[3];
uint8_t led_timeout_seconds = 0;
uint8_t power_button_index = 0;
uint8_t down_button_index = 1;
uint8_t buttons_pressed = 0;
uint8_t local_fixed_color_index = 128;
bool is_two_button;
bool button_tapped = false;
bool down_button_tapped = false;
bool ignore_power_button = false;
bool multibutton_in_progress = false;
bool power_button_increases_bri = true;
bool tap_handled = false;
bool invert_power_button_bri_direction = false;
bool button_pressed[3] = { false, false, false };
bool button_held[3];
bool button_unprocessed[3] = { false, false, false };
#ifdef USE_PWM_DIMMER_REMOTE
struct remote_pwm_dimmer remote_pwm_dimmers[MAX_PWM_DIMMER_KEYS];
struct remote_pwm_dimmer * active_remote_pwm_dimmer;
#endif  // USE_PWM_DIMMER_REMOTE

void PWMModulePreInit(void)
{
  Settings->seriallog_level = 0;
  Settings->flag.mqtt_serial = 0;  // Disable serial logging
  Settings->ledstate = 0;          // Disable LED usage

  // If the module was just changed to PWM Dimmer, set the defaults.
  if (TasmotaGlobal.module_changed) {
    Settings->flag.pwm_control = true;     // SetOption15 - Switch between commands PWM or COLOR/DIMMER/CT/CHANNEL
    Settings->bri_power_on = Settings->bri_preset_low = Settings->bri_preset_high = 0;
 }

  // Previous versions of PWM Dimmer used SetOption32 - Button held for factor times longer as the
  // hold time. The hold time is now fixed and SetOption32 is used as normal including to
  // determine how long a button is held before a reset command is executed. If SetOption32 is
  // still 5, change it to 40 (the default).
  if (Settings->param[P_HOLD_TIME] == 5) Settings->param[P_HOLD_TIME] = 40;

  // Make sure the brightness level settings are sensible.
  if (!Settings->bri_power_on) Settings->bri_power_on = 128;
  if (!Settings->bri_preset_low) Settings->bri_preset_low = 10;
  if (Settings->bri_preset_high < Settings->bri_preset_low) Settings->bri_preset_high = 255;

  PWMDimmerSetPoweredOffLed();

  // The relay initializes to on. If the power is supposed to be off, turn the relay off.
//  if (!TasmotaGlobal.power && PinUsed(GPIO_REL1)) digitalWrite(Pin(GPIO_REL1), bitRead(TasmotaGlobal.rel_inverted, 0) ? 1 : 0);

  // Find out how many buttons we have.
  uint8_t button_count = 0;
  for (uint32_t button_index = 0; button_index < MAX_PWM_DIMMER_KEYS; button_index++) {
    if (PinUsed(GPIO_KEY1, button_index)) button_count++;
  }
  if ((is_two_button = (button_count == 2))) down_button_index = 99;

#ifdef USE_PWM_DIMMER_REMOTE
  // If remote device mode is enabled, set the device group count to the number of buttons
  // present.
  if (Settings->flag4.multiple_device_groups) {
    Settings->flag4.device_groups_enabled = true;
    device_group_count = button_count;

    // If no relay or PWM is defined, all buttons control remote devices.
    if (!PinUsed(GPIO_REL1) && !PinUsed(GPIO_PWM1)
#ifdef USE_I2C
      && !PinUsed(GPIO_I2C_SCL)
#endif  // USE_I2C
      ) {
      first_device_group_is_local = false;

      // Back out the changes made in the light module under the assumtion we have a relay or PWM.
      UpdateDevicesPresent(-1);
      TasmotaGlobal.light_type = 0;
    }

    for (uint8_t i = 0; i < MAX_PWM_DIMMER_KEYS; i++) {
      active_remote_pwm_dimmer = &remote_pwm_dimmers[i];
      active_remote_pwm_dimmer->bri_power_on = 128;
      active_remote_pwm_dimmer->bri_preset_low = 10;
      active_remote_pwm_dimmer->bri_preset_high = 255;
      active_remote_pwm_dimmer->fixed_color_index = 128;
    }
  }
#endif  // USE_PWM_DIMMER_REMOTE
}

// bri: -1 = set to last local light bri, -2 = timeout, 0-255 = set to bri
void PWMDimmerSetBrightnessLeds(int32_t bri)
{
  // Find out how many of the LEDs have their ledmask bit set.
  uint32_t leds = 0;
  uint32_t mask = 1;
  int32_t led;
  for (led = 0; led < TasmotaGlobal.leds_present; led++) {
    if (Settings->ledmask & mask) leds++;
    mask <<= 1;
  }

  // If we found at least one LED, get the brightness to show and calculate the brightness level
  // difference between each LED.
  if (leds) {
    led_timeout_seconds = 5;
    if (bri < 0) {
      bri = ((bri == -2 && Settings->flag4.led_timeout) || !Light.power ? 0 : TasmotaGlobal.pwm_dimmer_led_bri);
      if (!bri || !Settings->flag4.led_timeout) led_timeout_seconds = 0;
    }

    // Turn the LED's on/off.
    uint32_t step = 256 / (leds + 1);
    int32_t level = 0;
    if (TasmotaGlobal.gpio_optiona.linkind_support) {
      step = 256 / leds;
      level = -step;
    }
    led = -1;
    mask = 0;
    for (uint32_t count = 0; count < leds; count++) {
      level += step;
      for (;;) {
        led++;
        mask <<= 1;
        if (!mask) mask = 1;
        if (Settings->ledmask & mask) break;
      }
      if (TasmotaGlobal.gpio_optiona.linkind_support) {
        SetLedPowerIdx(led, bri > level);
      } else {
        uint16_t pwm_led_bri = changeUIntScale((bri > level ? bri - level : 0), 0, step, 0, Settings->pwm_range);
        analogWrite(Pin(GPIO_LED1, led), bitRead(TasmotaGlobal.led_inverted, led) ? Settings->pwm_range - pwm_led_bri : pwm_led_bri);
      }
    }
  }
}

void PWMDimmerSetPoweredOffLed(void)
{
  // Set the powered-off LED state.
  if (PinUsed(GPIO_LEDLNK)) {
    bool power_off_led_on = !TasmotaGlobal.power && Settings->flag4.powered_off_led;
    if (TasmotaGlobal.ledlnk_inverted) power_off_led_on ^= 1;
    digitalWrite(Pin(GPIO_LEDLNK), power_off_led_on);
  }
}

void PWMDimmerSetPower(void)
{
  DigitalWrite(GPIO_REL1, 0, bitRead(TasmotaGlobal.rel_inverted, 0) ? !TasmotaGlobal.power : TasmotaGlobal.power);
  PWMDimmerSetPoweredOffLed();
}

#ifdef USE_DEVICE_GROUPS
void PWMDimmerHandleDevGroupItem(void)
{
#ifdef USE_PWM_DIMMER_REMOTE
  uint8_t device_group_index = *(uint8_t *)XdrvMailbox.topic;
  bool is_local = ((XdrvMailbox.index & DGR_FLAG_LOCAL) != 0);
  if (device_group_index > MAX_PWM_DIMMER_KEYS) return;
  struct remote_pwm_dimmer * remote_pwm_dimmer = &remote_pwm_dimmers[device_group_index];
#else  // USE_PWM_DIMMER_REMOTE
  if (!(XdrvMailbox.index & DGR_FLAG_LOCAL)) return;
#endif  // !USE_PWM_DIMMER_REMOTE
  uint32_t value = XdrvMailbox.payload;

  switch (XdrvMailbox.command_code) {
#ifdef USE_PWM_DIMMER_REMOTE
    case DGR_ITEM_LIGHT_BRI:
      remote_pwm_dimmer->bri = value;
      break;
    case DGR_ITEM_POWER:
      remote_pwm_dimmer->power_on = value & 1;
      remote_pwm_dimmer->power_button_increases_bri = (remote_pwm_dimmer->bri < 128);
      break;
#endif  // USE_PWM_DIMMER_REMOTE
    case DGR_ITEM_LIGHT_FIXED_COLOR:
#ifdef USE_PWM_DIMMER_REMOTE
      remote_pwm_dimmer->fixed_color_index = value;
      if (is_local)
#endif  // USE_PWM_DIMMER_REMOTE
        local_fixed_color_index = value;
      break;
    case DGR_ITEM_BRI_POWER_ON:
#ifdef USE_PWM_DIMMER_REMOTE
      remote_pwm_dimmer->bri_power_on = value;
      if (is_local)
#endif  // USE_PWM_DIMMER_REMOTE
        Settings->bri_power_on = value;
      break;
    case DGR_ITEM_BRI_PRESET_LOW:
#ifdef USE_PWM_DIMMER_REMOTE
      remote_pwm_dimmer->bri_preset_low = value;
      if (is_local)
#endif  // USE_PWM_DIMMER_REMOTE
        Settings->bri_preset_low = value;
      break;
    case DGR_ITEM_BRI_PRESET_HIGH:
#ifdef USE_PWM_DIMMER_REMOTE
      remote_pwm_dimmer->bri_preset_high = value;
      if (is_local)
#endif  // USE_PWM_DIMMER_REMOTE
        Settings->bri_preset_high = value;
      break;
    case DGR_ITEM_STATUS:
#ifdef USE_PWM_DIMMER_REMOTE
      if (is_local)
#endif  // USE_PWM_DIMMER_REMOTE
        SendDeviceGroupMessage(0, DGR_MSGTYP_UPDATE, DGR_ITEM_BRI_POWER_ON, Settings->bri_power_on,
          DGR_ITEM_BRI_PRESET_LOW, Settings->bri_preset_low, DGR_ITEM_BRI_PRESET_HIGH, Settings->bri_preset_high);
#ifdef USE_PWM_DIMMER_REMOTE
      else
        SendDeviceGroupMessage(-device_group_index, DGR_MSGTYP_UPDATE, DGR_ITEM_POWER, remote_pwm_dimmer->power_on,
          DGR_ITEM_BRI_POWER_ON, remote_pwm_dimmer->bri_power_on, DGR_ITEM_BRI_PRESET_LOW, remote_pwm_dimmer->bri_preset_low,
          DGR_ITEM_BRI_PRESET_HIGH, remote_pwm_dimmer->bri_preset_high);
#endif  // USE_PWM_DIMMER_REMOTE
      break;
  }
}
#endif  // USE_DEVICE_GROUPS

/*
*      ---------------- Single -----------------  ------------------------- Hold -------------------------
*      Off                       On               Off                       On
*
* 3 Button:
*   1  Power on                  Power off        Power on at low preset    Alternately inc/dec brightness
*   2  Power on at low preset    Dec brightness   NOP                       Dec brightness
*   3  Power on at high preset   Inc brightness   NOP                       Inc brightness
*
* 2 Button:
*   1  NOP                       Power off        Power on at low preset    Dec brightness
*   2  Power on                  Inc brightness   Power on at high preset   Inc brightness
*
* 1 Button:
*   1  Power on                  Power off        Power on at low preset    Alternately inc/dec brightness
*/
void PWMDimmerHandleButton(uint32_t button_index, bool pressed)
{
  bool handle_tap = false;
  bool state_updated = false;
  int8_t bri_hold = 0;
  int8_t bri_tap = 0;
  uint8_t power_on_bri = 0;
  uint8_t dgr_item = 0;
  uint8_t dgr_value = 0;
  uint8_t dgr_more_to_come = true;
  uint8_t mqtt_trigger = 0;

  // Initialize some variables.
#ifdef USE_PWM_DIMMER_REMOTE
  bool power_is_on = (active_remote_pwm_dimmer ? active_remote_pwm_dimmer->power_on : TasmotaGlobal.power);
  bool is_power_button = (button_index == power_button_index);
  bool is_down_button = (button_index == down_button_index);
#else // USE_PWM_DIMMER_REMOTE
  bool power_is_on = TasmotaGlobal.power;
  bool is_power_button = !button_index;
  bool is_down_button = (is_two_button ? false : button_index == (power_button_index ? 0 : 1));
#endif  // USE_PWM_DIMMER_REMOTE

  // If the button is being held, ...
  if (pressed) {
    uint32_t now = millis();

    // If the button was pressed and released but was not processed by support_button because the
    // button interval had not elapsed, publish an MQTT message.
    if (button_unprocessed[button_index]) {
      mqtt_trigger = 5;
#ifdef USE_PWM_DIMMER_REMOTE
      if (!active_remote_pwm_dimmer) mqtt_trigger += button_index;
#endif  // USE_PWM_DIMMER_REMOTE
      button_hold_time[button_index] = now + 750;
    }

    // Otherwise, if this is about the power button, ...
    else if (is_power_button) {

      // If we're not ignoring the power button and no other buttons are pressed, ...
      if (!ignore_power_button && buttons_pressed == 1) {

        // If the power is on, adjust the brightness. Set the direction based on the current
        // direction for the device and then invert the direction when the power button is released.
        // The new brightness will be calculated below.
        if (power_is_on) {
          if (is_two_button && !Settings->flag4.multiple_device_groups) {
            bri_hold = -1;
          }
          else {
#ifdef USE_PWM_DIMMER_REMOTE
            bri_hold = (active_remote_pwm_dimmer ? (active_remote_pwm_dimmer->power_button_increases_bri ? 1 : -1) : (power_button_increases_bri ? 1 : -1));
#else // USE_PWM_DIMMER_REMOTE
            bri_hold = (power_button_increases_bri ? 1 : -1);
#endif  // USE_PWM_DIMMER_REMOTE
            invert_power_button_bri_direction = true;
          }
        }

        // If the power is not on, turn it on using an initial brightness of bri_preset_low and set
        // the power button hold time to delay before we start increasing the brightness.
        else {
#ifdef USE_PWM_DIMMER_REMOTE
          if (active_remote_pwm_dimmer)
            power_on_bri = active_remote_pwm_dimmer->bri = active_remote_pwm_dimmer->bri_preset_low;
          else
#endif  // USE_PWM_DIMMER_REMOTE
            power_on_bri = Settings->bri_preset_low;
          button_hold_time[button_index] = now + 500;
        }
      }
    }

    // If this is about the down or up buttons, ...
    else {

      // If the power button is also pressed, ...
      if (button_pressed[power_button_index]) {

        // If the up or down button was tapped while holding the power button before this, handle
        // the operation below.
        if (button_tapped) {
          handle_tap = ignore_power_button = true;
          button_hold_time[button_index] = now + 750;
        }

        // Otherwise, if the power is on and remote mode is enabled, adjust the brightness. Set the
        // direction based on which button is pressed. The new brightness will be calculated below.
        else if (power_is_on && Settings->flag4.multiple_device_groups) {
          bri_hold = (is_down_button ? -1 : 1);
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
      if (!button_tapped) {
        if (power_is_on) {
          bri_hold = (is_down_button ? -1 : 1);
        }

        // If the power is off and this ia a two button switch, turn the power
        // on using a temporary brightness of bri_preset_high.
        else {
#ifdef USE_PWM_DIMMER_REMOTE
          if (active_remote_pwm_dimmer)
            power_on_bri = active_remote_pwm_dimmer->bri = active_remote_pwm_dimmer->bri_preset_high;
          else
#endif  // USE_PWM_DIMMER_REMOTE
            power_on_bri = Settings->bri_preset_high;
        }
      }
    }
  }

  // If the button was just released, ...
  else {
    bool button_was_held = button_held[button_index];

    // If this is about the power button, ...
    if (is_power_button) {

      // If the up or down button was tapped while the power button was held and the up or down
      // buttons weren't tapped or held afterwards, handle the operation based on which button was
      // tapped.
      if (button_tapped) {
        if (!tap_handled) {
#ifdef USE_PWM_DIMMER_REMOTE
          if (!active_remote_pwm_dimmer) {
#endif // USE_PWM_DIMMER_REMOTE

            // Toggle the LED timeout.
            if (down_button_tapped) {
                Settings->flag4.led_timeout ^= 1;
                if (Light.power) PWMDimmerSetBrightnessLeds(Settings->flag4.led_timeout ? 0 : -1);
            }

            // Toggle the powered-off LED option.
            else {
                Settings->flag4.powered_off_led ^= 1;
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
          if (active_remote_pwm_dimmer) {
            active_remote_pwm_dimmer->power_button_increases_bri ^= 1;
          } else
#endif  // USE_PWM_DIMMER_REMOTE
            power_button_increases_bri ^= 1;
          dgr_item = DGR_ITEM_FLAGS;
          state_updated = true;
        }
      }

      // If the power button was not held and we're not ignoring the next power button release,
      // toggle the power.
      else if (!ignore_power_button) {
#ifdef USE_PWM_DIMMER_REMOTE
        if (active_remote_pwm_dimmer)
          power_on_bri = active_remote_pwm_dimmer->bri_power_on;
        else
#endif  // USE_PWM_DIMMER_REMOTE
          if (!is_two_button || Settings->flag4.multiple_device_groups || power_is_on)
            power_on_bri = Settings->bri_power_on;
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
            bri_tap = (is_down_button ? -1 : 1);
            dgr_more_to_come = false;
            state_updated = true;
          }

          // If the button was held, we changed the brightness and sent updates with the
          // more-to-come message type while the button was held. Send a final update.
          else {
            dgr_item = DGR_ITEM_FLAGS;
            state_updated = true;
          }
        }

        // If the power is off and this is a two button switch, turn the power
        // on.
        else if (is_two_button) {
#ifdef USE_PWM_DIMMER_REMOTE
          if (active_remote_pwm_dimmer)
            power_on_bri = active_remote_pwm_dimmer->bri_power_on;
          else
#endif  // USE_PWM_DIMMER_REMOTE
            power_on_bri = Settings->bri_power_on;
        }

        // If the power is off and this is not a two button switch, turn the
        // power on using a temporary brightness of bri_preset_low if the down
        // button is pressed or bri_preset_high if the up button is pressed.
        else {
#ifdef USE_PWM_DIMMER_REMOTE
          if (active_remote_pwm_dimmer)
            power_on_bri = active_remote_pwm_dimmer->bri = (is_down_button ? active_remote_pwm_dimmer->bri_preset_low : active_remote_pwm_dimmer->bri_preset_high);
          else
#endif  // USE_PWM_DIMMER_REMOTE
            power_on_bri = (is_down_button ? Settings->bri_preset_low : Settings->bri_preset_high);
        }
      }
    }
  }

  // If we need to adjust the brightness, do it.
  int32_t negated_device_group_index = -power_button_index;
  if (bri_hold || bri_tap) {
    int32_t bri;
#ifdef USE_PWM_DIMMER_REMOTE
    if (active_remote_pwm_dimmer)
      bri = active_remote_pwm_dimmer->bri;
    else
#endif  // USE_PWM_DIMMER_REMOTE
      bri = light_state.getBri();
    int32_t bri_offset = Settings->dimmer_step;
    if (bri_tap)
      bri_offset *= bri_tap;
    else {
      bri_offset /= 5;
      if (!Settings->light_correction) bri_offset *= bri / 32;
      if (bri_offset < 1) bri_offset = 1;
      bri_offset *= bri_hold;
    }
    int32_t new_bri = bri + bri_offset;
    if (bri_offset > 0) {
      if (new_bri > 255) new_bri = 255;
    }
    else {
      if (new_bri < 1) new_bri = 1;
    }
    if (new_bri != bri) {
#ifdef USE_DEVICE_GROUPS
      SendDeviceGroupMessage(negated_device_group_index, (dgr_more_to_come ? DGR_MSGTYP_UPDATE_MORE_TO_COME : DGR_MSGTYP_UPDATE_DIRECT), DGR_ITEM_LIGHT_BRI, new_bri, DGR_ITEM_BRI_POWER_ON, new_bri);
#endif  // USE_DEVICE_GROUPS
#ifdef USE_PWM_DIMMER_REMOTE
      if (active_remote_pwm_dimmer) {
        active_remote_pwm_dimmer->bri_power_on = active_remote_pwm_dimmer->bri = new_bri;
        PWMDimmerSetBrightnessLeds(new_bri);
      }
      else {
#endif  // USE_PWM_DIMMER_REMOTE
        TasmotaGlobal.skip_light_fade = true;
#ifdef USE_DEVICE_GROUPS
        ignore_dgr_sends = true;
#endif  // USE_DEVICE_GROUPS
        light_state.setBri(new_bri);
        Settings->light_dimmer = light_state.BriToDimmer(new_bri);
        LightAnimate();
        TasmotaGlobal.skip_light_fade = false;
#ifdef USE_DEVICE_GROUPS
        ignore_dgr_sends = false;
#endif  // USE_DEVICE_GROUPS
        Settings->bri_power_on = new_bri;
#ifdef USE_PWM_DIMMER_REMOTE
      }
#endif  // USE_PWM_DIMMER_REMOTE
    }
#ifdef USE_PWM_DIMMER_REMOTE
    else if (active_remote_pwm_dimmer)
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
    if (active_remote_pwm_dimmer) {
      active_remote_pwm_dimmer->power_on ^= 1;
      new_power = active_remote_pwm_dimmer->power_on;
      PWMDimmerSetBrightnessLeds(new_power ? -power_on_bri : 0);
    }
    else
#endif  // USE_PWM_DIMMER_REMOTE
      new_power = TasmotaGlobal.power ^ 1;
    if (new_power)
      SendDeviceGroupMessage(negated_device_group_index, DGR_MSGTYP_UPDATE, DGR_ITEM_LIGHT_BRI, power_on_bri, DGR_ITEM_POWER, new_power);
    else
      SendDeviceGroupMessage(negated_device_group_index, DGR_MSGTYP_UPDATE, DGR_ITEM_POWER, new_power);
#endif  // USE_DEVICE_GROUPS
#ifdef USE_PWM_DIMMER_REMOTE
    if (active_remote_pwm_dimmer)
      active_remote_pwm_dimmer->power_button_increases_bri = (power_on_bri < 128);
    else {
#endif  // USE_PWM_DIMMER_REMOTE
      light_state.setBri(power_on_bri);
      Settings->light_dimmer = light_state.BriToDimmer(power_on_bri);
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
    tap_handled = true;

    // If the down button was tapped while holding the power button, send a device group update to
    // select the previous/next fixed color.
    if (down_button_tapped) {
#ifdef USE_DEVICE_GROUPS
      int8_t add_value = (is_down_button ? -1 : 1);
#ifdef USE_PWM_DIMMER_REMOTE
      if (active_remote_pwm_dimmer) {
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
    // If the up button was tapped while holding the power button, publish an MQTT Event Trigger#.
    else {
      mqtt_trigger = 3;
      if (is_down_button) mqtt_trigger = 4;
    }
  }

  // If we need to publish an MQTT trigger, do it.
  if (mqtt_trigger) {
    char topic[TOPSZ];
    Response_P(PSTR("Trigger%u"), mqtt_trigger);
#ifdef USE_DEVICE_GROUPS
    if (Settings->flag4.device_groups_enabled) {
      snprintf_P(topic, sizeof(topic), PSTR("cmnd/%s/EVENT"), device_groups[power_button_index].group_name);
      MqttPublish(topic);
    }
    else
#endif  // USE_DEVICE_GROUPS
      MqttPublishPrefixTopicRulesProcess_P(CMND, PSTR("EVENT"));
  }

  // If we need to send a device group update, do it.
  if (dgr_item) {
#ifdef USE_DEVICE_GROUPS
    DevGroupMessageType message_type = DGR_MSGTYP_UPDATE_DIRECT;
#ifdef USE_PWM_DIMMER_REMOTE
    if (handle_tap && !active_remote_pwm_dimmer)
#else // USE_PWM_DIMMER_REMOTE
    if (handle_tap)
#endif  // USE_PWM_DIMMER_REMOTE
      message_type = (DevGroupMessageType)(message_type + DGR_MSGTYPFLAG_WITH_LOCAL);
    SendDeviceGroupMessage(negated_device_group_index, message_type, dgr_item, dgr_value);
#endif  // USE_DEVICE_GROUPS
#ifdef USE_PWM_DIMMER_REMOTE
    if (!active_remote_pwm_dimmer)
#endif  // USE_PWM_DIMMER_REMOTE
      light_controller.saveSettings();
  }

  if (state_updated && Settings->flag3.hass_tele_on_power) { // SetOption59 - Send tele/%topic%/STATE in addition to stat/%topic%/RESULT
#ifdef USE_PWM_DIMMER_REMOTE
    if (!active_remote_pwm_dimmer)
#endif  // USE_PWM_DIMMER_REMOTE
      MqttPublishTeleState();
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
    parm[0] = Settings->bri_preset_low;
    parm[1] = Settings->bri_preset_high;
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
        Settings->bri_preset_low = parm[0];
        Settings->bri_preset_high = parm[1];
      } else
      {
        Settings->bri_preset_low = parm[1];
        Settings->bri_preset_high = parm[0];
      }
#ifdef USE_DEVICE_GROUPS
      SendDeviceGroupMessage(0, DGR_MSGTYP_UPDATE, DGR_ITEM_BRI_PRESET_LOW, Settings->bri_preset_low, DGR_ITEM_BRI_PRESET_HIGH, Settings->bri_preset_high);
#endif  // USE_DEVICE_GROUPS
    }
  }
  Response_P(PSTR("{\"" D_CMND_BRI_PRESET "\":{\"Low\":%d,\"High\":%d}}"), Settings->bri_preset_low, Settings->bri_preset_high);
}

#ifdef USE_DEVICE_GROUPS
void CmndPWMDimmerPWMs(void)
{
  if (XdrvMailbox.data_len > 0 && XdrvMailbox.payload <= 5) {
    Settings->pwm_dimmer_cfg.pwm_count = XdrvMailbox.payload - 1;
    TasmotaGlobal.restart_flag = 2;
  }
  Response_P(PSTR("{\"" D_CMND_PWM_DIMMER_PWMS "\":%u}"), Settings->pwm_dimmer_cfg.pwm_count + 1);
}
#endif  // USE_DEVICE_GROUPS

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv35(uint32_t function)
{
  bool result = false;

  if (PWM_DIMMER != TasmotaGlobal.module_type) return result;

  switch (function) {
    case FUNC_EVERY_SECOND:
      // Turn off/restore the brightness LED's if it's time.
      if (led_timeout_seconds && !--led_timeout_seconds) {
        PWMDimmerSetBrightnessLeds(-2);
      }

      // The powered-off LED is also the LedLink LED. If the state of it gets changed,
      // restore_powered_off_led_counter will get set to the number of seconds
      // to wait before restoring it to the proper state.
      if (TasmotaGlobal.restore_powered_off_led_counter && !--TasmotaGlobal.restore_powered_off_led_counter) PWMDimmerSetPoweredOffLed();
      break;

    case FUNC_BUTTON_PRESSED:
      // If the button is pressed or was just released, ...
      if (!Settings->flag3.mqtt_buttons && (!XdrvMailbox.payload || button_pressed[XdrvMailbox.index])) {
        uint32_t button_index = XdrvMailbox.index;
        uint32_t now = millis();

        // If the button is pressed, ...
        if (!XdrvMailbox.payload) {

          // If the button was just pressed, ...
          if (!button_pressed[button_index]) {

#ifdef USE_PWM_DIMMER_REMOTE
            // If there are no other buttons pressed right now and remote mode is enabled, make the
            // device associated with this button the device we're going to control. In remote mode,
            // whichever button is pressed first becomes the power button and any buttons pressed
            // while it is held affect the device associated with it. The up and down buttons change
            // depeneding on which button is the current power button and are based on the GPIOs
            // used on the MJ-SD01 type dimmers.
            //
            //                    Power   Down   Up
            // Position  GPIO     Button  GPIO  GPIO
            // --------  ----     ------  ----  ----
            // Top          0          1     1     0
            // Middle       1          2    15     0
            // Bottom      15          3    15     1
            if (!buttons_pressed && Settings->flag4.multiple_device_groups) {
              power_button_index = button_index;
              down_button_index = (is_two_button ? 99 : Pin(GPIO_KEY1, power_button_index) == 15 ? TasmotaGlobal.gpio_pin[1] : TasmotaGlobal.gpio_pin[15]) - 32;
              active_remote_pwm_dimmer = nullptr;
              if (power_button_index || !first_device_group_is_local)
                active_remote_pwm_dimmer = &remote_pwm_dimmers[power_button_index];
            }
#endif  // USE_PWM_DIMMER_REMOTE

            // Flag the button as pressed, increment the buttons pressed count and set the hold time.
            button_pressed[button_index] = true;
            buttons_pressed++;
            if (buttons_pressed > 1) multibutton_in_progress = true;
            uint32_t hold_delay = 250;
            if (button_index == power_button_index) hold_delay = Settings->param[P_HOLD_TIME] * 10;
            button_hold_time[button_index] = now + hold_delay;
          }

          // If hold time has arrived and no rule is enabled that handles the button hold, handle it.
          else if ((int32_t)(now - button_hold_time[button_index]) >= 0) {

#ifdef USE_RULES
            Response_P(PSTR("{\"Button%u\":{\"State\":3}}"), button_index + 1);
            Rules.no_execute = true;
            if (!XdrvRulesProcess(0)) {
#endif  // USE_RULES
              PWMDimmerHandleButton(button_index, true);
              button_held[button_index] = true;
#ifdef USE_RULES
            }
            Rules.no_execute = false;
#endif  // USE_RULES
          }
        }

        // If the button was just released, flag the button as released and decrement the buttons
        // pressed count.
        else {
          button_pressed[button_index] = false;
          buttons_pressed--;

          // If this is a multibutton press or the button was held, handle it.
          if (multibutton_in_progress || button_held[button_index]) {
            PWMDimmerHandleButton(button_index, false);

            // Set a timer so FUNC_ANY_KEY ignores the button if support_button winds up sending a
            // key because of this.
            ignore_any_key_time = now + 500;

            // If a multi-button operation is in progress or the button was pressed, released and
            // then held, tell support_button that we've handled it.
            result = true;
            Button.press_counter[button_index] = 0;
            if (buttons_pressed == 0) multibutton_in_progress = false;
            button_unprocessed[button_index] = false;
          }
          else {
            button_unprocessed[button_index] = true;
            ignore_any_key_time = now - 1;
          }

          // If the power button was just released, clear the flags associated with it.
          if (button_index == power_button_index) {
            if (ignore_power_button) ignore_any_key_time = now + 500;
            ignore_power_button = false;
            button_tapped = false;
          }
          button_held[button_index] = false;
        }
      }
      break;

    case FUNC_ANY_KEY:
      {
        uint32_t state = (XdrvMailbox.payload >> 8) & 0xFF;  // 0 = Off, 1 = On, 2 = Toggle, 3 = Hold, 10,11,12,13 and 14 for Button Multipress
        if ((state == 2 || state == 10) && (int32_t)(millis() - ignore_any_key_time) > 0) {

          uint32_t button_index = (XdrvMailbox.payload & 0xFF) - 1;
          button_unprocessed[button_index] = false;
          PWMDimmerHandleButton(button_index, false);
        }
      }
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

#endif  // USE_LIGHT
#endif  // USE_PWM_DIMMER
