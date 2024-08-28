/*
  support_esp32.ino - ESP32 specific support for Tasmota

  SPDX-FileCopyrightText: 2023 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef ESP32
/*********************************************************************************************\
 * ESP32, ESP32-S2, ESP32-S3, ESP32-C2, ESP32-C3, ESP32-C6 and ESP32-H2 Support
\*********************************************************************************************/

//                                                   11b 11g 11n  11n  11ax
const static char kWifiPhyMode[] PROGMEM = "low rate|11b|11g|HT20|HT40|HE20"; // Wi-Fi Modes

#include "soc/soc.h"
#include "soc/spi_reg.h"
// ESP32_ARCH contains the name of the architecture (used by autoconf)
#if CONFIG_IDF_TARGET_ESP32
  #ifdef CORE32SOLO1
    #define ESP32_ARCH            "esp32solo1"
  #else
    #define ESP32_ARCH            "esp32"
  #endif
#elif CONFIG_IDF_TARGET_ESP32S2
  #define ESP32_ARCH              "esp32s2"
#elif CONFIG_IDF_TARGET_ESP32S3
  #define ESP32_ARCH              "esp32s3"
#elif CONFIG_IDF_TARGET_ESP32C2
  #define ESP32_ARCH              "esp32c2"
#elif CONFIG_IDF_TARGET_ESP32C3
  #define ESP32_ARCH              "esp32c3"
#elif CONFIG_IDF_TARGET_ESP32C6
  #define ESP32_ARCH              "esp32c6"
#elif CONFIG_IDF_TARGET_ESP32H2
  #define ESP32_ARCH              "esp32h2"
#else
  #define ESP32_ARCH              ""
#endif

// See libraries\ESP32\examples\ResetReason.ino
#include "esp_chip_info.h"
#if CONFIG_IDF_TARGET_ESP32      // ESP32/PICO-D4
  #include "esp32/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32S2  // ESP32-S2
  #include "esp32s2/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32S3  // ESP32-S3
  #include "esp32s3/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32C2  // ESP32-C2
  #include "esp32c2/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32C3  // ESP32-C3
  #include "esp32c3/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32C6  // ESP32-C6
  #include "esp32c6/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32H2  // ESP32-H2
  #include "esp32h2/rom/rtc.h"
#else
  #error Target CONFIG_IDF_TARGET is not supported
#endif

// Set the Stacksize for Arduino core. Default is 8192, some builds may need a bigger one
size_t getArduinoLoopTaskStackSize(void) {
  return SET_ESP32_STACK_SIZE;
}

#include <esp_phy_init.h>

// Handle 20k of NVM

#include <nvs.h>

bool NvmExists(const char *sNvsName) {
  nvs_handle_t handle;
  if (nvs_open(sNvsName, NVS_READONLY, &handle) != ESP_OK) {
    return false;
  }
  nvs_close(handle);
  return true;
}

bool NvmLoad(const char *sNvsName, const char *sName, void *pSettings, unsigned nSettingsLen) {
  nvs_handle_t handle;
  esp_err_t result = nvs_open(sNvsName, NVS_READONLY, &handle);
  if (result != ESP_OK) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("NVS: Error %d"), result);
    return false;
  }
  size_t size = nSettingsLen;
  nvs_get_blob(handle, sName, pSettings, &size);
  nvs_close(handle);
  return true;
}

void NvmSave(const char *sNvsName, const char *sName, const void *pSettings, unsigned nSettingsLen) {
#ifdef USE_WEBCAM
  WcInterrupt(0);  // Stop stream if active to fix TG1WDT_SYS_RESET
#endif
  nvs_handle_t handle;
  esp_err_t result = nvs_open(sNvsName, NVS_READWRITE, &handle);
  if (result != ESP_OK) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("NVS: Error %d"), result);
  } else {
    nvs_set_blob(handle, sName, pSettings, nSettingsLen);
    nvs_commit(handle);
    nvs_close(handle);
  }
#ifdef USE_WEBCAM
  WcInterrupt(1);
#endif
}

int32_t NvmErase(const char *sNvsName) {
  nvs_handle_t handle;
  int32_t result = nvs_open(sNvsName, NVS_READWRITE, &handle);
  if (ESP_OK == result) { result = nvs_erase_all(handle); }
  if (ESP_OK == result) { result = nvs_commit(handle); }
  nvs_close(handle);
  return result;
}

void SettingsErase(uint8_t type) {
  // SDK and Tasmota data is held in default NVS partition
  // Tasmota data is held also in file /.settings on default filesystem
  // cal_data - SDK PHY calibration data as documented in esp_phy_init.h
  // qpc      - Tasmota Quick Power Cycle state
  // main     - Tasmota Settings data
  int32_t r1, r2, r3 = 0;
  switch (type) {
    case 0:               // Reset 2 = Erase all flash from program end to end of physical flash
    case 2:               // Reset 5, 6 = Erase all flash from program end to end of physical flash excluding filesystem
//      nvs_flash_erase();  // Erase RTC, PHY, sta.mac, ap.sndchan, ap.mac, Tasmota etc.
      r1 = NvmErase("qpc");
      r2 = NvmErase("main");
#ifdef USE_UFILESYS
      r3 = TfsDeleteFile(TASM_FILE_SETTINGS);
#endif
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
#ifdef USE_UFILESYS
      r3 = TfsDeleteFile(TASM_FILE_SETTINGS);
#endif
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_ERASE " Tasmota data (%d,%d,%d)"), r1, r2, r3);
      break;
  }
}

uint32_t SettingsRead(void *data, size_t size) {
#ifdef USE_UFILESYS
  if (TfsLoadFile(TASM_FILE_SETTINGS, (uint8_t*)data, size)) {
    return 2;
  }
#endif
  if (NvmLoad("main", "Settings", data, size)) {
    return 1;
  };
  return 0;
}

void SettingsWrite(const void *pSettings, unsigned nSettingsLen) {
#ifdef USE_UFILESYS
  TfsSaveFile(TASM_FILE_SETTINGS, (const uint8_t*)pSettings, nSettingsLen);
#endif
  NvmSave("main", "Settings", pSettings, nSettingsLen);
}

void QPCRead(void *pSettings, unsigned nSettingsLen) {
  NvmLoad("qpc", "pcreg", pSettings, nSettingsLen);
}

void QPCWrite(const void *pSettings, unsigned nSettingsLen) {
  NvmSave("qpc", "pcreg", pSettings, nSettingsLen);
}

bool OtaFactoryRead(void) {
  uint32_t pOtaLoader = 0;
  if (NvmExists("otal")) {
    NvmLoad("otal", "otal", &pOtaLoader, sizeof(pOtaLoader));
  } else {
    OtaFactoryWrite(pOtaLoader);
  }
  return pOtaLoader;
}

void OtaFactoryWrite(bool value) {
  uint32_t pOtaLoader = value;
  NvmSave("otal", "otal", &pOtaLoader, sizeof(pOtaLoader));
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
#include "spi_flash_mmap.h"
#include <memory>
#include <soc/soc.h>
#include <soc/efuse_reg.h>
#include <esp_partition.h>
extern "C" {
#include "esp_ota_ops.h"
#include "esp_image_format.h"
}
#include "esp_system.h"
#include "esp_flash.h"

#if CONFIG_IDF_TARGET_ESP32       // ESP32/PICO-D4
  #include "esp32/rom/spi_flash.h"
  #define ESP_FLASH_IMAGE_BASE 0x1000     // Flash offset containing magic flash size and spi mode
#elif CONFIG_IDF_TARGET_ESP32S2   // ESP32-S2
  #include "esp32s2/rom/spi_flash.h"
  #define ESP_FLASH_IMAGE_BASE 0x1000     // Flash offset containing magic flash size and spi mode
#elif CONFIG_IDF_TARGET_ESP32S3   // ESP32-S3
  #include "esp32s3/rom/spi_flash.h"
  #define ESP_FLASH_IMAGE_BASE 0x0000     // Esp32s3 is located at 0x0000
#elif CONFIG_IDF_TARGET_ESP32C2   // ESP32-C2
  #include "esp32c2/rom/spi_flash.h"
  #define ESP_FLASH_IMAGE_BASE 0x0000     // Esp32c2 is located at 0x0000
#elif CONFIG_IDF_TARGET_ESP32C3   // ESP32-C3
  #include "esp32c3/rom/spi_flash.h"
  #define ESP_FLASH_IMAGE_BASE 0x0000     // Esp32c3 is located at 0x0000
#elif CONFIG_IDF_TARGET_ESP32C6   // ESP32-C6
  #include "esp32c6/rom/spi_flash.h"
  #define ESP_FLASH_IMAGE_BASE 0x0000     // Esp32c6 is located at 0x0000
#elif CONFIG_IDF_TARGET_ESP32H2   // ESP32-H2
  #include "esp32h2/rom/spi_flash.h"
  #define ESP_FLASH_IMAGE_BASE 0x0000     // Esp32h2 is located at 0x0000
#else
    #error Target CONFIG_IDF_TARGET is not supported
#endif

#include "bootloader_common.h"

uint32_t EspProgramSize(const char *label) {
  const esp_partition_t *part = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, label);
  if (!part) {
    return 0;
  }
  const esp_partition_pos_t part_pos  = {
    .offset = part->address,
    .size = part->size,
  };
  esp_image_metadata_t data;
  data.start_addr = part_pos.offset;
  esp_image_verify(ESP_IMAGE_VERIFY, &part_pos, &data);
  return data.image_len;
}

bool EspSingleOtaPartition(void) {
  return (1 == esp_ota_get_app_partition_count());
}

uint32_t EspRunningFactoryPartition(void) {
  const esp_partition_t *cur_part = esp_ota_get_running_partition();
//  return (cur_part->type == 0 && cur_part->subtype == 0);
  if (cur_part->type == 0 && cur_part->subtype == 0) {
    return cur_part->size;
  }
  return 0;
}

void EspPrepRestartToSafeBoot(void) {
  const esp_partition_t *otadata_partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_OTA, NULL);
  if (otadata_partition) {
    esp_partition_erase_range(otadata_partition, 0, SPI_FLASH_SEC_SIZE * 2);
  }
}

bool EspPrepSwitchPartition(uint32_t state) {
  bool valid = EspSingleOtaPartition();
  if (valid) {
    bool running_factory = EspRunningFactoryPartition();
    switch (state) {
      case 0:  // Off = safeboot
        if (!running_factory) {
          EspPrepRestartToSafeBoot();
        } else {
          valid = false;
        }
        break;
      case 1:  // On = ota0
        if (running_factory) {
          const esp_partition_t* partition = esp_ota_get_next_update_partition(nullptr);
          esp_ota_set_boot_partition(partition);
        } else {
          valid = false;
        }
        break;
      case 2:  // Toggle
        if (!running_factory) {
          EspPrepRestartToSafeBoot();
        } else {
          const esp_partition_t* partition = esp_ota_get_next_update_partition(nullptr);
          esp_ota_set_boot_partition(partition);
        }
    }
  }
  return valid;
}

uint32_t EspFlashBaseAddress(void) {
  if (EspSingleOtaPartition()) {       // Only one partition so start at end of sketch
    const esp_partition_t *running = esp_ota_get_running_partition();
    if (!running) { return 0; }
    return running->address + ESP_getSketchSize();
  } else {                     // Select other partition
    const esp_partition_t* partition = esp_ota_get_next_update_partition(nullptr);
    if (!partition) { return 0; }
    return partition->address;  // For tasmota 0x00010000 or 0x00200000
  }
}

uint32_t EspFlashBaseEndAddress(void) {
  const esp_partition_t* partition = (EspSingleOtaPartition()) ? esp_ota_get_running_partition() : esp_ota_get_next_update_partition(nullptr);
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
// ESP32 specific
//

#ifdef CONFIG_IDF_TARGET_ESP32
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

void DisableBrownout(void) {
  // https://github.com/espressif/arduino-esp32/issues/863#issuecomment-347179737
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);  // Disable brownout detector
}
#endif  // ESP32

//
// ESP32 Alternatives
//

String ESP32GetResetReason(uint32_t cpu_no) {
	// tools\sdk\include\esp32\rom\rtc.h
	// tools\sdk\esp32\include\esp_rom\include\esp32c3\rom\rtc.h
	// tools\sdk\esp32\include\esp_rom\include\esp32s2\rom\rtc.h
  switch (rtc_get_reset_reason(cpu_no)) {                                   //     ESP32             ESP32-S / ESP32-C
    case 1  : return F("Vbat power on reset");                              // 1   POWERON_RESET     POWERON_RESET
    case 3  : return F("Software reset digital core");                      // 3   SW_RESET          RTC_SW_SYS_RESET
    case 4  : return F("Legacy watch dog reset digital core");              // 4   OWDT_RESET        -
    case 5  : return F("Deep Sleep reset digital core");                    // 5   DEEPSLEEP_RESET   DEEPSLEEP_RESET
    case 6  : return F("Reset by SLC module, reset digital core");          // 6   SDIO_RESET
    case 7  : return F("Timer Group0 Watch dog reset digital core");        // 7   TG0WDT_SYS_RESET
    case 8  : return F("Timer Group1 Watch dog reset digital core");        // 8   TG1WDT_SYS_RESET
    case 9  : return F("RTC Watch dog Reset digital core");                 // 9   RTCWDT_SYS_RESET
    case 10 : return F("Instrusion tested to reset CPU");                   // 10  INTRUSION_RESET
    case 11 : return F("Time Group0 reset CPU");                            // 11  TGWDT_CPU_RESET   TG0WDT_CPU_RESET
    case 12 : return F("Software reset CPU");                               // 12  SW_CPU_RESET      RTC_SW_CPU_RESET
    case 13 : return F("RTC Watch dog Reset CPU");                          // 13  RTCWDT_CPU_RESET
    case 14 : return F("or APP CPU, reseted by PRO CPU");                   // 14  EXT_CPU_RESET     -
    case 15 : return F("Reset when the vdd voltage is not stable");         // 15  RTCWDT_BROWN_OUT_RESET
    case 16 : return F("RTC Watch dog reset digital core and rtc module");  // 16  RTCWDT_RTC_RESET
    case 17 : return F("Time Group1 reset CPU");                            // 17  -                 TG1WDT_CPU_RESET
    case 18 : return F("Super watchdog reset digital core and rtc module"); // 18  -                 SUPER_WDT_RESET
    case 19 : return F("Glitch reset digital core and rtc module");         // 19  -                 GLITCH_RTC_RESET
    case 20 : return F("Efuse reset digital core");                         // 20                    EFUSE_RESET
    case 21 : return F("Usb uart reset digital core");                      // 21                    USB_UART_CHIP_RESET
    case 22 : return F("Usb jtag reset digital core");                      // 22                    USB_JTAG_CHIP_RESET
    case 23 : return F("Power glitch reset digital core and rtc module");   // 23                    POWER_GLITCH_RESET
  }

  return F("No meaning");                                                   // 0 and undefined
}

String ESP_getResetReason(void) {
  return ESP32GetResetReason(0);  // CPU 0
}

uint32_t ESP_ResetInfoReason(void) {
  RESET_REASON reason = rtc_get_reset_reason(0);
  if (1  == reason) { return REASON_DEFAULT_RST; }       // POWERON_RESET
  if ((3  == reason) || (12 == reason)) { return REASON_SOFT_RESTART; }  // SW_RESET / RTC_SW_SYS_RESET and SW_CPU_RESET / RTC_SW_CPU_RESET
  if (5  == reason) { return REASON_DEEP_SLEEP_AWAKE; }  // DEEPSLEEP_RESET
//  if (3  == reason) { return REASON_EXT_SYS_RST; }       // SW_RESET / RTC_SW_SYS_RESET
  return -1; //no "official error code", but should work with the current code base
}

uint32_t ESP_getChipId(void) {
  uint32_t id = 0;
  for (uint32_t i = 0; i < 17; i = i +8) {
    id |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  return id;
}

uint32_t ESP_getFlashChipMagicSize(void) {
    esp_image_header_t fhdr;
//    if(ESP.flashRead(ESP_FLASH_IMAGE_BASE, (uint32_t*)&fhdr.magic, sizeof(esp_image_header_t)) && fhdr.magic != ESP_IMAGE_HEADER_MAGIC) {
//      return 0;
//    }
    if (esp_flash_read(esp_flash_default_chip, (void*)&fhdr, ESP_FLASH_IMAGE_BASE, sizeof(esp_image_header_t)) && fhdr.magic != ESP_IMAGE_HEADER_MAGIC) {
      return 0;
    }
    return ESP_magicFlashChipSize(fhdr.spi_size);
}

uint32_t ESP_magicFlashChipSize(uint8_t spi_size) {
/*
    switch(spi_size & 0x0F) {
    case 0x0: // 8 MBit (1MB)
        return 1048576;
    case 0x1: // 16 MBit (2MB)
        return 2097152;
    case 0x2: // 32 MBit (4MB)
        return 4194304;
    case 0x3: // 64 MBit (8MB)
        return 8388608;
    case 0x4: // 128 MBit (16MB)
        return 16777216;
    case 0x5: // 256 MBit (32MB)
        return 33554432;
    default:  // fail so return (1KB)
        return 1024;
    }
*/
  // When spi_size is bigger than 11 will return 0 (0x100000000 = 0x00000000)
  return (uint32_t)0x100000 << (spi_size & 0x0F);  // 0 = 8 MBit (1MB), 5 = 256 MBit (32MB)
}

uint32_t ESP_getSketchSize(void) {
  static uint32_t sketchsize = 0;

  if (!sketchsize) {
    sketchsize = ESP.getSketchSize();  // This takes almost 2 seconds on an ESP32
  }
  return sketchsize;
}

uint32_t ESP_getFreeSketchSpace(void) {
  if (EspSingleOtaPartition()) {
    uint32_t size = EspRunningFactoryPartition();
    if (!size) {
      size = ESP.getFreeSketchSpace();
    }
    return size - ESP_getSketchSize();
  }
  return ESP.getFreeSketchSpace();
}

uint32_t ESP_getHeapSize(void) {
  return ESP.getHeapSize();
}

uint32_t ESP_getFreeHeap(void) {
  // ESP_getFreeHeap() returns also IRAM which we don't use
  return heap_caps_get_free_size(MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
}

uint32_t ESP_getFreeHeap1024(void) {
  return ESP_getFreeHeap() / 1024;
}
/*
float ESP_getFreeHeap1024(void) {
  return ((float)ESP_getFreeHeap()) / 1024;
}
*/

uint32_t ESP_getMaxAllocHeap(void) {
  // arduino returns IRAM but we want only DRAM
#ifdef USE_GT911 // GT911 IRQ crashes with heap_caps_get_largest_free_block
  return ESP_getFreeHeap();
#endif
  uint32_t free_block_size = heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  if (free_block_size > 100) { free_block_size -= 100; }
  return free_block_size;
}

int32_t ESP_getHeapFragmentation(void) {
  int32_t free_maxmem = 100 - (int32_t)(ESP_getMaxAllocHeap() * 100 / ESP_getFreeHeap());
  if (free_maxmem < 0) { free_maxmem = 0; }
  return free_maxmem;
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
  uint8_t buf[32];
  memcpy(buf, data, sizeof(buf));
  AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: Flash start address 0x%08X, Mmap address 0x%08X, Data %*_H"), address, data, sizeof(buf), (uint8_t*)&buf);
*/
  return data;
}

uint32_t ESP_getPsramSize(void) {
  return ESP.getPsramSize();
}

uint32_t ESP_getFreePsram(void) {
  return ESP.getFreePsram();
}

uint32_t ESP_getMaxAllocPsram(void) {
  return ESP.getMaxAllocPsram();
}

extern "C" {
  // bool IRAM_ATTR __attribute__((pure)) esp_psram_is_initialized(void)
  bool esp_psram_is_initialized(void);
}

// this function is a replacement for `psramFound()`.
// `psramFound()` can return true even if no PSRAM is actually installed
// This new version also checks `esp_spiram_is_initialized` to know if the PSRAM is initialized
bool FoundPSRAM(void) {
#if CONFIG_IDF_TARGET_ESP32C2 || CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C6 || DISABLE_PSRAMCHECK || CORE32SOLO1
  return psramFound();
#else
  return psramFound() && esp_psram_is_initialized();
#endif
}

// new function to check whether PSRAM is present and supported (i.e. required pacthes are present)
bool UsePSRAM(void) {
  static bool can_use_psram = CanUsePSRAM();
  return FoundPSRAM() && can_use_psram;
}

/*
 * ESP32 v1 and v2 needs some special patches to use PSRAM.
 * Standard Tasmota 32 do not include those patches.
 * If using ESP32 v1, please add: `-mfix-esp32-psram-cache-issue -lc-psram-workaround -lm-psram-workaround`
 *
 * This function returns true if the chip supports PSRAM natively (v3) or if the
 * patches are present.
 */
bool CanUsePSRAM(void) {
  if (!FoundPSRAM()) return false;
#ifdef HAS_PSRAM_FIX
  return true;
#endif
#ifdef CONFIG_IDF_TARGET_ESP32
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  uint32_t chip_revision = chip_info.revision;
  // idf5 efuse_hal_chip_revision(void)
  if (chip_revision < 100) { chip_revision *= 100; }  // Make <idf5 idf5
  if ((CHIP_ESP32 == chip_info.model) && (chip_revision < 300)) {
    return false;
  }
#endif // CONFIG_IDF_TARGET_ESP32
  return true;
}

void *special_malloc(uint32_t size) {
  if (UsePSRAM()) {
    return heap_caps_malloc(size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  } else {
    return malloc(size);
  }
}
void *special_realloc(void *ptr, size_t size) {
  if (UsePSRAM()) {
    return heap_caps_realloc(ptr, size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  } else {
    return realloc(ptr, size);
  }
}
void *special_calloc(size_t num, size_t size) {
  if (UsePSRAM()) {
    return heap_caps_calloc(num, size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  } else {
    return calloc(num, size);
  }
}

// Variants for IRAM heap, which need all accesses to be 32 bits aligned
void *special_malloc32(uint32_t size) {
  return heap_caps_malloc(size, MALLOC_CAP_32BIT);
}

float CpuTemperature(void) {
  return (float)temperatureRead();  // In Celsius
}

/*
#ifdef __cplusplus
extern "C" {
#endif

uint8_t temprature_sens_read();

#ifdef __cplusplus
}
#endif

#ifdef CONFIG_IDF_TARGET_ESP32
uint8_t temprature_sens_read();

float CpuTemperature(void) {
  uint8_t t = temprature_sens_read();

  AddLog(LOG_LEVEL_DEBUG, PSTR("TMP: value %d"), t);

  return (t - 32) / 1.8;
}
#else
float CpuTemperature(void) {
  // Currently (20210801) repeated calls to temperatureRead() on ESP32C3 and ESP32S2 result in IDF error messages
  static float t = NAN;
  if (isnan(t)) {
    t = (float)temperatureRead();  // In Celsius
  }
  return t;
}
#endif
*/

/*
#if CONFIG_IDF_TARGET_ESP32S2
#include "esp32s2/esp_efuse.h"
#elif CONFIG_IDF_TARGET_ESP32S3
#include "esp32s3/esp_efuse.h"
#elif CONFIG_IDF_TARGET_ESP32C3
#include "esp32c3/esp_efuse.h"
#endif
*/

// #include "esp_chip_info.h"

String GetDeviceHardware(void) {
  // https://www.espressif.com/en/products/socs

/*
Source: esp-idf esp_system.h or arduino core esp_chip_info.h and esptool

typedef enum {
    CHIP_ESP32  = 1, //!< ESP32
    CHIP_ESP32S2 = 2, //!< ESP32-S2
    CHIP_ESP32S3 = 9, //!< ESP32-S3
    CHIP_ESP32C3 = 5, //!< ESP32-C3
    CHIP_ESP32C2 = 12, //!< ESP32-C2
    CHIP_ESP32C6 = 13, //!< ESP32-C6
    CHIP_ESP32H2 = 16, //!< ESP32-H2
    CHIP_POSIX_LINUX = 999, //!< The code is running on POSIX/Linux simulator
} esp_chip_model_t;

// Chip feature flags, used in esp_chip_info_t
#define CHIP_FEATURE_EMB_FLASH      BIT(0)      //!< Chip has embedded flash memory
#define CHIP_FEATURE_WIFI_BGN       BIT(1)      //!< Chip has 2.4GHz WiFi
#define CHIP_FEATURE_BLE            BIT(4)      //!< Chip has Bluetooth LE
#define CHIP_FEATURE_BT             BIT(5)      //!< Chip has Bluetooth Classic
#define CHIP_FEATURE_IEEE802154     BIT(6)      //!< Chip has IEEE 802.15.4
#define CHIP_FEATURE_EMB_PSRAM      BIT(7)      //!< Chip has embedded psram

// The structure represents information about the chip
typedef struct {
    esp_chip_model_t model;  //!< chip model, one of esp_chip_model_t
    uint32_t features;       //!< bit mask of CHIP_FEATURE_x feature flags
    uint16_t revision;       //!< chip revision number (in format MXX; where M - wafer major version, XX - wafer minor version)
    uint8_t cores;           //!< number of CPU cores
} esp_chip_info_t;

*/
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  uint32_t chip_model = chip_info.model;
  uint32_t chip_revision = chip_info.revision;
//  uint32_t chip_revision = ESP.getChipRevision();
  // idf5 efuse_hal_chip_revision(void)
  if (chip_revision < 100) { chip_revision *= 100; }  // Make <idf5 idf5
  bool rev3 = (chip_revision >= 300);
//  bool single_core = (1 == ESP.getChipCores());
  bool single_core = (1 == chip_info.cores);
  uint32_t pkg_version = bootloader_common_get_chip_ver_pkg();

  switch (chip_model) {
    case 0:
    case 1: {  // ESP32
      /*
      ESP32 Series
      - 32-bit MCU & 2.4 GHz Wi-Fi & Bluetooth/Bluetooth LE
      - Two or one CPU core(s) with adjustable clock frequency, ranging from 80 MHz to 240 MHz
      - +19.5 dBm output power ensures a good physical range
      - Classic Bluetooth for legacy connections, also supporting L2CAP, SDP, GAP, SMP, AVDTP, AVCTP, A2DP (SNK) and AVRCP (CT)
      - Support for Bluetooth Low Energy (Bluetooth LE) profiles including L2CAP, GAP, GATT, SMP, and GATT-based profiles like BluFi, SPP-like, etc
      - Bluetooth Low Energy (Bluetooth LE) connects to smart phones, broadcasting low-energy beacons for easy detection
      - Sleep current is less than 5 μA, making it suitable for battery-powered and wearable-electronics applications
      - Peripherals include capacitive touch sensors, Hall sensor, SD card interface, Ethernet, high-speed SPI, UART, I2S and I2C
      */
#ifdef CONFIG_IDF_TARGET_ESP32

//      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HDW: ESP32 Model %d, Revision %d, Core %d"), chip_info.model, chip_revision, chip_info.cores);

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
        case 4:
          if (single_core) { return F("ESP32-U4WDH-S"); }    // Max 160MHz, Single core, QFN 5*5, 4MB embedded flash, ESP32-MINI-1, ESP32-DevKitM-1
          else {             return F("ESP32-U4WDH-D"); }    // Max 240MHz, Dual core, QFN 5*5, 4MB embedded flash
        case 5:
          if (rev3)        { return F("ESP32-PICO-V3"); }    // Max 240MHz, Dual core, LGA 7*7, ESP32-PICO-V3-ZERO, ESP32-PICO-V3-ZERO-DevKit
          else {             return F("ESP32-PICO-D4"); }    // Max 240MHz, Dual core, LGA 7*7, 4MB embedded flash, ESP32-PICO-KIT
        case 6:              return F("ESP32-PICO-V3-02");   // Max 240MHz, Dual core, LGA 7*7, 8MB embedded flash, 2MB embedded PSRAM, ESP32-PICO-MINI-02, ESP32-PICO-DevKitM-2
        case 7:              return F("ESP32-D0WDR2-V3");    // Max 240MHz, Dual core, QFN 5*5, ESP32-WROOM-32E, ESP32_WROVER-E, ESP32-DevKitC
      }
#endif  // CONFIG_IDF_TARGET_ESP32
      return F("ESP32");
    }
    case 2: {  // ESP32-S2
      /*
      ESP32-S2 Series
      - 32-bit MCU & 2.4 GHz Wi-Fi
      - High-performance 240 MHz single-core CPU
      - Ultra-low-power performance: fine-grained clock gating, dynamic voltage and frequency scaling
      - Security features: eFuse、flash encryption, secure boot, signature verification, integrated AES, SHA and RSA algorithms
      - Peripherals include 43 GPIOs, 1 full-speed USB OTG interface, SPI, I2S, UART, I2C, LED PWM, LCD interface, camera interface, ADC, DAC, touch sensor, temperature sensor
      - Availability of common cloud connectivity agents and common product features shortens the time to market
      */
#ifdef CONFIG_IDF_TARGET_ESP32S2
      uint32_t psram_ver = REG_GET_FIELD(EFUSE_RD_MAC_SPI_SYS_3_REG, EFUSE_PSRAM_VERSION);
      pkg_version += ((psram_ver & 0xF) * 100);

//      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HDW: ESP32 Model %d, Revision %d, Core %d, Package %d"), chip_info.model, chip_revision, chip_info.cores, chip_ver);

      switch (pkg_version) {
        case 0:              return F("ESP32-S2");           // Max 240MHz, Single core, QFN 7*7, ESP32-S2-WROOM, ESP32-S2-WROVER, ESP32-S2-Saola-1, ESP32-S2-Kaluga-1
        case 1:              return F("ESP32-S2FH2");        // Max 240MHz, Single core, QFN 7*7, 2MB embedded flash, ESP32-S2-MINI-1, ESP32-S2-DevKitM-1
        case 2:              return F("ESP32-S2FH4");        // Max 240MHz, Single core, QFN 7*7, 4MB embedded flash
        case 3:              return F("ESP32-S2FN4R2");      // Max 240MHz, Single core, QFN 7*7, 4MB embedded flash, 2MB embedded PSRAM, , ESP32-S2-MINI-1U, ESP32-S2-DevKitM-1U
        case 100:            return F("ESP32-S2R2");
        case 102:            return F("ESP32-S2FNR2");       // Max 240MHz, Single core, QFN 7*7, 4MB embedded flash, 2MB embedded PSRAM, , Lolin S2 mini
      }
#endif  // CONFIG_IDF_TARGET_ESP32S2
      return F("ESP32-S2");
    }
    case 5: {  // ESP32-C3 = ESP8685 if embedded flash
      /*
      ESP32-C3 Series
      - 32-bit RISC-V MCU & 2.4 GHz Wi-Fi & Bluetooth 5 (LE)
      - 32-bit RISC-V single-core processor with a four-stage pipeline that operates at up to 160 MHz
      - State-of-the-art power and RF performance
      - 400 KB of SRAM and 384 KB of ROM on the chip, and SPI, Dual SPI, Quad SPI, and QPI interfaces that allow connection to flash
      - Reliable security features ensured by RSA-3072-based secure boot, AES-128-XTS-based flash encryption, the innovative digital signature and the HMAC peripheral, hardware acceleration support for cryptographic algorithms
      - Rich set of peripheral interfaces and GPIOs, ideal for various scenarios and complex applications
      */
#ifdef CONFIG_IDF_TARGET_ESP32C3
      switch (pkg_version) {
        case 0:              return F("ESP32-C3");           // Max 160MHz, Single core, QFN 5*5, ESP32-C3-WROOM-02, ESP32-C3-DevKitC-02
//        case 1:              return F("ESP32-C3FH4");        // Max 160MHz, Single core, QFN 5*5, 4MB embedded flash, ESP32-C3-MINI-1, ESP32-C3-DevKitM-1
        case 1:              return F("ESP8685");            // Max 160MHz, Single core, QFN 5*5, 4MB embedded flash, ESP32-C3-MINI-1, ESP32-C3-DevKitM-1
        case 2:              return F("ESP32-C3 AZ");        // QFN32
        case 3:              return F("ESP8686");            // QFN24
      }
#endif  // CONFIG_IDF_TARGET_ESP32C3
      return F("ESP32-C3");
    }
    case 4:     // ESP32-S3(beta2)
    case 6:     // ESP32-S3(beta3)
    case 9:  {  // ESP32-S3
      /*
      ESP32-S3 Series
      - 32-bit MCU & 2.4 GHz Wi-Fi & Bluetooth 5 (LE)
      - Xtensa® 32-bit LX7 dual-core processor that operates at up to 240 MHz
      - 512 KB of SRAM and 384 KB of ROM on the chip, and SPI, Dual SPI, Quad SPI, Octal SPI, QPI, and OPI interfaces that allow connection to flash and external RAM
      - Additional support for vector instructions in the MCU, which provides acceleration for neural network computing and signal processing workloads
      - Peripherals include 45 programmable GPIOs, SPI, I2S, I2C, PWM, RMT, ADC and UART, SD/MMC host and TWAITM
      - Reliable security features ensured by RSA-based secure boot, AES-XTS-based flash encryption, the innovative digital signature and the HMAC peripheral, “World Controller”
      */
#ifdef CONFIG_IDF_TARGET_ESP32S3
      switch (pkg_version) {
        case 0:              return F("ESP32-S3");           // QFN56
        case 1:              return F("ESP32-S3-PICO-1");    // LGA56
      }
#endif  // CONFIG_IDF_TARGET_ESP32S3
      return F("ESP32-S3");                                  // Max 240MHz, Dual core, QFN 7*7, ESP32-S3-WROOM-1, ESP32-S3-DevKitC-1
    }
    case 12: {  // ESP32-C2 = ESP8684 if embedded flash
      /*
      ESP32-C2 Series
      - 32-bit RISC-V MCU & 2.4 GHz Wi-Fi & Bluetooth 5 (LE)
      - 32-bit RISC-V single-core processor that operates at up to 120 MHz
      - State-of-the-art power and RF performance
      - 576 KB ROM, 272 KB SRAM (16 KB for cache) on the chip
      - 14 programmable GPIOs: SPI, UART, I2C, LED PWM controller, General DMA controller (GDMA), SAR ADC, Temperature sensor
      */
#ifdef CONFIG_IDF_TARGET_ESP32C2
      switch (pkg_version) {
        case 0:              return F("ESP32-C2");
        case 1:              return F("ESP32-C2");
      }
#endif  // CONFIG_IDF_TARGET_ESP32C2
      return F("ESP32-C2");
    }
    case 7:     // ESP32-C6(beta)
    case 13: {  // ESP32-C6
      /*
      ESP32-C6 Series
      - 32-bit RISC-V MCU & 2.4 GHz Wi-Fi 6 & Bluetooth 5 (LE) & IEEE 802.15.4
      - 32-bit RISC-V single-core processor that operates at up to 160 MHz
      - State-of-the-art power and RF performance
      - 320 KB ROM, 512 KB SRAM, 16 KB Low-power SRAM on the chip, and works with external flash
      - 30 (QFN40) or 22 (QFN32) programmable GPIOs, with support for SPI, UART, I2C, I2S, RMT, TWAI and PWM
      */
#ifdef CONFIG_IDF_TARGET_ESP32C6
      switch (pkg_version) {
        case 0:              return F("ESP32-C6");
        case 1:              return F("ESP32-C6FH4");
      }
#endif  // CONFIG_IDF_TARGET_ESP32C6
      return F("ESP32-C6");
    }
    case 10:    // ESP32-H2(beta1)
    case 14:    // ESP32-H2(beta2)
    case 16: {  // ESP32-H2
#ifdef CONFIG_IDF_TARGET_ESP32H2
      switch (pkg_version) {
        case 0:              return F("ESP32-H2");
      }
#endif  // CONFIG_IDF_TARGET_ESP32H2
      return F("ESP32-H2");
    }
    case 18: {  // ESP32-P4
#ifdef CONFIG_IDF_TARGET_ESP32P4
      switch (pkg_version) {
        case 0:              return F("ESP32-P4");
      }
#endif  // CONFIG_IDF_TARGET_ESP32P4
      return F("ESP32-P4");
    }
  }
  return F("ESP32");
}

String GetDeviceHardwareRevision(void) {
  // ESP32-S2
  // ESP32-D0WDQ6 v1.0
  // ESP32-C3 v0.3
  // ESP32-C6FH4 v0.0
  String result = GetDeviceHardware();   // ESP32-C3

  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  uint32_t chip_revision = chip_info.revision;       // 16-bit chip revision number (in format MXX; where M - wafer major version, XX - wafer minor version)
  char revision[16];
  snprintf_P(revision, sizeof(revision), PSTR(" v%d.%d"), chip_revision / 100, chip_revision % 100);
  result += revision;                  // ESP32-C3 v0.3

  return result;
}

String GetCodeCores(void) {
#if defined(CORE32SOLO1)
  return F("single-core");
#else
  return F("");
#endif
}

uint32_t ESP_getChipCores(void) {
  return ESP.getChipCores();
}

uint32_t ESP_getChipRevision(void) {
  return ESP.getChipRevision();
}

String ESP_getEfuseMac(void) {
  return String(ESP.getEfuseMac());
}

String WifiGetPhyMode(void) {
  char stemp[10];
  return String(GetTextIndexed(stemp, sizeof(stemp), WiFiHelper::getPhyMode(), kWifiPhyMode));
}

/*********************************************************************************************\
 * High entropy hardware random generator
 * Thanks to DigitalAlchemist
\*********************************************************************************************/

#include <esp_random.h>

// Based on code from https://raw.githubusercontent.com/espressif/esp-idf/master/components/esp32/hw_random.c
uint32_t HwRandom(void) {
  // See for more info on the HW RNG:
  // https://docs.espressif.com/projects/esp-idf/en/latest/esp32s2/api-reference/system/random.html
  return esp_random();
}

/********************************************************************************************/
// Since ESP-IDF 4.4, GPIO matrix or I/O is not reset during a restart
// and GPIO configuration can get stuck because of leftovers
//
// This patched version of pinMode forces a full GPIO reset before setting new mode
//
#include "driver/gpio.h"

extern "C" void ARDUINO_ISR_ATTR __pinMode(uint8_t pin, uint8_t mode);

extern "C" void ARDUINO_ISR_ATTR pinMode(uint8_t pin, uint8_t mode) {
  gpio_reset_pin((gpio_num_t)pin);
  __pinMode(pin, mode);
#ifdef CONFIG_IDF_TARGET_ESP32C3
  // See GpioForceHoldRelay() below
  static uint64_t pin_hold_mask = 0;
  if (!bitRead(pin_hold_mask, pin)) {
    bitSet(pin_hold_mask, pin);
    gpio_hold_dis((gpio_num_t)pin);    // Allow state change
  }
#endif
}

#ifdef CONFIG_IDF_TARGET_ESP32C3
void GpioForceHoldRelay(void) {
  // Only ESP32-C3 toggles outputs on restart unless gpio_hold_en() is called before restart
  // Retain the state when the chip or system is reset, for example, when watchdog time-out or Deep-sleep

//  gpio_force_hold_all();             // This will hold flash/serial too so do not use

  // Use current gpio config
//  for (uint32_t i = 0; i < nitems(TasmotaGlobal.gpio_pin); i++) {
//    if ((TasmotaGlobal.gpio_pin[i] & 0xFFE0) == GPIO_REL1 << 5) {
  // Use future gpio config
  myio template_gp;
  TemplateGpios(&template_gp);
  for (uint32_t i = 0; i < nitems(Settings->my_gp.io); i++) {
    if (((Settings->my_gp.io[i] & 0xFFE0) == GPIO_REL1 << 5) ||
        ((template_gp.io[i] & 0xFFE0) == GPIO_REL1 << 5)) {
      gpio_hold_en((gpio_num_t)i);     // Retain the state when the chip or system is reset, for example, when watchdog time-out or Deep-sleep
    }
  }
}
#endif

/********************************************************************************************/

#endif  // ESP32
