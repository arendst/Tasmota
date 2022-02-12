/*  Bluetooth Mesh */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __PROV_H__
#define __PROV_H__

#include "nimble/porting/nimble/include/os/os_mbuf.h"
#include "../include/mesh/mesh.h"
#include "nimble/nimble/host/src/ble_hs_conn_priv.h"

int bt_mesh_pb_adv_open(const u8_t uuid[16], u16_t net_idx, u16_t addr,
			u8_t attention_duration);

void bt_mesh_pb_adv_recv(struct os_mbuf *buf);

bool bt_prov_active(void);

int bt_mesh_pb_gatt_open(uint16_t conn_handle);
int bt_mesh_pb_gatt_close(uint16_t conn_handle);
int bt_mesh_pb_gatt_recv(uint16_t conn_handle, struct os_mbuf *buf);

const struct bt_mesh_prov *bt_mesh_prov_get(void);

int bt_mesh_prov_init(const struct bt_mesh_prov *prov);

void bt_mesh_prov_reset_link(void);

void bt_mesh_prov_complete(u16_t net_idx, u16_t addr);
void bt_mesh_prov_node_added(u16_t net_idx, u16_t addr, u8_t num_elem);
void bt_mesh_prov_reset(void);

#endif
