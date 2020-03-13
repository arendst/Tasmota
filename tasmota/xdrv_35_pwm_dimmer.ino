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

#ifndef MAX_FIXED_COLOR
#define MAX_FIXED_COLOR     12
#endif  // MAX_FIXED_COLOR

// Note: CmndFade and CmndSpeed are in xdrv_04_light.
const char kPWMDimmerCommands[] PROGMEM = "|"  // No prefix
   D_CMND_BRI_PRESET "|" D_CMND_DIMMER "|" D_CMND_DIMMER_RANGE "|" D_CMND_FADE "|" D_CMND_SPEED;

void (* const PWMDimmerCommand[])(void) PROGMEM = {
  CmndBriPreset, &PWMDmmerCmndDimmer, &CmndDimmerRange, &CmndFade, &CmndSpeed };

#ifdef USE_PWM_DIMMER_REMOTE
struct remote_pwm_dimmer {
  power_t power;
  uint32_t dimmer_range;
  uint8_t light_speed;
  uint8_t bri_power_on;
  uint8_t bri_preset_low;
  uint8_t bri_preset_high;
  uint8_t fixed_color_index;
  uint8_t bri;
  bool power_button_increases_bri;
  bool light_fade;
};
#endif  // USE_PWM_DIMMER_REMOTE

uint32_t led_timeout_time = 0;
uint32_t turn_off_brightness_leds_time = 0;
uint32_t button_press_count_timeout = 0;
uint32_t button_hold_time[3];
uint8_t current_bri;
uint8_t target_bri;
uint8_t restore_powered_off_led = 0;
uint8_t power_button_index = 0;
uint8_t down_button_index = 1;
uint8_t up_button_index = 2;
uint8_t fixed_color_index;
uint8_t buttons_pressed = 0;
uint8_t button_press_count[3] = { 0, 0, 0 };
bool relay_is_on = false;
bool ignore_power_button_hold;
bool ignore_power_button_release;
bool ignore_up_down_button_release = false;
bool button_was_held = false;
bool power_button_increases_bri = true;
bool invert_power_button_bri_direction = false;
bool button_hold_sent[3];
bool button_pressed[3] = { false, false, false };
#ifdef USE_PWM_DIMMER_REMOTE
struct remote_pwm_dimmer * remote_pwm_dimmers;
struct remote_pwm_dimmer * active_remote_pwm_dimmer;
bool active_device_is_local;
#endif  // USE_PWM_DIMMER_REMOTE

void PWMModuleInit()
{
  Settings.seriallog_level = 0;
  Settings.flag.mqtt_serial = 0;  // Disable serial logging
  Settings.ledstate = 0;          // Disable LED usage
  Settings.flag.pwm_control = 0;  // Use basic PWM control instead of Light    

  if (Settings.last_module != Settings.module) {
    Settings.dimmer_hw_min = 102;
    Settings.dimmer_hw_max = 1023;
    Settings.bri_power_on = 50;
    Settings.bri_preset_low = 25;
    Settings.bri_preset_high = 255;
    Settings.last_module = Settings.module;
  }

  if (Settings.light_speed < 1) Settings.light_speed = 1;

  target_bri = ((Settings.power & 1) ? changeUIntScale(Settings.light_dimmer, 0, 100, 0, 255) : 0);
  current_bri = target_bri;

  if (pin[GPIO_PWM1] < 99) {
    uint32_t pwm_value = (current_bri ? changeUIntScale(current_bri, 1, 255, Settings.dimmer_hw_min, Settings.dimmer_hw_max) : 0);
    analogWrite(pin[GPIO_PWM1], bitRead(pwm_inverted, 0) ? Settings.dimmer_hw_max - pwm_value : pwm_value);
  }

  relay_is_on = (current_bri > 0);
  if (pin[GPIO_REL1] < 99) DigitalWrite(GPIO_REL1, bitRead(rel_inverted, 0) ? !relay_is_on : relay_is_on);

  PWMDimmerSetPoweredOffLed();
  PWMDimmerSetBrightnessLeds(0);
}

void PWMDimmerInit(void)
{
#ifdef USE_PWM_DIMMER_REMOTE
  if (Settings.flag4.remote_device_mode) {
    if (device_group_count > 1) {
      if ((remote_pwm_dimmers = (struct remote_pwm_dimmer *) calloc(device_group_count - 1, sizeof(struct remote_pwm_dimmer))) == nullptr) {
        AddLog_P2(LOG_LEVEL_ERROR, PSTR("PWMDimmer: error allocating PWM dimmer array"));
        Settings.flag4.remote_device_mode = false;
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
    uint32_t step = (!operation ? 256 / (leds_present + 1) : operation < 0 ? 255 : 0);
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
    if (!operation) led_timeout_time = (current_bri && Settings.flag4.led_timeout ? millis() + 5000 : 0);
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

void PWMDimmerAnimate(bool no_fade)
{
  // We're only here if something changed. If this is no longer the case, uncomment the following
  // line.
  //if (current_bri == target_bri) return;

  // Advance the current brightness towards the target.
  if (!no_fade && Settings.light_fade) {
    uint8_t offset = current_bri / Settings.light_speed / 5 + 1;
    uint8_t max_offset = abs(target_bri - current_bri);
    if (offset > max_offset) offset = max_offset;
    if (current_bri < target_bri)
      current_bri += offset;
    else
      current_bri -= offset;
  }
  else {
    current_bri = target_bri;
  }

  // Set the new PWM value.
  if (pin[GPIO_PWM1] < 99) {
    uint32_t pwm_value = (current_bri ? changeUIntScale(current_bri, 1, 255, Settings.dimmer_hw_min, Settings.dimmer_hw_max) : 0);
//AddLog_P2(LOG_LEVEL_INFO, PSTR("PWMDimmerAnimate: current_bri=%u, pwm_value =%u"), current_bri, pwm_value);
    analogWrite(pin[GPIO_PWM1], bitRead(pwm_inverted, 0) ? Settings.dimmer_hw_max - pwm_value : pwm_value);
  }

  // Handle a power state change.
  if (relay_is_on != (current_bri > 0)) {
    bool power_is_on = ((power & 1) != 0);
    if (power_is_on == relay_is_on) {
      ExecuteCommandPower(1, (relay_is_on ? POWER_OFF : POWER_ON), SRC_SWITCH);
    }

    relay_is_on = !relay_is_on;
    if (pin[GPIO_REL1] < 99) DigitalWrite(GPIO_REL1, bitRead(rel_inverted, 0) ? !relay_is_on : relay_is_on);

    // Set the powered-off LED.
    PWMDimmerSetPoweredOffLed();
  }

  // Set the brightness LED's.
  PWMDimmerSetBrightnessLeds(0);
}

void PWMDimmerSetBri(uint8_t bri)
{
  if (bri == target_bri) return;
  target_bri = bri;
  Settings.bri_power_on = target_bri;
  Settings.light_dimmer = changeUIntScale(target_bri, 0, 255, 0, 100);
  if (Settings.flag3.hass_tele_on_power) {  // SetOption59 - Send tele/%topic%/STATE in addition to stat/%topic%/RESULT
    MqttPublishTeleState();
  }
  PWMDimmerAnimate(false);
}

#ifdef USE_DEVICE_GROUPS
void PWMDimmerHandleDeviceGroupItem()
{
  static bool send_state = false;
  uint32_t value = XdrvMailbox.payload;
#ifdef USE_PWM_DIMMER_REMOTE
  uint8_t device_group_index = XdrvMailbox.index >> 16 & 0xff;
  bool device_is_local = device_groups[device_group_index].local;
  struct remote_pwm_dimmer * remote_pwm_dimmer = &remote_pwm_dimmers[device_group_index];
#endif  // USE_PWM_DIMMER_REMOTE

  switch (XdrvMailbox.command_code) {
    case DGR_ITEM_EOL:
      if (send_state && !(XdrvMailbox.index & DGR_FLAG_MORE_TO_COME)) {
        if (Settings.flag3.hass_tele_on_power) {  // SetOption59 - Send tele/%topic%/STATE in addition to stat/%topic%/RESULT
          MqttPublishTeleState();
        }
        send_state = false;
      }
      break;
    case DGR_ITEM_LIGHT_BRI:
#ifdef USE_PWM_DIMMER_REMOTE
      if (!device_is_local) {
        remote_pwm_dimmer->bri = value;
      }
      else
#endif  // USE_PWM_DIMMER_REMOTE
      if (target_bri != value) {
        PWMDimmerSetBri(value);
        send_state = true;
      }
      break;
    case DGR_ITEM_LIGHT_SPEED:
#ifdef USE_PWM_DIMMER_REMOTE
      if (!device_is_local) {
        remote_pwm_dimmer->light_speed = value;
      }
      else
#endif  // USE_PWM_DIMMER_REMOTE
      if (Settings.light_speed != value && value > 0 && value <= 40) {
        Settings.light_speed = value;
        send_state = true;
      }
      break;
    case DGR_ITEM_LIGHT_FIXED_COLOR:
#ifdef USE_PWM_DIMMER_REMOTE
      if (!device_is_local) {
        remote_pwm_dimmer->fixed_color_index = value;
      }
      else
#endif  // USE_PWM_DIMMER_REMOTE
        fixed_color_index = value;
      break;
    case DGR_ITEM_DIMMER_RANGE:
#ifdef USE_PWM_DIMMER_REMOTE
      if (!device_is_local)
        remote_pwm_dimmer->dimmer_range = value;
      else {
#endif  // USE_PWM_DIMMER_REMOTE
        Settings.dimmer_hw_min = value & 0xffff;
        Settings.dimmer_hw_max = value >> 16;
#ifdef USE_PWM_DIMMER_REMOTE
      }
#endif  // USE_PWM_DIMMER_REMOTE
      break;
    case DGR_ITEM_BRI_PRESET_LOW:
#ifdef USE_PWM_DIMMER_REMOTE
      if (!device_is_local)
        remote_pwm_dimmer->bri_preset_low = value;
      else
#endif  // USE_PWM_DIMMER_REMOTE
      if (Settings.bri_preset_low != value) {
        Settings.bri_preset_low = value;
        send_state = true;
      }
      break;
    case DGR_ITEM_BRI_PRESET_HIGH:
#ifdef USE_PWM_DIMMER_REMOTE
      if (!device_is_local)
        remote_pwm_dimmer->bri_preset_high = value;
      else
#endif  // USE_PWM_DIMMER_REMOTE
      if (Settings.bri_preset_high != value) {
        Settings.bri_preset_high = value;
        send_state = true;
      }
   break;
    case DGR_ITEM_BRI_POWER_ON:
#ifdef USE_PWM_DIMMER_REMOTE
      if (!device_is_local)
        remote_pwm_dimmer->bri_power_on = value;
      else
#endif  // USE_PWM_DIMMER_REMOTE
      if (Settings.bri_power_on != value) {
        Settings.bri_power_on = value;
        send_state = true;
      }
      break;
    case DGR_ITEM_STATUS:
#ifdef USE_PWM_DIMMER_REMOTE
      if (device_is_local)
#endif  // USE_PWM_DIMMER_REMOTE
        SendLocalDeviceGroupMessage(DGR_MSGTYP_UPDATE, DGR_ITEM_LIGHT_FADE, Settings.light_fade,
          DGR_ITEM_LIGHT_SPEED, Settings.light_speed, DGR_ITEM_LIGHT_BRI, target_bri,
          DGR_ITEM_BRI_PRESET_LOW, Settings.bri_preset_low, DGR_ITEM_BRI_PRESET_HIGH, Settings.bri_preset_high,
          DGR_ITEM_DIMMER_RANGE, Settings.dimmer_hw_min | Settings.dimmer_hw_max << 16,
          DGR_ITEM_BRI_POWER_ON, Settings.bri_power_on);
      break;
  }
}
#endif  // USE_DEVICE_GROUPS

void PWMDimmerHandleButton()
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
  * Released              Press up                On      No            Brigther
  * Released              Press down              On      No            Dimmer
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
  uint32_t button_index = XdrvMailbox.index;
  uint32_t now = millis();
  bool toggle_power = false;
  uint8_t dgr_item = 0;
  uint8_t dgr_value;

  // If the button is pressed, ...
  if (!XdrvMailbox.payload) {
    int8_t bri_direction = 0;

    // If the button was just pressed, flag the button as pressed, clear the hold sent flag and
    // increment the buttons pressed count.
    if (!button_pressed[button_index]) {
      button_pressed[button_index] = true;
      button_hold_sent[button_index] = false;
      buttons_pressed++;
#ifdef USE_PWM_DIMMER_REMOTE

      // If there are no other buttons pressed right now and remote mode is enabled, make the device
      // associated with this button the device we're going to control.
      if (buttons_pressed == 1 && Settings.flag4.remote_device_mode) {
        power_button_index = button_index;
        up_button_index = (button_index == 2 ? 1 : 2);
        down_button_index = (button_index ? 0 : 1);
        active_device_is_local = device_groups[power_button_index].local;
        if (!active_device_is_local) active_remote_pwm_dimmer = &remote_pwm_dimmers[power_button_index - 1];
      }
      if (button_index == power_button_index) {
#else // USE_PWM_DIMMER_REMOTE
      // If this is about the power button, initialize some variables.
      if (!button_index) {
#endif  // USE_PWM_DIMMER_REMOTE
        button_hold_time[button_index] = now + 500;
        ignore_power_button_hold = false;
        ignore_power_button_release = false;
        return;
      }

      // If this is not about the power button, load the new hold time. Note that the hold time for
      // the power button is longer than the hold time for the other buttons.
      button_hold_time[button_index] = now + 500;
    }

    // If the button is being held, send a button hold.
    else if (button_hold_time[button_index] < now) {
      if (!button_hold_sent[button_index]) {
        button_hold_sent[button_index] = true;
        SendKey(KEY_BUTTON, button_index + 1, POWER_HOLD);
      }

      // If the button has been held for over 10 seconds, execute the WiFiConfig 2 command.
      if (now - button_hold_time[button_index] > 10000) {
          char scmnd[20];
          snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_WIFICONFIG " 2"));
          ExecuteCommand(scmnd, SRC_BUTTON);
          return;
      }
    }

    // If this is about the power button, ...
#ifdef USE_PWM_DIMMER_REMOTE
    if (button_index == power_button_index) {
#else // USE_PWM_DIMMER_REMOTE
    if (!button_index) {
#endif  // USE_PWM_DIMMER_REMOTE

      // If the power button has been held with no other buttons pressed, ...
      if (!ignore_power_button_hold && button_hold_time[button_index] < now) {
        ignore_power_button_release = true;

        // If the device power is on, adjust the brightness. Set the direction based on the current
        // direction for the device and then invert the direction when the power button is released.
        // The new brightness will be calculated below.
#ifdef USE_PWM_DIMMER_REMOTE
        if (!active_device_is_local ? active_remote_pwm_dimmer->power : power) {
          bri_direction = (!active_device_is_local ? (active_remote_pwm_dimmer->power_button_increases_bri ? 1 : -1) : (power_button_increases_bri ? 1 : -1));
#else // USE_PWM_DIMMER_REMOTE
        if (power) {
          bri_direction = (power_button_increases_bri ? 1 : -1);
#endif  // USE_PWM_DIMMER_REMOTE
          invert_power_button_bri_direction = true;
        }

        // If the power is not on, turn it on using an initial brightness of bri_preset_low and set
        // the power button hold dimmer direction to true so holding the power switch increases the
        // brightness.
        else {
#ifdef USE_PWM_DIMMER_REMOTE
          if (!active_device_is_local) {
            active_remote_pwm_dimmer->bri = active_remote_pwm_dimmer->bri_preset_low;
            active_remote_pwm_dimmer->power_button_increases_bri = true;
          }
          else {
#endif  // USE_PWM_DIMMER_REMOTE
            target_bri = Settings.bri_preset_low;
            power_button_increases_bri = true;
#ifdef USE_PWM_DIMMER_REMOTE
          }
#endif  // USE_PWM_DIMMER_REMOTE
          toggle_power = true;

          // Reset the power button hold time to delay before we start increasing the brightness.
          button_hold_time[button_index] = now + 500;
        }
      }
    }

    // If this is about the down or up buttons, ...
    else {

      // If the power button is also pressed, set flags to ignore the power button being held and
      // the next power button release.
      if (button_pressed[power_button_index]) {
        ignore_power_button_release = ignore_power_button_hold = true;
      }

      // If the active device is local or the power button if also pressed, ...
#ifdef USE_PWM_DIMMER_REMOTE
      if (active_device_is_local == !button_pressed[power_button_index]) {
#endif  // USE_PWM_DIMMER_REMOTE
        bool is_down_button = (button_index == down_button_index);

        // If the power is not on, turn it on using a temporary brightness of bri_preset_low if the
        // down button is pressed or bri_preset_low if the up button is pressed.
#ifdef USE_PWM_DIMMER_REMOTE
        if ((!active_device_is_local ? !active_remote_pwm_dimmer->power : !power)) {
#else // USE_PWM_DIMMER_REMOTE
        if (!power) {
#endif  // USE_PWM_DIMMER_REMOTE
#ifdef USE_PWM_DIMMER_REMOTE
          if (!active_device_is_local)
            active_remote_pwm_dimmer->bri = (is_down_button ? active_remote_pwm_dimmer->bri_preset_low : active_remote_pwm_dimmer->bri_preset_high);
          else
#endif  // USE_PWM_DIMMER_REMOTE
            target_bri = (is_down_button ? Settings.bri_preset_low : Settings.bri_preset_high);
          toggle_power = true;
          button_hold_time[button_index] = now + 500;
          ignore_up_down_button_release = true;

          // If the power button is also pressed, set the power button hold dimmer direction
          // so holding the power switch adjusts the brightness away from the brightness we
          // just set.
#ifdef USE_PWM_DIMMER_REMOTE
          if (button_pressed[power_button_index]) active_remote_pwm_dimmer->power_button_increases_bri = is_down_button;
#endif  // USE_PWM_DIMMER_REMOTE
        }

        // If the power is on and the down/up button has been held, handle the action based on the
        // number of times the down or up button was pressed and released before holding it.
        else if (button_hold_time[button_index] < now) {
          uint8_t uint8_value;
          bool down_button_was_tapped = (button_press_count[down_button_index] > 0);
          uint8_t tap_count = (down_button_was_tapped ? button_press_count[down_button_index] : button_press_count[up_button_index]);
          uint16_t uint16_value;

          switch (tap_count) {
            case 0:
              // Adjust the brightness. Set the direction based on which button is pressed. The new
              // brightness will be calculated below.
              bri_direction = (is_down_button ? -1 : 1);
              break;

            case 1:
              if (down_button_was_tapped) {
                // Select the previous/next color.
#ifdef USE_DEVICE_GROUPS
#ifdef USE_PWM_DIMMER_REMOTE
                if (!active_device_is_local)
                  uint8_value = active_remote_pwm_dimmer->fixed_color_index;
                else
#endif  // USE_PWM_DIMMER_REMOTE
                  uint8_value = fixed_color_index;
                if (is_down_button) {
                  if (uint8_value)
                    uint8_value--;
                  else
                    uint8_value = MAX_FIXED_COLOR;
                }
                else {
                  if (uint8_value < MAX_FIXED_COLOR)
                    uint8_value++;
                  else
                    uint8_value = 0;
                }
#ifdef USE_PWM_DIMMER_REMOTE
                if (!active_device_is_local)
                  active_remote_pwm_dimmer->fixed_color_index = uint8_value;
                else
#endif  // USE_PWM_DIMMER_REMOTE
                  fixed_color_index = uint8_value;
                dgr_item = DGR_ITEM_LIGHT_FIXED_COLOR;
                dgr_value = uint8_value;
#endif // USE_DEVICE_GROUPS
                ;
              }
              else {
                // Publish MQTT Event SwitchTrigger#.
                char topic[TOPSZ];
#ifdef USE_PWM_DIMMER_REMOTE
                snprintf_P(topic, sizeof(topic), PSTR("%s/cmnd/Event"), device_groups[power_button_index].group_name);
#else  // USE_PWM_DIMMER_REMOTE
                snprintf_P(topic, sizeof(topic), PSTR("%s/cmnd/Event"), SettingsText(SET_MQTT_GRP_TOPIC));
#endif  // USE_PWM_DIMMER_REMOTE
                sprintf_P(mqtt_data, PSTR("SwitchTrigger%u"), (is_down_button ? 1 : 2));
                MqttPublish(topic);
              }
              break;

            case 2:
              if (down_button_was_tapped) {
                // Decrease/increase the minimum brightness.
#ifdef USE_PWM_DIMMER_REMOTE
                if (!active_device_is_local)
                  uint16_value = active_remote_pwm_dimmer->dimmer_range & 0xffff;
                else
#endif // USE_PWM_DIMMER_REMOTE
                  uint16_value = Settings.dimmer_hw_min;
                if (is_down_button) {
                  if (uint16_value > 0) uint16_value--;
                }
                else if (uint16_value < 65535) {
                  uint16_value++;
                }
#ifdef USE_PWM_DIMMER_REMOTE
                dgr_item = DGR_ITEM_DIMMER_RANGE;
                if (!active_device_is_local) {
                  active_remote_pwm_dimmer->dimmer_range &= 0xffff0000 | uint16_value;
                  dgr_value = active_remote_pwm_dimmer->dimmer_range;
                }
                else {
#endif // USE_PWM_DIMMER_REMOTE
                  Settings.dimmer_hw_min = uint16_value;
                  dgr_value = Settings.dimmer_hw_min | Settings.dimmer_hw_max << 16;
#ifdef USE_PWM_DIMMER_REMOTE
                }
#endif // USE_PWM_DIMMER_REMOTE
              }
              else {
                // Decrease/increase the fade speed.
#ifdef USE_PWM_DIMMER_REMOTE
                if (!active_device_is_local)
                  uint8_value = active_remote_pwm_dimmer->light_speed;
                else
#endif // USE_PWM_DIMMER_REMOTE
                  uint8_value = Settings.light_speed;
                if (is_down_button) {
                  if (uint8_value > 1) uint8_value--;
                }
                else if (uint8_value < 40) {
                  uint8_value++;
                }
#ifdef USE_PWM_DIMMER_REMOTE
                if (!active_device_is_local)
                  active_remote_pwm_dimmer->light_speed = uint8_value;
                else
#endif // USE_PWM_DIMMER_REMOTE
                  Settings.light_speed = uint8_value;
                dgr_item = DGR_ITEM_LIGHT_SPEED;
                dgr_value = uint8_value;
              }
              break;

            case 3:
              if (down_button_was_tapped) {
                // Decrease/increase the low brightness preset.
#ifdef USE_PWM_DIMMER_REMOTE
                if (!active_device_is_local)
                  uint8_value = active_remote_pwm_dimmer->bri_preset_low;
                else
#endif // USE_PWM_DIMMER_REMOTE
                  uint8_value = Settings.bri_preset_low;
                if (is_down_button) {
                  if (uint8_value > 3) uint8_value--;
                }
                else if (uint8_value < 255) {
                  uint8_value++;
                }
#ifdef USE_PWM_DIMMER_REMOTE
                if (!active_device_is_local)
                  active_remote_pwm_dimmer->bri_preset_low = uint8_value;
                else
#endif // USE_PWM_DIMMER_REMOTE
                  Settings.bri_preset_low = uint8_value;
                dgr_item = DGR_ITEM_BRI_PRESET_LOW;
                dgr_value = uint8_value;
              }
              else {
                // Decrease/increase the high brightness preset.
#ifdef USE_PWM_DIMMER_REMOTE
                if (!active_device_is_local)
                  uint8_value = active_remote_pwm_dimmer->bri_preset_high;
                else
#endif // USE_PWM_DIMMER_REMOTE
                  uint8_value = Settings.bri_preset_high;
                if (is_down_button) {
                  if (uint8_value > 3) uint8_value--;
                }
                else if (uint8_value < 255) {
                  uint8_value++;
                }
#ifdef USE_PWM_DIMMER_REMOTE
                if (!active_device_is_local)
                  active_remote_pwm_dimmer->bri_preset_high = uint8_value;
                else
#endif // USE_PWM_DIMMER_REMOTE
                  Settings.bri_preset_high = uint8_value;
                dgr_item = DGR_ITEM_BRI_PRESET_HIGH;
                dgr_value = uint8_value;
              }
              break;
          }

          button_was_held = true;

          // If the button was tapped before it was held, reset the button hold time to 1/2 second
          // and turn all the brightness LEDs on for 250ms.
          if (tap_count > 0) {
            button_hold_time[button_index] = now + 500;
            turn_off_brightness_leds_time = now + 250;
            PWMDimmerSetBrightnessLeds(1);
          }
        }
#ifdef USE_PWM_DIMMER_REMOTE
      }
#endif // USE_PWM_DIMMER_REMOTE
    }

    // If we need to adjust the brightness, do it. The brightness is adjusted faster the longer the
    // button is held.
    if (bri_direction) {
      int32_t bri;
#ifdef USE_PWM_DIMMER_REMOTE
      if (!active_device_is_local)
        bri = active_remote_pwm_dimmer->bri;
      else
#endif  // USE_PWM_DIMMER_REMOTE
        bri = target_bri;
      int32_t new_bri;
      int32_t offset = bri / 12 + 1;
      if (bri_direction > 0) {
        new_bri = bri + offset;
        if (new_bri > 255) new_bri = 255;
      }
      else {
        new_bri = bri - offset;
        if (new_bri < 3) new_bri = 3;
      }
      if (new_bri != bri) {
#ifdef USE_DEVICE_GROUPS
        SendDeviceGroupMessage(power_button_index, DGR_MSGTYP_UPDATE_MORE_TO_COME, DGR_ITEM_LIGHT_BRI, new_bri);
#endif  // USE_DEVICE_GROUPS
#ifdef USE_PWM_DIMMER_REMOTE
        if (!active_device_is_local)
          active_remote_pwm_dimmer->bri_power_on = active_remote_pwm_dimmer->bri = new_bri;
        else {
#endif  // USE_PWM_DIMMER_REMOTE
          Settings.bri_power_on = target_bri = new_bri;
          Settings.light_dimmer = changeUIntScale(target_bri, 0, 255, 0, 100);
          PWMDimmerAnimate(true);
#ifdef USE_PWM_DIMMER_REMOTE
        }
#endif  // USE_PWM_DIMMER_REMOTE
      }
    }
  }

  // If the button was just released, ...
  else if (button_pressed[button_index]) {

    // If the button was held, send a button off; otherwise, send a button toggle.
    SendKey(KEY_BUTTON, button_index + 1, (button_hold_sent[button_index] ? POWER_OFF : POWER_TOGGLE));

    // If this is about the power button, ...
#ifdef USE_PWM_DIMMER_REMOTE
    if (button_index == power_button_index) {
#else // USE_PWM_DIMMER_REMOTE
    if (!button_index) {
#endif  // USE_PWM_DIMMER_REMOTE

      // If we're ignoring the next power button released, ...
      if (ignore_power_button_release) {
        ignore_power_button_release = false;

        // If the power button was held with no other buttons pressed, we changed the brightness so
        // invert the bri direction for the next time and send a final update.
        if (invert_power_button_bri_direction) {
          invert_power_button_bri_direction = false;
          dgr_item = DGR_ITEM_LIGHT_BRI;
#ifdef USE_PWM_DIMMER_REMOTE
          if (!active_device_is_local) {
            active_remote_pwm_dimmer->power_button_increases_bri ^= 1;
            dgr_value = active_remote_pwm_dimmer->bri;
          }
          else {
#endif  // USE_PWM_DIMMER_REMOTE
            power_button_increases_bri ^= 1;
            dgr_value = target_bri;
#ifdef USE_PWM_DIMMER_REMOTE
          }
#endif  // USE_PWM_DIMMER_REMOTE
        }

        // The up and/or down buttons were pressed while the power button was pressed. Handle the
        // options based on the number of times the buttons were pressed.
        else {
          switch (button_press_count[down_button_index]) {
            case 1:
              // Toggle the powered-off LED option.
#ifdef USE_PWM_DIMMER_REMOTE
              if (active_device_is_local) {
#endif // USE_PWM_DIMMER_REMOTE
                Settings.flag4.powered_off_led ^= 1;
                PWMDimmerSetPoweredOffLed();
#ifdef USE_PWM_DIMMER_REMOTE
              }
#endif // USE_PWM_DIMMER_REMOTE
              break;
            case 2:
              // Toggle fading.
              dgr_item = DGR_ITEM_LIGHT_FADE;
#ifdef USE_PWM_DIMMER_REMOTE
              if (!active_device_is_local) {
                active_remote_pwm_dimmer->light_fade ^= 1;
                dgr_value = active_remote_pwm_dimmer->light_fade;
              }
              else {
#endif // USE_PWM_DIMMER_REMOTE
                Settings.light_fade ^= 1;
                dgr_value = Settings.light_fade;
#ifdef USE_PWM_DIMMER_REMOTE
              }
#endif // USE_PWM_DIMMER_REMOTE
              break;
          }
          button_press_count[down_button_index] = 0;

          switch (button_press_count[up_button_index]) {
            case 1:
              // Toggle the LED timeout.
#ifdef USE_PWM_DIMMER_REMOTE
              if (active_device_is_local) {
#endif // USE_PWM_DIMMER_REMOTE
                Settings.flag4.led_timeout ^= 1;
                if (relay_is_on) PWMDimmerSetBrightnessLeds(Settings.flag4.led_timeout ? -1 : 0);
#ifdef USE_PWM_DIMMER_REMOTE
              }
#endif // USE_PWM_DIMMER_REMOTE
              break;
          }
          button_press_count[up_button_index] = 0;
        }
      }

      // If we're not ignoring the power button until it's released, toggle the power.
      else {
#ifdef USE_PWM_DIMMER_REMOTE
        if (!active_device_is_local)
          active_remote_pwm_dimmer->bri = active_remote_pwm_dimmer->bri_power_on;
        else
#endif  // USE_PWM_DIMMER_REMOTE
          target_bri = Settings.bri_power_on;
        toggle_power = true;
      }
    }

    // If this is about the up or down buttons, ...
    else {

      // If the button was held, ...
      if (button_was_held) {
        button_was_held = false;

        // If the button was not tapped before it was held, we changed the brightness and sent
        // updates with the more-to-come message type. Send a final update.
        if (button_press_count[down_button_index] == 0 && button_press_count[up_button_index == 0]) {
          dgr_item = DGR_ITEM_LIGHT_BRI;
#ifdef USE_PWM_DIMMER_REMOTE
          if (!active_device_is_local)
            dgr_value = active_remote_pwm_dimmer->bri;
          else
#endif  // USE_PWM_DIMMER_REMOTE
            dgr_value = target_bri;
        }

        // Reset the the button press count.
        button_press_count[button_index] = 0;
      }

      // If the power was off and we turned it on with the down or up button, ignore it's release.
      if (ignore_up_down_button_release) {
        ignore_up_down_button_release = false;
      }

      // If the button was tapped (pressed and released quickly) and the power is on, increment the
      // count of how many times this button has been pressed.
      else if (button_hold_time[button_index] >= now) {
        button_press_count[button_index]++;
      }
    }

    // Flag the button as released.
    button_pressed[button_index] = false;
    buttons_pressed--;
    button_press_count_timeout = now + 500;
  }

  // If 
  else if (buttons_pressed == 0 && button_press_count_timeout && button_press_count_timeout < now) {
    button_press_count[0] = button_press_count[1] = button_press_count[2] = 0;
  }

  if (toggle_power) {
    power_t new_power;
    uint8_t new_bri;
#ifdef USE_DEVICE_GROUPS
#ifdef USE_PWM_DIMMER_REMOTE
    if (!active_device_is_local) {
      active_remote_pwm_dimmer->power ^= 1;
      new_power = active_remote_pwm_dimmer->power;
      new_bri = active_remote_pwm_dimmer->bri;
    }
    else {
#endif  // USE_PWM_DIMMER_REMOTE
      new_power = power ^ 1;
      new_bri = target_bri;
#ifdef USE_PWM_DIMMER_REMOTE
    }
#endif  // USE_PWM_DIMMER_REMOTE
    if (new_power)
      SendDeviceGroupMessage(power_button_index, DGR_MSGTYP_UPDATE, DGR_ITEM_LIGHT_BRI, new_bri, DGR_ITEM_POWER, new_power);
    else
      SendDeviceGroupMessage(power_button_index, DGR_MSGTYP_UPDATE, DGR_ITEM_POWER, new_power);
#endif  //0 USE_DEVICE_GROUPS

#ifdef USE_PWM_DIMMER_REMOTE
    // The target brightness has already been set. Execute the toggle power command with a source of
    // SRC_RETRY. This will turn the power on using the current target brightness (see
    // FUNC_SET_DEVICE_POWER below).
    if (active_device_is_local)
#endif  // USE_PWM_DIMMER_REMOTE
      ExecuteCommandPower(1, POWER_TOGGLE, SRC_RETRY);
  }

  // If we're not toggling the power and we made changes, send a group update.
  else if (dgr_item) {
#ifdef USE_DEVICE_GROUPS
    SendDeviceGroupMessage(power_button_index, DGR_MSGTYP_UPDATE_DIRECT, dgr_item, dgr_value);
#endif  // USE_DEVICE_GROUPS
    if (Settings.flag3.hass_tele_on_power) {  // SetOption59 - Send tele/%topic%/STATE in addition to stat/%topic%/RESULT
      MqttPublishTeleState();
    }
  }

  if (turn_off_brightness_leds_time && turn_off_brightness_leds_time < millis()) {
    turn_off_brightness_leds_time = 0;
    PWMDimmerSetBrightnessLeds(-1);
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
        if (parm[i] > 0) parm[i]--;
      }
      else {
        value = strtoul(ptr, &ptr, 0);
        if (value < 1 || parm[i] > 255) {
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

void PWMDmmerCmndDimmer(void)
{
  uint8_t dimmer = changeUIntScale(target_bri, 0, 255, 0, 100);
  if (1 == XdrvMailbox.data_len) {
    if ('+' == XdrvMailbox.data[0])
      XdrvMailbox.payload = (dimmer > 89) ? 100 : dimmer + 10;
    else if ('-' == XdrvMailbox.data[0]) {
      XdrvMailbox.payload = (dimmer < 11) ? 1 : dimmer - 10;
    }
  }

  if (XdrvMailbox.payload >= 0 && XdrvMailbox.payload <= 100) {
    uint8_t bri = changeUIntScale(XdrvMailbox.payload, 0, 100, 0, 255);
#ifdef USE_DEVICE_GROUPS
    SendLocalDeviceGroupMessage(DGR_MSGTYP_UPDATE, DGR_ITEM_LIGHT_BRI, bri);
#endif  // USE_DEVICE_GROUPS
    PWMDimmerSetBri(bri);
  }

  ResponseCmndNumber(Settings.light_dimmer);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv35(uint8_t function)
{
  bool result = false;

  if (PWM_DIMMER != my_module_type) return result;

  switch (function) {
    case FUNC_LOOP:
      if (current_bri != target_bri) PWMDimmerAnimate(false);
      break;

    case FUNC_EVERY_SECOND:
      // Turn off the brightness LED's if it's time.
      if (led_timeout_time && led_timeout_time < millis()) {
        for (uint32_t index = 0; index < leds_present; index++) SetLedPowerIdx(index, 0);
        led_timeout_time = 0;
      }

      // The powered-off LED is also the LedLink LED. If we lose the WiFi or MQTT server
      // connection, the LED will be set to a blinking state and will be turned off when the
      // connection is restored. If the state is blinking now, set a flag so we know that we need
      // to restore it when it stops blinking.
      if (global_state.data)
        restore_powered_off_led = 5;
      else if (restore_powered_off_led) {
        PWMDimmerSetPoweredOffLed();
        restore_powered_off_led--;
      }
      break;

    case FUNC_BUTTON_PRESSED:
      // Handle the button press/release instead of the button handler.
      PWMDimmerHandleButton();
      result = true;
      break;

#ifdef USE_DEVICE_GROUPS
    case FUNC_DEVICE_GROUP_ITEM:
      PWMDimmerHandleDeviceGroupItem();
      break;
#endif  // USE_DEVICE_GROUPS

    case FUNC_SET_DEVICE_POWER:
      // Handle turning the power on/off here so we can fade off. Set the target brightness and let
      // Animate handle turning the power off/on. If the source is SRC_RETRY, we're turning the
      // power on with a brightness preset and the target brightness has already been set.
      if (!XdrvMailbox.index)
        target_bri = 0;
      else if (SRC_RETRY != XdrvMailbox.payload) {
        target_bri = Settings.bri_power_on;
        Settings.light_dimmer = changeUIntScale(target_bri, 0, 255, 0, 100);
      }
      result = true;
      break;

    case FUNC_COMMAND:
      result = DecodeCommand(kPWMDimmerCommands, PWMDimmerCommand);
      break;

    case FUNC_INIT:
      PWMDimmerInit();
      break;

    case FUNC_PRE_INIT:
#ifdef USE_PWM_DIMMER_REMOTE
      // If remote device mode is enabled, set the device group count to the number of buttons
      // present.
      if (Settings.flag4.remote_device_mode) {
        for (uint32_t button_index = 0; button_index < MAX_KEYS; button_index++) {
          if (pin[GPIO_KEY1 + button_index] < 99) device_group_count++;
        }
      }
#endif  // USE_PWM_DIMMER_REMOTE
      break;

    case FUNC_MODULE_INIT:
      PWMModuleInit();
      result = true;
      break;
  }
  return result;
}

#endif  // USE_PWM_DIMMER