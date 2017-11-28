/*
  xsns_03_hlw8012.ino - sonoff pow HLW8012 energy sensor support for Sonoff-Tasmota

  Copyright (C) 2017  Theo Arends

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

/*********************************************************************************************\
 * HLW8012 - Energy
 *
 * Based on Source: Shenzhen Heli Technology Co., Ltd
\*********************************************************************************************/

#define FEATURE_POWER_LIMIT  true

/*********************************************************************************************/

#define HLW_PREF            10000    // 1000.0W
#define HLW_UREF             2200    // 220.0V
#define HLW_IREF             4545    // 4.545A

#define HLW_POWER_PROBE_TIME   10    // Number of seconds to probe for power before deciding none used

enum Hlw8012Commands {
  CMND_POWERLOW, CMND_POWERHIGH, CMND_VOLTAGELOW, CMND_VOLTAGEHIGH, CMND_CURRENTLOW, CMND_CURRENTHIGH,
  CMND_HLWPCAL, CMND_HLWPSET, CMND_HLWUCAL, CMND_HLWUSET, CMND_HLWICAL, CMND_HLWISET,
  CMND_ENERGYRESET, CMND_MAXENERGY, CMND_MAXENERGYSTART,
  CMND_MAXPOWER, CMND_MAXPOWERHOLD, CMND_MAXPOWERWINDOW,
  CMND_SAFEPOWER, CMND_SAFEPOWERHOLD, CMND_SAFEPOWERWINDOW };
const char kHlw8012Commands[] PROGMEM =
  D_CMND_POWERLOW "|" D_CMND_POWERHIGH "|" D_CMND_VOLTAGELOW "|" D_CMND_VOLTAGEHIGH "|" D_CMND_CURRENTLOW "|" D_CMND_CURRENTHIGH "|"
  D_CMND_HLWPCAL "|" D_CMND_HLWPSET "|" D_CMND_HLWUCAL "|" D_CMND_HLWUSET "|" D_CMND_HLWICAL "|" D_CMND_HLWISET "|"
  D_CMND_ENERGYRESET "|" D_CMND_MAXENERGY "|" D_CMND_MAXENERGYSTART "|"
  D_CMND_MAXPOWER "|" D_CMND_MAXPOWERHOLD "|" D_CMND_MAXPOWERWINDOW "|"
  D_CMND_SAFEPOWER "|" D_CMND_SAFEPOWERHOLD "|"  D_CMND_SAFEPOWERWINDOW ;

byte hlw_pmin_flag = 0;
byte hlw_pmax_flag = 0;
byte hlw_umin_flag = 0;
byte hlw_umax_flag = 0;
byte hlw_imin_flag = 0;
byte hlw_imax_flag = 0;

byte power_steady_cntr;
byte hlw_mkwh_state = 0;

#if FEATURE_POWER_LIMIT
  byte hlw_mplr_counter = 0;
  uint16_t hlw_mplh_counter = 0;
  uint16_t hlw_mplw_counter = 0;
#endif  // FEATURE_POWER_LIMIT

byte hlw_select_ui_flag;
byte hlw_load_off;
byte hlw_cf1_timer;
byte hlw_fifth_second;
byte hlw_startup;
unsigned long hlw_cf_pulse_length;
unsigned long hlw_cf_pulse_last_time;
unsigned long hlw_cf1_pulse_length;
unsigned long hlw_cf1_pulse_last_time;
unsigned long hlw_cf1_summed_pulse_length;
unsigned long hlw_cf1_pulse_counter;
unsigned long hlw_cf1_voltage_pulse_length;
unsigned long hlw_cf1_current_pulse_length;
unsigned long hlw_energy_counter;
unsigned long hlw_energy_period_counter;
unsigned long hlw_kWhtoday;
uint32_t hlw_lasttime;

unsigned long hlw_cf1_voltage_max_pulse_counter;
unsigned long hlw_cf1_current_max_pulse_counter;

Ticker tickerHLW;

#ifndef USE_WS2812_DMA  // Collides with Neopixelbus but solves exception
void HlwCfInterrupt() ICACHE_RAM_ATTR;
void HlwCf1Interrupt() ICACHE_RAM_ATTR;
#endif  // USE_WS2812_DMA

void HlwCfInterrupt()  // Service Power
{
  unsigned long us = micros();

  if (hlw_load_off) {  // Restart plen measurement
    hlw_cf_pulse_last_time = us;
    hlw_load_off = 0;
  } else {
    hlw_cf_pulse_length = us - hlw_cf_pulse_last_time;
    hlw_cf_pulse_last_time = us;
    hlw_energy_period_counter++;
    hlw_energy_counter++;
  }
}

void HlwCf1Interrupt()  // Service Voltage and Current
{
  unsigned long us = micros();

  hlw_cf1_pulse_length = us - hlw_cf1_pulse_last_time;
  hlw_cf1_pulse_last_time = us;
  if ((hlw_cf1_timer > 2) && (hlw_cf1_timer < 8)) {  // Allow for 300 mSec set-up time and measure for up to 1 second
    hlw_cf1_summed_pulse_length += hlw_cf1_pulse_length;
    hlw_cf1_pulse_counter++;
    if (10 == hlw_cf1_pulse_counter) {
      hlw_cf1_timer = 8;  // We need up to ten samples within 1 second (low current could take up to 0.3 second)
    }
  }
}

void hlw_200mS()
{
  unsigned long hlw_len;
  unsigned long hlw_temp;

  hlw_fifth_second++;
  if (5 == hlw_fifth_second) {
    hlw_fifth_second = 0;

    if (hlw_energy_period_counter) {
      hlw_len = 10000 / hlw_energy_period_counter;
      hlw_energy_period_counter = 0;
      if (hlw_len) {
        hlw_temp = ((HLW_PREF * Settings.hlw_power_calibration) / hlw_len) / 36;
        hlw_kWhtoday += hlw_temp;
        RtcSettings.hlw_kWhtoday = hlw_kWhtoday;
      }
    }
    if (RtcTime.valid) {
      if (LocalTime() == Midnight()) {
        Settings.hlw_kWhyesterday = hlw_kWhtoday;
        Settings.hlw_kWhtotal += (hlw_kWhtoday / 1000);
        RtcSettings.hlw_kWhtotal = Settings.hlw_kWhtotal;
        hlw_kWhtoday = 0;
        RtcSettings.hlw_kWhtoday = hlw_kWhtoday;
        hlw_mkwh_state = 3;
      }
      if ((RtcTime.hour == Settings.hlw_mkwhs) && (3 == hlw_mkwh_state)) {
        hlw_mkwh_state = 0;
      }
      if (hlw_startup && (RtcTime.day_of_year == Settings.hlw_kWhdoy)) {
        hlw_kWhtoday = Settings.hlw_kWhtoday;
        RtcSettings.hlw_kWhtoday = hlw_kWhtoday;
        hlw_startup = 0;
      }
    }
  }

  if (micros() - hlw_cf_pulse_last_time > (HLW_POWER_PROBE_TIME * 1000000)) {
    hlw_cf_pulse_length = 0;    // No load for some time
    hlw_load_off = 1;
  }

  hlw_cf1_timer++;
  if (hlw_cf1_timer >= 8) {
    hlw_cf1_timer = 0;
    hlw_select_ui_flag = (hlw_select_ui_flag) ? 0 : 1;
    digitalWrite(pin[GPIO_HLW_SEL], hlw_select_ui_flag);

    if (hlw_cf1_pulse_counter) {
      hlw_cf1_pulse_length = hlw_cf1_summed_pulse_length / hlw_cf1_pulse_counter;
    } else {
      hlw_cf1_pulse_length = 0;
    }
    if (hlw_select_ui_flag) {
      hlw_cf1_voltage_pulse_length = hlw_cf1_pulse_length;
      hlw_cf1_voltage_max_pulse_counter = hlw_cf1_pulse_counter;
    } else {
      hlw_cf1_current_pulse_length = hlw_cf1_pulse_length;
      hlw_cf1_current_max_pulse_counter = hlw_cf1_pulse_counter;
    }
    hlw_cf1_summed_pulse_length = 0;
    hlw_cf1_pulse_counter = 0;
  }
}

