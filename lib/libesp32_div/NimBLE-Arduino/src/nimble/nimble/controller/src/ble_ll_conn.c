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
#include "nimble/nimble/include/nimble/ble.h"
#include "nimble/nimble/include/nimble/nimble_opt.h"
#include "nimble/nimble/include/nimble/hci_common.h"
#include "nimble/nimble/include/nimble/ble_hci_trans.h"

#if defined(ARDUINO_ARCH_NRF5) && defined(NRF51)
#include "nimble/nimble/drivers/nrf51/include/ble/xcvr.h"
#elif defined(ARDUINO_ARCH_NRF5) && defined(NRF52_SERIES)
#include "nimble/nimble/drivers/nrf52/include/ble/xcvr.h"
#endif

#include "../include/controller/ble_ll.h"
#include "../include/controller/ble_ll_hci.h"
#include "../include/controller/ble_ll_scan.h"
#include "../include/controller/ble_ll_whitelist.h"
#include "../include/controller/ble_ll_sched.h"
#include "../include/controller/ble_ll_ctrl.h"
#include "../include/controller/ble_ll_resolv.h"
#include "../include/controller/ble_ll_adv.h"
#include "../include/controller/ble_ll_trace.h"
#include "../include/controller/ble_ll_rfmgmt.h"
#include "../include/controller/ble_phy.h"
#include "../include/controller/ble_hw.h"
#include "../include/controller/ble_ll_utils.h"
#include "ble_ll_conn_priv.h"

#if (BLETEST_THROUGHPUT_TEST == 1)
extern void bletest_completed_pkt(uint16_t handle);
#endif

/* XXX TODO
 * 1) I think if we are initiating and we already have a connection with
 * a device that we will still try and connect to it. Fix this.
 *  -> This is true. There are a couple things to do
 *      i) When a connection create is issued, if we already are connected
 *      deny it. BLE ERROR = 0x0B (ACL connection exists).
 *      ii) If we receive an advertisement while initiating and want to send
 *      a connect request to the device, make sure we dont have it.
 *      iii) I think I need to do something like this: I am initiating and
 *      advertising. Suppose the device I want to connect to sends me a connect
 *      request because I am advertising? What happens to connection? Deal
 *      with this!
 *
 * 2) Make sure we check incoming data packets for size and all that. You
 * know, supported octets and all that. For both rx and tx.
 *
 * 3) Make sure we are setting the schedule end time properly for both slave
 * and master. We should just set this to the end of the connection event.
 * We might want to guarantee a IFS time as well since the next event needs
 * to be scheduled prior to the start of the event to account for the time it
 * takes to get a frame ready (which is pretty much the IFS time).
 *
 * 4) looks like the current code will allow the 1st packet in a
 * connection to extend past the end of the allocated connection end
 * time. That is not good. Need to deal with that. Need to extend connection
 * end time.
 *
 * 6) Use error code 0x3E correctly! Connection failed to establish. If you
 * read the LE connection complete event, it says that if the connection
 * fails to be established that the connection complete event gets sent to
 * the host that issued the create connection. Need to resolve this.
 *
 * 7) How does peer address get set if we are using whitelist? Look at filter
 * policy and make sure you are doing this correctly.
 *
 * 8) Right now I use a fixed definition for required slots. CHange this.
 *
 * 10) See what connection state machine elements are purely master and
 * purely slave. We can make a union of them.
 *
 * 11) Not sure I am dealing with the connection terminate timeout perfectly.
 * I may extend a connection event too long although if it is always in terms
 * of connection events I am probably fine. Checking at end that the next
 * connection event will occur past terminate timeould would be fine.
 *
 * 12) When a slave receives a data packet in a connection it has to send a
 * response. Well, it should. If this packet will overrun the next scheduled
 * event, what should we do? Transmit anyway? Not transmit? For now, we just
 * transmit.
 *
 * 32kHz crystal
 * 1) When scheduling, I need to make sure I have time between
 * this one and the next. Should I deal with this in the sched. Or
 * is this basically accounted for given a slot? I really just need to
 * make sure everything is over N ticks before the next sched start!
 * Just add to end time?
 *
 * 2) I think one way to handle the problem of losing up to a microsecond
 * every time we call ble_ll_conn_next_event in a loop is to do everything by
 * keeping track of last anchor point. Would need last anchor usecs too. I guess
 * we could also keep last anchor usecs as a uint32 or something and when we
 * do the next event keep track of the residual using a different ticks to
 * usecs calculation. Not sure.
 */

/*
 * XXX: How should we deal with a late connection event? We need to determine
 * what we want to do under the following cases:
 *  1) The current connection event has not ended but a schedule item starts
 */

/* This is a dummy structure we use for the empty PDU */
struct ble_ll_empty_pdu
{
    struct os_mbuf om;
    struct os_mbuf_pkthdr pkt_hdr;
    struct ble_mbuf_hdr ble_hdr;
};

/* We cannot have more than 254 connections given our current implementation */
#if (MYNEWT_VAL(BLE_MAX_CONNECTIONS) >= 255)
    #error "Maximum # of connections is 254"
#endif

/* Global connection complete event. Used when initiating */
uint8_t *g_ble_ll_conn_comp_ev;

/* Global LL connection parameters */
struct ble_ll_conn_global_params g_ble_ll_conn_params;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV_SYNC_TRANSFER)
/* Global default sync transfer params */
struct ble_ll_conn_sync_transfer_params g_ble_ll_conn_sync_transfer_params;
#endif

/* Pointer to connection state machine we are trying to create */
struct ble_ll_conn_sm *g_ble_ll_conn_create_sm;

/* Pointer to current connection */
struct ble_ll_conn_sm *g_ble_ll_conn_cur_sm;

/* Connection state machine array */
struct ble_ll_conn_sm g_ble_ll_conn_sm[MYNEWT_VAL(BLE_MAX_CONNECTIONS)];

/* List of active connections */
struct ble_ll_conn_active_list g_ble_ll_conn_active_list;

/* List of free connections */
struct ble_ll_conn_free_list g_ble_ll_conn_free_list;

STATS_SECT_START(ble_ll_conn_stats)
    STATS_SECT_ENTRY(cant_set_sched)
    STATS_SECT_ENTRY(conn_ev_late)
    STATS_SECT_ENTRY(wfr_expirations)
    STATS_SECT_ENTRY(handle_not_found)
    STATS_SECT_ENTRY(no_conn_sm)
    STATS_SECT_ENTRY(no_free_conn_sm)
    STATS_SECT_ENTRY(rx_data_pdu_no_conn)
    STATS_SECT_ENTRY(rx_data_pdu_bad_aa)
    STATS_SECT_ENTRY(slave_rxd_bad_conn_req_params)
    STATS_SECT_ENTRY(slave_ce_failures)
    STATS_SECT_ENTRY(data_pdu_rx_dup)
    STATS_SECT_ENTRY(data_pdu_txg)
    STATS_SECT_ENTRY(data_pdu_txf)
    STATS_SECT_ENTRY(conn_req_txd)
    STATS_SECT_ENTRY(l2cap_enqueued)
    STATS_SECT_ENTRY(rx_ctrl_pdus)
    STATS_SECT_ENTRY(rx_l2cap_pdus)
    STATS_SECT_ENTRY(rx_l2cap_bytes)
    STATS_SECT_ENTRY(rx_malformed_ctrl_pdus)
    STATS_SECT_ENTRY(rx_bad_llid)
    STATS_SECT_ENTRY(tx_ctrl_pdus)
    STATS_SECT_ENTRY(tx_ctrl_bytes)
    STATS_SECT_ENTRY(tx_l2cap_pdus)
    STATS_SECT_ENTRY(tx_l2cap_bytes)
    STATS_SECT_ENTRY(tx_empty_pdus)
    STATS_SECT_ENTRY(mic_failures)
    STATS_SECT_ENTRY(sched_start_in_idle)
    STATS_SECT_ENTRY(sched_end_in_idle)
    STATS_SECT_ENTRY(conn_event_while_tmo)
STATS_SECT_END
STATS_SECT_DECL(ble_ll_conn_stats) ble_ll_conn_stats;

STATS_NAME_START(ble_ll_conn_stats)
    STATS_NAME(ble_ll_conn_stats, cant_set_sched)
    STATS_NAME(ble_ll_conn_stats, conn_ev_late)
    STATS_NAME(ble_ll_conn_stats, wfr_expirations)
    STATS_NAME(ble_ll_conn_stats, handle_not_found)
    STATS_NAME(ble_ll_conn_stats, no_conn_sm)
    STATS_NAME(ble_ll_conn_stats, no_free_conn_sm)
    STATS_NAME(ble_ll_conn_stats, rx_data_pdu_no_conn)
    STATS_NAME(ble_ll_conn_stats, rx_data_pdu_bad_aa)
    STATS_NAME(ble_ll_conn_stats, slave_rxd_bad_conn_req_params)
    STATS_NAME(ble_ll_conn_stats, slave_ce_failures)
    STATS_NAME(ble_ll_conn_stats, data_pdu_rx_dup)
    STATS_NAME(ble_ll_conn_stats, data_pdu_txg)
    STATS_NAME(ble_ll_conn_stats, data_pdu_txf)
    STATS_NAME(ble_ll_conn_stats, conn_req_txd)
    STATS_NAME(ble_ll_conn_stats, l2cap_enqueued)
    STATS_NAME(ble_ll_conn_stats, rx_ctrl_pdus)
    STATS_NAME(ble_ll_conn_stats, rx_l2cap_pdus)
    STATS_NAME(ble_ll_conn_stats, rx_l2cap_bytes)
    STATS_NAME(ble_ll_conn_stats, rx_malformed_ctrl_pdus)
    STATS_NAME(ble_ll_conn_stats, rx_bad_llid)
    STATS_NAME(ble_ll_conn_stats, tx_ctrl_pdus)
    STATS_NAME(ble_ll_conn_stats, tx_ctrl_bytes)
    STATS_NAME(ble_ll_conn_stats, tx_l2cap_pdus)
    STATS_NAME(ble_ll_conn_stats, tx_l2cap_bytes)
    STATS_NAME(ble_ll_conn_stats, tx_empty_pdus)
    STATS_NAME(ble_ll_conn_stats, mic_failures)
    STATS_NAME(ble_ll_conn_stats, sched_start_in_idle)
    STATS_NAME(ble_ll_conn_stats, sched_end_in_idle)
    STATS_NAME(ble_ll_conn_stats, conn_event_while_tmo)
STATS_NAME_END(ble_ll_conn_stats)

static void ble_ll_conn_event_end(struct ble_npl_event *ev);

#if (BLE_LL_BT5_PHY_SUPPORTED == 1)
/**
 * Checks to see if we should start a PHY update procedure
 *
 * If current phy is not one of the preferred we need to start control
 * procedure.
 *
 * XXX: we could also decide to change the PHY if RSSI is really good
 * and we are currently at 1Mbps or lower data rate and we could use
 * a higher data rate.
 *
 * @param connsm
 * @return 0: success; -1: no phy update procedure started
 */
int
ble_ll_conn_chk_phy_upd_start(struct ble_ll_conn_sm *csm)
{
    int rc;

    /* If no host preferences or  */
    if (((csm->phy_data.host_pref_tx_phys_mask == 0) &&
         (csm->phy_data.host_pref_rx_phys_mask == 0)) ||
        ((csm->phy_data.host_pref_tx_phys_mask & CONN_CUR_TX_PHY_MASK(csm)) &&
         (csm->phy_data.host_pref_rx_phys_mask & CONN_CUR_RX_PHY_MASK(csm)))) {
        rc = -1;
    } else {
        csm->phy_data.req_pref_tx_phys_mask = csm->phy_data.host_pref_tx_phys_mask;
        csm->phy_data.req_pref_rx_phys_mask = csm->phy_data.host_pref_rx_phys_mask;
        ble_ll_ctrl_proc_start(csm, BLE_LL_CTRL_PROC_PHY_UPDATE);
        rc = 0;
    }

    return rc;
}
#endif

static void
ble_ll_conn_calc_itvl_ticks(struct ble_ll_conn_sm *connsm)
{
    uint32_t ticks;
    uint32_t usecs;

    /*
     * Precalculate the number of ticks and remaining microseconds for
     * the connection interval
     */
    usecs = connsm->conn_itvl * BLE_LL_CONN_ITVL_USECS;
    ticks = os_cputime_usecs_to_ticks(usecs);
    connsm->conn_itvl_usecs = (uint8_t)(usecs -
                                        os_cputime_ticks_to_usecs(ticks));
    if (connsm->conn_itvl_usecs == 31) {
        connsm->conn_itvl_usecs = 0;
        ++ticks;
    }
    connsm->conn_itvl_ticks = ticks;
}

/**
 * Get the event buffer allocated to send the connection complete event
 * when we are initiating.
 *
 * @return uint8_t*
 */
static uint8_t *
ble_ll_init_get_conn_comp_ev(void)
{
    uint8_t *evbuf;

    evbuf = g_ble_ll_conn_comp_ev;
    BLE_LL_ASSERT(evbuf != NULL);
    g_ble_ll_conn_comp_ev = NULL;

    return evbuf;
}

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
/**
 * Called to determine if the received PDU is an empty PDU or not.
 */
static int
ble_ll_conn_is_empty_pdu(uint8_t *rxbuf)
{
    int rc;
    uint8_t llid;

    llid = rxbuf[0] & BLE_LL_DATA_HDR_LLID_MASK;
    if ((llid == BLE_LL_LLID_DATA_FRAG) && (rxbuf[1] == 0)) {
        rc = 1;
    } else {
        rc = 0;
    }
    return rc;
}
#endif

/**
 * Called to return the currently running connection state machine end time.
 * Always called when interrupts are disabled.
 *
 * @return int 0: s1 is not least recently used. 1: s1 is least recently used
 */
int
ble_ll_conn_is_lru(struct ble_ll_conn_sm *s1, struct ble_ll_conn_sm *s2)
{
    int rc;

    /* Set time that we last serviced the schedule */
    if ((int32_t)(s1->last_scheduled - s2->last_scheduled) < 0) {
        rc = 1;
    } else {
        rc = 0;
    }

    return rc;
}

/**
 * Called to return the currently running connection state machine end time.
 * Always called when interrupts are disabled.
 *
 * @return uint32_t
 */
uint32_t
ble_ll_conn_get_ce_end_time(void)
{
    uint32_t ce_end_time;

    if (g_ble_ll_conn_cur_sm) {
        ce_end_time = g_ble_ll_conn_cur_sm->ce_end_time;
    } else {
        ce_end_time = os_cputime_get32();
    }
    return ce_end_time;
}

/**
 * Called when connection state machine needs to halt. This function will:
 *  -> Disable the PHY, which will prevent any transmit/receive interrupts.
 *  -> Disable the wait for response timer, if running.
 *  -> Remove the connection state machine from the scheduler.
 *  -> Sets the Link Layer state to standby.
 *  -> Sets the current state machine to NULL.
 *
 *  NOTE: the ordering of these function calls is important! We have to stop
 *  the PHY and remove the schedule item before we can set the state to
 *  standby and set the current state machine pointer to NULL.
 */
static void
ble_ll_conn_halt(void)
{
    ble_phy_disable();
    ble_ll_state_set(BLE_LL_STATE_STANDBY);
    g_ble_ll_conn_cur_sm = NULL;
}

/**
 * Called when the current connection state machine is no longer being used.
 */
static void
ble_ll_conn_current_sm_over(struct ble_ll_conn_sm *connsm)
{

    ble_ll_conn_halt();

    /*
     * NOTE: the connection state machine may be NULL if we are calling
     * this when we are ending the connection. In that case, there is no
     * need to post to the LL the connection event end event
     */
    if (connsm) {
        ble_ll_event_send(&connsm->conn_ev_end);
    }
}

/**
 * Given a handle, find an active connection matching the handle
 *
 * @param handle
 *
 * @return struct ble_ll_conn_sm*
 */
struct ble_ll_conn_sm *
ble_ll_conn_find_active_conn(uint16_t handle)
{
    struct ble_ll_conn_sm *connsm;

    connsm = NULL;
    if ((handle != 0) && (handle <= MYNEWT_VAL(BLE_MAX_CONNECTIONS))) {
        connsm = &g_ble_ll_conn_sm[handle - 1];
        if (connsm->conn_state == BLE_LL_CONN_STATE_IDLE) {
            connsm = NULL;
        }
    }
    return connsm;
}

/**
 * Get a connection state machine.
 */
struct ble_ll_conn_sm *
ble_ll_conn_sm_get(void)
{
    struct ble_ll_conn_sm *connsm;

    connsm = STAILQ_FIRST(&g_ble_ll_conn_free_list);
    if (connsm) {
        STAILQ_REMOVE_HEAD(&g_ble_ll_conn_free_list, free_stqe);
    } else {
        STATS_INC(ble_ll_conn_stats, no_free_conn_sm);
    }

    return connsm;
}

static uint8_t
ble_ll_conn_calc_dci_csa1(struct ble_ll_conn_sm *conn)
{
    uint8_t curchan;
    uint8_t remap_index;
    uint8_t bitpos;

    /* Get next unmapped channel */
    curchan = conn->last_unmapped_chan + conn->hop_inc;
    if (curchan > BLE_PHY_NUM_DATA_CHANS) {
        curchan -= BLE_PHY_NUM_DATA_CHANS;
    }

    /* Save unmapped channel */
    conn->last_unmapped_chan = curchan;

    /* Is this a valid channel? */
    bitpos = 1 << (curchan & 0x07);
    if (conn->chanmap[curchan >> 3] & bitpos) {
        return curchan;
    }

    /* Calculate remap index */
    remap_index = curchan % conn->num_used_chans;

    return ble_ll_utils_remapped_channel(remap_index, conn->chanmap);
}

/**
 * Determine data channel index to be used for the upcoming/current
 * connection event
 *
 * @param conn
 * @param latency Used only for CSA #1
 *
 * @return uint8_t
 */
uint8_t
ble_ll_conn_calc_dci(struct ble_ll_conn_sm *conn, uint16_t latency)
{
    uint8_t index;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CSA2)
    if (CONN_F_CSA2_SUPP(conn)) {
        return ble_ll_utils_calc_dci_csa2(conn->event_cntr, conn->channel_id,
                                          conn->num_used_chans, conn->chanmap);
    }
#endif

    index = conn->data_chan_index;

    while (latency > 0) {
        index = ble_ll_conn_calc_dci_csa1(conn);
        latency--;
    }

    return index;
}

/**
 * Called when we are in the connection state and the wait for response timer
 * fires off.
 *
 * Context: Interrupt
 */
void
ble_ll_conn_wfr_timer_exp(void)
{
    struct ble_ll_conn_sm *connsm;

    connsm = g_ble_ll_conn_cur_sm;
    ble_ll_conn_current_sm_over(connsm);
    STATS_INC(ble_ll_conn_stats, wfr_expirations);
}

void
ble_ll_conn_reset_pending_aux_conn_rsp(void)
{
#if !MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    return;
#endif
    struct ble_ll_conn_sm *connsm;

    connsm = g_ble_ll_conn_create_sm;
    if (!connsm) {
        return;
    }

    if (CONN_F_AUX_CONN_REQ(connsm)) {
        STATS_INC(ble_ll_stats, aux_conn_rsp_err);
        CONN_F_CONN_REQ_TXD(connsm) = 0;
        CONN_F_AUX_CONN_REQ(connsm) = 0;
        ble_ll_sched_rmv_elem(&connsm->conn_sch);
        return;
    }

    return;
}

bool
ble_ll_conn_init_pending_aux_conn_rsp(void)
{
#if !MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    return false;
#endif
    struct ble_ll_conn_sm *connsm;

    connsm = g_ble_ll_conn_create_sm;
    if (!connsm) {
        return false;
    }

    return CONN_F_AUX_CONN_REQ(connsm);
}

void
ble_ll_conn_init_wfr_timer_exp(void)
{
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    struct ble_ll_conn_sm *connsm;

    connsm = g_ble_ll_conn_create_sm;
    if (!connsm) {
        return;
    }

    ble_ll_conn_reset_pending_aux_conn_rsp();
    connsm->inita_identity_used = 0;

    ble_ll_scan_interrupted(connsm->scansm);

#endif
}
/**
 * Callback for slave when it transmits a data pdu and the connection event
 * ends after the transmission.
 *
 * Context: Interrupt
 *
 * @param sch
 *
 */
static void
ble_ll_conn_wait_txend(void *arg)
{
    struct ble_ll_conn_sm *connsm;

    connsm = (struct ble_ll_conn_sm *)arg;
    ble_ll_conn_current_sm_over(connsm);
}

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
static void
ble_ll_conn_start_rx_encrypt(void *arg)
{
    struct ble_ll_conn_sm *connsm;

    connsm = (struct ble_ll_conn_sm *)arg;
    CONN_F_ENCRYPTED(connsm) = 1;
    ble_phy_encrypt_enable(connsm->enc_data.rx_pkt_cntr,
                           connsm->enc_data.iv,
                           connsm->enc_data.enc_block.cipher_text,
                           !CONN_IS_MASTER(connsm));
}

static void
ble_ll_conn_start_rx_unencrypt(void *arg)
{
    struct ble_ll_conn_sm *connsm;

    connsm = (struct ble_ll_conn_sm *)arg;
    CONN_F_ENCRYPTED(connsm) = 0;
    ble_phy_encrypt_disable();
}

static void
ble_ll_conn_txend_encrypt(void *arg)
{
    struct ble_ll_conn_sm *connsm;

    connsm = (struct ble_ll_conn_sm *)arg;
    CONN_F_ENCRYPTED(connsm) = 1;
    ble_ll_conn_current_sm_over(connsm);
}

static void
ble_ll_conn_rxend_unencrypt(void *arg)
{
    struct ble_ll_conn_sm *connsm;

    connsm = (struct ble_ll_conn_sm *)arg;
    CONN_F_ENCRYPTED(connsm) = 0;
    ble_ll_conn_current_sm_over(connsm);
}

static void
ble_ll_conn_continue_rx_encrypt(void *arg)
{
    struct ble_ll_conn_sm *connsm;

    connsm = (struct ble_ll_conn_sm *)arg;
    ble_phy_encrypt_set_pkt_cntr(connsm->enc_data.rx_pkt_cntr,
                                 !CONN_IS_MASTER(connsm));
}
#endif

/**
 * Returns the cputime of the next scheduled item on the scheduler list or
 * when the current connection will start its next interval (whichever is
 * earlier). This API is called when determining at what time we should end
 * the current connection event. The current connection event must end before
 * the next scheduled item. However, the current connection itself is not
 * in the scheduler list! Thus, we need to calculate the time at which the
 * next connection will start (the schedule start time; not the anchor point)
 * and not overrun it.
 *
 * Context: Interrupt
 *
 * @param connsm
 *
 * @return uint32_t
 */
static uint32_t
ble_ll_conn_get_next_sched_time(struct ble_ll_conn_sm *connsm)
{
#if MYNEWT_VAL(BLE_LL_STRICT_CONN_SCHEDULING)
    uint32_t ce_end;
    ce_end = connsm->ce_end_time;
#else
    uint32_t ce_end;
    uint32_t next_sched_time;

    /* Calculate time at which next connection event will start */
    /* NOTE: We dont care if this time is tick short. */
    ce_end = connsm->anchor_point + connsm->conn_itvl_ticks -
        g_ble_ll_sched_offset_ticks;
    if ((connsm->anchor_point_usecs + connsm->conn_itvl_usecs) >= 31) {
        ++ce_end;
    }

    if (ble_ll_sched_next_time(&next_sched_time)) {
        if (CPUTIME_LT(next_sched_time, ce_end)) {
            ce_end = next_sched_time;
        }
    }
#endif

    return ce_end;
}

/**
 * Called to check if certain connection state machine flags have been
 * set.
 *
 * @param connsm
 */
static void
ble_ll_conn_chk_csm_flags(struct ble_ll_conn_sm *connsm)
{
    uint8_t update_status;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
    if (connsm->csmflags.cfbit.send_ltk_req) {
        /*
         * Send Long term key request event to host. If masked, we need to
         * send a REJECT_IND.
         */
        if (ble_ll_hci_ev_ltk_req(connsm)) {
            ble_ll_ctrl_reject_ind_send(connsm, BLE_LL_CTRL_ENC_REQ,
                                        BLE_ERR_PINKEY_MISSING);
        }
        connsm->csmflags.cfbit.send_ltk_req = 0;
    }
#endif

    /*
     * There are two cases where this flag gets set:
     * 1) A connection update procedure was started and the event counter
     * has passed the instant.
     * 2) We successfully sent the reject reason.
     */
    if (connsm->csmflags.cfbit.host_expects_upd_event) {
        update_status = BLE_ERR_SUCCESS;
        if (IS_PENDING_CTRL_PROC(connsm, BLE_LL_CTRL_PROC_CONN_UPDATE)) {
            ble_ll_ctrl_proc_stop(connsm, BLE_LL_CTRL_PROC_CONN_UPDATE);
        } else {
            if (IS_PENDING_CTRL_PROC(connsm, BLE_LL_CTRL_PROC_CONN_PARAM_REQ)) {
                ble_ll_ctrl_proc_stop(connsm, BLE_LL_CTRL_PROC_CONN_PARAM_REQ);
                update_status = connsm->reject_reason;
            }
        }
        ble_ll_hci_ev_conn_update(connsm, update_status);
        connsm->csmflags.cfbit.host_expects_upd_event = 0;
    }

    /* Check if we need to send PHY update complete event */
#if (BLE_LL_BT5_PHY_SUPPORTED == 1)
    if (CONN_F_PHY_UPDATE_EVENT(connsm)) {
        if (!ble_ll_hci_ev_phy_update(connsm, BLE_ERR_SUCCESS)) {
            /* Sent event. Clear flag */
            CONN_F_PHY_UPDATE_EVENT(connsm) = 0;
        }
    }
#endif
}

/**
 * Called when we want to send a data channel pdu inside a connection event.
 *
 * Context: interrupt
 *
 * @param connsm
 *
 * @return int 0: success; otherwise failure to transmit
 */
static uint16_t
ble_ll_conn_adjust_pyld_len(struct ble_ll_conn_sm *connsm, uint16_t pyld_len)
{
    uint16_t phy_max_tx_octets;
    uint16_t ret;

#if (BLE_LL_BT5_PHY_SUPPORTED == 1)
    uint8_t phy_mode;

    if (connsm->phy_tx_transition) {
        phy_mode = ble_ll_phy_to_phy_mode(connsm->phy_tx_transition,
                                          connsm->phy_data.phy_options);
    } else {
        phy_mode = connsm->phy_data.tx_phy_mode;
    }

    phy_max_tx_octets = ble_ll_pdu_max_tx_octets_get(connsm->eff_max_tx_time,
                                                     phy_mode);

#else
    phy_max_tx_octets = ble_ll_pdu_max_tx_octets_get(connsm->eff_max_tx_time,
                                                     BLE_PHY_MODE_1M);
#endif

    ret = pyld_len;

    if (ret > connsm->eff_max_tx_octets) {
        ret = connsm->eff_max_tx_octets;
    }

    if (ret > phy_max_tx_octets) {
        ret = phy_max_tx_octets;
    }

    return ret;
}

static int
ble_ll_conn_tx_pdu(struct ble_ll_conn_sm *connsm)
{
    int rc;
    uint8_t md;
    uint8_t hdr_byte;
    uint8_t end_transition;
    uint8_t cur_txlen;
    uint8_t next_txlen;
    uint8_t cur_offset;
    uint16_t pktlen;
    uint32_t next_event_time;
    uint32_t ticks;
    struct os_mbuf *m;
    struct ble_mbuf_hdr *ble_hdr;
    struct os_mbuf_pkthdr *pkthdr = NULL;
    struct os_mbuf_pkthdr *nextpkthdr;
    struct ble_ll_empty_pdu empty_pdu;
    ble_phy_tx_end_func txend_func;
    int tx_phy_mode;
    uint8_t llid;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
    int is_ctrl;
    uint8_t opcode;
#endif

    /* For compiler warnings... */
    ble_hdr = NULL;
    m = NULL;
    md = 0;
    hdr_byte = BLE_LL_LLID_DATA_FRAG;

    if (connsm->csmflags.cfbit.terminate_ind_rxd) {
        /* We just received terminate indication.
         * Just send empty packet as an ACK
         */
        CONN_F_EMPTY_PDU_TXD(connsm) = 1;
        goto conn_tx_pdu;
    }

    /*
     * We need to check if we are retrying a pdu or if there is a pdu on
     * the transmit queue.
     */
    pkthdr = STAILQ_FIRST(&connsm->conn_txq);
    if (!connsm->cur_tx_pdu && !CONN_F_EMPTY_PDU_TXD(connsm) && !pkthdr) {
        CONN_F_EMPTY_PDU_TXD(connsm) = 1;
        goto conn_tx_pdu;
    }

    /*
     * If we dont have a pdu we have previously transmitted, take it off
     * the connection transmit queue
     */
    cur_offset = 0;
    if (!connsm->cur_tx_pdu && !CONN_F_EMPTY_PDU_TXD(connsm)) {
        /* Convert packet header to mbuf */
        m = OS_MBUF_PKTHDR_TO_MBUF(pkthdr);
        nextpkthdr = STAILQ_NEXT(pkthdr, omp_next);

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
        /*
         * If we are encrypting, we are only allowed to send certain
         * kinds of LL control PDU's. If none is enqueued, send empty pdu!
         */
        if (connsm->enc_data.enc_state > CONN_ENC_S_ENCRYPTED) {
            if (!ble_ll_ctrl_enc_allowed_pdu_tx(pkthdr)) {
                CONN_F_EMPTY_PDU_TXD(connsm) = 1;
                goto conn_tx_pdu;
            }

            /*
             * We will allow a next packet if it itself is allowed or we are
             * a slave and we are sending the START_ENC_RSP. The master has
             * to wait to receive the START_ENC_RSP from the slave before
             * packets can be let go.
             */
            if (nextpkthdr && !ble_ll_ctrl_enc_allowed_pdu_tx(nextpkthdr)
                && ((connsm->conn_role == BLE_LL_CONN_ROLE_MASTER) ||
                    !ble_ll_ctrl_is_start_enc_rsp(m))) {
                nextpkthdr = NULL;
            }
        }
#endif
        /* Take packet off queue*/
        STAILQ_REMOVE_HEAD(&connsm->conn_txq, omp_next);
        ble_hdr = BLE_MBUF_HDR_PTR(m);

        /*
         * We dequeued new packet for transmission.
         * If this is a data PDU we need to calculate payload length we can send
         * over current PHY. Effectively, this determines fragmentation of packet
         * into PDUs.
         * If this is a control PDU we send complete PDU as only data PDU can be
         * fragmented. We assume that checks (i.e. if remote supports such PDU)
         * were already performed before putting packet on queue.
         */
        llid = ble_hdr->txinfo.hdr_byte & BLE_LL_DATA_HDR_LLID_MASK;
        pktlen = pkthdr->omp_len;
        if (llid == BLE_LL_LLID_CTRL) {
            cur_txlen = pktlen;
        } else {
            cur_txlen = ble_ll_conn_adjust_pyld_len(connsm, pktlen);
        }
        ble_hdr->txinfo.pyld_len = cur_txlen;

        /* NOTE: header was set when first enqueued */
        hdr_byte = ble_hdr->txinfo.hdr_byte;
        connsm->cur_tx_pdu = m;
    } else {
        nextpkthdr = pkthdr;
        if (connsm->cur_tx_pdu) {
            m = connsm->cur_tx_pdu;
            ble_hdr = BLE_MBUF_HDR_PTR(m);
            pktlen = OS_MBUF_PKTLEN(m);
            cur_txlen = ble_hdr->txinfo.pyld_len;
            cur_offset = ble_hdr->txinfo.offset;
            if (cur_offset == 0) {
                hdr_byte = ble_hdr->txinfo.hdr_byte & BLE_LL_DATA_HDR_LLID_MASK;
            }
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
            if (connsm->enc_data.enc_state > CONN_ENC_S_ENCRYPTED) {
                /* We will allow a next packet if it itself is allowed */
                pkthdr = OS_MBUF_PKTHDR(connsm->cur_tx_pdu);
                if (nextpkthdr && !ble_ll_ctrl_enc_allowed_pdu_tx(nextpkthdr)
                    && ((connsm->conn_role == BLE_LL_CONN_ROLE_MASTER) ||
                        !ble_ll_ctrl_is_start_enc_rsp(connsm->cur_tx_pdu))) {
                    nextpkthdr = NULL;
                }
            }
#endif
        } else {
            /* Empty PDU here. NOTE: header byte gets set later */
            pktlen = 0;
            cur_txlen = 0;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
            if (connsm->enc_data.enc_state > CONN_ENC_S_ENCRYPTED) {
                /* We will allow a next packet if it itself is allowed */
                if (nextpkthdr && !ble_ll_ctrl_enc_allowed_pdu_tx(nextpkthdr)) {
                    nextpkthdr = NULL;
                }
            }
#endif
        }
    }

    /*
     * Set the more data data flag if we have more data to send and we
     * have not been asked to terminate
     */
    if (nextpkthdr || ((cur_offset + cur_txlen) < pktlen)) {
        /* Get next event time */
        next_event_time = ble_ll_conn_get_next_sched_time(connsm);

        /* XXX: TODO: need to check this with phy update procedure. There are
           limitations if we have started update */

        /*
         * Dont bother to set the MD bit if we cannot do the following:
         *  -> wait IFS, send the current frame.
         *  -> wait IFS, receive a maximum size frame.
         *  -> wait IFS, send the next frame.
         *  -> wait IFS, receive a maximum size frame.
         *
         *  For slave:
         *  -> wait IFS, send current frame.
         *  -> wait IFS, receive maximum size frame.
         *  -> wait IFS, send next frame.
         */
        if ((cur_offset + cur_txlen) < pktlen) {
            next_txlen = pktlen - (cur_offset + cur_txlen);
        } else {
            if (nextpkthdr->omp_len > connsm->eff_max_tx_octets) {
                next_txlen = connsm->eff_max_tx_octets;
            } else {
                next_txlen = nextpkthdr->omp_len;
            }
        }

        /*
         * XXX: this calculation is based on using the current time
         * and assuming the transmission will occur an IFS time from
         * now. This is not the most accurate especially if we have
         * received a frame and we are replying to it.
         */
#if BLE_LL_BT5_PHY_SUPPORTED
        tx_phy_mode = connsm->phy_data.tx_phy_mode;
#else
        tx_phy_mode = BLE_PHY_MODE_1M;
#endif

        ticks = (BLE_LL_IFS * 3) + connsm->eff_max_rx_time +
            ble_ll_pdu_tx_time_get(next_txlen, tx_phy_mode) +
            ble_ll_pdu_tx_time_get(cur_txlen, tx_phy_mode);

        if (connsm->conn_role == BLE_LL_CONN_ROLE_MASTER) {
            ticks += (BLE_LL_IFS + connsm->eff_max_rx_time);
        }

        ticks = os_cputime_usecs_to_ticks(ticks);
        if ((int32_t)((os_cputime_get32() + ticks) - next_event_time) < 0) {
            md = 1;
        }
     }

    /* If we send an empty PDU we need to initialize the header */
conn_tx_pdu:
    if (CONN_F_EMPTY_PDU_TXD(connsm)) {
        /*
         * This looks strange, but we dont use the data pointer in the mbuf
         * when we have an empty pdu.
         */
        m = (struct os_mbuf *)&empty_pdu;
        m->om_data = (uint8_t *)&empty_pdu;
        m->om_data += BLE_MBUF_MEMBLOCK_OVERHEAD;
        ble_hdr = &empty_pdu.ble_hdr;
        ble_hdr->txinfo.flags = 0;
        ble_hdr->txinfo.offset = 0;
        ble_hdr->txinfo.pyld_len = 0;
    }

    /* Set tx seqnum */
    if (connsm->tx_seqnum) {
        hdr_byte |= BLE_LL_DATA_HDR_SN_MASK;
    }

    /* If we have more data, set the bit */
    if (md) {
        hdr_byte |= BLE_LL_DATA_HDR_MD_MASK;
    }

    /* Set NESN (next expected sequence number) bit */
    if (connsm->next_exp_seqnum) {
        hdr_byte |= BLE_LL_DATA_HDR_NESN_MASK;
    }

    /* Set the header byte in the outgoing frame */
    ble_hdr->txinfo.hdr_byte = hdr_byte;

    /*
     * If we are a slave, check to see if this transmission will end the
     * connection event. We will end the connection event if we have
     * received a valid frame with the more data bit set to 0 and we dont
     * have more data.
     *
     * XXX: for a slave, we dont check to see if we can:
     *  -> wait IFS, rx frame from master (either big or small).
     *  -> wait IFS, send empty pdu or next pdu.
     *
     *  We could do this. Now, we just keep going and hope that we dont
     *  overrun next scheduled item.
     */
    if ((connsm->csmflags.cfbit.terminate_ind_rxd) ||
        ((connsm->conn_role == BLE_LL_CONN_ROLE_SLAVE) && (md == 0) &&
         (connsm->cons_rxd_bad_crc == 0) &&
         ((connsm->last_rxd_hdr_byte & BLE_LL_DATA_HDR_MD_MASK) == 0) &&
         !ble_ll_ctrl_is_terminate_ind(hdr_byte, m->om_data[0]))) {
        /* We will end the connection event */
        end_transition = BLE_PHY_TRANSITION_NONE;
        txend_func = ble_ll_conn_wait_txend;
    } else {
        /* Wait for a response here */
        end_transition = BLE_PHY_TRANSITION_TX_RX;
        txend_func = NULL;
    }

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
    llid = ble_hdr->txinfo.hdr_byte & BLE_LL_DATA_HDR_LLID_MASK;
    if (llid == BLE_LL_LLID_CTRL) {
        is_ctrl = 1;
        opcode = m->om_data[0];
    } else {
        is_ctrl = 0;
        opcode = 0;
    }

    if (is_ctrl && (opcode == BLE_LL_CTRL_START_ENC_RSP)) {
        /*
         * Both master and slave send the START_ENC_RSP encrypted and receive
         * encrypted
         */
        CONN_F_ENCRYPTED(connsm) = 1;
        connsm->enc_data.tx_encrypted = 1;
        ble_phy_encrypt_enable(connsm->enc_data.tx_pkt_cntr,
                               connsm->enc_data.iv,
                               connsm->enc_data.enc_block.cipher_text,
                               CONN_IS_MASTER(connsm));
    } else if (is_ctrl && (opcode == BLE_LL_CTRL_START_ENC_REQ)) {
        /*
         * Only the slave sends this and it gets sent unencrypted but
         * we receive encrypted
         */
        CONN_F_ENCRYPTED(connsm) = 0;
        connsm->enc_data.enc_state = CONN_ENC_S_START_ENC_RSP_WAIT;
        connsm->enc_data.tx_encrypted = 0;
        ble_phy_encrypt_disable();
        if (txend_func == NULL) {
            txend_func = ble_ll_conn_start_rx_encrypt;
        } else {
            txend_func = ble_ll_conn_txend_encrypt;
        }
    } else if (is_ctrl && (opcode == BLE_LL_CTRL_PAUSE_ENC_RSP)) {
        /*
         * The slave sends the PAUSE_ENC_RSP encrypted. The master sends
         * it unencrypted (note that link was already set unencrypted).
         */
        if (connsm->conn_role == BLE_LL_CONN_ROLE_SLAVE) {
            CONN_F_ENCRYPTED(connsm) = 1;
            connsm->enc_data.tx_encrypted = 1;
            ble_phy_encrypt_enable(connsm->enc_data.tx_pkt_cntr,
                                   connsm->enc_data.iv,
                                   connsm->enc_data.enc_block.cipher_text,
                                   CONN_IS_MASTER(connsm));
            if (txend_func == NULL) {
                txend_func = ble_ll_conn_start_rx_unencrypt;
            } else {
                txend_func = ble_ll_conn_rxend_unencrypt;
            }
        } else {
            CONN_F_ENCRYPTED(connsm) = 0;
            connsm->enc_data.enc_state = CONN_ENC_S_PAUSED;
            connsm->enc_data.tx_encrypted = 0;
            ble_phy_encrypt_disable();
        }
    } else {
        /* If encrypted set packet counter */
        if (CONN_F_ENCRYPTED(connsm)) {
            connsm->enc_data.tx_encrypted = 1;
            ble_phy_encrypt_set_pkt_cntr(connsm->enc_data.tx_pkt_cntr,
                                         CONN_IS_MASTER(connsm));
            if (txend_func == NULL) {
                txend_func = ble_ll_conn_continue_rx_encrypt;
            }
        }
    }
#endif

    /* Set transmit end callback */
    ble_phy_set_txend_cb(txend_func, connsm);
    rc = ble_phy_tx(ble_ll_tx_mbuf_pducb, m, end_transition);
    if (!rc) {
        /* Log transmit on connection state */
        cur_txlen = ble_hdr->txinfo.pyld_len;
        ble_ll_trace_u32x2(BLE_LL_TRACE_ID_CONN_TX, cur_txlen,
                           ble_hdr->txinfo.offset);

        /* Set last transmitted MD bit */
        CONN_F_LAST_TXD_MD(connsm) = md;

        /* Increment packets transmitted */
        if (CONN_F_EMPTY_PDU_TXD(connsm)) {
            if (connsm->csmflags.cfbit.terminate_ind_rxd) {
                connsm->csmflags.cfbit.terminate_ind_rxd_acked = 1;
            }
            STATS_INC(ble_ll_conn_stats, tx_empty_pdus);
        } else if ((hdr_byte & BLE_LL_DATA_HDR_LLID_MASK) == BLE_LL_LLID_CTRL) {
            STATS_INC(ble_ll_conn_stats, tx_ctrl_pdus);
            STATS_INCN(ble_ll_conn_stats, tx_ctrl_bytes, cur_txlen);
        } else {
            STATS_INC(ble_ll_conn_stats, tx_l2cap_pdus);
            STATS_INCN(ble_ll_conn_stats, tx_l2cap_bytes, cur_txlen);
        }
    }
    return rc;
}

