/*
  support_esp32.ino - ESP32 spezific code for Tasmota

  Copyright (C) 2020  Theo Arends / Jörg Schüler-Maroldt

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

#include <nvs.h>
#include <rom/rtc.h>

void SettingsErase(uint8_t type)
{
  if (1 == type) // SDK parameter area
  {
  }
  else if (2 == type) // Tasmota parameter area (0x0F3xxx - 0x0FBFFF) 
  {
  }
  else if (3 == type) // Tasmota and SDK parameter area (0x0F3xxx - 0x0FFFFF)
  {
  }

  noInterrupts();
  nvs_handle handle;
  nvs_open("main",  NVS_READWRITE, &handle);
  nvs_erase_all(handle);
  nvs_commit(handle);
  nvs_close(handle);
  interrupts();

  AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_ERASE " t=%d"), type);
}

void SettingsLoad(const char *sNvsName, const char *sName, void *pSettings, unsigned nSettingsLen)
{
    noInterrupts();
    nvs_handle handle;
    size_t size;
    nvs_open(sNvsName, NVS_READONLY, &handle);
    size = nSettingsLen;
    nvs_get_blob(handle, sName, pSettings, &size);
    nvs_close(handle);
    interrupts();
}

void SettingsSave(const char *sNvsName, const char *sName, const void *pSettings, unsigned nSettingsLen)
{
    nvs_handle handle;
    noInterrupts();
    nvs_open(sNvsName, NVS_READWRITE, &handle);
    nvs_set_blob(handle, sName, pSettings, nSettingsLen);
    nvs_commit(handle);
    nvs_close(handle);
    interrupts();
}

bool ESP_flashRead(uint32_t offset, uint32_t *data, size_t size)
{
    SettingsLoad("main", "Settings", data, size);    
}

bool ESP_flashReadHeader(uint32_t offset, uint32_t *data, size_t size)
{
    SettingsLoad("main", "SettingsH", data, size);    
}

void SettingsSaveMain(const void *pSettings, unsigned nSettingsLen)
{
    SettingsSave("main", "Settings", pSettings, nSettingsLen);
}

/*
void SettingsLoadMain(void *pSettings, unsigned nSettingsLen)
{
    SettingsLoad("main", "Settings", pSettings, nSettingsLen);
}

void SettingsLoadMainH(void *pSettingsH, unsigned nSettingsLenH)
{
    SettingsLoad("main", "SettingsH", pSettingsH, nSettingsLenH);
}
*/

void SettingsLoadUpg(void *pSettings, unsigned nSettingsLen)
{
    SettingsLoad("upg", "Settings", pSettings, nSettingsLen);
}

void SettingsLoadUpgH(void *pSettings, unsigned nSettingsLen)
{
    SettingsLoad("upg","SettingsH", pSettings, nSettingsLen);
}

//
// Time
//
static void obtain_time(void)
{
    // wait for time to be set
    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry = 0;
    const int retry_count = 10;
    while(timeinfo.tm_year < (2016 - 1900) && ++retry < retry_count) 
    {
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        time(&now);
        localtime_r(&now, &timeinfo);
    }
}

uint32_t sntp_get_current_timestamp(void)
{
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    // Is time set? If not, tm_year will be (1970 - 1900).
    if (timeinfo.tm_year < (2016 - 1900)) {
        obtain_time();
        // update 'now' variable with current time
        time(&now);
    }
    char strftime_buf[64];

    // Set timezone to Eastern Standard Time and print local time
    setenv("TZ", "EST5EDT,M3.2.0/2,M11.1.0", 1);
    tzset();
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);

    return now;
}

String GetTime(int type)
{
  char stime[25];   // Skip newline
//#warning "1,2,3 not ported"
  time_t now;
  time(&now);
  struct tm timeinfo;
  localtime_r(&now, &timeinfo);
  strftime(stime, sizeof(stime), "%c", &timeinfo); 
  return String(stime);
}

//
// Crash stuff
//

void CrashDump(void)
{

}

bool CrashFlag(void)
{
    return false;
}

void CrashDumpClear(void)
{

}
void CmndCrash(void)
{
/*
  volatile uint32_t dummy;
  dummy = *((uint32_t*) 0x00000000);
*/
}

// Do an infinite loop to trigger WDT watchdog
void CmndWDT(void)
{
/*
  volatile uint32_t dummy = 0;
  while (1) {
    dummy++;
  }
*/
}
// This will trigger the os watch after OSWATCH_RESET_TIME (=120) seconds
void CmndBlockedLoop(void)
{
/*
  while (1) {
    delay(1000);
  }
*/
}

#endif
