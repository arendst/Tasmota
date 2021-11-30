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
#include "nimble/porting/nimble/include/os/os.h"
#include "nimble/porting/nimble/include/os/os_cputime.h"

#if defined(ARDUINO_ARCH_NRF5) && defined(NRF51)
#include "nimble/nimble/drivers/nrf51/include/ble/xcvr.h"
#elif defined(ARDUINO_ARCH_NRF5) && defined(NRF52_SERIES)
#include "nimble/nimble/drivers/nrf52/include/ble/xcvr.h"
#endif

#include "../include/controller/ble_phy.h"
#include "../include/controller/ble_ll.h"
#include "../include/controller/ble_ll_sched.h"
#include "../include/controller/ble_ll_adv.h"
#include "../include/controller/ble_ll_scan.h"
#include "../include/controller/ble_ll_rfmgmt.h"
#include "../include/controller/ble_ll_trace.h"
#include "../include/controller/ble_ll_sync.h"
#include "ble_ll_priv.h"
#include "ble_ll_conn_priv.h"

/* XXX: this is temporary. Not sure what I want to do here */
struct hal_timer g_ble_ll_sched_timer;

uint8_t g_ble_ll_sched_offset_ticks;

#define BLE_LL_SCHED_ADV_WORST_CASE_USECS       \
    (BLE_LL_SCHED_MAX_ADV_PDU_USECS + BLE_LL_IFS + BLE_LL_SCHED_ADV_MAX_USECS \
     + XCVR_TX_SCHED_DELAY_USECS)

#if (BLE_LL_SCHED_DEBUG == 1)
int32_t g_ble_ll_sched_max_late;
int32_t g_ble_ll_sched_max_early;
#endif

/* XXX: TODO:
 *  1) Add some accounting to the schedule code to see how late we are
 *  (min/max?)
 *
 *  2) Need to determine how we really want to handle the case when we execute
 *  a schedule item but there is a current event. We could:
 *      -> Reschedule the schedule item and let current event finish
 *      -> Kill the current event and run the scheduled item.
 *      -> Disable schedule timer while in an event; could cause us to be late.
 *      -> Wait for current event to finish hoping it does before schedule item.
 */

/* Queue for timers */
TAILQ_HEAD(ll_sched_qhead, ble_ll_sched_item) g_ble_ll_sched_q;

#if MYNEWT_VAL(BLE_LL_STRICT_CONN_SCHEDULING)
struct ble_ll_sched_obj g_ble_ll_sched_data;
#endif

/**
 * Checks if two events in the schedule will overlap in time. NOTE: consecutive
 * schedule items can end and start at the same time.
 *
 * @param s1
 * @param s2
 *
 * @return int 0: dont overlap 1:overlap
 */
static int
ble_ll_sched_is_overlap(struct ble_ll_sched_item *s1,
                        struct ble_ll_sched_item *s2)
{
    int rc;

    rc = 1;
    if ((int32_t)(s1->start_time - s2->start_time) < 0) {
        /* Make sure this event does not overlap current event */
        if ((int32_t)(s1->end_time - s2->start_time) <= 0) {
            rc = 0;
        }
    } else {
        /* Check for overlap */
        if ((int32_t)(s1->start_time - s2->end_time) >= 0) {
            rc = 0;
        }
    }

    return rc;
}

/*
 * Determines if the schedule item overlaps the currently running schedule
 * item. We only care about connection schedule items
 */
static int
ble_ll_sched_overlaps_current(struct ble_ll_sched_item *sch)
{
    int rc;
    uint32_t ce_end_time;

    rc = 0;
    if (ble_ll_state_get() == BLE_LL_STATE_CONNECTION) {
        ce_end_time = ble_ll_conn_get_ce_end_time();
        if ((int32_t)(ce_end_time - sch->start_time) > 0) {
            rc = 1;
        }
    }
    return rc;
}

static int
ble_ll_sched_conn_overlap(struct ble_ll_sched_item *entry)
{
    int rc;
    struct ble_ll_conn_sm *connsm;

    /* Should only be advertising or a connection here */
    if (entry->sched_type == BLE_LL_SCHED_TYPE_CONN) {
        connsm = (struct ble_ll_conn_sm *)entry->cb_arg;
        entry->enqueued = 0;
        TAILQ_REMOVE(&g_ble_ll_sched_q, entry, link);
        ble_ll_event_send(&connsm->conn_ev_end);
        rc = 0;
    } else {
        rc = -1;
    }

    return rc;
}

static struct ble_ll_sched_item *
ble_ll_sched_insert_if_empty(struct ble_ll_sched_item *sch)
{
    struct ble_ll_sched_item *entry;

    entry = TAILQ_FIRST(&g_ble_ll_sched_q);
    if (!entry) {
        TAILQ_INSERT_HEAD(&g_ble_ll_sched_q, sch, link);
        sch->enqueued = 1;
    }
    return entry;
}

int
ble_ll_sched_conn_reschedule(struct ble_ll_conn_sm *connsm)
{
    int rc;
    os_sr_t sr;
    uint32_t usecs;
    struct ble_ll_sched_item *sch;
    struct ble_ll_sched_item *start_overlap;
    struct ble_ll_sched_item *end_overlap;
    struct ble_ll_sched_item *entry;
    struct ble_ll_conn_sm *tmp;

    /* Get schedule element from connection */
    sch = &connsm->conn_sch;

    /* Set schedule start and end times */
    sch->start_time = connsm->anchor_point - g_ble_ll_sched_offset_ticks;
    if (connsm->conn_role == BLE_LL_CONN_ROLE_SLAVE) {
        usecs = connsm->slave_cur_window_widening;
        sch->start_time -= (os_cputime_usecs_to_ticks(usecs) + 1);
        sch->remainder = 0;
    } else {
        sch->remainder = connsm->anchor_point_usecs;
    }
    sch->end_time = connsm->ce_end_time;

    /* Better be past current time or we just leave */
    if ((int32_t)(sch->start_time - os_cputime_get32()) < 0) {
        return -1;
    }

    /* We have to find a place for this schedule */
    OS_ENTER_CRITICAL(sr);

    if (ble_ll_sched_overlaps_current(sch)) {
        OS_EXIT_CRITICAL(sr);
        return -1;
    }

    /* Stop timer since we will add an element */
    os_cputime_timer_stop(&g_ble_ll_sched_timer);

    start_overlap = NULL;
    end_overlap = NULL;
    rc = 0;
    TAILQ_FOREACH(entry, &g_ble_ll_sched_q, link) {
        if (ble_ll_sched_is_overlap(sch, entry)) {
           if (entry->sched_type == BLE_LL_SCHED_TYPE_CONN &&
                            !ble_ll_conn_is_lru((struct ble_ll_conn_sm *)sch->cb_arg,
                                    (struct ble_ll_conn_sm *)entry->cb_arg)) {
                /* Only insert if this element is older than all that we
                 * overlap
                 */
                start_overlap = NULL;
                rc = -1;
                break;
            }

            if (start_overlap == NULL) {
                start_overlap = entry;
                end_overlap = entry;
            } else {
                end_overlap = entry;
            }
        } else {
            if ((int32_t)(sch->end_time - entry->start_time) <= 0) {
                rc = 0;
                TAILQ_INSERT_BEFORE(entry, sch, link);
                break;
            }
        }
    }

    if (!rc) {
        if (!entry) {
            TAILQ_INSERT_TAIL(&g_ble_ll_sched_q, sch, link);
        }
        sch->enqueued = 1;
    }

    /* Remove first to last scheduled elements */
    entry = start_overlap;
    while (entry) {
        start_overlap = TAILQ_NEXT(entry,link);
        switch (entry->sched_type) {
        case BLE_LL_SCHED_TYPE_CONN:
            tmp = (struct ble_ll_conn_sm *)entry->cb_arg;
            ble_ll_event_send(&tmp->conn_ev_end);
            break;
        case BLE_LL_SCHED_TYPE_ADV:
            ble_ll_adv_event_rmvd_from_sched((struct ble_ll_adv_sm *)entry->cb_arg);
            break;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
        case BLE_LL_SCHED_TYPE_AUX_SCAN:
            ble_ll_scan_end_adv_evt((struct ble_ll_aux_data *)entry->cb_arg);
            break;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV)
        case BLE_LL_SCHED_TYPE_PERIODIC:
            ble_ll_adv_periodic_rmvd_from_sched((struct ble_ll_adv_sm *)entry->cb_arg);
            break;
        case BLE_LL_SCHED_TYPE_SYNC:
            ble_ll_sync_rmvd_from_sched((struct ble_ll_sync_sm *)entry->cb_arg);
            break;
#endif
#endif
        default:
            BLE_LL_ASSERT(0);
            break;
        }

        TAILQ_REMOVE(&g_ble_ll_sched_q, entry, link);
        entry->enqueued = 0;

        if (entry == end_overlap) {
            break;
        }
        entry = start_overlap;
    }

    entry = TAILQ_FIRST(&g_ble_ll_sched_q);
    if (entry == sch) {
        ble_ll_rfmgmt_sched_changed(sch);
    } else {
        sch = entry;
    }

    OS_EXIT_CRITICAL(sr);

    /* Restart timer */
    BLE_LL_ASSERT(sch != NULL);
    os_cputime_timer_start(&g_ble_ll_sched_timer, sch->start_time);

    return rc;
}

