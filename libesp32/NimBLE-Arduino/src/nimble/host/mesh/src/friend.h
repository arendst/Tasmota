/*  Bluetooth Mesh */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __FRIEND_H__
#define __FRIEND_H__

#include "mesh/mesh.h"

enum bt_mesh_friend_pdu_type {
	BT_MESH_FRIEND_PDU_SINGLE,
	BT_MESH_FRIEND_PDU_PARTIAL,
	BT_MESH_FRIEND_PDU_COMPLETE,
};

bool bt_mesh_friend_match(u16_t net_idx, u16_t addr);

struct bt_mesh_friend *bt_mesh_friend_find(u16_t net_idx, u16_t lpn_addr,
					   bool valid, bool established);

void bt_mesh_friend_enqueue_rx(struct bt_mesh_net_rx *rx,
			       enum bt_mesh_friend_pdu_type type,
			       u64_t *seq_auth, struct os_mbuf *sbuf);
bool bt_mesh_friend_enqueue_tx(struct bt_mesh_net_tx *tx,
			       enum bt_mesh_friend_pdu_type type,
			       u64_t *seq_auth, struct os_mbuf *sbuf);

void bt_mesh_friend_clear_incomplete(struct bt_mesh_subnet *sub, u16_t src,
				     u16_t dst, u64_t *seq_auth);

void bt_mesh_friend_sec_update(u16_t net_idx);

void bt_mesh_friend_clear_net_idx(u16_t net_idx);

int bt_mesh_friend_poll(struct bt_mesh_net_rx *rx, struct os_mbuf *buf);
int bt_mesh_friend_req(struct bt_mesh_net_rx *rx, struct os_mbuf *buf);
int bt_mesh_friend_clear(struct bt_mesh_net_rx *rx, struct os_mbuf *buf);
int bt_mesh_friend_clear_cfm(struct bt_mesh_net_rx *rx,
			     struct os_mbuf *buf);
int bt_mesh_friend_sub_add(struct bt_mesh_net_rx *rx,
			   struct os_mbuf *buf);
int bt_mesh_friend_sub_rem(struct bt_mesh_net_rx *rx,
			   struct os_mbuf *buf);

int bt_mesh_friend_init(void);

#endif
