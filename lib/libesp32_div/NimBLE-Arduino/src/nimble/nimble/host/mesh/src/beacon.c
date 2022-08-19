/*  Bluetooth Mesh */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#if MYNEWT_VAL(BLE_MESH)

#define MESH_LOG_MODULE BLE_MESH_BEACON_LOG

#include <errno.h>
#include <assert.h>
#include "nimble/porting/nimble/include/os/os_mbuf.h"
#include "../include/mesh/mesh.h"

#include "adv.h"
#include "mesh_priv.h"
#include "net.h"
#include "prov.h"
#include "crypto.h"
#include "beacon.h"
#include "foundation.h"
#include "atomic.h"

#define PROVISIONED_INTERVAL      (K_SECONDS(10))

#define BEACON_TYPE_UNPROVISIONED  0x00
#define BEACON_TYPE_SECURE         0x01

/* 3 transmissions, 20ms interval */
#define UNPROV_XMIT                BT_MESH_TRANSMIT(2, 20)

/* 1 transmission, 20ms interval */
#define PROV_XMIT                  BT_MESH_TRANSMIT(0, 20)

static struct k_delayed_work beacon_timer;

static int cache_check(struct bt_mesh_subnet *sub, void *beacon_data)
{
	return !memcmp(sub->beacon_cache, beacon_data, 21);
}

static void cache_add(uint8_t data[21], struct bt_mesh_subnet *sub)
{
	memcpy(sub->beacon_cache, data, 21);
}

static void beacon_complete(int err, void *user_data)
{
	struct bt_mesh_subnet *sub = user_data;

	BT_DBG("err %d", err);

	sub->beacon_sent = k_uptime_get_32();
}

void bt_mesh_beacon_create(struct bt_mesh_subnet *sub,
			   struct os_mbuf *buf)
{
	uint8_t flags = bt_mesh_net_flags(sub);
	struct bt_mesh_subnet_keys *keys;

	net_buf_simple_add_u8(buf, BEACON_TYPE_SECURE);

	keys = &sub->keys[SUBNET_KEY_TX_IDX(sub)];

	net_buf_simple_add_u8(buf, flags);

	/* Network ID */
	net_buf_simple_add_mem(buf, keys->net_id, 8);

	/* IV Index */
	net_buf_simple_add_be32(buf, bt_mesh.iv_index);

	net_buf_simple_add_mem(buf, sub->auth, 8);

	BT_DBG("net_idx 0x%04x flags 0x%02x NetID %s", sub->net_idx,
	       flags, bt_hex(keys->net_id, 8));
	BT_DBG("IV Index 0x%08x Auth %s", (unsigned) bt_mesh.iv_index,
	       bt_hex(sub->auth, 8));
}

/* If the interval has passed or is within 5 seconds from now send a beacon */
#define BEACON_THRESHOLD(sub) (K_SECONDS(10 * ((sub)->beacons_last + 1)) - \
			       K_SECONDS(5))

static int secure_beacon_send(struct bt_mesh_subnet *sub, void *cb_data)
{
	static const struct bt_mesh_send_cb send_cb = {
		.end = beacon_complete,
	};
	uint32_t now = k_uptime_get_32();
	struct os_mbuf *buf;
	uint32_t time_diff;

	BT_DBG("");

	time_diff = now - sub->beacon_sent;
	if (time_diff < (600 * MSEC_PER_SEC) &&
		time_diff < BEACON_THRESHOLD(sub)) {
		return 0;
	}

	buf = bt_mesh_adv_create(BT_MESH_ADV_BEACON, PROV_XMIT, K_NO_WAIT);
	if (!buf) {
		BT_ERR("Unable to allocate beacon buffer");
		return -ENOMEM;
	}

	bt_mesh_beacon_create(sub, buf);

	bt_mesh_adv_send(buf, &send_cb, sub);
	net_buf_unref(buf);

	return 0;
}

static int unprovisioned_beacon_send(void)
{
	const struct bt_mesh_prov *prov;
	uint8_t uri_hash[16] = { 0 };
	struct os_mbuf *buf;
	uint16_t oob_info;

	BT_DBG("unprovisioned_beacon_send");

	buf = bt_mesh_adv_create(BT_MESH_ADV_BEACON, UNPROV_XMIT, K_NO_WAIT);
	if (!buf) {
		BT_ERR("Unable to allocate beacon buffer");
		return -ENOBUFS;
	}

	prov = bt_mesh_prov_get();

	net_buf_add_u8(buf, BEACON_TYPE_UNPROVISIONED);
	net_buf_add_mem(buf, prov->uuid, 16);

	if (prov->uri && bt_mesh_s1(prov->uri, uri_hash) == 0) {
		oob_info = prov->oob_info | BT_MESH_PROV_OOB_URI;
	} else {
		oob_info = prov->oob_info;
	}

	net_buf_add_be16(buf, oob_info);
	net_buf_add_mem(buf, uri_hash, 4);

	bt_mesh_adv_send(buf, NULL, NULL);
	net_buf_unref(buf);

	if (prov->uri) {
		size_t len;

		buf = bt_mesh_adv_create(BT_MESH_ADV_URI, UNPROV_XMIT,
					 K_NO_WAIT);
		if (!buf) {
			BT_ERR("Unable to allocate URI buffer");
			return -ENOBUFS;
		}

		len = strlen(prov->uri);
		if (net_buf_tailroom(buf) < len) {
			BT_WARN("Too long URI to fit advertising data");
		} else {
			net_buf_add_mem(buf, prov->uri, len);
			bt_mesh_adv_send(buf, NULL, NULL);
		}

		net_buf_unref(buf);
	}

	return 0;
}

static void unprovisioned_beacon_recv(struct os_mbuf *buf)
{
	const struct bt_mesh_prov *prov;
	uint8_t *uuid;
	uint16_t oob_info;
	uint32_t uri_hash_val;
	uint32_t *uri_hash = NULL;

	if (buf->om_len != 18 && buf->om_len != 22) {
		BT_ERR("Invalid unprovisioned beacon length (%u)", buf->om_len);
		return;
	}

	uuid = net_buf_simple_pull_mem(buf, 16);
	oob_info = net_buf_simple_pull_be16(buf);

	if (buf->om_len == 4) {
		uri_hash_val = net_buf_simple_pull_be32(buf);
		uri_hash = &uri_hash_val;
	}

	BT_DBG("uuid %s", bt_hex(uuid, 16));

	prov = bt_mesh_prov_get();

	if (prov->unprovisioned_beacon) {
		prov->unprovisioned_beacon(uuid,
					   (bt_mesh_prov_oob_info_t)oob_info,
					   uri_hash);
	}
}

static void sub_update_beacon_observation(struct bt_mesh_subnet *sub)
{
	sub->beacons_last = sub->beacons_cur;
	sub->beacons_cur = 0U;
}

static void update_beacon_observation(void)
{
	static bool first_half;

	/* Observation period is 20 seconds, whereas the beacon timer
	 * runs every 10 seconds. We process what's happened during the
	 * window only after the seconnd half.
	 */
	first_half = !first_half;
	if (first_half) {
		return;
	}

	bt_mesh_subnet_foreach(sub_update_beacon_observation);
}

