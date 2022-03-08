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

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "nimble/porting/nimble/include/os/os.h"
#include "nimble/nimble/include/nimble/ble.h"
#include "nimble/nimble/host/include/host/ble_uuid.h"
#include "ble_hs_priv.h"

#ifdef ESP_PLATFORM
#include "nimble/esp_port/port/include/esp_nimble_mem.h"
#endif

/**
 * ATT server - Attribute Protocol
 *
 * Notes on buffer reuse:
 * Most request handlers reuse the request buffer for the reponse.  This is
 * done to prevent out-of-memory conditions.  However, there are two handlers
 * which do not reuse the request buffer:
 *     1. Write request.
 *     2. Indicate request.
 *
 * Both of these handlers attempt to allocate a new buffer for the response
 * prior to processing the request.  If allocation fails, the request is not
 * processed, and the request buffer is reused for the transmission of an
 * "insufficient resources" ATT error response.  These handlers don't reuse the
 * request mbuf for an affirmative response because the buffer contains the
 * attribute data that gets passed to the application callback.  The
 * application may choose to retain the mbuf during the callback, so the stack
 */

STAILQ_HEAD(ble_att_svr_entry_list, ble_att_svr_entry);
static struct ble_att_svr_entry_list ble_att_svr_list;
static struct ble_att_svr_entry_list ble_att_svr_hidden_list;

static uint16_t ble_att_svr_id;

static void *ble_att_svr_entry_mem;
static struct os_mempool ble_att_svr_entry_pool;

static os_membuf_t ble_att_svr_prep_entry_mem[
    OS_MEMPOOL_SIZE(MYNEWT_VAL(BLE_ATT_SVR_MAX_PREP_ENTRIES),
                    sizeof (struct ble_att_prep_entry))
];

static struct os_mempool ble_att_svr_prep_entry_pool;

static struct ble_att_svr_entry *
ble_att_svr_entry_alloc(void)
{
    struct ble_att_svr_entry *entry;

    entry = os_memblock_get(&ble_att_svr_entry_pool);
    if (entry != NULL) {
        memset(entry, 0, sizeof *entry);
    }

    return entry;
}

static void
ble_att_svr_entry_free(struct ble_att_svr_entry *entry)
{
    os_memblock_put(&ble_att_svr_entry_pool, entry);
}

/**
 * Allocate the next handle id and return it.
 *
 * @return A new 16-bit handle ID.
 */
static uint16_t
ble_att_svr_next_id(void)
{
    /* Rollover is fatal. */
    BLE_HS_DBG_ASSERT(ble_att_svr_id != UINT16_MAX);
    return ++ble_att_svr_id;
}

/**
 * Register a host attribute with the BLE stack.
 *
 * @param ha                    A filled out ble_att structure to register
 * @param handle_id             A pointer to a 16-bit handle ID, which will be
 *                                  the handle that is allocated.
 * @param fn                    The callback function that gets executed when
 *                                  the attribute is operated on.
 *
 * @return 0 on success, non-zero error code on failure.
 */
int
ble_att_svr_register(const ble_uuid_t *uuid, uint8_t flags,
                     uint8_t min_key_size, uint16_t *handle_id,
                     ble_att_svr_access_fn *cb, void *cb_arg)
{
    struct ble_att_svr_entry *entry;

    entry = ble_att_svr_entry_alloc();
    if (entry == NULL) {
        return BLE_HS_ENOMEM;
    }

    entry->ha_uuid = uuid;
    entry->ha_flags = flags;
    entry->ha_min_key_size = min_key_size;
    entry->ha_handle_id = ble_att_svr_next_id();
    entry->ha_cb = cb;
    entry->ha_cb_arg = cb_arg;

    STAILQ_INSERT_TAIL(&ble_att_svr_list, entry, ha_next);

    if (handle_id != NULL) {
        *handle_id = entry->ha_handle_id;
    }

    return 0;
}

uint16_t
ble_att_svr_prev_handle(void)
{
    return ble_att_svr_id;
}

/**
 * Find a host attribute by handle id.
 *
 * @param handle_id             The handle_id to search for
 * @param ha_ptr                On input: Indicates the starting point of the
 *                                  walk; null means start at the beginning of
 *                                  the list, non-null means start at the
 *                                  following entry.
 *                              On output: Indicates the last ble_att element
 *                                  processed, or NULL if the entire list has
 *                                  been processed.
 *
 * @return                      0 on success; BLE_HS_ENOENT on not found.
 */
struct ble_att_svr_entry *
ble_att_svr_find_by_handle(uint16_t handle_id)
{
    struct ble_att_svr_entry *entry;

    for (entry = STAILQ_FIRST(&ble_att_svr_list);
         entry != NULL;
         entry = STAILQ_NEXT(entry, ha_next)) {

        if (entry->ha_handle_id == handle_id) {
            return entry;
        }
    }

    return NULL;
}

/**
 * Find a host attribute by UUID.
 *
 * @param uuid                  The ble_uuid_t to search for; null means
 *                                  find any type of attribute.
 * @param prev                  On input: Indicates the starting point of the
 *                                  walk; null means start at the beginning of
 *                                  the list, non-null means start at the
 *                                  following entry.
 *                              On output: Indicates the last ble_att element
 *                                  processed, or NULL if the entire list has
 *                                  been processed.
 *
 * @return                      0 on success; BLE_HS_ENOENT on not found.
 */
struct ble_att_svr_entry *
ble_att_svr_find_by_uuid(struct ble_att_svr_entry *prev, const ble_uuid_t *uuid,
                         uint16_t end_handle)
{
    struct ble_att_svr_entry *entry;

    if (prev == NULL) {
        entry = STAILQ_FIRST(&ble_att_svr_list);
    } else {
        entry = STAILQ_NEXT(prev, ha_next);
    }

    for (;
         entry != NULL && entry->ha_handle_id <= end_handle;
         entry = STAILQ_NEXT(entry, ha_next)) {

        if (uuid == NULL || ble_uuid_cmp(entry->ha_uuid, uuid) == 0) {
            return entry;
        }
    }

    return NULL;
}

static int
ble_att_svr_pullup_req_base(struct os_mbuf **om, int base_len,
                            uint8_t *out_att_err)
{
    uint8_t att_err;
    int rc;

    rc = ble_hs_mbuf_pullup_base(om, base_len);
    if (rc == BLE_HS_ENOMEM) {
        att_err = BLE_ATT_ERR_INSUFFICIENT_RES;
    } else {
        att_err = 0;
    }

    if (out_att_err != NULL) {
        *out_att_err = att_err;
    }

    return rc;
}

static void
ble_att_svr_get_sec_state(uint16_t conn_handle,
                          struct ble_gap_sec_state *out_sec_state)
{
    struct ble_hs_conn *conn;

    ble_hs_lock();

    conn = ble_hs_conn_find_assert(conn_handle);
    *out_sec_state = conn->bhc_sec_state;

    ble_hs_unlock();
}

static int
ble_att_svr_check_perms(uint16_t conn_handle, int is_read,
                        struct ble_att_svr_entry *entry,
                        uint8_t *out_att_err)
{
    struct ble_gap_sec_state sec_state;
    struct ble_store_value_sec value_sec;
    struct ble_store_key_sec key_sec;
    struct ble_hs_conn_addrs addrs;
    struct ble_hs_conn *conn;
    int author;
    int authen;
    int enc;
    int rc;

    if (is_read) {
        if (!(entry->ha_flags & BLE_ATT_F_READ)) {
            *out_att_err = BLE_ATT_ERR_READ_NOT_PERMITTED;
            return BLE_HS_EREJECT;
        }

        enc = entry->ha_flags & BLE_ATT_F_READ_ENC;
        authen = entry->ha_flags & BLE_ATT_F_READ_AUTHEN;
        author = entry->ha_flags & BLE_ATT_F_READ_AUTHOR;
    } else {
        if (!(entry->ha_flags & BLE_ATT_F_WRITE)) {
            *out_att_err = BLE_ATT_ERR_WRITE_NOT_PERMITTED;
            return BLE_HS_EREJECT;
        }

        enc = entry->ha_flags & BLE_ATT_F_WRITE_ENC;
        authen = entry->ha_flags & BLE_ATT_F_WRITE_AUTHEN;
        author = entry->ha_flags & BLE_ATT_F_WRITE_AUTHOR;
    }

    /* Bail early if this operation doesn't require security. */
    if (!enc && !authen && !author) {
        return 0;
    }

    ble_att_svr_get_sec_state(conn_handle, &sec_state);
    if ((enc || authen) && !sec_state.encrypted) {
        ble_hs_lock();
        conn = ble_hs_conn_find(conn_handle);
        if (conn != NULL) {
            ble_hs_conn_addrs(conn, &addrs);

            memset(&key_sec, 0, sizeof key_sec);
            key_sec.peer_addr = addrs.peer_id_addr;
        }
        ble_hs_unlock();

        rc = ble_store_read_peer_sec(&key_sec, &value_sec);
        if (rc == 0 && value_sec.ltk_present) {
            *out_att_err = BLE_ATT_ERR_INSUFFICIENT_ENC;
        } else {
            *out_att_err = BLE_ATT_ERR_INSUFFICIENT_AUTHEN;
        }

        return BLE_HS_ATT_ERR(*out_att_err);
    }

    if (authen && !sec_state.authenticated) {
        *out_att_err = BLE_ATT_ERR_INSUFFICIENT_AUTHEN;
        return BLE_HS_ATT_ERR(*out_att_err);
    }

    if (entry->ha_min_key_size > sec_state.key_size) {
        *out_att_err = BLE_ATT_ERR_INSUFFICIENT_KEY_SZ;
        return BLE_HS_ATT_ERR(*out_att_err);
    }

    if (author) {
        /* XXX: Prompt user for authorization. */
    }

    return 0;
}

/**
 * Calculates the number of ticks until a queued write times out on the
 * specified ATT server.  If this server is not in the process of receiving a
 * queued write, then BLE_HS_FOREVER is returned.  If a timeout just occurred,
 * 0 is returned.
 *
 * @param svr                   The ATT server to check.
 * @param now                   The current OS time.
 *
 * @return                      The number of ticks until the current queued
 *                                  write times out.
 */
int32_t
ble_att_svr_ticks_until_tmo(const struct ble_att_svr_conn *svr, ble_npl_time_t now)
{
#if BLE_HS_ATT_SVR_QUEUED_WRITE_TMO == 0
    return BLE_HS_FOREVER;
#endif

    int32_t time_diff;

    if (SLIST_EMPTY(&svr->basc_prep_list)) {
        return BLE_HS_FOREVER;
    }

    time_diff = svr->basc_prep_timeout_at - now;
    if (time_diff < 0) {
        return 0;
    }

    return time_diff;
}

/**
 * Allocates an mbuf to be used for an ATT response.  If an mbuf cannot be
 * allocated, the received request mbuf is reused for the error response.
 */
static int
ble_att_svr_pkt(struct os_mbuf **rxom, struct os_mbuf **out_txom,
                uint8_t *out_att_err)
{
    *out_txom = ble_hs_mbuf_l2cap_pkt();
    if (*out_txom != NULL) {
        return 0;
    }

    /* Allocation failure.  Reuse receive buffer for response. */
    *out_txom = *rxom;
    *rxom = NULL;
    *out_att_err = BLE_ATT_ERR_INSUFFICIENT_RES;
    return BLE_HS_ENOMEM;
}

static int
ble_att_svr_read(uint16_t conn_handle,
                 struct ble_att_svr_entry *entry,
                 uint16_t offset,
                 struct os_mbuf *om,
                 uint8_t *out_att_err)
{
    uint8_t att_err;
    int rc;

    att_err = 0;    /* Silence gcc warning. */

    if (conn_handle != BLE_HS_CONN_HANDLE_NONE) {
        rc = ble_att_svr_check_perms(conn_handle, 1, entry, &att_err);
        if (rc != 0) {
            goto err;
        }
    }

    BLE_HS_DBG_ASSERT(entry->ha_cb != NULL);
    rc = entry->ha_cb(conn_handle, entry->ha_handle_id,
                      BLE_ATT_ACCESS_OP_READ, offset, &om, entry->ha_cb_arg);
    if (rc != 0) {
        att_err = rc;
        rc = BLE_HS_EAPP;
        goto err;
    }

    return 0;

err:
    if (out_att_err != NULL) {
        *out_att_err = att_err;
    }
    return rc;
}

static int
ble_att_svr_read_flat(uint16_t conn_handle,
                      struct ble_att_svr_entry *entry,
                      uint16_t offset,
                      uint16_t max_len,
                      void *dst,
                      uint16_t *out_len,
                      uint8_t *out_att_err)
{
    struct os_mbuf *om;
    uint16_t len;
    int rc;

    om = ble_hs_mbuf_l2cap_pkt();
    if (om == NULL) {
        rc = BLE_HS_ENOMEM;
        goto done;
    }

    rc = ble_att_svr_read(conn_handle, entry, offset, om, out_att_err);
    if (rc != 0) {
        goto done;
    }

    len = OS_MBUF_PKTLEN(om);
    if (len > max_len) {
        rc = BLE_HS_EMSGSIZE;
        *out_att_err = BLE_ATT_ERR_UNLIKELY;
        goto done;
    }

    rc = os_mbuf_copydata(om, 0, len, dst);
    BLE_HS_DBG_ASSERT(rc == 0);

    *out_len = len;
    rc = 0;

done:
    os_mbuf_free_chain(om);
    return rc;
}

int
ble_att_svr_read_handle(uint16_t conn_handle, uint16_t attr_handle,
                        uint16_t offset, struct os_mbuf *om,
                        uint8_t *out_att_err)
{
    struct ble_att_svr_entry *entry;
    int rc;

    entry = ble_att_svr_find_by_handle(attr_handle);
    if (entry == NULL) {
        if (out_att_err != NULL) {
            *out_att_err = BLE_ATT_ERR_INVALID_HANDLE;
        }
        return BLE_HS_ENOENT;
    }

    rc = ble_att_svr_read(conn_handle, entry, offset, om, out_att_err);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

int
ble_att_svr_read_local(uint16_t attr_handle, struct os_mbuf **out_om)
{
    struct os_mbuf *om;
    int rc;

    om = ble_hs_mbuf_bare_pkt();
    if (om == NULL) {
        rc = BLE_HS_ENOMEM;
        goto err;
    }

    rc = ble_att_svr_read_handle(BLE_HS_CONN_HANDLE_NONE, attr_handle, 0, om,
                                 NULL);
    if (rc != 0) {
        goto err;
    }

    *out_om = om;
    return 0;

err:
    os_mbuf_free_chain(om);
    return rc;
}

static int
ble_att_svr_write(uint16_t conn_handle, struct ble_att_svr_entry *entry,
                  uint16_t offset, struct os_mbuf **om, uint8_t *out_att_err)
{
    uint8_t att_err = 0;
    int rc;

    BLE_HS_DBG_ASSERT(!ble_hs_locked_by_cur_task());

    if (conn_handle != BLE_HS_CONN_HANDLE_NONE) {
        rc = ble_att_svr_check_perms(conn_handle, 0, entry, &att_err);
        if (rc != 0) {
            goto done;
        }
    }

    BLE_HS_DBG_ASSERT(entry->ha_cb != NULL);
    rc = entry->ha_cb(conn_handle, entry->ha_handle_id,
                      BLE_ATT_ACCESS_OP_WRITE, offset, om, entry->ha_cb_arg);
    if (rc != 0) {
        att_err = rc;
        rc = BLE_HS_EAPP;
        goto done;
    }

done:
    if (out_att_err != NULL) {
        *out_att_err = att_err;
    }
    return rc;
}

static int
ble_att_svr_write_handle(uint16_t conn_handle, uint16_t attr_handle,
                         uint16_t offset, struct os_mbuf **om,
                         uint8_t *out_att_err)
{
    struct ble_att_svr_entry *entry;
    int rc;

    entry = ble_att_svr_find_by_handle(attr_handle);
    if (entry == NULL) {
        if (out_att_err != NULL) {
            *out_att_err = BLE_ATT_ERR_INVALID_HANDLE;
        }
        return BLE_HS_ENOENT;
    }

    rc = ble_att_svr_write(conn_handle, entry, offset, om, out_att_err);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

int
ble_att_svr_tx_error_rsp(uint16_t conn_handle, struct os_mbuf *txom,
                         uint8_t req_op, uint16_t handle, uint8_t error_code)
{
    struct ble_att_error_rsp *rsp;

    BLE_HS_DBG_ASSERT(error_code != 0);
    BLE_HS_DBG_ASSERT(OS_MBUF_PKTLEN(txom) == 0);

    rsp = ble_att_cmd_prepare(BLE_ATT_OP_ERROR_RSP, sizeof(*rsp), txom);
    if (rsp == NULL) {
        return BLE_HS_ENOMEM;
    }

    rsp->baep_req_op = req_op;
    rsp->baep_handle = htole16(handle);
    rsp->baep_error_code = error_code;

    return ble_att_tx(conn_handle, txom);
}

/**
 * Transmits a response or error message over the specified connection.
 *
 * The specified rc and err_status values control what gets sent as follows:
 *     o If rc == 0: tx an affirmative response.
 *     o Else if err_status != 0: tx an error response.
 *     o Else: tx nothing.
 *
 * In addition, if transmission of an affirmative response fails, an error is
 * sent instead.
 *
 * @param conn_handle           The handle of the connection to send over.
 * @param hs_status             The status indicating whether to transmit an
 *                                  affirmative response or an error.
 * @param txom                  Contains the affirmative response payload.
 * @param att_op                If an error is transmitted, this is the value
 *                                  of the error message's op field.
 * @param err_status            If an error is transmitted, this is the value
 *                                  of the error message's status field.
 * @param err_handle            If an error is transmitted, this is the value
 *                                  of the error message's attribute handle
 *                                  field.
 */
static int
ble_att_svr_tx_rsp(uint16_t conn_handle, int hs_status, struct os_mbuf *om,
                   uint8_t att_op, uint8_t err_status, uint16_t err_handle)
{
    struct ble_l2cap_chan *chan;
    struct ble_hs_conn *conn;
    int do_tx;
    int rc;

    if (hs_status != 0 && err_status == 0) {
        /* Processing failed, but err_status of 0 means don't send error. */
        do_tx = 0;
    } else {
        do_tx = 1;
    }

    if (do_tx) {
        ble_hs_lock();

        rc = ble_att_conn_chan_find(conn_handle, &conn, &chan);
        if (rc != 0) {
            /* No longer connected. */
            hs_status = rc;
        } else {
            if (hs_status == 0) {
                BLE_HS_DBG_ASSERT(om != NULL);

                ble_att_inc_tx_stat(om->om_data[0]);
                ble_att_truncate_to_mtu(chan, om);
                hs_status = ble_l2cap_tx(conn, chan, om);
                om = NULL;
                if (hs_status != 0) {
                    err_status = BLE_ATT_ERR_UNLIKELY;
                }
           }
        }

        ble_hs_unlock();

        if (hs_status != 0) {
            STATS_INC(ble_att_stats, error_rsp_tx);

            /* Reuse om for error response. */
            if (om == NULL) {
                om = ble_hs_mbuf_l2cap_pkt();
            } else {
                os_mbuf_adj(om, OS_MBUF_PKTLEN(om));
            }
            if (om != NULL) {
                ble_att_svr_tx_error_rsp(conn_handle, om, att_op,
                                         err_handle, err_status);
                om = NULL;
            }
        }
    }

    /* Free mbuf if it was not consumed (i.e., if the send failed). */
    os_mbuf_free_chain(om);

    return hs_status;
}

static int
ble_att_svr_build_mtu_rsp(uint16_t conn_handle, struct os_mbuf **rxom,
                          struct os_mbuf **out_txom, uint8_t *att_err)
{
    struct ble_att_mtu_cmd *cmd;
    struct ble_l2cap_chan *chan;
    struct os_mbuf *txom;
    uint16_t mtu;
    int rc;

    *att_err = 0; /* Silence unnecessary warning. */
    txom = NULL;

    ble_hs_lock();
    rc = ble_att_conn_chan_find(conn_handle, NULL, &chan);
    if (rc == 0) {
        mtu = chan->my_mtu;
    }
    ble_hs_unlock();

    if (rc != 0) {
        goto done;
    }

    /* Just reuse the request buffer for the response. */
    txom = *rxom;
    *rxom = NULL;
    os_mbuf_adj(txom, OS_MBUF_PKTLEN(txom));

    cmd = ble_att_cmd_prepare(BLE_ATT_OP_MTU_RSP, sizeof(*cmd), txom);
    if (cmd == NULL) {
        *att_err = BLE_ATT_ERR_INSUFFICIENT_RES;
        rc = BLE_HS_ENOMEM;
        goto done;
    }

    cmd->bamc_mtu = htole16(mtu);

    rc = 0;

done:
    *out_txom = txom;
    return rc;
}

int
ble_att_svr_rx_mtu(uint16_t conn_handle, struct os_mbuf **rxom)
{
    struct ble_att_mtu_cmd *cmd;
    struct ble_l2cap_chan *chan;
    struct ble_hs_conn *conn;
    struct os_mbuf *txom;
    uint16_t mtu;
    uint8_t att_err;
    int rc;

    txom = NULL;
    mtu = 0;

    rc = ble_att_svr_pullup_req_base(rxom, sizeof(*cmd), &att_err);
    if (rc != 0) {
        goto done;
    }

    cmd = (struct ble_att_mtu_cmd *)(*rxom)->om_data;

    mtu = le16toh(cmd->bamc_mtu);

    rc = ble_att_svr_build_mtu_rsp(conn_handle, rxom, &txom, &att_err);
    if (rc != 0) {
        goto done;
    }

    rc = 0;

done:
    rc = ble_att_svr_tx_rsp(conn_handle, rc, txom, BLE_ATT_OP_MTU_REQ,
                            att_err, 0);
    if (rc == 0) {
        ble_hs_lock();

        rc = ble_att_conn_chan_find(conn_handle, &conn, &chan);
        if (rc == 0) {
            ble_att_set_peer_mtu(chan, mtu);
            chan->flags |= BLE_L2CAP_CHAN_F_TXED_MTU;
            mtu = ble_att_chan_mtu(chan);
        }

        ble_hs_unlock();

        if (rc == 0) {
            ble_gap_mtu_event(conn_handle, BLE_L2CAP_CID_ATT, mtu);
        }
    }
    return rc;
}

/**
 * Fills the supplied mbuf with the variable length Information Data field of a
 * Find Information ATT response.
 *
 * @param req                   The Find Information request being responded
 *                                  to.
 * @param om                    The destination mbuf where the Information
 *                                  Data field gets written.
 * @param mtu                   The ATT L2CAP channel MTU.
 * @param format                On success, the format field of the response
 *                                  gets stored here.  One of:
 *                                     o BLE_ATT_FIND_INFO_RSP_FORMAT_16BIT
 *                                     o BLE_ATT_FIND_INFO_RSP_FORMAT_128BIT
 *
 * @return                      0 on success; nonzero on failure.
 */
static int
ble_att_svr_fill_info(uint16_t start_handle, uint16_t end_handle,
                      struct os_mbuf *om, uint16_t mtu, uint8_t *format)
{
    struct ble_att_svr_entry *ha;
    uint8_t *buf;
    int num_entries;
    int entry_sz;
    int rc;

    *format = 0;
    num_entries = 0;
    rc = 0;

    STAILQ_FOREACH(ha, &ble_att_svr_list, ha_next) {
        if (ha->ha_handle_id > end_handle) {
            rc = 0;
            goto done;
        }
        if (ha->ha_handle_id >= start_handle) {
            if (ha->ha_uuid->type == BLE_UUID_TYPE_16) {
                if (*format == 0) {
                    *format = BLE_ATT_FIND_INFO_RSP_FORMAT_16BIT;
                } else if (*format != BLE_ATT_FIND_INFO_RSP_FORMAT_16BIT) {
                    rc = 0;
                    goto done;
                }

                entry_sz = 4;
            } else {
                if (*format == 0) {
                    *format = BLE_ATT_FIND_INFO_RSP_FORMAT_128BIT;
                } else if (*format != BLE_ATT_FIND_INFO_RSP_FORMAT_128BIT) {
                    rc = 0;
                    goto done;
                }
                entry_sz = 18;
            }

            if (OS_MBUF_PKTLEN(om) + entry_sz > mtu) {
                rc = 0;
                goto done;
            }

            buf = os_mbuf_extend(om, entry_sz);
            if (buf == NULL) {
                rc = BLE_HS_ENOMEM;
                goto done;
            }

            put_le16(buf + 0, ha->ha_handle_id);

            ble_uuid_flat(ha->ha_uuid, buf + 2);

            num_entries++;
        }
    }

done:
    if (rc == 0 && num_entries == 0) {
        return BLE_HS_ENOENT;
    } else {
        return rc;
    }
}

static int
ble_att_svr_build_find_info_rsp(uint16_t conn_handle,
                                uint16_t start_handle, uint16_t end_handle,
                                struct os_mbuf **rxom,
                                struct os_mbuf **out_txom,
                                uint8_t *att_err)
{
    struct ble_att_find_info_rsp *rsp;
    struct os_mbuf *txom;
    uint16_t mtu;
    int rc;

    /* Just reuse the request buffer for the response. */
    txom = *rxom;
    *rxom = NULL;
    os_mbuf_adj(txom, OS_MBUF_PKTLEN(txom));

    /* Write the response base at the start of the buffer.  The format field is
     * unknown at this point; it will be filled in later.
     */
    rsp = ble_att_cmd_prepare(BLE_ATT_OP_FIND_INFO_RSP, sizeof(*rsp), txom);
    if (rsp == NULL) {
        *att_err = BLE_ATT_ERR_INSUFFICIENT_RES;
        rc = BLE_HS_ENOMEM;
        goto done;
    }

    /* Write the variable length Information Data field, populating the format
     * field as appropriate.
     */
    mtu = ble_att_mtu(conn_handle);
    rc = ble_att_svr_fill_info(start_handle, end_handle, txom, mtu,
                               &rsp->bafp_format);
    if (rc != 0) {
        *att_err = BLE_ATT_ERR_ATTR_NOT_FOUND;
        rc = BLE_HS_ENOENT;
        goto done;
    }

    rc = 0;

done:
    *out_txom = txom;
    return rc;
}

int
ble_att_svr_rx_find_info(uint16_t conn_handle, struct os_mbuf **rxom)
{
#if !MYNEWT_VAL(BLE_ATT_SVR_FIND_INFO)
    return BLE_HS_ENOTSUP;
#endif

    struct ble_att_find_info_req *req;
    struct os_mbuf *txom;
    uint16_t err_handle, start_handle, end_handle;
    uint8_t att_err;
    int rc;

    /* Initialize some values in case of early error. */
    txom = NULL;
    att_err = 0;
    err_handle = 0;

    rc = ble_att_svr_pullup_req_base(rxom, sizeof(*req), &att_err);
    if (rc != 0) {
        err_handle = 0;
        goto done;
    }

    req = (struct ble_att_find_info_req *)(*rxom)->om_data;
    start_handle = le16toh(req->bafq_start_handle);
    end_handle = le16toh(req->bafq_end_handle);

    /* Tx error response if start handle is greater than end handle or is equal
     * to 0 (Vol. 3, Part F, 3.4.3.1).
     */
    if (start_handle > end_handle || start_handle == 0) {
        att_err = BLE_ATT_ERR_INVALID_HANDLE;
        err_handle = start_handle;
        rc = BLE_HS_EBADDATA;
        goto done;
    }

    rc = ble_att_svr_build_find_info_rsp(conn_handle,
                                        start_handle, end_handle,
                                        rxom, &txom, &att_err);
    if (rc != 0) {
        err_handle = start_handle;
        goto done;
    }

    rc = 0;

done:
    rc = ble_att_svr_tx_rsp(conn_handle, rc, txom, BLE_ATT_OP_FIND_INFO_REQ,
                            att_err, err_handle);
    return rc;
}

/**
 * Fills a Find-By-Type-Value-Response with single entry.
 *
 * @param om                    The response mbuf.
 * @param first                 First handle ID in the current group of IDs.
 * @param last                  Last handle ID in the current group of ID.
 * @param mtu                   The ATT L2CAP channel MTU.
 *
 * @return                      0 if the response should be sent;
 *                              BLE_HS_EAGAIN if the entry was successfully
 *                                  processed and subsequent entries can be
 *                                  inspected.
 *                              Other nonzero on error.
 */
static int
ble_att_svr_fill_type_value_entry(struct os_mbuf *om, uint16_t first,
                                     uint16_t last, int mtu,
                                     uint8_t *out_att_err)
{
    uint16_t u16;
    int rsp_sz;
    int rc;

    rsp_sz = OS_MBUF_PKTHDR(om)->omp_len + 4;
    if (rsp_sz > mtu) {
        return 0;
    }

    put_le16(&u16, first);
    rc = os_mbuf_append(om, &u16, 2);
    if (rc != 0) {
        *out_att_err = BLE_ATT_ERR_INSUFFICIENT_RES;
        return BLE_HS_ENOMEM;
    }

    put_le16(&u16, last);
    rc = os_mbuf_append(om, &u16, 2);
    if (rc != 0) {
        *out_att_err = BLE_ATT_ERR_INSUFFICIENT_RES;
        return BLE_HS_ENOMEM;
    }

    return BLE_HS_EAGAIN;
}

static int
ble_att_svr_is_valid_find_group_type(const ble_uuid_t *uuid)
{
    uint16_t uuid16;

    uuid16 = ble_uuid_u16(uuid);

    return uuid16 == BLE_ATT_UUID_PRIMARY_SERVICE ||
           uuid16 == BLE_ATT_UUID_SECONDARY_SERVICE ||
           uuid16 == BLE_ATT_UUID_CHARACTERISTIC;
}

static int
ble_att_svr_is_valid_group_end(const ble_uuid_t *uuid_group,
                               const ble_uuid_t *uuid)
{
    uint16_t uuid16;

    /* Grouping is defined only for 16-bit UUIDs, so any attribute ends group
     * for non-16-bit UUIDs.
     */
    if (uuid_group->type != BLE_UUID_TYPE_16) {
        return 1;
    }

    /* Grouping is defined only for 16-bit UUIDs, so non-16-bit UUID attribute
     * cannot end group.
     */
    if (uuid->type != BLE_UUID_TYPE_16) {
        return 0;
    }

    switch (ble_uuid_u16(uuid_group)) {
    case BLE_ATT_UUID_PRIMARY_SERVICE:
    case BLE_ATT_UUID_SECONDARY_SERVICE:
        uuid16 = ble_uuid_u16(uuid);

        /* Only Primary or Secondary Service types end service group. */
        return uuid16 == BLE_ATT_UUID_PRIMARY_SERVICE ||
               uuid16 == BLE_ATT_UUID_SECONDARY_SERVICE;
    case BLE_ATT_UUID_CHARACTERISTIC:
        /* Any valid grouping type ends characteristic group */
        return ble_att_svr_is_valid_find_group_type(uuid);
    default:
        /* Any attribute type ends group of non-grouping type */
        return 1;
    }
}

/**
 * Fills the supplied mbuf with the variable length Handles-Information-List
 * field of a Find-By-Type-Value ATT response.
 *
 * @param req                   The Find-By-Type-Value-Request being responded
 *                                  to.
 * @param rxom                  The mbuf containing the received request.
 * @param txom                  The destination mbuf where the
 *                                  Handles-Information-List field gets
 *                                  written.
 * @param mtu                   The ATT L2CAP channel MTU.
 *
 * @return                      0 on success;
 *                              BLE_HS_ENOENT if attribute not found;
 *                              BLE_HS_EAPP on other error.
 */
static int
ble_att_svr_fill_type_value(uint16_t conn_handle,
                            uint16_t start_handle, uint16_t end_handle,
                            ble_uuid16_t attr_type,
                            struct os_mbuf *rxom, struct os_mbuf *txom,
                            uint16_t mtu, uint8_t *out_att_err)
{
    struct ble_att_svr_entry *ha;
    uint8_t buf[16];
    uint16_t attr_len;
    uint16_t first;
    uint16_t prev;
    int any_entries;
    int rc;

    first = 0;
    prev = 0;
    rc = 0;

    /* Iterate through the attribute list, keeping track of the current
     * matching group.  For each attribute entry, determine if data needs to be
     * written to the response.
     */
    STAILQ_FOREACH(ha, &ble_att_svr_list, ha_next) {
        if (ha->ha_handle_id < start_handle) {
            continue;
        }

        /* Continue to look for end of group in case group is in progress. */
        if (!first && ha->ha_handle_id > end_handle) {
            break;
        }

        /* With group in progress, check if current attribute ends it. */
        if (first) {
            if (!ble_att_svr_is_valid_group_end(&attr_type.u, ha->ha_uuid)) {
                prev = ha->ha_handle_id;
                continue;
            }

            rc = ble_att_svr_fill_type_value_entry(txom, first, prev, mtu,
                                                   out_att_err);
            if (rc != BLE_HS_EAGAIN) {
                goto done;
            }

            first = 0;
            prev = 0;

            /* Break in case we were just looking for end of group past the end
             * handle ID. */
            if (ha->ha_handle_id > end_handle) {
                break;
            }
        }

        /* Compare the attribute type and value to the request fields to
         * determine if this attribute matches.
         */
        if (ble_uuid_cmp(ha->ha_uuid, &attr_type.u) == 0) {
            rc = ble_att_svr_read_flat(conn_handle, ha, 0, sizeof buf, buf,
                                       &attr_len, out_att_err);
            if (rc != 0) {
                goto done;
            }
            /* value is at the end of req */
            rc = os_mbuf_cmpf(rxom, sizeof(struct ble_att_find_type_value_req),
                              buf, attr_len);
            if (rc == 0) {
                first = ha->ha_handle_id;
                prev = ha->ha_handle_id;
            }
        }
    }

    /* Process last group in case a group was in progress when the end of the
     * attribute list was reached.
     */
    if (first) {
        rc = ble_att_svr_fill_type_value_entry(txom, first, prev, mtu,
                                               out_att_err);
        if (rc == BLE_HS_EAGAIN) {
            rc = 0;
        }
    } else {
        rc = 0;
    }

done:
    any_entries = OS_MBUF_PKTHDR(txom)->omp_len >
                  BLE_ATT_FIND_TYPE_VALUE_RSP_BASE_SZ;
    if (rc == 0 && !any_entries) {
        *out_att_err = BLE_ATT_ERR_ATTR_NOT_FOUND;
        return BLE_HS_ENOENT;
    } else {
        return rc;
    }
}

static int
ble_att_svr_build_find_type_value_rsp(uint16_t conn_handle,
                                      uint16_t start_handle,
                                      uint16_t end_handle,
                                      ble_uuid16_t attr_type,
                                      struct os_mbuf **rxom,
                                      struct os_mbuf **out_txom,
                                      uint8_t *out_att_err)
{
    struct os_mbuf *txom;
    uint16_t mtu;
    uint8_t *buf;
    int rc;

    rc = ble_att_svr_pkt(rxom, &txom, out_att_err);
    if (rc != 0) {
        goto done;
    }

    /* info list is filled later on */
    buf = ble_att_cmd_prepare(BLE_ATT_OP_FIND_TYPE_VALUE_RSP, 0, txom);
    if (buf == NULL) {
        *out_att_err = BLE_ATT_ERR_INSUFFICIENT_RES;
        rc = BLE_HS_ENOMEM;
        goto done;
    }

    /* Write the variable length Information Data field. */
    mtu = ble_att_mtu(conn_handle);

    rc = ble_att_svr_fill_type_value(conn_handle, start_handle, end_handle,
                                     attr_type, *rxom, txom, mtu,
                                     out_att_err);
    if (rc != 0) {
        goto done;
    }

    rc = 0;

done:
    *out_txom = txom;
    return rc;
}

int
ble_att_svr_rx_find_type_value(uint16_t conn_handle, struct os_mbuf **rxom)
{
#if !MYNEWT_VAL(BLE_ATT_SVR_FIND_TYPE)
    return BLE_HS_ENOTSUP;
#endif

    struct ble_att_find_type_value_req *req;
    uint16_t start_handle, end_handle;
    ble_uuid16_t attr_type;
    struct os_mbuf *txom;
    uint16_t err_handle;
    uint8_t att_err;
    int rc;

    /* Initialize some values in case of early error. */
    txom = NULL;
    att_err = 0;
    err_handle = 0;

    rc = ble_att_svr_pullup_req_base(rxom, sizeof(*req), &att_err);
    if (rc != 0) {
        goto done;
    }

    req = (struct ble_att_find_type_value_req *)(*rxom)->om_data;
    start_handle = le16toh(req->bavq_start_handle);
    end_handle = le16toh(req->bavq_end_handle);
    attr_type = (ble_uuid16_t) BLE_UUID16_INIT(le16toh(req->bavq_attr_type));

    /* Tx error response if start handle is greater than end handle or is equal
     * to 0 (Vol. 3, Part F, 3.4.3.3).
     */
    if (start_handle > end_handle || start_handle == 0) {
        att_err = BLE_ATT_ERR_INVALID_HANDLE;
        err_handle = start_handle;
        rc = BLE_HS_EBADDATA;
        goto done;
    }
    rc = ble_att_svr_build_find_type_value_rsp(conn_handle, start_handle,
                                               end_handle, attr_type, rxom,
                                               &txom, &att_err);
    if (rc != 0) {
        err_handle = start_handle;
        goto done;
    }

    rc = 0;

done:
    rc = ble_att_svr_tx_rsp(conn_handle, rc, txom,
                            BLE_ATT_OP_FIND_TYPE_VALUE_REQ, att_err,
                            err_handle);
    return rc;
}

static int
ble_att_svr_build_read_type_rsp(uint16_t conn_handle,
                                uint16_t start_handle, uint16_t end_handle,
                                const ble_uuid_t *uuid,
                                struct os_mbuf **rxom,
                                struct os_mbuf **out_txom,
                                uint8_t *att_err,
                                uint16_t *err_handle)
{
    struct ble_att_attr_data_list *data;
    struct ble_att_read_type_rsp *rsp;
    struct ble_att_svr_entry *entry;
    struct os_mbuf *txom;
    uint16_t attr_len;
    uint16_t mtu;
    uint8_t buf[19];
    int entry_written;
    int txomlen;
    int prev_attr_len;
    int rc;

    *att_err = 0;    /* Silence unnecessary warning. */

    *err_handle = start_handle;
    entry_written = 0;
    prev_attr_len = 0;

    /* Just reuse the request buffer for the response. */
    txom = *rxom;
    *rxom = NULL;
    os_mbuf_adj(txom, OS_MBUF_PKTLEN(txom));

    /* Allocate space for the respose base, but don't fill in the fields.  They
     * get filled in at the end, when we know the value of the length field.
     */

    rsp = ble_att_cmd_prepare(BLE_ATT_OP_READ_TYPE_RSP, sizeof(*rsp), txom);
    if (rsp == NULL) {
        *att_err = BLE_ATT_ERR_INSUFFICIENT_RES;
        *err_handle = 0;
        rc = BLE_HS_ENOMEM;
        goto done;
    }

    mtu = ble_att_mtu(conn_handle);

    /* Find all matching attributes, writing a record for each. */
    entry = NULL;
    while (1) {
        entry = ble_att_svr_find_by_uuid(entry, uuid, end_handle);
        if (entry == NULL) {
            rc = BLE_HS_ENOENT;
            break;
        }

        if (entry->ha_handle_id >= start_handle) {
            rc = ble_att_svr_read_flat(conn_handle, entry, 0, sizeof buf, buf,
                                       &attr_len, att_err);
            if (rc != 0) {
                *err_handle = entry->ha_handle_id;
                goto done;
            }

            if (attr_len > mtu - 4) {
                attr_len = mtu - 4;
            }

            if (prev_attr_len == 0) {
                prev_attr_len = attr_len;
            } else if (prev_attr_len != attr_len) {
                break;
            }

            txomlen = OS_MBUF_PKTHDR(txom)->omp_len + 2 + attr_len;
            if (txomlen > mtu) {
                break;
            }

            data = os_mbuf_extend(txom, 2 + attr_len);
            if (data == NULL) {
                *att_err = BLE_ATT_ERR_INSUFFICIENT_RES;
                *err_handle = entry->ha_handle_id;
                rc = BLE_HS_ENOMEM;
                goto done;
            }

            data->handle = htole16(entry->ha_handle_id);
            memcpy(data->value, buf, attr_len);
            entry_written = 1;
        }
    }

done:
    if (!entry_written) {
        /* No matching attributes. */
        if (*att_err == 0) {
            *att_err = BLE_ATT_ERR_ATTR_NOT_FOUND;
        }
        if (rc == 0) {
            rc = BLE_HS_ENOENT;
        }
    } else {
        /* Send what we can, even if an error was encountered. */
        rc = 0;
        *att_err = 0;

        /* Fill the response base. */
        rsp->batp_length = htole16(sizeof(*data) + prev_attr_len);
    }

    *out_txom = txom;

    return rc;
}

int
ble_att_svr_rx_read_type(uint16_t conn_handle, struct os_mbuf **rxom)
{
#if !MYNEWT_VAL(BLE_ATT_SVR_READ_TYPE)
    return BLE_HS_ENOTSUP;
#endif

    struct ble_att_read_type_req *req;
    uint16_t start_handle, end_handle;
    struct os_mbuf *txom;
    uint16_t err_handle;
    uint16_t pktlen;
    ble_uuid_any_t uuid;
    uint8_t att_err;
    int rc;

    /* Initialize some values in case of early error. */
    txom = NULL;
    err_handle = 0;
    att_err = 0;

    pktlen = OS_MBUF_PKTLEN(*rxom);
    if (pktlen != sizeof(*req) + 2 && pktlen != sizeof(*req) + 16) {
        /* Malformed packet */
        rc = BLE_HS_EBADDATA;
        goto done;
    }

    rc = ble_att_svr_pullup_req_base(rxom, pktlen, &att_err);
    if (rc != 0) {
        goto done;
    }

    req = (struct ble_att_read_type_req *)(*rxom)->om_data;

    start_handle = le16toh(req->batq_start_handle);
    end_handle = le16toh(req->batq_end_handle);

    if (start_handle > end_handle || start_handle == 0) {
        att_err = BLE_ATT_ERR_INVALID_HANDLE;
        err_handle = start_handle;
        rc = BLE_HS_EBADDATA;
        goto done;
    }

    rc = ble_uuid_init_from_att_mbuf(&uuid, *rxom, sizeof(*req),
                                     pktlen - sizeof(*req));
    if (rc != 0) {
        att_err = BLE_ATT_ERR_INVALID_PDU;
        rc = BLE_HS_EMSGSIZE;
        goto done;
    }

    rc = ble_att_svr_build_read_type_rsp(conn_handle, start_handle, end_handle,
                                         &uuid.u, rxom, &txom, &att_err,
                                         &err_handle);
    if (rc != 0) {
        goto done;
    }

    rc = 0;

done:
    rc = ble_att_svr_tx_rsp(conn_handle, rc, txom, BLE_ATT_OP_READ_TYPE_REQ,
                            att_err, err_handle);
    return rc;
}

int
ble_att_svr_rx_read(uint16_t conn_handle, struct os_mbuf **rxom)
{
#if !MYNEWT_VAL(BLE_ATT_SVR_READ)
    return BLE_HS_ENOTSUP;
#endif

    struct ble_att_read_req *req;
    struct os_mbuf *txom;
    uint16_t err_handle;
    uint8_t att_err;
    int rc;

    /* Initialize some values in case of early error. */
    txom = NULL;
    att_err = 0;
    err_handle = 0;

    rc = ble_att_svr_pullup_req_base(rxom, sizeof(*req), &att_err);
    if (rc != 0) {
        goto done;
    }

    req = (struct ble_att_read_req *)(*rxom)->om_data;

    err_handle = le16toh(req->barq_handle);

    /* Just reuse the request buffer for the response. */
    txom = *rxom;
    *rxom = NULL;
    os_mbuf_adj(txom, OS_MBUF_PKTLEN(txom));

    if (ble_att_cmd_prepare(BLE_ATT_OP_READ_RSP, 0, txom) == NULL) {
        att_err = BLE_ATT_ERR_INSUFFICIENT_RES;
        rc = BLE_HS_ENOMEM;
        goto done;
    }

    rc = ble_att_svr_read_handle(conn_handle, err_handle, 0, txom, &att_err);
    if (rc != 0) {
        goto done;
    }

done:
    rc = ble_att_svr_tx_rsp(conn_handle, rc, txom, BLE_ATT_OP_READ_REQ,
                            att_err, err_handle);
    return rc;
}

int
ble_att_svr_rx_read_blob(uint16_t conn_handle, struct os_mbuf **rxom)
{
#if !MYNEWT_VAL(BLE_ATT_SVR_READ_BLOB)
    return BLE_HS_ENOTSUP;
#endif

    struct ble_att_read_blob_req *req;
    struct os_mbuf *txom;
    uint16_t err_handle, offset;
    uint8_t att_err;
    int rc;

    /* Initialize some values in case of early error. */
    txom = NULL;
    att_err = 0;
    err_handle = 0;

    rc = ble_att_svr_pullup_req_base(rxom, sizeof(*req), &att_err);
    if (rc != 0) {
        goto done;
    }

    req = (struct ble_att_read_blob_req *)(*rxom)->om_data;

    err_handle = le16toh(req->babq_handle);
    offset = le16toh(req->babq_offset);

    /* Just reuse the request buffer for the response. */
    txom = *rxom;
    *rxom = NULL;
    os_mbuf_adj(txom, OS_MBUF_PKTLEN(txom));

    if (ble_att_cmd_prepare(BLE_ATT_OP_READ_BLOB_RSP, 0, txom) == NULL) {
        att_err = BLE_ATT_ERR_INSUFFICIENT_RES;
        rc = BLE_HS_ENOMEM;
        goto done;
    }

    rc = ble_att_svr_read_handle(conn_handle, err_handle, offset,
                                 txom, &att_err);
    if (rc != 0) {
        goto done;
    }

    rc = 0;

done:
    rc = ble_att_svr_tx_rsp(conn_handle, rc, txom, BLE_ATT_OP_READ_BLOB_REQ,
                            att_err, err_handle);
    return rc;
}

static int
ble_att_svr_build_read_mult_rsp(uint16_t conn_handle,
                                struct os_mbuf **rxom,
                                struct os_mbuf **out_txom,
                                uint8_t *att_err,
                                uint16_t *err_handle)
{
    struct os_mbuf *txom;
    uint16_t handle;
    uint16_t mtu;
    int rc;

    mtu = ble_att_mtu(conn_handle);

    rc = ble_att_svr_pkt(rxom, &txom, att_err);
    if (rc != 0) {
        *err_handle = 0;
        goto done;
    }

    if (ble_att_cmd_prepare(BLE_ATT_OP_READ_MULT_RSP, 0, txom) == NULL) {
        *att_err = BLE_ATT_ERR_INSUFFICIENT_RES;
        *err_handle = 0;
        rc = BLE_HS_ENOMEM;
        goto done;
    }

    /* Iterate through requested handles, reading the corresponding attribute
     * for each.  Stop when there are no more handles to process, or the
     * response is full.
     */
    while (OS_MBUF_PKTLEN(*rxom) >= 2 && OS_MBUF_PKTLEN(txom) < mtu) {
        /* Ensure the full 16-bit handle is contiguous at the start of the
         * mbuf.
         */
        rc = ble_att_svr_pullup_req_base(rxom, 2, att_err);
        if (rc != 0) {
            *err_handle = 0;
            goto done;
        }

        /* Extract the 16-bit handle and strip it from the front of the
         * mbuf.
         */
        handle = get_le16((*rxom)->om_data);
        os_mbuf_adj(*rxom, 2);

        rc = ble_att_svr_read_handle(conn_handle, handle, 0, txom, att_err);
        if (rc != 0) {
            *err_handle = handle;
            goto done;
        }
    }

    rc = 0;

done:
    *out_txom = txom;
    return rc;
}

int
ble_att_svr_rx_read_mult(uint16_t conn_handle, struct os_mbuf **rxom)
{
#if !MYNEWT_VAL(BLE_ATT_SVR_READ_MULT)
    return BLE_HS_ENOTSUP;
#endif

    struct os_mbuf *txom;
    uint16_t err_handle;
    uint8_t att_err;
    int rc;

    /* Initialize some values in case of early error. */
    txom = NULL;
    err_handle = 0;
    att_err = 0;

    rc = ble_att_svr_build_read_mult_rsp(conn_handle, rxom, &txom, &att_err,
                                         &err_handle);

    return ble_att_svr_tx_rsp(conn_handle, rc, txom, BLE_ATT_OP_READ_MULT_REQ,
                              att_err, err_handle);
}

static int
ble_att_svr_is_valid_read_group_type(const ble_uuid_t *uuid)
{
    uint16_t uuid16;

    uuid16 = ble_uuid_u16(uuid);

    return uuid16 == BLE_ATT_UUID_PRIMARY_SERVICE ||
           uuid16 == BLE_ATT_UUID_SECONDARY_SERVICE;
}

static int
ble_att_svr_service_uuid(struct ble_att_svr_entry *entry,
                         ble_uuid_any_t *uuid, uint8_t *out_att_err)
{
    uint8_t val[16];
    uint16_t attr_len;
    int rc;

    rc = ble_att_svr_read_flat(BLE_HS_CONN_HANDLE_NONE, entry, 0, sizeof(val), val,
                               &attr_len, out_att_err);
    if (rc != 0) {
        return rc;
    }

    rc = ble_uuid_init_from_buf(uuid, val, attr_len);

    return rc;
}

static int
ble_att_svr_read_group_type_entry_write(struct os_mbuf *om, uint16_t mtu,
                                        uint16_t start_group_handle,
                                        uint16_t end_group_handle,
                                        const ble_uuid_t *service_uuid)
{
    uint8_t *buf;
    int len;

    if (service_uuid->type == BLE_UUID_TYPE_16) {
        len = BLE_ATT_READ_GROUP_TYPE_ADATA_SZ_16;
    } else {
        len = BLE_ATT_READ_GROUP_TYPE_ADATA_SZ_128;
    }
    if (OS_MBUF_PKTLEN(om) + len > mtu) {
        return BLE_HS_EMSGSIZE;
    }

    buf = os_mbuf_extend(om, len);
    if (buf == NULL) {
        return BLE_HS_ENOMEM;
    }

    put_le16(buf + 0, start_group_handle);
    put_le16(buf + 2, end_group_handle);
    ble_uuid_flat(service_uuid, buf + 4);

    return 0;
}

/**
 * @return                      0 on success; BLE_HS error code on failure.
 */
static int
ble_att_svr_build_read_group_type_rsp(uint16_t conn_handle,
                                      uint16_t start_handle,
                                      uint16_t end_handle,
                                      const ble_uuid_t *group_uuid,
                                      struct os_mbuf **rxom,
                                      struct os_mbuf **out_txom,
                                      uint8_t *att_err,
                                      uint16_t *err_handle)
{
    struct ble_att_read_group_type_rsp *rsp;
    struct ble_att_svr_entry *entry;
    struct os_mbuf *txom;
    uint16_t start_group_handle;
    uint16_t end_group_handle;
    uint16_t mtu;
    ble_uuid_any_t service_uuid;
    int rc;

    /* Silence warnings. */
    end_group_handle = 0;

    *att_err = 0;
    *err_handle = start_handle;

    mtu = ble_att_mtu(conn_handle);

    /* Just reuse the request buffer for the response. */
    txom = *rxom;
    *rxom = NULL;
    os_mbuf_adj(txom, OS_MBUF_PKTLEN(txom));

    /* Reserve space for the response base. */
    rsp = ble_att_cmd_prepare(BLE_ATT_OP_READ_GROUP_TYPE_RSP, sizeof(*rsp),
                              txom);
    if (rsp == NULL) {
        *att_err = BLE_ATT_ERR_INSUFFICIENT_RES;
        rc = BLE_HS_ENOMEM;
        goto done;
    }

    start_group_handle = 0;
    rsp->bagp_length = 0;
    STAILQ_FOREACH(entry, &ble_att_svr_list, ha_next) {
        if (entry->ha_handle_id < start_handle) {
            continue;
        }
        if (entry->ha_handle_id > end_handle) {
            /* The full input range has been searched. */
            rc = 0;
            goto done;
        }

        if (start_group_handle != 0) {
            /* We have already found the start of a group. */
            if (!ble_att_svr_is_valid_read_group_type(entry->ha_uuid)) {
                /* This attribute is part of the current group. */
                end_group_handle = entry->ha_handle_id;
            } else {
                /* This attribute marks the end of the group.  Write an entry
                 * representing the group to the response.
                 */
                rc = ble_att_svr_read_group_type_entry_write(
                    txom, mtu, start_group_handle, end_group_handle,
                    &service_uuid.u);
                start_group_handle = 0;
                end_group_handle = 0;
                if (rc != 0) {
                    *err_handle = entry->ha_handle_id;
                    if (rc == BLE_HS_ENOMEM) {
                        *att_err = BLE_ATT_ERR_INSUFFICIENT_RES;
                    } else {
                        BLE_HS_DBG_ASSERT(rc == BLE_HS_EMSGSIZE);
                    }
                    goto done;
                }
            }
        }

        if (start_group_handle == 0) {
            /* We are looking for the start of a group. */
            if (ble_uuid_cmp(entry->ha_uuid, group_uuid) == 0) {
                /* Found a group start.  Read the group UUID. */
                rc = ble_att_svr_service_uuid(entry, &service_uuid, att_err);
                if (rc != 0) {
                    *err_handle = entry->ha_handle_id;
                    goto done;
                }

                /* Make sure the group UUID lengths are consistent.  If this
                 * group has a different length UUID, then cut the response
                 * short.
                 */
                switch (rsp->bagp_length) {
                case 0:
                    if (service_uuid.u.type == BLE_UUID_TYPE_16) {
                        rsp->bagp_length = BLE_ATT_READ_GROUP_TYPE_ADATA_SZ_16;
                    } else {
                        rsp->bagp_length = BLE_ATT_READ_GROUP_TYPE_ADATA_SZ_128;
                    }
                    break;

                case BLE_ATT_READ_GROUP_TYPE_ADATA_SZ_16:
                    if (service_uuid.u.type != BLE_UUID_TYPE_16) {
                        rc = 0;
                        goto done;
                    }
                    break;

                case BLE_ATT_READ_GROUP_TYPE_ADATA_SZ_128:
                    if (service_uuid.u.type == BLE_UUID_TYPE_16) {
                        rc = 0;
                        goto done;
                    }
                    break;

                default:
                    BLE_HS_DBG_ASSERT(0);
                    goto done;
                }

                start_group_handle = entry->ha_handle_id;
                end_group_handle = entry->ha_handle_id;
            }
        }
    }

    rc = 0;

done:
    if (rc == 0) {
        if (start_group_handle != 0) {
            /* A group was being processed.  Add its corresponding entry to the
             * response.
             */

            if (entry == NULL) {
                /* We have reached the end of the attribute list.  Indicate an
                 * end handle of 0xffff so that the client knows there are no
                 * more attributes without needing to send a follow-up request.
                 */
                end_group_handle = 0xffff;
            }

            rc = ble_att_svr_read_group_type_entry_write(txom, mtu,
                                                         start_group_handle,
                                                         end_group_handle,
                                                         &service_uuid.u);
            if (rc == BLE_HS_ENOMEM) {
                *att_err = BLE_ATT_ERR_INSUFFICIENT_RES;
            }
        }

        if (OS_MBUF_PKTLEN(txom) <= BLE_ATT_READ_GROUP_TYPE_RSP_BASE_SZ) {
            *att_err = BLE_ATT_ERR_ATTR_NOT_FOUND;
            rc = BLE_HS_ENOENT;
        }
    }

    if (rc == 0 || rc == BLE_HS_EMSGSIZE) {
        rc = 0;
    }

    *out_txom = txom;
    return rc;
}

int
ble_att_svr_rx_read_group_type(uint16_t conn_handle, struct os_mbuf **rxom)
{
#if !MYNEWT_VAL(BLE_ATT_SVR_READ_GROUP_TYPE)
    return BLE_HS_ENOTSUP;
#endif

    struct ble_att_read_group_type_req *req;
    struct os_mbuf *txom;
    ble_uuid_any_t uuid;
    uint16_t err_handle, start_handle, end_handle;
    uint16_t pktlen;
    uint8_t att_err;
    int om_uuid_len;
    int rc;

    /* Initialize some values in case of early error. */
    txom = NULL;
    err_handle = 0;
    att_err = 0;

    pktlen = OS_MBUF_PKTLEN(*rxom);
    if (pktlen != sizeof(*req) + 2 && pktlen != sizeof(*req) + 16) {
        /* Malformed packet */
        rc = BLE_HS_EBADDATA;
        goto done;
    }

    rc = ble_att_svr_pullup_req_base(rxom, pktlen, &att_err);
    if (rc != 0) {
        goto done;
    }

    req = (struct ble_att_read_group_type_req *)(*rxom)->om_data;

    start_handle = le16toh(req->bagq_start_handle);
    end_handle = le16toh(req->bagq_end_handle);

    if (start_handle > end_handle || start_handle == 0) {
        att_err = BLE_ATT_ERR_INVALID_HANDLE;
        err_handle = start_handle;
        rc = BLE_HS_EBADDATA;
        goto done;
    }

    om_uuid_len = OS_MBUF_PKTHDR(*rxom)->omp_len - sizeof(*req);
    rc = ble_uuid_init_from_att_mbuf(&uuid, *rxom, sizeof(*req), om_uuid_len);
    if (rc != 0) {
        att_err = BLE_ATT_ERR_INVALID_PDU;
        err_handle = start_handle;
        rc = BLE_HS_EBADDATA;
        goto done;
    }

    if (!ble_att_svr_is_valid_read_group_type(&uuid.u)) {
        att_err = BLE_ATT_ERR_UNSUPPORTED_GROUP;
        err_handle = start_handle;
        rc = BLE_HS_EREJECT;
        goto done;
    }

    rc = ble_att_svr_build_read_group_type_rsp(conn_handle, start_handle,
                                               end_handle, &uuid.u,
                                               rxom, &txom, &att_err,
                                               &err_handle);
    if (rc != 0) {
        goto done;
    }

    rc = 0;

done:
    rc = ble_att_svr_tx_rsp(conn_handle, rc, txom,
                            BLE_ATT_OP_READ_GROUP_TYPE_REQ, att_err,
                            err_handle);
    return rc;
}

static int
ble_att_svr_build_write_rsp(struct os_mbuf **rxom, struct os_mbuf **out_txom,
                            uint8_t *att_err)
{
    struct os_mbuf *txom;
    int rc;

    /* Allocate a new buffer for the response.  A write response never reuses
     * the request buffer.  See the note at the top of this file for details.
     */
    rc = ble_att_svr_pkt(rxom, &txom, att_err);
    if (rc != 0) {
        goto done;
    }

    if (ble_att_cmd_prepare(BLE_ATT_OP_WRITE_RSP, 0, txom) == NULL) {
        *att_err = BLE_ATT_ERR_INSUFFICIENT_RES;
        rc = BLE_HS_ENOMEM;
        goto done;
    }

    rc = 0;

done:
    *out_txom = txom;
    return rc;
}

int
ble_att_svr_rx_write(uint16_t conn_handle, struct os_mbuf **rxom)
{
#if !MYNEWT_VAL(BLE_ATT_SVR_WRITE)
    return BLE_HS_ENOTSUP;
#endif

    struct ble_att_write_req *req;
    struct os_mbuf *txom;
    uint16_t handle;
    uint8_t att_err;
    int rc;

    /* Initialize some values in case of early error. */
    txom = NULL;
    att_err = 0;
    handle = 0;

    rc = ble_att_svr_pullup_req_base(rxom, sizeof(*req), &att_err);
    if (rc != 0) {
        goto done;
    }

    req = (struct ble_att_write_req *)(*rxom)->om_data;

    handle = le16toh(req->bawq_handle);

    /* Allocate the write response.  This must be done prior to processing the
     * request.  See the note at the top of this file for details.
     */
    rc = ble_att_svr_build_write_rsp(rxom, &txom, &att_err);
    if (rc != 0) {
        goto done;
    }

    /* Strip the request base from the front of the mbuf. */
    os_mbuf_adj(*rxom, sizeof(*req));

    rc = ble_att_svr_write_handle(conn_handle, handle, 0, rxom, &att_err);
    if (rc != 0) {
        goto done;
    }

    rc = 0;

done:
    rc = ble_att_svr_tx_rsp(conn_handle, rc, txom, BLE_ATT_OP_WRITE_REQ,
                            att_err, handle);
    return rc;
}

int
ble_att_svr_rx_write_no_rsp(uint16_t conn_handle, struct os_mbuf **rxom)
{
#if !MYNEWT_VAL(BLE_ATT_SVR_WRITE_NO_RSP)
    return BLE_HS_ENOTSUP;
#endif

    struct ble_att_write_req *req;
    uint8_t att_err;
    uint16_t handle;
    int rc;

    rc = ble_att_svr_pullup_req_base(rxom, sizeof(*req), &att_err);
    if (rc != 0) {
        return rc;
    }

    req = (struct ble_att_write_req *)(*rxom)->om_data;

    handle = le16toh(req->bawq_handle);

    /* Strip the request base from the front of the mbuf. */
    os_mbuf_adj(*rxom, sizeof(*req));

    return ble_att_svr_write_handle(conn_handle, handle, 0, rxom, &att_err);
}

int
ble_att_svr_write_local(uint16_t attr_handle, struct os_mbuf *om)
{
    int rc;

    rc = ble_att_svr_write_handle(BLE_HS_CONN_HANDLE_NONE, attr_handle, 0,
                                  &om, NULL);

    /* Free the mbuf if it wasn't relinquished to the application. */
    os_mbuf_free_chain(om);

    return rc;
}

static void
ble_att_svr_prep_free(struct ble_att_prep_entry *entry)
{
    if (entry != NULL) {
        os_mbuf_free_chain(entry->bape_value);
#if MYNEWT_VAL(BLE_HS_DEBUG)
        memset(entry, 0xff, sizeof *entry);
#endif
        os_memblock_put(&ble_att_svr_prep_entry_pool, entry);
    }
}

static struct ble_att_prep_entry *
ble_att_svr_prep_alloc(uint8_t *att_err)
{
    struct ble_att_prep_entry *entry;

    entry = os_memblock_get(&ble_att_svr_prep_entry_pool);
    if (entry == NULL) {
        *att_err = BLE_ATT_ERR_PREPARE_QUEUE_FULL;
        return NULL;
    }

    memset(entry, 0, sizeof *entry);
    entry->bape_value = ble_hs_mbuf_l2cap_pkt();
    if (entry->bape_value == NULL) {
        ble_att_svr_prep_free(entry);
        *att_err = BLE_ATT_ERR_INSUFFICIENT_RES;
        return NULL;
    }

    return entry;
}

static struct ble_att_prep_entry *
ble_att_svr_prep_find_prev(struct ble_att_svr_conn *basc, uint16_t handle,
                           uint16_t offset)
{
    struct ble_att_prep_entry *entry;
    struct ble_att_prep_entry *prev;

    prev = NULL;
    SLIST_FOREACH(entry, &basc->basc_prep_list, bape_next) {
        if (entry->bape_handle > handle) {
            break;
        }

        if (entry->bape_handle == handle && entry->bape_offset > offset) {
            break;
        }

        prev = entry;
    }

    return prev;
}

void
ble_att_svr_prep_clear(struct ble_att_prep_entry_list *prep_list)
{
    struct ble_att_prep_entry *entry;

    while ((entry = SLIST_FIRST(prep_list)) != NULL) {
        SLIST_REMOVE_HEAD(prep_list, bape_next);
        ble_att_svr_prep_free(entry);
    }
}

/**
 * @return                      0 on success; ATT error code on failure.
 */
static int
ble_att_svr_prep_validate(struct ble_att_prep_entry_list *prep_list,
                          uint16_t *err_handle)
{
    struct ble_att_prep_entry *entry;
    struct ble_att_prep_entry *prev;
    int cur_len;

    prev = NULL;
    SLIST_FOREACH(entry, prep_list, bape_next) {
        if (prev == NULL || prev->bape_handle != entry->bape_handle) {
            /* Ensure attribute write starts at offset 0. */
            if (entry->bape_offset != 0) {
                *err_handle = entry->bape_handle;
                return BLE_ATT_ERR_INVALID_OFFSET;
            }
        } else {
            /* Ensure entry continues where previous left off. */
            if (prev->bape_offset + OS_MBUF_PKTLEN(prev->bape_value) !=
                entry->bape_offset) {

                *err_handle = entry->bape_handle;
                return BLE_ATT_ERR_INVALID_OFFSET;
            }
        }

        cur_len = entry->bape_offset + OS_MBUF_PKTLEN(entry->bape_value);
        if (cur_len > BLE_ATT_ATTR_MAX_LEN) {
            *err_handle = entry->bape_handle;
            return BLE_ATT_ERR_INVALID_ATTR_VALUE_LEN;
        }

        prev = entry;
    }

    return 0;
}

static void
ble_att_svr_prep_extract(struct ble_att_prep_entry_list *prep_list,
                         uint16_t *out_attr_handle,
                         struct os_mbuf **out_om)
{
    struct ble_att_prep_entry *entry;
    struct ble_att_prep_entry *first;
    struct os_mbuf *om;
    uint16_t attr_handle;

    BLE_HS_DBG_ASSERT(!SLIST_EMPTY(prep_list));

    first = SLIST_FIRST(prep_list);
    attr_handle = first->bape_handle;
    om = NULL;

    while ((entry = SLIST_FIRST(prep_list)) != NULL) {
        if (entry->bape_handle != attr_handle) {
            break;
        }

        if (om == NULL) {
            om = entry->bape_value;
        } else {
            os_mbuf_concat(om, entry->bape_value);
        }
        entry->bape_value = NULL;

        SLIST_REMOVE_HEAD(prep_list, bape_next);
        ble_att_svr_prep_free(entry);
    }

    *out_attr_handle = attr_handle;
    *out_om = om;
}

/**
 * @return                      0 on success; ATT error code on failure.
 */
static int
ble_att_svr_prep_write(uint16_t conn_handle,
                       struct ble_att_prep_entry_list *prep_list,
                       uint16_t *err_handle)
{
    struct ble_att_svr_entry *attr;
    struct os_mbuf *om;
    uint16_t attr_handle;
    uint8_t att_err;
    int rc;

    *err_handle = 0; /* Silence unnecessary warning. */

    /* First, validate the contents of the prepare queue. */
    rc = ble_att_svr_prep_validate(prep_list, err_handle);
    if (rc != 0) {
        return rc;
    }

    /* Contents are valid; perform the writes. */
    while (!SLIST_EMPTY(prep_list)) {
        ble_att_svr_prep_extract(prep_list, &attr_handle, &om);

        /* Attribute existence was verified during prepare-write request
         * processing.
         */
        attr = ble_att_svr_find_by_handle(attr_handle);
        BLE_HS_DBG_ASSERT(attr != NULL);

        rc = ble_att_svr_write(conn_handle, attr, 0, &om, &att_err);
        os_mbuf_free_chain(om);
        if (rc != 0) {
            *err_handle = attr_handle;
            return att_err;
        }
    }

    return 0;
}

static int
ble_att_svr_insert_prep_entry(uint16_t conn_handle,
                              uint16_t handle, uint16_t offset,
                              const struct os_mbuf *rxom,
                              uint8_t *out_att_err)
{
    struct ble_att_prep_entry *prep_entry;
    struct ble_att_prep_entry *prep_prev;
    struct ble_hs_conn *conn;
    int rc;

    conn = ble_hs_conn_find_assert(conn_handle);

    prep_entry = ble_att_svr_prep_alloc(out_att_err);
    if (prep_entry == NULL) {
        return BLE_HS_ENOMEM;
    }
    prep_entry->bape_handle = handle;
    prep_entry->bape_offset = offset;

    /* Append attribute value from request onto prep mbuf. */
    rc = os_mbuf_appendfrom(
        prep_entry->bape_value,
        rxom,
        sizeof(struct ble_att_prep_write_cmd),
        OS_MBUF_PKTLEN(rxom) - sizeof(struct ble_att_prep_write_cmd));
    if (rc != 0) {
        /* Failed to allocate an mbuf to hold the additional data. */
        ble_att_svr_prep_free(prep_entry);

        /* XXX: We need to differentiate between "prepare queue full" and
         * "insufficient resources."  Currently, we always indicate prepare
         * queue full.
         */
        *out_att_err = BLE_ATT_ERR_PREPARE_QUEUE_FULL;
        return rc;
    }

    prep_prev = ble_att_svr_prep_find_prev(&conn->bhc_att_svr,
                                           handle, offset);
    if (prep_prev == NULL) {
        SLIST_INSERT_HEAD(&conn->bhc_att_svr.basc_prep_list, prep_entry,
                          bape_next);
    } else {
        SLIST_INSERT_AFTER(prep_prev, prep_entry, bape_next);
    }

#if BLE_HS_ATT_SVR_QUEUED_WRITE_TMO != 0
    conn->bhc_att_svr.basc_prep_timeout_at =
        ble_npl_time_get() + BLE_HS_ATT_SVR_QUEUED_WRITE_TMO;

    ble_hs_timer_resched();
#endif

    return 0;
}

int
ble_att_svr_rx_prep_write(uint16_t conn_handle, struct os_mbuf **rxom)
{
#if !MYNEWT_VAL(BLE_ATT_SVR_QUEUED_WRITE)
    return BLE_HS_ENOTSUP;
#endif

    struct ble_att_prep_write_cmd *req;
    struct ble_att_svr_entry *attr_entry;
    struct os_mbuf *txom;
    uint16_t err_handle;
    uint8_t att_err;
    int rc;

    /* Initialize some values in case of early error. */
    txom = NULL;
    att_err = 0;
    err_handle = 0;

    rc = ble_att_svr_pullup_req_base(rxom, sizeof(*req), &att_err);
    if (rc != 0) {
        goto done;
    }

    req = (struct ble_att_prep_write_cmd *)(*rxom)->om_data;

    err_handle = le16toh(req->bapc_handle);

    attr_entry = ble_att_svr_find_by_handle(le16toh(req->bapc_handle));

    /* A prepare write request gets rejected for the following reasons:
     * 1. Insufficient authorization.
     * 2. Insufficient authentication.
     * 3. Insufficient encryption key size (XXX: Not checked).
     * 4. Insufficient encryption (XXX: Not checked).
     * 5. Invalid handle.
     * 6. Write not permitted.
     */

    /* <5> */
    if (attr_entry == NULL) {
        rc = BLE_HS_ENOENT;
        att_err = BLE_ATT_ERR_INVALID_HANDLE;
        goto done;
    }

    /* <1>, <2>, <4>, <6> */
    rc = ble_att_svr_check_perms(conn_handle, 0, attr_entry, &att_err);
    if (rc != 0) {
        goto done;
    }

    ble_hs_lock();
    rc = ble_att_svr_insert_prep_entry(conn_handle, le16toh(req->bapc_handle),
                                       le16toh(req->bapc_offset), *rxom,
                                       &att_err);
    ble_hs_unlock();

    /* Reuse rxom for response.  On success, the response is identical to
     * request except for op code.  On error, the buffer contents will get
     * cleared before the error gets written.
     */
    txom = *rxom;
    *rxom = NULL;

    if (rc != 0) {
        goto done;
    }

    /* adjust for ATT header */
    os_mbuf_prepend(txom, 1);
    txom->om_data[0] = BLE_ATT_OP_PREP_WRITE_RSP;

    rc = 0;

done:
    rc = ble_att_svr_tx_rsp(conn_handle, rc, txom, BLE_ATT_OP_PREP_WRITE_REQ,
                            att_err, err_handle);
    return rc;
}

int
ble_att_svr_rx_exec_write(uint16_t conn_handle, struct os_mbuf **rxom)
{
#if !MYNEWT_VAL(BLE_ATT_SVR_QUEUED_WRITE)
    return BLE_HS_ENOTSUP;
#endif

    struct ble_att_prep_entry_list prep_list;
    struct ble_att_exec_write_req *req;
    struct ble_hs_conn *conn;
    struct os_mbuf *txom;
    uint16_t err_handle;
    uint8_t att_err;
    uint8_t flags;
    int rc;

    /* Initialize some values in case of early error. */
    txom = NULL;
    err_handle = 0;

    rc = ble_att_svr_pullup_req_base(rxom, sizeof(*req), &att_err);
    if (rc != 0) {
        flags = 0;
        goto done;
    }

    req = (struct ble_att_exec_write_req *)(*rxom)->om_data;

    flags = req->baeq_flags;

    /* Just reuse the request buffer for the response. */
    txom = *rxom;
    *rxom = NULL;
    os_mbuf_adj(txom, OS_MBUF_PKTLEN(txom));

    if (ble_att_cmd_prepare(BLE_ATT_OP_EXEC_WRITE_RSP, 0, txom) == NULL) {
        att_err = BLE_ATT_ERR_INSUFFICIENT_RES;
        rc = BLE_HS_ENOMEM;
        goto done;
    }

    rc = 0;

done:
    if (rc == 0) {
        ble_hs_lock();
        conn = ble_hs_conn_find_assert(conn_handle);

        /* Extract the list of prepared writes from the connection so
         * that they can be processed after the mutex is unlocked.  They
         * aren't processed now because attribute writes involve executing
         * an application callback.
         */
        prep_list = conn->bhc_att_svr.basc_prep_list;
        SLIST_INIT(&conn->bhc_att_svr.basc_prep_list);
        ble_hs_unlock();

        if (flags) {
            /* Perform attribute writes. */
            att_err = ble_att_svr_prep_write(conn_handle, &prep_list,
                                             &err_handle);
            if (att_err != 0) {
                rc = BLE_HS_EAPP;
            }
        }

        /* Free the prep entries. */
        ble_att_svr_prep_clear(&prep_list);
    }

    rc = ble_att_svr_tx_rsp(conn_handle, rc, txom, BLE_ATT_OP_EXEC_WRITE_REQ,
                            att_err, err_handle);
    return rc;
}

int
ble_att_svr_rx_notify(uint16_t conn_handle, struct os_mbuf **rxom)
{
#if !MYNEWT_VAL(BLE_ATT_SVR_NOTIFY)
    return BLE_HS_ENOTSUP;
#endif

    struct ble_att_notify_req *req;
    uint16_t handle;
    int rc;

    rc = ble_att_svr_pullup_req_base(rxom, sizeof(*req), NULL);
    if (rc != 0) {
        return BLE_HS_ENOMEM;
    }

    req = (struct ble_att_notify_req *)(*rxom)->om_data;

    handle = le16toh(req->banq_handle);

    if (handle == 0) {
        return BLE_HS_EBADDATA;
    }

    /* Strip the request base from the front of the mbuf. */
    os_mbuf_adj(*rxom, sizeof(*req));

    ble_gap_notify_rx_event(conn_handle, handle, *rxom, 0);
    *rxom = NULL;

    return 0;
}

/**
 * @return                      0 on success; nonzero on failure.
 */
static int
ble_att_svr_build_indicate_rsp(struct os_mbuf **rxom,
                               struct os_mbuf **out_txom, uint8_t *out_att_err)
{
    struct os_mbuf *txom;
    int rc;

    /* Allocate a new buffer for the response.  An indicate response never
     * reuses the request buffer.  See the note at the top of this file for
     * details.
     */
    rc = ble_att_svr_pkt(rxom, &txom, out_att_err);
    if (rc != 0) {
        goto done;
    }

    if (ble_att_cmd_prepare(BLE_ATT_OP_INDICATE_RSP, 0, txom) == NULL) {
        rc = BLE_HS_ENOMEM;
        *out_att_err = BLE_ATT_ERR_INSUFFICIENT_RES;
        goto done;
    }

    rc = 0;

done:
    *out_txom = txom;
    return rc;
}

int
ble_att_svr_rx_indicate(uint16_t conn_handle, struct os_mbuf **rxom)
{
#if !MYNEWT_VAL(BLE_ATT_SVR_INDICATE)
    return BLE_HS_ENOTSUP;
#endif

    struct ble_att_indicate_req *req;
    struct os_mbuf *txom;
    uint16_t handle;
    uint8_t att_err;
    int rc;

    /* Initialize some values in case of early error. */
    txom = NULL;
    att_err = 0;
    handle = 0;

    rc = ble_att_svr_pullup_req_base(rxom, sizeof(*req), NULL);
    if (rc != 0) {
        goto done;
    }

    req = (struct ble_att_indicate_req *)(*rxom)->om_data;

    handle = le16toh(req->baiq_handle);

    if (handle == 0) {
        rc = BLE_HS_EBADDATA;
        goto done;
    }

    /* Allocate the indicate response.  This must be done prior to processing
     * the request.  See the note at the top of this file for details.
     */
    rc = ble_att_svr_build_indicate_rsp(rxom, &txom, &att_err);
    if (rc != 0) {
        goto done;
    }

    /* Strip the request base from the front of the mbuf. */
    os_mbuf_adj(*rxom, sizeof(*req));

    ble_gap_notify_rx_event(conn_handle, handle, *rxom, 1);
    *rxom = NULL;

    rc = 0;

done:
    rc = ble_att_svr_tx_rsp(conn_handle, rc, txom, BLE_ATT_OP_INDICATE_REQ,
                            att_err, handle);
    return rc;
}

static void
ble_att_svr_move_entries(struct ble_att_svr_entry_list *src,
                         struct ble_att_svr_entry_list *dst,
                         uint16_t start_handle, uint16_t end_handle)
{

    struct ble_att_svr_entry *entry;
    struct ble_att_svr_entry *prev;
    struct ble_att_svr_entry *remove;
    struct ble_att_svr_entry *insert;

    /* Find first matching element to move */
    remove = NULL;
    entry = STAILQ_FIRST(src);
    while (entry && entry->ha_handle_id < start_handle) {
        remove = entry;
        entry = STAILQ_NEXT(entry, ha_next);
    }

    /* Nothing to remove? */
    if (!entry) {
        return;
    }

    /* Find element after which we'll put moved elements */
    prev = NULL;
    insert = STAILQ_FIRST(dst);
    while (insert && insert->ha_handle_id < start_handle) {
        prev = insert;
        insert = STAILQ_NEXT(insert, ha_next);
    }
    insert = prev;

    /* Move elements */
    while (entry && entry->ha_handle_id <= end_handle) {
        /* Remove either from head or after prev (which is current one) */
        if (remove == NULL) {
            STAILQ_REMOVE_HEAD(src, ha_next);
        } else {
            STAILQ_REMOVE_AFTER(src, remove, ha_next);
        }

        /* Insert current element */
        if (insert == NULL) {
            STAILQ_INSERT_HEAD(dst, entry, ha_next);
            insert = STAILQ_FIRST(dst);
        } else {
            STAILQ_INSERT_AFTER(dst, insert, entry, ha_next);
            insert = entry;
        }

        /* Calculate next candidate to remove */
        if (remove == NULL) {
            entry = STAILQ_FIRST(src);
        } else {
            entry = STAILQ_NEXT(remove, ha_next);
        }
    }
}

void
ble_att_svr_hide_range(uint16_t start_handle, uint16_t end_handle)
{
    ble_att_svr_move_entries(&ble_att_svr_list, &ble_att_svr_hidden_list,
                             start_handle, end_handle);
}

void
ble_att_svr_restore_range(uint16_t start_handle, uint16_t end_handle)
{
    ble_att_svr_move_entries(&ble_att_svr_hidden_list, &ble_att_svr_list,
                             start_handle, end_handle);
}

void
ble_att_svr_reset(void)
{
    struct ble_att_svr_entry *entry;

    while ((entry = STAILQ_FIRST(&ble_att_svr_list)) != NULL) {
        STAILQ_REMOVE_HEAD(&ble_att_svr_list, ha_next);
        ble_att_svr_entry_free(entry);
    }

    while ((entry = STAILQ_FIRST(&ble_att_svr_hidden_list)) != NULL) {
        STAILQ_REMOVE_HEAD(&ble_att_svr_hidden_list, ha_next);
        ble_att_svr_entry_free(entry);
    }

    /* Note: prep entries do not get freed here because it is assumed there are
     * no established connections.
     */
}

static void
ble_att_svr_free_start_mem(void)
{
#ifdef ESP_PLATFORM
    nimble_platform_mem_free(ble_att_svr_entry_mem);
#else
    free(ble_att_svr_entry_mem);
#endif
    ble_att_svr_entry_mem = NULL;
}

int
ble_att_svr_start(void)
{
    int rc;

    ble_att_svr_free_start_mem();

    if (ble_hs_max_attrs > 0) {
#ifdef ESP_PLATFORM
        ble_att_svr_entry_mem = nimble_platform_mem_malloc(
#else
        ble_att_svr_entry_mem = malloc(
#endif
            OS_MEMPOOL_BYTES(ble_hs_max_attrs,
                             sizeof (struct ble_att_svr_entry)));
        if (ble_att_svr_entry_mem == NULL) {
            rc = BLE_HS_ENOMEM;
            goto err;
        }

        rc = os_mempool_init(&ble_att_svr_entry_pool, ble_hs_max_attrs,
                             sizeof (struct ble_att_svr_entry),
                             ble_att_svr_entry_mem, "ble_att_svr_entry_pool");
        if (rc != 0) {
            rc = BLE_HS_EOS;
            goto err;
        }
    }

    return 0;

err:
    ble_att_svr_free_start_mem();
    return rc;
}

void
ble_att_svr_stop(void)
{
    ble_att_svr_free_start_mem();
}

int
ble_att_svr_init(void)
{
    int rc;

    if (MYNEWT_VAL(BLE_ATT_SVR_MAX_PREP_ENTRIES) > 0) {
        rc = os_mempool_init(&ble_att_svr_prep_entry_pool,
                             MYNEWT_VAL(BLE_ATT_SVR_MAX_PREP_ENTRIES),
                             sizeof (struct ble_att_prep_entry),
                             ble_att_svr_prep_entry_mem,
                             "ble_att_svr_prep_entry_pool");
        if (rc != 0) {
            return BLE_HS_EOS;
        }
    }

    STAILQ_INIT(&ble_att_svr_list);
    STAILQ_INIT(&ble_att_svr_hidden_list);

    ble_att_svr_id = 0;

    return 0;
}
