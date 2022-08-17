/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#if MYNEWT_VAL(BLE_MESH)

#include "crypto.h"
#define MESH_LOG_MODULE BLE_MESH_LOG

static inline void xor16(uint8_t *dst, const uint8_t *a, const uint8_t *b)
{
	dst[0] = a[0] ^ b[0];
	dst[1] = a[1] ^ b[1];
	dst[2] = a[2] ^ b[2];
	dst[3] = a[3] ^ b[3];
	dst[4] = a[4] ^ b[4];
	dst[5] = a[5] ^ b[5];
	dst[6] = a[6] ^ b[6];
	dst[7] = a[7] ^ b[7];
	dst[8] = a[8] ^ b[8];
	dst[9] = a[9] ^ b[9];
	dst[10] = a[10] ^ b[10];
	dst[11] = a[11] ^ b[11];
	dst[12] = a[12] ^ b[12];
	dst[13] = a[13] ^ b[13];
	dst[14] = a[14] ^ b[14];
	dst[15] = a[15] ^ b[15];
}

/* pmsg is assumed to have the nonce already present in bytes 1-13 */
static int ccm_calculate_X0(const uint8_t key[16], const uint8_t *aad, uint8_t aad_len,
			    size_t mic_size, uint8_t msg_len, uint8_t b[16],
			    uint8_t X0[16])
{
	int i, j, err;

	/* X_0 = e(AppKey, flags || nonce || length) */
	b[0] = (((mic_size - 2) / 2) << 3) | ((!!aad_len) << 6) | 0x01;

	sys_put_be16(msg_len, b + 14);

	err = bt_encrypt_be(key, b, X0);
	if (err) {
		return err;
	}

	/* If AAD is being used to authenticate, include it here */
	if (aad_len) {
		sys_put_be16(aad_len, b);

		for (i = 0; i < sizeof(uint16_t); i++) {
			b[i] = X0[i] ^ b[i];
		}

		j = 0;
		aad_len += sizeof(uint16_t);
		while (aad_len > 16) {
			do {
				b[i] = X0[i] ^ aad[j];
				i++, j++;
			} while (i < 16);

			aad_len -= 16;
			i = 0;

			err = bt_encrypt_be(key, b, X0);
			if (err) {
				return err;
			}
		}

		for (; i < aad_len; i++, j++) {
			b[i] = X0[i] ^ aad[j];
		}

		for (i = aad_len; i < 16; i++) {
			b[i] = X0[i];
		}

		err = bt_encrypt_be(key, b, X0);
		if (err) {
			return err;
		}
	}

	return 0;
}

static int ccm_auth(const uint8_t key[16], uint8_t nonce[13],
		    const uint8_t *cleartext_msg, size_t msg_len, const uint8_t *aad,
		    size_t aad_len, uint8_t *mic, size_t mic_size)
{
	uint8_t b[16], Xn[16], s0[16];
	uint16_t blk_cnt, last_blk;
	int err, j, i;

	last_blk = msg_len % 16;
	blk_cnt = (msg_len + 15) / 16;
	if (!last_blk) {
		last_blk = 16U;
	}

	b[0] = 0x01;
	memcpy(b + 1, nonce, 13);

	/* S[0] = e(AppKey, 0x01 || nonce || 0x0000) */
	sys_put_be16(0x0000, &b[14]);

	err = bt_encrypt_be(key, b, s0);
	if (err) {
		return err;
	}

	ccm_calculate_X0(key, aad, aad_len, mic_size, msg_len, b, Xn);

	for (j = 0; j < blk_cnt; j++) {
		/* X_1 = e(AppKey, X_0 ^ Payload[0-15]) */
		if (j + 1 == blk_cnt) {
			for (i = 0; i < last_blk; i++) {
				b[i] = Xn[i] ^ cleartext_msg[(j * 16) + i];
			}

			memcpy(&b[i], &Xn[i], 16 - i);
		} else {
			xor16(b, Xn, &cleartext_msg[j * 16]);
		}

		err = bt_encrypt_be(key, b, Xn);
		if (err) {
			return err;
		}
	}

	/* MIC = C_mic ^ X_1 */
	for (i = 0; i < mic_size; i++) {
		mic[i] = s0[i] ^ Xn[i];
	}

	return 0;
}

static int ccm_crypt(const uint8_t key[16], const uint8_t nonce[13],
		     const uint8_t *in_msg, uint8_t *out_msg, size_t msg_len)
{
	uint8_t a_i[16], s_i[16];
	uint16_t last_blk, blk_cnt;
	size_t i, j;
	int err;

	last_blk = msg_len % 16;
	blk_cnt = (msg_len + 15) / 16;
	if (!last_blk) {
		last_blk = 16U;
	}

	a_i[0] = 0x01;
	memcpy(&a_i[1], nonce, 13);

	for (j = 0; j < blk_cnt; j++) {
		/* S_1 = e(AppKey, 0x01 || nonce || 0x0001) */
		sys_put_be16(j + 1, &a_i[14]);

		err = bt_encrypt_be(key, a_i, s_i);
		if (err) {
			return err;
		}

		/* Encrypted = Payload[0-15] ^ C_1 */
		if (j < blk_cnt - 1) {
			xor16(&out_msg[j * 16], s_i, &in_msg[j * 16]);
		} else {
			for (i = 0; i < last_blk; i++) {
				out_msg[(j * 16) + i] =
					in_msg[(j * 16) + i] ^ s_i[i];
			}
		}
	}
	return 0;
}

int bt_ccm_decrypt(const uint8_t key[16], uint8_t nonce[13], const uint8_t *enc_msg,
		   size_t msg_len, const uint8_t *aad, size_t aad_len,
		   uint8_t *out_msg, size_t mic_size)
{
	uint8_t mic[16];

	if (aad_len >= 0xff00 || mic_size > sizeof(mic)) {
		return -EINVAL;
	}

	ccm_crypt(key, nonce, enc_msg, out_msg, msg_len);

	ccm_auth(key, nonce, out_msg, msg_len, aad, aad_len, mic, mic_size);

	if (memcmp(mic, enc_msg + msg_len, mic_size)) {
		return -EBADMSG;
	}

	return 0;
}

int bt_ccm_encrypt(const uint8_t key[16], uint8_t nonce[13], const uint8_t *msg,
		   size_t msg_len, const uint8_t *aad, size_t aad_len,
		   uint8_t *out_msg, size_t mic_size)
{
	uint8_t *mic = out_msg + msg_len;

	BT_DBG("key %s", bt_hex(key, 16));
	BT_DBG("nonce %s", bt_hex(nonce, 13));
	BT_DBG("msg (len %zu) %s", msg_len, bt_hex(msg, msg_len));
	BT_DBG("aad_len %zu mic_size %zu", aad_len, mic_size);

	/* Unsupported AAD size */
	if (aad_len >= 0xff00 || mic_size > 16) {
		return -EINVAL;
	}

	ccm_auth(key, nonce, out_msg, msg_len, aad, aad_len, mic, mic_size);

	ccm_crypt(key, nonce, msg, out_msg, msg_len);

	return 0;
}

#endif /* MYNEWT_VAL(BLE_MESH) */