/*
  xdrv_19_Shutter.ino - Shutter/Blind support for Sonoff-TasmotaS
  Copyright (C) 2018  Stefan Bode
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

#ifdef USE_SHUTTER   // +3.8k code

Ticker TickerShutter;

enum ShutterCommands {
  CMND_OPEN, CMND_CLOSE, CMND_STOP, CMND_POSITION, CMND_OPENTIME, CMND_CLOSETIME, CMND_SHUTTERRELAY, CMND_SET50PERCENT, CMND_SHUTTERSETCLOSE, CMND_SHUTTERINVERT, CMND_CALIBRATIONMATIX };
const char kShutterCommands[] PROGMEM =
  D_CMND_OPEN "|" D_CMND_CLOSE "|" D_CMND_STOP "|" D_CMND_POSITION  "|" D_CMND_OPENTIME "|" D_CMND_CLOSETIME "|" D_CMND_SHUTTERRELAY "|" D_CMND_SET50PERCENT "|" D_CMND_SHUTTERSETCLOSE "|" D_CMND_SHUTTERINVERT "|" D_CMND_CALIBRATIONMATIX;

enum ShutterModes { OFF_OPEN__OFF_CLOSE, OFF_ON__OPEN_CLOSE, PULSE_OPEN__PULSE_CLOSE };

const char JSON_SHUTTER_POS[] PROGMEM = "%s,\"%s-%d\":{\"position\":%d, \"direction\":%d}";                                  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
const uint16_t MOTOR_STOP_TIME=500; // in mS

uint16_t Shutter_Open_Time[MAX_SHUTTERS] ;               // duration to open the shutter
uint16_t Shutter_Close_Time[MAX_SHUTTERS];               // duration to close the shutter
int32_t Shutter_Open_Max[MAX_SHUTTERS];                 // max value on maximum open calculated
int32_t Shutter_Target_Position[MAX_SHUTTERS] ;         // position to go to
int32_t Shutter_Start_Position[MAX_SHUTTERS] ;
uint16_t Shutter_Close_Velocity[MAX_SHUTTERS];          // in relation to open velocity. higher value = faster
uint16_t Shutter_Operations[MAX_SHUTTERS];
int8_t  Shutter_Direction[MAX_SHUTTERS];                // 1 == UP , 0 == stop; -1 == down
int32_t Shutter_Real_Position[MAX_SHUTTERS];            // value between 0 and Shutter_Open_Max
//power_t shutter_mask = 0;                               // bit mask with 11 at the position of relays that belong to at least ONE shutter
power_t old_power = power;                              // preserve old bitmask for power to extract the relay that changes.
power_t SwitchedRelay = 0;                              // bitmatrix that contain the relays that was lastly changed.
uint32_t shutter_time[MAX_SHUTTERS] ;
uint8_t shutterMode = 0;                                // operation mode definition. see enum type above OFF_OPEN-OFF_CLOSE, OFF_ON-OPEN_CLOSE, PULSE_OPEN-PULSE_CLOSE

void Rtc_ms50_Second()
{
  for (byte i=0;i < MAX_SHUTTERS; i++) {
    shutter_time[i]++;
  }
}

int32_t percent_to_realposition(uint8_t percent,uint8_t index)
{
	if (Settings.shutter_set50percent[index] != 50) {
    return  percent <= 5 ?  Settings.shuttercoeff[2][index] * percent : Settings.shuttercoeff[1][index] * percent + Settings.shuttercoeff[0][index];
	} else {
		return  percent <= 5 ?  Settings.shuttercoeff[2][index] * percent : Settings.shuttercoeff[1][index] * percent + Settings.shuttercoeff[0][index];
	}
}

uint8_t realposition_to_percent(int32_t realpos, uint8_t index)
{
	if (Settings.shutter_set50percent[index] != 50) {
		return Settings.shuttercoeff[2][index] * 5 > realpos ? realpos / Settings.shuttercoeff[2][index] : (realpos-Settings.shuttercoeff[0][index]) / Settings.shuttercoeff[1][index];
	} else {
		return Settings.shuttercoeff[2][index] * 5 > realpos ? realpos / Settings.shuttercoeff[2][index] : (realpos-Settings.shuttercoeff[0][index]) / Settings.shuttercoeff[1][index];
	}
}

void ShutterInit()
{
  shutters_present = 0;
  shutter_mask = 0;
  //Initialize to get relay that changed
  old_power = power;
  char shutter_open_chr[10];
  char shutter_close_chr[10];
  bool relay_in_interlock = false;

  AddLog_P2(LOG_LEVEL_INFO, PSTR("Shutter accuracy digits: %d"), Settings.shutter_accuracy);

  for (byte i=0;i < MAX_SHUTTERS; i++) {
    // upgrade to 0.1sec calculation base.
    if ( Settings.shutter_accuracy == 0) {
      Settings.shutter_closetime[i] = Settings.shutter_closetime[i] * 10;
      Settings.shutter_opentime[i] = Settings.shutter_opentime[i] * 10;
    }
    // set startrelay to 1 on first init, but only to shutter 1. 90% usecase
    Settings.shutter_startrelay[i] = (Settings.shutter_startrelay[i] == 0 && i ==  0? 1 : Settings.shutter_startrelay[i]);
    if (Settings.shutter_startrelay[i] && Settings.shutter_startrelay[i] <9) {
      shutters_present++;

      // Determine shutter types
      shutter_mask |= 3 << (Settings.shutter_startrelay[i] -1)  ;

      for (uint8_t i = 0; i < MAX_INTERLOCKS * Settings.flag.interlock; i++) {
        //AddLog_P2(LOG_LEVEL_INFO, PSTR("Interlock state i=%d %d, flag %d, , shuttermask %d, maskedIL %d"),i, Settings.interlock[i], Settings.flag.interlock,shutter_mask, Settings.interlock[i]&shutter_mask);
        if (Settings.interlock[i] && Settings.interlock[i] & shutter_mask) {
          //AddLog_P2(LOG_LEVEL_INFO, PSTR("Relay in Interlock group"));
          relay_in_interlock = true;
        }
      }
      if ( relay_in_interlock) {
        if (Settings.pulse_timer[i] > 0) {
          shutterMode = PULSE_OPEN__PULSE_CLOSE;
        } else {
          shutterMode = OFF_OPEN__OFF_CLOSE;
        }
      } else {
        shutterMode = OFF_ON__OPEN_CLOSE;
      }

      TickerShutter.attach_ms(50, Rtc_ms50_Second );
      // default the 50 percent should not have any impact without changing it. set to 60
      Settings.shutter_set50percent[i] = (Settings.shutter_set50percent[i] == 0 ? 50 : Settings.shutter_set50percent[i]);
      // use 10 sec. as default to allow everybody to play without deep initialize
      Shutter_Open_Time[i] = Settings.shutter_opentime[i] > 0 ? Settings.shutter_opentime[i] : 100;
      Shutter_Close_Time[i] = Settings.shutter_closetime[i] > 0 ? Settings.shutter_closetime[i] : 100;

      // Update Calculation 20 because time interval is 0.05 sec
      Shutter_Open_Max[i] = 200 * Shutter_Open_Time[i];
      Shutter_Close_Velocity[i] =  Shutter_Open_Max[i] /  Shutter_Close_Time[i] / 2 ;

      // calculate a ramp slope at the first 5 percent to compensate that shutters move with down part later than the upper part
      Settings.shuttercoeff[1][i] = Shutter_Open_Max[i] * (100 - Settings.shutter_set50percent[i] ) / 5000;
      Settings.shuttercoeff[0][i] = Shutter_Open_Max[i] - (Settings.shuttercoeff[1][i] * 100);
      Settings.shuttercoeff[2][i] = (Settings.shuttercoeff[0][i] + 5 * Settings.shuttercoeff[1][i]) / 5;
      shutter_mask |= 3 << (Settings.shutter_startrelay[i] -1)  ;

      Shutter_Real_Position[i] = percent_to_realposition(Settings.shutter_position[i], i);
      //Shutter_Real_Position[i] =   Settings.shutter_position[i] <= 5 ?  Settings.shuttercoeff[2][i] * Settings.shutter_position[i] : Settings.shuttercoeff[1][i] * Settings.shutter_position[i] + Settings.shuttercoeff[0,i];
      Shutter_Start_Position[i] = Shutter_Real_Position[i];
      dtostrfd((double)Shutter_Open_Time[i] / 10 , 1, shutter_open_chr);
      dtostrfd((double)Shutter_Close_Time[i] / 10, 1, shutter_close_chr);

      AddLog_P2(LOG_LEVEL_INFO, PSTR("Shutter %d (Relay:%d): Init. Pos: %d [%d %%], Open Vel.: 100 Close Vel.: %d , Max Way: %d, Opentime %s [s], Closetime %s [s], CoedffCalc: c0: %d, c1 %d, c2: %d, c3: %d, c4: %d, binmask %d, is inverted %d, shuttermode %d"), i, Settings.shutter_startrelay[i],Shutter_Real_Position[i],Settings.shutter_position[i],  Shutter_Close_Velocity[i] , Shutter_Open_Max[i], shutter_open_chr, shutter_close_chr,Settings.shuttercoeff[0][i],Settings.shuttercoeff[1][i],Settings.shuttercoeff[2][i],Settings.shuttercoeff[3][i],Settings.shuttercoeff[4][i],shutter_mask,Settings.shutter_invert[i],shutterMode );
    } else {
      // terminate loop at first INVALID shutter.
      break;
    }
    Settings.shutter_accuracy = 1;
  }
}

void Schutter_Update_Position()
{
  char scommand[CMDSZ];
  char stopic[TOPSZ];

  for (byte i=0; i < shutters_present; i++) {
    if (Shutter_Direction[i] != 0) {
      //char stemp1[20];
      Shutter_Real_Position[i] = Shutter_Start_Position[i] + ( shutter_time[i] * (Shutter_Direction[i] > 0 ? 100 : -Shutter_Close_Velocity[i]));
      // avoid real position leaving the boundaries.
      Shutter_Real_Position[i] = Shutter_Real_Position[i] < 0 ? 0 : (Shutter_Real_Position[i] > Shutter_Open_Max[i] ? Shutter_Open_Max[i] : Shutter_Real_Position[i]) ;

      // Add additional runtime, if shutter did not reach the endstop for some time.
      if (Shutter_Target_Position[i] == Shutter_Real_Position[i] && Shutter_Target_Position[i] == 0) {
        // for every operation add 5x50ms = 250ms to stop position
        //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Adding additional runtime"));
        Shutter_Real_Position[i] += 500 * Shutter_Operations[i] ;
        Shutter_Operations[i]  = 0;
      }
      if (Shutter_Real_Position[i] * Shutter_Direction[i] >= Shutter_Target_Position[i] * Shutter_Direction[i] ) {
        // calculate relay number responsible for current movement.
        //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Stop Condition detected: real: %d, Target: %d, direction: %d"),Shutter_Real_Position[i], Shutter_Target_Position[i],Shutter_Direction[i]);
        uint8_t cur_relay = Settings.shutter_startrelay[i] + (Shutter_Direction[i] == 1 ? 0 : 1) ;
        char stemp2[10];

        Settings.shutter_position[i] = realposition_to_percent(Shutter_Real_Position[i], i);
        //Settings.shutter_position[i] = Settings.shuttercoeff[2][i] * 5 > Shutter_Real_Position[i] ? (Shutter_Real_Position[i] * 10 / Settings.shuttercoeff[2][i] + 4)/10 : ((Shutter_Real_Position[i]-Settings.shuttercoeff[0,i]) *10 / Settings.shuttercoeff[1][i] +4) / 10;

        if (0 < Settings.shutter_position[i] && Settings.shutter_position[i] < 100) {
          Shutter_Operations[i]++;
        } else {
          Shutter_Operations[i]  = 0;
        }

        dtostrfd((double)shutter_time[i] / 20, 1, stemp2);
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Shutter %d: Real Pos. %d, Stoppos: %ld, relay: %d, direction %d, pulsetimer: %d, rtcshutter: %s [s], operationtime %d"), i, Shutter_Real_Position[i], Settings.shutter_position[i], cur_relay -1, Shutter_Direction[i], Settings.pulse_timer[cur_relay -1], stemp2, Shutter_Operations[i]);
        Shutter_Start_Position[i] = Shutter_Real_Position[i];

        // sending MQTT result to broker
        snprintf_P(scommand, sizeof(scommand),PSTR("%s%d"), D_SHUTTER, i+1);
        GetTopic_P(stopic, STAT, mqtt_topic, scommand);
        Response_P("%d", Settings.shutter_invert[i] ? 100 - Settings.shutter_position[i]: Settings.shutter_position[i]);
        MqttPublish(stopic, Settings.flag.mqtt_power_retain);

        switch (shutterMode) {
          case PULSE_OPEN__PULSE_CLOSE:
            // we have a momentary switch here. Needs additional pulse on same relay after the end
            if (SRC_PULSETIMER == last_source || SRC_SHUTTER == last_source || SRC_WEBGUI == last_source) {
              ExecuteCommandPower(cur_relay, 1, SRC_SHUTTER);
            } else {
              last_source = SRC_SHUTTER;
            }
          break;
          case OFF_ON__OPEN_CLOSE:
            // This is a failsafe configuration. Relay1 ON/OFF Relay2 -1/1 direction
            if ((1 << (Settings.shutter_startrelay[i]-1)) & power) {
              ExecuteCommandPower(Settings.shutter_startrelay[i], 0, SRC_SHUTTER);
            }
          break;
          case OFF_OPEN__OFF_CLOSE:
            // avoid switching OFF a relay already OFF
            if ((1 << (cur_relay-1)) & power) {
              // Relay is on and need to be switched off.
              ExecuteCommandPower(cur_relay, 0, SRC_SHUTTER);
            }
          break;
        }
        Shutter_Direction[i] = 0;
        byte position =  Settings.shutter_invert[i] ? 100 - Settings.shutter_position[i]: Settings.shutter_position[i];
//        Response_P( JSON_SHUTTER_POS, mqtt_data, D_SHUTTER, i+1, position);
          Response_P( PSTR("{\"%s-%d\":{\"position\":%d, \"direction\":%d}}"), D_SHUTTER, i+1, position, 0/*Shutter_Direction[i]*/);
          MqttPublishPrefixTopic_P(RESULT_OR_TELE, mqtt_data);
          XdrvRulesProcess();
      }
    }
  }

}

void Shutter_StartInit (uint8_t index, uint8_t direction, int32_t target_pos)
{
  Shutter_Direction[index] = direction;
  Shutter_Target_Position[index] = target_pos;
  Shutter_Start_Position[index] = Shutter_Real_Position[index];
  shutter_time[index] = 0;
  //AddLog_P2(LOG_LEVEL_INFO,  PSTR("Start shutter: %d from %d to %d in directin %d"), index, Shutter_Start_Position[index], Shutter_Target_Position[index], Shutter_Direction[index]);
}

bool ShutterCommand()
{
  char command [CMDSZ];
  bool serviced = true;
  bool valid_entry = false;
  byte index;

  index = XdrvMailbox.index;

  int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic, kShutterCommands);
  if (-1 == command_code) {
    serviced = false;  // Unknown command
  }
  else if ( ( (CMND_OPEN == command_code) || (CMND_CLOSE == command_code) ) && (index > 0) && (index <= shutters_present)) {
    XdrvMailbox.payload = CMND_OPEN == command_code ? 100 : 0;
    last_source = SRC_WEBGUI;
    command_code = CMND_POSITION;
    Response_P( S_JSON_COMMAND_INDEX_NVALUE, command, index,1);
  }
  else if (CMND_STOP == command_code && (index > 0) && (index <= shutters_present)) {
    if (Shutter_Direction[index-1]!=0) {
      int32_t temp_realpos;
      //AddLog_P2(LOG_LEVEL_INFO,  PSTR("Stop moving shutter %d: direction:%d"), index, Shutter_Direction[index-1]);
      temp_realpos = Shutter_Start_Position[index-1] + ( (shutter_time[index-1]+10) * (Shutter_Direction[index-1] > 0 ? 100 : -Shutter_Close_Velocity[index-1]));
      XdrvMailbox.payload = realposition_to_percent(temp_realpos, index-1);
      //XdrvMailbox.payload = Settings.shuttercoeff[2][index-1] * 5 > temp_realpos ? temp_realpos / Settings.shuttercoeff[2][index-1] : (temp_realpos-Settings.shuttercoeff[0,index-1]) / Settings.shuttercoeff[1][index-1];
      last_source = SRC_WEBGUI;
      command_code = CMND_POSITION;
    }
    Response_P(S_JSON_COMMAND_INDEX_NVALUE, command, index,1);
  }
  else if (CMND_SHUTTERINVERT == command_code && (index > 0) && (index <= shutters_present)) {
    if ( (XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
      Settings.shutter_invert[index-1] = XdrvMailbox.payload;
    }
    Response_P( S_JSON_COMMAND_INDEX_NVALUE, command, index, Settings.shutter_invert[index-1]);
  }
  else if (((CMND_OPENTIME == command_code) || (CMND_CLOSETIME == command_code) ) && (index > 0) && (index <= shutters_present) ) {
    char time_chr[10];
    if (XdrvMailbox.data_len > 0) {
      if (CMND_OPENTIME == command_code) {
        Settings.shutter_opentime[index-1] = (uint16_t)(10 * CharToFloat(XdrvMailbox.data));
      } else {
        Settings.shutter_closetime[index-1] = (uint16_t)(10 * CharToFloat(XdrvMailbox.data));
      }
      ShutterInit();
    }
    dtostrfd( (double)(CMND_OPENTIME == command_code ? Settings.shutter_opentime[index-1] : Settings.shutter_closetime[index-1]) / 10  , 1, time_chr);
    Response_P( S_JSON_COMMAND_INDEX_SVALUE, command, index, time_chr );
  }
  else if ((CMND_SHUTTERRELAY == command_code) && (index > 0) && (index <= MAX_SHUTTERS)) {
    if ( (XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 64)) {
      Settings.shutter_startrelay[index-1] = XdrvMailbox.payload;
      if (XdrvMailbox.payload > 0) {
        shutter_mask |= 3 << (XdrvMailbox.payload - 1);
      } else {
        shutter_mask ^= 3 << (Settings.shutter_startrelay[index-1] - 1);
      }
      AddLog_P2(LOG_LEVEL_INFO, PSTR("Shutterrelay %d: is:%d"), index,  XdrvMailbox.payload);
      Settings.shutter_startrelay[index-1] = XdrvMailbox.payload;
      ShutterInit();
      // if payload is 0 to disable the relay there must be a reboot. Otherwhise does not work
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_NVALUE, command, index,Settings.shutter_startrelay[index-1] );
  }
  else if (CMND_SET50PERCENT == command_code && (index > 0) && (index <= shutters_present)) {
      if ( (XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
        Settings.shutter_set50percent[index-1] = Settings.shutter_invert[index-1] ? 100 - XdrvMailbox.payload : XdrvMailbox.payload;
        ShutterInit();
        Response_P(S_JSON_COMMAND_INDEX_NVALUE, command, index, XdrvMailbox.payload);
      } else {
        Response_P(S_JSON_COMMAND_INDEX_NVALUE, command, index, Settings.shutter_set50percent[index-1]);
      }
  }
  else if (CMND_SHUTTERSETCLOSE == command_code && (index > 0) && (index <= shutters_present)) {
      Shutter_Real_Position[index-1] = 0;
      Shutter_StartInit(index-1, 0, 0);
      Settings.shutter_position[index-1] = 0;
      Response_P(S_JSON_COMMAND_INDEX_NVALUE, command, index, D_CONFIGURATION_RESET);
  }
  else if ((CMND_CALIBRATIONMATIX == command_code) && (index > 0) && (index <= MAX_SHUTTERS)) {
    if (XdrvMailbox.data_len > 0) {
        Response_P(S_JSON_COMMAND_INDEX_SVALUE, command, index, XdrvMailbox.data);
    }
  }
  else {
    serviced = false;  // Unknown command
  }
  if (CMND_POSITION == command_code && (index > 0) && (index <= shutters_present)) {
    int8_t target_pos_percent;
    //limit the payload
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("shutterposition in: payload %d, index %d, source %d"), XdrvMailbox.payload , index, last_source );
    target_pos_percent = XdrvMailbox.payload < XdrvMailbox.payload ? 0 : (XdrvMailbox.payload > 100 ? 100 : XdrvMailbox.payload);
    serviced = true;
    // webgui still send also on inverted shutter the native position.
    target_pos_percent = Settings.shutter_invert[index-1] &&  SRC_WEBGUI != last_source ? 100 - target_pos_percent : target_pos_percent;
    if (target_pos_percent != -99) {
      //target_pos_percent = Settings.shutter_invert[index-1] ? 100 - target_pos_percent : target_pos_percent;
      Shutter_Target_Position[index-1] = percent_to_realposition(target_pos_percent, index-1);
      //Shutter_Target_Position[index-1] = XdrvMailbox.payload < 5 ?  Settings.shuttercoeff[2][index-1] * XdrvMailbox.payload : Settings.shuttercoeff[1][index-1] * XdrvMailbox.payload + Settings.shuttercoeff[0,index-1];
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("lastsource %d:, realpos %d, target %d, payload %d"), last_source, Shutter_Real_Position[index-1] ,Shutter_Target_Position[index-1],target_pos_percent);
    }
    if ( (target_pos_percent >= 0) && (target_pos_percent <= 100) && abs(Shutter_Target_Position[index-1] - Shutter_Real_Position[index-1] ) / Shutter_Close_Velocity[index-1] > 2) {
      int8_t new_shutterdirection = Shutter_Real_Position[index-1] < Shutter_Target_Position[index-1] ? 1 : -1;
      if (Shutter_Direction[index-1] ==  -new_shutterdirection ) {
        // direction need to be changed. on momentary switches first stop the Shutter
        if (shutterMode == PULSE_OPEN__PULSE_CLOSE) {
          // code for momentary shutters only small switch on to stop Shutter
          ExecuteCommandPower(Settings.shutter_startrelay[index-1] + (new_shutterdirection == 1 ? 0 : 1), 1, SRC_SHUTTER);
          delay(100);
        } else {
          ExecuteCommandPower(Settings.shutter_startrelay[index-1] + (new_shutterdirection == 1 ? 1 : 0), 0, SRC_SHUTTER);
          DelayForMotorStop();
        }
      }
      if (Shutter_Direction[index-1] !=  new_shutterdirection ) {
        Shutter_StartInit(index-1, new_shutterdirection, Shutter_Target_Position[index-1]);
        Shutter_Operations[index-1]++;
        if (shutterMode == OFF_ON__OPEN_CLOSE) {
          ExecuteCommandPower(Settings.shutter_startrelay[index-1] , 0, SRC_SHUTTER);
          //µAddLog_P2(LOG_LEVEL_DEBUG, PSTR("Delay5 5s, xdrv %d"), XdrvMailbox.payload);
          DelayForMotorStop();
          // Code for shutters with circuit safe configuration, switch the direction Relay
          ExecuteCommandPower(Settings.shutter_startrelay[index-1] +1, new_shutterdirection == 1 ? 0 : 1, SRC_SHUTTER);
          // power on
          ExecuteCommandPower(Settings.shutter_startrelay[index-1] , 1, SRC_SHUTTER);
        } else {
          // now start the motor for the right direction, work for momentary and normal shutters.
          AddLog_P2(LOG_LEVEL_INFO, PSTR("Start shutter in direction %d"), Shutter_Direction[index-1]);
          ExecuteCommandPower(Settings.shutter_startrelay[index-1] + (new_shutterdirection == 1 ? 0 : 1), 1, SRC_SHUTTER);
          //AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Delay6 5s, xdrv %d"), XdrvMailbox.payload);
        }
        SwitchedRelay = 0;
      }
      Response_P(S_JSON_COMMAND_INDEX_NVALUE, command, index,  Settings.shutter_invert[index-1] ? 100 - target_pos_percent : target_pos_percent);
    } else {
      target_pos_percent = realposition_to_percent(Shutter_Real_Position[index-1], index-1);
      Response_P( S_JSON_COMMAND_INDEX_NVALUE, command, index, Settings.shutter_invert[index-1]  ? 100 - target_pos_percent : target_pos_percent);
      command_code = 0;
    }
  }

  if (!serviced) {
    AddLog_P2(LOG_LEVEL_INFO, PSTR("Shutter unknown"));
  }
  return serviced;
}

