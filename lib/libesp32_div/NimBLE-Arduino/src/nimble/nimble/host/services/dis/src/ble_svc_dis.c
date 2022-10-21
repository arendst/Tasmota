/**
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

#include <assert.h>
#include <string.h>
#include "nimble/porting/nimble/include/sysinit/sysinit.h"
#include "nimble/nimble/host/include/host/ble_hs.h"
#include "../include/services/dis/ble_svc_dis.h"

/* Device information */
struct ble_svc_dis_data ble_svc_dis_data = {
    .model_number      = MYNEWT_VAL(BLE_SVC_DIS_MODEL_NUMBER_DEFAULT),
    .serial_number     = MYNEWT_VAL(BLE_SVC_DIS_SERIAL_NUMBER_DEFAULT),
    .firmware_revision = MYNEWT_VAL(BLE_SVC_DIS_FIRMWARE_REVISION_DEFAULT),
    .hardware_revision = MYNEWT_VAL(BLE_SVC_DIS_HARDWARE_REVISION_DEFAULT),
    .software_revision = MYNEWT_VAL(BLE_SVC_DIS_SOFTWARE_REVISION_DEFAULT),
    .manufacturer_name = MYNEWT_VAL(BLE_SVC_DIS_MANUFACTURER_NAME_DEFAULT),
    .system_id         = MYNEWT_VAL(BLE_SVC_DIS_SYSTEM_ID_DEFAULT),
};

/* Access function */
#if (MYNEWT_VAL(BLE_SVC_DIS_MODEL_NUMBER_READ_PERM)      >= 0) ||	\
    (MYNEWT_VAL(BLE_SVC_DIS_SERIAL_NUMBER_READ_PERM)     >= 0) ||	\
    (MYNEWT_VAL(BLE_SVC_DIS_HARDWARE_REVISION_READ_PERM) >= 0) ||	\
    (MYNEWT_VAL(BLE_SVC_DIS_FIRMWARE_REVISION_READ_PERM) >= 0) ||	\
    (MYNEWT_VAL(BLE_SVC_DIS_SOFTWARE_REVISION_READ_PERM) >= 0) ||	\
    (MYNEWT_VAL(BLE_SVC_DIS_MANUFACTURER_NAME_READ_PERM) >= 0) || \
    (MYNEWT_VAL(BLE_SVC_DIS_SYSTEM_ID_READ_PERM) >= 0)
static int
ble_svc_dis_access(uint16_t conn_handle, uint16_t attr_handle,
                   struct ble_gatt_access_ctxt *ctxt, void *arg);
#endif

static const struct ble_gatt_svc_def ble_svc_dis_defs[] = {
    { /*** Service: Device Information Service (DIS). */
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = BLE_UUID16_DECLARE(BLE_SVC_DIS_UUID16),
        .characteristics = (struct ble_gatt_chr_def[]) { {
#if (MYNEWT_VAL(BLE_SVC_DIS_MODEL_NUMBER_READ_PERM) >= 0)
	    /*** Characteristic: Model Number String */
            .uuid = BLE_UUID16_DECLARE(BLE_SVC_DIS_CHR_UUID16_MODEL_NUMBER),
            .access_cb = ble_svc_dis_access,
            .flags = BLE_GATT_CHR_F_READ |
	             MYNEWT_VAL(BLE_SVC_DIS_MODEL_NUMBER_READ_PERM),
        }, {
#endif
#if (MYNEWT_VAL(BLE_SVC_DIS_SERIAL_NUMBER_READ_PERM) >= 0)
	    /*** Characteristic: Serial Number String */
            .uuid = BLE_UUID16_DECLARE(BLE_SVC_DIS_CHR_UUID16_SERIAL_NUMBER),
            .access_cb = ble_svc_dis_access,
            .flags = BLE_GATT_CHR_F_READ |
	             MYNEWT_VAL(BLE_SVC_DIS_SERIAL_NUMBER_READ_PERM),
        }, {
#endif
#if (MYNEWT_VAL(BLE_SVC_DIS_HARDWARE_REVISION_READ_PERM) >= 0)
	    /*** Characteristic: Hardware Revision String */
            .uuid = BLE_UUID16_DECLARE(BLE_SVC_DIS_CHR_UUID16_HARDWARE_REVISION),
            .access_cb = ble_svc_dis_access,
            .flags = BLE_GATT_CHR_F_READ |
	             MYNEWT_VAL(BLE_SVC_DIS_HARDWARE_REVISION_READ_PERM),
        }, {
#endif
#if (MYNEWT_VAL(BLE_SVC_DIS_FIRMWARE_REVISION_READ_PERM) >= 0)
	    /*** Characteristic: Firmware Revision String */
            .uuid = BLE_UUID16_DECLARE(BLE_SVC_DIS_CHR_UUID16_FIRMWARE_REVISION),
            .access_cb = ble_svc_dis_access,
            .flags = BLE_GATT_CHR_F_READ |
	             MYNEWT_VAL(BLE_SVC_DIS_FIRMWARE_REVISION_READ_PERM),
        }, {
#endif
#if (MYNEWT_VAL(BLE_SVC_DIS_SOFTWARE_REVISION_READ_PERM) >= 0)
	    /*** Characteristic: Software Revision String */
            .uuid = BLE_UUID16_DECLARE(BLE_SVC_DIS_CHR_UUID16_SOFTWARE_REVISION),
            .access_cb = ble_svc_dis_access,
            .flags = BLE_GATT_CHR_F_READ |
	             MYNEWT_VAL(BLE_SVC_DIS_SOFTWARE_REVISION_READ_PERM),
        }, {
#endif
#if (MYNEWT_VAL(BLE_SVC_DIS_MANUFACTURER_NAME_READ_PERM) >= 0)
	    /*** Characteristic: Manufacturer Name */
            .uuid = BLE_UUID16_DECLARE(BLE_SVC_DIS_CHR_UUID16_MANUFACTURER_NAME),
            .access_cb = ble_svc_dis_access,
            .flags = BLE_GATT_CHR_F_READ |
	             MYNEWT_VAL(BLE_SVC_DIS_MANUFACTURER_NAME_READ_PERM),
        }, {
#endif
#if (MYNEWT_VAL(BLE_SVC_DIS_SYSTEM_ID_READ_PERM) >= 0)
      /*** Characteristic: System Id */
            .uuid = BLE_UUID16_DECLARE(BLE_SVC_DIS_CHR_UUID16_SYSTEM_ID),
            .access_cb = ble_svc_dis_access,
            .flags = BLE_GATT_CHR_F_READ |
               MYNEWT_VAL(BLE_SVC_DIS_SYSTEM_ID_READ_PERM),
            }, {
#endif

            0, /* No more characteristics in this service */
        }, }
    },

    {
        0, /* No more services. */
    },
};

