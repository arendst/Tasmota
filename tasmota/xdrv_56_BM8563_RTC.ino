/*
  xdrv_56_BM8563_RTC.ino - BM8563 RTC

  Copyright (C) 2021  Stephan Hadinger and Theo Arends

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

#ifdef USE_I2C
#ifdef USE_BM8563
/*********************************************************************************************\
 * BM8563 - Real Time Clock
 *
 * I2C Address: 0x51 (Fixed in library as BM8563_ADRESS)
\*********************************************************************************************/

#define XDRV_56             56
#define XI2C_59             59  // See I2CDEVICES.md

#include "BM8563.h"

struct {
  BM8563 Rtc;
  bool rtc_ready = false;
  bool ntp_time_ok = false;
} bm8563_driver;

uint32_t BM8563GetUtc(void) {
  RTC_TimeTypeDef RTCtime;
  // 1. read has errors ???
  bm8563_driver.Rtc.GetTime(&RTCtime);
//   core2_globs.Rtc.GetTime(&RTCtime);
  RTC_DateTypeDef RTCdate;
  bm8563_driver.Rtc.GetDate(&RTCdate);
  TIME_T tm;
  tm.second =  RTCtime.Seconds;
  tm.minute = RTCtime.Minutes;
  tm.hour = RTCtime.Hours;
  tm.day_of_week = RTCdate.WeekDay;
  tm.day_of_month = RTCdate.Date;
  tm.month = RTCdate.Month;
  tm.year = RTCdate.Year - 1970;
  return MakeTime(tm);
}

void BM8563SetUtc(uint32_t epoch_time) {
  TIME_T tm;
  BreakTime(epoch_time, tm);
  RTC_TimeTypeDef RTCtime;
  RTCtime.Hours = tm.hour;
  RTCtime.Minutes = tm.minute;
  RTCtime.Seconds = tm.second;
  bm8563_driver.Rtc.SetTime(&RTCtime);
  RTC_DateTypeDef RTCdate;
  RTCdate.WeekDay = tm.day_of_week;
  RTCdate.Month = tm.month;
  RTCdate.Date = tm.day_of_month;
  RTCdate.Year = tm.year + 1970;
  bm8563_driver.Rtc.SetDate(&RTCdate);
}

void InitTimeFromRTC(void) {
  if (Rtc.utc_time < START_VALID_TIME) {
    // set rtc from chip
    Rtc.utc_time = BM8563GetUtc();

    TIME_T tmpTime;
    TasmotaGlobal.ntp_force_sync = true;  // Force to sync with ntp
    BreakTime(Rtc.utc_time, tmpTime);
    Rtc.daylight_saving_time = RuleToTime(Settings->tflag[1], RtcTime.year);
    Rtc.standard_time = RuleToTime(Settings->tflag[0], RtcTime.year);
    AddLog(LOG_LEVEL_INFO, PSTR("I2C: Set time from BM8563 to RTC (" D_UTC_TIME ") %s, (" D_DST_TIME ") %s, (" D_STD_TIME ") %s"),
      GetDateAndTime(DT_UTC).c_str(), GetDateAndTime(DT_DST).c_str(), GetDateAndTime(DT_STD).c_str());
    if (Rtc.local_time < START_VALID_TIME) {  // 2016-01-01
      TasmotaGlobal.rules_flag.time_init = 1;
    } else {
      TasmotaGlobal.rules_flag.time_set = 1;
    }
  }
}

void BM8563Detect(void) {
#ifdef ESP32
  if (!I2cSetDevice(BM8563_ADRESS, 0)) {
    if (!I2cSetDevice(BM8563_ADRESS, 1)) { return; }        // check on bus 1
    bm8563_driver.Rtc.setBus(1);                            // switch to bus 1
    I2cSetActiveFound(BM8563_ADRESS, "BM8563", 1);
  } else {
    I2cSetActiveFound(BM8563_ADRESS, "BM8563", 0);
  }
#else
  if (!I2cSetDevice(BM8563_ADRESS)) { return; }
  I2cSetActiveFound(BM8563_ADRESS, "BM8563");
#endif

  bm8563_driver.Rtc.begin();
  bm8563_driver.rtc_ready = true;

  InitTimeFromRTC();
}

void BM8563EverySecond(void) {
  if (!bm8563_driver.ntp_time_ok && (Rtc.utc_time > START_VALID_TIME) && abs((int32_t)Rtc.utc_time - (int32_t)BM8563GetUtc()) > 3) {
    BM8563SetUtc(Rtc.utc_time);
    AddLog(LOG_LEVEL_INFO, PSTR("I2C: Write time to BM8563 from NTP (" D_UTC_TIME ") %s, (" D_DST_TIME ") %s, (" D_STD_TIME ") %s"),
      GetDateAndTime(DT_UTC).c_str(), GetDateAndTime(DT_DST).c_str(), GetDateAndTime(DT_STD).c_str());
    bm8563_driver.ntp_time_ok = true;
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv56(uint8_t function) {
  if (!I2cEnabled(XI2C_59)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    BM8563Detect();
  }
  else if (bm8563_driver.rtc_ready) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        BM8563EverySecond();
        break;
    }
  }
  return result;
}

#endif  // USE_BM8563
#endif  // USE_I2C
