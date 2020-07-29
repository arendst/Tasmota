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

#ifndef H_BLE_HS_PRIV_
#define H_BLE_HS_PRIV_

#include <assert.h>
#include <inttypes.h>
#include "ble_att_cmd_priv.h"
#include "ble_att_priv.h"
#include "ble_gap_priv.h"
#include "ble_gatt_priv.h"
#include "ble_hs_dbg_priv.h"
#include "ble_hs_hci_priv.h"
#include "ble_hs_atomic_priv.h"
#include "ble_hs_conn_priv.h"
#include "ble_hs_atomic_priv.h"
#include "ble_hs_mbuf_priv.h"
#include "ble_hs_startup_priv.h"
#include "ble_l2cap_priv.h"
#include "ble_l2cap_sig_priv.h"
#include "ble_l2cap_coc_priv.h"
#include "ble_sm_priv.h"
#include "ble_hs_adv_priv.h"
#include "ble_hs_flow_priv.h"
#include "ble_hs_pvcy_priv.h"
#include "ble_hs_id_priv.h"
#include "ble_hs_periodic_sync_priv.h"
#include "ble_uuid_priv.h"
#include "host/ble_hs.h"
#include "host/ble_monitor.h"
#include "nimble/nimble_opt.h"
#include "stats/stats.h"
#ifdef __cplusplus
extern "C" {
#endif

struct ble_hs_conn;
struct ble_l2cap_chan;
struct os_mbuf;
struct os_mempool;
struct os_event;

#define BLE_HS_SYNC_STATE_BAD           0
#define BLE_HS_SYNC_STATE_BRINGUP       1
#define BLE_HS_SYNC_STATE_GOOD          2

#define BLE_HS_ENABLED_STATE_OFF        0
#define BLE_HS_ENABLED_STATE_STOPPING   1
#define BLE_HS_ENABLED_STATE_ON         2

#if NIMBLE_BLE_CONNECT
#define BLE_HS_MAX_CONNECTIONS MYNEWT_VAL(BLE_MAX_CONNECTIONS)
#else
#define BLE_HS_MAX_CONNECTIONS 0
#endif

#if !MYNEWT_VAL(BLE_ATT_SVR_QUEUED_WRITE)
#define BLE_HS_ATT_SVR_QUEUED_WRITE_TMO 0
#else
#define BLE_HS_ATT_SVR_QUEUED_WRITE_TMO \
    MYNEWT_VAL(BLE_ATT_SVR_QUEUED_WRITE_TMO)
#endif

STATS_SECT_START(ble_hs_stats)
    STATS_SECT_ENTRY(conn_create)
    STATS_SECT_ENTRY(conn_delete)
    STATS_SECT_ENTRY(hci_cmd)
    STATS_SECT_ENTRY(hci_event)
    STATS_SECT_ENTRY(hci_invalid_ack)
    STATS_SECT_ENTRY(hci_unknown_event)
    STATS_SECT_ENTRY(hci_timeout)
    STATS_SECT_ENTRY(reset)
    STATS_SECT_ENTRY(sync)
    STATS_SECT_ENTRY(pvcy_add_entry)
    STATS_SECT_ENTRY(pvcy_add_entry_fail)
STATS_SECT_END
extern STATS_SECT_DECL(ble_hs_stats) ble_hs_stats;

extern struct os_mbuf_pool ble_hs_mbuf_pool;
extern uint8_t ble_hs_sync_state;
extern uint8_t ble_hs_enabled_state;

extern const uint8_t ble_hs_misc_null_addr[6];

extern uint16_t ble_hs_max_attrs;
extern uint16_t ble_hs_max_services;
extern uint16_t ble_hs_max_client_configs;

void ble_hs_process_rx_data_queue(void);
int ble_hs_tx_data(struct os_mbuf *om);
void ble_hs_wakeup_tx(void);
void ble_hs_enqueue_hci_event(uint8_t *hci_evt);
void ble_hs_event_enqueue(struct os_event *ev);

int ble_hs_hci_rx_evt(uint8_t *hci_ev, void *arg);
int ble_hs_hci_evt_acl_process(struct os_mbuf *om);

int ble_hs_misc_conn_chan_find(uint16_t conn_handle, uint16_t cid,
                               struct ble_hs_conn **out_conn,
                               struct ble_l2cap_chan **out_chan);
void ble_hs_misc_conn_chan_find_reqd(uint16_t conn_handle, uint16_t cid,
                                     struct ble_hs_conn **out_conn,
                                     struct ble_l2cap_chan **out_chan);
uint8_t ble_hs_misc_addr_type_to_id(uint8_t addr_type);
int ble_hs_misc_restore_irks(void);

int ble_hs_locked_by_cur_task(void);
int ble_hs_is_parent_task(void);
void ble_hs_lock_nested(void);
void ble_hs_unlock_nested(void);
void ble_hs_lock(void);
void ble_hs_unlock(void);
void ble_hs_hw_error(uint8_t hw_code);
void ble_hs_timer_resched(void);
void ble_hs_notifications_sched(void);
struct ble_npl_eventq *ble_hs_evq_get(void);
void ble_hs_stop_init(void);

struct ble_mqueue {
    STAILQ_HEAD(, os_mbuf_pkthdr) head;
    struct ble_npl_event ev;
};

int ble_mqueue_init(struct ble_mqueue *mq, ble_npl_event_fn *ev_fn, void *ev_arg);
struct os_mbuf *ble_mqueue_get(struct ble_mqueue *mq);
int ble_mqueue_put(struct ble_mqueue *mq, struct ble_npl_eventq *evq, struct os_mbuf *om);

#if MYNEWT_VAL(LOG_LEVEL) <= LOG_LEVEL_DEBUG && !BLE_MONITOR

#define BLE_HS_LOG_CMD(is_tx, cmd_type, cmd_name, conn_handle,                \
                       log_cb, cmd) do                                        \
{                                                                             \
    BLE_HS_LOG(DEBUG, "%sed %s command: %s; conn=%d ",                        \
               (is_tx) ? "tx" : "rx", (cmd_type), (cmd_name), (conn_handle)); \
    (log_cb)(cmd);                                                            \
    BLE_HS_LOG(DEBUG, "\n");                                                  \
} while (0)

#define BLE_HS_LOG_EMPTY_CMD(is_tx, cmd_type, cmd_name, conn_handle) do       \
{                                                                             \
    BLE_HS_LOG(DEBUG, "%sed %s command: %s; conn=%d ",                        \
               (is_tx) ? "tx" : "rx", (cmd_type), (cmd_name), (conn_handle)); \
    BLE_HS_LOG(DEBUG, "\n");                                                  \
} while (0)

#else

#define BLE_HS_LOG_CMD(is_tx, cmd_type, cmd_name, conn_handle, log_cb, cmd)
#define BLE_HS_LOG_EMPTY_CMD(is_tx, cmd_type, cmd_name, conn_handle)

#endif

#if MYNEWT_VAL(BLE_HS_DEBUG)
    #define BLE_HS_DBG_ASSERT(x) assert(x)
    #define BLE_HS_DBG_ASSERT_EVAL(x) assert(x)
#else
    #define BLE_HS_DBG_ASSERT(x)
    #define BLE_HS_DBG_ASSERT_EVAL(x) ((void)(x))
#endif

#ifdef __cplusplus
}
#endif

#endif
