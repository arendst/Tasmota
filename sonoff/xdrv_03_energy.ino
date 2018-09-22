/*
  xdrv_03_energy.ino - Energy sensor support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends

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

#ifdef USE_ENERGY_SENSOR
/*********************************************************************************************\
 * Energy
\*********************************************************************************************/

#define ENERGY_NONE             0

#define FEATURE_POWER_LIMIT  true

enum EnergyCommands {
  CMND_POWERDELTA,
  CMND_POWERLOW, CMND_POWERHIGH, CMND_VOLTAGELOW, CMND_VOLTAGEHIGH, CMND_CURRENTLOW, CMND_CURRENTHIGH,
  CMND_POWERSET, CMND_VOLTAGESET, CMND_CURRENTSET, CMND_FREQUENCYSET,
  CMND_ENERGYRESET, CMND_MAXENERGY, CMND_MAXENERGYSTART,
  CMND_MAXPOWER, CMND_MAXPOWERHOLD, CMND_MAXPOWERWINDOW,
  CMND_SAFEPOWER, CMND_SAFEPOWERHOLD, CMND_SAFEPOWERWINDOW };
const char kEnergyCommands[] PROGMEM =
  D_CMND_POWERDELTA "|"
  D_CMND_POWERLOW "|" D_CMND_POWERHIGH "|" D_CMND_VOLTAGELOW "|" D_CMND_VOLTAGEHIGH "|" D_CMND_CURRENTLOW "|" D_CMND_CURRENTHIGH "|"
  D_CMND_POWERSET "|" D_CMND_VOLTAGESET "|" D_CMND_CURRENTSET "|" D_CMND_FREQUENCYSET "|"
  D_CMND_ENERGYRESET "|" D_CMND_MAXENERGY "|" D_CMND_MAXENERGYSTART "|"
  D_CMND_MAXPOWER "|" D_CMND_MAXPOWERHOLD "|" D_CMND_MAXPOWERWINDOW "|"
  D_CMND_SAFEPOWER "|" D_CMND_SAFEPOWERHOLD "|"  D_CMND_SAFEPOWERWINDOW ;

float energy_voltage = 0;         // 123.1 V
float energy_current = 0;         // 123.123 A
float energy_power = 0;           // 123.1 W
float energy_power_factor = NAN;  // 0.12
int energy_calc_power_factor = 0; // Do not calculate power factor from data
float energy_frequency = NAN;     // 123.1 Hz
float energy_start = 0;           // 12345.12345 kWh total previous

float energy_daily = 0;           // 123.123 kWh
float energy_total = 0;           // 12345.12345 kWh
unsigned long energy_kWhtoday_delta = 0;  // 1212312345 Wh 10^-5 (deca micro Watt hours) - Overflows to energy_kWhtoday (HLW and CSE only)
unsigned long energy_kWhtoday;    // 12312312 Wh * 10^-2 (deca milli Watt hours) - 5764 = 0.05764 kWh = 0.058 kWh = energy_daily
unsigned long energy_period = 0;  // 12312312 Wh * 10^-2 (deca milli Watt hours) - 5764 = 0.05764 kWh = 0.058 kWh = energy_daily

float energy_power_last[3] = { 0 };
uint8_t energy_power_delta = 0;

bool energy_power_on = true;

byte energy_min_power_flag = 0;
byte energy_max_power_flag = 0;
byte energy_min_voltage_flag = 0;
byte energy_max_voltage_flag = 0;
byte energy_min_current_flag = 0;
byte energy_max_current_flag = 0;

byte energy_power_steady_cntr = 8;  // Allow for power on stabilization
byte energy_max_energy_state = 0;

#if FEATURE_POWER_LIMIT
byte energy_mplr_counter = 0;
uint16_t energy_mplh_counter = 0;
uint16_t energy_mplw_counter = 0;
#endif  // FEATURE_POWER_LIMIT

byte energy_fifth_second = 0;
Ticker ticker_energy;

int energy_command_code = 0;
/********************************************************************************************/

void EnergyUpdateToday()
{
  if (energy_kWhtoday_delta > 1000) {
    unsigned long delta = energy_kWhtoday_delta / 1000;
    energy_kWhtoday_delta -= (delta * 1000);
    energy_kWhtoday += delta;
  }
  RtcSettings.energy_kWhtoday = energy_kWhtoday;
  energy_daily = (float)energy_kWhtoday / 100000;
  energy_total = (float)(RtcSettings.energy_kWhtotal + energy_kWhtoday) / 100000;
}

/*********************************************************************************************/

void Energy200ms()
{
  energy_power_on = (power != 0) | Settings.flag.no_power_on_check;

  energy_fifth_second++;
  if (5 == energy_fifth_second) {
    energy_fifth_second = 0;

    XnrgCall(FUNC_EVERY_SECOND);

    if (RtcTime.valid) {
      if (LocalTime() == Midnight()) {
        Settings.energy_kWhyesterday = energy_kWhtoday;
        Settings.energy_kWhtotal += energy_kWhtoday;
        RtcSettings.energy_kWhtotal = Settings.energy_kWhtotal;
        energy_kWhtoday = 0;
        energy_kWhtoday_delta = 0;
        energy_period = energy_kWhtoday;
        EnergyUpdateToday();
        energy_max_energy_state = 3;
      }
      if ((RtcTime.hour == Settings.energy_max_energy_start) && (3 == energy_max_energy_state)) {
        energy_max_energy_state = 0;
      }
    }
  }

  XnrgCall(FUNC_EVERY_200_MSECOND);

  if (energy_calc_power_factor) {
    float power_factor = 0;
    if (energy_voltage && energy_current && energy_power) {
      power_factor = energy_power / (energy_voltage * energy_current);
      if (power_factor > 1) power_factor = 1;
    }
    energy_power_factor = power_factor;
  }
}

void EnergySaveState()
{
  Settings.energy_kWhdoy = (RtcTime.valid) ? RtcTime.day_of_year : 0;
  Settings.energy_kWhtoday = energy_kWhtoday;
  RtcSettings.energy_kWhtoday = energy_kWhtoday;
  Settings.energy_kWhtotal = RtcSettings.energy_kWhtotal;
}

boolean EnergyMargin(byte type, uint16_t margin, uint16_t value, byte &flag, byte &save_flag)
{
  byte change;

  if (!margin) return false;
  change = save_flag;
  if (type) {
    flag = (value > margin);
  } else {
    flag = (value < margin);
  }
  save_flag = flag;
  return (change != save_flag);
}

void EnergySetPowerSteadyCounter()
{
  energy_power_steady_cntr = 2;
}

