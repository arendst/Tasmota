/*  Bluetooth Mesh */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __CRYPTO_H__
#define __CRYPTO_H__

#include "../include/mesh/mesh.h"

struct bt_mesh_sg {
	const void *data;
	size_t len;
};

int bt_mesh_aes_cmac(const u8_t key[16], struct bt_mesh_sg *sg,
		     size_t sg_len, u8_t mac[16]);

static inline int bt_mesh_aes_cmac_one(const u8_t key[16], const void *m,
				       size_t len, u8_t mac[16])
{
	struct bt_mesh_sg sg = { m, len };

	return bt_mesh_aes_cmac(key, &sg, 1, mac);
}

static inline bool bt_mesh_s1(const char *m, u8_t salt[16])
{
	const u8_t zero[16] = { 0 };

	return bt_mesh_aes_cmac_one(zero, m, strlen(m), salt);
}

int bt_mesh_k1(const u8_t *ikm, size_t ikm_len, const u8_t salt[16],
	       const char *info, u8_t okm[16]);

#define bt_mesh_k1_str(ikm, ikm_len, salt_str, info, okm) \
({ \
	const u8_t salt[16] = salt_str; \
	bt_mesh_k1(ikm, ikm_len, salt, info, okm); \
})

int bt_mesh_k2(const u8_t n[16], const u8_t *p, size_t p_len,
	       u8_t net_id[1], u8_t enc_key[16], u8_t priv_key[16]);

int bt_mesh_k3(const u8_t n[16], u8_t out[8]);

int bt_mesh_k4(const u8_t n[16], u8_t out[1]);

int bt_mesh_id128(const u8_t n[16], const char *s, u8_t out[16]);

static inline int bt_mesh_id_resolving_key(const u8_t net_key[16],
					   u8_t resolving_key[16])
{
	return bt_mesh_k1_str(net_key, 16, "smbt", "smbi", resolving_key);
}

static inline int bt_mesh_identity_key(const u8_t net_key[16],
				       u8_t identity_key[16])
{
	return bt_mesh_id128(net_key, "nkik", identity_key);
}

static inline int bt_mesh_beacon_key(const u8_t net_key[16],
				     u8_t beacon_key[16])
{
	return bt_mesh_id128(net_key, "nkbk", beacon_key);
}

int bt_mesh_beacon_auth(const u8_t beacon_key[16], u8_t flags,
			const u8_t net_id[16], u32_t iv_index,
			u8_t auth[8]);

static inline int bt_mesh_app_id(const u8_t app_key[16], u8_t app_id[1])
{
	return bt_mesh_k4(app_key, app_id);
}

static inline int bt_mesh_session_key(const u8_t dhkey[32],
				      const u8_t prov_salt[16],
				      u8_t session_key[16])
{
	return bt_mesh_k1(dhkey, 32, prov_salt, "prsk", session_key);
}

static inline int bt_mesh_prov_nonce(const u8_t dhkey[32],
				     const u8_t prov_salt[16],
				     u8_t nonce[13])
{
	u8_t tmp[16];
	int err;

	err = bt_mesh_k1(dhkey, 32, prov_salt, "prsn", tmp);
	if (!err) {
		memcpy(nonce, tmp + 3, 13);
	}

	return err;
}

static inline int bt_mesh_dev_key(const u8_t dhkey[32],
				  const u8_t prov_salt[16],
				  u8_t dev_key[16])
{
	return bt_mesh_k1(dhkey, 32, prov_salt, "prdk", dev_key);
}

static inline int bt_mesh_prov_salt(const u8_t conf_salt[16],
				    const u8_t prov_rand[16],
				    const u8_t dev_rand[16],
				    u8_t prov_salt[16])
{
	const u8_t prov_salt_key[16] = { 0 };
	struct bt_mesh_sg sg[] = {
		{ conf_salt, 16 },
		{ prov_rand, 16 },
		{ dev_rand, 16 },
	};

	return bt_mesh_aes_cmac(prov_salt_key, sg, ARRAY_SIZE(sg), prov_salt);
}

int bt_mesh_net_obfuscate(u8_t *pdu, u32_t iv_index,
			  const u8_t privacy_key[16]);

int bt_mesh_net_encrypt(const u8_t key[16], struct os_mbuf *buf,
			u32_t iv_index, bool proxy);

int bt_mesh_net_decrypt(const u8_t key[16], struct os_mbuf *buf,
			u32_t iv_index, bool proxy);

int bt_mesh_app_encrypt_in_place(const u8_t key[16], bool dev_key, u8_t aszmic,
				 struct os_mbuf*buf, const u8_t *ad, u16_t src,
				 u16_t dst, u32_t seq_num, u32_t iv_index);

int bt_mesh_app_encrypt(const u8_t key[16], bool dev_key, u8_t aszmic,
			struct os_mbuf*buf, const u8_t *ad,
			u16_t src, u16_t dst, u32_t seq_num, u32_t iv_index);

int bt_mesh_app_decrypt_in_place(const u8_t key[16], bool dev_key, u8_t aszmic,
				 struct os_mbuf *buf, const u8_t *ad, u16_t src,
				 u16_t dst, u32_t seq_num, u32_t iv_index);

int bt_mesh_app_decrypt(const u8_t key[16], bool dev_key, u8_t aszmic,
			struct os_mbuf*buf, struct os_mbuf*out,
			const u8_t *ad, u16_t src, u16_t dst, u32_t seq_num,
			u32_t iv_index);

u8_t bt_mesh_fcs_calc(const u8_t *data, u8_t data_len);

bool bt_mesh_fcs_check(struct os_mbuf *buf, u8_t received_fcs);

int bt_mesh_virtual_addr(const u8_t virtual_label[16], u16_t *addr);

int bt_mesh_prov_conf_salt(const u8_t conf_inputs[145], u8_t salt[16]);

int bt_mesh_prov_conf_key(const u8_t dhkey[32], const u8_t conf_salt[16],
			  u8_t conf_key[16]);

int bt_mesh_prov_conf(const u8_t conf_key[16], const u8_t rand[16],
		      const u8_t auth[16], u8_t conf[16]);

int bt_mesh_prov_decrypt(const u8_t key[16], u8_t nonce[13],
			 const u8_t data[25 + 8], u8_t out[25]);

int bt_mesh_prov_encrypt(const u8_t key[16], u8_t nonce[13],
			 const u8_t data[25], u8_t out[25 + 8]);
#endif
