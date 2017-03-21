/*
Copyright (c) 2017 Theo Arends.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
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

byte hlw_SELflag, hlw_cf_timer, hlw_cf1_timer, hlw_fifth_second, hlw_startup;
unsigned long hlw_cf_plen, hlw_cf_last;
unsigned long hlw_cf1_plen, hlw_cf1_last, hlw_cf1_ptot, hlw_cf1_pcnt, hlw_cf1u_plen, hlw_cf1i_plen;
unsigned long hlw_Ecntr, hlw_EDcntr, hlw_kWhtoday;
uint32_t hlw_lasttime;

unsigned long hlw_cf1u_pcntmax, hlw_cf1i_pcntmax;

Ticker tickerHLW;

#ifndef USE_WS2812_DMA  // Collides with Neopixelbus but solves exception
void hlw_cf_interrupt() ICACHE_RAM_ATTR;
void hlw_cf1_interrupt() ICACHE_RAM_ATTR;
#endif  // USE_WS2812_DMA

void hlw_cf_interrupt()  // Service Power
{
  hlw_cf_plen = micros() - hlw_cf_last;
  hlw_cf_last = micros();
  if (hlw_cf_plen > 4000000) hlw_cf_plen = 0;  // Just powered on
  hlw_cf_timer = 15;  // Support down to 4W which takes about 3 seconds
  hlw_EDcntr++;
  hlw_Ecntr++;
}

void hlw_cf1_interrupt()  // Service Voltage and Current
{
  hlw_cf1_plen = micros() - hlw_cf1_last;
  hlw_cf1_last = micros();
  if ((hlw_cf1_timer > 2) && (hlw_cf1_timer < 8)) {  // Allow for 300 mSec set-up time and measure for up to 1 second
    hlw_cf1_ptot += hlw_cf1_plen;
    hlw_cf1_pcnt++;
    if (hlw_cf1_pcnt == 10) hlw_cf1_timer = 8;  // We need up to ten samples within 1 second (low current could take up to 0.3 second)
  }
}

void hlw_200mS()
{
  unsigned long hlw_len, hlw_temp;

  hlw_fifth_second++;
  if (hlw_fifth_second == 5) {
    hlw_fifth_second = 0;

    if (hlw_EDcntr) {
      hlw_len = 1000000 / hlw_EDcntr;
      hlw_EDcntr = 0;
      hlw_temp = (HLW_PREF * sysCfg.hlw_pcal) / hlw_len;
      hlw_kWhtoday += (hlw_temp * 100) / 36;
      rtcMem.hlw_kWhtoday = hlw_kWhtoday;
    }
    if (rtcTime.Valid) {
      if (rtc_loctime() == rtc_midnight()) {
        sysCfg.hlw_kWhyesterday = hlw_kWhtoday;
        hlw_kWhtoday = 0;
        rtcMem.hlw_kWhtoday = hlw_kWhtoday;
        hlw_mkwh_state = 3;
      }
      if ((rtcTime.Hour == sysCfg.hlw_mkwhs) && (hlw_mkwh_state == 3)) {
        hlw_mkwh_state = 0;
      }
      if (hlw_startup && (rtcTime.DayOfYear == sysCfg.hlw_kWhdoy)) {
        hlw_kWhtoday = sysCfg.hlw_kWhtoday;
        rtcMem.hlw_kWhtoday = hlw_kWhtoday;
        hlw_startup = 0;
      }
    }
  }

  if (hlw_cf_timer) {
    hlw_cf_timer--;
    if (!hlw_cf_timer) hlw_cf_plen = 0;  // No load for over three seconds
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
}

boolean hlw_readEnergy(byte option, float &ed, uint16_t &e, uint16_t &w, uint16_t &u, float &i, float &c)
{
  unsigned long hlw_len, hlw_temp, hlw_w, hlw_u, hlw_i;
  int hlw_period, hlw_interval;

//char log[LOGSZ];
//snprintf_P(log, sizeof(log), PSTR("HLW: CF %d, CF1U %d (%d), CF1I %d (%d)"), hlw_cf_plen, hlw_cf1u_plen, hlw_cf1u_pcntmax, hlw_cf1i_plen, hlw_cf1i_pcntmax);
//addLog(LOG_LEVEL_DEBUG, log);

  if (hlw_kWhtoday) {
    ed = (float)hlw_kWhtoday / 100000000;
  } else {
    ed = 0;
  }

  if (option) {
    if (!hlw_lasttime) {
      hlw_period = sysCfg.tele_period;
    } else {
      hlw_period = rtc_loctime() - hlw_lasttime;
    }
    hlw_lasttime = rtc_loctime();
    hlw_interval = 3600 / hlw_period;
    if (hlw_Ecntr) {
      hlw_len = hlw_period * 1000000 / hlw_Ecntr;
      hlw_Ecntr = 0;
      hlw_temp = ((HLW_PREF * sysCfg.hlw_pcal) / hlw_len) / hlw_interval;
      e = hlw_temp / 10;
    } else {
      e = 0;
    }
  }

  if (hlw_cf_plen) {
    hlw_w = (HLW_PREF * sysCfg.hlw_pcal) / hlw_cf_plen;
    w = hlw_w / 10;
  } else {
    w = 0;
  }
  if (hlw_cf1u_plen && (w || (power &1))) {
    hlw_u = (HLW_UREF * sysCfg.hlw_ucal) / hlw_cf1u_plen;
    u = hlw_u / 10;
  } else {
    u = 0;
  }
  if (hlw_cf1i_plen && w) {
    hlw_i = (HLW_IREF * sysCfg.hlw_ical) / hlw_cf1i_plen;
    i = (float)hlw_i / 1000;
  } else {
    i = 0;
  }
  if (hlw_i && hlw_u && hlw_w && w) {
    hlw_temp = (hlw_w * 100) / ((hlw_u * hlw_i) / 1000);
    if (hlw_temp > 100) {
      hlw_temp = 100;
    }
    c = (float)hlw_temp / 100;
  } else {
    c = 0;
  }

  return true;
}

void hlw_init()
{
  if (!sysCfg.hlw_pcal || (sysCfg.hlw_pcal == 4975)) {
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
  hlw_cf_timer = 0;
  hlw_cf1_timer = 0;
  tickerHLW.attach_ms(200, hlw_200mS);
}

/********************************************************************************************/

boolean hlw_margin(byte type, uint16_t margin, uint16_t value, byte &flag, byte &saveflag)
{
  byte change;

  if (!margin) return false;
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
  char log[LOGSZ], svalue[200];  // was MESSZ
  float ped, pi, pc;
  uint16_t uped, piv, pe, pw, pu;
  byte flag, jsonflg;

  if (power_steady_cntr) {
    power_steady_cntr--;
    return;
  }

  hlw_readEnergy(0, ped, pe, pw, pu, pi, pc);
  if (power && (sysCfg.hlw_pmin || sysCfg.hlw_pmax || sysCfg.hlw_umin || sysCfg.hlw_umax || sysCfg.hlw_imin || sysCfg.hlw_imax)) {
    piv = (uint16_t)(pi * 1000);

//    snprintf_P(log, sizeof(log), PSTR("HLW: W %d, U %d, I %d"), pw, pu, piv);
//    addLog(LOG_LEVEL_DEBUG, log);

    snprintf_P(svalue, sizeof(svalue), PSTR("{"));
    jsonflg = 0;
    if (hlw_margin(0, sysCfg.hlw_pmin, pw, flag, hlw_pminflg)) {
      snprintf_P(svalue, sizeof(svalue), PSTR("%s%s\"PowerLow\":\"%s\""), svalue, (jsonflg)?", ":"", (flag) ? MQTT_STATUS_ON : MQTT_STATUS_OFF);
      jsonflg = 1;
    }
    if (hlw_margin(1, sysCfg.hlw_pmax, pw, flag, hlw_pmaxflg)) {
      snprintf_P(svalue, sizeof(svalue), PSTR("%s%s\"PowerHigh\":\"%s\""), svalue, (jsonflg)?", ":"", (flag) ? MQTT_STATUS_ON : MQTT_STATUS_OFF);
      jsonflg = 1;
    }
    if (hlw_margin(0, sysCfg.hlw_umin, pu, flag, hlw_uminflg)) {
      snprintf_P(svalue, sizeof(svalue), PSTR("%s%s\"VoltageLow\":\"%s\""), svalue, (jsonflg)?", ":"", (flag) ? MQTT_STATUS_ON : MQTT_STATUS_OFF);
      jsonflg = 1;
    }
    if (hlw_margin(1, sysCfg.hlw_umax, pw, flag, hlw_umaxflg)) {
      snprintf_P(svalue, sizeof(svalue), PSTR("%s%s\"VoltageHigh\":\"%s\""), svalue, (jsonflg)?", ":"", (flag) ? MQTT_STATUS_ON : MQTT_STATUS_OFF);
      jsonflg = 1;
    }
    if (hlw_margin(0, sysCfg.hlw_imin, piv, flag, hlw_iminflg)) {
      snprintf_P(svalue, sizeof(svalue), PSTR("%s%s\"CurrentLow\":\"%s\""), svalue, (jsonflg)?", ":"", (flag) ? MQTT_STATUS_ON : MQTT_STATUS_OFF);
      jsonflg = 1;
    }
    if (hlw_margin(1, sysCfg.hlw_imax, piv, flag, hlw_imaxflg)) {
      snprintf_P(svalue, sizeof(svalue), PSTR("%s%s\"CurrentHigh\":\"%s\""), svalue, (jsonflg)?", ":"", (flag) ? MQTT_STATUS_ON : MQTT_STATUS_OFF);
      jsonflg = 1;
    }
    if (jsonflg) {
      snprintf_P(svalue, sizeof(svalue), PSTR("%s}"), svalue);
      mqtt_publish_topic_P(1, PSTR("MARGINS"), svalue);
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
          snprintf_P(svalue, sizeof(svalue), PSTR("{\"MaxPowerReached\":\"%d%s\"}"), pw, (sysCfg.value_units) ? " W" : "");
          mqtt_publish_topic_P(0, PSTR("WARNING"), svalue);
          do_cmnd_power(1, 0);
          if (!hlw_mplr_counter) hlw_mplr_counter = MAX_POWER_RETRY +1;
          hlw_mplw_counter = sysCfg.hlw_mplw;
        }
      }
    }
    else if (power && (pw <= sysCfg.hlw_mpl)) {
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
            snprintf_P(svalue, sizeof(svalue), PSTR("{\"PowerMonitor\":\"%s\"}"), MQTT_STATUS_ON);
            mqtt_publish_topic_P(0, PSTR("RESULT"), svalue);
            do_cmnd_power(1, 1);
          } else {
            snprintf_P(svalue, sizeof(svalue), PSTR("{\"MaxPowerReachedRetry\":\"%s\"}"), MQTT_STATUS_OFF);
            mqtt_publish_topic_P(0, PSTR("WARNING"), svalue);
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
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"EnergyMonitor\":\"%s\"}"), MQTT_STATUS_ON);
      mqtt_publish_topic_P(0, PSTR("RESULT"), svalue);
      do_cmnd_power(1, 1);
    }
    else if ((hlw_mkwh_state == 1) && (uped >= sysCfg.hlw_mkwh)) {
      hlw_mkwh_state = 2;
      dtostrf(ped, 1, 3, svalue);
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"MaxEnergyReached\":\"%s%s\"}"), svalue, (sysCfg.value_units) ? " kWh" : "");
      mqtt_publish_topic_P(0, PSTR("WARNING"), svalue);
      do_cmnd_power(1, 0);
    }
  }
#endif  // FEATURE_POWER_LIMIT
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

boolean hlw_command(char *type, uint16_t index, char *dataBuf, uint16_t data_len, int16_t payload, char *svalue, uint16_t ssvalue)
{
  boolean serviced = true;

  if (!strcmp(type,"POWERLOW")) {
    if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
      sysCfg.hlw_pmin = payload;
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"PowerLow\":\"%d%s\"}"), sysCfg.hlw_pmin, (sysCfg.value_units) ? " W" : "");
  }
  else if (!strcmp(type,"POWERHIGH")) {
    if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
      sysCfg.hlw_pmax = payload;
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"PowerHigh\":\"%d%s\"}"), sysCfg.hlw_pmax, (sysCfg.value_units) ? " W" : "");
  }
  else if (!strcmp(type,"VOLTAGELOW")) {
    if ((data_len > 0) && (payload >= 0) && (payload < 501)) {
      sysCfg.hlw_umin = payload;
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"VoltageLow\":\"%d%s\"}"), sysCfg.hlw_umin, (sysCfg.value_units) ? " V" : "");
  }
  else if (!strcmp(type,"VOLTAGEHIGH")) {
    if ((data_len > 0) && (payload >= 0) && (payload < 501)) {
      sysCfg.hlw_umax = payload;
    }
    snprintf_P(svalue, ssvalue, PSTR("[\"VoltageHigh\":\"%d%s\"}"), sysCfg.hlw_umax, (sysCfg.value_units) ? " V" : "");
  }
  else if (!strcmp(type,"CURRENTLOW")) {
    if ((data_len > 0) && (payload >= 0) && (payload < 16001)) {
      sysCfg.hlw_imin = payload;
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"CurrentLow\":\"%d%s\"}"), sysCfg.hlw_imin, (sysCfg.value_units) ? " mA" : "");
  }
  else if (!strcmp(type,"CURRENTHIGH")) {
    if ((data_len > 0) && (payload >= 0) && (payload < 16001)) {
      sysCfg.hlw_imax = payload;
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"CurrentHigh\":\"%d%s\"}"), sysCfg.hlw_imax, (sysCfg.value_units) ? " mA" : "");
  }
  else if (!strcmp(type,"HLWPCAL")) {
    if ((data_len > 0) && (payload > 0) && (payload < 32001)) {
      sysCfg.hlw_pcal = (payload > 9999) ? payload : HLW_PREF_PULSE;  // 12530
    }
    snprintf_P(svalue, ssvalue, PSTR("(\"HlwPcal\":\"%d%s\"}"), sysCfg.hlw_pcal, (sysCfg.value_units) ? " uS" : "");
  }
  else if (!strcmp(type,"HLWUCAL")) {
    if ((data_len > 0) && (payload > 0) && (payload < 32001)) {
      sysCfg.hlw_ucal = (payload > 999) ? payload : HLW_UREF_PULSE;  // 1950
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"HlwUcal\":\"%d%s\"}"), sysCfg.hlw_ucal, (sysCfg.value_units) ? " uS" : "");
  }
  else if (!strcmp(type,"HLWICAL")) {
    if ((data_len > 0) && (payload > 0) && (payload < 32001)) {
      sysCfg.hlw_ical = (payload > 2499) ? payload : HLW_IREF_PULSE;  // 3500
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"HlwIcal\":\"%d%s\"}"), sysCfg.hlw_ical, (sysCfg.value_units) ? " uS" : "");
  }
