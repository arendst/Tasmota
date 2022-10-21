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

int bt_mesh_aes_cmac(const uint8_t key[16], struct bt_mesh_sg *sg,
		     size_t sg_len, uint8_t mac[16]);

static inline int bt_mesh_aes_cmac_one(const uint8_t key[16], const void *m,
				       size_t len, uint8_t mac[16])
{
	struct bt_mesh_sg sg = { m, len };

	return bt_mesh_aes_cmac(key, &sg, 1, mac);
}

static inline bool bt_mesh_s1(const char *m, uint8_t salt[16])
{
	const uint8_t zero[16] = { 0 };

	return bt_mesh_aes_cmac_one(zero, m, strlen(m), salt);
}

int bt_mesh_k1(const uint8_t *ikm, size_t ikm_len, const uint8_t salt[16],
	       const char *info, uint8_t okm[16]);

#define bt_mesh_k1_str(ikm, ikm_len, salt_str, info, okm) \
({ \
	const uint8_t salt[16] = salt_str; \
	bt_mesh_k1(ikm, ikm_len, salt, info, okm); \
})

int bt_mesh_k2(const uint8_t n[16], const uint8_t *p, size_t p_len,
	       uint8_t net_id[1], uint8_t enc_key[16], uint8_t priv_key[16]);

int bt_mesh_k3(const uint8_t n[16], uint8_t out[8]);

int bt_mesh_k4(const uint8_t n[16], uint8_t out[1]);

int bt_mesh_id128(const uint8_t n[16], const char *s, uint8_t out[16]);

static inline int bt_mesh_id_resolving_key(const uint8_t net_key[16],
					   uint8_t resolving_key[16])
{
	return bt_mesh_k1_str(net_key, 16, "smbt", "smbi", resolving_key);
}

static inline int bt_mesh_identity_key(const uint8_t net_key[16],
				       uint8_t identity_key[16])
{
	return bt_mesh_id128(net_key, "nkik", identity_key);
}

static inline int bt_mesh_beacon_key(const uint8_t net_key[16],
				     uint8_t beacon_key[16])
{
	return bt_mesh_id128(net_key, "nkbk", beacon_key);
}

int bt_mesh_beacon_auth(const uint8_t beacon_key[16], uint8_t flags,
			const uint8_t net_id[8], uint32_t iv_index,
			uint8_t auth[8]);

static inline int bt_mesh_app_id(const uint8_t app_key[16], uint8_t app_id[1])
{
	return bt_mesh_k4(app_key, app_id);
}

static inline int bt_mesh_session_key(const uint8_t dhkey[32],
				      const uint8_t prov_salt[16],
				      uint8_t session_key[16])
{
	return bt_mesh_k1(dhkey, 32, prov_salt, "prsk", session_key);
}

static inline int bt_mesh_prov_nonce(const uint8_t dhkey[32],
				     const uint8_t prov_salt[16],
				     uint8_t nonce[13])
{
	uint8_t tmp[16];
	int err;

	err = bt_mesh_k1(dhkey, 32, prov_salt, "prsn", tmp);
	if (!err) {
		memcpy(nonce, tmp + 3, 13);
	}

	return err;
}

static inline int bt_mesh_dev_key(const uint8_t dhkey[32],
				  const uint8_t prov_salt[16],
				  uint8_t dev_key[16])
{
	return bt_mesh_k1(dhkey, 32, prov_salt, "prdk", dev_key);
}

static inline int bt_mesh_prov_salt(const uint8_t conf_salt[16],
				    const uint8_t prov_rand[16],
				    const uint8_t dev_rand[16],
				    uint8_t prov_salt[16])
{
	const uint8_t prov_salt_key[16] = { 0 };
	struct bt_mesh_sg sg[] = {
		{ conf_salt, 16 },
		{ prov_rand, 16 },
		{ dev_rand, 16 },
	};

	return bt_mesh_aes_cmac(prov_salt_key, sg, ARRAY_SIZE(sg), prov_salt);
}

int bt_mesh_net_obfuscate(uint8_t *pdu, uint32_t iv_index,
			  const uint8_t privacy_key[16]);

int bt_mesh_net_encrypt(const uint8_t key[16], struct os_mbuf *buf,
			uint32_t iv_index, bool proxy);

int bt_mesh_net_decrypt(const uint8_t key[16], struct os_mbuf *buf,
			uint32_t iv_index, bool proxy);

struct bt_mesh_app_crypto_ctx {
	bool dev_key;
	uint8_t aszmic;
	uint16_t src;
	uint16_t dst;
	uint32_t seq_num;
	uint32_t iv_index;
	const uint8_t *ad;
};

int bt_mesh_app_encrypt(const uint8_t key[16],
			const struct bt_mesh_app_crypto_ctx *ctx,
			struct os_mbuf *buf);

int bt_mesh_app_decrypt(const uint8_t key[16],
			const struct bt_mesh_app_crypto_ctx *ctx,
			struct os_mbuf *buf, struct os_mbuf *out);

uint8_t bt_mesh_fcs_calc(const uint8_t *data, uint8_t data_len);

bool bt_mesh_fcs_check(struct os_mbuf *buf, uint8_t received_fcs);

int bt_mesh_virtual_addr(const uint8_t virtual_label[16], uint16_t *addr);

int bt_mesh_prov_conf_salt(const uint8_t conf_inputs[145], uint8_t salt[16]);

int bt_mesh_prov_conf_key(const uint8_t dhkey[32], const uint8_t conf_salt[16],
			  uint8_t conf_key[16]);

int bt_mesh_prov_conf(const uint8_t conf_key[16], const uint8_t rand[16],
		      const uint8_t auth[16], uint8_t conf[16]);

int bt_mesh_prov_decrypt(const uint8_t key[16], uint8_t nonce[13],
			 const uint8_t data[25 + 8], uint8_t out[25]);

int bt_mesh_prov_encrypt(const uint8_t key[16], uint8_t nonce[13],
			 const uint8_t data[25], uint8_t out[25 + 8]);
#endif
