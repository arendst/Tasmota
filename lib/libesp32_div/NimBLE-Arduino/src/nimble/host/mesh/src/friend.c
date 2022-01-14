	/*  Bluetooth Mesh */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "syscfg/syscfg.h"
#define MESH_LOG_MODULE BLE_MESH_CRYPTO_LOG

#if MYNEWT_VAL(BLE_MESH_FRIEND)

#include <stdint.h>
#include <errno.h>
#include <assert.h>

#include "mesh/mesh.h"
#include "mesh/slist.h"
#include "mesh_priv.h"
#include "crypto.h"
#include "adv.h"
#include "net.h"
#include "transport.h"
#include "access.h"
#include "foundation.h"
#include "friend.h"

/* We reserve one extra buffer for each friendship, since we need to be able
 * to resend the last sent PDU, which sits separately outside of the queue.
 */
#define FRIEND_BUF_COUNT ((MYNEWT_VAL(BLE_MESH_FRIEND_QUEUE_SIZE) + 1) * MYNEWT_VAL(BLE_MESH_FRIEND_LPN_COUNT))

static os_membuf_t friend_buf_mem[OS_MEMPOOL_SIZE(
		FRIEND_BUF_COUNT,
		BT_MESH_ADV_DATA_SIZE + BT_MESH_MBUF_HEADER_SIZE)];

struct os_mbuf_pool friend_os_mbuf_pool;
static struct os_mempool friend_buf_mempool;

#define NET_BUF_FRAGS        BIT(0)

#define FRIEND_ADV(buf) CONTAINER_OF(BT_MESH_ADV(buf), struct friend_adv, adv)

/* PDUs from Friend to the LPN should only be transmitted once with the
 * smallest possible interval (20ms).
 */
#define FRIEND_XMIT         BT_MESH_TRANSMIT(0, 20)

struct friend_pdu_info {
	u16_t  src;
	u16_t  dst;

	u8_t   seq[3];

	u8_t   ttl:7,
	       ctl:1;

	u32_t  iv_index;
};

static struct friend_adv {
	struct bt_mesh_adv adv;
	u16_t app_idx;
} adv_pool[FRIEND_BUF_COUNT];

static struct bt_mesh_adv *adv_alloc(int id)
{
	adv_pool[id].app_idx = BT_MESH_KEY_UNUSED;
	return &adv_pool[id].adv;
}

static bool is_lpn_unicast(struct bt_mesh_friend *frnd, u16_t addr)
{
	if (frnd->lpn == BT_MESH_ADDR_UNASSIGNED) {
		return false;
	}

	return (addr >= frnd->lpn && addr < (frnd->lpn + frnd->num_elem));
}

struct bt_mesh_friend *bt_mesh_friend_find(u16_t net_idx, u16_t lpn_addr,
					   bool valid, bool established)
{
	int i;

	BT_DBG("net_idx 0x%04x lpn_addr 0x%04x", net_idx, lpn_addr);

	for (i = 0; i < ARRAY_SIZE(bt_mesh.frnd); i++) {
		struct bt_mesh_friend *frnd = &bt_mesh.frnd[i];

		if (valid && !frnd->valid) {
			continue;
		}

		if (established && !frnd->established) {
			continue;
		}

		if (net_idx != BT_MESH_KEY_ANY && frnd->net_idx != net_idx) {
			continue;
		}

		if (is_lpn_unicast(frnd, lpn_addr)) {
			return frnd;
		}
	}

	return NULL;
}

static void purge_buffers(struct net_buf_slist_t *list)
{
	struct os_mbuf *buf;

	while (!net_buf_slist_is_empty(list)) {
		buf = (void *)net_buf_slist_get(list);
		BT_MESH_ADV(buf)->flags &= ~NET_BUF_FRAGS;
		net_buf_unref(buf);
	}
}

/* Intentionally start a little bit late into the ReceiveWindow when
 * it's large enough. This may improve reliability with some platforms,
 * like the PTS, where the receiver might not have sufficiently compensated
 * for internal latencies required to start scanning.
 */
static s32_t recv_delay(struct bt_mesh_friend *frnd)
{
#if CONFIG_BT_MESH_FRIEND_RECV_WIN > 50
	return (s32_t)frnd->recv_delay + (CONFIG_BT_MESH_FRIEND_RECV_WIN / 5);
#else
	return frnd->recv_delay;
#endif
}

static void friend_clear(struct bt_mesh_friend *frnd)
{
	int i;

	BT_DBG("LPN 0x%04x", frnd->lpn);

	k_delayed_work_cancel(&frnd->timer);

	friend_cred_del(frnd->net_idx, frnd->lpn);

	if (frnd->last) {
		/* Cancel the sending if necessary */
		if (frnd->pending_buf) {
			BT_MESH_ADV(frnd->last)->busy = 0;
		}

		net_buf_unref(frnd->last);
		frnd->last = NULL;
	}

	purge_buffers(&frnd->queue);

	for (i = 0; i < ARRAY_SIZE(frnd->seg); i++) {
		struct bt_mesh_friend_seg *seg = &frnd->seg[i];

		purge_buffers(&seg->queue);
		seg->seg_count = 0U;
	}

	frnd->valid = 0;
	frnd->established = 0;
	frnd->pending_buf = 0;
	frnd->fsn = 0;
	frnd->queue_size = 0;
	frnd->pending_req = 0;
	memset(frnd->sub_list, 0, sizeof(frnd->sub_list));
}

void bt_mesh_friend_clear_net_idx(u16_t net_idx)
{
	int i;

	BT_DBG("net_idx 0x%04x", net_idx);

	for (i = 0; i < ARRAY_SIZE(bt_mesh.frnd); i++) {
		struct bt_mesh_friend *frnd = &bt_mesh.frnd[i];

		if (frnd->net_idx == BT_MESH_KEY_UNUSED) {
			continue;
		}

		if (net_idx == BT_MESH_KEY_ANY || frnd->net_idx == net_idx) {
			friend_clear(frnd);
		}
	}
}

void bt_mesh_friend_sec_update(u16_t net_idx)
{
	int i;

	BT_DBG("net_idx 0x%04x", net_idx);

	for (i = 0; i < ARRAY_SIZE(bt_mesh.frnd); i++) {
		struct bt_mesh_friend *frnd = &bt_mesh.frnd[i];

		if (frnd->net_idx == BT_MESH_KEY_UNUSED) {
			continue;
		}

		if (net_idx == BT_MESH_KEY_ANY || frnd->net_idx == net_idx) {
			frnd->sec_update = 1;
		}
	}
}

int bt_mesh_friend_clear(struct bt_mesh_net_rx *rx, struct os_mbuf *buf)
{
	struct bt_mesh_ctl_friend_clear *msg = (void *)buf->om_data;
	struct bt_mesh_friend *frnd;
	u16_t lpn_addr, lpn_counter;
	struct bt_mesh_net_tx tx = {
		.sub  = rx->sub,
		.ctx  = &rx->ctx,
		.src  = bt_mesh_primary_addr(),
		.xmit = bt_mesh_net_transmit_get(),
	};
	struct bt_mesh_ctl_friend_clear_confirm cfm;

	if (buf->om_len < sizeof(*msg)) {
		BT_WARN("Too short Friend Clear");
		return -EINVAL;
	}

	lpn_addr = sys_be16_to_cpu(msg->lpn_addr);
	lpn_counter = sys_be16_to_cpu(msg->lpn_counter);

	BT_DBG("LPN addr 0x%04x counter 0x%04x", lpn_addr, lpn_counter);

	frnd = bt_mesh_friend_find(rx->sub->net_idx, lpn_addr, false, false);
	if (!frnd) {
		BT_WARN("No matching LPN addr 0x%04x", lpn_addr);
		return 0;
	}

	/* A Friend Clear message is considered valid if the result of the
	 * subtraction of the value of the LPNCounter field of the Friend
	 * Request message (the one that initiated the friendship) from the
	 * value of the LPNCounter field of the Friend Clear message, modulo
	 * 65536, is in the range 0 to 255 inclusive.
	 */
	if (lpn_counter - frnd->lpn_counter > 255) {
		BT_WARN("LPN Counter out of range (old %u new %u)",
			frnd->lpn_counter, lpn_counter);
		return 0;
	}

	tx.ctx->send_ttl = BT_MESH_TTL_MAX;

	cfm.lpn_addr    = msg->lpn_addr;
	cfm.lpn_counter = msg->lpn_counter;

	bt_mesh_ctl_send(&tx, TRANS_CTL_OP_FRIEND_CLEAR_CFM, &cfm,
			 sizeof(cfm), NULL, NULL, NULL);

	friend_clear(frnd);

	return 0;
}

static void friend_sub_add(struct bt_mesh_friend *frnd, u16_t addr)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(frnd->sub_list); i++) {
		if (frnd->sub_list[i] == BT_MESH_ADDR_UNASSIGNED) {
			frnd->sub_list[i] = addr;
			return;
		}
	}

	BT_WARN("No space in friend subscription list");
}

static void friend_sub_rem(struct bt_mesh_friend *frnd, u16_t addr)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(frnd->sub_list); i++) {
		if (frnd->sub_list[i] == addr) {
			frnd->sub_list[i] = BT_MESH_ADDR_UNASSIGNED;
			return;
		}
	}
}

static struct os_mbuf *create_friend_pdu(struct bt_mesh_friend *frnd,
					 struct friend_pdu_info *info,
					 struct os_mbuf *sdu)
{
	struct os_mbuf *buf;

	buf = bt_mesh_adv_create_from_pool(&friend_os_mbuf_pool, adv_alloc,
					   BT_MESH_ADV_DATA,
					   FRIEND_XMIT, K_NO_WAIT);
	if (!buf) {
		return NULL;
	}

	net_buf_add_u8(buf, (info->iv_index & 1) << 7); /* Will be reset in encryption */

	if (info->ctl) {
		net_buf_add_u8(buf, info->ttl | 0x80);
	} else {
		net_buf_add_u8(buf, info->ttl);
	}

	net_buf_add_mem(buf, info->seq, sizeof(info->seq));

	net_buf_add_be16(buf, info->src);
	net_buf_add_be16(buf, info->dst);

	net_buf_add_mem(buf, sdu->om_data, sdu->om_len);

	return buf;
}

struct unseg_app_sdu_meta {
	struct bt_mesh_net_rx net;
	const u8_t *key;
	struct bt_mesh_subnet *subnet;
	bool is_dev_key;
	u8_t aid;
	u8_t *ad;
};

static int unseg_app_sdu_unpack(struct bt_mesh_friend *frnd,
				struct os_mbuf *buf,
				struct unseg_app_sdu_meta *meta)
{
	u16_t app_idx = FRIEND_ADV(buf)->app_idx;
	int err;

	meta->subnet = bt_mesh_subnet_get(frnd->net_idx);
	meta->is_dev_key = (app_idx == BT_MESH_KEY_DEV);
	meta->is_dev_key = BT_MESH_IS_DEV_KEY(app_idx);
	bt_mesh_net_header_parse(buf, &meta->net);
	err = bt_mesh_app_key_get(meta->subnet, app_idx, meta->net.ctx.recv_dst,
				  &meta->key, &meta->aid);
	if (err) {
		return err;
	}

	if (BT_MESH_ADDR_IS_VIRTUAL(meta->net.ctx.recv_dst)) {
		meta->ad = bt_mesh_label_uuid_get(meta->net.ctx.recv_dst);
		if (!meta->ad) {
			return -ENOENT;
		}
	} else {
		meta->ad = NULL;
	}

	return 0;
}

static int unseg_app_sdu_decrypt(struct bt_mesh_friend *frnd,
				 struct os_mbuf *buf,
				 const struct unseg_app_sdu_meta *meta)
{
	struct net_buf_simple_state state;
	int err;

	BT_DBG("");

	net_buf_simple_save(buf, &state);
	net_buf_simple_pull_mem(buf, 10);
	buf->om_len -= 4;

	err = bt_mesh_app_decrypt_in_place(meta->key, meta->is_dev_key,
					   0, buf, meta->ad, meta->net.ctx.addr,
					   meta->net.ctx.recv_dst, meta->net.seq,
					   BT_MESH_NET_IVI_TX);

	net_buf_simple_restore(buf, &state);
	return err;
}

static int unseg_app_sdu_encrypt(struct bt_mesh_friend *frnd,
				 struct os_mbuf *buf,
				 const struct unseg_app_sdu_meta *meta)
{
	struct net_buf_simple_state state;
	int err;

	BT_DBG("");

	net_buf_simple_save(buf, &state);
	net_buf_simple_pull_mem(buf, 10);
	buf->om_len -= 4;

	err = bt_mesh_app_encrypt_in_place(meta->key, meta->is_dev_key, 0, buf,
					   meta->ad, meta->net.ctx.addr,
					   meta->net.ctx.recv_dst, bt_mesh.seq,
					   BT_MESH_NET_IVI_TX);

	net_buf_simple_restore(buf, &state);
	return err;
}

static int unseg_app_sdu_prepare(struct bt_mesh_friend *frnd,
				 struct os_mbuf *buf)
{
	struct unseg_app_sdu_meta meta;
	int err;

	BT_DBG("");

	if (FRIEND_ADV(buf)->app_idx == BT_MESH_KEY_UNUSED) {
		return 0;
	}

	err = unseg_app_sdu_unpack(frnd, buf, &meta);
	if (err) {
		return err;
	}

