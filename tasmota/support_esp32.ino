/*
  support_esp32.ino - ESP32 specific code for Tasmota

  Copyright (C) 2021  Theo Arends / Jörg Schüler-Maroldt

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

uint32_t ESP_ResetInfoReason(void) {
  return resetInfo.reason;
}

String ESP_getResetReason(void) {
  return ESP.getResetReason();
}

uint32_t ESP_getChipId(void) {
  return ESP.getChipId();
}

uint32_t ESP_getSketchSize(void) {
  return ESP.getSketchSize();
}

uint32_t ESP_getFreeHeap(void) {
  return ESP.getFreeHeap();
}

uint32_t ESP_getMaxAllocHeap(void) {
/*
  From libraries.rst
  ESP.getMaxFreeBlockSize() returns the largest contiguous free RAM block in
  the heap, useful for checking heap fragmentation.  **NOTE:** Maximum
  ``malloc()``able block will be smaller due to memory manager overheads.

  From HeapMetric.ino
  ESP.getMaxFreeBlockSize() does not indicate the amount of memory that is
  available for use in a single malloc call.  It indicates the size of a
  contiguous block of (raw) memory before the umm_malloc overhead is removed.

  It should also be pointed out that, if you allow for the needed overhead in
  your malloc call, it could still fail in the general case. An IRQ handler
  could have allocated memory between the time you call
  ESP.getMaxFreeBlockSize() and your malloc call, reducing the available
  memory.
*/
  uint32_t free_block_size = ESP.getMaxFreeBlockSize();
  if (free_block_size > 100) { free_block_size -= 100; }
  return free_block_size;
}

void ESP_Restart(void) {
//  ESP.restart();            // This results in exception 3 on restarts on core 2.3.0
  ESP.reset();
}

uint32_t FlashWriteStartSector(void) {
  return (ESP.getSketchSize() / SPI_FLASH_SEC_SIZE) + 2;  // Stay on the safe side
}

uint32_t FlashWriteMaxSector(void) {
  return (((uint32_t)&_FS_end - 0x40200000) / SPI_FLASH_SEC_SIZE) - 2;
}

uint8_t* FlashDirectAccess(void) {
  return (uint8_t*)(0x40200000 + (FlashWriteStartSector() * SPI_FLASH_SEC_SIZE));
}
#endif

/*********************************************************************************************\
 * ESP32 Support
\*********************************************************************************************/

#ifdef ESP32

// Handle 20k of NVM

#include <nvs.h>
#include <rom/rtc.h>
#include <esp_phy_init.h>

void NvmLoad(const char *sNvsName, const char *sName, void *pSettings, unsigned nSettingsLen) {
  nvs_handle handle;
  noInterrupts();
  nvs_open(sNvsName, NVS_READONLY, &handle);
  size_t size = nSettingsLen;
  nvs_get_blob(handle, sName, pSettings, &size);
  nvs_close(handle);
  interrupts();
}

void NvmSave(const char *sNvsName, const char *sName, const void *pSettings, unsigned nSettingsLen) {
  nvs_handle handle;
  noInterrupts();
  nvs_open(sNvsName, NVS_READWRITE, &handle);
  nvs_set_blob(handle, sName, pSettings, nSettingsLen);
  nvs_commit(handle);
  nvs_close(handle);
  interrupts();
}

int32_t NvmErase(const char *sNvsName) {
  nvs_handle handle;
  noInterrupts();
  int32_t result = nvs_open(sNvsName, NVS_READWRITE, &handle);
  if (ESP_OK == result) { result = nvs_erase_all(handle); }
  if (ESP_OK == result) { result = nvs_commit(handle); }
  nvs_close(handle);
  interrupts();
  return result;
}

void SettingsErase(uint8_t type) {
  // All SDK and Tasmota data is held in default NVS partition
  // cal_data - SDK PHY calibration data as documented in esp_phy_init.h
  // qpc      - Tasmota Quick Power Cycle state
  // main     - Tasmota Settings data
  int32_t r1, r2, r3;
  switch (type) {
    case 0:               // Reset 2, 5, 6 = Erase all flash from program end to end of physical flash
//      nvs_flash_erase();  // Erase RTC, PHY, sta.mac, ap.sndchan, ap.mac, Tasmota etc.
      r1 = NvmErase("qpc");
      r2 = NvmErase("main");
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_ERASE " Tasmota data (%d,%d)"), r1, r2);
      break;
    case 1: case 4:       // Reset 3 or WIFI_FORCE_RF_CAL_ERASE = SDK parameter area
      r1 = esp_phy_erase_cal_data_in_nvs();
