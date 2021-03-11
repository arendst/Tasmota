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
#ifndef _HAP_PAIR_SETUP_H_
#define _HAP_PAIR_SETUP_H_
#include <stdint.h>
#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include <mu_srp.h>
#include <esp_hap_pair_common.h>

#define PS_NONCE1	"PS-Msg04"
#define PS_NONCE2	"PS-Msg05"
#define PS_NONCE3	"PS-Msg06"

#define PAIR_FLAG_TRANSIENT     0x00000010
#define PAIR_FLAG_SPLIT         0x01000000


typedef struct {
	uint8_t state;
	uint8_t method;
    uint32_t pairing_flags;
    int8_t pairing_flags_len;
	int len_s;
	char *bytes_s;
    int secret_len;
    char *shared_secret;
    mu_srp_handle_t srp_hd;
	hap_ctrl_data_t *ctrl;
	uint8_t session_key[32];
    TimerHandle_t timer;
    hap_secure_session_t *session;
    int sock_fd;
} pair_setup_ctx_t;
int hap_pair_setup_context_init(int sock_fd, void **ctx, uint8_t *buf, int bufsize, int *outlen);
int hap_pair_setup_process(void **ctx, uint8_t *buf, int inlen, int bufsize, int *outlen);
void hap_pair_setup_ctx_clean(void *sess_ctx);
int hap_pair_setup_manage_mfi_auth(pair_setup_ctx_t *ps_ctx, hap_tlv_data_t *tlv_data, hap_tlv_error_t *tlv_error);
#endif /* _HAP_PAIR_SETUP_H_ */
