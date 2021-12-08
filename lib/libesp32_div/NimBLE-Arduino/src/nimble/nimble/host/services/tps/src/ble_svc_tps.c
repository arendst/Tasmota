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
#include "../include/services/tps/ble_svc_tps.h"

/* XXX: We shouldn't be including the host's private header files.  The host
 * API needs to be updated with a function to query the advertising transmit
 * power.
 */
#include "nimble/nimble/host/src/ble_hs_hci_priv.h"

int8_t ble_svc_tps_tx_power_level;

/* Access function */
static int
ble_svc_tps_access(uint16_t conn_handle, uint16_t attr_handle,
                   struct ble_gatt_access_ctxt *ctxt, void *arg);

static const struct ble_gatt_svc_def ble_svc_tps_defs[] = {
    {
        /*** Service: Tx Power Service. */
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = BLE_UUID16_DECLARE(BLE_SVC_TPS_UUID16),
        .characteristics = (struct ble_gatt_chr_def[]) { {
            /*** Characteristic: Tx Power Level. */
            .uuid = BLE_UUID16_DECLARE(BLE_SVC_TPS_CHR_UUID16_TX_POWER_LEVEL),
            .access_cb = ble_svc_tps_access,
            .flags = BLE_GATT_CHR_F_READ,
        }, {
            0, /* No more characteristics in this service. */
        } },
    },

    {
        0, /* No more services. */
    },
};

/**
 * Simple read access callback for the tx power level
 * characteristic.
 */
static int
ble_svc_tps_access(uint16_t conn_handle, uint16_t attr_handle,
                   struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    int rc;

    assert(ctxt->chr == &ble_svc_tps_defs[0].characteristics[0]);

    switch (ctxt->op) {
    case BLE_GATT_ACCESS_OP_READ_CHR:
        rc = ble_hs_hci_util_read_adv_tx_pwr(&ble_svc_tps_tx_power_level);
        if (rc != 0) {
            return BLE_ATT_ERR_UNLIKELY;
        }

        rc = os_mbuf_append(ctxt->om, &ble_svc_tps_tx_power_level,
                            sizeof ble_svc_tps_tx_power_level);
        return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;

    default:
        assert(0);
        return BLE_ATT_ERR_UNLIKELY;
    }

    return 0;
}

/**
 * Initialize the TPS
 */
void
ble_svc_tps_init(void)
{
    int rc;

    /* Ensure this function only gets called by sysinit. */
    SYSINIT_ASSERT_ACTIVE();

    rc = ble_gatts_count_cfg(ble_svc_tps_defs);
    SYSINIT_PANIC_ASSERT(rc == 0);

    rc = ble_gatts_add_svcs(ble_svc_tps_defs);
    SYSINIT_PANIC_ASSERT(rc == 0);
}
