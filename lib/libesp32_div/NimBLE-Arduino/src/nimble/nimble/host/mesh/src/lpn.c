/*  Bluetooth Mesh */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#if MYNEWT_VAL(BLE_MESH)

#define MESH_LOG_MODULE BLE_MESH_LOW_POWER_LOG

#if MYNEWT_VAL(BLE_MESH_LOW_POWER)

#include <stdint.h>

#include "../include/mesh/mesh.h"
#include "mesh_priv.h"
#include "crypto.h"
#include "adv.h"
#include "net.h"
#include "transport.h"
#include "heartbeat.h"
#include "access.h"
#include "beacon.h"
#include "foundation.h"
#include "lpn.h"

#if MYNEWT_VAL(BLE_MESH_LPN_AUTO)
#define LPN_AUTO_TIMEOUT          K_SECONDS(MYNEWT_VAL(BLE_MESH_LPN_AUTO_TIMEOUT))
#else
#define LPN_AUTO_TIMEOUT          0
#endif

#define LPN_RECV_DELAY            MYNEWT_VAL(BLE_MESH_LPN_RECV_DELAY)
#define SCAN_LATENCY              min(MYNEWT_VAL(BLE_MESH_LPN_SCAN_LATENCY), \
				      LPN_RECV_DELAY)

#define FRIEND_REQ_RETRY_TIMEOUT  K_SECONDS(MYNEWT_VAL(BLE_MESH_LPN_RETRY_TIMEOUT))

#define FRIEND_REQ_WAIT           K_MSEC(100)
#define FRIEND_REQ_SCAN           K_SECONDS(1)
#define FRIEND_REQ_TIMEOUT        (FRIEND_REQ_WAIT + FRIEND_REQ_SCAN)

#define POLL_RETRY_TIMEOUT        K_MSEC(100)

#define REQ_RETRY_DURATION(lpn)  (LPN_RECV_DELAY + (lpn)->adv_duration + \
				       (lpn)->recv_win + POLL_RETRY_TIMEOUT)

#define POLL_TIMEOUT_INIT     (MYNEWT_VAL(BLE_MESH_LPN_INIT_POLL_TIMEOUT) * 100)

#define POLL_TIMEOUT              (MYNEWT_VAL(BLE_MESH_LPN_RECV_DELAY) * 100)

#define REQ_ATTEMPTS_MAX      6
#define REQ_ATTEMPTS(lpn)     MIN(REQ_ATTEMPTS_MAX, \
			  				  POLL_TIMEOUT / REQ_RETRY_DURATION(lpn))

#define POLL_TIMEOUT_MAX(lpn) (POLL_TIMEOUT - \
			  					  (REQ_ATTEMPTS(lpn) * REQ_RETRY_DURATION(lpn)))

#define CLEAR_ATTEMPTS        3

#define LPN_CRITERIA ((MYNEWT_VAL(BLE_MESH_LPN_MIN_QUEUE_SIZE)) | \
		      (MYNEWT_VAL(BLE_MESH_LPN_RSSI_FACTOR) << 3) | \
		      (MYNEWT_VAL(BLE_MESH_LPN_RECV_WIN_FACTOR) << 5))

#define POLL_TO(to) { (uint8_t)((to) >> 16), (uint8_t)((to) >> 8), (uint8_t)(to) }
#define LPN_POLL_TO POLL_TO(MYNEWT_VAL(BLE_MESH_LPN_POLL_TIMEOUT))

/* 2 transmissions, 20ms interval */
#define POLL_XMIT BT_MESH_TRANSMIT(1, 20)

static void (*lpn_cb)(uint16_t friend_addr, bool established);

#if MYNEWT_VAL(BLE_MESH_LOW_POWER_LOG_LVL) == LOG_LEVEL_DEBUG
static const char *state2str(int state)
{
	switch (state) {
	case BT_MESH_LPN_DISABLED:
		return "disabled";
	case BT_MESH_LPN_CLEAR:
		return "clear";
	case BT_MESH_LPN_TIMER:
		return "timer";
	case BT_MESH_LPN_ENABLED:
		return "enabled";
	case BT_MESH_LPN_REQ_WAIT:
		return "req wait";
	case BT_MESH_LPN_WAIT_OFFER:
		return "wait offer";
	case BT_MESH_LPN_ESTABLISHED:
		return "established";
	case BT_MESH_LPN_RECV_DELAY:
		return "recv delay";
	case BT_MESH_LPN_WAIT_UPDATE:
		return "wait update";
	default:
		return "(unknown)";
	}
}
#endif

static inline void lpn_set_state(int state)
{
#if MYNEWT_VAL(BLE_MESH_LOW_POWER_LOG_LVL) == LOG_LEVEL_DEBUG
	BT_DBG("%s -> %s", state2str(bt_mesh.lpn.state), state2str(state));
#endif
	bt_mesh.lpn.state = state;
}

