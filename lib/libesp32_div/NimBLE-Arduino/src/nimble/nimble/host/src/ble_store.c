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

#include "nimble/nimble/host/include/host/ble_store.h"
#include "ble_hs_priv.h"

int
ble_store_read(int obj_type, const union ble_store_key *key,
               union ble_store_value *val)
{
    int rc;

    ble_hs_lock();

    if (ble_hs_cfg.store_read_cb == NULL) {
        rc = BLE_HS_ENOTSUP;
    } else {
        rc = ble_hs_cfg.store_read_cb(obj_type, key, val);
    }

    ble_hs_unlock();

    return rc;
}

int
ble_store_write(int obj_type, const union ble_store_value *val)
{
    int rc;

    if (ble_hs_cfg.store_write_cb == NULL) {
        return BLE_HS_ENOTSUP;
    }

    while (1) {
        ble_hs_lock();
        rc = ble_hs_cfg.store_write_cb(obj_type, val);
        ble_hs_unlock();

        switch (rc) {
        case 0:
            return 0;
        case BLE_HS_ESTORE_CAP:
            /* Record didn't fit.  Give the application the opportunity to free
             * up some space.
             */
            rc = ble_store_overflow_event(obj_type, val);
            if (rc != 0) {
                return rc;
            }

            /* Application made room for the record; try again. */
            break;

        default:
            return rc;
        }
    }
}

int
ble_store_delete(int obj_type, const union ble_store_key *key)
{
    int rc;

    ble_hs_lock();

    if (ble_hs_cfg.store_delete_cb == NULL) {
        rc = BLE_HS_ENOTSUP;
    } else {
        rc = ble_hs_cfg.store_delete_cb(obj_type, key);
    }

    ble_hs_unlock();

    return rc;
}

static int
ble_store_status(struct ble_store_status_event *event)
{
    int rc;

    BLE_HS_DBG_ASSERT(!ble_hs_locked_by_cur_task());

    if (ble_hs_cfg.store_status_cb == NULL) {
        rc = BLE_HS_ENOTSUP;
    } else {
        rc = ble_hs_cfg.store_status_cb(event, ble_hs_cfg.store_status_arg);
    }

    return rc;
}

int
ble_store_overflow_event(int obj_type, const union ble_store_value *value)
{
    struct ble_store_status_event event;

    event.event_code = BLE_STORE_EVENT_OVERFLOW;
    event.overflow.obj_type = obj_type;
    event.overflow.value = value;

    return ble_store_status(&event);
}

int
ble_store_full_event(int obj_type, uint16_t conn_handle)
{
    struct ble_store_status_event event;

    event.event_code = BLE_STORE_EVENT_FULL;
    event.full.obj_type = obj_type;
    event.full.conn_handle = conn_handle;

    return ble_store_status(&event);
}

int
ble_store_read_our_sec(const struct ble_store_key_sec *key_sec,
                       struct ble_store_value_sec *value_sec)
{
    const union ble_store_key *store_key;
    union ble_store_value *store_value;
    int rc;

    BLE_HS_DBG_ASSERT(key_sec->peer_addr.type == BLE_ADDR_PUBLIC ||
                      key_sec->peer_addr.type == BLE_ADDR_RANDOM ||
                      ble_addr_cmp(&key_sec->peer_addr, BLE_ADDR_ANY) == 0);

    store_key = (void *)key_sec;
    store_value = (void *)value_sec;
    rc = ble_store_read(BLE_STORE_OBJ_TYPE_OUR_SEC, store_key, store_value);
    return rc;
}

static int
ble_store_persist_sec(int obj_type,
                      const struct ble_store_value_sec *value_sec)
{
    union ble_store_value *store_value;
    int rc;

    BLE_HS_DBG_ASSERT(value_sec->peer_addr.type == BLE_ADDR_PUBLIC ||
                      value_sec->peer_addr.type == BLE_ADDR_RANDOM);
    BLE_HS_DBG_ASSERT(value_sec->ltk_present ||
                      value_sec->irk_present ||
                      value_sec->csrk_present);

    store_value = (void *)value_sec;
    rc = ble_store_write(obj_type, store_value);
    return rc;
}

int
ble_store_write_our_sec(const struct ble_store_value_sec *value_sec)
{
    int rc;

    rc = ble_store_persist_sec(BLE_STORE_OBJ_TYPE_OUR_SEC, value_sec);
    return rc;
}

int
ble_store_delete_our_sec(const struct ble_store_key_sec *key_sec)
{
    union ble_store_key *store_key;
    int rc;

    store_key = (void *)key_sec;
    rc = ble_store_delete(BLE_STORE_OBJ_TYPE_OUR_SEC, store_key);
    return rc;
}

int
ble_store_delete_peer_sec(const struct ble_store_key_sec *key_sec)
{
    union ble_store_key *store_key;
    int rc;

    store_key = (void *)key_sec;
    rc = ble_store_delete(BLE_STORE_OBJ_TYPE_PEER_SEC, store_key);
    return rc;
}

int
ble_store_read_peer_sec(const struct ble_store_key_sec *key_sec,
                        struct ble_store_value_sec *value_sec)
{
    union ble_store_value *store_value;
    union ble_store_key *store_key;
    int rc;

    BLE_HS_DBG_ASSERT(key_sec->peer_addr.type == BLE_ADDR_PUBLIC ||
                      key_sec->peer_addr.type == BLE_ADDR_RANDOM);

