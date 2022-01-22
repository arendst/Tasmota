/*
 * Copyright 2020 Espressif Systems (Shanghai) PTE LTD
 *
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

#include "syscfg/syscfg.h"
#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)

#include <stdint.h>
#include <string.h>
#include "ble_hs_priv.h"
#include "host/ble_hs_id.h"
#include "nimble/ble.h"
#include "nimble/nimble_opt.h"
#include "ble_hs_resolv_priv.h"
#include "store/config/ble_store_config.h"
#include "../store/config/src/ble_store_config_priv.h"

/* Resolve list size, additional space to save local device's configuration */
#define BLE_RESOLV_LIST_SIZE    (MYNEWT_VAL(BLE_STORE_MAX_BONDS) + 1)
#define BLE_MAX_RPA_TIMEOUT_VAL 0xA1B8

static struct ble_hs_resolv_data g_ble_hs_resolv_data;
static struct ble_hs_resolv_entry g_ble_hs_resolv_list[BLE_RESOLV_LIST_SIZE];
/* Allocate one extra space for peer_records than no. of Bonds, it will take
 * care of storage overflow  */
static struct ble_hs_dev_records peer_dev_rec[BLE_RESOLV_LIST_SIZE];
static int ble_store_num_peer_dev_rec;

struct ble_hs_resolv_data {
    uint8_t addr_res_enabled;
    uint8_t rl_cnt;
    uint32_t rpa_tmo;
    struct ble_npl_callout rpa_timer;
};

/* NRPA bit: Enables NRPA as private address. */
static bool nrpa_pvcy;

/*** APIs for Peer Device Records.
 *
 * These Peer records are necessary to take care of Peers with RPA address when
 * they are not yet added to Resolving list ***/

struct ble_hs_dev_records *
ble_rpa_get_peer_dev_records(void)
{
    return &peer_dev_rec[0];
}

int
ble_rpa_get_num_peer_dev_records(void)
{
    return ble_store_num_peer_dev_rec;
}

void
ble_rpa_set_num_peer_dev_records(int num_rec)
{
    ble_store_num_peer_dev_rec = num_rec;
}

int
ble_rpa_remove_peer_dev_rec(struct ble_hs_dev_records *p_dev_rec)
{
    int i;

    for (i = 0; i < ble_store_num_peer_dev_rec; i++) {
        if (!(memcmp(p_dev_rec, &peer_dev_rec[i], sizeof(struct
                     ble_hs_dev_records)))) {
            memset(&peer_dev_rec[i], 0, sizeof(struct ble_hs_dev_records));
            break;
        }
    }

    if (i >= ble_store_num_peer_dev_rec) {
        return BLE_HS_EUNKNOWN;
    }

    ble_store_num_peer_dev_rec--;
    if ((i != ble_store_num_peer_dev_rec) && (ble_store_num_peer_dev_rec != 0)) {
        memmove(&peer_dev_rec[i], &peer_dev_rec[i + 1],
                (ble_store_num_peer_dev_rec - i) * sizeof(struct ble_hs_dev_records ));
    }

    BLE_HS_LOG(DEBUG, " RPA: removed device at index = %d, no. of peer records"
               " = %d\n", i, ble_store_num_peer_dev_rec);

    ble_store_persist_peer_records();
    return 0;
}

static void
ble_rpa_peer_dev_rec_clear_all(void)
{
    uint8_t i;

    /* As NVS record need to be deleted one by one, we loop through
     * peer_records */
    for (i = 0; i < ble_store_num_peer_dev_rec; i++) {
        ble_store_num_peer_dev_rec--;

        if ((i != ble_store_num_peer_dev_rec) && (ble_store_num_peer_dev_rec != 0)) {
            memmove(&peer_dev_rec[i], &peer_dev_rec[i + 1],
                    (ble_store_num_peer_dev_rec - i + 1) * sizeof(struct ble_hs_dev_records ));
        }

        ble_store_persist_peer_records();
    }
    return;
}

/* Find peer device record with the address value.
 *
 * @return peer_dev_record if the addr matches with either of rand_addr,
 *                         pseudo_addr or identity_addr
 *         NULL            otherwise
 */
struct ble_hs_dev_records *
ble_rpa_find_peer_dev_rec(uint8_t *addr)
{
    struct ble_hs_dev_records *p_dev_rec = &peer_dev_rec[0];
    uint8_t i;

    for (i = 0; i < ble_store_num_peer_dev_rec; i++, p_dev_rec++) {
        if (p_dev_rec->rec_used) {
            if (!(memcmp(p_dev_rec->rand_addr, addr, BLE_DEV_ADDR_LEN)) ||
                    !(memcmp(p_dev_rec->pseudo_addr, addr, BLE_DEV_ADDR_LEN)) ||
                    !(memcmp(p_dev_rec->identity_addr, addr, BLE_DEV_ADDR_LEN))) {
                return p_dev_rec;
            }
        }
    }
    return NULL;
}

/* Find peer device record with the IRK */
static struct ble_hs_dev_records *
ble_rpa_find_peer_dev_by_irk(uint8_t *irk)
{
    struct ble_hs_dev_records *p_dev_rec = &peer_dev_rec[0];
    uint8_t i;

    for (i = 0; i < ble_store_num_peer_dev_rec; i++, p_dev_rec++) {
        if ((p_dev_rec->rec_used) &&
                (!memcmp(irk, p_dev_rec->peer_sec.irk, 16))) {
            return p_dev_rec;
        }
    }
    return NULL;
}

/* Find out if the peer record at perticular index resolves received peer
 * address.
 *
 * @return true if the RPA is resolvable, false otherwise */
static bool
is_rpa_resolvable_by_peer_rec(struct ble_hs_dev_records *p_dev_rec, uint8_t *peer_add)
{
    if (p_dev_rec->peer_sec.irk_present) {
        if (ble_hs_resolv_rpa(peer_add, p_dev_rec->peer_sec.irk) == 0) {
            return true;
        }
    }
    return false;
}

static void
ble_rpa_replace_id_with_rand_addr(uint8_t *addr_type, uint8_t *peer_addr)
{
    struct ble_hs_dev_records *p_dev_rec;
    ble_addr_t p_addr = {0};
    struct ble_hs_conn *conn = NULL;

    p_dev_rec = ble_rpa_find_peer_dev_rec(peer_addr);

    if (p_dev_rec != NULL) {
        if (memcmp(p_dev_rec->rand_addr, p_dev_rec->identity_addr, BLE_DEV_ADDR_LEN)) {
            /* OTA address (before resolving) gets saved in RAND_ADDR when the peer
             * record is fetched from resolving list. Replace peer address
             * with rand_addr to maintain status quo for new pairing/encryption request. */
            p_addr.type = *addr_type;
            memcpy(&p_addr.val[0], peer_addr, BLE_DEV_ADDR_LEN);

            ble_hs_lock();

            conn = ble_hs_conn_find_by_addr(&p_addr);
            /* Rewrite the peer address history in ble_hs_conn. Need to take
             * this step to avoid taking wrong address during re-pairing
             * process  */
            if (conn != NULL) {
                conn->bhc_peer_rpa_addr.type = p_dev_rec->rand_addr_type;
                memcpy(&conn->bhc_peer_rpa_addr.val[0], p_dev_rec->rand_addr, BLE_DEV_ADDR_LEN);
                conn->bhc_peer_addr.type = p_dev_rec->rand_addr_type;
                memcpy(&conn->bhc_peer_addr.val[0], p_dev_rec->rand_addr, BLE_DEV_ADDR_LEN);
                BLE_HS_LOG(DEBUG, "\n Replace Identity addr with random addr received at"
                                  " start of the connection\n");
            }

            ble_hs_unlock();
        }
    }
    return;
}

/* Add peer to peer device records.
 *
 * @return 0                    if added successfully,
 *         BLE_HS_ESTORE_CAP    if the no. of peer device records are exceeding
 *                              maximum allowed value (No. of Bonds + 1)  */
int
ble_rpa_resolv_add_peer_rec(uint8_t *peer_addr)
{
    struct ble_hs_dev_records *p_dev_rec =
            &peer_dev_rec[ble_store_num_peer_dev_rec];
    uint8_t idx = 0;

    while (p_dev_rec->rec_used) {
        p_dev_rec++;
        idx++;
        if (idx > MYNEWT_VAL(BLE_STORE_MAX_BONDS)) {
            return BLE_HS_ESTORE_CAP;
        }
    }

    p_dev_rec->rec_used = 1;
    memcpy(p_dev_rec->pseudo_addr, peer_addr, BLE_DEV_ADDR_LEN);
    memcpy(p_dev_rec->rand_addr, peer_addr, BLE_DEV_ADDR_LEN);
    memcpy(p_dev_rec->identity_addr, peer_addr, BLE_DEV_ADDR_LEN);
    ble_store_num_peer_dev_rec++;

    return 0;
}

static struct ble_hs_resolv_entry *
ble_rpa_find_rl_from_peer_records(uint8_t *peer_addr, uint8_t *peer_addr_type)
{
    struct ble_hs_dev_records *p_dev_rec = NULL;
    struct ble_hs_resolv_entry *rl = NULL;
    int i;
    int rc = 0;

    for (i = (ble_store_num_peer_dev_rec - 1); i >= 0; i--) {
        p_dev_rec = &peer_dev_rec[i];
        /* If the record is not used, skip */
        if (!(p_dev_rec->rec_used)) {
            continue;
        }

        if (is_rpa_resolvable_by_peer_rec(p_dev_rec, peer_addr)) {
            memcpy(p_dev_rec->rand_addr, peer_addr, BLE_DEV_ADDR_LEN);
            p_dev_rec->rand_addr_type = *peer_addr_type;
            rl = ble_hs_resolv_list_find(p_dev_rec->identity_addr);
            if (rl) {
                memcpy(peer_addr, p_dev_rec->identity_addr, BLE_DEV_ADDR_LEN);
                *peer_addr_type = p_dev_rec->peer_sec.peer_addr.type;
            } else if ((rl = ble_hs_resolv_list_find(p_dev_rec->pseudo_addr))
                       != NULL) {
                memcpy(peer_addr, p_dev_rec->identity_addr, BLE_DEV_ADDR_LEN);
                *peer_addr_type = p_dev_rec->peer_sec.peer_addr.type;
            } else {
                /* Peer record exists, but RL does not
                 * exist, remove peer record */
                BLE_HS_LOG(DEBUG, "RPA resolvable but RL doesn't exist; remove"
                           " peer rec at index = %d \n", i);
                rc = ble_rpa_remove_peer_dev_rec(p_dev_rec);
                if (rc != 0) {
                    BLE_HS_LOG(ERROR, "Unexpected error; index exceeds max capacity\n");
                }
            }
            /* Break loop, unique entry */
            break;
        } else {
            /* As peer_dev_rec is added as soon as peer RPA
             * is spotted, we might encounter duplications,
             * remove peer_dev_rec here itself */
            if ((!memcmp(p_dev_rec->rand_addr, peer_addr, BLE_DEV_ADDR_LEN)) ||
                    (!memcmp(p_dev_rec->pseudo_addr, peer_addr, BLE_DEV_ADDR_LEN)) ||
                    (!memcmp(p_dev_rec->identity_addr, peer_addr, BLE_DEV_ADDR_LEN))) {
                BLE_HS_LOG(DEBUG, "RPA NOT resolvable; remove"
                           " peer rec at index = %d \n", i);
                rc = ble_rpa_remove_peer_dev_rec(p_dev_rec);
                if (rc != 0) {
                    BLE_HS_LOG(ERROR, "Unexpected error; index exceeds max capacity\n");
                    break;
                }
            }
        }
    }

    return rl;
}

void
ble_rpa_replace_peer_params_with_rl(uint8_t *peer_addr, uint8_t *addr_type,
                                    struct ble_hs_resolv_entry **rl)
{
    struct ble_hs_resolv_entry *rl_tmp = NULL;
    bool is_rpa = ble_hs_is_rpa(peer_addr, *addr_type);

    if (is_rpa) {
        ble_hs_log_flat_buf(peer_addr, BLE_DEV_ADDR_LEN);
        BLE_HS_LOG(DEBUG, "\n");

        /* Try to find RL from your peer_device records */
        rl_tmp = ble_rpa_find_rl_from_peer_records(peer_addr, addr_type);
    }

    if (rl != NULL) {
        *rl = rl_tmp;
    }
}

/****  APIs for Resolving List:  ****/

/**
 * Returns whether or not address resolution is enabled.
 *
 * @return uint8_t
 */
static uint8_t
is_ble_hs_resolv_enabled(void)
{
    return g_ble_hs_resolv_data.addr_res_enabled;
}

/* Check if the Host based RPA is enabled */
bool
ble_host_rpa_enabled(void)
{
    if (nrpa_pvcy) {
        return false;
    }

    if (is_ble_hs_resolv_enabled() && ble_hs_pvcy_enabled()) {
        return true;
    }
    return false;
}

static void
ble_hs_rand_prand_get(uint8_t *prand)
{
    uint16_t sum;
    uint8_t rc;

    while (1) {
        /* Get 24 bits of random data */
        rc = ble_hs_hci_util_rand(prand, 3);
        if (rc != 0) {
            return;
        }

        /* Prand cannot be all zeros or 1's. */
        sum = prand[0] + prand[1] + prand[2];
        if ((sum != 0) && (sum != (3 * 0xff))) {
            break;
        }
    }

    /* Upper two bits must be 01 */
    prand[2] &= ~0xc0;
    prand[2] |= 0x40;
}

/**
 * Called to generate a resolvable private address in rl structure
 *
 * @param rl
 * @param local
 */
static void
ble_hs_resolv_gen_priv_addr(struct ble_hs_resolv_entry *rl, int local)
{
    uint8_t *irk = NULL;
    uint8_t *prand = NULL;
    struct ble_encryption_block ecb = {0};
    uint8_t *addr = NULL;

    if (local) {
        addr = rl->rl_local_rpa;
        irk = rl->rl_local_irk;
    } else {
        addr = rl->rl_peer_rpa;
        irk = rl->rl_peer_irk;
    }

    /* Get prand */
    prand = addr + 3;
    ble_hs_rand_prand_get(prand);

    /* Calculate hash, hash = ah(local IRK, prand) */
    memcpy(ecb.key, irk, 16);

    memset(ecb.plain_text, 0, 13);
    ecb.plain_text[13] = prand[2];
    ecb.plain_text[14] = prand[1];
    ecb.plain_text[15] = prand[0];

    swap_in_place(ecb.key, 16);
    swap_in_place(ecb.plain_text, 16);

    /* Calculate hash */
    if (ble_sm_alg_encrypt(ecb.key, ecb.plain_text, ecb.cipher_text) != 0) {
        /* We can't do much here if the encryption fails */
        return;
    }
    swap_in_place(ecb.cipher_text, 16);

    addr[0] = ecb.cipher_text[15];
    addr[1] = ecb.cipher_text[14];
    addr[2] = ecb.cipher_text[13];
}

/* Called to generate private (RPA/NRPA) address and this address is set in controller as
 * Random address. This is necessary in Host based privacy because controller
 * is unaware of private address is being used */
