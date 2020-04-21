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

#ifndef H_HOST_UTIL_
#define H_HOST_UTIL_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Tries to configure the device with at least one Bluetooth address.
 * Addresses are restored in a hardware-specific fashion.
 *
 * @param prefer_random         Whether to attempt to restore a random address
 *                                  before checking if a public address has
 *                                  already been configured.
 *
 * @return                      0 on success;
 *                              BLE_HS_ENOADDR if the device does not have any
 *                                  available addresses.
 *                              Other BLE host core code on error.
 */
int ble_hs_util_ensure_addr(int prefer_random);

#ifdef __cplusplus
}
#endif

#endif