void EnergyMarginCheck()
{
  uint16_t energy_daily_u = 0;
  uint16_t energy_power_u = 0;
  uint16_t energy_voltage_u = 0;
  uint16_t energy_current_u = 0;
  boolean flag;
  boolean jsonflg;

  if (energy_power_steady_cntr) {
    energy_power_steady_cntr--;
    return;
  }

  if (Settings.energy_power_delta) {
    float delta = abs(energy_power_last[0] - energy_power);
    // Any delta compared to minimal delta
    float min_power = (energy_power_last[0] > energy_power) ? energy_power : energy_power_last[0];
    if (((delta / min_power) * 100) > Settings.energy_power_delta) {
      energy_power_delta = 1;
      energy_power_last[1] = energy_power;  // We only want one report so reset history
      energy_power_last[2] = energy_power;
    }
  }
  energy_power_last[0] = energy_power_last[1];  // Shift in history every second allowing power changes to settle for up to three seconds
  energy_power_last[1] = energy_power_last[2];
  energy_power_last[2] = energy_power;

  if (energy_power_on && (Settings.energy_min_power || Settings.energy_max_power || Settings.energy_min_voltage || Settings.energy_max_voltage || Settings.energy_min_current || Settings.energy_max_current)) {
    energy_power_u = (uint16_t)(energy_power);
    energy_voltage_u = (uint16_t)(energy_voltage);
    energy_current_u = (uint16_t)(energy_current * 1000);

//    snprintf_P(log_data, sizeof(log_data), PSTR("NRG: W %d, U %d, I %d"), energy_power_u, energy_voltage_u, energy_current_u);
//    AddLog(LOG_LEVEL_DEBUG);

    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{"));
    jsonflg = 0;
    if (EnergyMargin(0, Settings.energy_min_power, energy_power_u, flag, energy_min_power_flag)) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"" D_CMND_POWERLOW "\":\"%s\""), mqtt_data, (jsonflg)?",":"", GetStateText(flag));
      jsonflg = 1;
    }
    if (EnergyMargin(1, Settings.energy_max_power, energy_power_u, flag, energy_max_power_flag)) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"" D_CMND_POWERHIGH "\":\"%s\""), mqtt_data, (jsonflg)?",":"", GetStateText(flag));
      jsonflg = 1;
    }
    if (EnergyMargin(0, Settings.energy_min_voltage, energy_voltage_u, flag, energy_min_voltage_flag)) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"" D_CMND_VOLTAGELOW "\":\"%s\""), mqtt_data, (jsonflg)?",":"", GetStateText(flag));
      jsonflg = 1;
    }
    if (EnergyMargin(1, Settings.energy_max_voltage, energy_voltage_u, flag, energy_max_voltage_flag)) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"" D_CMND_VOLTAGEHIGH "\":\"%s\""), mqtt_data, (jsonflg)?",":"", GetStateText(flag));
      jsonflg = 1;
    }
    if (EnergyMargin(0, Settings.energy_min_current, energy_current_u, flag, energy_min_current_flag)) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"" D_CMND_CURRENTLOW "\":\"%s\""), mqtt_data, (jsonflg)?",":"", GetStateText(flag));
      jsonflg = 1;
    }
    if (EnergyMargin(1, Settings.energy_max_current, energy_current_u, flag, energy_max_current_flag)) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"" D_CMND_CURRENTHIGH "\":\"%s\""), mqtt_data, (jsonflg)?",":"", GetStateText(flag));
      jsonflg = 1;
    }
    if (jsonflg) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
      MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_MARGINS), MQTT_TELE_RETAIN);
      EnergyMqttShow();
    }
  }

#if FEATURE_POWER_LIMIT
  // Max Power
  if (Settings.energy_max_power_limit) {
    if (energy_power > Settings.energy_max_power_limit) {
      if (!energy_mplh_counter) {
        energy_mplh_counter = Settings.energy_max_power_limit_hold;
      } else {
        energy_mplh_counter--;
        if (!energy_mplh_counter) {
          snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_MAXPOWERREACHED "\":\"%d%s\"}"), energy_power_u, (Settings.flag.value_units) ? " " D_UNIT_WATT : "");
          MqttPublishPrefixTopic_P(STAT, S_RSLT_WARNING);
          EnergyMqttShow();
          ExecuteCommandPower(1, POWER_OFF, SRC_MAXPOWER);
          if (!energy_mplr_counter) {
            energy_mplr_counter = Settings.param[P_MAX_POWER_RETRY] +1;
          }
          energy_mplw_counter = Settings.energy_max_power_limit_window;
        }
      }
    }
    else if (power && (energy_power_u <= Settings.energy_max_power_limit)) {
      energy_mplh_counter = 0;
      energy_mplr_counter = 0;
      energy_mplw_counter = 0;
    }
    if (!power) {
      if (energy_mplw_counter) {
        energy_mplw_counter--;
      } else {
        if (energy_mplr_counter) {
          energy_mplr_counter--;
          if (energy_mplr_counter) {
            snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_POWERMONITOR "\":\"%s\"}"), GetStateText(1));
            MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_JSON_POWERMONITOR));
            ExecuteCommandPower(1, POWER_ON, SRC_MAXPOWER);
          } else {
            snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_MAXPOWERREACHEDRETRY "\":\"%s\"}"), GetStateText(0));
            MqttPublishPrefixTopic_P(STAT, S_RSLT_WARNING);
            EnergyMqttShow();
          }
        }
      }
    }
  }

  // Max Energy
  if (Settings.energy_max_energy) {
    energy_daily_u = (uint16_t)(energy_daily * 1000);
    if (!energy_max_energy_state && (RtcTime.hour == Settings.energy_max_energy_start)) {
      energy_max_energy_state = 1;
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_ENERGYMONITOR "\":\"%s\"}"), GetStateText(1));
      MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_JSON_ENERGYMONITOR));
      ExecuteCommandPower(1, POWER_ON, SRC_MAXENERGY);
    }
    else if ((1 == energy_max_energy_state) && (energy_daily_u >= Settings.energy_max_energy)) {
      energy_max_energy_state = 2;
      dtostrfd(energy_daily, 3, mqtt_data);
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_MAXENERGYREACHED "\":\"%s%s\"}"), mqtt_data, (Settings.flag.value_units) ? " " D_UNIT_KILOWATTHOUR : "");
      MqttPublishPrefixTopic_P(STAT, S_RSLT_WARNING);
      EnergyMqttShow();
      ExecuteCommandPower(1, POWER_OFF, SRC_MAXENERGY);
    }
  }
#endif  // FEATURE_POWER_LIMIT

  if (energy_power_delta) EnergyMqttShow();
}

void EnergyMqttShow()
{
// {"Time":"2017-12-16T11:48:55","ENERGY":{"Total":0.212,"Yesterday":0.000,"Today":0.014,"Period":2.0,"Power":22.0,"Factor":1.00,"Voltage":213.6,"Current":0.100}}
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_JSON_TIME "\":\"%s\""), GetDateAndTime(DT_LOCAL).c_str());
  int tele_period_save = tele_period;
  tele_period = 2;
  EnergyShow(1);
  tele_period = tele_period_save;
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
  MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);
  energy_power_delta = 0;
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

boolean EnergyCommand()
{
  char command [CMDSZ];
  char sunit[CMDSZ];
  boolean serviced = true;
  uint8_t status_flag = 0;
  uint8_t unit = 0;
  unsigned long nvalue = 0;

  int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic, kEnergyCommands);
  energy_command_code = command_code;
  if (-1 == command_code) {
    serviced = false;  // Unknown command
  }
  else if (CMND_POWERDELTA == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 101)) {
      Settings.energy_power_delta = (1 == XdrvMailbox.payload) ? DEFAULT_POWER_DELTA : XdrvMailbox.payload;
    }
    nvalue = Settings.energy_power_delta;
    unit = UNIT_PERCENTAGE;
  }
  else if (CMND_POWERLOW == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
      Settings.energy_min_power = XdrvMailbox.payload;
    }
    nvalue = Settings.energy_min_power;
    unit = UNIT_WATT;
  }
  else if (CMND_POWERHIGH == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
      Settings.energy_max_power = XdrvMailbox.payload;
    }
    nvalue = Settings.energy_max_power;
    unit = UNIT_WATT;
  }
  else if (CMND_VOLTAGELOW == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 501)) {
      Settings.energy_min_voltage = XdrvMailbox.payload;
    }
    nvalue = Settings.energy_min_voltage;
    unit = UNIT_VOLT;
  }
  else if (CMND_VOLTAGEHIGH == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 501)) {
      Settings.energy_max_voltage = XdrvMailbox.payload;
    }
    nvalue = Settings.energy_max_voltage;
    unit = UNIT_VOLT;
  }
  else if (CMND_CURRENTLOW == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 16001)) {
      Settings.energy_min_current = XdrvMailbox.payload;
    }
    nvalue = Settings.energy_min_current;
    unit = UNIT_MILLIAMPERE;
  }
  else if (CMND_CURRENTHIGH == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 16001)) {
      Settings.energy_max_current = XdrvMailbox.payload;
    }
    nvalue = Settings.energy_max_current;
    unit = UNIT_MILLIAMPERE;
  }
  else if ((CMND_ENERGYRESET == command_code) && (XdrvMailbox.index > 0) && (XdrvMailbox.index <= 3)) {
    char *p;
    unsigned long lnum = strtoul(XdrvMailbox.data, &p, 10);
    if (p != XdrvMailbox.data) {
      switch (XdrvMailbox.index) {
      case 1:
        energy_kWhtoday = lnum *100;
        energy_kWhtoday_delta = 0;
        energy_period = energy_kWhtoday;
        Settings.energy_kWhtoday = energy_kWhtoday;
        RtcSettings.energy_kWhtoday = energy_kWhtoday;
        energy_daily = (float)energy_kWhtoday / 100000;
        break;
      case 2:
        Settings.energy_kWhyesterday = lnum *100;
        break;
      case 3:
        RtcSettings.energy_kWhtotal = lnum *100;
        Settings.energy_kWhtotal = RtcSettings.energy_kWhtotal;
        energy_total = (float)(RtcSettings.energy_kWhtotal + energy_kWhtoday) / 100000;
        break;
      }
    }
    char energy_yesterday_chr[10];
    char energy_daily_chr[10];
    char energy_total_chr[10];

    dtostrfd(energy_total, Settings.flag2.energy_resolution, energy_total_chr);
    dtostrfd(energy_daily, Settings.flag2.energy_resolution, energy_daily_chr);
    dtostrfd((float)Settings.energy_kWhyesterday / 100000, Settings.flag2.energy_resolution, energy_yesterday_chr);

    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"%s\":{\"" D_JSON_TOTAL "\":%s,\"" D_JSON_YESTERDAY "\":%s,\"" D_JSON_TODAY "\":%s}}"),
      command, energy_total_chr, energy_yesterday_chr, energy_daily_chr);
    status_flag = 1;
  }
  else if ((CMND_POWERSET == command_code) && XnrgCall(FUNC_COMMAND)) {  // Watt
    nvalue = Settings.energy_power_calibration;
    unit = UNIT_MICROSECOND;
  }
  else if ((CMND_VOLTAGESET == command_code) && XnrgCall(FUNC_COMMAND)) {  // Volt
    nvalue = Settings.energy_voltage_calibration;
    unit = UNIT_MICROSECOND;
  }
  else if ((CMND_CURRENTSET == command_code) && XnrgCall(FUNC_COMMAND)) {  // milliAmpere
    nvalue = Settings.energy_current_calibration;
    unit = UNIT_MICROSECOND;
  }
  else if ((CMND_FREQUENCYSET == command_code) && XnrgCall(FUNC_COMMAND)) {  // Hz
    nvalue = Settings.energy_frequency_calibration;
    unit = UNIT_MICROSECOND;
  }

