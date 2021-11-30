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

#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#include "nimble/porting/nimble/include/sysinit/sysinit.h"

#if MYNEWT_VAL(BLE_LL_DTM)

#include <assert.h>
#include "nimble/porting/nimble/include/os/os.h"
#include "nimble/nimble/porting/nimble/include/stats/stats.h"
#include "../include/controller/ble_ll.h"
#include "../include/controller/ble_phy.h"
#include "../include/controller/ble_ll_sched.h"
#include "../include/controller/ble_ll_rfmgmt.h"
#include "ble_ll_dtm_priv.h"

STATS_SECT_START(ble_ll_dtm_stats)
    STATS_SECT_ENTRY(rx_count)
    STATS_SECT_ENTRY(tx_failed)
    STATS_SECT_ENTRY(rx_failed)
STATS_SECT_END
STATS_SECT_DECL(ble_ll_dtm_stats) ble_ll_dtm_stats;

STATS_NAME_START(ble_ll_dtm_stats)
    STATS_NAME(ble_ll_dtm_stats, rx_count)
    STATS_NAME(ble_ll_dtm_stats, tx_failed)
    STATS_NAME(ble_ll_dtm_stats, rx_failed)
STATS_NAME_END(ble_phy_stats)

struct dtm_ctx {
    uint8_t payload_packet;
    uint8_t itvl_rem_usec;
    uint16_t num_of_packets;
    uint32_t itvl_ticks;
#if MYNEWT_VAL(BLE_LL_DTM_EXTENSIONS)
    uint16_t num_of_packets_max;
#endif
    int active;
    uint8_t rf_channel;
    uint8_t phy_mode;
    struct os_mbuf *om;
    struct ble_npl_event evt;
    struct ble_ll_sched_item sch;
    uint32_t pdu_start_ticks;
    uint8_t pdu_start_usecs;
};

static struct dtm_ctx g_ble_ll_dtm_ctx;

static const uint8_t g_ble_ll_dtm_prbs9_data[] =
{
    0xff, 0xc1, 0xfb, 0xe8, 0x4c, 0x90, 0x72, 0x8b,
    0xe7, 0xb3, 0x51, 0x89, 0x63, 0xab, 0x23, 0x23,
    0x02, 0x84, 0x18, 0x72, 0xaa, 0x61, 0x2f, 0x3b,
    0x51, 0xa8, 0xe5, 0x37, 0x49, 0xfb, 0xc9, 0xca,
    0x0c, 0x18, 0x53, 0x2c, 0xfd, 0x45, 0xe3, 0x9a,
    0xe6, 0xf1, 0x5d, 0xb0, 0xb6, 0x1b, 0xb4, 0xbe,
    0x2a, 0x50, 0xea, 0xe9, 0x0e, 0x9c, 0x4b, 0x5e,
    0x57, 0x24, 0xcc, 0xa1, 0xb7, 0x59, 0xb8, 0x87,
    0xff, 0xe0, 0x7d, 0x74, 0x26, 0x48, 0xb9, 0xc5,
    0xf3, 0xd9, 0xa8, 0xc4, 0xb1, 0xd5, 0x91, 0x11,
    0x01, 0x42, 0x0c, 0x39, 0xd5, 0xb0, 0x97, 0x9d,
    0x28, 0xd4, 0xf2, 0x9b, 0xa4, 0xfd, 0x64, 0x65,
    0x06, 0x8c, 0x29, 0x96, 0xfe, 0xa2, 0x71, 0x4d,
    0xf3, 0xf8, 0x2e, 0x58, 0xdb, 0x0d, 0x5a, 0x5f,
    0x15, 0x28, 0xf5, 0x74, 0x07, 0xce, 0x25, 0xaf,
    0x2b, 0x12, 0xe6, 0xd0, 0xdb, 0x2c, 0xdc, 0xc3,
    0x7f, 0xf0, 0x3e, 0x3a, 0x13, 0xa4, 0xdc, 0xe2,
    0xf9, 0x6c, 0x54, 0xe2, 0xd8, 0xea, 0xc8, 0x88,
    0x00, 0x21, 0x86, 0x9c, 0x6a, 0xd8, 0xcb, 0x4e,
    0x14, 0x6a, 0xf9, 0x4d, 0xd2, 0x7e, 0xb2, 0x32,
    0x03, 0xc6, 0x14, 0x4b, 0x7f, 0xd1, 0xb8, 0xa6,
    0x79, 0x7c, 0x17, 0xac, 0xed, 0x06, 0xad, 0xaf,
    0x0a, 0x94, 0x7a, 0xba, 0x03, 0xe7, 0x92, 0xd7,
    0x15, 0x09, 0x73, 0xe8, 0x6d, 0x16, 0xee, 0xe1,
    0x3f, 0x78, 0x1f, 0x9d, 0x09, 0x52, 0x6e, 0xf1,
    0x7c, 0x36, 0x2a, 0x71, 0x6c, 0x75, 0x64, 0x44,
    0x80, 0x10, 0x43, 0x4e, 0x35, 0xec, 0x65, 0x27,
    0x0a, 0xb5, 0xfc, 0x26, 0x69, 0x3f, 0x59, 0x99,
    0x01, 0x63, 0x8a, 0xa5, 0xbf, 0x68, 0x5c, 0xd3,
    0x3c, 0xbe, 0x0b, 0xd6, 0x76, 0x83, 0xd6, 0x57,
    0x05, 0x4a, 0x3d, 0xdd, 0x81, 0x73, 0xc9, 0xeb,
    0x8a, 0x84, 0x39, 0xf4, 0x36, 0x0b, 0xf7
};

static const uint8_t g_ble_ll_dtm_prbs15_data[] =
{
    0xff, 0x7f, 0xf0, 0x3e, 0x3a, 0x13, 0xa4, 0xdc,
    0xe2, 0xf9, 0x6c, 0x54, 0xe2, 0xd8, 0xea, 0xc8,
    0x88, 0x00, 0x21, 0x86, 0x9c, 0x6a, 0xd8, 0xcb,
    0x4e, 0x14, 0x6a, 0xf9, 0x4d, 0xd2, 0x7e, 0xb2,
    0x32, 0x03, 0xc6, 0x14, 0x4b, 0x7f, 0xd1, 0xb8,
    0xa6, 0x79, 0x7c, 0x17, 0xac, 0xed, 0x06, 0xad,
    0xaf, 0x0a, 0x94, 0x7a, 0xba, 0x03, 0xe7, 0x92,
    0xd7, 0x15, 0x09, 0x73, 0xe8, 0x6d, 0x16, 0xee,
    0xe1, 0x3f, 0x78, 0x1f, 0x9d, 0x09, 0x52, 0x6e,
    0xf1, 0x7c, 0x36, 0x2a, 0x71, 0x6c, 0x75, 0x64,
    0x44, 0x80, 0x10, 0x43, 0x4e, 0x35, 0xec, 0x65,
    0x27, 0x0a, 0xb5, 0xfc, 0x26, 0x69, 0x3f, 0x59,
    0x99, 0x01, 0x63, 0x8a, 0xa5, 0xbf, 0x68, 0x5c,
    0xd3, 0x3c, 0xbe, 0x0b, 0xd6, 0x76, 0x83, 0xd6,
    0x57, 0x05, 0x4a, 0x3d, 0xdd, 0x81, 0x73, 0xc9,
    0xeb, 0x8a, 0x84, 0x39, 0xf4, 0x36, 0x0b, 0xf7,
    0xf0, 0x1f, 0xbc, 0x8f, 0xce, 0x04, 0x29, 0xb7,
    0x78, 0x3e, 0x1b, 0x95, 0x38, 0xb6, 0x3a, 0x32,
    0x22, 0x40, 0x88, 0x21, 0xa7, 0x1a, 0xf6, 0xb2,
    0x13, 0x85, 0x5a, 0x7e, 0x93, 0xb4, 0x9f, 0xac,
    0xcc, 0x80, 0x31, 0xc5, 0xd2, 0x5f, 0x34, 0xae,
    0x69, 0x1e, 0xdf, 0x05, 0x6b, 0xbb, 0x41, 0xeb,
    0xab, 0x02, 0xa5, 0x9e, 0xee, 0xc0, 0xb9, 0xe4,
    0x75, 0x45, 0xc2, 0x1c, 0x7a, 0x9b, 0x85, 0x7b,
    0xf8, 0x0f, 0xde, 0x47, 0x67, 0x82, 0x94, 0x5b,
    0x3c, 0x9f, 0x8d, 0x4a, 0x1c, 0x5b, 0x1d, 0x19,
    0x11, 0x20, 0xc4, 0x90, 0x53, 0x0d, 0x7b, 0xd9,
    0x89, 0x42, 0x2d, 0xbf, 0x49, 0xda, 0x4f, 0x56,
    0x66, 0xc0, 0x98, 0x62, 0xe9, 0x2f, 0x1a, 0xd7,
    0x34, 0x8f, 0xef, 0x82, 0xb5, 0xdd, 0xa0, 0xf5,
    0x55, 0x81, 0x52, 0x4f, 0x77, 0xe0, 0x5c, 0xf2,
    0xba, 0x22, 0x61, 0x0e, 0xbd, 0xcd, 0xc2
};

static const uint8_t channel_rf_to_index[] = {
        37,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 38, 11 ,12, 13, 14, 15,
        16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
        34, 35, 36, 39
};

#define BLE_DTM_SYNC_WORD          (0x71764129)
#define BLE_DTM_CRC                (0x555555)

static void ble_ll_dtm_ctx_free(struct dtm_ctx * ctx);

static void
ble_ll_dtm_set_next(struct dtm_ctx *ctx)
{
    struct ble_ll_sched_item *sch = &ctx->sch;

    ctx->pdu_start_ticks += ctx->itvl_ticks;
    ctx->pdu_start_usecs += ctx->itvl_rem_usec;
    if (ctx->pdu_start_usecs >= 31) {
       ctx->pdu_start_ticks++;
       ctx->pdu_start_usecs -= 31;
    }

    sch->start_time = ctx->pdu_start_ticks;
    sch->remainder = ctx->pdu_start_usecs;

    sch->start_time -= g_ble_ll_sched_offset_ticks;
}

static void
ble_ll_dtm_ev_tx_resched_cb(struct ble_npl_event *evt) {
    /* It is called in LL context */
    struct dtm_ctx *ctx = ble_npl_event_get_arg(evt);
    int rc;
    os_sr_t sr;

    OS_ENTER_CRITICAL(sr);
    if (!ctx->active || !ctx->om) {
        OS_EXIT_CRITICAL(sr);
        return;
    }
    OS_EXIT_CRITICAL(sr);

#if MYNEWT_VAL(BLE_LL_DTM_EXTENSIONS)
    if (g_ble_ll_dtm_ctx.num_of_packets_max &&
        (g_ble_ll_dtm_ctx.num_of_packets == g_ble_ll_dtm_ctx.num_of_packets_max)) {
        /*
         * XXX do not send more packets, but also do not stop DTM - it shall be
         * stopped as usual by HCI command since there is no standard way to
         * signal end of test to host.
         */
        return;
    }
#endif

    ble_ll_dtm_set_next(ctx);
    rc = ble_ll_sched_dtm(&ctx->sch);
    BLE_LL_ASSERT(rc == 0);
}

static int ble_ll_dtm_rx_start(void);

static void
ble_ll_dtm_ev_rx_restart_cb(struct ble_npl_event *evt) {
    if (ble_ll_dtm_rx_start() != 0) {
        ble_npl_eventq_put(&g_ble_ll_data.ll_evq, &g_ble_ll_dtm_ctx.evt);
        STATS_INC(ble_ll_dtm_stats, rx_failed);
    }
}

static void
ble_ll_dtm_tx_done(void *arg)
{
    struct dtm_ctx *ctx;

    ctx = arg;
    if (!ctx->active) {
        return;
    }

    g_ble_ll_dtm_ctx.num_of_packets++;

    /* Reschedule event in LL context */
    ble_npl_eventq_put(&g_ble_ll_data.ll_evq, &ctx->evt);

    ble_ll_state_set(BLE_LL_STATE_STANDBY);
}

