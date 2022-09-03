/*  Bluetooth Mesh */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __FRIEND_H__
#define __FRIEND_H__

#include "../include/mesh/mesh.h"

enum bt_mesh_friend_pdu_type {
	BT_MESH_FRIEND_PDU_SINGLE,
	BT_MESH_FRIEND_PDU_PARTIAL,
	BT_MESH_FRIEND_PDU_COMPLETE,
};

bool bt_mesh_friend_match(uint16_t net_idx, uint16_t addr);

struct bt_mesh_friend *bt_mesh_friend_find(uint16_t net_idx, uint16_t lpn_addr,
					   bool valid, bool established);

bool bt_mesh_friend_queue_has_space(uint16_t net_idx, uint16_t src, uint16_t dst,
				    uint64_t *seq_auth, uint8_t seg_count);

void bt_mesh_friend_enqueue_rx(struct bt_mesh_net_rx *rx,
			       enum bt_mesh_friend_pdu_type type,
			       uint64_t *seq_auth, uint8_t seg_count,
			       struct os_mbuf *sbuf);
bool bt_mesh_friend_enqueue_tx(struct bt_mesh_net_tx *tx,
			       enum bt_mesh_friend_pdu_type type,
			       uint64_t *seq_auth, uint8_t seg_count,
			       struct os_mbuf *sbuf);

void bt_mesh_friend_clear_incomplete(struct bt_mesh_subnet *sub, uint16_t src,
				     uint16_t dst, uint64_t *seq_auth);

void bt_mesh_friend_sec_update(uint16_t net_idx);

void bt_mesh_friends_clear(void);

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
