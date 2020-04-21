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

/**
 * This file implements a simple in-RAM key database for BLE host security
 * material and CCCDs.  As this database is only ble_store_ramd in RAM, its
 * contents are lost when the application terminates.
 */

#include <inttypes.h>
#include <string.h>

#include "sysinit/sysinit.h"
#include "syscfg/syscfg.h"
#include "host/ble_hs.h"
#include "store/ram/ble_store_ram.h"

static struct ble_store_value_sec
    ble_store_ram_our_secs[MYNEWT_VAL(BLE_STORE_MAX_BONDS)];
static int ble_store_ram_num_our_secs;

static struct ble_store_value_sec
    ble_store_ram_peer_secs[MYNEWT_VAL(BLE_STORE_MAX_BONDS)];
static int ble_store_ram_num_peer_secs;

static struct ble_store_value_cccd
    ble_store_ram_cccds[MYNEWT_VAL(BLE_STORE_MAX_CCCDS)];
static int ble_store_ram_num_cccds;

/*****************************************************************************
 * $sec                                                                      *
 *****************************************************************************/

static void
ble_store_ram_print_value_sec(const struct ble_store_value_sec *sec)
{
    if (sec->ltk_present) {
        BLE_HS_LOG(DEBUG, "ediv=%u rand=%llu authenticated=%d ltk=",
                       sec->ediv, sec->rand_num, sec->authenticated);
        ble_hs_log_flat_buf(sec->ltk, 16);
        BLE_HS_LOG(DEBUG, " ");
    }
    if (sec->irk_present) {
        BLE_HS_LOG(DEBUG, "irk=");
        ble_hs_log_flat_buf(sec->irk, 16);
        BLE_HS_LOG(DEBUG, " ");
    }
    if (sec->csrk_present) {
        BLE_HS_LOG(DEBUG, "csrk=");
        ble_hs_log_flat_buf(sec->csrk, 16);
        BLE_HS_LOG(DEBUG, " ");
    }

    BLE_HS_LOG(DEBUG, "\n");
}

static void
ble_store_ram_print_key_sec(const struct ble_store_key_sec *key_sec)
{
    if (ble_addr_cmp(&key_sec->peer_addr, BLE_ADDR_ANY)) {
        BLE_HS_LOG(DEBUG, "peer_addr_type=%d peer_addr=",
                       key_sec->peer_addr.type);
        ble_hs_log_flat_buf(key_sec->peer_addr.val, 6);
        BLE_HS_LOG(DEBUG, " ");
    }
    if (key_sec->ediv_rand_present) {
        BLE_HS_LOG(DEBUG, "ediv=0x%02x rand=0x%llx ",
                       key_sec->ediv, key_sec->rand_num);
    }
}

static int
ble_store_ram_find_sec(const struct ble_store_key_sec *key_sec,
                       const struct ble_store_value_sec *value_secs,
                       int num_value_secs)
{
    const struct ble_store_value_sec *cur;
    int skipped;
    int i;

    skipped = 0;

    for (i = 0; i < num_value_secs; i++) {
        cur = value_secs + i;

        if (ble_addr_cmp(&key_sec->peer_addr, BLE_ADDR_ANY)) {
            if (ble_addr_cmp(&cur->peer_addr, &key_sec->peer_addr)) {
                continue;
            }
        }

        if (key_sec->ediv_rand_present) {
            if (cur->ediv != key_sec->ediv) {
                continue;
            }

            if (cur->rand_num != key_sec->rand_num) {
                continue;
            }
        }

        if (key_sec->idx > skipped) {
            skipped++;
            continue;
        }

        return i;
    }

    return -1;
}

static int
ble_store_ram_read_our_sec(const struct ble_store_key_sec *key_sec,
                           struct ble_store_value_sec *value_sec)
{
    int idx;

    idx = ble_store_ram_find_sec(key_sec, ble_store_ram_our_secs,
                                 ble_store_ram_num_our_secs);
    if (idx == -1) {
        return BLE_HS_ENOENT;
    }

    *value_sec = ble_store_ram_our_secs[idx];
    return 0;
}

static int
ble_store_ram_write_our_sec(const struct ble_store_value_sec *value_sec)
{
    struct ble_store_key_sec key_sec;
    int idx;

    BLE_HS_LOG(DEBUG, "persisting our sec; ");
    ble_store_ram_print_value_sec(value_sec);

    ble_store_key_from_value_sec(&key_sec, value_sec);
    idx = ble_store_ram_find_sec(&key_sec, ble_store_ram_our_secs,
                                 ble_store_ram_num_our_secs);
    if (idx == -1) {
        if (ble_store_ram_num_our_secs >= MYNEWT_VAL(BLE_STORE_MAX_BONDS)) {
            BLE_HS_LOG(DEBUG, "error persisting our sec; too many entries "
                              "(%d)\n", ble_store_ram_num_our_secs);
            return BLE_HS_ESTORE_CAP;
        }

        idx = ble_store_ram_num_our_secs;
        ble_store_ram_num_our_secs++;
    }

    ble_store_ram_our_secs[idx] = *value_sec;
    return 0;
}

