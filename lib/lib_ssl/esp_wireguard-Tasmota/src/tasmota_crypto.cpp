/*
  tasmota_crypto.cpp - crypto layer to call bearssl from wireguard

  Copyright (C) 2025  Theo Arends, Stephan Hadinger

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

#include <Arduino.h>
#include <t_bearssl.h>
#ifdef ESP32
  #include "freertos/task.h"
  #include "freertos/FreeRTOS.h"
  #include "esp_expression_with_stack.h"
#endif // ESP32

//**************************************************************************************************************
// enable AddLog support within a C++ library
extern void AddLog(uint32_t loglevel, PGM_P formatP, ...);
enum LoggingLevels {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE};
//**************************************************************************************************************

#define BR_EC25519_IMPL  br_ec_c25519_m15  // BearSSL implementation for Curve 25519

// we need a global 
static struct dh_curve25519_t {
  uint8_t q[32];
  uint8_t n[32];
  int result;
#ifdef ESP32
  SemaphoreHandle_t crypto_done_sem;
  TaskHandle_t crypto_task_handle;
#endif // ESP32
} g_dh_curve25519;

static void run_crypto_scalarmult_curve25519(void) {
  g_dh_curve25519.result = BR_EC25519_IMPL.mul(g_dh_curve25519.q, 32, g_dh_curve25519.n, 32, BR_EC_curve25519);
}

#ifdef ESP32
// In case we are short in stack size, we run a FreeRTOS task instead with this entry point
static void task_crypto_scalarmult_curve25519(void* pvParameters) {
  run_crypto_scalarmult_curve25519();
  if(g_dh_curve25519.crypto_done_sem) {
      xSemaphoreGive(g_dh_curve25519.crypto_done_sem);
  }
  vTaskDelete(NULL);
}
#endif // ESP32

extern "C" int crypto_scalarmult_curve25519(unsigned char *q, const unsigned char *n,const unsigned char *p) {
  for (int32_t i=0; i<32; i++) {
    g_dh_curve25519.q[i] = p[i];
    g_dh_curve25519.n[i] = n[31-i];
  }
  g_dh_curve25519.result = 0;
#ifdef ESP32
  g_dh_curve25519.crypto_done_sem = NULL;
  g_dh_curve25519.crypto_task_handle = NULL;

  const char *taskName = pcTaskGetName(NULL);
  if (uxTaskGetStackHighWaterMark(nullptr) < 2000) {
    // we don't get the actual stack size but the lowest observed. Unfortunately we don't get the actual stack
    // so uxTaskGetStackHighWaterMark() is the best proxy we've got

    // create a new FreeRTOS task and a Semaphore to indicate the computation is done
    g_dh_curve25519.crypto_done_sem = xSemaphoreCreateBinary();
    if (g_dh_curve25519.crypto_done_sem != NULL) {
      xTaskCreatePinnedToCore(task_crypto_scalarmult_curve25519, "crypto_scalarmult_curve25519",
                              4096, NULL, 5/*Medium-High*/, &g_dh_curve25519.crypto_task_handle, 0);

      // Wait for completion (timeout 1s)
      if (xSemaphoreTake(g_dh_curve25519.crypto_done_sem, pdMS_TO_TICKS(1000)) == pdTRUE) {
        // all good
      } else {
        if (g_dh_curve25519.crypto_task_handle) {
            vTaskDelete(g_dh_curve25519.crypto_task_handle);
        }
      }

      vSemaphoreDelete(g_dh_curve25519.crypto_done_sem);
      g_dh_curve25519.crypto_done_sem = NULL;
      g_dh_curve25519.crypto_task_handle = NULL;
    }
  } else {
    run_crypto_scalarmult_curve25519();
  }
#else
  run_crypto_scalarmult_curve25519();
#endif

  if (g_dh_curve25519.result) {
    for (int32_t i=0; i<32; i++) {
      q[i] = g_dh_curve25519.q[i];
    }
    return 0;   // Success
  } else {
    AddLog(LOG_LEVEL_INFO, "WG : crypto_scalarmult_curve25519 failed (not enough memory)");
    return 1;   // Failure
  }
}
