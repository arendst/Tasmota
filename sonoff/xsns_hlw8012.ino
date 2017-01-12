/*
Copyright (c) 2016 Theo Arends.  All rights reserved.

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

#ifdef USE_POWERMONITOR
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

void hlw_cf_interrupt() ICACHE_RAM_ATTR;
void hlw_cf1_interrupt() ICACHE_RAM_ATTR;

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
    digitalWrite(HLW_SEL, hlw_SELflag);

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

  pinMode(HLW_SEL, OUTPUT);
  digitalWrite(HLW_SEL, hlw_SELflag);
  pinMode(HLW_CF1, INPUT_PULLUP);
  attachInterrupt(HLW_CF1, hlw_cf1_interrupt, FALLING);
  pinMode(HLW_CF, INPUT_PULLUP);
  attachInterrupt(HLW_CF, hlw_cf_interrupt, FALLING);

  hlw_startup = 1;
  hlw_lasttime = 0;
  hlw_fifth_second = 0;
  hlw_cf_timer = 0;
  hlw_cf1_timer = 0;
  tickerHLW.attach_ms(200, hlw_200mS);
}
#endif  // USE_POWERMONITOR

