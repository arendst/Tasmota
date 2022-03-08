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

#include "nimble/nimble/host/include/host/ble_hs.h"
#include "ble_hs_priv.h"

/**
 * Allocates an mbuf for use by the nimble host.
 */
static struct os_mbuf *
ble_hs_mbuf_gen_pkt(uint16_t leading_space)
{
    struct os_mbuf *om;
    int rc;

    om = os_msys_get_pkthdr(0, 0);
    if (om == NULL) {
        return NULL;
    }

    if (om->om_omp->omp_databuf_len < leading_space) {
        rc = os_mbuf_free_chain(om);
        BLE_HS_DBG_ASSERT_EVAL(rc == 0);
        return NULL;
    }

    om->om_data += leading_space;

    return om;
}

/**
 * Allocates an mbuf with no leading space.
 *
 * @return                  An empty mbuf on success; null on memory
 *                              exhaustion.
 */
struct os_mbuf *
ble_hs_mbuf_bare_pkt(void)
{
    return ble_hs_mbuf_gen_pkt(0);
}

/**
 * Allocates an mbuf suitable for an HCI ACL data packet.
 *
 * @return                  An empty mbuf on success; null on memory
 *                              exhaustion.
 */
struct os_mbuf *
ble_hs_mbuf_acl_pkt(void)
{
    return ble_hs_mbuf_gen_pkt(BLE_HCI_DATA_HDR_SZ + 1);
}

/**
 * Allocates an mbuf suitable for an L2CAP data packet.  The resulting packet
 * has sufficient leading space for:
 *     o ACL data header
 *     o L2CAP B-frame header
 *
 * @return                  An empty mbuf on success; null on memory
 *                              exhaustion.
 */
struct os_mbuf *
ble_hs_mbuf_l2cap_pkt(void)
{
    return ble_hs_mbuf_gen_pkt(BLE_HCI_DATA_HDR_SZ + BLE_L2CAP_HDR_SZ + 1);
}

struct os_mbuf *
ble_hs_mbuf_att_pkt(void)
{
    /* Prepare write request and response are the larget ATT commands which
     * contain attribute data.
     */
     return ble_hs_mbuf_gen_pkt(BLE_HCI_DATA_HDR_SZ +
                                BLE_L2CAP_HDR_SZ +
                                BLE_ATT_PREP_WRITE_CMD_BASE_SZ);
}

struct os_mbuf *
ble_hs_mbuf_from_flat(const void *buf, uint16_t len)
{
    struct os_mbuf *om;
    int rc;

    om = ble_hs_mbuf_att_pkt();
    if (om == NULL) {
        return NULL;
    }

    rc = os_mbuf_copyinto(om, 0, buf, len);
    if (rc != 0) {
        os_mbuf_free_chain(om);
        return NULL;
    }

    return om;
}

int
ble_hs_mbuf_to_flat(const struct os_mbuf *om, void *flat, uint16_t max_len,
                    uint16_t *out_copy_len)
{
    uint16_t copy_len;
    int rc;

    if (OS_MBUF_PKTLEN(om) <= max_len) {
        copy_len = OS_MBUF_PKTLEN(om);
    } else {
        copy_len = max_len;
    }

    rc = os_mbuf_copydata(om, 0, copy_len, flat);
    if (rc != 0) {
        return BLE_HS_EUNKNOWN;
    }

    if (copy_len > max_len) {
        rc = BLE_HS_EMSGSIZE;
    } else {
        rc = 0;
    }

    if (out_copy_len != NULL) {
        *out_copy_len = copy_len;
    }
    return rc;
}

int
ble_hs_mbuf_pullup_base(struct os_mbuf **om, int base_len)
{
    if (OS_MBUF_PKTLEN(*om) < base_len) {
        return BLE_HS_EBADDATA;
    }

    *om = os_mbuf_pullup(*om, base_len);
    if (*om == NULL) {
        return BLE_HS_ENOMEM;
    }

    return 0;
}
