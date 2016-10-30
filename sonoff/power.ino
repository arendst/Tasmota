/*
These routines provide support to my various ESP8266 based projects.

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

#define HLW_PREF_PULSE       4975    // 4975us = 201Hz = 1000W
#define HLW_UREF_PULSE       1666    // 1666us = 600Hz = 220V
#define HLW_IREF_PULSE       1666    // 1666us = 600Hz = 4.545A

byte hlw_SELflag;
byte hlw_SELcounter;
byte hlw_counter;
unsigned long hlw_cf_plen = 0;
unsigned long hlw_cf_last = 0;
unsigned long hlw_cf1_plen = 0;
unsigned long hlw_cf1_last = 0;
unsigned long hlw_cf1u_plen = 0;
unsigned long hlw_cf1i_plen = 0;
unsigned long hlw_Ecntr;
unsigned long hlw_EDcntr;
unsigned long hlw_kWhtoday;
uint32_t hlw_lasttime = 0;

uint16_t hlw_pmin = 0;
uint16_t hlw_pmax = 0;
uint16_t hlw_umin = 0;
uint16_t hlw_umax = 0;
uint16_t hlw_imin = 0;
uint16_t hlw_imax = 0;

byte hlw_pminflg = 0;
byte hlw_pmaxflg = 0;
byte hlw_uminflg = 0;
byte hlw_umaxflg = 0;
byte hlw_iminflg = 0;
byte hlw_imaxflg = 0;

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
  hlw_SELcounter++;
  if (hlw_SELcounter == 8) {
    hlw_SELcounter = 0;
    hlw_SELflag ^= 1;
  }
  if (hlw_SELflag) {
    hlw_cf1u_plen = hlw_cf1_plen;
  } else {
    hlw_cf1i_plen = hlw_cf1_plen;
  }
}

void hlw_second()
{
  unsigned long hlw_len;
  unsigned long hlw_temp;

/*
  if (hlw_cf1u_plen && ((micros() - hlw_cf1_last) < 100000) && hlw_cf_plen) {
    hlw_temp = (HLW_PREF * sysCfg.hlw_pcal) / hlw_cf_plen;
    hlw_kWhtoday += (hlw_temp * 100) / 36;
  }
*/
  if (hlw_EDcntr) {
    hlw_len = 1000000 / hlw_EDcntr;
    hlw_EDcntr = 0;
    hlw_temp = (HLW_PREF * sysCfg.hlw_pcal) / hlw_len;
    hlw_kWhtoday += (hlw_temp * 100) / 36;
  }

  if (rtc_loctime() == rtc_midnight()) {
    sysCfg.hlw_esave = hlw_kWhtoday;
    hlw_kWhtoday = 0;
  }
}

boolean hlw_readEnergy(byte option, float &ed, uint16_t &e, uint16_t &w, uint16_t &u, float &i, float &c)
{
  unsigned long hlw_len;
  unsigned long hlw_temp;
  unsigned long hlw_w;
  unsigned long hlw_u;
  unsigned long hlw_i;
  int hlw_period;
  int hlw_interval;

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
  if (hlw_cf1u_plen && ((micros() - hlw_cf1_last) < 100000)) {
    hlw_u = (HLW_UREF * sysCfg.hlw_ucal) / hlw_cf1u_plen;
    u = hlw_u / 10;
  } else {
    u = 0;
  }
  if (hlw_cf1i_plen && u) {
    hlw_i = (HLW_IREF * sysCfg.hlw_ical) / hlw_cf1i_plen;
    i = (float)hlw_i / 1000;
  } else {
    i = 0;
  }
  if (hlw_cf_plen && u) {
    hlw_w = (HLW_PREF * sysCfg.hlw_pcal) / hlw_cf_plen;
    w = hlw_w / 10;
  } else {
    w = 0;
  }
  if (hlw_i && hlw_u && hlw_w && u) {
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

boolean hlw_margin(byte bottom, uint16_t margin, uint16_t value, uint16_t &lastvalue, byte &lastflag, byte &flag)
{
  byte change;

  if (!margin) return false;
  if (!lastvalue) lastvalue = margin;
  if (bottom) {
    if ((value < margin) && (lastvalue < margin)) flag = 1;
    if ((value >= margin) && (lastvalue >= margin)) flag = 0;
  } else {
    if ((value > margin) && (lastvalue > margin)) flag = 1;
    if ((value <= margin) && (lastvalue <= margin)) flag = 0;
  }
  lastvalue = value;
  change = lastflag;
  if (flag && !lastflag) lastflag = 1;
  else if (!flag && lastflag) lastflag = 0;
  return (change != lastflag);
}

boolean hlw_pless(uint16_t margin, uint16_t value, byte &flag)
{
  return hlw_margin(1, margin, value, hlw_pmin, hlw_pminflg, flag);
}
boolean hlw_pmore(uint16_t margin, uint16_t value, byte &flag)
{
  return hlw_margin(0, margin, value, hlw_pmax, hlw_pmaxflg, flag);
}
boolean hlw_uless(uint16_t margin, uint16_t value, byte &flag)
{
  return hlw_margin(1, margin, value, hlw_umin, hlw_uminflg, flag);
}
boolean hlw_umore(uint16_t margin, uint16_t value, byte &flag)
{
  return hlw_margin(0, margin, value, hlw_umax, hlw_umaxflg, flag);
}
boolean hlw_iless(uint16_t margin, uint16_t value, byte &flag)
{
  return hlw_margin(1, margin, value, hlw_imin, hlw_iminflg, flag);
}
boolean hlw_imore(uint16_t margin, uint16_t value, byte &flag)
{
  return hlw_margin(0, margin, value, hlw_imax, hlw_imaxflg, flag);
}

void hlw_init()
{
  if (!sysCfg.hlw_pcal) {
    sysCfg.hlw_pcal = HLW_PREF_PULSE;
    sysCfg.hlw_ucal = HLW_UREF_PULSE;
    sysCfg.hlw_ical = HLW_IREF_PULSE;
  }
  hlw_SELcounter = 0;
  hlw_SELflag = 1;  // Voltage;
  
  hlw_Ecntr = 0;
  hlw_EDcntr = 0;
  hlw_kWhtoday = 0;
  
  pinMode(HLW_CF, INPUT_PULLUP);
  attachInterrupt(HLW_CF, hlw_cf_interrupt, FALLING);
  pinMode(HLW_SEL, OUTPUT);
  digitalWrite(HLW_SEL, hlw_SELflag);
  pinMode(HLW_CF1, INPUT_PULLUP);
  attachInterrupt(HLW_CF1, hlw_cf1_interrupt, FALLING);
}

#endif  // USE_POWERMONITOR

