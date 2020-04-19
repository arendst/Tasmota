/*
  support_esp32.ino - ESP32 specific code for Tasmota

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

/*********************************************************************************************\
 * ESP8266 Support
\*********************************************************************************************/

#ifdef ESP8266

extern "C" {
extern struct rst_info resetInfo;
}

uint32_t ESP_ResetInfoReason(void)
{
  return resetInfo.reason;
}

String ESP_getResetReason(void)
{
  return ESP.getResetReason();
}

uint32_t ESP_getChipId(void)
{
  return ESP.getChipId();
}

uint32_t ESP_getSketchSize(void)
{
  return ESP.getSketchSize();
}

void ESP_Restart(void)
{
//  ESP.restart();            // This results in exception 3 on restarts on core 2.3.0
  ESP.reset();
}

#endif

/*********************************************************************************************\
 * ESP32 Support
\*********************************************************************************************/

#ifdef ESP32

#include <nvs.h>
#include <rom/rtc.h>

void NvmLoad(const char *sNvsName, const char *sName, void *pSettings, unsigned nSettingsLen)
{
  nvs_handle handle;
  noInterrupts();
  nvs_open(sNvsName, NVS_READONLY, &handle);
  size_t size = nSettingsLen;
  nvs_get_blob(handle, sName, pSettings, &size);
  nvs_close(handle);
  interrupts();
}

void NvmSave(const char *sNvsName, const char *sName, const void *pSettings, unsigned nSettingsLen)
{
  nvs_handle handle;
  noInterrupts();
  nvs_open(sNvsName, NVS_READWRITE, &handle);
  nvs_set_blob(handle, sName, pSettings, nSettingsLen);
  nvs_commit(handle);
  nvs_close(handle);
  interrupts();
}

void NvmErase(const char *sNvsName)
{
  nvs_handle handle;
  noInterrupts();
  nvs_open(sNvsName, NVS_READWRITE, &handle);
  nvs_erase_all(handle);
  nvs_commit(handle);
  nvs_close(handle);
  interrupts();
}

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

  NvmErase("main");

  AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_ERASE " t=%d"), type);
}

void SettingsRead(void *data, size_t size)
{
  NvmLoad("main", "Settings", data, size);
}

void SettingsWrite(const void *pSettings, unsigned nSettingsLen)
{
  NvmSave("main", "Settings", pSettings, nSettingsLen);
}

void QPCRead(void *pSettings, unsigned nSettingsLen)
{
  NvmLoad("qpc", "pcreg", pSettings, nSettingsLen);
}

void QPCWrite(const void *pSettings, unsigned nSettingsLen)
{
  NvmSave("qpc", "pcreg", pSettings, nSettingsLen);
}

//
// sntp emulation
//
static bool bNetIsTimeSync = false;
//
void SntpInit()
{
  bNetIsTimeSync = true;
}

uint32_t SntpGetCurrentTimestamp(void)
{
  time_t now = 0;
  if (bNetIsTimeSync || ntp_force_sync)
  {
    //Serial_DebugX(("timesync configTime %d\n", ntp_force_sync, bNetIsTimeSync));
    // init to UTC Time
    configTime(0, 0, SettingsText(SET_NTPSERVER1), SettingsText(SET_NTPSERVER2), SettingsText(SET_NTPSERVER3));
    bNetIsTimeSync = false;
    ntp_force_sync = false;
  }
  time(&now);
  return now;
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

//
// ESP32 specific
//

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

void DisableBrownout(void)
{
  // https://github.com/espressif/arduino-esp32/issues/863#issuecomment-347179737
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);  // Disable brownout detector
}

//
// ESP32 Alternatives
//

String ESP32GetResetReason(uint32_t cpu_no)
{
	// tools\sdk\include\esp32\rom\rtc.h
  switch (rtc_get_reset_reason( (RESET_REASON) cpu_no)) {
    case POWERON_RESET          : return F("Vbat power on reset");                              // 1
    case SW_RESET               : return F("Software reset digital core");                      // 3
    case OWDT_RESET             : return F("Legacy watch dog reset digital core");              // 4
    case DEEPSLEEP_RESET        : return F("Deep Sleep reset digital core");                    // 5
    case SDIO_RESET             : return F("Reset by SLC module, reset digital core");          // 6
    case TG0WDT_SYS_RESET       : return F("Timer Group0 Watch dog reset digital core");        // 7
    case TG1WDT_SYS_RESET       : return F("Timer Group1 Watch dog reset digital core");        // 8
    case RTCWDT_SYS_RESET       : return F("RTC Watch dog Reset digital core");                 // 9
    case INTRUSION_RESET        : return F("Instrusion tested to reset CPU");                   // 10
    case TGWDT_CPU_RESET        : return F("Time Group reset CPU");                             // 11
    case SW_CPU_RESET           : return F("Software reset CPU");                               // 12
    case RTCWDT_CPU_RESET       : return F("RTC Watch dog Reset CPU");                          // 13
    case EXT_CPU_RESET          : return F("or APP CPU, reseted by PRO CPU");                   // 14
    case RTCWDT_BROWN_OUT_RESET : return F("Reset when the vdd voltage is not stable");         // 15
    case RTCWDT_RTC_RESET       : return F("RTC Watch dog reset digital core and rtc module");  // 16
    default                     : return F("NO_MEAN");                                          // 0
  }
}

String ESP_getResetReason(void)
{
  return ESP32GetResetReason(0);  // CPU 0
}

uint32_t ESP_ResetInfoReason(void)
{
  RESET_REASON reason = rtc_get_reset_reason(0);
  if (POWERON_RESET == reason) { return REASON_DEFAULT_RST; }
  if (SW_CPU_RESET == reason) { return REASON_SOFT_RESTART; }
  if (DEEPSLEEP_RESET == reason)  { return REASON_DEEP_SLEEP_AWAKE; }
  if (SW_RESET == reason) { return REASON_EXT_SYS_RST; }
}

uint32_t ESP_getChipId(void)
{
  uint32_t id = 0;
  for (uint32_t i = 0; i < 17; i = i +8) {
    id |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  return id;
}

uint32_t ESP_getSketchSize(void)
{
  static uint32_t sketchsize = 0;

  if (!sketchsize) {
    sketchsize = ESP.getSketchSize();  // This takes almost 2 seconds on an ESP32
  }
  return sketchsize;
}

void ESP_Restart(void)
{
  ESP.restart();
}

#endif  // ESP32