static int
ble_ll_dtm_tx_sched_cb(struct ble_ll_sched_item *sch)
{
    struct dtm_ctx *ctx = sch->cb_arg;
    int rc;

    if (!ctx->active) {
        return BLE_LL_SCHED_STATE_DONE;
    }

    rc = ble_phy_setchan(channel_rf_to_index[ctx->rf_channel],
                                            BLE_DTM_SYNC_WORD, BLE_DTM_CRC);
    if (rc != 0) {
        BLE_LL_ASSERT(0);
        return BLE_LL_SCHED_STATE_DONE;
    }

#if (MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_2M_PHY) || MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CODED_PHY))
    ble_phy_mode_set(ctx->phy_mode, ctx->phy_mode);
#endif
    ble_phy_set_txend_cb(ble_ll_dtm_tx_done, ctx);
    ble_phy_txpwr_set(0);

    sch->start_time += g_ble_ll_sched_offset_ticks;

    rc = ble_phy_tx_set_start_time(sch->start_time, sch->remainder);
    if (rc) {
        goto resched;
    }

    rc = ble_phy_tx(ble_ll_tx_mbuf_pducb, ctx->om, BLE_PHY_TRANSITION_NONE);
    if (rc) {
        goto resched;
    }

    ble_ll_state_set(BLE_LL_STATE_DTM);

    return BLE_LL_SCHED_STATE_DONE;

resched:
    /* Reschedule from LL task if late for this PDU */
    ble_npl_eventq_put(&g_ble_ll_data.ll_evq, &ctx->evt);

    STATS_INC(ble_ll_dtm_stats, tx_failed);

    return BLE_LL_SCHED_STATE_DONE;
}

static void
ble_ll_dtm_calculate_itvl(struct dtm_ctx *ctx, uint8_t len,
                          uint16_t cmd_interval, int phy_mode)
{
    uint32_t l;
    uint32_t itvl_usec;
    uint32_t itvl_ticks;

    /* Calculate interval as per spec Bluetooth 5.0 Vol 6. Part F, 4.1.6 */
    l = ble_ll_pdu_tx_time_get(len + BLE_LL_PDU_HDR_LEN, phy_mode);
    itvl_usec = ((l + 249 + 624) / 625) * 625;

#if MYNEWT_VAL(BLE_LL_DTM_EXTENSIONS)
    if (cmd_interval > itvl_usec) {
        itvl_usec = cmd_interval;
    }
#endif

    itvl_ticks = os_cputime_usecs_to_ticks(itvl_usec);
    ctx->itvl_rem_usec = (itvl_usec - os_cputime_ticks_to_usecs(itvl_ticks));
    if (ctx->itvl_rem_usec == 31) {
        ctx->itvl_rem_usec = 0;
        ++itvl_ticks;
    }
    ctx->itvl_ticks = itvl_ticks;
}

static int
ble_ll_dtm_tx_create_ctx(uint8_t packet_payload, uint8_t len,
                         uint8_t rf_channel, uint8_t phy_mode,
                         uint16_t cmd_interval, uint16_t cmd_pkt_count)
{
    int rc = 0;
    uint8_t byte_pattern;
    struct ble_mbuf_hdr *ble_hdr;
    struct os_mbuf *m;
    struct dtm_ctx *ctx = &g_ble_ll_dtm_ctx;
    struct ble_ll_sched_item *sch = &ctx->sch;

    /* MSYS is big enough to get continues memory */
    m = os_msys_get_pkthdr(len, sizeof(struct ble_mbuf_hdr));
    ctx->om = m;
    BLE_LL_ASSERT(g_ble_ll_dtm_ctx.om);

    ctx->phy_mode = phy_mode;
    ctx->rf_channel = rf_channel;
    ctx->num_of_packets = 0;
#if MYNEWT_VAL(BLE_LL_DTM_EXTENSIONS)
    ctx->num_of_packets_max = cmd_pkt_count;
#endif

    /* Set BLE transmit header */
    ble_hdr = BLE_MBUF_HDR_PTR(m);
    ble_hdr->txinfo.flags = 0;
    ble_hdr->txinfo.offset = 0;
    ble_hdr->txinfo.pyld_len = len;
    ble_hdr->txinfo.hdr_byte = packet_payload;

    switch(packet_payload) {
    case 0x00:
        if (os_mbuf_copyinto(m, 0, &g_ble_ll_dtm_prbs9_data, len)) {
            return 1;
        }
        goto schedule;
    case 0x01:
        byte_pattern = 0x0F;
        break;
    case 0x02:
        byte_pattern = 0x55;
        break;
    case 0x03:
        if (os_mbuf_copyinto(m, 0, &g_ble_ll_dtm_prbs15_data, len)) {
            return 1;
        }
        goto schedule;
    case 0x04:
        byte_pattern = 0xFF;
        break;
    case 0x05:
        byte_pattern = 0x00;
        break;
    case 0x06:
        byte_pattern = 0xF0;
        break;
    case 0x07:
        byte_pattern = 0xAA;
        break;
    default:
        return 1;
    }

    for (rc = 0; rc < len; rc++) {
        if (os_mbuf_copyinto(m, rc, &byte_pattern, 1)) {
            return 1;
        }
    }

schedule:
    ble_phy_enable_dtm();

    sch->sched_cb = ble_ll_dtm_tx_sched_cb;
    sch->cb_arg = ctx;
    sch->sched_type = BLE_LL_SCHED_TYPE_DTM;

    /* Prepare os_event */
    ble_npl_event_init(&ctx->evt, ble_ll_dtm_ev_tx_resched_cb, ctx);

    ble_ll_dtm_calculate_itvl(ctx, len, cmd_interval, phy_mode);

    ctx->pdu_start_ticks = ble_ll_rfmgmt_enable_now();
    ctx->pdu_start_usecs = 0;
    ble_ll_dtm_set_next(ctx);

    /* Set some start point for TX packets */
    rc = ble_ll_sched_dtm(sch);
    BLE_LL_ASSERT(rc == 0);

    g_ble_ll_dtm_ctx.active = 1;
    return 0;
}

static int
ble_ll_dtm_rx_start(void)
{
    os_sr_t sr;
    int rc;

    rc = ble_phy_setchan(channel_rf_to_index[g_ble_ll_dtm_ctx.rf_channel],
                         BLE_DTM_SYNC_WORD, BLE_DTM_CRC);
    if (rc) {
        return rc;
    }

#if (MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_2M_PHY) || MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CODED_PHY))
    ble_phy_mode_set(g_ble_ll_dtm_ctx.phy_mode, g_ble_ll_dtm_ctx.phy_mode);
#endif

    OS_ENTER_CRITICAL(sr);
    rc = ble_phy_rx_set_start_time(os_cputime_get32(), 0);
    OS_EXIT_CRITICAL(sr);
    if (rc && rc != BLE_PHY_ERR_RX_LATE) {
        return rc;
    }

    ble_ll_state_set(BLE_LL_STATE_DTM);

    return 0;
}

static int
ble_ll_dtm_rx_sched_cb(struct ble_ll_sched_item *sch)
{
    if (ble_ll_dtm_rx_start() != 0) {
        ble_npl_eventq_put(&g_ble_ll_data.ll_evq, &g_ble_ll_dtm_ctx.evt);
        STATS_INC(ble_ll_dtm_stats, rx_failed);
    }

    return BLE_LL_SCHED_STATE_DONE;
}

static int
ble_ll_dtm_rx_create_ctx(uint8_t rf_channel, uint8_t phy_mode)
{
    struct ble_ll_sched_item *sch = &g_ble_ll_dtm_ctx.sch;
    int rc;

    g_ble_ll_dtm_ctx.phy_mode = phy_mode;
    g_ble_ll_dtm_ctx.rf_channel = rf_channel;

    STATS_CLEAR(ble_ll_dtm_stats, rx_count);

    ble_npl_event_init(&g_ble_ll_dtm_ctx.evt, ble_ll_dtm_ev_rx_restart_cb,
                       NULL);

    sch->sched_cb = ble_ll_dtm_rx_sched_cb;
    sch->cb_arg = &g_ble_ll_dtm_ctx;
    sch->sched_type = BLE_LL_SCHED_TYPE_DTM;
    sch->start_time =  ble_ll_rfmgmt_enable_now();

    rc = ble_ll_sched_dtm(sch);
    BLE_LL_ASSERT(rc == 0);

    ble_phy_enable_dtm();

    g_ble_ll_dtm_ctx.active = 1;
    return 0;
}

static void
ble_ll_dtm_ctx_free(struct dtm_ctx * ctx)
{
    os_sr_t sr;

    OS_ENTER_CRITICAL(sr);
    if (!ctx->active) {
        OS_EXIT_CRITICAL(sr);
        return;
    }

    ble_ll_sched_rmv_elem(&ctx->sch);
    ble_npl_eventq_remove(&g_ble_ll_data.ll_evq, &g_ble_ll_dtm_ctx.evt);

    ble_phy_disable();
    ble_phy_disable_dtm();
    ble_ll_state_set(BLE_LL_STATE_STANDBY);
    ble_ll_rfmgmt_release();

    os_mbuf_free_chain(ctx->om);
    memset(ctx, 0, sizeof(*ctx));
    OS_EXIT_CRITICAL(sr);
}

static int
ble_ll_dtm_tx_test(uint8_t tx_chan, uint8_t len, uint8_t packet_payload,
                   uint8_t hci_phy, uint16_t interval, uint16_t pkt_count)
{
    uint8_t phy_mode;

    if (g_ble_ll_dtm_ctx.active) {
        return BLE_ERR_CTLR_BUSY;
    }

    switch (hci_phy) {
    case BLE_HCI_LE_PHY_1M:
        phy_mode = BLE_PHY_MODE_1M;
        break;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_2M_PHY)
    case BLE_HCI_LE_PHY_2M:
        phy_mode = BLE_PHY_MODE_2M;
        break;
#endif
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CODED_PHY)
    case BLE_HCI_LE_PHY_CODED_S8:
        phy_mode = BLE_PHY_MODE_CODED_125KBPS;
        break;
    case BLE_HCI_LE_PHY_CODED_S2:
        phy_mode = BLE_PHY_MODE_CODED_500KBPS;
        break;
#endif
    default:
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    if (tx_chan > 0x27 || packet_payload > 0x07) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    if (ble_ll_dtm_tx_create_ctx(packet_payload, len, tx_chan, phy_mode,
                                 interval, pkt_count)) {
        return BLE_ERR_UNSPECIFIED;
    }

    return BLE_ERR_SUCCESS;
}

#if MYNEWT_VAL(BLE_LL_DTM_EXTENSIONS)
static int
ble_ll_hci_dtm_tx_test_ext(const uint8_t *cmdbuf)
{
    const struct ble_hci_le_tx_test_ext_cp *cmd = (const void *) cmdbuf;

    return ble_ll_dtm_tx_test(cmd->tx_chan, cmd->test_data_len, cmd->payload,
                              BLE_HCI_LE_PHY_1M, le16toh(cmd->interval),
                              le16toh(cmd->pkt_count));
}

static int
ble_ll_hci_dtm_tx_test_v2_ext(const uint8_t *cmdbuf)
{
    const struct ble_hci_le_tx_test_v2_ext_cp *cmd = (const void *) cmdbuf;

    return ble_ll_dtm_tx_test(cmd->tx_chan, cmd->test_data_len, cmd->payload,
                              cmd->phy, le16toh(cmd->interval),
                              le16toh(cmd->pkt_count));
}
#endif