	/* No need to reencrypt the message if the sequence number is
	 * unchanged.
	 */
	if (meta.net.seq == bt_mesh.seq) {
		return 0;
	}

	err = unseg_app_sdu_decrypt(frnd, buf, &meta);
	if (err) {
		BT_WARN("Decryption failed! %d", err);
		return err;
	}

	err = unseg_app_sdu_encrypt(frnd, buf, &meta);
	if (err) {
		BT_WARN("Re-encryption failed! %d", err);
	}

	return err;
}

static int encrypt_friend_pdu(struct bt_mesh_friend *frnd, struct os_mbuf *buf,
			      bool master_cred)
{
	struct bt_mesh_subnet *sub = bt_mesh_subnet_get(frnd->net_idx);
	const u8_t *enc, *priv;
	u32_t iv_index;
	u16_t src;
	u8_t nid;
	int err;

	if (master_cred) {
		enc = sub->keys[sub->kr_flag].enc;
		priv = sub->keys[sub->kr_flag].privacy;
		nid = sub->keys[sub->kr_flag].nid;
	} else {
		if (friend_cred_get(sub, frnd->lpn, &nid, &enc, &priv)) {
			BT_ERR("friend_cred_get failed");
			return -ENOENT;
		}
	}

	src = sys_get_be16(&buf->om_data[5]);

	if (bt_mesh_elem_find(src)) {
		u32_t seq;

		if (FRIEND_ADV(buf)->app_idx != BT_MESH_KEY_UNUSED) {
			err = unseg_app_sdu_prepare(frnd, buf);
			if (err) {
				return err;
			}
		}

		seq = bt_mesh_next_seq();
		buf->om_data[2] = seq >> 16;
		buf->om_data[3] = seq >> 8;
		buf->om_data[4] = seq;

		iv_index = BT_MESH_NET_IVI_TX;
		FRIEND_ADV(buf)->app_idx = BT_MESH_KEY_UNUSED;
	} else {
		u8_t ivi = (buf->om_data[0] >> 7);
		iv_index = (bt_mesh.iv_index - ((bt_mesh.iv_index & 1) != ivi));
	}

	buf->om_data[0] = (nid | (iv_index & 1) << 7);

	if (bt_mesh_net_encrypt(enc, buf, iv_index, false)) {
		BT_ERR("Encrypting failed");
		return -EINVAL;
	}

	if (bt_mesh_net_obfuscate(buf->om_data, iv_index, priv)) {
		BT_ERR("Obfuscating failed");
		return -EINVAL;
	}

	return 0;
}

static struct os_mbuf *encode_friend_ctl(struct bt_mesh_friend *frnd,
					 u8_t ctl_op,
					 struct os_mbuf *sdu)
{
	struct friend_pdu_info info;

	BT_DBG("LPN 0x%04x", frnd->lpn);

	net_buf_simple_push_u8(sdu, TRANS_CTL_HDR(ctl_op, 0));

	info.src = bt_mesh_primary_addr();
	info.dst = frnd->lpn;

	info.ctl = 1;
	info.ttl = 0;

	memset(info.seq, 0, sizeof(info.seq));

	info.iv_index = BT_MESH_NET_IVI_TX;

	return create_friend_pdu(frnd, &info, sdu);
}

static struct os_mbuf *encode_update(struct bt_mesh_friend *frnd, u8_t md)
{
	struct bt_mesh_ctl_friend_update *upd;
	struct os_mbuf *sdu = NET_BUF_SIMPLE(1 + sizeof(*upd));
	struct bt_mesh_subnet *sub = bt_mesh_subnet_get(frnd->net_idx);
	struct os_mbuf *buf;

	__ASSERT_NO_MSG(sub != NULL);

	BT_DBG("lpn 0x%04x md 0x%02x", frnd->lpn, md);

	net_buf_simple_init(sdu, 1);

	upd = net_buf_simple_add(sdu, sizeof(*upd));
	upd->flags = bt_mesh_net_flags(sub);
	upd->iv_index = sys_cpu_to_be32(bt_mesh.iv_index);
	upd->md = md;

	buf = encode_friend_ctl(frnd, TRANS_CTL_OP_FRIEND_UPDATE, sdu);

	os_mbuf_free_chain(sdu);
	return buf;
}

static void enqueue_sub_cfm(struct bt_mesh_friend *frnd, u8_t xact)
{
	struct bt_mesh_ctl_friend_sub_confirm *cfm;
	struct os_mbuf *sdu = NET_BUF_SIMPLE(1 + sizeof(*cfm));
	struct os_mbuf *buf;

	BT_DBG("lpn 0x%04x xact 0x%02x", frnd->lpn, xact);

	net_buf_simple_init(sdu, 1);

	cfm = net_buf_simple_add(sdu, sizeof(*cfm));
	cfm->xact = xact;

	buf = encode_friend_ctl(frnd, TRANS_CTL_OP_FRIEND_SUB_CFM, sdu);
	if (!buf) {
		BT_ERR("Unable to encode Subscription List Confirmation");
		goto done;
	}

	if (encrypt_friend_pdu(frnd, buf, false)) {
		return;
	}

	if (frnd->last) {
		BT_DBG("Discarding last PDU");
		net_buf_unref(frnd->last);
	}

	frnd->last = buf;
	frnd->send_last = 1;

done:
	os_mbuf_free_chain(sdu);
}

static void friend_recv_delay(struct bt_mesh_friend *frnd)
{
	frnd->pending_req = 1;
	k_delayed_work_submit(&frnd->timer, recv_delay(frnd));
	BT_DBG("Waiting RecvDelay of %d ms", (int) recv_delay(frnd));
}

int bt_mesh_friend_sub_add(struct bt_mesh_net_rx *rx,
			   struct os_mbuf *buf)
{
	struct bt_mesh_friend *frnd;
	u8_t xact;

	if (buf->om_len < BT_MESH_FRIEND_SUB_MIN_LEN) {
		BT_WARN("Too short Friend Subscription Add");
		return -EINVAL;
	}

	frnd = bt_mesh_friend_find(rx->sub->net_idx, rx->ctx.addr, true, true);
	if (!frnd) {
		BT_WARN("No matching LPN addr 0x%04x", rx->ctx.addr);
		return 0;
	}

	if (frnd->pending_buf) {
		BT_WARN("Previous buffer not yet sent!");
		return 0;
	}

	friend_recv_delay(frnd);

	xact = net_buf_simple_pull_u8(buf);

	while (buf->om_len >= 2) {
		friend_sub_add(frnd, net_buf_simple_pull_be16(buf));
	}

	enqueue_sub_cfm(frnd, xact);

	return 0;
}

int bt_mesh_friend_sub_rem(struct bt_mesh_net_rx *rx,
			   struct os_mbuf *buf)
{
	struct bt_mesh_friend *frnd;
	u8_t xact;

	if (buf->om_len < BT_MESH_FRIEND_SUB_MIN_LEN) {
		BT_WARN("Too short Friend Subscription Remove");
		return -EINVAL;
	}

	frnd = bt_mesh_friend_find(rx->sub->net_idx, rx->ctx.addr, true, true);
	if (!frnd) {
		BT_WARN("No matching LPN addr 0x%04x", rx->ctx.addr);
		return 0;
	}

	if (frnd->pending_buf) {
		BT_WARN("Previous buffer not yet sent!");
		return 0;
	}

	friend_recv_delay(frnd);

	xact = net_buf_simple_pull_u8(buf);

	while (buf->om_len >= 2) {
		friend_sub_rem(frnd, net_buf_simple_pull_be16(buf));
	}

	enqueue_sub_cfm(frnd, xact);

	return 0;
}

static void enqueue_buf(struct bt_mesh_friend *frnd, struct os_mbuf *buf)
{
	net_buf_slist_put(&frnd->queue, buf);
	frnd->queue_size++;
}

static void enqueue_update(struct bt_mesh_friend *frnd, u8_t md)
{
	struct os_mbuf *buf;

	buf = encode_update(frnd, md);
	if (!buf) {
		BT_ERR("Unable to encode Friend Update");
		return;
	}

	frnd->sec_update = 0;
	enqueue_buf(frnd, buf);
}

int bt_mesh_friend_poll(struct bt_mesh_net_rx *rx, struct os_mbuf *buf)
{
	struct bt_mesh_ctl_friend_poll *msg = (void *)buf->om_data;
	struct bt_mesh_friend *frnd;

	if (buf->om_len < sizeof(*msg)) {
		BT_WARN("Too short Friend Poll");
		return -EINVAL;
	}

	frnd = bt_mesh_friend_find(rx->sub->net_idx, rx->ctx.addr, true, false);
	if (!frnd) {
		BT_WARN("No matching LPN addr 0x%04x", rx->ctx.addr);
		return 0;
	}

	if (msg->fsn & ~1) {
		BT_WARN("Prohibited (non-zero) padding bits");
		return -EINVAL;
	}

	if (frnd->pending_buf) {
		BT_WARN("Previous buffer not yet sent");
		return 0;
	}

	BT_DBG("msg->fsn %u frnd->fsn %u", (msg->fsn & 1), frnd->fsn);

	friend_recv_delay(frnd);

	if (!frnd->established) {
		BT_DBG("Friendship established with 0x%04x", frnd->lpn);
		frnd->established = 1;
	}

	if (msg->fsn == frnd->fsn && frnd->last) {
		BT_DBG("Re-sending last PDU");
		frnd->send_last = 1;
	} else {
		if (frnd->last) {
			net_buf_unref(frnd->last);
			frnd->last = NULL;
		}

		frnd->fsn = msg->fsn;

		if (net_buf_slist_is_empty(&frnd->queue)) {
			enqueue_update(frnd, 0);
			BT_DBG("Enqueued Friend Update to empty queue");
		}
	}

	return 0;
}

static struct bt_mesh_friend *find_clear(u16_t prev_friend)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(bt_mesh.frnd); i++) {
		struct bt_mesh_friend *frnd = &bt_mesh.frnd[i];

		if (frnd->clear.frnd == prev_friend) {
			return frnd;
		}
	}

	return NULL;
}

static void friend_clear_sent(int err, void *user_data)
{
	struct bt_mesh_friend *frnd = user_data;

	k_delayed_work_submit(&frnd->clear.timer,
			      K_SECONDS(frnd->clear.repeat_sec));
	frnd->clear.repeat_sec *= 2;
}

static const struct bt_mesh_send_cb clear_sent_cb = {
	.end = friend_clear_sent,
};

static void send_friend_clear(struct bt_mesh_friend *frnd)
{
	struct bt_mesh_msg_ctx ctx = {
		.net_idx  = frnd->net_idx,
		.app_idx  = BT_MESH_KEY_UNUSED,
		.addr     = frnd->clear.frnd,
		.send_ttl = BT_MESH_TTL_MAX,
	};
	struct bt_mesh_net_tx tx = {
		.sub  = &bt_mesh.sub[0],
		.ctx  = &ctx,
		.src  = bt_mesh_primary_addr(),
		.xmit = bt_mesh_net_transmit_get(),
	};
	struct bt_mesh_ctl_friend_clear req = {
		.lpn_addr    = sys_cpu_to_be16(frnd->lpn),
		.lpn_counter = sys_cpu_to_be16(frnd->lpn_counter),
	};

	BT_DBG("");

	bt_mesh_ctl_send(&tx, TRANS_CTL_OP_FRIEND_CLEAR, &req,
			 sizeof(req), NULL, &clear_sent_cb, frnd);
}

static void clear_timeout(struct ble_npl_event *work)
{
	struct bt_mesh_friend *frnd = ble_npl_event_get_arg(work);
	u32_t duration;

	BT_DBG("LPN 0x%04x (old) Friend 0x%04x", frnd->lpn, frnd->clear.frnd);

	duration = k_uptime_get_32() - frnd->clear.start;
	if (duration > 2 * frnd->poll_to) {
		BT_DBG("Clear Procedure timer expired");
		frnd->clear.frnd = BT_MESH_ADDR_UNASSIGNED;
		return;
	}

	send_friend_clear(frnd);
}

static void clear_procedure_start(struct bt_mesh_friend *frnd)
{
	BT_DBG("LPN 0x%04x (old) Friend 0x%04x", frnd->lpn, frnd->clear.frnd);

	frnd->clear.start = k_uptime_get_32();
	frnd->clear.repeat_sec = 1U;

	send_friend_clear(frnd);
}

int bt_mesh_friend_clear_cfm(struct bt_mesh_net_rx *rx,
			     struct os_mbuf *buf)
{
	struct bt_mesh_ctl_friend_clear_confirm *msg = (void *)buf->om_data;
	struct bt_mesh_friend *frnd;
	u16_t lpn_addr, lpn_counter;

	BT_DBG("");

	if (buf->om_len < sizeof(*msg)) {
		BT_WARN("Too short Friend Clear Confirm");
		return -EINVAL;
	}

	frnd = find_clear(rx->ctx.addr);
	if (!frnd) {
		BT_WARN("No pending clear procedure for 0x%02x", rx->ctx.addr);
		return 0;
	}

	lpn_addr = sys_be16_to_cpu(msg->lpn_addr);
	if (lpn_addr != frnd->lpn) {
		BT_WARN("LPN address mismatch (0x%04x != 0x%04x)",
			lpn_addr, frnd->lpn);
		return 0;
	}

	lpn_counter = sys_be16_to_cpu(msg->lpn_counter);
	if (lpn_counter != frnd->lpn_counter) {
		BT_WARN("LPN counter mismatch (0x%04x != 0x%04x)",
			lpn_counter, frnd->lpn_counter);
		return 0;
	}

	k_delayed_work_cancel(&frnd->clear.timer);
	frnd->clear.frnd = BT_MESH_ADDR_UNASSIGNED;

	return 0;
}

static void enqueue_offer(struct bt_mesh_friend *frnd, s8_t rssi)
{
	struct bt_mesh_ctl_friend_offer *off;
	struct os_mbuf *sdu = NET_BUF_SIMPLE(1 + sizeof(*off));
	struct os_mbuf *buf;

	BT_DBG("");

	net_buf_simple_init(sdu, 1);

	off = net_buf_simple_add(sdu, sizeof(*off));

	off->recv_win = CONFIG_BT_MESH_FRIEND_RECV_WIN,
	off->queue_size = CONFIG_BT_MESH_FRIEND_QUEUE_SIZE,
	off->sub_list_size = ARRAY_SIZE(frnd->sub_list),
	off->rssi = rssi,
	off->frnd_counter = sys_cpu_to_be16(frnd->counter);

	buf = encode_friend_ctl(frnd, TRANS_CTL_OP_FRIEND_OFFER, sdu);
	if (!buf) {
		BT_ERR("Unable to encode Friend Offer");
		goto done;
	}

	if (encrypt_friend_pdu(frnd, buf, true)) {
		return;
	}

	frnd->counter++;

	if (frnd->last) {
		net_buf_unref(frnd->last);
	}

	frnd->last = buf;
	frnd->send_last = 1;

done:
	os_mbuf_free_chain(sdu);
}

#define RECV_WIN                  CONFIG_BT_MESH_FRIEND_RECV_WIN
#define RSSI_FACT(crit)           (((crit) >> 5) & (u8_t)BIT_MASK(2))
#define RECV_WIN_FACT(crit)       (((crit) >> 3) & (u8_t)BIT_MASK(2))
#define MIN_QUEUE_SIZE_LOG(crit)  ((crit) & (u8_t)BIT_MASK(3))
#define MIN_QUEUE_SIZE(crit)      ((u32_t)BIT(MIN_QUEUE_SIZE_LOG(crit)))

static s32_t offer_delay(struct bt_mesh_friend *frnd, s8_t rssi, u8_t crit)
{
	/* Scaling factors. The actual values are 1, 1.5, 2 & 2.5, but we
	 * want to avoid floating-point arithmetic.
	 */
	static const u8_t fact[] = { 10, 15, 20, 25 };
	s32_t delay;

	BT_DBG("ReceiveWindowFactor %u ReceiveWindow %u RSSIFactor %u RSSI %d",
	       fact[RECV_WIN_FACT(crit)], RECV_WIN,
	       fact[RSSI_FACT(crit)], rssi);

	/* Delay = ReceiveWindowFactor * ReceiveWindow - RSSIFactor * RSSI */
	delay = (s32_t)fact[RECV_WIN_FACT(crit)] * RECV_WIN;
	delay -= (s32_t)fact[RSSI_FACT(crit)] * rssi;
	delay /= 10;

	BT_DBG("Local Delay calculated as %d ms", (int) delay);

	if (delay < 100) {
		return K_MSEC(100);
	}

	return K_MSEC(delay);
}

int bt_mesh_friend_req(struct bt_mesh_net_rx *rx, struct os_mbuf *buf)
{
	struct bt_mesh_ctl_friend_req *msg = (void *)buf->om_data;
	struct bt_mesh_friend *frnd = NULL;
	u32_t poll_to;
	int i;

	if (buf->om_len < sizeof(*msg)) {
		BT_WARN("Too short Friend Request");
		return -EINVAL;
	}

	if (msg->recv_delay <= 0x09) {
		BT_WARN("Prohibited ReceiveDelay (0x%02x)", msg->recv_delay);
		return -EINVAL;
	}

	poll_to = (((u32_t)msg->poll_to[0] << 16) |
		   ((u32_t)msg->poll_to[1] << 8) |
		   ((u32_t)msg->poll_to[2]));

	if (poll_to <= 0x000009 || poll_to >= 0x34bc00) {
		BT_WARN("Prohibited PollTimeout (0x%06x)", (unsigned) poll_to);
		return -EINVAL;
	}

	if (msg->num_elem == 0x00) {
		BT_WARN("Prohibited NumElements value (0x00)");
		return -EINVAL;
	}

	if (!BT_MESH_ADDR_IS_UNICAST(rx->ctx.addr + msg->num_elem - 1)) {
		BT_WARN("LPN elements stretch outside of unicast range");
		return -EINVAL;
	}

	if (!MIN_QUEUE_SIZE_LOG(msg->criteria)) {
		BT_WARN("Prohibited Minimum Queue Size in Friend Request");
		return -EINVAL;
	}

	if (CONFIG_BT_MESH_FRIEND_QUEUE_SIZE < MIN_QUEUE_SIZE(msg->criteria)) {
		BT_WARN("We have a too small Friend Queue size (%u < %u)",
			CONFIG_BT_MESH_FRIEND_QUEUE_SIZE,
			(unsigned) MIN_QUEUE_SIZE(msg->criteria));
		return 0;
	}

	frnd = bt_mesh_friend_find(rx->sub->net_idx, rx->ctx.addr, true, false);
	if (frnd) {
		BT_WARN("Existing LPN re-requesting Friendship");
		friend_clear(frnd);
		goto init_friend;
	}

	for (i = 0; i < ARRAY_SIZE(bt_mesh.frnd); i++) {
		if (!bt_mesh.frnd[i].valid) {
			frnd = &bt_mesh.frnd[i];
			frnd->valid = 1;
			break;
		}
	}

	if (!frnd) {
		BT_WARN("No free Friend contexts for new LPN");
		return -ENOMEM;
	}

init_friend:
	frnd->lpn = rx->ctx.addr;
	frnd->num_elem = msg->num_elem;
	frnd->net_idx = rx->sub->net_idx;
	frnd->recv_delay = msg->recv_delay;
	frnd->poll_to = poll_to * 100;
	frnd->lpn_counter = sys_be16_to_cpu(msg->lpn_counter);
	frnd->clear.frnd = sys_be16_to_cpu(msg->prev_addr);

	BT_DBG("LPN 0x%04x rssi %d recv_delay %u poll_to %ums",
	       frnd->lpn, rx->ctx.recv_rssi, frnd->recv_delay,
	       (unsigned) frnd->poll_to);

	if (BT_MESH_ADDR_IS_UNICAST(frnd->clear.frnd) &&
	    !bt_mesh_elem_find(frnd->clear.frnd)) {
		clear_procedure_start(frnd);
	}

	k_delayed_work_submit(&frnd->timer,
			      offer_delay(frnd, rx->ctx.recv_rssi,
					  msg->criteria));

	friend_cred_create(rx->sub, frnd->lpn, frnd->lpn_counter,
			   frnd->counter);

	enqueue_offer(frnd, rx->ctx.recv_rssi);

	return 0;
}

static bool is_seg(struct bt_mesh_friend_seg *seg, u16_t src, u16_t seq_zero)
{
	struct os_mbuf *buf = (void *)net_buf_slist_peek_head(&seg->queue);
	struct net_buf_simple_state state;
	u16_t buf_seq_zero;
	u16_t buf_src;

	if (!buf) {
		return false;
	}

	net_buf_simple_save(buf, &state);
	net_buf_skip(buf, 5);   /* skip IVI, NID, CTL, TTL, SEQ */
	buf_src = net_buf_pull_be16(buf);
	net_buf_skip(buf, 3);   /* skip DST, OP/AID */
	buf_seq_zero = ((net_buf_pull_be16(buf) >> 2) & TRANS_SEQ_ZERO_MASK);
	net_buf_simple_restore(buf, &state);

	return ((src == buf_src) && (seq_zero == buf_seq_zero));
}

static struct bt_mesh_friend_seg *get_seg(struct bt_mesh_friend *frnd,
					  u16_t src, u16_t seq_zero,
					  u8_t seg_count)
{
	struct bt_mesh_friend_seg *unassigned = NULL;
	int i;

	for (i = 0; i < ARRAY_SIZE(frnd->seg); i++) {
		struct bt_mesh_friend_seg *seg = &frnd->seg[i];

		if (is_seg(seg, src, seq_zero)) {
			return seg;
		}

		if (!unassigned && !net_buf_slist_peek_head(&seg->queue)) {
			unassigned = seg;
		}
	}

	if (unassigned) {
		unassigned->seg_count = seg_count;
	}

	return unassigned;
}

static void enqueue_friend_pdu(struct bt_mesh_friend *frnd,
			       enum bt_mesh_friend_pdu_type type,
			       u16_t src, u8_t seg_count,
			       struct os_mbuf *buf)
{
	struct bt_mesh_friend_seg *seg;

	BT_DBG("type %u", type);

	if (type == BT_MESH_FRIEND_PDU_SINGLE) {
		if (frnd->sec_update) {
			enqueue_update(frnd, 1);
		}

		enqueue_buf(frnd, buf);
		return;
	}

	u16_t seq_zero = (((buf->om_data[10] << 8 | buf->om_data[11]) >> 2) & TRANS_SEQ_ZERO_MASK);

	seg = get_seg(frnd, src, seq_zero, seg_count);
	if (!seg) {
		BT_ERR("No free friend segment RX contexts for 0x%04x", src);
		net_buf_unref(buf);
		return;
	}

	net_buf_slist_put(&seg->queue, buf);

	if (type == BT_MESH_FRIEND_PDU_COMPLETE) {
		if (frnd->sec_update) {
			enqueue_update(frnd, 1);
		}

		net_buf_slist_merge_slist(&frnd->queue, &seg->queue);

		frnd->queue_size += seg->seg_count;
		seg->seg_count = 0U;
	} else {
		/* Mark the buffer as having more to come after it */
		BT_MESH_ADV(buf)->flags |= NET_BUF_FRAGS;
	}
}

static void buf_send_start(u16_t duration, int err, void *user_data)
{
	struct bt_mesh_friend *frnd = user_data;

	BT_DBG("err %d", err);

	frnd->pending_buf = 0;

	/* Friend Offer doesn't follow the re-sending semantics */
	if (!frnd->established) {
		net_buf_unref(frnd->last);
		frnd->last = NULL;
	}
}

static void buf_send_end(int err, void *user_data)
{
	struct bt_mesh_friend *frnd = user_data;

	BT_DBG("err %d", err);

	if (frnd->pending_req) {
		BT_WARN("Another request before previous completed sending");
		return;
	}

	if (frnd->established) {
		k_delayed_work_submit(&frnd->timer, frnd->poll_to);
		BT_DBG("Waiting %u ms for next poll",
		       (unsigned) frnd->poll_to);
	} else {
		/* Friend offer timeout is 1 second */
		k_delayed_work_submit(&frnd->timer, K_SECONDS(1));
		BT_DBG("Waiting for first poll");
	}
}

static void friend_timeout(struct ble_npl_event *work)
{
	struct bt_mesh_friend *frnd = ble_npl_event_get_arg(work);
	static const struct bt_mesh_send_cb buf_sent_cb = {
		.start = buf_send_start,
		.end = buf_send_end,
	};

	__ASSERT_NO_MSG(frnd->pending_buf == 0);

	BT_DBG("lpn 0x%04x send_last %u last %p", frnd->lpn,
	       frnd->send_last, frnd->last);

	if (frnd->send_last && frnd->last) {
		BT_DBG("Sending frnd->last %p", frnd->last);
		frnd->send_last = 0;
		goto send_last;
	}

	if (frnd->established && !frnd->pending_req) {
		BT_WARN("Friendship lost with 0x%04x", frnd->lpn);
		friend_clear(frnd);
		return;
	}

	frnd->last = (void *)net_buf_slist_get(&frnd->queue);
	if (!frnd->last) {
		BT_WARN("Friendship not established with 0x%04x",
			frnd->lpn);
		friend_clear(frnd);
		return;
	}

	if (encrypt_friend_pdu(frnd, frnd->last, false)) {
		return;
	}

	/* Clear the flag we use for segment tracking */
	BT_MESH_ADV(frnd->last)->flags &= ~NET_BUF_FRAGS;

	BT_DBG("Sending buf %p from Friend Queue of LPN 0x%04x",
	       frnd->last, frnd->lpn);
	frnd->queue_size--;

send_last:
	frnd->pending_req = 0;
	frnd->pending_buf = 1;
	bt_mesh_adv_send(frnd->last, &buf_sent_cb, frnd);
}

int bt_mesh_friend_init(void)
{
	int rc;
	int i;

	rc = os_mempool_init(&friend_buf_mempool, FRIEND_BUF_COUNT,
			BT_MESH_ADV_DATA_SIZE + BT_MESH_MBUF_HEADER_SIZE,
			friend_buf_mem, "friend_buf_pool");
	assert(rc == 0);

	rc = os_mbuf_pool_init(&friend_os_mbuf_pool, &friend_buf_mempool,
			BT_MESH_ADV_DATA_SIZE + BT_MESH_MBUF_HEADER_SIZE,
			FRIEND_BUF_COUNT);
	assert(rc == 0);

	for (i = 0; i < ARRAY_SIZE(bt_mesh.frnd); i++) {
		struct bt_mesh_friend *frnd = &bt_mesh.frnd[i];
		int j;

		frnd->net_idx = BT_MESH_KEY_UNUSED;

		net_buf_slist_init(&frnd->queue);

		k_delayed_work_init(&frnd->timer, friend_timeout);
		k_delayed_work_add_arg(&frnd->timer, frnd);
		k_delayed_work_init(&frnd->clear.timer, clear_timeout);
		k_delayed_work_add_arg(&frnd->clear.timer, frnd);

		for (j = 0; j < ARRAY_SIZE(frnd->seg); j++) {
			net_buf_slist_init(&frnd->seg[j].queue);
		}
	}

	return 0;
}

static bool is_segack(struct os_mbuf *buf, u64_t *seqauth, u16_t src)
{
	struct net_buf_simple_state state;
	bool found = false;

	if (buf->om_len != 16) {
		return false;
	}

	net_buf_simple_save(buf, &state);

	net_buf_skip(buf, 1); /* skip IVI, NID */

	if (!(net_buf_pull_u8(buf) >> 7)) {
		goto end;
	}

	net_buf_pull(buf, 3); /* skip SEQNUM */

	if (src != net_buf_pull_be16(buf)) {
		goto end;
	}

	net_buf_skip(buf, 2); /* skip dst */

	if (TRANS_CTL_OP((u8_t *) net_buf_pull_mem(buf, 1)) != TRANS_CTL_OP_ACK) {
		goto end;
	}

	found = ((net_buf_pull_be16(buf) >> 2) & TRANS_SEQ_ZERO_MASK) ==
		(*seqauth & TRANS_SEQ_ZERO_MASK);
end:
	net_buf_simple_restore(buf, &state);
	return found;
}

static void friend_purge_old_ack(struct bt_mesh_friend *frnd, u64_t *seq_auth,
				 u16_t src)
{
	struct os_mbuf *cur, *prev = NULL;

	BT_DBG("SeqAuth %llx src 0x%04x", *seq_auth, src);

	for (cur = net_buf_slist_peek_head(&frnd->queue);
	     cur != NULL; prev = cur, cur = net_buf_slist_peek_next(cur)) {
		struct os_mbuf *buf = (void *)cur;

		if (is_segack(buf, seq_auth, src)) {
			BT_DBG("Removing old ack from Friend Queue");

			net_buf_slist_remove(&frnd->queue, prev, cur);
			frnd->queue_size--;

			net_buf_unref(buf);
			break;
		}
	}
}

static void friend_lpn_enqueue_rx(struct bt_mesh_friend *frnd,
				  struct bt_mesh_net_rx *rx,
				  enum bt_mesh_friend_pdu_type type,
				  u64_t *seq_auth, u8_t seg_count,
				  struct os_mbuf *sbuf)
{
	struct friend_pdu_info info;
	struct os_mbuf *buf;

	/* Because of network loopback, tx packets will also be passed into
	 * this rx function. These packets have already been added to the
	 * queue, and should be ignored.
	 */
	if (bt_mesh_elem_find(rx->ctx.addr)) {
		return;
	}

	BT_DBG("LPN 0x%04x queue_size %u", frnd->lpn,
	       (unsigned) frnd->queue_size);

	if (type == BT_MESH_FRIEND_PDU_SINGLE && seq_auth) {
		friend_purge_old_ack(frnd, seq_auth, rx->ctx.addr);
	}

	info.src = rx->ctx.addr;
	info.dst = rx->ctx.recv_dst;

	if (rx->net_if == BT_MESH_NET_IF_LOCAL) {
		info.ttl = rx->ctx.recv_ttl;
	} else {
		info.ttl = rx->ctx.recv_ttl - 1;
	}

	info.ctl = rx->ctl;

	info.seq[0] = (rx->seq >> 16);
	info.seq[1] = (rx->seq >> 8);
	info.seq[2] = rx->seq;

	info.iv_index = BT_MESH_NET_IVI_RX(rx);

	buf = create_friend_pdu(frnd, &info, sbuf);
	if (!buf) {
		BT_ERR("Failed to encode Friend buffer");
		return;
	}

	enqueue_friend_pdu(frnd, type, info.src, seg_count, buf);

	BT_DBG("Queued message for LPN 0x%04x, queue_size %u",
	       frnd->lpn, (unsigned) frnd->queue_size);
}

static void friend_lpn_enqueue_tx(struct bt_mesh_friend *frnd,
				  struct bt_mesh_net_tx *tx,
				  enum bt_mesh_friend_pdu_type type,
				  u64_t *seq_auth, u8_t seg_count,
				  struct os_mbuf *sbuf)
{
	struct friend_pdu_info info;
	struct os_mbuf *buf;

	BT_DBG("LPN 0x%04x", frnd->lpn);

	if (type == BT_MESH_FRIEND_PDU_SINGLE && seq_auth) {
		friend_purge_old_ack(frnd, seq_auth, tx->src);
	}

	info.src = tx->src;
	info.dst = tx->ctx->addr;

	info.ttl = tx->ctx->send_ttl;
	info.ctl = (tx->ctx->app_idx == BT_MESH_KEY_UNUSED);

	info.seq[0] = (bt_mesh.seq >> 16);
	info.seq[1] = (bt_mesh.seq >> 8);
	info.seq[2] = bt_mesh.seq;

	info.iv_index = BT_MESH_NET_IVI_TX;

	buf = create_friend_pdu(frnd, &info, sbuf);
	if (!buf) {
		BT_ERR("Failed to encode Friend buffer");
		return;
	}

	if (type == BT_MESH_FRIEND_PDU_SINGLE && !info.ctl) {
		/* Unsegmented application packets may be reencrypted later,
		 * as they depend on the the sequence number being the same
		 * when encrypting in transport and network.
		 */
		FRIEND_ADV(buf)->app_idx = tx->ctx->app_idx;
	}

	enqueue_friend_pdu(frnd, type, info.src, seg_count, buf);

	BT_DBG("Queued message for LPN 0x%04x", frnd->lpn);
}

static bool friend_lpn_matches(struct bt_mesh_friend *frnd, u16_t net_idx,
			       u16_t addr)
{
	int i;

	if (!frnd->established) {
		return false;
	}

	if (net_idx != frnd->net_idx) {
		return false;
	}

	if (BT_MESH_ADDR_IS_UNICAST(addr)) {
		return is_lpn_unicast(frnd, addr);
	}

	for (i = 0; i < ARRAY_SIZE(frnd->sub_list); i++) {
		if (frnd->sub_list[i] == addr) {
			return true;
		}
	}

	return false;
}

bool bt_mesh_friend_match(u16_t net_idx, u16_t addr)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(bt_mesh.frnd); i++) {
		struct bt_mesh_friend *frnd = &bt_mesh.frnd[i];

		if (friend_lpn_matches(frnd, net_idx, addr)) {
			BT_DBG("LPN 0x%04x matched address 0x%04x",
			       frnd->lpn, addr);
			return true;
		}
	}

	BT_DBG("No matching LPN for address 0x%04x", addr);

	return false;
}

static bool friend_queue_has_space(struct bt_mesh_friend *frnd, u16_t addr,
				   u64_t *seq_auth, u8_t seg_count)
{
	u32_t total = 0;
	int i;

	if (seg_count > CONFIG_BT_MESH_FRIEND_QUEUE_SIZE) {
		return false;
	}

	for (i = 0; i < ARRAY_SIZE(frnd->seg); i++) {
		struct bt_mesh_friend_seg *seg = &frnd->seg[i];

		if (seq_auth && is_seg(seg, addr, *seq_auth & TRANS_SEQ_ZERO_MASK)) {
			/* If there's a segment queue for this message then the
			 * space verification has already happened.
			 */
			return true;
		}

		total += seg->seg_count;
	}

	/* If currently pending segments combined with this segmented message
	 * are more than the Friend Queue Size, then there's no space. This
	 * is because we don't have a mechanism of aborting already pending
	 * segmented messages to free up buffers.
	 */
	return (CONFIG_BT_MESH_FRIEND_QUEUE_SIZE - total) > seg_count;
}

bool bt_mesh_friend_queue_has_space(u16_t net_idx, u16_t src, u16_t dst,
				    u64_t *seq_auth, u8_t seg_count)
{
	bool someone_has_space = false, friend_match = false;
	int i;

	for (i = 0; i < ARRAY_SIZE(bt_mesh.frnd); i++) {
		struct bt_mesh_friend *frnd = &bt_mesh.frnd[i];

		if (!friend_lpn_matches(frnd, net_idx, dst)) {
			continue;
		}

		friend_match = true;

		if (friend_queue_has_space(frnd, src, seq_auth, seg_count)) {
			someone_has_space = true;
		}
	}

	/* If there were no matched LPNs treat this as success, so the
	 * transport layer can continue its work.
	 */
	if (!friend_match) {
		return true;
	}

	/* From the transport layers perspective it's good enough that at
	 * least one Friend Queue has space. If there were multiple Friend
	 * matches then the destination must be a group address, in which
	 * case e.g. segment acks are not sent.
	 */
	return someone_has_space;
}

static bool friend_queue_prepare_space(struct bt_mesh_friend *frnd, u16_t addr,
				       u64_t *seq_auth, u8_t seg_count)
{
	bool pending_segments;
	u8_t avail_space;

	if (!friend_queue_has_space(frnd, addr, seq_auth, seg_count)) {
		return false;
	}

	avail_space = CONFIG_BT_MESH_FRIEND_QUEUE_SIZE - frnd->queue_size;
	pending_segments = false;

	while (pending_segments || avail_space < seg_count) {
		struct os_mbuf *buf = (void *)net_buf_slist_get(&frnd->queue);

		if (!buf) {
			BT_ERR("Unable to free up enough buffers");
			return false;
		}

		frnd->queue_size--;
		avail_space++;

		pending_segments = (BT_MESH_ADV(buf)->flags & NET_BUF_FRAGS);
		BT_DBG("PENDING SEGMENTS %d", pending_segments);

		/* Make sure old slist entry state doesn't remain */
		BT_MESH_ADV(buf)->flags &= ~NET_BUF_FRAGS;

		net_buf_unref(buf);
	}

	return true;
}

void bt_mesh_friend_enqueue_rx(struct bt_mesh_net_rx *rx,
			       enum bt_mesh_friend_pdu_type type,
			       u64_t *seq_auth, u8_t seg_count,
			       struct os_mbuf *sbuf)
{
	int i;

	if (!rx->friend_match ||
	    (rx->ctx.recv_ttl <= 1 && rx->net_if != BT_MESH_NET_IF_LOCAL) ||
	    bt_mesh_friend_get() != BT_MESH_FRIEND_ENABLED) {
		return;
	}

	BT_DBG("recv_ttl %u net_idx 0x%04x src 0x%04x dst 0x%04x",
	       rx->ctx.recv_ttl, rx->sub->net_idx, rx->ctx.addr,
	       rx->ctx.recv_dst);

	for (i = 0; i < ARRAY_SIZE(bt_mesh.frnd); i++) {
		struct bt_mesh_friend *frnd = &bt_mesh.frnd[i];

		if (!friend_lpn_matches(frnd, rx->sub->net_idx,
					rx->ctx.recv_dst)) {
			continue;
		}

		if (!friend_queue_prepare_space(frnd, rx->ctx.addr, seq_auth,
						seg_count)) {
			continue;
		}

		friend_lpn_enqueue_rx(frnd, rx, type, seq_auth, seg_count,
				      sbuf);
	}
}

bool bt_mesh_friend_enqueue_tx(struct bt_mesh_net_tx *tx,
			       enum bt_mesh_friend_pdu_type type,
			       u64_t *seq_auth, u8_t seg_count,
			       struct os_mbuf *sbuf)
{
	bool matched = false;
	int i;

	if (!bt_mesh_friend_match(tx->sub->net_idx, tx->ctx->addr) ||
	    bt_mesh_friend_get() != BT_MESH_FRIEND_ENABLED) {
		return matched;
	}

	BT_DBG("net_idx 0x%04x dst 0x%04x src 0x%04x", tx->sub->net_idx,
	       tx->ctx->addr, tx->src);

	for (i = 0; i < ARRAY_SIZE(bt_mesh.frnd); i++) {
		struct bt_mesh_friend *frnd = &bt_mesh.frnd[i];

		if (!friend_lpn_matches(frnd, tx->sub->net_idx,
					tx->ctx->addr)) {
			continue;
		}

		if (!friend_queue_prepare_space(frnd, tx->src, seq_auth,
						seg_count)) {
			continue;
		}

		friend_lpn_enqueue_tx(frnd, tx, type, seq_auth, seg_count,
				      sbuf);
		matched = true;
	}

	return matched;
}

void bt_mesh_friend_clear_incomplete(struct bt_mesh_subnet *sub, u16_t src,
				     u16_t dst, u64_t *seq_auth)
{
	int i;

	BT_DBG("");

	for (i = 0; i < ARRAY_SIZE(bt_mesh.frnd); i++) {
		struct bt_mesh_friend *frnd = &bt_mesh.frnd[i];
		int j;

		if (!friend_lpn_matches(frnd, sub->net_idx, dst)) {
			continue;
		}

		for (j = 0; j < ARRAY_SIZE(frnd->seg); j++) {
			struct bt_mesh_friend_seg *seg = &frnd->seg[j];

			if (!is_seg(seg, src, *seq_auth & TRANS_SEQ_ZERO_MASK)) {
				continue;
			}

			BT_WARN("Clearing incomplete segments for 0x%04x", src);

			purge_buffers(&seg->queue);
			seg->seg_count = 0U;
			break;
		}
	}
}

#endif /* MYNEWT_VAL(BLE_MESH_FRIEND) */
