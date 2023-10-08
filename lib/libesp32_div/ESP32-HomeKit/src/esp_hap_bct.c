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
#include <esp_hap_main.h>
#include <esp_hap_mdns.h>
#include <esp_hap_wifi.h>
#include <esp_hap_database.h>
#include <esp_mfi_debug.h>

static char *new_name;

void hap_bct_change_name(const char *name)
{
    if (new_name) {
        hap_platform_memory_free(new_name);
    }
    new_name = strdup(name);
    hap_send_event(HAP_INTERNAL_EVENT_BCT_CHANGE_NAME);
}

void hap_bct_hot_plug()
{
    hap_send_event(HAP_INTERNAL_EVENT_BCT_HOT_PLUG);
}

void hap_handle_bct_change_name()
{
    if (!new_name) {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "No BCT name specified");
        return;
    } else {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Changing BCT Name to %s", new_name);
    }
    if (hap_mdns_serv_name_change(&hap_priv.hap_mdns_handle, new_name) != HAP_SUCCESS) {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Failed to change BCT name");
    }
    hap_platform_memory_free(new_name);
    new_name = NULL;
}

void hap_handle_hot_plug()
{
    esp_wifi_stop();
    vTaskDelay((10 * 1000) / portTICK_PERIOD_MS); /* Wait for 10 seconds */
    esp_wifi_start();
    esp_wifi_connect();
}
