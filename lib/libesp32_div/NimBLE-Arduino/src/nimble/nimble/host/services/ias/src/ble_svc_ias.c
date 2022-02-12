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
#include "nimble/nimble/host/include/host/ble_hs.h"
#include "../include/services/ias/ble_svc_ias.h"

/* Callback function */
static ble_svc_ias_event_fn *ble_svc_ias_cb_fn;

/* Alert level */
static uint8_t ble_svc_ias_alert_level;

/* Write characteristic function */
static int
ble_svc_ias_chr_write(struct os_mbuf *om, uint16_t min_len,
                      uint16_t max_len, void *dst,
                      uint16_t *len);

/* Access function */
static int
ble_svc_ias_access(uint16_t conn_handle, uint16_t attr_handle,
                   struct ble_gatt_access_ctxt *ctxt, void *arg);

static const struct ble_gatt_svc_def ble_svc_ias_defs[] = {
    {
        /*** Service: Immediate Alert Service (IAS). */
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = BLE_UUID16_DECLARE(BLE_SVC_IAS_UUID16),
        .characteristics = (struct ble_gatt_chr_def[]) { {
            /*** Characteristic: Alert Level. */
            .uuid = BLE_UUID16_DECLARE(BLE_SVC_IAS_CHR_UUID16_ALERT_LEVEL),
            .access_cb = ble_svc_ias_access,
            .flags = BLE_GATT_CHR_F_WRITE_NO_RSP,
        }, {
            0, /* No more characteristics in this service. */
        } },
    },

    {
        0, /* No more services. */
    },
};

/**
 * Writes the received value from a characteristic write to
 * the given destination.
 */
static int
ble_svc_ias_chr_write(struct os_mbuf *om, uint16_t min_len,
                      uint16_t max_len, void *dst,
                      uint16_t *len)
{
    uint16_t om_len;
    int rc;

    om_len = OS_MBUF_PKTLEN(om);
    if (om_len < min_len || om_len > max_len) {
        return BLE_ATT_ERR_INVALID_ATTR_VALUE_LEN;
    }

    rc = ble_hs_mbuf_to_flat(om, dst, max_len, len);
    if (rc != 0) {
        return BLE_ATT_ERR_UNLIKELY;
    }

    return 0;
}

/**
 * Simple write access callback for the alert level
 * characteristic.
 */
static int
ble_svc_ias_access(uint16_t conn_handle, uint16_t attr_handle,
                   struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    int rc;

    assert(ctxt->chr == &ble_svc_ias_defs[0].characteristics[0]);

    switch (ctxt->op) {
    case BLE_GATT_ACCESS_OP_WRITE_CHR:
        rc = ble_svc_ias_chr_write(ctxt->om,
                                   sizeof ble_svc_ias_alert_level,
                                   sizeof ble_svc_ias_alert_level,
                                   &ble_svc_ias_alert_level, NULL);
	/* Call the IAS event function */
	if (ble_svc_ias_cb_fn) {
		ble_svc_ias_cb_fn(ble_svc_ias_alert_level);
	}
        return rc;

    default:
        assert(0);
        return BLE_ATT_ERR_UNLIKELY;
    }

    return 0;
}

/**
 * Designates the specified function as the IAS callback.  This callback is
 * necessary for this service to function properly.
 *
 * @param cb                        The callback function to call when
 *                                      the client signals an alert.
 */
void
ble_svc_ias_set_cb(ble_svc_ias_event_fn *cb)
{
    ble_svc_ias_cb_fn = cb;
}

/**
 * Initialize the IAS package.
 */
void
ble_svc_ias_init(void)
{
    int rc;

    /* Ensure this function only gets called by sysinit. */
    SYSINIT_ASSERT_ACTIVE();

    rc = ble_gatts_count_cfg(ble_svc_ias_defs);
    SYSINIT_PANIC_ASSERT(rc == 0);

    rc = ble_gatts_add_svcs(ble_svc_ias_defs);
    SYSINIT_PANIC_ASSERT(rc == 0);
}