static void beacon_send(struct ble_npl_event *work)
{
	/* Don't send anything if we have an active provisioning link */
	if (IS_ENABLED(CONFIG_BT_MESH_PB_ADV) && bt_mesh_prov_active()) {
		k_delayed_work_submit(&beacon_timer,
							  K_SECONDS(MYNEWT_VAL(BLE_MESH_UNPROV_BEACON_INT)));
		return;
	}

	BT_DBG("");

	if (bt_mesh_is_provisioned()) {
		update_beacon_observation();
		(void)bt_mesh_subnet_find(secure_beacon_send, NULL);

		/* Only resubmit if beaconing is still enabled */
		if (bt_mesh_beacon_enabled() ||
		    atomic_test_bit(bt_mesh.flags, BT_MESH_IVU_INITIATOR)) {
			k_delayed_work_submit(&beacon_timer,
					      PROVISIONED_INTERVAL);
		}

		return;
	}

	if (IS_ENABLED(BLE_MESH_PB_ADV)) {
		unprovisioned_beacon_send();
		k_delayed_work_submit(&beacon_timer,
							  K_SECONDS(MYNEWT_VAL(BLE_MESH_UNPROV_BEACON_INT)));
	}
}

struct beacon_params {
	const uint8_t *net_id;
	const uint8_t *auth;
	uint32_t iv_index;
	uint8_t flags;

	bool new_key;
};

static bool auth_match(struct bt_mesh_subnet_keys *keys,
		       const struct beacon_params *params)
{
	uint8_t net_auth[8];

	if (memcmp(params->net_id, keys->net_id, 8)) {
		return false;
	}

	bt_mesh_beacon_auth(keys->beacon, params->flags, keys->net_id,
			    params->iv_index, net_auth);

	if (memcmp(params->auth, net_auth, 8)) {
		BT_WARN("Authentication Value %s != %s",
			bt_hex(params->auth, 8), bt_hex(net_auth, 8));
		return false;
	}

	return true;
}

static int subnet_by_id(struct bt_mesh_subnet *sub, void *cb_data)
{
	struct beacon_params *params = cb_data;

	for (int i = 0; i < ARRAY_SIZE(sub->keys); i++) {
		if (sub->keys[i].valid && auth_match(&sub->keys[i], params)) {
			params->new_key = (i > 0);
			return true;
		}
	}

	return false;
}

static void secure_beacon_recv(struct os_mbuf *buf)
{
	struct beacon_params params;
	struct bt_mesh_subnet *sub;
	uint8_t *data;

	if (buf->om_len < 21) {
		BT_ERR("Too short secure beacon (len %u)", buf->om_len);
		return;
	}

	sub = bt_mesh_subnet_find(cache_check, buf->om_data);
	if (sub) {
		/* We've seen this beacon before - just update the stats */
		goto update_stats;
	}

	/* So we can add to the cache if auth matches */
	data = buf->om_data;

	params.flags = net_buf_simple_pull_u8(buf);
	params.net_id = net_buf_simple_pull_mem(buf, 8);
	params.iv_index = net_buf_simple_pull_be32(buf);
	params.auth = buf->om_data;

	BT_DBG("flags 0x%02x id %s iv_index 0x%08x",
	       params.flags, bt_hex(params.net_id, 8), params.iv_index);

	sub = bt_mesh_subnet_find(subnet_by_id, &params);
	if (!sub) {
		BT_DBG("No subnet that matched beacon");
		return;
	}

	if (sub->kr_phase == BT_MESH_KR_PHASE_2 && !params.new_key) {
		BT_WARN("Ignoring Phase 2 KR Update secured using old key");
		return;
	}

	cache_add(data, sub);

	bt_mesh_kr_update(sub, BT_MESH_KEY_REFRESH(params.flags),
			  params.new_key);

	/* If we have NetKey0 accept initiation only from it */
	if (bt_mesh_subnet_get(BT_MESH_KEY_PRIMARY) &&
	    sub->net_idx != BT_MESH_KEY_PRIMARY) {
		BT_WARN("Ignoring secure beacon on non-primary subnet");
		goto update_stats;
	}

	BT_DBG("net_idx 0x%04x iv_index 0x%08x, current iv_index 0x%08x",
	       sub->net_idx, params.iv_index, bt_mesh.iv_index);

	if (atomic_test_bit(bt_mesh.flags, BT_MESH_IVU_INITIATOR) &&
	    (atomic_test_bit(bt_mesh.flags, BT_MESH_IVU_IN_PROGRESS) ==
	     BT_MESH_IV_UPDATE(params.flags))) {
		bt_mesh_beacon_ivu_initiator(false);
	}

	bt_mesh_net_iv_update(params.iv_index, BT_MESH_IV_UPDATE(params.flags));

update_stats:
	if (bt_mesh_beacon_enabled() &&
	    sub->beacons_cur < 0xff) {
		sub->beacons_cur++;
	}
}

