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

#ifndef H_HCI_TRANSPORT_
#define H_HCI_TRANSPORT_

#include <inttypes.h>
#include "nimble/porting/nimble/include/os/os_mempool.h"

#ifdef __cplusplus
extern "C" {
#endif

struct os_mbuf;

#define BLE_HCI_TRANS_CMD_SZ        260

/*** Type of buffers for holding commands and events. */
/**
 * Controller-to-host event buffers.  Events have one of two priorities:
 * o Low-priority   (BLE_HCI_TRANS_BUF_EVT_LO)
 * o High-priority  (BLE_HCI_TRANS_BUF_EVT_HI)
 *
 * Low-priority event buffers are only used for advertising reports.  If there
 * are no free low-priority event buffers, then an incoming advertising report
 * will get dropped.
 *
 * High-priority event buffers are for everything except advertising reports.
 * If there are no free high-priority event buffers, a request to allocate one
 * will try to allocate a low-priority buffer instead.
 *
 * If you want all events to be given equal treatment, then you should allocate
 * low-priority events only.
 *
 * Event priorities solve the problem of critical events getting dropped due to
 * a flood of advertising reports.  This solution is likely temporary: when
 * HCI flow control is added, event priorities may become obsolete.
 *
 * Not all transports distinguish between low and high priority events.  If the
 * transport does not have separate settings for low and high buffer counts,
 * then it treats all events with equal priority.
 */
#define BLE_HCI_TRANS_BUF_EVT_LO    1
#define BLE_HCI_TRANS_BUF_EVT_HI    2

/* Host-to-controller command. */
#define BLE_HCI_TRANS_BUF_CMD       3

/** Callback function types; executed when HCI packets are received. */
typedef int ble_hci_trans_rx_cmd_fn(uint8_t *cmd, void *arg);
typedef int ble_hci_trans_rx_acl_fn(struct os_mbuf *om, void *arg);

/**
 * Sends an HCI event from the controller to the host.
 *
 * @param cmd                   The HCI event to send.  This buffer must be
 *                                  allocated via ble_hci_trans_buf_alloc().
 *
 * @return                      0 on success;
 *                              A BLE_ERR_[...] error code on failure.
 */
int ble_hci_trans_ll_evt_tx(uint8_t *hci_ev);

/**
 * Sends ACL data from controller to host.
 *
 * @param om                    The ACL data packet to send.
 *
 * @return                      0 on success;
 *                              A BLE_ERR_[...] error code on failure.
 */
int ble_hci_trans_ll_acl_tx(struct os_mbuf *om);

/**
 * Sends an HCI command from the host to the controller.
 *
 * @param cmd                   The HCI command to send.  This buffer must be
 *                                  allocated via ble_hci_trans_buf_alloc().
 *
 * @return                      0 on success;
 *                              A BLE_ERR_[...] error code on failure.
 */
int ble_hci_trans_hs_cmd_tx(uint8_t *cmd);

/**
 * Sends ACL data from host to controller.
 *
 * @param om                    The ACL data packet to send.
 *
 * @return                      0 on success;
 *                              A BLE_ERR_[...] error code on failure.
 */
int ble_hci_trans_hs_acl_tx(struct os_mbuf *om);

/**
 * Allocates a flat buffer of the specified type.
 *
 * @param type                  The type of buffer to allocate; one of the
 *                                  BLE_HCI_TRANS_BUF_[...] constants.
 *
 * @return                      The allocated buffer on success;
 *                              NULL on buffer exhaustion.
 */
uint8_t *ble_hci_trans_buf_alloc(int type);

/**
 * Frees the specified flat buffer.  The buffer must have been allocated via
 * ble_hci_trans_buf_alloc().
 *
 * @param buf                   The buffer to free.
 */
void ble_hci_trans_buf_free(uint8_t *buf);

/**
 * Configures a callback to get executed whenever an ACL data packet is freed.
 * The function is called immediately before the free occurs.
 *
 * @param cb                    The callback to configure.
 * @param arg                   An optional argument to pass to the callback.
 *
 * @return                      0 on success;
 *                              BLE_ERR_UNSUPPORTED if the transport does not
 *                                  support this operation.
 */
int ble_hci_trans_set_acl_free_cb(os_mempool_put_fn *cb, void *arg);

/**
 * Configures the HCI transport to operate with a controller.  The transport
 * will execute specified callbacks upon receiving HCI packets from the host.
 *
 * @param cmd_cb                The callback to execute upon receiving an HCI
 *                                  command.
 * @param cmd_arg               Optional argument to pass to the command
 *                                  callback.
 * @param acl_cb                The callback to execute upon receiving ACL
 *                                  data.
 * @param acl_arg               Optional argument to pass to the ACL
 *                                  callback.
 */
void ble_hci_trans_cfg_ll(ble_hci_trans_rx_cmd_fn *cmd_cb,
                          void *cmd_arg,
                          ble_hci_trans_rx_acl_fn *acl_cb,
                          void *acl_arg);

/**
 * Configures the HCI transport to operate with a host.  The transport will
 * execute specified callbacks upon receiving HCI packets from the controller.
 *
 * @param evt_cb                The callback to execute upon receiving an HCI
 *                                  event.
 * @param evt_arg               Optional argument to pass to the event
 *                                  callback.
 * @param acl_cb                The callback to execute upon receiving ACL
 *                                  data.
 * @param acl_arg               Optional argument to pass to the ACL
 *                                  callback.
 */
void ble_hci_trans_cfg_hs(ble_hci_trans_rx_cmd_fn *evt_cb,
                          void *evt_arg,
                          ble_hci_trans_rx_acl_fn *acl_cb,
                          void *acl_arg);

/**
 * Resets the HCI module to a clean state.  Frees all buffers and reinitializes
 * the underlying transport.
 *
 * @return                      0 on success;
 *                              A BLE_ERR_[...] error code on failure.
 */
int ble_hci_trans_reset(void);

#ifdef __cplusplus
}
#endif

#endif /* H_HCI_TRANSPORT_ */
