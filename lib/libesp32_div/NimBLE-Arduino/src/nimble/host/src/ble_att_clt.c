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

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include "os/os_mempool.h"
#include "nimble/ble.h"
#include "host/ble_uuid.h"
#include "ble_hs_priv.h"

/*****************************************************************************
 * $error response                                                           *
 *****************************************************************************/

int
ble_att_clt_rx_error(uint16_t conn_handle, struct os_mbuf **rxom)
{
    struct ble_att_error_rsp *rsp;
    int rc;

    rc = ble_hs_mbuf_pullup_base(rxom, sizeof(*rsp));
    if (rc != 0) {
        return rc;
    }

    rsp = (struct ble_att_error_rsp *)(*rxom)->om_data;

    ble_gattc_rx_err(conn_handle, le16toh(rsp->baep_handle),
                     le16toh(rsp->baep_error_code));

    return 0;
}

/*****************************************************************************
 * $mtu exchange                                                             *
 *****************************************************************************/

int
ble_att_clt_tx_mtu(uint16_t conn_handle, uint16_t mtu)
{
    struct ble_att_mtu_cmd *req;
    struct ble_l2cap_chan *chan;
    struct ble_hs_conn *conn;
    struct os_mbuf *txom;
    int rc;

    if (mtu < BLE_ATT_MTU_DFLT) {
        return BLE_HS_EINVAL;
    }

    ble_hs_lock();

    rc = ble_att_conn_chan_find(conn_handle, &conn, &chan);
    if (rc != 0) {
        rc = BLE_HS_ENOTCONN;
    } else if (chan->flags & BLE_L2CAP_CHAN_F_TXED_MTU) {
        rc = BLE_HS_EALREADY;
    } else {
        rc = 0;
    }
    ble_hs_unlock();

    if (rc != 0) {
        return rc;
    }

    req = ble_att_cmd_get(BLE_ATT_OP_MTU_REQ, sizeof(*req), &txom);
    if (req == NULL) {
        return BLE_HS_ENOMEM;
    }

    req->bamc_mtu = htole16(mtu);

    rc = ble_att_tx(conn_handle, txom);
    if (rc != 0) {
        return rc;
    }

    ble_hs_lock();

    rc = ble_att_conn_chan_find(conn_handle, &conn, &chan);
    if (rc == 0) {
        chan->flags |= BLE_L2CAP_CHAN_F_TXED_MTU;
    }

    ble_hs_unlock();

    return rc;
}

int
ble_att_clt_rx_mtu(uint16_t conn_handle, struct os_mbuf **rxom)
{
    struct ble_att_mtu_cmd *cmd;
    struct ble_l2cap_chan *chan;
    uint16_t mtu;
    int rc;

    mtu = 0;

    rc = ble_hs_mbuf_pullup_base(rxom, sizeof(*cmd));
    if (rc == 0) {
        cmd = (struct ble_att_mtu_cmd *)(*rxom)->om_data;

        ble_hs_lock();

        rc = ble_att_conn_chan_find(conn_handle, NULL, &chan);
        if (rc == 0) {
            ble_att_set_peer_mtu(chan, le16toh(cmd->bamc_mtu));
            mtu = ble_att_chan_mtu(chan);
        }

        ble_hs_unlock();

        if (rc == 0) {
            ble_gap_mtu_event(conn_handle, BLE_L2CAP_CID_ATT, mtu);
        }
    }

    ble_gattc_rx_mtu(conn_handle, rc, mtu);
    return rc;
}

/*****************************************************************************
 * $find information                                                         *
 *****************************************************************************/

int
ble_att_clt_tx_find_info(uint16_t conn_handle, uint16_t start_handle,
                         uint16_t end_handle)
{
#if !NIMBLE_BLE_ATT_CLT_FIND_INFO
    return BLE_HS_ENOTSUP;
#endif

    struct ble_att_find_info_req *req;
    struct os_mbuf *txom;

    if (start_handle == 0 || start_handle > end_handle) {
        return BLE_HS_EINVAL;
    }

    req = ble_att_cmd_get(BLE_ATT_OP_FIND_INFO_REQ, sizeof(*req), &txom);
    if (req == NULL) {
        return BLE_HS_ENOMEM;
    }

    req->bafq_start_handle = htole16(start_handle);
    req->bafq_end_handle = htole16(end_handle);

    return ble_att_tx(conn_handle, txom);
}

