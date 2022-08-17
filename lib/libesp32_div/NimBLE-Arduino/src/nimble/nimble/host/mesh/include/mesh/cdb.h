/*
 * Copyright (c) 2019 Tobias Svehagen
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _BLUETOOTH_MESH_CDB_H_
#define _BLUETOOTH_MESH_CDB_H_

#include "nimble/porting/nimble/include/syscfg/syscfg.h"

#if MYNEWT_VAL(BLE_MESH_CDB)
#define NODE_COUNT    CONFIG_BT_MESH_NODE_COUNT
#define SUBNET_COUNT  CONFIG_BT_MESH_SUBNET_COUNT
#define APP_KEY_COUNT CONFIG_BT_MESH_APP_KEY_COUNT
#else
#define NODE_COUNT    0
#define SUBNET_COUNT  0
#define APP_KEY_COUNT 0
#endif

#include "atomic.h"

enum {
	BT_MESH_CDB_NODE_CONFIGURED,
	BT_MESH_CDB_NODE_BLACKLISTED,

	BT_MESH_CDB_NODE_FLAG_COUNT
};

struct bt_mesh_cdb_node {
	uint8_t  uuid[16];
	uint16_t addr;
	uint16_t net_idx;
	uint8_t  num_elem;
	uint8_t  dev_key[16];

	ATOMIC_DEFINE(flags, BT_MESH_CDB_NODE_FLAG_COUNT);
};

struct bt_mesh_cdb_subnet {
	uint16_t net_idx;

	bool kr_flag;
	uint8_t kr_phase;

	struct {
		uint8_t net_key[16];
	} keys[2];
};

struct bt_mesh_cdb_app_key {
	uint16_t net_idx;
	uint16_t app_idx;

	struct {
		uint8_t app_key[16];
	} keys[2];
};

enum {
	BT_MESH_CDB_VALID,
	BT_MESH_CDB_SUBNET_PENDING,
	BT_MESH_CDB_KEYS_PENDING,
	BT_MESH_CDB_NODES_PENDING,
	BT_MESH_CDB_IVU_IN_PROGRESS,

	BT_MESH_CDB_FLAG_COUNT,
};

struct bt_mesh_cdb {
	uint32_t iv_index;

	ATOMIC_DEFINE(flags, BT_MESH_CDB_FLAG_COUNT);

	struct bt_mesh_cdb_node nodes[NODE_COUNT];
	struct bt_mesh_cdb_subnet subnets[SUBNET_COUNT];
	struct bt_mesh_cdb_app_key app_keys[APP_KEY_COUNT];
};

extern struct bt_mesh_cdb bt_mesh_cdb;

/** @brief Create the Mesh Configuration Database.
 *
 *  Create and initialize the Mesh Configuration Database. A primary subnet,
 *  ie one with NetIdx 0, will be added and the provided key will be used as
 *  NetKey for that subnet.
 *
 *  @param key The NetKey to be used for the primary subnet.
 *
 *  @return 0 on success or negative error code on failure.
 */
int bt_mesh_cdb_create(const uint8_t key[16]);

/** @brief Clear the Mesh Configuration Database.
 *
 *  Remove all nodes, subnets and app-keys stored in the database and mark
 *  the database as invalid. The data will be cleared from persistent storage
 *  if CONFIG_BT_SETTINGS is enabled.
 */
void bt_mesh_cdb_clear(void);

/** @brief Set and store the IV Index and IV Update flag.
 *
 *  The IV Index stored in the CDB will be the one used during provisioning
 *  of new nodes. This function is generally only used from inside the stack.
 *
 *  This function will store the data to persistent storage if
 *  CONFIG_BT_SETTINGS is enabled.
 *
 *  @param iv_index The new IV Index to use.
 *  @param iv_update True if there is an ongoing IV Update procedure.
 */
void bt_mesh_cdb_iv_update(uint32_t iv_index, bool iv_update);

/** @brief Allocate a node.
 *
 *  Allocate a new node in the CDB.
 *
 *  @param uuid UUID of the node.
 *  @param addr Address of the node's primary element. If 0, the lowest
 *              possible address available will be assigned to the node.
 *  @param num_elem Number of elements that the node has.
 *  @param net_idx NetIdx that the node was provisioned to.
 *
 *  @return The new node or NULL if it cannot be allocated.
 */
struct bt_mesh_cdb_node *bt_mesh_cdb_node_alloc(const uint8_t uuid[16], uint16_t addr,
						uint8_t num_elem, uint16_t net_idx);

/** @brief Delete a node.
 *
 *  Delete a node from the CDB.
 *
 *  @param node The node to be deleted.
 *  @param store If true, the node will be cleared from persistent storage.
 */
