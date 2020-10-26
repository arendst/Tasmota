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

#ifndef H_BLE_GAP_
#define H_BLE_GAP_

/**
 * @brief Bluetooth Host Generic Access Profile (GAP)
 * @defgroup bt_host_gap Bluetooth Host Generic Access Profile (GAP)
 * @ingroup bt_host
 * @{
 */

#include <inttypes.h>
#include "host/ble_hs.h"
#include "host/ble_hs_adv.h"
#include "syscfg/syscfg.h"

#ifdef __cplusplus
extern "C" {
#endif

struct hci_le_conn_complete;
struct hci_conn_update;

/** 30 ms. */
#define BLE_GAP_ADV_FAST_INTERVAL1_MIN      (30 * 1000 / BLE_HCI_ADV_ITVL)

/** 60 ms. */
#define BLE_GAP_ADV_FAST_INTERVAL1_MAX      (60 * 1000 / BLE_HCI_ADV_ITVL)

/** 100 ms. */
#define BLE_GAP_ADV_FAST_INTERVAL2_MIN      (100 * 1000 / BLE_HCI_ADV_ITVL)

/** 150 ms. */
#define BLE_GAP_ADV_FAST_INTERVAL2_MAX      (150 * 1000 / BLE_HCI_ADV_ITVL)

/** 30 ms; active scanning. */
#define BLE_GAP_SCAN_FAST_INTERVAL_MIN      (30 * 1000 / BLE_HCI_ADV_ITVL)

/** 60 ms; active scanning. */
#define BLE_GAP_SCAN_FAST_INTERVAL_MAX      (60 * 1000 / BLE_HCI_ADV_ITVL)

/** 11.25 ms; limited discovery interval. */
#define BLE_GAP_LIM_DISC_SCAN_INT           (11.25 * 1000 / BLE_HCI_SCAN_ITVL)

/** 11.25 ms; limited discovery window (not from the spec). */
#define BLE_GAP_LIM_DISC_SCAN_WINDOW        (11.25 * 1000 / BLE_HCI_SCAN_ITVL)

/** 30 ms; active scanning. */
#define BLE_GAP_SCAN_FAST_WINDOW            (30 * 1000 / BLE_HCI_SCAN_ITVL)

/* 30.72 seconds; active scanning. */
#define BLE_GAP_SCAN_FAST_PERIOD            (30.72 * 1000)

/** 1.28 seconds; background scanning. */
#define BLE_GAP_SCAN_SLOW_INTERVAL1         (1280 * 1000 / BLE_HCI_SCAN_ITVL)

/** 11.25 ms; background scanning. */
#define BLE_GAP_SCAN_SLOW_WINDOW1           (11.25 * 1000 / BLE_HCI_SCAN_ITVL)

/** 10.24 seconds. */
#define BLE_GAP_DISC_DUR_DFLT               (10.24 * 1000)

/** 30 seconds (not from the spec). */
#define BLE_GAP_CONN_DUR_DFLT               (30 * 1000)

/** 1 second. */
#define BLE_GAP_CONN_PAUSE_CENTRAL          (1 * 1000)

/** 5 seconds. */
#define BLE_GAP_CONN_PAUSE_PERIPHERAL       (5 * 1000)

/* 30 ms. */
#define BLE_GAP_INITIAL_CONN_ITVL_MIN       (30 * 1000 / BLE_HCI_CONN_ITVL)

/* 50 ms. */
#define BLE_GAP_INITIAL_CONN_ITVL_MAX       (50 * 1000 / BLE_HCI_CONN_ITVL)

/** Default channels mask: all three channels are used. */
#define BLE_GAP_ADV_DFLT_CHANNEL_MAP        0x07

#define BLE_GAP_INITIAL_CONN_LATENCY        0
#define BLE_GAP_INITIAL_SUPERVISION_TIMEOUT 0x0100
#define BLE_GAP_INITIAL_CONN_MIN_CE_LEN     0x0000
#define BLE_GAP_INITIAL_CONN_MAX_CE_LEN     0x0000

#define BLE_GAP_ROLE_MASTER                 0
#define BLE_GAP_ROLE_SLAVE                  1

#define BLE_GAP_EVENT_CONNECT               0
#define BLE_GAP_EVENT_DISCONNECT            1
/* Reserved                                 2 */
#define BLE_GAP_EVENT_CONN_UPDATE           3
#define BLE_GAP_EVENT_CONN_UPDATE_REQ       4
#define BLE_GAP_EVENT_L2CAP_UPDATE_REQ      5
#define BLE_GAP_EVENT_TERM_FAILURE          6
#define BLE_GAP_EVENT_DISC                  7
#define BLE_GAP_EVENT_DISC_COMPLETE         8
#define BLE_GAP_EVENT_ADV_COMPLETE          9
#define BLE_GAP_EVENT_ENC_CHANGE            10
#define BLE_GAP_EVENT_PASSKEY_ACTION        11
#define BLE_GAP_EVENT_NOTIFY_RX             12
#define BLE_GAP_EVENT_NOTIFY_TX             13
#define BLE_GAP_EVENT_SUBSCRIBE             14
#define BLE_GAP_EVENT_MTU                   15
#define BLE_GAP_EVENT_IDENTITY_RESOLVED     16
#define BLE_GAP_EVENT_REPEAT_PAIRING        17
#define BLE_GAP_EVENT_PHY_UPDATE_COMPLETE   18
#define BLE_GAP_EVENT_EXT_DISC              19
#define BLE_GAP_EVENT_PERIODIC_SYNC         20
#define BLE_GAP_EVENT_PERIODIC_REPORT       21
#define BLE_GAP_EVENT_PERIODIC_SYNC_LOST    22
#define BLE_GAP_EVENT_SCAN_REQ_RCVD         23

/*** Reason codes for the subscribe GAP event. */

/** Peer's CCCD subscription state changed due to a descriptor write. */
#define BLE_GAP_SUBSCRIBE_REASON_WRITE      1

/** Peer's CCCD subscription state cleared due to connection termination. */
#define BLE_GAP_SUBSCRIBE_REASON_TERM       2

/**
 * Peer's CCCD subscription state changed due to restore from persistence
 * (bonding restored).
 */
#define BLE_GAP_SUBSCRIBE_REASON_RESTORE    3

#define BLE_GAP_REPEAT_PAIRING_RETRY        1
#define BLE_GAP_REPEAT_PAIRING_IGNORE       2

/** Connection security state */
struct ble_gap_sec_state {
    /** If connection is encrypted */
    unsigned encrypted:1;

    /** If connection is authenticated */
    unsigned authenticated:1;

    /** If connection is bonded (security information is stored)  */
    unsigned bonded:1;

    /** Size of a key used for encryption */
    unsigned key_size:5;
};

/** Advertising parameters */
struct ble_gap_adv_params {
    /** Advertising mode. Can be one of following constants:
     *  - BLE_GAP_CONN_MODE_NON (non-connectable; 3.C.9.3.2).
     *  - BLE_GAP_CONN_MODE_DIR (directed-connectable; 3.C.9.3.3).
     *  - BLE_GAP_CONN_MODE_UND (undirected-connectable; 3.C.9.3.4).
     */
    uint8_t conn_mode;
    /** Discoverable mode. Can be one of following constants:
     *  - BLE_GAP_DISC_MODE_NON  (non-discoverable; 3.C.9.2.2).
     *  - BLE_GAP_DISC_MODE_LTD (limited-discoverable; 3.C.9.2.3).
     *  - BLE_GAP_DISC_MODE_GEN (general-discoverable; 3.C.9.2.4).
     */
    uint8_t disc_mode;

    /** Minimum advertising interval, if 0 stack use sane defaults */
    uint16_t itvl_min;
    /** Maximum advertising interval, if 0 stack use sane defaults */
    uint16_t itvl_max;
    /** Advertising channel map , if 0 stack use sane defaults */
    uint8_t channel_map;

    /** Advertising  Filter policy */
    uint8_t filter_policy;

    /** If do High Duty cycle for Directed Advertising */
    uint8_t high_duty_cycle:1;
};

/** @brief Connection descriptor */
struct ble_gap_conn_desc {
    /** Connection security state */
    struct ble_gap_sec_state sec_state;

    /** Local identity address */
    ble_addr_t our_id_addr;

    /** Peer identity address */
    ble_addr_t peer_id_addr;

    /** Local over-the-air address */
    ble_addr_t our_ota_addr;

    /** Peer over-the-air address */
    ble_addr_t peer_ota_addr;

    /** Connection handle */
    uint16_t conn_handle;

    /** Connection interval */
    uint16_t conn_itvl;

    /** Connection latency */
    uint16_t conn_latency;

    /** Connection supervision timeout */
    uint16_t supervision_timeout;

    /** Connection Role
     * Possible values BLE_GAP_ROLE_SLAVE or BLE_GAP_ROLE_MASTER
     */
    uint8_t role;

    /** Master clock accuracy */
    uint8_t master_clock_accuracy;
};

/** @brief Connection parameters  */
struct ble_gap_conn_params {
    /** Scan interval in 0.625ms units */
    uint16_t scan_itvl;

    /** Scan window in 0.625ms units */
    uint16_t scan_window;

    /** Minimum value for connection interval in 1.25ms units */
    uint16_t itvl_min;

    /** Maximum value for connection interval in 1.25ms units */
    uint16_t itvl_max;

    /** Connection latency */
    uint16_t latency;

    /** Supervision timeout in 10ms units */
    uint16_t supervision_timeout;

    /** Minimum length of connection event in 0.625ms units */
    uint16_t min_ce_len;

    /** Maximum length of connection event in 0.625ms units */
    uint16_t max_ce_len;
};

/** @brief Extended discovery parameters */
struct ble_gap_ext_disc_params {
    /** Scan interval in 0.625ms units */
    uint16_t itvl;

    /** Scan window in 0.625ms units */
    uint16_t window;

    /** If passive scan should be used */
    uint8_t passive:1;
};

/** @brief Discovery parameters */
struct ble_gap_disc_params {
    /** Scan interval in 0.625ms units */
    uint16_t itvl;

    /** Scan window in 0.625ms units */
    uint16_t window;

    /** Scan filter policy */
    uint8_t filter_policy;

    /** If limited discovery procedure should be used */
    uint8_t limited:1;

    /** If passive scan should be used */
    uint8_t passive:1;

    /** If enable duplicates filtering */
    uint8_t filter_duplicates:1;
};

/** @brief Connection parameters update parameters */
struct ble_gap_upd_params {
    /** Minimum value for connection interval in 1.25ms units */
    uint16_t itvl_min;

    /** Maximum value for connection interval in 1.25ms units */
    uint16_t itvl_max;

    /** Connection latency */
    uint16_t latency;

    /** Supervision timeout in 10ms units */
    uint16_t supervision_timeout;

    /** Minimum length of connection event in 0.625ms units */
    uint16_t min_ce_len;

    /** Maximum length of connection event in 0.625ms units */
    uint16_t max_ce_len;
};

/** @brief Passkey query */
struct ble_gap_passkey_params {
    /** Passkey action, can be one of following constants:
     *  - BLE_SM_IOACT_NONE
     *  - BLE_SM_IOACT_OOB
     *  - BLE_SM_IOACT_INPUT
     *  - BLE_SM_IOACT_DISP
     *  - BLE_SM_IOACT_NUMCMP
     */
    uint8_t action;

    /** Passkey to compare, valid for BLE_SM_IOACT_NUMCMP action */
    uint32_t numcmp;
};

#if MYNEWT_VAL(BLE_EXT_ADV)

#define BLE_GAP_EXT_ADV_DATA_STATUS_COMPLETE   0x00
#define BLE_GAP_EXT_ADV_DATA_STATUS_INCOMPLETE 0x01
#define BLE_GAP_EXT_ADV_DATA_STATUS_TRUNCATED  0x02

/** @brief Extended advertising report */
struct ble_gap_ext_disc_desc {
    /** Report properties bitmask
     *  - BLE_HCI_ADV_CONN_MASK
     *  - BLE_HCI_ADV_SCAN_MASK
     *  - BLE_HCI_ADV_DIRECT_MASK
     *  - BLE_HCI_ADV_SCAN_RSP_MASK
     *  - BLE_HCI_ADV_LEGACY_MASK
     *  */
    uint8_t props;

    /** Advertising data status, can be one of following constants:
     *  - BLE_GAP_EXT_ADV_DATA_STATUS_COMPLETE
     *  - BLE_GAP_EXT_ADV_DATA_STATUS_INCOMPLETE
     *  - BLE_GAP_EXT_ADV_DATA_STATUS_TRUNCATED
     */
    uint8_t data_status;

    /** Legacy advertising PDU type. Valid if BLE_HCI_ADV_LEGACY_MASK props is
     * set. Can be one of following constants:
     *  - BLE_HCI_ADV_RPT_EVTYPE_ADV_IND
     *  - BLE_HCI_ADV_RPT_EVTYPE_DIR_IND
     *  - BLE_HCI_ADV_RPT_EVTYPE_SCAN_IND
     *  - BLE_HCI_ADV_RPT_EVTYPE_NONCONN_IND
     *  - BLE_HCI_ADV_RPT_EVTYPE_SCAN_RSP
     */
    uint8_t legacy_event_type;

    /** Advertiser address */
    ble_addr_t addr;

    /** Received signal strength indication in dBm (127 if unavailable) */
    int8_t rssi;

    /** Advertiser transmit power in dBm (127 if unavailable) */
    int8_t tx_power;

    /** Advertising Set ID */
    uint8_t sid;

    /** Primary advertising PHY, can be one of following constants:
     *  - BLE_HCI_LE_PHY_1M
     *  - BLE_HCI_LE_PHY_CODED
     */
    uint8_t prim_phy;

    /** Secondary advertising PHY, can be one of following constants:
     *  - BLE_HCI_LE_PHY_1M
     *  - LE_HCI_LE_PHY_2M
     *  - BLE_HCI_LE_PHY_CODED
     */
    uint8_t sec_phy;

    /** Periodic advertising interval. 0 if no periodic advertising. */
    uint16_t periodic_adv_itvl;

    /** Advertising Data length */
    uint8_t length_data;

    /** Advertising data */
    uint8_t *data;

    /** Directed advertising address.  Valid if BLE_HCI_ADV_DIRECT_MASK props is
     * set (BLE_ADDR_ANY otherwise).
     */
    ble_addr_t direct_addr;
};
#endif

/** @brief Advertising report */
struct ble_gap_disc_desc {
    /** Advertising PDU type. Can be one of following constants:
     *  - BLE_HCI_ADV_RPT_EVTYPE_ADV_IND
     *  - BLE_HCI_ADV_RPT_EVTYPE_DIR_IND
     *  - BLE_HCI_ADV_RPT_EVTYPE_SCAN_IND
     *  - BLE_HCI_ADV_RPT_EVTYPE_NONCONN_IND
     *  - BLE_HCI_ADV_RPT_EVTYPE_SCAN_RSP
     */
    uint8_t event_type;

    /** Advertising Data length */
    uint8_t length_data;

    /** Advertiser address */
    ble_addr_t addr;

    /** Received signal strength indication in dBm (127 if unavailable) */
    int8_t rssi;

    /** Advertising data */
    uint8_t *data;

    /** Directed advertising address.  Valid for BLE_HCI_ADV_RPT_EVTYPE_DIR_IND
     * event type (BLE_ADDR_ANY otherwise).
     */
    ble_addr_t direct_addr;
};

struct ble_gap_repeat_pairing {
    /** The handle of the relevant connection. */
    uint16_t conn_handle;

    /** Properties of the existing bond. */
    uint8_t cur_key_size;
    uint8_t cur_authenticated:1;
    uint8_t cur_sc:1;

    /**
     * Properties of the imminent secure link if the pairing procedure is
     * allowed to continue.
     */
    uint8_t new_key_size;
    uint8_t new_authenticated:1;
    uint8_t new_sc:1;
    uint8_t new_bonding:1;
};

/**
 * Represents a GAP-related event.  When such an event occurs, the host
 * notifies the application by passing an instance of this structure to an
 * application-specified callback.
 */
struct ble_gap_event {
    /**
     * Indicates the type of GAP event that occurred.  This is one of the
     * BLE_GAP_EVENT codes.
     */
    uint8_t type;

    /**
     * A discriminated union containing additional details concerning the GAP
     * event.  The 'type' field indicates which member of the union is valid.
     */
    union {
        /**
         * Represents a connection attempt.  Valid for the following event
         * types:
         *     o BLE_GAP_EVENT_CONNECT
         */
        struct {
            /**
             * The status of the connection attempt;
             *     o 0: the connection was successfully established.
             *     o BLE host error code: the connection attempt failed for
             *       the specified reason.
             */
            int status;

            /** The handle of the relevant connection. */
            uint16_t conn_handle;
        } connect;

        /**
         * Represents a terminated connection.  Valid for the following event
         * types:
         *     o BLE_GAP_EVENT_DISCONNECT
         */
        struct {
            /**
             * A BLE host return code indicating the reason for the
             * disconnect.
             */
            int reason;

            /** Information about the connection prior to termination. */
            struct ble_gap_conn_desc conn;
        } disconnect;

        /**
         * Represents an advertising report received during a discovery
         * procedure.  Valid for the following event types:
         *     o BLE_GAP_EVENT_DISC
         */
        struct ble_gap_disc_desc disc;

#if MYNEWT_VAL(BLE_EXT_ADV)
        /**
         * Represents an extended advertising report received during a discovery
         * procedure.  Valid for the following event types:
         *     o BLE_GAP_EVENT_EXT_DISC
         */
        struct ble_gap_ext_disc_desc ext_disc;
#endif

        /**
         * Represents a completed discovery procedure.  Valid for the following
         * event types:
         *     o BLE_GAP_EVENT_DISC_COMPLETE
         */
        struct {
            /**
             * The reason the discovery procedure stopped.  Typical reason
             * codes are:
             *     o 0: Duration expired.
             *     o BLE_HS_EPREEMPTED: Host aborted procedure to configure a
             *       peer's identity.
             */
            int reason;
        } disc_complete;

        /**
         * Represents a completed advertise procedure.  Valid for the following
         * event types:
         *     o BLE_GAP_EVENT_ADV_COMPLETE
         */
        struct {
            /**
             * The reason the advertise procedure stopped.  Typical reason
             * codes are:
             *     o 0: Terminated due to connection.
             *     o BLE_HS_ETIMEOUT: Duration expired.
             *     o BLE_HS_EPREEMPTED: Host aborted procedure to configure a
             *       peer's identity.
             */
            int reason;

#if MYNEWT_VAL(BLE_EXT_ADV)
            /** Advertising instance */
            uint8_t instance;
            /** The handle of the relevant connection - valid if reason=0 */
            uint16_t conn_handle;
            /**
             * Number of completed extended advertising events
             *
             * This field is only valid if non-zero max_events was passed to
             * ble_gap_ext_adv_start() and advertising completed due to duration
             * timeout or max events transmitted.
             * */
            uint8_t num_ext_adv_events;
#endif
        } adv_complete;

        /**
         * Represents an attempt to update a connection's parameters.  If the
         * attempt was successful, the connection's descriptor reflects the
         * updated parameters.
         *
         * Valid for the following event types:
         *     o BLE_GAP_EVENT_CONN_UPDATE
         */
        struct {
            /**
             * The result of the connection update attempt;
             *     o 0: the connection was successfully updated.
             *     o BLE host error code: the connection update attempt failed
             *       for the specified reason.
             */
            int status;

            /** The handle of the relevant connection. */
            uint16_t conn_handle;
        } conn_update;

        /**
         * Represents a peer's request to update the connection parameters.
         * This event is generated when a peer performs any of the following
         * procedures:
         *     o L2CAP Connection Parameter Update Procedure
         *     o Link-Layer Connection Parameters Request Procedure
         *
         * To reject the request, return a non-zero HCI error code.  The value
         * returned is the reject reason given to the controller.
         *
         * Valid for the following event types:
         *     o BLE_GAP_EVENT_L2CAP_UPDATE_REQ
         *     o BLE_GAP_EVENT_CONN_UPDATE_REQ
         */
        struct {
            /**
             * Indicates the connection parameters that the peer would like to
             * use.
             */
            const struct ble_gap_upd_params *peer_params;

            /**
             * Indicates the connection parameters that the local device would
             * like to use.  The application callback should fill this in.  By
             * default, this struct contains the requested parameters (i.e.,
             * it is a copy of 'peer_params').
             */
            struct ble_gap_upd_params *self_params;

            /** The handle of the relevant connection. */
            uint16_t conn_handle;
        } conn_update_req;

        /**
         * Represents a failed attempt to terminate an established connection.
         * Valid for the following event types:
         *     o BLE_GAP_EVENT_TERM_FAILURE
         */
        struct {
            /**
             * A BLE host return code indicating the reason for the failure.
             */
            int status;

            /** The handle of the relevant connection. */
            uint16_t conn_handle;
        } term_failure;

        /**
         * Represents an attempt to change the encrypted state of a
         * connection.  If the attempt was successful, the connection
         * descriptor reflects the updated encrypted state.
         *
         * Valid for the following event types:
         *     o BLE_GAP_EVENT_ENC_CHANGE
         */
        struct {
            /**
             * Indicates the result of the encryption state change attempt;
             *     o 0: the encrypted state was successfully updated;
             *     o BLE host error code: the encryption state change attempt
             *       failed for the specified reason.
             */
            int status;

            /** The handle of the relevant connection. */
            uint16_t conn_handle;
        } enc_change;

        /**
         * Represents a passkey query needed to complete a pairing procedure.
         *
         * Valid for the following event types:
         *     o BLE_GAP_EVENT_PASSKEY_ACTION
         */
        struct {
            /** Contains details about the passkey query. */
            struct ble_gap_passkey_params params;

            /** The handle of the relevant connection. */
            uint16_t conn_handle;
        } passkey;

        /**
         * Represents a received ATT notification or indication.
         *
         * Valid for the following event types:
         *     o BLE_GAP_EVENT_NOTIFY_RX
         */
        struct {
            /**
             * The contents of the notification or indication.  If the
             * application wishes to retain this mbuf for later use, it must
             * set this pointer to NULL to prevent the stack from freeing it.
             */
            struct os_mbuf *om;

            /** The handle of the relevant ATT attribute. */
            uint16_t attr_handle;

            /** The handle of the relevant connection. */
            uint16_t conn_handle;

            /**
             * Whether the received command is a notification or an
             * indication;
             *     o 0: Notification;
             *     o 1: Indication.
             */
            uint8_t indication:1;
        } notify_rx;

        /**
         * Represents a transmitted ATT notification or indication, or a
         * completed indication transaction.
         *
         * Valid for the following event types:
         *     o BLE_GAP_EVENT_NOTIFY_TX
         */
        struct {
            /**
             * The status of the notification or indication transaction;
             *     o 0:                 Command successfully sent;
             *     o BLE_HS_EDONE:      Confirmation (indication ack) received;
             *     o BLE_HS_ETIMEOUT:   Confirmation (indication ack) never
             *                              received;
             *     o Other return code: Error.
             */
            int status;

            /** The handle of the relevant connection. */
            uint16_t conn_handle;

            /** The handle of the relevant characteristic value. */
            uint16_t attr_handle;

            /**
             * Whether the transmitted command is a notification or an
             * indication;
             *     o 0: Notification;
             *     o 1: Indication.
             */
            uint8_t indication:1;
        } notify_tx;

        /**
         * Represents a state change in a peer's subscription status.  In this
         * comment, the term "update" is used to refer to either a notification
         * or an indication.  This event is triggered by any of the following
         * occurrences:
         *     o Peer enables or disables updates via a CCCD write.
         *     o Connection is about to be terminated and the peer is
         *       subscribed to updates.
         *     o Peer is now subscribed to updates after its state was restored
         *       from persistence.  This happens when bonding is restored.
         *
         * Valid for the following event types:
         *     o BLE_GAP_EVENT_SUBSCRIBE
         */
        struct {
            /** The handle of the relevant connection. */
            uint16_t conn_handle;

            /** The value handle of the relevant characteristic. */
            uint16_t attr_handle;

            /** One of the BLE_GAP_SUBSCRIBE_REASON codes. */
            uint8_t reason;

            /** Whether the peer was previously subscribed to notifications. */
            uint8_t prev_notify:1;

            /** Whether the peer is currently subscribed to notifications. */
            uint8_t cur_notify:1;

            /** Whether the peer was previously subscribed to indications. */
            uint8_t prev_indicate:1;

            /** Whether the peer is currently subscribed to indications. */
            uint8_t cur_indicate:1;
        } subscribe;

        /**
         * Represents a change in an L2CAP channel's MTU.
         *
         * Valid for the following event types:
         *     o BLE_GAP_EVENT_MTU
         */
        struct {
            /** The handle of the relevant connection. */
            uint16_t conn_handle;

            /**
             * Indicates the channel whose MTU has been updated; either
             * BLE_L2CAP_CID_ATT or the ID of a connection-oriented channel.
             */
            uint16_t channel_id;

            /* The channel's new MTU. */
            uint16_t value;
        } mtu;

        /**
         * Represents a change in peer's identity. This is issued after
         * successful pairing when Identity Address Information was received.
         *
         * Valid for the following event types:
         *     o BLE_GAP_EVENT_IDENTITY_RESOLVED
         */
        struct {
            /** The handle of the relevant connection. */
            uint16_t conn_handle;
        } identity_resolved;

        /**
         * Represents a peer's attempt to pair despite a bond already existing.
         * The application has two options for handling this event type:
         *     o Retry: Return BLE_GAP_REPEAT_PAIRING_RETRY after deleting the
         *              conflicting bond.  The stack will verify the bond has
         *              been deleted and continue the pairing procedure.  If
         *              the bond is still present, this event will be reported
         *              again.
         *     o Ignore: Return BLE_GAP_REPEAT_PAIRING_IGNORE.  The stack will
         *               silently ignore the pairing request.
         *
         * Valid for the following event types:
         *     o BLE_GAP_EVENT_REPEAT_PAIRING
         */
        struct ble_gap_repeat_pairing repeat_pairing;

        /**
         * Represents a change of PHY. This is issue after successful
         * change on PHY.
         */
        struct {
            int status;
            uint16_t conn_handle;

            /**
             * Indicates enabled TX/RX PHY. Possible values:
             *     o BLE_GAP_LE_PHY_1M
             *     o BLE_GAP_LE_PHY_2M
             *     o BLE_GAP_LE_PHY_CODED
             */
            uint8_t tx_phy;
            uint8_t rx_phy;
        } phy_updated;
#if MYNEWT_VAL(BLE_PERIODIC_ADV)
        /**
         * Represents a periodic advertising sync established during discovery
         * procedure. Valid for the following event types:
         *     o BLE_GAP_EVENT_PERIODIC_SYNC
         */
        struct {
            /** BLE_ERR_SUCCESS on success or error code on failure. Other
             * fields are valid only for success
             */
            uint8_t status;
            /** Periodic sync handle */
            uint16_t sync_handle;

            /** Advertising Set ID */
            uint8_t sid;

            /** Advertiser address */
            ble_addr_t adv_addr;

            /** Advertising PHY, can be one of following constants:
             *  - BLE_HCI_LE_PHY_1M
             *  - LE_HCI_LE_PHY_2M
             *  - BLE_HCI_LE_PHY_CODED
            */
            uint8_t adv_phy;

            /** Periodic advertising interval */
            uint16_t per_adv_ival;

            /** Advertiser clock accuracy */
            uint8_t adv_clk_accuracy;
        } periodic_sync;

        /**
         * Represents a periodic advertising report received on established
         * sync. Valid for the following event types:
         *     o BLE_GAP_EVENT_PERIODIC_REPORT
         */
        struct {
            /** Periodic sync handle */
            uint16_t sync_handle;

            /** Advertiser transmit power in dBm (127 if unavailable) */
            int8_t tx_power;

            /** Received signal strength indication in dBm (127 if unavailable) */
            int8_t rssi;

            /** Advertising data status, can be one of following constants:
             *  - BLE_HCI_PERIODIC_DATA_STATUS_COMPLETE
             *  - BLE_HCI_PERIODIC_DATA_STATUS_INCOMPLETE
             *  - BLE_HCI_PERIODIC_DATA_STATUS_TRUNCATED
             */
            uint8_t data_status;

            /** Advertising Data length */
            uint8_t data_length;

            /** Advertising data */
            uint8_t *data;
        } periodic_report;

        /**
         * Represents a periodic advertising sync lost of established sync.
         * Sync lost reason can be BLE_HS_ETIMEOUT (sync timeout) or
         * BLE_HS_EDONE (sync terminated locally).
         * Valid for the following event types:
         *     o BLE_GAP_EVENT_PERIODIC_SYNC_LOST
         */
        struct {
            /** Periodic sync handle */
            uint16_t sync_handle;

            /** Reason for sync lost, can be BLE_HS_ETIMEOUT for timeout or
             * BLE_HS_EDONE for locally terminated sync
             */
            int reason;
        } periodic_sync_lost;
#endif

#if MYNEWT_VAL(BLE_EXT_ADV)
        /**
         * Represents a scan request for an extended advertising instance where
         * scan request notifications were enabled.
         * Valid for the following event types:
         *     o BLE_GAP_EVENT_SCAN_REQ_RCVD
         */
        struct {
            /** Extended advertising instance */
            uint8_t instance;
            /** Address of scanner */
            ble_addr_t scan_addr;
        } scan_req_rcvd;
#endif
    };
};

typedef int ble_gap_event_fn(struct ble_gap_event *event, void *arg);

#define BLE_GAP_CONN_MODE_NON               0
#define BLE_GAP_CONN_MODE_DIR               1
#define BLE_GAP_CONN_MODE_UND               2

#define BLE_GAP_DISC_MODE_NON               0
#define BLE_GAP_DISC_MODE_LTD               1
#define BLE_GAP_DISC_MODE_GEN               2

/**
 * Searches for a connection with the specified handle.  If a matching
 * connection is found, the supplied connection descriptor is filled
 * correspondingly.
 *
 * @param handle    The connection handle to search for.
 * @param out_desc  On success, this is populated with information relating to
 *                  the matching connection.  Pass NULL if you don't need this
 *                  information.
 *
 * @return          0 on success, BLE_HS_ENOTCONN if no matching connection was
 *                  found.
 */
int ble_gap_conn_find(uint16_t handle, struct ble_gap_conn_desc *out_desc);

/**
 * Searches for a connection with a peer with the specified address.
 * If a matching connection is found, the supplied connection descriptor
 * is filled correspondingly.
 *
 * @param addr      The ble address of a connected peer device to search for.
 * @param out_desc  On success, this is populated with information relating to
 *                  the matching connection.  Pass NULL if you don't need this
 *                  information.
 *
 * @return          0 on success, BLE_HS_ENOTCONN if no matching connection was
 *                  found.
 */
int ble_gap_conn_find_by_addr(const ble_addr_t *addr,
                              struct ble_gap_conn_desc *out_desc);

/**
 * Configures a connection to use the specified GAP event callback.  A
 * connection's GAP event callback is first specified when the connection is
 * created, either via advertising or initiation.  This function replaces the
 * callback that was last configured.
 *
 * @param conn_handle   The handle of the connection to configure.
 * @param cb            The callback to associate with the connection.
 * @param cb_arg        An optional argument that the callback receives.
 *
 * @return              0 on success, BLE_HS_ENOTCONN if there is no connection
 *                      with the specified handle.
 */
int ble_gap_set_event_cb(uint16_t conn_handle,
                         ble_gap_event_fn *cb, void *cb_arg);

/** @brief Start advertising
 *
 * This function configures and start advertising procedure.
 *
 * @param own_addr_type The type of address the stack should use for itself.
 *                      Valid values are:
 *                         - BLE_OWN_ADDR_PUBLIC
 *                         - BLE_OWN_ADDR_RANDOM
 *                         - BLE_OWN_ADDR_RPA_PUBLIC_DEFAULT
 *                         - BLE_OWN_ADDR_RPA_RANDOM_DEFAULT
 * @param direct_addr   The peer's address for directed advertising. This
 *                      parameter shall be non-NULL if directed advertising is
 *                      being used.
 * @param duration_ms   The duration of the advertisement procedure. On
 *                      expiration, the procedure ends and a
 *                      BLE_GAP_EVENT_ADV_COMPLETE event is reported. Units are
 *                      milliseconds. Specify BLE_HS_FOREVER for no expiration.
 * @param adv_params    Additional arguments specifying the particulars of the
 *                      advertising procedure.
 * @param cb            The callback to associate with this advertising
 *                      procedure.  If advertising ends, the event is reported
 *                      through this callback.  If advertising results in a
 *                      connection, the connection inherits this callback as its
 *                      event-reporting mechanism.
 * @param cb_arg        The optional argument to pass to the callback function.
 *
 * @return              0 on success, error code on failure.
 */
int ble_gap_adv_start(uint8_t own_addr_type, const ble_addr_t *direct_addr,
                      int32_t duration_ms,
                      const struct ble_gap_adv_params *adv_params,
                      ble_gap_event_fn *cb, void *cb_arg);

/**
 * Stops the currently-active advertising procedure.  A success return
 * code indicates that advertising has been fully aborted and a new advertising
 * procedure can be initiated immediately.
 *
 * NOTE: If the caller is running in the same task as the NimBLE host, or if it
 * is running in a higher priority task than that of the host, care must be
 * taken when restarting advertising.  Under these conditions, the following is
 * *not* a reliable method to restart advertising:
 *     ble_gap_adv_stop()
 *     ble_gap_adv_start()
 *
 * Instead, the call to `ble_gap_adv_start()` must be made in a separate event
 * context.  That is, `ble_gap_adv_start()` must be called asynchronously by
 * enqueueing an event on the current task's event queue.  See
 * https://github.com/apache/mynewt-nimble/pull/211 for more information.
 *
 * @return  0 on success, BLE_HS_EALREADY if there is no active advertising
 *          procedure, other error code on failure.
 */
int ble_gap_adv_stop(void);

/**
 * Indicates whether an advertisement procedure is currently in progress.
 *
 * @return 0 if no advertisement procedure in progress, 1 otherwise.
 */
int ble_gap_adv_active(void);

/**
 * Configures the data to include in subsequent advertisements.
 *
 * @param data      Buffer containing the advertising data.
 * @param data_len  The size of the advertising data, in bytes.
 *
 * @return          0 on succes,  BLE_HS_EBUSY if advertising is in progress,
 *                  other error code on failure.
 */
int ble_gap_adv_set_data(const uint8_t *data, int data_len);

/**
 * Configures the data to include in subsequent scan responses.
 *
 * @param data      Buffer containing the scan response data.
 * @param data_len  The size of the response data, in bytes.
 *
 * @return          0 on succes,  BLE_HS_EBUSY if advertising is in progress,
 *                  other error code on failure.
 */
int ble_gap_adv_rsp_set_data(const uint8_t *data, int data_len);

/**
 * Configures the fields to include in subsequent advertisements.  This is a
 * convenience wrapper for ble_gap_adv_set_data().
 *
 * @param adv_fields    Specifies the advertisement data.
 *
 * @return              0 on success,
 *                      BLE_HS_EBUSY if advertising is in progress,
 *                      BLE_HS_EMSGSIZE if the specified data is too large to
 *                      fit in an advertisement,
 *                      other error code on failure.
 */
int ble_gap_adv_set_fields(const struct ble_hs_adv_fields *rsp_fields);

/**
 * Configures the fields to include in subsequent scan responses.  This is a
 * convenience wrapper for ble_gap_adv_rsp_set_data().
 *
 * @param adv_fields   Specifies the scan response data.
 *
 * @return              0 on success,
 *                      BLE_HS_EBUSY if advertising is in progress,
 *                      BLE_HS_EMSGSIZE if the specified data is too large to
 *                      fit in a scan response,
 *                      other error code on failure.
 */
int ble_gap_adv_rsp_set_fields(const struct ble_hs_adv_fields *rsp_fields);

#if MYNEWT_VAL(BLE_EXT_ADV)
/** @brief Extended advertising parameters  */
struct ble_gap_ext_adv_params {
    /** If perform connectable advertising */
    unsigned int connectable:1;

    /** If perform scannable advertising */
    unsigned int scannable:1;

    /** If perform directed advertising */
    unsigned int directed:1;

    /** If perform high-duty directed advertising */
    unsigned int high_duty_directed:1;

    /** If use legacy PDUs for advertising */
    unsigned int legacy_pdu:1;

    /** If perform anonymous advertising */
    unsigned int anonymous:1;

    /** If include TX power in advertising PDU */
    unsigned int include_tx_power:1;

    /** If enable scan request notification  */
    unsigned int scan_req_notif:1;

    /** Minimum advertising interval in 0.625ms units, if 0 stack use sane
     *  defaults
     */
    uint32_t itvl_min;

    /** Maximum advertising interval in 0.625ms units, if 0 stack use sane
     *  defaults
     */
    uint32_t itvl_max;

    /** Advertising channel map , if 0 stack use sane defaults */
    uint8_t channel_map;

    /** Own address type to be used by advertising instance */
    uint8_t own_addr_type;

    /** Peer address for directed advertising, valid only if directed is set */
    ble_addr_t peer;

    /** Advertising  Filter policy */
    uint8_t filter_policy;

    /** Primary advertising PHY to use , can be one of following constants:
     *  - BLE_HCI_LE_PHY_1M
     *  - BLE_HCI_LE_PHY_CODED
     */
    uint8_t primary_phy;

    /** Secondary advertising PHY to use, can be one of following constants:
     *  - BLE_HCI_LE_PHY_1M
     *  - LE_HCI_LE_PHY_2M
     *  - BLE_HCI_LE_PHY_CODED
     */
    uint8_t secondary_phy;

    /** Preferred advertiser transmit power */
    int8_t tx_power;

    /** Advertising Set ID */
    uint8_t sid;
};

/**
 * Configure extended advertising instance
 *
 * @param instance            Instance ID
 * @param params              Additional arguments specifying the particulars
 *                            of the advertising.
 * @param selected_tx_power   Selected advertising transmit power will be
 *                            stored in that param if non-NULL.
 * @param cb                  The callback to associate with this advertising
 *                            procedure. Advertising complete event is reported
 *                            through this callback
 * @param cb_arg              The optional argument to pass to the callback
 *                            function.
 *
 * @return                      0 on success; nonzero on failure.
 */
int ble_gap_ext_adv_configure(uint8_t instance,
                              const struct ble_gap_ext_adv_params *params,
                              int8_t *selected_tx_power,
                              ble_gap_event_fn *cb, void *cb_arg);

/**
 * Set random address for configured advertising instance.
 *
 * @param instance            Instance ID
 * @param addr                Random address to be set
 *
 * @return                      0 on success; nonzero on failure.
 */
int ble_gap_ext_adv_set_addr(uint8_t instance, const ble_addr_t *addr);

/**
 * Start advertising instance.
 *
 * @param instance            Instance ID
 * @param duration            The duration of the advertisement procedure. On
 *                            expiration, the procedure ends and
 *                            a BLE_HS_FOREVER  event is reported.
 *                            Units are milliseconds. Specify 0 for no
 *                            expiration.
 * @params max_events         Number of advertising events that should be sent
 *                            before advertising ends and
 *                            a BLE_GAP_EVENT_ADV_COMPLETE event is reported.
 *                            Specify 0 for no limit.
 *
 * @return              0 on success, error code on failure.
 */
int ble_gap_ext_adv_start(uint8_t instance, int duration, int max_events);

/**
 * Stops advertising procedure for specified instance.
 *
 * @param instance            Instance ID
 *
 * @return  0 on success, BLE_HS_EALREADY if there is no active advertising
 *          procedure for instance, other error code on failure.
 */
int ble_gap_ext_adv_stop(uint8_t instance);

/**
 * Configures the data to include in advertisements packets for specified
 * advertising instance.
 *
 * @param instance            Instance ID
 * @param data                Chain containing the advertising data.
 *
 * @return          0 on success or error code on failure.
 */
int ble_gap_ext_adv_set_data(uint8_t instance, struct os_mbuf *data);

/**
 * Configures the data to include in subsequent scan responses for specified
 * advertisign instance.
 *
 * @param instance            Instance ID
 * @param data                Chain containing the scan response data.
 *
 * @return          0 on success or error code on failure.
 */

int ble_gap_ext_adv_rsp_set_data(uint8_t instance, struct os_mbuf *data);

/**
 * Remove existing advertising instance.
 *
 * @param instance            Instance ID
 *
 * @return              0 on success,
 *                      BLE_HS_EBUSY if advertising is in progress,
 *                      other error code on failure.
 */
int ble_gap_ext_adv_remove(uint8_t instance);

/**
 * Clear all existing advertising instances
 * @return              0 on success,
 *                      BLE_HS_EBUSY if advertising is in progress,
 *                      other error code on failure.
 */
int ble_gap_ext_adv_clear(void);
#endif

/* Periodic Advertising */
#if MYNEWT_VAL(BLE_PERIODIC_ADV)

/** @brief Periodic advertising parameters  */
struct ble_gap_periodic_adv_params {
    /** If include TX power in advertising PDU */
    unsigned int include_tx_power:1;

    /** Minimum advertising interval in 0.625ms units, if 0 stack use sane
     *  defaults
     */
    uint16_t itvl_min;

    /** Maximum advertising interval in 0.625ms units, if 0 stack use sane
     *  defaults
     */
    uint16_t itvl_max;
};

/** @brief Periodic sync parameters  */
struct ble_gap_periodic_sync_params {
    /** The maximum number of periodic advertising events that controller can
     * skip after a successful receive.
     * */
    uint16_t skip;

    /** Synchronization timeout for the periodic advertising train in 10ms units
     */
    uint16_t sync_timeout;
};

/**
 * Configure periodic advertising for specified advertising instance
 *
 * This is allowed only for instances configured as non-announymous,
 * non-connectable and non-scannable.
 *
 * @param instance            Instance ID
 * @param params              Additional arguments specifying the particulars
 *                            of periodic advertising.
 *
 * @return                    0 on success; nonzero on failure.
 */
int ble_gap_periodic_adv_configure(uint8_t instance,
                                   const struct ble_gap_periodic_adv_params *params);

/**
 * Start periodic advertising for specified advertising instance.
 *
 * @param instance            Instance ID
 *
 * @return              0 on success, error code on failure.
 */
int ble_gap_periodic_adv_start(uint8_t instance);

/**
 * Stop periodic advertising for specified advertising instance.
 *
 * @param instance            Instance ID
 *
 * @return              0 on success, error code on failure.
 */
int ble_gap_periodic_adv_stop(uint8_t instance);

/**
 * Configures the data to include in periodic advertisements for specified
 * advertising instance.
 *
 * @param instance            Instance ID
 * @param data                Chain containing the periodic advertising data.
 *
 * @return          0 on success or error code on failure.
 */
int ble_gap_periodic_adv_set_data(uint8_t instance, struct os_mbuf *data);

/**
 * Performs the Synchronization procedure with periodic advertiser.
 *
 * @param addr               Peer address to synchronize with. If NULL than
 *                           peers from periodic list are used.
 * @param adv_sid            Advertiser Set ID
 * @param params             Additional arguments specifying the particulars
 *                           of the synchronization procedure.
 * @param cb                 The callback to associate with this synchrnization
 *                           procedure. BLE_GAP_EVENT_PERIODIC_REPORT events
 *                           are reported only by this callback.
 * @param cb_arg             The optional argument to pass to the callback
 *                           function.
 *
 * @return                   0 on success; nonzero on failure.
 */
int ble_gap_periodic_adv_create_sync(const ble_addr_t *addr, uint8_t adv_sid,
                                     const struct ble_gap_periodic_sync_params *params,
                                     ble_gap_event_fn *cb, void *cb_arg);

/**
 * Cancel pending synchronization procedure.
 *
 * @return                   0 on success; nonzero on failure.
 */
int ble_gap_periodic_adv_create_sync_cancel(void);

/**
 * Terminate synchronization procedure.
 *
 * @param sync_handle        Handle identifying synchronization to terminate.
 *
 * @return                   0 on success; nonzero on failure.
 */
int ble_gap_periodic_adv_terminate_sync(uint16_t sync_handle);

/**
 * Add peer device to periodic synchronization list.
 *
 * @param addr               Peer address to add to list.
 * @param adv_sid            Advertiser Set ID
 *
 * @return                   0 on success; nonzero on failure.
 */
int ble_gap_add_dev_to_periodic_adv_list(const ble_addr_t *peer_addr,
                                         uint8_t adv_sid);

/**
 * Remove peer device from periodic synchronization list.
 *
 * @param addr               Peer address to remove from list.
 * @param adv_sid            Advertiser Set ID
 *
 * @return                   0 on success; nonzero on failure.
 */
int ble_gap_rem_dev_from_periodic_adv_list(const ble_addr_t *peer_addr,
                                           uint8_t adv_sid);

/**
 * Clear periodic synchrnization list.
 *
 * @return                   0 on success; nonzero on failure.
 */
int ble_gap_clear_periodic_adv_list(void);

/**
 * Get periodic synchronization list size.
 *
 * @param per_adv_list_size  On success list size is stored here.
 *
 * @return                   0 on success; nonzero on failure.
 */
int ble_gap_read_periodic_adv_list_size(uint8_t *per_adv_list_size);
#endif


/**
 * Performs the Limited or General Discovery Procedures.
 *
 * @param own_addr_type         The type of address the stack should use for
 *                                  itself when sending scan requests.  Valid
 *                                  values are:
 *                                      - BLE_ADDR_TYPE_PUBLIC
 *                                      - BLE_ADDR_TYPE_RANDOM
 *                                      - BLE_ADDR_TYPE_RPA_PUB_DEFAULT
 *                                      - BLE_ADDR_TYPE_RPA_RND_DEFAULT
 *                                  This parameter is ignored unless active
 *                                  scanning is being used.
 * @param duration_ms           The duration of the discovery procedure.
 *                                  On expiration, the procedure ends and a
 *                                  BLE_GAP_EVENT_DISC_COMPLETE event is
 *                                  reported.  Units are milliseconds.  Specify
 *                                  BLE_HS_FOREVER for no expiration.
 * @param disc_params           Additional arguments specifying the particulars
 *                                  of the discovery procedure.
 * @param cb                    The callback to associate with this discovery
 *                                  procedure.  Advertising reports and
 *                                  discovery termination events are reported
 *                                  through this callback.
 * @param cb_arg                The optional argument to pass to the callback
 *                                  function.
 *
 * @return                      0 on success; nonzero on failure.
 */
int ble_gap_disc(uint8_t own_addr_type, int32_t duration_ms,
                 const struct ble_gap_disc_params *disc_params,
                 ble_gap_event_fn *cb, void *cb_arg);

/**
 * Performs the Limited or General Extended Discovery Procedures.
 *
 * @param own_addr_type         The type of address the stack should use for
 *                              itself when sending scan requests.  Valid
 *                              values are:
 *                                      - BLE_ADDR_TYPE_PUBLIC
 *                                      - BLE_ADDR_TYPE_RANDOM
 *                                      - BLE_ADDR_TYPE_RPA_PUB_DEFAULT
 *                                      - BLE_ADDR_TYPE_RPA_RND_DEFAULT
 *                              This parameter is ignored unless active
 *                              scanning is being used.
 * @param duration              The duration of the discovery procedure.
 *                                  On expiration, if period is set to 0, the
 *                                  procedure ends and a
 *                                  BLE_GAP_EVENT_DISC_COMPLETE event is
 *                                  reported.  Units are 10 milliseconds.
 *                                  Specify 0 for no expiration.
 * @param period                Time interval from when the Controller started
 *                              its last Scan Duration until it begins the
 *                              subsequent Scan Duration. Specify 0 to scan
 *                              continuously. Units are 1.28 second.
 * @param limited               If limited discovery procedure should be used.
 * @param uncoded_params        Additional arguments specifying the particulars
 *                              of the discovery procedure for uncoded PHY.
 *                              If NULL is provided no scan is performed for
 *                              this PHY.
  * @param coded_params         Additional arguments specifying the particulars
 *                              of the discovery procedure for coded PHY.
 *                              If NULL is provided no scan is performed for
 *                              this PHY.
 * @param cb                    The callback to associate with this discovery
 *                              procedure.  Advertising reports and discovery
 *                              termination events are reported through this
 *                              callback.
 * @param cb_arg                The optional argument to pass to the callback
 *                              function.
 *
 * @return                      0 on success; nonzero on failure.
 */
int ble_gap_ext_disc(uint8_t own_addr_type, uint16_t duration, uint16_t period,
                     uint8_t filter_duplicates, uint8_t filter_policy,
                     uint8_t limited,
                     const struct ble_gap_ext_disc_params *uncoded_params,
                     const struct ble_gap_ext_disc_params *coded_params,
                     ble_gap_event_fn *cb, void *cb_arg);

/**
 * Cancels the discovery procedure currently in progress.  A success return
 * code indicates that scanning has been fully aborted; a new discovery or
 * connect procedure can be initiated immediately.
 *
 * @return                      0 on success;
 *                              BLE_HS_EALREADY if there is no discovery
 *                                  procedure to cancel;
 *                              Other nonzero on unexpected error.
 */
int ble_gap_disc_cancel(void);

/**
 * Indicates whether a discovery procedure is currently in progress.
 *
 * @return                      0: No discovery procedure in progress;
 *                              1: Discovery procedure in progress.
 */
int ble_gap_disc_active(void);

/**
 * Initiates a connect procedure.
 *
 * @param own_addr_type         The type of address the stack should use for
 *                                  itself during connection establishment.
 *                                      - BLE_OWN_ADDR_PUBLIC
 *                                      - BLE_OWN_ADDR_RANDOM
 *                                      - BLE_OWN_ADDR_RPA_PUBLIC_DEFAULT
 *                                      - BLE_OWN_ADDR_RPA_RANDOM_DEFAULT
 * @param peer_addr             The address of the peer to connect to.
 *                                  If this parameter is NULL, the white list
 *                                  is used.
 * @param duration_ms           The duration of the discovery procedure.
 *                                  On expiration, the procedure ends and a
 *                                  BLE_GAP_EVENT_DISC_COMPLETE event is
 *                                  reported.  Units are milliseconds.
 * @param conn_params           Additional arguments specifying the particulars
 *                                  of the connect procedure.  Specify null for
 *                                  default values.
 * @param cb                    The callback to associate with this connect
 *                                  procedure.  When the connect procedure
 *                                  completes, the result is reported through
 *                                  this callback.  If the connect procedure
 *                                  succeeds, the connection inherits this
 *                                  callback as its event-reporting mechanism.
 * @param cb_arg                The optional argument to pass to the callback
 *                                  function.
 *
 * @return                      0 on success;
 *                              BLE_HS_EALREADY if a connection attempt is
 *                                  already in progress;
 *                              BLE_HS_EBUSY if initiating a connection is not
 *                                  possible because scanning is in progress;
 *                              BLE_HS_EDONE if the specified peer is already
 *                                  connected;
 *                              Other nonzero on error.
 */
int ble_gap_connect(uint8_t own_addr_type, const ble_addr_t *peer_addr,
                    int32_t duration_ms,
                    const struct ble_gap_conn_params *params,
                    ble_gap_event_fn *cb, void *cb_arg);

/**
 * Initiates an extended connect procedure.
 *
 * @param own_addr_type         The type of address the stack should use for
 *                                  itself during connection establishment.
 *                                      - BLE_OWN_ADDR_PUBLIC
 *                                      - BLE_OWN_ADDR_RANDOM
 *                                      - BLE_OWN_ADDR_RPA_PUBLIC_DEFAULT
 *                                      - BLE_OWN_ADDR_RPA_RANDOM_DEFAULT
 * @param peer_addr             The address of the peer to connect to.
 *                                  If this parameter is NULL, the white list
 *                                  is used.
 * @param duration_ms           The duration of the discovery procedure.
 *                                  On expiration, the procedure ends and a
 *                                  BLE_GAP_EVENT_DISC_COMPLETE event is
 *                                  reported.  Units are milliseconds.
 * @param phy_mask              Define on which PHYs connection attempt should
 *                                  be done
 * @param phy_1m_conn_params     Additional arguments specifying the
 *                                  particulars of the connect procedure. When
 *                                  BLE_GAP_LE_PHY_1M_MASK is set in phy_mask
 *                                  this parameter can be specify to null for
 *                                  default values.
 * @param phy_2m_conn_params     Additional arguments specifying the
 *                                  particulars of the connect procedure. When
 *                                  BLE_GAP_LE_PHY_2M_MASK is set in phy_mask
 *                                  this parameter can be specify to null for
 *                                  default values.
 * @param phy_coded_conn_params  Additional arguments specifying the
 *                                  particulars of the connect procedure. When
 *                                  BLE_GAP_LE_PHY_CODED_MASK is set in
 *                                  phy_mask this parameter can be specify to
 *                                  null for default values.
 * @param cb                    The callback to associate with this connect
 *                                  procedure.  When the connect procedure
 *                                  completes, the result is reported through
 *                                  this callback.  If the connect procedure
 *                                  succeeds, the connection inherits this
 *                                  callback as its event-reporting mechanism.
 * @param cb_arg                The optional argument to pass to the callback
 *                                  function.
 *
 * @return                      0 on success;
 *                              BLE_HS_EALREADY if a connection attempt is
 *                                  already in progress;
 *                              BLE_HS_EBUSY if initiating a connection is not
 *                                  possible because scanning is in progress;
 *                              BLE_HS_EDONE if the specified peer is already
 *                                  connected;
 *                              Other nonzero on error.
 */
int ble_gap_ext_connect(uint8_t own_addr_type, const ble_addr_t *peer_addr,
                        int32_t duration_ms, uint8_t phy_mask,
                        const struct ble_gap_conn_params *phy_1m_conn_params,
                        const struct ble_gap_conn_params *phy_2m_conn_params,
                        const struct ble_gap_conn_params *phy_coded_conn_params,
                        ble_gap_event_fn *cb, void *cb_arg);

/**
 * Aborts a connect procedure in progress.
 *
 * @return                      0 on success;
 *                              BLE_HS_EALREADY if there is no active connect
 *                                  procedure.
 *                              Other nonzero on error.
 */
int ble_gap_conn_cancel(void);

/**
 * Indicates whether a connect procedure is currently in progress.
 *
 * @return                      0: No connect procedure in progress;
 *                              1: Connect procedure in progress.
 */
int ble_gap_conn_active(void);

/**
 * Terminates an established connection.
 *
 * @param conn_handle           The handle corresponding to the connection to
 *                                  terminate.
 * @param hci_reason            The HCI error code to indicate as the reason
 *                                  for termination.
 *
 * @return                      0 on success;
 *                              BLE_HS_ENOTCONN if there is no connection with
 *                                  the specified handle;
 *                              Other nonzero on failure.
 */
int ble_gap_terminate(uint16_t conn_handle, uint8_t hci_reason);

/**
 * Overwrites the controller's white list with the specified contents.
 *
 * @param addrs                 The entries to write to the white list.
 * @param white_list_count      The number of entries in the white list.
 *
 * @return                      0 on success; nonzero on failure.
 */
int ble_gap_wl_set(const ble_addr_t *addrs, uint8_t white_list_count);

/**
 * Initiates a connection parameter update procedure.
 *
 * @param conn_handle           The handle corresponding to the connection to
 *                                  update.
 * @param params                The connection parameters to attempt to update
 *                                  to.
 *
 * @return                      0 on success;
 *                              BLE_HS_ENOTCONN if the there is no connection
 *                                  with the specified handle;
 *                              BLE_HS_EALREADY if a connection update
 *                                  procedure for this connection is already in
 *                                  progress;
 *                              BLE_HS_EINVAL if requested parameters are
 *                                  invalid;
 *                              Other nonzero on error.
 */
int ble_gap_update_params(uint16_t conn_handle,
                          const struct ble_gap_upd_params *params);

/**
 * Initiates the GAP security procedure.
 *
 * Depending on connection role and stored security information this function
 * will start appropriate security procedure (pairing or encryption).
 *
 * @param conn_handle           The handle corresponding to the connection to
 *                              secure.
 *
 * @return                      0 on success;
 *                              BLE_HS_ENOTCONN if the there is no connection
 *                                  with the specified handle;
 *                              BLE_HS_EALREADY if an security procedure for
 *                                  this connection is already in progress;
 *                              Other nonzero on error.
 */
int ble_gap_security_initiate(uint16_t conn_handle);

/**
 * Initiates the GAP pairing procedure as a master. This is for testing only and
 * should not be used by application. Use ble_gap_security_initiate() instead.
 *
 * @param conn_handle           The handle corresponding to the connection to
 *                              start pairing on.
 *
 * @return                      0 on success;
 *                              BLE_HS_ENOTCONN if the there is no connection
 *                                  with the specified handle;
 *                              BLE_HS_EALREADY if an pairing procedure for
 *                                  this connection is already in progress;
 *                              Other nonzero on error.
 */
int ble_gap_pair_initiate(uint16_t conn_handle);

/**
 * Initiates the GAP encryption procedure as a master. This is for testing only
 * and should not be used by application. Use ble_gap_security_initiate()
 * instead.
 *
 * @param conn_handle           The handle corresponding to the connection to
 *                              start encryption.
 * @param key_size              Encryption key size
 * @param ltk                   Long Term Key to be used for encryption.
 * @param udiv                  Encryption Diversifier for LTK
 * @param rand_val              Random Value for EDIV and LTK
 * @param auth                  If LTK provided is authenticated.
 *
 * @return                      0 on success;
 *                              BLE_HS_ENOTCONN if the there is no connection
 *                                  with the specified handle;
 *                              BLE_HS_EALREADY if an encryption procedure for
 *                                  this connection is already in progress;
 *                              Other nonzero on error.
 */
int ble_gap_encryption_initiate(uint16_t conn_handle, uint8_t key_size,
                                const uint8_t *ltk, uint16_t ediv,
                                uint64_t rand_val, int auth);

/**
 * Retrieves the most-recently measured RSSI for the specified connection.  A
 * connection's RSSI is updated whenever a data channel PDU is received.
 *
 * @param conn_handle           Specifies the connection to query.
 * @param out_rssi              On success, the retrieved RSSI is written here.
 *
 * @return                      0 on success;
 *                              A BLE host HCI return code if the controller
 *                                  rejected the request;
 *                              A BLE host core return code on unexpected
 *                                  error.
 */
int ble_gap_conn_rssi(uint16_t conn_handle, int8_t *out_rssi);

/**
 * Unpairs a device with the specified address. The keys related to that peer
 * device are removed from storage and peer address is removed from the resolve
 * list from the controller. If a peer is connected, the connection is terminated.
 *
 * @param peer_addr             Address of the device to be unpaired
 *
 * @return                      0 on success;
 *                              A BLE host HCI return code if the controller
 *                                  rejected the request;
 *                              A BLE host core return code on unexpected
 *                                  error.
 */
int ble_gap_unpair(const ble_addr_t *peer_addr);

/**
 * Unpairs the oldest bonded peer device. The keys related to that peer
 * device are removed from storage and peer address is removed from the resolve
 * list from the controller. If a peer is connected, the connection is terminated.
 *
 * @return                      0 on success;
 *                              A BLE host HCI return code if the controller
 *                                  rejected the request;
 *                              A BLE host core return code on unexpected
 *                                  error.
 */
int ble_gap_unpair_oldest_peer(void);

/**
 * Similar to `ble_gap_unpair_oldest_peer()`, except it makes sure that the
 * peer received in input parameters is not deleted.
 *
 * @param peer_addr             Address of the peer (not to be deleted)
 *
 * @return                      0 on success;
 *                              A BLE host HCI return code if the controller
 *                                  rejected the request;
 *                              A BLE host core return code on unexpected
 *                                  error.
 */
int ble_gap_unpair_oldest_except(const ble_addr_t *peer_addr);

#define BLE_GAP_PRIVATE_MODE_NETWORK        0
#define BLE_GAP_PRIVATE_MODE_DEVICE         1

/**
 * Set privacy mode for specified peer device
 *
 * @param peer_addr          Peer device address
 * @param priv_mode          Privacy mode to be used. Can be one of following
 *                           constants:
 *                             - BLE_GAP_PRIVATE_MODE_NETWORK
 *                             - BLE_GAP_PRIVATE_MODE_DEVICE
 *
 * @return                   0 on success; nonzero on failure.
 */
int ble_gap_set_priv_mode(const ble_addr_t *peer_addr, uint8_t priv_mode);

#define BLE_GAP_LE_PHY_1M                   1
#define BLE_GAP_LE_PHY_2M                   2
#define BLE_GAP_LE_PHY_CODED                3
/**
 * Read PHYs used for specified connection.
 *
 * On success output parameters are filled with information about used PHY type.
 *
 * @param conn_handle       Connection handle
 * @param tx_phy            TX PHY used. Can be one of following constants:
 *                            - BLE_GAP_LE_PHY_1M
 *                            - BLE_GAP_LE_PHY_2M
 *                            - BLE_GAP_LE_PHY_CODED
 * @param rx_phy            RX PHY used. Can be one of following constants:
 *                            - BLE_GAP_LE_PHY_1M
 *                            - BLE_GAP_LE_PHY_2M
 *                            - BLE_GAP_LE_PHY_CODED
 *
 * @return                   0 on success; nonzero on failure.
 */
int ble_gap_read_le_phy(uint16_t conn_handle, uint8_t *tx_phy, uint8_t *rx_phy);

#define BLE_GAP_LE_PHY_1M_MASK              0x01
#define BLE_GAP_LE_PHY_2M_MASK              0x02
#define BLE_GAP_LE_PHY_CODED_MASK           0x04
#define BLE_GAP_LE_PHY_ANY_MASK             0x0F
/**
 * Set preferred default PHYs to be used for connections.
 *
 * @params tx_phys_mask     Preferred TX PHY. Can be mask of following
 *                          constants:
 *                          - BLE_GAP_LE_PHY_1M_MASK
 *                          - BLE_GAP_LE_PHY_2M_MASK
 *                          - BLE_GAP_LE_PHY_CODED_MASK
 *                          - BLE_GAP_LE_PHY_ANY_MASK
 * @params rx_phys_mask     Preferred RX PHY. Can be mask of following
 *                          constants:
 *                          - BLE_GAP_LE_PHY_1M_MASK
 *                          - BLE_GAP_LE_PHY_2M_MASK
 *                          - BLE_GAP_LE_PHY_CODED_MASK
 *                          - BLE_GAP_LE_PHY_ANY_MASK

 * @return                   0 on success; nonzero on failure.
 */
int ble_gap_set_prefered_default_le_phy(uint8_t tx_phys_mask,
                                        uint8_t rx_phys_mask);

#define BLE_GAP_LE_PHY_CODED_ANY            0
#define BLE_GAP_LE_PHY_CODED_S2             1
#define BLE_GAP_LE_PHY_CODED_S8             2
/**
 * Set preferred PHYs to be used for connection.
 *
 * @param conn_handle       Connection handle
 * @params tx_phys_mask     Preferred TX PHY. Can be mask of following
 *                          constants:
 *                          - BLE_GAP_LE_PHY_1M_MASK
 *                          - BLE_GAP_LE_PHY_2M_MASK
 *                          - BLE_GAP_LE_PHY_CODED_MASK
 *                          - BLE_GAP_LE_PHY_ANY_MASK
 * @params rx_phys_mask     Preferred RX PHY. Can be mask of following
 *                          constants:
 *                          - BLE_GAP_LE_PHY_1M_MASK
 *                          - BLE_GAP_LE_PHY_2M_MASK
 *                          - BLE_GAP_LE_PHY_CODED_MASK
 *                          - BLE_GAP_LE_PHY_ANY_MASK
 * @param phy_opts          Additional PHY options. Valid values are:
 *                          - BLE_GAP_LE_PHY_CODED_ANY
 *                          - BLE_GAP_LE_PHY_CODED_S2
 *                          - BLE_GAP_LE_PHY_CODED_S8
 *
 * @return                   0 on success; nonzero on failure.
 */
int ble_gap_set_prefered_le_phy(uint16_t conn_handle, uint8_t tx_phys_mask,
                                uint8_t rx_phys_mask, uint16_t phy_opts);

/**
 * Event listener structure
 *
 * This should be used as an opaque structure and not modified manually.
 */
struct ble_gap_event_listener {
    ble_gap_event_fn *fn;
    void *arg;
    SLIST_ENTRY(ble_gap_event_listener) link;
};

/**
 * Registers listener for GAP events
 *
 * On success listener structure will be initialized automatically and does not
 * need to be initialized prior to calling this function. To change callback
 * and/or argument unregister listener first and register it again.
 *
 * @param listener      Listener structure
 * @param fn            Callback function
 * @param arg           Callback argument
 *
 * @return              0 on success
 *                      BLE_HS_EINVAL if no callback is specified
 *                      BLE_HS_EALREADY if listener is already registered
 */
int ble_gap_event_listener_register(struct ble_gap_event_listener *listener,
                                    ble_gap_event_fn *fn, void *arg);

/**
 * Unregisters listener for GAP events
 *
 * @param listener      Listener structure
 *
 * @return              0 on success
 *                      BLE_HS_ENOENT if listener was not registered
 */
int ble_gap_event_listener_unregister(struct ble_gap_event_listener *listener);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif
