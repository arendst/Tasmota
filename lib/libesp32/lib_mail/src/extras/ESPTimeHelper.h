/*
 * ESP8266/ESP32 Internet Time Helper Arduino Library v 1.0.2
 *
 * The MIT License (MIT)
 * Copyright (c) 2021 K. Suwatchai (Mobizt)
 * 
 * 
 * Permission is hereby granted, free of charge, to any person returning a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef ESPTimeHelper_H
#define ESPTimeHelper_H

#include <time.h>
#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include "SDK_Version_Common.h"
#endif

class ESPTimeHelper
{
public:
  ESPTimeHelper();

  /** Set the system time from the NTP server
   * 
   * @param gmtOffset The GMT time offset in hour.
   * @param daylightOffset The Daylight time offset in hour.
   * @return boolean The status indicates the success of operation.
   * 
   * @note This requires internet connection
  */
  bool setClock(float gmtOffset, float daylightOffset);

  /** Set system time with provided timestamp
   * 
   * @param ts timestamp in seconds from midnight Jan 1, 1970.
   * @return error number, 0 for success.
  */
  int setTimestamp(time_t ts);

  /** Provide the Unix time
   * 
   * @return uint32_t The value of current Unix time.
  */
  uint32_t getUnixTime();

  /** Provide the timestamp from the year, month, date, hour, minute, 
   * and second provided.
   * 
   * @param year The year.
   * @param mon The month from 1 to 12.
   * @param date The dates.
   * @param hour The hours.
   * @param mins The minutes.
   * @param sec The seconds.
   * @return time_t The value of timestamp.
  */
  time_t getTimestamp(int year, int mon, int date, int hour, int mins, int sec);

  /** Provide the current year.
   * 
   * @return int The value of current year.
  */
  int getYear();

  /** Provide the current month.
   * 
   * @return int The value of current month.
  */
  int getMonth();

  /** Provide the current date.
   * 
   * @return int The value of current date.
  */
  int getDay();

  /** Provide the current day of week.
   * 
   * @return int The value of current day of week.
   * 
   * @note 1 for sunday and 7 for saturday.
  */
  int getDayOfWeek();

  /** Provide the current day of week in String.
   * 
   * @return String The value of day of week.
  */
  String getDayOfWeekString();

  /** Provide the current hour.
   * 
   * @return int The value of current hour (0 to 23).
  */
  int getHour();

  /** Provide the current minute.
   * 
   * @return int The value of current minute.
  */
  int getMin();

  /** Provide the current second.
   * 
   * @return int The value of current second.
  */
  int getSec();

  /** Provide the total days of current year.
   * 
   * @return int The value of total days of current year.
  */
  int getNumberOfDayThisYear();

  /** Provide the total days of from January 1, 1970 to specific date.
   * 
   * @param year The year from 1970.
   * @param mon The month from 1 to 12.
   * @param day The dates.
   * @return int The value of total days.
  */
  int getTotalDays(int year, int month, int day);

  /** Provide the day of week from specific date.
   * 
   * @param year The year from 1970.
   * @param mon The month from 1 to 12.
   * @param day The dates.
   * @return int the value of day of week.
   * @note 1 for sunday and 7 for saturday
  */
  int dayofWeek(int year, int month, int day);

  /** Provide the second of current hour.
   * 
   * @return int The value of current second.
  */
  int getCurrentSecond();

  /** Provide the current timestamp.
   * 
   * @return uint64_t The value of current timestamp.
  */
  uint64_t getCurrentTimestamp();

  /** Provide the date and time from second counted from January 1, 1970.
   * 
   * @param sec The seconds from January 1, 1970 00.00.
   * @return tm The tm structured data.
   * 
   * @note The returned structured data tm has the members e.g. 
   * tm_year (from 1900), tm_mon (from 0 to 11), tm_mday, tm_hour, 
   * tm_min and tm_sec.
  */
  struct tm getTimeFromSec(int seconds);

  /** Provide the current date time string that valid for Email.
   * 
   * @return String The current date time string.
  */
  String getDateTimeString();

  time_t now;
  uint64_t msec_time_diff = 0;
  struct tm timeinfo;
  float TZ = 0.0;
  uint8_t DST_MN = 0;
  bool clockReady = false;

private:
  int totalDays(int y, int m, int d);
  char *intStr(int value);
  void setSysTime();
  const char *dow[7] = {"sunday", "monday", "tuesday", "wednesday", "thurseday", "friday", "saturday"};
  const char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  const char *sdow[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
};

#endif //ESPTimeHelper_H
