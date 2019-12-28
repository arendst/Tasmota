/*
  xdrv_27_shutter.ino - Shutter/Blind support for Tasmota

  Copyright (C) 2019  Stefan Bode

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

#define D_SHUTTER "SHUTTER"

const uint16_t MOTOR_STOP_TIME = 500;   // in mS
const uint8_t steps_per_second = 20;    // FUNC_EVERY_50_MSECOND

uint8_t calibrate_pos[6] = {0,30,50,70,90,100};
uint16_t messwerte[5] = {30,50,70,90,100};
uint16_t last_execute_step;

enum ShutterModes { SHT_OFF_OPEN__OFF_CLOSE, SHT_OFF_ON__OPEN_CLOSE, SHT_PULSE_OPEN__PULSE_CLOSE, SHT_OFF_ON__OPEN_CLOSE_STEPPER,};

const char kShutterCommands[] PROGMEM = D_PRFX_SHUTTER "|"
  D_CMND_SHUTTER_OPEN "|" D_CMND_SHUTTER_CLOSE "|" D_CMND_SHUTTER_STOP "|" D_CMND_SHUTTER_POSITION  "|"
  D_CMND_SHUTTER_OPENTIME "|" D_CMND_SHUTTER_CLOSETIME "|" D_CMND_SHUTTER_RELAY "|"
  D_CMND_SHUTTER_SETHALFWAY "|" D_CMND_SHUTTER_SETCLOSE "|" D_CMND_SHUTTER_INVERT "|" D_CMND_SHUTTER_CLIBRATION "|"
  D_CMND_SHUTTER_MOTORDELAY "|" D_CMND_SHUTTER_FREQUENCY;

void (* const ShutterCommand[])(void) PROGMEM = {
  &CmndShutterOpen, &CmndShutterClose, &CmndShutterStop, &CmndShutterPosition,
  &CmndShutterOpenTime, &CmndShutterCloseTime, &CmndShutterRelay,
  &CmndShutterSetHalfway, &CmndShutterSetClose, &CmndShutterInvert, &CmndShutterCalibration , &CmndShutterMotorDelay,
  &CmndShutterFrequency};

const char JSON_SHUTTER_POS[] PROGMEM = "\"" D_PRFX_SHUTTER "%d\":{\"Position\":%d,\"direction\":%d}";
const char MSG_SHUTTER_POS[] PROGMEM = "SHT: " D_PRFX_SHUTTER " %d: Real. %d, Start: %d, Stop: %d, dir %d, motordelay %d, rtc: %s [s], freq %d";

#include <Ticker.h>

Ticker TickerShutter;

struct SHUTTER {
  power_t mask = 0;                       // bit mask with 11 at the position of relays that belong to at least ONE shutter
  power_t old_power = 0;                  // preserve old bitmask for power to extract the relay that changes.
  power_t switched_relay = 0;             // bitmatrix that contain the relays that was lastly changed.
  uint32_t time[MAX_SHUTTERS];            // operating time of the shutter in 0.05sec
  int32_t open_max[MAX_SHUTTERS];         // max value on maximum open calculated
  int32_t target_position[MAX_SHUTTERS];  // position to go to
  int32_t start_position[MAX_SHUTTERS];   // position before a movement is started. init at start
  int32_t real_position[MAX_SHUTTERS];    // value between 0 and Shutter.open_max
  uint16_t open_time[MAX_SHUTTERS];       // duration to open the shutter. 112 = 11.2sec
  uint16_t close_time[MAX_SHUTTERS];      // duration to close the shutter. 112 = 11.2sec
  uint16_t close_velocity[MAX_SHUTTERS];  // in relation to open velocity. higher value = faster
  int8_t  direction[MAX_SHUTTERS];        // 1 == UP , 0 == stop; -1 == down
  uint8_t mode = 0;                       // operation mode definition. see enum type above SHT_OFF_OPEN__OFF_CLOSE, SHT_OFF_ON__OPEN_CLOSE, SHT_PULSE_OPEN__PULSE_CLOSE
  int16_t motordelay[MAX_SHUTTERS];       // initial motorstarttime in 0.05sec.
  int16_t pwm_frequency;                 // frequency of PWN for stepper motors
  uint16_t max_pwm_frequency = 1000;      // maximum of PWM frequency for openig the shutter. depend on the motor and drivers
  uint16_t max_close_pwm_frequency[MAX_SHUTTERS];// maximum of PWM frequency for closeing the shutter. depend on the motor and drivers
  uint8_t skip_relay_change;                 // avoid overrun at endstops
  int32_t accelerator[MAX_SHUTTERS];         // speed of ramp-up, ramp down of shutter
} Shutter;

void ShutterRtc50mS(void)
{
  for (uint32_t i = 0; i < shutters_present; i++) {
    Shutter.time[i]++;
    if (Shutter.accelerator[i]) {
      Shutter.pwm_frequency += Shutter.accelerator[i];
      Shutter.pwm_frequency = tmax(0,tmin(Shutter.direction[i]==1 ? Shutter.max_pwm_frequency : Shutter.max_close_pwm_frequency[i],Shutter.pwm_frequency));
      analogWriteFreq(Shutter.pwm_frequency);
      analogWrite(pin[GPIO_PWM1+i], 50);
    }
  }
}


int32_t ShutterPercentToRealPosition(uint8_t percent,uint8_t index)
{
	if (Settings.shutter_set50percent[index] != 50) {
    return  percent <= 5 ?  Settings.shuttercoeff[2][index] * percent : Settings.shuttercoeff[1][index] * percent + Settings.shuttercoeff[0][index];
	} else {
    uint32_t realpos;
    // check against DIV 0
    for (uint8_t j=0 ; j < 5 ; j++) {
      if (Settings.shuttercoeff[j][index] == 0) {
        AddLog_P2(LOG_LEVEL_ERROR, PSTR("SHT: RESET/INIT CALIBRATION MATRIX DIV 0"));
        for (uint8_t k=0 ; k < 5 ; k++) {
          Settings.shuttercoeff[k][index] = messwerte[k] * 1000 / messwerte[4];
        }
      }
    }
    for (uint8_t i=0 ; i < 5 ; i++) {
      if (percent*10 > Settings.shuttercoeff[i][index]) {
        realpos = Shutter.open_max[index] * calibrate_pos[i+1] / 100;
        //AddLog_P2(LOG_LEVEL_INFO, PSTR("Realposition TEMP1: %d, %% %d, coeff %d"), realpos, percent, Settings.shuttercoeff[i][index]);
      } else {
        if ( i == 0) {
          realpos =  percent * Shutter.open_max[index] * calibrate_pos[i+1] / Settings.shuttercoeff[i][index] / 10;
        } else {
          //uint16_t addon = ( percent*10 - Settings.shuttercoeff[i-1][index] ) * Shutter_Open_Max[index] * (calibrate_pos[i+1] - calibrate_pos[i]) / (Settings.shuttercoeff[i][index] -Settings.shuttercoeff[i-1][index]) / 100;
          //AddLog_P2(LOG_LEVEL_INFO, PSTR("Realposition TEMP2: %d, %% %d, coeff %d"), addon, (calibrate_pos[i+1] - calibrate_pos[i]), (Settings.shuttercoeff[i][index] -Settings.shuttercoeff[i-1][index]));
          realpos += ( percent*10 - Settings.shuttercoeff[i-1][index] ) * Shutter.open_max[index] * (calibrate_pos[i+1] - calibrate_pos[i]) / (Settings.shuttercoeff[i][index] -Settings.shuttercoeff[i-1][index]) / 100;
        }
        break;
      }
    }
		return  realpos;
	}
}

uint8_t ShutterRealToPercentPosition(int32_t realpos, uint8_t index)
{
	if (Settings.shutter_set50percent[index] != 50) {
		return Settings.shuttercoeff[2][index] * 5 > realpos ? realpos / Settings.shuttercoeff[2][index] : (realpos-Settings.shuttercoeff[0][index]) / Settings.shuttercoeff[1][index];
	} else {
    uint16_t realpercent;

    for (uint8_t i=0 ; i < 5 ; i++) {
      if (realpos > Shutter.open_max[index] * calibrate_pos[i+1] / 100) {
        realpercent = Settings.shuttercoeff[i][index] /10;
        //AddLog_P2(LOG_LEVEL_INFO, PSTR("Realpercent TEMP1: %d, %% %d, coeff %d"), realpercent, realpos, Shutter_Open_Max[index] * calibrate_pos[i+1] / 100);
      } else {
        if ( i == 0) {
          realpercent =  ( realpos - (Shutter.open_max[index] * calibrate_pos[i] / 100) ) * 10 * Settings.shuttercoeff[i][index] / calibrate_pos[i+1] / Shutter.open_max[index];
        } else {
          //uint16_t addon = ( realpos - (Shutter_Open_Max[index] * calibrate_pos[i] / 100) ) * 10 * (Settings.shuttercoeff[i][index] - Settings.shuttercoeff[i-1][index]) / (calibrate_pos[i+1] - calibrate_pos[i])/ Shutter_Open_Max[index];
          //uint16_t addon = ( percent*10 - Settings.shuttercoeff[i-1][index] ) * Shutter_Open_Max[index] * (calibrate_pos[i+1] - calibrate_pos[i]) / (Settings.shuttercoeff[i][index] -Settings.shuttercoeff[i-1][index]) / 100;
          //AddLog_P2(LOG_LEVEL_INFO, PSTR("Realpercent TEMP2: %d, delta %d,  %% %d, coeff %d"), addon,( realpos - (Shutter_Open_Max[index] * calibrate_pos[i] / 100) ) , (calibrate_pos[i+1] - calibrate_pos[i])* Shutter_Open_Max[index]/100, (Settings.shuttercoeff[i][index] -Settings.shuttercoeff[i-1][index]));
          realpercent += ( realpos -  (Shutter.open_max[index] * calibrate_pos[i] / 100) ) * 10 * (Settings.shuttercoeff[i][index] - Settings.shuttercoeff[i-1][index]) / (calibrate_pos[i+1] - calibrate_pos[i]) / Shutter.open_max[index] ;
        }
        break;
      }
    }
    return realpercent;
  }
}


void ShutterInit(void)
{
  shutters_present = 0;
  Shutter.mask = 0;
  //Initialize to get relay that changed
  Shutter.old_power = power;
  bool relay_in_interlock = false;

  // if shutter 4 is unused
  if (Settings.shutter_startrelay[MAX_SHUTTERS] == 0) {
     Shutter.max_pwm_frequency = Settings.shuttercoeff[4][3] > 0 ? Settings.shuttercoeff[4][3] : Shutter.max_pwm_frequency;
  }
  for (uint32_t i = 0; i < MAX_SHUTTERS; i++) {
    // set startrelay to 1 on first init, but only to shutter 1. 90% usecase
    Settings.shutter_startrelay[i] = (Settings.shutter_startrelay[i] == 0 && i ==  0? 1 : Settings.shutter_startrelay[i]);
    if (Settings.shutter_startrelay[i] && Settings.shutter_startrelay[i] <9) {
      shutters_present++;

      // Determine shutter types
      Shutter.mask |= 3 << (Settings.shutter_startrelay[i] -1)  ;

      for (uint32_t j = 0; j < MAX_INTERLOCKS * Settings.flag.interlock; j++) {  // CMND_INTERLOCK - Enable/disable interlock
        //AddLog_P2(LOG_LEVEL_INFO, PSTR("SHT: Interlock state i=%d %d, flag %d, , shuttermask %d, maskedIL %d"),i, Settings.interlock[i], Settings.flag.interlock,Shutter.mask, Settings.interlock[i]&Shutter.mask);
        if (Settings.interlock[j] && Settings.interlock[j] & Shutter.mask) {
          //AddLog_P2(LOG_LEVEL_INFO, PSTR("SHT: Relay in Interlock group"));
          relay_in_interlock = true;
        }
      }
      if (relay_in_interlock) {
        if (Settings.pulse_timer[i] > 0) {
          Shutter.mode = SHT_PULSE_OPEN__PULSE_CLOSE;
        } else {
          Shutter.mode = SHT_OFF_OPEN__OFF_CLOSE;
        }
      } else {
        Shutter.mode = SHT_OFF_ON__OPEN_CLOSE;
        if (pin[GPIO_PWM1+i] < 99 && pin[GPIO_CNTR1+i]) {
          Shutter.mode = SHT_OFF_ON__OPEN_CLOSE_STEPPER;
          Shutter.pwm_frequency = 0;
          analogWriteFreq(Shutter.pwm_frequency);
          analogWrite(pin[GPIO_PWM1+i], 50);
        }
      }

      TickerShutter.attach_ms(50, ShutterRtc50mS );
      // default the 50 percent should not have any impact without changing it. set to 60
      Settings.shutter_set50percent[i] = Settings.shutter_set50percent[i] > 0 ? Settings.shutter_set50percent[i] : 50;

      // use 10 sec. as default to allow everybody to play without deep initialize
      Shutter.open_time[i] = Settings.shutter_opentime[i] > 0 ? Settings.shutter_opentime[i] : 100;
      Shutter.close_time[i] = Settings.shutter_closetime[i] > 0 ? Settings.shutter_closetime[i] : 100;

      // Update Calculation 20 because time interval is 0.05 sec
      Shutter.open_max[i] = 200 * Shutter.open_time[i];
      Shutter.close_velocity[i] =  Shutter.open_max[i] /  Shutter.close_time[i] / 2 ;
      Shutter.max_close_pwm_frequency[i] = Shutter.max_pwm_frequency*Shutter.open_time[i]/Shutter.close_time[i];
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHT: Shutter %d Closefreq: %d"),i, Shutter.max_close_pwm_frequency[i]);

      // calculate a ramp slope at the first 5 percent to compensate that shutters move with down part later than the upper part
      if (Settings.shutter_set50percent[i] != 50) {
      	Settings.shuttercoeff[1][i] = Shutter.open_max[i] * (100 - Settings.shutter_set50percent[i] ) / 5000;
      	Settings.shuttercoeff[0][i] = Shutter.open_max[i] - (Settings.shuttercoeff[1][i] * 100);
      	Settings.shuttercoeff[2][i] = (Settings.shuttercoeff[0][i] + 5 * Settings.shuttercoeff[1][i]) / 5;
      }
      Shutter.mask |= 3 << (Settings.shutter_startrelay[i] -1)  ;

      Shutter.real_position[i] = ShutterPercentToRealPosition(Settings.shutter_position[i], i);
      //Shutter.real_position[i] =   Settings.shutter_position[i] <= 5 ?  Settings.shuttercoeff[2][i] * Settings.shutter_position[i] : Settings.shuttercoeff[1][i] * Settings.shutter_position[i] + Settings.shuttercoeff[0,i];
      Shutter.start_position[i] = Shutter.real_position[i];
      Shutter.motordelay[i] = Settings.shutter_motordelay[i];

      char shutter_open_chr[10];
      dtostrfd((float)Shutter.open_time[i] / 10 , 1, shutter_open_chr);
      char shutter_close_chr[10];
      dtostrfd((float)Shutter.close_time[i] / 10, 1, shutter_close_chr);
      AddLog_P2(LOG_LEVEL_INFO, PSTR("SHT: Shutter %d (Relay:%d): Init. Pos: %d [%d %%], Open Vel.: 100 Close Vel.: %d , Max Way: %d, Opentime %s [s], Closetime %s [s], CoeffCalc: c0: %d, c1 %d, c2: %d, c3: %d, c4: %d, binmask %d, is inverted %d, shuttermode %d,motordelay %d"),
        i+1, Settings.shutter_startrelay[i], Shutter.real_position[i], Settings.shutter_position[i], Shutter.close_velocity[i], Shutter.open_max[i], shutter_open_chr, shutter_close_chr,
        Settings.shuttercoeff[0][i], Settings.shuttercoeff[1][i], Settings.shuttercoeff[2][i], Settings.shuttercoeff[3][i], Settings.shuttercoeff[4][i],
        Shutter.mask, Settings.shutter_invert[i], Shutter.mode, Shutter.motordelay[i]);

    } else {
      // terminate loop at first INVALID shutter.
      break;
    }

    Settings.shutter_accuracy = 1;
  }
}

void ShutterUpdatePosition(void)
{

  char scommand[CMDSZ];
  char stopic[TOPSZ];
  char stemp2[10];

  for (uint32_t i = 0; i < shutters_present; i++) {
    if (Shutter.direction[i] != 0) {
      int32_t stop_position_delta = 20;
      if (Shutter.mode == SHT_OFF_ON__OPEN_CLOSE_STEPPER) {
        // Calculate position with counter. Much more accurate and no need for motordelay workaround
        //                        adding some steps to stop early
        Shutter.real_position[i] =  ShutterCounterBasedPosition(i);

        int32_t  max_frequency = Shutter.direction[i] == 1 ? Shutter.max_pwm_frequency : Shutter.max_close_pwm_frequency[i];
        int32_t  max_freq_change_per_sec =  Shutter.max_pwm_frequency*steps_per_second / (Shutter.motordelay[i]>0 ? Shutter.motordelay[i] : 1);
        int32_t  min_runtime_ms = Shutter.pwm_frequency*1000 / max_freq_change_per_sec;
        int32_t  velocity = Shutter.direction[i] == 1 ? 100 : Shutter.close_velocity[i];
        int32_t  minstopway = min_runtime_ms * velocity / 100 * Shutter.pwm_frequency / max_frequency  * Shutter.direction[i] ;

        int32_t  next_possible_stop = Shutter.real_position[i] + minstopway ;
        stop_position_delta =200 * Shutter.pwm_frequency/max_frequency + Shutter.direction[i] * (next_possible_stop - Shutter.target_position[i]);
        //Shutter.accelerator[i] = tmin(tmax(max_freq_change_per_sec*(100-(Shutter.direction[i]*(Shutter.target_position[i]-next_possible_stop)    ))/2000 , max_freq_change_per_sec*9/200), max_freq_change_per_sec*11/200);
        //int32_t act_freq_change = max_freq_change_per_sec/20;
        AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: time: %d, velocity %d, minstopway %d,cur_freq %d, max_frequency %d, act_freq_change %d, min_runtime_ms %d, act.pos %d, next_stop %d, target: %d"),Shutter.time[i],velocity,minstopway,
                                      Shutter.pwm_frequency,max_frequency, Shutter.accelerator[i],min_runtime_ms,Shutter.real_position[i], next_possible_stop,Shutter.target_position[i]);

        if (Shutter.accelerator[i] < 0 || next_possible_stop * Shutter.direction[i] > Shutter.target_position[i] * Shutter.direction[i] ) {

            Shutter.accelerator[i] = - tmin(tmax(max_freq_change_per_sec*(100-(Shutter.direction[i]*(Shutter.target_position[i]-next_possible_stop)    ))/2000 , max_freq_change_per_sec*9/200), max_freq_change_per_sec*12/200);
            //AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Ramp down: acc: %d"),   Shutter.accelerator[i]);
        } else if (  Shutter.accelerator[i] > 0 && Shutter.pwm_frequency ==  max_frequency) {
          Shutter.accelerator[i] = 0;
        }
      } else {
        Shutter.real_position[i] = Shutter.start_position[i] + ( (Shutter.time[i] - Shutter.motordelay[i]) * (Shutter.direction[i] > 0 ? 100 : -Shutter.close_velocity[i]));
      }
      if ( Shutter.real_position[i] * Shutter.direction[i] + stop_position_delta >= Shutter.target_position[i] * Shutter.direction[i]  ) {
        // calculate relay number responsible for current movement.
        //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHT: Stop Condition detected: real: %d, Target: %d, direction: %d"),Shutter.real_position[i], Shutter.target_position[i],Shutter.direction[i]);
        uint8_t cur_relay = Settings.shutter_startrelay[i] + (Shutter.direction[i] == 1 ? 0 : 1) ;
        int16_t missing_steps;

        switch (Shutter.mode) {
          case SHT_PULSE_OPEN__PULSE_CLOSE:
            // we have a momentary switch here. Needs additional pulse on same relay after the end
            if (SRC_PULSETIMER == last_source || SRC_SHUTTER == last_source || SRC_WEBGUI == last_source) {
              ExecuteCommandPower(cur_relay, 1, SRC_SHUTTER);
            } else {
              last_source = SRC_SHUTTER;
            }
          break;
          case SHT_OFF_ON__OPEN_CLOSE_STEPPER:
            missing_steps = ((Shutter.target_position[i]-Shutter.start_position[i])*Shutter.direction[i]*Shutter.max_pwm_frequency/2000) - RtcSettings.pulse_counter[i];
            //prepare for stop PWM
            AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHT: Remain steps %d, counter %d, freq %d"), missing_steps, RtcSettings.pulse_counter[i] ,Shutter.pwm_frequency);
            Shutter.accelerator[i] = 0;
            Shutter.pwm_frequency = Shutter.pwm_frequency > 250 ? 250 : Shutter.pwm_frequency;
            analogWriteFreq(Shutter.pwm_frequency);
            analogWrite(pin[GPIO_PWM1+i], 50);
            Shutter.pwm_frequency = 0;
            analogWriteFreq(Shutter.pwm_frequency);
            while (RtcSettings.pulse_counter[i] < (uint32_t)(Shutter.target_position[i]-Shutter.start_position[i])*Shutter.direction[i]*Shutter.max_pwm_frequency/2000) {
              delay(1);
            }
            analogWrite(pin[GPIO_PWM1+i], 0);
            Shutter.real_position[i] = ShutterCounterBasedPosition(i);
            AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHT:Real %d, pulsecount %d, start %d"), Shutter.real_position[i],RtcSettings.pulse_counter[i], Shutter.start_position[i]);

            if ((1 << (Settings.shutter_startrelay[i]-1)) & power) {
              ExecuteCommandPower(Settings.shutter_startrelay[i], 0, SRC_SHUTTER);
              ExecuteCommandPower(Settings.shutter_startrelay[i]+1, 0, SRC_SHUTTER);
            }
          break;
          case SHT_OFF_ON__OPEN_CLOSE:
          if ((1 << (Settings.shutter_startrelay[i]-1)) & power) {
            ExecuteCommandPower(Settings.shutter_startrelay[i], 0, SRC_SHUTTER);
            ExecuteCommandPower(Settings.shutter_startrelay[i]+1, 0, SRC_SHUTTER);
          }
          break;
          case SHT_OFF_OPEN__OFF_CLOSE:
            // avoid switching OFF a relay already OFF
            if ((1 << (cur_relay-1)) & power) {
              // Relay is on and need to be switched off.
              ExecuteCommandPower(cur_relay, 0, SRC_SHUTTER);
            }
          break;
        }
        Settings.shutter_position[i] = ShutterRealToPercentPosition(Shutter.real_position[i], i);

        dtostrfd((float)Shutter.time[i] / steps_per_second, 1, stemp2);
        AddLog_P2(LOG_LEVEL_INFO, MSG_SHUTTER_POS, i+1, Shutter.real_position[i], Shutter.start_position[i], Shutter.target_position[i], Shutter.direction[i], Shutter.motordelay[i],stemp2,Shutter.pwm_frequency);
        Shutter.start_position[i] = Shutter.real_position[i];

        // sending MQTT result to broker
        snprintf_P(scommand, sizeof(scommand),PSTR(D_SHUTTER "%d"), i+1);
        GetTopic_P(stopic, STAT, mqtt_topic, scommand);
        Response_P("%d", Settings.shutter_invert[i] ? 100 - Settings.shutter_position[i]: Settings.shutter_position[i]);
        MqttPublish(stopic, Settings.flag.mqtt_power_retain);  // CMND_POWERRETAIN

        Shutter.direction[i] = 0;
        uint8_t position =  Settings.shutter_invert[i] ? 100 - Settings.shutter_position[i]: Settings.shutter_position[i];
        Response_P(PSTR("{"));
        ResponseAppend_P(JSON_SHUTTER_POS, i+1, position, 0 /*Shutter.direction[i]*/);
        ResponseJsonEnd();
        MqttPublishPrefixTopic_P(RESULT_OR_TELE, mqtt_data);
        XdrvRulesProcess();
      }
    }
  }
}

