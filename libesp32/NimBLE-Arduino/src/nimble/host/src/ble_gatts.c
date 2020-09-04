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

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "nimble/ble.h"
#include "host/ble_uuid.h"
#include "host/ble_store.h"
#include "ble_hs_priv.h"
#include "esp_nimble_mem.h"

#define BLE_GATTS_INCLUDE_SZ    6
#define BLE_GATTS_CHR_MAX_SZ    19

static const ble_uuid_t *uuid_pri =
    BLE_UUID16_DECLARE(BLE_ATT_UUID_PRIMARY_SERVICE);
static const ble_uuid_t *uuid_sec =
    BLE_UUID16_DECLARE(BLE_ATT_UUID_SECONDARY_SERVICE);
static const ble_uuid_t *uuid_inc =
    BLE_UUID16_DECLARE(BLE_ATT_UUID_INCLUDE);
static const ble_uuid_t *uuid_chr =
    BLE_UUID16_DECLARE(BLE_ATT_UUID_CHARACTERISTIC);
static const ble_uuid_t *uuid_ccc =
    BLE_UUID16_DECLARE(BLE_GATT_DSC_CLT_CFG_UUID16);

static const struct ble_gatt_svc_def **ble_gatts_svc_defs;
static int ble_gatts_num_svc_defs;

struct ble_gatts_svc_entry {
    const struct ble_gatt_svc_def *svc;
    uint16_t handle;            /* 0 means unregistered. */
    uint16_t end_group_handle;  /* 0xffff means unset. */
};

static struct ble_gatts_svc_entry *ble_gatts_svc_entries;
static uint16_t ble_gatts_num_svc_entries;

static os_membuf_t *ble_gatts_clt_cfg_mem;
static struct os_mempool ble_gatts_clt_cfg_pool;

struct ble_gatts_clt_cfg {
    uint16_t chr_val_handle;
    uint8_t flags;
    uint8_t allowed;
};

/** A cached array of handles for the configurable characteristics. */
static struct ble_gatts_clt_cfg *ble_gatts_clt_cfgs;
static int ble_gatts_num_cfgable_chrs;

STATS_SECT_DECL(ble_gatts_stats) ble_gatts_stats;
STATS_NAME_START(ble_gatts_stats)
    STATS_NAME(ble_gatts_stats, svcs)
    STATS_NAME(ble_gatts_stats, chrs)
    STATS_NAME(ble_gatts_stats, dscs)
    STATS_NAME(ble_gatts_stats, svc_def_reads)
    STATS_NAME(ble_gatts_stats, svc_inc_reads)
    STATS_NAME(ble_gatts_stats, chr_def_reads)
    STATS_NAME(ble_gatts_stats, chr_val_reads)
    STATS_NAME(ble_gatts_stats, chr_val_writes)
    STATS_NAME(ble_gatts_stats, dsc_reads)
    STATS_NAME(ble_gatts_stats, dsc_writes)
STATS_NAME_END(ble_gatts_stats)

static int
ble_gatts_svc_access(uint16_t conn_handle, uint16_t attr_handle,
                     uint8_t op, uint16_t offset, struct os_mbuf **om,
                     void *arg)
{
    const struct ble_gatt_svc_def *svc;
    uint8_t *buf;

    STATS_INC(ble_gatts_stats, svc_def_reads);

    BLE_HS_DBG_ASSERT(op == BLE_ATT_ACCESS_OP_READ);

    svc = arg;

    buf = os_mbuf_extend(*om, ble_uuid_length(svc->uuid));
    if (buf == NULL) {
        return BLE_ATT_ERR_INSUFFICIENT_RES;
    }

    ble_uuid_flat(svc->uuid, buf);

    return 0;
}

static int
ble_gatts_inc_access(uint16_t conn_handle, uint16_t attr_handle,
                     uint8_t op, uint16_t offset, struct os_mbuf **om,
                     void *arg)
{
    const struct ble_gatts_svc_entry *entry;
    uint16_t uuid16;
    uint8_t *buf;

    STATS_INC(ble_gatts_stats, svc_inc_reads);

    BLE_HS_DBG_ASSERT(op == BLE_ATT_ACCESS_OP_READ);

    entry = arg;

    buf = os_mbuf_extend(*om, 4);
    if (buf == NULL) {
        return BLE_ATT_ERR_INSUFFICIENT_RES;
    }
    put_le16(buf + 0, entry->handle);
    put_le16(buf + 2, entry->end_group_handle);

    /* Only include the service UUID if it has a 16-bit representation. */
    uuid16 = ble_uuid_u16(entry->svc->uuid);
    if (uuid16 != 0) {
        buf = os_mbuf_extend(*om, 2);
        if (buf == NULL) {
            return BLE_ATT_ERR_INSUFFICIENT_RES;
        }
        put_le16(buf, uuid16);
    }

    return 0;
}

static uint16_t
ble_gatts_chr_clt_cfg_allowed(const struct ble_gatt_chr_def *chr)
{
    uint16_t flags;

    flags = 0;
    if (chr->flags & BLE_GATT_CHR_F_NOTIFY) {
        flags |= BLE_GATTS_CLT_CFG_F_NOTIFY;
    }
    if (chr->flags & BLE_GATT_CHR_F_INDICATE) {
        flags |= BLE_GATTS_CLT_CFG_F_INDICATE;
    }

    return flags;
}

static uint8_t
ble_gatts_att_flags_from_chr_flags(ble_gatt_chr_flags chr_flags)
{
    uint8_t att_flags;

    att_flags = 0;
    if (chr_flags & BLE_GATT_CHR_F_READ) {
        att_flags |= BLE_ATT_F_READ;
    }
    if (chr_flags & (BLE_GATT_CHR_F_WRITE_NO_RSP | BLE_GATT_CHR_F_WRITE)) {
        att_flags |= BLE_ATT_F_WRITE;
    }
    if (chr_flags & BLE_GATT_CHR_F_READ_ENC) {
        att_flags |= BLE_ATT_F_READ_ENC;
    }
    if (chr_flags & BLE_GATT_CHR_F_READ_AUTHEN) {
        att_flags |= BLE_ATT_F_READ_AUTHEN;
    }
    if (chr_flags & BLE_GATT_CHR_F_READ_AUTHOR) {
        att_flags |= BLE_ATT_F_READ_AUTHOR;
    }
    if (chr_flags & BLE_GATT_CHR_F_WRITE_ENC) {
        att_flags |= BLE_ATT_F_WRITE_ENC;
    }
    if (chr_flags & BLE_GATT_CHR_F_WRITE_AUTHEN) {
        att_flags |= BLE_ATT_F_WRITE_AUTHEN;
    }
    if (chr_flags & BLE_GATT_CHR_F_WRITE_AUTHOR) {
        att_flags |= BLE_ATT_F_WRITE_AUTHOR;
    }

    return att_flags;
}

static uint8_t
ble_gatts_chr_properties(const struct ble_gatt_chr_def *chr)
{
    uint8_t properties;

    properties = 0;

    if (chr->flags & BLE_GATT_CHR_F_BROADCAST) {
        properties |= BLE_GATT_CHR_PROP_BROADCAST;
    }
    if (chr->flags & BLE_GATT_CHR_F_READ) {
        properties |= BLE_GATT_CHR_PROP_READ;
    }
    if (chr->flags & BLE_GATT_CHR_F_WRITE_NO_RSP) {
        properties |= BLE_GATT_CHR_PROP_WRITE_NO_RSP;
    }
    if (chr->flags & BLE_GATT_CHR_F_WRITE) {
        properties |= BLE_GATT_CHR_PROP_WRITE;
    }
    if (chr->flags & BLE_GATT_CHR_F_NOTIFY) {
        properties |= BLE_GATT_CHR_PROP_NOTIFY;
    }
    if (chr->flags & BLE_GATT_CHR_F_INDICATE) {
        properties |= BLE_GATT_CHR_PROP_INDICATE;
    }
    if (chr->flags & BLE_GATT_CHR_F_AUTH_SIGN_WRITE) {
        properties |= BLE_GATT_CHR_PROP_AUTH_SIGN_WRITE;
    }
    if (chr->flags &
        (BLE_GATT_CHR_F_RELIABLE_WRITE | BLE_GATT_CHR_F_AUX_WRITE)) {

        properties |= BLE_GATT_CHR_PROP_EXTENDED;
    }

    return properties;
}

