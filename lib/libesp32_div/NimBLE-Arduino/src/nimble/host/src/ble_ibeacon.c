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
#include "host/ble_hs_adv.h"
#include "ble_hs_priv.h"

#define BLE_IBEACON_MFG_DATA_SIZE       25

/**
 * Configures the device to advertise iBeacons.
 *
 * @param uuid                  The 128-bit UUID to advertise.
 * @param major                 The major version number to include in
 *                                  iBeacons.
 * @param minor                 The minor version number to include in
 *                                  iBeacons.
 * @param measured_power        The Measured Power (RSSI value at 1 Meter).
 *
 * @return                      0 on success;
 *                              BLE_HS_EBUSY if advertising is in progress;
 *                              Other nonzero on failure.
 */
int
ble_ibeacon_set_adv_data(void *uuid128, uint16_t major,
                         uint16_t minor, int8_t measured_power)
{
    struct ble_hs_adv_fields fields;
    uint8_t buf[BLE_IBEACON_MFG_DATA_SIZE];
    int rc;

    /** Company identifier (Apple). */
    buf[0] = 0x4c;
    buf[1] = 0x00;

    /** iBeacon indicator. */
    buf[2] = 0x02;
    buf[3] = 0x15;

    /** UUID. */
    memcpy(buf + 4, uuid128, 16);

    /** Version number. */
    put_be16(buf + 20, major);
    put_be16(buf + 22, minor);

    /* Measured Power ranging data (Calibrated tx power at 1 meters). */
    if (measured_power < -126 || measured_power > 20) {
        return BLE_HS_EINVAL;
    }
    buf[24] = measured_power;

    memset(&fields, 0, sizeof fields);
    fields.mfg_data = buf;
    fields.mfg_data_len = sizeof buf;

    /* Advertise two flags:
     *     o Discoverability in forthcoming advertisement (general)
     *     o BLE-only (BR/EDR unsupported).
     */
    fields.flags = BLE_HS_ADV_F_DISC_GEN |
                   BLE_HS_ADV_F_BREDR_UNSUP;

    rc = ble_gap_adv_set_fields(&fields);
    return rc;
}
