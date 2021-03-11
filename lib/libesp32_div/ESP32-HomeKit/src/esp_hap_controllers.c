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

#include <string.h>
#include <hap.h>
#include <esp_mfi_debug.h>

#include <esp_hap_main.h>
#include <esp_hap_database.h>
#include <esp_hap_controllers.h>
#include <esp_hap_keystore.h>

#define HAP_KEYSTORE_NAMESPACE_CTRL "hap_ctrl"

int hap_controllers_init()
{
	memset(hap_priv.controllers, 0, sizeof(hap_priv.controllers));
    char index_str[4];
    uint8_t i;
    size_t info_size;
    bool acc_paired = false;
	for (i = 0; i < HAP_MAX_CONTROLLERS; i++) {
        snprintf(index_str, sizeof(index_str), "%d", i);
        info_size = sizeof(hap_ctrl_info_t);
        if (hap_keystore_get(HAP_KEYSTORE_NAMESPACE_CTRL, index_str,
                    (uint8_t *)&hap_priv.controllers[i].info, &info_size) == HAP_SUCCESS) {
            if (info_size == sizeof(hap_ctrl_info_t)) {
                hap_priv.controllers[i].index = i;
                hap_priv.controllers[i].valid = true;
                acc_paired = true;
            }
        }
    }
    if (acc_paired) {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Accessory is Paired with atleast one controller");
    } else {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Accessory is not Paired with any controller");
    }
	return HAP_SUCCESS;
}

hap_ctrl_data_t *hap_controller_get_empty_loc()
{
	int i;
	for (i = 0; i < HAP_MAX_CONTROLLERS; i++) {
		if (!hap_priv.controllers[i].valid) {
            hap_priv.controllers[i].index = i;
			return &hap_priv.controllers[i];
        }
	}
	return NULL;
}

int hap_get_paired_controller_count()
{
	int i, cnt = 0;
	for (i = 0; i < HAP_MAX_CONTROLLERS; i++) {
		if (hap_priv.controllers[i].valid) {
            cnt++;
        }
	}
	return cnt;
}

bool is_accessory_paired()
{
	int i;
	for (i = 0; i < HAP_MAX_CONTROLLERS; i++) {
		if (hap_priv.controllers[i].valid)
			return true;
	}
	return false;
}

bool is_admin_paired()
{
	int i;
	for (i = 0; i < HAP_MAX_CONTROLLERS; i++) {
		if (hap_priv.controllers[i].valid && hap_priv.controllers[i].info.perms)
			return true;
	}
	return false;
}

int hap_controller_save(hap_ctrl_data_t *ctrl_data)
{
	ctrl_data->valid = true;
    char index_str[4];
    snprintf(index_str, sizeof(index_str), "%d", ctrl_data->index);
    int ret = hap_keystore_set(HAP_KEYSTORE_NAMESPACE_CTRL, index_str,
            (const uint8_t *)&ctrl_data->info, (size_t)sizeof(hap_ctrl_info_t));

    if (ret != HAP_SUCCESS) {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Failed to store controller %d", ctrl_data->index);
        return HAP_FAIL;
    }
    hap_report_event(HAP_EVENT_CTRL_PAIRED, ctrl_data->info.id, sizeof(ctrl_data->info.id));
    return HAP_SUCCESS;
}

void hap_controller_remove(hap_ctrl_data_t *ctrl_data)
{
	if (!ctrl_data)
        return;
    char index_str[4];
    snprintf(index_str, sizeof(index_str), "%d", ctrl_data->index);
    char id[HAP_CTRL_ID_LEN];
    strncpy(id, ctrl_data->info.id, sizeof(id));
    hap_keystore_delete(HAP_KEYSTORE_NAMESPACE_CTRL, index_str);
    memset(ctrl_data, 0, sizeof(hap_ctrl_data_t));
    hap_report_event(HAP_EVENT_CTRL_UNPAIRED, id, sizeof(id));
}

hap_ctrl_data_t *hap_get_controller(char *ctrl_id)
{
	int i;
	for (i = 0; i < HAP_MAX_CONTROLLERS; i++) {
		if (hap_priv.controllers[i].valid
				&& (!strcmp(hap_priv.controllers[i].info.id, ctrl_id)))
			return &hap_priv.controllers[i];
	}
	return NULL;
}

void hap_erase_controller_info()
{
    hap_keystore_delete_namespace(HAP_KEYSTORE_NAMESPACE_CTRL);
}