static inline void group_zero(atomic_t *target)
{
#if CONFIG_BT_MESH_LPN_GROUPS > 32
	int i;

	for (i = 0; i < ARRAY_SIZE(bt_mesh.lpn.added); i++) {
		atomic_set(&target[i], 0);
	}
#else
	atomic_set(target, 0);
#endif
}

static inline void group_set(atomic_t *target, atomic_t *source)
{
#if CONFIG_BT_MESH_LPN_GROUPS > 32
	int i;

	for (i = 0; i < ARRAY_SIZE(bt_mesh.lpn.added); i++) {
		atomic_or(&target[i], atomic_get(&source[i]));
	}
#else
	atomic_or(target, atomic_get(source));
#endif
}

static inline void group_clear(atomic_t *target, atomic_t *source)
{
#if CONFIG_BT_MESH_LPN_GROUPS > 32
	int i;

	for (i = 0; i < ARRAY_SIZE(bt_mesh.lpn.added); i++) {
		atomic_and(&target[i], ~atomic_get(&source[i]));
	}
#else
	atomic_and(target, ~atomic_get(source));
#endif
}

static void clear_friendship(bool force, bool disable);

static void friend_clear_sent(int err, void *user_data)
{
	struct bt_mesh_lpn *lpn = &bt_mesh.lpn;

	/* Scanning will enable if lpn state still enabled  */

	lpn->req_attempts++;

	if (err) {
		BT_ERR("Sending Friend Request failed (err %d)", err);
		lpn_set_state(BT_MESH_LPN_ENABLED);
		clear_friendship(false, lpn->disable);
		return;
	}

	lpn_set_state(BT_MESH_LPN_CLEAR);
	k_delayed_work_submit(&lpn->timer, FRIEND_REQ_TIMEOUT);
}

static const struct bt_mesh_send_cb clear_sent_cb = {
	.end = friend_clear_sent,
};

static int send_friend_clear(void)
{
	struct bt_mesh_msg_ctx ctx = {
		.net_idx     = bt_mesh.lpn.sub->net_idx,
		.app_idx     = BT_MESH_KEY_UNUSED,
		.addr        = bt_mesh.lpn.frnd,
		.send_ttl    = 0,
	};
	struct bt_mesh_net_tx tx = {
		.sub = bt_mesh.lpn.sub,
		.ctx = &ctx,
		.src = bt_mesh_primary_addr(),
		.xmit = bt_mesh_net_transmit_get(),
	};
	struct bt_mesh_ctl_friend_clear req = {
		.lpn_addr    = sys_cpu_to_be16(tx.src),
		.lpn_counter = sys_cpu_to_be16(bt_mesh.lpn.lpn_counter),
	};

	BT_DBG("");

	return bt_mesh_ctl_send(&tx, TRANS_CTL_OP_FRIEND_CLEAR, &req,
				sizeof(req), &clear_sent_cb, NULL);
}

static void clear_friendship(bool force, bool disable)
{
	struct bt_mesh_lpn *lpn = &bt_mesh.lpn;

	BT_DBG("force %u disable %u", force, disable);

	if (!force && lpn->established && !lpn->clear_success &&
	    lpn->req_attempts < CLEAR_ATTEMPTS) {
		send_friend_clear();
		lpn->disable = disable;
		return;
	}

	bt_mesh_rx_reset();

	k_delayed_work_cancel(&lpn->timer);

	if (lpn->clear_success) {
		lpn->old_friend = BT_MESH_ADDR_UNASSIGNED;
	} else {
		lpn->old_friend = lpn->frnd;
	}

	if (lpn_cb && lpn->frnd != BT_MESH_ADDR_UNASSIGNED) {
		lpn_cb(lpn->frnd, false);
	}

	lpn->frnd = BT_MESH_ADDR_UNASSIGNED;
	lpn->fsn = 0;
	lpn->req_attempts = 0;
	lpn->recv_win = 0;
	lpn->queue_size = 0;
	lpn->disable = 0;
	lpn->sent_req = 0;
	lpn->established = 0;
	lpn->clear_success = 0;
	lpn->sub = NULL;

	group_zero(lpn->added);
	group_zero(lpn->pending);
	group_zero(lpn->to_remove);

	/* Set this to 1 to force group subscription when the next
	 * Friendship is created, in case lpn->groups doesn't get
	 * modified meanwhile.
	 */
	lpn->groups_changed = 1;

	bt_mesh_hb_feature_changed(BT_MESH_FEAT_LOW_POWER);

	if (disable) {
		lpn_set_state(BT_MESH_LPN_DISABLED);
		return;
	}

	lpn_set_state(BT_MESH_LPN_ENABLED);
	k_delayed_work_submit(&lpn->timer, FRIEND_REQ_RETRY_TIMEOUT);
}

static void friend_req_sent(uint16_t duration, int err, void *user_data)
{
	struct bt_mesh_lpn *lpn = &bt_mesh.lpn;

	if (err) {
		BT_ERR("Sending Friend Request failed (err %d)", err);
		return;
	}

	lpn->adv_duration = duration;

	if (IS_ENABLED(CONFIG_BT_MESH_LPN_ESTABLISHMENT)) {
		k_delayed_work_submit(&lpn->timer, FRIEND_REQ_WAIT);
		lpn_set_state(BT_MESH_LPN_REQ_WAIT);
	} else {
		k_delayed_work_submit(&lpn->timer,
				      duration + FRIEND_REQ_TIMEOUT);
		lpn_set_state(BT_MESH_LPN_WAIT_OFFER);
	}
}

static const struct bt_mesh_send_cb friend_req_sent_cb = {
	.start = friend_req_sent,
};

static int send_friend_req(struct bt_mesh_lpn *lpn)
{
	const struct bt_mesh_comp *comp = bt_mesh_comp_get();
	struct bt_mesh_msg_ctx ctx = {
		.app_idx  = BT_MESH_KEY_UNUSED,
		.addr     = BT_MESH_ADDR_FRIENDS,
		.send_ttl = 0,
	};
	struct bt_mesh_net_tx tx = {
		.ctx = &ctx,
		.src = bt_mesh_primary_addr(),
		.xmit = POLL_XMIT,
	};

	lpn->lpn_counter++;

	struct bt_mesh_ctl_friend_req req = {
		.criteria    = LPN_CRITERIA,
		.recv_delay  = LPN_RECV_DELAY,
		.poll_to     = LPN_POLL_TO,
		.prev_addr   = sys_cpu_to_be16(lpn->old_friend),
		.num_elem    = comp->elem_count,
		.lpn_counter = sys_cpu_to_be16(lpn->lpn_counter),
	};

	BT_DBG("");

	lpn->sub = bt_mesh_subnet_next(NULL);
	if (!lpn->sub) {
		BT_ERR("No subnets, can't start LPN mode");
		return -ENOENT;
	}

	ctx.net_idx = lpn->sub->net_idx;
	tx.sub = lpn->sub;

	return bt_mesh_ctl_send(&tx, TRANS_CTL_OP_FRIEND_REQ, &req,
				sizeof(req), &friend_req_sent_cb, NULL);
}

static void req_sent(uint16_t duration, int err, void *user_data)
{
	struct bt_mesh_lpn *lpn = &bt_mesh.lpn;

#if MYNEWT_VAL(BLE_MESH_LOW_POWER_LOG_LVL) == LOG_LEVEL_DEBUG
	BT_DBG("req 0x%02x duration %u err %d state %s",
	       lpn->sent_req, duration, err, state2str(lpn->state));
#endif

	if (err) {
		BT_ERR("Sending request failed (err %d)", err);
		lpn->sent_req = 0;
		group_zero(lpn->pending);
		return;
	}

	lpn->req_attempts++;
	lpn->adv_duration = duration;

	if (lpn->established || IS_ENABLED(CONFIG_BT_MESH_LPN_ESTABLISHMENT)) {
		lpn_set_state(BT_MESH_LPN_RECV_DELAY);
		/* We start scanning a bit early to elimitate risk of missing
		 * response data due to HCI and other latencies.
		 */
		k_delayed_work_submit(&lpn->timer,
				      LPN_RECV_DELAY - SCAN_LATENCY);
	} else {
		lpn_set_state(BT_MESH_LPN_WAIT_UPDATE);
		k_delayed_work_submit(&lpn->timer,
				      LPN_RECV_DELAY + duration +
				      lpn->recv_win);
	}
}

static const struct bt_mesh_send_cb req_sent_cb = {
	.start = req_sent,
};

static int send_friend_poll(void)
{
	struct bt_mesh_msg_ctx ctx = {
		.net_idx     = bt_mesh.lpn.sub->net_idx,
		.app_idx     = BT_MESH_KEY_UNUSED,
		.addr        = bt_mesh.lpn.frnd,
		.send_ttl    = 0,
	};
	struct bt_mesh_net_tx tx = {
		.sub = bt_mesh.lpn.sub,
		.ctx = &ctx,
		.src = bt_mesh_primary_addr(),
		.xmit = POLL_XMIT,
		.friend_cred = true,
	};
	struct bt_mesh_lpn *lpn = &bt_mesh.lpn;
	uint8_t fsn = lpn->fsn;
	int err;

	BT_DBG("lpn->sent_req 0x%02x", lpn->sent_req);

	if (lpn->sent_req) {
		if (lpn->sent_req != TRANS_CTL_OP_FRIEND_POLL) {
			lpn->pending_poll = 1;
		}

		return 0;
	}

	err = bt_mesh_ctl_send(&tx, TRANS_CTL_OP_FRIEND_POLL, &fsn, 1,
			       		   &req_sent_cb, NULL);
	if (err == 0) {
		lpn->pending_poll = 0;
		lpn->sent_req = TRANS_CTL_OP_FRIEND_POLL;
	}

	return err;
}

