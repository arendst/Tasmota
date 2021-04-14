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

#if MYNEWT

#include "os/mynewt.h"
#include "ble_hs_priv.h"

static struct ble_hs_stop_listener ble_hs_shutdown_stop_listener;

/**
 * Called when the host stop procedure has completed.
 */
static void
ble_hs_shutdown_stop_cb(int status, void *arg)
{
    SYSDOWN_ASSERT_ACTIVE();

    /* Indicate to sysdown that the host is fully shut down. */
    sysdown_release();
}

int
ble_hs_shutdown(int reason)
{
    int rc;

    /* Ensure this function only gets called by sysdown. */
    SYSDOWN_ASSERT_ACTIVE();

    /* Initiate a host stop procedure. */
    rc = ble_hs_stop(&ble_hs_shutdown_stop_listener, ble_hs_shutdown_stop_cb,
                     NULL);
    switch (rc) {
    case 0:
        /* Stop initiated.  Wait for result to be reported asynchronously. */
        return SYSDOWN_IN_PROGRESS;

    case BLE_HS_EBUSY:
        /* Already stopping.  Wait for result to be reported asynchronously. */
        return SYSDOWN_IN_PROGRESS;

    case BLE_HS_EALREADY:
        /* Already stopped.  Shutdown complete. */
        return SYSDOWN_COMPLETE;

    default:
        BLE_HS_LOG(ERROR, "ble_hs_shutdown: failed to stop host; rc=%d\n", rc);
        return SYSDOWN_COMPLETE;
    }
}

#endif
