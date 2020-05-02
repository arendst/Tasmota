/*
 * ESP32 Internet Time Helper Arduino Library v 1.0.1
 *
 * The MIT License (MIT)
 * Copyright (c) 2019 K. Suwatchai (Mobizt)
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

#ifndef ESP32TimeHelper_CPP
#define ESP32TimeHelper_CPP

#ifdef ESP32

#include "ESP32TimeHelper.h"

ESP32TimeHelper::ESP32TimeHelper()
{
}
uint32_t ESP32TimeHelper::getUnixTime()
{
    uint32_t utime = (msec_time_diff + millis()) / 1000;
    return utime;
}

time_t ESP32TimeHelper::getTimestamp(int year, int mon, int date, int hour, int mins, int sec)
{
    struct tm timeinfo;
    timeinfo.tm_year = year - 1900;
    timeinfo.tm_mon = mon - 1;
    timeinfo.tm_mday = date;
    timeinfo.tm_hour = hour;
    timeinfo.tm_min = mins;
    timeinfo.tm_sec = sec;
    time_t ts = mktime(&timeinfo);
    return ts;
}

bool ESP32TimeHelper::setClock(float gmtOffset, float daylightOffset)
{
    TZ = gmtOffset;
    DST_MN = daylightOffset;
    configTime((TZ)*3600, (DST_MN)*60, "pool.ntp.org", "time.nist.gov", NULL);

    now = time(nullptr);
    int cnt = 0;
    while (now < 8 * 3600 * 2 && cnt < 20)
    {
        delay(50);
        now = time(nullptr);
        cnt++;
    }

    uint64_t tmp = now;
    tmp = tmp * 1000;
    msec_time_diff = tmp - millis();

    getLocalTime(&timeinfo);

    clockReady = now > 8 * 3600 * 2;
    return clockReady;
}

int ESP32TimeHelper::getYear()
{
    getLocalTime(&timeinfo);
    return timeinfo.tm_year + 1900;
}
int ESP32TimeHelper::getMonth()
{
    getLocalTime(&timeinfo);
    return timeinfo.tm_mon + 1;
}
int ESP32TimeHelper::getDay()
{
    getLocalTime(&timeinfo);
    return timeinfo.tm_mday;
}

int ESP32TimeHelper::getDayOfWeek()
{
    getLocalTime(&timeinfo);
    return timeinfo.tm_wday;
}
String ESP32TimeHelper::getDayOfWeekString()
{
    getLocalTime(&timeinfo);
    return dow[timeinfo.tm_wday];
}

int ESP32TimeHelper::getHour()
{
    getLocalTime(&timeinfo);
    return timeinfo.tm_hour;
}

int ESP32TimeHelper::getMin()
{
    getLocalTime(&timeinfo);
    return timeinfo.tm_min;
}
int ESP32TimeHelper::getSec()
{
    getLocalTime(&timeinfo);
    return timeinfo.tm_sec;
}
int ESP32TimeHelper::getNumberOfDayThisYear()
{
    getLocalTime(&timeinfo);
    return timeinfo.tm_yday + 1;
}

int ESP32TimeHelper::totalDays(int y, int m, int d)
{
    static char daytab[2][13] =
        {
            {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
            {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};
    int daystotal = d;
    for (int year = 1; year <= y; year++)
    {
        int max_month = (year < y ? 12 : m - 1);
        int leap = (year % 4 == 0);
        if (year % 100 == 0 && year % 400 != 0)
            leap = 0;
        for (int month = 1; month <= max_month; month++)
        {
            daystotal += daytab[leap][month];
        }
    }
    return daystotal;
}
int ESP32TimeHelper::getTotalDays(int year, int month, int day)
{
    return totalDays(year, month, day) - totalDays(1970, 1, 1);
}

int ESP32TimeHelper::dayofWeek(int year, int month, int day) /* 1 <= m <= 12,  y > 1752 (in the U.K.) */
{
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    year -= month < 3;
    return (year + year / 4 - year / 100 + year / 400 + t[month - 1] + day) % 7;
}

int ESP32TimeHelper::getCurrentSecond()
{
    return (timeinfo.tm_hour * 3600) + (timeinfo.tm_min * 60) + timeinfo.tm_sec;
}
uint64_t ESP32TimeHelper::getCurrentTimestamp()
{
    return now;
}
void ESP32TimeHelper::getTimeFromSec(int secCount, int &yrs, int &months, int &days, int &hr, int &min, int &sec)
{
    int _yrs = secCount / (365 * 24 * 3600);
    secCount = secCount - _yrs * (365 * 24 * 3600);
    yrs = _yrs;
    int _months = secCount / (30* 24 * 3600);
    secCount = secCount - _months * (30 * 24 * 3600);
    months = _months;
    int _days = secCount / (24 * 3600);
    secCount = secCount - _days * (24 * 3600);
    days = _days;
    int _hr = secCount / 3600;
    secCount = secCount - _hr * 3600;
    hr = _hr;
    int _min = secCount / 60;
    secCount = secCount - _min * 60;
    min = _min;
    sec = secCount;
}

#endif //ESP32

#endif //ESP32TimeHelper_CPP