/*
  xsns_hlw8012.ino - sonoff pow HLW8012 energy sensor support for Sonoff-Tasmota

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

byte hlw_pminflg = 0;
byte hlw_pmaxflg = 0;
byte hlw_uminflg = 0;
byte hlw_umaxflg = 0;
byte hlw_iminflg = 0;
byte hlw_imaxflg = 0;

byte power_steady_cntr;
byte hlw_mkwh_state = 0;

#if FEATURE_POWER_LIMIT
  byte hlw_mplr_counter = 0;
  uint16_t hlw_mplh_counter = 0;
  uint16_t hlw_mplw_counter = 0;
#endif  // FEATURE_POWER_LIMIT

byte hlw_SELflag;
byte hlw_load_off;
byte hlw_cf1_timer;
byte hlw_fifth_second;
byte hlw_startup;
unsigned long hlw_cf_plen;
unsigned long hlw_cf_last;
unsigned long hlw_cf1_plen;
unsigned long hlw_cf1_last;
unsigned long hlw_cf1_ptot;
unsigned long hlw_cf1_pcnt;
unsigned long hlw_cf1u_plen;
unsigned long hlw_cf1i_plen;
unsigned long hlw_Ecntr;
unsigned long hlw_EDcntr;
unsigned long hlw_kWhtoday;
uint32_t hlw_lasttime;

unsigned long hlw_cf1u_pcntmax;
unsigned long hlw_cf1i_pcntmax;

Ticker tickerHLW;

#ifndef USE_WS2812_DMA  // Collides with Neopixelbus but solves exception
void hlw_cf_interrupt() ICACHE_RAM_ATTR;
void hlw_cf1_interrupt() ICACHE_RAM_ATTR;
#endif  // USE_WS2812_DMA

void hlw_cf_interrupt()  // Service Power
{
  unsigned long us = micros();

  if (hlw_load_off) {  // Restart plen measurement
    hlw_cf_last = us;
    hlw_load_off = 0;
  } else {
    hlw_cf_plen = us - hlw_cf_last;
    hlw_cf_last = us;
    hlw_EDcntr++;
    hlw_Ecntr++;
  }
}

void hlw_cf1_interrupt()  // Service Voltage and Current
{
  unsigned long us = micros();

  hlw_cf1_plen = us - hlw_cf1_last;
  hlw_cf1_last = us;
  if ((hlw_cf1_timer > 2) && (hlw_cf1_timer < 8)) {  // Allow for 300 mSec set-up time and measure for up to 1 second
    hlw_cf1_ptot += hlw_cf1_plen;
    hlw_cf1_pcnt++;
    if (10 == hlw_cf1_pcnt) {
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

    if (hlw_EDcntr) {
      hlw_len = 10000 / hlw_EDcntr;
      hlw_EDcntr = 0;
      if (hlw_len) {
        hlw_temp = ((HLW_PREF * sysCfg.hlw_pcal) / hlw_len) / 36;
        hlw_kWhtoday += hlw_temp;
        rtcMem.hlw_kWhtoday = hlw_kWhtoday;
      }
    }
    if (rtcTime.Valid) {
      if (rtc_loctime() == rtc_midnight()) {
        sysCfg.hlw_kWhyesterday = hlw_kWhtoday;
        sysCfg.hlw_kWhtotal += (hlw_kWhtoday / 1000);
        rtcMem.hlw_kWhtotal = sysCfg.hlw_kWhtotal;
        hlw_kWhtoday = 0;
        rtcMem.hlw_kWhtoday = hlw_kWhtoday;
        hlw_mkwh_state = 3;
      }
      if ((rtcTime.Hour == sysCfg.hlw_mkwhs) && (3 == hlw_mkwh_state)) {
        hlw_mkwh_state = 0;
      }
      if (hlw_startup && (rtcTime.DayOfYear == sysCfg.hlw_kWhdoy)) {
        hlw_kWhtoday = sysCfg.hlw_kWhtoday;
        rtcMem.hlw_kWhtoday = hlw_kWhtoday;
        hlw_startup = 0;
      }
    }
  }

  if (micros() - hlw_cf_last > (HLW_POWER_PROBE_TIME * 1000000)) {
    hlw_cf_plen = 0;    // No load for some time
    hlw_load_off = 1;
  }

  hlw_cf1_timer++;
  if (hlw_cf1_timer >= 8) {
    hlw_cf1_timer = 0;
    hlw_SELflag = (hlw_SELflag) ? 0 : 1;
    digitalWrite(pin[GPIO_HLW_SEL], hlw_SELflag);

    if (hlw_cf1_pcnt) {
      hlw_cf1_plen = hlw_cf1_ptot / hlw_cf1_pcnt;
    } else {
      hlw_cf1_plen = 0;
    }
    if (hlw_SELflag) {
      hlw_cf1u_plen = hlw_cf1_plen;
      hlw_cf1u_pcntmax = hlw_cf1_pcnt;
    } else {
      hlw_cf1i_plen = hlw_cf1_plen;
      hlw_cf1i_pcntmax = hlw_cf1_pcnt;
    }
    hlw_cf1_ptot = 0;
    hlw_cf1_pcnt = 0;
  }
}

void hlw_savestate()
{
  sysCfg.hlw_kWhdoy = (rtcTime.Valid) ? rtcTime.DayOfYear : 0;
  sysCfg.hlw_kWhtoday = hlw_kWhtoday;
  sysCfg.hlw_kWhtotal = rtcMem.hlw_kWhtotal;
}

void hlw_readEnergy(byte option, float &et, float &ed, float &e, float &w, float &u, float &i, float &c)
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
  uint16_t hlw_interval;

//snprintf_P(log_data, sizeof(log_data), PSTR("HLW: CF %d, CF1U %d (%d), CF1I %d (%d)"), hlw_cf_plen, hlw_cf1u_plen, hlw_cf1u_pcntmax, hlw_cf1i_plen, hlw_cf1i_pcntmax);
//addLog(LOG_LEVEL_DEBUG);

  et = (float)(rtcMem.hlw_kWhtotal + (cur_kWhtoday / 1000)) / 100000;
  ed = 0;
  if (cur_kWhtoday) {
    ed = (float)cur_kWhtoday / 100000000;
  }
  e = 0;
  if (option) {
    if (!hlw_lasttime) {
      hlw_period = sysCfg.tele_period;
    } else {
      hlw_period = rtc_loctime() - hlw_lasttime;
    }
    hlw_lasttime = rtc_loctime();
    if (hlw_period) {
      hlw_interval = 3600 / hlw_period;
      if (hlw_Ecntr) {
        hlw_len = hlw_period * 1000000 / hlw_Ecntr;
        if (hlw_interval && hlw_len) {
          hlw_Ecntr = 0;
          hlw_temp = ((HLW_PREF * sysCfg.hlw_pcal) / hlw_len) / hlw_interval;
          e = (float)hlw_temp / 10;
        }
      }
    }
  }
  w = 0;
  if (hlw_cf_plen  && (power &1) && !hlw_load_off) {
    hlw_w = (HLW_PREF * sysCfg.hlw_pcal) / hlw_cf_plen;
    w = (float)hlw_w / 10;
  }
  u = 0;
  if (hlw_cf1u_plen && (power &1)) {     // If powered on always provide voltage
    hlw_u = (HLW_UREF * sysCfg.hlw_ucal) / hlw_cf1u_plen;
    u = (float)hlw_u / 10;
  }
  i = 0;
  if (hlw_cf1i_plen && w) {             // No current if no power being consumed
    hlw_i = (HLW_IREF * sysCfg.hlw_ical) / hlw_cf1i_plen;
    i = (float)hlw_i / 1000;
  }
  c = 0;
  if (hlw_i && hlw_u && hlw_w && w) {
    hlw_temp = (hlw_w * 100) / ((hlw_u * hlw_i) / 1000);
    if (hlw_temp > 100) {
      hlw_temp = 100;
    }
    c = (float)hlw_temp / 100;
  }
}

void hlw_init()
{
  if (!sysCfg.hlw_pcal || (4975 == sysCfg.hlw_pcal)) {
    sysCfg.hlw_pcal = HLW_PREF_PULSE;
    sysCfg.hlw_ucal = HLW_UREF_PULSE;
    sysCfg.hlw_ical = HLW_IREF_PULSE;
  }

  hlw_cf_plen = 0;
  hlw_cf_last = 0;
  hlw_cf1_plen = 0;
  hlw_cf1_last = 0;
  hlw_cf1u_plen = 0;
  hlw_cf1i_plen = 0;
  hlw_cf1u_pcntmax = 0;
  hlw_cf1i_pcntmax = 0;

  hlw_load_off = 1;
  hlw_Ecntr = 0;
  hlw_EDcntr = 0;
  hlw_kWhtoday = (RTC_Valid()) ? rtcMem.hlw_kWhtoday : 0;

  hlw_SELflag = 0;  // Voltage;

  pinMode(pin[GPIO_HLW_SEL], OUTPUT);
  digitalWrite(pin[GPIO_HLW_SEL], hlw_SELflag);
  pinMode(pin[GPIO_HLW_CF1], INPUT_PULLUP);
  attachInterrupt(pin[GPIO_HLW_CF1], hlw_cf1_interrupt, FALLING);
  pinMode(pin[GPIO_HLW_CF], INPUT_PULLUP);
  attachInterrupt(pin[GPIO_HLW_CF], hlw_cf_interrupt, FALLING);

  hlw_startup = 1;
  hlw_lasttime = 0;
  hlw_fifth_second = 0;
  hlw_cf1_timer = 0;
  tickerHLW.attach_ms(200, hlw_200mS);
}

/********************************************************************************************/

