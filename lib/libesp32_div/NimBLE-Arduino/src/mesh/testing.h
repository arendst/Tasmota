/**
 * @file testing.h
 * @brief Internal API for Bluetooth testing.
 */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __BT_TESTING_H
#define __BT_TESTING_H

#include "slist.h"
#include "glue.h"
#include "access.h"

/**
 * @brief Bluetooth testing
 * @defgroup bt_test_cb Bluetooth testing callbacks
 * @ingroup bluetooth
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Bluetooth Testing callbacks structure.
 *
 *  Callback structure to be used for Bluetooth testing purposes.
 *  Allows access to Bluetooth stack internals, not exposed by public API.
 */
struct bt_test_cb {
	void (*mesh_net_recv)(u8_t ttl, u8_t ctl, u16_t src, u16_t dst,
			      const void *payload, size_t payload_len);
	void (*mesh_model_bound)(u16_t addr, struct bt_mesh_model *model,
				 u16_t key_idx);
	void (*mesh_model_unbound)(u16_t addr, struct bt_mesh_model *model,
				   u16_t key_idx);
	void (*mesh_prov_invalid_bearer)(u8_t opcode);
	void (*mesh_trans_incomp_timer_exp)(void);

	sys_snode_t node;
};

/** Register callbacks for Bluetooth testing purposes
 *
 *  @param cb bt_test_cb callback structure
 */
void bt_test_cb_register(struct bt_test_cb *cb);

/** Unregister callbacks for Bluetooth testing purposes
 *
 *  @param cb bt_test_cb callback structure
 */
void bt_test_cb_unregister(struct bt_test_cb *cb);

/** Send Friend Subscription List Add message.
 *
 *  Used by Low Power node to send the group address for which messages are to
 *  be stored by Friend node.
 *
 *  @param group Group address
 *
 *  @return Zero on success or (negative) error code otherwise.
 */
int bt_test_mesh_lpn_group_add(u16_t group);

/** Send Friend Subscription List Remove message.
 *
 *  Used by Low Power node to remove the group addresses from Friend node
 *  subscription list. Messages sent to those addresses will not be stored
 *  by Friend node.
 *
 *  @param groups Group addresses
 *  @param groups_count Group addresses count
 *
 *  @return Zero on success or (negative) error code otherwise.
 */
int bt_test_mesh_lpn_group_remove(u16_t *groups, size_t groups_count);

/** Clear replay protection list cache.
 *
 *  @return Zero on success or (negative) error code otherwise.
 */
int bt_test_mesh_rpl_clear(void);

u8_t mod_bind(struct bt_mesh_model *model, u16_t key_idx);
u8_t mod_unbind(struct bt_mesh_model *model, u16_t key_idx, bool store);
int cmd_mesh_init(int argc, char *argv[]);

int bt_test_shell_init(void);
int bt_test_bind_app_key_to_model(struct bt_mesh_model *model, u16_t key_idx, u16_t id);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __BT_TESTING_H */
