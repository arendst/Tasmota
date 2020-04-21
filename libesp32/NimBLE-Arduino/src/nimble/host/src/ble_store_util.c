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

#include "host/ble_store.h"
#include "ble_hs_priv.h"

struct ble_store_util_peer_set {
    ble_addr_t *peer_id_addrs;
    int num_peers;
    int max_peers;
    int status;
};

struct ble_store_util_peer_cccd_set {
    struct ble_store_util_peer_set peer_set;
    ble_addr_t *curr_peer_addr;
};


static int
ble_store_util_iter_unique_peer(int obj_type,
                                union ble_store_value *val,
                                void *arg)
{
    struct ble_store_util_peer_set *set;
    int i;

    BLE_HS_DBG_ASSERT(obj_type == BLE_STORE_OBJ_TYPE_OUR_SEC ||
                      obj_type == BLE_STORE_OBJ_TYPE_PEER_SEC);

    set = arg;

    /* Do nothing if this peer is a duplicate. */
    for (i = 0; i < set->num_peers; i++) {
        if (ble_addr_cmp(set->peer_id_addrs + i, &val->sec.peer_addr) == 0) {
            return 0;
        }
    }

    if (set->num_peers >= set->max_peers) {
        /* Overflow; abort the iterate procedure. */
        set->status = BLE_HS_ENOMEM;
        return 1;
    }

    set->peer_id_addrs[set->num_peers] = val->sec.peer_addr;
    set->num_peers++;

    return 0;
}

static int
ble_store_util_iter_peer_cccd(int obj_type,
                              union ble_store_value *val,
                              void *arg)
{
    struct ble_store_util_peer_cccd_set *set;
    int i;

    set = arg;

    /* Do nothing if this peer is a duplicate or current peer */
    for (i = 0; i < set->peer_set.num_peers; i++) {
        if (ble_addr_cmp(set->peer_set.peer_id_addrs + i, &val->cccd.peer_addr) == 0) {
            return 0;
        }

        if (set->curr_peer_addr != NULL) {
            if (ble_addr_cmp(set->curr_peer_addr, &val->cccd.peer_addr) == 0) {
                return 0;
            }
        }
    }

    if (set->peer_set.num_peers >= set->peer_set.max_peers) {
        /* Overflow; abort the iterate procedure. */
        set->peer_set.status = BLE_HS_ENOMEM;
        return 1;
    }

    set->peer_set.peer_id_addrs[set->peer_set.num_peers] = val->cccd.peer_addr;
    set->peer_set.num_peers++;

    return 0;
}


/**
 * Retrieves the set of peer addresses for which a bond has been established.
 *
 * @param out_peer_id_addrs     On success, the set of bonded peer addresses
 *                                  gets written here.
 * @param out_num_peers         On success, the number of bonds gets written
 *                                  here.
 * @param max_peers             The capacity of the destination buffer.
 *
 * @return                      0 on success;
 *                              BLE_HS_ENOMEM if the destination buffer is too
 *                                  small;
 *                              Other nonzero on error.
 */
int
ble_store_util_bonded_peers(ble_addr_t *out_peer_id_addrs, int *out_num_peers,
                            int max_peers)
{
    struct ble_store_util_peer_set set = {
        .peer_id_addrs = out_peer_id_addrs,
        .num_peers = 0,
        .max_peers = max_peers,
        .status = 0,
    };
    int rc;

    rc = ble_store_iterate(BLE_STORE_OBJ_TYPE_OUR_SEC,
                           ble_store_util_iter_unique_peer,
                           &set);
    if (rc != 0) {
        return rc;
    }
    if (set.status != 0) {
        return set.status;
    }

    *out_num_peers = set.num_peers;
    return 0;
}

/**
 * Retrieves the set of peer addresses for which CCCDs are subscribed.
 *
 * @param out_peer_id_addrs     On success, the set of peer addresses
 *                                  gets written here.
 * @param out_num_peers         On success, the number of peer addresses gets written
 *                                  here.
 * @param max_peers             The capacity of the destination buffer.
 *
 * @param curr_peer_addrs       Current peer's address, ignore if NULL
 *
 * @return                      0 on success;
 *                              BLE_HS_ENOMEM if the destination buffer is too
 *                                  small;
 *                              Other nonzero on error.
 */
static int
ble_store_util_subscribed_cccds(ble_addr_t *out_peer_id_addrs, int *out_num_peers,
                                int max_peers, ble_addr_t *curr_peer_addr)
{
    struct ble_store_util_peer_cccd_set set = {
        .peer_set = {
            .peer_id_addrs = out_peer_id_addrs,
            .num_peers = 0,
            .max_peers = max_peers,
            .status = 0,
        },
        .curr_peer_addr = curr_peer_addr,
    };
    int rc;

    rc = ble_store_iterate(BLE_STORE_OBJ_TYPE_CCCD,
                           ble_store_util_iter_peer_cccd,
                           &set);
    if (rc != 0) {
        return rc;
    }
    if (set.peer_set.status != 0) {
        return set.peer_set.status;
    }

    *out_num_peers = set.peer_set.num_peers;
    return 0;
}

/**
 * Deletes all entries from the store that are attached to the specified peer
 * address.  This function deletes security entries and CCCD records.
 *
 * @param peer_id_addr          Entries with this peer address get deleted.
 *
 * @return                      0 on success;
 *                              Other nonzero on error.
 */
int
ble_store_util_delete_peer(const ble_addr_t *peer_id_addr)
{
    union ble_store_key key;
    int rc;

    memset(&key, 0, sizeof key);
    key.sec.peer_addr = *peer_id_addr;

    rc = ble_store_util_delete_all(BLE_STORE_OBJ_TYPE_OUR_SEC, &key);
    if (rc != 0) {
        return rc;
    }

    rc = ble_store_util_delete_all(BLE_STORE_OBJ_TYPE_PEER_SEC, &key);
    if (rc != 0) {
        return rc;
    }

    memset(&key, 0, sizeof key);
    key.cccd.peer_addr = *peer_id_addr;

    rc = ble_store_util_delete_all(BLE_STORE_OBJ_TYPE_CCCD, &key);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

/**
 * Deletes all entries from the store that match the specified key.
 *
 * @param type                  The type of store entry to delete.
 * @param key                   Entries matching this key get deleted.
 *
 * @return                      0 on success;
 *                              Other nonzero on error.
 */
int
ble_store_util_delete_all(int type, const union ble_store_key *key)
{
    int rc;

    do {
        rc = ble_store_delete(type, key);
    } while (rc == 0);

    if (rc != BLE_HS_ENOENT) {
        return rc;
    }

    return 0;
}

static int
ble_store_util_iter_count(int obj_type,
                          union ble_store_value *val,
                          void *arg)
{
    int *count;

    count = arg;
    (*count)++;

    return 0;
}

int
ble_store_util_count(int type, int *out_count)
{
    int rc;

    *out_count = 0;
    rc = ble_store_iterate(type,
                           ble_store_util_iter_count,
                           out_count);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

int
ble_store_util_delete_oldest_peer(void)
{
    ble_addr_t peer_id_addrs[MYNEWT_VAL(BLE_STORE_MAX_BONDS)];
    int num_peers;
    int rc;

    rc = ble_store_util_bonded_peers(
            peer_id_addrs, &num_peers,
            sizeof peer_id_addrs / sizeof peer_id_addrs[0]);
    if (rc != 0) {
        return rc;
    }

    if (num_peers == 0) {
        return 0;
    }

    rc = ble_store_util_delete_peer(&peer_id_addrs[0]);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

/**
 * Delete CCCDs of unbonded devices.
 *
 * @param curr_peer             Current peer's address (not to delete), ignore
 *                              ignore if NULL
 *
 * @return                      0 on success;
 *                              nonzero on error.
 */
int
ble_store_clean_old_cccds(const ble_addr_t *curr_peer)
{
    ble_addr_t peer_cccd_addrs[MYNEWT_VAL(BLE_STORE_MAX_CCCDS)];
    ble_addr_t peer_bonded_addrs[MYNEWT_VAL(BLE_STORE_MAX_BONDS)];
    int num_bonded_peers, num_cccd_peers;
    int i, j, rc;

    rc = ble_store_util_subscribed_cccds(&peer_cccd_addrs[0], &num_cccd_peers,
                                         MYNEWT_VAL(BLE_STORE_MAX_CCCDS),
                                         (void *) curr_peer);
    if (rc != 0) {
        return rc;
    }

    rc = ble_store_util_bonded_peers(&peer_bonded_addrs[0], &num_bonded_peers,
                                     MYNEWT_VAL(BLE_STORE_MAX_BONDS));
    if (rc != 0) {
        return rc;
    }

    union ble_store_key key = {0};
    /* Init rc to BLE_HS_ENOENT to indicate no CCCD is deleted */
    rc = BLE_HS_ENOENT;

    for (i = 0; i < num_cccd_peers; i++) {
        key.cccd.peer_addr = peer_cccd_addrs[i];

        for (j = 0; j < num_bonded_peers; j++) {
            if (memcmp(&peer_cccd_addrs[i], &peer_bonded_addrs[j],
                       sizeof(ble_addr_t)) == 0) {
                break;
            }
        }

        if (j < num_bonded_peers) {
            continue;
        }

        rc = ble_store_util_delete_all(BLE_STORE_OBJ_TYPE_CCCD, &key);
        if (rc != 0) {
            return rc;
        }
    }

    return rc;
}

/**
 * Round-robin status callback.  If a there is insufficient storage capacity
 * for a new record, delete the oldest bond and proceed with the persist
 * operation.
 *
 * Note: This is not the best behavior for an actual product because
 * uninteresting peers could cause important bonds to be deleted.  This is
 * useful for demonstrations and sample apps.
 */
int
ble_store_util_status_rr(struct ble_store_status_event *event, void *arg)
{
	int rc = BLE_HS_EUNKNOWN;
    switch (event->event_code) {
    case BLE_STORE_EVENT_OVERFLOW:
        switch (event->overflow.obj_type) {
			case BLE_STORE_OBJ_TYPE_OUR_SEC:
			case BLE_STORE_OBJ_TYPE_PEER_SEC:
				return ble_gap_unpair_oldest_peer();
			case BLE_STORE_OBJ_TYPE_CCCD:
				/* Try to remove unbonded CCCDs first */
				if ((rc = ble_store_clean_old_cccds((void *) &event->overflow.value->cccd.peer_addr)) == BLE_HS_ENOENT) {
					/* No unbonded CCCDs found to delete, try unpairing oldest peer
					* except current peer */
					return ble_gap_unpair_oldest_except_curr((void *) &event->overflow.value->cccd.peer_addr);
				}
				return rc;

			default:
				return BLE_HS_EUNKNOWN;
        
        /*    case BLE_STORE_OBJ_TYPE_OUR_SEC:
            case BLE_STORE_OBJ_TYPE_PEER_SEC:
            case BLE_STORE_OBJ_TYPE_CCCD:
                return ble_gap_unpair_oldest_peer();

            default:
                return BLE_HS_EUNKNOWN;
		*/
        }

    case BLE_STORE_EVENT_FULL:
        /* Just proceed with the operation.  If it results in an overflow,
         * we'll delete a record when the overflow occurs.
         */
        return 0;

    default:
        return BLE_HS_EUNKNOWN;
    }
}
