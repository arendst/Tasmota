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

void SettingsLoadUpg(void *pSettings, unsigned nSettingsLen)
{
  NvmLoad("upg", "Settings", pSettings, nSettingsLen);
}

void SettingsLoadUpgH(void *pSettings, unsigned nSettingsLen)
{
  NvmLoad("upg", "SettingsH", pSettings, nSettingsLen);
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

#endif  // ESP32