static int
ble_att_clt_parse_find_info_entry(struct os_mbuf **rxom, uint8_t rsp_format,
                                  struct ble_att_find_info_idata *idata)
{
    int entry_len;
    int rc;

    switch (rsp_format) {
    case BLE_ATT_FIND_INFO_RSP_FORMAT_16BIT:
        entry_len = 2 + 2;
        break;

    case BLE_ATT_FIND_INFO_RSP_FORMAT_128BIT:
        entry_len = 2 + 16;
        break;

    default:
        return BLE_HS_EBADDATA;
    }

    rc = ble_hs_mbuf_pullup_base(rxom, entry_len);
    if (rc != 0) {
        return rc;
    }

    idata->attr_handle = get_le16((*rxom)->om_data);

    switch (rsp_format) {
    case BLE_ATT_FIND_INFO_RSP_FORMAT_16BIT:
        rc = ble_uuid_init_from_att_mbuf(&idata->uuid, *rxom, 2, 2);
        if (rc != 0) {
            return BLE_HS_EBADDATA;
        }
        break;

    case BLE_ATT_FIND_INFO_RSP_FORMAT_128BIT:
        rc = ble_uuid_init_from_att_mbuf(&idata->uuid, *rxom, 2, 16);
        if (rc != 0) {
            return BLE_HS_EBADDATA;
        }
        break;

    default:
        BLE_HS_DBG_ASSERT(0);
        break;
    }

    os_mbuf_adj(*rxom, entry_len);
    return 0;
}

int
ble_att_clt_rx_find_info(uint16_t conn_handle, struct os_mbuf **om)
{
#if !NIMBLE_BLE_ATT_CLT_FIND_INFO
    return BLE_HS_ENOTSUP;
#endif

    struct ble_att_find_info_idata idata;
    struct ble_att_find_info_rsp *rsp;
    int rc;

    rc = ble_hs_mbuf_pullup_base(om, sizeof(*rsp));
    if (rc != 0) {
        goto done;
    }

    rsp = (struct ble_att_find_info_rsp *)(*om)->om_data;

    /* Strip the response base from the front of the mbuf. */
    os_mbuf_adj((*om), sizeof(*rsp));

    while (OS_MBUF_PKTLEN(*om) > 0) {
        rc = ble_att_clt_parse_find_info_entry(om, rsp->bafp_format, &idata);
        if (rc != 0) {
            goto done;
        }

        /* Hand find-info entry to GATT. */
        ble_gattc_rx_find_info_idata(conn_handle, &idata);
    }

    rc = 0;

done:
    /* Notify GATT that response processing is done. */
    ble_gattc_rx_find_info_complete(conn_handle, rc);
    return rc;
}

/*****************************************************************************
 * $find by type value                                                       *
 *****************************************************************************/

/*
 * TODO consider this to accept UUID instead of value, it is used only for this
 * anyway
 */
int
ble_att_clt_tx_find_type_value(uint16_t conn_handle, uint16_t start_handle,
                               uint16_t end_handle, uint16_t attribute_type,
                               const void *attribute_value, int value_len)
{
#if !NIMBLE_BLE_ATT_CLT_FIND_TYPE
    return BLE_HS_ENOTSUP;
#endif

    struct ble_att_find_type_value_req *req;
    struct os_mbuf *txom;

    if (start_handle == 0 || start_handle > end_handle) {
        return BLE_HS_EINVAL;
    }

    req = ble_att_cmd_get(BLE_ATT_OP_FIND_TYPE_VALUE_REQ, sizeof(*req) + value_len,
                          &txom);
    if (req == NULL) {
        return BLE_HS_ENOMEM;
    }

    req->bavq_start_handle = htole16(start_handle);
    req->bavq_end_handle = htole16(end_handle);
    req->bavq_attr_type = htole16(attribute_type);
    memcpy(req->bavq_value, attribute_value, value_len);

    return ble_att_tx(conn_handle, txom);
}

