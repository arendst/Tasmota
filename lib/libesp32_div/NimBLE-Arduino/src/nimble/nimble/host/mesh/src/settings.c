/*
 * Copyright (c) 2018 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#if MYNEWT_VAL(BLE_MESH)

#define MESH_LOG_MODULE BLE_MESH_SETTINGS_LOG

#if MYNEWT_VAL(BLE_MESH_SETTINGS)

#include "mesh_priv.h"
#include "../include/mesh/mesh.h"
#include "../include/mesh/glue.h"
#include "subnet.h"
#include "app_keys.h"
#include "net.h"
#include "rpl.h"
#include "crypto.h"
#include "transport.h"
#include "heartbeat.h"
#include "access.h"
#include "foundation.h"
#include "proxy.h"
#include "settings.h"
#include "lpn.h"
#include "cfg.h"


#include "config/config.h"

/* Tracking of what storage changes are pending for App and Net Keys. We
 * track this in a separate array here instead of within the respective
 * bt_mesh_app_key and bt_mesh_subnet structs themselves, since once a key
 * gets deleted its struct becomes invalid and may be reused for other keys.
 */
struct key_update {
	uint16_t key_idx:12,    /* AppKey or NetKey Index */
	      valid:1,       /* 1 if this entry is valid, 0 if not */
	      app_key:1,     /* 1 if this is an AppKey, 0 if a NetKey */
	      clear:1;       /* 1 if key needs clearing, 0 if storing */
};

static struct key_update key_updates[CONFIG_BT_MESH_APP_KEY_COUNT +
				     CONFIG_BT_MESH_SUBNET_COUNT];

static struct k_delayed_work pending_store;

/* Mesh network storage information */
struct net_val {
	uint16_t primary_addr;
	uint8_t  dev_key[16];
} __packed;

/* Sequence number storage */
struct seq_val {
	uint8_t val[3];
} __packed;

/* Heartbeat Publication storage */
struct hb_pub_val {
	uint16_t dst;
	uint8_t  period;
	uint8_t  ttl;
	uint16_t feat;
	uint16_t net_idx:12,
	      indefinite:1;
};

/* Miscelaneous configuration server model states */
struct cfg_val {
	uint8_t net_transmit;
	uint8_t relay;
	uint8_t relay_retransmit;
	uint8_t beacon;
	uint8_t gatt_proxy;
	uint8_t frnd;
	uint8_t default_ttl;
};

/* IV Index & IV Update storage */
struct iv_val {
	uint32_t iv_index;
	uint8_t  iv_update:1,
	      iv_duration:7;
} __packed;

/* Replay Protection List storage */
struct rpl_val {
	uint32_t seq:24,
	      old_iv:1;
};

/* NetKey storage information */
struct net_key_val {
	uint8_t kr_flag:1,
	     kr_phase:7;
	uint8_t val[2][16];
} __packed;

/* AppKey storage information */
struct app_key_val {
	uint16_t net_idx;
	bool  updated;
	uint8_t  val[2][16];
} __packed;

struct mod_pub_val {
	uint16_t addr;
	uint16_t key;
	uint8_t  ttl;
	uint8_t  retransmit;
	uint8_t  period;
	uint8_t  period_div:4,
	      cred:1;
};

/* Virtual Address information */
struct va_val {
	uint16_t ref;
	uint16_t addr;
	uint8_t uuid[16];
} __packed;

struct cdb_net_val {
	uint32_t iv_index;
	bool  iv_update;
} __packed;

/* Node storage information */
struct node_val {
	uint16_t net_idx;
	uint8_t  num_elem;
	uint8_t  flags;
#define F_NODE_CONFIGURED 0x01
	uint8_t  uuid[16];
	uint8_t  dev_key[16];
} __packed;

struct node_update {
	uint16_t addr;
	bool clear;
};

#if MYNEWT_VAL(BLE_MESH_CDB)
static struct node_update cdb_node_updates[MYNEWT_VAL(BLE_MESH_CDB_NODE_COUNT)];
static struct key_update cdb_key_updates[
					MYNEWT_VAL(BLE_MESH_CDB_SUBNET_COUNT) +
					MYNEWT_VAL(BLE_MESH_CDB_APP_KEY_COUNT)];
#endif

int settings_name_next(char *name, char **next)
{
	int rc = 0;

	if (next) {
		*next = NULL;
	}

	if (!name) {
		return 0;
	}

	/* name might come from flash directly, in flash the name would end
	 * with '=' or '\0' depending how storage is done. Flash reading is
	 * limited to what can be read
	 */
	while ((*name != '\0') && (*name != '=') &&
	       (*name != '/')) {
		rc++;
		name++;
	}

	if (*name == '/') {
		if (next) {
			*next = name + 1;
		}
		return rc;
	}

	return rc;
}

static int net_set(int argc, char **argv, char *val)
{
	struct net_val net;
	int len, err;

	BT_DBG("val %s", val ? val : "(null)");

	if (!val) {
		bt_mesh_comp_unprovision();
		memset(bt_mesh.dev_key, 0, sizeof(bt_mesh.dev_key));
		return 0;
	}

	len = sizeof(net);
	err = settings_bytes_from_str(val, &net, &len);
	if (err) {
		BT_ERR("Failed to decode value %s (err %d)", val, err);
		return err;
	}

	if (len != sizeof(net)) {
		BT_ERR("Unexpected value length (%d != %zu)", len, sizeof(net));
		return -EINVAL;
	}

	memcpy(bt_mesh.dev_key, net.dev_key, sizeof(bt_mesh.dev_key));
	bt_mesh_comp_provision(net.primary_addr);

	BT_DBG("Provisioned with primary address 0x%04x", net.primary_addr);
	BT_DBG("Recovered DevKey %s", bt_hex(bt_mesh.dev_key, 16));

	return 0;
}

static int iv_set(int argc, char **argv, char *val)
{
	struct iv_val iv;
	int len, err;

	BT_DBG("val %s", val ? val : "(null)");

	if (!val) {
		bt_mesh.iv_index = 0U;
		atomic_clear_bit(bt_mesh.flags, BT_MESH_IVU_IN_PROGRESS);
		return 0;
	}

	len = sizeof(iv);
	err = settings_bytes_from_str(val, &iv, &len);
	if (err) {
		BT_ERR("Failed to decode value %s (err %d)", val, err);
		return err;
	}

	if (len != sizeof(iv)) {
		BT_ERR("Unexpected value length (%d != %zu)", len, sizeof(iv));
		return -EINVAL;
	}

	bt_mesh.iv_index = iv.iv_index;
	atomic_set_bit_to(bt_mesh.flags, BT_MESH_IVU_IN_PROGRESS, iv.iv_update);
	bt_mesh.ivu_duration = iv.iv_duration;

	BT_DBG("IV Index 0x%04x (IV Update Flag %u) duration %u hours",
	       (unsigned) iv.iv_index, iv.iv_update, iv.iv_duration);

	return 0;
}

static int seq_set(int argc, char **argv, char *val)
{
	struct seq_val seq;
	int len, err;

	BT_DBG("val %s", val ? val : "(null)");

	if (!val) {
		bt_mesh.seq = 0;
		return 0;
	}

	len = sizeof(seq);
	err = settings_bytes_from_str(val, &seq, &len);
	if (err) {
		BT_ERR("Failed to decode value %s (err %d)", val, err);
		return err;
	}

	if (len != sizeof(seq)) {
		BT_ERR("Unexpected value length (%d != %zu)", len, sizeof(seq));
		return -EINVAL;
	}

	bt_mesh.seq = sys_get_le24(seq.val);

	if (CONFIG_BT_MESH_SEQ_STORE_RATE > 0) {
		/* Make sure we have a large enough sequence number. We
		 * subtract 1 so that the first transmission causes a write
		 * to the settings storage.
		 */
		bt_mesh.seq += (CONFIG_BT_MESH_SEQ_STORE_RATE -
				(bt_mesh.seq % CONFIG_BT_MESH_SEQ_STORE_RATE));
		bt_mesh.seq--;
	}

	BT_DBG("Sequence Number 0x%06x", bt_mesh.seq);

	return 0;
}

static int rpl_set(int argc, char **argv, char *val)
{
	struct bt_mesh_rpl *entry;
	struct rpl_val rpl;
	int len, err;
	uint16_t src;

	if (argc < 1) {
		BT_ERR("Invalid argc (%d)", argc);
		return -ENOENT;
	}

	BT_DBG("argv[0] %s val %s", argv[0], val ? val : "(null)");

	src = strtol(argv[0], NULL, 16);
	entry = bt_mesh_rpl_find(src);

	if (!val) {
		if (entry) {
			memset(entry, 0, sizeof(*entry));
		} else {
			BT_WARN("Unable to find RPL entry for 0x%04x", src);
		}

		return 0;
	}

	if (!entry) {
		entry = bt_mesh_rpl_alloc(src);
		if (!entry) {
			BT_ERR("Unable to allocate RPL entry for 0x%04x", src);
			return -ENOMEM;
		}
	}

	len = sizeof(rpl);
	err = settings_bytes_from_str(val, &rpl, &len);
	if (err) {
		BT_ERR("Failed to decode value %s (err %d)", val, err);
		return err;
	}

	if (len != sizeof(rpl)) {
		BT_ERR("Unexpected value length (%d != %zu)", len, sizeof(rpl));
		return -EINVAL;
	}

	entry->seq = rpl.seq;
	entry->old_iv = rpl.old_iv;

	BT_DBG("RPL entry for 0x%04x: Seq 0x%06x old_iv %u", entry->src,
	       (unsigned) entry->seq, entry->old_iv);

	return 0;
}

static int net_key_set(int argc, char **argv, char *val)
{
	struct net_key_val key;
	int len, err;
	uint16_t net_idx;

	BT_DBG("argv[0] %s val %s", argv[0], val ? val : "(null)");

	net_idx = strtol(argv[0], NULL, 16);

	len = sizeof(key);
	err = settings_bytes_from_str(val, &key, &len);
	if (err) {
		BT_ERR("Failed to decode value %s (err %d)", val, err);
		return err;
	}

	if (len != sizeof(key)) {
		BT_ERR("Unexpected value length (%d != %zu)", len, sizeof(key));
		return -EINVAL;
	}

	BT_DBG("NetKeyIndex 0x%03x recovered from storage", net_idx);

	return bt_mesh_subnet_set(
		net_idx, key.kr_phase, key.val[0],
		(key.kr_phase != BT_MESH_KR_NORMAL) ? key.val[1] : NULL);
}

static int app_key_set(int argc, char **argv, char *val)
{
	struct app_key_val key;
	uint16_t app_idx;
	int len_rd, err;

	BT_DBG("argv[0] %s val %s", argv[0], val ? val : "(null)");

	app_idx = strtol(argv[0], NULL, 16);
	len_rd = strtol(argv[1], NULL, 16);

	if (!len_rd) {
		return 0;
	}

	err = settings_bytes_from_str(val, &key, &len_rd);
	if (err) {
		BT_ERR("Failed to decode value %s (err %d)", val, err);
		return err;
	}

	err = bt_mesh_app_key_set(app_idx, key.net_idx, key.val[0],
			      key.updated ? key.val[1] : NULL);
	if (err) {
		BT_ERR("Failed to set \'app-key\'");
		return err;
	}

	BT_DBG("AppKeyIndex 0x%03x recovered from storage", app_idx);

	return 0;
}

static int hb_pub_set(int argc, char **argv, char *val)
{
	struct bt_mesh_hb_pub pub;
	struct hb_pub_val hb_val;
	int len, err;

	BT_DBG("val %s", val ? val : "(null)");

	len = sizeof(hb_val);
	err = settings_bytes_from_str(val, &hb_val, &len);
	if (err) {
		BT_ERR("Failed to decode value %s (err %d)", val, err);
		return err;
	}

	if (len != sizeof(hb_val)) {
		BT_ERR("Unexpected value length (%d != %zu)", len,
		       sizeof(hb_val));
		return -EINVAL;
	}

	pub.dst = hb_val.dst;
	pub.period = bt_mesh_hb_pwr2(hb_val.period);
	pub.ttl = hb_val.ttl;
	pub.feat = hb_val.feat;
	pub.net_idx = hb_val.net_idx;

	if (hb_val.indefinite) {
		pub.count = 0xffff;
	} else {
		pub.count = 0;
	}

	(void)bt_mesh_hb_pub_set(&pub);

	BT_DBG("Restored heartbeat publication");

	return 0;
}

static int cfg_set(int argc, char **argv, char *val)
{
	struct cfg_val cfg;
	int len, err;

	BT_DBG("val %s", val ? val : "(null)");

	if (!val) {
		BT_DBG("Cleared configuration state");
		return 0;
	}

	len = sizeof(cfg);
	err = settings_bytes_from_str(val, &cfg, &len);
	if (err) {
		BT_ERR("Failed to decode value %s (err %d)", val, err);
		return err;
	}

	if (len != sizeof(cfg)) {
		BT_ERR("Unexpected value length (%d != %zu)", len,
		       sizeof(cfg));
		return -EINVAL;
	}

	bt_mesh_net_transmit_set(cfg.net_transmit);
	bt_mesh_relay_set(cfg.relay, cfg.relay_retransmit);
	bt_mesh_beacon_set(cfg.beacon);
	bt_mesh_gatt_proxy_set(cfg.gatt_proxy);
	bt_mesh_friend_set(cfg.frnd);
	bt_mesh_default_ttl_set(cfg.default_ttl);

	BT_DBG("Restored configuration state");

	return 0;
}

static int mod_set_bind(struct bt_mesh_model *mod, char *val)
{
	int len, err, i;

	/* Start with empty array regardless of cleared or set value */
	for (i = 0; i < ARRAY_SIZE(mod->keys); i++) {
		mod->keys[i] = BT_MESH_KEY_UNUSED;
	}

	if (!val) {
		BT_DBG("Cleared bindings for model");
		return 0;
	}

	len = sizeof(mod->keys);
	err = settings_bytes_from_str(val, mod->keys, &len);
	if (err) {
		BT_ERR("Failed to decode value %s (err %d)", val, err);
		return -EINVAL;
	}

	BT_DBG("Decoded %u bound keys for model", len / sizeof(mod->keys[0]));
	return 0;
}

static int mod_set_sub(struct bt_mesh_model *mod, char *val)
{
	int len, err;

	/* Start with empty array regardless of cleared or set value */
	memset(mod->groups, 0, sizeof(mod->groups));

	if (!val) {
		BT_DBG("Cleared subscriptions for model");
		return 0;
	}

	len = sizeof(mod->groups);
	err = settings_bytes_from_str(val, mod->groups, &len);
	if (err) {
		BT_ERR("Failed to decode value %s (err %d)", val, err);
		return -EINVAL;
	}

	BT_DBG("Decoded %u subscribed group addresses for model",
	       len / sizeof(mod->groups[0]));
	return 0;
}

static int mod_set_pub(struct bt_mesh_model *mod, char *val)
{
	struct mod_pub_val pub;
	int len, err;

	if (!mod->pub) {
		BT_WARN("Model has no publication context!");
		return -EINVAL;
	}

	if (!val) {
		mod->pub->addr = BT_MESH_ADDR_UNASSIGNED;
		mod->pub->key = 0;
		mod->pub->cred = 0;
		mod->pub->ttl = 0;
		mod->pub->period = 0;
		mod->pub->retransmit = 0;
		mod->pub->count = 0;

		BT_DBG("Cleared publication for model");
		return 0;
	}

	len = sizeof(pub);
	err = settings_bytes_from_str(val, &pub, &len);
	if (err) {
		BT_ERR("Failed to decode value %s (err %d)", val, err);
		return -EINVAL;
	}

	if (len != sizeof(pub)) {
		BT_ERR("Invalid length for model publication");
		return -EINVAL;
	}

	mod->pub->addr = pub.addr;
	mod->pub->key = pub.key;
	mod->pub->cred = pub.cred;
	mod->pub->ttl = pub.ttl;
	mod->pub->period = pub.period;
	mod->pub->retransmit = pub.retransmit;
	mod->pub->count = 0;

	BT_DBG("Restored model publication, dst 0x%04x app_idx 0x%03x",
	       pub.addr, pub.key);

	return 0;
}

static int mod_data_set(struct bt_mesh_model *mod,
			char *name, char *len_rd)
{
	char *next;

	settings_name_next(name, &next);

	if (mod->cb && mod->cb->settings_set) {
		return mod->cb->settings_set(mod, next, len_rd);
	}

	return 0;
}

static int mod_set(bool vnd, int argc, char **argv, char *val)
{
	struct bt_mesh_model *mod;
	uint8_t elem_idx, mod_idx;
	uint16_t mod_key;

	if (argc < 2) {
		BT_ERR("Too small argc (%d)", argc);
		return -ENOENT;
	}

	mod_key = strtol(argv[0], NULL, 16);
	elem_idx = mod_key >> 8;
	mod_idx = mod_key;

	BT_DBG("Decoded mod_key 0x%04x as elem_idx %u mod_idx %u",
	       mod_key, elem_idx, mod_idx);

	mod = bt_mesh_model_get(vnd, elem_idx, mod_idx);
	if (!mod) {
		BT_ERR("Failed to get model for elem_idx %u mod_idx %u",
		       elem_idx, mod_idx);
		return -ENOENT;
	}

	if (!strcmp(argv[1], "bind")) {
		return mod_set_bind(mod, val);
	}

	if (!strcmp(argv[1], "sub")) {
		return mod_set_sub(mod, val);
	}

	if (!strcmp(argv[1], "pub")) {
		return mod_set_pub(mod, val);
	}

	if (!strcmp(argv[1], "data")) {
		return mod_data_set(mod, argv[1], val);	
	}

	BT_WARN("Unknown module key %s", argv[1]);
	return -ENOENT;
}

static int sig_mod_set(int argc, char **argv, char *val)
{
	return mod_set(false, argc, argv, val);
}

static int vnd_mod_set(int argc, char **argv, char *val)
{
	return mod_set(true, argc, argv, val);
}

#if CONFIG_BT_MESH_LABEL_COUNT > 0
static int va_set(int argc, char **argv, char *val)
{
	struct va_val va;
	struct bt_mesh_va *lab;
	uint16_t index;
	int len, err;

	if (argc < 1) {
		BT_ERR("Insufficient number of arguments");
		return -ENOENT;
	}

	index = strtol(argv[0], NULL, 16);

	if (val == NULL) {
		BT_WARN("Mesh Virtual Address length = 0");
		return 0;
	}

	err = settings_bytes_from_str(val, &va, &len);
	if (err) {
		BT_ERR("Failed to decode value %s (err %d)", val, err);
		return -EINVAL;
	}

	if (len != sizeof(struct va_val)) {
		BT_ERR("Invalid length for virtual address");
		return -EINVAL;
	}

	if (va.ref == 0) {
		BT_WARN("Ignore Mesh Virtual Address ref = 0");
		return 0;
	}

	lab = bt_mesh_va_get(index);
	if (lab == NULL) {
		BT_WARN("Out of labels buffers");
		return -ENOBUFS;
	}

	memcpy(lab->uuid, va.uuid, 16);
	lab->addr = va.addr;
	lab->ref = va.ref;

	BT_DBG("Restored Virtual Address, addr 0x%04x ref 0x%04x",
	       lab->addr, lab->ref);

	return 0;
}
#endif

#if MYNEWT_VAL(BLE_MESH_CDB)
static int cdb_net_set(int argc, char *val)
{
	struct cdb_net_val net;
	int len, err;

	len = sizeof(net);
	err = settings_bytes_from_str(val, &net, &len);
	if (err) {
		BT_ERR("Failed to set \'cdb_net\'");
		return err;
	}

	bt_mesh_cdb.iv_index = net.iv_index;

	if (net.iv_update) {
		atomic_set_bit(bt_mesh_cdb.flags, BT_MESH_CDB_IVU_IN_PROGRESS);
	}

	atomic_set_bit(bt_mesh_cdb.flags, BT_MESH_CDB_VALID);

	return 0;
}

static int cdb_node_set(int argc, char *str)
{
	struct bt_mesh_cdb_node *node;
	struct node_val val;
	uint16_t addr;
	int len, err;

	if (argc < 1) {
		BT_ERR("Insufficient number of arguments");
		return -ENOENT;
	}

	addr = strtol(str, NULL, 16);
	len = sizeof(str);

	if (argc < 1) {
		BT_DBG("val (null)");
		BT_DBG("Deleting node 0x%04x", addr);

		node = bt_mesh_cdb_node_get(addr);
		if (node) {
			bt_mesh_cdb_node_del(node, false);
		}

		return 0;
	}

	err = settings_bytes_from_str(str, &val, &len);
	if (err) {
		BT_ERR("Failed to decode value %s (err %d)", val, err);
		return -EINVAL;
	}

	if (len != sizeof(struct node_val)) {
		BT_ERR("Invalid length for node_val");
		return -EINVAL;
	}

	node = bt_mesh_cdb_node_get(addr);
	if (!node) {
		node = bt_mesh_cdb_node_alloc(val.uuid, addr, val.num_elem,
					      			  val.net_idx);
	}

	if (!node) {
		BT_ERR("No space for a new node");
		return -ENOMEM;
	}

	if (val.flags & F_NODE_CONFIGURED) {
		atomic_set_bit(node->flags, BT_MESH_CDB_NODE_CONFIGURED);
	}

	memcpy(node->uuid, val.uuid, 16);
	memcpy(node->dev_key, val.dev_key, 16);

	BT_DBG("Node 0x%04x recovered from storage", addr);

	return 0;
}

static int cdb_subnet_set(int argc, char *name)
{
	struct bt_mesh_cdb_subnet *sub;
	struct net_key_val key;
	uint16_t net_idx;
	int len, len_rd, err;

	if (!name) {
		BT_ERR("Insufficient number of arguments");
		return -ENOENT;
	}

	len_rd = sizeof(sub);
	net_idx = strtol(name, NULL, 16);
	sub = bt_mesh_cdb_subnet_get(net_idx);

	if (len_rd == 0) {
		BT_DBG("val (null)");
		if (!sub) {
			BT_ERR("No subnet with NetKeyIndex 0x%03x", net_idx);
			return -ENOENT;
		}

		BT_DBG("Deleting NetKeyIndex 0x%03x", net_idx);
		bt_mesh_cdb_subnet_del(sub, false);
		return 0;
	}

	len = sizeof(key);
	err = settings_bytes_from_str(name, &key, &len);
	if (err) {
		BT_ERR("Failed to set \'net-key\'");
		return err;
	}

	if (sub) {
		BT_DBG("Updating existing NetKeyIndex 0x%03x", net_idx);

		sub->kr_flag = key.kr_flag;
		sub->kr_phase = key.kr_phase;
		memcpy(sub->keys[0].net_key, &key.val[0], 16);
		memcpy(sub->keys[1].net_key, &key.val[1], 16);

		return 0;
	}

	sub = bt_mesh_cdb_subnet_alloc(net_idx);
	if (!sub) {
		BT_ERR("No space to allocate a new subnet");
		return -ENOMEM;
	}

	sub->kr_flag = key.kr_flag;
	sub->kr_phase = key.kr_phase;
	memcpy(sub->keys[0].net_key, &key.val[0], 16);
	memcpy(sub->keys[1].net_key, &key.val[1], 16);

	BT_DBG("NetKeyIndex 0x%03x recovered from storage", net_idx);

	return 0;
}

static int cdb_app_key_set(int argc, char *name)
{
	struct bt_mesh_cdb_app_key *app;
	struct app_key_val key;
	uint16_t app_idx;
	int len_rd, err;

	app_idx = strtol(name, NULL, 16);
	len_rd = sizeof(key);

	if (len_rd == 0) {
		BT_DBG("val (null)");
		BT_DBG("Deleting AppKeyIndex 0x%03x", app_idx);

		app = bt_mesh_cdb_app_key_get(app_idx);
		if (app) {
			bt_mesh_cdb_app_key_del(app, false);
		}

		return 0;
	}

	err = settings_bytes_from_str(name, &key, &len_rd);
	if (err) {
		BT_ERR("Failed to set \'app-key\'");
		return err;
	}

	app = bt_mesh_cdb_app_key_get(app_idx);
	if (!app) {
		app = bt_mesh_cdb_app_key_alloc(key.net_idx, app_idx);
	}

	if (!app) {
		BT_ERR("No space for a new app key");
		return -ENOMEM;
	}

	memcpy(app->keys[0].app_key, key.val[0], 16);
	memcpy(app->keys[1].app_key, key.val[1], 16);

	BT_DBG("AppKeyIndex 0x%03x recovered from storage", app_idx);

	return 0;
}

static int cdb_set(int argc, char **argv, char *name)
{
	int len;
	char *next;

	if (argc < 1) {
		BT_ERR("Insufficient number of arguments");
		return -ENOENT;
	}

	if (!strcmp(name, "Net")) {
		return cdb_net_set(1, name);
	}


	len = settings_name_next(name, &next);

	if (!next) {
		BT_ERR("Insufficient number of arguments");
		return -ENOENT;
	}

	if (!strncmp(name, "Node", len)) {
		return cdb_node_set(1, next);
	}

	if (!strncmp(name, "Subnet", len)) {
		return cdb_subnet_set(1, next);
	}

	if (!strncmp(name, "AppKey", len)) {
		return cdb_app_key_set(1, next);
	}

	BT_WARN("Unknown module key %s", name);
	return -ENOENT;
}
#endif

const struct mesh_setting {
	const char *name;
	int (*func)(int argc, char **argv, char *val);
} settings[] = {
	{ "Net", net_set },
	{ "IV", iv_set },
	{ "Seq", seq_set },
	{ "RPL", rpl_set },
	{ "NetKey", net_key_set },
	{ "AppKey", app_key_set },
	{ "HBPub", hb_pub_set },
	{ "Cfg", cfg_set },
	{ "s", sig_mod_set },
	{ "v", vnd_mod_set },
#if CONFIG_BT_MESH_LABEL_COUNT > 0
	{ "Va", va_set },
#endif
#if MYNEWT_VAL(BLE_MESH_CDB)
	{ "cdb", cdb_set },
#endif
};

static int mesh_set(int argc, char **argv, char *val)
{
	int i;

	if (argc < 1) {
		BT_ERR("Insufficient number of arguments");
		return -EINVAL;
	}

	BT_DBG("argv[0] %s val %s", argv[0], val ? val : "(null)");

	for (i = 0; i < ARRAY_SIZE(settings); i++) {
		if (!strcmp(settings[i].name, argv[0])) {
			argc--;
			argv++;

			return settings[i].func(argc, argv, val);
		}
	}

	BT_WARN("No matching handler for key %s", argv[0]);

	return -ENOENT;
}

static void commit_mod(struct bt_mesh_model *mod, struct bt_mesh_elem *elem,
		       bool vnd, bool primary, void *user_data)
{
	if (mod->pub && mod->pub->update &&
	    mod->pub->addr != BT_MESH_ADDR_UNASSIGNED) {
		int32_t ms = bt_mesh_model_pub_period_get(mod);
		if (ms) {
			BT_DBG("Starting publish timer (period %u ms)",
			       (unsigned) ms);
			k_delayed_work_submit(&mod->pub->timer, ms);
		}
	}

	if (!IS_ENABLED(CONFIG_BT_MESH_LOW_POWER)) {
		return;
	}

	for (int i = 0; i < ARRAY_SIZE(mod->groups); i++) {
		if (mod->groups[i] != BT_MESH_ADDR_UNASSIGNED) {
			bt_mesh_lpn_group_add(mod->groups[i]);
		}
	}
}

static int mesh_commit(void)
{
	if (!bt_mesh_subnet_next(NULL)) {
		/* Nothing to do since we're not yet provisioned */
		return 0;
	}

	if (IS_ENABLED(CONFIG_BT_MESH_PB_GATT)) {
		bt_mesh_proxy_prov_disable(true);
	}

	if (bt_mesh.ivu_duration < BT_MESH_IVU_MIN_HOURS) {
		k_delayed_work_submit(&bt_mesh.ivu_timer, BT_MESH_IVU_TIMEOUT);
	}

	bt_mesh_model_foreach(commit_mod, NULL);

	atomic_set_bit(bt_mesh.flags, BT_MESH_VALID);

	bt_mesh_start();

	return 0;
}

/* Pending flags that use K_NO_WAIT as the storage timeout */
#define NO_WAIT_PENDING_BITS (BIT(BT_MESH_NET_PENDING) |           \
			      BIT(BT_MESH_IV_PENDING) |            \
			      BIT(BT_MESH_SEQ_PENDING))

/* Pending flags that use CONFIG_BT_MESH_STORE_TIMEOUT */
#define GENERIC_PENDING_BITS (BIT(BT_MESH_KEYS_PENDING) |          \
			      BIT(BT_MESH_HB_PUB_PENDING) |        \
			      BIT(BT_MESH_CFG_PENDING) |           \
			      BIT(BT_MESH_MOD_PENDING))

static void schedule_store(int flag)
{
	int32_t timeout, remaining;

	atomic_set_bit(bt_mesh.flags, flag);

	if (atomic_get(bt_mesh.flags) & NO_WAIT_PENDING_BITS) {
		timeout = K_NO_WAIT;
	} else if (atomic_test_bit(bt_mesh.flags, BT_MESH_RPL_PENDING) &&
		   (!(atomic_get(bt_mesh.flags) & GENERIC_PENDING_BITS) ||
		    (CONFIG_BT_MESH_RPL_STORE_TIMEOUT <
		     CONFIG_BT_MESH_STORE_TIMEOUT))) {
		timeout = K_SECONDS(CONFIG_BT_MESH_RPL_STORE_TIMEOUT);
	} else {
		timeout = K_SECONDS(CONFIG_BT_MESH_STORE_TIMEOUT);
	}

	remaining = k_delayed_work_remaining_get(&pending_store);
	if (remaining && remaining < timeout) {
		BT_DBG("Not rescheduling due to existing earlier deadline");
		return;
	}

	BT_DBG("Waiting %d seconds", (int) (timeout / MSEC_PER_SEC));

	k_delayed_work_submit(&pending_store, timeout);
}

static void clear_iv(void)
{
	int err;

	err = settings_save_one("bt_mesh/IV", NULL);
	if (err) {
		BT_ERR("Failed to clear IV");
	} else {
		BT_DBG("Cleared IV");
	}
}

static void clear_net(void)
{
	int err;

	err = settings_save_one("bt_mesh/Net", NULL);
	if (err) {
		BT_ERR("Failed to clear Network");
	} else {
		BT_DBG("Cleared Network");
	}
}

static void store_pending_net(void)
{
	char buf[BT_SETTINGS_SIZE(sizeof(struct net_val))];
	struct net_val net;
	char *str;
	int err;

	BT_DBG("addr 0x%04x DevKey %s", bt_mesh_primary_addr(),
	       bt_hex(bt_mesh.dev_key, 16));

	net.primary_addr = bt_mesh_primary_addr();
	memcpy(net.dev_key, bt_mesh.dev_key, 16);

	str = settings_str_from_bytes(&net, sizeof(net), buf, sizeof(buf));
	if (!str) {
		BT_ERR("Unable to encode Network as value");
		return;
	}

	BT_DBG("Saving Network as value %s", str);
	err = settings_save_one("bt_mesh/Net", str);
	if (err) {
		BT_ERR("Failed to store Network");
	} else {
		BT_DBG("Stored Network");
	}
}

void bt_mesh_store_net(void)
{
	schedule_store(BT_MESH_NET_PENDING);
}

static void store_pending_iv(void)
{
	char buf[BT_SETTINGS_SIZE(sizeof(struct iv_val))];
	struct iv_val iv;
	char *str;
	int err;

	iv.iv_index = bt_mesh.iv_index;
	iv.iv_update = atomic_test_bit(bt_mesh.flags, BT_MESH_IVU_IN_PROGRESS);
	iv.iv_duration = bt_mesh.ivu_duration;

	str = settings_str_from_bytes(&iv, sizeof(iv), buf, sizeof(buf));
	if (!str) {
		BT_ERR("Unable to encode IV as value");
		return;
	}

	BT_DBG("Saving IV as value %s", str);
	err = settings_save_one("bt_mesh/IV", str);
	if (err) {
		BT_ERR("Failed to store IV");
	} else {
		BT_DBG("Stored IV");
	}
}

void bt_mesh_store_iv(bool only_duration)
{
	schedule_store(BT_MESH_IV_PENDING);

	if (!only_duration) {
		/* Always update Seq whenever IV changes */
		schedule_store(BT_MESH_SEQ_PENDING);
	}
}

static void store_pending_seq(void)
{
	char buf[BT_SETTINGS_SIZE(sizeof(struct seq_val))];
	struct seq_val seq;
	char *str;
	int err;

	sys_put_le24(bt_mesh.seq, seq.val);

	str = settings_str_from_bytes(&seq, sizeof(seq), buf, sizeof(buf));
	if (!str) {
		BT_ERR("Unable to encode Seq as value");
		return;
	}

	BT_DBG("Saving Seq as value %s", str);
	err = settings_save_one("bt_mesh/Seq", str);
	if (err) {
		BT_ERR("Failed to store Seq");
	} else {
		BT_DBG("Stored Seq");
	}
}

void bt_mesh_store_seq(void)
{
	if (CONFIG_BT_MESH_SEQ_STORE_RATE &&
	    (bt_mesh.seq % CONFIG_BT_MESH_SEQ_STORE_RATE)) {
		return;
	}

	schedule_store(BT_MESH_SEQ_PENDING);
}

static void store_rpl(struct bt_mesh_rpl *entry)
{
	char buf[BT_SETTINGS_SIZE(sizeof(struct rpl_val))];
	struct rpl_val rpl;
	char path[18];
	char *str;
	int err;

	BT_DBG("src 0x%04x seq 0x%06x old_iv %u", entry->src,
	       (unsigned) entry->seq, entry->old_iv);

	rpl.seq = entry->seq;
	rpl.old_iv = entry->old_iv;

	str = settings_str_from_bytes(&rpl, sizeof(rpl), buf, sizeof(buf));
	if (!str) {
		BT_ERR("Unable to encode RPL as value");
		return;
	}

	snprintk(path, sizeof(path), "bt_mesh/RPL/%x", entry->src);

	BT_DBG("Saving RPL %s as value %s", path, str);
	err = settings_save_one(path, str);
	if (err) {
		BT_ERR("Failed to store RPL");
	} else {
		BT_DBG("Stored RPL");
	}
}

static void clear_rpl(struct bt_mesh_rpl *rpl, void *user_data)
{
	int err;
	char path[18];

	if (!rpl->src) {
		return;
	}

	snprintk(path, sizeof(path), "bt/mesh/RPL/%x", rpl->src);
	err = settings_save_one(path, NULL);
	if (err) {
		BT_ERR("Failed to clear RPL");
	} else {
		BT_DBG("Cleared RPL");
	}

	(void)memset(rpl, 0, sizeof(*rpl));
}

static void store_pending_rpl(struct bt_mesh_rpl *rpl, void *user_data)
{
	BT_DBG("");

	if (rpl->store) {
		rpl->store = false;
		store_rpl(rpl);
	}
}

static void store_pending_hb_pub(void)
{
	char buf[BT_SETTINGS_SIZE(sizeof(struct hb_pub_val))];
	struct bt_mesh_hb_pub pub;
	struct hb_pub_val val;
	char *str;
	int err;

	bt_mesh_hb_pub_get(&pub);
	if (pub.dst == BT_MESH_ADDR_UNASSIGNED) {
		str = NULL;
	} else {
		val.indefinite = (pub.count == 0xffff);
		val.dst = pub.dst;
		val.period = bt_mesh_hb_log(pub.period);
		val.ttl = pub.ttl;
		val.feat = pub.feat;
		val.net_idx = pub.net_idx;

		str = settings_str_from_bytes(&val, sizeof(val),
					      buf, sizeof(buf));
		if (!str) {
			BT_ERR("Unable to encode hb pub as value");
			return;
		}
	}

	BT_DBG("Saving Heartbeat Publication as value %s",
	       str ? str : "(null)");
	err = settings_save_one("bt_mesh/HBPub", str);
	if (err) {
		BT_ERR("Failed to store Heartbeat Publication");
	} else {
		BT_DBG("Stored Heartbeat Publication");
	}
}

static void store_pending_cfg(void)
{
	char buf[BT_SETTINGS_SIZE(sizeof(struct cfg_val))];
	struct cfg_val val;
	char *str;
	int err;

	val.net_transmit = bt_mesh_net_transmit_get();
	val.relay = bt_mesh_relay_get();
	val.relay_retransmit = bt_mesh_relay_retransmit_get();
	val.beacon = bt_mesh_beacon_enabled();
	val.gatt_proxy = bt_mesh_gatt_proxy_get();
	val.frnd = bt_mesh_friend_get();
	val.default_ttl = bt_mesh_default_ttl_get();

	str = settings_str_from_bytes(&val, sizeof(val), buf, sizeof(buf));
	if (!str) {
		BT_ERR("Unable to encode configuration as value");
		return;
	}

	BT_DBG("Saving configuration as value %s", str);
	err = settings_save_one("bt_mesh/Cfg", str);
	if (err) {
		BT_ERR("Failed to store configuration");
	} else {
		BT_DBG("Stored configuration");
	}
}

static void clear_cfg(void)
{
	int err;

	err = settings_save_one("bt_mesh/Cfg", NULL);
	if (err) {
		BT_ERR("Failed to clear configuration");
	} else {
		BT_DBG("Cleared configuration");
	}
}

static void clear_app_key(uint16_t app_idx)
{
	char path[20];
	int err;

	BT_DBG("AppKeyIndex 0x%03x", app_idx);

	snprintk(path, sizeof(path), "bt_mesh/AppKey/%x", app_idx);
	err = settings_save_one(path, NULL);
	if (err) {
		BT_ERR("Failed to clear AppKeyIndex 0x%03x", app_idx);
	} else {
		BT_DBG("Cleared AppKeyIndex 0x%03x", app_idx);
	}
}

static void clear_net_key(uint16_t net_idx)
{
	char path[20];
	int err;

	BT_DBG("NetKeyIndex 0x%03x", net_idx);

	snprintk(path, sizeof(path), "bt_mesh/NetKey/%x", net_idx);
	err = settings_save_one(path, NULL);
	if (err) {
		BT_ERR("Failed to clear NetKeyIndex 0x%03x", net_idx);
	} else {
		BT_DBG("Cleared NetKeyIndex 0x%03x", net_idx);
	}
}

static void store_subnet(uint16_t net_idx)
{
	const struct bt_mesh_subnet *sub;
	struct net_key_val key;
	char buf[BT_SETTINGS_SIZE(sizeof(struct app_key_val))];
	char path[20];
	char *str;
	int err;

	sub = bt_mesh_subnet_get(net_idx);
	if (!sub) {
		BT_WARN("NetKeyIndex 0x%03x not found", net_idx);
		return;
	}

	BT_DBG("NetKeyIndex 0x%03x", net_idx);

	snprintk(path, sizeof(path), "bt/mesh/NetKey/%x", net_idx);

	memcpy(&key.val[0], sub->keys[0].net, 16);
	memcpy(&key.val[1], sub->keys[1].net, 16);
	key.kr_flag = 0U; /* Deprecated */
	key.kr_phase = sub->kr_phase;

	str = settings_str_from_bytes(&key, sizeof(key), buf, sizeof(buf));
	if (!str) {
		BT_ERR("Unable to encode AppKey as value");
		return;
	}

	err = settings_save_one(path, str);
	if (err) {
		BT_ERR("Failed to store NetKey");
	} else {
		BT_DBG("Stored NetKey");
	}
}

static void store_app(uint16_t app_idx)
{
	const struct bt_mesh_app_key *app;
	char buf[BT_SETTINGS_SIZE(sizeof(struct app_key_val))];
	struct app_key_val key;
	char path[20];
	char *str;
	int err;

	snprintk(path, sizeof(path), "bt/mesh/AppKey/%x", app_idx);

	app = bt_mesh_app_key_get(app_idx);
	if (!app) {
		BT_WARN("ApKeyIndex 0x%03x not found", app_idx);
		return;
	}

	key.net_idx = app->net_idx,
	key.updated = app->updated,

	memcpy(key.val[0], app->keys[0].val, 16);
	memcpy(key.val[1], app->keys[1].val, 16);

	str = settings_str_from_bytes(&key, sizeof(key), buf, sizeof(buf));
	if (!str) {
		BT_ERR("Unable to encode AppKey as value");
		return;
	}

	snprintk(path, sizeof(path), "bt_mesh/AppKey/%x", app->app_idx);

	BT_DBG("Saving AppKey %s as value %s", path, str);
	err = settings_save_one(path, str);
	if (err) {
		BT_ERR("Failed to store AppKey");
	} else {
		BT_DBG("Stored AppKey");
	}
}

static void store_pending_keys(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(key_updates); i++) {
		struct key_update *update = &key_updates[i];

		if (!update->valid) {
			continue;
		}

		if (update->clear) {
			if (update->app_key) {
				clear_app_key(update->key_idx);
			} else {
				clear_net_key(update->key_idx);
			}
		} else {
			store_subnet(update->key_idx);
		}

		update->valid = 0;
	}
}

#if MYNEWT_VAL(BLE_MESH_CDB)
static void clear_cdb(void)
{
	int err;

	err = settings_save_one("bt/mesh/cdb/Net", NULL);
	if (err) {
		BT_ERR("Failed to clear Network");
	} else {
		BT_DBG("Cleared Network");
	}
}

static void store_pending_cdb(void)
{
	char buf[BT_SETTINGS_SIZE(sizeof(struct cdb_net_val))];
	struct cdb_net_val net;
	int err;
	char *str;

	BT_DBG("");

	net.iv_index = bt_mesh_cdb.iv_index;
	net.iv_update = atomic_test_bit(bt_mesh_cdb.flags,
					BT_MESH_CDB_IVU_IN_PROGRESS);

	str = settings_str_from_bytes(&net, sizeof(net), buf, sizeof(buf));
	if (!str) {
		BT_ERR("Unable to encode Network as value");
		return;
	}
	err = settings_save_one("bt/mesh/cdb/Net", str);
	if (err) {
		BT_ERR("Failed to store Network value");
	} else {
		BT_DBG("Stored Network value");
	}
}

static void store_cdb_node(const struct bt_mesh_cdb_node *node)
{
	char buf[BT_SETTINGS_SIZE(sizeof(struct node_val))];
	struct node_val val;
	char path[30];
	char *str;
	int err;

	val.net_idx = node->net_idx;
	val.num_elem = node->num_elem;
	val.flags = 0;

	if (atomic_test_bit(node->flags, BT_MESH_CDB_NODE_CONFIGURED)) {
		val.flags |= F_NODE_CONFIGURED;
	}

	memcpy(val.uuid, node->uuid, 16);
	memcpy(val.dev_key, node->dev_key, 16);

	snprintk(path, sizeof(path), "bt/mesh/cdb/Node/%x", node->addr);

	str = settings_str_from_bytes(&val, sizeof(val), buf, sizeof(buf));
	if (!str) {
		BT_ERR("Unable to encode Node as value");
		return;
	}


	err = settings_save_one(path, str);
	if (err) {
		BT_ERR("Failed to store Node %s value", path);
	} else {
		BT_DBG("Stored Node %s value", path);
	}
}

static void clear_cdb_node(uint16_t addr)
{
	char path[30];
	int err;

	BT_DBG("Node 0x%04x", addr);

	snprintk(path, sizeof(path), "bt/mesh/cdb/Node/%x", addr);
	err = settings_save_one(path, NULL);
	if (err) {
		BT_ERR("Failed to clear Node 0x%04x", addr);
	} else {
		BT_DBG("Cleared Node 0x%04x", addr);
	}
}

static void store_pending_cdb_nodes(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(cdb_node_updates); ++i) {
		struct node_update *update = &cdb_node_updates[i];

		if (update->addr == BT_MESH_ADDR_UNASSIGNED) {
			continue;
		}

		BT_DBG("addr: 0x%04x, clear: %d", update->addr, update->clear);

		if (update->clear) {
			clear_cdb_node(update->addr);
		} else {
			struct bt_mesh_cdb_node *node;

			node = bt_mesh_cdb_node_get(update->addr);
			if (node) {
				store_cdb_node(node);
			} else {
				BT_WARN("Node 0x%04x not found", update->addr);
			}
		}

		update->addr = BT_MESH_ADDR_UNASSIGNED;
	}
}

static void store_cdb_subnet(const struct bt_mesh_cdb_subnet *sub)
{
	char buf[BT_SETTINGS_SIZE(sizeof(struct net_key_val))];
	struct net_key_val key;
	char path[30];
	int err;
	char *str;

	BT_DBG("NetKeyIndex 0x%03x NetKey %s", sub->net_idx,
	       bt_hex(sub->keys[0].net_key, 16));

	memcpy(&key.val[0], sub->keys[0].net_key, 16);
	memcpy(&key.val[1], sub->keys[1].net_key, 16);
	key.kr_flag = sub->kr_flag;
	key.kr_phase = sub->kr_phase;

	snprintk(path, sizeof(path), "bt/mesh/cdb/Subnet/%x", sub->net_idx);


	str = settings_str_from_bytes(&key, sizeof(key), buf, sizeof(buf));
	if (!str) {
		BT_ERR("Unable to encode Subnet as value");
		return;
	}
	err = settings_save_one(path, str);
	if (err) {
		BT_ERR("Failed to store Subnet value");
	} else {
		BT_DBG("Stored Subnet value");
	}
}

static void clear_cdb_subnet(uint16_t net_idx)
{
	char path[30];
	int err;

	BT_DBG("NetKeyIndex 0x%03x", net_idx);

	snprintk(path, sizeof(path), "bt/mesh/cdb/Subnet/%x", net_idx);
	err = settings_save_one(path, NULL);
	if (err) {
		BT_ERR("Failed to clear NetKeyIndex 0x%03x", net_idx);
	} else {
		BT_DBG("Cleared NetKeyIndex 0x%03x", net_idx);
	}
}

static void store_cdb_app_key(const struct bt_mesh_cdb_app_key *app)
{
	char buf[BT_SETTINGS_SIZE(sizeof(struct app_key_val))];
	struct app_key_val key;
	char path[30];
	int err;
	char *str;

	key.net_idx = app->net_idx;
	key.updated = false;
	memcpy(key.val[0], app->keys[0].app_key, 16);
	memcpy(key.val[1], app->keys[1].app_key, 16);

	snprintk(path, sizeof(path), "bt/mesh/cdb/AppKey/%x", app->app_idx);

	str = settings_str_from_bytes(&key, sizeof(key), buf, sizeof(buf));
	err = settings_save_one(path, str);
	if (err) {
		BT_ERR("Failed to store AppKey");
	} else {
		BT_DBG("Stored AppKey");
	}
}

static void clear_cdb_app_key(uint16_t app_idx)
{
	char path[30];
	int err;

	snprintk(path, sizeof(path), "bt/mesh/cdb/AppKey/%x", app_idx);
	err = settings_save_one(path, NULL);
	if (err) {
		BT_ERR("Failed to clear AppKeyIndex 0x%03x", app_idx);
	} else {
		BT_DBG("Cleared AppKeyIndex 0x%03x", app_idx);
	}
}

static void store_pending_cdb_keys(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(cdb_key_updates); i++) {
		struct key_update *update = &cdb_key_updates[i];

		if (!update->valid) {
			continue;
		}

		if (update->clear) {
			if (update->app_key) {
				clear_cdb_app_key(update->key_idx);
			} else {
				clear_cdb_subnet(update->key_idx);
			}
		} else {
			if (update->app_key) {
				struct bt_mesh_cdb_app_key *key;

				key = bt_mesh_cdb_app_key_get(update->key_idx);
				if (key) {
					store_cdb_app_key(key);
				} else {
					BT_WARN("AppKeyIndex 0x%03x not found",
						update->key_idx);
				}
			} else {
				struct bt_mesh_cdb_subnet *sub;

				sub = bt_mesh_cdb_subnet_get(update->key_idx);
				if (sub) {
					store_cdb_subnet(sub);
				} else {
					BT_WARN("NetKeyIndex 0x%03x not found",
						update->key_idx);
				}
			}
		}

		update->valid = 0U;
	}
}

static struct node_update *cdb_node_update_find(uint16_t addr,
					       struct node_update **free_slot)
{
	struct node_update *match;
	int i;

	match = NULL;
	*free_slot = NULL;

	for (i = 0; i < ARRAY_SIZE(cdb_node_updates); i++) {
		struct node_update *update = &cdb_node_updates[i];

		if (update->addr == BT_MESH_ADDR_UNASSIGNED) {
			*free_slot = update;
			continue;
		}

		if (update->addr == addr) {
			match = update;
		}
	}

	return match;
}
#endif

static void encode_mod_path(struct bt_mesh_model *mod, bool vnd,
			    const char *key, char *path, size_t path_len)
{
	uint16_t mod_key = (((uint16_t)mod->elem_idx << 8) | mod->mod_idx);

	if (vnd) {
		snprintk(path, path_len, "bt_mesh/v/%x/%s", mod_key, key);
	} else {
		snprintk(path, path_len, "bt_mesh/s/%x/%s", mod_key, key);
	}
}

static void store_pending_mod_bind(struct bt_mesh_model *mod, bool vnd)
{
	uint16_t keys[CONFIG_BT_MESH_MODEL_KEY_COUNT];
	char buf[BT_SETTINGS_SIZE(sizeof(keys))];
	char path[20];
	int i, count, err;
	char *val;

	for (i = 0, count = 0; i < ARRAY_SIZE(mod->keys); i++) {
		if (mod->keys[i] != BT_MESH_KEY_UNUSED) {
			keys[count++] = mod->keys[i];
		}
	}

	if (count) {
		val = settings_str_from_bytes(keys, count * sizeof(keys[0]),
					      buf, sizeof(buf));
		if (!val) {
			BT_ERR("Unable to encode model bindings as value");
			return;
		}
	} else {
		val = NULL;
	}

	encode_mod_path(mod, vnd, "bind", path, sizeof(path));

	BT_DBG("Saving %s as %s", path, val ? val : "(null)");
	err = settings_save_one(path, val);
	if (err) {
		BT_ERR("Failed to store bind");
	} else {
		BT_DBG("Stored bind");
	}
}

static void store_pending_mod_sub(struct bt_mesh_model *mod, bool vnd)
{
	uint16_t groups[CONFIG_BT_MESH_MODEL_GROUP_COUNT];
	char buf[BT_SETTINGS_SIZE(sizeof(groups))];
	char path[20];
	int i, count, err;
	char *val;

	for (i = 0, count = 0; i < CONFIG_BT_MESH_MODEL_GROUP_COUNT; i++) {
		if (mod->groups[i] != BT_MESH_ADDR_UNASSIGNED) {
			groups[count++] = mod->groups[i];
		}
	}

	if (count) {
		val = settings_str_from_bytes(groups, count * sizeof(groups[0]),
					      buf, sizeof(buf));
		if (!val) {
			BT_ERR("Unable to encode model subscription as value");
			return;
		}
	} else {
		val = NULL;
	}

	encode_mod_path(mod, vnd, "sub", path, sizeof(path));

	BT_DBG("Saving %s as %s", path, val ? val : "(null)");
	err = settings_save_one(path, val);
	if (err) {
		BT_ERR("Failed to store sub");
	} else {
		BT_DBG("Stored sub");
	}
}

static void store_pending_mod_pub(struct bt_mesh_model *mod, bool vnd)
{
	char buf[BT_SETTINGS_SIZE(sizeof(struct mod_pub_val))];
	struct mod_pub_val pub;
	char path[20];
	char *val;
	int err;

	if (!mod->pub || mod->pub->addr == BT_MESH_ADDR_UNASSIGNED) {
		val = NULL;
	} else {
		pub.addr = mod->pub->addr;
		pub.key = mod->pub->key;
		pub.ttl = mod->pub->ttl;
		pub.retransmit = mod->pub->retransmit;
		pub.period = mod->pub->period;
		pub.period_div = mod->pub->period_div;
		pub.cred = mod->pub->cred;

		val = settings_str_from_bytes(&pub, sizeof(pub),
					      buf, sizeof(buf));
		if (!val) {
			BT_ERR("Unable to encode model publication as value");
			return;
		}
	}

	encode_mod_path(mod, vnd, "pub", path, sizeof(path));

	BT_DBG("Saving %s as %s", path, val ? val : "(null)");
	err = settings_save_one(path, val);
	if (err) {
		BT_ERR("Failed to store pub");
	} else {
		BT_DBG("Stored pub");
	}
}

static void store_pending_mod(struct bt_mesh_model *mod,
			      struct bt_mesh_elem *elem, bool vnd,
			      bool primary, void *user_data)
{
	if (!mod->flags) {
		return;
	}

	if (mod->flags & BT_MESH_MOD_BIND_PENDING) {
		mod->flags &= ~BT_MESH_MOD_BIND_PENDING;
		store_pending_mod_bind(mod, vnd);
	}

	if (mod->flags & BT_MESH_MOD_SUB_PENDING) {
		mod->flags &= ~BT_MESH_MOD_SUB_PENDING;
		store_pending_mod_sub(mod, vnd);
	}

	if (mod->flags & BT_MESH_MOD_PUB_PENDING) {
		mod->flags &= ~BT_MESH_MOD_PUB_PENDING;
		store_pending_mod_pub(mod, vnd);
	}
}

#define IS_VA_DEL(_label)	((_label)->ref == 0)
static void store_pending_va(void)
{
	char buf[BT_SETTINGS_SIZE(sizeof(struct va_val))];
	struct bt_mesh_va *lab;
	struct va_val va;
	char path[18];
	char *val;
	uint16_t i;
	int err = 0;

	for (i = 0; (lab = bt_mesh_va_get(i)) != NULL; i++) {
		if (!lab->changed) {
			continue;
		}

		lab->changed = 0U;

		snprintk(path, sizeof(path), "bt_mesh/Va/%x", i);

		if (IS_VA_DEL(lab)) {
			val = NULL;
		} else {
			va.ref = lab->ref;
			va.addr = lab->addr;
			memcpy(va.uuid, lab->uuid, 16);

			val = settings_str_from_bytes(&va, sizeof(va),
						      buf, sizeof(buf));
			if (!val) {
				BT_ERR("Unable to encode model publication as value");
				return;
			}

			err = settings_save_one(path, val);
		}

		if (err) {
			BT_ERR("Failed to %s %s value (err %d)",
			       IS_VA_DEL(lab) ? "delete" : "store", path, err);
		} else {
			BT_DBG("%s %s value",
			       IS_VA_DEL(lab) ? "Deleted" : "Stored", path);
		}
	}
}

static void store_pending(struct ble_npl_event *work)
{
	BT_DBG("");

	if (atomic_test_and_clear_bit(bt_mesh.flags, BT_MESH_RPL_PENDING)) {
		if (atomic_test_bit(bt_mesh.flags, BT_MESH_VALID)) {
			bt_mesh_rpl_foreach(store_pending_rpl, NULL);
		} else {
			bt_mesh_rpl_foreach(clear_rpl, NULL);
		}
	}

	if (atomic_test_and_clear_bit(bt_mesh.flags, BT_MESH_KEYS_PENDING)) {
		store_pending_keys();
	}

	if (atomic_test_and_clear_bit(bt_mesh.flags, BT_MESH_NET_PENDING)) {
		if (atomic_test_bit(bt_mesh.flags, BT_MESH_VALID)) {
			store_pending_net();
		} else {
			clear_net();
		}
	}

	if (atomic_test_and_clear_bit(bt_mesh.flags, BT_MESH_IV_PENDING)) {
		if (atomic_test_bit(bt_mesh.flags, BT_MESH_VALID)) {
			store_pending_iv();
		} else {
			clear_iv();
		}
	}

	if (atomic_test_and_clear_bit(bt_mesh.flags, BT_MESH_SEQ_PENDING)) {
		store_pending_seq();
	}

	if (atomic_test_and_clear_bit(bt_mesh.flags, BT_MESH_HB_PUB_PENDING)) {
		store_pending_hb_pub();
	}

	if (atomic_test_and_clear_bit(bt_mesh.flags, BT_MESH_CFG_PENDING)) {
		if (atomic_test_bit(bt_mesh.flags, BT_MESH_VALID)) {
			store_pending_cfg();
		} else {
			clear_cfg();
		}
	}

	if (atomic_test_and_clear_bit(bt_mesh.flags, BT_MESH_MOD_PENDING)) {
		bt_mesh_model_foreach(store_pending_mod, NULL);
	}

	if (atomic_test_and_clear_bit(bt_mesh.flags, BT_MESH_VA_PENDING)) {
		store_pending_va();
	}

#if MYNEWT_VAL(BLE_MESH_CDB)
	if (IS_ENABLED(CONFIG_BT_MESH_CDB)) {
		if (atomic_test_and_clear_bit(bt_mesh_cdb.flags,
					      BT_MESH_CDB_SUBNET_PENDING)) {
			if (atomic_test_bit(bt_mesh_cdb.flags,
					    BT_MESH_CDB_VALID)) {
				store_pending_cdb();
			} else {
				clear_cdb();
			}
		}

		if (atomic_test_and_clear_bit(bt_mesh_cdb.flags,
					      BT_MESH_CDB_NODES_PENDING)) {
			store_pending_cdb_nodes();
		}

		if (atomic_test_and_clear_bit(bt_mesh_cdb.flags,
					      BT_MESH_CDB_KEYS_PENDING)) {
			store_pending_cdb_keys();
		}
	}
#endif
}

void bt_mesh_store_rpl(struct bt_mesh_rpl *entry)
{
	entry->store = true;
	schedule_store(BT_MESH_RPL_PENDING);
}

static struct key_update *key_update_find(bool app_key, uint16_t key_idx,
					  struct key_update **free_slot)
{
	struct key_update *match;
	int i;

	match = NULL;
	*free_slot = NULL;

	for (i = 0; i < ARRAY_SIZE(key_updates); i++) {
		struct key_update *update = &key_updates[i];

		if (!update->valid) {
			*free_slot = update;
			continue;
		}

		if (update->app_key != app_key) {
			continue;
		}

		if (update->key_idx == key_idx) {
			match = update;
		}
	}

	return match;
}

void bt_mesh_store_subnet(uint16_t net_idx)
{
	struct key_update *update, *free_slot;

	BT_DBG("NetKeyIndex 0x%03x", net_idx);

	update = key_update_find(false, net_idx, &free_slot);
	if (update) {
		update->clear = 0;
		schedule_store(BT_MESH_KEYS_PENDING);
		return;
	}

	if (!free_slot) {
		store_subnet(net_idx);
		return;
	}

	free_slot->valid = 1;
	free_slot->key_idx = net_idx;
	free_slot->app_key = 0;
	free_slot->clear = 0;

	schedule_store(BT_MESH_KEYS_PENDING);
}

void bt_mesh_store_app_key(uint16_t app_idx)
{
	struct key_update *update, *free_slot;

	BT_DBG("AppKeyIndex 0x%03x", app_idx);

	update = key_update_find(true, app_idx, &free_slot);
	if (update) {
		update->clear = 0;
		schedule_store(BT_MESH_KEYS_PENDING);
		return;
	}

	if (!free_slot) {
		store_app(app_idx);
		return;
	}

	free_slot->valid = 1;
	free_slot->key_idx = app_idx;
	free_slot->app_key = 1;
	free_slot->clear = 0;

	schedule_store(BT_MESH_KEYS_PENDING);
}

void bt_mesh_store_hb_pub(void)
{
	schedule_store(BT_MESH_HB_PUB_PENDING);
}

void bt_mesh_store_cfg(void)
{
	schedule_store(BT_MESH_CFG_PENDING);
}

void bt_mesh_clear_net(void)
{
	schedule_store(BT_MESH_NET_PENDING);
	schedule_store(BT_MESH_IV_PENDING);
	schedule_store(BT_MESH_CFG_PENDING);
}

void bt_mesh_clear_subnet(uint16_t net_idx)
{
	struct key_update *update, *free_slot;

	BT_DBG("NetKeyIndex 0x%03x", net_idx);

	update = key_update_find(false, net_idx, &free_slot);
	if (update) {
		update->clear = 1;
		schedule_store(BT_MESH_KEYS_PENDING);
		return;
	}

	if (!free_slot) {
		clear_net_key(net_idx);
		return;
	}

	free_slot->valid = 1;
	free_slot->key_idx = net_idx;
	free_slot->app_key = 0;
	free_slot->clear = 1;

	schedule_store(BT_MESH_KEYS_PENDING);
}

void bt_mesh_clear_app_key(uint16_t app_idx)
{
	struct key_update *update, *free_slot;

	BT_DBG("AppKeyIndex 0x%03x", app_idx);

	update = key_update_find(true, app_idx, &free_slot);
	if (update) {
		update->clear = 1;
		schedule_store(BT_MESH_KEYS_PENDING);
		return;
	}

	if (!free_slot) {
		clear_app_key(app_idx);
		return;
	}

	free_slot->valid = 1;
	free_slot->key_idx = app_idx;
	free_slot->app_key = 1;
	free_slot->clear = 1;

	schedule_store(BT_MESH_KEYS_PENDING);
}

void bt_mesh_clear_rpl(void)
{
	schedule_store(BT_MESH_RPL_PENDING);
}

void bt_mesh_store_mod_bind(struct bt_mesh_model *mod)
{
	mod->flags |= BT_MESH_MOD_BIND_PENDING;
	schedule_store(BT_MESH_MOD_PENDING);
}

void bt_mesh_store_mod_sub(struct bt_mesh_model *mod)
{
	mod->flags |= BT_MESH_MOD_SUB_PENDING;
	schedule_store(BT_MESH_MOD_PENDING);
}

void bt_mesh_store_mod_pub(struct bt_mesh_model *mod)
{
	mod->flags |= BT_MESH_MOD_PUB_PENDING;
	schedule_store(BT_MESH_MOD_PENDING);
}


void bt_mesh_store_label(void)
{
	schedule_store(BT_MESH_VA_PENDING);
}

#if MYNEWT_VAL(BLE_MESH_CDB)
static void schedule_cdb_store(int flag)
{
	atomic_set_bit(bt_mesh_cdb.flags, flag);
	k_delayed_work_submit(&pending_store, K_NO_WAIT);
}

void bt_mesh_store_cdb(void)
{
	schedule_cdb_store(BT_MESH_CDB_SUBNET_PENDING);
}

void bt_mesh_store_cdb_node(const struct bt_mesh_cdb_node *node)
{
	struct node_update *update, *free_slot;

	BT_DBG("Node 0x%04x", node->addr);

	update = cdb_node_update_find(node->addr, &free_slot);
	if (update) {
		update->clear = false;
		schedule_cdb_store(BT_MESH_CDB_NODES_PENDING);
		return;
	}

	if (!free_slot) {
		store_cdb_node(node);
		return;
	}

	free_slot->addr = node->addr;
	free_slot->clear = false;

	schedule_cdb_store(BT_MESH_CDB_NODES_PENDING);
}

void bt_mesh_clear_cdb_node(struct bt_mesh_cdb_node *node)
{
	struct node_update *update, *free_slot;

	BT_DBG("Node 0x%04x", node->addr);

	update = cdb_node_update_find(node->addr, &free_slot);
	if (update) {
		update->clear = true;
		schedule_cdb_store(BT_MESH_CDB_NODES_PENDING);
		return;
	}

	if (!free_slot) {
		clear_cdb_node(node->addr);
		return;
	}

	free_slot->addr = node->addr;
	free_slot->clear = true;

	schedule_cdb_store(BT_MESH_CDB_NODES_PENDING);
}

/* TODO: Could be shared with key_update_find? */
static struct key_update *cdb_key_update_find(bool app_key, uint16_t key_idx,
					     struct key_update **free_slot)
{
	struct key_update *match;
	int i;

	match = NULL;
	*free_slot = NULL;

	for (i = 0; i < ARRAY_SIZE(cdb_key_updates); i++) {
		struct key_update *update = &cdb_key_updates[i];

		if (!update->valid) {
			*free_slot = update;
			continue;
		}

		if (update->app_key != app_key) {
			continue;
		}

		if (update->key_idx == key_idx) {
			match = update;
		}
	}

	return match;
}

void bt_mesh_store_cdb_subnet(const struct bt_mesh_cdb_subnet *sub)
{
	struct key_update *update, *free_slot;

	BT_DBG("NetKeyIndex 0x%03x", sub->net_idx);

	update = cdb_key_update_find(false, sub->net_idx, &free_slot);
	if (update) {
		update->clear = 0U;
		schedule_cdb_store(BT_MESH_CDB_KEYS_PENDING);
		return;
	}

	if (!free_slot) {
		store_cdb_subnet(sub);
		return;
	}

	free_slot->valid = 1U;
	free_slot->key_idx = sub->net_idx;
	free_slot->app_key = 0U;
	free_slot->clear = 0U;

	schedule_cdb_store(BT_MESH_CDB_KEYS_PENDING);
}

void bt_mesh_clear_cdb_subnet(struct bt_mesh_cdb_subnet *sub)
{
	struct key_update *update, *free_slot;

	BT_DBG("NetKeyIndex 0x%03x", sub->net_idx);

	update = cdb_key_update_find(false, sub->net_idx, &free_slot);
	if (update) {
		update->clear = 1U;
		schedule_cdb_store(BT_MESH_CDB_KEYS_PENDING);
		return;
	}

	if (!free_slot) {
		clear_cdb_subnet(sub->net_idx);
		return;
	}

	free_slot->valid = 1U;
	free_slot->key_idx = sub->net_idx;
	free_slot->app_key = 0U;
	free_slot->clear = 1U;

	schedule_cdb_store(BT_MESH_CDB_KEYS_PENDING);
}

void bt_mesh_store_cdb_app_key(const struct bt_mesh_cdb_app_key *key)
{
	struct key_update *update, *free_slot;

	BT_DBG("AppKeyIndex 0x%03x", key->app_idx);

	update = cdb_key_update_find(true, key->app_idx, &free_slot);
	if (update) {
		update->clear = 0U;
		schedule_cdb_store(BT_MESH_CDB_KEYS_PENDING);
		return;
	}

	if (!free_slot) {
		store_cdb_app_key(key);
		return;
	}

	free_slot->valid = 1U;
	free_slot->key_idx = key->app_idx;
	free_slot->app_key = 1U;
	free_slot->clear = 0U;

	schedule_cdb_store(BT_MESH_CDB_KEYS_PENDING);
}

void bt_mesh_clear_cdb_app_key(struct bt_mesh_cdb_app_key *key)
{
	struct key_update *update, *free_slot;

	BT_DBG("AppKeyIndex 0x%03x", key->app_idx);

	update = cdb_key_update_find(true, key->app_idx, &free_slot);
	if (update) {
		update->clear = 1U;
		schedule_cdb_store(BT_MESH_CDB_KEYS_PENDING);
		return;
	}

	if (!free_slot) {
		clear_cdb_app_key(key->app_idx);
		return;
	}

	free_slot->valid = 1U;
	free_slot->key_idx = key->app_idx;
	free_slot->app_key = 1U;
	free_slot->clear = 1U;

	schedule_cdb_store(BT_MESH_CDB_KEYS_PENDING);
}
#endif

int bt_mesh_model_data_store(struct bt_mesh_model *mod, bool vnd,
			     			 const char *name, const void *data,
							 size_t data_len)
{
	char path[30];
	char buf[BT_SETTINGS_SIZE(sizeof(struct mod_pub_val))];
	char *val;
	int err;

	encode_mod_path(mod, vnd, "data", path, sizeof(path));
	if (name) {
		strcat(path, "/");
		strncat(path, name, 8);
	}

	if (data_len) {
		val = settings_str_from_bytes(data, data_len,
					      buf, sizeof(buf));
		if (!val) {
			BT_ERR("Unable to encode model publication as value");
			return -EINVAL;
		}
		err = settings_save_one(path, val);
	} else {
		err = settings_save_one(path, NULL);
	}

	if (err) {
		BT_ERR("Failed to store %s value", path);
	} else {
		BT_DBG("Stored %s value", path);
	}
	return err;
}

static struct conf_handler bt_mesh_settings_conf_handler = {
	.ch_name = "bt_mesh",
	.ch_get = NULL,
	.ch_set = mesh_set,
	.ch_commit = mesh_commit,
	.ch_export = NULL,
};

void bt_mesh_settings_init(void)
{
	int rc;

	rc = conf_register(&bt_mesh_settings_conf_handler);

	SYSINIT_PANIC_ASSERT_MSG(rc == 0,
				 "Failed to register bt_mesh_settings conf");

	k_delayed_work_init(&pending_store, store_pending);
}

#endif /* MYNEWT_VAL(BLE_MESH_SETTINGS) */
#endif
