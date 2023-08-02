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

#ifndef H_BLE_HS_MBUF_PRIV_
#define H_BLE_HS_MBUF_PRIV_

#ifdef __cplusplus
extern "C" {
#endif

struct os_mbuf;

struct os_mbuf *ble_hs_mbuf_bare_pkt(void);
struct os_mbuf *ble_hs_mbuf_acl_pkt(void);
struct os_mbuf *ble_hs_mbuf_l2cap_pkt(void);
int ble_hs_mbuf_pullup_base(struct os_mbuf **om, int base_len);

#ifdef __cplusplus
}
#endif

#endif