/**
 * Schedule callback for start of connection event.
 *
 * Context: Interrupt
 *
 * @param sch
 *
 * @return int 0: scheduled item is still running. 1: schedule item is done.
 */
static int
ble_ll_conn_event_start_cb(struct ble_ll_sched_item *sch)
{
    int rc;
    uint32_t usecs;
    uint32_t start;
    struct ble_ll_conn_sm *connsm;

    /* XXX: note that we can extend end time here if we want. Look at this */

    /* Set current connection state machine */
    connsm = (struct ble_ll_conn_sm *)sch->cb_arg;
    g_ble_ll_conn_cur_sm = connsm;
    BLE_LL_ASSERT(connsm);
    if (connsm->conn_state == BLE_LL_CONN_STATE_IDLE) {
        /* That should not happen. If it does it means connection
         * is already closed
         */
        STATS_INC(ble_ll_conn_stats, sched_start_in_idle);
        BLE_LL_ASSERT(0);
        ble_ll_conn_current_sm_over(connsm);
        return BLE_LL_SCHED_STATE_DONE;
    }

    /* Log connection event start */
    ble_ll_trace_u32(BLE_LL_TRACE_ID_CONN_EV_START, connsm->conn_handle);

    /* Disable whitelisting as connections do not use it */
    ble_ll_whitelist_disable();

    /* Set LL state */
    ble_ll_state_set(BLE_LL_STATE_CONNECTION);

    /* Set channel */
    ble_phy_setchan(connsm->data_chan_index, connsm->access_addr,
                    connsm->crcinit);

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
    ble_phy_resolv_list_disable();
#endif

#if (BLE_LL_BT5_PHY_SUPPORTED == 1)
    ble_phy_mode_set(connsm->phy_data.tx_phy_mode, connsm->phy_data.rx_phy_mode);
#endif

    if (connsm->conn_role == BLE_LL_CONN_ROLE_MASTER) {
        /* Set start time of transmission */
        start = sch->start_time + g_ble_ll_sched_offset_ticks;
        rc = ble_phy_tx_set_start_time(start, sch->remainder);
        if (!rc) {
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
            if (CONN_F_ENCRYPTED(connsm)) {
                ble_phy_encrypt_enable(connsm->enc_data.tx_pkt_cntr,
                                       connsm->enc_data.iv,
                                       connsm->enc_data.enc_block.cipher_text,
                                       1);
            } else {
                ble_phy_encrypt_disable();
            }
#endif
            rc = ble_ll_conn_tx_pdu(connsm);
            if (!rc) {
                rc = BLE_LL_SCHED_STATE_RUNNING;
            } else {
                /* Inform LL task of connection event end */
                rc = BLE_LL_SCHED_STATE_DONE;
            }
        } else {
            STATS_INC(ble_ll_conn_stats, conn_ev_late);
            rc = BLE_LL_SCHED_STATE_DONE;
        }
    } else {
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
        if (CONN_F_ENCRYPTED(connsm)) {
            ble_phy_encrypt_enable(connsm->enc_data.rx_pkt_cntr,
                                   connsm->enc_data.iv,
                                   connsm->enc_data.enc_block.cipher_text,
                                   1);
        } else {
            ble_phy_encrypt_disable();
        }
#endif

        /* XXX: what is this really for the slave? */
        start = sch->start_time + g_ble_ll_sched_offset_ticks;
        rc = ble_phy_rx_set_start_time(start, sch->remainder);
        if (rc) {
            /* End the connection event as we have no more buffers */
            STATS_INC(ble_ll_conn_stats, slave_ce_failures);
            rc = BLE_LL_SCHED_STATE_DONE;
        } else {
            /*
             * Set flag that tells slave to set last anchor point if a packet
             * has been received.
             */
            connsm->csmflags.cfbit.slave_set_last_anchor = 1;

            /*
             * Set the wait for response time. The anchor point is when we
             * expect the master to start transmitting. Worst-case, we expect
             * to hear a reply within the anchor point plus:
             *  -> current tx window size
             *  -> current window widening amount (includes +/- 16 usec jitter)
             *  -> Amount of time it takes to detect packet start.
             *  -> Some extra time (16 usec) to insure timing is OK
             */

            /*
             * For the 32 kHz crystal, the amount of usecs we have to wait
             * is not from the anchor point; we have to account for the time
             * from when the receiver is enabled until the anchor point. The
             * time we start before the anchor point is this:
             *   -> current window widening.
             *   -> up to one 32 kHz tick since we discard remainder.
             *   -> Up to one tick since the usecs to ticks calc can be off
             *   by up to one tick.
             * NOTES:
             * 1) the 61 we add is for the two ticks mentioned above.
             * 2) The address rx time and jitter is accounted for in the
             * phy function
             */
            usecs = connsm->slave_cur_tx_win_usecs + 61 +
                (2 * connsm->slave_cur_window_widening);
            ble_phy_wfr_enable(BLE_PHY_WFR_ENABLE_RX, 0, usecs);
            /* Set next wakeup time to connection event end time */
            rc = BLE_LL_SCHED_STATE_RUNNING;
        }
    }

    if (rc == BLE_LL_SCHED_STATE_DONE) {
        ble_ll_event_send(&connsm->conn_ev_end);
        ble_phy_disable();
        ble_ll_state_set(BLE_LL_STATE_STANDBY);
        g_ble_ll_conn_cur_sm = NULL;
    }

    /* Set time that we last serviced the schedule */
    connsm->last_scheduled = os_cputime_get32();
    return rc;
}

/**
 * Called to determine if the device is allowed to send the next pdu in the
 * connection event. This will always return 'true' if we are a slave. If we
 * are a master, we must be able to send the next fragment and get a minimum
 * sized response from the slave.
 *
 * Context: Interrupt context (rx end isr).
 *
 * @param connsm
 * @param begtime   Time at which IFS before pdu transmission starts
 *
 * @return int 0: not allowed to send 1: allowed to send
 */
static int
ble_ll_conn_can_send_next_pdu(struct ble_ll_conn_sm *connsm, uint32_t begtime,
                              uint32_t add_usecs)
{
    int rc;
    uint8_t rem_bytes;
    uint32_t ticks;
    uint32_t usecs;
    uint32_t next_sched_time;
    struct os_mbuf *txpdu;
    struct os_mbuf_pkthdr *pkthdr;
    struct ble_mbuf_hdr *txhdr;
    uint32_t allowed_usecs;
    int tx_phy_mode;

#if BLE_LL_BT5_PHY_SUPPORTED
    tx_phy_mode = connsm->phy_data.tx_phy_mode;
#else
    tx_phy_mode = BLE_PHY_MODE_1M;
#endif

    rc = 1;
    if (connsm->conn_role == BLE_LL_CONN_ROLE_MASTER) {
        /* Get next scheduled item time */
        next_sched_time = ble_ll_conn_get_next_sched_time(connsm);

        txpdu = connsm->cur_tx_pdu;
        if (!txpdu) {
            pkthdr = STAILQ_FIRST(&connsm->conn_txq);
            if (pkthdr) {
                txpdu = OS_MBUF_PKTHDR_TO_MBUF(pkthdr);
            }
        } else {
            pkthdr = OS_MBUF_PKTHDR(txpdu);
        }

        /* XXX: TODO: need to check this with phy update procedure. There are
           limitations if we have started update */
        if (txpdu) {
            txhdr = BLE_MBUF_HDR_PTR(txpdu);
            rem_bytes = pkthdr->omp_len - txhdr->txinfo.offset;
            if (rem_bytes > connsm->eff_max_tx_octets) {
                rem_bytes = connsm->eff_max_tx_octets;
            }
            usecs = ble_ll_pdu_tx_time_get(rem_bytes, tx_phy_mode);
        } else {
            /* We will send empty pdu (just a LL header) */
            usecs = ble_ll_pdu_tx_time_get(0, tx_phy_mode);
        }
        usecs += (BLE_LL_IFS * 2) + connsm->eff_max_rx_time;

        ticks = (uint32_t)(next_sched_time - begtime);
        allowed_usecs = os_cputime_ticks_to_usecs(ticks);
        if ((usecs + add_usecs) >= allowed_usecs) {
            rc = 0;
        }
    }

    return rc;
}

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_PING)
/**
 * Callback for the Authenticated payload timer. This function is called
 * when the authenticated payload timer expires. When the authenticated
 * payload timeout expires, we should
 *  -> Send the authenticated payload timeout event.
 *  -> Start the LE ping procedure.
 *  -> Restart the timer.
 *
 * @param arg
 */
void
ble_ll_conn_auth_pyld_timer_cb(struct ble_npl_event *ev)
{
    struct ble_ll_conn_sm *connsm;

    connsm = (struct ble_ll_conn_sm *)ble_npl_event_get_arg(ev);
    ble_ll_auth_pyld_tmo_event_send(connsm);
    ble_ll_ctrl_proc_start(connsm, BLE_LL_CTRL_PROC_LE_PING);
    ble_ll_conn_auth_pyld_timer_start(connsm);
}

void
ble_ll_conn_rd_features_timer_cb(struct ble_npl_event *ev)
{
    struct ble_ll_conn_sm *connsm;

    connsm = (struct ble_ll_conn_sm *)ble_npl_event_get_arg(ev);

    if (!connsm->csmflags.cfbit.pending_hci_rd_features ||
                                        !connsm->csmflags.cfbit.rxd_features) {
        return;
    }

    ble_ll_hci_ev_rd_rem_used_feat(connsm, BLE_ERR_SUCCESS);
    connsm->csmflags.cfbit.pending_hci_rd_features = 0;
}

/**
 * Start (or restart) the authenticated payload timer
 *
 * @param connsm
 */
void
ble_ll_conn_auth_pyld_timer_start(struct ble_ll_conn_sm *connsm)
{
    int32_t tmo;

    /* Timeout in is in 10 msec units */
    tmo = (int32_t)BLE_LL_CONN_AUTH_PYLD_OS_TMO(connsm->auth_pyld_tmo);
    ble_npl_callout_reset(&connsm->auth_pyld_timer, tmo);
}
#endif

static void
ble_ll_conn_master_common_init(struct ble_ll_conn_sm *connsm)
{

    /* Set master role */
    connsm->conn_role = BLE_LL_CONN_ROLE_MASTER;

    /* Set default ce parameters */

    /*
     * XXX: for now, we need twice the transmit window as our calculations
     * for the transmit window offset could be off.
     */
    connsm->tx_win_size = BLE_LL_CONN_TX_WIN_MIN + 1;
    connsm->tx_win_off = 0;
    connsm->master_sca = MYNEWT_VAL(BLE_LL_MASTER_SCA);

    /* Hop increment is a random value between 5 and 16. */
    connsm->hop_inc = (rand() % 12) + 5;

    /* Set channel map to map requested by host */
    connsm->num_used_chans = g_ble_ll_conn_params.num_used_chans;
    memcpy(connsm->chanmap, g_ble_ll_conn_params.master_chan_map,
           BLE_LL_CONN_CHMAP_LEN);

    /*  Calculate random access address and crc initialization value */
    connsm->access_addr = ble_ll_utils_calc_access_addr();
    connsm->crcinit = rand() & 0xffffff;

    /* Set initial schedule callback */
    connsm->conn_sch.sched_cb = ble_ll_conn_event_start_cb;
}
/**
 * Called when a create connection command has been received. This initializes
 * a connection state machine in the master role.
 *
 * NOTE: Must be called before the state machine is started
 *
 * @param connsm
 * @param hcc
 */
void
ble_ll_conn_master_init(struct ble_ll_conn_sm *connsm,
                        struct hci_create_conn *hcc)
{

    ble_ll_conn_master_common_init(connsm);

    /* Set slave latency and supervision timeout */
    connsm->slave_latency = hcc->conn_latency;
    connsm->supervision_tmo = hcc->supervision_timeout;

    /* Set own address type and peer address if needed */
    connsm->own_addr_type = hcc->own_addr_type;
    if (hcc->filter_policy == 0) {
        memcpy(&connsm->peer_addr, &hcc->peer_addr, BLE_DEV_ADDR_LEN);
        connsm->peer_addr_type = hcc->peer_addr_type;
    }

    /* XXX: for now, just make connection interval equal to max */
    connsm->conn_itvl = hcc->conn_itvl_max;

    /* Check the min/max CE lengths are less than connection interval */
    if (hcc->min_ce_len > (connsm->conn_itvl * 2)) {
        connsm->min_ce_len = connsm->conn_itvl * 2;
    } else {
        connsm->min_ce_len = hcc->min_ce_len;
    }

    if (hcc->max_ce_len > (connsm->conn_itvl * 2)) {
        connsm->max_ce_len = connsm->conn_itvl * 2;
    } else {
        connsm->max_ce_len = hcc->max_ce_len;
    }
}

static void
ble_ll_update_max_tx_octets_phy_mode(struct ble_ll_conn_sm *connsm)
{
    uint32_t usecs;

    usecs = connsm->eff_max_tx_time;

    connsm->max_tx_octets_phy_mode[BLE_PHY_MODE_1M] =
            ble_ll_pdu_max_tx_octets_get(usecs, BLE_PHY_MODE_1M);
    connsm->max_tx_octets_phy_mode[BLE_PHY_MODE_2M] =
            ble_ll_pdu_max_tx_octets_get(usecs, BLE_PHY_MODE_2M);
    connsm->max_tx_octets_phy_mode[BLE_PHY_MODE_CODED_125KBPS] =
            ble_ll_pdu_max_tx_octets_get(usecs, BLE_PHY_MODE_CODED_125KBPS);
    connsm->max_tx_octets_phy_mode[BLE_PHY_MODE_CODED_500KBPS] =
            ble_ll_pdu_max_tx_octets_get(usecs, BLE_PHY_MODE_CODED_500KBPS);
}

#if (BLE_LL_BT5_PHY_SUPPORTED == 1)

static void
ble_ll_conn_set_phy(struct ble_ll_conn_sm *connsm, int tx_phy, int rx_phy)
{

    struct ble_ll_conn_phy_data *phy_data = &connsm->phy_data;

    phy_data->rx_phy_mode = ble_ll_phy_to_phy_mode(rx_phy,
                                                   BLE_HCI_LE_PHY_CODED_ANY);
    phy_data->cur_rx_phy = rx_phy;

    phy_data->tx_phy_mode = ble_ll_phy_to_phy_mode(tx_phy,
                                                   BLE_HCI_LE_PHY_CODED_ANY);
    phy_data->cur_tx_phy = tx_phy;

}

static void
ble_ll_conn_init_phy(struct ble_ll_conn_sm *connsm, int phy)
{
    struct ble_ll_conn_global_params *conngp;

    /* Always initialize symmetric PHY - controller can change this later */
    ble_ll_conn_set_phy(connsm, phy, phy);

    /* Update data length management to match initial PHY */
    conngp = &g_ble_ll_conn_params;
    connsm->max_tx_octets = conngp->conn_init_max_tx_octets;
    connsm->max_rx_octets = conngp->supp_max_rx_octets;
    if (phy == BLE_PHY_CODED) {
        connsm->max_tx_time = conngp->conn_init_max_tx_time_coded;
        connsm->max_rx_time = BLE_LL_CONN_SUPP_TIME_MAX_CODED;
        connsm->rem_max_tx_time = BLE_LL_CONN_SUPP_TIME_MIN_CODED;
        connsm->rem_max_rx_time = BLE_LL_CONN_SUPP_TIME_MIN_CODED;
        /* Assume peer does support coded */
        connsm->remote_features[0] |= (BLE_LL_FEAT_LE_CODED_PHY >> 8);
    } else {
        connsm->max_tx_time = conngp->conn_init_max_tx_time_uncoded;
        connsm->max_rx_time = BLE_LL_CONN_SUPP_TIME_MAX_UNCODED;
        connsm->rem_max_tx_time = BLE_LL_CONN_SUPP_TIME_MIN_UNCODED;
        connsm->rem_max_rx_time = BLE_LL_CONN_SUPP_TIME_MIN_UNCODED;
    }
    connsm->eff_max_tx_time = connsm->rem_max_tx_time;
    connsm->eff_max_rx_time = connsm->rem_max_rx_time;
    connsm->rem_max_tx_octets = BLE_LL_CONN_SUPP_BYTES_MIN;
    connsm->rem_max_rx_octets = BLE_LL_CONN_SUPP_BYTES_MIN;
    connsm->eff_max_tx_octets = BLE_LL_CONN_SUPP_BYTES_MIN;
    connsm->eff_max_rx_octets = BLE_LL_CONN_SUPP_BYTES_MIN;

    ble_ll_update_max_tx_octets_phy_mode(connsm);
}

#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)

void
ble_ll_conn_ext_master_init(struct ble_ll_conn_sm *connsm,
                        struct hci_ext_create_conn *hcc)
{

    ble_ll_conn_master_common_init(connsm);

    /* Set own address type and peer address if needed */
    connsm->own_addr_type = hcc->own_addr_type;
    if (hcc->filter_policy == 0) {
        memcpy(&connsm->peer_addr, &hcc->peer_addr, BLE_DEV_ADDR_LEN);
        connsm->peer_addr_type = hcc->peer_addr_type;
    }

