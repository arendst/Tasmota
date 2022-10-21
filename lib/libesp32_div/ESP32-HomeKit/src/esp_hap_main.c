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
#include <errno.h>
#include <unistd.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <esp_event.h>
#include <esp_wifi.h>

#include <esp_mfi_debug.h>
#include <esp_hap_acc.h>
#include <esp_hap_char.h>
#include <esp_hap_database.h>
#include <esp_hap_ip_services.h>
#include <esp_hap_wifi.h>
#include <esp_hap_mdns.h>
#include <esp_hap_keystore.h>
#include <esp_hap_main.h>
#include <esp_hap_wac.h>
#include <esp_hap_bct_priv.h>
#include <esp_hap_pair_verify.h>
#include <hap_platform_os.h>
#include <_esp_hap_config.h>
#include <hap_platform_httpd.h>
#include <esp_hap_ip_services.h>

static QueueHandle_t xQueue;
ESP_EVENT_DEFINE_BASE(HAP_EVENT);
// static TaskHandle_t hap_loop_handle;

const char * hap_get_version(void)
{
    return MFI_VER;
}
static void hap_nw_configured_sm(hap_internal_event_t event, hap_state_t *state)
{
    switch (event) {
        case HAP_INTERNAL_EVENT_ACC_PAIRED:
            hap_mdns_announce(false);
            break;
        case HAP_INTERNAL_EVENT_ACC_UNPAIRED:
            hap_mdns_announce(false);
            break;
        case HAP_INTERNAL_EVENT_CONFIG_NUM_UPDATED:
            hap_increment_and_save_config_num();
            hap_mdns_announce(false);
            break;
        case HAP_INTERNAL_EVENT_BCT_CHANGE_NAME:
            /* Waiting for sometime to allow the response to reach the host */
            vTaskDelay(1000 / hap_platform_os_get_msec_per_tick());
            hap_handle_bct_change_name();
            break;
        case HAP_INTERNAL_EVENT_BCT_HOT_PLUG:
            /* Waiting for sometime to allow the response to reach the host */
            vTaskDelay(1000 / hap_platform_os_get_msec_per_tick());
            hap_handle_hot_plug();
            break;
        default:
            break;
    }
}

static void hap_common_sm(hap_internal_event_t event)
{
    const char *reboot_reason = HAP_REBOOT_REASON_UNKNOWN;
    switch (event) {
        case HAP_INTERNAL_EVENT_RESET_PAIRINGS:
            ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Resetting all Pairing Information");
            /* Wait for some time before erasing the information, so that the callee
             * gets some time for any additional operations
             */
            vTaskDelay(1000 / hap_platform_os_get_msec_per_tick());
            hap_close_all_sessions();
            hap_mdns_deannounce();
            hap_erase_controller_info();
            hap_erase_accessory_info();
            reboot_reason = HAP_REBOOT_REASON_RESET_PAIRINGS;
            break;
        case HAP_INTERNAL_EVENT_RESET_TO_FACTORY:
            ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Resetting to Factory Defaults");
            /* Wait for some time before erasing the information, so that the callee
             * gets some time for any additional operations
             */
            vTaskDelay(1000 / hap_platform_os_get_msec_per_tick());
            hap_close_all_sessions();
            hap_mdns_deannounce();
            hap_keystore_erase_all_data();
            reboot_reason = HAP_REBOOT_REASON_RESET_TO_FACTORY;
            break;
        case HAP_INTERNAL_EVENT_RESET_HOMEKIT_DATA:
            ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Resetting all HomeKit Data");
            /* Wait for some time before erasing the information, so that the callee
             * gets some time for any additional operations
             */
            vTaskDelay(1000 / hap_platform_os_get_msec_per_tick());
            hap_close_all_sessions();
            hap_mdns_deannounce();
            hap_erase_controller_info();
            hap_erase_network_info();
            hap_erase_accessory_info();
            reboot_reason = HAP_REBOOT_REASON_RESET_HOMEKIT_DATA;
            break;
        case HAP_INTERNAL_EVENT_REBOOT:
            vTaskDelay(1000 / hap_platform_os_get_msec_per_tick());
            /* Wait for some time and then close all the active sessions
             */
            hap_close_all_sessions();
            hap_mdns_deannounce();
            reboot_reason = HAP_REBOOT_REASON_REBOOT_ACC;
            break;
        case HAP_INTERNAL_EVENT_RESET_NETWORK:
            ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Resetting Network Credentials");
            /* Wait for some time, close all the active sessions and then
             * erase network info.
             */
            vTaskDelay(1000 / hap_platform_os_get_msec_per_tick());
            hap_close_all_sessions();
            hap_mdns_deannounce();
            hap_erase_network_info();
            reboot_reason = HAP_REBOOT_REASON_RESET_NETWORK;
            break;
        case HAP_INTERNAL_EVENT_TRIGGER_NOTIF:
/* TODO: Avoid direct http function. Notification could be even for iCloud or BLE.
 */
            hap_http_send_notif();
            return;
        default:
            return;
        }

    /* Wait for some time after peeforming the operations and then reboot */
    ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Rebooting...");
    hap_report_event(HAP_EVENT_ACC_REBOOTING, (void*)reboot_reason, strlen(reboot_reason) + 1);
    vTaskDelay(1000 / hap_platform_os_get_msec_per_tick());
    esp_restart();
}

static void hap_loop_task(void *param)
{
    hap_state_t cur_state = HAP_STATE_NONE;
    xQueue = xQueueCreate( 10, sizeof(hap_event_ctx_t) );
    hap_event_ctx_t hap_event;
    bool loop_continue = true;
    ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "HAP Main Loop Started");
    while (loop_continue) {
        if (xQueueReceive(xQueue, &hap_event, portMAX_DELAY) != pdTRUE) {
            continue;
        }
        if (hap_event.event == HAP_INTERNAL_EVENT_LOOP_STOP) {
            loop_continue = false;
            continue;
        }
        hap_common_sm(hap_event.event);
        hap_nw_configured_sm(hap_event.event, &cur_state);
    }
    vQueueDelete(xQueue);
    xQueue = NULL;
    vTaskDelete(NULL);
}

static bool loop_started;
int hap_loop_start()
{
    if (!loop_started) {
        loop_started = true;
        xTaskCreate(hap_loop_task, "hap-loop", hap_priv.cfg.task_stack_size, NULL,
                        hap_priv.cfg.task_priority, NULL);
    }
    return HAP_SUCCESS;
}

bool is_hap_loop_started()
{
    return loop_started;
}
int hap_send_event(hap_internal_event_t event)
{
    if (!is_hap_loop_started()) {
        return HAP_FAIL;
    }
    if (!xQueue) {
        return HAP_FAIL;
    }
    hap_event_ctx_t hap_event = {
        .event = event,
    };
    BaseType_t ret;
    if (xPortInIsrContext() == pdTRUE) {
        ret = xQueueSendFromISR(xQueue, &hap_event, NULL);
    } else {
        ret = xQueueSend(xQueue, &hap_event, 0);
    }
    if (ret == pdTRUE) {
        return HAP_SUCCESS;
    }
    return HAP_FAIL;
}

int hap_update_config_number()
{
    return hap_send_event(HAP_INTERNAL_EVENT_CONFIG_NUM_UPDATED);
}

int hap_loop_stop()
{
    return hap_send_event(HAP_INTERNAL_EVENT_LOOP_STOP);
}

#if 0
static void hap_network_event_handler(void* arg, esp_event_base_t event_base,
        int event_id, void* event_data)
{
    if (((event_base == WIFI_EVENT) && (event_id == WIFI_EVENT_STA_CONNECTED)) ||
        ((event_base == ETH_EVENT) && (event_id == ETHERNET_EVENT_CONNECTED))){
        hap_send_event(HAP_INTERNAL_EVENT_NETWORK_CONNECTED);
    }
}
#endif
int hap_init(hap_transport_t method)
{
    int ret = HAP_SUCCESS;
    if (!(method & (HAP_TRANSPORT_WIFI | HAP_TRANSPORT_ETHERNET))) {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Invalid Transport");
        return HAP_FAIL;
    }

    hap_priv.transport = method;

    ret = hap_keystore_init();
    if (ret != 0 ) {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "HAP Key Store Init failed");
        return ret;
    }

    ret = hap_database_init();
    if (ret != 0 ) {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "HAP Database Init failed");
        return ret;
    }
    ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "HAP Initialization succeeded. Version : %s", hap_get_version());

    return ret;
}

int hap_deinit(void)
{
    int ret = HAP_SUCCESS;
    //todo
    return ret;
}

int hap_reset_to_factory()
{
    return hap_send_event(HAP_INTERNAL_EVENT_RESET_TO_FACTORY);
}

int hap_reset_homekit_data()
{
    return hap_send_event(HAP_INTERNAL_EVENT_RESET_HOMEKIT_DATA);
}

int hap_reset_pairings()
{
    return hap_send_event(HAP_INTERNAL_EVENT_RESET_PAIRINGS);
}

int hap_reboot_accessory()
{
    return hap_send_event(HAP_INTERNAL_EVENT_REBOOT);
}

int hap_reset_network()
{
    return hap_send_event(HAP_INTERNAL_EVENT_RESET_NETWORK);
}

int hap_start(void)
{
    int ret = 0;
#ifdef CONFIG_HAP_MFI_ENABLE
    if (hap_priv.auth_type == HAP_MFI_AUTH_HW) {
        ret = hap_enable_hw_auth();
    } else if (hap_priv.auth_type == HAP_MFI_AUTH_SW) {
        ret =  hap_enable_sw_auth();
    }
#endif /* CONFIG_HAP_MFI_ENABLE */
    if (ret != 0) {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Failed to enable MFi %s authentication",
                hap_priv.auth_type == HAP_MFI_AUTH_HW ? "HW" : "SW");
        return HAP_FAIL;
    }

    if (!hap_get_first_acc()) {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Failed to start HAP. Please add an Accessory before hap_start()");
        return HAP_FAIL;
    }

    ret = hap_acc_setup_init();
    if (ret != HAP_SUCCESS) {
         ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Accessory Setup init failed");
         return ret;
    }

    ret = hap_httpd_start();
    if (ret != HAP_SUCCESS) {
         ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "HTTPD START Failed [%d]", ret);
         return ret;
    }

    ret = hap_event_queue_init();
    if (ret != HAP_SUCCESS) {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Queue Initialisation for Event Notifications Failed");
        return ret;
    }

    ret = hap_loop_start();
    if (ret != 0) {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "HAP Loop Failed: [%d]", ret);
        return ret;
    }
    ret = hap_mdns_init();
    if (ret != 0 ) {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "HAP mDNS Init failed");
        return ret;
    }

    ret = hap_ip_services_start();
    if (ret != 0) {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "HAP IP Services Start Failed [%d]", ret);
        return ret;
    }
    return HAP_SUCCESS;
}

int hap_stop(void)
{
    int ret = HAP_SUCCESS;
    hap_mdns_deannounce();
    hap_mdns_deinit();
    httpd_handle_t _handle = hap_platform_httpd_get_handle();
    if(_handle != NULL) hap_platform_httpd_stop(_handle);
    ret = hap_loop_stop();
    return ret;
}

void hap_report_event(hap_event_t event, void *data, size_t data_size)
{
    if (hap_priv.hap_event_handler) {
        hap_priv.hap_event_handler(event, data);
    }
    esp_event_post(HAP_EVENT, event, data, data_size, portMAX_DELAY);
}

void hap_register_event_handler(hap_event_handler_t handler)
{
    hap_priv.hap_event_handler = handler;
}
