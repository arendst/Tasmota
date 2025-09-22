/*
  xdrv_27_Shutter.ino - Shutter/Blind support for Tasmota

  Copyright (C) 2023 Stefan Bode

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

#ifdef ESP8266
#ifdef USE_SHUTTER
//#if defined(USE_SHUTTER) && !defined(USE_UFILESYS)
/*********************************************************************************************\
 * Shutter or Blind support using two consecutive relays
\*********************************************************************************************/

#define XDRV_27            27
#ifndef SHUTTER_STEPPER
  #define SHUTTER_STEPPER
#endif

#ifndef SHUTTER_RELAY_OPERATION_TIME
  #define SHUTTER_RELAY_OPERATION_TIME 100 // wait for direction relay 0.1sec before power up main relay
#endif

//#define SHUTTER_UNITTEST

#define D_SHUTTER "SHUTTER"


const uint16_t RESOLUTION = 1000;       // incresed to 1000 in 8.5 to ramp servos
const uint8_t  STEPS_PER_SECOND = 20;   // FUNC_EVERY_50_MSECOND
const uint16_t pwm_servo_max = 500;
const uint16_t pwm_servo_min = 90;

uint8_t calibrate_pos[6] = {0,30,50,70,90,100};
uint16_t messwerte[5] = {30,50,70,90,100};

int32_t  velocity_max = 0;
int32_t  velocity_change_per_step_max = 0;
int32_t  min_runtime_ms = 0;
int32_t  current_stop_way = 0;
int32_t  next_possible_stop_position = 0;
int32_t  current_real_position = 0;
int32_t  current_pwm_velocity = 0;
bool     sensor_data_reported = false;

const uint8_t MAX_MODES = 8;
enum Shutterposition_mode {SHT_UNDEF, SHT_TIME, SHT_TIME_UP_DOWN, SHT_TIME_GARAGE, SHT_COUNTER, SHT_PWM_VALUE, SHT_PWM_TIME,SHT_AUTOCONFIG};
enum Shutterswitch_mode {SHT_SWITCH, SHT_PULSE,};
enum ShutterButtonStates { SHT_NOT_PRESSED, SHT_PRESSED_MULTI, SHT_PRESSED_HOLD, SHT_PRESSED_IMMEDIATE, SHT_PRESSED_EXT_HOLD, SHT_PRESSED_MULTI_SIMULTANEOUS, SHT_PRESSED_HOLD_SIMULTANEOUS, SHT_PRESSED_EXT_HOLD_SIMULTANEOUS,};

const char kShutterCommands[] PROGMEM = D_PRFX_SHUTTER "|"
  D_CMND_SHUTTER_OPEN "|" D_CMND_SHUTTER_CLOSE "|" D_CMND_SHUTTER_TOGGLE "|" D_CMND_SHUTTER_TOGGLEDIR "|" D_CMND_SHUTTER_STOP "|" D_CMND_SHUTTER_POSITION "|"
  D_CMND_SHUTTER_OPENTIME "|" D_CMND_SHUTTER_CLOSETIME "|" D_CMND_SHUTTER_RELAY "|" D_CMND_SHUTTER_MODE "|"  D_CMND_SHUTTER_PWMRANGE "|"
  D_CMND_SHUTTER_SETHALFWAY "|" D_CMND_SHUTTER_SETCLOSE "|" D_CMND_SHUTTER_SETOPEN "|" D_CMND_SHUTTER_INVERT "|" D_CMND_SHUTTER_CLIBRATION "|"
  D_CMND_SHUTTER_MOTORDELAY "|" D_CMND_SHUTTER_FREQUENCY "|" D_CMND_SHUTTER_BUTTON "|" D_CMND_SHUTTER_LOCK "|" D_CMND_SHUTTER_ENABLEENDSTOPTIME "|" D_CMND_SHUTTER_INVERTWEBBUTTONS "|"
  D_CMND_SHUTTER_STOPOPEN "|" D_CMND_SHUTTER_STOPCLOSE "|" D_CMND_SHUTTER_STOPTOGGLE "|" D_CMND_SHUTTER_STOPTOGGLEDIR "|" D_CMND_SHUTTER_STOPPOSITION "|" D_CMND_SHUTTER_INCDEC "|"
#ifdef SHUTTER_UNITTEST
  D_CMND_SHUTTER_UNITTEST "|"
#endif  // SHUTTER_UNITTEST
  D_CMND_SHUTTER_TILTCONFIG "|" D_CMND_SHUTTER_SETTILT "|" D_CMND_SHUTTER_TILTINCDEC "|" D_CMND_SHUTTER_MOTORSTOP;

void (* const ShutterCommand[])(void) PROGMEM = {
  &CmndShutterOpen, &CmndShutterClose, &CmndShutterToggle, &CmndShutterToggleDir, &CmndShutterStop, &CmndShutterPosition,
  &CmndShutterOpenTime, &CmndShutterCloseTime, &CmndShutterRelay, &CmndShutterMode, &CmndShutterPwmRange,
  &CmndShutterSetHalfway, &CmndShutterSetClose, &CmndShutterSetOpen, &CmndShutterInvert, &CmndShutterCalibration , &CmndShutterMotorDelay,
  &CmndShutterFrequency, &CmndShutterButton, &CmndShutterLock, &CmndShutterEnableEndStopTime, &CmndShutterInvertWebButtons,
  &CmndShutterStopOpen, &CmndShutterStopClose, &CmndShutterStopToggle, &CmndShutterStopToggleDir, &CmndShutterStopPosition, &CmndShutterIncDec,
#ifdef SHUTTER_UNITTEST
  &CmndShutterUnitTest,
#endif  // SHUTTER_UNITTEST
  &CmndShutterTiltConfig, &CmndShutterSetTilt, &CmndShutterTiltIncDec, &CmndShutterMotorStop};

  const char JSON_SHUTTER_POS[] PROGMEM = "\"" D_PRFX_SHUTTER "%d\":{\"Position\":%d,\"Direction\":%d,\"Target\":%d,\"Tilt\":%d}";
  const char JSON_SHUTTER_BUTTON[] PROGMEM = "\"" D_PRFX_SHUTTER "%d\":{\"Button%d\":%d}";

#include <Ticker.h>

Ticker TickerShutter;

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
  int8_t   tilt_real_pos;      // -90 to 90
  int8_t   tilt_target_pos;    // target positon for movements of the tilt
  int8_t   tilt_target_pos_override;   // one time override of automatic calculation of tilt_target
  int8_t   tilt_start_pos;             // saved start position before shutter moves
  uint8_t  tilt_velocity;              // degree rotation per step 0.05sec
  int8_t   tiltmoving;                 // 0  operating move, 1 = operating tilt
  uint16_t venetian_delay = 0;         // Delay in steps before venetian shutter start physical moving. Based on tilt position
  uint16_t min_realPositionChange = 0; // minimum change of the position before the shutter operates. different for PWM and time based operations
  uint16_t min_TiltChange = 0;         // minimum change of the tilt before the shutter operates. different for PWM and time based operations
  uint16_t last_reported_time = 0;     // get information on skipped 50ms loop() slots
  uint32_t last_stop_time = 0;         // record the last time the relay was switched off
} Shutter[MAX_SHUTTERS];

struct SHUTTERGLOBAL {
  power_t  RelayShutterMask = 0;             // bit mask with 11 at the position of relays that belong to at least ONE shutter
  power_t  RelayOldMask = 0;                 // bitmatrix that contain the last known state of all relays. Required to detemine the manual changed relay.
  power_t  RelayCurrentMask = 0;             // bitmatrix that contain the current state of all relays
  uint8_t  LastChangedRelay = 0;             // Relay 1..32, 0 no change
  uint8_t  position_mode = 0;                // how to calculate actual position: SHT_TIME, SHT_COUNTER, SHT_PWM_VALUE, SHT_PWM_TIME
  uint8_t  skip_relay_change;                // avoid overrun at endstops
  uint8_t  start_reported = 0;               // indicates of the shutter start was reported through MQTT JSON
  uint16_t open_velocity_max = RESOLUTION;   // maximum of PWM change during opening. Defines velocity on opening. Steppers and Servos only
} ShutterGlobal;

#define SHT_DIV_ROUND(__A, __B) (((__A) + (__B)/2) / (__B))

uint8_t ShutterGetRelayNoFromBitfield(power_t number) {
    int position = 0;
    while (number != 0) {
        position++;
        if (number & 1) return position;
        number >>= 1;
    }
    return 0; // return 0 if no relay found
}

bool ShutterStatus(void) {
  if (Settings->flag3.shutter_mode) {  // SetOption80  - (Shutter) Enable shutter support (1)
    Response_P(PSTR("{\"" D_CMND_STATUS D_STATUS13_SHUTTER "\":{"));
    for (uint32_t i = 0; i < MAX_SHUTTERS; i++) {
      if (0 == Settings->shutter_startrelay[i]) { break; }
      if (i > 0) { ResponseAppend_P(PSTR(",")); }
      ResponseAppend_P(PSTR("\"" D_STATUS13_SHUTTER "%d\":{\"Relay1\":%d,\"Relay2\":%d,\"Open\":%d,\"Close\":%d,"
                                  "\"50perc\":%d,\"Delay\":%d,\"Opt\":\"%s\","
                                  "\"Calib\":[%d,%d,%d,%d,%d],"
                                  "\"Mode\":\"%d\"}"),
                                  i, Settings->shutter_startrelay[i], Settings->shutter_startrelay[i] +1, Settings->shutter_opentime[i], Settings->shutter_closetime[i],
                                  Settings->shutter_set50percent[i], Settings->shutter_motordelay[i], GetBinary8(Settings->shutter_options[i], 4).c_str(),
                                  Settings->shuttercoeff[0][i], Settings->shuttercoeff[1][i], Settings->shuttercoeff[2][i], Settings->shuttercoeff[3][i], Settings->shuttercoeff[4][i],
                                  Settings->shutter_mode);
    }
    ResponseJsonEndEnd();
    return true;
  }
  return false;
}


void ShutterLogPos(uint32_t i)
{
  char stemp2[10];
  dtostrfd((float)Shutter[i].time / STEPS_PER_SECOND, 2, stemp2);
  AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Shtr%d Real %d, Start %d, Stop %d, Dir %d, Delay %d, Rtc %s [s], Freq %d, PWM %d, Tilt %d"),
    i+1, Shutter[i].real_position, Shutter[i].start_position, Shutter[i].target_position, Shutter[i].direction, Shutter[i].motordelay, stemp2,
    Shutter[i].pwm_velocity, Shutter[i].pwm_value,Shutter[i].tilt_real_pos);
}

uint8_t ShutterGetStartRelay(uint8_t index) {
  return Settings->shutter_startrelay[index];
}

uint8_t ShutterGetOptions(uint8_t index) {
   return Settings->shutter_options[index];
}

int8_t ShutterGetTiltConfig(uint8_t config_idx,uint8_t index) {
  return Shutter[index].tilt_config[config_idx];
}

void ExecuteCommandPowerShutter(uint32_t device, uint32_t state, uint32_t source)
{
  // first implementation for virtual relays. Avoid switching relay numbers that do not exist.
  if (device <= TasmotaGlobal.devices_present) ExecuteCommandPower(device,state,source);
}

void ShutterUpdateVelocity(uint8_t i)
{
  // No Logging allowed. Part of RTC Timer
  // will be calles through RTC every 50ms.
  // do not allow accellerator to stop movement
  Shutter[i].pwm_velocity = tmax(velocity_change_per_step_max, Shutter[i].pwm_velocity+Shutter[i].accelerator);
  Shutter[i].pwm_velocity = tmin(Shutter[i].direction==1 ? ShutterGlobal.open_velocity_max : Shutter[i].close_velocity_max,Shutter[i].pwm_velocity);
  // respect hard coded SDK limit of PWM_MIN on PWM frequency.
  if (ShutterGlobal.position_mode == SHT_COUNTER) {
  	Shutter[i].pwm_velocity = tmax(PWM_MIN,Shutter[i].pwm_velocity);
  }
}

void ShutterRtc50mS(void)
{
#ifdef ESP32
  bool pwm_apply = false;   // ESP32 only, do we need to apply PWM changes
#endif
    // No Logging allowed. RTC Timer
  for (uint8_t i = 0; i < TasmotaGlobal.shutters_present; i++) {
    if (Shutter[i].direction) {
      // update position data before increasing counter
      Shutter[i].real_position =  ShutterCalculatePosition(i);
      Shutter[i].time++;
      ShutterCalculateAccelerator(i);
      switch (ShutterGlobal.position_mode) {
        case SHT_PWM_VALUE:
          ShutterUpdateVelocity(i);
          Shutter[i].real_position +=  Shutter[i].direction > 0 ? Shutter[i].pwm_velocity : (Shutter[i].direction < 0 ? -Shutter[i].pwm_velocity : 0);
          Shutter[i].pwm_value = SHT_DIV_ROUND((Settings->shutter_pwmrange[1][i]-Settings->shutter_pwmrange[0][i]) * Shutter[i].real_position , Shutter[i].open_max)+Settings->shutter_pwmrange[0][i];
          AnalogWrite(Pin(GPIO_PWM1, i), Shutter[i].pwm_value);
        break;

        case SHT_COUNTER:
          if (Shutter[i].accelerator) {
            //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Accelerator i=%d -> %d"),i, Shutter[i].accelerator);
            ShutterUpdateVelocity(i);
            digitalWrite(Pin(GPIO_PWM1, i), LOW);
  #ifdef ESP8266
            // Convert frequency into clock cycles
            uint32_t cc = microsecondsToClockCycles(1000000UL) / Shutter[i].pwm_velocity;
            startWaveformClockCycles(Pin(GPIO_PWM1, i), cc/2, cc/2, 0, -1, 0, false);
  #endif  // ESP8266
  #ifdef ESP32
            analogWriteFreq(Shutter[i].pwm_velocity,Pin(GPIO_PWM1, i));
            TasmotaGlobal.pwm_value[i] = 512;
            pwm_apply = true;
  #endif  // ESP32
          }
        break;
      }
    } // if (Shutter[i].direction)
  }
#ifdef ESP32
  if (pwm_apply) { PwmApplyGPIO(false); }
#endif
}

int32_t ShutterPercentToRealPosition(int16_t percent, uint32_t index)
{
	if (Settings->shutter_set50percent[index] != 50) {
    return (percent <= 5) ? Settings->shuttercoeff[2][index] * percent*10 : (Settings->shuttercoeff[1][index] * percent + (Settings->shuttercoeff[0][index]*10))*10;
	} else {
    int64_t realpos;
    // check against DIV 0
    for (uint32_t j = 0; j < 5; j++) {
      if (0 == Settings->shuttercoeff[j][index]) {
        AddLog(LOG_LEVEL_ERROR, PSTR("SHT: RESET/INIT CALIBRATION MATRIX DIV 0"));
        for (uint32_t k = 0; k < 5; k++) {
          Settings->shuttercoeff[k][index] = SHT_DIV_ROUND(calibrate_pos[k+1] * 1000, calibrate_pos[5]);
        }
      }
    }
    for (uint32_t k = 0; k < 5; k++) {
      if ((percent * 10) >= Settings->shuttercoeff[k][index]) {
        realpos = SHT_DIV_ROUND(Shutter[index].open_max * calibrate_pos[k+1], 100);
        //AddLog(LOG_LEVEL_ERROR, PSTR("SHT: Realposition TEMP1: %d, %d %%, coeff %d"), realpos, percent, Settings->shuttercoeff[k][index]);
      } else {
        //AddLog(LOG_LEVEL_ERROR, PSTR("SHT: Shutter[%d].open_max: %d"),index, Shutter[index].open_max);
        if (0 == k) {
          realpos =  SHT_DIV_ROUND((int64_t)percent * Shutter[index].open_max * calibrate_pos[k+1], Settings->shuttercoeff[k][index]*10 );
          //AddLog(LOG_LEVEL_ERROR, PSTR("SHT: Realposition TEMP3: %d, %d %%, coeff %d"), realpos, percent, Settings->shuttercoeff[k][index]);
        } else {
          //uint32_t addon = ( percent*10 - Settings->shuttercoeff[k-1][index] ) * Shutter[index].open_max * (calibrate_pos[k+1] - calibrate_pos[k]) / (Settings->shuttercoeff[k][index] -Settings->shuttercoeff[k-1][index]) / 100;
          //AddLog(LOG_LEVEL_ERROR, PSTR("SHT: Realposition TEMP2: %d, %d %%, coeff %d"), addon, (calibrate_pos[k+1] - calibrate_pos[k]), (Settings->shuttercoeff[k][index] -Settings->shuttercoeff[k-1][index]));
          realpos += SHT_DIV_ROUND(((int64_t)percent*10 - Settings->shuttercoeff[k-1][index] ) * Shutter[index].open_max * (calibrate_pos[k+1] - calibrate_pos[k]), (Settings->shuttercoeff[k][index] - Settings->shuttercoeff[k-1][index])*100);
        }
        break;
      }
    }
		return realpos < 0 ? 0 : realpos;
	}
}

uint8_t ShutterRealToPercentPosition(int32_t realpos, uint32_t index)
{
  int64_t realpercent;
  if (realpos == -9999) {
    realpos = Shutter[index].real_position;
  }
	if (Settings->shutter_set50percent[index] != 50) {
		realpercent = (Settings->shuttercoeff[2][index] * 5 > realpos/10) ? SHT_DIV_ROUND(realpos/10, Settings->shuttercoeff[2][index]) : SHT_DIV_ROUND(realpos/10-Settings->shuttercoeff[0][index]*10, Settings->shuttercoeff[1][index]);
	} else {
    for (uint32_t j = 0; j < 5; j++) {
      if (realpos >= Shutter[index].open_max * calibrate_pos[j+1] / 100) {
        realpercent = SHT_DIV_ROUND(Settings->shuttercoeff[j][index], 10);
        //AddLog(LOG_LEVEL_ERROR, PSTR("SHT: Realpercent TEMP1: %d %%, %d, coeff %d"), realpercent, realpos, Shutter[index].open_max * calibrate_pos[j+1] / 100);
      } else {
        //AddLog(LOG_LEVEL_ERROR, PSTR("SHT: Shutter[%d].open_max: %d"),index, Shutter[index].open_max);
        if (0 == j) {
          realpercent  = SHT_DIV_ROUND(((int64_t)realpos - SHT_DIV_ROUND(Shutter[index].open_max * calibrate_pos[j], 100)) * Settings->shuttercoeff[j][index], calibrate_pos[j+1]/10*Shutter[index].open_max);
        } else {
          //uint16_t addon = ( realpos - (Shutter[index].open_max * calibrate_pos[j] / 100) ) * 10 * (Settings->shuttercoeff[j][index] - Settings->shuttercoeff[j-1][index]) / (calibrate_pos[j+1] - calibrate_pos[j])/Shutter[index].open_max;
          //uint16_t addon = ( realpercent*10 - Settings->shuttercoeff[j-1][index] ) * Shutter[index].open_max * (calibrate_pos[j+1] - calibrate_pos[j]) / (Settings->shuttercoeff[j][index] -Settings->shuttercoeff[j-1][index]) / 100;
          //AddLog(LOG_LEVEL_ERROR, PSTR("SHT: Realpercent TEMP2: %d %%, delta %d, %d, coeff %d"), addon,( realpos - (Shutter[index].open_max * calibrate_pos[j] / 100) ) , (calibrate_pos[j+1] - calibrate_pos[j])* Shutter[index].open_max/100, (Settings->shuttercoeff[j][index] -Settings->shuttercoeff[j-1][index]));
         realpercent += SHT_DIV_ROUND(((int64_t)realpos - SHT_DIV_ROUND(Shutter[index].open_max * calibrate_pos[j], 100)) * (Settings->shuttercoeff[j][index] - Settings->shuttercoeff[j-1][index]), (calibrate_pos[j+1] - calibrate_pos[j])/10*Shutter[index].open_max) ;
	      }
        break;
      }
    }
  }
  realpercent = realpercent < 0 ? 0 : realpercent;
  return realpercent;
}

void ShutterInit(void)
{
  TasmotaGlobal.shutters_present = 0;
  ShutterGlobal.RelayShutterMask = 0;
  //Initialize to get relay that changed
  ShutterGlobal.RelayOldMask = TasmotaGlobal.power;


  // if shutter 4 is unused
  if (Settings->shutter_startrelay[MAX_SHUTTERS -1] == 0) {
     ShutterGlobal.open_velocity_max = Settings->shuttercoeff[4][3] > 0 ? Settings->shuttercoeff[4][3] : ShutterGlobal.open_velocity_max;
  }
  for (uint32_t i = 0; i < MAX_SHUTTERS; i++) {
    // set startrelay to 1 on first init, but only to shutter 1. 90% usecase
    if (Settings->shutter_startrelay[i] && (Settings->shutter_startrelay[i] <= 32 )) {
      bool relay_in_interlock = false;
      TasmotaGlobal.shutters_present++;

      // Add the two relays to the mask to knaw they belong to shutters
      ShutterGlobal.RelayShutterMask |= 3 << (Settings->shutter_startrelay[i] -1)  ;

      // All shutters must have same mode. Switch OR Pulse. N
      switch (Settings->pulse_timer[i]) {
        case 0:
          Shutter[i].switch_mode = SHT_SWITCH;
        break;
        default:
          Shutter[i].switch_mode = SHT_PULSE;
        break;
      }

      // Check if the relay is in an INTERLOCK group. required to set the right mode or
      // verify that on SHT_TIME INTERLOCK is set
      for (uint32_t j = 0; j < MAX_INTERLOCKS * Settings->flag.interlock; j++) {  // CMND_INTERLOCK - Enable/disable interlock
        //AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Interlock state i=%d %d, flag %d, Shuttermask %d, MaskedIL %d"),i, Settings->interlock[i], Settings->flag.interlock,ShutterGlobal.RelayShutterMask, Settings->interlock[i]&ShutterGlobal.RelayShutterMask);
        if (Settings->interlock[j] && (Settings->interlock[j] & ShutterGlobal.RelayShutterMask)) {
          //AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Relay in Interlock group"));
          relay_in_interlock = true;
        }
      }

      if (Settings->shutter_mode == SHT_AUTOCONFIG || Settings->shutter_mode == SHT_UNDEF) {
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Mode undef.. calculate..."));
        ShutterGlobal.position_mode = SHT_TIME;
        if (!relay_in_interlock) {
          // temporary to maintain old functionality
          if (Settings->shutter_mode == SHT_UNDEF) {
            ShutterGlobal.position_mode = SHT_TIME_UP_DOWN;
          }
          if (PinUsed(GPIO_PWM1, i) && PinUsed(GPIO_CNTR1, i)) {
            ShutterGlobal.position_mode = SHT_COUNTER;
          }
        }
      } else {
        ShutterGlobal.position_mode = Settings->shutter_mode;
      }
      AddLog(LOG_LEVEL_INFO, PSTR("SHT: ShutterMode: %d"), ShutterGlobal.position_mode);
      // main function for stepper and servos to control velocity and acceleration.
      TickerShutter.attach_ms(50, ShutterRtc50mS );

      // default the 50 percent should not have any impact without changing it. set to 60
      Settings->shutter_set50percent[i] = (Settings->shutter_set50percent[i] > 0) ? Settings->shutter_set50percent[i] : 50;

      // use 10 sec. as default to allow everybody to play without deep initialize
      Shutter[i].open_time = Settings->shutter_opentime[i] = (Settings->shutter_opentime[i] > 0) ? Settings->shutter_opentime[i] : 100;
      Shutter[i].close_time = Settings->shutter_closetime[i] = (Settings->shutter_closetime[i] > 0) ? Settings->shutter_closetime[i] : 100;

      // Update Calculation 20 because time interval is 0.05 sec ans time is in 0.1sec
      Shutter[i].open_max = STEPS_PER_SECOND * RESOLUTION * Shutter[i].open_time / 10;
      Shutter[i].close_velocity =  Shutter[i].open_max / Shutter[i].close_time / 2 ;

      // calculate a ramp slope at the first 5 percent to compensate that shutters move with down part later than the upper part
      if (Settings->shutter_set50percent[i] != 50) {
       	Settings->shuttercoeff[1][i] = Shutter[i].open_max/10 * (100 - Settings->shutter_set50percent[i] ) / 5000 ;
      	Settings->shuttercoeff[0][i] = Shutter[i].open_max/100 - (Settings->shuttercoeff[1][i] * 10);
      	Settings->shuttercoeff[2][i] = (int32_t)(Settings->shuttercoeff[0][i]*10 + 5 * Settings->shuttercoeff[1][i]) / 5;
        //AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Shtr%d Shutter[i].open_max %d, 50perc:%d, 0:%d, 1:%d 2:%d"), i, Shutter[i].open_max, Settings->shutter_set50percent[i], Settings->shuttercoeff[0][i],Settings->shuttercoeff[1][i],Settings->shuttercoeff[2][i]);
     }
      ShutterGlobal.RelayShutterMask |= 3 << (Settings->shutter_startrelay[i] -1);

      Shutter[i].real_position = ShutterPercentToRealPosition(Settings->shutter_position[i], i);

      Shutter[i].start_position = Shutter[i].target_position = Shutter[i].real_position;
      Shutter[i].motordelay = Settings->shutter_motordelay[i];
      Shutter[i].lastdirection = (50 < Settings->shutter_position[i]) ? 1 : -1;

      // Venetian Blind
      // ensure min is smaller than max
      Settings->shutter_tilt_config[2][i] = Settings->shutter_tilt_config[0][i] >= Settings->shutter_tilt_config[1][i]?0:Settings->shutter_tilt_config[2][i];
      //copy config to shutter
      for (uint8_t k=0; k<5; k++) {
        Shutter[i].tilt_config[k] =  Settings->shutter_tilt_config[k][i];
      }
      // wipe open/close position if duration is 0
      if (Shutter[i].tilt_config[2]==0) {
        Shutter[i].tilt_config[3] = Shutter[i].tilt_config[4] = 0;
      }
      Shutter[i].tilt_target_pos = Shutter[i].tilt_real_pos = Settings->shutter_tilt_pos[i];

      Shutter[i].tilt_velocity = Shutter[i].tilt_config[2] > 0 ? ((Shutter[i].tilt_config[1]-Shutter[i].tilt_config[0])/Shutter[i].tilt_config[2])+1  : 1;

      Shutter[i].close_velocity_max = ShutterGlobal.open_velocity_max*Shutter[i].open_time / Shutter[i].close_time;

      Shutter[i].min_realPositionChange = 2 * tmax(ShutterGlobal.open_velocity_max, Shutter[i].close_velocity_max);
      Shutter[i].min_TiltChange         = 2 * Shutter[i].tilt_velocity;

      switch (ShutterGlobal.position_mode) {
        case SHT_PWM_VALUE:
          ShutterGlobal.open_velocity_max =  RESOLUTION;
          // Initiate pwm range with defaults if not already set.
          Settings->shutter_pwmrange[0][i] = Settings->shutter_pwmrange[0][i] > 0 ? Settings->shutter_pwmrange[0][i] : pwm_servo_min;
          Settings->shutter_pwmrange[1][i] = Settings->shutter_pwmrange[1][i] > 0 ? Settings->shutter_pwmrange[1][i] : pwm_servo_max;
          Shutter[i].min_realPositionChange = 0;
          Shutter[i].min_TiltChange = 0;
        break;
        case SHT_TIME:
          // Test is the relays are in interlock mode. Disable shuttermode if error
          if (!relay_in_interlock) {
            TasmotaGlobal.shutters_present = 0,
            AddLog(LOG_LEVEL_ERROR, PSTR("SHT: ERROR: Shtr%d Relays are not in INTERLOCK. Pls read documentation. Shutter DISABLE. Fix and REBOOT"), i+1);
            return;
          }
        break;
      }
      AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Shtr%d min realpos_chg: %d, min tilt_chg %d"), i+1, Shutter[i].min_realPositionChange, Shutter[i].min_TiltChange);
      AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Shtr%d Openvel %d, Closevel: %d"), i+1, ShutterGlobal.open_velocity_max, Shutter[i].close_velocity_max);
      AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Shtr%d Init. Pos %d, Inv %d, Locked %d, Endstop enab %d, webButt inv %d, Motordel: %d"),
        i+1,  Shutter[i].real_position,
        (Settings->shutter_options[i] & 1) ? 1 : 0, (Settings->shutter_options[i] & 2) ? 1 : 0, (Settings->shutter_options[i] & 4) ? 1 : 0, (Settings->shutter_options[i] & 8) ? 1 : 0, Shutter[i].motordelay);

    } else {
      // terminate loop at first INVALID Shutter[i].
      break;
    }
    ShutterLimitRealAndTargetPositions(i);
    Settings->shutter_accuracy = 1;
    Settings->shutter_mode = ShutterGlobal.position_mode;
    // initialize MotorStop time with 500ms if not set
    // typical not set start values are 0 and 65535
    if (Settings->shutter_motorstop > 5000 || Settings->shutter_motorstop == 0) {
        Settings->shutter_motorstop = 500;
    }
  }
}

