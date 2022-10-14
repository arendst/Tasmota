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
#include <string.h>
#include <assert.h>
#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#include "nimble/porting/nimble/include/os/os.h"
#include "nimble/porting/nimble/include/os/os_cputime.h"

#if defined(ARDUINO_ARCH_NRF5) && defined(NRF51)
#include "nimble/nimble/drivers/nrf51/include/ble/xcvr.h"
#elif defined(ARDUINO_ARCH_NRF5) && defined(NRF52_SERIES)
#include "nimble/nimble/drivers/nrf52/include/ble/xcvr.h"
#endif

#include "nimble/nimble/include/nimble/ble.h"
#include "nimble/nimble/include/nimble/nimble_opt.h"
#include "nimble/nimble/include/nimble/hci_common.h"
#include "nimble/nimble/include/nimble/ble_hci_trans.h"
#include "../include/controller/ble_phy.h"
#include "../include/controller/ble_hw.h"
#include "../include/controller/ble_ll.h"
#include "../include/controller/ble_ll_hci.h"
#include "../include/controller/ble_ll_adv.h"
#include "../include/controller/ble_ll_sched.h"
#include "../include/controller/ble_ll_scan.h"
#include "../include/controller/ble_ll_whitelist.h"
#include "../include/controller/ble_ll_resolv.h"
#include "../include/controller/ble_ll_trace.h"
#include "../include/controller/ble_ll_utils.h"
#include "../include/controller/ble_ll_rfmgmt.h"
#include "ble_ll_conn_priv.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(array) \
        (sizeof(array) / sizeof((array)[0]))
#endif

/* XXX: TODO
 * 1) Need to look at advertising and scan request PDUs. Do I allocate these
 * once? Do I use a different pool for smaller ones? Do I statically declare
 * them?
 * 3) How do features get supported? What happens if device does not support
 * advertising? (for example)
 * 4) How to determine the advertising interval we will actually use. As of
 * now, we set it to max.
 */

/* Scheduling data for secondary channel */
struct ble_ll_adv_aux {
    struct ble_ll_sched_item sch;
    uint32_t start_time;
    uint16_t aux_data_offset;
    uint8_t chan;
    uint8_t ext_hdr;
    uint8_t aux_data_len;
    uint8_t payload_len;
};

/* Scheduling data for sync PDUs */
struct ble_ll_adv_sync {
    struct ble_ll_sched_item sch;
    uint32_t start_time;
    uint16_t sync_data_offset;
    uint8_t chan;
    uint8_t ext_hdr;
    uint8_t sync_data_len;
    uint8_t payload_len;
};

/*
 * Advertising state machine
 *
 * The advertising state machine data structure.
 *
 *  adv_pdu_len
 *      The length of the advertising PDU that will be sent. This does not
 *      include the preamble, access address and CRC.
 *
 *  initiator_addr:
 *      This is the address that we send in directed advertisements (the
 *      INITA field). If we are using Privacy this is a RPA that we need to
 *      generate. We reserve space in the advsm to save time when creating
 *      the ADV_DIRECT_IND. If own address type is not 2 or 3, this is simply
 *      the peer address from the set advertising parameters.
 */
struct ble_ll_adv_sm
{
    uint8_t adv_enabled;
    uint8_t adv_instance;
    uint8_t adv_chanmask;
    uint8_t adv_filter_policy;
    uint8_t own_addr_type;
    uint8_t peer_addr_type;
    uint8_t adv_chan;
    uint8_t adv_pdu_len;
    int8_t adv_rpa_index;
    int8_t adv_txpwr;
    uint16_t flags;
    uint16_t props;
    uint16_t adv_itvl_min;
    uint16_t adv_itvl_max;
    uint32_t adv_itvl_usecs;
    uint32_t adv_event_start_time;
    uint32_t adv_pdu_start_time;
    uint32_t adv_end_time;
    uint32_t adv_rpa_timer;
    uint8_t adva[BLE_DEV_ADDR_LEN];
    uint8_t adv_rpa[BLE_DEV_ADDR_LEN];
    uint8_t peer_addr[BLE_DEV_ADDR_LEN];
    uint8_t initiator_addr[BLE_DEV_ADDR_LEN];
    struct os_mbuf *adv_data;
    struct os_mbuf *new_adv_data;
    struct os_mbuf *scan_rsp_data;
    struct os_mbuf *new_scan_rsp_data;
    uint8_t *conn_comp_ev;
    struct ble_npl_event adv_txdone_ev;
    struct ble_ll_sched_item adv_sch;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CSA2)
    uint16_t channel_id;
    uint16_t event_cntr;
#endif
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    uint8_t aux_active : 1;
    uint8_t aux_index : 1;
    uint8_t aux_first_pdu : 1;
    uint8_t aux_not_scanned : 1;
    uint8_t aux_dropped : 1;
    struct ble_mbuf_hdr *rx_ble_hdr;
    struct os_mbuf **aux_data;
    struct ble_ll_adv_aux aux[2];
    struct ble_npl_event adv_sec_txdone_ev;
    uint16_t duration;
    uint16_t adi;
    uint8_t adv_random_addr[BLE_DEV_ADDR_LEN];
    uint8_t events_max;
    uint8_t events;
    uint8_t pri_phy;
    uint8_t sec_phy;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV)
    struct os_mbuf *periodic_adv_data;
    struct os_mbuf *periodic_new_data;
    uint32_t periodic_crcinit; /* only 3 bytes are used */
    uint32_t periodic_access_addr;
    uint16_t periodic_adv_itvl_min;
    uint16_t periodic_adv_itvl_max;
    uint16_t periodic_adv_props;
    uint16_t periodic_channel_id;
    uint16_t periodic_event_cntr;
    uint16_t periodic_chain_event_cntr;
    uint8_t periodic_adv_enabled : 1;
    uint8_t periodic_adv_active : 1;
    uint8_t periodic_sync_active : 1;
    uint8_t periodic_sync_index : 1;
    uint8_t periodic_num_used_chans;
    uint8_t periodic_chanmap[BLE_LL_CONN_CHMAP_LEN];
    uint32_t periodic_adv_itvl_ticks;
    uint8_t periodic_adv_itvl_rem_usec;
    uint8_t periodic_adv_event_start_time_remainder;
    uint32_t periodic_adv_event_start_time;
    struct ble_ll_adv_sync periodic_sync[2];
    struct ble_npl_event adv_periodic_txdone_ev;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV_SYNC_TRANSFER)
    uint16_t periodic_event_cntr_last_sent;
#endif
#endif
#endif
};

#define BLE_LL_ADV_SM_FLAG_TX_ADD                   0x0001
#define BLE_LL_ADV_SM_FLAG_RX_ADD                   0x0002
#define BLE_LL_ADV_SM_FLAG_SCAN_REQ_NOTIF           0x0004
#define BLE_LL_ADV_SM_FLAG_CONN_RSP_TXD             0x0008
#define BLE_LL_ADV_SM_FLAG_ACTIVE_CHANSET_MASK      0x0030 /* use helpers! */
#define BLE_LL_ADV_SM_FLAG_ADV_DATA_INCOMPLETE      0x0040
#define BLE_LL_ADV_SM_FLAG_CONFIGURED               0x0080
#define BLE_LL_ADV_SM_FLAG_ADV_RPA_TMO              0x0100
#define BLE_LL_ADV_SM_FLAG_NEW_ADV_DATA             0x0200
#define BLE_LL_ADV_SM_FLAG_NEW_SCAN_RSP_DATA        0x0400
#define BLE_LL_ADV_SM_FLAG_PERIODIC_CONFIGURED      0x0800
#define BLE_LL_ADV_SM_FLAG_PERIODIC_DATA_INCOMPLETE 0x1000
#define BLE_LL_ADV_SM_FLAG_PERIODIC_SYNC_SENDING    0x2000
#define BLE_LL_ADV_SM_FLAG_PERIODIC_NEW_DATA        0x4000

#define ADV_DATA_LEN(_advsm) \
                ((_advsm->adv_data) ? OS_MBUF_PKTLEN(advsm->adv_data) : 0)
#define SCAN_RSP_DATA_LEN(_advsm) \
                ((_advsm->scan_rsp_data) ? OS_MBUF_PKTLEN(advsm->scan_rsp_data) : 0)
#define AUX_DATA_LEN(_advsm) \
                (*(_advsm->aux_data) ? OS_MBUF_PKTLEN(*advsm->aux_data) : 0)

#define AUX_CURRENT(_advsm)     (&(_advsm->aux[_advsm->aux_index]))
#define AUX_NEXT(_advsm)        (&(_advsm->aux[_advsm->aux_index ^ 1]))

#define SYNC_CURRENT(_advsm) (&(_advsm->periodic_sync[_advsm->periodic_sync_index]))
#define SYNC_NEXT(_advsm)    (&(_advsm->periodic_sync[_advsm->periodic_sync_index ^ 1]))
#define SYNC_DATA_LEN(_advsm) \
                (_advsm->periodic_adv_data ? OS_MBUF_PKTLEN(advsm->periodic_adv_data) : 0)

/* The advertising state machine global object */
struct ble_ll_adv_sm g_ble_ll_adv_sm[BLE_ADV_INSTANCES];
struct ble_ll_adv_sm *g_ble_ll_cur_adv_sm;

static struct ble_ll_adv_sm *
ble_ll_adv_sm_find_configured(uint8_t instance)
{
    struct ble_ll_adv_sm *advsm;
    int i;

    /* in legacy mode we only allow instance 0 */
    if (!ble_ll_hci_adv_mode_ext()) {
        BLE_LL_ASSERT(instance == 0);
        return &g_ble_ll_adv_sm[0];
    }

    for (i = 0; i < ARRAY_SIZE(g_ble_ll_adv_sm); i++) {
        advsm = &g_ble_ll_adv_sm[i];

        if ((advsm->flags & BLE_LL_ADV_SM_FLAG_CONFIGURED) &&
                (advsm->adv_instance == instance)) {
            return advsm;
        }
    }

    return NULL;
}

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
static int
ble_ll_adv_active_chanset_is_pri(struct ble_ll_adv_sm *advsm)
{
    return (advsm->flags & BLE_LL_ADV_SM_FLAG_ACTIVE_CHANSET_MASK) == 0x10;
}

static int
ble_ll_adv_active_chanset_is_sec(struct ble_ll_adv_sm *advsm)
{
    return (advsm->flags & BLE_LL_ADV_SM_FLAG_ACTIVE_CHANSET_MASK) == 0x20;
}
#endif

static void
ble_ll_adv_active_chanset_clear(struct ble_ll_adv_sm *advsm)
{
    os_sr_t sr;

    OS_ENTER_CRITICAL(sr);
    advsm->flags &= ~BLE_LL_ADV_SM_FLAG_ACTIVE_CHANSET_MASK;
    OS_EXIT_CRITICAL(sr);
}

static void
ble_ll_adv_active_chanset_set_pri(struct ble_ll_adv_sm *advsm)
{
    os_sr_t sr;

    OS_ENTER_CRITICAL(sr);
    assert((advsm->flags & BLE_LL_ADV_SM_FLAG_ACTIVE_CHANSET_MASK) == 0);
    advsm->flags &= ~BLE_LL_ADV_SM_FLAG_ACTIVE_CHANSET_MASK;
    advsm->flags |= 0x10;
    OS_EXIT_CRITICAL(sr);
}

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
static void
ble_ll_adv_active_chanset_set_sec(struct ble_ll_adv_sm *advsm)
{
    os_sr_t sr;

    OS_ENTER_CRITICAL(sr);
    assert((advsm->flags & BLE_LL_ADV_SM_FLAG_ACTIVE_CHANSET_MASK) == 0);
    advsm->flags &= ~BLE_LL_ADV_SM_FLAG_ACTIVE_CHANSET_MASK;
    advsm->flags |= 0x20;
    OS_EXIT_CRITICAL(sr);
}
#endif

static void
ble_ll_adv_flags_set(struct ble_ll_adv_sm *advsm, uint16_t flags)
{
    os_sr_t sr;

    OS_ENTER_CRITICAL(sr);
    advsm->flags |= flags;
    OS_EXIT_CRITICAL(sr);
}

static void
ble_ll_adv_flags_clear(struct ble_ll_adv_sm *advsm, uint16_t flags)
{
    os_sr_t sr;

    OS_ENTER_CRITICAL(sr);
    advsm->flags &= ~flags;
    OS_EXIT_CRITICAL(sr);
}

static void ble_ll_adv_make_done(struct ble_ll_adv_sm *advsm, struct ble_mbuf_hdr *hdr);
static void ble_ll_adv_sm_init(struct ble_ll_adv_sm *advsm);
static void ble_ll_adv_sm_stop_timeout(struct ble_ll_adv_sm *advsm);

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
static void
ble_ll_adv_rpa_update(struct ble_ll_adv_sm *advsm)
{
    if (ble_ll_resolv_gen_rpa(advsm->peer_addr, advsm->peer_addr_type,
                          advsm->adva, 1)) {
        ble_ll_adv_flags_set(advsm, BLE_LL_ADV_SM_FLAG_TX_ADD);
    } else {
        if (advsm->own_addr_type & 1) {
            ble_ll_adv_flags_set(advsm, BLE_LL_ADV_SM_FLAG_TX_ADD);
        } else {
            ble_ll_adv_flags_clear(advsm, BLE_LL_ADV_SM_FLAG_TX_ADD);
        }
    }

    if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_DIRECTED) {
        if (ble_ll_resolv_gen_rpa(advsm->peer_addr, advsm->peer_addr_type,
                              advsm->initiator_addr, 0)) {
            ble_ll_adv_flags_set(advsm, BLE_LL_ADV_SM_FLAG_RX_ADD);
        } else {
            if (advsm->peer_addr_type & 1) {
                ble_ll_adv_flags_set(advsm, BLE_LL_ADV_SM_FLAG_RX_ADD);
            } else {
                ble_ll_adv_flags_clear(advsm, BLE_LL_ADV_SM_FLAG_RX_ADD);
            }
        }
    }
}

/**
 * Called to change advertisers ADVA and INITA (for directed advertisements)
 * as an advertiser needs to adhere to the resolvable private address generation
 * timer.
 *
 * NOTE: the resolvable private address code uses its own timer to regenerate
 * local resolvable private addresses. The advertising code uses its own
 * timer to reset the INITA (for directed advertisements). This code also sets
 * the appropriate txadd and rxadd bits that will go into the advertisement.
 *
 * Another thing to note: it is possible that an IRK is all zeroes in the
 * resolving list. That is why we need to check if the generated address is
 * in fact a RPA as a resolving list entry with all zeroes will use the
 * identity address (which may be a private address or public).
 *
 * @param advsm
 */
void
ble_ll_adv_chk_rpa_timeout(struct ble_ll_adv_sm *advsm)
{
    if (advsm->own_addr_type < BLE_HCI_ADV_OWN_ADDR_PRIV_PUB) {
        return;
    }

    if (advsm->flags & BLE_LL_ADV_SM_FLAG_ADV_RPA_TMO) {
        ble_ll_adv_rpa_update(advsm);
        ble_ll_adv_flags_clear(advsm, BLE_LL_ADV_SM_FLAG_ADV_RPA_TMO);
    }
}

void
ble_ll_adv_rpa_timeout(void)
{
    struct ble_ll_adv_sm *advsm;
    int i;

    for (i = 0; i < BLE_ADV_INSTANCES; i++) {
        advsm = &g_ble_ll_adv_sm[i];

        if (advsm->adv_enabled &&
                advsm->own_addr_type > BLE_HCI_ADV_OWN_ADDR_RANDOM) {
            /* Mark RPA as timed out so we get a new RPA */
            ble_ll_adv_flags_set(advsm, BLE_LL_ADV_SM_FLAG_ADV_RPA_TMO);
        }
    }
}
#endif

/**
 * Calculate the first channel that we should advertise upon when we start
 * an advertising event.
 *
 * @param advsm
 *
 * @return uint8_t The number of the first channel usable for advertising.
 */
static uint8_t
ble_ll_adv_first_chan(struct ble_ll_adv_sm *advsm)
{
    uint8_t adv_chan;

    /* Set first advertising channel */
    if (advsm->adv_chanmask & 0x01) {
        adv_chan = BLE_PHY_ADV_CHAN_START;
    } else if (advsm->adv_chanmask & 0x02) {
        adv_chan = BLE_PHY_ADV_CHAN_START + 1;
    } else {
        adv_chan = BLE_PHY_ADV_CHAN_START + 2;
    }

    return adv_chan;
}

/**
 * Calculate the final channel that we should advertise upon when we start
 * an advertising event.
 *
 * @param advsm
 *
 * @return uint8_t The number of the final channel usable for advertising.
 */
static uint8_t
ble_ll_adv_final_chan(struct ble_ll_adv_sm *advsm)
{
    uint8_t adv_chan;

    if (advsm->adv_chanmask & 0x04) {
        adv_chan = BLE_PHY_ADV_CHAN_START + 2;
    } else if (advsm->adv_chanmask & 0x02) {
        adv_chan = BLE_PHY_ADV_CHAN_START + 1;
    } else {
        adv_chan = BLE_PHY_ADV_CHAN_START;
    }

    return adv_chan;
}

/**
 * Create the advertising legacy PDU
 *
 * @param advsm Pointer to advertisement state machine
 */
static uint8_t
ble_ll_adv_legacy_pdu_make(uint8_t *dptr, void *pducb_arg, uint8_t *hdr_byte)
{
    struct ble_ll_adv_sm *advsm;
    uint8_t     adv_data_len;
    uint8_t     pdulen;
    uint8_t     pdu_type;

    advsm = pducb_arg;

    /* assume this is not a direct ind */
    adv_data_len = ADV_DATA_LEN(advsm);
    pdulen = BLE_DEV_ADDR_LEN + adv_data_len;

    if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_DIRECTED) {
        pdu_type = BLE_ADV_PDU_TYPE_ADV_DIRECT_IND;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CSA2)
        pdu_type |= BLE_ADV_PDU_HDR_CHSEL;
#endif

        if (advsm->flags & BLE_LL_ADV_SM_FLAG_RX_ADD) {
            pdu_type |= BLE_ADV_PDU_HDR_RXADD_RAND;
        }

        adv_data_len = 0;
        pdulen = BLE_ADV_DIRECT_IND_LEN;
    } else if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_CONNECTABLE) {
        pdu_type = BLE_ADV_PDU_TYPE_ADV_IND;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CSA2)
        pdu_type |= BLE_ADV_PDU_HDR_CHSEL;
#endif
    } else if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_SCANNABLE) {
        pdu_type = BLE_ADV_PDU_TYPE_ADV_SCAN_IND;
    } else {
        pdu_type = BLE_ADV_PDU_TYPE_ADV_NONCONN_IND;
    }

    /* An invalid advertising data length indicates a memory overwrite */
    assert(adv_data_len <= BLE_ADV_LEGACY_DATA_MAX_LEN);

    /* Set the PDU length in the state machine (includes header) */
    advsm->adv_pdu_len = pdulen + BLE_LL_PDU_HDR_LEN;

    /* Set TxAdd to random if needed. */
    if (advsm->flags & BLE_LL_ADV_SM_FLAG_TX_ADD) {
        pdu_type |= BLE_ADV_PDU_HDR_TXADD_RAND;
    }

    *hdr_byte = pdu_type;

    /* Construct advertisement */
    memcpy(dptr, advsm->adva, BLE_DEV_ADDR_LEN);
    dptr += BLE_DEV_ADDR_LEN;

    /* For ADV_DIRECT_IND add inita */
    if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_DIRECTED) {
        memcpy(dptr, advsm->initiator_addr, BLE_DEV_ADDR_LEN);
    }

    /* Copy in advertising data, if any */
    if (adv_data_len != 0) {
        os_mbuf_copydata(advsm->adv_data, 0, adv_data_len, dptr);
    }

    return pdulen;
}

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
static void
ble_ll_adv_put_aux_ptr(uint8_t chan, uint8_t phy, uint32_t offset,
                       uint8_t *dptr)
{
    dptr[0] = chan;

    if (offset > 245700) {
        dptr[0] |= 0x80;
        offset = offset / 300;
    } else {
        offset = offset / 30;
    }

    if (offset > 0x1fff) {
        offset = 0;
    }

    /* offset is 13bits and PHY 3 bits */
    dptr[1] = (offset & 0x000000ff);
    dptr[2] = ((offset >> 8) & 0x0000001f) | (phy - 1) << 5;
}

/**
 * Create the advertising PDU
 */
static uint8_t
ble_ll_adv_pdu_make(uint8_t *dptr, void *pducb_arg, uint8_t *hdr_byte)
{
    struct ble_ll_adv_sm *advsm;
    uint8_t pdu_type;
    uint8_t adv_mode;
    uint8_t ext_hdr_len;
    uint8_t ext_hdr_flags;
    uint32_t offset;

    advsm = pducb_arg;

    assert(ble_ll_adv_active_chanset_is_pri(advsm));
    if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY) {
        return ble_ll_adv_legacy_pdu_make(dptr, advsm, hdr_byte);
    }

    /* only ADV_EXT_IND goes on primary advertising channels */
    pdu_type = BLE_ADV_PDU_TYPE_ADV_EXT_IND;

    /* Set TxAdd to random if needed. */
    if (advsm->flags & BLE_LL_ADV_SM_FLAG_TX_ADD) {
        pdu_type |= BLE_ADV_PDU_HDR_TXADD_RAND;
    }

    *hdr_byte = pdu_type;

    adv_mode = 0;
    if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_CONNECTABLE) {
        adv_mode |= BLE_LL_EXT_ADV_MODE_CONN;
    }
    if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_SCANNABLE) {
        adv_mode |= BLE_LL_EXT_ADV_MODE_SCAN;
    }

    ext_hdr_len = BLE_LL_EXT_ADV_FLAGS_SIZE + BLE_LL_EXT_ADV_DATA_INFO_SIZE +
                  BLE_LL_EXT_ADV_AUX_PTR_SIZE;
    ext_hdr_flags = (1 << BLE_LL_EXT_ADV_DATA_INFO_BIT) |
                    (1 << BLE_LL_EXT_ADV_AUX_PTR_BIT);

    /* ext hdr len and adv mode */
    dptr[0] = ext_hdr_len | (adv_mode << 6);
    dptr += 1;

    /* ext hdr flags */
    dptr[0] = ext_hdr_flags;
    dptr += 1;

    /* ADI */
    dptr[0] = advsm->adi & 0x00ff;
    dptr[1] = advsm->adi >> 8;
    dptr += BLE_LL_EXT_ADV_DATA_INFO_SIZE;

    /* AuxPtr */
    if (AUX_CURRENT(advsm)->sch.enqueued) {
        offset = os_cputime_ticks_to_usecs(AUX_CURRENT(advsm)->start_time - advsm->adv_pdu_start_time);
    } else {
        offset = 0;
    }
    /* Always use channel from 1st AUX */
    ble_ll_adv_put_aux_ptr(AUX_CURRENT(advsm)->chan, advsm->sec_phy,
                           offset, dptr);

    return BLE_LL_EXT_ADV_HDR_LEN + ext_hdr_len;
}

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV)
static void
ble_ll_adv_put_syncinfo(struct ble_ll_adv_sm *advsm,
                        struct ble_ll_conn_sm *connsm, uint8_t *conn_event_cnt,
                        uint8_t *dptr)
{
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV_SYNC_TRANSFER)
    uint8_t anchor_usecs;
    uint16_t conn_cnt;
#endif
    unsigned int event_cnt_off = 0;
    uint32_t offset = 0;
    uint32_t anchor;
    uint8_t units;

    if (connsm) {
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV_SYNC_TRANSFER)
        anchor = connsm->anchor_point;
        anchor_usecs = connsm->anchor_point_usecs;
        conn_cnt = connsm->event_cntr;

        /* get anchor for conn event that is before periodic_adv_event_start_time */
        while (CPUTIME_GT(anchor, advsm->periodic_adv_event_start_time)) {
            ble_ll_conn_get_anchor(connsm, --conn_cnt, &anchor, &anchor_usecs);
        }

        offset = os_cputime_ticks_to_usecs(advsm->periodic_adv_event_start_time - anchor);
        offset -= anchor_usecs;
        offset += advsm->periodic_adv_event_start_time_remainder;

        /* connEventCount */
        put_le16(conn_event_cnt, conn_cnt);
#endif
    } else {
        anchor = advsm->periodic_adv_event_start_time;

        /* Get periodic event that is past AUX start time (so that we always
         * provide valid offset if it is not too far in future). This can
         * happen if advertising event is interleaved with periodic advertising
         * event (when chaining).
         */
        while (CPUTIME_GT(AUX_CURRENT(advsm)->start_time, anchor)) {
            anchor += advsm->periodic_adv_itvl_ticks;
            event_cnt_off++;
        }

        offset = os_cputime_ticks_to_usecs(anchor - AUX_CURRENT(advsm)->start_time);
        offset += advsm->periodic_adv_event_start_time_remainder;
        offset += advsm->periodic_adv_itvl_rem_usec;
    }

    /* Sync Packet Offset (13 bits), Offset Units (1 bit), Offset Adjust (1 bit),
     * RFU (1 bit)
     */
    if (offset > 245700) {
        units = 0x20;
        offset = offset / 300;

        if (offset >= 0x2000) {
            if (connsm) {
                offset -= 0x2000;
                units |= 0x40;
            } else {
                /* not able to represent time in offset */
                offset = 0;
                units = 0x00;
                event_cnt_off = 0;
            }
        }

    } else {
        units = 0x00;
        offset = offset / 30;
    }

    dptr[0] = (offset & 0x000000ff);
    dptr[1] = ((offset >> 8) & 0x0000001f) | units;

    /* Interval (2 bytes) */
    put_le16(&dptr[2], advsm->periodic_adv_itvl_max);

    /* Channels Mask (37 bits) */
    dptr[4] = advsm->periodic_chanmap[0];
    dptr[5] = advsm->periodic_chanmap[1];
    dptr[6] = advsm->periodic_chanmap[2];
    dptr[7] = advsm->periodic_chanmap[3];
    dptr[8] = advsm->periodic_chanmap[4] & 0x1f;

    /* SCA (3 bits) */
    dptr[8] |= BLE_LL_SCA_ENUM << 5;

    /* AA (4 bytes) */
    put_le32(&dptr[9], advsm->periodic_access_addr);

    /* CRCInit (3 bytes) */
    dptr[13] = (uint8_t)advsm->periodic_crcinit;
    dptr[14] = (uint8_t)(advsm->periodic_crcinit >> 8);
    dptr[15] = (uint8_t)(advsm->periodic_crcinit >> 16);

    /* Event Counter (2 bytes) */
    put_le16(&dptr[16], advsm->periodic_event_cntr + event_cnt_off);
}
#endif

/**
 * Create the AUX PDU
 */
static uint8_t
ble_ll_adv_aux_pdu_make(uint8_t *dptr, void *pducb_arg, uint8_t *hdr_byte)
{
    struct ble_ll_adv_sm *advsm;
    struct ble_ll_adv_aux *aux;
    uint8_t adv_mode;
    uint8_t pdu_type;
    uint8_t ext_hdr_len;
    uint32_t offset;

    advsm = pducb_arg;
    aux = AUX_CURRENT(advsm);

    assert(!(advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY));
    assert(ble_ll_adv_active_chanset_is_sec(advsm));

    /* It's the same for AUX_ADV_IND and AUX_CHAIN_IND */
    pdu_type = BLE_ADV_PDU_TYPE_AUX_ADV_IND;

    /* We do not create scannable PDUs here - this is handled separately */
    adv_mode = 0;
    if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_CONNECTABLE) {
        adv_mode |= BLE_LL_EXT_ADV_MODE_CONN;
    }

    ext_hdr_len = aux->payload_len - BLE_LL_EXT_ADV_HDR_LEN - aux->aux_data_len;
    dptr[0] = (adv_mode << 6) | ext_hdr_len;
    dptr += 1;

    /* only put flags if needed */
    if (aux->ext_hdr) {
        dptr[0] = aux->ext_hdr;
        dptr += 1;
    }

    if (aux->ext_hdr & (1 << BLE_LL_EXT_ADV_ADVA_BIT)) {

        /* Set TxAdd to random if needed. */
        if (advsm->flags & BLE_LL_ADV_SM_FLAG_TX_ADD) {
            pdu_type |= BLE_ADV_PDU_HDR_TXADD_RAND;
        }

        memcpy(dptr, advsm->adva, BLE_LL_EXT_ADV_ADVA_SIZE);
        dptr += BLE_LL_EXT_ADV_ADVA_SIZE;
    }

    if (aux->ext_hdr & (1 << BLE_LL_EXT_ADV_TARGETA_BIT)) {
        memcpy(dptr, advsm->initiator_addr, BLE_LL_EXT_ADV_TARGETA_SIZE);
        dptr += BLE_LL_EXT_ADV_TARGETA_SIZE;

        /* Set RxAdd to random if needed. */
        if (advsm->flags & BLE_LL_ADV_SM_FLAG_RX_ADD) {
            pdu_type |= BLE_ADV_PDU_HDR_RXADD_RAND;
        }
    }

    if (aux->ext_hdr & (1 << BLE_LL_EXT_ADV_DATA_INFO_BIT)) {
        dptr[0] = advsm->adi & 0x00ff;
        dptr[1] = advsm->adi >> 8;
        dptr += BLE_LL_EXT_ADV_DATA_INFO_SIZE;
    }

    if (aux->ext_hdr & (1 << BLE_LL_EXT_ADV_AUX_PTR_BIT)) {
        if (!AUX_NEXT(advsm)->sch.enqueued) {
            /*
             * Trim data here in case we do not have next aux scheduled. This
             * can happen if next aux was outside advertising set period and
             * was removed from scheduler.
             */
            offset = 0;
        } else if (advsm->rx_ble_hdr) {
            offset = os_cputime_ticks_to_usecs(AUX_NEXT(advsm)->start_time - advsm->rx_ble_hdr->beg_cputime);
            offset -= (advsm->rx_ble_hdr->rem_usecs + ble_ll_pdu_tx_time_get(12, advsm->sec_phy) + BLE_LL_IFS);
        } else {
            offset = os_cputime_ticks_to_usecs(AUX_NEXT(advsm)->start_time - aux->start_time);
        }

        ble_ll_adv_put_aux_ptr(AUX_NEXT(advsm)->chan, advsm->sec_phy,
                               offset, dptr);

        dptr += BLE_LL_EXT_ADV_AUX_PTR_SIZE;
    }

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV)
    if (aux->ext_hdr & (1 << BLE_LL_EXT_ADV_SYNC_INFO_BIT)) {
        ble_ll_adv_put_syncinfo(advsm, NULL, NULL, dptr);
        dptr += BLE_LL_EXT_ADV_SYNC_INFO_SIZE;
    }
#endif

    if (aux->ext_hdr & (1 << BLE_LL_EXT_ADV_TX_POWER_BIT)) {
        dptr[0] = advsm->adv_txpwr + ble_ll_get_tx_pwr_compensation();
        dptr += BLE_LL_EXT_ADV_TX_POWER_SIZE;
    }

    if (aux->aux_data_len) {
        os_mbuf_copydata(*advsm->aux_data, aux->aux_data_offset,
                         aux->aux_data_len, dptr);
    }

    *hdr_byte = pdu_type;

    return aux->payload_len;
}

static uint8_t
ble_ll_adv_aux_scannable_pdu_make(uint8_t *dptr, void *pducb_arg, uint8_t *hdr_byte)
{
    struct ble_ll_adv_sm *advsm;
    uint8_t pdu_type;
    uint8_t *ext_hdr_len;
    uint8_t *ext_hdr;
    uint8_t pdulen;

    advsm = pducb_arg;

    assert(!(advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY));
    assert(advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_SCANNABLE);
    assert(advsm->aux_first_pdu);
    assert(ble_ll_adv_active_chanset_is_sec(advsm));

    pdu_type = BLE_ADV_PDU_TYPE_AUX_ADV_IND;

    ext_hdr_len = &dptr[0];
    ext_hdr = &dptr[1];
    dptr += 2;

    /* Flags always */
    *ext_hdr_len = BLE_LL_EXT_ADV_FLAGS_SIZE;
    *ext_hdr = 0;

    /* AdvA when non anonymous */
    if (!(advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_ANON_ADV)) {
        /* Set TxAdd to random if needed. */
        if (advsm->flags & BLE_LL_ADV_SM_FLAG_TX_ADD) {
            pdu_type |= BLE_ADV_PDU_HDR_TXADD_RAND;
        }

        *ext_hdr_len += BLE_LL_EXT_ADV_ADVA_SIZE;
        *ext_hdr |= (1 << BLE_LL_EXT_ADV_ADVA_BIT);
        memcpy(dptr, advsm->adva, BLE_LL_EXT_ADV_ADVA_SIZE);
        dptr += BLE_LL_EXT_ADV_ADVA_SIZE;
    }

    /* TargetA only for directed */
    if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_DIRECTED) {
        *ext_hdr_len += BLE_LL_EXT_ADV_TARGETA_SIZE;
        *ext_hdr |= (1 << BLE_LL_EXT_ADV_TARGETA_BIT);
        memcpy(dptr, advsm->initiator_addr, BLE_LL_EXT_ADV_TARGETA_SIZE);
        dptr += BLE_LL_EXT_ADV_TARGETA_SIZE;

        /* Set RxAdd to random if needed. */
        if (advsm->flags & BLE_LL_ADV_SM_FLAG_RX_ADD) {
            pdu_type |= BLE_ADV_PDU_HDR_RXADD_RAND;
        }
    }

    /* ADI always */
    *ext_hdr_len += BLE_LL_EXT_ADV_DATA_INFO_SIZE;
    *ext_hdr |= (1 << BLE_LL_EXT_ADV_DATA_INFO_BIT);
    dptr[0] = advsm->adi & 0x00ff;
    dptr[1] = advsm->adi >> 8;
    dptr += BLE_LL_EXT_ADV_DATA_INFO_SIZE;

    /* TxPower if configured */
    if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_INC_TX_PWR) {
        *ext_hdr_len += BLE_LL_EXT_ADV_TX_POWER_SIZE;
        *ext_hdr |= (1 << BLE_LL_EXT_ADV_TX_POWER_BIT);
        dptr[0] = advsm->adv_txpwr + ble_ll_get_tx_pwr_compensation();
        dptr += BLE_LL_EXT_ADV_TX_POWER_SIZE;
    }

    pdulen = BLE_LL_EXT_ADV_HDR_LEN + *ext_hdr_len;

    *hdr_byte = pdu_type;
    *ext_hdr_len |= (BLE_LL_EXT_ADV_MODE_SCAN << 6);

    return pdulen;
}
#endif

static uint8_t
ble_ll_adv_scan_rsp_legacy_pdu_make(uint8_t *dptr, void *pducb_arg,
                                    uint8_t *hdr_byte)
{
    struct ble_ll_adv_sm *advsm;
    uint8_t     scan_rsp_len;
    uint8_t     pdulen;
    uint8_t     hdr;

    advsm = pducb_arg;

    /* Make sure that the length is valid */
    scan_rsp_len = SCAN_RSP_DATA_LEN(advsm);
    assert(scan_rsp_len <= BLE_SCAN_RSP_LEGACY_DATA_MAX_LEN);

    /* Set BLE transmit header */
    pdulen = BLE_DEV_ADDR_LEN + scan_rsp_len;
    hdr = BLE_ADV_PDU_TYPE_SCAN_RSP;
    if (advsm->flags & BLE_LL_ADV_SM_FLAG_TX_ADD) {
        hdr |= BLE_ADV_PDU_HDR_TXADD_RAND;
    }

    *hdr_byte = hdr;

    /*
     * The adva in this packet will be the same one that was being advertised
     * and is based on the peer identity address in the set advertising
     * parameters. If a different peer sends us a scan request (for some reason)
     * we will reply with an adva that was not generated based on the local irk
     * of the peer sending the scan request.
     */

    /* Construct scan response */
    memcpy(dptr, advsm->adva, BLE_DEV_ADDR_LEN);
    if (scan_rsp_len != 0) {
        os_mbuf_copydata(advsm->scan_rsp_data, 0, scan_rsp_len,
                         dptr + BLE_DEV_ADDR_LEN);
    }

    return pdulen;
}

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
/**
 * Create a scan response PDU
 *
 * @param advsm
 */
static uint8_t
ble_ll_adv_scan_rsp_pdu_make(uint8_t *dptr, void *pducb_arg, uint8_t *hdr_byte)
{
    struct ble_ll_adv_sm *advsm;

    advsm = pducb_arg;

    if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY) {
        return ble_ll_adv_scan_rsp_legacy_pdu_make(dptr, pducb_arg, hdr_byte);
    }

    return ble_ll_adv_aux_pdu_make(dptr, pducb_arg, hdr_byte);
}

struct aux_conn_rsp_data {
    struct ble_ll_adv_sm *advsm;
    uint8_t *peer;
    uint8_t rxadd;
};

/**
 * Create a AUX connect response PDU
 *
 * @param advsm
 */
static uint8_t
ble_ll_adv_aux_conn_rsp_pdu_make(uint8_t *dptr, void *pducb_arg,
                                 uint8_t *hdr_byte)
{
    struct aux_conn_rsp_data *rsp_data;
    uint8_t     pdulen;
    uint8_t     ext_hdr_len;
    uint8_t     ext_hdr_flags;
    uint8_t     hdr;

    rsp_data = pducb_arg;

    /* flags,AdvA and TargetA */
    ext_hdr_len = BLE_LL_EXT_ADV_FLAGS_SIZE + BLE_LL_EXT_ADV_ADVA_SIZE +
                  BLE_LL_EXT_ADV_TARGETA_SIZE;
    ext_hdr_flags = (1 << BLE_LL_EXT_ADV_ADVA_BIT);
    ext_hdr_flags |= (1 << BLE_LL_EXT_ADV_TARGETA_BIT);

    pdulen = BLE_LL_EXT_ADV_HDR_LEN + ext_hdr_len;

    /* Set BLE transmit header */
    hdr = BLE_ADV_PDU_TYPE_AUX_CONNECT_RSP;
    if (rsp_data->rxadd) {
        hdr |= BLE_ADV_PDU_HDR_RXADD_MASK;
    }
    if (rsp_data->advsm->flags & BLE_LL_ADV_SM_FLAG_TX_ADD) {
        hdr |= BLE_ADV_PDU_HDR_TXADD_MASK;
    }

    *hdr_byte = hdr;

    /* ext hdr len and adv mode (00b) */
    dptr[0] = ext_hdr_len;
    dptr += 1;

    /* ext hdr flags */
    dptr[0] = ext_hdr_flags;
    dptr += 1;

    memcpy(dptr, rsp_data->advsm->adva, BLE_LL_EXT_ADV_ADVA_SIZE);
    dptr += BLE_LL_EXT_ADV_ADVA_SIZE;

    memcpy(dptr, rsp_data->peer, BLE_LL_EXT_ADV_TARGETA_SIZE);
    dptr += BLE_LL_EXT_ADV_ADVA_SIZE;

    return pdulen;
}
#endif

/**
 * Called to indicate the advertising event is over.
 *
 * Context: Interrupt
 *
 * @param advsm
 *
 */
static void
ble_ll_adv_tx_done(void *arg)
{
    struct ble_ll_adv_sm *advsm;

    /* reset power to max after advertising */
    ble_phy_txpwr_set(MYNEWT_VAL(BLE_LL_TX_PWR_DBM));

    advsm = (struct ble_ll_adv_sm *)arg;

    ble_ll_trace_u32x2(BLE_LL_TRACE_ID_ADV_TXDONE, advsm->adv_instance,
                       advsm->flags & BLE_LL_ADV_SM_FLAG_ACTIVE_CHANSET_MASK);

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    if (ble_ll_adv_active_chanset_is_pri(advsm)) {
        ble_npl_eventq_put(&g_ble_ll_data.ll_evq, &advsm->adv_txdone_ev);
    } else if (ble_ll_adv_active_chanset_is_sec(advsm)) {
        ble_npl_eventq_put(&g_ble_ll_data.ll_evq, &advsm->adv_sec_txdone_ev);
    } else {
        assert(0);
    }
#else
    ble_npl_eventq_put(&g_ble_ll_data.ll_evq, &advsm->adv_txdone_ev);
#endif

    ble_ll_state_set(BLE_LL_STATE_STANDBY);

    ble_ll_adv_active_chanset_clear(advsm);

    /* We no longer have a current state machine */
    g_ble_ll_cur_adv_sm = NULL;
}

/*
 * Called when an advertising event has been removed from the scheduler
 * without being run.
 */
void
ble_ll_adv_event_rmvd_from_sched(struct ble_ll_adv_sm *advsm)
{
    /*
     * Need to set advertising channel to final chan so new event gets
     * scheduled.
     */
    advsm->adv_chan = ble_ll_adv_final_chan(advsm);
    ble_npl_eventq_put(&g_ble_ll_data.ll_evq, &advsm->adv_txdone_ev);
}

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV)
/*
 * Called when a periodic event has been removed from the scheduler
 * without being run.
 */
void
ble_ll_adv_periodic_rmvd_from_sched(struct ble_ll_adv_sm *advsm)
{
    ble_npl_eventq_put(&g_ble_ll_data.ll_evq, &advsm->adv_periodic_txdone_ev);
}
#endif

/**
 * This is the scheduler callback (called from interrupt context) which
 * transmits an advertisement.
 *
 * Context: Interrupt (scheduler)
 *
 * @param sch
 *
 * @return int
 */
static int
ble_ll_adv_tx_start_cb(struct ble_ll_sched_item *sch)
{
    int rc;
    uint8_t end_trans;
    uint32_t txstart;
    struct ble_ll_adv_sm *advsm;

    /* Get the state machine for the event */
    advsm = (struct ble_ll_adv_sm *)sch->cb_arg;

    /* Set the current advertiser */
    g_ble_ll_cur_adv_sm = advsm;

    ble_ll_adv_active_chanset_set_pri(advsm);

    if ((advsm->flags & BLE_LL_ADV_SM_FLAG_NEW_ADV_DATA) ||
        (advsm->flags & BLE_LL_ADV_SM_FLAG_NEW_SCAN_RSP_DATA)) {
        goto adv_tx_done;
    }

    /* Set the power */
    ble_phy_txpwr_set(advsm->adv_txpwr);

    /* Set channel */
    rc = ble_phy_setchan(advsm->adv_chan, BLE_ACCESS_ADDR_ADV, BLE_LL_CRCINIT_ADV);
    assert(rc == 0);

#if (BLE_LL_BT5_PHY_SUPPORTED == 1)
    /* Set phy mode */
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY) {
        ble_phy_mode_set(BLE_PHY_MODE_1M, BLE_PHY_MODE_1M);
    } else {
        ble_phy_mode_set(advsm->pri_phy, advsm->pri_phy);
    }
#else
    ble_phy_mode_set(BLE_PHY_MODE_1M, BLE_PHY_MODE_1M);
#endif
#endif

    /* Set transmit start time. */
    txstart = sch->start_time + g_ble_ll_sched_offset_ticks;
    rc = ble_phy_tx_set_start_time(txstart, sch->remainder);
    if (rc) {
        STATS_INC(ble_ll_stats, adv_late_starts);
        goto adv_tx_done;
    }

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
    ble_phy_encrypt_disable();
#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
    advsm->adv_rpa_index = -1;
    if (ble_ll_resolv_enabled()) {
        ble_phy_resolv_list_enable();
    } else {
        ble_phy_resolv_list_disable();
    }
#endif

    /* We switch to RX after connectable or scannable legacy packets. */
    if ((advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY) &&
            ((advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_CONNECTABLE) ||
             (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_SCANNABLE))) {
        end_trans = BLE_PHY_TRANSITION_TX_RX;
        ble_phy_set_txend_cb(NULL, NULL);
    } else {
        end_trans = BLE_PHY_TRANSITION_NONE;
        ble_phy_set_txend_cb(ble_ll_adv_tx_done, advsm);
    }

    /* Transmit advertisement */
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    rc = ble_phy_tx(ble_ll_adv_pdu_make, advsm, end_trans);
#else
    rc = ble_phy_tx(ble_ll_adv_legacy_pdu_make, advsm, end_trans);
#endif
    if (rc) {
        goto adv_tx_done;
    }

    /* Enable/disable whitelisting based on filter policy */
    if (advsm->adv_filter_policy != BLE_HCI_ADV_FILT_NONE) {
        ble_ll_whitelist_enable();
    } else {
        ble_ll_whitelist_disable();
    }

    /* Set link layer state to advertising */
    ble_ll_state_set(BLE_LL_STATE_ADV);

    /* Count # of adv. sent */
    STATS_INC(ble_ll_stats, adv_txg);

    return BLE_LL_SCHED_STATE_RUNNING;

adv_tx_done:
    ble_ll_adv_tx_done(advsm);
    return BLE_LL_SCHED_STATE_DONE;
}

static void
ble_ll_adv_set_sched(struct ble_ll_adv_sm *advsm)
{
    uint32_t max_usecs;
    struct ble_ll_sched_item *sch;

    sch = &advsm->adv_sch;
    sch->cb_arg = advsm;
    sch->sched_cb = ble_ll_adv_tx_start_cb;
    sch->sched_type = BLE_LL_SCHED_TYPE_ADV;

    /* Set end time to maximum time this schedule item may take */
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY) {
        max_usecs = ble_ll_pdu_tx_time_get(advsm->adv_pdu_len, BLE_PHY_MODE_1M);

        if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_DIRECTED) {
            max_usecs += BLE_LL_SCHED_DIRECT_ADV_MAX_USECS;
        } else if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_CONNECTABLE) {
            max_usecs += BLE_LL_SCHED_ADV_MAX_USECS;
        }
    } else {
        /*
         * In ADV_EXT_IND we always set only ADI and AUX so the payload length
         * is always 7 bytes.
         */
        max_usecs = ble_ll_pdu_tx_time_get(7, advsm->pri_phy);
    }
#else
    max_usecs = ble_ll_pdu_tx_time_get(advsm->adv_pdu_len, BLE_PHY_MODE_1M);

    if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_DIRECTED) {
        max_usecs += BLE_LL_SCHED_DIRECT_ADV_MAX_USECS;
    } else if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_CONNECTABLE) {
        max_usecs += BLE_LL_SCHED_ADV_MAX_USECS;
    }
#endif

    sch->start_time = advsm->adv_pdu_start_time - g_ble_ll_sched_offset_ticks;
    sch->remainder = 0;
    sch->end_time = advsm->adv_pdu_start_time +
                    ble_ll_usecs_to_ticks_round_up(max_usecs);
}

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
static int
ble_ll_adv_secondary_tx_start_cb(struct ble_ll_sched_item *sch)
{
    int rc;
    uint8_t end_trans;
    uint32_t txstart;
    struct ble_ll_adv_sm *advsm;
    ble_phy_tx_pducb_t pducb;
    struct ble_ll_adv_aux *aux;

    /* Get the state machine for the event */
    advsm = (struct ble_ll_adv_sm *)sch->cb_arg;

    /* Set the current advertiser */
    g_ble_ll_cur_adv_sm = advsm;

    ble_ll_adv_active_chanset_set_sec(advsm);

    /* Set the power */
    ble_phy_txpwr_set(advsm->adv_txpwr);

    /* Set channel */
    aux = AUX_CURRENT(advsm);
    rc = ble_phy_setchan(aux->chan, BLE_ACCESS_ADDR_ADV,
                         BLE_LL_CRCINIT_ADV);
    assert(rc == 0);

#if (BLE_LL_BT5_PHY_SUPPORTED == 1)
    /* Set phy mode */
     ble_phy_mode_set(advsm->sec_phy, advsm->sec_phy);
#endif

    /* Set transmit start time. */
    txstart = sch->start_time + g_ble_ll_sched_offset_ticks;
    rc = ble_phy_tx_set_start_time(txstart, sch->remainder);
    if (rc) {
        STATS_INC(ble_ll_stats, adv_late_starts);
        goto adv_aux_dropped;
    }

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
    ble_phy_encrypt_disable();
#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
    advsm->adv_rpa_index = -1;
    if (ble_ll_resolv_enabled()) {
        ble_phy_resolv_list_enable();
    } else {
        ble_phy_resolv_list_disable();
    }
#endif

    /* Set phy mode based on type of advertisement */
    if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_CONNECTABLE) {
        end_trans = BLE_PHY_TRANSITION_TX_RX;
        ble_phy_set_txend_cb(NULL, NULL);
        pducb = ble_ll_adv_aux_pdu_make;
    } else if ((advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_SCANNABLE) &&
               advsm->aux_first_pdu) {
        end_trans = BLE_PHY_TRANSITION_TX_RX;
        ble_phy_set_txend_cb(NULL, NULL);
        pducb = ble_ll_adv_aux_scannable_pdu_make;
    } else {
        end_trans = BLE_PHY_TRANSITION_NONE;
        ble_phy_set_txend_cb(ble_ll_adv_tx_done, advsm);
        pducb = ble_ll_adv_aux_pdu_make;
    }

    /* Transmit advertisement */
    rc = ble_phy_tx(pducb, advsm, end_trans);
    if (rc) {
        goto adv_aux_dropped;
    }

    /* Enable/disable whitelisting based on filter policy */
    if (advsm->adv_filter_policy != BLE_HCI_ADV_FILT_NONE) {
        ble_ll_whitelist_enable();
    } else {
        ble_ll_whitelist_disable();
    }

    /* Set link layer state to advertising */
    ble_ll_state_set(BLE_LL_STATE_ADV);

    /* Count # of adv. sent */
    STATS_INC(ble_ll_stats, adv_txg);

    return BLE_LL_SCHED_STATE_RUNNING;

adv_aux_dropped:
    advsm->aux_dropped = 1;
    ble_ll_adv_tx_done(advsm);
    return BLE_LL_SCHED_STATE_DONE;
}

static uint8_t
ble_ll_adv_aux_scannable_pdu_payload_len(struct ble_ll_adv_sm *advsm)
{
    uint8_t len;

    /* Flags, ADI always */
    len = BLE_LL_EXT_ADV_HDR_LEN + BLE_LL_EXT_ADV_FLAGS_SIZE
            + BLE_LL_EXT_ADV_DATA_INFO_SIZE;

    /* AdvA if not anonymous */
    if (!(advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_ANON_ADV)) {
        len += BLE_LL_EXT_ADV_ADVA_SIZE;
    }

    /* TargetA only for directed */
    if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_DIRECTED) {
        len += BLE_LL_EXT_ADV_TARGETA_SIZE;
    }

    /* TxPower if configured */
    if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_INC_TX_PWR) {
        len += BLE_LL_EXT_ADV_TX_POWER_SIZE;
    }

    return len;
}

static void
ble_ll_adv_aux_calculate(struct ble_ll_adv_sm *advsm,
                         struct ble_ll_adv_aux *aux, uint16_t aux_data_offset)
{
    uint16_t rem_aux_data_len;
    uint8_t hdr_len;
    bool chainable;

    assert(!aux->sch.enqueued);
    assert((AUX_DATA_LEN(advsm) > aux_data_offset) ||
           (AUX_DATA_LEN(advsm) == 0 && aux_data_offset == 0));

    aux->aux_data_offset = aux_data_offset;
    aux->aux_data_len = 0;
    aux->payload_len = 0;
    aux->ext_hdr = 0;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CSA2)
    aux->chan = ble_ll_utils_calc_dci_csa2(advsm->event_cntr++,
                                           advsm->channel_id,
                                           g_ble_ll_conn_params.num_used_chans,
                                           g_ble_ll_conn_params.master_chan_map);
#else
    aux->chan = ble_ll_utils_remapped_channel(ble_ll_rand() % BLE_PHY_NUM_DATA_CHANS,
                                              g_ble_ll_conn_params.master_chan_map);
#endif

    rem_aux_data_len = AUX_DATA_LEN(advsm) - aux_data_offset;
    chainable = !(advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_CONNECTABLE);

    hdr_len = BLE_LL_EXT_ADV_HDR_LEN;

    if (!(advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_SCANNABLE)) {
        /* ADI */
        aux->ext_hdr |= (1 << BLE_LL_EXT_ADV_DATA_INFO_BIT);
        hdr_len += BLE_LL_EXT_ADV_DATA_INFO_SIZE;
    }

    /* AdvA for 1st PDU in chain (i.e. AUX_ADV_IND or AUX_SCAN_RSP) */
    if (aux_data_offset == 0 &&
        !(advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_ANON_ADV)) {
        aux->ext_hdr |= (1 << BLE_LL_EXT_ADV_ADVA_BIT);
        hdr_len += BLE_LL_EXT_ADV_ADVA_SIZE;
    }

    /* Note: this function does not calculate AUX_ADV_IND when advertising is
     * scannable. Instead it is calculated in ble_ll_adv_aux_schedule_first().
     *
     * However this function calculates length of AUX_SCAN_RSP and according
     * to BT 5.0 Vol 6 Part B, 2.3.2.3, TargetA shall not be include there.
     *
     * This is why TargetA is added to all directed advertising here unless it
     * is scannable one.
     *
     * Note. TargetA shall not be also in AUX_CHAIN_IND
     */
    if (aux_data_offset == 0  &&
        (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_DIRECTED) &&
            !(advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_SCANNABLE)) {
        aux->ext_hdr |= (1 << BLE_LL_EXT_ADV_TARGETA_BIT);
        hdr_len += BLE_LL_EXT_ADV_TARGETA_SIZE;
    }

    /* TxPower if configured.
     * Note: TxPower should not be be present in AUX_CHAIN_IND
     */
    if (aux_data_offset == 0 &&
        (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_INC_TX_PWR)) {
        aux->ext_hdr |= (1 << BLE_LL_EXT_ADV_TX_POWER_BIT);
        hdr_len += BLE_LL_EXT_ADV_TX_POWER_SIZE;
    }

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV)
    /* SyncInfo for 1st PDU in chain (i.e. AUX_ADV_IND only) if periodic
     * advertising is enabled
     */
    if (aux_data_offset == 0 && advsm->periodic_adv_active) {
        aux->ext_hdr |= (1 << BLE_LL_EXT_ADV_SYNC_INFO_BIT);
        hdr_len += BLE_LL_EXT_ADV_SYNC_INFO_SIZE;
    }
#endif

    /* if we have any fields in ext header we need to add flags, note that Aux
     * PTR is handled later and it will account for flags if needed
     */
    if (aux->ext_hdr) {
        hdr_len += BLE_LL_EXT_ADV_FLAGS_SIZE;
    }

    /* AdvData always */
    aux->aux_data_len = min(BLE_LL_MAX_PAYLOAD_LEN - hdr_len, rem_aux_data_len);

    /* AuxPtr if there are more AdvData remaining that we can fit here */
    if (chainable && (rem_aux_data_len > aux->aux_data_len)) {
            /* adjust for flags that needs to be added if AuxPtr is only field
             * in Extended Header
             */
            if (!aux->ext_hdr) {
                hdr_len += BLE_LL_EXT_ADV_FLAGS_SIZE;
                aux->aux_data_len -= BLE_LL_EXT_ADV_FLAGS_SIZE;
            }

            aux->ext_hdr |= (1 << BLE_LL_EXT_ADV_AUX_PTR_BIT);
            hdr_len += BLE_LL_EXT_ADV_AUX_PTR_SIZE;
            aux->aux_data_len -= BLE_LL_EXT_ADV_AUX_PTR_SIZE;

            /* PDU payload should be full if chained */
            assert(hdr_len + aux->aux_data_len == BLE_LL_MAX_PAYLOAD_LEN);
    }

    aux->payload_len = hdr_len + aux->aux_data_len;
}

static void
ble_ll_adv_aux_scheduled(struct ble_ll_adv_sm *advsm, uint32_t sch_start,
                         void *arg)
{
    struct ble_ll_adv_aux *aux = arg;

    aux->start_time = sch_start + g_ble_ll_sched_offset_ticks;
}

static void
ble_ll_adv_aux_schedule_next(struct ble_ll_adv_sm *advsm)
{
    struct ble_ll_adv_aux *aux;
    struct ble_ll_adv_aux *aux_next;
    struct ble_ll_sched_item *sch;
    uint16_t rem_aux_data_len;
    uint16_t next_aux_data_offset;
    uint32_t max_usecs;

    assert(advsm->aux_active);

    aux = AUX_CURRENT(advsm);
    aux_next = AUX_NEXT(advsm);

    assert(!aux_next->sch.enqueued);

    /*
     * Do not schedule next aux if current aux is no longer scheduled since we
     * do not have reference time for scheduling.
     */
    if (!aux->sch.enqueued) {
        return;
    }

    /*
     * Do not schedule next aux if current aux does not have AuxPtr in extended
     * header as this means we do not need subsequent ADV_CHAIN_IND to be sent.
     */
    if (!(aux->ext_hdr & (1 << BLE_LL_EXT_ADV_AUX_PTR_BIT))) {
        return;
    }

    next_aux_data_offset = aux->aux_data_offset + aux->aux_data_len;

    assert(AUX_DATA_LEN(advsm) >= next_aux_data_offset);

    rem_aux_data_len = AUX_DATA_LEN(advsm) - next_aux_data_offset;
    assert(rem_aux_data_len > 0);

    ble_ll_adv_aux_calculate(advsm, aux_next, next_aux_data_offset);
    max_usecs = ble_ll_pdu_tx_time_get(aux_next->payload_len, advsm->sec_phy);

    aux_next->start_time = aux->sch.end_time +
                           ble_ll_usecs_to_ticks_round_up(BLE_LL_MAFS + MYNEWT_VAL(BLE_LL_SCHED_AUX_CHAIN_MAFS_DELAY));

    sch = &aux_next->sch;
    sch->start_time = aux_next->start_time - g_ble_ll_sched_offset_ticks;
    sch->remainder = 0;
    sch->end_time = aux_next->start_time +
                    ble_ll_usecs_to_ticks_round_up(max_usecs);
    ble_ll_sched_adv_new(&aux_next->sch, ble_ll_adv_aux_scheduled, aux_next);

    /*
     * In case duration is set for advertising set we need to check if newly
     * scheduled aux will fit inside duration. If not, remove it from scheduler
     * so advertising will stop after current aux.
     */
    if (advsm->duration && (aux_next->sch.end_time > advsm->adv_end_time)) {
        ble_ll_sched_rmv_elem(&aux_next->sch);
    }
}

static void
ble_ll_adv_aux_schedule_first(struct ble_ll_adv_sm *advsm)
{
    struct ble_ll_adv_aux *aux;
    struct ble_ll_sched_item *sch;
    uint32_t max_usecs;

    assert(!advsm->aux_active);
    assert(!advsm->aux[0].sch.enqueued);
    assert(!advsm->aux[1].sch.enqueued);

    advsm->aux_active = 1;
    advsm->aux_index = 0;
    advsm->aux_first_pdu = 1;
    advsm->aux_not_scanned = 0;
    advsm->aux_dropped = 0;

    aux = AUX_CURRENT(advsm);
    ble_ll_adv_aux_calculate(advsm, aux, 0);

    /* Set end time to maximum time this schedule item may take */
    if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_CONNECTABLE) {
        max_usecs = ble_ll_pdu_tx_time_get(aux->payload_len, advsm->sec_phy) +
                    BLE_LL_IFS +
                    /* AUX_CONN_REQ */
                    ble_ll_pdu_tx_time_get(34 + 14, advsm->sec_phy)  +
                    BLE_LL_IFS +
                    /* AUX_CONN_RSP */
                    ble_ll_pdu_tx_time_get(14, advsm->sec_phy);
    } else if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_SCANNABLE) {
        /* For scannable advertising we need to calculate how much time we
         * need for AUX_ADV_IND along with AUX_SCAN_REQ, AUX_SCAN_RSP and
         * IFS in between.
         *
         * Note:
         *  1. aux->payload_len, which calculated by above ble_ll_adv_aux_calulcate(),
         * contains AUX_SCAN_RSP length.
         *  2. length of AUX_ADV_IND is calculated by special function:
         *      ble_ll_adv_aux_scannable_pdu_payload_len()
         */
        max_usecs = ble_ll_pdu_tx_time_get(ble_ll_adv_aux_scannable_pdu_payload_len(advsm),
                                           advsm->sec_phy) +
                    BLE_LL_IFS +
                    /* AUX_SCAN_REQ */
                    ble_ll_pdu_tx_time_get(12, advsm->sec_phy)  +
                    BLE_LL_IFS +
                    /* AUX_SCAN_RSP */
                    ble_ll_pdu_tx_time_get(aux->payload_len, advsm->sec_phy);
    } else {
        max_usecs = ble_ll_pdu_tx_time_get(aux->payload_len, advsm->sec_phy);
    }

    sch = &aux->sch;
    sch->start_time = aux->start_time - g_ble_ll_sched_offset_ticks;
    sch->remainder = 0;
    sch->end_time = aux->start_time + ble_ll_usecs_to_ticks_round_up(max_usecs);
    ble_ll_sched_adv_new(sch, ble_ll_adv_aux_scheduled, aux);
}

static void
ble_ll_adv_aux_set_start_time(struct ble_ll_adv_sm *advsm)
{
    static const uint8_t bits[8] = {0, 1, 1, 2, 1, 2, 2, 3};
    struct ble_ll_sched_item *sched = &advsm->adv_sch;
    uint32_t adv_pdu_dur;
    uint32_t adv_event_dur;
    uint8_t chans;

    assert(!advsm->aux_active);
    assert(!advsm->aux[0].sch.enqueued);
    assert(!advsm->aux[1].sch.enqueued);

    assert(advsm->adv_chanmask > 0 &&
           advsm->adv_chanmask <= BLE_HCI_ADV_CHANMASK_DEF);

    chans = bits[advsm->adv_chanmask];

    /*
     * We want to schedule auxiliary packet as soon as possible after the end
     * of advertising event, but no sooner than T_MAFS. The interval between
     * advertising packets is 250 usecs (8.19 ticks) on LE Coded and a bit less
     * on 1M, but it can vary a bit due to scheduling which we can't really
     * control. Since we round ticks up for both interval and T_MAFS, we still
     * have some margin here. The worst thing that can happen is that we skip
     * last advertising packet which is not a bit problem so leave it as-is, no
     * need to make code more complicated.
     */

    /*
     * XXX: this could be improved if phy has TX-TX transition with controlled
     *      or predefined interval, but since it makes advertising code even
     *      more complicated let's skip it for now...
     */

    adv_pdu_dur = (int32_t)(sched->end_time - sched->start_time) -
                  g_ble_ll_sched_offset_ticks;

    /* 9 is 8.19 ticks rounded up - see comment above */
    adv_event_dur = (adv_pdu_dur * chans) + (9 * (chans - 1));

    advsm->aux[0].start_time = advsm->adv_event_start_time + adv_event_dur +
                               ble_ll_usecs_to_ticks_round_up(BLE_LL_MAFS + MYNEWT_VAL(BLE_LL_SCHED_AUX_MAFS_DELAY));
}

static void
ble_ll_adv_aux_schedule(struct ble_ll_adv_sm *advsm)
{
    /*
     * For secondary channel we always start by scheduling two consecutive
     * auxiliary packets at once. Then, after sending one packet we try to
     * schedule another one as long as there are some data left to send. This
     * is to make sure we can always calculate AuxPtr to subsequent packet
     * without need to scheduled it in an interrupt.
     */

    ble_ll_adv_aux_set_start_time(advsm);
    ble_ll_adv_aux_schedule_first(advsm);
    ble_ll_adv_aux_schedule_next(advsm);

    /*
     * In case duration is set for advertising set we need to check if at least
     * 1st aux will fit inside duration. If not, stop advertising now so we do
     * not start extended advertising event which we cannot finish in time.
     */
    if (advsm->duration &&
            (AUX_CURRENT(advsm)->sch.end_time > advsm->adv_end_time)) {
        ble_ll_adv_sm_stop_timeout(advsm);
    }
}
#endif

/**
 * Called when advertising need to be halted. This normally should not be called
 * and is only called when a scheduled item executes but advertising is still
 * running.
 *
 * Context: Interrupt
 */
void
ble_ll_adv_halt(void)
{
    struct ble_ll_adv_sm *advsm;

    if (g_ble_ll_cur_adv_sm != NULL) {
        advsm = g_ble_ll_cur_adv_sm;

        ble_ll_trace_u32(BLE_LL_TRACE_ID_ADV_HALT, advsm->adv_instance);

        ble_phy_txpwr_set(MYNEWT_VAL(BLE_LL_TX_PWR_DBM));

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV)
        if (advsm->flags & BLE_LL_ADV_SM_FLAG_PERIODIC_SYNC_SENDING) {
            ble_ll_adv_flags_clear(advsm,
                                   BLE_LL_ADV_SM_FLAG_PERIODIC_SYNC_SENDING);
            ble_npl_eventq_put(&g_ble_ll_data.ll_evq,
                               &advsm->adv_periodic_txdone_ev);
            ble_ll_state_set(BLE_LL_STATE_STANDBY);
            g_ble_ll_cur_adv_sm = NULL;
            return;
        }
#endif

        ble_npl_eventq_put(&g_ble_ll_data.ll_evq, &advsm->adv_txdone_ev);
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
        if (!(advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY)) {
            ble_npl_eventq_put(&g_ble_ll_data.ll_evq, &advsm->adv_sec_txdone_ev);
        }
#endif

        ble_ll_state_set(BLE_LL_STATE_STANDBY);
        ble_ll_adv_active_chanset_clear(g_ble_ll_cur_adv_sm);
        g_ble_ll_cur_adv_sm = NULL;
    } else {
        ble_ll_trace_u32(BLE_LL_TRACE_ID_ADV_HALT, UINT32_MAX);
    }
}

/**
 * Called by the HCI command parser when a set advertising parameters command
 * has been received.
 *
 * Context: Link Layer task (HCI command parser)
 *
 * @param cmd
 *
 * @return int
 */
