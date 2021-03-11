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
#ifndef _HAP_SECURE_MESSAGE_H_
#define _HAP_SECURE_MESSAGE_H_
#include <stdint.h>
#include <esp_http_server.h>
/** Information for Software Token based authentication.
 * To be used only if MFi chip is not present on the accessory
 */
typedef struct {
    /** UUID for the accessory */
    uint8_t uuid[16];
    /** Token associated with the UUID */
    uint8_t *token;
    /* Length of the above token */
    size_t token_len;
} hap_software_token_info_t;

int hap_register_secure_message_handler(httpd_handle_t handle);
int hap_unregister_secure_message_handler(httpd_handle_t handle);
#endif /* _HAP_SECURE_MESSAGE_H_ */
