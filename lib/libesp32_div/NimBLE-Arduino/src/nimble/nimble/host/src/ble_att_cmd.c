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

#include <errno.h>
#include <string.h>
#include "nimble/porting/nimble/include/os/os.h"
#include "nimble/nimble/include/nimble/ble.h"
#include "ble_hs_priv.h"
#include "nimble/nimble/host/include/host/ble_att.h"
#include "nimble/nimble/host/include/host/ble_uuid.h"
#include "ble_hs_priv.h"

#if NIMBLE_BLE_CONNECT
void *
ble_att_cmd_prepare(uint8_t opcode, size_t len, struct os_mbuf *txom)
{
    struct ble_att_hdr *hdr;

    if (os_mbuf_extend(txom, sizeof(*hdr) + len) == NULL) {
        os_mbuf_free_chain(txom);
        return NULL;
    }

    hdr = (struct ble_att_hdr *)(txom)->om_data;

    hdr->opcode = opcode;

    return hdr->data;
}

void *
ble_att_cmd_get(uint8_t opcode, size_t len, struct os_mbuf **txom)
{
    *txom = ble_hs_mbuf_l2cap_pkt();
    if (*txom == NULL) {
        return NULL;
    }

    return ble_att_cmd_prepare(opcode, len, *txom);
}

int
ble_att_tx(uint16_t conn_handle, struct os_mbuf *txom)
{
    struct ble_l2cap_chan *chan;
    struct ble_hs_conn *conn;
    int rc;

    BLE_HS_DBG_ASSERT_EVAL(txom->om_len >= 1);
    ble_att_inc_tx_stat(txom->om_data[0]);

    ble_hs_lock();

    rc = ble_hs_misc_conn_chan_find_reqd(conn_handle, BLE_L2CAP_CID_ATT, &conn,
                                         &chan);
    if (rc != 0) {
        os_mbuf_free_chain(txom);
    } else {
        ble_att_truncate_to_mtu(chan, txom);
        rc = ble_l2cap_tx(conn, chan, txom);
    }

    ble_hs_unlock();

    return rc;
}

static const void *
ble_att_init_parse(uint8_t op, const void *payload,
                   int min_len, int actual_len)
{
    const uint8_t *u8ptr;

    BLE_HS_DBG_ASSERT(actual_len >= min_len);

    u8ptr = payload;
    BLE_HS_DBG_ASSERT(u8ptr[0] == op);

    return u8ptr + 1;
}

static void *
ble_att_init_write(uint8_t op, void *payload, int min_len, int actual_len)
{
    uint8_t *u8ptr;

    BLE_HS_DBG_ASSERT(actual_len >= min_len);

    u8ptr = payload;
    u8ptr[0] = op;

    return u8ptr + 1;
}

void
ble_att_error_rsp_parse(const void *payload, int len,
                        struct ble_att_error_rsp *dst)
{
    const struct ble_att_error_rsp *src;

    src = ble_att_init_parse(BLE_ATT_OP_ERROR_RSP, payload,
                             BLE_ATT_ERROR_RSP_SZ, len);

    dst->baep_req_op = src->baep_req_op;
    dst->baep_handle = le16toh(src->baep_handle);
    dst->baep_error_code = src->baep_error_code;
}

void
ble_att_error_rsp_write(void *payload, int len,
                        const struct ble_att_error_rsp *src)
{
    struct ble_att_error_rsp *dst;

    dst = ble_att_init_write(BLE_ATT_OP_ERROR_RSP, payload,
                             BLE_ATT_ERROR_RSP_SZ, len);

    dst->baep_req_op = src->baep_req_op;
    dst->baep_handle = htole16(src->baep_handle);
    dst->baep_error_code = src->baep_error_code;
}

void
ble_att_mtu_req_parse(const void *payload, int len,
                      struct ble_att_mtu_cmd *dst)
{
    const struct ble_att_mtu_cmd *src;

    src = ble_att_init_parse(BLE_ATT_OP_MTU_REQ, payload, BLE_ATT_MTU_CMD_SZ,
                             len);

    dst->bamc_mtu = le16toh(src->bamc_mtu);
}

void
ble_att_mtu_rsp_parse(const void *payload, int len,
                      struct ble_att_mtu_cmd *dst)
{
    const struct ble_att_mtu_cmd *src;

    src = ble_att_init_parse(BLE_ATT_OP_MTU_RSP, payload, BLE_ATT_MTU_CMD_SZ,
                             len);

    dst->bamc_mtu = le16toh(src->bamc_mtu);
}

void
ble_att_mtu_req_write(void *payload, int len,
                      const struct ble_att_mtu_cmd *src)
{
    struct ble_att_mtu_cmd *dst;

    dst = ble_att_init_write(BLE_ATT_OP_MTU_REQ, payload,
                             BLE_ATT_MTU_CMD_SZ, len);

    dst->bamc_mtu = htole16(src->bamc_mtu);
}

void
ble_att_mtu_rsp_write(void *payload, int len,
                      const struct ble_att_mtu_cmd *src)
{
    struct ble_att_mtu_cmd *dst;

    dst = ble_att_init_write(BLE_ATT_OP_MTU_RSP, payload,
                             BLE_ATT_MTU_CMD_SZ, len);
    dst->bamc_mtu = htole16(src->bamc_mtu);
}

void
ble_att_find_info_req_parse(const void *payload, int len,
                            struct ble_att_find_info_req *dst)
{
    const struct ble_att_find_info_req *src;

    src = ble_att_init_parse(BLE_ATT_OP_FIND_INFO_REQ, payload,
                             BLE_ATT_FIND_INFO_REQ_SZ, len);

    dst->bafq_start_handle = le16toh(src->bafq_start_handle);
    dst->bafq_end_handle = le16toh(src->bafq_end_handle);
}

void
ble_att_find_info_req_write(void *payload, int len,
                            const struct ble_att_find_info_req *src)
{
    struct ble_att_find_info_req *dst;

    dst = ble_att_init_write(BLE_ATT_OP_FIND_INFO_REQ, payload,
                             BLE_ATT_FIND_INFO_REQ_SZ, len);

    dst->bafq_start_handle = htole16(src->bafq_start_handle);
    dst->bafq_end_handle = htole16(src->bafq_end_handle);
}

void
ble_att_find_info_rsp_parse(const void *payload, int len,
                            struct ble_att_find_info_rsp *dst)
{
    const struct ble_att_find_info_rsp *src;

    src = ble_att_init_parse(BLE_ATT_OP_FIND_INFO_RSP, payload,
                             BLE_ATT_FIND_INFO_RSP_BASE_SZ, len);

    dst->bafp_format = src->bafp_format;
}

void
ble_att_find_info_rsp_write(void *payload, int len,
                            const struct ble_att_find_info_rsp *src)
{
    struct ble_att_find_info_rsp *dst;

    dst = ble_att_init_write(BLE_ATT_OP_FIND_INFO_RSP, payload,
                             BLE_ATT_FIND_INFO_RSP_BASE_SZ, len);

    dst->bafp_format = src->bafp_format;
}

void
ble_att_find_type_value_req_parse(const void *payload, int len,
                                  struct ble_att_find_type_value_req *dst)
{
    const struct ble_att_find_type_value_req *src;

    src = ble_att_init_parse(BLE_ATT_OP_FIND_TYPE_VALUE_REQ, payload,
                             BLE_ATT_FIND_TYPE_VALUE_REQ_BASE_SZ, len);

    dst->bavq_start_handle = le16toh(src->bavq_start_handle);
    dst->bavq_end_handle = le16toh(src->bavq_end_handle);
    dst->bavq_attr_type = le16toh(src->bavq_attr_type);
}

void
ble_att_find_type_value_req_write(
    void *payload, int len, const struct ble_att_find_type_value_req *src)
{
    struct ble_att_find_type_value_req *dst;

    dst = ble_att_init_write(BLE_ATT_OP_FIND_TYPE_VALUE_REQ, payload,
                             BLE_ATT_FIND_TYPE_VALUE_REQ_BASE_SZ, len);

    dst->bavq_start_handle = htole16(src->bavq_start_handle);
    dst->bavq_end_handle = htole16(src->bavq_end_handle);
    dst->bavq_attr_type = htole16(src->bavq_attr_type);
}

void
ble_att_read_type_req_parse(const void *payload, int len,
                            struct ble_att_read_type_req *dst)
{
    const struct ble_att_read_type_req *src;

    src = ble_att_init_parse(BLE_ATT_OP_READ_TYPE_REQ, payload,
                             BLE_ATT_READ_TYPE_REQ_BASE_SZ, len);

    dst->batq_start_handle = le16toh(src->batq_start_handle);
    dst->batq_end_handle = le16toh(src->batq_end_handle);
}

void
ble_att_read_type_req_write(void *payload, int len,
                            const struct ble_att_read_type_req *src)
{
    struct ble_att_read_type_req *dst;

    dst = ble_att_init_write(BLE_ATT_OP_READ_TYPE_REQ, payload,
                             BLE_ATT_READ_TYPE_REQ_BASE_SZ, len);

    dst->batq_start_handle = htole16(src->batq_start_handle);
    dst->batq_end_handle = htole16(src->batq_end_handle);
}

void
ble_att_read_type_rsp_parse(const void *payload, int len,
                            struct ble_att_read_type_rsp *dst)
{
    const struct ble_att_read_type_rsp *src;

    src = ble_att_init_parse(BLE_ATT_OP_READ_TYPE_RSP, payload,
                             BLE_ATT_READ_TYPE_RSP_BASE_SZ, len);

    dst->batp_length = src->batp_length;
}

void
ble_att_read_type_rsp_write(void *payload, int len,
                            const struct ble_att_read_type_rsp *src)
{
    struct ble_att_read_type_rsp *dst;

    dst = ble_att_init_write(BLE_ATT_OP_READ_TYPE_RSP, payload,
                             BLE_ATT_READ_TYPE_RSP_BASE_SZ, len);

    dst->batp_length = src->batp_length;
}

void
ble_att_read_req_parse(const void *payload, int len,
                       struct ble_att_read_req *dst)
{
    const struct ble_att_read_req *src;

    src = ble_att_init_parse(BLE_ATT_OP_READ_REQ, payload,
                             BLE_ATT_READ_REQ_SZ, len);

    dst->barq_handle = le16toh(src->barq_handle);
}

void
ble_att_read_req_write(void *payload, int len,
                       const struct ble_att_read_req *src)
{
    struct ble_att_read_req *dst;

    dst = ble_att_init_write(BLE_ATT_OP_READ_REQ, payload,
                             BLE_ATT_READ_REQ_SZ, len);

    dst->barq_handle = htole16(src->barq_handle);
}

void
ble_att_read_blob_req_parse(const void *payload, int len,
                            struct ble_att_read_blob_req *dst)
{
    const struct ble_att_read_blob_req *src;

    src = ble_att_init_parse(BLE_ATT_OP_READ_BLOB_REQ, payload,
                             BLE_ATT_READ_BLOB_REQ_SZ, len);

    dst->babq_handle = le16toh(src->babq_handle);
    dst->babq_offset = le16toh(src->babq_offset);
}

void
ble_att_read_blob_req_write(void *payload, int len,
                            const struct ble_att_read_blob_req *src)
{
    struct ble_att_read_blob_req *dst;

    dst = ble_att_init_write(BLE_ATT_OP_READ_BLOB_REQ, payload,
                             BLE_ATT_READ_BLOB_REQ_SZ, len);

    dst->babq_handle = htole16(src->babq_handle);
    dst->babq_offset = htole16(src->babq_offset);
}

void
ble_att_read_mult_req_parse(const void *payload, int len)
{
    ble_att_init_parse(BLE_ATT_OP_READ_MULT_REQ, payload,
                       BLE_ATT_READ_MULT_REQ_BASE_SZ, len);
}

void
ble_att_read_mult_req_write(void *payload, int len)
{
    ble_att_init_write(BLE_ATT_OP_READ_MULT_REQ, payload,
                       BLE_ATT_READ_MULT_REQ_BASE_SZ, len);
}

void
ble_att_read_mult_rsp_parse(const void *payload, int len)
{
    ble_att_init_parse(BLE_ATT_OP_READ_MULT_RSP, payload,
                       BLE_ATT_READ_MULT_RSP_BASE_SZ, len);
}

void
ble_att_read_mult_rsp_write(void *payload, int len)
{
    ble_att_init_write(BLE_ATT_OP_READ_MULT_RSP, payload,
                       BLE_ATT_READ_MULT_RSP_BASE_SZ, len);
}

void
ble_att_read_group_type_req_parse(const void *payload, int len,
                                  struct ble_att_read_group_type_req *dst)
{
    const struct ble_att_read_group_type_req *src;

