/*
  xdrv_03_energy.ino - Energy sensor support for Sonoff-Tasmota

  Copyright (C) 2019  Theo Arends

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

#define XDRV_03                3
#define XSNS_03                3

//#define USE_ENERGY_MARGIN_DETECTION
//  #define USE_ENERGY_POWER_LIMIT

#define ENERGY_NONE            0
#define ENERGY_WATCHDOG        4        // Allow up to 4 seconds before deciding no valid data present

#include <Ticker.h>

#define D_CMND_POWERCAL "PowerCal"
#define D_CMND_VOLTAGECAL "VoltageCal"
#define D_CMND_CURRENTCAL "CurrentCal"
#define D_CMND_TARIFF "Tariff"

enum EnergyCommands {
  CMND_POWERCAL, CMND_VOLTAGECAL, CMND_CURRENTCAL,
  CMND_POWERSET, CMND_VOLTAGESET, CMND_CURRENTSET, CMND_FREQUENCYSET };

const char kEnergyCommands[] PROGMEM = "|"  // No prefix
  D_CMND_POWERCAL "|" D_CMND_VOLTAGECAL "|" D_CMND_CURRENTCAL "|"
  D_CMND_POWERSET "|" D_CMND_VOLTAGESET "|" D_CMND_CURRENTSET "|" D_CMND_FREQUENCYSET "|"
#ifdef USE_ENERGY_MARGIN_DETECTION
  D_CMND_POWERDELTA "|" D_CMND_POWERLOW "|" D_CMND_POWERHIGH "|" D_CMND_VOLTAGELOW "|" D_CMND_VOLTAGEHIGH "|" D_CMND_CURRENTLOW "|" D_CMND_CURRENTHIGH "|"
#ifdef USE_ENERGY_POWER_LIMIT
  D_CMND_MAXENERGY "|" D_CMND_MAXENERGYSTART "|"
  D_CMND_MAXPOWER "|" D_CMND_MAXPOWERHOLD "|" D_CMND_MAXPOWERWINDOW "|"
  D_CMND_SAFEPOWER "|" D_CMND_SAFEPOWERHOLD "|"  D_CMND_SAFEPOWERWINDOW "|"
#endif  // USE_ENERGY_POWER_LIMIT
#endif  // USE_ENERGY_MARGIN_DETECTION
  D_CMND_ENERGYRESET "|" D_CMND_TARIFF ;

void (* const EnergyCommand[])(void) PROGMEM = {
  &CmndPowerCal, &CmndVoltageCal, &CmndCurrentCal,
  &CmndPowerSet, &CmndVoltageSet, &CmndCurrentSet, &CmndFrequencySet,
#ifdef USE_ENERGY_MARGIN_DETECTION
  &CmndPowerDelta, &CmndPowerLow, &CmndPowerHigh, &CmndVoltageLow, &CmndVoltageHigh, &CmndCurrentLow, &CmndCurrentHigh,
#ifdef USE_ENERGY_POWER_LIMIT
  &CmndMaxEnergy, &CmndMaxEnergyStart,
  &CmndMaxPower, &CmndMaxPowerHold, &CmndMaxPowerWindow,
  &CmndSafePower, &CmndSafePowerHold, &CmndSafePowerWindow,
#endif  // USE_ENERGY_POWER_LIMIT
#endif  // USE_ENERGY_MARGIN_DETECTION
  &CmndEnergyReset, &CmndTariff };

struct ENERGY {
  float voltage = 0;           // 123.1 V
  float current = 0;           // 123.123 A
  float active_power = 0;      // 123.1 W
  float apparent_power = NAN;  // 123.1 VA
  float reactive_power = NAN;  // 123.1 VAr
  float power_factor = NAN;    // 0.12
  float frequency = NAN;       // 123.1 Hz
  float start_energy = 0;      // 12345.12345 kWh total previous

  float daily = 0;             // 123.123 kWh
  float total = 0;             // 12345.12345 kWh tariff 1 + 2
  float total1 = 0;            // 12345.12345 kWh tariff 1 - off-peak

  unsigned long kWhtoday_delta = 0;  // 1212312345 Wh 10^-5 (deca micro Watt hours) - Overflows to Energy.kWhtoday (HLW and CSE only)
  unsigned long kWhtoday;      // 12312312 Wh * 10^-2 (deca milli Watt hours) - 5764 = 0.05764 kWh = 0.058 kWh = Energy.daily
  unsigned long kWhtoday1;     // 12312312 Wh * 10^-2 (deca milli Watt hours) - 5764 = 0.05764 kWh = 0.058 kWh = Energy.daily
  unsigned long period = 0;    // 12312312 Wh * 10^-2 (deca milli Watt hours) - 5764 = 0.05764 kWh = 0.058 kWh = Energy.daily

  uint8_t fifth_second = 0;
  uint8_t command_code = 0;
  uint8_t data_valid = 0;

  bool voltage_available = true;  // Enable if voltage is measured
  bool current_available = true;  // Enable if current is measured

  bool type_dc = false;
  bool power_on = true;

#ifdef USE_ENERGY_MARGIN_DETECTION
  float power_history[3] = { 0 };
  uint8_t power_steady_counter = 8;  // Allow for power on stabilization
  uint8_t power_delta = 0;
  bool min_power_flag = false;
  bool max_power_flag = false;
  bool min_voltage_flag = false;
  bool max_voltage_flag = false;
  bool min_current_flag = false;
  bool max_current_flag = false;

#ifdef USE_ENERGY_POWER_LIMIT
  uint16_t mplh_counter = 0;
  uint16_t mplw_counter = 0;
  uint8_t mplr_counter = 0;
  uint8_t max_energy_state  = 0;
#endif  // USE_ENERGY_POWER_LIMIT

#endif  // USE_ENERGY_MARGIN_DETECTION
} Energy;

Ticker ticker_energy;

/********************************************************************************************/

