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

#ifndef H_BLE_LL_TEST_PRIV_
#define H_BLE_LL_TEST_PRIV_

#include <stdint.h>
#include <stdbool.h>
#include "nimble/ble.h"

int ble_ll_hci_dtm_tx_test(const uint8_t *cmdbuf, uint8_t len);
int ble_ll_hci_dtm_tx_test_v2(const uint8_t *cmdbuf, uint8_t len);

int ble_ll_hci_dtm_rx_test(const uint8_t *cmdbuf, uint8_t len);
int ble_ll_hci_dtm_rx_test_v2(const uint8_t *cmdbuf, uint8_t len);

int ble_ll_dtm_end_test(uint8_t *rsp, uint8_t *rsplen);

int ble_ll_dtm_rx_isr_start(struct ble_mbuf_hdr *rxhdr, uint32_t aa);
int ble_ll_dtm_rx_isr_end(uint8_t *rxbuf, struct ble_mbuf_hdr *rxhdr);
void ble_ll_dtm_rx_pkt_in(struct os_mbuf *rxpdu, struct ble_mbuf_hdr *hdr);
void ble_ll_dtm_wfr_timer_exp(void);
void ble_ll_dtm_reset(void);
#endif
