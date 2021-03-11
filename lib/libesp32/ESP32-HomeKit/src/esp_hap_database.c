/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2020 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS products only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <stdio.h>
#include <sodium/crypto_sign_ed25519.h>
#include <string.h>
#include <hap_platform_memory.h>

#include <esp_mfi_rand.h>
#include <esp_mfi_sha.h>
#include <esp_mfi_debug.h>

#include <esp_hap_main.h>
#include <esp_hap_keystore.h>
#include <esp_hap_database.h>
#include <esp_hap_controllers.h>

#include <esp_mfi_base64.h>

#define HAP_KEY_ACC_ID                  "acc_id"
#define HAP_KEY_LTSKA                   "ltska"
#define HAP_KEY_LTPKA                   "ltpka"
#define HAP_KEY_CONFIG_NUM              "config_num"
#define HAP_KEY_FW_REV                  "fw_rev"
#define HAP_KEY_CUR_AID                 "cur_aid"
#define HAP_KEY_STATE_NUM              "state_num"

#define HAP_KEY_SETUP_ID                "setup_id"
#define HAP_KEY_SETUP_SALT              "setup_salt"
#define HAP_KEY_SETUP_VERIFIER          "setup_verifier"

#define HAP_LOOP_STACK              (4 * 1024)
#define HAP_MAIN_THREAD_PRIORITY    7
#define HAP_MAX_NOTIF_CHARS         8
#define HAP_SOCK_RECV_TIMEOUT       10
#define HAP_SOCK_SEND_TIMEOUT       10

hap_priv_t hap_priv = {
    .cfg = {
        .task_stack_size = HAP_LOOP_STACK,
        .task_priority = HAP_MAIN_THREAD_PRIORITY,
        .max_event_notif_chars = HAP_MAX_NOTIF_CHARS,
        .unique_param = UNIQUE_SSID,
        .recv_timeout = HAP_SOCK_RECV_TIMEOUT,
        .send_timeout = HAP_SOCK_SEND_TIMEOUT,
        .sw_token_max_len = HAP_SW_TOKEN_MAX_LEN,
    }
};

static void hap_save_config_number()
{

    hap_keystore_set(HAP_KEYSTORE_NAMESPACE_HAPMAIN, HAP_KEY_CONFIG_NUM,
            (uint8_t *)&hap_priv.config_num, sizeof(hap_priv.config_num));
}

static void hap_get_config_number()
{
    size_t config_num_len = sizeof(hap_priv.config_num);
    if (hap_keystore_get(HAP_KEYSTORE_NAMESPACE_HAPMAIN, HAP_KEY_CONFIG_NUM,
                (uint8_t *)&hap_priv.config_num, &config_num_len) != HAP_SUCCESS) {
        hap_priv.config_num = 1;
        hap_save_config_number();
    }
    if (hap_priv.config_num > 65535) {
        hap_priv.config_num = 1;
        hap_save_config_number();
    }
}

void hap_increment_and_save_config_num()
{
    hap_priv.config_num++;
    if (hap_priv.config_num > 65535) {
        hap_priv.config_num = 1;
    }
    hap_save_config_number();
}


static void hap_save_state_number()
{
    hap_keystore_set(HAP_KEYSTORE_NAMESPACE_HAPMAIN, HAP_KEY_STATE_NUM,
            (uint8_t *)&hap_priv.state_num, sizeof(hap_priv.state_num));
}

void hap_increment_and_save_state_num()
{
    if (is_accessory_paired()) {
        hap_priv.state_num++;
        /* If value becomes 0 after incrementing, it means that it has wrapped around.
         * So, reset to 1
         */
        if (hap_priv.state_num == 0) {
            hap_priv.state_num = 1;
        }
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Updated state number to %d", hap_priv.state_num);
        hap_save_state_number();
    }
}

static void hap_init_state_number()
{
    size_t state_num_len = sizeof(hap_priv.state_num);
    if (hap_keystore_get(HAP_KEYSTORE_NAMESPACE_HAPMAIN, HAP_KEY_STATE_NUM,
                (uint8_t *)&hap_priv.state_num, &state_num_len) != HAP_SUCCESS) {
        /* If state number is not found, initialise with 1 and store.
         */
        hap_priv.state_num = 1;
        hap_save_state_number();
    } else {
        hap_increment_and_save_state_num();
    }
}

static void hap_save_cur_aid()
{
    hap_keystore_set(HAP_KEYSTORE_NAMESPACE_HAPMAIN, HAP_KEY_CUR_AID,
            (uint8_t *)&hap_priv.cur_aid, sizeof(hap_priv.cur_aid));
}

static void hap_get_cur_aid()
{
    size_t aid_len = sizeof(hap_priv.cur_aid);
    if (hap_keystore_get(HAP_KEYSTORE_NAMESPACE_HAPMAIN, HAP_KEY_CUR_AID,
                (uint8_t *)&hap_priv.cur_aid, &aid_len) != HAP_SUCCESS) {
        /* AID = 1 is reserved for Primary Accessory. So, we set the initial
         * value to 1, so that the bridged accessories get assigned aid from
         * 2 onwards
         */
        hap_priv.cur_aid = 1;
        hap_save_cur_aid();
    }
}

static int hap_get_setup_id()
{
    /* Read setup id from NVS, only if it is not already set from the accessory code */
    if (!strlen(hap_priv.setup_id)) {
        size_t setup_id_len = sizeof(hap_priv.setup_id);
        if (hap_factory_keystore_get(HAP_FACTORY_NAMESPACE_HAP_SETUP, HAP_KEY_SETUP_ID,
                (uint8_t *)hap_priv.setup_id, &setup_id_len) != HAP_SUCCESS) {
            return HAP_FAIL;
        }
    }
    return HAP_SUCCESS;
}

static int hap_get_setup_info()
{
    /* If the setup code has been set directly, no need to check for setup info */
    if (hap_priv.setup_code) {
        return HAP_SUCCESS;
    }
    /* If the setup info has been set externally, directly from the accessory code,
     * no need to check in the NVS
     */
    if (!hap_priv.setup_info) {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Getting setup info from factory NVS");
        hap_priv.setup_info = hap_platform_memory_calloc(1, sizeof(hap_setup_info_t));
        if (!hap_priv.setup_info)
            return HAP_FAIL;
        size_t salt_len = sizeof(hap_priv.setup_info->salt);
        size_t verifier_len = sizeof(hap_priv.setup_info->verifier);
        int ret = hap_factory_keystore_get(HAP_FACTORY_NAMESPACE_HAP_SETUP, HAP_KEY_SETUP_SALT,
                hap_priv.setup_info->salt, &salt_len);
        ret |= hap_factory_keystore_get(HAP_FACTORY_NAMESPACE_HAP_SETUP, HAP_KEY_SETUP_VERIFIER,
                hap_priv.setup_info->verifier, &verifier_len);
        if (ret != HAP_SUCCESS) {
            hap_platform_memory_free(hap_priv.setup_info);
            hap_priv.setup_info = NULL;
            return HAP_FAIL;
        }
    }
    return HAP_SUCCESS;
}

static void hap_check_fw_version()
{
    char fw_rev[64] = {0};
    size_t fw_rev_len = sizeof(fw_rev);
    /* Check if the firmware revision is stored in NVS */
    if (hap_keystore_get(HAP_KEYSTORE_NAMESPACE_HAPMAIN, HAP_KEY_FW_REV,
                (uint8_t *)fw_rev, &fw_rev_len) == HAP_SUCCESS) {
        /* If the firmware revision is found, compare with the current revision.
         * If it is the same, no need to do anything. So, just return
         */
        if (strncmp(fw_rev, hap_priv.primary_acc.fw_rev, sizeof(fw_rev)) == 0) {
            return;
        } else {
            /* If there is a version mismatch, it means that the firmware was upgraded.
             * Update config number in that case
             */
            ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "FW Update detected. Incrementing config number");
            hap_increment_and_save_config_num();
        }
    }
    /* Save the new firmare revision to NVS */
    hap_keystore_set(HAP_KEYSTORE_NAMESPACE_HAPMAIN, HAP_KEY_FW_REV,
            (uint8_t *)hap_priv.primary_acc.fw_rev,
            strlen(hap_priv.primary_acc.fw_rev));
}

int hap_acc_setup_init()
{
    if (hap_get_setup_id() != HAP_SUCCESS) {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Setup ID absent");
        return HAP_FAIL;
    } else {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Setup ID: %s", hap_priv.setup_id);
    }

    if (hap_get_setup_info() != HAP_SUCCESS) {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Setup Info absent");
        return HAP_FAIL;
    }

    uint8_t digest[MFI_SHA512_SIZE] = {0};
    esp_mfi_sha_ctx_t ctx = 0;
    ctx = esp_mfi_sha512_new();
    if (!ctx) {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Out of Memory");
        return HAP_FAIL;
    }
    /* Compute setup hash by taking a SHA512 hash of the setup id and device id */
    esp_mfi_sha512_init(ctx);
    esp_mfi_sha512_update(ctx, (const uint8_t *)hap_priv.setup_id, strlen(hap_priv.setup_id));
    esp_mfi_sha512_update(ctx, (const uint8_t *)hap_priv.acc_id, strlen(hap_priv.acc_id));
    esp_mfi_sha512_final(ctx, digest);
    /* Copy only the first 4 bytes as the setup hash */
    memcpy(hap_priv.setup_hash, digest, SETUP_HASH_LEN);
    esp_mfi_sha512_free(ctx);

    int hash_size = sizeof(hap_priv.setup_hash_str);
    esp_mfi_base64_encode((const char *)hap_priv.setup_hash, SETUP_HASH_LEN,
            hap_priv.setup_hash_str, hash_size, &hash_size);

    hap_check_fw_version();

    return HAP_SUCCESS;
}

int hap_database_init(void)
{
    uint8_t id[6];
    size_t val_size = sizeof(id);
    if (hap_keystore_get(HAP_KEYSTORE_NAMESPACE_HAPMAIN, HAP_KEY_ACC_ID, id, &val_size) == HAP_SUCCESS) {
        val_size = sizeof(hap_priv.ltska);
        hap_keystore_get(HAP_KEYSTORE_NAMESPACE_HAPMAIN, HAP_KEY_LTSKA, hap_priv.ltska, &val_size);
        val_size = sizeof(hap_priv.ltpka);
        hap_keystore_get(HAP_KEYSTORE_NAMESPACE_HAPMAIN, HAP_KEY_LTPKA, hap_priv.ltpka, &val_size);
    } else {
        /* If the accessory ID is not found in keystore, create and store a new random ID */
	    esp_mfi_get_random(id, sizeof(id));
        hap_keystore_set(HAP_KEYSTORE_NAMESPACE_HAPMAIN, HAP_KEY_ACC_ID, id, sizeof(id));
        /* Also create a new ED25519 key pair */
	    esp_mfi_get_random(hap_priv.ltska, sizeof(hap_priv.ltska));
        crypto_sign_ed25519_keypair(hap_priv.ltpka, hap_priv.ltska);
        hap_keystore_set(HAP_KEYSTORE_NAMESPACE_HAPMAIN, HAP_KEY_LTSKA, hap_priv.ltska, sizeof(hap_priv.ltska));
        hap_keystore_set(HAP_KEYSTORE_NAMESPACE_HAPMAIN, HAP_KEY_LTPKA, hap_priv.ltpka, sizeof(hap_priv.ltpka));
    }

    memcpy(hap_priv.raw_acc_id, id, sizeof(hap_priv.raw_acc_id));
	snprintf(hap_priv.acc_id, sizeof(hap_priv.acc_id), "%02X:%02X:%02X:%02X:%02X:%02X",
			id[0], id[1], id[2], id[3], id[4], id[5]);

	hap_controllers_init();
    hap_get_config_number();
    hap_get_cur_aid();
    hap_init_state_number();
    ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Database initialised. Accessory Device ID: %s", hap_priv.acc_id);
	return HAP_SUCCESS;
}

char *hap_get_acc_id()
{
	return hap_priv.acc_id;
}

int hap_get_next_aid(char *id)
{
    hap_priv.cur_aid++;
    hap_save_cur_aid();
    return hap_priv.cur_aid;
}

void hap_erase_accessory_info()
{
    hap_keystore_delete_namespace(HAP_KEYSTORE_NAMESPACE_HAPMAIN);
}

void hap_configure_unique_param(hap_unique_param_t param)
{
    hap_priv.cfg.unique_param = param;
}

int hap_get_config(hap_cfg_t *cfg)
{
    if (!cfg) {
        return HAP_FAIL;
    }
    *cfg = hap_priv.cfg;
    return HAP_SUCCESS;
}

int hap_set_config(const hap_cfg_t *cfg)
{
    if (!cfg) {
        return HAP_FAIL;
    }
    hap_priv.cfg = *cfg;
    return HAP_SUCCESS;
}