int
ble_ll_adv_set_adv_params(const uint8_t *cmdbuf, uint8_t len)
{
    const struct ble_hci_le_set_adv_params_cp *cmd = (const void *) cmdbuf;
    struct ble_ll_adv_sm *advsm;
    uint8_t adv_filter_policy;
    uint16_t adv_itvl_min;
    uint16_t adv_itvl_max;
    uint16_t props;

    if (len != sizeof(*cmd)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    advsm = &g_ble_ll_adv_sm[0];
    if (advsm->adv_enabled) {
        return BLE_ERR_CMD_DISALLOWED;
    }

    /* Make sure intervals are OK (along with advertising type */
    adv_itvl_min = le16toh(cmd->min_interval);
    adv_itvl_max = le16toh(cmd->max_interval);

    /*
     * Get the filter policy now since we will ignore it if we are doing
     * directed advertising
     */
    adv_filter_policy = cmd->filter_policy;

    switch (cmd->type) {
    case BLE_HCI_ADV_TYPE_ADV_DIRECT_IND_HD:
        adv_filter_policy = BLE_HCI_ADV_FILT_NONE;
        memcpy(advsm->peer_addr, cmd->peer_addr, BLE_DEV_ADDR_LEN);

        /* Ignore min/max interval */
        adv_itvl_min = 0;
        adv_itvl_max = 0;

        props = BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY_HD_DIR ;
        break;
    case BLE_HCI_ADV_TYPE_ADV_DIRECT_IND_LD:
        adv_filter_policy = BLE_HCI_ADV_FILT_NONE;
        memcpy(advsm->peer_addr, cmd->peer_addr, BLE_DEV_ADDR_LEN);

        props = BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY_LD_DIR ;
        break;
    case BLE_HCI_ADV_TYPE_ADV_IND:
        props = BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY_IND;
        break;
    case BLE_HCI_ADV_TYPE_ADV_NONCONN_IND:
        props = BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY_NONCONN;
        break;
    case BLE_HCI_ADV_TYPE_ADV_SCAN_IND:
        props = BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY_SCAN;
        break;
    default:
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    /* Make sure intervals values are valid
     * (HD directed advertising ignores those parameters)
     */
    if (!(props & BLE_HCI_LE_SET_EXT_ADV_PROP_HD_DIRECTED)) {
        if ((adv_itvl_min > adv_itvl_max) ||
                (adv_itvl_min < BLE_HCI_ADV_ITVL_MIN) ||
                (adv_itvl_min > BLE_HCI_ADV_ITVL_MAX) ||
                (adv_itvl_max < BLE_HCI_ADV_ITVL_MIN) ||
                (adv_itvl_max > BLE_HCI_ADV_ITVL_MAX)) {
            return BLE_ERR_INV_HCI_CMD_PARMS;
        }
    }

    if ((cmd->own_addr_type > BLE_HCI_ADV_OWN_ADDR_MAX) ||
        (cmd->peer_addr_type > BLE_HCI_ADV_PEER_ADDR_MAX)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    advsm->adv_txpwr = MYNEWT_VAL(BLE_LL_TX_PWR_DBM);

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
    if (cmd->own_addr_type > BLE_HCI_ADV_OWN_ADDR_RANDOM) {
        /* Copy peer address */
        memcpy(advsm->peer_addr, cmd->peer_addr, BLE_DEV_ADDR_LEN);
    }
#else
    /* If we dont support privacy some address types wont work */
    if (cmd->own_addr_type > BLE_HCI_ADV_OWN_ADDR_RANDOM) {
        return BLE_ERR_UNSUPPORTED;
    }
#endif

    /* There are only three adv channels, so check for any outside the range */
    if (((cmd->chan_map & 0xF8) != 0) || (cmd->chan_map == 0)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    /* Check for valid filter policy */
    if (adv_filter_policy > BLE_HCI_ADV_FILT_MAX) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    /* Fill out rest of advertising state machine */
    advsm->own_addr_type = cmd->own_addr_type;
    advsm->peer_addr_type = cmd->peer_addr_type;
    advsm->adv_filter_policy = adv_filter_policy;
    advsm->adv_chanmask = cmd->chan_map;
    advsm->adv_itvl_min = adv_itvl_min;
    advsm->adv_itvl_max = adv_itvl_max;
    advsm->props = props;

    return 0;
}

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
static void
ble_ll_adv_update_did(struct ble_ll_adv_sm *advsm)
{
    uint16_t old_adi = advsm->adi;

    /*
     * The Advertising DID for a given advertising set shall be initialized
     * with a randomly chosen value. Whenever the Host provides new advertising
     * data or scan response data for a given advertising set (whether it is the
     * same as the previous data or not), the Advertising DID shall be updated.
     * The new value shall be a randomly chosen value that is not the same as
     * the previously used value.
     */
    do {
        advsm->adi = (advsm->adi & 0xf000) | (ble_ll_rand() & 0x0fff);
    } while (old_adi == advsm->adi);
}
#endif

static void
ble_ll_adv_update_adv_scan_rsp_data(struct ble_ll_adv_sm *advsm)
{
    if (!(advsm->flags & BLE_LL_ADV_SM_FLAG_NEW_ADV_DATA) &&
                    !(advsm->flags & BLE_LL_ADV_SM_FLAG_NEW_SCAN_RSP_DATA)) {
        return;
    }

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    if (advsm->aux_active) {
        return;
    }
#endif

    if (advsm->flags & BLE_LL_ADV_SM_FLAG_NEW_ADV_DATA) {
        if (advsm->new_adv_data) {
            os_mbuf_free_chain(advsm->adv_data);
            advsm->adv_data = advsm->new_adv_data;
            advsm->new_adv_data = NULL;
        }
        ble_ll_adv_flags_clear(advsm, BLE_LL_ADV_SM_FLAG_NEW_ADV_DATA);
    } else if (advsm->flags & BLE_LL_ADV_SM_FLAG_NEW_SCAN_RSP_DATA) {
        os_mbuf_free_chain(advsm->scan_rsp_data);
        advsm->scan_rsp_data = advsm->new_scan_rsp_data;
        advsm->new_scan_rsp_data = NULL;
        ble_ll_adv_flags_clear(advsm, BLE_LL_ADV_SM_FLAG_NEW_SCAN_RSP_DATA);
    }

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    /* DID shall be updated when host provides new advertising data */
    ble_ll_adv_update_did(advsm);
#endif
}

/**
 * Stop advertising state machine
 *
 * Context: Link Layer task.
 *
 * @param advsm
 */
static void
ble_ll_adv_sm_stop(struct ble_ll_adv_sm *advsm)
{
    os_sr_t sr;

    if (advsm->adv_enabled) {
        ble_ll_rfmgmt_release();

        /* Remove any scheduled advertising items */
        ble_ll_sched_rmv_elem(&advsm->adv_sch);
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
        advsm->aux_active = 0;
        ble_ll_sched_rmv_elem(&advsm->aux[0].sch);
        ble_ll_sched_rmv_elem(&advsm->aux[1].sch);
#endif

        /* Set to standby if we are no longer advertising */
        OS_ENTER_CRITICAL(sr);
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
        if ((g_ble_ll_cur_adv_sm == advsm) &&
               !(advsm->flags & BLE_LL_ADV_SM_FLAG_PERIODIC_SYNC_SENDING)) {
            ble_phy_disable();
            ble_ll_state_set(BLE_LL_STATE_STANDBY);
            g_ble_ll_cur_adv_sm = NULL;
            ble_ll_scan_chk_resume();
        }
#else
        if (ble_ll_state_get() == BLE_LL_STATE_ADV) {
            ble_phy_disable();
            ble_ll_state_set(BLE_LL_STATE_STANDBY);
            g_ble_ll_cur_adv_sm = NULL;
            ble_ll_scan_chk_resume();
        }
#endif
        OS_EXIT_CRITICAL(sr);

        ble_npl_eventq_remove(&g_ble_ll_data.ll_evq, &advsm->adv_txdone_ev);
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
        ble_npl_eventq_remove(&g_ble_ll_data.ll_evq, &advsm->adv_sec_txdone_ev);
#endif

        /* If there is an event buf we need to free it */
        if (advsm->conn_comp_ev) {
            ble_hci_trans_buf_free(advsm->conn_comp_ev);
            advsm->conn_comp_ev = NULL;
        }

        ble_ll_adv_active_chanset_clear(advsm);

        /* Disable advertising */
        advsm->adv_enabled = 0;

        /* Check if there is outstanding update */
        ble_ll_adv_update_adv_scan_rsp_data(advsm);
    }
}

static void
ble_ll_adv_sm_stop_timeout(struct ble_ll_adv_sm *advsm)
{
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    if (ble_ll_hci_adv_mode_ext()) {
        ble_ll_hci_ev_send_adv_set_terminated(BLE_ERR_DIR_ADV_TMO,
                                                        advsm->adv_instance, 0,
                                                        advsm->events);
    }
#endif

    /*
     * For high duty directed advertising we need to send connection
     * complete event with proper status
     */
    if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_HD_DIRECTED) {
        ble_ll_conn_comp_event_send(NULL, BLE_ERR_DIR_ADV_TMO,
                                    advsm->conn_comp_ev, advsm);
        advsm->conn_comp_ev = NULL;
    }

    /* Disable advertising */
    ble_ll_adv_sm_stop(advsm);
}

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
static void
ble_ll_adv_sm_stop_limit_reached(struct ble_ll_adv_sm *advsm)
{
    ble_ll_hci_ev_send_adv_set_terminated(BLE_ERR_LIMIT_REACHED,
                                          advsm->adv_instance, 0,
                                          advsm->events);

    /*
     * For high duty directed advertising we need to send connection
     * complete event with proper status
     *
     * Spec is a bit unambiguous here since it doesn't define what code should
     * be used if HD directed advertising was terminated before timeout due to
     * events count limit. For now just use same code as with duration timeout.
     */
    if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_HD_DIRECTED) {
        ble_ll_conn_comp_event_send(NULL, BLE_ERR_DIR_ADV_TMO,
                                    advsm->conn_comp_ev, advsm);
        advsm->conn_comp_ev = NULL;
    }

    /* Disable advertising */
    ble_ll_adv_sm_stop(advsm);
}
#endif

static void
ble_ll_adv_scheduled(struct ble_ll_adv_sm *advsm, uint32_t sch_start, void *arg)
{
    /* The event start time is when we start transmission of the adv PDU */
    advsm->adv_event_start_time = sch_start + g_ble_ll_sched_offset_ticks;
    advsm->adv_pdu_start_time = advsm->adv_event_start_time;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    /* this is validated for HD adv so no need to do additional checks here
     * duration is in 10ms units
     */
    if (advsm->duration) {
        advsm->adv_end_time = advsm->adv_event_start_time +
                             os_cputime_usecs_to_ticks(advsm->duration * 10000);
    }
#else
    /* Set the time at which we must end directed, high-duty cycle advertising.
     */
    if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_HD_DIRECTED) {
        advsm->adv_end_time = advsm->adv_event_start_time +
                     os_cputime_usecs_to_ticks(BLE_LL_ADV_STATE_HD_MAX * 1000);
    }
#endif
}

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV)
static uint8_t
ble_ll_adv_sync_pdu_make(uint8_t *dptr, void *pducb_arg, uint8_t *hdr_byte)
{
    struct ble_ll_adv_sm *advsm;
    struct ble_ll_adv_sync *sync;
    uint8_t adv_mode;
    uint8_t pdu_type;
    uint8_t ext_hdr_len;
    uint32_t offset;

    advsm = pducb_arg;
    sync = SYNC_CURRENT(advsm);

    assert(!ble_ll_adv_active_chanset_is_sec(advsm));
    assert(advsm->flags & BLE_LL_ADV_SM_FLAG_PERIODIC_SYNC_SENDING);

    /* It's the same for AUX_SYNC_IND and AUX_CHAIN_IND */
    pdu_type = BLE_ADV_PDU_TYPE_AUX_SYNC_IND;

    /* non-connectable and non-scannable */
    adv_mode = 0;

    ext_hdr_len = sync->payload_len - BLE_LL_EXT_ADV_HDR_LEN - sync->sync_data_len;
    dptr[0] = (adv_mode << 6) | ext_hdr_len;
    dptr += 1;

    /* only put flags if needed */
    if (sync->ext_hdr) {
        dptr[0] = sync->ext_hdr;
        dptr += 1;
    }

    if (sync->ext_hdr & (1 << BLE_LL_EXT_ADV_AUX_PTR_BIT)) {
        if (!SYNC_NEXT(advsm)->sch.enqueued) {
            /*
             * Trim data here in case we do not have next sync scheduled. This
             * can happen if next sync was outside advertising set period and
             * was removed from scheduler.
             */
            offset = 0;
        } else {
            offset = os_cputime_ticks_to_usecs(SYNC_NEXT(advsm)->start_time - sync->start_time);
        }

        ble_ll_adv_put_aux_ptr(SYNC_NEXT(advsm)->chan, advsm->sec_phy,
                               offset, dptr);

        dptr += BLE_LL_EXT_ADV_AUX_PTR_SIZE;
    }

    if (sync->ext_hdr & (1 << BLE_LL_EXT_ADV_TX_POWER_BIT)) {
        dptr[0] = advsm->adv_txpwr + ble_ll_get_tx_pwr_compensation();
        dptr += BLE_LL_EXT_ADV_TX_POWER_SIZE;
    }

    if (sync->sync_data_len) {
        os_mbuf_copydata(advsm->periodic_adv_data, sync->sync_data_offset,
                         sync->sync_data_len, dptr);
    }

    *hdr_byte = pdu_type;

    return sync->payload_len;
}


static void
ble_ll_adv_sync_tx_done(struct ble_ll_adv_sm *advsm)
{
    /* reset power to max after advertising */
    ble_phy_txpwr_set(MYNEWT_VAL(BLE_LL_TX_PWR_DBM));

    /* for sync we trace a no pri nor sec set */
    ble_ll_trace_u32x2(BLE_LL_TRACE_ID_ADV_TXDONE, advsm->adv_instance, 0);

    assert(advsm->flags & BLE_LL_ADV_SM_FLAG_PERIODIC_SYNC_SENDING);
    assert(!ble_ll_adv_active_chanset_is_sec(advsm));

    ble_npl_eventq_put(&g_ble_ll_data.ll_evq, &advsm->adv_periodic_txdone_ev);

    ble_ll_state_set(BLE_LL_STATE_STANDBY);
    ble_ll_adv_flags_clear(advsm, BLE_LL_ADV_SM_FLAG_PERIODIC_SYNC_SENDING);

    /* We no longer have a current state machine */
    g_ble_ll_cur_adv_sm = NULL;
}

/**
 * Called to indicate the advertising sync event is over.
 *
 * Context: Interrupt
 *
 * @param advsm
 *
 */
static void
ble_ll_adv_sync_tx_end(void *arg)
{
    struct ble_ll_adv_sm *advsm = arg;

    ble_ll_adv_sync_tx_done(advsm);

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV_SYNC_TRANSFER)
    /* store last sent periodic counter */
    advsm->periodic_event_cntr_last_sent = advsm->periodic_event_cntr;
#endif
}

static int
ble_ll_adv_sync_tx_start_cb(struct ble_ll_sched_item *sch)
{
    int rc;
    uint32_t txstart;
    struct ble_ll_adv_sm *advsm;
    struct ble_ll_adv_sync *sync;

    /* Get the state machine for the event */
    advsm = (struct ble_ll_adv_sm *)sch->cb_arg;

    /* Set the current advertiser */
    g_ble_ll_cur_adv_sm = advsm;

    ble_ll_adv_active_chanset_clear(advsm);
    ble_ll_adv_flags_set(advsm, BLE_LL_ADV_SM_FLAG_PERIODIC_SYNC_SENDING);

    /* Set the power */
    ble_phy_txpwr_set(advsm->adv_txpwr);

    /* Set channel */
    sync = SYNC_CURRENT(advsm);
    rc = ble_phy_setchan(sync->chan, advsm->periodic_access_addr,
                         advsm->periodic_crcinit);

    assert(rc == 0);

#if (BLE_LL_BT5_PHY_SUPPORTED == 1)
    /* Set phy mode */
     ble_phy_mode_set(advsm->sec_phy, advsm->sec_phy);
#endif

    /* Set transmit start time. */
    txstart = sch->start_time + g_ble_ll_sched_offset_ticks;
    rc = ble_phy_tx_set_start_time(txstart, sch->remainder);
    if (rc) {
        STATS_INC(ble_ll_stats, adv_late_starts);
        goto adv_tx_done;
    }

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
    ble_phy_encrypt_disable();
#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
    ble_phy_resolv_list_disable();
#endif

    /* Transmit advertisement */
    ble_phy_set_txend_cb(ble_ll_adv_sync_tx_end, advsm);
    rc = ble_phy_tx(ble_ll_adv_sync_pdu_make, advsm, BLE_PHY_TRANSITION_NONE);
    if (rc) {
        goto adv_tx_done;
    }

    /* disable whitelisting, we are always non-connectable non-scannable */
    ble_ll_whitelist_disable();

    /* Set link layer state to advertising */
    ble_ll_state_set(BLE_LL_STATE_ADV);

    /* Count # of adv. sent */
    STATS_INC(ble_ll_stats, adv_txg);

    return BLE_LL_SCHED_STATE_RUNNING;

adv_tx_done:
    ble_ll_adv_sync_tx_done(advsm);
    return BLE_LL_SCHED_STATE_DONE;
}

static void
ble_ll_adv_sync_calculate(struct ble_ll_adv_sm *advsm,
                          struct ble_ll_adv_sync *sync,
                          uint16_t sync_data_offset,
                          uint8_t chan)
{
    uint16_t rem_sync_data_len;
    uint8_t hdr_len;

    assert(!sync->sch.enqueued);
    assert((SYNC_DATA_LEN(advsm) > sync_data_offset) ||
           (SYNC_DATA_LEN(advsm) == 0 && sync_data_offset == 0));

    sync->sync_data_offset = sync_data_offset;
    sync->sync_data_len = 0;
    sync->payload_len = 0;
    sync->ext_hdr = 0;
    sync->chan = chan;

    rem_sync_data_len = SYNC_DATA_LEN(advsm) - sync_data_offset;

    hdr_len = BLE_LL_EXT_ADV_HDR_LEN;

    /* TxPower if configured
     * Note: TxPower shall not be present in chain PDU for SYNC
     */
    if (sync_data_offset == 0 &&
        (advsm->periodic_adv_props & BLE_HCI_LE_SET_PERIODIC_ADV_PROP_INC_TX_PWR)) {
        sync->ext_hdr |= (1 << BLE_LL_EXT_ADV_TX_POWER_BIT);
        hdr_len += BLE_LL_EXT_ADV_TX_POWER_SIZE;
    }

    /* if we have any fields in ext header we need to add flags, note that Aux
     * PTR is handled later and it will account for flags if needed
     *
     * This could be handled inside TxPower but lets keep code consistent with
     * how Aux calculate works and this also make it easier to add more fields
     * into flags if needed in future
     */
    if (sync->ext_hdr) {
        hdr_len += BLE_LL_EXT_ADV_FLAGS_SIZE;
    }

    /* AdvData always */
    sync->sync_data_len = min(BLE_LL_MAX_PAYLOAD_LEN - hdr_len, rem_sync_data_len);

    /* AuxPtr if there are more AdvData remaining that we can fit here */
    if ((rem_sync_data_len > sync->sync_data_len)) {
            /* adjust for flags that needs to be added if AuxPtr is only field
             * in Extended Header
             */
            if (!sync->ext_hdr) {
                hdr_len += BLE_LL_EXT_ADV_FLAGS_SIZE;
                sync->sync_data_len -= BLE_LL_EXT_ADV_FLAGS_SIZE;
            }

            sync->ext_hdr |= (1 << BLE_LL_EXT_ADV_AUX_PTR_BIT);
            hdr_len += BLE_LL_EXT_ADV_AUX_PTR_SIZE;
            sync->sync_data_len -= BLE_LL_EXT_ADV_AUX_PTR_SIZE;

            /* PDU payload should be full if chained */
            assert(hdr_len + sync->sync_data_len == BLE_LL_MAX_PAYLOAD_LEN);
    }

    sync->payload_len = hdr_len + sync->sync_data_len;
}

static void
ble_ll_adv_periodic_schedule_first(struct ble_ll_adv_sm *advsm,
                                   bool first_pdu)
{
    struct ble_ll_adv_sync *sync;
    struct ble_ll_sched_item *sch;
    uint32_t sch_start;
    uint32_t max_usecs;
    uint8_t chan;
    int rc;

    assert(!advsm->periodic_sync_active);
    assert(!advsm->periodic_sync[0].sch.enqueued);
    assert(!advsm->periodic_sync[1].sch.enqueued);

    advsm->periodic_sync_active = 1;
    advsm->periodic_sync_index = 0;

    sync = SYNC_CURRENT(advsm);

    /* For first SYNC packet in chain we use separate CSA#2 state to maintain
     * freq hopping as advertised in SyncInfo
     *
     * Preincrement event counter as we later send this in PDU so make sure
     * same values are used
     */
    chan = ble_ll_utils_calc_dci_csa2(++advsm->periodic_event_cntr,
                                      advsm->periodic_channel_id,
                                      advsm->periodic_num_used_chans,
                                      advsm->periodic_chanmap);

    ble_ll_adv_sync_calculate(advsm, sync, 0, chan);

    /* sync is always non-connectable and non-scannable*/
    max_usecs = ble_ll_pdu_tx_time_get(sync->payload_len, advsm->sec_phy);

    sch = &sync->sch;

    advsm->periodic_adv_event_start_time_remainder += advsm->periodic_adv_itvl_rem_usec;
    if (advsm->periodic_adv_event_start_time_remainder >= 31) {
        advsm->periodic_adv_event_start_time++;
        advsm->periodic_adv_event_start_time_remainder -= 31;
    }

    sch->start_time = advsm->periodic_adv_event_start_time;
    sch->remainder = advsm->periodic_adv_event_start_time_remainder;
    sch->end_time = sch->start_time + ble_ll_usecs_to_ticks_round_up(max_usecs);
    sch->start_time -= g_ble_ll_sched_offset_ticks;

    rc = ble_ll_sched_periodic_adv(sch, &sch_start, first_pdu);
    if (rc) {
        STATS_INC(ble_ll_stats, periodic_adv_drop_event);
        ble_npl_eventq_put(&g_ble_ll_data.ll_evq,
                           &advsm->adv_periodic_txdone_ev);
        return;
    }

    sync->start_time = sch_start + g_ble_ll_sched_offset_ticks;

    assert(first_pdu ||
           (sync->start_time == advsm->periodic_adv_event_start_time));

    /* The event start time is when we start transmission of the SYNC PDU */
    advsm->periodic_adv_event_start_time = sync->start_time;
}

static void
ble_ll_adv_sync_next_scheduled(struct ble_ll_adv_sm *advsm, uint32_t sch_start,
                         void *arg)
{
    struct ble_ll_adv_sync *sync = arg;

    sync->start_time = sch_start + g_ble_ll_sched_offset_ticks;
}

static void
ble_ll_adv_periodic_schedule_next(struct ble_ll_adv_sm *advsm)
{
    struct ble_ll_adv_sync *sync;
    struct ble_ll_adv_sync *sync_next;
    struct ble_ll_sched_item *sch;
    uint16_t rem_sync_data_len;
    uint16_t next_sync_data_offset;
    uint32_t max_usecs;
    uint8_t chan;

    assert(advsm->periodic_sync_active);

    sync = SYNC_CURRENT(advsm);
    sync_next = SYNC_NEXT(advsm);

    assert(!sync_next->sch.enqueued);

    /*
     * Do not schedule next sync if current sync is no longer scheduled since we
     * do not have reference time for scheduling.
     */
    if (!sync->sch.enqueued) {
        return;
    }

    /*
     * Do not schedule next sync if current sync does not have AuxPtr in extended
     * header as this means we do not need subsequent ADV_CHAIN_IND to be sent.
     */
    if (!(sync->ext_hdr & (1 << BLE_LL_EXT_ADV_AUX_PTR_BIT))) {
        return;
    }

    next_sync_data_offset = sync->sync_data_offset + sync->sync_data_len;

    assert(SYNC_DATA_LEN(advsm) >= next_sync_data_offset);

    rem_sync_data_len = SYNC_DATA_LEN(advsm) - next_sync_data_offset;
    assert(rem_sync_data_len > 0);

    /* we use separate counter for chaining */
    chan = ble_ll_utils_calc_dci_csa2(advsm->periodic_chain_event_cntr++,
                                      advsm->periodic_channel_id,
                                      advsm->periodic_num_used_chans,
                                      advsm->periodic_chanmap);

    ble_ll_adv_sync_calculate(advsm, sync_next, next_sync_data_offset, chan);
    max_usecs = ble_ll_pdu_tx_time_get(sync_next->payload_len, advsm->sec_phy);

    sync_next->start_time = sync->sch.end_time +
                            ble_ll_usecs_to_ticks_round_up(BLE_LL_MAFS + MYNEWT_VAL(BLE_LL_SCHED_AUX_CHAIN_MAFS_DELAY));

    sch = &sync_next->sch;
    sch->start_time = sync_next->start_time - g_ble_ll_sched_offset_ticks;

    /* adjust for previous packets remainder */
    sch->remainder = sync->sch.remainder;
    sch->end_time = sync_next->start_time +
                    ble_ll_usecs_to_ticks_round_up(max_usecs);

    /* here we can use ble_ll_sched_adv_new as we don't care about timing */
    ble_ll_sched_adv_new(&sync_next->sch, ble_ll_adv_sync_next_scheduled,
                         sync_next);

    /* if we are pass advertising interval, drop chain */
    if (sch->end_time > advsm->periodic_adv_event_start_time +
            advsm->periodic_adv_itvl_ticks) {
        STATS_INC(ble_ll_stats, periodic_chain_drop_event);
        ble_ll_sched_rmv_elem(&sync->sch);
        ble_npl_eventq_put(&g_ble_ll_data.ll_evq,
                           &advsm->adv_periodic_txdone_ev);
    }
}

