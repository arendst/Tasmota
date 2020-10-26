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

#ifndef ESP32TimeHelper_H
#define ESP32TimeHelper_H

#ifdef ESP32


#include <time.h>
#include <Arduino.h>
#include <WiFi.h>

class ESP32TimeHelper
{
public:
  ESP32TimeHelper();
  bool clockReady = false;
  bool setClock(float gmtOffset, float daylightOffset);
  uint32_t getUnixTime();
  time_t getTimestamp(int year, int mon, int date, int hour, int mins, int sec);
  int getYear();
  int getMonth();
  int getDay();
  int getDayOfWeek();
  String getDayOfWeekString();
  int getHour();
  int getMin();
  int getSec();
  int getNumberOfDayThisYear();
  int getTotalDays(int year, int month, int day);
  int dayofWeek(int year, int month, int day);
  int getCurrentSecond();
  uint64_t getCurrentTimestamp();
  void getTimeFromSec(int secCount, int &yrs, int &months, int &days, int &hr, int &min, int &sec);

private:
  time_t now;
  uint64_t msec_time_diff = 0;
  struct tm timeinfo;
  float TZ = 0.0;
  float DST_MN = 0.0;
  
  bool setClock();
  int totalDays(int y, int m, int d);
  const char *dow[20] = {"sunday", "monday", "tuesday", "wednesday", "thurseday", "friday", "saturday"};
};

#endif //ESP32

#endif //ESP32TimeHelper_H
