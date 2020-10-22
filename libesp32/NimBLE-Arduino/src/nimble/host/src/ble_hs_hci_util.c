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

#include <string.h>
#include "nimble/hci_common.h"
#include "host/ble_hs_hci.h"
#include "ble_hs_priv.h"

uint16_t
ble_hs_hci_util_handle_pb_bc_join(uint16_t handle, uint8_t pb, uint8_t bc)
{
    BLE_HS_DBG_ASSERT(handle <= 0x0fff);
    BLE_HS_DBG_ASSERT(pb <= 0x03);
    BLE_HS_DBG_ASSERT(bc <= 0x03);

    return (handle  << 0)   |
           (pb      << 12)  |
           (bc      << 14);
}

int
ble_hs_hci_util_read_adv_tx_pwr(int8_t *out_tx_pwr)
{
    uint8_t params_len;
    int rc;

    rc = ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_LE,
                                      BLE_HCI_OCF_LE_RD_ADV_CHAN_TXPWR),
                           NULL, 0,out_tx_pwr, 1, &params_len);
    if (rc != 0) {
        return rc;
    }

    if (params_len != 1                     ||
        *out_tx_pwr < BLE_HCI_ADV_CHAN_TXPWR_MIN ||
        *out_tx_pwr > BLE_HCI_ADV_CHAN_TXPWR_MAX) {
        BLE_HS_LOG(WARN, "advertiser txpwr out of range\n");
    }

    return 0;
}

int
ble_hs_hci_util_rand(void *dst, int len)
{
    uint8_t rsp_buf[BLE_HCI_LE_RAND_LEN];
    uint8_t params_len;
    uint8_t *u8ptr;
    int chunk_sz;
    int rc;

    u8ptr = dst;
    while (len > 0) {
        rc = ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_LE, BLE_HCI_OCF_LE_RAND),
                               NULL, 0, rsp_buf, sizeof rsp_buf, &params_len);
        if (rc != 0) {
            return rc;
        }
        if (params_len != sizeof rsp_buf) {
            return BLE_HS_ECONTROLLER;
        }

        chunk_sz = min(len, sizeof rsp_buf);
        memcpy(u8ptr, rsp_buf, chunk_sz);

        len -= chunk_sz;
        u8ptr += chunk_sz;
    }

    return 0;
}

int
ble_hs_hci_util_read_rssi(uint16_t conn_handle, int8_t *out_rssi)
{
    uint8_t buf[BLE_HCI_READ_RSSI_LEN];
    uint8_t params[BLE_HCI_READ_RSSI_ACK_PARAM_LEN];
    uint16_t params_conn_handle;
    uint8_t params_len;
    int rc;

    ble_hs_hci_cmd_build_read_rssi(conn_handle, buf, sizeof buf);
    rc = ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_STATUS_PARAMS,
                                      BLE_HCI_OCF_RD_RSSI), buf, sizeof(buf),
                           params, sizeof(params), &params_len);
    if (rc != 0) {
        return rc;
    }

    if (params_len != BLE_HCI_READ_RSSI_ACK_PARAM_LEN) {
        return BLE_HS_ECONTROLLER;
    }

    params_conn_handle = get_le16(params + 0);
    if (params_conn_handle != conn_handle) {
        return BLE_HS_ECONTROLLER;
    }

    *out_rssi = params[2];

    return 0;
}

