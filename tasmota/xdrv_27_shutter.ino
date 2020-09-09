/*
  xdrv_27_shutter.ino - Shutter/Blind support for Tasmota

  Copyright (C) 2020  Stefan Bode

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

#ifdef USE_SHUTTER
/*********************************************************************************************\
 * Shutter or Blind support using two consecutive relays
\*********************************************************************************************/

#define XDRV_27            27
#ifndef SHUTTER_STEPPER
  #define SHUTTER_STEPPER
#endif

#define D_SHUTTER "SHUTTER"

const uint16_t MOTOR_STOP_TIME = 500;   // in mS
const uint16_t RESOLUTION = 1000;       // incresed to 1000 in 8.5 to ramp servos
const uint8_t STEPS_PER_SECOND = 20;    // FUNC_EVERY_50_MSECOND
const uint16_t pwm_max = 500;
const uint16_t pwm_min = 90;

uint8_t calibrate_pos[6] = {0,30,50,70,90,100};
uint16_t messwerte[5] = {30,50,70,90,100};

int32_t  velocity_max = 0;
int32_t  velocity_change_per_step_max = 0;
int32_t  min_runtime_ms = 0;
int32_t  current_stop_way = 0;
int32_t  next_possible_stop_position = 0;
int32_t  toBeAcc = 0;


const uint8_t MAX_MODES = 7;
enum Shutterposition_mode {SHT_UNDEF, SHT_TIME, SHT_TIME_UP_DOWN, SHT_TIME_GARAGE, SHT_COUNTER, SHT_PWM_VALUE, SHT_PWM_TIME,};
enum Shutterswitch_mode {SHT_SWITCH, SHT_PULSE,};
enum ShutterButtonStates { SHT_NOT_PRESSED, SHT_PRESSED_MULTI, SHT_PRESSED_HOLD, SHT_PRESSED_IMMEDIATE, SHT_PRESSED_EXT_HOLD, SHT_PRESSED_MULTI_SIMULTANEOUS, SHT_PRESSED_HOLD_SIMULTANEOUS, SHT_PRESSED_EXT_HOLD_SIMULTANEOUS,};

const char kShutterCommands[] PROGMEM = D_PRFX_SHUTTER "|"
  D_CMND_SHUTTER_OPEN "|" D_CMND_SHUTTER_CLOSE "|" D_CMND_SHUTTER_TOGGLE "|" D_CMND_SHUTTER_TOGGLEDIR "|" D_CMND_SHUTTER_STOP "|" D_CMND_SHUTTER_POSITION "|"
  D_CMND_SHUTTER_OPENTIME "|" D_CMND_SHUTTER_CLOSETIME "|" D_CMND_SHUTTER_RELAY "|" D_CMND_SHUTTER_MODE "|"
  D_CMND_SHUTTER_SETHALFWAY "|" D_CMND_SHUTTER_SETCLOSE "|" D_CMND_SHUTTER_SETOPEN "|" D_CMND_SHUTTER_INVERT "|" D_CMND_SHUTTER_CLIBRATION "|"
  D_CMND_SHUTTER_MOTORDELAY "|" D_CMND_SHUTTER_FREQUENCY "|" D_CMND_SHUTTER_BUTTON "|" D_CMND_SHUTTER_LOCK "|" D_CMND_SHUTTER_ENABLEENDSTOPTIME "|" D_CMND_SHUTTER_INVERTWEBBUTTONS "|"
  D_CMND_SHUTTER_STOPOPEN "|" D_CMND_SHUTTER_STOPCLOSE "|" D_CMND_SHUTTER_STOPTOGGLE "|" D_CMND_SHUTTER_STOPTOGGLEDIR "|" D_CMND_SHUTTER_STOPPOSITION;

void (* const ShutterCommand[])(void) PROGMEM = {
  &CmndShutterOpen, &CmndShutterClose, &CmndShutterToggle, &CmndShutterToggleDir, &CmndShutterStop, &CmndShutterPosition,
  &CmndShutterOpenTime, &CmndShutterCloseTime, &CmndShutterRelay, &CmndShutterMode,
  &CmndShutterSetHalfway, &CmndShutterSetClose, &CmndShutterSetOpen, &CmndShutterInvert, &CmndShutterCalibration , &CmndShutterMotorDelay,
  &CmndShutterFrequency, &CmndShutterButton, &CmndShutterLock, &CmndShutterEnableEndStopTime, &CmndShutterInvertWebButtons,
  &CmndShutterStopOpen, &CmndShutterStopClose, &CmndShutterStopToggle, &CmndShutterStopToggleDir, &CmndShutterStopPosition};

  const char JSON_SHUTTER_POS[] PROGMEM = "\"" D_PRFX_SHUTTER "%d\":{\"Position\":%d,\"Direction\":%d,\"Target\":%d}";
  const char JSON_SHUTTER_BUTTON[] PROGMEM = "\"" D_PRFX_SHUTTER "%d\":{\"Button%d\":%d}";

#include <Ticker.h>

Ticker TickerShutter;

struct SHUTTER {
  power_t RelayShutterMask = 0;              // bit mask with 11 at the position of relays that belong to at least ONE shutter
  power_t RelayOldMask = 0;                  // bitmatrix that contain the last known state of all relays. Required to detemine the manual changed relay.
  power_t RelayCurrentMask = 0;              // bitmatrix that contain the current state of all relays
  uint32_t time[MAX_SHUTTERS];               // operating time of the shutter in 0.05sec
  int32_t open_max[MAX_SHUTTERS];            // max value on maximum open calculated
  int32_t target_position[MAX_SHUTTERS];     // position to go to
  int32_t start_position[MAX_SHUTTERS];      // position before a movement is started. init at start
  int32_t real_position[MAX_SHUTTERS];       // value between 0 and Shutter.open_max
  uint16_t open_time[MAX_SHUTTERS];          // duration to open the shutter. 112 = 11.2sec
  uint16_t close_time[MAX_SHUTTERS];         // duration to close the shutter. 112 = 11.2sec
  uint16_t close_velocity[MAX_SHUTTERS];     // in relation to open velocity. higher value = faster
  int8_t  direction[MAX_SHUTTERS];           // 1 == UP , 0 == stop; -1 == down
  int8_t  lastdirection[MAX_SHUTTERS];       // last direction (1 == UP , -1 == down)
  uint8_t position_mode=0;                   // how to calculate actual position: SHT_TIME, SHT_COUNTER, SHT_PWM_VALUE, SHT_PWM_TIME
  uint8_t switch_mode[MAX_SHUTTERS];         // how to switch relays: SHT_SWITCH, SHT_PULSE
  int16_t motordelay[MAX_SHUTTERS];          // initial motorstarttime in 0.05sec. Also uses for ramp at steppers and servos
  int16_t pwm_velocity[MAX_SHUTTERS];        // frequency of PWN for stepper motors or PWM duty cycle change for PWM servo
  uint16_t pwm_value[MAX_SHUTTERS];          // dutyload of PWM 0..1023 on ESP8266
  uint16_t pwm_min[MAX_SHUTTERS];            // dutyload of PWM 0..1023 on ESP8266
  uint16_t pwm_max[MAX_SHUTTERS];            // dutyload of PWM 0..1023 on ESP8266
  uint16_t open_velocity_max = 1000;         // maximum of PWM change during opening. Defines velocity on opening. Steppers and Servos only
  uint16_t close_velocity_max[MAX_SHUTTERS]; // maximum of PWM change during closeing. Defines velocity on opening. Steppers and Servos only
  uint8_t skip_relay_change;                 // avoid overrun at endstops
  int32_t accelerator[MAX_SHUTTERS];         // speed of ramp-up, ramp down of shutters with velocity control. Steppers and Servos only
  uint8_t start_reported = 0;                // indicates of the shutter start was reported through MQTT JSON
} Shutter;

#define SHT_DIV_ROUND(__A, __B) (((__A) + (__B)/2) / (__B))

void ShutterLogPos(uint32_t i)
{
  char stemp2[10];
  dtostrfd((float)Shutter.time[i] / STEPS_PER_SECOND, 2, stemp2);
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHT: Shutter %d Real %d, Start %d, Stop %d, Dir %d, Delay %d, Rtc %s [s], Freq %d, PWM %d"),
    i+1, Shutter.real_position[i], Shutter.start_position[i], Shutter.target_position[i], Shutter.direction[i], Shutter.motordelay[i], stemp2, Shutter.pwm_velocity[i], Shutter.pwm_value[i]);
}

void ExecuteCommandPowerShutter(uint32_t device, uint32_t state, uint32_t source)
{
  // first implementation for virtual relays. Avoid switching relay numbers that do not exist.
  if (device <= devices_present) ExecuteCommandPower(device,state,source);
}

void ShutterUpdateVelocity(uint8_t i)
{
  // No Logging allowed. Part of RTC Timer
  // will be calles through RTC every 50ms.
  Shutter.pwm_velocity[i] += Shutter.accelerator[i];
  Shutter.pwm_velocity[i] = tmax(0,tmin(Shutter.direction[i]==1 ? Shutter.open_velocity_max : Shutter.close_velocity_max[i],Shutter.pwm_velocity[i]));
}

void ShutterRtc50mS(void)
{
    // No Logging allowed. RTC Timer
  for (uint8_t i = 0; i < shutters_present; i++) {
    if (Shutter.direction[i]) {
      // update position data before increasing counter
      Shutter.real_position[i] =  ShutterCalculatePosition(i);
      Shutter.time[i]++;
      ShutterCalculateAccelerator(i);
      switch (Shutter.position_mode) {
        case SHT_PWM_VALUE:
          ShutterUpdateVelocity(i);
          Shutter.real_position[i] +=  Shutter.direction[i] > 0 ? Shutter.pwm_velocity[i] : (Shutter.direction[i] < 0 ? -Shutter.pwm_velocity[i] : 0);
          Shutter.pwm_value[i] = SHT_DIV_ROUND((Shutter.pwm_max[i]-Shutter.pwm_min[i]) * Shutter.real_position[i] , Shutter.open_max[i])+Shutter.pwm_min[i];
          analogWrite(Pin(GPIO_PWM1, i), Shutter.pwm_value[i]);
        break;

        case SHT_COUNTER:
          if (Shutter.accelerator[i]) {
            //AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: accelerator i=%d ->  %d"),i, Shutter.accelerator[i]);
            ShutterUpdateVelocity(i);
            analogWriteFreq(Shutter.pwm_velocity[i]);
            analogWrite(Pin(GPIO_PWM1, i), 50);
          }
        break;
      }
    } // if (Shutter.direction[i])
  }
}

int32_t ShutterPercentToRealPosition(uint32_t percent, uint32_t index)
{
	if (Settings.shutter_set50percent[index] != 50) {
    return (percent <= 5) ? Settings.shuttercoeff[2][index] * percent : Settings.shuttercoeff[1][index] * percent + Settings.shuttercoeff[0][index];
	} else {
    uint32_t realpos;
    // check against DIV 0
    for (uint32_t j = 0; j < 5; j++) {
      if (0 == Settings.shuttercoeff[j][index]) {
        AddLog_P2(LOG_LEVEL_ERROR, PSTR("SHT: RESET/INIT CALIBRATION MATRIX DIV 0"));
        for (uint32_t k = 0; k < 5; k++) {
          Settings.shuttercoeff[k][index] = SHT_DIV_ROUND(calibrate_pos[k+1] * 1000, calibrate_pos[5]);
        }
      }
    }
    for (uint32_t i = 0; i < 5; i++) {
      if ((percent * 10) >= Settings.shuttercoeff[i][index]) {
        realpos = SHT_DIV_ROUND(Shutter.open_max[index] * calibrate_pos[i+1], 100);
        //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Realposition TEMP1: %d, %% %d, coeff %d"), realpos, percent, Settings.shuttercoeff[i][index]);
      } else {
        if (0 == i) {
          realpos =  SHT_DIV_ROUND(SHT_DIV_ROUND(percent * Shutter.open_max[index] * calibrate_pos[i+1], Settings.shuttercoeff[i][index]), 10);
        } else {
          //uint16_t addon = ( percent*10 - Settings.shuttercoeff[i-1][index] ) * Shutter_Open_Max[index] * (calibrate_pos[i+1] - calibrate_pos[i]) / (Settings.shuttercoeff[i][index] -Settings.shuttercoeff[i-1][index]) / 100;
          //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Realposition TEMP2: %d, %% %d, coeff %d"), addon, (calibrate_pos[i+1] - calibrate_pos[i]), (Settings.shuttercoeff[i][index] -Settings.shuttercoeff[i-1][index]));
          realpos += SHT_DIV_ROUND(SHT_DIV_ROUND((percent*10 - Settings.shuttercoeff[i-1][index] ) * Shutter.open_max[index] * (calibrate_pos[i+1] - calibrate_pos[i]), Settings.shuttercoeff[i][index] - Settings.shuttercoeff[i-1][index]), 100);
        }
        break;
      }
    }
		return realpos;
	}
}

uint8_t ShutterRealToPercentPosition(int32_t realpos, uint32_t index)
{
	if (Settings.shutter_set50percent[index] != 50) {
		return (Settings.shuttercoeff[2][index] * 5 > realpos) ? SHT_DIV_ROUND(realpos, Settings.shuttercoeff[2][index]) : SHT_DIV_ROUND(realpos-Settings.shuttercoeff[0][index], Settings.shuttercoeff[1][index]);
	} else {
    uint16_t realpercent;

    for (uint32_t i = 0; i < 5; i++) {
      if (realpos >= Shutter.open_max[index] * calibrate_pos[i+1] / 100) {
        realpercent = SHT_DIV_ROUND(Settings.shuttercoeff[i][index], 10);
        //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Realpercent TEMP1: %d, %% %d, coeff %d"), realpercent, realpos, Shutter_Open_Max[index] * calibrate_pos[i+1] / 100);
      } else {
        if (0 == i) {
          realpercent  = SHT_DIV_ROUND(SHT_DIV_ROUND((realpos - SHT_DIV_ROUND(Shutter.open_max[index] * calibrate_pos[i], 100)) * 10 * Settings.shuttercoeff[i][index], calibrate_pos[i+1]), Shutter.open_max[index]);
        } else {
          //uint16_t addon = ( realpos - (Shutter_Open_Max[index] * calibrate_pos[i] / 100) ) * 10 * (Settings.shuttercoeff[i][index] - Settings.shuttercoeff[i-1][index]) / (calibrate_pos[i+1] - calibrate_pos[i])/ Shutter_Open_Max[index];
          //uint16_t addon = ( percent*10 - Settings.shuttercoeff[i-1][index] ) * Shutter_Open_Max[index] * (calibrate_pos[i+1] - calibrate_pos[i]) / (Settings.shuttercoeff[i][index] -Settings.shuttercoeff[i-1][index]) / 100;
          //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Realpercent TEMP2: %d, delta %d,  %% %d, coeff %d"), addon,( realpos - (Shutter_Open_Max[index] * calibrate_pos[i] / 100) ) , (calibrate_pos[i+1] - calibrate_pos[i])* Shutter_Open_Max[index]/100, (Settings.shuttercoeff[i][index] -Settings.shuttercoeff[i-1][index]));
          realpercent += SHT_DIV_ROUND(SHT_DIV_ROUND((realpos - SHT_DIV_ROUND(Shutter.open_max[index] * calibrate_pos[i], 100)) * 10 * (Settings.shuttercoeff[i][index] - Settings.shuttercoeff[i-1][index]), (calibrate_pos[i+1] - calibrate_pos[i])), Shutter.open_max[index]) ;
        }
        break;
      }
    }
    return (int16_t)realpercent < 0 ? 0 : realpercent;
  }
}

void ShutterInit(void)
{
  shutters_present = 0;
  Shutter.RelayShutterMask = 0;
  //Initialize to get relay that changed
  Shutter.RelayOldMask = power;


  // if shutter 4 is unused
  if (Settings.shutter_startrelay[MAX_SHUTTERS -1] == 0) {
     Shutter.open_velocity_max = Settings.shuttercoeff[4][3] > 0 ? Settings.shuttercoeff[4][3] : Shutter.open_velocity_max;
  }
  for (uint32_t i = 0; i < MAX_SHUTTERS; i++) {
    // set startrelay to 1 on first init, but only to shutter 1. 90% usecase
    Settings.shutter_startrelay[i] = (Settings.shutter_startrelay[i] == 0 && i ==  0? 1 : Settings.shutter_startrelay[i]);
    if (Settings.shutter_startrelay[i] && (Settings.shutter_startrelay[i] < 9)) {
      shutters_present++;

      // Add the two relays to the mask to knaw they belong to shutters
      Shutter.RelayShutterMask |= 3 << (Settings.shutter_startrelay[i] -1)  ;

      // All shutters must have same mode. Switch OR Pulse. N
      switch (Settings.pulse_timer[i]) {
        case 0:
          Shutter.switch_mode[i] = SHT_SWITCH;
        break;
        default:
          Shutter.switch_mode[i] = SHT_PULSE;
        break;
      }

      if (Settings.shutter_mode == SHT_UNDEF) {
        bool relay_in_interlock = false;
        AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: mode undef.. calculate..."));

        for (uint32_t j = 0; j < MAX_INTERLOCKS * Settings.flag.interlock; j++) {  // CMND_INTERLOCK - Enable/disable interlock
          //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHT: Interlock state i=%d %d, flag %d, , shuttermask %d, maskedIL %d"),i, Settings.interlock[i], Settings.flag.interlock,Shutter.RelayShutterMask, Settings.interlock[i]&Shutter.RelayShutterMask);
          if (Settings.interlock[j] && (Settings.interlock[j] & Shutter.RelayShutterMask)) {
            //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHT: Relay in Interlock group"));
            relay_in_interlock = true;
          }
        }

        switch (Settings.pulse_timer[i+1]) {
          case 0:
            Shutter.position_mode = SHT_TIME_GARAGE;
          break;
          default:
            if (relay_in_interlock) {
              Shutter.position_mode = SHT_TIME;
            } else {
              Shutter.position_mode = SHT_TIME_UP_DOWN;
              if (PinUsed(GPIO_PWM1, i) && PinUsed(GPIO_CNTR1, i)) {
                Shutter.position_mode = SHT_COUNTER;
              }
            }

          break;
        }
      } else {
        Shutter.position_mode = Settings.shutter_mode;
      }

      // main function for stepper and servos to control velocity and acceleration.
      TickerShutter.attach_ms(50, ShutterRtc50mS );

      // default the 50 percent should not have any impact without changing it. set to 60
      Settings.shutter_set50percent[i] = (Settings.shutter_set50percent[i] > 0) ? Settings.shutter_set50percent[i] : 50;

      // use 10 sec. as default to allow everybody to play without deep initialize
      Shutter.open_time[i] = (Settings.shutter_opentime[i] > 0) ? Settings.shutter_opentime[i] : 100;
      Shutter.close_time[i] = (Settings.shutter_closetime[i] > 0) ? Settings.shutter_closetime[i] : 100;

      //temporary hard coded.
      Shutter.pwm_min[i] = pwm_min;
      Shutter.pwm_max[i] = pwm_max;

      // Update Calculation 20 because time interval is 0.05 sec ans time is in 0.1sec
      Shutter.open_max[i] = STEPS_PER_SECOND * RESOLUTION * Shutter.open_time[i] / 10;
      Shutter.close_velocity[i] =  Shutter.open_max[i] / Shutter.close_time[i] / 2 ;

      // calculate a ramp slope at the first 5 percent to compensate that shutters move with down part later than the upper part
      if (Settings.shutter_set50percent[i] != 50) {
      	Settings.shuttercoeff[1][i] = Shutter.open_max[i] * (100 - Settings.shutter_set50percent[i] ) / 5000;
      	Settings.shuttercoeff[0][i] = Shutter.open_max[i] - (Settings.shuttercoeff[1][i] * 100);
      	Settings.shuttercoeff[2][i] = (Settings.shuttercoeff[0][i] + 5 * Settings.shuttercoeff[1][i]) / 5;
      }
      Shutter.RelayShutterMask |= 3 << (Settings.shutter_startrelay[i] -1);

      Shutter.real_position[i] = ShutterPercentToRealPosition(Settings.shutter_position[i], i);

      Shutter.start_position[i] = Shutter.target_position[i] = Shutter.real_position[i];
      Shutter.motordelay[i] = Settings.shutter_motordelay[i];
      Shutter.lastdirection[i] = (50 < Settings.shutter_position[i]) ? 1 : -1;

      switch (Shutter.position_mode) {
        case SHT_PWM_VALUE:
          Shutter.open_velocity_max =  RESOLUTION;
        break;
      }
      Shutter.close_velocity_max[i] = Shutter.open_velocity_max*Shutter.open_time[i] / Shutter.close_time[i];

      //AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Shutter %d Openvel %d, Closevel: %d"),i, Shutter.open_velocity_max, Shutter.close_velocity_max[i]);
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHT%d: Init. Pos: %d,inverted %d, locked %d, end stop time enabled %d, webButtons inverted %d"),
        i+1,  Shutter.real_position[i],
        (Settings.shutter_options[i]&1) ? 1 : 0, (Settings.shutter_options[i]&2) ? 1 : 0, (Settings.shutter_options[i]&4) ? 1 : 0, (Settings.shutter_options[i]&8) ? 1 : 0);

    } else {
      // terminate loop at first INVALID shutter.
      break;
    }
    ShutterLimitRealAndTargetPositions(i);
    Settings.shutter_accuracy = 1;
    Settings.shutter_mode = Shutter.position_mode;
  }
}

