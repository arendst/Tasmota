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

#ifndef H_BLE_HS_STOP_
#define H_BLE_HS_STOP_

/** @typedef ble_hs_stop_fn
 * @brief Callback function; reports the result of a host stop procedure.
 *
 * @param status                The result of the host stop procedure.  One of
 *                                  the HAL_RESET_[...] codes or an
 *                                  implementation-defined value.
 * @param arg                   Optional argument specified when the stop
 *                                  procedure was initiated.
 *
 */
typedef void ble_hs_stop_fn(int status, void *arg);

/**
 * @brief Used to report the result of a stop procedure.
 *
 * This should be used as an opaque structure and not modified manually.
 */
struct ble_hs_stop_listener {
    ble_hs_stop_fn *fn;
    void *arg;
    SLIST_ENTRY(ble_hs_stop_listener) link;
};

/**
 * @brief Stops the BLE host.
 *
 * Aborts all active GAP procedures and terminates all open connections.
 * Connection termination is performed asynchronously, so this function's
 * result is reported via the provided listener.
 *
 * @param listener              A listener to populate.  This object's initial
 *                                  value doesn't matter, but its lifetime must
 *                                  extend until the stop procedure completes.
 * @param fn                    The callback to execute when the stop procedure
 *                                  completes.
 * @param arg                   Optional argument to pass to the callback.
 *
 * @return                      0: Stop procedure successfully initiated.
 *                              BLE_HS_EBUSY: Stop procedure already in
 *                                  progress; the provided callback gets called
 *                                  when the procedure completes.
 *                              BLE_HS_EALREADY: Host already stopped; the
 *                                  provided callback does *not* get called.
 */
int ble_hs_stop(struct ble_hs_stop_listener *listener,
                ble_hs_stop_fn *fn, void *arg);

#endif