void EnergyUpdateToday(void)
{
  if (Energy.kWhtoday_delta > 1000) {
    unsigned long delta = Energy.kWhtoday_delta / 1000;
    Energy.kWhtoday_delta -= (delta * 1000);
    Energy.kWhtoday += delta;
  }
  uint32_t energy_diff = Energy.kWhtoday - RtcSettings.energy_kWhtoday;

  RtcSettings.energy_kWhtoday = Energy.kWhtoday;
  Energy.daily = (float)Energy.kWhtoday / 100000;
  Energy.total = (float)(RtcSettings.energy_kWhtotal + Energy.kWhtoday) / 100000;

  if ((RtcTime.hour < Settings.param[P_ENERGY_TARIFF2]) ||  // Tarrif1 = Off-Peak
      (RtcTime.hour >= Settings.param[P_ENERGY_TARIFF1]) ||
      (Settings.flag3.energy_weekend && ((RtcTime.day_of_week == 1) ||
                                         (RtcTime.day_of_week == 7)))
     ) {
    Energy.kWhtoday1 += energy_diff;
    RtcSettings.energy_usage.usage1_kWhtoday = Energy.kWhtoday1;
    Energy.total1 = (float)(RtcSettings.energy_usage.usage1_kWhtotal + Energy.kWhtoday1) / 100000;
  }
}

/*********************************************************************************************/

void Energy200ms(void)
{
  Energy.power_on = (power != 0) | Settings.flag.no_power_on_check;

  Energy.fifth_second++;
  if (5 == Energy.fifth_second) {
    Energy.fifth_second = 0;

    XnrgCall(FUNC_ENERGY_EVERY_SECOND);

    if (RtcTime.valid) {
      if (LocalTime() == Midnight()) {
        Settings.energy_kWhyesterday = Energy.kWhtoday;

        Settings.energy_kWhtotal += Energy.kWhtoday;
        RtcSettings.energy_kWhtotal = Settings.energy_kWhtotal;
        Energy.kWhtoday = 0;
        RtcSettings.energy_kWhtoday = 0;

        Settings.energy_usage.usage1_kWhtotal += Energy.kWhtoday1;
        RtcSettings.energy_usage.usage1_kWhtotal = Settings.energy_usage.usage1_kWhtotal;
        Energy.kWhtoday1 = 0;
        RtcSettings.energy_usage.usage1_kWhtoday = 0;

        Energy.kWhtoday_delta = 0;
        Energy.period = Energy.kWhtoday;
        EnergyUpdateToday();
#if defined(USE_ENERGY_MARGIN_DETECTION) && defined(USE_ENERGY_POWER_LIMIT)
        Energy.max_energy_state  = 3;
#endif  // USE_ENERGY_POWER_LIMIT
      }
#if defined(USE_ENERGY_MARGIN_DETECTION) && defined(USE_ENERGY_POWER_LIMIT)
      if ((RtcTime.hour == Settings.energy_max_energy_start) && (3 == Energy.max_energy_state )) {
        Energy.max_energy_state  = 0;
      }
#endif  // USE_ENERGY_POWER_LIMIT

    }
  }

  XnrgCall(FUNC_EVERY_200_MSECOND);
}

void EnergySaveState(void)
{
  Settings.energy_kWhdoy = (RtcTime.valid) ? RtcTime.day_of_year : 0;

  Settings.energy_kWhtoday = Energy.kWhtoday;
  RtcSettings.energy_kWhtoday = Energy.kWhtoday;
  Settings.energy_kWhtotal = RtcSettings.energy_kWhtotal;

  Settings.energy_usage.usage1_kWhtoday = Energy.kWhtoday1;
  RtcSettings.energy_usage.usage1_kWhtoday = Energy.kWhtoday1;
  Settings.energy_usage.usage1_kWhtotal = RtcSettings.energy_usage.usage1_kWhtotal;
}

