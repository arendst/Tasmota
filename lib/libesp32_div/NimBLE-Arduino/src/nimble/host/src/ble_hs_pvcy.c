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
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include "stats/stats.h"
#include "ble_hs_priv.h"
#include "ble_hs_resolv_priv.h"
#include "host/ble_hs_pvcy.h"

static uint8_t ble_hs_pvcy_started;
static uint8_t ble_hs_pvcy_irk[16];

/** Use this as a default IRK if none gets set. */
const uint8_t ble_hs_pvcy_default_irk[16] = {
    0xef, 0x8d, 0xe2, 0x16, 0x4f, 0xec, 0x43, 0x0d,
    0xbf, 0x5b, 0xdd, 0x34, 0xc0, 0x53, 0x1e, 0xb8,
};

static int
ble_hs_pvcy_set_addr_timeout(uint16_t timeout)
{
    struct ble_hci_le_set_rpa_tmo_cp cmd;

#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
    return ble_hs_resolv_set_rpa_tmo(timeout);
#endif

    if (timeout == 0 || timeout > 0xA1B8) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    cmd.rpa_timeout = htole16(timeout);

    return ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_LE,
                                        BLE_HCI_OCF_LE_SET_RPA_TMO),
                             &cmd, sizeof(cmd), NULL, 0);
}

#if (!MYNEWT_VAL(BLE_HOST_BASED_PRIVACY))
static int
ble_hs_pvcy_set_resolve_enabled(int enable)
{
    struct ble_hci_le_set_addr_res_en_cp cmd;

    cmd.enable = enable;

    return ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_LE,
                                        BLE_HCI_OCF_LE_SET_ADDR_RES_EN),
                             &cmd, sizeof(cmd), NULL, 0);
}
#endif

int
ble_hs_pvcy_remove_entry(uint8_t addr_type, const uint8_t *addr)
{
    struct ble_hci_le_rmv_resolve_list_cp cmd;
    int rc;

    if (addr_type > BLE_ADDR_RANDOM) {
        addr_type = addr_type % 2;
    }

    cmd.peer_addr_type = addr_type;
    memcpy(cmd.peer_id_addr, addr, BLE_DEV_ADDR_LEN);
#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
    rc = ble_hs_resolv_list_rmv(addr_type, &cmd.peer_id_addr[0]);
#else
    rc = ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_LE,
                                      BLE_HCI_OCF_LE_RMV_RESOLV_LIST),
                           &cmd, sizeof(cmd), NULL, 0);
#endif
    return rc;
}

#if (!MYNEWT_VAL(BLE_HOST_BASED_PRIVACY))
static int
ble_hs_pvcy_clear_entries(void)
{
    return ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_LE,
                                        BLE_HCI_OCF_LE_CLR_RESOLV_LIST),
                             NULL, 0, NULL, 0);
}
#endif

static int
ble_hs_pvcy_add_entry_hci(const uint8_t *addr, uint8_t addr_type,
                          const uint8_t *irk)
{
    struct ble_hci_le_add_resolv_list_cp cmd;
    int rc;

    if (addr_type > BLE_ADDR_RANDOM) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    cmd.peer_addr_type = addr_type;
    memcpy(cmd.peer_id_addr, addr, 6);
    memcpy(cmd.local_irk, ble_hs_pvcy_irk, 16);
    memcpy(cmd.peer_irk, irk, 16);

#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
    rc = ble_hs_resolv_list_add((uint8_t *) &cmd);
    if (rc != 0) {
        return rc;
    }

#else
    ble_addr_t peer_addr;

    rc = ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_LE,
                                      BLE_HCI_OCF_LE_ADD_RESOLV_LIST),
                           &cmd, sizeof(cmd), NULL, 0);
    if (rc != 0) {
        return rc;
    }

    /* FIXME Controller is BT5.0 and default privacy mode is network which
     * can cause problems for apps which are not aware of it. We need to
     * sort it out somehow. For now we set device mode for all of the peer
     * devices and application should change it to network if needed
     */
    peer_addr.type = addr_type;
    memcpy(peer_addr.val, addr, sizeof peer_addr.val);
    rc = ble_hs_pvcy_set_mode(&peer_addr, BLE_GAP_PRIVATE_MODE_DEVICE);
    if (rc != 0) {
        return rc;
    }
#endif

    return 0;
}

int
ble_hs_pvcy_add_entry(const uint8_t *addr, uint8_t addr_type,
                      const uint8_t *irk)
{
    int rc;

    STATS_INC(ble_hs_stats, pvcy_add_entry);

    /* No GAP procedures can be active when adding an entry to the resolving
     * list (Vol 2, Part E, 7.8.38).  Stop all GAP procedures and temporarily
     * prevent any new ones from being started.
     */
#if (MYNEWT_VAL(BLE_HOST_BASED_PRIVACY))
    rc = ble_hs_pvcy_add_entry_hci(addr, addr_type, irk);
#else
    ble_gap_preempt();

    /* Try to add the entry now that GAP is halted. */
    rc = ble_hs_pvcy_add_entry_hci(addr, addr_type, irk);

    /* Allow GAP procedures to be started again. */
    ble_gap_preempt_done();

#endif
    if (rc != 0) {
        STATS_INC(ble_hs_stats, pvcy_add_entry_fail);
    }

    return rc;
}

int
ble_hs_pvcy_ensure_started(void)
{
    int rc;

    if (ble_hs_pvcy_started) {
        return 0;
    }

#if (MYNEWT_VAL(BLE_HOST_BASED_PRIVACY))
    /*This is to be called only once*/
    ble_hs_resolv_init();
#endif

    /* Set up the periodic change of our RPA. */
    rc = ble_hs_pvcy_set_addr_timeout(MYNEWT_VAL(BLE_RPA_TIMEOUT));
    if (rc != 0) {
        return rc;
    }

    ble_hs_pvcy_started = 1;

    return 0;
}

int
ble_hs_pvcy_set_our_irk(const uint8_t *irk)
{
    uint8_t tmp_addr[6];
    uint8_t new_irk[16];
    int rc;

    if (irk != NULL) {
        memcpy(new_irk, irk, 16);
    } else {
        memcpy(new_irk, ble_hs_pvcy_default_irk, 16);
    }

    /* Clear the resolving list if this is a new IRK. */
    if (memcmp(ble_hs_pvcy_irk, new_irk, 16) != 0) {
        memcpy(ble_hs_pvcy_irk, new_irk, 16);

#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
        if (irk != NULL) {
            bool rpa_state = false;

            if ((rpa_state = ble_host_rpa_enabled()) == true) {
                ble_hs_resolv_enable(0);
            }

            ble_hs_resolv_list_clear_all();

            if (rpa_state) {
                ble_hs_resolv_enable(1);
            }
        }
#else
        rc = ble_hs_pvcy_set_resolve_enabled(0);
        if (rc != 0) {
            return rc;
        }

        rc = ble_hs_pvcy_clear_entries();
        if (rc != 0) {
            return rc;
        }

        rc = ble_hs_pvcy_set_resolve_enabled(1);
        if (rc != 0) {
            return rc;
        }

#endif
        /*
         * Add local IRK entry with 00:00:00:00:00:00 address. This entry will
         * be used to generate RPA for non-directed advertising if own_addr_type
         * is set to rpa_pub since we use all-zero address as peer addres in
         * such case. Peer IRK should be left all-zero since this is not for an
         * actual peer.
         */
        memset(tmp_addr, 0, 6);
        memset(new_irk, 0, 16);
        rc = ble_hs_pvcy_add_entry(tmp_addr, 0, new_irk);
        if (rc != 0) {
            return rc;
        }
    }

    return 0;
}

int
ble_hs_pvcy_our_irk(const uint8_t **out_irk)
{
    /* XXX: Return error if privacy not supported. */

    *out_irk = ble_hs_pvcy_irk;
    return 0;
}

int
ble_hs_pvcy_set_mode(const ble_addr_t *addr, uint8_t priv_mode)
{
    struct ble_hci_le_set_privacy_mode_cp cmd;

    if (addr->type > BLE_ADDR_RANDOM) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    cmd.mode = priv_mode;
    cmd.peer_id_addr_type = addr->type;
    memcpy(cmd.peer_id_addr, addr->val, BLE_DEV_ADDR_LEN);

    return ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_LE,
                                        BLE_HCI_OCF_LE_SET_PRIVACY_MODE),
                             &cmd, sizeof(cmd), NULL, 0);
}

bool
ble_hs_pvcy_enabled(void)
{
    return ble_hs_pvcy_started;
}

#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
int
ble_hs_pvcy_rpa_config(uint8_t enable)
{
    int rc = 0;

    if (enable != NIMBLE_HOST_DISABLE_PRIVACY) {
        rc = ble_hs_pvcy_ensure_started();
        if (rc != 0) {
            return rc;
        }

        ble_hs_resolv_enable(true);

        /* Configure NRPA address related flags according to input parameter */
        if (enable == NIMBLE_HOST_ENABLE_NRPA) {
            ble_hs_resolv_nrpa_enable();
        } else {
            ble_hs_resolv_nrpa_disable();
        }

        /* Generate local RPA address and set it in controller */
        rc = ble_hs_gen_own_private_rnd();
    } else {
        ble_hs_resolv_enable(false);
    }

    return rc;
}
#endif