/**
 * Called to schedule a connection when the current role is master.
 *
 * Context: Interrupt
 *
 * @param connsm
 * @param ble_hdr
 * @param pyld_len
 *
 * @return int
 */
#if MYNEWT_VAL(BLE_LL_STRICT_CONN_SCHEDULING)
int
ble_ll_sched_master_new(struct ble_ll_conn_sm *connsm,
                        struct ble_mbuf_hdr *ble_hdr, uint8_t pyld_len)
{
    int rc;
    os_sr_t sr;
    uint32_t initial_start;
    uint32_t earliest_start;
    uint32_t earliest_end;
    uint32_t dur;
    uint32_t itvl_t;
    uint32_t adv_rxend;
    int i;
    uint32_t tpp;
    uint32_t tse;
    uint32_t np;
    uint32_t cp;
    uint32_t tick_in_period;

    struct ble_ll_sched_item *entry;
    struct ble_ll_sched_item *sch;

    /* Better have a connsm */
    BLE_LL_ASSERT(connsm != NULL);

    /* Get schedule element from connection */
    rc = -1;
    sch = &connsm->conn_sch;

    /* XXX:
     * The calculations for the 32kHz crystal bear alot of explanation. The
     * earliest possible time that the master can start the connection with a
     * slave is 1.25 msecs from the end of the connection request. The
     * connection request is sent an IFS time from the end of the advertising
     * packet that was received plus the time it takes to send the connection
     * request. At 1 Mbps, this is 1752 usecs, or 57.41 ticks. Using 57 ticks
     * makes us off ~13 usecs. Since we dont want to actually calculate the
     * receive end time tick (this would take too long), we assume the end of
     * the advertising PDU is 'now' (we call os_cputime_get32). We dont know
     * how much time it will take to service the ISR but if we are more than the
     * rx to tx time of the chip we will not be successful transmitting the
     * connect request. All this means is that we presume that the slave will
     * receive the connect request later than we expect but no earlier than
     * 13 usecs before (this is important).
     *
     * The code then attempts to schedule the connection at the
     * earliest time although this may not be possible. When the actual
     * schedule start time is determined, the master has to determine if this
     * time is more than a transmit window offset interval (1.25 msecs). The
     * master has to tell the slave how many transmit window offsets there are
     * from the earliest possible time to when the actual transmit start will
     * occur. Later in this function you will see the calculation. The actual
     * transmission start has to occur within the transmit window. The transmit
     * window interval is in units of 1.25 msecs and has to be at least 1. To
     * make things a bit easier (but less power efficient for the slave), we
     * use a transmit window of 2. We do this because we dont quite know the
     * exact start of the transmission and if we are too early or too late we
     * could miss the transmit window. A final note: the actual transmission
     * start (the anchor point) is sched offset ticks from the schedule start
     * time. We dont add this to the calculation when calculating the window
     * offset. The reason we dont do this is we want to insure we transmit
     * after the window offset we tell the slave. For example, say we think
     * we are transmitting 1253 usecs from the earliest start. This would cause
     * us to send a transmit window offset of 1. Since we are actually
     * transmitting earlier than the slave thinks we could end up transmitting
     * before the window offset. Transmitting later is fine since we have the
     * transmit window to do so. Transmitting before is bad, since the slave
     * wont be listening. We could do better calculation if we wanted to use
     * a transmit window of 1 as opposed to 2, but for now we dont care.
     */
    dur = os_cputime_usecs_to_ticks(g_ble_ll_sched_data.sch_ticks_per_period);
    adv_rxend = os_cputime_get32();
    if (ble_hdr->rxinfo.channel >= BLE_PHY_NUM_DATA_CHANS) {
        /*
         * We received packet on advertising channel which means this is a legacy
         * PDU on 1 Mbps - we do as described above.
         */
        earliest_start = adv_rxend + 57;
    } else {
        /*
         * The calculations are similar as above.
         *
         * We received packet on data channel which means this is AUX_ADV_IND
         * received on secondary adv channel. We can schedule first packet at
         * the earliest after "T_IFS + AUX_CONNECT_REQ + transmitWindowDelay".
         * AUX_CONNECT_REQ and transmitWindowDelay times vary depending on which
         * PHY we received on.
         *
         */
        if (ble_hdr->rxinfo.phy == BLE_PHY_1M) {
            // 150 + 352 + 2500 = 3002us = 98.37 ticks
            earliest_start = adv_rxend + 98;
        } else if (ble_hdr->rxinfo.phy == BLE_PHY_2M) {
            // 150 + 180 + 2500 = 2830us = 92.73 ticks
            earliest_start = adv_rxend + 93;
        } else if (ble_hdr->rxinfo.phy == BLE_PHY_CODED) {
            // 150 + 2896 + 3750 = 6796us = 222.69 ticks
            earliest_start = adv_rxend + 223;
        } else {
            BLE_LL_ASSERT(0);
        }
    }
    earliest_start += MYNEWT_VAL(BLE_LL_CONN_INIT_MIN_WIN_OFFSET) *
                      BLE_LL_SCHED_32KHZ_TICKS_PER_SLOT;
    itvl_t = connsm->conn_itvl_ticks;

    /* We have to find a place for this schedule */
    OS_ENTER_CRITICAL(sr);

    /*
     * Are there any allocated periods? If not, set epoch start to earliest
     * time
     */
    if (g_ble_ll_sched_data.sch_num_occ_periods == 0) {
        g_ble_ll_sched_data.sch_epoch_start = earliest_start;
        cp = 0;
    } else {
        /*
         * Earliest start must occur on period boundary.
         * (tse = ticks since epoch)
         */
        tpp = g_ble_ll_sched_data.sch_ticks_per_period;
        tse = earliest_start - g_ble_ll_sched_data.sch_epoch_start;
        np = tse / tpp;
        cp = np % BLE_LL_SCHED_PERIODS;
        tick_in_period = tse - (np * tpp);
        if (tick_in_period != 0) {
            ++cp;
            if (cp == BLE_LL_SCHED_PERIODS) {
                cp = 0;
            }
            earliest_start += (tpp - tick_in_period);
        }

        /* Now find first un-occupied period starting from cp */
        for (i = 0; i < BLE_LL_SCHED_PERIODS; ++i) {
            if (g_ble_ll_sched_data.sch_occ_period_mask & (1 << cp)) {
                ++cp;
                if (cp == BLE_LL_SCHED_PERIODS) {
                    cp = 0;
                }
                earliest_start += tpp;
            } else {
                /* not occupied */
                break;
            }
        }
        /* Should never happen but if it does... */
        if (i == BLE_LL_SCHED_PERIODS) {
            OS_EXIT_CRITICAL(sr);
            return rc;
        }
    }

    sch->start_time = earliest_start;
    initial_start = earliest_start;
    earliest_end = earliest_start + dur;

    if (!ble_ll_sched_insert_if_empty(sch)) {
        /* Nothing in schedule. Schedule as soon as possible */
        rc = 0;
        connsm->tx_win_off = MYNEWT_VAL(BLE_LL_CONN_INIT_MIN_WIN_OFFSET);
    } else {
        os_cputime_timer_stop(&g_ble_ll_sched_timer);
        TAILQ_FOREACH(entry, &g_ble_ll_sched_q, link) {
            /* Set these because overlap function needs them to be set */
            sch->start_time = earliest_start;
            sch->end_time = earliest_end;

            /* We can insert if before entry in list */
            if ((int32_t)(sch->end_time - entry->start_time) <= 0) {
                if ((earliest_start - initial_start) <= itvl_t) {
                    rc = 0;
                    TAILQ_INSERT_BEFORE(entry, sch, link);
                }
                break;
            }

            /* Check for overlapping events */
            if (ble_ll_sched_is_overlap(sch, entry)) {
                /* Earliest start is end of this event since we overlap */
                earliest_start = entry->end_time;
                earliest_end = earliest_start + dur;
            }
        }

        /* Must be able to schedule within one connection interval */
        if (!entry) {
            if ((earliest_start - initial_start) <= itvl_t) {
                rc = 0;
                TAILQ_INSERT_TAIL(&g_ble_ll_sched_q, sch, link);
            }
        }

        if (!rc) {
            /* calculate number of window offsets. Each offset is 1.25 ms */
            sch->enqueued = 1;
            /*
             * NOTE: we dont add sched offset ticks as we want to under-estimate
             * the transmit window slightly since the window size is currently
             * 2 when using a 32768 crystal.
             */
            dur = os_cputime_ticks_to_usecs(earliest_start - initial_start);
            connsm->tx_win_off = dur / BLE_LL_CONN_TX_OFF_USECS;
        }
    }

    if (!rc) {
        sch->start_time = earliest_start;
        sch->end_time = earliest_end;
        /*
         * Since we have the transmit window to transmit in, we dont need
         * to set the anchor point usecs; just transmit to the nearest tick.
         */
        connsm->anchor_point = earliest_start + g_ble_ll_sched_offset_ticks;
        connsm->anchor_point_usecs = 0;
        connsm->ce_end_time = earliest_end;
        connsm->period_occ_mask = (1 << cp);
        g_ble_ll_sched_data.sch_occ_period_mask |= connsm->period_occ_mask;
        ++g_ble_ll_sched_data.sch_num_occ_periods;
    }


    /* Get head of list to restart timer */
    sch = TAILQ_FIRST(&g_ble_ll_sched_q);
    ble_ll_rfmgmt_sched_changed(sch);

    OS_EXIT_CRITICAL(sr);

    os_cputime_timer_start(&g_ble_ll_sched_timer, sch->start_time);

    return rc;
}
#else
int
ble_ll_sched_master_new(struct ble_ll_conn_sm *connsm,
                        struct ble_mbuf_hdr *ble_hdr, uint8_t pyld_len)
{
    int rc;
    os_sr_t sr;
    uint8_t req_slots;
    uint32_t initial_start;
    uint32_t earliest_start;
    uint32_t earliest_end;
    uint32_t dur;
    uint32_t itvl_t;
    uint32_t adv_rxend;
    struct ble_ll_sched_item *entry;
    struct ble_ll_sched_item *sch;

    /*
     * XXX: TODO this code assumes the advertisement and connect request were
     * sent at 1Mbps.
     */

    /* Get schedule element from connection */
    rc = -1;
    sch = &connsm->conn_sch;
    req_slots = MYNEWT_VAL(BLE_LL_CONN_INIT_SLOTS);

    /* XXX:
     * The calculations for the 32kHz crystal bear alot of explanation. The
     * earliest possible time that the master can start the connection with a
     * slave is 1.25 msecs from the end of the connection request. The
     * connection request is sent an IFS time from the end of the advertising
     * packet that was received plus the time it takes to send the connection
     * request. At 1 Mbps, this is 1752 usecs, or 57.41 ticks. Using 57 ticks
     * makes us off ~13 usecs. Since we dont want to actually calculate the
     * receive end time tick (this would take too long), we assume the end of
     * the advertising PDU is 'now' (we call os_cputime_get32). We dont know
     * how much time it will take to service the ISR but if we are more than the
     * rx to tx time of the chip we will not be successful transmitting the
     * connect request. All this means is that we presume that the slave will
     * receive the connect request later than we expect but no earlier than
     * 13 usecs before (this is important).
     *
     * The code then attempts to schedule the connection at the
     * earliest time although this may not be possible. When the actual
     * schedule start time is determined, the master has to determine if this
     * time is more than a transmit window offset interval (1.25 msecs). The
     * master has to tell the slave how many transmit window offsets there are
     * from the earliest possible time to when the actual transmit start will
     * occur. Later in this function you will see the calculation. The actual
     * transmission start has to occur within the transmit window. The transmit
     * window interval is in units of 1.25 msecs and has to be at least 1. To
     * make things a bit easier (but less power efficient for the slave), we
     * use a transmit window of 2. We do this because we dont quite know the
     * exact start of the transmission and if we are too early or too late we
     * could miss the transmit window. A final note: the actual transmission
     * start (the anchor point) is sched offset ticks from the schedule start
     * time. We dont add this to the calculation when calculating the window
     * offset. The reason we dont do this is we want to insure we transmit
     * after the window offset we tell the slave. For example, say we think
     * we are transmitting 1253 usecs from the earliest start. This would cause
     * us to send a transmit window offset of 1. Since we are actually
     * transmitting earlier than the slave thinks we could end up transmitting
     * before the window offset. Transmitting later is fine since we have the
     * transmit window to do so. Transmitting before is bad, since the slave
     * wont be listening. We could do better calculation if we wanted to use
     * a transmit window of 1 as opposed to 2, but for now we dont care.
     */
    dur = req_slots * BLE_LL_SCHED_32KHZ_TICKS_PER_SLOT;
    adv_rxend = os_cputime_get32();
    if (ble_hdr->rxinfo.channel >= BLE_PHY_NUM_DATA_CHANS) {
        /*
         * We received packet on advertising channel which means this is a legacy
         * PDU on 1 Mbps - we do as described above.
         */
        earliest_start = adv_rxend + 57;
    } else {
        /*
         * The calculations are similar as above.
         *
         * We received packet on data channel which means this is AUX_ADV_IND
         * received on secondary adv channel. We can schedule first packet at
         * the earliest after "T_IFS + AUX_CONNECT_REQ + transmitWindowDelay".
         * AUX_CONNECT_REQ and transmitWindowDelay times vary depending on which
         * PHY we received on.
         *
         */
        if (ble_hdr->rxinfo.phy == BLE_PHY_1M) {
            // 150 + 352 + 2500 = 3002us = 98.37 ticks
            earliest_start = adv_rxend + 98;
        } else if (ble_hdr->rxinfo.phy == BLE_PHY_2M) {
            // 150 + 180 + 2500 = 2830us = 92.73 ticks
            earliest_start = adv_rxend + 93;
        } else if (ble_hdr->rxinfo.phy == BLE_PHY_CODED) {
            // 150 + 2896 + 3750 = 6796us = 222.69 ticks
            earliest_start = adv_rxend + 223;
        } else {
            BLE_LL_ASSERT(0);
        }
    }
    earliest_start += MYNEWT_VAL(BLE_LL_CONN_INIT_MIN_WIN_OFFSET) *
                      BLE_LL_SCHED_32KHZ_TICKS_PER_SLOT;
    earliest_end = earliest_start + dur;
    itvl_t = connsm->conn_itvl_ticks;

