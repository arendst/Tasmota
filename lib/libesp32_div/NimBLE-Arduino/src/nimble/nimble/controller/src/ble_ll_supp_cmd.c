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
#ifndef ESP_PLATFORM

#include <stdint.h>
#include <string.h>

#include "nimble/nimble/include/nimble/ble.h"
#include "nimble/nimble/include/nimble/nimble_opt.h"
#include "nimble/nimble/include/nimble/hci_common.h"
#include "../include/controller/ble_ll.h"
#include "../include/controller/ble_ll_hci.h"

/* Octet 0 */
#define BLE_SUPP_CMD_DISCONNECT             (1 << 5)
#define BLE_LL_SUPP_CMD_OCTET_0             (BLE_SUPP_CMD_DISCONNECT)

/* Octet 5 */
#define BLE_SUPP_CMD_SET_EVENT_MASK         (1 << 6)
#define BLE_LL_SUPP_CMD_OCTET_5             (BLE_SUPP_CMD_SET_EVENT_MASK)

/* Octet 10 */
#define BLE_SUPP_CMD_RD_TX_PWR              (0 << 2)
#define BLE_LL_SUPP_CMD_OCTET_10            (BLE_SUPP_CMD_RD_TX_PWR)

/* Octet 14 */
#define BLE_SUPP_CMD_RD_LOC_VER             (1 << 3)
#define BLE_SUPP_CMD_RD_LOC_SUPP_FEAT       (1 << 5)
#define BLE_LL_SUPP_CMD_OCTET_14            \
(                                           \
    BLE_SUPP_CMD_RD_LOC_VER         |       \
    BLE_SUPP_CMD_RD_LOC_SUPP_FEAT           \
)

/* Octet 15 */
#define BLE_SUPP_CMD_RD_BD_ADDR             (1 << 1)
#define BLE_SUPP_CMD_RD_RSSI                (1 << 5)

#define BLE_LL_SUPP_CMD_OCTET_15            \
(                                           \
    BLE_SUPP_CMD_RD_BD_ADDR         |       \
    BLE_SUPP_CMD_RD_RSSI                    \
)

/* Octet 25 */
#define BLE_SUPP_CMD_LE_SET_EV_MASK         (1 << 0)
#define BLE_SUPP_CMD_LE_RD_BUF_SIZE         (1 << 1)
#define BLE_SUPP_CMD_LE_RD_LOC_FEAT         (1 << 2)
#define BLE_SUPP_CMD_LE_SET_RAND_ADDR       (1 << 4)
#define BLE_SUPP_CMD_LE_SET_ADV_PARAMS      (1 << 5)
#define BLE_SUPP_CMD_LE_SET_ADV_TX_PWR      (1 << 6)
#define BLE_SUPP_CMD_LE_SET_ADV_DATA        (1 << 7)

#define BLE_LL_SUPP_CMD_OCTET_25            \
(                                           \
    BLE_SUPP_CMD_LE_SET_EV_MASK     |       \
    BLE_SUPP_CMD_LE_RD_BUF_SIZE     |       \
    BLE_SUPP_CMD_LE_RD_LOC_FEAT     |       \
    BLE_SUPP_CMD_LE_SET_RAND_ADDR   |       \
    BLE_SUPP_CMD_LE_SET_ADV_PARAMS  |       \
    BLE_SUPP_CMD_LE_SET_ADV_TX_PWR  |       \
    BLE_SUPP_CMD_LE_SET_ADV_DATA            \
)

/* Octet 26 */
#define BLE_SUPP_CMD_LE_SET_SCAN_RSP_DATA   (1 << 0)
#define BLE_SUPP_CMD_LE_SET_ADV_ENABLE      (1 << 1)
#define BLE_SUPP_CMD_LE_SET_SCAN_PARAMS     (1 << 2)
#define BLE_SUPP_CMD_LE_SET_SCAN_ENABLE     (1 << 3)
#define BLE_SUPP_CMD_LE_CREATE_CONN         (1 << 4)
#define BLE_SUPP_CMD_LE_CREATE_CONN_CANCEL  (1 << 5)
#define BLE_SUPP_CMD_LE_RD_WHITELIST_SIZE   (1 << 6)
#define BLE_SUPP_CMD_LE_CLR_WHITELIST       (1 << 7)

#define BLE_LL_SUPP_CMD_OCTET_26            \
(                                           \
    BLE_SUPP_CMD_LE_SET_SCAN_RSP_DATA   |   \
    BLE_SUPP_CMD_LE_SET_ADV_ENABLE      |   \
    BLE_SUPP_CMD_LE_SET_SCAN_PARAMS     |   \
    BLE_SUPP_CMD_LE_SET_SCAN_ENABLE     |   \
    BLE_SUPP_CMD_LE_CREATE_CONN         |   \
    BLE_SUPP_CMD_LE_CREATE_CONN_CANCEL  |   \
    BLE_SUPP_CMD_LE_RD_WHITELIST_SIZE   |   \
    BLE_SUPP_CMD_LE_CLR_WHITELIST           \
)

/* Octet 27 */
#define BLE_SUPP_CMD_LE_ADD_DEV_WHITELIST   (1 << 0)
#define BLE_SUPP_CMD_LE_RMV_DEV_WHITELIST   (1 << 1)
#define BLE_SUPP_CMD_LE_CONN_UPDATE         (1 << 2)
#define BLE_SUPP_CMD_LE_SET_HOST_CHAN_CLASS (1 << 3)
#define BLE_SUPP_CMD_LE_RD_CHAN_MAP         (1 << 4)
#define BLE_SUPP_CMD_LE_RD_REM_USED_FEAT    (1 << 5)
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
#define BLE_SUPP_CMD_LE_ENCRYPT             (1 << 6)
#else
#define BLE_SUPP_CMD_LE_ENCRYPT             (0 << 6)
#endif
#define BLE_SUPP_CMD_LE_RAND                (1 << 7)

#define BLE_LL_SUPP_CMD_OCTET_27            \
(                                           \
    BLE_SUPP_CMD_LE_ENCRYPT             |   \
    BLE_SUPP_CMD_LE_RAND                |   \
    BLE_SUPP_CMD_LE_ADD_DEV_WHITELIST   |   \
    BLE_SUPP_CMD_LE_RMV_DEV_WHITELIST   |   \
    BLE_SUPP_CMD_LE_CONN_UPDATE         |   \
    BLE_SUPP_CMD_LE_SET_HOST_CHAN_CLASS |   \
    BLE_SUPP_CMD_LE_RD_CHAN_MAP         |   \
    BLE_SUPP_CMD_LE_RD_REM_USED_FEAT        \
)

/* Octet 28 */
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
#define BLE_SUPP_CMD_LE_START_ENCRYPT       (1 << 0)
#define BLE_SUPP_CMD_LE_LTK_REQ_REPLY       (1 << 1)
#define BLE_SUPP_CMD_LE_LTK_REQ_NEG_REPLY   (1 << 2)
#else
#define BLE_SUPP_CMD_LE_START_ENCRYPT       (0 << 0)
#define BLE_SUPP_CMD_LE_LTK_REQ_REPLY       (0 << 1)
#define BLE_SUPP_CMD_LE_LTK_REQ_NEG_REPLY   (0 << 2)
#endif
#define BLE_SUPP_CMD_LE_READ_SUPP_STATES    (1 << 3)

#if MYNEWT_VAL(BLE_LL_DTM)
#define BLE_SUPP_CMD_LE_RX_TEST             (1 << 4)
#define BLE_SUPP_CMD_LE_TX_TEST             (1 << 5)
#define BLE_SUPP_CMD_LE_TEST_END            (1 << 6)

#else
#define BLE_SUPP_CMD_LE_RX_TEST             (0 << 4)
#define BLE_SUPP_CMD_LE_TX_TEST             (0 << 5)
#define BLE_SUPP_CMD_LE_TEST_END            (0 << 6)
#endif

#define BLE_LL_SUPP_CMD_OCTET_28            \
(                                           \
    BLE_SUPP_CMD_LE_START_ENCRYPT       |   \
    BLE_SUPP_CMD_LE_LTK_REQ_REPLY       |   \
    BLE_SUPP_CMD_LE_LTK_REQ_NEG_REPLY   |   \
    BLE_SUPP_CMD_LE_READ_SUPP_STATES    |   \
    BLE_SUPP_CMD_LE_RX_TEST             |   \
    BLE_SUPP_CMD_LE_TX_TEST             |   \
    BLE_SUPP_CMD_LE_TEST_END                \
)

/* Octet 33 */
#define BLE_SUPP_CMD_LE_REM_CONN_PRR        (1 << 4)
#define BLE_SUPP_CMD_LE_REM_CONN_PRNR       (1 << 5)
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_DATA_LEN_EXT)
#define BLE_SUPP_CMD_LE_SET_DATALEN         (1 << 6)
#define BLE_SUPP_CMD_LE_RD_SUGG_DATALEN     (1 << 7)
#else
#define BLE_SUPP_CMD_LE_SET_DATALEN         (0 << 6)
#define BLE_SUPP_CMD_LE_RD_SUGG_DATALEN     (0 << 7)
#endif

#define BLE_LL_SUPP_CMD_OCTET_33            \
(                                           \
    BLE_SUPP_CMD_LE_REM_CONN_PRR        |   \
    BLE_SUPP_CMD_LE_REM_CONN_PRNR       |   \
    BLE_SUPP_CMD_LE_SET_DATALEN         |   \
    BLE_SUPP_CMD_LE_RD_SUGG_DATALEN         \
)

/* Octet 34 */
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_DATA_LEN_EXT)
#define BLE_SUPP_CMD_LE_WR_SUGG_DATALEN     (1 << 0)
#else
#define BLE_SUPP_CMD_LE_WR_SUGG_DATALEN     (0 << 0)
#endif
#define BLE_SUPP_CMD_LE_READ_LOCAL_P256_PK  (0 << 1)
#define BLE_SUPP_CMD_LE_GENERATE_DH_KEY     (0 << 2)
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
#define BLE_SUPP_CMD_LE_ADD_RESOLV_LIST     (1 << 3)
#define BLE_SUPP_CMD_LE_REMOVE_RESOLV_LIST  (1 << 4)
#define BLE_SUPP_CMD_LE_CLEAR_RESOLV_LIST   (1 << 5)
#define BLE_SUPP_CMD_LE_RD_RESOLV_SIZE      (1 << 6)
#define BLE_SUPP_CMD_LE_RD_PEER_RESV_ADDR   (1 << 7)
#else
#define BLE_SUPP_CMD_LE_ADD_RESOLV_LIST     (0 << 3)
#define BLE_SUPP_CMD_LE_REMOVE_RESOLV_LIST  (0 << 4)
#define BLE_SUPP_CMD_LE_CLEAR_RESOLV_LIST   (0 << 5)
#define BLE_SUPP_CMD_LE_RD_RESOLV_SIZE      (0 << 6)
#define BLE_SUPP_CMD_LE_RD_PEER_RESV_ADDR   (0 << 7)
#endif

#define BLE_LL_SUPP_CMD_OCTET_34            \
(                                           \
    BLE_SUPP_CMD_LE_WR_SUGG_DATALEN     |   \
    BLE_SUPP_CMD_LE_READ_LOCAL_P256_PK  |   \
    BLE_SUPP_CMD_LE_GENERATE_DH_KEY     |   \
    BLE_SUPP_CMD_LE_ADD_RESOLV_LIST     |   \
    BLE_SUPP_CMD_LE_REMOVE_RESOLV_LIST  |   \
    BLE_SUPP_CMD_LE_CLEAR_RESOLV_LIST   |   \
    BLE_SUPP_CMD_LE_RD_RESOLV_SIZE      |   \
    BLE_SUPP_CMD_LE_RD_PEER_RESV_ADDR       \
)

/* Octet 35 */
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
#define BLE_SUPP_CMD_LE_RD_LOCAL_RESV_ADDR  (1 << 0)
#define BLE_SUPP_CMD_LE_SET_ADDR_RES_EN     (1 << 1)
#define BLE_SUPP_CMD_LE_SET_RESV_ADDR_TMO   (1 << 2)
#else
#define BLE_SUPP_CMD_LE_RD_LOCAL_RESV_ADDR  (0 << 0)
#define BLE_SUPP_CMD_LE_SET_ADDR_RES_EN     (0 << 1)
#define BLE_SUPP_CMD_LE_SET_RESV_ADDR_TMO   (0 << 2)
#endif
#define BLE_SUPP_CMD_LE_RD_MAX_DATALEN      (1 << 3)
#if (BLE_LL_BT5_PHY_SUPPORTED == 1)
#define BLE_SUPP_CMD_LE_READ_PHY            (1 << 4)
#define BLE_SUPP_CMD_LE_SET_DEFAULT_PHY     (1 << 5)
#define BLE_SUPP_CMD_LE_SET_PHY             (1 << 6)
#else
#define BLE_SUPP_CMD_LE_READ_PHY            (0 << 4)
#define BLE_SUPP_CMD_LE_SET_DEFAULT_PHY     (0 << 5)
#define BLE_SUPP_CMD_LE_SET_PHY             (0 << 6)
#endif

#if MYNEWT_VAL(BLE_LL_DTM)
#define BLE_SUPP_CMD_LE_ENHANCED_RX_TEST    (1 << 7)
#else
#define BLE_SUPP_CMD_LE_ENHANCED_RX_TEST    (0 << 7)
#endif

#define BLE_LL_SUPP_CMD_OCTET_35            \
(                                           \
    BLE_SUPP_CMD_LE_RD_LOCAL_RESV_ADDR  |   \
    BLE_SUPP_CMD_LE_SET_ADDR_RES_EN     |   \
    BLE_SUPP_CMD_LE_SET_RESV_ADDR_TMO   |   \
    BLE_SUPP_CMD_LE_RD_MAX_DATALEN      |   \
    BLE_SUPP_CMD_LE_READ_PHY            |   \
    BLE_SUPP_CMD_LE_SET_DEFAULT_PHY     |   \
    BLE_SUPP_CMD_LE_SET_PHY             |   \
    BLE_SUPP_CMD_LE_ENHANCED_RX_TEST        \
)

/* Octet 36 */
#if MYNEWT_VAL(BLE_LL_DTM)
#define BLE_SUPP_CMD_LE_ENHANCED_TX_TEST    (1 << 0)
#else
#define BLE_SUPP_CMD_LE_ENHANCED_TX_TEST    (0 << 0)
#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
#define BLE_SUPP_CMD_LE_SET_ADVS_RAND_ADDR  (1 << 1)
#define BLE_SUPP_CMD_LE_SET_EXT_ADV_PARAM   (1 << 2)
#define BLE_SUPP_CMD_LE_SET_EXT_ADV_DATA    (1 << 3)
#define BLE_SUPP_CMD_LE_SET_EXT_SCAN_RSP    (1 << 4)
#define BLE_SUPP_CMD_LE_SET_EXT_ADV_ENABLE  (1 << 5)
#define BLE_SUPP_CMD_LE_RD_MAX_ADV_DATA_LEN (1 << 6)
#define BLE_SUPP_CMD_LE_RD_NUM_SUPP_ADVS    (1 << 7)
#else
#define BLE_SUPP_CMD_LE_SET_ADVS_RAND_ADDR  (0 << 1)
#define BLE_SUPP_CMD_LE_SET_EXT_ADV_PARAM   (0 << 2)
#define BLE_SUPP_CMD_LE_SET_EXT_ADV_DATA    (0 << 3)
#define BLE_SUPP_CMD_LE_SET_EXT_SCAN_RSP    (0 << 4)
#define BLE_SUPP_CMD_LE_SET_EXT_ADV_ENABLE  (0 << 5)
#define BLE_SUPP_CMD_LE_RD_MAX_ADV_DATA_LEN (0 << 6)
#define BLE_SUPP_CMD_LE_RD_NUM_SUPP_ADVS    (0 << 7)
#endif

#define BLE_LL_SUPP_CMD_OCTET_36            \
(                                           \
    BLE_SUPP_CMD_LE_ENHANCED_TX_TEST    |   \
    BLE_SUPP_CMD_LE_SET_ADVS_RAND_ADDR  |   \
    BLE_SUPP_CMD_LE_SET_EXT_ADV_PARAM   |   \
    BLE_SUPP_CMD_LE_SET_EXT_ADV_DATA    |   \
    BLE_SUPP_CMD_LE_SET_EXT_SCAN_RSP    |   \
    BLE_SUPP_CMD_LE_SET_EXT_ADV_ENABLE  |   \
    BLE_SUPP_CMD_LE_RD_MAX_ADV_DATA_LEN |   \
    BLE_SUPP_CMD_LE_RD_NUM_SUPP_ADVS        \
)

/* Octet 37 */
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
#define BLE_SUPP_CMD_LE_REMOVE_ADVS         (1 << 0)
#define BLE_SUPP_CMD_LE_CLEAR_ADVS          (1 << 1)
#else
#define BLE_SUPP_CMD_LE_REMOVE_ADVS         (0 << 0)
#define BLE_SUPP_CMD_LE_CLEAR_ADVS          (0 << 1)
#endif
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV)
#define BLE_SUPP_CMD_LE_SET_PADV_PARAM      (1 << 2)
#define BLE_SUPP_CMD_LE_SET_PADV_DATA       (1 << 3)
#define BLE_SUPP_CMD_LE_SET_PADV_ENABLE     (1 << 4)
#else
#define BLE_SUPP_CMD_LE_SET_PADV_PARAM      (0 << 2)
#define BLE_SUPP_CMD_LE_SET_PADV_DATA       (0 << 3)
#define BLE_SUPP_CMD_LE_SET_PADV_ENABLE     (0 << 4)
#endif
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
#define BLE_SUPP_CMD_LE_SET_EXT_SCAN_PARAM  (1 << 5)
#define BLE_SUPP_CMD_LE_SET_EXT_SCAN_ENABLE (1 << 6)
#define BLE_SUPP_CMD_LE_EXT_CREATE_CONN     (1 << 7)
#else
#define BLE_SUPP_CMD_LE_SET_EXT_SCAN_PARAM  (0 << 5)
#define BLE_SUPP_CMD_LE_SET_EXT_SCAN_ENABLE (0 << 6)
#define BLE_SUPP_CMD_LE_EXT_CREATE_CONN     (0 << 7)
#endif

#define BLE_LL_SUPP_CMD_OCTET_37            \
(                                           \
    BLE_SUPP_CMD_LE_REMOVE_ADVS         |   \
    BLE_SUPP_CMD_LE_CLEAR_ADVS          |   \
    BLE_SUPP_CMD_LE_SET_PADV_PARAM      |   \
    BLE_SUPP_CMD_LE_SET_PADV_DATA       |   \
    BLE_SUPP_CMD_LE_SET_PADV_ENABLE     |   \
    BLE_SUPP_CMD_LE_SET_EXT_SCAN_PARAM  |   \
    BLE_SUPP_CMD_LE_SET_EXT_SCAN_ENABLE |   \
    BLE_SUPP_CMD_LE_EXT_CREATE_CONN         \
)

/* Octet 38 */
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV)
#define BLE_SUPP_CMD_LE_PADV_CREATE_SYNC    (1 << 0)
#define BLE_SUPP_CMD_LE_PADV_CREATE_SYNC_C  (1 << 1)
#define BLE_SUPP_CMD_LE_PADV_TERMINATE_SYNC (1 << 2)
#define BLE_SUPP_CMD_LE_ADD_PADV_LIST       (1 << 3)
#define BLE_SUPP_CMD_LE_REMOVE_PADV_LIST    (1 << 4)
#define BLE_SUPP_CMD_LE_CLEAR_PADV_LIST     (1 << 5)
#define BLE_SUPP_CMD_LE_RD_PADV_LIST_SIZE   (1 << 6)
#else
#define BLE_SUPP_CMD_LE_PADV_CREATE_SYNC    (0 << 0)
#define BLE_SUPP_CMD_LE_PADV_CREATE_SYNC_C  (0 << 1)
#define BLE_SUPP_CMD_LE_PADV_TERMINATE_SYNC (0 << 2)
#define BLE_SUPP_CMD_LE_ADD_PADV_LIST       (0 << 3)
#define BLE_SUPP_CMD_LE_REMOVE_PADV_LIST    (0 << 4)
#define BLE_SUPP_CMD_LE_CLEAR_PADV_LIST     (0 << 5)
#define BLE_SUPP_CMD_LE_RD_PADV_LIST_SIZE   (0 << 6)
#endif
#define BLE_SUPP_CMD_LE_RD_TX_POWER         (1 << 7)

#define BLE_LL_SUPP_CMD_OCTET_38            \
(                                           \
    BLE_SUPP_CMD_LE_PADV_CREATE_SYNC    |   \
    BLE_SUPP_CMD_LE_PADV_CREATE_SYNC_C  |   \
    BLE_SUPP_CMD_LE_PADV_TERMINATE_SYNC |   \
    BLE_SUPP_CMD_LE_ADD_PADV_LIST       |   \
    BLE_SUPP_CMD_LE_REMOVE_PADV_LIST    |   \
    BLE_SUPP_CMD_LE_CLEAR_PADV_LIST     |   \
    BLE_SUPP_CMD_LE_RD_PADV_LIST_SIZE   |   \
    BLE_SUPP_CMD_LE_RD_TX_POWER             \
)

