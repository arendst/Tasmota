/*
  xdrv_13_controller.ino - Controller Support with TimeProp and PID for Sonoff-Tasmota

  Copyright (C) 2018  Colin Law, Thomas Herrmann and Adrian Scillato

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

enum PIDCommands { CMND_PID_SETPV, CMND_PID_SETSETPOINT, CMND_PID_SETPROPBAND, CMND_PID_SETINTEGRAL_TIME,
  CMND_PID_SETDERIVATIVE_TIME, CMND_PID_SETINITIAL_INT, CMND_PID_SETDERIV_SMOOTH_FACTOR, CMND_PID_SETAUTO,
  CMND_PID_SETMANUAL_POWER, CMND_PID_SETMAX_INTERVAL, CMND_PID_SETUPDATE_SECS };
const char kPIDCommands[] PROGMEM = D_CMND_PID_SETPV "|" D_CMND_PID_SETSETPOINT "|" D_CMND_PID_SETPROPBAND "|"
  D_CMND_PID_SETINTEGRAL_TIME "|" D_CMND_PID_SETDERIVATIVE_TIME "|" D_CMND_PID_SETINITIAL_INT "|" D_CMND_PID_SETDERIV_SMOOTH_FACTOR "|"
  D_CMND_PID_SETAUTO "|" D_CMND_PID_SETMANUAL_POWER "|" D_CMND_PID_SETMAX_INTERVAL "|" D_CMND_PID_SETUPDATE_SECS;

static PID pid;
static int update_secs = PID_UPDATE_SECS <= 0  ?  0  :  PID_UPDATE_SECS;   // how often (secs) the pid alogorithm is run
static int max_interval = PID_MAX_INTERVAL;
static unsigned long last_pv_update_secs = 0;
static boolean run_pid_now = false;     // tells PID_Every_Second to run the pid algorithm

void PID_Init()
{
  snprintf_P(log_data, sizeof(log_data), "PID Init");
  AddLog(LOG_LEVEL_INFO);
  pid.initialise( PID_SETPOINT, PID_PROPBAND, PID_INTEGRAL_TIME, PID_DERIVATIVE_TIME, PID_INITIAL_INT,
    PID_MAX_INTERVAL, PID_DERIV_SMOOTH_FACTOR, PID_AUTO, PID_MANUAL_POWER );
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
  snprintf_P(log_data, sizeof(log_data), "PID_Show_Sensor: mqtt_data: %s", mqtt_data);
  AddLog(LOG_LEVEL_INFO);
  StaticJsonBuffer<400> jsonBuffer;
  // force mqtt_data to read only to stop parse from overwriting it
  JsonObject& data_json = jsonBuffer.parseObject((const char*)mqtt_data);
  if (data_json.success()) {
    const char* value = data_json["DS18B20"]["Temperature"];
    // check that something was found and it contains a number
    //if (value != NULL  &&  strlen(value) > 0  &&  isdigit(value[0]) ) {
    if (value != NULL && strlen(value) > 0 && (isdigit(value[0]) || (value[0] == '-' && isdigit(value[1])) ) ) {
      snprintf_P(log_data, sizeof(log_data), "PID_Show_Sensor: Temperature: %s", value);
      AddLog(LOG_LEVEL_INFO);
      // pass the value to the pid alogorithm to use as current pv
      last_pv_update_secs = utc_time;
      pid.setPv(atof(value), last_pv_update_secs);
      // also trigger running the pid algorithm if we have been told to run it each pv sample
      if (update_secs == 0) {
        // this runs it at the next second
        run_pid_now = true;
      }
    } else {
      snprintf_P(log_data, sizeof(log_data), "PID_Show_Sensor - no temperature found");
      AddLog(LOG_LEVEL_INFO);
    }
  } else  {
    // parse failed
    snprintf_P(log_data, sizeof(log_data), "PID_Show_Sensor - json parse failed");
    AddLog(LOG_LEVEL_INFO);
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

boolean PID_Command()
{
  char command [CMDSZ];
  boolean serviced = true;
  uint8_t ua_prefix_len = strlen(D_CMND_PID); // to detect prefix of command

  snprintf_P(log_data, sizeof(log_data), "Command called: "
    "index: %d data_len: %d payload: %d topic: %s data: %s",
    XdrvMailbox.index,
    XdrvMailbox.data_len,
    XdrvMailbox.payload,
    (XdrvMailbox.payload >= 0 ? XdrvMailbox.topic : ""),
    (XdrvMailbox.data_len >= 0 ? XdrvMailbox.data : ""));
  AddLog(LOG_LEVEL_INFO);

  if (0 == strncasecmp_P(XdrvMailbox.topic, PSTR(D_CMND_PID), ua_prefix_len)) {
    // command starts with pid_
    int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic + ua_prefix_len, kPIDCommands);
    serviced = true;
    switch (command_code) {
      case CMND_PID_SETPV:
        snprintf_P(log_data, sizeof(log_data), "PID command setpv");
        AddLog(LOG_LEVEL_INFO);
        last_pv_update_secs = utc_time;
        pid.setPv(atof(XdrvMailbox.data), last_pv_update_secs);
        // also trigger running the pid algorithm if we have been told to run it each pv sample
        if (update_secs == 0) {
          // this runs it at the next second
          run_pid_now = true;
        }
        break;

      case CMND_PID_SETSETPOINT:
        snprintf_P(log_data, sizeof(log_data), "PID command setsetpoint");
        AddLog(LOG_LEVEL_INFO);
        pid.setSp(atof(XdrvMailbox.data));
        break;

      case CMND_PID_SETPROPBAND:
        snprintf_P(log_data, sizeof(log_data), "PID command propband");
        AddLog(LOG_LEVEL_INFO);
        pid.setPb(atof(XdrvMailbox.data));
        break;

      case CMND_PID_SETINTEGRAL_TIME:
        snprintf_P(log_data, sizeof(log_data), "PID command Ti");
        AddLog(LOG_LEVEL_INFO);
        pid.setTi(atof(XdrvMailbox.data));
        break;

      case CMND_PID_SETDERIVATIVE_TIME:
        snprintf_P(log_data, sizeof(log_data), "PID command Td");
        AddLog(LOG_LEVEL_INFO);
        pid.setTd(atof(XdrvMailbox.data));
        break;

      case CMND_PID_SETINITIAL_INT:
        snprintf_P(log_data, sizeof(log_data), "PID command initial int");
        AddLog(LOG_LEVEL_INFO);
        pid.setInitialInt(atof(XdrvMailbox.data));
        break;

      case CMND_PID_SETDERIV_SMOOTH_FACTOR:
        snprintf_P(log_data, sizeof(log_data), "PID command deriv smooth");
        AddLog(LOG_LEVEL_INFO);
        pid.setDSmooth(atof(XdrvMailbox.data));
        break;

      case CMND_PID_SETAUTO:
        snprintf_P(log_data, sizeof(log_data), "PID command auto");
        AddLog(LOG_LEVEL_INFO);
        pid.setAuto(atoi(XdrvMailbox.data));
        break;

      case CMND_PID_SETMANUAL_POWER:
        snprintf_P(log_data, sizeof(log_data), "PID command manual power");
        AddLog(LOG_LEVEL_INFO);
        pid.setManualPower(atof(XdrvMailbox.data));
        break;

      case CMND_PID_SETMAX_INTERVAL:
      snprintf_P(log_data, sizeof(log_data), "PID command set max interval");
      AddLog(LOG_LEVEL_INFO);
      max_interval = atoi(XdrvMailbox.data);
      pid.setMaxInterval(max_interval);
      break;

      case CMND_PID_SETUPDATE_SECS:
        snprintf_P(log_data, sizeof(log_data), "PID command set update secs");
        AddLog(LOG_LEVEL_INFO);
        update_secs = atoi(XdrvMailbox.data) ;
        if (update_secs < 0) update_secs = 0;
        break;

      default:
        serviced = false;
  }

    if (serviced) {
      // set mqtt RESULT
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"%s\":\"%s\"}"), XdrvMailbox.topic, XdrvMailbox.data);
    }

  } else {
    serviced = false;
  }
  return serviced;
}

static void run_pid()
{
  double power = pid.tick(utc_time);
  char buf[10];
  dtostrfd(power, 3, buf);
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"%s\":\"%s\"}"), "power", buf);
  MqttPublishPrefixTopic_P(TELE, "PID", false);
#if defined PID_USE_TIMPROP
    // send power to appropriate timeprop output
    Timeprop_Set_Power( PID_USE_TIMPROP-1, power );
#endif // PID_USE_TIMPROP
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_92

boolean Xdrv92(byte function)
{
  boolean result = false;

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

#endif // USE_PID







/*
  xdrv_91_timeprop.ino - Timeprop support for Sonoff-Tasmota
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
 * Code to drive one or more relays in a time proportioned manner give a
 * required power value.
 *
 * Given required power values in the range 0.0 to 1.0 the relays will be
 * driven on/off in such that the average power suppled will represent
 * the required power.
 * The cycle time is configurable.  If, for example, the
 * period is set to 10 minutes and the power input is 0.2 then the output will
 * be on for two minutes in every ten minutes.
 *
 * A value for actuator dead time may be provided. If you have a device that
 * takes a significant time to open/close then set this to the average of the
 * open and close times.  The algorithim will then adjust the output timing
 * accordingly to ensure that the output is not switched more rapidly than
 * the actuator can cope with.
 *
 * A facility to invert the output is provided which can be useful when used in
 * refrigeration processes and similar.
 *
 * In the case where only one relay is being driven the power value is set by
 * writing the value to the mqtt topic cmnd/timeprop_setpower_0.  If more than
 * one relay is being driven (as might be the case for a heat/cool application
 * where one relay drives the heater and the other the cooler) then the power
 * for the second relay is written to topic cmnd/timeprop_setpower_1 and so on.
 *
 * To cope with the problem of temporary wifi failure etc a
 * TIMEPROP_MAX_UPDATE_INTERVALS value is available. This can be set to the max
 * expected time between power updates and if this time is exceeded then the
 * power will fallback to a given safe value until a new value is provided. Set
 * the interval to 0 to disable this feature.
 *
 * Usage:
 * Place this file in the sonoff folder.
 * Clone the library https://github.com/colinl/process-control.git from Github
 * into a subfolder of lib.
 * In user_config.h or user_config_override.h for a single relay, include
 * code as follows:

 #define USE_TIMEPROP    //  include the timeprop feature (+1.2k)
   // for single output
   #define TIMEPROP_NUM_OUTPUTS          1       // how many outputs to control (with separate alogorithm for each)
   #define TIMEPROP_CYCLETIMES           60      // cycle time seconds
   #define TIMEPROP_DEADTIMES            0       // actuator action time seconds
   #define TIMEPROP_OPINVERTS            false   // whether to invert the output
   #define TIMEPROP_FALLBACK_POWERS      0       // falls back to this if too long betwen power updates
   #define TIMEPROP_MAX_UPDATE_INTERVALS 120     // max no secs that are allowed between power updates (0 to disable)
   #define TIMEPROP_RELAYS               1       // which relay to control 1:8

 * or for two relays:
 #define USE_TIMEPROP    //  include the timeprop feature (+1.2k)
   // for single output
   #define TIMEPROP_NUM_OUTPUTS          2               // how many outputs to control (with separate alogorithm for each)
   #define TIMEPROP_CYCLETIMES           60,     10      // cycle time seconds
   #define TIMEPROP_DEADTIMES            0,      0       // actuator action time seconds
   #define TIMEPROP_OPINVERTS            false,  false   // whether to invert the output
   #define TIMEPROP_FALLBACK_POWERS      0,      0       // falls back to this if too long betwen power updates
   #define TIMEPROP_MAX_UPDATE_INTERVALS 120,    120     // max no secs that are allowed between power updates (0 to disable)
   #define TIMEPROP_RELAYS               1,      2       // which relay to control 1:8

 * Publish values between 0 and 1 to the topic(s) described above
 *
**/


#ifdef USE_TIMEPROP

# include "Timeprop.h"

#define D_CMND_TIMEPROP "timeprop_"
#define D_CMND_TIMEPROP_SETPOWER "setpower_"    // add index no on end (0:8) and data is power 0:1

enum TimepropCommands { CMND_TIMEPROP_SETPOWER };
const char kTimepropCommands[] PROGMEM = D_CMND_TIMEPROP_SETPOWER;

static Timeprop timeprops[TIMEPROP_NUM_OUTPUTS];
static int relayNos[TIMEPROP_NUM_OUTPUTS] = {TIMEPROP_RELAYS};
static long currentRelayStates = 0;  // current actual relay states. Bit 0 first relay

/* call this from elsewhere if required to set the power value for one of the timeprop instances */
/* index specifies which one, 0 up */
void Timeprop_Set_Power( int index, float power )
{
  if (index >= 0  &&  index < TIMEPROP_NUM_OUTPUTS)
  {
    timeprops[index].setPower( power, utc_time);
  }
}

void Timeprop_Init()
{
  snprintf_P(log_data, sizeof(log_data), "Timeprop Init");
  AddLog(LOG_LEVEL_INFO);
  int cycleTimes[TIMEPROP_NUM_OUTPUTS] = {TIMEPROP_CYCLETIMES};
  int deadTimes[TIMEPROP_NUM_OUTPUTS] = {TIMEPROP_DEADTIMES};
  int opInverts[TIMEPROP_NUM_OUTPUTS] = {TIMEPROP_OPINVERTS};
  int fallbacks[TIMEPROP_NUM_OUTPUTS] = {TIMEPROP_FALLBACK_POWERS};
  int maxIntervals[TIMEPROP_NUM_OUTPUTS] = {TIMEPROP_MAX_UPDATE_INTERVALS};

  for (int i=0; i<TIMEPROP_NUM_OUTPUTS; i++) {
    timeprops[i].initialise(cycleTimes[i], deadTimes[i], opInverts[i], fallbacks[i],
      maxIntervals[i], utc_time);
  }
}

void Timeprop_Every_Second() {
  for (int i=0; i<TIMEPROP_NUM_OUTPUTS; i++) {
    int newState = timeprops[i].tick(utc_time);
    if (newState != bitRead(currentRelayStates, relayNos[i]-1)){
      ExecuteCommandPower(relayNos[i], newState);
    }
  }
}

// called by the system each time a relay state is changed
void Timeprop_Xdrv_Power() {
  // for a single relay the state is in the lsb of index, I have think that for
  // multiple outputs then succesive bits will hold the state but have not been
  // able to test that
  currentRelayStates = XdrvMailbox.index;
}

/* struct XDRVMAILBOX { */
/*   uint16_t      valid; */
/*   uint16_t      index; */
/*   uint16_t      data_len; */
/*   int16_t       payload; */
/*   char         *topic; */
/*   char         *data; */
/* } XdrvMailbox; */

// To get here post with topic cmnd/timeprop_setpower_n where n is index into timeprops 0:7
boolean Timeprop_Command()
{
  char command [CMDSZ];
  boolean serviced = true;
  uint8_t ua_prefix_len = strlen(D_CMND_TIMEPROP); // to detect prefix of command
  /*
  snprintf_P(log_data, sizeof(log_data), "Command called: "
    "index: %d data_len: %d payload: %d topic: %s data: %s\n",
    XdrvMailbox.index,
    XdrvMailbox.data_len,
    XdrvMailbox.payload,
    (XdrvMailbox.payload >= 0 ? XdrvMailbox.topic : ""),
    (XdrvMailbox.data_len >= 0 ? XdrvMailbox.data : ""));

    AddLog(LOG_LEVEL_INFO);
  */
  if (0 == strncasecmp_P(XdrvMailbox.topic, PSTR(D_CMND_TIMEPROP), ua_prefix_len)) {
    // command starts with timeprop_
    int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic + ua_prefix_len, kTimepropCommands);
    if (CMND_TIMEPROP_SETPOWER == command_code) {
      /*
      snprintf_P(log_data, sizeof(log_data), "Timeprop command timeprop_setpower: "
        "index: %d data_len: %d payload: %d topic: %s data: %s",
	      XdrvMailbox.index,
	      XdrvMailbox.data_len,
	      XdrvMailbox.payload,
	      (XdrvMailbox.payload >= 0 ? XdrvMailbox.topic : ""),
	      (XdrvMailbox.data_len >= 0 ? XdrvMailbox.data : ""));
        AddLog(LOG_LEVEL_INFO);
      */
      if (XdrvMailbox.index >=0 && XdrvMailbox.index < TIMEPROP_NUM_OUTPUTS) {
        timeprops[XdrvMailbox.index].setPower( atof(XdrvMailbox.data), utc_time );
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_TIMEPROP D_CMND_TIMEPROP_SETPOWER "%d\":\"%s\"}"),
        XdrvMailbox.index, XdrvMailbox.data);
    }
    else {
      serviced = false;
    }
  } else {
    serviced = false;
  }
  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_91

boolean Xdrv91(byte function)
{
  boolean result = false;

  switch (function) {
  case FUNC_INIT:
    Timeprop_Init();
    break;
  case FUNC_EVERY_SECOND:
    Timeprop_Every_Second();
    break;
  case FUNC_COMMAND:
    result = Timeprop_Command();
    break;
  case FUNC_SET_POWER:
    Timeprop_Xdrv_Power();
    break;
  }
  return result;
}

#endif // USE_TIMEPROP
