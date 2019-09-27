/*
  xdrv_92_pid.ino - PID algorithm plugin for Sonoff-Tasmota
  Copyright (C) 2018 Colin Law and Thomas Herrmann
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

/**
 * Code to
 *
 * Usage:
 * Place this file in the sonoff folder.
 * Clone the library https://github.com/colinl/process-control.git from Github
 * into a subfolder of lib.
 * If you want to use a time proportioned relay output with this then also get
 * xdrv_91_timeprop.ino
 * In user_config.h or user_config_override.h include code as follows:

 #define USE_PID         // include the pid feature (+4.3k)
   #define PID_SETPOINT                  19.5    // Setpoint value. This is the process value that the process is
                                                 // aiming for.
                                                 // May be adjusted via MQTT using cmnd pid_sp

   #define PID_PROPBAND                  5       // Proportional band in process units (eg degrees). This controls
                                                 // the gain of the loop and is the range of process value over which
                                                 // the power output will go from 0 to full power. The units are that
                                                 // of the process and setpoint, so for example in a heating
                                                 // application it might be set to 1.5 degrees.
                                                 // May be adjusted via MQTT using cmnd pid_pb

   #define PID_INTEGRAL_TIME             1800    // Integral time seconds. This is a setting for the integral time,
                                                 // in seconds. It represents the time constant of the integration
                                                 // effect. The larger the value the slower the integral effect will be.
                                                 // Obviously the slower the process is the larger this should be. For
                                                 // example for a domestic room heated by convection radiators a setting
                                                 // of one hour might be appropriate (in seconds). To disable the
                                                 // integral effect set this to a large number.
                                                 // May be adjusted via MQTT using cmnd pid_ti

   #define PID_DERIVATIVE_TIME           15      // Derivative time seconds. This is a setting for the derivative time,
                                                 // in seconds. It represents the time constant of the derivative effect.
                                                 // The larger the value the greater will be the derivative effect.
                                                 // Typically this will be set to somewhat less than 25% of the integral
                                                 // setting, once the integral has been adjusted to the optimum value. To
                                                 // disable the derivative effect set this to 0. When initially tuning a
                                                 // loop it is often sensible to start with derivative zero and wind it in
                                                 // once other parameters have been setup.
                                                 // May be adjusted via MQTT using cmnd pid_td

   #define PID_INITIAL_INT               0.5     // Initial integral value (0:1). This is an initial value which is used
                                                 // to preset the integrated error value when the flow is deployed in
                                                 // order to assist in homing in on the setpoint the first time. It should
                                                 // be set to an estimate of what the power requirement might be in order
                                                 // to maintain the process at the setpoint. For example for a domestic
                                                 // room heating application it might be set to 0.2 indicating that 20% of
                                                 // the available power might be required to maintain the setpoint. The
                                                 // value is of no consequence apart from device restart.

   #define PID_MAX_INTERVAL              300     // This is the maximum time in seconds that is expected between samples.
                                                 // It is provided to cope with unusual situations such as a faulty sensor
                                                 // that might prevent the node from being supplied with a process value.
                                                 // If no new process value is received for this time then the power is set
                                                 // to the value defined for PID_MANUAL_POWER.
                                                 // May be adjusted via MQTT using cmnd pid_max_interval

   #define PID_DERIV_SMOOTH_FACTOR       3       // In situations where the process sensor has limited resolution (such as
                                                 // the DS18B20), the use of deriviative can be problematic as when the
                                                 // process is changing only slowly the steps in the value cause spikes in
                                                 // the derivative. To reduce the effect of these this parameter can be
                                                 // set to apply a filter to the derivative term. I have found that with
                                                 // the DS18B20 that a value of 3 here can be beneficial, providing
                                                 // effectively a low pass filter on the derivative at 1/3 of the derivative
                                                 // time. This feature may also be useful if the process value is particularly
                                                 // noisy. The smaller the value the greater the filtering effect but the
                                                 // more it will reduce the effectiveness of the derivative. A value of zero
                                                 // disables this feature.
                                                 // May be adjusted via MQTT using cmnd pid_d_smooth

   #define PID_AUTO                      1       // Auto mode 1 or 0 (for manual). This can be used to enable or disable
                                                 // the control (1=enable, auto mode, 0=disabled, manual mode). When in
                                                 // manual mode the output is set the value definded for PID_MANUAL_POWER
                                                 // May be adjusted via MQTT using cmnd pid_auto

   #define PID_MANUAL_POWER              0       // Power output when in manual mode or fallback mode if too long elapses
                                                 // between process values
                                                 // May be adjusted via MQTT using cmnd pid_manual_power

   #define PID_UPDATE_SECS               0       // How often to run the pid algorithm (integer secs) or 0 to run the algorithm
                                                 // each time a new pv value is received, for most applictions specify 0.
                                                 // Otherwise set this to a time
                                                 // that is short compared to the response of the process.  For example,
                                                 // something like 15 seconds may well be appropriate for a domestic room
                                                 // heating application.
                                                 // May be adjusted via MQTT using cmnd pid_update_secs

   #define PID_USE_TIMPROP               1       // To use an internal relay for a time proportioned output to drive the
                                                 // process, set this to indicate which timeprop output to use. For a device
                                                 // with just one relay then this will be 1.
                                                 // It is then also necessary to define USE_TIMEPROP and set the output up as
                                                 // explained in xdrv_91_timeprop.ino
                                                 // To disable this feature leave this undefined (undefined, not defined to nothing).

   #define PID_USE_LOCAL_SENSOR                  // if defined then the local sensor will be used for pv. Leave undefined if
                                                 // this is not required.  The rate that the sensor is read is defined by TELE_PERIOD
                                                 // If not using the sensor then you can supply process values via MQTT using
                                                 // cmnd pid_pv
    #define PID_USE_CUSTOMTOPIC "shutterposition"    // if defined the result of the PID process will be send as MQTT message the defined
                                                     // topic.

 * Help with using the PID algorithm and with loop tuning can be found at
 * http://blog.clanlaw.org.uk/2018/01/09/PID-tuning-with-node-red-contrib-pid.html
 * This is directed towards using the algorithm in the node-red node node-red-contrib-pid but the algorithm here is based on
 * the code there and the tuning techique described there should work just the same.

 *
**/