bool ShutterState(uint8_t device)
{
  device--;
  device &= 3;
  return (Settings.flag3.shutter_mode &&  // SetOption80 - Enable shutter support
          (Shutter.mask & (1 << (Settings.shutter_startrelay[device]-1))) );
}

void ShutterStartInit(uint8_t i, int8_t direction, int32_t target_pos)
{
  //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHT: dir %d, delta1 %d, delta2 %d, grant %d"),direction, (Shutter.open_max[i] - Shutter.real_position[i]) / Shutter.close_velocity[i], Shutter.real_position[i] / Shutter.close_velocity[i], 2+Shutter.motordelay[i]);
  if (  ( direction ==  1 && (Shutter.open_max[i] - Shutter.real_position[i]) / 100 <= 2 )
     || ( direction == -1 && Shutter.real_position[i] / Shutter.close_velocity[i]   <= 2)) {
    Shutter.skip_relay_change = 1 ;
  } else {
    if (Shutter.mode == SHT_OFF_ON__OPEN_CLOSE_STEPPER) {
      Shutter.pwm_frequency = 0;
      analogWriteFreq(Shutter.pwm_frequency);
      analogWrite(pin[GPIO_PWM1+i], 0);
      // can be operated without counter, but then not that acurate.
      if (pin[GPIO_CNTR1+i] < 99) {
        RtcSettings.pulse_counter[i] = 0;
      }
      Shutter.accelerator[i] = Shutter.max_pwm_frequency / (Shutter.motordelay[i]>0 ? Shutter.motordelay[i] : 1);
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHT: Ramp up: %d"), Shutter.accelerator[i]);
    }
    Shutter.target_position[i] = target_pos;
    Shutter.start_position[i] = Shutter.real_position[i];
    Shutter.time[i] = 0;
    Shutter.skip_relay_change = 0;
    Shutter.direction[i] = direction;
    //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHT: real %d, start %d, counter %d, max_freq %d, dir %d, freq %d"),Shutter.real_position[i], Shutter.start_position[i] ,RtcSettings.pulse_counter[i],Shutter.max_pwm_frequency , Shutter.direction[i] ,Shutter.max_pwm_frequency );
  }
  //AddLog_P2(LOG_LEVEL_INFO,  PSTR("SHT: Start shutter: %d from %d to %d in directin %d"), i, Shutter.start_position[i], Shutter.target_position[i], Shutter.direction[i]);
}

void ShutterWaitForMotorStop(uint8_t i)
{
  AddLog_P2(LOG_LEVEL_INFO, PSTR("SHT: Wait for Motorstop.."));
  if (Shutter.mode == SHT_OFF_ON__OPEN_CLOSE || Shutter.mode == SHT_OFF_ON__OPEN_CLOSE_STEPPER) {
    if ( Shutter.mode = SHT_OFF_ON__OPEN_CLOSE_STEPPER) {
      //AddLog_P2(LOG_LEVEL_INFO, PSTR("SHT: Frequency change %d"), Shutter.pwm_frequency);
      while (Shutter.pwm_frequency > 0) {
        Shutter.accelerator[i] = 0;
        Shutter.pwm_frequency = tmax(Shutter.pwm_frequency-((Shutter.direction[i] == 1 ? Shutter.max_pwm_frequency : Shutter.max_close_pwm_frequency[i])/(Shutter.motordelay[i]+1)) , 0);
        analogWriteFreq(Shutter.pwm_frequency);
        analogWrite(pin[GPIO_PWM1+i], 50);
        delay(50);
      }
      analogWrite(pin[GPIO_PWM1+i], 0);
      Shutter.real_position[i] = ShutterCounterBasedPosition(i);
    } else {
      ExecuteCommandPower(Settings.shutter_startrelay[i], 0, SRC_SHUTTER);
      delay(MOTOR_STOP_TIME);
    }
  } else {
    delay(MOTOR_STOP_TIME);
  }
}

void ShutterReportPosition(void)
{
  uint16_t shutter_moving = 0;
  for (uint8_t i = 0; i < shutters_present; i++) {
    //AddLog_P2(LOG_LEVEL_INFO, PSTR("SHT: Shutter %d: Real Pos: %d"), i+1,Shutter.real_position[i]);
    if (Shutter.direction[i] != 0) {
      char stemp2[10];
      uint8_t position =  ShutterRealToPercentPosition(Shutter.real_position[i], i);
      dtostrfd((float)Shutter.time[i] / steps_per_second, 2, stemp2);
      shutter_moving = 1;
      //Settings.shutter_position[i] = Settings.shuttercoeff[2][i] * 5 > Shutter.real_position[i] ? Shutter.real_position[i] / Settings.shuttercoeff[2][i] : (Shutter.real_position[i]-Settings.shuttercoeff[0,i]) / Settings.shuttercoeff[1][i];
      AddLog_P2(LOG_LEVEL_INFO, MSG_SHUTTER_POS, i+1, Shutter.real_position[i], Shutter.start_position[i], Shutter.target_position[i], Shutter.direction[i], Shutter.motordelay[i],stemp2,Shutter.pwm_frequency);
      Response_P(PSTR("{"));
      ResponseAppend_P(JSON_SHUTTER_POS, i+1, Settings.shutter_invert[i] ? 100-position : position, Shutter.direction[i]);
      ResponseJsonEnd();
      MqttPublishPrefixTopic_P(RESULT_OR_TELE, mqtt_data);
    }
  }
  if (rules_flag.shutter_moving > shutter_moving) {
    rules_flag.shutter_moved = 1;
  } else {
    rules_flag.shutter_moved = 0;
  }
  rules_flag.shutter_moving = shutter_moving;
  //AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: rules_flag.shutter_moving: %d, moved %d"), rules_flag.shutter_moving, rules_flag.shutter_moved);
}