    connsm->initial_params = *hcc;
}

void
ble_ll_conn_ext_set_params(struct ble_ll_conn_sm *connsm,
                           struct hci_ext_conn_params *hcc_params, int phy)
{
    /* Set slave latency and supervision timeout */
    connsm->slave_latency = hcc_params->conn_latency;
    connsm->supervision_tmo = hcc_params->supervision_timeout;

    /* XXX: for now, just make connection interval equal to max */
    connsm->conn_itvl = hcc_params->conn_itvl_max;


    /* Check the min/max CE lengths are less than connection interval */
    if (hcc_params->min_ce_len > (connsm->conn_itvl * 2)) {
        connsm->min_ce_len = connsm->conn_itvl * 2;
    } else {
        connsm->min_ce_len = hcc_params->min_ce_len;
    }

    if (hcc_params->max_ce_len > (connsm->conn_itvl * 2)) {
        connsm->max_ce_len = connsm->conn_itvl * 2;
    } else {
        connsm->max_ce_len = hcc_params->max_ce_len;
    }

    ble_ll_conn_calc_itvl_ticks(connsm);

#if (BLE_LL_BT5_PHY_SUPPORTED == 1)
    ble_ll_conn_init_phy(connsm, phy);
#endif
}


#endif

static void
ble_ll_conn_set_csa(struct ble_ll_conn_sm *connsm, bool chsel)
{
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CSA2)
    if (chsel) {
        CONN_F_CSA2_SUPP(connsm) = 1;
        connsm->channel_id = ((connsm->access_addr & 0xffff0000) >> 16) ^
                              (connsm->access_addr & 0x0000ffff);

        /* calculate the next data channel */
        connsm->data_chan_index = ble_ll_conn_calc_dci(connsm, 0);
        return;
    }
#endif

    connsm->last_unmapped_chan = 0;

    /* calculate the next data channel */
    connsm->data_chan_index = ble_ll_conn_calc_dci(connsm, 1);
}

/**
 * Create a new connection state machine. This is done once per
 * connection when the HCI command "create connection" is issued to the
 * controller or when a slave receives a connect request.
 *
 * Context: Link Layer task
 *
 * @param connsm
 */
void
ble_ll_conn_sm_new(struct ble_ll_conn_sm *connsm)
{
    struct ble_ll_conn_global_params *conn_params;

    /* Reset following elements */
    connsm->csmflags.conn_flags = 0;
    connsm->event_cntr = 0;
    connsm->conn_state = BLE_LL_CONN_STATE_IDLE;
    connsm->disconnect_reason = 0;
    connsm->rxd_disconnect_reason = 0;
    connsm->conn_features = BLE_LL_CONN_INITIAL_FEATURES;
    memset(connsm->remote_features, 0, sizeof(connsm->remote_features));
    connsm->vers_nr = 0;
    connsm->comp_id = 0;
    connsm->sub_vers_nr = 0;
    connsm->reject_reason = BLE_ERR_SUCCESS;
    connsm->conn_rssi = BLE_LL_CONN_UNKNOWN_RSSI;
    connsm->rpa_index = -1;
    connsm->inita_identity_used = 0;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV_SYNC_TRANSFER)
    connsm->sync_transfer_sync_timeout = g_ble_ll_conn_sync_transfer_params.sync_timeout_us;
    connsm->sync_transfer_mode = g_ble_ll_conn_sync_transfer_params.mode;
    connsm->sync_transfer_skip = g_ble_ll_conn_sync_transfer_params.max_skip;
#endif

    /* XXX: TODO set these based on PHY that started connection */
#if (BLE_LL_BT5_PHY_SUPPORTED == 1)
    connsm->phy_data.cur_tx_phy = BLE_PHY_1M;
    connsm->phy_data.cur_rx_phy = BLE_PHY_1M;
    connsm->phy_data.tx_phy_mode = BLE_PHY_MODE_1M;
    connsm->phy_data.rx_phy_mode = BLE_PHY_MODE_1M;
    connsm->phy_data.req_pref_tx_phys_mask = 0;
    connsm->phy_data.req_pref_rx_phys_mask = 0;
    connsm->phy_data.host_pref_tx_phys_mask = g_ble_ll_data.ll_pref_tx_phys;
    connsm->phy_data.host_pref_rx_phys_mask = g_ble_ll_data.ll_pref_rx_phys;
    connsm->phy_data.phy_options = 0;
    connsm->phy_tx_transition = 0;
#endif

    /* Reset current control procedure */
    connsm->cur_ctrl_proc = BLE_LL_CTRL_PROC_IDLE;
    connsm->pending_ctrl_procs = 0;

    /*
     * Set handle in connection update procedure to 0. If the handle
     * is non-zero it means that the host initiated the connection
     * parameter update request and the rest of the parameters are valid.
     */
    connsm->conn_param_req.handle = 0;

    /* Connection end event */
    ble_npl_event_init(&connsm->conn_ev_end, ble_ll_conn_event_end, connsm);

    /* Initialize transmit queue and ack/flow control elements */
    STAILQ_INIT(&connsm->conn_txq);
    connsm->cur_tx_pdu = NULL;
    connsm->tx_seqnum = 0;
    connsm->next_exp_seqnum = 0;
    connsm->cons_rxd_bad_crc = 0;
    connsm->last_rxd_sn = 1;
    connsm->completed_pkts = 0;

    /* initialize data length mgmt */
    conn_params = &g_ble_ll_conn_params;
    connsm->max_tx_octets = conn_params->conn_init_max_tx_octets;
    connsm->max_rx_octets = conn_params->supp_max_rx_octets;
    connsm->max_tx_time = conn_params->conn_init_max_tx_time;
    connsm->max_rx_time = conn_params->supp_max_rx_time;
    connsm->rem_max_tx_time = BLE_LL_CONN_SUPP_TIME_MIN;
    connsm->rem_max_rx_time = BLE_LL_CONN_SUPP_TIME_MIN;
    connsm->eff_max_tx_time = BLE_LL_CONN_SUPP_TIME_MIN;
    connsm->eff_max_rx_time = BLE_LL_CONN_SUPP_TIME_MIN;
    connsm->rem_max_tx_octets = BLE_LL_CONN_SUPP_BYTES_MIN;
    connsm->rem_max_rx_octets = BLE_LL_CONN_SUPP_BYTES_MIN;
    connsm->eff_max_tx_octets = BLE_LL_CONN_SUPP_BYTES_MIN;
    connsm->eff_max_rx_octets = BLE_LL_CONN_SUPP_BYTES_MIN;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CODED_PHY)
    connsm->host_req_max_tx_time = 0;
#endif

    ble_ll_update_max_tx_octets_phy_mode(connsm);

    /* Reset encryption data */
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
    memset(&connsm->enc_data, 0, sizeof(struct ble_ll_conn_enc_data));
    connsm->enc_data.enc_state = CONN_ENC_S_UNENCRYPTED;
#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_PING)
    connsm->auth_pyld_tmo = BLE_LL_CONN_DEF_AUTH_PYLD_TMO;
    CONN_F_LE_PING_SUPP(connsm) = 1;
    ble_npl_callout_init(&connsm->auth_pyld_timer,
                    &g_ble_ll_data.ll_evq,
                    ble_ll_conn_auth_pyld_timer_cb,
                    connsm);
#endif

    ble_ll_conn_calc_itvl_ticks(connsm);

    /* Add to list of active connections */
    SLIST_INSERT_HEAD(&g_ble_ll_conn_active_list, connsm, act_sle);
}

void
ble_ll_conn_update_eff_data_len(struct ble_ll_conn_sm *connsm)
{
    int send_event;
    uint16_t eff_time;
    uint16_t eff_bytes;

    /* Assume no event sent */
    send_event = 0;

    /* See if effective times have changed */
    eff_time = min(connsm->rem_max_tx_time, connsm->max_rx_time);
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CODED_PHY)
    if (connsm->phy_data.cur_rx_phy == BLE_PHY_CODED) {
        eff_time = max(eff_time, BLE_LL_CONN_SUPP_TIME_MIN_CODED);
    }
#endif
    if (eff_time != connsm->eff_max_rx_time) {
        connsm->eff_max_rx_time = eff_time;
        send_event = 1;
    }
    eff_time = min(connsm->rem_max_rx_time, connsm->max_tx_time);
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CODED_PHY)
    if (connsm->phy_data.cur_tx_phy == BLE_PHY_CODED) {
        eff_time = max(eff_time, BLE_LL_CONN_SUPP_TIME_MIN_CODED);
    }
#endif
    if (eff_time != connsm->eff_max_tx_time) {
        connsm->eff_max_tx_time = eff_time;
        send_event = 1;

        ble_ll_update_max_tx_octets_phy_mode(connsm);
    }
    eff_bytes = min(connsm->rem_max_tx_octets, connsm->max_rx_octets);
    if (eff_bytes != connsm->eff_max_rx_octets) {
        connsm->eff_max_rx_octets = eff_bytes;
        send_event = 1;
    }
    eff_bytes = min(connsm->rem_max_rx_octets, connsm->max_tx_octets);
    if (eff_bytes != connsm->eff_max_tx_octets) {
        connsm->eff_max_tx_octets = eff_bytes;
        send_event = 1;
    }

    if (send_event) {
        ble_ll_hci_ev_datalen_chg(connsm);
    }
}

/**
 * Called when a connection is terminated
 *
 * Context: Link Layer task.
 *
 * @param connsm
 * @param ble_err
 */
void
ble_ll_conn_end(struct ble_ll_conn_sm *connsm, uint8_t ble_err)
{
    struct os_mbuf *m;
    struct os_mbuf_pkthdr *pkthdr;
    os_sr_t sr;

    /* Remove scheduler events just in case */
    ble_ll_sched_rmv_elem(&connsm->conn_sch);

    /* In case of the supervision timeout we shall make sure
     * that there is no ongoing connection event. It could happen
     * because we scheduled connection event before checking connection timeout.
     * If connection event managed to start, let us drop it.
     */
    OS_ENTER_CRITICAL(sr);
    if (g_ble_ll_conn_cur_sm == connsm) {
        ble_ll_conn_halt();
        STATS_INC(ble_ll_conn_stats, conn_event_while_tmo);
    }
    OS_EXIT_CRITICAL(sr);

    /* Stop any control procedures that might be running */
    ble_npl_callout_stop(&connsm->ctrl_proc_rsp_timer);

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_PING)
    ble_npl_callout_stop(&connsm->auth_pyld_timer);
#endif

    /* Remove from the active connection list */
    SLIST_REMOVE(&g_ble_ll_conn_active_list, connsm, ble_ll_conn_sm, act_sle);

    /* Free the current transmit pdu if there is one. */
    if (connsm->cur_tx_pdu) {
        os_mbuf_free_chain(connsm->cur_tx_pdu);
        connsm->cur_tx_pdu = NULL;
    }

    /* Free all packets on transmit queue */
    while (1) {
        /* Get mbuf pointer from packet header pointer */
        pkthdr = STAILQ_FIRST(&connsm->conn_txq);
        if (!pkthdr) {
            break;
        }
        STAILQ_REMOVE_HEAD(&connsm->conn_txq, omp_next);

        m = (struct os_mbuf *)((uint8_t *)pkthdr - sizeof(struct os_mbuf));
        os_mbuf_free_chain(m);
    }

    /* Make sure events off queue */
    ble_npl_eventq_remove(&g_ble_ll_data.ll_evq, &connsm->conn_ev_end);

#if MYNEWT_VAL(BLE_LL_STRICT_CONN_SCHEDULING)
    /* Remove from occupied periods */
    OS_ENTER_CRITICAL(sr);
    BLE_LL_ASSERT(g_ble_ll_sched_data.sch_num_occ_periods > 0);
    BLE_LL_ASSERT(g_ble_ll_sched_data.sch_occ_period_mask & connsm->period_occ_mask);
    --g_ble_ll_sched_data.sch_num_occ_periods;
    g_ble_ll_sched_data.sch_occ_period_mask &= ~connsm->period_occ_mask;
    OS_EXIT_CRITICAL(sr);
#endif

    /* Connection state machine is now idle */
    connsm->conn_state = BLE_LL_CONN_STATE_IDLE;

    /*
     * If we have features and there's pending HCI command, send an event before
     * disconnection event so it does make sense to host.
     */
    if (connsm->csmflags.cfbit.pending_hci_rd_features &&
                                        connsm->csmflags.cfbit.rxd_features) {
        ble_ll_hci_ev_rd_rem_used_feat(connsm, BLE_ERR_SUCCESS);
        connsm->csmflags.cfbit.pending_hci_rd_features = 0;
    }

    /*
     * If there is still pending read features request HCI command, send an
     * event to complete it.
     */
    if (connsm->csmflags.cfbit.pending_hci_rd_features) {
        ble_ll_hci_ev_rd_rem_used_feat(connsm, ble_err);
        connsm->csmflags.cfbit.pending_hci_rd_features = 0;
    }

    /*
     * We need to send a disconnection complete event. Connection Complete for
     * canceling connection creation is sent from LE Create Connection Cancel
     * Command handler.
     *
     * If the ble error is "success" it means that the reset command was
     * received and we should not send an event.
     */
    if (ble_err && (ble_err != BLE_ERR_UNK_CONN_ID ||
                                connsm->csmflags.cfbit.terminate_ind_rxd)) {
        ble_ll_disconn_comp_event_send(connsm, ble_err);
    }

    /* Put connection state machine back on free list */
    STAILQ_INSERT_TAIL(&g_ble_ll_conn_free_list, connsm, free_stqe);

    /* Log connection end */
    ble_ll_trace_u32x3(BLE_LL_TRACE_ID_CONN_END, connsm->conn_handle,
                       connsm->event_cntr, (uint32_t)ble_err);
}

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV_SYNC_TRANSFER)
void
ble_ll_conn_get_anchor(struct ble_ll_conn_sm *connsm, uint16_t conn_event,
                       uint32_t *anchor, uint8_t *anchor_usecs)
{
    uint32_t ticks;
    uint32_t itvl;

    itvl = (connsm->conn_itvl * BLE_LL_CONN_ITVL_USECS);

    if ((int16_t)(conn_event - connsm->event_cntr) < 0) {
        itvl *= connsm->event_cntr - conn_event;
        ticks = os_cputime_usecs_to_ticks(itvl);
        *anchor = connsm->anchor_point - ticks;
    } else {
        itvl *= conn_event - connsm->event_cntr;
        ticks = os_cputime_usecs_to_ticks(itvl);
        *anchor = connsm->anchor_point + ticks;
    }

    *anchor_usecs = connsm->anchor_point_usecs;
    *anchor_usecs += (itvl - os_cputime_ticks_to_usecs(ticks));
    if (*anchor_usecs >= 31) {
        (*anchor)++;
        *anchor_usecs -= 31;
    }
}
#endif

/**
 * Called to move to the next connection event.
 *
 * Context: Link Layer task.
 *
 * @param connsm
 *
 * @return int
 */
static int
ble_ll_conn_next_event(struct ble_ll_conn_sm *connsm)
{
    uint16_t latency;
    uint32_t itvl;
    uint32_t cur_ww;
    uint32_t max_ww;
    struct ble_ll_conn_upd_req *upd;
    uint32_t ticks;
    uint32_t usecs;

    /* XXX: deal with connection request procedure here as well */
    ble_ll_conn_chk_csm_flags(connsm);

    /* If unable to start terminate procedure, start it now */
    if (connsm->disconnect_reason && !CONN_F_TERMINATE_STARTED(connsm)) {
        ble_ll_ctrl_terminate_start(connsm);
    }

    if (CONN_F_TERMINATE_STARTED(connsm) && (connsm->conn_role == BLE_LL_CONN_ROLE_SLAVE)) {
        /* Some of the devices waits whole connection interval to ACK our
         * TERMINATE_IND sent as a Slave. Since we are here it means we are still waiting for ACK.
         * Make sure we catch it in next connection event.
         */
        connsm->slave_latency = 0;
    }

    /*
     * XXX: TODO Probably want to add checks to see if we need to start
     * a control procedure here as an instant may have prevented us from
     * starting one.
     */

    /*
     * XXX TODO: I think this is technically incorrect. We can allow slave
     * latency if we are doing one of these updates as long as we
     * know that the master has received the ACK to the PDU that set
     * the instant
     */
    /* Set event counter to the next connection event that we will tx/rx in */
    itvl = connsm->conn_itvl * BLE_LL_CONN_ITVL_USECS;
    latency = 1;
    if (connsm->csmflags.cfbit.allow_slave_latency      &&
        !connsm->csmflags.cfbit.conn_update_sched       &&
        !CONN_F_PHY_UPDATE_SCHED(connsm)                &&
        !connsm->csmflags.cfbit.chanmap_update_scheduled) {
        if (connsm->csmflags.cfbit.pkt_rxd) {
            latency += connsm->slave_latency;
            itvl = itvl * latency;
        }
    }
    connsm->event_cntr += latency;

    /* Set next connection event start time */
    /* We can use pre-calculated values for one interval if latency is 1. */
    if (latency == 1) {
        connsm->anchor_point += connsm->conn_itvl_ticks;
        connsm->anchor_point_usecs += connsm->conn_itvl_usecs;
    } else {
        uint32_t ticks;
        ticks = os_cputime_usecs_to_ticks(itvl);
        connsm->anchor_point += ticks;
        connsm->anchor_point_usecs += (itvl - os_cputime_ticks_to_usecs(ticks));
    }
    if (connsm->anchor_point_usecs >= 31) {
        ++connsm->anchor_point;
        connsm->anchor_point_usecs -= 31;
    }

    /*
     * If a connection update has been scheduled and the event counter
     * is now equal to the instant, we need to adjust the start of the
     * connection by the the transmit window offset. We also copy in the
     * update parameters as they now should take effect.
     */
    if (connsm->csmflags.cfbit.conn_update_sched &&
        (connsm->event_cntr == connsm->conn_update_req.instant)) {

        /* Set flag so we send connection update event */
        upd = &connsm->conn_update_req;
        if ((connsm->conn_role == BLE_LL_CONN_ROLE_MASTER)  ||
            ((connsm->conn_role == BLE_LL_CONN_ROLE_SLAVE) &&
             IS_PENDING_CTRL_PROC(connsm, BLE_LL_CTRL_PROC_CONN_PARAM_REQ)) ||
            (connsm->conn_itvl != upd->interval)            ||
            (connsm->slave_latency != upd->latency)         ||
            (connsm->supervision_tmo != upd->timeout)) {
            connsm->csmflags.cfbit.host_expects_upd_event = 1;
        }

        connsm->supervision_tmo = upd->timeout;
        connsm->slave_latency = upd->latency;
        connsm->tx_win_size = upd->winsize;
        connsm->slave_cur_tx_win_usecs =
            connsm->tx_win_size * BLE_LL_CONN_TX_WIN_USECS;
        connsm->tx_win_off = upd->winoffset;
        connsm->conn_itvl = upd->interval;
        ble_ll_conn_calc_itvl_ticks(connsm);
        if (upd->winoffset != 0) {
            usecs = upd->winoffset * BLE_LL_CONN_ITVL_USECS;
            ticks = os_cputime_usecs_to_ticks(usecs);
            connsm->anchor_point += ticks;
            usecs = usecs - os_cputime_ticks_to_usecs(ticks);
            connsm->anchor_point_usecs += usecs;
            if (connsm->anchor_point_usecs >= 31) {
                ++connsm->anchor_point;
                connsm->anchor_point_usecs -= 31;
            }
        }

        /* Reset the starting point of the connection supervision timeout */
        connsm->last_rxd_pdu_cputime = connsm->anchor_point;

        /* Reset update scheduled flag */
        connsm->csmflags.cfbit.conn_update_sched = 0;
    }

    /*
     * If there is a channel map request pending and we have reached the
     * instant, change to new channel map. Note there is a special case here.
     * If we received a channel map update with an instant equal to the event
     * counter, when we get here the event counter has already been
     * incremented by 1. That is why we do a signed comparison and change to
     * new channel map once the event counter equals or has passed channel
     * map update instant.
     */
    if (connsm->csmflags.cfbit.chanmap_update_scheduled &&
        ((int16_t)(connsm->chanmap_instant - connsm->event_cntr) <= 0)) {

        /* XXX: there is a chance that the control packet is still on
         * the queue of the master. This means that we never successfully
         * transmitted update request. Would end up killing connection
           on slave side. Could ignore it or see if still enqueued. */
        connsm->num_used_chans =
            ble_ll_utils_calc_num_used_chans(connsm->req_chanmap);
        memcpy(connsm->chanmap, connsm->req_chanmap, BLE_LL_CONN_CHMAP_LEN);

        connsm->csmflags.cfbit.chanmap_update_scheduled = 0;

        ble_ll_ctrl_proc_stop(connsm, BLE_LL_CTRL_PROC_CHAN_MAP_UPD);

        /* XXX: host could have resent channel map command. Need to
           check to make sure we dont have to restart! */
    }

#if (BLE_LL_BT5_PHY_SUPPORTED == 1)
    if (CONN_F_PHY_UPDATE_SCHED(connsm) &&
        (connsm->event_cntr == connsm->phy_instant)) {

        /* Set cur phy to new phy */
        if (connsm->phy_data.new_tx_phy) {
            connsm->phy_data.cur_tx_phy = connsm->phy_data.new_tx_phy;
            connsm->phy_data.tx_phy_mode =
                                ble_ll_phy_to_phy_mode(connsm->phy_data.cur_tx_phy,
                                                   connsm->phy_data.phy_options);
        }

        if (connsm->phy_data.new_rx_phy) {
            connsm->phy_data.cur_rx_phy = connsm->phy_data.new_rx_phy;
            connsm->phy_data.rx_phy_mode =
                                ble_ll_phy_to_phy_mode(connsm->phy_data.cur_rx_phy,
                                                   connsm->phy_data.phy_options);
        }

        /* Clear flags and set flag to send event at next instant */
        CONN_F_PHY_UPDATE_SCHED(connsm) = 0;
        CONN_F_PHY_UPDATE_EVENT(connsm) = 1;

        ble_ll_ctrl_phy_update_proc_complete(connsm);

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CODED_PHY)
        /* Recalculate effective connection parameters */
        ble_ll_conn_update_eff_data_len(connsm);

        /*
         * If PHY in either direction was changed to coded, we assume that peer
         * does support LE Coded PHY even if features were not exchanged yet.
         * This means that MaxRxTime can be updated to supported max and we need
         * initiate DLE to notify peer about the change.
         */
        if (((connsm->phy_data.cur_tx_phy == BLE_PHY_CODED) ||
             (connsm->phy_data.cur_rx_phy == BLE_PHY_CODED)) &&
            !(connsm->remote_features[0] & (BLE_LL_FEAT_LE_CODED_PHY >> 8))) {
            connsm->remote_features[0] |= (BLE_LL_FEAT_LE_CODED_PHY >> 8);
            connsm->max_rx_time = BLE_LL_CONN_SUPP_TIME_MAX_CODED;
            ble_ll_ctrl_initiate_dle(connsm);
        }
#endif
    }
#endif

    /* Calculate data channel index of next connection event */
    connsm->data_chan_index = ble_ll_conn_calc_dci(connsm, latency);

    /*
     * If we are trying to terminate connection, check if next wake time is
     * passed the termination timeout. If so, no need to continue with
     * connection as we will time out anyway.
     */
    if (CONN_F_TERMINATE_STARTED(connsm)) {
        if ((int32_t)(connsm->terminate_timeout - connsm->anchor_point) <= 0) {
            return -1;
        }
    }

    /*
     * Calculate ce end time. For a slave, we need to add window widening and
     * the transmit window if we still have one.
     */
#if MYNEWT_VAL(BLE_LL_STRICT_CONN_SCHEDULING)
    itvl = g_ble_ll_sched_data.sch_ticks_per_period;
#else
    itvl = MYNEWT_VAL(BLE_LL_CONN_INIT_SLOTS) * BLE_LL_SCHED_32KHZ_TICKS_PER_SLOT;
#endif
    if (connsm->conn_role == BLE_LL_CONN_ROLE_SLAVE) {

        cur_ww = ble_ll_utils_calc_window_widening(connsm->anchor_point,
                                                   connsm->last_anchor_point,
                                                   connsm->master_sca);
        max_ww = (connsm->conn_itvl * (BLE_LL_CONN_ITVL_USECS/2)) - BLE_LL_IFS;
        if (cur_ww >= max_ww) {
            return -1;
        }
        cur_ww += BLE_LL_JITTER_USECS;
        connsm->slave_cur_window_widening = cur_ww;
        itvl += os_cputime_usecs_to_ticks(cur_ww + connsm->slave_cur_tx_win_usecs);
    }
    itvl -= g_ble_ll_sched_offset_ticks;
    connsm->ce_end_time = connsm->anchor_point + itvl;

    return 0;
}

/**
 * Called when a connection has been created. This function will
 *  -> Set the connection state to created.
 *  -> Start the connection supervision timer
 *  -> Set the Link Layer state to connection.
 *  -> Send a connection complete event.
 *
 *  See Section 4.5.2 Vol 6 Part B
 *
 *  Context: Link Layer
 *
 * @param connsm
 *
 * @ return 0: connection NOT created. 1: connection created
 */
static int
ble_ll_conn_created(struct ble_ll_conn_sm *connsm, struct ble_mbuf_hdr *rxhdr)
{
    int rc;
    uint8_t *evbuf;
    uint32_t endtime;
    uint32_t usecs;

    /* XXX: TODO this assumes we received in 1M phy */

    /* Set state to created */
    connsm->conn_state = BLE_LL_CONN_STATE_CREATED;

    /* Clear packet received flag */
    connsm->csmflags.cfbit.pkt_rxd = 0;

    /* Consider time created the last scheduled time */
    connsm->last_scheduled = os_cputime_get32();

    /*
     * Set the last rxd pdu time since this is where we want to start the
     * supervision timer from.
     */
    connsm->last_rxd_pdu_cputime = connsm->last_scheduled;

    /*
     * Set first connection event time. If slave the endtime is the receive end
     * time of the connect request. The actual connection starts 1.25 msecs plus
     * the transmit window offset from the end of the connection request.
     */
    rc = 1;
    if (connsm->conn_role == BLE_LL_CONN_ROLE_SLAVE) {
        /*
         * With a 32.768 kHz crystal we dont care about the remaining usecs
         * when setting last anchor point. The only thing last anchor is used
         * for is to calculate window widening. The effect of this is
         * negligible.
         */
        connsm->last_anchor_point = rxhdr->beg_cputime;

        usecs = rxhdr->rem_usecs + 1250 +
                (connsm->tx_win_off * BLE_LL_CONN_TX_WIN_USECS) +
                ble_ll_pdu_tx_time_get(BLE_CONNECT_REQ_LEN,
                                       rxhdr->rxinfo.phy_mode);

        if (rxhdr->rxinfo.channel < BLE_PHY_NUM_DATA_CHANS) {
            switch (rxhdr->rxinfo.phy) {
            case BLE_PHY_1M:
            case BLE_PHY_2M:
                usecs += 1250;
                break;
            case BLE_PHY_CODED:
                usecs += 2500;
                break;
            default:
                BLE_LL_ASSERT(0);
                break;
            }
        }

        /* Anchor point is cputime. */
        endtime = os_cputime_usecs_to_ticks(usecs);
        connsm->anchor_point = rxhdr->beg_cputime + endtime;
        connsm->anchor_point_usecs = usecs - os_cputime_ticks_to_usecs(endtime);
        if (connsm->anchor_point_usecs == 31) {
            ++connsm->anchor_point;
            connsm->anchor_point_usecs = 0;
        }

        connsm->slave_cur_tx_win_usecs =
            connsm->tx_win_size * BLE_LL_CONN_TX_WIN_USECS;
#if MYNEWT_VAL(BLE_LL_STRICT_CONN_SCHEDULING)
        connsm->ce_end_time = connsm->anchor_point +
            g_ble_ll_sched_data.sch_ticks_per_period +
            os_cputime_usecs_to_ticks(connsm->slave_cur_tx_win_usecs) + 1;

#else
        connsm->ce_end_time = connsm->anchor_point +
            (MYNEWT_VAL(BLE_LL_CONN_INIT_SLOTS) * BLE_LL_SCHED_32KHZ_TICKS_PER_SLOT)
            + os_cputime_usecs_to_ticks(connsm->slave_cur_tx_win_usecs) + 1;
#endif
        connsm->slave_cur_window_widening = BLE_LL_JITTER_USECS;

        /* Start the scheduler for the first connection event */
        while (ble_ll_sched_slave_new(connsm)) {
            if (ble_ll_conn_next_event(connsm)) {
                STATS_INC(ble_ll_conn_stats, cant_set_sched);
                rc = 0;
                break;
            }
        }
    }

    /* Send connection complete event to inform host of connection */
    if (rc) {
#if (BLE_LL_BT5_PHY_SUPPORTED == 1)
        /*
         * If we have default phy preferences and they are different than
         * the current PHY's in use, start update procedure.
         */
        /*
         * XXX: should we attempt to start this without knowing if
         * the other side can support it?
         */
        if (!ble_ll_conn_chk_phy_upd_start(connsm)) {
            CONN_F_CTRLR_PHY_UPDATE(connsm) = 1;
        }
#endif
        if (connsm->conn_role == BLE_LL_CONN_ROLE_SLAVE) {
            ble_ll_adv_send_conn_comp_ev(connsm, rxhdr);
        } else {
            evbuf = ble_ll_init_get_conn_comp_ev();
            ble_ll_conn_comp_event_send(connsm, BLE_ERR_SUCCESS, evbuf, NULL);
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CSA2)
            ble_ll_hci_ev_le_csa(connsm);
#endif

            /*
             * Initiate features exchange
             *
             * XXX we do this only as a master as it was observed that sending
             * LL_SLAVE_FEATURE_REQ after connection breaks some recent iPhone
             * models; for slave just assume master will initiate features xchg
             * if it has some additional features to use.
             */
            ble_ll_ctrl_proc_start(connsm, BLE_LL_CTRL_PROC_FEATURE_XCHG);
        }
    }

    return rc;
}

/**
 * Called upon end of connection event
 *
 * Context: Link-layer task
 *
 * @param void *arg Pointer to connection state machine
 *
 */
static void
ble_ll_conn_event_end(struct ble_npl_event *ev)
{
    uint8_t ble_err;
    uint32_t tmo;
    struct ble_ll_conn_sm *connsm;

    ble_ll_rfmgmt_release();

    /* Better be a connection state machine! */
    connsm = (struct ble_ll_conn_sm *)ble_npl_event_get_arg(ev);
    BLE_LL_ASSERT(connsm);
    if (connsm->conn_state == BLE_LL_CONN_STATE_IDLE) {
        /* That should not happen. If it does it means connection
         * is already closed.
         * Make sure LL state machine is in idle
         */
        STATS_INC(ble_ll_conn_stats, sched_end_in_idle);
        BLE_LL_ASSERT(0);

        /* Just in case */
        ble_ll_state_set(BLE_LL_STATE_STANDBY);

        ble_ll_scan_chk_resume();
        return;
    }

    /* Log event end */
    ble_ll_trace_u32x2(BLE_LL_TRACE_ID_CONN_EV_END, connsm->conn_handle,
                       connsm->event_cntr);

    ble_ll_scan_chk_resume();

    /* If we have transmitted the terminate IND successfully, we are done */
    if ((connsm->csmflags.cfbit.terminate_ind_txd) ||
                    (connsm->csmflags.cfbit.terminate_ind_rxd &&
                     connsm->csmflags.cfbit.terminate_ind_rxd_acked)) {
        if (connsm->csmflags.cfbit.terminate_ind_txd) {
            ble_err = BLE_ERR_CONN_TERM_LOCAL;
        } else {
            /* Make sure the disconnect reason is valid! */
            ble_err = connsm->rxd_disconnect_reason;
            if (ble_err == 0) {
                ble_err = BLE_ERR_REM_USER_CONN_TERM;
            }
        }
        ble_ll_conn_end(connsm, ble_err);
        return;
    }

    /* Remove any connection end events that might be enqueued */
    ble_npl_eventq_remove(&g_ble_ll_data.ll_evq, &connsm->conn_ev_end);

    /*
     * If we have received a packet, we can set the current transmit window
     * usecs to 0 since we dont need to listen in the transmit window.
     */
    if (connsm->csmflags.cfbit.pkt_rxd) {
        connsm->slave_cur_tx_win_usecs = 0;
    }

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_PING)
    /*
     * If we are encrypted and have passed the authenticated payload timeout
     * we need to send an event to tell the host. Unfortunately, I think we
     * need one of these per connection and we have to set this timer
     * fairly accurately. So we need to another event in the connection.
     * This sucks.
     *
     * The way this works is that whenever the timer expires it just gets reset
     * and we send the autheticated payload timeout event. Note that this timer
     * should run even when encryption is paused.
     * XXX: what should be here? Was there code here that got deleted?
     */
#endif

    /* Move to next connection event */
    if (ble_ll_conn_next_event(connsm)) {
        ble_ll_conn_end(connsm, BLE_ERR_CONN_TERM_LOCAL);
        return;
    }

    /* Reset "per connection event" variables */
    connsm->cons_rxd_bad_crc = 0;
    connsm->csmflags.cfbit.pkt_rxd = 0;

    /* See if we need to start any control procedures */
    ble_ll_ctrl_chk_proc_start(connsm);

    /* Set initial schedule callback */
    connsm->conn_sch.sched_cb = ble_ll_conn_event_start_cb;

    /* XXX: I think all this fine for when we do connection updates, but
       we may want to force the first event to be scheduled. Not sure */
    /* Schedule the next connection event */
    while (ble_ll_sched_conn_reschedule(connsm)) {
        if (ble_ll_conn_next_event(connsm)) {
            ble_ll_conn_end(connsm, BLE_ERR_CONN_TERM_LOCAL);
            return;
        }
    }

    /*
     * This is definitely not perfect but hopefully will be fine in regards to
     * the specification. We check the supervision timer at connection event
     * end. If the next connection event is going to start past the supervision
     * timeout we end the connection here. I guess this goes against the spec
     * in two ways:
     * 1) We are actually causing a supervision timeout before the time
     * specified. However, this is really a moot point because the supervision
     * timeout would have expired before we could possibly receive a packet.
     * 2) We may end the supervision timeout a bit later than specified as
     * we only check this at event end and a bad CRC could cause us to continue
     * the connection event longer than the supervision timeout. Given that two
     * bad CRC's consecutively ends the connection event, I dont regard this as
     * a big deal but it could cause a slightly longer supervision timeout.
     */
    if (connsm->conn_state == BLE_LL_CONN_STATE_CREATED) {
        tmo = (uint32_t)connsm->conn_itvl * BLE_LL_CONN_ITVL_USECS * 6UL;
        ble_err = BLE_ERR_CONN_ESTABLISHMENT;
    } else {
        tmo = connsm->supervision_tmo * BLE_HCI_CONN_SPVN_TMO_UNITS * 1000UL;
        ble_err = BLE_ERR_CONN_SPVN_TMO;
    }
    /* XXX: Convert to ticks to usecs calculation instead??? */
    tmo = os_cputime_usecs_to_ticks(tmo);
    if ((int32_t)(connsm->anchor_point - connsm->last_rxd_pdu_cputime) >= tmo) {
        ble_ll_conn_end(connsm, ble_err);
        return;
    }

    /* If we have completed packets, send an event */
    ble_ll_conn_num_comp_pkts_event_send(connsm);

    /* If we have features and there's pending HCI command, send an event */
    if (connsm->csmflags.cfbit.pending_hci_rd_features &&
                                        connsm->csmflags.cfbit.rxd_features) {
        ble_ll_hci_ev_rd_rem_used_feat(connsm, BLE_ERR_SUCCESS);
        connsm->csmflags.cfbit.pending_hci_rd_features = 0;
    }
}

/**
 * Update the connection request PDU with the address type and address of
 * advertiser we are going to send connect request to.
 *
 * @param m
 * @param adva
 * @param addr_type     Address type of ADVA from received advertisement.
 * @param inita
 * @param inita_type     Address type of INITA from received advertisement.

 * @param txoffset      The tx window offset for this connection
 */
static void
ble_ll_conn_connect_ind_prepare(struct ble_ll_conn_sm *connsm,
                                struct ble_ll_scan_pdu_data *pdu_data,
                                uint8_t adva_type, uint8_t *adva,
                                uint8_t inita_type, uint8_t *inita,
                                int rpa_index, uint8_t channel)
{
    uint8_t hdr;
    uint8_t *addr;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
    int is_rpa;
    struct ble_ll_resolv_entry *rl;
#endif

    hdr = BLE_ADV_PDU_TYPE_CONNECT_IND;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CSA2)
    /* We need CSA2 bit only for legacy connect */
    if (channel >= BLE_PHY_NUM_DATA_CHANS) {
        hdr |= BLE_ADV_PDU_HDR_CHSEL;
    }
#endif

    if (adva_type) {
        /* Set random address */
        hdr |= BLE_ADV_PDU_HDR_RXADD_MASK;
    }

    if (inita) {
        memcpy(pdu_data->inita, inita, BLE_DEV_ADDR_LEN);
        if (inita_type) {
            hdr |= BLE_ADV_PDU_HDR_TXADD_RAND;
        }
    } else {
        /* Get pointer to our device address */
        connsm = g_ble_ll_conn_create_sm;
        if ((connsm->own_addr_type & 1) == 0) {
            addr = g_dev_addr;
        } else {
            hdr |= BLE_ADV_PDU_HDR_TXADD_RAND;
            addr = g_random_addr;
        }

    /* XXX: do this ahead of time? Calculate the local rpa I mean */
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
        if (connsm->own_addr_type > BLE_HCI_ADV_OWN_ADDR_RANDOM) {
            rl = NULL;
            is_rpa = ble_ll_is_rpa(adva, adva_type);
            if (is_rpa) {
                if (rpa_index >= 0) {
                    rl = &g_ble_ll_resolv_list[rpa_index];
                }
            } else {
                /* we look for RL entry to generate local RPA regardless if
                 * resolving is enabled or not (as this is is for local RPA
                 * not peer RPA)
                 */
                 rl = ble_ll_resolv_list_find(adva, adva_type);
            }

            /*
             * If peer in on resolving list, we use RPA generated with Local IRK
             * from resolving list entry. In other case, we need to use our identity
             * address (see  Core 5.0, Vol 6, Part B, section 6.4).
             */
            if (rl && rl->rl_has_local) {
                hdr |= BLE_ADV_PDU_HDR_TXADD_RAND;
                ble_ll_resolv_get_priv_addr(rl, 1, pdu_data->inita);
                addr = NULL;
            }
        }
#endif

        if (addr) {
            memcpy(pdu_data->inita, addr, BLE_DEV_ADDR_LEN);
            /* Identity address used */
            connsm->inita_identity_used = 1;
        }
    }

    memcpy(pdu_data->adva, adva, BLE_DEV_ADDR_LEN);

    pdu_data->hdr_byte = hdr;
}

/* Returns true if the address matches the connection peer address having in
 * mind privacy mode
 */
static int
ble_ll_conn_is_peer_adv(uint8_t addr_type, uint8_t *adva, int index)
{
    int rc;
    uint8_t *peer_addr = NULL;
    struct ble_ll_conn_sm *connsm;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
    struct ble_ll_resolv_entry *rl;
#endif

    /* XXX: Deal with different types of random addresses here! */
    connsm = g_ble_ll_conn_create_sm;
    if (!connsm) {
        return 0;
    }

    switch (connsm->peer_addr_type) {
    /* Fall-through intentional */
    case BLE_HCI_CONN_PEER_ADDR_PUBLIC:
    case BLE_HCI_CONN_PEER_ADDR_RANDOM:
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
        if (ble_ll_addr_is_id(adva, addr_type)) {
                /* Peer uses its identity address. Let's verify privacy mode.
                 *
                 * Note: Core Spec 5.0 Vol 6, Part B
                 * If the Host has added the peer device to the resolving list
                 * with an all-zero peer IRK, the Controller shall only accept
                 * the peer's identity address.
                 */
            if (ble_ll_resolv_enabled()) {
                rl = ble_ll_resolv_list_find(adva, addr_type);
                if (rl && (rl->rl_priv_mode == BLE_HCI_PRIVACY_NETWORK) &&
                    rl->rl_has_peer) {
                    return 0;
                }
            }
        }

        /* Check if peer uses RPA. If so and it match, use it as controller
         * supports privacy mode
         */
        if ((index >= 0) &&
                (g_ble_ll_resolv_list[index].rl_addr_type == connsm->peer_addr_type)) {
            peer_addr = g_ble_ll_resolv_list[index].rl_identity_addr;
        }
#endif
        /*
         * If we are here it means we don't know the device, lets
         * check if type is what we are looking for and later
         * if address matches
         */
        if ((connsm->peer_addr_type == addr_type) && !peer_addr) {
            peer_addr = adva;
        }

        break;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
    case BLE_HCI_CONN_PEER_ADDR_PUBLIC_IDENT:
        if ((index < 0) ||
            (g_ble_ll_resolv_list[index].rl_addr_type != 0)) {
            return 0;
        }
        peer_addr = g_ble_ll_resolv_list[index].rl_identity_addr;
        break;
    case BLE_HCI_CONN_PEER_ADDR_RANDOM_IDENT:
        if ((index < 0) ||
            (g_ble_ll_resolv_list[index].rl_addr_type != 1)) {
            return 0;
        }
        peer_addr = g_ble_ll_resolv_list[index].rl_identity_addr;
        break;
#endif
    default:
        peer_addr = NULL;
        break;
    }

    rc = 0;
    if (peer_addr) {
        if (!memcmp(peer_addr, connsm->peer_addr, BLE_DEV_ADDR_LEN)) {
            rc = 1;
        }
    }

    return rc;
}

static void
ble_ll_conn_connect_ind_txend_to_standby(void *arg)
{
    ble_ll_state_set(BLE_LL_STATE_STANDBY);
}

static void
ble_ll_conn_connect_ind_txend_to_init(void *arg)
{
    ble_ll_state_set(BLE_LL_STATE_INITIATING);
}

static uint8_t
ble_ll_conn_connect_ind_tx_pducb(uint8_t *dptr, void *pducb_arg, uint8_t *hdr_byte)
{
    struct ble_ll_conn_sm *connsm;
    struct ble_ll_scan_pdu_data *pdu_data;

    connsm = pducb_arg;
    /*
     * pdu_data was prepared just before starting TX and is expected to be
     * still valid here
     */
    pdu_data = ble_ll_scan_get_pdu_data();

    memcpy(dptr, pdu_data->inita, BLE_DEV_ADDR_LEN);
    memcpy(dptr + BLE_DEV_ADDR_LEN, pdu_data->adva, BLE_DEV_ADDR_LEN);

    dptr += 2 * BLE_DEV_ADDR_LEN;

    put_le32(dptr, connsm->access_addr);
    dptr[4] = (uint8_t)connsm->crcinit;
    dptr[5] = (uint8_t)(connsm->crcinit >> 8);
    dptr[6] = (uint8_t)(connsm->crcinit >> 16);
    dptr[7] = connsm->tx_win_size;
    put_le16(dptr + 8, connsm->tx_win_off);
    put_le16(dptr + 10, connsm->conn_itvl);
    put_le16(dptr + 12, connsm->slave_latency);
    put_le16(dptr + 14, connsm->supervision_tmo);
    memcpy(dptr + 16, &connsm->chanmap, BLE_LL_CONN_CHMAP_LEN);
    dptr[21] = connsm->hop_inc | (connsm->master_sca << 5);

    *hdr_byte = pdu_data->hdr_byte;

    return 34;
}

/**
 * Send a connection requestion to an advertiser
 *
 * Context: Interrupt
 *
 * @param addr_type Address type of advertiser
 * @param adva Address of advertiser
 */
int
ble_ll_conn_connect_ind_send(struct ble_ll_conn_sm *connsm, uint8_t end_trans)
{
    int rc;

    if (end_trans == BLE_PHY_TRANSITION_NONE) {
        ble_phy_set_txend_cb(ble_ll_conn_connect_ind_txend_to_standby, NULL);
    } else {
        ble_phy_set_txend_cb(ble_ll_conn_connect_ind_txend_to_init, NULL);
    }

    rc = ble_phy_tx(ble_ll_conn_connect_ind_tx_pducb, connsm, end_trans);

    return rc;
}

/**
 * Called when a schedule item overlaps the currently running connection
 * event. This generally should not happen, but if it does we stop the
 * current connection event to let the schedule item run.
 *
 * NOTE: the phy has been disabled as well as the wfr timer before this is
 * called.
 */
void
ble_ll_conn_event_halt(void)
{
    ble_ll_state_set(BLE_LL_STATE_STANDBY);
    if (g_ble_ll_conn_cur_sm) {
        g_ble_ll_conn_cur_sm->csmflags.cfbit.pkt_rxd = 0;
        ble_ll_event_send(&g_ble_ll_conn_cur_sm->conn_ev_end);
        g_ble_ll_conn_cur_sm = NULL;
    }
}

/**
 * Process a received PDU while in the initiating state.
 *
 * Context: Link Layer task.
 *
 * @param pdu_type
 * @param rxbuf
 * @param ble_hdr
 */
void
ble_ll_init_rx_pkt_in(uint8_t pdu_type, uint8_t *rxbuf,
                      struct ble_mbuf_hdr *ble_hdr)
{
    uint8_t addr_type;
    uint8_t *addr;
    uint8_t *adv_addr;
    uint8_t *inita;
    uint8_t inita_type;
    struct ble_ll_conn_sm *connsm;
    int ext_adv_mode = -1;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    struct ble_ll_aux_data *aux_data = NULL;

     if (ble_hdr->rxinfo.user_data) {
         /* aux_data just a local helper, no need to ref
          * as ble_hdr->rxinfo.user_data is unref in the end of this function
          */
         aux_data = ble_hdr->rxinfo.user_data;
     }
#endif

    /* Get the connection state machine we are trying to create */
    connsm = g_ble_ll_conn_create_sm;
    if (!connsm) {
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
        if (aux_data) {
            ble_ll_scan_aux_data_unref(ble_hdr->rxinfo.user_data);
            ble_hdr->rxinfo.user_data = NULL;
        }
#endif
        return;
    }

    if (!BLE_MBUF_HDR_CRC_OK(ble_hdr)) {
        goto scan_continue;
    }

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    if (BLE_MBUF_HDR_AUX_INVALID(ble_hdr)) {
        goto scan_continue;
    }

    if (pdu_type == BLE_ADV_PDU_TYPE_ADV_EXT_IND) {
        if (BLE_MBUF_HDR_WAIT_AUX(ble_hdr)) {
            /* Just continue scanning. We are waiting for AUX */
            if (!ble_ll_sched_aux_scan(ble_hdr, connsm->scansm, aux_data)) {
                /* ref for aux ptr in the scheduler */
                ble_ll_scan_aux_data_unref(ble_hdr->rxinfo.user_data);
                ble_hdr->rxinfo.user_data = NULL;
                ble_ll_scan_chk_resume();
                return;
            }
            goto scan_continue;
        }
    }

    if (CONN_F_AUX_CONN_REQ(connsm)) {
        if (pdu_type != BLE_ADV_PDU_TYPE_AUX_CONNECT_RSP) {
            /* Wait for connection response, in this point of time aux is NULL */
            BLE_LL_ASSERT(ble_hdr->rxinfo.user_data == NULL);
            return;
        }
    }
#endif

    /* If we have sent a connect request, we need to enter CONNECTION state */
    if (connsm && CONN_F_CONN_REQ_TXD(connsm)) {
        /* Set address of advertiser to which we are connecting. */

        if (ble_ll_scan_adv_decode_addr(pdu_type, rxbuf, ble_hdr,
                                        &adv_addr, &addr_type,
                                        &inita, &inita_type, &ext_adv_mode)) {
            /* Something got wrong, keep trying to connect */
            goto scan_continue;
        }

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
        /*
         * Did we resolve this address? If so, set correct peer address
         * and peer address type.
         */
        if (connsm->rpa_index >= 0) {
            addr_type = g_ble_ll_resolv_list[connsm->rpa_index].rl_addr_type + 2;
            addr = g_ble_ll_resolv_list[connsm->rpa_index].rl_identity_addr;
        } else {
            addr = adv_addr;
        }
#else
        addr = adv_addr;
#endif

        if (connsm->rpa_index >= 0) {
            connsm->peer_addr_type = addr_type;
            memcpy(connsm->peer_addr, addr, BLE_DEV_ADDR_LEN);

            ble_ll_scan_set_peer_rpa(adv_addr);

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
            /* Update resolving list with current peer RPA */
            ble_ll_resolv_set_peer_rpa(connsm->rpa_index, rxbuf + BLE_LL_PDU_HDR_LEN);
            if (ble_ll_is_rpa(inita, inita_type)) {
                ble_ll_resolv_set_local_rpa(connsm->rpa_index, inita);
            }

#endif
        } else if (ble_ll_scan_whitelist_enabled()) {
            /* if WL is used we need to store peer addr also if it was not
             * resolved
             */
            connsm->peer_addr_type = addr_type;
            memcpy(connsm->peer_addr, addr, BLE_DEV_ADDR_LEN);
        }

        /* Connection has been created. Stop scanning */
        g_ble_ll_conn_create_sm = NULL;
        ble_ll_scan_sm_stop(0);

        /* For AUX Connect CSA2 is mandatory. Otherwise we need to check bit
         * mask
         */
        if (ble_hdr->rxinfo.channel < BLE_PHY_NUM_DATA_CHANS) {
            ble_ll_conn_set_csa(connsm, 1);
        } else {
            ble_ll_conn_set_csa(connsm, rxbuf[0] & BLE_ADV_PDU_HDR_CHSEL_MASK);
        }

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
#if (BLE_LL_BT5_PHY_SUPPORTED == 1)
        /* Lets take last used phy */
        ble_ll_conn_init_phy(connsm, ble_hdr->rxinfo.phy);
#endif
        if (aux_data) {
            ble_ll_scan_aux_data_unref(ble_hdr->rxinfo.user_data);
            ble_hdr->rxinfo.user_data = NULL;
        }
#endif
        ble_ll_conn_created(connsm, NULL);
        return;
    }

scan_continue:
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    /* Drop last reference and keep continue to connect */
    if (aux_data) {
        ble_ll_scan_aux_data_unref(ble_hdr->rxinfo.user_data);
        ble_hdr->rxinfo.user_data = NULL;
    }
#endif
    ble_ll_scan_chk_resume();
}

/**
 * Called when a receive PDU has started and we are in the initiating state.
 *
 * Context: Interrupt
 *
 * @param pdu_type
 * @param ble_hdr
 *
 * @return int
 *  0: we will not attempt to reply to this frame
 *  1: we may send a response to this frame.
 */
int
ble_ll_init_rx_isr_start(uint8_t pdu_type, struct ble_mbuf_hdr *ble_hdr)
{
    struct ble_ll_conn_sm *connsm;

    connsm = g_ble_ll_conn_create_sm;
    if (!connsm) {
        return 0;
    }

    if ((pdu_type == BLE_ADV_PDU_TYPE_ADV_IND) ||
        (pdu_type == BLE_ADV_PDU_TYPE_ADV_DIRECT_IND ||
         pdu_type == BLE_ADV_PDU_TYPE_AUX_CONNECT_RSP)) {
        return 1;
    }

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    if (pdu_type == BLE_ADV_PDU_TYPE_ADV_EXT_IND &&
                                                connsm->scansm->ext_scanning) {
        if (connsm->scansm->cur_aux_data) {
            STATS_INC(ble_ll_stats, aux_received);
        }

        ble_hdr->rxinfo.flags |= BLE_MBUF_HDR_F_EXT_ADV;
        return 1;
    }
#endif

    return 0;
}

/**
 * Called when a receive PDU has ended and we are in the initiating state.
 *
 * Context: Interrupt
 *
 * @param rxpdu
 * @param crcok
 * @param ble_hdr
 *
 * @return int
 *       < 0: Disable the phy after reception.
 *      == 0: Success. Do not disable the PHY.
 *       > 0: Do not disable PHY as that has already been done.
 */
int
ble_ll_init_rx_isr_end(uint8_t *rxbuf, uint8_t crcok,
                       struct ble_mbuf_hdr *ble_hdr)
{
    int rc;
    int resolved;
    int chk_wl;
    int index;
    uint8_t pdu_type;
    uint8_t adv_addr_type;
    uint8_t peer_addr_type;
    uint8_t *adv_addr = NULL;
    uint8_t *peer;
    uint8_t *init_addr = NULL;
    uint8_t init_addr_type;
    uint8_t pyld_len;
    uint8_t inita_is_rpa;
    uint8_t conn_req_end_trans;
    struct os_mbuf *rxpdu;
    struct ble_ll_conn_sm *connsm;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
    struct ble_ll_resolv_entry *rl;
#endif
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    struct ble_ll_scan_sm *scansm;
    uint8_t phy;
#endif
    int ext_adv_mode = -1;

    /* Get connection state machine to use if connection to be established */
    connsm = g_ble_ll_conn_create_sm;
    /* This could happen if connection init was cancelled while isr end was
     * already pending
     */
    if (!connsm) {
        ble_ll_state_set(BLE_LL_STATE_STANDBY);
        return -1;
    }

    rc = -1;
    pdu_type = rxbuf[0] & BLE_ADV_PDU_HDR_TYPE_MASK;
    pyld_len = rxbuf[1];

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    scansm = connsm->scansm;
    if (scansm->cur_aux_data) {
        ble_hdr->rxinfo.user_data = scansm->cur_aux_data;
        scansm->cur_aux_data = NULL;
    }
#endif

    if (!crcok) {
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
        /* Invalid packet - make sure we do not wait for AUX_CONNECT_RSP */
        ble_ll_conn_reset_pending_aux_conn_rsp();
#endif

        /* Ignore this packet */
        goto init_rx_isr_exit;
    }

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    /* If we sent AUX_CONNECT_REQ, we only expect AUX_CONNECT_RSP here */
    if (CONN_F_AUX_CONN_REQ(connsm)) {
        if (pdu_type != BLE_ADV_PDU_TYPE_AUX_CONNECT_RSP) {
            STATS_INC(ble_ll_stats, aux_conn_rsp_err);
            CONN_F_CONN_REQ_TXD(connsm) = 0;
            CONN_F_AUX_CONN_REQ(connsm) = 0;
            ble_ll_sched_rmv_elem(&connsm->conn_sch);
        }
        goto init_rx_isr_exit;
    }
#endif

    inita_is_rpa = 0;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    if (pdu_type == BLE_ADV_PDU_TYPE_ADV_EXT_IND) {
        if (!scansm->ext_scanning) {
            goto init_rx_isr_exit;
        }

        rc = ble_ll_scan_update_aux_data(ble_hdr, rxbuf, NULL);
        if (rc < 0) {
            /* No memory or broken packet */
            ble_hdr->rxinfo.flags |= BLE_MBUF_HDR_F_AUX_INVALID;
            goto init_rx_isr_exit;
        }
    }
#endif

    /* Lets get addresses from advertising report*/
    if (ble_ll_scan_adv_decode_addr(pdu_type, rxbuf, ble_hdr,
                                    &adv_addr, &adv_addr_type,
                                    &init_addr, &init_addr_type,
                                    &ext_adv_mode)) {
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
        ble_hdr->rxinfo.flags |= BLE_MBUF_HDR_F_AUX_INVALID;
#endif
        goto init_rx_isr_exit;
    }

    switch (pdu_type) {
    case BLE_ADV_PDU_TYPE_ADV_IND:
        break;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    case BLE_ADV_PDU_TYPE_ADV_EXT_IND:
        rc = -1;

        /* If this is not connectable adv mode, lets skip it */
        if (!(ext_adv_mode & BLE_LL_EXT_ADV_MODE_CONN)) {
            goto init_rx_isr_exit;
        }

        if (!adv_addr) {
            ble_hdr->rxinfo.flags |= BLE_MBUF_HDR_F_AUX_PTR_WAIT;
            goto init_rx_isr_exit;
        }

        if (!init_addr) {
            break;
        }
        /* if there is direct address lets fall down and check it.*/
        // no break
#endif
    case BLE_ADV_PDU_TYPE_ADV_DIRECT_IND:
        inita_is_rpa = (uint8_t)ble_ll_is_rpa(init_addr, init_addr_type);
        if (!inita_is_rpa) {

            /* Resolving will be done later. Check if identity InitA matches */
            if (!ble_ll_is_our_devaddr(init_addr, init_addr_type)) {
                goto init_rx_isr_exit;
            }
        }
#if !MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
        else {
            /* If privacy is off - reject RPA InitA*/
            goto init_rx_isr_exit;
        }
#endif

        break;
    default:
        goto init_rx_isr_exit;
    }

    /* Should we send a connect request? */
    index = -1;
    peer = adv_addr;
    peer_addr_type = adv_addr_type;

    resolved = 0;
    chk_wl = ble_ll_scan_whitelist_enabled();

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
    if (ble_ll_is_rpa(adv_addr, adv_addr_type) && ble_ll_resolv_enabled()) {
        index = ble_hw_resolv_list_match();
        if (index >= 0) {
            rl = &g_ble_ll_resolv_list[index];

            ble_hdr->rxinfo.flags |= BLE_MBUF_HDR_F_RESOLVED;
            connsm->rpa_index = index;
            peer = rl->rl_identity_addr;
            peer_addr_type = rl->rl_addr_type;
            resolved = 1;

            /* Assure privacy */
            if ((rl->rl_priv_mode == BLE_HCI_PRIVACY_NETWORK) && init_addr &&
                !inita_is_rpa && rl->rl_has_local) {
                goto init_rx_isr_exit;
            }

            /*
             * If the InitA is a RPA, we must see if it resolves based on the
             * identity address of the resolved ADVA.
             */
            if (init_addr && inita_is_rpa) {
                if (!ble_ll_resolv_rpa(init_addr,
                                       g_ble_ll_resolv_list[index].rl_local_irk)) {
                    goto init_rx_isr_exit;
                }

                /* Core Specification Vol 6, Part B, Section 6.4:
                 * "The Link Layer should not set the InitA field to the same
                 * value as the TargetA field in the received advertising PDU."
                 *
                 * We update the received PDU directly here, so ble_ll_init_rx_pkt_in
                 * can process it as is.
                 */
                memcpy(init_addr, rl->rl_local_rpa, BLE_DEV_ADDR_LEN);
            }

        } else {
            if (chk_wl) {
                goto init_rx_isr_exit;
            }

            /* Could not resolved InitA */
            if (init_addr && inita_is_rpa) {
                goto init_rx_isr_exit;
            }
        }
    } else if (init_addr) {

        /* If resolving is off and InitA is RPA we reject advertising */
        if (inita_is_rpa && !ble_ll_resolv_enabled()) {
            goto init_rx_isr_exit;
        }

        /* Let's see if we have IRK with that peer.*/
        rl = ble_ll_resolv_list_find(adv_addr, adv_addr_type);

        /* Lets make sure privacy mode is correct together with InitA in case it
         * is identity address
         */
        if (rl && !inita_is_rpa &&
           (rl->rl_priv_mode == BLE_HCI_PRIVACY_NETWORK) &&
           rl->rl_has_local) {
            goto init_rx_isr_exit;
        }

        /*
         * If the InitA is a RPA, we must see if it resolves based on the
         * identity address of the resolved ADVA.
         */
        if (inita_is_rpa) {
            if (!rl || !ble_ll_resolv_rpa(init_addr, rl->rl_local_irk)) {
                goto init_rx_isr_exit;
            }

            /* Core Specification Vol 6, Part B, Section 6.4:
             * "The Link Layer should not set the InitA field to the same
             * value as the TargetA field in the received advertising PDU."
             *
             * We update the received PDU directly here, so ble_ll_init_rx_pkt_in
             * can process it as is.
             */
            memcpy(init_addr, rl->rl_local_rpa, BLE_DEV_ADDR_LEN);
        }
    }
#endif

