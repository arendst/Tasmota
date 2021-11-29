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
#include "nimble/nimble/host/include/host/ble_hs_id.h"
#include "ble_hs_priv.h"
#include "ble_hs_resolv_priv.h"

static uint8_t ble_hs_id_pub[6];
static uint8_t ble_hs_id_rnd[6];

bool
ble_hs_is_rpa(uint8_t *addr, uint8_t addr_type)
{
    bool rc = 0;
    /* According to spec v4.2, Vol 6, Part B, section 1.3.2.2, the two most
     * significant bits of RPA shall be equal to 0 and 1 */
    if (addr_type && ((addr[5] & 0xc0) == 0x40)) {
        rc = 1;
    }
    return rc;
}

void
ble_hs_id_set_pub(const uint8_t *pub_addr)
{
    ble_hs_lock();
    memcpy(ble_hs_id_pub, pub_addr, 6);
    ble_hs_unlock();
}

int
ble_hs_id_gen_rnd(int nrpa, ble_addr_t *out_addr)
{
    int rc;

    out_addr->type = BLE_ADDR_RANDOM;

    rc = ble_hs_hci_util_rand(out_addr->val, 6);
    if (rc != 0) {
        return rc;
    }

    if (nrpa) {
        out_addr->val[5] &= ~0xc0;
    } else {
        out_addr->val[5] |= 0xc0;
    }

    return 0;
}

#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
/**
 * Sets the device's pseudo Non Resolvable Private Address when 'Host based
 * privacy' is in use.
 *
 * @return                      0 on success;
 *                              Appropriate error code if failure.
 */
int
ble_hs_id_set_nrpa_rnd()
{

    ble_addr_t nrpa_addr;
    int rc;

    ble_hs_id_gen_rnd(1, &nrpa_addr);

    ble_hs_lock();

    /* set the NRPA address as pseudo random address in controller */
    rc = ble_hs_hci_util_set_random_addr(nrpa_addr.val);
    if (rc != 0) {
        goto done;
    }

    memcpy(ble_hs_id_rnd, nrpa_addr.val, BLE_DEV_ADDR_LEN);

done:
    ble_hs_unlock();
    return rc;
}

/**
 * Sets the device's pseudo RPA address when 'Host based privacy' is in use.
 * The address type (RPA) is inferred from the most-significant bits. The
 * address is specified in host byte order (little-endian!).
 *
 * @param rnd_addr              The RPA address to set.
 *
 * @return                      0 on success;
 *                              BLE_HS_EINVAL if the specified address is not a
 *                                  resolvable private address.
 *                              Other nonzero on error.
 */
int
ble_hs_id_set_pseudo_rnd(const uint8_t *rnd_addr)
{
    uint8_t addr_type_byte;
    int rc;
    int i, rnd_part_sum = 0;

    ble_hs_lock();

    /* Make sure all bits of rnd_addr are neither one nor zero (3rd, 4th and
     * 5th bytes of rnd_addr(RPA) are prand) Vol 6, Part B, section 1.3.2.2
     * The two most significant bits of RPA shall be equal to 0 and 1 */
    addr_type_byte = rnd_addr[5] & 0xc0;
    for (i = 3; i < BLE_DEV_ADDR_LEN; i++) {
        rnd_part_sum += *(rnd_addr + i);
    }
    rnd_part_sum -= addr_type_byte;

    /* All ones in random part: 3*(0xFF) - 0x40 = 0x2BD  */
    if ((addr_type_byte != 0x40) ||
        (rnd_part_sum == 0) || (rnd_part_sum == 0x2BD)) {
        rc = BLE_HS_EINVAL;
        goto done;
    }
    /* set the RPA address as pseudo random address in controller */
    rc = ble_hs_hci_util_set_random_addr(rnd_addr);
    if (rc != 0) {
        goto done;
    }

    memcpy(ble_hs_id_rnd, rnd_addr, BLE_DEV_ADDR_LEN);

done:
    ble_hs_unlock();
    return rc;
}
#endif

int
ble_hs_id_set_rnd(const uint8_t *rnd_addr)
{
    uint8_t addr_type_byte;
    int rc;
    int ones;

    ble_hs_lock();

    /* Make sure random part of rnd_addr is not all ones or zeros. Reference:
     * Core v5.0, Vol 6, Part B, section 1.3.2.1 */
    addr_type_byte = rnd_addr[5] & 0xc0;

    /* count bits set to 1 in random part of address */
    ones = __builtin_popcount(rnd_addr[0]);
    ones += __builtin_popcount(rnd_addr[1]);
    ones += __builtin_popcount(rnd_addr[2]);
    ones += __builtin_popcount(rnd_addr[3]);
    ones += __builtin_popcount(rnd_addr[4]);
    ones += __builtin_popcount(rnd_addr[5] & 0x3f);

    if ((addr_type_byte != 0x00 && addr_type_byte != 0xc0) ||
            (ones == 0 || ones == 46)) {
        rc = BLE_HS_EINVAL;
        goto done;
    }

    rc = ble_hs_hci_util_set_random_addr(rnd_addr);
    if (rc != 0) {
        goto done;
    }

    memcpy(ble_hs_id_rnd, rnd_addr, 6);

done:
    ble_hs_unlock();
    return rc;
}