#ifdef USE_PID

# include "PID.h"

#define D_CMND_PID "pid_"

#define D_CMND_PID_SETPV "pv"
#define D_CMND_PID_SETSETPOINT "sp"
#define D_CMND_PID_SETPROPBAND "pb"
#define D_CMND_PID_SETINTEGRAL_TIME "ti"
#define D_CMND_PID_SETDERIVATIVE_TIME "td"
#define D_CMND_PID_SETINITIAL_INT "initint"
#define D_CMND_PID_SETDERIV_SMOOTH_FACTOR "d_smooth"
#define D_CMND_PID_SETAUTO "auto"
#define D_CMND_PID_SETMANUAL_POWER "manual_power"
#define D_CMND_PID_SETMAX_INTERVAL "max_interval"
#define D_CMND_PID_SETUPDATE_SECS "update_secs"
#define D_CMND_PID_SHOWSENSOR "PID_Show_Sensor"
#define D_CMND_PID_COMMAND "PID command "
#define D_CMND_PID_RESULTNAME "power"

enum PIDCommands { CMND_PID_SETPV, CMND_PID_SETSETPOINT, CMND_PID_SETPROPBAND, CMND_PID_SETINTEGRAL_TIME,
  CMND_PID_SETDERIVATIVE_TIME, CMND_PID_SETINITIAL_INT, CMND_PID_SETDERIV_SMOOTH_FACTOR, CMND_PID_SETAUTO,
  CMND_PID_SETMANUAL_POWER, CMND_PID_SETMAX_INTERVAL, CMND_PID_SETUPDATE_SECS };
const char kPIDCommands[] PROGMEM = D_CMND_PID_SETPV "|" D_CMND_PID_SETSETPOINT "|" D_CMND_PID_SETPROPBAND "|"
  D_CMND_PID_SETINTEGRAL_TIME "|" D_CMND_PID_SETDERIVATIVE_TIME "|" D_CMND_PID_SETINITIAL_INT "|" D_CMND_PID_SETDERIV_SMOOTH_FACTOR "|"
  D_CMND_PID_SETAUTO "|" D_CMND_PID_SETMANUAL_POWER "|" D_CMND_PID_SETMAX_INTERVAL "|" D_CMND_PID_SETUPDATE_SECS;

#define MAX_PIID 4

static PID pid[MAX_PIID];
static int update_secs = PID_UPDATE_SECS <= 0  ?  0  :  PID_UPDATE_SECS;   // how often (secs) the pid alogorithm is run
static int max_interval = PID_MAX_INTERVAL;
static unsigned long last_pv_update_secs = 0;
static bool run_pid_now = false;     // tells PID_Every_Second to run the pid algorithm

