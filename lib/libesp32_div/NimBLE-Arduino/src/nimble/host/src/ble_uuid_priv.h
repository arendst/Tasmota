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

#ifndef H_BLE_UUID_PRIV_
#define H_BLE_UUID_PRIV_

#include "host/ble_uuid.h"

#ifdef __cplusplus
extern "C" {
#endif

struct os_mbuf;

int ble_uuid_init_from_att_mbuf(ble_uuid_any_t *uuid, struct os_mbuf *om,
                                int off, int len);
int ble_uuid_init_from_att_buf(ble_uuid_any_t *uuid, const void *buf,
                               size_t len);

int ble_uuid_to_any(const ble_uuid_t *uuid, ble_uuid_any_t *uuid_any);
int ble_uuid_to_mbuf(const ble_uuid_t *uuid, struct os_mbuf *om);
int ble_uuid_flat(const ble_uuid_t *uuid, void *dst);
int ble_uuid_length(const ble_uuid_t *uuid);

#ifdef __cplusplus
}
#endif

#endif
