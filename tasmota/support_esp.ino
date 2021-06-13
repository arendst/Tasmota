/*
  support_esp.ino - ESP specific code for Tasmota

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
 * ESP8266 and ESP32 specific code
 *
 * At the end the common Tasmota calls are provided
\*********************************************************************************************/

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

void ESP_Restart(void) {
//  ESP.restart();            // This results in exception 3 on restarts on core 2.3.0
  ESP.reset();
}

uint32_t FlashWriteStartSector(void) {
  return (ESP.getSketchSize() / SPI_FLASH_SEC_SIZE) + 2;  // Stay on the safe side
}

uint32_t FlashWriteMaxSector(void) {
  return (((uint32_t)&_FS_start - 0x40200000) / SPI_FLASH_SEC_SIZE) - 2;
}

uint8_t* FlashDirectAccess(void) {
  return (uint8_t*)(0x40200000 + (FlashWriteStartSector() * SPI_FLASH_SEC_SIZE));
}

void *special_malloc(uint32_t size) {
  return malloc(size);
}

void *special_realloc(void *ptr, size_t size) {
  return realloc(ptr, size);
}

void *special_calloc(size_t num, size_t size) {
  return calloc(num, size);
}

String GetDeviceHardware(void) {
  // esptool.py get_efuses
  uint32_t efuse1 = *(uint32_t*)(0x3FF00050);
  uint32_t efuse2 = *(uint32_t*)(0x3FF00054);
//  uint32_t efuse3 = *(uint32_t*)(0x3FF00058);
//  uint32_t efuse4 = *(uint32_t*)(0x3FF0005C);

  if (((efuse1 & (1 << 4)) || (efuse2 & (1 << 16))) && (ESP.getFlashChipRealSize() < 1048577)) {  // ESP8285 can only have 1M flash
    return F("ESP8285");
  }
  return F("ESP8266EX");
}

#endif

/*********************************************************************************************\
 * ESP32 Support
\*********************************************************************************************/

#ifdef ESP32

// Handle 20k of NVM

#include <nvs.h>

// See libraries\ESP32\examples\ResetReason.ino
#if ESP_IDF_VERSION_MAJOR > 3      // IDF 4+
  #if CONFIG_IDF_TARGET_ESP32      // ESP32/PICO-D4
    #include "esp32/rom/rtc.h"
  #elif CONFIG_IDF_TARGET_ESP32S2  // ESP32-S2
    #include "esp32s2/rom/rtc.h"
  #elif CONFIG_IDF_TARGET_ESP32C3  // ESP32-C3
    #include "esp32c3/rom/rtc.h"
  #else
    #error Target CONFIG_IDF_TARGET is not supported
  #endif
#else // ESP32 Before IDF 4.0
  #include "rom/rtc.h"
#endif

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
  // SDK and Tasmota data is held in default NVS partition
  // Tasmota data is held also in file /.settings on default filesystem
  // cal_data - SDK PHY calibration data as documented in esp_phy_init.h
  // qpc      - Tasmota Quick Power Cycle state
  // main     - Tasmota Settings data
  int32_t r1, r2, r3;
  switch (type) {
    case 0:               // Reset 2 = Erase all flash from program end to end of physical flash
    case 2:               // Reset 5, 6 = Erase all flash from program end to end of physical flash excluding filesystem
//      nvs_flash_erase();  // Erase RTC, PHY, sta.mac, ap.sndchan, ap.mac, Tasmota etc.
      r1 = NvmErase("qpc");
      r2 = NvmErase("main");
      r3 = TfsDeleteFile(TASM_FILE_SETTINGS);
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_ERASE " Tasmota data (%d,%d,%d)"), r1, r2, r3);
      break;
    case 1:               // Reset 3 = SDK parameter area
    case 4:               // WIFI_FORCE_RF_CAL_ERASE = SDK parameter area
      r1 = esp_phy_erase_cal_data_in_nvs();
//      r1 = NvmErase("cal_data");
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_ERASE " PHY data (%d)"), r1);
      break;
    case 3:               // QPC Reached = QPC, Tasmota and SDK parameter area (0x0F3xxx - 0x0FFFFF)
//      nvs_flash_erase();  // Erase RTC, PHY, sta.mac, ap.sndchan, ap.mac, Tasmota etc.
      r1 = NvmErase("qpc");
      r2 = NvmErase("main");
//      r3 = esp_phy_erase_cal_data_in_nvs();
//      r3 = NvmErase("cal_data");
//      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_ERASE " Tasmota (%d,%d) and PHY data (%d)"), r1, r2, r3);
      r3 = TfsDeleteFile(TASM_FILE_SETTINGS);
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_ERASE " Tasmota data (%d,%d,%d)"), r1, r2, r3);
      break;
  }
}

uint32_t SettingsRead(void *data, size_t size) {
  uint32_t source = 1;
  if (!TfsLoadFile(TASM_FILE_SETTINGS, (uint8_t*)data, size)) {
    source = 0;
    NvmLoad("main", "Settings", data, size);
  }
  return source;
}

void SettingsWrite(const void *pSettings, unsigned nSettingsLen) {
  TfsSaveFile(TASM_FILE_SETTINGS, (const uint8_t*)pSettings, nSettingsLen);
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
  AddLog(LOG_LEVEL_INFO, PSTR("NVS: Used %d/%d entries, NameSpaces %d"),
    nvs_stats.used_entries, nvs_stats.total_entries, nvs_stats.namespace_count);
}

//
// Flash memory mapping
//

// See Esp.cpp
#include "Esp.h"
#include "esp_spi_flash.h"
#include <memory>
#include <soc/soc.h>
#include <soc/efuse_reg.h>
#include <esp_partition.h>
extern "C" {
#include "esp_ota_ops.h"
#include "esp_image_format.h"
}
#include "esp_system.h"
#if ESP_IDF_VERSION_MAJOR > 3       // IDF 4+
  #if CONFIG_IDF_TARGET_ESP32       // ESP32/PICO-D4
    #include "esp32/rom/spi_flash.h"
  #elif CONFIG_IDF_TARGET_ESP32S2   // ESP32-S2
    #include "esp32s2/rom/spi_flash.h"
  #elif CONFIG_IDF_TARGET_ESP32C3   // ESP32-C3
    #include "esp32c3/rom/spi_flash.h"
  #else
    #error Target CONFIG_IDF_TARGET is not supported
  #endif
#else // ESP32 Before IDF 4.0
  #include "rom/spi_flash.h"
#endif

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
  AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: Spi_flash_map %d"), err);

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

    AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: Partition start 0x%08X, Partition end 0x%08X, Mmap data 0x%08X"), partition->address, partition->size, TasmotaGlobal_mmap_data);

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

#if CONFIG_IDF_TARGET_ESP32
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
#elif CONFIG_IDF_TARGET_ESP32S2
	// tools\sdk\esp32\include\esp_rom\include\esp32s2\rom\rtc.h
  switch (rtc_get_reset_reason(cpu_no)) {
    case POWERON_RESET          : return F("Vbat power on reset");                              // 1
    case RTC_SW_SYS_RESET       : return F("Software reset digital core");                      // 3
    case DEEPSLEEP_RESET        : return F("Deep Sleep reset digital core");                    // 5
    case TG0WDT_SYS_RESET       : return F("Timer Group0 Watch dog reset digital core");        // 7
    case TG1WDT_SYS_RESET       : return F("Timer Group1 Watch dog reset digital core");        // 8
    case RTCWDT_SYS_RESET       : return F("RTC Watch dog Reset digital core");                 // 9
    case INTRUSION_RESET        : return F("Instrusion tested to reset CPU");                   // 10
    case TG0WDT_CPU_RESET       : return F("Time Group0 reset CPU");                            // 11
    case RTC_SW_CPU_RESET       : return F("Software reset CPU");                               // 12
    case RTCWDT_CPU_RESET       : return F("RTC Watch dog Reset CPU");                          // 13
    case RTCWDT_BROWN_OUT_RESET : return F("Reset when the vdd voltage is not stable");         // 15
    case RTCWDT_RTC_RESET       : return F("RTC Watch dog reset digital core and rtc module");  // 16
    case TG1WDT_CPU_RESET       : return F("Time Group1 reset CPU");                            // 17
    case SUPER_WDT_RESET        : return F("Super watchdog reset digital core and rtc module"); // 18
    case GLITCH_RTC_RESET       : return F("Glitch reset digital core and rtc module");         // 19
  }
#endif

  return F("No meaning");                                                                       // 0 and undefined
}

String ESP_getResetReason(void) {
  return ESP32GetResetReason(0);  // CPU 0
}

uint32_t ESP_ResetInfoReason(void) {
  RESET_REASON reason = rtc_get_reset_reason(0);
#if CONFIG_IDF_TARGET_ESP32
  if (POWERON_RESET == reason) { return REASON_DEFAULT_RST; }
  if (SW_CPU_RESET == reason) { return REASON_SOFT_RESTART; }
  if (DEEPSLEEP_RESET == reason)  { return REASON_DEEP_SLEEP_AWAKE; }
  if (SW_RESET == reason) { return REASON_EXT_SYS_RST; }
#elif CONFIG_IDF_TARGET_ESP32S2
  if (POWERON_RESET == reason) { return REASON_DEFAULT_RST; }
  if (RTC_SW_CPU_RESET == reason) { return REASON_SOFT_RESTART; }
  if (DEEPSLEEP_RESET == reason)  { return REASON_DEEP_SLEEP_AWAKE; }
  if (RTC_SW_SYS_RESET == reason) { return REASON_EXT_SYS_RST; }
#endif
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
  return ESP.getFreeHeap();
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
  AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: Flash start address 0x%08X, Mmap address 0x%08X"), address, data);

  uint8_t buf[32];
  memcpy(buf, data, sizeof(buf));
  AddLogBuffer(LOG_LEVEL_DEBUG, (uint8_t*)&buf, 32);
*/
  return data;
}

void *special_malloc(uint32_t size) {
  if (psramFound()) {
    return heap_caps_malloc(size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  } else {
    return malloc(size);
  }
}
void *special_realloc(void *ptr, size_t size) {
  if (psramFound()) {
    return heap_caps_realloc(ptr, size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  } else {
    return realloc(ptr, size);
  }
}
void *special_calloc(size_t num, size_t size) {
  if (psramFound()) {
    return heap_caps_calloc(num, size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  } else {
    return calloc(num, size);
  }
}

float CpuTemperature(void) {
  return ConvertTemp(temperatureRead());
}

/*
#if CONFIG_IDF_TARGET_ESP32S2
#include "esp32s2/esp_efuse.h"
#elif CONFIG_IDF_TARGET_ESP32S3
#include "esp32s3/esp_efuse.h"
#elif CONFIG_IDF_TARGET_ESP32C3
#include "esp32c3/esp_efuse.h"
#endif
*/

String GetDeviceHardware(void) {
  // https://www.espressif.com/en/products/socs

/*
Source: esp-idf esp_system.h and esptool

typedef enum {
    CHIP_ESP32   = 1, //!< ESP32
    CHIP_ESP32S2 = 2, //!< ESP32-S2
    CHIP_ESP32S3 = 4, //!< ESP32-S3
    CHIP_ESP32C3 = 5, //!< ESP32-C3
} esp_chip_model_t;

// Chip feature flags, used in esp_chip_info_t
#define CHIP_FEATURE_EMB_FLASH      BIT(0)      //!< Chip has embedded flash memory
#define CHIP_FEATURE_WIFI_BGN       BIT(1)      //!< Chip has 2.4GHz WiFi
#define CHIP_FEATURE_BLE            BIT(4)      //!< Chip has Bluetooth LE
#define CHIP_FEATURE_BT             BIT(5)      //!< Chip has Bluetooth Classic

// The structure represents information about the chip
typedef struct {
    esp_chip_model_t model;  //!< chip model, one of esp_chip_model_t
    uint32_t features;       //!< bit mask of CHIP_FEATURE_x feature flags
    uint8_t cores;           //!< number of CPU cores
    uint8_t revision;        //!< chip revision number
} esp_chip_info_t;

*/
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);

  uint32_t chip_model = chip_info.model;
  uint32_t chip_revision = chip_info.revision;
//  uint32_t chip_revision = ESP.getChipRevision();
  bool rev3 = (3 == chip_revision);
//  bool single_core = (1 == ESP.getChipCores());
  bool single_core = (1 == chip_info.cores);

  if (chip_model < 2) {  // ESP32
#ifdef CONFIG_IDF_TARGET_ESP32
/* esptool:
    def get_pkg_version(self):
        word3 = self.read_efuse(3)
        pkg_version = (word3 >> 9) & 0x07
        pkg_version += ((word3 >> 2) & 0x1) << 3
        return pkg_version
*/
    uint32_t chip_ver = REG_GET_FIELD(EFUSE_BLK0_RDATA3_REG, EFUSE_RD_CHIP_VER_PKG);
    uint32_t pkg_version = chip_ver & 0x7;

//    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HDW: ESP32 Model %d, Revision %d, Core %d, Package %d"), chip_info.model, chip_revision, chip_info.cores, chip_ver);

    switch (pkg_version) {
      case 0:
        if (single_core) { return F("ESP32-S0WDQ6"); }     // Max 240MHz, Single core, QFN 6*6
        else if (rev3)   { return F("ESP32-D0WDQ6-V3"); }  // Max 240MHz, Dual core, QFN 6*6
        else {             return F("ESP32-D0WDQ6"); }     // Max 240MHz, Dual core, QFN 6*6
      case 1:
        if (single_core) { return F("ESP32-S0WD"); }       // Max 160MHz, Single core, QFN 5*5, ESP32-SOLO-1, ESP32-DevKitC
        else if (rev3)   { return F("ESP32-D0WD-V3"); }    // Max 240MHz, Dual core, QFN 5*5, ESP32-WROOM-32E, ESP32_WROVER-E, ESP32-DevKitC
        else {             return F("ESP32-D0WD"); }       // Max 240MHz, Dual core, QFN 5*5, ESP32-WROOM-32D, ESP32_WROVER-B, ESP32-DevKitC
      case 2:              return F("ESP32-D2WD");         // Max 160MHz, Dual core, QFN 5*5, 2MB embedded flash
      case 3:
        if (single_core) { return F("ESP32-S0WD-OEM"); }   // Max 160MHz, Single core, QFN 5*5, Xiaomi Yeelight
        else {             return F("ESP32-D0WD-OEM"); }   // Max 240MHz, Dual core, QFN 5*5
      case 4:              return F("ESP32-U4WDH");        // Max 160MHz, Single core, QFN 5*5, 4MB embedded flash, ESP32-MINI-1, ESP32-DevKitM-1
      case 5:
        if (rev3)        { return F("ESP32-PICO-V3"); }    // Max 240MHz, Dual core, LGA 7*7, ESP32-PICO-V3-ZERO, ESP32-PICO-V3-ZERO-DevKit
        else {             return F("ESP32-PICO-D4"); }    // Max 240MHz, Dual core, LGA 7*7, 4MB embedded flash, ESP32-PICO-KIT
      case 6:              return F("ESP32-PICO-V3-02");   // Max 240MHz, Dual core, LGA 7*7, 8MB embedded flash, 2MB embedded PSRAM, ESP32-PICO-MINI-02, ESP32-PICO-DevKitM-2
    }
#endif  // CONFIG_IDF_TARGET_ESP32
    return F("ESP32");
  }
  else if (2 == chip_model) {  // ESP32-S2
#ifdef CONFIG_IDF_TARGET_ESP32S2
/* esptool:
    def get_pkg_version(self):
        num_word = 3
        block1_addr = self.EFUSE_BASE + 0x044
        word3 = self.read_reg(block1_addr + (4 * num_word))
        pkg_version = (word3 >> 21) & 0x0F
        return pkg_version
*/
    uint32_t chip_ver = REG_GET_FIELD(EFUSE_RD_MAC_SPI_SYS_3_REG, EFUSE_PKG_VERSION);
    uint32_t pkg_version = chip_ver & 0x7;
//    uint32_t pkg_version = esp_efuse_get_pkg_ver();

//    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HDW: ESP32 Model %d, Revision %d, Core %d, Package %d"), chip_info.model, chip_revision, chip_info.cores, chip_ver);

    switch (pkg_version) {
      case 0:              return F("ESP32-S2");           // Max 240MHz, Single core, QFN 7*7, ESP32-S2-WROOM, ESP32-S2-WROVER, ESP32-S2-Saola-1, ESP32-S2-Kaluga-1
      case 1:              return F("ESP32-S2FH2");        // Max 240MHz, Single core, QFN 7*7, 2MB embedded flash, ESP32-S2-MINI-1, ESP32-S2-DevKitM-1
      case 2:              return F("ESP32-S2FH4");        // Max 240MHz, Single core, QFN 7*7, 4MB embedded flash
      case 3:              return F("ESP32-S2FN4R2");      // Max 240MHz, Single core, QFN 7*7, 4MB embedded flash, 2MB embedded PSRAM, , ESP32-S2-MINI-1U, ESP32-S2-DevKitM-1U
    }
#endif  // CONFIG_IDF_TARGET_ESP32S2
    return F("ESP32-S2");
  }
  else if (4 == chip_model) {  // ESP32-S3
    return F("ESP32-S3");                                  // Max 240MHz, Dual core, QFN 7*7, ESP32-S3-WROOM-1, ESP32-S3-DevKitC-1
  }
  else if (5 == chip_model) {  // ESP32-C3
#ifdef CONFIG_IDF_TARGET_ESP32C3
/* esptool:
    def get_pkg_version(self):
        num_word = 3
        block1_addr = self.EFUSE_BASE + 0x044
        word3 = self.read_reg(block1_addr + (4 * num_word))
        pkg_version = (word3 >> 21) & 0x0F
        return pkg_version
*/
    uint32_t chip_ver = REG_GET_FIELD(EFUSE_RD_MAC_SPI_SYS_3_REG, EFUSE_PKG_VERSION);
    uint32_t pkg_version = chip_ver & 0x7;
//    uint32_t pkg_version = esp_efuse_get_pkg_ver();

//    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HDW: ESP32 Model %d, Revision %d, Core %d, Package %d"), chip_info.model, chip_revision, chip_info.cores, chip_ver);

    switch (pkg_version) {
      case 0:              return F("ESP32-C3");           // Max 160MHz, Single core, QFN 5*5, ESP32-C3-WROOM-02, ESP32-C3-DevKitC-02
      case 1:              return F("ESP32-C3FH4");        // Max 160MHz, Single core, QFN 5*5, 4MB embedded flash, ESP32-C3-MINI-1, ESP32-C3-DevKitM-1
    }
#endif  // CONFIG_IDF_TARGET_ESP32C3
    return F("ESP32-C3");
  }
  else if (6 == chip_model) {  // ESP32-S3(beta3)
    return F("ESP32-S3");
  }
  else if (7 == chip_model) {  // ESP32-C6
#ifdef CONFIG_IDF_TARGET_ESP32C6
/* esptool:
    def get_pkg_version(self):
        num_word = 3
        block1_addr = self.EFUSE_BASE + 0x044
        word3 = self.read_reg(block1_addr + (4 * num_word))
        pkg_version = (word3 >> 21) & 0x0F
        return pkg_version
*/
    uint32_t chip_ver = REG_GET_FIELD(EFUSE_RD_MAC_SPI_SYS_3_REG, EFUSE_PKG_VERSION);
    uint32_t pkg_version = chip_ver & 0x7;
//    uint32_t pkg_version = esp_efuse_get_pkg_ver();

//    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HDW: ESP32 Model %d, Revision %d, Core %d, Package %d"), chip_info.model, chip_revision, chip_info.cores, chip_ver);

    switch (pkg_version) {
      case 0:              return F("ESP32-C6");
    }
#endif  // CONFIG_IDF_TARGET_ESP32C6
    return F("ESP32-C6");
  }
  return F("ESP32");
}

#endif  // ESP32

/*********************************************************************************************\
 * ESP Support
\*********************************************************************************************/

uint32_t ESP_getFreeHeap1024(void) {
  return ESP_getFreeHeap() / 1024;
}
/*
float ESP_getFreeHeap1024(void) {
  return ((float)ESP_getFreeHeap()) / 1024;
}
*/

/*********************************************************************************************\
 * High entropy hardware random generator
 * Thanks to DigitalAlchemist
\*********************************************************************************************/
// Based on code from https://raw.githubusercontent.com/espressif/esp-idf/master/components/esp32/hw_random.c
uint32_t HwRandom(void) {
#if ESP8266
  // https://web.archive.org/web/20160922031242/http://esp8266-re.foogod.com/wiki/Random_Number_Generator
  #define _RAND_ADDR 0x3FF20E44UL
#endif  // ESP8266
#ifdef ESP32
  #define _RAND_ADDR 0x3FF75144UL
#endif  // ESP32
  static uint32_t last_ccount = 0;
  uint32_t ccount;
  uint32_t result = 0;
  do {
    ccount = ESP.getCycleCount();
    result ^= *(volatile uint32_t *)_RAND_ADDR;
  } while (ccount - last_ccount < 64);
  last_ccount = ccount;
  return result ^ *(volatile uint32_t *)_RAND_ADDR;
#undef _RAND_ADDR
}