void ShutterReportPosition(bool always, uint32_t index)
{
  Response_P(PSTR("{"));
  uint32_t i = 0;
  uint32_t n = TasmotaGlobal.shutters_present;
  uint8_t shutter_running = 0;
  for (i; i < n; i++) {
    if (Shutter[i].direction != 0) {
      shutter_running++;
    }
  }

  // Allow function exit if nothing to report (99.9% use case)
  if (!always && !shutter_running) return;

  if( index != MAX_SHUTTERS) {
    i = index;
    n = index+1;
  } else {
    i = 0;
  }
  for (i; i < n; i++) {
    //AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Shtr%d Real Pos %d"), i+1,Shutter[i].real_position);

    if (Shutter[i].direction != 0) {
      ShutterLogPos(i);
      shutter_running++;
    }
    if (i && index == MAX_SHUTTERS) { ResponseAppend_P(PSTR(",")); }
    uint8_t position = ShutterRealToPercentPosition(Shutter[i].real_position, i);
    uint8_t target   = ShutterRealToPercentPosition(Shutter[i].target_position, i);
    ResponseAppend_P(JSON_SHUTTER_POS, i+1, (Settings->shutter_options[i] & 1) ? 100 - position : position, Shutter[i].direction,(Settings->shutter_options[i] & 1) ? 100 - target : target, Shutter[i].tilt_real_pos );
  }
  ResponseJsonEnd();
  if (always || shutter_running) {
    MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR(D_PRFX_SHUTTER));  // RulesProcess() now re-entry protected
  }
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: rules_flag.shutter_moving: %d, moved %d"), TasmotaGlobal.rules_flag.shutter_moving, TasmotaGlobal.rules_flag.shutter_moved);
}

void ShutterLimitRealAndTargetPositions(uint32_t i)
{
  if (Shutter[i].real_position<0) Shutter[i].real_position = 0;
  if (Shutter[i].real_position>Shutter[i].open_max) Shutter[i].real_position = Shutter[i].open_max;
  if (Shutter[i].target_position<0) Shutter[i].target_position = 0;
  if (Shutter[i].target_position>Shutter[i].open_max) Shutter[i].target_position = Shutter[i].open_max;
}

void ShutterCalculateAccelerator(uint8_t i)
{
  // No Logging allowed. Part of RTC Timer
  if (Shutter[i].direction != 0) {
    switch (ShutterGlobal.position_mode) {
      case SHT_COUNTER:
      case SHT_PWM_VALUE:
        current_real_position =  Shutter[i].real_position;
        current_pwm_velocity = Shutter[i].pwm_velocity;
        // calculate max velocity allowed in this direction
        velocity_max = Shutter[i].direction == 1 ? ShutterGlobal.open_velocity_max : Shutter[i].close_velocity_max;
        // calculate max change of velocyty based on the defined motordelay in steps
        velocity_change_per_step_max =  velocity_max / (Shutter[i].motordelay>0 ? Shutter[i].motordelay : 1);
        // minimumtime required from current velocity to stop
        min_runtime_ms = current_pwm_velocity * 1000 / STEPS_PER_SECOND / velocity_change_per_step_max;
        // decellaration way from current velocity
        current_stop_way = min_runtime_ms * STEPS_PER_SECOND * (current_pwm_velocity + velocity_change_per_step_max) * Shutter[i].direction  / 2 / ShutterGlobal.open_velocity_max - (Shutter[i].accelerator<0?Shutter[i].direction*1000*current_pwm_velocity/ShutterGlobal.open_velocity_max:0);
        next_possible_stop_position = current_real_position + current_stop_way ;
        // ensure that the accelerotor kicks in at the first overrun of the target position
        if (  Shutter[i].accelerator < 0 || next_possible_stop_position * Shutter[i].direction > Shutter[i].target_position * Shutter[i].direction ) {
            // if startet to early because of 0.05sec maximum accuracy and final position is to far away (200) accelerate a bit less
            if (next_possible_stop_position * Shutter[i].direction+200 < Shutter[i].target_position * Shutter[i].direction) {
              Shutter[i].accelerator = -velocity_change_per_step_max*9/10;
            } else {
              // in any case increase accelleration if overrun is detected during decelleration
              if (next_possible_stop_position * Shutter[i].direction > Shutter[i].target_position * Shutter[i].direction && Shutter[i].accelerator < 0) {
                Shutter[i].accelerator = -velocity_change_per_step_max*11/10;
              } else {
                // as long as the calculated end position is ok stay with proposed decelleration
                Shutter[i].accelerator = -velocity_change_per_step_max;
              }
            }
            // detect during the acceleration phase the point final speed is reached
        } else if (  Shutter[i].accelerator > 0 && current_pwm_velocity == velocity_max) {
          Shutter[i].accelerator = 0;
        }
      break;
    }
  }
}

void ShutterDecellerateForStop(uint8_t i)
{
#ifdef ESP32
  bool pwm_apply = false;   // ESP32 only, do we need to apply PWM changes
#endif
  switch (ShutterGlobal.position_mode) {
    case SHT_PWM_VALUE:
    case SHT_COUNTER:
      int16_t missing_steps;
      Shutter[i].accelerator = -(ShutterGlobal.open_velocity_max / (Shutter[i].motordelay>4 ? (Shutter[i].motordelay*11)/10 : 4) );

      while (Shutter[i].pwm_velocity > -2*Shutter[i].accelerator &&  Shutter[i].pwm_velocity != PWM_MIN) {
	      delay(50);
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Velocity %ld, Delta %d"), Shutter[i].pwm_velocity, Shutter[i].accelerator );
        // Control will be done in RTC Ticker.
      }
      if (ShutterGlobal.position_mode == SHT_COUNTER){
        missing_steps = ((Shutter[i].target_position-Shutter[i].start_position)*Shutter[i].direction*ShutterGlobal.open_velocity_max/RESOLUTION/STEPS_PER_SECOND) - RtcSettings.pulse_counter[i];
        //prepare for stop PWM
        Shutter[i].accelerator = 0;
        Shutter[i].pwm_velocity = 0;
        //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Remain %d count %d -> target %d, dir %d"), missing_steps, RtcSettings.pulse_counter[i], (uint32_t)(Shutter[i].target_position-Shutter[i].start_position)*Shutter[i].direction*ShutterGlobal.open_velocity_max/RESOLUTION/STEPS_PER_SECOND, Shutter[i].direction);
        while (RtcSettings.pulse_counter[i] < (uint32_t)(Shutter[i].target_position-Shutter[i].start_position)*Shutter[i].direction*ShutterGlobal.open_velocity_max/RESOLUTION/STEPS_PER_SECOND && missing_steps > 0) {
        }
#ifdef ESP8266
        AnalogWrite(Pin(GPIO_PWM1, i), 0); // removed with 8.3 because of reset caused by watchog
#endif
#ifdef ESP32
        TasmotaGlobal.pwm_value[i] = 0;
        pwm_apply = true;
#endif  // ESP32
        Shutter[i].real_position = ShutterCalculatePosition(i);
        //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Remain steps %d"), missing_steps);
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Real %d, Pulsecount %d, tobe %d, Start %d"), Shutter[i].real_position,RtcSettings.pulse_counter[i],  (uint32_t)(Shutter[i].target_position-Shutter[i].start_position)*Shutter[i].direction*ShutterGlobal.open_velocity_max/RESOLUTION/STEPS_PER_SECOND, Shutter[i].start_position);
      }
      Shutter[i].direction = 0;
      Shutter[i].pwm_velocity = 0;
    break;
  }
#ifdef ESP32
  if (pwm_apply) { PwmApplyGPIO(false); }
#endif
}

void ShutterPowerOff(uint8_t i)
{
  AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Stop %d Mode %d time %d"), i+1,Shutter[i].switch_mode, Shutter[i].time); // fix log to indicate correct shutter number
  ShutterDecellerateForStop(i);
  uint8_t cur_relay = Settings->shutter_startrelay[i] + (Shutter[i].direction == 1 ? 0 : (uint8_t)(ShutterGlobal.position_mode == SHT_TIME)) ;
  if (Shutter[i].direction !=0) {
    Shutter[i].direction = 0;
  }
  if (Shutter[i].real_position == Shutter[i].start_position)  {
    //AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Update target tilt shutter %d from %d to %d"), i+1,  Shutter[i].tilt_target_pos , Shutter[i].tilt_real_pos);
    Shutter[i].tilt_target_pos = Shutter[i].tilt_real_pos;
  }
  TasmotaGlobal.rules_flag.shutter_moved = 1;
  switch (Shutter[i].switch_mode) {
    case SHT_SWITCH:
      for (int8_t k=0;k<2;k++) {
        if ((1 << (Settings->shutter_startrelay[i]+k-1)) & TasmotaGlobal.power) {
          ExecuteCommandPowerShutter(Settings->shutter_startrelay[i]+k, 0, SRC_SHUTTER);
        }
      }
    break;
    case SHT_PULSE:
      // we have a momentary switch here. Needs additional pulse on same relay after the end
      if ((SRC_PULSETIMER == TasmotaGlobal.last_source || SRC_SHUTTER == TasmotaGlobal.last_source || SRC_WEBGUI == TasmotaGlobal.last_source)) {
        ExecuteCommandPowerShutter(cur_relay, 1, SRC_SHUTTER);
        // switch off direction relay to make it power less
        if (((1 << (Settings->shutter_startrelay[i])) & TasmotaGlobal.power)  && Settings->shutter_startrelay[i]+1 != cur_relay) {
          ExecuteCommandPowerShutter(Settings->shutter_startrelay[i]+1, 0, SRC_SHUTTER);
        }
      } else {
        TasmotaGlobal.last_source = SRC_SHUTTER;
      }
    break;
  }
  // Store current PWM value to ensure proper position after reboot.
  switch (ShutterGlobal.position_mode) {
    case SHT_PWM_VALUE:
      Shutter[i].pwm_value = SHT_DIV_ROUND((Settings->shutter_pwmrange[1][i]-Settings->shutter_pwmrange[0][i]) * Shutter[i].target_position , Shutter[i].open_max)+Settings->shutter_pwmrange[0][i];
      AnalogWrite(Pin(GPIO_PWM1, i), Shutter[i].pwm_value);
      AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: PWM final %d"),Shutter[i].pwm_value);
      char scmnd[20];
  #ifdef SHUTTER_CLEAR_PWM_ONSTOP
      // free the PWM servo lock on stop.
      AnalogWrite(Pin(GPIO_PWM1, i), 0);
  #endif
      break;
  }
  if (Settings->save_data) {
    TasmotaGlobal.save_data_counter = Settings->save_data;
  }
  Shutter[i].last_stop_time = millis();
}

void ShutterWaitForMotorStop(uint8_t i)
{
  Shutter[i].last_stop_time = millis();
  ShutterWaitForMotorStart(i);
}

void ShutterWaitForMotorStart(uint8_t i)
{
  while (millis() - Shutter[i].last_stop_time < Settings->shutter_motorstop) { // statement is overflow proof
    loop();
  }
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Stoptime done"));
}

void ShutterUpdatePosition(void)
{
  char scommand[CMDSZ];
  char stopic[TOPSZ];
  for (uint32_t i = 0; i < TasmotaGlobal.shutters_present; i++) {
    if (Shutter[i].direction != 0) {
      if (!ShutterGlobal.start_reported) {
        ShutterReportPosition(true, i);
        ShutterGlobal.start_reported = 1;
      }
      int32_t deltatime = Shutter[i].time-Shutter[i].last_reported_time;
      Shutter[i].last_reported_time = Shutter[i].time+1;
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Shtr%d Time %d(%d), cStop %d, cVelo %d, mVelo %d, aVelo %d, mRun %d, aPos %d, aPos2 %d, nStop %d, Trgt %d, mVelo %d, Dir %d, Tilt %d, TrgtTilt: %d, Tiltmove: %d"),
        i+1, Shutter[i].time, deltatime, current_stop_way, current_pwm_velocity, velocity_max, Shutter[i].accelerator, min_runtime_ms, current_real_position,Shutter[i].real_position,
        next_possible_stop_position, Shutter[i].target_position, velocity_change_per_step_max, Shutter[i].direction,Shutter[i].tilt_real_pos, Shutter[i].tilt_target_pos,
         Shutter[i].tiltmoving);
      if ( ((Shutter[i].real_position * Shutter[i].direction >= Shutter[i].target_position * Shutter[i].direction &&  Shutter[i].tiltmoving==0) ||
           ((int16_t)Shutter[i].tilt_real_pos * Shutter[i].direction * Shutter[i].tilt_config[2] >= (int16_t)Shutter[i].tilt_target_pos * Shutter[i].direction * Shutter[i].tilt_config[2] && Shutter[i].tiltmoving==1))
           || (ShutterGlobal.position_mode == SHT_COUNTER && Shutter[i].accelerator <0 && Shutter[i].pwm_velocity+Shutter[i].accelerator<PWM_MIN)) {
        if (Shutter[i].direction != 0) {
          Shutter[i].lastdirection = Shutter[i].direction;
        }
        ShutterPowerOff(i);
        ShutterLimitRealAndTargetPositions(i);
        Settings->shutter_position[i] = ShutterRealToPercentPosition(Shutter[i].real_position, i);
        Shutter[i].start_position = Shutter[i].real_position;

        //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Pre: Tilt not match %d -> %d, moving: %d"),Shutter[i].tilt_real_pos,Shutter[i].tilt_target_pos,Shutter[i].tiltmoving);
        if (abs(Shutter[i].tilt_real_pos - Shutter[i].tilt_target_pos) > Shutter[i].min_TiltChange && Shutter[i].tiltmoving == 0) {
          AddLog(LOG_LEVEL_INFO, PSTR("SHT: Tilt not match %d -> %d"),Shutter[i].tilt_real_pos,Shutter[i].tilt_target_pos);
          char databuf[1] = "";
          XdrvMailbox.data = databuf;
          XdrvMailbox.payload = -99;
          XdrvMailbox.index = i+1;
          Shutter[i].tiltmoving = 1;
          CmndShutterPosition();
          return;
        } else {
          Settings->shutter_tilt_pos[i] = Shutter[i].tilt_real_pos;
        }
        ShutterLogPos(i);

        if (!Settings->flag4.only_json_message) {  // SetOption90 - Disable non-json MQTT response
          // sending MQTT result to broker
          snprintf_P(scommand, sizeof(scommand),PSTR(D_SHUTTER "%d"), i+1);
          GetTopic_P(stopic, STAT, TasmotaGlobal.mqtt_topic, scommand);
          Response_P("%d", (Settings->shutter_options[i] & 1) ? 100 - Settings->shutter_position[i]: Settings->shutter_position[i]);
          MqttPublish(stopic, Settings->flag.mqtt_power_retain);  // CMND_POWERRETAIN
        }

        ShutterReportPosition(true, i);
        TasmotaGlobal.rules_flag.shutter_moved = 1;
      }
    }
  }
}

bool ShutterState(uint32_t device)
{
  if (device > 4) { return false; }
  device--;
  device &= 3;
  return (Settings->flag3.shutter_mode &&  // SetOption80 - Enable shutter support
          (ShutterGlobal.RelayShutterMask & (1 << (Settings->shutter_startrelay[device]-1))) );
}

void ShutterAllowPreStartProcedure(uint8_t i) {
  // Tricky!!! Execute command status 2 while in the 10 sec loop and you'll end up in an exception
  // What PreStartProcedure do you want to execute here?
  // Anyway, as long var1 != 99 this is skipped (luckily)
#ifdef USE_RULES
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Delay Start? var%d <99>=<%s>, max10s?"),i + 1, rules_vars[i]);
  // wait for response from rules
  uint32_t end_time = millis() + 10000;
  while (!TimeReached(end_time) && (String)rules_vars[i] == "99") {
    delay(1);
  }
#endif  // USE_RULES
}

void ShutterStartInit(uint32_t i, int32_t direction, int32_t target_pos)
{
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: dir %d, delta1 %d, delta2 %d"),direction, (Shutter[i].open_max - Shutter[i].real_position) / Shutter[i].close_velocity, Shutter[i].real_position / Shutter[i].close_velocity);
  if ( ( ( (1 == direction) && ((Shutter[i].open_max - Shutter[i].real_position)  <= Shutter[i].min_realPositionChange))
     || ( (-1 == direction) &&  (Shutter[i].real_position <= Shutter[i].min_realPositionChange)) )
     && abs(Shutter[i].tilt_real_pos-Shutter[i].tilt_target_pos) <=  Shutter[i].min_TiltChange) {
    ShutterGlobal.skip_relay_change = 1;
  } else {
    Shutter[i].pwm_velocity = 0;
    ShutterWaitForMotorStart(i);
    switch (ShutterGlobal.position_mode) {
#ifdef SHUTTER_STEPPER
      case SHT_COUNTER:
#ifdef ESP8266
        analogWriteFreq(Shutter[i].pwm_velocity);
        AnalogWrite(Pin(GPIO_PWM1, i), 0);
#endif
#ifdef ESP32
        analogWriteFreq(PWM_MIN,Pin(GPIO_PWM1, i));
        TasmotaGlobal.pwm_value[i] = 0;
        PwmApplyGPIO(false);
#endif
        RtcSettings.pulse_counter[i] = 0;
      break;
#endif
    }

    Shutter[i].accelerator = ShutterGlobal.open_velocity_max / (Shutter[i].motordelay>0 ? Shutter[i].motordelay : 1);
    Shutter[i].target_position = target_pos;
    Shutter[i].start_position = Shutter[i].real_position;
    TasmotaGlobal.rules_flag.shutter_moving = 1;
    sensor_data_reported = false;
    ShutterAllowPreStartProcedure(i);
    Shutter[i].time = Shutter[i].last_reported_time = 0;

    ShutterGlobal.skip_relay_change = 0;
    TasmotaGlobal.rules_flag.shutter_moved  = 0;
    ShutterGlobal.start_reported = 0;
    Shutter[i].tilt_real_pos = tmax(tmin(Shutter[i].tilt_real_pos,Shutter[i].tilt_config[1]),Shutter[i].tilt_config[0]);
    Shutter[i].tilt_start_pos =  Shutter[i].tilt_real_pos;
    if (Shutter[i].tilt_config[1]-Shutter[i].tilt_config[0] != 0) {
      Shutter[i].venetian_delay = SHT_DIV_ROUND((direction > 0 ? Shutter[i].tilt_config[1]-Shutter[i].tilt_real_pos : Shutter[i].tilt_real_pos-Shutter[i].tilt_config[0]) * Shutter[i].tilt_config[2], Shutter[i].tilt_config[1]-Shutter[i].tilt_config[0]);
      //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: real %d, start %d, counter %d,freq_max %d, dir %d, freq %d"),Shutter[i].real_position, Shutter[i].start_position ,RtcSettings.pulse_counter[i],ShutterGlobal.open_velocity_max , direction ,ShutterGlobal.open_velocity_max );
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: VenetianDelay: %d, Pos: %d, Dir: %d, Delta: %d, Dur: %d, StartP: %d, TgtP: %d"),
        Shutter[i].venetian_delay, Shutter[i].tilt_real_pos,direction,(Shutter[i].tilt_config[1]-Shutter[i].tilt_config[0]), Shutter[i].tilt_config[2],Shutter[i].tilt_start_pos,Shutter[i].tilt_target_pos);
    }

    // avoid file system writes during move to minimize missing steps
    if (Settings->save_data) {
      uint32_t move_duration = (direction > 0) ? Shutter[i].open_time : Shutter[i].close_time;
      TasmotaGlobal.save_data_counter = Settings->save_data + (move_duration / 10) +1;
    }
  }
  //AddLog(LOG_LEVEL_DEBUG,  PSTR("SHT: Start shtr%d from %d to %d in dir: %d"), i, Shutter[i].start_position, Shutter[i].target_position, direction);

  Shutter[i].direction = direction; // Last action. This causes RTC to start.
}

int32_t ShutterCalculatePosition(uint32_t i)
{
  // No Logging allowed. Part of RTC Timer
  if (Shutter[i].direction != 0) {
    switch (ShutterGlobal.position_mode) {
      case SHT_COUNTER:
        return ((int64_t)RtcSettings.pulse_counter[i]*Shutter[i].direction*STEPS_PER_SECOND * RESOLUTION / ShutterGlobal.open_velocity_max)+Shutter[i].start_position;
        break;
      case SHT_TIME:
      case SHT_TIME_UP_DOWN:
      case SHT_TIME_GARAGE:
        if (Shutter[i].tilt_config[2] > 0) {
          if (Shutter[i].time <= Shutter[i].venetian_delay+Shutter[i].motordelay) {
            Shutter[i].tilt_real_pos = (Shutter[i].tilt_start_pos + ((Shutter[i].direction * (int16_t)(Shutter[i].time - tmin(Shutter[i].motordelay, Shutter[i].time)) * (Shutter[i].tilt_config[1]-Shutter[i].tilt_config[0])) / Shutter[i].tilt_config[2]));
          } else {
            Shutter[i].tilt_real_pos = Shutter[i].direction == 1 ? Shutter[i].tilt_config[1] : Shutter[i].tilt_config[0];
          }
        }
        return Shutter[i].start_position + ( (Shutter[i].time - tmin(Shutter[i].venetian_delay+Shutter[i].motordelay, Shutter[i].time)) * (Shutter[i].direction > 0 ? RESOLUTION : -Shutter[i].close_velocity));
        break;
      case SHT_PWM_TIME:
        break;
      case SHT_PWM_VALUE:
        return Shutter[i].real_position;
      break;
      default:
        break;
    }
  } else {
    return Shutter[i].real_position;
  }
  return 0;  // Never reaches here, Satisfy compiler
}

void ShutterRelayChanged(void)
{

  // ShutterGlobal.RelayCurrentMask = binary relay that was recently changed and cause an Action
  // powerstate_local = binary powermatrix and relays from shutter: 0..3
  // relays_changed = bool if one of the relays that belong to the shutter changed not by shutter or pulsetimer
  char stemp1[10];

	for (uint32_t i = 0; i < TasmotaGlobal.shutters_present; i++) {
		power_t powerstate_local = (TasmotaGlobal.power >> (Settings->shutter_startrelay[i] -1)) & 3;
    // SRC_IGNORE added because INTERLOCK function bite causes this as last source for changing the relay.
		//uint8   manual_relays_changed = ((ShutterGlobal.RelayCurrentMask >> (Settings->shutter_startrelay[i] -1)) & 3) && SRC_IGNORE != TasmotaGlobal.last_source && SRC_SHUTTER != TasmotaGlobal.last_source && SRC_PULSETIMER != TasmotaGlobal.last_source ;
    uint8   manual_relays_changed = ((ShutterGlobal.RelayCurrentMask >> (Settings->shutter_startrelay[i] -1)) & 3) && SRC_SHUTTER != TasmotaGlobal.last_source && SRC_PULSETIMER != TasmotaGlobal.last_source ;
    //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Shtr%d, Source %s, Powerstate %ld, RelayMask %d, ManualChange %d"),
    //  i+1, GetTextIndexed(stemp1, sizeof(stemp1), TasmotaGlobal.last_source, kCommandSource), powerstate_local,ShutterGlobal.RelayCurrentMask,manual_relays_changed);
    if (manual_relays_changed) {
      //ShutterGlobal.skip_relay_change = true;
      ShutterLimitRealAndTargetPositions(i);
      switch (Shutter[i].switch_mode ) {
        case SHT_PULSE:
          if (Shutter[i].direction != 0 && powerstate_local) {
            Shutter[i].target_position = Shutter[i].real_position;
            powerstate_local = 0;
  					AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Shtr%d, Switch OFF motor. Target %ld, Source %s, Powerstate %ld, RelayMask %d, ManualChange %d"),
              i+1, Shutter[i].target_position, GetTextIndexed(stemp1, sizeof(stemp1), TasmotaGlobal.last_source, kCommandSource), powerstate_local,ShutterGlobal.RelayCurrentMask,manual_relays_changed);
          }
        break;
        default:
          TasmotaGlobal.last_source = SRC_SHUTTER; // avoid switch off in the next loop
          if (Shutter[i].direction != 0 ) Shutter[i].target_position = Shutter[i].real_position;
      }
      if (powerstate_local > 0) {
        Shutter[i].tiltmoving = 0;
      }
      switch (ShutterGlobal.position_mode) {
          // enum Shutterposition_mode {SHT_TIME, SHT_TIME_UP_DOWN, SHT_TIME_GARAGE, SHT_COUNTER, SHT_PWM_VALUE, SHT_PWM_TIME,};
          case SHT_TIME_UP_DOWN:
          case SHT_COUNTER:
          case SHT_PWM_VALUE:
          case SHT_PWM_TIME:
	     ShutterPowerOff(i);
          case SHT_TIME: {
            // powerstate_local == 0 => direction=0, stop
            // powerstate_local == 1 => direction=1, target=Shutter[i].open_max
            // powerstate_local == 2 => direction=-1, target=0 // only happen on SHT_TIME
            // powerstate_local == 3 => direction=-1, target=0 // only happen if NOT SHT_TIME
            int8_t direction = (powerstate_local == 0) ? 0 : (powerstate_local == 1) ? 1 : -1;
            int32_t target =    (powerstate_local == 1) ? Shutter[i].open_max : 0;

            if (direction != 0) {
              ShutterStartInit(i, direction, target);
            } else {
              Shutter[i].target_position = Shutter[i].real_position;
              Shutter[i].last_stop_time = millis();
            }
            break;
          }
          case SHT_TIME_GARAGE:
            switch (powerstate_local) {
              case 1:
                ShutterStartInit(i, Shutter[i].lastdirection * -1, Shutter[i].lastdirection == 1 ? 0 : Shutter[i].open_max);
                AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Shtr%d Garage. NewTarget %d"), i, Shutter[i].target_position);
                break;
              default:
                Shutter[i].target_position = Shutter[i].real_position;
            }
        } 
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Shtr%d, Target %ld, Power: %d, tiltmv: %d"), i+1, Shutter[i].target_position, powerstate_local,Shutter[i].tiltmoving);
		 } // if (manual_relays_changed)
  } // for (uint32_t i = 0; i < TasmotaGlobal.shutters_present; i++)
}

bool ShutterButtonIsSimultaneousHold(uint32_t button_index, uint32_t shutter_index)
{
  // check for simultaneous shutter button hold
  uint32 min_shutterbutton_hold_timer = -1; // -1 == max(uint32)
  for (uint32_t i = 0; i < MAX_SHUTTER_KEYS; i++) {
    if ((button_index != i) && (Settings->shutter_button[i] & (1<<31)) && ((Settings->shutter_button[i] & 0x03) == shutter_index) && (Button.hold_timer[i] < min_shutterbutton_hold_timer))
      min_shutterbutton_hold_timer = Button.hold_timer[i];
  }
  return ((-1 != min_shutterbutton_hold_timer) && (min_shutterbutton_hold_timer > (Button.hold_timer[button_index]>>1)));
}

bool ShutterButtonHandler(void)
{
  uint8_t  buttonState = SHT_NOT_PRESSED;
  uint8_t  button = XdrvMailbox.payload;
  uint8_t  press_index;
  uint32_t button_index = XdrvMailbox.index;
  uint8_t  shutter_index = Settings->shutter_button[button_index] & 0x03;
  uint16_t loops_per_second = 1000 / Settings->button_debounce;  // ButtonDebounce (50)

  if ((PRESSED == button) && (NOT_PRESSED == Button.last_state[button_index])) {
    if (Settings->flag.button_single) {                   // SetOption13 (0) - Allow only single button press for immediate action
        buttonState = SHT_PRESSED_MULTI;
        press_index = 1;
    } else {
      if ((Shutter[shutter_index].direction) && (Button.press_counter[button_index]==0)) {
        buttonState = SHT_PRESSED_IMMEDIATE;
        press_index = 1;
        Button.press_counter[button_index] = 99; // Remember to discard further action for press & hold within button timings
      } else {
        Button.press_counter[button_index] = (Button.window_timer[button_index]) ? Button.press_counter[button_index] +1 : 1;
        // Button.window_timer[button_index] = (Button.press_counter[button_index]==1) ? loops_per_second / 2 : loops_per_second;  // 0.5 second multi press window after 1st press, 1s afterwards
        Button.window_timer[button_index] = (loops_per_second >> 2) * 3; // 0.75 second multi press window
      }
    }
    TasmotaGlobal.blinks = 201;
  }

  if (NOT_PRESSED == button) {
    //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Shtr%d, Button %d, hold %d, dir %d, index %d, payload %d"), shutter_index+1, button_index+1, Button.hold_timer[button_index],Shutter[shutter_index].direction,XdrvMailbox.index,XdrvMailbox.payload);
    if (Shutter[shutter_index].direction 
        && (Button.hold_timer[button_index] > 0 
            && (!Settings->flag.button_single 
                || Button.hold_timer[button_index] > 20))
        && !(Settings->shutter_button[button_index] & (0x01<<29))) {
      XdrvMailbox.index = shutter_index +1;
      XdrvMailbox.payload = XdrvMailbox.index;
      //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Shtr%d, Button %d, hold %d, dir %d, index %d, payload %d"), shutter_index+1, button_index+1, Button.hold_timer[button_index],Shutter[shutter_index].direction,XdrvMailbox.index,XdrvMailbox.payload);
      CmndShutterStop();
    }
    Button.hold_timer[button_index] = 0;
  } else {
    Button.hold_timer[button_index]++;
    if (!Settings->flag.button_single) {               // SetOption13 (0) - Allow only single button press for immediate action
      if (Settings->param[P_HOLD_IGNORE] > 0) {        // SetOption40 (0) - Do not ignore button hold
        if (Button.hold_timer[button_index] > loops_per_second * Settings->param[P_HOLD_IGNORE] / 10) {
          Button.hold_timer[button_index] = 0;         // Reset button hold counter to stay below hold trigger
          Button.press_counter[button_index] = 0;      // Discard button press to disable functionality
        }
      }
      if ((Button.press_counter[button_index]<99) && (Button.hold_timer[button_index] == loops_per_second * Settings->param[P_HOLD_TIME] / 10)) {  // press still valid && SetOption32 (40) - Button hold
        // check for simultaneous shutter button hold
        if (ShutterButtonIsSimultaneousHold(button_index, shutter_index)) {
          // simultaneous shutter button hold detected
          for (uint32_t i = 0; i < MAX_SHUTTER_KEYS; i++)
            if ((Settings->shutter_button[i] & (1<<31)) && ((Settings->shutter_button[i] & 0x03) == shutter_index))
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
      if ((Button.press_counter[button_index]==0) && (Button.hold_timer[button_index] == loops_per_second * IMMINENT_RESET_FACTOR * Settings->param[P_HOLD_TIME] / 10)) {  // SetOption32 (40) - Button held for factor times longer
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

  if (!Settings->flag.button_single) {                    // SetOption13 (0) - Allow multi-press
    if (Button.window_timer[button_index]) {
      Button.window_timer[button_index]--;
    } else {
      if (!TasmotaGlobal.restart_flag && !Button.hold_timer[button_index] && (Button.press_counter[button_index] > 0)) {
        if (Button.press_counter[button_index]<99) {
          // check for simultaneous shutter button press
          uint32 min_shutterbutton_press_counter = -1; // -1 == max(uint32)
          for (uint32_t i = 0; i < MAX_SHUTTER_KEYS; i++) {
            AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: ShutterButton[i] %ld, ShutterIndex %d, ButtonPressCounter[i] %d, minShutterButtonPressCounter %d, i %d"),
              Settings->shutter_button[i], shutter_index, Button.press_counter[i] , min_shutterbutton_press_counter, i);
            if ((button_index != i) && (Settings->shutter_button[i] & (1<<31)) && ((Settings->shutter_button[i] & 0x03) == shutter_index) && (i != button_index) && (Button.press_counter[i] < min_shutterbutton_press_counter)) {
              min_shutterbutton_press_counter = Button.press_counter[i];
              AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: minShutterButtonPressCounter %d"), min_shutterbutton_press_counter);
            }
          }
          if (min_shutterbutton_press_counter == Button.press_counter[button_index]) {
            // simultaneous shutter button press detected
            AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Simultanous press detected"));
            press_index = Button.press_counter[button_index];
            for (uint32_t i = 0; i < MAX_SHUTTER_KEYS; i++)
              if ((Settings->shutter_button[i] & (1<<31)) && ((Settings->shutter_button[i] & 0x03) != shutter_index))
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
    if ((!Settings->flag.button_restrict) && (((press_index>=5) && (press_index<=7)) || (buttonState == SHT_PRESSED_EXT_HOLD) || (buttonState == SHT_PRESSED_EXT_HOLD_SIMULTANEOUS))){
      // check number of buttons for this shutter
      uint8_t shutter_index_num_buttons = 0;
      for (uint32_t i = 0; i < MAX_SHUTTER_KEYS; i++) {
        if ((Settings->shutter_button[i] & (1<<31)) && ((Settings->shutter_button[i] & 0x03) == shutter_index)) {
          shutter_index_num_buttons++;
        }
      }
      if ((buttonState == SHT_PRESSED_MULTI_SIMULTANEOUS) || ((shutter_index_num_buttons==1) && (buttonState == SHT_PRESSED_MULTI))){
        // 5x..7x && no SetOption1 (0) checked above
        // simultaneous or stand alone button press 5x, 6x, 7x detected
        char scmnd[20];
        snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_WIFICONFIG " 2"));
        ExecuteCommand(scmnd, SRC_BUTTON);
        return true;
      } else if ((buttonState == SHT_PRESSED_EXT_HOLD_SIMULTANEOUS) || ((shutter_index_num_buttons==1) && (buttonState == SHT_PRESSED_EXT_HOLD))){
        // no SetOption1 (0) checked above
        // simultaneous or stand alone button extended hold detected
        char scmnd[20];
        snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_RESET " 1"));
        ExecuteCommand(scmnd, SRC_BUTTON);
        return true;
      }
    }
    if (buttonState <= SHT_PRESSED_IMMEDIATE) {
      if (Settings->shutter_startrelay[shutter_index] && Settings->shutter_startrelay[shutter_index] <9) {
        uint8_t pos_press_index = (buttonState == SHT_PRESSED_HOLD) ? 3 : (press_index-1);
        if (pos_press_index>3) pos_press_index=3;
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Shtr%d, Button %d = %d (single=1, double=2, tripple=3, hold=4)"), shutter_index+1, button_index+1, pos_press_index+1);
        XdrvMailbox.index = shutter_index +1;
        TasmotaGlobal.last_source = SRC_BUTTON;
        XdrvMailbox.data_len = 0;
        char databuf[1] = "";
        XdrvMailbox.data = databuf;
        XdrvMailbox.command = NULL;
        if (buttonState == SHT_PRESSED_IMMEDIATE) {
          XdrvMailbox.payload = XdrvMailbox.index;
          CmndShutterStop();
        } else {
          uint8_t position = (Settings->shutter_button[button_index]>>(6*pos_press_index + 2)) & 0x03f;
          if (position) {
            if (Shutter[shutter_index].direction) {
              XdrvMailbox.payload = XdrvMailbox.index;
              CmndShutterStop();
            } else {
              XdrvMailbox.payload = position = (position-1)<<1;
              //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Shtr%d -> %d"), shutter_index+1, position);
              if (102 == position) {
                XdrvMailbox.payload = XdrvMailbox.index;
                CmndShutterToggle();
              } else {
		if (position == ShutterRealToPercentPosition(Shutter[XdrvMailbox.index-1].real_position, XdrvMailbox.index-1) ) {
                  Shutter[XdrvMailbox.index -1].tilt_target_pos = position==0? Shutter[XdrvMailbox.index -1].tilt_config[0]:(position==100?Shutter[XdrvMailbox.index -1].tilt_config[1]:Shutter[XdrvMailbox.index -1].tilt_target_pos);
                  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Shtr%d -> Endpoint movement detected at %d. Set Tilt: %d"), shutter_index+1, position, Shutter[XdrvMailbox.index -1].tilt_target_pos);
                }
                CmndShutterPosition();
              }
              if (Settings->shutter_button[button_index] & ((0x01<<26)<<pos_press_index)) {
                // MQTT broadcast to grouptopic
                char scommand[CMDSZ];
                char stopic[TOPSZ];
                for (uint32_t i = 0; i < MAX_SHUTTERS; i++) {
                  if ( ((i==shutter_index) || (Settings->shutter_button[button_index] & (0x01<<30))) && 0 == (Settings->shutter_options[i] & 2) ) {
                    snprintf_P(scommand, sizeof(scommand),PSTR("ShutterPosition%d"), i+1);
                    GetGroupTopic_P(stopic, scommand, SET_MQTT_GRP_TOPIC);
                    Response_P("%d", position);
                    MqttPublish(stopic, false);
                  }
                } // for (uint32_t)
              } // if (Settings->shutter)
            } // ende else
          } // if (position)
        } // end else
      } // if   if (Settings->shutter_startrelay[shutter_index]
    }
    Response_P(PSTR("{"));
    ResponseAppend_P(JSON_SHUTTER_BUTTON, shutter_index+1, (buttonState <= SHT_PRESSED_EXT_HOLD) ? (button_index+1) : 0, press_index);
    ResponseJsonEnd();
    MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR(D_PRFX_SHUTTER));
  }
  return true;
}

void ShutterSetPosition(uint32_t device, uint32_t position)
{
  char svalue[32];                   // Command and number parameter
  snprintf_P(svalue, sizeof(svalue), PSTR(D_PRFX_SHUTTER D_CMND_SHUTTER_POSITION "%d %d"), device, position);
  ExecuteCommand(svalue, SRC_SHUTTER);
}

void ShutterToggle(bool dir)
{
  AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Toggle: %d, i %d, dir %d, lastdir %d"), XdrvMailbox.payload, XdrvMailbox.index, dir, Shutter[XdrvMailbox.index-1].lastdirection);
  if ((1 == XdrvMailbox.index) && (XdrvMailbox.payload != -99)) {
    XdrvMailbox.index = XdrvMailbox.payload;
  }
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    uint32_t index = XdrvMailbox.index-1;
    if (dir) {
      XdrvMailbox.payload = (Shutter[index].direction==0 ? ((Shutter[index].lastdirection > 0) ? 0 : 100) : (Shutter[index].direction > 0) ? 0 : 100);
    }
    else {
      XdrvMailbox.payload = (50 < ShutterRealToPercentPosition(Shutter[index].real_position, index)) ? 0 : 100;
    }
    XdrvMailbox.data_len = 0;
    TasmotaGlobal.last_source = SRC_WEBGUI;
    CmndShutterPosition();
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndShutterOpen(void)
{
  //AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Payload open: %d, i %d"), XdrvMailbox.payload, XdrvMailbox.index);
  if ((!XdrvMailbox.usridx) && (XdrvMailbox.payload != -99)) {
    XdrvMailbox.index = XdrvMailbox.payload;
  }
  XdrvMailbox.payload = 100;
  TasmotaGlobal.last_source = SRC_WEBGUI;
  CmndShutterPosition();
}

void CmndShutterStopOpen(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    uint32_t index = XdrvMailbox.index-1;
    if (Shutter[index].direction) {
      CmndShutterStop();
    } else {
      CmndShutterOpen();
    }
  }
}

void CmndShutterClose(void)
{
  //AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Payload close: %d, i %d"), XdrvMailbox.payload, XdrvMailbox.index);
  if ((!XdrvMailbox.usridx) && (XdrvMailbox.payload != -99)) {
    XdrvMailbox.index = XdrvMailbox.payload;
  }
  XdrvMailbox.payload = 0;
  TasmotaGlobal.last_source = SRC_WEBGUI;
  CmndShutterPosition();
}

void CmndShutterStopClose(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    uint32_t index = XdrvMailbox.index-1;
    if (Shutter[index].direction) {
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
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    uint32_t index = XdrvMailbox.index-1;
    if (Shutter[index].direction) {
      CmndShutterStop();
    } else {
      CmndShutterToggle();
    }
  }
}

void CmndShutterStopToggleDir(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    uint32_t index = XdrvMailbox.index-1;
    if (Shutter[index].direction) {
      CmndShutterStop();
    } else {
      CmndShutterToggleDir();
    }
  }
}

void CmndShutterStop(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    //AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Try Stop %d: dir: %d"), XdrvMailbox.index, Shutter[XdrvMailbox.index -1].direction);
    if (!(Settings->shutter_options[XdrvMailbox.index-1] & 2)) {
      if ((!XdrvMailbox.usridx) && (XdrvMailbox.payload != -99)) {
        XdrvMailbox.index = XdrvMailbox.payload;
      }
      uint32_t i = XdrvMailbox.index -1;
      if (Shutter[i].direction != 0) {

        AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Stop %d: dir: %d"), XdrvMailbox.index, Shutter[i].direction);
        Shutter[i].target_position = Shutter[i].real_position;
        TasmotaGlobal.last_source = SRC_SHUTTER;
      }
      if (XdrvMailbox.command)
        ResponseCmndDone();
        ShutterUpdatePosition();
    } else {
      if (XdrvMailbox.command)
        ResponseCmndIdxChar("Locked");
    }
  }
}

void CmndShutterIncDec(void)
{
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Change in: payload %s (%d), payload %d, idx %d, src %d"), XdrvMailbox.data , XdrvMailbox.data_len, XdrvMailbox.payload , XdrvMailbox.index, TasmotaGlobal.last_source );
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    if (XdrvMailbox.data_len > 0) {
      XdrvMailbox.payload =  ShutterRealToPercentPosition(Shutter[XdrvMailbox.index-1].target_position, XdrvMailbox.index-1)+XdrvMailbox.payload;
      // limit position to boundaries
      XdrvMailbox.payload = XdrvMailbox.payload < 0 ? 0 : (XdrvMailbox.payload > 100 ? 100 : XdrvMailbox.payload);
      CmndShutterPosition();
    }
  }
}

