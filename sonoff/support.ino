/*********************************************************************************************\
 * Config
\*********************************************************************************************/

extern "C" {
#include "spi_flash.h"
}
extern "C" uint32_t _SPIFFS_end;
#define CFG_LOCATION (((uint32_t)&_SPIFFS_end - 0x40200000) / SPI_FLASH_SEC_SIZE) - 2  // End of SPIFFS area

void CFG_Save()
{
  char log[LOGSZ];
  
  if (memcmp(&myCfg, &sysCfg, sizeof(SYSCFG))) {
    noInterrupts();
    if (sysCfg.saveFlag == 0) {  // Handle default and rollover
      spi_flash_erase_sector(CFG_LOCATION + (sysCfg.saveFlag &1));
      spi_flash_write((CFG_LOCATION + (sysCfg.saveFlag &1)) * SPI_FLASH_SEC_SIZE, (uint32 *)&sysCfg, sizeof(SYSCFG));
    }
    sysCfg.saveFlag++;
    spi_flash_erase_sector(CFG_LOCATION + (sysCfg.saveFlag &1));
    spi_flash_write((CFG_LOCATION + (sysCfg.saveFlag &1)) * SPI_FLASH_SEC_SIZE, (uint32 *)&sysCfg, sizeof(SYSCFG));
    interrupts();
    snprintf_P(log, sizeof(log), PSTR("Config: Saved configuration to flash at %X and count %d"), CFG_LOCATION + (sysCfg.saveFlag &1), sysCfg.saveFlag);
    addLog(LOG_LEVEL_DEBUG, log);
    myCfg = sysCfg;
  }
}

void CFG_Load()
{
  char log[LOGSZ];

  noInterrupts();
  spi_flash_read((CFG_LOCATION) * SPI_FLASH_SEC_SIZE, (uint32 *)&sysCfg, sizeof(SYSCFG));
  spi_flash_read((CFG_LOCATION + 1) * SPI_FLASH_SEC_SIZE, (uint32 *)&myCfg, sizeof(SYSCFG));
  interrupts();
  if (sysCfg.saveFlag < myCfg.saveFlag) sysCfg = myCfg;
  snprintf_P(log, sizeof(log), PSTR("Config: Loaded configuration from flash at %X and count %d"), CFG_LOCATION + (sysCfg.saveFlag &1), sysCfg.saveFlag);
  addLog(LOG_LEVEL_DEBUG, log);
  if (sysCfg.cfg_holder != CFG_HOLDER) CFG_Default();
  myCfg = sysCfg;
}

void CFG_Erase()
{
  char log[LOGSZ];
  SpiFlashOpResult result;

  uint32_t _sectorStart = (ESP.getSketchSize() / SPI_FLASH_SEC_SIZE) + 1;
  uint32_t _sectorEnd = ESP.getFlashChipRealSize() / SPI_FLASH_SEC_SIZE;
  byte seriallog_level = sysCfg.seriallog_level;

  snprintf_P(log, sizeof(log), PSTR("Config: Erasing %d flash sectors"), _sectorEnd - _sectorStart);
  addLog(LOG_LEVEL_DEBUG, log);

  for (uint32_t _sector = _sectorStart; _sector < _sectorEnd; _sector++) {
    noInterrupts();
    result = spi_flash_erase_sector(_sector);
    interrupts();
    if (LOG_LEVEL_DEBUG_MORE <= seriallog_level) {
      Serial.print(F("Flash: Erased sector "));
      Serial.print(_sector);
      if (result == SPI_FLASH_RESULT_OK)
        Serial.println(F(" OK"));
      else
        Serial.println(F(" Error"));
      delay(10);
    }
  }
}

/*********************************************************************************************\
 * Wifi
\*********************************************************************************************/

#define WIFI_SMARTSEC  60   // seconds
#define WIFI_CHECKSEC  20   // seconds
#define WIFI_RETRY     16

uint8_t wificounter;
uint8_t wifiretry = WIFI_RETRY;
uint8_t smartcounter = 0;

void WIFI_smartconfig()
{
  smartcounter = WIFI_SMARTSEC;   // Allow up to WIFI_SMARTSECS seconds for phone to provide ssid/pswd
  wificounter = smartcounter +5;
  addLog(LOG_LEVEL_INFO, "Smartconfig: Started and active for 1 minute");
  WiFi.beginSmartConfig();
}

void WIFI_check_ip()
{
  if ((WiFi.status() == WL_CONNECTED) && (static_cast<uint32_t>(WiFi.localIP()) != 0)) {
    wificounter = WIFI_CHECKSEC;
    wifiretry = WIFI_RETRY;
    addLog(LOG_LEVEL_DEBUG_MORE, "Wifi: Connected");
  } else {
    switch (WiFi.status()) {
      case WL_NO_SSID_AVAIL:
      case WL_CONNECT_FAILED:
        addLog(LOG_LEVEL_DEBUG, "Wifi: STATION_CONNECT_FAIL");
        WIFI_smartconfig();
        break;
      default:
        addLog(LOG_LEVEL_DEBUG, "Wifi: STATION_IDLE");
        if (wifiretry == (WIFI_RETRY / 2)) WiFi.begin();
        wifiretry--;
        if (wifiretry)
          wificounter = 1;
        else
          WIFI_smartconfig();
        break;
    }
  }
}

void WIFI_Check(uint8_t param)
{
  char log[LOGSZ];
  
  wificounter--;
  switch (param) {
    case WIFI_SMARTCONFIG:
      WIFI_smartconfig();
      break;
    default:
      if (wificounter <= 0) {
        addLog(LOG_LEVEL_DEBUG_MORE, "Wifi: Checking connection...");
        wificounter = WIFI_CHECKSEC;
        WIFI_check_ip();
      }
      if (smartcounter) {
        smartcounter--;
        if (smartcounter) {
          if (WiFi.smartConfigDone()) {
            smartcounter = 0;
            memcpy(sysCfg.sta_ssid, WiFi.SSID().c_str(), strlen(WiFi.SSID().c_str())+1);
            memcpy(sysCfg.sta_pwd, WiFi.psk().c_str(), strlen(WiFi.psk().c_str())+1);
            snprintf_P(log, sizeof(log), PSTR("Smartconfig: SSID %s and Password %s"), sysCfg.sta_ssid, sysCfg.sta_pwd);
            addLog(LOG_LEVEL_INFO, log);
          }
        }
        if (smartcounter == 0) {
          WiFi.stopSmartConfig();
          restartflag = 2;     
        }
      }
      break;
  }
}

void WIFI_Connect(char *Hostname)
{
  char log[LOGSZ];

  WiFi.persistent(false);   // Solve possible wifi init errors
  WiFi.hostname(Hostname);
  snprintf_P(log, sizeof(log), PSTR("Wifi: Connecting to %s as %s"), sysCfg.sta_ssid, Hostname);
  addLog(LOG_LEVEL_DEBUG, log);
  WiFi.setAutoConnect(true);
  WiFi.mode(WIFI_STA);     // Disable AP mode
  WiFi.begin(sysCfg.sta_ssid, sysCfg.sta_pwd);
  wificounter = 1;
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

static const uint8_t monthDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // API starts months from 1, this array starts from 0
static const char monthNames[37] = { "JanFebMrtAprMayJunJulAugSepOctNovDec" };

uint32_t utctime = 0, loctime = 0, dsttime = 0, stdtime = 0;
uint8_t ntpsync = 0;

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

void rtc_second()
{
  char log[LOGSZ];
  uint32_t stdoffset, dstoffset;
  TIME_T tmpTime;
  
  utctime++;
  // NTP Sync every hour at x:0:10
  if (rtcTime.Minute == 0) {
    if ((rtcTime.Second >= 10) && !ntpsync) {
      utctime = sntp_get_current_timestamp();
      ntpsync = (utctime) ? 1 : 0;
      snprintf_P(log, sizeof(log), PSTR("RTC: sntp %d, (UTC) %s"), utctime, rtc_time(0).c_str());
      addLog(LOG_LEVEL_DEBUG, log);
      if (ntpsync) {
        breakTime(utctime, tmpTime);
        rtcTime.Year = tmpTime.Year + 1970;
        dsttime = toTime_t(myDST, rtcTime.Year);
        stdtime = toTime_t(mySTD, rtcTime.Year);
        snprintf_P(log, sizeof(log), PSTR("RTC:  DST %d, (DST) %s"), dsttime, rtc_time(2).c_str());
        addLog(LOG_LEVEL_DEBUG, log);
        snprintf_P(log, sizeof(log), PSTR("RTC:  STD %d, (STD) %s"), stdtime, rtc_time(3).c_str());
        addLog(LOG_LEVEL_DEBUG, log);
      }
    }
    if (rtcTime.Second == 40) ntpsync = 0;
  }
  loctime = utctime;
  if (sysCfg.timezone == 99) {
    if (loctime > 1451602800) {  // 2016-01-01
      dstoffset = myDST.offset * SECS_PER_MIN;
      stdoffset = mySTD.offset * SECS_PER_MIN;
      if ((utctime >= (dsttime - stdoffset)) && (utctime < (stdtime - dstoffset)))
        loctime += dstoffset;  // Daylight Saving Time
      else
        loctime += stdoffset;  // Standard Time
    }
  } else {
    loctime += sysCfg.timezone * SECS_PER_HOUR;
  }
  breakTime(loctime, rtcTime);
  rtcTime.Year = rtcTime.Year + 1970;
}

void rtc_init()
{
  sntp_setservername(0, NTP_SERVER1);
  sntp_setservername(1, NTP_SERVER2);
  sntp_setservername(2, NTP_SERVER3);
  sntp_stop();
  sntp_set_timezone(0);      // UTC time
  sntp_init();
  utctime = 0;
}

/*********************************************************************************************\
 * Syslog
\*********************************************************************************************/

void syslog(const char *message)
{
  char mess[MESSZ], str[TOPSZ+MESSZ];

  portUDP.beginPacket(sysCfg.syslog_host, SYS_LOG_PORT);
  strlcpy(mess, message, sizeof(mess));
  mess[sizeof(mess)-1] = 0;
  snprintf_P(str, sizeof(str), PSTR("%s %s"), Hostname, mess);
  portUDP.write(str);
  portUDP.endPacket();
}

void addLog(byte loglevel, const char *line)
{
  char mxtime[9];
  
  snprintf_P(mxtime, sizeof(mxtime), PSTR("%02d:%02d:%02d"), rtcTime.Hour, rtcTime.Minute, rtcTime.Second);
  
#ifdef DEBUG_ESP_PORT
  DEBUG_ESP_PORT.printf("%s %s\n", mxtime, line);  
#endif
#ifdef SERIAL_IO
  if (loglevel <= sysCfg.seriallog_level) Serial.printf("%s %s\n", mxtime, line);
#endif
  if ((WiFi.status() == WL_CONNECTED) && (loglevel <= sysCfg.syslog_level)) syslog(line);
}

void addLog(byte loglevel, String& string)
{
  addLog(loglevel, string.c_str());
}

/*********************************************************************************************\
 * 
\*********************************************************************************************/
