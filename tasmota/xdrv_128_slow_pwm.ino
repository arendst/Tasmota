
/*
  xdrv_128_slow_pwm.ino - Slow PWM

  Copyright (C) 2021  Laszlo Magyar

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

#ifdef USE_SLOW_PWM
#ifndef FIRMWARE_MINIMAL

/*********************************************************************************************\
 * PWM with slower than 1Hz periods
 *
 * Slow PWM is good for eg. heating elements, thermoelectric valve actuators that are connected
 * to mains AC voltage, where you don't need to use phase angle dimmers, where you can use
 * simple SSRs (Solid State Relays), or even regular relays.
 *
 * Slow PWM works as plain PWM and also as brightness only Light (SetOption15 0 and
 * SetOption15 1), as Light it has built in HA support by default.
 *
 * Current implementation:
 * - can be turned on only by build flag in user_config_override.h: #define USE_SLOW_PWM
 * - adds new command SlowPwmPeriod, and adds new setting slow_pwm_period
 * - replaces Pwm<x> command functionality, extends PwmRange command functionality
 * - replaces Pwm<x> pin behaviour (normal Pwm and Light, SetOption15 0 or 1)
 * - does not modify other PWM related behavior (eg. status led, buzzer, shutter, pwm dimmer,
 *   backlight, etc.) - only pwm status led is tested on ESP8266
 *
 * Tested with:
 * - ESP8266 - NodeMcu v2: D1, D2, D5, D6, D7 pins (GPIO 5, 4, 14, 12, 13)
 * - ESP32 - Wemos D1 Mini ESP32: D1, D2, D5, D6, D7 pins (GPIO 22, 21, 18, 19, 23)
 *
 * How to use it:
 * - rebuild with #define USE_SLOW_PWM in user_config_override.h
 * - upload new firmware
 * - Important: in case of ESP32, if PWM was set up on any pin before new firmware is uploaded,
 *   power cycle the board, because ESP32 has hardware PWM, and power cycling the device will
 *   detach pins from hardware PWM (that were previously attached to hardware PWM by previous
 *   firmware), and Slow PWM can use them as digital IO pins
 * - use Generic(18) or ESP32-DevKit(1) module type
 * - set GPIO pins as PWM<x> or PWM_i<x>
 * - use SlowPwmPeriod <x> command to set slow PWM cycle time
 *   Note: Slow PWM's time resolution is 50ms, setting period times lower than 5s will decrase
 *   even Dimmer 1..100 resolution. Use low values only for test purposes.
 * - from now on, there are 3 scenarios:
 *   - as plain PWM output:
 *     - execute: SetOption15 0
 *     - use PWM<x> commands to set duty cycle
 *     - use PwmRange <x>, SlowPwmPeriod <x> commands to change parameters
 *   - as Light, only with 1 PWM pin (this has built in HA support also, as normal Light entity)
 *     see: https://tasmota.github.io/docs/Lights/#1-channel-dimmable-light
 *     - execute: LedTable 0  - or recompile with Settings->light_correction = 0;
 *     - use Power<x>, Dimmer<x> commands to set duty cycle
 *     - use PwmRange <x>, SlowPwmPeriod <x> commands to change parameters
 *       Note: restart device when PwmRange changed, Light updates range only when reinitialized
 *   - as Light, but 1..5 PWM pins (this has built in HA support also, as normal Light entity)
 *     see: https://tasmota.github.io/docs/Lights/#independent-pwm-channels
 *     - execute: LedTable 0  - or recompile with Settings->light_correction = 0;
 *     - execute: MultiPwm 1 (previously SetOption68 1)  - or recompile with #define LIGHT_CHANNEL_MODE true
 *     - use Power<x>, Channel<x> commands to set duty cycle
 *     - use PwmRange <x>, SlowPwmPeriod <x> commands to change parameters
 *       Note: restart device when PwmRange changed, Light updates range only when reinitialized
 *
 * I'm aware of https://tasmota.github.io/docs/Time-Proportioned-Output-support/
 * - Timeprop has more features but no user interface. Slow PWM has less functions but has user
 *   interface and built in HA support as Light entity.
 * - main Timeprop features that are missing in Slow PWM:
 *   - safety mechanism to revert to a specified fallback value when MQTT communication is lost:
 *     - can be implemented with Tasmota rules
 *   - actuator dead time:
 *     - with other control specific features I think it has better place in the main control
 *       logic in HA or other controller software
 *   - different cycle times per pin:
 *     - that could be useful
 *
 * Further development:
 * - v0: as is now, it remains a build option, only hard core users will use it
 *   (though we should rename it as xdrv_57 instead of xdrv_128)
 * - v1: enable it with a new SetOption, still all PWM pin behavior will be changed when turned on
 * - v2: create new pin config, like "SPWM<x>", where only pins defined as Slow PWM will be
 *   affected with this behavior
 * - v3: extend scripting (Berry?) with the implemented v1/v2 solution
 *
\*********************************************************************************************/