/**
 * Simple read access callback for the device information service
 * characteristic.
 */
#if (MYNEWT_VAL(BLE_SVC_DIS_MODEL_NUMBER_READ_PERM)      >= 0) ||	\
    (MYNEWT_VAL(BLE_SVC_DIS_SERIAL_NUMBER_READ_PERM)     >= 0) ||	\
    (MYNEWT_VAL(BLE_SVC_DIS_HARDWARE_REVISION_READ_PERM) >= 0) ||	\
    (MYNEWT_VAL(BLE_SVC_DIS_FIRMWARE_REVISION_READ_PERM) >= 0) ||	\
    (MYNEWT_VAL(BLE_SVC_DIS_SOFTWARE_REVISION_READ_PERM) >= 0) ||	\
    (MYNEWT_VAL(BLE_SVC_DIS_MANUFACTURER_NAME_READ_PERM) >= 0) || \
    (MYNEWT_VAL(BLE_SVC_DIS_SYSTEM_ID_READ_PERM) >= 0)
static int
ble_svc_dis_access(uint16_t conn_handle, uint16_t attr_handle,
                   struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    uint16_t uuid    = ble_uuid_u16(ctxt->chr->uuid);
    const char *info = NULL;

    switch(uuid) {
#if (MYNEWT_VAL(BLE_SVC_DIS_MODEL_NUMBER_READ_PERM) >= 0)
    case BLE_SVC_DIS_CHR_UUID16_MODEL_NUMBER:
        info = ble_svc_dis_data.model_number;
#ifdef MYNEWT_VAL_BLE_SVC_DIS_MODEL_NUMBER_NAME_DEFAULT
        if (info == NULL) {
            info = MYNEWT_VAL(BLE_SVC_DIS_MODEL_NUMBER_DEFAULT);
        }
#endif
        break;
#endif
#if (MYNEWT_VAL(BLE_SVC_DIS_SERIAL_NUMBER_READ_PERM) >= 0)
    case BLE_SVC_DIS_CHR_UUID16_SERIAL_NUMBER:
        info = ble_svc_dis_data.serial_number;
#ifdef MYNEWT_VAL_BLE_SVC_DIS_SERIAL_NUMBER_DEFAULT
        if (info == NULL) {
            info = MYNEWT_VAL(BLE_SVC_DIS_SERIAL_NUMBER_DEFAULT);
        }
#endif
        break;
#endif
#if (MYNEWT_VAL(BLE_SVC_DIS_FIRMWARE_REVISION_READ_PERM) >= 0)
    case BLE_SVC_DIS_CHR_UUID16_FIRMWARE_REVISION:
        info = ble_svc_dis_data.firmware_revision;
#ifdef MYNEWT_VAL_BLE_SVC_DIS_FIRMWARE_REVISION_DEFAULT
        if (info == NULL) {
            info = MYNEWT_VAL(BLE_SVC_DIS_FIRMWARE_REVISION_DEFAULT);
        }
#endif
        break;
#endif
#if (MYNEWT_VAL(BLE_SVC_DIS_HARDWARE_REVISION_READ_PERM) >= 0)
    case BLE_SVC_DIS_CHR_UUID16_HARDWARE_REVISION:
        info = ble_svc_dis_data.hardware_revision;
#ifdef MYNEWT_VAL_BLE_SVC_DIS_HARDWARE_REVISION_DEFAULT
        if (info == NULL) {
            info = MYNEWT_VAL(BLE_SVC_DIS_HARDWARE_REVISION_DEFAULT);
        }
#endif
        break;
#endif
#if (MYNEWT_VAL(BLE_SVC_DIS_SOFTWARE_REVISION_READ_PERM) >= 0)
    case BLE_SVC_DIS_CHR_UUID16_SOFTWARE_REVISION:
        info = ble_svc_dis_data.software_revision;
#ifdef MYNEWT_VAL_BLE_SVC_DIS_SOFTWARE_REVISION_DEFAULT
        if (info == NULL) {
            info = MYNEWT_VAL(BLE_SVC_DIS_SOFTWARE_REVISION_DEFAULT);
        }
#endif
        break;
#endif
#if (MYNEWT_VAL(BLE_SVC_DIS_MANUFACTURER_NAME_READ_PERM) >= 0)
    case BLE_SVC_DIS_CHR_UUID16_MANUFACTURER_NAME:
        info = ble_svc_dis_data.manufacturer_name;
#ifdef MYNEWT_VAL_BLE_SVC_DIS_MANUFACTURER_NAME_DEFAULT
        if (info == NULL) {
            info = MYNEWT_VAL(BLE_SVC_DIS_MANUFACTURER_NAME_DEFAULT);
        }
#endif
        break;
#endif
#if (MYNEWT_VAL(BLE_SVC_DIS_SYSTEM_ID_READ_PERM) >= 0)
    case BLE_SVC_DIS_CHR_UUID16_SYSTEM_ID:
        info = ble_svc_dis_data.system_id;
#ifdef MYNEWT_VAL_BLE_SVC_DIS_SYSTEM_ID_DEFAULT
        if (info == NULL) {
            info = MYNEWT_VAL(BLE_SVC_DIS_SYSTEM_ID_DEFAULT);
        }
#endif
        break;
#endif
    default:
        assert(0);
        return BLE_ATT_ERR_UNLIKELY;
    }

    if (info != NULL) {
       int rc = os_mbuf_append(ctxt->om, info, strlen(info));
       return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
    }

    return 0;
}
#endif

