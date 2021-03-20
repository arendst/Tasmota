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


#include <esp_log.h>
#include <nvs_flash.h>
#include <string.h>
#include <FS.h>

#define HAP_PLATFORM_DEF_NVS_PARTITION          "nvs"
#define HAP_PLATFORM_DEF_FACTORY_NVS_PARTITION  "factory_nvs"

extern "C" {

static const char *TAG = "hap_platform_keystore";

const char * hap_platform_keystore_get_nvs_partition_name() {
    return HAP_PLATFORM_DEF_NVS_PARTITION;
}

const char * hap_platform_keystore_get_factory_nvs_partition_name() {
    return HAP_PLATFORM_DEF_FACTORY_NVS_PARTITION;
}

#define HAP_USE_LITTLEFS

#ifdef HAP_USE_LITTLEFS

#include <LITTLEFS.h>

extern FS *ffsp;

int hap_platform_keystore_init_partition(const char *part_name, bool read_only) {
  return 0;
}

int hap_platform_keystore_get(const char *part_name, const char *name_space, const char *key, uint8_t *val, size_t *val_size) {
  char path[48];
  strcpy(path, "/");
  strcat(path, part_name);

  File fp = ffsp->open(path, "r");
  if (!fp) {
    ffsp->mkdir(path);
    return -1;
  }
  fp.close();

  strcat(path, "/");
  strcat(path, name_space);
  fp = ffsp->open(path, "r");
  if (!fp) {
    ffsp->mkdir(path);
    return -1;
  }
  fp.close();

  strcat(path, "/");
  strcat(path, key);
  fp = ffsp->open(path, "r");
  if (fp) {
    fp.read(val, *val_size);
    fp.close();
  } else {
    *val_size = 0;
    return -1;
  }
  return 0;
}

int hap_platform_keystore_set(const char *part_name, const char *name_space, const char *key, const uint8_t *val, const size_t val_len) {
  char path[48];
  strcpy(path, "/");
  strcat(path, part_name);

  File fp = ffsp->open(path, "r");
  if (!fp) {
    ffsp->mkdir(path);
  }
  fp.close();

  strcat(path, "/");
  strcat(path, name_space);
  fp = ffsp->open(path, "r");
  if (!fp) {
    ffsp->mkdir(path);
  }
  fp.close();

  strcat(path, "/");
  strcat(path, key);
  fp = ffsp->open(path, "w");
  if (fp) {
    fp.write(val, val_len);
    fp.close();
  } else {
    return -1;
  }
  return 0;
}

int hap_platform_keystore_delete(const char *part_name, const char *name_space, const char *key) {
  char path[48];
  strcpy(path, "/");
  strcat(path, part_name);
  strcat(path, "/");
  strcat(path, name_space);
  strcat(path, "/");
  strcat(path, key);
  ffsp->remove(path);
  return 0;
}

// should
int hap_platform_keystore_delete_namespace(const char *part_name, const char *name_space) {
  char path[48];
  strcpy(path, "/");
  strcat(path, part_name);
  strcat(path, "/");
  strcat(path, name_space);
  File fp = ffsp->open(path, "r");
  if (fp.isDirectory()) {
    while (true) {
      File entry = fp.openNextFile();
      if (!entry) break;
      char p[48];
      strcpy(p,entry.name());
      entry.close();
      ffsp->remove(p);
    }
  }
  return 0;
}

// last resort only
int hap_platfrom_keystore_erase_partition(const char *part_name) {
char path[48];
strcpy(path, "/");
strcat(path, part_name);
File fp = ffsp->open(path, "r");
if (fp.isDirectory()) {
  while (true) {
    File entry = fp.openNextFile();
    if (!entry) break;
    const char *ep = entry.name();
    if (*ep=='/') ep++;
    char *lcp = strrchr(ep,'/');
    if (lcp) {
      ep = lcp + 1;
    }
    char p[48];
    strcpy(p,entry.name());
    if (entry.isDirectory()) {
      hap_platform_keystore_delete_namespace(part_name, ep);
      entry.close();
      ffsp->rmdir(p);
    } else {
      entry.close();
      ffsp->remove(p);
    }

  }
}
  return 0;
}

#else

#ifdef CONFIG_NVS_ENCRYPTION
int hap_platform_keystore_init_partition(const char *part_name, bool read_only)
{
    esp_err_t err;
    nvs_sec_cfg_t *cfg = NULL;
    nvs_sec_cfg_t sec_cfg;
    esp_partition_iterator_t iterator = esp_partition_find(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_NVS_KEYS, NULL);
    if (iterator) {
        const esp_partition_t *partition = esp_partition_get(iterator);
        err = nvs_flash_read_security_cfg(partition, &sec_cfg);
        if (err == ESP_OK) {
            cfg = &sec_cfg;
        } else {
            ESP_LOGE(TAG, "No NVS keys found");
        }
    } else {
        ESP_LOGE(TAG, "No NVS keys partition found");
    }
    if (!cfg) {
        ESP_LOGE(TAG, "NVS partition '%s' not encrypted", part_name);
    } else {
        ESP_LOGI(TAG, "NVS partition '%s' is encrypted", part_name);
    }
    if (read_only) {
        err = nvs_flash_secure_init_partition(part_name, cfg);
    } else {
        err = nvs_flash_secure_init_partition(part_name, cfg);
        if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
            ESP_ERROR_CHECK(nvs_flash_erase_partition(part_name));
            err = nvs_flash_secure_init_partition(part_name, cfg);
        }
    }
    if (err == ESP_OK) {
        return 0;
    }
    return -1;
}
#else
int hap_platform_keystore_init_partition(const char *part_name, bool read_only)
{
    esp_err_t err;
    if (read_only) {
        err = nvs_flash_init_partition(part_name);
    } else {
        err = nvs_flash_init_partition(part_name);
        if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
            ESP_ERROR_CHECK(nvs_flash_erase_partition(part_name));
            err = nvs_flash_init_partition(part_name);
        }
    }
    if (err == ESP_OK) {
        return 0;
    }
    return -1;
}
#endif /* CONFIG_NVS_ENCRYPTION */

int hap_platform_keystore_get(const char *part_name, const char *name_space, const char *key, uint8_t *val, size_t *val_size)
{
    nvs_handle handle;
    esp_err_t err = nvs_open_from_partition(part_name, name_space, NVS_READONLY, &handle);
    if (err != ESP_OK) {
        return -1;
    } else {
        err = nvs_get_blob(handle, key, val, val_size);
        nvs_close(handle);
    }
    if (err == ESP_OK) {
        return 0;
    }
    return -1;
}

int hap_platform_keystore_set(const char *part_name, const char *name_space, const char *key, const uint8_t *val, const size_t val_len)

{
    nvs_handle handle;
    esp_err_t err = nvs_open_from_partition(part_name, name_space, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%d) opening NVS handle!", err);
    } else {
        err = nvs_set_blob(handle, key, val, val_len);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Failed to write %s", key);
        } else {
            nvs_commit(handle);
        }
        nvs_close(handle);
    }
    if (err == ESP_OK) {
        return 0;
    }
    return -1;
}

int hap_platform_keystore_delete(const char *part_name, const char *name_space, const char *key)
{
    nvs_handle handle;
    esp_err_t err = nvs_open_from_partition(part_name, name_space, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%d) opening NVS handle!", err);
    } else {
        err = nvs_erase_key(handle, key);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Failed to delete %s", key);
        } else {
            nvs_commit(handle);
        }
        nvs_close(handle);
    }
    if (err == ESP_OK) {
        return 0;
    }
    return -1;
}

int hap_platform_keystore_delete_namespace(const char *part_name, const char *name_space)
{
    nvs_handle handle;
    esp_err_t err = nvs_open_from_partition(part_name, name_space, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%d) opening NVS handle!", err);
    } else {
        err = nvs_erase_all(handle);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Failed to delete %s", name_space);
        } else {
            nvs_commit(handle);
        }
        nvs_close(handle);
    }
    if (err == ESP_OK) {
        return 0;
    }
    return -1;
}

int hap_platfrom_keystore_erase_partition(const char *part_name)
{
    esp_err_t err = nvs_flash_erase_partition(part_name);
    if (err == ESP_OK) {
        return 0;
    }
    return -1;
}
#endif // USE_LITTLEFS

}
