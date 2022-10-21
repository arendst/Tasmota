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
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "nimble/porting/nimble/include/sysinit/sysinit.h"
#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#include "nimble/porting/nimble/include/os/os.h"
#include "nimble/porting/nimble/include/os/os_cputime.h"
#include "nimble/porting/nimble/include/stats/stats.h"
#include "nimble/nimble/include/nimble/ble.h"
#include "nimble/nimble/include/nimble/nimble_opt.h"
#include "nimble/nimble/include/nimble/hci_common.h"
#include "nimble/nimble/include/nimble/ble_hci_trans.h"
#include "../include/controller/ble_hw.h"
#include "../include/controller/ble_phy.h"
#include "../include/controller/ble_phy_trace.h"
#include "../include/controller/ble_ll.h"
#include "../include/controller/ble_ll_adv.h"
#include "../include/controller/ble_ll_sched.h"
#include "../include/controller/ble_ll_scan.h"
#include "../include/controller/ble_ll_hci.h"
#include "../include/controller/ble_ll_whitelist.h"
#include "../include/controller/ble_ll_resolv.h"
#include "../include/controller/ble_ll_rfmgmt.h"
#include "../include/controller/ble_ll_trace.h"
#include "../include/controller/ble_ll_sync.h"
#include "ble_ll_conn_priv.h"

#if MYNEWT_VAL(BLE_LL_DTM)
#include "ble_ll_dtm_priv.h"
#endif

/* XXX:
 *
 * 1) use the sanity task!
 * 2) Need to figure out what to do with packets that we hand up that did
 * not pass the filter policy for the given state. Currently I count all
 * packets I think. Need to figure out what to do with this.
 * 3) For the features defined, we need to conditionally compile code.
 * 4) Should look into always disabled the wfr interrupt if we receive the
 * start of a frame. Need to look at the various states to see if this is the
 * right thing to do.
 */

/* Supported states */
#define BLE_LL_S_NCA                    (0x00000000001)
#define BLE_LL_S_SA                     (0x00000000002)
#define BLE_LL_S_CA                     (0x00000000004)
#define BLE_LL_S_HDCA                   (0x00000000008)
#define BLE_LL_S_PS                     (0x00000000010)
#define BLE_LL_S_AS                     (0x00000000020)
#define BLE_LL_S_INIT                   (0x00000000040)
#define BLE_LL_S_SLAVE                  (0x00000000080)
#define BLE_LL_S_NCA_PS                 (0x00000000100)
#define BLE_LL_S_SA_PS                  (0x00000000200)
#define BLE_LL_S_CA_PS                  (0x00000000400)
#define BLE_LL_S_HDCA_PS                (0x00000000800)
#define BLE_LL_S_NCA_AS                 (0x00000001000)
#define BLE_LL_S_SA_AS                  (0x00000002000)
#define BLE_LL_S_CA_AS                  (0x00000004000)
#define BLE_LL_S_HDCA_AS                (0x00000008000)
#define BLE_LL_S_NCA_INIT               (0x00000010000)
#define BLE_LL_S_SA_INIT                (0x00000020000)
#define BLE_LL_S_NCA_MASTER             (0x00000040000)
#define BLE_LL_S_SA_MASTER              (0x00000080000)
#define BLE_LL_S_NCA_SLAVE              (0x00000100000)
#define BLE_LL_S_SA_SLAVE               (0x00000200000)
#define BLE_LL_S_PS_INIT                (0x00000400000)
#define BLE_LL_S_AS_INIT                (0x00000800000)
#define BLE_LL_S_PS_MASTER              (0x00001000000)
#define BLE_LL_S_AS_MASTER              (0x00002000000)
#define BLE_LL_S_PS_SLAVE               (0x00004000000)
#define BLE_LL_S_AS_SLAVE               (0x00008000000)
#define BLE_LL_S_INIT_MASTER            (0x00010000000)
#define BLE_LL_S_LDCA                   (0x00020000000)
#define BLE_LL_S_LDCA_PS                (0x00040000000)
#define BLE_LL_S_LDCA_AS                (0x00080000000)
#define BLE_LL_S_CA_INIT                (0x00100000000)
#define BLE_LL_S_HDCA_INIT              (0x00200000000)
#define BLE_LL_S_LDCA_INIT              (0x00400000000)
#define BLE_LL_S_CA_MASTER              (0x00800000000)
#define BLE_LL_S_HDCA_MASTER            (0x01000000000)
#define BLE_LL_S_LDCA_MASTER            (0x02000000000)
#define BLE_LL_S_CA_SLAVE               (0x04000000000)
#define BLE_LL_S_HDCA_SLAVE             (0x08000000000)
#define BLE_LL_S_LDCA_SLAVE             (0x10000000000)
#define BLE_LL_S_INIT_SLAVE             (0x20000000000)

#define BLE_LL_SUPPORTED_STATES             \
(                                           \
    BLE_LL_S_NCA                    |       \
    BLE_LL_S_SA                     |       \
    BLE_LL_S_CA                     |       \
    BLE_LL_S_HDCA                   |       \
    BLE_LL_S_PS                     |       \
    BLE_LL_S_AS                     |       \
    BLE_LL_S_INIT                   |       \
    BLE_LL_S_SLAVE                  |       \
    BLE_LL_S_NCA_PS                 |       \
    BLE_LL_S_SA_PS                  |       \
    BLE_LL_S_CA_PS                  |       \
    BLE_LL_S_HDCA_PS                |       \
    BLE_LL_S_NCA_AS                 |       \
    BLE_LL_S_SA_AS                  |       \
    BLE_LL_S_CA_AS                  |       \
    BLE_LL_S_HDCA_AS                |       \
    BLE_LL_S_NCA_INIT               |       \
    BLE_LL_S_SA_INIT                |       \
    BLE_LL_S_NCA_MASTER             |       \
    BLE_LL_S_SA_MASTER              |       \
    BLE_LL_S_NCA_SLAVE              |       \
    BLE_LL_S_SA_SLAVE               |       \
    BLE_LL_S_PS_INIT                |       \
    BLE_LL_S_AS_INIT                |       \
    BLE_LL_S_PS_MASTER              |       \
    BLE_LL_S_AS_MASTER              |       \
    BLE_LL_S_PS_SLAVE               |       \
    BLE_LL_S_AS_SLAVE               |       \
    BLE_LL_S_INIT_MASTER            |       \
    BLE_LL_S_LDCA                   |       \
    BLE_LL_S_LDCA_PS                |       \
    BLE_LL_S_LDCA_AS                |       \
    BLE_LL_S_CA_INIT                |       \
    BLE_LL_S_HDCA_INIT              |       \
    BLE_LL_S_LDCA_INIT              |       \
    BLE_LL_S_CA_MASTER              |       \
    BLE_LL_S_HDCA_MASTER            |       \
    BLE_LL_S_LDCA_MASTER            |       \
    BLE_LL_S_CA_SLAVE               |       \
    BLE_LL_S_HDCA_SLAVE             |       \
    BLE_LL_S_LDCA_SLAVE             |       \
    BLE_LL_S_INIT_SLAVE)

/* The global BLE LL data object */
struct ble_ll_obj g_ble_ll_data;