int
ble_ll_hci_dtm_tx_test(const uint8_t *cmdbuf, uint8_t len)
{
    const struct ble_hci_le_tx_test_cp *cmd = (const void *) cmdbuf;

#if MYNEWT_VAL(BLE_LL_DTM_EXTENSIONS)
    if (len == sizeof(struct ble_hci_le_tx_test_ext_cp)) {
        return ble_ll_hci_dtm_tx_test_ext(cmdbuf);
    }
#endif

    if (len != sizeof(*cmd)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    return ble_ll_dtm_tx_test(cmd->tx_chan, cmd->test_data_len, cmd->payload,
                              BLE_HCI_LE_PHY_1M, 0, 0);
}

int
ble_ll_hci_dtm_tx_test_v2(const uint8_t *cmdbuf, uint8_t len)
{
    const struct ble_hci_le_tx_test_v2_cp *cmd = (const void *) cmdbuf;

#if MYNEWT_VAL(BLE_LL_DTM_EXTENSIONS)
    if (len == sizeof(struct ble_hci_le_tx_test_v2_ext_cp)) {
        return ble_ll_hci_dtm_tx_test_v2_ext(cmdbuf);
    }
#endif

    if (len != sizeof(*cmd)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    return ble_ll_dtm_tx_test(cmd->tx_chan, cmd->test_data_len, cmd->payload,
                              cmd->phy, 0, 0);
}

static int
ble_ll_dtm_rx_test(uint8_t rx_chan, uint8_t hci_phy)
{
    uint8_t phy_mode;

    if (g_ble_ll_dtm_ctx.active) {
        return BLE_ERR_CTLR_BUSY;
    }

    switch (hci_phy) {
    case BLE_HCI_LE_PHY_1M:
        phy_mode = BLE_PHY_MODE_1M;
        break;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_2M_PHY)
    case BLE_HCI_LE_PHY_2M:
        phy_mode = BLE_PHY_MODE_2M;
        break;
#endif
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CODED_PHY)
    case BLE_HCI_LE_PHY_CODED:
        phy_mode = BLE_PHY_MODE_CODED_500KBPS;
        break;
#endif
    default:
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    if (rx_chan > 0x27) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    if (ble_ll_dtm_rx_create_ctx(rx_chan, phy_mode)) {
        return BLE_ERR_UNSPECIFIED;
    }

    return BLE_ERR_SUCCESS;
}

int ble_ll_hci_dtm_rx_test(const uint8_t *cmdbuf, uint8_t len)
{
    const struct ble_hci_le_rx_test_cp *cmd = (const void *) cmdbuf;

    if (len != sizeof(*cmd)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    return ble_ll_dtm_rx_test(cmd->rx_chan, BLE_HCI_LE_PHY_1M);
}

int ble_ll_hci_dtm_rx_test_v2(const uint8_t *cmdbuf, uint8_t len)
{
    const struct ble_hci_le_rx_test_v2_cp *cmd = (const void *) cmdbuf;

    if (len != sizeof(*cmd)) {
        return BLE_ERR_INV_HCI_CMD_PARMS;
    }

    /* TODO ignoring modulation index */

    return ble_ll_dtm_rx_test(cmd->rx_chan, cmd->phy);
}

int ble_ll_dtm_end_test(uint8_t *rsp, uint8_t *rsplen)
{
    put_le16(rsp, g_ble_ll_dtm_ctx. num_of_packets);
    *rsplen = 2;

    ble_ll_dtm_ctx_free(&g_ble_ll_dtm_ctx);
    return BLE_ERR_SUCCESS;
}

int ble_ll_dtm_rx_isr_start(struct ble_mbuf_hdr *rxhdr, uint32_t aa)
{
    return 0;
}

void
ble_ll_dtm_rx_pkt_in(struct os_mbuf *rxpdu, struct ble_mbuf_hdr *hdr)
{
    if (BLE_MBUF_HDR_CRC_OK(hdr)) {
        /* XXX Compare data. */
        g_ble_ll_dtm_ctx.num_of_packets++;
        STATS_INC(ble_ll_dtm_stats, rx_count);
    }

    if (ble_ll_dtm_rx_start() != 0) {
        ble_npl_eventq_put(&g_ble_ll_data.ll_evq, &g_ble_ll_dtm_ctx.evt);
        STATS_INC(ble_ll_dtm_stats, rx_failed);
    }
}

int
ble_ll_dtm_rx_isr_end(uint8_t *rxbuf, struct ble_mbuf_hdr *rxhdr)
{
    struct os_mbuf *rxpdu;

    if (!g_ble_ll_dtm_ctx.active) {
        return -1;
    }

    rxpdu = ble_ll_rxpdu_alloc(rxbuf[1] + BLE_LL_PDU_HDR_LEN);

    /* Copy the received pdu and hand it up */
    if (rxpdu) {
        ble_phy_rxpdu_copy(rxbuf, rxpdu);
        ble_ll_rx_pdu_in(rxpdu);
    }

    return 0;
}

void
ble_ll_dtm_wfr_timer_exp(void)
{
    /* Should not be needed */
    BLE_LL_ASSERT(0);
}


void
ble_ll_dtm_reset(void)
{
    ble_ll_dtm_ctx_free(&g_ble_ll_dtm_ctx);
}

void
ble_ll_dtm_init(void)
{
    int rc;

    rc = stats_init_and_reg(STATS_HDR(ble_ll_dtm_stats),
                            STATS_SIZE_INIT_PARMS(ble_ll_dtm_stats, STATS_SIZE_32),
                            STATS_NAME_INIT_PARMS(ble_ll_dtm_stats),
                            "ble_ll_dtm");
    SYSINIT_PANIC_ASSERT(rc == 0);
}
#endif
#endif