/*
  xdrv_91_esp32_twai.ino - ESP32 TWAI support for Tasmota

  SPDX-FileCopyrightText: 2025 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_VID6608

#define XDRV_92             92
#define VID6608_MAX_DRIVES  4

#include "vid6608.h"

bool vid6608Present = false;
vid6608 *vid6608Drives[VID6608_MAX_DRIVES];

int PinLocal(uint32_t gpio, uint32_t index = 0) {
  uint16_t real_gpio = gpio << 5;
  uint16_t mask = 0xFFE0;
  if (index < GPIO_ANY) {
    real_gpio += index;
    mask = 0xFFFF;
  }
  for (uint32_t i = 0; i < nitems(TasmotaGlobal.gpio_pin); i++) {
    AddLog(LOG_LEVEL_INFO, PSTR("Pin: %d , %d != %d"), TasmotaGlobal.gpio_pin[i], mask, real_gpio);
    if ((TasmotaGlobal.gpio_pin[i] & mask) == real_gpio) {
      return i;              // Pin number configured for gpio
    }
  }
  return -1;                 // No pin used for gpio
}

void VID6608Init() {
  AddLog(LOG_LEVEL_INFO, PSTR("VID6608: Init"));
  for (uint32_t x = 0; x < VID6608_MAX_DRIVES; x++) {
    AddLog(LOG_LEVEL_INFO, PSTR("VID6608: test pin %d: %d, %d"), x, PinLocal(GPIO_VID6608_F, x), PinLocal(GPIO_VID6608_CW, x));
    if (PinUsed(GPIO_VID6608_F, x) && PinUsed(GPIO_VID6608_CW, x)) {
      // We have motor defined at number x
      uint32_t pinStep = Pin(GPIO_VID6608_F, x);
      uint32_t pinDir = Pin(GPIO_VID6608_CW, x);
      vid6608Drives[x] = new vid6608(pinStep, pinDir);
      vid6608Present = true;
      AddLog(LOG_LEVEL_INFO, PSTR("VID6608: add drive at pin %d, %d"), pinStep, pinDir);
      vid6608Drives[x]->zero();
    } else {
      vid6608Drives[x] = nullptr;
    }
  }
}

void VID6608SetPos(uint8_t index, uint16_t pos) {
  vid6608 *driver = vid6608Drives[index];
  if (driver) {
    driver->moveTo(pos);
  }
}

bool VID6608Loop() {
  for (uint8_t x = 0; x < VID6608_MAX_DRIVES; x++) {
    vid6608 *driver = vid6608Drives[x];
    if (driver) {
      driver->loop();
    }
  }
  return true;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv92(uint32_t function) {
  // Driver initalization
  if (FUNC_INIT == function) {
    VID6608Init();
    return false;
  }  
  // We are not initilized?
  if (!vid6608Present) {
    return false;
  }
  // Normal callbacks  
  bool result = false;
  
  switch (function) {
    case FUNC_LOOP:
      result = VID6608Loop();
  }
  
  return result;
}

#endif  // USE_VID6608
