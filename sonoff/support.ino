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
\*********************************************************************************************/

#define NTP_SERVER1 "pool.ntp.org"
#define NTP_SERVER2 "nl.pool.ntp.org"
#define NTP_SERVER3 "0.nl.pool.ntp.org"

extern "C" {
#include "sntp.h"
}

#define LEAP_YEAR(Y) ( ((1970+Y)>0) && !((1970+Y)%4) && ( ((1970+Y)%100) || !((1970+Y)%400) ) )

static const uint8_t monthDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // API starts months from 1, this array starts from 0

static const char monthNames[37] = { "JanFebMrtAprMayJunJulAugSepOctNovDec" };

uint32_t myrtc = 0;
uint8_t ntpsync = 0;

void convertTime()
{
  // given myrtc as time in Linux format break into time components
  // this is a more compact version of the C library localtime function
  // note that internally the year is offset from 1970 - compensated at the end

  uint8_t year;
  uint8_t month, monthLength;
  uint32_t time;
  unsigned long days;

  time = (uint32_t) myrtc;
  rtcTime.Second = time % 60;
  time /= 60; // now it is minutes
  rtcTime.Minute = time % 60;
  time /= 60; // now it is hours
  rtcTime.Hour = time % 24;
  time /= 24; // now it is days
  rtcTime.Wday = ((time + 4) % 7) + 1;  // Sunday is day 1

  year = 0;
  days = 0;
  while ((unsigned) (days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
    year++;
  }
  rtcTime.Year = year; // year is offset from 1970

  days -= LEAP_YEAR(year) ? 366 : 365;
  time -= days; // now it is days in this year, starting at 0

  days = 0;
  month = 0;
  monthLength = 0;
  for (month = 0; month < 12; month++) {
    if (month == 1) { // february
      if (LEAP_YEAR(year)) {
        monthLength = 29;
      }
      else {
        monthLength = 28;
      }
    }
    else {
      monthLength = monthDays[month];
    }

    if (time >= monthLength) {
      time -= monthLength;
    }
    else {
      break;
    }
  }
  strlcpy(rtcTime.MonthName, monthNames +(month *3), sizeof(rtcTime.MonthName));
  rtcTime.Month = month + 1;  // jan is month 1
  rtcTime.Day = time + 1;     // day of month
  rtcTime.Year = rtcTime.Year + 1970;
}

void rtc_second()
{
  char log[LOGSZ];
  
  // NTP Sync every hour at x:0:10
  if (rtcTime.Minute == 0) {
    if ((rtcTime.Second >= 10) && !ntpsync) {
      myrtc = sntp_get_current_timestamp();
      ntpsync = (myrtc) ? 1 : 0;
      snprintf_P(log, sizeof(log), PSTR("RTC: sntp %d, %s"), myrtc, sntp_get_real_time(myrtc));
      addLog(LOG_LEVEL_DEBUG, log);
    }
    if (rtcTime.Second == 40) ntpsync = 0;
  }
  myrtc++;
  convertTime();
}

void rtc_timezone(uint8_t timezone)
{
  sntp_stop();
//  if (true == sntp_set_timezone(sysCfg.timezone))
  sntp_set_timezone(timezone);
  sntp_init();
  myrtc = sntp_get_current_timestamp();
  ntpsync = 0;
}

void rtc_init(uint8_t timezone)
{
  sntp_setservername(0, NTP_SERVER1);
  sntp_setservername(1, NTP_SERVER2);
//  sntp_setservername(2, NTP_SERVER3);
  sntp_stop();
//  if (true == sntp_set_timezone(sysCfg.timezone))
  sntp_set_timezone(timezone);
  sntp_init();
  myrtc = 0;
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
