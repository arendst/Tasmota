/*  Bluetooth Mesh */

/*
 * Copyright (c) 2017 Intel Corporation
 * Copyright (c) 2020 Lingao Meng
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define MESH_LOG_MODULE BLE_MESH_PROV_LOG

#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#if MYNEWT_VAL(BLE_MESH)

#include "crypto.h"
#include "adv.h"
#include "../include/mesh/mesh.h"
#include "net.h"
#include "rpl.h"
#include "beacon.h"
#include "access.h"
#include "foundation.h"
#include "proxy.h"
#include "prov.h"
#include "settings.h"

static void send_pub_key(void);
static void pub_key_ready(const uint8_t *pkey);

static int reset_state(void)
{
	return bt_mesh_prov_reset_state(pub_key_ready);
}

static void prov_send_fail_msg(uint8_t err)
{
	struct os_mbuf *buf = PROV_BUF(2);

	BT_DBG("%u", err);

	bt_mesh_prov_link.expect = PROV_NO_PDU;

	bt_mesh_prov_buf_init(buf, PROV_FAILED);
	net_buf_simple_add_u8(buf, err);

	if (bt_mesh_prov_send(buf, NULL)) {
		BT_ERR("Failed to send Provisioning Failed message");
	}
}

static void prov_fail(uint8_t reason)
{
	/* According to Bluetooth Mesh Specification v1.0.1, Section 5.4.4, the
	 * provisioner just closes the link when something fails, while the
	 * provisionee sends the fail message, and waits for the provisioner to
	 * close the link.
	 */
	prov_send_fail_msg(reason);
}

static void prov_invite(const uint8_t *data)
{
	struct os_mbuf *buf = PROV_BUF(12);

	BT_DBG("Attention Duration: %u seconds", data[0]);

	if (data[0]) {
		bt_mesh_attention(NULL, data[0]);
	}

	bt_mesh_prov_link.conf_inputs[0] = data[0];

	bt_mesh_prov_buf_init(buf, PROV_CAPABILITIES);

	/* Number of Elements supported */
	net_buf_simple_add_u8(buf, bt_mesh_elem_count());

	/* Supported algorithms - FIPS P-256 Eliptic Curve */
	net_buf_simple_add_be16(buf, BIT(PROV_ALG_P256));

	/* Public Key Type, Only "No OOB" Public Key is supported */
	net_buf_simple_add_u8(buf, PUB_KEY_NO_OOB);

	/* Static OOB Type */
	net_buf_simple_add_u8(buf, bt_mesh_prov->static_val ? BIT(0) : 0x00);

	/* Output OOB Size */
	net_buf_simple_add_u8(buf, bt_mesh_prov->output_size);

	/* Output OOB Action */
	net_buf_simple_add_be16(buf, bt_mesh_prov->output_actions);

	/* Input OOB Size */
	net_buf_simple_add_u8(buf, bt_mesh_prov->input_size);

	/* Input OOB Action */
	net_buf_simple_add_be16(buf, bt_mesh_prov->input_actions);

	memcpy(&bt_mesh_prov_link.conf_inputs[1], &buf->om_data[1], 11);

	if (bt_mesh_prov_send(buf, NULL)) {
		BT_ERR("Failed to send capabilities");
		return;
	}

	bt_mesh_prov_link.expect = PROV_START;
}