#define XDRV_128             128

const char kSlowPwmCommands[] PROGMEM = "|"  // No prefix
  D_CMND_SLOWPWMPERIOD
  ;

void (* const SlowPwmCommand[])(void) PROGMEM = {
  &CmndSlowPwmPeriod
  };

/*********************************************************************************************\
 * Implementation
\*********************************************************************************************/

class SlowPWM {
  private:

    uint32_t period_time;
    uint32_t period_start_time;

    struct {
      uint8_t pin_number;
      uint8_t current_digital_value;
      uint16_t pwm_value;
      uint32_t duty_time;
    } pins[MAX_PWMS];

    uint8_t pinCount;

    uint32_t SettingsSlowPwmPeriodTime(void) {
      return (((Settings->slow_pwm_period >= SLOW_PWM_MIN) && (Settings->slow_pwm_period <= SLOW_PWM_MAX)) ? Settings->slow_pwm_period : SLOW_PWM_PERIOD) * 1000;
    }

    uint16_t SettingsPwmRange(void) {
      return Settings->pwm_range;
    }

  public:

    void AnalogWrite(uint8_t pin_number, uint16_t pwm_value) {
      int i;
      // search pin in pins
      for (i = 0; i < pinCount && pins[i].pin_number != pin_number; i++);
      // if new pin
      if (i == pinCount) {
        // if no more place
        if (pinCount == sizeof(pins) / sizeof(pins[0])) return;
        // if first pin used, start time calculation in the loop
        if (pinCount == 0) {
          period_time = SettingsSlowPwmPeriodTime();
          period_start_time = millis();
        }
        // add new pin
        pinCount++;
        pins[i].pin_number = pin_number;
      }
      // set pin pwm parameters
      pins[i].current_digital_value = -1;
      pins[i].pwm_value = pwm_value;
      pins[i].duty_time = period_time * pwm_value / SettingsPwmRange();
    }

    void PeriodChanged(void) {
      period_time = SettingsSlowPwmPeriodTime();
      period_start_time = millis();
      for (int i = 0; i < pinCount; i++) {
        pins[i].duty_time = period_time * pins[i].pwm_value / SettingsPwmRange();
      }
    }

    void RangeChanged(void) {
      for (int i = 0; i < pinCount; i++) {
        if (pins[i].pwm_value > SettingsPwmRange()) {
          pins[i].pwm_value = SettingsPwmRange();
          pins[i].duty_time = period_time;
        } else {
          pins[i].duty_time = period_time * pins[i].pwm_value / SettingsPwmRange();
        }
      }
    }

    void PwmInit(void) {
      pinCount = 0;
    }

    void PwmLoop(void) {
      if (pinCount > 0) {
        uint32_t period_time_passed = millis() - period_start_time;
        if (period_time_passed >= period_time) {
          period_start_time += period_time;
          period_time_passed -= period_time;
        }
        for (int i = 0; i < pinCount; i++) {
          if (period_time_passed < pins[i].duty_time) {
            if (pins[i].current_digital_value != 1) {
              digitalWrite(pins[i].pin_number, 1);
              pins[i].current_digital_value = 1;
            }
          } else {
            if (pins[i].current_digital_value != 0) {
              digitalWrite(pins[i].pin_number, 0);
              pins[i].current_digital_value = 0;
            }
          }
        }
      }
    }
} slow_pwm;

void SlowPWMAnalogWrite(uint8_t pin, uint16_t value) {
  slow_pwm.AnalogWrite(pin, value);
}

void SlowPWMAnalogWritePeriodChanged(void) {
  slow_pwm.PeriodChanged();
}

void SlowPWMAnalogWriteRangeChanged(void) {
  slow_pwm.RangeChanged();
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndSlowPwmPeriod(void)
{
  if ((0 == XdrvMailbox.payload) || ((XdrvMailbox.payload >= SLOW_PWM_MIN) && (XdrvMailbox.payload <= SLOW_PWM_MAX))) {
    Settings->slow_pwm_period = (0 == XdrvMailbox.payload) ? SLOW_PWM_PERIOD : XdrvMailbox.payload;
    SlowPWMAnalogWritePeriodChanged();
  }
  ResponseCmndNumber(Settings->slow_pwm_period);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv128(uint8_t function) {
  bool result = false;

  switch (function) {
    case FUNC_MODULE_INIT:
      slow_pwm.PwmInit();
      break;
    case FUNC_EVERY_50_MSECOND:
      slow_pwm.PwmLoop();
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kSlowPwmCommands, SlowPwmCommand);
      break;
  }

  return result;
}

#endif  // FIRMWARE_MINIMAL
#endif  // USE_SLOW_PWM
