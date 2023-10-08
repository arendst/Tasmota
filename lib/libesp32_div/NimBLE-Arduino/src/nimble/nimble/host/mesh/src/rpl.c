/*  Bluetooth Mesh */

/*
 * Copyright (c) 2017 Intel Corporation
 * Copyright (c) 2020 Lingao Meng
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define MESH_LOG_MODULE BLE_MESH_RPL_LOG

#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#if MYNEWT_VAL(BLE_MESH)

#include "nimble/porting/nimble/include/log/log.h"

#include "mesh_priv.h"
#include "adv.h"
#include "net.h"
#include "rpl.h"
#include "settings.h"

static struct bt_mesh_rpl replay_list[MYNEWT_VAL(BLE_MESH_CRPL)];

void bt_mesh_rpl_update(struct bt_mesh_rpl *rpl,
		struct bt_mesh_net_rx *rx)
{
	rpl->src = rx->ctx.addr;
	rpl->seq = rx->seq;
	rpl->old_iv = rx->old_iv;

	if (IS_ENABLED(CONFIG_BT_SETTINGS)) {
		bt_mesh_store_rpl(rpl);
	}
}

/* Check the Replay Protection List for a replay attempt. If non-NULL match
 * parameter is given the RPL slot is returned but it is not immediately
 * updated (needed for segmented messages), whereas if a NULL match is given
 * the RPL is immediately updated (used for unsegmented messages).
 */
bool bt_mesh_rpl_check(struct bt_mesh_net_rx *rx,
		struct bt_mesh_rpl **match)
{
	int i;

	/* Don't bother checking messages from ourselves */
	if (rx->net_if == BT_MESH_NET_IF_LOCAL) {
		return false;
	}

	/* The RPL is used only for the local node */
	if (!rx->local_match) {
		return false;
	}

	for (i = 0; i < ARRAY_SIZE(replay_list); i++) {
		struct bt_mesh_rpl *rpl = &replay_list[i];

		/* Empty slot */
		if (!rpl->src) {
			if (match) {
				*match = rpl;
			} else {
				bt_mesh_rpl_update(rpl, rx);
			}

			return false;
		}

		/* Existing slot for given address */
		if (rpl->src == rx->ctx.addr) {
			if (rx->old_iv && !rpl->old_iv) {
				return true;
			}

			if ((!rx->old_iv && rpl->old_iv) ||
			    rpl->seq < rx->seq) {
				if (match) {
					*match = rpl;
				} else {
					bt_mesh_rpl_update(rpl, rx);
				}

				return false;
			} else {
				return true;
			}
		}
	}

	BT_ERR("RPL is full!");
	return true;
}

void bt_mesh_rpl_clear(void)
{
	BT_DBG("");

	if (IS_ENABLED(CONFIG_BT_SETTINGS)) {
		bt_mesh_clear_rpl();
	} else {
		(void)memset(replay_list, 0, sizeof(replay_list));
	}
}

struct bt_mesh_rpl *bt_mesh_rpl_find(uint16_t src)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(replay_list); i++) {
		if (replay_list[i].src == src) {
			return &replay_list[i];
		}
	}

	return NULL;
}

struct bt_mesh_rpl *bt_mesh_rpl_alloc(uint16_t src)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(replay_list); i++) {
		if (!replay_list[i].src) {
			replay_list[i].src = src;
			return &replay_list[i];
		}
	}

	return NULL;
}

void bt_mesh_rpl_foreach(bt_mesh_rpl_func_t func, void *user_data)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(replay_list); i++) {
		func(&replay_list[i], user_data);
	}
}

void bt_mesh_rpl_reset(void)
{
	int i;

	/* Discard "old old" IV Index entries from RPL and flag
	 * any other ones (which are valid) as old.
	 */
	for (i = 0; i < ARRAY_SIZE(replay_list); i++) {
		struct bt_mesh_rpl *rpl = &replay_list[i];

		if (rpl->src) {
			if (rpl->old_iv) {
				(void)memset(rpl, 0, sizeof(*rpl));
			} else {
				rpl->old_iv = true;
			}

			if (IS_ENABLED(CONFIG_BT_SETTINGS)) {
				bt_mesh_store_rpl(rpl);
			}
		}
	}
}

#endif /* MYNEWT_VAL(BLE_MESH) */