    /* Check filter policy */
    if (chk_wl) {
        if (!ble_ll_whitelist_match(peer, peer_addr_type, resolved)) {
            goto init_rx_isr_exit;
        }
    } else {
        /* Must match the connection address */
        if (!ble_ll_conn_is_peer_adv(adv_addr_type, adv_addr, index)) {
            goto init_rx_isr_exit;
        }
    }
    ble_hdr->rxinfo.flags |= BLE_MBUF_HDR_F_DEVMATCH;

    /* For CONNECT_IND we don't go into RX state */
    conn_req_end_trans = BLE_PHY_TRANSITION_NONE;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    /* Check if we should send AUX_CONNECT_REQ and wait for AUX_CONNECT_RSP */
    if (ble_hdr->rxinfo.channel < BLE_PHY_NUM_DATA_CHANS) {
        conn_req_end_trans = BLE_PHY_TRANSITION_TX_RX;
    }

    if (connsm->scansm->ext_scanning) {
            phy = ble_hdr->rxinfo.phy;

            /* Update connection state machine with appropriate parameters for
             * certain PHY
             */
            ble_ll_conn_ext_set_params(connsm,
                                       &connsm->initial_params.params[phy - 1],
                                       phy);

    }
#endif

    /* Schedule new connection */
    if (ble_ll_sched_master_new(connsm, ble_hdr, pyld_len)) {
        STATS_INC(ble_ll_conn_stats, cant_set_sched);
        goto init_rx_isr_exit;
    }

    /* Prepare data for connect request */
    ble_ll_conn_connect_ind_prepare(connsm,
                                    ble_ll_scan_get_pdu_data(),
                                    adv_addr_type, adv_addr,
                                    init_addr_type, init_addr,
                                    index, ble_hdr->rxinfo.channel);

    /* Setup to transmit the connect request */
    rc = ble_ll_conn_connect_ind_send(connsm, conn_req_end_trans);
    if (rc) {
        ble_ll_sched_rmv_elem(&connsm->conn_sch);
        goto init_rx_isr_exit;
    }

    if (init_addr && !inita_is_rpa) {
        connsm->inita_identity_used = 1;
    }

    CONN_F_CONN_REQ_TXD(connsm) = 1;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    if (ble_hdr->rxinfo.channel < BLE_PHY_NUM_DATA_CHANS) {
        /* Lets wait for AUX_CONNECT_RSP */
        CONN_F_AUX_CONN_REQ(connsm) = 1;
        /* Keep aux data until we get scan response */
        scansm->cur_aux_data = ble_hdr->rxinfo.user_data;
        ble_hdr->rxinfo.user_data = NULL;
        STATS_INC(ble_ll_stats, aux_conn_req_tx);
    }
#endif

    STATS_INC(ble_ll_conn_stats, conn_req_txd);

init_rx_isr_exit:

    /*
     * We have to restart receive if we cant hand up pdu. We return 0 so that
     * the phy does not get disabled.
     */
    rxpdu = ble_ll_rxpdu_alloc(pyld_len + BLE_LL_PDU_HDR_LEN);
    if (rxpdu == NULL) {
        /*
         * XXX: possible allocate the PDU when we start initiating?
         * I cannot say I like this solution, but if we cannot allocate a PDU
         * to hand up to the LL, we need to remove the connection we just
         * scheduled since the connection state machine will not get processed
         * by link layer properly. For now, just remove it from the scheduler
         */
        if (CONN_F_CONN_REQ_TXD(connsm) == 1) {
            CONN_F_CONN_REQ_TXD(connsm) = 0;
            CONN_F_AUX_CONN_REQ(connsm) = 0;
            ble_ll_sched_rmv_elem(&connsm->conn_sch);
        }
        ble_phy_restart_rx();
        rc = 0;
    } else {
        ble_phy_rxpdu_copy(rxbuf, rxpdu);
        ble_ll_rx_pdu_in(rxpdu);
    }

    if (rc) {
        ble_ll_state_set(BLE_LL_STATE_STANDBY);
    }

    return rc;
}

/**
 * Function called when a timeout has occurred for a connection. There are
 * two types of timeouts: a connection supervision timeout and control
 * procedure timeout.
 *
 * Context: Link Layer task
 *
 * @param connsm
 * @param ble_err
 */
void
ble_ll_conn_timeout(struct ble_ll_conn_sm *connsm, uint8_t ble_err)
{
    int was_current;
    os_sr_t sr;

    was_current = 0;
    OS_ENTER_CRITICAL(sr);
    if (g_ble_ll_conn_cur_sm == connsm) {
        ble_ll_conn_current_sm_over(NULL);
        was_current = 1;
    }
    OS_EXIT_CRITICAL(sr);

    /* Check if we need to resume scanning */
    if (was_current) {
        ble_ll_scan_chk_resume();
    }

    ble_ll_conn_end(connsm, ble_err);
}

/**
 * Called when a data channel PDU has started that matches the access
 * address of the current connection. Note that the CRC of the PDU has not
 * been checked yet.
 *
 * Context: Interrupt
 *
 * @param rxhdr
 */
int
ble_ll_conn_rx_isr_start(struct ble_mbuf_hdr *rxhdr, uint32_t aa)
{
    struct ble_ll_conn_sm *connsm;

    /*
     * Disable wait for response timer since we receive a response. We dont
     * care if this is the response we were waiting for or not; the code
     * called at receive end will deal with ending the connection event
     * if needed
     */
    connsm = g_ble_ll_conn_cur_sm;
    if (connsm) {
        /* Double check access address. Better match connection state machine */
        if (aa != connsm->access_addr) {
            STATS_INC(ble_ll_conn_stats, rx_data_pdu_bad_aa);
            ble_ll_state_set(BLE_LL_STATE_STANDBY);
            ble_ll_event_send(&connsm->conn_ev_end);
            g_ble_ll_conn_cur_sm = NULL;
            return -1;
        }

        /* Set connection handle in mbuf header */
        rxhdr->rxinfo.handle = connsm->conn_handle;

        /* Set flag denoting we have received a packet in connection event */
        connsm->csmflags.cfbit.pkt_rxd = 1;

        /* Connection is established */
        connsm->conn_state = BLE_LL_CONN_STATE_ESTABLISHED;

        /* Set anchor point (and last) if 1st rxd frame in connection event */
        if (connsm->csmflags.cfbit.slave_set_last_anchor) {
            connsm->csmflags.cfbit.slave_set_last_anchor = 0;
            connsm->last_anchor_point = rxhdr->beg_cputime;
            connsm->anchor_point = connsm->last_anchor_point;
            connsm->anchor_point_usecs = rxhdr->rem_usecs;
        }
    }
    return 1;
}

/**
 * Called from the Link Layer task when a data PDU has been received
 *
 * Context: Link layer task
 *
 * @param rxpdu Pointer to received pdu
 * @param rxpdu Pointer to ble mbuf header of received pdu
 */
void
ble_ll_conn_rx_data_pdu(struct os_mbuf *rxpdu, struct ble_mbuf_hdr *hdr)
{
    uint8_t hdr_byte;
    uint8_t rxd_sn;
    uint8_t *rxbuf;
    uint8_t llid;
    uint16_t acl_len;
    uint16_t acl_hdr;
    struct ble_ll_conn_sm *connsm;

    if (BLE_MBUF_HDR_CRC_OK(hdr)) {
        /* XXX: there is a chance that the connection was thrown away and
           re-used before processing packets here. Fix this. */
        /* We better have a connection state machine */
        connsm = ble_ll_conn_find_active_conn(hdr->rxinfo.handle);
        if (connsm) {
            /* Check state machine */
            ble_ll_conn_chk_csm_flags(connsm);

            /* Validate rx data pdu */
            rxbuf = rxpdu->om_data;
            hdr_byte = rxbuf[0];
            acl_len = rxbuf[1];
            llid = hdr_byte & BLE_LL_DATA_HDR_LLID_MASK;

            /*
             * Check that the LLID and payload length are reasonable.
             * Empty payload is only allowed for LLID == 01b.
             *  */
            if ((llid == 0) ||
                ((acl_len == 0) && (llid != BLE_LL_LLID_DATA_FRAG))) {
                STATS_INC(ble_ll_conn_stats, rx_bad_llid);
                goto conn_rx_data_pdu_end;
            }

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
            /* Check if PDU is allowed when encryption is started. If not,
             * terminate connection.
             *
             * Reference: Core 5.0, Vol 6, Part B, 5.1.3.1
             */
            if ((connsm->enc_data.enc_state > CONN_ENC_S_PAUSE_ENC_RSP_WAIT) &&
                    !ble_ll_ctrl_enc_allowed_pdu_rx(rxpdu)) {
                ble_ll_conn_timeout(connsm, BLE_ERR_CONN_TERM_MIC);
                goto conn_rx_data_pdu_end;
            }
#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_PING)
            /*
             * Reset authenticated payload timeout if valid MIC. NOTE: we dont
             * check the MIC failure bit as that would have terminated the
             * connection
             */
            if ((connsm->enc_data.enc_state == CONN_ENC_S_ENCRYPTED) &&
                CONN_F_LE_PING_SUPP(connsm) && (acl_len != 0)) {
                ble_ll_conn_auth_pyld_timer_start(connsm);
            }
#endif

            /* Update RSSI */
            connsm->conn_rssi = hdr->rxinfo.rssi;

            /*
             * If we are a slave, we can only start to use slave latency
             * once we have received a NESN of 1 from the master
             */
            if (connsm->conn_role == BLE_LL_CONN_ROLE_SLAVE) {
                if (hdr_byte & BLE_LL_DATA_HDR_NESN_MASK) {
                    connsm->csmflags.cfbit.allow_slave_latency = 1;
                }
            }

            /*
             * Discard the received PDU if the sequence number is the same
             * as the last received sequence number
             */
            rxd_sn = hdr_byte & BLE_LL_DATA_HDR_SN_MASK;
            if (rxd_sn != connsm->last_rxd_sn) {
                /* Update last rxd sn */
                connsm->last_rxd_sn = rxd_sn;

                /* No need to do anything if empty pdu */
                if ((llid == BLE_LL_LLID_DATA_FRAG) && (acl_len == 0)) {
                    goto conn_rx_data_pdu_end;
                }

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
                /*
                 * XXX: should we check to see if we are in a state where we
                 * might expect to get an encrypted PDU?
                 */
                if (BLE_MBUF_HDR_MIC_FAILURE(hdr)) {
                    STATS_INC(ble_ll_conn_stats, mic_failures);
                    ble_ll_conn_timeout(connsm, BLE_ERR_CONN_TERM_MIC);
                    goto conn_rx_data_pdu_end;
                }
#endif

                if (llid == BLE_LL_LLID_CTRL) {
                    /* Process control frame */
                    STATS_INC(ble_ll_conn_stats, rx_ctrl_pdus);
                    if (ble_ll_ctrl_rx_pdu(connsm, rxpdu)) {
                        STATS_INC(ble_ll_conn_stats, rx_malformed_ctrl_pdus);
                    }
                } else {
                    /* Count # of received l2cap frames and byes */
                    STATS_INC(ble_ll_conn_stats, rx_l2cap_pdus);
                    STATS_INCN(ble_ll_conn_stats, rx_l2cap_bytes, acl_len);

                    /* NOTE: there should be at least two bytes available */
                    BLE_LL_ASSERT(OS_MBUF_LEADINGSPACE(rxpdu) >= 2);
                    os_mbuf_prepend(rxpdu, 2);
                    rxbuf = rxpdu->om_data;

                    acl_hdr = (llid << 12) | connsm->conn_handle;
                    put_le16(rxbuf, acl_hdr);
                    put_le16(rxbuf + 2, acl_len);
                    ble_hci_trans_ll_acl_tx(rxpdu);
                }

                /* NOTE: we dont free the mbuf since we handed it off! */
                return;
            } else {
                STATS_INC(ble_ll_conn_stats, data_pdu_rx_dup);
            }
        } else {
            STATS_INC(ble_ll_conn_stats, no_conn_sm);
        }
    }

    /* Free buffer */
conn_rx_data_pdu_end:
    os_mbuf_free_chain(rxpdu);
}

/**
 * Called when a packet has been received while in the connection state.
 *
 * Context: Interrupt
 *
 * @param rxpdu
 * @param crcok
 *
 * @return int
 *       < 0: Disable the phy after reception.
 *      == 0: Success. Do not disable the PHY.
 *       > 0: Do not disable PHY as that has already been done.
 */
int
ble_ll_conn_rx_isr_end(uint8_t *rxbuf, struct ble_mbuf_hdr *rxhdr)
{
    int rc;
    int is_ctrl;
    uint8_t hdr_byte;
    uint8_t hdr_sn;
    uint8_t hdr_nesn;
    uint8_t conn_sn;
    uint8_t conn_nesn;
    uint8_t reply;
    uint8_t rem_bytes;
    uint8_t opcode = 0;
    uint8_t rx_pyld_len;
    uint32_t begtime;
    uint32_t add_usecs;
    struct os_mbuf *txpdu;
    struct ble_ll_conn_sm *connsm;
    struct os_mbuf *rxpdu;
    struct ble_mbuf_hdr *txhdr;
    int rx_phy_mode;

    /* Retrieve the header and payload length */
    hdr_byte = rxbuf[0];
    rx_pyld_len = rxbuf[1];

    /*
     * We need to attempt to allocate a buffer here. The reason we do this
     * now is that we should not ack the packet if we have no receive
     * buffers available. We want to free up our transmit PDU if it was
     * acked, but we should not ack the received frame if we cant hand it up.
     * NOTE: we hand up empty pdu's to the LL task!
     */
    rxpdu = ble_ll_rxpdu_alloc(rx_pyld_len + BLE_LL_PDU_HDR_LEN);

    /*
     * We should have a current connection state machine. If we dont, we just
     * hand the packet to the higher layer to count it.
     */
    rc = -1;
    connsm = g_ble_ll_conn_cur_sm;
    if (!connsm) {
        STATS_INC(ble_ll_conn_stats, rx_data_pdu_no_conn);
        goto conn_exit;
    }

    /*
     * Calculate the end time of the received PDU. NOTE: this looks strange
     * but for the 32768 crystal we add the time it takes to send the packet
     * to the 'additional usecs' field to save some calculations.
     */
    begtime = rxhdr->beg_cputime;
#if BLE_LL_BT5_PHY_SUPPORTED
    rx_phy_mode = connsm->phy_data.rx_phy_mode;
#else
    rx_phy_mode = BLE_PHY_MODE_1M;
#endif
    add_usecs = rxhdr->rem_usecs +
            ble_ll_pdu_tx_time_get(rx_pyld_len, rx_phy_mode);

    /*
     * Check the packet CRC. A connection event can continue even if the
     * received PDU does not pass the CRC check. If we receive two consecutive
     * CRC errors we end the conection event.
     */
    if (!BLE_MBUF_HDR_CRC_OK(rxhdr)) {
        /*
         * Increment # of consecutively received CRC errors. If more than
         * one we will end the connection event.
         */
        ++connsm->cons_rxd_bad_crc;
        if (connsm->cons_rxd_bad_crc >= 2) {
            reply = 0;
        } else {
            if (connsm->conn_role == BLE_LL_CONN_ROLE_MASTER) {
                reply = CONN_F_LAST_TXD_MD(connsm);
            } else {
                /* A slave always responds with a packet */
                reply = 1;
            }
        }
    } else {
        /* Reset consecutively received bad crcs (since this one was good!) */
        connsm->cons_rxd_bad_crc = 0;

        /* Set last valid received pdu time (resets supervision timer) */
        connsm->last_rxd_pdu_cputime = begtime +
                                        os_cputime_usecs_to_ticks(add_usecs);

        /*
         * Check for valid LLID before proceeding. We have seen some weird
         * things with the PHY where the CRC is OK but we dont have a valid
         * LLID. This should really never happen but if it does we will just
         * bail. An error stat will get incremented at the LL.
         */
        if ((hdr_byte & BLE_LL_DATA_HDR_LLID_MASK) == 0) {
            goto conn_exit;
        }

        /* Set last received header byte */
        connsm->last_rxd_hdr_byte = hdr_byte;

        is_ctrl = 0;
        if ((hdr_byte & BLE_LL_DATA_HDR_LLID_MASK) == BLE_LL_LLID_CTRL) {
            is_ctrl = 1;
            opcode = rxbuf[2];
        }

        /*
         * If SN bit from header does not match NESN in connection, this is
         * a resent PDU and should be ignored.
         */
        hdr_sn = hdr_byte & BLE_LL_DATA_HDR_SN_MASK;
        conn_nesn = connsm->next_exp_seqnum;
        if (rxpdu && ((hdr_sn && conn_nesn) || (!hdr_sn && !conn_nesn))) {
            connsm->next_exp_seqnum ^= 1;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
            if (CONN_F_ENCRYPTED(connsm) && !ble_ll_conn_is_empty_pdu(rxbuf)) {
                ++connsm->enc_data.rx_pkt_cntr;
            }
#endif
        }

        ble_ll_trace_u32x2(BLE_LL_TRACE_ID_CONN_RX, connsm->tx_seqnum,
                           !!(hdr_byte & BLE_LL_DATA_HDR_NESN_MASK));

        /*
         * Check NESN bit from header. If same as tx seq num, the transmission
         * is acknowledged. Otherwise we need to resend this PDU.
         */
        if (CONN_F_EMPTY_PDU_TXD(connsm) || connsm->cur_tx_pdu) {
            hdr_nesn = hdr_byte & BLE_LL_DATA_HDR_NESN_MASK;
            conn_sn = connsm->tx_seqnum;
            if ((hdr_nesn && conn_sn) || (!hdr_nesn && !conn_sn)) {
                /* We did not get an ACK. Must retry the PDU */
                STATS_INC(ble_ll_conn_stats, data_pdu_txf);
            } else {
                /* Transmit success */
                connsm->tx_seqnum ^= 1;
                STATS_INC(ble_ll_conn_stats, data_pdu_txg);

                /* If we transmitted the empty pdu, clear flag */
                if (CONN_F_EMPTY_PDU_TXD(connsm)) {
                    CONN_F_EMPTY_PDU_TXD(connsm) = 0;
                    goto chk_rx_terminate_ind;
                }

                /*
                 * Determine if we should remove packet from queue or if there
                 * are more fragments to send.
                 */
                txpdu = connsm->cur_tx_pdu;
                if (txpdu) {
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
                    if (connsm->enc_data.tx_encrypted) {
                        ++connsm->enc_data.tx_pkt_cntr;
                    }
#endif
                    txhdr = BLE_MBUF_HDR_PTR(txpdu);
                    if ((txhdr->txinfo.hdr_byte & BLE_LL_DATA_HDR_LLID_MASK)
                        == BLE_LL_LLID_CTRL) {
                        connsm->cur_tx_pdu = NULL;
                        /* Note: the mbuf is freed by this call */
                        rc = ble_ll_ctrl_tx_done(txpdu, connsm);
                        if (rc) {
                            /* Means we transmitted a TERMINATE_IND */
                            goto conn_exit;
                        } else {
                            goto chk_rx_terminate_ind;
                        }
                    }

                    /* Increment offset based on number of bytes sent */
                    txhdr->txinfo.offset += txhdr->txinfo.pyld_len;
                    if (txhdr->txinfo.offset >= OS_MBUF_PKTLEN(txpdu)) {
                        /* If l2cap pdu, increment # of completed packets */
                        if (txhdr->txinfo.pyld_len != 0) {
#if (BLETEST_THROUGHPUT_TEST == 1)
                            bletest_completed_pkt(connsm->conn_handle);
#endif
                            ++connsm->completed_pkts;
                            if (connsm->completed_pkts > 2) {
                                ble_npl_eventq_put(&g_ble_ll_data.ll_evq,
                                                   &g_ble_ll_data.ll_comp_pkt_ev);
                            }
                        }
                        os_mbuf_free_chain(txpdu);
                        connsm->cur_tx_pdu = NULL;
                    } else {
                        rem_bytes = OS_MBUF_PKTLEN(txpdu) - txhdr->txinfo.offset;
                        /* Adjust payload for max TX time and octets */

#if (BLE_LL_BT5_PHY_SUPPORTED == 1)
                        if (is_ctrl &&
                            (connsm->conn_role == BLE_LL_CONN_ROLE_SLAVE) &&
                            (opcode == BLE_LL_CTRL_PHY_UPDATE_IND)) {
                            connsm->phy_tx_transition =
                                    ble_ll_ctrl_phy_tx_transition_get(rxbuf[3]);
                        }
#endif

                        rem_bytes = ble_ll_conn_adjust_pyld_len(connsm, rem_bytes);
                        txhdr->txinfo.pyld_len = rem_bytes;
                    }
                }
            }
        }

        /* Should we continue connection event? */
        /* If this is a TERMINATE_IND, we have to reply */
chk_rx_terminate_ind:
        /* If we received a terminate IND, we must set some flags */
        if (is_ctrl && (opcode == BLE_LL_CTRL_TERMINATE_IND)
                    && (rx_pyld_len == (1 + BLE_LL_CTRL_TERMINATE_IND_LEN))) {
            connsm->csmflags.cfbit.terminate_ind_rxd = 1;
            connsm->rxd_disconnect_reason = rxbuf[3];
        }

        if (connsm->conn_role == BLE_LL_CONN_ROLE_MASTER) {
            reply = CONN_F_LAST_TXD_MD(connsm) || (hdr_byte & BLE_LL_DATA_HDR_MD_MASK);
        } else {
            /* A slave always replies */
            reply = 1;
        }
    }

    /* If reply flag set, send data pdu and continue connection event */
    rc = -1;
    if (rx_pyld_len && CONN_F_ENCRYPTED(connsm)) {
        rx_pyld_len += BLE_LL_DATA_MIC_LEN;
    }
    if (reply && ble_ll_conn_can_send_next_pdu(connsm, begtime, add_usecs)) {
        rc = ble_ll_conn_tx_pdu(connsm);
    }

conn_exit:
    /* Copy the received pdu and hand it up */
    if (rxpdu) {
        ble_phy_rxpdu_copy(rxbuf, rxpdu);
        ble_ll_rx_pdu_in(rxpdu);
    }

    /* Send link layer a connection end event if over */
    if (rc) {
        ble_ll_conn_current_sm_over(connsm);
    }

    return rc;
}

