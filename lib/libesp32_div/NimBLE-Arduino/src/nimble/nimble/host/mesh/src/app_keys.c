/*
 * Copyright (c) 2017 Intel Corporation
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#if MYNEWT_VAL(BLE_MESH)

#include <string.h>
#include <stdlib.h>
#include "../include/mesh/mesh.h"
#include "mesh_priv.h"
#include "net.h"
#include "app_keys.h"
#include "rpl.h"
#include "settings.h"
#include "crypto.h"
#include "adv.h"
#include "proxy.h"
#include "friend.h"
#include "foundation.h"
#include "access.h"
#include "subnet.h"

#define MESH_LOG_MODULE BLE_MESH_LOG
#include "nimble/porting/nimble/include/log/log.h"

static struct bt_mesh_app_key apps[CONFIG_BT_MESH_APP_KEY_COUNT] = {
	[0 ... (CONFIG_BT_MESH_APP_KEY_COUNT - 1)] = {
		.app_idx = BT_MESH_KEY_UNUSED,
		.net_idx = BT_MESH_KEY_UNUSED,
	}
};

static void app_key_evt(struct bt_mesh_app_key *app, enum bt_mesh_key_evt evt)
{
	int i;

	for (i = 0; i < (sizeof(bt_mesh_app_key_cb_list)/sizeof(void *)); i++) {
		if (bt_mesh_app_key_cb_list[i]) {
			BT_DBG("app_key_evt %d", i);
			bt_mesh_app_key_cb_list[i] (app->app_idx, app->net_idx, evt);
		}
	}
}

struct bt_mesh_app_key *app_get(uint16_t app_idx)
{
	for (int i = 0; i < ARRAY_SIZE(apps); i++) {
		if (apps[i].app_idx == app_idx) {
			return &apps[i];
		}
	}

	return NULL;
}

static struct bt_mesh_app_key *app_key_alloc(uint16_t app_idx)
{
	struct bt_mesh_app_key *app = NULL;

	for (int i = 0; i < ARRAY_SIZE(apps); i++) {
		/* Check for already existing app_key */
		if (apps[i].app_idx == app_idx) {
			return &apps[i];
		}

		if (!app && apps[i].app_idx == BT_MESH_KEY_UNUSED) {
			app = &apps[i];
		}
	}

	return app;
}

static void app_key_del(struct bt_mesh_app_key *app)
{
	BT_DBG("AppIdx 0x%03x", app->app_idx);

	if (IS_ENABLED(CONFIG_BT_SETTINGS)) {
		bt_mesh_clear_app_key(app->app_idx);
	}

	app_key_evt(app, BT_MESH_KEY_DELETED);

	app->net_idx = BT_MESH_KEY_UNUSED;
	app->app_idx = BT_MESH_KEY_UNUSED;
	(void)memset(app->keys, 0, sizeof(app->keys));
}

static void app_key_revoke(struct bt_mesh_app_key *app)
{
	if (!app->updated) {
		return;
	}

	memcpy(&app->keys[0], &app->keys[1], sizeof(app->keys[0]));
	memset(&app->keys[1], 0, sizeof(app->keys[1]));
	app->updated = false;

	if (IS_ENABLED(CONFIG_BT_SETTINGS)) {
		bt_mesh_store_app_key(app->app_idx);
	}

	app_key_evt(app, BT_MESH_KEY_REVOKED);
}


static void subnet_evt(struct bt_mesh_subnet *sub, enum bt_mesh_key_evt evt)
{
	if (evt == BT_MESH_KEY_UPDATED || evt == BT_MESH_KEY_ADDED) {
		return;
	}

	for (int i = 0; i < ARRAY_SIZE(apps); i++) {
		struct bt_mesh_app_key *app = &apps[i];

		if (app->app_idx == BT_MESH_KEY_UNUSED) {
			continue;
		}

		if (app->net_idx != sub->net_idx) {
			continue;
		}

		if (evt == BT_MESH_KEY_DELETED) {
			app_key_del(app);
		} else if (evt == BT_MESH_KEY_REVOKED) {
			app_key_revoke(app);
		} else if (evt == BT_MESH_KEY_SWAPPED && app->updated) {
			app_key_evt(app, BT_MESH_KEY_SWAPPED);
		}
	}
}


uint8_t bt_mesh_app_key_add(uint16_t app_idx, uint16_t net_idx,
			const uint8_t key[16])
{
	if (!bt_mesh_subnet_cb_list[0]) {
		bt_mesh_subnet_cb_list[0] = subnet_evt;
	}

	struct bt_mesh_app_key *app;

	BT_DBG("net_idx 0x%04x app_idx %04x val %s", net_idx, app_idx,
	       bt_hex(key, 16));

	if (!bt_mesh_subnet_get(net_idx)) {
		return STATUS_INVALID_NETKEY;
	}

	app = app_key_alloc(app_idx);
	if (!app) {
		return STATUS_INSUFF_RESOURCES;
	}

	if (app->app_idx == app_idx) {
		if (app->net_idx != net_idx) {
			return STATUS_INVALID_BINDING;
		}

		if (memcmp(key, app->keys[0].val, 16)) {
			return STATUS_IDX_ALREADY_STORED;
		}

		return STATUS_SUCCESS;
	}

	if (bt_mesh_app_id(key, &app->keys[0].id)) {
		return STATUS_CANNOT_SET;
	}

	BT_DBG("AppIdx 0x%04x AID 0x%02x", app_idx, app->keys[0].id);

	app->net_idx = net_idx;
	app->app_idx = app_idx;
	app->updated = false;
	memcpy(app->keys[0].val, key, 16);

	if (IS_ENABLED(CONFIG_BT_SETTINGS)) {
		BT_DBG("Storing AppKey persistently");
		bt_mesh_store_app_key(app->app_idx);
	}

	app_key_evt(app, BT_MESH_KEY_ADDED);

	return STATUS_SUCCESS;
}

struct bt_mesh_app_key *bt_mesh_app_key_get(uint16_t app_idx)
{
	struct bt_mesh_app_key *app;

	app = app_get(app_idx);
	if (app) {
		return app;
	}

	return NULL;
}

uint8_t bt_mesh_app_key_update(uint16_t app_idx, uint16_t net_idx,
			       const uint8_t key[16])
{
	struct bt_mesh_app_key *app;
	struct bt_mesh_subnet *sub;

	BT_DBG("net_idx 0x%04x app_idx %04x val %s", net_idx, app_idx,
	       bt_hex(key, 16));

	app = app_get(app_idx);
	if (!app) {
		return STATUS_INVALID_APPKEY;
	}

	if (net_idx != BT_MESH_KEY_UNUSED && app->net_idx != net_idx) {
		return STATUS_INVALID_BINDING;
	}

	sub = bt_mesh_subnet_get(app->net_idx);
	if (!sub) {
		return STATUS_INVALID_NETKEY;
	}

	/* The AppKey Update message shall generate an error when node
	 * is in normal operation, Phase 2, or Phase 3 or in Phase 1
	 * when the AppKey Update message on a valid AppKeyIndex when
	 * the AppKey value is different.
	 */
	if (sub->kr_phase != BT_MESH_KR_PHASE_1) {
		return STATUS_CANNOT_UPDATE;
	}

	if (app->updated) {
		if (memcmp(app->keys[1].val, key, 16)) {
			return STATUS_IDX_ALREADY_STORED;
		}

		return STATUS_SUCCESS;
	}

	if (bt_mesh_app_id(key, &app->keys[1].id)) {
		return STATUS_CANNOT_UPDATE;
	}

	BT_DBG("app_idx 0x%04x AID 0x%02x", app_idx, app->keys[1].id);

	app->updated = true;
	memcpy(app->keys[1].val, key, 16);

	if (IS_ENABLED(CONFIG_BT_SETTINGS)) {
		BT_DBG("Storing AppKey persistently");
		bt_mesh_store_app_key(app->app_idx);
	}

	app_key_evt(app, BT_MESH_KEY_UPDATED);

	return STATUS_SUCCESS;
}

uint8_t bt_mesh_app_key_del(uint16_t app_idx, uint16_t net_idx)
{
	struct bt_mesh_app_key *app;

	BT_DBG("AppIdx 0x%03x", app_idx);

	if (net_idx != BT_MESH_KEY_UNUSED && !bt_mesh_subnet_get(net_idx)) {
		return STATUS_INVALID_NETKEY;
	}

	app = app_get(app_idx);
	if (!app) {
		/* This could be a retry of a previous attempt that had its
		 * response lost, so pretend that it was a success.
		 */
		return STATUS_SUCCESS;
	}

	if (net_idx != BT_MESH_KEY_UNUSED && net_idx != app->net_idx) {
		return STATUS_INVALID_BINDING;
	}

	app_key_del(app);

	return STATUS_SUCCESS;
}

int bt_mesh_app_key_set(uint16_t app_idx, uint16_t net_idx,
		    const uint8_t old_key[16], const uint8_t new_key[16])
{
	struct bt_mesh_app_key *app;

	app = app_key_alloc(app_idx);
	if (!app) {
		return -ENOMEM;
	}

	if (app->app_idx == app_idx) {
		return 0;
	}

	BT_DBG("AppIdx 0x%04x AID 0x%02x", app_idx, app->keys[0].id);

	memcpy(app->keys[0].val, old_key, 16);
	if (bt_mesh_app_id(old_key, &app->keys[0].id)) {
		return -EIO;
	}

	if (new_key) {
		memcpy(app->keys[1].val, new_key, 16);
		if (bt_mesh_app_id(new_key, &app->keys[1].id)) {
			return -EIO;
		}
	}

	app->net_idx = net_idx;
	app->app_idx = app_idx;
	app->updated = !!new_key;

	return 0;
}

bool bt_mesh_app_key_exists(uint16_t app_idx)
{
	for (int i = 0; i < ARRAY_SIZE(apps); i++) {
		if (apps[i].app_idx == app_idx) {
			return true;
		}
	}

	return false;
}

ssize_t bt_mesh_app_keys_get(uint16_t net_idx, uint16_t app_idxs[], size_t max,
			     off_t skip)
{
	size_t count = 0;

	for (int i = 0; i < ARRAY_SIZE(apps); i++) {
		struct bt_mesh_app_key *app = &apps[i];

		if (app->app_idx == BT_MESH_KEY_UNUSED) {
			continue;
		}

		if (net_idx != BT_MESH_KEY_ANY && app->net_idx != net_idx) {
			continue;
		}

		if (skip) {
			skip--;
			continue;
		}

		if (count >= max) {
			return -ENOMEM;
		}

		app_idxs[count++] = app->app_idx;
	}

	return count;
}

int bt_mesh_keys_resolve(struct bt_mesh_msg_ctx *ctx,
			 struct bt_mesh_subnet **sub,
			 const uint8_t **app_key, uint8_t *aid)
{
	struct bt_mesh_app_key *app = NULL;

	if (BT_MESH_IS_DEV_KEY(ctx->app_idx)) {
		/* With device keys, the application has to decide which subnet
		 * to send on.
		 */
		*sub = bt_mesh_subnet_get(ctx->net_idx);
		if (!*sub) {
			BT_WARN("Unknown NetKey 0x%03x", ctx->net_idx);
			return -EINVAL;
		}

		if (ctx->app_idx == BT_MESH_KEY_DEV_REMOTE &&
		    !bt_mesh_elem_find(ctx->addr)) {
			struct bt_mesh_cdb_node *node;

			if (!IS_ENABLED(CONFIG_BT_MESH_CDB)) {
				BT_WARN("No DevKey for 0x%04x", ctx->addr);
				return -EINVAL;
			}

			node = bt_mesh_cdb_node_get(ctx->addr);
			if (!node) {
				BT_WARN("No DevKey for 0x%04x", ctx->addr);
				return -EINVAL;
			}

			*app_key = node->dev_key;
		} else {
			*app_key = bt_mesh.dev_key;
		}

		*aid = 0;
		return 0;
	}

	app = app_get(ctx->app_idx);
	if (!app) {
		BT_WARN("Unknown AppKey 0x%03x", ctx->app_idx);
		return -EINVAL;
	}

	*sub = bt_mesh_subnet_get(app->net_idx);
	if (!*sub) {
		BT_WARN("Unknown NetKey 0x%03x", app->net_idx);
		return -EINVAL;
	}

	if ((*sub)->kr_phase == BT_MESH_KR_PHASE_2 && app->updated) {
		*aid = app->keys[1].id;
		*app_key = app->keys[1].val;
	} else {
		*aid = app->keys[0].id;
		*app_key = app->keys[0].val;
	}

	return 0;
}

uint16_t bt_mesh_app_key_find(bool dev_key, uint8_t aid,
			      struct bt_mesh_net_rx *rx,
			      int (*cb)(struct bt_mesh_net_rx *rx,
					const uint8_t key[16], void *cb_data),
			      void *cb_data)
{
	int err, i;

	if (dev_key) {
		/* Attempt remote dev key first, as that is only available for
		 * provisioner devices, which normally don't interact with nodes
		 * that know their local dev key.
		 */
		if (IS_ENABLED(CONFIG_BT_MESH_CDB) &&
		    rx->net_if != BT_MESH_NET_IF_LOCAL) {
			struct bt_mesh_cdb_node *node;

			node = bt_mesh_cdb_node_get(rx->ctx.addr);
			if (node && !cb(rx, node->dev_key, cb_data)) {
				return BT_MESH_KEY_DEV_REMOTE;
			}
		}

		/** Bluetooth Mesh Specification v1.0.1, section 3.4.3:
		 *  The Device key is only valid for unicast addresses.
		 */
		if (BT_MESH_ADDR_IS_UNICAST(rx->ctx.recv_dst)) {
			err = cb(rx, bt_mesh.dev_key, cb_data);
			if (!err) {
				return BT_MESH_KEY_DEV_LOCAL;
			}
		}

		return BT_MESH_KEY_UNUSED;
	}

	for (i = 0; i < ARRAY_SIZE(apps); i++) {
		const struct bt_mesh_app_key *app = &apps[i];
		const struct bt_mesh_app_cred *cred;

		if (app->app_idx == BT_MESH_KEY_UNUSED) {
			continue;
		}

		if (app->net_idx != rx->sub->net_idx) {
			continue;
		}

		if (rx->new_key && app->updated) {
			cred = &app->keys[1];
		} else {
			cred = &app->keys[0];
		}

		if (cred->id != aid) {
			continue;
		}

		err = cb(rx, cred->val, cb_data);
		if (err) {
			continue;
		}

		return app->app_idx;
	}

	return BT_MESH_KEY_UNUSED;
}


void bt_mesh_app_keys_reset(void)
{
	for (int i = 0; i < ARRAY_SIZE(apps); i++) {
		struct bt_mesh_app_key *app = &apps[i];

		if (app->app_idx != BT_MESH_KEY_UNUSED) {
			app_key_del(app);
		}
	}
}

#endif /* MYNEWT_VAL(BLE_MESH) */