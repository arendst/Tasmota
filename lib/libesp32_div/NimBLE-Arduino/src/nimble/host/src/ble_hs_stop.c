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
#include "nimble/nimble_npl.h"
#ifndef MYNEWT
#include "nimble/nimble_port.h"
#endif

#define BLE_HOST_STOP_TIMEOUT_MS MYNEWT_VAL(BLE_HS_STOP_ON_SHUTDOWN_TIMEOUT)

static struct ble_gap_event_listener ble_hs_stop_gap_listener;

/**
 * List of stop listeners.  These are notified when a stop procedure completes.
 */
SLIST_HEAD(ble_hs_stop_listener_slist, ble_hs_stop_listener);
static struct ble_hs_stop_listener_slist ble_hs_stop_listeners;

/* Track number of connections */
static uint8_t ble_hs_stop_conn_cnt;

static struct ble_npl_callout ble_hs_stop_terminate_tmo;

/**
 * Called when a stop procedure has completed.
 */
static void
ble_hs_stop_done(int status)
{
    struct ble_hs_stop_listener_slist slist;
    struct ble_hs_stop_listener *listener;

    ble_npl_callout_stop(&ble_hs_stop_terminate_tmo);

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
        rc = ble_gap_periodic_adv_sync_terminate(sync_handle);
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
 * Terminates connection.
 */
static int
ble_hs_stop_terminate_conn(struct ble_hs_conn *conn, void *arg)
{
    int rc;

    rc = ble_gap_terminate_with_conn(conn, BLE_ERR_REM_USER_CONN_TERM);
    if (rc == 0) {
        /* Terminate procedure successfully initiated.  Let the GAP event
         * handler deal with the result.
         */
        ble_hs_stop_conn_cnt++;
    } else {
        /* If failed, just make sure we are not going to wait for connection complete event,
         * just count it as already disconnected
         */
        BLE_HS_LOG(ERROR, "ble_hs_stop: failed to terminate connection; rc=%d\n", rc);
    }

    return 0;
}

/**
 * This is called when host graceful disconnect timeout fires. That means some devices
 * are out of range and disconnection completed did no happen yet.
 */
static void
ble_hs_stop_terminate_timeout_cb(struct ble_npl_event *ev)
{
    BLE_HS_LOG(ERROR, "ble_hs_stop_terminate_timeout_cb,"
                      "%d connection(s) still up \n", ble_hs_stop_conn_cnt);

    /* TODO: Shall we send error here? */
    ble_hs_stop_done(0);
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

        ble_hs_stop_conn_cnt--;

        if (ble_hs_stop_conn_cnt == 0) {
            ble_hs_stop_done(0);
        }
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

#if MYNEWT_VAL(BLE_PERIODIC_ADV)
    /* Check for active periodic sync first and terminate it all */
    rc = ble_hs_stop_terminate_all_periodic_sync();
    if (rc != 0) {
        return rc;
    }
#endif

    rc = ble_gap_event_listener_register(&ble_hs_stop_gap_listener,
                                         ble_hs_stop_gap_event, NULL);
    if (rc != 0) {
        return rc;
    }

    ble_hs_lock();
    ble_hs_conn_foreach(ble_hs_stop_terminate_conn, NULL);
    ble_hs_unlock();

    if (ble_hs_stop_conn_cnt > 0) {
        ble_npl_callout_reset(&ble_hs_stop_terminate_tmo,
                              ble_npl_time_ms_to_ticks32(BLE_HOST_STOP_TIMEOUT_MS));
    } else {
        /* No connections, stop is completed */
        ble_hs_stop_done(0);
    }

    return 0;
}

void
ble_hs_stop_init(void)
{
#ifdef MYNEWT
    ble_npl_callout_init(&ble_hs_stop_terminate_tmo, ble_npl_eventq_dflt_get(),
                         ble_hs_stop_terminate_timeout_cb, NULL);
#else
    ble_npl_callout_init(&ble_hs_stop_terminate_tmo, nimble_port_get_dflt_eventq(),
                         ble_hs_stop_terminate_timeout_cb, NULL);
#endif
}
