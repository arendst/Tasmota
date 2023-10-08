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
#ifndef ESP_PLATFORM

#include <stdint.h>
#include <assert.h>
#include <string.h>
#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#include "nimble/porting/nimble/include/os/os.h"
#include "nimble/nimble/include/nimble/ble.h"
#include "nimble/nimble/include/nimble/nimble_opt.h"

#if defined(ARDUINO_ARCH_NRF5) && defined(NRF51)
#include "nimble/nimble/drivers/nrf51/include/ble/xcvr.h"
#elif defined(ARDUINO_ARCH_NRF5) && defined(NRF52_SERIES)
#include "nimble/nimble/drivers/nrf52/include/ble/xcvr.h"
#endif

#include "../include/controller/ble_ll_whitelist.h"
#include "../include/controller/ble_ll_hci.h"
#include "../include/controller/ble_ll_adv.h"
#include "../include/controller/ble_ll_scan.h"
#include "../include/controller/ble_hw.h"

#if (MYNEWT_VAL(BLE_LL_WHITELIST_SIZE) < BLE_HW_WHITE_LIST_SIZE)
#define BLE_LL_WHITELIST_SIZE       MYNEWT_VAL(BLE_LL_WHITELIST_SIZE)
#else
#define BLE_LL_WHITELIST_SIZE       BLE_HW_WHITE_LIST_SIZE
#endif

struct ble_ll_whitelist_entry
{
    uint8_t wl_valid;
    uint8_t wl_addr_type;
    uint8_t wl_dev_addr[BLE_DEV_ADDR_LEN];
};

struct ble_ll_whitelist_entry g_ble_ll_whitelist[BLE_LL_WHITELIST_SIZE];

static int
ble_ll_whitelist_chg_allowed(void)
{
    int rc;

    /*
     * This command is not allowed if:
     *  -> advertising uses the whitelist and we are currently advertising.
     *  -> scanning uses the whitelist and is enabled.
     *  -> initiating uses whitelist and a LE create connection command is in
     *     progress
     */
    rc = 1;
    if (!ble_ll_adv_can_chg_whitelist() || !ble_ll_scan_can_chg_whitelist()) {
        rc = 0;
    }
    return rc;
}

/**
 * Clear the whitelist.
 *
 * @return int 0: success, BLE error code otherwise
 */
int
ble_ll_whitelist_clear(void)
{
    int i;
    struct ble_ll_whitelist_entry *wl;

    /* Check proper state */
    if (!ble_ll_whitelist_chg_allowed()) {
        return BLE_ERR_CMD_DISALLOWED;
    }

    /* Set the number of entries to 0 */
    wl = &g_ble_ll_whitelist[0];
    for (i = 0; i < BLE_LL_WHITELIST_SIZE; ++i) {
        wl->wl_valid = 0;
        ++wl;
    }

#if (BLE_USES_HW_WHITELIST == 1)
    ble_hw_whitelist_clear();
#endif

    return BLE_ERR_SUCCESS;
}

/**
 * Read the size of the whitelist. This is the total number of whitelist
 * entries allowed by the controller.
 *
 * @param rspbuf Pointer to response buffer
 *
 * @return int 0: success.
 */
int
ble_ll_whitelist_read_size(uint8_t *rspbuf, uint8_t *rsplen)
{
    struct ble_hci_le_rd_white_list_rp *rsp = (void *) rspbuf;

    rsp->size = BLE_LL_WHITELIST_SIZE;

    *rsplen = sizeof(*rsp);

    return BLE_ERR_SUCCESS;
}

/**
 * Searches the whitelist to determine if the address is present in the
 * whitelist. This is an internal API that only searches the link layer
 * whitelist and does not care about the hardware whitelist
 *
 * @param addr      Device or identity address to check.
 * @param addr_type Public address (0) or random address (1)
 *
 * @return int 0: device is not on whitelist; otherwise the return value
 * is the 'position' of the device in the whitelist (the index of the element
 * plus 1).
 */
static int
ble_ll_whitelist_search(const uint8_t *addr, uint8_t addr_type)
{
    int i;
    struct ble_ll_whitelist_entry *wl;

    wl = &g_ble_ll_whitelist[0];
    for (i = 0; i < BLE_LL_WHITELIST_SIZE; ++i) {
        if ((wl->wl_valid) && (wl->wl_addr_type == addr_type) &&
            (!memcmp(&wl->wl_dev_addr[0], addr, BLE_DEV_ADDR_LEN))) {
            return i + 1;
        }
        ++wl;
    }

    return 0;
}

/**
 * Is there a match between the device and a device on the whitelist.
 *
 * NOTE: This API uses the HW, if present, to determine if there was a match
 * between a received address and an address in the whitelist. If the HW does
 * not support whitelisting this API is the same as the whitelist search API
 *
 * @param addr
 * @param addr_type Public address (0) or random address (1)
 * @param is_ident  True if addr is an identity address; false otherwise
 *
 * @return int
 */
int
ble_ll_whitelist_match(uint8_t *addr, uint8_t addr_type, int is_ident)
{
    int rc;
#if (BLE_USES_HW_WHITELIST == 1)
    /*
     * XXX: This should be changed. This is HW specific: some HW may be able
     * to both resolve a private address and perform a whitelist check. The
     * current BLE hw cannot support this.
     */
    if (is_ident) {
        rc = ble_ll_whitelist_search(addr, addr_type);
    } else {
        rc = ble_hw_whitelist_match();
    }
#else
    rc = ble_ll_whitelist_search(addr, addr_type);
#endif
    return rc;
}

/**
 * Add a device to the whitelist
 *
 * @return int
 */
int
ble_ll_whitelist_add(const uint8_t *cmdbuf, uint8_t len)
{
    const struct ble_hci_le_add_whte_list_cp *cmd = (const void *) cmdbuf;
    struct ble_ll_whitelist_entry *wl;
    int rc;
    int i;

    if (len != sizeof(*cmd)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    /* Must be in proper state */
    if (!ble_ll_whitelist_chg_allowed()) {
        return BLE_ERR_CMD_DISALLOWED;
    }

    /* Check if we have any open entries */
    rc = BLE_ERR_SUCCESS;
    if (!ble_ll_whitelist_search(cmd->addr, cmd->addr_type)) {
        wl = &g_ble_ll_whitelist[0];
        for (i = 0; i < BLE_LL_WHITELIST_SIZE; ++i) {
            if (wl->wl_valid == 0) {
                memcpy(&wl->wl_dev_addr[0], cmd->addr, BLE_DEV_ADDR_LEN);
                wl->wl_addr_type = cmd->addr_type;
                wl->wl_valid = 1;
                break;
            }
            ++wl;
        }

        if (i == BLE_LL_WHITELIST_SIZE) {
            rc = BLE_ERR_MEM_CAPACITY;
        } else {
#if (BLE_USES_HW_WHITELIST == 1)
            rc = ble_hw_whitelist_add(cmd->addr, cmd->addr_type);
#endif
        }
    }

    return rc;
}

/**
 * Remove a device from the whitelist
 *
 * @param cmdbuf
 *
 * @return int 0: success, BLE error code otherwise
 */
int
ble_ll_whitelist_rmv(const uint8_t *cmdbuf, uint8_t len)
{
    const struct ble_hci_le_rmv_white_list_cp *cmd = (const void *) cmdbuf;
    int position;

    if (len != sizeof(*cmd)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    /* Must be in proper state */
    if (!ble_ll_whitelist_chg_allowed()) {
        return BLE_ERR_CMD_DISALLOWED;
    }

    position = ble_ll_whitelist_search(cmd->addr, cmd->addr_type);
    if (position) {
        g_ble_ll_whitelist[position - 1].wl_valid = 0;
    }

#if (BLE_USES_HW_WHITELIST == 1)
    ble_hw_whitelist_rmv(cmd->addr, cmd->addr_type);
#endif

    return BLE_ERR_SUCCESS;
}

/**
 * Enable whitelisting.
 *
 * Note: This function has no effect if we are not using HW whitelisting
 */
void
ble_ll_whitelist_enable(void)
{
#if (BLE_USES_HW_WHITELIST == 1)
    ble_hw_whitelist_enable();
#endif
}

/**
 * Disable whitelisting.
 *
 * Note: This function has no effect if we are not using HW whitelisting
 */
void
ble_ll_whitelist_disable(void)
{
#if (BLE_USES_HW_WHITELIST == 1)
    ble_hw_whitelist_disable();
#endif
}

#endif