void CmndShutterPosition(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    if (!(Settings->shutter_options[XdrvMailbox.index-1] & 2)) {
      uint32_t index = XdrvMailbox.index-1;
      //limit the payload
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Pos. payload <%s> (%d), payload %d, idx %d (%d), src %d"), XdrvMailbox.data , XdrvMailbox.data_len, XdrvMailbox.payload , XdrvMailbox.index, XdrvMailbox.usridx, TasmotaGlobal.last_source );

      if (XdrvMailbox.data_len >= 3) {
        // check if input is of format "position,tilt"
        uint32_t i = 0;
        char *str_ptr;
        char data_copy[strlen(XdrvMailbox.data) +1];
        strncpy(data_copy, XdrvMailbox.data, sizeof(data_copy));  // Duplicate data as strtok_r will modify it.
        // Loop through the data string, splitting on ',' seperators.
        for (char *str = strtok_r(data_copy, ",", &str_ptr); str && i < 2; str = strtok_r(nullptr, ",", &str_ptr), i++) {
          switch(i) {
            case 0:
              XdrvMailbox.payload = atoi(str);
              break;
            case 1:
              Shutter[index].tilt_target_pos_override = atoi(str);
              break;
          }
        }
      }

      // value 0 with data_len > 0 can mean Open
      // special handling fo UP,DOWN,TOGGLE,STOP and similar commands command 
      // 
      if ( XdrvMailbox.data_len > 0 ) {
        // set len to 0 to avoid loop 
        uint32_t data_len_save = XdrvMailbox.data_len;
        int32_t  payload_save  = XdrvMailbox.payload;
        XdrvMailbox.data_len   = 0;
        XdrvMailbox.payload    = -99;
	      
        if (!strcasecmp(XdrvMailbox.data,D_CMND_SHUTTER_UP) || !strcasecmp(XdrvMailbox.data,D_CMND_SHUTTER_OPEN) || ((Shutter[index].direction==0) && !strcasecmp(XdrvMailbox.data,D_CMND_SHUTTER_STOPOPEN))) {
          CmndShutterOpen();
          return;
        }
        if (!strcasecmp(XdrvMailbox.data,D_CMND_SHUTTER_DOWN) || !strcasecmp(XdrvMailbox.data,D_CMND_SHUTTER_CLOSE) || ((Shutter[index].direction==0) && !strcasecmp(XdrvMailbox.data,D_CMND_SHUTTER_STOPCLOSE))) {
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
        if (!strcasecmp(XdrvMailbox.data,D_CMND_SHUTTER_STOP) || ((Shutter[index].direction) && (!strcasecmp(XdrvMailbox.data,D_CMND_SHUTTER_STOPOPEN) || !strcasecmp(XdrvMailbox.data,D_CMND_SHUTTER_STOPCLOSE)))) {
          CmndShutterStop();
          return;
        }
        // restore values
        XdrvMailbox.payload  = payload_save;
        XdrvMailbox.data_len = data_len_save;
      }

      int8_t target_pos_percent = (XdrvMailbox.payload < 0) ? (XdrvMailbox.payload == -99 ? ShutterRealToPercentPosition(Shutter[index].real_position, index) : 0) : ((XdrvMailbox.payload > 100) ? 100 : XdrvMailbox.payload);
      target_pos_percent = ((Settings->shutter_options[index] & 1) && (    (SRC_SERIAL     != TasmotaGlobal.last_source)
                                                                        && (SRC_WEBGUI     != TasmotaGlobal.last_source)
                                                                        && (SRC_WEBCOMMAND != TasmotaGlobal.last_source)
                                                                      )) ? 100 - target_pos_percent : target_pos_percent;
	    
      // if position is either 0 or 100 reset the tilt to avoid tilt moving at the end
      if (target_pos_percent ==   0 && ShutterRealToPercentPosition(Shutter[index].real_position, index)  > 0  ) {Shutter[index].tilt_target_pos = Shutter[index].tilt_config[4];}
      if (target_pos_percent == 100 && ShutterRealToPercentPosition(Shutter[index].real_position, index)  < 100) {Shutter[index].tilt_target_pos = Shutter[index].tilt_config[3];}
       
      // manual override of tiltposition
      if (Shutter[index].tilt_target_pos_override != -128) {
        Shutter[index].tilt_target_pos = tmin(tmax( Shutter[index].tilt_config[0],Shutter[index].tilt_target_pos_override ), Shutter[index].tilt_config[1]);
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Override tilt set: %d --> %d"),Shutter[index].tilt_target_pos_override, Shutter[index].tilt_target_pos);
        Shutter[index].tilt_target_pos_override = -128;
      }

      if (XdrvMailbox.payload != -99) {
        //target_pos_percent = (Settings->shutter_options[index] & 1) ? 100 - target_pos_percent : target_pos_percent;
        Shutter[index].target_position = ShutterPercentToRealPosition(target_pos_percent, index);
        //Shutter[i].accelerator[index] = ShutterGlobal.open_velocity_max / ((Shutter[i].motordelay[index] > 0) ? Shutter[i].motordelay[index] : 1);
        //Shutter[i].target_position[index] = XdrvMailbox.payload < 5 ?  Settings->shuttercoeff[2][index] * XdrvMailbox.payload : Settings->shuttercoeff[1][index] * XdrvMailbox.payload + Settings->shuttercoeff[0,index];
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: lastsource %d:, real %d, target %d, tiltreal: %d, tilttarget: %d, payload %d"), TasmotaGlobal.last_source, Shutter[index].real_position ,Shutter[index].target_position,Shutter[index].tilt_real_pos, Shutter[index].tilt_target_pos,target_pos_percent);
      }
      if ( (target_pos_percent >= 0) && (target_pos_percent <= 100) &&
           (abs(Shutter[index].target_position - Shutter[index].real_position ) > Shutter[index].min_realPositionChange ||
            abs(Shutter[index].tilt_target_pos - Shutter[index].tilt_real_pos ) > Shutter[index].min_TiltChange) ) {
        if (Settings->shutter_options[index] & 4) {
          if (0   == target_pos_percent && Shutter[index].real_position > 0) Shutter[index].target_position -= 1 * RESOLUTION * STEPS_PER_SECOND;
          if (100 == target_pos_percent && Shutter[index].real_position < Shutter[index].open_max) Shutter[index].target_position += 1 * RESOLUTION * STEPS_PER_SECOND;
        }
        int8_t new_shutterdirection;
        if (abs(Shutter[index].target_position - Shutter[index].real_position ) > Shutter[index].min_realPositionChange) {
          new_shutterdirection = Shutter[index].real_position < Shutter[index].target_position ? 1 : -1;
          Shutter[index].tiltmoving = 0;
        } else {
          new_shutterdirection = Shutter[index].tilt_real_pos < Shutter[index].tilt_target_pos ? 1 : -1;
          Shutter[index].tiltmoving = 1;
        }

        if (Shutter[index].direction == -new_shutterdirection) {
          //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Stop shutter to reverse direction"));
          ShutterPowerOff(index);
        }
        if (Shutter[index].direction != new_shutterdirection) {
          ShutterStartInit(index, new_shutterdirection, Shutter[index].target_position);
          uint8_t save_direction = Shutter[index].direction;
          Shutter[index].direction = 0; // set temporary direction = 0 to avoid RTC timer sarting. Some delay may happen before shutter starts moving
          switch (ShutterGlobal.position_mode) {
            case SHT_COUNTER:
            case SHT_PWM_TIME:
            case SHT_PWM_VALUE:
            case SHT_TIME_UP_DOWN:
              if (!ShutterGlobal.skip_relay_change) {
                // Code for shutters with circuit safe configuration, switch the direction Relay
                ExecuteCommandPowerShutter(Settings->shutter_startrelay[index] +1, new_shutterdirection == 1 ? 0 : 1, SRC_SHUTTER);
                delay(SHUTTER_RELAY_OPERATION_TIME);
                // power on
                ExecuteCommandPowerShutter(Settings->shutter_startrelay[index], 1, SRC_SHUTTER);
              }
              //if (ShutterGlobal.position_mode != SHT_TIME_UP_DOWN) ExecuteCommandPowerShutter(Settings->shutter_startrelay[index]+2, 1, SRC_SHUTTER);
            break;
            case SHT_TIME:
              if (!ShutterGlobal.skip_relay_change) {
                if ( (TasmotaGlobal.power >> (Settings->shutter_startrelay[index] -1)) & 3 > 0 ) {
                  ExecuteCommandPowerShutter(Settings->shutter_startrelay[index] + (new_shutterdirection == 1 ? 1 : 0), Shutter[index].switch_mode == SHT_SWITCH ? 0 : 1, SRC_SHUTTER);
                }
                ExecuteCommandPowerShutter(Settings->shutter_startrelay[index] + (new_shutterdirection == 1 ? 0 : 1), 1, SRC_SHUTTER);
              }
            break;
            case SHT_TIME_GARAGE:
              if (!ShutterGlobal.skip_relay_change) {
                if (new_shutterdirection == Shutter[index].lastdirection) {
                  AddLog(LOG_LEVEL_INFO, PSTR("SHT: Garage not move in this direction: %d"), Shutter[index].switch_mode == SHT_PULSE);
                  for (uint8_t k=0 ; k <= (uint8_t)(Shutter[index].switch_mode == SHT_PULSE) ; k++) {
                    ExecuteCommandPowerShutter(Settings->shutter_startrelay[index], 1, SRC_SHUTTER);
                    ShutterWaitForMotorStop(index);
                    ExecuteCommandPowerShutter(Settings->shutter_startrelay[index], 0, SRC_SHUTTER);
                    ShutterWaitForMotorStop(index);
                  }
                  // reset shutter time to avoid 2 seconds above count as runtime
                  Shutter[index].time = 0;
                } // if (new_shutterdirection == Shutter[i].lastdirection[index])
                ExecuteCommandPowerShutter(Settings->shutter_startrelay[index], 1, SRC_SHUTTER);
              } // if (!ShutterGlobal.skip_relay_change)
            break;
          } // switch (ShutterGlobal.position_mode)
          Shutter[index].direction = save_direction;
          ShutterGlobal.RelayCurrentMask = 0;
        } // if (Shutter[i].direction[index] != new_shutterdirection)
      } else {
        target_pos_percent = ShutterRealToPercentPosition(Shutter[index].real_position, index);
      }
      index = (!XdrvMailbox.usridx && !XdrvMailbox.data_len)?MAX_SHUTTERS:index;
      ShutterReportPosition(true, index);
      ShutterGlobal.start_reported = 1;
      XdrvMailbox.index = index +1;  // Fix random index for ShutterClose
    } else {
      ShutterReportPosition(true, MAX_SHUTTERS);
      if (XdrvMailbox.command)
        ResponseCmndIdxChar("Locked");
    }
  }
}

void CmndShutterStopPosition(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    uint32_t index = XdrvMailbox.index-1;
    if (Shutter[index].direction) {
      XdrvMailbox.payload = -99;
      CmndShutterStop();
    } else {
      CmndShutterPosition();
    }
  }
}

void CmndShutterOpenTime(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    if (XdrvMailbox.data_len > 0) {
      Settings->shutter_opentime[XdrvMailbox.index -1] = (uint16_t)(10 * CharToFloat(XdrvMailbox.data));
      ShutterInit();
    }
/*
    char time_chr[10];
    dtostrfd((float)(Settings->shutter_opentime[XdrvMailbox.index -1]) / 10, 1, time_chr);
    ResponseCmndIdxChar(time_chr);
*/
    ResponseCmndIdxFloat((float)(Settings->shutter_opentime[XdrvMailbox.index -1]) / 10, 1);
  }
}

void CmndShutterCloseTime(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    if (XdrvMailbox.data_len > 0) {
      Settings->shutter_closetime[XdrvMailbox.index -1] = (uint16_t)(10 * CharToFloat(XdrvMailbox.data));
      ShutterInit();
    }
/*
    char time_chr[10];
    dtostrfd((float)(Settings->shutter_closetime[XdrvMailbox.index -1]) / 10, 1, time_chr);
    ResponseCmndIdxChar(time_chr);
*/
    ResponseCmndIdxFloat((float)(Settings->shutter_closetime[XdrvMailbox.index -1]) / 10, 1);
  }
}

void CmndShutterMotorDelay(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    if (XdrvMailbox.data_len > 0) {
      Settings->shutter_motordelay[XdrvMailbox.index -1] = (uint8_t)(STEPS_PER_SECOND * CharToFloat(XdrvMailbox.data));
      ShutterInit();
      //AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Shtr Init1. realdelay %d"),Shutter[XdrvMailbox.index -1].motordelay);
    }
/*
    char time_chr[10];
    dtostrfd((float)(Shutter[XdrvMailbox.index -1].motordelay) / STEPS_PER_SECOND, 2, time_chr);
    ResponseCmndIdxChar(time_chr);
*/
    ResponseCmndIdxFloat((float)(Shutter[XdrvMailbox.index -1].motordelay) / STEPS_PER_SECOND, 2);
  }
}

void CmndShutterMode(void)
{
  if (!XdrvMailbox.usridx) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= MAX_MODES)) {
      Settings->shutter_mode =  ShutterGlobal.position_mode =  XdrvMailbox.payload;
      ShutterInit();
    }
    ResponseCmndNumber(ShutterGlobal.position_mode);
  }
}

void CmndShutterRelay(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 32) && (XdrvMailbox.index <= MAX_SHUTTERS)) {
    //Settings->shutter_startrelay[XdrvMailbox.index -1] = XdrvMailbox.payload;
    if (XdrvMailbox.payload > 0) {
      ShutterGlobal.RelayShutterMask |= 3 << (XdrvMailbox.payload - 1);
    } else {
      ShutterGlobal.RelayShutterMask ^= 3 << (Settings->shutter_startrelay[XdrvMailbox.index -1] - 1);
    }
    AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: relold:%d index:%d, mode:%d, relaymask: %ld"),
    Settings->shutter_startrelay[XdrvMailbox.index -1] , XdrvMailbox.index ,Settings->shutter_mode, ShutterGlobal.RelayShutterMask );
    if (Settings->shutter_startrelay[XdrvMailbox.index -1] == 0 && XdrvMailbox.index == 1 && Settings->shutter_mode == SHT_UNDEF) {
      // first shutter was not defined, maybe init
      Settings->shutter_mode = SHT_AUTOCONFIG;
      AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Autoconfig"));
    }
    Settings->shutter_startrelay[XdrvMailbox.index -1] = XdrvMailbox.payload;

    ShutterInit();
    // if payload is 0 to disable the relay there must be a reboot. Otherwhise does not work
  }
  uint32_t start = (!XdrvMailbox.usridx && !XdrvMailbox.data_len)?0:XdrvMailbox.index -1;
  uint32_t end   = (!XdrvMailbox.usridx && !XdrvMailbox.data_len)?TasmotaGlobal.shutters_present:XdrvMailbox.index;
  // {"ShutterRelay1":"1","ShutterRelay2":"3","ShutterRelay3":"5"}
  Response_P(PSTR("{"));
  for (uint32_t i = start; i < end; i++) {
    ResponseAppend_P(PSTR("%s\"" D_PRFX_SHUTTER D_CMND_SHUTTER_RELAY "%d\":%d"), (i>start)?",":"", i+1, Settings->shutter_startrelay[i]);
  }
  ResponseAppend_P(PSTR("}"));
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
            for (uint32_t i=0 ; i < MAX_SHUTTER_KEYS ; i++)
              if ((Settings->shutter_button[i]&0x3) == (XdrvMailbox.index-1))
                Settings->shutter_button[i] = 0;
          } else {
            if (setting) {
              // anything was set
              setting |= (1<<31);
              setting |= (XdrvMailbox.index-1) & 0x3;
            }
            Settings->shutter_button[button_index-1] = setting;
          }
        }
      }
      char setting_chr[30*MAX_SHUTTER_KEYS] = "-", *setting_chr_ptr = setting_chr;
      for (uint32_t i=0 ; i < MAX_SHUTTER_KEYS ; i++) {
        setting = Settings->shutter_button[i];
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
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
      Settings->shutter_set50percent[XdrvMailbox.index -1] = (Settings->shutter_options[XdrvMailbox.index -1] & 1) ? 100 - XdrvMailbox.payload : XdrvMailbox.payload;
      Settings->shuttercoeff[0][XdrvMailbox.index -1] = 0;
      ShutterInit();
    }
  ResponseCmndIdxNumber((Settings->shutter_options[XdrvMailbox.index -1] & 1) ? 100 - Settings->shutter_set50percent[XdrvMailbox.index -1] : Settings->shutter_set50percent[XdrvMailbox.index -1]);
  }
}

void CmndShutterFrequency(void)
{
  if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= 20000)) {
    ShutterGlobal.open_velocity_max =  XdrvMailbox.payload;
    if (TasmotaGlobal.shutters_present < 4) {
      Settings->shuttercoeff[4][3] = ShutterGlobal.open_velocity_max;
    }
    ShutterInit();
  }
  ResponseCmndNumber(ShutterGlobal.open_velocity_max);
}

void CmndShutterSetClose(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    Shutter[XdrvMailbox.index -1].real_position = 0;
    Shutter[XdrvMailbox.index -1].tilt_real_pos = Shutter[XdrvMailbox.index -1].tilt_config[0];
    Shutter[XdrvMailbox.index -1].lastdirection = -1;
    ShutterStartInit(XdrvMailbox.index -1, 0, 0);
    Settings->shutter_position[XdrvMailbox.index -1] = 0;
    ResponseCmndIdxChar(D_CONFIGURATION_RESET);
  }
}

void CmndShutterSetOpen(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    Shutter[XdrvMailbox.index -1].real_position = Shutter[XdrvMailbox.index -1].open_max;
    Shutter[XdrvMailbox.index -1].tilt_real_pos = Shutter[XdrvMailbox.index -1].tilt_config[1];
    Shutter[XdrvMailbox.index -1].lastdirection = 1;
    ShutterStartInit(XdrvMailbox.index -1, 0, Shutter[XdrvMailbox.index -1].open_max);
    Settings->shutter_position[XdrvMailbox.index -1] = 100;
    ResponseCmndIdxChar(D_CONFIGURATION_RESET);
  }
}

void CmndShutterPwmRange(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    if (XdrvMailbox.data_len > 0) {
      uint8_t i = 0;
      char *str_ptr;

      char data_copy[strlen(XdrvMailbox.data) +1];
      strncpy(data_copy, XdrvMailbox.data, sizeof(data_copy));  // Duplicate data as strtok_r will modify it.
      // Loop through the data string, splitting on ' ' seperators.
      for (char *str = strtok_r(data_copy, " ,", &str_ptr); str && i < 2; str = strtok_r(nullptr, " ,", &str_ptr), i++) {
        uint16_t field = atoi(str);
        // The fields in a data string can only range from 1-30000.
        // and following value must be higher than previous one
        if ((field <= 0) || (field > 1023)) {
          break;
        }
        Settings->shutter_pwmrange[i][XdrvMailbox.index -1] = field;
      }
      AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Shtr%d Init1. pwmmin %d, pwmmax %d"), XdrvMailbox.index , Settings->shutter_pwmrange[0][XdrvMailbox.index -1], Settings->shutter_pwmrange[1][XdrvMailbox.index -1]);
      ShutterInit();
      ResponseCmndIdxChar(XdrvMailbox.data);
    } else {
      char setting_chr[30] = "0";
      snprintf_P(setting_chr, sizeof(setting_chr), PSTR("Shutter %d: min:%d max:%d"), XdrvMailbox.index, Settings->shutter_pwmrange[0][XdrvMailbox.index -1], Settings->shutter_pwmrange[1][XdrvMailbox.index -1]);
      ResponseCmndIdxChar(setting_chr);
    }
  }
}

void CmndShutterCalibration(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    if (XdrvMailbox.data_len > 0) {
      uint8_t i = 0;
      char *str_ptr;

      char data_copy[strlen(XdrvMailbox.data) +1];
      strncpy(data_copy, XdrvMailbox.data, sizeof(data_copy));  // Duplicate data as strtok_r will modify it.
      // Loop through the data string, splitting on ' ' seperators.
      for (char *str = strtok_r(data_copy, " ,", &str_ptr); str && i < 5; str = strtok_r(nullptr, " ,", &str_ptr), i++) {
        int field = atoi(str);
        // The fields in a data string can only range from 1-30000.
        // and following value must be higher than previous one
        if ((field <= 0) || (field > 30000) || ( (i>0) && (field <= messwerte[i-1]) ) ) {
          break;
        }
        messwerte[i] = field;
      }
      Settings->shutter_set50percent[XdrvMailbox.index -1] = 50;
      for (i = 0; i < 5; i++) {
        Settings->shuttercoeff[i][XdrvMailbox.index -1] = SHT_DIV_ROUND((uint32_t)messwerte[i] * 1000, messwerte[4]);
        AddLog(LOG_LEVEL_DEBUG, PSTR("SHT: Shuttercoeff %d, i %d, Value %d, MeasuredValue %d"), i,XdrvMailbox.index -1,Settings->shuttercoeff[i][XdrvMailbox.index -1], messwerte[i]);
      }
      ShutterInit();
      ResponseCmndIdxChar(XdrvMailbox.data);
    } else {
      char setting_chr[30] = "0";
      snprintf_P(setting_chr, sizeof(setting_chr), PSTR("%d %d %d %d %d"), Settings->shuttercoeff[0][XdrvMailbox.index -1], Settings->shuttercoeff[1][XdrvMailbox.index -1], Settings->shuttercoeff[2][XdrvMailbox.index -1], Settings->shuttercoeff[3][XdrvMailbox.index -1], Settings->shuttercoeff[4][XdrvMailbox.index -1]);
      ResponseCmndIdxChar(setting_chr);
    }
  }
}

void ShutterOptionsSetHelper(uint16_t option)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    if (XdrvMailbox.payload == 0) {
      Settings->shutter_options[XdrvMailbox.index -1] &= ~(option);
    } else if (XdrvMailbox.payload == 1) {
      Settings->shutter_options[XdrvMailbox.index -1] |= (option);
    }
    ResponseCmndIdxNumber((Settings->shutter_options[XdrvMailbox.index -1] & option) ? 1 : 0);
  }
}

void CmndShutterInvert(void)
{
  ShutterOptionsSetHelper(1);
}

void CmndShutterLock(void)
{
  ShutterOptionsSetHelper(2);
}

void CmndShutterEnableEndStopTime(void)
{
  ShutterOptionsSetHelper(4);
}

void CmndShutterInvertWebButtons(void)
{
  ShutterOptionsSetHelper(8);
}

void CmndShutterSetTilt(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    if (XdrvMailbox.payload != -99 ) {
      Shutter[XdrvMailbox.index -1].tilt_target_pos = tmin(tmax(XdrvMailbox.payload, Shutter[XdrvMailbox.index -1].tilt_config[0]), Shutter[XdrvMailbox.index -1].tilt_config[1]);
    }
    // assuming OPEN=100=tilt_config[3]/CLOSE=0=tilt_config[4]
    if (XdrvMailbox.data_len > 3 && XdrvMailbox.payload >= 0 ) {
      Shutter[XdrvMailbox.index -1].tilt_target_pos = Shutter[XdrvMailbox.index -1].tilt_config[XdrvMailbox.payload?3:4];
    }
  }
  XdrvMailbox.data[0] = '\0';
  AddLog(LOG_LEVEL_INFO, PSTR("SHT: TiltTarget %d, payload %d"), Shutter[XdrvMailbox.index -1].tilt_target_pos,XdrvMailbox.payload);
  Shutter[XdrvMailbox.index -1].tiltmoving = 1;
  // Avoid shutterposition try to interpret "open/close or payload"
  XdrvMailbox.data_len = 0;
  XdrvMailbox.payload = -99;
  CmndShutterPosition();
}

void CmndShutterTiltConfig(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present)) {
    if (XdrvMailbox.data_len > 0) {
      uint8_t i = 0;
      char *str_ptr;
      char data_copy[strlen(XdrvMailbox.data) +1];
      strncpy(data_copy, XdrvMailbox.data, sizeof(data_copy));  // Duplicate data as strtok_r will modify it.
      // Loop through the data string, splitting on ' ' seperators.
      for (char *str = strtok_r(data_copy, " ,", &str_ptr); str && i < 6; str = strtok_r(nullptr, " ,", &str_ptr), i++) {
        Shutter[XdrvMailbox.index -1].tilt_config[i] = Settings->shutter_tilt_config[i][XdrvMailbox.index -1] = atoi(str);
      }
      // avoid negative runtime
      Settings->shutter_tilt_config[2][XdrvMailbox.index -1] = Shutter[XdrvMailbox.index -1].tilt_config[2] = Shutter[XdrvMailbox.index -1].tilt_config[2] >= 0 ? Shutter[XdrvMailbox.index -1].tilt_config[2] : 127;
      ShutterInit();
    }
    char setting_chr[30] = "0";
    snprintf_P(setting_chr, sizeof(setting_chr), PSTR("%d %d %d %d %d"), Shutter[XdrvMailbox.index -1].tilt_config[0], Shutter[XdrvMailbox.index -1].tilt_config[1],Shutter[XdrvMailbox.index -1].tilt_config[2],Shutter[XdrvMailbox.index -1].tilt_config[3],Shutter[XdrvMailbox.index -1].tilt_config[4]);
    ResponseCmndIdxChar(setting_chr);
    AddLog(LOG_LEVEL_INFO, PSTR("SHT: TiltConfig %d, min: %d, max %d, runtime %d, close_pos: %d, open_pos: %d"), XdrvMailbox.index ,Shutter[XdrvMailbox.index -1].tilt_config[0], Shutter[XdrvMailbox.index -1].tilt_config[1],Shutter[XdrvMailbox.index -1].tilt_config[2],Shutter[XdrvMailbox.index -1].tilt_config[3],Shutter[XdrvMailbox.index -1].tilt_config[4]);
  }
}

void  CmndShutterTiltIncDec(void)
{
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Change in: payload %s (%d), payload %d, idx %d, src %d"), XdrvMailbox.data , XdrvMailbox.data_len, XdrvMailbox.payload , XdrvMailbox.index, TasmotaGlobal.last_source );
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= TasmotaGlobal.shutters_present) && XdrvMailbox.data_len > 0) {
    XdrvMailbox.payload = Shutter[XdrvMailbox.index -1].tilt_target_pos+XdrvMailbox.payload;
    CmndShutterSetTilt();
  } else {
    ResponseCmndIdxNumber(XdrvMailbox.payload);
  }
}

void CmndShutterMotorStop(void)
{
  if (!XdrvMailbox.usridx) {
    if ((XdrvMailbox.payload >= 0) ) {
      Settings->shutter_motorstop = XdrvMailbox.payload;
      ShutterInit();
    }
    ResponseCmndNumber(Settings->shutter_motorstop);
  }
}

#ifdef SHUTTER_UNITTEST
void CmndShutterUnitTest(void) {
  int16_t input_percent[10] = {-5,0,10,26,35,55,80,99,100,105};
  int16_t output_percent[10] = {0,0,10,26,35,55,80,99,100,100};
  uint32_t result_percent[2][2][10] = {{{0,0,24000,62400,84000,132000,192000,237600,240000,240000},
                                  {0,0,360000,936000,1260000,1980000,2880000,3564000,3600000,3600000}},
                                  {{0,0,76296,100000,113333,174299,205795,237983,240000,240000},
                                  {0,0,1144444,1500000,1700000,2614488,3086929,3569748,3600000,3600000}}};

  uint32_t result = 0;
  char svalue[50];                   // Command and number parameter
  Settings->shuttercoeff[0][0] = 0;
  for (uint8_t i=0; i<2 ; i++){
    snprintf_P(svalue, sizeof(svalue), PSTR(D_PRFX_SHUTTER D_CMND_SHUTTER_OPENTIME "%d %d"), 1, 12);
    ExecuteCommand(svalue, SRC_SHUTTER);
    ShutterInit();
    for (uint8_t j=0; j<2 ; j++){
      for (uint8_t k=0; k<10 ; k++){
        result += (result_percent[i][j][k] == ShutterPercentToRealPosition(input_percent[k] , 0) ? 0 : 1);
        AddLog(LOG_LEVEL_ERROR, PSTR("SHT: ShutterPercentToRealPosition error %d: %d <-> %d"),result, ShutterPercentToRealPosition(input_percent[k] , 0), result_percent[i][j][k]);
      }
      snprintf_P(svalue, sizeof(svalue), PSTR(D_PRFX_SHUTTER D_CMND_SHUTTER_OPENTIME "%d %d"), 1, 180);
      ExecuteCommand(svalue, SRC_SHUTTER);
    }
    snprintf_P(svalue, sizeof(svalue), PSTR(D_PRFX_SHUTTER D_CMND_SHUTTER_CLIBRATION "%d %s"), 1, "15 83 105 185 210");
    ExecuteCommand(svalue, SRC_SHUTTER);
  }
  if (!result){
    AddLog(LOG_LEVEL_ERROR, PSTR("SHT: ShutterPercentToRealPosition:     PASS"));
  } else {
    AddLog(LOG_LEVEL_ERROR, PSTR("SHT: ShutterPercentToRealPosition:     FAIL"));
  }
  Settings->shuttercoeff[0][0] = 0;
  for (uint8_t i=0; i<2 ; i++){
    snprintf_P(svalue, sizeof(svalue), PSTR(D_PRFX_SHUTTER D_CMND_SHUTTER_OPENTIME "%d %d"), 1, 12);
    ExecuteCommand(svalue, SRC_SHUTTER);
    ShutterInit();
    for (uint8_t j=0; j<2 ; j++){
      for (uint8_t k=0; k<10 ; k++){
        result += (output_percent[k] == ShutterRealToPercentPosition(result_percent[i][j][k]  , 0) ? 0 : 1);
        AddLog(LOG_LEVEL_ERROR, PSTR("SHT: ShutterRealToPercentPosition error %d: %d <-> %d"),result, ShutterRealToPercentPosition(result_percent[i][j][k] , 0), output_percent[k]);
      }
      snprintf_P(svalue, sizeof(svalue), PSTR(D_PRFX_SHUTTER D_CMND_SHUTTER_OPENTIME "%d %d"), 1, 180);
      ExecuteCommand(svalue, SRC_SHUTTER);
    }
    snprintf_P(svalue, sizeof(svalue), PSTR(D_PRFX_SHUTTER D_CMND_SHUTTER_CLIBRATION "%d %s"), 1, "15 83 105 185 210");
    ExecuteCommand(svalue, SRC_SHUTTER);
  }
  if (!result){
    AddLog(LOG_LEVEL_ERROR, PSTR("SHT: ShutterRealToPercentPosition:     PASS"));
  } else {
    AddLog(LOG_LEVEL_ERROR, PSTR("SHT: ShutterRealToPercentPosition:     FAIL"));
  }
}
#endif  // SHUTTER_UNITTEST

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv27(uint32_t function)
{
  bool result = false;

  if (Settings->flag3.shutter_mode) {  // SetOption80 - Enable shutter support
    uint8_t  counter = XdrvMailbox.index==0?1:XdrvMailbox.index;
    uint8_t  counterend = XdrvMailbox.index==0?TasmotaGlobal.shutters_present:XdrvMailbox.index;
    uint32_t rescue_index    = XdrvMailbox.index;
    int32_t  rescue_payload  = XdrvMailbox.payload;
    uint32_t rescue_data_len = XdrvMailbox.data_len;
    char stemp1[10];
    power_t save_powermatrix;
    switch (function) {
      case FUNC_EVERY_50_MSECOND:
        ShutterUpdatePosition();
        break;
      case FUNC_EVERY_SECOND:
      //case FUNC_EVERY_250_MSECOND:
        ShutterReportPosition(false, MAX_SHUTTERS);
        break;
      case FUNC_INIT:
        ShutterInit();
        break;
      case FUNC_COMMAND:
        for (uint8_t i = counter; i <= counterend; i++) {
          XdrvMailbox.index = i;
          XdrvMailbox.payload = rescue_payload;
          XdrvMailbox.data_len = rescue_data_len;
          result = DecodeCommand(kShutterCommands, ShutterCommand);
        }
        break;
      case FUNC_JSON_APPEND:
        if (!sensor_data_reported || TasmotaGlobal.tele_period != 2) {
          sensor_data_reported = true;
          for (uint8_t i = 0; i < TasmotaGlobal.shutters_present; i++) {
            uint8_t position = ShutterRealToPercentPosition(Shutter[i].real_position, i);
            uint8_t target   = ShutterRealToPercentPosition(Shutter[i].target_position, i);
            ResponseAppend_P(",");
            ResponseAppend_P(JSON_SHUTTER_POS, i+1, (Settings->shutter_options[i] & 1) ? 100 - position : position, Shutter[i].direction,(Settings->shutter_options[i] & 1) ? 100 - target : target, Shutter[i].tilt_real_pos );
            if (Shutter[i].direction != 0) {
              sensor_data_reported = false;
            }
  #ifdef USE_DOMOTICZ
            if ((0 == TasmotaGlobal.tele_period) && (0 == i)) {
              DomoticzSensor(DZ_SHUTTER, (Settings->shutter_options[i] & 1) ? 100 - position : position);
            }
  #endif  // USE_DOMOTICZ
          }
        }
        break;
      case FUNC_SET_POWER:

        // extract the number of the relay that was switched and save for later in Update Position.
        ShutterGlobal.RelayCurrentMask = XdrvMailbox.index ^ ShutterGlobal.RelayOldMask;
        ShutterGlobal.LastChangedRelay = ShutterGetRelayNoFromBitfield(XdrvMailbox.index ^ ShutterGlobal.RelayOldMask);
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: FUNC_SET_POWER Relaymask %d SwitchedRelay:%d by %s, payload %d, powermask %d"), ShutterGlobal.RelayOldMask, ShutterGlobal.LastChangedRelay,GetTextIndexed(stemp1, sizeof(stemp1), TasmotaGlobal.last_source, kCommandSource),XdrvMailbox.payload, TasmotaGlobal.power);
        save_powermatrix = TasmotaGlobal.power;
        if (!ShutterGlobal.LastChangedRelay) {
          ShutterGlobal.skip_relay_change = 1;
          //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("INVALID REQUEST"));
        } else {
          ShutterRelayChanged();
          ShutterGlobal.RelayOldMask = XdrvMailbox.index;
          TasmotaGlobal.power = save_powermatrix;
        }
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: FUNC_SET_POWER end. powermask %d"), TasmotaGlobal.power);
      break;
      case FUNC_SET_DEVICE_POWER:
        if (ShutterGlobal.skip_relay_change ) {
          //AddLog(LOG_LEVEL_ERROR, PSTR("SHT: Skip relay change %d"), i+1);
          result = true;
          ShutterGlobal.skip_relay_change = 0;
          AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Skipping switch off relay %d"), ShutterGlobal.LastChangedRelay);
          //ExecuteCommandPowerShutter(i+1, 0, SRC_SHUTTER);
          if (ShutterGlobal.LastChangedRelay) ShutterGlobal.RelayOldMask = TasmotaGlobal.power ^=  1<<(ShutterGlobal.LastChangedRelay-1);
          //ShutterGlobal.RelayOldMask ^= 1<<(ShutterGlobal.LastChangedRelay-1);
        }
      break;
      case FUNC_BUTTON_PRESSED:
        if (XdrvMailbox.index < MAX_SHUTTER_KEYS && Settings->shutter_button[XdrvMailbox.index] & (1<<31)) {
          ShutterButtonHandler();
          result = true;
        }
      break;
      case FUNC_ACTIVE:
        result = true;
        break;
    }
    XdrvMailbox.index = rescue_index;
    XdrvMailbox.payload = rescue_payload;
    XdrvMailbox.data_len = rescue_data_len;
  }
  return result;
}

#endif  // USE_SHUTTER
#endif  // ESP8266
