/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stddef.h>

#include "console/console.h"
#include "mesh/testing.h"
#include "mesh/slist.h"
#include "mesh/glue.h"
#include "mesh/access.h"

#include "net.h"
#include "testing.h"
#include "access.h"
#include "foundation.h"
#include "lpn.h"
#include "transport.h"

static sys_slist_t cb_slist;

void bt_test_cb_register(struct bt_test_cb *cb)
{
	sys_slist_append(&cb_slist, &cb->node);
}

void bt_test_cb_unregister(struct bt_test_cb *cb)
{
	sys_slist_find_and_remove(&cb_slist, &cb->node);
}

void bt_test_mesh_net_recv(u8_t ttl, u8_t ctl, u16_t src, u16_t dst,
			   const void *payload, size_t payload_len)
{
	struct bt_test_cb *cb;

	SYS_SLIST_FOR_EACH_CONTAINER(&cb_slist, cb, node) {
		if (cb->mesh_net_recv) {
			cb->mesh_net_recv(ttl, ctl, src, dst, payload,
					  payload_len);
		}
	}
}

void bt_test_mesh_model_bound(u16_t addr, struct bt_mesh_model *model,
			      u16_t key_idx)
{
	struct bt_test_cb *cb;

	SYS_SLIST_FOR_EACH_CONTAINER(&cb_slist, cb, node) {
		if (cb->mesh_model_bound) {
			cb->mesh_model_bound(addr, model, key_idx);
		}
	}
}

void bt_test_mesh_model_unbound(u16_t addr, struct bt_mesh_model *model,
				u16_t key_idx)
{
	struct bt_test_cb *cb;

	SYS_SLIST_FOR_EACH_CONTAINER(&cb_slist, cb, node) {
		if (cb->mesh_model_unbound) {
			cb->mesh_model_unbound(addr, model, key_idx);
		}
	}
}

void bt_test_mesh_prov_invalid_bearer(u8_t opcode)
{
	struct bt_test_cb *cb;

	SYS_SLIST_FOR_EACH_CONTAINER(&cb_slist, cb, node) {
		if (cb->mesh_prov_invalid_bearer) {
			cb->mesh_prov_invalid_bearer(opcode);
		}
	}
}

void bt_test_mesh_trans_incomp_timer_exp(void)
{
	struct bt_test_cb *cb;

	SYS_SLIST_FOR_EACH_CONTAINER(&cb_slist, cb, node) {
		if (cb->mesh_trans_incomp_timer_exp) {
			cb->mesh_trans_incomp_timer_exp();
		}
	}
}

int bt_test_mesh_lpn_group_add(u16_t group)
{
	bt_mesh_lpn_group_add(group);

	return 0;
}

int bt_test_mesh_lpn_group_remove(u16_t *groups, size_t groups_count)
{
	bt_mesh_lpn_group_del(groups, groups_count);

	return 0;
}

int bt_test_mesh_rpl_clear(void)
{
	bt_mesh_rpl_clear();

	return 0;
}

void bt_test_print_credentials(void)
{
	int i;
	u8_t nid;
	const u8_t *enc;
	const u8_t *priv;
	struct bt_mesh_subnet *sub;
	struct bt_mesh_app_key *app_key;

	console_printf("IV Index: %08lx\n", (long) bt_mesh.iv_index);
	console_printf("Dev key: %s\n", bt_hex(bt_mesh.dev_key, 16));

	for (i = 0; i < MYNEWT_VAL(BLE_MESH_SUBNET_COUNT); ++i)
	{
		if (bt_mesh.app_keys[i].net_idx == BT_MESH_KEY_UNUSED) {
			continue;
		}

		sub = &bt_mesh.sub[i];

		console_printf("Subnet: %d\n", i);
		console_printf("\tNetKeyIdx: %04x\n",
			       sub->net_idx);
		console_printf("\tNetKey: %s\n",
			       bt_hex(sub->keys[sub->kr_flag].net, 16));
	}

	for (i = 0; i < MYNEWT_VAL(BLE_MESH_APP_KEY_COUNT); ++i)
	{
		if (bt_mesh.app_keys[i].net_idx == BT_MESH_KEY_UNUSED) {
			continue;
		}

		sub = &bt_mesh.sub[i];
		app_key = &bt_mesh.app_keys[i];

		console_printf("AppKey: %d\n", i);
		console_printf("\tNetKeyIdx: %04x\n",
			       app_key->net_idx);
		console_printf("\tAppKeyIdx: %04x\n",
			       app_key->app_idx);
		console_printf("\tAppKey: %s\n",
			       bt_hex(app_key->keys[sub->kr_flag].val, 16));
	}

	for (i = 0; i < MYNEWT_VAL(BLE_MESH_SUBNET_COUNT); ++i)
	{
		if (bt_mesh.sub[i].net_idx == BT_MESH_KEY_UNUSED) {
			continue;
		}

		if (friend_cred_get(&bt_mesh.sub[i], BT_MESH_ADDR_UNASSIGNED,
				&nid, &enc, &priv)) {
			return;
		}

		console_printf("Friend cred: %d\n", i);
		console_printf("\tNetKeyIdx: %04x\n",
			       bt_mesh.sub[i].net_idx);
		console_printf("\tNID: %02x\n", nid);
		console_printf("\tEncKey: %s\n",
			       bt_hex(enc, 16));
		console_printf("\tPrivKey: %s\n",
			       bt_hex(priv, 16));
	}
}

int bt_test_shell_init(void)
{
#if MYNEWT_VAL(BLE_MESH_SHELL)
	return cmd_mesh_init(0, NULL);
#else
	return -ENOTSUP;
#endif
}

int bt_test_bind_app_key_to_model(struct bt_mesh_model *model, u16_t key_idx, u16_t id)
{
	struct bt_mesh_model *found_model;

	found_model = bt_mesh_model_find(bt_mesh_model_elem(model), id);
	if (!found_model) {
		return STATUS_INVALID_MODEL;
	}

	return mod_bind(found_model, key_idx);
}