void bt_mesh_beacon_recv(struct os_mbuf *buf)
{
	uint8_t type;

	BT_DBG("%u bytes: %s", buf->om_len, bt_hex(buf->om_data, buf->om_len));

	if (buf->om_len < 1) {
		BT_ERR("Too short beacon");
		return;
	}

	type = net_buf_simple_pull_u8(buf);
	switch (type) {
	case BEACON_TYPE_UNPROVISIONED:
		if (IS_ENABLED(BLE_MESH_PB_ADV)) {
			unprovisioned_beacon_recv(buf);
		}
		break;
	case BEACON_TYPE_SECURE:
		secure_beacon_recv(buf);
		break;
	default:
		BT_WARN("Unknown beacon type 0x%02x", type);
		break;
	}
}

void bt_mesh_beacon_update(struct bt_mesh_subnet *sub)
{
	uint8_t flags = bt_mesh_net_flags(sub);
	struct bt_mesh_subnet_keys *keys;
	int err;

	keys = &sub->keys[SUBNET_KEY_TX_IDX(sub)];

	BT_DBG("NetIndex 0x%03x Using %s key", sub->net_idx,
	       SUBNET_KEY_TX_IDX(sub) ? "new" : "current");
	BT_DBG("flags 0x%02x, IVI 0x%08x", flags, bt_mesh.iv_index);

	err = bt_mesh_beacon_auth(keys->beacon, flags, keys->net_id,
				   bt_mesh.iv_index, sub->auth);
	if (err) {
		BT_ERR("Failed updating net beacon for 0x%03x", sub->net_idx);
	}
}

static void subnet_evt(struct bt_mesh_subnet *sub, enum bt_mesh_key_evt evt)
{
	if (evt != BT_MESH_KEY_DELETED) {
		bt_mesh_beacon_update(sub);
	}
}

void bt_mesh_beacon_init(void)
{
		if (!bt_mesh_subnet_cb_list[1]) {
		bt_mesh_subnet_cb_list[1] = subnet_evt;
	}

	k_delayed_work_init(&beacon_timer, beacon_send);
}

void bt_mesh_beacon_ivu_initiator(bool enable)
{
	atomic_set_bit_to(bt_mesh.flags, BT_MESH_IVU_INITIATOR, enable);

	if (enable) {
		k_delayed_work_submit(&beacon_timer, K_NO_WAIT);
	} else if (!bt_mesh_beacon_enabled()) {
		k_delayed_work_cancel(&beacon_timer);
	}
}

static void subnet_beacon_enable(struct bt_mesh_subnet *sub)
{
	sub->beacons_last = 0U;
	sub->beacons_cur = 0U;

	bt_mesh_beacon_update(sub);
}

void bt_mesh_beacon_enable(void)
{
	if (!bt_mesh_is_provisioned()) {
		k_delayed_work_submit(&beacon_timer, K_NO_WAIT);
		return;
	}

	bt_mesh_subnet_foreach(subnet_beacon_enable);

	k_delayed_work_submit(&beacon_timer, K_NO_WAIT);
}

void bt_mesh_beacon_disable(void)
{
	if (!atomic_test_bit(bt_mesh.flags, BT_MESH_IVU_INITIATOR)) {
		k_delayed_work_cancel(&beacon_timer);
	}
}
#endif