#ifdef USE_ENERGY_MARGIN_DETECTION
bool EnergyMargin(bool type, uint16_t margin, uint16_t value, bool &flag, bool &save_flag)
{
  bool change;

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

void EnergyMarginCheck(void)
{
  uint16_t energy_daily_u = 0;
  uint16_t energy_power_u = 0;
  uint16_t energy_voltage_u = 0;
  uint16_t energy_current_u = 0;
  bool flag;
  bool jsonflg;

  if (Energy.power_steady_counter) {
    Energy.power_steady_counter--;
    return;
  }

  if (Settings.energy_power_delta) {
    float delta = abs(Energy.power_history[0] - Energy.active_power);
    // Any delta compared to minimal delta
    float min_power = (Energy.power_history[0] > Energy.active_power) ? Energy.active_power : Energy.power_history[0];
    if (((delta / min_power) * 100) > Settings.energy_power_delta) {
      Energy.power_delta = 1;
      Energy.power_history[1] = Energy.active_power;  // We only want one report so reset history
      Energy.power_history[2] = Energy.active_power;
    }
  }
  Energy.power_history[0] = Energy.power_history[1];  // Shift in history every second allowing power changes to settle for up to three seconds
  Energy.power_history[1] = Energy.power_history[2];
  Energy.power_history[2] = Energy.active_power;

  if (Energy.power_on && (Settings.energy_min_power || Settings.energy_max_power || Settings.energy_min_voltage || Settings.energy_max_voltage || Settings.energy_min_current || Settings.energy_max_current)) {
    energy_power_u = (uint16_t)(Energy.active_power);
    energy_voltage_u = (uint16_t)(Energy.voltage);
    energy_current_u = (uint16_t)(Energy.current * 1000);

    DEBUG_DRIVER_LOG(PSTR("NRG: W %d, U %d, I %d"), energy_power_u, energy_voltage_u, energy_current_u);

    Response_P(PSTR("{"));
    jsonflg = false;
    if (EnergyMargin(false, Settings.energy_min_power, energy_power_u, flag, Energy.min_power_flag)) {
      ResponseAppend_P(PSTR("%s\"" D_CMND_POWERLOW "\":\"%s\""), (jsonflg)?",":"", GetStateText(flag));
      jsonflg = true;
    }
    if (EnergyMargin(true, Settings.energy_max_power, energy_power_u, flag, Energy.max_power_flag)) {
      ResponseAppend_P(PSTR("%s\"" D_CMND_POWERHIGH "\":\"%s\""), (jsonflg)?",":"", GetStateText(flag));
      jsonflg = true;
    }
    if (EnergyMargin(false, Settings.energy_min_voltage, energy_voltage_u, flag, Energy.min_voltage_flag)) {
      ResponseAppend_P(PSTR("%s\"" D_CMND_VOLTAGELOW "\":\"%s\""), (jsonflg)?",":"", GetStateText(flag));
      jsonflg = true;
    }
    if (EnergyMargin(true, Settings.energy_max_voltage, energy_voltage_u, flag, Energy.max_voltage_flag)) {
      ResponseAppend_P(PSTR("%s\"" D_CMND_VOLTAGEHIGH "\":\"%s\""), (jsonflg)?",":"", GetStateText(flag));
      jsonflg = true;
    }
    if (EnergyMargin(false, Settings.energy_min_current, energy_current_u, flag, Energy.min_current_flag)) {
      ResponseAppend_P(PSTR("%s%s\"" D_CMND_CURRENTLOW "\":\"%s\""), (jsonflg)?",":"", GetStateText(flag));
      jsonflg = true;
    }
    if (EnergyMargin(true, Settings.energy_max_current, energy_current_u, flag, Energy.max_current_flag)) {
      ResponseAppend_P(PSTR("%s%s\"" D_CMND_CURRENTHIGH "\":\"%s\""), (jsonflg)?",":"", GetStateText(flag));
      jsonflg = true;
    }
    if (jsonflg) {
      ResponseJsonEnd();
      MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_MARGINS), MQTT_TELE_RETAIN);
      EnergyMqttShow();
    }
  }

#ifdef USE_ENERGY_POWER_LIMIT
  // Max Power
  if (Settings.energy_max_power_limit) {
    if (Energy.active_power > Settings.energy_max_power_limit) {
      if (!Energy.mplh_counter) {
        Energy.mplh_counter = Settings.energy_max_power_limit_hold;
      } else {
        Energy.mplh_counter--;
        if (!Energy.mplh_counter) {
          Response_P(PSTR("{\"" D_JSON_MAXPOWERREACHED "\":\"%d%s\"}"), energy_power_u, (Settings.flag.value_units) ? " " D_UNIT_WATT : "");
          MqttPublishPrefixTopic_P(STAT, S_RSLT_WARNING);
          EnergyMqttShow();
          ExecuteCommandPower(1, POWER_OFF, SRC_MAXPOWER);
          if (!Energy.mplr_counter) {
            Energy.mplr_counter = Settings.param[P_MAX_POWER_RETRY] +1;
          }
          Energy.mplw_counter = Settings.energy_max_power_limit_window;
        }
      }
    }
    else if (power && (energy_power_u <= Settings.energy_max_power_limit)) {
      Energy.mplh_counter = 0;
      Energy.mplr_counter = 0;
      Energy.mplw_counter = 0;
    }
    if (!power) {
      if (Energy.mplw_counter) {
        Energy.mplw_counter--;
      } else {
        if (Energy.mplr_counter) {
          Energy.mplr_counter--;
          if (Energy.mplr_counter) {
            Response_P(PSTR("{\"" D_JSON_POWERMONITOR "\":\"%s\"}"), GetStateText(1));
            MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_JSON_POWERMONITOR));
            ExecuteCommandPower(1, POWER_ON, SRC_MAXPOWER);
          } else {
            Response_P(PSTR("{\"" D_JSON_MAXPOWERREACHEDRETRY "\":\"%s\"}"), GetStateText(0));
            MqttPublishPrefixTopic_P(STAT, S_RSLT_WARNING);
            EnergyMqttShow();
          }
        }
      }
    }
  }

  // Max Energy
  if (Settings.energy_max_energy) {
    energy_daily_u = (uint16_t)(Energy.daily * 1000);
    if (!Energy.max_energy_state  && (RtcTime.hour == Settings.energy_max_energy_start)) {
      Energy.max_energy_state  = 1;
      Response_P(PSTR("{\"" D_JSON_ENERGYMONITOR "\":\"%s\"}"), GetStateText(1));
      MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_JSON_ENERGYMONITOR));
      ExecuteCommandPower(1, POWER_ON, SRC_MAXENERGY);
    }
    else if ((1 == Energy.max_energy_state ) && (energy_daily_u >= Settings.energy_max_energy)) {
      Energy.max_energy_state  = 2;
      dtostrfd(Energy.daily, 3, mqtt_data);
      Response_P(PSTR("{\"" D_JSON_MAXENERGYREACHED "\":\"%s%s\"}"), mqtt_data, (Settings.flag.value_units) ? " " D_UNIT_KILOWATTHOUR : "");
      MqttPublishPrefixTopic_P(STAT, S_RSLT_WARNING);
      EnergyMqttShow();
      ExecuteCommandPower(1, POWER_OFF, SRC_MAXENERGY);
    }
  }
#endif  // USE_ENERGY_POWER_LIMIT

  if (Energy.power_delta) { EnergyMqttShow(); }
}

void EnergyMqttShow(void)
{
// {"Time":"2017-12-16T11:48:55","ENERGY":{"Total":0.212,"Yesterday":0.000,"Today":0.014,"Period":2.0,"Power":22.0,"Factor":1.00,"Voltage":213.6,"Current":0.100}}
  ResponseBeginTime();
  int tele_period_save = tele_period;
  tele_period = 2;
  EnergyShow(true);
  tele_period = tele_period_save;
  ResponseJsonEnd();
  MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag.mqtt_sensor_retain);
  Energy.power_delta = 0;
}
#endif  // USE_ENERGY_MARGIN_DETECTION

