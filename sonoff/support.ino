/*
These routines provide support to my various ESP8266 based projects.

Copyright (c) 2016 Theo Arends.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

/*********************************************************************************************\
 * Config - Flash or Spiffs
\*********************************************************************************************/

extern "C" {
#include "spi_flash.h"
}

#define SPIFFS_CONFIG       "/config.ini"
#define SPIFFS_START        ((uint32_t)&_SPIFFS_start - 0x40200000) / SPI_FLASH_SEC_SIZE
#define SPIFFS_END          ((uint32_t)&_SPIFFS_end - 0x40200000) / SPI_FLASH_SEC_SIZE
#define CFG_LOCATION        SPIFFS_END - 2

uint32_t _cfgHash = 0;
int spiffsflag = 0;

uint32_t getHash()
{
  uint32_t hash = 0;
  uint8_t *bytes = (uint8_t*)&sysCfg;

  for (int i = 0; i < sizeof(SYSCFG); i++) hash += bytes[i]*(i+1);
  return hash;  
}

/*********************************************************************************************\
 * Config Save - Save parameters to Flash or Spiffs ONLY if any parameter has changed
\*********************************************************************************************/
void CFG_Save()
{
  char log[LOGSZ];

  if ((getHash() != _cfgHash) && (spiffsPresent())) {
    if (!spiffsflag) {
#ifdef USE_SPIFFS
      sysCfg.saveFlag++;
      File f = SPIFFS.open(SPIFFS_CONFIG, "r+");
      if (f) {
        uint8_t *bytes = (uint8_t*)&sysCfg;
        for (int i = 0; i < sizeof(SYSCFG); i++) f.write(bytes[i]);
        f.close();
        snprintf_P(log, sizeof(log), PSTR("Config: Saved configuration (%d bytes) to spiffs count %d"), sizeof(SYSCFG), sysCfg.saveFlag);
        addLog(LOG_LEVEL_DEBUG, log);
      } else {
        addLog_P(LOG_LEVEL_ERROR, PSTR("Config: ERROR - Saving configuration failed"));
      }
    } else {
#endif  // USE_SPIFFS
      noInterrupts();
      if (sysCfg.saveFlag == 0) {  // Handle default and rollover
        spi_flash_erase_sector(CFG_LOCATION + (sysCfg.saveFlag &1));
        spi_flash_write((CFG_LOCATION + (sysCfg.saveFlag &1)) * SPI_FLASH_SEC_SIZE, (uint32*)&sysCfg, sizeof(SYSCFG));
      }
      sysCfg.saveFlag++;
      spi_flash_erase_sector(CFG_LOCATION + (sysCfg.saveFlag &1));
      spi_flash_write((CFG_LOCATION + (sysCfg.saveFlag &1)) * SPI_FLASH_SEC_SIZE, (uint32*)&sysCfg, sizeof(SYSCFG));
      interrupts();
      snprintf_P(log, sizeof(log), PSTR("Config: Saved configuration (%d bytes) to flash at %X and count %d"), sizeof(SYSCFG), CFG_LOCATION + (sysCfg.saveFlag &1), sysCfg.saveFlag);
      addLog(LOG_LEVEL_DEBUG, log);
    }
    _cfgHash = getHash();
  }
}

void CFG_Load()
{
  char log[LOGSZ];

  if (spiffsPresent()) {
    if (!spiffsflag) {
#ifdef USE_SPIFFS
      File f = SPIFFS.open(SPIFFS_CONFIG, "r+");
      if (f) {
        uint8_t *bytes = (uint8_t*)&sysCfg;
        for (int i = 0; i < sizeof(SYSCFG); i++) bytes[i] = f.read();
        f.close();
        snprintf_P(log, sizeof(log), PSTR("Config: Loaded configuration from spiffs count %d"), sysCfg.saveFlag);
        addLog(LOG_LEVEL_DEBUG, log);
      } else {
        addLog_P(LOG_LEVEL_ERROR, PSTR("Config: ERROR - Loading configuration failed"));
      }
    } else {  
#endif  // USE_SPIFFS
      struct SYSCFGH {
        unsigned long cfg_holder;
        unsigned long saveFlag;
      } _sysCfgH;

      noInterrupts();
      spi_flash_read((CFG_LOCATION) * SPI_FLASH_SEC_SIZE, (uint32*)&sysCfg, sizeof(SYSCFG));
      spi_flash_read((CFG_LOCATION + 1) * SPI_FLASH_SEC_SIZE, (uint32*)&_sysCfgH, sizeof(SYSCFGH));
      if (sysCfg.saveFlag < _sysCfgH.saveFlag)
        spi_flash_read((CFG_LOCATION + 1) * SPI_FLASH_SEC_SIZE, (uint32*)&sysCfg, sizeof(SYSCFG));
      interrupts();
      snprintf_P(log, sizeof(log), PSTR("Config: Loaded configuration from flash at %X and count %d"), CFG_LOCATION + (sysCfg.saveFlag &1), sysCfg.saveFlag);
      addLog(LOG_LEVEL_DEBUG, log);
    }
  }
  if (sysCfg.cfg_holder != CFG_HOLDER) CFG_Default();
  _cfgHash = getHash();
}