#if FEATURE_POWER_LIMIT
  else if (!strcmp(type,"MAXPOWER")) {
    if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
      sysCfg.hlw_mpl = payload;
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"MaxPower\":\"%d%s\"}"), sysCfg.hlw_mpl, (sysCfg.value_units) ? " W" : "");
  }
  else if (!strcmp(type,"MAXPOWERHOLD")) {
    if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
      sysCfg.hlw_mplh = (payload == 1) ? MAX_POWER_HOLD : payload;
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"MaxPowerHold\":\"%d%s\"}"), sysCfg.hlw_mplh, (sysCfg.value_units) ? " Sec" : "");
  }
  else if (!strcmp(type,"MAXPOWERWINDOW")) {
    if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
      sysCfg.hlw_mplw = (payload == 1) ? MAX_POWER_WINDOW : payload;
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"MaxPowerWindow\":\"%d%s\"}"), sysCfg.hlw_mplw, (sysCfg.value_units) ? " Sec" : "");
  }
  else if (!strcmp(type,"SAFEPOWER")) {
    if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
      sysCfg.hlw_mspl = payload;
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"SafePower\":\"%d%s\"}"), sysCfg.hlw_mspl, (sysCfg.value_units) ? " W" : "");
  }
  else if (!strcmp(type,"SAFEPOWERHOLD")) {
    if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
      sysCfg.hlw_msplh = (payload == 1) ? SAFE_POWER_HOLD : payload;
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"SafePowerHold\":\"%d%s\"}"), sysCfg.hlw_msplh, (sysCfg.value_units) ? " Sec" : "");
  }
  else if (!strcmp(type,"SAFEPOWERWINDOW")) {
    if ((data_len > 0) && (payload >= 0) && (payload < 1440)) {
      sysCfg.hlw_msplw = (payload == 1) ? SAFE_POWER_WINDOW : payload;
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"SafePowerWindow\":\"%d%s\"}"), sysCfg.hlw_msplw, (sysCfg.value_units) ? " Min" : "");
  }
  else if (!strcmp(type,"MAXENERGY")) {
    if ((data_len > 0) && (payload >= 0) && (payload < 3601)) {
      sysCfg.hlw_mkwh = payload;
      hlw_mkwh_state = 3;
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"MaxEnergy\":\"%d%s\"}"), sysCfg.hlw_mkwh, (sysCfg.value_units) ? " Wh" : "");
  }
  else if (!strcmp(type,"MAXENERGYSTART")) {
    if ((data_len > 0) && (payload >= 0) && (payload < 24)) {
      sysCfg.hlw_mkwhs = payload;
    }
    snprintf_P(svalue, ssvalue, PSTR("{\"MaxEnergyStart\":\"%d%s\"}"), sysCfg.hlw_mkwhs, (sysCfg.value_units) ? " Hr" : "");
  }
#endif  // FEATURE_POWER_LIMIT
  else {
    serviced = false;
  }
  return serviced;
}

void hlw_commands(char *svalue, uint16_t ssvalue)
{
  snprintf_P(svalue, ssvalue, PSTR("{\"Commands5\":\"PowerLow, PowerHigh, VoltageLow, VoltageHigh, CurrentLow, CurrentHigh, HlwPcal, HlwUcal, HlwIcal%s\"}"),
    (FEATURE_POWER_LIMIT)?", SafePower, SafePowerHold, SafePowerWindow, MaxPower, MaxPowerHold, MaxPowerWindow, MaxEnergy, MaxEnergyStart":"");
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void hlw_mqttStat(byte option, char* svalue, uint16_t ssvalue)
{
  char stemp0[10], stemp1[10], stemp2[10], stemp3[10], speriod[20];
  float ped, pi, pc;
  uint16_t pe, pw, pu;

  hlw_readEnergy(option, ped, pe, pw, pu, pi, pc);
  dtostrf((float)sysCfg.hlw_kWhyesterday / 100000000, 1, ENERGY_RESOLUTION &7, stemp0);
  dtostrf(ped, 1, ENERGY_RESOLUTION &7, stemp1);
  dtostrf(pc, 1, 2, stemp2);
  dtostrf(pi, 1, 3, stemp3);
  snprintf_P(speriod, sizeof(speriod), PSTR(", \"Period\":%d"), pe);
  snprintf_P(svalue, ssvalue, PSTR("%s\"Yesterday\":%s, \"Today\":%s%s, \"Power\":%d, \"Factor\":%s, \"Voltage\":%d, \"Current\":%s}"),
    svalue, stemp0, stemp1, (option) ? speriod : "", pw, stemp2, pu, stemp3);
#ifdef USE_DOMOTICZ
  dtostrf(ped * 1000, 1, 1, stemp1);
  domoticz_sensor4(pw, stemp1);
#endif  // USE_DOMOTICZ
}

void hlw_mqttPresent()
{
// {"Time":"2017-03-04T13:37:24", "Yesterday":0.013, "Today":0.000, "Period":0, "Power":0, "Factor":0.00, "Voltage":0, "Current":0.000}
  char svalue[200];  // was MESSZ

  snprintf_P(svalue, sizeof(svalue), PSTR("{\"Time\":\"%s\", "), getDateTime().c_str());
  hlw_mqttStat(1, svalue, sizeof(svalue));

//  snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/ENERGY"), PUB_PREFIX2, sysCfg.mqtt_topic);
//  mqtt_publish(stopic, svalue);

  mqtt_publish_topic_P(1, PSTR("ENERGY"), svalue);
}

void hlw_mqttStatus(char* svalue, uint16_t ssvalue)
{
  snprintf_P(svalue, ssvalue, PSTR("{\"StatusPWR\":{"));
  hlw_mqttStat(0, svalue, ssvalue);
  snprintf_P(svalue, ssvalue, PSTR("%s}"), svalue);
}

#ifdef USE_WEBSERVER
const char HTTP_ENERGY_SNS[] PROGMEM =
  "<tr><th>Voltage</th><td>%d V</td></tr>"
  "<tr><th>Current</th><td>%s A</td></tr>"
  "<tr><th>Power</th><td>%d W</td></tr>"
  "<tr><th>Power Factor</th><td>%s</td></tr>"
  "<tr><th>Energy Today</th><td>%s kWh</td></tr>"
  "<tr><th>Energy Yesterday</th><td>%s kWh</td></tr>";

String hlw_webPresent()
{
  String page = "";
  char stemp[10], stemp2[10], stemp3[10], stemp4[10], sensor[300];
  float ped, pi, pc;
  uint16_t pe, pw, pu;

  hlw_readEnergy(0, ped, pe, pw, pu, pi, pc);

  dtostrf(pi, 1, 3, stemp);
  dtostrf(pc, 1, 2, stemp2);
  dtostrf(ped, 1, ENERGY_RESOLUTION &7, stemp3);
  dtostrf((float)sysCfg.hlw_kWhyesterday / 100000000, 1, ENERGY_RESOLUTION &7, stemp4);
  snprintf_P(sensor, sizeof(sensor), HTTP_ENERGY_SNS, pu, stemp, pw, stemp2, stemp3, stemp4);
  page += sensor;
  return page;
}
#endif  // USE_WEBSERVER

