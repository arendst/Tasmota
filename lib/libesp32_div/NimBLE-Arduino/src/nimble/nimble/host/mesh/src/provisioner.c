/*  Bluetooth Mesh */

/*
 * Copyright (c) 2017 Intel Corporation
 * Copyright (c) 2020 Lingao Meng
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#define MESH_LOG_MODULE BLE_MESH_PROV_LOG

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

static struct {
	struct bt_mesh_cdb_node *node;
	uint16_t addr;
	uint16_t net_idx;
	uint8_t attention_duration;
	uint8_t uuid[16];
} prov_device;

static void send_pub_key(void);
static void prov_dh_key_gen(void);
static void pub_key_ready(const uint8_t *pkey);

static int reset_state(void)
{
#if MYNEWT_VAL(BLE_MESH_CDB)
	if (prov_device.node != NULL) {
		bt_mesh_cdb_node_del(prov_device.node, false);
	}
#endif
	return bt_mesh_prov_reset_state(pub_key_ready);
}

static void prov_link_close(enum prov_bearer_link_status status)
{
	BT_DBG("%u", status);
	bt_mesh_prov_link.expect = PROV_NO_PDU;

	bt_mesh_prov_link.bearer->link_close(status);
}

static void prov_fail(uint8_t reason)
{
	/* According to Bluetooth Mesh Specification v1.0.1, Section 5.4.4, the
	 * provisioner just closes the link when something fails, while the
	 * provisionee sends the fail message, and waits for the provisioner to
	 * close the link.
	 */
	prov_link_close(PROV_BEARER_LINK_STATUS_FAIL);
}

static void send_invite(void)
{
	struct os_mbuf *inv = PROV_BUF(2);

	BT_DBG("");

	bt_mesh_prov_buf_init(inv, PROV_INVITE);
	net_buf_simple_add_u8(inv, prov_device.attention_duration);

	bt_mesh_prov_link.conf_inputs[0] = prov_device.attention_duration;

	if (bt_mesh_prov_send(inv, NULL)) {
		BT_ERR("Failed to send invite");
		return;
	}

	bt_mesh_prov_link.expect = PROV_CAPABILITIES;
}

static void start_sent(int err, void *cb_data)
{
	if (!bt_pub_key_get()) {
		atomic_set_bit(bt_mesh_prov_link.flags, WAIT_PUB_KEY);
		BT_WARN("Waiting for local public key");
	} else {
		send_pub_key();
	}
}

static void send_start(void)
{
	BT_DBG("");
	uint8_t method, action;
	struct os_mbuf *start = PROV_BUF(6);

	const uint8_t *data = &bt_mesh_prov_link.conf_inputs[1 + 3];

	bt_mesh_prov_buf_init(start, PROV_START);
	net_buf_simple_add_u8(start, PROV_ALG_P256);

	if (atomic_test_bit(bt_mesh_prov_link.flags, REMOTE_PUB_KEY) &&
	    *data == PUB_KEY_OOB) {
		net_buf_simple_add_u8(start, PUB_KEY_OOB);
		atomic_set_bit(bt_mesh_prov_link.flags, OOB_PUB_KEY);
	} else {
		net_buf_simple_add_u8(start, PUB_KEY_NO_OOB);
	}

	if (bt_mesh_prov_link.oob_method == AUTH_METHOD_INPUT) {
		method = AUTH_METHOD_OUTPUT;
		if (bt_mesh_prov_link.oob_action == INPUT_OOB_STRING) {
			action = OUTPUT_OOB_STRING;
		} else {
			action = OUTPUT_OOB_NUMBER;
		}

	} else if (bt_mesh_prov_link.oob_method == AUTH_METHOD_OUTPUT) {
		method = AUTH_METHOD_INPUT;
		if (bt_mesh_prov_link.oob_action == OUTPUT_OOB_STRING) {
			action = INPUT_OOB_STRING;
		} else {
			action = INPUT_OOB_NUMBER;
		}
	} else {
		method = bt_mesh_prov_link.oob_method;
		action = 0x00;
	}

	net_buf_simple_add_u8(start, bt_mesh_prov_link.oob_method);

	net_buf_simple_add_u8(start, bt_mesh_prov_link.oob_action);

	net_buf_simple_add_u8(start, bt_mesh_prov_link.oob_size);

	memcpy(&bt_mesh_prov_link.conf_inputs[12], &start->om_data[1], 5);

	if (bt_mesh_prov_auth(method, action, bt_mesh_prov_link.oob_size) < 0) {
		BT_ERR("Invalid authentication method: 0x%02x; "
		       "action: 0x%02x; size: 0x%02x", method,
		       action, bt_mesh_prov_link.oob_size);
		return;
	}

	if (bt_mesh_prov_send(start, start_sent)) {
		BT_ERR("Failed to send Provisioning Start");
		return;
	}
}

