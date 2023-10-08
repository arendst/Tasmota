/*
 * Copyright (c) 2019 Tobias Svehagen
 *
 * SPDX-License-Identifier: Apache-2.0
 */

struct bt_mesh_node *bt_mesh_node_find(u16_t addr);
struct bt_mesh_node *bt_mesh_node_alloc(u16_t addr, u8_t num_elem,
					u16_t net_idx);
void bt_mesh_node_del(struct bt_mesh_node *node, bool store);