/* Global link layer statistics */
STATS_SECT_DECL(ble_ll_stats) ble_ll_stats;
STATS_NAME_START(ble_ll_stats)
    STATS_NAME(ble_ll_stats, hci_cmds)
    STATS_NAME(ble_ll_stats, hci_cmd_errs)
    STATS_NAME(ble_ll_stats, hci_events_sent)
    STATS_NAME(ble_ll_stats, bad_ll_state)
    STATS_NAME(ble_ll_stats, bad_acl_hdr)
    STATS_NAME(ble_ll_stats, no_bufs)
    STATS_NAME(ble_ll_stats, rx_adv_pdu_crc_ok)
    STATS_NAME(ble_ll_stats, rx_adv_pdu_crc_err)
    STATS_NAME(ble_ll_stats, rx_adv_bytes_crc_ok)
    STATS_NAME(ble_ll_stats, rx_adv_bytes_crc_err)
    STATS_NAME(ble_ll_stats, rx_data_pdu_crc_ok)
    STATS_NAME(ble_ll_stats, rx_data_pdu_crc_err)
    STATS_NAME(ble_ll_stats, rx_data_bytes_crc_ok)
    STATS_NAME(ble_ll_stats, rx_data_bytes_crc_err)
    STATS_NAME(ble_ll_stats, rx_adv_malformed_pkts)
    STATS_NAME(ble_ll_stats, rx_adv_ind)
    STATS_NAME(ble_ll_stats, rx_adv_direct_ind)
    STATS_NAME(ble_ll_stats, rx_adv_nonconn_ind)
    STATS_NAME(ble_ll_stats, rx_adv_ext_ind)
    STATS_NAME(ble_ll_stats, rx_scan_reqs)
    STATS_NAME(ble_ll_stats, rx_scan_rsps)
    STATS_NAME(ble_ll_stats, rx_connect_reqs)
    STATS_NAME(ble_ll_stats, rx_scan_ind)
    STATS_NAME(ble_ll_stats, rx_aux_connect_rsp)
    STATS_NAME(ble_ll_stats, adv_txg)
    STATS_NAME(ble_ll_stats, adv_late_starts)
    STATS_NAME(ble_ll_stats, adv_resched_pdu_fail)
    STATS_NAME(ble_ll_stats, adv_drop_event)
    STATS_NAME(ble_ll_stats, sched_state_conn_errs)
    STATS_NAME(ble_ll_stats, sched_state_adv_errs)
    STATS_NAME(ble_ll_stats, scan_starts)
    STATS_NAME(ble_ll_stats, scan_stops)
    STATS_NAME(ble_ll_stats, scan_req_txf)
    STATS_NAME(ble_ll_stats, scan_req_txg)
    STATS_NAME(ble_ll_stats, scan_rsp_txg)
    STATS_NAME(ble_ll_stats, aux_missed_adv)
    STATS_NAME(ble_ll_stats, aux_scheduled)
    STATS_NAME(ble_ll_stats, aux_received)
    STATS_NAME(ble_ll_stats, aux_fired_for_read)
    STATS_NAME(ble_ll_stats, aux_allocated)
    STATS_NAME(ble_ll_stats, aux_freed)
    STATS_NAME(ble_ll_stats, aux_sched_cb)
    STATS_NAME(ble_ll_stats, aux_conn_req_tx)
    STATS_NAME(ble_ll_stats, aux_conn_rsp_tx)
    STATS_NAME(ble_ll_stats, aux_conn_rsp_err)
    STATS_NAME(ble_ll_stats, aux_scan_req_tx)
    STATS_NAME(ble_ll_stats, aux_scan_rsp_err)
    STATS_NAME(ble_ll_stats, aux_chain_cnt)
    STATS_NAME(ble_ll_stats, aux_chain_err)
    STATS_NAME(ble_ll_stats, aux_scan_drop)
    STATS_NAME(ble_ll_stats, adv_evt_dropped)
    STATS_NAME(ble_ll_stats, scan_timer_stopped)
    STATS_NAME(ble_ll_stats, scan_timer_restarted)
    STATS_NAME(ble_ll_stats, periodic_adv_drop_event)
    STATS_NAME(ble_ll_stats, periodic_chain_drop_event)
    STATS_NAME(ble_ll_stats, sync_event_failed)
    STATS_NAME(ble_ll_stats, sync_received)
    STATS_NAME(ble_ll_stats, sync_chain_failed)
    STATS_NAME(ble_ll_stats, sync_missed_err)
    STATS_NAME(ble_ll_stats, sync_crc_err)
    STATS_NAME(ble_ll_stats, sync_rx_buf_err)
    STATS_NAME(ble_ll_stats, sync_scheduled)
    STATS_NAME(ble_ll_stats, sched_state_sync_errs)
    STATS_NAME(ble_ll_stats, sched_invalid_pdu)
STATS_NAME_END(ble_ll_stats)

static void ble_ll_event_rx_pkt(struct ble_npl_event *ev);
static void ble_ll_event_tx_pkt(struct ble_npl_event *ev);
static void ble_ll_event_dbuf_overflow(struct ble_npl_event *ev);

#if MYNEWT

/* The BLE LL task data structure */
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
#define BLE_LL_STACK_SIZE   (120)
#else
#define BLE_LL_STACK_SIZE   (90)
#endif

struct os_task g_ble_ll_task;

OS_TASK_STACK_DEFINE(g_ble_ll_stack, BLE_LL_STACK_SIZE);

#endif /* MYNEWT */

/** Our global device address (public) */
uint8_t g_dev_addr[BLE_DEV_ADDR_LEN];

/** Our random address */
uint8_t g_random_addr[BLE_DEV_ADDR_LEN];

static const uint16_t g_ble_ll_pdu_header_tx_time[BLE_PHY_NUM_MODE] =
{
    [BLE_PHY_MODE_1M] =
            (BLE_LL_PREAMBLE_LEN + BLE_LL_ACC_ADDR_LEN + BLE_LL_CRC_LEN +
                    BLE_LL_PDU_HDR_LEN) << 3,
    [BLE_PHY_MODE_2M] =
            (BLE_LL_PREAMBLE_LEN * 2 + BLE_LL_ACC_ADDR_LEN + BLE_LL_CRC_LEN +
                    BLE_LL_PDU_HDR_LEN) << 2,
    /* For Coded PHY we have exact TX times provided by specification:
     * - Preamble, Access Address, CI, TERM1 (always coded as S=8)
     * - PDU, CRC, TERM2 (coded as S=2 or S=8)
     * (Vol 6, Part B, 2.2).
     */
    [BLE_PHY_MODE_CODED_125KBPS] =
            (80 + 256 + 16 + 24 + 8 * (BLE_LL_PDU_HDR_LEN * 8 + 24 + 3)),
    [BLE_PHY_MODE_CODED_500KBPS] =
            (80 + 256 + 16 + 24 + 2 * (BLE_LL_PDU_HDR_LEN * 8 + 24 + 3)),
};

/**
 * Counts the number of advertising PDU's received, by type. For advertising
 * PDU's that contain a destination address, we still count these packets even
 * if they are not for us.
 *
 * @param pdu_type
 */
static void
ble_ll_count_rx_adv_pdus(uint8_t pdu_type)
{
    /* Count received packet types  */
    switch (pdu_type) {
    case BLE_ADV_PDU_TYPE_ADV_EXT_IND:
        STATS_INC(ble_ll_stats, rx_adv_ext_ind);
        break;
    case BLE_ADV_PDU_TYPE_ADV_IND:
        STATS_INC(ble_ll_stats, rx_adv_ind);
        break;
    case BLE_ADV_PDU_TYPE_ADV_DIRECT_IND:
        STATS_INC(ble_ll_stats, rx_adv_direct_ind);
        break;
    case BLE_ADV_PDU_TYPE_ADV_NONCONN_IND:
        STATS_INC(ble_ll_stats, rx_adv_nonconn_ind);
        break;
    case BLE_ADV_PDU_TYPE_SCAN_REQ:
        STATS_INC(ble_ll_stats, rx_scan_reqs);
        break;
    case BLE_ADV_PDU_TYPE_SCAN_RSP:
        STATS_INC(ble_ll_stats, rx_scan_rsps);
        break;
    case BLE_ADV_PDU_TYPE_CONNECT_IND:
        STATS_INC(ble_ll_stats, rx_connect_reqs);
        break;
    case BLE_ADV_PDU_TYPE_AUX_CONNECT_RSP:
        STATS_INC(ble_ll_stats, rx_aux_connect_rsp);
        break;
    case BLE_ADV_PDU_TYPE_ADV_SCAN_IND:
        STATS_INC(ble_ll_stats, rx_scan_ind);
        break;
    default:
        break;
    }
}

struct os_mbuf *
ble_ll_rxpdu_alloc(uint16_t len)
{
    struct os_mbuf *om_ret;
    struct os_mbuf *om_next;
    struct os_mbuf *om;
    struct os_mbuf_pkthdr *pkthdr;
    uint16_t databuf_len;
    int rem_len;

    /*
     * Make sure that data in mbuf are word-aligned with and without packet
     * header. This is essential for proper and quick copying of received PDUs
     * into mbufs.
     */
    _Static_assert((offsetof(struct os_mbuf, om_data) & 3) == 0,
                   "Unaligned om_data");
    _Static_assert(((offsetof(struct os_mbuf, om_data) +
                     sizeof(struct os_mbuf_pkthdr) +
                     sizeof(struct ble_mbuf_hdr)) & 3) == 0,
                   "Unaligned data trailing packet header");

    om_ret = os_msys_get_pkthdr(len, sizeof(struct ble_mbuf_hdr));
    if (!om_ret) {
        goto rxpdu_alloc_fail;
    }

    /* Set complete PDU length in packet header */
    pkthdr = OS_MBUF_PKTHDR(om_ret);
    pkthdr->omp_len = len;

    rem_len = len;

    /*
     * Calculate length of data in memory block. We assume length is rounded
     * down to word size so PHY can do word-size aligned data copy to mbufs
     * (except for last one) and leave remainder unused.
     *
     * Note that there likely won't be any remainder here since all pools have
     * block size aligned to word size anyway.
     */
    databuf_len = om_ret->om_omp->omp_databuf_len & ~3;

    /*
     * First mbuf can store less data due to packet header. Also we reserve one
     * word for leading space to prepend header when necessary (like for data
     * PDU before handing over to HCI)
     */
    om_ret->om_data += 4;
    rem_len -= databuf_len - om_ret->om_pkthdr_len - 4;

    /* Allocate and chain mbufs until there's enough space to store complete PDU */
    om = om_ret;
    while (rem_len > 0) {
        om_next = os_msys_get(rem_len, 0);
        if (!om_next) {
            os_mbuf_free_chain(om_ret);
            goto rxpdu_alloc_fail;
        }

        SLIST_NEXT(om, om_next) = om_next;
        om = om_next;

        rem_len -= databuf_len;
    }

    return om_ret;

rxpdu_alloc_fail:
    STATS_INC(ble_ll_stats, no_bufs);
    return NULL;
}

int
ble_ll_chk_txrx_octets(uint16_t octets)
{
    int rc;

    if ((octets < BLE_LL_CONN_SUPP_BYTES_MIN) ||
        (octets > BLE_LL_CONN_SUPP_BYTES_MAX)) {
        rc = 0;
    } else {
        rc = 1;
    }

    return rc;
}

int
ble_ll_chk_txrx_time(uint16_t time)
{
    int rc;

    if ((time < BLE_LL_CONN_SUPP_TIME_MIN) ||
        (time > BLE_LL_CONN_SUPP_TIME_MAX)) {
        rc = 0;
    } else {
        rc = 1;
    }

    return rc;
}

/**
 * Checks to see if the address is a resolvable private address.
 *
 * NOTE: the addr_type parameter will be 0 if the address is public;
 * any other value is random (all non-zero values).
 *
 * @param addr
 * @param addr_type Public (zero) or Random (non-zero) address
 *
 * @return int
 */
int
ble_ll_is_rpa(const uint8_t *addr, uint8_t addr_type)
{
    int rc;

    if (addr_type && ((addr[5] & 0xc0) == 0x40)) {
        rc = 1;
    } else {
        rc = 0;
    }
    return rc;
}

int
ble_ll_addr_is_id(uint8_t *addr, uint8_t addr_type)
{
    return !addr_type || ((addr[5] & 0xc0) == 0xc0);
}

int
ble_ll_addr_subtype(const uint8_t *addr, uint8_t addr_type)
{
    if (!addr_type) {
        return BLE_LL_ADDR_SUBTYPE_IDENTITY;
    }

    switch (addr[5] >> 6) {
    case 0:
        return BLE_LL_ADDR_SUBTYPE_NRPA; /* NRPA */
    case 1:
        return BLE_LL_ADDR_SUBTYPE_RPA; /* RPA */
    default:
        return BLE_LL_ADDR_SUBTYPE_IDENTITY; /* static random */
    }
}

int
ble_ll_is_valid_public_addr(const uint8_t *addr)
{
    int i;

    for (i = 0; i < BLE_DEV_ADDR_LEN; ++i) {
        if (addr[i]) {
            return 1;
        }
    }

    return 0;
}

/* Checks to see that the device is a valid random address */
int
ble_ll_is_valid_random_addr(const uint8_t *addr)
{
    int i;
    int rc;
    uint16_t sum;
    uint8_t addr_type;

    /* Make sure all bits are neither one nor zero */
    sum = 0;
    for (i = 0; i < (BLE_DEV_ADDR_LEN -1); ++i) {
        sum += addr[i];
    }
    sum += addr[5] & 0x3f;

    if ((sum == 0) || (sum == ((5*255) + 0x3f))) {
        return 0;
    }

    /* Get the upper two bits of the address */
    rc = 1;
    addr_type = addr[5] & 0xc0;
    if (addr_type == 0xc0) {
        /* Static random address. No other checks needed */
    } else if (addr_type == 0x40) {
        /* Resolvable */
        sum = addr[3] + addr[4] + (addr[5] & 0x3f);
        if ((sum == 0) || (sum == (255 + 255 + 0x3f))) {
            rc = 0;
        }
    } else if (addr_type == 0) {
        /* non-resolvable. Cant be equal to public */
        if (!memcmp(g_dev_addr, addr, BLE_DEV_ADDR_LEN)) {
            rc = 0;
        }
    } else {
        /* Invalid upper two bits */
        rc = 0;
    }

    return rc;
}
int
ble_ll_is_valid_own_addr_type(uint8_t own_addr_type, const uint8_t *random_addr)
{
    int rc;

    switch (own_addr_type) {
    case BLE_HCI_ADV_OWN_ADDR_PUBLIC:
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
    case BLE_HCI_ADV_OWN_ADDR_PRIV_PUB:
#endif
        rc = ble_ll_is_valid_public_addr(g_dev_addr);
        break;
    case BLE_HCI_ADV_OWN_ADDR_RANDOM:
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
    case BLE_HCI_ADV_OWN_ADDR_PRIV_RAND:
#endif
        rc = ble_ll_is_valid_random_addr(random_addr);
        break;
    default:
        rc = 0;
        break;
    }

    return rc;
}

int
ble_ll_set_public_addr(const uint8_t *addr)
{
    memcpy(g_dev_addr, addr, BLE_DEV_ADDR_LEN);

    return BLE_ERR_SUCCESS;
}

/**
 * Called from the HCI command parser when the set random address command
 * is received.
 *
 * Context: Link Layer task (HCI command parser)
 *
 * @param addr Pointer to address
 *
 * @return int 0: success
 */
int
ble_ll_set_random_addr(const uint8_t *cmdbuf, uint8_t len, bool hci_adv_ext)
{
    const struct ble_hci_le_set_rand_addr_cp *cmd = (const void *) cmdbuf;

    if (len < sizeof(*cmd)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    /* If the Host issues this command when scanning or legacy advertising is
     * enabled, the Controller shall return the error code Command Disallowed.
     *
     * Test specification extends this also to initiating.
     */

    if (g_ble_ll_conn_create_sm || ble_ll_scan_enabled() ||
                                (!hci_adv_ext && ble_ll_adv_enabled())) {
        return BLE_ERR_CMD_DISALLOWED;
    }

    if (!ble_ll_is_valid_random_addr(cmd->addr)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    memcpy(g_random_addr, cmd->addr, BLE_DEV_ADDR_LEN);

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    /* For instance 0 we need same address if legacy advertising might be
     * used. If extended advertising is in use than this command doesn't
     * affect instance 0.
     */
    if (!hci_adv_ext)
        ble_ll_adv_set_random_addr(cmd->addr, 0);
#endif

    return BLE_ERR_SUCCESS;
}

/**
 * Checks to see if an address is our device address (either public or
 * random)
 *
 * @param addr
 * @param addr_type
 *
 * @return int 0: not our device address. 1: is our device address
 */
int
ble_ll_is_our_devaddr(uint8_t *addr, int addr_type)
{
    int rc;
    uint8_t *our_addr;

    if (addr_type) {
        our_addr = g_random_addr;
    } else {
        our_addr = g_dev_addr;
    }

    rc = 0;
    if (!memcmp(our_addr, addr, BLE_DEV_ADDR_LEN)) {
        rc = 1;
    }

    return rc;
}

/**
 * Get identity address
 *
 * @param addr_type Random (1). Public(0)
 *
 * @return pointer to identity address of given type.
 */
uint8_t*
ble_ll_get_our_devaddr(uint8_t addr_type)
{
    if (addr_type) {
        return g_random_addr;
    }

    return g_dev_addr;
}

/**
 * Wait for response timeout function
 *
 * Context: interrupt (ble scheduler)
 *
 * @param arg
 */
void
ble_ll_wfr_timer_exp(void *arg)
{
    int rx_start;
    uint8_t lls;

    rx_start = ble_phy_rx_started();
    lls = g_ble_ll_data.ll_state;

    ble_ll_trace_u32x3(BLE_LL_TRACE_ID_WFR_EXP, lls, ble_phy_xcvr_state_get(),
                       (uint32_t)rx_start);

    /* If we have started a reception, there is nothing to do here */
    if (!rx_start) {
        switch (lls) {
        case BLE_LL_STATE_ADV:
            ble_ll_adv_wfr_timer_exp();
            break;
        case BLE_LL_STATE_CONNECTION:
            ble_ll_conn_wfr_timer_exp();
            break;
        case BLE_LL_STATE_SCANNING:
            ble_ll_scan_wfr_timer_exp();
            break;
        case BLE_LL_STATE_INITIATING:
            ble_ll_conn_init_wfr_timer_exp();
            break;
#if MYNEWT_VAL(BLE_LL_DTM)
        case BLE_LL_STATE_DTM:
            ble_ll_dtm_wfr_timer_exp();
            break;
#endif
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV)
        case BLE_LL_STATE_SYNC:
            ble_ll_sync_wfr_timer_exp();
            break;
#endif
        default:
            break;
        }
    }
}

/**
 * ll tx pkt in proc
 *
 * Process ACL data packet input from host
 *
 * Context: Link layer task
 *
 */
static void
ble_ll_tx_pkt_in(void)
{
    uint16_t handle;
    uint16_t length;
    uint16_t pb;
    struct os_mbuf_pkthdr *pkthdr;
    struct os_mbuf *om;
    os_sr_t sr;

    /* Drain all packets off the queue */
    while (STAILQ_FIRST(&g_ble_ll_data.ll_tx_pkt_q)) {
        /* Get mbuf pointer from packet header pointer */
        pkthdr = STAILQ_FIRST(&g_ble_ll_data.ll_tx_pkt_q);
        om = (struct os_mbuf *)((uint8_t *)pkthdr - sizeof(struct os_mbuf));

        /* Remove from queue */
        OS_ENTER_CRITICAL(sr);
        STAILQ_REMOVE_HEAD(&g_ble_ll_data.ll_tx_pkt_q, omp_next);
        OS_EXIT_CRITICAL(sr);

        /* Strip HCI ACL header to get handle and length */
        handle = get_le16(om->om_data);
        length = get_le16(om->om_data + 2);
        os_mbuf_adj(om, sizeof(struct hci_data_hdr));

        /* Do some basic error checking */
        pb = handle & 0x3000;
        if ((pkthdr->omp_len != length) || (pb > 0x1000) || (length == 0)) {
            /* This is a bad ACL packet. Count a stat and free it */
            STATS_INC(ble_ll_stats, bad_acl_hdr);
            os_mbuf_free_chain(om);
            continue;
        }

        /* Hand to connection state machine */
        ble_ll_conn_tx_pkt_in(om, handle, length);
    }
}

/**
 * Count Link Layer statistics for received PDUs
 *
 * Context: Link layer task
 *
 * @param hdr
 * @param len
 */
static void
ble_ll_count_rx_stats(struct ble_mbuf_hdr *hdr, uint16_t len, uint8_t pdu_type)
{
    uint8_t crcok;
    bool connection_data;

    crcok = BLE_MBUF_HDR_CRC_OK(hdr);
    connection_data = (BLE_MBUF_HDR_RX_STATE(hdr) == BLE_LL_STATE_CONNECTION);

#if MYNEWT_VAL(BLE_LL_DTM)
    /* Reuse connection stats for DTM */
    if (!connection_data) {
        connection_data = (BLE_MBUF_HDR_RX_STATE(hdr) == BLE_LL_STATE_DTM);
    }
#endif

    if (crcok) {
        if (connection_data) {
            STATS_INC(ble_ll_stats, rx_data_pdu_crc_ok);
            STATS_INCN(ble_ll_stats, rx_data_bytes_crc_ok, len);
        } else {
            STATS_INC(ble_ll_stats, rx_adv_pdu_crc_ok);
            STATS_INCN(ble_ll_stats, rx_adv_bytes_crc_ok, len);
            ble_ll_count_rx_adv_pdus(pdu_type);
        }
    } else {
        if (connection_data) {
            STATS_INC(ble_ll_stats, rx_data_pdu_crc_err);
            STATS_INCN(ble_ll_stats, rx_data_bytes_crc_err, len);
        } else {
            STATS_INC(ble_ll_stats, rx_adv_pdu_crc_err);
            STATS_INCN(ble_ll_stats, rx_adv_bytes_crc_err, len);
        }
    }
}

/**
 * ll rx pkt in
 *
 * Process received packet from PHY.
 *
 * Context: Link layer task
 *
 */
static void
ble_ll_rx_pkt_in(void)
{
    os_sr_t sr;
    uint8_t pdu_type;
    uint8_t *rxbuf;
    struct os_mbuf_pkthdr *pkthdr;
    struct ble_mbuf_hdr *ble_hdr;
    struct os_mbuf *m;

    /* Drain all packets off the queue */
    while (STAILQ_FIRST(&g_ble_ll_data.ll_rx_pkt_q)) {
        /* Get mbuf pointer from packet header pointer */
        pkthdr = STAILQ_FIRST(&g_ble_ll_data.ll_rx_pkt_q);
        m = (struct os_mbuf *)((uint8_t *)pkthdr - sizeof(struct os_mbuf));

        /* Remove from queue */
        OS_ENTER_CRITICAL(sr);
        STAILQ_REMOVE_HEAD(&g_ble_ll_data.ll_rx_pkt_q, omp_next);
        OS_EXIT_CRITICAL(sr);

        /* Note: pdu type wont get used unless this is an advertising pdu */
        ble_hdr = BLE_MBUF_HDR_PTR(m);
        rxbuf = m->om_data;
        pdu_type = rxbuf[0] & BLE_ADV_PDU_HDR_TYPE_MASK;
        ble_ll_count_rx_stats(ble_hdr, pkthdr->omp_len, pdu_type);

        /* Process the data or advertising pdu */
        /* Process the PDU */
        switch (BLE_MBUF_HDR_RX_STATE(ble_hdr)) {
        case BLE_LL_STATE_CONNECTION:
            ble_ll_conn_rx_data_pdu(m, ble_hdr);
            /* m is going to be free by function above */
            m = NULL;
            break;
        case BLE_LL_STATE_ADV:
            ble_ll_adv_rx_pkt_in(pdu_type, rxbuf, ble_hdr);
            break;
        case BLE_LL_STATE_SCANNING:
            ble_ll_scan_rx_pkt_in(pdu_type, m, ble_hdr);
            break;
        case BLE_LL_STATE_INITIATING:
            ble_ll_init_rx_pkt_in(pdu_type, rxbuf, ble_hdr);
            break;
#if MYNEWT_VAL(BLE_LL_DTM)
        case BLE_LL_STATE_DTM:
            ble_ll_dtm_rx_pkt_in(m, ble_hdr);
            break;
#endif
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV)
        case BLE_LL_STATE_SYNC:
            ble_ll_sync_rx_pkt_in(m, ble_hdr);
            break;
#endif
        default:
            /* Any other state should never occur */
            STATS_INC(ble_ll_stats, bad_ll_state);
            break;
        }
        if (m) {
            /* Free the packet buffer */
            os_mbuf_free_chain(m);
        }
    }
}

/**
 * Called to put a packet on the Link Layer receive packet queue.
 *
 * @param rxpdu Pointer to received PDU
 */
void
ble_ll_rx_pdu_in(struct os_mbuf *rxpdu)
{
    struct os_mbuf_pkthdr *pkthdr;

    pkthdr = OS_MBUF_PKTHDR(rxpdu);
    STAILQ_INSERT_TAIL(&g_ble_ll_data.ll_rx_pkt_q, pkthdr, omp_next);
    ble_npl_eventq_put(&g_ble_ll_data.ll_evq, &g_ble_ll_data.ll_rx_pkt_ev);
}

/**
 * Called to put a packet on the Link Layer transmit packet queue.
 *
 * @param txpdu Pointer to transmit packet
 */
void
ble_ll_acl_data_in(struct os_mbuf *txpkt)
{
    os_sr_t sr;
    struct os_mbuf_pkthdr *pkthdr;

    pkthdr = OS_MBUF_PKTHDR(txpkt);
    OS_ENTER_CRITICAL(sr);
    STAILQ_INSERT_TAIL(&g_ble_ll_data.ll_tx_pkt_q, pkthdr, omp_next);
    OS_EXIT_CRITICAL(sr);
    ble_npl_eventq_put(&g_ble_ll_data.ll_evq, &g_ble_ll_data.ll_tx_pkt_ev);
}

/**
 * Called to post event to Link Layer when a data buffer overflow has
 * occurred.
 *
 * Context: Interrupt
 *
 */
void
ble_ll_data_buffer_overflow(void)
{
    ble_npl_eventq_put(&g_ble_ll_data.ll_evq, &g_ble_ll_data.ll_dbuf_overflow_ev);
}

/**
 * Called when a HW error occurs.
 *
 * Context: Interrupt
 */
void
ble_ll_hw_error(void)
{
    ble_npl_callout_reset(&g_ble_ll_data.ll_hw_err_timer, 0);
}

/**
 * Called when the HW error timer expires.
 *
 * @param arg
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
static void
ble_ll_hw_err_timer_cb(struct ble_npl_event *ev)
{
    if (ble_ll_hci_ev_hw_err(BLE_HW_ERR_HCI_SYNC_LOSS)) {
        /*
         * Restart callout if failed to allocate event. Try to allocate an
         * event every 50 milliseconds (or each OS tick if a tick is longer
         * than 100 msecs).
         */
        ble_npl_callout_reset(&g_ble_ll_data.ll_hw_err_timer,
                         ble_npl_time_ms_to_ticks32(50));
    }
}
#pragma GCC diagnostic pop

/**
 * Called upon start of received PDU
 *
 * Context: Interrupt
 *
 * @param rxpdu
 *        chan
 *
 * @return int
 *   < 0: A frame we dont want to receive.
 *   = 0: Continue to receive frame. Dont go from rx to tx
 *   > 0: Continue to receive frame and go from rx to tx when done
 */
int
ble_ll_rx_start(uint8_t *rxbuf, uint8_t chan, struct ble_mbuf_hdr *rxhdr)
{
    int rc;
    uint8_t pdu_type;

    /* Advertising channel PDU */
    pdu_type = rxbuf[0] & BLE_ADV_PDU_HDR_TYPE_MASK;

    ble_ll_trace_u32x2(BLE_LL_TRACE_ID_RX_START, g_ble_ll_data.ll_state,
                       pdu_type);

    switch (g_ble_ll_data.ll_state) {
    case BLE_LL_STATE_CONNECTION:
        rc = ble_ll_conn_rx_isr_start(rxhdr, ble_phy_access_addr_get());
        break;
    case BLE_LL_STATE_ADV:
        rc = ble_ll_adv_rx_isr_start(pdu_type);
        break;
    case BLE_LL_STATE_INITIATING:
        rc = ble_ll_init_rx_isr_start(pdu_type, rxhdr);
        break;
    case BLE_LL_STATE_SCANNING:
        rc = ble_ll_scan_rx_isr_start(pdu_type, &rxhdr->rxinfo.flags);
        break;
#if MYNEWT_VAL(BLE_LL_DTM)
    case BLE_LL_STATE_DTM:
        rc = ble_ll_dtm_rx_isr_start(rxhdr, ble_phy_access_addr_get());
        break;
#endif
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV)
    case BLE_LL_STATE_SYNC:
        rc = ble_ll_sync_rx_isr_start(pdu_type, rxhdr);
        break;
#endif
    default:
        /* Should not be in this state! */
        rc = -1;
        STATS_INC(ble_ll_stats, bad_ll_state);
        break;
    }

    return rc;
}

/**
 * Called by the PHY when a receive packet has ended.
 *
 * NOTE: Called from interrupt context!
 *
 * @param rxbuf Pointer to received PDU data
 *        rxhdr Pointer to BLE header of received mbuf
 *
 * @return int
 *       < 0: Disable the phy after reception.
 *      == 0: Success. Do not disable the PHY.
 *       > 0: Do not disable PHY as that has already been done.
 */
