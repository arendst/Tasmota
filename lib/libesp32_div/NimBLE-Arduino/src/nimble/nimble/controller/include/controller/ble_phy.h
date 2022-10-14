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

#ifndef H_BLE_PHY_
#define H_BLE_PHY_

#include "nimble/nimble/include/nimble/hci_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct os_mbuf;

/* Channel/Frequency defintions */
#define BLE_PHY_NUM_CHANS           (40)
#define BLE_PHY_NUM_DATA_CHANS      (37)
#define BLE_PHY_CHAN0_FREQ_MHZ      (2402)
#define BLE_PHY_DATA_CHAN0_FREQ_MHZ (2404)
#define BLE_PHY_CHAN_SPACING_MHZ    (2)
#define BLE_PHY_NUM_ADV_CHANS       (3)
#define BLE_PHY_ADV_CHAN_START      (37)

/* Power */
#define BLE_PHY_MAX_PWR_DBM         (10)

/* Deviation */
#define BLE_PHY_DEV_KHZ             (185)
#define BLE_PHY_BINARY_ZERO         (-BLE_PHY_DEV)
#define BLE_PHY_BINARY_ONE          (BLE_PHY_DEV)

/* Max. clock drift */
#define BLE_PHY_MAX_DRIFT_PPM       (50)

/* Data rate */
#define BLE_PHY_BIT_RATE_BPS        (1000000)

/* Macros */
#define BLE_IS_ADV_CHAN(chan)       (chan >= BLE_PHY_ADV_CHAN_START)
#define BLE_IS_DATA_CHAN(chan)      (chan < BLE_PHY_ADV_CHAN_START)

/* PHY states */
#define BLE_PHY_STATE_IDLE          (0)
#define BLE_PHY_STATE_RX            (1)
#define BLE_PHY_STATE_TX            (2)

/* BLE PHY transitions */
#define BLE_PHY_TRANSITION_NONE     (0)
#define BLE_PHY_TRANSITION_RX_TX    (1)
#define BLE_PHY_TRANSITION_TX_RX    (2)

/* PHY error codes */
#define BLE_PHY_ERR_RADIO_STATE     (1)
#define BLE_PHY_ERR_INIT            (2)
#define BLE_PHY_ERR_INV_PARAM       (3)
#define BLE_PHY_ERR_NO_BUFS         (4)
#define BLE_PHY_ERR_TX_LATE         (5)
#define BLE_PHY_ERR_RX_LATE         (6)

/* Maximun PDU length. Includes LL header of 2 bytes and 255 bytes payload. */
#define BLE_PHY_MAX_PDU_LEN         (257)

/* Wait for response timer */
typedef void (*ble_phy_tx_end_func)(void *arg);

/* Initialize the PHY */
int ble_phy_init(void);

/* Reset the PHY */
int ble_phy_reset(void);

/* Set the PHY channel */
int ble_phy_setchan(uint8_t chan, uint32_t access_addr, uint32_t crcinit);

/* Set transmit start time */
int ble_phy_tx_set_start_time(uint32_t cputime, uint8_t rem_usecs);

/* Set receive start time */
int ble_phy_rx_set_start_time(uint32_t cputime, uint8_t rem_usecs);

/* Set the transmit end callback and argument */
void ble_phy_set_txend_cb(ble_phy_tx_end_func txend_cb, void *arg);

typedef uint8_t (*ble_phy_tx_pducb_t)(uint8_t *dptr, void *pducb_arg,
                                      uint8_t *hdr_byte);

/* Place the PHY into transmit mode */
int ble_phy_tx(ble_phy_tx_pducb_t pducb, void *pducb_arg, uint8_t end_trans);

/* Place the PHY into receive mode */
int ble_phy_rx(void);

/* Copies the received PHY buffer into the allocated pdu */
void ble_phy_rxpdu_copy(uint8_t *dptr, struct os_mbuf *rxpdu);

/* Set the transmit power */
int ble_phy_txpwr_set(int dbm);

/* Get highest allowed power from range */
int ble_phy_txpower_round(int dbm);

/* Get the transmit power */
int ble_phy_txpwr_get(void);

/* Set RX path power compensation value rounded to integer dB */
void ble_phy_set_rx_pwr_compensation(int8_t compensation);

/* Disable the PHY */
void ble_phy_disable(void);

#define BLE_PHY_WFR_ENABLE_RX       (0)
#define BLE_PHY_WFR_ENABLE_TXRX     (1)

void ble_phy_wfr_enable(int txrx, uint8_t tx_phy_mode, uint32_t wfr_usecs);

/* Starts rf clock */
void ble_phy_rfclk_enable(void);

/* Stops rf clock */
void ble_phy_rfclk_disable(void);

/*
 * Used to restart reception on same channel after wfr timer expiration or
 * frame received.
 */
void ble_phy_restart_rx(void);

/* Gets the current state of the PHY */
int ble_phy_state_get(void);

/* Gets current state of transceiver */
uint8_t ble_phy_xcvr_state_get(void);

/* Returns 'true' if a reception has started */
int ble_phy_rx_started(void);

/*
 * Returns the maximum supported tx/rx PDU payload size, in bytes, for data
 * channel PDUs (this does not apply to advertising channel PDUs). Note
 * that the data channel PDU is composed of a 2-byte header, the payload, and
 * an optional MIC. The maximum payload is 251 bytes.
 */
uint8_t ble_phy_max_data_pdu_pyld(void);

/* Gets the current access address */
uint32_t ble_phy_access_addr_get(void);

/* Enable encryption */
void ble_phy_encrypt_enable(uint64_t pkt_counter, uint8_t *iv, uint8_t *key,
                            uint8_t is_master);

/* Disable encryption */
void ble_phy_encrypt_disable(void);

/* Set the packet counters and dir used by LE encyption */
void ble_phy_encrypt_set_pkt_cntr(uint64_t pkt_counter, int dir);

/* Enable phy resolving list */
void ble_phy_resolv_list_enable(void);

/* Disable phy resolving list */
void ble_phy_resolv_list_disable(void);

/*
 * PHY mode values for 1M, 2M and Coded S=8 are the same as corresponding values
 * of PHY. This makes conversion between 'phy' and 'phy_mode' easier and it also
 * means that default coding for Coded will be S=8, unless explicitly translated
 * to S=2.
 */
#define BLE_PHY_MODE_CODED_500KBPS  (0)
#define BLE_PHY_MODE_1M             (1)
#define BLE_PHY_MODE_2M             (2)
#define BLE_PHY_MODE_CODED_125KBPS  (3)

/* The number of different modes */
#define BLE_PHY_NUM_MODE            (4)

/* PHY numbers (compatible with HCI) */
#define BLE_PHY_1M                  (BLE_HCI_LE_PHY_1M)
#define BLE_PHY_2M                  (BLE_HCI_LE_PHY_2M)
#define BLE_PHY_CODED               (BLE_HCI_LE_PHY_CODED)

/* PHY bitmasks (compatible with HCI) */
#define BLE_PHY_MASK_1M             (BLE_HCI_LE_PHY_1M_PREF_MASK)
#define BLE_PHY_MASK_2M             (BLE_HCI_LE_PHY_2M_PREF_MASK)
#define BLE_PHY_MASK_CODED          (BLE_HCI_LE_PHY_CODED_PREF_MASK)

#if (MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_2M_PHY) || MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CODED_PHY))
uint32_t ble_phy_mode_pdu_start_off(int phy);
void ble_phy_mode_set(uint8_t tx_phy_mode, uint8_t rx_phy_mode);
#else
#define ble_phy_mode_pdu_start_off(phy)     (40)

#endif

int ble_phy_get_cur_phy(void);
static inline int ble_ll_phy_to_phy_mode(int phy, int phy_options)
{
    int phy_mode;

    /*
     * 'phy' value can be used as 'phy_mode' value unless S=2 coding is explicitly
     * required. By default we'll use S=2 for Coded.
     */
    phy_mode = phy;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CODED_PHY)
    if (phy == BLE_PHY_CODED && phy_options == BLE_HCI_LE_PHY_CODED_S2_PREF) {
        phy_mode = BLE_PHY_MODE_CODED_500KBPS;
    }
#else
    (void)phy_options;
#endif

    return phy_mode;
}

#if MYNEWT_VAL(BLE_LL_DTM)
void ble_phy_enable_dtm(void);
void ble_phy_disable_dtm(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* H_BLE_PHY_ */
