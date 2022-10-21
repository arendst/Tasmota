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
#include <string.h>
#include <sys/socket.h>

#include <sodium/crypto_aead_chacha20poly1305.h>
#include <byte_convert.h>

#include <esp_mfi_debug.h>
#include <hap.h>
#include <esp_hap_database.h>
#include <esp_hap_pair_common.h>
#include <esp_hap_pair_verify.h>

#define HAP_MAX_NW_FRAME_SIZE	1024 /* As per HAP Specifications */
#define AUTH_TAG_LEN            16
typedef struct {
	uint8_t pkt_size[2];
	uint8_t data[HAP_MAX_NW_FRAME_SIZE];
	/* The Poly auth tag buffer will get used only if the data length is
	 * greater than HAP_MAX_NW_FRAME_SIZE - 16.
	 * Else, the auth tag would be included in the data buffer itself
	 */
	uint8_t poly_auth_tag[AUTH_TAG_LEN];
} hap_encrypt_frame_t;

typedef struct {
	uint16_t pkt_size;
	uint16_t bytes_read;
	uint8_t data[HAP_MAX_NW_FRAME_SIZE + AUTH_TAG_LEN];
	hap_secure_session_t *session;
} hap_decrypt_frame_t;

typedef int (*hap_decrypt_read_fn_t) (uint8_t *buf, int buf_size, void *context);
static int min(int val1, int val2)
{
	if (val1 < val2)
		return val1;
	return val2;
}
static int hap_httpd_raw_recv(uint8_t *buf, int buf_size, void *context)
{
	int sock = *((int *)context);
	return recv(sock, buf, buf_size, 0);
}

/* Frame format as per HAP Specifications:
 * <2: AAD for Little Endian length of encrypted data (n) in bytes>
 * <n: Encrypted data according to AEAD algorithm, upto 1024 bytes>
 * <16: authTag according to AEAD algorithm>
 */
int hap_encrypt_data(hap_encrypt_frame_t *frame, hap_secure_session_t *session,
		uint8_t *buf, int buflen)
{
	if (!session)
		return HAP_FAIL;
	put_u16_le(frame->pkt_size, buflen);
	/* Encrypt the received data as per Chacha20-Poly1305 AEAD algorithm.
	 * The authTag will be appended at the end of data. Hence, pointer given as
	 * frame->data + nlen
	 */
    unsigned long long mlen = 16;
    uint8_t newnonce[12];
    memset(newnonce, 0, sizeof newnonce);
    memcpy(newnonce+4, session->encrypt_nonce, 8);
    crypto_aead_chacha20poly1305_ietf_encrypt_detached(frame->data, frame->data + buflen, &mlen,
                buf, buflen, frame->pkt_size, 2, NULL, newnonce, session->encrypt_key);

	/* Increment nonce after every frame */
	uint64_t int_nonce = get_u64_le(session->encrypt_nonce);
	int_nonce++;
	put_u64_le(session->encrypt_nonce, int_nonce);
	return 2 + buflen + 16; /* Total length of the encrypted data */
}

#include <esp_hap_main.h>
void hap_close_ctrl_sessions_fix(hap_secure_session_t *session)
{
	if (!session)
		return;
	int i;
	printf("---- hap_close_ctrl_sessions_fix begin -----\n");
	for (i = 0; i < HAP_MAX_SESSIONS; i++) {
        if (!hap_priv.sessions[i])
            continue;
		if (hap_priv.sessions[i] == session) {
            hap_report_event(HAP_EVENT_CTRL_DISCONNECTED, (session->ctrl->info.id),
                    sizeof((session->ctrl->info.id)));
			/* TODO: Use some generic function and not a direct HTTPD function
			 */
            printf("---- trigger_close fd: %d\n", hap_priv.sessions[i]->conn_identifier);
            httpd_sess_trigger_close(hap_priv.server, hap_priv.sessions[i]->conn_identifier);
		}
	}
	printf("---- hap_close_ctrl_sessions_fix end ----\n");
}

int hap_decrypt_error(hap_secure_session_t *session)
{
	ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Decryption error/Connection lost. Marking session as invalid");
	if (session) {
		session->state = STATE_INVALID;
        //hap_close_ctrl_sessions(session->ctrl);
        hap_close_ctrl_sessions_fix(session);
	}
	return HAP_FAIL;
}

int hap_decrypt_data(hap_decrypt_frame_t *frame, hap_secure_session_t *session,
	void *buf, int buf_size, hap_decrypt_read_fn_t read_fn, void *context)
{
	if (!frame || !session)
		return -1;
	if (frame->session != session) {
		memset(frame, 0, sizeof(hap_decrypt_frame_t));
		frame->session = session;
	}
	if ((frame->pkt_size - frame->bytes_read) == 0) {
		int len = read_fn(frame->data, 2, context);
		if (len == 0) { //nothing received, but we should NOT consider this is a 'decrypt_error'
			return 0;
		}
		if (len < 2) {
			//len is -1 or 1
			//len = -1: socket disconnected
			//len =  1: try receiving 2 bytes timeout (SO_RCVTIMEO is set in esp_hap_ip_services.c)
			printf("---- error 1 ----, len: %d\n", len);
			//Decryption error/Connection lost on ESP32 with multiple Apple devices running Home
			//https://github.com/espressif/esp-homekit-sdk/issues/14
			//---- error 1 ----, len: 0
			return hap_decrypt_error(session);
		}

		frame->pkt_size = get_u16_le(frame->data);
		frame->bytes_read = 0;
		uint16_t bytes_to_read = frame->pkt_size + AUTH_TAG_LEN; /* +AUTH_TAG_LEN as the receivedd packet will also have the auth Tag */
		while (bytes_to_read) {
			int num_bytes = read_fn(&frame->data[frame->bytes_read],
					bytes_to_read, context);
			if (num_bytes <= 0) {
				printf("---- error 2 ----\n");
				return hap_decrypt_error(session);
			}
			bytes_to_read -= num_bytes;
			frame->bytes_read += num_bytes;
		}
		frame->bytes_read -= AUTH_TAG_LEN; /* -AUTH_TAG_LEN to get only the data length */
		uint8_t aad[2];
        int ret;
		put_u16_le(aad, frame->pkt_size); /* Packet size is the AAD for AEAD */
        uint8_t newnonce[12];
        memset(newnonce, 0, sizeof newnonce);
        memcpy(newnonce+4, session->decrypt_nonce, 8);
        ret = crypto_aead_chacha20poly1305_ietf_decrypt_detached(frame->data, NULL, frame->data, frame->pkt_size,
                    &frame->data[frame->bytes_read], aad, 2, newnonce, session->decrypt_key);
        if (ret != 0) { 
			ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "AEAD decryption failure");
			printf("---- error 3 ----\n");
			return hap_decrypt_error(session);
		}
		frame->bytes_read = 0;
		/* Increment nonce after every frame */
		int64_t int_nonce = get_u64_le(session->decrypt_nonce);
		int_nonce++;
		put_u64_le(session->decrypt_nonce, int_nonce);
	}
	int bytes = min(frame->pkt_size - frame->bytes_read, buf_size);
	memcpy(buf, &frame->data[frame->bytes_read], bytes);
	frame->bytes_read += bytes;
	return bytes;
}

int hap_httpd_send(httpd_handle_t hd, int sockfd, const char *buf, unsigned buf_len, int flags)
{
	hap_secure_session_t *session = httpd_sess_get_ctx(hap_priv.server, sockfd);
	if (session && (session->state == STATE_VERIFIED)) {
		uint8_t *buf_ptr = (uint8_t *)buf;
		int tmp_buf_len = buf_len;
		while (tmp_buf_len) {
			hap_encrypt_frame_t encrypt_frame;
			memset(&encrypt_frame, 0, sizeof(encrypt_frame));
			int len = min(tmp_buf_len, HAP_MAX_NW_FRAME_SIZE);
			int send_len = hap_encrypt_data(&encrypt_frame, session, buf_ptr, len);
			if (send(sockfd, (uint8_t *)&encrypt_frame, send_len, flags) <= 0)
				return HAP_FAIL;
			tmp_buf_len -= len;
			buf_ptr += len;
		}
		/* Return the total length at the end since this API expects so
		 */
		return buf_len;
	}
	return send(sockfd, buf, buf_len, flags);
}

int hap_httpd_recv(httpd_handle_t hd, int sockfd, char *buf, unsigned buf_len, int flags)
{
	static hap_decrypt_frame_t decrypt_frame;
	hap_secure_session_t *session = httpd_sess_get_ctx(hap_priv.server, sockfd);
	if (session) {
		if (session->state == STATE_VERIFIED) {
			return hap_decrypt_data(&decrypt_frame, session, buf, buf_len,
					hap_httpd_raw_recv, &sockfd);
		} else {
			/* If the session state is invalid, we return an error.
			 * The errno is set here explicitly, so that even if the higher layers
			 * query for it, they do not get a stale value.
			 * EACCES means permission denied, which indeed is the case here.
			 */
			errno = EACCES;
			return HAP_FAIL;
		}
	}
	return recv(sockfd, buf, buf_len, sockfd);
}
