/*  Bluetooth Mesh */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include "syscfg/syscfg.h"
#define BT_DBG_ENABLED (MYNEWT_VAL(BLE_MESH_DEBUG_MODEL))
#include "host/ble_hs_log.h"

#include "mesh/mesh.h"
#include "mesh_priv.h"
#include "adv.h"
#include "net.h"
#include "transport.h"
#include "access.h"
#include "foundation.h"

#define HEALTH_TEST_STANDARD 0x00

/* Health Server context of the primary element */
struct bt_mesh_health_srv *health_srv;

static void health_get_registered(struct bt_mesh_model *mod,
				  u16_t company_id,
				  struct os_mbuf *msg)
{
	struct bt_mesh_health_srv *srv = mod->user_data;
	u8_t *test_id;

	BT_DBG("Company ID 0x%04x", company_id);

	bt_mesh_model_msg_init(msg, OP_HEALTH_FAULT_STATUS);

	test_id = net_buf_simple_add(msg, 1);
	net_buf_simple_add_le16(msg, company_id);

	if (srv->cb && srv->cb->fault_get_reg) {
		u8_t fault_count = net_buf_simple_tailroom(msg) - 4;
		int err;

		err = srv->cb->fault_get_reg(mod, company_id, test_id,
					     net_buf_simple_tail(msg),
					     &fault_count);
		if (err) {
			BT_ERR("Failed to get faults (err %d)", err);
			*test_id = HEALTH_TEST_STANDARD;
		} else {
			net_buf_simple_add(msg, fault_count);
		}
	} else {
		BT_WARN("No callback for getting faults");
		*test_id = HEALTH_TEST_STANDARD;
	}
}

static size_t health_get_current(struct bt_mesh_model *mod,
				 struct os_mbuf *msg)
{
	struct bt_mesh_health_srv *srv = mod->user_data;
	const struct bt_mesh_comp *comp;
	u8_t *test_id, *company_ptr;
	u16_t company_id;
	u8_t fault_count;
	int err;

	bt_mesh_model_msg_init(msg, OP_HEALTH_CURRENT_STATUS);

	test_id = net_buf_simple_add(msg, 1);
	company_ptr = net_buf_simple_add(msg, sizeof(company_id));
	comp = bt_mesh_comp_get();

	if (srv->cb && srv->cb->fault_get_cur) {
		fault_count = net_buf_simple_tailroom(msg);
		err = srv->cb->fault_get_cur(mod, test_id, &company_id,
					     net_buf_simple_tail(msg),
					     &fault_count);
		if (err) {
			BT_ERR("Failed to get faults (err %d)", err);
			sys_put_le16(comp->cid, company_ptr);
			*test_id = HEALTH_TEST_STANDARD;
			fault_count = 0;
		} else {
			sys_put_le16(company_id, company_ptr);
			net_buf_simple_add(msg, fault_count);
		}
	} else {
		BT_WARN("No callback for getting faults");
		sys_put_le16(comp->cid, company_ptr);
		*test_id = HEALTH_TEST_STANDARD;
		fault_count = 0;
	}

	return fault_count;
}

static void health_fault_get(struct bt_mesh_model *model,
			     struct bt_mesh_msg_ctx *ctx,
			     struct os_mbuf *buf)
{
	struct os_mbuf *sdu = NET_BUF_SIMPLE(BT_MESH_TX_SDU_MAX);
	u16_t company_id;

	company_id = net_buf_simple_pull_le16(buf);

	BT_DBG("company_id 0x%04x", company_id);

	health_get_registered(model, company_id, sdu);

	if (bt_mesh_model_send(model, ctx, sdu, NULL, NULL)) {
		BT_ERR("Unable to send Health Current Status response");
	}

	os_mbuf_free_chain(sdu);
}

static void health_fault_clear_unrel(struct bt_mesh_model *model,
				     struct bt_mesh_msg_ctx *ctx,
				     struct os_mbuf *buf)
{
	struct bt_mesh_health_srv *srv = model->user_data;
	u16_t company_id;

	company_id = net_buf_simple_pull_le16(buf);

	BT_DBG("company_id 0x%04x", company_id);

	if (srv->cb && srv->cb->fault_clear) {
		srv->cb->fault_clear(model, company_id);
	}
}

static void health_fault_clear(struct bt_mesh_model *model,
			       struct bt_mesh_msg_ctx *ctx,
			       struct os_mbuf *buf)
{
	struct os_mbuf *sdu = NET_BUF_SIMPLE(BT_MESH_TX_SDU_MAX);
	struct bt_mesh_health_srv *srv = model->user_data;
	u16_t company_id;

	company_id = net_buf_simple_pull_le16(buf);

	BT_DBG("company_id 0x%04x", company_id);

	if (srv->cb && srv->cb->fault_clear) {
		srv->cb->fault_clear(model, company_id);
	}

	health_get_registered(model, company_id, sdu);

	if (bt_mesh_model_send(model, ctx, sdu, NULL, NULL)) {
		BT_ERR("Unable to send Health Current Status response");
	}

	os_mbuf_free_chain(sdu);
}

static void health_fault_test_unrel(struct bt_mesh_model *model,
				    struct bt_mesh_msg_ctx *ctx,
				    struct os_mbuf *buf)
{
	struct bt_mesh_health_srv *srv = model->user_data;
	u16_t company_id;
	u8_t test_id;

	test_id = net_buf_simple_pull_u8(buf);
	company_id = net_buf_simple_pull_le16(buf);

	BT_DBG("test 0x%02x company 0x%04x", test_id, company_id);

	if (srv->cb && srv->cb->fault_test) {
		srv->cb->fault_test(model, test_id, company_id);
	}
}

static void health_fault_test(struct bt_mesh_model *model,
			      struct bt_mesh_msg_ctx *ctx,
			      struct os_mbuf *buf)
{
	struct os_mbuf *sdu = NET_BUF_SIMPLE(BT_MESH_TX_SDU_MAX);
	struct bt_mesh_health_srv *srv = model->user_data;
	u16_t company_id;
	u8_t test_id;

	BT_DBG("");

	test_id = net_buf_simple_pull_u8(buf);
	company_id = net_buf_simple_pull_le16(buf);

	BT_DBG("test 0x%02x company 0x%04x", test_id, company_id);

	if (srv->cb && srv->cb->fault_test) {
		int err;

		err = srv->cb->fault_test(model, test_id, company_id);
		if (err) {
			BT_WARN("Running fault test failed with err %d", err);
			goto done;
		}
	}

	health_get_registered(model, company_id, sdu);

	if (bt_mesh_model_send(model, ctx, sdu, NULL, NULL)) {
		BT_ERR("Unable to send Health Current Status response");
	}

done:
	os_mbuf_free_chain(sdu);
}

static void send_attention_status(struct bt_mesh_model *model,
				  struct bt_mesh_msg_ctx *ctx)
{
	/* Needed size: opcode (2 bytes) + msg + MIC */
	struct os_mbuf *msg = NET_BUF_SIMPLE(2 + 1 + 4);
	struct bt_mesh_health_srv *srv = model->user_data;
	u8_t time;

	time = k_delayed_work_remaining_get(&srv->attn_timer) / 1000;
	BT_DBG("%u second%s", time, (time == 1) ? "" : "s");

	bt_mesh_model_msg_init(msg, OP_ATTENTION_STATUS);

	net_buf_simple_add_u8(msg, time);

	if (bt_mesh_model_send(model, ctx, msg, NULL, NULL)) {
		BT_ERR("Unable to send Attention Status");
	}

	os_mbuf_free_chain(msg);
}

static void attention_get(struct bt_mesh_model *model,
			  struct bt_mesh_msg_ctx *ctx,
			  struct os_mbuf *buf)
{
	BT_DBG("");

	send_attention_status(model, ctx);
}

static void attention_set_unrel(struct bt_mesh_model *model,
				struct bt_mesh_msg_ctx *ctx,
				struct os_mbuf *buf)
{
	u8_t time;

	time = net_buf_simple_pull_u8(buf);

	BT_DBG("%u second%s", time, (time == 1) ? "" : "s");

	bt_mesh_attention(model, time);
}

static void attention_set(struct bt_mesh_model *model,
			  struct bt_mesh_msg_ctx *ctx,
			  struct os_mbuf *buf)
{
	BT_DBG("");

	attention_set_unrel(model, ctx, buf);

	send_attention_status(model, ctx);
}

static void send_health_period_status(struct bt_mesh_model *model,
				      struct bt_mesh_msg_ctx *ctx)
{
	/* Needed size: opcode (2 bytes) + msg + MIC */
	struct os_mbuf *msg = NET_BUF_SIMPLE(2 + 1 + 4);

	bt_mesh_model_msg_init(msg, OP_HEALTH_PERIOD_STATUS);

	net_buf_simple_add_u8(msg, model->pub->period_div);

	if (bt_mesh_model_send(model, ctx, msg, NULL, NULL)) {
		BT_ERR("Unable to send Health Period Status");
	}

	os_mbuf_free_chain(msg);
}

static void health_period_get(struct bt_mesh_model *model,
			      struct bt_mesh_msg_ctx *ctx,
			      struct os_mbuf *buf)
{
	BT_DBG("");

	send_health_period_status(model, ctx);
}

static void health_period_set_unrel(struct bt_mesh_model *model,
				    struct bt_mesh_msg_ctx *ctx,
				    struct os_mbuf *buf)
{
	u8_t period;

	period = net_buf_simple_pull_u8(buf);
	if (period > 15) {
		BT_WARN("Prohibited period value %u", period);
		return;
	}

	BT_DBG("period %u", period);

	model->pub->period_div = period;
}

static void health_period_set(struct bt_mesh_model *model,
			      struct bt_mesh_msg_ctx *ctx,
			      struct os_mbuf *buf)
{
	BT_DBG("");

	health_period_set_unrel(model, ctx, buf);

	send_health_period_status(model, ctx);
}

const struct bt_mesh_model_op bt_mesh_health_srv_op[] = {
	{ OP_HEALTH_FAULT_GET,         2,   health_fault_get },
	{ OP_HEALTH_FAULT_CLEAR,       2,   health_fault_clear },
	{ OP_HEALTH_FAULT_CLEAR_UNREL, 2,   health_fault_clear_unrel },
	{ OP_HEALTH_FAULT_TEST,        3,   health_fault_test },
	{ OP_HEALTH_FAULT_TEST_UNREL,  3,   health_fault_test_unrel },
	{ OP_HEALTH_PERIOD_GET,        0,   health_period_get },
	{ OP_HEALTH_PERIOD_SET,        1,   health_period_set },
	{ OP_HEALTH_PERIOD_SET_UNREL,  1,   health_period_set_unrel },
	{ OP_ATTENTION_GET,            0,   attention_get },
	{ OP_ATTENTION_SET,            1,   attention_set },
	{ OP_ATTENTION_SET_UNREL,      1,   attention_set_unrel },
	BT_MESH_MODEL_OP_END,
};

static int health_pub_update(struct bt_mesh_model *mod)
{
	struct bt_mesh_model_pub *pub = mod->pub;
	size_t count;

	BT_DBG("");

	count = health_get_current(mod, pub->msg);
	if (!count) {
		pub->period_div = 0;
	}

	return 0;
}

int bt_mesh_fault_update(struct bt_mesh_elem *elem)
{
	struct bt_mesh_model *mod;

	mod = bt_mesh_model_find(elem, BT_MESH_MODEL_ID_HEALTH_SRV);
	if (!mod) {
		return -EINVAL;
	}

	return bt_mesh_model_publish(mod);
}

static void attention_off(struct ble_npl_event *work)
{
	struct bt_mesh_health_srv *srv = ble_npl_event_get_arg(work);
	BT_DBG("");

	if (srv->cb && srv->cb->attn_off) {
		srv->cb->attn_off(srv->model);
	}
}

int bt_mesh_health_srv_init(struct bt_mesh_model *model, bool primary)
{
	struct bt_mesh_health_srv *srv = model->user_data;

	if (!srv) {
		if (!primary) {
			return 0;
		}

		BT_ERR("No Health Server context provided");
		return -EINVAL;
	}

	if (!model->pub) {
		BT_ERR("Health Server has no publication support");
		return -EINVAL;
	}

	model->pub->update = health_pub_update;

	k_delayed_work_init(&srv->attn_timer, attention_off);
	k_delayed_work_add_arg(&srv->attn_timer, srv);

	srv->model = model;

	if (primary) {
		health_srv = srv;
	}

	return 0;
}

void bt_mesh_attention(struct bt_mesh_model *model, u8_t time)
{
	struct bt_mesh_health_srv *srv;

	BT_DBG("bt_mesh_attention");
	if (!model) {
		srv = health_srv;
		if (!srv) {
			BT_WARN("No Health Server available");
			return;
		}

		model = srv->model;
	} else {
		srv = model->user_data;
	}

	if (time) {
		if (srv->cb && srv->cb->attn_on) {
			srv->cb->attn_on(model);
		}

		k_delayed_work_submit(&srv->attn_timer, time * 1000);
	} else {
		k_delayed_work_cancel(&srv->attn_timer);

		if (srv->cb && srv->cb->attn_off) {
			srv->cb->attn_off(model);
		}
	}
}