static void
ble_ll_adv_sync_schedule(struct ble_ll_adv_sm *advsm, bool first_pdu)
{
    /*
     * For secondary channel we always start by scheduling two consecutive
     * auxiliary packets at once. Then, after sending one packet we try to
     * schedule another one as long as there are some data left to send. This
     * is to make sure we can always calculate AuxPtr to subsequent packet
     * without need to scheduled it in an interrupt.
     */

    ble_ll_adv_periodic_schedule_first(advsm, first_pdu);
    ble_ll_adv_periodic_schedule_next(advsm);
}

static void
ble_ll_adv_reschedule_periodic_event(struct ble_ll_adv_sm *advsm)
{
    advsm->periodic_adv_event_start_time += advsm->periodic_adv_itvl_ticks;
    ble_ll_adv_sync_schedule(advsm, false);
}

static void
ble_ll_adv_update_periodic_data(struct ble_ll_adv_sm *advsm)
{
    if (!(advsm->flags & BLE_LL_ADV_SM_FLAG_PERIODIC_NEW_DATA)) {
        return;
    }

    if (advsm->periodic_sync_active) {
        return;
    }

    if (advsm->periodic_new_data) {
        os_mbuf_free_chain(advsm->periodic_adv_data);
        advsm->periodic_adv_data = advsm->periodic_new_data;
        advsm->periodic_new_data = NULL;
    }

    ble_ll_adv_flags_clear(advsm, BLE_LL_ADV_SM_FLAG_PERIODIC_NEW_DATA);
}

/**
 * Called when periodic packet is txd on secondary channel
 *
 * Context: Link Layer task.
 *
 * @param ev
 */
static void
ble_ll_adv_periodic_done(struct ble_ll_adv_sm *advsm)
{
    struct ble_ll_adv_sync *sync;
    struct ble_ll_adv_sync *sync_next;

    assert(advsm->periodic_adv_enabled);
    assert(advsm->periodic_adv_active);
    assert(advsm->periodic_sync_active);

    ble_ll_rfmgmt_release();

    sync = SYNC_CURRENT(advsm);
    sync_next = SYNC_NEXT(advsm);

    /* Remove anything else scheduled for periodic */
    ble_ll_sched_rmv_elem(&sync->sch);
    ble_npl_eventq_remove(&g_ble_ll_data.ll_evq, &advsm->adv_periodic_txdone_ev);

    /* If we have next SYNC scheduled, try to schedule another one */
    if (sync_next->sch.enqueued) {
        advsm->periodic_sync_index ^= 1;
        ble_ll_adv_periodic_schedule_next(advsm);
        return;
    }

    /* Check if we need to resume scanning */
    ble_ll_scan_chk_resume();

    advsm->periodic_sync_active = 0;
    ble_ll_adv_update_periodic_data(advsm);
    ble_ll_adv_reschedule_periodic_event(advsm);
}

static void
ble_ll_adv_periodic_event_done(struct ble_npl_event *ev)
{
    ble_ll_adv_periodic_done(ble_npl_event_get_arg(ev));
}

static void
ble_ll_adv_sm_start_periodic(struct ble_ll_adv_sm *advsm)
{
    uint32_t usecs;
    uint32_t ticks;

    /*
     * The Advertising DID is not required to change when a SyncInfo field is
     * added to or removed from an advertising set. However, if it does not
     * change, then scanners may fail to synchronize to periodic advertising
     * because entries in the Advertising DID cache (see Section 4.3.3) mean
     * they ignore the advertisements containing the SyncInfo field. Therefore,
     * advertisers should update the Advertising DID when a periodic advertising
     * train is enabled.
     */
    ble_ll_adv_update_did(advsm);

    advsm->periodic_adv_active = 1;

    /* keep channel map since we cannot change it later on */
    memcpy(advsm->periodic_chanmap, g_ble_ll_conn_params.master_chan_map,
           BLE_LL_CONN_CHMAP_LEN);
    advsm->periodic_num_used_chans = g_ble_ll_conn_params.num_used_chans;
    advsm->periodic_event_cntr = 0;
    /* for chaining we start with random counter as we share access addr */
    advsm->periodic_chain_event_cntr = ble_ll_rand();
    advsm->periodic_access_addr = ble_ll_utils_calc_access_addr();
    advsm->periodic_channel_id = ((advsm->periodic_access_addr & 0xffff0000) >> 16) ^
                                 (advsm->periodic_access_addr & 0x0000ffff);
    advsm->periodic_crcinit = ble_ll_rand() & 0xffffff;

    usecs = (uint32_t)advsm->periodic_adv_itvl_max * BLE_LL_ADV_PERIODIC_ITVL;
    ticks = os_cputime_usecs_to_ticks(usecs);

    advsm->periodic_adv_itvl_rem_usec = (usecs - os_cputime_ticks_to_usecs(ticks));
    if (advsm->periodic_adv_itvl_rem_usec == 31) {
        advsm->periodic_adv_itvl_rem_usec = 0;
        ticks++;
    }
    advsm->periodic_adv_itvl_ticks = ticks;

    /* There is no point in starting periodic advertising until next advertising
     * event since SyncInfo is needed for synchronization
     */
    advsm->periodic_adv_event_start_time_remainder = 0;
    advsm->periodic_adv_event_start_time = advsm->adv_pdu_start_time +
                      os_cputime_usecs_to_ticks(advsm->adv_itvl_usecs + 5000);

    ble_ll_adv_sync_schedule(advsm, true);
}

static void
ble_ll_adv_sm_stop_periodic(struct ble_ll_adv_sm *advsm)
{
    os_sr_t sr;

    ble_ll_rfmgmt_release();

    if (!advsm->periodic_adv_active) {
        return;
    }

    /*
     * The Advertising DID is not required to change when a SyncInfo field is
     * added to or removed from an advertising set. However, if it does not
     * change, then scanners may unnecessary try to synchronize to instance that
     * no longer has periodic advertising enabled  because entries in the
     * Advertising DID cache (see Section 4.3.3) mean they ignore the
     * advertisements no longer containing the SyncInfo field. Therefore,
     * advertisers should update the Advertising DID when a periodic advertising
     * train is disabled.
     */
    ble_ll_adv_update_did(advsm);

    /* Remove any scheduled advertising items */
    advsm->periodic_adv_active = 0;
    advsm->periodic_sync_active = 0;
    ble_ll_sched_rmv_elem(&advsm->periodic_sync[0].sch);
    ble_ll_sched_rmv_elem(&advsm->periodic_sync[1].sch);

    /* Set to standby if we are no longer advertising */
    OS_ENTER_CRITICAL(sr);
    if ((g_ble_ll_cur_adv_sm == advsm) &&
            (advsm->flags & BLE_LL_ADV_SM_FLAG_PERIODIC_SYNC_SENDING)) {
        ble_phy_disable();
        ble_ll_state_set(BLE_LL_STATE_STANDBY);
        g_ble_ll_cur_adv_sm = NULL;
        ble_ll_scan_chk_resume();
    }
    OS_EXIT_CRITICAL(sr);

    ble_ll_adv_flags_clear(advsm, BLE_LL_ADV_SM_FLAG_PERIODIC_SYNC_SENDING);

    ble_npl_eventq_remove(&g_ble_ll_data.ll_evq,
                          &advsm->adv_periodic_txdone_ev);

    ble_ll_adv_update_periodic_data(advsm);
}
#endif

/**
 * Start the advertising state machine. This is called when the host sends
 * the "enable advertising" command and is not called again while in the
 * advertising state.
 *
 * Context: Link-layer task.
 *
 * @param advsm Pointer to advertising state machine
 *
 * @return int
 */
static int
ble_ll_adv_sm_start(struct ble_ll_adv_sm *advsm)
{
    uint8_t adv_chan;
    uint8_t *addr;
    uint8_t *evbuf;
    uint32_t start_delay_us;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CSA2)
    uint32_t access_addr;
#endif
    const uint8_t *random_addr;
    uint32_t earliest_start_time;
    int32_t delta;

    /* only clear flags that are not set from HCI */
    ble_ll_adv_flags_clear(advsm, BLE_LL_ADV_SM_FLAG_TX_ADD |
                                  BLE_LL_ADV_SM_FLAG_RX_ADD |
                                  BLE_LL_ADV_SM_FLAG_CONN_RSP_TXD);

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    random_addr = advsm->adv_random_addr;
#else
    random_addr = g_random_addr;
#endif

    if (!ble_ll_is_valid_own_addr_type(advsm->own_addr_type, random_addr)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    /*
     * Get an event with which to send the connection complete event if
     * this is connectable
     */
    if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_CONNECTABLE) {
        /* We expect this to be NULL but if not we wont allocate one... */
        if (advsm->conn_comp_ev == NULL) {
            evbuf = ble_hci_trans_buf_alloc(BLE_HCI_TRANS_BUF_EVT_HI);
            if (!evbuf) {
                return BLE_ERR_MEM_CAPACITY;
            }
            advsm->conn_comp_ev = evbuf;
        }
    }

    /* Set advertising address */
    if ((advsm->own_addr_type & 1) == 0) {
        addr = g_dev_addr;
    } else {
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
        addr = advsm->adv_random_addr;
#else
        addr = g_random_addr;
#endif
        advsm->flags |= BLE_LL_ADV_SM_FLAG_TX_ADD;
    }
    memcpy(advsm->adva, addr, BLE_DEV_ADDR_LEN);

    if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_DIRECTED) {
        memcpy(advsm->initiator_addr, advsm->peer_addr, BLE_DEV_ADDR_LEN);
        if (advsm->peer_addr_type & 1) {
            advsm->flags |= BLE_LL_ADV_SM_FLAG_RX_ADD;
        }
    }

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
    /* This will generate an RPA for both initiator addr and adva */
    if (advsm->own_addr_type > BLE_HCI_ADV_OWN_ADDR_RANDOM) {
        ble_ll_adv_rpa_update(advsm);
    }
#endif

    /* Set flag telling us that advertising is enabled */
    advsm->adv_enabled = 1;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CSA2)
    advsm->event_cntr = 0;
    access_addr = ble_ll_utils_calc_access_addr();
    advsm->channel_id = ((access_addr & 0xffff0000) >> 16) ^
                         (access_addr & 0x0000ffff);
#endif

    /* Determine the advertising interval we will use */
    if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_HD_DIRECTED) {
        /* Set it to max. allowed for high duty cycle advertising */
        advsm->adv_itvl_usecs = BLE_LL_ADV_PDU_ITVL_HD_MS_MAX;
    } else {
        advsm->adv_itvl_usecs = (uint32_t)advsm->adv_itvl_max;
        advsm->adv_itvl_usecs *= BLE_LL_ADV_ITVL;
    }

    /* Set first advertising channel */
    adv_chan = ble_ll_adv_first_chan(advsm);
    advsm->adv_chan = adv_chan;

    /*
     * Scheduling 1st PDU is a bit tricky.
     * Earliest possible start time is after RF is enabled so just force RF to
     * start here to see when if will be fully enabled - it will be too early,
     * but this is the only reliable way to have it enabled on time.
     * Next we calculate expected start time (randomize it a bit) and this is
     * used to setup start time for scheduler item.
     * Then we check if start time for scheduler item (which includes scheduler
     * overhead) is no earlier than calculated earliest possible start time and
     * adjust scheduler item if necessary.
     */
    earliest_start_time = ble_ll_rfmgmt_enable_now();

    start_delay_us = ble_ll_rand() % (BLE_LL_ADV_DELAY_MS_MAX * 1000);
    advsm->adv_pdu_start_time = os_cputime_get32() +
                                os_cputime_usecs_to_ticks(start_delay_us);

    ble_ll_adv_set_sched(advsm);

    delta = (int32_t)(advsm->adv_sch.start_time - earliest_start_time);
    if (delta < 0) {
        advsm->adv_sch.start_time -= delta;
        advsm->adv_sch.end_time -= delta;
    }

    /* This does actual scheduling */
    ble_ll_sched_adv_new(&advsm->adv_sch, ble_ll_adv_scheduled, NULL);

    /* we start periodic before AE since we need PDU start time in SyncInfo */
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV)
    if (advsm->periodic_adv_enabled && !advsm->periodic_adv_active) {
        ble_ll_adv_sm_start_periodic(advsm);
    }
#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    if (!(advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY)) {
        ble_ll_adv_aux_schedule(advsm);
    }
#endif

    return BLE_ERR_SUCCESS;
}

/**
 * Called when the LE HCI command read advertising channel tx power command
 * has been received. Returns the current advertising transmit power.
 *
 * Context: Link Layer task (HCI command parser)
 *
 * @return int
 */
int
ble_ll_adv_read_txpwr(uint8_t *rspbuf, uint8_t *rsplen)
{
    struct ble_hci_le_rd_adv_chan_txpwr_rp *rsp = (void *) rspbuf;

    rsp->power_level = MYNEWT_VAL(BLE_LL_TX_PWR_DBM);

    *rsplen = sizeof(*rsp);
    return BLE_ERR_SUCCESS;
}

/**
 * Turn advertising on/off.
 *
 * Context: Link Layer task
 *
 * @param cmd
 *
 * @return int
 */
static int
ble_ll_adv_set_enable(uint8_t instance, uint8_t enable, int duration,
                      uint8_t events)
{
    int rc;
    struct ble_ll_adv_sm *advsm;

    advsm = ble_ll_adv_sm_find_configured(instance);
    if (!advsm) {
        return BLE_ERR_UNK_ADV_INDENT;
    }

    rc = BLE_ERR_SUCCESS;
    if (enable == 1) {
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
        if (advsm->flags & BLE_LL_ADV_SM_FLAG_ADV_DATA_INCOMPLETE) {
            return BLE_ERR_CMD_DISALLOWED;
        }

        if (ble_ll_hci_adv_mode_ext() &&
                (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_SCANNABLE) &&
                !(advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY) &&
                SCAN_RSP_DATA_LEN(advsm) == 0) {
            return BLE_ERR_CMD_DISALLOWED;
        }

        /* handle specifics of HD dir adv enabled in legacy way */
        if (duration < 0) {
            if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_HD_DIRECTED) {
                duration = BLE_LL_ADV_STATE_HD_MAX / 10;
            } else {
                duration = 0;
            }
        }
        advsm->duration = duration;
        advsm->events_max = events;
        advsm->events = 0;
#endif

        /* If already enabled, do nothing */
        if (!advsm->adv_enabled) {
            /* Start the advertising state machine */
            rc = ble_ll_adv_sm_start(advsm);
        }
    } else if (enable == 0) {
        ble_ll_adv_sm_stop(advsm);
    } else {
        rc = BLE_ERR_INV_HCI_CMD_PARMS;
    }

    return rc;
}

int
ble_ll_hci_adv_set_enable(const uint8_t *cmdbuf, uint8_t len)
{
    const struct ble_hci_le_set_adv_enable_cp *cmd = (const void *) cmdbuf;

    if (len != sizeof(*cmd)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    return ble_ll_adv_set_enable(0, cmd->enable, -1, 0);
}

static void
ble_ll_adv_update_data_mbuf(struct os_mbuf **omp, bool new_data, uint16_t maxlen,
                            const void *data, uint16_t datalen)
{
    struct os_mbuf *om;
    int ret;

    om = *omp;

    if (new_data) {
        if (om) {
            os_mbuf_free_chain(om);
        }

        om = os_msys_get_pkthdr(datalen, 0);
        if (!om) {
            goto done;
        }
    }

    assert(om);

    if (OS_MBUF_PKTLEN(om) + datalen > maxlen) {
        os_mbuf_free_chain(om);
        om = NULL;
        goto done;
    }

    ret = os_mbuf_append(om, data, datalen);
    if (ret) {
        os_mbuf_free_chain(om);
        om = NULL;
    }

done:
    *omp = om;
}

/**
 * Set the scan response data that the controller will send.
 *
 * @param cmd
 * @param len
 *
 * @return int
 */
static int
ble_ll_adv_set_scan_rsp_data(const uint8_t *data, uint8_t datalen,
                             uint8_t instance, uint8_t operation)
{
    struct ble_ll_adv_sm *advsm;
    bool new_data;

    advsm = ble_ll_adv_sm_find_configured(instance);
    if (!advsm) {
        return BLE_ERR_UNK_ADV_INDENT;
    }

    /* check if type of advertising support scan rsp */
    if (!(advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_SCANNABLE)) {
        if (!(advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY)) {
            return BLE_ERR_INV_HCI_CMD_PARMS;
        }
    }

    switch (operation) {
    case BLE_HCI_LE_SET_DATA_OPER_COMPLETE:
        if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY) {
            if (datalen > BLE_SCAN_RSP_LEGACY_DATA_MAX_LEN) {
                return BLE_ERR_INV_HCI_CMD_PARMS;
            }
        }

        break;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    case BLE_HCI_LE_SET_DATA_OPER_LAST:
        /* TODO mark scan rsp as complete? */
        /* fall through */
    case BLE_HCI_LE_SET_DATA_OPER_INT:
        if (!advsm->scan_rsp_data) {
            return BLE_ERR_INV_HCI_CMD_PARMS;
        }

        if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY) {
            return BLE_ERR_INV_HCI_CMD_PARMS;
        }

        if (advsm->adv_enabled) {
            return BLE_ERR_CMD_DISALLOWED;
        }

        if (!datalen) {
            return BLE_ERR_INV_HCI_CMD_PARMS;
        }
        break;
    case BLE_HCI_LE_SET_DATA_OPER_FIRST:
        if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY) {
            return BLE_ERR_INV_HCI_CMD_PARMS;
        }

        if (advsm->adv_enabled) {
            return BLE_ERR_CMD_DISALLOWED;
        }

        if (!datalen) {
            return BLE_ERR_INV_HCI_CMD_PARMS;
        }

        break;
#endif
    default:
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    new_data = (operation == BLE_HCI_LE_SET_DATA_OPER_COMPLETE) ||
               (operation == BLE_HCI_LE_SET_DATA_OPER_FIRST);

    if (advsm->adv_enabled) {
        if (advsm->new_scan_rsp_data) {
            ble_ll_adv_flags_clear(advsm, BLE_LL_ADV_SM_FLAG_NEW_SCAN_RSP_DATA);
            os_mbuf_free_chain(advsm->new_scan_rsp_data);
            advsm->new_scan_rsp_data = NULL;
        }

        ble_ll_adv_update_data_mbuf(&advsm->new_scan_rsp_data, new_data,
                                    BLE_ADV_DATA_MAX_LEN, data, datalen);
        if (!advsm->new_scan_rsp_data) {
            return BLE_ERR_MEM_CAPACITY;
        }
        ble_ll_adv_flags_set(advsm, BLE_LL_ADV_SM_FLAG_NEW_SCAN_RSP_DATA);
    } else {
        ble_ll_adv_update_data_mbuf(&advsm->scan_rsp_data, new_data,
                                    BLE_SCAN_RSP_DATA_MAX_LEN, data, datalen);
        if (!advsm->scan_rsp_data) {
            return BLE_ERR_MEM_CAPACITY;
        }

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
        /* DID shall be updated when host provides new scan response data */
        ble_ll_adv_update_did(advsm);
#endif
    }

    return BLE_ERR_SUCCESS;
}

int
ble_ll_hci_set_scan_rsp_data(const uint8_t *cmdbuf, uint8_t len)
{
    const struct ble_hci_le_set_scan_rsp_data_cp *cmd = (const void *) cmdbuf;

    if ((len != sizeof(*cmd)) || (cmd->scan_rsp_len > sizeof(cmd->scan_rsp))) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    return ble_ll_adv_set_scan_rsp_data(cmd->scan_rsp, cmd->scan_rsp_len, 0,
                                        BLE_HCI_LE_SET_DATA_OPER_COMPLETE);
}
/**
 * Called by the LL HCI command parser when a set advertising
 * data command has been sent from the host to the controller.
 *
 * @param cmd Pointer to command data
 * @param len Length of command data
 *
 * @return int 0: success; BLE_ERR_INV_HCI_CMD_PARMS otherwise.
 */
static int
ble_ll_adv_set_adv_data(const uint8_t *data, uint8_t datalen, uint8_t instance,
                        uint8_t operation)
{
    struct ble_ll_adv_sm *advsm;
    bool new_data;

    advsm = ble_ll_adv_sm_find_configured(instance);
    if (!advsm) {
        return BLE_ERR_UNK_ADV_INDENT;
    }

    /* check if type of advertising support adv data */
    if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY) {
        if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_DIRECTED) {
            if (ble_ll_hci_adv_mode_ext()) {
                return BLE_ERR_INV_HCI_CMD_PARMS;
            }
        }
    } else {
        if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_SCANNABLE) {
            return BLE_ERR_INV_HCI_CMD_PARMS;
        }
    }

    switch (operation) {
    case BLE_HCI_LE_SET_DATA_OPER_COMPLETE:
        if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY) {
            if (datalen > BLE_ADV_LEGACY_DATA_MAX_LEN) {
                return BLE_ERR_INV_HCI_CMD_PARMS;
            }
        }

        ble_ll_adv_flags_clear(advsm, BLE_LL_ADV_SM_FLAG_ADV_DATA_INCOMPLETE);

        break;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    case BLE_HCI_LE_SET_DATA_OPER_UNCHANGED:
        if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY) {
            return BLE_ERR_INV_HCI_CMD_PARMS;
        }

        if (!advsm->adv_enabled || !ADV_DATA_LEN(advsm) || datalen) {
            return BLE_ERR_INV_HCI_CMD_PARMS;
        }

        /* update DID only */
        ble_ll_adv_update_did(advsm);
        return BLE_ERR_SUCCESS;
    case BLE_HCI_LE_SET_DATA_OPER_LAST:
        ble_ll_adv_flags_clear(advsm, BLE_LL_ADV_SM_FLAG_ADV_DATA_INCOMPLETE);
        /* fall through */
    case BLE_HCI_LE_SET_DATA_OPER_INT:
        if (!advsm->adv_data) {
            return BLE_ERR_INV_HCI_CMD_PARMS;
        }

        if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY) {
            return BLE_ERR_INV_HCI_CMD_PARMS;
        }

        if (!datalen) {
            return BLE_ERR_INV_HCI_CMD_PARMS;
        }

        if (advsm->adv_enabled) {
            return BLE_ERR_CMD_DISALLOWED;
        }
        break;
    case BLE_HCI_LE_SET_DATA_OPER_FIRST:
        if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY) {
            return BLE_ERR_INV_HCI_CMD_PARMS;
        }

        if (advsm->adv_enabled) {
            return BLE_ERR_CMD_DISALLOWED;
        }

        if (!datalen) {
            return BLE_ERR_INV_HCI_CMD_PARMS;
        }

        ble_ll_adv_flags_set(advsm, BLE_LL_ADV_SM_FLAG_ADV_DATA_INCOMPLETE);
        break;
#endif
    default:
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    new_data = (operation == BLE_HCI_LE_SET_DATA_OPER_COMPLETE) ||
               (operation == BLE_HCI_LE_SET_DATA_OPER_FIRST);

    if (advsm->adv_enabled) {
        if (advsm->new_adv_data) {
            ble_ll_adv_flags_clear(advsm, BLE_LL_ADV_SM_FLAG_NEW_ADV_DATA);
            os_mbuf_free_chain(advsm->new_adv_data);
            advsm->new_adv_data = NULL;
        }

        ble_ll_adv_update_data_mbuf(&advsm->new_adv_data, new_data,
                                    BLE_ADV_DATA_MAX_LEN, data, datalen);
        if (!advsm->new_adv_data) {
            return BLE_ERR_MEM_CAPACITY;
        }
        ble_ll_adv_flags_set(advsm, BLE_LL_ADV_SM_FLAG_NEW_ADV_DATA);
    } else {
        ble_ll_adv_update_data_mbuf(&advsm->adv_data, new_data,
                                    BLE_ADV_DATA_MAX_LEN, data, datalen);
        if (!advsm->adv_data) {
            return BLE_ERR_MEM_CAPACITY;
        }

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
        /* DID shall be updated when host provides new advertising data */
        ble_ll_adv_update_did(advsm);
#endif
        }

    return BLE_ERR_SUCCESS;
}

int
ble_ll_hci_set_adv_data(const uint8_t *cmdbuf, uint8_t len)
{
    const struct ble_hci_le_set_adv_data_cp *cmd = (const void *) cmdbuf;

    if ((len != sizeof(*cmd)) || (cmd->adv_data_len > sizeof(cmd->adv_data))) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    return ble_ll_adv_set_adv_data(cmd->adv_data, cmd->adv_data_len, 0,
                                   BLE_HCI_LE_SET_DATA_OPER_COMPLETE);
}

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
static bool
pri_phy_valid(uint8_t phy)
{
    switch (phy) {
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CODED_PHY)
    case BLE_HCI_LE_PHY_CODED:
#endif
    case BLE_HCI_LE_PHY_1M:
        return true;
    default:
        return false;
    }
}

static bool
sec_phy_valid(uint8_t phy)
{
    switch (phy) {
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CODED_PHY)
    case BLE_HCI_LE_PHY_CODED:
#endif
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_2M_PHY)
    case BLE_HCI_LE_PHY_2M:
#endif
    case BLE_HCI_LE_PHY_1M:
        return true;
    default:
        return false;
    }
}

static struct ble_ll_adv_sm *
ble_ll_adv_sm_get(uint8_t instance)
{
    struct ble_ll_adv_sm *advsm;
    int i;

    advsm = ble_ll_adv_sm_find_configured(instance);
    if (advsm) {
        return advsm;
    }

    for (i = 0; i < ARRAY_SIZE(g_ble_ll_adv_sm); i++) {
        advsm = &g_ble_ll_adv_sm[i];

        if (!(advsm->flags & BLE_LL_ADV_SM_FLAG_CONFIGURED)) {
            ble_ll_adv_sm_init(advsm);

           /* configured flag is set by caller on success config */
           advsm->adv_instance = instance;
           return advsm;
        }
    }

    return NULL;
}

int
ble_ll_adv_ext_set_param(const uint8_t *cmdbuf, uint8_t len,
                         uint8_t *rspbuf, uint8_t *rsplen)
{
    const struct ble_hci_le_set_ext_adv_params_cp *cmd = (const void *) cmdbuf;
    struct ble_hci_le_set_ext_adv_params_rp *rsp = (void *) rspbuf;
    struct ble_ll_adv_sm *advsm;
    uint32_t adv_itvl_min;
    uint32_t adv_itvl_max;
    uint16_t props;
    int rc;

    if (len != sizeof(*cmd )) {
        rc = BLE_ERR_INV_HCI_CMD_PARMS;
        goto done;
    }

    advsm = ble_ll_adv_sm_get(cmd->adv_handle);
    if (!advsm) {
        rc = BLE_ERR_MEM_CAPACITY;
        goto done;
    }

    if (advsm->adv_enabled) {
        rc = BLE_ERR_CMD_DISALLOWED;
        goto done;
    }

    props = le16toh(cmd->props);

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV)
    /* If the Host issues this command when periodic advertising is enabled for
     * the specified advertising set and connectable, scannable, legacy, or
     * anonymous advertising is specified, the Controller shall return the
     * error code Invalid HCI Command Parameters (0x12).
     */
    if (advsm->flags & BLE_LL_ADV_SM_FLAG_PERIODIC_CONFIGURED) {
        if (advsm->periodic_adv_enabled) {
            if (props & (BLE_HCI_LE_SET_EXT_ADV_PROP_SCANNABLE |
                         BLE_HCI_LE_SET_EXT_ADV_PROP_CONNECTABLE |
                         BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY |
                         BLE_HCI_LE_SET_EXT_ADV_PROP_ANON_ADV)) {
                rc = BLE_ERR_INV_HCI_CMD_PARMS;
                goto done;
            }
        }
    }
#endif

    adv_itvl_min = cmd->pri_itvl_min[2] << 16 | cmd->pri_itvl_min[1] << 8 |
                   cmd->pri_itvl_min[0];
    adv_itvl_max = cmd->pri_itvl_max[2] << 16 | cmd->pri_itvl_max[1] << 8 |
                   cmd->pri_itvl_max[0];

    if (props & ~BLE_HCI_LE_SET_EXT_ADV_PROP_MASK) {
        rc = BLE_ERR_INV_HCI_CMD_PARMS;
        goto done;
    }

    if (props & BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY) {
        if (ADV_DATA_LEN(advsm) > BLE_ADV_LEGACY_DATA_MAX_LEN ||
            SCAN_RSP_DATA_LEN(advsm) > BLE_SCAN_RSP_LEGACY_DATA_MAX_LEN) {
            rc = BLE_ERR_INV_HCI_CMD_PARMS;
            goto done;
        }

        /* if legacy bit is set possible values are limited */
        switch (props) {
        case BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY_IND:
        case BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY_LD_DIR:
        case BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY_HD_DIR:
        case BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY_SCAN:
        case BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY_NONCONN:
            break;
        default:
            rc = BLE_ERR_INV_HCI_CMD_PARMS;
            goto done;
        }
    } else {
        /* HD directed advertising allowed only on legacy PDUs */
        if (props & BLE_HCI_LE_SET_EXT_ADV_PROP_HD_DIRECTED) {
            rc = BLE_ERR_INV_HCI_CMD_PARMS;
            goto done;
        }

        /* if ext advertising PDUs are used then it shall not be both
         * connectable and scanable
         */
        if ((props & BLE_HCI_LE_SET_EXT_ADV_PROP_CONNECTABLE) &&
            (props & BLE_HCI_LE_SET_EXT_ADV_PROP_SCANNABLE)) {
            rc = BLE_ERR_INV_HCI_CMD_PARMS;
            goto done;
        }
    }

    /* High Duty Directed advertising is special */
    if (props & BLE_HCI_LE_SET_EXT_ADV_PROP_HD_DIRECTED) {
        if (ADV_DATA_LEN(advsm) || SCAN_RSP_DATA_LEN(advsm)) {
            rc = BLE_ERR_INV_HCI_CMD_PARMS;
            goto done;
        }

        /* Ignore min/max interval */
        adv_itvl_min = 0;
        adv_itvl_max = 0;
    } else {
        /* validate intervals for non HD-directed advertising */
        if ((adv_itvl_min > adv_itvl_max) ||
                (adv_itvl_min < BLE_HCI_ADV_ITVL_MIN) ||
                (adv_itvl_max < BLE_HCI_ADV_ITVL_MIN)) {
            rc = BLE_ERR_INV_HCI_CMD_PARMS;
            goto done;
        }

        /* TODO for now limit those to values from legacy advertising
         *
         * If the primary advertising interval range is outside the advertising
         * interval range supported by the Controller, then the Controller shall
         * return the error code Unsupported Feature or Parameter Value (0x11).
         */
        if ((adv_itvl_min > BLE_HCI_ADV_ITVL_MAX) ||
                (adv_itvl_max > BLE_HCI_ADV_ITVL_MAX)) {
            rc = BLE_ERR_UNSUPPORTED;
            goto done;
        }
    }

    /* There are only three adv channels, so check for any outside the range */
    if (((cmd->pri_chan_map & 0xF8) != 0) || (cmd->pri_chan_map == 0)) {
        rc = BLE_ERR_INV_HCI_CMD_PARMS;
        goto done;
    }

    if (cmd->own_addr_type > BLE_HCI_ADV_OWN_ADDR_MAX) {
        rc = BLE_ERR_INV_HCI_CMD_PARMS;
        goto done;
    }

#if !MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
    /* If we dont support privacy some address types wont work */
    if (cmd->own_addr_type > BLE_HCI_ADV_OWN_ADDR_RANDOM) {
        rc = BLE_ERR_UNSUPPORTED;
        goto done;
    }
#endif

    /* peer address type is only valid for directed */
    if ((props & BLE_HCI_LE_SET_EXT_ADV_PROP_DIRECTED) &&
            (cmd->peer_addr_type > BLE_HCI_ADV_PEER_ADDR_MAX)) {
        rc = BLE_ERR_INV_HCI_CMD_PARMS;
        goto done;
    }

    /* Check filter policy (valid only for undirected) */
    if (!(props & BLE_HCI_LE_SET_EXT_ADV_PROP_DIRECTED) &&
         cmd->filter_policy > BLE_HCI_ADV_FILT_MAX) {
        rc = BLE_ERR_INV_HCI_CMD_PARMS;
        goto done;
    }

    if (!pri_phy_valid(cmd->pri_phy)) {
        rc = BLE_ERR_INV_HCI_CMD_PARMS;
        goto done;
    }

    /* check secondary phy only if not using legacy PDUs */
    if (!(props & BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY) &&
            !sec_phy_valid(cmd->sec_phy)) {
        rc = BLE_ERR_INV_HCI_CMD_PARMS;
        goto done;
    }

    if (cmd->sid > 0x0f) {
        rc = BLE_ERR_INV_HCI_CMD_PARMS;
        goto done;
    }

    if (cmd->scan_req_notif > 0x01) {
        rc = BLE_ERR_INV_HCI_CMD_PARMS;
        goto done;
    }

    rc = BLE_ERR_SUCCESS;

    if (cmd->tx_power == 127) {
        /* no preference */
        advsm->adv_txpwr = MYNEWT_VAL(BLE_LL_TX_PWR_DBM);
    } else {
        advsm->adv_txpwr = ble_phy_txpower_round(cmd->tx_power);
    }

    /* we can always store as those are validated and used only when needed */
    advsm->peer_addr_type = cmd->peer_addr_type;
    memcpy(advsm->peer_addr, cmd->peer_addr, BLE_DEV_ADDR_LEN);
    advsm->own_addr_type = cmd->own_addr_type;
    advsm->adv_filter_policy = cmd->filter_policy;
    advsm->adv_chanmask = cmd->pri_chan_map;
    advsm->adv_itvl_min = adv_itvl_min;
    advsm->adv_itvl_max = adv_itvl_max;
    advsm->pri_phy = cmd->pri_phy;
    advsm->sec_phy = cmd->sec_phy;
    /* Update SID only */
    advsm->adi = (advsm->adi & 0x0fff) | ((cmd->sid << 12));

    advsm->props = props;

    /* Set proper mbuf chain for aux data */
    if (props & BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY) {
        advsm->aux_data = NULL;
    } else if (props & BLE_HCI_LE_SET_EXT_ADV_PROP_SCANNABLE) {
        advsm->aux_data = &advsm->scan_rsp_data;
    } else {
        advsm->aux_data = &advsm->adv_data;
    }

    if (cmd->scan_req_notif) {
        ble_ll_adv_flags_set(advsm, BLE_LL_ADV_SM_FLAG_SCAN_REQ_NOTIF);
    } else {
        ble_ll_adv_flags_clear(advsm, BLE_LL_ADV_SM_FLAG_SCAN_REQ_NOTIF);
    }

    ble_ll_adv_flags_set(advsm, BLE_LL_ADV_SM_FLAG_CONFIGURED);

done:
    /* Update TX power */
    rsp->tx_power = rc ? 0 : advsm->adv_txpwr;

    *rsplen = sizeof(*rsp);
    return rc;
}

