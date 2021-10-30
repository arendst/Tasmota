/*
  xdrv_86_esp32_m5epd47.ino - ESP32 m5stack EPD47 support for Tasmota

  Copyright (C) 2021  Gerhard Mutz and Theo Arends

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

#ifdef ESP32
#ifdef USE_M5EPD47
/*********************************************************************************************\
 * M5Stack Epaper 47 support
 *
 * Module 19

 {"NAME":"M5Stack_EPD47","GPIO":[6210,1,1,1,6720,1,1,1,704,672,736,1,1,1,1,1,0,640,608,1,0,641,1,0,0,0,0,0,609,1,1,1,0,0,0,1],"FLAG":0,"BASE":1}

\*********************************************************************************************/

#define XDRV_86        86

#include "M5EPD.h"
#include "BM8563.h"

struct M5EPD_globs {
  BM8563 *Rtc;
  M5EPD m5epd;
  bool ready;
  int32_t shutdownseconds;
  uint8_t wakeup_hour;
  uint8_t wakeup_minute;
  uint8_t shutdowndelay;
} M5EPD_globs;

/*********************************************************************************************/

void M5EPDDoShutdown(void) {
  SettingsSaveAll();
  RtcSettingsSave();
  M5EPD_globs.Rtc->clearIRQ();
  if (M5EPD_globs.shutdownseconds > 0) {
    M5EPD_globs.Rtc->SetAlarmIRQ(M5EPD_globs.shutdownseconds);
  } else {
    RTC_TimeTypeDef wut;
    wut.Hours = M5EPD_globs.wakeup_hour;
    wut.Minutes = M5EPD_globs.wakeup_minute;
    M5EPD_globs.Rtc->SetAlarmIRQ(wut);
  }
  delay(10);
  M5EPD_globs.m5epd.shutdown();
}

/*********************************************************************************************/

void M5EPDModuleInit(void) {

  M5EPD_globs.Rtc = Get_BM8563();
  if (M5EPD_globs.Rtc) {
    M5EPD_globs.m5epd.begin();
    delay(100);
    AddLog(LOG_LEVEL_INFO, PSTR("DRV: M5 E-Paper 4.7"));
    M5EPD_globs.ready = true;
  } else {
    M5EPD_globs.ready = false;
  }
}

void M5EPDEverySecond(void) {
  if (M5EPD_globs.ready) {
    if (M5EPD_globs.shutdowndelay) {
      M5EPD_globs.shutdowndelay--;
      if (!M5EPD_globs.shutdowndelay) {
        M5EPDDoShutdown();
      }
    }
  }
}

void M5EPDShow(uint32_t json) {
  if (!M5EPD_globs.ready) return;
  float bvolt = (float)M5EPD_globs.m5epd.getBatteryVoltage()/1000.0;
  if (json) {
    ResponseAppend_P(PSTR(",\"M5EPD\":{\"BV\":%1.3f}"), bvolt);
  } else {
    WSContentSend_Voltage("Batterie", bvolt);
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

const char kM5EPDCommands[] PROGMEM = "M5EPD|"
  "Shutdown";

void (* const M5EPDCommand[])(void) PROGMEM = {
  &CmndM5EPDShutdown};

void CmndM5EPDShutdown(void) {
  char *mp = strchr(XdrvMailbox.data, ':');
  if (mp) {
    M5EPD_globs.wakeup_hour = atoi(XdrvMailbox.data);
    M5EPD_globs.wakeup_minute = atoi(mp+1);
    M5EPD_globs.shutdownseconds = -1;
    M5EPD_globs.shutdowndelay = 10;
    char tbuff[16];
    sprintf(tbuff, "%02.2d" D_HOUR_MINUTE_SEPARATOR "%02.2d", M5EPD_globs.wakeup_hour, M5EPD_globs.wakeup_minute );
    ResponseCmndChar(tbuff);
  } else {
    if (XdrvMailbox.payload >= 30)  {
      M5EPD_globs.shutdownseconds = XdrvMailbox.payload;
      M5EPD_globs.shutdowndelay = 10;
    }
    ResponseCmndNumber(XdrvMailbox.payload);
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv86(uint8_t function) {
  bool result = false;

  switch (function) {
    case FUNC_EVERY_SECOND:
      M5EPDEverySecond();
      break;
    case FUNC_JSON_APPEND:
      M5EPDShow(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      M5EPDShow(0);
      break;
#endif
    case FUNC_COMMAND:
      result = DecodeCommand(kM5EPDCommands, M5EPDCommand);
      break;
    case FUNC_INIT:
      M5EPDModuleInit();
      break;
  }
  return result;
}

#endif  // USE_M5STACK_M5EPD
#endif  // ESP32
