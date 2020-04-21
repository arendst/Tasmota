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

#ifndef H_BLE_GATT_PRIV_
#define H_BLE_GATT_PRIV_

#include "syscfg/syscfg.h"
#include "stats/stats.h"
#include "host/ble_gatt.h"
#ifdef __cplusplus
extern "C" {
#endif

struct ble_att_read_type_adata;
struct ble_att_find_type_value_hinfo;
struct ble_att_find_info_idata;
struct ble_att_read_group_type_adata;
struct ble_att_prep_write_cmd;

STATS_SECT_START(ble_gattc_stats)
    STATS_SECT_ENTRY(mtu)
    STATS_SECT_ENTRY(mtu_fail)
    STATS_SECT_ENTRY(disc_all_svcs)
    STATS_SECT_ENTRY(disc_all_svcs_fail)
    STATS_SECT_ENTRY(disc_svc_uuid)
    STATS_SECT_ENTRY(disc_svc_uuid_fail)
    STATS_SECT_ENTRY(find_inc_svcs)
    STATS_SECT_ENTRY(find_inc_svcs_fail)
    STATS_SECT_ENTRY(disc_all_chrs)
    STATS_SECT_ENTRY(disc_all_chrs_fail)
    STATS_SECT_ENTRY(disc_chrs_uuid)
    STATS_SECT_ENTRY(disc_chrs_uuid_fail)
    STATS_SECT_ENTRY(disc_all_dscs)
    STATS_SECT_ENTRY(disc_all_dscs_fail)
    STATS_SECT_ENTRY(read)
    STATS_SECT_ENTRY(read_fail)
    STATS_SECT_ENTRY(read_uuid)
    STATS_SECT_ENTRY(read_uuid_fail)
    STATS_SECT_ENTRY(read_long)
    STATS_SECT_ENTRY(read_long_fail)
    STATS_SECT_ENTRY(read_mult)
    STATS_SECT_ENTRY(read_mult_fail)
    STATS_SECT_ENTRY(write_no_rsp)
    STATS_SECT_ENTRY(write_no_rsp_fail)
    STATS_SECT_ENTRY(write)
    STATS_SECT_ENTRY(write_fail)
    STATS_SECT_ENTRY(write_long)
    STATS_SECT_ENTRY(write_long_fail)
    STATS_SECT_ENTRY(write_reliable)
    STATS_SECT_ENTRY(write_reliable_fail)
    STATS_SECT_ENTRY(notify)
    STATS_SECT_ENTRY(notify_fail)
    STATS_SECT_ENTRY(indicate)
    STATS_SECT_ENTRY(indicate_fail)
    STATS_SECT_ENTRY(proc_timeout)
STATS_SECT_END
extern STATS_SECT_DECL(ble_gattc_stats) ble_gattc_stats;

STATS_SECT_START(ble_gatts_stats)
    STATS_SECT_ENTRY(svcs)
    STATS_SECT_ENTRY(chrs)
    STATS_SECT_ENTRY(dscs)
    STATS_SECT_ENTRY(svc_def_reads)
    STATS_SECT_ENTRY(svc_inc_reads)
    STATS_SECT_ENTRY(chr_def_reads)
    STATS_SECT_ENTRY(chr_val_reads)
    STATS_SECT_ENTRY(chr_val_writes)
    STATS_SECT_ENTRY(dsc_reads)
    STATS_SECT_ENTRY(dsc_writes)
STATS_SECT_END
extern STATS_SECT_DECL(ble_gatts_stats) ble_gatts_stats;

#define BLE_GATT_CHR_DECL_SZ_16     5
#define BLE_GATT_CHR_DECL_SZ_128    19

typedef uint8_t ble_gatts_conn_flags;

struct ble_gatts_conn {
    struct ble_gatts_clt_cfg *clt_cfgs;
    int num_clt_cfgs;

    uint16_t indicate_val_handle;
};

/*** @client. */

int ble_gattc_locked_by_cur_task(void);
void ble_gatts_indicate_fail_notconn(uint16_t conn_handle);

void ble_gattc_rx_err(uint16_t conn_handle, uint16_t handle, uint16_t status);
void ble_gattc_rx_mtu(uint16_t conn_handle, int status, uint16_t chan_mtu);
void ble_gattc_rx_read_type_adata(uint16_t conn_handle,
                                  struct ble_att_read_type_adata *adata);
void ble_gattc_rx_read_type_complete(uint16_t conn_handle, int status);
void ble_gattc_rx_read_rsp(uint16_t conn_handle, int status,
                           struct os_mbuf **rxom);
void ble_gattc_rx_read_blob_rsp(uint16_t conn_handle, int status,
                                struct os_mbuf **rxom);
void ble_gattc_rx_read_mult_rsp(uint16_t conn_handle, int status,
                                struct os_mbuf **rxom);
void ble_gattc_rx_read_group_type_adata(
    uint16_t conn_handle, struct ble_att_read_group_type_adata *adata);
void ble_gattc_rx_read_group_type_complete(uint16_t conn_handle, int rc);
void ble_gattc_rx_find_type_value_hinfo(
    uint16_t conn_handle, struct ble_att_find_type_value_hinfo *hinfo);
void ble_gattc_rx_find_type_value_complete(uint16_t conn_handle, int status);
void ble_gattc_rx_write_rsp(uint16_t conn_handle);
void ble_gattc_rx_prep_write_rsp(uint16_t conn_handle, int status,
                                 uint16_t handle, uint16_t offset,
                                 struct os_mbuf **rxom);
void ble_gattc_rx_exec_write_rsp(uint16_t conn_handle, int status);
void ble_gattc_rx_indicate_rsp(uint16_t conn_handle);
void ble_gattc_rx_find_info_idata(uint16_t conn_handle,
                                  struct ble_att_find_info_idata *idata);
void ble_gattc_rx_find_info_complete(uint16_t conn_handle, int status);
void ble_gattc_connection_txable(uint16_t conn_handle);
void ble_gattc_connection_broken(uint16_t conn_handle);
int32_t ble_gattc_timer(void);

int ble_gattc_any_jobs(void);
int ble_gattc_init(void);

/*** @server. */
#define BLE_GATTS_CLT_CFG_F_NOTIFY              0x0001
#define BLE_GATTS_CLT_CFG_F_INDICATE            0x0002
#define BLE_GATTS_CLT_CFG_F_MODIFIED            0x0080 /* Internal only. */
#define BLE_GATTS_CLT_CFG_F_RESERVED            0xfffc

#define BLE_GATTS_INC_SVC_LEN_NO_UUID           4
#define BLE_GATTS_INC_SVC_LEN_UUID              6

/**
 * Contains counts of resources required by the GATT server.  The contents of
 * this struct are generally used to populate a configuration struct before
 * the host is initialized.
 */
struct ble_gatt_resources {
    /** Number of services. */
    uint16_t svcs;

    /** Number of included services. */
    uint16_t incs;

    /** Number of characteristics. */
    uint16_t chrs;

    /** Number of descriptors. */
    uint16_t dscs;

    /**
     * Number of client characteristic configuration descriptors.  Each of
     * these also contributes to the total descriptor count.
     */
    uint16_t cccds;

    /** Total number of ATT attributes. */
    uint16_t attrs;
};

int ble_gatts_rx_indicate_ack(uint16_t conn_handle, uint16_t chr_val_handle);
int ble_gatts_send_next_indicate(uint16_t conn_handle);
void ble_gatts_tx_notifications(void);
void ble_gatts_bonding_established(uint16_t conn_handle);
void ble_gatts_bonding_restored(uint16_t conn_handle);
void ble_gatts_connection_broken(uint16_t conn_handle);
void ble_gatts_lcl_svc_foreach(ble_gatt_svc_foreach_fn cb, void *arg);
int ble_gatts_register_svcs(const struct ble_gatt_svc_def *svcs,
                            ble_gatt_register_fn *register_cb,
                            void *cb_arg);
int ble_gatts_clt_cfg_access(uint16_t conn_handle, uint16_t attr_handle,
                             uint8_t op, uint16_t offset, struct os_mbuf **om,
                             void *arg);

/*** @misc. */
int ble_gatts_conn_can_alloc(void);
int ble_gatts_conn_init(struct ble_gatts_conn *gatts_conn);
int ble_gatts_init(void);

#ifdef __cplusplus
}
#endif

#endif
