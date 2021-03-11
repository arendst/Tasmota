/*
* Copyright 2015-2019 Espressif Systems (Shanghai) PTE LTD
*
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


#include "syscfg/syscfg.h"

#if MYNEWT_VAL(BLE_STORE_CONFIG_PERSIST)

#include <string.h>
#include <esp_system.h>
#include "sysinit/sysinit.h"
#include "host/ble_hs.h"
#include "store/config/ble_store_config.h"
#include "ble_store_config_priv.h"
#include "esp_log.h"
#include "nvs.h"
#include "../../../src/ble_hs_resolv_priv.h"


#define NIMBLE_NVS_STR_NAME_MAX_LEN              16
#define NIMBLE_NVS_PEER_SEC_KEY                  "peer_sec"
#define NIMBLE_NVS_OUR_SEC_KEY                   "our_sec"
#define NIMBLE_NVS_CCCD_SEC_KEY                  "cccd_sec"
#define NIMBLE_NVS_PEER_RECORDS_KEY              "p_dev_rec"
#define NIMBLE_NVS_NAMESPACE                     "nimble_bond"

typedef uint32_t nvs_handle_t;

static const char *TAG = "NIMBLE_NVS";

/*****************************************************************************
 * $ MISC                                                                    *
 *****************************************************************************/

static void
get_nvs_key_string(int obj_type, int index, char *key_string)
{
    if (obj_type == BLE_STORE_OBJ_TYPE_PEER_DEV_REC) {
        sprintf(key_string, "%s_%d", NIMBLE_NVS_PEER_RECORDS_KEY, index);
    } else {
        if (obj_type == BLE_STORE_OBJ_TYPE_PEER_SEC) {
            sprintf(key_string, "%s_%d", NIMBLE_NVS_PEER_SEC_KEY, index);
        } else if (obj_type == BLE_STORE_OBJ_TYPE_OUR_SEC) {
            sprintf(key_string, "%s_%d", NIMBLE_NVS_OUR_SEC_KEY, index);
        } else {
            sprintf(key_string, "%s_%d", NIMBLE_NVS_CCCD_SEC_KEY, index);
        }
    }
}

/* compares values at two addresses of size = item_size
* @Returns               index if entries match
*                       -1 if mismatch
*/
static int
get_nvs_matching_index(void *nvs_val, void *db_list, int db_num, size_t
                       item_size)
{
    uint8_t *db_item = (uint8_t *)db_list;
    int i;

    for (i = 0; i < db_num; i++) {
        if (memcmp(nvs_val, db_item, item_size) == 0) {
            /* Key matches with the one in RAM database */
            return i;
        }
        db_item += item_size;
    }
    return -1;
}

static int
get_nvs_max_obj_value(int obj_type)
{
    /* If host based privacy is enabled */
    if (obj_type == BLE_STORE_OBJ_TYPE_PEER_DEV_REC) {
        return (MYNEWT_VAL(BLE_STORE_MAX_BONDS) + 1);
    } else {
        if (obj_type == BLE_STORE_OBJ_TYPE_CCCD) {
            return MYNEWT_VAL(BLE_STORE_MAX_CCCDS);
        } else {
            return MYNEWT_VAL(BLE_STORE_MAX_BONDS);
        }
    }
}

/*****************************************************************************
 * $ NVS                                                                     *
 *****************************************************************************/
#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
static int
get_nvs_peer_record(char *key_string, struct ble_hs_dev_records *p_dev_rec)
{
    esp_err_t err;
    size_t required_size = 0;
    nvs_handle_t nimble_handle;

    err = nvs_open(NIMBLE_NVS_NAMESPACE, NVS_READWRITE, &nimble_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "NVS open operation failed");
        return BLE_HS_ESTORE_FAIL;
    }

    err = nvs_get_blob(nimble_handle, key_string, NULL, &required_size);

    /* if Address pointer for value is NULL, filling of value not needed */
    if (err != ESP_OK || p_dev_rec == NULL) {
        goto end;
    }

    err = nvs_get_blob(nimble_handle, key_string, p_dev_rec,
                       &required_size);

end:
    nvs_close(nimble_handle);
    return err;
}
#endif

static int
get_nvs_db_value(int obj_type, char *key_string, union ble_store_value *val)
{
    esp_err_t err;
    size_t required_size = 0;
    nvs_handle_t nimble_handle;

    err = nvs_open(NIMBLE_NVS_NAMESPACE, NVS_READWRITE, &nimble_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "NVS open operation failed");
        return BLE_HS_ESTORE_FAIL;
    }

    err = nvs_get_blob(nimble_handle, key_string, NULL, &required_size);

    /* if Address pointer for value is NULL, filling of value not needed */
    if (err != ESP_OK || val == NULL) {
        goto end;
    }

    if (obj_type == BLE_STORE_OBJ_TYPE_CCCD) {
        err = nvs_get_blob(nimble_handle, key_string, &val->cccd,
                           &required_size);
    } else {
        err = nvs_get_blob(nimble_handle, key_string, &val->sec,
                           &required_size);
    }

end:
    nvs_close(nimble_handle);
    return err;
}

/* Finds empty index or total count or index to be deleted in NVS database
* This function serves 3 different purposes depending upon 'empty' and `value`
* arguments.
* @ returns             - empty NVS index, if empty = 1
*                       - count of NVS database, if empty = 0, value = NULL
*                       - index that does not match with RAM db, if empty = 0 &
*                         value has valid database address.
*/
static int
get_nvs_db_attribute(int obj_type, bool empty, void *value, int num_value)
{
    union ble_store_value cur = {0};
#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
    struct ble_hs_dev_records p_dev_rec = {0};
#endif
    esp_err_t err;
    int i, count = 0, max_limit = 0;
    char key_string[NIMBLE_NVS_STR_NAME_MAX_LEN];

    max_limit = get_nvs_max_obj_value(obj_type);

    for (i = 1; i <= max_limit; i++) {
        get_nvs_key_string(obj_type, i, key_string);

#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
        if (obj_type != BLE_STORE_OBJ_TYPE_PEER_DEV_REC) {
#endif
            err = get_nvs_db_value(obj_type, key_string, &cur);
#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
        } else {
            err = get_nvs_peer_record(key_string, &p_dev_rec);
        }
#endif
        /* Check if the user is searching for empty index to write to */
        if (err == ESP_ERR_NVS_NOT_FOUND) {
            if (empty) {
                ESP_LOGD(TAG, "Empty NVS index found = %d for obj_type = %d", i, obj_type);
                return i;
            }
        } else if (err == ESP_OK) {
            count++;
            /* If user has provided value, then the purpose is to find
             * non-matching entry from NVS */
            if (value) {
#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
                if (obj_type == BLE_STORE_OBJ_TYPE_PEER_DEV_REC) {
                    err = get_nvs_matching_index(&p_dev_rec, value, num_value,
                                                 sizeof(struct ble_hs_dev_records));
                } else
#endif
                {
                    if (obj_type != BLE_STORE_OBJ_TYPE_CCCD) {
                        err = get_nvs_matching_index(&cur.sec, value, num_value,
                                                     sizeof(struct ble_store_value_sec));
                    } else {
                        err = get_nvs_matching_index(&cur.cccd, value, num_value,
                                                     sizeof(struct ble_store_value_cccd));
                    }
                }
                /* If found non-matching/odd entry of NVS with entries in the
                 * internal database, return NVS index so can be deleted */
                if (err == -1 && !empty) {
                    return i;
                }
            }
        } else {
            ESP_LOGE(TAG, "NVS read operation failed while fetching size !!");
            return -1;
        }
    }

    if (empty == 0) {
        return count;
    } else {
        return (max_limit + 1);
    }
}

/* Deletes NVS value at given index
* @Returns               0 on success,
*                       -1 on NVS memory access failure
*/
static int
ble_nvs_delete_value(int obj_type, int8_t index)
{
    esp_err_t err;
    nvs_handle_t nimble_handle;
    char key_string[NIMBLE_NVS_STR_NAME_MAX_LEN];

    if (index > get_nvs_max_obj_value(obj_type)) {
        ESP_LOGE(TAG, "Invalid index provided to delete");
        return BLE_HS_EUNKNOWN;
    }

    err = nvs_open(NIMBLE_NVS_NAMESPACE, NVS_READWRITE, &nimble_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "NVS open operation failed !!");
        return BLE_HS_ESTORE_FAIL;
    }

    get_nvs_key_string(obj_type, index, key_string);

    /* Erase the key with given index */
    err = nvs_erase_key(nimble_handle, key_string);
    if (err != ESP_OK) {
        goto error;
    }
    err = nvs_commit(nimble_handle);
    if (err != ESP_OK) {
        goto error;
    }

    nvs_close(nimble_handle);
    return 0;
error:
    nvs_close(nimble_handle);
    return BLE_HS_ESTORE_FAIL;
}

static int
ble_nvs_write_key_value(char *key, const void *value, size_t required_size)
{
    nvs_handle_t nimble_handle;
    esp_err_t err;

    err = nvs_open(NIMBLE_NVS_NAMESPACE, NVS_READWRITE, &nimble_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "NVS open operation failed !!");
        return BLE_HS_ESTORE_FAIL;
    }

    err = nvs_set_blob(nimble_handle, key, value, required_size);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "NVS write operation failed !!");
        goto error;
    }

    /* NVS commit and close */
    err = nvs_commit(nimble_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "NVS commit operation failed !!");
        goto error;
    }

    nvs_close(nimble_handle);
    return 0;
error:
    nvs_close(nimble_handle);
    return BLE_HS_ESTORE_FAIL;
}

/* To write key value in NVS.
* @Returns              0 if success
*                       BLE_HS_ESTORE_FAIL if failure
*                       BLE_HS_ESTORE_CAP if no space in NVS
*/
static int
ble_store_nvs_write(int obj_type, const union ble_store_value *val)
{
    char key_string[NIMBLE_NVS_STR_NAME_MAX_LEN];
    int8_t write_key_index = 0;

    write_key_index = get_nvs_db_attribute(obj_type, 1, NULL, 0);
    if (write_key_index == -1) {
        ESP_LOGE(TAG, "NVS operation failed !!");
        return BLE_HS_ESTORE_FAIL;
    } else if (write_key_index > get_nvs_max_obj_value(obj_type)) {

        /* bare-bone config code will take care of capacity overflow event,
         * however another check added for consistency */
        ESP_LOGD(TAG, "NVS size overflow.");
        return BLE_HS_ESTORE_CAP;
    }

    get_nvs_key_string(obj_type, write_key_index, key_string);

    if (obj_type == BLE_STORE_OBJ_TYPE_CCCD) {
        return ble_nvs_write_key_value(key_string, &val->cccd, sizeof(struct
                                       ble_store_value_cccd));
    } else {
        return ble_nvs_write_key_value(key_string, &val->sec, sizeof(struct
                                       ble_store_value_sec));
    }
}

#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
/* If Host based privacy is enabled */
static int
ble_store_nvs_peer_records(int obj_type, const struct ble_hs_dev_records *p_dev_rec)
{
    char key_string[NIMBLE_NVS_STR_NAME_MAX_LEN];
    int8_t write_key_index = 0;

    write_key_index = get_nvs_db_attribute(obj_type, 1, NULL, 0);
    if (write_key_index == -1) {
        ESP_LOGE(TAG, "NVS operation failed !!");
        return BLE_HS_ESTORE_FAIL;
    } else if (write_key_index > get_nvs_max_obj_value(obj_type)) {

        /* bare-bone config code will take care of capacity overflow event,
         * however another check added for consistency */
        ESP_LOGD(TAG, "NVS size overflow.");
        return BLE_HS_ESTORE_CAP;
    }

    get_nvs_key_string(obj_type, write_key_index, key_string);

    return ble_nvs_write_key_value(key_string, p_dev_rec, sizeof(struct
                                   ble_hs_dev_records));
}
#endif

static int
populate_db_from_nvs(int obj_type, void *dst, int *db_num)
{
    uint8_t *db_item = (uint8_t *)dst;
    union ble_store_value cur = {0};
#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
    struct ble_hs_dev_records p_dev_rec = {0};
#endif

    esp_err_t err;
    int i;
    char key_string[NIMBLE_NVS_STR_NAME_MAX_LEN];

    for (i = 1; i <= get_nvs_max_obj_value(obj_type); i++) {
        get_nvs_key_string(obj_type, i, key_string);

#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
        if (obj_type != BLE_STORE_OBJ_TYPE_PEER_DEV_REC) {
#endif
            err = get_nvs_db_value(obj_type, key_string, &cur);
            if (err == ESP_ERR_NVS_NOT_FOUND) {
                continue;
            } else if (err != ESP_OK) {
                ESP_LOGE(TAG, "NVS read operation failed !!");
                return -1;
            }
#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
        } else {
            err = get_nvs_peer_record(key_string, &p_dev_rec);
            if (err == ESP_ERR_NVS_NOT_FOUND) {
                continue;
            } else if (err != ESP_OK) {
                ESP_LOGE(TAG, "NVS read operation failed !!");
                return -1;
            }
        }

        /* NVS index has data, fill up the ram db with it */
        if (obj_type == BLE_STORE_OBJ_TYPE_PEER_DEV_REC) {
            ESP_LOGD(TAG, "Peer dev records filled from NVS index = %d", i);
            memcpy(db_item, &p_dev_rec, sizeof(struct ble_hs_dev_records));
            db_item += sizeof(struct ble_hs_dev_records);
            (*db_num)++;
        } else
#endif
        {
            if (obj_type == BLE_STORE_OBJ_TYPE_CCCD) {
                ESP_LOGD(TAG, "CCCD in RAM is filled up from NVS index = %d", i);
                memcpy(db_item, &cur.cccd, sizeof(struct ble_store_value_cccd));
                db_item += sizeof(struct ble_store_value_cccd);
                (*db_num)++;
            } else {
                ESP_LOGD(TAG, "KEY in RAM is filled up from NVS index = %d", i);
                memcpy(db_item, &cur.sec, sizeof(struct ble_store_value_sec));
                db_item += sizeof(struct ble_store_value_sec);
                (*db_num)++;
            }
        }
    }
    return 0;
}

/* Gets the database in RAM filled up with keys stored in NVS. The sequence of
 * the keys in database may get lost.
 */
static int
ble_nvs_restore_sec_keys(void)
{
    esp_err_t err;

    err = populate_db_from_nvs(BLE_STORE_OBJ_TYPE_OUR_SEC, ble_store_config_our_secs,
                               &ble_store_config_num_our_secs);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "NVS operation failed for 'our sec'");
        return err;
    }
    ESP_LOGD(TAG, "ble_store_config_our_secs restored %d bonds", ble_store_config_num_our_secs);

    err = populate_db_from_nvs(BLE_STORE_OBJ_TYPE_PEER_SEC, ble_store_config_peer_secs,
                               &ble_store_config_num_peer_secs);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "NVS operation failed for 'peer sec'");
        return err;
    }
    ESP_LOGD(TAG, "ble_store_config_peer_secs restored %d bonds",
             ble_store_config_num_peer_secs);

    err = populate_db_from_nvs(BLE_STORE_OBJ_TYPE_CCCD, ble_store_config_cccds,
                               &ble_store_config_num_cccds);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "NVS operation failed for 'CCCD'");
        return err;
    }
    ESP_LOGD(TAG, "ble_store_config_cccds restored %d bonds",
             ble_store_config_num_cccds);

    return 0;
}