int
ble_hs_gen_own_private_rnd(void)
{
    if (nrpa_pvcy) {
        return ble_hs_id_set_nrpa_rnd();
    }

    struct ble_hs_resolv_entry *rl = &g_ble_hs_resolv_list[0];

    ble_hs_resolv_gen_priv_addr(rl, 1);
    return ble_hs_id_set_pseudo_rnd(rl->rl_local_rpa);
}

/* Called to fetch local RPA address */
uint8_t *
ble_hs_get_rpa_local(void)
{
    struct ble_hs_resolv_entry *rl = &g_ble_hs_resolv_list[0];
    return rl->rl_local_rpa;
}

/**
 * Called when the Resolvable private address timer expires. This timer
 * is used to regenerate local and peers RPA's in the resolving list.
 */
static void
ble_hs_resolv_rpa_timer_cb(struct ble_npl_event *ev)
{
    if (ble_host_rpa_enabled() || (nrpa_pvcy)) {
        BLE_HS_LOG(DEBUG, "RPA/NRPA Timeout; start active adv & scan with new Private address \n");
        ble_gap_preempt();
        /* Generate local private address */
        ble_hs_gen_own_private_rnd();
        ble_npl_callout_reset(&g_ble_hs_resolv_data.rpa_timer,
                              (int32_t)g_ble_hs_resolv_data.rpa_tmo);
        ble_gap_preempt_done();
    }

    return;
}

static bool
is_irk_nonzero(uint8_t *irk)
{
    int i;
    bool rc = false;

    for (i = 0; i < 16; ++i) {
        if (*irk != 0) {
            rc = true;
            break;
        }
        ++irk;
    }

    return rc;
}

/**
 * Used to determine if the device is on the resolving list.
 *
 * @param addr
 * @param addr_type Public address (0) or random address (1)
 *
 * @return int 0: device is not on resolving list; otherwise the return value
 * is the 'position' of the device in the resolving list (the index of the
 * element).
 */
static int
ble_hs_is_on_resolv_list(uint8_t *addr, uint8_t addr_type)
{
    int i;
    struct ble_hs_resolv_entry *rl = &g_ble_hs_resolv_list[1];

    for (i = 1; i < g_ble_hs_resolv_data.rl_cnt; ++i) {
        if ((rl->rl_addr_type == addr_type) &&
                (!memcmp(rl->rl_identity_addr, addr, BLE_DEV_ADDR_LEN))) {
            return i;
        }
        ++rl;
    }

    return 0;
}

/**
 * Used to find Resolving list entry with the provided address field.
 *
 * @param addr
 *
 * @return Pointer to resolving list entry or NULL if no entry found.
 */
struct ble_hs_resolv_entry *
ble_hs_resolv_list_find(uint8_t *addr)
{
    int i;
    struct ble_hs_resolv_entry *rl = &g_ble_hs_resolv_list[1];

    for (i = 1; i < g_ble_hs_resolv_data.rl_cnt; ++i) {
        if (memcmp(rl->rl_identity_addr, addr, BLE_DEV_ADDR_LEN) == 0) {
            return rl;
        }

        if (memcmp(rl->rl_pseudo_id, addr, BLE_DEV_ADDR_LEN) == 0) {
            return rl;
        }

        if (memcmp(rl->rl_peer_rpa, addr, BLE_DEV_ADDR_LEN) == 0) {
            return rl;
        }
        ++rl;
    }
    return NULL;
}

/**
 * Add a device to the resolving list
 *
 * @return int
 */