void bt_mesh_cdb_node_del(struct bt_mesh_cdb_node *node, bool store);

/** @brief Get a node by address.
 *
 *  Try to find the node that has the provided address assigned to one of its
 *  elements.
 *
 *  @param addr Address of the element to look for.
 *
 *  @return The node that has an element with address addr or NULL if no such
 *          node exists.
 */
struct bt_mesh_cdb_node *bt_mesh_cdb_node_get(uint16_t addr);

/** @brief Store node to persistent storage.
 *
 *  @param node Node to be stored.
 */
void bt_mesh_cdb_node_store(const struct bt_mesh_cdb_node *node);

enum {
	BT_MESH_CDB_ITER_STOP = 0,
	BT_MESH_CDB_ITER_CONTINUE,
};

/** @typedef bt_mesh_cdb_node_func_t
 *  @brief Node iterator callback.
 *
 *  @param node Node found.
 *  @param user_data Data given.
 *
 *  @return BT_MESH_CDB_ITER_CONTINUE to continue to iterate through the nodes
 *          or BT_MESH_CDB_ITER_STOP to stop.
 */
typedef uint8_t (*bt_mesh_cdb_node_func_t)(struct bt_mesh_cdb_node *node,
					void *user_data);

/** @brief Node iterator.
 *
 *  Iterate nodes in the Mesh Configuration Database. The callback function
 *  will only be called for valid, ie allocated, nodes.
 *
 *  @param func Callback function.
 *  @param user_data Data to pass to the callback.
 */
void bt_mesh_cdb_node_foreach(bt_mesh_cdb_node_func_t func, void *user_data);

/** @brief Allocate a subnet.
 *
 *  Allocate a new subnet in the CDB.
 *
 *  @param net_idx NetIdx of the subnet.
 *
 *  @return The new subnet or NULL if it cannot be allocated.
 */
struct bt_mesh_cdb_subnet *bt_mesh_cdb_subnet_alloc(uint16_t net_idx);

/** @brief Delete a subnet.
 *
 *  Delete a subnet from the CDB.
 *
 *  @param sub The subnet to be deleted.
 *  @param store If true, the subnet will be cleared from persistent storage.
 */
void bt_mesh_cdb_subnet_del(struct bt_mesh_cdb_subnet *sub, bool store);

/** @brief Get a subnet by NetIdx
 *
 *  Try to find the subnet with the specified NetIdx.
 *
 *  @param net_idx NetIdx of the subnet to look for.
 *
 *  @return The subnet with the specified NetIdx or NULL if no such subnet
 *          exists.
 */
struct bt_mesh_cdb_subnet *bt_mesh_cdb_subnet_get(uint16_t net_idx);

/** @brief Store subnet to persistent storage.
 *
 *  @param sub Subnet to be stored.
 */
void bt_mesh_cdb_subnet_store(const struct bt_mesh_cdb_subnet *sub);

/** @brief Get the flags for a subnet
 *
 *  @param sub The subnet to get flags for.
 *
 *  @return The flags for the subnet.
 */
uint8_t bt_mesh_cdb_subnet_flags(const struct bt_mesh_cdb_subnet *sub);


/** @brief Allocate an application key.
 *
 *  Allocate a new application key in the CDB.
 *
 *  @param net_idx NetIdx of NetKey that the application key is bound to.
 *  @param app_idx AppIdx of the application key.
 *
 *  @return The new application key or NULL if it cannot be allocated.
 */
struct bt_mesh_cdb_app_key *bt_mesh_cdb_app_key_alloc(uint16_t net_idx,
						      uint16_t app_idx);

/** @brief Delete an application key.
 *
 *  Delete an application key from the CDB.
 *
 *  @param key The application key to be deleted.
 *  @param store If true, the key will be cleared from persistent storage.
 */
void bt_mesh_cdb_app_key_del(struct bt_mesh_cdb_app_key *key, bool store);

/** @brief Get an application key by AppIdx
 *
 *  Try to find the application key with the specified AppIdx.
 *
 *  @param app_idx AppIdx of the application key to look for.
 *
 *  @return The application key with the specified AppIdx or NULL if no such key
 *          exists.
 */
struct bt_mesh_cdb_app_key *bt_mesh_cdb_app_key_get(uint16_t app_idx);

/** @brief Store application key to persistent storage.
 *
 *  @param key Application key to be stored.
 */
void bt_mesh_cdb_app_key_store(const struct bt_mesh_cdb_app_key *key);

#endif /* ZEPHYR_INCLUDE_BLUETOOTH_MESH_CDB_H_ */