void DelayForMotorStop()
{
  AddLog_P2(LOG_LEVEL_INFO, PSTR("Wait for Motorstop %d"), MOTOR_STOP_TIME);
  delay(MOTOR_STOP_TIME);
}

void Schutter_Report_Position()
{
  uint16_t shutter_moving = 0;
  for (byte i=0; i < shutters_present; i++) {
    if (Shutter_Direction[i] != 0) {
      char stemp1[20];
      char stemp2[10];
      dtostrfd((double)shutter_time[i] / 20, 1, stemp2);
      shutter_moving = 1;
      //Settings.shutter_position[i] = Settings.shuttercoeff[2][i] * 5 > Shutter_Real_Position[i] ? Shutter_Real_Position[i] / Settings.shuttercoeff[2][i] : (Shutter_Real_Position[i]-Settings.shuttercoeff[0,i]) / Settings.shuttercoeff[1][i];
      AddLog_P2(LOG_LEVEL_INFO, PSTR("Shutter %d: Real Pos: %d, Target %d, source: %s, start-pos: %d %%, direction: %d, rtcshutter: %s  [s]"), i,Shutter_Real_Position[i], Shutter_Target_Position[i], GetTextIndexed(stemp1, sizeof(stemp1), last_source, kCommandSource), Settings.shutter_position[i], Shutter_Direction[i], stemp2 );
    }
  }
  if (rules_flag.shutter_moving > shutter_moving) {
    rules_flag.shutter_moved = 1;
  } else {
    rules_flag.shutter_moved = 0;
  }
  rules_flag.shutter_moving = shutter_moving;
  AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("rules_flag.shutter_moving: %d, moved %d"), rules_flag.shutter_moving, rules_flag.shutter_moved);
}

void Shutter_Relay_changed()
{

  // SwitchedRelay = binary relay that was recently changed and cause an Action
  // powerstate_local = binary powermatrix and relays from shutter: 0..3
  // relays_changed = bool if one of the relays that belong to the shutter changed not by shutter or pulsetimer
  char stemp1[10];

	for (byte i=0; i < shutters_present; i++) {
		power_t powerstate_local = (power >> (Settings.shutter_startrelay[i] -1)) & 3;
		uint8   manual_relays_changed = ((SwitchedRelay >> (Settings.shutter_startrelay[i] -1)) & 3) && SRC_IGNORE != last_source && SRC_SHUTTER != last_source && SRC_PULSETIMER != last_source ;

		if (manual_relays_changed) {
      if (shutterMode == OFF_ON__OPEN_CLOSE) {
				switch (powerstate_local) {
					case 1:
            DelayForMotorStop();
					  Shutter_StartInit(i, 1, Shutter_Open_Max[i]);
					  break;
					case 3:
            DelayForMotorStop();
					  Shutter_StartInit(i, -1, 0);
					  break;
					default:
					  Shutter_Direction[i] = 0;
					  Shutter_Target_Position[i] = Shutter_Real_Position[i];
				  }
			} else {
        if (Shutter_Direction[i] != 0 && (!powerstate_local || (powerstate_local && shutterMode == PULSE_OPEN__PULSE_CLOSE))) {
					Shutter_Target_Position[i] = Shutter_Real_Position[i];
					AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Shutter %d: Switch OFF motor. Target: %ld, source: %s, powerstate_local %ld, switchedRelay %d, manual change %d"), i, Shutter_Target_Position[i], GetTextIndexed(stemp1, sizeof(stemp1), last_source, kCommandSource), powerstate_local,SwitchedRelay,manual_relays_changed);
				} else {
					last_source = SRC_SHUTTER; // avoid switch off in the next loop
					if (powerstate_local == 2) { // testing on CLOSE relay, if ON
					  // close with relay two
            DelayForMotorStop();
					  Shutter_StartInit(i, -1, 0);
					} else {
					  // opens with relay one
            DelayForMotorStop();
					  Shutter_StartInit(i, 1, Shutter_Open_Max[i]);
					}
				}
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("Shutter %d: Target: %ld, powerstatelocal %d"), i, Shutter_Target_Position[i], powerstate_local);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////
// Shutter specific functions
// TODO: move to shutter driver and make them accessible in a generic way

// device: 1..<numberOfShutters>
// position: 0-100
void SetShutterPosition(uint8_t device, uint8_t position)
{
  char topic [] = D_CMND_POSITION;
  XdrvMailbox.index = device;
  XdrvMailbox.data_len = 0;
  XdrvMailbox.payload = position;
  XdrvMailbox.grpflg = 0;
  XdrvMailbox.topic = topic;
  XdrvMailbox.data = NULL;
  ShutterCommand();
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_97

bool Xdrv97(uint8_t function)
{
  bool result = false;

  if (Settings.flag3.shutter_mode) {
    switch (function) {
      case FUNC_PRE_INIT:
        ShutterInit();
        break;
      case FUNC_EVERY_50_MSECOND:
        Schutter_Update_Position();
        break;
      case FUNC_EVERY_SECOND:
        Schutter_Report_Position();
        break;
      case FUNC_COMMAND:
        result = ShutterCommand();
        break;
      case FUNC_JSON_APPEND:
        for (byte i=0; i < shutters_present; i++) {
          byte position =  Settings.shutter_invert[i] ? 100 - Settings.shutter_position[i]: Settings.shutter_position[i];
          Response_P( JSON_SHUTTER_POS, mqtt_data, D_SHUTTER, i+1, position, Shutter_Direction[i]);

          #ifdef USE_DOMOTICZ
            if ((0 == tele_period) ) {
               DomoticzSensor(DZ_SHUTTER, position);
            }
          #endif  // USE_DOMOTICZ
        }
        break;
      case FUNC_SET_POWER:
        char stemp1[10];
        // extract the number of the relay that was switched and save for later in Update Position.
        SwitchedRelay = power ^ old_power;
        old_power = power;
        AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("Switched relay: %d by %s"), SwitchedRelay,GetTextIndexed(stemp1, sizeof(stemp1), last_source, kCommandSource));
        Shutter_Relay_changed();
      break;
    }
  }
  return result;
}

#endif
