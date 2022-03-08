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

#ifndef H_BLE_HS_PERIODIC_SYNC_
#define H_BLE_HS_PERIODIC_SYNC_

#include <inttypes.h>
#include "nimble/porting/nimble/include/os/queue.h"
#ifdef __cplusplus
extern "C" {
#endif

struct ble_hs_periodic_sync {
    SLIST_ENTRY(ble_hs_periodic_sync) next;
    uint16_t   sync_handle;
    ble_addr_t advertiser_addr;
    uint8_t    adv_sid;

    ble_gap_event_fn *cb;
    void *cb_arg;

    struct ble_npl_event lost_ev;
};

struct ble_hs_periodic_sync *ble_hs_periodic_sync_alloc(void);
void ble_hs_periodic_sync_free(struct ble_hs_periodic_sync *psync);
void ble_hs_periodic_sync_insert(struct ble_hs_periodic_sync *psync);
void ble_hs_periodic_sync_remove(struct ble_hs_periodic_sync *psync);
struct ble_hs_periodic_sync *ble_hs_periodic_sync_find_by_handle(uint16_t sync_handle);
struct ble_hs_periodic_sync *ble_hs_periodic_sync_find(const ble_addr_t *addr,
                                                       uint8_t sid);
struct ble_hs_periodic_sync *ble_hs_periodic_sync_first(void);
int ble_hs_periodic_sync_init(void);

#ifdef __cplusplus
}
#endif

#endif