int
ble_hs_resolv_list_add(uint8_t *cmdbuf)
{
    uint8_t addr_type;
    uint8_t *ident_addr;
    struct ble_hs_resolv_entry *rl;
    struct ble_hs_dev_records *p_dev_rec = NULL;

    /* Check if we have any open entries */
    if (g_ble_hs_resolv_data.rl_cnt >= BLE_RESOLV_LIST_SIZE) {
        return BLE_HS_ENOMEM;
    }

    addr_type = cmdbuf[0];
    ident_addr = cmdbuf + 1;

/*--------------------------------------------------------------------------------*/
    /* Temporary workaround to resolve an issue when deinitializing the stack
     * and reinitializing. If the a peer deletes the bonding info after deiniting
     * it will not be able to re-bond without this. Awaiting upstream fix.
     */
/*
    if (ble_hs_is_on_resolv_list(ident_addr, addr_type)) {
        return BLE_HS_EINVAL;
    }
*/
    int position = ble_hs_is_on_resolv_list(ident_addr, addr_type);
    if (position) {
        memmove(&g_ble_hs_resolv_list[position],
                &g_ble_hs_resolv_list[position + 1],
                (g_ble_hs_resolv_data.rl_cnt - position) * sizeof (struct
                        ble_hs_resolv_entry));
        --g_ble_hs_resolv_data.rl_cnt;
    }
/*--------------------------------------------------------------------------------*/

    rl = &g_ble_hs_resolv_list[g_ble_hs_resolv_data.rl_cnt];
    memset(rl, 0, sizeof(*rl));

    rl->rl_addr_type = addr_type;
    memcpy(rl->rl_identity_addr, ident_addr, BLE_DEV_ADDR_LEN);
    swap_buf(rl->rl_peer_irk, cmdbuf + 7, 16);
    swap_buf(rl->rl_local_irk, cmdbuf + 23, 16);

    if (is_irk_nonzero(rl->rl_peer_irk)) {
        p_dev_rec = ble_rpa_find_peer_dev_by_irk(rl->rl_peer_irk);

        if (p_dev_rec != NULL) {
            memcpy(p_dev_rec->identity_addr, ident_addr, 6);
            memcpy(rl->rl_pseudo_id, p_dev_rec->pseudo_addr, 6);
        }
    }

    /* generate a local and peer RPAs now, those will be updated by timer
     * when resolution is enabled
     */
    ble_hs_resolv_gen_priv_addr(rl, 1);
    ble_hs_resolv_gen_priv_addr(rl, 0);
    ++(g_ble_hs_resolv_data.rl_cnt);
    BLE_HS_LOG(DEBUG, "Device added to RL, Resolving list count = %d\n", g_ble_hs_resolv_data.rl_cnt);

    return 0;
}

/**
 * Remove a device from the resolving list
 *
 * @return int 0: success, BLE Host error code otherwise
 */
int
ble_hs_resolv_list_rmv(uint8_t addr_type, uint8_t *ident_addr)
{
    int position, rc = BLE_HS_ENOENT;

    /* Remove from IRK records */
    position = ble_hs_is_on_resolv_list(ident_addr, addr_type);
    if (position) {

        memmove(&g_ble_hs_resolv_list[position],
                &g_ble_hs_resolv_list[position + 1],
                (g_ble_hs_resolv_data.rl_cnt - position) * sizeof (struct
                        ble_hs_resolv_entry));
        --g_ble_hs_resolv_data.rl_cnt;

        rc = 0;
    }

    /* As we are removing the RL record, it is needed to change
     * peer_address to its latest received OTA address, this helps when existing bond at
     * peer side is removed */
    ble_rpa_replace_id_with_rand_addr(&addr_type, ident_addr);

    return rc;
}

/**
 * Clear the resolving list
 */
void
ble_hs_resolv_list_clear_all(void)
{
    g_ble_hs_resolv_data.rl_cnt = 0;
    memset(g_ble_hs_resolv_list, 0, BLE_RESOLV_LIST_SIZE * sizeof(struct
           ble_hs_resolv_entry));

    /* Now delete peer device records as well */
    ble_rpa_peer_dev_rec_clear_all();

    return;
}

/**
* Called by host stack to enable NRPA privacy flag for future reference
*/
void
ble_hs_resolv_nrpa_enable(void)
{
    nrpa_pvcy = true;
}

/**
* Called by host stack to disable NRPA privacy flag
*/
void
ble_hs_resolv_nrpa_disable(void)
{
    nrpa_pvcy = false;
}
/**
 * Called to enable or disable address resolution in the host
 *
 * @param bool
 *
 * @return int
 */
void
ble_hs_resolv_enable(bool enable)
{
    int32_t tmo;

    /* If we change state, we need to disable/enable the RPA timer */
    if (enable ^ g_ble_hs_resolv_data.addr_res_enabled) {
        if (enable) {
            tmo = (int32_t)g_ble_hs_resolv_data.rpa_tmo;
            ble_npl_callout_reset(&g_ble_hs_resolv_data.rpa_timer, tmo);
        } else {
            ble_npl_callout_stop(&g_ble_hs_resolv_data.rpa_timer);
        }
        g_ble_hs_resolv_data.addr_res_enabled = enable;
    }

    return;
}

/**
 * Set the resolvable private address timeout.
 *
 * @param cmdbuf
 *
 * @return int
 */
int
ble_hs_resolv_set_rpa_tmo(uint16_t tmo_secs)
{
    /* Though the check validates smaller timeout values, it is recommended to
     * set it to enough bigger value (~15 minutes). There is no point in
     * changing RPA address aggressively and ending up sacrificing normal BLE
     * operations. Max RPA_TIMEOUT is ~11.5HRS (Spec v4.2, Vol 2, Part E,
     * section 7.8.45) */
    if (!((tmo_secs > 0) && (tmo_secs <= BLE_MAX_RPA_TIMEOUT_VAL))) {
        return BLE_HS_EINVAL;
    }

    g_ble_hs_resolv_data.rpa_tmo = ble_npl_time_ms_to_ticks32(tmo_secs * 1000);

    /* If resolving is not enabled, we are done here. */
    if (!is_ble_hs_resolv_enabled()) {
        return 0;
    }

    /* Reset timeout if resolving is enabled */
    ble_npl_callout_reset(&g_ble_hs_resolv_data.rpa_timer,
                          (int32_t)g_ble_hs_resolv_data.rpa_tmo);

    return 0;
}

/**
 * Resolve a Resolvable Private Address
 *
 * @param rpa
 * @param index
 *
 * @return int
 */
int
ble_hs_resolv_rpa(uint8_t *rpa, uint8_t *irk)
{
    int rc;
    struct ble_encryption_block ecb = {0};

    if (!(is_irk_nonzero(irk))) {
        return BLE_HS_EINVAL;
    }

    swap_buf(ecb.key, irk, 16);
    memset(ecb.plain_text, 0, 16);

    ecb.plain_text[15] = rpa[3];
    ecb.plain_text[14] = rpa[4];
    ecb.plain_text[13] = rpa[5];

    swap_in_place(ecb.plain_text, 16);

    /* Send the data to ble_sm_alg_encrypt in little-endian style */
    rc = ble_sm_alg_encrypt(ecb.key, ecb.plain_text, ecb.cipher_text);
    if (rc != 0) {
        return rc;
    }
    swap_in_place(ecb.cipher_text, 16);

    if ((ecb.cipher_text[15] == rpa[0]) && (ecb.cipher_text[14] == rpa[1]) &&
            (ecb.cipher_text[13] == rpa[2])) {
        rc = 0;
    } else {
        rc = BLE_HS_ENOENT;
    }

    return rc;
}

void ble_hs_resolv_init(void)
{
    g_ble_hs_resolv_data.rpa_tmo = ble_npl_time_ms_to_ticks32(MYNEWT_VAL(BLE_RPA_TIMEOUT) * 1000);

    ble_npl_callout_init(&g_ble_hs_resolv_data.rpa_timer,
                         ble_hs_evq_get(),
                         ble_hs_resolv_rpa_timer_cb,
                         NULL);
}

#endif  /* if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY) */
