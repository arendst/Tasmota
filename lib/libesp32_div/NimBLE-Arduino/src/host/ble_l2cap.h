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

#ifndef H_BLE_L2CAP_
#define H_BLE_L2CAP_

#include "nimble/nimble_opt.h"
#ifdef __cplusplus
extern "C" {
#endif

struct ble_l2cap_sig_update_req;
struct ble_hs_conn;

#define BLE_L2CAP_CID_ATT           4
#define BLE_L2CAP_CID_SIG           5
#define BLE_L2CAP_CID_SM            6

#define BLE_L2CAP_SIG_OP_REJECT                 0x01
#define BLE_L2CAP_SIG_OP_CONNECT_REQ            0x02
#define BLE_L2CAP_SIG_OP_CONNECT_RSP            0x03
#define BLE_L2CAP_SIG_OP_CONFIG_REQ             0x04
#define BLE_L2CAP_SIG_OP_CONFIG_RSP             0x05
#define BLE_L2CAP_SIG_OP_DISCONN_REQ            0x06
#define BLE_L2CAP_SIG_OP_DISCONN_RSP            0x07
#define BLE_L2CAP_SIG_OP_ECHO_REQ               0x08
#define BLE_L2CAP_SIG_OP_ECHO_RSP               0x09
#define BLE_L2CAP_SIG_OP_INFO_REQ               0x0a
#define BLE_L2CAP_SIG_OP_INFO_RSP               0x0b
#define BLE_L2CAP_SIG_OP_CREATE_CHAN_REQ        0x0c
#define BLE_L2CAP_SIG_OP_CREATE_CHAN_RSP        0x0d
#define BLE_L2CAP_SIG_OP_MOVE_CHAN_REQ          0x0e
#define BLE_L2CAP_SIG_OP_MOVE_CHAN_RSP          0x0f
#define BLE_L2CAP_SIG_OP_MOVE_CHAN_CONF_REQ     0x10
#define BLE_L2CAP_SIG_OP_MOVE_CHAN_CONF_RSP     0x11
#define BLE_L2CAP_SIG_OP_UPDATE_REQ             0x12
#define BLE_L2CAP_SIG_OP_UPDATE_RSP             0x13
#define BLE_L2CAP_SIG_OP_LE_CREDIT_CONNECT_REQ  0x14
#define BLE_L2CAP_SIG_OP_LE_CREDIT_CONNECT_RSP  0x15
#define BLE_L2CAP_SIG_OP_FLOW_CTRL_CREDIT       0x16
#define BLE_L2CAP_SIG_OP_CREDIT_CONNECT_REQ     0x17
#define BLE_L2CAP_SIG_OP_CREDIT_CONNECT_RSP     0x18
#define BLE_L2CAP_SIG_OP_CREDIT_RECONFIG_REQ    0x19
#define BLE_L2CAP_SIG_OP_CREDIT_RECONFIG_RSP    0x1A
#define BLE_L2CAP_SIG_OP_MAX                    0x1B

#define BLE_L2CAP_SIG_ERR_CMD_NOT_UNDERSTOOD    0x0000
#define BLE_L2CAP_SIG_ERR_MTU_EXCEEDED          0x0001
#define BLE_L2CAP_SIG_ERR_INVALID_CID           0x0002

#define BLE_L2CAP_COC_ERR_CONNECTION_SUCCESS        0x0000
#define BLE_L2CAP_COC_ERR_UNKNOWN_LE_PSM            0x0002
#define BLE_L2CAP_COC_ERR_NO_RESOURCES              0x0004
#define BLE_L2CAP_COC_ERR_INSUFFICIENT_AUTHEN       0x0005
#define BLE_L2CAP_COC_ERR_INSUFFICIENT_AUTHOR       0x0006
#define BLE_L2CAP_COC_ERR_INSUFFICIENT_KEY_SZ       0x0007
#define BLE_L2CAP_COC_ERR_INSUFFICIENT_ENC          0x0008
#define BLE_L2CAP_COC_ERR_INVALID_SOURCE_CID        0x0009
#define BLE_L2CAP_COC_ERR_SOURCE_CID_ALREADY_USED   0x000A
#define BLE_L2CAP_COC_ERR_UNACCEPTABLE_PARAMETERS   0x000B
#define BLE_L2CAP_COC_ERR_INVALID_PARAMETERS        0x000C

#define BLE_L2CAP_ERR_RECONFIG_SUCCEED                       0x0000
#define BLE_L2CAP_ERR_RECONFIG_REDUCTION_MTU_NOT_ALLOWED     0x0001
#define BLE_L2CAP_ERR_RECONFIG_REDUCTION_MPS_NOT_ALLOWED     0x0002
#define BLE_L2CAP_ERR_RECONFIG_INVALID_DCID                  0x0003
#define BLE_L2CAP_ERR_RECONFIG_UNACCAPTED_PARAM              0x0004

#define BLE_L2CAP_EVENT_COC_CONNECTED                 0
#define BLE_L2CAP_EVENT_COC_DISCONNECTED              1
#define BLE_L2CAP_EVENT_COC_ACCEPT                    2
#define BLE_L2CAP_EVENT_COC_DATA_RECEIVED             3
#define BLE_L2CAP_EVENT_COC_TX_UNSTALLED              4
#define BLE_L2CAP_EVENT_COC_RECONFIG_COMPLETED        5
#define BLE_L2CAP_EVENT_COC_PEER_RECONFIGURED         6

typedef void ble_l2cap_sig_update_fn(uint16_t conn_handle, int status,
                                     void *arg);

struct ble_l2cap_sig_update_params {
    uint16_t itvl_min;
    uint16_t itvl_max;
    uint16_t slave_latency;
    uint16_t timeout_multiplier;
};

int ble_l2cap_sig_update(uint16_t conn_handle,
                         struct ble_l2cap_sig_update_params *params,
                         ble_l2cap_sig_update_fn *cb, void *cb_arg);

struct ble_l2cap_chan;

/**
 * Represents a L2CAP-related event.
 * When such an event occurs, the host notifies the application by passing an
 * instance of this structure to an application-specified callback.
 */
struct ble_l2cap_event {
    /**
     * Indicates the type of L2CAP event that occurred.  This is one of the
     * BLE_L2CAP_EVENT codes.
     */
    uint8_t type;

    /**
     * A discriminated union containing additional details concerning the L2CAP
     * event.  The 'type' field indicates which member of the union is valid.
     */
    union {
        /**
         * Represents a connection attempt. Valid for the following event
         * types:
         *     o BLE_L2CAP_EVENT_COC_CONNECTED */
        struct {
            /**
             * The status of the connection attempt;
             *     o 0: the connection was successfully established.
             *     o BLE host error code: the connection attempt failed for
             *       the specified reason.
             */
            int status;

            /** Connection handle of the relevant connection */
            uint16_t conn_handle;

            /** The L2CAP channel of the relevant L2CAP connection. */
            struct ble_l2cap_chan *chan;
        } connect;

        /**
         * Represents a terminated connection. Valid for the following event
         * types:
         *     o BLE_L2CAP_EVENT_COC_DISCONNECTED
         */
        struct {
            /** Connection handle of the relevant connection */
            uint16_t conn_handle;

            /** Information about the L2CAP connection prior to termination. */
            struct ble_l2cap_chan *chan;
        } disconnect;

        /**
         * Represents connection accept. Valid for the following event
         * types:
         *     o BLE_L2CAP_EVENT_COC_ACCEPT
         */
        struct {
            /** Connection handle of the relevant connection */
            uint16_t conn_handle;

            /** MTU supported by peer device on the channel */
            uint16_t peer_sdu_size;

            /** The L2CAP channel of the relevant L2CAP connection. */
            struct ble_l2cap_chan *chan;
        } accept;

        /**
         * Represents received data. Valid for the following event
         * types:
         *     o BLE_L2CAP_EVENT_COC_DATA_RECEIVED
         */
        struct {
            /** Connection handle of the relevant connection */
            uint16_t conn_handle;

            /** The L2CAP channel of the relevant L2CAP connection. */
            struct ble_l2cap_chan *chan;

            /** The mbuf with received SDU. */
            struct os_mbuf *sdu_rx;
        } receive;

        /**
         * Represents tx_unstalled data. Valid for the following event
         * types:
         *     o BLE_L2CAP_EVENT_COC_TX_UNSTALLED
         */
        struct {
            /** Connection handle of the relevant connection */
            uint16_t conn_handle;

            /** The L2CAP channel of the relevant L2CAP connection. */
            struct ble_l2cap_chan *chan;

            /**
             * The status of the send attempt which was stalled due to
             * lack of credits; This can be non zero only if there
             * is an issue with memory allocation for following SDU fragments.
             * In such a case last SDU has been partially sent to peer device
             * and it is up to application to decide how to handle it.
             */
            int status;
        } tx_unstalled;

        /**
         * Represents reconfiguration done. Valid for the following event
         * types:
         *      o BLE_L2CAP_EVENT_COC_RECONFIG_COMPLETED
         *      o BLE_L2CAP_EVENT_COC_PEER_RECONFIGURED
         */
        struct {
            /**
             * The status of the reconfiguration attempt;
             *     o 0: the reconfiguration was successfully done.
             *     o BLE host error code: the reconfiguration attempt failed for
             *       the specified reason.
             */
            int status;

            /** Connection handle of the relevant connection */
            uint16_t conn_handle;

            /** The L2CAP channel of the relevant L2CAP connection. */
            struct ble_l2cap_chan *chan;
        } reconfigured;
    };
};

struct ble_l2cap_chan_info {
    uint16_t scid;
    uint16_t dcid;
    uint16_t our_l2cap_mtu;
    uint16_t peer_l2cap_mtu;
    uint16_t psm;
    uint16_t our_coc_mtu;
    uint16_t peer_coc_mtu;
};

typedef int ble_l2cap_event_fn(struct ble_l2cap_event *event, void *arg);


uint16_t ble_l2cap_get_conn_handle(struct ble_l2cap_chan *chan);
int ble_l2cap_create_server(uint16_t psm, uint16_t mtu,
                            ble_l2cap_event_fn *cb, void *cb_arg);

int ble_l2cap_connect(uint16_t conn_handle, uint16_t psm, uint16_t mtu,
                      struct os_mbuf *sdu_rx,
                      ble_l2cap_event_fn *cb, void *cb_arg);
int ble_l2cap_disconnect(struct ble_l2cap_chan *chan);
int ble_l2cap_send(struct ble_l2cap_chan *chan, struct os_mbuf *sdu_tx);
int ble_l2cap_recv_ready(struct ble_l2cap_chan *chan, struct os_mbuf *sdu_rx);
int ble_l2cap_get_chan_info(struct ble_l2cap_chan *chan, struct ble_l2cap_chan_info *chan_info);

#ifdef __cplusplus
}
#endif

#endif
