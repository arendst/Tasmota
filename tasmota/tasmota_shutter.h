/*
  xdrv_27_Shutter[i].ino - Shutter/Blind support for Tasmota

  Copyright (C) 2021  Stefan Bode

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
#ifndef _TASMOTA_SHUTTER_H_
#define _TASMOTA_SHUTTER_H_

#ifdef USE_SHUTTER
// moved here struct SHUTTER to be used outside of xdrv_27_shutter.ino too
struct SHUTTER {
  uint32_t time;               // operating time of the shutter in 0.05sec
  int32_t  open_max;           // max value on maximum open calculated
  int32_t  target_position;    // position to go to
  int32_t  start_position;     // position before a movement is started. init at start
  int32_t  real_position;      // value between 0 and Shutter[i].open_max
  uint16_t open_time;          // duration to open the Shutter[i]. 112 = 11.2sec
  uint16_t close_time;         // duration to close the Shutter[i]. 112 = 11.2sec
  uint16_t close_velocity;     // in relation to open velocity. higher value = faster
  int8_t   direction;          // 1 == UP , 0 == stop; -1 == down
  int8_t   lastdirection;      // last direction (1 == UP , -1 == down)
  uint8_t  switch_mode;        // how to switch relays: SHT_SWITCH, SHT_PULSE
  int8_t   motordelay;         // initial motorstarttime in 0.05sec. Also uses for ramp at steppers and servos, negative if motor stops late
  int16_t  pwm_velocity;       // frequency of PWN for stepper motors or PWM duty cycle change for PWM servo
  uint16_t pwm_value;          // dutyload of PWM 0..1023 on ESP8266
  uint16_t close_velocity_max; // maximum of PWM change during closeing. Defines velocity on opening. Steppers and Servos only
  int32_t  accelerator;        // speed of ramp-up, ramp down of shutters with velocity control. Steppers and Servos only
  int8_t   tilt_config[5];     // tilt_min, tilt_max, duration, tilt_closed_value, tilt_opened_value
  int8_t  tilt_real_pos;       // -90 to 90
  int16_t  tilt_target_pos;
  int16_t  tilt_start_pos;
  uint8_t  tilt_velocity;      // degree rotation per step 0.05sec
  uint16_t venetian_delay = 0;  // Delay in steps before venetian shutter start physical moving. Based on tilt position
} Shutter[MAX_SHUTTERS];

#endif //USE_SHUTTER

#endif //_TASMOTA_SHUTTER_H_