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
#ifndef ESP_PLATFORM
 
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#include "nimble/nimble/include/nimble/ble.h"
#include "nimble/nimble/include/nimble/nimble_opt.h"
#include "nimble/nimble/include/nimble/hci_common.h"
#include "../include/controller/ble_ll.h"
#include "../include/controller/ble_ll_hci.h"
#include "../include/controller/ble_ll_ctrl.h"
#include "../include/controller/ble_ll_trace.h"
#include "../include/controller/ble_hw.h"
#include "../include/controller/ble_ll_sync.h"
#include "ble_ll_conn_priv.h"

/* To use spec sample data for testing */
#undef BLE_LL_ENCRYPT_USE_TEST_DATA

/*
 * For console debug to show session key calculation. NOTE: if you define
 * this the stack requirements for the LL task go up considerably. The
 * default stack will not be enough and must be increased.
 */
#undef BLE_LL_ENCRYPT_DEBUG
#ifdef BLE_LL_ENCRYPT_DEBUG
#include "console/console.h"
#endif

/*
 * XXX:
 *  1) Do I need to keep track of which procedures have already been done?
 *     Do I need to worry about repeating procedures?
 *  2) Should we create pool of control pdu's?. Dont need more
 *  than the # of connections and can probably deal with quite a few less
 *  if we have lots of connections.
 *  3) What about procedures that have been completed but try to restart?
 *  4) NOTE: there is a supported features procedure. However, in the case
 *  of data length extension, if the receiving device does not understand
 *  the pdu or it does not support data length extension, the LL_UNKNOWN_RSP
 *  pdu is sent. That needs to be processed...
 *  5) We are supposed to remember when we do the data length update proc if
 *  the device sent us an unknown rsp. We should not send it another len req.
 *  Implement this how? Through remote supported features?
 *  8) How to count control pdus sent. DO we count enqueued + sent, or only
 *  sent (actually attempted to tx). Do we count failures? How?
 */

/*
 * XXX: I definitely have an issue with control procedures and connection
 * param request procedure and connection update procedure. This was
 * noted when receiving an unknown response. Right now, I am getting confused
 * with connection parameter request and updates regarding which procedures
 * are running. So I need to go look through all the code and see where I
 * used the request procedure and the update procedure and make sure I am doing
 * the correct thing.
 */

/*
 * This array contains the length of the CtrData field in LL control PDU's.
 * Note that there is a one byte opcode which precedes this field in the LL
 * control PDU, so total data channel payload length for the control pdu is
 * one greater.
 */
const uint8_t g_ble_ll_ctrl_pkt_lengths[BLE_LL_CTRL_OPCODES] =
{
    BLE_LL_CTRL_CONN_UPD_REQ_LEN,
    BLE_LL_CTRL_CHAN_MAP_LEN,
    BLE_LL_CTRL_TERMINATE_IND_LEN,
    BLE_LL_CTRL_ENC_REQ_LEN,
    BLE_LL_CTRL_ENC_RSP_LEN,
    BLE_LL_CTRL_START_ENC_REQ_LEN,
    BLE_LL_CTRL_START_ENC_RSP_LEN,
    BLE_LL_CTRL_UNK_RSP_LEN,
    BLE_LL_CTRL_FEATURE_LEN,
    BLE_LL_CTRL_FEATURE_LEN,
    BLE_LL_CTRL_PAUSE_ENC_REQ_LEN,
    BLE_LL_CTRL_PAUSE_ENC_RSP_LEN,
    BLE_LL_CTRL_VERSION_IND_LEN,
    BLE_LL_CTRL_REJ_IND_LEN,
    BLE_LL_CTRL_SLAVE_FEATURE_REQ_LEN,
    BLE_LL_CTRL_CONN_PARAMS_LEN,
    BLE_LL_CTRL_CONN_PARAMS_LEN,
    BLE_LL_CTRL_REJECT_IND_EXT_LEN,
    BLE_LL_CTRL_PING_LEN,
    BLE_LL_CTRL_PING_LEN,
    BLE_LL_CTRL_LENGTH_REQ_LEN,
    BLE_LL_CTRL_LENGTH_REQ_LEN,
    BLE_LL_CTRL_PHY_REQ_LEN,
    BLE_LL_CTRL_PHY_RSP_LEN,
    BLE_LL_CTRL_PHY_UPD_IND_LEN,
    BLE_LL_CTRL_MIN_USED_CHAN_LEN,
    BLE_LL_CTRL_CTE_REQ_LEN,
    BLE_LL_CTRL_CTE_RSP_LEN,
    BLE_LL_CTRL_PERIODIC_SYNC_IND_LEN,
    BLE_LL_CTRL_CLOCK_ACCURACY_REQ_LEN,
    BLE_LL_CTRL_CLOCK_ACCURACY_RSP_LEN,
    BLE_LL_CTRL_CIS_REQ_LEN,
    BLE_LL_CTRL_CIS_RSP_LEN,
    BLE_LL_CTRL_CIS_IND_LEN,
    BLE_LL_CTRL_CIS_TERMINATE_LEN
};

/**
 * Called to determine if a LL control procedure with an instant has
 * been initiated.
 *
 * If the function returns a 0 it means no conflicting procedure has
 * been initiated. Otherwise it returns the appropriate BLE error code to
 * send.
 *
 * @param connsm Pointer to connection state machine.
 * @param req_ctrl_proc The procedure that the peer is trying to initiate
 *
 * @return uint8_t
 */
uint8_t
ble_ll_ctrl_proc_with_instant_initiated(struct ble_ll_conn_sm *connsm,
                                        uint8_t req_ctrl_proc)
{
    uint8_t err;

    switch (connsm->cur_ctrl_proc) {
    case BLE_LL_CTRL_PROC_PHY_UPDATE:
    case BLE_LL_CTRL_PROC_CONN_UPDATE:
    case BLE_LL_CTRL_PROC_CONN_PARAM_REQ:
    case BLE_LL_CTRL_PROC_CHAN_MAP_UPD:
        if (req_ctrl_proc == connsm->cur_ctrl_proc) {
            err = BLE_ERR_LMP_COLLISION;
        } else if ((connsm->cur_ctrl_proc == BLE_LL_CTRL_PROC_CONN_UPDATE) &&
                   (req_ctrl_proc == BLE_LL_CTRL_PROC_CONN_PARAM_REQ)) {
            err = BLE_ERR_LMP_COLLISION;
        } else {
            err = BLE_ERR_DIFF_TRANS_COLL;
        }
        break;
    default:
        err = 0;
    }

    return err;
}

/**
 * Create a LL_REJECT_EXT_IND pdu.
 *
 * @param rej_opcode Opcode to be rejected.
 * @param err: error response
 * @param ctrdata: Pointer to where CtrData starts in pdu
 */
void
ble_ll_ctrl_rej_ext_ind_make(uint8_t rej_opcode, uint8_t err, uint8_t *ctrdata)
{
    ctrdata[0] = rej_opcode;
    ctrdata[1] = err;
}

#if (BLE_LL_BT5_PHY_SUPPORTED == 1)
/**
 * Called to cancel a phy update procedure.
 *
 * @param connsm
 * @param ble_err
 */
void
ble_ll_ctrl_phy_update_cancel(struct ble_ll_conn_sm *connsm, uint8_t ble_err)
{
    /* cancel any pending phy update procedures */
    CLR_PENDING_CTRL_PROC(connsm, BLE_LL_CTRL_PROC_PHY_UPDATE);

    /* Check if the host wants an event */
    if (CONN_F_HOST_PHY_UPDATE(connsm)) {
        ble_ll_hci_ev_phy_update(connsm, ble_err);
        CONN_F_HOST_PHY_UPDATE(connsm) = 0;
    }

    /* Clear any bits for phy updates that might be in progress */
    CONN_F_CTRLR_PHY_UPDATE(connsm) = 0;
}
#endif

static int
ble_ll_ctrl_len_proc(struct ble_ll_conn_sm *connsm, uint8_t *dptr)
{
    int rc;
    struct ble_ll_len_req ctrl_req;

    /* Extract parameters and check if valid */
    ctrl_req.max_rx_bytes = get_le16(dptr);
    ctrl_req.max_rx_time = get_le16(dptr + 2);
    ctrl_req.max_tx_bytes = get_le16(dptr + 4);
    ctrl_req.max_tx_time = get_le16(dptr + 6);

    if ((ctrl_req.max_rx_bytes < BLE_LL_CONN_SUPP_BYTES_MIN) ||
        (ctrl_req.max_rx_time < BLE_LL_CONN_SUPP_TIME_MIN) ||
        (ctrl_req.max_tx_bytes < BLE_LL_CONN_SUPP_BYTES_MIN) ||
        (ctrl_req.max_tx_time < BLE_LL_CONN_SUPP_TIME_MIN)) {
        rc = 1;
    } else {
        /* Update parameters */
        connsm->rem_max_rx_time = ctrl_req.max_rx_time;
        connsm->rem_max_tx_time = ctrl_req.max_tx_time;
        connsm->rem_max_rx_octets = ctrl_req.max_rx_bytes;
        connsm->rem_max_tx_octets = ctrl_req.max_tx_bytes;

        /* Recalculate effective connection parameters */
        ble_ll_conn_update_eff_data_len(connsm);
        rc = 0;
    }

    return rc;
}

/**
 * Process a received LL_PING_RSP control pdu.
 *
 * NOTE: we dont have to reset the callout since this packet will have had a
 * valid MIC and that will restart the authenticated payload timer
 *
 * @param connsm
 */
static void
ble_ll_ctrl_rx_ping_rsp(struct ble_ll_conn_sm *connsm)
{
    /* Stop the control procedure */
    ble_ll_ctrl_proc_stop(connsm, BLE_LL_CTRL_PROC_LE_PING);
}

/**
 * Called when we receive either a connection parameter request or response.
 *
 * @param connsm
 * @param dptr
 * @param rspbuf
 * @param opcode
 *
 * @return int
 */
static int
ble_ll_ctrl_conn_param_pdu_proc(struct ble_ll_conn_sm *connsm, uint8_t *dptr,
                                uint8_t *rspbuf, uint8_t opcode)
{
    int rc;
    int indicate;
    uint8_t rsp_opcode;
    uint8_t ble_err;
    struct ble_ll_conn_params *req;
    struct hci_conn_update *hcu;

    /* Extract parameters and check if valid */
    req = &connsm->conn_cp;
    req->interval_min = get_le16(dptr);
    req->interval_max = get_le16(dptr + 2);
    req->latency = get_le16(dptr + 4);
    req->timeout = get_le16(dptr + 6);
    req->pref_periodicity = dptr[8];
    req->ref_conn_event_cnt  = get_le16(dptr + 9);
    req->offset0 = get_le16(dptr + 11);
    req->offset1 = get_le16(dptr + 13);
    req->offset2 = get_le16(dptr + 15);
    req->offset3 = get_le16(dptr + 17);
    req->offset4 = get_le16(dptr + 19);
    req->offset5 = get_le16(dptr + 21);

    /* Check if parameters are valid */
    ble_err = BLE_ERR_SUCCESS;
    rc = ble_ll_conn_hci_chk_conn_params(req->interval_min,
                                         req->interval_max,
                                         req->latency,
                                         req->timeout);
    if (rc) {
        ble_err = BLE_ERR_INV_LMP_LL_PARM;
        goto conn_param_pdu_exit;
    }

    /*
     * Check if there is a requested change to either the interval, timeout
     * or latency. If not, this may just be an anchor point change and we do
     * not have to notify the host.
     *  XXX: what if we dont like the parameters? When do we check that out?
     */
    indicate = 1;
    if (opcode == BLE_LL_CTRL_CONN_PARM_REQ) {
        if ((connsm->conn_itvl >= req->interval_min) &&
            (connsm->conn_itvl <= req->interval_max) &&
            (connsm->supervision_tmo == req->timeout) &&
            (connsm->slave_latency == req->latency)) {
            indicate = 0;
            goto conn_parm_req_do_indicate;
        }
    }

    /*
     * A change has been requested. Is it within the values specified by
     * the host? Note that for a master we will not be processing a
     * connect param request from a slave if we are currently trying to
     * update the connection parameters. This means that the previous
     * check is all we need for a master (when receiving a request).
     */
    if ((connsm->conn_role == BLE_LL_CONN_ROLE_SLAVE) ||
        (opcode == BLE_LL_CTRL_CONN_PARM_RSP)) {
        /*
         * Not sure what to do about the slave. It is possible that the
         * current connection parameters are not the same ones as the local host
         * has provided? Not sure what to do here. Do we need to remember what
         * host sent us? For now, I will assume that we need to remember what
         * the host sent us and check it out.
         */
        hcu = &connsm->conn_param_req;
        if (hcu->handle != 0) {
            if (!((req->interval_min < hcu->conn_itvl_min) ||
                  (req->interval_min > hcu->conn_itvl_max) ||
                  (req->interval_max < hcu->conn_itvl_min) ||
                  (req->interval_max > hcu->conn_itvl_max) ||
                  (req->latency != hcu->conn_latency) ||
                  (req->timeout != hcu->supervision_timeout))) {
                indicate = 0;
            }
        }
    }

conn_parm_req_do_indicate:
    /*
     * XXX: are the connection update parameters acceptable? If not, we will
     * need to know before we indicate to the host that they are acceptable.
     */
    if (indicate) {
        /* If Host masked out Remote Connection Parameter Request Event, we need to
         * send Reject back to the remote device
         */
        if (!ble_ll_hci_is_le_event_enabled(BLE_HCI_LE_SUBEV_REM_CONN_PARM_REQ)){
            ble_err = BLE_ERR_UNSUPP_REM_FEATURE;
            goto conn_param_pdu_exit;
        }

        /*
         * Send event to host. At this point we leave and wait to get
         * an answer.
         */
        ble_ll_hci_ev_rem_conn_parm_req(connsm, req);
        connsm->host_reply_opcode = opcode;
        connsm->csmflags.cfbit.awaiting_host_reply = 1;
        rsp_opcode = 255;
    } else {
        /* Create reply to connection request */
        rsp_opcode = ble_ll_ctrl_conn_param_reply(connsm, rspbuf, req);
    }

conn_param_pdu_exit:
    if (ble_err) {
        rsp_opcode = BLE_LL_CTRL_REJECT_IND_EXT;
        rspbuf[1] = opcode;
        rspbuf[2] = ble_err;
    }
    return rsp_opcode;
}

/**
 * Called to make a connection update request LL control PDU
 *
 * Context: Link Layer
 *
 * @param connsm
 * @param rsp
 */
static void
ble_ll_ctrl_conn_upd_make(struct ble_ll_conn_sm *connsm, uint8_t *pyld,
                          struct ble_ll_conn_params *cp)
{
    uint16_t instant;
    uint32_t dt;
    uint32_t num_old_ce;
    uint32_t new_itvl_usecs;
    uint32_t old_itvl_usecs;
    struct hci_conn_update *hcu;
    struct ble_ll_conn_upd_req *req;

    /*
     * Set instant. We set the instant to the current event counter plus
     * the amount of slave latency as the slave may not be listening
     * at every connection interval and we are not sure when the connect
     * request will actually get sent. We add one more event plus the
     * minimum as per the spec of 6 connection events.
     */
    instant = connsm->event_cntr + connsm->slave_latency + 6 + 1;

    /*
     * XXX: This should change in the future, but for now we will just
     * start the new instant at the same anchor using win offset 0.
     */
    /* Copy parameters in connection update structure */
    hcu = &connsm->conn_param_req;
    req = &connsm->conn_update_req;
    if (cp) {
        /* XXX: so we need to make the new anchor point some time away
         * from txwinoffset by some amount of msecs. Not sure how to do
           that here. We dont need to, but we should. */
        /* Calculate offset from requested offsets (if any) */
        if (cp->offset0 != 0xFFFF) {
            new_itvl_usecs = cp->interval_max * BLE_LL_CONN_ITVL_USECS;
            old_itvl_usecs = connsm->conn_itvl * BLE_LL_CONN_ITVL_USECS;
            if ((int16_t)(cp->ref_conn_event_cnt - instant) >= 0) {
                num_old_ce = cp->ref_conn_event_cnt - instant;
                dt = old_itvl_usecs * num_old_ce;
                dt += (cp->offset0 * BLE_LL_CONN_ITVL_USECS);
                dt = dt % new_itvl_usecs;
            } else {
                num_old_ce = instant - cp->ref_conn_event_cnt;
                dt = old_itvl_usecs * num_old_ce;
                dt -= (cp->offset0 * BLE_LL_CONN_ITVL_USECS);
                dt = dt % new_itvl_usecs;
                dt = new_itvl_usecs - dt;
            }
            req->winoffset = dt / BLE_LL_CONN_TX_WIN_USECS;
        } else {
            req->winoffset = 0;
        }
        req->interval = cp->interval_max;
        req->timeout = cp->timeout;
        req->latency = cp->latency;
        req->winsize = 1;
    } else {
        req->interval = hcu->conn_itvl_max;
        req->timeout = hcu->supervision_timeout;
        req->latency = hcu->conn_latency;
        req->winoffset = 0;
        req->winsize = connsm->tx_win_size;
    }
    req->instant = instant;

    /* XXX: make sure this works for the connection parameter request proc. */
    pyld[0] = req->winsize;
    put_le16(pyld + 1, req->winoffset);
    put_le16(pyld + 3, req->interval);
    put_le16(pyld + 5, req->latency);
    put_le16(pyld + 7, req->timeout);
    put_le16(pyld + 9, instant);

    /* Set flag in state machine to denote we have scheduled an update */
    connsm->csmflags.cfbit.conn_update_sched = 1;
}

/**
 * Called to process and UNKNOWN_RSP LL control packet.
 *
 * Context: Link Layer Task
 *
 * @param dptr
 */
static int
ble_ll_ctrl_proc_unk_rsp(struct ble_ll_conn_sm *connsm, uint8_t *dptr, uint8_t *rspdata)
{
    uint8_t ctrl_proc;
    uint8_t opcode;

    /* Get opcode of unknown LL control frame */
    opcode = dptr[0];

    /* Convert opcode to control procedure id */
    switch (opcode) {
    case BLE_LL_CTRL_LENGTH_REQ:
        ctrl_proc = BLE_LL_CTRL_PROC_DATA_LEN_UPD;
        BLE_LL_CONN_CLEAR_FEATURE(connsm, BLE_LL_FEAT_DATA_LEN_EXT);
        break;
    case BLE_LL_CTRL_CONN_UPDATE_IND:
        ctrl_proc = BLE_LL_CTRL_PROC_CONN_UPDATE;
        break;
    case BLE_LL_CTRL_SLAVE_FEATURE_REQ:
        ctrl_proc = BLE_LL_CTRL_PROC_FEATURE_XCHG;
        BLE_LL_CONN_CLEAR_FEATURE(connsm, BLE_LL_FEAT_SLAVE_INIT);
        break;
    case BLE_LL_CTRL_CONN_PARM_REQ:
        BLE_LL_CONN_CLEAR_FEATURE(connsm, BLE_LL_FEAT_CONN_PARM_REQ);
        if (connsm->conn_role == BLE_LL_CONN_ROLE_MASTER) {
            ble_ll_ctrl_conn_upd_make(connsm, rspdata, NULL);
            connsm->reject_reason = BLE_ERR_SUCCESS;
            return BLE_LL_CTRL_CONN_UPDATE_IND;
        }
        /* Else falls through. */
        /* note: fall-through intentional */
    case BLE_LL_CTRL_CONN_PARM_RSP:
        ctrl_proc = BLE_LL_CTRL_PROC_CONN_PARAM_REQ;
        break;
    case BLE_LL_CTRL_PING_REQ:
        /* LL can authenticate remote device even if remote device does not
         * support LE Ping feature.
         */
        ctrl_proc = BLE_LL_CTRL_PROC_LE_PING;
        BLE_LL_CONN_CLEAR_FEATURE(connsm, BLE_LL_FEAT_LE_PING);
        break;
#if (BLE_LL_BT5_PHY_SUPPORTED ==1)
    case BLE_LL_CTRL_PHY_REQ:
        ble_ll_ctrl_phy_update_cancel(connsm, BLE_ERR_UNSUPP_REM_FEATURE);
        ctrl_proc = BLE_LL_CTRL_PROC_PHY_UPDATE;
        break;
#endif
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_SCA_UPDATE)
    case BLE_LL_CTRL_CLOCK_ACCURACY_REQ:
        ble_ll_hci_ev_sca_update(connsm, BLE_ERR_UNSUPPORTED, 0);
        ctrl_proc = BLE_LL_CTRL_PROC_SCA_UPDATE;
        break;
#endif
    default:
        ctrl_proc = BLE_LL_CTRL_PROC_NUM;
        break;
    }

    /* If we are running this one currently, stop it */
    if (connsm->cur_ctrl_proc == ctrl_proc) {
        /* Stop the control procedure */
        ble_ll_ctrl_proc_stop(connsm, ctrl_proc);
        if (ctrl_proc == BLE_LL_CTRL_PROC_CONN_PARAM_REQ) {
            ble_ll_hci_ev_conn_update(connsm, BLE_ERR_UNSUPP_REM_FEATURE);
        } else if (ctrl_proc == BLE_LL_CTRL_PROC_FEATURE_XCHG) {
            if (connsm->csmflags.cfbit.pending_hci_rd_features) {
                ble_ll_hci_ev_rd_rem_used_feat(connsm,
                                                   BLE_ERR_UNSUPP_REM_FEATURE);
            }
            connsm->csmflags.cfbit.pending_hci_rd_features = 0;
        }
    }

    return BLE_ERR_MAX;
}

/**
 * Callback when LL control procedure times out (for a given connection). If
 * this is called, it means that we need to end the connection because it
 * has not responded to a LL control request.
 *
 * Context: Link Layer
 *
 * @param arg Pointer to connection state machine.
 */
static void
ble_ll_ctrl_proc_rsp_timer_cb(struct ble_npl_event *ev)
{
    /* Control procedure has timed out. Kill the connection */
    ble_ll_conn_timeout((struct ble_ll_conn_sm *)ble_npl_event_get_arg(ev),
                        BLE_ERR_LMP_LL_RSP_TMO);
}

static void
ble_ll_ctrl_start_rsp_timer(struct ble_ll_conn_sm *connsm)
{
    ble_npl_callout_init(&connsm->ctrl_proc_rsp_timer,
                    &g_ble_ll_data.ll_evq,
                    ble_ll_ctrl_proc_rsp_timer_cb,
                    connsm);

    /* Re-start timer. Control procedure timeout is 40 seconds */
    ble_npl_callout_reset(&connsm->ctrl_proc_rsp_timer,
                     ble_npl_time_ms_to_ticks32(BLE_LL_CTRL_PROC_TIMEOUT_MS));
}

/**
 * Convert a phy mask to a numeric phy value.
 *
 * NOTE: only one bit should be set here and there should be at least one.
 * If this function returns a 0 it is an error!
 *
 * @param phy_mask Bitmask of phy
 *
 * @return uint8_t The numeric value associated with the phy mask
 *
 * BLE_HCI_LE_PHY_1M                    (1)
 * BLE_HCI_LE_PHY_2M                    (2)
 * BLE_HCI_LE_PHY_CODED                 (3)
 */
uint8_t
ble_ll_ctrl_phy_from_phy_mask(uint8_t phy_mask)
{
    uint8_t phy;

    /*
     * NOTE: wipe out unsupported PHYs. There should not be an unsupported
     * in this mask if the other side is working correctly.
     */
#if !MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_2M_PHY)
    phy_mask &= ~BLE_HCI_LE_PHY_2M_PREF_MASK;
#endif
#if !MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CODED_PHY)
    phy_mask &= ~BLE_HCI_LE_PHY_CODED_PREF_MASK;
#endif

    if (phy_mask & BLE_PHY_MASK_1M) {
        phy = BLE_PHY_1M;
        phy_mask &= ~BLE_PHY_MASK_1M;
    } else if (phy_mask & BLE_PHY_MASK_2M) {
        phy = BLE_PHY_2M;
        phy_mask &= ~BLE_PHY_MASK_2M;
    } else if (phy_mask & BLE_PHY_MASK_CODED) {
        phy = BLE_PHY_CODED;
        phy_mask &= ~BLE_PHY_MASK_CODED;
    } else {
        phy = 0;
    }

    if (phy_mask != 0) {
        phy = 0;
    }

    return phy;
}

#if (BLE_LL_BT5_PHY_SUPPORTED == 1)
uint8_t
ble_ll_ctrl_phy_tx_transition_get(uint8_t phy_mask)
{
    /*
     * Evaluate PHYs in transition starting from the one with longest TX time
     * so we select the one that allows shortest payload to be sent. This is
     * to make sure we do not violate timing restriction on new PHY.
     */
    if (phy_mask & BLE_PHY_MASK_CODED) {
        return BLE_PHY_CODED;
    } else if (phy_mask & BLE_PHY_MASK_1M) {
        return BLE_PHY_1M;
    } else if (phy_mask & BLE_PHY_MASK_2M) {
        return BLE_PHY_2M;
    }

    return 0;
}

void
ble_ll_ctrl_phy_update_proc_complete(struct ble_ll_conn_sm *connsm)
{
    int chk_proc_stop;
    int chk_host_phy;

    chk_proc_stop = 1;
    chk_host_phy = 1;

    connsm->phy_tx_transition = 0;

    if (CONN_F_PEER_PHY_UPDATE(connsm)) {
        CONN_F_PEER_PHY_UPDATE(connsm) = 0;
    } else if (CONN_F_CTRLR_PHY_UPDATE(connsm)) {
        CONN_F_CTRLR_PHY_UPDATE(connsm) = 0;
    } else {
        /* Must be a host-initiated update */
        CONN_F_HOST_PHY_UPDATE(connsm) = 0;
        chk_host_phy = 0;
        if (CONN_F_PHY_UPDATE_EVENT(connsm) == 0) {
            ble_ll_hci_ev_phy_update(connsm, BLE_ERR_SUCCESS);
        }
    }

    /* Must check if we need to start host procedure */
    if (chk_host_phy) {
        if (CONN_F_HOST_PHY_UPDATE(connsm)) {
            if (ble_ll_conn_chk_phy_upd_start(connsm)) {
                CONN_F_HOST_PHY_UPDATE(connsm) = 0;
            } else {
                chk_proc_stop = 0;
            }
        }
    }

    if (chk_proc_stop) {
        ble_ll_ctrl_proc_stop(connsm, BLE_LL_CTRL_PROC_PHY_UPDATE);
    }
}

/**
 *
 *  There is probably a better way for the controller to choose which PHY use.
 *  There are no BER metrics and RSSI does not give you S/N so for now we will
 *  choose this heirarchy:
 *     -> if 2Mbps available, use it.
 *     -> If 1Mbps available, use it.
 *     -> otherwise use coded phy.
 *
 * @param prefs The mask of preferred phys
 * @return uint8_t The phy to use (not a mask)
 */
