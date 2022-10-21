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

#ifndef _HAP_WAC_H_
#define _HAP_WAC_H_
#include <stdint.h>
#include <esp_event.h>
#include <esp_wifi.h>
#include <esp_err.h>
/** HomeKit Event Base */
ESP_EVENT_DECLARE_BASE(HAP_WAC_EVENT);
/** HomeKit Events */
typedef enum {
    /* WAC has started. Associated data is NULL */
    HAP_WAC_EVENT_STARTED,
    /** WAC has timed out because of inactivity. Associated data is NULL.
     * Appropriate indications should be given to the user, so that the accessory can be rebooted
     * for restarting WAC.
     */
    HAP_WAC_EVENT_TIMEOUT,
    /** WAC was successful and will stop after some time. Associated data is NULL.
     */
    HAP_WAC_EVENT_SUCCESS,
    /** WAC is requesting to start SoftAP. Associated data is a pointer to a NULL terminated SSID.
     * Using this SSID isn't required, but recommended. The network security should be "Open" for
     * WAC to work. If SoftAP has already been started, nothing needs to be done for this event.
     * The helper function hap_wifi_softap_start() can be used for this.
     */
    HAP_WAC_EVENT_REQ_SOFTAP_START,
    /** WAC is requesting to stop SoftAP. Associated data is NULL.
     * The helper function hap_wifi_softap_stop() can be used for this.
     */
    HAP_WAC_EVENT_REQ_SOFTAP_STOP,
    /** WAC has received the Wi-Fi credentials. Associated data is a pointer to wifi_config_t
     * structure. This should be used to connect to the Wi-Fi network.
     * The helper function hap_wifi_sta_connect() can be used for this.
     */
    HAP_WAC_EVENT_RECV_CRED,
    /** WAC has stopped */
    HAP_WAC_EVENT_STOPPED
} hap_wac_event_t;

/** Start WAC Provisioning.
 *
 * This should be called when the accessory boots up in an unprovisioned mode.
 * The helper function hap_wifi_is_provisioned() function can be used to find
 * if the accessory is provisioned or not.
 *
 * @return 0 on success.
 * @return -1 on failure.
 */
int hap_wac_start(void);

/** Stop WAC Provisioning.
 *
 * This should be used only if WAC needs to be stopped explicitly (say, after
 * provisioning was done using some other means). Else, WAC stops automatically,
 * either after the timeout or after successful provisioning.
 *
 * @return 0 on success.
 * @return -1 on failure.
 */
int hap_wac_stop(void);

/** Check is accessory is provisioned
 *
 * @param[out] provisioned Pointer to an allocated boolean variable. Will
 *      be set to true if accessory is provisioned, else false.
 *
 * @return ESP_OK on success.
 * @return error on failure.
 */
esp_err_t hap_wifi_is_provisioned(bool *provisioned);

/** Start SoftAP
 *
 * @param[in] ssid NULL terminated ssid string.
 *
 * @return ESP_OK on success.
 * @return error on failure.
 */
esp_err_t hap_wifi_softap_start(char *ssid);

/** Stop Soft AP
 *
 * @return ESP_OK on success.
 * @return error on failure.
 */
esp_err_t hap_wifi_softap_stop(void);

/** Connect to Wi-Fi network with given configuration
 *
 * @param[in] config Pointer to \ref wifi_config_t structure.
 *
 * @return ESP_OK on success.
 * @return error on failure.
 */
esp_err_t hap_wifi_sta_connect(wifi_config_t *config);

#endif /* _HAP_WAC_H_ */
