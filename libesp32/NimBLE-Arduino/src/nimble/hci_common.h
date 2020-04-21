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

/*
 * HCI Command Header
 *
 * Comprises the following fields
 *  -> Opcode group field & Opcode command field (2)
 *  -> Parameter Length                          (1)
 *      Length of all the parameters (does not include any part of the hci
 *      command header
 */
#define BLE_HCI_CMD_HDR_LEN                 (3)

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
#define BLE_HCI_OCF_RD_REM_VER_INFO         (0x001D)

/* List of OCF for Controller and Baseband commands (OGF=0x03) */
#define BLE_HCI_OCF_CB_SET_EVENT_MASK       (0x0001)
#define BLE_HCI_OCF_CB_RESET                (0x0003)
#define BLE_HCI_OCF_CB_READ_TX_PWR          (0x002D)
#define BLE_HCI_OCF_CB_SET_CTLR_TO_HOST_FC  (0x0031)
#define BLE_HCI_OCF_CB_HOST_BUF_SIZE        (0x0033)
#define BLE_HCI_OCF_CB_HOST_NUM_COMP_PKTS   (0x0035)
#define BLE_HCI_OCF_CB_SET_EVENT_MASK2      (0x0063)
#define BLE_HCI_OCF_CB_RD_AUTH_PYLD_TMO     (0x007B)
#define BLE_HCI_OCF_CB_WR_AUTH_PYLD_TMO     (0x007C)

/* List of OCF for Info Param commands (OGF=0x04) */
#define BLE_HCI_OCF_IP_RD_LOCAL_VER         (0x0001)
#define BLE_HCI_OCF_IP_RD_LOC_SUPP_CMD      (0x0002)
#define BLE_HCI_OCF_IP_RD_LOC_SUPP_FEAT     (0x0003)
#define BLE_HCI_OCF_IP_RD_BUF_SIZE          (0x0005)
#define BLE_HCI_OCF_IP_RD_BD_ADDR           (0x0009)

/* List of OCF for Status parameters commands (OGF = 0x05) */
#define BLE_HCI_OCF_RD_RSSI                 (0x0005)

/* List of OCF for LE commands (OGF = 0x08) */
#define BLE_HCI_OCF_LE_SET_EVENT_MASK               (0x0001)
#define BLE_HCI_OCF_LE_RD_BUF_SIZE                  (0x0002)
#define BLE_HCI_OCF_LE_RD_LOC_SUPP_FEAT             (0x0003)
/* NOTE: 0x0004 is intentionally left undefined */
#define BLE_HCI_OCF_LE_SET_RAND_ADDR                (0x0005)
#define BLE_HCI_OCF_LE_SET_ADV_PARAMS               (0x0006)
#define BLE_HCI_OCF_LE_RD_ADV_CHAN_TXPWR            (0x0007)
#define BLE_HCI_OCF_LE_SET_ADV_DATA                 (0x0008)
#define BLE_HCI_OCF_LE_SET_SCAN_RSP_DATA            (0x0009)
#define BLE_HCI_OCF_LE_SET_ADV_ENABLE               (0x000A)
#define BLE_HCI_OCF_LE_SET_SCAN_PARAMS              (0x000B)
#define BLE_HCI_OCF_LE_SET_SCAN_ENABLE              (0x000C)
#define BLE_HCI_OCF_LE_CREATE_CONN                  (0x000D)
#define BLE_HCI_OCF_LE_CREATE_CONN_CANCEL           (0x000E)
#define BLE_HCI_OCF_LE_RD_WHITE_LIST_SIZE           (0x000F)
#define BLE_HCI_OCF_LE_CLEAR_WHITE_LIST             (0x0010)
#define BLE_HCI_OCF_LE_ADD_WHITE_LIST               (0x0011)
#define BLE_HCI_OCF_LE_RMV_WHITE_LIST               (0x0012)
#define BLE_HCI_OCF_LE_CONN_UPDATE                  (0x0013)
#define BLE_HCI_OCF_LE_SET_HOST_CHAN_CLASS          (0x0014)
#define BLE_HCI_OCF_LE_RD_CHAN_MAP                  (0x0015)
#define BLE_HCI_OCF_LE_RD_REM_FEAT                  (0x0016)
#define BLE_HCI_OCF_LE_ENCRYPT                      (0x0017)
#define BLE_HCI_OCF_LE_RAND                         (0x0018)
#define BLE_HCI_OCF_LE_START_ENCRYPT                (0x0019)
#define BLE_HCI_OCF_LE_LT_KEY_REQ_REPLY             (0x001A)
#define BLE_HCI_OCF_LE_LT_KEY_REQ_NEG_REPLY         (0x001B)
#define BLE_HCI_OCF_LE_RD_SUPP_STATES               (0x001C)
#define BLE_HCI_OCF_LE_RX_TEST                      (0x001D)
#define BLE_HCI_OCF_LE_TX_TEST                      (0x001E)
#define BLE_HCI_OCF_LE_TEST_END                     (0x001F)
#define BLE_HCI_OCF_LE_REM_CONN_PARAM_RR            (0x0020)
#define BLE_HCI_OCF_LE_REM_CONN_PARAM_NRR           (0x0021)
#define BLE_HCI_OCF_LE_SET_DATA_LEN                 (0x0022)
#define BLE_HCI_OCF_LE_RD_SUGG_DEF_DATA_LEN         (0x0023)
#define BLE_HCI_OCF_LE_WR_SUGG_DEF_DATA_LEN         (0x0024)
#define BLE_HCI_OCF_LE_RD_P256_PUBKEY               (0x0025)
#define BLE_HCI_OCF_LE_GEN_DHKEY                    (0x0026)
#define BLE_HCI_OCF_LE_ADD_RESOLV_LIST              (0x0027)
#define BLE_HCI_OCF_LE_RMV_RESOLV_LIST              (0x0028)
#define BLE_HCI_OCF_LE_CLR_RESOLV_LIST              (0x0029)
#define BLE_HCI_OCF_LE_RD_RESOLV_LIST_SIZE          (0x002A)
#define BLE_HCI_OCF_LE_RD_PEER_RESOLV_ADDR          (0x002B)
#define BLE_HCI_OCF_LE_RD_LOCAL_RESOLV_ADDR         (0x002C)
#define BLE_HCI_OCF_LE_SET_ADDR_RES_EN              (0x002D)
#define BLE_HCI_OCF_LE_SET_RPA_TMO                  (0x002E)
#define BLE_HCI_OCF_LE_RD_MAX_DATA_LEN              (0x002F)
#define BLE_HCI_OCF_LE_RD_PHY                       (0x0030)
#define BLE_HCI_OCF_LE_SET_DEFAULT_PHY              (0x0031)
#define BLE_HCI_OCF_LE_SET_PHY                      (0x0032)
#define BLE_HCI_OCF_LE_ENH_RX_TEST                  (0x0033)
#define BLE_HCI_OCF_LE_ENH_TX_TEST                  (0x0034)
#define BLE_HCI_OCF_LE_SET_ADV_SET_RND_ADDR         (0x0035)
#define BLE_HCI_OCF_LE_SET_EXT_ADV_PARAM            (0x0036)
#define BLE_HCI_OCF_LE_SET_EXT_ADV_DATA             (0x0037)
#define BLE_HCI_OCF_LE_SET_EXT_SCAN_RSP_DATA        (0x0038)
#define BLE_HCI_OCF_LE_SET_EXT_ADV_ENABLE           (0x0039)
#define BLE_HCI_OCF_LE_RD_MAX_ADV_DATA_LEN          (0x003A)
#define BLE_HCI_OCF_LE_RD_NUM_OF_ADV_SETS           (0x003B)
#define BLE_HCI_OCF_LE_REMOVE_ADV_SET               (0x003C)
#define BLE_HCI_OCF_LE_CLEAR_ADV_SETS               (0x003D)
#define BLE_HCI_OCF_LE_SET_PERIODIC_ADV_PARAMS      (0x003E)
#define BLE_HCI_OCF_LE_SET_PERIODIC_ADV_DATA        (0x003F)
#define BLE_HCI_OCF_LE_SET_PERIODIC_ADV_ENABLE      (0x0040)
#define BLE_HCI_OCF_LE_SET_EXT_SCAN_PARAM           (0x0041)
#define BLE_HCI_OCF_LE_SET_EXT_SCAN_ENABLE          (0x0042)
#define BLE_HCI_OCF_LE_EXT_CREATE_CONN              (0x0043)
#define BLE_HCI_OCF_LE_PERIODIC_ADV_CREATE_SYNC          (0x0044)
#define BLE_HCI_OCF_LE_PERIODIC_ADV_CREATE_SYNC_CANCEL   (0x0045)
#define BLE_HCI_OCF_LE_PERIODIC_ADV_TERM_SYNC            (0x0046)
#define BLE_HCI_OCF_LE_ADD_DEV_TO_PERIODIC_ADV_LIST      (0x0047)
#define BLE_HCI_OCF_LE_REM_DEV_FROM_PERIODIC_ADV_LIST    (0x0048)
#define BLE_HCI_OCF_LE_CLEAR_PERIODIC_ADV_LIST           (0x0049)
#define BLE_HCI_OCF_LE_RD_PERIODIC_ADV_LIST_SIZE         (0x004A)
#define BLE_HCI_OCF_LE_RD_TRANSMIT_POWER            (0x004B)
#define BLE_HCI_OCF_LE_RD_RF_PATH_COMPENSATION      (0x004C)
#define BLE_HCI_OCF_LE_WR_RF_PATH_COMPENSATION      (0x004D)
#define BLE_HCI_OCF_LE_SET_PRIVACY_MODE             (0x004E)

/* Command Specific Definitions */
#define BLE_HCI_VARIABLE_LEN                (0xFF)

/* --- Disconnect command (OGF 0x01, OCF 0x0006) --- */
#define BLE_HCI_DISCONNECT_CMD_LEN          (3)

/* --- Set event mask (OGF 0x03, OCF 0x0001 --- */
#define BLE_HCI_SET_EVENT_MASK_LEN          (8)

/* --- Set controller to host flow control (OGF 0x03, OCF 0x0031) --- */
#define BLE_HCI_CTLR_TO_HOST_FC_LEN         (1)

#define BLE_HCI_CTLR_TO_HOST_FC_OFF         (0)
#define BLE_HCI_CTLR_TO_HOST_FC_ACL         (1)
#define BLE_HCI_CTLR_TO_HOST_FC_SYNC        (2)
#define BLE_HCI_CTLR_TO_HOST_FC_BOTH        (3)

/* --- Host buffer size (OGF 0x03, OCF 0x0033) --- */
#define BLE_HCI_HOST_BUF_SIZE_LEN           (7)

/* --- Host number of completed packets (OGF 0x03, OCF 0x0035) --- */
#define BLE_HCI_HOST_NUM_COMP_PKTS_HDR_LEN  (1)
#define BLE_HCI_HOST_NUM_COMP_PKTS_ENT_LEN  (4)

/* --- Read BD_ADDR (OGF 0x04, OCF 0x0009 --- */
#define BLE_HCI_IP_RD_BD_ADDR_ACK_PARAM_LEN (6)

/* --- Read buffer size (OGF 0x04, OCF 0x0005) --- */
#define BLE_HCI_IP_RD_BUF_SIZE_LEN          (0)
#define BLE_HCI_IP_RD_BUF_SIZE_RSPLEN       (7) /* No status byte. */

/* --- Read/Write authenticated payload timeout (ocf 0x007B/0x007C) */
#define BLE_HCI_RD_AUTH_PYLD_TMO_LEN        (4)
#define BLE_HCI_WR_AUTH_PYLD_TMO_LEN        (2)

/* --- Read local version information (OGF 0x04, OCF 0x0001) --- */
#define BLE_HCI_RD_LOC_VER_INFO_RSPLEN      (8) /* No status byte. */

/* --- Read local supported command (OGF 0x04, OCF 0x0002) --- */
#define BLE_HCI_RD_LOC_SUPP_CMD_RSPLEN      (64) /* No status byte. */

/* --- Read local supported features (OGF 0x04, OCF 0x0003) --- */
#define BLE_HCI_RD_LOC_SUPP_FEAT_RSPLEN     (8) /* No status byte. */

/* --- Read RSSI (OGF 0x05, OCF 0x0005) --- */
#define BLE_HCI_READ_RSSI_LEN               (2)
#define BLE_HCI_READ_RSSI_ACK_PARAM_LEN     (3)  /* No status byte. */

/* --- LE set event mask (OCF 0x0001) --- */
#define BLE_HCI_SET_LE_EVENT_MASK_LEN       (8)

/* --- LE read buffer size (OCF 0x0002) --- */
#define BLE_HCI_RD_BUF_SIZE_LEN             (0)
#define BLE_HCI_RD_BUF_SIZE_RSPLEN          (3) /* No status byte. */

/* --- LE read local supported features (OCF 0x0003) --- */
#define BLE_HCI_RD_LE_LOC_SUPP_FEAT_RSPLEN  (8) /* No status byte. */

/* --- LE set random address (OCF 0x0005) */
#define BLE_HCI_SET_RAND_ADDR_LEN           (6)

/* --- LE set advertising parameters (OCF 0x0006) */
#define BLE_HCI_SET_ADV_PARAM_LEN           (15)

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
#define BLE_HCI_ADV_CHAN_TXPWR_ACK_PARAM_LEN   (2)  /* Includes status byte. */
#define BLE_HCI_ADV_CHAN_TXPWR_MIN             (-20)
#define BLE_HCI_ADV_CHAN_TXPWR_MAX             (10)

/* --- LE set advertising data (OCF 0x0008) */
#define BLE_HCI_MAX_ADV_DATA_LEN            (31)
#define BLE_HCI_SET_ADV_DATA_LEN            (32)

/* --- LE set scan response data (OCF 0x0009) */
#define BLE_HCI_MAX_SCAN_RSP_DATA_LEN       (31)
#define BLE_HCI_SET_SCAN_RSP_DATA_LEN       (32)

/* --- LE set advertising enable (OCF 0x000a) */
#define BLE_HCI_SET_ADV_ENABLE_LEN          (1)

/* --- LE set scan enable (OCF 0x000c) */
#define BLE_HCI_SET_SCAN_ENABLE_LEN         (2)

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
#define BLE_HCI_SET_SCAN_PARAM_LEN          (7)
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

/* --- LE connection update (OCF 0x0013) */
#define BLE_HCI_CONN_UPDATE_LEN             (14)

/* --- LE set host channel classification command (OCF 0x0014) */
#define BLE_HCI_SET_HOST_CHAN_CLASS_LEN     (5)

/* --- LE read channel map command (OCF 0x0015) */
#define BLE_HCI_RD_CHANMAP_LEN              (2)
#define BLE_HCI_RD_CHANMAP_RSP_LEN          (7)

/* --- LE read remote features (OCF 0x0016) */
#define BLE_HCI_CONN_RD_REM_FEAT_LEN        (2)

/* --- LE encrypt (OCF 0x0017) */
#define BLE_HCI_LE_ENCRYPT_LEN              (32)

/* --- LE rand (OCF 0x0018) */
#define BLE_HCI_LE_RAND_LEN                 (8)

/* --- LE start encryption (OCF 0x0019) */
#define BLE_HCI_LE_START_ENCRYPT_LEN        (28)

/* ---  LE long term key request reply command (OCF 0x001a) */
#define BLE_HCI_LT_KEY_REQ_REPLY_LEN        (18)
#define BLE_HCI_LT_KEY_REQ_REPLY_ACK_PARAM_LEN (2) /* No status byte. */

/* ---  LE long term key request negative reply command (OCF 0x001b) */
#define BLE_HCI_LT_KEY_REQ_NEG_REPLY_LEN    (2)
#define BLE_HCI_LT_KEY_REQ_NEG_REPLY_ACK_PARAM_LEN (2)

/* --- LE read supported states (OCF 0x001C) --- */
#define BLE_HCI_RD_SUPP_STATES_RSPLEN       (8)

/* --- LE receiver test command (OCF 0x001D) --- */
#define BLE_HCI_RX_TEST_LEN                 (1)

/* --- LE transitter test command (OCF 0x001E) --- */
#define BLE_HCI_TX_TEST_LEN                 (3)

/* --- LE remote connection parameter request reply (OCF 0x0020) */
#define BLE_HCI_CONN_PARAM_REPLY_LEN        (14)

/* --- LE remote connection parameter request negative reply (OCF 0x0021) */
#define BLE_HCI_CONN_PARAM_NEG_REPLY_LEN    (3)

/* --- LE set data length (OCF 0x0022) */
#define BLE_HCI_SET_DATALEN_LEN             (6)
#define BLE_HCI_SET_DATALEN_ACK_PARAM_LEN   (2)  /* No status byte. */
#define BLE_HCI_SET_DATALEN_TX_OCTETS_MIN   (0x001b)
#define BLE_HCI_SET_DATALEN_TX_OCTETS_MAX   (0x00fb)
#define BLE_HCI_SET_DATALEN_TX_TIME_MIN     (0x0148)
#define BLE_HCI_SET_DATALEN_TX_TIME_MAX     (0x4290)

/* --- LE read suggested default data length (OCF 0x0023) */
#define BLE_HCI_RD_SUGG_DATALEN_RSPLEN      (4)

/* --- LE write suggested default data length (OCF 0x0024) */
#define BLE_HCI_WR_SUGG_DATALEN_LEN         (4)

/* --- LE generate DHKEY command (OCF 0x0026) */
#define BLE_HCI_GEN_DHKEY_LEN               (64)

/* --- LE add device to resolving list (OCF 0x0027) */
#define BLE_HCI_ADD_TO_RESOLV_LIST_LEN      (39)

/* --- LE add device to resolving list (OCF 0x0028) */
#define BLE_HCI_RMV_FROM_RESOLV_LIST_LEN    (7)

/* --- LE read peer resolvable address (OCF 0x002B) */
#define BLE_HCI_RD_PEER_RESOLV_ADDR_LEN     (7)

/* --- LE read peer resolvable address (OCF 0x002C) */
#define BLE_HCI_RD_LOC_RESOLV_ADDR_LEN      (7)

/* --- LE set address resolution enable (OCF 0x002D) */
#define BLE_HCI_SET_ADDR_RESOL_ENA_LEN      (1)

/* --- LE set resolvable private address timeout (OCF 0x002E) */
#define BLE_HCI_SET_RESOLV_PRIV_ADDR_TO_LEN (2)

/* --- LE read maximum data length (OCF 0x002F) */
#define BLE_HCI_RD_MAX_DATALEN_RSPLEN       (8)

/* --- LE read maximum default PHY (OCF 0x0030) */
#define BLE_HCI_LE_RD_PHY_LEN               (2)
#define BLE_HCI_LE_RD_PHY_RSPLEN            (4)
#define BLE_HCI_LE_PHY_1M                   (1)
#define BLE_HCI_LE_PHY_2M                   (2)
#define BLE_HCI_LE_PHY_CODED                (3)

/* --- LE set default PHY (OCF 0x0031) */
#define BLE_HCI_LE_SET_DEFAULT_PHY_LEN              (3)
#define BLE_HCI_LE_PHY_NO_TX_PREF_MASK              (0x01)
#define BLE_HCI_LE_PHY_NO_RX_PREF_MASK              (0x02)
#define BLE_HCI_LE_PHY_1M_PREF_MASK                 (0x01)
#define BLE_HCI_LE_PHY_2M_PREF_MASK                 (0x02)
#define BLE_HCI_LE_PHY_CODED_PREF_MASK              (0x04)

#define BLE_HCI_LE_PHY_PREF_MASK_ALL                \
    (BLE_HCI_LE_PHY_1M_PREF_MASK | BLE_HCI_LE_PHY_2M_PREF_MASK |  \
     BLE_HCI_LE_PHY_CODED_PREF_MASK)

/* --- LE set PHY (OCF 0x0032) */
#define BLE_HCI_LE_SET_PHY_LEN                      (7)
#define BLE_HCI_LE_PHY_CODED_ANY                    (0x0000)
#define BLE_HCI_LE_PHY_CODED_S2_PREF                (0x0001)
#define BLE_HCI_LE_PHY_CODED_S8_PREF                (0x0002)

/* --- LE enhanced receiver test (OCF 0x0033) */
#define BLE_HCI_LE_ENH_RX_TEST_LEN                  (3)
#define BLE_HCI_LE_PHY_1M                           (1)
#define BLE_HCI_LE_PHY_2M                           (2)
#define BLE_HCI_LE_PHY_CODED                        (3)

/* --- LE enhanced transmitter test (OCF 0x0034) */
#define BLE_HCI_LE_ENH_TX_TEST_LEN                  (4)
#define BLE_HCI_LE_PHY_CODED_S8                     (3)
#define BLE_HCI_LE_PHY_CODED_S2                     (4)

/* --- LE set advertising set random address (OCF 0x0035) */
#define BLE_HCI_LE_SET_ADV_SET_RND_ADDR_LEN         (7)

/* --- LE set extended advertising parameters (OCF 0x0036) */
#define BLE_HCI_LE_SET_EXT_ADV_PARAM_LEN            (25)
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
#define BLE_HCI_SET_EXT_ADV_DATA_HDR_LEN            (4)

#define BLE_HCI_LE_SET_EXT_ADV_DATA_LEN             BLE_HCI_VARIABLE_LEN
#define BLE_HCI_LE_SET_DATA_OPER_INT        (0)
#define BLE_HCI_LE_SET_DATA_OPER_FIRST      (1)
#define BLE_HCI_LE_SET_DATA_OPER_LAST       (2)
#define BLE_HCI_LE_SET_DATA_OPER_COMPLETE   (3)
#define BLE_HCI_LE_SET_DATA_OPER_UNCHANGED  (4)

/* --- LE set extended scan response data (OCF 0x0038) */
#define BLE_HCI_MAX_EXT_SCAN_RSP_DATA_LEN           (251)
#define BLE_HCI_SET_EXT_SCAN_RSP_DATA_HDR_LEN       (4)

#define BLE_HCI_LE_SET_EXT_SCAN_RSP_DATA_LEN        BLE_HCI_VARIABLE_LEN


/* --- LE set extended advertising enable (OCF 0x0039) */
#define BLE_HCI_LE_SET_EXT_ADV_ENABLE_LEN           BLE_HCI_VARIABLE_LEN

/* --- LE remove advertising set (OCF 0x003C) */
#define BLE_HCI_LE_REMOVE_ADV_SET_LEN               (1)

/* --- LE read maximum advertising data length (OCF 0x003A) */
#define BLE_HCI_RD_MAX_ADV_DATA_LEN                 (2)

/* --- LE read number of supported advertising sets (OCF 0x003B) */
#define BLE_HCI_RD_NR_SUP_ADV_SETS                  (1)

/* --- LE set periodic advertising parameters (OCF 0x003E) */
#define BLE_HCI_LE_SET_PERIODIC_ADV_PARAMS_LEN      (7)
#define BLE_HCI_LE_SET_PERIODIC_ADV_PROP_INC_TX_PWR (0x0040)
#define BLE_HCI_LE_SET_PERIODIC_ADV_PROP_MASK       (0x0040)

/* --- LE set periodic advertising data (OCF 0x003F) */
#define BLE_HCI_LE_SET_PERIODIC_ADV_DATA_LEN             BLE_HCI_VARIABLE_LEN
#define BLE_HCI_MAX_PERIODIC_ADV_DATA_LEN                (252)
#define BLE_HCI_SET_PERIODIC_ADV_DATA_HDR_LEN            (3)

/* --- LE periodic advertising enable (OCF 0x0040) */
#define BLE_HCI_LE_SET_PERIODIC_ADV_ENABLE_LEN           (2)

/* --- LE set extended scan parameters (OCF 0x0041) */
#define BLE_HCI_LE_SET_EXT_SCAN_PARAM_LEN           BLE_HCI_VARIABLE_LEN
#define BLE_HCI_LE_EXT_SCAN_BASE_LEN                (3)
#define BLE_HCI_LE_EXT_SCAN_SINGLE_PARAM_LEN        (5)

/* --- LE set extended scan enable (OCF 0x0042) */
#define BLE_HCI_LE_SET_EXT_SCAN_ENABLE_LEN          (6)

/* --- LE extended create connection (OCF 0x0043) */
#define BLE_HCI_LE_EXT_CREATE_CONN_LEN              BLE_HCI_VARIABLE_LEN
#define BLE_HCI_LE_EXT_CREATE_CONN_BASE_LEN         (10)

/* --- LE periodic advertising create sync (OCF 0x0044) */
#define BLE_HCI_LE_PERIODIC_ADV_CREATE_SYNC_LEN          (14)

/* --- LE periodic advertising terminate (OCF 0x0046) */
#define BLE_HCI_LE_PERIODIC_ADV_TERM_SYNC_LEN            (2)

/* --- LE add device to periodic advertising list (OCF 0x0047) */
#define BLE_HCI_LE_ADD_DEV_TO_PERIODIC_ADV_LIST_LEN      (8)

/* --- LE remove device from periodic advertising list (OCF 0x0048) */
#define BLE_HCI_LE_REM_DEV_FROM_PERIODIC_ADV_LIST_LEN    (8)

#define BLE_HCI_PERIODIC_DATA_STATUS_COMPLETE   0x00
#define BLE_HCI_PERIODIC_DATA_STATUS_INCOMPLETE 0x01
#define BLE_HCI_PERIODIC_DATA_STATUS_TRUNCATED  0x02

/* --- LE write RF path (OCF 0x004D) */
#define BLE_HCI_LE_WR_RF_PATH_COMPENSATION_LEN      (4)

/* --- LE set privacy mode (OCF 0x004E) */
#define BLE_HCI_LE_SET_PRIVACY_MODE_LEN             (8)
#define BLE_HCI_PRIVACY_NETWORK                     (0)
#define BLE_HCI_PRIVACY_DEVICE                      (1)

/* Event Codes */
#define BLE_HCI_EVCODE_INQUIRY_CMP          (0x01)
#define BLE_HCI_EVCODE_INQUIRY_RESULT       (0x02)
#define BLE_HCI_EVCODE_CONN_DONE            (0x03)
#define BLE_HCI_EVCODE_CONN_REQUEST         (0x04)
#define BLE_HCI_EVCODE_DISCONN_CMP          (0x05)
#define BLE_HCI_EVCODE_AUTH_CMP             (0x06)
#define BLE_HCI_EVCODE_REM_NAME_REQ_CMP     (0x07)
#define BLE_HCI_EVCODE_ENCRYPT_CHG          (0x08)
#define BLE_HCI_EVCODE_CHG_LINK_KEY_CMP     (0x09)
#define BLE_HCI_EVCODE_MASTER_LINK_KEY_CMP  (0x0A)
#define BLE_HCI_EVCODE_RD_REM_SUPP_FEAT_CMP (0x0B)
#define BLE_HCI_EVCODE_RD_REM_VER_INFO_CMP  (0x0C)
#define BLE_HCI_EVCODE_QOS_SETUP_CMP        (0x0D)
#define BLE_HCI_EVCODE_COMMAND_COMPLETE     (0x0E)
#define BLE_HCI_EVCODE_COMMAND_STATUS       (0x0F)
#define BLE_HCI_EVCODE_HW_ERROR             (0x10)
#define BLE_HCI_EVCODE_NUM_COMP_PKTS        (0x13)
#define BLE_HCI_EVCODE_MODE_CHANGE          (0x14)
#define BLE_HCI_EVCODE_RETURN_LINK_KEYS     (0x15)
#define BLE_HCI_EVCODE_PIN_CODE_REQ         (0x16)
#define BLE_HCI_EVCODE_LINK_KEY_REQ         (0x17)
#define BLE_HCI_EVCODE_LINK_KEY_NOTIFY      (0x18)
#define BLE_HCI_EVCODE_LOOPBACK_CMD         (0x19)
#define BLE_HCI_EVCODE_DATA_BUF_OVERFLOW    (0x1A)
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
#define BLE_HCI_EVCODE_VENDOR_DEBUG         (0xFF)

/* LE sub-event codes */
#define BLE_HCI_LE_SUBEV_CONN_COMPLETE          (0x01)
#define BLE_HCI_LE_SUBEV_ADV_RPT                (0x02)
#define BLE_HCI_LE_SUBEV_CONN_UPD_COMPLETE      (0x03)
#define BLE_HCI_LE_SUBEV_RD_REM_USED_FEAT       (0x04)
#define BLE_HCI_LE_SUBEV_LT_KEY_REQ             (0x05)
#define BLE_HCI_LE_SUBEV_REM_CONN_PARM_REQ      (0x06)
#define BLE_HCI_LE_SUBEV_DATA_LEN_CHG           (0x07)
#define BLE_HCI_LE_SUBEV_RD_LOC_P256_PUBKEY     (0x08)
#define BLE_HCI_LE_SUBEV_GEN_DHKEY_COMPLETE     (0x09)
#define BLE_HCI_LE_SUBEV_ENH_CONN_COMPLETE      (0x0A)
#define BLE_HCI_LE_SUBEV_DIRECT_ADV_RPT         (0x0B)
#define BLE_HCI_LE_SUBEV_PHY_UPDATE_COMPLETE    (0x0C)
#define BLE_HCI_LE_SUBEV_EXT_ADV_RPT            (0x0D)
#define BLE_HCI_LE_SUBEV_PERIODIC_ADV_SYNC_ESTAB     (0x0E)
#define BLE_HCI_LE_SUBEV_PERIODIC_ADV_RPT            (0x0F)
#define BLE_HCI_LE_SUBEV_PERIODIC_ADV_SYNC_LOST      (0x10)
#define BLE_HCI_LE_SUBEV_SCAN_TIMEOUT           (0x11)
#define BLE_HCI_LE_SUBEV_ADV_SET_TERMINATED     (0x12)
#define BLE_HCI_LE_SUBEV_SCAN_REQ_RCVD          (0x13)
#define BLE_HCI_LE_SUBEV_CHAN_SEL_ALG           (0x14)

/* Generic event header */
#define BLE_HCI_EVENT_HDR_LEN               (2)

/* Event specific definitions */
/* Event disconnect complete */
#define BLE_HCI_EVENT_DISCONN_COMPLETE_LEN  (4)

/* Event encryption change (code=0x08) */
#define BLE_HCI_EVENT_ENCRYPT_CHG_LEN       (4)

/* Event hardware error (code=0x10) */
#define BLE_HCI_EVENT_HW_ERROR_LEN          (1)

/* Event key refresh complete (code=0x30) */
#define BLE_HCI_EVENT_ENC_KEY_REFRESH_LEN   (3)

/* Event command complete */
#define BLE_HCI_EVENT_CMD_COMPLETE_HDR_LEN  (5)
#define BLE_HCI_EVENT_CMD_COMPLETE_MIN_LEN  (6)

/* Event command status */
#define BLE_HCI_EVENT_CMD_STATUS_LEN        (6)

/* Number of completed packets */
#define BLE_HCI_EVENT_NUM_COMP_PKTS_HDR_LEN (1)
#define BLE_HCI_EVENT_NUM_COMP_PKTS_ENT_LEN (4)

/* Read remote version informaton */
#define BLE_HCI_EVENT_RD_RM_VER_LEN         (8)

/* Data buffer overflow event */
#define BLE_HCI_EVENT_DATABUF_OVERFLOW_LEN  (1)
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

/* LE sub-event specific definitions */
#define BLE_HCI_LE_MIN_LEN                  (1) /* Not including event hdr. */

/* LE connection complete event (sub event 0x01) */
#define BLE_HCI_LE_CONN_COMPLETE_LEN            (19)
#define BLE_HCI_LE_CONN_COMPLETE_ROLE_MASTER    (0x00)
#define BLE_HCI_LE_CONN_COMPLETE_ROLE_SLAVE     (0x01)

/* Maximum valid connection handle value */
#define BLE_HCI_LE_CONN_HANDLE_MAX              (0x0eff)

/* LE advertising report event. (sub event 0x02) */
#define BLE_HCI_LE_ADV_RPT_MIN_LEN          (12)
#define BLE_HCI_LE_ADV_DIRECT_RPT_LEN       (18)
#define BLE_HCI_LE_ADV_RPT_NUM_RPTS_MIN     (1)
#define BLE_HCI_LE_ADV_RPT_NUM_RPTS_MAX     (0x19)

/* Length of each record in an LE direct advertising report event. */
#define BLE_HCI_LE_ADV_DIRECT_RPT_SUB_LEN   (16)

/* LE connection update complete event (sub event 0x03) */
#define BLE_HCI_LE_CONN_UPD_LEN             (10)

/* LE long term key request event (sub event 0x05) */
#define BLE_HCI_LE_LT_KEY_REQ_LEN           (13)

/* LE connection update complete event (sub event 0x03) */
#define BLE_HCI_LE_RD_REM_USED_FEAT_LEN     (12)

/* LE remote connection parameter request event (sub event 0x06) */
#define BLE_HCI_LE_REM_CONN_PARM_REQ_LEN    (11)

/* LE data length change event (sub event 0x07) */
#define BLE_HCI_LE_DATA_LEN_CHG_LEN         (11)

/* LE PHY update complete event (sub event 0x0C) */
#define BLE_HCI_LE_PHY_UPD_LEN              (6)

/* LE Periodic Advertising Sync Established Event (sub event 0x0e) */
#define BLE_HCI_LE_PERIODIC_ADV_SYNC_ESTAB_LEN   (16)

/* LE Periodic Advertising Report Event (sub event 0x0f) */
#define BLE_HCI_LE_PERIODIC_ADV_RPT_LEN   (8)

/* LE Periodic Advertising Sync Lost Event (sub event 0x10) */
#define BLE_HCI_LE_PERIODIC_ADV_SYNC_LOST_LEN   (3)

/* LE Scan Timeout Event (sub event 0x11) */
#define BLE_HCI_LE_SUBEV_SCAN_TIMEOUT_LEN   (1)

/*  LE Advertising Set Terminated Event (sub event 0x12) */
#define BLE_HCI_LE_SUBEV_ADV_SET_TERMINATED_LEN   (6)

/* LE Scan Request Received event (sub event 0x13) */
#define BLE_HCI_LE_SUBEV_SCAN_REQ_RCVD_LEN   (9)

/* LE Channel Selection Algorithm event (sub event 0x14) */
#define BLE_HCI_LE_SUBEV_CHAN_SEL_ALG_LEN   (4)

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

/* Sub-event 0x0A: enhanced connection complete */
#define BLE_HCI_LE_ENH_CONN_COMPLETE_LEN    (31)

/*--- Shared data structures ---*/

/* Host buffer size (OGF=0x03, OCF=0x0033) */
struct hci_host_buf_size
{
    uint16_t acl_pkt_len;
    uint8_t sync_pkt_len;
    uint16_t num_acl_pkts;
    uint16_t num_sync_pkts;
};

/* Host number of completed packets (OGF=0x03, OCF=0x0035) */
struct hci_host_num_comp_pkts_entry
{
    uint16_t conn_handle;
    uint16_t num_pkts;
};

/* Read local version information (OGF=0x0004, OCF=0x0001) */
struct hci_loc_ver_info
{
   uint8_t status;
   uint8_t hci_version;
   uint16_t hci_revision;
   uint8_t lmp_pal_version;
   uint16_t mfrg_name;
   uint8_t lmp_pal_subversion;
};

/* set random address command (ocf = 0x0005) */
struct hci_rand_addr
{
    uint8_t addr[6];
};

/* set advertising parameters command (ocf = 0x0006) */
struct hci_adv_params
{
    uint8_t adv_type;
    uint8_t adv_channel_map;
    uint8_t own_addr_type;
    uint8_t peer_addr_type;
    uint8_t adv_filter_policy;
    uint16_t adv_itvl_min;
    uint16_t adv_itvl_max;
    uint8_t peer_addr[BLE_DEV_ADDR_LEN];
};

/* LE create connection command (ocf=0x000d). */
struct hci_create_conn
{
    uint16_t scan_itvl;
    uint16_t scan_window;
    uint8_t filter_policy;
    uint8_t peer_addr_type;
    uint8_t peer_addr[BLE_DEV_ADDR_LEN];
    uint8_t own_addr_type;
    uint16_t conn_itvl_min;
    uint16_t conn_itvl_max;
    uint16_t conn_latency;
    uint16_t supervision_timeout;
    uint16_t min_ce_len;
    uint16_t max_ce_len;
};

/* LE Read Local P-256 Public Key Complete Event */
struct hci_le_subev_rd_loc_p256_pubkey {
    uint8_t status;
    uint8_t pubkey[64];
} __attribute__((packed));

/* LE Generate DHKey Complete Event */
struct hci_le_subev_gen_dhkey_complete {
    uint8_t status;
    uint8_t dhkey[32];
} __attribute__((packed));

/* LE Directed Advertising Report Event */
struct hci_le_subev_direct_adv_rpt_param {
    uint8_t evt_type;
    uint8_t addr_type;
    uint8_t addr[6];
    uint8_t dir_addr_type;
    uint8_t dir_addr[6];
    int8_t rssi;
} __attribute__((packed));

struct hci_le_subev_direct_adv_rpt {
    uint8_t num_reports;
    struct hci_le_subev_direct_adv_rpt_param params[0];
} __attribute__((packed));

#if MYNEWT_VAL(BLE_EXT_ADV)
/* LE create connection command (ocf=0x0043). */
struct hci_ext_conn_params
{
    uint16_t scan_itvl;
    uint16_t scan_window;
    uint16_t conn_itvl_min;
    uint16_t conn_itvl_max;
    uint16_t conn_latency;
    uint16_t supervision_timeout;
    uint16_t min_ce_len;
    uint16_t max_ce_len;
};

struct hci_ext_create_conn
{
    uint8_t filter_policy;
    uint8_t own_addr_type;
    uint8_t peer_addr_type;
    uint8_t peer_addr[BLE_DEV_ADDR_LEN];
    uint8_t init_phy_mask;
    struct hci_ext_conn_params params[3];
};

struct hci_ext_adv_report_param {
    uint16_t evt_type;
    uint8_t addr_type;
    uint8_t addr[6];
    uint8_t prim_phy;
    uint8_t sec_phy;
    uint8_t sid;
    int8_t tx_power;
    int8_t rssi;
    uint16_t per_adv_itvl;
    uint8_t dir_addr_type;
    uint8_t dir_addr[6];
    uint8_t adv_data_len;
    uint8_t adv_data[0];
} __attribute__((packed));

struct hci_ext_adv_report {
    /* We support one report per event for now */
    uint8_t subevt;
    uint8_t num_reports;
    struct hci_ext_adv_report_param params[0];
} __attribute__((packed));

/* Ext Adv Set enable parameters, not in HCI order */
struct hci_ext_adv_set
{
    uint8_t handle;
    uint8_t events;
    uint16_t duration;
};

/* Ext Advertising Parameters */
struct hci_ext_adv_params
{
    uint16_t properties;
    uint32_t min_interval;
    uint32_t max_interval;
    uint8_t chan_map;
    uint8_t own_addr_type;
    uint8_t peer_addr_type;
    uint8_t peer_addr[6];
    uint8_t filter_policy;
    int8_t tx_power;
    uint8_t primary_phy;
    uint8_t max_skip;
    uint8_t secondary_phy;
    uint8_t sid;
    uint8_t scan_req_notif;
};

/* LE Extended Advertising Report Event */
struct hci_le_subev_ext_adv_rpt {
    uint8_t num_reports;
    struct hci_ext_adv_report_param params[0];
} __attribute__((packed));

#if MYNEWT_VAL(BLE_PERIODIC_ADV)
/* LE Periodic Advertising Sync Established Event */
struct hci_le_subev_periodic_adv_sync_estab {
    uint8_t status;
    uint16_t sync_handle;
    uint8_t sid;
    uint8_t adv_addr_type;
    uint8_t adv_addr[6];
    uint8_t adv_phy;
    uint16_t per_adv_ival;
    uint8_t adv_clk_accuracy;
} __attribute__((packed));

/* LE Periodic Advertising Report Event */
struct hci_le_subev_periodic_adv_rpt {
    uint16_t sync_handle;
    int8_t tx_power;
    int8_t rssi;
    uint8_t unused;
    uint8_t data_status;
    uint8_t data_length;
    uint8_t data[0];
} __attribute__((packed));

/* LE Periodic Advertising Sync Lost Event */
struct hci_le_subev_periodic_adv_sync_lost {
    uint16_t sync_handle;
} __attribute__((packed));
#endif

/* LE Advertising Set Terminated Event */
struct hci_le_subev_adv_set_terminated {
    uint8_t status;
    uint8_t adv_handle;
    uint16_t conn_handle;
    uint8_t num_compl_ext_adv_ev;
} __attribute__((packed));

/* LE Scan Request Received Event */
struct hci_le_subev_scan_req_rcvd {
    uint8_t adv_handle;
    uint8_t scan_addr_type;
    uint8_t scan_addr[6];
} __attribute__((packed));

#endif

/* LE Channel Selection Algorithm Event */
struct hci_le_subev_chan_sel_alg {
    uint16_t conn_handle;
    uint8_t chan_sel_alg;
} __attribute__((packed));

/* LE connection update command (ocf=0x0013). */
struct hci_conn_update
{
    uint16_t handle;
    uint16_t conn_itvl_min;
    uint16_t conn_itvl_max;
    uint16_t conn_latency;
    uint16_t supervision_timeout;
    uint16_t min_ce_len;
    uint16_t max_ce_len;
};

/* LE start encryption command (ocf=0x0019) (note: fields out of order). */
struct hci_start_encrypt
{
    uint16_t connection_handle;
    uint16_t encrypted_diversifier;
    uint64_t random_number;
    uint8_t long_term_key[16];
};

/* LE long term key request reply command (ocf=0x001a). */
struct hci_lt_key_req_reply
{
    uint16_t conn_handle;
    uint8_t long_term_key[16];
};

/* LE Remote connection parameter request reply command */
struct hci_conn_param_reply
{
    uint16_t handle;
    uint16_t conn_itvl_min;
    uint16_t conn_itvl_max;
    uint16_t conn_latency;
    uint16_t supervision_timeout;
    uint16_t min_ce_len;
    uint16_t max_ce_len;
};

/* LE Remote connection parameter request negative reply command */
struct hci_conn_param_neg_reply
{
    uint16_t handle;
    uint8_t reason;
};

/* Encryption change event (code=0x08) (note: fields out of order) */
struct hci_encrypt_change
{
    uint8_t status;
    uint8_t encryption_enabled;
    uint16_t connection_handle;
};

/* Encryption key refresh complete event (code=0x30) */
struct hci_encrypt_key_refresh
{
    uint8_t status;
    uint16_t connection_handle;
};

/* Connection complete LE meta subevent */
struct hci_le_conn_complete
{
    uint8_t subevent_code;
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

/* Connection update complete LE meta subevent */
struct hci_le_conn_upd_complete
{
    uint8_t subevent_code;
    uint8_t status;
    uint16_t connection_handle;
    uint16_t conn_itvl;
    uint16_t conn_latency;
    uint16_t supervision_timeout;
};

/* Remote connection parameter request LE meta subevent */
struct hci_le_conn_param_req
{
    uint8_t subevent_code;
    uint16_t connection_handle;
    uint16_t itvl_min;
    uint16_t itvl_max;
    uint16_t latency;
    uint16_t timeout;
};

/* Read Remote Supported Features complete LE meta subevent */
struct hci_le_rd_rem_supp_feat_complete
{
    uint8_t subevent_code;
    uint8_t status;
    uint16_t connection_handle;
    uint8_t features[8];
};

/* LE long term key request event (note: fields out of order). */
struct hci_le_lt_key_req
{
    uint64_t random_number;
    uint16_t connection_handle;
    uint16_t encrypted_diversifier;
    uint8_t subevent_code;
};

/* Disconnection complete event (note: fields out of order). */
struct hci_disconn_complete
{
    uint16_t connection_handle;
    uint8_t status;
    uint8_t reason;
};

/* Read RSSI command-complete parameters (note: fields out of order). */
struct hci_read_rssi_ack_params
{
    uint16_t connection_handle;
    uint8_t status;
    int8_t rssi;
};

/* PHY updated completed LE meta subevent */
struct hci_le_phy_upd_complete
{
    uint8_t subevent_code;
    uint8_t status;
    uint16_t connection_handle;
    uint8_t tx_phy;
    uint8_t rx_phy;
};

/* LE Advertising Set Terminated subevent*/
struct hci_le_adv_set_terminated
{
    uint8_t subevent_code;
    uint8_t status;
    uint8_t adv_handle;
    uint16_t conn_handle;
    uint8_t completed_events;
};

/* LE Scan Request Received subevent */
struct hci_le_scan_req_rcvd
{
    uint8_t subevent_code;
    uint8_t adv_handle;
    uint8_t scan_addr_type;
    uint8_t scan_addr[BLE_DEV_ADDR_LEN];
};

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

struct hci_add_dev_to_resolving_list {
    uint8_t addr_type;
    uint8_t addr[6];
    uint8_t local_irk[16];
    uint8_t peer_irk[16];
};

/* External data structures */
extern const uint8_t g_ble_hci_le_cmd_len[BLE_HCI_NUM_LE_CMDS];

#ifdef __cplusplus
}
#endif

#endif /* H_BLE_HCI_COMMON_ */