static uint8_t
ble_ll_ctrl_find_new_phy(uint8_t phy_mask_prefs)
{
    uint8_t new_phy;

    new_phy = phy_mask_prefs;
    if (new_phy) {
        if (new_phy & BLE_PHY_MASK_2M) {
            new_phy = BLE_PHY_2M;
        } else if (new_phy & BLE_PHY_MASK_1M) {
            new_phy = BLE_PHY_1M;
        } else {
            new_phy = BLE_PHY_CODED;
        }
    }

    return new_phy;
}

/**
 * Create a LL_PHY_UPDATE_IND pdu
 *
 * @param connsm Pointer to connection state machine
 * @param dptr Pointer to PHY_REQ or PHY_RSP data.
 * @param ctrdata: Pointer to where CtrData of UPDATE_IND pdu starts
 * @param slave_req flag denoting if slave requested this. 0: no 1:yes
 */
static void
ble_ll_ctrl_phy_update_ind_make(struct ble_ll_conn_sm *connsm, uint8_t *dptr,
                                uint8_t *ctrdata, int slave_req)
{
    uint8_t m_to_s;
    uint8_t s_to_m;
    uint8_t tx_phys;
    uint8_t rx_phys;
    uint16_t instant;
    uint8_t is_slave_sym = 0;

    /* Get preferences from PDU */
    tx_phys = dptr[0];
    rx_phys = dptr[1];

    /* If we are master, check if slave requested symmetric PHY */
    if (connsm->conn_role == BLE_LL_CONN_ROLE_MASTER) {
        is_slave_sym = tx_phys == rx_phys;
        is_slave_sym &= __builtin_popcount(tx_phys) == 1;
    }

    /* Get m_to_s and s_to_m masks */
    if (slave_req) {
        m_to_s = connsm->phy_data.host_pref_tx_phys_mask & rx_phys;
        s_to_m = connsm->phy_data.host_pref_rx_phys_mask & tx_phys;
    } else {
        m_to_s = connsm->phy_data.req_pref_tx_phys_mask & rx_phys;
        s_to_m = connsm->phy_data.req_pref_rx_phys_mask & tx_phys;
    }

    if (is_slave_sym) {
        /*
         * If either s_to_m or m_to_s is 0, it means for at least one direction
         * requested PHY is not our preferred one so make sure we keep current
         * PHY in both directions
         *
         * Core 5.2, Vol 6, PartB, 5.1.10
         *     If the slave specified a single PHY in both the TX_PHYS and
         *     RX_PHYS fields and both fields are the same, the master shall
         *     either select the PHY specified by the slave for both directions
         *     or shall leave both directions unchanged.
         */
        if ((s_to_m == 0) || (m_to_s == 0)) {
            s_to_m = 0;
            m_to_s = 0;
        } else {
            BLE_LL_ASSERT(s_to_m == m_to_s);
        }
    }

    /* Calculate new PHYs to use */
    m_to_s = ble_ll_ctrl_find_new_phy(m_to_s);
    s_to_m = ble_ll_ctrl_find_new_phy(s_to_m);

    /* Make sure we do not indicate PHY change if the same as current one */
    if (m_to_s == connsm->phy_data.cur_tx_phy) {
        m_to_s = 0;
    }
    if (s_to_m == connsm->phy_data.cur_rx_phy) {
        s_to_m = 0;
    }

    /* At this point, m_to_s and s_to_m are not masks; they are numeric */

    /*
     * If not changing we still send update ind. Check if hosts expects
     * the event and if so send it. Stop control procedure if it is the
     * one running.
     */
    if ((m_to_s == 0) && (s_to_m == 0)) {
        if (CONN_F_PEER_PHY_UPDATE(connsm)) {
            CONN_F_PEER_PHY_UPDATE(connsm) = 0;
        } else if (CONN_F_CTRLR_PHY_UPDATE(connsm)) {
            CONN_F_CTRLR_PHY_UPDATE(connsm) = 0;
            ble_ll_ctrl_proc_stop(connsm, BLE_LL_CTRL_PROC_PHY_UPDATE);
        } else {
            ble_ll_hci_ev_phy_update(connsm, BLE_ERR_SUCCESS);
            CONN_F_HOST_PHY_UPDATE(connsm) = 0;
            ble_ll_ctrl_proc_stop(connsm, BLE_LL_CTRL_PROC_PHY_UPDATE);
        }
        instant = 0;
    } else {
        /* Determine instant we will use. 6 more is minimum */
        instant = connsm->event_cntr + connsm->slave_latency + 6 + 1;
        connsm->phy_instant = instant;
        CONN_F_PHY_UPDATE_SCHED(connsm) = 1;

        /* Set new phys to use when instant occurs */
        connsm->phy_data.new_tx_phy = m_to_s;
        connsm->phy_data.new_rx_phy = s_to_m;

        /* Convert m_to_s and s_to_m to masks */
        if (m_to_s) {
            m_to_s = 1 << (m_to_s - 1);
        }

        if (s_to_m) {
            s_to_m = 1 << (s_to_m - 1);
        }
    }

    ctrdata[0] = m_to_s;
    ctrdata[1] = s_to_m;
    put_le16(ctrdata + 2, instant);
}

/**
 * Create a LL_PHY_REQ or LL_PHY_RSP pdu
 *
 * @param connsm Pointer to connection state machine
 * @param ctrdata: Pointer to where CtrData starts in pdu
 */
static void
ble_ll_ctrl_phy_req_rsp_make(struct ble_ll_conn_sm *connsm, uint8_t *ctrdata)
{
    /* If no preference we use current phy */
    if (connsm->phy_data.host_pref_tx_phys_mask == 0) {
        ctrdata[0] = CONN_CUR_TX_PHY_MASK(connsm);
    } else {
        ctrdata[0] = connsm->phy_data.host_pref_tx_phys_mask;
    }
    if (connsm->phy_data.host_pref_rx_phys_mask == 0) {
        ctrdata[1] = CONN_CUR_RX_PHY_MASK(connsm);
    } else {
        ctrdata[1] = connsm->phy_data.host_pref_rx_phys_mask;
    }
}

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_SCA_UPDATE)
/**
 * Create a LL_CLOCK_ACCURACY_REQ or LL_CLOCK_ACCURACY_RSP pdu
 *
 * @param connsm Pointer to connection state machine
 * @param ctrdata: Pointer to where CtrData starts in pdu
 */
static void
ble_ll_ctrl_sca_req_rsp_make(struct ble_ll_conn_sm *connsm, uint8_t *ctrdata)
{
    ctrdata[0] = BLE_LL_SCA_ENUM;
}
#endif

static uint8_t
ble_ll_ctrl_rx_phy_req(struct ble_ll_conn_sm *connsm, uint8_t *req,
                       uint8_t *rsp)
{
    uint8_t rsp_opcode;
    uint8_t err;

    /*
     * XXX: TODO if we have an instant in progress we should end connection.
     * At least it seems that is the case. Need to figure out more from
     * the spec here.
     */

    /* Check if we have already initiated a procedure with an instant */
    err = ble_ll_ctrl_proc_with_instant_initiated(connsm,
                                                  BLE_LL_CTRL_PROC_PHY_UPDATE);

    if (connsm->conn_role == BLE_LL_CONN_ROLE_MASTER) {
        if (err) {
            ble_ll_ctrl_rej_ext_ind_make(BLE_LL_CTRL_PHY_REQ, err, rsp);
            rsp_opcode = BLE_LL_CTRL_REJECT_IND_EXT;
        } else {
            /*
             * NOTE: do not change order of these two lines as the call to
             * make the LL_PHY_UPDATE_IND pdu might clear the flag.
             */
            CONN_F_PEER_PHY_UPDATE(connsm) = 1;
            ble_ll_ctrl_phy_update_ind_make(connsm, req, rsp, 1);
            rsp_opcode = BLE_LL_CTRL_PHY_UPDATE_IND;
        }
    } else {
        /* XXX: deal with other control procedures that we need to stop */
        if (err) {
            if (connsm->cur_ctrl_proc == BLE_LL_CTRL_PROC_PHY_UPDATE) {
                ble_npl_callout_stop(&connsm->ctrl_proc_rsp_timer);
                connsm->cur_ctrl_proc = BLE_LL_CTRL_PROC_IDLE;
            }

            /* If there is a PHY update procedure pending cancel it */
            ble_ll_ctrl_phy_update_cancel(connsm, err);

            /* XXX: ? Should not be any phy update events */
            CONN_F_PHY_UPDATE_EVENT(connsm) = 0;
        }

        /* XXX: TODO: if we started another procedure with an instant
         * why are we doing this? Need to look into this.*/

        /* Respond to master's phy update procedure */
        CONN_F_PEER_PHY_UPDATE(connsm) = 1;
        ble_ll_ctrl_phy_req_rsp_make(connsm, rsp);
        rsp_opcode = BLE_LL_CTRL_PHY_RSP;

        connsm->phy_tx_transition = ble_ll_ctrl_phy_tx_transition_get(req[1] | rsp[0]);

        /* Start response timer */
        connsm->cur_ctrl_proc = BLE_LL_CTRL_PROC_PHY_UPDATE;
        ble_ll_ctrl_start_rsp_timer(connsm);
    }
    return rsp_opcode;
}

/**
 * Process a received LL_PHY_RSP pdu
 *
 * @param connsm
 * @param dptr Pointer to LL_PHY_RSP ctrdata
 * @param rsp Pointer to CtrData of PHY_UPDATE_IND.
 *
 * @return uint8_t
 */
static uint8_t
ble_ll_ctrl_rx_phy_rsp(struct ble_ll_conn_sm *connsm, uint8_t *dptr,
                       uint8_t *rsp)
{
    uint8_t rsp_opcode;

    rsp_opcode = BLE_ERR_MAX;
    if (connsm->conn_role == BLE_LL_CONN_ROLE_MASTER) {
        if (connsm->cur_ctrl_proc == BLE_LL_CTRL_PROC_PHY_UPDATE) {
            ble_ll_ctrl_phy_update_ind_make(connsm, dptr, rsp, 0);
            ble_npl_callout_stop(&connsm->ctrl_proc_rsp_timer);
            rsp_opcode = BLE_LL_CTRL_PHY_UPDATE_IND;
        }

        /*
         * If not in the process of doing this control procedure something
         * is wrong. End connection? Assert?
         *
         * XXX: TODO count some stat?
         */
    } else {
        rsp_opcode = BLE_LL_CTRL_UNKNOWN_RSP;
    }

    /* NOTE: slave should never receive one of these */

    return rsp_opcode;
}

/**
 * Called when a LL_PHY_UPDATE_IND pdu is received
 *
 * NOTE: slave is the only device that should receive this.
 *
 * @param connsm
 * @param dptr
 *
 * @return uint8_t
 */
static uint8_t
ble_ll_ctrl_rx_phy_update_ind(struct ble_ll_conn_sm *connsm, uint8_t *dptr)
{
    int no_change;
    uint8_t new_m_to_s_mask;
    uint8_t new_s_to_m_mask;
    uint8_t new_tx_phy;
    uint8_t new_rx_phy;
    uint16_t instant;
    uint16_t delta;

    if (connsm->conn_role == BLE_LL_CONN_ROLE_MASTER) {
        return BLE_LL_CTRL_UNKNOWN_RSP;
    }

    /*
     * Reception stops the procedure response timer but does not
     * complete the procedure
     */
    if (connsm->cur_ctrl_proc == BLE_LL_CTRL_PROC_PHY_UPDATE) {
        ble_npl_callout_stop(&connsm->ctrl_proc_rsp_timer);
    }

    /*
     * XXX: Should we check to see if we are expecting to receive one
     * of these, and if not, kill connection? Meaning we better be
     * doing either a PEER, CTRLR, or HOST phy update.
     */
    /* get the new phy masks and see if we need to change */
    new_m_to_s_mask = dptr[0];
    new_s_to_m_mask = dptr[1];
    instant = get_le16(dptr + 2);

    if ((new_m_to_s_mask == 0) && (new_s_to_m_mask == 0)) {
        /* No change in phy */
        no_change = 1;
    } else {
        no_change = 0;
        /*
         * NOTE: from the slaves perspective, the m to s phy is the one
         * that the slave will receive on; s to m is the one it will
         * transmit on
         */
        new_rx_phy = ble_ll_ctrl_phy_from_phy_mask(new_m_to_s_mask);
        new_tx_phy = ble_ll_ctrl_phy_from_phy_mask(new_s_to_m_mask);

        if ((new_tx_phy == 0) && (new_rx_phy == 0)) {
            /* XXX: this is an error! What to do??? */
            no_change = 1;
        }

        if ((new_tx_phy == connsm->phy_data.cur_tx_phy) &&
            (new_rx_phy == connsm->phy_data.cur_rx_phy)) {
            no_change = 1;
        }
    }

    if (!no_change) {
        /* If instant is in the past, we have to end the connection */
        delta = (instant - connsm->event_cntr) & 0xFFFF;
        if (delta >= 32767) {
            ble_ll_conn_timeout(connsm, BLE_ERR_INSTANT_PASSED);
        } else {
            connsm->phy_data.new_tx_phy = new_tx_phy;
            connsm->phy_data.new_rx_phy = new_rx_phy;
            connsm->phy_instant = instant;
            CONN_F_PHY_UPDATE_SCHED(connsm) = 1;
        }
        return BLE_ERR_MAX;
    }

    ble_ll_ctrl_phy_update_proc_complete(connsm);

    return BLE_ERR_MAX;
}
#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV_SYNC_TRANSFER)
/**
 * Called when a BLE_LL_CTRL_PERIODIC_SYNC_IND PDU is received
 *
 * @param connsm
 * @param dptr
 *
 * @return uint8_t
 */