static void prov_start(const uint8_t *data)
{
	BT_DBG("Algorithm:   0x%02x", data[0]);
	BT_DBG("Public Key:  0x%02x", data[1]);
	BT_DBG("Auth Method: 0x%02x", data[2]);
	BT_DBG("Auth Action: 0x%02x", data[3]);
	BT_DBG("Auth Size:   0x%02x", data[4]);

	if (data[0] != PROV_ALG_P256) {
		BT_ERR("Unknown algorithm 0x%02x", data[0]);
		prov_fail(PROV_ERR_NVAL_FMT);
		return;
	}

	if (data[1] != PUB_KEY_NO_OOB) {
		BT_ERR("Invalid public key type: 0x%02x", data[1]);
		prov_fail(PROV_ERR_NVAL_FMT);
		return;
	}

	memcpy(&bt_mesh_prov_link.conf_inputs[12], data, 5);

	bt_mesh_prov_link.expect = PROV_PUB_KEY;

	if (bt_mesh_prov_auth(data[2], data[3], data[4]) < 0) {
		BT_ERR("Invalid authentication method: 0x%02x; "
		       "action: 0x%02x; size: 0x%02x", data[2], data[3],
		       data[4]);
		prov_fail(PROV_ERR_NVAL_FMT);
	}

	if (atomic_test_bit(bt_mesh_prov_link.flags, OOB_STATIC_KEY)) {
		memcpy(bt_mesh_prov_link.auth + 16 - bt_mesh_prov->static_val_len,
		       bt_mesh_prov->static_val, bt_mesh_prov->static_val_len);
		(void)memset(bt_mesh_prov_link.auth, 0,
			     sizeof(bt_mesh_prov_link.auth) - bt_mesh_prov->static_val_len);
	}
}

static void send_confirm(void)
{
	struct os_mbuf *cfm = PROV_BUF(17);

	BT_DBG("ConfInputs[0]   %s", bt_hex(bt_mesh_prov_link.conf_inputs, 64));
	BT_DBG("ConfInputs[64]  %s", bt_hex(&bt_mesh_prov_link.conf_inputs[64], 64));
	BT_DBG("ConfInputs[128] %s", bt_hex(&bt_mesh_prov_link.conf_inputs[128], 17));

	if (bt_mesh_prov_conf_salt(bt_mesh_prov_link.conf_inputs,
				   bt_mesh_prov_link.conf_salt)) {
		BT_ERR("Unable to generate confirmation salt");
		prov_fail(PROV_ERR_UNEXP_ERR);
		return;
	}

	BT_DBG("ConfirmationSalt: %s", bt_hex(bt_mesh_prov_link.conf_salt, 16));

	if (bt_mesh_prov_conf_key(bt_mesh_prov_link.dhkey, bt_mesh_prov_link.conf_salt,
				  bt_mesh_prov_link.conf_key)) {
		BT_ERR("Unable to generate confirmation key");
		prov_fail(PROV_ERR_UNEXP_ERR);
		return;
	}

	BT_DBG("ConfirmationKey: %s", bt_hex(bt_mesh_prov_link.conf_key, 16));

	if (bt_rand(bt_mesh_prov_link.rand, 16)) {
		BT_ERR("Unable to generate random number");
		prov_fail(PROV_ERR_UNEXP_ERR);
		return;
	}

	BT_DBG("LocalRandom: %s", bt_hex(bt_mesh_prov_link.rand, 16));

	bt_mesh_prov_buf_init(cfm, PROV_CONFIRM);

	if (bt_mesh_prov_conf(bt_mesh_prov_link.conf_key, bt_mesh_prov_link.rand,
			      bt_mesh_prov_link.auth, net_buf_simple_add(cfm, 16))) {
		BT_ERR("Unable to generate confirmation value");
		prov_fail(PROV_ERR_UNEXP_ERR);
		return;
	}

	if (bt_mesh_prov_send(cfm, NULL)) {
		BT_ERR("Failed to send Provisioning Confirm");
		return;
	}

	bt_mesh_prov_link.expect = PROV_RANDOM;

}

static void send_input_complete(void)
{
	struct os_mbuf *buf = PROV_BUF(1);

	bt_mesh_prov_buf_init(buf, PROV_INPUT_COMPLETE);
	if (bt_mesh_prov_send(buf, NULL)) {
		BT_ERR("Failed to send Provisioning Input Complete");
	}
	bt_mesh_prov_link.expect = PROV_CONFIRM;
}

static void public_key_sent(int err, void *cb_data)
{
	atomic_set_bit(bt_mesh_prov_link.flags, PUB_KEY_SENT);

	if (atomic_test_bit(bt_mesh_prov_link.flags, INPUT_COMPLETE)) {
		send_input_complete();
		return;
	}
}

static void send_pub_key(void)
{
	struct os_mbuf *buf = PROV_BUF(65);
	const uint8_t *key;

	key = bt_pub_key_get();
	if (!key) {
		BT_ERR("No public key available");
		prov_fail(PROV_ERR_UNEXP_ERR);
		return;
	}

	BT_DBG("Local Public Key: %s", bt_hex(key, 64));

	bt_mesh_prov_buf_init(buf, PROV_PUB_KEY);

	/* Swap X and Y halves independently to big-endian */
	sys_memcpy_swap(net_buf_simple_add(buf, 32), key, 32);
	sys_memcpy_swap(net_buf_simple_add(buf, 32), &key[32], 32);

	/* PublicKeyRemote */
	memcpy(&bt_mesh_prov_link.conf_inputs[81], &buf->om_data[1], 64);

	if (bt_mesh_prov_send(buf, public_key_sent)) {
		BT_ERR("Failed to send Public Key");
		return;
	}

	if (atomic_test_bit(bt_mesh_prov_link.flags, WAIT_NUMBER) ||
	    atomic_test_bit(bt_mesh_prov_link.flags, WAIT_STRING)) {
		bt_mesh_prov_link.expect = PROV_NO_PDU; /* Wait for input */
	} else {
		bt_mesh_prov_link.expect = PROV_CONFIRM;
	}
}

static void prov_dh_key_cb(const uint8_t dhkey[32])
{
	BT_DBG("%p", dhkey);

	if (!dhkey) {
		BT_ERR("DHKey generation failed");
		prov_fail(PROV_ERR_UNEXP_ERR);
		return;
	}

	sys_memcpy_swap(bt_mesh_prov_link.dhkey, dhkey, 32);

	BT_DBG("DHkey: %s", bt_hex(bt_mesh_prov_link.dhkey, 32));

	send_pub_key();
}

static void prov_dh_key_gen(void)
{
	uint8_t remote_pk_le[64], *remote_pk;

	remote_pk = &bt_mesh_prov_link.conf_inputs[17];

	/* Copy remote key in little-endian for bt_dh_key_gen().
	 * X and Y halves are swapped independently. The bt_dh_key_gen()
	 * will also take care of validating the remote public key.
	 */
	sys_memcpy_swap(remote_pk_le, remote_pk, 32);
	sys_memcpy_swap(&remote_pk_le[32], &remote_pk[32], 32);

	if (bt_dh_key_gen(remote_pk_le, prov_dh_key_cb)) {
		BT_ERR("Failed to generate DHKey");
		prov_fail(PROV_ERR_UNEXP_ERR);
	}
}

static void prov_pub_key(const uint8_t *data)
{
	BT_DBG("Remote Public Key: %s", bt_hex(data, 64));

	/* PublicKeyProvisioner */
	memcpy(&bt_mesh_prov_link.conf_inputs[17], data, 64);

	if (!bt_pub_key_get()) {
		/* Clear retransmit timer */
		bt_mesh_prov_link.bearer->clear_tx();
		atomic_set_bit(bt_mesh_prov_link.flags, WAIT_PUB_KEY);
		BT_WARN("Waiting for local public key");
		return;
	}

	prov_dh_key_gen();
}

static void pub_key_ready(const uint8_t *pkey)
{
	if (!pkey) {
		BT_WARN("Public key not available");
		return;
	}

	BT_DBG("Local public key ready");

	if (atomic_test_and_clear_bit(bt_mesh_prov_link.flags, WAIT_PUB_KEY)) {
		prov_dh_key_gen();
	}
}

static void notify_input_complete(void)
{
	if (atomic_test_and_clear_bit(bt_mesh_prov_link.flags,
				      NOTIFY_INPUT_COMPLETE) &&
	    bt_mesh_prov->input_complete) {
		bt_mesh_prov->input_complete();
	}
}

static void send_random(void)
{
	struct os_mbuf *rnd = PROV_BUF(17);

	bt_mesh_prov_buf_init(rnd, PROV_RANDOM);
	net_buf_simple_add_mem(rnd, bt_mesh_prov_link.rand, 16);

	if (bt_mesh_prov_send(rnd, NULL)) {
		BT_ERR("Failed to send Provisioning Random");
		return;
	}

	bt_mesh_prov_link.expect = PROV_DATA;
}

static void prov_random(const uint8_t *data)
{
	uint8_t conf_verify[16];

	BT_DBG("Remote Random: %s", bt_hex(data, 16));
	if (!memcmp(data, bt_mesh_prov_link.rand, 16)) {
		BT_ERR("Random value is identical to ours, rejecting.");
		prov_fail(PROV_ERR_CFM_FAILED);
		return;
	}

	if (bt_mesh_prov_conf(bt_mesh_prov_link.conf_key, data,
			      bt_mesh_prov_link.auth, conf_verify)) {
		BT_ERR("Unable to calculate confirmation verification");
		prov_fail(PROV_ERR_UNEXP_ERR);
		return;
	}

	if (memcmp(conf_verify, bt_mesh_prov_link.conf, 16)) {
		BT_ERR("Invalid confirmation value");
		BT_DBG("Received:   %s", bt_hex(bt_mesh_prov_link.conf, 16));
		BT_DBG("Calculated: %s",  bt_hex(conf_verify, 16));
		prov_fail(PROV_ERR_CFM_FAILED);
		return;
	}

	if (bt_mesh_prov_salt(bt_mesh_prov_link.conf_salt, data,
			      bt_mesh_prov_link.rand, bt_mesh_prov_link.prov_salt)) {
		BT_ERR("Failed to generate provisioning salt");
		prov_fail(PROV_ERR_UNEXP_ERR);
		return;
	}

	BT_DBG("ProvisioningSalt: %s", bt_hex(bt_mesh_prov_link.prov_salt, 16));

	send_random();
}

static void prov_confirm(const uint8_t *data)
{
	BT_DBG("Remote Confirm: %s", bt_hex(data, 16));

	memcpy(bt_mesh_prov_link.conf, data, 16);

	notify_input_complete();

	send_confirm();
}

static inline bool is_pb_gatt(void)
{
	return bt_mesh_prov_link.bearer &&
	       bt_mesh_prov_link.bearer->type == BT_MESH_PROV_GATT;
}

static void prov_data(const uint8_t *data)
{
	struct os_mbuf *msg = PROV_BUF(1);
	uint8_t session_key[16];
	uint8_t nonce[13];
	uint8_t dev_key[16];
	uint8_t pdu[25];
	uint8_t flags;
	uint32_t iv_index;
	uint16_t addr;
	uint16_t net_idx;
	int err;
	bool identity_enable;

	BT_DBG("");

	err = bt_mesh_session_key(bt_mesh_prov_link.dhkey,
				  bt_mesh_prov_link.prov_salt, session_key);
	if (err) {
		BT_ERR("Unable to generate session key");
		prov_fail(PROV_ERR_UNEXP_ERR);
		return;
	}

	BT_DBG("SessionKey: %s", bt_hex(session_key, 16));

	err = bt_mesh_prov_nonce(bt_mesh_prov_link.dhkey,
				 bt_mesh_prov_link.prov_salt, nonce);
	if (err) {
		BT_ERR("Unable to generate session nonce");
		prov_fail(PROV_ERR_UNEXP_ERR);
		return;
	}

	BT_DBG("Nonce: %s", bt_hex(nonce, 13));

	err = bt_mesh_prov_decrypt(session_key, nonce, data, pdu);
	if (err) {
		BT_ERR("Unable to decrypt provisioning data");
		prov_fail(PROV_ERR_DECRYPT);
		return;
	}

	err = bt_mesh_dev_key(bt_mesh_prov_link.dhkey,
			      bt_mesh_prov_link.prov_salt, dev_key);
	if (err) {
		BT_ERR("Unable to generate device key");
		prov_fail(PROV_ERR_UNEXP_ERR);
		return;
	}

	BT_DBG("DevKey: %s", bt_hex(dev_key, 16));

	net_idx = sys_get_be16(&pdu[16]);
	flags = pdu[18];
	iv_index = sys_get_be32(&pdu[19]);
	addr = sys_get_be16(&pdu[23]);

	BT_DBG("net_idx %u iv_index 0x%08x, addr 0x%04x",
	       net_idx, iv_index, addr);

	bt_mesh_prov_buf_init(msg, PROV_COMPLETE);
	if (bt_mesh_prov_send(msg, NULL)) {
		BT_ERR("Failed to send Provisioning Complete");
		return;
	}

	/* Ignore any further PDUs on this link */
	bt_mesh_prov_link.expect = PROV_NO_PDU;

	/* Store info, since bt_mesh_provision() will end up clearing it */
	if (IS_ENABLED(CONFIG_BT_MESH_GATT_PROXY)) {
		identity_enable = is_pb_gatt();
	} else {
		identity_enable = false;
	}

	err = bt_mesh_provision(pdu, net_idx, flags, iv_index, addr, dev_key);
	if (err) {
		BT_ERR("Failed to provision (err %d)", err);
		return;
	}

	/* After PB-GATT provisioning we should start advertising
	 * using Node Identity.
	 */
	if (IS_ENABLED(CONFIG_BT_MESH_GATT_PROXY) && identity_enable) {
		bt_mesh_proxy_identity_enable();
	}
}

static void local_input_complete(void)
{
	if (atomic_test_bit(bt_mesh_prov_link.flags, PUB_KEY_SENT)) {
		send_input_complete();
	} else {
		atomic_set_bit(bt_mesh_prov_link.flags, INPUT_COMPLETE);
	}
}

static void prov_link_closed(void)
{
	reset_state();
}

static void prov_link_opened(void)
{
	bt_mesh_prov_link.expect = PROV_INVITE;
}

static const struct bt_mesh_prov_role role_device = {
	.input_complete = local_input_complete,
	.link_opened = prov_link_opened,
	.link_closed = prov_link_closed,
	.error = prov_fail,
	.op = {
		[PROV_INVITE] = prov_invite,
		[PROV_START] = prov_start,
		[PROV_PUB_KEY] = prov_pub_key,
		[PROV_CONFIRM] = prov_confirm,
		[PROV_RANDOM] = prov_random,
		[PROV_DATA] = prov_data,
	},
};

int bt_mesh_prov_enable(bt_mesh_prov_bearer_t bearers)
{
	BT_DBG("bt_mesh_prov_enable");

	if (bt_mesh_is_provisioned()) {
		return -EALREADY;
	}

	if (IS_ENABLED(CONFIG_BT_MESH_PB_ADV) &&
	    (bearers & BT_MESH_PROV_ADV)) {
		pb_adv.link_accept(bt_mesh_prov_bearer_cb_get(), NULL);
	}

	if (IS_ENABLED(CONFIG_BT_MESH_PB_GATT) &&
	    (bearers & BT_MESH_PROV_GATT)) {
		pb_gatt.link_accept(bt_mesh_prov_bearer_cb_get(), NULL);
	}

	BT_DBG("bt_mesh_prov_link.role = &role_device");
	bt_mesh_prov_link.role = &role_device;

	return 0;
}

int bt_mesh_prov_disable(bt_mesh_prov_bearer_t bearers)
{
	if (bt_mesh_is_provisioned()) {
		return -EALREADY;
	}

	if (IS_ENABLED(CONFIG_BT_MESH_PB_ADV) &&
	    (bearers & BT_MESH_PROV_ADV)) {
		bt_mesh_beacon_disable();
		bt_mesh_scan_disable();
	}

	if (IS_ENABLED(CONFIG_BT_MESH_PB_GATT) &&
	    (bearers & BT_MESH_PROV_GATT)) {
		bt_mesh_proxy_prov_disable(true);
	}

	return 0;
}

#endif /* MYNEWT_VAL(BLE_MESH) */
