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

#include <string.h>
#include <errno.h>
#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#include "nimble/porting/nimble/include/os/os.h"
#include "nimble/nimble/host/include/host/ble_hs_id.h"
#include "ble_hs_priv.h"

#if MYNEWT_VAL(BLE_PERIODIC_ADV)
static SLIST_HEAD(, ble_hs_periodic_sync) g_ble_hs_periodic_sync_handles;
static struct os_mempool ble_hs_periodic_sync_pool;

static os_membuf_t ble_hs_psync_elem_mem[
    OS_MEMPOOL_SIZE(MYNEWT_VAL(BLE_MAX_PERIODIC_SYNCS),
                    sizeof (struct ble_hs_periodic_sync))
];

struct ble_hs_periodic_sync *
ble_hs_periodic_sync_alloc(void)
{
    struct ble_hs_periodic_sync *psync;

    psync = os_memblock_get(&ble_hs_periodic_sync_pool);
    if (psync) {
        memset(psync, 0, sizeof(*psync));
    }

    return psync;
}

void
ble_hs_periodic_sync_free(struct ble_hs_periodic_sync *psync)
{
    int rc;

    if (psync == NULL) {
        return;
    }

#if MYNEWT_VAL(BLE_HS_DEBUG)
    memset(psync, 0xff, sizeof *psync);
#endif
    rc = os_memblock_put(&ble_hs_periodic_sync_pool, psync);
    BLE_HS_DBG_ASSERT_EVAL(rc == 0);
}

void
ble_hs_periodic_sync_insert(struct ble_hs_periodic_sync *psync)
{
    BLE_HS_DBG_ASSERT(ble_hs_locked_by_cur_task());

    BLE_HS_DBG_ASSERT_EVAL(
                       ble_hs_periodic_sync_find_by_handle(psync->sync_handle) == NULL);

    SLIST_INSERT_HEAD(&g_ble_hs_periodic_sync_handles, psync, next);
}

void
ble_hs_periodic_sync_remove(struct ble_hs_periodic_sync *psync)
{
    BLE_HS_DBG_ASSERT(ble_hs_locked_by_cur_task());

    SLIST_REMOVE(&g_ble_hs_periodic_sync_handles, psync, ble_hs_periodic_sync,
                 next);
}

struct ble_hs_periodic_sync *
ble_hs_periodic_sync_find_by_handle(uint16_t sync_handle)
{
    struct ble_hs_periodic_sync *psync;

    BLE_HS_DBG_ASSERT(ble_hs_locked_by_cur_task());

    SLIST_FOREACH(psync, &g_ble_hs_periodic_sync_handles, next) {
        if (psync->sync_handle == sync_handle) {
            return psync;
        }
    }

    return NULL;
}

struct ble_hs_periodic_sync *
ble_hs_periodic_sync_find(const ble_addr_t *addr, uint8_t sid)
{
    struct ble_hs_periodic_sync *psync;

    BLE_HS_DBG_ASSERT(ble_hs_locked_by_cur_task());

    if (!addr) {
        return NULL;
    }

    SLIST_FOREACH(psync, &g_ble_hs_periodic_sync_handles, next) {
        if ((ble_addr_cmp(&psync->advertiser_addr, addr) == 0) &&
                (psync->adv_sid == sid)) {
            return psync;
        }
    }

    return NULL;
}

/**
 * Retrieves the first periodic discovery handle in the list.
 */
struct ble_hs_periodic_sync *
ble_hs_periodic_sync_first(void)
{
    struct ble_hs_periodic_sync *psync;

    ble_hs_lock();
    psync = SLIST_FIRST(&g_ble_hs_periodic_sync_handles);
    ble_hs_unlock();

    return psync;
}

int
ble_hs_periodic_sync_init(void)
{
    int rc;

    rc = os_mempool_init(&ble_hs_periodic_sync_pool,
                         MYNEWT_VAL(BLE_MAX_PERIODIC_SYNCS),
                         sizeof (struct ble_hs_periodic_sync),
                         ble_hs_psync_elem_mem, "ble_hs_periodic_disc_pool");
    if (rc != 0) {
        return BLE_HS_EOS;
    }

    SLIST_INIT(&g_ble_hs_periodic_sync_handles);

    return 0;
}
#endif