static bool prov_check_method(struct bt_mesh_dev_capabilities *caps)
{
	if (bt_mesh_prov_link.oob_method == AUTH_METHOD_STATIC) {
		if (!caps->static_oob) {
			BT_WARN("Device not support OOB static authentication provisioning");
			return false;
		}
	} else if (bt_mesh_prov_link.oob_method == AUTH_METHOD_INPUT) {
		if (bt_mesh_prov_link.oob_size > caps->input_size) {
			BT_WARN("The required input length (0x%02x) "
				"exceeds the device capacity (0x%02x)",
				bt_mesh_prov_link.oob_size, caps->input_size);
			return false;
		}

		if (!(BIT(bt_mesh_prov_link.oob_action) & caps->input_actions)) {
			BT_WARN("The required input action (0x%02x) "
				"not supported by the device (0x%02x)",
				bt_mesh_prov_link.oob_action, caps->input_actions);
			return false;
		}

		if (bt_mesh_prov_link.oob_action == INPUT_OOB_STRING) {
			if (!bt_mesh_prov->output_string) {
				BT_WARN("Not support output string");
				return false;
			}
		} else {
			if (!bt_mesh_prov->output_number) {
				BT_WARN("Not support output number");
				return false;
			}
		}
	} else if (bt_mesh_prov_link.oob_method == AUTH_METHOD_OUTPUT) {
		if (bt_mesh_prov_link.oob_size > caps->output_size) {
			BT_WARN("The required output length (0x%02x) "
				"exceeds the device capacity (0x%02x)",
				bt_mesh_prov_link.oob_size, caps->output_size);
			return false;
		}

		if (!(BIT(bt_mesh_prov_link.oob_action) & caps->output_actions)) {
			BT_WARN("The required output action (0x%02x) "
				"not supported by the device (0x%02x)",
				bt_mesh_prov_link.oob_action, caps->output_actions);
			return false;
		}

		if (!bt_mesh_prov->input) {
			BT_WARN("Not support input");
			return false;
		}
	}

	return true;
}