void ShutterReportPosition(bool always, uint32_t index)
{
  Response_P(PSTR("{"));
  rules_flag.shutter_moving = 0;
  uint32_t i = 0;
  uint32_t n = shutters_present;
  if( index != MAX_SHUTTERS) {
    i = index;
    n = index+1;
  }
  for (i; i < n; i++) {
    //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHT: Shutter %d: Real Pos: %d"), i+1,Shutter.real_position[i]);
    uint32_t position = ShutterRealToPercentPosition(Shutter.real_position[i], i);
    if (Shutter.direction[i] != 0) {
      rules_flag.shutter_moving = 1;
      ShutterLogPos(i);
    }
    if (i && index == MAX_SHUTTERS) { ResponseAppend_P(PSTR(",")); }
    uint32_t target = ShutterRealToPercentPosition(Shutter.target_position[i], i);
    ResponseAppend_P(JSON_SHUTTER_POS, i+1, (Settings.shutter_options[i] & 1) ? 100-position : position, Shutter.direction[i],(Settings.shutter_options[i] & 1) ? 100-target : target );
  }
  ResponseJsonEnd();
  if (always || (rules_flag.shutter_moving)) {
    MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR(D_PRFX_SHUTTER));  // RulesProcess() now re-entry protected
  }

  //AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: rules_flag.shutter_moving: %d, moved %d"), rules_flag.shutter_moving, rules_flag.shutter_moved);

}

void ShutterLimitRealAndTargetPositions(uint32_t i) {
  if (Shutter.real_position[i]<0) Shutter.real_position[i] = 0;
  if (Shutter.real_position[i]>Shutter.open_max[i]) Shutter.real_position[i] = Shutter.open_max[i];
  if (Shutter.target_position[i]<0) Shutter.target_position[i] = 0;
  if (Shutter.target_position[i]>Shutter.open_max[i]) Shutter.target_position[i] = Shutter.open_max[i];
}

void ShutterCalculateAccelerator(uint8_t i)
{
  // No Logging allowed. Part of RTC Timer
  if (Shutter.direction[i] != 0) {
    switch (Shutter.position_mode) {
      case SHT_COUNTER:
      case SHT_PWM_VALUE:
        // calculate max velocity allowed in this direction
        velocity_max = Shutter.direction[i] == 1 ? Shutter.open_velocity_max : Shutter.close_velocity_max[i];
        // calculate max change of velocyty based on the defined motordelay in steps
        velocity_change_per_step_max =  velocity_max / (Shutter.motordelay[i]>0 ? Shutter.motordelay[i] : 1);
        // minimumtime required from current velocity to stop
        min_runtime_ms = Shutter.pwm_velocity[i] * 1000 / STEPS_PER_SECOND / velocity_change_per_step_max;
        // decellartion way from current velocity
        current_stop_way = (min_runtime_ms * (Shutter.pwm_velocity[i]+velocity_change_per_step_max)/100 - Shutter.pwm_velocity[i])*RESOLUTION/Shutter.open_velocity_max * Shutter.direction[i] ;
        next_possible_stop_position = Shutter.real_position[i] + current_stop_way ;
        toBeAcc = 0;
        // ensure that the accelerotor kicks in at least one step BEFORE it is to late and a hard stop required.
        if (Shutter.accelerator[i] < 0 || (next_possible_stop_position * Shutter.direction[i]) +RESOLUTION*Shutter.pwm_velocity[i]/Shutter.open_velocity_max>= Shutter.target_position[i] * Shutter.direction[i] ) {
            // 10 times the deviation is the p-value of this simple p-regulator
            toBeAcc = 100+(Shutter.direction[i]*(next_possible_stop_position-Shutter.target_position[i])*velocity_max/Shutter.pwm_velocity[i]*10/RESOLUTION);
            Shutter.accelerator[i] = - tmin(tmax( velocity_change_per_step_max*toBeAcc/100  , (velocity_change_per_step_max*9/10)), (velocity_change_per_step_max*11/10));
        } else if (  Shutter.accelerator[i] > 0 && Shutter.pwm_velocity[i] ==  velocity_max) {
          Shutter.accelerator[i] = 0;
        }
      break;
    }
  }
}

void ShutterDecellerateForStop(uint8_t i)
{
  switch (Shutter.position_mode) {
    case SHT_PWM_VALUE:
    case SHT_COUNTER:
      int16_t missing_steps;
      Shutter.accelerator[i] = -(Shutter.open_velocity_max / (Shutter.motordelay[i]>0 ? Shutter.motordelay[i] : 1) *11/10);
      while (Shutter.pwm_velocity[i] > -2*Shutter.accelerator[i] ) {
        AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: velocity: %ld, delta: %d"), Shutter.pwm_velocity[i], Shutter.accelerator[i] );
        //Shutter.pwm_velocity[i] = tmax(Shutter.pwm_velocity[i]-Shutter.accelerator[i] , 0);
        // Control will be done in RTC Ticker.
        delay(50);
      }
      if (Shutter.position_mode == SHT_COUNTER){
        missing_steps = ((Shutter.target_position[i]-Shutter.start_position[i])*Shutter.direction[i]*Shutter.open_velocity_max/RESOLUTION/STEPS_PER_SECOND) - RtcSettings.pulse_counter[i];
        //prepare for stop PWM
        AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Remain steps %d, counter %d, freq %d"), missing_steps, RtcSettings.pulse_counter[i] ,Shutter.pwm_velocity[i]);
        Shutter.accelerator[i] = 0;
        Shutter.pwm_velocity[i] = Shutter.pwm_velocity[i] > 250 ? 250 : Shutter.pwm_velocity[i];
        analogWriteFreq(Shutter.pwm_velocity[i]);
        analogWrite(Pin(GPIO_PWM1, i), 50);
        Shutter.pwm_velocity[i] = 0;
        analogWriteFreq(Shutter.pwm_velocity[i]);
        while (RtcSettings.pulse_counter[i] < (uint32_t)(Shutter.target_position[i]-Shutter.start_position[i])*Shutter.direction[i]*Shutter.open_velocity_max/RESOLUTION/STEPS_PER_SECOND) {
          delay(1);
        }
        analogWrite(Pin(GPIO_PWM1, i), 0); // removed with 8.3 because of reset caused by watchog
        Shutter.real_position[i] = ShutterCalculatePosition(i);
        AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Real %d, pulsecount %d, start %d"), Shutter.real_position[i],RtcSettings.pulse_counter[i], Shutter.start_position[i]);

      }
      Shutter.direction[i] = 0;
      Shutter.pwm_velocity[i] = 0;
    break;
  }
}

void ShutterPowerOff(uint8_t i) {
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHT: Stop Shutter %d .."), i);
  ShutterDecellerateForStop(i);
  if (Shutter.direction[i] !=0) {
    Shutter.direction[i] = 0;
    delay(MOTOR_STOP_TIME);
  }
  switch (Shutter.switch_mode[i]) {
    case SHT_SWITCH:
      if ((1 << (Settings.shutter_startrelay[i]-1)) & power) {
        ExecuteCommandPowerShutter(Settings.shutter_startrelay[i], 0, SRC_SHUTTER);
      }
      if ((1 << (Settings.shutter_startrelay[i])) & power) {
        ExecuteCommandPowerShutter(Settings.shutter_startrelay[i]+1, 0, SRC_SHUTTER);
      }
    break;
    case SHT_PULSE:
      uint8_t cur_relay = Settings.shutter_startrelay[i] + (Shutter.direction[i] == 1 ? 0 : (uint8_t)(Shutter.position_mode == SHT_TIME)) ;
      // we have a momentary switch here. Needs additional pulse on same relay after the end
      if ((SRC_PULSETIMER == last_source || SRC_SHUTTER == last_source || SRC_WEBGUI == last_source)) {
        ExecuteCommandPowerShutter(cur_relay, 1, SRC_SHUTTER);
        // switch off direction relay to make it power less
        if ((1 << (Settings.shutter_startrelay[i])) & power) {
          ExecuteCommandPowerShutter(Settings.shutter_startrelay[i]+1, 0, SRC_SHUTTER);
        }
      } else {
        last_source = SRC_SHUTTER;
      }
    break;
  }
  // Store current PWM value to ensure proper position after reboot.
  switch (Shutter.position_mode) {
    case SHT_PWM_VALUE:
    char scmnd[20];
    snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_PWM " %d" ),Shutter.pwm_value[i]);
    ExecuteCommand(scmnd, SRC_BUTTON);
    break;
  }
}

void ShutterUpdatePosition(void)
{

  char scommand[CMDSZ];
  char stopic[TOPSZ];

  for (uint32_t i = 0; i < shutters_present; i++) {
    if (Shutter.direction[i] != 0) {

      // Calculate position with counter. Much more accurate and no need for motordelay workaround
      //                        adding some steps to stop early
      //Shutter.real_position[i] =  ShutterCalculatePosition(i);
      if (!Shutter.start_reported) {
        ShutterReportPosition(true, i);
        XdrvRulesProcess();
        Shutter.start_reported = 1;
      }
      //ShutterCalculateAccelerator(i);
      AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: time: %d, toBeAcc %d, current_stop_way %d,vel_vur %d, vel_max %d, act_vel_change %d, min_runtime_ms %d, act.pos %d, next_stop %d, target: %d,  velocity_change_per_step_max %d"),Shutter.time[i],toBeAcc,current_stop_way,
                                    Shutter.pwm_velocity[i],velocity_max, Shutter.accelerator[i],min_runtime_ms,Shutter.real_position[i], next_possible_stop_position,Shutter.target_position[i],velocity_change_per_step_max);


      if ( Shutter.real_position[i] * Shutter.direction[i] >= Shutter.target_position[i] * Shutter.direction[i] || Shutter.pwm_velocity[i]<velocity_change_per_step_max) {
        if (Shutter.direction[i] != 0) {
          Shutter.lastdirection[i] = Shutter.direction[i];
        }
        ShutterPowerOff(i);
        ShutterLimitRealAndTargetPositions(i);
        Settings.shutter_position[i] = ShutterRealToPercentPosition(Shutter.real_position[i], i);
        Shutter.start_position[i] = Shutter.real_position[i];

        ShutterLogPos(i);

        // sending MQTT result to broker
        snprintf_P(scommand, sizeof(scommand),PSTR(D_SHUTTER "%d"), i+1);
        GetTopic_P(stopic, STAT, mqtt_topic, scommand);
        Response_P("%d", (Settings.shutter_options[i] & 1) ? 100 - Settings.shutter_position[i]: Settings.shutter_position[i]);
        MqttPublish(stopic, Settings.flag.mqtt_power_retain);  // CMND_POWERRETAIN
        ShutterReportPosition(true, i);
        rules_flag.shutter_moved = 1;
        XdrvRulesProcess();
      }
    }
  }
}

bool ShutterState(uint32_t device)
{
  device--;
  device &= 3;
  return (Settings.flag3.shutter_mode &&  // SetOption80 - Enable shutter support
          (Shutter.RelayShutterMask & (1 << (Settings.shutter_startrelay[device]-1))) );
}

void ShutterStartInit(uint32_t i, int32_t direction, int32_t target_pos)
{
  //AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: dir %d, delta1 %d, delta2 %d, grant %d"),direction, (Shutter.open_max[i] - Shutter.real_position[i]) / Shutter.close_velocity[i], Shutter.real_position[i] / Shutter.close_velocity[i], 2+Shutter.motordelay[i]);
  if (  ( (1 == direction) && ((Shutter.open_max[i] - Shutter.real_position[i]) / 100 <= 2) )
     || ( (-1 == direction) && (Shutter.real_position[i] / Shutter.close_velocity[i] <= 2)) ) {
    Shutter.skip_relay_change = 1;
  } else {
    Shutter.pwm_velocity[i] = 0;
    switch (Shutter.position_mode) {
#ifdef SHUTTER_STEPPER
      case SHT_COUNTER:
        analogWriteFreq(Shutter.pwm_velocity[i]);
        analogWrite(Pin(GPIO_PWM1, i), 0);
        RtcSettings.pulse_counter[i] = 0;
      break;
#endif
    }
    Shutter.accelerator[i] = Shutter.open_velocity_max / (Shutter.motordelay[i]>0 ? Shutter.motordelay[i] : 1);
    Shutter.target_position[i] = target_pos;
    Shutter.start_position[i] = Shutter.real_position[i];
    Shutter.time[i] = 0;
    Shutter.skip_relay_change = 0;
    Shutter.direction[i] = direction;
    rules_flag.shutter_moving = 1;
    rules_flag.shutter_moved  = 0;
    Shutter.start_reported = 0;
    //AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: real %d, start %d, counter %d,freq_max %d, dir %d, freq %d"),Shutter.real_position[i], Shutter.start_position[i] ,RtcSettings.pulse_counter[i],Shutter.open_velocity_max , Shutter.direction[i] ,Shutter.open_velocity_max );
  }
  //AddLog_P2(LOG_LEVEL_DEBUG,  PSTR("SHT: Start shutter: %d from %d to %d in direction %d"), i, Shutter.start_position[i], Shutter.target_position[i], Shutter.direction[i]);
}


int32_t ShutterCalculatePosition(uint32_t i)
{
    // No Logging allowed. Part of RTC Timer
  if (Shutter.direction[i] != 0) {
    switch (Shutter.position_mode) {
      case SHT_COUNTER:
        return ((int32_t)RtcSettings.pulse_counter[i]*Shutter.direction[i]*STEPS_PER_SECOND*RESOLUTION / Shutter.open_velocity_max)+Shutter.start_position[i];
        break;
      case SHT_TIME:
      case SHT_TIME_UP_DOWN:
      case SHT_TIME_GARAGE:
        return Shutter.start_position[i] + ( (Shutter.time[i] - Shutter.motordelay[i]) * (Shutter.direction[i] > 0 ? RESOLUTION : -Shutter.close_velocity[i]));
        break;
      case SHT_PWM_TIME:
        break;
      case SHT_PWM_VALUE:
        return Shutter.real_position[i];
      break;
      default:
        break;
      }
    } else {
      return Shutter.real_position[i];
    }
}

void ShutterRelayChanged(void)
{

  // Shutter.RelayCurrentMask = binary relay that was recently changed and cause an Action
  // powerstate_local = binary powermatrix and relays from shutter: 0..3
  // relays_changed = bool if one of the relays that belong to the shutter changed not by shutter or pulsetimer
  char stemp1[10];

	for (uint32_t i = 0; i < shutters_present; i++) {
		power_t powerstate_local = (power >> (Settings.shutter_startrelay[i] -1)) & 3;
    // SRC_IGNORE added because INTERLOCK function bite causes this as last source for changing the relay.
		//uint8   manual_relays_changed = ((Shutter.RelayCurrentMask >> (Settings.shutter_startrelay[i] -1)) & 3) && SRC_IGNORE != last_source && SRC_SHUTTER != last_source && SRC_PULSETIMER != last_source ;
    uint8   manual_relays_changed = ((Shutter.RelayCurrentMask >> (Settings.shutter_startrelay[i] -1)) & 3) && SRC_SHUTTER != last_source && SRC_PULSETIMER != last_source ;
    AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Shutter %d: source: %s, powerstate_local %ld, Shutter.RelayCurrentMask %d, manual change %d"), i+1, GetTextIndexed(stemp1, sizeof(stemp1), last_source, kCommandSource), powerstate_local,Shutter.RelayCurrentMask,manual_relays_changed);
    if (manual_relays_changed) {
      //Shutter.skip_relay_change = true;
      ShutterLimitRealAndTargetPositions(i);
      switch (Shutter.switch_mode[i] ) {
        case SHT_PULSE:
          if (Shutter.direction[i] != 0 && powerstate_local) {
            Shutter.target_position[i] = Shutter.real_position[i];
            powerstate_local = 0;
  					AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Shutter %d: Switch OFF motor. Target: %ld, source: %s, powerstate_local %ld, Shutter.RelayCurrentMask %d, manual change %d"), i+1, Shutter.target_position[i], GetTextIndexed(stemp1, sizeof(stemp1), last_source, kCommandSource), powerstate_local,Shutter.RelayCurrentMask,manual_relays_changed);
          }
        break;
        default:
          last_source = SRC_SHUTTER; // avoid switch off in the next loop
          if (Shutter.direction[i] != 0 ) ShutterPowerOff(i);
      }
      switch (Shutter.position_mode) {
        // enum Shutterposition_mode {SHT_TIME, SHT_TIME_UP_DOWN, SHT_TIME_GARAGE, SHT_COUNTER, SHT_PWM_VALUE, SHT_PWM_TIME,};
        case SHT_TIME_UP_DOWN:
        case SHT_COUNTER:
        case SHT_PWM_VALUE:
        case SHT_PWM_TIME:
          ShutterPowerOff(i);
          switch (powerstate_local) {
            case 1:
              ShutterStartInit(i, 1, Shutter.open_max[i]);
              break;
            case 3:
              ShutterStartInit(i, -1, 0);
              break;
            default:
              //AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Shutter %d: Switch OFF motor."),i);
              Shutter.target_position[i] = Shutter.real_position[i];
            }
        break;
        case SHT_TIME:
          switch (powerstate_local) {
            case 1:
              ShutterStartInit(i, 1, Shutter.open_max[i]);
              break;
            case 2:
              ShutterStartInit(i, -1, 0);
              break;
              default:
                //AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Shutter %d: Switch OFF motor."),i);
                Shutter.target_position[i] = Shutter.real_position[i];
          }
        break;
        case SHT_TIME_GARAGE:
         switch (powerstate_local) {
           case 1:
             ShutterStartInit(i, Shutter.lastdirection[i]*-1 , Shutter.lastdirection[i] == 1 ?  0 : Shutter.open_max[i]);
             AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Shutter %d Garage. NewTarget %d"), i, Shutter.target_position[i]);
           break;
           default:
            Shutter.target_position[i] = Shutter.real_position[i];
         }


			  } // switch (Shutter.position_mode)
        AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Shutter %d: Target: %ld, powerstatelocal %d"), i+1, Shutter.target_position[i], powerstate_local);
		 } // if (manual_relays_changed)
  } // for (uint32_t i = 0; i < shutters_present; i++)
}

bool ShutterButtonIsSimultaneousHold(uint32_t button_index, uint32_t shutter_index) {
  // check for simultaneous shutter button hold
  uint32 min_shutterbutton_hold_timer = -1; // -1 == max(uint32)
  for (uint32_t i = 0; i < MAX_KEYS; i++) {
    if ((button_index != i) && (Settings.shutter_button[i] & (1<<31)) && ((Settings.shutter_button[i] & 0x03) == shutter_index) && (Button.hold_timer[i] < min_shutterbutton_hold_timer))
      min_shutterbutton_hold_timer = Button.hold_timer[i];
  }
  return ((-1 != min_shutterbutton_hold_timer) && (min_shutterbutton_hold_timer > (Button.hold_timer[button_index]>>1)));
}

void ShutterButtonHandler(void)
{
  uint8_t buttonState = SHT_NOT_PRESSED;
  uint8_t button = XdrvMailbox.payload;
  uint8_t press_index;
  uint32_t button_index = XdrvMailbox.index;
  uint8_t shutter_index = Settings.shutter_button[button_index] & 0x03;
  uint16_t loops_per_second = 1000 / Settings.button_debounce;  // ButtonDebounce (50)

  if ((PRESSED == button) && (NOT_PRESSED == Button.last_state[button_index])) {
    if (Settings.flag.button_single) {                   // SetOption13 (0) - Allow only single button press for immediate action
        buttonState = SHT_PRESSED_MULTI;
        press_index = 1;
    } else {
      if ((Shutter.direction[shutter_index]) && (Button.press_counter[button_index]==0)) {
        buttonState = SHT_PRESSED_IMMEDIATE;
        press_index = 1;
        Button.press_counter[button_index] = 99; // Remember to discard further action for press & hold within button timings
      } else {
        Button.press_counter[button_index] = (Button.window_timer[button_index]) ? Button.press_counter[button_index] +1 : 1;
        // Button.window_timer[button_index] = (Button.press_counter[button_index]==1) ? loops_per_second / 2 : loops_per_second;  // 0.5 second multi press window after 1st press, 1s afterwards
        Button.window_timer[button_index] = (loops_per_second >> 2) * 3; // 0.75 second multi press window
      }
    }
    blinks = 201;
  }

  if (NOT_PRESSED == button) {
    Button.hold_timer[button_index] = 0;
  } else {
    Button.hold_timer[button_index]++;
    if (!Settings.flag.button_single) {                   // SetOption13 (0) - Allow only single button press for immediate action
      if (Settings.param[P_HOLD_IGNORE] > 0) {         // SetOption40 (0) - Do not ignore button hold
        if (Button.hold_timer[button_index] > loops_per_second * Settings.param[P_HOLD_IGNORE] / 10) {
          Button.hold_timer[button_index] = 0;         // Reset button hold counter to stay below hold trigger
          Button.press_counter[button_index] = 0;      // Discard button press to disable functionality
        }
      }
      if ((Button.press_counter[button_index]<99) && (Button.hold_timer[button_index] == loops_per_second * Settings.param[P_HOLD_TIME] / 10)) {  // press still valid && SetOption32 (40) - Button hold
        // check for simultaneous shutter button hold
        if (ShutterButtonIsSimultaneousHold(button_index, shutter_index)) {
          // simultaneous shutter button hold detected
          for (uint32_t i = 0; i < MAX_KEYS; i++)
            if ((Settings.shutter_button[i] & (1<<31)) && ((Settings.shutter_button[i] & 0x03) == shutter_index))
              Button.press_counter[i] = 99; // Remember to discard further action for press & hold within button timings
          press_index = 0;
          buttonState = SHT_PRESSED_HOLD_SIMULTANEOUS;
        }
        if (Button.press_counter[button_index]<99) {
          press_index = 0;
          buttonState = SHT_PRESSED_HOLD;
        }
        Button.press_counter[button_index] = 0;
      }
      if ((Button.press_counter[button_index]==0) && (Button.hold_timer[button_index] == loops_per_second * IMMINENT_RESET_FACTOR * Settings.param[P_HOLD_TIME] / 10)) {  // SetOption32 (40) - Button held for factor times longer
        press_index = -1;
        // check for simultaneous shutter button extend hold
        if (ShutterButtonIsSimultaneousHold(button_index, shutter_index)) {
          // simultaneous shutter button extend hold detected
          buttonState = SHT_PRESSED_EXT_HOLD_SIMULTANEOUS;
        } else {
          buttonState = SHT_PRESSED_EXT_HOLD;
        }
      }
    }
  }

  if (!Settings.flag.button_single) {                    // SetOption13 (0) - Allow multi-press
    if (Button.window_timer[button_index]) {
      Button.window_timer[button_index]--;
    } else {
      if (!restart_flag && !Button.hold_timer[button_index] && (Button.press_counter[button_index] > 0)) {
        if (Button.press_counter[button_index]<99) {
          // check for simultaneous shutter button press
          uint32 min_shutterbutton_press_counter = -1; // -1 == max(uint32)
          for (uint32_t i = 0; i < MAX_KEYS; i++) {
            AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Settings.shutter_button[i] %ld, shutter_index %d, Button.press_counter[i] %d, min_shutterbutton_press_counter %d, i %d"), Settings.shutter_button[i], shutter_index, Button.press_counter[i] , min_shutterbutton_press_counter, i);
            if ((button_index != i) && (Settings.shutter_button[i] & (1<<31)) && ((Settings.shutter_button[i] & 0x03) == shutter_index) && (i != button_index) && (Button.press_counter[i] < min_shutterbutton_press_counter)) {
              min_shutterbutton_press_counter = Button.press_counter[i];
              AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: min_shutterbutton_press_counter %d"), min_shutterbutton_press_counter);
            }
          }
          if (min_shutterbutton_press_counter == Button.press_counter[button_index]) {
            // simultaneous shutter button press detected
            AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: simultanous presss deteced"));
            press_index = Button.press_counter[button_index];
            for (uint32_t i = 0; i < MAX_KEYS; i++)
              if ((Settings.shutter_button[i] & (1<<31)) && ((Settings.shutter_button[i] & 0x03) != shutter_index))
                Button.press_counter[i] = 99; // Remember to discard further action for press & hold within button timings
            buttonState = SHT_PRESSED_MULTI_SIMULTANEOUS;
          }
          if ((buttonState != SHT_PRESSED_MULTI_SIMULTANEOUS) && (Button.press_counter[button_index]<99)) {
            // no simultaneous shutter button press >3 detected
            press_index = Button.press_counter[button_index];
            buttonState = SHT_PRESSED_MULTI;
          }
        }
        Button.press_counter[button_index] = 0;
      }
    }
  }

  if (buttonState != SHT_NOT_PRESSED) {
    if ((!Settings.flag.button_restrict) && (((press_index>=5) && (press_index<=7)) || (buttonState == SHT_PRESSED_EXT_HOLD) || (buttonState == SHT_PRESSED_EXT_HOLD_SIMULTANEOUS))){
      // check number of buttons for this shutter
      uint8_t shutter_index_num_buttons = 0;
      for (uint32_t i = 0; i < MAX_KEYS; i++) {
        if ((Settings.shutter_button[i] & (1<<31)) && ((Settings.shutter_button[i] & 0x03) == shutter_index)) {
          shutter_index_num_buttons++;
        }
      }
      if ((buttonState == SHT_PRESSED_MULTI_SIMULTANEOUS) || ((shutter_index_num_buttons==1) && (buttonState == SHT_PRESSED_MULTI))){
        // 5x..7x && no SetOption1 (0) checked above
        // simultaneous or stand alone button press 5x, 6x, 7x detected
        char scmnd[20];
        snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_WIFICONFIG " 2"));
        ExecuteCommand(scmnd, SRC_BUTTON);
        return;
      } else if ((buttonState == SHT_PRESSED_EXT_HOLD_SIMULTANEOUS) || ((shutter_index_num_buttons==1) && (buttonState == SHT_PRESSED_EXT_HOLD))){
        // no SetOption1 (0) checked above
        // simultaneous or stand alone button extended hold detected
        char scmnd[20];
        snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_RESET " 1"));
        ExecuteCommand(scmnd, SRC_BUTTON);
        return;
      }
    }
    if (buttonState <= SHT_PRESSED_IMMEDIATE) {
      if (Settings.shutter_startrelay[shutter_index] && Settings.shutter_startrelay[shutter_index] <9) {
        uint8_t pos_press_index = (buttonState == SHT_PRESSED_HOLD) ? 3 : (press_index-1);
        if (pos_press_index>3) pos_press_index=3;
        AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: shutter %d, button %d = %d (single=1, double=2, tripple=3, hold=4)"), shutter_index+1, button_index+1, pos_press_index+1);
        XdrvMailbox.index = shutter_index +1;
        last_source = SRC_BUTTON;
        XdrvMailbox.data_len = 0;
        char databuf[1] = "";
        XdrvMailbox.data = databuf;
        XdrvMailbox.command = NULL;
        if (buttonState == SHT_PRESSED_IMMEDIATE) {
          XdrvMailbox.payload = XdrvMailbox.index;
          CmndShutterStop();
        } else {
          uint8_t position = (Settings.shutter_button[button_index]>>(6*pos_press_index + 2)) & 0x03f;
          if (position) {
            if (Shutter.direction[shutter_index]) {
              XdrvMailbox.payload = XdrvMailbox.index;
              CmndShutterStop();
            } else {
              XdrvMailbox.payload = position = (position-1)<<1;
              //AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: shutter %d -> %d"), shutter_index+1, position);
              if (102 == position) {
                XdrvMailbox.payload = XdrvMailbox.index;
                CmndShutterToggle();
              } else {
                CmndShutterPosition();
              }
              if (Settings.shutter_button[button_index] & ((0x01<<26)<<pos_press_index)) {
                // MQTT broadcast to grouptopic
                char scommand[CMDSZ];
                char stopic[TOPSZ];
                for (uint32_t i = 0; i < MAX_SHUTTERS; i++) {
                  if ((i==shutter_index) || (Settings.shutter_button[button_index] & (0x01<<30))) {
                    snprintf_P(scommand, sizeof(scommand),PSTR("ShutterPosition%d"), i+1);
                    GetGroupTopic_P(stopic, scommand, SET_MQTT_GRP_TOPIC);
                    Response_P("%d", position);
                    MqttPublish(stopic, false);
                  }
                } // for (uint32_t)
              } // if (Settings.shutter)
            } // ende else
          } // if (position)
        } // end else
      } // if   if (Settings.shutter_startrelay[shutter_index]
    }
    Response_P(PSTR("{"));
    ResponseAppend_P(JSON_SHUTTER_BUTTON, shutter_index+1, (buttonState <= SHT_PRESSED_EXT_HOLD) ? (button_index+1) : 0, press_index);
    ResponseJsonEnd();
    MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR(D_PRFX_SHUTTER));
  }
}

