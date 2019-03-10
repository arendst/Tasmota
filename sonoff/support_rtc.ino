/*
  support_rtc.ino - Real Time Clock support for Sonoff-Tasmota

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

/*********************************************************************************************\
 * Sources: Time by Michael Margolis and Paul Stoffregen (https://github.com/PaulStoffregen/Time)
 *          Timezone by Jack Christensen (https://github.com/JChristensen/Timezone)
\*********************************************************************************************/

#define SECS_PER_MIN  ((uint32_t)(60UL))
#define SECS_PER_HOUR ((uint32_t)(3600UL))
#define SECS_PER_DAY  ((uint32_t)(SECS_PER_HOUR * 24UL))
#define MINS_PER_HOUR ((uint32_t)(60UL))
#define LEAP_YEAR(Y)  (((1970+Y)>0) && !((1970+Y)%4) && (((1970+Y)%100) || !((1970+Y)%400)))

extern "C" {
#include "sntp.h"
}
#include <Ticker.h>

Ticker TickerRtc;

static const uint8_t kDaysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // API starts months from 1, this array starts from 0
static const char kMonthNamesEnglish[] = "JanFebMarAprMayJunJulAugSepOctNovDec";

uint32_t utc_time = 0;
uint32_t local_time = 0;
uint32_t daylight_saving_time = 0;
uint32_t standard_time = 0;
uint32_t ntp_time = 0;
uint32_t midnight = 0;
uint32_t restart_time = 0;
int32_t  time_timezone = 0;
uint8_t  midnight_now = 0;
uint8_t  ntp_sync_minute = 0;

String GetBuildDateAndTime(void)
{
  // "2017-03-07T11:08:02" - ISO8601:2004
  char bdt[21];
  char *p;
  char mdate[] = __DATE__;  // "Mar  7 2017"
  char *smonth = mdate;
  int day = 0;
  int year = 0;

  // sscanf(mdate, "%s %d %d", bdt, &day, &year);  // Not implemented in 2.3.0 and probably too much code
  uint8_t i = 0;
  for (char *str = strtok_r(mdate, " ", &p); str && i < 3; str = strtok_r(NULL, " ", &p)) {
    switch (i++) {
    case 0:  // Month
      smonth = str;
      break;
    case 1:  // Day
      day = atoi(str);
      break;
    case 2:  // Year
      year = atoi(str);
    }
  }
  int month = (strstr(kMonthNamesEnglish, smonth) -kMonthNamesEnglish) /3 +1;
  snprintf_P(bdt, sizeof(bdt), PSTR("%d" D_YEAR_MONTH_SEPARATOR "%02d" D_MONTH_DAY_SEPARATOR "%02d" D_DATE_TIME_SEPARATOR "%s"), year, month, day, __TIME__);
  return String(bdt);  // 2017-03-07T11:08:02
}

String GetTimeZone(void)
{
  char tz[7];

  snprintf_P(tz, sizeof(tz), PSTR("%+03d:%02d"), time_timezone / 60, abs(time_timezone % 60));

  return String(tz);  // -03:45
}

String GetDuration(uint32_t time)
{
  char dt[16];

  TIME_T ut;
  BreakTime(time, ut);

  // "P128DT14H35M44S" - ISO8601:2004 - https://en.wikipedia.org/wiki/ISO_8601 Durations
//  snprintf_P(dt, sizeof(dt), PSTR("P%dDT%02dH%02dM%02dS"), ut.days, ut.hour, ut.minute, ut.second);

  // "128 14:35:44" - OpenVMS
  // "128T14:35:44" - Tasmota
  snprintf_P(dt, sizeof(dt), PSTR("%dT%02d:%02d:%02d"), ut.days, ut.hour, ut.minute, ut.second);

  return String(dt);  // 128T14:35:44
}

String GetDT(uint32_t time)
{
  // "2017-03-07T11:08:02" - ISO8601:2004

  char dt[20];
  TIME_T tmpTime;

  BreakTime(time, tmpTime);
  snprintf_P(dt, sizeof(dt), PSTR("%04d-%02d-%02dT%02d:%02d:%02d"),
    tmpTime.year +1970, tmpTime.month, tmpTime.day_of_month, tmpTime.hour, tmpTime.minute, tmpTime.second);

  return String(dt);  // 2017-03-07T11:08:02
}

/*
 * timestamps in https://en.wikipedia.org/wiki/ISO_8601 format
 *
 *  DT_UTC - current data and time in Greenwich, England (aka GMT)
 *  DT_LOCAL - current date and time taking timezone into account
 *  DT_RESTART - the date and time this device last started, in local timezone
 *
 * Format:
 *  "2017-03-07T11:08:02-07:00" - if DT_LOCAL and SetOption52 = 1
 *  "2017-03-07T11:08:02"       - otherwise
 */