void bt_mesh_lpn_disable(bool force)
{
	if (bt_mesh.lpn.state == BT_MESH_LPN_DISABLED) {
		return;
	}

	clear_friendship(force, true);
}

int bt_mesh_lpn_set(bool enable)
{
	struct bt_mesh_lpn *lpn = &bt_mesh.lpn;

	if (enable) {
		if (lpn->state != BT_MESH_LPN_DISABLED) {
			return 0;
		}
	} else {
		if (lpn->state == BT_MESH_LPN_DISABLED) {
			return 0;
		}
	}

	if (!bt_mesh_is_provisioned()) {
		if (enable) {
			lpn_set_state(BT_MESH_LPN_ENABLED);
		} else {
			lpn_set_state(BT_MESH_LPN_DISABLED);
		}

		return 0;
	}

	if (enable) {
		lpn_set_state(BT_MESH_LPN_ENABLED);

		if (IS_ENABLED(CONFIG_BT_MESH_LPN_ESTABLISHMENT)) {
			bt_mesh_scan_disable();
		}

		send_friend_req(lpn);
	} else {
		if (IS_ENABLED(CONFIG_BT_MESH_LPN_AUTO) &&
		    lpn->state == BT_MESH_LPN_TIMER) {
			k_delayed_work_cancel(&lpn->timer);
			lpn_set_state(BT_MESH_LPN_DISABLED);
		} else {
			bt_mesh_lpn_disable(false);
		}
	}

	return 0;
}

static void friend_response_received(struct bt_mesh_lpn *lpn)
{
	BT_DBG("lpn->sent_req 0x%02x", lpn->sent_req);

	if (lpn->sent_req == TRANS_CTL_OP_FRIEND_POLL) {
		lpn->fsn++;
	}

	k_delayed_work_cancel(&lpn->timer);
	bt_mesh_scan_disable();
	lpn_set_state(BT_MESH_LPN_ESTABLISHED);
	lpn->req_attempts = 0;
	lpn->sent_req = 0;
}

void bt_mesh_lpn_msg_received(struct bt_mesh_net_rx *rx)
{
	struct bt_mesh_lpn *lpn = &bt_mesh.lpn;

	if (lpn->state == BT_MESH_LPN_TIMER) {
		BT_DBG("Restarting establishment timer");
		k_delayed_work_submit(&lpn->timer, LPN_AUTO_TIMEOUT);
		return;
	}

	if (lpn->sent_req != TRANS_CTL_OP_FRIEND_POLL) {
		BT_WARN("Unexpected message withouth a preceding Poll");
		return;
	}

	friend_response_received(lpn);

	BT_DBG("Requesting more messages from Friend");

	send_friend_poll();
}

static int friend_cred_create(struct bt_mesh_net_cred *cred,
			      const uint8_t key[16])
{
	struct bt_mesh_lpn *lpn = &bt_mesh.lpn;

	return bt_mesh_friend_cred_create(cred, bt_mesh_primary_addr(),
					  lpn->frnd, lpn->lpn_counter,
					  lpn->frnd_counter, key);
}

int bt_mesh_lpn_friend_offer(struct bt_mesh_net_rx *rx,
			     struct os_mbuf *buf)
{
	struct bt_mesh_ctl_friend_offer *msg = (void *)buf->om_data;
	struct bt_mesh_lpn *lpn = &bt_mesh.lpn;
	uint16_t frnd_counter;
	int err;

	if (buf->om_len < sizeof(*msg)) {
		BT_WARN("Too short Friend Offer");
		return -EINVAL;
	}

	if (lpn->state != BT_MESH_LPN_WAIT_OFFER) {
		BT_WARN("Ignoring unexpected Friend Offer");
		return 0;
	}

	if (!msg->recv_win) {
		BT_WARN("Prohibited ReceiveWindow value");
		return -EINVAL;
	}

	frnd_counter = sys_be16_to_cpu(msg->frnd_counter);

	BT_DBG("recv_win %u queue_size %u sub_list_size %u rssi %d counter %u",
	       msg->recv_win, msg->queue_size, msg->sub_list_size, msg->rssi,
	       frnd_counter);