    /* We have to find a place for this schedule */
    OS_ENTER_CRITICAL(sr);

    /* The schedule item must occur after current running item (if any) */
    sch->start_time = earliest_start;
    initial_start = earliest_start;

    if (!ble_ll_sched_insert_if_empty(sch)) {
        /* Nothing in schedule. Schedule as soon as possible */
        rc = 0;
        connsm->tx_win_off = MYNEWT_VAL(BLE_LL_CONN_INIT_MIN_WIN_OFFSET);
    } else {
        os_cputime_timer_stop(&g_ble_ll_sched_timer);
        TAILQ_FOREACH(entry, &g_ble_ll_sched_q, link) {
            /* Set these because overlap function needs them to be set */
            sch->start_time = earliest_start;
            sch->end_time = earliest_end;

            /* We can insert if before entry in list */
            if ((int32_t)(sch->end_time - entry->start_time) <= 0) {
                if ((earliest_start - initial_start) <= itvl_t) {
                    rc = 0;
                    TAILQ_INSERT_BEFORE(entry, sch, link);
                }
                break;
            }

            /* Check for overlapping events */
            if (ble_ll_sched_is_overlap(sch, entry)) {
                /* Earliest start is end of this event since we overlap */
                earliest_start = entry->end_time;
                earliest_end = earliest_start + dur;
            }
        }

        /* Must be able to schedule within one connection interval */
        if (!entry) {
            if ((earliest_start - initial_start) <= itvl_t) {
                rc = 0;
                TAILQ_INSERT_TAIL(&g_ble_ll_sched_q, sch, link);
            }
        }

        if (!rc) {
            /* calculate number of window offsets. Each offset is 1.25 ms */
            sch->enqueued = 1;
            /*
             * NOTE: we dont add sched offset ticks as we want to under-estimate
             * the transmit window slightly since the window size is currently
             * 2 when using a 32768 crystal.
             */
            dur = os_cputime_ticks_to_usecs(earliest_start - initial_start);
            connsm->tx_win_off = dur / BLE_LL_CONN_TX_OFF_USECS;
        }
    }

    if (!rc) {
        sch->start_time = earliest_start;
        sch->end_time = earliest_end;
        /*
         * Since we have the transmit window to transmit in, we dont need
         * to set the anchor point usecs; just transmit to the nearest tick.
         */
        connsm->anchor_point = earliest_start + g_ble_ll_sched_offset_ticks;
        connsm->anchor_point_usecs = 0;
        connsm->ce_end_time = earliest_end;
    }

    /* Get head of list to restart timer */
    sch = TAILQ_FIRST(&g_ble_ll_sched_q);
    ble_ll_rfmgmt_sched_changed(sch);

    OS_EXIT_CRITICAL(sr);

    os_cputime_timer_start(&g_ble_ll_sched_timer, sch->start_time);

    return rc;
}
#endif

/**
 * Schedules a slave connection for the first time.
 *
 * Context: Link Layer
 *
 * @param connsm
 *
 * @return int
 */
int
ble_ll_sched_slave_new(struct ble_ll_conn_sm *connsm)
{
    int rc;
    os_sr_t sr;
    struct ble_ll_sched_item *entry;
    struct ble_ll_sched_item *next_sch;
    struct ble_ll_sched_item *sch;
    int first = 0;

    /* Get schedule element from connection */
    rc = -1;
    sch = &connsm->conn_sch;

    /* Set schedule start and end times */
    /*
     * XXX: for now, we dont care about anchor point usecs for the slave. It
     * does not matter if we turn on the receiver up to one tick before w
     * need to. We also subtract one extra tick since the conversion from
     * usecs to ticks could be off by up to 1 tick.
     */
    sch->start_time = connsm->anchor_point - g_ble_ll_sched_offset_ticks -
        os_cputime_usecs_to_ticks(connsm->slave_cur_window_widening) - 1;
    sch->end_time = connsm->ce_end_time;
    sch->remainder = 0;

    /* We have to find a place for this schedule */
    OS_ENTER_CRITICAL(sr);

    /* The schedule item must occur after current running item (if any) */
    if (ble_ll_sched_overlaps_current(sch)) {
        OS_EXIT_CRITICAL(sr);
        return rc;
    }

    entry = ble_ll_sched_insert_if_empty(sch);
    if (!entry) {
        /* Nothing in schedule. Schedule as soon as possible */
        rc = 0;
        first = 1;
    } else {
        os_cputime_timer_stop(&g_ble_ll_sched_timer);
        while (1) {
            next_sch = entry->link.tqe_next;
            /* Insert if event ends before next starts */
            if ((int32_t)(sch->end_time - entry->start_time) <= 0) {
                rc = 0;
                TAILQ_INSERT_BEFORE(entry, sch, link);
                break;
            }

            if (ble_ll_sched_is_overlap(sch, entry)) {
                /* If we overlap with a connection, we re-schedule */
                if (ble_ll_sched_conn_overlap(entry)) {
                    break;
                }
            }

            /* Move to next entry */
            entry = next_sch;

            /* Insert at tail if none left to check */
            if (!entry) {
                rc = 0;
                TAILQ_INSERT_TAIL(&g_ble_ll_sched_q, sch, link);
                break;
            }
        }

        if (!rc) {
            sch->enqueued = 1;
        }

        next_sch = TAILQ_FIRST(&g_ble_ll_sched_q);
        if (next_sch == sch) {
            first = 1;
        } else {
            sch = next_sch;
        }
    }

    if (first) {
        ble_ll_rfmgmt_sched_changed(sch);
    }

    OS_EXIT_CRITICAL(sr);

    os_cputime_timer_start(&g_ble_ll_sched_timer, sch->start_time);

    return rc;
}

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV)
/*
 * Determines if the schedule item overlaps the currently running schedule
 * item. This function cares about connection and sync.
 */
static int
ble_ll_sched_sync_overlaps_current(struct ble_ll_sched_item *sch)
{
    uint32_t end_time;
    uint8_t state;

    state = ble_ll_state_get();
    switch (state) {
    case BLE_LL_STATE_CONNECTION:
        end_time = ble_ll_conn_get_ce_end_time();
        break;
    case BLE_LL_STATE_SYNC:
        end_time = ble_ll_sync_get_event_end_time();
        break;
    default:
        return 0;
    }

    return CPUTIME_GT(end_time, sch->start_time);
}

int
ble_ll_sched_sync_reschedule(struct ble_ll_sched_item *sch,
                             uint32_t anchor_point, uint8_t anchor_point_usecs,
                             uint32_t window_widening,
                             int8_t phy_mode)
{
    struct ble_ll_sched_item *entry;
    uint8_t start_time_rem_usecs;
    uint8_t window_rem_usecs;
    uint32_t window_ticks;
    uint32_t start_time;
    uint32_t end_time;
    uint32_t dur;
    int rc = 0;
    os_sr_t sr;

    window_ticks = os_cputime_usecs_to_ticks(window_widening);
    window_rem_usecs = window_widening - os_cputime_ticks_to_usecs(window_ticks);

    /* adjust for subtraction */
    anchor_point_usecs += 31;
    anchor_point--;

    start_time = anchor_point - window_ticks;
    start_time_rem_usecs = anchor_point_usecs - window_rem_usecs;
    if (start_time_rem_usecs >= 31) {
        start_time++;
        start_time_rem_usecs -= 31;
    }

    dur = ble_ll_pdu_tx_time_get(MYNEWT_VAL(BLE_LL_SCHED_SCAN_SYNC_PDU_LEN),
                                 phy_mode);
    end_time = start_time + os_cputime_usecs_to_ticks(dur);

    start_time -= g_ble_ll_sched_offset_ticks;

    /* Set schedule start and end times */
    sch->start_time = start_time;
    sch->remainder = start_time_rem_usecs;
    sch->end_time = end_time;

    /* Better be past current time or we just leave */
    if (CPUTIME_LEQ(sch->start_time, os_cputime_get32())) {
        return -1;
    }

    /* We have to find a place for this schedule */
    OS_ENTER_CRITICAL(sr);

    if (ble_ll_sched_sync_overlaps_current(sch)) {
        OS_EXIT_CRITICAL(sr);
        return -1;
    }

    /* Try to find slot for sync scan. */
    os_cputime_timer_stop(&g_ble_ll_sched_timer);

    TAILQ_FOREACH(entry, &g_ble_ll_sched_q, link) {
        /* We can insert if before entry in list */
        if (CPUTIME_LEQ(sch->end_time, entry->start_time)) {
            TAILQ_INSERT_BEFORE(entry, sch, link);
            sch->enqueued = 1;
            break;
        }

        /* Check for overlapping events. For now drop if it overlaps with
         * anything. We can make it smarter later on
         */
        if (ble_ll_sched_is_overlap(sch, entry)) {
            rc = -1;
            break;
        }
    }

    if (!entry) {
        TAILQ_INSERT_TAIL(&g_ble_ll_sched_q, sch, link);
        sch->enqueued = 1;
    }

    entry = TAILQ_FIRST(&g_ble_ll_sched_q);
    if (entry == sch) {
        ble_ll_rfmgmt_sched_changed(sch);
    } else {
        sch = entry;
    }

    OS_EXIT_CRITICAL(sr);

    /* Restart timer */
    BLE_LL_ASSERT(sch != NULL);
    os_cputime_timer_start(&g_ble_ll_sched_timer, sch->start_time);

    return rc;
}

int
ble_ll_sched_sync(struct ble_ll_sched_item *sch,
                  uint32_t beg_cputime, uint32_t rem_usecs,
                  uint32_t offset, int8_t phy_mode)
{
    struct ble_ll_sched_item *entry;
    uint32_t start_time_rem_usecs;
    uint32_t off_rem_usecs;
    uint32_t start_time;
    uint32_t off_ticks;
    uint32_t end_time;
    uint32_t dur;
    os_sr_t sr;
    int rc = 0;

    off_ticks = os_cputime_usecs_to_ticks(offset);
    off_rem_usecs = offset - os_cputime_ticks_to_usecs(off_ticks);

    start_time = beg_cputime + off_ticks;
    start_time_rem_usecs = rem_usecs + off_rem_usecs;
    if (start_time_rem_usecs >= 31) {
        start_time++;
        start_time_rem_usecs -= 31;
    }

    dur = ble_ll_pdu_tx_time_get(MYNEWT_VAL(BLE_LL_SCHED_SCAN_SYNC_PDU_LEN),
                                  phy_mode);
    end_time = start_time + os_cputime_usecs_to_ticks(dur);

    start_time -= g_ble_ll_sched_offset_ticks;

    sch->start_time = start_time;
    sch->remainder = start_time_rem_usecs;
    sch->end_time = end_time;

    OS_ENTER_CRITICAL(sr);

    if (!ble_ll_sched_insert_if_empty(sch)) {
        /* Nothing in schedule. Schedule as soon as possible
         * If we are here it means sch has been added to the scheduler */
        goto done;
    }

    /* Try to find slot for scan. */
    os_cputime_timer_stop(&g_ble_ll_sched_timer);
    TAILQ_FOREACH(entry, &g_ble_ll_sched_q, link) {
        /* We can insert if before entry in list */
        if (CPUTIME_LEQ(sch->end_time, entry->start_time)) {
            TAILQ_INSERT_BEFORE(entry, sch, link);
            sch->enqueued = 1;
            break;
        }

        /* Check for overlapping events. For now drop if it overlaps with
         * anything. We can make it smarter later on
         */
        if (ble_ll_sched_is_overlap(sch, entry)) {
            rc = -1;
            break;
        }
    }

    if (!entry) {
        TAILQ_INSERT_TAIL(&g_ble_ll_sched_q, sch, link);
        sch->enqueued = 1;
    }

done:
    entry = TAILQ_FIRST(&g_ble_ll_sched_q);
    if (entry == sch) {
        ble_ll_rfmgmt_sched_changed(sch);
    } else {
        sch = entry;
    }

    OS_EXIT_CRITICAL(sr);

    /* Restart timer */
    BLE_LL_ASSERT(sch != NULL);
    os_cputime_timer_start(&g_ble_ll_sched_timer, sch->start_time);

    STATS_INC(ble_ll_stats, sync_scheduled);
    return rc;
}
#endif

int
ble_ll_sched_adv_new(struct ble_ll_sched_item *sch, ble_ll_sched_adv_new_cb cb,
                     void *arg)
{
    os_sr_t sr;
    uint32_t adv_start;
    uint32_t duration;
    struct ble_ll_sched_item *entry;
    struct ble_ll_sched_item *orig;

    /* Get length of schedule item */
    duration = sch->end_time - sch->start_time;
    orig = sch;

    OS_ENTER_CRITICAL(sr);
    entry = ble_ll_sched_insert_if_empty(sch);
    if (!entry) {
        adv_start = sch->start_time;
    } else {
        /* XXX: no need to stop timer if not first on list. Modify code? */
        os_cputime_timer_stop(&g_ble_ll_sched_timer);
        TAILQ_FOREACH(entry, &g_ble_ll_sched_q, link) {
            /* We can insert if before entry in list */
            if ((int32_t)(sch->end_time - entry->start_time) <= 0) {
                TAILQ_INSERT_BEFORE(entry, sch, link);
                break;
            }

            /* Check for overlapping events */
            if (ble_ll_sched_is_overlap(sch, entry)) {
                /* Earliest start is end of this event since we overlap */
                sch->start_time = entry->end_time;
                sch->end_time = sch->start_time + duration;
            }
        }

        if (!entry) {
            TAILQ_INSERT_TAIL(&g_ble_ll_sched_q, sch, link);
        }
        adv_start = sch->start_time;

        sch->enqueued = 1;

        /* Restart with head of list */
        sch = TAILQ_FIRST(&g_ble_ll_sched_q);
    }

    if (cb) {
        cb((struct ble_ll_adv_sm *)orig->cb_arg, adv_start, arg);
    }

    if (orig == sch) {
        ble_ll_rfmgmt_sched_changed(sch);
    }

    OS_EXIT_CRITICAL(sr);

    /* Restart timer */
    BLE_LL_ASSERT(sch != NULL);
    os_cputime_timer_start(&g_ble_ll_sched_timer, sch->start_time);

    return 0;
}

int
ble_ll_sched_periodic_adv(struct ble_ll_sched_item *sch, uint32_t *start,
                          bool after_overlap)
{
    int rc = 0;
    os_sr_t sr;
    uint32_t adv_start;
    uint32_t duration;
    struct ble_ll_sched_item *entry;
    struct ble_ll_sched_item *orig = sch;

    /* Get length of schedule item */
    duration = sch->end_time - sch->start_time;

    OS_ENTER_CRITICAL(sr);
    entry = ble_ll_sched_insert_if_empty(sch);
    if (!entry) {
        adv_start = sch->start_time;
    } else {
        /* XXX: no need to stop timer if not first on list. Modify code? */
        os_cputime_timer_stop(&g_ble_ll_sched_timer);
        TAILQ_FOREACH(entry, &g_ble_ll_sched_q, link) {
            /* We can insert if before entry in list */
            if ((int32_t)(sch->end_time - entry->start_time) <= 0) {
                TAILQ_INSERT_BEFORE(entry, sch, link);
                break;
            }

            /* Check for overlapping events */
            if (ble_ll_sched_is_overlap(sch, entry)) {
                if (after_overlap) {
                    /* Earliest start is end of this event since we overlap */
                    sch->start_time = entry->end_time;
                    sch->end_time = sch->start_time + duration;
                } else {
                    rc = -1;
                    break;
                }
            }
        }

        if (!entry) {
            TAILQ_INSERT_TAIL(&g_ble_ll_sched_q, sch, link);
        }
        adv_start = sch->start_time;

        if (!rc) {
            sch->enqueued = 1;
        }

        /* Restart with head of list */
        sch = TAILQ_FIRST(&g_ble_ll_sched_q);
    }

    if (!rc) {
        *start = adv_start;
    }

    if (orig == sch) {
        ble_ll_rfmgmt_sched_changed(sch);
    }

    OS_EXIT_CRITICAL(sr);

    /* Restart timer */
    BLE_LL_ASSERT(sch != NULL);
    os_cputime_timer_start(&g_ble_ll_sched_timer, sch->start_time);

    return rc;
}

int
ble_ll_sched_adv_reschedule(struct ble_ll_sched_item *sch, uint32_t *start,
                            uint32_t max_delay_ticks)
{
    int rc;
    os_sr_t sr;
    uint32_t orig_start;
    uint32_t duration;
    uint32_t rand_ticks;
    struct ble_ll_sched_item *entry;
    struct ble_ll_sched_item *next_sch;
    struct ble_ll_sched_item *before;
    struct ble_ll_sched_item *start_overlap;
    struct ble_ll_sched_item *end_overlap;

    /* Get length of schedule item */
    duration = sch->end_time - sch->start_time;

    /* Add maximum randomization delay to end */
    rand_ticks = max_delay_ticks;
    sch->end_time += max_delay_ticks;

    start_overlap = NULL;
    end_overlap = NULL;
    before = NULL;
    rc = 0;
    OS_ENTER_CRITICAL(sr);

    entry = ble_ll_sched_insert_if_empty(sch);
    if (entry) {
        os_cputime_timer_stop(&g_ble_ll_sched_timer);
        while (1) {
            next_sch = entry->link.tqe_next;
            if (ble_ll_sched_is_overlap(sch, entry)) {
                if (start_overlap == NULL) {
                    start_overlap = entry;
                    end_overlap = entry;
                } else {
                    end_overlap = entry;
                }
            } else {
                if ((int32_t)(sch->end_time - entry->start_time) <= 0) {
                    before = entry;
                    break;
                }
            }

            entry = next_sch;
            if (entry == NULL) {
                break;
            }
        }

        /*
         * If there is no overlap, we either insert before the 'before' entry
         * or we insert at the end if there is no before entry.
         */
        if (start_overlap == NULL) {
            if (before) {
                TAILQ_INSERT_BEFORE(before, sch, link);
            } else {
                TAILQ_INSERT_TAIL(&g_ble_ll_sched_q, sch, link);
            }
        } else {
            /*
             * This item will overlap with others. See if we can fit it in
             * with original duration.
             */
            before = NULL;
            orig_start = sch->start_time;
            entry = start_overlap;
            sch->end_time = sch->start_time + duration;
            while (1) {
                next_sch = entry->link.tqe_next;
                if ((int32_t)(sch->end_time - entry->start_time) <= 0) {
                    rand_ticks = entry->start_time - sch->end_time;
                    before = entry;
                    TAILQ_INSERT_BEFORE(before, sch, link);
                    break;
                } else {
                    sch->start_time = entry->end_time;
                    sch->end_time = sch->start_time + duration;
                }

                if (entry == end_overlap) {
                    rand_ticks = (orig_start + max_delay_ticks) - sch->start_time;
                    if (rand_ticks > max_delay_ticks) {
                        /* No place for advertisement. */
                        rc = -1;
                    } else {
                        if (next_sch == NULL) {
                            TAILQ_INSERT_TAIL(&g_ble_ll_sched_q, sch, link);
                        } else {
                            TAILQ_INSERT_BEFORE(next_sch, sch, link);
                        }
                    }
                    break;
                }
                entry = next_sch;
                BLE_LL_ASSERT(entry != NULL);
            }
        }
    }

    if (!rc) {
        sch->enqueued = 1;
        if (rand_ticks) {
            sch->start_time += rand() % rand_ticks;
        }
        sch->end_time = sch->start_time + duration;
        *start = sch->start_time;

        if (sch == TAILQ_FIRST(&g_ble_ll_sched_q)) {
            ble_ll_rfmgmt_sched_changed(sch);
        }
    }

    OS_EXIT_CRITICAL(sr);

    sch = TAILQ_FIRST(&g_ble_ll_sched_q);
    os_cputime_timer_start(&g_ble_ll_sched_timer, sch->start_time);

    return rc;
}

int
ble_ll_sched_adv_resched_pdu(struct ble_ll_sched_item *sch)
{
    uint8_t lls;
    os_sr_t sr;
    struct ble_ll_sched_item *entry;

    OS_ENTER_CRITICAL(sr);

    lls = ble_ll_state_get();
    if ((lls == BLE_LL_STATE_ADV) || (lls == BLE_LL_STATE_CONNECTION) ||
            (lls == BLE_LL_STATE_SYNC)) {
        goto adv_resched_pdu_fail;
    }

    entry = ble_ll_sched_insert_if_empty(sch);
    if (entry) {
        /* If we overlap with the first item, simply re-schedule */
        if (ble_ll_sched_is_overlap(sch, entry)) {
            goto adv_resched_pdu_fail;
        }
        os_cputime_timer_stop(&g_ble_ll_sched_timer);
        TAILQ_INSERT_BEFORE(entry, sch, link);
        sch->enqueued = 1;
    }

    ble_ll_rfmgmt_sched_changed(TAILQ_FIRST(&g_ble_ll_sched_q));

    OS_EXIT_CRITICAL(sr);
    os_cputime_timer_start(&g_ble_ll_sched_timer, sch->start_time);
    return 0;

adv_resched_pdu_fail:
    OS_EXIT_CRITICAL(sr);
    return -1;
}

/**
 * Remove a schedule element
 *
 * @param sched_type
 *
 * @return int 0 - removed, 1 - not in the list
 */
int
ble_ll_sched_rmv_elem(struct ble_ll_sched_item *sch)
{
    os_sr_t sr;
    struct ble_ll_sched_item *first;
    int rc = 1;

    if (!sch) {
        return rc;
    }

    OS_ENTER_CRITICAL(sr);
    if (sch->enqueued) {
        first = TAILQ_FIRST(&g_ble_ll_sched_q);
        if (first == sch) {
            os_cputime_timer_stop(&g_ble_ll_sched_timer);
        }

        TAILQ_REMOVE(&g_ble_ll_sched_q, sch, link);
        sch->enqueued = 0;
        rc = 0;

        if (first == sch) {
            first = TAILQ_FIRST(&g_ble_ll_sched_q);
            if (first) {
                os_cputime_timer_start(&g_ble_ll_sched_timer, first->start_time);
            }
            ble_ll_rfmgmt_sched_changed(first);
        }
    }
    OS_EXIT_CRITICAL(sr);

    return rc;
}