/**
 * Retrieves one of the device's identity addresses.  The device can have two
 * identity addresses: one public and one random.  The id_addr_type argument
 * specifies which of these two addresses to retrieve.
 *
 * @param id_addr_type          The type of identity address to retrieve.
 *                                  Valid values are:
 *                                      o BLE_ADDR_PUBLIC
 *                                      o BLE_ADDR_RANDOM
 * @param out_id_addr           On success, this is reseated to point to the
 *                                  retrieved 6-byte identity address.  Pass
 *                                  NULL if you do not require this
 *                                  information.

 * @param out_is_nrpa           On success, the pointed-to value indicates
 *                                  whether the retrieved address is a
 *                                  non-resolvable private address.  Pass NULL
 *                                  if you do not require this information.
 *
 * @return                      0 on success;
 *                              BLE_HS_EINVAL if an invalid address type was
 *                                  specified;
 *                              BLE_HS_ENOADDR if the device does not have an
 *                                  identity address of the requested type;
 *                              Other BLE host core code on error.
 */
int
ble_hs_id_addr(uint8_t id_addr_type, const uint8_t **out_id_addr,
               int *out_is_nrpa)
{
    const uint8_t *id_addr;
    int nrpa;

    BLE_HS_DBG_ASSERT(ble_hs_locked_by_cur_task());

    switch (id_addr_type) {
    case BLE_ADDR_PUBLIC:
        id_addr = ble_hs_id_pub;
        nrpa = 0;
        break;

    case BLE_ADDR_RANDOM:
        id_addr = ble_hs_id_rnd;
        nrpa = (ble_hs_id_rnd[5] & 0xc0) == 0;
        break;

    default:
        return BLE_HS_EINVAL;
    }

    if (memcmp(id_addr, ble_hs_misc_null_addr, 6) == 0) {
        return BLE_HS_ENOADDR;
    }

    if (out_id_addr != NULL) {
        *out_id_addr = id_addr;
    }
    if (out_is_nrpa != NULL) {
        *out_is_nrpa = nrpa;
    }

    return 0;
}

int
ble_hs_id_copy_addr(uint8_t id_addr_type, uint8_t *out_id_addr,
                    int *out_is_nrpa)
{
    const uint8_t *addr;
    int rc;

    ble_hs_lock();

    rc = ble_hs_id_addr(id_addr_type, &addr, out_is_nrpa);
    if (rc == 0 && out_id_addr != NULL) {
        memcpy(out_id_addr, addr, 6);
    }

    ble_hs_unlock();

    return rc;
}

static int
ble_hs_id_addr_type_usable(uint8_t own_addr_type)
{
    uint8_t id_addr_type;
    int nrpa;
    int rc;

    switch (own_addr_type) {
    case BLE_OWN_ADDR_PUBLIC:
    case BLE_OWN_ADDR_RANDOM:
        rc = ble_hs_id_addr(own_addr_type, NULL, NULL);
        if (rc != 0) {
            return rc;
        }
        break;

    case BLE_OWN_ADDR_RPA_PUBLIC_DEFAULT:
    case BLE_OWN_ADDR_RPA_RANDOM_DEFAULT:
        id_addr_type = ble_hs_misc_own_addr_type_to_id(own_addr_type);
        rc = ble_hs_id_addr(id_addr_type, NULL, &nrpa);
        if (rc != 0) {
            return rc;
        }
        if (nrpa) {
            return BLE_HS_ENOADDR;
        }
        break;

    default:
        return BLE_HS_EINVAL;
    }

    return 0;
}

int
ble_hs_id_use_addr(uint8_t own_addr_type)
{
    int rc;

    rc = ble_hs_id_addr_type_usable(own_addr_type);
    if (rc != 0) {
        return rc;
    }

    /* If privacy is being used, make sure RPA rotation is in effect. */
    if (own_addr_type == BLE_OWN_ADDR_RPA_PUBLIC_DEFAULT ||
        own_addr_type == BLE_OWN_ADDR_RPA_RANDOM_DEFAULT) {

        rc = ble_hs_pvcy_ensure_started();
        if (rc != 0) {
            return rc;
        }
    }

    return 0;
}

int
ble_hs_id_infer_auto(int privacy, uint8_t *out_addr_type)
{
    static const uint8_t pub_addr_types[] = {
        BLE_OWN_ADDR_RANDOM,
        BLE_OWN_ADDR_PUBLIC,
    };
    static const uint8_t priv_addr_types[] = {
        BLE_OWN_ADDR_RPA_RANDOM_DEFAULT,
        BLE_OWN_ADDR_RPA_PUBLIC_DEFAULT,
    };
    const uint8_t *addr_types;
    uint8_t addr_type;
    int num_addr_types;
    int rc;
    int i;

    ble_hs_lock();

    if (privacy) {
        addr_types = priv_addr_types;
        num_addr_types = sizeof priv_addr_types / sizeof priv_addr_types[0];
    } else {
        addr_types = pub_addr_types;
        num_addr_types = sizeof pub_addr_types / sizeof pub_addr_types[0];
    }

    for (i = 0; i < num_addr_types; i++) {
        addr_type = addr_types[i];

        rc = ble_hs_id_addr_type_usable(addr_type);
        switch (rc) {
        case 0:
            *out_addr_type = addr_type;
            goto done;

        case BLE_HS_ENOADDR:
            break;

        default:
            goto done;
        }
    }

    rc = BLE_HS_ENOADDR;

done:
    ble_hs_unlock();
    return rc;
}

/**
 * Clears both the public and random addresses.  This function is necessary
 * when the controller loses its random address (e.g., on a stack reset).
 */
void
ble_hs_id_reset(void)
{
    memset(ble_hs_id_pub, 0, sizeof ble_hs_id_pub);
    memset(ble_hs_id_rnd, 0, sizeof ble_hs_id_pub);
}

/**
 * Clears random address. This function is necessary when the host wants to
 * clear random address.
 */
 void
 ble_hs_id_rnd_reset(void)
 {
    memset(ble_hs_id_rnd, 0, sizeof ble_hs_id_rnd);
 }