void CFG_Erase()
{
  char log[LOGSZ];
  SpiFlashOpResult result;

  uint32_t _sectorStart = (ESP.getSketchSize() / SPI_FLASH_SEC_SIZE) + 1;
  uint32_t _sectorEnd = ESP.getFlashChipRealSize() / SPI_FLASH_SEC_SIZE;
  boolean _serialoutput = (LOG_LEVEL_DEBUG_MORE <= sysCfg.seriallog_level);

  snprintf_P(log, sizeof(log), PSTR("Config: Erasing %d flash sectors"), _sectorEnd - _sectorStart);
  addLog(LOG_LEVEL_DEBUG, log);

  for (uint32_t _sector = _sectorStart; _sector < _sectorEnd; _sector++) {
    noInterrupts();
    result = spi_flash_erase_sector(_sector);
    interrupts();
    if (_serialoutput) {
      Serial.print(F("Flash: Erased sector "));
      Serial.print(_sector);
      if (result == SPI_FLASH_RESULT_OK) {
        Serial.println(F(" OK"));
      } else {
        Serial.println(F(" Error"));
      }
      delay(10);
    }
  }
}

boolean spiffsPresent()
{
  return (SPIFFS_END - SPIFFS_START);
}

#ifdef USE_SPIFFS
void initSpiffs()
{
  spiffsflag = 0;
  if (!spiffsPresent()) {
    spiffsflag = 1;
  } else {
    if (!SPIFFS.begin()) {
      addLog_P(LOG_LEVEL_ERROR, PSTR("SPIFFS: WARNING - Failed to mount file system. Will use flash"));
      spiffsflag = 2;
    } else {
      addLog_P(LOG_LEVEL_DEBUG, PSTR("SPIFFS: Mount successful"));
      File f = SPIFFS.open(SPIFFS_CONFIG, "r");
      if (!f) {
        addLog_P(LOG_LEVEL_DEBUG, PSTR("SPIFFS: Formatting..."));
        SPIFFS.format();
        addLog_P(LOG_LEVEL_DEBUG, PSTR("SPIFFS: Formatted"));
        File f = SPIFFS.open(SPIFFS_CONFIG, "w");
        if (f) {
          for (int i = 0; i < sizeof(SYSCFG); i++) f.write(0);
          f.close();
        } else {
          addLog_P(LOG_LEVEL_ERROR, PSTR("SPIFFS: WARNING - Failed to init config file. Will use flash"));
          spiffsflag = 3;
        }
      }
    }
  }  
}
#endif  // USE_SPIFFS

/*********************************************************************************************\
 * Wifi
\*********************************************************************************************/

#define WIFI_CONFIG_SEC   60   // seconds before restart
#define WIFI_MANAGER_SEC  120  // seconds before restart
#define WIFI_CHECKSEC     20   // seconds
#define WIFI_RETRY        16

uint8_t _wificounter, _wifiretry, _wpsresult, _wificonfigflag = 0, _wifiConfigCounter = 0;

int WIFI_getRSSIasQuality(int RSSI)
{
  int quality = 0;

  if (RSSI <= -100) {
    quality = 0;
  } else if (RSSI >= -50) {
    quality = 100;
  } else {
    quality = 2 * (RSSI + 100);
  }
  return quality;
}

boolean WIFI_configCounter()
{
  if (_wifiConfigCounter) _wifiConfigCounter = WIFI_MANAGER_SEC;
  return (_wifiConfigCounter);
}

extern "C" {
#include "user_interface.h"
}

void WIFI_wps_status_cb(wps_cb_status status);

void WIFI_wps_status_cb(wps_cb_status status)
{
  char log[LOGSZ];

/* from user_interface.h:
  enum wps_cb_status {
    WPS_CB_ST_SUCCESS = 0,
    WPS_CB_ST_FAILED,
    WPS_CB_ST_TIMEOUT,
    WPS_CB_ST_WEP,      // WPS failed because that WEP is not supported
    WPS_CB_ST_SCAN_ERR, // can not find the target WPS AP
  };
*/

  _wpsresult = status;
  if (_wpsresult == WPS_CB_ST_SUCCESS) {
    wifi_wps_disable();
  } else {
    snprintf_P(log, sizeof(log), PSTR("WPSconfig: FAILED with status %d"), _wpsresult);
    addLog(LOG_LEVEL_DEBUG, log);
    _wifiConfigCounter = 2;
  }
}

boolean WIFI_WPSConfigDone(void)
{
  return (!_wpsresult);
}

boolean WIFI_beginWPSConfig(void)
{
  _wpsresult = 99;
  WiFi.disconnect();
  if (!wifi_wps_disable()) return false;
  if (!wifi_wps_enable(WPS_TYPE_PBC)) return false;  // so far only WPS_TYPE_PBC is supported (SDK 2.0.0)
  if (!wifi_set_wps_cb((wps_st_cb_t) &WIFI_wps_status_cb)) return false;
  if (!wifi_wps_start()) return false;
  return true;
}

void WIFI_config(int type)
{
  if (!_wificonfigflag) {
    _wificonfigflag = type;
    _wifiConfigCounter = WIFI_CONFIG_SEC;   // Allow up to WIFI_CONFIG_SECS seconds for phone to provide ssid/pswd
    _wificounter = _wifiConfigCounter +5;
    blinks = 1999;
    if (_wificonfigflag == WIFI_SMARTCONFIG) {
      addLog_P(LOG_LEVEL_INFO, PSTR("Smartconfig: Active for 1 minute"));
      WiFi.beginSmartConfig();
    }
    else if (_wificonfigflag == WIFI_WPSCONFIG) {
      if (WIFI_beginWPSConfig()) {
        addLog_P(LOG_LEVEL_INFO, PSTR("WPSconfig: Active for 1 minute"));
      } else {
        addLog_P(LOG_LEVEL_INFO, PSTR("WPSconfig: Failed to start"));
        _wifiConfigCounter = 3;
      }
    }
#ifdef USE_WEBSERVER
    else if (_wificonfigflag == WIFI_MANAGER) {
      addLog_P(LOG_LEVEL_INFO, PSTR("Wifimanager: Active for 1 minute for initial request"));
      beginWifiManager();
    }
#endif  // USE_WEBSERVER
  }
}

void WIFI_check_ip()
{
  if ((WiFi.status() == WL_CONNECTED) && (static_cast<uint32_t>(WiFi.localIP()) != 0)) {
    _wificounter = WIFI_CHECKSEC;
    _wifiretry = WIFI_RETRY;
    addLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("Wifi: Connected"));
  } else {
    switch (WiFi.status()) {
      case WL_NO_SSID_AVAIL:
      case WL_CONNECT_FAILED:
        addLog_P(LOG_LEVEL_DEBUG, PSTR("Wifi: STATION_CONNECT_FAIL"));
        WIFI_config(sysCfg.sta_config);
        break;
      default:
        addLog_P(LOG_LEVEL_DEBUG, PSTR("Wifi: STATION_IDLE"));
        if ((_wifiretry == (WIFI_RETRY / 2)) && (WiFi.status() != WL_CONNECTED)) {
//          WiFi.begin();
          WiFi.begin(sysCfg.sta_ssid, sysCfg.sta_pwd);
        }
        _wifiretry--;
        if (_wifiretry) {
          _wificounter = 1;
        } else {
          WIFI_config(sysCfg.sta_config);
        }
        break;
    }
  }
}

void WIFI_Check(int param)
{
  char log[LOGSZ];
  
  _wificounter--;
  switch (param) {
  case WIFI_SMARTCONFIG:
  case WIFI_MANAGER:
  case WIFI_WPSCONFIG:
    WIFI_config(param);
    break;
  default:
    if (_wifiConfigCounter) {
      _wifiConfigCounter--;
      _wificounter = _wifiConfigCounter +5;
      if (_wifiConfigCounter) {
        if ((_wificonfigflag == WIFI_SMARTCONFIG) && WiFi.smartConfigDone()) _wifiConfigCounter = 0;
        if ((_wificonfigflag == WIFI_WPSCONFIG) && WIFI_WPSConfigDone()) _wifiConfigCounter = 0;
        if (!_wifiConfigCounter) {
          if (strlen(WiFi.SSID().c_str())) strlcpy(sysCfg.sta_ssid, WiFi.SSID().c_str(), sizeof(sysCfg.sta_ssid));
          if (strlen(WiFi.psk().c_str())) strlcpy(sysCfg.sta_pwd, WiFi.psk().c_str(), sizeof(sysCfg.sta_pwd));
          snprintf_P(log, sizeof(log), PSTR("Wificonfig: SSID %s and Password %s"), sysCfg.sta_ssid, sysCfg.sta_pwd);
          addLog(LOG_LEVEL_INFO, log);
        }
      }
      if (!_wifiConfigCounter) {
        if (_wificonfigflag == WIFI_SMARTCONFIG) WiFi.stopSmartConfig();
        restartflag = 2;     
      }
    } else {
      if (_wificounter <= 0) {
        addLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("Wifi: Checking connection..."));
        _wificounter = WIFI_CHECKSEC;
        WIFI_check_ip();
      }
#ifdef USE_WEBSERVER
      if ((WiFi.status() == WL_CONNECTED) && (static_cast<uint32_t>(WiFi.localIP()) != 0) && !_wificonfigflag) {
        if (sysCfg.webserver) {
          startWebserver(sysCfg.webserver, WiFi.localIP());
        } else {
          stopWebserver();
        }
      }
#endif  // USE_WEBSERVER
    }
  }
}

int WIFI_State()
{
  int state;

  if ((WiFi.status() == WL_CONNECTED) && (static_cast<uint32_t>(WiFi.localIP()) != 0)) state = WIFI_STATUS;
  if (_wificonfigflag) state = _wificonfigflag;
  return state;
}

const char PhyMode[] = " BGN";

void WIFI_Connect(char *Hostname)
{
  char log[LOGSZ];

  WiFi.persistent(false);   // Solve possible wifi init errors
//  WiFi.setPhyMode(WIFI_PHY_MODE_11N);
  if (!strncmp(ESP.getSdkVersion(),"1.5.3",5)) {
    addLog_P(LOG_LEVEL_DEBUG, "Wifi: Patch issue 2186");
    WiFi.mode(WIFI_OFF);    // See https://github.com/esp8266/Arduino/issues/2186
  }
  WiFi.mode(WIFI_STA);      // Disable AP mode
  WiFi.disconnect();
  WiFi.setAutoConnect(true);
  WiFi.hostname(Hostname);
  WiFi.begin(sysCfg.sta_ssid, sysCfg.sta_pwd);
  snprintf_P(log, sizeof(log), PSTR("Wifi: Connecting to %s (%s) in mode 11%c as %s"), sysCfg.sta_ssid, sysCfg.sta_pwd, PhyMode[WiFi.getPhyMode() & 0x3], Hostname);
  addLog(LOG_LEVEL_INFO, log);
  _wifiretry = WIFI_RETRY;
  _wificounter = 1;
}

/*********************************************************************************************\
 * Real Time Clock
 * 
 * Sources: Time by Michael Margolis and Paul Stoffregen (https://github.com/PaulStoffregen/Time)
 *          Timezone by Jack Christensen (https://github.com/JChristensen/Timezone)
\*********************************************************************************************/

extern "C" {
#include "sntp.h"
}

#define SECS_PER_MIN  ((uint32_t)(60UL))
#define SECS_PER_HOUR ((uint32_t)(3600UL))
#define SECS_PER_DAY  ((uint32_t)(SECS_PER_HOUR * 24UL))
#define LEAP_YEAR(Y)  (((1970+Y)>0) && !((1970+Y)%4) && (((1970+Y)%100) || !((1970+Y)%400)))

