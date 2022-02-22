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
#include <errno.h>
#include "nimble/nimble/include/nimble/ble.h"
#include "nimble/nimble/host/include/host/ble_hs_adv.h"
#include "ble_hs_priv.h"

struct find_field_data {
    uint8_t type;
    const struct ble_hs_adv_field *field;
};

static ble_uuid16_t ble_hs_adv_uuids16[BLE_HS_ADV_MAX_FIELD_SZ / 2];
static ble_uuid32_t ble_hs_adv_uuids32[BLE_HS_ADV_MAX_FIELD_SZ / 4];
static ble_uuid128_t ble_hs_adv_uuids128[BLE_HS_ADV_MAX_FIELD_SZ / 16];

static int
ble_hs_adv_set_hdr(uint8_t type, uint8_t data_len, uint8_t max_len,
                   uint8_t *dst, uint8_t *dst_len, struct os_mbuf *om)
{
    int rc;

    if (om ) {
        data_len++;
        rc = os_mbuf_append(om, &data_len, sizeof(data_len));
        if (rc) {
            return rc;
        }

        return os_mbuf_append(om, &type, sizeof(type));
    }


    if (*dst_len + 2 + data_len > max_len) {
        return BLE_HS_EMSGSIZE;
    }

    dst[*dst_len] = data_len + 1;
    dst[*dst_len + 1] = type;

    *dst_len += 2;

    return 0;
}

static int
ble_hs_adv_set_flat_mbuf(uint8_t type, int data_len, const void *data,
                         uint8_t *dst, uint8_t *dst_len, uint8_t max_len,
                         struct os_mbuf *om)
{
    int rc;

    BLE_HS_DBG_ASSERT(data_len > 0);

    rc = ble_hs_adv_set_hdr(type, data_len, max_len, dst, dst_len, om);
    if (rc != 0) {
        return rc;
    }

    if (om) {
        return os_mbuf_append(om, data, data_len);
    }

    memcpy(dst + *dst_len, data, data_len);
    *dst_len += data_len;

    return 0;
}

int
ble_hs_adv_set_flat(uint8_t type, int data_len, const void *data,
                    uint8_t *dst, uint8_t *dst_len, uint8_t max_len)
{
#if !NIMBLE_BLE_ADVERTISE
    return BLE_HS_ENOTSUP;
#endif

    return ble_hs_adv_set_flat_mbuf(type, data_len, data, dst, dst_len, max_len,
                                    NULL);
}

static int
ble_hs_adv_set_array_uuid16(uint8_t type, uint8_t num_elems,
                            const ble_uuid16_t *elems, uint8_t *dst,
                            uint8_t *dst_len, uint8_t max_len,
                            struct os_mbuf *om)
{
    int rc;
    int i;

    rc = ble_hs_adv_set_hdr(type, num_elems * 2, max_len, dst,
                            dst_len, om);
    if (rc != 0) {
        return rc;
    }

    for (i = 0; i < num_elems; i++) {
        if (om) {
            rc = ble_uuid_to_mbuf(&elems[i].u, om);
            if (rc) {
                return rc;
            }
        } else {
            ble_uuid_flat(&elems[i].u, dst + *dst_len);
            *dst_len += 2;
        }
    }

    return 0;
}

static int
ble_hs_adv_set_array_uuid32(uint8_t type, uint8_t num_elems,
                            const ble_uuid32_t *elems, uint8_t *dst,
                            uint8_t *dst_len, uint8_t max_len,
                            struct os_mbuf *om)
{
    uint32_t uuid_le;
    int rc;
    int i;

    rc = ble_hs_adv_set_hdr(type, num_elems * 4, max_len, dst,
                            dst_len, om);
    if (rc != 0) {
        return rc;
    }

    for (i = 0; i < num_elems; i++) {
        /* We cannot use ble_uuid_flat here since it converts 32-bit UUIDs to
         * 128-bit as ATT requires. In AD, 32-bit UUID shall be written as an
         * actual 32-bit value.
         */
        if (om) {
            uuid_le = htole32(elems[i].value);
            rc = os_mbuf_append(om, &uuid_le, sizeof(uuid_le));
            if (rc) {
                return rc;
            }
        } else {
            put_le32(dst + *dst_len, elems[i].value);
            *dst_len += 4;
        }
    }

    return 0;
}

static int
ble_hs_adv_set_array_uuid128(uint8_t type, uint8_t num_elems,
                             const ble_uuid128_t *elems, uint8_t *dst,
                             uint8_t *dst_len, uint8_t max_len,
                             struct os_mbuf *om)
{
    int rc;
    int i;

    rc = ble_hs_adv_set_hdr(type, num_elems * 16, max_len, dst,
                            dst_len, om);
    if (rc != 0) {
        return rc;
    }

    for (i = 0; i < num_elems; i++) {
        if (om) {
            rc = ble_uuid_to_mbuf(&elems[i].u, om);
            if (rc) {
                return rc;
            }
        } else {
            ble_uuid_flat(&elems[i].u, dst + *dst_len);
            *dst_len += 16;
        }
    }

    return 0;
}

static int
ble_hs_adv_set_array16(uint8_t type, uint8_t num_elems, const uint16_t *elems,
                       uint8_t *dst, uint8_t *dst_len, uint8_t max_len,
                       struct os_mbuf *om)
{
    uint16_t tmp;
    int rc;
    int i;

    rc = ble_hs_adv_set_hdr(type, num_elems * sizeof *elems, max_len, dst,
                            dst_len, om);
    if (rc != 0) {
        return rc;
    }

    for (i = 0; i < num_elems; i++) {
        if (om) {
            tmp = htole16(elems[i]);
            rc = os_mbuf_append(om, &tmp, sizeof(tmp));
            if (rc) {
                return rc;
            }
        } else {
            put_le16(dst + *dst_len, elems[i]);
            *dst_len += sizeof elems[i];
        }
    }

    return 0;
}

static int
adv_set_fields(const struct ble_hs_adv_fields *adv_fields,
                      uint8_t *dst, uint8_t *dst_len, uint8_t max_len,
                      struct os_mbuf *om)
{
#if !NIMBLE_BLE_ADVERTISE
    return BLE_HS_ENOTSUP;
#endif

    uint8_t type;
    int8_t tx_pwr_lvl;
    uint8_t dst_len_local;
    int rc;

    dst_len_local = 0;

    /*** 0x01 - Flags. */
    /* The application has two options concerning the flags field:
     * 1. Don't include it in advertisements (flags == 0).
     * 2. Explicitly specify the value (flags != 0).
     *
     * Note: The CSS prohibits advertising a flags value of 0, so this method
     * of specifying option 1 vs. 2 is sound.
     */
    if (adv_fields->flags != 0) {
        rc = ble_hs_adv_set_flat_mbuf(BLE_HS_ADV_TYPE_FLAGS, 1,
                                      &adv_fields->flags, dst, &dst_len_local,
                                      max_len, om);

        if (rc != 0) {
            return rc;
        }
    }

    /*** 0x02,0x03 - 16-bit service class UUIDs. */
    if (adv_fields->uuids16 != NULL && adv_fields->num_uuids16) {
        if (adv_fields->uuids16_is_complete) {
            type = BLE_HS_ADV_TYPE_COMP_UUIDS16;
        } else {
            type = BLE_HS_ADV_TYPE_INCOMP_UUIDS16;
        }

        rc = ble_hs_adv_set_array_uuid16(type, adv_fields->num_uuids16,
                                         adv_fields->uuids16, dst, &dst_len_local,
                                         max_len, om);
        if (rc != 0) {
            return rc;
        }
    }

    /*** 0x04,0x05 - 32-bit service class UUIDs. */
    if (adv_fields->uuids32 != NULL && adv_fields->num_uuids32) {
        if (adv_fields->uuids32_is_complete) {
            type = BLE_HS_ADV_TYPE_COMP_UUIDS32;
        } else {
            type = BLE_HS_ADV_TYPE_INCOMP_UUIDS32;
        }

        rc = ble_hs_adv_set_array_uuid32(type, adv_fields->num_uuids32,
                                         adv_fields->uuids32, dst, &dst_len_local,
                                         max_len, om);
        if (rc != 0) {
            return rc;
        }
    }