String GetDateAndTime(uint8_t time_type)
{
  // "2017-03-07T11:08:02-07:00" - ISO8601:2004
  uint32_t time = local_time;

  switch (time_type) {
    case DT_ENERGY:
      time = Settings.energy_kWhtotal_time;
      break;
    case DT_UTC:
      time = utc_time;
      break;
    case DT_RESTART:
      if (restart_time == 0) {
        return "";
      }
      time = restart_time;
      break;
  }
  String dt = GetDT(time);  // 2017-03-07T11:08:02
  if (Settings.flag3.time_append_timezone && (DT_LOCAL == time_type)) {
    dt += GetTimeZone();    // 2017-03-07T11:08:02-07:00
  }
  return dt;  // 2017-03-07T11:08:02-07:00
}

String GetTime(int type)
{
  /* type 1 - Local time
   * type 2 - Daylight Savings time
   * type 3 - Standard time
   */
  char stime[25];   // Skip newline

  uint32_t time = utc_time;
  if (1 == type) time = local_time;
  if (2 == type) time = daylight_saving_time;
  if (3 == type) time = standard_time;
  snprintf_P(stime, sizeof(stime), sntp_get_real_time(time));

  return String(stime);  // Thu Nov 01 11:41:02 2018
}

uint32_t UpTime(void)
{
  if (restart_time) {
    return utc_time - restart_time;
  } else {
    return uptime;
  }
}

uint32_t MinutesUptime(void)
{
  return (UpTime() / 60);
}

String GetUptime(void)
{
  return GetDuration(UpTime());
}

uint32_t MinutesPastMidnight(void)
{
  uint32_t minutes = 0;

  if (RtcTime.valid) {
    minutes = (RtcTime.hour *60) + RtcTime.minute;
  }
  return minutes;
}

