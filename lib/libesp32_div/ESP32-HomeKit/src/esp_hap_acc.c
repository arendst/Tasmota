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
#include <esp_wifi.h>
#include <hap_platform_memory.h>
#include <esp_hap_acc.h>
#include <esp_mfi_debug.h>
#include <esp_mfi_debug.h>

#include <hap_apple_servs.h>
#include <hap_apple_chars.h>
#include <esp_hap_database.h>
#include <esp_hap_keystore.h>
#include <esp_hap_main.h>

/* Primary Accessory Pointer */
static __hap_acc_t *primary_acc;

/*****************************************************************************************************/

hap_acc_t *hap_get_first_acc()
{
    return (hap_acc_t *)primary_acc;
}

hap_acc_t *hap_acc_get_next(hap_acc_t *ha)
{
    if (!ha)
        return NULL;

    __hap_acc_t *_ha = (__hap_acc_t *)ha;
    return (hap_acc_t *)_ha->next;
}
/* Service write callback to handle "Identify"  */
static int hap_acc_info_write(hap_write_data_t write_data[], int count,
        void *serv_priv, void *write_priv)
{
	int i;
    __hap_char_t *_hc;
	for (i = 0; i < count; i++) {
        _hc = (__hap_char_t *)write_data[i].hc;
		if (!strcmp(_hc->type_uuid, HAP_CHAR_UUID_IDENTIFY)) {
            __hap_acc_t *_ha = (__hap_acc_t *)serv_priv;
            if (_ha) {
                _ha->identify_routine((hap_acc_t *)_ha);
                *(write_data->status) = HAP_STATUS_SUCCESS;
                continue;
            }
        }
        *(write_data->status) = HAP_STATUS_VAL_INVALID;
	}
	return HAP_SUCCESS;
}

/**
 * @brief HAP create an accessory
 */
hap_acc_t *hap_acc_create(hap_acc_cfg_t *acc_cfg)
{
    static bool first = true;
    int ret = 0;
    __hap_acc_t *_ha = hap_platform_memory_calloc(1, sizeof(__hap_acc_t));
    if (!_ha) {
        return NULL;
    }

    _ha->identify_routine = acc_cfg->identify_routine;
    _ha->next_iid = 1;

    /* Add the Accessory Information Service internally */
    hap_serv_t *hs = hap_serv_create("3E");
    if (!hs) {
        goto acc_create_fail;
    }
    ret = hap_serv_add_char(hs, hap_char_bool_create(HAP_CHAR_UUID_IDENTIFY, HAP_CHAR_PERM_PW, false));
    ret |= hap_serv_add_char(hs, hap_char_string_create(HAP_CHAR_UUID_MANUFACTURER, HAP_CHAR_PERM_PR, acc_cfg->manufacturer));
    ret |= hap_serv_add_char(hs, hap_char_string_create(HAP_CHAR_UUID_MODEL, HAP_CHAR_PERM_PR, acc_cfg->model));
    ret |= hap_serv_add_char(hs, hap_char_string_create(HAP_CHAR_UUID_NAME, HAP_CHAR_PERM_PR, acc_cfg->name));
    ret |= hap_serv_add_char(hs, hap_char_string_create(HAP_CHAR_UUID_SERIAL_NUMBER, HAP_CHAR_PERM_PR, acc_cfg->serial_num));
    ret |= hap_serv_add_char(hs, hap_char_string_create(HAP_CHAR_UUID_FIRMWARE_REVISION, HAP_CHAR_PERM_PR, acc_cfg->fw_rev));
    if (acc_cfg->hw_rev) {
        ret |= hap_serv_add_char(hs, hap_char_string_create(HAP_CHAR_UUID_HARDWARE_REVISION, HAP_CHAR_PERM_PR, acc_cfg->hw_rev));
    }
    
    if (ret) {
        goto acc_create_fail;
    }

    hap_serv_set_write_cb(hs, hap_acc_info_write);
    hap_serv_set_priv(hs,(void *)_ha);
    hap_acc_add_serv((hap_acc_t *)_ha, hs);

    if (first) {
        /* Add the Procol Information Service Internally */
        hs = hap_serv_create("A2");
        if (!hs) {
            goto acc_create_fail;
        }
        ret = hap_serv_add_char(hs, hap_char_string_create("37", HAP_CHAR_PERM_PR, "1.1.0"));

        if (ret) {
            goto acc_create_fail;
        }
        hap_acc_add_serv((hap_acc_t *)_ha, hs);
        hap_priv.cid = acc_cfg->cid;
        first = false;
    }

    return (hap_acc_t *)_ha;

acc_create_fail:
    hap_acc_delete((hap_acc_t *)_ha);
    return NULL;
}

