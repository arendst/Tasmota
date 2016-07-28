#define NTP_SERVER1 "pool.ntp.org"
#define NTP_SERVER2 "nl.pool.ntp.org"
#define NTP_SERVER3 "0.nl.pool.ntp.org"

extern "C" {
#include "sntp.h"
}

#define LEAP_YEAR(Y) ( ((1970+Y)>0) && !((1970+Y)%4) && ( ((1970+Y)%100) || !((1970+Y)%400) ) )

static const uint8_t monthDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // API starts months from 1, this array starts from 0

static const char monthNames[37] = { "JanFebMrtAprMayJunJulAugSepOctNovDec" };

uint32_t myrtc = 0;
uint8_t ntpsync = 0;

void convertTime()
{
  // given myrtc as time in Linux format break into time components
  // this is a more compact version of the C library localtime function
  // note that internally the year is offset from 1970 - compensated at the end

  uint8_t year;
  uint8_t month, monthLength;
  uint32_t time;
  unsigned long days;

  time = (uint32_t) myrtc;
  rtcTime.Second = time % 60;
  time /= 60; // now it is minutes
  rtcTime.Minute = time % 60;
  time /= 60; // now it is hours
  rtcTime.Hour = time % 24;
  time /= 24; // now it is days
  rtcTime.Wday = ((time + 4) % 7) + 1;  // Sunday is day 1

  year = 0;
  days = 0;
  while ((unsigned) (days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
    year++;
  }
  rtcTime.Year = year; // year is offset from 1970

  days -= LEAP_YEAR(year) ? 366 : 365;
  time -= days; // now it is days in this year, starting at 0

  days = 0;
  month = 0;
  monthLength = 0;
  for (month = 0; month < 12; month++) {
    if (month == 1) { // february
      if (LEAP_YEAR(year)) {
        monthLength = 29;
      }
      else {
        monthLength = 28;
      }
    }
    else {
      monthLength = monthDays[month];
    }

    if (time >= monthLength) {
      time -= monthLength;
    }
    else {
      break;
    }
  }
  strncpy(rtcTime.MonthName, monthNames +(month *3), 3);
  rtcTime.Month = month + 1;  // jan is month 1
  rtcTime.Day = time + 1;     // day of month
  rtcTime.Year = rtcTime.Year + 1970;
}

void rtc_second()
{
  char log[LOGSZ];
  
  // NTP Sync every hour at x:0:10
  if (rtcTime.Minute == 0) {
    if ((rtcTime.Second >= 10) && !ntpsync) {
      myrtc = sntp_get_current_timestamp();
      ntpsync = (myrtc) ? 1 : 0;
      snprintf_P(log, LOGSZ, PSTR("RTC: sntp %d, %s"), myrtc, sntp_get_real_time(myrtc));
      addLog(LOG_LEVEL_DEBUG, log);
    }
    if (rtcTime.Second == 40) ntpsync = 0;
  }
  myrtc++;
  convertTime();
}

void rtc_timezone(uint8_t timezone)
{
  sntp_stop();
//  if (true == sntp_set_timezone(sysCfg.timezone))
  sntp_set_timezone(timezone);
  sntp_init();
  myrtc = sntp_get_current_timestamp();
  ntpsync = 0;
}

void rtc_init(uint8_t timezone)
{
  sntp_setservername(0, NTP_SERVER1);
  sntp_setservername(1, NTP_SERVER2);
//  sntp_setservername(2, NTP_SERVER3);
  sntp_stop();
//  if (true == sntp_set_timezone(sysCfg.timezone))
  sntp_set_timezone(timezone);
  sntp_init();
  myrtc = 0;
}
