/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef H_BLE_HW_
#define H_BLE_HW_

#ifdef __cplusplus
extern "C" {
#endif

#include "nimble/porting/nimble/include/syscfg/syscfg.h"

#if defined(ARCH_sim)
#define BLE_USES_HW_WHITELIST   (0)
#else
#define BLE_USES_HW_WHITELIST   MYNEWT_VAL(BLE_HW_WHITELIST_ENABLE)
#endif

/* Returns the number of hw whitelist elements */
uint8_t ble_hw_whitelist_size(void);

/* Clear the whitelist */
void ble_hw_whitelist_clear(void);

/* Remove a device from the hw whitelist */
void ble_hw_whitelist_rmv(const uint8_t *addr, uint8_t addr_type);

/* Add a device to the hw whitelist */
int ble_hw_whitelist_add(const uint8_t *addr, uint8_t addr_type);

/* Enable hw whitelisting */
void ble_hw_whitelist_enable(void);

/* Enable hw whitelisting */
void ble_hw_whitelist_disable(void);

/* Boolean function returning true if address matches a whitelist entry */
int ble_hw_whitelist_match(void);

/* Encrypt data */
struct ble_encryption_block;
int ble_hw_encrypt_block(struct ble_encryption_block *ecb);

/* Random number generation */
typedef void (*ble_rng_isr_cb_t)(uint8_t rnum);
int ble_hw_rng_init(ble_rng_isr_cb_t cb, int bias);

/**
 * Start the random number generator
 *
 * @return int
 */
int ble_hw_rng_start(void);

/**
 * Stop the random generator
 *
 * @return int
 */
int ble_hw_rng_stop(void);

/**
 * Read the random number generator.
 *
 * @return uint8_t
 */
uint8_t ble_hw_rng_read(void);

/*  Clear the resolving list*/
void ble_hw_resolv_list_clear(void);

/* Add a device to the hw resolving list */
int ble_hw_resolv_list_add(uint8_t *irk);

/* Remove a device from the hw resolving list */
void ble_hw_resolv_list_rmv(int index);

/* Returns the size of the whitelist in HW */
uint8_t ble_hw_resolv_list_size(void);

/* Enable the resolving list */
void ble_hw_resolv_list_enable(void);

/* Disables resolving list devices */
void ble_hw_resolv_list_disable(void);

/* Returns index of resolved address; -1 if not resolved */
int ble_hw_resolv_list_match(void);

/* Returns public device address or -1 if not present */
int ble_hw_get_public_addr(ble_addr_t *addr);

/* Returns random static address or -1 if not present */
int ble_hw_get_static_addr(ble_addr_t *addr);

#ifdef __cplusplus
}
#endif

#endif /* H_BLE_HW_ */
