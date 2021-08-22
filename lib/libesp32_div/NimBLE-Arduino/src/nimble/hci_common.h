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

#ifndef H_BLE_HCI_COMMON_
#define H_BLE_HCI_COMMON_

#include "ble.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BLE_HCI_MAX_DATA_LEN (MYNEWT_VAL(BLE_HCI_EVT_BUF_SIZE) - \
                              sizeof(struct ble_hci_ev))

/* Generic command header */
struct ble_hci_cmd {
    uint16_t opcode;
    uint8_t  length;
    uint8_t  data[0];
} __attribute__((packed));

/* Generic event header */
struct ble_hci_ev {
    uint8_t opcode;
    uint8_t length;
    uint8_t  data[0];
} __attribute__((packed));

#define BLE_HCI_OPCODE_NOP                  (0)

/* Set opcode based on OCF and OGF */
#define BLE_HCI_OP(ogf, ocf)            ((ocf) | ((ogf) << 10))

/* Get the OGF and OCF from the opcode in the command */
#define BLE_HCI_OGF(opcode)                 (((opcode) >> 10) & 0x003F)
#define BLE_HCI_OCF(opcode)                 ((opcode) & 0x03FF)

/* Opcode Group definitions (note: 0x07 not defined in spec) */
#define BLE_HCI_OGF_LINK_CTRL               (0x01)
#define BLE_HCI_OGF_LINK_POLICY             (0x02)
#define BLE_HCI_OGF_CTLR_BASEBAND           (0x03)
#define BLE_HCI_OGF_INFO_PARAMS             (0x04)
#define BLE_HCI_OGF_STATUS_PARAMS           (0x05)
#define BLE_HCI_OGF_TESTING                 (0x06)
#define BLE_HCI_OGF_LE                      (0x08)
#define BLE_HCI_OGF_VENDOR                  (0x3F)

/*
 * Number of LE commands. NOTE: this is really just used to size the array
 * containing the lengths of the LE commands.
 */
#define BLE_HCI_NUM_LE_CMDS                 (79)

/* List of OCF for Link Control commands (OGF=0x01) */
#define BLE_HCI_OCF_DISCONNECT_CMD          (0x0006)
struct ble_hci_lc_disconnect_cp {
    uint16_t conn_handle;
    uint8_t  reason;
} __attribute__((packed));

#define BLE_HCI_OCF_RD_REM_VER_INFO         (0x001D)
struct ble_hci_rd_rem_ver_info_cp {
    uint16_t conn_handle;
} __attribute__((packed));

/* List of OCF for Controller and Baseband commands (OGF=0x03) */
#define BLE_HCI_OCF_CB_SET_EVENT_MASK       (0x0001)
struct ble_hci_cb_set_event_mask_cp {
    uint64_t event_mask;
} __attribute__((packed));

#define BLE_HCI_OCF_CB_RESET                (0x0003)

#define BLE_HCI_OCF_CB_READ_TX_PWR          (0x002D)
struct ble_hci_cb_read_tx_pwr_cp {
    uint16_t conn_handle;
    uint8_t  type;
} __attribute__((packed));

struct ble_hci_cb_read_tx_pwr_rp {
    uint16_t conn_handle;
    int8_t   tx_level;
} __attribute__((packed));


#define BLE_HCI_OCF_CB_SET_CTLR_TO_HOST_FC  (0x0031)
struct ble_hci_cb_ctlr_to_host_fc_cp {
    uint8_t enable;
} __attribute__((packed));

#define BLE_HCI_OCF_CB_HOST_BUF_SIZE        (0x0033)
struct ble_hci_cb_host_buf_size_cp {
    uint16_t acl_data_len;
    uint8_t  sco_data_len;
    uint16_t acl_num;
    uint16_t sco_num;
} __attribute__((packed));

#define BLE_HCI_OCF_CB_HOST_NUM_COMP_PKTS   (0x0035)
struct  ble_hci_cb_host_num_comp_pkts_entry {
    uint16_t handle;
    uint16_t count;
} __attribute__((packed));
struct ble_hci_cb_host_num_comp_pkts_cp {
    uint8_t handles;
    struct ble_hci_cb_host_num_comp_pkts_entry h[0];
} __attribute__((packed));

#define BLE_HCI_OCF_CB_SET_EVENT_MASK2      (0x0063)
struct ble_hci_cb_set_event_mask2_cp {
    uint64_t event_mask2;
} __attribute__((packed));

#define BLE_HCI_OCF_CB_RD_AUTH_PYLD_TMO     (0x007B)
struct ble_hci_cb_rd_auth_pyld_tmo_cp {
    uint16_t conn_handle;
} __attribute__((packed));
struct ble_hci_cb_rd_auth_pyld_tmo_rp {
    uint16_t conn_handle;
    uint16_t tmo;
} __attribute__((packed));

#define BLE_HCI_OCF_CB_WR_AUTH_PYLD_TMO     (0x007C)
struct ble_hci_cb_wr_auth_pyld_tmo_cp {
    uint16_t conn_handle;
    uint16_t tmo;
} __attribute__((packed));
struct ble_hci_cb_wr_auth_pyld_tmo_rp {
    uint16_t conn_handle;
} __attribute__((packed));

/* List of OCF for Info Param commands (OGF=0x04) */
#define BLE_HCI_OCF_IP_RD_LOCAL_VER         (0x0001)
struct ble_hci_ip_rd_local_ver_rp {
    uint8_t  hci_ver;
    uint16_t hci_rev;
    uint8_t  lmp_ver;
    uint16_t manufacturer;
    uint16_t lmp_subver;
} __attribute__((packed));

#define BLE_HCI_OCF_IP_RD_LOC_SUPP_CMD      (0x0002)
struct ble_hci_ip_rd_loc_supp_cmd_rp {
    uint8_t commands[64];
} __attribute__((packed));

#define BLE_HCI_OCF_IP_RD_LOC_SUPP_FEAT     (0x0003)
struct ble_hci_ip_rd_loc_supp_feat_rp {
    uint64_t features;
} __attribute__((packed));

#define BLE_HCI_OCF_IP_RD_BUF_SIZE          (0x0005)
struct ble_hci_ip_rd_buf_size_rp {
    uint16_t acl_data_len;
    uint8_t  sco_data_len;
    uint16_t acl_num;
    uint16_t sco_num;
} __attribute__((packed));

#define BLE_HCI_OCF_IP_RD_BD_ADDR           (0x0009)
struct ble_hci_ip_rd_bd_addr_rp {
    uint8_t addr[6];
} __attribute__((packed));

/* List of OCF for Status parameters commands (OGF = 0x05) */
#define BLE_HCI_OCF_RD_RSSI                 (0x0005)
struct ble_hci_rd_rssi_cp {
    uint16_t handle;
} __attribute__((packed));
struct ble_hci_rd_rssi_rp {
    uint16_t handle;
    int8_t   rssi;
} __attribute__((packed));

/* List of OCF for LE commands (OGF = 0x08) */
#define BLE_HCI_OCF_LE_SET_EVENT_MASK               (0x0001)
struct ble_hci_le_set_event_mask_cp {
    uint64_t event_mask;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_BUF_SIZE                  (0x0002)
struct ble_hci_le_rd_buf_size_rp {
    uint16_t data_len;
    uint8_t  data_packets;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_LOC_SUPP_FEAT             (0x0003)
struct ble_hci_le_rd_loc_supp_feat_rp {
    uint64_t features;
} __attribute__((packed));

/* NOTE: 0x0004 is intentionally left undefined */
#define BLE_HCI_OCF_LE_SET_RAND_ADDR                (0x0005)
struct ble_hci_le_set_rand_addr_cp {
    uint8_t addr[6];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_ADV_PARAMS               (0x0006)
struct ble_hci_le_set_adv_params_cp {
    uint16_t min_interval;
    uint16_t max_interval;
    uint8_t type;
    uint8_t own_addr_type;
    uint8_t peer_addr_type;
    uint8_t peer_addr[6];
    uint8_t chan_map;
    uint8_t filter_policy;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_ADV_CHAN_TXPWR            (0x0007)
struct ble_hci_le_rd_adv_chan_txpwr_rp {
    int8_t power_level;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_ADV_DATA                 (0x0008)
#define BLE_HCI_MAX_ADV_DATA_LEN                    (31)
struct ble_hci_le_set_adv_data_cp {
    uint8_t adv_data_len;
    uint8_t adv_data[BLE_HCI_MAX_ADV_DATA_LEN];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_SCAN_RSP_DATA            (0x0009)
#define BLE_HCI_MAX_SCAN_RSP_DATA_LEN               (31)
struct ble_hci_le_set_scan_rsp_data_cp {
    uint8_t scan_rsp_len;
    uint8_t scan_rsp[BLE_HCI_MAX_SCAN_RSP_DATA_LEN];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_ADV_ENABLE               (0x000A)
struct ble_hci_le_set_adv_enable_cp {
    uint8_t enable;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_SCAN_PARAMS              (0x000B)
struct ble_hci_le_set_scan_params_cp {
    uint8_t  scan_type;
    uint16_t scan_itvl;
    uint16_t scan_window;
    uint8_t  own_addr_type;
    uint8_t  filter_policy;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_SCAN_ENABLE              (0x000C)
struct ble_hci_le_set_scan_enable_cp {
    uint8_t enable;
    uint8_t filter_duplicates;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_CREATE_CONN                  (0x000D)
struct ble_hci_le_create_conn_cp {
    uint16_t scan_itvl;
    uint16_t scan_window;
    uint8_t  filter_policy;
    uint8_t  peer_addr_type;
    uint8_t  peer_addr[6];
    uint8_t  own_addr_type;
    uint16_t min_conn_itvl;
    uint16_t max_conn_itvl;
    uint16_t conn_latency;
    uint16_t tmo;
    uint16_t min_ce;
    uint16_t max_ce;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_CREATE_CONN_CANCEL           (0x000E)

#define BLE_HCI_OCF_LE_RD_WHITE_LIST_SIZE           (0x000F)
struct ble_hci_le_rd_white_list_rp {
    uint8_t size;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_CLEAR_WHITE_LIST             (0x0010)

#define BLE_HCI_OCF_LE_ADD_WHITE_LIST               (0x0011)
struct ble_hci_le_add_whte_list_cp {
    uint8_t addr_type;
    uint8_t addr[6];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RMV_WHITE_LIST               (0x0012)
struct ble_hci_le_rmv_white_list_cp {
    uint8_t addr_type;
    uint8_t addr[6];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_CONN_UPDATE                  (0x0013)
struct ble_hci_le_conn_update_cp {
    uint16_t conn_handle;
    uint16_t conn_itvl_min;
    uint16_t conn_itvl_max;
    uint16_t conn_latency;
    uint16_t supervision_timeout;
    uint16_t min_ce_len;
    uint16_t max_ce_len;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_HOST_CHAN_CLASS          (0x0014)
struct ble_hci_le_set_host_chan_class_cp {
    uint8_t chan_map[5];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_CHAN_MAP                  (0x0015)
struct ble_hci_le_rd_chan_map_cp {
    uint16_t conn_handle;
} __attribute__((packed));
struct ble_hci_le_rd_chan_map_rp {
    uint16_t conn_handle;
    uint8_t chan_map[5];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_REM_FEAT                  (0x0016)
struct ble_hci_le_rd_rem_feat_cp {
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_ENCRYPT                      (0x0017)
struct ble_hci_le_encrypt_cp {
    uint8_t key[16];
    uint8_t data[16];
} __attribute__((packed));
struct ble_hci_le_encrypt_rp {
    uint8_t data[16];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RAND                         (0x0018)
struct ble_hci_le_rand_rp {
    uint64_t random_number;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_START_ENCRYPT                (0x0019)
struct ble_hci_le_start_encrypt_cp {
    uint16_t conn_handle;
    uint64_t rand;
    uint16_t div;
    uint8_t  ltk[16];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_LT_KEY_REQ_REPLY             (0x001A)
struct ble_hci_le_lt_key_req_reply_cp {
    uint16_t conn_handle;
    uint8_t  ltk[16];
} __attribute__((packed));
struct ble_hci_le_lt_key_req_reply_rp {
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_LT_KEY_REQ_NEG_REPLY         (0x001B)
struct ble_hci_le_lt_key_req_neg_reply_cp {
    uint16_t conn_handle;
} __attribute__((packed));
struct ble_hci_le_lt_key_req_neg_reply_rp {
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_SUPP_STATES               (0x001C)
struct ble_hci_le_rd_supp_states_rp {
    uint64_t states;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RX_TEST                      (0x001D)
struct ble_hci_le_rx_test_cp {
    uint8_t rx_chan;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_TX_TEST                      (0x001E)
struct ble_hci_le_tx_test_cp {
    uint8_t tx_chan;
    uint8_t test_data_len;
    uint8_t payload;
} __attribute__((packed));
#if MYNEWT_VAL(BLE_LL_DTM_EXTENSIONS)
struct ble_hci_le_tx_test_ext_cp {
    uint8_t tx_chan;
    uint8_t test_data_len;
    uint8_t payload;
    uint16_t interval;
    uint16_t pkt_count;
} __attribute__((packed));
#endif

#define BLE_HCI_OCF_LE_TEST_END                     (0x001F)
struct ble_hci_le_test_end_rp {
    uint16_t num_packets;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_REM_CONN_PARAM_RR            (0x0020)
struct ble_hci_le_rem_conn_param_rr_cp {
    uint16_t conn_handle;
    uint16_t conn_itvl_min;
    uint16_t conn_itvl_max;
    uint16_t conn_latency;
    uint16_t supervision_timeout;
    uint16_t min_ce;
    uint16_t max_ce;
} __attribute__((packed));
struct ble_hci_le_rem_conn_param_rr_rp {
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_REM_CONN_PARAM_NRR           (0x0021)
struct ble_hci_le_rem_conn_params_nrr_cp {
    uint16_t conn_handle;
    uint8_t  reason;
} __attribute__((packed));
struct ble_hci_le_rem_conn_params_nrr_rp {
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_DATA_LEN                 (0x0022)
struct ble_hci_le_set_data_len_cp {
    uint16_t conn_handle;
    uint16_t tx_octets;
    uint16_t tx_time;
} __attribute__((packed));
struct ble_hci_le_set_data_len_rp {
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_SUGG_DEF_DATA_LEN         (0x0023)
struct ble_hci_le_rd_sugg_def_data_len_rp {
    uint16_t max_tx_octets;
    uint16_t max_tx_time;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_WR_SUGG_DEF_DATA_LEN         (0x0024)
struct ble_hci_le_wr_sugg_def_data_len_cp {
    uint16_t max_tx_octets;
    uint16_t max_tx_time;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_P256_PUBKEY               (0x0025)

#define BLE_HCI_OCF_LE_GEN_DHKEY                    (0x0026)
struct ble_hci_le_gen_dhkey_cp {
    uint8_t pkey[64];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_ADD_RESOLV_LIST              (0x0027)
struct ble_hci_le_add_resolv_list_cp {
    uint8_t peer_addr_type;
    uint8_t peer_id_addr[6];
    uint8_t peer_irk[16];
    uint8_t local_irk[16];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RMV_RESOLV_LIST              (0x0028)
struct ble_hci_le_rmv_resolve_list_cp {
    uint8_t peer_addr_type;
    uint8_t peer_id_addr[6];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_CLR_RESOLV_LIST              (0x0029)

#define BLE_HCI_OCF_LE_RD_RESOLV_LIST_SIZE          (0x002A)
struct ble_hci_le_rd_resolv_list_size_rp {
    uint8_t size;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_PEER_RESOLV_ADDR          (0x002B)
struct ble_hci_le_rd_peer_recolv_addr_cp {
    uint8_t peer_addr_type;
    uint8_t peer_id_addr[6];
} __attribute__((packed));
struct ble_hci_le_rd_peer_recolv_addr_rp {
    uint8_t rpa[6];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_LOCAL_RESOLV_ADDR         (0x002C)
struct ble_hci_le_rd_local_recolv_addr_cp {
    uint8_t peer_addr_type;
    uint8_t peer_id_addr[6];
} __attribute__((packed));
struct ble_hci_le_rd_local_recolv_addr_rp {
    uint8_t rpa[6];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_ADDR_RES_EN              (0x002D)
struct ble_hci_le_set_addr_res_en_cp {
    uint8_t enable;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_RPA_TMO                  (0x002E)
struct ble_hci_le_set_rpa_tmo_cp {
    uint16_t rpa_timeout;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_MAX_DATA_LEN              (0x002F)
struct ble_hci_le_rd_max_data_len_rp {
    uint16_t max_tx_octests;
    uint16_t max_tx_time;
    uint16_t max_rx_octests;
    uint16_t max_rx_time;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_PHY                       (0x0030)
struct ble_hci_le_rd_phy_cp {
    uint16_t conn_handle;
} __attribute__((packed));
struct ble_hci_le_rd_phy_rp {
    uint16_t conn_handle;
    uint8_t tx_phy;
    uint8_t rx_phy;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_DEFAULT_PHY              (0x0031)
struct ble_hci_le_set_default_phy_cp {
    uint8_t all_phys;
    uint8_t tx_phys;
    uint8_t rx_phys;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_PHY                      (0x0032)
struct ble_hci_le_set_phy_cp {
    uint16_t conn_handle;
    uint8_t all_phys;
    uint8_t tx_phys;
    uint8_t rx_phys;
    uint16_t phy_options;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RX_TEST_V2                  (0x0033)
struct ble_hci_le_rx_test_v2_cp {
    uint8_t rx_chan;
    uint8_t phy;
    uint8_t index;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_TX_TEST_V2                  (0x0034)
struct ble_hci_le_tx_test_v2_cp {
    uint8_t tx_chan;
    uint8_t test_data_len;
    uint8_t payload;
    uint8_t phy;
} __attribute__((packed));
#if MYNEWT_VAL(BLE_LL_DTM_EXTENSIONS)
struct ble_hci_le_tx_test_v2_ext_cp {
    uint8_t tx_chan;
    uint8_t test_data_len;
    uint8_t payload;
    uint8_t phy;
    uint16_t interval;
    uint16_t pkt_count;
} __attribute__((packed));
#endif

#define BLE_HCI_OCF_LE_SET_ADV_SET_RND_ADDR         (0x0035)
struct ble_hci_le_set_adv_set_rnd_addr_cp {
    uint8_t adv_handle;
    uint8_t addr[6];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_EXT_ADV_PARAM            (0x0036)
struct ble_hci_le_set_ext_adv_params_cp {
    uint8_t  adv_handle;
    uint16_t props;
    uint8_t  pri_itvl_min[3];
    uint8_t  pri_itvl_max[3];
    uint8_t pri_chan_map;
    uint8_t own_addr_type;
    uint8_t peer_addr_type;
    uint8_t peer_addr[6];
    uint8_t filter_policy;
    int8_t tx_power;
    uint8_t pri_phy;
    uint8_t sec_max_skip;
    uint8_t sec_phy;
    uint8_t sid;
    uint8_t scan_req_notif;
} __attribute__((packed));
struct ble_hci_le_set_ext_adv_params_rp {
    int8_t  tx_power;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_EXT_ADV_DATA             (0x0037)
struct ble_hci_le_set_ext_adv_data_cp {
    uint8_t adv_handle;
    uint8_t operation;
    uint8_t fragment_pref;
    uint8_t adv_data_len;
    uint8_t adv_data[0];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_EXT_SCAN_RSP_DATA        (0x0038)
struct ble_hci_le_set_ext_scan_rsp_data_cp {
    uint8_t adv_handle;
    uint8_t operation;
    uint8_t fragment_pref;
    uint8_t scan_rsp_len;
    uint8_t scan_rsp[0];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_EXT_ADV_ENABLE           (0x0039)
struct adv_set {
    uint8_t adv_handle;
    uint16_t duration;
    uint8_t max_events;
} __attribute__((packed));
struct ble_hci_le_set_ext_adv_enable_cp {
    uint8_t enable;
    uint8_t num_sets;
    struct adv_set sets[0];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_MAX_ADV_DATA_LEN          (0x003A)
struct ble_hci_le_rd_max_adv_data_len_rp {
    uint16_t max_adv_data_len;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_NUM_OF_ADV_SETS           (0x003B)
struct ble_hci_le_rd_num_of_adv_sets_rp {
    uint8_t num_sets;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_REMOVE_ADV_SET               (0x003C)
struct ble_hci_le_remove_adv_set_cp {
    uint8_t adv_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_CLEAR_ADV_SETS               (0x003D)

#define BLE_HCI_OCF_LE_SET_PERIODIC_ADV_PARAMS      (0x003E)
struct ble_hci_le_set_periodic_adv_params_cp {
    uint8_t adv_handle;
    uint16_t min_itvl;
    uint16_t max_itvl;
    uint16_t props;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_PERIODIC_ADV_DATA        (0x003F)
struct ble_hci_le_set_periodic_adv_data_cp {
    uint8_t adv_handle;
    uint8_t operation;
    uint8_t adv_data_len;
    uint8_t adv_data[0];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_PERIODIC_ADV_ENABLE      (0x0040)
struct ble_hci_le_set_periodic_adv_enable_cp {
    uint8_t enable;
    uint8_t adv_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_EXT_SCAN_PARAM           (0x0041)
struct scan_params {
    uint8_t  type;
    uint16_t itvl;
    uint16_t window;
} __attribute__((packed));
struct ble_hci_le_set_ext_scan_params_cp {
    uint8_t own_addr_type;
    uint8_t filter_policy;
    uint8_t phys;
    struct scan_params scans[0];
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_EXT_SCAN_ENABLE          (0x0042)
struct ble_hci_le_set_ext_scan_enable_cp {
    uint8_t  enable;
    uint8_t  filter_dup;
    uint16_t duration;
    uint16_t period;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_EXT_CREATE_CONN              (0x0043)
struct conn_params {
    uint16_t scan_itvl;
    uint16_t scan_window;
    uint16_t conn_min_itvl;
    uint16_t conn_max_itvl;
    uint16_t conn_latency;
    uint16_t supervision_timeout;
    uint16_t min_ce;
    uint16_t max_ce;
} __attribute__((packed));
struct ble_hci_le_ext_create_conn_cp {
    uint8_t filter_policy;
    uint8_t own_addr_type;
    uint8_t peer_addr_type;
    uint8_t peer_addr[6];
    uint8_t init_phy_mask;
    struct conn_params conn_params[0];
} __attribute__((packed));

#define BLE_HCI_LE_PERIODIC_ADV_CREATE_SYNC_OPT_FILTER      0x01
#define BLE_HCI_LE_PERIODIC_ADV_CREATE_SYNC_OPT_DISABLED    0x02

#define BLE_HCI_OCF_LE_PERIODIC_ADV_CREATE_SYNC          (0x0044)
struct ble_hci_le_periodic_adv_create_sync_cp {
    uint8_t  options;
    uint8_t  sid;
    uint8_t  peer_addr_type;
    uint8_t  peer_addr[6];
    uint16_t skip;
    uint16_t sync_timeout;
    uint8_t  sync_cte_type;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_PERIODIC_ADV_CREATE_SYNC_CANCEL   (0x0045)

#define BLE_HCI_OCF_LE_PERIODIC_ADV_TERM_SYNC            (0x0046)
struct ble_hci_le_periodic_adv_term_sync_cp {
    uint16_t sync_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_ADD_DEV_TO_PERIODIC_ADV_LIST      (0x0047)
struct ble_hci_le_add_dev_to_periodic_adv_list_cp {
    uint8_t peer_addr_type;
    uint8_t peer_addr[6];
    uint8_t sid;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_REM_DEV_FROM_PERIODIC_ADV_LIST    (0x0048)
struct ble_hci_le_rem_dev_from_periodic_adv_list_cp {
    uint8_t peer_addr_type;
    uint8_t peer_addr[6];
    uint8_t sid;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_CLEAR_PERIODIC_ADV_LIST           (0x0049)

#define BLE_HCI_OCF_LE_RD_PERIODIC_ADV_LIST_SIZE         (0x004A)
struct ble_hci_le_rd_periodic_adv_list_size_rp {
    uint8_t list_size;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_TRANSMIT_POWER            (0x004B)
struct ble_hci_le_rd_transmit_power_rp {
    int8_t min_tx_power;
    int8_t max_tx_power;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RD_RF_PATH_COMPENSATION      (0x004C)
struct ble_hci_le_rd_rf_path_compensation_rp {
    int16_t tx_path_compensation;
    int16_t rx_path_compensation;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_WR_RF_PATH_COMPENSATION      (0x004D)
struct ble_hci_le_wr_rf_path_compensation_cp {
    int16_t tx_path_compensation;
    int16_t rx_path_compensation;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_PRIVACY_MODE             (0x004E)
struct ble_hci_le_set_privacy_mode_cp {
    uint8_t peer_id_addr_type;
    uint8_t peer_id_addr[6];
    uint8_t mode;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_RX_TEST_V3                        (0x004F)
#define BLE_HCI_OCF_LE_TX_TEST_V3                        (0x0050)
#define BLE_HCI_OCF_LE_SET_CONNLESS_CTE_TX_PARAMS        (0x0051)
#define BLE_HCI_OCF_LE_SET_CONNLESS_CTE_TX_ENABLE        (0x0052)
#define BLE_HCI_OCF_LE_SET_CONNLESS_IQ_SAMPLING_ENABLE   (0x0053)
#define BLE_HCI_OCF_LE_SET_CONN_CTE_RX_PARAMS            (0x0054)
#define BLE_HCI_OCF_LE_SET_CONN_CTE_TX_PARAMS            (0x0055)
#define BLE_HCI_OCF_LE_SET_CONN_CTE_REQ_ENABLE           (0x0056)
#define BLE_HCI_OCF_LE_SET_CONN_CTE_RESP_ENABLE          (0x0057)
#define BLE_HCI_OCF_LE_RD_ANTENNA_INFO                   (0x0058)

#define BLE_HCI_OCF_LE_PERIODIC_ADV_RECEIVE_ENABLE       (0x0059)
struct ble_hci_le_periodic_adv_receive_enable_cp {
    uint16_t sync_handle;
    uint8_t enable;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_PERIODIC_ADV_SYNC_TRANSFER        (0x005A)
struct ble_hci_le_periodic_adv_sync_transfer_cp {
    uint16_t conn_handle;
    uint16_t service_data;
    uint16_t sync_handle;
} __attribute__((packed));
struct ble_hci_le_periodic_adv_sync_transfer_rp {
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_PERIODIC_ADV_SET_INFO_TRANSFER    (0x005B)
struct ble_hci_le_periodic_adv_set_info_transfer_cp {
    uint16_t conn_handle;
    uint16_t service_data;
    uint8_t adv_handle;
} __attribute__((packed));
struct ble_hci_le_periodic_adv_set_info_transfer_rp {
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_PERIODIC_ADV_SYNC_TRANSFER_PARAMS (0x005C)
struct ble_hci_le_periodic_adv_sync_transfer_params_cp {
    uint16_t conn_handle;
    uint8_t  mode;
    uint16_t skip;
    uint16_t sync_timeout;
    uint8_t  sync_cte_type;
} __attribute__((packed));
struct ble_hci_le_periodic_adv_sync_transfer_params_rp {
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_SET_DEFAULT_SYNC_TRANSFER_PARAMS  (0x005D)
struct ble_hci_le_set_default_periodic_sync_transfer_params_cp {
    uint8_t  mode;
    uint16_t skip;
    uint16_t sync_timeout;
    uint8_t  sync_cte_type;
} __attribute__((packed));

#define BLE_HCI_OCF_LE_GENERATE_DHKEY_V2                 (0x005E)
#define BLE_HCI_OCF_LE_MODIFY_SCA                        (0x005F)

#define BLE_HCI_OCF_LE_SET_HOST_FEAT                     (0x0074)
struct ble_hci_le_set_host_feat_cp {
    uint8_t bit_num;
    uint8_t val;
} __attribute__((packed));

/* Command Specific Definitions */
/* --- Set controller to host flow control (OGF 0x03, OCF 0x0031) --- */
#define BLE_HCI_CTLR_TO_HOST_FC_OFF         (0)
#define BLE_HCI_CTLR_TO_HOST_FC_ACL         (1)
#define BLE_HCI_CTLR_TO_HOST_FC_SYNC        (2)
#define BLE_HCI_CTLR_TO_HOST_FC_BOTH        (3)

/* --- LE set advertising parameters (OCF 0x0006) */
/* Advertising types */
#define BLE_HCI_ADV_TYPE_ADV_IND            (0)
#define BLE_HCI_ADV_TYPE_ADV_DIRECT_IND_HD  (1)
#define BLE_HCI_ADV_TYPE_ADV_SCAN_IND       (2)
#define BLE_HCI_ADV_TYPE_ADV_NONCONN_IND    (3)
#define BLE_HCI_ADV_TYPE_ADV_DIRECT_IND_LD  (4)
#define BLE_HCI_ADV_TYPE_MAX                (4)

#define BLE_HCI_ADV_CONN_MASK               (0x0001)
#define BLE_HCI_ADV_SCAN_MASK               (0x0002)
#define BLE_HCI_ADV_DIRECT_MASK             (0x0004)
#define BLE_HCI_ADV_SCAN_RSP_MASK           (0x0008)
#define BLE_HCI_ADV_LEGACY_MASK             (0x0010)

#define BLE_HCI_ADV_DATA_STATUS_COMPLETE    (0x0000)
#define BLE_HCI_ADV_DATA_STATUS_INCOMPLETE  (0x0020)
#define BLE_HCI_ADV_DATA_STATUS_TRUNCATED   (0x0040)
#define BLE_HCI_ADV_DATA_STATUS_MASK        (0x0060)

/* Own address types */
#define BLE_HCI_ADV_OWN_ADDR_PUBLIC         (0)
#define BLE_HCI_ADV_OWN_ADDR_RANDOM         (1)
#define BLE_HCI_ADV_OWN_ADDR_PRIV_PUB       (2)
#define BLE_HCI_ADV_OWN_ADDR_PRIV_RAND      (3)
#define BLE_HCI_ADV_OWN_ADDR_MAX            (3)

/* Advertisement peer address Type */
#define BLE_HCI_ADV_PEER_ADDR_PUBLIC        (0)
#define BLE_HCI_ADV_PEER_ADDR_RANDOM        (1)
#define BLE_HCI_ADV_PEER_ADDR_MAX           (1)

/* --- LE advertising channel tx power (OCF 0x0007) */
#define BLE_HCI_ADV_CHAN_TXPWR_MIN             (-20)
#define BLE_HCI_ADV_CHAN_TXPWR_MAX             (10)

/* --- LE set scan enable (OCF 0x000c) */

/* Connect peer address type */
#define BLE_HCI_CONN_PEER_ADDR_PUBLIC        (0)
#define BLE_HCI_CONN_PEER_ADDR_RANDOM        (1)
#define BLE_HCI_CONN_PEER_ADDR_PUBLIC_IDENT  (2)
#define BLE_HCI_CONN_PEER_ADDR_RANDOM_IDENT  (3)
#define BLE_HCI_CONN_PEER_ADDR_MAX           (3)

/*
 * Advertising filter policy
 *
 * Determines how an advertiser filters scan and connection requests.
 *
 *  NONE: no filtering (default value). No whitelist used.
 *  SCAN: process all connection requests but only scans from white list.
 *  CONN: process all scan request but only connection requests from white list
 *  BOTH: ignore all scan and connection requests unless in white list.
 */
#define BLE_HCI_ADV_FILT_NONE               (0)
#define BLE_HCI_ADV_FILT_SCAN               (1)
#define BLE_HCI_ADV_FILT_CONN               (2)
#define BLE_HCI_ADV_FILT_BOTH               (3)
#define BLE_HCI_ADV_FILT_MAX                (3)

#define BLE_HCI_ADV_FILT_DEF                (BLE_HCI_ADV_FILT_NONE)

/* Advertising interval */
#define BLE_HCI_ADV_ITVL                    (625)           /* usecs */
#define BLE_HCI_ADV_ITVL_MIN                (32)            /* units */
#define BLE_HCI_ADV_ITVL_MAX                (16384)         /* units */
#define BLE_HCI_ADV_ITVL_NONCONN_MIN        (160)           /* units */

#define BLE_HCI_ADV_ITVL_DEF                (0x800)         /* 1.28 seconds */
#define BLE_HCI_ADV_CHANMASK_DEF            (0x7)           /* all channels */

/* Set scan parameters */
#define BLE_HCI_SCAN_TYPE_PASSIVE           (0)
#define BLE_HCI_SCAN_TYPE_ACTIVE            (1)

/* Scan interval and scan window timing */
#define BLE_HCI_SCAN_ITVL                   (625)           /* usecs */
#define BLE_HCI_SCAN_ITVL_MIN               (4)             /* units */
#define BLE_HCI_SCAN_ITVL_MAX               (16384)         /* units */
#define BLE_HCI_SCAN_ITVL_DEF               (16)            /* units */
#define BLE_HCI_SCAN_WINDOW_MIN             (4)             /* units */
#define BLE_HCI_SCAN_WINDOW_MAX             (16384)         /* units */
#define BLE_HCI_SCAN_WINDOW_DEF             (16)            /* units */

/*
 * Scanning filter policy
 *  NO_WL:
 *      Scanner processes all advertising packets (white list not used) except
 *      directed, connectable advertising packets not sent to the scanner.
 *  USE_WL:
 *      Scanner processes advertisements from white list only. A connectable,
 *      directed advertisment is ignored unless it contains scanners address.
 *  NO_WL_INITA:
 *      Scanner process all advertising packets (white list not used). A
 *      connectable, directed advertisement shall not be ignored if the InitA
 *      is a resolvable private address.
 *  USE_WL_INITA:
 *      Scanner process advertisements from white list only. A connectable,
 *      directed advertisement shall not be ignored if the InitA is a
 *      resolvable private address.
 */
#define BLE_HCI_SCAN_FILT_NO_WL             (0)
#define BLE_HCI_SCAN_FILT_USE_WL            (1)
#define BLE_HCI_SCAN_FILT_NO_WL_INITA       (2)
#define BLE_HCI_SCAN_FILT_USE_WL_INITA      (3)
#define BLE_HCI_SCAN_FILT_MAX               (3)

/* Whitelist commands */
#define BLE_HCI_ADD_WHITE_LIST_LEN          (7)
#define BLE_HCI_RMV_WHITE_LIST_LEN          (7)

/* Create Connection */
#define BLE_HCI_CREATE_CONN_LEN             (25)
#define BLE_HCI_CONN_ITVL                   (1250)  /* usecs */
#define BLE_HCI_CONN_FILT_NO_WL             (0)
#define BLE_HCI_CONN_FILT_USE_WL            (1)
#define BLE_HCI_CONN_FILT_MAX               (1)
#define BLE_HCI_CONN_ITVL_MIN               (0x0006)
#define BLE_HCI_CONN_ITVL_MAX               (0x0c80)
#define BLE_HCI_CONN_LATENCY_MIN            (0x0000)
#define BLE_HCI_CONN_LATENCY_MAX            (0x01f3)
#define BLE_HCI_CONN_SPVN_TIMEOUT_MIN       (0x000a)
#define BLE_HCI_CONN_SPVN_TIMEOUT_MAX       (0x0c80)
#define BLE_HCI_CONN_SPVN_TMO_UNITS         (10)    /* msecs */
#define BLE_HCI_INITIATOR_FILT_POLICY_MAX   (1)

/* Peer Address Type */
#define BLE_HCI_CONN_PEER_ADDR_PUBLIC       (0)
#define BLE_HCI_CONN_PEER_ADDR_RANDOM       (1)
#define BLE_HCI_CONN_PEER_ADDR_PUB_ID       (2)
#define BLE_HCI_CONN_PEER_ADDR_RAND_ID      (3)
#define BLE_HCI_CONN_PEER_ADDR_MAX          (3)


/* --- LE set data length (OCF 0x0022) */
#define BLE_HCI_SET_DATALEN_TX_OCTETS_MIN   (0x001b)
#define BLE_HCI_SET_DATALEN_TX_OCTETS_MAX   (0x00fb)
#define BLE_HCI_SET_DATALEN_TX_TIME_MIN     (0x0148)
#define BLE_HCI_SET_DATALEN_TX_TIME_MAX     (0x4290)

/* --- LE read maximum default PHY (OCF 0x0030) */
#define BLE_HCI_LE_PHY_1M                   (1)
#define BLE_HCI_LE_PHY_2M                   (2)
#define BLE_HCI_LE_PHY_CODED                (3)

/* --- LE set default PHY (OCF 0x0031) */
#define BLE_HCI_LE_PHY_NO_TX_PREF_MASK              (0x01)
#define BLE_HCI_LE_PHY_NO_RX_PREF_MASK              (0x02)
#define BLE_HCI_LE_PHY_1M_PREF_MASK                 (0x01)
#define BLE_HCI_LE_PHY_2M_PREF_MASK                 (0x02)
#define BLE_HCI_LE_PHY_CODED_PREF_MASK              (0x04)

#define BLE_HCI_LE_PHY_PREF_MASK_ALL                \
    (BLE_HCI_LE_PHY_1M_PREF_MASK | BLE_HCI_LE_PHY_2M_PREF_MASK |  \
     BLE_HCI_LE_PHY_CODED_PREF_MASK)

/* --- LE set PHY (OCF 0x0032) */
#define BLE_HCI_LE_PHY_CODED_ANY                    (0x0000)
#define BLE_HCI_LE_PHY_CODED_S2_PREF                (0x0001)
#define BLE_HCI_LE_PHY_CODED_S8_PREF                (0x0002)

/* --- LE enhanced receiver test (OCF 0x0033) */
#define BLE_HCI_LE_PHY_1M                           (1)
#define BLE_HCI_LE_PHY_2M                           (2)
#define BLE_HCI_LE_PHY_CODED                        (3)

/* --- LE enhanced transmitter test (OCF 0x0034) */
#define BLE_HCI_LE_PHY_CODED_S8                     (3)
#define BLE_HCI_LE_PHY_CODED_S2                     (4)

/* --- LE set extended advertising parameters (OCF 0x0036) */
#define BLE_HCI_LE_SET_EXT_ADV_PROP_CONNECTABLE     (0x0001)
#define BLE_HCI_LE_SET_EXT_ADV_PROP_SCANNABLE       (0x0002)
#define BLE_HCI_LE_SET_EXT_ADV_PROP_DIRECTED        (0x0004)
#define BLE_HCI_LE_SET_EXT_ADV_PROP_HD_DIRECTED     (0x0008)
#define BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY          (0x0010)
#define BLE_HCI_LE_SET_EXT_ADV_PROP_ANON_ADV        (0x0020)
#define BLE_HCI_LE_SET_EXT_ADV_PROP_INC_TX_PWR      (0x0040)
#define BLE_HCI_LE_SET_EXT_ADV_PROP_MASK            (0x7F)

#define BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY_IND      (0x0013)
#define BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY_LD_DIR   (0x0015)
#define BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY_HD_DIR   (0x001d)
#define BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY_SCAN     (0x0012)
#define BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY_NONCONN  (0x0010)

/* --- LE set extended advertising data (OCF 0x0037) */
#define BLE_HCI_MAX_EXT_ADV_DATA_LEN                (251)

#define BLE_HCI_LE_SET_DATA_OPER_INT        (0)
#define BLE_HCI_LE_SET_DATA_OPER_FIRST      (1)
#define BLE_HCI_LE_SET_DATA_OPER_LAST       (2)
#define BLE_HCI_LE_SET_DATA_OPER_COMPLETE   (3)
#define BLE_HCI_LE_SET_DATA_OPER_UNCHANGED  (4)

/* --- LE set extended scan response data (OCF 0x0038) */
#define BLE_HCI_MAX_EXT_SCAN_RSP_DATA_LEN           (251)

/* --- LE set periodic advertising parameters (OCF 0x003E) */
#define BLE_HCI_LE_SET_PERIODIC_ADV_PROP_INC_TX_PWR (0x0040)
#define BLE_HCI_LE_SET_PERIODIC_ADV_PROP_MASK       (0x0040)

/* --- LE set periodic advertising data (OCF 0x003F) */
#define BLE_HCI_MAX_PERIODIC_ADV_DATA_LEN                (252)

/* --- LE remove device from periodic advertising list (OCF 0x0048) */
#define BLE_HCI_PERIODIC_DATA_STATUS_COMPLETE   0x00
#define BLE_HCI_PERIODIC_DATA_STATUS_INCOMPLETE 0x01
#define BLE_HCI_PERIODIC_DATA_STATUS_TRUNCATED  0x02

/* --- LE set privacy mode (OCF 0x004E) */
#define BLE_HCI_PRIVACY_NETWORK                     (0)
#define BLE_HCI_PRIVACY_DEVICE                      (1)

/* Event Codes */
#define BLE_HCI_EVCODE_INQUIRY_CMP          (0x01)
#define BLE_HCI_EVCODE_INQUIRY_RESULT       (0x02)
#define BLE_HCI_EVCODE_CONN_DONE            (0x03)
#define BLE_HCI_EVCODE_CONN_REQUEST         (0x04)
#define BLE_HCI_EVCODE_DISCONN_CMP          (0x05)
struct ble_hci_ev_disconn_cmp {
    uint8_t status;
    uint16_t conn_handle;
    uint8_t reason;
} __attribute__((packed));

#define BLE_HCI_EVCODE_AUTH_CMP             (0x06)
#define BLE_HCI_EVCODE_REM_NAME_REQ_CMP     (0x07)

#define BLE_HCI_EVCODE_ENCRYPT_CHG          (0x08)
struct ble_hci_ev_enrypt_chg {
    uint8_t status;
    uint16_t connection_handle;
    uint8_t enabled;
} __attribute__((packed));

#define BLE_HCI_EVCODE_CHG_LINK_KEY_CMP     (0x09)
#define BLE_HCI_EVCODE_MASTER_LINK_KEY_CMP  (0x0A)
#define BLE_HCI_EVCODE_RD_REM_SUPP_FEAT_CMP (0x0B)
#define BLE_HCI_EVCODE_RD_REM_VER_INFO_CMP  (0x0C)
struct ble_hci_ev_rd_rem_ver_info_cmp {
    uint8_t  status;
    uint16_t conn_handle;
    uint8_t  version;
    uint16_t manufacturer;
    uint16_t subversion;
} __attribute__((packed));

#define BLE_HCI_EVCODE_QOS_SETUP_CMP        (0x0D)

#define BLE_HCI_EVCODE_COMMAND_COMPLETE     (0x0E)
struct ble_hci_ev_command_complete {
    uint8_t  num_packets;
    uint16_t opcode;
    uint8_t  status;
    uint8_t  return_params[0];
} __attribute__((packed));
/* NOP is exception and has no return parameters */
struct ble_hci_ev_command_complete_nop {
    uint8_t  num_packets;
    uint16_t opcode;
} __attribute__((packed));

#define BLE_HCI_EVCODE_COMMAND_STATUS       (0x0F)
struct ble_hci_ev_command_status {
    uint8_t status;
    uint8_t  num_packets;
    uint16_t opcode;
} __attribute__((packed));

#define BLE_HCI_EVCODE_HW_ERROR             (0x10)
struct ble_hci_ev_hw_error {
    uint8_t hw_code;
} __attribute__((packed));

#define BLE_HCI_EVCODE_NUM_COMP_PKTS        (0x13)
struct comp_pkt {
    uint16_t handle;
    uint16_t packets;
} __attribute__((packed));;
struct ble_hci_ev_num_comp_pkts {
    uint8_t count;
    struct comp_pkt completed[0];
} __attribute__((packed));

#define BLE_HCI_EVCODE_MODE_CHANGE          (0x14)
#define BLE_HCI_EVCODE_RETURN_LINK_KEYS     (0x15)
#define BLE_HCI_EVCODE_PIN_CODE_REQ         (0x16)
#define BLE_HCI_EVCODE_LINK_KEY_REQ         (0x17)
#define BLE_HCI_EVCODE_LINK_KEY_NOTIFY      (0x18)
#define BLE_HCI_EVCODE_LOOPBACK_CMD         (0x19)

#define BLE_HCI_EVCODE_DATA_BUF_OVERFLOW    (0x1A)
struct ble_hci_ev_data_buf_overflow {
    uint8_t link_type;
} __attribute__((packed));

#define BLE_HCI_EVCODE_MAX_SLOTS_CHG        (0x1B)
#define BLE_HCI_EVCODE_READ_CLK_OFF_COMP    (0x1C)
#define BLE_HCI_EVCODE_CONN_PKT_TYPE_CHG    (0x1D)
#define BLE_HCI_EVCODE_QOS_VIOLATION        (0x1E)
/* NOTE: 0x1F not defined */
#define BLE_HCI_EVCODE_PSR_MODE_CHG         (0x20)
#define BLE_HCI_EVCODE_FLOW_SPEC_COMP       (0x21)
#define BLE_HCI_EVCODE_INQ_RESULT_RSSI      (0x22)
#define BLE_HCI_EVCODE_READ_REM_EXT_FEAT    (0x23)
/* NOTE: 0x24 - 0x2B not defined */
#define BLE_HCI_EVCODE_SYNCH_CONN_COMP      (0x2C)
#define BLE_HCI_EVCODE_SYNCH_CONN_CHG       (0x2D)
#define BLE_HCI_EVCODE_SNIFF_SUBRATING      (0x2E)
#define BLE_HCI_EVCODE_EXT_INQ_RESULT       (0x2F)

#define BLE_HCI_EVCODE_ENC_KEY_REFRESH      (0x30)
struct ble_hci_ev_enc_key_refresh {
    uint8_t status;
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_EVOCDE_IO_CAP_REQ           (0x31)
#define BLE_HCI_EVCODE_IO_CAP_RSP           (0x32)
#define BLE_HCI_EVCODE_USER_CONFIRM_REQ     (0x33)
#define BLE_HCI_EVCODE_PASSKEY_REQ          (0x34)
#define BLE_HCI_EVCODE_REM_OOB_DATA_REQ     (0x35)
#define BLE_HCI_EVCODE_SIMPLE_PAIR_COMP     (0x36)
/* NOTE: 0x37 not defined */
#define BLE_HCI_EVCODE_LNK_SPVN_TMO_CHG     (0x38)
#define BLE_HCI_EVCODE_ENH_FLUSH_COMP       (0x39)
#define BLE_HCI_EVCODE_USER_PASSKEY_NOTIFY  (0x3B)
#define BLE_HCI_EVCODE_KEYPRESS_NOTIFY      (0x3C)
#define BLE_HCI_EVCODE_REM_HOST_SUPP_FEAT   (0x3D)

#define BLE_HCI_EVCODE_LE_META              (0x3E)
struct ble_hci_ev_le_meta {
    uint8_t subevent;
    uint8_t data[0];
} __attribute__((packed));

/* NOTE: 0x3F not defined */
#define BLE_HCI_EVCODE_PHYS_LINK_COMP       (0x40)
#define BLE_HCI_EVCODE_CHAN_SELECTED        (0x41)
#define BLE_HCI_EVCODE_DISCONN_PHYS_LINK    (0x42)
#define BLE_HCI_EVCODE_PHYS_LINK_LOSS_EARLY (0x43)
#define BLE_HCI_EVCODE_PHYS_LINK_RECOVERY   (0x44)
#define BLE_HCI_EVCODE_LOGICAL_LINK_COMP    (0x45)
#define BLE_HCI_EVCODE_DISCONN_LOGICAL_LINK (0x46)
#define BLE_HCI_EVCODE_FLOW_SPEC_MODE_COMP  (0x47)
#define BLE_HCI_EVCODE_NUM_COMP_DATA_BLKS   (0x48)
#define BLE_HCI_EVCODE_AMP_START_TEST       (0x49)
#define BLE_HCI_EVOCDE_AMP_TEST_END         (0x4A)
#define BLE_HCI_EVOCDE_AMP_RCVR_REPORT      (0x4B)
#define BLE_HCI_EVCODE_SHORT_RANGE_MODE_CHG (0x4C)
#define BLE_HCI_EVCODE_AMP_STATUS_CHG       (0x4D)
#define BLE_HCI_EVCODE_TRIG_CLK_CAPTURE     (0x4E)
#define BLE_HCI_EVCODE_SYNCH_TRAIN_COMP     (0x4F)
#define BLE_HCI_EVCODE_SYNCH_TRAIN_RCVD     (0x50)
#define BLE_HCI_EVCODE_SLAVE_BCAST_RX       (0x51)
#define BLE_HCI_EVCODE_SLAVE_BCAST_TMO      (0x52)
#define BLE_HCI_EVCODE_TRUNC_PAGE_COMP      (0x53)
#define BLE_HCI_EVCODE_SLAVE_PAGE_RSP_TMO   (0x54)
#define BLE_HCI_EVCODE_SLAVE_BCAST_CHAN_MAP (0x55)
#define BLE_HCI_EVCODE_INQ_RSP_NOTIFY       (0x56)

#define BLE_HCI_EVCODE_AUTH_PYLD_TMO        (0x57)
struct ble_hci_ev_auth_pyld_tmo {
    uint16_t conn_handle;
} __attribute__((packed));

#define BLE_HCI_EVCODE_SAM_STATUS_CHG       (0x58)

#define BLE_HCI_EVCODE_VENDOR_DEBUG         (0xFF)
struct ble_hci_ev_vendor_debug {
    uint8_t id;
    uint8_t data[0];
}__attribute__((packed));

/* LE sub-event codes */
#define BLE_HCI_LE_SUBEV_CONN_COMPLETE          (0x01)
struct ble_hci_ev_le_subev_conn_complete {
    uint8_t  subev_code;
    uint8_t  status;
    uint16_t conn_handle;
    uint8_t  role;
    uint8_t  peer_addr_type;
    uint8_t  peer_addr[6];
    uint16_t conn_itvl;
    uint16_t conn_latency;
    uint16_t supervision_timeout;
    uint8_t  mca;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_ADV_RPT                (0x02)
struct adv_report {
    uint8_t type;
    uint8_t addr_type;
    uint8_t addr[6];
    uint8_t data_len;
    uint8_t data[0];
} __attribute__((packed));
struct ble_hci_ev_le_subev_adv_rpt {
    uint8_t  subev_code;
    uint8_t  num_reports;
    struct adv_report reports[0];
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_CONN_UPD_COMPLETE      (0x03)
struct ble_hci_ev_le_subev_conn_upd_complete {
    uint8_t  subev_code;
    uint8_t  status;
    uint16_t conn_handle;
    uint16_t conn_itvl;
    uint16_t conn_latency;
    uint16_t supervision_timeout;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_RD_REM_USED_FEAT       (0x04)
struct ble_hci_ev_le_subev_rd_rem_used_feat {
    uint8_t  subev_code;
    uint8_t  status;
    uint16_t conn_handle;
    uint8_t features[8];
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_LT_KEY_REQ             (0x05)
struct ble_hci_ev_le_subev_lt_key_req {
    uint8_t  subev_code;
    uint16_t conn_handle;
    uint64_t rand;
    uint16_t div;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_REM_CONN_PARM_REQ      (0x06)
struct ble_hci_ev_le_subev_rem_conn_param_req {
    uint8_t  subev_code;
    uint16_t conn_handle;
    uint16_t min_interval;
    uint16_t max_interval;
    uint16_t latency;
    uint16_t timeout;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_DATA_LEN_CHG           (0x07)
struct ble_hci_ev_le_subev_data_len_chg {
    uint8_t  subev_code;
    uint16_t conn_handle;
    uint16_t max_tx_octets;
    uint16_t max_tx_time;
    uint16_t max_rx_octets;
    uint16_t max_rx_time;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_RD_LOC_P256_PUBKEY     (0x08)
struct ble_hci_ev_le_subev_rd_loc_p256_pubkey {
    uint8_t subev_code;
    uint8_t status;
    uint8_t public_key[64];
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_GEN_DHKEY_COMPLETE     (0x09)
struct ble_hci_ev_le_subev_gen_dhkey_complete {
    uint8_t subev_code;
    uint8_t status;
    uint8_t dh_key[32];
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_ENH_CONN_COMPLETE      (0x0A)
struct ble_hci_ev_le_subev_enh_conn_complete {
    uint8_t  subev_code;
    uint8_t  status;
    uint16_t conn_handle;
    uint8_t  role;
    uint8_t  peer_addr_type;
    uint8_t  peer_addr[6];
    uint8_t  local_rpa[6];
    uint8_t  peer_rpa[6];
    uint16_t conn_itvl;
    uint16_t conn_latency;
    uint16_t supervision_timeout;
    uint8_t  mca;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_DIRECT_ADV_RPT         (0x0B)
struct dir_adv_report {
    uint8_t type;
    uint8_t addr_type;
    uint8_t addr[6];
    uint8_t dir_addr_type;
    uint8_t dir_addr[6];
    int8_t  rssi;
} __attribute__((packed));
struct ble_hci_ev_le_subev_direct_adv_rpt {
    uint8_t subev_code;
    uint8_t num_reports;
    struct dir_adv_report reports[0];
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_PHY_UPDATE_COMPLETE    (0x0C)
struct ble_hci_ev_le_subev_phy_update_complete {
    uint8_t  subev_code;
    uint8_t  status;
    uint16_t conn_handle;
    uint8_t  tx_phy;
    uint8_t  rx_phy;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_EXT_ADV_RPT            (0x0D)
struct ext_adv_report {
    uint16_t  evt_type;
    uint8_t  addr_type;
    uint8_t  addr[6];
    uint8_t  pri_phy;
    uint8_t  sec_phy;
    uint8_t  sid;
    int8_t   tx_power;
    int8_t   rssi;
    uint16_t periodic_itvl;
    uint8_t  dir_addr_type;
    uint8_t  dir_addr[6];
    uint8_t  data_len;
    uint8_t  data[0];
} __attribute__((packed));
struct ble_hci_ev_le_subev_ext_adv_rpt {
    uint8_t subev_code;
    uint8_t num_reports;
    struct ext_adv_report reports[0];
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_PERIODIC_ADV_SYNC_ESTAB     (0x0E)
struct ble_hci_ev_le_subev_periodic_adv_sync_estab {
    uint8_t  subev_code;
    uint8_t  status;
    uint16_t sync_handle;
    uint8_t  sid;
    uint8_t  peer_addr_type;
    uint8_t  peer_addr[6];
    uint8_t  phy;
    uint16_t interval;
    uint8_t  aca;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_PERIODIC_ADV_RPT            (0x0F)
struct ble_hci_ev_le_subev_periodic_adv_rpt {
    uint8_t  subev_code;
    uint16_t sync_handle;
    int8_t   tx_power;
    int8_t   rssi;
    uint8_t  cte_type;
    uint8_t  data_status;
    uint8_t  data_len;
    uint8_t  data[0];
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_PERIODIC_ADV_SYNC_LOST      (0x10)
struct ble_hci_ev_le_subev_periodic_adv_sync_lost {
    uint8_t  subev_code;
    uint16_t sync_handle;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_SCAN_TIMEOUT           (0x11)
struct ble_hci_ev_le_subev_scan_timeout {
    uint8_t  subev_code;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_ADV_SET_TERMINATED     (0x12)
struct ble_hci_ev_le_subev_adv_set_terminated {
    uint8_t  subev_code;
    uint8_t  status;
    uint8_t  adv_handle;
    uint16_t conn_handle;
    uint8_t  num_events;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_SCAN_REQ_RCVD          (0x13)
struct ble_hci_ev_le_subev_scan_req_rcvd {
    uint8_t subev_code;
    uint8_t adv_handle;
    uint8_t peer_addr_type;
    uint8_t peer_addr[6];
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_CHAN_SEL_ALG           (0x14)
struct ble_hci_ev_le_subev_chan_sel_alg {
    uint8_t  subev_code;
    uint16_t conn_handle;
    uint8_t  csa;
} __attribute__((packed));

#define BLE_HCI_LE_SUBEV_CONNLESS_IQ_RPT        (0x15)
#define BLE_HCI_LE_SUBEV_CONN_IQ_RPT            (0x16)
#define BLE_HCI_LE_SUBEV_CTE_REQ_FAILED         (0x17)

#define BLE_HCI_LE_SUBEV_PERIODIC_ADV_SYNC_TRANSFER   (0x18)
struct ble_hci_ev_le_subev_periodic_adv_sync_transfer {
    uint8_t  subev_code;
    uint8_t  status;
    uint16_t conn_handle;
    uint16_t service_data;
    uint16_t sync_handle;
    uint8_t  sid;
    uint8_t  peer_addr_type;
    uint8_t  peer_addr[6];
    uint8_t  phy;
    uint16_t interval;
    uint8_t  aca;
} __attribute__((packed));

/* Data buffer overflow event */
#define BLE_HCI_EVENT_ACL_BUF_OVERFLOW      (0x01)

/* Advertising report */
#define BLE_HCI_ADV_RPT_EVTYPE_ADV_IND      (0)
#define BLE_HCI_ADV_RPT_EVTYPE_DIR_IND      (1)
#define BLE_HCI_ADV_RPT_EVTYPE_SCAN_IND     (2)
#define BLE_HCI_ADV_RPT_EVTYPE_NONCONN_IND  (3)
#define BLE_HCI_ADV_RPT_EVTYPE_SCAN_RSP     (4)

/* Bluetooth 5, Vol 2, Part E, 7.7.65.13 */
#define BLE_HCI_LEGACY_ADV_EVTYPE_ADV_IND                 (0x13)
#define BLE_HCI_LEGACY_ADV_EVTYPE_ADV_DIRECT_IND          (0x15)
#define BLE_HCI_LEGACY_ADV_EVTYPE_ADV_SCAN_IND            (0x12)
#define BLE_HCI_LEGACY_ADV_EVTYPE_ADV_NONCON_IND          (0x10)
#define BLE_HCI_LEGACY_ADV_EVTYPE_SCAN_RSP_ADV_IND        (0x1b)
#define BLE_HCI_LEGACY_ADV_EVTYPE_SCAN_RSP_ADV_SCAN_IND   (0x1a)

/* LE connection complete event (sub event 0x01) */
#define BLE_HCI_LE_CONN_COMPLETE_ROLE_MASTER    (0x00)
#define BLE_HCI_LE_CONN_COMPLETE_ROLE_SLAVE     (0x01)

/* Maximum valid connection handle value */
#define BLE_HCI_LE_CONN_HANDLE_MAX              (0x0eff)

/* LE advertising report event. (sub event 0x02) */
#define BLE_HCI_LE_ADV_RPT_NUM_RPTS_MIN     (1)
#define BLE_HCI_LE_ADV_RPT_NUM_RPTS_MAX     (0x19)

/* Bluetooth Assigned numbers for version information.*/
#define BLE_HCI_VER_BCS_1_0b                (0)
#define BLE_HCI_VER_BCS_1_1                 (1)
#define BLE_HCI_VER_BCS_1_2                 (2)
#define BLE_HCI_VER_BCS_2_0_EDR             (3)
#define BLE_HCI_VER_BCS_2_1_EDR             (4)
#define BLE_HCI_VER_BCS_3_0_HCS             (5)
#define BLE_HCI_VER_BCS_4_0                 (6)
#define BLE_HCI_VER_BCS_4_1                 (7)
#define BLE_HCI_VER_BCS_4_2                 (8)
#define BLE_HCI_VER_BCS_5_0                 (9)
#define BLE_HCI_VER_BCS_5_1                 (10)
#define BLE_HCI_VER_BCS_5_2                 (11)

#define BLE_LMP_VER_BCS_1_0b                (0)
#define BLE_LMP_VER_BCS_1_1                 (1)
#define BLE_LMP_VER_BCS_1_2                 (2)
#define BLE_LMP_VER_BCS_2_0_EDR             (3)
#define BLE_LMP_VER_BCS_2_1_EDR             (4)
#define BLE_LMP_VER_BCS_3_0_HCS             (5)
#define BLE_LMP_VER_BCS_4_0                 (6)
#define BLE_LMP_VER_BCS_4_1                 (7)
#define BLE_LMP_VER_BCS_4_2                 (8)
#define BLE_LMP_VER_BCS_5_0                 (9)
#define BLE_LMP_VER_BCS_5_1                 (10)
#define BLE_LMP_VER_BCS_5_2                 (11)

/* selected HCI and LMP version */
#if MYNEWT_VAL(BLE_VERSION) == 50
#define BLE_HCI_VER_BCS BLE_HCI_VER_BCS_5_0
#define BLE_LMP_VER_BCS BLE_LMP_VER_BCS_5_0
#elif MYNEWT_VAL(BLE_VERSION) == 51
#define BLE_HCI_VER_BCS BLE_HCI_VER_BCS_5_1
#define BLE_LMP_VER_BCS BLE_LMP_VER_BCS_5_1
#elif MYNEWT_VAL(BLE_VERSION) == 52
#define BLE_HCI_VER_BCS BLE_HCI_VER_BCS_5_2
#define BLE_LMP_VER_BCS BLE_LMP_VER_BCS_5_2

#endif

#define BLE_HCI_DATA_HDR_SZ                 4
#define BLE_HCI_DATA_HANDLE(handle_pb_bc)   (((handle_pb_bc) & 0x0fff) >> 0)
#define BLE_HCI_DATA_PB(handle_pb_bc)       (((handle_pb_bc) & 0x3000) >> 12)
#define BLE_HCI_DATA_BC(handle_pb_bc)       (((handle_pb_bc) & 0xc000) >> 14)

struct hci_data_hdr
{
    uint16_t hdh_handle_pb_bc;
    uint16_t hdh_len;
};

#define BLE_HCI_PB_FIRST_NON_FLUSH          0
#define BLE_HCI_PB_MIDDLE                   1
#define BLE_HCI_PB_FIRST_FLUSH              2
#define BLE_HCI_PB_FULL                     3

#ifdef __cplusplus
}
#endif

#endif /* H_BLE_HCI_COMMON_ */