	lpn->frnd_counter = frnd_counter;
	lpn->frnd = rx->ctx.addr;

	/* Create friend credentials for each of the valid keys in the
	 * friendship subnet:
	 */
	for (int i = 0; i < ARRAY_SIZE(lpn->cred); i++) {
		if (!lpn->sub->keys[i].valid) {
			continue;
		}

		err = friend_cred_create(&lpn->cred[i], lpn->sub->keys[i].net);
		if (err) {
			lpn->frnd = BT_MESH_ADDR_UNASSIGNED;
			return err;
		}
	}
	/* TODO: Add offer acceptance criteria check */
	k_delayed_work_cancel(&lpn->timer);

	lpn->recv_win = msg->recv_win;
	lpn->queue_size = msg->queue_size;

	err = send_friend_poll();
	if (err) {
		lpn->sub = NULL;
		lpn->frnd = BT_MESH_ADDR_UNASSIGNED;
		lpn->recv_win = 0;
		lpn->queue_size = 0;
		return err;
	}

	return 0;
}

int bt_mesh_lpn_friend_clear_cfm(struct bt_mesh_net_rx *rx,
				 struct os_mbuf *buf)
{
	struct bt_mesh_ctl_friend_clear_confirm *msg = (void *)buf->om_data;
	struct bt_mesh_lpn *lpn = &bt_mesh.lpn;
	uint16_t addr, counter;

	if (buf->om_len < sizeof(*msg)) {
		BT_WARN("Too short Friend Clear Confirm");
		return -EINVAL;
	}

	if (lpn->state != BT_MESH_LPN_CLEAR) {
		BT_WARN("Ignoring unexpected Friend Clear Confirm");
		return 0;
	}

	addr = sys_be16_to_cpu(msg->lpn_addr);
	counter = sys_be16_to_cpu(msg->lpn_counter);

	BT_DBG("LPNAddress 0x%04x LPNCounter 0x%04x", addr, counter);

	if (addr != bt_mesh_primary_addr() || counter != lpn->lpn_counter) {
		BT_WARN("Invalid parameters in Friend Clear Confirm");
		return 0;
	}

	lpn->clear_success = 1;
	clear_friendship(false, lpn->disable);

	return 0;
}

static void lpn_group_add(uint16_t group)
{
	struct bt_mesh_lpn *lpn = &bt_mesh.lpn;
	uint16_t *free_slot = NULL;
	int i;

	for (i = 0; i < ARRAY_SIZE(lpn->groups); i++) {
		if (lpn->groups[i] == group) {
			atomic_clear_bit(lpn->to_remove, i);
			return;
		}

		if (!free_slot && lpn->groups[i] == BT_MESH_ADDR_UNASSIGNED) {
			free_slot = &lpn->groups[i];
		}
	}

	if (!free_slot) {
		BT_WARN("Friend Subscription List exceeded!");
		return;
	}

	*free_slot = group;
	lpn->groups_changed = 1;
}

static void lpn_group_del(uint16_t group)
{
	struct bt_mesh_lpn *lpn = &bt_mesh.lpn;
	int i;

	for (i = 0; i < ARRAY_SIZE(lpn->groups); i++) {
		if (lpn->groups[i] == group) {
			if (atomic_test_bit(lpn->added, i) ||
			    atomic_test_bit(lpn->pending, i)) {
				atomic_set_bit(lpn->to_remove, i);
				lpn->groups_changed = 1;
			} else {
				lpn->groups[i] = BT_MESH_ADDR_UNASSIGNED;
			}
		}
	}
}

static inline int group_popcount(atomic_t *target)
{
#if CONFIG_BT_MESH_LPN_GROUPS > 32
	int i, count = 0;

	for (i = 0; i < ARRAY_SIZE(bt_mesh.lpn.added); i++) {
		count += popcount(atomic_get(&target[i]));
	}
#else
	return popcount(atomic_get(target));
#endif
}

