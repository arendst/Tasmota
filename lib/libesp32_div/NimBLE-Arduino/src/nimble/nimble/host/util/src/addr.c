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
#include "../include/host/util/util.h"

#if MYNEWT_VAL(BLE_CONTROLLER)
#include "nimble/nimble/controller/include/controller/ble_hw.h"
#endif

static int
ble_hs_util_load_rand_addr(ble_addr_t *addr)
{
    /* XXX: It is unfortunate that the function to retrieve the random address
     * is in the controller package.  A host-only device ought to be able to
     * automically restore a random address.
     */
#if MYNEWT_VAL(BLE_CONTROLLER)
    int rc;

    rc = ble_hw_get_static_addr(addr);
    if (rc == 0) {
        return 0;
    }
#endif

    return BLE_HS_ENOADDR;
}

static int
ble_hs_util_ensure_rand_addr(void)
{
    ble_addr_t addr;
    int rc;

    /* If we already have a random address, then we are done. */
    rc = ble_hs_id_copy_addr(BLE_ADDR_RANDOM, NULL, NULL);
    if (rc == 0) {
        return 0;
    }

    /* Otherwise, try to load a random address. */
    rc = ble_hs_util_load_rand_addr(&addr);
    if (rc != 0) {
        return rc;
    }

    /* Configure nimble to use the random address. */
    rc = ble_hs_id_set_rnd(addr.val);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

int
ble_hs_util_ensure_addr(int prefer_random)
{
    int rc;

    if (prefer_random) {
        /* Try to load a random address. */
        rc = ble_hs_util_ensure_rand_addr();
        if (rc == BLE_HS_ENOADDR) {
            /* No random address; try to load a public address. */
            rc = ble_hs_id_copy_addr(BLE_ADDR_PUBLIC, NULL, NULL);
        }
    } else {
        /* Try to load a public address. */
        rc = ble_hs_id_copy_addr(BLE_ADDR_PUBLIC, NULL, NULL);
        if (rc == BLE_HS_ENOADDR) {
            /* No public address; try to load a random address. */
            rc = ble_hs_util_ensure_rand_addr();
        }
    }

    return rc;
}
