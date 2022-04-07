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

#ifndef H_BLE_LL_SCHED_
#define H_BLE_LL_SCHED_

#ifdef __cplusplus
extern "C" {
#endif

/* Time per BLE scheduler slot */
#define BLE_LL_SCHED_USECS_PER_SLOT         (1250)
#define BLE_LL_SCHED_32KHZ_TICKS_PER_SLOT   (41)    /* 1 tick = 30.517 usecs */

/*
 * Worst case time needed for scheduled advertising item. This is the longest
 * possible time to receive a scan request and send a scan response (with the
 * appropriate IFS time between them). This number is calculated using the
 * following formula: IFS + SCAN_REQ + IFS + SCAN_RSP = 150 + 176 + 150 + 376.
 * Note: worst case time to tx adv, rx scan req and send scan rsp is 1228 usecs.
 * This assumes maximum sized advertising PDU and scan response PDU.
 *
 * For connectable advertising events no scan request is allowed. In this case
 * we just need to receive a connect request PDU: IFS + CONNECT_REQ = 150 + 352.
 * Note: worst-case is 376 + 150 + 352 = 878 usecs
 *
 * NOTE: The advertising PDU transmit time is NOT included here since we know
 * how long that will take (worst-case is 376 usecs).
 */
#define BLE_LL_SCHED_ADV_MAX_USECS          (852)
#define BLE_LL_SCHED_DIRECT_ADV_MAX_USECS   (502)
#define BLE_LL_SCHED_MAX_ADV_PDU_USECS      (376)

/*
 * This is the offset from the start of the scheduled item until the actual
 * tx/rx should occur, in ticks.
 */
extern uint8_t g_ble_ll_sched_offset_ticks;

/*
 * This is the number of slots needed to transmit and receive a maximum
 * size PDU, including an IFS time before each. The actual time is
 * 2120 usecs for tx/rx and 150 for IFS = 4540 usecs.
 */
#define BLE_LL_SCHED_MAX_TXRX_SLOT  (4 * BLE_LL_SCHED_USECS_PER_SLOT)

/* BLE scheduler errors */
#define BLE_LL_SCHED_ERR_OVERLAP    (1)

/* Types of scheduler events */
#define BLE_LL_SCHED_TYPE_ADV       (1)
#define BLE_LL_SCHED_TYPE_SCAN      (2)
#define BLE_LL_SCHED_TYPE_CONN      (3)
#define BLE_LL_SCHED_TYPE_AUX_SCAN  (4)
#define BLE_LL_SCHED_TYPE_DTM       (5)
#define BLE_LL_SCHED_TYPE_PERIODIC  (6)
#define BLE_LL_SCHED_TYPE_SYNC      (7)

/* Return values for schedule callback. */
#define BLE_LL_SCHED_STATE_RUNNING  (0)
#define BLE_LL_SCHED_STATE_DONE     (1)

/* Callback function */
struct ble_ll_sched_item;
typedef int (*sched_cb_func)(struct ble_ll_sched_item *sch);
typedef void (*sched_remove_cb_func)(struct ble_ll_sched_item *sch);
/*
 * Strict connection scheduling (for the master) is different than how
 * connections are normally scheduled. With strict connection scheduling we
 * introduce the concept of a "period". A period is a collection of slots. Each
 * slot is 1.25 msecs in length. The number of slots in a period is determined
 * by the syscfg value BLE_LL_CONN_INIT_SLOTS. A collection of periods is called
 * an epoch. The length of an epoch is determined by the number of connections
 * (BLE_MAX_CONNECTIONS plus BLE_LL_ADD_STRICT_SCHED_PERIODS). Connections
 * will be scheduled at period boundaries. Any scanning/initiating/advertising
 * will be done in unused periods, if possible.
 */
#if MYNEWT_VAL(BLE_LL_STRICT_CONN_SCHEDULING)
#define BLE_LL_SCHED_PERIODS    (MYNEWT_VAL(BLE_MAX_CONNECTIONS) + \
                                 MYNEWT_VAL(BLE_LL_ADD_STRICT_SCHED_PERIODS))

struct ble_ll_sched_obj
{
    uint8_t sch_num_occ_periods;
    uint32_t sch_occ_period_mask;
    uint32_t sch_ticks_per_period;
    uint32_t sch_ticks_per_epoch;
    uint32_t sch_epoch_start;
};