void
ble_ll_sched_rmv_elem_type(uint8_t type, sched_remove_cb_func remove_cb)
{
    os_sr_t sr;
    struct ble_ll_sched_item *entry;
    struct ble_ll_sched_item *first;

    OS_ENTER_CRITICAL(sr);
    first = TAILQ_FIRST(&g_ble_ll_sched_q);

    if (!first) {
        OS_EXIT_CRITICAL(sr);
        return;
    }

    TAILQ_FOREACH(entry, &g_ble_ll_sched_q, link) {
        if (entry->sched_type == type) {
            if (first == entry) {
                os_cputime_timer_stop(&g_ble_ll_sched_timer);
                first = NULL;
            }

            TAILQ_REMOVE(&g_ble_ll_sched_q, entry, link);
            remove_cb(entry);
            entry->enqueued = 0;
        }
    }

    if (!first) {
        first = TAILQ_FIRST(&g_ble_ll_sched_q);
        if (first) {
            os_cputime_timer_start(&g_ble_ll_sched_timer, first->start_time);
        }
        ble_ll_rfmgmt_sched_changed(first);
    }

    OS_EXIT_CRITICAL(sr);
}

/**
 * Executes a schedule item by calling the schedule callback function.
 *
 * Context: Interrupt
 *
 * @param sch Pointer to schedule item
 *
 * @return int 0: schedule item is not over; otherwise schedule item is done.
 */
static int
ble_ll_sched_execute_item(struct ble_ll_sched_item *sch)
{
    int rc;
    uint8_t lls;

    lls = ble_ll_state_get();

    ble_ll_trace_u32x3(BLE_LL_TRACE_ID_SCHED, lls, os_cputime_get32(),
                       sch->start_time);

    if (lls == BLE_LL_STATE_STANDBY) {
        goto sched;
    }

    /* If aux scan scheduled and LL is in state when scanner is running
     * in 3 states:
     * BLE_LL_STATE_SCANNING
     * BLE_LL_STATE_INITIATING
     * BLE_LL_STATE_STANDBY
     *
     * Let scanner to decide to disable phy or not.
     */
    if (sch->sched_type == BLE_LL_SCHED_TYPE_AUX_SCAN) {
        if (lls == BLE_LL_STATE_INITIATING || lls == BLE_LL_STATE_SCANNING) {
            goto sched;
        }
    }

    /*
     * This is either an advertising event or connection event start. If
     * we are scanning or initiating just stop it.
     */

    /* We have to disable the PHY no matter what */
    ble_phy_disable();

    if (lls == BLE_LL_STATE_SCANNING) {
        ble_ll_state_set(BLE_LL_STATE_STANDBY);
        ble_ll_scan_halt();
    } else if (lls == BLE_LL_STATE_INITIATING) {
        ble_ll_state_set(BLE_LL_STATE_STANDBY);
        ble_ll_scan_halt();
        /* PHY is disabled - make sure we do not wait for AUX_CONNECT_RSP */
        ble_ll_conn_reset_pending_aux_conn_rsp();
    } else if (lls == BLE_LL_STATE_ADV) {
        STATS_INC(ble_ll_stats, sched_state_adv_errs);
        ble_ll_adv_halt();
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV)
    } else if (lls == BLE_LL_STATE_SYNC) {
        STATS_INC(ble_ll_stats, sched_state_sync_errs);
        ble_ll_sync_halt();
#endif
    } else {
        STATS_INC(ble_ll_stats, sched_state_conn_errs);
        ble_ll_conn_event_halt();
    }

sched:
    BLE_LL_DEBUG_GPIO(SCHED_ITEM_CB, 1);
    BLE_LL_ASSERT(sch->sched_cb);
    rc = sch->sched_cb(sch);
    BLE_LL_DEBUG_GPIO(SCHED_ITEM_CB, 0);
    return rc;
}

/**
 * Run the BLE scheduler. Iterate through all items on the schedule queue.
 *
 * Context: interrupt (scheduler)
 *
 * @return int
 */
static void
ble_ll_sched_run(void *arg)
{
    struct ble_ll_sched_item *sch;

    BLE_LL_DEBUG_GPIO(SCHED_RUN, 1);

    /* Look through schedule queue */
    sch = TAILQ_FIRST(&g_ble_ll_sched_q);
    if (sch) {
#if (BLE_LL_SCHED_DEBUG == 1)
        int32_t dt;

        /* Make sure we have passed the start time of the first event */
        dt = (int32_t)(os_cputime_get32() - sch->start_time);
        if (dt > g_ble_ll_sched_max_late) {
            g_ble_ll_sched_max_late = dt;
        }
        if (dt < g_ble_ll_sched_max_early) {
            g_ble_ll_sched_max_early = dt;
        }
#endif

        /* Remove schedule item and execute the callback */
        TAILQ_REMOVE(&g_ble_ll_sched_q, sch, link);
        sch->enqueued = 0;
        ble_ll_sched_execute_item(sch);

        /* Restart if there is an item on the schedule */
        sch = TAILQ_FIRST(&g_ble_ll_sched_q);
        if (sch) {
            os_cputime_timer_start(&g_ble_ll_sched_timer, sch->start_time);
        }
        ble_ll_rfmgmt_sched_changed(sch);
    }

    BLE_LL_DEBUG_GPIO(SCHED_RUN, 0);
}

/**
 * Called to determine when the next scheduled event will occur.
 *
 * If there are not scheduled events this function returns 0; otherwise it
 * returns 1 and *next_event_time is set to the start time of the next event.
 *
 * @param next_event_time
 *
 * @return int 0: No events are scheduled 1: there is an upcoming event
 */
int
ble_ll_sched_next_time(uint32_t *next_event_time)
{
    int rc;
    os_sr_t sr;
    struct ble_ll_sched_item *first;

    rc = 0;
    OS_ENTER_CRITICAL(sr);
    first = TAILQ_FIRST(&g_ble_ll_sched_q);
    if (first) {
        *next_event_time = first->start_time;
        rc = 1;
    }
    OS_EXIT_CRITICAL(sr);

    return rc;
}

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
/**
 * Called to check if there is place for a planned scan req.
 *
 * @param chan
 * @param phy_mode
 *
 * @return int 0: Clear for scan req 1: there is an upcoming event
 */
int
ble_ll_sched_scan_req_over_aux_ptr(uint32_t chan, uint8_t phy_mode)
{
    struct ble_ll_sched_item *sch;
    uint32_t usec_dur;
    uint32_t now = os_cputime_get32();

    /* Lets calculate roughly how much time we need for scan req and scan rsp */
    usec_dur = ble_ll_pdu_tx_time_get(BLE_SCAN_REQ_LEN, phy_mode);
    if (chan >=  BLE_PHY_NUM_DATA_CHANS) {
        usec_dur += ble_ll_pdu_tx_time_get(BLE_SCAN_RSP_MAX_LEN, phy_mode);
    } else {
        usec_dur += ble_ll_pdu_tx_time_get(BLE_SCAN_RSP_MAX_EXT_LEN, phy_mode);
    }

    sch = TAILQ_FIRST(&g_ble_ll_sched_q);
    while (sch) {
        /* Let's check if there is no scheduled item which want to start within
         * given usecs.*/
        if ((int32_t)(sch->start_time - now + os_cputime_usecs_to_ticks(usec_dur)) > 0) {
            /* We are fine. Have time for scan req */
            return 0;
        }

        /* There is something in the scheduler. If it is not aux ptr we assume
         * it is more important that scan req
         */
        if (sch->sched_type != BLE_LL_SCHED_TYPE_AUX_SCAN) {
            return 1;
        }

        ble_ll_scan_end_adv_evt((struct ble_ll_aux_data *)sch->cb_arg);
        TAILQ_REMOVE(&g_ble_ll_sched_q, sch, link);
        sch->enqueued = 0;
        sch = TAILQ_FIRST(&g_ble_ll_sched_q);
    }
    return 0;
}

