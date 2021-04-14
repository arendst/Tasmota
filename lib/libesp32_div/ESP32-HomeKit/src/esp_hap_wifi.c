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
#include <esp_mfi_debug.h>
#include <string.h>
#include <esp_wifi.h>
#include <esp_hap_database.h>

esp_err_t hap_wifi_is_provisioned(bool *provisioned)
{
    if (!provisioned) {
        return ESP_ERR_INVALID_ARG;
    }

    *provisioned = false;

    /* Get Wi-Fi Station configuration */
    wifi_config_t wifi_cfg;
    if (esp_wifi_get_config(ESP_IF_WIFI_STA, &wifi_cfg) != ESP_OK) {
        return ESP_FAIL;
    }

    if (strlen((const char *) wifi_cfg.sta.ssid)) {
        *provisioned = true;
    }
    return ESP_OK;
}
bool hap_is_network_configured(void)
{
    /* If only the Ethernet is enabled, return true */
    if (hap_priv.transport == HAP_TRANSPORT_ETHERNET) {
        return true;
    }

    bool provisioned = false;
    hap_wifi_is_provisioned(&provisioned);
    return provisioned;
}

void hap_erase_network_info(void)
{
    esp_wifi_restore();
}

esp_err_t hap_wifi_softap_start(char *ssid)
{
    if (!ssid) {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "SSID cannot be NULL");
    }
    ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Starting SoftAP with SSID: %s", ssid);
    wifi_config_t wifi_config = {
        .ap = {
            .ssid = "",
            .ssid_len = 0,
            .max_connection = 4,
            .password = "",
            .authmode = WIFI_AUTH_OPEN
        },
    };
    size_t ssid_len = strnlen(ssid, sizeof(wifi_config.ap.ssid));
    memcpy(wifi_config.ap.ssid, ssid, ssid_len);
    wifi_mode_t mode;
    esp_wifi_get_mode(&mode);
    if (mode == WIFI_MODE_STA) {
        esp_wifi_set_mode(WIFI_MODE_APSTA);
    } else {
        esp_wifi_set_mode(WIFI_MODE_AP);
    }
    esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config);
    esp_wifi_start();
    return ESP_OK;
}

esp_err_t hap_wifi_softap_stop(void)
{
    ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Stopping SoftAP.");
    wifi_mode_t mode;
    esp_wifi_get_mode(&mode);
    if (mode == WIFI_MODE_AP) {
        esp_wifi_stop();
    }
    esp_wifi_set_mode(WIFI_MODE_STA);
    return ESP_OK;
}

esp_err_t hap_wifi_sta_connect(wifi_config_t *config)
{
    ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Connecting to Wi-Fi.");
    wifi_mode_t mode;
    esp_wifi_get_mode(&mode);
    if (mode == WIFI_MODE_AP || mode == WIFI_MODE_APSTA) {
        esp_wifi_set_mode(WIFI_MODE_APSTA);
    } else {
        esp_wifi_set_mode(WIFI_MODE_STA);
    }
    esp_wifi_set_config(ESP_IF_WIFI_STA, config);
    esp_wifi_start();
    esp_wifi_connect();
    return ESP_OK;
}
