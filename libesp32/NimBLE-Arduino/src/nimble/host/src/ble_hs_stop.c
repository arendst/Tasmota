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

#include <assert.h>
#include "sysinit/sysinit.h"
#include "syscfg/syscfg.h"
#include "ble_hs_priv.h"

static ble_npl_event_fn ble_hs_stop_term_event_cb;
static struct ble_npl_event ble_hs_stop_term_ev;

static struct ble_gap_event_listener ble_hs_stop_gap_listener;

/**
 * List of stop listeners.  These are notified when a stop procedure completes.
 */
SLIST_HEAD(ble_hs_stop_listener_slist, ble_hs_stop_listener);
static struct ble_hs_stop_listener_slist ble_hs_stop_listeners;

/**
 * Called when a stop procedure has completed.
 */
static void
ble_hs_stop_done(int status)
{
    struct ble_hs_stop_listener_slist slist;
    struct ble_hs_stop_listener *listener;

    ble_hs_lock();

    ble_gap_event_listener_unregister(&ble_hs_stop_gap_listener);

    slist = ble_hs_stop_listeners;
    SLIST_INIT(&ble_hs_stop_listeners);

    ble_hs_enabled_state = BLE_HS_ENABLED_STATE_OFF;

    ble_hs_unlock();

    SLIST_FOREACH(listener, &slist, link) {
        listener->fn(status, listener->arg);
    }
}

#if MYNEWT_VAL(BLE_PERIODIC_ADV)
/**
 * Terminates all active periodic sync handles
 *
 * If there are no active periodic sync handles, signals completion of the
 * close procedure.
 */
static int
ble_hs_stop_terminate_all_periodic_sync(void)
{
    int rc = 0;
    struct ble_hs_periodic_sync *psync;
    uint16_t sync_handle;

    while((psync = ble_hs_periodic_sync_first())){
        /* Terminate sync command waits a command complete event, so there
         * is no need to wait for GAP event, as the calling thread will be
         * blocked on the hci semaphore until the command complete is received.
         *
         * Also, once the sync is terminated, the psync will be freed and
         * removed from the list such that the next call to
         * ble_hs_periodic_sync_first yields the next psync handle
         */
        sync_handle = psync->sync_handle;
        rc = ble_gap_periodic_adv_terminate_sync(sync_handle);
        if (rc != 0 && rc != BLE_HS_ENOTCONN) {
            BLE_HS_LOG(ERROR, "failed to terminate periodic sync=0x%04x, rc=%d\n",
                       sync_handle, rc);
            return rc;
        }
    }

    return 0;
}
#endif

/**
 * Terminates the first open connection.
 *
 * If there are no open connections, Check for any active periodic sync
 * handles.
 */
static void
ble_hs_stop_terminate_next_conn(void)
{
    uint16_t handle;
    int rc;

    handle = ble_hs_atomic_first_conn_handle();
    if (handle == BLE_HS_CONN_HANDLE_NONE) {
        /* No open connections.  Signal completion of the stop procedure. */
        ble_hs_stop_done(0);
        return;
    }

    rc = ble_gap_terminate(handle, BLE_ERR_REM_USER_CONN_TERM);
    if (rc == 0) {
        /* Terminate procedure successfully initiated.  Let the GAP event
         * handler deal with the result.
         */
    } else {
        BLE_HS_LOG(ERROR,
            "ble_hs_stop: failed to terminate connection; rc=%d\n", rc);
        ble_hs_stop_done(rc);
    }
}

/**
 * Event handler.  Attempts to terminate the first open connection if there is
 * one.  All additional connections are terminated elsewhere in the GAP event
 * handler.
 *
 * If there are no connections, signals completion of the stop procedure.
 */
static void
ble_hs_stop_term_event_cb(struct ble_npl_event *ev)
{
    ble_hs_stop_terminate_next_conn();
}

/**
 * GAP event callback.  Listens for connection termination and then terminates
 * the next one.
 *
 * If there are no connections, signals completion of the stop procedure.
 */
static int
ble_hs_stop_gap_event(struct ble_gap_event *event, void *arg)
{
    /* Only process connection termination events. */
    if (event->type == BLE_GAP_EVENT_DISCONNECT ||
        event->type == BLE_GAP_EVENT_TERM_FAILURE) {

        ble_hs_stop_terminate_next_conn();
    }

    return 0;
}

/**
 * Registers a listener to listen for completion of the current stop procedure.
 */
static void
ble_hs_stop_register_listener(struct ble_hs_stop_listener *listener,
                              ble_hs_stop_fn *fn, void *arg)
{
    BLE_HS_DBG_ASSERT(fn != NULL);

    listener->fn = fn;
    listener->arg = arg;
    SLIST_INSERT_HEAD(&ble_hs_stop_listeners, listener, link);
}

static int
ble_hs_stop_begin(struct ble_hs_stop_listener *listener,
                   ble_hs_stop_fn *fn, void *arg)
{
    switch (ble_hs_enabled_state) {
    case BLE_HS_ENABLED_STATE_ON:
        /* Host is enabled; proceed with the stop procedure. */
        ble_hs_enabled_state = BLE_HS_ENABLED_STATE_STOPPING;
        if (listener != NULL) {
            ble_hs_stop_register_listener(listener, fn, arg);
        }

        /* Put the host in the "stopping" state and ensure the host timer is
         * not running.
         */
        ble_hs_timer_resched();
        return 0;

    case BLE_HS_ENABLED_STATE_STOPPING:
        /* A stop procedure is already in progress.  Just listen for the
         * procedure's completion.
         */
        if (listener != NULL) {
            ble_hs_stop_register_listener(listener, fn, arg);
        }
        return BLE_HS_EBUSY;

    case BLE_HS_ENABLED_STATE_OFF:
        /* Host already stopped. */
        return BLE_HS_EALREADY;

    default:
        assert(0);
        return BLE_HS_EUNKNOWN;
    }
}

int
ble_hs_stop(struct ble_hs_stop_listener *listener, 
            ble_hs_stop_fn *fn, void *arg)
{
    int rc;

    ble_hs_lock();
    rc = ble_hs_stop_begin(listener, fn, arg);
    ble_hs_unlock();

    switch (rc) {
    case 0:
        break;

    case BLE_HS_EBUSY:
        return 0;

    default:
        return rc;
    }

    /* Abort all active GAP procedures. */
    ble_gap_preempt();
    ble_gap_preempt_done();

    rc = ble_gap_event_listener_register(&ble_hs_stop_gap_listener,
                                         ble_hs_stop_gap_event, NULL);
    if (rc != 0) {
        return rc;
    }

#if MYNEWT_VAL(BLE_PERIODIC_ADV)
    /* Check for active periodic sync first and terminate it all */
    rc = ble_hs_stop_terminate_all_periodic_sync();
    if (rc != 0) {
        return rc;
    }
#endif

    /* Schedule termination of all open connections in the host task.  This is
     * done even if there are no open connections so that the result of the
     * stop procedure is signaled in a consistent manner (asynchronously).
     */
    ble_npl_eventq_put(ble_hs_evq_get(), &ble_hs_stop_term_ev);

    return 0;
}

void
ble_hs_stop_init(void)
{
    ble_npl_event_init(&ble_hs_stop_term_ev, ble_hs_stop_term_event_cb, NULL);
}
