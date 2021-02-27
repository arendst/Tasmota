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

/* remaining work:

i2s microphone as loudness sensor
rtc better sync

*/

#ifdef ESP32
#ifdef USE_M5STACK_CORE2

#include <Esp.h>
#include <sys/time.h>
#include <esp_system.h>

#include <AXP192.h>
#include <MPU6886.h>
#include <BM8563_RTC.h>
#include <soc/rtc.h>

#define XDRV_84          84

struct CORE2_globs {
  AXP192 Axp;
  MPU6886 Mpu;
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
  int16_t x;
  int16_t y;
  int16_t z;
} core2_adc;

// cause SC card is needed by scripter
void CORE2_Module_Init(void) {

  // m5stack uses pin 38 not selectable in tasmota
  SPI.setFrequency(40000000);
  SPI.begin(18, 38, 23, -1);
  // establish power chip on wire1 SDA 21, SCL 22
  core2_globs.Axp.begin();
  I2cSetActiveFound(AXP_ADDR, "AXP192");

  core2_globs.Axp.SetAdcState(true);
  // motor voltage
  core2_globs.Axp.SetLDOVoltage(3,2000);

  core2_globs.Mpu.Init();
  I2cSetActiveFound(MPU6886_ADDRESS, "MPU6886");

  core2_globs.Rtc.begin();
  I2cSetActiveFound(RTC_ADRESS, "RTC");

  core2_globs.ready = true;
}


void CORE2_Init(void) {

  if (Rtc.utc_time < START_VALID_TIME) {
    // set rtc from chip
    Rtc.utc_time = Get_utc();

    TIME_T tmpTime;
    TasmotaGlobal.ntp_force_sync = true; //force to sync with ntp
    BreakTime(Rtc.utc_time, tmpTime);
    Rtc.daylight_saving_time = RuleToTime(Settings.tflag[1], RtcTime.year);
    Rtc.standard_time = RuleToTime(Settings.tflag[0], RtcTime.year);
    AddLog(LOG_LEVEL_INFO, PSTR("Set time from BM8563 to RTC (" D_UTC_TIME ") %s, (" D_DST_TIME ") %s, (" D_STD_TIME ") %s"),
                GetDateAndTime(DT_UTC).c_str(), GetDateAndTime(DT_DST).c_str(), GetDateAndTime(DT_STD).c_str());
    if (Rtc.local_time < START_VALID_TIME) {  // 2016-01-01
      TasmotaGlobal.rules_flag.time_init = 1;
    } else {
      TasmotaGlobal.rules_flag.time_set = 1;
    }

  }

}

void CORE2_audio_power(bool power) {
  core2_globs.Axp.SetSpkEnable(power);
}

#ifdef USE_WEBSERVER
const char HTTP_CORE2[] PROGMEM =
 "{s}VBUS Voltage" "{m}%s V" "{e}"
 "{s}VBUS Current" "{m}%s mA" "{e}"
 "{s}BATT Voltage" "{m}%s V" "{e}"
 "{s}BATT Current" "{m}%s mA" "{e}"
 "{s}Chip Temperature" "{m}%s C" "{e}";
#ifdef USE_MPU6886
const char HTTP_CORE2_MPU[] PROGMEM =
 "{s}MPU x" "{m}%d mg" "{e}"
 "{s}MPU y" "{m}%d mg" "{e}"
 "{s}MPU z" "{m}%d mg" "{e}";
#endif // USE_MPU6886
#endif  // USE_WEBSERVER


void CORE2_loop(uint32_t flg) {

}

void CORE2_WebShow(uint32_t json) {

  char vstring[32];
  char bvstring[32];
  char cstring[32];
  char bcstring[32];
  char tstring[32];
  dtostrfd(core2_adc.vbus_v, 3, vstring);
  dtostrfd(core2_adc.batt_v, 3, bvstring);
  dtostrfd(core2_adc.vbus_c, 1, cstring);
  dtostrfd(core2_adc.batt_c, 1, bcstring);
  dtostrfd(core2_adc.temp, 2, tstring);

  if (json) {
    ResponseAppend_P(PSTR(",\"CORE2\":{\"VBV\":%s,\"BV\":%s,\"VBC\":%s,\"BC\":%s,\"CT\":%s"), vstring, cstring, bvstring, bcstring, tstring);

#ifdef USE_MPU6886
    ResponseAppend_P(PSTR(",\"MPUX\":%d,\"MPUY\":%d,\"MPUZ\":%d"), core2_adc.x, core2_adc.y, core2_adc.z);
#endif
    ResponseJsonEnd();
  } else {
    WSContentSend_PD(HTTP_CORE2, vstring, cstring, bvstring, bcstring, tstring);

#ifdef USE_MPU6886
    WSContentSend_PD(HTTP_CORE2_MPU, core2_adc.x, core2_adc.y, core2_adc.z);
#endif // USE_MPU6886
  }
}

