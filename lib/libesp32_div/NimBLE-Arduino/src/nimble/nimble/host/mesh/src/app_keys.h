/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _BT_MESH_APP_KEYS_H_
#define _BT_MESH_APP_KEYS_H_

#include "../include/mesh/mesh.h"
#include "subnet.h"

/** Mesh Application. */
struct bt_mesh_app_key {
	uint16_t net_idx;
	uint16_t app_idx;
	bool updated;
	struct bt_mesh_app_cred {
		uint8_t id;
		uint8_t val[16];
	} keys[2];
};

/** @brief Reset the app keys module. */
void bt_mesh_app_keys_reset(void);

/** @brief Get the application key with the given AppIdx.
 *
 *  @param app_idx App index.
 *
 *  @return The matching application, or NULL if the application isn't known.
 */
struct bt_mesh_app_key *bt_mesh_app_key_get(uint16_t app_idx);

/** @brief Initialize a new application key with the given parameters.
 *
 *  @param app_idx AppIndex.
 *  @param net_idx NetIndex the application is bound to.
 *  @param old_key Current application key.
 *  @param new_key Updated application key, or NULL if not known.
 *
 *  @return 0 on success, or (negative) error code on failure.
 */
int bt_mesh_app_key_set(uint16_t app_idx, uint16_t net_idx,
			const uint8_t old_key[16], const uint8_t new_key[16]);

/** @brief Resolve the message encryption keys, given a message context.
 *
 *  Will use the @c ctx::app_idx and @c ctx::net_idx fields to find a pair of
 *  message encryption keys. If @c ctx::app_idx represents a device key, the
 *  @c ctx::net_idx will be used to determine the net key. Otherwise, the
 *  @c ctx::net_idx parameter will be ignored.
 *
 *  @param ctx     Message context.
 *  @param sub     Subnet return parameter.
 *  @param app_key Application return parameter.
 *  @param aid     Application ID return parameter.
 *
 *  @return 0 on success, or (negative) error code on failure.
 */
int bt_mesh_keys_resolve(struct bt_mesh_msg_ctx *ctx,
			 struct bt_mesh_subnet **sub,
			 const uint8_t **app_key, uint8_t *aid);

/** @brief Iterate through all matching application keys and call @c cb on each.
 *
 *  @param dev_key Whether to return device keys.
 *  @param aid     7 bit application ID to match.
 *  @param rx      RX structure to match against.
 *  @param cb      Callback to call for every valid app key.
 *  @param cb_data Callback data to pass to the callback.
 *
 *  @return The AppIdx that yielded a 0-return from the callback.
 */
uint16_t bt_mesh_app_key_find(bool dev_key, uint8_t aid,
			      struct bt_mesh_net_rx *rx,
			      int (*cb)(struct bt_mesh_net_rx *rx,
					const uint8_t key[16], void *cb_data),
			      void *cb_data);

struct bt_mesh_app_key *app_get(uint16_t app_idx);

extern void (*bt_mesh_app_key_cb_list[1]) (uint16_t app_idx, uint16_t net_idx,
			enum bt_mesh_key_evt evt);

#endif /* _BT_MESH_APP_KEYS_H_ */