static bool sub_update(uint8_t op)
{
	struct bt_mesh_lpn *lpn = &bt_mesh.lpn;
	int added_count = group_popcount(lpn->added);
	struct bt_mesh_msg_ctx ctx = {
		.net_idx     = lpn->sub->net_idx,
		.app_idx     = BT_MESH_KEY_UNUSED,
		.addr        = lpn->frnd,
		.send_ttl    = 0,
	};
	struct bt_mesh_net_tx tx = {
		.sub = lpn->sub,
		.ctx = &ctx,
		.src = bt_mesh_primary_addr(),
		.xmit = POLL_XMIT,
		.friend_cred = true,
	};
	struct bt_mesh_ctl_friend_sub req;
	size_t i, g;

	BT_DBG("op 0x%02x sent_req 0x%02x", op, lpn->sent_req);

	if (lpn->sent_req) {
		return false;
	}

	for (i = 0, g = 0; i < ARRAY_SIZE(lpn->groups); i++) {
		if (lpn->groups[i] == BT_MESH_ADDR_UNASSIGNED) {
			continue;
		}

		if (op == TRANS_CTL_OP_FRIEND_SUB_ADD) {
			if (atomic_test_bit(lpn->added, i)) {
				continue;
			}
		} else {
			if (!atomic_test_bit(lpn->to_remove, i)) {
				continue;
			}
		}

		if (added_count + g >= lpn->queue_size) {
			BT_WARN("Friend Queue Size exceeded");
			break;
		}

		req.addr_list[g++] = sys_cpu_to_be16(lpn->groups[i]);
		atomic_set_bit(lpn->pending, i);

		if (g == ARRAY_SIZE(req.addr_list)) {
			break;
		}
	}

	if (g == 0) {
		group_zero(lpn->pending);
		return false;
	}

	req.xact = lpn->xact_next++;

	if (bt_mesh_ctl_send(&tx, op, &req, 1 + g * 2, 
						 &req_sent_cb, NULL) < 0) {
		group_zero(lpn->pending);
		return false;
	}

	lpn->xact_pending = req.xact;
	lpn->sent_req = op;
	return true;
}

static void update_timeout(struct bt_mesh_lpn *lpn)
{
	if (lpn->established) {
		BT_WARN("No response from Friend during ReceiveWindow");
		bt_mesh_scan_disable();
		lpn_set_state(BT_MESH_LPN_ESTABLISHED);
		k_delayed_work_submit(&lpn->timer, POLL_RETRY_TIMEOUT);
	} else {
		if (IS_ENABLED(CONFIG_BT_MESH_LPN_ESTABLISHMENT)) {
			bt_mesh_scan_disable();
		}

		if (lpn->req_attempts < REQ_ATTEMPTS(lpn)) {
			BT_WARN("Retrying first Friend Poll");
			lpn->sent_req = 0;
			if (send_friend_poll() == 0) {
				return;
			}
		}

		BT_ERR("Timed out waiting for first Friend Update");
		clear_friendship(false, false);
	}
}

static void lpn_timeout(struct ble_npl_event *work)
{
	struct bt_mesh_lpn *lpn = &bt_mesh.lpn;

#if MYNEWT_VAL(BLE_MESH_LOW_POWER_LOG_LVL) == LOG_LEVEL_DEBUG
	BT_DBG("state: %s", state2str(lpn->state));
#endif

	switch (lpn->state) {
	case BT_MESH_LPN_DISABLED:
		break;
	case BT_MESH_LPN_CLEAR:
		clear_friendship(false, bt_mesh.lpn.disable);
		break;
	case BT_MESH_LPN_TIMER:
		BT_DBG("Starting to look for Friend nodes");
		lpn_set_state(BT_MESH_LPN_ENABLED);
		if (IS_ENABLED(CONFIG_BT_MESH_LPN_ESTABLISHMENT)) {
			bt_mesh_scan_disable();
		}
		/* fall through */
	case BT_MESH_LPN_ENABLED:
		send_friend_req(lpn);
		break;
	case BT_MESH_LPN_REQ_WAIT:
		bt_mesh_scan_enable();
		k_delayed_work_submit(&lpn->timer,
				      lpn->adv_duration + FRIEND_REQ_SCAN);
		lpn_set_state(BT_MESH_LPN_WAIT_OFFER);
		break;
	case BT_MESH_LPN_WAIT_OFFER:
		BT_WARN("No acceptable Friend Offers received");
		if (IS_ENABLED(CONFIG_BT_MESH_LPN_ESTABLISHMENT)) {
			bt_mesh_scan_disable();
		}
		lpn->lpn_counter++;
		lpn_set_state(BT_MESH_LPN_ENABLED);
		lpn->sent_req = 0U;
		k_delayed_work_submit(&lpn->timer, FRIEND_REQ_RETRY_TIMEOUT);
		break;
	case BT_MESH_LPN_ESTABLISHED:
		if (lpn->req_attempts < REQ_ATTEMPTS(lpn)) {
			uint8_t req = lpn->sent_req;

			lpn->sent_req = 0;

			if (!req || req == TRANS_CTL_OP_FRIEND_POLL) {
				send_friend_poll();
			} else {
				sub_update(req);
			}

			break;
		}

		BT_ERR("No response from Friend after %u retries",
		       lpn->req_attempts);
		lpn->req_attempts = 0;
		clear_friendship(false, false);
		break;
	case BT_MESH_LPN_RECV_DELAY:
		k_delayed_work_submit(&lpn->timer,
				      lpn->adv_duration + SCAN_LATENCY +
				      lpn->recv_win);
		bt_mesh_scan_enable();
		lpn_set_state(BT_MESH_LPN_WAIT_UPDATE);
		break;
	case BT_MESH_LPN_WAIT_UPDATE:
		update_timeout(lpn);
		break;
	default:
		__ASSERT(0, "Unhandled LPN state");
		break;
	}
}