/**
 * Called to schedule a aux scan.
 *
 * Context: Interrupt
 *
 * @param ble_hdr
 * @param scansm
 * @param aux_scan
 *
 * @return 0 on success, 1 otherwise
 */
int
ble_ll_sched_aux_scan(struct ble_mbuf_hdr *ble_hdr,
                      struct ble_ll_scan_sm *scansm,
                      struct ble_ll_aux_data *aux_scan)
{
    int rc = 1;
    os_sr_t sr;
    uint32_t off_ticks;
    uint32_t off_rem_usecs;
    uint32_t start_time;
    uint32_t start_time_rem_usecs;
    uint32_t end_time;
    uint32_t dur;
    struct ble_ll_sched_item *entry;
    struct ble_ll_sched_item *sch;
    int phy_mode;

    sch = &aux_scan->sch;
    BLE_LL_ASSERT(sch->cb_arg == NULL);

    off_ticks = os_cputime_usecs_to_ticks(aux_scan->offset);
    off_rem_usecs = aux_scan->offset - os_cputime_ticks_to_usecs(off_ticks);

    start_time = ble_hdr->beg_cputime + off_ticks;
    start_time_rem_usecs = ble_hdr->rem_usecs + off_rem_usecs;
    if (start_time_rem_usecs >= 31) {
        start_time++;
        start_time_rem_usecs -= 31;
    }
    start_time -= g_ble_ll_sched_offset_ticks;

    /* Let's calculate time we reserve for aux packet. For now we assume to wait
     * for fixed number of bytes and handle possible interrupting it in
     * ble_ll_sched_execute_item(). This is because aux packet can be up to
     * 256bytes and we don't want to block sched that long
     */
    phy_mode = ble_ll_phy_to_phy_mode(aux_scan->aux_phy,
                                      BLE_HCI_LE_PHY_CODED_ANY);
    dur = ble_ll_pdu_tx_time_get(MYNEWT_VAL(BLE_LL_SCHED_SCAN_AUX_PDU_LEN),
                                 phy_mode);
    end_time = start_time + os_cputime_usecs_to_ticks(dur);

    sch->start_time = start_time;
    sch->remainder = start_time_rem_usecs;
    sch->end_time = end_time;

    OS_ENTER_CRITICAL(sr);

    if (!ble_ll_sched_insert_if_empty(sch)) {
        /* Nothing in schedule. Schedule as soon as possible
         * If we are here it means sch has been added to the scheduler */
        rc = 0;
        goto done;
    }

    /* Try to find slot for aux scan. */
    os_cputime_timer_stop(&g_ble_ll_sched_timer);
    TAILQ_FOREACH(entry, &g_ble_ll_sched_q, link) {
        /* We can insert if before entry in list */
        if ((int32_t)(sch->end_time - entry->start_time) <= 0) {
            rc = 0;
            TAILQ_INSERT_BEFORE(entry, sch, link);
            sch->enqueued = 1;
            break;
        }

        /* Check for overlapping events. For now drop if it overlaps with
         * anything. We can make it smarter later on
         */
        if (ble_ll_sched_is_overlap(sch, entry)) {
            break;
        }
    }

    if (!entry) {
        rc = 0;
        TAILQ_INSERT_TAIL(&g_ble_ll_sched_q, sch, link);
        sch->enqueued = 1;
    }

done:

    if (rc == 0) {
        sch->cb_arg = ble_ll_scan_aux_data_ref(aux_scan);
        STATS_INC(ble_ll_stats, aux_scheduled);
    }

    /* Get head of list to restart timer */
    entry = TAILQ_FIRST(&g_ble_ll_sched_q);
    if (entry == sch) {
        ble_ll_rfmgmt_sched_changed(sch);
    } else {
        sch = entry;
    }

    OS_EXIT_CRITICAL(sr);

    /* Restart timer */
    BLE_LL_ASSERT(sch != NULL);
    os_cputime_timer_start(&g_ble_ll_sched_timer, sch->start_time);

    return rc;
}
#endif

#if MYNEWT_VAL(BLE_LL_DTM)
int ble_ll_sched_dtm(struct ble_ll_sched_item *sch)
{
    int rc;
    os_sr_t sr;
    struct ble_ll_sched_item *entry;

    OS_ENTER_CRITICAL(sr);

    if (!ble_ll_sched_insert_if_empty(sch)) {
        /* Nothing in schedule. Schedule as soon as possible
         * If we are here it means sch has been added to the scheduler */
        rc = 0;
        goto done;
    }

    /* Try to find slot for test. */
    os_cputime_timer_stop(&g_ble_ll_sched_timer);
    TAILQ_FOREACH(entry, &g_ble_ll_sched_q, link) {
        /* We can insert if before entry in list */
        if (sch->end_time <= entry->start_time) {
            rc = 0;
            TAILQ_INSERT_BEFORE(entry, sch, link);
            sch->enqueued = 1;
            break;
        }

        /* Check for overlapping events. For now drop if it overlaps with
         * anything. We can make it smarter later on
         */
        if (ble_ll_sched_is_overlap(sch, entry)) {
            OS_EXIT_CRITICAL(sr);
            return -1;
        }
    }

    if (!entry) {
        rc = 0;
        TAILQ_INSERT_TAIL(&g_ble_ll_sched_q, sch, link);
        sch->enqueued = 1;
    }

done:

    /* Get head of list to restart timer */
    sch = TAILQ_FIRST(&g_ble_ll_sched_q);

    ble_ll_rfmgmt_sched_changed(sch);

    OS_EXIT_CRITICAL(sr);

    /* Restart timer */
    BLE_LL_ASSERT(sch != NULL);
    os_cputime_timer_start(&g_ble_ll_sched_timer, sch->start_time);

    return rc;
}
#endif
/**
 * Stop the scheduler
 *
 * Context: Link Layer task
 */
void
ble_ll_sched_stop(void)
{
    os_cputime_timer_stop(&g_ble_ll_sched_timer);
}

/**
 * Initialize the scheduler. Should only be called once and should be called
 * before any of the scheduler API are called.
 *
 * @return int
 */
int
ble_ll_sched_init(void)
{
    BLE_LL_DEBUG_GPIO_INIT(SCHED_ITEM_CB);
    BLE_LL_DEBUG_GPIO_INIT(SCHED_RUN);

    /*
     * Initialize max early to large negative number. This is used
     * to determine the worst-case "early" time the schedule was called. Dont
     * expect this to be less than -3 or -4.
     */
#if (BLE_LL_SCHED_DEBUG == 1)
    g_ble_ll_sched_max_early = -50000;
#endif

    /*
     * This is the offset from the start of the scheduled item until the actual
     * tx/rx should occur, in ticks. We also "round up" to the nearest tick.
     */
    g_ble_ll_sched_offset_ticks =
        (uint8_t) os_cputime_usecs_to_ticks(XCVR_TX_SCHED_DELAY_USECS + 30);

    /* Initialize cputimer for the scheduler */
    os_cputime_timer_init(&g_ble_ll_sched_timer, ble_ll_sched_run, NULL);

#if MYNEWT_VAL(BLE_LL_STRICT_CONN_SCHEDULING)
    memset(&g_ble_ll_sched_data, 0, sizeof(struct ble_ll_sched_obj));
    g_ble_ll_sched_data.sch_ticks_per_period =
        os_cputime_usecs_to_ticks(MYNEWT_VAL(BLE_LL_USECS_PER_PERIOD));
    g_ble_ll_sched_data.sch_ticks_per_epoch = BLE_LL_SCHED_PERIODS *
        g_ble_ll_sched_data.sch_ticks_per_period;
#endif

    return 0;
}

#endif