void BreakTime(uint32_t time_input, TIME_T &tm)
{
// break the given time_input into time components
// this is a more compact version of the C library localtime function
// note that year is offset from 1970 !!!

  uint8_t year;
  uint8_t month;
  uint8_t month_length;
  uint32_t time;
  unsigned long days;

  time = time_input;
  tm.second = time % 60;
  time /= 60;                // now it is minutes
  tm.minute = time % 60;
  time /= 60;                // now it is hours
  tm.hour = time % 24;
  time /= 24;                // now it is days
  tm.days = time;
  tm.day_of_week = ((time + 4) % 7) + 1;  // Sunday is day 1

  year = 0;
  days = 0;
  while((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
    year++;
  }
  tm.year = year;            // year is offset from 1970

  days -= LEAP_YEAR(year) ? 366 : 365;
  time -= days;              // now it is days in this year, starting at 0
  tm.day_of_year = time;

  days = 0;
  month = 0;
  month_length = 0;
  for (month = 0; month < 12; month++) {
    if (1 == month) { // february
      if (LEAP_YEAR(year)) {
        month_length = 29;
      } else {
        month_length = 28;
      }
    } else {
      month_length = kDaysInMonth[month];
    }

    if (time >= month_length) {
      time -= month_length;
    } else {
      break;
    }
  }
  strlcpy(tm.name_of_month, kMonthNames + (month *3), 4);
  tm.month = month + 1;      // jan is month 1
  tm.day_of_month = time + 1;         // day of month
  tm.valid = (time_input > 1451602800);  // 2016-01-01
}

uint32_t MakeTime(TIME_T &tm)
{
// assemble time elements into time_t
// note year argument is offset from 1970

  int i;
  uint32_t seconds;

  // seconds from 1970 till 1 jan 00:00:00 of the given year
  seconds = tm.year * (SECS_PER_DAY * 365);
  for (i = 0; i < tm.year; i++) {
    if (LEAP_YEAR(i)) {
      seconds +=  SECS_PER_DAY;   // add extra days for leap years
    }
  }

  // add days for this year, months start from 1
  for (i = 1; i < tm.month; i++) {
    if ((2 == i) && LEAP_YEAR(tm.year)) {
      seconds += SECS_PER_DAY * 29;
    } else {
      seconds += SECS_PER_DAY * kDaysInMonth[i-1];  // monthDay array starts from 0
    }
  }
  seconds+= (tm.day_of_month - 1) * SECS_PER_DAY;
  seconds+= tm.hour * SECS_PER_HOUR;
  seconds+= tm.minute * SECS_PER_MIN;
  seconds+= tm.second;
  return seconds;
}

uint32_t RuleToTime(TimeRule r, int yr)
{
  TIME_T tm;
  uint32_t t;
  uint8_t m;
  uint8_t w;                // temp copies of r.month and r.week

  m = r.month;
  w = r.week;
  if (0 == w) {             // Last week = 0
    if (++m > 12) {         // for "Last", go to the next month
      m = 1;
      yr++;
    }
    w = 1;                  // and treat as first week of next month, subtract 7 days later
  }

  tm.hour = r.hour;
  tm.minute = 0;
  tm.second = 0;
  tm.day_of_month = 1;
  tm.month = m;
  tm.year = yr - 1970;
  t = MakeTime(tm);         // First day of the month, or first day of next month for "Last" rules
  BreakTime(t, tm);
  t += (7 * (w - 1) + (r.dow - tm.day_of_week + 7) % 7) * SECS_PER_DAY;
  if (0 == r.week) {
    t -= 7 * SECS_PER_DAY;  // back up a week if this is a "Last" rule
  }
  return t;
}

uint32_t UtcTime(void)
{
  return utc_time;
}

uint32_t LocalTime(void)
{
  return local_time;
}

uint32_t Midnight(void)
{
  return midnight;
}

bool MidnightNow(void)
{
  bool mnflg = midnight_now;
  if (mnflg) midnight_now = 0;
  return mnflg;
}

void RtcSecond(void)
{
  TIME_T tmpTime;

  if ((ntp_sync_minute > 59) && (RtcTime.minute > 2)) ntp_sync_minute = 1;                 // If sync prepare for a new cycle
  uint8_t offset = (uptime < 30) ? RtcTime.second : (((ESP.getChipId() & 0xF) * 3) + 3) ;  // First try ASAP to sync. If fails try once every 60 seconds based on chip id
  if (!global_state.wifi_down && (offset == RtcTime.second) && ((RtcTime.year < 2016) || (ntp_sync_minute == RtcTime.minute) || ntp_force_sync)) {
    ntp_time = sntp_get_current_timestamp();
    if (ntp_time > 1451602800) {  // Fix NTP bug in core 2.4.1/SDK 2.2.1 (returns Thu Jan 01 08:00:10 1970 after power on)
      ntp_force_sync = false;
      utc_time = ntp_time;
      ntp_sync_minute = 60;  // Sync so block further requests
      if (restart_time == 0) {
        restart_time = utc_time - uptime;  // save first ntp time as restart time
      }
      BreakTime(utc_time, tmpTime);
      RtcTime.year = tmpTime.year + 1970;
      daylight_saving_time = RuleToTime(Settings.tflag[1], RtcTime.year);
      standard_time = RuleToTime(Settings.tflag[0], RtcTime.year);
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION "(" D_UTC_TIME ") %s, (" D_DST_TIME ") %s, (" D_STD_TIME ") %s"), GetTime(0).c_str(), GetTime(2).c_str(), GetTime(3).c_str());
      if (local_time < 1451602800) {  // 2016-01-01
        rules_flag.time_init = 1;
      } else {
        rules_flag.time_set = 1;
      }
    } else {
      ntp_sync_minute++;  // Try again in next minute
    }
  }
  utc_time++;
  local_time = utc_time;
  if (local_time > 1451602800) {  // 2016-01-01
    int16_t timezone_minutes = Settings.timezone_minutes;
    if (Settings.timezone < 0) { timezone_minutes *= -1; }
    time_timezone = (Settings.timezone * SECS_PER_HOUR) + (timezone_minutes * SECS_PER_MIN);
    if (99 == Settings.timezone) {
      int32_t dstoffset = Settings.toffset[1] * SECS_PER_MIN;
      int32_t stdoffset = Settings.toffset[0] * SECS_PER_MIN;
      if (Settings.tflag[1].hemis) {
        // Southern hemisphere
        if ((utc_time >= (standard_time - dstoffset)) && (utc_time < (daylight_saving_time - stdoffset))) {
          time_timezone = stdoffset;  // Standard Time
        } else {
          time_timezone = dstoffset;  // Daylight Saving Time
        }
      } else {
        // Northern hemisphere
        if ((utc_time >= (daylight_saving_time - stdoffset)) && (utc_time < (standard_time - dstoffset))) {
          time_timezone = dstoffset;  // Daylight Saving Time
        } else {
          time_timezone = stdoffset;  // Standard Time
        }
      }
    }
    local_time += time_timezone;
    time_timezone /= 60;
    if (!Settings.energy_kWhtotal_time) { Settings.energy_kWhtotal_time = local_time; }
  }
  BreakTime(local_time, RtcTime);

  if (RtcTime.valid) {
    if (!midnight) {
      midnight = local_time - (RtcTime.hour * 3600) - (RtcTime.minute * 60) - RtcTime.second;
    }
    if (!RtcTime.hour && !RtcTime.minute && !RtcTime.second) {
      midnight = local_time;
      midnight_now = 1;
    }
  }

  RtcTime.year += 1970;
}

void RtcInit(void)
{
  sntp_setservername(0, Settings.ntp_server[0]);
  sntp_setservername(1, Settings.ntp_server[1]);
  sntp_setservername(2, Settings.ntp_server[2]);
  sntp_stop();
  sntp_set_timezone(0);      // UTC time
  sntp_init();
  utc_time = 0;
  BreakTime(utc_time, RtcTime);
  TickerRtc.attach(1, RtcSecond);
}