#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
static int
ble_nvs_restore_peer_records(void)
{
    esp_err_t err;
    int ble_store_num_peer_dev_rec = 0;
    struct ble_hs_dev_records *peer_dev_rec = ble_rpa_get_peer_dev_records();

    err = populate_db_from_nvs(BLE_STORE_OBJ_TYPE_PEER_DEV_REC, peer_dev_rec,
                               &ble_store_num_peer_dev_rec);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "NVS operation failed fetching 'Peer Dev Records'");
        return err;
    }

    ble_rpa_set_num_peer_dev_records(ble_store_num_peer_dev_rec);
    ESP_LOGD(TAG, "peer_dev_rec restored %d records", ble_store_num_peer_dev_rec);

    return 0;
}
#endif

int ble_store_config_persist_cccds(void)
{
    int nvs_count, nvs_idx;
    union ble_store_value val;

    nvs_count = get_nvs_db_attribute(BLE_STORE_OBJ_TYPE_CCCD, 0, NULL, 0);
    if (nvs_count == -1) {
        ESP_LOGE(TAG, "NVS operation failed while persisting CCCD");
        return BLE_HS_ESTORE_FAIL;
    }

    if (nvs_count < ble_store_config_num_cccds) {

        /* NVS db count less than RAM count, write operation */
        ESP_LOGD(TAG, "Persisting CCCD value in NVS...");
        val.cccd = ble_store_config_cccds[ble_store_config_num_cccds - 1];
        return ble_store_nvs_write(BLE_STORE_OBJ_TYPE_CCCD, &val);
    } else if (nvs_count > ble_store_config_num_cccds) {
        /* NVS db count more than RAM count, delete operation */
        nvs_idx = get_nvs_db_attribute(BLE_STORE_OBJ_TYPE_CCCD, 0,
                                       ble_store_config_cccds, ble_store_config_num_cccds);
        if (nvs_idx == -1) {
            ESP_LOGE(TAG, "NVS delete operation failed for CCCD");
            return BLE_HS_ESTORE_FAIL;
        }
        ESP_LOGD(TAG, "Deleting CCCD, nvs idx = %d", nvs_idx);
        return ble_nvs_delete_value(BLE_STORE_OBJ_TYPE_CCCD, nvs_idx);
    }
    return 0;
}

int ble_store_config_persist_peer_secs(void)
{
    int nvs_count, nvs_idx;
    union ble_store_value val;

    nvs_count = get_nvs_db_attribute(BLE_STORE_OBJ_TYPE_PEER_SEC, 0, NULL, 0);
    if (nvs_count == -1) {
        ESP_LOGE(TAG, "NVS operation failed while persisting peer sec");
        return BLE_HS_ESTORE_FAIL;
    }

    if (nvs_count < ble_store_config_num_peer_secs) {

        /* NVS db count less than RAM count, write operation */
        ESP_LOGD(TAG, "Persisting peer sec value in NVS...");
        val.sec = ble_store_config_peer_secs[ble_store_config_num_peer_secs - 1];
        return ble_store_nvs_write(BLE_STORE_OBJ_TYPE_PEER_SEC, &val);
    } else if (nvs_count > ble_store_config_num_peer_secs) {
        /* NVS db count more than RAM count, delete operation */
        nvs_idx = get_nvs_db_attribute(BLE_STORE_OBJ_TYPE_PEER_SEC, 0,
                                       ble_store_config_peer_secs, ble_store_config_num_peer_secs);
        if (nvs_idx == -1) {
            ESP_LOGE(TAG, "NVS delete operation failed for peer sec");
            return BLE_HS_ESTORE_FAIL;
        }
        ESP_LOGD(TAG, "Deleting peer sec, nvs idx = %d", nvs_idx);
        return ble_nvs_delete_value(BLE_STORE_OBJ_TYPE_PEER_SEC, nvs_idx);
    }
    return 0;
}

int ble_store_config_persist_our_secs(void)
{
    int nvs_count, nvs_idx;
    union ble_store_value val;

    nvs_count = get_nvs_db_attribute(BLE_STORE_OBJ_TYPE_OUR_SEC, 0, NULL, 0);
    if (nvs_count == -1) {
        ESP_LOGE(TAG, "NVS operation failed while persisting our sec");
        return BLE_HS_ESTORE_FAIL;
    }

    if (nvs_count < ble_store_config_num_our_secs) {

        /* NVS db count less than RAM count, write operation */
        ESP_LOGD(TAG, "Persisting our sec value to NVS...");
        val.sec = ble_store_config_our_secs[ble_store_config_num_our_secs - 1];
        return ble_store_nvs_write(BLE_STORE_OBJ_TYPE_OUR_SEC, &val);
    } else if (nvs_count > ble_store_config_num_our_secs) {
        /* NVS db count more than RAM count, delete operation */
        nvs_idx = get_nvs_db_attribute(BLE_STORE_OBJ_TYPE_OUR_SEC, 0,
                                       ble_store_config_our_secs, ble_store_config_num_our_secs);
        if (nvs_idx == -1) {
            ESP_LOGE(TAG, "NVS delete operation failed for our sec");
            return BLE_HS_ESTORE_FAIL;
        }
        ESP_LOGD(TAG, "Deleting our sec, nvs idx = %d", nvs_idx);
        return ble_nvs_delete_value(BLE_STORE_OBJ_TYPE_OUR_SEC, nvs_idx);
    }
    return 0;
}

#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
int ble_store_persist_peer_records(void)
{
    int nvs_count, nvs_idx;
    struct ble_hs_dev_records peer_rec;
    int ble_store_num_peer_dev_rec = ble_rpa_get_num_peer_dev_records();
    struct ble_hs_dev_records *peer_dev_rec = ble_rpa_get_peer_dev_records();

    nvs_count = get_nvs_db_attribute(BLE_STORE_OBJ_TYPE_PEER_DEV_REC, 0, NULL, 0);
    if (nvs_count == -1) {
        ESP_LOGE(TAG, "NVS operation failed while persisting peer_dev_rec");
        return BLE_HS_ESTORE_FAIL;
    }

    if (nvs_count < ble_store_num_peer_dev_rec) {

        /* NVS db count less than RAM count, write operation */
        ESP_LOGD(TAG, "Persisting peer dev record to NVS...");
        peer_rec = peer_dev_rec[ble_store_num_peer_dev_rec - 1];
        return ble_store_nvs_peer_records(BLE_STORE_OBJ_TYPE_PEER_DEV_REC, &peer_rec);
    } else if (nvs_count > ble_store_num_peer_dev_rec) {
        /* NVS db count more than RAM count, delete operation */
        nvs_idx = get_nvs_db_attribute(BLE_STORE_OBJ_TYPE_PEER_DEV_REC, 0,
                                       peer_dev_rec,
                                       ble_store_num_peer_dev_rec);
        if (nvs_idx == -1) {
            ESP_LOGE(TAG, "NVS delete operation failed for peer records");
            return BLE_HS_ESTORE_FAIL;
        }
        ESP_LOGD(TAG, "Deleting peer record, nvs idx = %d", nvs_idx);
        return ble_nvs_delete_value(BLE_STORE_OBJ_TYPE_PEER_DEV_REC, nvs_idx);
    }
    return 0;
}
#endif

void ble_store_config_conf_init(void)
{
    int err;

    err = ble_nvs_restore_sec_keys();
    if (err != 0) {
        ESP_LOGE(TAG, "NVS operation failed, can't retrieve the bonding info");
    }
#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
    err = ble_nvs_restore_peer_records();
    if (err != 0) {
        ESP_LOGE(TAG, "NVS operation failed, can't retrieve the peer records");
    }
#endif
}

/***************************************************************************************/
#endif /* MYNEWT_VAL(BLE_STORE_CONFIG_PERSIST) */
