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

#ifndef H_BLE_GAP_CONN_
#define H_BLE_GAP_CONN_

#include <inttypes.h>
#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#include "nimble/porting/nimble/include/stats/stats.h"
#include "nimble/nimble/host/include/host/ble_gap.h"
#ifdef __cplusplus
extern "C" {
#endif

struct hci_le_conn_upd_complete;
struct hci_le_conn_param_req;
struct hci_le_conn_complete;
struct hci_disconn_complete;
struct hci_encrypt_change;
struct ble_hs_hci_ack;
struct ble_hs_adv;

STATS_SECT_START(ble_gap_stats)
    STATS_SECT_ENTRY(wl_set)
    STATS_SECT_ENTRY(wl_set_fail)
    STATS_SECT_ENTRY(adv_stop)
    STATS_SECT_ENTRY(adv_stop_fail)
    STATS_SECT_ENTRY(adv_start)
    STATS_SECT_ENTRY(adv_start_fail)
    STATS_SECT_ENTRY(adv_set_data)
    STATS_SECT_ENTRY(adv_set_data_fail)
    STATS_SECT_ENTRY(adv_rsp_set_data)
    STATS_SECT_ENTRY(adv_rsp_set_data_fail)
    STATS_SECT_ENTRY(discover)
    STATS_SECT_ENTRY(discover_fail)
    STATS_SECT_ENTRY(initiate)
    STATS_SECT_ENTRY(initiate_fail)
    STATS_SECT_ENTRY(terminate)
    STATS_SECT_ENTRY(terminate_fail)
    STATS_SECT_ENTRY(cancel)
    STATS_SECT_ENTRY(cancel_fail)
    STATS_SECT_ENTRY(update)
    STATS_SECT_ENTRY(update_fail)
    STATS_SECT_ENTRY(connect_mst)
    STATS_SECT_ENTRY(connect_slv)
    STATS_SECT_ENTRY(disconnect)
    STATS_SECT_ENTRY(rx_disconnect)
    STATS_SECT_ENTRY(rx_update_complete)
    STATS_SECT_ENTRY(rx_adv_report)
    STATS_SECT_ENTRY(rx_conn_complete)
    STATS_SECT_ENTRY(discover_cancel)
    STATS_SECT_ENTRY(discover_cancel_fail)
    STATS_SECT_ENTRY(security_initiate)
    STATS_SECT_ENTRY(security_initiate_fail)
STATS_SECT_END

extern STATS_SECT_DECL(ble_gap_stats) ble_gap_stats;

#define BLE_GAP_CONN_MODE_MAX               3
#define BLE_GAP_DISC_MODE_MAX               3

#if MYNEWT_VAL(BLE_EXT_ADV) && NIMBLE_BLE_SCAN
void ble_gap_rx_le_scan_timeout(void);
#endif

#if MYNEWT_VAL(BLE_EXT_ADV)
void ble_gap_rx_ext_adv_report(struct ble_gap_ext_disc_desc *desc);
void ble_gap_rx_adv_set_terminated(const struct ble_hci_ev_le_subev_adv_set_terminated *ev);
#if MYNEWT_VAL(BLE_PERIODIC_ADV)
void ble_gap_rx_peroidic_adv_sync_estab(const struct ble_hci_ev_le_subev_periodic_adv_sync_estab *ev);
void ble_gap_rx_periodic_adv_rpt(const struct ble_hci_ev_le_subev_periodic_adv_rpt *ev);
void ble_gap_rx_periodic_adv_sync_lost(const struct ble_hci_ev_le_subev_periodic_adv_sync_lost *ev);
void ble_gap_rx_periodic_adv_sync_transfer(const struct ble_hci_ev_le_subev_periodic_adv_sync_transfer *ev);
#endif
void ble_gap_rx_scan_req_rcvd(const struct ble_hci_ev_le_subev_scan_req_rcvd *ev);
#endif
void ble_gap_rx_adv_report(struct ble_gap_disc_desc *desc);
void ble_gap_rx_rd_rem_sup_feat_complete(const struct ble_hci_ev_le_subev_rd_rem_used_feat *ev);

struct ble_gap_conn_complete
{
    uint8_t status;
    uint16_t connection_handle;
    uint8_t role;
    uint8_t peer_addr_type;
    uint8_t peer_addr[BLE_DEV_ADDR_LEN];
    uint16_t conn_itvl;
    uint16_t conn_latency;
    uint16_t supervision_timeout;
    uint8_t master_clk_acc;
    uint8_t local_rpa[BLE_DEV_ADDR_LEN];
    uint8_t peer_rpa[BLE_DEV_ADDR_LEN];
};

int ble_gap_rx_conn_complete(struct ble_gap_conn_complete *evt, uint8_t instance);
void ble_gap_rx_disconn_complete(const struct ble_hci_ev_disconn_cmp *ev);
void ble_gap_rx_update_complete(const struct ble_hci_ev_le_subev_conn_upd_complete *ev);
void ble_gap_rx_param_req(const struct ble_hci_ev_le_subev_rem_conn_param_req *ev);
int ble_gap_rx_l2cap_update_req(uint16_t conn_handle,
                                struct ble_gap_upd_params *params);
void ble_gap_rx_phy_update_complete(const struct ble_hci_ev_le_subev_phy_update_complete *ev);
void ble_gap_enc_event(uint16_t conn_handle, int status,
                       int security_restored, int bonded);
void ble_gap_passkey_event(uint16_t conn_handle,
                           struct ble_gap_passkey_params *passkey_params);
void ble_gap_notify_rx_event(uint16_t conn_handle, uint16_t attr_handle,
                             struct os_mbuf *om, int is_indication);
void ble_gap_notify_tx_event(int status, uint16_t conn_handle,
                             uint16_t attr_handle, int is_indication);
void ble_gap_subscribe_event(uint16_t conn_handle, uint16_t attr_handle,
                             uint8_t reason,
                             uint8_t prev_notify, uint8_t cur_notify,
                             uint8_t prev_indicate, uint8_t cur_indicate);
void ble_gap_mtu_event(uint16_t conn_handle, uint16_t cid, uint16_t mtu);
void ble_gap_identity_event(uint16_t conn_handle);
int ble_gap_repeat_pairing_event(const struct ble_gap_repeat_pairing *rp);
int ble_gap_master_in_progress(void);

void ble_gap_preempt(void);
void ble_gap_preempt_done(void);

int ble_gap_terminate_with_conn(struct ble_hs_conn *conn, uint8_t hci_reason);
void ble_gap_conn_broken(uint16_t conn_handle, int reason);
void ble_gap_reset_state(int reason);
int32_t ble_gap_timer(void);

int ble_gap_init(void);
void ble_gap_deinit(void);

#if MYNEWT_VAL(BLE_HS_DEBUG)
int ble_gap_dbg_update_active(uint16_t conn_handle);
#endif

#ifdef __cplusplus
}
#endif

#endif
