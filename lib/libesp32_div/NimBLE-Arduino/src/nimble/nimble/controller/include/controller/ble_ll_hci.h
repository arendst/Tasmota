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

#ifndef H_BLE_LL_HCI_
#define H_BLE_LL_HCI_

#ifdef __cplusplus
extern "C" {
#endif

#include "nimble/nimble/include/nimble/hci_common.h"

/* For supported commands */
#define BLE_LL_SUPP_CMD_LEN (45)
extern const uint8_t g_ble_ll_supp_cmds[BLE_LL_SUPP_CMD_LEN];

/* The largest event the controller will send. */
#define BLE_LL_MAX_EVT_LEN  MYNEWT_VAL(BLE_HCI_EVT_BUF_SIZE)

/*
 * This determines the number of outstanding commands allowed from the
 * host to the controller. NOTE: you cannot change this without modifying
 * other portions of the code as we currently use a global os event for
 * the command; you would need to allocate a pool of these.
 */
#define BLE_LL_CFG_NUM_HCI_CMD_PKTS     (1)

typedef void (*ble_ll_hci_post_cmd_complete_cb)(void);

/* Initialize LL HCI */
void ble_ll_hci_init(void);

/* Used to determine if the LE event is enabled/disabled */
bool ble_ll_hci_is_le_event_enabled(unsigned int subev);

/* Used to determine if event is enabled/disabled */
bool ble_ll_hci_is_event_enabled(unsigned int evcode);

/* Send event from controller to host */
int ble_ll_hci_event_send(struct ble_hci_ev *hci_ev);

/* Sends a command complete with a no-op opcode to host */
void ble_ll_hci_send_noop(void);

/* Checks the preferref phy masks from set default phy and set phy commands */
int ble_ll_hci_chk_phy_masks(uint8_t all_phys, uint8_t tx_phys, uint8_t rx_phys,
                             uint8_t *txphy, uint8_t *rxphy);

/* Returns true if Extended Advertising HCI commands are in use */
bool ble_ll_hci_adv_mode_ext(void);

/* Get TX power compensation rounded to integer dB */
int8_t ble_ll_get_tx_pwr_compensation(void);

#ifdef __cplusplus
}
#endif

#endif /* H_BLE_LL_HCI_ */
