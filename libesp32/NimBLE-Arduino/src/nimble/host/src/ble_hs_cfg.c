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

#include "syscfg/syscfg.h"
#include "host/ble_hs.h"

struct ble_hs_cfg ble_hs_cfg = {
    /** Security manager settings. */
    .sm_io_cap = MYNEWT_VAL(BLE_SM_IO_CAP),
    .sm_oob_data_flag = MYNEWT_VAL(BLE_SM_OOB_DATA_FLAG),
    .sm_bonding = MYNEWT_VAL(BLE_SM_BONDING),
    .sm_mitm = MYNEWT_VAL(BLE_SM_MITM),
    .sm_sc = MYNEWT_VAL(BLE_SM_SC),
    .sm_keypress = MYNEWT_VAL(BLE_SM_KEYPRESS),
    .sm_our_key_dist = MYNEWT_VAL(BLE_SM_OUR_KEY_DIST),
    .sm_their_key_dist = MYNEWT_VAL(BLE_SM_THEIR_KEY_DIST),
};
