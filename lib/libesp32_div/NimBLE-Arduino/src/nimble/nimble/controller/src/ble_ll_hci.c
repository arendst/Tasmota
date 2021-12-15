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
#include "nimble/nimble/include/nimble/hci_common.h"
#include "nimble/nimble/include/nimble/ble_hci_trans.h"
#include "../include/controller/ble_hw.h"
#include "../include/controller/ble_ll_adv.h"
#include "../include/controller/ble_ll_scan.h"
#include "../include/controller/ble_ll.h"
#include "../include/controller/ble_ll_hci.h"
#include "../include/controller/ble_ll_whitelist.h"
#include "../include/controller/ble_ll_resolv.h"
#include "../include/controller/ble_ll_sync.h"
#include "ble_ll_priv.h"
#include "ble_ll_conn_priv.h"

#if MYNEWT_VAL(BLE_LL_DTM)
#include "ble_ll_dtm_priv.h"
#endif

static void ble_ll_hci_cmd_proc(struct ble_npl_event *ev);

/* OS event to enqueue command */
static struct ble_npl_event g_ble_ll_hci_cmd_ev;

/* LE event mask */
static uint64_t g_ble_ll_hci_le_event_mask;
static uint64_t g_ble_ll_hci_event_mask;
static uint64_t g_ble_ll_hci_event_mask2;

static int16_t rx_path_pwr_compensation;
static int16_t tx_path_pwr_compensation;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
static enum {
    ADV_MODE_ANY,
    ADV_MODE_LEGACY,
    ADV_MODE_EXT,
} hci_adv_mode;

bool ble_ll_hci_adv_mode_ext(void)
{
    return hci_adv_mode == ADV_MODE_EXT;
}
#else
bool
ble_ll_hci_adv_mode_ext(void)
{
    return false;
}
#endif

/**
 * ll hci get num cmd pkts
 *
 * Returns the number of command packets that the host is allowed to send
 * to the controller.
 *
 * @return uint8_t
 */
static uint8_t
ble_ll_hci_get_num_cmd_pkts(void)
{
    return BLE_LL_CFG_NUM_HCI_CMD_PKTS;
}

/**
 * Send an event to the host.
 *
 * @param evbuf Pointer to event buffer to send
 *
 * @return int 0: success; -1 otherwise.
 */
int
ble_ll_hci_event_send(struct ble_hci_ev *hci_ev)
{
    int rc;

    BLE_LL_DEBUG_GPIO(HCI_EV, 1);

    BLE_LL_ASSERT(sizeof(*hci_ev) + hci_ev->length <= BLE_LL_MAX_EVT_LEN);

    /* Count number of events sent */
    STATS_INC(ble_ll_stats, hci_events_sent);

    /* Send the event to the host */
    rc = ble_hci_trans_ll_evt_tx((uint8_t *)hci_ev);

    BLE_LL_DEBUG_GPIO(HCI_EV, 0);

    return rc;
}

/**
 * Created and sends a command complete event with the no-op opcode to the
 * host.
 */
void
ble_ll_hci_send_noop(void)
{
    struct ble_hci_ev_command_complete_nop *ev;
    struct ble_hci_ev *hci_ev;

    hci_ev = (void *) ble_hci_trans_buf_alloc(BLE_HCI_TRANS_BUF_EVT_HI);
    if (hci_ev) {
        /* Create a command complete event with a NO-OP opcode */
        hci_ev->opcode = BLE_HCI_EVCODE_COMMAND_COMPLETE;

        hci_ev->length = sizeof(*ev);
        ev = (void *)hci_ev->data;

        ev->num_packets = ble_ll_hci_get_num_cmd_pkts();
        ev->opcode = BLE_HCI_OPCODE_NOP;

        ble_ll_hci_event_send(hci_ev);
    }
}

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
/**
 * LE encrypt command
 *
 * @param cmdbuf
 * @param rspbuf
 * @param rsplen
 *
 * @return int
 */
static int
ble_ll_hci_le_encrypt(const uint8_t *cmdbuf, uint8_t len, uint8_t *rspbuf,
                      uint8_t *rsplen)
{
    const struct ble_hci_le_encrypt_cp *cmd = (const void *) cmdbuf;
    struct ble_hci_le_encrypt_rp *rsp = (void *)rspbuf;
    struct ble_encryption_block ecb;
    int rc;

    /* Call the link layer to encrypt the data */
    swap_buf(ecb.key, cmd->key, BLE_ENC_BLOCK_SIZE);
    swap_buf(ecb.plain_text, cmd->data, BLE_ENC_BLOCK_SIZE);
    rc = ble_hw_encrypt_block(&ecb);
    if (!rc) {
        swap_buf(rsp->data, ecb.cipher_text, BLE_ENC_BLOCK_SIZE);
        *rsplen = sizeof(*rsp);
        rc = BLE_ERR_SUCCESS;
    } else {
        rc = BLE_ERR_CTLR_BUSY;
    }

    return rc;
}
#endif

/**
 * LE rand command
 *
 * @param cmdbuf
 * @param rspbuf
 * @param rsplen
 *
 * @return int
 */
static int
ble_ll_hci_le_rand(uint8_t *rspbuf, uint8_t *rsplen)
{
    struct ble_hci_le_rand_rp *rsp = (void *) rspbuf;

    ble_ll_rand_data_get((uint8_t *)&rsp->random_number,
                         sizeof(rsp->random_number));

    *rsplen = sizeof(*rsp);
    return BLE_ERR_SUCCESS;
}

/**
 * Read local version
 *
 * @param rspbuf
 * @param rsplen
 *
 * @return int
 */
static int
ble_ll_hci_rd_local_version(uint8_t *rspbuf, uint8_t *rsplen)
{
    struct ble_hci_ip_rd_local_ver_rp *rsp = (void *) rspbuf;

    rsp->hci_ver = BLE_HCI_VER_BCS;
    rsp->hci_rev = 0;
    rsp->lmp_ver = BLE_LMP_VER_BCS;
    rsp->manufacturer = htole16(MYNEWT_VAL(BLE_LL_MFRG_ID));
    rsp->lmp_subver = 0;

    *rsplen = sizeof(*rsp);
    return BLE_ERR_SUCCESS;
}

/**
 * Read local supported features
 *
 * @param rspbuf
 * @param rsplen
 *
 * @return int
 */
static int
ble_ll_hci_rd_local_supp_feat(uint8_t *rspbuf, uint8_t *rsplen)
{
    struct ble_hci_ip_rd_loc_supp_feat_rp *rsp = (void *) rspbuf;

    /*
     * The only two bits we set here currently are (5th byte):
     *      BR/EDR not supported        (bit 5)
     *      LE supported (controller)   (bit 6)
     */
    rsp->features = htole64(0x0000006000000000);

    *rsplen = sizeof(*rsp);
    return BLE_ERR_SUCCESS;
}