boolean hlw_margin(byte type, uint16_t margin, uint16_t value, byte &flag, byte &saveflag)
{
  byte change;

  if (!margin) {
    return false;
  }
  change = saveflag;
  if (type) {
    flag = (value > margin);
  } else {
    flag = (value < margin);
  }
  saveflag = flag;
  return (change != saveflag);
}

void hlw_setPowerSteadyCounter(byte value)
{
  power_steady_cntr = 2;
}

void hlw_margin_chk()
{
  float pet;
  float ped;
  float pe;
  float pw;
  float pu;
  float pi;
  float pc;
  uint16_t uped;
  uint16_t pwv;
  uint16_t puv;
  uint16_t piv;
  boolean flag;
  boolean jsonflg;

  if (power_steady_cntr) {
    power_steady_cntr--;
    return;
  }

  hlw_readEnergy(0, pet, ped, pe, pw, pu, pi, pc);
  if (power && (sysCfg.hlw_pmin || sysCfg.hlw_pmax || sysCfg.hlw_umin || sysCfg.hlw_umax || sysCfg.hlw_imin || sysCfg.hlw_imax)) {
    pwv = (uint16_t)(pw);
    puv = (uint16_t)(pu);
    piv = (uint16_t)(pi * 1000);

//    snprintf_P(log_data, sizeof(log_data), PSTR("HLW: W %d, U %d, I %d"), pw, pu, piv);
//    addLog(LOG_LEVEL_DEBUG);

    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{"));
    jsonflg = 0;
    if (hlw_margin(0, sysCfg.hlw_pmin, pwv, flag, hlw_pminflg)) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"" D_CMND_POWERLOW "\":\"%s\""), mqtt_data, (jsonflg)?", ":"", getStateText(flag));
      jsonflg = 1;
    }
    if (hlw_margin(1, sysCfg.hlw_pmax, pwv, flag, hlw_pmaxflg)) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"" D_CMND_POWERHIGH "\":\"%s\""), mqtt_data, (jsonflg)?", ":"", getStateText(flag));
      jsonflg = 1;
    }
    if (hlw_margin(0, sysCfg.hlw_umin, puv, flag, hlw_uminflg)) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"" D_CMND_VOLTAGELOW "\":\"%s\""), mqtt_data, (jsonflg)?", ":"", getStateText(flag));
      jsonflg = 1;
    }
    if (hlw_margin(1, sysCfg.hlw_umax, puv, flag, hlw_umaxflg)) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"" D_CMND_VOLTAGEHIGH "\":\"%s\""), mqtt_data, (jsonflg)?", ":"", getStateText(flag));
      jsonflg = 1;
    }
    if (hlw_margin(0, sysCfg.hlw_imin, piv, flag, hlw_iminflg)) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"" D_CMND_CURRENTLOW "\":\"%s\""), mqtt_data, (jsonflg)?", ":"", getStateText(flag));
      jsonflg = 1;
    }
    if (hlw_margin(1, sysCfg.hlw_imax, piv, flag, hlw_imaxflg)) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"" D_CMND_CURRENTHIGH "\":\"%s\""), mqtt_data, (jsonflg)?", ":"", getStateText(flag));
      jsonflg = 1;
    }
    if (jsonflg) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
      mqtt_publish_topic_P(2, PSTR(D_RSLT_MARGINS));
      hlw_mqttPresent(0);
    }
  }

