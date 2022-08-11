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

#ifndef ESPTimeHelper_CPP
#define ESPTimeHelper_CPP

#include "ESPTimeHelper.h"

ESPTimeHelper::ESPTimeHelper()
{
}

uint32_t ESPTimeHelper::getUnixTime()
{
    uint32_t utime = (msec_time_diff + millis()) / 1000;
    return utime;
}

int ESPTimeHelper::setTimestamp(time_t ts)
{
    struct timeval tm = {ts, 0};//sec, us
    return settimeofday((const timeval *)&tm, 0);
}

time_t ESPTimeHelper::getTimestamp(int year, int mon, int date, int hour, int mins, int sec)
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

bool ESPTimeHelper::setClock(float gmtOffset, float daylightOffset)
{
    TZ = gmtOffset;
    DST_MN = daylightOffset;
    configTime((TZ)*3600, (DST_MN)*60, "pool.ntp.org", "time.nist.gov");

    now = time(nullptr);
    uint8_t attempts = 0;
    while (now < 1577836800)
    {
        now = time(nullptr);
        attempts++;
        if (attempts > 200 || now > 1577836800)
            break;
        delay(100);
    }

    uint64_t tmp = now;
    tmp = tmp * 1000;
    msec_time_diff = tmp - millis();

#if defined(ESP32)
    getLocalTime(&timeinfo);
#elif defined(ESP8266)
    gmtime_r(&now, &timeinfo);
#endif

    clockReady = now > 8 * 3600 * 2;
    return clockReady;
}

int ESPTimeHelper::getYear()
{
    setSysTime();
    return timeinfo.tm_year + 1900;
}
int ESPTimeHelper::getMonth()
{
    setSysTime();
    return timeinfo.tm_mon + 1;
}
int ESPTimeHelper::getDay()
{
    setSysTime();
    return timeinfo.tm_mday;
}

int ESPTimeHelper::getDayOfWeek()
{
    setSysTime();
    return timeinfo.tm_wday;
}
String ESPTimeHelper::getDayOfWeekString()
{
    setSysTime();
    return dow[timeinfo.tm_wday];
}

int ESPTimeHelper::getHour()
{
    setSysTime();
    return timeinfo.tm_hour;
}

int ESPTimeHelper::getMin()
{
    setSysTime();
    return timeinfo.tm_min;
}
int ESPTimeHelper::getSec()
{
    setSysTime();
    return timeinfo.tm_sec;
}
int ESPTimeHelper::getNumberOfDayThisYear()
{
    setSysTime();
    return timeinfo.tm_yday + 1;
}

int ESPTimeHelper::totalDays(int y, int m, int d)
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
int ESPTimeHelper::getTotalDays(int year, int month, int day)
{
    return totalDays(year, month, day) - totalDays(1970, 1, 1);
}

int ESPTimeHelper::dayofWeek(int year, int month, int day) /* 1 <= m <= 12,  y > 1752 (in the U.K.) */
{
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    year -= month < 3;
    return (year + year / 4 - year / 100 + year / 400 + t[month - 1] + day) % 7;
}

int ESPTimeHelper::getCurrentSecond()
{
    return (timeinfo.tm_hour * 3600) + (timeinfo.tm_min * 60) + timeinfo.tm_sec;
}
uint64_t ESPTimeHelper::getCurrentTimestamp()
{
    return now;
}
struct tm ESPTimeHelper::getTimeFromSec(int seconds)
{
    struct tm timeinfo;
    int _yrs = seconds / (365 * 24 * 3600);
    seconds = seconds - _yrs * (365 * 24 * 3600);
    timeinfo.tm_year = _yrs - 1900;
    int _months = seconds / (30 * 24 * 3600);
    seconds = seconds - _months * (30 * 24 * 3600);
    timeinfo.tm_mon = _months - 1;
    int _days = seconds / (24 * 3600);
    seconds = seconds - _days * (24 * 3600);
    timeinfo.tm_mday = _days;
    int _hr = seconds / 3600;
    seconds = seconds - _hr * 3600;
    timeinfo.tm_hour = _hr;
    int _min = seconds / 60;
    seconds = seconds - _min * 60;
    timeinfo.tm_min = _min;
    timeinfo.tm_sec = seconds;
    return timeinfo;
}

char *ESPTimeHelper::intStr(int value)
{
    char *buf = new char[36];
    memset(buf, 0, 36);
    itoa(value, buf, 10);
    return buf;
}

String ESPTimeHelper::getDateTimeString()
{
    setSysTime();
    std::string s;

    s = sdow[timeinfo.tm_wday];

    s += ", ";
    char *tmp = intStr(timeinfo.tm_mday);
    s += tmp;
    delete[] tmp;

    s += " ";
    s += months[timeinfo.tm_mon];

    s += " ";
    tmp = intStr(timeinfo.tm_year + 1900);
    s += tmp;
    delete[] tmp;

    s += " ";
    if (timeinfo.tm_hour < 10)
        s += "0";
    tmp = intStr(timeinfo.tm_hour);
    s += tmp;
    delete[] tmp;

    s += ":";
    if (timeinfo.tm_min < 10)
        s += "0";
    tmp = intStr(timeinfo.tm_min);
    s += tmp;
    delete[] tmp;

    s += ":";
    if (timeinfo.tm_sec < 10)
        s += "0";
    tmp = intStr(timeinfo.tm_sec);
    s += tmp;
    delete[] tmp;

    int p = 1;
    if (TZ < 0)
        p = -1;
    int tz = TZ;
    float dif = (p * (TZ - tz)) * 60.0;
    if (TZ < 0)
        s += " -";
    else
        s += " +";

    if (tz < 10)
        s += "0";
    tmp = intStr(tz);
    s += tmp;
    delete[] tmp;

    if (dif < 10)
        s += "0";
    tmp = intStr((int)dif);
    s += tmp;
    delete[] tmp;

    return s.c_str();
}

void ESPTimeHelper::setSysTime()
{
#if defined(ESP32)
    getLocalTime(&timeinfo);
#elif defined(ESP8266)
    now = time(nullptr);
    localtime_r(&now, &timeinfo);
#endif
}

#endif //ESPTimeHelper_CPP