int
ble_ll_rx_end(uint8_t *rxbuf, struct ble_mbuf_hdr *rxhdr)
{
    int rc;
    int badpkt;
    uint8_t pdu_type;
    uint8_t len;
    uint8_t crcok;
    struct os_mbuf *rxpdu;

    /* Get CRC status from BLE header */
    crcok = BLE_MBUF_HDR_CRC_OK(rxhdr);

    /* Get advertising PDU type and length */
    pdu_type = rxbuf[0] & BLE_ADV_PDU_HDR_TYPE_MASK;
    len = rxbuf[1];

    ble_ll_trace_u32x3(BLE_LL_TRACE_ID_RX_END, pdu_type, len,
                       rxhdr->rxinfo.flags);

#if MYNEWT_VAL(BLE_LL_DTM)
    if (BLE_MBUF_HDR_RX_STATE(rxhdr) == BLE_LL_STATE_DTM) {
        rc = ble_ll_dtm_rx_isr_end(rxbuf, rxhdr);
        return rc;
    }
#endif

    if (BLE_MBUF_HDR_RX_STATE(rxhdr) == BLE_LL_STATE_CONNECTION) {
        rc = ble_ll_conn_rx_isr_end(rxbuf, rxhdr);
        return rc;
    }

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV)
    if (BLE_MBUF_HDR_RX_STATE(rxhdr) == BLE_LL_STATE_SYNC) {
        rc = ble_ll_sync_rx_isr_end(rxbuf, rxhdr);
        return rc;
    }
#endif

    /* If the CRC checks, make sure lengths check! */
    badpkt = 0;
    if (crcok) {
        switch (pdu_type) {
        case BLE_ADV_PDU_TYPE_SCAN_REQ:
        case BLE_ADV_PDU_TYPE_ADV_DIRECT_IND:
            if (len != BLE_SCAN_REQ_LEN) {
                badpkt = 1;
            }
            break;
        case BLE_ADV_PDU_TYPE_SCAN_RSP:
        case BLE_ADV_PDU_TYPE_ADV_IND:
        case BLE_ADV_PDU_TYPE_ADV_SCAN_IND:
        case BLE_ADV_PDU_TYPE_ADV_NONCONN_IND:
            if ((len < BLE_DEV_ADDR_LEN) || (len > BLE_ADV_SCAN_IND_MAX_LEN)) {
                badpkt = 1;
            }
            break;
        case BLE_ADV_PDU_TYPE_AUX_CONNECT_RSP:
            break;
        case BLE_ADV_PDU_TYPE_ADV_EXT_IND:
            break;
        case BLE_ADV_PDU_TYPE_CONNECT_IND:
            if (len != BLE_CONNECT_REQ_LEN) {
                badpkt = 1;
            }
            break;
        default:
            badpkt = 1;
            break;
        }

        /* If this is a malformed packet, just kill it here */
        if (badpkt) {
            STATS_INC(ble_ll_stats, rx_adv_malformed_pkts);
        }
    }

    /* Hand packet to the appropriate state machine (if crc ok) */
    rxpdu = NULL;
    switch (BLE_MBUF_HDR_RX_STATE(rxhdr)) {
    case BLE_LL_STATE_ADV:
        if (!badpkt) {
            rxpdu = ble_ll_rxpdu_alloc(len + BLE_LL_PDU_HDR_LEN);
            if (rxpdu) {
                ble_phy_rxpdu_copy(rxbuf, rxpdu);
            }
        }
        rc = ble_ll_adv_rx_isr_end(pdu_type, rxpdu, crcok);
        break;
    case BLE_LL_STATE_SCANNING:
        if (!badpkt) {
            rxpdu = ble_ll_rxpdu_alloc(len + BLE_LL_PDU_HDR_LEN);
            if (rxpdu) {
                ble_phy_rxpdu_copy(rxbuf, rxpdu);
            }
        }
        rc = ble_ll_scan_rx_isr_end(rxpdu, crcok);
        break;
    case BLE_LL_STATE_INITIATING:
        rc = ble_ll_init_rx_isr_end(rxbuf, crcok, rxhdr);
        break;
    default:
        rc = -1;
        STATS_INC(ble_ll_stats, bad_ll_state);
        break;
    }

    /* Hand packet up to higher layer (regardless of CRC failure) */
    if (rxpdu) {
        ble_ll_rx_pdu_in(rxpdu);
    }

    return rc;
}

uint8_t
ble_ll_tx_mbuf_pducb(uint8_t *dptr, void *pducb_arg, uint8_t *hdr_byte)
{
    struct os_mbuf *txpdu;
    struct ble_mbuf_hdr *ble_hdr;

    txpdu = pducb_arg;
    BLE_LL_ASSERT(txpdu);
    ble_hdr = BLE_MBUF_HDR_PTR(txpdu);

    os_mbuf_copydata(txpdu, ble_hdr->txinfo.offset, ble_hdr->txinfo.pyld_len,
                     dptr);

    *hdr_byte = ble_hdr->txinfo.hdr_byte;

    return ble_hdr->txinfo.pyld_len;
}

uint8_t
ble_ll_tx_flat_mbuf_pducb(uint8_t *dptr, void *pducb_arg, uint8_t *hdr_byte)
{
    struct os_mbuf *txpdu;
    struct ble_mbuf_hdr *ble_hdr;

    txpdu = pducb_arg;
    BLE_LL_ASSERT(txpdu);
    ble_hdr = BLE_MBUF_HDR_PTR(txpdu);

    memcpy(dptr, txpdu->om_data, ble_hdr->txinfo.pyld_len);

    *hdr_byte = ble_hdr->txinfo.hdr_byte;

    return ble_hdr->txinfo.pyld_len;
}

static void
ble_ll_event_rx_pkt(struct ble_npl_event *ev)
{
    ble_ll_rx_pkt_in();
}

static void
ble_ll_event_tx_pkt(struct ble_npl_event *ev)
{
    ble_ll_tx_pkt_in();
}

static void
ble_ll_event_dbuf_overflow(struct ble_npl_event *ev)
{
    ble_ll_hci_ev_databuf_overflow();
}

static void
ble_ll_event_comp_pkts(struct ble_npl_event *ev)
{
    ble_ll_conn_num_comp_pkts_event_send(NULL);
}

/**
 * Link Layer task.
 *
 * This is the task that runs the Link Layer.
 *
 * @param arg
 */
void
ble_ll_task(void *arg)
{
    struct ble_npl_event *ev;

    /* Init ble phy */
    ble_phy_init();

    /* Set output power to 1mW (0 dBm) */
    ble_phy_txpwr_set(MYNEWT_VAL(BLE_LL_TX_PWR_DBM));

    /* Register callback for transport */
    ble_hci_trans_cfg_ll(ble_ll_hci_cmd_rx, NULL, ble_ll_hci_acl_rx, NULL);

    /* Tell the host that we are ready to receive packets */
    ble_ll_hci_send_noop();

    while (1) {
        ev = ble_npl_eventq_get(&g_ble_ll_data.ll_evq, BLE_NPL_TIME_FOREVER);
        assert(ev);
        ble_npl_event_run(ev);
    }
}

/**
 * ble ll state set
 *
 * Called to set the current link layer state.
 *
 * Context: Interrupt and Link Layer task
 *
 * @param ll_state
 */
void
ble_ll_state_set(uint8_t ll_state)
{
    g_ble_ll_data.ll_state = ll_state;
}

/**
 * ble ll state get
 *
 * Called to get the current link layer state.
 *
 * Context: Link Layer task (can be called from interrupt context though).
 *
 * @return ll_state
 */
uint8_t
ble_ll_state_get(void)
{
    return g_ble_ll_data.ll_state;
}

/**
 * ble ll event send
 *
 * Send an event to the Link Layer task
 *
 * @param ev Event to add to the Link Layer event queue.
 */
void
ble_ll_event_send(struct ble_npl_event *ev)
{
    ble_npl_eventq_put(&g_ble_ll_data.ll_evq, ev);
}

/**
 * Returns the features supported by the link layer
 *
 * @return uint8_t bitmask of supported features.
 */
uint64_t
ble_ll_read_supp_states(void)
{
    return BLE_LL_SUPPORTED_STATES;
}

/**
 * Returns the features supported by the link layer
 *
 * @return uint64_t bitmask of supported features.
 */
uint64_t
ble_ll_read_supp_features(void)
{
    return g_ble_ll_data.ll_supp_features;
}

/**
 * Sets the features controlled by the host.
 *
 * @return HCI command status
 */
int
ble_ll_set_host_feat(const uint8_t *cmdbuf, uint8_t len)
{
    const struct ble_hci_le_set_host_feat_cp *cmd = (const void *) cmdbuf;
    uint64_t mask;

    if (len != sizeof(*cmd)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    if (!SLIST_EMPTY(&g_ble_ll_conn_active_list)) {
        return BLE_ERR_CMD_DISALLOWED;
    }

    if ((cmd->bit_num > 0x3F) || (cmd->val > 1)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    mask = (uint64_t)1 << (cmd->bit_num);
    if (!(mask & BLE_LL_HOST_CONTROLLED_FEATURES)) {
        return BLE_ERR_UNSUPPORTED;
    }

    if (cmd->val == 0) {
        g_ble_ll_data.ll_supp_features &= ~(mask);
    } else {
        g_ble_ll_data.ll_supp_features |= mask;
    }

    return BLE_ERR_SUCCESS;
}
/**
 * Flush a link layer packet queue.
 *
 * @param pktq
 */
static void
ble_ll_flush_pkt_queue(struct ble_ll_pkt_q *pktq)
{
    struct os_mbuf_pkthdr *pkthdr;
    struct os_mbuf *om;

    /* FLush all packets from Link layer queues */
    while (STAILQ_FIRST(pktq)) {
        /* Get mbuf pointer from packet header pointer */
        pkthdr = STAILQ_FIRST(pktq);
        om = OS_MBUF_PKTHDR_TO_MBUF(pkthdr);

        /* Remove from queue and free the mbuf */
        STAILQ_REMOVE_HEAD(pktq, omp_next);
        os_mbuf_free_chain(om);
    }
}

/**
 * Called to initialize a mbuf used by the controller
 *
 * NOTE: this is only used when the mbuf is created by the controller;
 * it should not be used for data packets (ACL data packets) that come from
 * the host. This routine assumes that the entire pdu length can fit in
 * one mbuf contiguously.
 *
 * @param m
 * @param pdulen
 * @param hdr
 */
void
ble_ll_mbuf_init(struct os_mbuf *m, uint8_t pdulen, uint8_t hdr)
{
    struct ble_mbuf_hdr *ble_hdr;

    /* Set mbuf length and packet length */
    m->om_len = pdulen;
    OS_MBUF_PKTHDR(m)->omp_len = pdulen;

    /* Set BLE transmit header */
    ble_hdr = BLE_MBUF_HDR_PTR(m);
    ble_hdr->txinfo.flags = 0;
    ble_hdr->txinfo.offset = 0;
    ble_hdr->txinfo.pyld_len = pdulen;
    ble_hdr->txinfo.hdr_byte = hdr;
}

static void
ble_ll_validate_task(void)
{
#ifdef MYNEWT
#ifndef NDEBUG
    struct os_task_info oti;

    os_task_info_get(&g_ble_ll_task, &oti);

    BLE_LL_ASSERT(oti.oti_stkusage < oti.oti_stksize);
#endif
#endif
}

/**
 * Called to reset the controller. This performs a "software reset" of the link
 * layer; it does not perform a HW reset of the controller nor does it reset
 * the HCI interface.
 *
 * Context: Link Layer task (HCI command)
 *
 * @return int The ble error code to place in the command complete event that
 * is returned when this command is issued.
 */
int
ble_ll_reset(void)
{
    int rc;
    os_sr_t sr;

    /* do sanity check on LL task stack */
    ble_ll_validate_task();

    OS_ENTER_CRITICAL(sr);
    ble_phy_disable();
    ble_ll_sched_stop();
    ble_ll_scan_reset();
    ble_ll_rfmgmt_reset();
    OS_EXIT_CRITICAL(sr);

    /* Stop any advertising */
    ble_ll_adv_reset();

#if MYNEWT_VAL(BLE_LL_DTM)
    ble_ll_dtm_reset();
#endif

    /* Stop sync */
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV)
    ble_ll_sync_reset();
#endif

    /* FLush all packets from Link layer queues */
    ble_ll_flush_pkt_queue(&g_ble_ll_data.ll_tx_pkt_q);
    ble_ll_flush_pkt_queue(&g_ble_ll_data.ll_rx_pkt_q);

    /* Reset LL stats */
    STATS_RESET(ble_ll_stats);

    /* Reset any preferred PHYs */
    g_ble_ll_data.ll_pref_tx_phys = 0;
    g_ble_ll_data.ll_pref_rx_phys = 0;

    /* Reset connection module */
    ble_ll_conn_module_reset();

    /* All this does is re-initialize the event masks so call the hci init */
    ble_ll_hci_init();

    /* Reset scheduler */
    ble_ll_sched_init();

    /* Set state to standby */
    ble_ll_state_set(BLE_LL_STATE_STANDBY);

    /* Reset our random address */
    memset(g_random_addr, 0, BLE_DEV_ADDR_LEN);

    /* Clear the whitelist */
    ble_ll_whitelist_clear();

    /* Reset resolving list */
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
    ble_ll_resolv_list_reset();
#endif

    /* Re-initialize the PHY */
    rc = ble_phy_init();

    return rc;
}

uint32_t
ble_ll_pdu_tx_time_get(uint16_t payload_len, int phy_mode)
{
    uint32_t usecs;

#if (BLE_LL_BT5_PHY_SUPPORTED)
    if (phy_mode == BLE_PHY_MODE_1M) {
        /* 8 usecs per byte */
        usecs = payload_len << 3;
    } else if (phy_mode == BLE_PHY_MODE_2M) {
        /* 4 usecs per byte */
        usecs = payload_len << 2;
    } else if (phy_mode == BLE_PHY_MODE_CODED_125KBPS) {
        /* S=8 => 8 * 8 = 64 usecs per byte */
        usecs = payload_len << 6;
    } else if (phy_mode == BLE_PHY_MODE_CODED_500KBPS) {
        /* S=2 => 2 * 8 = 16 usecs per byte */
        usecs = payload_len << 4;
    } else {
        BLE_LL_ASSERT(0);
    }

    usecs += g_ble_ll_pdu_header_tx_time[phy_mode];
#else
    usecs = (((payload_len) + BLE_LL_PDU_HDR_LEN + BLE_LL_ACC_ADDR_LEN
            + BLE_LL_PREAMBLE_LEN + BLE_LL_CRC_LEN) << 3);
#endif

    return usecs;
}

uint16_t
ble_ll_pdu_max_tx_octets_get(uint32_t usecs, int phy_mode)
{
    uint32_t header_tx_time;
    uint16_t octets = 0;

    BLE_LL_ASSERT(phy_mode < BLE_PHY_NUM_MODE);

    header_tx_time = g_ble_ll_pdu_header_tx_time[phy_mode];

    /*
     * Current conn max tx time can be too short to even send a packet header
     * and this can happen if we changed connection form uncoded to coded phy.
     * However, the lower bound for conn max tx time (all of them) depends on
     * current phy (uncoded/coded) but it always allows to send at least 27
     * bytes of payload thus we alwyas return at least 27 from here.
     *
     * Reference:
     * Core v5.0, Vol 6, Part B, section 4.5.10
     * see connEffectiveMaxTxTime and connEffectiveMaxRxTime definitions
     */

    if (usecs < header_tx_time) {
        return 27;
    }

    usecs -= header_tx_time;

    if (phy_mode == BLE_PHY_MODE_1M) {
        /* 8 usecs per byte */
        octets = usecs >> 3;
    } else if (phy_mode == BLE_PHY_MODE_2M) {
        /* 4 usecs per byte */
        octets = usecs >> 2;
    } else if (phy_mode == BLE_PHY_MODE_CODED_125KBPS) {
        /* S=8 => 8 * 8 = 64 usecs per byte */
        octets = usecs >> 6;
    } else if (phy_mode == BLE_PHY_MODE_CODED_500KBPS) {
        /* S=2 => 2 * 8 = 16 usecs per byte */
        octets = usecs >> 4;
    } else {
        BLE_LL_ASSERT(0);
    }

    /* see comment at the beginning */
    return max(27, octets);
}

static inline bool
ble_ll_is_addr_empty(const uint8_t *addr)
{
    return memcmp(addr, BLE_ADDR_ANY, BLE_DEV_ADDR_LEN) == 0;
}

/**
 * Initialize the Link Layer. Should be called only once
 *
 * @return int
 */
void
ble_ll_init(void)
{
    int rc;
    uint64_t features;
    ble_addr_t addr;
    struct ble_ll_obj *lldata;

    /* Ensure this function only gets called by sysinit. */
    SYSINIT_ASSERT_ACTIVE();

    ble_ll_trace_init();
    ble_phy_trace_init();

    /* Set public device address if not already set */
    if (ble_ll_is_addr_empty(g_dev_addr)) {
        /* Use sycfg address if configured, otherwise try to read from HW */
        if (!ble_ll_is_addr_empty(MYNEWT_VAL(BLE_PUBLIC_DEV_ADDR))) {
            memcpy(g_dev_addr, MYNEWT_VAL(BLE_PUBLIC_DEV_ADDR), BLE_DEV_ADDR_LEN);
        } else {
            rc = ble_hw_get_public_addr(&addr);
            if (!rc) {
                memcpy(g_dev_addr, &addr.val[0], BLE_DEV_ADDR_LEN);
            }
        }
    }

    ble_ll_rfmgmt_init();

    /* Get pointer to global data object */
    lldata = &g_ble_ll_data;

    /* Set acl pkt size and number */
    lldata->ll_num_acl_pkts = MYNEWT_VAL(BLE_ACL_BUF_COUNT);
    lldata->ll_acl_pkt_size = MYNEWT_VAL(BLE_ACL_BUF_SIZE);

    /* Initialize eventq */
    ble_npl_eventq_init(&lldata->ll_evq);

    /* Initialize the transmit (from host) and receive (from phy) queues */
    STAILQ_INIT(&lldata->ll_tx_pkt_q);
    STAILQ_INIT(&lldata->ll_rx_pkt_q);

    /* Initialize transmit (from host) and receive packet (from phy) event */
    ble_npl_event_init(&lldata->ll_rx_pkt_ev, ble_ll_event_rx_pkt, NULL);
    ble_npl_event_init(&lldata->ll_tx_pkt_ev, ble_ll_event_tx_pkt, NULL);

    /* Initialize data buffer overflow event and completed packets */
    ble_npl_event_init(&lldata->ll_dbuf_overflow_ev, ble_ll_event_dbuf_overflow, NULL);
    ble_npl_event_init(&lldata->ll_comp_pkt_ev, ble_ll_event_comp_pkts, NULL);

    /* Initialize the HW error timer */
    /* NOT USED WITH RAM HCI
     * Commented out to prevent creating an unnecessary timer
    ble_npl_callout_init(&g_ble_ll_data.ll_hw_err_timer,
                         &g_ble_ll_data.ll_evq,
                         ble_ll_hw_err_timer_cb,
                         NULL);
    */

    /* Initialize LL HCI */
    ble_ll_hci_init();

    /* Init the scheduler */
    ble_ll_sched_init();

    /* Initialize advertiser */
    ble_ll_adv_init();

    /* Initialize a scanner */
    ble_ll_scan_init();

    /* Initialize the connection module */
    ble_ll_conn_module_init();

    /* Set the supported features. NOTE: we always support extended reject. */
    features = BLE_LL_FEAT_EXTENDED_REJ;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_DATA_LEN_EXT)
    features |= BLE_LL_FEAT_DATA_LEN_EXT;
#endif
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_CONN_PARAM_REQ)
    features |= BLE_LL_FEAT_CONN_PARM_REQ;
