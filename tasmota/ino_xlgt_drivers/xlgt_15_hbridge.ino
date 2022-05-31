/*
  xlgt_15_hbridge.ino - HBridge support for Tasmota

  SPDX-FileCopyrightText: 2021 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

//#define USE_LIGHT_HBRIDGE

#ifdef ESP32
#ifdef USE_LIGHT
#ifdef USE_LIGHT_HBRIDGE
/*********************************************************************************************\
 *
\*********************************************************************************************/

#define XLGT_15               15

/*
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_attr.h"

#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"
*/

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "driver/mcpwm.h"
#include "string.h"
#include "freertos/queue.h"
#include "esp_attr.h"
#include "soc/rtc.h"
#include "soc/mcpwm_reg.h"    // ESP32C3 has no mcpwm
#include "soc/mcpwm_struct.h"

mcpwm_pin_config_t pin_config;

bool HBSetChannelsFromFunc(void) {


  return false;
}

void HBModuleSelected(void) {
  if (!ValidTemplate(PSTR("HBRIDGE POC1"))) { return; }

//  if (!PinUsed(GPIO_OUTPUT_HI) || !PinUsed(GPIO_OUTPUT_LO)) { return; }


  TasmotaGlobal.light_type = LT_NU8;      // LT_HBRIDGE
  TasmotaGlobal.light_driver = XLGT_15;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xlgt15(uint8_t function) {
  bool result = false;

  switch (function) {
    case FUNC_SET_CHANNELS:
      result = HBSetChannelsFromFunc();
      break;
    case FUNC_MODULE_INIT:
      HBModuleSelected();
      break;
  }
  return result;
}

#endif  // USE_LIGHT_HBRIDGE
#endif  // USE_LIGHT
#endif  // ESP32