static int
ble_gatts_chr_def_access(uint16_t conn_handle, uint16_t attr_handle,
                         uint8_t op, uint16_t offset, struct os_mbuf **om,
                         void *arg)
{
    const struct ble_gatt_chr_def *chr;
    uint8_t *buf;

    STATS_INC(ble_gatts_stats, chr_def_reads);

    BLE_HS_DBG_ASSERT(op == BLE_ATT_ACCESS_OP_READ);

    chr = arg;

    buf = os_mbuf_extend(*om, 3);
    if (buf == NULL) {
        return BLE_ATT_ERR_INSUFFICIENT_RES;
    }

    buf[0] = ble_gatts_chr_properties(chr);

    /* The value attribute is always immediately after the declaration. */
    put_le16(buf + 1, attr_handle + 1);

    buf = os_mbuf_extend(*om, ble_uuid_length(chr->uuid));
    if (buf == NULL) {
        return BLE_ATT_ERR_INSUFFICIENT_RES;
    }

    ble_uuid_flat(chr->uuid, buf);

    return 0;
}

static int
ble_gatts_chr_is_sane(const struct ble_gatt_chr_def *chr)
{
    if (chr->uuid == NULL) {
        return 0;
    }

    if (chr->access_cb == NULL) {
        return 0;
    }

    /* XXX: Check properties. */

    return 1;
}

static uint8_t
ble_gatts_chr_op(uint8_t att_op)
{
    switch (att_op) {
    case BLE_ATT_ACCESS_OP_READ:
        return BLE_GATT_ACCESS_OP_READ_CHR;

    case BLE_ATT_ACCESS_OP_WRITE:
        return BLE_GATT_ACCESS_OP_WRITE_CHR;

    default:
        BLE_HS_DBG_ASSERT(0);
        return BLE_GATT_ACCESS_OP_READ_CHR;
    }
}

static void
ble_gatts_chr_inc_val_stat(uint8_t gatt_op)
{
    switch (gatt_op) {
    case BLE_GATT_ACCESS_OP_READ_CHR:
        STATS_INC(ble_gatts_stats, chr_val_reads);
        break;

    case BLE_GATT_ACCESS_OP_WRITE_CHR:
        STATS_INC(ble_gatts_stats, chr_val_writes);
        break;

    default:
        break;
    }
}

/**
 * Indicates whether the set of registered services can be modified.  The
 * service set is mutable if:
 *     o No peers are connected, and
 *     o No GAP operations are active (advertise, discover, or connect).
 *
 * @return                      true if the GATT service set can be modified;
 *                              false otherwise.
 */
static bool
ble_gatts_mutable(void)
{
    /* Ensure no active GAP procedures. */
    if (ble_gap_adv_active() ||
        ble_gap_disc_active() ||
        ble_gap_conn_active()) {

        return false;
    }

    /* Ensure no established connections. */
    if (ble_hs_conn_first() != NULL) {
        return false;
    }

    return true;
}

static int
ble_gatts_val_access(uint16_t conn_handle, uint16_t attr_handle,
                     uint16_t offset, struct ble_gatt_access_ctxt *gatt_ctxt,
                     struct os_mbuf **om, ble_gatt_access_fn *access_cb,
                     void *cb_arg)
{
    uint16_t initial_len;
    int attr_len;
    int new_om;
    int rc;

    switch (gatt_ctxt->op) {
    case BLE_GATT_ACCESS_OP_READ_CHR:
    case BLE_GATT_ACCESS_OP_READ_DSC:
        /* A characteristic value is being read.
         *
         * If the read specifies an offset of 0:
         *     just append the characteristic value directly onto the response
         *     mbuf.
         *
         * Else:
         *     allocate a new mbuf to hold the characteristic data, then append
         *     the requested portion onto the response mbuf.
         */
        if (offset == 0) {
            new_om = 0;
            gatt_ctxt->om = *om;
        } else {
            new_om = 1;
            gatt_ctxt->om = os_msys_get_pkthdr(0, 0);
            if (gatt_ctxt->om == NULL) {
                return BLE_ATT_ERR_INSUFFICIENT_RES;
            }
        }

        initial_len = OS_MBUF_PKTLEN(gatt_ctxt->om);
        rc = access_cb(conn_handle, attr_handle, gatt_ctxt, cb_arg);
        if (rc == 0) {
            attr_len = OS_MBUF_PKTLEN(gatt_ctxt->om) - initial_len - offset;
            if (attr_len >= 0) {
                if (new_om) {
                    os_mbuf_appendfrom(*om, gatt_ctxt->om, offset, attr_len);
                }
            } else {
                rc = BLE_ATT_ERR_INVALID_OFFSET;
            }
        }

        if (new_om) {
            os_mbuf_free_chain(gatt_ctxt->om);
        }
        return rc;

    case BLE_GATT_ACCESS_OP_WRITE_CHR:
    case BLE_GATT_ACCESS_OP_WRITE_DSC:
        gatt_ctxt->om = *om;
        rc = access_cb(conn_handle, attr_handle, gatt_ctxt, cb_arg);
        *om = gatt_ctxt->om;
        return rc;

    default:
        BLE_HS_DBG_ASSERT(0);
        return BLE_ATT_ERR_UNLIKELY;
    }
}

static int
ble_gatts_chr_val_access(uint16_t conn_handle, uint16_t attr_handle,
                         uint8_t att_op, uint16_t offset,
                         struct os_mbuf **om, void *arg)
{
    const struct ble_gatt_chr_def *chr_def;
    struct ble_gatt_access_ctxt gatt_ctxt;
    int rc;

    chr_def = arg;
    BLE_HS_DBG_ASSERT(chr_def != NULL && chr_def->access_cb != NULL);

    gatt_ctxt.op = ble_gatts_chr_op(att_op);
    gatt_ctxt.chr = chr_def;

    ble_gatts_chr_inc_val_stat(gatt_ctxt.op);
    rc = ble_gatts_val_access(conn_handle, attr_handle, offset, &gatt_ctxt, om,
                              chr_def->access_cb, chr_def->arg);

    return rc;
}

static int
ble_gatts_find_svc_entry_idx(const struct ble_gatt_svc_def *svc)
{
    int i;

    for (i = 0; i < ble_gatts_num_svc_entries; i++) {
        if (ble_gatts_svc_entries[i].svc == svc) {
            return i;
        }
    }

    return -1;
}

static int
ble_gatts_svc_incs_satisfied(const struct ble_gatt_svc_def *svc)
{
    int idx;
    int i;

    if (svc->includes == NULL) {
        /* No included services. */
        return 1;
    }

    for (i = 0; svc->includes[i] != NULL; i++) {
        idx = ble_gatts_find_svc_entry_idx(svc->includes[i]);
        if (idx == -1 || ble_gatts_svc_entries[idx].handle == 0) {
            return 0;
        }
    }

    return 1;
}

