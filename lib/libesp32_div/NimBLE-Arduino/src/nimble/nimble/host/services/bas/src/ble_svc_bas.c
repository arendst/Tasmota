/**
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
#include <string.h>

#include "nimble/porting/nimble/include/sysinit/sysinit.h"
#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#include "nimble/nimble/host/include/host/ble_hs.h"
#include "nimble/nimble/host/include/host/ble_gap.h"
#include "../include/services/bas/ble_svc_bas.h"

/* Characteristic value handles */
#if MYNEWT_VAL(BLE_SVC_BAS_BATTERY_LEVEL_NOTIFY_ENABLE) > 0
static uint16_t ble_svc_bas_battery_handle;
#endif

/* Battery level */
uint8_t ble_svc_bas_battery_level;

/* Access function */
static int
ble_svc_bas_access(uint16_t conn_handle, uint16_t attr_handle,
                   struct ble_gatt_access_ctxt *ctxt, void *arg);

static const struct ble_gatt_svc_def ble_svc_bas_defs[] = {
    {
        /*** Battery Service. */
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = BLE_UUID16_DECLARE(BLE_SVC_BAS_UUID16),
        .characteristics = (struct ble_gatt_chr_def[]) { {
	    /*** Battery level characteristic */
            .uuid = BLE_UUID16_DECLARE(BLE_SVC_BAS_CHR_UUID16_BATTERY_LEVEL),
            .access_cb = ble_svc_bas_access,
#if MYNEWT_VAL(BLE_SVC_BAS_BATTERY_LEVEL_NOTIFY_ENABLE) > 0
	    .val_handle = &ble_svc_bas_battery_handle,
#endif
            .flags = BLE_GATT_CHR_F_READ |
#if MYNEWT_VAL(BLE_SVC_BAS_BATTERY_LEVEL_NOTIFY_ENABLE) > 0
	             BLE_GATT_CHR_F_NOTIFY |
#endif
	             MYNEWT_VAL(BLE_SVC_BAS_BATTERY_LEVEL_READ_PERM),
	    }, {
            0, /* No more characteristics in this service. */
        } },
    },

    {
        0, /* No more services. */
    },
};

/**
 * BAS access function
 */
static int
ble_svc_bas_access(uint16_t conn_handle, uint16_t attr_handle,
                          struct ble_gatt_access_ctxt *ctxt,
                          void *arg)
{
    uint16_t uuid16 = ble_uuid_u16(ctxt->chr->uuid);
    int rc;

    switch (uuid16) {
    case BLE_SVC_BAS_CHR_UUID16_BATTERY_LEVEL:
        assert(ctxt->op == BLE_GATT_ACCESS_OP_READ_CHR);
        rc = os_mbuf_append(ctxt->om, &ble_svc_bas_battery_level,
                            sizeof ble_svc_bas_battery_level);
        return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;

    default:
        assert(0);
        return BLE_ATT_ERR_UNLIKELY;
    }
}


/**
 * Set the battery level, must be between 0 and 100.
 * If greater than 100, it will be silently set to 100.
 */
int
ble_svc_bas_battery_level_set(uint8_t level) {
    if (level > 100)
	level = 100;
    if (ble_svc_bas_battery_level != level) {
	ble_svc_bas_battery_level = level;
#if MYNEWT_VAL(BLE_SVC_BAS_BATTERY_LEVEL_NOTIFY_ENABLE) > 0
        ble_gatts_chr_updated(ble_svc_bas_battery_handle);
#endif
    }
    return 0;
}

/**
 * Initialize the Battery Service.
 */
void
ble_svc_bas_init(void)
{
    int rc;

    /* Ensure this function only gets called by sysinit. */
    SYSINIT_ASSERT_ACTIVE();

    rc = ble_gatts_count_cfg(ble_svc_bas_defs);
    SYSINIT_PANIC_ASSERT(rc == 0);

    rc = ble_gatts_add_svcs(ble_svc_bas_defs);
    SYSINIT_PANIC_ASSERT(rc == 0);
}