#if FEATURE_POWER_LIMIT
  // Max Power
  if (sysCfg.hlw_mpl) {
    if (pw > sysCfg.hlw_mpl) {
      if (!hlw_mplh_counter) {
        hlw_mplh_counter = sysCfg.hlw_mplh;
      } else {
        hlw_mplh_counter--;
        if (!hlw_mplh_counter) {
          snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_MAXPOWERREACHED "\":\"%d%s\"}"), pwv, (sysCfg.flag.value_units) ? " " D_UNIT_WATT : "");
          mqtt_publish_topic_P(1, S_RSLT_WARNING);
          hlw_mqttPresent(0);
          do_cmnd_power(1, 0);
          if (!hlw_mplr_counter) {
            hlw_mplr_counter = sysCfg.param[P_MAX_POWER_RETRY] +1;
          }
          hlw_mplw_counter = sysCfg.hlw_mplw;
        }
      }
    }
    else if (power && (pwv <= sysCfg.hlw_mpl)) {
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
            snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_POWERMONITOR "\":\"%s\"}"), getStateText(1));
            mqtt_publish_topic_P(5, PSTR(D_POWERMONITOR));
            do_cmnd_power(1, 1);
          } else {
            snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_MAXPOWERREACHEDRETRY "\":\"%s\"}"), getStateText(0));
            mqtt_publish_topic_P(1, S_RSLT_WARNING);
            hlw_mqttPresent(0);
          }
        }
      }
    }
  }

  // Max Energy
  if (sysCfg.hlw_mkwh) {
    uped = (uint16_t)(ped * 1000);
    if (!hlw_mkwh_state && (rtcTime.Hour == sysCfg.hlw_mkwhs)) {
      hlw_mkwh_state = 1;
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_ENERGYMONITOR "\":\"%s\"}"), getStateText(1));
      mqtt_publish_topic_P(5, PSTR(D_ENERGYMONITOR));
      do_cmnd_power(1, 1);
    }
    else if ((1 == hlw_mkwh_state) && (uped >= sysCfg.hlw_mkwh)) {
      hlw_mkwh_state = 2;
      dtostrfd(ped, 3, mqtt_data);
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_MAXENERGYREACHED "\":\"%s%s\"}"), mqtt_data, (sysCfg.flag.value_units) ? " " D_UNIT_KILOWATTHOUR : "");
      mqtt_publish_topic_P(1, S_RSLT_WARNING);
      hlw_mqttPresent(0);
      do_cmnd_power(1, 0);
    }
  }
#endif  // FEATURE_POWER_LIMIT
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

boolean hlw_command(char *type, uint16_t index, char *dataBuf, uint16_t data_len, int16_t payload)
{
  boolean serviced = true;
  uint8_t caltext = 0;

  if (!strcasecmp_P(type, PSTR(D_CMND_POWERLOW))) {
    if ((payload >= 0) && (payload < 3601)) {
      sysCfg.hlw_pmin = payload;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_POWERLOW "\":\"%d%s\"}"), sysCfg.hlw_pmin, (sysCfg.flag.value_units) ? " " D_UNIT_WATT : "");
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_POWERHIGH))) {
    if ((payload >= 0) && (payload < 3601)) {
      sysCfg.hlw_pmax = payload;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_POWERHIGH "\":\"%d%s\"}"), sysCfg.hlw_pmax, (sysCfg.flag.value_units) ? " " D_UNIT_WATT : "");
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_VOLTAGELOW))) {
    if ((payload >= 0) && (payload < 501)) {
      sysCfg.hlw_umin = payload;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_VOLTAGELOW "\":\"%d%s\"}"), sysCfg.hlw_umin, (sysCfg.flag.value_units) ? " " D_UNIT_VOLT : "");
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_VOLTAGEHIGH))) {
    if ((payload >= 0) && (payload < 501)) {
      sysCfg.hlw_umax = payload;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_VOLTAGEHIGH "\":\"%d%s\"}"), sysCfg.hlw_umax, (sysCfg.flag.value_units) ? " " D_UNIT_VOLT : "");
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_CURRENTLOW))) {
    if ((payload >= 0) && (payload < 16001)) {
      sysCfg.hlw_imin = payload;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_CURRENTLOW "\":\"%d%s\"}"), sysCfg.hlw_imin, (sysCfg.flag.value_units) ? " " D_UNIT_MILLIAMPERE : "");
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_CURRENTHIGH))) {
    if ((payload >= 0) && (payload < 16001)) {
      sysCfg.hlw_imax = payload;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_CURRENTHIGH "\":\"%d%s\"}"), sysCfg.hlw_imax, (sysCfg.flag.value_units) ? " " D_UNIT_MILLIAMPERE : "");
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_ENERGYRESET))) {
    if ((payload >= 1) && (payload <= 3)) {
      switch (payload) {
      case 1:
        hlw_kWhtoday = 0;
        rtcMem.hlw_kWhtoday = 0;
        sysCfg.hlw_kWhtoday = 0;
        break;
      case 2:
        sysCfg.hlw_kWhyesterday = 0;
        break;
      case 3:
        rtcMem.hlw_kWhtotal = 0;
        sysCfg.hlw_kWhtotal = rtcMem.hlw_kWhtotal;
        break;
      }
    }
    char sey[10];
    char sen[10];
    char set[10];
    dtostrfd((float)sysCfg.hlw_kWhyesterday / 100000000, sysCfg.flag.energy_resolution, sey);
    dtostrfd((float)rtcMem.hlw_kWhtoday / 100000000, sysCfg.flag.energy_resolution, sen);
    dtostrfd((float)(rtcMem.hlw_kWhtotal + (hlw_kWhtoday / 1000)) / 100000, sysCfg.flag.energy_resolution, set);
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_ENERGYRESET "\":{\"" D_TOTAL "\":%s, \"" D_YESTERDAY "\":%s, \"" D_TODAY "\":%s}}"), set, sey, sen);
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_HLWPCAL))) {
    if ((payload > 0) && (payload < 32001)) {
      sysCfg.hlw_pcal = (payload > 4000) ? payload : HLW_PREF_PULSE;  // 12530
    }
    caltext = 1;
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_HLWPSET))) {
    if ((payload > 0) && (payload < 3601) && hlw_cf_plen) {
      sysCfg.hlw_pcal = (payload * 10 * hlw_cf_plen) / HLW_PREF;
    }
    caltext = 1;
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_HLWUCAL))) {
    if ((payload > 0) && (payload < 32001)) {
      sysCfg.hlw_ucal = (payload > 999) ? payload : HLW_UREF_PULSE;  // 1950
    }
    caltext = 2;
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_HLWUSET))) {
    if ((payload > 0) && (payload < 501) && hlw_cf1u_plen) {
      sysCfg.hlw_ucal = (payload * 10 * hlw_cf1u_plen) / HLW_UREF;
    }
    caltext = 2;
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_HLWICAL))) {
    if ((payload > 0) && (payload < 32001)) {
      sysCfg.hlw_ical = (payload > 1100) ? payload : HLW_IREF_PULSE;  // 3500
    }
    caltext = 3;
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_HLWISET))) {
    if ((payload > 0) && (payload < 16001) && hlw_cf1i_plen) {
      sysCfg.hlw_ical = (payload * hlw_cf1i_plen) / HLW_IREF;
    }
    caltext = 3;
  }