void HlwSaveState()
{
  Settings.hlw_kWhdoy = (RtcTime.valid) ? RtcTime.day_of_year : 0;
  Settings.hlw_kWhtoday = hlw_kWhtoday;
  Settings.hlw_kWhtotal = RtcSettings.hlw_kWhtotal;
}

void HlwReadEnergy(byte option, float &total_energy, float &daily_energy, float &energy, float &watts, float &voltage, float &current, float &power_factor)
{
/* option 0 = do not calculate period energy usage
 * option 1 = calculate period energy usage
 */
  unsigned long cur_kWhtoday = hlw_kWhtoday;
  unsigned long hlw_len;
  unsigned long hlw_temp;
  unsigned long hlw_w;
  unsigned long hlw_u;
  unsigned long hlw_i;
  uint16_t hlw_period;

//snprintf_P(log_data, sizeof(log_data), PSTR("HLW: CF %d, CF1U %d (%d), CF1I %d (%d)"), hlw_cf_pulse_length, hlw_cf1_voltage_pulse_length, hlw_cf1_voltage_max_pulse_counter, hlw_cf1_current_pulse_length, hlw_cf1_current_max_pulse_counter);
//AddLog(LOG_LEVEL_DEBUG);

  total_energy = (float)(RtcSettings.hlw_kWhtotal + (cur_kWhtoday / 1000)) / 100000;
  daily_energy = 0;
  if (cur_kWhtoday) {
    daily_energy = (float)cur_kWhtoday / 100000000;
  }
  energy = 0;
  if (option) {
    if (!hlw_lasttime) {
      hlw_period = Settings.tele_period;
    } else {
      hlw_period = LocalTime() - hlw_lasttime;
    }
    hlw_lasttime = LocalTime();
    if (hlw_period) {
      uint16_t hlw_interval = 3600 / hlw_period;
      if (hlw_energy_counter) {
        hlw_len = hlw_period * 1000000 / hlw_energy_counter;
        if (hlw_interval && hlw_len) {
          hlw_energy_counter = 0;
          hlw_temp = ((HLW_PREF * Settings.hlw_power_calibration) / hlw_len) / hlw_interval;
          energy = (float)hlw_temp / 10;
        }
      }
    }
  }
  watts = 0;
  if (hlw_cf_pulse_length  && (power &1) && !hlw_load_off) {
    hlw_w = (HLW_PREF * Settings.hlw_power_calibration) / hlw_cf_pulse_length;
    watts = (float)hlw_w / 10;
  }
  voltage = 0;
  if (hlw_cf1_voltage_pulse_length && (power &1)) {     // If powered on always provide voltage
    hlw_u = (HLW_UREF * Settings.hlw_voltage_calibration) / hlw_cf1_voltage_pulse_length;
    voltage = (float)hlw_u / 10;
  }
  current = 0;
  if (hlw_cf1_current_pulse_length && watts) {             // No current if no power being consumed
    hlw_i = (HLW_IREF * Settings.hlw_current_calibration) / hlw_cf1_current_pulse_length;
    current = (float)hlw_i / 1000;
  }
  power_factor = 0;
  if (hlw_i && hlw_u && hlw_w && watts) {
    hlw_temp = (hlw_w * 100) / ((hlw_u * hlw_i) / 1000);
    if (hlw_temp > 100) {
      hlw_temp = 100;
    }
    power_factor = (float)hlw_temp / 100;
  }
}