//      r1 = NvmErase("cal_data");
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_ERASE " PHY data (%d)"), r1);
      break;
    case 2:               // Not used = QPC and Tasmota parameter area (0x0F3xxx - 0x0FBFFF)
      r1 = NvmErase("qpc");
      r2 = NvmErase("main");
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_ERASE " Tasmota data (%d,%d)"), r1, r2);
      break;
    case 3:               // QPC Reached = QPC, Tasmota and SDK parameter area (0x0F3xxx - 0x0FFFFF)
//      nvs_flash_erase();  // Erase RTC, PHY, sta.mac, ap.sndchan, ap.mac, Tasmota etc.
      r1 = NvmErase("qpc");
      r2 = NvmErase("main");
//      r3 = esp_phy_erase_cal_data_in_nvs();
//      r3 = NvmErase("cal_data");
//      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_ERASE " Tasmota (%d,%d) and PHY data (%d)"), r1, r2, r3);
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_ERASE " Tasmota data (%d,%d)"), r1, r2);
      break;
  }
}

void SettingsRead(void *data, size_t size) {
#ifdef USE_TFS
//  if (!TfsLoadFile("/settings", (uint8_t*)data, size)) {
    NvmLoad("main", "Settings", data, size);
//  }
#else
  NvmLoad("main", "Settings", data, size);
#endif
}

void SettingsWrite(const void *pSettings, unsigned nSettingsLen) {
#ifdef USE_TFS
//  TfsSaveFile("/settings", (const uint8_t*)pSettings, nSettingsLen);
#endif
  NvmSave("main", "Settings", pSettings, nSettingsLen);
}

void QPCRead(void *pSettings, unsigned nSettingsLen) {
  NvmLoad("qpc", "pcreg", pSettings, nSettingsLen);
}

void QPCWrite(const void *pSettings, unsigned nSettingsLen) {
  NvmSave("qpc", "pcreg", pSettings, nSettingsLen);
}

void NvsInfo(void) {
  nvs_stats_t nvs_stats;
  nvs_get_stats(NULL, &nvs_stats);
  AddLog_P(LOG_LEVEL_INFO, PSTR("NVS: Used %d/%d entries, NameSpaces %d"),
    nvs_stats.used_entries, nvs_stats.total_entries, nvs_stats.namespace_count);
}

//
// Flash memory mapping
//

#include "Esp.h"
#include "rom/spi_flash.h"
#include "esp_spi_flash.h"
#include <memory>
#include <soc/soc.h>
#include <soc/efuse_reg.h>
#include <esp_partition.h>
extern "C" {
#include "esp_ota_ops.h"
#include "esp_image_format.h"
}

uint32_t EspFlashBaseAddress(void) {
  const esp_partition_t* partition = esp_ota_get_next_update_partition(nullptr);
  if (!partition) { return 0; }
  return partition->address;  // For tasmota 0x00010000 or 0x00200000
}

uint32_t EspFlashBaseEndAddress(void) {
  const esp_partition_t* partition = esp_ota_get_next_update_partition(nullptr);
  if (!partition) { return 0; }
  return partition->address + partition->size;  // For tasmota 0x00200000 or 0x003F0000
}

uint8_t* EspFlashMmap(uint32_t address) {
  static spi_flash_mmap_handle_t handle = 0;

  if (handle) {
    spi_flash_munmap(handle);
    handle = 0;
  }

  const uint8_t* data;
  int32_t err = spi_flash_mmap(address, 5 * SPI_FLASH_MMU_PAGE_SIZE, SPI_FLASH_MMAP_DATA, (const void **)&data, &handle);

/*
  AddLog_P(LOG_LEVEL_DEBUG, PSTR("DBG: Spi_flash_map %d"), err);

  spi_flash_mmap_dump();
*/
  return (uint8_t*)data;
}

/*
int32_t EspPartitionMmap(uint32_t action) {
  static spi_flash_mmap_handle_t handle;

  int32_t err = 0;
  if (1 == action) {
    const esp_partition_t *partition = esp_ota_get_running_partition();
//    const esp_partition_t* partition = esp_ota_get_next_update_partition(nullptr);
    if (!partition) { return 0; }
    err = esp_partition_mmap(partition, 0, 4 * SPI_FLASH_MMU_PAGE_SIZE, SPI_FLASH_MMAP_DATA, (const void **)&TasmotaGlobal_mmap_data, &handle);

    AddLog_P(LOG_LEVEL_DEBUG, PSTR("DBG: Partition start 0x%08X, Partition end 0x%08X, Mmap data 0x%08X"), partition->address, partition->size, TasmotaGlobal_mmap_data);

  } else {
    spi_flash_munmap(handle);
    handle = 0;
  }
  return err;
}

*/
//
// Crash stuff
//

void CrashDump(void) {
}

bool CrashFlag(void) {
  return false;
}

void CrashDumpClear(void) {
}

void CmndCrash(void) {
  /*
  volatile uint32_t dummy;
  dummy = *((uint32_t*) 0x00000000);
*/
}

// Do an infinite loop to trigger WDT watchdog
void CmndWDT(void) {
  /*
  volatile uint32_t dummy = 0;
  while (1) {
    dummy++;
  }
*/
}
// This will trigger the os watch after OSWATCH_RESET_TIME (=120) seconds
void CmndBlockedLoop(void) {
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

void DisableBrownout(void) {
  // https://github.com/espressif/arduino-esp32/issues/863#issuecomment-347179737
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);  // Disable brownout detector
}

//
// ESP32 Alternatives
//

String ESP32GetResetReason(uint32_t cpu_no) {
	// tools\sdk\include\esp32\rom\rtc.h
  switch (rtc_get_reset_reason(cpu_no)) {
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
  }
  return F("No meaning");                                                                       // 0 and undefined
}

String ESP_getResetReason(void) {
  return ESP32GetResetReason(0);  // CPU 0
}

uint32_t ESP_ResetInfoReason(void) {
  RESET_REASON reason = rtc_get_reset_reason(0);
  if (POWERON_RESET == reason) { return REASON_DEFAULT_RST; }
  if (SW_CPU_RESET == reason) { return REASON_SOFT_RESTART; }
  if (DEEPSLEEP_RESET == reason)  { return REASON_DEEP_SLEEP_AWAKE; }
  if (SW_RESET == reason) { return REASON_EXT_SYS_RST; }
  return -1; //no "official error code", but should work with the current code base
}

uint32_t ESP_getChipId(void) {
  uint32_t id = 0;
  for (uint32_t i = 0; i < 17; i = i +8) {
    id |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  return id;
}

uint32_t ESP_getSketchSize(void) {
  static uint32_t sketchsize = 0;

  if (!sketchsize) {
    sketchsize = ESP.getSketchSize();  // This takes almost 2 seconds on an ESP32
  }
  return sketchsize;
}

uint32_t ESP_getFreeHeap(void) {
//  return ESP.getFreeHeap();
  return ESP.getMaxAllocHeap();
}

uint32_t ESP_getMaxAllocHeap(void) {
  // largest block of heap that can be allocated at once
  uint32_t free_block_size = ESP.getMaxAllocHeap();
  if (free_block_size > 100) { free_block_size -= 100; }
  return free_block_size;
}

void ESP_Restart(void) {
  ESP.restart();
}

uint32_t FlashWriteStartSector(void) {
  // Needs to be on SPI_FLASH_MMU_PAGE_SIZE (= 0x10000) alignment for mmap usage
  uint32_t aligned_address = ((EspFlashBaseAddress() + (2 * SPI_FLASH_MMU_PAGE_SIZE)) / SPI_FLASH_MMU_PAGE_SIZE) * SPI_FLASH_MMU_PAGE_SIZE;
  return aligned_address / SPI_FLASH_SEC_SIZE;
}

uint32_t FlashWriteMaxSector(void) {
  // Needs to be on SPI_FLASH_MMU_PAGE_SIZE (= 0x10000) alignment for mmap usage
  uint32_t aligned_end_address = (EspFlashBaseEndAddress() / SPI_FLASH_MMU_PAGE_SIZE) * SPI_FLASH_MMU_PAGE_SIZE;
  return aligned_end_address / SPI_FLASH_SEC_SIZE;
}

uint8_t* FlashDirectAccess(void) {
  uint32_t address = FlashWriteStartSector() * SPI_FLASH_SEC_SIZE;
  uint8_t* data = EspFlashMmap(address);
/*
  AddLog_P(LOG_LEVEL_DEBUG, PSTR("DBG: Flash start address 0x%08X, Mmap address 0x%08X"), address, data);

  uint8_t buf[32];
  memcpy(buf, data, sizeof(buf));
  AddLogBuffer(LOG_LEVEL_DEBUG, (uint8_t*)&buf, 32);
*/
  return data;
}

#endif  // ESP32