int hap_acc_add_accessory_flags(hap_acc_t *ha, uint32_t flags)
{
    if (!ha) {
        return HAP_FAIL;
    }
    hap_serv_t *hs = hap_acc_get_serv_by_uuid(ha, HAP_SERV_UUID_ACCESSORY_INFORMATION);
    if (!hs) {
        return HAP_FAIL;
    }
    return hap_serv_add_char(hs, hap_char_accessory_flags_create(flags));

}

int hap_acc_update_accessory_flags(hap_acc_t *ha, uint32_t flags)
{
    if (!ha) {
        return HAP_FAIL;
    }
    hap_serv_t *hs = hap_acc_get_serv_by_uuid(ha, HAP_SERV_UUID_ACCESSORY_INFORMATION);
    if (!hs) {
        return HAP_FAIL;
    }
    hap_char_t *hc = hap_serv_get_char_by_uuid(hs, HAP_CHAR_UUID_ACCESSORY_FLAGS);
    if (!hc) {
        return HAP_FAIL;
    }
    hap_val_t val = {
        .u = flags,
    };
    return hap_char_update_val(hc, &val);
}

int hap_acc_add_product_data(hap_acc_t *ha, uint8_t *product_data, size_t data_size)
{
    if (!ha) {
        return HAP_FAIL;
    }
    hap_serv_t *hs = hap_acc_get_serv_by_uuid(ha, HAP_SERV_UUID_ACCESSORY_INFORMATION);
    if (!hs) {
        return HAP_FAIL;
    }
    if (data_size != 8) {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Product data size is not 8");
    }
    uint8_t *buf = calloc(1, data_size);
    if (!buf) {
        return HAP_FAIL;
    }
    memcpy(buf, product_data, data_size);
    hap_data_val_t data_val = {
        .buf = buf,
        .buflen = data_size
    };
    return hap_serv_add_char(hs, hap_char_product_data_create(&data_val));
}

const hap_val_t *hap_get_product_data()
{
    hap_char_t *acc_info = hap_acc_get_serv_by_uuid(hap_get_first_acc(), HAP_SERV_UUID_ACCESSORY_INFORMATION);
    if (acc_info) {
        hap_char_t *product_data = hap_serv_get_char_by_uuid(acc_info, HAP_CHAR_UUID_PRODUCT_DATA);
        if (product_data) {
            return hap_char_get_val(product_data);
        }
    }
    return NULL;
}
/**
 * @brief check if accessory's AID matches the target AID
 */
bool hap_check_aid(__hap_acc_t *accessory, int32_t aid)
{
    return accessory->aid == aid ? true : false;
}

hap_serv_t *hap_acc_get_first_serv(hap_acc_t *ha)
{
    return ((__hap_acc_t *)ha)->servs;
}
/**
 * @brief get target service by it's type description string
 */
hap_serv_t *hap_acc_get_serv_by_iid(hap_acc_t *ha, int32_t iid)
{
    if (!ha)
        return NULL;

    hap_serv_t *hs;
    for (hs = hap_acc_get_first_serv(ha); hs; hs = hap_serv_get_next(hs)) {
        if (((__hap_serv_t *)hs)->iid == iid)
            return hs;
    }
    return NULL;
}

hap_serv_t *hap_acc_get_serv_by_uuid(hap_acc_t *ha, const char *uuid)
{
    if (!ha)
        return NULL;

    hap_serv_t *hs;
    for (hs = hap_acc_get_first_serv(ha); hs; hs = hap_serv_get_next(hs)) {
        if (!strcmp(((__hap_serv_t *)hs)->type_uuid, uuid))
            return hs;
    }
    return NULL;
}

/**
 * @brief get target characteristics by it's IID
 */
hap_char_t *hap_acc_get_char_by_iid(hap_acc_t *ha, int32_t iid)
{
    if (!ha)
        return NULL;
    hap_serv_t *hs;
    hap_char_t *hc;
    for (hs = hap_acc_get_first_serv(ha); hs; hs = hap_serv_get_next(hs)) {
        hc = hap_serv_get_char_by_iid(hs, iid);
        if (hc)
            return hc;
    }
    return NULL;
}

int hap_acc_get_info(hap_acc_cfg_t *acc_cfg)
{
    ESP_MFI_ASSERT(acc_cfg);
    hap_acc_t *ha = hap_get_first_acc();

    ESP_MFI_ASSERT(ha);

    hap_serv_t *hs = hap_acc_get_serv_by_uuid(ha, HAP_SERV_UUID_ACCESSORY_INFORMATION);

    hap_char_t *hc = hap_serv_get_char_by_uuid(hs, HAP_CHAR_UUID_NAME);
    acc_cfg->name = ((__hap_char_t *)hc)->val.s;
    
    hc = hap_serv_get_char_by_uuid(hs, HAP_CHAR_UUID_MODEL);
    acc_cfg->model = ((__hap_char_t *)hc)->val.s;
    
    hc = hap_serv_get_char_by_uuid(hs, HAP_CHAR_UUID_MANUFACTURER);
    acc_cfg->manufacturer = ((__hap_char_t *)hc)->val.s;

    hc = hap_serv_get_char_by_uuid(hs, HAP_CHAR_UUID_SERIAL_NUMBER);
    acc_cfg->serial_num = ((__hap_char_t *)hc)->val.s;
    
    hc = hap_serv_get_char_by_uuid(hs, HAP_CHAR_UUID_FIRMWARE_REVISION);
    acc_cfg->fw_rev = ((__hap_char_t *)hc)->val.s;

    hc = hap_serv_get_char_by_uuid(hs, HAP_CHAR_UUID_HARDWARE_REVISION);
    if (hc) {
        acc_cfg->hw_rev = ((__hap_char_t *)hc)->val.s;
    } else {
        acc_cfg->hw_rev = NULL;
    }

    hs = hap_acc_get_serv_by_uuid(ha, HAP_SERV_UUID_PROTOCOL_INFORMATION);

    hc = hap_serv_get_char_by_uuid(hs, HAP_CHAR_UUID_VERSION);
    acc_cfg->pv = ((__hap_char_t *)hc)->val.s;

    return 0;
}

/**
 * @brief add a characteristics to a service
 */
int hap_acc_add_serv(hap_acc_t *ha, hap_serv_t *hs)
{
    ESP_MFI_ASSERT(ha);
    ESP_MFI_ASSERT(hs);
    __hap_acc_t *_ha = (__hap_acc_t *)ha;
    __hap_serv_t *_hs = (__hap_serv_t *)hs;

    if (_hs->parent) {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Service already added");
        return HAP_FAIL;
    }

    /* If the accessory has no services, add this as the first */
    if (!_ha->servs) {
        _ha->servs = hs;
    } else {
        /* Else loop through the services to get to the last one,
         * and add this at the end
         */
        __hap_serv_t *temp = (__hap_serv_t *)_ha->servs;
        while (temp->next_serv)
            temp = (__hap_serv_t *)temp->next_serv;
        temp->next_serv = hs;
    }
	_hs->iid = _ha->next_iid++;
	__hap_char_t *_hc = (__hap_char_t *)_hs->chars;
	while(_hc) {
		_hc->iid = _ha->next_iid++;
		_hc = (__hap_char_t *)_hc->next_char;
	}
    _hs->parent = ha;
    return 0;
}

static void hap_add_acc_to_list(__hap_acc_t *primary, __hap_acc_t *new)
{
    __hap_acc_t *cur = primary;
    while (cur->next) {
        cur = cur->next;
    }
    cur->next = new;
}

static void hap_remove_acc_from_list(__hap_acc_t *primary, __hap_acc_t *old)
{
    __hap_acc_t *cur = primary;
    while (cur->next != old) {
        cur = cur->next;
    }
    cur->next = cur->next->next;
}

#define HAP_BRIDGE_KEYSTORE     "hap_bridge"

int hap_get_unique_aid(const char *id)
{
    if (!id) {
        return -1;
    }
    int aid = 0;
    size_t aid_size = sizeof(aid);
    if (hap_keystore_get(HAP_KEYSTORE_NAMESPACE_HAPMAIN, id, (uint8_t *)&aid, &aid_size) != HAP_SUCCESS) {
        aid = hap_get_next_aid();
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Assigning aid = %d for Bridged accessory %s", aid, id);
        hap_keystore_set(HAP_KEYSTORE_NAMESPACE_HAPMAIN, id, (uint8_t *)&aid, sizeof(aid));
    } else {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Using aid = %d for Bridged accessory %s", aid, id);
    }
    return aid;
}

/**
 * @brief HAP add accessory to HAP kernel
 */
void hap_add_accessory(hap_acc_t *ha)
{
    if (!ha) {
        return;
    }
    if (primary_acc) {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Primary Accessory already added. Use hap_add_bridged_accessory() instead");
        return;
    }
    __hap_acc_t *_ha = (__hap_acc_t *)ha;
    _ha->aid = 1;
    primary_acc = _ha;
    if (hap_priv.cfg.unique_param >= UNIQUE_NAME) {
        char name[74];
        uint8_t eth_mac[6];
        esp_wifi_get_mac(WIFI_IF_STA, eth_mac);
        hap_serv_t *hs = hap_acc_get_serv_by_uuid(ha, HAP_SERV_UUID_ACCESSORY_INFORMATION);
        hap_char_t *hc = hap_serv_get_char_by_uuid(hs, HAP_CHAR_UUID_NAME);
        snprintf(name, sizeof(name), "%s-%02X%02X%02X", ((__hap_char_t *)hc)->val.s,
                eth_mac[3], eth_mac[4], eth_mac[5]);
        hap_platform_memory_free(((__hap_char_t *)hc)->val.s);
        ((__hap_char_t *)hc)->val.s = strdup(name);
    }
    hap_acc_get_info(&hap_priv.primary_acc);
}

void hap_add_bridged_accessory(hap_acc_t *ha, int aid)
{
    if (!ha) {
        return;
    }
    __hap_acc_t *_ha = (__hap_acc_t *)ha;
    if (aid) {
        _ha->aid = aid;
    } else {
        _ha->aid = hap_get_next_aid();
    }

    hap_add_acc_to_list(primary_acc, _ha);
    if (!hap_priv.cfg.disable_config_num_update) {
        hap_update_config_number();
    }
}

void hap_remove_bridged_accessory(hap_acc_t *ha)
{
    if ((__hap_acc_t *)ha == primary_acc) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Cannot remove primary accessory");
    } else {
        if (ha) {
            hap_remove_acc_from_list(primary_acc, (__hap_acc_t *)ha);
            if (!hap_priv.cfg.disable_config_num_update) {
                hap_update_config_number();
            }
        }

    }
}
/**
 * @brief HAP delete target accessory
 */
void hap_acc_delete(hap_acc_t *ha)
{
	/* Returning success even if pointer is NULL, because it means
	 * that the accessory is absent and as good as deleted
	 */
	if (!ha)
		return;
	__hap_acc_t *_ha = (__hap_acc_t *)ha;
	__hap_serv_t *_hs = (__hap_serv_t *)_ha->servs;
	while (_hs) {
		_ha->servs = _hs->next_serv;
		hap_serv_delete((hap_serv_t *)_hs);
		_hs = (__hap_serv_t *)_ha->servs;
	}
    hap_platform_memory_free(_ha);
}

/**
 * @brief HAP get target accessory AID
 */
uint32_t hap_acc_get_aid(hap_acc_t *ha)
{
    ESP_MFI_ASSERT(ha);
    __hap_acc_t *_ha = (__hap_acc_t *)ha;

    return _ha->aid;
}

/**
 * @brief delete all accessories
 */
void hap_delete_all_accessories(void)
{
    __hap_acc_t *next, *ha = primary_acc;
    while (ha) {
        next = ha->next;
        hap_acc_delete((hap_acc_t *)ha);
        ha = next;
    }
}
/**
 * @brief get target accessory by AID
 */
hap_acc_t *hap_acc_get_by_aid(int32_t aid)
{
	hap_acc_t *ha;
	for (ha = hap_get_first_acc(); ha; ha = hap_acc_get_next(ha)) {
        if (((__hap_acc_t *)ha)->aid == aid) {
            return ha;
        }
    }
    return NULL;
}