#if FEATURE_POWER_LIMIT
  else if (!strcasecmp_P(type, PSTR(D_CMND_MAXPOWER))) {
    if ((payload >= 0) && (payload < 3601)) {
      sysCfg.hlw_mpl = payload;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_MAXPOWER "\":\"%d%s\"}"), sysCfg.hlw_mpl, (sysCfg.flag.value_units) ? " " D_UNIT_WATT : "");
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_MAXPOWERHOLD))) {
    if ((payload >= 0) && (payload < 3601)) {
      sysCfg.hlw_mplh = (1 == payload) ? MAX_POWER_HOLD : payload;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_MAXPOWERHOLD "\":\"%d%s\"}"), sysCfg.hlw_mplh, (sysCfg.flag.value_units) ? " " D_UNIT_SECOND : "");
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_MAXPOWERWINDOW))) {
    if ((payload >= 0) && (payload < 3601)) {
      sysCfg.hlw_mplw = (1 == payload) ? MAX_POWER_WINDOW : payload;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_MAXPOWERWINDOW "\":\"%d%s\"}"), sysCfg.hlw_mplw, (sysCfg.flag.value_units) ? " " D_UNIT_SECOND : "");
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_SAFEPOWER))) {
    if ((payload >= 0) && (payload < 3601)) {
      sysCfg.hlw_mspl = payload;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_SAFEPOWER "\":\"%d%s\"}"), sysCfg.hlw_mspl, (sysCfg.flag.value_units) ? " " D_UNIT_WATT : "");
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_SAFEPOWERHOLD))) {
    if ((payload >= 0) && (payload < 3601)) {
      sysCfg.hlw_msplh = (1 == payload) ? SAFE_POWER_HOLD : payload;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_SAFEPOWERHOLD "\":\"%d%s\"}"), sysCfg.hlw_msplh, (sysCfg.flag.value_units) ? " " D_UNIT_SECOND : "");
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_SAFEPOWERWINDOW))) {
    if ((payload >= 0) && (payload < 1440)) {
      sysCfg.hlw_msplw = (1 == payload) ? SAFE_POWER_WINDOW : payload;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_SAFEPOWERWINDOW "\":\"%d%s\"}"), sysCfg.hlw_msplw, (sysCfg.flag.value_units) ? " " D_UNIT_MINUTE : "");
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_MAXENERGY))) {
    if ((payload >= 0) && (payload < 3601)) {
      sysCfg.hlw_mkwh = payload;
      hlw_mkwh_state = 3;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_MAXENERGY "\":\"%d%s\"}"), sysCfg.hlw_mkwh, (sysCfg.flag.value_units) ? " " D_UNIT_WATTHOUR : "");
  }
  else if (!strcasecmp_P(type, PSTR(D_CMND_MAXENERGYSTART))) {
    if ((payload >= 0) && (payload < 24)) {
      sysCfg.hlw_mkwhs = payload;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_MAXENERGYSTART "\":\"%d%s\"}"), sysCfg.hlw_mkwhs, (sysCfg.flag.value_units) ? " " D_UNIT_HOUR : "");
  }
#endif  // FEATURE_POWER_LIMIT
  else {
    serviced = false;
  }
  switch (caltext) {
    case 1:
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_HLWPCAL "\":\"%d%s\"}"), sysCfg.hlw_pcal, (sysCfg.flag.value_units) ? " " D_UNIT_MICROSECOND : "");
      break;
    case 2:
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_HLWUCAL "\":\"%d%s\"}"), sysCfg.hlw_ucal, (sysCfg.flag.value_units) ? " " D_UNIT_MICROSECOND : "");
      break;
    case 3:
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_HLWICAL "\":\"%d%s\"}"), sysCfg.hlw_ical, (sysCfg.flag.value_units) ? " " D_UNIT_MICROSECOND : "");
      break;
  }
  return serviced;
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void hlw_mqttStat(byte option)
{
/* option 0 = do not show period energy usage
 * option 1 = show period energy usage
 */
  float pet;
  float ped;
  float pe;
  float pw;
  float pu;
  float pi;
  float pc;
  char spet[10];
  char sped[10];
  char spe[10];
  char spw[10];
  char spu[10];
  char spi[10];
  char spc[10];
  char sey[10];
  char speriod[20];

  hlw_readEnergy(option, pet, ped, pe, pw, pu, pi, pc);
  dtostrfd(pet, sysCfg.flag.energy_resolution, spet);
  dtostrfd(ped, sysCfg.flag.energy_resolution, sped);
  dtostrfd(pe, sysCfg.flag.wattage_resolution, spe);
  dtostrfd(pw, sysCfg.flag.wattage_resolution, spw);
  dtostrfd(pu, sysCfg.flag.voltage_resolution, spu);
  dtostrfd(pi, 3, spi);
  dtostrfd(pc, 2, spc);
  dtostrfd((float)sysCfg.hlw_kWhyesterday / 100000000, sysCfg.flag.energy_resolution, sey);
  snprintf_P(speriod, sizeof(speriod), PSTR(", \"" D_PERIOD "\":%s"), spe);
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s\"" D_TOTAL "\":%s, \"" D_YESTERDAY "\":%s, \"" D_TODAY "\":%s%s, \"" D_POWERUSAGE "\":%s, \"" D_POWERFACTOR "\":%s, \"" D_VOLTAGE "\":%s, \"" D_CURRENT "\":%s}"),
    mqtt_data, spet, sey, sped, (option) ? speriod : "", spw, spc, spu, spi);
#ifdef USE_DOMOTICZ
  if (option) {  // Only send if telemetry
    dtostrfd(pet * 1000, 1, spet);
    domoticz_sensor4((uint16_t)pw, spet);
  }
#endif  // USE_DOMOTICZ
}

void hlw_mqttPresent(byte option)
{
/* option 0 = do not show period energy usage
 * option 1 = show period energy usage
 */
// {"Time":"2017-03-04T13:37:24", "Total":0.013, "Yesterday":0.013, "Today":0.000, "Period":0, "Power":0, "Factor":0.00, "Voltage":0, "Current":0.000}
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_TIME "\":\"%s\", "), getDateTime().c_str());
  hlw_mqttStat(option);
  mqtt_publish_topic_P(2, PSTR(D_RSLT_ENERGY));
}

void hlw_mqttStatus()
{
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_STATUS D_STATUS8_POWER "\":{"));
  hlw_mqttStat(0);
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
}

#ifdef USE_WEBSERVER
const char HTTP_ENERGY_SNS[] PROGMEM =
  "<tr><th>" D_VOLTAGE "</th><td>%s " D_UNIT_VOLT "</td></tr>"
  "<tr><th>" D_CURRENT "</th><td>%s " D_UNIT_AMPERE "</td></tr>"
  "<tr><th>" D_POWERUSAGE "</th><td>%s " D_UNIT_WATT "</td></tr>"
  "<tr><th>" D_POWER_FACTOR "</th><td>%s</td></tr>"
  "<tr><th>" D_ENERGY_TODAY  "</th><td>%s " D_UNIT_KILOWATTHOUR "</td></tr>"
  "<tr><th>" D_ENERGY_YESTERDAY "</th><td>%s " D_UNIT_KILOWATTHOUR "</td></tr>"
  "<tr><th>" D_ENERGY_TOTAL "</th><td>%s " D_UNIT_KILOWATTHOUR "</td></tr>";

String hlw_webPresent()
{
  float pet;
  float ped;
  float pe;
  float pw;
  float pu;
  float pi;
  float pc;
  char spet[10];
  char sped[10];
  char spw[10];
  char spu[10];
  char spi[10];
  char spc[10];
  char sey[10];
  char sensor[400];

  hlw_readEnergy(0, pet, ped, pe, pw, pu, pi, pc);
  dtostrfi(pet, sysCfg.flag.energy_resolution, spet);
  dtostrfi(ped, sysCfg.flag.energy_resolution, sped);
  dtostrfi(pw, sysCfg.flag.wattage_resolution, spw);
  dtostrfi(pu, sysCfg.flag.voltage_resolution, spu);
  dtostrfi(pi, 3, spi);
  dtostrfi(pc, 2, spc);
  dtostrfi((float)sysCfg.hlw_kWhyesterday / 100000000, sysCfg.flag.energy_resolution, sey);
  snprintf_P(sensor, sizeof(sensor), HTTP_ENERGY_SNS, spu, spi, spw, spc, sped, sey, spet);
  return String(sensor);
}
#endif  // USE_WEBSERVER

