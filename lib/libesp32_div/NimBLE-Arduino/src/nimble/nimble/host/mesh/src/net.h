/*  Bluetooth Mesh */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __NET_H__
#define __NET_H__

#define BT_MESH_NET_FLAG_KR       BIT(0)
#define BT_MESH_NET_FLAG_IVU      BIT(1)

#define BT_MESH_KR_NORMAL         0x00
#define BT_MESH_KR_PHASE_1        0x01
#define BT_MESH_KR_PHASE_2        0x02
#define BT_MESH_KR_PHASE_3        0x03

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

struct bt_mesh_app_key {
	u16_t net_idx;
	u16_t app_idx;
	bool  updated;
	struct bt_mesh_app_keys {
		u8_t id;
		u8_t val[16];
	} keys[2];
};

struct bt_mesh_node {
	u16_t addr;
	u16_t net_idx;
	u8_t  dev_key[16];
	u8_t  num_elem;
};

struct bt_mesh_subnet {
	u32_t beacon_sent;        /* Timestamp of last sent beacon */
	u8_t  beacons_last;       /* Number of beacons during last
				   * observation window
				   */
	u8_t  beacons_cur;        /* Number of beaconds observed during
				   * currently ongoing window.
				   */

	u8_t  beacon_cache[21];   /* Cached last authenticated beacon */

	u16_t net_idx;            /* NetKeyIndex */

	bool  kr_flag;            /* Key Refresh Flag */
	u8_t  kr_phase;           /* Key Refresh Phase */

	u8_t  node_id;            /* Node Identity State */
	u32_t node_id_start;      /* Node Identity started timestamp */

	u8_t  auth[8];            /* Beacon Authentication Value */

	struct bt_mesh_subnet_keys {
		u8_t net[16];       /* NetKey */
		u8_t nid;           /* NID */
		u8_t enc[16];       /* EncKey */
		u8_t net_id[8];     /* Network ID */
#if (MYNEWT_VAL(BLE_MESH_GATT_PROXY))
		u8_t identity[16];  /* IdentityKey */
#endif
		u8_t privacy[16];   /* PrivacyKey */
		u8_t beacon[16];    /* BeaconKey */
	} keys[2];
};

struct bt_mesh_rpl {
	u16_t src;
	bool  old_iv;
#if (MYNEWT_VAL(BLE_MESH_SETTINGS))
	bool  store;
#endif
	u32_t seq;
};

#if MYNEWT_VAL(BLE_MESH_FRIEND)
#define FRIEND_SEG_RX MYNEWT_VAL(BLE_MESH_FRIEND_SEG_RX)
#define FRIEND_SUB_LIST_SIZE MYNEWT_VAL(BLE_MESH_FRIEND_SUB_LIST_SIZE)
#else
#define FRIEND_SEG_RX 0
#define FRIEND_SUB_LIST_SIZE 0
#endif

struct bt_mesh_friend {
	u16_t lpn;
	u8_t  recv_delay;
	u8_t  fsn:1,
	      send_last:1,
	      pending_req:1,
	      sec_update:1,
	      pending_buf:1,
	      valid:1,
	      established:1;
	s32_t poll_to;
	u8_t  num_elem;
	u16_t lpn_counter;
	u16_t counter;

	u16_t net_idx;

	u16_t sub_list[FRIEND_SUB_LIST_SIZE];

	struct k_delayed_work timer;

	struct bt_mesh_friend_seg {
		struct net_buf_slist_t queue;

		/* The target number of segments, i.e. not necessarily
		 * the current number of segments, in the queue. This is
		 * used for Friend Queue free space calculations.
		 */
		u8_t        seg_count;
	} seg[FRIEND_SEG_RX];

	struct os_mbuf *last;

	struct net_buf_slist_t queue;
	u32_t queue_size;

	/* Friend Clear Procedure */
	struct {
		u32_t start;                  /* Clear Procedure start */
		u16_t frnd;                   /* Previous Friend's address */
		u16_t repeat_sec;             /* Repeat timeout in seconds */
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
	u8_t xact_next;
	u8_t xact_pending;
	u8_t sent_req;

	/* Address of our Friend when we're a LPN. Unassigned if we don't
	 * have a friend yet.
	 */
	u16_t frnd;

	/* Value from the friend offer */
	u8_t  recv_win;

	u8_t  req_attempts;     /* Number of Request attempts */

	s32_t poll_timeout;

	u8_t  groups_changed:1, /* Friend Subscription List needs updating */
	      pending_poll:1,   /* Poll to be sent after subscription */
	      disable:1,        /* Disable LPN after clearing */
	      fsn:1,            /* Friend Sequence Number */
	      established:1,    /* Friendship established */
	      clear_success:1;  /* Friend Clear Confirm received */

	/* Friend Queue Size */
	u8_t  queue_size;

	/* LPNCounter */
	u16_t counter;

	/* Previous Friend of this LPN */
	u16_t old_friend;

	/* Duration reported for last advertising packet */
	u16_t adv_duration;

	/* Next LPN related action timer */
	struct k_delayed_work timer;

	/* Subscribed groups */
	u16_t groups[LPN_GROUPS];

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
	BT_MESH_NODES_PENDING,

	/* Don't touch - intentionally last */
	BT_MESH_FLAG_COUNT,
};

struct bt_mesh_net {
	u32_t iv_index; /* Current IV Index */
	u32_t seq;      /* Next outgoing sequence number (24 bits) */

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
	u8_t  ivu_duration;

	/* Timer to track duration in current IV Update state */
	struct k_delayed_work ivu_timer;

	u8_t dev_key[16];

#if MYNEWT_VAL(BLE_MESH_PROVISIONER)
	struct bt_mesh_node nodes[MYNEWT_VAL(BLE_MESH_NODE_COUNT)];
#endif

	struct bt_mesh_app_key app_keys[MYNEWT_VAL(BLE_MESH_APP_KEY_COUNT)];

	struct bt_mesh_subnet sub[MYNEWT_VAL(BLE_MESH_SUBNET_COUNT)];

	struct bt_mesh_rpl rpl[MYNEWT_VAL(BLE_MESH_CRPL)];
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
	u32_t  seq;            /* Sequence Number */
	u8_t   old_iv:1,       /* iv_index - 1 was used */
	       new_key:1,      /* Data was encrypted with updated key */
	       friend_cred:1,  /* Data was encrypted with friend cred */
	       ctl:1,          /* Network Control */
	       net_if:2,       /* Network interface */
	       local_match:1,  /* Matched a local element */
	       friend_match:1; /* Matched an LPN we're friends for */
	u16_t  msg_cache_idx;  /* Index of entry in message cache */
};

/* Encoding context for Network/Transport data */
struct bt_mesh_net_tx {
	struct bt_mesh_subnet *sub;
	struct bt_mesh_msg_ctx *ctx;
	u16_t src;
	u8_t  xmit;
	u8_t  friend_cred:1,
	      aszmic:1,
	      aid:6;
};

extern struct bt_mesh_net bt_mesh;

#define BT_MESH_NET_IVI_TX (bt_mesh.iv_index - \
			    atomic_test_bit(bt_mesh.flags, \
					    BT_MESH_IVU_IN_PROGRESS))
#define BT_MESH_NET_IVI_RX(rx) (bt_mesh.iv_index - (rx)->old_iv)

#define BT_MESH_NET_HDR_LEN 9

int bt_mesh_net_keys_create(struct bt_mesh_subnet_keys *keys,
			    const u8_t key[16]);

int bt_mesh_net_create(u16_t idx, u8_t flags, const u8_t key[16],
		       u32_t iv_index);

u8_t bt_mesh_net_flags(struct bt_mesh_subnet *sub);

bool bt_mesh_kr_update(struct bt_mesh_subnet *sub, u8_t new_kr, bool new_key);

void bt_mesh_net_revoke_keys(struct bt_mesh_subnet *sub);

int bt_mesh_net_beacon_update(struct bt_mesh_subnet *sub);

void bt_mesh_rpl_reset(void);

bool bt_mesh_net_iv_update(u32_t iv_index, bool iv_update);

void bt_mesh_net_sec_update(struct bt_mesh_subnet *sub);

struct bt_mesh_subnet *bt_mesh_subnet_get(u16_t net_idx);

struct bt_mesh_subnet *bt_mesh_subnet_find(const u8_t net_id[8], u8_t flags,
					   u32_t iv_index, const u8_t auth[8],
					   bool *new_key);

int bt_mesh_net_encode(struct bt_mesh_net_tx *tx, struct os_mbuf *buf,
		       bool proxy);

int bt_mesh_net_send(struct bt_mesh_net_tx *tx, struct os_mbuf *buf,
		     const struct bt_mesh_send_cb *cb, void *cb_data);

int bt_mesh_net_resend(struct bt_mesh_subnet *sub, struct os_mbuf *buf,
		       bool new_key, const struct bt_mesh_send_cb *cb,
		       void *cb_data);

int bt_mesh_net_decode(struct os_mbuf *data, enum bt_mesh_net_if net_if,
		       struct bt_mesh_net_rx *rx, struct os_mbuf *buf);

void bt_mesh_net_recv(struct os_mbuf *data, s8_t rssi,
		      enum bt_mesh_net_if net_if);

u32_t bt_mesh_next_seq(void);

void bt_mesh_net_start(void);

void bt_mesh_net_init(void);
void bt_mesh_net_header_parse(struct os_mbuf *buf,
			      struct bt_mesh_net_rx *rx);

/* Friendship Credential Management */
struct friend_cred {
	u16_t net_idx;
	u16_t addr;

	u16_t lpn_counter;
	u16_t frnd_counter;

	struct {
		u8_t nid;         /* NID */
		u8_t enc[16];     /* EncKey */
		u8_t privacy[16]; /* PrivacyKey */
	} cred[2];
};

int friend_cred_get(struct bt_mesh_subnet *sub, u16_t addr, u8_t *nid,
			    const u8_t **enc, const u8_t **priv);
int friend_cred_set(struct friend_cred *cred, u8_t idx, const u8_t net_key[16]);
void friend_cred_refresh(u16_t net_idx);
int friend_cred_update(struct bt_mesh_subnet *sub);
struct friend_cred *friend_cred_create(struct bt_mesh_subnet *sub, u16_t addr,
				       u16_t lpn_counter, u16_t frnd_counter);
void friend_cred_clear(struct friend_cred *cred);
int friend_cred_del(u16_t net_idx, u16_t addr);

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
