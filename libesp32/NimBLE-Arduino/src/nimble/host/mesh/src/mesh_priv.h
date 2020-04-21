/*  Bluetooth Mesh */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __MESH_PRIV_H
#define __MESH_PRIV_H

#define BT_MESH_KEY_PRIMARY 0x0000
#define BT_MESH_KEY_ANY     0xffff

#define BT_MESH_ADDR_IS_UNICAST(addr) ((addr) && (addr) < 0x8000)
#define BT_MESH_ADDR_IS_GROUP(addr) ((addr) >= 0xc000 && (addr) <= 0xff00)
#define BT_MESH_ADDR_IS_VIRTUAL(addr) ((addr) >= 0x8000 && (addr) < 0xc000)
#define BT_MESH_ADDR_IS_RFU(addr) ((addr) >= 0xff00 && (addr) <= 0xfffb)
struct bt_mesh_net;

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

bool bt_mesh_is_provisioned(void);

#endif
