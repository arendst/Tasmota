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

#include "host/ble_hs.h"

#ifdef __cplusplus
extern "C" {
#endif

#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
/* Called to configure local(own) privacy (RPA) when using host based privacy. In
 * Host based privacy as controller is not aware of RPA, we do it via
 * 'BLE_ADDR_RANDOM' addr_type route.
 *
 * @param                enable RPA when enable is not 0
 *                       disable RPA otherwise
 *
 * @return               return 0 when successful.
 *                       return appropriate error code otherwise
 */
int ble_hs_pvcy_rpa_config(uint8_t enable);
#endif