/* Octet 39 */
#define BLE_SUPP_CMD_LE_RD_RF_PATH_COMP     (1 << 0)
#define BLE_SUPP_CMD_LE_WR_RF_PATH_COMP     (1 << 1)
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
#define BLE_SUPP_CMD_LE_SET_PRIVACY_MODE    (1 << 2)
#else
#define BLE_SUPP_CMD_LE_SET_PRIVACY_MODE    (0 << 2)
#endif

#define BLE_LL_SUPP_CMD_OCTET_39            \
(                                           \
    BLE_SUPP_CMD_LE_RD_RF_PATH_COMP     |   \
    BLE_SUPP_CMD_LE_WR_RF_PATH_COMP     |   \
    BLE_SUPP_CMD_LE_SET_PRIVACY_MODE        \
)

/* Octet 40 */
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV) && MYNEWT_VAL(BLE_VERSION) >= 51
#define BLE_SUPP_CMD_LE_PADV_RECV_ENABLE (1 << 5)
#else
#define BLE_SUPP_CMD_LE_PADV_RECV_ENABLE (0 << 5)
#endif
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV_SYNC_TRANSFER)
#define BLE_SUPP_CMD_LE_PADV_SYNC_TRANSFER (1 << 6)
#define BLE_SUPP_CMD_LE_PADV_SET_INFO_TRANSFER (1 << 7)
#else
#define BLE_SUPP_CMD_LE_PADV_SYNC_TRANSFER (0 << 6)
#define BLE_SUPP_CMD_LE_PADV_SET_INFO_TRANSFER (0 << 7)
#endif

#define BLE_LL_SUPP_CMD_OCTET_40             \
(                                            \
    BLE_SUPP_CMD_LE_PADV_RECV_ENABLE       | \
    BLE_SUPP_CMD_LE_PADV_SYNC_TRANSFER     | \
    BLE_SUPP_CMD_LE_PADV_SET_INFO_TRANSFER   \
)

/* Octet 41 */
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV_SYNC_TRANSFER)
#define BLE_SUPP_CMD_LE_PADV_SYNC_TRANSFER_PARAMS (1 << 0)
#define BLE_SUPP_CMD_LE_PADV_DEFAULT_SYNC_TRANSFER_PARAMS (1 << 1)
#else
#define BLE_SUPP_CMD_LE_PADV_SYNC_TRANSFER_PARAMS (0 << 0)
#define BLE_SUPP_CMD_LE_PADV_DEFAULT_SYNC_TRANSFER_PARAMS (0 << 1)
#endif
#define BLE_LL_SUPP_CMD_OCTET_41                        \
(                                                       \
    BLE_SUPP_CMD_LE_PADV_SYNC_TRANSFER_PARAMS |         \
    BLE_SUPP_CMD_LE_PADV_DEFAULT_SYNC_TRANSFER_PARAMS   \
)

/* Defines the array of supported commands */
const uint8_t g_ble_ll_supp_cmds[BLE_LL_SUPP_CMD_LEN] =
{
    BLE_LL_SUPP_CMD_OCTET_0,            /* Octet 0 */
    0,
    0,
    0,
    0,
    BLE_LL_SUPP_CMD_OCTET_5,
    0,
    0,
    0,                                  /* Octet 8 */
    0,
    BLE_LL_SUPP_CMD_OCTET_10,
    0,
    0,
    0,
    BLE_LL_SUPP_CMD_OCTET_14,
    BLE_LL_SUPP_CMD_OCTET_15,
    0,                                  /* Octet 16 */
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,                                 /* Octet 24 */
    BLE_LL_SUPP_CMD_OCTET_25,
    BLE_LL_SUPP_CMD_OCTET_26,
    BLE_LL_SUPP_CMD_OCTET_27,
    BLE_LL_SUPP_CMD_OCTET_28,
    0,
    0,
    0,
    0,                                  /* Octet 32 */
    BLE_LL_SUPP_CMD_OCTET_33,
    BLE_LL_SUPP_CMD_OCTET_34,
    BLE_LL_SUPP_CMD_OCTET_35,
    BLE_LL_SUPP_CMD_OCTET_36,
    BLE_LL_SUPP_CMD_OCTET_37,
    BLE_LL_SUPP_CMD_OCTET_38,
    BLE_LL_SUPP_CMD_OCTET_39,
    BLE_LL_SUPP_CMD_OCTET_40,           /* Octet 40 */
    BLE_LL_SUPP_CMD_OCTET_41,
};

#endif
