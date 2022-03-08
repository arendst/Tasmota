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
#include "nimble/nimble/include/nimble/hci_common.h"
#include "nimble/nimble/host/include/host/ble_hs_hci.h"
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
    struct ble_hci_le_rd_adv_chan_txpwr_rp rsp;
    int rc;

    rc = ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_LE,
                                      BLE_HCI_OCF_LE_RD_ADV_CHAN_TXPWR),
                           NULL, 0, &rsp, sizeof(rsp));
    if (rc != 0) {
        return rc;
    }

    *out_tx_pwr = rsp.power_level;

    if (*out_tx_pwr < BLE_HCI_ADV_CHAN_TXPWR_MIN ||
        *out_tx_pwr > BLE_HCI_ADV_CHAN_TXPWR_MAX) {
        BLE_HS_LOG(WARN, "advertiser txpwr out of range\n");
    }

    return 0;
}

int
ble_hs_hci_util_rand(void *dst, int len)
{
    struct ble_hci_le_rand_rp rsp;
    uint8_t *u8ptr;
    int chunk_sz;
    int rc;

    u8ptr = dst;
    while (len > 0) {
        rc = ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_LE, BLE_HCI_OCF_LE_RAND),
                               NULL, 0, &rsp, sizeof(rsp));
        if (rc != 0) {
            return rc;
        }

        chunk_sz = min(len, sizeof(rsp));
        memcpy(u8ptr, &rsp.random_number, chunk_sz);

        len -= chunk_sz;
        u8ptr += chunk_sz;
    }

    return 0;
}

int
ble_hs_hci_util_read_rssi(uint16_t conn_handle, int8_t *out_rssi)
{
    struct ble_hci_rd_rssi_cp cmd;
    struct ble_hci_rd_rssi_rp rsp;

    int rc;

    cmd.handle = htole16(conn_handle);

    rc = ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_STATUS_PARAMS,
                                      BLE_HCI_OCF_RD_RSSI), &cmd, sizeof(cmd),
                           &rsp, sizeof(rsp));
    if (rc != 0) {
        return rc;
    }

    if (le16toh(rsp.handle) != conn_handle) {
        return BLE_HS_ECONTROLLER;
    }

    *out_rssi = rsp.rssi;

    return 0;
}

int
ble_hs_hci_util_set_random_addr(const uint8_t *addr)
{
    struct ble_hci_le_set_rand_addr_cp cmd;

    memcpy(cmd.addr, addr, BLE_DEV_ADDR_LEN);

    return ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_LE,
                                        BLE_HCI_OCF_LE_SET_RAND_ADDR),
                             &cmd, sizeof(cmd), NULL, 0);
}

int
ble_hs_hci_util_set_data_len(uint16_t conn_handle, uint16_t tx_octets,
                             uint16_t tx_time)
{
    struct ble_hci_le_set_data_len_cp cmd;
    struct ble_hci_le_set_data_len_rp rsp;
    int rc;

    if (tx_octets < BLE_HCI_SET_DATALEN_TX_OCTETS_MIN ||
        tx_octets > BLE_HCI_SET_DATALEN_TX_OCTETS_MAX) {
        return BLE_HS_EINVAL;
    }

    if (tx_time < BLE_HCI_SET_DATALEN_TX_TIME_MIN ||
        tx_time > BLE_HCI_SET_DATALEN_TX_TIME_MAX) {
        return BLE_HS_EINVAL;
    }

    cmd.conn_handle = htole16(conn_handle);
    cmd.tx_octets = htole16(tx_octets);
    cmd.tx_time = htole16(tx_time);

    rc = ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_LE,
                                      BLE_HCI_OCF_LE_SET_DATA_LEN),
                           &cmd, sizeof(cmd), &rsp, sizeof(rsp));
    if (rc != 0) {
        return rc;
    }

    if (le16toh(rsp.conn_handle) != conn_handle) {
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
    struct ble_hci_le_rd_chan_map_cp cmd;
    struct ble_hci_le_rd_chan_map_rp rsp;
    int rc;

    cmd.conn_handle = htole16(conn_handle);

    rc = ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_LE,
                                      BLE_HCI_OCF_LE_RD_CHAN_MAP),
                           &cmd, sizeof(cmd), &rsp, sizeof(rsp));
    if (rc != 0) {
        return rc;
    }

    if (le16toh(rsp.conn_handle) != conn_handle) {
        return BLE_HS_ECONTROLLER;
    }

    memcpy(out_chan_map, rsp.chan_map, 5);

    return 0;
}

int
ble_hs_hci_set_chan_class(const uint8_t *chan_map)
{
    struct ble_hci_le_set_host_chan_class_cp cmd;

    memcpy(cmd.chan_map, chan_map, sizeof(cmd.chan_map));

    return ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_LE,
                                        BLE_HCI_OCF_LE_SET_HOST_CHAN_CLASS),
                             &cmd, sizeof(cmd), NULL, 0);
}