void bt_mesh_lpn_group_add(uint16_t group)
{
	BT_DBG("group 0x%04x", group);

	lpn_group_add(group);

	if (!bt_mesh_lpn_established() || bt_mesh.lpn.sent_req) {
		return;
	}

	sub_update(TRANS_CTL_OP_FRIEND_SUB_ADD);
}

void bt_mesh_lpn_group_del(uint16_t *groups, size_t group_count)
{
	int i;

	for (i = 0; i < group_count; i++) {
		if (groups[i] != BT_MESH_ADDR_UNASSIGNED) {
			BT_DBG("group 0x%04x", groups[i]);
			lpn_group_del(groups[i]);
		}
	}

	if (!bt_mesh_lpn_established() || bt_mesh.lpn.sent_req) {
		return;
	}

	sub_update(TRANS_CTL_OP_FRIEND_SUB_REM);
}

static int32_t poll_timeout(struct bt_mesh_lpn *lpn)
{
	/* If we're waiting for segment acks keep polling at high freq */
	if (bt_mesh_tx_in_progress()) {
		return min(POLL_TIMEOUT_MAX(lpn), K_SECONDS(1));
	}

	if (lpn->poll_timeout < POLL_TIMEOUT_MAX(lpn)) {
		lpn->poll_timeout *= 2;
		lpn->poll_timeout = min(lpn->poll_timeout,
					POLL_TIMEOUT_MAX(lpn));
	}

	BT_DBG("Poll Timeout is %ums", (unsigned) lpn->poll_timeout);

	return lpn->poll_timeout;
}

int bt_mesh_lpn_friend_sub_cfm(struct bt_mesh_net_rx *rx,
			       struct os_mbuf *buf)
{
	struct bt_mesh_ctl_friend_sub_confirm *msg = (void *)buf->om_data;
	struct bt_mesh_lpn *lpn = &bt_mesh.lpn;

	if (buf->om_len < sizeof(*msg)) {
		BT_WARN("Too short Friend Subscription Confirm");
		return -EINVAL;
	}

	BT_DBG("xact 0x%02x", msg->xact);

	if (!lpn->sent_req) {
		BT_WARN("No pending subscription list message");
		return 0;
	}

	if (msg->xact != lpn->xact_pending) {
		BT_WARN("Transaction mismatch (0x%02x != 0x%02x)",
			msg->xact, lpn->xact_pending);
		return 0;
	}

	if (lpn->sent_req == TRANS_CTL_OP_FRIEND_SUB_ADD) {
		group_set(lpn->added, lpn->pending);
		group_zero(lpn->pending);
	} else if (lpn->sent_req == TRANS_CTL_OP_FRIEND_SUB_REM) {
		int i;

		group_clear(lpn->added, lpn->pending);

		for (i = 0; i < ARRAY_SIZE(lpn->groups); i++) {
			if (atomic_test_and_clear_bit(lpn->pending, i) &&
			    atomic_test_and_clear_bit(lpn->to_remove, i)) {
				lpn->groups[i] = BT_MESH_ADDR_UNASSIGNED;
			}
		}
	} else {
		BT_WARN("Unexpected Friend Subscription Confirm");
		return 0;
	}

	friend_response_received(lpn);

	if (lpn->groups_changed) {
		sub_update(TRANS_CTL_OP_FRIEND_SUB_ADD);
		sub_update(TRANS_CTL_OP_FRIEND_SUB_REM);

		if (!lpn->sent_req) {
			lpn->groups_changed = 0;
		}
	}

	if (lpn->pending_poll) {
		send_friend_poll();
	}

	if (!lpn->sent_req) {
		int32_t timeout = poll_timeout(lpn);

		k_delayed_work_submit(&lpn->timer, K_MSEC(timeout));
	}

	return 0;
}

int bt_mesh_lpn_friend_update(struct bt_mesh_net_rx *rx,
			      struct os_mbuf *buf)
{
	struct bt_mesh_ctl_friend_update *msg = (void *)buf->om_data;
	struct bt_mesh_lpn *lpn = &bt_mesh.lpn;
	struct bt_mesh_subnet *sub = rx->sub;
	uint32_t iv_index;

	if (buf->om_len < sizeof(*msg)) {
		BT_WARN("Too short Friend Update");
		return -EINVAL;
	}