void EnergyOverTempCheck()
{
  if (global_update) {
    if (power && (global_temperature != 9999) && (global_temperature > Settings.param[P_OVER_TEMP])) {  // Device overtemp, turn off relays
      SetAllPower(POWER_ALL_OFF, SRC_OVERTEMP);
    }
  }
  if (Energy.data_valid <= ENERGY_WATCHDOG) {
    Energy.data_valid++;
    if (Energy.data_valid > ENERGY_WATCHDOG) {
      // Reset energy registers
      Energy.voltage = 0;
      Energy.current = 0;
      Energy.active_power = 0;
      if (!isnan(Energy.frequency)) { Energy.frequency = 0; }
      if (!isnan(Energy.power_factor)) { Energy.power_factor = 0; }
      Energy.start_energy = 0;
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void EnergyCommandResponse(uint32_t nvalue, uint32_t unit)
{
  if (UNIT_MILLISECOND == unit) {
    snprintf_P(XdrvMailbox.command, CMDSZ, PSTR("%sCal"), XdrvMailbox.command);
    unit = UNIT_MICROSECOND;
  }

  if (Settings.flag.value_units) {
    char sunit[CMDSZ];
    Response_P(S_JSON_COMMAND_LVALUE_SPACE_UNIT, XdrvMailbox.command, nvalue, GetTextIndexed(sunit, sizeof(sunit), unit, kUnitNames));
  } else {
    Response_P(S_JSON_COMMAND_LVALUE, XdrvMailbox.command, nvalue);
  }
}

void CmndEnergyReset(void)
{
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 3)) {
    char *p;
    unsigned long lnum = strtoul(XdrvMailbox.data, &p, 10);
    if (p != XdrvMailbox.data) {
      switch (XdrvMailbox.index) {
      case 1:
        Energy.kWhtoday = lnum *100;
        Energy.kWhtoday_delta = 0;
        Energy.period = Energy.kWhtoday;
        Settings.energy_kWhtoday = Energy.kWhtoday;
        RtcSettings.energy_kWhtoday = Energy.kWhtoday;
        Energy.daily = (float)Energy.kWhtoday / 100000;
        if (!RtcSettings.energy_kWhtotal && !Energy.kWhtoday) { Settings.energy_kWhtotal_time = LocalTime(); }
        break;
      case 2:
        Settings.energy_kWhyesterday = lnum *100;
        break;
      case 3:
        RtcSettings.energy_kWhtotal = lnum *100;
        Settings.energy_kWhtotal = RtcSettings.energy_kWhtotal;
        Energy.total = (float)(RtcSettings.energy_kWhtotal + Energy.kWhtoday) / 100000;
        Settings.energy_kWhtotal_time = (!Energy.kWhtoday) ? LocalTime() : Midnight();
        break;
      }
    }

    if (Energy.kWhtoday1 > Energy.kWhtoday) {
      Energy.kWhtoday1 = Energy.kWhtoday;
    }
    if (Settings.energy_usage.usage1_kWhtoday > Settings.energy_kWhtoday) {
      Settings.energy_usage.usage1_kWhtoday = Settings.energy_kWhtoday;
      RtcSettings.energy_usage.usage1_kWhtoday = Settings.energy_kWhtoday;
    }
    if (Settings.energy_usage.usage1_kWhtotal > Settings.energy_kWhtotal) {
      Settings.energy_usage.usage1_kWhtotal = Settings.energy_kWhtotal;
      RtcSettings.energy_usage.usage1_kWhtotal = Settings.energy_kWhtotal;
    }

    char energy_total_chr[33];
    dtostrfd(Energy.total, Settings.flag2.energy_resolution, energy_total_chr);
    char energy_daily_chr[33];
    dtostrfd(Energy.daily, Settings.flag2.energy_resolution, energy_daily_chr);
    char energy_yesterday_chr[33];
    dtostrfd((float)Settings.energy_kWhyesterday / 100000, Settings.flag2.energy_resolution, energy_yesterday_chr);

    Response_P(PSTR("{\"%s\":{\"" D_JSON_TOTAL "\":%s,\"" D_JSON_YESTERDAY "\":%s,\"" D_JSON_TODAY "\":%s}}"),
      XdrvMailbox.command, energy_total_chr, energy_yesterday_chr, energy_daily_chr);
  }
}

void CmndTariff(void)
{
  // Tariff1 23
  // Tariff2 7
  // Tariff3 0/1
  if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 2)) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 24)) {
      Settings.param[P_ENERGY_TARIFF1 + XdrvMailbox.index -1] = XdrvMailbox.payload;
    }
  }
  else if (XdrvMailbox.index == 3) {
    Settings.flag3.energy_weekend = XdrvMailbox.payload & 1;
  }
  Response_P(PSTR("{\"%s\":{\"Off-Peak\":%d,\"Standard\":%d,\"Weekend\":\"%s\"}}"),
    XdrvMailbox.command, Settings.param[P_ENERGY_TARIFF1], Settings.param[P_ENERGY_TARIFF2], GetStateText(Settings.flag3.energy_weekend));
}

void CmndPowerCal(void)
{
  Energy.command_code = CMND_POWERCAL;
  if (XnrgCall(FUNC_COMMAND)) {  // microseconds
    if ((XdrvMailbox.payload > 999) && (XdrvMailbox.payload < 32001)) {
      Settings.energy_power_calibration = XdrvMailbox.payload;
    }
    EnergyCommandResponse(Settings.energy_power_calibration, UNIT_MICROSECOND);
  }
}

void CmndVoltageCal(void)
{
  Energy.command_code = CMND_VOLTAGECAL;
  if (XnrgCall(FUNC_COMMAND)) {  // microseconds
    if ((XdrvMailbox.payload > 999) && (XdrvMailbox.payload < 32001)) {
      Settings.energy_voltage_calibration = XdrvMailbox.payload;
    }
    EnergyCommandResponse(Settings.energy_voltage_calibration, UNIT_MICROSECOND);
  }
}

void CmndCurrentCal(void)
{
  Energy.command_code = CMND_CURRENTCAL;
  if (XnrgCall(FUNC_COMMAND)) {  // microseconds
    if ((XdrvMailbox.payload > 999) && (XdrvMailbox.payload < 32001)) {
      Settings.energy_current_calibration = XdrvMailbox.payload;
    }
    EnergyCommandResponse(Settings.energy_current_calibration, UNIT_MICROSECOND);
  }
}

void CmndPowerSet(void)
{
  Energy.command_code = CMND_POWERSET;
  if (XnrgCall(FUNC_COMMAND)) {  // Watt
    EnergyCommandResponse(Settings.energy_power_calibration, UNIT_MILLISECOND);
  }
}

void CmndVoltageSet(void)
{
  Energy.command_code = CMND_VOLTAGESET;
  if (XnrgCall(FUNC_COMMAND)) {  // Volt
    EnergyCommandResponse(Settings.energy_voltage_calibration, UNIT_MILLISECOND);
  }
}

void CmndCurrentSet(void)
{
  Energy.command_code = CMND_CURRENTSET;
  if (XnrgCall(FUNC_COMMAND)) {  // milliAmpere
    EnergyCommandResponse(Settings.energy_current_calibration, UNIT_MILLISECOND);
  }
}

void CmndFrequencySet(void)
{
  Energy.command_code = CMND_FREQUENCYSET;
  if (XnrgCall(FUNC_COMMAND)) {  // Hz
    EnergyCommandResponse(Settings.energy_frequency_calibration, UNIT_MILLISECOND);
  }
}

#ifdef USE_ENERGY_MARGIN_DETECTION
void CmndPowerDelta(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 101)) {
    Settings.energy_power_delta = XdrvMailbox.payload;
  }
  EnergyCommandResponse(Settings.energy_power_delta, UNIT_PERCENTAGE);
}

void CmndPowerLow(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
    Settings.energy_min_power = XdrvMailbox.payload;
  }
  EnergyCommandResponse(Settings.energy_min_power, UNIT_WATT);
}

void CmndPowerHigh(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
    Settings.energy_max_power = XdrvMailbox.payload;
  }
  EnergyCommandResponse(Settings.energy_max_power, UNIT_WATT);
}

void CmndVoltageLow(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 501)) {
    Settings.energy_min_voltage = XdrvMailbox.payload;
  }
  EnergyCommandResponse(Settings.energy_min_voltage, UNIT_VOLT);
}

void CmndVoltageHigh(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 501)) {
    Settings.energy_max_voltage = XdrvMailbox.payload;
  }
  EnergyCommandResponse(Settings.energy_max_voltage, UNIT_VOLT);
}

void CmndCurrentLow(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 16001)) {
    Settings.energy_min_current = XdrvMailbox.payload;
  }
  EnergyCommandResponse(Settings.energy_min_current, UNIT_MILLIAMPERE);
}

void CmndCurrentHigh(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 16001)) {
    Settings.energy_max_current = XdrvMailbox.payload;
  }
  EnergyCommandResponse(Settings.energy_max_current, UNIT_MILLIAMPERE);
}

#ifdef USE_ENERGY_POWER_LIMIT
void CmndMaxPower(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
    Settings.energy_max_power_limit = XdrvMailbox.payload;
  }
  EnergyCommandResponse(Settings.energy_max_power_limit, UNIT_WATT);
}

void CmndMaxPowerHold(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
    Settings.energy_max_power_limit_hold = (1 == XdrvMailbox.payload) ? MAX_POWER_HOLD : XdrvMailbox.payload;
  }
  EnergyCommandResponse(Settings.energy_max_power_limit_hold, UNIT_SECOND);
}

void CmndMaxPowerWindow(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
    Settings.energy_max_power_limit_window = (1 == XdrvMailbox.payload) ? MAX_POWER_WINDOW : XdrvMailbox.payload;
  }
  EnergyCommandResponse(Settings.energy_max_power_limit_window, UNIT_SECOND);
}

void CmndSafePower(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
    Settings.energy_max_power_safe_limit = XdrvMailbox.payload;
  }
  EnergyCommandResponse(Settings.energy_max_power_safe_limit, UNIT_WATT);
}

void CmndSafePowerHold(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
    Settings.energy_max_power_safe_limit_hold = (1 == XdrvMailbox.payload) ? SAFE_POWER_HOLD : XdrvMailbox.payload;
  }
  EnergyCommandResponse(Settings.energy_max_power_safe_limit_hold, UNIT_SECOND);
}

void CmndSafePowerWindow(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 1440)) {
    Settings.energy_max_power_safe_limit_window = (1 == XdrvMailbox.payload) ? SAFE_POWER_WINDOW : XdrvMailbox.payload;
  }
  EnergyCommandResponse(Settings.energy_max_power_safe_limit_window, UNIT_MINUTE);
}

void CmndMaxEnergy(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
    Settings.energy_max_energy = XdrvMailbox.payload;
    Energy.max_energy_state  = 3;
  }
  EnergyCommandResponse(Settings.energy_max_energy, UNIT_WATTHOUR);
}

void CmndMaxEnergyStart(void)
{
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 24)) {
    Settings.energy_max_energy_start = XdrvMailbox.payload;
  }
  EnergyCommandResponse(Settings.energy_max_energy_start, UNIT_HOUR);
}
#endif  // USE_ENERGY_POWER_LIMIT
#endif  // USE_ENERGY_MARGIN_DETECTION

void EnergyDrvInit(void)
{
  energy_flg = ENERGY_NONE;
  XnrgCall(FUNC_PRE_INIT);
}

void EnergySnsInit(void)
{
  XnrgCall(FUNC_INIT);

  if (energy_flg) {
    Energy.kWhtoday = (RtcSettingsValid()) ? RtcSettings.energy_kWhtoday : (RtcTime.day_of_year == Settings.energy_kWhdoy) ? Settings.energy_kWhtoday : 0;
    Energy.kWhtoday1 = (RtcSettingsValid()) ? RtcSettings.energy_usage.usage1_kWhtoday : (RtcTime.day_of_year == Settings.energy_kWhdoy) ? Settings.energy_usage.usage1_kWhtoday : 0;
    Energy.kWhtoday_delta = 0;
    Energy.period = Energy.kWhtoday;
    EnergyUpdateToday();
    ticker_energy.attach_ms(200, Energy200ms);
  }
}

