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
    const uint8_t *bha_params;
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

int ble_hs_hci_cmd_tx(uint16_t opcode, const void *cmd, uint8_t cmd_len,
                      void *rsp, uint8_t rsp_len);
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
int ble_hs_hci_util_data_hdr_strip(struct os_mbuf *om,
                                   struct hci_data_hdr *out_hdr);
int ble_hs_hci_evt_process(const struct ble_hci_ev *ev);

int ble_hs_hci_cmd_send_buf(uint16_t opcode, const void *buf, uint8_t buf_len);
int ble_hs_hci_set_buf_sz(uint16_t pktlen, uint16_t max_pkts);
void ble_hs_hci_add_avail_pkts(uint16_t delta);

uint16_t ble_hs_hci_util_handle_pb_bc_join(uint16_t handle, uint8_t pb,
                                           uint8_t bc);

int ble_hs_hci_acl_tx_now(struct ble_hs_conn *conn, struct os_mbuf **om);
int ble_hs_hci_acl_tx(struct ble_hs_conn *conn, struct os_mbuf **om);

int ble_hs_hci_frag_num_mbufs(void);
int ble_hs_hci_frag_num_mbufs_free(void);

#ifdef __cplusplus
}
#endif

#endif
