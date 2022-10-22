/*  Bluetooth Mesh */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#if MYNEWT_VAL(BLE_MESH)

#define MESH_LOG_MODULE BLE_MESH_PROV_LOG

#include <errno.h>

#include "../include/mesh/mesh.h"
#include "mesh_priv.h"

#include "crypto.h"
#include "atomic.h"
#include "net.h"
#include "access.h"
#include "foundation.h"
#include "prov.h"

struct bt_mesh_prov_link bt_mesh_prov_link;
const struct bt_mesh_prov *bt_mesh_prov;

static void pub_key_ready(const uint8_t *pkey)
{
	if (!pkey) {
		BT_WARN("Public key not available");
		return;
	}
	BT_DBG("Local public key ready");
}

int bt_mesh_prov_reset_state(void (*func)(const uint8_t key[64]))
{
	BT_DBG("bt_mesh_prov_reset_state");

	int err;
	static struct bt_pub_key_cb pub_key_cb;
	const size_t offset = offsetof(struct bt_mesh_prov_link, dhkey);

	pub_key_cb.func = func ? func : pub_key_ready;

	/* Disable Attention Timer if it was set */
	if (bt_mesh_prov_link.conf_inputs[0]) {
		bt_mesh_attention(NULL, 0);
	}

	atomic_clear(bt_mesh_prov_link.flags);
	(void)memset((uint8_t *)&bt_mesh_prov_link + offset, 0,
		     sizeof(bt_mesh_prov_link) - offset);

	err = bt_pub_key_gen(&pub_key_cb);
	if (err) {
		BT_ERR("Failed to generate public key (%d)", err);
		return err;
	}
	return 0;
}

static bt_mesh_output_action_t output_action(uint8_t action)
{
	switch (action) {
	case OUTPUT_OOB_BLINK:
		return BT_MESH_BLINK;
	case OUTPUT_OOB_BEEP:
		return BT_MESH_BEEP;
	case OUTPUT_OOB_VIBRATE:
		return BT_MESH_VIBRATE;
	case OUTPUT_OOB_NUMBER:
		return BT_MESH_DISPLAY_NUMBER;
	case OUTPUT_OOB_STRING:
		return BT_MESH_DISPLAY_STRING;
	default:
		return BT_MESH_NO_OUTPUT;
	}
}

static bt_mesh_input_action_t input_action(uint8_t action)
{
	switch (action) {
	case INPUT_OOB_PUSH:
		return BT_MESH_PUSH;
	case INPUT_OOB_TWIST:
		return BT_MESH_TWIST;
	case INPUT_OOB_NUMBER:
		return BT_MESH_ENTER_NUMBER;
	case INPUT_OOB_STRING:
		return BT_MESH_ENTER_STRING;
	default:
		return BT_MESH_NO_INPUT;
	}
}

int bt_mesh_prov_auth(uint8_t method, uint8_t action, uint8_t size)
{
	bt_mesh_output_action_t output;
	bt_mesh_input_action_t input;

	switch (method) {
	case AUTH_METHOD_NO_OOB:
		if (action || size) {
			return -EINVAL;
		}

		(void)memset(bt_mesh_prov_link.auth, 0, sizeof(bt_mesh_prov_link.auth));
		return 0;
	case AUTH_METHOD_STATIC:
		if (action || size) {
			return -EINVAL;
		}

		atomic_set_bit(bt_mesh_prov_link.flags, OOB_STATIC_KEY);

		return 0;

	case AUTH_METHOD_OUTPUT:
		output = output_action(action);
		if (!output) {
			return -EINVAL;
		}

		if (!(bt_mesh_prov->output_actions & output)) {
			return -EINVAL;
		}

		if (size > bt_mesh_prov->output_size) {
			return -EINVAL;
		}

		atomic_set_bit(bt_mesh_prov_link.flags, NOTIFY_INPUT_COMPLETE);

		if (output == BT_MESH_DISPLAY_STRING) {
			unsigned char str[9];
			uint8_t i;

			bt_rand(str, size);

			/* Normalize to '0' .. '9' & 'A' .. 'Z' */
			for (i = 0; i < size; i++) {
				str[i] %= 36;
				if (str[i] < 10) {
					str[i] += '0';
				} else {
					str[i] += 'A' - 10;
				}
			}
			str[size] = '\0';

			memcpy(bt_mesh_prov_link.auth, str, size);
			memset(bt_mesh_prov_link.auth + size, 0,
				     sizeof(bt_mesh_prov_link.auth) - size);

			return bt_mesh_prov->output_string((char *)str);
		} else {
			uint32_t div[8] = { 10, 100, 1000, 10000, 100000,
					    1000000, 10000000, 100000000 };
			uint32_t num;

			bt_rand(&num, sizeof(num));
			num %= div[size - 1];

			sys_put_be32(num, &bt_mesh_prov_link.auth[12]);
			memset(bt_mesh_prov_link.auth, 0, 12);

			return bt_mesh_prov->output_number(output, num);
		}

	case AUTH_METHOD_INPUT:
		input = input_action(action);
		if (!input) {
			return -EINVAL;
		}

		if (!(bt_mesh_prov->input_actions & input)) {
			return -EINVAL;
		}

		if (size > bt_mesh_prov->input_size) {
			return -EINVAL;
		}

		if (input == BT_MESH_ENTER_STRING) {
			atomic_set_bit(bt_mesh_prov_link.flags, WAIT_STRING);
		} else {
			atomic_set_bit(bt_mesh_prov_link.flags, WAIT_NUMBER);
		}

		return bt_mesh_prov->input(input, size);

	default:
		return -EINVAL;
	}
}

int bt_mesh_input_number(uint32_t num)
{
	BT_DBG("%u", (unsigned) num);

	if (!atomic_test_and_clear_bit(bt_mesh_prov_link.flags, WAIT_NUMBER)) {
		return -EINVAL;
	}

	sys_put_be32(num, &bt_mesh_prov_link.auth[12]);

	bt_mesh_prov_link.role->input_complete();

	return 0;
}

int bt_mesh_input_string(const char *str)
{
	BT_DBG("%s", str);

	if (!atomic_test_and_clear_bit(bt_mesh_prov_link.flags, WAIT_STRING)) {
		return -EINVAL;
	}

	strncpy((char *)bt_mesh_prov_link.auth, str, bt_mesh_prov->input_size);

	bt_mesh_prov_link.role->input_complete();

	return 0;
}

const struct bt_mesh_prov *bt_mesh_prov_get(void)
{
	return bt_mesh_prov;
}

bool bt_mesh_prov_active(void)
{
	return atomic_test_bit(bt_mesh_prov_link.flags, LINK_ACTIVE);
}

static void prov_recv(const struct prov_bearer *bearer, void *cb_data,
		      		  struct os_mbuf *buf)
{
	static const uint8_t op_len[10] = {
		[PROV_INVITE] = 1,
		[PROV_CAPABILITIES] = 11,
		[PROV_START] = 5,
		[PROV_PUB_KEY] = 64,
		[PROV_INPUT_COMPLETE] = 0,
		[PROV_CONFIRM] = 16,
		[PROV_RANDOM] = 16,
		[PROV_DATA] = 33,
		[PROV_COMPLETE] = 0,
		[PROV_FAILED] = 1,
	};

	uint8_t type = buf->om_data[0];
	BT_DBG("type 0x%02x len %u", type, buf->om_len);

	if (type >= ARRAY_SIZE(bt_mesh_prov_link.role->op)) {
		BT_ERR("Unknown provisioning PDU type 0x%02x", type);
		bt_mesh_prov_link.role->error(PROV_ERR_NVAL_FMT);
		return;
	}

	if ((type != PROV_FAILED && type != bt_mesh_prov_link.expect) ||
	    !bt_mesh_prov_link.role->op[type]) {
		BT_WARN("Unexpected msg 0x%02x != 0x%02x", type, bt_mesh_prov_link.expect);
		bt_mesh_prov_link.role->error(PROV_ERR_UNEXP_PDU);
		return;
	}

	if (1 + op_len[type] != buf->om_len) {
		BT_ERR("Invalid length %u for type 0x%02x", buf->om_len, type);
		bt_mesh_prov_link.role->error(PROV_ERR_NVAL_FMT);
		return;
	}

	bt_mesh_prov_link.role->op[type](&buf->om_data[1]);
}

static void prov_link_opened(const struct prov_bearer *bearer, void *cb_data)
{
	atomic_set_bit(bt_mesh_prov_link.flags, LINK_ACTIVE);

	BT_ERR("bt_mesh_prov->link_open");
	if (bt_mesh_prov->link_open) {
		bt_mesh_prov->link_open(bearer->type);
	}

	BT_ERR("bt_mesh_prov_link.bearer");
	bt_mesh_prov_link.bearer = bearer;

	BT_ERR("bt_mesh_prov_link.role->link_opened");
	BT_ERR("%p", bt_mesh_prov_link.role);
	BT_ERR("%p", bt_mesh_prov_link.role->link_opened);
	if (bt_mesh_prov_link.role->link_opened) {
		bt_mesh_prov_link.role->link_opened();
	}
	BT_ERR("done");
}

static void prov_link_closed(const struct prov_bearer *bearer, void *cb_data,
			     enum prov_bearer_link_status reason)
{
	if (bt_mesh_prov_link.role->link_closed) {
		bt_mesh_prov_link.role->link_closed();
	}

	if (bt_mesh_prov->link_close) {
		bt_mesh_prov->link_close(bearer->type);
	}
}

static void prov_bearer_error(const struct prov_bearer *bearer, void *cb_data,
			      uint8_t err)
{
	if (bt_mesh_prov_link.role->error) {
		bt_mesh_prov_link.role->error(err);
	}
}

static const struct prov_bearer_cb prov_bearer_cb = {
	.link_opened = prov_link_opened,
	.link_closed = prov_link_closed,
	.error = prov_bearer_error,
	.recv = prov_recv,
};

const struct prov_bearer_cb *bt_mesh_prov_bearer_cb_get(void)
{
	return &prov_bearer_cb;
}

void bt_mesh_prov_complete(uint16_t net_idx, uint16_t addr)
{
	if (bt_mesh_prov->complete) {
		bt_mesh_prov->complete(net_idx, addr);
	}
}

void bt_mesh_prov_reset(void)
{
	BT_DBG("bt_mesh_prov_reset");

	if (IS_ENABLED(CONFIG_BT_MESH_PB_ADV)) {
		pb_adv_reset();
	}

	if (IS_ENABLED(CONFIG_BT_MESH_PB_GATT)) {
		pb_gatt_reset();
	}

	bt_mesh_prov_reset_state(NULL);

	if (bt_mesh_prov->reset) {
		bt_mesh_prov->reset();
	}
}

int bt_mesh_prov_init(const struct bt_mesh_prov *prov_info)
{
	if (!prov_info) {
		BT_ERR("No provisioning context provided");
		return -EINVAL;
	}

	bt_mesh_prov = prov_info;

	if (IS_ENABLED(CONFIG_BT_MESH_PB_ADV)) {
		pb_adv_init();
	}
	if (IS_ENABLED(CONFIG_BT_MESH_PB_GATT)) {
		pb_gatt_init();
	}

	return bt_mesh_prov_reset_state(NULL);
}

#endif /* MYNEWT_VAL(BLE_MESH) */