/**
 * Read local supported commands
 *
 * @param rspbuf
 * @param rsplen
 *
 * @return int
 */
static int
ble_ll_hci_rd_local_supp_cmd(uint8_t *rspbuf, uint8_t *rsplen)
{
    struct ble_hci_ip_rd_loc_supp_cmd_rp *rsp = (void *) rspbuf;

    memset(rsp->commands, 0, sizeof(rsp->commands));
    memcpy(rsp->commands, g_ble_ll_supp_cmds, sizeof(g_ble_ll_supp_cmds));

    *rsplen = sizeof(*rsp);
    return BLE_ERR_SUCCESS;
}

/**
 * Called to read the public device address of the device
 *
 *
 * @param rspbuf
 * @param rsplen
 *
 * @return int
 */
static int
ble_ll_hci_rd_bd_addr(uint8_t *rspbuf, uint8_t *rsplen)
{
    struct ble_hci_ip_rd_bd_addr_rp *rsp = (void *) rspbuf;

    memcpy(rsp->addr, g_dev_addr, BLE_DEV_ADDR_LEN);

    *rsplen = sizeof(*rsp);
    return BLE_ERR_SUCCESS;
}

/**
 * ll hci set le event mask
 *
 * Called when the LL controller receives a set LE event mask command.
 *
 * Context: Link Layer task (HCI command parser)
 *
 * @param cmdbuf Pointer to command buf.
 *
 * @return int BLE_ERR_SUCCESS. Does not return any errors.
 */
static int
ble_ll_hci_set_le_event_mask(const uint8_t *cmdbuf, uint8_t len)
{
    const struct ble_hci_le_set_event_mask_cp *cmd = (const void *) cmdbuf;

    if (len != sizeof(*cmd)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    g_ble_ll_hci_le_event_mask = le64toh(cmd->event_mask);

    return BLE_ERR_SUCCESS;
}

/**
 * HCI read buffer size command. Returns the ACL data packet length and
 * num data packets.
 *
 * @param rspbuf Pointer to response buffer
 * @param rsplen Length of response buffer
 *
 * @return int BLE error code
 */
static int
ble_ll_hci_le_read_bufsize(uint8_t *rspbuf, uint8_t *rsplen)
{
    struct ble_hci_le_rd_buf_size_rp *rp = (void *) rspbuf;

    rp->data_len = htole16(g_ble_ll_data.ll_acl_pkt_size);
    rp->data_packets = g_ble_ll_data.ll_num_acl_pkts;

    *rsplen = sizeof(*rp);
    return BLE_ERR_SUCCESS;
}

#if (BLE_LL_BT5_PHY_SUPPORTED == 1)
/**
 * Checks the preferred phy masks for validity and places the preferred masks
 * in the input phy masks

 * @return int BLE_ERR_SUCCESS or BLE_ERR_INV_HCI_CMD_PARMS or BLE_ERR_UNSUPPORTED
 */
int
ble_ll_hci_chk_phy_masks(uint8_t all_phys, uint8_t tx_phys, uint8_t rx_phys,
                         uint8_t *txphy, uint8_t *rxphy)
{
    /* Check for RFU */
    if ((tx_phys & ~BLE_HCI_LE_PHY_PREF_MASK_ALL) ||
                    (rx_phys & ~BLE_HCI_LE_PHY_PREF_MASK_ALL)) {
        return BLE_ERR_UNSUPPORTED;
    }

    if ((!(all_phys & BLE_HCI_LE_PHY_NO_TX_PREF_MASK) && (tx_phys == 0)) ||
        (!(all_phys & BLE_HCI_LE_PHY_NO_RX_PREF_MASK) && (rx_phys == 0))) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    /* If phy not supported, return error */
#if !MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_2M_PHY)
    if((tx_phys & BLE_HCI_LE_PHY_2M_PREF_MASK) ||
                    (rx_phys & BLE_HCI_LE_PHY_2M_PREF_MASK)) {
        return BLE_ERR_UNSUPPORTED;
    }
#endif
#if !MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CODED_PHY)
    if ((tx_phys & BLE_HCI_LE_PHY_CODED_PREF_MASK) ||
                    (rx_phys & BLE_HCI_LE_PHY_CODED_PREF_MASK)) {
        return BLE_ERR_UNSUPPORTED;
    }
#endif
    /* Set the default PHY preferences */
    if (all_phys & BLE_HCI_LE_PHY_NO_TX_PREF_MASK) {
        tx_phys = BLE_HCI_LE_PHY_PREF_MASK_ALL;
    }
    *txphy = tx_phys;

    if (all_phys & BLE_HCI_LE_PHY_NO_RX_PREF_MASK) {
        rx_phys = BLE_HCI_LE_PHY_PREF_MASK_ALL;
    }
    *rxphy = rx_phys;

    return BLE_ERR_SUCCESS;
}

/**
 * Set PHY preferences for connection
 *
 * @param cmdbuf
 *
 * @return int
 */
static int
ble_ll_hci_le_set_def_phy(const uint8_t *cmdbuf, uint8_t len)
{
    const struct ble_hci_le_set_default_phy_cp *cmd = (const void *) cmdbuf;
    int rc;

    if (len != sizeof(*cmd)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    rc = ble_ll_hci_chk_phy_masks(cmd->all_phys, cmd->tx_phys, cmd->rx_phys,
                                  &g_ble_ll_data.ll_pref_tx_phys,
                                  &g_ble_ll_data.ll_pref_rx_phys);
    return rc;
}
#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_DATA_LEN_EXT)
/**
 * HCI write suggested default data length command.
 *
 * This command is used by the host to change the initial max tx octets/time
 * for all connections. Note that if the controller does not support the
 * requested times no error is returned; the controller simply ignores the
 * request (but remembers what the host requested for the read suggested
 * default data length command). The spec allows for the controller to
 * disregard the host.
 *
 * @param rspbuf Pointer to response buffer
 * @param rsplen Length of response buffer
 *
 * @return int BLE error code
 */
static int
ble_ll_hci_le_wr_sugg_data_len(const uint8_t *cmdbuf, uint8_t len)
{
    const struct ble_hci_le_wr_sugg_def_data_len_cp *cmd = (const void*) cmdbuf;
    uint16_t tx_oct;
    uint16_t tx_time;
    int rc;

    if (len != sizeof(*cmd)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    /* Get suggested octets and time */
    tx_oct = le16toh(cmd->max_tx_octets);
    tx_time = le16toh(cmd->max_tx_time);

    /* If valid, write into suggested and change connection initial times */
    if (ble_ll_chk_txrx_octets(tx_oct) && ble_ll_chk_txrx_time(tx_time)) {
        g_ble_ll_conn_params.sugg_tx_octets = (uint8_t)tx_oct;
        g_ble_ll_conn_params.sugg_tx_time = tx_time;

        /*
         * We can disregard host suggestion, but we are a nice controller so
         * let's use host suggestion, unless they exceed max supported values
         * in which case we just use our max.
         */
        g_ble_ll_conn_params.conn_init_max_tx_octets =
                        min(tx_oct, g_ble_ll_conn_params.supp_max_tx_octets);
        g_ble_ll_conn_params.conn_init_max_tx_time =
                        min(tx_time, g_ble_ll_conn_params.supp_max_tx_time);

        /*
         * Use the same for coded and uncoded defaults. These are used when PHY
         * parameters are initialized and we want to use values overridden by
         * host. Make sure we do not exceed max supported time on uncoded.
         */
        g_ble_ll_conn_params.conn_init_max_tx_time_uncoded =
                                min(BLE_LL_CONN_SUPP_TIME_MAX_UNCODED,
                                    g_ble_ll_conn_params.conn_init_max_tx_time);
        g_ble_ll_conn_params.conn_init_max_tx_time_coded =
                                g_ble_ll_conn_params.conn_init_max_tx_time;

        rc = BLE_ERR_SUCCESS;
    } else {
        rc = BLE_ERR_INV_HCI_CMD_PARMS;
    }

    return rc;
}

/**
 * HCI read suggested default data length command. Returns the controllers
 * initial max tx octet/time.
 *
 * @param rspbuf Pointer to response buffer
 * @param rsplen Length of response buffer
 *
 * @return int BLE error code
 */
static int
ble_ll_hci_le_rd_sugg_data_len(uint8_t *rspbuf, uint8_t *rsplen)
{
    struct ble_hci_le_rd_sugg_def_data_len_rp *rsp = (void *) rspbuf;

    /* Place the data packet length and number of packets in the buffer */
    rsp->max_tx_octets = htole16(g_ble_ll_conn_params.sugg_tx_octets);
    rsp->max_tx_time = htole16(g_ble_ll_conn_params.sugg_tx_time);

    *rsplen = sizeof(*rsp);
    return BLE_ERR_SUCCESS;
}

/**
 * HCI read maximum data length command. Returns the controllers max supported
 * rx/tx octets/times.
 *
 * @param rspbuf Pointer to response buffer
 * @param rsplen Length of response buffer
 *
 * @return int BLE error code
 */
static int
ble_ll_hci_le_rd_max_data_len(uint8_t *rspbuf, uint8_t *rsplen)
{
    struct ble_hci_le_rd_max_data_len_rp *rsp = (void *)rspbuf;

    /* Place the data packet length and number of packets in the buffer */
    rsp->max_tx_octests = htole16(g_ble_ll_conn_params.supp_max_tx_octets);
    rsp->max_tx_time = htole16(g_ble_ll_conn_params.supp_max_tx_time);
    rsp->max_rx_octests = htole16(g_ble_ll_conn_params.supp_max_rx_octets);
    rsp->max_rx_time = htole16(g_ble_ll_conn_params.supp_max_rx_time);

    *rsplen = sizeof(*rsp);
    return BLE_ERR_SUCCESS;
}
#endif

/**
 * HCI read local supported features command. Returns the features
 * supported by the controller.
 *
 * @param rspbuf Pointer to response buffer
 * @param rsplen Length of response buffer
 *
 * @return int BLE error code
 */
static int
ble_ll_hci_le_read_local_features(uint8_t *rspbuf, uint8_t *rsplen)
{
    struct ble_hci_le_rd_loc_supp_feat_rp *rsp = (void *) rspbuf;

    rsp->features = htole64(ble_ll_read_supp_features());

    *rsplen = sizeof(*rsp);
    return BLE_ERR_SUCCESS;
}

/**
 * HCI read local supported states command. Returns the states
 * supported by the controller.
 *
 * @param rspbuf Pointer to response buffer
 * @param rsplen Length of response buffer
 *
 * @return int BLE error code
 */
static int
ble_ll_hci_le_read_supp_states(uint8_t *rspbuf, uint8_t *rsplen)
{
    struct ble_hci_le_rd_supp_states_rp *rsp = (void *) rspbuf;

    /* Add list of supported states. */
    rsp->states = htole64(ble_ll_read_supp_states());

    *rsplen = sizeof(*rsp);
    return BLE_ERR_SUCCESS;
}


/**
 * Checks to see if a LE event has been disabled by the host.
 *
 * @param subev Sub-event code of the LE Meta event. Note that this can
 * be a value from 1 to 64, inclusive.
 *
 * @return uint8_t 0: event is not enabled; otherwise event is enabled.
 */
bool
ble_ll_hci_is_le_event_enabled(unsigned int subev)
{
    /* The LE meta event must be enabled for any LE event to be enabled */
    if (g_ble_ll_hci_event_mask & (1ull << (BLE_HCI_EVCODE_LE_META - 1))) {
        return g_ble_ll_hci_le_event_mask & (1ull << (subev - 1));
    }

    return false;
}

/**
 * Checks to see if an event has been disabled by the host.
 *
 * NOTE: there are two "pages" of event masks; the first page is for event
 * codes between 0 and 63 and the second page is for event codes 64 and
 * greater.
 *
 * @param evcode This is the event code for the event.
 *
 * @return uint8_t 0: event is not enabled; otherwise event is enabled.
 */
bool
ble_ll_hci_is_event_enabled(unsigned int evcode)
{
    if (evcode >= 64) {
        return g_ble_ll_hci_event_mask2 & (1ull << (evcode - 64));
    }

    return g_ble_ll_hci_event_mask & (1ull << (evcode - 1));
}

/**
 * Called to determine if the reply to the command should be a command complete
 * event or a command status event.
 *
 * @param ocf
 *
 * @return int 0: return command complete; 1: return command status event
 */
static int
ble_ll_hci_le_cmd_send_cmd_status(uint16_t ocf)
{
    int rc;

    switch (ocf) {
    case BLE_HCI_OCF_LE_RD_REM_FEAT:
    case BLE_HCI_OCF_LE_CREATE_CONN:
    case BLE_HCI_OCF_LE_EXT_CREATE_CONN:
    case BLE_HCI_OCF_LE_CONN_UPDATE:
    case BLE_HCI_OCF_LE_START_ENCRYPT:
    case BLE_HCI_OCF_LE_RD_P256_PUBKEY:
    case BLE_HCI_OCF_LE_GEN_DHKEY:
    case BLE_HCI_OCF_LE_SET_PHY:
    case BLE_HCI_OCF_LE_PERIODIC_ADV_CREATE_SYNC:
        rc = 1;
        break;
    default:
        rc = 0;
        break;
    }
    return rc;
}

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
/** HCI LE read maximum advertising data length command. Returns the controllers
* max supported advertising data length;
*
* @param rspbuf Pointer to response buffer
* @param rsplen Length of response buffer
*
* @return int BLE error code
*/
static int
ble_ll_adv_rd_max_adv_data_len(uint8_t *rspbuf, uint8_t *rsplen)
{
    struct ble_hci_le_rd_max_adv_data_len_rp *rsp = (void *) rspbuf;

    rsp->max_adv_data_len = htole16(BLE_ADV_DATA_MAX_LEN);

    *rsplen = sizeof(*rsp);
    return BLE_ERR_SUCCESS;
}

/**
 * HCI LE read number of supported advertising sets
 *
 * @param rspbuf Pointer to response buffer
 * @param rsplen Length of response buffer
 *
 * @return int BLE error code
 */
static int
ble_ll_adv_rd_sup_adv_sets(uint8_t *rspbuf, uint8_t *rsplen)
{
    struct ble_hci_le_rd_num_of_adv_sets_rp *rsp = (void *)rspbuf;

    rsp->num_sets = BLE_ADV_INSTANCES;

    *rsplen = sizeof(*rsp);
    return BLE_ERR_SUCCESS;
}

static bool
ble_ll_is_valid_adv_mode(uint8_t ocf)
{
    /*
     * If, since the last power-on or reset, the Host has ever issued a legacy
     * advertising command and then issues an extended advertising command, or
     * has ever issued an extended advertising command and then issues a legacy
     * advertising command, the Controller shall return the error code Command
     * Disallowed (0x0C).
    */

    switch(ocf) {
    case BLE_HCI_OCF_LE_CREATE_CONN:
    case BLE_HCI_OCF_LE_SET_ADV_PARAMS:
    case BLE_HCI_OCF_LE_SET_ADV_ENABLE:
    case BLE_HCI_OCF_LE_SET_ADV_DATA:
    case BLE_HCI_OCF_LE_SET_SCAN_PARAMS:
    case BLE_HCI_OCF_LE_SET_SCAN_ENABLE:
    case BLE_HCI_OCF_LE_SET_SCAN_RSP_DATA:
    case BLE_HCI_OCF_LE_RD_ADV_CHAN_TXPWR:
        if (hci_adv_mode == ADV_MODE_EXT) {
            return false;
        }

        hci_adv_mode = ADV_MODE_LEGACY;
        break;
    case BLE_HCI_OCF_LE_EXT_CREATE_CONN:
    case BLE_HCI_OCF_LE_SET_EXT_ADV_DATA:
    case BLE_HCI_OCF_LE_SET_EXT_ADV_ENABLE:
    case BLE_HCI_OCF_LE_SET_EXT_ADV_PARAM:
    case BLE_HCI_OCF_LE_SET_EXT_SCAN_ENABLE:
    case BLE_HCI_OCF_LE_SET_EXT_SCAN_PARAM:
    case BLE_HCI_OCF_LE_SET_EXT_SCAN_RSP_DATA:
    case BLE_HCI_OCF_LE_RD_MAX_ADV_DATA_LEN:
    case BLE_HCI_OCF_LE_RD_NUM_OF_ADV_SETS:
    case BLE_HCI_OCF_LE_REMOVE_ADV_SET:
    case BLE_HCI_OCF_LE_CLEAR_ADV_SETS:
    case BLE_HCI_OCF_LE_SET_PERIODIC_ADV_PARAMS:
    case BLE_HCI_OCF_LE_SET_PERIODIC_ADV_DATA:
    case BLE_HCI_OCF_LE_SET_PERIODIC_ADV_ENABLE:
    case BLE_HCI_OCF_LE_PERIODIC_ADV_CREATE_SYNC:
    case BLE_HCI_OCF_LE_PERIODIC_ADV_CREATE_SYNC_CANCEL:
    case BLE_HCI_OCF_LE_PERIODIC_ADV_TERM_SYNC:
    case BLE_HCI_OCF_LE_ADD_DEV_TO_PERIODIC_ADV_LIST:
    case BLE_HCI_OCF_LE_REM_DEV_FROM_PERIODIC_ADV_LIST:
    case BLE_HCI_OCF_LE_CLEAR_PERIODIC_ADV_LIST:
    case BLE_HCI_OCF_LE_RD_PERIODIC_ADV_LIST_SIZE:
#if MYNEWT_VAL(BLE_VERSION) >= 51
    case BLE_HCI_OCF_LE_PERIODIC_ADV_RECEIVE_ENABLE:
#endif
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV_SYNC_TRANSFER)
    case BLE_HCI_OCF_LE_PERIODIC_ADV_SYNC_TRANSFER:
    case BLE_HCI_OCF_LE_PERIODIC_ADV_SET_INFO_TRANSFER:
    case BLE_HCI_OCF_LE_PERIODIC_ADV_SYNC_TRANSFER_PARAMS:
    case BLE_HCI_OCF_LE_SET_DEFAULT_SYNC_TRANSFER_PARAMS:
#endif
        if (hci_adv_mode == ADV_MODE_LEGACY) {
            return false;
        }

        hci_adv_mode = ADV_MODE_EXT;
        break;
    default:
        break;
    }

    return true;
}
#endif

static int
ble_ll_read_tx_power(uint8_t *rspbuf, uint8_t *rsplen)
{
    struct ble_hci_le_rd_transmit_power_rp *rsp = (void *) rspbuf;

    rsp->min_tx_power = ble_phy_txpower_round(-127);
    rsp->max_tx_power = ble_phy_txpower_round(126);

    *rsplen = sizeof(*rsp);
    return BLE_ERR_SUCCESS;
}

static int
ble_ll_read_rf_path_compensation(uint8_t *rspbuf, uint8_t *rsplen)
{
    struct ble_hci_le_rd_rf_path_compensation_rp *rsp = (void *) rspbuf;

    rsp->rx_path_compensation = htole16(rx_path_pwr_compensation);
    rsp->tx_path_compensation = htole16(tx_path_pwr_compensation);

    *rsplen = sizeof(*rsp);;
    return BLE_ERR_SUCCESS;
}

static int
ble_ll_write_rf_path_compensation(const uint8_t *cmdbuf, uint8_t len)
{
    const struct ble_hci_le_wr_rf_path_compensation_cp *cmd = (const void *)cmdbuf;
    int16_t rx;
    int16_t tx;

    if (len != sizeof(*cmd)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    tx = le16toh(cmd->tx_path_compensation);
    rx = le16toh(cmd->rx_path_compensation);

    if ((tx < -1280) || (tx > 1280) || (rx < -1280) || (rx > 1280)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    tx_path_pwr_compensation = tx;
    rx_path_pwr_compensation = rx;

    ble_phy_set_rx_pwr_compensation(rx_path_pwr_compensation / 10);

    return BLE_ERR_SUCCESS;
}

int8_t
ble_ll_get_tx_pwr_compensation(void)
{
    return tx_path_pwr_compensation / 10;
}

/**
 * Process a LE command sent from the host to the controller. The HCI command
 * has a 3 byte command header followed by data. The header is:
 *  -> opcode (2 bytes)
 *  -> Length of parameters (1 byte; does include command header bytes).
 *
 * @param cmdbuf Pointer to command buffer. Points to start of command header.
 * @param ocf    Opcode command field.
 * @param *rsplen Pointer to length of response
 *
 * @return int  This function returns a BLE error code. If a command status
 *              event should be returned as opposed to command complete,
 *              256 gets added to the return value.
 */
static int
ble_ll_hci_le_cmd_proc(const uint8_t *cmdbuf, uint8_t len, uint16_t ocf,
                       uint8_t *rspbuf, uint8_t *rsplen,
                       ble_ll_hci_post_cmd_complete_cb *cb)
{
    int rc;

    /* Assume error; if all pass rc gets set to 0 */
    rc = BLE_ERR_INV_HCI_CMD_PARMS;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    if (!ble_ll_is_valid_adv_mode(ocf)) {
        rc = BLE_ERR_CMD_DISALLOWED;

        /*
         * This code is here because we add 256 to the return code to denote
         * that the reply to this command should be command status (as opposed to
         * command complete).
         *
         * For unknown HCI command let us return always command status as per
         * specification Bluetooth 5, Vol. 2, Chapter 4.4
         */
        if (ble_ll_hci_le_cmd_send_cmd_status(ocf)) {
            rc += (BLE_ERR_MAX + 1);
        }

        return rc;
    }
#endif

    switch (ocf) {
    case BLE_HCI_OCF_LE_SET_EVENT_MASK:
        rc = ble_ll_hci_set_le_event_mask(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_RD_BUF_SIZE:
        if (len == 0) {
            rc = ble_ll_hci_le_read_bufsize(rspbuf, rsplen);
        }
        break;
    case BLE_HCI_OCF_LE_RD_LOC_SUPP_FEAT:
        if (len == 0) {
            rc = ble_ll_hci_le_read_local_features(rspbuf, rsplen);
        }
        break;
    case BLE_HCI_OCF_LE_SET_RAND_ADDR:
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
        rc = ble_ll_set_random_addr(cmdbuf, len, hci_adv_mode == ADV_MODE_EXT);
#else
        rc = ble_ll_set_random_addr(cmdbuf, len, false);
#endif
        break;
    case BLE_HCI_OCF_LE_SET_ADV_PARAMS:
        rc = ble_ll_adv_set_adv_params(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_RD_ADV_CHAN_TXPWR:
        if (len == 0) {
            rc = ble_ll_adv_read_txpwr(rspbuf, rsplen);
        }
        break;
    case BLE_HCI_OCF_LE_SET_ADV_DATA:
        rc = ble_ll_hci_set_adv_data(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_SET_SCAN_RSP_DATA:
        rc = ble_ll_hci_set_scan_rsp_data(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_SET_ADV_ENABLE:
        rc = ble_ll_hci_adv_set_enable(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_SET_SCAN_PARAMS:
        rc = ble_ll_scan_set_scan_params(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_SET_SCAN_ENABLE:
        rc = ble_ll_hci_scan_set_enable(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_CREATE_CONN:
        rc = ble_ll_conn_create(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_CREATE_CONN_CANCEL:
        if (len == 0) {
            rc = ble_ll_conn_create_cancel(cb);
        }
        break;
    case BLE_HCI_OCF_LE_RD_WHITE_LIST_SIZE:
        if (len == 0) {
            rc = ble_ll_whitelist_read_size(rspbuf, rsplen);
        }
        break;
    case BLE_HCI_OCF_LE_CLEAR_WHITE_LIST:
        if (len == 0) {
            rc = ble_ll_whitelist_clear();
        }
        break;
    case BLE_HCI_OCF_LE_ADD_WHITE_LIST:
        rc = ble_ll_whitelist_add(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_RMV_WHITE_LIST:
        rc = ble_ll_whitelist_rmv(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_CONN_UPDATE:
        rc = ble_ll_conn_hci_update(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_SET_HOST_CHAN_CLASS:
        rc = ble_ll_conn_hci_set_chan_class(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_RD_CHAN_MAP:
        rc = ble_ll_conn_hci_rd_chan_map(cmdbuf, len, rspbuf, rsplen);
        break;
    case BLE_HCI_OCF_LE_RD_REM_FEAT:
        rc = ble_ll_conn_hci_read_rem_features(cmdbuf, len);
        break;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
    case BLE_HCI_OCF_LE_ENCRYPT:
        rc = ble_ll_hci_le_encrypt(cmdbuf, len, rspbuf, rsplen);
        break;
#endif
    case BLE_HCI_OCF_LE_RAND:
        if (len == 0) {
            rc = ble_ll_hci_le_rand(rspbuf, rsplen);
        }
        break;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
    case BLE_HCI_OCF_LE_START_ENCRYPT:
        rc = ble_ll_conn_hci_le_start_encrypt(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_LT_KEY_REQ_REPLY:
        rc = ble_ll_conn_hci_le_ltk_reply(cmdbuf, len, rspbuf, rsplen);
        break;
    case BLE_HCI_OCF_LE_LT_KEY_REQ_NEG_REPLY:
        rc = ble_ll_conn_hci_le_ltk_neg_reply(cmdbuf, len, rspbuf, rsplen);
        break;
#endif
    case BLE_HCI_OCF_LE_RD_SUPP_STATES :
        if (len == 0) {
            rc = ble_ll_hci_le_read_supp_states(rspbuf, rsplen);
        }
        break;
#if MYNEWT_VAL(BLE_LL_DTM)
    case BLE_HCI_OCF_LE_TX_TEST:
        rc = ble_ll_hci_dtm_tx_test(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_RX_TEST:
        rc = ble_ll_hci_dtm_rx_test(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_TEST_END:
        if (len == 0) {
            rc = ble_ll_dtm_end_test(rspbuf, rsplen);
        }
        break;
#endif
    case BLE_HCI_OCF_LE_REM_CONN_PARAM_RR:
        rc = ble_ll_conn_hci_param_rr(cmdbuf, len, rspbuf, rsplen);
        break;
    case BLE_HCI_OCF_LE_REM_CONN_PARAM_NRR:
        rc = ble_ll_conn_hci_param_nrr(cmdbuf, len, rspbuf, rsplen);
        break;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_DATA_LEN_EXT)
    case BLE_HCI_OCF_LE_SET_DATA_LEN:
        rc = ble_ll_conn_hci_set_data_len(cmdbuf, len, rspbuf, rsplen);
        break;
    case BLE_HCI_OCF_LE_RD_SUGG_DEF_DATA_LEN:
        if (len == 0) {
            rc = ble_ll_hci_le_rd_sugg_data_len(rspbuf, rsplen);
        }
        break;
    case BLE_HCI_OCF_LE_WR_SUGG_DEF_DATA_LEN:
        rc = ble_ll_hci_le_wr_sugg_data_len(cmdbuf, len);
        break;
#endif
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
    case BLE_HCI_OCF_LE_ADD_RESOLV_LIST:
        rc = ble_ll_resolv_list_add(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_RMV_RESOLV_LIST:
        rc = ble_ll_resolv_list_rmv(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_CLR_RESOLV_LIST:
        if (len == 0) {
            rc = ble_ll_resolv_list_clr();
        }
        break;
    case BLE_HCI_OCF_LE_RD_RESOLV_LIST_SIZE:
        if (len == 0) {
            rc = ble_ll_resolv_list_read_size(rspbuf, rsplen);
        }
        break;
    case BLE_HCI_OCF_LE_RD_PEER_RESOLV_ADDR:
        rc = ble_ll_resolv_peer_addr_rd(cmdbuf, len, rspbuf, rsplen);
        break;
    case BLE_HCI_OCF_LE_RD_LOCAL_RESOLV_ADDR:
        rc = ble_ll_resolv_local_addr_rd(cmdbuf, len, rspbuf, rsplen);
        break;
    case BLE_HCI_OCF_LE_SET_ADDR_RES_EN:
        rc = ble_ll_resolv_enable_cmd(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_SET_RPA_TMO:
        rc = ble_ll_resolv_set_rpa_tmo(cmdbuf, len);
        break;
#endif
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_DATA_LEN_EXT)
    case BLE_HCI_OCF_LE_RD_MAX_DATA_LEN:
        if (len == 0) {
            rc = ble_ll_hci_le_rd_max_data_len(rspbuf, rsplen);
        }
        break;
#endif
#if (BLE_LL_BT5_PHY_SUPPORTED == 1)
    case BLE_HCI_OCF_LE_RD_PHY:
        rc = ble_ll_conn_hci_le_rd_phy(cmdbuf, len, rspbuf, rsplen);
        break;
    case BLE_HCI_OCF_LE_SET_DEFAULT_PHY:
        rc = ble_ll_hci_le_set_def_phy(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_SET_PHY:
        rc = ble_ll_conn_hci_le_set_phy(cmdbuf, len);
        break;
#endif
#if MYNEWT_VAL(BLE_LL_DTM)
    case BLE_HCI_OCF_LE_RX_TEST_V2:
        rc = ble_ll_hci_dtm_rx_test_v2(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_TX_TEST_V2:
        rc = ble_ll_hci_dtm_tx_test_v2(cmdbuf, len);
        break;
#endif
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    case BLE_HCI_OCF_LE_SET_ADV_SET_RND_ADDR:
        rc = ble_ll_adv_hci_set_random_addr(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_SET_EXT_ADV_PARAM:
        rc = ble_ll_adv_ext_set_param(cmdbuf, len, rspbuf, rsplen);
        break;
    case BLE_HCI_OCF_LE_SET_EXT_ADV_DATA:
        rc = ble_ll_adv_ext_set_adv_data(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_SET_EXT_SCAN_RSP_DATA:
        rc = ble_ll_adv_ext_set_scan_rsp(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_SET_EXT_ADV_ENABLE:
        rc =  ble_ll_adv_ext_set_enable(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_RD_MAX_ADV_DATA_LEN:
        if (len == 0) {
            rc = ble_ll_adv_rd_max_adv_data_len(rspbuf, rsplen);
        }
        break;
    case BLE_HCI_OCF_LE_RD_NUM_OF_ADV_SETS:
        if (len == 0) {
            rc = ble_ll_adv_rd_sup_adv_sets(rspbuf, rsplen);
        }
        break;
    case BLE_HCI_OCF_LE_REMOVE_ADV_SET:
        rc =  ble_ll_adv_remove(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_CLEAR_ADV_SETS:
        if (len == 0) {
            rc =  ble_ll_adv_clear_all();
        }
        break;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV)
    case BLE_HCI_OCF_LE_SET_PERIODIC_ADV_PARAMS:
        rc = ble_ll_adv_periodic_set_param(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_SET_PERIODIC_ADV_DATA:
        rc = ble_ll_adv_periodic_set_data(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_SET_PERIODIC_ADV_ENABLE:
        rc = ble_ll_adv_periodic_enable(cmdbuf, len);
        break;
#endif
#endif
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    case BLE_HCI_OCF_LE_SET_EXT_SCAN_PARAM:
        rc = ble_ll_set_ext_scan_params(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_SET_EXT_SCAN_ENABLE:
        rc = ble_ll_hci_ext_scan_set_enable(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_EXT_CREATE_CONN:
        rc = ble_ll_ext_conn_create(cmdbuf, len);
        break;
#endif
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV)
    case BLE_HCI_OCF_LE_PERIODIC_ADV_CREATE_SYNC:
        rc = ble_ll_sync_create(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_PERIODIC_ADV_CREATE_SYNC_CANCEL:
        if (len == 0) {
            rc = ble_ll_sync_cancel(cb);
        }
        break;
    case BLE_HCI_OCF_LE_PERIODIC_ADV_TERM_SYNC:
        rc = ble_ll_sync_terminate(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_ADD_DEV_TO_PERIODIC_ADV_LIST:
        rc = ble_ll_sync_list_add(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_REM_DEV_FROM_PERIODIC_ADV_LIST:
        rc = ble_ll_sync_list_remove(cmdbuf, len);
        break;
    case BLE_HCI_OCF_LE_CLEAR_PERIODIC_ADV_LIST:
        if (len == 0) {
            rc = ble_ll_sync_list_clear();
        }
        break;
    case BLE_HCI_OCF_LE_RD_PERIODIC_ADV_LIST_SIZE:
        if (len == 0) {
            rc = ble_ll_sync_list_size(rspbuf, rsplen);
        }
        break;
#if MYNEWT_VAL(BLE_VERSION) >= 51
    case BLE_HCI_OCF_LE_PERIODIC_ADV_RECEIVE_ENABLE:
        rc = ble_ll_sync_receive_enable(cmdbuf, len);
        break;
#endif
#endif
    case BLE_HCI_OCF_LE_RD_TRANSMIT_POWER:
        rc = ble_ll_read_tx_power(rspbuf, rsplen);
        break;
    case BLE_HCI_OCF_LE_RD_RF_PATH_COMPENSATION:
        rc = ble_ll_read_rf_path_compensation(rspbuf, rsplen);
        break;
    case BLE_HCI_OCF_LE_WR_RF_PATH_COMPENSATION:
        rc = ble_ll_write_rf_path_compensation(cmdbuf, len);
        break;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
    case BLE_HCI_OCF_LE_SET_PRIVACY_MODE:
        rc = ble_ll_resolve_set_priv_mode(cmdbuf, len);
        break;
#endif
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV_SYNC_TRANSFER)
    case BLE_HCI_OCF_LE_PERIODIC_ADV_SYNC_TRANSFER:
        rc = ble_ll_sync_transfer(cmdbuf, len, rspbuf, rsplen);
        break;
    case BLE_HCI_OCF_LE_PERIODIC_ADV_SET_INFO_TRANSFER:
        rc = ble_ll_adv_periodic_set_info_transfer(cmdbuf, len, rspbuf, rsplen);
        break;
    case BLE_HCI_OCF_LE_PERIODIC_ADV_SYNC_TRANSFER_PARAMS:
        rc = ble_ll_set_sync_transfer_params(cmdbuf, len, rspbuf, rsplen);
        break;
    case BLE_HCI_OCF_LE_SET_DEFAULT_SYNC_TRANSFER_PARAMS:
        rc = ble_ll_set_default_sync_transfer_params(cmdbuf, len);
        break;
#endif
#if MYNEWT_VAL(BLE_VERSION) >= 52
    case BLE_HCI_OCF_LE_SET_HOST_FEAT:
        rc = ble_ll_set_host_feat(cmdbuf, len);
        break;
#endif
    default:
        rc = BLE_ERR_UNKNOWN_HCI_CMD;
        break;
    }

    /*
     * This code is here because we add 256 to the return code to denote
     * that the reply to this command should be command status (as opposed to
     * command complete).
     *
     * For unknown HCI command let us return always command status as per
     * specification Bluetooth 5, Vol. 2, Chapter 4.4
     */
    if (ble_ll_hci_le_cmd_send_cmd_status(ocf) || rc == BLE_ERR_UNKNOWN_HCI_CMD) {
        rc += (BLE_ERR_MAX + 1);
    }

    return rc;
}

/**
 * Process a link control command sent from the host to the controller. The HCI
 * command has a 3 byte command header followed by data. The header is:
 *  -> opcode (2 bytes)
 *  -> Length of parameters (1 byte; does include command header bytes).
 *
 * @param cmdbuf Pointer to command buffer. Points to start of command header.
 * @param ocf    Opcode command field.
 *
 * @return int  This function returns a BLE error code. If a command status
 *              event should be returned as opposed to command complete,
 *              256 gets added to the return value.
 */
static int
ble_ll_hci_link_ctrl_cmd_proc(const uint8_t *cmdbuf, uint8_t len, uint16_t ocf)
{
    int rc;

    switch (ocf) {
    case BLE_HCI_OCF_DISCONNECT_CMD:
        rc = ble_ll_conn_hci_disconnect_cmd(cmdbuf, len);
        /* Send command status instead of command complete */
        rc += (BLE_ERR_MAX + 1);
        break;

    case BLE_HCI_OCF_RD_REM_VER_INFO:
        rc = ble_ll_conn_hci_rd_rem_ver_cmd(cmdbuf, len);
        /* Send command status instead of command complete */
        rc += (BLE_ERR_MAX + 1);
        break;

    default:
        rc = BLE_ERR_UNKNOWN_HCI_CMD;
        break;
    }

    return rc;
}

static int
ble_ll_hci_cb_set_event_mask(const uint8_t *cmdbuf, uint8_t len)
{
    const struct ble_hci_cb_set_event_mask_cp *cmd = (const void *) cmdbuf;

    if (len != sizeof (*cmd)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    g_ble_ll_hci_event_mask = le64toh(cmd->event_mask);

    return BLE_ERR_SUCCESS;
}

static int
ble_ll_hci_cb_set_event_mask2(const uint8_t *cmdbuf, uint8_t len)
{
    const struct ble_hci_cb_set_event_mask2_cp *cmd = (const void *) cmdbuf;

    if (len != sizeof (*cmd)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    g_ble_ll_hci_event_mask2 = le64toh(cmd->event_mask2);

    return BLE_ERR_SUCCESS;
}

static int
ble_ll_hci_ctlr_bb_cmd_proc(const uint8_t *cmdbuf, uint8_t len, uint16_t ocf,
                            uint8_t *rspbuf, uint8_t *rsplen)
{
    int rc;

    /* Assume error; if all pass rc gets set to 0 */
    rc = BLE_ERR_INV_HCI_CMD_PARMS;

    switch (ocf) {
    case BLE_HCI_OCF_CB_SET_EVENT_MASK:
        rc = ble_ll_hci_cb_set_event_mask(cmdbuf, len);
        break;
    case BLE_HCI_OCF_CB_RESET:
        if (len == 0) {
            rc = ble_ll_reset();
        }
        break;
    case BLE_HCI_OCF_CB_SET_EVENT_MASK2:
        rc = ble_ll_hci_cb_set_event_mask2(cmdbuf, len);
        break;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_PING)
    case BLE_HCI_OCF_CB_RD_AUTH_PYLD_TMO:
        rc = ble_ll_conn_hci_rd_auth_pyld_tmo(cmdbuf, len, rspbuf, rsplen);
        break;
    case BLE_HCI_OCF_CB_WR_AUTH_PYLD_TMO:
        rc = ble_ll_conn_hci_wr_auth_pyld_tmo(cmdbuf, len, rspbuf, rsplen);
        break;
#endif
    default:
        rc = BLE_ERR_UNKNOWN_HCI_CMD;
        break;
    }

    return rc;
}

static int
ble_ll_hci_info_params_cmd_proc(const uint8_t *cmdbuf, uint8_t len,
                                uint16_t ocf, uint8_t *rspbuf, uint8_t *rsplen)
{
    int rc;

    /* Assume error; if all pass rc gets set to 0 */
    rc = BLE_ERR_INV_HCI_CMD_PARMS;

    switch (ocf) {
    case BLE_HCI_OCF_IP_RD_LOCAL_VER:
        if (len == 0) {
            rc = ble_ll_hci_rd_local_version(rspbuf, rsplen);
        }
        break;
    case BLE_HCI_OCF_IP_RD_LOC_SUPP_CMD:
        if (len == 0) {
            rc = ble_ll_hci_rd_local_supp_cmd(rspbuf, rsplen);
        }
        break;
    case BLE_HCI_OCF_IP_RD_LOC_SUPP_FEAT:
        if (len == 0) {
            rc = ble_ll_hci_rd_local_supp_feat(rspbuf, rsplen);
        }
        break;
    case BLE_HCI_OCF_IP_RD_BD_ADDR:
        if (len == 0) {
            rc = ble_ll_hci_rd_bd_addr(rspbuf, rsplen);
        }
        break;
    default:
        rc = BLE_ERR_UNKNOWN_HCI_CMD;
        break;
    }

    return rc;
}

static int
ble_ll_hci_status_params_cmd_proc(const uint8_t *cmdbuf, uint8_t len,
                                  uint16_t ocf, uint8_t *rspbuf,
                                  uint8_t *rsplen)
{
    int rc;

    switch (ocf) {
    case BLE_HCI_OCF_RD_RSSI:
        rc = ble_ll_conn_hci_rd_rssi(cmdbuf, len, rspbuf, rsplen);
        break;
    default:
        rc = BLE_ERR_UNKNOWN_HCI_CMD;
        break;
    }

    return rc;
}

/**
 * Called to process an HCI command from the host.
 *
 * @param ev Pointer to os event containing a pointer to command buffer
 */
static void
ble_ll_hci_cmd_proc(struct ble_npl_event *ev)
{
    int rc;
    uint8_t ogf;
    uint8_t rsplen;
    struct ble_hci_cmd *cmd;
    uint16_t opcode;
    uint16_t ocf;
    ble_ll_hci_post_cmd_complete_cb post_cb = NULL;
    struct ble_hci_ev *hci_ev;
    struct ble_hci_ev_command_status *cmd_status;
    struct ble_hci_ev_command_complete *cmd_complete;
    uint8_t *rspbuf;

    BLE_LL_DEBUG_GPIO(HCI_CMD, 1);

    /* The command buffer is the event argument */
    cmd = ble_npl_event_get_arg(ev);
    BLE_LL_ASSERT(cmd != NULL);

    /* Get the opcode from the command buffer */
    opcode = le16toh(cmd->opcode);
    ocf = BLE_HCI_OCF(opcode);
    ogf = BLE_HCI_OGF(opcode);

    /*
     * The command response pointer points into the same buffer as the
     * command data itself. That is fine, as each command reads all the data
     * before crafting a response.
     * Also reuse cmd buffer for complete event
     */
    hci_ev = (struct ble_hci_ev *) cmd;
    rspbuf = hci_ev->data + sizeof(*cmd_complete);

    /* Assume response length is zero */
    rsplen = 0;

    switch (ogf) {
    case BLE_HCI_OGF_LINK_CTRL:
        rc = ble_ll_hci_link_ctrl_cmd_proc(cmd->data, cmd->length, ocf);
        break;
    case BLE_HCI_OGF_CTLR_BASEBAND:
        rc = ble_ll_hci_ctlr_bb_cmd_proc(cmd->data, cmd->length, ocf, rspbuf, &rsplen);
        break;
    case BLE_HCI_OGF_INFO_PARAMS:
        rc = ble_ll_hci_info_params_cmd_proc(cmd->data, cmd->length, ocf, rspbuf, &rsplen);
        break;
    case BLE_HCI_OGF_STATUS_PARAMS:
        rc = ble_ll_hci_status_params_cmd_proc(cmd->data, cmd->length, ocf, rspbuf, &rsplen);
        break;
    case BLE_HCI_OGF_LE:
        rc = ble_ll_hci_le_cmd_proc(cmd->data, cmd->length, ocf, rspbuf, &rsplen, &post_cb);
        break;
    default:
        /* XXX: Need to support other OGF. For now, return unsupported */
        rc = BLE_ERR_UNKNOWN_HCI_CMD;
        break;
    }

    /* If no response is generated, we free the buffers */
    BLE_LL_ASSERT(rc >= 0);
    if (rc <= BLE_ERR_MAX) {
        /* Create a command complete event with status from command */
        hci_ev->opcode = BLE_HCI_EVCODE_COMMAND_COMPLETE;
        hci_ev->length = sizeof(*cmd_complete) + rsplen;

        cmd_complete = (void *) hci_ev->data;
        cmd_complete->num_packets = ble_ll_hci_get_num_cmd_pkts();
        cmd_complete->opcode = htole16(opcode);
        cmd_complete->status = (uint8_t) rc;
    } else {
        /* Create a command status event */
        rc -= (BLE_ERR_MAX + 1);

        hci_ev->opcode = BLE_HCI_EVCODE_COMMAND_STATUS;
        hci_ev->length = sizeof(*cmd_status);

        cmd_status = (void *) hci_ev->data;
        cmd_status->status = (uint8_t)rc;
        cmd_status->num_packets = ble_ll_hci_get_num_cmd_pkts();
        cmd_status->opcode = htole16(opcode);
    }

    /* Count commands and those in error */
    if (rc) {
        STATS_INC(ble_ll_stats, hci_cmd_errs);
    } else {
        STATS_INC(ble_ll_stats, hci_cmds);
    }

    /* Send the event (events cannot be masked) */
    ble_ll_hci_event_send(hci_ev);

    /* Call post callback if set by command handler */
    if (post_cb) {
        post_cb();
    }

    BLE_LL_DEBUG_GPIO(HCI_CMD, 0);
}

/**
 * Sends an HCI command to the controller.  On success, the supplied buffer is
 * relinquished to the controller task.  On failure, the caller must free the
 * buffer.
 *
 * @param cmd                   A flat buffer containing the HCI command to
 *                                  send.
 *
 * @return                      0 on success;
 *                              BLE_ERR_MEM_CAPACITY on HCI buffer exhaustion.
 */
int
ble_ll_hci_cmd_rx(uint8_t *cmd, void *arg)
{
    struct ble_npl_event *ev;

    /* Get an event structure off the queue */
    ev = &g_ble_ll_hci_cmd_ev;
    if (ble_npl_event_is_queued(ev)) {
        return BLE_ERR_MEM_CAPACITY;
    }

    /* Fill out the event and post to Link Layer */
    ble_npl_event_set_arg(ev, cmd);
    ble_npl_eventq_put(&g_ble_ll_data.ll_evq, ev);

    return 0;
}

/* Send ACL data from host to contoller */
int
ble_ll_hci_acl_rx(struct os_mbuf *om, void *arg)
{
    ble_ll_acl_data_in(om);
    return 0;
}

/**
 * Initalize the LL HCI.
 *
 * NOTE: This function is called by the HCI RESET command so if any code
 * is added here it must be OK to be executed when the reset command is used.
 */
void
ble_ll_hci_init(void)
{
    BLE_LL_DEBUG_GPIO_INIT(HCI_CMD);
    BLE_LL_DEBUG_GPIO_INIT(HCI_EV);

    /* Set event callback for command processing */
    ble_npl_event_init(&g_ble_ll_hci_cmd_ev, ble_ll_hci_cmd_proc, NULL);

    /* Set defaults for LE events: Vol 2 Part E 7.8.1 */
    g_ble_ll_hci_le_event_mask = 0x1f;

    /* Set defaults for controller/baseband events: Vol 2 Part E 7.3.1 */
    g_ble_ll_hci_event_mask = 0x1fffffffffff;


    /* Set page 2 to 0 */
    g_ble_ll_hci_event_mask2 = 0;

    /* reset RF path compensation values */
    rx_path_pwr_compensation = 0;
    tx_path_pwr_compensation = 0;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    /* after reset both legacy and extended advertising commands are allowed */
    hci_adv_mode = ADV_MODE_ANY;
#endif
}

#endif
