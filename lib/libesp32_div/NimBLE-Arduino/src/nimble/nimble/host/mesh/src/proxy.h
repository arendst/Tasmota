/*  Bluetooth Mesh */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __PROXY_H__
#define __PROXY_H__

#define BT_MESH_PROXY_NET_PDU   0x00
#define BT_MESH_PROXY_BEACON    0x01
#define BT_MESH_PROXY_CONFIG    0x02
#define BT_MESH_PROXY_PROV      0x03

#include "../include/mesh/mesh.h"
#include "../include/mesh/slist.h"

struct bt_mesh_proxy_idle_cb {
	sys_snode_t n;
	void (*cb)(void);
};

int bt_mesh_proxy_send(uint16_t conn_handle, uint8_t type, struct os_mbuf *msg);

int bt_mesh_proxy_prov_enable(void);
int bt_mesh_proxy_prov_disable(bool disconnect);

int bt_mesh_proxy_gatt_enable(void);
int bt_mesh_proxy_gatt_disable(void);
void bt_mesh_proxy_gatt_disconnect(void);

void bt_mesh_proxy_beacon_send(struct bt_mesh_subnet *sub);

struct os_mbuf *bt_mesh_proxy_get_buf(void);

int32_t bt_mesh_proxy_adv_start(void);
void bt_mesh_proxy_adv_stop(void);

void bt_mesh_proxy_identity_start(struct bt_mesh_subnet *sub);
void bt_mesh_proxy_identity_stop(struct bt_mesh_subnet *sub);

bool bt_mesh_proxy_relay(struct os_mbuf *buf, uint16_t dst);
void bt_mesh_proxy_addr_add(struct os_mbuf *buf, uint16_t addr);

int bt_mesh_proxy_init(void);
void bt_mesh_proxy_on_idle(struct bt_mesh_proxy_idle_cb *cb);

int ble_mesh_proxy_gap_event(struct ble_gap_event *event, void *arg);

#endif