void HlwInit()
{
  if (!Settings.hlw_power_calibration || (4975 == Settings.hlw_power_calibration)) {
    Settings.hlw_power_calibration = HLW_PREF_PULSE;
    Settings.hlw_voltage_calibration = HLW_UREF_PULSE;
    Settings.hlw_current_calibration = HLW_IREF_PULSE;
  }

  hlw_cf_pulse_length = 0;
  hlw_cf_pulse_last_time = 0;
  hlw_cf1_pulse_length = 0;
  hlw_cf1_pulse_last_time = 0;
  hlw_cf1_voltage_pulse_length = 0;
  hlw_cf1_current_pulse_length = 0;
  hlw_cf1_voltage_max_pulse_counter = 0;
  hlw_cf1_current_max_pulse_counter = 0;

  hlw_load_off = 1;
  hlw_energy_counter = 0;
  hlw_energy_period_counter = 0;
  hlw_kWhtoday = (RtcSettingsValid()) ? RtcSettings.hlw_kWhtoday : 0;

  hlw_select_ui_flag = 0;  // Voltage;

  pinMode(pin[GPIO_HLW_SEL], OUTPUT);
  digitalWrite(pin[GPIO_HLW_SEL], hlw_select_ui_flag);
  pinMode(pin[GPIO_HLW_CF1], INPUT_PULLUP);
  attachInterrupt(pin[GPIO_HLW_CF1], HlwCf1Interrupt, FALLING);
  pinMode(pin[GPIO_HLW_CF], INPUT_PULLUP);
  attachInterrupt(pin[GPIO_HLW_CF], HlwCfInterrupt, FALLING);

  hlw_startup = 1;
  hlw_lasttime = 0;
  hlw_fifth_second = 0;
  hlw_cf1_timer = 0;
  tickerHLW.attach_ms(200, hlw_200mS);
}

/********************************************************************************************/

boolean HlwMargin(byte type, uint16_t margin, uint16_t value, byte &flag, byte &save_flag)
{
  byte change;

  if (!margin) {
    return false;
  }
  change = save_flag;
  if (type) {
    flag = (value > margin);
  } else {
    flag = (value < margin);
  }
  save_flag = flag;
  return (change != save_flag);
}

void HlwSetPowerSteadyCounter(byte value)
{
  power_steady_cntr = 2;
}

void HlwMarginCheck()
{
  float total_energy;
  float daily_energy;
  float energy;
  float watts;
  float voltage;
  float current;
  float power_factor;
  uint16_t udaily_energy;
  uint16_t uwatts;
  uint16_t uvoltage;
  uint16_t ucurrent;
  boolean flag;
  boolean jsonflg;

  if (power_steady_cntr) {
    power_steady_cntr--;
    return;
  }

  HlwReadEnergy(0, total_energy, daily_energy, energy, watts, voltage, current, power_factor);
  if (power && (Settings.hlw_pmin || Settings.hlw_pmax || Settings.hlw_umin || Settings.hlw_umax || Settings.hlw_imin || Settings.hlw_imax)) {
    uwatts = (uint16_t)(watts);
    uvoltage = (uint16_t)(voltage);
    ucurrent = (uint16_t)(current * 1000);

//    snprintf_P(log_data, sizeof(log_data), PSTR("HLW: W %d, U %d, I %d"), watts, voltage, ucurrent);
//    AddLog(LOG_LEVEL_DEBUG);

    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{"));
    jsonflg = 0;
    if (HlwMargin(0, Settings.hlw_pmin, uwatts, flag, hlw_pmin_flag)) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"" D_CMND_POWERLOW "\":\"%s\""), mqtt_data, (jsonflg)?",":"", GetStateText(flag));
      jsonflg = 1;
    }
    if (HlwMargin(1, Settings.hlw_pmax, uwatts, flag, hlw_pmax_flag)) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"" D_CMND_POWERHIGH "\":\"%s\""), mqtt_data, (jsonflg)?",":"", GetStateText(flag));
      jsonflg = 1;
    }
    if (HlwMargin(0, Settings.hlw_umin, uvoltage, flag, hlw_umin_flag)) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"" D_CMND_VOLTAGELOW "\":\"%s\""), mqtt_data, (jsonflg)?",":"", GetStateText(flag));
      jsonflg = 1;
    }
    if (HlwMargin(1, Settings.hlw_umax, uvoltage, flag, hlw_umax_flag)) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"" D_CMND_VOLTAGEHIGH "\":\"%s\""), mqtt_data, (jsonflg)?",":"", GetStateText(flag));
      jsonflg = 1;
    }
    if (HlwMargin(0, Settings.hlw_imin, ucurrent, flag, hlw_imin_flag)) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"" D_CMND_CURRENTLOW "\":\"%s\""), mqtt_data, (jsonflg)?",":"", GetStateText(flag));
      jsonflg = 1;
    }
    if (HlwMargin(1, Settings.hlw_imax, ucurrent, flag, hlw_imax_flag)) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"" D_CMND_CURRENTHIGH "\":\"%s\""), mqtt_data, (jsonflg)?",":"", GetStateText(flag));
      jsonflg = 1;
    }
    if (jsonflg) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
      MqttPublishPrefixTopic_P(2, PSTR(D_RSLT_MARGINS));
      MqttShowHlw8012(0);
    }
  }

#if FEATURE_POWER_LIMIT
  // Max Power
  if (Settings.hlw_mpl) {
    if (watts > Settings.hlw_mpl) {
      if (!hlw_mplh_counter) {
        hlw_mplh_counter = Settings.hlw_mplh;
      } else {
        hlw_mplh_counter--;
        if (!hlw_mplh_counter) {
          snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_MAXPOWERREACHED "\":\"%d%s\"}"), uwatts, (Settings.flag.value_units) ? " " D_UNIT_WATT : "");
          MqttPublishPrefixTopic_P(1, S_RSLT_WARNING);
          MqttShowHlw8012(0);
          ExecuteCommandPower(1, 0);
          if (!hlw_mplr_counter) {
            hlw_mplr_counter = Settings.param[P_MAX_POWER_RETRY] +1;
          }
          hlw_mplw_counter = Settings.hlw_mplw;
        }
      }
    }
    else if (power && (uwatts <= Settings.hlw_mpl)) {
      hlw_mplh_counter = 0;
      hlw_mplr_counter = 0;
      hlw_mplw_counter = 0;
    }
    if (!power) {
      if (hlw_mplw_counter) {
        hlw_mplw_counter--;
      } else {
        if (hlw_mplr_counter) {
          hlw_mplr_counter--;
          if (hlw_mplr_counter) {
            snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_POWERMONITOR "\":\"%s\"}"), GetStateText(1));
            MqttPublishPrefixTopic_P(5, PSTR(D_POWERMONITOR));
            ExecuteCommandPower(1, 1);
          } else {
            snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_MAXPOWERREACHEDRETRY "\":\"%s\"}"), GetStateText(0));
            MqttPublishPrefixTopic_P(1, S_RSLT_WARNING);
            MqttShowHlw8012(0);
          }
        }
      }
    }
  }

  // Max Energy
  if (Settings.hlw_mkwh) {
    udaily_energy = (uint16_t)(daily_energy * 1000);
    if (!hlw_mkwh_state && (RtcTime.hour == Settings.hlw_mkwhs)) {
      hlw_mkwh_state = 1;
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_ENERGYMONITOR "\":\"%s\"}"), GetStateText(1));
      MqttPublishPrefixTopic_P(5, PSTR(D_ENERGYMONITOR));
      ExecuteCommandPower(1, 1);
    }
    else if ((1 == hlw_mkwh_state) && (udaily_energy >= Settings.hlw_mkwh)) {
      hlw_mkwh_state = 2;
      dtostrfd(daily_energy, 3, mqtt_data);
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_MAXENERGYREACHED "\":\"%s%s\"}"), mqtt_data, (Settings.flag.value_units) ? " " D_UNIT_KILOWATTHOUR : "");
      MqttPublishPrefixTopic_P(1, S_RSLT_WARNING);
      MqttShowHlw8012(0);
      ExecuteCommandPower(1, 0);
    }
  }
#endif  // FEATURE_POWER_LIMIT
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

boolean HlwCommand(char *type, uint16_t index, char *dataBuf, uint16_t data_len, int16_t payload)
{
  char command [CMDSZ];
  char sunit[CMDSZ];
  boolean serviced = true;
  uint8_t status_flag = 0;
  uint8_t unit = 0;
  unsigned long nvalue = 0;

  int command_code = GetCommandCode(command, sizeof(command), type, kHlw8012Commands);
  if (CMND_POWERLOW == command_code) {
    if ((payload >= 0) && (payload < 3601)) {
      Settings.hlw_pmin = payload;
    }
    nvalue = Settings.hlw_pmin;
    unit = UNIT_WATT;
  }
  else if (CMND_POWERHIGH == command_code) {
    if ((payload >= 0) && (payload < 3601)) {
      Settings.hlw_pmax = payload;
    }
    nvalue = Settings.hlw_pmax;
    unit = UNIT_WATT;
  }
  else if (CMND_VOLTAGELOW == command_code) {
    if ((payload >= 0) && (payload < 501)) {
      Settings.hlw_umin = payload;
    }
    nvalue = Settings.hlw_umin;
    unit = UNIT_VOLT;
  }
  else if (CMND_VOLTAGEHIGH == command_code) {
    if ((payload >= 0) && (payload < 501)) {
      Settings.hlw_umax = payload;
    }
    nvalue = Settings.hlw_umax;
    unit = UNIT_VOLT;
  }
  else if (CMND_CURRENTLOW == command_code) {
    if ((payload >= 0) && (payload < 16001)) {
      Settings.hlw_imin = payload;
    }
    nvalue = Settings.hlw_imin;
    unit = UNIT_MILLIAMPERE;
  }
  else if (CMND_CURRENTHIGH == command_code) {
    if ((payload >= 0) && (payload < 16001)) {
      Settings.hlw_imax = payload;
    }
    nvalue = Settings.hlw_imax;
    unit = UNIT_MILLIAMPERE;
  }
  else if ((CMND_ENERGYRESET == command_code) && (index > 0) && (index <= 3)) {
    char *p;
    unsigned long lnum = strtoul(dataBuf, &p, 10);
    if (p != dataBuf) {
      switch (index) {
      case 1:
        hlw_kWhtoday = lnum *100000;
        RtcSettings.hlw_kWhtoday = hlw_kWhtoday;
        Settings.hlw_kWhtoday = hlw_kWhtoday;
        break;
      case 2:
        Settings.hlw_kWhyesterday = lnum *100000;
        break;
      case 3:
        RtcSettings.hlw_kWhtotal = lnum *100;
        Settings.hlw_kWhtotal = RtcSettings.hlw_kWhtotal;
        break;
      }
    }
    char syesterday_energy[10];
    char stoday_energy[10];
    char stotal_energy[10];
    dtostrfd((float)Settings.hlw_kWhyesterday / 100000000, Settings.flag2.energy_resolution, syesterday_energy);
    dtostrfd((float)RtcSettings.hlw_kWhtoday / 100000000, Settings.flag2.energy_resolution, stoday_energy);
    dtostrfd((float)(RtcSettings.hlw_kWhtotal + (hlw_kWhtoday / 1000)) / 100000, Settings.flag2.energy_resolution, stotal_energy);
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"%s\":{\"" D_TOTAL "\":%s,\"" D_YESTERDAY "\":%s,\"" D_TODAY "\":%s}}"),
      command, stotal_energy, syesterday_energy, stoday_energy);
    status_flag = 1;
  }
  else if (CMND_HLWPCAL == command_code) {
    if ((payload > 0) && (payload < 32001)) {
      Settings.hlw_power_calibration = (payload > 4000) ? payload : HLW_PREF_PULSE;  // 12530
    }
    nvalue = Settings.hlw_power_calibration;
    unit = UNIT_MICROSECOND;
  }
  else if (CMND_HLWPSET == command_code) {
    if ((payload > 0) && (payload < 3601) && hlw_cf_pulse_length) {
      Settings.hlw_power_calibration = (payload * 10 * hlw_cf_pulse_length) / HLW_PREF;
    }
    snprintf_P(command, sizeof(command), PSTR(D_CMND_HLWPCAL));
    nvalue = Settings.hlw_power_calibration;
    unit = UNIT_MICROSECOND;
  }
  else if (CMND_HLWUCAL == command_code) {
    if ((payload > 0) && (payload < 32001)) {
      Settings.hlw_voltage_calibration = (payload > 999) ? payload : HLW_UREF_PULSE;  // 1950
    }
    nvalue = Settings.hlw_voltage_calibration;
    unit = UNIT_MICROSECOND;
  }
  else if (CMND_HLWUSET == command_code) {
    if ((payload > 0) && (payload < 501) && hlw_cf1_voltage_pulse_length) {
      Settings.hlw_voltage_calibration = (payload * 10 * hlw_cf1_voltage_pulse_length) / HLW_UREF;
    }
    snprintf_P(command, sizeof(command), PSTR(D_CMND_HLWUCAL));
    nvalue = Settings.hlw_voltage_calibration;
    unit = UNIT_MICROSECOND;
  }
  else if (CMND_HLWICAL == command_code) {
    if ((payload > 0) && (payload < 32001)) {
      Settings.hlw_current_calibration = (payload > 1100) ? payload : HLW_IREF_PULSE;  // 3500
    }
    nvalue = Settings.hlw_current_calibration;
    unit = UNIT_MICROSECOND;
  }
  else if (CMND_HLWISET == command_code) {
    if ((payload > 0) && (payload < 16001) && hlw_cf1_current_pulse_length) {
      Settings.hlw_current_calibration = (payload * hlw_cf1_current_pulse_length) / HLW_IREF;
    }
    snprintf_P(command, sizeof(command), PSTR(D_CMND_HLWICAL));
    nvalue = Settings.hlw_current_calibration;
    unit = UNIT_MICROSECOND;
  }
#if FEATURE_POWER_LIMIT
  else if (CMND_MAXPOWER == command_code) {
    if ((payload >= 0) && (payload < 3601)) {
      Settings.hlw_mpl = payload;
    }
    nvalue = Settings.hlw_mpl;
    unit = UNIT_WATT;
  }
  else if (CMND_MAXPOWERHOLD == command_code) {
    if ((payload >= 0) && (payload < 3601)) {
      Settings.hlw_mplh = (1 == payload) ? MAX_POWER_HOLD : payload;
    }
    nvalue = Settings.hlw_mplh;
    unit = UNIT_SECOND;
  }
  else if (CMND_MAXPOWERWINDOW == command_code) {
    if ((payload >= 0) && (payload < 3601)) {
      Settings.hlw_mplw = (1 == payload) ? MAX_POWER_WINDOW : payload;
    }
    nvalue = Settings.hlw_mplw;
    unit = UNIT_SECOND;
  }
  else if (CMND_SAFEPOWER == command_code) {
    if ((payload >= 0) && (payload < 3601)) {
      Settings.hlw_mspl = payload;
    }
    nvalue = Settings.hlw_mspl;
    unit = UNIT_WATT;
  }
  else if (CMND_SAFEPOWERHOLD == command_code) {
    if ((payload >= 0) && (payload < 3601)) {
      Settings.hlw_msplh = (1 == payload) ? SAFE_POWER_HOLD : payload;
    }
    nvalue = Settings.hlw_msplh;
    unit = UNIT_SECOND;
  }
  else if (CMND_SAFEPOWERWINDOW == command_code) {
    if ((payload >= 0) && (payload < 1440)) {
      Settings.hlw_msplw = (1 == payload) ? SAFE_POWER_WINDOW : payload;
    }
    nvalue = Settings.hlw_msplw;
    unit = UNIT_MINUTE;
  }
  else if (CMND_MAXENERGY == command_code) {
    if ((payload >= 0) && (payload < 3601)) {
      Settings.hlw_mkwh = payload;
      hlw_mkwh_state = 3;
    }
    nvalue = Settings.hlw_mkwh;
    unit = UNIT_WATTHOUR;
  }
  else if (CMND_MAXENERGYSTART == command_code) {
    if ((payload >= 0) && (payload < 24)) {
      Settings.hlw_mkwhs = payload;
    }
    nvalue = Settings.hlw_mkwhs;
    unit = UNIT_HOUR;
  }
#endif  // FEATURE_POWER_LIMIT
  else {
    serviced = false;
  }
  if (!status_flag) {
    if (Settings.flag.value_units) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE_SPACE_UNIT, command, nvalue, GetTextIndexed(sunit, sizeof(sunit), unit, kUnitNames));
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command, nvalue);
    }
  }
  return serviced;
}

/********************************************************************************************/

#ifdef USE_WEBSERVER
const char HTTP_ENERGY_SNS[] PROGMEM =
  "{s}" D_VOLTAGE "{m}%s " D_UNIT_VOLT "{e}"
  "{s}" D_CURRENT "{m}%s " D_UNIT_AMPERE "{e}"
  "{s}" D_POWERUSAGE "{m}%s " D_UNIT_WATT "{e}"
  "{s}" D_POWER_FACTOR "{m}%s{e}"
  "{s}" D_ENERGY_TODAY  "{m}%s " D_UNIT_KILOWATTHOUR "{e}"
  "{s}" D_ENERGY_YESTERDAY "{m}%s " D_UNIT_KILOWATTHOUR "{e}"
  "{s}" D_ENERGY_TOTAL "{m}%s " D_UNIT_KILOWATTHOUR "{e}";      // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

void HlwShow(boolean json, boolean option)
{
/* option 0 = do not show period energy usage
 * option 1 = show period energy usage
 */
  float total_energy;
  float daily_energy;
  float energy;
  float watts;
  float voltage;
  float current;
  float power_factor;
  char stotal_energy[10];
  char sdaily_energy[10];
  char senergy[10];
  char swatts[10];
  char svoltage[10];
  char scurrent[10];
  char spower_factor[10];
  char syesterday_energy[10];
  char speriod[20];

  HlwReadEnergy(option, total_energy, daily_energy, energy, watts, voltage, current, power_factor);
  dtostrfd(total_energy, Settings.flag2.energy_resolution, stotal_energy);
  dtostrfd(daily_energy, Settings.flag2.energy_resolution, sdaily_energy);
  dtostrfd(energy, Settings.flag2.wattage_resolution, senergy);
  dtostrfd(watts, Settings.flag2.wattage_resolution, swatts);
  dtostrfd(voltage, Settings.flag2.voltage_resolution, svoltage);
  dtostrfd(current, Settings.flag2.current_resolution, scurrent);
  dtostrfd(power_factor, 2, spower_factor);
  dtostrfd((float)Settings.hlw_kWhyesterday / 100000000, Settings.flag2.energy_resolution, syesterday_energy);

  if (json) {
    snprintf_P(speriod, sizeof(speriod), PSTR(",\"" D_PERIOD "\":%s"), senergy);
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s\"" D_TOTAL "\":%s,\"" D_YESTERDAY "\":%s,\"" D_TODAY "\":%s%s,\"" D_POWERUSAGE "\":%s,\"" D_POWERFACTOR "\":%s,\"" D_VOLTAGE "\":%s,\"" D_CURRENT "\":%s}"),
      mqtt_data, stotal_energy, syesterday_energy, sdaily_energy, (option) ? speriod : "", swatts, spower_factor, svoltage, scurrent);
#ifdef USE_DOMOTICZ
    if (option) {  // Only send if telemetry
      dtostrfd(total_energy * 1000, 1, stotal_energy);
      DomoticzSensorPowerEnergy((uint16_t)watts, stotal_energy);  // PowerUsage, EnergyToday
      DomoticzSensor(DZ_VOLTAGE, svoltage);  // Voltage
      DomoticzSensor(DZ_CURRENT, scurrent);  // Current
    }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
  } else {
    snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_ENERGY_SNS, svoltage, scurrent, swatts, spower_factor, sdaily_energy, syesterday_energy, stotal_energy);
#endif  // USE_WEBSERVER
  }
}

void MqttShowHlw8012(byte option)
{
/* option 0 = do not show period energy usage
 * option 1 = show period energy usage
 */
// {"Time":"2017-03-04T13:37:24", "Total":0.013, "Yesterday":0.013, "Today":0.000, "Period":0, "Power":0, "Factor":0.00, "Voltage":0, "Current":0.000}
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_TIME "\":\"%s\","), GetDateAndTime().c_str());
  HlwShow(1, option);
  MqttPublishPrefixTopic_P(2, PSTR(D_RSLT_ENERGY), Settings.flag.mqtt_sensor_retain);
}

void HlwMqttStatus()
{
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_STATUS D_STATUS8_POWER "\":{"));
  HlwShow(1, 0);
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_03

boolean Xsns03(byte function)
{
  boolean result = false;

  if (hlw_flg) {
    switch (function) {
      case FUNC_XSNS_INIT:
        HlwInit();
        break;
//      case FUNC_XSNS_PREP:
//        break;
//      case FUNC_XSNS_JSON_APPEND:
//        break;
      case FUNC_XSNS_MQTT_SHOW:
        MqttShowHlw8012(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_XSNS_WEB:
        HlwShow(0, 0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}