static uint8_t
ble_ll_ctrl_rx_periodic_sync_ind(struct ble_ll_conn_sm *connsm, uint8_t *dptr)
{
    if (connsm->sync_transfer_mode) {
        ble_ll_sync_periodic_ind(connsm, dptr, connsm->sync_transfer_mode == 1,
                                 connsm->sync_transfer_skip,
                                 connsm->sync_transfer_sync_timeout);
    }
    return BLE_ERR_MAX;
}
#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_SCA_UPDATE)
/**
 * Called when a BLE_LL_CTRL_CLOCK_ACCURACY_REQ PDU is received
 *
 * @param connsm
 * @param dptr
 * @param rsp Pointer to CtrData of BLE_LL_CTRL_CLOCK_ACCURACY_RSP.
 *
 * @return uint8_t
 */
static uint8_t
ble_ll_ctrl_rx_sca_req(struct ble_ll_conn_sm *connsm, uint8_t *dptr,
                       uint8_t *rsp)
{
    ble_ll_ctrl_sca_req_rsp_make(connsm, rsp);
    return BLE_LL_CTRL_CLOCK_ACCURACY_RSP;
}

/**
 * Called when a BLE_LL_CTRL_CLOCK_ACCURACY_RSP PDU is received
 *
 * @param connsm
 * @param dptr
 *
 * @return uint8_t
 */
static uint8_t
ble_ll_ctrl_rx_sca_rsp(struct ble_ll_conn_sm *connsm, uint8_t *dptr)
{
    if (connsm->cur_ctrl_proc != BLE_LL_CTRL_PROC_SCA_UPDATE) {
        return BLE_LL_CTRL_UNKNOWN_RSP;
    }
    ble_ll_ctrl_proc_stop(connsm, BLE_LL_CTRL_PROC_SCA_UPDATE);
    ble_ll_hci_ev_sca_update(connsm, BLE_ERR_SUCCESS, dptr[0]);
    return BLE_ERR_MAX;
}

#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_ISO)
static uint8_t
ble_ll_ctrl_rx_cis_req(struct ble_ll_conn_sm *connsm, uint8_t *dptr,
                       uint8_t *rspdata)
{
    return BLE_LL_CTRL_UNKNOWN_RSP;
}

static uint8_t
ble_ll_ctrl_rx_cis_rsp(struct ble_ll_conn_sm *connsm, uint8_t *dptr,
                       uint8_t *rspdata)
{
    return BLE_LL_CTRL_UNKNOWN_RSP;
}

static uint8_t
ble_ll_ctrl_rx_cis_ind(struct ble_ll_conn_sm *connsm, uint8_t *dptr)
{
    return BLE_LL_CTRL_UNKNOWN_RSP;
}
#endif
/**
 * Create a link layer length request or length response PDU.
 *
 * NOTE: this function does not set the LL data pdu header nor does it
 * set the opcode in the buffer.
 *
 * @param connsm
 * @param dptr: Pointer to where control pdu payload starts
 */
static void
ble_ll_ctrl_datalen_upd_make(struct ble_ll_conn_sm *connsm, uint8_t *dptr)
{
    put_le16(dptr + 1, connsm->max_rx_octets);
    put_le16(dptr + 3, connsm->max_rx_time);
    put_le16(dptr + 5, connsm->max_tx_octets);
    put_le16(dptr + 7, connsm->max_tx_time);
}

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
void
ble_ll_calc_session_key(struct ble_ll_conn_sm *connsm)
{
#ifdef BLE_LL_ENCRYPT_DEBUG
    int cnt;
#endif

    /* XXX: possibly have some way out of this if this locks up */
    while (1) {
        if (!ble_hw_encrypt_block(&connsm->enc_data.enc_block)) {
            break;
        }
    }

#ifdef BLE_LL_ENCRYPT_DEBUG
    console_printf("Calculating Session Key for handle=%u",
                   connsm->conn_handle);

    console_printf("\nLTK:");
    for (cnt = 0; cnt < 16; ++cnt) {
        console_printf("%02x", connsm->enc_data.enc_block.key[cnt]);
    }
    console_printf("\nSKD:");
    for (cnt = 0; cnt < 16; ++cnt) {
        console_printf("%02x", connsm->enc_data.enc_block.plain_text[cnt]);
    }
    console_printf("\nSession Key:");
    for (cnt = 0; cnt < 16; ++cnt) {
        console_printf("%02x", connsm->enc_data.enc_block.cipher_text[cnt]);
    }
    console_printf("\nIV:");
    for (cnt = 0; cnt < 8; ++ cnt) {
        console_printf("%02x", connsm->enc_data.iv[cnt]);
    }
    console_printf("\n");
#endif
}

/**
 * Called to determine if this is a control PDU we are allowed to send. This
 * is called when a link is being encrypted, as only certain control PDU's
 * area lowed to be sent.
 *
 * XXX: the current code may actually allow some control pdu's to be sent
 * in states where they shouldnt. I dont expect those states to occur so I
 * dont try to check for them but we could do more... for example there are
 * different PDUs allowed for master/slave and TX/RX
 *
 * @param llid
 * @param opcode
 * @param len
 *
 * @return int
 */
static int
ble_ll_ctrl_enc_allowed_pdu(uint8_t llid, uint8_t len, uint8_t opcode)
{
    int allowed;

    allowed = 0;

    switch (llid) {
    case BLE_LL_LLID_CTRL:
        switch (opcode) {
        case BLE_LL_CTRL_REJECT_IND:
        case BLE_LL_CTRL_REJECT_IND_EXT:
        case BLE_LL_CTRL_START_ENC_RSP:
        case BLE_LL_CTRL_START_ENC_REQ:
        case BLE_LL_CTRL_ENC_REQ:
        case BLE_LL_CTRL_ENC_RSP:
        case BLE_LL_CTRL_PAUSE_ENC_REQ:
        case BLE_LL_CTRL_PAUSE_ENC_RSP:
        case BLE_LL_CTRL_TERMINATE_IND:
            allowed = 1;
            break;
        }
        break;
    case BLE_LL_LLID_DATA_FRAG:
        if (len == 0) {
            /* Empty PDUs are allowed */
            allowed = 1;
        }
        break;
    }

    return allowed;
}

int
ble_ll_ctrl_enc_allowed_pdu_rx(struct os_mbuf *rxpdu)
{
    uint8_t llid;
    uint8_t len;
    uint8_t opcode;

    llid = rxpdu->om_data[0] & BLE_LL_DATA_HDR_LLID_MASK;
    len = rxpdu->om_data[1];
    if (llid == BLE_LL_LLID_CTRL) {
        opcode = rxpdu->om_data[2];
    } else {
        opcode = 0;
    }

    return ble_ll_ctrl_enc_allowed_pdu(llid, len, opcode);
}

int
ble_ll_ctrl_enc_allowed_pdu_tx(struct os_mbuf_pkthdr *pkthdr)
{
    struct os_mbuf *m;
    struct ble_mbuf_hdr *ble_hdr;
    uint8_t llid;
    uint8_t len;
    uint8_t opcode;

    m = OS_MBUF_PKTHDR_TO_MBUF(pkthdr);
    ble_hdr = BLE_MBUF_HDR_PTR(m);

    llid = ble_hdr->txinfo.hdr_byte & BLE_LL_DATA_HDR_LLID_MASK;
    len = ble_hdr->txinfo.pyld_len;
    if (llid == BLE_LL_LLID_CTRL) {
        opcode = m->om_data[0];
    } else {
        opcode = 0;
    }

    return ble_ll_ctrl_enc_allowed_pdu(llid, len, opcode);
}

int
ble_ll_ctrl_is_start_enc_rsp(struct os_mbuf *txpdu)
{
    int is_start_enc_rsp;
    uint8_t opcode;
    uint8_t llid;
    struct ble_mbuf_hdr *ble_hdr;

    is_start_enc_rsp = 0;
    ble_hdr = BLE_MBUF_HDR_PTR(txpdu);

    llid = ble_hdr->txinfo.hdr_byte & BLE_LL_DATA_HDR_LLID_MASK;
    if (llid == BLE_LL_LLID_CTRL) {
        opcode = txpdu->om_data[0];
        if (opcode == BLE_LL_CTRL_START_ENC_RSP) {
            is_start_enc_rsp = 1;
        }
    }

    return is_start_enc_rsp;
}

/**
 * Called to create and send a LL_START_ENC_REQ
 *
 * @param connsm
 * @param err
 *
 * @return int
 */
int
ble_ll_ctrl_start_enc_send(struct ble_ll_conn_sm *connsm)
{
    int rc;
    struct os_mbuf *om;

    om = os_msys_get_pkthdr(BLE_LL_CTRL_MAX_PDU_LEN,
                            sizeof(struct ble_mbuf_hdr));
    if (om) {
        om->om_data[0] = BLE_LL_CTRL_START_ENC_REQ;
        ble_ll_conn_enqueue_pkt(connsm, om, BLE_LL_LLID_CTRL, 1);

        /* Wait for LL_START_ENC_RSP. If there is already procedure in progress,
         * LL response timer is already running.
         */
        if (connsm->cur_ctrl_proc == BLE_LL_CTRL_PROC_IDLE) {
            connsm->cur_ctrl_proc = BLE_LL_CTRL_PROC_ENCRYPT;
            ble_ll_ctrl_start_rsp_timer(connsm);
        }

        rc = 0;
    } else {
        rc = -1;
    }
    return rc;
}

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_ISO)
static void
ble_ll_ctrl_cis_create(struct ble_ll_conn_sm *connsm, uint8_t *dptr)
{
    /* TODO Implement */
    return;
}
#endif

/**
 * Create a link layer control "encrypt request" PDU.
 *
 * The LL_ENC_REQ PDU format is:
 *      Rand    (8)
 *      EDIV    (2)
 *      SKDm    (8)
 *      IVm     (4)
 *
 * The random number and encrypted diversifier come from the host command.
 * Controller generates master portion of SDK and IV.
 *
 * NOTE: this function does not set the LL data pdu header nor does it
 * set the opcode in the buffer.
 *
 * @param connsm
 * @param dptr: Pointer to where control pdu payload starts
 */
static void
ble_ll_ctrl_enc_req_make(struct ble_ll_conn_sm *connsm, uint8_t *dptr)
{
    put_le64(dptr, connsm->enc_data.host_rand_num);
    put_le16(dptr + 8, connsm->enc_data.enc_div);

#ifdef BLE_LL_ENCRYPT_USE_TEST_DATA
    /* IV stored LSB to MSB, IVm is LSB, IVs is MSB */
    put_le64(dptr + 10, g_bletest_SKDm);
    swap_buf(connsm->enc_data.enc_block.plain_text + 8, dptr + 10, 8);
    put_le32(dptr + 18, g_bletest_IVm);
    memcpy(connsm->enc_data.iv, dptr + 18, 4);
    return;
#endif

    ble_ll_rand_data_get(connsm->enc_data.enc_block.plain_text + 8, 8);
    swap_buf(dptr + 10, connsm->enc_data.enc_block.plain_text + 8, 8);
    ble_ll_rand_data_get(connsm->enc_data.iv, 4);
    memcpy(dptr + 18, connsm->enc_data.iv, 4);
}

/**
 * Called when LL_ENC_RSP is received by the master.
 *
 * Context: Link Layer Task.
 *
 * Format of the LL_ENC_RSP is:
 *      SKDs (8)
 *      IVs  (4)
 *
 *  The master now has the long term key (from the start encrypt command)
 *  and the SKD (stored in the plain text encryption block). From this the
 *  sessionKey is generated.
 *
 * @param connsm
 * @param dptr
 */
static void
ble_ll_ctrl_rx_enc_rsp(struct ble_ll_conn_sm *connsm, uint8_t *dptr)
{
    /* Calculate session key now that we have received the ENC_RSP */
    if (connsm->cur_ctrl_proc == BLE_LL_CTRL_PROC_ENCRYPT) {
        /* In case we were already encrypted we need to reset packet counters */
        connsm->enc_data.rx_pkt_cntr = 0;
        connsm->enc_data.tx_pkt_cntr = 0;
        connsm->enc_data.tx_encrypted = 0;

        swap_buf(connsm->enc_data.enc_block.plain_text, dptr, 8);
        memcpy(connsm->enc_data.iv + 4, dptr + 8, 4);
        ble_ll_calc_session_key(connsm);
        connsm->enc_data.enc_state = CONN_ENC_S_START_ENC_REQ_WAIT;
    }
}

/**
 * Called when we have received a LL control encryption request PDU. This
 * should only be received by a slave.
 *
 * The LL_ENC_REQ PDU format is:
 *      Rand    (8)
 *      EDIV    (2)
 *      SKDm    (8)
 *      IVm     (4)
 *
 * This function returns the response opcode. Typically this will be ENC_RSP
 * but it could be a reject ind. Note that the caller of this function
 * will send the REJECT_IND_EXT if supported by remote.
 *
 * NOTE: if this is received by a master we will silently discard the PDU
 * (denoted by return BLE_ERR_MAX).
 *
 * @param connsm
 * @param dptr      Pointer to start of encrypt request data.
 * @param rspbuf
 */
