/*
  xsns_33_ds3231.ino - DS3231/DS1307 RTC chip support for Tasmota

  Copyright (C) 2021  Guy Elgabsi (guy.elg AT gmail.com) and Theo Arends

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
#ifdef USE_DS3231
/*********************************************************************************************\
 * DS1307 and DS3231 support
 *
 * DS3231 - An accurate RTC that used for get time when you do not have internet connection.
 *          We store UTC time in the DS3231, so we can use the standart functions.
 * HOWTO Use : Initially the time needs to be set into the DS3231 either by
 *             - hand using command TIME <epochtime>
 *             - internet using NTP
 *             - GPS using UBX driver
 *             Once stored the time will be automaticaly updated by the DS2331 after power on
 * Source: Guy Elgabsi with special thanks to Jack Christensen
 *
 * I2C Address: 0x68
\*********************************************************************************************/

#define XSNS_33             33
#define XI2C_26             26      // See I2CDEVICES.md

#ifndef USE_RTC_ADDR
#define USE_RTC_ADDR        0x68    // DS3231 I2C Address
#endif

#ifndef USE_GPS                     // USE_GPS provides it's own (better) NTP server so skip this one
#define DS3231_NTP_SERVER
#endif

// DS3231 Register Addresses
#define DS3231_SECONDS      0x00
#define DS3231_MINUTES      0x01
#define DS3231_HOURS        0x02
#define DS3231_DAY          0x03
#define DS3231_DATE         0x04
#define DS3231_MONTH        0x05
#define DS3231_YEAR         0x06
#define DS3231_CONTROL      0x0E
#define DS3231_STATUS       0x0F

// Control register bits
#define DS3231_OSF          7
#define DS3231_EOSC         7
#define DS3231_BBSQW        6
#define DS3231_CONV         5
#define DS3231_RS2          4
#define DS3231_RS1          3
#define DS3231_INTCN        2

//Other
#define DS3231_HR1224       6       // Hours register 12 or 24 hour mode (24 hour mode==0)
#define DS3231_CENTURY      7       // Century bit in Month register
#define DS3231_DYDT         6       // Day/Date flag bit in alarm Day/Date registers

bool ds3231_detected = false;

/*********************************************************************************************/

uint8_t DS3231bcd2dec(uint8_t n) {
  return n - 6 * (n >> 4);
}

uint8_t DS3231dec2bcd(uint8_t n) {
  return n + 6 * (n / 10);
}

/*-------------------------------------------------------------------------------------------*\
 * Read time from DS3231 and return the epoch time (second since 1-1-1970 00:00)
\*-------------------------------------------------------------------------------------------*/
uint32_t DS3231ReadTime(void) {
  TIME_T tm;
  tm.second = DS3231bcd2dec(I2cRead8(USE_RTC_ADDR, DS3231_SECONDS));
  tm.minute = DS3231bcd2dec(I2cRead8(USE_RTC_ADDR, DS3231_MINUTES));
  tm.hour = DS3231bcd2dec(I2cRead8(USE_RTC_ADDR, DS3231_HOURS) & ~_BV(DS3231_HR1224));    // Assumes 24hr clock
  tm.day_of_week = I2cRead8(USE_RTC_ADDR, DS3231_DAY);
  tm.day_of_month = DS3231bcd2dec(I2cRead8(USE_RTC_ADDR, DS3231_DATE));
  tm.month = DS3231bcd2dec(I2cRead8(USE_RTC_ADDR, DS3231_MONTH) & ~_BV(DS3231_CENTURY));  // Don't use the Century bit
  tm.year = DS3231bcd2dec(I2cRead8(USE_RTC_ADDR, DS3231_YEAR));
  return MakeTime(tm);
}

/*-------------------------------------------------------------------------------------------*\
 * Get time as TIME_T and set the DS3231 time to this value
\*-------------------------------------------------------------------------------------------*/
void DS3231SetTime (uint32_t epoch_time) {
  TIME_T tm;
  BreakTime(epoch_time, tm);
  I2cWrite8(USE_RTC_ADDR, DS3231_SECONDS, DS3231dec2bcd(tm.second));
  I2cWrite8(USE_RTC_ADDR, DS3231_MINUTES, DS3231dec2bcd(tm.minute));
  I2cWrite8(USE_RTC_ADDR, DS3231_HOURS, DS3231dec2bcd(tm.hour));
  I2cWrite8(USE_RTC_ADDR, DS3231_DAY, tm.day_of_week);
  I2cWrite8(USE_RTC_ADDR, DS3231_DATE, DS3231dec2bcd(tm.day_of_month));
  I2cWrite8(USE_RTC_ADDR, DS3231_MONTH, DS3231dec2bcd(tm.month));
  I2cWrite8(USE_RTC_ADDR, DS3231_YEAR, DS3231dec2bcd(tm.year));
  I2cWrite8(USE_RTC_ADDR, DS3231_STATUS, I2cRead8(USE_RTC_ADDR, DS3231_STATUS) & ~_BV(DS3231_OSF));  // Clear the Oscillator Stop Flag
}

/*********************************************************************************************/

void DS3231Detect(void) {
  if (!I2cSetDevice(USE_RTC_ADDR)) { return; }

  if (I2cValidRead(USE_RTC_ADDR, DS3231_STATUS, 1)) {
    I2cSetActiveFound(USE_RTC_ADDR, "DS3231");
    ds3231_detected = true;

    if (Rtc.utc_time < START_VALID_TIME) {                      // We still did not sync with NTP/GPS (time not valid), so read time from DS3231
      uint32_t ds3231_time = DS3231ReadTime();                  // Read UTC TIME from DS3231
      if (ds3231_time > START_VALID_TIME) {
        Rtc.utc_time = ds3231_time;
        RtcSync("DS3231");
      }
    }
  }
}

void DS3231TimeSynced(void) {
  if ((Rtc.utc_time > START_VALID_TIME) &&                      // Valid UTC time
      (abs((int32_t)(Rtc.utc_time - DS3231ReadTime())) > 2)) {  // Time has drifted from RTC more than 2 seconds
    DS3231SetTime(Rtc.utc_time);                                // Update the DS3231 time
    AddLog(LOG_LEVEL_DEBUG, PSTR("DS3: Re-synced (" D_UTC_TIME ") %s"), GetDateAndTime(DT_UTC).c_str());
  }
}

#ifdef DS3231_NTP_SERVER
/*********************************************************************************************\
 * NTP functions
\*********************************************************************************************/

#include "NTPServer.h"
#include "NTPPacket.h"

#define NTP_MILLIS_OFFSET   50

NtpServer DS3231timeServer(PortUdp);

bool ds3231_running_NTP = false;

void DS3231EverySecond(void) {
  if (ds3231_running_NTP) {
    DS3231timeServer.processOneRequest(Rtc.utc_time, NTP_MILLIS_OFFSET);
  }
}

/*********************************************************************************************\
 * Supported commands for Sensor33:
 *
 * Sensor33 0  - NTP server off (default)
 * Sensor33 1  - NTP server on
\*********************************************************************************************/

bool DS3231NTPCmd(void) {
  bool serviced = true;

  if (XdrvMailbox.payload >= 0) {
    ds3231_running_NTP = 0;
    if ((XdrvMailbox.payload &1) && DS3231timeServer.beginListening()) {
      ds3231_running_NTP = 1;
    }
  }
  Response_P(PSTR("{\"Sensor33\":{\"NTPServer\":\"%s\"}}"), GetStateText(ds3231_running_NTP));

  return serviced;
}
#endif  // DS3231_NTP_SERVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns33(uint8_t function) {
  if (!I2cEnabled(XI2C_26)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    DS3231Detect();
  }
  else if (ds3231_detected) {
    switch (function) {
#ifdef DS3231_NTP_SERVER
      case FUNC_EVERY_SECOND:
        DS3231EverySecond();
        break;
      case FUNC_COMMAND_SENSOR:
        if (XSNS_33 == XdrvMailbox.index) {
          result = DS3231NTPCmd();
        }
        break;
#endif  // DS3231_NTP_SERVER
      case FUNC_TIME_SYNCED:
        DS3231TimeSynced();
        break;
    }
  }
  return result;
}

#endif // USE_DS3231
#endif // USE_I2C