Ticker tickerRTC;

static const uint8_t monthDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // API starts months from 1, this array starts from 0
static const char monthNames[37] = { "JanFebMrtAprMayJunJulAugSepOctNovDec" };

uint32_t utctime = 0, loctime = 0, dsttime = 0, stdtime = 0, ntptime = 0, midnight = 0;
uint8_t ntpsync = 0;

rtcCallback rtcCb = NULL;

void breakTime(uint32_t timeInput, TIME_T &tm)
{
// break the given timeInput into time components
// this is a more compact version of the C library localtime function
// note that year is offset from 1970 !!!

  uint8_t year, month, monthLength;
  uint32_t time;
  unsigned long days;

  time = timeInput;
  tm.Second = time % 60;
  time /= 60;                // now it is minutes
  tm.Minute = time % 60;
  time /= 60;                // now it is hours
  tm.Hour = time % 24;
  time /= 24;                // now it is days
  tm.Wday = ((time + 4) % 7) + 1;  // Sunday is day 1 
  
  year = 0;  
  days = 0;
  while((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
    year++;
  }
  tm.Year = year;            // year is offset from 1970 
  
  days -= LEAP_YEAR(year) ? 366 : 365;
  time -= days;              // now it is days in this year, starting at 0
  
  days = 0;
  month = 0;
  monthLength = 0;
  for (month = 0; month < 12; month++) {
    if (month == 1) { // february
      if (LEAP_YEAR(year)) {
        monthLength = 29;
      } else {
        monthLength = 28;
      }
    } else {
      monthLength = monthDays[month];
    }
    
    if (time >= monthLength) {
      time -= monthLength;
    } else {
      break;
    }
  }
  strlcpy(tm.MonthName, monthNames + (month *3), 4);
  tm.Month = month + 1;      // jan is month 1  
  tm.Day = time + 1;         // day of month
}

uint32_t makeTime(TIME_T &tm)
{   
// assemble time elements into time_t 
// note year argument is offset from 1970
  
  int i;
  uint32_t seconds;

  // seconds from 1970 till 1 jan 00:00:00 of the given year
  seconds = tm.Year * (SECS_PER_DAY * 365);
  for (i = 0; i < tm.Year; i++) {
    if (LEAP_YEAR(i)) {
      seconds +=  SECS_PER_DAY;   // add extra days for leap years
    }
  }
  
  // add days for this year, months start from 1
  for (i = 1; i < tm.Month; i++) {
    if ((i == 2) && LEAP_YEAR(tm.Year)) { 
      seconds += SECS_PER_DAY * 29;
    } else {
      seconds += SECS_PER_DAY * monthDays[i-1];  // monthDay array starts from 0
    }
  }
  seconds+= (tm.Day - 1) * SECS_PER_DAY;
  seconds+= tm.Hour * SECS_PER_HOUR;
  seconds+= tm.Minute * SECS_PER_MIN;
  seconds+= tm.Second;
  return seconds; 
}

uint32_t toTime_t(TimeChangeRule r, int yr)
{
    TIME_T tm;
    uint32_t t;
    uint8_t m, w;            // temp copies of r.month and r.week

    m = r.month;
    w = r.week;
    if (w == 0) {            // Last week = 0
        if (++m > 12) {      // for "Last", go to the next month
            m = 1;
            yr++;
        }
        w = 1;               // and treat as first week of next month, subtract 7 days later
    }

    tm.Hour = r.hour;
    tm.Minute = 0;
    tm.Second = 0;
    tm.Day = 1;
    tm.Month = m;
    tm.Year = yr - 1970;
    t = makeTime(tm);        // First day of the month, or first day of next month for "Last" rules
    breakTime(t, tm); 
    t += (7 * (w - 1) + (r.dow - tm.Wday + 7) % 7) * SECS_PER_DAY;
    if (r.week == 0) t -= 7 * SECS_PER_DAY;    //back up a week if this is a "Last" rule
    return t;
}

String rtc_time(int type)
{
  char stime[25];   // Skip newline

  uint32_t time = utctime;
  if (type == 1) time = loctime;
  if (type == 2) time = dsttime;
  if (type == 3) time = stdtime;
  snprintf_P(stime, sizeof(stime), PSTR("%s"), sntp_get_real_time(time));
  return String(stime);
}

uint32_t rtc_loctime()
{
  return loctime;
}

uint32_t rtc_midnight()
{
  return midnight;
}

void rtc_second()
{
  char log[LOGSZ];
  uint32_t stdoffset, dstoffset;
  TIME_T tmpTime;

  // NTP Sync every hour at x:0:10
  if (rtcTime.Minute == 0) {
    if ((rtcTime.Second > 9) && !ntpsync) {
      ntpsync = 1;
      ntptime = sntp_get_current_timestamp();
      if (ntptime) {
        utctime = ntptime;
        breakTime(utctime, tmpTime);
        rtcTime.Year = tmpTime.Year + 1970;
        dsttime = toTime_t(myDST, rtcTime.Year);
        stdtime = toTime_t(mySTD, rtcTime.Year);
        snprintf_P(log, sizeof(log), PSTR("RTC: (UTC) %s"), rtc_time(0).c_str());
        addLog(LOG_LEVEL_DEBUG, log);
        snprintf_P(log, sizeof(log), PSTR("RTC: (DST) %s"), rtc_time(2).c_str());
        addLog(LOG_LEVEL_DEBUG, log);
        snprintf_P(log, sizeof(log), PSTR("RTC: (STD) %s"), rtc_time(3).c_str());
        addLog(LOG_LEVEL_DEBUG, log);
      }
    }
  } else {
    ntpsync = 0;
  }
  utctime++;
  loctime = utctime;
  if (loctime > 1451602800) {  // 2016-01-01
    if (sysCfg.timezone == 99) {
      dstoffset = myDST.offset * SECS_PER_MIN;
      stdoffset = mySTD.offset * SECS_PER_MIN;
      if ((utctime >= (dsttime - stdoffset)) && (utctime < (stdtime - dstoffset))) {
        loctime += dstoffset;  // Daylight Saving Time
      } else {
        loctime += stdoffset;  // Standard Time
      }
    } else {
      loctime += sysCfg.timezone * SECS_PER_HOUR;
    }
  }
  breakTime(loctime, rtcTime);
  if ((!midnight || (!rtcTime.Hour && !rtcTime.Minute && !rtcTime.Second)) && (loctime > 1451602800)) {
    midnight = loctime;
  }
  rtcTime.Year += 1970;
#ifdef USE_POWERMONITOR
  hlw_second();
#endif
  if (rtcCb) rtcCb();
}

void rtc_init(rtcCallback cb)
{
  rtcCb = cb;
  sntp_setservername(0, (char*)NTP_SERVER1);
  sntp_setservername(1, (char*)NTP_SERVER2);
  sntp_setservername(2, (char*)NTP_SERVER3);
  sntp_stop();
  sntp_set_timezone(0);      // UTC time
  sntp_init();
  utctime = 0;
  tickerRTC.attach(1, rtc_second);
}

#ifdef SEND_TELEMETRY_DS18B20
/*********************************************************************************************\
 * DS18B20 - Temperature
 * 
 * Source: Marinus vd Broek https://github.com/ESP8266nu/ESPEasy
\*********************************************************************************************/

uint8_t dsb_reset()
{
  uint8_t r;
  uint8_t retries = 125;
  
  pinMode(DSB_PIN, INPUT);
  do  {                                 // wait until the wire is high... just in case
    if (--retries == 0) return 0;
    delayMicroseconds(2);
  } while (!digitalRead(DSB_PIN));
  pinMode(DSB_PIN, OUTPUT);
  digitalWrite(DSB_PIN, LOW);
  delayMicroseconds(492);               // Dallas spec. = Min. 480uSec. Arduino 500uSec.
  pinMode(DSB_PIN, INPUT);              // Float
  delayMicroseconds(40);
  r = !digitalRead(DSB_PIN);
  delayMicroseconds(420);
  return r;
}

uint8_t dsb_read_bit(void)
{
  uint8_t r;

  pinMode(DSB_PIN, OUTPUT);
  digitalWrite(DSB_PIN, LOW);
  delayMicroseconds(3);
  pinMode(DSB_PIN, INPUT);             // let pin float, pull up will raise
  delayMicroseconds(10);
  r = digitalRead(DSB_PIN);
  delayMicroseconds(53);
  return r;
}

uint8_t dsb_read(void)
{
  uint8_t bitMask;
  uint8_t r = 0;

  for (bitMask = 0x01; bitMask; bitMask <<= 1)
    if (dsb_read_bit()) r |= bitMask;
  return r;
}

void dsb_write_bit(uint8_t v)
{
  if (v & 1) {
    digitalWrite(DSB_PIN, LOW);
    pinMode(DSB_PIN, OUTPUT);
    delayMicroseconds(10);
    digitalWrite(DSB_PIN, HIGH);
    delayMicroseconds(55);
  } else {
    digitalWrite(DSB_PIN, LOW);
    pinMode(DSB_PIN, OUTPUT);
    delayMicroseconds(65);
    digitalWrite(DSB_PIN, HIGH);
    delayMicroseconds(5);
  }
}

void dsb_write(uint8_t ByteToWrite)
{
  uint8_t bitMask;
  
  for (bitMask = 0x01; bitMask; bitMask <<= 1)
    dsb_write_bit((bitMask & ByteToWrite) ? 1 : 0);
}

void dsb_readTempPrep()
{
  dsb_reset();
  dsb_write(0xCC);           // Skip ROM
  dsb_write(0x44);           // Start conversion
}

boolean dsb_readTemp(float &t)
{
  int16_t DSTemp;
  byte msb, lsb;

  t = NAN;

/*
  dsb_reset();
  dsb_write(0xCC);           // Skip ROM
  dsb_write(0x44);           // Start conversion
  delay(800);
*/
  dsb_reset();
  dsb_write(0xCC);           // Skip ROM
  dsb_write(0xBE);           // Read scratchpad
  lsb = dsb_read();
  msb = dsb_read();
  dsb_reset();
  
  DSTemp = (msb << 8) + lsb;
  t = (float(DSTemp) * 0.0625);
  return (!isnan(t));
}
#endif  // SEND_TELEMETRY_DS18B20

#ifdef SEND_TELEMETRY_DHT
/*********************************************************************************************\
 * DHT11, DHT21 (AM2301), DHT22 (AM2302, AM2321) - Temperature and Humidy
 * 
 * Reading temperature or humidity takes about 250 milliseconds!
 * Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
 * Source: Adafruit Industries https://github.com/adafruit/DHT-sensor-library
\*********************************************************************************************/

#define MIN_INTERVAL 2000

uint8_t data[5];
uint32_t _lastreadtime, _maxcycles;
bool _lastresult;

void dht_readPrep()
{
  digitalWrite(DHT_PIN, HIGH);
}

uint32_t dht_expectPulse(bool level)
{
  uint32_t count = 0;
  
  while (digitalRead(DHT_PIN) == level)
    if (count++ >= _maxcycles) return 0;
  return count;
}

boolean dht_read()
{
  char log[LOGSZ];
  uint32_t cycles[80];
  uint32_t currenttime = millis();
  
  if ((currenttime - _lastreadtime) < 2000) {
    return _lastresult;
  }
  _lastreadtime = currenttime;

  data[0] = data[1] = data[2] = data[3] = data[4] = 0;

//  digitalWrite(DHT_PIN, HIGH);
//  delay(250);

  pinMode(DHT_PIN, OUTPUT);
  digitalWrite(DHT_PIN, LOW);
  delay(20);

  noInterrupts();
  digitalWrite(DHT_PIN, HIGH);
  delayMicroseconds(40);
  pinMode(DHT_PIN, INPUT_PULLUP);
  delayMicroseconds(10);
  if (dht_expectPulse(LOW) == 0) {
    addLog_P(LOG_LEVEL_DEBUG, PSTR("DHT: Timeout waiting for start signal low pulse"));
    _lastresult = false;
    return _lastresult;
  }
  if (dht_expectPulse(HIGH) == 0) {
    addLog_P(LOG_LEVEL_DEBUG, PSTR("DHT: Timeout waiting for start signal high pulse"));
    _lastresult = false;
    return _lastresult;
  }
  for (int i=0; i<80; i+=2) {
    cycles[i]   = dht_expectPulse(LOW);
    cycles[i+1] = dht_expectPulse(HIGH);
  }
  interrupts();

  for (int i=0; i<40; ++i) {
    uint32_t lowCycles  = cycles[2*i];
    uint32_t highCycles = cycles[2*i+1];
    if ((lowCycles == 0) || (highCycles == 0)) {
      addLog_P(LOG_LEVEL_DEBUG, PSTR("DHT: Timeout waiting for pulse"));
      _lastresult = false;
      return _lastresult;
    }
    data[i/8] <<= 1;
    if (highCycles > lowCycles) data[i/8] |= 1;
  }

  snprintf_P(log, sizeof(log), PSTR("DHT: Received %02X, %02X, %02X, %02X, %02X =? %02X"),
    data[0], data[1], data[2], data[3], data[4], (data[0] + data[1] + data[2] + data[3]) & 0xFF);
  addLog(LOG_LEVEL_DEBUG, log);

  if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
    _lastresult = true;
    return _lastresult;
  } else {
    addLog_P(LOG_LEVEL_DEBUG, PSTR("DHT: Checksum failure"));
    _lastresult = false;
    return _lastresult;
  }
}

