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

#ifndef H_L2CAP_COC_PRIV_
#define H_L2CAP_COC_PRIV_

#include <inttypes.h>
#include "syscfg/syscfg.h"
#include "os/queue.h"
#include "os/os_mbuf.h"
#include "host/ble_l2cap.h"
#include "ble_l2cap_sig_priv.h"
#ifdef __cplusplus
extern "C" {
#endif

#define BLE_L2CAP_COC_CID_START                 0x0040
#define BLE_L2CAP_COC_CID_END                   0x007F

struct ble_l2cap_chan;

#define BLE_L2CAP_COC_FLAG_STALLED              0x01

struct ble_l2cap_coc_endpoint {
    struct os_mbuf *sdu;
    uint16_t mtu;
    uint16_t credits;
    uint16_t data_offset;
    uint8_t flags;
};

struct ble_l2cap_coc_srv {
    STAILQ_ENTRY(ble_l2cap_coc_srv) next;
    uint16_t psm;
    uint16_t mtu;
    ble_l2cap_event_fn *cb;
    void *cb_arg;
};

#if MYNEWT_VAL(BLE_L2CAP_COC_MAX_NUM) != 0
int ble_l2cap_coc_init(void);
int ble_l2cap_coc_create_server(uint16_t psm, uint16_t mtu,
                                ble_l2cap_event_fn *cb, void *cb_arg);
int ble_l2cap_coc_create_srv_chan(uint16_t conn_handle, uint16_t psm,
                                  struct ble_l2cap_chan **chan);
struct ble_l2cap_chan * ble_l2cap_coc_chan_alloc(uint16_t conn_handle,
                                                 uint16_t psm, uint16_t mtu,
                                                 struct os_mbuf *sdu_rx,
                                                 ble_l2cap_event_fn *cb,
                                                 void *cb_arg);
void ble_l2cap_coc_cleanup_chan(struct ble_l2cap_chan *chan);
void ble_l2cap_coc_le_credits_update(uint16_t conn_handle, uint16_t dcid,
                                    uint16_t credits);
int ble_l2cap_coc_recv_ready(struct ble_l2cap_chan *chan,
                             struct os_mbuf *sdu_rx);
int ble_l2cap_coc_send(struct ble_l2cap_chan *chan, struct os_mbuf *sdu_tx);
#else
#define ble_l2cap_coc_init()                                    0
#define ble_l2cap_coc_create_server(psm, mtu, cb, cb_arg)       BLE_HS_ENOTSUP
#define ble_l2cap_coc_recv_ready(chan, sdu_rx)                  BLE_HS_ENOTSUP
#define ble_l2cap_coc_cleanup_chan(chan)
#define ble_l2cap_coc_send(chan, sdu_tx)                        BLE_HS_ENOTSUP
#endif

#ifdef __cplusplus
}
#endif

#endif /* H_L2CAP_COC_PRIV_ */
