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
#include <stdint.h>
#include <esp_http_server.h>
#include <esp_mfi_debug.h>
#include <esp_hap_pair_common.h>
#include <esp_hap_pair_setup.h>
#include <esp_hap_database.h>
#include <hap.h>

ESP_EVENT_DEFINE_BASE(HAP_WAC_EVENT);

int hap_pair_setup_manage_mfi_auth(pair_setup_ctx_t *ps_ctx, hap_tlv_data_t *tlv_data, hap_tlv_error_t *tlv_error)
{

    if (ps_ctx->method == HAP_METHOD_PAIR_SETUP) {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Secure pair setup not supported. Please use MFi variant of the SDK.");
        *tlv_error = kTLVError_Unknown;
        return HAP_FAIL;
    }
    ESP_MFI_DEBUG(ESP_MFI_DEBUG_WARN, "Using pair-setup without MFi.");
    return ESP_OK;
}

int hap_enable_mfi_auth(hap_mfi_auth_type_t auth_type)
{
    ESP_MFI_DEBUG(ESP_MFI_DEBUG_WARN, "MFi auth not supported. Falling back to HAP_MFI_AUTH_NONE");
    hap_priv.auth_type = HAP_MFI_AUTH_NONE;
    return 0;
}

int hap_wac_start(void)
{
    ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "WAC not supported. Please use MFi variant of the SDK.");
    return ESP_FAIL;
}

int hap_wac_stop(void)
{
    ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "WAC not supported. Please use MFi variant of the SDK.");
    return ESP_FAIL;
}
int hap_enable_hw_auth(void)
{
    ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "MFi HW Auth not supported. Please use MFi variant of the SDK.");
    return ESP_FAIL;

}
int hap_enable_sw_auth(void)
{
    ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "MFi SW Auth not supported. Please use MFi variant of the SDK.");
    return ESP_FAIL;
}
int hap_register_secure_message_handler(httpd_handle_t handle)
{
    ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "MFi SW Auth not supported. Please use MFi variant of the SDK.");
    return ESP_FAIL;
}
int hap_unregister_secure_message_handler(httpd_handle_t handle)
{
    ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "MFi SW Auth not supported. Please use MFi variant of the SDK.");
    return ESP_FAIL;
}