void ShutterSetPosition(uint32_t device, uint32_t position)
{
  char svalue[32];                   // Command and number parameter
  snprintf_P(svalue, sizeof(svalue), PSTR(D_PRFX_SHUTTER D_CMND_SHUTTER_POSITION "%d %d"), device, position);
  ExecuteCommand(svalue, SRC_SHUTTER);
}

void ShutterToggle(bool dir)
{
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHT: Payload toggle: %d, i %d, dir %d"), XdrvMailbox.payload, XdrvMailbox.index, dir);
  if ((1 == XdrvMailbox.index) && (XdrvMailbox.payload != -99)) {
    XdrvMailbox.index = XdrvMailbox.payload;
  }
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= shutters_present)) {
    uint32_t index = XdrvMailbox.index-1;
    if (dir) {
      XdrvMailbox.payload = (Shutter.lastdirection[index] > 0) ? 0 : 100;
    }
    else {
      XdrvMailbox.payload = (50 < ShutterRealToPercentPosition(Shutter.real_position[index], index)) ? 0 : 100;
    }
    XdrvMailbox.data_len = 0;
    last_source = SRC_WEBGUI;
    CmndShutterPosition();
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndShutterOpen(void)
{
  //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHT: Payload open: %d, i %d"), XdrvMailbox.payload, XdrvMailbox.index);
  if ((1 == XdrvMailbox.index) && (XdrvMailbox.payload != -99)) {
    XdrvMailbox.index = XdrvMailbox.payload;
  }
  XdrvMailbox.payload = 100;
  last_source = SRC_WEBGUI;
  CmndShutterPosition();
}

void CmndShutterStopOpen(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= shutters_present)) {
    uint32_t index = XdrvMailbox.index-1;
    if (Shutter.direction[index]) {
      CmndShutterStop();
    } else {
      CmndShutterOpen();
    }
  }
}

void CmndShutterClose(void)
{
  //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHT: Payload close: %d, i %d"), XdrvMailbox.payload, XdrvMailbox.index);
  if ((1 == XdrvMailbox.index) && (XdrvMailbox.payload != -99)) {
    XdrvMailbox.index = XdrvMailbox.payload;
  }
  XdrvMailbox.payload = 0;
  XdrvMailbox.data_len = 0;
  last_source = SRC_WEBGUI;
  CmndShutterPosition();
}

void CmndShutterStopClose(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= shutters_present)) {
    uint32_t index = XdrvMailbox.index-1;
    if (Shutter.direction[index]) {
      CmndShutterStop();
    } else {
      CmndShutterClose();
    }
  }
}

void CmndShutterToggle(void)
{
  ShutterToggle(false);
}

void CmndShutterToggleDir(void)
{
  ShutterToggle(true);
}

void CmndShutterStopToggle(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= shutters_present)) {
    uint32_t index = XdrvMailbox.index-1;
    if (Shutter.direction[index]) {
      CmndShutterStop();
    } else {
      CmndShutterToggle();
    }
  }
}

void CmndShutterStopToggleDir(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= shutters_present)) {
    uint32_t index = XdrvMailbox.index-1;
    if (Shutter.direction[index]) {
      CmndShutterStop();
    } else {
      CmndShutterToggleDir();
    }
  }
}

void CmndShutterStop(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= shutters_present)) {
    if (!(Settings.shutter_options[XdrvMailbox.index-1] & 2)) {
      if ((1 == XdrvMailbox.index) && (XdrvMailbox.payload != -99)) {
        XdrvMailbox.index = XdrvMailbox.payload;
      }
      uint32_t i = XdrvMailbox.index -1;
      if (Shutter.direction[i] != 0) {

        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHT: Stop moving %d: dir: %d"), XdrvMailbox.index, Shutter.direction[i]);
        // set stop position 10 steps ahead (0.5sec to allow normal stop)

        //ToDo: Replace with function
        int32_t temp_realpos = Shutter.start_position[i] + ( (Shutter.time[i]+10) * (Shutter.direction[i] > 0 ? 100 : -Shutter.close_velocity[i]));
        XdrvMailbox.payload = ShutterRealToPercentPosition(temp_realpos, i);
        //XdrvMailbox.payload = Settings.shuttercoeff[2][i] * 5 > temp_realpos ? temp_realpos / Settings.shuttercoeff[2][i] : (temp_realpos-Settings.shuttercoeff[0,i]) / Settings.shuttercoeff[1][i];
        last_source = SRC_WEBGUI;
        CmndShutterPosition();
      } else {
        if (XdrvMailbox.command)
          ResponseCmndDone();
      }
    } else {
      if (XdrvMailbox.command)
        ResponseCmndIdxChar("Locked");
    }
  }
}

void CmndShutterPosition(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= shutters_present)) {
    if (!(Settings.shutter_options[XdrvMailbox.index-1] & 2)) {
      uint32_t index = XdrvMailbox.index-1;
      //limit the payload
      AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Pos. in: payload %s (%d), payload %d, idx %d, src %d"), XdrvMailbox.data , XdrvMailbox.data_len, XdrvMailbox.payload , XdrvMailbox.index, last_source );

      // value 0 with data_len > 0 can mean Open
      // special handling fo UP,DOWN,TOGGLE,STOP command comming with payload -99
      if ((XdrvMailbox.data_len > 1) && (XdrvMailbox.payload <= 0)) {
        //UpperCase(XdrvMailbox.data, XdrvMailbox.data);
        if (!strcasecmp(XdrvMailbox.data,D_CMND_SHUTTER_UP) || !strcasecmp(XdrvMailbox.data,D_CMND_SHUTTER_OPEN) || ((Shutter.direction[index]==0) && !strcasecmp(XdrvMailbox.data,D_CMND_SHUTTER_STOPOPEN))) {
          CmndShutterOpen();
          return;
        }
        if (!strcasecmp(XdrvMailbox.data,D_CMND_SHUTTER_DOWN) || !strcasecmp(XdrvMailbox.data,D_CMND_SHUTTER_CLOSE) || ((Shutter.direction[index]==0) && !strcasecmp(XdrvMailbox.data,D_CMND_SHUTTER_STOPCLOSE))) {
          CmndShutterClose();
          return;
        }
        if (!strcasecmp(XdrvMailbox.data,D_CMND_SHUTTER_TOGGLE)) {
          CmndShutterToggle();
          return;
        }
        if (!strcasecmp(XdrvMailbox.data,D_CMND_SHUTTER_TOGGLEDIR)) {
          CmndShutterToggleDir();
          return;
        }
        if (!strcasecmp(XdrvMailbox.data,D_CMND_SHUTTER_STOP) || ((Shutter.direction[index]) && (!strcasecmp(XdrvMailbox.data,D_CMND_SHUTTER_STOPOPEN) || !strcasecmp(XdrvMailbox.data,D_CMND_SHUTTER_STOPCLOSE)))) {
          XdrvMailbox.payload = -99;
          CmndShutterStop();
          return;
        }
      }

      int8_t target_pos_percent = (XdrvMailbox.payload < 0) ? (XdrvMailbox.payload == -99 ? ShutterRealToPercentPosition(Shutter.real_position[index], index) : 0) : ((XdrvMailbox.payload > 100) ? 100 : XdrvMailbox.payload);
      // webgui still send also on inverted shutter the native position.
      target_pos_percent = ((Settings.shutter_options[index] & 1) && (SRC_WEBGUI != last_source)) ? 100 - target_pos_percent : target_pos_percent;
      if (XdrvMailbox.payload != -99) {
        //target_pos_percent = (Settings.shutter_options[index] & 1) ? 100 - target_pos_percent : target_pos_percent;
        Shutter.target_position[index] = ShutterPercentToRealPosition(target_pos_percent, index);
        //Shutter.accelerator[index] = Shutter.open_velocity_max / ((Shutter.motordelay[index] > 0) ? Shutter.motordelay[index] : 1);
        //Shutter.target_position[index] = XdrvMailbox.payload < 5 ?  Settings.shuttercoeff[2][index] * XdrvMailbox.payload : Settings.shuttercoeff[1][index] * XdrvMailbox.payload + Settings.shuttercoeff[0,index];
        AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: lastsource %d:, real %d, target %d, payload %d"), last_source, Shutter.real_position[index] ,Shutter.target_position[index],target_pos_percent);
      }
      if ( (target_pos_percent >= 0) && (target_pos_percent <= 100) && abs(Shutter.target_position[index] - Shutter.real_position[index] ) / Shutter.close_velocity[index] > 2) {
        if (Settings.shutter_options[index] & 4) {
          if (0   == target_pos_percent) Shutter.target_position[index] -= 1 * RESOLUTION * STEPS_PER_SECOND;
          if (100 == target_pos_percent) Shutter.target_position[index] += 1 * RESOLUTION * STEPS_PER_SECOND;
        }
        int8_t new_shutterdirection = Shutter.real_position[index] < Shutter.target_position[index] ? 1 : -1;
        if (Shutter.direction[index] == -new_shutterdirection) {
          ShutterPowerOff(index);
        }
        if (Shutter.direction[index] != new_shutterdirection) {
          ShutterStartInit(index, new_shutterdirection, Shutter.target_position[index]);
          switch (Shutter.position_mode) {
            case SHT_COUNTER:
            case SHT_PWM_TIME:
            case SHT_PWM_VALUE:
            case SHT_TIME_UP_DOWN:
              if (!Shutter.skip_relay_change) {
                // Code for shutters with circuit safe configuration, switch the direction Relay
                ExecuteCommandPowerShutter(Settings.shutter_startrelay[index] +1, new_shutterdirection == 1 ? 0 : 1, SRC_SHUTTER);
                // power on
                ExecuteCommandPowerShutter(Settings.shutter_startrelay[index], 1, SRC_SHUTTER);
              }
              if (Shutter.position_mode != SHT_TIME_UP_DOWN) ExecuteCommandPowerShutter(Settings.shutter_startrelay[index]+2, 1, SRC_SHUTTER);
            break;
            case SHT_TIME:
              if (!Shutter.skip_relay_change) {
                if ( (power >> (Settings.shutter_startrelay[index] -1)) & 3 > 0 ) {
                  ExecuteCommandPowerShutter(Settings.shutter_startrelay[index] + (new_shutterdirection == 1 ? 1 : 0), Shutter.switch_mode[index] == SHT_SWITCH ? 0 : 1, SRC_SHUTTER);
                }
                ExecuteCommandPowerShutter(Settings.shutter_startrelay[index] + (new_shutterdirection == 1 ? 0 : 1), 1, SRC_SHUTTER);
              }
            break;
            case SHT_TIME_GARAGE:
              if (!Shutter.skip_relay_change) {
                if (new_shutterdirection == Shutter.lastdirection[index]) {
                  AddLog_P2(LOG_LEVEL_INFO, PSTR("SHT: Garage not move in this direction: %d"), Shutter.switch_mode[index] == SHT_PULSE);
                  for (uint8_t k=0 ; k <= (uint8_t)(Shutter.switch_mode[index] == SHT_PULSE) ; k++) {
                    ExecuteCommandPowerShutter(Settings.shutter_startrelay[index], 1, SRC_SHUTTER);
                    delay(500);
                    ExecuteCommandPowerShutter(Settings.shutter_startrelay[index], 0, SRC_SHUTTER);
                    delay(500);
                  }
                  // reset shutter time to avoid 2 seconds above count as runtime
                  Shutter.time[index] = 0;
                } // if (new_shutterdirection == Shutter.lastdirection[index])
                ExecuteCommandPowerShutter(Settings.shutter_startrelay[index], 1, SRC_SHUTTER);
              } // if (!Shutter.skip_relay_change)
            break;
          } // switch (Shutter.position_mode)
          Shutter.RelayCurrentMask = 0;
        } // if (Shutter.direction[index] != new_shutterdirection)
      } else {
        target_pos_percent = ShutterRealToPercentPosition(Shutter.real_position[index], index);
        ShutterReportPosition(true, index);
      }
      XdrvMailbox.index = index +1;  // Fix random index for ShutterClose
      if (XdrvMailbox.command)
        ResponseCmndIdxNumber((Settings.shutter_options[index] & 1) ? 100 - target_pos_percent : target_pos_percent);
    } else {
      ShutterReportPosition(true, MAX_SHUTTERS);
      if (XdrvMailbox.command)
        ResponseCmndIdxChar("Locked");
    }
  }
}

void CmndShutterStopPosition(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= shutters_present)) {
    uint32_t index = XdrvMailbox.index-1;
    if (Shutter.direction[index]) {
      XdrvMailbox.payload = -99;
      CmndShutterStop();
    } else {
      CmndShutterPosition();
    }
  }
}
void CmndShutterOpenTime(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= shutters_present)) {
    if (XdrvMailbox.data_len > 0) {
      Settings.shutter_opentime[XdrvMailbox.index -1] = (uint16_t)(10 * CharToFloat(XdrvMailbox.data));
      ShutterInit();
    }
    char time_chr[10];
    dtostrfd((float)(Settings.shutter_opentime[XdrvMailbox.index -1]) / 10, 1, time_chr);
    ResponseCmndIdxChar(time_chr);
  }
}

void CmndShutterCloseTime(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= shutters_present)) {
    if (XdrvMailbox.data_len > 0) {
      Settings.shutter_closetime[XdrvMailbox.index -1] = (uint16_t)(10 * CharToFloat(XdrvMailbox.data));
      ShutterInit();
    }
    char time_chr[10];
    dtostrfd((float)(Settings.shutter_closetime[XdrvMailbox.index -1]) / 10, 1, time_chr);
    ResponseCmndIdxChar(time_chr);
  }
}

void CmndShutterMotorDelay(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= shutters_present)) {
    if (XdrvMailbox.data_len > 0) {
      Settings.shutter_motordelay[XdrvMailbox.index -1] = (uint16_t)(STEPS_PER_SECOND * CharToFloat(XdrvMailbox.data));
      ShutterInit();
    }
    char time_chr[10];
    dtostrfd((float)(Settings.shutter_motordelay[XdrvMailbox.index -1]) / STEPS_PER_SECOND, 2, time_chr);
    ResponseCmndIdxChar(time_chr);
  }
}

void CmndShutterMode(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= MAX_MODES)) {
    Shutter.position_mode =  XdrvMailbox.payload;
    Settings.shutter_mode =  XdrvMailbox.payload;
    ShutterInit();
    ResponseCmndNumber(XdrvMailbox.payload);  // ????
  } else {
    ResponseCmndNumber(Shutter.position_mode);
  }
}

void CmndShutterRelay(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_SHUTTERS)) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 64)) {
      Settings.shutter_startrelay[XdrvMailbox.index -1] = XdrvMailbox.payload;
      if (XdrvMailbox.payload > 0) {
        Shutter.RelayShutterMask |= 3 << (XdrvMailbox.payload - 1);
      } else {
        Shutter.RelayShutterMask ^= 3 << (Settings.shutter_startrelay[XdrvMailbox.index -1] - 1);
      }
      Settings.shutter_startrelay[XdrvMailbox.index -1] = XdrvMailbox.payload;
      ShutterInit();
      // if payload is 0 to disable the relay there must be a reboot. Otherwhise does not work
    }
    ResponseCmndIdxNumber(Settings.shutter_startrelay[XdrvMailbox.index -1]);
  }
}

void CmndShutterButton(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_SHUTTERS)) {
    uint32_t setting = 0;
    // (setting>>31)&(0x01) : enabled
    // (setting>>30)&(0x01) : mqtt broadcast to all index
    // (setting>>29)&(0x01) : mqtt broadcast hold
    // (setting>>28)&(0x01) : mqtt broadcast tripple press
    // (setting>>27)&(0x01) : mqtt broadcast double press
    // (setting>>26)&(0x01) : mqtt broadcast single press
    // (setting>>20)&(0x3f) : shutter_position hold; 0 disabled, 1..101 == 0..100%, 102 == toggle
    // (setting>>14)&(0x3f) : shutter_position tripple press 0 disabled, 1..101 == 0..100%, 102 == toggle
    // (setting>> 8)&(0x3f) : shutter_position double press 0 disabled, 1..101 == 0..100%, 102 == toggle
    // (setting>> 2)&(0x3f) : shutter_position single press 0 disabled, 1..101 == 0..100%, 102 == toggle
    // (setting>> 0)&(0x03) : shutter_index
    if (XdrvMailbox.data_len > 0) {
        uint32_t i = 0;
        uint32_t button_index = 0;
        bool done = false;
        bool isShortCommand = false;
        char *str_ptr;

        char data_copy[strlen(XdrvMailbox.data) +1];
        strncpy(data_copy, XdrvMailbox.data, sizeof(data_copy));  // Duplicate data as strtok_r will modify it.
        // Loop through the data string, splitting on ' ' seperators.
        for (char *str = strtok_r(data_copy, " ", &str_ptr); str && i < (1+4+4+1); str = strtok_r(nullptr, " ", &str_ptr), i++) {
          int field;
          switch (str[0]) {
            case '-':
              field = -1;
              break;
            case 't':
              field = 102;
              break;
            default:
             field = atoi(str);
             break;
          }
          switch (i) {
            case 0:
              if ((field >= -1) && (field<=4)) {
                button_index = (field<=0)?(-1):field;
                done = (button_index==-1);
              } else
                done = true;
            break;
            case 1:
              if (!strcmp_P(str, PSTR("up"))) {
                setting |= (((100>>1)+1)<<2) | (((50>>1)+1)<<8) | (((75>>1)+1)<<14) | (((100>>1)+1)<<20);
                isShortCommand = true;
                break;
              } else if (!strcmp_P(str, PSTR("down"))) {
                setting |= (((0>>1)+1)<<2) | (((50>>1)+1)<<8) | (((25>>1)+1)<<14) | (((0>>1)+1)<<20);
                isShortCommand = true;
                break;
              } else if (!strcmp_P(str, PSTR("updown"))) {
                setting |= (((100>>1)+1)<<2) | (((0>>1)+1)<<8) | (((50>>1)+1)<<14);
                isShortCommand = true;
                break;
              } else if (!strcmp_P(str, PSTR("toggle"))) {
                setting |= (((102>>1)+1)<<2) | (((50>>1)+1)<<8);
                isShortCommand = true;
                break;
              }
            case 2:
              if (isShortCommand) {
                if ((field==1) && (setting & (0x3F<<(2+6*3))))
                  // if short command up or down (hold press position set) then also enable MQTT broadcast
                  setting |= (0x3<<29);
                done = true;
                break;
              }
            case 3:
            case 4:
              if ((field >= -1) && (field<=102))
                setting |= (((field>>1)+1)<<(i*6 + (2-6)));
            break;
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
              if (field==1)
                setting |= (1<<(i + (26-5)));
            break;
          }
          if (done) break;
        }

        if (button_index) {
          if (button_index==-1) {
            // remove all buttons for this shutter
            for (uint32_t i=0 ; i < MAX_KEYS ; i++)
              if ((Settings.shutter_button[i]&0x3) == (XdrvMailbox.index-1))
                Settings.shutter_button[i] = 0;
          } else {
            if (setting) {
              // anything was set
              setting |= (1<<31);
              setting |= (XdrvMailbox.index-1) & 0x3;
            }
            Settings.shutter_button[button_index-1] = setting;
          }
        }
      }
      char setting_chr[30*MAX_KEYS] = "-", *setting_chr_ptr = setting_chr;
      for (uint32_t i=0 ; i < MAX_KEYS ; i++) {
        setting = Settings.shutter_button[i];
        if ((setting&(1<<31)) && ((setting&0x3) == (XdrvMailbox.index-1))) {
          if (*setting_chr_ptr == 0)
            setting_chr_ptr += sprintf_P(setting_chr_ptr, PSTR("|"));
          setting_chr_ptr += snprintf_P(setting_chr_ptr, 2, PSTR("%d"), i+1);

          for (uint32_t j=0 ; j < 4 ; j++) {
            int8_t pos = (((setting>> (2+6*j))&(0x3f))-1)<<1;
            if (0 <= pos)
              if (102 == pos) {
                setting_chr_ptr += sprintf_P(setting_chr_ptr, PSTR(" t"));
              } else {
                setting_chr_ptr += snprintf_P(setting_chr_ptr, 5, PSTR(" %d"), pos);
              }
            else
              setting_chr_ptr += sprintf_P(setting_chr_ptr, PSTR(" -"));
          }
          for (uint32_t j=0 ; j < 5 ; j++) {
            bool mqtt = ((setting>>(26+j))&(0x01)!=0);
            if (mqtt)
              setting_chr_ptr += sprintf_P(setting_chr_ptr, PSTR(" 1"));
            else
              setting_chr_ptr += sprintf_P(setting_chr_ptr, PSTR(" -"));
          }
        }
      }
      ResponseCmndIdxChar(setting_chr);
   }
}

