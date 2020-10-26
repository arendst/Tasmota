/*
 * Copyright 2020 Espressif Systems (Shanghai) PTE LTD
 *
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

#ifndef H_BLE_HS_PVCY_
#define H_BLE_HS_PVCY_

#include "host/ble_hs.h"

#ifdef __cplusplus
extern "C" {
#endif

#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)

#define NIMBLE_HOST_DISABLE_PRIVACY            0x00
#define NIMBLE_HOST_ENABLE_RPA                 0x01
#define NIMBLE_HOST_ENABLE_NRPA                0x02

/* Called to configure local(own) privacy (RPA/NRPA) when using Host based privacy.
 * In Host based privacy, as controller is not aware of RPA/NRPA address is in use,
 * we do it through 'BLE_ADDR_RANDOM (0x01)' addr_type route. This is necessary
 * so as to set the private address as random address in controller.
 * Remember to configure `BLE_SM_PAIR_KEY_DIST_ID` in our & their
 * key distributions for using RPA. For NRPA part of privacy it is not
 * necessary to configure key distributions in host, as anyway NRPA is non-resolvable.
 * Please call this API once host-controller are synced as we set the private
 * (RPA/NRPA) address using host-controller HCI commands.
 *
 * To give brief information on how to use this feature,
 * please refer to following steps while using RPA feature:
 *
 * 1. Include "host/ble_hs_pvcy.h".
 * 2. Set own_addr_type to `BLE_OWN_ADDR_RANDOM`.
 * 3. Add `BLE_SM_PAIR_KEY_DIST_ID` to key distribution in
 *    `ble_hs_cfg.sm_our_key_dist` & `ble_hs_cfg.sm_their_key_dist`.
 * 4. Call `ble_hs_pvcy_rpa_config(1)` in Host-Controller sync callback.
 *
 * In case of NRPA, steps 1, 2 and calling ble_hs_pvcy_rpa_config(2) will
 * suffice.
 *
 * @param                enable RPA when param      = 1 (NIMBLE_HOST_ENABLE_RPA)
 *                       enable NRPA when param     = 2 (NIMBLE_HOST_ENABLE_NRPA)
 *                       disable privacy when param = 0 (NIMBLE_HOST_DISABLE_PRIVACY)
 *
 * @return               return 0 when successful.
 *                       return appropriate error code otherwise
 */
int ble_hs_pvcy_rpa_config(uint8_t enable);
#endif

#ifdef __cplusplus
}
#endif

#endif
