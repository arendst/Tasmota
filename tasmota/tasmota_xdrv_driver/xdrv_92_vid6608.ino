/*
  xdrv_91_esp32_twai.ino - ESP32 TWAI support for Tasmota

  SPDX-FileCopyrightText: 2025 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_VID6608

#define XDRV_92               92

#include "vid6608.h"

void VID6608Init() {}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv92(uint32_t function) {
  bool result = false;

  if (FUNC_INIT == function) {
    VID6608Init();
    return result;
  }
  return result;
}

#endif  // USE_VID6608