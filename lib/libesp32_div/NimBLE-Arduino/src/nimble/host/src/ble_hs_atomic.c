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

#include "ble_hs_priv.h"

int
ble_hs_atomic_conn_delete(uint16_t conn_handle)
{
    struct ble_hs_conn *conn;

    ble_hs_lock();
    conn = ble_hs_conn_find(conn_handle);
    if (conn != NULL) {
        ble_hs_conn_remove(conn);
#if MYNEWT_VAL(BLE_PERIODIC_ADV_SYNC_TRANSFER)
        if (conn->psync) {
            ble_hs_periodic_sync_free(conn->psync);
        }
#endif
        ble_hs_conn_free(conn);

    }
    ble_hs_unlock();

    return conn != NULL ? 0 : BLE_HS_ENOTCONN;
}

void
ble_hs_atomic_conn_insert(struct ble_hs_conn *conn)
{
    ble_hs_lock();
    ble_hs_conn_insert(conn);
    ble_hs_unlock();
}

int
ble_hs_atomic_conn_flags(uint16_t conn_handle, ble_hs_conn_flags_t *out_flags)
{
    struct ble_hs_conn *conn;
    int rc;

    ble_hs_lock();

    conn = ble_hs_conn_find(conn_handle);
    if (conn == NULL) {
        rc = BLE_HS_ENOTCONN;
    } else {
        rc = 0;
        if (out_flags != NULL) {
            *out_flags = conn->bhc_flags;
        }
    }

    ble_hs_unlock();

    return rc;
}

int
ble_hs_atomic_conn_set_flags(uint16_t conn_handle, ble_hs_conn_flags_t flags,
                             int on)
{
    struct ble_hs_conn *conn;
    int rc;

    ble_hs_lock();

    conn = ble_hs_conn_find(conn_handle);
    if (conn == NULL) {
        rc = BLE_HS_ENOTCONN;
    } else {
        rc = 0;

        if (on) {
            conn->bhc_flags |= flags;
        } else {
            conn->bhc_flags &= ~flags;
        }
    }

    ble_hs_unlock();

    return rc;
}

uint16_t
ble_hs_atomic_first_conn_handle(void)
{
    const struct ble_hs_conn *conn;
    uint16_t conn_handle;

    ble_hs_lock();

    conn = ble_hs_conn_first();
    if (conn != NULL) {
        conn_handle = conn->bhc_handle;
    } else {
        conn_handle = BLE_HS_CONN_HANDLE_NONE;
    }

    ble_hs_unlock();

    return conn_handle;
}