#endif
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_SLAVE_INIT_FEAT_XCHG)
    features |= BLE_LL_FEAT_SLAVE_INIT;
#endif
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
    features |= BLE_LL_FEAT_LE_ENCRYPTION;
#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
    features |= (BLE_LL_FEAT_LL_PRIVACY | BLE_LL_FEAT_EXT_SCAN_FILT);
    ble_ll_resolv_init();
#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_PING)
    features |= BLE_LL_FEAT_LE_PING;
#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    features |= BLE_LL_FEAT_EXT_ADV;
#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CSA2)
    /* CSA2 */
    features |= BLE_LL_FEAT_CSA2;
#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_2M_PHY)
    features |= BLE_LL_FEAT_LE_2M_PHY;
#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CODED_PHY)
    features |= BLE_LL_FEAT_LE_CODED_PHY;
#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV)
    features |= BLE_LL_FEAT_PERIODIC_ADV;
    ble_ll_sync_init();
#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV_SYNC_TRANSFER)
    features |= BLE_LL_FEAT_SYNC_TRANS_RECV;
    features |= BLE_LL_FEAT_SYNC_TRANS_SEND;
#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_SCA_UPDATE)
    features |= BLE_LL_FEAT_SCA_UPDATE;
#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_ISO)
    features |= BLE_LL_FEAT_CIS_MASTER;
    features |= BLE_LL_FEAT_CIS_SLAVE;
    features |= BLE_LL_FEAT_ISO_BROADCASTER;
    features |= BLE_LL_FEAT_ISO_HOST_SUPPORT;
#endif

    lldata->ll_supp_features = features;

    /* Initialize random number generation */
    ble_ll_rand_init();
    /* Start the random number generator */
    ble_ll_rand_start();

    rc = stats_init_and_reg(STATS_HDR(ble_ll_stats),
                            STATS_SIZE_INIT_PARMS(ble_ll_stats, STATS_SIZE_32),
                            STATS_NAME_INIT_PARMS(ble_ll_stats),
                            "ble_ll");
    SYSINIT_PANIC_ASSERT(rc == 0);

#if MYNEWT_VAL(BLE_LL_DTM)
    ble_ll_dtm_init();
#endif

#if MYNEWT
    /* Initialize the LL task */
    os_task_init(&g_ble_ll_task, "ble_ll", ble_ll_task, NULL,
                 MYNEWT_VAL(BLE_LL_PRIO), OS_WAIT_FOREVER, g_ble_ll_stack,
                 BLE_LL_STACK_SIZE);
#else

/*
 * For non-Mynewt OS it is required that OS creates task for LL and run LL
 * routine which is wrapped by nimble_port_ll_task_func().
 */

#endif
}

#endif