    src = ble_att_init_parse(BLE_ATT_OP_READ_GROUP_TYPE_REQ, payload,
                             BLE_ATT_READ_GROUP_TYPE_REQ_BASE_SZ, len);

    dst->bagq_start_handle = le16toh(src->bagq_start_handle);
    dst->bagq_end_handle = le16toh(src->bagq_end_handle);
}

void
ble_att_read_group_type_req_write(
    void *payload, int len, const struct ble_att_read_group_type_req *src)
{
    struct ble_att_read_group_type_req *dst;

    dst = ble_att_init_write(BLE_ATT_OP_READ_GROUP_TYPE_REQ, payload,
                             BLE_ATT_READ_GROUP_TYPE_REQ_BASE_SZ, len);

    dst->bagq_start_handle = htole16(src->bagq_start_handle);
    dst->bagq_end_handle = htole16(src->bagq_end_handle);
}

void
ble_att_read_group_type_rsp_parse(const void *payload, int len,
                                  struct ble_att_read_group_type_rsp *dst)
{
    const struct ble_att_read_group_type_rsp *src;

    src = ble_att_init_parse(BLE_ATT_OP_READ_GROUP_TYPE_RSP, payload,
                             BLE_ATT_READ_GROUP_TYPE_RSP_BASE_SZ, len);

    dst->bagp_length = src->bagp_length;
}

void
ble_att_read_group_type_rsp_write(
    void *payload, int len, const struct ble_att_read_group_type_rsp *src)
{
    struct ble_att_read_group_type_rsp *dst;

    dst = ble_att_init_write(BLE_ATT_OP_READ_GROUP_TYPE_RSP, payload,
                             BLE_ATT_READ_GROUP_TYPE_RSP_BASE_SZ, len);

    dst->bagp_length = src->bagp_length;
}

void
ble_att_write_req_parse(const void *payload, int len,
                        struct ble_att_write_req *dst)
{
    const struct ble_att_write_req *src;

    src = ble_att_init_parse(BLE_ATT_OP_WRITE_REQ, payload,
                             BLE_ATT_WRITE_REQ_BASE_SZ, len);

    dst->bawq_handle = le16toh(src->bawq_handle);
}

void
ble_att_write_cmd_parse(const void *payload, int len,
                        struct ble_att_write_req *dst)
{
    const struct ble_att_write_req *src;

    src = ble_att_init_parse(BLE_ATT_OP_WRITE_CMD, payload,
                             BLE_ATT_WRITE_REQ_BASE_SZ, len);
    dst->bawq_handle = le16toh(src->bawq_handle);
}

void
ble_att_write_req_write(void *payload, int len,
                        const struct ble_att_write_req *src)
{
    struct ble_att_write_req *dst;

    dst = ble_att_init_write(BLE_ATT_OP_WRITE_REQ, payload,
                             BLE_ATT_WRITE_REQ_BASE_SZ, len);
    dst->bawq_handle = htole16(src->bawq_handle);
}

void
ble_att_write_cmd_write(void *payload, int len,
                        const struct ble_att_write_req *src)
{
    struct ble_att_write_req *dst;

    dst = ble_att_init_write(BLE_ATT_OP_WRITE_CMD, payload,
                             BLE_ATT_WRITE_REQ_BASE_SZ, len);
    dst->bawq_handle = htole16(src->bawq_handle);
}

void
ble_att_prep_write_req_parse(const void *payload, int len,
                             struct ble_att_prep_write_cmd *dst)
{
    const struct ble_att_prep_write_cmd *src;

    src = ble_att_init_parse(BLE_ATT_OP_PREP_WRITE_REQ, payload,
                             BLE_ATT_PREP_WRITE_CMD_BASE_SZ, len);

    dst->bapc_handle = le16toh(src->bapc_handle);
    dst->bapc_offset = le16toh(src->bapc_offset);
}

void
ble_att_prep_write_req_write(void *payload, int len,
                             const struct ble_att_prep_write_cmd *src)
{
    struct ble_att_prep_write_cmd *dst;

    dst = ble_att_init_write(BLE_ATT_OP_PREP_WRITE_REQ, payload,
                             BLE_ATT_PREP_WRITE_CMD_BASE_SZ, len);

    dst->bapc_handle = htole16(src->bapc_handle);
    dst->bapc_offset = htole16(src->bapc_offset);
}

void
ble_att_prep_write_rsp_parse(const void *payload, int len,
                             struct ble_att_prep_write_cmd *dst)
{
    const struct ble_att_prep_write_cmd *src;

    src = ble_att_init_parse(BLE_ATT_OP_PREP_WRITE_RSP, payload,
                             BLE_ATT_PREP_WRITE_CMD_BASE_SZ, len);

    dst->bapc_handle = le16toh(src->bapc_handle);
    dst->bapc_offset = le16toh(src->bapc_offset);
}

void
ble_att_prep_write_rsp_write(void *payload, int len,
                             const struct ble_att_prep_write_cmd *src)
{
    struct ble_att_prep_write_cmd *dst;

    dst = ble_att_init_write(BLE_ATT_OP_PREP_WRITE_RSP, payload,
                             BLE_ATT_PREP_WRITE_CMD_BASE_SZ, len);

    dst->bapc_handle = htole16(src->bapc_handle);
    dst->bapc_offset = htole16(src->bapc_offset);
}

void
ble_att_exec_write_req_parse(const void *payload, int len,
                             struct ble_att_exec_write_req *dst)
{
    const struct ble_att_exec_write_req *src;

    src = ble_att_init_parse(BLE_ATT_OP_EXEC_WRITE_REQ, payload,
                             BLE_ATT_EXEC_WRITE_REQ_SZ, len);

    dst->baeq_flags = src->baeq_flags;
}

void
ble_att_exec_write_req_write(void *payload, int len,
                             const struct ble_att_exec_write_req *src)
{
    struct ble_att_exec_write_req *dst;

    dst = ble_att_init_write(BLE_ATT_OP_EXEC_WRITE_REQ, payload,
                             BLE_ATT_EXEC_WRITE_REQ_SZ, len);

    dst->baeq_flags = src->baeq_flags;
}

void
ble_att_exec_write_rsp_parse(const void *payload, int len)
{
    ble_att_init_parse(BLE_ATT_OP_EXEC_WRITE_RSP, payload,
                       BLE_ATT_EXEC_WRITE_RSP_SZ, len);
}

void
ble_att_exec_write_rsp_write(void *payload, int len)
{
    ble_att_init_write(BLE_ATT_OP_EXEC_WRITE_RSP, payload,
                       BLE_ATT_EXEC_WRITE_RSP_SZ, len);
}

void
ble_att_notify_req_parse(const void *payload, int len,
                         struct ble_att_notify_req *dst)
{
    const struct ble_att_notify_req *src;

    src = ble_att_init_parse(BLE_ATT_OP_NOTIFY_REQ, payload,
                             BLE_ATT_NOTIFY_REQ_BASE_SZ, len);

    dst->banq_handle = le16toh(src->banq_handle);
}

void
ble_att_notify_req_write(void *payload, int len,
                         const struct ble_att_notify_req *src)
{
    struct ble_att_notify_req *dst;

    dst = ble_att_init_write(BLE_ATT_OP_NOTIFY_REQ, payload,
                             BLE_ATT_NOTIFY_REQ_BASE_SZ, len);

    dst->banq_handle = htole16(src->banq_handle);
}

void
ble_att_indicate_req_parse(const void *payload, int len,
                           struct ble_att_indicate_req *dst)
{
    const struct ble_att_indicate_req *src;

    src = ble_att_init_parse(BLE_ATT_OP_INDICATE_REQ, payload,
                             BLE_ATT_INDICATE_REQ_BASE_SZ, len);

    dst->baiq_handle = le16toh(src->baiq_handle);
}

void
ble_att_indicate_req_write(void *payload, int len,
                           const struct ble_att_indicate_req *src)
{
    struct ble_att_indicate_req *dst;

    dst = ble_att_init_write(BLE_ATT_OP_INDICATE_REQ, payload,
                             BLE_ATT_INDICATE_REQ_BASE_SZ, len);

    dst->baiq_handle = htole16(src->baiq_handle);
}

void
ble_att_indicate_rsp_parse(const void *payload, int len)
{
    ble_att_init_parse(BLE_ATT_OP_INDICATE_RSP, payload,
                       BLE_ATT_INDICATE_RSP_SZ, len);
}

void
ble_att_indicate_rsp_write(void *payload, int len)
{
    ble_att_init_write(BLE_ATT_OP_INDICATE_RSP, payload,
                       BLE_ATT_INDICATE_RSP_SZ, len);
}

#endif