float dht_convertCtoF(float c)
{
  return c * 1.8 + 32;
}

boolean dht_readTempHum(bool S, float &t, float &h)
{
  t = NAN;
  h = NAN;

  if (dht_read()) {
    switch (DHT_TYPE) {
    case DHT11:
      h = data[0];
      t = data[2];
      if(S) t = dht_convertCtoF(t);
      break;
    case DHT22:
    case DHT21:
      h = data[0];
      h *= 256;
      h += data[1];
      h *= 0.1;
      t = data[2] & 0x7F;
      t *= 256;
      t += data[3];
      t *= 0.1;
      if (data[2] & 0x80) t *= -1;
      if(S) t = dht_convertCtoF(t);
      break;
    }
  }
  return (!isnan(t) && !isnan(h)); 
}

void dht_init()
{
  char log[LOGSZ];
  _maxcycles = microsecondsToClockCycles(1000);  // 1 millisecond timeout for
                                                 // reading pulses from DHT sensor.
  pinMode(DHT_PIN, INPUT_PULLUP);
  _lastreadtime = -MIN_INTERVAL;

  snprintf_P(log, sizeof(log), PSTR("DHT: Max clock cycles %d"), _maxcycles);
  addLog(LOG_LEVEL_DEBUG, log);
}
#endif  // SEND_TELEMETRY_DHT