static int
ble_store_ram_delete_obj(void *values, int value_size, int idx,
                         int *num_values)
{
    uint8_t *dst;
    uint8_t *src;
    int move_count;

    (*num_values)--;
    if (idx < *num_values) {
        dst = values;
        dst += idx * value_size;
        src = dst + value_size;

        move_count = *num_values - idx;
        memmove(dst, src, move_count);
    }

    return 0;
}

static int
ble_store_ram_delete_sec(const struct ble_store_key_sec *key_sec,
                         struct ble_store_value_sec *value_secs,
                         int *num_value_secs)
{
    int idx;
    int rc;

    idx = ble_store_ram_find_sec(key_sec, value_secs, *num_value_secs);
    if (idx == -1) {
        return BLE_HS_ENOENT;
    }

    rc = ble_store_ram_delete_obj(value_secs, sizeof *value_secs, idx,
                                  num_value_secs);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

static int
ble_store_ram_delete_our_sec(const struct ble_store_key_sec *key_sec)
{
    int rc;

    rc = ble_store_ram_delete_sec(key_sec, ble_store_ram_our_secs,
                                  &ble_store_ram_num_our_secs);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

static int
ble_store_ram_delete_peer_sec(const struct ble_store_key_sec *key_sec)
{
    int rc;

    rc = ble_store_ram_delete_sec(key_sec, ble_store_ram_peer_secs,
                                  &ble_store_ram_num_peer_secs);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

static int
ble_store_ram_read_peer_sec(const struct ble_store_key_sec *key_sec,
                            struct ble_store_value_sec *value_sec)
{
    int idx;

    idx = ble_store_ram_find_sec(key_sec, ble_store_ram_peer_secs,
                             ble_store_ram_num_peer_secs);
    if (idx == -1) {
        return BLE_HS_ENOENT;
    }

    *value_sec = ble_store_ram_peer_secs[idx];
    return 0;
}

static int
ble_store_ram_write_peer_sec(const struct ble_store_value_sec *value_sec)
{
    struct ble_store_key_sec key_sec;
    int idx;

    BLE_HS_LOG(DEBUG, "persisting peer sec; ");
    ble_store_ram_print_value_sec(value_sec);

    ble_store_key_from_value_sec(&key_sec, value_sec);
    idx = ble_store_ram_find_sec(&key_sec, ble_store_ram_peer_secs,
                                 ble_store_ram_num_peer_secs);
    if (idx == -1) {
        if (ble_store_ram_num_peer_secs >= MYNEWT_VAL(BLE_STORE_MAX_BONDS)) {
            BLE_HS_LOG(DEBUG, "error persisting peer sec; too many entries "
                             "(%d)\n", ble_store_ram_num_peer_secs);
            return BLE_HS_ESTORE_CAP;
        }

        idx = ble_store_ram_num_peer_secs;
        ble_store_ram_num_peer_secs++;
    }

    ble_store_ram_peer_secs[idx] = *value_sec;
    return 0;
}

/*****************************************************************************
 * $cccd                                                                     *
 *****************************************************************************/

static int
ble_store_ram_find_cccd(const struct ble_store_key_cccd *key)
{
    struct ble_store_value_cccd *cccd;
    int skipped;
    int i;

    skipped = 0;
    for (i = 0; i < ble_store_ram_num_cccds; i++) {
        cccd = ble_store_ram_cccds + i;

        if (ble_addr_cmp(&key->peer_addr, BLE_ADDR_ANY)) {
            if (ble_addr_cmp(&cccd->peer_addr, &key->peer_addr)) {
                continue;
            }
        }

        if (key->chr_val_handle != 0) {
            if (cccd->chr_val_handle != key->chr_val_handle) {
                continue;
            }
        }

        if (key->idx > skipped) {
            skipped++;
            continue;
        }

        return i;
    }

    return -1;
}

static int
ble_store_ram_delete_cccd(const struct ble_store_key_cccd *key_cccd)
{
    int idx;
    int rc;

    idx = ble_store_ram_find_cccd(key_cccd);
    if (idx == -1) {
        return BLE_HS_ENOENT;
    }

    rc = ble_store_ram_delete_obj(ble_store_ram_cccds,
                                  sizeof *ble_store_ram_cccds,
                                  idx,
                                  &ble_store_ram_num_cccds);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

static int
ble_store_ram_read_cccd(const struct ble_store_key_cccd *key_cccd,
                        struct ble_store_value_cccd *value_cccd)
{
    int idx;

    idx = ble_store_ram_find_cccd(key_cccd);
    if (idx == -1) {
        return BLE_HS_ENOENT;
    }

    *value_cccd = ble_store_ram_cccds[idx];
    return 0;
}

static int
ble_store_ram_write_cccd(const struct ble_store_value_cccd *value_cccd)
{
    struct ble_store_key_cccd key_cccd;
    int idx;

    ble_store_key_from_value_cccd(&key_cccd, value_cccd);
    idx = ble_store_ram_find_cccd(&key_cccd);
    if (idx == -1) {
        if (ble_store_ram_num_cccds >= MYNEWT_VAL(BLE_STORE_MAX_CCCDS)) {
            BLE_HS_LOG(DEBUG, "error persisting cccd; too many entries (%d)\n",
                       ble_store_ram_num_cccds);
            return BLE_HS_ESTORE_CAP;
        }

        idx = ble_store_ram_num_cccds;
        ble_store_ram_num_cccds++;
    }

    ble_store_ram_cccds[idx] = *value_cccd;
    return 0;
}

/*****************************************************************************
 * $api                                                                      *
 *****************************************************************************/

/**
 * Searches the database for an object matching the specified criteria.
 *
 * @return                      0 if a key was found; else BLE_HS_ENOENT.
 */
int
ble_store_ram_read(int obj_type, const union ble_store_key *key,
                   union ble_store_value *value)
{
    int rc;

    switch (obj_type) {
    case BLE_STORE_OBJ_TYPE_PEER_SEC:
        /* An encryption procedure (bonding) is being attempted.  The nimble
         * stack is asking us to look in our key database for a long-term key
         * corresponding to the specified ediv and random number.
         *
         * Perform a key lookup and populate the context object with the
         * result.  The nimble stack will use this key if this function returns
         * success.
         */
        BLE_HS_LOG(DEBUG, "looking up peer sec; ");
        ble_store_ram_print_key_sec(&key->sec);
        BLE_HS_LOG(DEBUG, "\n");
        rc = ble_store_ram_read_peer_sec(&key->sec, &value->sec);
        return rc;

    case BLE_STORE_OBJ_TYPE_OUR_SEC:
        BLE_HS_LOG(DEBUG, "looking up our sec; ");
        ble_store_ram_print_key_sec(&key->sec);
        BLE_HS_LOG(DEBUG, "\n");
        rc = ble_store_ram_read_our_sec(&key->sec, &value->sec);
        return rc;

    case BLE_STORE_OBJ_TYPE_CCCD:
        rc = ble_store_ram_read_cccd(&key->cccd, &value->cccd);
        return rc;

    default:
        return BLE_HS_ENOTSUP;
    }
}

/**
 * Adds the specified object to the database.
 *
 * @return                      0 on success; BLE_HS_ESTORE_CAP if the database
 *                                  is full.
 */
int
ble_store_ram_write(int obj_type, const union ble_store_value *val)
{
    int rc;

    switch (obj_type) {
    case BLE_STORE_OBJ_TYPE_PEER_SEC:
        rc = ble_store_ram_write_peer_sec(&val->sec);
        return rc;

    case BLE_STORE_OBJ_TYPE_OUR_SEC:
        rc = ble_store_ram_write_our_sec(&val->sec);
        return rc;

    case BLE_STORE_OBJ_TYPE_CCCD:
        rc = ble_store_ram_write_cccd(&val->cccd);
        return rc;

    default:
        return BLE_HS_ENOTSUP;
    }
}

int
ble_store_ram_delete(int obj_type, const union ble_store_key *key)
{
    int rc;

    switch (obj_type) {
    case BLE_STORE_OBJ_TYPE_PEER_SEC:
        rc = ble_store_ram_delete_peer_sec(&key->sec);
        return rc;

    case BLE_STORE_OBJ_TYPE_OUR_SEC:
        rc = ble_store_ram_delete_our_sec(&key->sec);
        return rc;

    case BLE_STORE_OBJ_TYPE_CCCD:
        rc = ble_store_ram_delete_cccd(&key->cccd);
        return rc;

    default:
        return BLE_HS_ENOTSUP;
    }
}

void
ble_store_ram_init(void)
{
    /* Ensure this function only gets called by sysinit. */
    SYSINIT_ASSERT_ACTIVE();

    ble_hs_cfg.store_read_cb = ble_store_ram_read;
    ble_hs_cfg.store_write_cb = ble_store_ram_write;
    ble_hs_cfg.store_delete_cb = ble_store_ram_delete;

    /* Re-initialize BSS values in case of unit tests. */
    ble_store_ram_num_our_secs = 0;
    ble_store_ram_num_peer_secs = 0;
    ble_store_ram_num_cccds = 0;
}