static void prov_capabilities(const uint8_t *data)
{
	struct bt_mesh_dev_capabilities caps;

	caps.elem_count = data[0];
	BT_DBG("Elements:          %u", caps.elem_count);

	caps.algorithms = sys_get_be16(&data[1]);
	BT_DBG("Algorithms:        %u", caps.algorithms);

	caps.pub_key_type = data[3];
	caps.static_oob = data[4];
	caps.output_size = data[5];
	BT_DBG("Public Key Type:   0x%02x", caps.pub_key_type);
	BT_DBG("Static OOB Type:   0x%02x", caps.static_oob);
	BT_DBG("Output OOB Size:   %u", caps.output_size);

	caps.output_actions = (bt_mesh_output_action_t)data[6];
	caps.input_size = data[8];
	caps.input_actions = (bt_mesh_input_action_t)data[9];
	BT_DBG("Output OOB Action: 0x%04x", caps.output_actions);
	BT_DBG("Input OOB Size:    %u", caps.input_size);
	BT_DBG("Input OOB Action:  0x%04x", caps.input_actions);

	if (data[0] == 0) {
		BT_ERR("Invalid number of elements");
		prov_fail(PROV_ERR_NVAL_FMT);
		return;
	}
#if MYNEWT_VAL(BLE_MESH_CDB)
	prov_device.node =
		bt_mesh_cdb_node_alloc(prov_device.uuid,
				       prov_device.addr, data[0],
				       prov_device.net_idx);
	if (prov_device.node == NULL) {
		BT_ERR("Failed allocating node 0x%04x", prov_device.addr);
		prov_fail(PROV_ERR_RESOURCES);
		return;
	}
#endif
	memcpy(&bt_mesh_prov_link.conf_inputs[1], data, 11);

	if (bt_mesh_prov->capabilities) {
		bt_mesh_prov->capabilities(&caps);
	}

	if (!prov_check_method(&caps)) {
		prov_fail(PROV_ERR_UNEXP_ERR);
		return;
	}

	send_start();
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

	if (bt_mesh_prov_conf_key(bt_mesh_prov_link.dhkey,
				  bt_mesh_prov_link.conf_salt, bt_mesh_prov_link.conf_key)) {
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

	if (bt_mesh_prov_conf(bt_mesh_prov_link.conf_key,
			      bt_mesh_prov_link.rand, bt_mesh_prov_link.auth,
			      net_buf_simple_add(cfm, 16))) {
		BT_ERR("Unable to generate confirmation value");
		prov_fail(PROV_ERR_UNEXP_ERR);
		return;
	}

	if (bt_mesh_prov_send(cfm, NULL)) {
		BT_ERR("Failed to send Provisioning Confirm");
		return;
	}

	bt_mesh_prov_link.expect = PROV_CONFIRM;
}

static void public_key_sent(int err, void *cb_data)
{
	atomic_set_bit(bt_mesh_prov_link.flags, PUB_KEY_SENT);

	if (atomic_test_bit(bt_mesh_prov_link.flags, OOB_PUB_KEY) &&
	    atomic_test_bit(bt_mesh_prov_link.flags, REMOTE_PUB_KEY)) {
		prov_dh_key_gen();
		return;
	}

	bt_mesh_prov_link.expect = PROV_PUB_KEY;
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

	/* PublicKeyProvisioner */
	memcpy(&bt_mesh_prov_link.conf_inputs[17], &buf->om_databuf[1], 64);

	if (bt_mesh_prov_send(buf, public_key_sent)) {
		BT_ERR("Failed to send Public Key");
		return;
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

	if (atomic_test_bit(bt_mesh_prov_link.flags, WAIT_STRING) ||
	    atomic_test_bit(bt_mesh_prov_link.flags, WAIT_NUMBER) ||
	    atomic_test_bit(bt_mesh_prov_link.flags, NOTIFY_INPUT_COMPLETE)) {
		atomic_set_bit(bt_mesh_prov_link.flags, WAIT_CONFIRM);
		return;
	}

	send_confirm();
}

static void prov_dh_key_gen(void)
{
	uint8_t remote_pk_le[64], *remote_pk;

	remote_pk = &bt_mesh_prov_link.conf_inputs[81];

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

	if (atomic_test_bit(bt_mesh_prov_link.flags, NOTIFY_INPUT_COMPLETE)) {
		bt_mesh_prov_link.expect = PROV_INPUT_COMPLETE;
	}
}

static void prov_pub_key(const uint8_t *data)
{
	BT_DBG("Remote Public Key: %s", bt_hex(data, 64));

	atomic_set_bit(bt_mesh_prov_link.flags, REMOTE_PUB_KEY);

	/* PublicKeyDevice */
	memcpy(&bt_mesh_prov_link.conf_inputs[81], data, 64);
	bt_mesh_prov_link.bearer->clear_tx();

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
		send_pub_key();
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

static void prov_input_complete(const uint8_t *data)
{
	BT_DBG("");

	notify_input_complete();

	if (atomic_test_and_clear_bit(bt_mesh_prov_link.flags, WAIT_CONFIRM)) {
		send_confirm();
	}
}

static void send_prov_data(void)
{
	struct os_mbuf *pdu = PROV_BUF(34);
#if MYNEWT_VAL(BLE_MESH_CDB)
	struct bt_mesh_cdb_subnet *sub;
#endif
	uint8_t session_key[16];
	uint8_t nonce[13];
	int err;

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

	err = bt_mesh_dev_key(bt_mesh_prov_link.dhkey,
			      bt_mesh_prov_link.prov_salt, prov_device.node->dev_key);
	if (err) {
		BT_ERR("Unable to generate device key");
		prov_fail(PROV_ERR_UNEXP_ERR);
		return;
	}

	BT_DBG("DevKey: %s", bt_hex(prov_device.node->dev_key, 16));
#if MYNEWT_VAL(BLE_MESH_CDB)
	sub = bt_mesh_cdb_subnet_get(prov_device.node->net_idx);
	if (sub == NULL) {
		BT_ERR("No subnet with net_idx %u",
		       prov_device.node->net_idx);
		prov_fail(PROV_ERR_UNEXP_ERR);
		return;
	}
#endif
	bt_mesh_prov_buf_init(pdu, PROV_DATA);
#if MYNEWT_VAL(BLE_MESH_CDB)
	net_buf_simple_add_mem(pdu, sub->keys[sub->kr_flag].net_key, 16);
	net_buf_simple_add_be16(pdu, prov_device.node->net_idx);
	net_buf_simple_add_u8(pdu, bt_mesh_cdb_subnet_flags(sub));
	net_buf_simple_add_be32(pdu, bt_mesh_cdb.iv_index);
#endif
	net_buf_simple_add_be16(pdu, prov_device.node->addr);
	net_buf_simple_add(pdu, 8); /* For MIC */

	BT_DBG("net_idx %u, iv_index 0x%08x, addr 0x%04x",
	       prov_device.node->net_idx, bt_mesh.iv_index,
	       prov_device.node->addr);

	err = bt_mesh_prov_encrypt(session_key, nonce, &pdu->om_data[1],
				   &pdu->om_data[1]);
	if (err) {
		BT_ERR("Unable to encrypt provisioning data");
		prov_fail(PROV_ERR_DECRYPT);
		return;
	}

	if (bt_mesh_prov_send(pdu, NULL)) {
		BT_ERR("Failed to send Provisioning Data");
		return;
	}

	bt_mesh_prov_link.expect = PROV_COMPLETE;
}

static void prov_complete(const uint8_t *data)
{
	struct bt_mesh_cdb_node *node = prov_device.node;

	BT_DBG("key %s, net_idx %u, num_elem %u, addr 0x%04x",
	       bt_hex(node->dev_key, 16), node->net_idx, node->num_elem,
	       node->addr);

#if MYNEWT_VAL(BLE_MESH_CDB)
	if (IS_ENABLED(CONFIG_BT_SETTINGS)) {
		bt_mesh_store_cdb_node(node);
	}
#endif

	prov_device.node = NULL;
	prov_link_close(PROV_BEARER_LINK_STATUS_SUCCESS);

	if (bt_mesh_prov->node_added) {
		bt_mesh_prov->node_added(node->net_idx, node->uuid, node->addr,
					 node->num_elem);
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

	bt_mesh_prov_link.expect = PROV_RANDOM;
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

	if (bt_mesh_prov_conf(bt_mesh_prov_link.conf_key,
			      data, bt_mesh_prov_link.auth, conf_verify)) {
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

	if (bt_mesh_prov_salt(bt_mesh_prov_link.conf_salt,
			      bt_mesh_prov_link.rand, data, bt_mesh_prov_link.prov_salt)) {
		BT_ERR("Failed to generate provisioning salt");
		prov_fail(PROV_ERR_UNEXP_ERR);
		return;
	}

	BT_DBG("ProvisioningSalt: %s", bt_hex(bt_mesh_prov_link.prov_salt, 16));

	send_prov_data();
}

static void prov_confirm(const uint8_t *data)
{
	BT_DBG("Remote Confirm: %s", bt_hex(data, 16));

	memcpy(bt_mesh_prov_link.conf, data, 16);

	send_random();
}

static void prov_failed(const uint8_t *data)
{
	BT_WARN("Error: 0x%02x", data[0]);
	reset_state();
}

static void local_input_complete(void)
{
	if (atomic_test_and_clear_bit(bt_mesh_prov_link.flags, WAIT_CONFIRM)) {
		send_confirm();
	}
}

static void prov_link_closed(void)
{
	reset_state();
}

static void prov_link_opened(void)
{
	send_invite();
}

static const struct bt_mesh_prov_role role_provisioner = {
	.input_complete = local_input_complete,
	.link_opened = prov_link_opened,
	.link_closed = prov_link_closed,
	.error = prov_fail,
	.op = {
		[PROV_CAPABILITIES] = prov_capabilities,
		[PROV_PUB_KEY] = prov_pub_key,
		[PROV_INPUT_COMPLETE] = prov_input_complete,
		[PROV_CONFIRM] = prov_confirm,
		[PROV_RANDOM] = prov_random,
		[PROV_COMPLETE] = prov_complete,
		[PROV_FAILED] = prov_failed,
	},
};

static void prov_set_method(uint8_t method, uint8_t action, uint8_t size)
{
	bt_mesh_prov_link.oob_method = method;
	bt_mesh_prov_link.oob_action = action;
	bt_mesh_prov_link.oob_size = size;
}

int bt_mesh_auth_method_set_input(bt_mesh_input_action_t action, uint8_t size)
{
	if (!action || !size || size > 8) {
		return -EINVAL;
	}

	prov_set_method(AUTH_METHOD_INPUT, find_msb_set(action) - 1, size);
	return 0;
}

int bt_mesh_auth_method_set_output(bt_mesh_output_action_t action, uint8_t size)
{
	if (!action || !size || size > 8) {
		return -EINVAL;
	}

	prov_set_method(AUTH_METHOD_OUTPUT, find_msb_set(action) - 1, size);
	return 0;
}

int bt_mesh_auth_method_set_static(const uint8_t *static_val, uint8_t size)
{
	if (!size || !static_val || size > 16) {
		return -EINVAL;
	}

	prov_set_method(AUTH_METHOD_STATIC, 0, 0);

	memcpy(bt_mesh_prov_link.auth + 16 - size, static_val, size);
	if (size < 16) {
		(void)memset(bt_mesh_prov_link.auth, 0,
			     sizeof(bt_mesh_prov_link.auth) - size);
	}
	return 0;
}

int bt_mesh_auth_method_set_none(void)
{
	prov_set_method(AUTH_METHOD_NO_OOB, 0, 0);
	return 0;
}

int bt_mesh_prov_remote_pub_key_set(const uint8_t public_key[64])
{
	if (public_key == NULL) {
		return -EINVAL;
	}

	if (atomic_test_and_set_bit(bt_mesh_prov_link.flags, REMOTE_PUB_KEY)) {
		return -EALREADY;
	}

	/* Swap X and Y halves independently to big-endian */
	memcpy(&bt_mesh_prov_link.conf_inputs[81], public_key, 32);
	memcpy(&bt_mesh_prov_link.conf_inputs[81 + 32], &public_key[32], 32);

	return 0;
}

#if defined(CONFIG_BT_MESH_PB_ADV)
int bt_mesh_pb_adv_open(const uint8_t uuid[16], uint16_t net_idx, uint16_t addr,
			uint8_t attention_duration)
{
	int err;

	if (atomic_test_and_set_bit(bt_mesh_prov_link.flags, LINK_ACTIVE)) {
		return -EBUSY;
	}

	atomic_set_bit(bt_mesh_prov_link.flags, PROVISIONER);
	memcpy(prov_device.uuid, uuid, 16);
	prov_device.addr = addr;
	prov_device.net_idx = net_idx;
	prov_device.attention_duration = attention_duration;
	bt_mesh_prov_link.bearer = &pb_adv;
	bt_mesh_prov_link.role = &role_provisioner;

	err = bt_mesh_prov_link.bearer->link_open(prov_device.uuid, PROTOCOL_TIMEOUT,
						  bt_mesh_prov_bearer_cb_get(), NULL);
	if (err) {
		atomic_clear_bit(bt_mesh_prov_link.flags, LINK_ACTIVE);
	}

	return err;
}
#endif
#endif /* MYNEWT_VAL(BLE_MESH) */
