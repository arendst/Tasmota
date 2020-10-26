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
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "os/os.h"
#include "mem/mem.h"
#include "nimble/ble_hci_trans.h"
#include "host/ble_monitor.h"
#include "ble_hs_priv.h"
#include "ble_hs_dbg_priv.h"
#include "ble_monitor_priv.h"

#define BLE_HCI_CMD_TIMEOUT_MS  2000

static struct ble_npl_mutex ble_hs_hci_mutex;
static struct ble_npl_sem ble_hs_hci_sem;

static uint8_t *ble_hs_hci_ack;
static uint16_t ble_hs_hci_buf_sz;
static uint8_t ble_hs_hci_max_pkts;
static uint32_t ble_hs_hci_sup_feat;
static uint8_t ble_hs_hci_version;

#define BLE_HS_HCI_FRAG_DATABUF_SIZE    \
    (BLE_ACL_MAX_PKT_SIZE +             \
     BLE_HCI_DATA_HDR_SZ +              \
     sizeof (struct os_mbuf_pkthdr) +   \
     sizeof (struct os_mbuf))

#define BLE_HS_HCI_FRAG_MEMBLOCK_SIZE   \
    (OS_ALIGN(BLE_HS_HCI_FRAG_DATABUF_SIZE, 4))

#define BLE_HS_HCI_FRAG_MEMPOOL_SIZE    \
    OS_MEMPOOL_SIZE(1, BLE_HS_HCI_FRAG_MEMBLOCK_SIZE)

/**
 *  A one-element mbuf pool dedicated to holding outgoing ACL data packets.
 *  This dedicated pool prevents a deadlock caused by mbuf exhaustion.  Without
 *  this pool, all msys mbufs could be permanently allocated, preventing us
 *  from fragmenting outgoing packets and sending them (and ultimately freeing
 *  them).
 */
static os_membuf_t ble_hs_hci_frag_data[BLE_HS_HCI_FRAG_MEMPOOL_SIZE];
static struct os_mbuf_pool ble_hs_hci_frag_mbuf_pool;
static struct os_mempool ble_hs_hci_frag_mempool;

/**
 * The number of available ACL transmit buffers on the controller.  This
 * variable must only be accessed while the host mutex is locked.
 */
uint16_t ble_hs_hci_avail_pkts;

#if MYNEWT_VAL(BLE_HS_PHONY_HCI_ACKS)
static ble_hs_hci_phony_ack_fn *ble_hs_hci_phony_ack_cb;
#endif

#if MYNEWT_VAL(BLE_HS_PHONY_HCI_ACKS)
void
ble_hs_hci_set_phony_ack_cb(ble_hs_hci_phony_ack_fn *cb)
{
    ble_hs_hci_phony_ack_cb = cb;
}
#endif

static void
ble_hs_hci_lock(void)
{
    int rc;

    rc = ble_npl_mutex_pend(&ble_hs_hci_mutex, BLE_NPL_TIME_FOREVER);
    BLE_HS_DBG_ASSERT_EVAL(rc == 0 || rc == OS_NOT_STARTED);
}

static void
ble_hs_hci_unlock(void)
{
    int rc;

    rc = ble_npl_mutex_release(&ble_hs_hci_mutex);
    BLE_HS_DBG_ASSERT_EVAL(rc == 0 || rc == OS_NOT_STARTED);
}

int
ble_hs_hci_set_buf_sz(uint16_t pktlen, uint16_t max_pkts)
{
    if (pktlen == 0 || max_pkts == 0) {
        return BLE_HS_EINVAL;
    }

    ble_hs_hci_buf_sz = pktlen;
    ble_hs_hci_max_pkts = max_pkts;
    ble_hs_hci_avail_pkts = max_pkts;

    return 0;
}

/**
 * Increases the count of available controller ACL buffers.
 */
void
ble_hs_hci_add_avail_pkts(uint16_t delta)
{
    BLE_HS_DBG_ASSERT(ble_hs_locked_by_cur_task());

    if (ble_hs_hci_avail_pkts + delta > UINT16_MAX) {
        ble_hs_sched_reset(BLE_HS_ECONTROLLER);
    } else {
        ble_hs_hci_avail_pkts += delta;
    }
}

static int
ble_hs_hci_rx_cmd_complete(uint8_t event_code, uint8_t *data, int len,
                           struct ble_hs_hci_ack *out_ack)
{
    uint16_t opcode;
    uint8_t *params;
    uint8_t params_len;
    uint8_t num_pkts;

    if (len < BLE_HCI_EVENT_CMD_COMPLETE_HDR_LEN) {
        return BLE_HS_ECONTROLLER;
    }

    num_pkts = data[2];
    opcode = get_le16(data + 3);
    params = data + 5;

    /* XXX: Process num_pkts field. */
    (void)num_pkts;

    out_ack->bha_opcode = opcode;

    params_len = len - BLE_HCI_EVENT_CMD_COMPLETE_HDR_LEN;
    if (params_len > 0) {
        out_ack->bha_status = BLE_HS_HCI_ERR(params[0]);
    } else if (opcode == BLE_HCI_OPCODE_NOP) {
        out_ack->bha_status = 0;
    } else {
        out_ack->bha_status = BLE_HS_ECONTROLLER;
    }

    /* Don't include the status byte in the parameters blob. */
    if (params_len > 1) {
        out_ack->bha_params = params + 1;
        out_ack->bha_params_len = params_len - 1;
    } else {
        out_ack->bha_params = NULL;
        out_ack->bha_params_len = 0;
    }

    return 0;
}

static int
ble_hs_hci_rx_cmd_status(uint8_t event_code, uint8_t *data, int len,
                         struct ble_hs_hci_ack *out_ack)
{
    uint16_t opcode;
    uint8_t num_pkts;
    uint8_t status;

    if (len < BLE_HCI_EVENT_CMD_STATUS_LEN) {
        return BLE_HS_ECONTROLLER;
    }

    status = data[2];
    num_pkts = data[3];
    opcode = get_le16(data + 4);

    /* XXX: Process num_pkts field. */
    (void)num_pkts;

    out_ack->bha_opcode = opcode;
    out_ack->bha_params = NULL;
    out_ack->bha_params_len = 0;
    out_ack->bha_status = BLE_HS_HCI_ERR(status);

    return 0;
}

static int
ble_hs_hci_process_ack(uint16_t expected_opcode,
                       uint8_t *params_buf, uint8_t params_buf_len,
                       struct ble_hs_hci_ack *out_ack)
{
    uint8_t event_code;
    uint8_t param_len;
    uint8_t event_len;
    int rc;

    BLE_HS_DBG_ASSERT(ble_hs_hci_ack != NULL);

    /* Count events received */
    STATS_INC(ble_hs_stats, hci_event);

    /* Display to console */
    ble_hs_dbg_event_disp(ble_hs_hci_ack);

    event_code = ble_hs_hci_ack[0];
    param_len = ble_hs_hci_ack[1];
    event_len = param_len + 2;

    /* Clear ack fields up front to silence spurious gcc warnings. */
    memset(out_ack, 0, sizeof *out_ack);

    switch (event_code) {
    case BLE_HCI_EVCODE_COMMAND_COMPLETE:
        rc = ble_hs_hci_rx_cmd_complete(event_code, ble_hs_hci_ack,
                                         event_len, out_ack);
        break;

    case BLE_HCI_EVCODE_COMMAND_STATUS:
        rc = ble_hs_hci_rx_cmd_status(event_code, ble_hs_hci_ack,
                                       event_len, out_ack);
        break;

    default:
        BLE_HS_DBG_ASSERT(0);
        rc = BLE_HS_EUNKNOWN;
        break;
    }

    if (rc == 0) {
        if (params_buf == NULL || out_ack->bha_params == NULL) {
            out_ack->bha_params_len = 0;
        } else {
            if (out_ack->bha_params_len > params_buf_len) {
                out_ack->bha_params_len = params_buf_len;
                rc = BLE_HS_ECONTROLLER;
            }
            memcpy(params_buf, out_ack->bha_params, out_ack->bha_params_len);
        }
        out_ack->bha_params = params_buf;

        if (out_ack->bha_opcode != expected_opcode) {
            rc = BLE_HS_ECONTROLLER;
        }
    }

    if (rc != 0) {
        STATS_INC(ble_hs_stats, hci_invalid_ack);
    }

    return rc;
}

static int
ble_hs_hci_wait_for_ack(void)
{
    int rc;

#if MYNEWT_VAL(BLE_HS_PHONY_HCI_ACKS)
    if (ble_hs_hci_phony_ack_cb == NULL) {
        rc = BLE_HS_ETIMEOUT_HCI;
    } else {
        ble_hs_hci_ack =
            ble_hci_trans_buf_alloc(BLE_HCI_TRANS_BUF_CMD);
        BLE_HS_DBG_ASSERT(ble_hs_hci_ack != NULL);
        rc = ble_hs_hci_phony_ack_cb(ble_hs_hci_ack, 260);
    }
#else
    rc = ble_npl_sem_pend(&ble_hs_hci_sem,
                     ble_npl_time_ms_to_ticks32(BLE_HCI_CMD_TIMEOUT_MS));
    switch (rc) {
    case 0:
        BLE_HS_DBG_ASSERT(ble_hs_hci_ack != NULL);

#if BLE_MONITOR
        ble_monitor_send(BLE_MONITOR_OPCODE_EVENT_PKT, ble_hs_hci_ack,
                         ble_hs_hci_ack[1] + BLE_HCI_EVENT_HDR_LEN);
#endif

        break;
    case OS_TIMEOUT:
        rc = BLE_HS_ETIMEOUT_HCI;
        STATS_INC(ble_hs_stats, hci_timeout);
        break;
    default:
        rc = BLE_HS_EOS;
        break;
    }
#endif

    return rc;
}

int
ble_hs_hci_cmd_tx(uint16_t opcode, void *cmd, uint8_t cmd_len,
                  void *evt_buf, uint8_t evt_buf_len,
                  uint8_t *out_evt_buf_len)
{
    struct ble_hs_hci_ack ack;
    int rc;

    BLE_HS_DBG_ASSERT(ble_hs_hci_ack == NULL);
    ble_hs_hci_lock();

    rc = ble_hs_hci_cmd_send_buf(opcode, cmd, cmd_len);
    if (rc != 0) {
        goto done;
    }

    rc = ble_hs_hci_wait_for_ack();
    if (rc != 0) {
        ble_hs_sched_reset(rc);
        goto done;
    }

    rc = ble_hs_hci_process_ack(opcode, evt_buf, evt_buf_len, &ack);
    if (rc != 0) {
        ble_hs_sched_reset(rc);
        goto done;
    }

    if (out_evt_buf_len != NULL) {
        *out_evt_buf_len = ack.bha_params_len;
    }

    rc = ack.bha_status;

done:
    if (ble_hs_hci_ack != NULL) {
        ble_hci_trans_buf_free(ble_hs_hci_ack);
        ble_hs_hci_ack = NULL;
    }

    ble_hs_hci_unlock();
    return rc;
}

int
ble_hs_hci_cmd_tx_empty_ack(uint16_t opcode, void *cmd, uint8_t cmd_len)
{
    int rc;

    rc = ble_hs_hci_cmd_tx(opcode, cmd, cmd_len, NULL, 0, NULL);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

void
ble_hs_hci_rx_ack(uint8_t *ack_ev)
{
    if (ble_npl_sem_get_count(&ble_hs_hci_sem) > 0) {
        /* This ack is unexpected; ignore it. */
        ble_hci_trans_buf_free(ack_ev);
        return;
    }
    BLE_HS_DBG_ASSERT(ble_hs_hci_ack == NULL);

    /* Unblock the application now that the HCI command buffer is populated
     * with the acknowledgement.
     */
    ble_hs_hci_ack = ack_ev;
    ble_npl_sem_release(&ble_hs_hci_sem);
}

int
ble_hs_hci_rx_evt(uint8_t *hci_ev, void *arg)
{
    int enqueue = 0;

    BLE_HS_DBG_ASSERT(hci_ev != NULL);

    switch (hci_ev[0]) {
    case BLE_HCI_EVCODE_COMMAND_COMPLETE:
        if (hci_ev[3] == 0 && hci_ev[4] == 0) {
            enqueue = 1;
        }
        break;

    case BLE_HCI_EVCODE_COMMAND_STATUS:
        if (hci_ev[4] == 0 && hci_ev[5] == 0) {
            enqueue = 1;
        }
        break;

    default:
        enqueue = 1;
        break;
    }

    if (enqueue) {
        ble_hs_enqueue_hci_event(hci_ev);
    } else {
        ble_hs_hci_rx_ack(hci_ev);
    }

    return 0;
}

/**
 * Calculates the largest ACL payload that the controller can accept.
 */
static uint16_t
ble_hs_hci_max_acl_payload_sz(void)
{
    /* As per BLE 5.1 Standard, Vol. 2, Part E, section 7.8.2:
     * The LE_Read_Buffer_Size command is used to read the maximum size of the
     * data portion of HCI LE ACL Data Packets sent from the Host to the
     * Controller.
     */
    return ble_hs_hci_buf_sz;
}

/**
 * Allocates an mbuf to contain an outgoing ACL data fragment.
 */
static struct os_mbuf *
ble_hs_hci_frag_alloc(uint16_t frag_size, void *arg)
{
    struct os_mbuf *om;

    /* Prefer the dedicated one-element fragment pool. */
    om = os_mbuf_get_pkthdr(&ble_hs_hci_frag_mbuf_pool, 0);
    if (om != NULL) {
        om->om_data += BLE_HCI_DATA_HDR_SZ;
        return om;
    }

    /* Otherwise, fall back to msys. */
    om = ble_hs_mbuf_acl_pkt();
    if (om != NULL) {
        return om;
    }

    return NULL;
}

/**
 * Retrieves the total capacity of the ACL fragment pool (always 1).
 */
int
ble_hs_hci_frag_num_mbufs(void)
{
    return ble_hs_hci_frag_mempool.mp_num_blocks;
}

/**
 * Retrieves the the count of free buffers in the ACL fragment pool.
 */
int
ble_hs_hci_frag_num_mbufs_free(void)
{
    return ble_hs_hci_frag_mempool.mp_num_free;
}

static struct os_mbuf *
ble_hs_hci_acl_hdr_prepend(struct os_mbuf *om, uint16_t handle,
                           uint8_t pb_flag)
{
    struct hci_data_hdr hci_hdr;
    struct os_mbuf *om2;

    hci_hdr.hdh_handle_pb_bc =
        ble_hs_hci_util_handle_pb_bc_join(handle, pb_flag, 0);
    put_le16(&hci_hdr.hdh_len, OS_MBUF_PKTHDR(om)->omp_len);

    om2 = os_mbuf_prepend(om, sizeof hci_hdr);
    if (om2 == NULL) {
        return NULL;
    }

    om = om2;
    om = os_mbuf_pullup(om, sizeof hci_hdr);
    if (om == NULL) {
        return NULL;
    }

    memcpy(om->om_data, &hci_hdr, sizeof hci_hdr);

#if !BLE_MONITOR
    BLE_HS_LOG(DEBUG, "host tx hci data; handle=%d length=%d\n", handle,
               get_le16(&hci_hdr.hdh_len));
#endif

    return om;
}

int
ble_hs_hci_acl_tx_now(struct ble_hs_conn *conn, struct os_mbuf **om)
{
    struct os_mbuf *txom;
    struct os_mbuf *frag;
    uint8_t pb;
    int rc;

    BLE_HS_DBG_ASSERT(ble_hs_locked_by_cur_task());

    txom = *om;
    *om = NULL;

    if (!(conn->bhc_flags & BLE_HS_CONN_F_TX_FRAG)) {
        /* The first fragment uses the first-non-flush packet boundary value.
         * After sending the first fragment, pb gets set appropriately for all
         * subsequent fragments in this packet.
         */
        pb = BLE_HCI_PB_FIRST_NON_FLUSH;
    } else {
        pb = BLE_HCI_PB_MIDDLE;
    }

    /* Send fragments until the entire packet has been sent. */
    while (txom != NULL && ble_hs_hci_avail_pkts > 0) {
        frag = mem_split_frag(&txom, ble_hs_hci_max_acl_payload_sz(),
                              ble_hs_hci_frag_alloc, NULL);
        if (frag == NULL) {
            *om = txom;
            return BLE_HS_EAGAIN;
        }

        frag = ble_hs_hci_acl_hdr_prepend(frag, conn->bhc_handle, pb);
        if (frag == NULL) {
            rc = BLE_HS_ENOMEM;
            goto err;
        }

#if !BLE_MONITOR
        BLE_HS_LOG(DEBUG, "ble_hs_hci_acl_tx(): ");
        ble_hs_log_mbuf(frag);
        BLE_HS_LOG(DEBUG, "\n");
#endif

        rc = ble_hs_tx_data(frag);
        if (rc != 0) {
            goto err;
        }

        /* If any fragments remain, they should be marked as 'middle'
         * fragments.
         */
        conn->bhc_flags |= BLE_HS_CONN_F_TX_FRAG;
        pb = BLE_HCI_PB_MIDDLE;

        /* Account for the controller buf that will hold the txed fragment. */
        conn->bhc_outstanding_pkts++;
        ble_hs_hci_avail_pkts--;
    }

    if (txom != NULL) {
        /* The controller couldn't accommodate some or all of the packet. */
        *om = txom;
        return BLE_HS_EAGAIN;
    }

    /* The entire packet was transmitted. */
    conn->bhc_flags &= ~BLE_HS_CONN_F_TX_FRAG;

    return 0;

err:
    BLE_HS_DBG_ASSERT(rc != 0);

    conn->bhc_flags &= ~BLE_HS_CONN_F_TX_FRAG;
    os_mbuf_free_chain(txom);
    return rc;
}

/**
 * Transmits an HCI ACL data packet.  This function consumes the supplied mbuf,
 * regardless of the outcome.
 *
 * @return                      0 on success;
 *                              BLE_HS_EAGAIN if the packet could not be sent
 *                                  in its entirety due to controller buffer
 *                                  exhaustion.  The unsent data is pointed to
 *                                  by the `om` parameter.
 *                              A BLE host core return code on unexpected
 *                                  error.
 *
 */
int
ble_hs_hci_acl_tx(struct ble_hs_conn *conn, struct os_mbuf **om)
{
    BLE_HS_DBG_ASSERT(ble_hs_locked_by_cur_task());

    /* If this conn is already backed up, don't even try to send. */
    if (STAILQ_FIRST(&conn->bhc_tx_q) != NULL) {
        return BLE_HS_EAGAIN;
    }

    return ble_hs_hci_acl_tx_now(conn, om);
}

void
ble_hs_hci_set_le_supported_feat(uint32_t feat)
{
    ble_hs_hci_sup_feat = feat;
}

uint32_t
ble_hs_hci_get_le_supported_feat(void)
{
    return ble_hs_hci_sup_feat;
}

void
ble_hs_hci_set_hci_version(uint8_t hci_version)
{
    ble_hs_hci_version = hci_version;
}

uint8_t
ble_hs_hci_get_hci_version(void)
{
    return ble_hs_hci_version;
}

void
ble_hs_hci_init(void)
{
    int rc;

    rc = ble_npl_sem_init(&ble_hs_hci_sem, 0);
    BLE_HS_DBG_ASSERT_EVAL(rc == 0);

    rc = ble_npl_mutex_init(&ble_hs_hci_mutex);
    BLE_HS_DBG_ASSERT_EVAL(rc == 0);

    rc = mem_init_mbuf_pool(ble_hs_hci_frag_data,
                            &ble_hs_hci_frag_mempool,
                            &ble_hs_hci_frag_mbuf_pool,
                            1,
                            BLE_HS_HCI_FRAG_MEMBLOCK_SIZE,
                            "ble_hs_hci_frag");
    BLE_HS_DBG_ASSERT_EVAL(rc == 0);
}

void
ble_hs_hci_deinit(void)
{
    ble_npl_mutex_deinit(&ble_hs_hci_mutex);

    ble_npl_sem_deinit(&ble_hs_hci_sem);
}
