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
#include <stdint.h>
#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#include "nimble/nimble/include/nimble/ble.h"
#include "nimble/nimble/include/nimble/hci_common.h"
#include "../include/controller/ble_ll_iso.h"

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_ISO)

int
ble_ll_iso_read_tx_sync(const uint8_t *cmdbuf, uint8_t len)
{
    return BLE_ERR_UNSUPPORTED;
}

int
ble_ll_iso_set_cig_param(const uint8_t *cmdbuf, uint8_t len,
                         uint8_t *rspbuf, uint8_t *rsplen)
{
    return BLE_ERR_UNSUPPORTED;
}

int
ble_ll_iso_create_cis(const uint8_t *cmdbuf, uint8_t len)
{
    return BLE_ERR_UNSUPPORTED;
}

int
ble_ll_iso_disconnect_cmd(const struct ble_hci_lc_disconnect_cp *cmd)
{
    return BLE_ERR_UNSUPPORTED;
}

int
ble_ll_iso_remove_cig(const uint8_t *cmdbuf, uint8_t len,
                      uint8_t *rspbuf, uint8_t *rsplen)
{
    return BLE_ERR_UNSUPPORTED;
}

int
ble_ll_iso_accept_cis_req(const uint8_t *cmdbuf, uint8_t len)
{
    return BLE_ERR_UNSUPPORTED;
}

int
ble_ll_iso_reject_cis_req(const uint8_t *cmdbuf, uint8_t len)
{
    return BLE_ERR_UNSUPPORTED;
}

int
ble_ll_iso_setup_iso_data_path(const uint8_t *cmdbuf, uint8_t len)
{
    return BLE_ERR_UNSUPPORTED;
}

int
ble_ll_iso_remove_iso_data_path(const uint8_t *cmdbuf, uint8_t len)
{
    /* Nothing to do here for now when HCI is supported */
    return 0;
}
int
ble_ll_iso_create_big(const uint8_t *cmdbuf, uint8_t len)
{
    return BLE_ERR_UNSUPPORTED;
}

int
ble_ll_iso_terminate_big(const uint8_t *cmdbuf, uint8_t len)
{
    return BLE_ERR_UNSUPPORTED;
}

int
ble_ll_iso_big_create_sync(const uint8_t *cmdbuf, uint8_t len)
{
    return BLE_ERR_UNSUPPORTED;
}

int
ble_ll_iso_big_terminate_sync(const uint8_t *cmdbuf, uint8_t len)
{
    return BLE_ERR_UNSUPPORTED;
}

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_ISO_TEST)
int
ble_ll_iso_set_cig_param_test(const uint8_t *cmdbuf, uint8_t len,
                              uint8_t *rspbuf, uint8_t *rsplen)
{
    return BLE_ERR_UNSUPPORTED;
}

int
ble_ll_iso_create_big_test(const uint8_t *cmdbuf, uint8_t len)
{
    return BLE_ERR_UNSUPPORTED;
}

int
ble_ll_iso_transmit_test(const uint8_t *cmdbuf, uint8_t len)
{
    return BLE_ERR_UNSUPPORTED;
}

int
ble_ll_iso_receive_test(const uint8_t *cmdbuf, uint8_t len)
{
    return BLE_ERR_UNSUPPORTED;
}

int
ble_ll_iso_read_counters_test(const uint8_t *cmdbuf, uint8_t len)
{
    return BLE_ERR_UNSUPPORTED;
}

int
ble_ll_iso_end_test(const uint8_t *cmdbuf, uint8_t len)
{
    return BLE_ERR_UNSUPPORTED;
}
#endif
#endif
#endif