int32_t ShutterCounterBasedPosition(uint8 i)
{
  return ((int32_t)RtcSettings.pulse_counter[i]*Shutter.direction[i]*2000 / Shutter.max_pwm_frequency)+Shutter.start_position[i];
}

void ShutterRelayChanged(void)
{

  // Shutter.switched_relay = binary relay that was recently changed and cause an Action
  // powerstate_local = binary powermatrix and relays from shutter: 0..3
  // relays_changed = bool if one of the relays that belong to the shutter changed not by shutter or pulsetimer
  char stemp1[10];

	for (uint32_t i = 0; i < shutters_present; i++) {
		power_t powerstate_local = (power >> (Settings.shutter_startrelay[i] -1)) & 3;
		//uint8   manual_relays_changed = ((Shutter.switched_relay >> (Settings.shutter_startrelay[i] -1)) & 3) && SRC_IGNORE != last_source && SRC_SHUTTER != last_source && SRC_PULSETIMER != last_source ;
    uint8   manual_relays_changed = ((Shutter.switched_relay >> (Settings.shutter_startrelay[i] -1)) & 3) && SRC_SHUTTER != last_source && SRC_PULSETIMER != last_source ;
    //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHT: Shutter %d: source: %s, powerstate_local %ld, Shutter.switched_relay %d, manual change %d"), i+1, GetTextIndexed(stemp1, sizeof(stemp1), last_source, kCommandSource), powerstate_local,Shutter.switched_relay,manual_relays_changed);
    if (manual_relays_changed) {
      //Shutter.skip_relay_change = true;
      if (Shutter.mode == SHT_OFF_ON__OPEN_CLOSE ||  Shutter.mode == SHT_OFF_ON__OPEN_CLOSE_STEPPER) {
        ShutterWaitForMotorStop(i);
				switch (powerstate_local) {
					case 1:
            ShutterStartInit(i, 1, Shutter.open_max[i]);
					  break;
					case 3:
					  ShutterStartInit(i, -1, 0);
					  break;
					default:
            //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHT: Shutter %d: Switch OFF motor."),i);
					  Shutter.target_position[i] = Shutter.real_position[i];
				  }
			} else {
        if (Shutter.direction[i] != 0 && (!powerstate_local || (powerstate_local && Shutter.mode == SHT_PULSE_OPEN__PULSE_CLOSE))) {
					Shutter.target_position[i] = Shutter.real_position[i];
					AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHT: Shutter %d: Switch OFF motor. Target: %ld, source: %s, powerstate_local %ld, Shutter.switched_relay %d, manual change %d"), i+1, Shutter.target_position[i], GetTextIndexed(stemp1, sizeof(stemp1), last_source, kCommandSource), powerstate_local,Shutter.switched_relay,manual_relays_changed);
				} else {
					last_source = SRC_SHUTTER; // avoid switch off in the next loop
					if (powerstate_local == 2) { // testing on CLOSE relay, if ON
					  // close with relay two
            ShutterWaitForMotorStop(i);
					  ShutterStartInit(i, -1, 0);
					} else {
					  // opens with relay one
            ShutterWaitForMotorStop(i);
					  ShutterStartInit(i, 1, Shutter.open_max[i]);
					}
				}
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHT: Shutter %d: Target: %ld, powerstatelocal %d"), i+1, Shutter.target_position[i], powerstate_local);
			}
		}
	}
}

void ShutterSetPosition(uint8_t device, uint8_t position)
{
  char svalue[32];                   // Command and number parameter
  snprintf_P(svalue, sizeof(svalue), PSTR(D_PRFX_SHUTTER D_CMND_SHUTTER_POSITION "%d %d"), device, position);
  ExecuteCommand(svalue, SRC_IGNORE);
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndShutterOpen(void)
{
  //AddLog_P2(LOG_LEVEL_INFO, PSTR("SHT: Payload close: %d, i %d"), XdrvMailbox.payload, XdrvMailbox.i);
  if ( XdrvMailbox.index == 1 && XdrvMailbox.payload != -99) {
    XdrvMailbox.index = XdrvMailbox.payload;
  }
  XdrvMailbox.payload = 100;
  last_source = SRC_WEBGUI;
  CmndShutterPosition();
}

void CmndShutterClose(void)
{
  //AddLog_P2(LOG_LEVEL_INFO, PSTR("SHT: Payload open: %d, i %d"), XdrvMailbox.payload, XdrvMailbox.i);
  if ( XdrvMailbox.index == 1 && XdrvMailbox.payload != -99) {
    XdrvMailbox.index = XdrvMailbox.payload;
  }
  XdrvMailbox.payload = 0;
  XdrvMailbox.data_len = 0;
  last_source = SRC_WEBGUI;
  CmndShutterPosition();
}

void CmndShutterStop(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= shutters_present)) {
    if ( XdrvMailbox.index == 1 && XdrvMailbox.payload != -99) {
      XdrvMailbox.index = XdrvMailbox.payload;
    }
    uint32_t i = XdrvMailbox.index -1;
    if (Shutter.direction[i] != 0) {

      AddLog_P2(LOG_LEVEL_INFO, PSTR("SHT: Stop moving %d: dir: %d"), XdrvMailbox.index, Shutter.direction[i]);
      // set stop position 10 steps ahead (0.5sec to allow normal stop)
      int32_t temp_realpos = Shutter.start_position[i] + ( (Shutter.time[i]+10) * (Shutter.direction[i] > 0 ? 100 : -Shutter.close_velocity[i]));
      XdrvMailbox.payload = ShutterRealToPercentPosition(temp_realpos, i);
      //XdrvMailbox.payload = Settings.shuttercoeff[2][i] * 5 > temp_realpos ? temp_realpos / Settings.shuttercoeff[2][i] : (temp_realpos-Settings.shuttercoeff[0,i]) / Settings.shuttercoeff[1][i];
      last_source = SRC_WEBGUI;
      CmndShutterPosition();
    } else {
      ResponseCmndDone();
    }
  }
}

void CmndShutterPosition(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= shutters_present)) {
    uint32_t index = XdrvMailbox.index-1;
    //limit the payload
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHT: Pos. in: payload %s (%d), payload %d, idx %d, src %d"), XdrvMailbox.data , XdrvMailbox.data_len, XdrvMailbox.payload , XdrvMailbox.index, last_source );

    if (XdrvMailbox.data_len > 1 &&  XdrvMailbox.payload <=0) {
      UpperCase(XdrvMailbox.data, XdrvMailbox.data);
      if (!strcmp(XdrvMailbox.data,"UP"))   { CmndShutterOpen(); }
      if (!strcmp(XdrvMailbox.data,"DOWN")) { CmndShutterClose(); }
      if (!strcmp(XdrvMailbox.data,"STOP")) { CmndShutterStop(); }
      return;
    }

    int8_t target_pos_percent = XdrvMailbox.payload < 0 ? 0 : (XdrvMailbox.payload > 100 ? 100 : XdrvMailbox.payload);
    // webgui still send also on inverted shutter the native position.
    target_pos_percent = Settings.shutter_invert[index] &&  SRC_WEBGUI != last_source ? 100 - target_pos_percent : target_pos_percent;
    if (XdrvMailbox.payload != -99) {
      //target_pos_percent = Settings.shutter_invert[index] ? 100 - target_pos_percent : target_pos_percent;
      Shutter.target_position[index] = ShutterPercentToRealPosition(target_pos_percent, index);
      Shutter.accelerator[index] = Shutter.max_pwm_frequency / (Shutter.motordelay[index]>0 ? Shutter.motordelay[index] : 1);
      //Shutter.target_position[index] = XdrvMailbox.payload < 5 ?  Settings.shuttercoeff[2][index] * XdrvMailbox.payload : Settings.shuttercoeff[1][index] * XdrvMailbox.payload + Settings.shuttercoeff[0,index];
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHT: lastsource %d:, real %d, target %d, payload %d"), last_source, Shutter.real_position[index] ,Shutter.target_position[index],target_pos_percent);
    }
    if ( (target_pos_percent >= 0) && (target_pos_percent <= 100) && abs(Shutter.target_position[index] - Shutter.real_position[index] ) / Shutter.close_velocity[index] > 2) {
      int8_t new_shutterdirection = Shutter.real_position[index] < Shutter.target_position[index] ? 1 : -1;
      if (Shutter.direction[index] ==  -new_shutterdirection ) {
        // direction need to be changed. on momentary switches first stop the Shutter
        if (Shutter.mode == SHT_PULSE_OPEN__PULSE_CLOSE) {
          // code for momentary shutters only small switch on to stop Shutter
          ExecuteCommandPower(Settings.shutter_startrelay[index] + (new_shutterdirection == 1 ? 0 : 1), 1, SRC_SHUTTER);
          delay(100);
        } else {
          if (Shutter.mode == SHT_OFF_OPEN__OFF_CLOSE) {
            ExecuteCommandPower(Settings.shutter_startrelay[index] + (new_shutterdirection == 1 ? 1 : 0), 0, SRC_SHUTTER);
            ShutterWaitForMotorStop(index);
          }
        }
      }
      if (Shutter.direction[index] !=  new_shutterdirection ) {
        if (Shutter.mode == SHT_OFF_ON__OPEN_CLOSE || Shutter.mode == SHT_OFF_ON__OPEN_CLOSE_STEPPER) {
          //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHT: Delay5 5s, xdrv %d"), XdrvMailbox.payload);
          ShutterWaitForMotorStop(index);
          ExecuteCommandPower(Settings.shutter_startrelay[index], 0, SRC_SHUTTER);
          ShutterStartInit(index, new_shutterdirection, Shutter.target_position[index]);
          // Code for shutters with circuit safe configuration, switch the direction Relay
          ExecuteCommandPower(Settings.shutter_startrelay[index] +1, new_shutterdirection == 1 ? 0 : 1, SRC_SHUTTER);
          // power on
          ExecuteCommandPower(Settings.shutter_startrelay[index], 1, SRC_SHUTTER);
        } else {
          // now start the motor for the right direction, work for momentary and normal shutters.
          AddLog_P2(LOG_LEVEL_INFO, PSTR("SHT: Start in dir %d"), Shutter.direction[index]);
          ShutterStartInit(index, new_shutterdirection, Shutter.target_position[index]);
          ExecuteCommandPower(Settings.shutter_startrelay[index] + (new_shutterdirection == 1 ? 0 : 1), 1, SRC_SHUTTER);
          //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("SHT: Delay6 5s, xdrv %d"), XdrvMailbox.payload);
        }
        Shutter.switched_relay = 0;
      }
    } else {
      target_pos_percent = ShutterRealToPercentPosition(Shutter.real_position[index], index);
    }
    XdrvMailbox.index = index +1;  // Fix random index for ShutterClose
    ResponseCmndIdxNumber(Settings.shutter_invert[index] ? 100 - target_pos_percent : target_pos_percent);
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
      Settings.shutter_motordelay[XdrvMailbox.index -1] = (uint16_t)(steps_per_second * CharToFloat(XdrvMailbox.data));
      ShutterInit();
    }
    char time_chr[10];
    dtostrfd((float)(Settings.shutter_motordelay[XdrvMailbox.index -1]) / steps_per_second, 2, time_chr);
    ResponseCmndIdxChar(time_chr);
  }
}

void CmndShutterRelay(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_SHUTTERS)) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 64)) {
      Settings.shutter_startrelay[XdrvMailbox.index -1] = XdrvMailbox.payload;
      if (XdrvMailbox.payload > 0) {
        Shutter.mask |= 3 << (XdrvMailbox.payload - 1);
      } else {
        Shutter.mask ^= 3 << (Settings.shutter_startrelay[XdrvMailbox.index -1] - 1);
      }
      Settings.shutter_startrelay[XdrvMailbox.index -1] = XdrvMailbox.payload;
      ShutterInit();
      // if payload is 0 to disable the relay there must be a reboot. Otherwhise does not work
    }
    ResponseCmndIdxNumber(Settings.shutter_startrelay[XdrvMailbox.index -1]);
  }
}

void CmndShutterSetHalfway(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= shutters_present)) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
      Settings.shutter_set50percent[XdrvMailbox.index -1] = Settings.shutter_invert[XdrvMailbox.index -1] ? 100 - XdrvMailbox.payload : XdrvMailbox.payload;
      ShutterInit();
    }
  ResponseCmndIdxNumber(Settings.shutter_invert[XdrvMailbox.index -1] ? 100 - Settings.shutter_set50percent[XdrvMailbox.index -1] : Settings.shutter_set50percent[XdrvMailbox.index -1]);
  }
}

void CmndShutterFrequency(void)
{
  if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= 20000)) {
    Shutter.max_pwm_frequency =  XdrvMailbox.payload;
    if (shutters_present < 4) {
      Settings.shuttercoeff[4][3] = Shutter.max_pwm_frequency;
    }
    ShutterInit();
    ResponseCmndNumber(XdrvMailbox.payload);  // ????
  } else {
    ResponseCmndNumber(Shutter.max_pwm_frequency);
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

void CmndShutterInvert(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= shutters_present)) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
      Settings.shutter_invert[XdrvMailbox.index -1] = XdrvMailbox.payload;
    }
    ResponseCmndIdxNumber(Settings.shutter_invert[XdrvMailbox.index -1]);
  }
}

void CmndShutterCalibration(void)  // ????
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= shutters_present)) {
    if (XdrvMailbox.data_len > 0) {
        uint32_t i = 0;
        char *str_ptr;
        char* version_dup = strdup(XdrvMailbox.data);  // Duplicate the version_str as strtok_r will modify it.
        // Loop through the version string, splitting on '.' seperators.
        for (char *str = strtok_r(version_dup, " ", &str_ptr); str && i < 5; str = strtok_r(nullptr, " ", &str_ptr), i++) {
          int field = atoi(str);
          // The fields in a version string can only range from 1-255.
          // and following value must be higher than previous one
          if ((field <= 0) || (field > 255) ||  ( (i>0) && (field <= messwerte[i-1]) ) ) {
            free(version_dup);
            break;
          }
          messwerte[i] = field;
        }
        for (i=0 ; i < 5 ; i++) {
          Settings.shuttercoeff[i][XdrvMailbox.index -1] = messwerte[i] * 1000 / messwerte[4];
          AddLog_P2(LOG_LEVEL_INFO, PSTR("Settings.shuttercoeff: %d, i: %d, value: %d, messwert %d"), i,XdrvMailbox.index -1,Settings.shuttercoeff[i][XdrvMailbox.index -1], messwerte[i]);
        }
        ShutterInit();
        ResponseCmndIdxChar(XdrvMailbox.data);
    }
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
        ShutterReportPosition();
        break;

      case FUNC_COMMAND:
        result = DecodeCommand(kShutterCommands, ShutterCommand);
        break;
      case FUNC_JSON_APPEND:
        for (uint8_t i = 0; i < shutters_present; i++) {
          uint8_t position =  Settings.shutter_invert[i] ? 100 - Settings.shutter_position[i]: Settings.shutter_position[i];
          ResponseAppend_P(",");
          ResponseAppend_P(JSON_SHUTTER_POS, i+1, position, Shutter.direction[i]);
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
        Shutter.switched_relay = XdrvMailbox.index ^ Shutter.old_power;
        AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Switched relay: %d by %s"), Shutter.switched_relay,GetTextIndexed(stemp1, sizeof(stemp1), last_source, kCommandSource));
        ShutterRelayChanged();
        Shutter.old_power = XdrvMailbox.index;
      break;
      case FUNC_SET_DEVICE_POWER:
        if (Shutter.skip_relay_change ) {
          uint8_t i;
          for (i = 0; i < devices_present; i++) {
            if (Shutter.switched_relay &1) {
              break;
            }
            Shutter.switched_relay >>= 1;
          }
          //AddLog_P2(LOG_LEVEL_ERROR, PSTR("SHT: skip relay change: %d"),i+1);
          result = true;
          Shutter.skip_relay_change = 0;
          AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("SHT: Skipping switch off relay %d"),i);
          ExecuteCommandPower(i+1, 0, SRC_SHUTTER);
        }
      break;
    }
  }
  return result;
}

#endif //USE_SHUTTER
