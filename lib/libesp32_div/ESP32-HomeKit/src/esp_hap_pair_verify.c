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
#include <stdio.h>
#include <string.h>
#include <sodium/crypto_scalarmult_curve25519.h>
#include <sodium/crypto_sign_ed25519.h>
#include <hkdf-sha.h>
#include <sodium/crypto_aead_chacha20poly1305.h>
#include <esp_http_server.h>
#include <hap_platform_memory.h>

#include <esp_hap_main.h>
#include <esp_hap_pair_common.h>
#include <esp_hap_database.h>
#include <esp_hap_char.h>
#include <hexdump.h>
#include <esp_mfi_debug.h>
#include <esp_mfi_rand.h>

#define PAIR_VERIFY_ENCRYPT_SALT	"Pair-Verify-Encrypt-Salt"
#define PAIR_VERIFY_ENCRYPT_INFO	"Pair-Verify-Encrypt-Info"
#define PV_NONCE1 			"PV-Msg02"
#define PV_NONCE2 			"PV-Msg03"
#define CONTROL_SALT			"Control-Salt"
#define CONTROL_READ_INFO		"Control-Read-Encryption-Key"
#define CONTROL_WRITE_INFO		"Control-Write-Encryption-Key"

typedef struct {
	/* It is important that "state" should be the first element of the structure.
	 * It will be the first element, even in hap_secure_session_t.
	 * This will be useful while checking the state from the context, irrespective
	 * of whether the context points to pair_verify_ctx_t or hap_secure_session_t.
	 */
	uint8_t state;
	uint8_t ctrl_curve_pk[CURVE_KEY_LEN];
	uint8_t acc_curve_pk[CURVE_KEY_LEN];
	uint8_t hkdf_key[ENCRYPT_KEY_LEN];
	uint8_t shared_secret[CURVE_KEY_LEN];
	hap_secure_session_t *session;
} pair_verify_ctx_t;

void hap_close_ctrl_sessions(hap_ctrl_data_t *ctrl)
{
	if (!ctrl)
		return;
	int i;
	printf("---- hap_close_ctrl_sessions begin -----\n");
	for (i = 0; i < HAP_MAX_SESSIONS; i++) {
        if (!hap_priv.sessions[i])
            continue;
		if (hap_priv.sessions[i]->ctrl == ctrl) {
            hap_report_event(HAP_EVENT_CTRL_DISCONNECTED, (ctrl->info.id),
                    sizeof((ctrl->info.id)));
			/* TODO: Use some generic function and not a direct HTTPD function
			 */
            printf("---- trigger_close fd: %d\n", hap_priv.sessions[i]->conn_identifier);
            httpd_sess_trigger_close(hap_priv.server, hap_priv.sessions[i]->conn_identifier);
            break;
		}
	}
	printf("----hap_close_ctrl_sessions end ----\n");
}

int hap_get_ctrl_session_index(hap_secure_session_t *session)
{
	int i;
	for (i = 0; i < HAP_MAX_SESSIONS; i++) {
		if (hap_priv.sessions[i] == session)
			return i;
	}
	return -1;
}

void hap_close_all_sessions()
{
	int i;
	for (i = 0; i < HAP_MAX_SESSIONS; i++) {
		if (hap_priv.sessions[i]) {
			ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Closing Session");
            hap_close_ctrl_sessions(hap_priv.sessions[i]->ctrl);
        }
	}
}

static void hap_add_secure_session(hap_secure_session_t *session)
{
	int i;
	for (i = 0; i < HAP_MAX_SESSIONS; i++) {
		if (hap_priv.sessions[i] == NULL) {
			hap_priv.sessions[i] = session;
            hap_report_event(HAP_EVENT_CTRL_CONNECTED, session->ctrl->info.id,
                            sizeof(session->ctrl->info.id));
            /* Set the disconnected_event_sent flag here to false so that an
             * event can be sent later for a state change, when no controller
             * is connected.
             * HAP Spec R15 say that the state number should change only once
             * between accessory disconneted (from all controllers) to connected
             * state.
             */
            hap_priv.disconnected_event_sent = false;
			//ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "HomeKit Session active");
			ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "HomeKit Session active, fd: %d", session->conn_identifier);
			break;
		}
	}
}

void hap_free_session(void *session)
{
	if (!session)
		return;
	int i;
	for (i = 0; i < HAP_MAX_SESSIONS; i++) {
		if (hap_priv.sessions[i] == session) {
			/* Disable all characteristic notifications on this session */
			int fd = (hap_priv.sessions[i])->conn_identifier;
			hap_disable_all_char_notif(i);
			hap_priv.sessions[i] = NULL;
			//ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "HomeKit Session terminated");
			ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "HomeKit Session terminated, fd: %d", fd);
			break;
		}
	}
	hap_platform_memory_free(session);
}

static int hap_pair_verify_process_start(pair_verify_ctx_t *pv_ctx, uint8_t *buf, int inlen,
		int bufsize, int *outlen)
{
	/* Parse the data received from the controller */
	uint8_t state;
	if (!pv_ctx) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "No context");
		hap_prepare_error_tlv(STATE_M2, kTLVError_Unknown, buf, bufsize, outlen);
		return HAP_FAIL;
	}
	if ((get_value_from_tlv(buf, inlen, kTLVType_State, &state, sizeof(state)) < 0) ||
		(get_value_from_tlv(buf, inlen, kTLVType_PublicKey, pv_ctx->ctrl_curve_pk,
				    CURVE_KEY_LEN) < 0)) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Invalid TLVs received");
		hap_prepare_error_tlv(STATE_M2, kTLVError_Unknown, buf, bufsize, outlen);
		return HAP_FAIL;
	}
	if (state != STATE_M1) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Incorrect State received");
		hap_prepare_error_tlv(STATE_M2, kTLVError_Unknown, buf, bufsize, outlen);
		return HAP_FAIL;
	}

	ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Pair Verify M1 Received");
	hex_dbg_with_name("ctrl curve pk", pv_ctx->ctrl_curve_pk, 32);

	/* Generate a new Curve25519 Key Pair */
	uint8_t acc_curve_sk[CURVE_KEY_LEN];
	esp_mfi_get_random(acc_curve_sk, CURVE_KEY_LEN);
	/* This particular value of basepoint is required to generate the public key
	 * from secret key
	 */
	uint8_t basepoint[32] = {9};
    if (crypto_scalarmult_curve25519(pv_ctx->acc_curve_pk, acc_curve_sk, basepoint) == -1) {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Curve25519 Error");
		hap_prepare_error_tlv(STATE_M2, kTLVError_Unknown, buf, bufsize, outlen);
		return HAP_FAIL;
    }
	hex_dbg_with_name("acc curve sk", acc_curve_sk, 32);
	hex_dbg_with_name("acc curve pk", pv_ctx->acc_curve_pk, 32);
    if (crypto_scalarmult_curve25519(pv_ctx->shared_secret, acc_curve_sk, pv_ctx->ctrl_curve_pk) == -1) {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Curve25519 Error");
		hap_prepare_error_tlv(STATE_M2, kTLVError_Unknown, buf, bufsize, outlen);
		return HAP_FAIL;
    }

	hex_dbg_with_name("shared secret", pv_ctx->shared_secret, 32);

	/* Construct AccessoryInfo by concatenating
	 * a. Accessory's Curve25519 Public Key
	 * b. Accessory ID
	 * c. Controller's Curve25519 Public Key received in M1
	 */
	uint8_t acc_info[2 * CURVE_KEY_LEN + HAP_ACC_ID_LEN];
	int acc_info_len  = 0;
	memcpy(acc_info, pv_ctx->acc_curve_pk, CURVE_KEY_LEN);
	acc_info_len += CURVE_KEY_LEN;
	memcpy(acc_info + acc_info_len, hap_priv.acc_id, strlen(hap_priv.acc_id));
	acc_info_len += strlen(hap_priv.acc_id);
	memcpy(acc_info + acc_info_len, pv_ctx->ctrl_curve_pk, CURVE_KEY_LEN);
	acc_info_len += CURVE_KEY_LEN;

	hex_dbg_with_name("acc_info", acc_info, acc_info_len);

	/* Use Ed25519 to generate AccessorySignature by signing AccessoryInfo
	 * with its Long Term Secret Key AccessoryLTSK
	 */
	unsigned char ed_sign[64];
    unsigned long long ed_sign_len;
    crypto_sign_ed25519_detached(ed_sign, &ed_sign_len, acc_info, acc_info_len, hap_priv.ltska);
	hex_dbg_with_name("sign", ed_sign, 64);

	/* Construct a subTLV with
	 * kTLVType_Identifier : Accessory Identifier
	 * kTLVType_Signature : AccessorySignature generated above
	 */
	uint8_t subtlv[4 + HAP_ACC_ID_LEN + ED_SIGN_LEN + POLY_AUTHTAG_LEN];
	hap_tlv_data_t tlv_data;
	tlv_data.bufptr = subtlv;
	tlv_data.bufsize = sizeof(subtlv);
	tlv_data.curlen = 0;
	add_tlv(&tlv_data, kTLVType_Identifier, strlen(hap_priv.acc_id), hap_priv.acc_id);
	add_tlv(&tlv_data, kTLVType_Signature, sizeof(ed_sign), ed_sign);
	int subtlv_len = tlv_data.curlen;
	hex_dbg_with_name("subtlv", subtlv, subtlv_len);

	/* Derive Symmetric Session encryption key SessionKey from the curve
	 * shared secret using HKDF-SHA-512
	 */
	hkdf(SHA512, (unsigned char *) PAIR_VERIFY_ENCRYPT_SALT,
			strlen(PAIR_VERIFY_ENCRYPT_SALT),
			pv_ctx->shared_secret, sizeof(pv_ctx->shared_secret),
			(unsigned char *) PAIR_VERIFY_ENCRYPT_INFO,
			strlen(PAIR_VERIFY_ENCRYPT_INFO),
			pv_ctx->hkdf_key, sizeof(pv_ctx->hkdf_key));
	/* Encrypt the sub TLV to get encryptedData and an authTag using
	 * Chacha20-Poly1305 AEAD Algorithm
	 */
	uint8_t edata[4 + HAP_ACC_ID_LEN + ED_SIGN_LEN + POLY_AUTHTAG_LEN];
    unsigned long long mlen = 16;
    uint8_t newnonce[12];
    memset(newnonce, 0, sizeof newnonce);
    memcpy(newnonce+4, PV_NONCE1, 8);

    crypto_aead_chacha20poly1305_ietf_encrypt_detached(edata, edata + subtlv_len, &mlen, subtlv, subtlv_len, NULL, 0, NULL, newnonce, pv_ctx->hkdf_key);

	int edata_len = subtlv_len + POLY_AUTHTAG_LEN;

	hex_dbg_with_name("encrypt_data", edata, edata_len);

	/* Construct the response M2 */
	tlv_data.bufptr = buf;
	tlv_data.bufsize = bufsize;
	tlv_data.curlen = 0;
	state = STATE_M2;
	if ((add_tlv(&tlv_data, kTLVType_State, 1, &state) < 0) ||
			(add_tlv(&tlv_data, kTLVType_PublicKey, CURVE_KEY_LEN,
				 pv_ctx->acc_curve_pk) < 0) ||
			(add_tlv(&tlv_data, kTLVType_EncryptedData, edata_len, edata) < 0)) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "TLV creation failed");
		hap_prepare_error_tlv(STATE_M2, kTLVError_Unknown, buf, bufsize, outlen);
		return HAP_FAIL;
	}
	*outlen = tlv_data.curlen;
	hex_dbg_with_name("M2", buf, *outlen);
	pv_ctx->state = STATE_M2;
	ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Pair Verify M2 Successful");
	return HAP_SUCCESS;
}

static int hap_pair_verify_process_finish(pair_verify_ctx_t *pv_ctx, uint8_t *buf, int inlen,
		int bufsize, int *outlen)
{
	/* Parse the data received from the controller */
	uint8_t state;
	if (!pv_ctx) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "No context");
		hap_prepare_error_tlv(STATE_M4, kTLVError_Unknown, buf, bufsize, outlen);
		return HAP_FAIL;
	}
	uint8_t edata[4 + HAP_CTRL_ID_LEN + ED_SIGN_LEN + POLY_AUTHTAG_LEN];
	int edata_len;
	if ((get_value_from_tlv(buf, inlen, kTLVType_State, &state, sizeof(state)) < 0) ||
		((edata_len = get_value_from_tlv(buf, inlen, kTLVType_EncryptedData,
					 edata, sizeof(edata))) < 0)) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Invalid TLVs received");
		hap_prepare_error_tlv(STATE_M4, kTLVError_Unknown, buf, bufsize, outlen);
		return HAP_FAIL;
	}
	if (state != STATE_M3) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Incorrect State received");
		hap_prepare_error_tlv(STATE_M4, kTLVError_Unknown, buf, bufsize, outlen);
		return HAP_FAIL;
	}
	ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Pair Verify M3 Received");

    /* Decrypt the received data to get the subTLV */
    uint8_t newnonce[12];
    memset(newnonce, 0, sizeof newnonce);
    memcpy(newnonce+4, PV_NONCE2, 8);
    int ret;
    ret = crypto_aead_chacha20poly1305_ietf_decrypt_detached(edata, NULL, edata, edata_len - POLY_AUTHTAG_LEN, edata + edata_len - POLY_AUTHTAG_LEN, NULL, 0, newnonce, pv_ctx->hkdf_key); 
    if (ret != 0) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Decryption error");
		hap_prepare_error_tlv(STATE_M4, kTLVError_Authentication, buf, bufsize, outlen);
		return HAP_FAIL;
	}

	/* Parse the subTLV to get the iOSDevicePairingID and iOSDeviceSignature
	 */
	edata_len = edata_len - POLY_AUTHTAG_LEN;
    unsigned char ed_sign[64];
	char ctrl_id[HAP_CTRL_ID_LEN];
	memset(ctrl_id, 0, sizeof(ctrl_id));
	if ((get_value_from_tlv(edata, edata_len, kTLVType_Identifier,
					ctrl_id, sizeof(ctrl_id)) < 0) ||
			(get_value_from_tlv(edata, edata_len, kTLVType_Signature,
					ed_sign, sizeof(ed_sign)) < 0)) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Wrong subTLV received");
		hap_prepare_error_tlv(STATE_M4, kTLVError_Unknown, buf, bufsize, outlen);
		return HAP_FAIL;
	}

	/* Check if the controller is present in the database i.e. check
	 * if the controller was paired with the accessory
	 */
	hap_ctrl_data_t *ctrl = hap_get_controller(ctrl_id);
	if (!ctrl) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "No ctrl details found");
		hap_prepare_error_tlv(STATE_M4, kTLVError_Authentication, buf, bufsize, outlen);
		return HAP_FAIL;
	}

	/* Construct iOSDeviceInfo by concatenating
	 * a. Controllers's Curve25519 Public Key (received in M1)
	 * b. Controller ID
	 * c. Accessory's Curve25519 Public Key (sent in M2)
	 */
	uint8_t ios_dev_info[2 * CURVE_KEY_LEN + HAP_CTRL_ID_LEN];
	int ios_dev_info_len = 0;
	memcpy(ios_dev_info, pv_ctx->ctrl_curve_pk, CURVE_KEY_LEN);
	ios_dev_info_len += CURVE_KEY_LEN;
	memcpy(ios_dev_info + ios_dev_info_len, ctrl_id, strlen(ctrl_id));
	ios_dev_info_len += strlen(ctrl_id);
	memcpy(ios_dev_info + ios_dev_info_len, pv_ctx->acc_curve_pk,
			CURVE_KEY_LEN);
	ios_dev_info_len += CURVE_KEY_LEN;

	/* Validate the signature with the received iOSDeviceSignature */
    if (crypto_sign_ed25519_verify_detached(ed_sign, ios_dev_info, ios_dev_info_len, ctrl->info.ltpk) != 0) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Signature mismatch");
		hap_prepare_error_tlv(STATE_M4, kTLVError_Authentication, buf, bufsize, outlen);
		return HAP_FAIL;
	}

	/* Allocate memory for the secure session information */
	hap_secure_session_t *session = hap_platform_memory_calloc(sizeof(hap_secure_session_t), 1);
	if (!session) {
		hap_prepare_error_tlv(STATE_M4, kTLVError_Unknown, buf, bufsize, outlen);
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Memory allocation failed");
		return HAP_FAIL;
	}
	/* Construct the response M4 */
	hap_tlv_data_t tlv_data;
	tlv_data.bufptr = buf;
	tlv_data.bufsize = bufsize;
	tlv_data.curlen = 0;
	state = STATE_M4;
	if (add_tlv(&tlv_data, kTLVType_State, 1, &state) < 0) {
		hap_prepare_error_tlv(STATE_M4, kTLVError_Unknown, buf, bufsize, outlen);
		hap_platform_memory_free(session);
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "TLV creation failed");
		return HAP_FAIL;
	}
	*outlen = tlv_data.curlen;

	/* Generate the Encryption and Decryption Keys.
	 * Since, read and write are from the controller's point of view,
	 * encryption key uses READ_INFO and decryption key uses WRITE_INFO
	 *
	 * Also, set the nonce to zero
	 */
	hkdf(SHA512, (unsigned char *) CONTROL_SALT, strlen(CONTROL_SALT),
			pv_ctx->shared_secret, sizeof(pv_ctx->shared_secret),
			(unsigned char *) CONTROL_READ_INFO, strlen(CONTROL_READ_INFO),
			session->encrypt_key, sizeof(session->encrypt_key));

	hkdf(SHA512, (unsigned char *) CONTROL_SALT, strlen(CONTROL_SALT),
			pv_ctx->shared_secret, sizeof(pv_ctx->shared_secret),
			(unsigned char *) CONTROL_WRITE_INFO, strlen(CONTROL_WRITE_INFO),
			session->decrypt_key, sizeof(session->decrypt_key));

	session->state = STATE_VERIFIED;
	pv_ctx->state = STATE_VERIFIED;

	memset(session->encrypt_nonce, 0, sizeof(session->encrypt_nonce));
	memset(session->decrypt_nonce, 0, sizeof(session->decrypt_nonce));
	session->ctrl = ctrl;

	pv_ctx->session = session;

	/* Add the session information to database */
	hap_add_secure_session(session);
	ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Pair Verify Successful for %s", ctrl_id);
	return HAP_SUCCESS;
}