int
ble_hs_hci_util_set_random_addr(const uint8_t *addr)
{
    uint8_t buf[BLE_HCI_SET_RAND_ADDR_LEN];
    int rc;

    /* set the address in the controller */

    rc = ble_hs_hci_cmd_build_set_random_addr(addr, buf, sizeof(buf));
    if (rc != 0) {
        return rc;
    }

    rc = ble_hs_hci_cmd_tx_empty_ack(BLE_HCI_OP(BLE_HCI_OGF_LE,
                                                BLE_HCI_OCF_LE_SET_RAND_ADDR),
                                     buf, BLE_HCI_SET_RAND_ADDR_LEN);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

int
ble_hs_hci_util_set_data_len(uint16_t conn_handle, uint16_t tx_octets,
                             uint16_t tx_time)
{

    uint8_t buf[BLE_HCI_SET_DATALEN_LEN];
    uint8_t params[BLE_HCI_SET_DATALEN_ACK_PARAM_LEN];
    uint16_t params_conn_handle;
    uint8_t params_len;
    int rc;

    rc = ble_hs_hci_cmd_build_set_data_len(conn_handle, tx_octets, tx_time,
                                           buf, sizeof buf);
    if (rc != 0) {
        return BLE_HS_HCI_ERR(rc);
    }

    rc = ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_LE,
                                      BLE_HCI_OCF_LE_SET_DATA_LEN),
                           buf, sizeof(buf), params,
                           BLE_HCI_SET_DATALEN_ACK_PARAM_LEN, &params_len);
    if (rc != 0) {
        return rc;
    }

    if (params_len != BLE_HCI_SET_DATALEN_ACK_PARAM_LEN) {
        return BLE_HS_ECONTROLLER;
    }

    params_conn_handle = get_le16(params + 0);
    if (params_conn_handle != conn_handle) {
        return BLE_HS_ECONTROLLER;
    }

    return 0;
}

int
ble_hs_hci_util_data_hdr_strip(struct os_mbuf *om,
                               struct hci_data_hdr *out_hdr)
{
    int rc;

    rc = os_mbuf_copydata(om, 0, BLE_HCI_DATA_HDR_SZ, out_hdr);
    if (rc != 0) {
        return BLE_HS_ECONTROLLER;
    }

    /* Strip HCI ACL data header from the front of the packet. */
    os_mbuf_adj(om, BLE_HCI_DATA_HDR_SZ);

    out_hdr->hdh_handle_pb_bc = get_le16(&out_hdr->hdh_handle_pb_bc);
    out_hdr->hdh_len = get_le16(&out_hdr->hdh_len);

    return 0;
}

int
ble_hs_hci_read_chan_map(uint16_t conn_handle, uint8_t *out_chan_map)
{
    uint8_t buf[BLE_HCI_RD_CHANMAP_LEN];
    uint8_t params[BLE_HCI_RD_CHANMAP_RSP_LEN];
    uint16_t params_conn_handle;
    uint8_t params_len;
    int rc;

    ble_hs_hci_cmd_build_le_read_chan_map(conn_handle, buf, sizeof buf);
    rc = ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_LE,
                                      BLE_HCI_OCF_LE_RD_CHAN_MAP),
                           buf, sizeof(buf), params, BLE_HCI_RD_CHANMAP_RSP_LEN,
                           &params_len);
    if (rc != 0) {
        return rc;
    }

    if (params_len != BLE_HCI_RD_CHANMAP_RSP_LEN) {
        return BLE_HS_ECONTROLLER;
    }

    params_conn_handle = get_le16(params + 0);
    if (params_conn_handle != conn_handle) {
        return BLE_HS_ECONTROLLER;
    }

    memcpy(out_chan_map, params + 2, 5);

    return 0;
}

int
ble_hs_hci_set_chan_class(const uint8_t *chan_map)
{
    uint8_t buf[BLE_HCI_SET_HOST_CHAN_CLASS_LEN];
    int rc;

    ble_hs_hci_cmd_build_le_set_host_chan_class(chan_map, buf, sizeof buf);
    rc = ble_hs_hci_cmd_tx_empty_ack(BLE_HCI_OP(BLE_HCI_OGF_LE,
                                            BLE_HCI_OCF_LE_SET_HOST_CHAN_CLASS),
                                     buf, sizeof(buf));
    if (rc != 0) {
        return rc;
    }

    return 0;
}
