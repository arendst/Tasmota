/*  Bluetooth Mesh */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __LPN_H__
#define __LPN_H__

#include "../include/mesh/mesh.h"

int bt_mesh_lpn_friend_update(struct bt_mesh_net_rx *rx,
			      struct os_mbuf *buf);
int bt_mesh_lpn_friend_offer(struct bt_mesh_net_rx *rx,
			     struct os_mbuf *buf);
int bt_mesh_lpn_friend_clear_cfm(struct bt_mesh_net_rx *rx,
				 struct os_mbuf *buf);
int bt_mesh_lpn_friend_sub_cfm(struct bt_mesh_net_rx *rx,
			       struct os_mbuf *buf);

static inline bool bt_mesh_lpn_established(void)
{
#if (MYNEWT_VAL(BLE_MESH_LOW_POWER))
	return bt_mesh.lpn.established;
#else
	return false;
#endif
}

static inline bool bt_mesh_lpn_match(uint16_t addr)
{
#if (MYNEWT_VAL(BLE_MESH_LOW_POWER))
	if (bt_mesh_lpn_established()) {
		return (addr == bt_mesh.lpn.frnd);
	}
#endif
	return false;
}

static inline bool bt_mesh_lpn_waiting_update(void)
{
#if (MYNEWT_VAL(BLE_MESH_LOW_POWER))
	return (bt_mesh.lpn.state == BT_MESH_LPN_WAIT_UPDATE);
#else
	return false;
#endif
}

static inline bool bt_mesh_lpn_timer(void)
{
#if MYNEWT_VAL(BLE_MESH_LOW_POWER) && MYNEWT_VAL(BLE_MESH_LPN_AUTO)
	return (bt_mesh.lpn.state == BT_MESH_LPN_TIMER);
#else
	return false;
#endif
}

void bt_mesh_lpn_msg_received(struct bt_mesh_net_rx *rx);

void bt_mesh_lpn_group_add(uint16_t group);
void bt_mesh_lpn_group_del(uint16_t *groups, size_t group_count);

void bt_mesh_lpn_disable(bool force);

int bt_mesh_lpn_init(void);

#endif
