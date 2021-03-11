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

#ifndef H_BLE_HS_HCI_
#define H_BLE_HS_HCI_

/**
 * @brief Bluetooth Host HCI utils
 * @defgroup bt_host_hci Bluetooth Host HCI utils
 * @ingroup bt_host
 * @{
 */

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Queries the controller for the channel map used with the specified
 * connection.  The channel map is represented as an array of five bytes, with
 * each bit corresponding to an individual channel.  The array is interpreted
 * as little-endian, such that:
 *     map[0] & 0x01 --> Channel 0.
 *     map[0] & 0x02 --> Channel 1.
 *     ...
 *     map[1] & 0x01 --> Channel 8.
 *
 * As there are 37 channels, only the first 37 bits get written.
 *
 * If a bit is 1, the corresponding channel is used.  Otherwise, the channel is
 * unused.
 *
 * @param conn_handle           The handle of the connection whose channel map
 *                                  is being read.
 * @param out_chan_map          On success, the retrieved channel map gets
 *                                  written here.  This buffer must have a size
 *                                  >= 5 bytes.
 *
 * @return                      0 on success;
 *                              A BLE host HCI return code if the controller
 *                                  rejected the request;
 *                              A BLE host core return code on unexpected
 *                                  error.
 */
int ble_hs_hci_read_chan_map(uint16_t conn_handle, uint8_t *out_chan_map);

/**
 * Instructs the controller to use the specified channel map.  The channel map
 * is represented as an array of five bytes, with each bit corresponding to an
 * individual channel.  The array is interpreted as little-endian, such that:
 *     map[0] & 0x01 --> Channel 0.
 *     map[0] & 0x02 --> Channel 1.
 *     ...
 *     map[1] & 0x01 --> Channel 8.
 *
 * As there are 37 channels, only the first 37 bits should be written are used.
 *
 * If a bit is 1, the corresponding channel can be used.  Otherwise, the
 * channel should not be used.
 *
 * @param chan_map              The channel map to configure.  This buffer
 *                                  should have a size of 5 bytes.
 *
 * @return                      0 on success;
 *                              A BLE host HCI return code if the controller
 *                                  rejected the request;
 *                              A BLE host core return code on unexpected
 *                                  error.
 */
int ble_hs_hci_set_chan_class(const uint8_t *chan_map);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif
