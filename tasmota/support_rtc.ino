/*
  support_rtc.ino - Real Time Clock support for Tasmota

  Copyright (C) 2021  Theo Arends

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

const uint32_t SECS_PER_MIN = 60UL;
const uint32_t SECS_PER_HOUR = 3600UL;
const uint32_t SECS_PER_DAY = SECS_PER_HOUR * 24UL;
const uint32_t MINS_PER_HOUR = 60UL;

#define LEAP_YEAR(Y)  (((1970+Y)>0) && !((1970+Y)%4) && (((1970+Y)%100) || !((1970+Y)%400)))

#include <Ticker.h>

Ticker TickerRtc;

static const uint8_t kDaysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // API starts months from 1, this array starts from 0
static const char kMonthNamesEnglish[] = "JanFebMarAprMayJunJulAugSepOctNovDec";

struct RTC {
  uint32_t utc_time = 0;
  uint32_t local_time = 0;
  uint32_t daylight_saving_time = 0;
  uint32_t standard_time = 0;
  uint32_t midnight = 0;
  uint32_t restart_time = 0;
  uint32_t millis = 0;
//  uint32_t last_sync = 0;
  int32_t time_timezone = 0;
  bool time_synced = false;
  bool midnight_now = false;
  bool user_time_entry = false;               // Override NTP by user setting
} Rtc;

uint32_t UtcTime(void)
{
  return Rtc.utc_time;
}

uint32_t LocalTime(void)
{
  return Rtc.local_time;
}

uint32_t Midnight(void)
{
  return Rtc.midnight;
}

bool MidnightNow(void)
{
  if (Rtc.midnight_now) {
    Rtc.midnight_now = false;
    return true;
  }
  return false;
}

bool IsDst(void)
{
  if (Rtc.time_timezone == Settings.toffset[1]) {
    return true;
  }
  return false;
}

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
  for (char *str = strtok_r(mdate, " ", &p); str && i < 3; str = strtok_r(nullptr, " ", &p)) {
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

String GetMinuteTime(uint32_t minutes)
{
  char tm[6];
  snprintf_P(tm, sizeof(tm), PSTR("%02d:%02d"), minutes / 60, minutes % 60);

  return String(tm);  // 03:45
}

String GetTimeZone(void)
{
  char tz[7];
  snprintf_P(tz, sizeof(tz), PSTR("%+03d:%02d"), Rtc.time_timezone / 60, abs(Rtc.time_timezone % 60));

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
 *  DT_UTC     - current data and time in Greenwich, England (aka GMT)
 *  DT_LOCAL   - current date and time taking timezone into account
 *  DT_RESTART - the date and time this device last started, in local timezone
 *
 * Format:
 *  "2017-03-07T11:08:02-07:00" - if DT_LOCAL and SetOption52 = 1
 *  "2017-03-07T11:08:02"       - otherwise
 */
String GetDateAndTime(uint8_t time_type)
{
  // "2017-03-07T11:08:02-07:00" - ISO8601:2004
  uint32_t time = Rtc.local_time;

  switch (time_type) {
    case DT_UTC:
      time = Rtc.utc_time;
      break;
//    case DT_LOCALNOTZ:  // Is default anyway but allows for not appendig timezone
//      time = Rtc.local_time;
//      break;
    case DT_DST:
      time = Rtc.daylight_saving_time;
      break;
    case DT_STD:
      time = Rtc.standard_time;
      break;
    case DT_RESTART:
      if (Rtc.restart_time == 0) {
        return "";
      }
      time = Rtc.restart_time;
      break;
    case DT_ENERGY:
      time = Settings.energy_kWhtotal_time;
      break;
    case DT_BOOTCOUNT:
      time = Settings.bootcount_reset_time;
      break;
  }
  String dt = GetDT(time);  // 2017-03-07T11:08:02

  if (DT_LOCAL_MILLIS == time_type) {
    char ms[10];
    snprintf_P(ms, sizeof(ms), PSTR(".%03d"), RtcMillis());
    dt += ms;
    time_type = DT_LOCAL;
  }

  if (Settings.flag3.time_append_timezone && (DT_LOCAL == time_type)) {  // SetOption52 - Append timezone to JSON time
    dt += GetTimeZone();    // 2017-03-07T11:08:02-07:00
  }
  return dt;  // 2017-03-07T11:08:02-07:00
}

uint32_t UpTime(void)
{
  if (Rtc.restart_time) {
    return Rtc.utc_time - Rtc.restart_time;
  } else {
    return TasmotaGlobal.uptime;
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

uint32_t RtcMillis(void) {
  return (millis() - Rtc.millis) % 1000;
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
  tm.valid = (time_input > START_VALID_TIME);  // 2016-01-01
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

void RtcSecond(void)
{
  static uint32_t last_sync = 0;
  static bool mutex = false;

  if (mutex) { return; }

  if (Rtc.time_synced) {
    mutex = true;

    Rtc.time_synced = false;
    last_sync = Rtc.utc_time;

    if (Rtc.restart_time == 0) {
      Rtc.restart_time = Rtc.utc_time - TasmotaGlobal.uptime;  // save first synced time as restart time
    }

    TIME_T tmpTime;
    BreakTime(Rtc.utc_time, tmpTime);
    RtcTime.year = tmpTime.year + 1970;
    Rtc.daylight_saving_time = RuleToTime(Settings.tflag[1], RtcTime.year);
    Rtc.standard_time = RuleToTime(Settings.tflag[0], RtcTime.year);

    // Do not use AddLog_P( here (interrupt routine) if syslog or mqttlog is enabled. UDP/TCP will force exception 9
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("RTC: " D_UTC_TIME " %s, " D_DST_TIME " %s, " D_STD_TIME " %s"),
      GetDateAndTime(DT_UTC).c_str(), GetDateAndTime(DT_DST).c_str(), GetDateAndTime(DT_STD).c_str());

    if (Rtc.local_time < START_VALID_TIME) {  // 2016-01-01
      TasmotaGlobal.rules_flag.time_init = 1;
    } else {
      TasmotaGlobal.rules_flag.time_set = 1;
    }
  } else {
    Rtc.utc_time++;  // Increment every second
  }
  Rtc.millis = millis();

  if ((Rtc.utc_time > (2 * 60 * 60)) && (last_sync < Rtc.utc_time - (2 * 60 * 60))) {  // Every two hours a warning
    // Do not use AddLog_P( here (interrupt routine) if syslog or mqttlog is enabled. UDP/TCP will force exception 9
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("RTC: Not synced"));
    last_sync = Rtc.utc_time;
  }

  Rtc.local_time = Rtc.utc_time;
  if (Rtc.local_time > START_VALID_TIME) {  // 2016-01-01
    int16_t timezone_minutes = Settings.timezone_minutes;
    if (Settings.timezone < 0) { timezone_minutes *= -1; }
    Rtc.time_timezone = (Settings.timezone * SECS_PER_HOUR) + (timezone_minutes * SECS_PER_MIN);
    if (99 == Settings.timezone) {
      int32_t dstoffset = Settings.toffset[1] * SECS_PER_MIN;
      int32_t stdoffset = Settings.toffset[0] * SECS_PER_MIN;
      if (Settings.tflag[1].hemis) {
        // Southern hemisphere
        if ((Rtc.utc_time >= (Rtc.standard_time - dstoffset)) && (Rtc.utc_time < (Rtc.daylight_saving_time - stdoffset))) {
          Rtc.time_timezone = stdoffset;  // Standard Time
        } else {
          Rtc.time_timezone = dstoffset;  // Daylight Saving Time
        }
      } else {
        // Northern hemisphere
        if ((Rtc.utc_time >= (Rtc.daylight_saving_time - stdoffset)) && (Rtc.utc_time < (Rtc.standard_time - dstoffset))) {
          Rtc.time_timezone = dstoffset;  // Daylight Saving Time
        } else {
          Rtc.time_timezone = stdoffset;  // Standard Time
        }
      }
    }
    Rtc.local_time += Rtc.time_timezone;
    Rtc.time_timezone /= 60;
    if (!Settings.energy_kWhtotal_time) {
      Settings.energy_kWhtotal_time = Rtc.local_time;
    }
    if (Settings.bootcount_reset_time < START_VALID_TIME) {
      Settings.bootcount_reset_time = Rtc.local_time;
    }
  }

  BreakTime(Rtc.local_time, RtcTime);
  if (RtcTime.valid) {
    if (!Rtc.midnight) {
      Rtc.midnight = Rtc.local_time - (RtcTime.hour * 3600) - (RtcTime.minute * 60) - RtcTime.second;
    }
    if (!RtcTime.hour && !RtcTime.minute && !RtcTime.second) {
      Rtc.midnight = Rtc.local_time;
      Rtc.midnight_now = true;
    }
  }

  RtcTime.year += 1970;

  mutex = false;
}

void RtcSync(void) {
  Rtc.time_synced = true;
  RtcSecond();
//  AddLog_P(LOG_LEVEL_DEBUG, PSTR("RTC: Synced"));
}

void RtcSetTime(uint32_t epoch) {
  if (epoch < START_VALID_TIME) {  // 2016-01-01
    Rtc.user_time_entry = false;
    TasmotaGlobal.ntp_force_sync = true;
  } else {
    Rtc.user_time_entry = true;
    Rtc.utc_time = epoch -1;    // Will be corrected by RtcSecond
  }
}

void RtcInit(void) {
  Rtc.utc_time = 0;
  BreakTime(Rtc.utc_time, RtcTime);
  TickerRtc.attach(1, RtcSecond);
}

void RtcPreInit(void) {
  Rtc.millis = millis();
}