int
ble_ll_adv_ext_set_adv_data(const uint8_t *cmdbuf, uint8_t cmdlen)
{
    const struct ble_hci_le_set_ext_adv_data_cp *cmd = (const void *) cmdbuf;

    if (cmdlen < sizeof(*cmd )) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    if (cmd->adv_data_len > BLE_HCI_MAX_EXT_ADV_DATA_LEN ||
            cmd->adv_data_len > cmdlen - sizeof(*cmd)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    /* TODO fragment preference ignored for now */

    return ble_ll_adv_set_adv_data(cmd->adv_data, cmd->adv_data_len,
                                   cmd->adv_handle, cmd->operation);
}

int
ble_ll_adv_ext_set_scan_rsp(const uint8_t *cmdbuf, uint8_t cmdlen)
{
    const struct ble_hci_le_set_ext_scan_rsp_data_cp *cmd = (const void *) cmdbuf;

    if (cmdlen < sizeof(*cmd )) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    if (cmd->scan_rsp_len > BLE_HCI_MAX_EXT_ADV_DATA_LEN ||
            cmd->scan_rsp_len > cmdlen - sizeof(*cmd)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    /* TODO fragment preference ignored for now */

    return ble_ll_adv_set_scan_rsp_data(cmd->scan_rsp, cmd->scan_rsp_len,
                                        cmd->adv_handle, cmd->operation);
}

/**
 * HCI LE extended advertising enable command
 *
 * @param cmd Pointer to command data
 * @param len Command data length
 *
 * @return int BLE error code
 */
int
ble_ll_adv_ext_set_enable(const uint8_t *cmdbuf, uint8_t len)
{
    const struct ble_hci_le_set_ext_adv_enable_cp *cmd = (const void *) cmdbuf;
    struct ble_ll_adv_sm *advsm;
    int i, j, rc;

    if (len < sizeof(*cmd)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    /* check if length is correct */
    if (len != 2 + (cmd->num_sets * sizeof(cmd->sets[0]))) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    if (cmd->num_sets > BLE_ADV_INSTANCES) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    if (cmd->num_sets == 0) {
        if (cmd->enable) {
            return BLE_ERR_INV_HCI_CMD_PARMS;
        }

        /* disable all instances */
        for (i = 0; i < BLE_ADV_INSTANCES; i++) {
            ble_ll_adv_set_enable(i, 0, 0, 0);
        }

        return BLE_ERR_SUCCESS;
    }

    /* validate instances */
    for (i = 0; i < cmd->num_sets; i++) {
        /* validate duplicated sets */
        for (j = i + 1; j < cmd->num_sets; j++) {
            if (cmd->sets[i].adv_handle == cmd->sets[j].adv_handle) {
                return BLE_ERR_INV_HCI_CMD_PARMS;
            }
        }

        advsm = ble_ll_adv_sm_find_configured(cmd->sets[i].adv_handle);
        if (!advsm) {
            return BLE_ERR_UNK_ADV_INDENT;
        }

        if (cmd->enable) {
            if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_HD_DIRECTED) {
                if (cmd->sets[i].duration == 0 ||
                        le16toh(cmd->sets[i].duration) > 128) {
                    return BLE_ERR_INV_HCI_CMD_PARMS;
                }
            }
        }
    }

    for (i = 0; i < cmd->num_sets; i++) {
        rc = ble_ll_adv_set_enable(cmd->sets[i].adv_handle, cmd->enable,
                                   le16toh(cmd->sets[i].duration),
                                   cmd->sets[i].max_events);
        if (rc) {
            return rc;
        }
    }

    return BLE_ERR_SUCCESS;
}

int
ble_ll_adv_set_random_addr(const uint8_t *addr, uint8_t instance)
{
    struct ble_ll_adv_sm *advsm;

    advsm = ble_ll_adv_sm_find_configured(instance);
    if (!advsm) {
        return BLE_ERR_UNK_ADV_INDENT;
    }

    /*
     * Reject if connectable advertising is on
     * Core Spec Vol. 2 Part E 7.8.52
     */
    if (advsm->adv_enabled &&
            (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_CONNECTABLE)) {
        return BLE_ERR_CMD_DISALLOWED;
    }

    memcpy(advsm->adv_random_addr, addr, BLE_DEV_ADDR_LEN);
    return BLE_ERR_SUCCESS;
}

int
ble_ll_adv_hci_set_random_addr(const uint8_t *cmdbuf, uint8_t len)
{
    const struct ble_hci_le_set_adv_set_rnd_addr_cp *cmd = (const void *) cmdbuf;

    if (len != sizeof(*cmd)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    return ble_ll_adv_set_random_addr(cmd->addr, cmd->adv_handle);
}

/**
 * HCI LE extended advertising remove command
 *
 * @return int BLE error code
 */
int
ble_ll_adv_remove(const uint8_t *cmdbuf, uint8_t len)
{
    const struct ble_hci_le_remove_adv_set_cp *cmd = (const void *) cmdbuf;
    struct ble_ll_adv_sm *advsm;

    if (len != sizeof(*cmd)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    advsm = ble_ll_adv_sm_find_configured(cmd->adv_handle);
    if (!advsm) {
        return BLE_ERR_UNK_ADV_INDENT;
    }

    if (advsm->adv_enabled) {
        return BLE_ERR_CMD_DISALLOWED;
    }

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV)
    if (advsm->periodic_adv_enabled) {
        return BLE_ERR_CMD_DISALLOWED;
    }

    if (advsm->periodic_adv_data) {
        os_mbuf_free_chain(advsm->periodic_adv_data);
    }
#endif

    if (advsm->adv_data) {
        os_mbuf_free_chain(advsm->adv_data);
    }
    if (advsm->scan_rsp_data) {
        os_mbuf_free_chain(advsm->scan_rsp_data);
    }

    ble_ll_adv_sm_init(advsm);

    return BLE_ERR_SUCCESS;
}

/**
 * HCI LE extended advertising clear command
 *
 * @return int BLE error code
 */
int
ble_ll_adv_clear_all(void)
{
    int i;

    for (i = 0; i < BLE_ADV_INSTANCES; i++) {
        if (g_ble_ll_adv_sm[i].adv_enabled) {
            return BLE_ERR_CMD_DISALLOWED;
        }

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV)
    if (g_ble_ll_adv_sm[i].periodic_adv_enabled) {
        return BLE_ERR_CMD_DISALLOWED;
    }
#endif
    }

    ble_ll_adv_reset();

    return BLE_ERR_SUCCESS;
}

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV)
static uint16_t
ble_ll_adv_sync_get_pdu_len(uint16_t data_len, uint16_t *data_offset,
                            uint16_t props)
{
    uint16_t rem_data_len = data_len - *data_offset;
    uint8_t hdr_len = BLE_LL_EXT_ADV_HDR_LEN;
    uint8_t ext_hdr = 0;

    /* TxPower if configured
     * Note: TxPower shall not be present in chain PDU for SYNC
     */
    if (*data_offset == 0 &&
        (props & BLE_HCI_LE_SET_PERIODIC_ADV_PROP_INC_TX_PWR)) {
        ext_hdr |= (1 << BLE_LL_EXT_ADV_TX_POWER_BIT);
        hdr_len += BLE_LL_EXT_ADV_TX_POWER_SIZE;
    }

    /* if we have any fields in ext header we need to add flags, note that Aux
     * PTR is handled later and it will account for flags if needed
     *
     * This could be handled inside TxPower but lets keep code consistent with
     * how Aux calculate works and this also make it easier to add more fields
     * into flags if needed in future
     */
    if (ext_hdr) {
        hdr_len += BLE_LL_EXT_ADV_FLAGS_SIZE;
    }

    /* AdvData always */
    data_len = min(BLE_LL_MAX_PAYLOAD_LEN - hdr_len, rem_data_len);

    /* AuxPtr if there are more AdvData remaining that we can fit here */
    if (rem_data_len > data_len) {
            /* adjust for flags that needs to be added if AuxPtr is only field
             * in Extended Header
             */
            if (!ext_hdr) {
                hdr_len += BLE_LL_EXT_ADV_FLAGS_SIZE;
                data_len -= BLE_LL_EXT_ADV_FLAGS_SIZE;
            }

            hdr_len += BLE_LL_EXT_ADV_AUX_PTR_SIZE;
            data_len -= BLE_LL_EXT_ADV_AUX_PTR_SIZE;

            /* PDU payload should be full if chained */
            BLE_LL_ASSERT(hdr_len + data_len == BLE_LL_MAX_PAYLOAD_LEN);
    }

    *data_offset += data_len;

    return hdr_len + data_len;
}

static bool
ble_ll_adv_periodic_check_data_itvl(uint16_t payload_len, uint16_t props,
                                    uint16_t itvl, uint8_t phy)
{
    uint32_t max_usecs = 0;
    uint32_t itvl_usecs;
    uint16_t offset = 0;
    uint16_t pdu_len;

    while (offset < payload_len) {
        pdu_len = ble_ll_adv_sync_get_pdu_len(payload_len, &offset, props);

        max_usecs += ble_ll_pdu_tx_time_get(pdu_len, phy);
        max_usecs += ble_ll_usecs_to_ticks_round_up(BLE_LL_MAFS +
                                MYNEWT_VAL(BLE_LL_SCHED_AUX_CHAIN_MAFS_DELAY));
    }

    itvl_usecs = (uint32_t)itvl * BLE_LL_ADV_PERIODIC_ITVL;

    return max_usecs < itvl_usecs;
}

int
ble_ll_adv_periodic_set_param(const uint8_t *cmdbuf, uint8_t len)
{
    const struct ble_hci_le_set_periodic_adv_params_cp *cmd = (const void *) cmdbuf;
    struct ble_ll_adv_sm *advsm;
    uint16_t adv_itvl_min;
    uint16_t adv_itvl_max;
    uint16_t props;

    if (len != sizeof(*cmd)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    adv_itvl_min = le16toh(cmd->min_itvl);
    adv_itvl_max = le16toh(cmd->max_itvl);
    props = le16toh(cmd->props);

    advsm = ble_ll_adv_sm_find_configured(cmd->adv_handle);
    if (!advsm) {
        return BLE_ERR_UNK_ADV_INDENT;
    }

    /* If the advertising set identified by the Advertising_Handle specified
     * scannable, connectable, legacy, or anonymous advertising, the Controller
     * shall return the error code Invalid HCI Command Parameters (0x12).
     */
    if (advsm->props & (BLE_HCI_LE_SET_EXT_ADV_PROP_ANON_ADV |
                        BLE_HCI_LE_SET_EXT_ADV_PROP_SCANNABLE |
                        BLE_HCI_LE_SET_EXT_ADV_PROP_CONNECTABLE |
                        BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    /* If the Host issues this command when periodic advertising is enabled for
     * the specified advertising set, the Controller shall return the error code
     * Command Disallowed (0x0C).
     */
    if (advsm->periodic_adv_enabled) {
        return BLE_ERR_CMD_DISALLOWED;
    }

    /* validate intervals */
    if ((adv_itvl_min < 0x0006) || (adv_itvl_max < 0x006) ||
            (adv_itvl_min > adv_itvl_max)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    /* validate properties */
    if (props & ~BLE_HCI_LE_SET_PERIODIC_ADV_PROP_MASK) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    /* If the advertising set already contains periodic advertising data and the
     * length of the data is greater than the maximum that the Controller can
     * transmit within a periodic advertising interval of
     * Periodic_Advertising_Interval_Max, the Controller shall return the error
     * code Packet Too Long (0x45).
     */
    if (!ble_ll_adv_periodic_check_data_itvl(SYNC_DATA_LEN(advsm), props,
                                             adv_itvl_max, advsm->sec_phy)) {
        return BLE_ERR_PACKET_TOO_LONG;
    }

    advsm->periodic_adv_itvl_min = adv_itvl_min;
    advsm->periodic_adv_itvl_max = adv_itvl_max;
    advsm->periodic_adv_props = props;

    ble_ll_adv_flags_set(advsm, BLE_LL_ADV_SM_FLAG_PERIODIC_CONFIGURED);

    return BLE_ERR_SUCCESS;
}

int
ble_ll_adv_periodic_set_data(const uint8_t *cmdbuf, uint8_t len)
{
    const struct ble_hci_le_set_periodic_adv_data_cp *cmd = (const void *) cmdbuf;
    struct ble_ll_adv_sm *advsm;
    uint16_t payload_total_len;
    bool new_data = false;

    if (len < sizeof(*cmd)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    if (cmd->adv_data_len > BLE_HCI_MAX_PERIODIC_ADV_DATA_LEN ||
            cmd->adv_data_len != len - sizeof(*cmd)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    advsm = ble_ll_adv_sm_find_configured(cmd->adv_handle);
    if (!advsm) {
        return BLE_ERR_UNK_ADV_INDENT;
    }

    if (!(advsm->flags & BLE_LL_ADV_SM_FLAG_PERIODIC_CONFIGURED)) {
        return BLE_ERR_CMD_DISALLOWED;
    }

    switch (cmd->operation) {
    case BLE_HCI_LE_SET_DATA_OPER_LAST:
    case BLE_HCI_LE_SET_DATA_OPER_INT:
        if (!(advsm->flags & BLE_LL_ADV_SM_FLAG_PERIODIC_DATA_INCOMPLETE)) {
            return BLE_ERR_INV_HCI_CMD_PARMS;
        }

        if (!advsm->periodic_adv_data || !cmd->adv_data_len) {
            return BLE_ERR_INV_HCI_CMD_PARMS;
        }

        if (advsm->periodic_adv_enabled) {
            return BLE_ERR_CMD_DISALLOWED;
        }
        break;
    case BLE_HCI_LE_SET_DATA_OPER_FIRST:
        if (advsm->periodic_adv_enabled) {
            return BLE_ERR_CMD_DISALLOWED;
        }

        if (!cmd->adv_data_len) {
            return BLE_ERR_INV_HCI_CMD_PARMS;
        }
        new_data = true;
        break;
    case BLE_HCI_LE_SET_DATA_OPER_COMPLETE:
        new_data = true;
        break;
    default:
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    payload_total_len = cmd->adv_data_len;
    if (!new_data) {
        payload_total_len += SYNC_DATA_LEN(advsm);
    }

    /* If the combined length of the data is greater than the maximum that the
     * Controller can transmit within the current periodic advertising interval
     * (if periodic advertising is currently enabled) or the
     * Periodic_Advertising_Interval_Max for the advertising set (if currently
     * disabled), all the data shall be discarded and the Controller shall
     * return the error code Packet Too Long (0x45).
     */
    if (!ble_ll_adv_periodic_check_data_itvl(payload_total_len,
                                             advsm->periodic_adv_props,
                                             advsm->periodic_adv_itvl_max,
                                             advsm->sec_phy)) {
        return BLE_ERR_PACKET_TOO_LONG;
    }

    if (advsm->periodic_adv_active) {
        ble_ll_adv_flags_clear(advsm, BLE_LL_ADV_SM_FLAG_PERIODIC_NEW_DATA);

        ble_ll_adv_update_data_mbuf(&advsm->periodic_new_data, true,
                                    BLE_ADV_DATA_MAX_LEN,
                                    cmd->adv_data, cmd->adv_data_len);
        if (!advsm->periodic_new_data) {
            return BLE_ERR_MEM_CAPACITY;
        }

        ble_ll_adv_flags_set(advsm, BLE_LL_ADV_SM_FLAG_PERIODIC_NEW_DATA);
    } else {
        ble_ll_adv_update_data_mbuf(&advsm->periodic_adv_data, new_data,
                                    BLE_ADV_DATA_MAX_LEN, cmd->adv_data,
                                    cmd->adv_data_len);
        if (!advsm->periodic_adv_data) {
            return BLE_ERR_MEM_CAPACITY;
        }
    }

    /* set/clear incomplete data flag only on success */
    switch (cmd->operation) {
    case BLE_HCI_LE_SET_DATA_OPER_LAST:
    case BLE_HCI_LE_SET_DATA_OPER_COMPLETE:
        ble_ll_adv_flags_clear(advsm,
                               BLE_LL_ADV_SM_FLAG_PERIODIC_DATA_INCOMPLETE);
        break;
    case BLE_HCI_LE_SET_DATA_OPER_INT:
    case BLE_HCI_LE_SET_DATA_OPER_FIRST:
    default:
        ble_ll_adv_flags_set(advsm,
                             BLE_LL_ADV_SM_FLAG_PERIODIC_DATA_INCOMPLETE);
        break;
    }

    return BLE_ERR_SUCCESS;
}

int
ble_ll_adv_periodic_enable(const uint8_t *cmdbuf, uint8_t len)
{
    const struct ble_hci_le_set_periodic_adv_enable_cp *cmd = (const void *)cmdbuf;
    struct ble_ll_adv_sm *advsm;

    if (len != sizeof(*cmd)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    advsm = ble_ll_adv_sm_find_configured(cmd->adv_handle);
    if (!advsm) {
        return BLE_ERR_UNK_ADV_INDENT;
    }

    if (cmd->enable) {
        if (advsm->flags & BLE_LL_ADV_SM_FLAG_PERIODIC_DATA_INCOMPLETE) {
            return BLE_ERR_CMD_DISALLOWED;
        }

        /* If Enable is set to 0x01 and the length of the periodic advertising
         * data is greater than the maximum that the Controller can transmit
         * within the chosen periodicadvertising interval, the Controller shall
         * return the error code Packet Too Long (0x45).
         */
        if (!ble_ll_adv_periodic_check_data_itvl(SYNC_DATA_LEN(advsm),
                                                 advsm->periodic_adv_props,
                                                 advsm->periodic_adv_itvl_max,
                                                 advsm->sec_phy)) {
            return BLE_ERR_PACKET_TOO_LONG;
        }

        /* If the advertising set is not currently enabled (see the
         * LE_Set_Extended_Advertising_Enable command), the periodic advertising
         * is not started until the advertising set is enabled.
         */
        if (advsm->adv_enabled && !advsm->periodic_adv_active) {
            /* Start the periodic advertising state machine */
            ble_ll_adv_sm_start_periodic(advsm);
        }
    } else {
        /* Stop the periodic advertising state machine */
        ble_ll_adv_sm_stop_periodic(advsm);
    }

    advsm->periodic_adv_enabled = cmd->enable;

    return BLE_ERR_SUCCESS;
}

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV_SYNC_TRANSFER)
static int
ble_ll_adv_periodic_send_sync_ind(struct ble_ll_adv_sm *advsm,
                                  struct ble_ll_conn_sm *connsm,
                                  uint16_t service_data)
{
    struct os_mbuf *om;
    uint8_t *sync_ind;

    om = os_msys_get_pkthdr(BLE_LL_CTRL_MAX_PDU_LEN,
                            sizeof(struct ble_mbuf_hdr));
    if (!om) {
        return BLE_ERR_MEM_CAPACITY;
    }

    om->om_data[0] = BLE_LL_CTRL_PERIODIC_SYNC_IND;

    sync_ind = om->om_data + 1;

    /* ID (service_data), already in LE order */
    memcpy(sync_ind, &service_data, sizeof(service_data));

    /* fill in syncinfo */
    ble_ll_adv_put_syncinfo(advsm, connsm, sync_ind + 20, sync_ind + 2);

    /* lastPaEventCounter */
    put_le16(sync_ind + 22, advsm->periodic_event_cntr_last_sent);

    /* SID, AType, SCA */
    sync_ind[24] = (advsm->adi >> 12);
    sync_ind[24] |= !!(advsm->flags & BLE_LL_ADV_SM_FLAG_TX_ADD) << 4;
    sync_ind[24] |= BLE_LL_SCA_ENUM << 5;

    /* PHY */
    sync_ind[25] = (0x01 << (advsm->sec_phy - 1));

    /* AdvA */
    memcpy(sync_ind + 26, advsm->adva, BLE_DEV_ADDR_LEN);

    /* syncConnEventCount */
    put_le16(sync_ind + 32, connsm->event_cntr);

    ble_ll_conn_enqueue_pkt(connsm, om, BLE_LL_LLID_CTRL,
                            BLE_LL_CTRL_PERIODIC_SYNC_IND_LEN + 1);

    return BLE_ERR_SUCCESS;
}

int
ble_ll_adv_periodic_set_info_transfer(const uint8_t *cmdbuf, uint8_t len,
                                      uint8_t *rspbuf, uint8_t *rsplen)
{
    const struct ble_hci_le_periodic_adv_set_info_transfer_cp *cmd = (const void *)cmdbuf;
     struct ble_hci_le_periodic_adv_set_info_transfer_rp *rsp = (void *) rspbuf;
     struct ble_ll_conn_sm *connsm;
     struct ble_ll_adv_sm *advsm;
     uint16_t handle;
     int rc;

     if (len != sizeof(*cmd)) {
         rc = BLE_ERR_INV_HCI_CMD_PARMS;
         goto done;
     }

     advsm = ble_ll_adv_sm_find_configured(cmd->adv_handle);
     if (!advsm) {
         rc = BLE_ERR_UNK_ADV_INDENT;
         goto done;
     }

     if (!advsm->periodic_adv_active) {
         rc = BLE_ERR_CMD_DISALLOWED;
         goto done;
     }

     handle = le16toh(cmd->conn_handle);
     if (handle > 0xeff) {
         rc = BLE_ERR_INV_HCI_CMD_PARMS;
         goto done;
     }

     connsm = ble_ll_conn_find_active_conn(handle);
     if (!connsm) {
         rc = BLE_ERR_UNK_CONN_ID;
         goto done;
     }

     /* TODO should not need to shift
      * byte 3 (0 byte is conn_feature) , bit 1
      *
      * Allow initiate LL procedure only if remote supports it.
      */
     if (!(connsm->remote_features[2] & (BLE_LL_FEAT_SYNC_TRANS_RECV >> (8 * 3)))) {
         rc = BLE_ERR_UNSUPP_REM_FEATURE;
         goto done;
     }

     rc = ble_ll_adv_periodic_send_sync_ind(advsm, connsm, cmd->service_data);
 done:
     rsp->conn_handle = cmd->conn_handle;
     *rsplen = sizeof(*rsp);
     return rc;
}
#endif
#endif
#endif

/**
 * Says whether the specified address is already connected or not.
 * @param   [in]    addr        The peer address.
 * @param   [in]    addr_type   Public address (0) or random address (1).
 * @return  Return 1 if already connected, 0 otherwise.
 */
static int
ble_ll_adv_already_connected(const uint8_t* addr, uint8_t addr_type)
{
    struct ble_ll_conn_sm *connsm;

    /* extracted from ble_ll_conn_slave_start function */
    SLIST_FOREACH(connsm, &g_ble_ll_conn_active_list, act_sle) {
        if (!memcmp(&connsm->peer_addr, addr, BLE_DEV_ADDR_LEN)) {
            if (addr_type == BLE_ADDR_RANDOM) {
                if (connsm->peer_addr_type & 1) {
                    return 1;
                }
            } else {
                if ((connsm->peer_addr_type & 1) == 0) {
                    return 1;
                }
            }
        }
    }

    return 0;
}

/**
 * Called when the LL receives a scan request or connection request
 *
 * Context: Called from interrupt context.
 *
 * @param rxbuf
 *
 * @return -1: request not for us or is a connect request.
 *          0: request (scan) is for us and we successfully went from rx to tx.
 *        > 0: PHY error attempting to go from rx to tx.
 */
static int
ble_ll_adv_rx_req(uint8_t pdu_type, struct os_mbuf *rxpdu)
{
    int rc;
    int resolved;
    uint8_t chk_wl;
    uint8_t txadd;
    uint8_t peer_addr_type;
    uint8_t *rxbuf;
    uint8_t *adva;
    uint8_t *peer;
    struct ble_mbuf_hdr *ble_hdr;
    struct ble_ll_adv_sm *advsm;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    struct aux_conn_rsp_data rsp_data;
#endif
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
    struct ble_ll_resolv_entry *rl;
#endif

    /* See if adva in the request (scan or connect) matches what we sent */
    advsm = g_ble_ll_cur_adv_sm;
    rxbuf = rxpdu->om_data;
    adva = rxbuf + BLE_LL_PDU_HDR_LEN + BLE_DEV_ADDR_LEN;
    if (memcmp(advsm->adva, adva, BLE_DEV_ADDR_LEN)) {
        return -1;
    }

    /* Set device match bit if we are whitelisting */
    if (pdu_type == BLE_ADV_PDU_TYPE_SCAN_REQ) {
        chk_wl = advsm->adv_filter_policy & 1;
    } else {
        chk_wl = advsm->adv_filter_policy & 2;
    }

    /* Get the peer address type */
    if (rxbuf[0] & BLE_ADV_PDU_HDR_TXADD_MASK) {
        txadd = BLE_ADDR_RANDOM;
    } else {
        txadd = BLE_ADDR_PUBLIC;
    }

    ble_hdr = BLE_MBUF_HDR_PTR(rxpdu);
    peer = rxbuf + BLE_LL_PDU_HDR_LEN;
    peer_addr_type = txadd;
    resolved = 0;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
    rl = NULL;
    if (ble_ll_resolv_enabled()) {
        if (ble_ll_is_rpa(peer, txadd)) {
            advsm->adv_rpa_index = ble_hw_resolv_list_match();
            if (advsm->adv_rpa_index >= 0) {
                ble_hdr->rxinfo.flags |= BLE_MBUF_HDR_F_RESOLVED;
                rl = &g_ble_ll_resolv_list[advsm->adv_rpa_index];
                if (chk_wl) {
                    peer = rl->rl_identity_addr;
                    peer_addr_type = rl->rl_addr_type;
                    resolved = 1;
                }
            } else {
                if (chk_wl) {
                    return -1;
                }
            }
        } else {
            /* Verify privacy mode */
            rl = ble_ll_resolv_list_find(peer, peer_addr_type);
            if (rl && (rl->rl_priv_mode == BLE_HCI_PRIVACY_NETWORK) &&
                rl->rl_has_peer) {
                return -1;
            }
        }
    }
#endif

    /* Set device match bit if we are whitelisting */
    if (chk_wl && !ble_ll_whitelist_match(peer, peer_addr_type, resolved)) {
        return -1;
    }

    /*
     * We set the device match bit to tell the upper layer that we will
     * accept the request
     */
    ble_hdr->rxinfo.flags |= BLE_MBUF_HDR_F_DEVMATCH;

    /* Setup to transmit the scan response if appropriate */
    rc = -1;

    if (pdu_type == BLE_ADV_PDU_TYPE_SCAN_REQ) {
        /* PHY used for scan requests shall be the same as the PHY used for the
         * PDU that they reply to so no need to change PHY mode.
         */
        ble_phy_set_txend_cb(ble_ll_adv_tx_done, advsm);

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
        if (advsm->flags & BLE_LL_ADV_SM_FLAG_SCAN_REQ_NOTIF) {
            ble_ll_hci_ev_send_scan_req_recv(advsm->adv_instance, peer,
                                             peer_addr_type);
        }

        /*
         * We need to store current rxed packet header temporarily so AuxPtr
         * can be calculated (if necessary) relative to AUX_SCAN_RSP instead of
         * AUX_ADV_IND.
         */

        advsm->rx_ble_hdr = ble_hdr;
        rc = ble_phy_tx(ble_ll_adv_scan_rsp_pdu_make, advsm,
                        BLE_PHY_TRANSITION_NONE);
        advsm->rx_ble_hdr = NULL;
#else
        rc = ble_phy_tx(ble_ll_adv_scan_rsp_legacy_pdu_make, advsm,
                        BLE_PHY_TRANSITION_NONE);
#endif

        if (!rc) {
            ble_hdr->rxinfo.flags |= BLE_MBUF_HDR_F_SCAN_RSP_TXD;
            STATS_INC(ble_ll_stats, scan_rsp_txg);
        }
    } else if (pdu_type == BLE_ADV_PDU_TYPE_AUX_CONNECT_REQ) {
        /* See if the device is already connected */
        if (ble_ll_adv_already_connected(peer, peer_addr_type)) {
            return -1;
        }

        /*
         * Only accept connect requests from the desired address if we
         * are doing directed advertising
         */
        if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_DIRECTED) {
            if (memcmp(advsm->initiator_addr, peer, BLE_DEV_ADDR_LEN)) {
                return -1;
            }
        }

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
        if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY) {
            return -1;
        }

        /* use remote address used over the air */
        rsp_data.advsm = advsm;
        rsp_data.peer = rxbuf + BLE_LL_PDU_HDR_LEN;
        rsp_data.rxadd = rxbuf[0] & BLE_ADV_PDU_HDR_TXADD_MASK;

        ble_phy_set_txend_cb(ble_ll_adv_tx_done, advsm);
        rc = ble_phy_tx(ble_ll_adv_aux_conn_rsp_pdu_make, &rsp_data,
                        BLE_PHY_TRANSITION_NONE);
        if (!rc) {
            ble_ll_adv_flags_set(advsm, BLE_LL_ADV_SM_FLAG_CONN_RSP_TXD);
            STATS_INC(ble_ll_stats, aux_conn_rsp_tx);
        }
#endif
    }

    return rc;
}

/**
 * Called when a connect request has been received.
 *
 * Context: Link Layer
 *
 * @param rxbuf
 * @param flags
 *
 * @return 0: no connection started. 1: connection started
 */
static int
ble_ll_adv_conn_req_rxd(uint8_t *rxbuf, struct ble_mbuf_hdr *hdr,
                        struct ble_ll_adv_sm *advsm)
{
    int valid;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
    uint8_t resolved;
#endif
    uint8_t addr_type;
    uint8_t *inita;
    uint8_t *ident_addr;

    /* Don't create connection if AUX_CONNECT_RSP was not send */
    if (!(advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY)) {
        if (!(advsm->flags & BLE_LL_ADV_SM_FLAG_CONN_RSP_TXD)) {
            return 0;
        }
    }

    /* Check filter policy. */
    valid = 0;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
    resolved = BLE_MBUF_HDR_RESOLVED(hdr);
#endif
    inita = rxbuf + BLE_LL_PDU_HDR_LEN;
    if (hdr->rxinfo.flags & BLE_MBUF_HDR_F_DEVMATCH) {

        valid = 1;
        if (rxbuf[0] & BLE_ADV_PDU_HDR_TXADD_MASK) {
            addr_type = BLE_ADDR_RANDOM;
        } else {
            addr_type = BLE_ADDR_PUBLIC;
        }

        /*
         * Only accept connect requests from the desired address if we
         * are doing directed advertising
         */
        if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_DIRECTED) {
            ident_addr = inita;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
            if (resolved) {
                ident_addr = g_ble_ll_resolv_list[advsm->adv_rpa_index].rl_identity_addr;
                addr_type = g_ble_ll_resolv_list[advsm->adv_rpa_index].rl_addr_type;
            }
#endif
            if ((addr_type != advsm->peer_addr_type) ||
                memcmp(advsm->peer_addr, ident_addr, BLE_DEV_ADDR_LEN)) {
                valid = 0;
            }
        }
    }

    if (valid) {
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
        if (resolved) {
            /* Retain the resolvable private address that we received. */
            memcpy(advsm->adv_rpa, inita, BLE_DEV_ADDR_LEN);

            /* Update resolving list with current peer RPA */
            ble_ll_resolv_set_peer_rpa(advsm->adv_rpa_index, inita);

            /*
             * Overwrite received inita with identity address since that
             * is used from now on.
             */
            memcpy(inita,
                   g_ble_ll_resolv_list[advsm->adv_rpa_index].rl_identity_addr,
                   BLE_DEV_ADDR_LEN);

            /* Peer address type is an identity address */
            addr_type = g_ble_ll_resolv_list[advsm->adv_rpa_index].rl_addr_type;
            addr_type += 2;
        }
#endif

        /* Try to start slave connection. If successful, stop advertising */
        valid = ble_ll_conn_slave_start(rxbuf, addr_type, hdr,
                          !(advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY));
        if (valid) {
            /* stop advertising only if not transmitting connection response */
            if (!(advsm->flags & BLE_LL_ADV_SM_FLAG_CONN_RSP_TXD)) {
                ble_ll_adv_sm_stop(advsm);
            }
        }
    }

    return valid;
}

/**
 * Called on phy rx pdu end when in advertising state.
 *
 * There are only two pdu types we care about in this state: scan requests
 * and connection requests. When we receive a scan request we must determine if
 * we need to send a scan response and that needs to be acted on within T_IFS.
 *
 * When we receive a connection request, we need to determine if we will allow
 * this device to start a connection with us. However, no immediate response is
 * sent so we handle this at the link layer task.
 *
 * Context: Interrupt
 *
 * @param pdu_type Type of pdu received.
 * @param rxpdu Pointer to received PDU
 *
 * @return int
 *       < 0: Disable the phy after reception.
 *      == 0: Do not disable the PHY
 *       > 0: Do not disable PHY as that has already been done.
 */
int
ble_ll_adv_rx_isr_end(uint8_t pdu_type, struct os_mbuf *rxpdu, int crcok)
{
    int rc;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    struct ble_mbuf_hdr *rxhdr;
#endif

    rc = -1;
    if (rxpdu == NULL) {
        ble_ll_adv_tx_done(g_ble_ll_cur_adv_sm);
    } else {
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
        rxhdr = BLE_MBUF_HDR_PTR(rxpdu);
        rxhdr->rxinfo.user_data = g_ble_ll_cur_adv_sm;
        if (ble_ll_adv_active_chanset_is_sec(g_ble_ll_cur_adv_sm)) {
            rxhdr->rxinfo.flags |= BLE_MBUF_HDR_F_EXT_ADV_SEC;
        } else {
            assert(ble_ll_adv_active_chanset_is_pri(g_ble_ll_cur_adv_sm));
        }
#endif
        if (crcok) {
            if ((pdu_type == BLE_ADV_PDU_TYPE_SCAN_REQ) ||
                (pdu_type == BLE_ADV_PDU_TYPE_CONNECT_IND)) {
                /* Process request */
                rc = ble_ll_adv_rx_req(pdu_type, rxpdu);
            }
        }

        if (rc) {
            /* We no longer have a current state machine */
            g_ble_ll_cur_adv_sm = NULL;
        }
    }

    if (rc) {
        ble_ll_state_set(BLE_LL_STATE_STANDBY);
    }

    return rc;
}

/**
 * Process a received packet at the link layer task when in the advertising
 * state
 *
 * Context: Link Layer
 *
 *
 * @param ptype
 * @param rxbuf
 * @param hdr
 *
 * @return int
 */
void
ble_ll_adv_rx_pkt_in(uint8_t ptype, uint8_t *rxbuf, struct ble_mbuf_hdr *hdr)
{
    int adv_event_over;
    struct ble_ll_adv_sm *advsm;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    advsm = (struct ble_ll_adv_sm *)hdr->rxinfo.user_data;
#else
    advsm = &g_ble_ll_adv_sm[0];
#endif

    /*
     * It is possible that advertising was stopped and a packet plcaed on the
     * LL receive packet queue. In this case, just ignore the received packet
     * as the advertising state machine is no longer "valid"
     */
    if (!advsm->adv_enabled) {
        return;
    }

    /*
     * If we have received a scan request and we are transmitting a response
     * or we have received a valid connect request, dont "end" the advertising
     * event. In the case of a connect request we will stop advertising. In
     * the case of the scan response transmission we will get a transmit
     * end callback.
     */
    adv_event_over = 1;
    if (BLE_MBUF_HDR_CRC_OK(hdr)) {
        if (ptype == BLE_ADV_PDU_TYPE_CONNECT_IND) {
            if (ble_ll_adv_conn_req_rxd(rxbuf, hdr, advsm)) {
                adv_event_over = 0;
            }
        } else {
            if ((ptype == BLE_ADV_PDU_TYPE_SCAN_REQ) &&
                (hdr->rxinfo.flags & BLE_MBUF_HDR_F_SCAN_RSP_TXD)) {
                adv_event_over = 0;
            }
        }
    }

    if (adv_event_over) {
        ble_ll_adv_make_done(advsm, hdr);
    }
}

/**
 * Called when a receive PDU has started and we are advertising.
 *
 * Context: interrupt
 *
 * @param pdu_type
 * @param rxpdu
 *
 * @return int
 *   < 0: A frame we dont want to receive.
 *   = 0: Continue to receive frame. Dont go from rx to tx
 *   > 0: Continue to receive frame and go from rx to tx when done
 */
int
ble_ll_adv_rx_isr_start(uint8_t pdu_type)
{
    int rc;
    struct ble_ll_adv_sm *advsm;

    /* Assume we will abort the frame */
    rc = -1;

    /* If we get a scan request we must tell the phy to go from rx to tx */
    advsm = g_ble_ll_cur_adv_sm;
    if (pdu_type == BLE_ADV_PDU_TYPE_SCAN_REQ) {
        /* Only accept scan requests if we are indirect adv or scan adv */
        if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_SCANNABLE) {
            rc = 1;
        }
    } else {
        /* Only accept connect requests if connectable advertising event */
        if (pdu_type == BLE_ADV_PDU_TYPE_CONNECT_IND) {
            if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_CONNECTABLE) {
                /* Need transition to TX if extended adv */
                rc = !(advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY);
            }
        }
    }

    /*
     * If we abort the frame, we need to post the LL task to check if the
     * advertising event is over.
     */
    if (rc < 0) {
        ble_ll_adv_tx_done(advsm);
    }

    return rc;
}

static void
ble_ll_adv_drop_event(struct ble_ll_adv_sm *advsm)
{
    STATS_INC(ble_ll_stats, adv_drop_event);

    ble_ll_sched_rmv_elem(&advsm->adv_sch);
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    ble_ll_sched_rmv_elem(&advsm->aux[0].sch);
    ble_ll_sched_rmv_elem(&advsm->aux[1].sch);

    ble_npl_eventq_remove(&g_ble_ll_data.ll_evq, &advsm->adv_sec_txdone_ev);
    advsm->aux_active = 0;
#endif

    advsm->adv_chan = ble_ll_adv_final_chan(advsm);
    ble_npl_eventq_put(&g_ble_ll_data.ll_evq, &advsm->adv_txdone_ev);
}

static void
ble_ll_adv_reschedule_event(struct ble_ll_adv_sm *advsm)
{
    int rc;
    uint32_t start_time;
    uint32_t max_delay_ticks;

    assert(advsm->adv_enabled);

    if (!advsm->adv_sch.enqueued) {
        if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_HD_DIRECTED) {
            max_delay_ticks = 0;
        } else {
            max_delay_ticks =
                    os_cputime_usecs_to_ticks(BLE_LL_ADV_DELAY_MS_MAX * 1000);
        }

        rc = ble_ll_sched_adv_reschedule(&advsm->adv_sch, &start_time,
                                         max_delay_ticks);
        if (rc) {
            ble_ll_adv_drop_event(advsm);
            return;
        }

        start_time += g_ble_ll_sched_offset_ticks;
        advsm->adv_event_start_time = start_time;
        advsm->adv_pdu_start_time = start_time;
    }

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    if (!(advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY) &&
                                                        !advsm->aux_active) {
        ble_ll_adv_aux_schedule(advsm);
    }
#endif
}

/**
 * Called when an advertising event is over.
 *
 * Context: Link Layer task.
 *
 * @param arg Pointer to advertising state machine.
 */
static void
ble_ll_adv_done(struct ble_ll_adv_sm *advsm)

{
    int rc;
    int resched_pdu;
    uint8_t mask;
    uint8_t final_adv_chan;
    int32_t delta_t;
    uint32_t itvl;
    uint32_t tick_itvl;
    uint32_t start_time;

    assert(advsm->adv_enabled);

    ble_ll_rfmgmt_release();

    ble_ll_adv_update_adv_scan_rsp_data(advsm);

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    if (advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY) {
        /* stop advertising this was due to transmitting connection response */
        if (advsm->flags & BLE_LL_ADV_SM_FLAG_CONN_RSP_TXD) {
            ble_ll_adv_sm_stop(advsm);
            return;
        }
    }
#endif

    /* Remove the element from the schedule if it is still there. */
    ble_ll_sched_rmv_elem(&advsm->adv_sch);

    ble_npl_eventq_remove(&g_ble_ll_data.ll_evq, &advsm->adv_txdone_ev);

    /*
     * Check if we have ended our advertising event. If our last advertising
     * packet was sent on the last channel, it means we are done with this
     * event.
     */
    final_adv_chan = ble_ll_adv_final_chan(advsm);

    if (advsm->adv_chan == final_adv_chan) {
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
        if (advsm->events_max) {
            advsm->events++;
        }
#endif

        ble_ll_scan_chk_resume();

        /* This event is over. Set adv channel to first one */
        advsm->adv_chan = ble_ll_adv_first_chan(advsm);

        /*
         * Calculate start time of next advertising event. NOTE: we do not
         * add the random advDelay as the scheduling code will do that.
         */
        itvl = advsm->adv_itvl_usecs;
        tick_itvl = os_cputime_usecs_to_ticks(itvl);
        advsm->adv_event_start_time += tick_itvl;
        advsm->adv_pdu_start_time = advsm->adv_event_start_time;

        /*
         * The scheduled time better be in the future! If it is not, we will
         * just keep advancing until we the time is in the future
         */
        start_time = advsm->adv_pdu_start_time - g_ble_ll_sched_offset_ticks;

        delta_t = (int32_t)(start_time - os_cputime_get32());
        if (delta_t < 0) {
            /*
             * NOTE: we just the same interval that we calculated earlier.
             * No real need to keep recalculating a new interval.
             */
            while (delta_t < 0) {
                advsm->adv_event_start_time += tick_itvl;
                advsm->adv_pdu_start_time = advsm->adv_event_start_time;
                delta_t += (int32_t)tick_itvl;
            }
        }
        resched_pdu = 0;
    } else {
        /*
         * Move to next advertising channel. If not in the mask, just
         * increment by 1. We can do this because we already checked if we
         * just transmitted on the last advertising channel
         */
        ++advsm->adv_chan;
        mask = 1 << (advsm->adv_chan - BLE_PHY_ADV_CHAN_START);
        if ((mask & advsm->adv_chanmask) == 0) {
            ++advsm->adv_chan;
        }

        /*
         * We will transmit right away. Set next pdu start time to now
         * plus a xcvr start delay just so we dont count late adv starts
         */
        advsm->adv_pdu_start_time = os_cputime_get32() +
                                    g_ble_ll_sched_offset_ticks;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
        /* If we're past aux (unlikely, but can happen), just drop an event */
        if (!(advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY) &&
                advsm->aux_active &&
                advsm->adv_pdu_start_time > AUX_CURRENT(advsm)->start_time) {
            ble_ll_adv_drop_event(advsm);
            return;
        }
#endif

        resched_pdu = 1;
    }

    /* check if advertising timed out */
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    if (advsm->duration &&
        advsm->adv_pdu_start_time >= advsm->adv_end_time) {
        /* Legacy PDUs need to be stop here.
         * For ext adv it will be stopped when AUX is done (unless it was
         * dropped so check if AUX is active here as well).
         */
        if ((advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY) ||
                !advsm->aux_active) {
            ble_ll_adv_sm_stop_timeout(advsm);
        }

        return;
    }
#else
    if ((advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_HD_DIRECTED) &&
            (advsm->adv_pdu_start_time >= advsm->adv_end_time)) {
        ble_ll_adv_sm_stop_timeout(advsm);
        return;
    }
#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    if (advsm->events_max && (advsm->events >= advsm->events_max)) {
        /* Legacy PDUs need to be stop here.
         * For ext adv it will be stopped when AUX is done (unless it was
         * dropped so check if AUX is active here as well).
         */
        if ((advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY) ||
                !advsm->aux_active) {
            ble_ll_adv_sm_stop_limit_reached(advsm);
        }

        return;
    }
#endif

    /* We need to regenerate our RPA's if we have passed timeout */
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
    ble_ll_adv_chk_rpa_timeout(advsm);
#endif

    /* Schedule advertising transmit */
    ble_ll_adv_set_sched(advsm);

    if (!resched_pdu) {
        ble_ll_adv_reschedule_event(advsm);
        return;
    }

    /*
     * In the unlikely event we can't reschedule this, just post a done event
     * and we will reschedule the next advertising PDU.
     */
    rc = ble_ll_sched_adv_resched_pdu(&advsm->adv_sch);
    if (rc) {
        STATS_INC(ble_ll_stats, adv_resched_pdu_fail);
        ble_npl_eventq_put(&g_ble_ll_data.ll_evq, &advsm->adv_txdone_ev);
    }
}

static void
ble_ll_adv_event_done(struct ble_npl_event *ev)
{
    ble_ll_adv_done(ble_npl_event_get_arg(ev));
}

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
/**
 * Called when auxiliary packet is txd on secondary channel
 *
 * Context: Link Layer task.
 *
 * @param ev
 */
static void
ble_ll_adv_sec_done(struct ble_ll_adv_sm *advsm)
{
    struct ble_ll_adv_aux *aux;
    struct ble_ll_adv_aux *aux_next;

    assert(advsm->adv_enabled);
    assert(advsm->aux_active);

    aux = AUX_CURRENT(advsm);
    aux_next = AUX_NEXT(advsm);

    /* We don't need RF anymore */
    ble_ll_rfmgmt_release();

    if (advsm->aux_dropped) {
        ble_ll_adv_drop_event(advsm);
        return;
    }

    if (advsm->aux_not_scanned) {
        ble_ll_sched_rmv_elem(&aux_next->sch);
    }

    /* Remove anything else scheduled for secondary channel */
    ble_ll_sched_rmv_elem(&aux->sch);
    ble_npl_eventq_remove(&g_ble_ll_data.ll_evq, &advsm->adv_sec_txdone_ev);

    /* Stop advertising due to transmitting connection response */
    if (advsm->flags & BLE_LL_ADV_SM_FLAG_CONN_RSP_TXD) {
        ble_ll_adv_sm_stop(advsm);
        return;
    }

    /* If we have next AUX scheduled, try to schedule another one */
    if (aux_next->sch.enqueued) {
        advsm->aux_index ^= 1;
        advsm->aux_first_pdu = 0;
        ble_ll_adv_aux_schedule_next(advsm);
        return;
    }

    ble_ll_scan_chk_resume();

    /* Check if advertising timed out */
    if (advsm->duration && (advsm->adv_pdu_start_time >= advsm->adv_end_time)) {
        ble_ll_adv_sm_stop_timeout(advsm);
        return;
    }

    if (advsm->events_max && (advsm->events >= advsm->events_max)) {
        ble_ll_adv_sm_stop_limit_reached(advsm);
        return;
    }

    advsm->aux_active = 0;
    ble_ll_adv_update_adv_scan_rsp_data(advsm);
    ble_ll_adv_reschedule_event(advsm);
}

static void
ble_ll_adv_sec_event_done(struct ble_npl_event *ev)
{
    ble_ll_adv_sec_done(ble_npl_event_get_arg(ev));
}
#endif

static void
ble_ll_adv_make_done(struct ble_ll_adv_sm *advsm, struct ble_mbuf_hdr *hdr)
{
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    if (BLE_MBUF_HDR_EXT_ADV_SEC(hdr)) {
        assert(!(advsm->props & BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY));
        assert(ble_ll_adv_active_chanset_is_sec(advsm));
        ble_ll_adv_active_chanset_clear(advsm);
        ble_ll_adv_sec_done(advsm);
    } else {
        assert(ble_ll_adv_active_chanset_is_pri(advsm));
        ble_ll_adv_active_chanset_clear(advsm);
        ble_ll_adv_done(advsm);
    }
#else
    ble_ll_adv_active_chanset_clear(advsm);
    ble_ll_adv_done(advsm);
#endif
}

/**
 * Checks if the controller can change the whitelist. If advertising is enabled
 * and is using the whitelist the controller is not allowed to change the
 * whitelist.
 *
 * @return int 0: not allowed to change whitelist; 1: change allowed.
 */
int
ble_ll_adv_can_chg_whitelist(void)
{
    struct ble_ll_adv_sm *advsm;
    int rc;
    int i;

    rc = 1;
    for (i = 0; i < BLE_ADV_INSTANCES; ++i) {
        advsm = &g_ble_ll_adv_sm[i];
        if (advsm->adv_enabled &&
            (advsm->adv_filter_policy != BLE_HCI_ADV_FILT_NONE)) {
            rc = 0;
            break;
        }
    }

    return rc;
}

/**
 * Sends the connection complete event when advertising a connection starts.
 *
 * @return uint8_t* Pointer to event buffer
 */
void
ble_ll_adv_send_conn_comp_ev(struct ble_ll_conn_sm *connsm,
                             struct ble_mbuf_hdr *rxhdr)
{
    uint8_t *evbuf;
    struct ble_ll_adv_sm *advsm;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    advsm = (struct ble_ll_adv_sm *)rxhdr->rxinfo.user_data;
#else
    advsm = &g_ble_ll_adv_sm[0];
#endif

    evbuf = advsm->conn_comp_ev;
    assert(evbuf != NULL);
    advsm->conn_comp_ev = NULL;

    ble_ll_conn_comp_event_send(connsm, BLE_ERR_SUCCESS, evbuf, advsm);

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CSA2)
    ble_ll_hci_ev_le_csa(connsm);
#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    if (ble_ll_hci_adv_mode_ext()) {
        ble_ll_hci_ev_send_adv_set_terminated(0, advsm->adv_instance,
                                          connsm->conn_handle, advsm->events);
    }
#endif
}

/**
 * Returns the local resolvable private address currently being using by
 * the advertiser
 *
 * @return uint8_t*
 */
uint8_t *
ble_ll_adv_get_local_rpa(struct ble_ll_adv_sm *advsm)
{
    uint8_t *rpa = NULL;

    if (advsm->own_addr_type > BLE_HCI_ADV_OWN_ADDR_RANDOM) {
        if ((advsm->flags & BLE_LL_ADV_SM_FLAG_TX_ADD) &&
                                    ble_ll_is_rpa(advsm->adva, 1)) {
            rpa = advsm->adva;
        }
    }

    return rpa;
}

/**
 * Returns the peer resolvable private address of last device connecting to us
 *
 * @return uint8_t*
 */
uint8_t *
ble_ll_adv_get_peer_rpa(struct ble_ll_adv_sm *advsm)
{
    /* XXX: should this go into IRK list or connection? */
    return advsm->adv_rpa;
}

/**
 * Called when the LL wait for response timer expires while in the advertising
 * state. Disables the phy and
 *
 */
void
ble_ll_adv_wfr_timer_exp(void)
{
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    g_ble_ll_cur_adv_sm->aux_not_scanned = 1;
#endif

    ble_phy_disable();
    ble_ll_adv_tx_done(g_ble_ll_cur_adv_sm);
}

/**
 * Reset the advertising state machine.
 *
 * Context: Link Layer task
 *
 */
void
ble_ll_adv_reset(void)
{
    int i;
    struct ble_ll_adv_sm *advsm;

    for (i = 0; i < BLE_ADV_INSTANCES; ++i) {
        advsm = &g_ble_ll_adv_sm[i];

        /* Stop advertising state machine */
        ble_ll_adv_sm_stop(advsm);

        /* clear any data present */
        os_mbuf_free_chain(advsm->adv_data);
        os_mbuf_free_chain(advsm->scan_rsp_data);

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV)
        /* Stop periodic advertising state machine */
        ble_ll_adv_sm_stop_periodic(advsm);

        /* clear any periodic data present */
        os_mbuf_free_chain(advsm->periodic_adv_data);
#endif

        /* re-initialize the advertiser state machine */
        ble_ll_adv_sm_init(advsm);
    }
}

/* Called to determine if advertising is enabled.
 */
uint8_t
ble_ll_adv_enabled(void)
{
    int i;

    for (i = 0; i < BLE_ADV_INSTANCES; i++) {
        if (g_ble_ll_adv_sm[i].adv_enabled) {
            return 1;
        }
    }

    return 0;
}

static void
ble_ll_adv_sm_init(struct ble_ll_adv_sm *advsm)
{
    memset(advsm, 0, sizeof(struct ble_ll_adv_sm));

    advsm->adv_itvl_min = BLE_HCI_ADV_ITVL_DEF;
    advsm->adv_itvl_max = BLE_HCI_ADV_ITVL_DEF;
    advsm->adv_chanmask = BLE_HCI_ADV_CHANMASK_DEF;

    /* Initialize advertising tx done event */
    ble_npl_event_init(&advsm->adv_txdone_ev, ble_ll_adv_event_done, advsm);
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    ble_npl_event_init(&advsm->adv_sec_txdone_ev, ble_ll_adv_sec_event_done, advsm);
#endif
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV)
    ble_npl_event_init(&advsm->adv_periodic_txdone_ev,
                       ble_ll_adv_periodic_event_done, advsm);
#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    /* Initialize aux schedulers */
    advsm->aux_active = 0;
    advsm->aux[0].sch.cb_arg = advsm;
    advsm->aux[0].sch.sched_cb = ble_ll_adv_secondary_tx_start_cb;
    advsm->aux[0].sch.sched_type = BLE_LL_SCHED_TYPE_ADV;
    advsm->aux[1].sch.cb_arg = advsm;
    advsm->aux[1].sch.sched_cb = ble_ll_adv_secondary_tx_start_cb;
    advsm->aux[1].sch.sched_type = BLE_LL_SCHED_TYPE_ADV;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV)
    /* Initialize sync schedulers */
    advsm->periodic_sync_active = 0;
    advsm->periodic_sync[0].sch.cb_arg = advsm;
    advsm->periodic_sync[0].sch.sched_cb = ble_ll_adv_sync_tx_start_cb;
    advsm->periodic_sync[0].sch.sched_type = BLE_LL_SCHED_TYPE_PERIODIC;
    advsm->periodic_sync[1].sch.cb_arg = advsm;
    advsm->periodic_sync[1].sch.sched_cb = ble_ll_adv_sync_tx_start_cb;
    advsm->periodic_sync[1].sch.sched_type = BLE_LL_SCHED_TYPE_PERIODIC;
#endif
#endif

    /* Configure instances to be legacy on start */
    advsm->props |= BLE_HCI_LE_SET_EXT_ADV_PROP_SCANNABLE;
    advsm->props |= BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY;
}

/**
 * Initialize the advertising functionality of a BLE device. This should
 * be called once on initialization
 */
void
ble_ll_adv_init(void)
{
    int i;

    /* Set default advertising parameters */
    for (i = 0; i < BLE_ADV_INSTANCES; ++i) {
        ble_ll_adv_sm_init(&g_ble_ll_adv_sm[i]);
    }
}

#endif