#ifdef USE_WEBSERVER
const char HTTP_ENERGY_SNS1[] PROGMEM =
  "{s}" D_POWERUSAGE_APPARENT "{m}%s " D_UNIT_VA "{e}"
  "{s}" D_POWERUSAGE_REACTIVE "{m}%s " D_UNIT_VAR "{e}"
  "{s}" D_POWER_FACTOR "{m}%s{e}";

const char HTTP_ENERGY_SNS2[] PROGMEM =
  "{s}" D_ENERGY_TODAY  "{m}%s " D_UNIT_KILOWATTHOUR "{e}"
  "{s}" D_ENERGY_YESTERDAY "{m}%s " D_UNIT_KILOWATTHOUR "{e}"
  "{s}" D_ENERGY_TOTAL "{m}%s " D_UNIT_KILOWATTHOUR "{e}";      // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

void EnergyShow(bool json)
{
  char speriod[20];
//  char sfrequency[20];

  bool show_energy_period = (0 == tele_period);

  float power_factor = Energy.power_factor;

  char apparent_power_chr[33];
  char reactive_power_chr[33];
  char power_factor_chr[33];
  char frequency_chr[33];
  if (!Energy.type_dc) {
    if (Energy.current_available && Energy.voltage_available) {
      float apparent_power = Energy.apparent_power;
      if (isnan(apparent_power)) {
        apparent_power = Energy.voltage * Energy.current;
      }
      if (apparent_power < Energy.active_power) {  // Should be impossible
        Energy.active_power = apparent_power;
      }

      if (isnan(power_factor)) {
        power_factor = (Energy.active_power && apparent_power) ? Energy.active_power / apparent_power : 0;
        if (power_factor > 1) power_factor = 1;
      }

      float reactive_power = Energy.reactive_power;
      if (isnan(reactive_power)) {
        reactive_power = 0;
        uint32_t difference = ((uint32_t)(apparent_power * 100) - (uint32_t)(Energy.active_power * 100)) / 10;
        if ((Energy.current > 0.005) && ((difference > 15) || (difference > (uint32_t)(apparent_power * 100 / 1000)))) {
          // calculating reactive power only if current is greater than 0.005A and
          // difference between active and apparent power is greater than 1.5W or 1%
          reactive_power = (float)(RoundSqrtInt((uint32_t)(apparent_power * apparent_power * 100) - (uint32_t)(Energy.active_power * Energy.active_power * 100))) / 10;
        }
      }

      dtostrfd(apparent_power, Settings.flag2.wattage_resolution, apparent_power_chr);
      dtostrfd(reactive_power, Settings.flag2.wattage_resolution, reactive_power_chr);
      dtostrfd(power_factor, 2, power_factor_chr);
    }
    if (!isnan(Energy.frequency)) {
      dtostrfd(Energy.frequency, Settings.flag2.frequency_resolution, frequency_chr);
    }
  }

  char voltage_chr[33];
  dtostrfd(Energy.voltage, Settings.flag2.voltage_resolution, voltage_chr);
  char current_chr[33];
  dtostrfd(Energy.current, Settings.flag2.current_resolution, current_chr);
  char active_power_chr[33];
  dtostrfd(Energy.active_power, Settings.flag2.wattage_resolution, active_power_chr);
  char energy_daily_chr[33];
  dtostrfd(Energy.daily, Settings.flag2.energy_resolution, energy_daily_chr);
  char energy_yesterday_chr[33];
  dtostrfd((float)Settings.energy_kWhyesterday / 100000, Settings.flag2.energy_resolution, energy_yesterday_chr);
  char energy_total_chr[33];
  dtostrfd(Energy.total, Settings.flag2.energy_resolution, energy_total_chr);

  float energy = 0;
  char energy_period_chr[33];
  if (show_energy_period) {
    if (Energy.period) energy = (float)(Energy.kWhtoday - Energy.period) / 100;
    Energy.period = Energy.kWhtoday;
    dtostrfd(energy, Settings.flag2.wattage_resolution, energy_period_chr);
    snprintf_P(speriod, sizeof(speriod), PSTR(",\"" D_JSON_PERIOD "\":%s"), energy_period_chr);
  }

  if (json) {
    ResponseAppend_P(PSTR(",\"" D_RSLT_ENERGY "\":{\"" D_JSON_TOTAL_START_TIME "\":\"%s\",\"" D_JSON_TOTAL "\":%s,\"" D_JSON_YESTERDAY "\":%s,\"" D_JSON_TODAY "\":%s%s,\"" D_JSON_POWERUSAGE "\":%s"),
      GetDateAndTime(DT_ENERGY).c_str(), energy_total_chr, energy_yesterday_chr, energy_daily_chr, (show_energy_period) ? speriod : "", active_power_chr);
    if (!Energy.type_dc) {
      if (Energy.current_available && Energy.voltage_available) {
        ResponseAppend_P(PSTR(",\"" D_JSON_APPARENT_POWERUSAGE "\":%s,\"" D_JSON_REACTIVE_POWERUSAGE "\":%s,\"" D_JSON_POWERFACTOR "\":%s"),
          apparent_power_chr, reactive_power_chr, power_factor_chr);
      }
      if (!isnan(Energy.frequency)) {
        ResponseAppend_P(PSTR(",\"" D_JSON_FREQUENCY "\":%s"), frequency_chr);
      }
    }
    if (Energy.voltage_available) {
      ResponseAppend_P(PSTR(",\"" D_JSON_VOLTAGE "\":%s"), voltage_chr);
    }
    if (Energy.current_available) {
      ResponseAppend_P(PSTR(",\"" D_JSON_CURRENT "\":%s"), current_chr);
    }
    ResponseJsonEnd();

#ifdef USE_DOMOTICZ
    if (show_energy_period) {  // Only send if telemetry
      dtostrfd(Energy.total * 1000, 1, energy_total_chr);
      DomoticzSensorPowerEnergy((int)Energy.active_power, energy_total_chr);  // PowerUsage, EnergyToday

      dtostrfd((Energy.total - Energy.total1) * 1000, 1, energy_total_chr);  // Tariff2
      char energy_total1_chr[33];
      dtostrfd(Energy.total1 * 1000, 1, energy_total1_chr);  // Tariff1
      char energy_non[2] = "0";
      DomoticzSensorP1SmartMeter(energy_total1_chr, energy_total_chr, energy_non, energy_non, (int)Energy.active_power, 0);

      if (Energy.voltage_available) {
        DomoticzSensor(DZ_VOLTAGE, voltage_chr);  // Voltage
      }
      if (Energy.current_available) {
        DomoticzSensor(DZ_CURRENT, current_chr);  // Current
      }
    }
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
    if (show_energy_period) {
      if (Energy.voltage_available) {
        KnxSensor(KNX_ENERGY_VOLTAGE, Energy.voltage);
      }
      if (Energy.current_available) {
        KnxSensor(KNX_ENERGY_CURRENT, Energy.current);
      }
      KnxSensor(KNX_ENERGY_POWER, Energy.active_power);
      if (!Energy.type_dc) { KnxSensor(KNX_ENERGY_POWERFACTOR, power_factor); }
      KnxSensor(KNX_ENERGY_DAILY, Energy.daily);
      KnxSensor(KNX_ENERGY_TOTAL, Energy.total);
      KnxSensor(KNX_ENERGY_START, Energy.start_energy);
    }
#endif  // USE_KNX
#ifdef USE_WEBSERVER
  } else {
    if (Energy.voltage_available) {
      WSContentSend_PD(PSTR("{s}" D_VOLTAGE "{m}%s " D_UNIT_VOLT "{e}"), voltage_chr);
    }
    if (Energy.current_available) {
      WSContentSend_PD(PSTR("{s}" D_CURRENT "{m}%s " D_UNIT_AMPERE "{e}"), current_chr);
    }
    WSContentSend_PD(PSTR("{s}" D_POWERUSAGE "{m}%s " D_UNIT_WATT "{e}"), active_power_chr);
    if (!Energy.type_dc) {
      if (Energy.current_available && Energy.voltage_available) {
        WSContentSend_PD(HTTP_ENERGY_SNS1, apparent_power_chr, reactive_power_chr, power_factor_chr);
      }
      if (!isnan(Energy.frequency)) {
        WSContentSend_PD(PSTR("{s}" D_FREQUENCY "{m}%s " D_UNIT_HERTZ "{e}"), frequency_chr);
      }
    }
    WSContentSend_PD(HTTP_ENERGY_SNS2, energy_daily_chr, energy_yesterday_chr, energy_total_chr);
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv03(uint8_t function)
{
  bool result = false;

  if (FUNC_PRE_INIT == function) {
    EnergyDrvInit();
  }
  else if (energy_flg) {
    switch (function) {
      case FUNC_LOOP:
        XnrgCall(FUNC_LOOP);
        break;
#ifdef USE_ENERGY_MARGIN_DETECTION
      case FUNC_SET_POWER:
        Energy.power_steady_counter = 2;
        break;
#endif  // USE_ENERGY_MARGIN_DETECTION
      case FUNC_SERIAL:
        result = XnrgCall(FUNC_SERIAL);
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kEnergyCommands, EnergyCommand);
        break;
    }
  }
  return result;
}

bool Xsns03(uint8_t function)
{
  bool result = false;

  if (energy_flg) {
    switch (function) {
      case FUNC_INIT:
        EnergySnsInit();
        break;
      case FUNC_EVERY_SECOND:
#ifdef USE_ENERGY_MARGIN_DETECTION
        EnergyMarginCheck();
#endif  // USE_ENERGY_MARGIN_DETECTION
        EnergyOverTempCheck();
        break;
      case FUNC_JSON_APPEND:
        EnergyShow(true);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        EnergyShow(false);
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