    /*** 0x06,0x07 - 128-bit service class UUIDs. */
    if (adv_fields->uuids128 != NULL && adv_fields->num_uuids128 > 0) {
        if (adv_fields->uuids128_is_complete) {
            type = BLE_HS_ADV_TYPE_COMP_UUIDS128;
        } else {
            type = BLE_HS_ADV_TYPE_INCOMP_UUIDS128;
        }

        rc = ble_hs_adv_set_array_uuid128(type, adv_fields->num_uuids128,
                                          adv_fields->uuids128, dst, &dst_len_local,
                                          max_len, om);
        if (rc != 0) {
            return rc;
        }
    }

    /*** 0x08,0x09 - Local name. */
    if (adv_fields->name != NULL && adv_fields->name_len > 0) {
        if (adv_fields->name_is_complete) {
            type = BLE_HS_ADV_TYPE_COMP_NAME;
        } else {
            type = BLE_HS_ADV_TYPE_INCOMP_NAME;
        }

        rc = ble_hs_adv_set_flat_mbuf(type, adv_fields->name_len,
                                      adv_fields->name, dst, &dst_len_local, max_len,
                                      om);
        if (rc != 0) {
            return rc;
        }
    }

    /*** 0x0a - Tx power level. */
    if (adv_fields->tx_pwr_lvl_is_present) {
        /* Read the power level from the controller if requested; otherwise use
         * the explicitly specified value.
         */
        if (adv_fields->tx_pwr_lvl == BLE_HS_ADV_TX_PWR_LVL_AUTO) {
            rc = ble_hs_hci_util_read_adv_tx_pwr(&tx_pwr_lvl);
            if (rc != 0) {
                return rc;
            }
        } else {
            tx_pwr_lvl = adv_fields->tx_pwr_lvl;
        }

        rc = ble_hs_adv_set_flat_mbuf(BLE_HS_ADV_TYPE_TX_PWR_LVL, 1,
                                      &tx_pwr_lvl, dst, &dst_len_local, max_len, om);
        if (rc != 0) {
            return rc;
        }
    }

    /*** 0x12 - Slave connection interval range. */
    if (adv_fields->slave_itvl_range != NULL) {
        rc = ble_hs_adv_set_flat_mbuf(BLE_HS_ADV_TYPE_SLAVE_ITVL_RANGE,
                                      BLE_HS_ADV_SLAVE_ITVL_RANGE_LEN,
                                      adv_fields->slave_itvl_range, dst,
                                      &dst_len_local, max_len, om);
        if (rc != 0) {
            return rc;
        }
    }

    /*** 0x16 - Service data - 16-bit UUID. */
    if (adv_fields->svc_data_uuid16 != NULL && adv_fields->svc_data_uuid16_len) {
        rc = ble_hs_adv_set_flat_mbuf(BLE_HS_ADV_TYPE_SVC_DATA_UUID16,
                                      adv_fields->svc_data_uuid16_len,
                                      adv_fields->svc_data_uuid16, dst, &dst_len_local,
                                      max_len, om);
        if (rc != 0) {
            return rc;
        }
    }

    /*** 0x17 - Public target address. */
    if (adv_fields->public_tgt_addr != NULL &&
        adv_fields->num_public_tgt_addrs != 0) {

        rc = ble_hs_adv_set_flat_mbuf(BLE_HS_ADV_TYPE_PUBLIC_TGT_ADDR,
                                 BLE_HS_ADV_PUBLIC_TGT_ADDR_ENTRY_LEN *
                                     adv_fields->num_public_tgt_addrs,
                                 adv_fields->public_tgt_addr, dst, &dst_len_local,
                                 max_len, om);
        if (rc != 0) {
            return rc;
        }
    }

    /*** 0x19 - Appearance. */
    if (adv_fields->appearance_is_present) {
        rc = ble_hs_adv_set_flat_mbuf(BLE_HS_ADV_TYPE_APPEARANCE,
                                      BLE_HS_ADV_APPEARANCE_LEN,
                                      &adv_fields->appearance, dst, &dst_len_local,
                                      max_len, om);
        if (rc != 0) {
            return rc;
        }
    }

    /*** 0x1a - Advertising interval. */
    if (adv_fields->adv_itvl_is_present) {
        rc = ble_hs_adv_set_array16(BLE_HS_ADV_TYPE_ADV_ITVL, 1,
                                    &adv_fields->adv_itvl, dst, &dst_len_local,
                                    max_len, om);
        if (rc != 0) {
            return rc;
        }
    }

