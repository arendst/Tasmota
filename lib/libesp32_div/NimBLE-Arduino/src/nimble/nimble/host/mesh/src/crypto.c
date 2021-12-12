/*  Bluetooth Mesh */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#if MYNEWT_VAL(BLE_MESH)

#define MESH_LOG_MODULE BLE_MESH_CRYPTO_LOG

#include <string.h>
#include <stdbool.h>
#include <errno.h>

#if (MYNEWT_VAL(BLE_CRYPTO_STACK_MBEDTLS))
#include "mbedtls/aes.h"
#include "mbedtls/cipher.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/cmac.h"
#include "mbedtls/ecdh.h"
#include "mbedtls/ecp.h"

#else
#include "nimble/ext/tinycrypt/include/tinycrypt/constants.h"
#include "nimble/ext/tinycrypt/include/tinycrypt/utils.h"
#include "nimble/ext/tinycrypt/include/tinycrypt/aes.h"
#include "nimble/ext/tinycrypt/include/tinycrypt/cmac_mode.h"
#include "nimble/ext/tinycrypt/include/tinycrypt/ccm_mode.h"
#endif

#include "crypto.h"

#define NET_MIC_LEN(pdu) (((pdu)[1] & 0x80) ? 8 : 4)
#define APP_MIC_LEN(aszmic) ((aszmic) ? 8 : 4)

#if MYNEWT_VAL(BLE_CRYPTO_STACK_MBEDTLS)
int bt_mesh_aes_cmac(const u8_t key[16], struct bt_mesh_sg *sg,
		     size_t sg_len, u8_t mac[16])
{
    int rc = BLE_HS_EUNKNOWN;
    mbedtls_cipher_context_t ctx = {0};
    const mbedtls_cipher_info_t *cipher_info;

    mbedtls_cipher_init(&ctx);

    cipher_info = mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_128_ECB);
    if (cipher_info == NULL) {
        goto exit;
    }

    if (mbedtls_cipher_setup(&ctx, cipher_info) != 0) {
        goto exit;
    }

    rc = mbedtls_cipher_cmac_starts(&ctx, key, 128);
    if (rc != 0) {
        goto exit;
    }

    for (; sg_len; sg_len--, sg++) {
        if (sg->len != 0 && sg->data != NULL) {
            if ((rc = mbedtls_cipher_cmac_update(&ctx, sg->data, sg->len)) != 0) {
                goto exit;
            }
        }
    }
    rc = mbedtls_cipher_cmac_finish(&ctx, mac);

exit:
    mbedtls_cipher_free(&ctx);
    if (rc != 0) {
        return -EIO;
    }

    return 0;
}

#else
int bt_mesh_aes_cmac(const u8_t key[16], struct bt_mesh_sg *sg,
		     size_t sg_len, u8_t mac[16])
{
	struct tc_aes_key_sched_struct sched;
	struct tc_cmac_struct state;

	if (tc_cmac_setup(&state, key, &sched) == TC_CRYPTO_FAIL) {
		return -EIO;
	}

	for (; sg_len; sg_len--, sg++) {
		if (tc_cmac_update(&state, sg->data,
				   sg->len) == TC_CRYPTO_FAIL) {
			return -EIO;
		}
	}

	if (tc_cmac_final(mac, &state) == TC_CRYPTO_FAIL) {
		return -EIO;
	}

	return 0;
}
#endif

int bt_mesh_k1(const u8_t *ikm, size_t ikm_len, const u8_t salt[16],
	       const char *info, u8_t okm[16])
{
	int err;

	err = bt_mesh_aes_cmac_one(salt, ikm, ikm_len, okm);
	if (err < 0) {
		return err;
	}

	return bt_mesh_aes_cmac_one(okm, info, strlen(info), okm);
}

int bt_mesh_k2(const u8_t n[16], const u8_t *p, size_t p_len,
	       u8_t net_id[1], u8_t enc_key[16], u8_t priv_key[16])
{
	struct bt_mesh_sg sg[3];
	u8_t salt[16];
	u8_t out[16];
	u8_t t[16];
	u8_t pad;
	int err;

	BT_DBG("n %s", bt_hex(n, 16));
	BT_DBG("p %s", bt_hex(p, p_len));

	err = bt_mesh_s1("smk2", salt);
	if (err) {
		return err;
	}

	err = bt_mesh_aes_cmac_one(salt, n, 16, t);
	if (err) {
		return err;
	}

	pad = 0x01;

	sg[0].data = NULL;
	sg[0].len  = 0;
	sg[1].data = p;
	sg[1].len  = p_len;
	sg[2].data = &pad;
	sg[2].len  = sizeof(pad);

	err = bt_mesh_aes_cmac(t, sg, ARRAY_SIZE(sg), out);
	if (err) {
		return err;
	}

	net_id[0] = out[15] & 0x7f;

	sg[0].data = out;
	sg[0].len  = sizeof(out);
	pad = 0x02;

	err = bt_mesh_aes_cmac(t, sg, ARRAY_SIZE(sg), out);
	if (err) {
		return err;
	}

	memcpy(enc_key, out, 16);

	pad = 0x03;

	err = bt_mesh_aes_cmac(t, sg, ARRAY_SIZE(sg), out);
	if (err) {
		return err;
	}

	memcpy(priv_key, out, 16);

	BT_DBG("NID 0x%02x enc_key %s", net_id[0], bt_hex(enc_key, 16));
	BT_DBG("priv_key %s", bt_hex(priv_key, 16));

	return 0;
}

int bt_mesh_k3(const u8_t n[16], u8_t out[8])
{
	u8_t id64[] = { 'i', 'd', '6', '4', 0x01 };
	u8_t tmp[16];
	u8_t t[16];
	int err;

	err = bt_mesh_s1("smk3", tmp);
	if (err) {
		return err;
	}

	err = bt_mesh_aes_cmac_one(tmp, n, 16, t);
	if (err) {
		return err;
	}

	err = bt_mesh_aes_cmac_one(t, id64, sizeof(id64), tmp);
	if (err) {
		return err;
	}

	memcpy(out, tmp + 8, 8);

	return 0;
}

int bt_mesh_k4(const u8_t n[16], u8_t out[1])
{
	u8_t id6[] = { 'i', 'd', '6', 0x01 };
	u8_t tmp[16];
	u8_t t[16];
	int err;

	err = bt_mesh_s1("smk4", tmp);
	if (err) {
		return err;
	}

	err = bt_mesh_aes_cmac_one(tmp, n, 16, t);
	if (err) {
		return err;
	}

	err = bt_mesh_aes_cmac_one(t, id6, sizeof(id6), tmp);
	if (err) {
		return err;
	}

	out[0] = tmp[15] & BIT_MASK(6);

	return 0;
}

int bt_mesh_id128(const u8_t n[16], const char *s, u8_t out[16])
{
	const char *id128 = "id128\x01";
	u8_t salt[16];
	int err;

	err = bt_mesh_s1(s, salt);
	if (err) {
		return err;
	}

	return bt_mesh_k1(n, 16, salt, id128, out);
}

static int bt_mesh_ccm_decrypt(const u8_t key[16], u8_t nonce[13],
			       const u8_t *enc_msg, size_t msg_len,
			       const u8_t *aad, size_t aad_len,
			       u8_t *out_msg, size_t mic_size)
{
	u8_t msg[16], pmsg[16], cmic[16], cmsg[16], Xn[16], mic[16];
	u16_t last_blk, blk_cnt;
	size_t i, j;
	int err;

	if (msg_len < 1 || aad_len >= 0xff00) {
		return -EINVAL;
	}

	/* C_mic = e(AppKey, 0x01 || nonce || 0x0000) */
	pmsg[0] = 0x01;
	memcpy(pmsg + 1, nonce, 13);
	sys_put_be16(0x0000, pmsg + 14);

	err = bt_encrypt_be(key, pmsg, cmic);
	if (err) {
		return err;
	}

	/* X_0 = e(AppKey, 0x09 || nonce || length) */
	if (mic_size == sizeof(u64_t)) {
		pmsg[0] = 0x19 | (aad_len ? 0x40 : 0x00);
	} else {
		pmsg[0] = 0x09 | (aad_len ? 0x40 : 0x00);
	}

	memcpy(pmsg + 1, nonce, 13);
	sys_put_be16(msg_len, pmsg + 14);

	err = bt_encrypt_be(key, pmsg, Xn);
	if (err) {
		return err;
	}

	/* If AAD is being used to authenticate, include it here */
	if (aad_len) {
		sys_put_be16(aad_len, pmsg);

		for (i = 0; i < sizeof(u16_t); i++) {
			pmsg[i] = Xn[i] ^ pmsg[i];
		}

		j = 0;
		aad_len += sizeof(u16_t);
		while (aad_len > 16) {
			do {
				pmsg[i] = Xn[i] ^ aad[j];
				i++, j++;
			} while (i < 16);

			aad_len -= 16;
			i = 0;

			err = bt_encrypt_be(key, pmsg, Xn);
			if (err) {
				return err;
			}
		}

		for (; i < aad_len; i++, j++) {
			pmsg[i] = Xn[i] ^ aad[j];
		}

		for (i = aad_len; i < 16; i++) {
			pmsg[i] = Xn[i];
		}

		err = bt_encrypt_be(key, pmsg, Xn);
		if (err) {
			return err;
		}
	}

	last_blk = msg_len % 16;
	blk_cnt = (msg_len + 15) / 16;
	if (!last_blk) {
		last_blk = 16;
	}

	for (j = 0; j < blk_cnt; j++) {
		if (j + 1 == blk_cnt) {
			/* C_1 = e(AppKey, 0x01 || nonce || 0x0001) */
			pmsg[0] = 0x01;
			memcpy(pmsg + 1, nonce, 13);
			sys_put_be16(j + 1, pmsg + 14);

			err = bt_encrypt_be(key, pmsg, cmsg);
			if (err) {
				return err;
			}

			/* Encrypted = Payload[0-15] ^ C_1 */
			for (i = 0; i < last_blk; i++) {
				msg[i] = enc_msg[(j * 16) + i] ^ cmsg[i];
			}

			memcpy(out_msg + (j * 16), msg, last_blk);

			/* X_1 = e(AppKey, X_0 ^ Payload[0-15]) */
			for (i = 0; i < last_blk; i++) {
				pmsg[i] = Xn[i] ^ msg[i];
			}

			for (i = last_blk; i < 16; i++) {
				pmsg[i] = Xn[i] ^ 0x00;
			}

			err = bt_encrypt_be(key, pmsg, Xn);
			if (err) {
				return err;
			}

			/* MIC = C_mic ^ X_1 */
			for (i = 0; i < sizeof(mic); i++) {
				mic[i] = cmic[i] ^ Xn[i];
			}
		} else {
			/* C_1 = e(AppKey, 0x01 || nonce || 0x0001) */
			pmsg[0] = 0x01;
			memcpy(pmsg + 1, nonce, 13);
			sys_put_be16(j + 1, pmsg + 14);

			err = bt_encrypt_be(key, pmsg, cmsg);
			if (err) {
				return err;
			}

			/* Encrypted = Payload[0-15] ^ C_1 */
			for (i = 0; i < 16; i++) {
				msg[i] = enc_msg[(j * 16) + i] ^ cmsg[i];
			}

			memcpy(out_msg + (j * 16), msg, 16);

			/* X_1 = e(AppKey, X_0 ^ Payload[0-15]) */
			for (i = 0; i < 16; i++) {
				pmsg[i] = Xn[i] ^ msg[i];
			}

			err = bt_encrypt_be(key, pmsg, Xn);
			if (err) {
				return err;
			}
		}
	}

	if (memcmp(mic, enc_msg + msg_len, mic_size)) {
		return -EBADMSG;
	}

	return 0;
}

static int bt_mesh_ccm_encrypt(const u8_t key[16], u8_t nonce[13],
			       const u8_t *msg, size_t msg_len,
			       const u8_t *aad, size_t aad_len,
			       u8_t *out_msg, size_t mic_size)
{
	u8_t pmsg[16], cmic[16], cmsg[16], mic[16], Xn[16];
	u16_t blk_cnt, last_blk;
	size_t i, j;
	int err;

	BT_DBG("key %s", bt_hex(key, 16));
	BT_DBG("nonce %s", bt_hex(nonce, 13));
	BT_DBG("msg (len %zu) %s", msg_len, bt_hex(msg, msg_len));
	BT_DBG("aad_len %zu mic_size %zu", aad_len, mic_size);

	/* Unsupported AAD size */
	if (aad_len >= 0xff00) {
		return -EINVAL;
	}

	/* C_mic = e(AppKey, 0x01 || nonce || 0x0000) */
	pmsg[0] = 0x01;
	memcpy(pmsg + 1, nonce, 13);
	sys_put_be16(0x0000, pmsg + 14);

	err = bt_encrypt_be(key, pmsg, cmic);
	if (err) {
		return err;
	}

	/* X_0 = e(AppKey, 0x09 || nonce || length) */
	if (mic_size == sizeof(u64_t)) {
		pmsg[0] = 0x19 | (aad_len ? 0x40 : 0x00);
	} else {
		pmsg[0] = 0x09 | (aad_len ? 0x40 : 0x00);
	}

	memcpy(pmsg + 1, nonce, 13);
	sys_put_be16(msg_len, pmsg + 14);

	err = bt_encrypt_be(key, pmsg, Xn);
	if (err) {
		return err;
	}

	/* If AAD is being used to authenticate, include it here */
	if (aad_len) {
		sys_put_be16(aad_len, pmsg);

		for (i = 0; i < sizeof(u16_t); i++) {
			pmsg[i] = Xn[i] ^ pmsg[i];
		}

		j = 0;
		aad_len += sizeof(u16_t);
		while (aad_len > 16) {
			do {
				pmsg[i] = Xn[i] ^ aad[j];
				i++, j++;
			} while (i < 16);

			aad_len -= 16;
			i = 0;

			err = bt_encrypt_be(key, pmsg, Xn);
			if (err) {
				return err;
			}
		}

		for (; i < aad_len; i++, j++) {
			pmsg[i] = Xn[i] ^ aad[j];
		}

		for (i = aad_len; i < 16; i++) {
			pmsg[i] = Xn[i];
		}

		err = bt_encrypt_be(key, pmsg, Xn);
		if (err) {
			return err;
		}
	}

	last_blk = msg_len % 16;
	blk_cnt = (msg_len + 15) / 16;
	if (!last_blk) {
		last_blk = 16;
	}

	for (j = 0; j < blk_cnt; j++) {
		if (j + 1 == blk_cnt) {
			/* X_1 = e(AppKey, X_0 ^ Payload[0-15]) */
			for (i = 0; i < last_blk; i++) {
				pmsg[i] = Xn[i] ^ msg[(j * 16) + i];
			}
			for (i = last_blk; i < 16; i++) {
				pmsg[i] = Xn[i] ^ 0x00;
			}

			err = bt_encrypt_be(key, pmsg, Xn);
			if (err) {
				return err;
			}

			/* MIC = C_mic ^ X_1 */
			for (i = 0; i < sizeof(mic); i++) {
				mic[i] = cmic[i] ^ Xn[i];
			}

			/* C_1 = e(AppKey, 0x01 || nonce || 0x0001) */
			pmsg[0] = 0x01;
			memcpy(pmsg + 1, nonce, 13);
			sys_put_be16(j + 1, pmsg + 14);

			err = bt_encrypt_be(key, pmsg, cmsg);
			if (err) {
				return err;
			}

			/* Encrypted = Payload[0-15] ^ C_1 */
			for (i = 0; i < last_blk; i++) {
				out_msg[(j * 16) + i] =
					msg[(j * 16) + i] ^ cmsg[i];
			}
		} else {
			/* X_1 = e(AppKey, X_0 ^ Payload[0-15]) */
			for (i = 0; i < 16; i++) {
				pmsg[i] = Xn[i] ^ msg[(j * 16) + i];
			}

			err = bt_encrypt_be(key, pmsg, Xn);
			if (err) {
				return err;
			}

			/* C_1 = e(AppKey, 0x01 || nonce || 0x0001) */
			pmsg[0] = 0x01;
			memcpy(pmsg + 1, nonce, 13);
			sys_put_be16(j + 1, pmsg + 14);

			err = bt_encrypt_be(key, pmsg, cmsg);
			if (err) {
				return err;
			}

			/* Encrypted = Payload[0-15] ^ C_N */
			for (i = 0; i < 16; i++) {
				out_msg[(j * 16) + i] =
					msg[(j * 16) + i] ^ cmsg[i];
			}

		}
	}

	memcpy(out_msg + msg_len, mic, mic_size);

	return 0;
}

static void create_proxy_nonce(u8_t nonce[13], const u8_t *pdu,
			       u32_t iv_index)
{
	/* Nonce Type */
	nonce[0] = 0x03;

	/* Pad */
	nonce[1] = 0x00;

	/* Sequence Number */
	nonce[2] = pdu[2];
	nonce[3] = pdu[3];
	nonce[4] = pdu[4];

	/* Source Address */
	nonce[5] = pdu[5];
	nonce[6] = pdu[6];

	/* Pad */
	nonce[7] = 0;
	nonce[8] = 0;

	/* IV Index */
	sys_put_be32(iv_index, &nonce[9]);
}

static void create_net_nonce(u8_t nonce[13], const u8_t *pdu,
			     u32_t iv_index)
{
	/* Nonce Type */
	nonce[0] = 0x00;

	/* FRND + TTL */
	nonce[1] = pdu[1];

	/* Sequence Number */
	nonce[2] = pdu[2];
	nonce[3] = pdu[3];
	nonce[4] = pdu[4];

	/* Source Address */
	nonce[5] = pdu[5];
	nonce[6] = pdu[6];

	/* Pad */
	nonce[7] = 0;
	nonce[8] = 0;

	/* IV Index */
	sys_put_be32(iv_index, &nonce[9]);
}

int bt_mesh_net_obfuscate(u8_t *pdu, u32_t iv_index,
			  const u8_t privacy_key[16])
{
	u8_t priv_rand[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, };
	u8_t tmp[16];
	int err, i;

	BT_DBG("IVIndex %u, PrivacyKey %s", (unsigned) iv_index,
	       bt_hex(privacy_key, 16));

	sys_put_be32(iv_index, &priv_rand[5]);
	memcpy(&priv_rand[9], &pdu[7], 7);

	BT_DBG("PrivacyRandom %s", bt_hex(priv_rand, 16));

	err = bt_encrypt_be(privacy_key, priv_rand, tmp);
	if (err) {
		return err;
	}

	for (i = 0; i < 6; i++) {
		pdu[1 + i] ^= tmp[i];
	}

	return 0;
}

int bt_mesh_net_encrypt(const u8_t key[16], struct os_mbuf *buf,
			u32_t iv_index, bool proxy)
{
	u8_t mic_len = NET_MIC_LEN(buf->om_data);
	u8_t nonce[13];
	int err;

	BT_DBG("IVIndex %u EncKey %s mic_len %u", (unsigned) iv_index,
	       bt_hex(key, 16), mic_len);
	BT_DBG("PDU (len %u) %s", buf->om_len, bt_hex(buf->om_data, buf->om_len));

	if (IS_ENABLED(CONFIG_BT_MESH_PROXY) && proxy) {
		create_proxy_nonce(nonce, buf->om_data, iv_index);
	} else {
		create_net_nonce(nonce, buf->om_data, iv_index);
	}

	BT_DBG("Nonce %s", bt_hex(nonce, 13));

	err = bt_mesh_ccm_encrypt(key, nonce, &buf->om_data[7], buf->om_len - 7,
				  NULL, 0, &buf->om_data[7], mic_len);
	if (!err) {
		net_buf_simple_add(buf, mic_len);
	}

	return err;
}

int bt_mesh_net_decrypt(const u8_t key[16], struct os_mbuf *buf,
			u32_t iv_index, bool proxy)
{
	u8_t mic_len = NET_MIC_LEN(buf->om_data);
	u8_t nonce[13];

	BT_DBG("PDU (%u bytes) %s", buf->om_len, bt_hex(buf->om_data, buf->om_len));
	BT_DBG("iv_index %u, key %s mic_len %u", (unsigned) iv_index,
	       bt_hex(key, 16), mic_len);

	if (IS_ENABLED(CONFIG_BT_MESH_PROXY) && proxy) {
		create_proxy_nonce(nonce, buf->om_data, iv_index);
	} else {
		create_net_nonce(nonce, buf->om_data, iv_index);
	}

	BT_DBG("Nonce %s", bt_hex(nonce, 13));

	buf->om_len -= mic_len;

	return bt_mesh_ccm_decrypt(key, nonce, &buf->om_data[7], buf->om_len - 7,
				   NULL, 0, &buf->om_data[7], mic_len);
}

static void create_app_nonce(u8_t nonce[13], bool dev_key, u8_t aszmic,
			     u16_t src, u16_t dst, u32_t seq_num,
			     u32_t iv_index)
{
	if (dev_key) {
		nonce[0] = 0x02;
	} else {
		nonce[0] = 0x01;
	}

	sys_put_be32((seq_num | ((u32_t)aszmic << 31)), &nonce[1]);

	sys_put_be16(src, &nonce[5]);
	sys_put_be16(dst, &nonce[7]);

	sys_put_be32(iv_index, &nonce[9]);
}

static int mesh_app_encrypt(const u8_t key[16], bool dev_key, u8_t aszmic,
			    struct os_mbuf *buf, const u8_t *ad,
			    u16_t src, u16_t dst, u32_t seq_num, u32_t iv_index)
{
	u8_t nonce[13];

	BT_DBG("AppKey %s", bt_hex(key, 16));
	BT_DBG("dev_key %u src 0x%04x dst 0x%04x", dev_key, src, dst);
	BT_DBG("seq_num 0x%08x iv_index 0x%08x", (unsigned) seq_num,
	       (unsigned) iv_index);
	BT_DBG("Clear: %s", bt_hex(buf->om_data, buf->om_len));

	create_app_nonce(nonce, dev_key, aszmic, src, dst, seq_num, iv_index);

	BT_DBG("Nonce  %s", bt_hex(nonce, 13));

	return bt_mesh_ccm_encrypt(key, nonce, buf->om_data, buf->om_len, ad,
				   ad ? 16 : 0, buf->om_data,
				   APP_MIC_LEN(aszmic));
}

int bt_mesh_app_encrypt_in_place(const u8_t key[16], bool dev_key, u8_t aszmic,
				 struct os_mbuf *buf, const u8_t *ad, u16_t src,
				 u16_t dst, u32_t seq_num, u32_t iv_index)
{
	int err;

	err = mesh_app_encrypt(key, dev_key, aszmic, buf, ad, src, dst,
			       seq_num, iv_index);
	if (!err) {
		BT_DBG("Encr: %s", bt_hex(buf->om_data, buf->om_len));
	}

	return err;
}

int bt_mesh_app_encrypt(const u8_t key[16], bool dev_key, u8_t aszmic,
			struct os_mbuf *buf, const u8_t *ad,
			u16_t src, u16_t dst, u32_t seq_num, u32_t iv_index)
{
	int err;

	err = mesh_app_encrypt(key, dev_key, aszmic, buf, ad, src, dst,
			       seq_num, iv_index);

	if (!err) {
		net_buf_simple_add(buf, APP_MIC_LEN(aszmic));
		BT_DBG("Encr: %s", bt_hex(buf->om_data, buf->om_len));
	}

	return err;
}

static int mesh_app_decrypt(const u8_t key[16], bool dev_key, u8_t aszmic,
			    struct os_mbuf *buf, struct os_mbuf *out,
			    const u8_t *ad, u16_t src, u16_t dst,
			    u32_t seq_num, u32_t iv_index)
{
	u8_t nonce[13];

	BT_DBG("EncData (len %u) %s", buf->om_len,
	       bt_hex(buf->om_data, buf->om_len));

	create_app_nonce(nonce, dev_key, aszmic, src, dst, seq_num, iv_index);

	BT_DBG("AppKey %s", bt_hex(key, 16));
	BT_DBG("Nonce  %s", bt_hex(nonce, 13));

	return bt_mesh_ccm_decrypt(key, nonce, buf->om_data, buf->om_len, ad,
				   ad ? 16 : 0, out->om_data,
				   APP_MIC_LEN(aszmic));
}

int bt_mesh_app_decrypt_in_place(const u8_t key[16], bool dev_key, u8_t aszmic,
				 struct os_mbuf *buf, const u8_t *ad, u16_t src,
				 u16_t dst, u32_t seq_num, u32_t iv_index)
{
	return mesh_app_decrypt(key, dev_key, aszmic, buf, buf,
				ad, src, dst, seq_num, iv_index);
}

int bt_mesh_app_decrypt(const u8_t key[16], bool dev_key, u8_t aszmic,
			struct os_mbuf *buf, struct os_mbuf *out,
			const u8_t *ad, u16_t src, u16_t dst, u32_t seq_num,
			u32_t iv_index)
{
	int err;

	err = mesh_app_decrypt(key, dev_key, aszmic, buf, out,
			       ad, src, dst, seq_num, iv_index);
	if (!err) {
		net_buf_simple_add(out, buf->om_len);
	}

	return err;
}

/* reversed, 8-bit, poly=0x07 */
static const u8_t crc_table[256] = {
	0x00, 0x91, 0xe3, 0x72, 0x07, 0x96, 0xe4, 0x75,
	0x0e, 0x9f, 0xed, 0x7c, 0x09, 0x98, 0xea, 0x7b,
	0x1c, 0x8d, 0xff, 0x6e, 0x1b, 0x8a, 0xf8, 0x69,
	0x12, 0x83, 0xf1, 0x60, 0x15, 0x84, 0xf6, 0x67,

	0x38, 0xa9, 0xdb, 0x4a, 0x3f, 0xae, 0xdc, 0x4d,
	0x36, 0xa7, 0xd5, 0x44, 0x31, 0xa0, 0xd2, 0x43,
	0x24, 0xb5, 0xc7, 0x56, 0x23, 0xb2, 0xc0, 0x51,
	0x2a, 0xbb, 0xc9, 0x58, 0x2d, 0xbc, 0xce, 0x5f,

	0x70, 0xe1, 0x93, 0x02, 0x77, 0xe6, 0x94, 0x05,
	0x7e, 0xef, 0x9d, 0x0c, 0x79, 0xe8, 0x9a, 0x0b,
	0x6c, 0xfd, 0x8f, 0x1e, 0x6b, 0xfa, 0x88, 0x19,
	0x62, 0xf3, 0x81, 0x10, 0x65, 0xf4, 0x86, 0x17,

	0x48, 0xd9, 0xab, 0x3a, 0x4f, 0xde, 0xac, 0x3d,
	0x46, 0xd7, 0xa5, 0x34, 0x41, 0xd0, 0xa2, 0x33,
	0x54, 0xc5, 0xb7, 0x26, 0x53, 0xc2, 0xb0, 0x21,
	0x5a, 0xcb, 0xb9, 0x28, 0x5d, 0xcc, 0xbe, 0x2f,

	0xe0, 0x71, 0x03, 0x92, 0xe7, 0x76, 0x04, 0x95,
	0xee, 0x7f, 0x0d, 0x9c, 0xe9, 0x78, 0x0a, 0x9b,
	0xfc, 0x6d, 0x1f, 0x8e, 0xfb, 0x6a, 0x18, 0x89,
	0xf2, 0x63, 0x11, 0x80, 0xf5, 0x64, 0x16, 0x87,

	0xd8, 0x49, 0x3b, 0xaa, 0xdf, 0x4e, 0x3c, 0xad,
	0xd6, 0x47, 0x35, 0xa4, 0xd1, 0x40, 0x32, 0xa3,
	0xc4, 0x55, 0x27, 0xb6, 0xc3, 0x52, 0x20, 0xb1,
	0xca, 0x5b, 0x29, 0xb8, 0xcd, 0x5c, 0x2e, 0xbf,

	0x90, 0x01, 0x73, 0xe2, 0x97, 0x06, 0x74, 0xe5,
	0x9e, 0x0f, 0x7d, 0xec, 0x99, 0x08, 0x7a, 0xeb,
	0x8c, 0x1d, 0x6f, 0xfe, 0x8b, 0x1a, 0x68, 0xf9,
	0x82, 0x13, 0x61, 0xf0, 0x85, 0x14, 0x66, 0xf7,

	0xa8, 0x39, 0x4b, 0xda, 0xaf, 0x3e, 0x4c, 0xdd,
	0xa6, 0x37, 0x45, 0xd4, 0xa1, 0x30, 0x42, 0xd3,
	0xb4, 0x25, 0x57, 0xc6, 0xb3, 0x22, 0x50, 0xc1,
	0xba, 0x2b, 0x59, 0xc8, 0xbd, 0x2c, 0x5e, 0xcf
};

u8_t bt_mesh_fcs_calc(const u8_t *data, u8_t data_len)
{
	u8_t fcs = 0xff;

	while (data_len--) {
		fcs = crc_table[fcs ^ *data++];
	}

	BT_DBG("fcs 0x%02x", 0xff - fcs);

	return 0xff - fcs;
}

bool bt_mesh_fcs_check(struct os_mbuf *buf, u8_t received_fcs)
{
	const u8_t *data = buf->om_data;
	u16_t data_len = buf->om_len;
	u8_t fcs = 0xff;

	while (data_len--) {
		fcs = crc_table[fcs ^ *data++];
	}

	return crc_table[fcs ^ received_fcs] == 0xcf;
}

int bt_mesh_virtual_addr(const u8_t virtual_label[16], u16_t *addr)
{
	u8_t salt[16];
	u8_t tmp[16];
	int err;

	err = bt_mesh_s1("vtad", salt);
	if (err) {
		return err;
	}

	err = bt_mesh_aes_cmac_one(salt, virtual_label, 16, tmp);
	if (err) {
		return err;
	}

	*addr = (sys_get_be16(&tmp[14]) & 0x3fff) | 0x8000;

	return 0;
}

int bt_mesh_prov_conf_salt(const u8_t conf_inputs[145], u8_t salt[16])
{
	const u8_t conf_salt_key[16] = { 0 };

	return bt_mesh_aes_cmac_one(conf_salt_key, conf_inputs, 145, salt);
}

int bt_mesh_prov_conf_key(const u8_t dhkey[32], const u8_t conf_salt[16],
			  u8_t conf_key[16])
{
	return bt_mesh_k1(dhkey, 32, conf_salt, "prck", conf_key);
}

int bt_mesh_prov_conf(const u8_t conf_key[16], const u8_t rand[16],
		      const u8_t auth[16], u8_t conf[16])
{
	struct bt_mesh_sg sg[] = { { rand, 16 }, { auth, 16 } };

	BT_DBG("ConfirmationKey %s", bt_hex(conf_key, 16));
	BT_DBG("RandomDevice %s", bt_hex(rand, 16));
	BT_DBG("AuthValue %s", bt_hex(auth, 16));

	return bt_mesh_aes_cmac(conf_key, sg, ARRAY_SIZE(sg), conf);
}

int bt_mesh_prov_decrypt(const u8_t key[16], u8_t nonce[13],
			 const u8_t data[25 + 8], u8_t out[25])
{
	return bt_mesh_ccm_decrypt(key, nonce, data, 25, NULL, 0, out, 8);
}

int bt_mesh_prov_encrypt(const u8_t key[16], u8_t nonce[13],
			 const u8_t data[25], u8_t out[25 + 8])
{
	return bt_mesh_ccm_encrypt(key, nonce, data, 25, NULL, 0, out, 8);
}

int bt_mesh_beacon_auth(const u8_t beacon_key[16], u8_t flags,
			const u8_t net_id[8], u32_t iv_index,
			u8_t auth[8])
{
	u8_t msg[13], tmp[16];
	int err;

	BT_DBG("BeaconKey %s", bt_hex(beacon_key, 16));
	BT_DBG("NetId %s", bt_hex(net_id, 8));
	BT_DBG("IV Index 0x%08x", (unsigned) iv_index);

	msg[0] = flags;
	memcpy(&msg[1], net_id, 8);
	sys_put_be32(iv_index, &msg[9]);

	BT_DBG("BeaconMsg %s", bt_hex(msg, sizeof(msg)));

	err = bt_mesh_aes_cmac_one(beacon_key, msg, sizeof(msg), tmp);
	if (!err) {
		memcpy(auth, tmp, 8);
	}

	return err;
}
#endif