const char CORE2_Commands[] PROGMEM = "CORE2|"
  "SHUTDOWN";

void (* const CORE2_Command[])(void) PROGMEM = {
  &CORE2_Shutdown};


void CORE2_Shutdown(void) {
  char *mp = strchr(XdrvMailbox.data, ':');
  if (mp) {
    core2_globs.wakeup_hour = atoi(XdrvMailbox.data);
    core2_globs.wakeup_minute = atoi(mp+1);
    core2_globs.shutdownseconds = -1;
    core2_globs.shutdowndelay = 10;
    char tbuff[16];
    sprintf(tbuff,"%02.2d:%02.2d", core2_globs.wakeup_hour, core2_globs.wakeup_minute );
    ResponseCmndChar(tbuff);
  } else {
    if (XdrvMailbox.payload >= 30)  {
      core2_globs.shutdownseconds = XdrvMailbox.payload;
      core2_globs.shutdowndelay = 10;
    }
    ResponseCmndNumber(XdrvMailbox.payload);
  }

}

void CORE2_DoShutdown(void) {
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

extern uint8_t tbstate[3];


// c2ps(a b)
float core2_setaxppin(uint32_t sel, uint32_t val) {
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
      GetRtc();
      break;
  }
  return 0;
}

void core2_disp_pwr(uint8_t on) {
  core2_globs.Axp.SetDCDC3(on);
}

// display dimmer ranges from 0-15
// very little effect
void core2_disp_dim(uint8_t dim) {
uint16_t voltage = 2200;

  voltage += ((uint32_t)dim*1200)/15;
  core2_globs.Axp.SetLcdVoltage(voltage);


//  core2_globs.Axp.ScreenBreath(dim);

}


void GetRtc(void) {
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

  AddLog(LOG_LEVEL_INFO, PSTR("RTC: %02d:%02d:%02d"), RTCtime.Hours, RTCtime.Minutes, RTCtime.Seconds);
  AddLog(LOG_LEVEL_INFO, PSTR("RTC: %02d.%02d.%04d"),  RTCdate.Date, RTCdate.Month, RTCdate.Year);

}

void Set_utc(uint32_t epoch_time) {
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

uint32_t Get_utc(void) {
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
  tm.year =RTCdate.Year - 1970;
  return MakeTime(tm);
}

void CORE2_EverySecond(void) {
  if (core2_globs.ready) {
    CORE2_GetADC();

    if (Rtc.utc_time > START_VALID_TIME && core2_globs.tset==false && abs(Rtc.utc_time - Get_utc()) > 3) {
      Set_utc(Rtc.utc_time);
      AddLog(LOG_LEVEL_INFO, PSTR("Write Time TO BM8563 from NTP (" D_UTC_TIME ") %s, (" D_DST_TIME ") %s, (" D_STD_TIME ") %s"),
                  GetDateAndTime(DT_UTC).c_str(), GetDateAndTime(DT_DST).c_str(), GetDateAndTime(DT_STD).c_str());
      core2_globs.tset = true;
    }

    if (core2_globs.shutdowndelay) {
      core2_globs.shutdowndelay--;
      if (!core2_globs.shutdowndelay) {
        CORE2_DoShutdown();
      }
    }
  }
}

void CORE2_GetADC(void) {
    core2_adc.vbus_v = core2_globs.Axp.GetVBusVoltage();
    core2_adc.batt_v = core2_globs.Axp.GetBatVoltage();
    core2_adc.vbus_c = core2_globs.Axp.GetVinCurrent();
    core2_adc.batt_c = core2_globs.Axp.GetBatCurrent();

    core2_adc.temp = core2_globs.Axp.GetTempInAXP192();
#ifdef USE_MPU6886
      float x;
      float y;
      float z;
      core2_globs.Mpu.getAccelData(&x, &y, &z);
      core2_adc.x=x*1000;
      core2_adc.y=y*1000;
      core2_adc.z=z*1000;
#endif // USE_MPU6886
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv84(uint8_t function) {
  bool result = false;

  switch (function) {

    case FUNC_WEB_SENSOR:
#ifdef USE_WEBSERVER
      CORE2_WebShow(0);
#endif
      break;
    case FUNC_JSON_APPEND:
      CORE2_WebShow(1);
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(CORE2_Commands, CORE2_Command);
      break;
    case FUNC_MODULE_INIT:
      CORE2_Module_Init();
      break;
    case FUNC_INIT:
      CORE2_Init();
      break;
    case FUNC_EVERY_SECOND:
      CORE2_EverySecond();
      break;
    case FUNC_LOOP:
      CORE2_loop(1);
      break;

  }
  return result;
}

#endif  // USE_M5STACK_CORE2
#endif  // ESP32
