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

 #if defined(ARDUINO_ARCH_NRF5) && defined(NRF51)

#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#include "nimble/porting/nimble/include/os/os.h"
#include "../include/ble/xcvr.h"
#include "nimble/nimble/include/nimble/ble.h"
#include "nimble/nimble/include/nimble/nimble_opt.h"
#include "nimble/nimble/controller/include/controller/ble_phy.h"
#include "nimble/nimble/controller/include/controller/ble_phy_trace.h"
#include "nimble/nimble/controller/include/controller/ble_ll.h"
#include "nrf.h"

#if MYNEWT
#include "mcu/nrf51_clock.h"
#include "mcu/cmsis_nvic.h"
#else
#include "core_cm0.h"
#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_2M_PHY)
#error LE 2M PHY cannot be enabled on nRF51
#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CODED_PHY)
#error LE Coded PHY cannot be enabled on nRF51
#endif

/* XXX: 4) Make sure RF is higher priority interrupt than schedule */

/*
 * XXX: Maximum possible transmit time is 1 msec for a 60ppm crystal
 * and 16ms for a 30ppm crystal! We need to limit PDU size based on
 * crystal accuracy. Look at this in the spec.
 */

/* XXX: private header file? */
extern uint8_t g_nrf_num_irks;
extern uint32_t g_nrf_irk_list[];

/* To disable all radio interrupts */
#define NRF_RADIO_IRQ_MASK_ALL  (0x34FF)

/*
 * We configure the nrf with a 1 byte S0 field, 8 bit length field, and
 * zero bit S1 field. The preamble is 8 bits long.
 */
#define NRF_LFLEN_BITS          (8)
#define NRF_S0_LEN              (1)

/* Maximum length of frames */
#define NRF_MAXLEN              (255)
#define NRF_BALEN               (3)     /* For base address of 3 bytes */

/* Maximum tx power */
#define NRF_TX_PWR_MAX_DBM      (4)
#define NRF_TX_PWR_MIN_DBM      (-40)

/* Max. encrypted payload length */
#define NRF_MAX_ENCRYPTED_PYLD_LEN  (27)
#define NRF_ENC_HDR_SIZE            (3)
#define NRF_ENC_BUF_SIZE            \
    (NRF_MAX_ENCRYPTED_PYLD_LEN + NRF_ENC_HDR_SIZE + BLE_LL_DATA_MIC_LEN)

/* BLE PHY data structure */
struct ble_phy_obj
{
    uint8_t phy_stats_initialized;
    int8_t  phy_txpwr_dbm;
    uint8_t phy_chan;
    uint8_t phy_state;
    uint8_t phy_transition;
    uint8_t phy_rx_started;
    uint8_t phy_encrypted;
    uint8_t phy_privacy;
    uint8_t phy_tx_pyld_len;
    uint8_t *rxdptr;
    int8_t  rx_pwr_compensation;
    uint32_t phy_aar_scratch;
    uint32_t phy_access_address;
    struct ble_mbuf_hdr rxhdr;
    void *txend_arg;
    ble_phy_tx_end_func txend_cb;
    uint32_t phy_start_cputime;
};
struct ble_phy_obj g_ble_phy_data;

/* XXX: if 27 byte packets desired we can make this smaller */
/* Global transmit/receive buffer */
static uint32_t g_ble_phy_tx_buf[(BLE_PHY_MAX_PDU_LEN + 3) / 4];
static uint32_t g_ble_phy_rx_buf[(BLE_PHY_MAX_PDU_LEN + 3) / 4];

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
/* Make sure word-aligned for faster copies */
static uint32_t g_ble_phy_enc_buf[(NRF_ENC_BUF_SIZE + 3) / 4];
#endif

/* RF center frequency for each channel index (offset from 2400 MHz) */
static const uint8_t g_ble_phy_chan_freq[BLE_PHY_NUM_CHANS] = {
     4,  6,  8, 10, 12, 14, 16, 18, 20, 22, /* 0-9 */
    24, 28, 30, 32, 34, 36, 38, 40, 42, 44, /* 10-19 */
    46, 48, 50, 52, 54, 56, 58, 60, 62, 64, /* 20-29 */
    66, 68, 70, 72, 74, 76, 78,  2, 26, 80, /* 30-39 */
};

/* Statistics */
STATS_SECT_START(ble_phy_stats)
    STATS_SECT_ENTRY(phy_isrs)
    STATS_SECT_ENTRY(tx_good)
    STATS_SECT_ENTRY(tx_fail)
    STATS_SECT_ENTRY(tx_late)
    STATS_SECT_ENTRY(tx_bytes)
    STATS_SECT_ENTRY(rx_starts)
    STATS_SECT_ENTRY(rx_aborts)
    STATS_SECT_ENTRY(rx_valid)
    STATS_SECT_ENTRY(rx_crc_err)
    STATS_SECT_ENTRY(rx_late)
    STATS_SECT_ENTRY(radio_state_errs)
    STATS_SECT_ENTRY(rx_hw_err)
    STATS_SECT_ENTRY(tx_hw_err)
STATS_SECT_END
STATS_SECT_DECL(ble_phy_stats) ble_phy_stats;

STATS_NAME_START(ble_phy_stats)
    STATS_NAME(ble_phy_stats, phy_isrs)
    STATS_NAME(ble_phy_stats, tx_good)
    STATS_NAME(ble_phy_stats, tx_fail)
    STATS_NAME(ble_phy_stats, tx_late)
    STATS_NAME(ble_phy_stats, tx_bytes)
    STATS_NAME(ble_phy_stats, rx_starts)
    STATS_NAME(ble_phy_stats, rx_aborts)
    STATS_NAME(ble_phy_stats, rx_valid)
    STATS_NAME(ble_phy_stats, rx_crc_err)
    STATS_NAME(ble_phy_stats, rx_late)
    STATS_NAME(ble_phy_stats, radio_state_errs)
    STATS_NAME(ble_phy_stats, rx_hw_err)
    STATS_NAME(ble_phy_stats, tx_hw_err)
STATS_NAME_END(ble_phy_stats)

/*
 * NOTE:
 * Tested the following to see what would happen:
 *  -> NVIC has radio irq enabled (interrupt # 1, mask 0x2).
 *  -> Set up nrf to receive. Clear ADDRESS event register.
 *  -> Enable ADDRESS interrupt on nrf5 by writing to INTENSET.
 *  -> Enable RX.
 *  -> Disable interrupts globally using OS_ENTER_CRITICAL().
 *  -> Wait until a packet is received and the ADDRESS event occurs.
 *  -> Call ble_phy_disable().
 *
 *  At this point I wanted to see the state of the cortex NVIC. The IRQ
 *  pending bit was TRUE for the radio interrupt (as expected) as we never
 *  serviced the radio interrupt (interrupts were disabled).
 *
 *  What was unexpected was this: without clearing the pending IRQ in the NVIC,
 *  when radio interrupts were re-enabled (address event bit in INTENSET set to
 *  1) and the radio ADDRESS event register read 1 (it was never cleared after
 *  the first address event), the radio did not enter the ISR! I would have
 *  expected that if the following were true, an interrupt would occur:
 *      -> NVIC ISER bit set to TRUE
 *      -> NVIC ISPR bit reads TRUE, meaning interrupt is pending.
 *      -> Radio peripheral interrupts are enabled for some event (or events).
 *      -> Corresponding event register(s) in radio peripheral read 1.
 *
 *  Not sure what the end result of all this is. We will clear the pending
 *  bit in the NVIC just to be sure when we disable the PHY.
 */

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
/* Per nordic, the number of bytes needed for scratch is 16 + MAX_PKT_SIZE. */
#define NRF_ENC_SCRATCH_WORDS   (((NRF_MAX_ENCRYPTED_PYLD_LEN + 16) + 3) / 4)

uint32_t g_nrf_encrypt_scratchpad[NRF_ENC_SCRATCH_WORDS];

struct nrf_ccm_data
{
    uint8_t key[16];
    uint64_t pkt_counter;
    uint8_t dir_bit;
    uint8_t iv[8];
} __attribute__((packed));

struct nrf_ccm_data g_nrf_ccm_data;
#endif

/**
 * Copies the data from the phy receive buffer into a mbuf chain.
 *
 * @param dptr Pointer to receive buffer
 * @param rxpdu Pointer to already allocated mbuf chain
 *
 * NOTE: the packet header already has the total mbuf length in it. The
 * lengths of the individual mbufs are not set prior to calling.
 *
 */
void
ble_phy_rxpdu_copy(uint8_t *dptr, struct os_mbuf *rxpdu)
{
    uint32_t rem_len;
    uint32_t copy_len;
    uint32_t block_len;
    uint32_t block_rem_len;
    void *dst;
    void *src;
    struct os_mbuf * om;

    /* Better be aligned */
    assert(((uint32_t)dptr & 3) == 0);

    block_len = rxpdu->om_omp->omp_databuf_len;
    rem_len = OS_MBUF_PKTHDR(rxpdu)->omp_len;
    src = dptr;

    /*
     * Setup for copying from first mbuf which is shorter due to packet header
     * and extra leading space
     */
    copy_len = block_len - rxpdu->om_pkthdr_len - 4;
    om = rxpdu;
    dst = om->om_data;

    while (true) {
        /*
         * Always copy blocks of length aligned to word size, only last mbuf
         * will have remaining non-word size bytes appended.
         */
        block_rem_len = copy_len;
        copy_len = min(copy_len, rem_len);
        copy_len &= ~3;

        dst = om->om_data;
        om->om_len = copy_len;
        rem_len -= copy_len;
        block_rem_len -= copy_len;

        __asm__ volatile (".syntax unified              \n"
                          "   mov  r4, %[len]           \n"
                          "   b    2f                   \n"
                          "1: ldr  r3, [%[src], %[len]] \n"
                          "   str  r3, [%[dst], %[len]] \n"
                          "2: subs %[len], #4           \n"
                          "   bpl  1b                   \n"
                          "   adds %[src], %[src], r4   \n"
                          "   adds %[dst], %[dst], r4   \n"
                          : [dst] "+l" (dst), [src] "+l" (src),
                            [len] "+l" (copy_len)
                          :
                          : "r3", "r4", "memory"
                         );

        if ((rem_len < 4) && (block_rem_len >= rem_len)) {
            break;
        }

        /* Move to next mbuf */
        om = SLIST_NEXT(om, om_next);
        copy_len = block_len;
    }

    /* Copy remaining bytes, if any, to last mbuf */
    om->om_len += rem_len;
    __asm__ volatile (".syntax unified              \n"
                      "   b    2f                   \n"
                      "1: ldrb r3, [%[src], %[len]] \n"
                      "   strb r3, [%[dst], %[len]] \n"
                      "2: subs %[len], #1           \n"
                      "   bpl  1b                   \n"
                      : [len] "+l" (rem_len)
                      : [dst] "l" (dst), [src] "l" (src)
                      : "r3", "memory"
                     );

    /* Copy header */
    memcpy(BLE_MBUF_HDR_PTR(rxpdu), &g_ble_phy_data.rxhdr,
           sizeof(struct ble_mbuf_hdr));
}

/**
 * Called when we want to wait if the radio is in either the rx or tx
 * disable states. We want to wait until that state is over before doing
 * anything to the radio
 */
static void
nrf_wait_disabled(void)
{
    uint32_t state;

    /*
     * RX and TX states have the same values except for 3rd bit (0=RX, 1=TX) so
     * we use RX symbols only.
     */
    state = NRF_RADIO->STATE & 0x07;

    if (state != RADIO_STATE_STATE_Disabled) {
        /* If PHY is in idle state for whatever reason, disable it now */
        if (state == RADIO_STATE_STATE_RxIdle) {
            NRF_RADIO->TASKS_DISABLE = 1;
            STATS_INC(ble_phy_stats, radio_state_errs);
        }

        if (state == RADIO_STATE_STATE_RxDisable) {
            /* This will end within a short time (6 usecs). Just poll */
            while (NRF_RADIO->STATE == state) {
                /* If this fails, something is really wrong. Should last
                 * no more than 6 usecs */
            }
        }
    }
}

/**
 *
 *
 */
int
ble_phy_set_start_time(uint32_t cputime, uint8_t rem_usecs)
{
    uint32_t next_cc;
    uint32_t cur_cc;
    uint32_t cntr;
    uint32_t delta;

    /*
     * XXX: The TXEN time is 140 usecs but there may be additional delays
     * Need to look at this.
     */

    /*
     * With the 32.768 kHz crystal, we may need to adjust the RTC compare
     * value by 1 tick due to the time it takes for TXEN. The code uses a 5 RTC
     * tick offset, which is 152.5 usecs. The TXEN time is 140 usecs. This
     * means that with a remainder of 0, TIMER0 should be set to 12 or 13 (as
     * TIMER0 counts at 1MHz). A remainder of 19 or more we will need to add
     * 1 tick. We dont need to add 1 tick per se, but it does give us slightly
     * more time and thus less of a chance to miss a tick. Another note: we
     * cant set TIMER0 CC to 0 as the compare wont occur; it must be 1 or more.
     * This is why we subtract 18 (as opposed to 19) as rem_uses will be >= 1.
     */
    if (rem_usecs <= 18) {
        cputime -= 5;
        rem_usecs += 12;
    } else {
        cputime -= 4;
        rem_usecs -= 18;
    }

    /*
     * Can we set the RTC compare to start TIMER0? We can do it if:
     *      a) Current compare value is not N+1 or N+2 ticks from current
     *      counter.
     *      b) The value we want to set is not at least N+2 from current
     *      counter.
     *
     * NOTE: since the counter can tick 1 while we do these calculations we
     * need to account for it.
     */
    next_cc = cputime & 0xffffff;
    cur_cc = NRF_RTC0->CC[0];
    cntr = NRF_RTC0->COUNTER;

    delta = (cur_cc - cntr) & 0xffffff;
    if ((delta <= 3) && (delta != 0)) {
        return -1;
    }
    delta = (next_cc - cntr) & 0xffffff;
    if ((delta & 0x800000) || (delta < 3)) {
        return -1;
    }

    /* Clear and set TIMER0 to fire off at proper time */
    NRF_TIMER0->TASKS_CLEAR = 1;
    NRF_TIMER0->CC[0] = rem_usecs;
    NRF_TIMER0->EVENTS_COMPARE[0] = 0;

    /* Set RTC compare to start TIMER0 */
    NRF_RTC0->EVENTS_COMPARE[0] = 0;
    NRF_RTC0->CC[0] = next_cc;
    NRF_RTC0->EVTENSET = RTC_EVTENSET_COMPARE0_Msk;

    /* Enable PPI */
    NRF_PPI->CHENSET = PPI_CHEN_CH31_Msk;

    /* Store the cputime at which we set the RTC */
    g_ble_phy_data.phy_start_cputime = cputime;

    return 0;
}

/**
 * Function is used to set PPI so that we can time out waiting for a reception
 * to occur. This happens for two reasons: we have sent a packet and we are
 * waiting for a respons (txrx should be set to ENABLE_TXRX) or we are
 * starting a connection event and we are a slave and we are waiting for the
 * master to send us a packet (txrx should be set to ENABLE_RX).
 *
 * NOTE: when waiting for a txrx turn-around, wfr_usecs is not used as there
 * is no additional time to wait; we know when we should receive the address of
 * the received frame.
 *
 * @param txrx Flag denoting if this wfr is a txrx turn-around or not.
 * @param tx_phy_mode phy mode for last TX (not used on nRF51)
 * @param wfr_usecs Amount of usecs to wait.
 */
void
ble_phy_wfr_enable(int txrx, uint8_t tx_phy_mode, uint32_t wfr_usecs)
{
    uint32_t end_time;

    if (txrx == BLE_PHY_WFR_ENABLE_TXRX) {
        /*
         * Timeout occurs an IFS time plus time it takes to receive address
         * from the transmit end. We add additional time to make sure the
         * address event comes before the compare. Note that transmit end
         * is captured in CC[2]. I just made up the 16 usecs I add here.
         */
        end_time = NRF_TIMER0->CC[2] + BLE_LL_IFS +
            ble_phy_mode_pdu_start_off(BLE_PHY_MODE_1M) + 16;
    } else {
        /* CC[0] is set to when RXEN occurs. */
        end_time = NRF_TIMER0->CC[0] + XCVR_RX_START_DELAY_USECS + wfr_usecs +
            ble_phy_mode_pdu_start_off(BLE_PHY_MODE_1M) + BLE_LL_JITTER_USECS;
    }

    /* wfr_secs is the time from rxen until timeout */
    NRF_TIMER0->CC[3] = end_time;
    NRF_TIMER0->EVENTS_COMPARE[3] = 0;

    /* Enable wait for response PPI */
    NRF_PPI->CHENSET = (PPI_CHEN_CH4_Msk | PPI_CHEN_CH5_Msk);

    /* Enable the disabled interrupt so we time out on events compare */
    NRF_RADIO->INTENSET = RADIO_INTENSET_DISABLED_Msk;
}

/**
 * Setup transceiver for receive.
 */
static void
ble_phy_rx_xcvr_setup(void)
{
    uint8_t *dptr;

    dptr = (uint8_t *)&g_ble_phy_rx_buf[0];

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
    if (g_ble_phy_data.phy_encrypted) {
        dptr += 3;
        NRF_RADIO->PACKETPTR = (uint32_t)&g_ble_phy_enc_buf[0];
        NRF_CCM->INPTR = (uint32_t)&g_ble_phy_enc_buf[0];
        NRF_CCM->OUTPTR = (uint32_t)dptr;
        NRF_CCM->SCRATCHPTR = (uint32_t)&g_nrf_encrypt_scratchpad[0];
        NRF_CCM->MODE = CCM_MODE_MODE_Decryption;
        NRF_CCM->CNFPTR = (uint32_t)&g_nrf_ccm_data;
        NRF_CCM->SHORTS = 0;
        NRF_CCM->EVENTS_ERROR = 0;
        NRF_CCM->EVENTS_ENDCRYPT = 0;
        NRF_PPI->CHENSET = PPI_CHEN_CH24_Msk | PPI_CHEN_CH25_Msk;
    } else {
        NRF_RADIO->PACKETPTR = (uint32_t)dptr;
    }
#else
    NRF_RADIO->PACKETPTR = (uint32_t)dptr;
#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
    if (g_ble_phy_data.phy_privacy) {
        dptr += 3;
        NRF_RADIO->PACKETPTR = (uint32_t)dptr;
        NRF_RADIO->PCNF0 = (6 << RADIO_PCNF0_LFLEN_Pos) |
                           (2 << RADIO_PCNF0_S1LEN_Pos) |
                           (NRF_S0_LEN << RADIO_PCNF0_S0LEN_Pos);
        NRF_AAR->ENABLE = AAR_ENABLE_ENABLE_Enabled;
        NRF_AAR->IRKPTR = (uint32_t)&g_nrf_irk_list[0];
        NRF_AAR->SCRATCHPTR = (uint32_t)&g_ble_phy_data.phy_aar_scratch;
        NRF_AAR->EVENTS_END = 0;
        NRF_AAR->EVENTS_RESOLVED = 0;
        NRF_AAR->EVENTS_NOTRESOLVED = 0;
    } else {
        if (g_ble_phy_data.phy_encrypted == 0) {
            NRF_RADIO->PCNF0 = (NRF_LFLEN_BITS << RADIO_PCNF0_LFLEN_Pos) |
                               (NRF_S0_LEN << RADIO_PCNF0_S0LEN_Pos);
            /* XXX: do I only need to do this once? Figure out what I can do
               once. */
            NRF_AAR->ENABLE = AAR_ENABLE_ENABLE_Disabled;
        }
    }
#endif

    /* Turn off trigger TXEN on output compare match and AAR on bcmatch */
    NRF_PPI->CHENCLR = PPI_CHEN_CH20_Msk | PPI_CHEN_CH23_Msk;

    /* Reset the rx started flag. Used for the wait for response */
    g_ble_phy_data.phy_rx_started = 0;
    g_ble_phy_data.phy_state = BLE_PHY_STATE_RX;
    g_ble_phy_data.rxdptr = dptr;

    /* I want to know when 1st byte received (after address) */
    NRF_RADIO->BCC = 8; /* in bits */
    NRF_RADIO->EVENTS_ADDRESS = 0;
    NRF_RADIO->EVENTS_DEVMATCH = 0;
    NRF_RADIO->EVENTS_BCMATCH = 0;
    NRF_RADIO->EVENTS_RSSIEND = 0;
    NRF_RADIO->SHORTS = RADIO_SHORTS_END_DISABLE_Msk |
                        RADIO_SHORTS_READY_START_Msk |
                        RADIO_SHORTS_DISABLED_TXEN_Msk |
                        RADIO_SHORTS_ADDRESS_BCSTART_Msk |
                        RADIO_SHORTS_ADDRESS_RSSISTART_Msk |
                        RADIO_SHORTS_DISABLED_RSSISTOP_Msk;

    NRF_RADIO->INTENSET = RADIO_INTENSET_ADDRESS_Msk;
}

/**
 * Called from interrupt context when the transmit ends
 *
 */
static void
ble_phy_tx_end_isr(void)
{
    uint8_t was_encrypted;
    uint8_t transition;
    uint8_t txlen;
    uint32_t wfr_time;

    /* If this transmission was encrypted we need to remember it */
    was_encrypted = g_ble_phy_data.phy_encrypted;
    (void)was_encrypted;

    /* Better be in TX state! */
    assert(g_ble_phy_data.phy_state == BLE_PHY_STATE_TX);

    /* Clear events and clear interrupt on disabled event */
    NRF_RADIO->EVENTS_DISABLED = 0;
    NRF_RADIO->INTENCLR = RADIO_INTENCLR_DISABLED_Msk;
    NRF_RADIO->EVENTS_END = 0;
    wfr_time = NRF_RADIO->SHORTS;
    (void)wfr_time;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
    /*
     * XXX: not sure what to do. We had a HW error during transmission.
     * For now I just count a stat but continue on like all is good.
     */
    if (was_encrypted) {
        if (NRF_CCM->EVENTS_ERROR) {
            STATS_INC(ble_phy_stats, tx_hw_err);
            NRF_CCM->EVENTS_ERROR = 0;
        }
    }
#endif

    /* Call transmit end callback */
    if (g_ble_phy_data.txend_cb) {
        g_ble_phy_data.txend_cb(g_ble_phy_data.txend_arg);
    }

    transition = g_ble_phy_data.phy_transition;
    if (transition == BLE_PHY_TRANSITION_TX_RX) {
        /* Packet pointer needs to be reset. */
        ble_phy_rx_xcvr_setup();

        /*
         * Enable the wait for response timer. Note that cc #1 on
         * timer 0 contains the transmit start time
         */
        txlen = g_ble_phy_data.phy_tx_pyld_len;
        if (txlen && was_encrypted) {
            txlen += BLE_LL_DATA_MIC_LEN;
        }
        ble_phy_wfr_enable(BLE_PHY_WFR_ENABLE_TXRX, 0, 0);
    } else {
        /*
         * XXX: not sure we need to stop the timer here all the time. Or that
         * it should be stopped here.
         */
        NRF_TIMER0->TASKS_STOP = 1;
        NRF_TIMER0->TASKS_SHUTDOWN = 1;
        NRF_PPI->CHENCLR = PPI_CHEN_CH4_Msk | PPI_CHEN_CH5_Msk |
                           PPI_CHEN_CH20_Msk | PPI_CHEN_CH31_Msk;
        assert(transition == BLE_PHY_TRANSITION_NONE);
    }
}

static void
ble_phy_rx_end_isr(void)
{
    int rc;
    uint8_t *dptr;
    uint8_t crcok;
    struct ble_mbuf_hdr *ble_hdr;

    /* Clear events and clear interrupt */
    NRF_RADIO->EVENTS_END = 0;
    NRF_RADIO->INTENCLR = RADIO_INTENCLR_END_Msk;

    /* Disable automatic RXEN */
    NRF_PPI->CHENCLR = PPI_CHEN_CH21_Msk;

    /* Set RSSI and CRC status flag in header */
    ble_hdr = &g_ble_phy_data.rxhdr;
    assert(NRF_RADIO->EVENTS_RSSIEND != 0);
    ble_hdr->rxinfo.rssi = (-1 * NRF_RADIO->RSSISAMPLE) +
                            g_ble_phy_data.rx_pwr_compensation;

    dptr = g_ble_phy_data.rxdptr;

    /* Count PHY crc errors and valid packets */
    crcok = (uint8_t)NRF_RADIO->CRCSTATUS;
    if (!crcok) {
        STATS_INC(ble_phy_stats, rx_crc_err);
    } else {
        STATS_INC(ble_phy_stats, rx_valid);
        ble_hdr->rxinfo.flags |= BLE_MBUF_HDR_F_CRC_OK;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
        if (g_ble_phy_data.phy_encrypted) {
            /* Only set MIC failure flag if frame is not zero length */
            if ((dptr[1] != 0) && (NRF_CCM->MICSTATUS == 0)) {
                ble_hdr->rxinfo.flags |= BLE_MBUF_HDR_F_MIC_FAILURE;
            }

            /*
             * XXX: not sure how to deal with this. This should not
             * be a MIC failure but we should not hand it up. I guess
             * this is just some form of rx error and that is how we
             * handle it? For now, just set CRC error flags
             */
            if (NRF_CCM->EVENTS_ERROR) {
                STATS_INC(ble_phy_stats, rx_hw_err);
                ble_hdr->rxinfo.flags &= ~BLE_MBUF_HDR_F_CRC_OK;
            }

            /*
             * XXX: This is a total hack work-around for now but I dont
             * know what else to do. If ENDCRYPT is not set and we are
             * encrypted we need to not trust this frame and drop it.
             */
            if (NRF_CCM->EVENTS_ENDCRYPT == 0) {
                STATS_INC(ble_phy_stats, rx_hw_err);
                ble_hdr->rxinfo.flags &= ~BLE_MBUF_HDR_F_CRC_OK;
            }
        }
#endif
    }

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION) || MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
    if (g_ble_phy_data.phy_encrypted || g_ble_phy_data.phy_privacy) {
        /*
         * XXX: This is a horrible ugly hack to deal with the RAM S1 byte.
         * This should get fixed as we should not be handing up the header
         * and length as part of the pdu.
         */
        dptr[2] = dptr[1];
        dptr[1] = dptr[0];
        ++dptr;
    }
#endif
    rc = ble_ll_rx_end(dptr, ble_hdr);
    if (rc < 0) {
        ble_phy_disable();
    }
}

static void
ble_phy_rx_start_isr(void)
{
    int rc;
    uint32_t state;
    uint32_t usecs;
    uint32_t ticks;
    struct ble_mbuf_hdr *ble_hdr;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
    uint8_t *dptr;

    dptr = (uint8_t *)&g_ble_phy_rx_buf[0];
#endif

    /* Clear events and clear interrupt */
    NRF_RADIO->EVENTS_ADDRESS = 0;

    /* Clear wfr timer channels and DISABLED interrupt */
    NRF_RADIO->INTENCLR = RADIO_INTENCLR_DISABLED_Msk | RADIO_INTENCLR_ADDRESS_Msk;
    NRF_PPI->CHENCLR = PPI_CHEN_CH4_Msk | PPI_CHEN_CH5_Msk;

    /* Initialize flags, channel and state in ble header at rx start */
    ble_hdr = &g_ble_phy_data.rxhdr;
    ble_hdr->rxinfo.flags = ble_ll_state_get();
    ble_hdr->rxinfo.channel = g_ble_phy_data.phy_chan;
    ble_hdr->rxinfo.handle = 0;
    ble_hdr->rxinfo.phy = BLE_PHY_1M;
    ble_hdr->rxinfo.phy_mode = BLE_PHY_MODE_1M;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    ble_hdr->rxinfo.user_data = NULL;
#endif

    /*
     * Calculate receive start time.
     *
     * XXX: possibly use other routine with remainder!
     */
    usecs = NRF_TIMER0->CC[1] - ble_phy_mode_pdu_start_off(BLE_PHY_MODE_1M);
    ticks = os_cputime_usecs_to_ticks(usecs);
    ble_hdr->rem_usecs = usecs - os_cputime_ticks_to_usecs(ticks);
    if (ble_hdr->rem_usecs == 31) {
        ble_hdr->rem_usecs = 0;
        ++ticks;
    }
    ble_hdr->beg_cputime = g_ble_phy_data.phy_start_cputime + ticks;

    /* Wait to get 1st byte of frame */
    while (1) {
        state = NRF_RADIO->STATE;
        if (NRF_RADIO->EVENTS_BCMATCH != 0) {
            break;
        }

        /*
         * If state is disabled, we should have the BCMATCH. If not,
         * something is wrong!
         */
        if (state == RADIO_STATE_STATE_Disabled) {
            NRF_RADIO->INTENCLR = NRF_RADIO_IRQ_MASK_ALL;
            NRF_RADIO->SHORTS = 0;
            return;
        }
    }

    /* Call Link Layer receive start function */
    rc = ble_ll_rx_start(g_ble_phy_data.rxdptr, g_ble_phy_data.phy_chan,
                         &g_ble_phy_data.rxhdr);
    if (rc >= 0) {
        /* Set rx started flag and enable rx end ISR */
        g_ble_phy_data.phy_rx_started = 1;
        NRF_RADIO->INTENSET = RADIO_INTENSET_END_Msk;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
        /* Must start aar if we need to  */
        if (g_ble_phy_data.phy_privacy) {
            NRF_RADIO->EVENTS_BCMATCH = 0;
            NRF_PPI->CHENSET = PPI_CHEN_CH23_Msk;
            /*
             * Setup AAR to resolve AdvA and trigger it after complete address
             * is received, i.e. after PDU header and AdvA is received.
             *
             * AdvA starts at 4th octet in receive buffer, after S0, len and S1
             * fields.
             *
             * In case of extended advertising AdvA is located after extended
             * header (+2 octets).
             */
            if (BLE_MBUF_HDR_EXT_ADV(&g_ble_phy_data.rxhdr)) {
                NRF_AAR->ADDRPTR = (uint32_t)(dptr + 5);
                NRF_RADIO->BCC = (BLE_DEV_ADDR_LEN + BLE_LL_PDU_HDR_LEN + 2) * 8;

            } else {
                NRF_AAR->ADDRPTR = (uint32_t)(dptr + 3);
                NRF_RADIO->BCC = (BLE_DEV_ADDR_LEN + BLE_LL_PDU_HDR_LEN) * 8;
            }
        }
#endif
    } else {
        /* Disable PHY */
        ble_phy_disable();
        STATS_INC(ble_phy_stats, rx_aborts);
    }

    /* Count rx starts */
    STATS_INC(ble_phy_stats, rx_starts);
}

static void
ble_phy_isr(void)
{
    uint32_t irq_en;

    os_trace_isr_enter();

    /* Read irq register to determine which interrupts are enabled */
    irq_en = NRF_RADIO->INTENCLR;

    /*
     * NOTE: order of checking is important! Possible, if things get delayed,
     * we have both an ADDRESS and DISABLED interrupt in rx state. If we get
     * an address, we disable the DISABLED interrupt.
     */

    /* We get this if we have started to receive a frame */
    if ((irq_en & RADIO_INTENCLR_ADDRESS_Msk) && NRF_RADIO->EVENTS_ADDRESS) {
        irq_en &= ~RADIO_INTENCLR_DISABLED_Msk;
        ble_phy_rx_start_isr();
    }

    /* Check for disabled event. This only happens for transmits now */
    if ((irq_en & RADIO_INTENCLR_DISABLED_Msk) && NRF_RADIO->EVENTS_DISABLED) {
        if (g_ble_phy_data.phy_state == BLE_PHY_STATE_RX) {
            NRF_RADIO->EVENTS_DISABLED = 0;
            ble_ll_wfr_timer_exp(NULL);
        } else {
            ble_phy_tx_end_isr();
        }
    }

    /* Receive packet end (we dont enable this for transmit) */
    if ((irq_en & RADIO_INTENCLR_END_Msk) && NRF_RADIO->EVENTS_END) {
        ble_phy_rx_end_isr();
    }

    /* Ensures IRQ is cleared */
    irq_en = NRF_RADIO->SHORTS;

    /* Count # of interrupts */
    STATS_INC(ble_phy_stats, phy_isrs);

    os_trace_isr_exit();
}

/**
 * ble phy init
 *
 * Initialize the PHY.
 *
 * @return int 0: success; PHY error code otherwise
 */
int
ble_phy_init(void)
{
    int rc;

    /* Set phy channel to an invalid channel so first set channel works */
    g_ble_phy_data.phy_chan = BLE_PHY_NUM_CHANS;

    g_ble_phy_data.rx_pwr_compensation = 0;

    /* Toggle peripheral power to reset (just in case) */
    NRF_RADIO->POWER = 0;
    NRF_RADIO->POWER = 1;

    /* Disable all interrupts */
    NRF_RADIO->INTENCLR = NRF_RADIO_IRQ_MASK_ALL;

    /* Set configuration registers */
    NRF_RADIO->MODE = RADIO_MODE_MODE_Ble_1Mbit;
    NRF_RADIO->PCNF0 = (NRF_LFLEN_BITS << RADIO_PCNF0_LFLEN_Pos) |
                       (NRF_S0_LEN << RADIO_PCNF0_S0LEN_Pos);
    /* XXX: should maxlen be 251 for encryption? */
    NRF_RADIO->PCNF1 = NRF_MAXLEN |
                       (RADIO_PCNF1_ENDIAN_Little <<  RADIO_PCNF1_ENDIAN_Pos) |
                       (NRF_BALEN << RADIO_PCNF1_BALEN_Pos) |
                       RADIO_PCNF1_WHITEEN_Msk;

    /* Set base0 with the advertising access address */
    NRF_RADIO->BASE0 = (BLE_ACCESS_ADDR_ADV << 8) & 0xFFFFFF00;
    NRF_RADIO->PREFIX0 = (BLE_ACCESS_ADDR_ADV >> 24) & 0xFF;

    /* Configure the CRC registers */
    NRF_RADIO->CRCCNF = RADIO_CRCCNF_SKIPADDR_Msk | RADIO_CRCCNF_LEN_Three;

    /* Configure BLE poly */
    NRF_RADIO->CRCPOLY = 0x0100065B;

    /* Configure IFS */
    NRF_RADIO->TIFS = BLE_LL_IFS;

    /* Captures tx/rx start in timer0 cc 1 and tx/rx end in timer0 cc 2 */
    NRF_PPI->CHENSET = PPI_CHEN_CH26_Msk | PPI_CHEN_CH27_Msk;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
    NRF_CCM->INTENCLR = 0xffffffff;
    NRF_CCM->SHORTS = CCM_SHORTS_ENDKSGEN_CRYPT_Msk;
    NRF_CCM->EVENTS_ERROR = 0;
    memset(g_nrf_encrypt_scratchpad, 0, sizeof(g_nrf_encrypt_scratchpad));
#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
    g_ble_phy_data.phy_aar_scratch = 0;
    NRF_AAR->IRKPTR = (uint32_t)&g_nrf_irk_list[0];
    NRF_AAR->INTENCLR = 0xffffffff;
    NRF_AAR->EVENTS_END = 0;
    NRF_AAR->EVENTS_RESOLVED = 0;
    NRF_AAR->EVENTS_NOTRESOLVED = 0;
    NRF_AAR->NIRK = 0;
#endif

    /* TIMER0 setup for PHY when using RTC */
    NRF_TIMER0->TASKS_STOP = 1;
    NRF_TIMER0->TASKS_SHUTDOWN = 1;
    NRF_TIMER0->BITMODE = 3;    /* 32-bit timer */
    NRF_TIMER0->MODE = 0;       /* Timer mode */
    NRF_TIMER0->PRESCALER = 4;  /* gives us 1 MHz */

    /*
     * PPI setup.
     * Channel 4: Captures TIMER0 in CC[3] when EVENTS_ADDRESS occurs. Used
     *            to cancel the wait for response timer.
     * Channel 5: TIMER0 CC[3] to TASKS_DISABLE on radio. This is the wait
     *            for response timer.
     */
    NRF_PPI->CH[4].EEP = (uint32_t)&(NRF_RADIO->EVENTS_ADDRESS);
    NRF_PPI->CH[4].TEP = (uint32_t)&(NRF_TIMER0->TASKS_CAPTURE[3]);
    NRF_PPI->CH[5].EEP = (uint32_t)&(NRF_TIMER0->EVENTS_COMPARE[3]);
    NRF_PPI->CH[5].TEP = (uint32_t)&(NRF_RADIO->TASKS_DISABLE);

    /* Set isr in vector table and enable interrupt */
#ifndef RIOT_VERSION
    NVIC_SetPriority(RADIO_IRQn, 0);
#endif
#if MYNEWT
    NVIC_SetVector(RADIO_IRQn, (uint32_t)ble_phy_isr);
#else
    ble_npl_hw_set_isr(RADIO_IRQn, ble_phy_isr);
#endif
    NVIC_EnableIRQ(RADIO_IRQn);

    /* Register phy statistics */
    if (!g_ble_phy_data.phy_stats_initialized) {
        rc = stats_init_and_reg(STATS_HDR(ble_phy_stats),
                                STATS_SIZE_INIT_PARMS(ble_phy_stats,
                                                      STATS_SIZE_32),
                                STATS_NAME_INIT_PARMS(ble_phy_stats),
                                "ble_phy");
        assert(rc == 0);

        g_ble_phy_data.phy_stats_initialized  = 1;
    }

    return 0;
}

/**
 * Puts the phy into receive mode.
 *
 * @return int 0: success; BLE Phy error code otherwise
 */
int
ble_phy_rx(void)
{
    /* Check radio state */
    nrf_wait_disabled();
    if (NRF_RADIO->STATE != RADIO_STATE_STATE_Disabled) {
        ble_phy_disable();
        STATS_INC(ble_phy_stats, radio_state_errs);
        return BLE_PHY_ERR_RADIO_STATE;
    }

    /* Make sure all interrupts are disabled */
    NRF_RADIO->INTENCLR = NRF_RADIO_IRQ_MASK_ALL;

    /* Clear events prior to enabling receive */
    NRF_RADIO->EVENTS_END = 0;
    NRF_RADIO->EVENTS_DISABLED = 0;

    /* Setup for rx */
    ble_phy_rx_xcvr_setup();

    /* Start the receive task in the radio if not automatically going to rx */
    if ((NRF_PPI->CHEN & PPI_CHEN_CH21_Msk) == 0) {
        NRF_RADIO->TASKS_RXEN = 1;
    }

    return 0;
}

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
/**
 * Called to enable encryption at the PHY. Note that this state will persist
 * in the PHY; in other words, if you call this function you have to call
 * disable so that future PHY transmits/receives will not be encrypted.
 *
 * @param pkt_counter
 * @param iv
 * @param key
 * @param is_master
 */
void
ble_phy_encrypt_enable(uint64_t pkt_counter, uint8_t *iv, uint8_t *key,
                       uint8_t is_master)
{
    memcpy(g_nrf_ccm_data.key, key, 16);
    g_nrf_ccm_data.pkt_counter = pkt_counter;
    memcpy(g_nrf_ccm_data.iv, iv, 8);
    g_nrf_ccm_data.dir_bit = is_master;
    g_ble_phy_data.phy_encrypted = 1;

    /* Encryption uses LFLEN=5, S1LEN = 3. */
    NRF_RADIO->PCNF0 = (5 << RADIO_PCNF0_LFLEN_Pos) |
                       (3 << RADIO_PCNF0_S1LEN_Pos) |
                       (NRF_S0_LEN << RADIO_PCNF0_S0LEN_Pos);

    /* Enable the module (AAR cannot be on while CCM on) */
    NRF_AAR->ENABLE = AAR_ENABLE_ENABLE_Disabled;
    NRF_CCM->ENABLE = CCM_ENABLE_ENABLE_Enabled;
}

void
ble_phy_encrypt_set_pkt_cntr(uint64_t pkt_counter, int dir)
{
    g_nrf_ccm_data.pkt_counter = pkt_counter;
    g_nrf_ccm_data.dir_bit = dir;
}

void
ble_phy_encrypt_disable(void)
{
    NRF_PPI->CHENCLR = (PPI_CHEN_CH24_Msk | PPI_CHEN_CH25_Msk);
    NRF_CCM->TASKS_STOP = 1;
    NRF_CCM->EVENTS_ERROR = 0;
    NRF_CCM->ENABLE = CCM_ENABLE_ENABLE_Disabled;

    /* Switch back to normal length */
    NRF_RADIO->PCNF0 = (NRF_LFLEN_BITS << RADIO_PCNF0_LFLEN_Pos) |
                       (NRF_S0_LEN << RADIO_PCNF0_S0LEN_Pos);

    g_ble_phy_data.phy_encrypted = 0;
}
#endif

void
ble_phy_set_txend_cb(ble_phy_tx_end_func txend_cb, void *arg)
{
    /* Set transmit end callback and arg */
    g_ble_phy_data.txend_cb = txend_cb;
    g_ble_phy_data.txend_arg = arg;
}

/**
 * Called to set the start time of a transmission.
 *
 * This function is called to set the start time when we are not going from
 * rx to tx automatically.
 *
 * NOTE: care must be taken when calling this function. The channel should
 * already be set.
 *
 * @param cputime   This is the tick at which the 1st bit of the preamble
 *                  should be transmitted
 * @param rem_usecs This is used only when the underlying timing uses a 32.768
 *                  kHz crystal. It is the # of usecs from the cputime tick
 *                  at which the first bit of the preamble should be
 *                  transmitted.
 * @return int
 */
int
ble_phy_tx_set_start_time(uint32_t cputime, uint8_t rem_usecs)
{
    int rc;

    ble_phy_trace_u32x2(BLE_PHY_TRACE_ID_START_TX, cputime, rem_usecs);

    /* XXX: This should not be necessary, but paranoia is good! */
    /* Clear timer0 compare to RXEN since we are transmitting */
    NRF_PPI->CHENCLR = PPI_CHEN_CH21_Msk;

    /*
     * XXX: The TXEN time is 140 usecs but there may be additional delays
     * Need to look at this.
     */
    if (ble_phy_set_start_time(cputime, rem_usecs) != 0) {
        STATS_INC(ble_phy_stats, tx_late);
        ble_phy_disable();
        rc = BLE_PHY_ERR_TX_LATE;
    } else {
        /* Enable PPI to automatically start TXEN */
        NRF_PPI->CHENSET = PPI_CHEN_CH20_Msk;
        rc = 0;
    }
    return rc;
}
/**
 * Called to set the start time of a reception
 *
 * This function acts a bit differently than transmit. If we are late getting
 * here we will still attempt to receive.
 *
 * NOTE: care must be taken when calling this function. The channel should
 * already be set.
 *
 * @param cputime
 *
 * @return int
 */
int
ble_phy_rx_set_start_time(uint32_t cputime, uint8_t rem_usecs)
{
    int rc;

    ble_phy_trace_u32x2(BLE_PHY_TRACE_ID_START_RX, cputime, rem_usecs);

    /* XXX: This should not be necessary, but paranoia is good! */
    /* Clear timer0 compare to TXEN since we are transmitting */
    NRF_PPI->CHENCLR = PPI_CHEN_CH20_Msk;

    /*
     * XXX: The RXEN time is 138 usecs but there may be additional delays
     * Need to look at this.
     */
    if (ble_phy_set_start_time(cputime, rem_usecs) != 0) {
        STATS_INC(ble_phy_stats, rx_late);
        NRF_PPI->CHENCLR = PPI_CHEN_CH21_Msk;
        NRF_RADIO->TASKS_RXEN = 1;
        rc = BLE_PHY_ERR_RX_LATE;
    } else {
        /* Enable PPI to automatically start RXEN */
        NRF_PPI->CHENSET = PPI_CHEN_CH21_Msk;

        /* Start rx */
        rc = ble_phy_rx();
    }
    return rc;
}

int
ble_phy_tx(ble_phy_tx_pducb_t pducb, void *pducb_arg, uint8_t end_trans)
{
    int rc;
    uint8_t *dptr;
    uint8_t payload_len;
    uint8_t payload_off;
    uint8_t hdr_byte;
    uint32_t state;
    uint32_t shortcuts;

    /*
     * This check is to make sure that the radio is not in a state where
     * it is moving to disabled state. If so, let it get there.
     */
    nrf_wait_disabled();

    /*
     * XXX: Although we may not have to do this here, I clear all the PPI
     * that should not be used when transmitting. Some of them are only enabled
     * if encryption and/or privacy is on, but I dont care. Better to be
     * paranoid, and if you are going to clear one, might as well clear them
     * all.
     */
    NRF_PPI->CHENCLR = PPI_CHEN_CH4_Msk | PPI_CHEN_CH5_Msk | PPI_CHEN_CH23_Msk |
                       PPI_CHEN_CH25_Msk;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
    if (g_ble_phy_data.phy_encrypted) {
        /* RAM representation has S0, LENGTH and S1 fields. (3 bytes) */
        dptr = (uint8_t *)&g_ble_phy_enc_buf[0];
        payload_off = 3;

        NRF_CCM->SHORTS = 1;
        NRF_CCM->INPTR = (uint32_t)&g_ble_phy_enc_buf[0];
        NRF_CCM->OUTPTR = (uint32_t)&g_ble_phy_tx_buf[0];
        NRF_CCM->SCRATCHPTR = (uint32_t)&g_nrf_encrypt_scratchpad[0];
        NRF_CCM->EVENTS_ERROR = 0;
        NRF_CCM->MODE = CCM_MODE_MODE_Encryption;
        NRF_CCM->CNFPTR = (uint32_t)&g_nrf_ccm_data;
        NRF_PPI->CHENSET = PPI_CHEN_CH24_Msk;
    } else {
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
        /* Reconfigure PCNF0 */
        NRF_RADIO->PCNF0 = (NRF_LFLEN_BITS << RADIO_PCNF0_LFLEN_Pos) |
                           (NRF_S0_LEN << RADIO_PCNF0_S0LEN_Pos);
        NRF_AAR->IRKPTR = (uint32_t)&g_nrf_irk_list[0];
#endif
        /* RAM representation has S0 and LENGTH fields (2 bytes) */
        dptr = (uint8_t *)&g_ble_phy_tx_buf[0];
        payload_off = 2;
    }
#else

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
    /* Reconfigure PCNF0 */
    NRF_RADIO->PCNF0 = (NRF_LFLEN_BITS << RADIO_PCNF0_LFLEN_Pos) |
                       (NRF_S0_LEN << RADIO_PCNF0_S0LEN_Pos);
#endif

    /* RAM representation has S0 and LENGTH fields (2 bytes) */
    dptr = (uint8_t *)&g_ble_phy_tx_buf[0];
    payload_off = 2;
#endif

    NRF_RADIO->PACKETPTR = (uint32_t)&g_ble_phy_tx_buf[0];

    /* Clear the ready, end and disabled events */
    NRF_RADIO->EVENTS_READY = 0;
    NRF_RADIO->EVENTS_END = 0;
    NRF_RADIO->EVENTS_DISABLED = 0;

    /* Enable shortcuts for transmit start/end. */
    shortcuts = RADIO_SHORTS_END_DISABLE_Msk | RADIO_SHORTS_READY_START_Msk;
    if (end_trans == BLE_PHY_TRANSITION_TX_RX) {
        shortcuts |= RADIO_SHORTS_DISABLED_RXEN_Msk;
    }
    NRF_RADIO->SHORTS = shortcuts;
    NRF_RADIO->INTENSET = RADIO_INTENSET_DISABLED_Msk;

    /* Set PDU payload */
    payload_len = pducb(&dptr[payload_off], pducb_arg, &hdr_byte);

    /* Set PDU header */
    dptr[0] = hdr_byte;
    dptr[1] = payload_len;
    if (payload_off > 2) {
        dptr[2] = 0;
    }

    /* Set the PHY transition */
    g_ble_phy_data.phy_transition = end_trans;

    /* Set transmitted payload length */
    g_ble_phy_data.phy_tx_pyld_len = payload_len;

    /* If we already started transmitting, abort it! */
    state = NRF_RADIO->STATE;
    if (state != RADIO_STATE_STATE_Tx) {

        /* Set phy state to transmitting and count packet statistics */
        g_ble_phy_data.phy_state = BLE_PHY_STATE_TX;
        STATS_INC(ble_phy_stats, tx_good);
        STATS_INCN(ble_phy_stats, tx_bytes, payload_len + BLE_LL_PDU_HDR_LEN);
        rc = BLE_ERR_SUCCESS;
    } else {
        ble_phy_disable();
        STATS_INC(ble_phy_stats, tx_late);
        rc = BLE_PHY_ERR_RADIO_STATE;
    }

    return rc;
}

/**
 * ble phy txpwr set
 *
 * Set the transmit output power (in dBm).
 *
 * NOTE: If the output power specified is within the BLE limits but outside
 * the chip limits, we "rail" the power level so we dont exceed the min/max
 * chip values.
 *
 * @param dbm Power output in dBm.
 *
 * @return int 0: success; anything else is an error
 */
int
ble_phy_txpwr_set(int dbm)
{
    /* Check valid range */
    assert(dbm <= BLE_PHY_MAX_PWR_DBM);

    /* "Rail" power level if outside supported range */
    if (dbm > NRF_TX_PWR_MAX_DBM) {
        dbm = NRF_TX_PWR_MAX_DBM;
    } else {
        if (dbm < NRF_TX_PWR_MIN_DBM) {
            dbm = NRF_TX_PWR_MIN_DBM;
        }
    }

    NRF_RADIO->TXPOWER = dbm;
    g_ble_phy_data.phy_txpwr_dbm = dbm;

    return 0;
}

/**
 * ble phy txpwr round
 *
 * Get the rounded transmit output power (in dBm).
 *
 * @param dbm Power output in dBm.
 *
 * @return int Rounded power in dBm
 */
int ble_phy_txpower_round(int dbm)
{
    /* "Rail" power level if outside supported range */
    if (dbm > NRF_TX_PWR_MAX_DBM) {
        dbm = NRF_TX_PWR_MAX_DBM;
    } else {
        if (dbm < NRF_TX_PWR_MIN_DBM) {
            dbm = NRF_TX_PWR_MIN_DBM;
        }
    }

    return dbm;
}

/**
 * ble phy txpwr get
 *
 * Get the transmit power.
 *
 * @return int  The current PHY transmit power, in dBm
 */
int
ble_phy_txpwr_get(void)
{
    return g_ble_phy_data.phy_txpwr_dbm;
}

void
ble_phy_set_rx_pwr_compensation(int8_t compensation)
{
    g_ble_phy_data.rx_pwr_compensation = compensation;
}

/**
 * ble phy setchan
 *
 * Sets the logical frequency of the transceiver. The input parameter is the
 * BLE channel index (0 to 39, inclusive). The NRF frequency register works like
 * this: logical frequency = 2400 + FREQ (MHz).
 *
 * Thus, to get a logical frequency of 2402 MHz, you would program the
 * FREQUENCY register to 2.
 *
 * @param chan This is the Data Channel Index or Advertising Channel index
 *
 * @return int 0: success; PHY error code otherwise
 */
int
ble_phy_setchan(uint8_t chan, uint32_t access_addr, uint32_t crcinit)
{
    uint32_t prefix;

    assert(chan < BLE_PHY_NUM_CHANS);

    /* Check for valid channel range */
    if (chan >= BLE_PHY_NUM_CHANS) {
        return BLE_PHY_ERR_INV_PARAM;
    }

    /* Get correct frequency */
    if (chan < BLE_PHY_NUM_DATA_CHANS) {
        /* Set current access address */
        g_ble_phy_data.phy_access_address = access_addr;

        /* Configure logical address 1 and crcinit */
        prefix = NRF_RADIO->PREFIX0;
        prefix &= 0xffff00ff;
        prefix |= ((access_addr >> 24) & 0xFF) << 8;
        NRF_RADIO->BASE1 = (access_addr << 8) & 0xFFFFFF00;
        NRF_RADIO->PREFIX0 = prefix;
        NRF_RADIO->TXADDRESS = 1;
        NRF_RADIO->RXADDRESSES = (1 << 1);
        NRF_RADIO->CRCINIT = crcinit;
    } else {
        /* Logical adddress 0 preconfigured */
        NRF_RADIO->TXADDRESS = 0;
        NRF_RADIO->RXADDRESSES = (1 << 0);
        NRF_RADIO->CRCINIT = BLE_LL_CRCINIT_ADV;

        /* Set current access address */
        g_ble_phy_data.phy_access_address = BLE_ACCESS_ADDR_ADV;
    }

    /* Set the frequency and the data whitening initial value */
    g_ble_phy_data.phy_chan = chan;
    NRF_RADIO->FREQUENCY = g_ble_phy_chan_freq[chan];
    NRF_RADIO->DATAWHITEIV = chan;

    return 0;
}

/**
 * Stop the timer used to count microseconds when using RTC for cputime
 */
static void
ble_phy_stop_usec_timer(void)
{
    NRF_TIMER0->TASKS_STOP = 1;
    NRF_TIMER0->TASKS_SHUTDOWN = 1;
    NRF_RTC0->EVTENCLR = RTC_EVTENSET_COMPARE0_Msk;
}

/**
 * ble phy disable irq and ppi
 *
 * This routine is to be called when reception was stopped due to either a
 * wait for response timeout or a packet being received and the phy is to be
 * restarted in receive mode. Generally, the disable routine is called to stop
 * the phy.
 */
static void
ble_phy_disable_irq_and_ppi(void)
{
    NRF_RADIO->INTENCLR = NRF_RADIO_IRQ_MASK_ALL;
    NRF_RADIO->SHORTS = 0;
    NRF_RADIO->TASKS_DISABLE = 1;
    NRF_PPI->CHENCLR = PPI_CHEN_CH4_Msk | PPI_CHEN_CH5_Msk | PPI_CHEN_CH20_Msk |
          PPI_CHEN_CH21_Msk | PPI_CHEN_CH23_Msk | PPI_CHEN_CH24_Msk |
          PPI_CHEN_CH25_Msk | PPI_CHEN_CH31_Msk;
    NVIC_ClearPendingIRQ(RADIO_IRQn);
    g_ble_phy_data.phy_state = BLE_PHY_STATE_IDLE;
}

void
ble_phy_restart_rx(void)
{
    ble_phy_stop_usec_timer();
    ble_phy_disable_irq_and_ppi();
    ble_phy_rx();
}

/**
 * ble phy disable
 *
 * Disables the PHY. This should be called when an event is over. It stops
 * the usec timer (if used), disables interrupts, disables the RADIO, disables
 * PPI and sets state to idle.
 */
void
ble_phy_disable(void)
{
    ble_phy_trace_void(BLE_PHY_TRACE_ID_DISABLE);

    ble_phy_stop_usec_timer();
    ble_phy_disable_irq_and_ppi();
}

/* Gets the current access address */
uint32_t ble_phy_access_addr_get(void)
{
    return g_ble_phy_data.phy_access_address;
}

/**
 * Return the phy state
 *
 * @return int The current PHY state.
 */
int
ble_phy_state_get(void)
{
    return g_ble_phy_data.phy_state;
}

/**
 * Called to see if a reception has started
 *
 * @return int
 */
int
ble_phy_rx_started(void)
{
    return g_ble_phy_data.phy_rx_started;
}

/**
 * Return the transceiver state
 *
 * @return int transceiver state.
 */
uint8_t
ble_phy_xcvr_state_get(void)
{
    uint32_t state;
    state = NRF_RADIO->STATE;
    return (uint8_t)state;
}

/**
 * Called to return the maximum data pdu payload length supported by the
 * phy. For this chip, if encryption is enabled, the maximum payload is 27
 * bytes.
 *
 * @return uint8_t Maximum data channel PDU payload size supported
 */
uint8_t
ble_phy_max_data_pdu_pyld(void)
{
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
    return NRF_MAX_ENCRYPTED_PYLD_LEN;
#else
    return BLE_LL_DATA_PDU_MAX_PYLD;
#endif
}

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
void
ble_phy_resolv_list_enable(void)
{
    NRF_AAR->NIRK = (uint32_t)g_nrf_num_irks;
    g_ble_phy_data.phy_privacy = 1;
}

void
ble_phy_resolv_list_disable(void)
{
    g_ble_phy_data.phy_privacy = 0;
}
#endif

void
ble_phy_rfclk_enable(void)
{
#if MYNEWT
    nrf51_clock_hfxo_request();
#else
    NRF_CLOCK->TASKS_HFCLKSTART = 1;
#endif
}

void
ble_phy_rfclk_disable(void)
{
#if MYNEWT
    nrf51_clock_hfxo_release();
#else
    NRF_CLOCK->TASKS_HFCLKSTOP = 1;
#endif
}
#endif
