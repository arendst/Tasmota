/*
  xsns_56_rtc_chips.ino - RTC chip support for Tasmota

  SPDX-FileCopyrightText: 2022 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_I2C
#ifdef USE_RTC_CHIPS
/*********************************************************************************************\
 * RTC chip support
 * 
 * #define USE_DS3231
 *   DS1307 and DS3231 at I2C address 0x68
 *   Used by Ulanzi TC001
 * #define USE_BM8563
 *   BM8563 at I2C address 0x51
 *   Used by M5Stack and IOTTIMER (v3)
 * #define USE_PCF85363
 *   PCF85363 at I2C address 0x51
 *   Used by Shelly 3EM
 * #define USE_RX8010
 *   RX8010 at I2C address 0x32
 *   Used by IOTTIMER (v1 and v2)
\*********************************************************************************************/

#define XDRV_56             56

#ifdef USE_GPS                     // GPS driver has it's own NTP server
#undef RTC_NTP_SERVER              // Disable NTP server (+0k8 code)
#endif

struct {
  uint32_t (* ReadTime)(void);
  void (* SetTime)(uint32_t);
  int32_t (* MemRead)(uint8_t *, uint32_t);
  int32_t (* MemWrite)(uint8_t *, uint32_t);
  void (* ShowSensor)(bool);
  bool detected;
  int8_t mem_size = -1;
  uint8_t address;
  uint8_t bus;
  char name[10];
} RtcChip;

/*********************************************************************************************\
 * DS1307 and DS3231
 *
 * I2C Address: 0x68
\*********************************************************************************************/
#ifdef USE_DS3231

#define XI2C_26             26      // See I2CDEVICES.md

#define DS3231_ADDRESS      0x68    // DS3231 I2C Address

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
#define DS3231_TEMP_MSB     0x11
#define DS3231_TEMP_LSB     0x12

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

/*-------------------------------------------------------------------------------------------*\
 * Read time from DS3231 and return the epoch time (second since 1-1-1970 00:00)
\*-------------------------------------------------------------------------------------------*/
uint32_t DS3231ReadTime(void) {
  TIME_T tm;
  tm.second = Bcd2Dec(I2cRead8(RtcChip.address, DS3231_SECONDS));
  tm.minute = Bcd2Dec(I2cRead8(RtcChip.address, DS3231_MINUTES));
  tm.hour = Bcd2Dec(I2cRead8(RtcChip.address, DS3231_HOURS) & ~_BV(DS3231_HR1224));    // Assumes 24hr clock
  tm.day_of_week = I2cRead8(RtcChip.address, DS3231_DAY);
  tm.day_of_month = Bcd2Dec(I2cRead8(RtcChip.address, DS3231_DATE));
  tm.month = Bcd2Dec(I2cRead8(RtcChip.address, DS3231_MONTH) & ~_BV(DS3231_CENTURY));  // Don't use the Century bit
  // MakeTime requires tm.year as number of years since 1970, 
  // However DS3231 is supposed to hold the true year but before this PR it was written tm.year directly
  // Assuming we read ... means ...
  //   00..21   = 1970..1990 written before PR (to support a RTC written with 1970) => don't apply correction
  //   22..51   = 2022..2051 written after PR => apply +30 years correction
  //   52..99   = 2022..2069 written before PR => don't apply correction
  uint8_t year = Bcd2Dec(I2cRead8(RtcChip.address, DS3231_YEAR));
  tm.year = ((year <= 21) || (year >= 52)) ? (year) : (year+30);
  return MakeTime(tm);
}

/*-------------------------------------------------------------------------------------------*\
 * Read temperature from DS3231 internal sensor, return as float
\*-------------------------------------------------------------------------------------------*/
#ifdef DS3231_ENABLE_TEMP
float DS3231ReadTemp(void) {
  int16_t temp_reg = I2cReadS16(RtcChip.address, DS3231_TEMP_MSB) >> 6;
  float temp = temp_reg * 0.25;
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("RTC: DS3231 temp_reg=%d"), temp_reg);
  return temp;
}
#endif // #ifdef DS3231_ENABLE_TEMP

/*-------------------------------------------------------------------------------------------*\
 * Show temperature from DS3231 internal sensor, Web or SENSOR
\*-------------------------------------------------------------------------------------------*/
#ifdef DS3231_ENABLE_TEMP
void D3231ShowSensor(bool json) {
    float f_temperature = ConvertTemp(DS3231ReadTemp());

    if (json) {
        ResponseAppend_P(PSTR(",\"DS3231\":{\"" D_JSON_TEMPERATURE "\":%*_f}"), Settings->flag2.temperature_resolution, &f_temperature);
#ifdef USE_DOMOTICZ
        if (0 == TasmotaGlobal.tele_period) {
          DomoticzFloatSensor(DZ_TEMP, f_temperature);
        }
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
        if (0 == TasmotaGlobal.tele_period) {
          KnxSensor(KNX_TEMPERATURE, f_temperature);
        }
#endif  // USE_KNX
    } 
#ifdef USE_WEBSERVER
    else {
        WSContentSend_Temp("DS3231", f_temperature);
    }
#endif // #ifdef USE_WEBSERVER
}
#endif // #ifdef DS3231_ENABLE_TEMP


/*-------------------------------------------------------------------------------------------*\
 * Get time as TIME_T and set the DS3231 time to this value
\*-------------------------------------------------------------------------------------------*/
void DS3231SetTime(uint32_t epoch_time) {
  TIME_T tm;
  BreakTime(epoch_time, tm);
  I2cWrite8(RtcChip.address, DS3231_SECONDS, Dec2Bcd(tm.second));
  I2cWrite8(RtcChip.address, DS3231_MINUTES, Dec2Bcd(tm.minute));
  I2cWrite8(RtcChip.address, DS3231_HOURS, Dec2Bcd(tm.hour));
  I2cWrite8(RtcChip.address, DS3231_DAY, tm.day_of_week);
  I2cWrite8(RtcChip.address, DS3231_DATE, Dec2Bcd(tm.day_of_month));
  I2cWrite8(RtcChip.address, DS3231_MONTH, Dec2Bcd(tm.month));
  // BreakTime returns tm.year as number of years since 1970, while DS3231 expect the true year. Adusting to avoir leap year error
  uint8_t true_year = (tm.year < 30) ? (tm.year + 70) : (tm.year - 30);
  I2cWrite8(RtcChip.address, DS3231_YEAR, Dec2Bcd(true_year));
  I2cWrite8(RtcChip.address, DS3231_STATUS, I2cRead8(RtcChip.address, DS3231_STATUS) & ~_BV(DS3231_OSF));  // Clear the Oscillator Stop Flag
}

/*-------------------------------------------------------------------------------------------*\
 * Detection
\*-------------------------------------------------------------------------------------------*/
void DS3231Detected(void) {
  if (!RtcChip.detected && I2cEnabled(XI2C_26)) {
    RtcChip.address = DS3231_ADDRESS;
    if (I2cSetDevice(RtcChip.address)) {
      if (I2cValidRead(RtcChip.address, DS3231_STATUS, 1)) {
        RtcChip.detected = 1;
        strcpy_P(RtcChip.name, PSTR("DS3231"));
        RtcChip.ReadTime = &DS3231ReadTime;
        RtcChip.SetTime = &DS3231SetTime;
#ifdef DS3231_ENABLE_TEMP
        RtcChip.ShowSensor = &D3231ShowSensor;
#endif
        RtcChip.mem_size = -1;
      }
    }
  }
}
#endif  // USE_DS3231



/*********************************************************************************************\
 * PCF85063 support
 *
 * I2C Address: 0x51
\*********************************************************************************************/
#ifdef USE_PCF85063

#define XI2C_92             92       // Unique ID for I2C device search

#define PCF85063_ADDRESS    0x51     // PCF85063 I2C Address


#define PCF85063_REG_CTRL1      0x00
#define PCF85063_REG_CTRL2      0x01
#define PCF85063_REG_OFFSET     0x02
#define PCF85063_REG_SECONDS    0x04
#define PCF85063_REG_MINUTES    0x05
#define PCF85063_REG_HOURS      0x06
#define PCF85063_REG_DAYS       0x07
#define PCF85063_REG_WEEKDAYS   0x08
#define PCF85063_REG_MONTHS     0x09
#define PCF85063_REG_YEARS      0x0A


uint32_t Pcf85063ReadTime(void) {
  Wire.beginTransmission(RtcChip.address);
  Wire.write(PCF85063_REG_SECONDS);
  Wire.endTransmission(false);   // false -> repeated start
  Wire.requestFrom((uint8_t)RtcChip.address, (uint8_t)7);

  uint8_t sec   = Wire.read(); // 0x04
  uint8_t min   = Wire.read(); // 0x05
  uint8_t hour  = Wire.read(); // 0x06
  uint8_t day   = Wire.read(); // 0x07
  uint8_t wday  = Wire.read(); // 0x08
  uint8_t month = Wire.read(); // 0x09
  uint8_t year  = Wire.read(); // 0x0A

  TIME_T tm;
  tm.second       = Bcd2Dec(sec  & 0x7F); 
  tm.minute       = Bcd2Dec(min  & 0x7F);
  tm.hour         = Bcd2Dec(hour & 0x3F);
  tm.day_of_month = Bcd2Dec(day  & 0x3F);
  tm.day_of_week  = wday & 0x07; 
  tm.month        = Bcd2Dec(month & 0x1F) -1; 
  uint8_t y = Bcd2Dec(year);
  tm.year = (y + 30);
  return MakeTime(tm);
}


void Pcf85063SetTime(uint32_t epoch_time) {
  TIME_T tm;
  BreakTime(epoch_time, tm);


  uint8_t year = (tm.year -30); 
  if (year > 99) { year = 99; } 

  uint8_t bcd_sec   = Dec2Bcd(tm.second);
  uint8_t bcd_min   = Dec2Bcd(tm.minute);
  uint8_t bcd_hour  = Dec2Bcd(tm.hour);
  uint8_t bcd_day   = Dec2Bcd(tm.day_of_month);
  uint8_t bcd_wday  = tm.day_of_week & 0x07;
  uint8_t bcd_month = Dec2Bcd(tm.month +1);
  uint8_t bcd_year  = Dec2Bcd(year);

  Wire.beginTransmission(RtcChip.address);
  Wire.write(PCF85063_REG_SECONDS);
  Wire.write(bcd_sec);
  Wire.write(bcd_min);
  Wire.write(bcd_hour);
  Wire.write(bcd_day);
  Wire.write(bcd_wday);
  Wire.write(bcd_month);
  Wire.write(bcd_year);
  Wire.endTransmission();
}

/*-------------------------------------------------------------------------------------------*\
 * Detection
\*-------------------------------------------------------------------------------------------*/
void Pcf85063Detected(void) {
  if (!RtcChip.detected && I2cEnabled(XI2C_92)) {
    RtcChip.address = PCF85063_ADDRESS;
    // Vyskúšame, či vieme prečítať nejaký register
    if (I2cSetDevice(RtcChip.address)) {
      // Skúsime napr. prečítať PCF85063_REG_CTRL1
      if (I2cValidRead(RtcChip.address, PCF85063_REG_CTRL1, 1)) {
        RtcChip.detected = 1;
        strcpy_P(RtcChip.name, PSTR("PCF85063"));
        RtcChip.ReadTime = &Pcf85063ReadTime;
        RtcChip.SetTime  = &Pcf85063SetTime;
        RtcChip.mem_size = -1;    // Nemá extra user RAM, ak by si nepotreboval

        // Ak by si chcel implementovať MemRead/MemWrite, doplň RtcChip.MemRead a RtcChip.MemWrite
      }
    }
  }
}
#endif // USE_PCF85063



/*********************************************************************************************\
 * BM8563 - Real Time Clock
 *
 * I2C Address: 0x51 (Fixed in library as BM8563_ADRESS)
\*********************************************************************************************/
#ifdef USE_BM8563

#define XI2C_59             59       // See I2CDEVICES.md

#include "BM8563.h"

struct {
  BM8563 Rtc;
  bool rtc_ready = false;
  bool ntp_time_ok = false;
} bm8563_driver;

uint32_t BM8563GetUtc(void) {
  RTC_TimeTypeDef RTCtime;
  // 1. read has errors ???
  bm8563_driver.Rtc.GetTime(&RTCtime);
//   core2_globs.Rtc.GetTime(&RTCtime);
  RTC_DateTypeDef RTCdate;
  bm8563_driver.Rtc.GetDate(&RTCdate);
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

void BM8563SetUtc(uint32_t epoch_time) {
  TIME_T tm;
  BreakTime(epoch_time, tm);
  RTC_TimeTypeDef RTCtime;
  RTCtime.Hours = tm.hour;
  RTCtime.Minutes = tm.minute;
  RTCtime.Seconds = tm.second;
  bm8563_driver.Rtc.SetTime(&RTCtime);
  RTC_DateTypeDef RTCdate;
  RTCdate.WeekDay = tm.day_of_week;
  RTCdate.Month = tm.month;
  RTCdate.Date = tm.day_of_month;
  RTCdate.Year = tm.year + 1970;
  bm8563_driver.Rtc.SetDate(&RTCdate);
}

/*-------------------------------------------------------------------------------------------*\
 * Detection
\*-------------------------------------------------------------------------------------------*/
void BM8563Detected(void) {
  if (!RtcChip.detected && I2cEnabled(XI2C_59)) {
    RtcChip.address = BM8563_ADRESS;
    if (I2cSetDevice(RtcChip.address, 0)) {
      RtcChip.detected = 1;
    }
#ifdef ESP32
    else if (I2cSetDevice(RtcChip.address, 1)) {
      RtcChip.detected = 1;
      RtcChip.bus = 1;
      bm8563_driver.Rtc.setBus(1);                            // switch to bus 1
    }
#endif
    if (RtcChip.detected) {
      bm8563_driver.Rtc.begin();
      strcpy_P(RtcChip.name, PSTR("BM8563"));
      RtcChip.ReadTime = &BM8563GetUtc;
      RtcChip.SetTime = &BM8563SetUtc;
      RtcChip.mem_size = -1;
    }
  }
}
#endif  // USE_BM8563

/*********************************************************************************************\
 * PCF85363 support
 *
 * I2C Address: 0x51
\*********************************************************************************************/
#ifdef USE_PCF85363

#define XI2C_66             66      // See I2CDEVICES.md

#define PCF85363_ADDRESS    0x51    // PCF85363 I2C Address

/*-------------------------------------------------------------------------------------------*\
 * Read time and return the epoch time (second since 1-1-1970 00:00)
\*-------------------------------------------------------------------------------------------*/
uint32_t Pcf85363ReadTime(void) {
  Wire.beginTransmission(RtcChip.address);
  Wire.write(0x00);
  Wire.endTransmission();

  uint8_t buffer[8];
  Wire.requestFrom(RtcChip.address, (uint8_t)8);
  for (uint32_t i = 0; i < 8; i++) { buffer[i] = Wire.read(); }
  Wire.endTransmission();

  TIME_T tm;
  tm.second = Bcd2Dec(buffer[1] & 0x7F);
  tm.minute = Bcd2Dec(buffer[2] & 0x7F);
  tm.hour = Bcd2Dec(buffer[3]);
  tm.day_of_month = Bcd2Dec(buffer[4]);
  tm.day_of_week = buffer[5];
  tm.month = Bcd2Dec(buffer[6]);
  tm.year = 30 + Bcd2Dec(buffer[7]);  // Offset from 1970. So 2022 - 1970 = 52
  return MakeTime(tm);
}

/*-------------------------------------------------------------------------------------------*\
 * Get time as TIME_T and set time to this value
\*-------------------------------------------------------------------------------------------*/
void Pcf85363SetTime(uint32_t epoch_time) {
  TIME_T tm;
  BreakTime(epoch_time, tm);

  uint8_t buffer[8];
  buffer[0] = 0x00;                  // 100th_seconds (not used)
  buffer[1] = Dec2Bcd(tm.second);
  buffer[2] = Dec2Bcd(tm.minute);
  buffer[3] = Dec2Bcd(tm.hour);
  buffer[4] = Dec2Bcd(tm.day_of_month);
  buffer[5] = tm.day_of_week;
  buffer[6] = Dec2Bcd(tm.month);
  buffer[7] = Dec2Bcd(tm.year -30);  // Offset from 1970
/*
  // Handbook page 13
  Wire.beginTransmission(RtcChip.address);
  Wire.write(0x2E);
  Wire.write(0x01);                  // Set stop
  Wire.write(0xA4);                  // Clear prescaler
  for (uint32_t i = 0; i < 8; i++) { Wire.write(buffer[i]); }
  Wire.endTransmission();
  Wire.beginTransmission(RtcChip.address);
  Wire.write(0x2E);
  Wire.write(0x00);                  // Set start
  Wire.endTransmission();
*/
  Wire.beginTransmission(RtcChip.address);
  Wire.write(0x00);
  for (uint32_t i = 0; i < 8; i++) { Wire.write(buffer[i]); }
  Wire.endTransmission();
}

/*-------------------------------------------------------------------------------------------*\
 * Dump all registers
\*-------------------------------------------------------------------------------------------*/
/*
void Pcf85363Dump(void) {
  uint8_t buffer[64];

  // 0x00 to 0x2F
  Wire.beginTransmission(RtcChip.address);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.requestFrom(RtcChip.address, (uint8_t)48);
  for (uint32_t i = 0; i < 48; i++) {
    buffer[i] = Wire.read();
  }
  Wire.endTransmission();
  AddLog(LOG_LEVEL_DEBUG, PSTR("P85: Read 0x00: %48_H"), buffer);

  // 0x40 to 0x7F
  Wire.beginTransmission(RtcChip.address);
  Wire.write(0x40);
  Wire.endTransmission();
  Wire.requestFrom(RtcChip.address, (uint8_t)64);
  for (uint32_t i = 0; i < 64; i++) {
    buffer[i] = Wire.read();
  }
  Wire.endTransmission();
  AddLog(LOG_LEVEL_DEBUG, PSTR("P85: Read 0x40: %64_H"), buffer);
}
*/

/*-------------------------------------------------------------------------------------------*\
 * Memory block functions
\*-------------------------------------------------------------------------------------------*/
int32_t Pcf8563MemRead(uint8_t *buffer, uint32_t size) {
  return I2cReadBuffer(RtcChip.address, 0x40, buffer, size);
}

int32_t Pcf8563MemWrite(uint8_t *buffer, uint32_t size) {
  return I2cWriteBuffer(RtcChip.address, 0x40, (uint8_t *)buffer, size);
}

/*-------------------------------------------------------------------------------------------*\
 * Detection
\*-------------------------------------------------------------------------------------------*/
void Pcf85363Detected(void) {
  if (!RtcChip.detected && I2cEnabled(XI2C_66)) {
    RtcChip.address = PCF85363_ADDRESS;
    if (I2cSetDevice(RtcChip.address)) {
      RtcChip.detected = 1;
      strcpy_P(RtcChip.name, PSTR("PCF85363"));
      RtcChip.ReadTime = &Pcf85363ReadTime;
      RtcChip.SetTime = &Pcf85363SetTime;
      RtcChip.mem_size = 64;
      RtcChip.MemRead = &Pcf8563MemRead;
      RtcChip.MemWrite = &Pcf8563MemWrite;
    }
  }
}
#endif // USE_PCF85363

/*********************************************************************************************\
 * RX8010 - Real Time Clock
 * based on linux/rtc-rx8010.c
 *
 * I2C Address: 0x32
\*********************************************************************************************/
#ifdef USE_RX8010

#define XI2C_90             90       // See I2CDEVICES.md

#define RX8010_ADDRESS      0x32     // RX8010 I2C Address

// RX8010 Register Addresses
#define RX8010_REG_SEC		  0x10
#define RX8010_REG_MIN		  0x11
#define RX8010_REG_HOUR		  0x12
#define RX8010_REG_WDAY		  0x13
#define RX8010_REG_MDAY		  0x14
#define RX8010_REG_MONTH	  0x15
#define RX8010_REG_YEAR		  0x16
#define RX8010_REG_CTRL		  0x1F

// Control Register (1Fh) bit positions
#define RX8010_BIT_CTRL_STOP	6

/*-------------------------------------------------------------------------------------------*\
 * Read time from RX8010 and return the epoch time (second since 1-1-1970 00:00)
\*-------------------------------------------------------------------------------------------*/
uint32_t Rx8010ReadTime(void) {
  TIME_T tm;

  uint8_t data[7];
  I2cReadBuffer(RtcChip.address, RX8010_REG_SEC, data, 7, RtcChip.bus);
  tm.second = Bcd2Dec(data[0] & 0x7F);
  tm.minute = Bcd2Dec(data[1] & 0x7F);
  tm.hour = Bcd2Dec(data[2] & 0x3F);    // Assumes 24hr clock
  tm.day_of_month = Bcd2Dec(data[3] & 0x3F);
  tm.month = Bcd2Dec(data[4] & 0x3F) -1;
  tm.year = Bcd2Dec(data[5]);
	if (tm.year < 70) { tm.year += 100; }
  tm.day_of_week = Bcd2Dec(data[6] & 0x7F);
  return MakeTime(tm);
}

/*-------------------------------------------------------------------------------------------*\
 * Get time as TIME_T and set the RX8010 time to this value
\*-------------------------------------------------------------------------------------------*/
void Rx8010SetTime(uint32_t epoch_time) {
  TIME_T tm;
  BreakTime(epoch_time, tm);
	// Set STOP bit before changing clock/calendar
  I2cWrite8(RtcChip.address, RX8010_REG_CTRL, I2cRead8(RtcChip.address, RX8010_REG_CTRL, RtcChip.bus) | _BV(RX8010_BIT_CTRL_STOP), RtcChip.bus);
  uint8_t data[7];
  data[0] = Dec2Bcd(tm.second);
  data[1] = Dec2Bcd(tm.minute);
  data[2] = Dec2Bcd(tm.hour);
  data[3] = Dec2Bcd(tm.day_of_month);
  data[4] = Dec2Bcd(tm.month +1);
  data[5] = Dec2Bcd(tm.year % 100);
  data[6] = Dec2Bcd(tm.day_of_week);
  I2cWriteBuffer(RtcChip.address, RX8010_REG_SEC, data, 7, RtcChip.bus);
	// Clear STOP bit after changing clock/calendar
  I2cWrite8(RtcChip.address, RX8010_REG_CTRL, I2cRead8(RtcChip.address, RX8010_REG_CTRL, RtcChip.bus) & ~_BV(RX8010_BIT_CTRL_STOP), RtcChip.bus);
}

/*-------------------------------------------------------------------------------------------*\
 * Detection
\*-------------------------------------------------------------------------------------------*/
void Rx8010Detected(void) {
  if (!RtcChip.detected && I2cEnabled(XI2C_90)) {
    RtcChip.address = RX8010_ADDRESS;
    for (RtcChip.bus = 0; RtcChip.bus < 2; RtcChip.bus++) {
      if (!I2cSetDevice(RtcChip.address, RtcChip.bus)) { continue; }
      if (I2cValidRead(RtcChip.address, RX8010_REG_CTRL, 1, RtcChip.bus)) {
        RtcChip.detected = 1;
        strcpy_P(RtcChip.name, PSTR("RX8010"));
        RtcChip.ReadTime = &Rx8010ReadTime;
        RtcChip.SetTime = &Rx8010SetTime;
        RtcChip.mem_size = -1;
        break;
      }
    }
  }
}
#endif  // USE_RX8010

/*********************************************************************************************\
 * RTC Detect and time set
\*********************************************************************************************/

void RtcChipDetect(void) {
  RtcChip.detected = 0;
  RtcChip.bus = 0;

#ifdef USE_DS3231
  DS3231Detected();
#endif  // USE_DS3231
#ifdef USE_BM8563
  BM8563Detected();
#endif  // USE_BM8563
#ifdef USE_PCF85363
  Pcf85363Detected();
#endif // USE_PCF85363
#ifdef USE_RX8010
  Rx8010Detected();
#endif  // USE_RX8010
#ifdef USE_PCF85063
  Pcf85063Detected();
#endif  // USE_PCF85063

  if (!RtcChip.detected) { return; }

  I2cSetActiveFound(RtcChip.address, RtcChip.name, RtcChip.bus);

  if (Rtc.utc_time < START_VALID_TIME) {                          // Not sync with NTP/GPS (time not valid), so read time
    uint32_t time = RtcChip.ReadTime();                           // Read UTC TIME
    if (time > START_VALID_TIME) {
      Rtc.utc_time = time;
      RtcSync(RtcChip.name);
    }
  }
}

void RtcChipTimeSynced(void) {
  if ((Rtc.utc_time > START_VALID_TIME) &&                        // Valid UTC time
      (abs((int32_t)(Rtc.utc_time - RtcChip.ReadTime())) > 2)) {  // Time has drifted from RTC more than 2 seconds
    RtcChip.SetTime(Rtc.utc_time);                                // Update time
    AddLog(LOG_LEVEL_DEBUG, PSTR("RTC: %s re-synced (" D_UTC_TIME ") %s"), RtcChip.name, GetDateAndTime(DT_UTC).c_str());
  }
}

int32_t RtcChipMemSize(void) {
  return RtcChip.mem_size;                                        // Not supported or max size
}

int32_t RtcChipMemRead(uint8_t *buffer, uint32_t size) {
  if (size <= RtcChip.mem_size) {
    return RtcChip.MemRead(buffer, size);
  }
  return -1;                                                      // Not supported or too large
}

int32_t RtcChipMemWrite(uint8_t *buffer, uint32_t size) {
  if (size <= RtcChip.mem_size) {
    return RtcChip.MemWrite(buffer, size);
  }
  return -1;                                                      // Not supported or too large
}

/*********************************************************************************************\
 * NTP server functions
\*********************************************************************************************/
#ifdef RTC_NTP_SERVER

#include "NTPServer.h"
#include "NTPPacket.h"

#define NTP_MILLIS_OFFSET   50

const char kRtcChipCommands[] PROGMEM = "Rtc|"  // Prefix
  D_CMND_NTPSERVER;

void (* const RtcChipCommand[])(void) PROGMEM = {
  &CmndRtcNtpServer };

NtpServer RtcChipTimeServer(PortUdp);

void RtcChipEverySecond(void) {
  static bool ntp_server_started = false;

  if (TasmotaGlobal.global_state.network_down) { return; }  // Exception on ESP32 if network is down (#17338)

  if (Settings->sbflag1.local_ntp_server && (Rtc.utc_time > START_VALID_TIME)) {
    if (!ntp_server_started) {
      if (RtcChipTimeServer.beginListening()) {
        ntp_server_started = true;
        AddLog(LOG_LEVEL_DEBUG, PSTR("RTC: NTP server started"));
      }
    } else {
      RtcChipTimeServer.processOneRequest(Rtc.utc_time, NTP_MILLIS_OFFSET);
    }
  }
}

void CmndRtcNtpServer(void) {
  // RtcChipNtpServer 0 or 1
  if ((XdrvMailbox.payload >= 0) && !TasmotaGlobal.global_state.network_down) {
    Settings->sbflag1.local_ntp_server = 0;
    if ((XdrvMailbox.payload &1) && RtcChipTimeServer.beginListening()) {
      Settings->sbflag1.local_ntp_server = 1;
    }
  }
  ResponseCmndStateText(Settings->sbflag1.local_ntp_server);
}
#endif  // RTC_NTP_SERVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv56(uint32_t function) {
  bool result = false;

#ifdef RTC_NTP_SERVER
  switch (function) {
    case FUNC_EVERY_SECOND:
      RtcChipEverySecond();
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kRtcChipCommands, RtcChipCommand);
      break;
  }
#endif  // RTC_NTP_SERVER

  if (FUNC_SETUP_RING1 == function) {
    RtcChipDetect();
  }
  else if (RtcChip.detected) {
    switch (function) {
      case FUNC_TIME_SYNCED:
        RtcChipTimeSynced();
        break;
      case FUNC_WEB_SENSOR:
        if (RtcChip.ShowSensor) RtcChip.ShowSensor(0);
        break;
      case FUNC_JSON_APPEND:
        if (RtcChip.ShowSensor) RtcChip.ShowSensor(1);
        break;
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }

  return result;
}

#endif  // USE_RTC_CHIPS
#endif  // USE_I2C
