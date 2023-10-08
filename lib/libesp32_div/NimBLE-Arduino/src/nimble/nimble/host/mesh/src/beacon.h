/*  Bluetooth Mesh */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __BEACON_H__
#define __BEACON_H__

#include "nimble/porting/nimble/include/os/os_mbuf.h"

void bt_mesh_beacon_enable(void);
void bt_mesh_beacon_disable(void);

void bt_mesh_beacon_ivu_initiator(bool enable);

void bt_mesh_beacon_recv(struct os_mbuf *buf);

void bt_mesh_beacon_create(struct bt_mesh_subnet *sub,
			   struct os_mbuf *buf);

void bt_mesh_beacon_init(void);
void bt_mesh_beacon_update(struct bt_mesh_subnet *sub);

#endif
