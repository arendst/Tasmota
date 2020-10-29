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

#ifndef H_BLE_HS_
#define H_BLE_HS_

/**
 * @brief Bluetooth Host
 * @defgroup bt_host Bluetooth Host
 * @{
 */

#include <inttypes.h>
#include "nimble/hci_common.h"
#include "host/ble_att.h"
#include "host/ble_eddystone.h"
#include "host/ble_gap.h"
#include "host/ble_gatt.h"
#include "host/ble_hs_adv.h"
#include "host/ble_hs_id.h"
#include "host/ble_hs_hci.h"
#include "host/ble_hs_log.h"
#include "host/ble_hs_mbuf.h"
#include "host/ble_hs_stop.h"
#include "host/ble_ibeacon.h"
#include "host/ble_l2cap.h"
#include "host/ble_sm.h"
#include "host/ble_store.h"
#include "host/ble_uuid.h"
#include "nimble/nimble_npl.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BLE_HS_FOREVER              INT32_MAX

/** Connection handle not present */
#define BLE_HS_CONN_HANDLE_NONE     0xffff

/**
 * @brief Bluetooth Host Error Code
 * @defgroup bt_host_err Bluetooth Host Error Code
 *
 * Defines error codes returned by Bluetooth host. If error comes from specific
 * component (eg L2CAP or Security Manager) it is shifted by base allowing to
 * identify component.
 * @{
 */

#define BLE_HS_EAGAIN               1
#define BLE_HS_EALREADY             2
#define BLE_HS_EINVAL               3
#define BLE_HS_EMSGSIZE             4
#define BLE_HS_ENOENT               5
#define BLE_HS_ENOMEM               6
#define BLE_HS_ENOTCONN             7
#define BLE_HS_ENOTSUP              8
#define BLE_HS_EAPP                 9
#define BLE_HS_EBADDATA             10
#define BLE_HS_EOS                  11
#define BLE_HS_ECONTROLLER          12
#define BLE_HS_ETIMEOUT             13
#define BLE_HS_EDONE                14
#define BLE_HS_EBUSY                15
#define BLE_HS_EREJECT              16
#define BLE_HS_EUNKNOWN             17
#define BLE_HS_EROLE                18
#define BLE_HS_ETIMEOUT_HCI         19
#define BLE_HS_ENOMEM_EVT           20
#define BLE_HS_ENOADDR              21
#define BLE_HS_ENOTSYNCED           22
#define BLE_HS_EAUTHEN              23
#define BLE_HS_EAUTHOR              24
#define BLE_HS_EENCRYPT             25
#define BLE_HS_EENCRYPT_KEY_SZ      26
#define BLE_HS_ESTORE_CAP           27
#define BLE_HS_ESTORE_FAIL          28
#define BLE_HS_EPREEMPTED           29
#define BLE_HS_EDISABLED            30
#define BLE_HS_ESTALLED             31

/** Error base for ATT errors */
#define BLE_HS_ERR_ATT_BASE         0x100

/** Converts error to ATT base */
#define BLE_HS_ATT_ERR(x)           ((x) ? BLE_HS_ERR_ATT_BASE + (x) : 0)

/** Error base for HCI errors */
#define BLE_HS_ERR_HCI_BASE         0x200

/** Converts error to HCI base */
#define BLE_HS_HCI_ERR(x)           ((x) ? BLE_HS_ERR_HCI_BASE + (x) : 0)

/** Error base for L2CAP errors */
#define BLE_HS_ERR_L2C_BASE         0x300

/** Converts error to L2CAP base */
#define BLE_HS_L2C_ERR(x)           ((x) ? BLE_HS_ERR_L2C_BASE + (x) : 0)

/** Error base for local Security Manager errors */
#define BLE_HS_ERR_SM_US_BASE       0x400

/** Converts error to local Security Manager base */
#define BLE_HS_SM_US_ERR(x)         ((x) ? BLE_HS_ERR_SM_US_BASE + (x) : 0)

/** Error base for remote (peer) Security Manager errors */
#define BLE_HS_ERR_SM_PEER_BASE     0x500

/** Converts error to remote (peer) Security Manager base */
#define BLE_HS_SM_PEER_ERR(x)       ((x) ? BLE_HS_ERR_SM_PEER_BASE + (x) : 0)

/** Error base for hardware errors */
#define BLE_HS_ERR_HW_BASE          0x600

/** Converts error to hardware error  base */
#define BLE_HS_HW_ERR(x)            (BLE_HS_ERR_HW_BASE + (x))

/**
 * @}
 */

/**
 * @brief Bluetooth Host Configuration
 * @defgroup bt_host_conf Bluetooth Host Configuration
 *
 * @{
 */

/**
 * @brief Local Input-Output capabilities of device
 * @defgroup bt_host_io_local Local Input-Output capabilities of device
 *
 * @{
 */

/** DisplayOnly IO capability */
#define BLE_HS_IO_DISPLAY_ONLY              0x00

/** DisplayYesNo IO capability */
#define BLE_HS_IO_DISPLAY_YESNO             0x01

/** KeyboardOnly IO capability */
#define BLE_HS_IO_KEYBOARD_ONLY             0x02

/** NoInputNoOutput IO capability */
#define BLE_HS_IO_NO_INPUT_OUTPUT           0x03

/** KeyboardDisplay Only IO capability */
#define BLE_HS_IO_KEYBOARD_DISPLAY          0x04

/**
 * @}
 */

/** @brief Stack reset callback
 *
 * @param reason Reason code for reset
 */
typedef void ble_hs_reset_fn(int reason);


/** @brief Stack sync callback */
typedef void ble_hs_sync_fn(void);

/** @brief Bluetooth Host main configuration structure
 *
 * Those can be used by application to configure stack.
 *
 * The only reason Security Manager (sm_ members) is configurable at runtime is
 * to simplify security testing. Defaults for those are configured by selecting
 * proper options in application's syscfg.
 */
struct ble_hs_cfg {
    /**
     * An optional callback that gets executed upon registration of each GATT
     * resource (service, characteristic, or descriptor).
     */
    ble_gatt_register_fn *gatts_register_cb;

    /**
     * An optional argument that gets passed to the GATT registration
     * callback.
     */
    void *gatts_register_arg;

    /** Security Manager Local Input Output Capabilities */
    uint8_t sm_io_cap;

    /** @brief Security Manager OOB flag
     *
     * If set proper flag in Pairing Request/Response will be set.
     */
    unsigned sm_oob_data_flag:1;

    /** @brief Security Manager Bond flag
     *
     * If set proper flag in Pairing Request/Response will be set. This results
     * in storing keys distributed during bonding.
     */
    unsigned sm_bonding:1;

    /** @brief Security Manager MITM flag
     *
     * If set proper flag in Pairing Request/Response will be set. This results
     * in requiring Man-In-The-Middle protection when pairing.
     */
    unsigned sm_mitm:1;

    /** @brief Security Manager Secure Connections flag
     *
     * If set proper flag in Pairing Request/Response will be set. This results
     * in using LE Secure Connections for pairing if also supported by remote
     * device. Fallback to legacy pairing if not supported by remote.
     */
    unsigned sm_sc:1;

    /** @brief Security Manager Key Press Notification flag
     *
     * Currently unsupported and should not be set.
     */
    unsigned sm_keypress:1;

    /** @brief Security Manager Local Key Distribution Mask */
    uint8_t sm_our_key_dist;

    /** @brief Security Manager Remote Key Distribution Mask */
    uint8_t sm_their_key_dist;

    /** @brief Stack reset callback
     *
     * This callback is executed when the host resets itself and the controller
     * due to fatal error.
     */
    ble_hs_reset_fn *reset_cb;

    /** @brief Stack sync callback
     *
     * This callback is executed when the host and controller become synced.
     * This happens at startup and after a reset.
     */
    ble_hs_sync_fn *sync_cb;

    /* XXX: These need to go away. Instead, the nimble host package should
     * require the host-store API (not yet implemented)..
     */
    /** Storage Read callback handles read of security material */
    ble_store_read_fn *store_read_cb;

    /** Storage Write callback handles write of security material */
    ble_store_write_fn *store_write_cb;

    /** Storage Delete callback handles deletion of security material */
    ble_store_delete_fn *store_delete_cb;

    /** @brief Storage Status callback.
     *
     * This callback gets executed when a persistence operation cannot be
     * performed or a persistence failure is imminent. For example, if is
     * insufficient storage capacity for a record to be persisted, this
     * function gets called to give the application the opportunity to make
     * room.
     */
    ble_store_status_fn *store_status_cb;

    /** An optional argument that gets passed to the storage status callback. */
    void *store_status_arg;
};

extern struct ble_hs_cfg ble_hs_cfg;

/**
 * @}
 */

/**
 * @brief Indicates whether the host is enabled.  The host is enabled if it is
 * starting or fully started.  It is disabled if it is stopping or stopped.
 *
 * @return 1 if the host is enabled;
 *         0 if the host is disabled.
 */
int ble_hs_is_enabled(void);

/**
 * Indicates whether the host has synchronized with the controller.
 * Synchronization must occur before any host procedures can be performed.
 *
 * @return 1 if the host and controller are in sync;
 *         0 if the host and controller are out of sync.
 */
int ble_hs_synced(void);

/**
 * Synchronizes the host with the controller by sending a sequence of HCI
 * commands.  This function must be called before any other host functionality
 * is used, but it must be called after both the host and controller are
 * initialized.  Typically, the host-parent-task calls this function at the top
 * of its task routine.  This function must only be called in the host parent
 * task.  A safe alternative for starting the stack from any task is to call
 * `ble_hs_sched_start()`.
 *
 * If the host fails to synchronize with the controller (if the controller is
 * not fully booted, for example), the host will attempt to resynchronize every
 * 100 ms. For this reason, an error return code is not necessarily fatal.
 *
 * @return 0 on success; nonzero on error.
 */
int ble_hs_start(void);

/**
 * Enqueues a host start event to the default event queue.  The actual host
 * startup is performed in the host parent task, but using the default queue
 * here ensures the event won't run until the end of main() when this is
 * called during system initialization.  This allows the application to
 * configure the host package in the meantime.
 *
 * If auto-start is disabled, the application should use this function to start
 * the BLE stack.  This function can be called at any time as long as the host
 * is stopped.  When the host successfully starts, the application is notified
 * via the ble_hs_cfg.sync_cb callback.
 */
void ble_hs_sched_start(void);

/**
 * Causes the host to reset the NimBLE stack as soon as possible.  The
 * application is notified when the reset occurs via the host reset callback.
 *
 * @param reason The host error code that gets passed to the reset callback.
 */
void ble_hs_sched_reset(int reason);

/**
 * Designates the specified event queue for NimBLE host work. By default, the
 * host uses the default event queue and runs in the main task. This function
 * is useful if you want the host to run in a different task.
 *
 * @param evq The event queue to use for host work.
 */
void ble_hs_evq_set(struct ble_npl_eventq *evq);

/**
 * Initializes the NimBLE host. This function must be called before the OS is
 * started. The NimBLE stack requires an application task to function.  One
 * application task in particular is designated as the "host parent task". In
 * addition to application-specific work, the host parent task does work for
 * NimBLE by processing events generated by the host.
 */
void ble_hs_init(void);

/**
 * Deinitializes the NimBLE host. This function must be called after the
 * NimBLE host stop procedure is complete.
 */
void ble_hs_deinit(void);

/**
 * @brief Called when the system is shutting down.  Stops the BLE host.
 *
 * @param reason                The reason for the shutdown.  One of the
 *                                  HAL_RESET_[...] codes or an
 *                                  implementation-defined value.
 *
 * @return                      SYSDOWN_IN_PROGRESS. 
 */
int ble_hs_shutdown(int reason);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif
