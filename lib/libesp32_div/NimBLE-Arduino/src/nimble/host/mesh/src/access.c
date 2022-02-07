/*  Bluetooth Mesh */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "syscfg/syscfg.h"
#define MESH_LOG_MODULE BLE_MESH_ACCESS_LOG

#include <errno.h>
#include <os/os_mbuf.h>

#include "mesh/mesh.h"

#include "mesh_priv.h"
#include "adv.h"
#include "net.h"
#include "lpn.h"
#include "transport.h"
#include "access.h"
#include "foundation.h"
#if MYNEWT_VAL(BLE_MESH_SHELL_MODELS)
#include "mesh/model_cli.h"
#endif

static const struct bt_mesh_comp *dev_comp;
static u16_t dev_primary_addr;

void bt_mesh_model_foreach(void (*func)(struct bt_mesh_model *mod,
					struct bt_mesh_elem *elem,
					bool vnd, bool primary,
					void *user_data),
			   void *user_data)
{
	int i, j;

	for (i = 0; i < dev_comp->elem_count; i++) {
		struct bt_mesh_elem *elem = &dev_comp->elem[i];

		for (j = 0; j < elem->model_count; j++) {
			struct bt_mesh_model *model = &elem->models[j];

			func(model, elem, false, i == 0, user_data);
		}

		for (j = 0; j < elem->vnd_model_count; j++) {
			struct bt_mesh_model *model = &elem->vnd_models[j];

			func(model, elem, true, i == 0, user_data);
		}
	}
}

s32_t bt_mesh_model_pub_period_get(struct bt_mesh_model *mod)
{
	int period;

	if (!mod->pub) {
		return 0;
	}

	switch (mod->pub->period >> 6) {
	case 0x00:
		/* 1 step is 100 ms */
		period = K_MSEC((mod->pub->period & BIT_MASK(6)) * 100);
		break;
	case 0x01:
		/* 1 step is 1 second */
		period = K_SECONDS(mod->pub->period & BIT_MASK(6));
		break;
	case 0x02:
		/* 1 step is 10 seconds */
		period = K_SECONDS((mod->pub->period & BIT_MASK(6)) * 10);
		break;
	case 0x03:
		/* 1 step is 10 minutes */
		period = K_MINUTES((mod->pub->period & BIT_MASK(6)) * 10);
		break;
	default:
		CODE_UNREACHABLE;
	}

	if (mod->pub->fast_period) {
		return period >> mod->pub->period_div;
	} else {
		return period;
	}
}

static s32_t next_period(struct bt_mesh_model *mod)
{
	struct bt_mesh_model_pub *pub = mod->pub;
	u32_t elapsed, period;

	period = bt_mesh_model_pub_period_get(mod);
	if (!period) {
		return 0;
	}

	elapsed = k_uptime_get_32() - pub->period_start;

	BT_DBG("Publishing took %ums", (unsigned) elapsed);

	if (elapsed > period) {
		BT_WARN("Publication sending took longer than the period");
		/* Return smallest positive number since 0 means disabled */
		return K_MSEC(1);
	}

	return period - elapsed;
}

static void publish_sent(int err, void *user_data)
{
	struct bt_mesh_model *mod = user_data;
	s32_t delay;

	BT_DBG("err %d", err);

	if (mod->pub->count) {
		delay = BT_MESH_PUB_TRANSMIT_INT(mod->pub->retransmit);
	} else {
		delay = next_period(mod);
	}

	if (delay) {
		BT_DBG("Publishing next time in %dms", (int) delay);
		k_delayed_work_submit(&mod->pub->timer, delay);
	}
}

static void publish_start(u16_t duration, int err, void *user_data)
{
	struct bt_mesh_model *mod = user_data;
	struct bt_mesh_model_pub *pub = mod->pub;

	if (err) {
		BT_ERR("Failed to publish: err %d", err);
		return;
	}

	/* Initialize the timestamp for the beginning of a new period */
	if (pub->count == BT_MESH_PUB_TRANSMIT_COUNT(pub->retransmit)) {
		pub->period_start = k_uptime_get_32();
	}
}

static const struct bt_mesh_send_cb pub_sent_cb = {
	.start = publish_start,
	.end = publish_sent,
};

static int publish_retransmit(struct bt_mesh_model *mod)
{
	struct os_mbuf *sdu = NET_BUF_SIMPLE(BT_MESH_TX_SDU_MAX);
	struct bt_mesh_model_pub *pub = mod->pub;
	struct bt_mesh_app_key *key;
	struct bt_mesh_msg_ctx ctx = {
		.addr = pub->addr,
		.send_ttl = pub->ttl,
	};
	struct bt_mesh_net_tx tx = {
		.ctx = &ctx,
		.src = bt_mesh_model_elem(mod)->addr,
		.xmit = bt_mesh_net_transmit_get(),
		.friend_cred = pub->cred,
	};
	int err;

	key = bt_mesh_app_key_find(pub->key);
	if (!key) {
		err = -EADDRNOTAVAIL;
		goto done;
	}

	tx.sub = bt_mesh_subnet_get(key->net_idx);

	ctx.net_idx = key->net_idx;
	ctx.app_idx = key->app_idx;

	net_buf_simple_init(sdu, 0);
	net_buf_simple_add_mem(sdu, pub->msg->om_data, pub->msg->om_len);

	pub->count--;

	err = bt_mesh_trans_send(&tx, sdu, &pub_sent_cb, mod);

done:
	os_mbuf_free_chain(sdu);
	return err;
}

static void mod_publish(struct ble_npl_event *work)
{
	struct bt_mesh_model_pub *pub = ble_npl_event_get_arg(work);
	s32_t period_ms;
	int err;

	BT_DBG("");

	period_ms = bt_mesh_model_pub_period_get(pub->mod);
	BT_DBG("period %u ms", (unsigned) period_ms);

	if (pub->count) {
		err = publish_retransmit(pub->mod);
		if (err) {
			BT_ERR("Failed to retransmit (err %d)", err);

			pub->count = 0;

			/* Continue with normal publication */
			if (period_ms) {
				k_delayed_work_submit(&pub->timer, period_ms);
			}
		}

		return;
	}

	if (!period_ms) {
		return;
	}

	__ASSERT_NO_MSG(pub->update != NULL);

	err = pub->update(pub->mod);
	if (err) {
		BT_ERR("Failed to update publication message");
		return;
	}

	err = bt_mesh_model_publish(pub->mod);
	if (err) {
		BT_ERR("Publishing failed (err %d)", err);
	}
}

struct bt_mesh_elem *bt_mesh_model_elem(struct bt_mesh_model *mod)
{
	return &dev_comp->elem[mod->elem_idx];
}

struct bt_mesh_model *bt_mesh_model_get(bool vnd, u8_t elem_idx, u8_t mod_idx)
{
	struct bt_mesh_elem *elem;

	if (elem_idx >= dev_comp->elem_count) {
		BT_ERR("Invalid element index %u", elem_idx);
		return NULL;
	}

	elem = &dev_comp->elem[elem_idx];

	if (vnd) {
		if (mod_idx >= elem->vnd_model_count) {
			BT_ERR("Invalid vendor model index %u", mod_idx);
			return NULL;
		}

		return &elem->vnd_models[mod_idx];
	} else {
		if (mod_idx >= elem->model_count) {
			BT_ERR("Invalid SIG model index %u", mod_idx);
			return NULL;
		}

		return &elem->models[mod_idx];
	}
}

static void mod_init(struct bt_mesh_model *mod, struct bt_mesh_elem *elem,
		     bool vnd, bool primary, void *user_data)
{
	int i;

	if (mod->pub) {
		mod->pub->mod = mod;
		k_delayed_work_init(&mod->pub->timer, mod_publish);
		k_delayed_work_add_arg(&mod->pub->timer, mod->pub);
	}

	for (i = 0; i < ARRAY_SIZE(mod->keys); i++) {
		mod->keys[i] = BT_MESH_KEY_UNUSED;
	}

	mod->elem_idx = elem - dev_comp->elem;
	if (vnd) {
		mod->mod_idx = mod - elem->vnd_models;
	} else {
		mod->mod_idx = mod - elem->models;
	}

	if (mod->cb && mod->cb->init) {
		mod->cb->init(mod);
	}
}

int bt_mesh_comp_register(const struct bt_mesh_comp *comp)
{
	/* There must be at least one element */
	if (!comp->elem_count) {
		return -EINVAL;
	}

	dev_comp = comp;

	bt_mesh_model_foreach(mod_init, NULL);

	return 0;
}

void bt_mesh_comp_provision(u16_t addr)
{
	int i;

	dev_primary_addr = addr;

	BT_DBG("addr 0x%04x elem_count %zu", addr, dev_comp->elem_count);

	for (i = 0; i < dev_comp->elem_count; i++) {
		struct bt_mesh_elem *elem = &dev_comp->elem[i];

		elem->addr = addr++;

		BT_DBG("addr 0x%04x mod_count %u vnd_mod_count %u",
		       elem->addr, elem->model_count, elem->vnd_model_count);
	}
}

void bt_mesh_comp_unprovision(void)
{
	BT_DBG("");

	dev_primary_addr = BT_MESH_ADDR_UNASSIGNED;

	bt_mesh_model_foreach(mod_init, NULL);
}

u16_t bt_mesh_primary_addr(void)
{
	return dev_primary_addr;
}

static u16_t *model_group_get(struct bt_mesh_model *mod, u16_t addr)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(mod->groups); i++) {
		if (mod->groups[i] == addr) {
			return &mod->groups[i];
		}
	}

	return NULL;
}

struct find_group_visitor_ctx {
	u16_t *entry;
	struct bt_mesh_model *mod;
	u16_t addr;
};

static enum bt_mesh_walk find_group_mod_visitor(struct bt_mesh_model *mod,
						u32_t depth, void *user_data)
{
	struct find_group_visitor_ctx *ctx = user_data;

	if (mod->elem_idx != ctx->mod->elem_idx) {
		return BT_MESH_WALK_CONTINUE;
	}

	ctx->entry = model_group_get(mod, ctx->addr);
	if (ctx->entry) {
		ctx->mod = mod;
		return BT_MESH_WALK_STOP;
	}

	return BT_MESH_WALK_CONTINUE;
}

u16_t *bt_mesh_model_find_group(struct bt_mesh_model **mod, u16_t addr)
{
	struct find_group_visitor_ctx ctx = {
		.mod = *mod,
		.entry = NULL,
		.addr = addr,
	};

	bt_mesh_model_tree_walk(bt_mesh_model_root(*mod),
				find_group_mod_visitor, &ctx);

	*mod = ctx.mod;
	return ctx.entry;
}

static struct bt_mesh_model *bt_mesh_elem_find_group(struct bt_mesh_elem *elem,
						     u16_t group_addr)
{
	struct bt_mesh_model *model;
	u16_t *match;
	int i;

	for (i = 0; i < elem->model_count; i++) {
		model = &elem->models[i];

		match = model_group_get(model, group_addr);
		if (match) {
			return model;
		}
	}

	for (i = 0; i < elem->vnd_model_count; i++) {
		model = &elem->vnd_models[i];

		match = model_group_get(model, group_addr);
		if (match) {
			return model;
		}
	}

	return NULL;
}

struct bt_mesh_elem *bt_mesh_elem_find(u16_t addr)
{
	u16_t index;

	if (BT_MESH_ADDR_IS_UNICAST(addr)) {
		index = (addr - dev_comp->elem[0].addr);
		if (index < dev_comp->elem_count) {
			return &dev_comp->elem[index];
		} else {
			return NULL;
		}
	}

	for (index = 0; index < dev_comp->elem_count; index++) {
		struct bt_mesh_elem *elem = &dev_comp->elem[index];

		if (bt_mesh_elem_find_group(elem, addr)) {
			return elem;
		}
	}

	return NULL;
}

u8_t bt_mesh_elem_count(void)
{
	return dev_comp->elem_count;
}

static bool model_has_key(struct bt_mesh_model *mod, u16_t key)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(mod->keys); i++) {
		if (mod->keys[i] == key ||
		    (mod->keys[i] == BT_MESH_KEY_DEV_ANY &&
		     BT_MESH_IS_DEV_KEY(key))) {
			return true;
		}
	}

	return false;
}

static bool model_has_dst(struct bt_mesh_model *mod, u16_t dst)
{
	if (BT_MESH_ADDR_IS_UNICAST(dst)) {
		return (dev_comp->elem[mod->elem_idx].addr == dst);
	} else if (BT_MESH_ADDR_IS_GROUP(dst) || BT_MESH_ADDR_IS_VIRTUAL(dst)) {
		return bt_mesh_model_find_group(&mod, dst);
	}

	return (mod->elem_idx == 0 && bt_mesh_fixed_group_match(dst));
}

static const struct bt_mesh_model_op *find_op(struct bt_mesh_model *models,
					      u8_t model_count, u32_t opcode,
					      struct bt_mesh_model **model)
{
	u8_t i;

	for (i = 0; i < model_count; i++) {
		const struct bt_mesh_model_op *op;

		*model = &models[i];

		for (op = (*model)->op; op->func; op++) {
			if (op->opcode == opcode) {
				return op;
			}
		}
	}

	*model = NULL;
	return NULL;
}

static int get_opcode(struct os_mbuf *buf, u32_t *opcode)
{
	switch (buf->om_data[0] >> 6) {
	case 0x00:
	case 0x01:
		if (buf->om_data[0] == 0x7f) {
			BT_ERR("Ignoring RFU OpCode");
			return -EINVAL;
		}

		*opcode = net_buf_simple_pull_u8(buf);
		return 0;
	case 0x02:
		if (buf->om_len < 2) {
			BT_ERR("Too short payload for 2-octet OpCode");
			return -EINVAL;
		}

		*opcode = net_buf_simple_pull_be16(buf);
		return 0;
	case 0x03:
		if (buf->om_len < 3) {
			BT_ERR("Too short payload for 3-octet OpCode");
			return -EINVAL;
		}

		*opcode = net_buf_simple_pull_u8(buf) << 16;
		*opcode |= net_buf_simple_pull_le16(buf);
		return 0;
	}

	CODE_UNREACHABLE;
}

bool bt_mesh_fixed_group_match(u16_t addr)
{
	/* Check for fixed group addresses */
	switch (addr) {
	case BT_MESH_ADDR_ALL_NODES:
		return true;
	case BT_MESH_ADDR_PROXIES:
		return (bt_mesh_gatt_proxy_get() == BT_MESH_GATT_PROXY_ENABLED);
	case BT_MESH_ADDR_FRIENDS:
		return (bt_mesh_friend_get() == BT_MESH_FRIEND_ENABLED);
	case BT_MESH_ADDR_RELAYS:
		return (bt_mesh_relay_get() == BT_MESH_RELAY_ENABLED);
	default:
		return false;
	}
}

void bt_mesh_model_recv(struct bt_mesh_net_rx *rx, struct os_mbuf *buf)
{
	struct bt_mesh_model *models, *model;
	const struct bt_mesh_model_op *op;
	u32_t opcode;
	u8_t count;
	int i;

	BT_DBG("app_idx 0x%04x src 0x%04x dst 0x%04x", rx->ctx.app_idx,
	       rx->ctx.addr, rx->ctx.recv_dst);
	BT_DBG("len %u: %s", buf->om_len, bt_hex(buf->om_data, buf->om_len));

	if (get_opcode(buf, &opcode) < 0) {
		BT_WARN("Unable to decode OpCode");
		return;
	}

	BT_DBG("OpCode 0x%08x", (unsigned) opcode);

	for (i = 0; i < dev_comp->elem_count; i++) {
		struct bt_mesh_elem *elem = &dev_comp->elem[i];
		struct net_buf_simple_state state;

		/* SIG models cannot contain 3-byte (vendor) OpCodes, and
		 * vendor models cannot contain SIG (1- or 2-byte) OpCodes, so
		 * we only need to do the lookup in one of the model lists.
		 */
		if (BT_MESH_MODEL_OP_LEN(opcode) < 3) {
			models = elem->models;
			count = elem->model_count;
		} else {
			models = elem->vnd_models;
			count = elem->vnd_model_count;
		}

		op = find_op(models, count, opcode, &model);
		if (!op) {
			BT_DBG("No OpCode 0x%08x for elem %d", opcode, i);
			continue;
		}

		if (!model_has_key(model, rx->ctx.app_idx)) {
				continue;
			}

		if (!model_has_dst(model, rx->ctx.recv_dst)) {
			continue;
		}

		if (buf->om_len < op->min_len) {
			BT_ERR("Too short message for OpCode 0x%08x", opcode);
			continue;
		}

		/* The callback will likely parse the buffer, so
		 * store the parsing state in case multiple models
		 * receive the message.
		 */
		net_buf_simple_save(buf, &state);
		op->func(model, &rx->ctx, buf);
		net_buf_simple_restore(buf, &state);
	}
}

void bt_mesh_model_msg_init(struct os_mbuf *msg, u32_t opcode)
{
	net_buf_simple_init(msg, 0);

	switch (BT_MESH_MODEL_OP_LEN(opcode)) {
	case 1:
		net_buf_simple_add_u8(msg, opcode);
		break;
	case 2:
		net_buf_simple_add_be16(msg, opcode);
		break;
	case 3:
		net_buf_simple_add_u8(msg, ((opcode >> 16) & 0xff));
		net_buf_simple_add_le16(msg, opcode & 0xffff);
		break;
	default:
		BT_WARN("Unknown opcode format");
		break;
	}
}

static int model_send(struct bt_mesh_model *model,
		      struct bt_mesh_net_tx *tx, bool implicit_bind,
		      struct os_mbuf *msg,
		      const struct bt_mesh_send_cb *cb, void *cb_data)
{
	BT_DBG("net_idx 0x%04x app_idx 0x%04x dst 0x%04x", tx->ctx->net_idx,
	       tx->ctx->app_idx, tx->ctx->addr);
	BT_DBG("len %u: %s", msg->om_len, bt_hex(msg->om_data, msg->om_len));

	if (!bt_mesh_is_provisioned()) {
		BT_ERR("Local node is not yet provisioned");
		return -EAGAIN;
	}

	if (net_buf_simple_tailroom(msg) < 4) {
		BT_ERR("Not enough tailroom for TransMIC");
		return -EINVAL;
	}

	if (msg->om_len > BT_MESH_TX_SDU_MAX - 4) {
		BT_ERR("Too big message");
		return -EMSGSIZE;
	}

	if (!implicit_bind && !model_has_key(model, tx->ctx->app_idx)) {
		BT_ERR("Model not bound to AppKey 0x%04x", tx->ctx->app_idx);
		return -EINVAL;
	}

	return bt_mesh_trans_send(tx, msg, cb, cb_data);
}

int bt_mesh_model_send(struct bt_mesh_model *model,
		       struct bt_mesh_msg_ctx *ctx,
		       struct os_mbuf *msg,
		       const struct bt_mesh_send_cb *cb, void *cb_data)
{
	struct bt_mesh_net_tx tx = {
		.sub = bt_mesh_subnet_get(ctx->net_idx),
		.ctx = ctx,
		.src = bt_mesh_model_elem(model)->addr,
		.xmit = bt_mesh_net_transmit_get(),
		.friend_cred = 0,
	};

	return model_send(model, &tx, false, msg, cb, cb_data);
}

int bt_mesh_model_publish(struct bt_mesh_model *model)
{
	struct os_mbuf *sdu = NET_BUF_SIMPLE(BT_MESH_TX_SDU_MAX);
	struct bt_mesh_model_pub *pub = model->pub;
	struct bt_mesh_app_key *key;
	struct bt_mesh_msg_ctx ctx = {
	};
	struct bt_mesh_net_tx tx = {
		.ctx = &ctx,
		.src = bt_mesh_model_elem(model)->addr,
		.xmit = bt_mesh_net_transmit_get(),
	};
	int err;

	BT_DBG("");

	if (!pub) {
		err = -ENOTSUP;
		goto done;
	}

	if (pub->addr == BT_MESH_ADDR_UNASSIGNED) {
		err = -EADDRNOTAVAIL;
		goto done;
	}

	key = bt_mesh_app_key_find(pub->key);
	if (!key) {
		err = -EADDRNOTAVAIL;
		goto done;
	}

	if (pub->msg->om_len + 4 > BT_MESH_TX_SDU_MAX) {
		BT_ERR("Message does not fit maximum SDU size");
		err = -EMSGSIZE;
		goto done;
	}

	if (pub->count) {
		BT_WARN("Clearing publish retransmit timer");
		k_delayed_work_cancel(&pub->timer);
	}

	net_buf_simple_init(sdu, 0);
	net_buf_simple_add_mem(sdu, pub->msg->om_data, pub->msg->om_len);

	ctx.addr = pub->addr;
	ctx.send_ttl = pub->ttl;
	ctx.net_idx = key->net_idx;
	ctx.app_idx = key->app_idx;

	tx.friend_cred = pub->cred;
	tx.sub = bt_mesh_subnet_get(ctx.net_idx),

	pub->count = BT_MESH_PUB_TRANSMIT_COUNT(pub->retransmit);

	BT_DBG("Publish Retransmit Count %u Interval %ums", pub->count,
	       BT_MESH_PUB_TRANSMIT_INT(pub->retransmit));

	err = model_send(model, &tx, true, sdu, &pub_sent_cb, model);
	if (err) {
		/* Don't try retransmissions for this publish attempt */
		pub->count = 0;
		/* Make sure the publish timer gets reset */
		publish_sent(err, model);
	}

done:
	os_mbuf_free_chain(sdu);
	return err;
}

struct bt_mesh_model *bt_mesh_model_find_vnd(const struct bt_mesh_elem *elem,
					     u16_t company, u16_t id)
{
	u8_t i;

	for (i = 0; i < elem->vnd_model_count; i++) {
		if (elem->vnd_models[i].vnd.company == company &&
		    elem->vnd_models[i].vnd.id == id) {
			return &elem->vnd_models[i];
		}
	}

	return NULL;
}

struct bt_mesh_model *bt_mesh_model_find(const struct bt_mesh_elem *elem,
					 u16_t id)
{
	u8_t i;

	for (i = 0; i < elem->model_count; i++) {
		if (elem->models[i].id == id) {
			return &elem->models[i];
		}
	}

	return NULL;
}

const struct bt_mesh_comp *bt_mesh_comp_get(void)
{
	return dev_comp;
}

struct bt_mesh_model *bt_mesh_model_root(struct bt_mesh_model *mod)
{
#ifdef CONFIG_BT_MESH_MODEL_EXTENSIONS
	while (mod->next) {
		mod = mod->next;
	}
#endif
	return mod;
}

void bt_mesh_model_tree_walk(struct bt_mesh_model *root,
			     enum bt_mesh_walk (*cb)(struct bt_mesh_model *mod,
						     u32_t depth,
						     void *user_data),
			     void *user_data)
{
	struct bt_mesh_model *m = root;
	u32_t depth = 0;

	do {
		if (cb(m, depth, user_data) == BT_MESH_WALK_STOP) {
			return;
		}
#if MYNEWT_VAL(BLE_MESH_MODEL_EXTENSIONS)
		if (m->extends) {
			m = m->extends;
			depth++;
		} else if (m->flags & BT_MESH_MOD_NEXT_IS_PARENT) {
			m = m->next->next;
			depth--;
		} else {
			m = m->next;
		}
#endif
	} while (m && m != root);
}

#if MYNEWT_VAL(BLE_MESH_MODEL_EXTENSIONS)
int bt_mesh_model_extend(struct bt_mesh_model *mod,
			 struct bt_mesh_model *base_mod)
{
	/* Form a cyclical LCRS tree:
	 * The extends-pointer points to the first child, and the next-pointer
	 * points to the next sibling. The last sibling is marked by the
	 * BT_MESH_MOD_NEXT_IS_PARENT flag, and its next-pointer points back to
	 * the parent. This way, the whole tree is accessible from any node.
	 *
	 * We add children (extend them) by inserting them as the first child.
	 */
	if (base_mod->next) {
		return -EALREADY;
	}

	if (mod->extends) {
		base_mod->next = mod->extends;
	} else {
		base_mod->next = mod;
		base_mod->flags |= BT_MESH_MOD_NEXT_IS_PARENT;
	}

	mod->extends = base_mod;
	return 0;
}
#endif