void CmndShutterSetHalfway(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= shutters_present)) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
      Settings.shutter_set50percent[XdrvMailbox.index -1] = (Settings.shutter_options[XdrvMailbox.index -1] & 1) ? 100 - XdrvMailbox.payload : XdrvMailbox.payload;
      ShutterInit();
    }
  ResponseCmndIdxNumber((Settings.shutter_options[XdrvMailbox.index -1] & 1) ? 100 - Settings.shutter_set50percent[XdrvMailbox.index -1] : Settings.shutter_set50percent[XdrvMailbox.index -1]);
  }
}

void CmndShutterFrequency(void)
{
  if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= 20000)) {
    Shutter.open_velocity_max =  XdrvMailbox.payload;
    if (shutters_present < 4) {
      Settings.shuttercoeff[4][3] = Shutter.open_velocity_max;
    }
    ShutterInit();
    ResponseCmndNumber(XdrvMailbox.payload);  // ????
  } else {
    ResponseCmndNumber(Shutter.open_velocity_max);
  }
}

void CmndShutterSetClose(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= shutters_present)) {
    Shutter.real_position[XdrvMailbox.index -1] = 0;
    ShutterStartInit(XdrvMailbox.index -1, 0, 0);
    Settings.shutter_position[XdrvMailbox.index -1] = 0;
    ResponseCmndIdxChar(D_CONFIGURATION_RESET);
  }
}

void CmndShutterSetOpen(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= shutters_present)) {
    Shutter.real_position[XdrvMailbox.index -1] = Shutter.open_max[XdrvMailbox.index -1];
    ShutterStartInit(XdrvMailbox.index -1, 0, Shutter.open_max[XdrvMailbox.index -1]);
    Settings.shutter_position[XdrvMailbox.index -1] = 100;
    ResponseCmndIdxChar(D_CONFIGURATION_RESET);
  }
}

void CmndShutterInvert(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= shutters_present)) {
    if (XdrvMailbox.payload == 0) {
      Settings.shutter_options[XdrvMailbox.index -1] &= ~(1);
    } else if (XdrvMailbox.payload == 1) {
      Settings.shutter_options[XdrvMailbox.index -1] |= (1);
    }
    ResponseCmndIdxNumber((Settings.shutter_options[XdrvMailbox.index -1] & 1) ? 1 : 0);
  }
}

void CmndShutterCalibration(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= shutters_present)) {
    if (XdrvMailbox.data_len > 0) {
      uint32_t i = 0;
      char *str_ptr;

      char data_copy[strlen(XdrvMailbox.data) +1];
      strncpy(data_copy, XdrvMailbox.data, sizeof(data_copy));  // Duplicate data as strtok_r will modify it.
      // Loop through the data string, splitting on ' ' seperators.
      for (char *str = strtok_r(data_copy, " ", &str_ptr); str && i < 5; str = strtok_r(nullptr, " ", &str_ptr), i++) {
        int field = atoi(str);
        // The fields in a data string can only range from 1-30000.
        // and following value must be higher than previous one
        if ((field <= 0) || (field > 30000) || ( (i>0) && (field <= messwerte[i-1]) ) ) {
          break;
        }
        messwerte[i] = field;
      }
      for (i = 0; i < 5; i++) {
        Settings.shuttercoeff[i][XdrvMailbox.index -1] = SHT_DIV_ROUND((uint32_t)messwerte[i] * 1000, messwerte[4]);
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Settings.shuttercoeff: %d, i: %d, value: %d, messwert %d"), i,XdrvMailbox.index -1,Settings.shuttercoeff[i][XdrvMailbox.index -1], messwerte[i]);
      }
      ShutterInit();
      ResponseCmndIdxChar(XdrvMailbox.data);
    } else {
      char setting_chr[30] = "0";
      snprintf_P(setting_chr, sizeof(setting_chr), PSTR("%d %d %d %d %d"), Settings.shuttercoeff[0][XdrvMailbox.index -1], Settings.shuttercoeff[1][XdrvMailbox.index -1], Settings.shuttercoeff[2][XdrvMailbox.index -1], Settings.shuttercoeff[3][XdrvMailbox.index -1], Settings.shuttercoeff[4][XdrvMailbox.index -1]);
      ResponseCmndIdxChar(setting_chr);
    }
  }
}

void CmndShutterLock(void) {
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= shutters_present)) {
    if (XdrvMailbox.payload == 0) {
      Settings.shutter_options[XdrvMailbox.index -1] &= ~(2);
    } else if (XdrvMailbox.payload == 1) {
      Settings.shutter_options[XdrvMailbox.index -1] |= (2);
    }
    ResponseCmndIdxNumber((Settings.shutter_options[XdrvMailbox.index -1] & 2) ? 1 : 0);
  }
}

void CmndShutterEnableEndStopTime(void) {
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= shutters_present)) {
    if (XdrvMailbox.payload == 0) {
      Settings.shutter_options[XdrvMailbox.index -1] &= ~(4);
    } else if (XdrvMailbox.payload == 1) {
      Settings.shutter_options[XdrvMailbox.index -1] |= (4);
    }
    ResponseCmndIdxNumber((Settings.shutter_options[XdrvMailbox.index -1] & 4) ? 1 : 0);
  }
}

void CmndShutterInvertWebButtons(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= shutters_present)) {
    if (XdrvMailbox.payload == 0) {
      Settings.shutter_options[XdrvMailbox.index -1] &= ~(8);
    } else if (XdrvMailbox.payload == 1) {
      Settings.shutter_options[XdrvMailbox.index -1] |= (8);
    }
    ResponseCmndIdxNumber((Settings.shutter_options[XdrvMailbox.index -1] & 8) ? 1 : 0);
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv27(uint8_t function)
{
  bool result = false;

  if (Settings.flag3.shutter_mode) {  // SetOption80 - Enable shutter support
    switch (function) {
      case FUNC_PRE_INIT:
        ShutterInit();
        break;
      case FUNC_EVERY_50_MSECOND:
        ShutterUpdatePosition();
        break;
      case FUNC_EVERY_SECOND:
      //case FUNC_EVERY_250_MSECOND:
        ShutterReportPosition(false, MAX_SHUTTERS);
        break;

      case FUNC_COMMAND:
        result = DecodeCommand(kShutterCommands, ShutterCommand);
        break;
      case FUNC_JSON_APPEND:
        for (uint8_t i = 0; i < shutters_present; i++) {
          uint8_t position = (Settings.shutter_options[i] & 1) ? 100 - Settings.shutter_position[i] : Settings.shutter_position[i];
          uint8_t target   = (Settings.shutter_options[i] & 1) ? 100 - ShutterRealToPercentPosition(Shutter.target_position[i], i) : ShutterRealToPercentPosition(Shutter.target_position[i], i);

          ResponseAppend_P(",");
          ResponseAppend_P(JSON_SHUTTER_POS, i+1, position, Shutter.direction[i],target);
#ifdef USE_DOMOTICZ
          if ((0 == tele_period) && (0 == i)) {
             DomoticzSensor(DZ_SHUTTER, position);
          }
#endif  // USE_DOMOTICZ
        }
        break;
      case FUNC_SET_POWER:
        char stemp1[10];
        // extract the number of the relay that was switched and save for later in Update Position.
        Shutter.RelayCurrentMask = XdrvMailbox.index ^ Shutter.RelayOldMask;
        AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Switched relay: %d by %s"), Shutter.RelayCurrentMask,GetTextIndexed(stemp1, sizeof(stemp1), last_source, kCommandSource));
        ShutterRelayChanged();
        Shutter.RelayOldMask = XdrvMailbox.index;
      break;
      case FUNC_SET_DEVICE_POWER:
        if (Shutter.skip_relay_change ) {
          uint8_t i;
          for (i = 0; i < devices_present; i++) {
            if (Shutter.RelayCurrentMask &1) {
              break;
            }
            Shutter.RelayCurrentMask >>= 1;
          }
          //AddLog_P2(LOG_LEVEL_ERROR, PSTR("SHT: skip relay change: %d"),i+1);
          result = true;
          Shutter.skip_relay_change = 0;
          AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Skipping switch off relay %d"),i);
          ExecuteCommandPowerShutter(i+1, 0, SRC_SHUTTER);
        }
      break;
      case FUNC_BUTTON_PRESSED:
        if (Settings.shutter_button[XdrvMailbox.index] & (1<<31)) {
          ShutterButtonHandler();
          result = true;
        }
      break;
    }
  }
  return result;
}

#endif //USE_SHUTTER