static int
ble_att_clt_parse_find_type_value_hinfo(
    struct os_mbuf **om, struct ble_att_find_type_value_hinfo *dst)
{
    struct ble_att_handle_group *group;
    int rc;

    rc = ble_hs_mbuf_pullup_base(om, sizeof(*group));
    if (rc != 0) {
        return BLE_HS_EBADDATA;
    }

    group = (struct ble_att_handle_group *)(*om)->om_data;

    dst->attr_handle = le16toh(group->attr_handle);
    dst->group_end_handle = le16toh(group->group_end_handle);

    os_mbuf_adj((*om), sizeof(*group));

    return 0;
}

int
ble_att_clt_rx_find_type_value(uint16_t conn_handle, struct os_mbuf **rxom)
{
#if !NIMBLE_BLE_ATT_CLT_FIND_TYPE
    return BLE_HS_ENOTSUP;
#endif

    struct ble_att_find_type_value_hinfo hinfo;
    int rc;

    /* Parse the Handles-Information-List field, passing each entry to GATT. */
    rc = 0;
    while (OS_MBUF_PKTLEN(*rxom) > 0) {
        rc = ble_att_clt_parse_find_type_value_hinfo(rxom, &hinfo);
        if (rc != 0) {
            break;
        }

        ble_gattc_rx_find_type_value_hinfo(conn_handle, &hinfo);
    }

    /* Notify GATT client that the full response has been parsed. */
    ble_gattc_rx_find_type_value_complete(conn_handle, rc);

    return 0;
}

/*****************************************************************************
 * $read by type                                                             *
 *****************************************************************************/

int
ble_att_clt_tx_read_type(uint16_t conn_handle, uint16_t start_handle,
                         uint16_t end_handle, const ble_uuid_t *uuid)
{
#if !NIMBLE_BLE_ATT_CLT_READ_TYPE
    return BLE_HS_ENOTSUP;
#endif

    struct ble_att_read_type_req *req;
    struct os_mbuf *txom;

    if (start_handle == 0 || start_handle > end_handle) {
        return BLE_HS_EINVAL;
    }

    req = ble_att_cmd_get(BLE_ATT_OP_READ_TYPE_REQ,
                          sizeof(*req) + ble_uuid_length(uuid), &txom);
    if (req == NULL) {
        return BLE_HS_ENOMEM;
    }

    req->batq_start_handle = htole16(start_handle);
    req->batq_end_handle = htole16(end_handle);

    ble_uuid_flat(uuid, req->uuid);

    return ble_att_tx(conn_handle, txom);
}

int
ble_att_clt_rx_read_type(uint16_t conn_handle, struct os_mbuf **rxom)
{
#if !NIMBLE_BLE_ATT_CLT_READ_TYPE
    return BLE_HS_ENOTSUP;
#endif

    struct ble_att_read_type_adata adata;
    struct ble_att_attr_data_list *data;
    struct ble_att_read_type_rsp *rsp;
    uint8_t data_len;
    int rc;

    rc = ble_hs_mbuf_pullup_base(rxom, sizeof(*rsp));
    if (rc != 0) {
        goto done;
    }

    rsp = (struct ble_att_read_type_rsp *)(*rxom)->om_data;

    data_len = rsp->batp_length;

    /* Strip the response base from the front of the mbuf. */
    os_mbuf_adj(*rxom, sizeof(*rsp));

    if (data_len < sizeof(*data)) {
        rc = BLE_HS_EBADDATA;
        goto done;
    }

    /* Parse the Attribute Data List field, passing each entry to the GATT. */
    while (OS_MBUF_PKTLEN(*rxom) > 0) {
        rc = ble_hs_mbuf_pullup_base(rxom, data_len);
        if (rc != 0) {
            break;
        }

        data = (struct ble_att_attr_data_list *)(*rxom)->om_data;

        adata.att_handle = le16toh(data->handle);
        adata.value_len = data_len - sizeof(*data);
        adata.value = data->value;

        ble_gattc_rx_read_type_adata(conn_handle, &adata);
        os_mbuf_adj(*rxom, data_len);
    }

done:
    /* Notify GATT that the response is done being parsed. */
    ble_gattc_rx_read_type_complete(conn_handle, rc);
    return rc;

}

/*****************************************************************************
 * $read                                                                     *
 *****************************************************************************/

int
ble_att_clt_tx_read(uint16_t conn_handle, uint16_t handle)
{
#if !NIMBLE_BLE_ATT_CLT_READ
    return BLE_HS_ENOTSUP;
#endif

    struct ble_att_read_req *req;
    struct os_mbuf *txom;
    int rc;

    if (handle == 0) {
        return BLE_HS_EINVAL;
    }

    req = ble_att_cmd_get(BLE_ATT_OP_READ_REQ, sizeof(*req), &txom);
    if (req == NULL) {
        return BLE_HS_ENOMEM;
    }

    req->barq_handle = htole16(handle);

    rc = ble_att_tx(conn_handle, txom);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

int
ble_att_clt_rx_read(uint16_t conn_handle, struct os_mbuf **rxom)
{
#if !NIMBLE_BLE_ATT_CLT_READ
    return BLE_HS_ENOTSUP;
#endif

    /* Pass the Attribute Value field to GATT. */
    ble_gattc_rx_read_rsp(conn_handle, 0, rxom);
    return 0;
}

/*****************************************************************************
 * $read blob                                                                *
 *****************************************************************************/

int
ble_att_clt_tx_read_blob(uint16_t conn_handle, uint16_t handle, uint16_t offset)
{
#if !NIMBLE_BLE_ATT_CLT_READ_BLOB
    return BLE_HS_ENOTSUP;
#endif

    struct ble_att_read_blob_req *req;
    struct os_mbuf *txom;
    int rc;

    if (handle == 0) {
        return BLE_HS_EINVAL;
    }

    req = ble_att_cmd_get(BLE_ATT_OP_READ_BLOB_REQ, sizeof(*req), &txom);
    if (req == NULL) {
        return BLE_HS_ENOMEM;
    }

    req->babq_handle = htole16(handle);
    req->babq_offset = htole16(offset);

    rc = ble_att_tx(conn_handle, txom);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

int
ble_att_clt_rx_read_blob(uint16_t conn_handle, struct os_mbuf **rxom)
{
#if !NIMBLE_BLE_ATT_CLT_READ_BLOB
    return BLE_HS_ENOTSUP;
#endif

    /* Pass the Attribute Value field to GATT. */
    ble_gattc_rx_read_blob_rsp(conn_handle, 0, rxom);
    return 0;
}

/*****************************************************************************
 * $read multiple                                                            *
 *****************************************************************************/
int
ble_att_clt_tx_read_mult(uint16_t conn_handle, const uint16_t *handles,
                         int num_handles)
{
#if !NIMBLE_BLE_ATT_CLT_READ_MULT
    return BLE_HS_ENOTSUP;
#endif

    struct ble_att_read_mult_req *req;
    struct os_mbuf *txom;
    int i;

    if (num_handles < 1) {
        return BLE_HS_EINVAL;
    }

    req = ble_att_cmd_get(BLE_ATT_OP_READ_MULT_REQ,
                          sizeof(req->handles[0]) * num_handles,
                          &txom);
    if (req == NULL) {
        return BLE_HS_ENOMEM;
    }

    for(i = 0; i < num_handles; i++) {
        req->handles[i] = htole16(handles[i]);
    }

    return ble_att_tx(conn_handle, txom);
}

int
ble_att_clt_rx_read_mult(uint16_t conn_handle, struct os_mbuf **rxom)
{
#if !NIMBLE_BLE_ATT_CLT_READ_MULT
    return BLE_HS_ENOTSUP;
#endif

    /* Pass the Attribute Value field to GATT. */
    ble_gattc_rx_read_mult_rsp(conn_handle, 0, rxom);
    return 0;
}

/*****************************************************************************
 * $read by group type                                                       *
 *****************************************************************************/

int
ble_att_clt_tx_read_group_type(uint16_t conn_handle,
                               uint16_t start_handle, uint16_t end_handle,
                               const ble_uuid_t *uuid)
{
#if !NIMBLE_BLE_ATT_CLT_READ_GROUP_TYPE
    return BLE_HS_ENOTSUP;
#endif

    struct ble_att_read_group_type_req *req;
    struct os_mbuf *txom;

    if (start_handle == 0 || start_handle > end_handle) {
        return BLE_HS_EINVAL;
    }

    req = ble_att_cmd_get(BLE_ATT_OP_READ_GROUP_TYPE_REQ,
                          sizeof(*req) + ble_uuid_length(uuid), &txom);
    if (req == NULL) {
        return BLE_HS_ENOMEM;
    }

    req->bagq_start_handle = htole16(start_handle);
    req->bagq_end_handle = htole16(end_handle);
    ble_uuid_flat(uuid, req->uuid);

    return ble_att_tx(conn_handle, txom);
}

static int
ble_att_clt_parse_read_group_type_adata(
    struct os_mbuf **om, int data_len,
    struct ble_att_read_group_type_adata *adata)
{
    int rc;

    if (data_len < BLE_ATT_READ_GROUP_TYPE_ADATA_BASE_SZ + 1) {
        return BLE_HS_EMSGSIZE;
    }

    rc = ble_hs_mbuf_pullup_base(om, data_len);
    if (rc != 0) {
        return rc;
    }

    adata->att_handle = get_le16((*om)->om_data + 0);
    adata->end_group_handle = get_le16((*om)->om_data + 2);
    adata->value_len = data_len - BLE_ATT_READ_GROUP_TYPE_ADATA_BASE_SZ;
    adata->value = (*om)->om_data + BLE_ATT_READ_GROUP_TYPE_ADATA_BASE_SZ;

    return 0;
}

int
ble_att_clt_rx_read_group_type(uint16_t conn_handle, struct os_mbuf **rxom)
{
#if !NIMBLE_BLE_ATT_CLT_READ_GROUP_TYPE
    return BLE_HS_ENOTSUP;
#endif

    struct ble_att_read_group_type_adata adata;
    struct ble_att_read_group_type_rsp *rsp;
    uint8_t len;
    int rc;

    rc = ble_hs_mbuf_pullup_base(rxom, sizeof(*rsp));
    if (rc != 0) {
        goto done;
    }

    rsp = (struct ble_att_read_group_type_rsp *)(*rxom)->om_data;

    len = rsp->bagp_length;

    /* Strip the base from the front of the response. */
    os_mbuf_adj(*rxom, sizeof(*rsp));

    /* Parse the Attribute Data List field, passing each entry to GATT. */
    while (OS_MBUF_PKTLEN(*rxom) > 0) {
        rc = ble_att_clt_parse_read_group_type_adata(rxom, len, &adata);
        if (rc != 0) {
            goto done;
        }

        ble_gattc_rx_read_group_type_adata(conn_handle, &adata);
        os_mbuf_adj(*rxom, len);
    }

done:
    /* Notify GATT that the response is done being parsed. */
    ble_gattc_rx_read_group_type_complete(conn_handle, rc);
    return rc;
}

/*****************************************************************************
 * $write                                                                    *
 *****************************************************************************/

int
ble_att_clt_tx_write_req(uint16_t conn_handle, uint16_t handle,
                         struct os_mbuf *txom)
{
#if !NIMBLE_BLE_ATT_CLT_WRITE
    return BLE_HS_ENOTSUP;
#endif

    struct ble_att_write_req *req;
    struct os_mbuf *txom2;

    req = ble_att_cmd_get(BLE_ATT_OP_WRITE_REQ, sizeof(*req), &txom2);
    if (req == NULL) {
        os_mbuf_free_chain(txom);
        return BLE_HS_ENOMEM;
    }

    req->bawq_handle = htole16(handle);
    os_mbuf_concat(txom2, txom);

    return ble_att_tx(conn_handle, txom2);
}

int
ble_att_clt_tx_write_cmd(uint16_t conn_handle, uint16_t handle,
                         struct os_mbuf *txom)
{
#if !NIMBLE_BLE_ATT_CLT_WRITE_NO_RSP
    return BLE_HS_ENOTSUP;
#endif

    struct ble_att_write_cmd *cmd;
    struct os_mbuf *txom2;
    uint8_t b;
    int rc;
    int i;

    BLE_HS_LOG(DEBUG, "ble_att_clt_tx_write_cmd(): ");
    for (i = 0; i < OS_MBUF_PKTLEN(txom); i++) {
        if (i != 0) {
            BLE_HS_LOG(DEBUG, ":");
        }
        rc = os_mbuf_copydata(txom, i, 1, &b);
        assert(rc == 0);
        BLE_HS_LOG(DEBUG, "0x%02x", b);
    }


    cmd = ble_att_cmd_get(BLE_ATT_OP_WRITE_CMD, sizeof(*cmd), &txom2);
    if (cmd == NULL) {
        os_mbuf_free_chain(txom);
        return BLE_HS_ENOMEM;
    }

    cmd->handle = htole16(handle);
    os_mbuf_concat(txom2, txom);

    return ble_att_tx(conn_handle, txom2);
}

int
ble_att_clt_rx_write(uint16_t conn_handle, struct os_mbuf **rxom)
{
#if !NIMBLE_BLE_ATT_CLT_WRITE
    return BLE_HS_ENOTSUP;
#endif

    /* No payload. */
    ble_gattc_rx_write_rsp(conn_handle);
    return 0;
}

/*****************************************************************************
 * $prepare write request                                                    *
 *****************************************************************************/

int
ble_att_clt_tx_prep_write(uint16_t conn_handle, uint16_t handle,
                          uint16_t offset, struct os_mbuf *txom)
{
#if !NIMBLE_BLE_ATT_CLT_PREP_WRITE
    return BLE_HS_ENOTSUP;
#endif

    struct ble_att_prep_write_cmd *req;
    struct os_mbuf *txom2;
    int rc;

    if (handle == 0) {
        rc = BLE_HS_EINVAL;
        goto err;
    }

    if (offset + OS_MBUF_PKTLEN(txom) > BLE_ATT_ATTR_MAX_LEN) {
        rc = BLE_HS_EINVAL;
        goto err;
    }

    if (OS_MBUF_PKTLEN(txom) >
        ble_att_mtu(conn_handle) - BLE_ATT_PREP_WRITE_CMD_BASE_SZ) {
        rc = BLE_HS_EINVAL;
        goto err;
    }

    req = ble_att_cmd_get(BLE_ATT_OP_PREP_WRITE_REQ, sizeof(*req), &txom2);
    if (req == NULL) {
        rc = BLE_HS_ENOMEM;
        goto err;
    }

    req->bapc_handle = htole16(handle);
    req->bapc_offset = htole16(offset);
    os_mbuf_concat(txom2, txom);

    return ble_att_tx(conn_handle, txom2);

err:
    os_mbuf_free_chain(txom);
    return rc;
}

int
ble_att_clt_rx_prep_write(uint16_t conn_handle, struct os_mbuf **rxom)
{
#if !NIMBLE_BLE_ATT_CLT_PREP_WRITE
    return BLE_HS_ENOTSUP;
#endif

    struct ble_att_prep_write_cmd *rsp;
    uint16_t handle, offset;
    int rc;

    /* Initialize some values in case of early error. */
    handle = 0;
    offset = 0;

    rc = ble_hs_mbuf_pullup_base(rxom, sizeof(*rsp));
    if (rc != 0) {
        goto done;
    }

    rsp = (struct ble_att_prep_write_cmd *)(*rxom)->om_data;

    handle = le16toh(rsp->bapc_handle);
    offset = le16toh(rsp->bapc_offset);

    /* Strip the base from the front of the response. */
    os_mbuf_adj(*rxom, sizeof(*rsp));

done:
    /* Notify GATT client that the full response has been parsed. */
    ble_gattc_rx_prep_write_rsp(conn_handle, rc, handle, offset, rxom);
    return rc;
}

/*****************************************************************************
 * $execute write request                                                    *
 *****************************************************************************/

int
ble_att_clt_tx_exec_write(uint16_t conn_handle, uint8_t flags)
{
#if !NIMBLE_BLE_ATT_CLT_EXEC_WRITE
    return BLE_HS_ENOTSUP;
#endif

    struct ble_att_exec_write_req *req;
    struct os_mbuf *txom;
    int rc;

    req = ble_att_cmd_get(BLE_ATT_OP_EXEC_WRITE_REQ, sizeof(*req), &txom);
    if (req == NULL) {
        return BLE_HS_ENOMEM;
    }

    req->baeq_flags = flags;

    rc = ble_att_tx(conn_handle, txom);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

int
ble_att_clt_rx_exec_write(uint16_t conn_handle, struct os_mbuf **rxom)
{
#if !NIMBLE_BLE_ATT_CLT_EXEC_WRITE
    return BLE_HS_ENOTSUP;
#endif

    ble_gattc_rx_exec_write_rsp(conn_handle, 0);
    return 0;
}

/*****************************************************************************
 * $handle value notification                                                *
 *****************************************************************************/

int
ble_att_clt_tx_notify(uint16_t conn_handle, uint16_t handle,
                      struct os_mbuf *txom)
{
#if !NIMBLE_BLE_ATT_CLT_NOTIFY
    return BLE_HS_ENOTSUP;
#endif

    struct ble_att_notify_req *req;
    struct os_mbuf *txom2;
    int rc;

    if (handle == 0) {
        rc = BLE_HS_EINVAL;
        goto err;
    }

    req = ble_att_cmd_get(BLE_ATT_OP_NOTIFY_REQ, sizeof(*req), &txom2);
    if (req == NULL) {
        rc = BLE_HS_ENOMEM;
        goto err;
    }

    req->banq_handle = htole16(handle);
    os_mbuf_concat(txom2, txom);

    return ble_att_tx(conn_handle, txom2);

err:
    os_mbuf_free_chain(txom);
    return rc;
}

/*****************************************************************************
 * $handle value indication                                                  *
 *****************************************************************************/

int
ble_att_clt_tx_indicate(uint16_t conn_handle, uint16_t handle,
                        struct os_mbuf *txom)
{
#if !NIMBLE_BLE_ATT_CLT_INDICATE
    return BLE_HS_ENOTSUP;
#endif

    struct ble_att_indicate_req *req;
    struct os_mbuf *txom2;
    int rc;

    if (handle == 0) {
        rc = BLE_HS_EINVAL;
        goto err;
    }

    req = ble_att_cmd_get(BLE_ATT_OP_INDICATE_REQ, sizeof(*req), &txom2);
    if (req == NULL) {
        rc = BLE_HS_ENOMEM;
        goto err;
    }

    req->baiq_handle = htole16(handle);
    os_mbuf_concat(txom2, txom);

    return ble_att_tx(conn_handle, txom2);

err:
    os_mbuf_free_chain(txom);
    return rc;
}

int
ble_att_clt_rx_indicate(uint16_t conn_handle, struct os_mbuf **rxom)
{
#if !NIMBLE_BLE_ATT_CLT_INDICATE
    return BLE_HS_ENOTSUP;
#endif

    /* No payload. */
    ble_gattc_rx_indicate_rsp(conn_handle);
    return 0;
}
