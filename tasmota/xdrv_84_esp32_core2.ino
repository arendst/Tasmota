/*
  xdrv_84_esp32_core2.ino - ESP32 m5stack core2 support for Tasmota

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
#ifdef USE_M5STACK_CORE2
/*********************************************************************************************\
 * M5Stack Core2 support
 *
 * Module 7
 * Template {"NAME":"M5Core2","GPIO":[1,1,1,1,6720,6368,0,0,0,1,1,6400,0,0,736,1,0,0,0,704,0,1,1,1,0,0,0,0,640,608,1,1,1,0,672,0],"FLAG":0,"BASE":7}
 *
 * Initial commands:
 * - DisplayType 2
 * - DisplayCols 27
 * - (optional) DisplayMode 2
 * - Power on
 * - Voltres 3
 * - Ampres 1
 *
 * Todo:
 * - i2s microphone as loudness sensor
 * - rtc better sync
\*********************************************************************************************/

#define XDRV_84          84

#include <Esp.h>
#include <sys/time.h>
#include <esp_system.h>

#include <AXP192.h>
#include <BM8563_RTC.h>
#include <soc/rtc.h>
#include <SPI.h>

struct CORE2_globs {
  AXP192 Axp;
  BM8563_RTC Rtc;
  bool ready;
  bool tset;
  int32_t shutdownseconds;
  uint8_t wakeup_hour;
  uint8_t wakeup_minute;
  uint8_t shutdowndelay;
} core2_globs;

struct CORE2_ADC {
  float vbus_v;
  float batt_v;
  float vbus_c;
  float batt_c;
  float temp;
} core2_adc;

/*********************************************************************************************/

void Core2DoShutdown(void) {
  SettingsSaveAll();
  RtcSettingsSave();
  core2_globs.Rtc.clearIRQ();
  if (core2_globs.shutdownseconds > 0) {
    core2_globs.Rtc.SetAlarmIRQ(core2_globs.shutdownseconds);
  } else {
    RTC_TimeTypeDef wut;
    wut.Hours = core2_globs.wakeup_hour;
    wut.Minutes = core2_globs.wakeup_minute;
    core2_globs.Rtc.SetAlarmIRQ(wut);
  }
  delay(10);
  core2_globs.Axp.PowerOff();
}

void Core2GetADC(void) {
  core2_adc.vbus_v = core2_globs.Axp.GetVBusVoltage();
  core2_adc.batt_v = core2_globs.Axp.GetBatVoltage();
  core2_adc.vbus_c = core2_globs.Axp.GetVinCurrent();
  core2_adc.batt_c = core2_globs.Axp.GetBatCurrent();

  core2_adc.temp = ConvertTemp(core2_globs.Axp.GetTempInAXP192());
}

void Core2GetRtc(void) {
  RTC_TimeTypeDef RTCtime;
  core2_globs.Rtc.GetTime(&RTCtime);
  RtcTime.hour = RTCtime.Hours;
  RtcTime.minute = RTCtime.Minutes;
  RtcTime.second = RTCtime.Seconds;

  RTC_DateTypeDef RTCdate;
  core2_globs.Rtc.GetDate(&RTCdate);
  RtcTime.day_of_week = RTCdate.WeekDay;
  RtcTime.month = RTCdate.Month;
  RtcTime.day_of_month = RTCdate.Date;
  RtcTime.year = RTCdate.Year;

  AddLog(LOG_LEVEL_INFO, PSTR("CR2: Set RTC %04d-%02d-%02dT%02d:%02d:%02d"),
    RTCdate.Year, RTCdate.Month, RTCdate.Date, RTCtime.Hours, RTCtime.Minutes, RTCtime.Seconds);
}

void Core2SetUtc(uint32_t epoch_time) {
  TIME_T tm;
  BreakTime(epoch_time, tm);
  RTC_TimeTypeDef RTCtime;
  RTCtime.Hours = tm.hour;
  RTCtime.Minutes = tm.minute;
  RTCtime.Seconds = tm.second;
  core2_globs.Rtc.SetTime(&RTCtime);
  RTC_DateTypeDef RTCdate;
  RTCdate.WeekDay = tm.day_of_week;
  RTCdate.Month = tm.month;
  RTCdate.Date = tm.day_of_month;
  RTCdate.Year = tm.year + 1970;
  core2_globs.Rtc.SetDate(&RTCdate);
}

uint32_t Core2GetUtc(void) {
  RTC_TimeTypeDef RTCtime;
  // 1. read has errors ???
  core2_globs.Rtc.GetTime(&RTCtime);
  core2_globs.Rtc.GetTime(&RTCtime);
  RTC_DateTypeDef RTCdate;
  core2_globs.Rtc.GetDate(&RTCdate);
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

/*********************************************************************************************\
 * Called from xdrv_10_scripter.ino
\*********************************************************************************************/

extern uint8_t tbstate[3];

// c2ps(a b)
float Core2SetAxpPin(uint32_t sel, uint32_t val) {
  switch (sel) {
    case 0:
      core2_globs.Axp.SetLed(val);
      break;
    case 1:
      core2_globs.Axp.SetLDOEnable(3, val);
      break;
    case 2:
      if (val<1 || val>3) val = 1;
      return tbstate[val - 1] & 1;
      break;
    case 3:
      switch (val) {
        case 0:
          return core2_globs.Axp.isACIN();
          break;
        case 1:
          return core2_globs.Axp.isCharging();
          break;
        case 2:
          return core2_globs.Axp.isVBUS();
          break;
        case 3:
          return core2_globs.Axp.AXPInState();
          break;
      }
      break;
    default:
      Core2GetRtc();
      break;
  }
  return 0;
}

/*********************************************************************************************\
 * Called from xdrv_42_i2s_audio.ino
\*********************************************************************************************/

void Core2AudioPower(bool power) {
  core2_globs.Axp.SetSpkEnable(power);
}

/*********************************************************************************************\
 * Called from xdsp_04_ili9341.ino and xdsp_17_universal.ino
\*********************************************************************************************/

void Core2DisplayPower(uint8_t on) {
  core2_globs.Axp.SetDCDC3(on);
}

// display dimmer ranges from 0-15
// very little effect
void Core2DisplayDim(uint8_t dim) {
  uint16_t voltage = 2200;

  voltage += ((uint32_t)dim*1200)/15;
  core2_globs.Axp.SetLcdVoltage(voltage);

//  core2_globs.Axp.ScreenBreath(dim);
}

/*********************************************************************************************/

// cause SC card is needed by scripter
void Core2ModuleInit(void) {
  // m5stack uses pin 38 not selectable in tasmota
  SPI.setFrequency(40000000);
  SPI.begin(18, 38, 23, -1);
  // establish power chip on wire1 SDA 21, SCL 22
  core2_globs.Axp.begin();
  I2cSetActiveFound(AXP_ADDR, "AXP192");

  core2_globs.Axp.SetAdcState(true);
  // motor voltage
  core2_globs.Axp.SetLDOVoltage(3,2000);

  core2_globs.Rtc.begin();
  I2cSetActiveFound(RTC_ADRESS, "RTC");

  core2_globs.ready = true;
}

void Core2Init(void) {
  if (Rtc.utc_time < START_VALID_TIME) {
    // set rtc from chip
    Rtc.utc_time = Core2GetUtc();

    TIME_T tmpTime;
    TasmotaGlobal.ntp_force_sync = true;  // Force to sync with ntp
    BreakTime(Rtc.utc_time, tmpTime);
    Rtc.daylight_saving_time = RuleToTime(Settings->tflag[1], RtcTime.year);
    Rtc.standard_time = RuleToTime(Settings->tflag[0], RtcTime.year);
    AddLog(LOG_LEVEL_INFO, PSTR("CR2: Set time from BM8563 to RTC (" D_UTC_TIME ") %s, (" D_DST_TIME ") %s, (" D_STD_TIME ") %s"),
                GetDateAndTime(DT_UTC).c_str(), GetDateAndTime(DT_DST).c_str(), GetDateAndTime(DT_STD).c_str());
    if (Rtc.local_time < START_VALID_TIME) {  // 2016-01-01
      TasmotaGlobal.rules_flag.time_init = 1;
    } else {
      TasmotaGlobal.rules_flag.time_set = 1;
    }
  }
}

void Core2Loop(uint32_t flg) {

}

void Core2EverySecond(void) {
  if (core2_globs.ready) {
    Core2GetADC();

    if (Rtc.utc_time > START_VALID_TIME && core2_globs.tset==false && abs((int32_t)Rtc.utc_time - (int32_t)Core2GetUtc()) > 3) {
      Core2SetUtc(Rtc.utc_time);
      AddLog(LOG_LEVEL_INFO, PSTR("CR2: Write Time TO BM8563 from NTP (" D_UTC_TIME ") %s, (" D_DST_TIME ") %s, (" D_STD_TIME ") %s"),
                  GetDateAndTime(DT_UTC).c_str(), GetDateAndTime(DT_DST).c_str(), GetDateAndTime(DT_STD).c_str());
      core2_globs.tset = true;
    }

    if (core2_globs.shutdowndelay) {
      core2_globs.shutdowndelay--;
      if (!core2_globs.shutdowndelay) {
        Core2DoShutdown();
      }
    }
  }
}

void Core2Show(uint32_t json) {
  if (json) {
    ResponseAppend_P(PSTR(",\"Core2\":{\"VBV\":%*_f,\"VBC\":%*_f,\"BV\":%*_f,\"BC\":%*_f,\"" D_JSON_TEMPERATURE "\":%*_f}"),
      Settings->flag2.voltage_resolution, &core2_adc.vbus_v,
      Settings->flag2.current_resolution, &core2_adc.vbus_c,
      Settings->flag2.voltage_resolution, &core2_adc.batt_v,
      Settings->flag2.current_resolution, &core2_adc.batt_c,
      Settings->flag2.temperature_resolution, &core2_adc.temp);
  } else {
    WSContentSend_Voltage("VBus", core2_adc.vbus_v);
    WSContentSend_CurrentMA("VBus", core2_adc.vbus_c);
    WSContentSend_Voltage("Batt", core2_adc.batt_v);
    WSContentSend_CurrentMA("Batt", core2_adc.batt_c);
    WSContentSend_Temp("Core2", core2_adc.temp);
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

const char kCore2Commands[] PROGMEM = "Core2|"
  "Shutdown";

void (* const Core2Command[])(void) PROGMEM = {
  &CmndCore2Shutdown};

void CmndCore2Shutdown(void) {
  char *mp = strchr(XdrvMailbox.data, ':');
  if (mp) {
    core2_globs.wakeup_hour = atoi(XdrvMailbox.data);
    core2_globs.wakeup_minute = atoi(mp+1);
    core2_globs.shutdownseconds = -1;
    core2_globs.shutdowndelay = 10;
    char tbuff[16];
    sprintf(tbuff, "%02.2d" D_HOUR_MINUTE_SEPARATOR "%02.2d", core2_globs.wakeup_hour, core2_globs.wakeup_minute );
    ResponseCmndChar(tbuff);
  } else {
    if (XdrvMailbox.payload >= 30)  {
      core2_globs.shutdownseconds = XdrvMailbox.payload;
      core2_globs.shutdowndelay = 10;
    }
    ResponseCmndNumber(XdrvMailbox.payload);
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv84(uint8_t function) {
  bool result = false;

  switch (function) {
    case FUNC_LOOP:
      Core2Loop(1);
      break;
    case FUNC_EVERY_SECOND:
      Core2EverySecond();
      break;
    case FUNC_JSON_APPEND:
      Core2Show(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      Core2Show(0);
      break;
#endif
    case FUNC_COMMAND:
      result = DecodeCommand(kCore2Commands, Core2Command);
      break;
    case FUNC_INIT:
      Core2Init();
      break;
    case FUNC_MODULE_INIT:
      Core2ModuleInit();
      break;
  }
  return result;
}

#endif  // USE_M5STACK_CORE2
#endif  // ESP32
