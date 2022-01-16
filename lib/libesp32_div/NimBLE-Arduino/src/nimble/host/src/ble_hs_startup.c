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

#include <stddef.h>
#include <string.h>
#include "host/ble_hs.h"
#include "host/ble_hs_hci.h"
#include "ble_hs_priv.h"

#if !MYNEWT_VAL(BLE_CONTROLLER)
static int
ble_hs_startup_read_sup_f_tx(void)
{
    struct ble_hci_ip_rd_loc_supp_feat_rp rsp;
    int rc;

    rc = ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_INFO_PARAMS,
                                      BLE_HCI_OCF_IP_RD_LOC_SUPP_FEAT),
                           NULL, 0, &rsp, sizeof(rsp));
    if (rc != 0) {
        return rc;
    }

    /* for now we don't use it outside of init sequence so check this here
     * LE Supported (Controller) byte 4, bit 6
     */
    if (!(rsp.features & 0x0000006000000000)) {
        BLE_HS_LOG(ERROR, "Controller doesn't support LE\n");
        return BLE_HS_ECONTROLLER;
    }

    return 0;
}
#endif

static int
ble_hs_startup_read_local_ver_tx(void)
{
    struct ble_hci_ip_rd_local_ver_rp rsp;
    int rc;

    rc = ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_INFO_PARAMS,
                                      BLE_HCI_OCF_IP_RD_LOCAL_VER),
                           NULL, 0, &rsp, sizeof(rsp));
    if (rc != 0) {
        return rc;
    }

    /* For now we are interested only in HCI Version */
    ble_hs_hci_set_hci_version(rsp.hci_ver);

    return 0;
}

static int
ble_hs_startup_le_read_sup_f_tx(void)
{
    struct ble_hci_le_rd_loc_supp_feat_rp rsp;
    int rc;

    rc = ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_LE,
                                      BLE_HCI_OCF_LE_RD_LOC_SUPP_FEAT),
                           NULL,0, &rsp, sizeof(rsp));
    if (rc != 0) {
        return rc;
    }

    ble_hs_hci_set_le_supported_feat(le64toh(rsp.features));

    return 0;
}

static int
ble_hs_startup_le_read_buf_sz_tx(uint16_t *out_pktlen, uint8_t *out_max_pkts)
{
    struct ble_hci_le_rd_buf_size_rp rsp;
    int rc;

    rc = ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_LE,
                                      BLE_HCI_OCF_LE_RD_BUF_SIZE), NULL, 0,
                           &rsp, sizeof(rsp));
    if (rc != 0) {
        return rc;
    }

    *out_pktlen = le16toh(rsp.data_len);
    *out_max_pkts = rsp.data_packets;

    return 0;
}

static int
ble_hs_startup_read_buf_sz_tx(uint16_t *out_pktlen, uint16_t *out_max_pkts)
{
    struct ble_hci_ip_rd_buf_size_rp rsp;
    int rc;

    rc = ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_INFO_PARAMS,
                                      BLE_HCI_OCF_IP_RD_BUF_SIZE), NULL, 0,
                           &rsp, sizeof(rsp));
    if (rc != 0) {
        return rc;
    }

    *out_pktlen = le16toh(rsp.acl_data_len);
    *out_max_pkts = le16toh(rsp.acl_num);

    return 0;
}

static int
ble_hs_startup_read_buf_sz(void)
{
    uint16_t le_pktlen = 0;
    uint16_t max_pkts = 0;
    uint16_t pktlen = 0;
    uint8_t le_max_pkts = 0;
    int rc;

    rc = ble_hs_startup_le_read_buf_sz_tx(&le_pktlen, &le_max_pkts);
    if (rc != 0) {
        return rc;
    }

    if (le_pktlen != 0) {
        pktlen = le_pktlen;
        max_pkts = le_max_pkts;
    } else {
        rc = ble_hs_startup_read_buf_sz_tx(&pktlen, &max_pkts);
        if (rc != 0) {
            return rc;
        }
    }

    rc = ble_hs_hci_set_buf_sz(pktlen, max_pkts);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

static int
ble_hs_startup_read_bd_addr(void)
{
    struct ble_hci_ip_rd_bd_addr_rp rsp;
    int rc;

    rc = ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_INFO_PARAMS,
                                      BLE_HCI_OCF_IP_RD_BD_ADDR),
                           NULL, 0, &rsp, sizeof(rsp));
    if (rc != 0) {
        return rc;
    }

    ble_hs_id_set_pub(rsp.addr);
    return 0;
}

static int
ble_hs_startup_le_set_evmask_tx(void)
{
    struct ble_hci_le_set_event_mask_cp cmd;
    uint8_t version;
    uint64_t mask;
    int rc;

    version = ble_hs_hci_get_hci_version();

    /* TODO should we also check for supported commands when setting this? */

    /**
     * Enable the following LE events:
     *     0x0000000000000001 LE Connection Complete Event
     *     0x0000000000000002 LE Advertising Report Event
     *     0x0000000000000004 LE Connection Update Complete Event
     *     0x0000000000000008 LE Read Remote Used Features Complete Event
     *     0x0000000000000010 LE Long Term Key Request Event
     */
    mask = 0x000000000000001f;

    if (version >= BLE_HCI_VER_BCS_4_1) {
        /**
         * Enable the following LE events:
         *   0x0000000000000020 LE Remote Connection Parameter Request Event */
        mask |= 0x0000000000000020;
    }

    if (version >= BLE_HCI_VER_BCS_4_2) {
        /**
         * Enable the following LE events:
         *   0x0000000000000040 LE Data Length Change Event
         *   0x0000000000000200 LE Enhanced Connection Complete Event
         *   0x0000000000000400 LE Directed Advertising Report Event
         */
        mask |= 0x0000000000000640;
    }

    if (version >= BLE_HCI_VER_BCS_5_0) {
        /**
         * Enable the following LE events:
         *   0x0000000000000800 LE PHY Update Complete Event
         *   0x0000000000001000 LE Extended Advertising Report Event
         *   0x0000000000002000 LE Periodic Advertising Sync Established Event
         *   0x0000000000004000 LE Periodic Advertising Report Event
         *   0x0000000000008000 LE Periodic Advertising Sync Lost Event
         *   0x0000000000010000 LE Extended Scan Timeout Event
         *   0x0000000000020000 LE Extended Advertising Set Terminated Event
         *   0x0000000000040000 LE Scan Request Received Event
         *   0x0000000000080000 LE Channel Selection Algorithm Event
         */
        mask |= 0x00000000000ff800;
    }

#if MYNEWT_VAL(BLE_PERIODIC_ADV_SYNC_TRANSFER)
    if (version >= BLE_HCI_VER_BCS_5_1) {
        /**
         * Enable the following LE events:
         * 0x0000000000800000 LE Periodic Advertising Sync Transfer Received event
         */
        mask |= 0x0000000000800000;
    }
#endif

    cmd.event_mask = htole64(mask);

    rc = ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_LE,
                                      BLE_HCI_OCF_LE_SET_EVENT_MASK),
                           &cmd, sizeof(cmd), NULL, 0);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

static int
ble_hs_startup_set_evmask_tx(void)
{
    struct ble_hci_cb_set_event_mask_cp cmd;
    struct ble_hci_cb_set_event_mask2_cp cmd2;
    uint8_t version;
    int rc;

    version = ble_hs_hci_get_hci_version();

    /**
     * Enable the following events:
     *     0x0000000000000010 Disconnection Complete Event
     *     0x0000000000000080 Encryption Change Event
     *     0x0000000000008000 Hardware Error Event
     *     0x0000000002000000 Data Buffer Overflow Event
     *     0x0000800000000000 Encryption Key Refresh Complete Event
     *     0x2000000000000000 LE Meta-Event
     */
    cmd.event_mask = htole64(0x2000800002008090);

    rc = ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_CTLR_BASEBAND,
                                      BLE_HCI_OCF_CB_SET_EVENT_MASK),
                           &cmd, sizeof(cmd), NULL, 0);
    if (rc != 0) {
        return rc;
    }

    if (version >= BLE_HCI_VER_BCS_4_1) {
        /**
         * Enable the following events:
         *     0x0000000000800000 Authenticated Payload Timeout Event
         */
        cmd2.event_mask2 = htole64(0x0000000000800000);
        rc = ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_CTLR_BASEBAND,
                                          BLE_HCI_OCF_CB_SET_EVENT_MASK2),
                               &cmd2, sizeof(cmd2), NULL, 0);
        if (rc != 0) {
            return rc;
        }
    }

    return 0;
}

static int
ble_hs_startup_reset_tx(void)
{
    return ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_CTLR_BASEBAND,
                                        BLE_HCI_OCF_CB_RESET),
                             NULL, 0, NULL, 0);
}

int
ble_hs_startup_go(void)
{
    int rc;

    rc = ble_hs_startup_reset_tx();
    if (rc != 0) {
        return rc;
    }

    rc = ble_hs_startup_read_local_ver_tx();
    if (rc != 0) {
        return rc;
    }

    /* XXX: Read local supported commands. */

    /* we need to check this only if using external controller */
#if !MYNEWT_VAL(BLE_CONTROLLER)
    if (ble_hs_hci_get_hci_version() < BLE_HCI_VER_BCS_4_0) {
        BLE_HS_LOG(ERROR, "Required controller version is 4.0 (6)\n");
        return BLE_HS_ECONTROLLER;
    }

    rc = ble_hs_startup_read_sup_f_tx();
    if (rc != 0) {
        return rc;
    }
#endif

    rc = ble_hs_startup_set_evmask_tx();
    if (rc != 0) {
        return rc;
    }

    rc = ble_hs_startup_le_set_evmask_tx();
    if (rc != 0) {
        return rc;
    }

    rc = ble_hs_startup_read_buf_sz();
    if (rc != 0) {
        return rc;
    }

    rc = ble_hs_startup_le_read_sup_f_tx();
    if (rc != 0) {
        return rc;
    }

    rc = ble_hs_startup_read_bd_addr();
    if (rc != 0) {
        return rc;
    }

    ble_hs_pvcy_set_our_irk(NULL);

    /* If flow control is enabled, configure the controller to use it. */
    ble_hs_flow_startup();

    return 0;
}
