/*  Bluetooth Mesh */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#if MYNEWT_VAL(BLE_MESH)

#define MESH_LOG_MODULE BLE_MESH_NET_LOG

#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include "nimble/porting/nimble/include/os/os_mbuf.h"
#include "../include/mesh/mesh.h"

#include "crypto.h"
#include "adv.h"
#include "mesh_priv.h"
#include "net.h"
#include "rpl.h"
#include "lpn.h"
#include "friend.h"
#include "proxy.h"
#include "transport.h"
#include "access.h"
#include "foundation.h"
#include "beacon.h"
#include "settings.h"
#include "prov.h"
#include "cfg.h"
#include "../include/mesh/glue.h"
#include "../include/mesh/slist.h"

/* Minimum valid Mesh Network PDU length. The Network headers
 * themselves take up 9 bytes. After that there is a minumum of 1 byte
 * payload for both CTL=1 and CTL=0 PDUs (smallest OpCode is 1 byte). CTL=1
 * PDUs must use a 64-bit (8 byte) NetMIC, whereas CTL=0 PDUs have at least
 * a 32-bit (4 byte) NetMIC and AppMIC giving again a total of 8 bytes.
 */
#define BT_MESH_NET_MIN_PDU_LEN (BT_MESH_NET_HDR_LEN + 1 + 8)

#define LOOPBACK_MAX_PDU_LEN (BT_MESH_NET_HDR_LEN + 16)
#define LOOPBACK_USER_DATA_SIZE sizeof(struct bt_mesh_subnet *)
#define LOOPBACK_BUF_SUB(buf) (*(struct bt_mesh_subnet **)net_buf_user_data(buf))

/* Seq limit after IV Update is triggered */
#define IV_UPDATE_SEQ_LIMIT 8000000

#define IVI(pdu)           ((pdu)[0] >> 7)
#define NID(pdu)           ((pdu)[0] & 0x7f)
#define CTL(pdu)           ((pdu)[1] >> 7)
#define TTL(pdu)           ((pdu)[1] & 0x7f)
#define SEQ(pdu)           (sys_get_be24(&pdu[2]))
#define SRC(pdu)           (sys_get_be16(&(pdu)[5]))
#define DST(pdu)           (sys_get_be16(&(pdu)[7]))

static struct {
	uint32_t src : 15, /* MSb of source is always 0 */
	      seq : 17;
} msg_cache[MYNEWT_VAL(BLE_MESH_MSG_CACHE_SIZE)];
static uint16_t msg_cache_next;

/* Singleton network context (the implementation only supports one) */
struct bt_mesh_net bt_mesh = {
	.local_queue = STAILQ_HEAD_INITIALIZER(bt_mesh.local_queue),
};

static struct os_mbuf_pool loopback_os_mbuf_pool;
static struct os_mempool loopback_buf_mempool;
os_membuf_t loopback_mbuf_membuf[
		OS_MEMPOOL_SIZE(LOOPBACK_MAX_PDU_LEN + BT_MESH_MBUF_HEADER_SIZE,
        MYNEWT_VAL(BLE_MESH_LOOPBACK_BUFS))];

static uint32_t dup_cache[MYNEWT_VAL(BLE_MESH_MSG_CACHE_SIZE)];
static int   dup_cache_next;

static bool check_dup(struct os_mbuf *data)
{
	const uint8_t *tail = net_buf_simple_tail(data);
	uint32_t val;
	int i;

	val = sys_get_be32(tail - 4) ^ sys_get_be32(tail - 8);

	for (i = 0; i < ARRAY_SIZE(dup_cache); i++) {
		if (dup_cache[i] == val) {
			return true;
		}
	}

	dup_cache[dup_cache_next++] = val;
	dup_cache_next %= ARRAY_SIZE(dup_cache);

	return false;
}

static bool msg_cache_match(struct os_mbuf *pdu)
{
	uint16_t i;

	for (i = 0; i < ARRAY_SIZE(msg_cache); i++) {
		if (msg_cache[i].src == SRC(pdu->om_data) &&
		    msg_cache[i].seq == (SEQ(pdu->om_data) & BIT_MASK(17))) {
			return true;
		}
	}

	return false;
}

