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
#include <stdlib.h>
#include "nimble/porting/nimble/include/os/os.h"
#include "ble_hs_priv.h"

const uint8_t ble_hs_misc_null_addr[6];

int
ble_hs_misc_conn_chan_find(uint16_t conn_handle, uint16_t cid,
                           struct ble_hs_conn **out_conn,
                           struct ble_l2cap_chan **out_chan)
{
    struct ble_l2cap_chan *chan;
    struct ble_hs_conn *conn;
    int rc;

    conn = ble_hs_conn_find(conn_handle);
    if (conn == NULL) {
        chan = NULL;
        rc = BLE_HS_ENOTCONN;
    } else {
        chan = ble_hs_conn_chan_find_by_scid(conn, cid);
        if (chan == NULL) {
            rc = BLE_HS_ENOTCONN;
        } else {
            rc = 0;
        }
    }

    if (out_conn != NULL) {
        *out_conn = conn;
    }
    if (out_chan != NULL) {
        *out_chan = chan;
    }

    return rc;
}

int
ble_hs_misc_conn_chan_find_reqd(uint16_t conn_handle, uint16_t cid,
                                struct ble_hs_conn **out_conn,
                                struct ble_l2cap_chan **out_chan)
{
    struct ble_l2cap_chan *chan;
    struct ble_hs_conn *conn;
    int rc;

    rc = ble_hs_misc_conn_chan_find(conn_handle, cid, &conn, &chan);
    if (rc != 0) {
        return rc;
    }

    if (out_conn != NULL) {
        *out_conn = conn;
    }
    if (out_chan != NULL) {
        *out_chan = chan;
    }

    return 0;
}

uint8_t
ble_hs_misc_own_addr_type_to_id(uint8_t own_addr_type)
{
    switch (own_addr_type) {
    case BLE_OWN_ADDR_PUBLIC:
    case BLE_OWN_ADDR_RPA_PUBLIC_DEFAULT:
         return BLE_ADDR_PUBLIC;

    case BLE_OWN_ADDR_RANDOM:
    case BLE_OWN_ADDR_RPA_RANDOM_DEFAULT:
         return BLE_ADDR_RANDOM;

    default:
        BLE_HS_DBG_ASSERT(0);
        return BLE_ADDR_PUBLIC;
    }
}

uint8_t
ble_hs_misc_peer_addr_type_to_id(uint8_t peer_addr_type)
{
    switch (peer_addr_type) {
        case BLE_ADDR_PUBLIC:
        case BLE_ADDR_PUBLIC_ID:
            return BLE_ADDR_PUBLIC;

        case BLE_ADDR_RANDOM:
        case BLE_ADDR_RANDOM_ID:
            return BLE_ADDR_RANDOM;

        default:
            BLE_HS_DBG_ASSERT(0);
            return BLE_ADDR_PUBLIC;
    }
}

static int
ble_hs_misc_restore_one_irk(int obj_type, union ble_store_value *val,
                            void *cookie)
{
    const struct ble_store_value_sec *sec;
    int rc;

    BLE_HS_DBG_ASSERT(obj_type == BLE_STORE_OBJ_TYPE_PEER_SEC);

    sec = &val->sec;
    if (sec->irk_present) {
        rc = ble_hs_pvcy_add_entry(sec->peer_addr.val, sec->peer_addr.type,
                                   sec->irk);
        if (rc != 0) {
            BLE_HS_LOG(ERROR, "failed to configure restored IRK\n");
        }
    }

    return 0;
}

int
ble_hs_misc_restore_irks(void)
{
    int rc;

    rc = ble_store_iterate(BLE_STORE_OBJ_TYPE_PEER_SEC,
                           ble_hs_misc_restore_one_irk,
                           NULL);
    return rc;
}
