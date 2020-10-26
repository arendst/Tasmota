/**
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

#ifndef H_BLE_SVC_BAS_
#define H_BLE_SVC_BAS_

/* 16 Bit Battery Service UUID */
#define BLE_SVC_BAS_UUID16                                   0x180F

/* 16 Bit Battery Service Characteristic UUIDs */
#define BLE_SVC_BAS_CHR_UUID16_BATTERY_LEVEL                 0x2A19

int ble_svc_bas_battery_level_set(uint8_t level);

#endif
