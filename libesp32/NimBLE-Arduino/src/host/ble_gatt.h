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

#ifndef H_BLE_GATT_
#define H_BLE_GATT_

/**
 * @brief Bluetooth Generic Attribute Profile (GATT)
 * @defgroup bt_gatt Bluetooth Generic Attribute Profile (GATT)
 * @ingroup bt_host
 * @{
 */

#include <inttypes.h>
#include "host/ble_att.h"
#include "host/ble_uuid.h"
#ifdef __cplusplus
extern "C" {
#endif

struct ble_hs_conn;
struct ble_att_error_rsp;
struct ble_hs_cfg;

#define BLE_GATT_REGISTER_OP_SVC                        1
#define BLE_GATT_REGISTER_OP_CHR                        2
#define BLE_GATT_REGISTER_OP_DSC                        3

#define BLE_GATT_SVC_UUID16                             0x1801
#define BLE_GATT_DSC_CLT_CFG_UUID16                     0x2902

#define BLE_GATT_CHR_PROP_BROADCAST                     0x01
#define BLE_GATT_CHR_PROP_READ                          0x02
#define BLE_GATT_CHR_PROP_WRITE_NO_RSP                  0x04
#define BLE_GATT_CHR_PROP_WRITE                         0x08
#define BLE_GATT_CHR_PROP_NOTIFY                        0x10
#define BLE_GATT_CHR_PROP_INDICATE                      0x20
#define BLE_GATT_CHR_PROP_AUTH_SIGN_WRITE               0x40
#define BLE_GATT_CHR_PROP_EXTENDED                      0x80

#define BLE_GATT_ACCESS_OP_READ_CHR                     0
#define BLE_GATT_ACCESS_OP_WRITE_CHR                    1
#define BLE_GATT_ACCESS_OP_READ_DSC                     2
#define BLE_GATT_ACCESS_OP_WRITE_DSC                    3

#define BLE_GATT_CHR_F_BROADCAST                        0x0001
#define BLE_GATT_CHR_F_READ                             0x0002
#define BLE_GATT_CHR_F_WRITE_NO_RSP                     0x0004
#define BLE_GATT_CHR_F_WRITE                            0x0008
#define BLE_GATT_CHR_F_NOTIFY                           0x0010
#define BLE_GATT_CHR_F_INDICATE                         0x0020
#define BLE_GATT_CHR_F_AUTH_SIGN_WRITE                  0x0040
#define BLE_GATT_CHR_F_RELIABLE_WRITE                   0x0080
#define BLE_GATT_CHR_F_AUX_WRITE                        0x0100
#define BLE_GATT_CHR_F_READ_ENC                         0x0200
#define BLE_GATT_CHR_F_READ_AUTHEN                      0x0400
#define BLE_GATT_CHR_F_READ_AUTHOR                      0x0800
#define BLE_GATT_CHR_F_WRITE_ENC                        0x1000
#define BLE_GATT_CHR_F_WRITE_AUTHEN                     0x2000
#define BLE_GATT_CHR_F_WRITE_AUTHOR                     0x4000

#define BLE_GATT_SVC_TYPE_END                           0
#define BLE_GATT_SVC_TYPE_PRIMARY                       1
#define BLE_GATT_SVC_TYPE_SECONDARY                     2

/*** @client. */
struct ble_gatt_error {
    uint16_t status;
    uint16_t att_handle;
};

struct ble_gatt_svc {
    uint16_t start_handle;
    uint16_t end_handle;
    ble_uuid_any_t uuid;
};

struct ble_gatt_attr {
    uint16_t handle;
    uint16_t offset;
    struct os_mbuf *om;
};

struct ble_gatt_chr {
    uint16_t def_handle;
    uint16_t val_handle;
    uint8_t properties;
    ble_uuid_any_t uuid;
};

struct ble_gatt_dsc {
    uint16_t handle;
    ble_uuid_any_t uuid;
};

typedef int ble_gatt_mtu_fn(uint16_t conn_handle,
                            const struct ble_gatt_error *error,
                            uint16_t mtu, void *arg);
typedef int ble_gatt_disc_svc_fn(uint16_t conn_handle,
                                 const struct ble_gatt_error *error,
                                 const struct ble_gatt_svc *service,
                                 void *arg);

/**
 * The host will free the attribute mbuf automatically after the callback is
 * executed.  The application can take ownership of the mbuf and prevent it
 * from being freed by assigning NULL to attr->om.
 */
typedef int ble_gatt_attr_fn(uint16_t conn_handle,
                             const struct ble_gatt_error *error,
                             struct ble_gatt_attr *attr,
                             void *arg);

/**
 * The host will free the attribute mbufs automatically after the callback is
 * executed.  The application can take ownership of the mbufs and prevent them
 * from being freed by assigning NULL to each attribute's om field.
 */
typedef int ble_gatt_reliable_attr_fn(uint16_t conn_handle,
                                      const struct ble_gatt_error *error,
                                      struct ble_gatt_attr *attrs,
                                      uint8_t num_attrs, void *arg);

typedef int ble_gatt_chr_fn(uint16_t conn_handle,
                            const struct ble_gatt_error *error,
                            const struct ble_gatt_chr *chr, void *arg);

typedef int ble_gatt_dsc_fn(uint16_t conn_handle,
                            const struct ble_gatt_error *error,
                            uint16_t chr_val_handle,
                            const struct ble_gatt_dsc *dsc,
                            void *arg);

/**
 * Initiates GATT procedure: Exchange MTU.
 *
 * @param conn_handle           The connection over which to execute the
 *                                  procedure.
 * @param cb                    The function to call to report procedure status
 *                                  updates; null for no callback.
 * @param cb_arg                The optional argument to pass to the callback
 *                                  function.
 *
 * @return                      0 on success; nonzero on failure.
 */
int ble_gattc_exchange_mtu(uint16_t conn_handle,
                           ble_gatt_mtu_fn *cb, void *cb_arg);

/**
 * Initiates GATT procedure: Discover All Primary Services.
 *
 * @param conn_handle           The connection over which to execute the
 *                                  procedure.
 * @param cb                    The function to call to report procedure status
 *                                  updates; null for no callback.
 * @param cb_arg                The optional argument to pass to the callback
 *                                  function.
 */
int ble_gattc_disc_all_svcs(uint16_t conn_handle,
                            ble_gatt_disc_svc_fn *cb, void *cb_arg);

/**
 * Initiates GATT procedure: Discover Primary Service by Service UUID.
 *
 * @param conn_handle           The connection over which to execute the
 *                                  procedure.
 * @param service_uuid128       The 128-bit UUID of the service to discover.
 * @param cb                    The function to call to report procedure status
 *                                  updates; null for no callback.
 * @param cb_arg                The optional argument to pass to the callback
 *                                  function.
 *
 * @return                      0 on success; nonzero on failure.
 */
int ble_gattc_disc_svc_by_uuid(uint16_t conn_handle, const ble_uuid_t *uuid,
                               ble_gatt_disc_svc_fn *cb, void *cb_arg);

/**
 * Initiates GATT procedure: Find Included Services.
 *
 * @param conn_handle           The connection over which to execute the
 *                                  procedure.
 * @param start_handle          The handle to begin the search at (generally
 *                                  the service definition handle).
 * @param end_handle            The handle to end the search at (generally the
 *                                  last handle in the service).
 * @param cb                    The function to call to report procedure status
 *                                  updates; null for no callback.
 * @param cb_arg                The optional argument to pass to the callback
 *                                  function.
 *
 * @return                      0 on success; nonzero on failure.
 */
int ble_gattc_find_inc_svcs(uint16_t conn_handle, uint16_t start_handle,
                            uint16_t end_handle,
                            ble_gatt_disc_svc_fn *cb, void *cb_arg);

/**
 * Initiates GATT procedure: Discover All Characteristics of a Service.
 *
 * @param conn_handle           The connection over which to execute the
 *                                  procedure.
 * @param start_handle          The handle to begin the search at (generally
 *                                  the service definition handle).
 * @param end_handle            The handle to end the search at (generally the
 *                                  last handle in the service).
 * @param cb                    The function to call to report procedure status
 *                                  updates; null for no callback.
 * @param cb_arg                The optional argument to pass to the callback
 *                                  function.
 *
 * @return                      0 on success; nonzero on failure.
 */
int ble_gattc_disc_all_chrs(uint16_t conn_handle, uint16_t start_handle,
                            uint16_t end_handle, ble_gatt_chr_fn *cb,
                            void *cb_arg);

/**
 * Initiates GATT procedure: Discover Characteristics by UUID.
 *
 * @param conn_handle           The connection over which to execute the
 *                                  procedure.
 * @param start_handle          The handle to begin the search at (generally
 *                                  the service definition handle).
 * @param end_handle            The handle to end the search at (generally the
 *                                  last handle in the service).
 * @param chr_uuid128           The 128-bit UUID of the characteristic to
 *                                  discover.
 * @param cb                    The function to call to report procedure status
 *                                  updates; null for no callback.
 * @param cb_arg                The optional argument to pass to the callback
 *                                  function.
 *
 * @return                      0 on success; nonzero on failure.
 */
int ble_gattc_disc_chrs_by_uuid(uint16_t conn_handle, uint16_t start_handle,
                               uint16_t end_handle, const ble_uuid_t *uuid,
                               ble_gatt_chr_fn *cb, void *cb_arg);

/**
 * Initiates GATT procedure: Discover All Characteristic Descriptors.
 *
 * @param conn_handle           The connection over which to execute the
 *                                  procedure.
 * @param chr_val_handle        The handle of the characteristic value
 *                                  attribute.
 * @param chr_end_handle        The last handle in the characteristic
 *                                  definition.
 * @param cb                    The function to call to report procedure status
 *                                  updates; null for no callback.
 * @param cb_arg                The optional argument to pass to the callback
 *                                  function.
 *
 * @return                      0 on success; nonzero on failure.
 */
int ble_gattc_disc_all_dscs(uint16_t conn_handle, uint16_t start_handle,
                            uint16_t end_handle,
                            ble_gatt_dsc_fn *cb, void *cb_arg);

/**
 * Initiates GATT procedure: Read Characteristic Value.
 *
 * @param conn_handle           The connection over which to execute the
 *                                  procedure.
 * @param attr_handle           The handle of the characteristic value to read.
 * @param cb                    The function to call to report procedure status
 *                                  updates; null for no callback.
 * @param cb_arg                The optional argument to pass to the callback
 *                                  function.
 *
 * @return                      0 on success; nonzero on failure.
 */
int ble_gattc_read(uint16_t conn_handle, uint16_t attr_handle,
                   ble_gatt_attr_fn *cb, void *cb_arg);

/**
 * Initiates GATT procedure: Read Using Characteristic UUID.
 *
 * @param conn_handle           The connection over which to execute the
 *                                  procedure.
 * @param start_handle          The first handle to search (generally the
 *                                  handle of the service definition).
 * @param end_handle            The last handle to search (generally the
 *                                  last handle in the service definition).
 * @param cb                    The function to call to report procedure status
 *                                  updates; null for no callback.
 * @param cb_arg                The optional argument to pass to the callback
 *                                  function.
 *
 * @return                      0 on success; nonzero on failure.
 */
int ble_gattc_read_by_uuid(uint16_t conn_handle, uint16_t start_handle,
                           uint16_t end_handle, const ble_uuid_t *uuid,
                           ble_gatt_attr_fn *cb, void *cb_arg);

/**
 * Initiates GATT procedure: Read Long Characteristic Values.
 *
 * @param conn_handle           The connection over which to execute the
 *                                  procedure.
 * @param handle                The handle of the characteristic value to read.
 * @param cb                    The function to call to report procedure status
 *                                  updates; null for no callback.
 * @param cb_arg                The optional argument to pass to the callback
 *                                  function.
 *
 * @return                      0 on success; nonzero on failure.
 */
int ble_gattc_read_long(uint16_t conn_handle, uint16_t handle, uint16_t offset,
                        ble_gatt_attr_fn *cb, void *cb_arg);

/**
 * Initiates GATT procedure: Read Multiple Characteristic Values.
 *
 * @param conn_handle           The connection over which to execute the
 *                                  procedure.
 * @param handles               An array of 16-bit attribute handles to read.
 * @param num_handles           The number of entries in the "handles" array.
 * @param cb                    The function to call to report procedure status
 *                                  updates; null for no callback.
 * @param cb_arg                The optional argument to pass to the callback
 *                                  function.
 *
 * @return                      0 on success; nonzero on failure.
 */
int ble_gattc_read_mult(uint16_t conn_handle, const uint16_t *handles,
                        uint8_t num_handles, ble_gatt_attr_fn *cb,
                        void *cb_arg);

/**
 * Initiates GATT procedure: Write Without Response.  This function consumes
 * the supplied mbuf regardless of the outcome.
 *
 * @param conn_handle           The connection over which to execute the
 *                                  procedure.
 * @param attr_handle           The handle of the characteristic value to write
 *                                  to.
 * @param txom                  The value to write to the characteristic.
 *
 * @return                      0 on success; nonzero on failure.
 */
int ble_gattc_write_no_rsp(uint16_t conn_handle, uint16_t attr_handle,
                           struct os_mbuf *om);

/**
 * Initiates GATT procedure: Write Without Response.  This function consumes
 * the supplied mbuf regardless of the outcome.
 *
 * @param conn_handle           The connection over which to execute the
 *                                  procedure.
 * @param attr_handle           The handle of the characteristic value to write
 *                                  to.
 * @param value                 The value to write to the characteristic.
 * @param value_len             The number of bytes to write.
 *
 * @return                      0 on success; nonzero on failure.
 */
int ble_gattc_write_no_rsp_flat(uint16_t conn_handle, uint16_t attr_handle,
                                const void *data, uint16_t data_len);

/**
 * Initiates GATT procedure: Write Characteristic Value.  This function
 * consumes the supplied mbuf regardless of the outcome.
 *
 * @param conn_handle           The connection over which to execute the
 *                                  procedure.
 * @param attr_handle           The handle of the characteristic value to write
 *                                  to.
 * @param txom                  The value to write to the characteristic.
 * @param cb                    The function to call to report procedure status
 *                                  updates; null for no callback.
 * @param cb_arg                The optional argument to pass to the callback
 *                                  function.
 *
 * @return                      0 on success; nonzero on failure.
 */
int ble_gattc_write(uint16_t conn_handle, uint16_t attr_handle,
                    struct os_mbuf *om,
                    ble_gatt_attr_fn *cb, void *cb_arg);

/**
 * Initiates GATT procedure: Write Characteristic Value (flat buffer version).
 *
 * @param conn_handle           The connection over which to execute the
 *                                  procedure.
 * @param attr_handle           The handle of the characteristic value to write
 *                                  to.
 * @param value                 The value to write to the characteristic.
 * @param value_len             The number of bytes to write.
 * @param cb                    The function to call to report procedure status
 *                                  updates; null for no callback.
 * @param cb_arg                The optional argument to pass to the callback
 *                                  function.
 *
 * @return                      0 on success; nonzero on failure.
 */
int ble_gattc_write_flat(uint16_t conn_handle, uint16_t attr_handle,
                         const void *data, uint16_t data_len,
                         ble_gatt_attr_fn *cb, void *cb_arg);

/**
 * Initiates GATT procedure: Write Long Characteristic Values.  This function
 * consumes the supplied mbuf regardless of the outcome.
 *
 * @param conn_handle           The connection over which to execute the
 *                                  procedure.
 * @param attr_handle           The handle of the characteristic value to write
 *                                  to.
 * @param txom                  The value to write to the characteristic.
 * @param cb                    The function to call to report procedure status
 *                                  updates; null for no callback.
 * @param cb_arg                The optional argument to pass to the callback
 *                                  function.
 *
 * @return                      0 on success; nonzero on failure.
 */
int ble_gattc_write_long(uint16_t conn_handle, uint16_t attr_handle,
                         uint16_t offset, struct os_mbuf *om,
                         ble_gatt_attr_fn *cb, void *cb_arg);

/**
 * Initiates GATT procedure: Reliable Writes.  This function consumes the
 * supplied mbufs regardless of the outcome.
 *
 * @param conn_handle           The connection over which to execute the
 *                                  procedure.
 * @param attrs                 An array of attribute descriptors; specifies
 *                                  which characteristics to write to and what
 *                                  data to write to them.  The mbuf pointer in
 *                                  each attribute is set to NULL by this
 *                                  function.
 * @param num_attrs             The number of characteristics to write; equal
 *                                  to the number of elements in the 'attrs'
 *                                  array.
 * @param cb                    The function to call to report procedure status
 *                                  updates; null for no callback.
 * @param cb_arg                The optional argument to pass to the callback
 *                                  function.
 */
int ble_gattc_write_reliable(uint16_t conn_handle,
                             struct ble_gatt_attr *attrs,
                             int num_attrs, ble_gatt_reliable_attr_fn *cb,
                             void *cb_arg);

/**
 * Sends a "free-form" characteristic notification.  This function consumes the
 * supplied mbuf regardless of the outcome.
 *
 * @param conn_handle           The connection over which to execute the
 *                                  procedure.
 * @param chr_val_handle        The attribute handle to indicate in the
 *                                  outgoing notification.
 * @param txom                  The value to write to the characteristic.
 *
 * @return                      0 on success; nonzero on failure.
 */
int ble_gattc_notify_custom(uint16_t conn_handle, uint16_t att_handle,
                            struct os_mbuf *om);

/**
 * Sends a characteristic notification.  The content of the message is read
 * from the specified characteristic.
 *
 * @param conn_handle           The connection over which to execute the
 *                                  procedure.
 * @param chr_val_handle        The value attribute handle of the
 *                                  characteristic to include in the outgoing
 *                                  notification.
 *
 * @return                      0 on success; nonzero on failure.
 */
int ble_gattc_notify(uint16_t conn_handle, uint16_t chr_val_handle);

/**
 * Sends a "free-form" characteristic indication.  The provided mbuf contains
 * the indication payload.  This function consumes the supplied mbuf regardless
 * of the outcome.
 *
 * @param conn_handle           The connection over which to execute the
 *                                  procedure.
 * @param chr_val_handle        The value attribute handle of the
 *                                  characteristic to include in the outgoing
 *                                  indication.
 * @param txom                  The data to include in the indication.
 *
 * @return                      0 on success; nonzero on failure.
 */
int ble_gattc_indicate_custom(uint16_t conn_handle, uint16_t chr_val_handle,
                              struct os_mbuf *txom);

/**
 * Sends a characteristic indication.  The content of the message is read from
 * the specified characteristic.
 *
 * @param conn_handle           The connection over which to execute the
 *                                  procedure.
 * @param chr_val_handle        The value attribute handle of the
 *                                  characteristic to include in the outgoing
 *                                  indication.
 *
 * @return                      0 on success; nonzero on failure.
 */
int ble_gattc_indicate(uint16_t conn_handle, uint16_t chr_val_handle);

int ble_gattc_init(void);

/*** @server. */

struct ble_gatt_access_ctxt;
typedef int ble_gatt_access_fn(uint16_t conn_handle, uint16_t attr_handle,
                               struct ble_gatt_access_ctxt *ctxt, void *arg);

typedef uint16_t ble_gatt_chr_flags;

struct ble_gatt_chr_def {
    /**
     * Pointer to characteristic UUID; use BLE_UUIDxx_DECLARE macros to declare
     * proper UUID; NULL if there are no more characteristics in the service.
     */
    const ble_uuid_t *uuid;

    /**
     * Callback that gets executed when this characteristic is read or
     * written.
     */
    ble_gatt_access_fn *access_cb;

    /** Optional argument for callback. */
    void *arg;

    /**
     * Array of this characteristic's descriptors.  NULL if no descriptors.
     * Do not include CCCD; it gets added automatically if this
     * characteristic's notify or indicate flag is set.
     */
    struct ble_gatt_dsc_def *descriptors;

    /** Specifies the set of permitted operations for this characteristic. */
    ble_gatt_chr_flags flags;

    /** Specifies minimum required key size to access this characteristic. */
    uint8_t min_key_size;

    /**
     * At registration time, this is filled in with the characteristic's value
     * attribute handle.
     */
    uint16_t *val_handle;
};

struct ble_gatt_svc_def {
    /**
     * One of the following:
     *     o BLE_GATT_SVC_TYPE_PRIMARY - primary service
     *     o BLE_GATT_SVC_TYPE_SECONDARY - secondary service
     *     o 0 - No more services in this array.
     */
    uint8_t type;

    /**
     * Pointer to service UUID; use BLE_UUIDxx_DECLARE macros to declare
     * proper UUID; NULL if there are no more characteristics in the service.
     */
    const ble_uuid_t *uuid;

    /**
     * Array of pointers to other service definitions.  These services are
     * reported as "included services" during service discovery.  Terminate the
     * array with NULL.
     */
    const struct ble_gatt_svc_def **includes;

    /**
     * Array of characteristic definitions corresponding to characteristics
     * belonging to this service.
     */
    const struct ble_gatt_chr_def *characteristics;
};

struct ble_gatt_dsc_def {
    /**
     * Pointer to descriptor UUID; use BLE_UUIDxx_DECLARE macros to declare
     * proper UUID; NULL if there are no more characteristics in the service.
     */
    const ble_uuid_t *uuid;

    /** Specifies the set of permitted operations for this descriptor. */
    uint8_t att_flags;

    /** Specifies minimum required key size to access this descriptor. */
    uint8_t min_key_size;

    /** Callback that gets executed when the descriptor is read or written. */
    ble_gatt_access_fn *access_cb;

    /** Optional argument for callback. */
    void *arg;
};

/**
 * Context for an access to a GATT characteristic or descriptor.  When a client
 * reads or writes a locally registered characteristic or descriptor, an
 * instance of this struct gets passed to the application callback.
 */
struct ble_gatt_access_ctxt {
    /**
     * Indicates the gatt operation being performed.  This is equal to one of
     * the following values:
     *     o  BLE_GATT_ACCESS_OP_READ_CHR
     *     o  BLE_GATT_ACCESS_OP_WRITE_CHR
     *     o  BLE_GATT_ACCESS_OP_READ_DSC
     *     o  BLE_GATT_ACCESS_OP_WRITE_DSC
     */
    uint8_t op;

    /**
     * A container for the GATT access data.
     *     o For reads: The application populates this with the value of the
     *       characteristic or descriptor being read.
     *     o For writes: This is already populated with the value being written
     *       by the peer.  If the application wishes to retain this mbuf for
     *       later use, the access callback must set this pointer to NULL to
     *       prevent the stack from freeing it.
     */
    struct os_mbuf *om;

    /**
     * The GATT operation being performed dictates which field in this union is
     * valid.  If a characteristic is being accessed, the chr field is valid.
     * Otherwise a descriptor is being accessed, in which case the dsc field
     * is valid.
     */
    union {
        /**
         * The characteristic definition corresponding to the characteristic
         * being accessed.  This is what the app registered at startup.
         */
        const struct ble_gatt_chr_def *chr;

        /**
         * The descriptor definition corresponding to the descriptor being
         * accessed.  This is what the app registered at startup.
         */
        const struct ble_gatt_dsc_def *dsc;
    };
};

/**
 * Context passed to the registration callback; represents the GATT service,
 * characteristic, or descriptor being registered.
 */
struct ble_gatt_register_ctxt {
    /**
     * Indicates the gatt registration operation just performed.  This is
     * equal to one of the following values:
     *     o BLE_GATT_REGISTER_OP_SVC
     *     o BLE_GATT_REGISTER_OP_CHR
     *     o BLE_GATT_REGISTER_OP_DSC
     */
    uint8_t op;

    /**
     * The value of the op field determines which field in this union is valid.
     */
    union {
        /** Service; valid if op == BLE_GATT_REGISTER_OP_SVC. */
        struct {
            /** The ATT handle of the service definition attribute. */
            uint16_t handle;

            /**
             * The service definition representing the service being
             * registered.
             */
            const struct ble_gatt_svc_def *svc_def;
        } svc;

        /** Characteristic; valid if op == BLE_GATT_REGISTER_OP_CHR. */
        struct {
            /** The ATT handle of the characteristic definition attribute. */
            uint16_t def_handle;

            /** The ATT handle of the characteristic value attribute. */
            uint16_t val_handle;

            /**
             * The characteristic definition representing the characteristic
             * being registered.
             */
            const struct ble_gatt_chr_def *chr_def;

            /**
             * The service definition corresponding to the characteristic's
             * parent service.
             */
            const struct ble_gatt_svc_def *svc_def;
        } chr;

        /** Descriptor; valid if op == BLE_GATT_REGISTER_OP_DSC. */
        struct {
            /** The ATT handle of the descriptor definition attribute. */
            uint16_t handle;

            /**
             * The descriptor definition corresponding to the descriptor being
             * registered.
             */
            const struct ble_gatt_dsc_def *dsc_def;

            /**
             * The characteristic definition corresponding to the descriptor's
             * parent characteristic.
             */
            const struct ble_gatt_chr_def *chr_def;

            /**
             * The service definition corresponding to the descriptor's
             * grandparent service
             */
            const struct ble_gatt_svc_def *svc_def;
        } dsc;
    };
};

typedef void ble_gatt_register_fn(struct ble_gatt_register_ctxt *ctxt,
                                  void *arg);

/**
 * Queues a set of service definitions for registration.  All services queued
 * in this manner get registered when ble_gatts_start() is called.
 *
 * @param svcs                  An array of service definitions to queue for
 *                                  registration.  This array must be
 *                                  terminated with an entry whose 'type'
 *                                  equals 0.
 *
 * @return                      0 on success;
 *                              BLE_HS_ENOMEM on heap exhaustion.
 */
int ble_gatts_add_svcs(const struct ble_gatt_svc_def *svcs);

/**
 * Set visibility of local GATT service. Invisible services are not removed
 * from database but are not discoverable by peer devices. Service Changed
 * should be handled by application when needed by calling
 * ble_svc_gatt_changed().
 *
 * @param handle                Handle of service
 * @param visible               non-zero if service should be visible
 *
 * @return                      0 on success;
 *                              BLE_HS_ENOENT if service wasn't found.
 */
int ble_gatts_svc_set_visibility(uint16_t handle, int visible);

/**
 * Adjusts a host configuration object's settings to accommodate the specified
 * service definition array.  This function adds the counts to the appropriate
 * fields in the supplied configuration object without clearing them first, so
 * it can be called repeatedly with different inputs to calculate totals.  Be
 * sure to zero the GATT server settings prior to the first call to this
 * function.
 *
 * @param defs                  The service array containing the resource
 *                                  definitions to be counted.
 *
 * @return                      0 on success;
 *                              BLE_HS_EINVAL if the svcs array contains an
 *                                  invalid resource definition.
 */
int ble_gatts_count_cfg(const struct ble_gatt_svc_def *defs);

/**
 * Send notification (or indication) to any connected devices that have
 * subscribed for notification (or indication) for specified characteristic.
 *
 * @param chr_val_handle        Characteristic value handle
 */
void ble_gatts_chr_updated(uint16_t chr_val_handle);

/**
 * Retrieves the attribute handle associated with a local GATT service.
 *
 * @param uuid                  The UUID of the service to look up.
 * @param out_handle            On success, populated with the handle of the
 *                                  service attribute.  Pass null if you don't
 *                                  need this value.
 *
 * @return                      0 on success;
 *                              BLE_HS_ENOENT if the specified service could
 *                                  not be found.
 */
int ble_gatts_find_svc(const ble_uuid_t *uuid, uint16_t *out_handle);

/**
 * Retrieves the pair of attribute handles associated with a local GATT
 * characteristic.
 *
 * @param svc_uuid              The UUID of the parent service.
 * @param chr_uuid              The UUID of the characteristic to look up.
 * @param out_def_handle        On success, populated with the handle
 *                                  of the characteristic definition attribute.
 *                                  Pass null if you don't need this value.
 * @param out_val_handle        On success, populated with the handle
 *                                  of the characteristic value attribute.
 *                                  Pass null if you don't need this value.
 *
 * @return                      0 on success;
 *                              BLE_HS_ENOENT if the specified service or
 *                                  characteristic could not be found.
 */
int ble_gatts_find_chr(const ble_uuid_t *svc_uuid, const ble_uuid_t *chr_uuid,
                       uint16_t *out_def_handle, uint16_t *out_val_handle);

/**
 * Retrieves the attribute handle associated with a local GATT descriptor.
 *
 * @param svc_uuid              The UUID of the grandparent service.
 * @param chr_uuid              The UUID of the parent characteristic.
 * @param dsc_uuid              The UUID of the descriptor ro look up.
 * @param out_handle            On success, populated with the handle
 *                                  of the descriptor attribute.  Pass null if
 *                                  you don't need this value.
 *
 * @return                      0 on success;
 *                              BLE_HS_ENOENT if the specified service,
 *                                  characteristic, or descriptor could not be
 *                                  found.
 */
int ble_gatts_find_dsc(const ble_uuid_t *svc_uuid, const ble_uuid_t *chr_uuid,
                       const ble_uuid_t *dsc_uuid, uint16_t *out_dsc_handle);

typedef void (*ble_gatt_svc_foreach_fn)(const struct ble_gatt_svc_def *svc,
                                        uint16_t handle,
                                        uint16_t end_group_handle,
                                        void *arg);

/**
 * Prints dump of local GATT database. This is useful to log local state of
 * database in human readable form.
 */
void ble_gatts_show_local(void);

/**
 * Resets the GATT server to its initial state.  On success, this function
 * removes all supported services, characteristics, and descriptors.  This
 * function requires that:
 *     o No peers are connected, and
 *     o No GAP operations are active (advertise, discover, or connect).
 *
 * @return                      0 on success;
 *                              BLE_HS_EBUSY if the GATT server could not be
 *                                  reset due to existing connections or active
 *                                  GAP procedures.
 */
int ble_gatts_reset(void);

/**
 * Makes all registered services available to peers.  This function gets called
 * automatically by the NimBLE host on startup; manual calls are only necessary
 * for replacing the set of supported services with a new one.  This function
 * requires that:
 *     o No peers are connected, and
 *     o No GAP operations are active (advertise, discover, or connect).
 *
 * @return                      0 on success;
 *                              A BLE host core return code on unexpected
 *                                  error.
 */
int ble_gatts_start(void);

/**
 * Resets the GATT configuration parameters and deallocates the memory of attributes.
 *
 */
void ble_gatts_stop(void);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif
