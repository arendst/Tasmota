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

#include <string.h>
#include <errno.h>
#include "syscfg/syscfg.h"
#include "os/os.h"
#include "nimble/ble.h"
#include "nimble/hci_common.h"
#include "ble_hs_priv.h"
#include "ble_l2cap_coc_priv.h"

_Static_assert(sizeof (struct ble_l2cap_hdr) == BLE_L2CAP_HDR_SZ,
               "struct ble_l2cap_hdr must be 4 bytes");

struct os_mempool ble_l2cap_chan_pool;

static os_membuf_t ble_l2cap_chan_mem[
    OS_MEMPOOL_SIZE(MYNEWT_VAL(BLE_L2CAP_MAX_CHANS) +
                    MYNEWT_VAL(BLE_L2CAP_COC_MAX_NUM),
                    sizeof (struct ble_l2cap_chan))
];

STATS_SECT_DECL(ble_l2cap_stats) ble_l2cap_stats;
STATS_NAME_START(ble_l2cap_stats)
    STATS_NAME(ble_l2cap_stats, chan_create)
    STATS_NAME(ble_l2cap_stats, chan_delete)
    STATS_NAME(ble_l2cap_stats, update_init)
    STATS_NAME(ble_l2cap_stats, update_rx)
    STATS_NAME(ble_l2cap_stats, update_fail)
    STATS_NAME(ble_l2cap_stats, proc_timeout)
    STATS_NAME(ble_l2cap_stats, sig_tx)
    STATS_NAME(ble_l2cap_stats, sig_rx)
    STATS_NAME(ble_l2cap_stats, sm_tx)
    STATS_NAME(ble_l2cap_stats, sm_rx)
STATS_NAME_END(ble_l2cap_stats)

struct ble_l2cap_chan *
ble_l2cap_chan_alloc(uint16_t conn_handle)
{
    struct ble_l2cap_chan *chan;

    chan = os_memblock_get(&ble_l2cap_chan_pool);
    if (chan == NULL) {
        return NULL;
    }

    memset(chan, 0, sizeof *chan);
    chan->conn_handle = conn_handle;

    STATS_INC(ble_l2cap_stats, chan_create);

    return chan;
}

void
ble_l2cap_chan_free(struct ble_l2cap_chan *chan)
{
    int rc;

    if (chan == NULL) {
        return;
    }

    os_mbuf_free_chain(chan->rx_buf);
    ble_l2cap_coc_cleanup_chan(chan);

#if MYNEWT_VAL(BLE_HS_DEBUG)
    memset(chan, 0xff, sizeof *chan);
#endif
    rc = os_memblock_put(&ble_l2cap_chan_pool, chan);
    BLE_HS_DBG_ASSERT_EVAL(rc == 0);

    STATS_INC(ble_l2cap_stats, chan_delete);
}

bool
ble_l2cap_is_mtu_req_sent(const struct ble_l2cap_chan *chan)
{
    return (chan->flags & BLE_L2CAP_CHAN_F_TXED_MTU);
}

int
ble_l2cap_parse_hdr(struct os_mbuf *om, int off,
                    struct ble_l2cap_hdr *l2cap_hdr)
{
    int rc;

    rc = os_mbuf_copydata(om, off, sizeof *l2cap_hdr, l2cap_hdr);
    if (rc != 0) {
        return BLE_HS_EMSGSIZE;
    }

    l2cap_hdr->len = get_le16(&l2cap_hdr->len);
    l2cap_hdr->cid = get_le16(&l2cap_hdr->cid);

    return 0;
}

struct os_mbuf *
ble_l2cap_prepend_hdr(struct os_mbuf *om, uint16_t cid, uint16_t len)
{
    struct ble_l2cap_hdr hdr;

    put_le16(&hdr.len, len);
    put_le16(&hdr.cid, cid);

    om = os_mbuf_prepend_pullup(om, sizeof hdr);
    if (om == NULL) {
        return NULL;
    }

    memcpy(om->om_data, &hdr, sizeof hdr);

    return om;
}

uint16_t
ble_l2cap_get_conn_handle(struct ble_l2cap_chan *chan)
{
    if (!chan) {
        return BLE_HS_CONN_HANDLE_NONE;
    }

    return chan->conn_handle;
}

int
ble_l2cap_create_server(uint16_t psm, uint16_t mtu,
                        ble_l2cap_event_fn *cb, void *cb_arg)
{
    return ble_l2cap_coc_create_server(psm, mtu, cb, cb_arg);
}

int
ble_l2cap_connect(uint16_t conn_handle, uint16_t psm, uint16_t mtu,
                  struct os_mbuf *sdu_rx, ble_l2cap_event_fn *cb, void *cb_arg)
{
    return ble_l2cap_sig_coc_connect(conn_handle, psm, mtu, sdu_rx, cb, cb_arg);
}

int ble_l2cap_disconnect(struct ble_l2cap_chan *chan)
{
    return ble_l2cap_sig_disconnect(chan);
}

/**
 * Transmits a packet over an L2CAP channel.  This function only consumes the
 * supplied mbuf on success.
 */
int
ble_l2cap_send(struct ble_l2cap_chan *chan, struct os_mbuf *sdu)
{
    return ble_l2cap_coc_send(chan, sdu);
}

int
ble_l2cap_recv_ready(struct ble_l2cap_chan *chan, struct os_mbuf *sdu_rx)
{
    return ble_l2cap_coc_recv_ready(chan, sdu_rx);
}

void
ble_l2cap_remove_rx(struct ble_hs_conn *conn, struct ble_l2cap_chan *chan)
{
    conn->bhc_rx_chan = NULL;
    os_mbuf_free_chain(chan->rx_buf);
    chan->rx_buf = NULL;
    chan->rx_len = 0;
}

static void
ble_l2cap_append_rx(struct ble_l2cap_chan *chan, struct os_mbuf *frag)
{
    int rc;

    (void)rc;

#if MYNEWT_VAL(BLE_L2CAP_JOIN_RX_FRAGS)
    /* Copy the data from the incoming fragment into the packet in progress. */
    rc = os_mbuf_appendfrom(chan->rx_buf, frag, 0, OS_MBUF_PKTLEN(frag));
    if (rc == 0) {
        os_mbuf_free_chain(frag);
        return;
    }
#endif

    /* Join disabled or append failed due to mbuf shortage.  Just attach the
     * mbuf to the end of the packet.
     */
    os_mbuf_concat(chan->rx_buf, frag);
}

static int
ble_l2cap_rx_payload(struct ble_hs_conn *conn, struct ble_l2cap_chan *chan,
                     struct os_mbuf *om,
                     ble_l2cap_rx_fn **out_rx_cb)
{
    int len_diff;
    int rc;

    if (chan->rx_buf == NULL) {
        /* First fragment in packet. */
        chan->rx_buf = om;
    } else {
        /* Continuation of packet in progress. */
        ble_l2cap_append_rx(chan, om);
    }

    /* Determine if packet is fully reassembled. */
    len_diff = OS_MBUF_PKTLEN(chan->rx_buf) - chan->rx_len;
    if (len_diff > 0) {
        /* More data than expected; data corruption. */
        ble_l2cap_remove_rx(conn, chan);
        rc = BLE_HS_EBADDATA;
    } else if (len_diff == 0) {
        /* All fragments received. */
        *out_rx_cb = chan->rx_fn;
        rc = 0;
    } else {
        /* More fragments remain. */
#if MYNEWT_VAL(BLE_L2CAP_RX_FRAG_TIMEOUT) != 0
        conn->bhc_rx_timeout =
            ble_npl_time_get() + MYNEWT_VAL(BLE_L2CAP_RX_FRAG_TIMEOUT);

        ble_hs_timer_resched();
#endif
        rc = BLE_HS_EAGAIN;
    }

    return rc;
}

static uint16_t
ble_l2cap_get_mtu(struct ble_l2cap_chan *chan)
{
    if (chan->scid == BLE_L2CAP_CID_ATT) {
        /* In case of ATT chan->my_mtu keeps preferred MTU which is later
         * used during exchange MTU procedure. Helper below will gives us actual
         * MTU on the channel, which is 23 or higher if exchange MTU has been
         * done
         */
        return ble_att_chan_mtu(chan);
    }

    return chan->my_mtu;
}

/**
 * Processes an incoming L2CAP fragment.
 *
 * @param conn                  The connection the L2CAP fragment was sent
 *                                  over.
 * @param hci_hdr               The ACL data header that was at the start of
 *                                  the L2CAP fragment.  This header has been
 *                                  stripped from the mbuf parameter.
 * @param om                    An mbuf containing the L2CAP data.  If this is
 *                                  the first fragment, the L2CAP header is at
 *                                  the start of the mbuf.  For subsequent
 *                                  fragments, the mbuf starts with L2CAP
 *                                  payload data.
 * @param out_rx_cb             If a full L2CAP packet has been received, a
 *                                  pointer to the appropriate handler gets
 *                                  written here.  The caller should pass the
 *                                  receive buffer to this callback.
 * @param out_rx_buf            If a full L2CAP packet has been received, this
 *                                  will point to the entire L2CAP packet.  To
 *                                  process the packet, pass this buffer to the
 *                                  receive handler (out_rx_cb).
 * @param out_reject_cid        Indicates whether an L2CAP Command Reject
 *                                  command should be sent.  If this equals -1,
 *                                  no reject should get sent.  Otherwise, the
 *                                  value indicates the CID that the outgoing
 *                                  reject should specify.
 *
 * @return                      0 if a complete L2CAP packet has been received.
 *                              BLE_HS_EAGAIN if a partial L2CAP packet has
 *                                  been received; more fragments are expected.
 *                              Other value on error.
 */
int
ble_l2cap_rx(struct ble_hs_conn *conn,
             struct hci_data_hdr *hci_hdr,
             struct os_mbuf *om,
             ble_l2cap_rx_fn **out_rx_cb,
             int *out_reject_cid)
{
    struct ble_l2cap_chan *chan;
    struct ble_l2cap_hdr l2cap_hdr;
    uint8_t pb;
    int rc;

    *out_reject_cid = -1;

    pb = BLE_HCI_DATA_PB(hci_hdr->hdh_handle_pb_bc);
    switch (pb) {
    case BLE_HCI_PB_FIRST_FLUSH:
        /* First fragment. */
        rc = ble_l2cap_parse_hdr(om, 0, &l2cap_hdr);
        if (rc != 0) {
            goto err;
        }

        /* Strip L2CAP header from the front of the mbuf. */
        os_mbuf_adj(om, BLE_L2CAP_HDR_SZ);

        chan = ble_hs_conn_chan_find_by_scid(conn, l2cap_hdr.cid);
        if (chan == NULL) {
            rc = BLE_HS_ENOENT;

            /* Unsupported channel. If the target CID is the black hole
             * channel, quietly drop the packet.  Otherwise, send an invalid
             * CID response.
             */
            if (l2cap_hdr.cid != BLE_L2CAP_CID_BLACK_HOLE) {
                BLE_HS_LOG(DEBUG, "rx on unknown L2CAP channel: %d\n",
                           l2cap_hdr.cid);
                *out_reject_cid = l2cap_hdr.cid;
            }
            goto err;
        }

        if (chan->rx_buf != NULL) {
            /* Previous data packet never completed.  Discard old packet. */
            ble_l2cap_remove_rx(conn, chan);
        }

        if (l2cap_hdr.len > ble_l2cap_get_mtu(chan)) {
            /* More data then we expected on the channel */
            rc = BLE_HS_EBADDATA;
            goto err;
        }

        /* Remember channel and length of L2CAP data for reassembly. */
        conn->bhc_rx_chan = chan;
        chan->rx_len = l2cap_hdr.len;
        break;

    case BLE_HCI_PB_MIDDLE:
        chan = conn->bhc_rx_chan;
        if (chan == NULL || chan->rx_buf == NULL) {
            /* Middle fragment without the start.  Discard new packet. */
            rc = BLE_HS_EBADDATA;
            goto err;
        }
        break;

    default:
        rc = BLE_HS_EBADDATA;
        goto err;
    }

    rc = ble_l2cap_rx_payload(conn, chan, om, out_rx_cb);
    om = NULL;
    if (rc != 0) {
        goto err;
    }

    return 0;

err:
    os_mbuf_free_chain(om);
    return rc;
}

/**
 * Transmits the L2CAP payload contained in the specified mbuf.  The supplied
 * mbuf is consumed, regardless of the outcome of the function call.
 *
 * @param chan                  The L2CAP channel to transmit over.
 * @param txom                  The data to transmit.
 *
 * @return                      0 on success; nonzero on error.
 */
int
ble_l2cap_tx(struct ble_hs_conn *conn, struct ble_l2cap_chan *chan,
             struct os_mbuf *txom)
{
    int rc;

    txom = ble_l2cap_prepend_hdr(txom, chan->dcid, OS_MBUF_PKTLEN(txom));
    if (txom == NULL) {
        return BLE_HS_ENOMEM;
    }

    rc = ble_hs_hci_acl_tx(conn, &txom);
    switch (rc) {
    case 0:
        /* Success. */
        return 0;

    case BLE_HS_EAGAIN:
        /* Controller could not accommodate full packet.  Enqueue remainder. */
        STAILQ_INSERT_TAIL(&conn->bhc_tx_q, OS_MBUF_PKTHDR(txom), omp_next);
        return 0;

    default:
        /* Error. */
        return rc;
    }
}

int
ble_l2cap_init(void)
{
    int rc;

    rc = os_mempool_init(&ble_l2cap_chan_pool,
                         MYNEWT_VAL(BLE_L2CAP_MAX_CHANS) +
                         MYNEWT_VAL(BLE_L2CAP_COC_MAX_NUM),
                         sizeof (struct ble_l2cap_chan),
                         ble_l2cap_chan_mem, "ble_l2cap_chan_pool");
    if (rc != 0) {
        return BLE_HS_EOS;
    }

    rc = ble_l2cap_sig_init();
    if (rc != 0) {
        return rc;
    }

    rc = ble_l2cap_coc_init();
    if (rc != 0) {
        return rc;
    }

    rc = ble_sm_init();
    if (rc != 0) {
        return rc;
    }

    rc = stats_init_and_reg(
        STATS_HDR(ble_l2cap_stats), STATS_SIZE_INIT_PARMS(ble_l2cap_stats,
        STATS_SIZE_32), STATS_NAME_INIT_PARMS(ble_l2cap_stats), "ble_l2cap");
    if (rc != 0) {
        return BLE_HS_EOS;
    }

    return 0;
}
