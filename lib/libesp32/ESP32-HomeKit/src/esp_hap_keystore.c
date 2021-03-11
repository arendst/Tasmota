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
#include <hap.h>
#include <esp_mfi_debug.h>
#include <hap_platform_keystore.h>

static bool keystore_init_done;
static char *hap_platform_nvs_partition;
static char *hap_platform_factory_nvs_partition;

int hap_keystore_init()
{
    if (keystore_init_done) {
        return HAP_SUCCESS;
    }



    hap_platform_nvs_partition = hap_platform_keystore_get_nvs_partition_name();

  //  hap_platfrom_keystore_erase_partition(hap_platform_nvs_partition);

    int err = hap_platform_keystore_init_partition(hap_platform_nvs_partition, false);
    if (err != 0) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Error (%d) NVS init failed", err);
        return HAP_FAIL;
    }
    /* Not cheking the return value, as this partition may be absent */
    hap_platform_factory_nvs_partition = hap_platform_keystore_get_factory_nvs_partition_name();
    hap_platform_keystore_init_partition(hap_platform_factory_nvs_partition, true);

    keystore_init_done = true;
    ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Keystore initialised");
    return HAP_SUCCESS;
}

int __hap_keystore_get(const char *part_name, const char *name_space, const char *key, uint8_t *val, size_t *val_size)
{
    if (!keystore_init_done) {
        return HAP_FAIL;
    }

    int err  = hap_platform_keystore_get(part_name, name_space, key, val, val_size);
    if (err != 0) {
        return HAP_FAIL;
    }
    return HAP_SUCCESS;
}
int hap_keystore_get(const char *name_space, const char *key, uint8_t *val, size_t *val_size)
{

    return __hap_keystore_get(hap_platform_nvs_partition, name_space, key, val, val_size);
}
int hap_factory_keystore_get(const char *name_space, const char *key, uint8_t *val, size_t *val_size)
{
    return __hap_keystore_get(hap_platform_factory_nvs_partition, name_space, key, val, val_size);
}

int __hap_keystore_set(const char *part_name, const char *name_space, const char *key, const uint8_t *val, const size_t val_len)

{
    if (!keystore_init_done) {
        return HAP_FAIL;
    }

    int err  = hap_platform_keystore_set(part_name, name_space, key, val, val_len);
    if (err != 0) {
        return HAP_FAIL;
    }
    return HAP_SUCCESS;
}

int hap_keystore_set(const char *name_space, const char *key, const uint8_t *val, const size_t val_len)
{
    return __hap_keystore_set(hap_platform_nvs_partition, name_space, key, val, val_len);
}

int hap_factory_keystore_set(const char *name_space, const char *key, const uint8_t *val, const size_t val_len)
{
    return __hap_keystore_set(hap_platform_factory_nvs_partition, name_space, key, val, val_len);
}

int hap_keystore_delete(const char *name_space, const char *key)
{
    if (!keystore_init_done) {
        return HAP_FAIL;
    }

    int err = hap_platform_keystore_delete(hap_platform_nvs_partition, name_space, key);
    if (err != 0) {
        return HAP_FAIL;
    }
    return HAP_SUCCESS;
}

int hap_keystore_delete_namespace(const char *name_space)
{
    if (!keystore_init_done) {
        return HAP_FAIL;
    }

    int err = hap_platform_keystore_delete_namespace(hap_platform_nvs_partition, name_space);
    if (err != 0) {
        return HAP_FAIL;
    }
    return HAP_SUCCESS;
}

void hap_keystore_erase_all_data()
{
    hap_platfrom_keystore_erase_partition(hap_platform_nvs_partition);
}