const char *
ble_svc_dis_model_number(void)
{
    return ble_svc_dis_data.model_number;
}

int
ble_svc_dis_model_number_set(const char *value)
{
    ble_svc_dis_data.model_number = value;
    return 0;
}

const char *
ble_svc_dis_serial_number(void)
{
    return ble_svc_dis_data.serial_number;
}

int
ble_svc_dis_serial_number_set(const char *value)
{
    ble_svc_dis_data.serial_number = value;
    return 0;
}

const char *
ble_svc_dis_firmware_revision(void)
{
    return ble_svc_dis_data.firmware_revision;
}

int
ble_svc_dis_firmware_revision_set(const char *value)
{
    ble_svc_dis_data.firmware_revision = value;
    return 0;
}

const char *
ble_svc_dis_hardware_revision(void)
{
    return ble_svc_dis_data.hardware_revision;
}

int
ble_svc_dis_hardware_revision_set(const char *value)
{
    ble_svc_dis_data.hardware_revision = value;
    return 0;
}

const char *
ble_svc_dis_software_revision(void)
{
    return ble_svc_dis_data.software_revision;
}

int
ble_svc_dis_software_revision_set(const char *value)
{
    ble_svc_dis_data.software_revision = value;
    return 0;
}

const char *
ble_svc_dis_manufacturer_name(void)
{
    return ble_svc_dis_data.manufacturer_name;
}

int
ble_svc_dis_manufacturer_name_set(const char *value)
{
    ble_svc_dis_data.manufacturer_name = value;
    return 0;
}

const char *
ble_svc_dis_system_id(void)
{
    return ble_svc_dis_data.system_id;
}

int
ble_svc_dis_system_id_set(const char *value)
{
    ble_svc_dis_data.system_id = value;
    return 0;
}

/**
 * Initialize the DIS package.
 */
void
ble_svc_dis_init(void)
{
    int rc;

    /* Ensure this function only gets called by sysinit. */
    SYSINIT_ASSERT_ACTIVE();

    rc = ble_gatts_count_cfg(ble_svc_dis_defs);
    SYSINIT_PANIC_ASSERT(rc == 0);

    rc = ble_gatts_add_svcs(ble_svc_dis_defs);
    SYSINIT_PANIC_ASSERT(rc == 0);
}
