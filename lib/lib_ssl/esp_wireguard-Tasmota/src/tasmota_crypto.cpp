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
} g_dh_curve25519;

static void run_crypto_scalarmult_curve25519(void) {
  g_dh_curve25519.result = BR_EC25519_IMPL.mul(g_dh_curve25519.q, 32, g_dh_curve25519.n, 32, BR_EC_curve25519);
}

extern "C" int crypto_scalarmult_curve25519(unsigned char *q, const unsigned char *n,const unsigned char *p) {
  for (int32_t i=0; i<32; i++) {
    g_dh_curve25519.q[i] = p[i];
    g_dh_curve25519.n[i] = n[31-i];
  }

#ifdef ESP32
  const char *taskName = pcTaskGetName(NULL);
  if (uxTaskGetStackHighWaterMark(nullptr) < 2000) {
    //Allocate a stack buffer, from heap or as a static form:
    StackType_t *shared_stack = (StackType_t*) heap_caps_malloc(4096 * sizeof(StackType_t), MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL);
    //Allocate a mutex to protect its usage:
    SemaphoreHandle_t lock = xSemaphoreCreateMutex();
    //Call the desired function using the macro helper:
    esp_execute_shared_stack_function(lock,
                                    shared_stack,
                                    4096,
                                    run_crypto_scalarmult_curve25519);

    vSemaphoreDelete(lock);
    free(shared_stack);
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
    return 1;   // Failure
  }
}