static void msg_cache_add(struct bt_mesh_net_rx *rx)
{
	/* Add to the cache */
	rx->msg_cache_idx = msg_cache_next++;
	msg_cache[rx->msg_cache_idx].src = rx->ctx.addr;
	msg_cache[rx->msg_cache_idx].seq = rx->seq;
	msg_cache_next %= ARRAY_SIZE(msg_cache);
}

int bt_mesh_net_create(uint16_t idx, uint8_t flags, const uint8_t key[16],
		       uint32_t iv_index)
{
	int err;

	BT_DBG("idx %u flags 0x%02x iv_index %u", idx, flags, iv_index);

	BT_DBG("NetKey %s", bt_hex(key, 16));

	if (BT_MESH_KEY_REFRESH(flags)) {
		err = bt_mesh_subnet_set(idx, BT_MESH_KR_PHASE_2, NULL, key);
	} else {
		err = bt_mesh_subnet_set(idx, BT_MESH_KR_NORMAL, key, NULL);
	}

	if (err) {
		BT_ERR("Failed creating subnet");
		return err;
	}

	(void)memset(msg_cache, 0, sizeof(msg_cache));
	msg_cache_next = 0U;

	bt_mesh.iv_index = iv_index;
	atomic_set_bit_to(bt_mesh.flags, BT_MESH_IVU_IN_PROGRESS,
			  BT_MESH_IV_UPDATE(flags));

	/* Set minimum required hours, since the 96-hour minimum requirement
	 * doesn't apply straight after provisioning (since we can't know how
	 * long has actually passed since the network changed its state).
	 */
	bt_mesh.ivu_duration = BT_MESH_IVU_MIN_HOURS;

	if (IS_ENABLED(CONFIG_BT_SETTINGS)) {
		BT_DBG("Storing network information persistently");
		bt_mesh_store_net();
		bt_mesh_store_subnet(idx);
		bt_mesh_store_iv(false);
	}

	return 0;
}

#if MYNEWT_VAL(BLE_MESH_IV_UPDATE_TEST)
void bt_mesh_iv_update_test(bool enable)
{
	atomic_set_bit_to(bt_mesh.flags, BT_MESH_IVU_TEST, enable);
	/* Reset the duration variable - needed for some PTS tests */
	bt_mesh.ivu_duration = 0;
}

bool bt_mesh_iv_update(void)
{
	if (!bt_mesh_is_provisioned()) {
		BT_ERR("Not yet provisioned");
		return false;
	}

	if (atomic_test_bit(bt_mesh.flags, BT_MESH_IVU_IN_PROGRESS)) {
		bt_mesh_net_iv_update(bt_mesh.iv_index, false);
	} else {
		bt_mesh_net_iv_update(bt_mesh.iv_index + 1, true);
	}

	return atomic_test_bit(bt_mesh.flags, BT_MESH_IVU_IN_PROGRESS);
}
#endif /* CONFIG_BT_MESH_IV_UPDATE_TEST */

bool bt_mesh_net_iv_update(uint32_t iv_index, bool iv_update)
{
	if (atomic_test_bit(bt_mesh.flags, BT_MESH_IVU_IN_PROGRESS)) {
		/* We're currently in IV Update mode */

		if (iv_index != bt_mesh.iv_index) {
			BT_WARN("IV Index mismatch: 0x%08x != 0x%08x",
				(unsigned) iv_index,
				(unsigned) bt_mesh.iv_index);
			return false;
		}

		if (iv_update) {
			/* Nothing to do */
			BT_DBG("Already in IV Update in Progress state");
			return false;
		}
	} else {
		/* We're currently in Normal mode */

		if (iv_index == bt_mesh.iv_index) {
			BT_DBG("Same IV Index in normal mode");
			return false;
		}

		if (iv_index < bt_mesh.iv_index ||
		    iv_index > bt_mesh.iv_index + 42) {
			BT_ERR("IV Index out of sync: 0x%08x != 0x%08x",
			       (unsigned) iv_index,
			       (unsigned) bt_mesh.iv_index);
			return false;
		}

		if (iv_index > bt_mesh.iv_index + 1) {
			BT_WARN("Performing IV Index Recovery");
			bt_mesh_rpl_clear();
			bt_mesh.iv_index = iv_index;
			bt_mesh.seq = 0;
			goto do_update;
		}

		if (iv_index == bt_mesh.iv_index + 1 && !iv_update) {
			BT_WARN("Ignoring new index in normal mode");
			return false;
		}

		if (!iv_update) {
			/* Nothing to do */
			BT_DBG("Already in Normal state");
			return false;
		}
	}

	if (!(IS_ENABLED(CONFIG_BT_MESH_IV_UPDATE_TEST) &&
	      atomic_test_bit(bt_mesh.flags, BT_MESH_IVU_TEST))) {
		if (bt_mesh.ivu_duration < BT_MESH_IVU_MIN_HOURS) {
			BT_WARN("IV Update before minimum duration");
			return false;
		}
	}

	/* Defer change to Normal Operation if there are pending acks */
	if (!iv_update && bt_mesh_tx_in_progress()) {
		BT_WARN("IV Update deferred because of pending transfer");
		atomic_set_bit(bt_mesh.flags, BT_MESH_IVU_PENDING);
		return false;
	}

do_update:
	atomic_set_bit_to(bt_mesh.flags, BT_MESH_IVU_IN_PROGRESS, iv_update);
	bt_mesh.ivu_duration = 0U;

	if (iv_update) {
		bt_mesh.iv_index = iv_index;
		BT_DBG("IV Update state entered. New index 0x%08x",
		       (unsigned) bt_mesh.iv_index);

		bt_mesh_rpl_reset();
	} else {
		BT_DBG("Normal mode entered");
		bt_mesh.seq = 0;
	}

	k_delayed_work_submit(&bt_mesh.ivu_timer, BT_MESH_IVU_TIMEOUT);

	/* Notify other modules */
	if (IS_ENABLED(CONFIG_BT_MESH_FRIEND)) {
		bt_mesh_friend_sec_update(BT_MESH_KEY_ANY);
	}

	if (IS_ENABLED(CONFIG_BT_MESH_GATT_PROXY) &&
	    bt_mesh_gatt_proxy_get() == BT_MESH_GATT_PROXY_ENABLED) {
		bt_mesh_proxy_beacon_send(NULL);
	}

	bt_mesh_subnet_foreach(bt_mesh_beacon_update);

	if (MYNEWT_VAL(BLE_MESH_CDB)) {
		bt_mesh_cdb_iv_update(iv_index, iv_update);
	}

	if (IS_ENABLED(CONFIG_BT_SETTINGS)) {
		bt_mesh_store_iv(false);
	}

	return true;
}

uint32_t bt_mesh_next_seq(void)
{
	uint32_t seq = bt_mesh.seq++;

	if (IS_ENABLED(CONFIG_BT_SETTINGS)) {
		bt_mesh_store_seq();
	}

	if (!atomic_test_bit(bt_mesh.flags, BT_MESH_IVU_IN_PROGRESS) &&
	    bt_mesh.seq > IV_UPDATE_SEQ_LIMIT &&
	    bt_mesh_subnet_get(BT_MESH_KEY_PRIMARY)) {
		bt_mesh_beacon_ivu_initiator(true);
		bt_mesh_net_iv_update(bt_mesh.iv_index + 1, true);
	}

	return seq;
}

static void bt_mesh_net_local(struct ble_npl_event *work)
{
	struct os_mbuf *buf;

	while ((buf = net_buf_slist_get(&bt_mesh.local_queue))) {
		struct bt_mesh_subnet *sub = LOOPBACK_BUF_SUB(buf);
		struct bt_mesh_net_rx rx = {
			.ctx = {
				.net_idx = sub->net_idx,
				/* Initialize AppIdx to a sane value */
				.app_idx = BT_MESH_KEY_UNUSED,
				.recv_ttl = TTL(buf->om_data),
				/* TTL=1 only goes to local IF */
				.send_ttl = 1U,
				.addr = SRC(buf->om_data),
				.recv_dst = DST(buf->om_data),
				.recv_rssi = 0,
			},
			.net_if = BT_MESH_NET_IF_LOCAL,
			.sub = sub,
			.old_iv = (IVI(buf->om_data) != (bt_mesh.iv_index & 0x01)),
			.ctl = CTL(buf->om_data),
			.seq = SEQ(buf->om_data),
			.new_key = SUBNET_KEY_TX_IDX(sub),
			.local_match = 1U,
			.friend_match = 0U,
		};

		BT_DBG("src: 0x%04x dst: 0x%04x seq 0x%06x sub %p", rx.ctx.addr,
		       rx.ctx.addr, rx.seq, sub);

		(void) bt_mesh_trans_recv(buf, &rx);
		net_buf_unref(buf);
	}
}