extern struct ble_ll_sched_obj g_ble_ll_sched_data;

/*
 * XXX: TODO:
 * -> How do we know epoch start is up to date? Not wrapped?
 * -> for now, only do this with no more than 32 connections.
 * -> Do not let initiating occur if no empty sched slots
 */
#endif

/*
 * Schedule item
 *  sched_type: This is the type of the schedule item.
 *  enqueued: Flag denoting if item is on the scheduler list. 0: no, 1:yes
 *  remainder: # of usecs from offset till tx/rx should occur
 *  txrx_offset: Number of ticks from start time until tx/rx should occur.
 *
 */
struct ble_ll_sched_item
{
    uint8_t         sched_type;
    uint8_t         enqueued;
    uint8_t         remainder;
    uint32_t        start_time;
    uint32_t        end_time;
    void            *cb_arg;
    sched_cb_func   sched_cb;
    TAILQ_ENTRY(ble_ll_sched_item) link;
};

/* Initialize the scheduler */
int ble_ll_sched_init(void);

/* Remove item(s) from schedule */
int ble_ll_sched_rmv_elem(struct ble_ll_sched_item *sch);

void ble_ll_sched_rmv_elem_type(uint8_t type, sched_remove_cb_func remove_cb);

/* Schedule a new master connection */
struct ble_ll_conn_sm;
int ble_ll_sched_master_new(struct ble_ll_conn_sm *connsm,
                            struct ble_mbuf_hdr *ble_hdr, uint8_t pyld_len);

/* Schedule a new slave connection */
int ble_ll_sched_slave_new(struct ble_ll_conn_sm *connsm);

struct ble_ll_adv_sm;
typedef void ble_ll_sched_adv_new_cb(struct ble_ll_adv_sm *advsm,
                                     uint32_t sch_start, void *arg);

/* Schedule a new advertising event */
int ble_ll_sched_adv_new(struct ble_ll_sched_item *sch,
                         ble_ll_sched_adv_new_cb cb, void *arg);

/* Schedule periodic advertising event */
int ble_ll_sched_periodic_adv(struct ble_ll_sched_item *sch, uint32_t *start,
                              bool after_overlap);

int ble_ll_sched_sync_reschedule(struct ble_ll_sched_item *sch,
                                 uint32_t anchor_point,
                                 uint8_t anchor_point_usecs,
                                 uint32_t window_widening, int8_t phy_mode);
int ble_ll_sched_sync(struct ble_ll_sched_item *sch,
                      uint32_t beg_cputime, uint32_t rem_usecs, uint32_t offset,
                      int8_t phy_mode);

/* Reschedule an advertising event */
int ble_ll_sched_adv_reschedule(struct ble_ll_sched_item *sch, uint32_t *start,
                                uint32_t max_delay_ticks);

/* Reschedule and advertising pdu */
int ble_ll_sched_adv_resched_pdu(struct ble_ll_sched_item *sch);

/* Reschedule a connection that had previously been scheduled or that is over */
int ble_ll_sched_conn_reschedule(struct ble_ll_conn_sm * connsm);

/**
 * Called to determine when the next scheduled event will occur.
 *
 * If there are not scheduled events this function returns 0; otherwise it
 * returns 1 and *next_event_time is set to the start time of the next event.
 *
 * @param next_event_time cputime at which next scheduled event will occur
 *
 * @return int 0: No events are scheduled 1: there is an upcoming event
 */
int ble_ll_sched_next_time(uint32_t *next_event_time);

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
struct ble_ll_scan_sm;
struct ble_ll_aux_data;
int ble_ll_sched_aux_scan(struct ble_mbuf_hdr *ble_hdr,
                          struct ble_ll_scan_sm *scansm,
                          struct ble_ll_aux_data *aux_scan);

int ble_ll_sched_scan_req_over_aux_ptr(uint32_t chan, uint8_t phy_mode);
#endif

/* Stop the scheduler */
void ble_ll_sched_stop(void);

#if MYNEWT_VAL(BLE_LL_DTM)
int ble_ll_sched_dtm(struct ble_ll_sched_item *sch);
#endif

#ifdef __cplusplus
}
#endif

#endif /* H_LL_SCHED_ */