void PID_Init()
{
  AddLog_P2(LOG_LEVEL_INFO, "PID Init");
  for (byte i=0; i<MAX_PIID; i++) {
    pid[0].initialise( PID_SETPOINT, PID_PROPBAND, PID_INTEGRAL_TIME, PID_DERIVATIVE_TIME, PID_INITIAL_INT,
      PID_MAX_INTERVAL, PID_DERIV_SMOOTH_FACTOR, PID_AUTO, PID_MANUAL_POWER );
  }
}

void PID_Every_Second() {
  static int sec_counter = 0;
  // run the pid algorithm if run_pid_now is true or if the right number of seconds has passed or if too long has
  // elapsed since last pv update. If too long has elapsed the the algorithm will deal with that.
  if (run_pid_now  ||  utc_time - last_pv_update_secs > max_interval  ||  (update_secs != 0 && sec_counter++ % update_secs  ==  0)) {
    run_pid();
    run_pid_now = false;
  }
}

void PID_Show_Sensor() {
  // Called each time new sensor data available, data in mqtt data in same format
  // as published in tele/SENSOR
  // Update period is specified in TELE_PERIOD
  // e.g. "{"Time":"2018-03-13T16:48:05","DS18B20":{"Temperature":22.0},"TempUnit":"C"}"
  AddLog_P2(LOG_LEVEL_INFO, D_CMND_PID_SHOWSENSOR ": mqtt_data: %s", mqtt_data);
  StaticJsonBuffer<400> jsonBuffer;
  // force mqtt_data to read only to stop parse from overwriting it
  JsonObject& data_json = jsonBuffer.parseObject((const char*)mqtt_data);
  if (data_json.success()) {
    const char* value = data_json["DS18B20-1"]["Temperature"];
    // check that something was found and it contains a number
    if (value != NULL && strlen(value) > 0 && (isdigit(value[0]) || (value[0] == '-' && isdigit(value[1])) ) ) {
      AddLog_P2(LOG_LEVEL_INFO, D_CMND_PID_SHOWSENSOR ": Temperature: %s", value);

      // pass the value to the pid alogorithm to use as current pv
      last_pv_update_secs = utc_time;
      pid[0].setPv(atof(value), last_pv_update_secs);
      // also trigger running the pid algorithm if we have been told to run it each pv sample
      if (update_secs == 0) {
        // this runs it at the next second
        run_pid_now = true;
      }
    } else {
      AddLog_P2(LOG_LEVEL_INFO,  D_CMND_PID_SHOWSENSOR ": no temperature found");
    }
  } else  {
    // parse failed
    AddLog_P2(LOG_LEVEL_INFO,  D_CMND_PID_SHOWSENSOR ": json parse failed");
  }
}


/* struct XDRVMAILBOX { */
/*   uint16_t      valid; */
/*   uint16_t      index; */
/*   uint16_t      data_len; */
/*   int16_t       payload; */
/*   char         *topic; */
/*   char         *data; */
/* } XdrvMailbox; */

