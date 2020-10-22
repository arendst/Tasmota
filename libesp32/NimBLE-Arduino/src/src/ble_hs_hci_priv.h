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

#ifndef H_BLE_HS_HCI_PRIV_
#define H_BLE_HS_HCI_PRIV_

#include "nimble/hci_common.h"
#ifdef __cplusplus
extern "C" {
#endif

struct ble_hs_conn;
struct os_mbuf;

#define BLE_HS_HCI_LE_FEAT_ENCRYPTION                   (0x00000001)
#define BLE_HS_HCI_LE_FEAT_CONN_PARAM_REQUEST           (0x00000002)
#define BLE_HS_HCI_LE_FEAT_EXT_REJECT                   (0x00000004)
#define BLE_HS_HCI_LE_FEAT_SLAVE_FEAT_EXCHANGE          (0x00000008)
#define BLE_HS_HCI_LE_FEAT_PING                         (0x00000010)
#define BLE_HS_HCI_LE_FEAT_DATA_PACKET_LENGTH_EXT       (0x00000020)
#define BLE_HS_HCI_LE_FEAT_LL_PRIVACY                   (0x00000040)
#define BLE_HS_HCI_LE_FEAT_EXT_SCANNER_FILTER_POLICIES  (0x00000080)
#define BLE_HS_HCI_LE_FEAT_2M_PHY                       (0x00000100)
#define BLE_HS_HCI_LE_FEAT_STABLE_MOD_INDEX_TX          (0x00000200)
#define BLE_HS_HCI_LE_FEAT_STABLE_MOD_INDEX_RX          (0x00000400)
#define BLE_HS_HCI_LE_FEAT_CODED_PHY                    (0x00000800)
#define BLE_HS_HCI_LE_FEAT_EXT_ADV                      (0x00001000)
#define BLE_HS_HCI_LE_FEAT_PERIODIC_ADV                 (0x00002000)
#define BLE_HS_HCI_LE_FEAT_CSA2                         (0x00004000)
#define BLE_HS_HCI_LE_FEAT_POWER_CLASS_1                (0x00008000)
#define BLE_HS_HCI_LE_FEAT_MIN_NUM_USED_CHAN            (0x00010000)

struct ble_hs_hci_ack {
    int bha_status;         /* A BLE_HS_E<...> error; NOT a naked HCI code. */
    uint8_t *bha_params;
    int bha_params_len;
    uint16_t bha_opcode;
    uint8_t bha_hci_handle;
};

#if MYNEWT_VAL(BLE_EXT_ADV)
struct ble_hs_hci_ext_scan_param {
    uint8_t scan_type;
    uint16_t scan_itvl;
    uint16_t scan_window;
};

struct ble_hs_hci_ext_conn_params {
    uint16_t scan_itvl;
    uint16_t scan_window;
    uint16_t conn_itvl;
    uint16_t conn_windows;
};

#if MYNEWT_VAL(BLE_PERIODIC_ADV)
/* Periodic Advertising Parameters */
struct hci_periodic_adv_params
{
    uint16_t min_interval;
    uint16_t max_interval;
    uint16_t properties;
};
#endif
#endif

extern uint16_t ble_hs_hci_avail_pkts;

int ble_hs_hci_cmd_tx(uint16_t opcode, void *cmd, uint8_t cmd_len,
                      void *evt_buf, uint8_t evt_buf_len,
                      uint8_t *out_evt_buf_len);
int ble_hs_hci_cmd_tx_empty_ack(uint16_t opcode, void *cmd, uint8_t cmd_len);
void ble_hs_hci_rx_ack(uint8_t *ack_ev);
void ble_hs_hci_init(void);
void ble_hs_hci_deinit(void);

void ble_hs_hci_set_le_supported_feat(uint32_t feat);
uint32_t ble_hs_hci_get_le_supported_feat(void);
void ble_hs_hci_set_hci_version(uint8_t hci_version);
uint8_t ble_hs_hci_get_hci_version(void);

#if MYNEWT_VAL(BLE_HS_PHONY_HCI_ACKS)
typedef int ble_hs_hci_phony_ack_fn(uint8_t *ack, int ack_buf_len);
void ble_hs_hci_set_phony_ack_cb(ble_hs_hci_phony_ack_fn *cb);
#endif

int ble_hs_hci_util_read_adv_tx_pwr(int8_t *out_pwr);
int ble_hs_hci_util_rand(void *dst, int len);
int ble_hs_hci_util_read_rssi(uint16_t conn_handle, int8_t *out_rssi);
int ble_hs_hci_util_set_random_addr(const uint8_t *addr);
int ble_hs_hci_util_set_data_len(uint16_t conn_handle, uint16_t tx_octets,
                                 uint16_t tx_time);
int ble_hs_hci_util_data_hdr_strip(struct os_mbuf *om,
                                   struct hci_data_hdr *out_hdr);
int ble_hs_hci_evt_process(uint8_t *data);

void ble_hs_hci_cmd_write_hdr(uint8_t ogf, uint16_t ocf, uint8_t len,
                              void *buf);
int ble_hs_hci_cmd_send_buf(uint16_t opcode, void *buf, uint8_t buf_len);
void ble_hs_hci_cmd_build_set_event_mask(uint64_t event_mask,
                                         uint8_t *dst, int dst_len);
void ble_hs_hci_cmd_build_set_event_mask2(uint64_t event_mask, uint8_t *dst,
                                          int dst_len);
void ble_hs_hci_cmd_build_disconnect(uint16_t handle, uint8_t reason,
                                     uint8_t *dst, int dst_len);
void ble_hs_hci_cmd_build_read_rssi(uint16_t handle, uint8_t *dst,
                                    int dst_len);
void ble_hs_hci_cmd_build_le_set_host_chan_class(const uint8_t *chan_map,
                                                 uint8_t *dst, int dst_len);
void ble_hs_hci_cmd_build_le_read_chan_map(uint16_t conn_handle,
                                           uint8_t *dst, int dst_len);
int ble_hs_hci_cmd_build_le_set_scan_rsp_data(const uint8_t *data, uint8_t len,
                                              uint8_t *dst, int dst_len);
int ble_hs_hci_cmd_build_le_set_adv_data(const uint8_t *data, uint8_t len,
                                         uint8_t *dst, int dst_len);
int ble_hs_hci_cmd_build_le_set_adv_params(const struct hci_adv_params *adv,
                                           uint8_t *dst, int dst_len);
void ble_hs_hci_cmd_build_le_set_event_mask(uint64_t event_mask,
                                            uint8_t *dst, int dst_len);
int ble_hs_hci_cmd_le_read_buffer_size(void);
void ble_hs_hci_cmd_build_le_set_adv_enable(uint8_t enable, uint8_t *dst,
                                            int dst_len);
int ble_hs_hci_cmd_le_set_adv_enable(uint8_t enable);
int ble_hs_hci_cmd_build_le_set_scan_params(uint8_t scan_type,
                                            uint16_t scan_itvl,
                                            uint16_t scan_window,
                                            uint8_t own_addr_type,
                                            uint8_t filter_policy,
                                            uint8_t *dst, int dst_len);
void ble_hs_hci_cmd_build_le_set_scan_enable(uint8_t enable,
                                             uint8_t filter_dups,
                                             uint8_t *dst, uint8_t dst_len);
int ble_hs_hci_cmd_le_set_scan_enable(uint8_t enable, uint8_t filter_dups);
int ble_hs_hci_cmd_build_le_create_connection(
    const struct hci_create_conn *hcc, uint8_t *cmd, int cmd_len);
int ble_hs_hci_cmd_build_le_add_to_whitelist(const uint8_t *addr,
                                             uint8_t addr_type,
                                             uint8_t *dst, int dst_len);
int ble_hs_hci_cmd_reset(void);
int ble_hs_hci_cmd_tx_set_ctlr_to_host_fc(uint8_t fc_enable);
int ble_hs_hci_cmd_tx_host_buf_size(const struct hci_host_buf_size *cmd);
int ble_hs_hci_cmd_build_host_num_comp_pkts_entry(
    const struct hci_host_num_comp_pkts_entry *entry,
    uint8_t *dst, int dst_len);
int ble_hs_hci_cmd_read_adv_pwr(void);
int ble_hs_hci_cmd_le_create_conn_cancel(void);
int ble_hs_hci_cmd_build_le_conn_update(const struct hci_conn_update *hcu,
                                        uint8_t *dst, int dst_len);
int ble_hs_hci_cmd_le_conn_update(const struct hci_conn_update *hcu);
void ble_hs_hci_cmd_build_le_lt_key_req_reply(
    const struct hci_lt_key_req_reply *hkr, uint8_t *dst, int dst_len);
void ble_hs_hci_cmd_build_le_lt_key_req_neg_reply(uint16_t conn_handle,
                                                  uint8_t *dst, int dst_len);
void ble_hs_hci_cmd_build_le_conn_param_reply(
    const struct hci_conn_param_reply *hcr, uint8_t *dst, int dst_len);
int ble_hs_hci_cmd_le_conn_param_reply(const struct hci_conn_param_reply *hcr);
void ble_hs_hci_cmd_build_le_conn_param_neg_reply(
    const struct hci_conn_param_neg_reply *hcn, uint8_t *dst, int dst_len);
int ble_hs_hci_cmd_le_conn_param_neg_reply(
    const struct hci_conn_param_neg_reply *hcn);
void ble_hs_hci_cmd_build_le_start_encrypt(const struct hci_start_encrypt *cmd,
                                           uint8_t *dst, int dst_len);
int ble_hs_hci_set_buf_sz(uint16_t pktlen, uint16_t max_pkts);
void ble_hs_hci_add_avail_pkts(uint16_t delta);

uint16_t ble_hs_hci_util_handle_pb_bc_join(uint16_t handle, uint8_t pb,
                                           uint8_t bc);

int ble_hs_hci_acl_tx_now(struct ble_hs_conn *conn, struct os_mbuf **om);
int ble_hs_hci_acl_tx(struct ble_hs_conn *conn, struct os_mbuf **om);

int ble_hs_hci_cmd_build_set_data_len(uint16_t connection_handle,
                                      uint16_t tx_octets, uint16_t tx_time,
                                      uint8_t *dst, int dst_len);
int ble_hs_hci_cmd_build_add_to_resolv_list(
    const struct hci_add_dev_to_resolving_list *padd,
    uint8_t *dst, int dst_len);
int ble_hs_hci_cmd_build_remove_from_resolv_list(
    uint8_t addr_type, const uint8_t *addr, uint8_t *dst, int dst_len);
int ble_hs_hci_cmd_build_read_peer_resolv_addr(
    uint8_t peer_identity_addr_type, const uint8_t *peer_identity_addr,
    uint8_t *dst, int dst_len);
int ble_hs_hci_cmd_build_read_lcl_resolv_addr(
    uint8_t local_identity_addr_type, const uint8_t *local_identity_addr,
    uint8_t *dst, int dst_len);
int ble_hs_hci_cmd_build_set_addr_res_en(
    uint8_t enable, uint8_t *dst, int dst_len);
int ble_hs_hci_cmd_build_set_resolv_priv_addr_timeout(
    uint16_t timeout, uint8_t *dst, int dst_len);
int ble_hs_hci_cmd_build_set_random_addr(const uint8_t *addr,
                                         uint8_t *dst, int dst_len);

#if MYNEWT_VAL(BLE_EXT_ADV)
int ble_hs_hci_cmd_build_le_set_ext_scan_params(uint8_t own_addr_type,
                                               uint8_t filter_policy,
                                               uint8_t phy_mask,
                                               uint8_t phy_count,
                                               struct ble_hs_hci_ext_scan_param *params[],
                                               uint8_t *dst, uint16_t dst_len);

int ble_hs_hci_cmd_build_le_set_ext_scan_enable(uint8_t enable,
                                                uint8_t filter_dups,
                                                uint16_t duration,
                                                uint16_t period,
                                                uint8_t *dst, uint16_t dst_len);

int ble_hs_hci_cmd_build_le_ext_create_conn(const struct hci_ext_create_conn *hcc,
                                            uint8_t *cmd, int cmd_len);

int
ble_hs_hci_cmd_build_le_ext_adv_set_random_addr(uint8_t handle,
                                                const uint8_t *addr,
                                                uint8_t *cmd, int cmd_len);

int
ble_hs_hci_cmd_build_le_ext_adv_data(uint8_t handle, uint8_t operation,
                                     uint8_t frag_pref, struct os_mbuf *data,
                                     uint8_t data_len,
                                     uint8_t *cmd, int cmd_len);

int
ble_hs_hci_cmd_build_le_ext_adv_enable(uint8_t enable, uint8_t sets_num,
                                       const struct hci_ext_adv_set *sets,
                                       uint8_t *cmd, int cmd_len);

int
ble_hs_hci_cmd_build_le_ext_adv_params(uint8_t handle,
                                       const struct hci_ext_adv_params *params,
                                       uint8_t *cmd, int cmd_len);

int
ble_hs_hci_cmd_build_le_ext_adv_remove(uint8_t handle,
                                       uint8_t *cmd, int cmd_len);

#if MYNEWT_VAL(BLE_PERIODIC_ADV)
int
ble_hs_hci_cmd_build_le_periodic_adv_params(uint8_t handle,
                                       const struct hci_periodic_adv_params *params,
                                       uint8_t *cmd, int cmd_len);

int
ble_hs_hci_cmd_build_le_periodic_adv_enable(uint8_t enable,
                                       uint8_t handle,
                                       uint8_t *cmd, int cmd_len);

int
ble_hs_hci_cmd_build_le_periodic_adv_data(uint8_t handle, uint8_t operation,
                                     struct os_mbuf *data,
                                     uint8_t data_len,
                                     uint8_t *cmd, int cmd_len);
int
ble_hs_hci_cmd_build_le_periodic_adv_create_sync(uint8_t filter_policy,
                                        uint8_t adv_sid,
                                        uint8_t adv_add_type,
                                        const uint8_t *adv_addr,
                                        uint16_t skip,
                                        uint16_t sync_timeout,
                                        uint8_t *cmd, int cmd_len);

int
ble_hs_hci_cmd_build_le_periodic_adv_terminate_sync(uint16_t sync_handle,
                                               uint8_t *cmd, int cmd_len);

int
ble_hs_hci_cmd_build_le_add_dev_to_periodic_adv_list(uint8_t adv_add_type,
                                                const uint8_t *adv_addr,
                                                uint8_t adv_sid,
                                                uint8_t *cmd, int cmd_len);
int
ble_hs_hci_cmd_build_le_rem_dev_from_periodic_adv_list(uint8_t adv_add_type,
                                                  const uint8_t *adv_addr,
                                                  uint8_t adv_sid,
                                                  uint8_t *cmd, int cmd_len);
#endif

#endif

int ble_hs_hci_cmd_build_le_enh_recv_test(uint8_t rx_chan, uint8_t phy,
                                          uint8_t mod_idx,
                                          uint8_t *dst, uint16_t dst_len);

int ble_hs_hci_cmd_build_le_enh_trans_test(uint8_t tx_chan,
                                           uint8_t test_data_len,
                                           uint8_t packet_payload_idx,
                                           uint8_t phy,
                                           uint8_t *dst, uint16_t dst_len);

int ble_hs_hci_cmd_build_le_set_priv_mode(const uint8_t *addr, uint8_t addr_type,
                                          uint8_t priv_mode, uint8_t *dst,
                                          uint16_t dst_len);

int ble_hs_hci_cmd_build_le_read_phy(uint16_t conn_handle, uint8_t *dst,
                                     int dst_len);

int ble_hs_hci_cmd_build_le_set_default_phy(uint8_t tx_phys_mask,
                                            uint8_t rx_phys_mask,
                                            uint8_t *dst, int dst_len);

int ble_hs_hci_cmd_build_le_set_phy(uint16_t conn_handle, uint8_t tx_phys_mask,
                                    uint8_t rx_phys_mask, uint16_t phy_opts,
                                    uint8_t *dst, int dst_len);

int ble_hs_hci_frag_num_mbufs(void);
int ble_hs_hci_frag_num_mbufs_free(void);

#if MYNEWT_VAL(BLE_EXT_ADV)
#endif

int ble_hs_hci_cmd_build_le_read_remote_feat(uint16_t handle, uint8_t *dst,
                                                                 int dst_len);
#ifdef __cplusplus
}
#endif

#endif