int hap_pair_verify_process(void **ctx, uint8_t *buf, int inlen, int bufsize, int *outlen)
{
	pair_verify_ctx_t *pv_ctx = (pair_verify_ctx_t *)(*ctx);
	if (pv_ctx) {
		if (pv_ctx->state == STATE_M0)
			return hap_pair_verify_process_start(pv_ctx, buf, inlen,
					bufsize, outlen);
		else if (pv_ctx->state == STATE_M2) {
			int ret = hap_pair_verify_process_finish(pv_ctx, buf, inlen,
					bufsize, outlen);
			/* Successful finish means that the pair verify was successful.
			 * So, we clear the old context and assign the secure_session
			 * as the new context
			 */
			if (ret == HAP_SUCCESS) {
				hap_secure_session_t *session = pv_ctx->session;
				hap_platform_memory_free(pv_ctx);
				*ctx = session;
			}
			return ret;
		}
	}
	ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Error processing Pair Verify Data");
	hap_prepare_error_tlv(STATE_M2, kTLVError_Unknown, buf, bufsize, outlen);
	return HAP_FAIL;
}

int hap_pair_verify_context_init(void **ctx, uint8_t *buf, int bufsize, int *outlen)
{
	pair_verify_ctx_t *pv_ctx;

	pv_ctx = (pair_verify_ctx_t *) hap_platform_memory_calloc(sizeof(pair_verify_ctx_t), 1);
	if (!pv_ctx) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Failed to create Pair Verify Context");
		hap_prepare_error_tlv(STATE_M2, kTLVError_Unknown, buf, bufsize, outlen);
		return HAP_FAIL;
	}
	*ctx = pv_ctx;
	ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "######## Starting Pair Verify ########");
	return HAP_SUCCESS;
}

uint8_t hap_pair_verify_get_state(void *ctx)
{
	pair_verify_ctx_t *pv_ctx = (pair_verify_ctx_t *)ctx;
	if (pv_ctx)
		return pv_ctx->state;
	return 0;
}