static uint8_t
ble_ll_ctrl_rx_enc_req(struct ble_ll_conn_sm *connsm, uint8_t *dptr,
                       uint8_t *rspdata)
{
    if (connsm->conn_role != BLE_LL_CONN_ROLE_SLAVE) {
        return BLE_LL_CTRL_UNKNOWN_RSP;
    }

    connsm->enc_data.enc_state = CONN_ENC_S_ENC_RSP_TO_BE_SENT;

    /* In case we were already encrypted we need to reset packet counters */
    connsm->enc_data.rx_pkt_cntr = 0;
    connsm->enc_data.tx_pkt_cntr = 0;
    connsm->enc_data.tx_encrypted = 0;

    /* Extract information from request */
    connsm->enc_data.host_rand_num = get_le64(dptr);
    connsm->enc_data.enc_div = get_le16(dptr + 8);

#if BLE_LL_ENCRYPT_USE_TEST_DATA
    swap_buf(connsm->enc_data.enc_block.plain_text + 8, dptr + 10, 8);
    memcpy(connsm->enc_data.iv, dptr + 18, 4);

    put_le64(rspdata, g_bletest_SKDs);
    swap_buf(connsm->enc_data.enc_block.plain_text, rspdata, 8);
    put_le32(rspdata + 8, g_bletest_IVs);
    memcpy(connsm->enc_data.iv + 4, rspdata + 8, 4);
    return BLE_LL_CTRL_ENC_RSP;
#endif

    swap_buf(connsm->enc_data.enc_block.plain_text + 8, dptr + 10, 8);
    memcpy(connsm->enc_data.iv, dptr + 18, 4);

    /* Create the ENC_RSP. Concatenate our SKD and IV */
    ble_ll_rand_data_get(connsm->enc_data.enc_block.plain_text, 8);
    swap_buf(rspdata, connsm->enc_data.enc_block.plain_text, 8);
    ble_ll_rand_data_get(connsm->enc_data.iv + 4, 4);
    memcpy(rspdata + 8, connsm->enc_data.iv + 4, 4);

    return BLE_LL_CTRL_ENC_RSP;
}

static uint8_t
ble_ll_ctrl_rx_start_enc_req(struct ble_ll_conn_sm *connsm)
{
    int rc;

    /* Only master should receive start enc request */
    rc = BLE_ERR_MAX;
    if (connsm->conn_role == BLE_LL_CONN_ROLE_MASTER) {
        /* We only want to send a START_ENC_RSP if we havent yet */
        if (connsm->enc_data.enc_state == CONN_ENC_S_START_ENC_REQ_WAIT) {
            connsm->enc_data.enc_state = CONN_ENC_S_START_ENC_RSP_WAIT;
            rc = BLE_LL_CTRL_START_ENC_RSP;
        }
    } else {
        rc = BLE_LL_CTRL_UNKNOWN_RSP;
    }
    return rc;
}

static uint8_t
ble_ll_ctrl_rx_pause_enc_req(struct ble_ll_conn_sm *connsm)
{
    int rc;

    /*
     * The spec does not say what to do here, but if we receive a pause
     * encryption request and we are not encrypted, what do we do? We
     * ignore it...
     */
    rc = BLE_ERR_MAX;
    if ((connsm->conn_role == BLE_LL_CONN_ROLE_SLAVE) &&
        (connsm->enc_data.enc_state == CONN_ENC_S_ENCRYPTED)) {
        rc = BLE_LL_CTRL_PAUSE_ENC_RSP;
    } else {
        rc = BLE_LL_CTRL_UNKNOWN_RSP;
    }

    return rc;
}

/**
 * Called when a LL control pdu with opcode PAUSE_ENC_RSP is received.
 *
 *
 * @param connsm
 *
 * @return uint8_t
 */
static uint8_t
ble_ll_ctrl_rx_pause_enc_rsp(struct ble_ll_conn_sm *connsm)
{
    int rc;

    if (connsm->conn_role == BLE_LL_CONN_ROLE_MASTER) {
        rc = BLE_LL_CTRL_PAUSE_ENC_RSP;
    } else if (connsm->enc_data.enc_state == CONN_ENC_S_PAUSE_ENC_RSP_WAIT) {
        /* Master sends back unencrypted LL_PAUSE_ENC_RSP.
         * From this moment encryption is paused.
         */
        rc = BLE_ERR_MAX;
        connsm->enc_data.enc_state = CONN_ENC_S_PAUSED;
    } else {
        rc = BLE_LL_CTRL_UNKNOWN_RSP;
    }

    return rc;
}

/**
 * Called when we have received a LL_CTRL_START_ENC_RSP.
 *
 * Context: Link-layer task
 *
 * @param connsm
 *
 * @return uint8_t
 */
static uint8_t
ble_ll_ctrl_rx_start_enc_rsp(struct ble_ll_conn_sm *connsm)
{
    int rc;

    /* Not in proper state. Discard */
    if (connsm->enc_data.enc_state != CONN_ENC_S_START_ENC_RSP_WAIT) {
        return BLE_ERR_MAX;
    }

    /* If master, we are done. Stop control procedure and sent event to host */
    if (connsm->conn_role == BLE_LL_CONN_ROLE_MASTER) {

        ble_ll_ctrl_proc_stop(connsm, BLE_LL_CTRL_PROC_ENCRYPT);

        /* We are encrypted */
        connsm->enc_data.enc_state = CONN_ENC_S_ENCRYPTED;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_PING)
        ble_ll_conn_auth_pyld_timer_start(connsm);
#endif
        rc = BLE_ERR_MAX;
    } else {
        /* Procedure has completed but slave needs to send START_ENC_RSP */
        rc = BLE_LL_CTRL_START_ENC_RSP;

        /* Stop timer if it was started when sending START_ENC_REQ */
        if (connsm->cur_ctrl_proc == BLE_LL_CTRL_PROC_ENCRYPT) {
            ble_ll_ctrl_proc_stop(connsm, BLE_LL_CTRL_PROC_ENCRYPT);
        }
    }

    /*
     * XXX: for now, a Slave sends this event when it receivest the
     * START_ENC_RSP from the master. It might be technically incorrect
     * to send it before we transmit our own START_ENC_RSP.
     */
    ble_ll_hci_ev_encrypt_chg(connsm, BLE_ERR_SUCCESS);

    return rc;
}

#endif

/**
 * Called to make a connection parameter request or response control pdu.
 *
 * @param connsm
 * @param dptr Pointer to start of data. NOTE: the opcode is not part
 *             of the data.
 */
static void
ble_ll_ctrl_conn_param_pdu_make(struct ble_ll_conn_sm *connsm, uint8_t *dptr,
                                struct ble_ll_conn_params *req)
{
    uint16_t offset;
    struct hci_conn_update *hcu;

    /* If we were passed in a request, we use the parameters from the request */
    if (req) {
        put_le16(dptr, req->interval_min);
        put_le16(dptr + 2, req->interval_max);
        put_le16(dptr + 4, req->latency);
        put_le16(dptr + 6, req->timeout);
    } else {
        hcu = &connsm->conn_param_req;
        /* The host should have provided the parameters! */
        BLE_LL_ASSERT(hcu->handle != 0);
        put_le16(dptr, hcu->conn_itvl_min);
        put_le16(dptr + 2, hcu->conn_itvl_max);
        put_le16(dptr + 4, hcu->conn_latency);
        put_le16(dptr + 6, hcu->supervision_timeout);
    }

    /* XXX: NOTE: if interval min and interval max are != to each
     * other this value should be set to non-zero. I think this
     * applies only when an offset field is set. See section 5.1.7.1 pg 103
     * Vol 6 Part B.
     */
    /* XXX: for now, set periodicity to 0 */
    dptr[8] = 0;

    /* XXX: deal with reference event count. what to put here? */
    put_le16(dptr + 9, connsm->event_cntr);

    /* XXX: For now, dont use offsets */
    offset = 0xFFFF;
    put_le16(dptr + 11, offset);
    put_le16(dptr + 13, offset);
    put_le16(dptr + 15, offset);
    put_le16(dptr + 17, offset);
    put_le16(dptr + 19, offset);
    put_le16(dptr + 21, offset);
}

static void
ble_ll_ctrl_version_ind_make(struct ble_ll_conn_sm *connsm, uint8_t *pyld)
{
    /* Set flag to denote we have sent/received this */
    connsm->csmflags.cfbit.version_ind_sent = 1;

    /* Fill out response */
    pyld[0] = BLE_HCI_VER_BCS;
    put_le16(pyld + 1, MYNEWT_VAL(BLE_LL_MFRG_ID));
    put_le16(pyld + 3, BLE_LL_SUB_VERS_NR);
}

/**
 * Called to make a LL control channel map request PDU.
 *
 * @param connsm    Pointer to connection state machine
 * @param pyld      Pointer to payload of LL control PDU
 */
static void
ble_ll_ctrl_chanmap_req_make(struct ble_ll_conn_sm *connsm, uint8_t *pyld)
{
    /* Copy channel map that host desires into request */
    memcpy(pyld, g_ble_ll_conn_params.master_chan_map, BLE_LL_CONN_CHMAP_LEN);
    memcpy(connsm->req_chanmap, pyld, BLE_LL_CONN_CHMAP_LEN);

    /* Place instant into request */
    connsm->chanmap_instant = connsm->event_cntr + connsm->slave_latency + 6 + 1;
    put_le16(pyld + BLE_LL_CONN_CHMAP_LEN, connsm->chanmap_instant);

    /* Set scheduled flag */
    connsm->csmflags.cfbit.chanmap_update_scheduled = 1;
}

/**
 * Called to respond to a LL control PDU connection parameter request or
 * response.
 *
 * @param connsm
 * @param rsp
 * @param req
 *
 * @return uint8_t
 */
uint8_t
ble_ll_ctrl_conn_param_reply(struct ble_ll_conn_sm *connsm, uint8_t *rsp,
                             struct ble_ll_conn_params *req)
{
    uint8_t rsp_opcode;

    if (connsm->conn_role == BLE_LL_CONN_ROLE_SLAVE) {
        /* Create a connection parameter response */
        ble_ll_ctrl_conn_param_pdu_make(connsm, rsp + 1, req);
        rsp_opcode = BLE_LL_CTRL_CONN_PARM_RSP;
    } else {
        /* Create a connection update pdu */
        ble_ll_ctrl_conn_upd_make(connsm, rsp + 1, req);
        rsp_opcode = BLE_LL_CTRL_CONN_UPDATE_IND;
    }

    return rsp_opcode;
}

/**
 * Called when we have received a LL_REJECT_IND or LL_REJECT_IND_EXT link
 * layer control Data Channel pdu.
 *
 * @param connsm
 * @param dptr
 * @param opcode
 */
static int
ble_ll_ctrl_rx_reject_ind(struct ble_ll_conn_sm *connsm, uint8_t *dptr,
                          uint8_t opcode, uint8_t *rspdata)
{
    uint8_t ble_error;
    uint8_t rsp_opcode = BLE_ERR_MAX;

    /* Get error out of received PDU */
    if (opcode == BLE_LL_CTRL_REJECT_IND) {
        ble_error = dptr[0];
    } else {
        ble_error = dptr[1];
    }

    /* XXX: should I check to make sure the rejected opcode is sane
       if we receive ind ext? */
    switch (connsm->cur_ctrl_proc) {
    case BLE_LL_CTRL_PROC_CONN_PARAM_REQ:
        if (opcode == BLE_LL_CTRL_REJECT_IND_EXT) {
            /* As a master we should send connection update indication in this point */
            if (connsm->conn_role == BLE_LL_CONN_ROLE_MASTER) {
                rsp_opcode = BLE_LL_CTRL_CONN_UPDATE_IND;
                ble_ll_ctrl_conn_upd_make(connsm, rspdata, NULL);
                connsm->reject_reason = BLE_ERR_SUCCESS;
            } else {
                ble_ll_ctrl_proc_stop(connsm, BLE_LL_CTRL_PROC_CONN_PARAM_REQ);
                ble_ll_hci_ev_conn_update(connsm, ble_error);
            }
        }
        break;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
    case BLE_LL_CTRL_PROC_ENCRYPT:
        ble_ll_ctrl_proc_stop(connsm, BLE_LL_CTRL_PROC_ENCRYPT);
        ble_ll_hci_ev_encrypt_chg(connsm, ble_error);
        connsm->enc_data.enc_state = CONN_ENC_S_UNENCRYPTED;
        break;
#endif
#if (BLE_LL_BT5_PHY_SUPPORTED == 1)
    case BLE_LL_CTRL_PROC_PHY_UPDATE:
        ble_ll_ctrl_phy_update_cancel(connsm, ble_error);
        ble_ll_ctrl_proc_stop(connsm, BLE_LL_CTRL_PROC_PHY_UPDATE);
        break;
#endif
    case BLE_LL_CTRL_PROC_DATA_LEN_UPD:
        /* That should not happen according to Bluetooth 5.0 Vol6 Part B, 5.1.9
         * However we need this workaround as there are devices on the market
         * which do send LL_REJECT on LL_LENGTH_REQ when collision happens
         */
        ble_ll_ctrl_proc_stop(connsm, BLE_LL_CTRL_PROC_DATA_LEN_UPD);
        break;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_SCA_UPDATE)
    case BLE_LL_CTRL_PROC_SCA_UPDATE:
        ble_ll_hci_ev_sca_update(connsm, ble_error, 0);
        ble_ll_ctrl_proc_stop(connsm, BLE_LL_CTRL_PROC_SCA_UPDATE);
        break;
#endif
    default:
        break;
    }

    return rsp_opcode;
}

/**
 * Called when we receive a connection update event
 *
 * @param connsm
 * @param dptr
 *
 * @return int
 */
