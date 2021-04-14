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
#ifndef _HAP_PAIR_VERIFY_H_
#define _HAP_PAIR_VERIFY_H_
#include <esp_hap_pair_common.h>
#include <esp_hap_controllers.h>
int hap_pair_verify_context_init(void **ctx, uint8_t *buf, int bufsize, int *outlen);
int hap_pair_verify_process(void **ctx, uint8_t *buf, int inlen, int bufsize, int *outlen);
uint8_t hap_pair_verify_get_state(void *ctx);
void hap_free_session(void *session);
int hap_get_ctrl_session_index(hap_secure_session_t *session);
void hap_close_ctrl_sessions(hap_ctrl_data_t *ctrl);
void hap_close_all_sessions();
#endif /* _HAP_PAIR_VERIFY_H_ */
