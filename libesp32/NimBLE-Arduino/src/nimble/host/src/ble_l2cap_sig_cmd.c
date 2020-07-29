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
#include "ble_hs_priv.h"

int
ble_l2cap_sig_tx(uint16_t conn_handle, struct os_mbuf *txom)
{
    struct ble_l2cap_chan *chan;
    struct ble_hs_conn *conn;
    int rc;

    ble_hs_lock();
    ble_hs_misc_conn_chan_find_reqd(conn_handle, BLE_L2CAP_CID_SIG,
                                    &conn, &chan);
    rc = ble_l2cap_tx(conn, chan, txom);
    ble_hs_unlock();

    return rc;
}

void
ble_l2cap_sig_hdr_parse(void *payload, uint16_t len,
                        struct ble_l2cap_sig_hdr *dst)
{
    struct ble_l2cap_sig_hdr *src = payload;

    BLE_HS_DBG_ASSERT(len >= BLE_L2CAP_SIG_HDR_SZ);

    dst->op = src->op;
    dst->identifier = src->identifier;
    dst->length = le16toh(src->length);
}

int
ble_l2cap_sig_reject_tx(uint16_t conn_handle, uint8_t id, uint16_t reason,
                        void *data, int data_len)
{
    struct ble_l2cap_sig_reject *cmd;
    struct os_mbuf *txom;

    cmd = ble_l2cap_sig_cmd_get(BLE_L2CAP_SIG_OP_REJECT, id,
                           sizeof(*cmd) + data_len, &txom);
    if (!cmd) {
        return BLE_HS_ENOMEM;
    }

    cmd->reason = htole16(reason);
    memcpy(cmd->data, data, data_len);

    STATS_INC(ble_l2cap_stats, sig_rx);
    return ble_l2cap_sig_tx(conn_handle, txom);
}

int
ble_l2cap_sig_reject_invalid_cid_tx(uint16_t conn_handle, uint8_t id,
                                    uint16_t src_cid, uint16_t dst_cid)
{
    struct {
        uint16_t local_cid;
        uint16_t remote_cid;
    } data = {
        .local_cid = dst_cid,
        .remote_cid = src_cid,
    };

    return ble_l2cap_sig_reject_tx(conn_handle, id,
                                 BLE_L2CAP_SIG_ERR_INVALID_CID,
                                 &data, sizeof data);
}

void *
ble_l2cap_sig_cmd_get(uint8_t opcode, uint8_t id, uint16_t len,
                      struct os_mbuf **txom)
{
    struct ble_l2cap_sig_hdr *hdr;

    *txom = ble_hs_mbuf_l2cap_pkt();
    if (*txom == NULL) {
        return NULL;
    }

    if (os_mbuf_extend(*txom, sizeof(*hdr) + len) == NULL) {
        os_mbuf_free_chain(*txom);
        return NULL;
    }

    hdr = (struct ble_l2cap_sig_hdr *)(*txom)->om_data;

    hdr->op = opcode;
    hdr->identifier = id;
    hdr->length = htole16(len);

    return hdr->data;
}