/*********************************************************************************************\
 * Syslog
\*********************************************************************************************/

void syslog(const char *message)
{
  char mess[MESSZ], str[TOPSZ+MESSZ];

  portUDP.beginPacket(sysCfg.syslog_host, sysCfg.syslog_port);
  strlcpy(mess, message, sizeof(mess));
  mess[sizeof(mess)-1] = 0;
  snprintf_P(str, sizeof(str), PSTR("%s ESP-%s"), Hostname, mess);
  portUDP.write(str);
  portUDP.endPacket();
}

void addLog(byte loglevel, const char *line)
{
  char mxtime[9];
  
  snprintf_P(mxtime, sizeof(mxtime), PSTR("%02d:%02d:%02d"), rtcTime.Hour, rtcTime.Minute, rtcTime.Second);
  
#ifdef DEBUG_ESP_PORT
  DEBUG_ESP_PORT.printf("%s %s\n", mxtime, line);  
//  yield();
#endif  // DEBUG_ESP_PORT
  if (loglevel <= sysCfg.seriallog_level) Serial.printf("%s %s\n", mxtime, line);
//  yield();
#ifdef USE_WEBSERVER
  if (loglevel <= sysCfg.weblog_level) {
    Log[logidx] = String(mxtime) + " " + String(line);
    logidx++;
    if (logidx > MAX_LOG_LINES -1) logidx = 0;
  }
#endif  // USE_WEBSERVER
  if ((WiFi.status() == WL_CONNECTED) && (loglevel <= sysCfg.syslog_level)) syslog(line);
//  yield();
}

void addLog_P(byte loglevel, const char *formatP)
{
  char mess[MESSZ];
  
  snprintf_P(mess, sizeof(mess), formatP);
  addLog(loglevel, mess);
}

/*********************************************************************************************\
 * 
\*********************************************************************************************/

