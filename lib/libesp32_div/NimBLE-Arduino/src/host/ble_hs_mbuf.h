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

#ifndef H_BLE_HS_MBUF_
#define H_BLE_HS_MBUF_

/**
 * @brief Bluetooth Host chained memory buffer (mbuf)
 * @defgroup bt_host_mbuf Bluetooth Host chained memory buffer (mbuf)
 * @ingroup bt_host
 * @{
 */

#include <inttypes.h>
#ifdef __cplusplus
extern "C" {
#endif

struct os_mbuf;

/**
 * Allocates an mbuf suitable for an ATT command packet.  The resulting packet
 * has sufficient leading space for:
 *  - ACL data header
 *  - L2CAP B-frame header
 *  - Largest ATT command base (prepare write request / response).
 *
 * @return An empty mbuf on success, NULL on error.
 */
struct os_mbuf *ble_hs_mbuf_att_pkt(void);

/**
 * Allocates an mbuf and fills it with the contents of the specified flat
 * buffer.
 *
 * @param buf The flat buffer to copy from.
 * @param len The length of the flat buffer.
 *
 * @return A newly-allocated mbuf on success, NULL on error.
 */
struct os_mbuf *ble_hs_mbuf_from_flat(const void *buf, uint16_t len);

/**
 * Copies the contents of an mbuf into the specified flat buffer.  If the flat
 * buffer is too small to contain the mbuf's contents, it is filled to capacity
 * and BLE_HS_EMSGSIZE is returned.
 *
 * @param om            The mbuf to copy from.
 * @param flat          The destination flat buffer.
 * @param max_len       The size of the flat buffer.
 * @param out_copy_len  The number of bytes actually copied gets written here.
 *
 * @return               0 on success or BLE host core return code on error.
 */
int ble_hs_mbuf_to_flat(const struct os_mbuf *om, void *flat, uint16_t max_len,
                        uint16_t *out_copy_len);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif
