/*
  support_pwm.ino - command support for Tasmota

  Copyright (C) 2021  Theo Arends & Stephan Hadinger

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


/***********************************************************************\
 * PWM Control for ESP32
\***********************************************************************/
#ifdef ESP32

// All changes in PWM have been applied, rearm all change indicators
void PwmRearmChanges(void) {
  for (uint32_t i = 0; i < MAX_PWMS; i++) {
    // Init expected changes
    TasmotaGlobal.pwm_value[i] = -1;          // no change wanted
    TasmotaGlobal.pwm_phase[i] = -1;          // no change wanted
  }
}

// Load PWM values from settings and intiliaze values
// void PwmLoadFromSettings(void) {
//   for (uint32_t i = 0; i < MAX_PWMS; i++) {
//     if (i < MAX_PWMS_LEGACY) {
//       TasmotaGlobal.pwm_cur_value[i] = Settings->pwm_value[i];    // retrieve in Legacy pool for 0..4
//     } else {
//       TasmotaGlobal.pwm_cur_value[i] = Settings->pwm_value_ext[i - MAX_PWMS_LEGACY];    // retrieve in Legacy pool for 5..15
//     }
//     TasmotaGlobal.pwm_cur_phase[i] = 0;       // no phase shift for now, will be recomputed at first push to GPIOs
//   }
//   PwmRearmChanges();    // reset expected changes
// }

// Copy current values to Settings
void PwmSaveToSettings(void) {
  for (uint32_t i = 0; i < MAX_PWMS; i++) {
    if (i < MAX_PWMS_LEGACY) {
      Settings->pwm_value[i] = TasmotaGlobal.pwm_cur_value[i];    // store in Legacy pool for 0..4
    } else {
      Settings->pwm_value_ext[i - MAX_PWMS_LEGACY] = TasmotaGlobal.pwm_cur_value[i];    // retrieve in Legacy pool for 5..15
    }
  }
}

/***********************************************************************\
 * PWM Control for ESP32
\***********************************************************************/
// Apply PWM expected values to actual GPIO PWM
// As input, `TasmotaGlobal.pwm_value[]` and `TasmotaGlobal.pwm_phase[]` contain the new expected values
// or `-1` if no change.
// Auto-phasing is recomputed, and changes are applied to GPIO if there is a physical GPIO configured and an actual change needed
//
void PwmApplyGPIO(void) {
  uint32_t pwm_phase_accumulator = 0;     // dephase each PWM channel with the value of the previous

  for (uint32_t i = 0; i < MAX_PWMS; i++) {
    // compute `pwm_val`, the virtual value of PWM (not taking into account inverted)
    uint32_t pwm_val = TasmotaGlobal.pwm_cur_value[i];      // logical value of PWM, 0..1023
    if (TasmotaGlobal.pwm_value[i] >= 0) { pwm_val = TasmotaGlobal.pwm_value[i]; }    // new value explicitly specified
    if (pwm_val > Settings->pwm_range) { pwm_val = Settings->pwm_range; } // prevent overflow

    // gpio_val : actual value of GPIO, taking into account inversion
    uint32_t gpio_val = bitRead(TasmotaGlobal.pwm_inverted, i) ? Settings->pwm_range - pwm_val : pwm_val;

    // compute phase
    uint32_t pwm_phase = TasmotaGlobal.pwm_cur_phase[i];    // pwm_phase is the logical phase of the active pulse, ignoring inverted
    uint32_t gpio_phase = pwm_phase;          // gpio is the physical phase taking into account inverted
    if (TasmotaGlobal.pwm_phase[i] >= 0) {
      pwm_phase = TasmotaGlobal.pwm_phase[i]; // if explicit set explicitly, 
    } else if (Settings->flag5.pwm_force_same_phase) {
      pwm_phase = 0;                          // if auto-phase is off
    } else {
      // compute auto-phase
      pwm_phase = pwm_phase_accumulator;
      uint32_t pwm_phase_invert = bitRead(TasmotaGlobal.pwm_inverted, i) ? pwm_val : 0;   // move phase if inverted
      gpio_phase = (pwm_phase + pwm_phase_invert) & Settings->pwm_range;
      // accumulate phase for next GPIO
      pwm_phase_accumulator = (pwm_phase + pwm_val) & Settings->pwm_range;
    }

    // apply new values to GPIO if GPIO is set
    if (PinUsed(GPIO_PWM1, i)) {
      if ((pwm_val != TasmotaGlobal.pwm_cur_value[i]) || (pwm_phase != TasmotaGlobal.pwm_cur_phase[i])) {
        // GPIO has PWM and there is a chnage to apply, apply it
        analogWritePhase(Pin(GPIO_PWM1, i), gpio_val, gpio_phase);
      }
    }

    // set new current values
    TasmotaGlobal.pwm_cur_value[i] = pwm_val;
    TasmotaGlobal.pwm_cur_phase[i] = pwm_phase;
  }
  // AddLog(LOG_LEVEL_INFO, "PWM: Val=%03X-%03X-%03X-%03X-%03X Phase=%03X-%03X-%03X-%03X-%03X Range=%03X",
  //                         TasmotaGlobal.pwm_cur_value[0], TasmotaGlobal.pwm_cur_value[1], TasmotaGlobal.pwm_cur_value[2], TasmotaGlobal.pwm_cur_value[3],
  //                         TasmotaGlobal.pwm_cur_value[4],
  //                         TasmotaGlobal.pwm_cur_phase[0], TasmotaGlobal.pwm_cur_phase[1], TasmotaGlobal.pwm_cur_phase[2], TasmotaGlobal.pwm_cur_phase[3],
  //                         TasmotaGlobal.pwm_cur_phase[4],
  //                         Settings->pwm_range
  //                         );
  PwmSaveToSettings();    // copy to Settings
  PwmRearmChanges();      // reset expected changes
}

void CmndPwm(void)
{
  if (TasmotaGlobal.pwm_present && (XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_PWMS)) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= Settings->pwm_range) && PinUsed(GPIO_PWM1, XdrvMailbox.index -1)) {
      TasmotaGlobal.pwm_value[XdrvMailbox.index - 1] = XdrvMailbox.payload;
      PwmApplyGPIO();
    }
    Response_P(PSTR("{"));
    MqttShowPWMState();  // Render the PWM status to MQTT
    ResponseJsonEnd();
  }
}

void GpioInitPwm(void) {
  PwmRearmChanges();

  for (uint32_t i = 0; i < MAX_PWMS; i++) {     // Basic PWM control only
    if (PinUsed(GPIO_PWM1, i)) {
      analogAttach(Pin(GPIO_PWM1, i));
      if (i < TasmotaGlobal.light_type) {
        // force PWM GPIOs to black
        TasmotaGlobal.pwm_value[i] = 0;
      } else {
        TasmotaGlobal.pwm_present = true;
        if (i < MAX_PWMS_LEGACY) {
          TasmotaGlobal.pwm_value[i] = Settings->pwm_value[i];
        } else {
          TasmotaGlobal.pwm_value[i] = Settings->pwm_value_ext[i - MAX_PWMS_LEGACY];
        }
      }
    }
  }
  PwmApplyGPIO();   // apply all changes
}

/********************************************************************************************/

void ResetPwm(void)
{
  for (uint32_t i = 0; i < MAX_PWMS; i++) {     // Basic PWM control only
    TasmotaGlobal.pwm_value[i] = 0;
  }
  PwmApplyGPIO();
}

#else // now for ESP8266

void PwmRearmChanges(void) {}

void CmndPwm(void)
{
  if (TasmotaGlobal.pwm_present && (XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_PWMS)) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= Settings->pwm_range) && PinUsed(GPIO_PWM1, XdrvMailbox.index -1)) {
      uint32_t pwm_index = XdrvMailbox.index - 1;
      if (pwm_index < MAX_PWMS_LEGACY) {                        // write in the appropriate settings pool
        Settings->pwm_value[pwm_index] = XdrvMailbox.payload;
      } else {
        Settings->pwm_value_ext[pwm_index - MAX_PWMS_LEGACY] = XdrvMailbox.payload;
      }
      analogWrite(Pin(GPIO_PWM1, pwm_index), bitRead(TasmotaGlobal.pwm_inverted, pwm_index) ? Settings->pwm_range - XdrvMailbox.payload : XdrvMailbox.payload);
    }
    Response_P(PSTR("{"));
    MqttShowPWMState();  // Render the PWM status to MQTT
    ResponseJsonEnd();
  }
}

