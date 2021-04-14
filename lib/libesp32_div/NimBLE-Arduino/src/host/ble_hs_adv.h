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

#ifndef H_BLE_HS_ADV_
#define H_BLE_HS_ADV_

#include <inttypes.h>
#include "host/ble_uuid.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BLE_HS_ADV_MAX_SZ           BLE_HCI_MAX_ADV_DATA_LEN

/** Max field payload size (account for 2-byte header). */
#define BLE_HS_ADV_MAX_FIELD_SZ     (BLE_HS_ADV_MAX_SZ - 2)

struct ble_hs_adv_field {
    uint8_t length;
    uint8_t type;
    uint8_t value[];
};

typedef int (* ble_hs_adv_parse_func_t) (const struct ble_hs_adv_field *,
                                         void *);

struct ble_hs_adv_fields {
    /*** 0x01 - Flags. */
    uint8_t flags;

    /*** 0x02,0x03 - 16-bit service class UUIDs. */
    ble_uuid16_t *uuids16;
    uint8_t num_uuids16;
    unsigned uuids16_is_complete:1;

    /*** 0x04,0x05 - 32-bit service class UUIDs. */
    ble_uuid32_t *uuids32;
    uint8_t num_uuids32;
    unsigned uuids32_is_complete:1;

    /*** 0x06,0x07 - 128-bit service class UUIDs. */
    ble_uuid128_t *uuids128;
    uint8_t num_uuids128;
    unsigned uuids128_is_complete:1;

    /*** 0x08,0x09 - Local name. */
    uint8_t *name;
    uint8_t name_len;
    unsigned name_is_complete:1;

    /*** 0x0a - Tx power level. */
    int8_t tx_pwr_lvl;
    unsigned tx_pwr_lvl_is_present:1;

    /*** 0x0d - Slave connection interval range. */
    uint8_t *slave_itvl_range;

    /*** 0x16 - Service data - 16-bit UUID. */
    uint8_t *svc_data_uuid16;
    uint8_t svc_data_uuid16_len;

    /*** 0x17 - Public target address. */
    uint8_t *public_tgt_addr;
    uint8_t num_public_tgt_addrs;

    /*** 0x19 - Appearance. */
    uint16_t appearance;
    unsigned appearance_is_present:1;

    /*** 0x1a - Advertising interval. */
    uint16_t adv_itvl;
    unsigned adv_itvl_is_present:1;

    /*** 0x20 - Service data - 32-bit UUID. */
    uint8_t *svc_data_uuid32;
    uint8_t svc_data_uuid32_len;

    /*** 0x21 - Service data - 128-bit UUID. */
    uint8_t *svc_data_uuid128;
    uint8_t svc_data_uuid128_len;

    /*** 0x24 - URI. */
    uint8_t *uri;
    uint8_t uri_len;

    /*** 0xff - Manufacturer specific data. */
    uint8_t *mfg_data;
    uint8_t mfg_data_len;
};

#define BLE_HS_ADV_TYPE_FLAGS                   0x01
#define BLE_HS_ADV_TYPE_INCOMP_UUIDS16          0x02
#define BLE_HS_ADV_TYPE_COMP_UUIDS16            0x03
#define BLE_HS_ADV_TYPE_INCOMP_UUIDS32          0x04
#define BLE_HS_ADV_TYPE_COMP_UUIDS32            0x05
#define BLE_HS_ADV_TYPE_INCOMP_UUIDS128         0x06
#define BLE_HS_ADV_TYPE_COMP_UUIDS128           0x07
#define BLE_HS_ADV_TYPE_INCOMP_NAME             0x08
#define BLE_HS_ADV_TYPE_COMP_NAME               0x09
#define BLE_HS_ADV_TYPE_TX_PWR_LVL              0x0a
#define BLE_HS_ADV_TYPE_SLAVE_ITVL_RANGE        0x12
#define BLE_HS_ADV_TYPE_SOL_UUIDS16             0x14
#define BLE_HS_ADV_TYPE_SOL_UUIDS128            0x15
#define BLE_HS_ADV_TYPE_SVC_DATA_UUID16         0x16
#define BLE_HS_ADV_TYPE_PUBLIC_TGT_ADDR         0x17
#define BLE_HS_ADV_TYPE_RANDOM_TGT_ADDR         0x18
#define BLE_HS_ADV_TYPE_APPEARANCE              0x19
#define BLE_HS_ADV_TYPE_ADV_ITVL                0x1a
#define BLE_HS_ADV_TYPE_SVC_DATA_UUID32         0x20
#define BLE_HS_ADV_TYPE_SVC_DATA_UUID128        0x21
#define BLE_HS_ADV_TYPE_URI                     0x24
#define BLE_HS_ADV_TYPE_MESH_PROV               0x29
#define BLE_HS_ADV_TYPE_MESH_MESSAGE            0x2a
#define BLE_HS_ADV_TYPE_MESH_BEACON             0x2b
#define BLE_HS_ADV_TYPE_MFG_DATA                0xff

#define BLE_HS_ADV_FLAGS_LEN                    1
#define BLE_HS_ADV_F_DISC_LTD                   0x01
#define BLE_HS_ADV_F_DISC_GEN                   0x02
#define BLE_HS_ADV_F_BREDR_UNSUP                0x04

#define BLE_HS_ADV_TX_PWR_LVL_LEN               1

/**
 * Set the tx_pwr_lvl field to this if you want the stack to fill in the tx
 * power level field.
 */
#define BLE_HS_ADV_TX_PWR_LVL_AUTO              (-128)

#define BLE_HS_ADV_SLAVE_ITVL_RANGE_LEN         4

#define BLE_HS_ADV_SVC_DATA_UUID16_MIN_LEN      2

#define BLE_HS_ADV_PUBLIC_TGT_ADDR_ENTRY_LEN    6

#define BLE_HS_ADV_APPEARANCE_LEN               2

#define BLE_HS_ADV_ADV_ITVL_LEN                 2

#define BLE_HS_ADV_SVC_DATA_UUID32_MIN_LEN      4

#define BLE_HS_ADV_SVC_DATA_UUID128_MIN_LEN     16

int ble_hs_adv_set_fields_mbuf(const struct ble_hs_adv_fields *adv_fields,
                               struct os_mbuf *om);

int ble_hs_adv_set_fields(const struct ble_hs_adv_fields *adv_fields,
                          uint8_t *dst, uint8_t *dst_len, uint8_t max_len);

int ble_hs_adv_parse_fields(struct ble_hs_adv_fields *adv_fields, uint8_t *src,
                            uint8_t src_len);

int ble_hs_adv_parse(const uint8_t *data, uint8_t length,
                     ble_hs_adv_parse_func_t func, void *user_data);

#ifdef __cplusplus
}
#endif

#endif
