/*  Bluetooth Mesh */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#include "../include/mesh/mesh.h"

#define TRANS_SEQ_AUTH_NVAL 0xffffffffffffffff

#define BT_MESH_TX_SDU_MAX             (CONFIG_BT_MESH_TX_SEG_MAX * 12)

#define TRANS_SEQ_ZERO_MASK            ((u16_t)BIT_MASK(13))
#define TRANS_CTL_OP_MASK              ((u8_t)BIT_MASK(7))
#define TRANS_CTL_OP(data)             ((data)[0] & TRANS_CTL_OP_MASK)
#define TRANS_CTL_HDR(op, seg)         ((op & TRANS_CTL_OP_MASK) | (seg << 7))

#define TRANS_CTL_OP_ACK               0x00
#define TRANS_CTL_OP_FRIEND_POLL       0x01
#define TRANS_CTL_OP_FRIEND_UPDATE     0x02
#define TRANS_CTL_OP_FRIEND_REQ        0x03
#define TRANS_CTL_OP_FRIEND_OFFER      0x04
#define TRANS_CTL_OP_FRIEND_CLEAR      0x05
#define TRANS_CTL_OP_FRIEND_CLEAR_CFM  0x06
#define TRANS_CTL_OP_FRIEND_SUB_ADD    0x07
#define TRANS_CTL_OP_FRIEND_SUB_REM    0x08
#define TRANS_CTL_OP_FRIEND_SUB_CFM    0x09
#define TRANS_CTL_OP_HEARTBEAT         0x0a

struct bt_mesh_ctl_friend_poll {
	u8_t  fsn;
}__attribute__((__packed__));

struct bt_mesh_ctl_friend_update {
	u8_t  flags;
	u32_t iv_index;
	u8_t  md;
}__attribute__((__packed__));

struct bt_mesh_ctl_friend_req {
	u8_t  criteria;
	u8_t  recv_delay;
	u8_t  poll_to[3];
	u16_t prev_addr;
	u8_t  num_elem;
	u16_t lpn_counter;
}__attribute__((__packed__));

struct bt_mesh_ctl_friend_offer {
	u8_t  recv_win;
	u8_t  queue_size;
	u8_t  sub_list_size;
	s8_t  rssi;
	u16_t frnd_counter;
}__attribute__((__packed__));

struct bt_mesh_ctl_friend_clear {
	u16_t lpn_addr;
	u16_t lpn_counter;
}__attribute__((__packed__));

struct bt_mesh_ctl_friend_clear_confirm {
	u16_t lpn_addr;
	u16_t lpn_counter;
}__attribute__((__packed__));

#define BT_MESH_FRIEND_SUB_MIN_LEN (1 + 2)
struct bt_mesh_ctl_friend_sub {
	u8_t  xact;
	u16_t addr_list[5];
}__attribute__((__packed__));

struct bt_mesh_ctl_friend_sub_confirm {
	u8_t  xact;
}__attribute__((__packed__));

void bt_mesh_set_hb_sub_dst(u16_t addr);

struct bt_mesh_app_key *bt_mesh_app_key_find(u16_t app_idx);

bool bt_mesh_tx_in_progress(void);

void bt_mesh_rx_reset(void);
void bt_mesh_tx_reset(void);

int bt_mesh_ctl_send(struct bt_mesh_net_tx *tx, u8_t ctl_op, void *data,
		     size_t data_len, u64_t *seq_auth,
		     const struct bt_mesh_send_cb *cb, void *cb_data);

int bt_mesh_trans_send(struct bt_mesh_net_tx *tx, struct os_mbuf *msg,
		       const struct bt_mesh_send_cb *cb, void *cb_data);

int bt_mesh_trans_recv(struct os_mbuf *buf, struct bt_mesh_net_rx *rx);

void bt_mesh_trans_init(void);

void bt_mesh_rpl_clear(void);

void bt_mesh_heartbeat_send(void);

int bt_mesh_app_key_get(const struct bt_mesh_subnet *subnet, u16_t app_idx,
			u16_t addr, const u8_t **key, u8_t *aid);
