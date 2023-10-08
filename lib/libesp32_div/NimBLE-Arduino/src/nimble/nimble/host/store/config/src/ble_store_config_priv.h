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

#ifndef H_BLE_STORE_CONFIG_PRIV_
#define H_BLE_STORE_CONFIG_PRIV_

#ifdef __cplusplus
extern "C" {
#endif

extern struct ble_store_value_sec
    ble_store_config_our_secs[MYNEWT_VAL(BLE_STORE_MAX_BONDS)];
extern int ble_store_config_num_our_secs;

extern struct ble_store_value_sec
    ble_store_config_peer_secs[MYNEWT_VAL(BLE_STORE_MAX_BONDS)];
extern int ble_store_config_num_peer_secs;

extern struct ble_store_value_cccd
    ble_store_config_cccds[MYNEWT_VAL(BLE_STORE_MAX_CCCDS)];
extern int ble_store_config_num_cccds;

#if MYNEWT_VAL(BLE_STORE_CONFIG_PERSIST)

int ble_store_config_persist_our_secs(void);
int ble_store_config_persist_peer_secs(void);
int ble_store_config_persist_cccds(void);
void ble_store_config_conf_init(void);

#else

static inline int ble_store_config_persist_our_secs(void)   { return 0; }
static inline int ble_store_config_persist_peer_secs(void)  { return 0; }
static inline int ble_store_config_persist_cccds(void)      { return 0; }
static inline void ble_store_config_conf_init(void)         { }

#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
static inline int ble_store_persist_peer_records(void)      { return 0; }
#endif
#endif /* MYNEWT_VAL(BLE_STORE_CONFIG_PERSIST) */

#ifdef __cplusplus
}
#endif

#endif