static int
ble_ll_ctrl_rx_conn_update(struct ble_ll_conn_sm *connsm, uint8_t *dptr)
{
    uint8_t rsp_opcode;
    uint16_t conn_events;
    struct ble_ll_conn_upd_req *reqdata;

    /* Only a slave should receive this */
    if (connsm->conn_role == BLE_LL_CONN_ROLE_MASTER) {
        return BLE_LL_CTRL_UNKNOWN_RSP;
    }

    /* Retrieve parameters */
    reqdata = &connsm->conn_update_req;
    reqdata->winsize = dptr[0];
    reqdata->winoffset = get_le16(dptr + 1);
    reqdata->interval = get_le16(dptr + 3);
    reqdata->latency = get_le16(dptr + 5);
    reqdata->timeout = get_le16(dptr + 7);
    reqdata->instant = get_le16(dptr + 9);

    /* XXX: validate them at some point. If they dont check out, we
       return the unknown response */
    rsp_opcode = BLE_ERR_MAX;

    /* If instant is in the past, we have to end the connection */
    conn_events = (reqdata->instant - connsm->event_cntr) & 0xFFFF;
    if (conn_events >= 32767) {
        ble_ll_conn_timeout(connsm, BLE_ERR_INSTANT_PASSED);
    } else {
        connsm->csmflags.cfbit.conn_update_sched = 1;

        /*
         * Errata says that receiving a connection update when the event
         * counter is equal to the instant means wesimply ignore the window
         * offset and window size. Anchor point has already been set based on
         * first packet received in connection event. Given that we increment
         * the event counter BEFORE checking to see if the instant is equal to
         * the event counter what we do here is increment the instant and set
         * the window offset and size to 0.
         */
        if (conn_events == 0) {
            reqdata->winoffset = 0;
            reqdata->winsize = 0;
            reqdata->instant += 1;
        }
    }

    return rsp_opcode;
}

void
ble_ll_ctrl_initiate_dle(struct ble_ll_conn_sm *connsm)
{
    if (!(connsm->conn_features & BLE_LL_FEAT_DATA_LEN_EXT)) {
        return;
    }

    /*
     * Section 4.5.10 Vol 6 PART B. If the max tx/rx time or octets
     * exceeds the minimum, data length procedure needs to occur
     */
    if ((connsm->max_tx_octets <= BLE_LL_CONN_SUPP_BYTES_MIN) &&
        (connsm->max_rx_octets <= BLE_LL_CONN_SUPP_BYTES_MIN) &&
        (connsm->max_tx_time <= BLE_LL_CONN_SUPP_TIME_MIN) &&
        (connsm->max_rx_time <= BLE_LL_CONN_SUPP_TIME_MIN)) {
        return;
    }

    ble_ll_ctrl_proc_start(connsm, BLE_LL_CTRL_PROC_DATA_LEN_UPD);
}

static void
ble_ll_ctrl_update_features(struct ble_ll_conn_sm *connsm, uint8_t *feat)
{
    connsm->conn_features = feat[0];
    memcpy(connsm->remote_features, feat + 1, 7);

    /* If we received peer's features for the 1st time, we should try DLE */
    if (!connsm->csmflags.cfbit.rxd_features) {
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CODED_PHY)
        /*
         * If connection was established on uncoded PHY, by default we use
         * MaxTxTime and MaxRxTime applicable for that PHY since we are not
         * allowed to indicate longer supported time if peer does not support
         * LE Coded PHY. However, once we know that peer does support it we can
         * update those values to ones applicable for coded PHY.
         */
        if (connsm->remote_features[0] & (BLE_LL_FEAT_LE_CODED_PHY >> 8)) {
            if (connsm->host_req_max_tx_time) {
                connsm->max_tx_time = max(connsm->max_tx_time,
                                          connsm->host_req_max_tx_time);
            } else {
                connsm->max_tx_time = g_ble_ll_conn_params.conn_init_max_tx_time_coded;
            }
            connsm->max_rx_time = BLE_LL_CONN_SUPP_TIME_MAX_CODED;
        }
#endif

        connsm->csmflags.cfbit.pending_initiate_dle = 1;
        connsm->csmflags.cfbit.rxd_features = 1;
    }
}

/**
 * Called when we receive a feature request or a slave initiated feature
 * request.
 *
 *
 * @param connsm
 * @param dptr
 * @param rspbuf
 * @param opcode
 * @param new_features
 *
 * @return int
 */
static int
ble_ll_ctrl_rx_feature_req(struct ble_ll_conn_sm *connsm, uint8_t *dptr,
                           uint8_t *rspbuf, uint8_t opcode)
{
    uint8_t rsp_opcode;
    uint64_t our_feat;

    /*
     * Only accept slave feature requests if we are a master and feature
     * requests if we are a slave.
     */
    if (opcode ==  BLE_LL_CTRL_SLAVE_FEATURE_REQ) {
        if (connsm->conn_role != BLE_LL_CONN_ROLE_MASTER) {
            return BLE_LL_CTRL_UNKNOWN_RSP;
        }
    } else {
        /* XXX: not sure this is correct but do it anyway */
        if (connsm->conn_role != BLE_LL_CONN_ROLE_SLAVE) {
            return BLE_LL_CTRL_UNKNOWN_RSP;
        }
    }

    our_feat = ble_ll_read_supp_features();

    rsp_opcode = BLE_LL_CTRL_FEATURE_RSP;

    ble_ll_ctrl_update_features(connsm, dptr);

    /*
     * 1st octet of features should be common features of local and remote
     * controller - we call this 'connection features'
     * remaining octets are features of controller which sends PDU, in this case
     * it's our controller
     *
     * See: Vol 6, Part B, section 2.4.2.10
     */
    connsm->conn_features &= our_feat;

    put_le64(rspbuf + 1, our_feat);
    rspbuf[1] = connsm->conn_features;

    return rsp_opcode;
}

/**
 * Called when we receive a feature response
 *
 * @param connsm
 * @param dptr
 * @param new_features
 *
 */
static void
ble_ll_ctrl_rx_feature_rsp(struct ble_ll_conn_sm *connsm, uint8_t *dptr)
{
    ble_ll_ctrl_update_features(connsm, dptr);

    /* Stop the control procedure */
    if (IS_PENDING_CTRL_PROC(connsm, BLE_LL_CTRL_PROC_FEATURE_XCHG)) {
        ble_ll_ctrl_proc_stop(connsm, BLE_LL_CTRL_PROC_FEATURE_XCHG);
    }

    /* Send event to host if pending features read */
    if (connsm->csmflags.cfbit.pending_hci_rd_features) {
        ble_ll_hci_ev_rd_rem_used_feat(connsm, BLE_ERR_SUCCESS);
        connsm->csmflags.cfbit.pending_hci_rd_features = 0;
    }
}

/**
 *
 *
 * Context: Link Layer task
 *
 * @param connsm
 * @param dptr
 * @param rspbuf
 *
 * @return int
 */
static int
ble_ll_ctrl_rx_conn_param_req(struct ble_ll_conn_sm *connsm, uint8_t *dptr,
                              uint8_t *rspbuf)
{
    uint8_t rsp_opcode;

    /*
     * This is not in the specification per se but it simplifies the
     * implementation. If we get a connection parameter request and we
     * are awaiting a reply from the host, simply ignore the request. This
     * might not be a good idea if the parameters are different, but oh
     * well. This is not expected to happen anyway. A return of BLE_ERR_MAX
     * means that we will simply discard the connection parameter request
     */
    if (connsm->csmflags.cfbit.awaiting_host_reply) {
        return BLE_ERR_MAX;
    }

    /* XXX: remember to deal with this on the master: if the slave has
     * initiated a procedure we may have received its connection parameter
     * update request and have signaled the host with an event. If that
     * is the case, we will need to drop the host command when we get it
       and also clear any applicable states. */

    /* XXX: Read 5.3 again. There are multiple control procedures that might
     * be pending (a connection update) that will cause collisions and the
       behavior below. */
    /*
     * Check for procedure collision (Vol 6 PartB 5.3). If we are a slave
     * and we receive a request we "consider the slave initiated
     * procedure as complete". This means send a connection update complete
     * event (with error).
     *
     * If a master, we send reject with a
     * transaction collision error code.
     */
    if (IS_PENDING_CTRL_PROC(connsm, BLE_LL_CTRL_PROC_CONN_PARAM_REQ)) {
        if (connsm->conn_role == BLE_LL_CONN_ROLE_SLAVE) {
            ble_ll_ctrl_proc_stop(connsm, BLE_LL_CTRL_PROC_CONN_PARAM_REQ);
            ble_ll_hci_ev_conn_update(connsm, BLE_ERR_LMP_COLLISION);
        } else {
            /* The master sends reject ind ext w/error code 0x23 */
            rsp_opcode = BLE_LL_CTRL_REJECT_IND_EXT;
            rspbuf[1] = BLE_LL_CTRL_CONN_PARM_REQ;
            rspbuf[2] = BLE_ERR_LMP_COLLISION;
            return rsp_opcode;
        }
    }

    /*
     * If we are a master and we currently performing a channel map
     * update procedure we need to return an error
     */
    if ((connsm->conn_role == BLE_LL_CONN_ROLE_MASTER) &&
        (connsm->csmflags.cfbit.chanmap_update_scheduled)) {
        rsp_opcode = BLE_LL_CTRL_REJECT_IND_EXT;
        rspbuf[1] = BLE_LL_CTRL_CONN_PARM_REQ;
        rspbuf[2] = BLE_ERR_DIFF_TRANS_COLL;
        return rsp_opcode;
    }

    /* Process the received connection parameter request */
    rsp_opcode = ble_ll_ctrl_conn_param_pdu_proc(connsm, dptr, rspbuf,
                                                 BLE_LL_CTRL_CONN_PARM_REQ);
    return rsp_opcode;
}

static int
ble_ll_ctrl_rx_conn_param_rsp(struct ble_ll_conn_sm *connsm, uint8_t *dptr,
                              uint8_t *rspbuf)
{
    uint8_t rsp_opcode;

    /* A slave should never receive this response */
    if (connsm->conn_role == BLE_LL_CONN_ROLE_SLAVE) {
        return BLE_LL_CTRL_UNKNOWN_RSP;
    }

    /*
     * This case should never happen! It means that the slave initiated a
     * procedure and the master initiated one as well. If we do get in this
     * state just clear the awaiting reply. The slave will hopefully stop its
     * procedure when we reply.
     */
    if (connsm->csmflags.cfbit.awaiting_host_reply) {
        connsm->csmflags.cfbit.awaiting_host_reply = 0;
    }

    /* If we receive a response and no procedure is pending, just leave */
    if (!IS_PENDING_CTRL_PROC(connsm, BLE_LL_CTRL_PROC_CONN_PARAM_REQ)) {
        return BLE_ERR_MAX;
    }

    /* Process the received connection parameter response */
    rsp_opcode = ble_ll_ctrl_conn_param_pdu_proc(connsm, dptr, rspbuf,
                                                 BLE_LL_CTRL_CONN_PARM_RSP);
    return rsp_opcode;
}

/**
 * Called to process the LL control PDU VERSION_IND
 *
 * Context: Link Layer task
 *
 * @param connsm
 * @param dptr
 * @param rspbuf
 *
 * @return int
 */
static int
ble_ll_ctrl_rx_version_ind(struct ble_ll_conn_sm *connsm, uint8_t *dptr,
                           uint8_t *rspbuf)
{
    uint8_t rsp_opcode;

    /* Process the packet */
    connsm->vers_nr = dptr[0];
    connsm->comp_id = get_le16(dptr + 1);
    connsm->sub_vers_nr = get_le16(dptr + 3);
    connsm->csmflags.cfbit.rxd_version_ind = 1;

    rsp_opcode = BLE_ERR_MAX;
    if (!connsm->csmflags.cfbit.version_ind_sent) {
        rsp_opcode = BLE_LL_CTRL_VERSION_IND;
        ble_ll_ctrl_version_ind_make(connsm, rspbuf);
    }

    /* Stop the control procedure */
    if (IS_PENDING_CTRL_PROC(connsm, BLE_LL_CTRL_PROC_VERSION_XCHG)) {
        ble_ll_hci_ev_rd_rem_ver(connsm, BLE_ERR_SUCCESS);
        ble_ll_ctrl_proc_stop(connsm, BLE_LL_CTRL_PROC_VERSION_XCHG);
    }
    return rsp_opcode;
}

/**
 * Called to process a received channel map request control pdu.
 *
 * Context: Link Layer task
 *
 * @param connsm
 * @param dptr
 */
static int
ble_ll_ctrl_rx_chanmap_req(struct ble_ll_conn_sm *connsm, uint8_t *dptr)
{
    uint16_t instant;
    uint16_t conn_events;

    if (connsm->conn_role == BLE_LL_CONN_ROLE_MASTER) {
        return BLE_LL_CTRL_UNKNOWN_RSP;
    }

    /* If instant is in the past, we have to end the connection */
    instant = get_le16(dptr + BLE_LL_CONN_CHMAP_LEN);
    conn_events = (instant - connsm->event_cntr) & 0xFFFF;
    if (conn_events >= 32767) {
        ble_ll_conn_timeout(connsm, BLE_ERR_INSTANT_PASSED);
    } else {
        connsm->chanmap_instant = instant;
        memcpy(connsm->req_chanmap, dptr, BLE_LL_CONN_CHMAP_LEN);
        connsm->csmflags.cfbit.chanmap_update_scheduled = 1;
    }

    return BLE_ERR_MAX;
}

/**
 * Initiate LL control procedure.
 *
 * This function is called to obtain a mbuf to send a LL control PDU. The data
 * channel PDU header is not part of the mbuf data; it is part of the BLE
 * header (which is part of the mbuf).
 *
 * Context: LL task.
 *
 * @param connsm
 * @param ctrl_proc
 */
static struct os_mbuf *
ble_ll_ctrl_proc_init(struct ble_ll_conn_sm *connsm, int ctrl_proc)
{
    uint8_t len;
    uint8_t opcode;
    uint8_t *dptr;
    uint8_t *ctrdata;
    struct os_mbuf *om;

    /* Get an mbuf for the control pdu */
    om = os_msys_get_pkthdr(BLE_LL_CTRL_MAX_PDU_LEN, sizeof(struct ble_mbuf_hdr));

    if (om) {
        /* The control data starts after the opcode (1 byte) */
        dptr = om->om_data;
        ctrdata = dptr + 1;

        switch (ctrl_proc) {
        case BLE_LL_CTRL_PROC_CONN_UPDATE:
            opcode = BLE_LL_CTRL_CONN_UPDATE_IND;
            ble_ll_ctrl_conn_upd_make(connsm, ctrdata, NULL);
            break;
        case BLE_LL_CTRL_PROC_CHAN_MAP_UPD:
            opcode = BLE_LL_CTRL_CHANNEL_MAP_REQ;
            ble_ll_ctrl_chanmap_req_make(connsm, ctrdata);
            break;
        case BLE_LL_CTRL_PROC_FEATURE_XCHG:
            if (connsm->conn_role == BLE_LL_CONN_ROLE_MASTER) {
                opcode = BLE_LL_CTRL_FEATURE_REQ;
            } else {
                opcode = BLE_LL_CTRL_SLAVE_FEATURE_REQ;
            }
            put_le64(ctrdata, ble_ll_read_supp_features());
            break;
        case BLE_LL_CTRL_PROC_VERSION_XCHG:
            opcode = BLE_LL_CTRL_VERSION_IND;
            ble_ll_ctrl_version_ind_make(connsm, ctrdata);
            break;
        case BLE_LL_CTRL_PROC_TERMINATE:
            opcode = BLE_LL_CTRL_TERMINATE_IND;
            ctrdata[0] = connsm->disconnect_reason;
            break;
        case BLE_LL_CTRL_PROC_CONN_PARAM_REQ:
            opcode = BLE_LL_CTRL_CONN_PARM_REQ;
            ble_ll_ctrl_conn_param_pdu_make(connsm, ctrdata, NULL);
            break;
        case BLE_LL_CTRL_PROC_LE_PING:
            opcode = BLE_LL_CTRL_PING_REQ;
            break;
        case BLE_LL_CTRL_PROC_DATA_LEN_UPD:
            opcode = BLE_LL_CTRL_LENGTH_REQ;
            ble_ll_ctrl_datalen_upd_make(connsm, dptr);
            break;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
        /* XXX: deal with already encrypted connection.*/
        case BLE_LL_CTRL_PROC_ENCRYPT:
            /* If we are already encrypted we do pause procedure */
            if (connsm->enc_data.enc_state == CONN_ENC_S_ENCRYPTED) {
                opcode = BLE_LL_CTRL_PAUSE_ENC_REQ;
            } else {
                opcode = BLE_LL_CTRL_ENC_REQ;
                ble_ll_ctrl_enc_req_make(connsm, ctrdata);
            }
            break;
#endif
#if (BLE_LL_BT5_PHY_SUPPORTED == 1)
        case BLE_LL_CTRL_PROC_PHY_UPDATE:
            opcode = BLE_LL_CTRL_PHY_REQ;
            ble_ll_ctrl_phy_req_rsp_make(connsm, ctrdata);
            break;
#endif
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_SCA_UPDATE)
        case BLE_LL_CTRL_PROC_SCA_UPDATE:
            opcode = BLE_LL_CTRL_CLOCK_ACCURACY_REQ;
            ble_ll_ctrl_sca_req_rsp_make(connsm, ctrdata);
            break;
#endif
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_ISO)
        case BLE_LL_CTRL_PROC_CIS_CREATE:
            opcode = BLE_LL_CTRL_CIS_REQ;
            ble_ll_ctrl_cis_create(connsm, ctrdata);
            break;
#endif
        default:
            BLE_LL_ASSERT(0);
            break;
        }

        /* Set llid, length and opcode */
        dptr[0] = opcode;
        len = g_ble_ll_ctrl_pkt_lengths[opcode] + 1;

        /* Add packet to transmit queue of connection */
        ble_ll_conn_enqueue_pkt(connsm, om, BLE_LL_LLID_CTRL, len);
    }

    return om;
}

/**
 * Called to determine if the pdu is a TERMINATE_IND
 *
 * @param hdr
 * @param opcode
 *
 * @return int
 */
int
ble_ll_ctrl_is_terminate_ind(uint8_t hdr, uint8_t opcode)
{
    int rc;

    rc = 0;
    if ((hdr & BLE_LL_DATA_HDR_LLID_MASK) == BLE_LL_LLID_CTRL) {
        if (opcode == BLE_LL_CTRL_TERMINATE_IND) {
            rc = 1;
        }
    }
    return rc;
}

/**
 * Stops the LL control procedure indicated by 'ctrl_proc'.
 *
 * Context: Link Layer task
 *
 * @param connsm
 * @param ctrl_proc
 */
void
ble_ll_ctrl_proc_stop(struct ble_ll_conn_sm *connsm, int ctrl_proc)
{
    if (connsm->cur_ctrl_proc == ctrl_proc) {
        ble_npl_callout_stop(&connsm->ctrl_proc_rsp_timer);
        connsm->cur_ctrl_proc = BLE_LL_CTRL_PROC_IDLE;
    }
    CLR_PENDING_CTRL_PROC(connsm, ctrl_proc);

    /* If there are others, start them */
    ble_ll_ctrl_chk_proc_start(connsm);
}

/**
 * Called to start the terminate procedure.
 *
 * Context: Link Layer task.
 *
 * @param connsm
 */
void
ble_ll_ctrl_terminate_start(struct ble_ll_conn_sm *connsm)
{
    int ctrl_proc;
    uint32_t usecs;
    struct os_mbuf *om;

    BLE_LL_ASSERT(connsm->disconnect_reason != 0);

    ctrl_proc = BLE_LL_CTRL_PROC_TERMINATE;
    om = ble_ll_ctrl_proc_init(connsm, ctrl_proc);
    if (om) {
        CONN_F_TERMINATE_STARTED(connsm) = 1;

        /* Set terminate "timeout" */
        usecs = connsm->supervision_tmo * BLE_HCI_CONN_SPVN_TMO_UNITS * 1000;
        connsm->terminate_timeout = os_cputime_get32() +
            os_cputime_usecs_to_ticks(usecs);
    }
}

/**
 * Called to start a LL control procedure except for the terminate procedure. We
 * always set the control procedure pending bit even if the control procedure
 * has been initiated.
 *
 * Context: Link Layer task.
 *
 * @param connsm Pointer to connection state machine.
 */
void
ble_ll_ctrl_proc_start(struct ble_ll_conn_sm *connsm, int ctrl_proc)
{
    struct os_mbuf *om;

    BLE_LL_ASSERT(ctrl_proc != BLE_LL_CTRL_PROC_TERMINATE);

    om = NULL;
    if (connsm->cur_ctrl_proc == BLE_LL_CTRL_PROC_IDLE) {
        /* Initiate the control procedure. */
        om = ble_ll_ctrl_proc_init(connsm, ctrl_proc);
        if (om) {
            /* Set the current control procedure */
            connsm->cur_ctrl_proc = ctrl_proc;

            /* Initialize the procedure response timeout */
            if (ctrl_proc != BLE_LL_CTRL_PROC_CHAN_MAP_UPD) {
                ble_ll_ctrl_start_rsp_timer(connsm);
            }
        }
    }

    /* Set bitmask denoting control procedure is pending */
    connsm->pending_ctrl_procs |= (1 << ctrl_proc);
}

/**
 * Called to determine if we need to start a LL control procedure for the given
 * connection.
 *
 * Context: Link Layer
 *
 * @param connsm Pointer to connection state machine.
 */
void
ble_ll_ctrl_chk_proc_start(struct ble_ll_conn_sm *connsm)
{
    int i;

    /* XXX: TODO new rules! Cannot start certain control procedures if other
     * ones are peer initiated. We need to wait. Deal with this.
     */

    /*
     * If we are terminating, dont start any new procedures but start
     * terminate if needed
     */
    if (connsm->disconnect_reason) {
        if (!CONN_F_TERMINATE_STARTED(connsm)) {
            /*
             * If the terminate procedure has not started it means we were not
             * able to start it right away (no control pdu was available).
             * Start it now. No need to start any other procedures.
             */
            ble_ll_ctrl_terminate_start(connsm);
        }
        return;
    }

    /* If there is a running procedure or no pending, do nothing */
    if ((connsm->cur_ctrl_proc == BLE_LL_CTRL_PROC_IDLE) &&
        (connsm->pending_ctrl_procs != 0)) {
        /*
         * The specification says there is no priority to control procedures
         * so just start from the first one for now.
         */
        for (i = 0; i < BLE_LL_CTRL_PROC_NUM; ++i) {
            if (IS_PENDING_CTRL_PROC(connsm, i)) {
                /*
                 * The version exchange is a special case. If we have already
                 * received the information dont start it.
                 */
                if ((i == BLE_LL_CTRL_PROC_VERSION_XCHG) &&
                    (connsm->csmflags.cfbit.rxd_version_ind)) {
                    ble_ll_hci_ev_rd_rem_ver(connsm, BLE_ERR_SUCCESS);
                    CLR_PENDING_CTRL_PROC(connsm, i);
                } else {
                    ble_ll_ctrl_proc_start(connsm, i);
                    break;
                }
            }
        }
    }
}

/**
 * Called when the Link Layer receives a LL control PDU.
 *
 * NOTE: this function uses the received PDU for the response in some cases. If
 * the received PDU is not used it needs to be freed here.
 *
 * XXX: may want to check, for both master and slave, whether the control
 * pdu should be received by that role. Might make for less code...
 * Context: Link Layer
 *
 * @param om
 * @param connsm
 */