bool PID_Command()
{
  char command [CMDSZ];
  bool serviced = true;
  uint8_t ua_prefix_len = strlen(D_CMND_PID); // to detect prefix of command

  AddLog_P2(LOG_LEVEL_INFO, "Command called: "
    "index: %d data_len: %d payload: %d topic: %s data: %s",
    XdrvMailbox.index,
    XdrvMailbox.data_len,
    XdrvMailbox.payload,
    (XdrvMailbox.payload >= 0 ? XdrvMailbox.topic : ""),
    (XdrvMailbox.data_len >= 0 ? XdrvMailbox.data : ""));


  if (0 == strncasecmp_P(XdrvMailbox.topic, PSTR(D_CMND_PID), ua_prefix_len) && XdrvMailbox.index <= MAX_PIID) {
    // command starts with pid_
    int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic + ua_prefix_len, kPIDCommands);
    serviced = true;
    switch (command_code) {
      case CMND_PID_SETPV:
        AddLog_P2(LOG_LEVEL_INFO, D_CMND_PID_COMMAND "setpv");

        last_pv_update_secs = utc_time;
        pid[XdrvMailbox.index-1].setPv(atof(XdrvMailbox.data), last_pv_update_secs);
        // also trigger running the pid algorithm if we have been told to run it each pv sample
        if (update_secs == 0) {
          // this runs it at the next second
          run_pid_now = true;
        }
        break;

      case CMND_PID_SETSETPOINT:
        AddLog_P2(LOG_LEVEL_INFO, D_CMND_PID_COMMAND "setsetpoint");
        pid[XdrvMailbox.index-1].setSp(atof(XdrvMailbox.data));
        break;

      case CMND_PID_SETPROPBAND:
        AddLog_P2(LOG_LEVEL_INFO, D_CMND_PID_COMMAND "propband");

        pid[XdrvMailbox.index-1].setPb(atof(XdrvMailbox.data));
        break;

      case CMND_PID_SETINTEGRAL_TIME:
        AddLog_P2(LOG_LEVEL_INFO, D_CMND_PID_COMMAND "Ti");

        pid[XdrvMailbox.index-1].setTi(atof(XdrvMailbox.data));
        break;

      case CMND_PID_SETDERIVATIVE_TIME:
        AddLog_P2(LOG_LEVEL_INFO, D_CMND_PID_COMMAND "Td");

        pid[XdrvMailbox.index-1].setTd(atof(XdrvMailbox.data));
        break;

      case CMND_PID_SETINITIAL_INT:
        AddLog_P2(LOG_LEVEL_INFO, D_CMND_PID_COMMAND "initial int");

        pid[XdrvMailbox.index-1].setInitialInt(atof(XdrvMailbox.data));
        break;

      case CMND_PID_SETDERIV_SMOOTH_FACTOR:
        AddLog_P2(LOG_LEVEL_INFO, D_CMND_PID_COMMAND "deriv smooth");

        pid[XdrvMailbox.index-1].setDSmooth(atof(XdrvMailbox.data));
        break;

      case CMND_PID_SETAUTO:
        AddLog_P2(LOG_LEVEL_INFO, D_CMND_PID_COMMAND "auto");

        pid[XdrvMailbox.index-1].setAuto(atoi(XdrvMailbox.data));
        break;

      case CMND_PID_SETMANUAL_POWER:
        AddLog_P2(LOG_LEVEL_INFO, D_CMND_PID_COMMAND "manual power");

        pid[XdrvMailbox.index-1].setManualPower(atof(XdrvMailbox.data));
        break;

      case CMND_PID_SETMAX_INTERVAL:
      AddLog_P2(LOG_LEVEL_INFO, D_CMND_PID_COMMAND "max interval");

      max_interval = atoi(XdrvMailbox.data);
      pid[XdrvMailbox.index-1].setMaxInterval(max_interval);
      break;

      case CMND_PID_SETUPDATE_SECS:
        AddLog_P2(LOG_LEVEL_INFO, D_CMND_PID_COMMAND "update secs");

        update_secs = atoi(XdrvMailbox.data) ;
        if (update_secs < 0) update_secs = 0;
        break;

      default:
        serviced = false;
  }

    if (serviced) {
      // set mqtt RESULT
      ResponseAppend_P(PSTR("{\"%s\":\"%s\"}"), XdrvMailbox.topic, XdrvMailbox.data);
    }

  } else {
    serviced = false;
  }
  return serviced;
}

static void run_pid()
{
  double power = pid[0].tick(utc_time);
  char buf[10];
  dtostrfd(power, 3, buf);
  // implement global char an publish only on change
  ResponseAppend_P(PSTR("{\"%s\":\"%s\"}"), D_CMND_PID_RESULTNAME, buf);
  MqttPublishPrefixTopic_P(TELE, "PID", false);

#if defined PID_USE_CUSTOMTOPIC
  dtostrfd(power*100, 0, buf);
  ResponseAppend_P( PSTR(PID_USE_CUSTOMTOPIC " %s"), buf);
  ExecuteCommand(mqtt_data, SRC_IGNORE);
#endif

#if defined PID_USE_TIMPROP
    // send power to appropriate timeprop output
    //Timeprop_Set_Power( PID_USE_TIMPROP-1, power );
#endif // PID_USE_TIMPROP
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_92 92

bool Xdrv92(uint8_t function)
{
  bool result = false;

  switch (function) {
  case FUNC_INIT:
    PID_Init();
    break;
  case FUNC_EVERY_SECOND:
    PID_Every_Second();
    break;
  case FUNC_SHOW_SENSOR:
    // only use this if the pid loop is to use the local sensor for pv
    #if defined PID_USE_LOCAL_SENSOR
      PID_Show_Sensor();
    #endif // PID_USE_LOCAL_SENSOR
    break;
  case FUNC_COMMAND:
    result = PID_Command();
    break;
  }
  return result;
}

#endif // USE_TIMEPROP
