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

#include <assert.h>

#include "nimble/porting/nimble/include/sysinit/sysinit.h"
#include "nimble/nimble/host/include/host/ble_hs.h"
#include "../include/services/gatt/ble_svc_gatt.h"

static uint16_t ble_svc_gatt_changed_val_handle;
static uint16_t ble_svc_gatt_start_handle;
static uint16_t ble_svc_gatt_end_handle;

static int
ble_svc_gatt_access(uint16_t conn_handle, uint16_t attr_handle,
                    struct ble_gatt_access_ctxt *ctxt, void *arg);

static const struct ble_gatt_svc_def ble_svc_gatt_defs[] = {
    {
        /*** Service: GATT */
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = BLE_UUID16_DECLARE(BLE_GATT_SVC_UUID16),
        .characteristics = (struct ble_gatt_chr_def[]) { {
            .uuid = BLE_UUID16_DECLARE(BLE_SVC_GATT_CHR_SERVICE_CHANGED_UUID16),
            .access_cb = ble_svc_gatt_access,
            .val_handle = &ble_svc_gatt_changed_val_handle,
            .flags = BLE_GATT_CHR_F_INDICATE,
        }, {
            0, /* No more characteristics in this service. */
        } },
    },

    {
        0, /* No more services. */
    },
};

static int
ble_svc_gatt_access(uint16_t conn_handle, uint16_t attr_handle,
                    struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    uint8_t *u8p;

    /* The only operation allowed for this characteristic is indicate.  This
     * access callback gets called by the stack when it needs to read the
     * characteristic value to populate the outgoing indication command.
     * Therefore, this callback should only get called during an attempt to
     * read the characteristic.
     */
    assert(ctxt->op == BLE_GATT_ACCESS_OP_READ_CHR);
    assert(ctxt->chr == &ble_svc_gatt_defs[0].characteristics[0]);

    u8p = os_mbuf_extend(ctxt->om, 4);
    if (u8p == NULL) {
        return BLE_HS_ENOMEM;
    }

    put_le16(u8p + 0, ble_svc_gatt_start_handle);
    put_le16(u8p + 2, ble_svc_gatt_end_handle);

    return 0;
}

/**
 * Indicates a change in attribute assignment to all subscribed peers.
 * Unconnected bonded peers receive an indication when they next connect.
 *
 * @param start_handle          The start of the affected handle range.
 * @param end_handle            The end of the affected handle range.
 */
void
ble_svc_gatt_changed(uint16_t start_handle, uint16_t end_handle)
{
    ble_svc_gatt_start_handle = start_handle;
    ble_svc_gatt_end_handle = end_handle;
    ble_gatts_chr_updated(ble_svc_gatt_changed_val_handle);
}

void
ble_svc_gatt_init(void)
{
    int rc;

    /* Ensure this function only gets called by sysinit. */
    SYSINIT_ASSERT_ACTIVE();

    rc = ble_gatts_count_cfg(ble_svc_gatt_defs);
    SYSINIT_PANIC_ASSERT(rc == 0);

    rc = ble_gatts_add_svcs(ble_svc_gatt_defs);
    SYSINIT_PANIC_ASSERT(rc == 0);
}