	if (lpn->sent_req != TRANS_CTL_OP_FRIEND_POLL) {
		BT_WARN("Unexpected friend update");
		return 0;
	}

	if (sub->kr_phase == BT_MESH_KR_PHASE_2 && !rx->new_key) {
		BT_WARN("Ignoring Phase 2 KR Update secured using old key");
		return 0;
	}

	if (atomic_test_bit(bt_mesh.flags, BT_MESH_IVU_INITIATOR) &&
	    (atomic_test_bit(bt_mesh.flags, BT_MESH_IVU_IN_PROGRESS) ==
	     BT_MESH_IV_UPDATE(msg->flags))) {
		bt_mesh_beacon_ivu_initiator(false);
	}

	if (!lpn->established) {
		/* This is normally checked on the transport layer, however
		 * in this state we're also still accepting master
		 * credentials so we need to ensure the right ones (Friend
		 * Credentials) were used for this message.
		 */
		if (!rx->friend_cred) {
			BT_WARN("Friend Update with wrong credentials");
			return -EINVAL;
		}

		lpn->established = 1;

		BT_INFO("Friendship established with 0x%04x", lpn->frnd);

		bt_mesh_hb_feature_changed(BT_MESH_FEAT_LOW_POWER);

		if (lpn_cb) {
			lpn_cb(lpn->frnd, true);
		}

		/* Set initial poll timeout */
		lpn->poll_timeout = min(POLL_TIMEOUT_MAX(lpn),
					POLL_TIMEOUT_INIT);
	}

	friend_response_received(lpn);

	iv_index = sys_be32_to_cpu(msg->iv_index);

	BT_DBG("flags 0x%02x iv_index 0x%08x md %u", msg->flags,
	       (unsigned) iv_index, msg->md);

	bt_mesh_kr_update(sub, BT_MESH_KEY_REFRESH(msg->flags), rx->new_key);
	bt_mesh_net_iv_update(iv_index, BT_MESH_IV_UPDATE(msg->flags));

	if (lpn->groups_changed) {
		sub_update(TRANS_CTL_OP_FRIEND_SUB_ADD);
		sub_update(TRANS_CTL_OP_FRIEND_SUB_REM);

		if (!lpn->sent_req) {
			lpn->groups_changed = 0;
		}
	}

	if (msg->md) {
		BT_DBG("Requesting for more messages");
		send_friend_poll();
	}

	if (!lpn->sent_req) {
		int32_t timeout = poll_timeout(lpn);

		k_delayed_work_submit(&lpn->timer, K_MSEC(timeout));
	}

	return 0;
}

int bt_mesh_lpn_poll(void)
{
	if (!bt_mesh.lpn.established) {
		return -EAGAIN;
	}

	BT_DBG("Requesting more messages");

	return send_friend_poll();
}

void bt_mesh_lpn_set_cb(void (*cb)(uint16_t friend_addr, bool established))
{
	lpn_cb = cb;
}

static void subnet_evt(struct bt_mesh_subnet *sub, enum bt_mesh_key_evt evt)
{
	switch (evt) {
	case BT_MESH_KEY_DELETED:
		if (sub == bt_mesh.lpn.sub) {
			BT_DBG("NetKey deleted");
			clear_friendship(true, false);
		}
		break;
	case BT_MESH_KEY_UPDATED:
		BT_DBG("NetKey updated");
		friend_cred_create(&bt_mesh.lpn.cred[1], sub->keys[1].net);
		break;
	default:
		break;
	}
}

int bt_mesh_lpn_init(void)
{
	struct bt_mesh_lpn *lpn = &bt_mesh.lpn;

	if (!bt_mesh_subnet_cb_list[2]) {
		bt_mesh_subnet_cb_list[2] = subnet_evt;
	}

	BT_DBG("");

	lpn->groups_changed = 0;

	k_delayed_work_init(&lpn->timer, lpn_timeout);

	if (lpn->state == BT_MESH_LPN_ENABLED) {
		if (IS_ENABLED(CONFIG_BT_MESH_LPN_ESTABLISHMENT)) {
			bt_mesh_scan_disable();
		} else {
			bt_mesh_scan_enable();
		}

		send_friend_req(lpn);
	} else {
		bt_mesh_scan_enable();

		if (IS_ENABLED(CONFIG_BT_MESH_LPN_AUTO)) {
			BT_DBG("Waiting %u ms for messages", LPN_AUTO_TIMEOUT);
			lpn_set_state(BT_MESH_LPN_TIMER);
			k_delayed_work_submit(&lpn->timer, LPN_AUTO_TIMEOUT);
		}
	}

	return 0;
}

#endif /* MYNEWT_VAL(BLE_MESH_LOW_POWER) */
#endif