/**
 * Called to adjust payload length to fit into max effective octets and TX time
 * on current PHY.
 */
/**
 * Called to enqueue a packet on the transmit queue of a connection. Should
 * only be called by the controller.
 *
 * Context: Link Layer
 *
 *
 * @param connsm
 * @param om
 */
void
ble_ll_conn_enqueue_pkt(struct ble_ll_conn_sm *connsm, struct os_mbuf *om,
                        uint8_t hdr_byte, uint8_t length)
{
    os_sr_t sr;
    struct os_mbuf_pkthdr *pkthdr;
    struct ble_mbuf_hdr *ble_hdr;
    int lifo;

    /* Set mbuf length and packet length if a control PDU */
    if (hdr_byte == BLE_LL_LLID_CTRL) {
        om->om_len = length;
        OS_MBUF_PKTHDR(om)->omp_len = length;
    }

    /* Set BLE transmit header */
    ble_hdr = BLE_MBUF_HDR_PTR(om);
    ble_hdr->txinfo.flags = 0;
    ble_hdr->txinfo.offset = 0;
    ble_hdr->txinfo.hdr_byte = hdr_byte;

    /*
     * Initial payload length is calculate when packet is dequeued, there's no
     * need to do this now.
     */

    lifo = 0;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
    if (connsm->enc_data.enc_state > CONN_ENC_S_ENCRYPTED) {
        uint8_t llid;

        /*
         * If this is one of the following types we need to insert it at
         * head of queue.
         */
        llid = ble_hdr->txinfo.hdr_byte & BLE_LL_DATA_HDR_LLID_MASK;
        if (llid == BLE_LL_LLID_CTRL) {
            switch (om->om_data[0]) {
            case BLE_LL_CTRL_TERMINATE_IND:
            case BLE_LL_CTRL_REJECT_IND:
            case BLE_LL_CTRL_REJECT_IND_EXT:
            case BLE_LL_CTRL_START_ENC_REQ:
            case BLE_LL_CTRL_START_ENC_RSP:
                lifo = 1;
                break;
            case BLE_LL_CTRL_PAUSE_ENC_RSP:
                if (connsm->conn_role == BLE_LL_CONN_ROLE_MASTER) {
                    lifo = 1;
                }
                break;
            case BLE_LL_CTRL_ENC_REQ:
            case BLE_LL_CTRL_ENC_RSP:
                /* If encryption has been paused, we don't want to send any packets from the
                 * TX queue, as they would go unencrypted.
                 */
                if (connsm->enc_data.enc_state == CONN_ENC_S_PAUSED) {
                    lifo = 1;
                }
                break;
            default:
                break;
            }
        }
    }
#endif

    /* Add to transmit queue for the connection */
    pkthdr = OS_MBUF_PKTHDR(om);
    OS_ENTER_CRITICAL(sr);
    if (lifo) {
        STAILQ_INSERT_HEAD(&connsm->conn_txq, pkthdr, omp_next);
    } else {
        STAILQ_INSERT_TAIL(&connsm->conn_txq, pkthdr, omp_next);
    }
    OS_EXIT_CRITICAL(sr);
}

/**
 * Data packet from host.
 *
 * Context: Link Layer task
 *
 * @param om
 * @param handle
 * @param length
 *
 * @return int
 */
void
ble_ll_conn_tx_pkt_in(struct os_mbuf *om, uint16_t handle, uint16_t length)
{
    uint8_t hdr_byte;
    uint16_t conn_handle;
    uint16_t pb;
    struct ble_ll_conn_sm *connsm;

    /* See if we have an active matching connection handle */
    conn_handle = handle & 0x0FFF;
    connsm = ble_ll_conn_find_active_conn(conn_handle);
    if (connsm) {
        /* Construct LL header in buffer (NOTE: pb already checked) */
        pb = handle & 0x3000;
        if (pb == 0) {
            hdr_byte = BLE_LL_LLID_DATA_START;
        } else {
            hdr_byte = BLE_LL_LLID_DATA_FRAG;
        }

        /* Add to total l2cap pdus enqueue */
        STATS_INC(ble_ll_conn_stats, l2cap_enqueued);

        /* Clear flags field in BLE header */
        ble_ll_conn_enqueue_pkt(connsm, om, hdr_byte, length);
    } else {
        /* No connection found! */
        STATS_INC(ble_ll_conn_stats, handle_not_found);
        os_mbuf_free_chain(om);
    }
}

/**
 * Called to set the global channel mask that we use for all connections.
 *
 * @param num_used_chans
 * @param chanmap
 */
void
ble_ll_conn_set_global_chanmap(uint8_t num_used_chans, const uint8_t *chanmap)
{
    struct ble_ll_conn_sm *connsm;
    struct ble_ll_conn_global_params *conn_params;

    /* Do nothing if same channel map */
    conn_params = &g_ble_ll_conn_params;
    if (!memcmp(conn_params->master_chan_map, chanmap, BLE_LL_CONN_CHMAP_LEN)) {
        return;
    }

    /* Change channel map and cause channel map update procedure to start */
    conn_params->num_used_chans = num_used_chans;
    memcpy(conn_params->master_chan_map, chanmap, BLE_LL_CONN_CHMAP_LEN);

    /* Perform channel map update */
    SLIST_FOREACH(connsm, &g_ble_ll_conn_active_list, act_sle) {
        if (connsm->conn_role == BLE_LL_CONN_ROLE_MASTER) {
            ble_ll_ctrl_proc_start(connsm, BLE_LL_CTRL_PROC_CHAN_MAP_UPD);
        }
    }
}

/**
 * Called when a device has received a connect request while advertising and
 * the connect request has passed the advertising filter policy and is for
 * us. This will start a connection in the slave role assuming that we dont
 * already have a connection with this device and that the connect request
 * parameters are valid.
 *
 * Context: Link Layer
 *
 * @param rxbuf Pointer to received Connect Request PDU
 *
 * @return 0: connection not started; 1 connecton started
 */
int
ble_ll_conn_slave_start(uint8_t *rxbuf, uint8_t pat, struct ble_mbuf_hdr *rxhdr,
                        bool force_csa2)
{
    int rc;
    uint32_t temp;
    uint32_t crcinit;
    uint8_t *inita;
    uint8_t *dptr;
    struct ble_ll_conn_sm *connsm;

    /* Ignore the connection request if we are already connected*/
    inita = rxbuf + BLE_LL_PDU_HDR_LEN;
    SLIST_FOREACH(connsm, &g_ble_ll_conn_active_list, act_sle) {
        if (!memcmp(&connsm->peer_addr, inita, BLE_DEV_ADDR_LEN)) {
            if (rxbuf[0] & BLE_ADV_PDU_HDR_TXADD_MASK) {
                if (connsm->peer_addr_type & 1) {
                    return 0;
                }
            } else {
                if ((connsm->peer_addr_type & 1) == 0) {
                    return 0;
                }
            }
        }
    }

    /* Allocate a connection. If none available, dont do anything */
    connsm = ble_ll_conn_sm_get();
    if (connsm == NULL) {
        return 0;
    }

    /* Set the pointer at the start of the connection data */
    dptr = rxbuf + BLE_LL_CONN_REQ_ADVA_OFF + BLE_DEV_ADDR_LEN;

    /* Set connection state machine information */
    connsm->access_addr = get_le32(dptr);
    crcinit = dptr[6];
    crcinit = (crcinit << 8) | dptr[5];
    crcinit = (crcinit << 8) | dptr[4];
    connsm->crcinit = crcinit;
    connsm->tx_win_size = dptr[7];
    connsm->tx_win_off = get_le16(dptr + 8);
    connsm->conn_itvl = get_le16(dptr + 10);
    connsm->slave_latency = get_le16(dptr + 12);
    connsm->supervision_tmo = get_le16(dptr + 14);
    memcpy(&connsm->chanmap, dptr + 16, BLE_LL_CONN_CHMAP_LEN);
    connsm->hop_inc = dptr[21] & 0x1F;
    connsm->master_sca = dptr[21] >> 5;

    /* Error check parameters */
    if ((connsm->tx_win_off > connsm->conn_itvl) ||
        (connsm->conn_itvl < BLE_HCI_CONN_ITVL_MIN) ||
        (connsm->conn_itvl > BLE_HCI_CONN_ITVL_MAX) ||
        (connsm->tx_win_size < BLE_LL_CONN_TX_WIN_MIN) ||
        (connsm->slave_latency > BLE_LL_CONN_SLAVE_LATENCY_MAX)) {
        goto err_slave_start;
    }

    /* Slave latency cannot cause a supervision timeout */
    temp = (connsm->slave_latency + 1) * (connsm->conn_itvl * 2) *
            BLE_LL_CONN_ITVL_USECS;
    if ((connsm->supervision_tmo * 10000) <= temp ) {
        goto err_slave_start;
    }

    /*
     * The transmit window must be less than or equal to the lesser of 10
     * msecs or the connection interval minus 1.25 msecs.
     */
    temp = connsm->conn_itvl - 1;
    if (temp > 8) {
        temp = 8;
    }
    if (connsm->tx_win_size > temp) {
        goto err_slave_start;
    }

    /* Set the address of device that we are connecting with */
    memcpy(&connsm->peer_addr, inita, BLE_DEV_ADDR_LEN);
    connsm->peer_addr_type = pat;

    /* Calculate number of used channels; make sure it meets min requirement */
    connsm->num_used_chans = ble_ll_utils_calc_num_used_chans(connsm->chanmap);
    if (connsm->num_used_chans < 2) {
        goto err_slave_start;
    }

    /* Start the connection state machine */
    connsm->conn_role = BLE_LL_CONN_ROLE_SLAVE;
    ble_ll_conn_sm_new(connsm);

#if (BLE_LL_BT5_PHY_SUPPORTED == 1)
    /* Use the same PHY as we received CONNECT_REQ on */
    ble_ll_conn_init_phy(connsm, rxhdr->rxinfo.phy);
#endif

    ble_ll_conn_set_csa(connsm,
                        force_csa2 || (rxbuf[0] & BLE_ADV_PDU_HDR_CHSEL_MASK));

    /* Set initial schedule callback */
    connsm->conn_sch.sched_cb = ble_ll_conn_event_start_cb;
    rc = ble_ll_conn_created(connsm, rxhdr);
    if (!rc) {
        SLIST_REMOVE(&g_ble_ll_conn_active_list, connsm, ble_ll_conn_sm, act_sle);
        STAILQ_INSERT_TAIL(&g_ble_ll_conn_free_list, connsm, free_stqe);
    }
    return rc;

err_slave_start:
    STAILQ_INSERT_TAIL(&g_ble_ll_conn_free_list, connsm, free_stqe);
    STATS_INC(ble_ll_conn_stats, slave_rxd_bad_conn_req_params);
    return 0;
}

#define MAX_TIME_UNCODED(_maxbytes) \
        ble_ll_pdu_tx_time_get(_maxbytes + BLE_LL_DATA_MIC_LEN, \
                               BLE_PHY_MODE_1M);
#define MAX_TIME_CODED(_maxbytes) \
        ble_ll_pdu_tx_time_get(_maxbytes + BLE_LL_DATA_MIC_LEN, \
                               BLE_PHY_MODE_CODED_125KBPS);

/**
 * Called to reset the connection module. When this function is called the
 * scheduler has been stopped and the phy has been disabled. The LL should
 * be in the standby state.
 *
 * Context: Link Layer task
 */
void
ble_ll_conn_module_reset(void)
{
    uint8_t max_phy_pyld;
    uint16_t maxbytes;
    struct ble_ll_conn_sm *connsm;
    struct ble_ll_conn_global_params *conn_params;

    /* Kill the current one first (if one is running) */
    if (g_ble_ll_conn_cur_sm) {
        connsm = g_ble_ll_conn_cur_sm;
        g_ble_ll_conn_cur_sm = NULL;
        ble_ll_conn_end(connsm, BLE_ERR_SUCCESS);
    }

    /* Free the global connection complete event if there is one */
    if (g_ble_ll_conn_comp_ev) {
        ble_hci_trans_buf_free(g_ble_ll_conn_comp_ev);
        g_ble_ll_conn_comp_ev = NULL;
    }

    /* Reset connection we are attempting to create */
    g_ble_ll_conn_create_sm = NULL;

    /* Now go through and end all the connections */
    while (1) {
        connsm = SLIST_FIRST(&g_ble_ll_conn_active_list);
        if (!connsm) {
            break;
        }
        ble_ll_conn_end(connsm, BLE_ERR_SUCCESS);
    }

    /* Get the maximum supported PHY PDU size from the PHY */
    max_phy_pyld = ble_phy_max_data_pdu_pyld();

    /* Configure the global LL parameters */
    conn_params = &g_ble_ll_conn_params;

    maxbytes = min(MYNEWT_VAL(BLE_LL_SUPP_MAX_RX_BYTES), max_phy_pyld);
    conn_params->supp_max_rx_octets = maxbytes;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CODED_PHY)
    conn_params->supp_max_rx_time = MAX_TIME_CODED(maxbytes);
#else
    conn_params->supp_max_rx_time = MAX_TIME_UNCODED(maxbytes);
#endif

    maxbytes = min(MYNEWT_VAL(BLE_LL_SUPP_MAX_TX_BYTES), max_phy_pyld);
    conn_params->supp_max_tx_octets = maxbytes;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CODED_PHY)
    conn_params->supp_max_tx_time = MAX_TIME_CODED(maxbytes);
#else
    conn_params->supp_max_tx_time = MAX_TIME_UNCODED(maxbytes);
#endif

    maxbytes = min(MYNEWT_VAL(BLE_LL_CONN_INIT_MAX_TX_BYTES), max_phy_pyld);
    conn_params->conn_init_max_tx_octets = maxbytes;
    conn_params->conn_init_max_tx_time = MAX_TIME_UNCODED(maxbytes);
    conn_params->conn_init_max_tx_time_uncoded = MAX_TIME_UNCODED(maxbytes);
    conn_params->conn_init_max_tx_time_coded = MAX_TIME_CODED(maxbytes);

    conn_params->sugg_tx_octets = BLE_LL_CONN_SUPP_BYTES_MIN;
    conn_params->sugg_tx_time = BLE_LL_CONN_SUPP_TIME_MIN;

    /* Mask in all channels by default */
    conn_params->num_used_chans = BLE_PHY_NUM_DATA_CHANS;
    memset(conn_params->master_chan_map, 0xff, BLE_LL_CONN_CHMAP_LEN - 1);
    conn_params->master_chan_map[4] = 0x1f;

    /* Reset statistics */
    STATS_RESET(ble_ll_conn_stats);

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV_SYNC_TRANSFER)
    /* reset default sync transfer params */
    g_ble_ll_conn_sync_transfer_params.max_skip = 0;
    g_ble_ll_conn_sync_transfer_params.mode = 0;
    g_ble_ll_conn_sync_transfer_params.sync_timeout_us = 0;
#endif
}

/* Initialize the connection module */
void
ble_ll_conn_module_init(void)
{
    int rc;
    uint16_t i;
    struct ble_ll_conn_sm *connsm;

    /* Initialize list of active conections */
    SLIST_INIT(&g_ble_ll_conn_active_list);
    STAILQ_INIT(&g_ble_ll_conn_free_list);

    /*
     * Take all the connections off the free memory pool and add them to
     * the free connection list, assigning handles in linear order. Note:
     * the specification allows a handle of zero; we just avoid using it.
     */
    connsm = &g_ble_ll_conn_sm[0];
    for (i = 0; i < MYNEWT_VAL(BLE_MAX_CONNECTIONS); ++i) {

        memset(connsm, 0, sizeof(struct ble_ll_conn_sm));
        connsm->conn_handle = i + 1;
        STAILQ_INSERT_TAIL(&g_ble_ll_conn_free_list, connsm, free_stqe);

        /* Initialize fixed schedule elements */
        connsm->conn_sch.sched_type = BLE_LL_SCHED_TYPE_CONN;
        connsm->conn_sch.cb_arg = connsm;
        ++connsm;
    }

    /* Register connection statistics */
    rc = stats_init_and_reg(STATS_HDR(ble_ll_conn_stats),
                            STATS_SIZE_INIT_PARMS(ble_ll_conn_stats, STATS_SIZE_32),
                            STATS_NAME_INIT_PARMS(ble_ll_conn_stats),
                            "ble_ll_conn");
    BLE_LL_ASSERT(rc == 0);

    /* Call reset to finish reset of initialization */
    ble_ll_conn_module_reset();
}

#endif
