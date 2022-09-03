/*  Bluetooth Mesh */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __MESH_PRIV_H
#define __MESH_PRIV_H

#include <stdbool.h>
#include <stdint.h>

#define BT_MESH_KEY_PRIMARY 0x0000
#define BT_MESH_KEY_ANY     0xffff

enum bt_mesh_key_evt {
	BT_MESH_KEY_ADDED,   /* New key added */
	BT_MESH_KEY_DELETED, /* Existing key deleted */
	BT_MESH_KEY_UPDATED, /* KR phase 1, second key added */
	BT_MESH_KEY_SWAPPED, /* KR phase 2, now sending on second key */
	BT_MESH_KEY_REVOKED, /* KR phase 3, old key removed */
};

/** Appkey callback. Instantiate with @ref BT_MESH_APP_KEY_CB */
struct bt_mesh_app_key_cb {
	void (*evt_handler)(uint16_t app_idx, uint16_t net_idx,
			    enum bt_mesh_key_evt evt);
};

struct bt_mesh_net;
int bt_mesh_start(void);

#define OP_GEN_ONOFF_GET		BT_MESH_MODEL_OP_2(0x82, 0x01)
#define OP_GEN_ONOFF_SET		BT_MESH_MODEL_OP_2(0x82, 0x02)
#define OP_GEN_ONOFF_SET_UNACK		BT_MESH_MODEL_OP_2(0x82, 0x03)
#define OP_GEN_ONOFF_STATUS		BT_MESH_MODEL_OP_2(0x82, 0x04)
#define OP_GEN_LEVEL_GET		BT_MESH_MODEL_OP_2(0x82, 0x05)
#define OP_GEN_LEVEL_SET		BT_MESH_MODEL_OP_2(0x82, 0x06)
#define OP_GEN_LEVEL_SET_UNACK		BT_MESH_MODEL_OP_2(0x82, 0x07)
#define OP_GEN_LEVEL_STATUS		BT_MESH_MODEL_OP_2(0x82, 0x08)
#define OP_GEN_DELTA_SET		BT_MESH_MODEL_OP_2(0x82, 0x09)
#define OP_GEN_DELTA_SET_UNACK		BT_MESH_MODEL_OP_2(0x82, 0x0a)
#define OP_GEN_MOVE_SET			BT_MESH_MODEL_OP_2(0x82, 0x0b)
#define OP_GEN_MOVE_SET_UNACK		BT_MESH_MODEL_OP_2(0x82, 0x0c)
#define OP_LIGHT_LIGHTNESS_GET		BT_MESH_MODEL_OP_2(0x82, 0x4b)
#define OP_LIGHT_LIGHTNESS_SET		BT_MESH_MODEL_OP_2(0x82, 0x4c)
#define OP_LIGHT_LIGHTNESS_SET_UNACK	BT_MESH_MODEL_OP_2(0x82, 0x4d)
#define OP_LIGHT_LIGHTNESS_STATUS	BT_MESH_MODEL_OP_2(0x82, 0x4e)

bool bt_mesh_is_provisioned(void);

#endif
