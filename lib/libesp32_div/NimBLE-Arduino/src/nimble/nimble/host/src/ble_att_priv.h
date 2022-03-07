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

#ifndef H_BLE_ATT_PRIV_
#define H_BLE_ATT_PRIV_

#include <inttypes.h>
#include "nimble/porting/nimble/include/stats/stats.h"
#include "nimble/nimble/host/include/host/ble_att.h"
#include "nimble/nimble/host/include/host/ble_uuid.h"
#include "nimble/nimble/include/nimble/nimble_npl.h"

#ifdef __cplusplus
extern "C" {
#endif

struct os_mbuf;
struct ble_hs_conn;
struct ble_l2cap_chan;
struct ble_att_find_info_req;
struct ble_att_error_rsp;
struct ble_att_mtu_cmd;
struct ble_att_read_req;
struct ble_att_read_blob_req;
struct ble_att_read_type_req;
struct ble_att_read_group_type_req;
struct ble_att_read_group_type_rsp;
struct ble_att_find_type_value_req;
struct ble_att_write_req;
struct ble_att_prep_write_cmd;
struct ble_att_exec_write_req;
struct ble_att_notify_req;
struct ble_att_indicate_req;

STATS_SECT_START(ble_att_stats)
    STATS_SECT_ENTRY(error_rsp_rx)
    STATS_SECT_ENTRY(error_rsp_tx)
    STATS_SECT_ENTRY(mtu_req_rx)
    STATS_SECT_ENTRY(mtu_req_tx)
    STATS_SECT_ENTRY(mtu_rsp_rx)
    STATS_SECT_ENTRY(mtu_rsp_tx)
    STATS_SECT_ENTRY(find_info_req_rx)
    STATS_SECT_ENTRY(find_info_req_tx)
    STATS_SECT_ENTRY(find_info_rsp_rx)
    STATS_SECT_ENTRY(find_info_rsp_tx)
    STATS_SECT_ENTRY(find_type_value_req_rx)
    STATS_SECT_ENTRY(find_type_value_req_tx)
    STATS_SECT_ENTRY(find_type_value_rsp_rx)
    STATS_SECT_ENTRY(find_type_value_rsp_tx)
    STATS_SECT_ENTRY(read_type_req_rx)
    STATS_SECT_ENTRY(read_type_req_tx)
    STATS_SECT_ENTRY(read_type_rsp_rx)
    STATS_SECT_ENTRY(read_type_rsp_tx)
    STATS_SECT_ENTRY(read_req_rx)
    STATS_SECT_ENTRY(read_req_tx)
    STATS_SECT_ENTRY(read_rsp_rx)
    STATS_SECT_ENTRY(read_rsp_tx)
    STATS_SECT_ENTRY(read_blob_req_rx)
    STATS_SECT_ENTRY(read_blob_req_tx)
    STATS_SECT_ENTRY(read_blob_rsp_rx)
    STATS_SECT_ENTRY(read_blob_rsp_tx)
    STATS_SECT_ENTRY(read_mult_req_rx)
    STATS_SECT_ENTRY(read_mult_req_tx)
    STATS_SECT_ENTRY(read_mult_rsp_rx)
    STATS_SECT_ENTRY(read_mult_rsp_tx)
    STATS_SECT_ENTRY(read_group_type_req_rx)
    STATS_SECT_ENTRY(read_group_type_req_tx)
    STATS_SECT_ENTRY(read_group_type_rsp_rx)
    STATS_SECT_ENTRY(read_group_type_rsp_tx)
    STATS_SECT_ENTRY(write_req_rx)
    STATS_SECT_ENTRY(write_req_tx)
    STATS_SECT_ENTRY(write_rsp_rx)
    STATS_SECT_ENTRY(write_rsp_tx)
    STATS_SECT_ENTRY(prep_write_req_rx)
    STATS_SECT_ENTRY(prep_write_req_tx)
    STATS_SECT_ENTRY(prep_write_rsp_rx)
    STATS_SECT_ENTRY(prep_write_rsp_tx)
    STATS_SECT_ENTRY(exec_write_req_rx)
    STATS_SECT_ENTRY(exec_write_req_tx)
    STATS_SECT_ENTRY(exec_write_rsp_rx)
    STATS_SECT_ENTRY(exec_write_rsp_tx)
    STATS_SECT_ENTRY(notify_req_rx)
    STATS_SECT_ENTRY(notify_req_tx)
    STATS_SECT_ENTRY(indicate_req_rx)
    STATS_SECT_ENTRY(indicate_req_tx)
    STATS_SECT_ENTRY(indicate_rsp_rx)
    STATS_SECT_ENTRY(indicate_rsp_tx)
    STATS_SECT_ENTRY(write_cmd_rx)
    STATS_SECT_ENTRY(write_cmd_tx)
STATS_SECT_END
extern STATS_SECT_DECL(ble_att_stats) ble_att_stats;

struct ble_att_prep_entry {
    SLIST_ENTRY(ble_att_prep_entry) bape_next;
    uint16_t bape_handle;
    uint16_t bape_offset;

    /* XXX: This is wasteful; we should use one mbuf chain for the entire
     * prepared write, and compress the data into as few mbufs as possible.
     */
    struct os_mbuf *bape_value;
};

SLIST_HEAD(ble_att_prep_entry_list, ble_att_prep_entry);

struct ble_att_svr_conn {
    /** This list is sorted by attribute handle ID. */
    struct ble_att_prep_entry_list basc_prep_list;
    ble_npl_time_t basc_prep_timeout_at;
};

/**
 * Handles a host attribute request.
 *
 * @param entry                 The host attribute being requested.
 * @param op                    The operation being performed on the attribute.
 * @param arg                   The request data associated with that host
 *                                  attribute.
 *
 * @return                      0 on success;
 *                              One of the BLE_ATT_ERR_[...] codes on
 *                                  failure.
 */
typedef int ble_att_svr_access_fn(uint16_t conn_handle, uint16_t attr_handle,
                                  uint8_t op, uint16_t offset,
                                  struct os_mbuf **om, void *arg);

int ble_att_svr_register(const ble_uuid_t *uuid, uint8_t flags,
                         uint8_t min_key_size, uint16_t *handle_id,
                         ble_att_svr_access_fn *cb, void *cb_arg);

struct ble_att_svr_entry {
    STAILQ_ENTRY(ble_att_svr_entry) ha_next;

    const ble_uuid_t *ha_uuid;
    uint8_t ha_flags;
    uint8_t ha_min_key_size;
    uint16_t ha_handle_id;
    ble_att_svr_access_fn *ha_cb;
    void *ha_cb_arg;
};

SLIST_HEAD(ble_att_clt_entry_list, ble_att_clt_entry);

/*** @gen */

struct ble_l2cap_chan *ble_att_create_chan(uint16_t conn_handle);
int ble_att_conn_chan_find(uint16_t conn_handle, struct ble_hs_conn **out_conn,
                           struct ble_l2cap_chan **out_chan);
void ble_att_inc_tx_stat(uint8_t att_op);
void ble_att_truncate_to_mtu(const struct ble_l2cap_chan *att_chan,
                             struct os_mbuf *txom);
void ble_att_set_peer_mtu(struct ble_l2cap_chan *chan, uint16_t peer_mtu);
uint16_t ble_att_chan_mtu(const struct ble_l2cap_chan *chan);
int ble_att_init(void);

/*** @svr */

int ble_att_svr_start(void);
void ble_att_svr_stop(void);

struct ble_att_svr_entry *
ble_att_svr_find_by_uuid(struct ble_att_svr_entry *start_at,
                         const ble_uuid_t *uuid,
                         uint16_t end_handle);
uint16_t ble_att_svr_prev_handle(void);
int ble_att_svr_rx_mtu(uint16_t conn_handle, struct os_mbuf **rxom);
struct ble_att_svr_entry *ble_att_svr_find_by_handle(uint16_t handle_id);
int32_t ble_att_svr_ticks_until_tmo(const struct ble_att_svr_conn *svr,
                                    ble_npl_time_t now);
int ble_att_svr_rx_find_info(uint16_t conn_handle, struct os_mbuf **rxom);
int ble_att_svr_rx_find_type_value(uint16_t conn_handle,
                                   struct os_mbuf **rxom);
int ble_att_svr_rx_read_type(uint16_t conn_handle,
                             struct os_mbuf **rxom);
int ble_att_svr_rx_read_group_type(uint16_t conn_handle,
                                   struct os_mbuf **rxom);
int ble_att_svr_rx_read(uint16_t conn_handle,
                        struct os_mbuf **rxom);
int ble_att_svr_rx_read_blob(uint16_t conn_handle,
                             struct os_mbuf **rxom);
int ble_att_svr_rx_read_mult(uint16_t conn_handle,
                             struct os_mbuf **rxom);
int ble_att_svr_rx_write(uint16_t conn_handle,
                         struct os_mbuf **rxom);
int ble_att_svr_rx_write_no_rsp(uint16_t conn_handle, struct os_mbuf **rxom);
int ble_att_svr_rx_prep_write(uint16_t conn_handle,
                              struct os_mbuf **rxom);
int ble_att_svr_rx_exec_write(uint16_t conn_handle,
                              struct os_mbuf **rxom);
int ble_att_svr_rx_notify(uint16_t conn_handle,
                          struct os_mbuf **rxom);
int ble_att_svr_rx_indicate(uint16_t conn_handle,
                            struct os_mbuf **rxom);
void ble_att_svr_prep_clear(struct ble_att_prep_entry_list *prep_list);
int ble_att_svr_read_handle(uint16_t conn_handle, uint16_t attr_handle,
                            uint16_t offset, struct os_mbuf *om,
                            uint8_t *out_att_err);
void ble_att_svr_reset(void);
int ble_att_svr_init(void);

void ble_att_svr_hide_range(uint16_t start_handle, uint16_t end_handle);
void ble_att_svr_restore_range(uint16_t start_handle, uint16_t end_handle);

int ble_att_svr_tx_error_rsp(uint16_t conn_handle, struct os_mbuf *txom,
                             uint8_t req_op, uint16_t handle,
                             uint8_t error_code);
/*** $clt */

/** An information-data entry in a find information response. */
struct ble_att_find_info_idata {
    uint16_t attr_handle;
    ble_uuid_any_t uuid;
};

/** A handles-information entry in a find by type value response. */
struct ble_att_find_type_value_hinfo {
    uint16_t attr_handle;
    uint16_t group_end_handle;
};

/** An attribute-data entry in a read by type response. */
struct ble_att_read_type_adata {
    uint16_t att_handle;
    int value_len;
    uint8_t *value;

};

/** An attribute-data entry in a read by group type response. */
struct ble_att_read_group_type_adata {
    uint16_t att_handle;
    uint16_t end_group_handle;
    int value_len;
    uint8_t *value;
};

int ble_att_clt_rx_error(uint16_t conn_handle, struct os_mbuf **rxom);
int ble_att_clt_tx_mtu(uint16_t conn_handle, uint16_t mtu);
int ble_att_clt_rx_mtu(uint16_t conn_handle, struct os_mbuf **rxom);
int ble_att_clt_tx_read(uint16_t conn_handle, uint16_t handle);
int ble_att_clt_rx_read(uint16_t conn_handle, struct os_mbuf **rxom);
int ble_att_clt_tx_read_blob(uint16_t conn_handle, uint16_t handle,
                             uint16_t offset);
int ble_att_clt_rx_read_blob(uint16_t conn_handle, struct os_mbuf **rxom);
int ble_att_clt_tx_read_mult(uint16_t conn_handle,
                             const uint16_t *handles, int num_handles);
int ble_att_clt_rx_read_mult(uint16_t conn_handle, struct os_mbuf **rxom);
int ble_att_clt_tx_read_type(uint16_t conn_handle, uint16_t start_handle,
                             uint16_t end_handle, const ble_uuid_t *uuid);
int ble_att_clt_rx_read_type(uint16_t conn_handle, struct os_mbuf **rxom);
int ble_att_clt_tx_read_group_type(uint16_t conn_handle,
                                   uint16_t start_handle, uint16_t end_handle,
                                   const ble_uuid_t *uuid128);
int ble_att_clt_rx_read_group_type(uint16_t conn_handle,
                                   struct os_mbuf **rxom);
int ble_att_clt_tx_find_info(uint16_t conn_handle, uint16_t start_handle,
                             uint16_t end_handle);
int ble_att_clt_rx_find_info(uint16_t conn_handle, struct os_mbuf **rxom);
int ble_att_clt_tx_find_type_value(uint16_t conn_handle, uint16_t start_handle,
                                   uint16_t end_handle, uint16_t attribute_type,
                                   const void *attribute_value, int value_len);
int ble_att_clt_rx_find_type_value(uint16_t conn_handle,
                                   struct os_mbuf **rxom);
int ble_att_clt_tx_write_req(uint16_t conn_handle, uint16_t handle,
                             struct os_mbuf *txom);
int ble_att_clt_tx_write_cmd(uint16_t conn_handle, uint16_t handle,
                             struct os_mbuf *txom);
int ble_att_clt_tx_prep_write(uint16_t conn_handle, uint16_t handle,
                              uint16_t offset, struct os_mbuf *txom);
int ble_att_clt_rx_prep_write(uint16_t conn_handle, struct os_mbuf **rxom);
int ble_att_clt_tx_exec_write(uint16_t conn_handle, uint8_t flags);
int ble_att_clt_rx_exec_write(uint16_t conn_handle, struct os_mbuf **rxom);
int ble_att_clt_rx_write(uint16_t conn_handle, struct os_mbuf **rxom);
int ble_att_clt_tx_notify(uint16_t conn_handle, uint16_t handle,
                          struct os_mbuf *txom);
int ble_att_clt_tx_indicate(uint16_t conn_handle, uint16_t handle,
                            struct os_mbuf *txom);
int ble_att_clt_rx_indicate(uint16_t conn_handle, struct os_mbuf **rxom);

#ifdef __cplusplus
}
#endif

#endif