int
ble_ll_ctrl_rx_pdu(struct ble_ll_conn_sm *connsm, struct os_mbuf *om)
{
    uint32_t features;
    uint32_t feature;
    uint8_t len;
    uint8_t opcode;
    uint8_t rsp_opcode;
    uint8_t *dptr;
    uint8_t *rspbuf;
    uint8_t *rspdata;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
    int restart_encryption;
#endif
    int rc = 0;

    /* XXX: where do we validate length received and packet header length?
     * do this in LL task when received. Someplace!!! What I mean
     * is we should validate the over the air length with the mbuf length.
       Should the PHY do that???? */

    /*
     * dptr points to om_data pointer. The first byte of om_data is the
     * first byte of the Data Channel PDU header. Get length from header and
     * opcode from LL control PDU.
     */
    dptr = om->om_data;
    len = dptr[1];
    opcode = dptr[2];

    /*
     * rspbuf points to first byte of response. The response buffer does not
     * contain the Data Channel PDU. Thus, the first byte of rspbuf is the
     * LL control PDU payload (the opcode of the control PDU). rspdata
     * points to CtrData in the control PDU.
     */
    rspbuf = dptr;
    rspdata = rspbuf + 1;

    /* Move data pointer to start of control data (2 byte PDU hdr + opcode) */
    dptr += (BLE_LL_PDU_HDR_LEN + 1);

    /*
     * Subtract the opcode from the length. Note that if the length was zero,
     * which would be an error, we will fail the check against the length
     * of the control packet.
     */
    --len;

    ble_ll_trace_u32x2(BLE_LL_TRACE_ID_CTRL_RX, opcode, len);

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
    restart_encryption = 0;
#endif

    /* If opcode comes from reserved value or CtrlData fields is invalid
     * we shall respond with LL_UNKNOWN_RSP
     */
    if ((opcode >= BLE_LL_CTRL_OPCODES) ||
        (len != g_ble_ll_ctrl_pkt_lengths[opcode])) {
        rc = -1;
        rsp_opcode = BLE_LL_CTRL_UNKNOWN_RSP;
        goto ll_ctrl_send_rsp;
    }

    /* Check if the feature is supported. */
    switch (opcode) {
    case BLE_LL_CTRL_LENGTH_REQ:
        feature = BLE_LL_FEAT_DATA_LEN_EXT;
        break;
    case BLE_LL_CTRL_SLAVE_FEATURE_REQ:
        feature = BLE_LL_FEAT_SLAVE_INIT;
        break;
    case BLE_LL_CTRL_CONN_PARM_REQ:
    case BLE_LL_CTRL_CONN_PARM_RSP:
        feature = BLE_LL_FEAT_CONN_PARM_REQ;
        break;
    case BLE_LL_CTRL_ENC_REQ:
    case BLE_LL_CTRL_START_ENC_REQ:
    case BLE_LL_CTRL_PAUSE_ENC_REQ:
        feature = BLE_LL_FEAT_LE_ENCRYPTION;
        break;
    case BLE_LL_CTRL_PING_REQ:
        feature = BLE_LL_FEAT_LE_PING;
        break;
    case BLE_LL_CTRL_PHY_REQ:
        feature = BLE_LL_FEAT_LE_2M_PHY | BLE_LL_FEAT_LE_CODED_PHY;
        break;
    case BLE_LL_CTRL_MIN_USED_CHAN_IND:
        feature = BLE_LL_FEAT_MIN_USED_CHAN;
        break;
    case BLE_LL_CTRL_PERIODIC_SYNC_IND:
        feature = BLE_LL_FEAT_SYNC_TRANS_RECV;
        break;
    default:
        feature = 0;
        break;
    }

    if (feature) {
        features = ble_ll_read_supp_features();
        if ((features & feature) == 0) {
            if (opcode == BLE_LL_CTRL_ENC_REQ) {
                if (connsm->conn_features & BLE_LL_FEAT_EXTENDED_REJ) {
                    rsp_opcode = BLE_LL_CTRL_REJECT_IND_EXT;
                    rspbuf[1] = opcode;
                    rspbuf[2] = BLE_ERR_UNSUPP_REM_FEATURE;

                } else {
                    rsp_opcode = BLE_LL_CTRL_REJECT_IND;
                    rspbuf[1] = BLE_ERR_UNSUPP_REM_FEATURE;
                }
            } else {
                /* Construct unknown rsp pdu */
                rsp_opcode = BLE_LL_CTRL_UNKNOWN_RSP;
            }
            goto ll_ctrl_send_rsp;
        }
    }

    /* Process opcode */
    rsp_opcode = BLE_ERR_MAX;
    switch (opcode) {
    case BLE_LL_CTRL_CONN_UPDATE_IND:
        rsp_opcode = ble_ll_ctrl_rx_conn_update(connsm, dptr);
        break;
    case BLE_LL_CTRL_CHANNEL_MAP_REQ:
        rsp_opcode = ble_ll_ctrl_rx_chanmap_req(connsm, dptr);
        break;
    case BLE_LL_CTRL_LENGTH_REQ:
        /* Extract parameters and check if valid */
        if (ble_ll_ctrl_len_proc(connsm, dptr)) {
            rc  = -1;
            rsp_opcode = BLE_LL_CTRL_UNKNOWN_RSP;
            goto ll_ctrl_send_rsp;
        }

        /*
         * If we have not started this procedure ourselves and it is
         * pending, no need to perform it.
         */
        if ((connsm->cur_ctrl_proc != BLE_LL_CTRL_PROC_DATA_LEN_UPD) &&
            IS_PENDING_CTRL_PROC(connsm, BLE_LL_CTRL_PROC_DATA_LEN_UPD)) {
            CLR_PENDING_CTRL_PROC(connsm, BLE_LL_CTRL_PROC_DATA_LEN_UPD);
        }

        /* Send a response */
        rsp_opcode = BLE_LL_CTRL_LENGTH_RSP;
        ble_ll_ctrl_datalen_upd_make(connsm, rspbuf);
        break;
    case BLE_LL_CTRL_LENGTH_RSP:
        /* According to specification, process this only if we asked for it. */
        if (connsm->cur_ctrl_proc == BLE_LL_CTRL_PROC_DATA_LEN_UPD) {
            /*
             * Process the received data. If received data is invalid, we'll
             * reply with LL_UNKNOWN_RSP as per spec, but we still need to stop
             * control procedure to avoid timeout.
             */
            if (ble_ll_ctrl_len_proc(connsm, dptr)) {
                rc = -1;
                rsp_opcode = BLE_LL_CTRL_UNKNOWN_RSP;
            }

            /* Stop the control procedure */
            ble_ll_ctrl_proc_stop(connsm, BLE_LL_CTRL_PROC_DATA_LEN_UPD);
        }
        break;
    case BLE_LL_CTRL_UNKNOWN_RSP:
        rsp_opcode = ble_ll_ctrl_proc_unk_rsp(connsm, dptr, rspdata);
        break;
    case BLE_LL_CTRL_FEATURE_REQ:
        rsp_opcode = ble_ll_ctrl_rx_feature_req(connsm, dptr, rspbuf, opcode);
        break;
    /* XXX: check to see if ctrl procedure was running? Do we care? */
    case BLE_LL_CTRL_FEATURE_RSP:
        ble_ll_ctrl_rx_feature_rsp(connsm, dptr);
        break;
    case BLE_LL_CTRL_VERSION_IND:
        rsp_opcode = ble_ll_ctrl_rx_version_ind(connsm, dptr, rspdata);
        break;
    case BLE_LL_CTRL_SLAVE_FEATURE_REQ:
        rsp_opcode = ble_ll_ctrl_rx_feature_req(connsm, dptr, rspbuf, opcode);
        break;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
    case BLE_LL_CTRL_ENC_REQ:
        rsp_opcode = ble_ll_ctrl_rx_enc_req(connsm, dptr, rspdata);
        break;
    case BLE_LL_CTRL_ENC_RSP:
        ble_ll_ctrl_rx_enc_rsp(connsm, dptr);
        break;
    case BLE_LL_CTRL_START_ENC_REQ:
        rsp_opcode = ble_ll_ctrl_rx_start_enc_req(connsm);
        break;
    case BLE_LL_CTRL_START_ENC_RSP:
        rsp_opcode = ble_ll_ctrl_rx_start_enc_rsp(connsm);
        break;
    case BLE_LL_CTRL_PAUSE_ENC_REQ:
        rsp_opcode = ble_ll_ctrl_rx_pause_enc_req(connsm);
        break;
    case BLE_LL_CTRL_PAUSE_ENC_RSP:
        rsp_opcode = ble_ll_ctrl_rx_pause_enc_rsp(connsm);
        if (rsp_opcode == BLE_LL_CTRL_PAUSE_ENC_RSP) {
            restart_encryption = 1;
        }
        break;
#endif
    case BLE_LL_CTRL_PING_REQ:
        rsp_opcode = BLE_LL_CTRL_PING_RSP;
        break;
    case BLE_LL_CTRL_PING_RSP:
        ble_ll_ctrl_rx_ping_rsp(connsm);
        break;
    case BLE_LL_CTRL_CONN_PARM_REQ:
        rsp_opcode = ble_ll_ctrl_rx_conn_param_req(connsm, dptr, rspbuf);
        break;
    case BLE_LL_CTRL_CONN_PARM_RSP:
        rsp_opcode = ble_ll_ctrl_rx_conn_param_rsp(connsm, dptr, rspbuf);
        break;
    /* Fall-through intentional... */
    case BLE_LL_CTRL_REJECT_IND:
    case BLE_LL_CTRL_REJECT_IND_EXT:
        /* Sometimes reject triggers sending other LL CTRL msg */
        rsp_opcode = ble_ll_ctrl_rx_reject_ind(connsm, dptr, opcode, rspdata);
        break;
#if (BLE_LL_BT5_PHY_SUPPORTED == 1)
    case BLE_LL_CTRL_PHY_REQ:
        rsp_opcode = ble_ll_ctrl_rx_phy_req(connsm, dptr, rspdata);
        break;
    case BLE_LL_CTRL_PHY_RSP:
        rsp_opcode = ble_ll_ctrl_rx_phy_rsp(connsm, dptr, rspdata);
        break;
    case BLE_LL_CTRL_PHY_UPDATE_IND:
        rsp_opcode = ble_ll_ctrl_rx_phy_update_ind(connsm, dptr);
        break;
#endif
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_SCA_UPDATE)
    case BLE_LL_CTRL_CLOCK_ACCURACY_REQ:
        rsp_opcode = ble_ll_ctrl_rx_sca_req(connsm, dptr, rspdata);
        break;
    case BLE_LL_CTRL_CLOCK_ACCURACY_RSP:
        rsp_opcode = ble_ll_ctrl_rx_sca_rsp(connsm, dptr);
        break;
#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_ISO)
    case BLE_LL_CTRL_CIS_REQ:
        rsp_opcode = ble_ll_ctrl_rx_cis_req(connsm, dptr, rspdata);
        break;
    case BLE_LL_CTRL_CIS_RSP:
        rsp_opcode = ble_ll_ctrl_rx_cis_rsp(connsm, dptr, rspdata);
        break;
    case BLE_LL_CTRL_CIS_IND:
        rsp_opcode = ble_ll_ctrl_rx_cis_ind(connsm, dptr);
        break;
#endif
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV_SYNC_TRANSFER)
    case BLE_LL_CTRL_PERIODIC_SYNC_IND:
        rsp_opcode = ble_ll_ctrl_rx_periodic_sync_ind(connsm, dptr);
        break;
#endif
    default:
        /* Nothing to do here */
        break;
    }

    /* Free mbuf or send response */
ll_ctrl_send_rsp:
    if (rsp_opcode == BLE_ERR_MAX) {
        os_mbuf_free_chain(om);
    } else {
        /*
         * Write the response opcode into the buffer. If this is an unknown
         * response, put opcode of unknown pdu into buffer.
         */
        rspbuf[0] = rsp_opcode;
        if (rsp_opcode == BLE_LL_CTRL_UNKNOWN_RSP) {
            rspbuf[1] = opcode;
        }
        len = g_ble_ll_ctrl_pkt_lengths[rsp_opcode] + 1;
        ble_ll_conn_enqueue_pkt(connsm, om, BLE_LL_LLID_CTRL, len);
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
        if (restart_encryption) {
            /* XXX: what happens if this fails? Meaning we cant allocate
               mbuf? */
            ble_ll_ctrl_proc_init(connsm, BLE_LL_CTRL_PROC_ENCRYPT);
        }
#endif
    }

    if (connsm->csmflags.cfbit.pending_initiate_dle) {
        connsm->csmflags.cfbit.pending_initiate_dle = 0;
        ble_ll_ctrl_initiate_dle(connsm);
    }

    return rc;
}

/**
 * Called to create and send a REJECT_IND_EXT control PDU or a REJECT_IND
 *
 * @param connsm
 * @param rej_opcode
 * @param err
 *
 * @return int
 */
int
ble_ll_ctrl_reject_ind_send(struct ble_ll_conn_sm *connsm, uint8_t rej_opcode,
                            uint8_t err)
{
    int rc;
    uint8_t len;
    uint8_t opcode;
    uint8_t *rspbuf;
    struct os_mbuf *om;

    om = os_msys_get_pkthdr(BLE_LL_CTRL_MAX_PDU_LEN,
                            sizeof(struct ble_mbuf_hdr));
    if (om) {
        rspbuf = om->om_data;
        opcode = BLE_LL_CTRL_REJECT_IND_EXT;
        if (rej_opcode == BLE_LL_CTRL_ENC_REQ) {
            if ((connsm->conn_features & BLE_LL_FEAT_EXTENDED_REJ) == 0) {
                opcode = BLE_LL_CTRL_REJECT_IND;
            }
        }
        rspbuf[0] = opcode;
        if (opcode == BLE_LL_CTRL_REJECT_IND) {
            rspbuf[1] = err;
            len = BLE_LL_CTRL_REJ_IND_LEN + 1;
        } else {
            rspbuf[1] = rej_opcode;
            rspbuf[2] = err;
            len = BLE_LL_CTRL_REJECT_IND_EXT_LEN + 1;
        }
        ble_ll_conn_enqueue_pkt(connsm, om, BLE_LL_LLID_CTRL, len);
        rc = 0;
    } else {
        rc = 1;
    }
    return rc;
}

/**
 * Called when a Link Layer Control pdu has been transmitted successfully.
 * This is called when we have a received a PDU during the ISR.
 *
 * Context: ISR
 *
 * @param txpdu
 *
 * @return int
 */
int
ble_ll_ctrl_tx_done(struct os_mbuf *txpdu, struct ble_ll_conn_sm *connsm)
{
    int rc;
    uint8_t opcode;

    rc = 0;
    opcode = txpdu->om_data[0];
    switch (opcode) {
    case BLE_LL_CTRL_TERMINATE_IND:
        connsm->csmflags.cfbit.terminate_ind_txd = 1;
        rc = -1;
        break;
    case BLE_LL_CTRL_REJECT_IND_EXT:
        if (connsm->cur_ctrl_proc == BLE_LL_CTRL_PROC_CONN_PARAM_REQ) {
            /* If rejecting opcode is BLE_LL_CTRL_PROC_CONN_PARAM_REQ and
             * reason is LMP collision that means we are master on the link and
             * peer wanted to start procedure which we already started.
             * Let's wait for response and do not close procedure. */
            if (txpdu->om_data[1] == BLE_LL_CTRL_CONN_PARM_REQ &&
                            txpdu->om_data[2] != BLE_ERR_LMP_COLLISION) {
                connsm->reject_reason = txpdu->om_data[2];
                connsm->csmflags.cfbit.host_expects_upd_event = 1;
            }
        }
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
        if (connsm->enc_data.enc_state > CONN_ENC_S_ENCRYPTED) {
            connsm->enc_data.enc_state = CONN_ENC_S_UNENCRYPTED;
        }
#endif
        break;
    case BLE_LL_CTRL_REJECT_IND:
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
        connsm->enc_data.enc_state = CONN_ENC_S_UNENCRYPTED;
#endif
        break;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
    case BLE_LL_CTRL_PAUSE_ENC_REQ:
        /* note: fall-through intentional */
    case BLE_LL_CTRL_ENC_REQ:
        connsm->enc_data.enc_state = CONN_ENC_S_ENC_RSP_WAIT;
        break;
    case BLE_LL_CTRL_ENC_RSP:
        connsm->enc_data.enc_state = CONN_ENC_S_LTK_REQ_WAIT;
        connsm->csmflags.cfbit.send_ltk_req = 1;
        break;
    case BLE_LL_CTRL_START_ENC_RSP:
        if (connsm->conn_role == BLE_LL_CONN_ROLE_SLAVE) {
            connsm->enc_data.enc_state = CONN_ENC_S_ENCRYPTED;
            if (CONN_F_LE_PING_SUPP(connsm)) {
                ble_ll_conn_auth_pyld_timer_start(connsm);
            }
        }
        break;
    case BLE_LL_CTRL_PAUSE_ENC_RSP:
        if (connsm->conn_role == BLE_LL_CONN_ROLE_SLAVE) {
            connsm->enc_data.enc_state = CONN_ENC_S_PAUSE_ENC_RSP_WAIT;
        }
        break;
#endif
#if (BLE_LL_BT5_PHY_SUPPORTED == 1)
    case BLE_LL_CTRL_PHY_REQ:
        connsm->phy_tx_transition =
                    ble_ll_ctrl_phy_tx_transition_get(connsm->phy_data.req_pref_tx_phys_mask);
        break;
    case BLE_LL_CTRL_PHY_UPDATE_IND:
        connsm->phy_tx_transition =
                    ble_ll_ctrl_phy_tx_transition_get(txpdu->om_data[2]);
        break;
#endif
    default:
        break;
    }

    os_mbuf_free_chain(txpdu);
    return rc;
}
#endif
