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

#ifndef H_NIMBLE_OPT_AUTO_
#define H_NIMBLE_OPT_AUTO_

#include "syscfg/syscfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/***
 * Automatic options.
 *
 * These settings are generated automatically from the user-specified syscfg
 * settings.
 */

#undef NIMBLE_BLE_ADVERTISE
#define NIMBLE_BLE_ADVERTISE                    \
    (MYNEWT_VAL(BLE_ROLE_BROADCASTER) || MYNEWT_VAL(BLE_ROLE_PERIPHERAL))

#undef NIMBLE_BLE_SCAN
#define NIMBLE_BLE_SCAN                         \
    (MYNEWT_VAL(BLE_ROLE_CENTRAL) || MYNEWT_VAL(BLE_ROLE_OBSERVER))

#undef NIMBLE_BLE_CONNECT
#define NIMBLE_BLE_CONNECT                      \
    (MYNEWT_VAL(BLE_ROLE_CENTRAL) || MYNEWT_VAL(BLE_ROLE_PERIPHERAL))


/** Supported client ATT commands. */

#undef NIMBLE_BLE_ATT_CLT_FIND_INFO
#define NIMBLE_BLE_ATT_CLT_FIND_INFO            \
    (MYNEWT_VAL(BLE_GATT_DISC_ALL_DSCS))

#undef NIMBLE_BLE_ATT_CLT_FIND_TYPE
#define NIMBLE_BLE_ATT_CLT_FIND_TYPE            \
    (MYNEWT_VAL(BLE_GATT_DISC_SVC_UUID))

#undef NIMBLE_BLE_ATT_CLT_READ_TYPE
#define NIMBLE_BLE_ATT_CLT_READ_TYPE            \
    (MYNEWT_VAL(BLE_GATT_FIND_INC_SVCS) ||      \
     MYNEWT_VAL(BLE_GATT_DISC_ALL_CHRS) ||      \
     MYNEWT_VAL(BLE_GATT_DISC_CHRS_UUID) ||     \
     MYNEWT_VAL(BLE_GATT_READ_UUID))

#undef NIMBLE_BLE_ATT_CLT_READ
#define NIMBLE_BLE_ATT_CLT_READ                 \
    (MYNEWT_VAL(BLE_GATT_READ) ||               \
     MYNEWT_VAL(BLE_GATT_READ_LONG) ||          \
     MYNEWT_VAL(BLE_GATT_FIND_INC_SVCS))

#undef NIMBLE_BLE_ATT_CLT_READ_BLOB
#define NIMBLE_BLE_ATT_CLT_READ_BLOB            \
    (MYNEWT_VAL(BLE_GATT_READ_LONG))

#undef NIMBLE_BLE_ATT_CLT_READ_MULT
#define NIMBLE_BLE_ATT_CLT_READ_MULT            \
    (MYNEWT_VAL(BLE_GATT_READ_MULT))

#undef NIMBLE_BLE_ATT_CLT_READ_GROUP_TYPE
#define NIMBLE_BLE_ATT_CLT_READ_GROUP_TYPE      \
    (MYNEWT_VAL(BLE_GATT_DISC_ALL_SVCS))

#undef NIMBLE_BLE_ATT_CLT_WRITE
#define NIMBLE_BLE_ATT_CLT_WRITE                \
    (MYNEWT_VAL(BLE_GATT_WRITE))

#undef NIMBLE_BLE_ATT_CLT_WRITE_NO_RSP
#define NIMBLE_BLE_ATT_CLT_WRITE_NO_RSP         \
    (MYNEWT_VAL(BLE_GATT_WRITE_NO_RSP))

#undef NIMBLE_BLE_ATT_CLT_PREP_WRITE
#define NIMBLE_BLE_ATT_CLT_PREP_WRITE           \
    (MYNEWT_VAL(BLE_GATT_WRITE_LONG))

#undef NIMBLE_BLE_ATT_CLT_EXEC_WRITE
#define NIMBLE_BLE_ATT_CLT_EXEC_WRITE           \
    (MYNEWT_VAL(BLE_GATT_WRITE_LONG))

#undef NIMBLE_BLE_ATT_CLT_NOTIFY
#define NIMBLE_BLE_ATT_CLT_NOTIFY               \
    (MYNEWT_VAL(BLE_GATT_NOTIFY))

#undef NIMBLE_BLE_ATT_CLT_INDICATE
#define NIMBLE_BLE_ATT_CLT_INDICATE             \
    (MYNEWT_VAL(BLE_GATT_INDICATE))

/** Security manager settings. */

#undef NIMBLE_BLE_SM
#define NIMBLE_BLE_SM   (MYNEWT_VAL(BLE_SM_LEGACY) || MYNEWT_VAL(BLE_SM_SC))

#ifdef __cplusplus
}
#endif

#endif