static int
ble_gatts_register_inc(struct ble_gatts_svc_entry *entry)
{
    uint16_t handle;
    int rc;

    BLE_HS_DBG_ASSERT(entry->handle != 0);
    BLE_HS_DBG_ASSERT(entry->end_group_handle != 0xffff);

    rc = ble_att_svr_register(uuid_inc, BLE_ATT_F_READ, 0, &handle,
                              ble_gatts_inc_access, entry);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

static uint8_t
ble_gatts_dsc_op(uint8_t att_op)
{
    switch (att_op) {
    case BLE_ATT_ACCESS_OP_READ:
        return BLE_GATT_ACCESS_OP_READ_DSC;

    case BLE_ATT_ACCESS_OP_WRITE:
        return BLE_GATT_ACCESS_OP_WRITE_DSC;

    default:
        BLE_HS_DBG_ASSERT(0);
        return BLE_GATT_ACCESS_OP_READ_DSC;
    }
}

static void
ble_gatts_dsc_inc_stat(uint8_t gatt_op)
{
    switch (gatt_op) {
    case BLE_GATT_ACCESS_OP_READ_DSC:
        STATS_INC(ble_gatts_stats, dsc_reads);
        break;

    case BLE_GATT_ACCESS_OP_WRITE_DSC:
        STATS_INC(ble_gatts_stats, dsc_writes);
        break;

    default:
        break;
    }
}

static int
ble_gatts_dsc_access(uint16_t conn_handle, uint16_t attr_handle,
                     uint8_t att_op, uint16_t offset, struct os_mbuf **om,
                     void *arg)
{
    const struct ble_gatt_dsc_def *dsc_def;
    struct ble_gatt_access_ctxt gatt_ctxt;
    int rc;

    dsc_def = arg;
    BLE_HS_DBG_ASSERT(dsc_def != NULL && dsc_def->access_cb != NULL);

    gatt_ctxt.op = ble_gatts_dsc_op(att_op);
    gatt_ctxt.dsc = dsc_def;

    ble_gatts_dsc_inc_stat(gatt_ctxt.op);
    rc = ble_gatts_val_access(conn_handle, attr_handle, offset, &gatt_ctxt, om,
                              dsc_def->access_cb, dsc_def->arg);

    return rc;
}

static int
ble_gatts_dsc_is_sane(const struct ble_gatt_dsc_def *dsc)
{
    if (dsc->uuid == NULL) {
        return 0;
    }

    if (dsc->access_cb == NULL) {
        return 0;
    }

    return 1;
}

static int
ble_gatts_register_dsc(const struct ble_gatt_svc_def *svc,
                       const struct ble_gatt_chr_def *chr,
                       const struct ble_gatt_dsc_def *dsc,
                       uint16_t chr_def_handle,
                       ble_gatt_register_fn *register_cb, void *cb_arg)
{
    struct ble_gatt_register_ctxt register_ctxt;
    uint16_t dsc_handle;
    int rc;

    if (!ble_gatts_dsc_is_sane(dsc)) {
        return BLE_HS_EINVAL;
    }

    rc = ble_att_svr_register(dsc->uuid, dsc->att_flags, dsc->min_key_size,
                              &dsc_handle, ble_gatts_dsc_access, (void *)dsc);
    if (rc != 0) {
        return rc;
    }

    if (register_cb != NULL) {
        register_ctxt.op = BLE_GATT_REGISTER_OP_DSC;
        register_ctxt.dsc.handle = dsc_handle;
        register_ctxt.dsc.svc_def = svc;
        register_ctxt.dsc.chr_def = chr;
        register_ctxt.dsc.dsc_def = dsc;
        register_cb(&register_ctxt, cb_arg);
    }

    STATS_INC(ble_gatts_stats, dscs);

    return 0;

}

static int
ble_gatts_clt_cfg_find_idx(struct ble_gatts_clt_cfg *cfgs,
                           uint16_t chr_val_handle)
{
    struct ble_gatts_clt_cfg *cfg;
    int i;

    for (i = 0; i < ble_gatts_num_cfgable_chrs; i++) {
        cfg = cfgs + i;
        if (cfg->chr_val_handle == chr_val_handle) {
            return i;
        }
    }

    return -1;
}

static struct ble_gatts_clt_cfg *
ble_gatts_clt_cfg_find(struct ble_gatts_clt_cfg *cfgs,
                       uint16_t chr_val_handle)
{
    int idx;

    idx = ble_gatts_clt_cfg_find_idx(cfgs, chr_val_handle);
    if (idx == -1) {
        return NULL;
    } else {
        return cfgs + idx;
    }
}

static void
ble_gatts_subscribe_event(uint16_t conn_handle, uint16_t attr_handle,
                          uint8_t reason,
                          uint8_t prev_flags, uint8_t cur_flags)
{
    if ((prev_flags ^ cur_flags) & ~BLE_GATTS_CLT_CFG_F_RESERVED) {
        ble_gap_subscribe_event(conn_handle,
                                attr_handle,
                                reason,
                                prev_flags  & BLE_GATTS_CLT_CFG_F_NOTIFY,
                                cur_flags   & BLE_GATTS_CLT_CFG_F_NOTIFY,
                                prev_flags  & BLE_GATTS_CLT_CFG_F_INDICATE,
                                cur_flags   & BLE_GATTS_CLT_CFG_F_INDICATE);
    }
}

/**
 * Performs a read or write access on a client characteritic configuration
 * descriptor (CCCD).
 *
 * @param conn                  The connection of the peer doing the accessing.
 * @apram attr_handle           The handle of the CCCD.
 * @param att_op                The ATT operation being performed (read or
 *                                  write).
 * @param ctxt                  Communication channel between this function and
 *                                  the caller within the nimble stack.
 *                                  Semantics depends on the operation being
 *                                  performed.
 * @param out_cccd              If the CCCD should be persisted as a result of
 *                                  the access, the data-to-be-persisted gets
 *                                  written here.  If no persistence is
 *                                  necessary, out_cccd->chr_val_handle is set
 *                                  to 0.
 *
 * @return                      0 on success; nonzero on failure.
 */
static int
ble_gatts_clt_cfg_access_locked(struct ble_hs_conn *conn, uint16_t attr_handle,
                                uint8_t att_op, uint16_t offset,
                                struct os_mbuf *om,
                                struct ble_store_value_cccd *out_cccd,
                                uint8_t *out_prev_clt_cfg_flags,
                                uint8_t *out_cur_clt_cfg_flags)
{
    struct ble_gatts_clt_cfg *clt_cfg;
    uint16_t chr_val_handle;
    uint16_t flags;
    uint8_t gatt_op;
    uint8_t *buf;

    /* Assume nothing needs to be persisted. */
    out_cccd->chr_val_handle = 0;

    /* We always register the client characteristics descriptor with handle
     * (chr_val + 1).
     */
    chr_val_handle = attr_handle - 1;
    if (chr_val_handle > attr_handle) {
        /* Attribute handle wrapped somehow. */
        return BLE_ATT_ERR_UNLIKELY;
    }

    clt_cfg = ble_gatts_clt_cfg_find(conn->bhc_gatt_svr.clt_cfgs,
                                     chr_val_handle);
    if (clt_cfg == NULL) {
        return BLE_ATT_ERR_UNLIKELY;
    }

    /* Assume no change in flags. */
    *out_prev_clt_cfg_flags = clt_cfg->flags;
    *out_cur_clt_cfg_flags = clt_cfg->flags;

    gatt_op = ble_gatts_dsc_op(att_op);
    ble_gatts_dsc_inc_stat(gatt_op);

    switch (gatt_op) {
    case BLE_GATT_ACCESS_OP_READ_DSC:
        STATS_INC(ble_gatts_stats, dsc_reads);
        buf = os_mbuf_extend(om, 2);
        if (buf == NULL) {
            return BLE_ATT_ERR_INSUFFICIENT_RES;
        }
        put_le16(buf, clt_cfg->flags & ~BLE_GATTS_CLT_CFG_F_RESERVED);
        break;

    case BLE_GATT_ACCESS_OP_WRITE_DSC:
        STATS_INC(ble_gatts_stats, dsc_writes);
        if (OS_MBUF_PKTLEN(om) != 2) {
            return BLE_ATT_ERR_INVALID_ATTR_VALUE_LEN;
        }

        om = os_mbuf_pullup(om, 2);
        BLE_HS_DBG_ASSERT(om != NULL);

        flags = get_le16(om->om_data);
        if ((flags & ~clt_cfg->allowed) != 0) {
            return BLE_ATT_ERR_REQ_NOT_SUPPORTED;
        }

        if (clt_cfg->flags != flags) {
            clt_cfg->flags = flags;
            *out_cur_clt_cfg_flags = flags;

            /* Successful writes get persisted for bonded connections. */
            if (conn->bhc_sec_state.bonded) {
                out_cccd->peer_addr = conn->bhc_peer_addr;
                out_cccd->chr_val_handle = chr_val_handle;
                out_cccd->flags = clt_cfg->flags;
                out_cccd->value_changed = 0;
            }
        }
        break;

    default:
        BLE_HS_DBG_ASSERT(0);
        return BLE_ATT_ERR_UNLIKELY;
    }

    return 0;
}

int
ble_gatts_clt_cfg_access(uint16_t conn_handle, uint16_t attr_handle,
                         uint8_t op, uint16_t offset, struct os_mbuf **om,
                         void *arg)
{
    struct ble_store_value_cccd cccd_value;
    struct ble_store_key_cccd cccd_key;
    struct ble_hs_conn *conn;
    uint16_t chr_val_handle;
    uint8_t prev_flags;
    uint8_t cur_flags;
    int rc;

    ble_hs_lock();

    conn = ble_hs_conn_find(conn_handle);
    if (conn == NULL) {
        rc = BLE_ATT_ERR_UNLIKELY;
    } else {
        rc = ble_gatts_clt_cfg_access_locked(conn, attr_handle, op, offset,
                                             *om, &cccd_value, &prev_flags,
                                             &cur_flags);
    }

    ble_hs_unlock();

    if (rc != 0) {
        return rc;
    }

    /* The value attribute is always immediately after the declaration. */
    chr_val_handle = attr_handle - 1;

    /* Tell the application if the peer changed its subscription state. */
    ble_gatts_subscribe_event(conn_handle, chr_val_handle,
                              BLE_GAP_SUBSCRIBE_REASON_WRITE,
                              prev_flags, cur_flags);

    /* Persist the CCCD if required. */
    if (cccd_value.chr_val_handle != 0) {
        if (cccd_value.flags == 0) {
            ble_store_key_from_value_cccd(&cccd_key, &cccd_value);
            rc = ble_store_delete_cccd(&cccd_key);
        } else {
            rc = ble_store_write_cccd(&cccd_value);
        }
    }

    return rc;
}

static int
ble_gatts_register_clt_cfg_dsc(uint16_t *att_handle)
{
    int rc;

    rc = ble_att_svr_register(uuid_ccc, BLE_ATT_F_READ | BLE_ATT_F_WRITE, 0,
                              att_handle, ble_gatts_clt_cfg_access, NULL);
    if (rc != 0) {
        return rc;
    }

    STATS_INC(ble_gatts_stats, dscs);

    return 0;
}

static int
ble_gatts_register_chr(const struct ble_gatt_svc_def *svc,
                       const struct ble_gatt_chr_def *chr,
                       ble_gatt_register_fn *register_cb, void *cb_arg)
{
    struct ble_gatt_register_ctxt register_ctxt;
    struct ble_gatt_dsc_def *dsc;
    uint16_t def_handle;
    uint16_t val_handle;
    uint16_t dsc_handle;
    uint8_t att_flags;
    int rc;

    if (!ble_gatts_chr_is_sane(chr)) {
        return BLE_HS_EINVAL;
    }

    if (ble_gatts_chr_clt_cfg_allowed(chr) != 0) {
        if (ble_gatts_num_cfgable_chrs > ble_hs_max_client_configs) {
            return BLE_HS_ENOMEM;
        }
        ble_gatts_num_cfgable_chrs++;
    }

    /* Register characteristic definition attribute (cast away const on
     * callback arg).
     */
    rc = ble_att_svr_register(uuid_chr, BLE_ATT_F_READ, 0, &def_handle,
                              ble_gatts_chr_def_access, (void *)chr);
    if (rc != 0) {
        return rc;
    }

    /* Register characteristic value attribute (cast away const on callback
     * arg).
     */
    att_flags = ble_gatts_att_flags_from_chr_flags(chr->flags);
    rc = ble_att_svr_register(chr->uuid, att_flags, chr->min_key_size,
                              &val_handle, ble_gatts_chr_val_access,
                              (void *)chr);
    if (rc != 0) {
        return rc;
    }
    BLE_HS_DBG_ASSERT(val_handle == def_handle + 1);

    if (chr->val_handle != NULL) {
        *chr->val_handle = val_handle;
    }

    if (register_cb != NULL) {
        register_ctxt.op = BLE_GATT_REGISTER_OP_CHR;
        register_ctxt.chr.def_handle = def_handle;
        register_ctxt.chr.val_handle = val_handle;
        register_ctxt.chr.svc_def = svc;
        register_ctxt.chr.chr_def = chr;
        register_cb(&register_ctxt, cb_arg);
    }

    if (ble_gatts_chr_clt_cfg_allowed(chr) != 0) {
        rc = ble_gatts_register_clt_cfg_dsc(&dsc_handle);
        if (rc != 0) {
            return rc;
        }
        BLE_HS_DBG_ASSERT(dsc_handle == def_handle + 2);
    }

    /* Register each descriptor. */
    if (chr->descriptors != NULL) {
        for (dsc = chr->descriptors; dsc->uuid != NULL; dsc++) {
            rc = ble_gatts_register_dsc(svc, chr, dsc, def_handle, register_cb,
                                        cb_arg);
            if (rc != 0) {
                return rc;
            }
        }
    }

    STATS_INC(ble_gatts_stats, chrs);

    return 0;
}

static int
ble_gatts_svc_type_to_uuid(uint8_t svc_type, const ble_uuid_t **uuid)
{
    switch (svc_type) {
    case BLE_GATT_SVC_TYPE_PRIMARY:
        *uuid = uuid_pri;
        return 0;

    case BLE_GATT_SVC_TYPE_SECONDARY:
        *uuid = uuid_sec;
        return 0;

    default:
        return BLE_HS_EINVAL;
    }
}

static int
ble_gatts_svc_is_sane(const struct ble_gatt_svc_def *svc)
{
    if (svc->type != BLE_GATT_SVC_TYPE_PRIMARY &&
        svc->type != BLE_GATT_SVC_TYPE_SECONDARY) {

        return 0;
    }

    if (svc->uuid == NULL) {
        return 0;
    }

    return 1;
}

static int
ble_gatts_register_svc(const struct ble_gatt_svc_def *svc,
                       uint16_t *out_handle,
                       ble_gatt_register_fn *register_cb, void *cb_arg)
{
    const struct ble_gatt_chr_def *chr;
    struct ble_gatt_register_ctxt register_ctxt;
    const ble_uuid_t *uuid;
    int idx;
    int rc;
    int i;

    if (!ble_gatts_svc_incs_satisfied(svc)) {
        return BLE_HS_EAGAIN;
    }

    if (!ble_gatts_svc_is_sane(svc)) {
        return BLE_HS_EINVAL;
    }

    /* Prevent spurious maybe-uninitialized gcc warning. */
    uuid = NULL;

    rc = ble_gatts_svc_type_to_uuid(svc->type, &uuid);
    BLE_HS_DBG_ASSERT_EVAL(rc == 0);

    /* Register service definition attribute (cast away const on callback
     * arg).
     */
    rc = ble_att_svr_register(uuid, BLE_ATT_F_READ, 0, out_handle,
                              ble_gatts_svc_access, (void *)svc);
    if (rc != 0) {
        return rc;
    }

    if (register_cb != NULL) {
        register_ctxt.op = BLE_GATT_REGISTER_OP_SVC;
        register_ctxt.svc.handle = *out_handle;
        register_ctxt.svc.svc_def = svc;
        register_cb(&register_ctxt, cb_arg);
    }

    /* Register each include. */
    if (svc->includes != NULL) {
        for (i = 0; svc->includes[i] != NULL; i++) {
            idx = ble_gatts_find_svc_entry_idx(svc->includes[i]);
            BLE_HS_DBG_ASSERT_EVAL(idx != -1);

            rc = ble_gatts_register_inc(ble_gatts_svc_entries + idx);
            if (rc != 0) {
                return rc;
            }
        }
    }

    /* Register each characteristic. */
    if (svc->characteristics != NULL) {
        for (chr = svc->characteristics; chr->uuid != NULL; chr++) {
            rc = ble_gatts_register_chr(svc, chr, register_cb, cb_arg);
            if (rc != 0) {
                return rc;
            }
        }
    }

    STATS_INC(ble_gatts_stats, svcs);

    return 0;
}

static int
ble_gatts_register_round(int *out_num_registered, ble_gatt_register_fn *cb,
                         void *cb_arg)
{
    struct ble_gatts_svc_entry *entry;
    uint16_t handle;
    int rc;
    int i;

    *out_num_registered = 0;
    for (i = 0; i < ble_gatts_num_svc_entries; i++) {
        entry = ble_gatts_svc_entries + i;

        if (entry->handle == 0) {
            rc = ble_gatts_register_svc(entry->svc, &handle, cb, cb_arg);
            switch (rc) {
            case 0:
                /* Service successfully registered. */
                entry->handle = handle;
                entry->end_group_handle = ble_att_svr_prev_handle();
                (*out_num_registered)++;
                break;

            case BLE_HS_EAGAIN:
                /* Service could not be registered due to unsatisfied includes.
                 * Try again on the next iteration.
                 */
                break;

            default:
                return rc;
            }
        }
    }

    if (*out_num_registered == 0) {
        /* There is a circular dependency. */
        return BLE_HS_EINVAL;
    }

    return 0;
}

/**
 * Registers a set of services, characteristics, and descriptors to be accessed
 * by GATT clients.
 *
 * @param svcs                  A table of the service definitions to be
 *                                  registered.
 * @param cb                    The function to call for each service,
 *                                  characteristic, and descriptor that gets
 *                                  registered.
 * @param cb_arg                The optional argument to pass to the callback
 *                                  function.
 *
 * @return                      0 on success;
 *                              BLE_HS_ENOMEM if registration failed due to
 *                                  resource exhaustion;
 *                              BLE_HS_EINVAL if the service definition table
 *                                  contains an invalid element.
 */
int
ble_gatts_register_svcs(const struct ble_gatt_svc_def *svcs,
                        ble_gatt_register_fn *cb, void *cb_arg)
{
    int total_registered;
    int cur_registered;
    int num_svcs;
    int idx;
    int rc;
    int i;

    for (i = 0; svcs[i].type != BLE_GATT_SVC_TYPE_END; i++) {
        idx = ble_gatts_num_svc_entries + i;
        if (idx >= ble_hs_max_services) {
            return BLE_HS_ENOMEM;
        }

        ble_gatts_svc_entries[idx].svc = svcs + i;
        ble_gatts_svc_entries[idx].handle = 0;
        ble_gatts_svc_entries[idx].end_group_handle = 0xffff;
    }
    num_svcs = i;
    ble_gatts_num_svc_entries += num_svcs;

    total_registered = 0;
    while (total_registered < num_svcs) {
        rc = ble_gatts_register_round(&cur_registered, cb, cb_arg);
        if (rc != 0) {
            return rc;
        }
        total_registered += cur_registered;
    }

    return 0;
}

static int
ble_gatts_clt_cfg_size(void)
{
    return ble_gatts_num_cfgable_chrs * sizeof (struct ble_gatts_clt_cfg);
}

/**
 * Handles GATT server clean up for a terminated connection:
 *     o Informs the application that the peer is no longer subscribed to any
 *       characteristic updates.
 *     o Frees GATT server resources consumed by the connection (CCCDs).
 */
void
ble_gatts_connection_broken(uint16_t conn_handle)
{
    struct ble_gatts_clt_cfg *clt_cfgs;
    struct ble_hs_conn *conn;
    int num_clt_cfgs;
    int rc;
    int i;

    /* Find the specified connection and extract its CCCD entries.  Extracting
     * the clt_cfg pointer and setting the original to null is done for two
     * reasons:
     *     1. So that the CCCD entries can be safely processed after unlocking
     *        the mutex.
     *     2. To ensure a subsequent indicate procedure for this peer is not
     *        attempted, as the connection is about to be terminated.  This
     *        avoids a spurious notify-tx GAP event callback to the
     *        application.  By setting the clt_cfg pointer to null, it is
     *        assured that the connection has no pending indications to send.
     */
    ble_hs_lock();
    conn = ble_hs_conn_find(conn_handle);
    if (conn != NULL) {
        clt_cfgs = conn->bhc_gatt_svr.clt_cfgs;
        num_clt_cfgs = conn->bhc_gatt_svr.num_clt_cfgs;

        conn->bhc_gatt_svr.clt_cfgs = NULL;
        conn->bhc_gatt_svr.num_clt_cfgs = 0;
    }
    ble_hs_unlock();

    if (conn == NULL) {
        return;
    }

    /* If there is an indicate procedure in progress for this connection,
     * inform the application that it has failed.
     */
    ble_gatts_indicate_fail_notconn(conn_handle);

    /* Now that the mutex is unlocked, inform the application that the peer is
     * no longer subscribed to any characteristic updates.
     */
    if (clt_cfgs != NULL) {
        for (i = 0; i < num_clt_cfgs; i++) {
            ble_gatts_subscribe_event(conn_handle, clt_cfgs[i].chr_val_handle,
                                      BLE_GAP_SUBSCRIBE_REASON_TERM,
                                      clt_cfgs[i].flags, 0);
        }

        rc = os_memblock_put(&ble_gatts_clt_cfg_pool, clt_cfgs);
        BLE_HS_DBG_ASSERT_EVAL(rc == 0);
    }
}

static void
ble_gatts_free_svc_defs(void)
{
    nimble_platform_mem_free(ble_gatts_svc_defs);
    ble_gatts_svc_defs = NULL;
    ble_gatts_num_svc_defs = 0;
}

static void
ble_gatts_free_mem(void)
{
    nimble_platform_mem_free(ble_gatts_clt_cfg_mem);
    ble_gatts_clt_cfg_mem = NULL;

    nimble_platform_mem_free(ble_gatts_svc_entries);
    ble_gatts_svc_entries = NULL;
}


void
ble_gatts_stop(void)
{

    ble_hs_max_services = 0;
    ble_hs_max_attrs = 0;
    ble_hs_max_client_configs = 0;

    ble_gatts_free_mem();
    ble_gatts_free_svc_defs();
    ble_att_svr_stop();
}

int
ble_gatts_start(void)
{
    struct ble_att_svr_entry *ha;
    struct ble_gatt_chr_def *chr;
    uint16_t allowed_flags;
    ble_uuid16_t uuid = BLE_UUID16_INIT(BLE_ATT_UUID_CHARACTERISTIC);
    int num_elems;
    int idx;
    int rc;
    int i;

    ble_hs_lock();
    if (!ble_gatts_mutable()) {
        rc = BLE_HS_EBUSY;
        goto done;
    }

    ble_gatts_free_mem();

    rc = ble_att_svr_start();
    if (rc != 0) {
        goto done;
    }

    if (ble_hs_max_client_configs > 0) {
        ble_gatts_clt_cfg_mem = nimble_platform_mem_malloc(
            OS_MEMPOOL_BYTES(ble_hs_max_client_configs,
                             sizeof (struct ble_gatts_clt_cfg)));
        if (ble_gatts_clt_cfg_mem == NULL) {
            rc = BLE_HS_ENOMEM;
            goto done;
        }
    }

    if (ble_hs_max_services > 0) {
        ble_gatts_svc_entries =
            nimble_platform_mem_malloc(ble_hs_max_services * sizeof *ble_gatts_svc_entries);
        if (ble_gatts_svc_entries == NULL) {
            rc = BLE_HS_ENOMEM;
            goto done;
        }
    }


    ble_gatts_num_svc_entries = 0;
    for (i = 0; i < ble_gatts_num_svc_defs; i++) {
        rc = ble_gatts_register_svcs(ble_gatts_svc_defs[i],
                                     ble_hs_cfg.gatts_register_cb,
                                     ble_hs_cfg.gatts_register_arg);
        if (rc != 0) {
            goto done;
        }
    }
    ble_gatts_free_svc_defs();

    if (ble_gatts_num_cfgable_chrs == 0) {
        rc = 0;
        goto done;
    }

    /* Initialize client-configuration memory pool. */
    num_elems = ble_hs_max_client_configs / ble_gatts_num_cfgable_chrs;
    rc = os_mempool_init(&ble_gatts_clt_cfg_pool, num_elems,
                         ble_gatts_clt_cfg_size(), ble_gatts_clt_cfg_mem,
                         "ble_gatts_clt_cfg_pool");
    if (rc != 0) {
        rc = BLE_HS_EOS;
        goto done;
    }

    /* Allocate the cached array of handles for the configuration
     * characteristics.
     */
    ble_gatts_clt_cfgs = os_memblock_get(&ble_gatts_clt_cfg_pool);
    if (ble_gatts_clt_cfgs == NULL) {
        rc = BLE_HS_ENOMEM;
        goto done;
    }

    /* Fill the cache. */
    idx = 0;
    ha = NULL;
    while ((ha = ble_att_svr_find_by_uuid(ha, &uuid.u, 0xffff)) != NULL) {
        chr = ha->ha_cb_arg;
        allowed_flags = ble_gatts_chr_clt_cfg_allowed(chr);
        if (allowed_flags != 0) {
            BLE_HS_DBG_ASSERT_EVAL(idx < ble_gatts_num_cfgable_chrs);

            ble_gatts_clt_cfgs[idx].chr_val_handle = ha->ha_handle_id + 1;
            ble_gatts_clt_cfgs[idx].allowed = allowed_flags;
            ble_gatts_clt_cfgs[idx].flags = 0;
            idx++;
        }
    }

done:
    if (rc != 0) {
        ble_gatts_free_mem();
        ble_gatts_free_svc_defs();
    }

    ble_hs_unlock();
    return rc;
}

int
ble_gatts_conn_can_alloc(void)
{
    return ble_gatts_num_cfgable_chrs == 0 ||
           ble_gatts_clt_cfg_pool.mp_num_free > 0;
}

int
ble_gatts_conn_init(struct ble_gatts_conn *gatts_conn)
{
    if (ble_gatts_num_cfgable_chrs > 0) {
        gatts_conn->clt_cfgs = os_memblock_get(&ble_gatts_clt_cfg_pool);
        if (gatts_conn->clt_cfgs == NULL) {
            return BLE_HS_ENOMEM;
        }

        /* Initialize the client configuration with a copy of the cache. */
        memcpy(gatts_conn->clt_cfgs, ble_gatts_clt_cfgs,
               ble_gatts_clt_cfg_size());
        gatts_conn->num_clt_cfgs = ble_gatts_num_cfgable_chrs;
    } else {
        gatts_conn->clt_cfgs = NULL;
        gatts_conn->num_clt_cfgs = 0;
    }

    return 0;
}


/**
 * Schedules a notification or indication for the specified peer-CCCD pair.  If
 * the update should be sent immediately, it is indicated in the return code.
 *
 * @param conn                  The connection to schedule the update for.
 * @param clt_cfg               The client config entry corresponding to the
 *                                  peer and affected characteristic.
 *
 * @return                      The att_op of the update to send immediately,
 *                                  if any.  0 if nothing should get sent.
 */
static uint8_t
ble_gatts_schedule_update(struct ble_hs_conn *conn,
                          struct ble_gatts_clt_cfg *clt_cfg)
{
    uint8_t att_op;

    if (!(clt_cfg->flags & BLE_GATTS_CLT_CFG_F_MODIFIED)) {
        /* Characteristic not modified.  Nothing to send. */
        att_op = 0;
    } else if (clt_cfg->flags & BLE_GATTS_CLT_CFG_F_NOTIFY) {
        /* Notifications always get sent immediately. */
        att_op = BLE_ATT_OP_NOTIFY_REQ;
    } else if (clt_cfg->flags & BLE_GATTS_CLT_CFG_F_INDICATE) {
        /* Only one outstanding indication per peer is allowed.  If we
         * are still awaiting an ack, mark this CCCD as updated so that
         * we know to send the indication upon receiving the expected ack.
         * If there isn't an outstanding indication, send this one now.
         */
        if (conn->bhc_gatt_svr.indicate_val_handle != 0) {
            att_op = 0;
        } else {
            att_op = BLE_ATT_OP_INDICATE_REQ;
        }
    } else {
        /* Peer isn't subscribed to notifications or indications.  Nothing to
         * send.
         */
        att_op = 0;
    }

    /* If we will be sending an update, clear the modified flag so that we
     * don't double-send.
     */
    if (att_op != 0) {
        clt_cfg->flags &= ~BLE_GATTS_CLT_CFG_F_MODIFIED;
    }

    return att_op;
}

int
ble_gatts_send_next_indicate(uint16_t conn_handle)
{
    struct ble_gatts_clt_cfg *clt_cfg;
    struct ble_hs_conn *conn;
    uint16_t chr_val_handle;
    int rc;
    int i;

    /* Assume no pending indications. */
    chr_val_handle = 0;

    ble_hs_lock();

    conn = ble_hs_conn_find(conn_handle);
    if (conn != NULL) {
        for (i = 0; i < conn->bhc_gatt_svr.num_clt_cfgs; i++) {
            clt_cfg = conn->bhc_gatt_svr.clt_cfgs + i;
            if (clt_cfg->flags & BLE_GATTS_CLT_CFG_F_MODIFIED) {
                BLE_HS_DBG_ASSERT(clt_cfg->flags &
                                  BLE_GATTS_CLT_CFG_F_INDICATE);

                chr_val_handle = clt_cfg->chr_val_handle;

                /* Clear pending flag in anticipation of indication tx. */
                clt_cfg->flags &= ~BLE_GATTS_CLT_CFG_F_MODIFIED;
                break;
            }
        }
    }

    ble_hs_unlock();

    if (conn == NULL) {
        return BLE_HS_ENOTCONN;
    }

    if (chr_val_handle == 0) {
        return BLE_HS_ENOENT;
    }

    rc = ble_gattc_indicate(conn_handle, chr_val_handle);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

int
ble_gatts_rx_indicate_ack(uint16_t conn_handle, uint16_t chr_val_handle)
{
    struct ble_store_value_cccd cccd_value;
    struct ble_gatts_clt_cfg *clt_cfg;
    struct ble_hs_conn *conn;
    int clt_cfg_idx;
    int persist;
    int rc;

    clt_cfg_idx = ble_gatts_clt_cfg_find_idx(ble_gatts_clt_cfgs,
                                             chr_val_handle);
    if (clt_cfg_idx == -1) {
        /* This characteristic does not have a CCCD. */
        return BLE_HS_ENOENT;
    }

    clt_cfg = ble_gatts_clt_cfgs + clt_cfg_idx;
    if (!(clt_cfg->allowed & BLE_GATTS_CLT_CFG_F_INDICATE)) {
        /* This characteristic does not allow indications. */
        return BLE_HS_ENOENT;
    }

    ble_hs_lock();

    conn = ble_hs_conn_find(conn_handle);
    BLE_HS_DBG_ASSERT(conn != NULL);
    if (conn->bhc_gatt_svr.indicate_val_handle == chr_val_handle) {
        /* This acknowledgement is expected. */
        rc = 0;

        /* Mark that there is no longer an outstanding txed indicate. */
        conn->bhc_gatt_svr.indicate_val_handle = 0;

        /* Determine if we need to persist that there is no pending indication
         * for this peer-characteristic pair.  If the characteristic has not
         * been modified since we sent the indication, there is no indication
         * pending.
         */
        BLE_HS_DBG_ASSERT(conn->bhc_gatt_svr.num_clt_cfgs > clt_cfg_idx);
        clt_cfg = conn->bhc_gatt_svr.clt_cfgs + clt_cfg_idx;
        BLE_HS_DBG_ASSERT(clt_cfg->chr_val_handle == chr_val_handle);

        persist = conn->bhc_sec_state.bonded &&
                  !(clt_cfg->flags & BLE_GATTS_CLT_CFG_F_MODIFIED);
        if (persist) {
            cccd_value.peer_addr = conn->bhc_peer_addr;
            cccd_value.chr_val_handle = chr_val_handle;
            cccd_value.flags = clt_cfg->flags;
            cccd_value.value_changed = 0;
        }
    } else {
        /* This acknowledgement doesn't correspond to the outstanding
         * indication; ignore it.
         */
        rc = BLE_HS_ENOENT;
    }

    ble_hs_unlock();

    if (rc != 0) {
        return rc;
    }

    if (persist) {
        rc = ble_store_write_cccd(&cccd_value);
        if (rc != 0) {
            /* XXX: How should this error get reported? */
        }
    }

    return 0;
}

void
ble_gatts_chr_updated(uint16_t chr_val_handle)
{
    struct ble_store_value_cccd cccd_value;
    struct ble_store_key_cccd cccd_key;
    struct ble_gatts_clt_cfg *clt_cfg;
    struct ble_hs_conn *conn;
    int new_notifications = 0;
    int clt_cfg_idx;
    int persist;
    int rc;
    int i;

    /* Determine if notifications or indications are allowed for this
     * characteristic.  If not, return immediately.
     */
    clt_cfg_idx = ble_gatts_clt_cfg_find_idx(ble_gatts_clt_cfgs,
                                             chr_val_handle);
    if (clt_cfg_idx == -1) {
        return;
    }

    /*** Send notifications and indications to connected devices. */

    ble_hs_lock();
    for (i = 0; ; i++) {
        /* XXX: This is inefficient when there are a lot of connections.
         * Consider using a "foreach" function to walk the connection list.
         */
        conn = ble_hs_conn_find_by_idx(i);
        if (conn == NULL) {
            break;
        }

        BLE_HS_DBG_ASSERT_EVAL(conn->bhc_gatt_svr.num_clt_cfgs >
                               clt_cfg_idx);
        clt_cfg = conn->bhc_gatt_svr.clt_cfgs + clt_cfg_idx;
        BLE_HS_DBG_ASSERT_EVAL(clt_cfg->chr_val_handle == chr_val_handle);

        /* Mark the CCCD entry as modified. */
        clt_cfg->flags |= BLE_GATTS_CLT_CFG_F_MODIFIED;
        new_notifications = 1;
    }
    ble_hs_unlock();

    if (new_notifications) {
        ble_hs_notifications_sched();
    }

    /*** Persist updated flag for unconnected and not-yet-bonded devices. */

    /* Retrieve each record corresponding to the modified characteristic. */
    cccd_key.peer_addr = *BLE_ADDR_ANY;
    cccd_key.chr_val_handle = chr_val_handle;
    cccd_key.idx = 0;

    while (1) {
        rc = ble_store_read_cccd(&cccd_key, &cccd_value);
        if (rc != 0) {
            /* Read error or no more CCCD records. */
            break;
        }

        /* Determine if this record needs to be rewritten. */
        ble_hs_lock();
        conn = ble_hs_conn_find_by_addr(&cccd_key.peer_addr);

        if (conn == NULL) {
            /* Device isn't connected; persist the changed flag so that an
             * update can be sent when the device reconnects and rebonds.
             */
            persist = 1;
        } else if (cccd_value.flags & BLE_GATTS_CLT_CFG_F_INDICATE) {
            /* Indication for a connected device; record that the
             * characteristic has changed until we receive the ack.
             */
            persist = 1;
        } else {
            /* Notification for a connected device; we already sent it so there
             * is no need to persist.
             */
            persist = 0;
        }

        ble_hs_unlock();

        /* Only persist if the value changed flag wasn't already sent (i.e.,
         * don't overwrite with identical data).
         */
        if (persist && !cccd_value.value_changed) {
            cccd_value.value_changed = 1;
            ble_store_write_cccd(&cccd_value);
        }

        /* Read the next matching record. */
        cccd_key.idx++;
    }
}

/**
 * Sends notifications or indications for the specified characteristic to all
 * connected devices.  The bluetooth spec does not allow more than one
 * concurrent indication for a single peer, so this function will hold off on
 * sending such indications.
 */
static void
ble_gatts_tx_notifications_one_chr(uint16_t chr_val_handle)
{
    struct ble_gatts_clt_cfg *clt_cfg;
    struct ble_hs_conn *conn;
    uint16_t conn_handle;
    uint8_t att_op;
    int clt_cfg_idx;
    int i;

    /* Determine if notifications / indications are enabled for this
     * characteristic.
     */
    clt_cfg_idx = ble_gatts_clt_cfg_find_idx(ble_gatts_clt_cfgs,
                                             chr_val_handle);
    if (clt_cfg_idx == -1) {
        return;
    }

    for (i = 0; ; i++) {
        ble_hs_lock();

        conn = ble_hs_conn_find_by_idx(i);
        if (conn != NULL) {
            BLE_HS_DBG_ASSERT_EVAL(conn->bhc_gatt_svr.num_clt_cfgs >
                                   clt_cfg_idx);
            clt_cfg = conn->bhc_gatt_svr.clt_cfgs + clt_cfg_idx;
            BLE_HS_DBG_ASSERT_EVAL(clt_cfg->chr_val_handle == chr_val_handle);

            /* Determine what type of command should get sent, if any. */
            att_op = ble_gatts_schedule_update(conn, clt_cfg);
            conn_handle = conn->bhc_handle;
        } else {
            /* Silence some spurious gcc warnings. */
            att_op = 0;
            conn_handle = BLE_HS_CONN_HANDLE_NONE;
        }
        ble_hs_unlock();

        if (conn == NULL) {
            /* No more connected devices. */
            break;
        }

        switch (att_op) {
        case 0:
            break;

        case BLE_ATT_OP_NOTIFY_REQ:
            ble_gattc_notify(conn_handle, chr_val_handle);
            break;

        case BLE_ATT_OP_INDICATE_REQ:
            ble_gattc_indicate(conn_handle, chr_val_handle);
            break;

        default:
            BLE_HS_DBG_ASSERT(0);
            break;
        }
    }
}

/**
 * Sends all pending notifications and indications.  The bluetooth spec does
 * not allow more than one concurrent indication for a single peer, so this
 * function will hold off on sending such indications.
 */
void
ble_gatts_tx_notifications(void)
{
    uint16_t chr_val_handle;
    int i;

    for (i = 0; i < ble_gatts_num_cfgable_chrs; i++) {
        chr_val_handle = ble_gatts_clt_cfgs[i].chr_val_handle;
        ble_gatts_tx_notifications_one_chr(chr_val_handle);
    }
}

void
ble_gatts_bonding_established(uint16_t conn_handle)
{
    struct ble_store_value_cccd cccd_value;
    struct ble_gatts_clt_cfg *clt_cfg;
    struct ble_gatts_conn *gatt_srv;
    struct ble_hs_conn *conn;
    int i;

    ble_hs_lock();

    conn = ble_hs_conn_find(conn_handle);
    BLE_HS_DBG_ASSERT(conn != NULL);
    BLE_HS_DBG_ASSERT(conn->bhc_sec_state.bonded);

    cccd_value.peer_addr = conn->bhc_peer_addr;
    gatt_srv = &conn->bhc_gatt_svr;

    for (i = 0; i < gatt_srv->num_clt_cfgs; ++i) {
        clt_cfg = (gatt_srv->clt_cfgs + i);
        if (clt_cfg == NULL) {
            continue;
        }

        if (clt_cfg->flags != 0) {
            cccd_value.chr_val_handle = clt_cfg->chr_val_handle;
            cccd_value.flags = clt_cfg->flags;
            cccd_value.value_changed = 0;

            /* Store write use ble_hs_lock */
            ble_hs_unlock();
            ble_store_write_cccd(&cccd_value);
            ble_hs_lock();

            conn = ble_hs_conn_find(conn_handle);
            BLE_HS_DBG_ASSERT(conn != NULL);
        }
    }

    ble_hs_unlock();
}

/**
 * Called when bonding has been restored via the encryption procedure.  This
 * function:
 *     o Restores persisted CCCD entries for the connected peer.
 *     o Sends all pending notifications to the connected peer.
 *     o Sends up to one pending indication to the connected peer; schedules
 *       any remaining pending indications.
 */
void
ble_gatts_bonding_restored(uint16_t conn_handle)
{
    struct ble_store_value_cccd cccd_value;
    struct ble_store_key_cccd cccd_key;
    struct ble_gatts_clt_cfg *clt_cfg;
    struct ble_hs_conn *conn;
    uint8_t att_op;
    int rc;

    ble_hs_lock();

    conn = ble_hs_conn_find(conn_handle);
    BLE_HS_DBG_ASSERT(conn != NULL);
    BLE_HS_DBG_ASSERT(conn->bhc_sec_state.bonded);

    cccd_key.peer_addr = conn->bhc_peer_addr;
    cccd_key.chr_val_handle = 0;
    cccd_key.idx = 0;

    ble_hs_unlock();

    while (1) {
        rc = ble_store_read_cccd(&cccd_key, &cccd_value);
        if (rc != 0) {
            break;
        }

        /* Assume no notification or indication will get sent. */
        att_op = 0;

        ble_hs_lock();

        conn = ble_hs_conn_find(conn_handle);
        BLE_HS_DBG_ASSERT(conn != NULL);

        clt_cfg = ble_gatts_clt_cfg_find(conn->bhc_gatt_svr.clt_cfgs,
                                         cccd_value.chr_val_handle);
        if (clt_cfg != NULL) {
            clt_cfg->flags = cccd_value.flags;

            if (cccd_value.value_changed) {
                /* The characteristic's value changed while the device was
                 * disconnected or unbonded.  Schedule the notification or
                 * indication now.
                 */
                clt_cfg->flags |= BLE_GATTS_CLT_CFG_F_MODIFIED;
                att_op = ble_gatts_schedule_update(conn, clt_cfg);
            }
        }

        ble_hs_unlock();

        /* Tell the application if the peer changed its subscription state
         * when it was restored from persistence.
         */
        ble_gatts_subscribe_event(conn_handle, cccd_value.chr_val_handle,
                                  BLE_GAP_SUBSCRIBE_REASON_RESTORE,
                                  0, cccd_value.flags);

        switch (att_op) {
        case 0:
            break;

        case BLE_ATT_OP_NOTIFY_REQ:
            rc = ble_gattc_notify(conn_handle, cccd_value.chr_val_handle);
            if (rc == 0) {
                cccd_value.value_changed = 0;
                ble_store_write_cccd(&cccd_value);
            }
            break;

        case BLE_ATT_OP_INDICATE_REQ:
            ble_gattc_indicate(conn_handle, cccd_value.chr_val_handle);
            break;

        default:
            BLE_HS_DBG_ASSERT(0);
            break;
        }

        cccd_key.idx++;
    }
}

static struct ble_gatts_svc_entry *
ble_gatts_find_svc_entry(const ble_uuid_t *uuid)
{
    struct ble_gatts_svc_entry *entry;
    int i;

    for (i = 0; i < ble_gatts_num_svc_entries; i++) {
        entry = ble_gatts_svc_entries + i;
        if (ble_uuid_cmp(uuid, entry->svc->uuid) == 0) {
            return entry;
        }
    }

    return NULL;
}

static int
ble_gatts_find_svc_chr_attr(const ble_uuid_t *svc_uuid,
                            const ble_uuid_t *chr_uuid,
                            struct ble_gatts_svc_entry **out_svc_entry,
                            struct ble_att_svr_entry **out_att_chr)
{
    struct ble_gatts_svc_entry *svc_entry;
    struct ble_att_svr_entry *att_svc;
    struct ble_att_svr_entry *next;
    struct ble_att_svr_entry *cur;

    svc_entry = ble_gatts_find_svc_entry(svc_uuid);
    if (svc_entry == NULL) {
        return BLE_HS_ENOENT;
    }

    att_svc = ble_att_svr_find_by_handle(svc_entry->handle);
    if (att_svc == NULL) {
        return BLE_HS_EUNKNOWN;
    }

    cur = STAILQ_NEXT(att_svc, ha_next);
    while (1) {
        if (cur == NULL) {
            /* Reached end of attribute list without a match. */
            return BLE_HS_ENOENT;
        }
        next = STAILQ_NEXT(cur, ha_next);

        if (cur->ha_handle_id == svc_entry->end_group_handle) {
            /* Reached end of service without a match. */
            return BLE_HS_ENOENT;
        }

        if (ble_uuid_u16(cur->ha_uuid) == BLE_ATT_UUID_CHARACTERISTIC &&
            next != NULL &&
            ble_uuid_cmp(next->ha_uuid, chr_uuid) == 0) {

            if (out_svc_entry != NULL) {
                *out_svc_entry = svc_entry;
            }
            if (out_att_chr != NULL) {
                *out_att_chr = next;
            }
            return 0;
        }

        cur = next;
    }
}

int
ble_gatts_find_svc(const ble_uuid_t *uuid, uint16_t *out_handle)
{
    struct ble_gatts_svc_entry *entry;

    entry = ble_gatts_find_svc_entry(uuid);
    if (entry == NULL) {
        return BLE_HS_ENOENT;
    }

    if (out_handle != NULL) {
        *out_handle = entry->handle;
    }
    return 0;
}

int
ble_gatts_find_chr(const ble_uuid_t *svc_uuid, const ble_uuid_t *chr_uuid,
                   uint16_t *out_def_handle, uint16_t *out_val_handle)
{
    struct ble_att_svr_entry *att_chr;
    int rc;

    rc = ble_gatts_find_svc_chr_attr(svc_uuid, chr_uuid, NULL, &att_chr);
    if (rc != 0) {
        return rc;
    }

    if (out_def_handle) {
        *out_def_handle = att_chr->ha_handle_id - 1;
    }
    if (out_val_handle) {
        *out_val_handle = att_chr->ha_handle_id;
    }
    return 0;
}

int
ble_gatts_find_dsc(const ble_uuid_t *svc_uuid, const ble_uuid_t *chr_uuid,
                   const ble_uuid_t *dsc_uuid, uint16_t *out_handle)
{
    struct ble_gatts_svc_entry *svc_entry;
    struct ble_att_svr_entry *att_chr;
    struct ble_att_svr_entry *cur;
    uint16_t uuid16;
    int rc;

    rc = ble_gatts_find_svc_chr_attr(svc_uuid, chr_uuid, &svc_entry,
                                     &att_chr);
    if (rc != 0) {
        return rc;
    }

    cur = STAILQ_NEXT(att_chr, ha_next);
    while (1) {
        if (cur == NULL) {
            /* Reached end of attribute list without a match. */
            return BLE_HS_ENOENT;
        }

        if (cur->ha_handle_id > svc_entry->end_group_handle) {
            /* Reached end of service without a match. */
            return BLE_HS_ENOENT;
        }

        uuid16 = ble_uuid_u16(cur->ha_uuid);
        if (uuid16 == BLE_ATT_UUID_CHARACTERISTIC) {
            /* Reached end of characteristic without a match. */
            return BLE_HS_ENOENT;
        }

        if (ble_uuid_cmp(cur->ha_uuid, dsc_uuid) == 0) {
            if (out_handle != NULL) {
                *out_handle = cur->ha_handle_id;
                return 0;
            }
        }
        cur = STAILQ_NEXT(cur, ha_next);
    }
}

int
ble_gatts_add_svcs(const struct ble_gatt_svc_def *svcs)
{
    void *p;
    int rc;

    ble_hs_lock();
    if (!ble_gatts_mutable()) {
        rc = BLE_HS_EBUSY;
        goto done;
    }

    p = realloc(ble_gatts_svc_defs,
                (ble_gatts_num_svc_defs + 1) * sizeof *ble_gatts_svc_defs);
    if (p == NULL) {
        rc = BLE_HS_ENOMEM;
        goto done;
    }

    ble_gatts_svc_defs = p;
    ble_gatts_svc_defs[ble_gatts_num_svc_defs] = svcs;
    ble_gatts_num_svc_defs++;

    rc = 0;

done:
    ble_hs_unlock();
    return rc;
}

int
ble_gatts_svc_set_visibility(uint16_t handle, int visible)
{
    int i;

    for (i = 0; i < ble_gatts_num_svc_entries; i++) {
        struct ble_gatts_svc_entry *entry = &ble_gatts_svc_entries[i];

        if (entry->handle == handle) {
            if (visible) {
                ble_att_svr_restore_range(entry->handle, entry->end_group_handle);
            } else {
                ble_att_svr_hide_range(entry->handle, entry->end_group_handle);
            }
            return 0;
        }
    }

    return BLE_HS_ENOENT;
}

/**
 * Accumulates counts of each resource type required by the specified service
 * definition array.  This function is generally used to calculate some host
 * configuration values prior to initialization.  This function adds the counts
 * to the appropriate fields in the supplied ble_gatt_resources object without
 * clearing them first, so it can be called repeatedly with different inputs to
 * calculate totals.  Be sure to zero the resource struct prior to the first
 * call to this function.
 *
 * @param svcs                  The service array containing the resource
 *                                  definitions to be counted.
 * @param res                   The resource counts are accumulated in this
 *                                  struct.
 *
 * @return                      0 on success;
 *                              BLE_HS_EINVAL if the svcs array contains an
 *                                  invalid resource definition.
 */
static int
ble_gatts_count_resources(const struct ble_gatt_svc_def *svcs,
                          struct ble_gatt_resources *res)
{
    const struct ble_gatt_svc_def *svc;
    const struct ble_gatt_chr_def *chr;
    int s;
    int i;
    int c;
    int d;

    for (s = 0; svcs[s].type != BLE_GATT_SVC_TYPE_END; s++) {
        svc = svcs + s;

        if (!ble_gatts_svc_is_sane(svc)) {
            BLE_HS_DBG_ASSERT(0);
            return BLE_HS_EINVAL;
        }

        /* Each service requires:
         *     o 1 service
         *     o 1 attribute
         */
        res->svcs++;
        res->attrs++;

        if (svc->includes != NULL) {
            for (i = 0; svc->includes[i] != NULL; i++) {
                /* Each include requires:
                 *     o 1 include
                 *     o 1 attribute
                 */
                res->incs++;
                res->attrs++;
            }
        }

        if (svc->characteristics != NULL) {
            for (c = 0; svc->characteristics[c].uuid != NULL; c++) {
                chr = svc->characteristics + c;

                if (!ble_gatts_chr_is_sane(chr)) {
                    BLE_HS_DBG_ASSERT(0);
                    return BLE_HS_EINVAL;
                }

                /* Each characteristic requires:
                 *     o 1 characteristic
                 *     o 2 attributes
                 */
                res->chrs++;
                res->attrs += 2;

                /* If the characteristic permits notifications or indications,
                 * it has a CCCD.
                 */
                if (chr->flags & BLE_GATT_CHR_F_NOTIFY ||
                    chr->flags & BLE_GATT_CHR_F_INDICATE) {

                    /* Each CCCD requires:
                     *     o 1 descriptor
                     *     o 1 CCCD
                     *     o 1 attribute
                     */
                    res->dscs++;
                    res->cccds++;
                    res->attrs++;
                }

                if (chr->descriptors != NULL) {
                    for (d = 0; chr->descriptors[d].uuid != NULL; d++) {
                        if (!ble_gatts_dsc_is_sane(chr->descriptors + d)) {
                            BLE_HS_DBG_ASSERT(0);
                            return BLE_HS_EINVAL;
                        }

                        /* Each descriptor requires:
                         *     o 1 descriptor
                         *     o 1 attribute
                         */
                        res->dscs++;
                        res->attrs++;
                    }
                }
            }
        }
    }

    return 0;
}
int
ble_gatts_count_cfg(const struct ble_gatt_svc_def *defs)
{
    struct ble_gatt_resources res = { 0 };
    int rc;

    rc = ble_gatts_count_resources(defs, &res);
    if (rc != 0) {
        return rc;
    }

    ble_hs_max_services += res.svcs;
    ble_hs_max_attrs += res.attrs;

    /* Reserve an extra CCCD for the cache. */
    ble_hs_max_client_configs +=
        res.cccds * (MYNEWT_VAL(BLE_MAX_CONNECTIONS) + 1);

    return 0;
}

void
ble_gatts_lcl_svc_foreach(ble_gatt_svc_foreach_fn cb, void *arg)
{
    int i;

    for (i = 0; i < ble_gatts_num_svc_entries; i++) {
        cb(ble_gatts_svc_entries[i].svc,
           ble_gatts_svc_entries[i].handle,
           ble_gatts_svc_entries[i].end_group_handle, arg);
    }
}

int
ble_gatts_reset(void)
{
    int rc;

    ble_hs_lock();

    if (!ble_gatts_mutable()) {
        rc = BLE_HS_EBUSY;
    } else {
        /* Unregister all ATT attributes. */
        ble_att_svr_reset();
        ble_gatts_num_cfgable_chrs = 0;
        rc = 0;

        /* Note: gatts memory gets freed on next call to ble_gatts_start(). */
    }

    ble_hs_unlock();

    return rc;
}

int
ble_gatts_init(void)
{
    int rc;

    ble_gatts_num_cfgable_chrs = 0;
    ble_gatts_clt_cfgs = NULL;

    rc = stats_init_and_reg(
        STATS_HDR(ble_gatts_stats), STATS_SIZE_INIT_PARMS(ble_gatts_stats,
        STATS_SIZE_32), STATS_NAME_INIT_PARMS(ble_gatts_stats), "ble_gatts");
    if (rc != 0) {
        return BLE_HS_EOS;
    }

    return 0;

}
