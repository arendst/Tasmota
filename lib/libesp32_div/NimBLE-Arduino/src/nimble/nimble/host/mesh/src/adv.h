/*  Bluetooth Mesh */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __ADV_H__
#define __ADV_H__

/* Maximum advertising data payload for a single data type */
#include "../include/mesh/mesh.h"

#define BT_MESH_ADV(om) (*(struct bt_mesh_adv **) OS_MBUF_USRHDR(om))

#define BT_MESH_ADV_DATA_SIZE 31

/* The user data is a pointer (4 bytes) to struct bt_mesh_adv */
#define BT_MESH_ADV_USER_DATA_SIZE (sizeof(struct bt_mesh_adv *))

#define BT_MESH_MBUF_HEADER_SIZE (sizeof(struct os_mbuf_pkthdr) + \
                                    BT_MESH_ADV_USER_DATA_SIZE +\
				    sizeof(struct os_mbuf))

enum bt_mesh_adv_type
{
	BT_MESH_ADV_PROV,
	BT_MESH_ADV_DATA,
	BT_MESH_ADV_BEACON,
	BT_MESH_ADV_URI,
};

typedef void (*bt_mesh_adv_func_t)(struct os_mbuf *buf, u16_t duration,
				   int err, void *user_data);

struct bt_mesh_adv {
	const struct bt_mesh_send_cb *cb;
	void *cb_data;

	u8_t      type:2,
		  busy:1;
	u8_t      xmit;

	/* For transport layer segment sending */
	struct {
		u8_t attempts;
	} seg;

	u8_t flags;

	int ref_cnt;
	struct ble_npl_event ev;
};

typedef struct bt_mesh_adv *(*bt_mesh_adv_alloc_t)(int id);

/* xmit_count: Number of retransmissions, i.e. 0 == 1 transmission */
struct os_mbuf *bt_mesh_adv_create(enum bt_mesh_adv_type type, u8_t xmit,
				   s32_t timeout);

struct os_mbuf *bt_mesh_adv_create_from_pool(struct os_mbuf_pool *pool,
					     bt_mesh_adv_alloc_t get_id,
					     enum bt_mesh_adv_type type,
					     u8_t xmit, s32_t timeout);

void bt_mesh_adv_send(struct os_mbuf *buf, const struct bt_mesh_send_cb *cb,
		      void *cb_data);

void bt_mesh_adv_update(void);

void bt_mesh_adv_init(void);

int bt_mesh_scan_enable(void);

int bt_mesh_scan_disable(void);

int ble_adv_gap_mesh_cb(struct ble_gap_event *event, void *arg);
#endif
