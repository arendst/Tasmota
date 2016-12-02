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

byte hlw_SELflag, hlw_cf1_timer, hlw_seconds, hlw_startup;
unsigned long hlw_cf_plen, hlw_cf_last;
unsigned long hlw_cf1_plen, hlw_cf1_last;
unsigned long hlw_cf1u_plen, hlw_cf1i_plen;
unsigned long hlw_Ecntr, hlw_EDcntr, hlw_kWhtoday;
uint32_t hlw_lasttime;

Ticker tickerHLW;

void hlw_cf_interrupt() ICACHE_RAM_ATTR;
void hlw_cf1_interrupt() ICACHE_RAM_ATTR;

void hlw_cf_interrupt()
{
  hlw_cf_plen = micros() - hlw_cf_last;
  hlw_cf_last = micros();
  hlw_EDcntr++;
  hlw_Ecntr++;
}

void hlw_cf1_interrupt()
{
  hlw_cf1_plen = micros() - hlw_cf1_last;
  hlw_cf1_last = micros();
  if ((hlw_cf1_timer > 2) && (hlw_cf1_timer < 7)) {
    if (hlw_SELflag) {
      hlw_cf1i_plen = hlw_cf1_plen;
    } else {
      hlw_cf1u_plen = hlw_cf1_plen;
    }
  }
}

void hlw_200mS()
{
  unsigned long hlw_len, hlw_temp;

  hlw_seconds++;
  if (hlw_seconds == 5) {
    hlw_seconds = 0;

    if (hlw_EDcntr) {
      hlw_len = 1000000 / hlw_EDcntr;
      hlw_EDcntr = 0;
      hlw_temp = (HLW_PREF * sysCfg.hlw_pcal) / hlw_len;
      hlw_kWhtoday += (hlw_temp * 100) / 36;
    } else {
      hlw_cf_plen = 0;
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

  hlw_cf1_timer--;
  if (!hlw_cf1_timer) {
    hlw_cf1_timer = 10;
    hlw_SELflag = (hlw_SELflag) ? 0 : 1;
    digitalWrite(HLW_SEL, hlw_SELflag);
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

//  char log[LOGSZ];
//  snprintf_P(log, sizeof(log), PSTR("HLW: CF %d, CF1U %d, CF1I %d"), hlw_cf_plen, hlw_cf1u_plen, hlw_cf1i_plen);
//  addLog(LOG_LEVEL_DEBUG, log);

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
  if (hlw_cf1u_plen && w) {
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
  hlw_seconds = 0;
  hlw_cf1_timer = 1;
  tickerHLW.attach_ms(200, hlw_200mS);
}

#endif  // USE_POWERMONITOR

