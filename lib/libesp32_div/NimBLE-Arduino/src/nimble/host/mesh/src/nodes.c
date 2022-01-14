/*
 * Copyright (c) 2019 Tobias Svehagen
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "syscfg/syscfg.h"
#define MESH_LOG_MODULE BLE_MESH_PROV_LOG

#if MYNEWT_VAL(BLE_MESH_PROVISIONER)

#include "mesh/mesh.h"

#include "mesh_priv.h"
#include "net.h"
#include "access.h"
#include "settings.h"

/*
 * Check if an address range from addr_start for addr_start + num_elem - 1 is
 * free for use. When a conflict is found, next will be set to the next address
 * available after the conflicting range and -EAGAIN will be returned.
 */
static int addr_is_free(u16_t addr_start, u8_t num_elem, u16_t *next)
{
	const struct bt_mesh_comp *comp = bt_mesh_comp_get();
	u16_t addr_end = addr_start + num_elem - 1;
	u16_t other_start, other_end;
	int i;

	if (comp == NULL) {
		return -EINVAL;
	}

	if (!BT_MESH_ADDR_IS_UNICAST(addr_start) ||
	    !BT_MESH_ADDR_IS_UNICAST(addr_end) ||
	    num_elem == 0 || next == NULL) {
		return -EINVAL;
	}

	other_start = bt_mesh_primary_addr();
	other_end = other_start + comp->elem_count - 1;

	/* Compare with local element addresses */
	if (!(addr_end < other_start || addr_start > other_end)) {
		*next = other_end + 1;
		return -EAGAIN;
	}

	for (i = 0; i < ARRAY_SIZE(bt_mesh.nodes); i++) {
		struct bt_mesh_node *node = &bt_mesh.nodes[i];

		if (node->net_idx == BT_MESH_KEY_UNUSED) {
			continue;
		}

		other_start = node->addr;
		other_end = other_start + node->num_elem - 1;

		if (!(addr_end < other_start || addr_start > other_end)) {
			*next = other_end + 1;
			return -EAGAIN;
		}
	}

	return 0;
}

/*
 * Find the lowest possible starting address that can fit num_elem elements. If
 * a free address range cannot be found, BT_MESH_ADDR_UNASSIGNED will be
 * returned. Otherwise the first address in the range is returned.
 *
 * NOTE: This is quite an ineffective algorithm as it might need to look
 *       through the array of nodes N+2 times. A more effective algorithm
 *       could be used if the nodes were stored in a sorted list.
 */
static u16_t find_lowest_free_addr(u8_t num_elem)
{
	u16_t addr = 1, next;
	int err, i;

	/*
	 * It takes a maximum of node count + 2 to find a free address if there
	 * is any. +1 for our own address and +1 for making sure that the
	 * address range is valid.
	 */
	for (i = 0; i < ARRAY_SIZE(bt_mesh.nodes) + 2; ++i) {
		err = addr_is_free(addr, num_elem, &next);
		if (err == 0) {
			break;
		} else if (err != -EAGAIN) {
			addr = BT_MESH_ADDR_UNASSIGNED;
			break;
		}

		addr = next;
	}

	return addr;
}

struct bt_mesh_node *bt_mesh_node_find(u16_t addr)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(bt_mesh.nodes); i++) {
		struct bt_mesh_node *node = &bt_mesh.nodes[i];

		if (addr >= node->addr &&
		    addr <= node->addr + node->num_elem - 1) {
			return node;
		}
	}

	return NULL;
}

struct bt_mesh_node *bt_mesh_node_alloc(u16_t addr, u8_t num_elem,
					u16_t net_idx)
{
	int i;

	BT_DBG("");

	if (addr == BT_MESH_ADDR_UNASSIGNED) {
		addr = find_lowest_free_addr(num_elem);
		if (addr == BT_MESH_ADDR_UNASSIGNED) {
			return NULL;
		}
	} else if (!addr_is_free(addr, num_elem, NULL)) {
		return NULL;
	}

	for (i = 0; i < ARRAY_SIZE(bt_mesh.nodes); i++) {
		struct bt_mesh_node *node = &bt_mesh.nodes[i];

		if (node->addr == BT_MESH_ADDR_UNASSIGNED) {
			node->addr = addr;
			node->num_elem = num_elem;
			node->net_idx = net_idx;
			return node;
		}
	}

	return NULL;
}

void bt_mesh_node_del(struct bt_mesh_node *node, bool store)
{
	BT_DBG("Node addr 0x%04x store %u", node->addr, store);

	if (IS_ENABLED(CONFIG_BT_SETTINGS) && store) {
		bt_mesh_clear_node(node);
	}

	node->addr = BT_MESH_ADDR_UNASSIGNED;
	(void)memset(node->dev_key, 0, sizeof(node->dev_key));
}

#endif
