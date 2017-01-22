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

#define HLW_PREF            10000    // 1000.0W
#define HLW_UREF             2200    // 220.0V
#define HLW_IREF             4545    // 4.545A

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
    }
    if (rtc_loctime() == rtc_midnight()) {
      sysCfg.hlw_kWhyesterday = hlw_kWhtoday;
      hlw_kWhtoday = 0;
    }
    if (hlw_startup && rtcTime.Valid && (rtcTime.DayOfYear == sysCfg.hlw_kWhdoy)) {
      hlw_kWhtoday = sysCfg.hlw_kWhtoday;
      hlw_startup = 0;
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
  hlw_kWhtoday = 0;

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

void hlw_margin_chk()
{
  char log[LOGSZ], stopic[TOPSZ], svalue[MESSZ];
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

    snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/TELEMETRY"), PUB_PREFIX2, sysCfg.mqtt_topic);
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
      mqtt_publish(stopic, svalue);
    }
  }

#ifdef FEATURE_POWER_LIMIT
  snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/RESULT"), PUB_PREFIX, sysCfg.mqtt_topic);
  // Max Power
  if (sysCfg.hlw_mpl) {
    if (pw > sysCfg.hlw_mpl) {
      if (!hlw_mplh_counter) {
        hlw_mplh_counter = sysCfg.hlw_mplh;
      } else {
        hlw_mplh_counter--;
        if (!hlw_mplh_counter) {
          snprintf_P(svalue, sizeof(svalue), PSTR("{\"MaxPowerReached\":\"%d%s\"}"), pw, (sysCfg.value_units) ? " W" : "");
          mqtt_publish(stopic, svalue);
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
            snprintf_P(svalue, sizeof(stopic), PSTR("{\"PowerMonitor\":\"%s\"}"), MQTT_STATUS_ON);
            mqtt_publish(stopic, svalue);
            do_cmnd_power(1, 1);
          } else {
            snprintf_P(svalue, sizeof(stopic), PSTR("{\"MaxPowerReachedRetry\":\"%s\"}"), MQTT_STATUS_OFF);
            mqtt_publish(stopic, svalue);
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
      snprintf_P(svalue, sizeof(stopic), PSTR("{\"EnergyMonitor\":\"%s\"}"), MQTT_STATUS_ON);
      mqtt_publish(stopic, svalue);
      do_cmnd_power(1, 1);
    }
    else if ((hlw_mkwh_state == 1) && (uped >= sysCfg.hlw_mkwh)) {
      hlw_mkwh_state = 2;
      dtostrf(ped, 1, 3, svalue);
      snprintf_P(svalue, sizeof(svalue), PSTR("{\"MaxEnergyReached\":\"%s%s\"}"), svalue, (sysCfg.value_units) ? " kWh" : "");
      mqtt_publish(stopic, svalue);
      do_cmnd_power(1, 0);
    }
  }
#endif  // FEATURE_POWER_LIMIT
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void hlw_mqttPresent()
{
  char stopic[TOPSZ], svalue[MESSZ], stime[21], stemp0[10], stemp1[10], stemp2[10], stemp3[10];
  float ped, pi, pc;
  uint16_t pe, pw, pu;

  snprintf_P(stime, sizeof(stime), PSTR("%04d-%02d-%02dT%02d:%02d:%02d"),
    rtcTime.Year, rtcTime.Month, rtcTime.Day, rtcTime.Hour, rtcTime.Minute, rtcTime.Second);
  hlw_readEnergy(1, ped, pe, pw, pu, pi, pc);
  dtostrf((float)sysCfg.hlw_kWhyesterday / 100000000, 1, 3, stemp0);
  dtostrf(ped, 1, 3, stemp1);
  dtostrf(pc, 1, 2, stemp2);
  dtostrf(pi, 1, 3, stemp3);
  snprintf_P(stopic, sizeof(stopic), PSTR("%s/%s/TELEMETRY"), PUB_PREFIX2, sysCfg.mqtt_topic);
  snprintf_P(svalue, sizeof(svalue), PSTR("{\"Time\":\"%s\", \"Energy\":{\"Yesterday\":\"%s\", \"Today\":\"%s\", \"Period\":%d, \"Power\":%d, \"Factor\":\"%s\", \"Voltage\":%d, \"Current\":\"%s\"}}"),
    stime, stemp0, stemp1, pe, pw, stemp2, pu, stemp3);
  mqtt_publish(stopic, svalue);
}

#ifdef USE_WEBSERVER
String hlw_webPresent()
{
  char stemp[10];
  float ped, pi, pc;
  uint16_t pe, pw, pu;
  String page = "";

  hlw_readEnergy(0, ped, pe, pw, pu, pi, pc);
  page += F("<tr><td>Voltage: </td><td>"); page += String(pu); page += F(" V</td></tr>");
  dtostrf(pi, 1, 3, stemp);
  page += F("<tr><td>Current: </td><td>"); page += stemp; page += F(" A</td></tr>");
  page += F("<tr><td>Power: </td><td>"); page += String(pw); page += F(" W</td></tr>");
  dtostrf(pc, 1, 2, stemp);
  page += F("<tr><td>Power Factor: </td><td>"); page += stemp; page += F("</td></tr>");
  dtostrf(ped, 1, 3, stemp);
  page += F("<tr><td>Energy Today: </td><td>"); page += stemp; page += F(" kWh</td></tr>");
  dtostrf((float)sysCfg.hlw_kWhyesterday / 100000000, 1, 3, stemp);
  page += F("<tr><td>Energy Yesterday: </td><td>"); page += stemp; page += F(" kWh</td></tr>");
  return page;
}
#endif  // USE_WEBSERVER

