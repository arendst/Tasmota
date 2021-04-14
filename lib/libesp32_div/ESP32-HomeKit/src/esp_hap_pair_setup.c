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
#include <stdbool.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include <mu_srp.h>
#include <sodium/crypto_aead_chacha20poly1305.h>
#include <hkdf-sha.h>
#include <sodium/crypto_sign_ed25519.h>
#include <hexdump.h>
#include <hap_platform_memory.h>
#include <hap_platform_os.h>

#include <esp_hap_pair_common.h>
#include <esp_hap_pair_setup.h>
#include <esp_hap_database.h>
#include <esp_hap_main.h>
#include <esp_hap_acc.h>
#include <esp_mfi_debug.h>

/* Maximum attempts allowed for Pair Setup, as per HAP Specifications */
#define HAP_PAIR_SETUP_MAX_ATTEMPTS	100

#define PAIR_SETUP_ENCRYPT_SALT		"Pair-Setup-Encrypt-Salt"
#define PAIR_SETUP_ENCRYPT_INFO		"Pair-Setup-Encrypt-Info"
#define PAIR_SETUP_CTRL_SIGN_SALT	"Pair-Setup-Controller-Sign-Salt"
#define PAIR_SETUP_CTRL_SIGN_INFO	"Pair-Setup-Controller-Sign-Info"
#define PAIR_SETUP_ACC_SIGN_SALT	"Pair-Setup-Accessory-Sign-Salt"
#define PAIR_SETUP_ACC_SIGN_INFO	"Pair-Setup-Accessory-Sign-Info"

#define PS_CTX_INIT	1
#define PS_CTX_DEINIT	2

/* Timeout if pair setup not completed in 1 minute (60sec)*/
#define HAP_SETUP_TIMEOUT_IN_TICKS  ((40 * 1000) / hap_platform_os_get_msec_per_tick())

static int hap_pair_setup_process_srp_start(pair_setup_ctx_t *ps_ctx, uint8_t *buf, int inlen,
		int bufsize, int *outlen)
{
	/* Pair setup is not allowed if the accessory is already paired */
	if (is_accessory_paired()) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Accessory is already paired. "
				"Please use \"Add Pairing\" to add more controllers");
		hap_prepare_error_tlv(STATE_M2, kTLVError_Unavailable, buf, bufsize, outlen);
		return HAP_FAIL;
	}

	/* Pair Setup is not allowed if the failed attempts have exceeded the
	 * maximum allowed attempts.
	 */
	if (hap_priv.pair_attempts >= HAP_PAIR_SETUP_MAX_ATTEMPTS) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Too many attempts. Aborting");
		hap_prepare_error_tlv(STATE_M2, kTLVError_MaxTries, buf, bufsize, outlen);
		return HAP_FAIL;
	}

	ps_ctx->ctrl = hap_controller_get_empty_loc();
	if (!ps_ctx->ctrl) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "No empty controller slot. Aborting");
		hap_prepare_error_tlv(STATE_M2, kTLVError_Unknown, buf, bufsize, outlen);
		return HAP_FAIL;
	}

	uint8_t state;
	if ((get_value_from_tlv(buf, inlen, kTLVType_State, &state, sizeof(state)) < 0) ||
		(get_value_from_tlv(buf, inlen, kTLVType_Method,
				    &ps_ctx->method, sizeof(ps_ctx->method)) < 0)) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Invalid TLVs received");
		hap_prepare_error_tlv(STATE_M2, kTLVError_Unknown, buf, bufsize, outlen);
		return HAP_FAIL;
	}
	if (state != STATE_M1) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Incorrect State received");
		hap_prepare_error_tlv(STATE_M2, kTLVError_Unknown, buf, bufsize, outlen);
		return HAP_FAIL;
	}
	ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Pair Setup M1 Received");

    int flags_len;
    if ((flags_len = get_value_from_tlv(buf, inlen, kTLVType_Flags, &ps_ctx->pairing_flags, sizeof(ps_ctx->pairing_flags))) > 0) {
        ps_ctx->pairing_flags_len = flags_len;
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Got pairing flags %x", ps_ctx->pairing_flags);

        /* If the Split pairing flag is not set, it is an error */
        if (!(ps_ctx->pairing_flags & PAIR_FLAG_SPLIT)) {
            ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Pairing Flags received, but the Split flag is not set");
            hap_prepare_error_tlv(STATE_M2, kTLVError_Unknown, buf, bufsize, outlen);
            return HAP_FAIL;
        }

        /* If either the current or the previous request didn't have transient flag set, return authentication error */
        if (!((ps_ctx->pairing_flags | hap_priv.pairing_flags) & PAIR_FLAG_TRANSIENT)) {
            ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Split pairing received before preceding Transient Pairing");
            hap_prepare_error_tlv(STATE_M2, kTLVError_Authentication, buf, bufsize, outlen);
            return HAP_FAIL;
        }
    }
    hap_priv.pairing_flags = ps_ctx->pairing_flags;

	int len_B = 0;
	char *bytes_B;

	/* Create SRP Salt and Verifier for the provided pairing PIN */
    mu_srp_init(&ps_ctx->srp_hd, MU_NG_3072);

    /* If a setup code is explicitly set, use it */
    if (hap_priv.setup_code) {
        ps_ctx->len_s = 16;
        mu_srp_srv_pubkey(&ps_ctx->srp_hd, "Pair-Setup", (const char*)hap_priv.setup_code, strlen(hap_priv.setup_code),
                ps_ctx->len_s, &bytes_B, &len_B, &ps_ctx->bytes_s);
    } else {
        /* Else, use the salt and verifier for SRP. This should be the default production case
         */
        if (mu_srp_set_salt_verifier(&ps_ctx->srp_hd, (char *)hap_priv.setup_info->salt, sizeof(hap_priv.setup_info->salt),
                    (char *)hap_priv.setup_info->verifier, sizeof(hap_priv.setup_info->verifier)) < 0) {
            ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "SRP-6a Salt-Verifier Init Failed");
            hap_prepare_error_tlv(STATE_M2, kTLVError_Unknown, buf, bufsize, outlen);
            return HAP_FAIL;
        }
        ps_ctx->bytes_s = (char *)hap_priv.setup_info->salt;
        ps_ctx->len_s = sizeof(hap_priv.setup_info->salt);
        mu_srp_srv_pubkey_from_salt_verifier(&ps_ctx->srp_hd, &bytes_B, &len_B);
    }
	if (!ps_ctx->bytes_s || !bytes_B) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "SRP-6a Verifier Creation Failed");
		hap_prepare_error_tlv(STATE_M2, kTLVError_Unknown, buf, bufsize, outlen);
		return HAP_FAIL;
	}

	hex_dbg_with_name("salt", (uint8_t *)ps_ctx->bytes_s, ps_ctx->len_s);
	hex_dbg_with_name("acc_srp_public_key", (uint8_t *)bytes_B, len_B);

	/* Construct the response M2 */
	hap_tlv_data_t tlv_data;
	tlv_data.bufptr = buf;
	tlv_data.bufsize = bufsize;
	tlv_data.curlen = 0;
	state = STATE_M2;
	if ((add_tlv(&tlv_data, kTLVType_State, 1, &state) < 0) ||
			(add_tlv(&tlv_data, kTLVType_PublicKey, len_B, (void *)bytes_B) < 0) ||
			(add_tlv(&tlv_data, kTLVType_Salt, ps_ctx->len_s, ps_ctx->bytes_s) < 0 )) {
		hap_prepare_error_tlv(STATE_M2, kTLVError_Unknown, buf, bufsize, outlen);
		return HAP_FAIL;
	}
    /* Not adding any error check here, because without the pairing flags, the pairing will
     * anyways fail later.
     */
    if (ps_ctx->pairing_flags_len) {
        add_tlv(&tlv_data, kTLVType_Flags, ps_ctx->pairing_flags_len, &ps_ctx->pairing_flags);
    }
	*outlen = tlv_data.curlen;
	ps_ctx->state = state;
	ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Pair Setup M2 Successful");
    hap_report_event(HAP_EVENT_PAIRING_STARTED, NULL, 0);
	return HAP_SUCCESS;
}


static int hap_pair_setup_process_srp_verify(pair_setup_ctx_t *ps_ctx, uint8_t *buf, int inlen,
		int bufsize, int *outlen)
{
	uint8_t state;
	char ctrl_public_key[384];
	int ctrl_public_key_len;
	char ctrl_proof[64];
	int ctrl_proof_len;

	if ((get_value_from_tlv(buf, inlen, kTLVType_State, &state, sizeof(state)) < 0) ||
		((ctrl_public_key_len = get_value_from_tlv(buf, inlen, kTLVType_PublicKey,
				ctrl_public_key, sizeof(ctrl_public_key))) < 0) ||
		((ctrl_proof_len = get_value_from_tlv(buf, inlen, kTLVType_Proof,
				ctrl_proof, sizeof(ctrl_proof))) < 0)) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Invalid TLVs received");
		hap_prepare_error_tlv(STATE_M4, kTLVError_Unknown, buf, bufsize, outlen);
		return HAP_FAIL;
	}
	if (state != STATE_M3) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Incorrect State received");
		hap_prepare_error_tlv(STATE_M4, kTLVError_Unknown, buf, bufsize, outlen);
		return HAP_FAIL;
	}
	ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Pair Setup M3 Received");

	hex_dbg_with_name("ctrl_srp_public_key", (uint8_t *)ctrl_public_key, ctrl_public_key_len);
	hex_dbg_with_name("ctrl_proof", (uint8_t *)ctrl_proof, ctrl_proof_len);
    mu_srp_get_session_key(&ps_ctx->srp_hd, ctrl_public_key, ctrl_public_key_len, &ps_ctx->shared_secret, &ps_ctx->secret_len);
    char host_proof[SHA512HashSize];
    int ret = mu_srp_exchange_proofs(&ps_ctx->srp_hd, "Pair-Setup", ctrl_proof, host_proof);
    if (ret != 1) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "SRP Verify: Controller Authentication failed");
		hap_prepare_error_tlv(STATE_M4, kTLVError_Authentication, buf, bufsize, outlen);
        hap_report_event(HAP_EVENT_PAIRING_ABORTED, NULL, 0);
		return HAP_FAIL;
    }
    int acc_proof_length = SHA512HashSize;

	hkdf(SHA512, (uint8_t *) PAIR_SETUP_ENCRYPT_SALT, strlen(PAIR_SETUP_ENCRYPT_SALT),
			(uint8_t *)ps_ctx->shared_secret, ps_ctx->secret_len,
			(uint8_t *) PAIR_SETUP_ENCRYPT_INFO, strlen(PAIR_SETUP_ENCRYPT_INFO),
			ps_ctx->session_key, sizeof(ps_ctx->session_key));

	/* Construct the response M4 */
	hap_tlv_data_t tlv_data;
	tlv_data.bufptr = buf;
	tlv_data.bufsize = bufsize;
	tlv_data.curlen = 0;
	state = STATE_M4;
	if ((add_tlv(&tlv_data, kTLVType_State, 1, &state) < 0) ||
			(add_tlv(&tlv_data, kTLVType_Proof, acc_proof_length,
				 (void *)host_proof) < 0)) {
		hap_prepare_error_tlv(STATE_M4, kTLVError_Unknown, buf, bufsize, outlen);
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "TLV creation failed");
		return HAP_FAIL;
    }
    hap_tlv_error_t tlv_error = 0;
    if (hap_pair_setup_manage_mfi_auth(ps_ctx, &tlv_data, &tlv_error) != ESP_OK) {
        hap_prepare_error_tlv(STATE_M4, tlv_error, buf, bufsize, outlen);
        return HAP_FAIL;
    }
	*outlen = tlv_data.curlen;
	ps_ctx->state = state;
	ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Pair Setup M4 Successful");
	return HAP_SUCCESS;
}
static int hap_pair_setup_process_exchange(pair_setup_ctx_t *ps_ctx, uint8_t *buf, int inlen,
		int bufsize, int *outlen)
{
	uint8_t state;
	uint8_t edata[220];
	int edata_len;
    int ret;

	if ((get_value_from_tlv(buf, inlen, kTLVType_State, &state, sizeof(state)) < 0) ||
		((edata_len = get_value_from_tlv(buf, inlen, kTLVType_EncryptedData,
				edata, sizeof(edata))) < 0))  {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Invalid TLVs received");
		hap_prepare_error_tlv(STATE_M6, kTLVError_Unknown, buf, bufsize, outlen);
		return HAP_FAIL;
	}
	hex_dbg_with_name("recv_encrypted_data", edata, edata_len);
	if (state != STATE_M5) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Incorrect State received");
		hap_prepare_error_tlv(STATE_M6, kTLVError_Unknown, buf, bufsize, outlen);
		return HAP_FAIL;
    }
	ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Pair Setup M5 Received");
    edata_len -= 16; /* 16 bytes for the authTag */
    uint8_t newnonce[12];
    memset(newnonce, 0, sizeof newnonce);
    memcpy(newnonce+4, (uint8_t *)PS_NONCE2, 8);
    ret = crypto_aead_chacha20poly1305_ietf_decrypt_detached(edata, NULL, edata, edata_len,
            &edata[edata_len], NULL, 0, newnonce, ps_ctx->session_key);
    if (ret != 0) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Decryption Failed");
		hap_prepare_error_tlv(STATE_M6, kTLVError_Authentication, buf, bufsize, outlen);
		return HAP_FAIL;
	}
	hex_dbg_with_name("subtlv", edata, edata_len);
	int ctrl_id_len;
	unsigned char ed_sign[64];
    unsigned long long ed_sign_len;
	if (((ctrl_id_len = get_value_from_tlv(edata, edata_len, kTLVType_Identifier,
					ps_ctx->ctrl->info.id, sizeof(ps_ctx->ctrl->info.id))) < 0) ||
			(get_value_from_tlv(edata, edata_len, kTLVType_PublicKey,
					    ps_ctx->ctrl->info.ltpk, ED_KEY_LEN) != ED_KEY_LEN) ||
			(get_value_from_tlv(edata, edata_len, kTLVType_Signature,
					    ed_sign, sizeof(ed_sign)) != sizeof(ed_sign))) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Invalid subTLV received");
		hap_prepare_error_tlv(STATE_M6, kTLVError_Authentication, buf, bufsize, outlen);
		return HAP_FAIL;
	}
	ps_ctx->ctrl->info.id[ctrl_id_len] = 0; /* NULL termination */
	hex_dbg_with_name("ctrl_id", (uint8_t *)ps_ctx->ctrl->info.id, ctrl_id_len);
	hex_dbg_with_name("ltpkc", ps_ctx->ctrl->info.ltpk, ED_KEY_LEN);
	hex_dbg_with_name("ctrl_sign", ed_sign, sizeof(ed_sign));

	/* Derive iOSDeviceX from SRP shared secret using HKDF-SHA512 */
	uint8_t ios_device_x[32];
	hkdf(SHA512, (unsigned char *) PAIR_SETUP_CTRL_SIGN_SALT,
			strlen(PAIR_SETUP_CTRL_SIGN_SALT), (uint8_t *)ps_ctx->shared_secret, ps_ctx->secret_len,
			(unsigned char *) PAIR_SETUP_CTRL_SIGN_INFO,
			strlen(PAIR_SETUP_CTRL_SIGN_INFO),
			ios_device_x, sizeof(ios_device_x));
	/* Construct iOSDeviceInfo by concatenating
	 * iOSDeviceX
	 * iOSDevicePairingID (ctrl_id)
	 * iOSDeviceLTPK (ltpkc)
	 */
	uint8_t info_buf[HAP_CTRL_ID_LEN + 32 + ED_KEY_LEN];
	uint8_t *ios_dev_info = info_buf;
	int ios_dev_info_len = 0;
	memcpy(ios_dev_info, ios_device_x, sizeof(ios_device_x));
	ios_dev_info_len += sizeof(ios_device_x);
	memcpy(&ios_dev_info[ios_dev_info_len], ps_ctx->ctrl->info.id, ctrl_id_len);
	ios_dev_info_len += ctrl_id_len;
	memcpy(&ios_dev_info[ios_dev_info_len], ps_ctx->ctrl->info.ltpk, ED_KEY_LEN);
	ios_dev_info_len += ED_KEY_LEN;

    ret = crypto_sign_ed25519_verify_detached(ed_sign, ios_dev_info, ios_dev_info_len, ps_ctx->ctrl->info.ltpk);
	/* Verify Signature of constructed iOSDeviceInfo using the iOSDeviceLTPK */
    if (ret != 0) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Invalid Signature");
		hap_prepare_error_tlv(STATE_M6, kTLVError_Authentication, buf, bufsize, outlen);
		return HAP_FAIL;
	}

	hex_dbg_with_name("ltpka", hap_priv.ltpka, sizeof(hap_priv.ltpka));

	/* Derive AccessoryX from the SRP shared secret using HKDF-SHA512 */
	uint8_t acc_x[32];
	hkdf(SHA512, (unsigned char *) PAIR_SETUP_ACC_SIGN_SALT,
			strlen(PAIR_SETUP_ACC_SIGN_SALT), (uint8_t *)ps_ctx->shared_secret, ps_ctx->secret_len,
			(unsigned char *) PAIR_SETUP_ACC_SIGN_INFO,
			strlen(PAIR_SETUP_ACC_SIGN_INFO),
			acc_x, sizeof(acc_x));
	/* Construct AccessoryInfo by concatenating
	 * AccessoryX
	 * AccessoryPairingID (acc_id)
	 * AccessoryLTPK (ltpka)
	 */
	uint8_t *acc_info = info_buf;
	int acc_info_len = 0;
	memcpy(acc_info, acc_x, sizeof(acc_x));
	acc_info_len += sizeof(acc_x);
	memcpy(&ios_dev_info[acc_info_len], hap_priv.acc_id, strlen(hap_priv.acc_id));
	acc_info_len += strlen(hap_priv.acc_id);
	memcpy(&ios_dev_info[acc_info_len], hap_priv.ltpka, sizeof(hap_priv.ltpka));
	acc_info_len += sizeof(hap_priv.ltpka);

	/* Generate AccessorySignature by signing AccessoryInfo with AccessoryLTSK
	 */
    crypto_sign_ed25519_detached(ed_sign, &ed_sign_len, acc_info, acc_info_len, hap_priv.ltska);
	hex_dbg_with_name("acc_sign", ed_sign, sizeof(ed_sign));

	/* Create subTLV with:
	 * kTLVType_Identifier : Accessory ID (acc_id)
	 * kTLVType_PublicKey : Accessory LTPK
	 * kTLVType_Signature : AccessorySignature
	 */

	uint8_t subtlv[6 + HAP_ACC_ID_LEN + ED_KEY_LEN + ED_SIGN_LEN + POLY_AUTHTAG_LEN];

	hap_tlv_data_t tlv_data;
	tlv_data.bufptr = subtlv;
	tlv_data.bufsize = sizeof(subtlv);
	tlv_data.curlen = 0;
	add_tlv(&tlv_data, kTLVType_Identifier, strlen(hap_priv.acc_id), hap_priv.acc_id);
	add_tlv(&tlv_data, kTLVType_PublicKey, sizeof(hap_priv.ltpka), hap_priv.ltpka);
	add_tlv(&tlv_data, kTLVType_Signature, sizeof(ed_sign), ed_sign);
	int subtlv_len = tlv_data.curlen;
	hex_dbg_with_name("subtlv", subtlv, subtlv_len);

	/* Encrypt the subTLV using the session key */

    unsigned long long mlen = 16;
    memset(newnonce, 0, sizeof newnonce);
    memcpy(newnonce+4, (uint8_t *) PS_NONCE3, 8);
    crypto_aead_chacha20poly1305_ietf_encrypt_detached(subtlv, &subtlv[subtlv_len], &mlen, subtlv,
                subtlv_len, NULL, 0, NULL, newnonce, ps_ctx->session_key); 
	hex_dbg_with_name("send_encrypt_data", subtlv, subtlv_len + 16);

	/* Construct the response M6 */
	tlv_data.bufptr = buf;
	tlv_data.bufsize = bufsize;
	tlv_data.curlen = 0;
	state = STATE_M6;
	if ((add_tlv(&tlv_data, kTLVType_State, 1, &state) < 0) ||
			(add_tlv(&tlv_data, kTLVType_EncryptedData, subtlv_len + 16,
				 subtlv) < 0)) {
		hap_prepare_error_tlv(STATE_M4, kTLVError_Unknown, buf, bufsize, outlen);
		return HAP_FAIL;
	}
	*outlen = tlv_data.curlen;
	ps_ctx->state = state;
	ps_ctx->ctrl->info.perms = 1; /* Controller added using pair setup is always an admin */
	ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Pair Setup Successful for %s", ps_ctx->ctrl->info.id);
    /* Reset the Pairing Attempts count */
	hap_priv.pair_attempts = 0;
    hap_controller_save(ps_ctx->ctrl);
    hap_send_event(HAP_INTERNAL_EVENT_ACC_PAIRED);
	return HAP_SUCCESS;
}
static uint8_t hap_pair_setup_get_received_state(uint8_t *buf, int inlen)
{
	uint8_t state = 0;
	get_value_from_tlv(buf, inlen, kTLVType_State, &state, sizeof(state));
	return state;
}

static void hap_pair_setup_timeout(TimerHandle_t handle)
{
    ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR, "Clearing Pair Setup Context due to inactivity");
    pair_setup_ctx_t *ps_ctx = pvTimerGetTimerID(handle);
    if (ps_ctx) {
        httpd_sess_trigger_close(hap_priv.server, ps_ctx->sock_fd);
        hap_report_event(HAP_EVENT_PAIRING_ABORTED, NULL, 0);
    }
}

/* Pair Setup can have only a single context at a time as the specification does not allow
 * multiple controllers to perform Pair Setup Simultaneously.
 *
 * The purpose of this function is to manage allocation and resetting of the context by
 * using a static variable rather than a global variable.
 */
static pair_setup_ctx_t *hap_pair_setup_ctx_action(int action)
{
	static pair_setup_ctx_t *ps_ctx;
	/* If the call is to initialise the context, check if it is already
	 * initialised.
	 */
	if (action == PS_CTX_INIT) {
		/* If the context is already initialised, it means that pair setup
		 * is already in progress. So, return NULL.
		 * Else, allocate the context.
		 */
		if (ps_ctx)
			return NULL;
		else {
			ps_ctx = hap_platform_memory_calloc(sizeof(pair_setup_ctx_t), 1);
            if (ps_ctx) {
                ps_ctx->timer = xTimerCreate("hap_setup_timer", HAP_SETUP_TIMEOUT_IN_TICKS,
                            pdFALSE, (void *) ps_ctx, hap_pair_setup_timeout);
                xTimerStart(ps_ctx->timer, 0);
            }
        }
	} else if (action == PS_CTX_DEINIT) {
		/* If the call is to de-initialise the context, clean it up
		 * and set the pointer to NULL
		 */
		if (ps_ctx) {
            if (ps_ctx->timer) {
                xTimerStop(ps_ctx->timer, 0);
                xTimerDelete(ps_ctx->timer, 100);
            }
            mu_srp_free(&ps_ctx->srp_hd);
			hap_platform_memory_free(ps_ctx);
		}
		ps_ctx = NULL;
	}
	return ps_ctx;
}


void hap_pair_setup_ctx_clean(void *sess_ctx)
{
	if (sess_ctx) {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Cleaning Pair Setup Context");
		hap_pair_setup_ctx_action(PS_CTX_DEINIT);
    }
}

int hap_pair_setup_context_init(int sock_fd, void **ctx, uint8_t *buf, int bufsize, int *outlen)
{
	pair_setup_ctx_t *ps_ctx = hap_pair_setup_ctx_action(PS_CTX_INIT);

	if (!ps_ctx) {
        ESP_MFI_DEBUG(ESP_MFI_DEBUG_ERR,"######## Aborted! Pair Setup in Progress with another controller ########");
		hap_prepare_error_tlv(STATE_M2, kTLVError_Busy, buf, bufsize, outlen);
		return HAP_FAIL;
	}
    ps_ctx->sock_fd = sock_fd;
	*ctx = ps_ctx;
	ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "######## Starting Pair Setup ########");
	return HAP_SUCCESS;

}
int hap_pair_setup_process(void **ctx, uint8_t *buf, int inlen, int bufsize, int *outlen)
{
	pair_setup_ctx_t *ps_ctx = (pair_setup_ctx_t *)(*ctx);

	uint8_t recv_state = hap_pair_setup_get_received_state(buf, inlen);
	if (!ps_ctx) {
		hap_prepare_error_tlv(recv_state + 1, kTLVError_Unknown, buf, bufsize, outlen);
		return HAP_FAIL;
	}
	/* Receiving STATE_M1 in the message while the pair-setup was already in progress means
	 * that pair setup was restarted. Handle it accordingly
	 */
	if ((recv_state == STATE_M1) && (ps_ctx->state != STATE_M0)) {
		ESP_MFI_DEBUG(ESP_MFI_DEBUG_INFO, "Restarting Pair Setup");
        int sock_fd = ps_ctx->sock_fd;
		hap_pair_setup_ctx_clean(ps_ctx);
		int ret = hap_pair_setup_context_init(sock_fd, ctx, buf, bufsize, outlen);
		if (ret != HAP_SUCCESS)
			return ret;
		ps_ctx = (pair_setup_ctx_t *)(*ctx);
	}
	if (ps_ctx->state == STATE_M0) {
		return hap_pair_setup_process_srp_start(ps_ctx, buf, inlen, bufsize, outlen);
	} else if (ps_ctx->state == STATE_M2) {
		hap_priv.pair_attempts++;
		int ret = hap_pair_setup_process_srp_verify(ps_ctx, buf, inlen, bufsize, outlen);
        if (ps_ctx->session) {
            *ctx = ps_ctx->session;
            hap_pair_setup_ctx_clean(ps_ctx);
        }
        return ret;
	} else if (ps_ctx->state == STATE_M4) {
		int ret = hap_pair_setup_process_exchange(ps_ctx, buf, inlen, bufsize, outlen);
		/* If last step of pair setup is successful, it means that the context would
		 * be no more required. Hence, clear it.
		 */
		if (ret == HAP_SUCCESS) {
			hap_pair_setup_ctx_clean(ps_ctx);
			*ctx = NULL;
		}
		return ret;
	}
	hap_prepare_error_tlv(STATE_M2, kTLVError_Unknown, buf, bufsize, outlen);
	return HAP_FAIL;
}

void hap_set_setup_code(const char *setup_code)
{
    if (hap_priv.setup_code)
        hap_platform_memory_free(hap_priv.setup_code);
    hap_priv.setup_code = strdup(setup_code);
}

int hap_set_setup_info(const hap_setup_info_t *setup_info)
{
    if (!setup_info)
        return HAP_FAIL;
    if (hap_priv.setup_info)
        hap_platform_memory_free(hap_priv.setup_info);
    hap_priv.setup_info = hap_platform_memory_calloc(1, sizeof(hap_setup_info_t));
    if (!hap_priv.setup_info)
        return HAP_FAIL;
    memcpy(hap_priv.setup_info, setup_info, sizeof(hap_setup_info_t));
    return HAP_SUCCESS;
}

int hap_set_setup_id(const char *setup_id)
{
    if (setup_id == NULL || strlen(setup_id) != SETUP_ID_LEN)
        return HAP_FAIL;
    strcpy(hap_priv.setup_id, setup_id);
    return HAP_SUCCESS;
}
