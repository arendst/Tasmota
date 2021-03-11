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
#include <stdlib.h>
#include <string.h>
#include <esp_err.h>
#include <esp_log.h>
#include <hap.h>

#include <base36.h>

static const char *TAG = "esp_hap_setup_payload";

#define SETUP_CODE_MASK     0x0000000007ffffff
#define HAP_OVER_IP_MASK    0x0000000010000000
#define WAC_MASK            0x0000000040000000
#define SETUP_PAYLOAD_PREFIX              "X-HM://00"

static void remove_chars(char *str, char c)
{
    int i = 0, j = 0;
    while(str[i]) {
        if (str[i] != c) {
            str[j] = str[i];
            j++;
        }
        i++;
    }
    str[j] = 0;
}

char *esp_hap_get_setup_payload(char *setup_code, char *setup_id, bool wac_support, hap_cid_t cid)
{
    if (!setup_code || !setup_id) {
        ESP_LOGE(TAG, "Setup code or Setup ID cannot be NULL");
        return NULL;
    }
    uint64_t payload = 0;
    if (strlen(setup_code) != 10 || strlen(setup_id) != 4) {
        ESP_LOGE(TAG, "Setup code or Setup ID not correct. Eg. 111-22-333, ES32");
        return NULL;
    }
    char setup_code_copy[11];
    strcpy(setup_code_copy, setup_code);
    remove_chars(setup_code_copy, '-');
    int64_t code = atoi(setup_code_copy);
    int64_t category = cid;
    category <<= 31;

    payload |= code;
    payload |= category;
    payload |= HAP_OVER_IP_MASK;
    if (wac_support) {
        payload |= WAC_MASK;
    }
    char *base36_str = base36_to_str(payload);
    char setup_payload[24];
    snprintf(setup_payload, sizeof(setup_payload), "%s%s%s", SETUP_PAYLOAD_PREFIX, base36_str, setup_id);
    free(base36_str);
    return strdup(setup_payload);
}
