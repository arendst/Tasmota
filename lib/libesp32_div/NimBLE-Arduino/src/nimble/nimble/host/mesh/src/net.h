/*  Bluetooth Mesh */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __NET_H__
#define __NET_H__

#include "subnet.h"

#define BT_MESH_IV_UPDATE(flags)   ((flags >> 1) & 0x01)
#define BT_MESH_KEY_REFRESH(flags) (flags & 0x01)

#include <stdbool.h>
#include "atomic.h"
#include "../include/mesh/mesh.h"
#include "../include/mesh/glue.h"

/* How many hours in between updating IVU duration */
#define BT_MESH_IVU_MIN_HOURS      96
#define BT_MESH_IVU_HOURS          (BT_MESH_IVU_MIN_HOURS /     \
				    CONFIG_BT_MESH_IVU_DIVIDER)
#define BT_MESH_IVU_TIMEOUT        K_HOURS(BT_MESH_IVU_HOURS)

struct bt_mesh_net_cred;

struct bt_mesh_node {
	uint16_t addr;
	uint16_t net_idx;
	uint8_t  dev_key[16];
	uint8_t  num_elem;
};

#if MYNEWT_VAL(BLE_MESH_FRIEND)
#define FRIEND_SEG_RX MYNEWT_VAL(BLE_MESH_FRIEND_SEG_RX)
#define FRIEND_SUB_LIST_SIZE MYNEWT_VAL(BLE_MESH_FRIEND_SUB_LIST_SIZE)
#else
#define FRIEND_SEG_RX 0
#define FRIEND_SUB_LIST_SIZE 0
#endif

struct bt_mesh_friend {
	uint16_t lpn;
	uint8_t  recv_delay;
	uint8_t  fsn:1,
	      send_last:1,
	      pending_req:1,
	      pending_buf:1,
	      established:1;
	int32_t poll_to;
	uint8_t  num_elem;
	uint16_t lpn_counter;
	uint16_t counter;

	struct bt_mesh_subnet *subnet;

	struct bt_mesh_net_cred cred[2];

	uint16_t sub_list[FRIEND_SUB_LIST_SIZE];

	struct k_delayed_work timer;

	struct bt_mesh_friend_seg {
		struct net_buf_slist_t queue;

		/* The target number of segments, i.e. not necessarily
		 * the current number of segments, in the queue. This is
		 * used for Friend Queue free space calculations.
		 */
		uint8_t        seg_count;
	} seg[FRIEND_SEG_RX];

	struct os_mbuf *last;

	struct net_buf_slist_t queue;
	uint32_t queue_size;

	/* Friend Clear Procedure */
	struct {
		uint32_t start;                  /* Clear Procedure start */
		uint16_t frnd;                   /* Previous Friend's address */
		uint16_t repeat_sec;             /* Repeat timeout in seconds */
		struct k_delayed_work timer;  /* Repeat timer */
	} clear;
};

#if (MYNEWT_VAL(BLE_MESH_LOW_POWER))
#define LPN_GROUPS CONFIG_BT_MESH_LPN_GROUPS
#else
#define LPN_GROUPS 0
#endif

/* Low Power Node state */
struct bt_mesh_lpn {
	enum __packed {
		BT_MESH_LPN_DISABLED,     /* LPN feature is disabled */
		BT_MESH_LPN_CLEAR,        /* Clear in progress */
		BT_MESH_LPN_TIMER,        /* Waiting for auto timer expiry */
		BT_MESH_LPN_ENABLED,      /* LPN enabled, but no Friend */
		BT_MESH_LPN_REQ_WAIT,     /* Wait before scanning for offers */
		BT_MESH_LPN_WAIT_OFFER,   /* Friend Req sent */
		BT_MESH_LPN_ESTABLISHED,  /* Friendship established */
		BT_MESH_LPN_RECV_DELAY,   /* Poll sent, waiting ReceiveDelay */
		BT_MESH_LPN_WAIT_UPDATE,  /* Waiting for Update or message */
	} state;

	/* Transaction Number (used for subscription list) */
	uint8_t xact_next;
	uint8_t xact_pending;
	uint8_t sent_req;

	/* Address of our Friend when we're a LPN. Unassigned if we don't
	 * have a friend yet.
	 */
	uint16_t frnd;

	/* Value from the friend offer */
	uint8_t  recv_win;

	uint8_t  req_attempts;     /* Number of Request attempts */

	int32_t poll_timeout;

	uint8_t  groups_changed:1, /* Friend Subscription List needs updating */
	      pending_poll:1,   /* Poll to be sent after subscription */
	      disable:1,        /* Disable LPN after clearing */
	      fsn:1,            /* Friend Sequence Number */
	      established:1,    /* Friendship established */
	      clear_success:1;  /* Friend Clear Confirm received */

	/* Friend Queue Size */
	uint8_t  queue_size;

	/* FriendCounter */
	uint16_t frnd_counter;

	/* LPNCounter */
	uint16_t lpn_counter;

	/* Previous Friend of this LPN */
	uint16_t old_friend;

	/* Duration reported for last advertising packet */
	uint16_t adv_duration;

	/* Next LPN related action timer */
	struct k_delayed_work timer;

	/* Subscribed groups */
	uint16_t groups[LPN_GROUPS];

	struct bt_mesh_subnet *sub;

	struct bt_mesh_net_cred cred[2];

	/* Bit fields for tracking which groups the Friend knows about */
	ATOMIC_DEFINE(added, LPN_GROUPS);
	ATOMIC_DEFINE(pending, LPN_GROUPS);
	ATOMIC_DEFINE(to_remove, LPN_GROUPS);
};

/* bt_mesh_net.flags */
enum {
	BT_MESH_VALID,           /* We have been provisioned */
	BT_MESH_SUSPENDED,       /* Network is temporarily suspended */
	BT_MESH_IVU_IN_PROGRESS, /* IV Update in Progress */
	BT_MESH_IVU_INITIATOR,   /* IV Update initiated by us */
	BT_MESH_IVU_TEST,        /* IV Update test mode */
	BT_MESH_IVU_PENDING,     /* Update blocked by SDU in progress */

	/* pending storage actions, must reside within first 32 flags */
	BT_MESH_RPL_PENDING,
	BT_MESH_KEYS_PENDING,
	BT_MESH_NET_PENDING,
	BT_MESH_IV_PENDING,
	BT_MESH_SEQ_PENDING,
	BT_MESH_HB_PUB_PENDING,
	BT_MESH_CFG_PENDING,
	BT_MESH_MOD_PENDING,
	BT_MESH_VA_PENDING,

	/* Feature flags */
	BT_MESH_RELAY,
	BT_MESH_BEACON,
	BT_MESH_GATT_PROXY,
	BT_MESH_FRIEND,

	/* Don't touch - intentionally last */
	BT_MESH_FLAG_COUNT,
};

struct bt_mesh_net {
	uint32_t iv_index; /* Current IV Index */
	uint32_t seq;      /* Next outgoing sequence number (24 bits) */

	ATOMIC_DEFINE(flags, BT_MESH_FLAG_COUNT);

	/* Local network interface */
	struct ble_npl_callout local_work;
	struct net_buf_slist_t local_queue;

#if MYNEWT_VAL(BLE_MESH_FRIEND)
	/* Friend state, unique for each LPN that we're Friends for */
	struct bt_mesh_friend frnd[MYNEWT_VAL(BLE_MESH_FRIEND_LPN_COUNT)];
#endif

#if (MYNEWT_VAL(BLE_MESH_LOW_POWER))
	struct bt_mesh_lpn lpn;  /* Low Power Node state */
#endif

	/* Number of hours in current IV Update state */
	uint8_t  ivu_duration;

	uint8_t net_xmit;
	uint8_t relay_xmit;
	uint8_t default_ttl;

	/* Timer to track duration in current IV Update state */
	struct k_delayed_work ivu_timer;

	uint8_t dev_key[16];
};

/* Network interface */
enum bt_mesh_net_if {
	BT_MESH_NET_IF_ADV,
	BT_MESH_NET_IF_LOCAL,
	BT_MESH_NET_IF_PROXY,
	BT_MESH_NET_IF_PROXY_CFG,
};

/* Decoding context for Network/Transport data */
struct bt_mesh_net_rx {
	struct bt_mesh_subnet *sub;
	struct bt_mesh_msg_ctx ctx;
	uint32_t  seq;            /* Sequence Number */
	uint8_t   old_iv:1,       /* iv_index - 1 was used */
	       new_key:1,      /* Data was encrypted with updated key */
	       friend_cred:1,  /* Data was encrypted with friend cred */
	       ctl:1,          /* Network Control */
	       net_if:2,       /* Network interface */
	       local_match:1,  /* Matched a local element */
	       friend_match:1; /* Matched an LPN we're friends for */
	uint16_t  msg_cache_idx;  /* Index of entry in message cache */
};

/* Encoding context for Network/Transport data */
struct bt_mesh_net_tx {
	struct bt_mesh_subnet *sub;
	struct bt_mesh_msg_ctx *ctx;
	uint16_t src;
	uint8_t  xmit;
	uint8_t  friend_cred:1,
	      aszmic:1,
	      aid:6;
};

extern struct bt_mesh_net bt_mesh;

#define BT_MESH_NET_IVI_TX (bt_mesh.iv_index - \
			    atomic_test_bit(bt_mesh.flags, \
					    BT_MESH_IVU_IN_PROGRESS))
#define BT_MESH_NET_IVI_RX(rx) (bt_mesh.iv_index - (rx)->old_iv)

#define BT_MESH_NET_HDR_LEN 9

static inline void *net_buf_user_data(const struct os_mbuf *buf)
{
	return (void *)buf->om_data;
}

int bt_mesh_net_create(uint16_t idx, uint8_t flags, const uint8_t key[16],
		       uint32_t iv_index);

bool bt_mesh_net_iv_update(uint32_t iv_index, bool iv_update);

int bt_mesh_net_encode(struct bt_mesh_net_tx *tx, struct os_mbuf *buf,
		       bool proxy);

int bt_mesh_net_decode(struct os_mbuf *in, enum bt_mesh_net_if net_if,
		       struct bt_mesh_net_rx *rx, struct os_mbuf *out);

int bt_mesh_net_send(struct bt_mesh_net_tx *tx, struct os_mbuf *buf,
		     const struct bt_mesh_send_cb *cb, void *cb_data);

void bt_mesh_net_recv(struct os_mbuf *data, int8_t rssi,
		      enum bt_mesh_net_if net_if);

void bt_mesh_net_loopback_clear(uint16_t net_idx);

uint32_t bt_mesh_next_seq(void);

void bt_mesh_net_init(void);
void bt_mesh_net_header_parse(struct os_mbuf *buf,
			      struct bt_mesh_net_rx *rx);


static inline void send_cb_finalize(const struct bt_mesh_send_cb *cb,
				    void *cb_data)
{
	if (!cb) {
		return;
	}

	if (cb->start) {
		cb->start(0, 0, cb_data);
	}

	if (cb->end) {
		cb->end(0, cb_data);
	}
}

#endif