#if FEATURE_POWER_LIMIT
  else if (CMND_MAXPOWER == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
      Settings.energy_max_power_limit = XdrvMailbox.payload;
    }
    nvalue = Settings.energy_max_power_limit;
    unit = UNIT_WATT;
  }
  else if (CMND_MAXPOWERHOLD == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
      Settings.energy_max_power_limit_hold = (1 == XdrvMailbox.payload) ? MAX_POWER_HOLD : XdrvMailbox.payload;
    }
    nvalue = Settings.energy_max_power_limit_hold;
    unit = UNIT_SECOND;
  }
  else if (CMND_MAXPOWERWINDOW == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
      Settings.energy_max_power_limit_window = (1 == XdrvMailbox.payload) ? MAX_POWER_WINDOW : XdrvMailbox.payload;
    }
    nvalue = Settings.energy_max_power_limit_window;
    unit = UNIT_SECOND;
  }
  else if (CMND_SAFEPOWER == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
      Settings.energy_max_power_safe_limit = XdrvMailbox.payload;
    }
    nvalue = Settings.energy_max_power_safe_limit;
    unit = UNIT_WATT;
  }
  else if (CMND_SAFEPOWERHOLD == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
      Settings.energy_max_power_safe_limit_hold = (1 == XdrvMailbox.payload) ? SAFE_POWER_HOLD : XdrvMailbox.payload;
    }
    nvalue = Settings.energy_max_power_safe_limit_hold;
    unit = UNIT_SECOND;
  }
  else if (CMND_SAFEPOWERWINDOW == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 1440)) {
      Settings.energy_max_power_safe_limit_window = (1 == XdrvMailbox.payload) ? SAFE_POWER_WINDOW : XdrvMailbox.payload;
    }
    nvalue = Settings.energy_max_power_safe_limit_window;
    unit = UNIT_MINUTE;
  }
  else if (CMND_MAXENERGY == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
      Settings.energy_max_energy = XdrvMailbox.payload;
      energy_max_energy_state = 3;
    }
    nvalue = Settings.energy_max_energy;
    unit = UNIT_WATTHOUR;
  }
  else if (CMND_MAXENERGYSTART == command_code) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 24)) {
      Settings.energy_max_energy_start = XdrvMailbox.payload;
    }
    nvalue = Settings.energy_max_energy_start;
    unit = UNIT_HOUR;
  }
#endif  // FEATURE_POWER_LIMIT
  else serviced = false;  // Unknown command

  if (serviced && !status_flag) {

    if (UNIT_MICROSECOND == unit) {
      snprintf_P(command, sizeof(command), PSTR("%sCal"), command);
    }

    if (Settings.flag.value_units) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_LVALUE_SPACE_UNIT, command, nvalue, GetTextIndexed(sunit, sizeof(sunit), unit, kUnitNames));
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_LVALUE, command, nvalue);
    }
  }

  return serviced;
}

void EnergyDrvInit()
{
  energy_flg = ENERGY_NONE;
  XnrgCall(FUNC_PRE_INIT);
}

void EnergySnsInit()
{
  XnrgCall(FUNC_INIT);

  if (energy_flg) {
    energy_kWhtoday = (RtcSettingsValid()) ? RtcSettings.energy_kWhtoday : (RtcTime.day_of_year == Settings.energy_kWhdoy) ? Settings.energy_kWhtoday : 0;
    energy_kWhtoday_delta = 0;
    energy_period = energy_kWhtoday;
    EnergyUpdateToday();
    ticker_energy.attach_ms(200, Energy200ms);
  }
}

#ifdef USE_WEBSERVER
const char HTTP_ENERGY_SNS1[] PROGMEM = "%s"
  "{s}" D_VOLTAGE "{m}%s " D_UNIT_VOLT "{e}"
  "{s}" D_CURRENT "{m}%s " D_UNIT_AMPERE "{e}"
  "{s}" D_POWERUSAGE "{m}%s " D_UNIT_WATT "{e}";

const char HTTP_ENERGY_SNS2[] PROGMEM = "%s"
  "{s}" D_POWER_FACTOR "{m}%s{e}";

const char HTTP_ENERGY_SNS3[] PROGMEM = "%s"
  "{s}" D_FREQUENCY "{m}%s " D_UNIT_HERTZ "{e}";

const char HTTP_ENERGY_SNS4[] PROGMEM = "%s"
  "{s}" D_ENERGY_TODAY  "{m}%s " D_UNIT_KILOWATTHOUR "{e}"
  "{s}" D_ENERGY_YESTERDAY "{m}%s " D_UNIT_KILOWATTHOUR "{e}"
  "{s}" D_ENERGY_TOTAL "{m}%s " D_UNIT_KILOWATTHOUR "{e}";      // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

void EnergyShow(boolean json)
{
  char energy_total_chr[10];
  char energy_daily_chr[10];
  char energy_period_chr[10];
  char energy_power_chr[10];
  char energy_voltage_chr[10];
  char energy_current_chr[10];
  char energy_frequency_chr[10];
  char energy_power_factor_chr[10];
  char energy_yesterday_chr[10];
  char speriod[20];
  char spfactor[20];
  char sfrequency[20];

  bool show_energy_period = (0 == tele_period);

  dtostrfd(energy_power, Settings.flag2.wattage_resolution, energy_power_chr);
  dtostrfd(energy_voltage, Settings.flag2.voltage_resolution, energy_voltage_chr);
  dtostrfd(energy_current, Settings.flag2.current_resolution, energy_current_chr);
  dtostrfd(energy_total, Settings.flag2.energy_resolution, energy_total_chr);
  dtostrfd(energy_daily, Settings.flag2.energy_resolution, energy_daily_chr);
  dtostrfd((float)Settings.energy_kWhyesterday / 100000, Settings.flag2.energy_resolution, energy_yesterday_chr);

  float energy = 0;
  if (show_energy_period) {
    if (energy_period) energy = (float)(energy_kWhtoday - energy_period) / 100;
    energy_period = energy_kWhtoday;
    dtostrfd(energy, Settings.flag2.wattage_resolution, energy_period_chr);
    snprintf_P(speriod, sizeof(speriod), PSTR(",\"" D_JSON_PERIOD "\":%s"), energy_period_chr);
  }
  if (!isnan(energy_frequency)) {
    dtostrfd(energy_frequency, Settings.flag2.frequency_resolution, energy_frequency_chr);
    snprintf_P(sfrequency, sizeof(sfrequency), PSTR(",\"" D_JSON_FREQUENCY "\":%s"), energy_frequency_chr);
  }
  if (!isnan(energy_power_factor)) {
    dtostrfd(energy_power_factor, 2, energy_power_factor_chr);
    snprintf_P(spfactor, sizeof(spfactor), PSTR(",\"" D_JSON_POWERFACTOR "\":%s"), energy_power_factor_chr);
  }

  if (json) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"" D_RSLT_ENERGY "\":{\"" D_JSON_TOTAL "\":%s,\"" D_JSON_YESTERDAY "\":%s,\"" D_JSON_TODAY "\":%s%s,\""
      D_JSON_POWERUSAGE "\":%s%s,\"" D_JSON_VOLTAGE "\":%s,\"" D_JSON_CURRENT "\":%s%s}"),
      mqtt_data, energy_total_chr, energy_yesterday_chr, energy_daily_chr, (show_energy_period) ? speriod : "",
      energy_power_chr, (!isnan(energy_power_factor)) ? spfactor : "", energy_voltage_chr, energy_current_chr, (!isnan(energy_frequency)) ? sfrequency : "");
#ifdef USE_DOMOTICZ
    if (show_energy_period) {  // Only send if telemetry
      dtostrfd(energy_total * 1000, 1, energy_total_chr);
      DomoticzSensorPowerEnergy((int)energy_power, energy_total_chr);  // PowerUsage, EnergyToday
      DomoticzSensor(DZ_VOLTAGE, energy_voltage_chr);  // Voltage
      DomoticzSensor(DZ_CURRENT, energy_current_chr);  // Current
    }
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
    if (show_energy_period) {
      KnxSensor(KNX_ENERGY_VOLTAGE, energy_voltage);
      KnxSensor(KNX_ENERGY_CURRENT, energy_current);
      KnxSensor(KNX_ENERGY_POWER, energy_power);
      if (!isnan(energy_power_factor)) { KnxSensor(KNX_ENERGY_POWERFACTOR, energy_power_factor); }
      KnxSensor(KNX_ENERGY_DAILY, energy_daily);
      KnxSensor(KNX_ENERGY_TOTAL, energy_total);
      KnxSensor(KNX_ENERGY_START, energy_start);
    }
#endif  // USE_KNX
#ifdef USE_WEBSERVER
  } else {
    snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_ENERGY_SNS1, mqtt_data, energy_voltage_chr, energy_current_chr, energy_power_chr);
    if (!isnan(energy_power_factor)) { snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_ENERGY_SNS2, mqtt_data, energy_power_factor_chr); }
    if (!isnan(energy_frequency)) { snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_ENERGY_SNS3, mqtt_data, energy_frequency_chr); }
    snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_ENERGY_SNS4, mqtt_data, energy_daily_chr, energy_yesterday_chr, energy_total_chr);
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_03

boolean Xdrv03(byte function)
{
  boolean result = false;

  if (FUNC_PRE_INIT == function) {
    EnergyDrvInit();
  }
  else if (energy_flg) {
    switch (function) {
      case FUNC_COMMAND:
        result = EnergyCommand();
        break;
      case FUNC_SET_POWER:
        EnergySetPowerSteadyCounter();
        break;
      case FUNC_SERIAL:
        result = XnrgCall(FUNC_SERIAL);
        break;
    }
  }
  return result;
}

#define XSNS_03

boolean Xsns03(byte function)
{
  boolean result = false;

  if (energy_flg) {
    switch (function) {
      case FUNC_INIT:
        EnergySnsInit();
        break;
      case FUNC_EVERY_SECOND:
        EnergyMarginCheck();
        break;
      case FUNC_JSON_APPEND:
        EnergyShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        EnergyShow(0);
        break;
#endif  // USE_WEBSERVER
      case FUNC_SAVE_BEFORE_RESTART:
        EnergySaveState();
        break;
    }
  }
  return result;
}

#endif  // USE_ENERGY_SENSOR