void GpioInitPwm(void) {
  for (uint32_t i = 0; i < MAX_PWMS; i++) {     // Basic PWM control only
    if (PinUsed(GPIO_PWM1, i)) {
      pinMode(Pin(GPIO_PWM1, i), OUTPUT);
      if (i < TasmotaGlobal.light_type) {
        // force PWM GPIOs to low or high mode if belongs to the light (always <5), see #7165
        analogWrite(Pin(GPIO_PWM1, i), bitRead(TasmotaGlobal.pwm_inverted, i) ? Settings->pwm_range : 0);
      } else {
        TasmotaGlobal.pwm_present = true;
        if (i < MAX_PWMS_LEGACY) {
          analogWrite(Pin(GPIO_PWM1, i), bitRead(TasmotaGlobal.pwm_inverted, i) ? Settings->pwm_range - Settings->pwm_value[i] : Settings->pwm_value[i]);
        } else {
          analogWrite(Pin(GPIO_PWM1, i), bitRead(TasmotaGlobal.pwm_inverted, i) ? Settings->pwm_range - Settings->pwm_value_ext[i] : Settings->pwm_value_ext[i]);
        }
      }
    }
  }
}

/********************************************************************************************/

void ResetPwm(void)
{
  for (uint32_t i = 0; i < MAX_PWMS; i++) {     // Basic PWM control only
    if (PinUsed(GPIO_PWM1, i)) {
      analogWrite(Pin(GPIO_PWM1, i), bitRead(TasmotaGlobal.pwm_inverted, i) ? Settings->pwm_range : 0);
//      analogWrite(Pin(GPIO_PWM1, i), bitRead(TasmotaGlobal.pwm_inverted, i) ? Settings->pwm_range - Settings->pwm_value[i] : Settings->pwm_value[i]);
    }
  }
}

#endif // ESP8266

void CmndPwmfrequency(void)
{
  if ((1 == XdrvMailbox.payload) || ((XdrvMailbox.payload >= PWM_MIN) && (XdrvMailbox.payload <= PWM_MAX))) {
    Settings->pwm_frequency = (1 == XdrvMailbox.payload) ? PWM_FREQ : XdrvMailbox.payload;
    analogWriteFreq(Settings->pwm_frequency);   // Default is 1000 (core_esp8266_wiring_pwm.c)
#ifdef USE_LIGHT
    LightReapplyColor();
    LightAnimate();
#endif // USE_LIGHT
  }
  ResponseCmndNumber(Settings->pwm_frequency);
}

void CmndPwmrange(void) {
  // Support only 8 (=255), 9 (=511) and 10 (=1023) bits resolution
  if ((1 == XdrvMailbox.payload) || ((XdrvMailbox.payload > 254) && (XdrvMailbox.payload < 1024))) {
    uint32_t pwm_range = XdrvMailbox.payload;
    uint32_t pwm_resolution = 0;
    while (pwm_range) {
      pwm_resolution++;
      pwm_range >>= 1;
    }
    pwm_range = (1 << pwm_resolution) - 1;
    uint32_t old_pwm_range = Settings->pwm_range;
    Settings->pwm_range = (1 == XdrvMailbox.payload) ? PWM_RANGE : pwm_range;
    for (uint32_t i = 0; i < MAX_PWMS; i++) {
      if (i < MAX_PWMS_LEGACY) {
        if (Settings->pwm_value[i] > Settings->pwm_range) {
          Settings->pwm_value[i] = Settings->pwm_range;
        }
      } else {
        if (Settings->pwm_value_ext[i - MAX_PWMS_LEGACY] > Settings->pwm_range) {
          Settings->pwm_value_ext[i - MAX_PWMS_LEGACY] = Settings->pwm_range;
        }
      }
    }
    if (Settings->pwm_range != old_pwm_range) {  // On ESP32 this prevents loss of duty state
      analogWriteRange(Settings->pwm_range);     // Default is 1023 (Arduino.h)
    }
  }
  ResponseCmndNumber(Settings->pwm_range);
}

void MqttShowPWMState(void)
{
  ResponseAppend_P(PSTR("\"" D_CMND_PWM "\":{"));
  bool first = true;
  for (uint32_t i = 0; i < MAX_PWMS; i++) {   // TODO
    if (PinUsed(GPIO_PWM1, i)) {
      uint32_t pwm_val = (i < MAX_PWMS_LEGACY) ? Settings->pwm_value[i] : Settings->pwm_value_ext[i - MAX_PWMS_LEGACY];
      ResponseAppend_P(PSTR("%s\"" D_CMND_PWM "%d\":%d"), first ? "" : ",", i+1, pwm_val);
      first = false;
    }
  }
  ResponseJsonEnd();
}
