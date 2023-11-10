/*
  support_esp8266.ino - ESP8266 specific support for Tasmota

  SPDX-FileCopyrightText: 2023 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef ESP8266
/*********************************************************************************************\
 * ESP8266 and ESP8285 Support
\*********************************************************************************************/

extern "C" {
extern struct rst_info resetInfo;
}

/*********************************************************************************************\
 * Core overrides executed by core
\*********************************************************************************************/

// Add below line to tasmota_globals.h
// extern "C" void resetPins();
//
// This function overrules __resetPins() which is executed by core init() as initPins() in core_esp8266_wiring.cpp
//
// 20221229 - (v12.3.1.2) Enabled with additional check to execute on power on only to fix relay clicks on power on
// 20200321 - (v8.2.0.1) Disable core functionality to fix relay clicks on restart after OTA - make function return without setting pinMode
void resetPins() {
  if ((resetInfo.reason == REASON_DEFAULT_RST) || (resetInfo.reason == REASON_EXT_SYS_RST)) {
    // Only perform at power on
    for (int i = 0; i <= 5; ++i) {
      pinMode(i, INPUT);
    }
    // pins 6-11 are used for the SPI flash interface ESP8266
    for (int i = 12; i <= 16; ++i) {
      pinMode(i, INPUT);
    }
  }
}

/*********************************************************************************************\
 * Hardware related
\*********************************************************************************************/

void HwWdtDisable(void) {
  *((volatile uint32_t*) 0x60000900) &= ~(1);  // Hardware WDT OFF
}

void HwWdtEnable(void) {
  *((volatile uint32_t*) 0x60000900) |= 1;     // Hardware WDT ON
}

void WdtDisable(void) {
  ESP.wdtDisable();
  HwWdtDisable();
}

void WdtEnable(void) {
  HwWdtEnable();
  ESP.wdtEnable(0);
}

/*********************************************************************************************\
 * ESP8266 specifics
\*********************************************************************************************/

uint32_t ESP_ResetInfoReason(void) {
  return resetInfo.reason;
}

String ESP_getResetReason(void) {
  return ESP.getResetReason();
}

uint32_t ESP_getChipId(void) {
  return ESP.getChipId();
}

uint32_t ESP_getFreeSketchSpace(void) {
  return ESP.getFreeSketchSpace();
}

uint32_t ESP_getSketchSize(void) {
  return ESP.getSketchSize();
}

uint32_t ESP_getFreeHeap(void) {
  return ESP.getFreeHeap();
}

uint32_t ESP_getFreeHeap1024(void) {
  return ESP_getFreeHeap() / 1024;
}
/*
float ESP_getFreeHeap1024(void) {
  return ((float)ESP_getFreeHeap()) / 1024;
}
*/

uint32_t ESP_getFlashChipId(void) {
  return ESP.getFlashChipId();
}

uint32_t ESP_getFlashChipRealSize(void) {
  return ESP.getFlashChipRealSize();
}

uint32_t ESP_getFlashChipSize(void) {
  return ESP.getFlashChipSize();
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
  /*
  ESP8266 SoCs
  - 32-bit MCU & 2.4 GHz Wi-Fi
  - High-performance 160 MHz single-core CPU
  - +19.5 dBm output power ensures a good physical range
  - Sleep current is less than 20 μA, making it suitable for battery-powered and wearable-electronics applications
  - Peripherals include UART, GPIO, I2C, I2S, SDIO, PWM, ADC and SPI
  */
  // esptool.py get_efuses
  uint32_t efuse0 = *(uint32_t*)(0x3FF00050);
//  uint32_t efuse1 = *(uint32_t*)(0x3FF00054);
  uint32_t efuse2 = *(uint32_t*)(0x3FF00058);
  uint32_t efuse3 = *(uint32_t*)(0x3FF0005C);

  bool r0_4 = efuse0 & (1 << 4);                   // ESP8285
  bool r2_16 = efuse2 & (1 << 16);                 // ESP8285
  if (r0_4 || r2_16) {                             // ESP8285
    //                                                              1M 2M 2M 4M flash size
    //   r0_4                                                       1  1  0  0
    bool r3_25 = efuse3 & (1 << 25);               // flash matrix  0  0  1  1
    bool r3_26 = efuse3 & (1 << 26);               // flash matrix  0  1  0  1
    bool r3_27 = efuse3 & (1 << 27);               // flash matrix  0  0  0  0
    uint32_t pkg_version = 0;
    if (!r3_27) {
      if (r0_4 && !r3_25) {
        pkg_version = (r3_26) ? 2 : 1;
      }
      else if (!r0_4 && r3_25) {
        pkg_version = (r3_26) ? 4 : 2;
      }
    }
    bool max_temp = efuse0 & (1 << 5);             // Max flash temperature (0 = 85C, 1 = 105C)
    switch (pkg_version) {
      case 1:
        if (max_temp) { return F("ESP8285H08"); }  // 1M flash
        else {          return F("ESP8285N08"); }
      case 2:
        if (max_temp) { return F("ESP8285H16"); }  // 2M flash
        else {          return F("ESP8285N16"); }
      case 4:
        if (max_temp) { return F("ESP8285H32"); }  // 4M flash
        else {          return F("ESP8285N32"); }
    }
    return F("ESP8285");
  }
  return F("ESP8266EX");
}

String GetDeviceHardwareRevision(void) {
  // No known revisions for ESP8266/85
  return GetDeviceHardware();
}

String GetCodeCores(void) {
  return F("");
}

/*********************************************************************************************\
 * High entropy hardware random generator
 * Thanks to DigitalAlchemist
\*********************************************************************************************/

// Based on code from https://raw.githubusercontent.com/espressif/esp-idf/master/components/esp32/hw_random.c
uint32_t HwRandom(void) {
  // https://web.archive.org/web/20160922031242/http://esp8266-re.foogod.com/wiki/Random_Number_Generator
  #define _RAND_ADDR 0x3FF20E44UL
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

#endif  // ESP8266