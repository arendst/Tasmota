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
#ifndef _HAP_DATABASE_H_
#define _HAP_DATABASE_H_

#include <hap.h>
#include <esp_hap_controllers.h>
#include <esp_hap_pair_common.h>
#include <esp_hap_mdns.h>
#include <esp_hap_secure_message.h>
#include <esp_http_server.h>

#define HAP_KEYSTORE_NAMESPACE_HAPMAIN  "hap_main"
#define HAP_FACTORY_NAMESPACE_HAP_SETUP "hap_setup"

#define HAP_MAX_SESSIONS	8
#define SETUP_ID_LEN        4
#define SETUP_HASH_LEN      4

#define HAP_ACC_ID_LEN		18 /* AA:BB:CC:XX:YY:ZZ\0 */
#define ED_KEY_LEN		32
#define HAP_SW_TOKEN_MAX_LEN        1200


typedef struct {
    hap_acc_cfg_t primary_acc;
    uint32_t config_num;
    uint32_t cur_aid;
    uint8_t raw_acc_id[6];
	char acc_id[HAP_ACC_ID_LEN];
    char setup_id[SETUP_ID_LEN + 1];
    uint8_t setup_hash[SETUP_HASH_LEN];
    char setup_hash_str[SETUP_HASH_LEN * 2 + 1];
	uint8_t ltska[ED_KEY_LEN];
	uint8_t ltpka[ED_KEY_LEN];
	hap_cid_t cid;
	hap_ctrl_data_t controllers[HAP_MAX_CONTROLLERS];
	hap_secure_session_t *sessions[HAP_MAX_SESSIONS];
	uint8_t pair_attempts;
    hap_mdns_handle_t wac_mdns_handle;
    hap_mdns_handle_t hap_mdns_handle;
    hap_setup_info_t *setup_info;
    char *setup_code;
    char *ssid;
    char *password;
    hap_software_token_info_t *token_info;
    uint8_t features;
    hap_event_handler_t hap_event_handler;
    char *softap_ssid;
    void (*ext_nw_prov_start)(void *data, const char *name);
    void (*ext_nw_prov_stop)(void *data);
    void *ext_nw_prov_data;
    hap_cfg_t cfg;
    hap_transport_t transport;
    uint32_t pairing_flags;
    httpd_handle_t server;
    uint8_t *product_data;
    uint16_t state_num;
    bool disconnected_event_sent;
    hap_mfi_auth_type_t auth_type;
} hap_priv_t;

extern hap_priv_t hap_priv;
int hap_database_init(void);
char *hap_get_acc_id();
int hap_get_next_aid();
int hap_acc_setup_init();
void hap_erase_accessory_info();
void hap_increment_and_save_config_num();
void hap_increment_and_save_state_num();
#endif /* _HAP_DATABASE_H_ */
