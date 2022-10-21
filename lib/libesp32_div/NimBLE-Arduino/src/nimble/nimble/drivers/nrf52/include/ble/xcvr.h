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

#if defined(ARDUINO_ARCH_NRF5) && defined(NRF52_SERIES)

#ifndef H_BLE_XCVR_
#define H_BLE_XCVR_

#ifdef __cplusplus
extern "C" {
#endif

#define XCVR_RX_RADIO_RAMPUP_USECS  (40)
#define XCVR_TX_RADIO_RAMPUP_USECS  (40)

/*
 * NOTE: we have to account for the RTC output compare issue. We want it to be
 * 5 ticks.
 */
#define XCVR_PROC_DELAY_USECS         (153)
#define XCVR_RX_START_DELAY_USECS     (XCVR_RX_RADIO_RAMPUP_USECS)
#define XCVR_TX_START_DELAY_USECS     (XCVR_TX_RADIO_RAMPUP_USECS)
#define XCVR_TX_SCHED_DELAY_USECS     \
    (XCVR_TX_START_DELAY_USECS + XCVR_PROC_DELAY_USECS)
#define XCVR_RX_SCHED_DELAY_USECS     \
    (XCVR_RX_START_DELAY_USECS + XCVR_PROC_DELAY_USECS)

/*
 * Define HW whitelist size. This is the total possible whitelist size;
 * not necessarily the size that will be used (may be smaller)
 */
#define BLE_HW_WHITE_LIST_SIZE        (8)

#ifdef __cplusplus
}
#endif

#endif /* H_BLE_XCVR_ */
#endif