    /*** 0x20 - Service data - 32-bit UUID. */
    if (adv_fields->svc_data_uuid32 != NULL && adv_fields->svc_data_uuid32_len) {
        rc = ble_hs_adv_set_flat_mbuf(BLE_HS_ADV_TYPE_SVC_DATA_UUID32,
                                     adv_fields->svc_data_uuid32_len,
                                     adv_fields->svc_data_uuid32, dst, &dst_len_local,
                                     max_len, om);
        if (rc != 0) {
            return rc;
        }
    }

    /*** 0x21 - Service data - 128-bit UUID. */
    if (adv_fields->svc_data_uuid128 != NULL && adv_fields->svc_data_uuid128_len) {
        rc = ble_hs_adv_set_flat_mbuf(BLE_HS_ADV_TYPE_SVC_DATA_UUID128,
                                      adv_fields->svc_data_uuid128_len,
                                      adv_fields->svc_data_uuid128, dst,
                                      &dst_len_local, max_len, om);
        if (rc != 0) {
            return rc;
        }
    }

    /*** 0x24 - URI. */
    if (adv_fields->uri != NULL && adv_fields->uri_len) {
        rc = ble_hs_adv_set_flat_mbuf(BLE_HS_ADV_TYPE_URI, adv_fields->uri_len,
                                      adv_fields->uri, dst, &dst_len_local, max_len,
                                      om);
        if (rc != 0) {
            return rc;
        }
    }

    /*** 0xff - Manufacturer specific data. */
    if ((adv_fields->mfg_data != NULL) && (adv_fields->mfg_data_len >= 2)) {
        rc = ble_hs_adv_set_flat_mbuf(BLE_HS_ADV_TYPE_MFG_DATA,
                                      adv_fields->mfg_data_len,
                                      adv_fields->mfg_data,
                                      dst, &dst_len_local, max_len, om);
        if (rc != 0) {
            return rc;
        }
    }

    if (dst_len) {
        *dst_len = dst_len_local;
    }

    return 0;
}

/**
 * Converts a high-level set of fields to a byte buffer.
 *
 * @return                      0 on success; nonzero on failure.
 */
int
ble_hs_adv_set_fields(const struct ble_hs_adv_fields *adv_fields,
                      uint8_t *dst, uint8_t *dst_len, uint8_t max_len)
{
#if !NIMBLE_BLE_ADVERTISE
    return BLE_HS_ENOTSUP;
#endif

    return adv_set_fields(adv_fields, dst, dst_len, max_len, NULL);
}

int
ble_hs_adv_set_fields_mbuf(const struct ble_hs_adv_fields *adv_fields,
                           struct os_mbuf *om)
{
#if !NIMBLE_BLE_ADVERTISE
    return BLE_HS_ENOTSUP;
#endif
    return adv_set_fields(adv_fields, NULL, NULL, 0, om);
}

static int
ble_hs_adv_parse_uuids16(struct ble_hs_adv_fields *adv_fields,
                         const uint8_t *data, uint8_t data_len)
{
    ble_uuid_any_t uuid;
    int i;

    if (data_len % 2 != 0) {
        return BLE_HS_EBADDATA;
    }

    adv_fields->uuids16 = ble_hs_adv_uuids16;
    adv_fields->num_uuids16 = data_len / 2;

    for (i = 0; i < adv_fields->num_uuids16; i++) {
        ble_uuid_init_from_buf(&uuid, data + i * 2, 2);
        ble_hs_adv_uuids16[i] = uuid.u16;
    }

    return 0;
}

static int
ble_hs_adv_parse_uuids32(struct ble_hs_adv_fields *adv_fields,
                         const uint8_t *data, uint8_t data_len)
{
    ble_uuid_any_t uuid;
    int i;

    if (data_len % 4 != 0) {
        return BLE_HS_EBADDATA;
    }

    adv_fields->uuids32 = ble_hs_adv_uuids32;
    adv_fields->num_uuids32 = data_len / 4;

    for (i = 0; i < adv_fields->num_uuids32; i++) {
        ble_uuid_init_from_buf(&uuid, data + i * 4, 4);
        ble_hs_adv_uuids32[i] = uuid.u32;
    }

    return 0;
}

static int
ble_hs_adv_parse_uuids128(struct ble_hs_adv_fields *adv_fields,
                          const uint8_t *data, uint8_t data_len)
{
    ble_uuid_any_t uuid;
    int i;

    if (data_len % 16 != 0) {
        return BLE_HS_EBADDATA;
    }

    adv_fields->uuids128 = ble_hs_adv_uuids128;
    adv_fields->num_uuids128 = data_len / 16;

    for (i = 0; i < adv_fields->num_uuids128; i++) {
        ble_uuid_init_from_buf(&uuid, data + i * 16, 16);
        ble_hs_adv_uuids128[i] = uuid.u128;
    }

    return 0;
}

static int
ble_hs_adv_parse_one_field(struct ble_hs_adv_fields *adv_fields,
                           uint8_t *total_len, const uint8_t *src,
                           uint8_t src_len)
{
    uint8_t data_len;
    uint8_t type;
    const uint8_t *data;
    int rc;

    if (src_len < 1) {
        return BLE_HS_EMSGSIZE;
    }
    *total_len = src[0] + 1;

    if (src_len < *total_len) {
        return BLE_HS_EMSGSIZE;
    }

    type = src[1];
    data = src + 2;
    data_len = *total_len - 2;

    if (data_len > BLE_HS_ADV_MAX_FIELD_SZ) {
        return BLE_HS_EBADDATA;
    }

    switch (type) {
    case BLE_HS_ADV_TYPE_FLAGS:
        if (data_len != BLE_HS_ADV_FLAGS_LEN) {
            return BLE_HS_EBADDATA;
        }
        adv_fields->flags = *data;
        break;

    case BLE_HS_ADV_TYPE_INCOMP_UUIDS16:
        rc = ble_hs_adv_parse_uuids16(adv_fields, data, data_len);
        if (rc != 0) {
            return rc;
        }
        adv_fields->uuids16_is_complete = 0;
        break;

    case BLE_HS_ADV_TYPE_COMP_UUIDS16:
        rc = ble_hs_adv_parse_uuids16(adv_fields, data, data_len);
        if (rc != 0) {
            return rc;
        }
        adv_fields->uuids16_is_complete = 1;
        break;

    case BLE_HS_ADV_TYPE_INCOMP_UUIDS32:
        rc = ble_hs_adv_parse_uuids32(adv_fields, data, data_len);
        if (rc != 0) {
            return rc;
        }
        adv_fields->uuids32_is_complete = 0;
        break;

    case BLE_HS_ADV_TYPE_COMP_UUIDS32:
        rc = ble_hs_adv_parse_uuids32(adv_fields, data, data_len);
        if (rc != 0) {
            return rc;
        }
        adv_fields->uuids32_is_complete = 1;
        break;

    case BLE_HS_ADV_TYPE_INCOMP_UUIDS128:
        rc = ble_hs_adv_parse_uuids128(adv_fields, data, data_len);
        if (rc != 0) {
            return rc;
        }
        adv_fields->uuids128_is_complete = 0;
        break;

    case BLE_HS_ADV_TYPE_COMP_UUIDS128:
        rc = ble_hs_adv_parse_uuids128(adv_fields, data, data_len);
        if (rc != 0) {
            return rc;
        }
        adv_fields->uuids128_is_complete = 1;
        break;

    case BLE_HS_ADV_TYPE_INCOMP_NAME:
        adv_fields->name = data;
        adv_fields->name_len = data_len;
        adv_fields->name_is_complete = 0;
        break;

    case BLE_HS_ADV_TYPE_COMP_NAME:
        adv_fields->name = data;
        adv_fields->name_len = data_len;
        adv_fields->name_is_complete = 1;
        break;

    case BLE_HS_ADV_TYPE_TX_PWR_LVL:
        if (data_len != BLE_HS_ADV_TX_PWR_LVL_LEN) {
            return BLE_HS_EBADDATA;
        }
        adv_fields->tx_pwr_lvl = *data;
        adv_fields->tx_pwr_lvl_is_present = 1;
        break;

    case BLE_HS_ADV_TYPE_SLAVE_ITVL_RANGE:
        if (data_len != BLE_HS_ADV_SLAVE_ITVL_RANGE_LEN) {
            return BLE_HS_EBADDATA;
        }
        adv_fields->slave_itvl_range = data;
        break;

    case BLE_HS_ADV_TYPE_SVC_DATA_UUID16:
        if (data_len < BLE_HS_ADV_SVC_DATA_UUID16_MIN_LEN) {
            return BLE_HS_EBADDATA;
        }
        adv_fields->svc_data_uuid16 = data;
        adv_fields->svc_data_uuid16_len = data_len;
        break;

    case BLE_HS_ADV_TYPE_PUBLIC_TGT_ADDR:
        if (data_len % BLE_HS_ADV_PUBLIC_TGT_ADDR_ENTRY_LEN != 0) {
            return BLE_HS_EBADDATA;
        }
        adv_fields->public_tgt_addr = data;
        adv_fields->num_public_tgt_addrs =
            data_len / BLE_HS_ADV_PUBLIC_TGT_ADDR_ENTRY_LEN;
        break;

    case BLE_HS_ADV_TYPE_APPEARANCE:
        if (data_len != BLE_HS_ADV_APPEARANCE_LEN) {
            return BLE_HS_EBADDATA;
        }
        adv_fields->appearance = get_le16(data);
        adv_fields->appearance_is_present = 1;
        break;

    case BLE_HS_ADV_TYPE_ADV_ITVL:
        if (data_len != BLE_HS_ADV_ADV_ITVL_LEN) {
            return BLE_HS_EBADDATA;
        }
        adv_fields->adv_itvl = get_le16(data);
        adv_fields->adv_itvl_is_present = 1;
        break;

    case BLE_HS_ADV_TYPE_SVC_DATA_UUID32:
        if (data_len < BLE_HS_ADV_SVC_DATA_UUID32_MIN_LEN) {
            return BLE_HS_EBADDATA;
        }
        adv_fields->svc_data_uuid32 = data;
        adv_fields->svc_data_uuid32_len = data_len;
        break;

    case BLE_HS_ADV_TYPE_SVC_DATA_UUID128:
        if (data_len < BLE_HS_ADV_SVC_DATA_UUID128_MIN_LEN) {
            return BLE_HS_EBADDATA;
        }
        adv_fields->svc_data_uuid128 = data;
        adv_fields->svc_data_uuid128_len = data_len;
        break;

    case BLE_HS_ADV_TYPE_URI:
        adv_fields->uri = data;
        adv_fields->uri_len = data_len;
        break;

    case BLE_HS_ADV_TYPE_MFG_DATA:
        adv_fields->mfg_data = data;
        adv_fields->mfg_data_len = data_len;
        break;

    default:
        break;
    }

    return 0;
}

int
ble_hs_adv_parse_fields(struct ble_hs_adv_fields *adv_fields,
                        const uint8_t *src, uint8_t src_len)
{
    uint8_t field_len;
    int rc;

    memset(adv_fields, 0, sizeof *adv_fields);

    while (src_len > 0) {
        rc = ble_hs_adv_parse_one_field(adv_fields, &field_len, src, src_len);
        if (rc != 0) {
            return rc;
        }

        src += field_len;
        src_len -= field_len;
    }

    return 0;
}

int
ble_hs_adv_parse(const uint8_t *data, uint8_t length,
                 ble_hs_adv_parse_func_t func, void *user_data)
{
    const struct ble_hs_adv_field *field;

    while (length > 1) {
        field = (const void *) data;

        if (field->length >= length) {
            return BLE_HS_EBADDATA;
        }

        if (func(field, user_data) == 0) {
            return 0;
        }

        length -= 1 + field->length;
        data += 1 + field->length;
    }

    return 0;
}

static int
find_field_func(const struct ble_hs_adv_field *field, void *user_data)
{
    struct find_field_data *ffd = user_data;

    if (field->type != ffd->type) {
        return BLE_HS_EAGAIN;
    }

    ffd->field = field;

    return 0;
}

int
ble_hs_adv_find_field(uint8_t type, const uint8_t *data, uint8_t length,
                      const struct ble_hs_adv_field **out)
{
    int rc;
    struct find_field_data ffd = {
            .type = type,
            .field = NULL,
    };

    rc = ble_hs_adv_parse(data, length, find_field_func, &ffd);
    if (rc != 0) {
        return rc;
    }

    if (!ffd.field) {
        return BLE_HS_ENOENT;
    }

    *out = ffd.field;

    return 0;
}