static const struct bt_mesh_net_cred *net_tx_cred_get(struct bt_mesh_net_tx *tx)
{
#if defined(BLE_MESH_LOW_POWER)
	if (tx->friend_cred && bt_mesh_lpn_established()) {
		return &bt_mesh.lpn.cred[SUBNET_KEY_TX_IDX(tx->sub)];
	}
#endif

	tx->friend_cred = 0U;
	return &tx->sub->keys[SUBNET_KEY_TX_IDX(tx->sub)].msg;
}

static int net_header_encode(struct bt_mesh_net_tx *tx, uint8_t nid,
			     struct os_mbuf *buf)
{
	const bool ctl = (tx->ctx->app_idx == BT_MESH_KEY_UNUSED);

	if (ctl && net_buf_simple_tailroom(buf) < 8) {
		BT_ERR("Insufficient MIC space for CTL PDU");
		return -EINVAL;
	} else if (net_buf_simple_tailroom(buf) < 4) {
		BT_ERR("Insufficient MIC space for PDU");
		return -EINVAL;
	}

	BT_DBG("src 0x%04x dst 0x%04x ctl %u seq 0x%06x",
	       tx->src, tx->ctx->addr, ctl, bt_mesh.seq);

	net_buf_simple_push_be16(buf, tx->ctx->addr);
	net_buf_simple_push_be16(buf, tx->src);

	net_buf_simple_push_be24(buf, bt_mesh_next_seq());

	if (ctl) {
		net_buf_simple_push_u8(buf, tx->ctx->send_ttl | 0x80);
	} else {
		net_buf_simple_push_u8(buf, tx->ctx->send_ttl);
	}

	net_buf_simple_push_u8(buf, (nid | (BT_MESH_NET_IVI_TX & 1) << 7));

	return 0;
}

static int net_encrypt(struct os_mbuf *buf,
		       const struct bt_mesh_net_cred *cred, uint32_t iv_index,
		       bool proxy)
{
	int err;

	err = bt_mesh_net_encrypt(cred->enc, buf, iv_index, proxy);
	if (err) {
		return err;
	}

	return bt_mesh_net_obfuscate(buf->om_data, iv_index, cred->privacy);
}

int bt_mesh_net_encode(struct bt_mesh_net_tx *tx, struct os_mbuf *buf,
		       bool proxy)
{
	const struct bt_mesh_net_cred *cred;
	int err;

	cred = net_tx_cred_get(tx);
	err = net_header_encode(tx, cred->nid, buf);
	if (err) {
		return err;
	}

	return net_encrypt(buf, cred, BT_MESH_NET_IVI_TX, proxy);
}

static int loopback(const struct bt_mesh_net_tx *tx, const uint8_t *data,
		    size_t len)
{
	struct os_mbuf *buf;

	buf = os_mbuf_get_pkthdr(&loopback_os_mbuf_pool, 0);
	if (!buf) {
		BT_WARN("Unable to allocate loopback");
		return -ENOMEM;
	}

	BT_DBG("");

	LOOPBACK_BUF_SUB(buf) = tx->sub;

	net_buf_add_mem(buf, data, len);

	net_buf_slist_put(&bt_mesh.local_queue, buf);

	k_work_submit(&bt_mesh.local_work);

	return 0;
}

int bt_mesh_net_send(struct bt_mesh_net_tx *tx, struct os_mbuf *buf,
		     const struct bt_mesh_send_cb *cb, void *cb_data)
{
	const struct bt_mesh_net_cred *cred;
	int err;

	BT_DBG("src 0x%04x dst 0x%04x len %u headroom %zu tailroom %zu",
	       tx->src, tx->ctx->addr, buf->om_len, net_buf_headroom(buf),
	       net_buf_tailroom(buf));
	BT_DBG("Payload len %u: %s", buf->om_len, bt_hex(buf->om_data, buf->om_len));
	BT_DBG("Seq 0x%06x", bt_mesh.seq);

	cred = net_tx_cred_get(tx);
	err = net_header_encode(tx, cred->nid, buf);
	if (err) {
		goto done;
	}

	BT_DBG("encoded %u bytes: %s", buf->om_len,
		   bt_hex(buf->om_data, buf->om_len));

	/* Deliver to local network interface if necessary */
	if (bt_mesh_fixed_group_match(tx->ctx->addr) ||
	    bt_mesh_elem_find(tx->ctx->addr)) {
		err = loopback(tx, buf->om_data, buf->om_len);

		/* Local unicast messages should not go out to network */
		if (BT_MESH_ADDR_IS_UNICAST(tx->ctx->addr) ||
		    tx->ctx->send_ttl == 1U) {
			if (!err) {
				send_cb_finalize(cb, cb_data);
			}

			goto done;
		}
	}
	/* Mesh spec 3.4.5.2: "The output filter of the interface connected to
	 * advertising or GATT bearers shall drop all messages with TTL value
	 * set to 1." If a TTL=1 packet wasn't for a local interface, it is
	 * invalid.
	 */
	if (tx->ctx->send_ttl == 1U) {
		err = -EINVAL;
		goto done;
	}

	err = net_encrypt(buf, cred, BT_MESH_NET_IVI_TX, false);
	if (err) {
		goto done;
	}

	/* Deliver to GATT Proxy Clients if necessary. */
	if (IS_ENABLED(CONFIG_BT_MESH_GATT_PROXY) &&
	    bt_mesh_proxy_relay(buf, tx->ctx->addr) &&
	    BT_MESH_ADDR_IS_UNICAST(tx->ctx->addr)) {
		/* Notify completion if this only went through the Mesh Proxy */
		send_cb_finalize(cb, cb_data);

		err = 0;
		goto done;
	}

	bt_mesh_adv_send(buf, cb, cb_data);

done:
	net_buf_unref(buf);
	return err;
}

void bt_mesh_net_loopback_clear(uint16_t net_idx)
{
	struct net_buf_slist_t new_list;
	struct os_mbuf *buf;

	BT_DBG("0x%04x", net_idx);

	net_buf_slist_init(&new_list);

	while ((buf = net_buf_slist_get(&bt_mesh.local_queue))) {
		struct bt_mesh_subnet *sub = LOOPBACK_BUF_SUB(buf);

		if (net_idx == BT_MESH_KEY_ANY || net_idx == sub->net_idx) {
			BT_DBG("Dropped 0x%06x", SEQ(buf->om_data));
			net_buf_unref(buf);
		} else {
			net_buf_slist_put(&new_list, buf);
		}
	}

	bt_mesh.local_queue = new_list;
}

static bool net_decrypt(struct bt_mesh_net_rx *rx, struct os_mbuf *in,
			struct os_mbuf *out,
			const struct bt_mesh_net_cred *cred)
{
	bool proxy = (rx->net_if == BT_MESH_NET_IF_PROXY_CFG);

	if (NID(in->om_data) != cred->nid) {
		return false;
	}

	BT_DBG("NID 0x%02x", NID(in->om_data));
	BT_DBG("IVI %u net->iv_index 0x%08x", IVI(in->om_data), bt_mesh.iv_index);

	rx->old_iv = (IVI(in->om_data) != (bt_mesh.iv_index & 0x01));
	net_buf_simple_reset(out);
	net_buf_simple_add_mem(out, in->om_data, in->om_len);

	if (bt_mesh_net_obfuscate(out->om_data, BT_MESH_NET_IVI_RX(rx),
				  cred->privacy)) {
		return false;
	}

	rx->ctx.addr = SRC(out->om_data);
	if (!BT_MESH_ADDR_IS_UNICAST(rx->ctx.addr)) {
		BT_DBG("Ignoring non-unicast src addr 0x%04x", rx->ctx.addr);
		return false;
	}

	if (bt_mesh_elem_find(rx->ctx.addr)) {
		BT_DBG("Dropping locally originated packet");
		return false;
	}

	if (rx->net_if == BT_MESH_NET_IF_ADV && msg_cache_match(out)) {
		BT_DBG("Duplicate found in Network Message Cache");
		return false;
	}

	BT_DBG("src 0x%04x", rx->ctx.addr);
	return bt_mesh_net_decrypt(cred->enc, out, BT_MESH_NET_IVI_RX(rx),
				   proxy) == 0;
}

/* Relaying from advertising to the advertising bearer should only happen
 * if the Relay state is set to enabled. Locally originated packets always
 * get sent to the advertising bearer. If the packet came in through GATT,
 * then we should only relay it if the GATT Proxy state is enabled.
 */
static bool relay_to_adv(enum bt_mesh_net_if net_if)
{
	switch (net_if) {
	case BT_MESH_NET_IF_ADV:
		return (bt_mesh_relay_get() == BT_MESH_RELAY_ENABLED);
	case BT_MESH_NET_IF_PROXY:
		return (bt_mesh_gatt_proxy_get() == BT_MESH_GATT_PROXY_ENABLED);
	default:
		return false;
	}
}

static void bt_mesh_net_relay(struct os_mbuf *sbuf,
			      struct bt_mesh_net_rx *rx)
{
	const struct bt_mesh_net_cred *cred;
	struct os_mbuf *buf;
	uint8_t transmit;

	if (rx->ctx.recv_ttl <= 1U) {
		return;
	}

	if (rx->net_if == BT_MESH_NET_IF_ADV &&
	    !rx->friend_cred &&
	    bt_mesh_relay_get() != BT_MESH_RELAY_ENABLED &&
	    bt_mesh_gatt_proxy_get() != BT_MESH_GATT_PROXY_ENABLED) {
		return;
	}

	BT_DBG("TTL %u CTL %u dst 0x%04x", rx->ctx.recv_ttl, rx->ctl,
	       rx->ctx.recv_dst);

	/* The Relay Retransmit state is only applied to adv-adv relaying.
	 * Anything else (like GATT to adv, or locally originated packets)
	 * use the Network Transmit state.
	 */
	if (rx->net_if == BT_MESH_NET_IF_ADV && !rx->friend_cred) {
		transmit = bt_mesh_relay_retransmit_get();
	} else {
		transmit = bt_mesh_net_transmit_get();
	}

	buf = bt_mesh_adv_create(BT_MESH_ADV_DATA, transmit, K_NO_WAIT);
	if (!buf) {
		BT_ERR("Out of relay buffers");
		return;
	}

	/* Leave CTL bit intact */
	sbuf->om_data[1] &= 0x80;
	sbuf->om_data[1] |= rx->ctx.recv_ttl - 1U;

	net_buf_add_mem(buf, sbuf->om_data, sbuf->om_len);

	cred = &rx->sub->keys[SUBNET_KEY_TX_IDX(rx->sub)].msg;

	BT_DBG("Relaying packet. TTL is now %u", TTL(buf->om_data));

	/* Update NID if RX or RX was with friend credentials */
	if (rx->friend_cred) {
		buf->om_data[0] &= 0x80; /* Clear everything except IVI */
		buf->om_data[0] |= cred->nid;
	}

	/* We re-encrypt and obfuscate using the received IVI rather than
	 * the normal TX IVI (which may be different) since the transport
	 * layer nonce includes the IVI.
	 */
	if (net_encrypt(buf, cred, BT_MESH_NET_IVI_RX(rx), false)) {
		BT_ERR("Re-encrypting failed");
		goto done;
	}

	BT_DBG("encoded %u bytes: %s", buf->om_len,
	       bt_hex(buf->om_data, buf->om_len));

	/* When the Friend node relays message for lpn, the message will be
	 * retransmitted using the managed master security credentials and
	 * the Network PDU shall be retransmitted to all network interfaces.
	 */
	if (IS_ENABLED(CONFIG_BT_MESH_GATT_PROXY) &&
	    (rx->friend_cred ||
	     bt_mesh_gatt_proxy_get() == BT_MESH_GATT_PROXY_ENABLED)) {
		bt_mesh_proxy_relay(buf, rx->ctx.recv_dst);
	}

	if (relay_to_adv(rx->net_if) || rx->friend_cred) {
		bt_mesh_adv_send(buf, NULL, NULL);
	}

done:
	net_buf_unref(buf);
}

void bt_mesh_net_header_parse(struct os_mbuf *buf,
			      struct bt_mesh_net_rx *rx)
{
	rx->old_iv = (IVI(buf->om_data) != (bt_mesh.iv_index & 0x01));
	rx->ctl = CTL(buf->om_data);
	rx->ctx.recv_ttl = TTL(buf->om_data);
	rx->seq = SEQ(buf->om_data);
	rx->ctx.addr = SRC(buf->om_data);
	rx->ctx.recv_dst = DST(buf->om_data);
}

int bt_mesh_net_decode(struct os_mbuf *in, enum bt_mesh_net_if net_if,
		       struct bt_mesh_net_rx *rx, struct os_mbuf *out)
{
	if (in->om_len < BT_MESH_NET_MIN_PDU_LEN) {
		BT_WARN("Dropping too short mesh packet (len %u)", in->om_len);
		BT_WARN("%s", bt_hex(in->om_data, in->om_len));
		return -EINVAL;
	}

	if (net_if == BT_MESH_NET_IF_ADV && check_dup(in)) {
		BT_DBG("duplicate packet; dropping %u bytes: %s", in->om_len,
			   bt_hex(in->om_data, in->om_len));
		return -EINVAL;
	}

	BT_DBG("%u bytes: %s", in->om_len, bt_hex(in->om_data, in->om_len));

	rx->net_if = net_if;

	if (!bt_mesh_net_cred_find(rx, in, out, net_decrypt)) {
		BT_DBG("Unable to find matching net for packet");
		return -ENOENT;
	}

	/* Initialize AppIdx to a sane value */
	rx->ctx.app_idx = BT_MESH_KEY_UNUSED;

	rx->ctx.recv_ttl = TTL(out->om_data);

	/* Default to responding with TTL 0 for non-routed messages */
	if (rx->ctx.recv_ttl == 0) {
		rx->ctx.send_ttl = 0;
	} else {
		rx->ctx.send_ttl = BT_MESH_TTL_DEFAULT;
	}

	rx->ctl = CTL(out->om_data);
	rx->seq = SEQ(out->om_data);
	rx->ctx.recv_dst = DST(out->om_data);

	BT_DBG("Decryption successful. Payload len %u: %s", out->om_len,
		   bt_hex(out->om_data, out->om_len));

	if (net_if != BT_MESH_NET_IF_PROXY_CFG &&
	    rx->ctx.recv_dst == BT_MESH_ADDR_UNASSIGNED) {
		BT_ERR("Destination address is unassigned; dropping packet");
		return -EBADMSG;
	}

	BT_DBG("src 0x%04x dst 0x%04x ttl %u", rx->ctx.addr, rx->ctx.recv_dst,
	       rx->ctx.recv_ttl);
	BT_DBG("PDU: %s", bt_hex(out->om_data, out->om_len));

	msg_cache_add(rx);

	return 0;
}

void bt_mesh_net_recv(struct os_mbuf *data, int8_t rssi,
		      enum bt_mesh_net_if net_if)
{
	struct os_mbuf *buf = NET_BUF_SIMPLE(29);
	struct bt_mesh_net_rx rx = { .ctx.recv_rssi = rssi };
	struct net_buf_simple_state state;

	BT_DBG("rssi %d net_if %u", rssi, net_if);

	if (!bt_mesh_is_provisioned()) {
		BT_ERR("Not provisioned; dropping packet");
		goto done;
	}

	if (bt_mesh_net_decode(data, net_if, &rx, buf)) {
		goto done;
	}

	/* Save the state so the buffer can later be relayed */
	net_buf_simple_save(buf, &state);

	rx.local_match = (bt_mesh_fixed_group_match(rx.ctx.recv_dst) ||
			  bt_mesh_elem_find(rx.ctx.recv_dst));

	if ((MYNEWT_VAL(BLE_MESH_GATT_PROXY)) &&
	    net_if == BT_MESH_NET_IF_PROXY) {
		bt_mesh_proxy_addr_add(data, rx.ctx.addr);

		if (bt_mesh_gatt_proxy_get() == BT_MESH_GATT_PROXY_DISABLED &&
		    !rx.local_match) {
			BT_INFO("Proxy is disabled; ignoring message");
			goto done;
		}
	}

	/* The transport layer has indicated that it has rejected the message,
	 * but would like to see it again if it is received in the future.
	 * This can happen if a message is received when the device is in
	 * Low Power mode, but the message was not encrypted with the friend
	 * credentials. Remove it from the message cache so that we accept
	 * it again in the future.
	 */
	if (bt_mesh_trans_recv(buf, &rx) == -EAGAIN) {
		BT_WARN("Removing rejected message from Network Message Cache");
		msg_cache[rx.msg_cache_idx].src = BT_MESH_ADDR_UNASSIGNED;
		/* Rewind the next index now that we're not using this entry */
		msg_cache_next = rx.msg_cache_idx;
	}

	/* Relay if this was a group/virtual address, or if the destination
	 * was neither a local element nor an LPN we're Friends for.
	 */
	if (!BT_MESH_ADDR_IS_UNICAST(rx.ctx.recv_dst) ||
	    (!rx.local_match && !rx.friend_match)) {
		net_buf_simple_restore(buf, &state);
		bt_mesh_net_relay(buf, &rx);
	}

done:
    os_mbuf_free_chain(buf);
}

static void ivu_refresh(struct ble_npl_event *work)
{
	bt_mesh.ivu_duration = MIN(UINT8_MAX,
	       bt_mesh.ivu_duration + BT_MESH_IVU_HOURS);

	BT_DBG("%s for %u hour%s",
	       atomic_test_bit(bt_mesh.flags, BT_MESH_IVU_IN_PROGRESS) ?
	       "IVU in Progress" : "IVU Normal mode",
	       bt_mesh.ivu_duration, bt_mesh.ivu_duration == 1 ? "" : "s");

	if (bt_mesh.ivu_duration < BT_MESH_IVU_MIN_HOURS) {
		if (IS_ENABLED(CONFIG_BT_SETTINGS)) {
			bt_mesh_store_iv(true);
		}

		k_delayed_work_submit(&bt_mesh.ivu_timer, BT_MESH_IVU_TIMEOUT);
		return;
	}

	if (atomic_test_bit(bt_mesh.flags, BT_MESH_IVU_IN_PROGRESS)) {
		bt_mesh_beacon_ivu_initiator(true);
		bt_mesh_net_iv_update(bt_mesh.iv_index, false);
	} else if (IS_ENABLED(CONFIG_BT_SETTINGS)) {
		bt_mesh_store_iv(true);
	}
}

void bt_mesh_net_init(void)
{
	int rc;

	k_delayed_work_init(&bt_mesh.ivu_timer, ivu_refresh);

	k_work_init(&bt_mesh.local_work, bt_mesh_net_local);
	net_buf_slist_init(&bt_mesh.local_queue);

	rc = os_mempool_init(&loopback_buf_mempool, MYNEWT_VAL(BLE_MESH_LOOPBACK_BUFS),
			     LOOPBACK_MAX_PDU_LEN + BT_MESH_MBUF_HEADER_SIZE,
			     &loopback_mbuf_membuf[0], "loopback_buf_pool");
	assert(rc == 0);

	rc = os_mbuf_pool_init(&loopback_os_mbuf_pool, &loopback_buf_mempool,
						   LOOPBACK_MAX_PDU_LEN + BT_MESH_MBUF_HEADER_SIZE,
						   MYNEWT_VAL(BLE_MESH_LOOPBACK_BUFS));
	assert(rc == 0);
}
#endif