    store_key = (void *)key_sec;
    store_value = (void *)value_sec;
    rc = ble_store_read(BLE_STORE_OBJ_TYPE_PEER_SEC, store_key, store_value);

    if (rc != 0) {
        return rc;
    }

    return 0;
}

int
ble_store_write_peer_sec(const struct ble_store_value_sec *value_sec)
{
    int rc;

    rc = ble_store_persist_sec(BLE_STORE_OBJ_TYPE_PEER_SEC, value_sec);
    if (rc != 0) {
        return rc;
    }

    if (ble_addr_cmp(&value_sec->peer_addr, BLE_ADDR_ANY) &&
        value_sec->irk_present) {

        /* Write the peer IRK to the controller keycache
         * There is not much to do here if it fails */
        rc = ble_hs_pvcy_add_entry(value_sec->peer_addr.val,
                                          value_sec->peer_addr.type,
                                          value_sec->irk);
        if (rc != 0) {
            return rc;
        }
    }

    return 0;
}

int
ble_store_read_cccd(const struct ble_store_key_cccd *key,
                    struct ble_store_value_cccd *out_value)
{
    union ble_store_value *store_value;
    union ble_store_key *store_key;
    int rc;

    store_key = (void *)key;
    store_value = (void *)out_value;
    rc = ble_store_read(BLE_STORE_OBJ_TYPE_CCCD, store_key, store_value);
    return rc;
}

int
ble_store_write_cccd(const struct ble_store_value_cccd *value)
{
    union ble_store_value *store_value;
    int rc;

    store_value = (void *)value;
    rc = ble_store_write(BLE_STORE_OBJ_TYPE_CCCD, store_value);
    return rc;
}

int
ble_store_delete_cccd(const struct ble_store_key_cccd *key)
{
    union ble_store_key *store_key;
    int rc;

    store_key = (void *)key;
    rc = ble_store_delete(BLE_STORE_OBJ_TYPE_CCCD, store_key);
    return rc;
}

void
ble_store_key_from_value_cccd(struct ble_store_key_cccd *out_key,
                              const struct ble_store_value_cccd *value)
{
    out_key->peer_addr = value->peer_addr;
    out_key->chr_val_handle = value->chr_val_handle;
    out_key->idx = 0;
}

void
ble_store_key_from_value_sec(struct ble_store_key_sec *out_key,
                             const struct ble_store_value_sec *value)
{
    out_key->peer_addr = value->peer_addr;

    out_key->ediv = value->ediv;
    out_key->rand_num = value->rand_num;
    out_key->ediv_rand_present = 1;
    out_key->idx = 0;
}

void
ble_store_key_from_value(int obj_type,
                         union ble_store_key *out_key,
                         const union ble_store_value *value)
{
    switch (obj_type) {
    case BLE_STORE_OBJ_TYPE_OUR_SEC:
    case BLE_STORE_OBJ_TYPE_PEER_SEC:
        ble_store_key_from_value_sec(&out_key->sec, &value->sec);
        break;

    case BLE_STORE_OBJ_TYPE_CCCD:
        ble_store_key_from_value_cccd(&out_key->cccd, &value->cccd);
        break;

    default:
        BLE_HS_DBG_ASSERT(0);
        break;
    }
}

int
ble_store_iterate(int obj_type,
                  ble_store_iterator_fn *callback,
                  void *cookie)
{
    union ble_store_key key;
    union ble_store_value value;
    int idx = 0;
    uint8_t *pidx;
    int rc;

    /* a magic value to retrieve anything */
    memset(&key, 0, sizeof(key));
    switch(obj_type) {
        case BLE_STORE_OBJ_TYPE_PEER_SEC:
        case BLE_STORE_OBJ_TYPE_OUR_SEC:
            key.sec.peer_addr = *BLE_ADDR_ANY;
            pidx = &key.sec.idx;
            break;
        case BLE_STORE_OBJ_TYPE_CCCD:
            key.cccd.peer_addr = *BLE_ADDR_ANY;
            pidx = &key.cccd.idx;
            break;
        default:
            BLE_HS_DBG_ASSERT(0);
            return BLE_HS_EINVAL;
    }

    while (1) {
        *pidx = idx;
        rc = ble_store_read(obj_type, &key, &value);
        switch (rc) {
        case 0:
            if (callback != NULL) {
                rc = callback(obj_type, &value, cookie);
                if (rc != 0) {
                    /* User function indicates to stop iterating. */
                    return 0;
                }
            }
            break;

        case BLE_HS_ENOENT:
            /* No more entries. */
            return 0;

        default:
            /* Read error. */
            return rc;
        }

        idx++;
    }
}

/**
 * Deletes all objects from the BLE host store.
 *
 * @return                      0 on success; nonzero on failure.
 */
int
ble_store_clear(void)
{
    const uint8_t obj_types[] = {
        BLE_STORE_OBJ_TYPE_OUR_SEC,
        BLE_STORE_OBJ_TYPE_PEER_SEC,
        BLE_STORE_OBJ_TYPE_CCCD,
    };
    union ble_store_key key;
    int obj_type;
    int rc;
    int i;

    /* A zeroed key will always retrieve the first value. */
    memset(&key, 0, sizeof key);

    for (i = 0; i < sizeof obj_types / sizeof obj_types[0]; i++) {
        obj_type = obj_types[i];

        do {
            rc = ble_store_delete(obj_type, &key);
        } while (rc == 0);

        /* BLE_HS_ENOENT means we deleted everything. */
        if (rc != BLE_HS_ENOENT) {
            return rc;
        }
    }

    return 0;
}
