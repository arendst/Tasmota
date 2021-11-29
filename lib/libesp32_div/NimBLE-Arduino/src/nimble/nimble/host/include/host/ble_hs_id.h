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

#ifndef H_BLE_HS_ID_
#define H_BLE_HS_ID_

/**
 * @brief Bluetooth Host Identity
 * @defgroup bt_host_id Bluetooth Host Identity
 * @ingroup bt_host
 * @{
 */

#include <inttypes.h>
#include "nimble/nimble/include/nimble/ble.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Generates a new random address.  This function does not configure the device
 * with the new address; the caller can use the address in subsequent
 * operations.
 *
 * @param nrpa                  The type of random address to generate:
 *                                  0: static
 *                                  1: non-resolvable private
 * @param out_addr              On success, the generated address gets written
 *                                  here.
 *
 * @return                      0 on success; nonzero on failure.
 */
int ble_hs_id_gen_rnd(int nrpa, ble_addr_t *out_addr);

/**
 * Sets the device's random address.  The address type (static vs.
 * non-resolvable private) is inferred from the most-significant byte of the
 * address.  The address is specified in host byte order (little-endian!).
 *
 * @param rnd_addr              The random address to set.
 *
 * @return                      0 on success;
 *                              BLE_HS_EINVAL if the specified address is not a
 *                                  valid static random or non-resolvable
 *                                  private address.
 *                              Other nonzero on error.
 */
int ble_hs_id_set_rnd(const uint8_t *rnd_addr);

/**
 * Retrieves one of the device's identity addresses.  The device can have two
 * identity addresses: one public and one random.  The id_addr_type argument
 * specifies which of these two addresses to retrieve.
 *
 * @param id_addr_type          The type of identity address to retrieve.
 *                                  Valid values are:
 *                                      o BLE_ADDR_PUBLIC
 *                                      o BLE_ADDR_RANDOM
 * @param out_id_addr           On success, the requested identity address is
 *                                  copied into this buffer.  The buffer must
 *                                  be at least six bytes in size.  Pass NULL
 *                                  if you do not require this information.
 * @param out_is_nrpa           On success, the pointed-to value indicates
 *                                  whether the retrieved address is a
 *                                  non-resolvable private address.  Pass NULL
 *                                  if you do not require this information.
 *
 * @return                      0 on success;
 *                              BLE_HS_EINVAL if an invalid address type was
 *                                  specified;
 *                              BLE_HS_ENOADDR if the device does not have an
 *                                  identity address of the requested type;
 *                              Other BLE host core code on error.
 */
int ble_hs_id_copy_addr(uint8_t id_addr_type, uint8_t *out_id_addr,
                        int *out_is_nrpa);

/**
 * Determines the best address type to use for automatic address type
 * resolution.  Calculation of the best address type is done as follows:
 *
 * if privacy requested:
 *     if we have a random static address:
 *          --> RPA with static random ID
 *     else
 *          --> RPA with public ID
 *     end
 * else
 *     if we have a random static address:
 *          --> random static address
 *     else
 *          --> public address
 *     end
 * end
 *
 * @param privacy               (0/1) Whether to use a private address.
 * @param out_addr_type         On success, the "own addr type" code gets
 *                                  written here.
 *
 * @return                      0 if an address type was successfully inferred.
 *                              BLE_HS_ENOADDR if the device does not have a
 *                                  suitable address.
 *                              Other BLE host core code on error.
 */
int ble_hs_id_infer_auto(int privacy, uint8_t *out_addr_type);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif
