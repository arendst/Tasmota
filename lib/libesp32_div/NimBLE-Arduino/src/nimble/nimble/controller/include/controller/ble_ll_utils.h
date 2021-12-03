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

#include <stdint.h>

uint32_t ble_ll_utils_calc_access_addr(void);
uint8_t ble_ll_utils_remapped_channel(uint8_t remap_index, const uint8_t *chanmap);
uint8_t ble_ll_utils_calc_dci_csa2(uint16_t event_cntr, uint16_t channel_id,
                                   uint8_t num_used_chans, const uint8_t *chanmap);
uint8_t ble_ll_utils_calc_num_used_chans(const uint8_t *chanmap);
uint32_t ble_ll_utils_calc_window_widening(uint32_t anchor_point,
                                           uint32_t last_anchor_point,
                                           uint8_t master_sca);
