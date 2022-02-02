/*  Bluetooth Mesh */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __ACCESS_H__
#define __ACCESS_H__

#include "mesh/mesh.h"

/* bt_mesh_model.flags */
enum {
	BT_MESH_MOD_BIND_PENDING = BIT(0),
	BT_MESH_MOD_SUB_PENDING = BIT(1),
	BT_MESH_MOD_PUB_PENDING = BIT(2),
	BT_MESH_MOD_DATA_PRESENT = BIT(3),
	BT_MESH_MOD_NEXT_IS_PARENT = BIT(4),
};

/* Tree walk return codes */
enum bt_mesh_walk {
	BT_MESH_WALK_STOP,
	BT_MESH_WALK_CONTINUE,
};

void bt_mesh_elem_register(struct bt_mesh_elem *elem, u8_t count);

u8_t bt_mesh_elem_count(void);

/* Find local element based on unicast or group address */
struct bt_mesh_elem *bt_mesh_elem_find(u16_t addr);

struct bt_mesh_model *bt_mesh_model_root(struct bt_mesh_model *mod);
void bt_mesh_model_tree_walk(struct bt_mesh_model *root,
			     enum bt_mesh_walk (*cb)(struct bt_mesh_model *mod,
						     u32_t depth,
						     void *user_data),
			     void *user_data);

u16_t *bt_mesh_model_find_group(struct bt_mesh_model **mod, u16_t addr);

bool bt_mesh_fixed_group_match(u16_t addr);

void bt_mesh_model_foreach(void (*func)(struct bt_mesh_model *mod,
					struct bt_mesh_elem *elem,
					bool vnd, bool primary,
					void *user_data),
			   void *user_data);

s32_t bt_mesh_model_pub_period_get(struct bt_mesh_model *mod);

void bt_mesh_comp_provision(u16_t addr);
void bt_mesh_comp_unprovision(void);

u16_t bt_mesh_primary_addr(void);

const struct bt_mesh_comp *bt_mesh_comp_get(void);

struct bt_mesh_model *bt_mesh_model_get(bool vnd, u8_t elem_idx, u8_t mod_idx);

void bt_mesh_model_recv(struct bt_mesh_net_rx *rx, struct os_mbuf *buf);

int bt_mesh_comp_register(const struct bt_mesh_comp *comp);
#endif
